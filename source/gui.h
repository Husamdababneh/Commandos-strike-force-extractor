/* ========================================================================
   $File: gui.h
   $Date: 2025-02-15
   $Revision: : @Incomplete
   $Creator: Husam Dababneh
   $Description: 
    ========================================================================*/
#pragma once


const char* shader = R"(
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
