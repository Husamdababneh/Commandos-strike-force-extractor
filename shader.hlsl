

struct VS_INPUT {
    float3 topLeft : POS;
    float2 size : SIZE;
    bool isSelected: SELECTED;
};

struct GS_INPUT {
    float4 topLeft : SV_POSITION;
    float2 size : SIZE;
    bool isSelected: SELECTED;
};

struct PS_INPUT {
    float4 position : SV_POSITION;
    bool isSelected: SELECTED;
};

cbuffer ScreenData : register(b0) {
    float2 screenSize; // (width, height)
    float2 padding;
}


//- Vertex Shader
GS_INPUT VS_Main(VS_INPUT input) {
    GS_INPUT output;
    // output.position = float4(input.center, 1.0f);
    output.topLeft = float4(
        (input.topLeft.x / screenSize.x) * 2.0f - 1.0f,  // Convert X to [-1,1]
        1.0f - (input.topLeft.y / screenSize.y) * 2.0f,  // Convert Y to [-1,1] (Flip Y)
        input.topLeft.z, 1.0f
    );
    output.size = (input.size / screenSize) * 2 ;
    output.isSelected = input.isSelected;
    return output;
}

// Converts from pixel space to NDC, keeping aspect ratio
float2 AdjustSize(float2 size) {
    float aspect = screenSize.y / screenSize.x; // Height / Width
    return float2(size.x / screenSize.x, (size.y / screenSize.y) * aspect) * 2.0f;
}

//- Geometry Shader
[maxvertexcount(6)]
void GS_Main(point GS_INPUT input[1], inout TriangleStream<PS_INPUT> triStream) {
    float2 size = input[0].size;  // Full size (width, height)
    float4 topLeft = input[0].topLeft;  // This is the given top-left corner
    bool isSelected = input[0].isSelected;
    // Compute the other three corners from the top-left
    float4 corners[4] = {
        topLeft,                                       // Top-left
        topLeft + float4(size.x,  0.0f, 0.0f, 0.0f),   // Top-right
        topLeft + float4(size.x, -size.y, 0.0f, 0.0f), // Bottom-right
        topLeft + float4(0.0f,   -size.y, 0.0f, 0.0f)  // Bottom-left
    };

    PS_INPUT v;
    v.isSelected = isSelected;
    
    // Triangle 1 (Top-left, Top-right, Bottom-right)
    v.position = corners[0]; triStream.Append(v);
    v.position = corners[1]; triStream.Append(v);
    v.position = corners[2]; triStream.Append(v);
    triStream.RestartStrip();

    // Triangle 2 (Top-left, Bottom-right, Bottom-left)
    v.position = corners[0]; triStream.Append(v);
    v.position = corners[2]; triStream.Append(v);
    v.position = corners[3]; triStream.Append(v);
    triStream.RestartStrip();


}


//- Pixel Shader
float4 PS_Main(PS_INPUT input) : SV_TARGET
{
    float alpha = 1.0f;
    if (input.isSelected) alpha = 0.7;
    return float4(1.0f, 1.0f, alpha, 0.0f); // White color
}
