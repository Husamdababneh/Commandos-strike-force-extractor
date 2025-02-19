
//- Vertex Shader

struct VS_INPUT {
    float3 center : POS;
    float2 size : SIZE;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
};


PS_INPUT VShader(VS_INPUT input, uint vertexID : SV_VertexID, uint instanceID : SV_InstanceID) {
    PS_INPUT output;
    
    float halfWidth  = input.size.x * 0.5f;
    float halfHeight = input.size.y * 0.5f;
    
    float3 topLeft     = input.center + float3(-halfWidth, halfHeight, 0.0f);
    float3 topRight    = input.center + float3(halfWidth, halfHeight, 0.0f);
    float3 bottomLeft  = input.center + float3(-halfWidth, -halfHeight, 0.0f);
    float3 bottomRight = input.center + float3(halfWidth, -halfHeight, 0.0f);

         if (vertexID == 0) output.position = float4(topLeft, 1.0f);
    else if (vertexID == 1) output.position = float4(topRight, 1.0f);
    else if (vertexID == 2) output.position = float4(bottomLeft, 1.0f);
    else                    output.position = float4(bottomRight, 1.0f);

    return output;
}




//- Pixel Shader
float4 PShader(PS_INPUT position) : SV_TARGET
{
    return float4(1.0f, 1.0f, 1.0f, 1.0f); // White color
}
