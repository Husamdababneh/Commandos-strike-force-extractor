

struct VS_INPUT {
    float3 center : POS;
    float2 size : SIZE;
};

struct GS_INPUT {
    float4 position : SV_POSITION;
    float2 size : SIZE;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
    float2 uv : TEXCOORD;
};


//- Vertex Shader
GS_INPUT VS_Main(VS_INPUT input, uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID) {
    GS_INPUT output;
    output.position = float4(input.center, 1.0f);
    output.size = input.size;
    return output;
}

//- Geometry Shader
[maxvertexcount(6)]
void GS_Main(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> triStream) {
    float2 halfSize = input[0].size * 0.5f;
    float4 center = input[0].position;
    
    float4 corners[4] = {
        center + float4(-halfSize.x,  halfSize.y, 0.0f, 0.0f), // Top-left
        center + float4( halfSize.x,  halfSize.y, 0.0f, 0.0f), // Top-right
        center + float4( halfSize.x, -halfSize.y, 0.0f, 0.0f), // Bottom-right
        center + float4(-halfSize.x, -halfSize.y, 0.0f, 0.0f)  // Bottom-left
    };
    
    float2 uvs[4] = {
        float2(0, 0), float2(1, 0), float2(1, 1), float2(0, 1)
    };
    
    PS_INPUT v;
    
    // Triangle 1 (Top-left, Top-right, Bottom-right)
    v.position = corners[0]; v.uv = uvs[0]; triStream.Append(v);
    v.position = corners[1]; v.uv = uvs[1]; triStream.Append(v);
    v.position = corners[2]; v.uv = uvs[2]; triStream.Append(v);
    triStream.RestartStrip();
    
    // Triangle 2 (Top-left, Bottom-right, Bottom-left)
    v.position = corners[0]; v.uv = uvs[0]; triStream.Append(v);
    v.position = corners[2]; v.uv = uvs[2]; triStream.Append(v);
    v.position = corners[3]; v.uv = uvs[3]; triStream.Append(v);
    triStream.RestartStrip();
}


//- Pixel Shader
float4 PS_Main(PS_INPUT position) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f); // White color
}
