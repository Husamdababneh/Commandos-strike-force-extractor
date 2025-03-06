/* ========================================================================
   $File: gui.h
   $Date: 2025-02-15
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/
#pragma once


global const char* shader = R"(
struct VOut{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VOut VertexShader(float4 position : POSITION, float4 color : COLOR)
{
    VOut output;

    output.position = position;
    output.color = color;

    return output;
}


float4 PShader(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    return color;
}

)";


// global 


struct Vec2f {
    union {
        struct { float x,y; };
        struct { float u,v; };
        float data[2];
    };
};


struct Vec3f {
    union {
        struct { float x,y,z; };
        struct { float r,g,b; };
        float data[3];
    };
};


struct Vec4f {
    union {
        struct { float x,y,z,w; };
        struct { float r,g,b,a; };
        float data[4];
    };
};

struct Vertex {
    float x, y, z;
    Vec4f color;
};


struct UiVertex {
    float x, y, z;
    Vec4f color;
};

struct UiRect {
    Vec3f pos;
    Vec2f size;
    bool isSelected;
};

struct ScreenData {
    f32 width, height;
    float padding[2]; // Ensure 16-byte alignment for directx constant buffer requierment
};

struct Arena {
    void* base;
    void* current;
    u64 size;
};

#define ArenaPush(arena, size) arena_push_size(arena, size)
#define ArenaPushType(arena, type) (type*)arena_push_size(arena, sizeof(type))
void* arena_push_size(Arena* arena, u64 size);
void arena_reset(Arena* arena);
    
struct Ui {
    Arena* arena;
    // UiRect* rects;
    u32 rectsCount;
    u32 maxRectsCount;
    
    ID3D11Buffer* vertexBuffer;
    u32 maxVertexCount;
    u32 currentVertexCount;
    
    ID3D11Buffer* indexBuffer;
    u32 maxIndexCount;
    u32 currentIndexCount;
    
    ID3D11Buffer *pVBuffer;                // the pointer to the vertex buffer
    ID3D11Buffer *screenBuffer;            // the pointer to the screen data constant buffer
    ID3D11InputLayout *pLayout;            // the pointer to the input layout
    ID3D11VertexShader *pVS;               // the pointer to the vertex shader
    ID3D11GeometryShader *pGS;             // the pointer to the pixel shader
    ID3D11PixelShader *pPS;                // the pointer to the pixel shader

    D3D11_PRIMITIVE_TOPOLOGY topology;    
};
void PushRect(Ui* ui, Vec3f pos, Vec2f size);


struct DroppedFiles {
    Arena* arena;
    u64 capacity;
    u64 count;
    StringView* sv;
};
void init_dropped_files(u64 capacity, DroppedFiles* result);
    
struct WindowContext {
    HWND window;
    Arena* arena;

    IDXGISwapChain *swapchain;             // the pointer to the swap chain interface
    ID3D11Device *dev;                     // the pointer to our Direct3D device interface
    ID3D11DeviceContext *devcon;           // the pointer to our Direct3D device context
    ID3D11RenderTargetView *backbuffer;    // global declaration
    
    u32 mousePosX;
    u32 mousePosY;

    u32 width;
    u32 height;

};


#define DXGIFormatCase(x) case x: return #x;

global const char* DXGIFormatToString(DXGI_FORMAT format) {
    switch (format) {
        DXGIFormatCase(DXGI_FORMAT_UNKNOWN)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32A32_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32A32_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32A32_UINT)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32A32_SINT)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32_UINT)
        DXGIFormatCase(DXGI_FORMAT_R32G32B32_SINT)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_UINT)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R16G16B16A16_SINT)
        DXGIFormatCase(DXGI_FORMAT_R32G32_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R32G32_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R32G32_UINT)
        DXGIFormatCase(DXGI_FORMAT_R32G32_SINT)
        DXGIFormatCase(DXGI_FORMAT_R32G8X24_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_D32_FLOAT_S8X24_UINT)
        DXGIFormatCase(DXGI_FORMAT_R32_FLOAT_X8X24_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_X32_TYPELESS_G8X24_UINT)
        DXGIFormatCase(DXGI_FORMAT_R10G10B10A2_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R10G10B10A2_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R10G10B10A2_UINT)
        DXGIFormatCase(DXGI_FORMAT_R11G11B10_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_UINT)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R8G8B8A8_SINT)
        DXGIFormatCase(DXGI_FORMAT_R16G16_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R16G16_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R16G16_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R16G16_UINT)
        DXGIFormatCase(DXGI_FORMAT_R16G16_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R16G16_SINT)
        DXGIFormatCase(DXGI_FORMAT_R32_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_D32_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R32_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_R32_UINT)
        DXGIFormatCase(DXGI_FORMAT_R32_SINT)
        DXGIFormatCase(DXGI_FORMAT_R24G8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_D24_UNORM_S8_UINT)
        DXGIFormatCase(DXGI_FORMAT_R24_UNORM_X8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_X24_TYPELESS_G8_UINT)
        DXGIFormatCase(DXGI_FORMAT_R8G8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R8G8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R8G8_UINT)
        DXGIFormatCase(DXGI_FORMAT_R8G8_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R8G8_SINT)
        DXGIFormatCase(DXGI_FORMAT_R16_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R16_FLOAT)
        DXGIFormatCase(DXGI_FORMAT_D16_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R16_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R16_UINT)
        DXGIFormatCase(DXGI_FORMAT_R16_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R16_SINT)
        DXGIFormatCase(DXGI_FORMAT_R8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_R8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R8_UINT)
        DXGIFormatCase(DXGI_FORMAT_R8_SNORM)
        DXGIFormatCase(DXGI_FORMAT_R8_SINT)
        DXGIFormatCase(DXGI_FORMAT_A8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R1_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R9G9B9E5_SHAREDEXP)
        DXGIFormatCase(DXGI_FORMAT_R8G8_B8G8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_G8R8_G8B8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC1_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC1_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC1_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_BC2_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC2_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC2_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_BC3_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC3_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC3_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_BC4_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC4_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC4_SNORM)
        DXGIFormatCase(DXGI_FORMAT_BC5_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC5_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC5_SNORM)
        DXGIFormatCase(DXGI_FORMAT_B5G6R5_UNORM)
        DXGIFormatCase(DXGI_FORMAT_B5G5R5A1_UNORM)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8A8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8X8_UNORM)
        DXGIFormatCase(DXGI_FORMAT_R10G10B10_XR_BIAS_A2_UNORM)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8A8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8A8_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8X8_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_B8G8R8X8_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_BC6H_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC6H_UF16)
        DXGIFormatCase(DXGI_FORMAT_BC6H_SF16)
        DXGIFormatCase(DXGI_FORMAT_BC7_TYPELESS)
        DXGIFormatCase(DXGI_FORMAT_BC7_UNORM)
        DXGIFormatCase(DXGI_FORMAT_BC7_UNORM_SRGB)
        DXGIFormatCase(DXGI_FORMAT_AYUV)
        DXGIFormatCase(DXGI_FORMAT_Y410)
        DXGIFormatCase(DXGI_FORMAT_Y416)
        DXGIFormatCase(DXGI_FORMAT_NV12)
        DXGIFormatCase(DXGI_FORMAT_P010)
        DXGIFormatCase(DXGI_FORMAT_P016)
        DXGIFormatCase(DXGI_FORMAT_420_OPAQUE)
        DXGIFormatCase(DXGI_FORMAT_YUY2)
        DXGIFormatCase(DXGI_FORMAT_Y210)
        DXGIFormatCase(DXGI_FORMAT_Y216)
        DXGIFormatCase(DXGI_FORMAT_FORCE_UINT)
        default: return "UNKNOWN_FORMAT";
    }
}


