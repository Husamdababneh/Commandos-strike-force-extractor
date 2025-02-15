/* ========================================================================
   $File: gui.cpp
   $Date: 2025-02-15
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/



#include "gui.h"

#pragma comment(lib,"Kernel32.lib")
#pragma comment(lib,"User32.lib")
#pragma comment(lib,"Gdi32.lib")
#pragma comment(lib,"Dbghelp.lib")
#pragma comment(lib,"Shell32.lib")

#define _CRT_secure_no_warnings 1
#define WIN32_LEAN_AND_MEAN
#define NOCOMM
#include "windows.h"
#include <windowsx.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include "D3DCompiler.h"


// include the Direct3D Library file
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")




// global declarations
IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
ID3D11Device *dev;                     // the pointer to our Direct3D device interface
ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
ID3D11RenderTargetView *backbuffer;    // global declaration

ID3D11InputLayout *pLayout;            // the pointer to the input layout
ID3D11VertexShader *pVS;               // the pointer to the vertex shader
ID3D11PixelShader *pPS;                // the pointer to the pixel shader

ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer


// function prototypes
void InitD3D(HWND hWnd);     // sets up and initializes Direct3D
void InitGraphics(void);    // creates the shape to render
void InitPipeline(void);    // loads and prepares the shaders
void RenderFrame(void);
void CleanD3D(void);         // closes Direct3D and releases memory


b32 running = true;

#define printLastErrorMessage() _printLastErrorMessage(__FILE__, __FUNCTION__, __LINE__);
void _printLastErrorMessage(const char* fileName = "UNKNOWN", const char* function = "UNKNOWN", s64 line = -1);
void setViewPort(HWND hWnd);
void compileResult(HRESULT result, ID3DBlob *errors);
    
LRESULT CALLBACK
MainWindowCallback(HWND   hWindow,
                   UINT   message,
                   WPARAM wParam,
                   LPARAM lParam)
{
    (void)wParam;
    (void)lParam;
    (void)hWindow;
    
    LRESULT result = 0;
    // TODO: Handle each case as documented in win32 documentation since for example,
    //       WM_GETICON must return a handle to the icon
    //       WM_CREATE  must return 0 to continue creaing the window, -1 will destroy the window... etc
    switch(message)
    {
      case WM_CLOSE:
      {
          printf("WM_CLOSE\n");
          running = false;
      }break;
      case WM_KEYDOWN: {
          if (wParam == VK_ESCAPE) {
              running = false;
          }
      }break;
      case WM_DROPFILES: {
          HDROP hDrop = (HDROP)wParam;
          UINT fileCount = DragQueryFileA(hDrop, 0xFFFFFFFF, NULL, 0);
          for (UINT i = 0; i < fileCount; ++i) {
              char filePath[MAX_PATH] = {};
              DragQueryFile(hDrop, i, filePath, MAX_PATH);
              printf("[%d] : %s\n", i, filePath);
          }          
          DragFinish(hDrop);  // Free memory
          
      }break;
      case WM_SIZE: {
          if (wParam != SIZE_MINIMIZED){
              //setViewPort(hWindow);
          }
      }break;
      default:
      {
          result = DefWindowProc(hWindow, message, wParam, lParam);
      }break;
    }
    
    return result;
}

void gui() {


    HMODULE instance = GetModuleHandleA(0);
    WNDCLASS wc      = {0};
    wc.style         = CS_OWNDC | CS_HREDRAW | CS_VREDRAW /*Redraw if size changes*/;
    wc.lpfnWndProc   = MainWindowCallback;
    wc.hInstance     = instance;
    wc.lpszClassName = "windowClassName";
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW); // override WM_SETCURSOR to change the shape of the cursor  


    RegisterClassA(&wc);
    
    DWORD     dwExStyle    = WS_EX_ACCEPTFILES;
    LPCSTR    lpClassName  = "windowClassName";
    LPCSTR    lpWindowName = wc.lpszClassName;
    DWORD     dwStyle      = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
    int       X            = CW_USEDEFAULT;
    int       Y            = CW_USEDEFAULT;
    int       nWidth       = 1920 / 2;
    int       nHeight      = 1080 / 2;
    HWND      hWndParent   = NULL;
    HMENU     hMenu        = NULL;
    HINSTANCE hInstance    = instance;
    LPVOID lpParam         = NULL;
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

    // ShowWindow(window, nCmdShow);


    // set up and initialize Direct3D
    InitD3D(window);
    
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
        RenderFrame();
    }

    CleanD3D();
    
    PostQuitMessage(0);
    DestroyWindow(window);
    return;
    
}


// this is the function used to render a single frame
void RenderFrame(void)
{
    // clear the back buffer to a deep blue
    const Vec4f color = {
        .r = 0.0f,
        .g = 0.2f,
        .b = 0.4f,
        .a = 1.0f
    };

   
    devcon->ClearRenderTargetView(backbuffer, color.data);
    
    // do 3D rendering on the back buffer here
    // select which vertex buffer to display
    UINT stride = sizeof(Vertex);
    UINT offset = 0;
    devcon->IASetVertexBuffers(0, 1, &pVBuffer, &stride, &offset);

    // select which primtive type we are using
    devcon->IASetPrimitiveTopology(D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

    // draw the vertex buffer to the back buffer
    devcon->Draw(3, 0);
    // switch the back buffer and the front buffer
    swapchain->Present(0, 0);
}

// this function initializes and prepares Direct3D for use
void InitD3D(HWND hWnd)
{
    // create a struct to hold information about the swap chain
    DXGI_SWAP_CHAIN_DESC scd;

    // clear out the struct for use
    ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

    // fill the swap chain description struct
    scd.BufferCount = 1;                                    // one back buffer
    scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;     // use 32-bit color
    scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;      // how swap chain is to be used
    scd.OutputWindow = hWnd;                                // the window to be used
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
                                  &swapchain,
                                  &dev,
                                  NULL,
                                  &devcon);
    // TODO: something might be wrong here 
    // printLastErrorMessage();
    
    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    
    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release();
    
    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);

    
    // Rect lpRect;
    // GetWindowRect(hWnd, &lpRect);
    
    // auto width = lpRect.right - lpRect.left;
    // auto hieght = lpRect.bottom - lpRect.top;
    
    // D3D11_VIEWPORT viewport = {};
    
    // viewport.TopLeftX = 0;
    // viewport.TopLeftY = 0;
    // viewport.Width = (float)width;
    // viewport.Height = (float)hieght;
    // viewport.MinDepth = 0.0f;
    // viewport.MaxDepth = 1.0f;
    
    // devcon->RSSetViewports(1, &viewport);
    setViewPort(hWnd);
    InitGraphics();
    InitPipeline();
}

void setViewPort(HWND hWnd) {
    if (!devcon) return;

    RECT lpRect;
    GetWindowRect(hWnd, &lpRect);
    
    auto width = lpRect.right - lpRect.left;
    auto hieght = lpRect.bottom - lpRect.top;
    
    
    // Release buffers that depends on the swap chain
    backbuffer->Release();

    // Resize SwapChain buffers
    swapchain->ResizeBuffers(0, width, hieght, DXGI_FORMAT_UNKNOWN, 0);
    
    // get the address of the back buffer
    ID3D11Texture2D *pBackBuffer;
    swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
    
    // use the back buffer address to create the render target
    dev->CreateRenderTargetView(pBackBuffer, NULL, &backbuffer);
    pBackBuffer->Release(); 
    
    
    D3D11_VIEWPORT viewport = {};
    viewport.TopLeftX = 0;
    viewport.TopLeftY = 0;
    viewport.Width = (float)width;
    viewport.Height = (float)hieght;
    viewport.MinDepth = 0.0f;
    viewport.MaxDepth = 1.0f;

    
    devcon->RSSetViewports(1, &viewport);
    // set the render target as the back buffer
    devcon->OMSetRenderTargets(1, &backbuffer, NULL);
}

// this is the function that cleans up Direct3D and COM
void CleanD3D()
{
    // close and release all existing COM objects
    pVS->Release();
    pPS->Release();
    swapchain->Release();
    backbuffer->Release();
    dev->Release();
    devcon->Release();
}


// this is the function that creates the shape to render
void InitGraphics()
{
    // create a triangle using the VERTEX struct
    Vertex OurVertices[] =
    {
        {0.0f, 0.5f, 0.0f, Vec4f{1.0f, 0.0f, 0.0f, 1.0f}},
        {0.45f, -0.5, 0.0f, Vec4f{0.0f, 1.0f, 0.0f, 1.0f}},
        {-0.45f, -0.5f, 0.0f, Vec4f{0.0f, 0.0f, 1.0f, 1.0f}}
    };


    // create the vertex buffer
    D3D11_BUFFER_DESC bd;
    ZeroMemory(&bd, sizeof(bd));

    bd.Usage = D3D11_USAGE_DYNAMIC;                // write access access by CPU and GPU
    bd.ByteWidth = sizeof(Vertex) * 3;             // size is the VERTEX struct * 3
    bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;       // use as a vertex buffer
    bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;    // allow CPU to write in buffer

    dev->CreateBuffer(&bd, NULL, &pVBuffer);       // create the buffer


    // copy the vertices into the buffer
    D3D11_MAPPED_SUBRESOURCE ms;
    devcon->Map(pVBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);    // map the buffer
    memcpy(ms.pData, OurVertices, sizeof(OurVertices));                 // copy the data
    devcon->Unmap(pVBuffer, NULL);                                      // unmap the buffer
}


// this function loads and prepares the shaders
void InitPipeline()
{
    void* filedata;
    u64 size = read_entire_file("shader.hlsl", &filedata);
    if (!size){
		printf("Couldn't read shader.hlsl\n");
        ExitProcess(-1);
	}

    char* shaderSrc = (char*)filedata;
    auto shaderCompileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
    // load and compile the two shaders
    ID3DBlob *VS, *PS, *errors;
    HRESULT vsCompileResult =
        // D3DCompile(shaderSrc, size, 0, 0, 0, "VShader", "vs_5_0", shaderCompileFlags, 0, &VS, &errors);
        D3DCompileFromFile(L"shader.hlsl", 0, 0, "VShader", "vs_5_0", shaderCompileFlags, 0, &VS, &errors);


    compileResult(vsCompileResult, errors);

    
    HRESULT psCompileResult =
        // D3DCompile(shaderSrc, size, 0, 0, 0, "PShader", "ps_5_0", shaderCompileFlags, 0, &PS, &errors);
        D3DCompileFromFile(L"shader.hlsl", 0, 0, "PShader", "ps_5_0", shaderCompileFlags, 0, &PS, &errors);
    
    compileResult(vsCompileResult, errors);

    // encapsulate both shaders into shader objects
    dev->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &pVS);
    dev->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &pPS);

    // set the shader objects
    devcon->VSSetShader(pVS, 0, 0);
    devcon->PSSetShader(pPS, 0, 0);

    // create the input layout object
    D3D11_INPUT_ELEMENT_DESC ied[] =
    {
        {"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
        {"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
    };

    dev->CreateInputLayout(ied, 2, VS->GetBufferPointer(), VS->GetBufferSize(), &pLayout);
    devcon->IASetInputLayout(pLayout);
}


void _printLastErrorMessage(const char* fileName, const char* function, s64 line) {
    DWORD error = GetLastError();
    if (error == 0) {
        printf("No error.\n");
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
