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
};

struct Ui {
    ID3D11Buffer* vertexBuffer;
    u32 maxVertexCount;
    u32 currentVertexCount;
    
    ID3D11Buffer* indexBuffer;
    u32 maxIndexCount;
    u32 currentIndexCount;    
};
