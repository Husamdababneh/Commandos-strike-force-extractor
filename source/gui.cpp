/* ========================================================================
   $File: gui.cpp
   $Date: 2025-02-15
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/



#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Winmm.lib")

#define _CRT_secure_no_warnings 1
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include "windows.h"
#include <windowsx.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "D3DCompiler.h"
#include "dxgi1_6.h"
// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3dcompiler.lib")

#include "icon.cpp"
#include "gui.h"

static constexpr u64 MAX_FILE_COUNT = 20;
global char GlobalDroppedFile[MAX_PATH];
global b32 GlobalDroppedFileProcessed = true;
// function prototypes
void InitD3D(WindowContext*);     // sets up and initializes Direct3D
void CleanD3D(WindowContext*);         // closes Direct3D and releases memory
void setViewPort(WindowContext*);

void InitPipeline(WindowContext*);    // loads and prepares the shaders
void RenderFrame(WindowContext*);

Ui InitGraphics(WindowContext*);    // creates the shape to render
void RenderUi(WindowContext*, Ui*);
u32 getWindowRefreshRate();

b32 running = true;

#define printLastErrorMessage() _printLastErrorMessage(__FILE__, __FUNCTION__, __LINE__);
void _printLastErrorMessage(const char* fileName = "UNKNOWN", const char* function = "UNKNOWN", s64 line = -1);
void compileResult(HRESULT result, ID3DBlob *errors);
    
LRESULT CALLBACK
MainWindowCallback(HWND   hWindow,
                   UINT   message,
                   WPARAM wParam,
                   LPARAM lParam)
{

    WindowContext* windowContext = (WindowContext*)GetWindowLongPtrA(hWindow, GWLP_USERDATA);
    
    LRESULT result = 0;
    // TODO: Handle each case as documented in win32 documentation since for example,
    //       WM_GETICON must return a handle to the icon
    //       WM_CREATE  must return 0 to continue creaing the window, -1 will destroy the window... etc

    // https://blog.getpaint.net/2017/08/12/win32-how-to-get-the-refresh-rate-for-a-window/
    // TODO: Handle WM_DISPLAYCHANGE to detect if the refresh rate or resolution changes
    switch(message)
    {
      case WM_CLOSE:
      {
          printf("WM_CLOSE\n");
          running = false;
      }break;
      case WM_KEYDOWN:
      {
          // TODO: What is the best way to express that a button is pressed
          if (wParam == VK_ESCAPE) running = false;
      }break;
      case WM_DROPFILES:
      {
          if (!windowContext) break;
          HDROP hDrop = (HDROP)wParam;
          UINT fileCount = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);
          u32 count = min(fileCount, MAX_FILE_COUNT);
          if(fileCount > MAX_FILE_COUNT) MessageBoxA(hWindow, "We can only handle 20 files", "Warning", MB_OK);
          if (count > 1) {
              printf("TODO: Handle More than 1 file\n");
              MessageBoxA(hWindow, "We can only handle 1 file now", "Warning", MB_OK);
              return 0;
          }
          printf("Received WM_DROPFILES message\n");
          
          for (UINT i = 0; i < count; ++i) {
              DragQueryFile(hDrop, i, GlobalDroppedFile, MAX_PATH);
              printf("[%d] : %s\n", i, GlobalDroppedFile);
              GlobalDroppedFileProcessed = false;
          }
          DragFinish(hDrop);  // Free memory
          return 0;
      }break;
      case WM_MOUSEMOVE:
      {
          if (windowContext) {
              windowContext->mousePosX = GET_X_LPARAM(lParam);
              windowContext->mousePosY = GET_Y_LPARAM(lParam);
          };
      }break;
      case WM_SIZE:
      {
          if (wParam != SIZE_MINIMIZED && windowContext){
              setViewPort(windowContext);
          }
      }break;
      default:
      {
          result = DefWindowProc(hWindow, message, wParam, lParam);
      }break;
    }
    
    return result;
}

global u64 PERFORMANCE_COUNT_FREQUENCY;

internal inline LARGE_INTEGER
Win32GetWallClock(void) {
    LARGE_INTEGER result;
    QueryPerformanceCounter(&result);
    return result;
}

internal inline f32
Win32GetSecondsElapsed(LARGE_INTEGER start, LARGE_INTEGER end) {
    return (end.QuadPart - start.QuadPart) / (f32)PERFORMANCE_COUNT_FREQUENCY;
}

static
Arena* arena_alloc(u64 size) {
    u64 newSize = size + sizeof(Arena);
    Arena* arena = (Arena*)VirtualAlloc(NULL,
                                        (SIZE_T)size, 
                                        MEM_COMMIT,
                                        PAGE_READWRITE);
    if (!arena){
        printLastErrorMessage();
        ExitProcess(-1);
    }
    arena->base = (u8*)arena + sizeof(Arena);
    arena->current = arena->base;
    arena->size = size;
    return arena;
}

void* arena_push_size(Arena* arena, u64 size) {
    //TODO: 
    void* result = arena->current;
    arena->current = (u8*)arena->current + size;
    return result;
};

static
void arena_reset(Arena* arena) {
    arena->current = arena->base;
}

static
void arena_destory(Arena* arena) {
    BOOL success = VirtualFree((u8*)arena->base - sizeof(Arena),
                               0,
                               MEM_RELEASE);
}

void gui() {

    // Create A console to direct the stdout to it
    /*
    if(!AllocConsole()){
        printLastErrorMessage();
        ExitProcess(-1);
    }
    */
    LARGE_INTEGER perfCountFreq;
    QueryPerformanceFrequency(&perfCountFreq); // how many counts per secound
    PERFORMANCE_COUNT_FREQUENCY = perfCountFreq.QuadPart;

    b32 CanSleep = timeBeginPeriod(1) == TIMERR_NOERROR;

    constexpr const char* WINDOW_CLASS_NAME = "windowClassName";

    
    HMODULE instance = GetModuleHandleA(0);

    HICON hIcon = CreateIcon(instance,        // application instance  
                             32,              // icon width 
                             32,              // icon height 
                             1,               // number of XOR planes 
                             1,               // number of bits per pixel 
                             ANDmaskIcon,     // AND bitmask  
                             XORmaskIcon);    // XOR bitmask
    
    WNDCLASSEXA wc;
    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW /*Redraw if size changes*/;
    wc.lpfnWndProc   = MainWindowCallback;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = instance;
    wc.hIcon         = hIcon;
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // override WM_SETCURSOR to change the shape of the cursor
    wc.hbrBackground = NULL;
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = WINDOW_CLASS_NAME;
    wc.hIconSm       = hIcon;
    
    RegisterClassExA(&wc);


    
    DWORD     dwExStyle    = WS_EX_ACCEPTFILES;
    LPCSTR    lpClassName  = WINDOW_CLASS_NAME;
    LPCSTR    lpWindowName = "Three Way Deadlock";
    DWORD     dwStyle      = WS_OVERLAPPEDWINDOW; // WS_OVERLAPPEDWINDOW | WS_POPUP
    int       X            = CW_USEDEFAULT;
    int       Y            = CW_USEDEFAULT;
    int       nWidth       = 1920 / 2;
    int       nHeight      = 1080 / 2;
    HWND      hWndParent   = NULL;
    HMENU     hMenu        = NULL;
    HINSTANCE hInstance    = instance;
    LPVOID    lpParam      = NULL;
    HWND window = CreateWindowExA(dwExStyle,
                                  lpClassName,
                                  lpWindowName,
                                  dwStyle,
                                  X,
                                  Y,
                                  nWidth,
                                  nHeight,
                                  hWndParent,
                                  hMenu,
                                  hInstance,
                                  lpParam);

    if(!window)
    {
        printLastErrorMessage();
        ExitProcess(0);
    }


    WindowContext context = {};
    context.window       = window;
    context.arena        = arena_alloc(16_MB);
    context.width        = nWidth;
    context.height       = nHeight;


    
    
    SetWindowLongPtrA(window, GWLP_USERDATA, (LONG_PTR)&context);
    DragAcceptFiles(window, TRUE);
    ShowWindow(window, SW_SHOW);

    // set up and initialize Direct3D
    InitD3D(&context);

    Ui ui = InitGraphics(&context);
    // PushRect(&ui, { -0.45f, 0.5f, 0.0f }, { 0.9f, 1.0f });
    PushRect(&ui, {  100.0f,  100.0f, 0.0f }, { 100.0, 100.0f });
    PushRect(&ui, {  125.0f,  360.0f, 0.0f }, { 100.f, 100.f });
    

    const Vec4f backgroundColor = {
        .r = 0.117f,
        .g = 0.117f,
        .b = 0.117f,
        .a = 1.0f
    };


    LARGE_INTEGER LastCounter = Win32GetWallClock();


    // u64 LastCycleCount = __rdtsc();

    u32 MonitorRefreshHz = getWindowRefreshRate();
    u32 GameUpdateHz = MonitorRefreshHz;
    f32 TargetSecondsPerFrame = 1.0f / (f32)GameUpdateHz;
    
    u32 frameCount = 0;
    while(running) {
        MSG message;
        while(PeekMessageA(&message, window, 0, 0, PM_REMOVE))
        {
            if (message.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&message); 
            DispatchMessage(&message);
        }
        
        // this is for profiling 
        // u64 EndCycleCount = __rdtsc();

        LARGE_INTEGER WorkCounter = Win32GetWallClock();

        // u64 cyclesElapsed  = EndCycleCount - LastCycleCount;
        // LastCycleCount = EndCycleCount;
        
        f64 WorkSecondsElapsed = Win32GetSecondsElapsed(WorkCounter, LastCounter);

        f64 SecondsElapsedForFrame = WorkSecondsElapsed;
        if (SecondsElapsedForFrame < TargetSecondsPerFrame) {
            while(SecondsElapsedForFrame < TargetSecondsPerFrame){
                // TODO: Find a better way to wait, this consumes CPU for no reason at all
                // Naive way to conpensate the time of contex switching 
                constexpr f32 SECONDS_TO_MILLISECONDS = 1000.f;
                DWORD timeToSleep = (DWORD)(SECONDS_TO_MILLISECONDS *
                                            (TargetSecondsPerFrame - SecondsElapsedForFrame));
                if (CanSleep) Sleep(timeToSleep);
                SecondsElapsedForFrame = Win32GetSecondsElapsed(LastCounter,
                                                                Win32GetWallClock());
            }
        }else {
            printf("We missed a frame\n");
        }


        if (!GlobalDroppedFileProcessed) {
            PakFile pakFile = ParsePakFile(GlobalDroppedFile);
            for (u32 i = 0; i < pakFile.pak->ResNum; ++i) {
                printf("File [%d]:[%s]\n", pakFile.files[i].size, pakFile.files[i].filename);
            }

            delete[] pakFile.files;
            delete[] pakFile.dataToBeFreed;
            GlobalDroppedFileProcessed = true;
        }
       
        // Background Color   
        context.devcon->ClearRenderTargetView(context.backbuffer, backgroundColor.data);                
       
        // RenderFrame(&context);
        RenderUi(&context, &ui);
        
       
        // Swap buffers
        context.swapchain->Present(0, 0);

        LastCounter = Win32GetWallClock();
   }

    CleanD3D(&context);
    
    PostQuitMessage(0);
    DestroyWindow(window);
    // BOOL WINAPI FreeConsole(void);
    return;
    
}


// this is the function used to render a single frame
void RenderUi(WindowContext* context, Ui* ui)
{
    // Update UI data

    UiRect* rects = (UiRect*)ui->arena->base;
    for(u32 i = 0; i < ui->rectsCount; ++i) {
        // UiRect& rect = ui->rects[i];
        UiRect& rect = rects[i];
        
        f32 x0 = rect.pos.x;
        f32 x1 = rect.pos.x + rect.size.x;

        f32 y0 = rect.pos.y;
        f32 y1 = rect.pos.y + rect.size.y;
        
        if (context->mousePosX >= x0 && context->mousePosX <= x1 &&
            context->mousePosY >= y0 && context->mousePosY <= y1) {
            rect.isSelected = true;
        } else {
            rect.isSelected = false;
        }            
    };
    
    // copy the vertices into the buffer
    // this really should happen when we decide to draw
    D3D11_MAPPED_SUBRESOURCE ms;
    context->devcon->Map(ui->pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, rects, sizeof(UiRect) * ui->rectsCount);                 // copy the data
    context->devcon->Unmap(ui->pVBuffer, NULL);                                      // unmap the buffer


    ScreenData screenData;
    screenData.width  = (f32)context->width;
    screenData.height = (f32)context->height;
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->devcon->Map(ui->screenBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &screenData, sizeof(Vec2f));
    context->devcon->Unmap(ui->screenBuffer, 0);
    
    // Update screen buffer if screen size changes
    
    // do 3D rendering on the back buffer here
    // select which vertex buffer to display
    UINT stride = sizeof(UiRect);
    UINT offset = 0;
    context->devcon->IASetVertexBuffers(0, 1, &ui->pVBuffer, &stride, &offset);
    
    // Set shaders
    context->devcon->VSSetConstantBuffers(0, 1, &ui->screenBuffer);
    context->devcon->VSSetShader(ui->pVS, 0, 0);
    context->devcon->GSSetConstantBuffers(0, 1, &ui->screenBuffer);
    context->devcon->GSSetShader(ui->pGS, 0, 0);
    context->devcon->PSSetShader(ui->pPS, 0, 0);
    
    // Set layout
    context->devcon->IASetInputLayout(ui->pLayout);    

    // select which primtive type we are using
    context->devcon->IASetPrimitiveTopology(ui->topology);

    // draw the vertex buffer to the back buffer
    context->devcon->DrawInstanced(4, ui->rectsCount, 0, 0);
}

// this function initializes and prepares Direct3D for use
void InitD3D(WindowContext* context)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = context->window;                     // the window to be used
    scd.SampleDesc.Count = 4;                               // how many multisamples
    scd.Windowed = TRUE;                                    // windowed/full-screen mode

    // create a device, device context and swap chain using the information in the scd struct
    D3D11CreateDeviceAndSwapChain(NULL,
                                  D3D_DRIVER_TYPE_HARDWARE, 
                                  NULL,
                                  D3D11_CREATE_DEVICE_DEBUG,
                                  NULL,
                                  NULL,
                                  D3D11_SDK_VERSION,
                                  &scd,
                                  &context->swapchain,
                                  &context->dev,
                                  NULL,
                                  &context->devcon);
    // TODO: something might be wrong here 
    // printLastErrorMessage();
    
    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    context->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    
    // use the back buffer address to create the render target
    context->dev->CreateRenderTargetView(pBackBuffer, NULL, &context->backbuffer);
    pBackBuffer->Release();
    
    // set the render target as the back buffer
    context->devcon->OMSetRenderTargets(1, &context->backbuffer, NULL);

    setViewPort(context);
}

void setViewPort(WindowContext* context) {
    if (!context->devcon) return;

    RECT lpRect;
    GetClientRect(context->window, &lpRect);
    
    auto width = lpRect.right - lpRect.left;
    auto height = lpRect.bottom - lpRect.top;
    
    context->width = width;
    context->height = height;
    
    // Release buffers that depends on the swap chain
    context->backbuffer->Release();

    // Resize SwapChain buffers
    context->swapchain->ResizeBuffers(0, width, height, DXGI_FORMAT_UNKNOWN, 0);
    
    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    context->swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    
    // use the back buffer address to create the render target
    context->dev->CreateRenderTargetView(pBackBuffer, NULL, &context->backbuffer);
    pBackBuffer->Release(); 
    
    
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)height;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    
    context->devcon->RSSetViewports(1, &viewport);
    // set the render target as the back buffer
    context->devcon->OMSetRenderTargets(1, &context->backbuffer, NULL);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D(WindowContext* context)
{
    // close and release all existing COM objects
    // pVS->Release();
    // pPS->Release();
    
#define ReleaseObject(x) if (x) x->Release();
    if (context) {
        ReleaseObject(context->swapchain);
        ReleaseObject(context->backbuffer);
        ReleaseObject(context->dev);
        ReleaseObject(context->devcon);
    }
}


void PushRect(Ui* ui, Vec3f pos, Vec2f size) {

    UiRect* rect = ArenaPushType(ui->arena, UiRect);
    rect->pos   = pos;
    rect->size  = size;
    // ui->rects[ui->rectsCount] = rect;
    ui->rectsCount++;
}

// this is the function that creates the shape to render
Ui InitGraphics(WindowContext* context) {
    Ui ui = {};
    ui.maxRectsCount = 2000;
    ui.arena = arena_alloc(sizeof(UiRect) * ui.maxRectsCount);
    // ui.rects = new UiRect[ui.maxRectsCount];
    
    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(UiRect) * ui.maxRectsCount;
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    context->dev->CreateBuffer(&bd, NULL, &ui.pVBuffer);       // create the buffer

    
    ScreenData screenData;
    screenData.width  = (f32)context->width;
    screenData.height = (f32)context->height;

    D3D11_BUFFER_DESC cbDesc = {};
    cbDesc.Usage = D3D11_USAGE_DYNAMIC;
    cbDesc.ByteWidth = sizeof(ScreenData);
    cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
    cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
    cbDesc.MiscFlags = 0;

  
    context->dev->CreateBuffer(&cbDesc, NULL, &ui.screenBuffer);
    
    D3D11_MAPPED_SUBRESOURCE mappedResource;
    context->devcon->Map(ui.screenBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    memcpy(mappedResource.pData, &screenData, sizeof(Vec2f));
    context->devcon->Unmap(ui.screenBuffer, 0);


    
    ui.topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP;
    
    void* filedata;
    u64 size = read_entire_file("./shader.hlsl", &filedata);
    if (!size){
		printf("Couldn't read shader.hlsl\n");
        ExitProcess(-1);
	}

    char* shaderSrc = (char*)filedata;
    auto shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    // load and compile the two shaders
    ID3DBlob *VS, *GS, *PS, *errors;
    HRESULT vsCompileResult =
        D3DCompileFromFile(L"shader.hlsl", 0, 0, "VS_Main", "vs_5_0", shaderCompileFlags, 0, &VS, &errors);

    compileResult(vsCompileResult, errors);

    HRESULT gsCompileResult =
        D3DCompileFromFile(L"shader.hlsl", 0, 0, "GS_Main", "gs_5_0", shaderCompileFlags, 0, &GS, &errors);
    
    compileResult(gsCompileResult, errors);
    
    HRESULT psCompileResult =
        D3DCompileFromFile(L"shader.hlsl", 0, 0, "PS_Main", "ps_5_0", shaderCompileFlags, 0, &PS, &errors);
    
    compileResult(psCompileResult, errors);

    // encapsulate both shaders into shader objects
    context->dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &ui.pVS);
    context->dev->CreateGeometryShader(GS->GetBufferPointer(), GS->GetBufferSize(), NULL, &ui.pGS);
    context->dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &ui.pPS);

    // set the shader objects
    context->devcon->VSSetShader(ui.pVS, 0, 0);
    context->devcon->GSSetShader(ui.pGS, 0, 0);
    context->devcon->PSSetShader(ui.pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC layout[] = {
        { "POS",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0,  0, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "SIZE", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 12, D3D11_INPUT_PER_INSTANCE_DATA, 1 },
        { "SELECTED", 0, DXGI_FORMAT_R8_UINT,     0, 20, D3D11_INPUT_PER_INSTANCE_DATA, 1 }
    };
    
    context->dev->CreateInputLayout(layout,
                                    sizeof(layout)/sizeof(D3D11_INPUT_ELEMENT_DESC),
                                    VS->GetBufferPointer(),
                                    VS->GetBufferSize(),
                                    &ui.pLayout);
    context->devcon->IASetInputLayout(ui.pLayout);

    delete[] filedata;
    return ui;
}



void _printLastErrorMessage(const char* fileName, const char* function, s64 line) {
    DWORD error = GetLastError();
    if (error == 0) {
        // printf("No error.\n");
        return;
    }

    char buffer[512];  // Buffer for the formatted message

    DWORD size = FormatMessageA(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        error,
        0,  // Default language
        buffer,
        sizeof(buffer),
        NULL
    );

    printf("ERROR LOG [%s], [%s:%lld]\n   ", fileName, function, line);
    if (size > 0) {
        printf("Error %lu: %s\n", error, buffer);
    } else {
        printf("Failed to retrieve error message.\n");
    }
}

void compileResult(HRESULT result, ID3DBlob *errors) {
    if (S_OK != result) {
        printf("Could not compile shader\n");
        if (errors) {
            printf("%s\n", (char*)errors->GetBufferPointer());
        }
        ExitProcess(-1);
    }
}


u32 getWindowRefreshRate() {
    u32 result;
#if 0
    DXGI_MODE_DESC modeDesc = {};
    UINT numModes = 0;
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, nullptr);
    DXGI_MODE_DESC* pModes = new DXGI_MODE_DESC[numModes];
    pOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, 0, &numModes, pModes);

    for (u32 i = 0; i < numModes; ++i){
        auto& mode = pModes[i];
        printf("Mode[%d]: Width: %d, Height: %d, %d Hz\n",
               i,
               mode.Width,
               mode.Height,
               mode.RefreshRate.Numerator / mode.RefreshRate.Denominator);
        
    }

    result = pModes[numModes - 1].RefreshRate.Numerator / pModes[numModes - 1].RefreshRate.Denominator;
    delete[] pModes;
#elif 0
    // Check for failures
    DXGI_OUTPUT_DESC outputDesc;
    pOutput->GetDesc(&outputDesc);

    HMONITOR monitor = outputDesc.Monitor;

    MONITORINFOEXA monitorInfo;
    monitorInfo.cbSize  = sizeof(MONITORINFOEXA);
    GetMonitorInfoA(monitor, &monitorInfo);
    
    DEVMODEA currentMode = {};
    currentMode.dmSize = sizeof(DEVMODEA);
    currentMode.dmDriverExtra = 0;
    EnumDisplaySettingsA(monitorInfo.szDevice,
                         ENUM_CURRENT_SETTINGS,
                         &currentMode);
    
    
    // Query IDXGIOutput1 for FindClosestMatchingMode1
    IDXGIOutput1* pOutput1 = nullptr;
    auto win32Result = pOutput->QueryInterface(__uuidof(IDXGIOutput1), (void**)&pOutput1);
    if (SUCCEEDED(win32Result)) {
        
        b32 useDefaultRefreshRate = 1 == currentMode.dmDisplayFrequency || 0 == currentMode.dmDisplayFrequency;
        DXGI_MODE_DESC1 modeToMatch = {};
        modeToMatch.Width                   = currentMode.dmPelsWidth;
        modeToMatch.Height                  = currentMode.dmPelsHeight;
        modeToMatch.RefreshRate.Numerator   = useDefaultRefreshRate ? 0 : currentMode.dmDisplayFrequency;
        modeToMatch.RefreshRate.Denominator = useDefaultRefreshRate ? 0 : 1;
        modeToMatch.Format                  = DXGI_FORMAT_R32G32B32A32_FLOAT ;  // Must specify a valid format
        modeToMatch.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
        modeToMatch.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;
        
        DXGI_MODE_DESC1 currentMode;
        win32Result = pOutput1->FindClosestMatchingMode1(nullptr, &currentMode, nullptr);
        if (SUCCEEDED(win32Result)) {
            printf("Current Monitor Refresh Rate: %d Hz\n", 
                currentMode.RefreshRate.Numerator / currentMode.RefreshRate.Denominator);
        } else {
            printLastErrorMessage();
            printf("Failed to retrieve the current display mode.\n");
        }
        pOutput1->Release();
    } else {
        printf("IDXGIOutput1 not supported, falling back to DXGI 1.0.\n");
    }
    
    // result = outputDesc.RefreshRate.Numerator / outputDesc.RefreshRate.Denominator;

#else
    
    IDXGIFactory* pFactory = nullptr;
    if (S_OK != CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&pFactory)) {
        printLastErrorMessage();
        ExitProcess(-1);
    }

    IDXGIAdapter* pAdapter = nullptr;
    pFactory->EnumAdapters(0, &pAdapter);

    IDXGIOutput* pOutput = nullptr;
    pAdapter->EnumOutputs(0, &pOutput);
    
    DXGI_OUTPUT_DESC outputDesc;
    pOutput->GetDesc(&outputDesc);

    HMONITOR monitor = outputDesc.Monitor;

    MONITORINFOEXA monitorInfo;
    monitorInfo.cbSize  = sizeof(MONITORINFOEXA);
    GetMonitorInfoA(monitor, &monitorInfo);
    
    DEVMODEA currentMode = {};
    currentMode.dmSize = sizeof(DEVMODEA);
    currentMode.dmDriverExtra = 0;
    
    EnumDisplaySettingsA(monitorInfo.szDevice,
                         ENUM_CURRENT_SETTINGS,
                         &currentMode);
    
    result = currentMode.dmDisplayFrequency;
#endif
        
    pOutput->Release();
    pAdapter->Release();
    pFactory->Release();
    return result;
}






