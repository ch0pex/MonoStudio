// Vertex binding reflection: vertex pulling (no vertex buffer bindings)
//
// No struct vertex input — data fetched manually via SV_VertexID.
// Expected reflection output: 0 bindings
//
// Also useful for future reflection tests:
//   - Global parameter count and names (gFrameData, gVertices, gAlbedos, gSamplers)
//   - Entry point names and stages (vertMain=vertex, fragMain=fragment)
//   - Resource types: ConstantBuffer, StructuredBuffer[], Texture2D[], SamplerState[]

struct FrameData {
    float4x4 view_proj;
    float3   camera_pos;
    uint     frame_index;
};

struct Vertex {
    float3 position;
    float3 normal;
    float2 uv;
};

ConstantBuffer<FrameData>   gFrameData;
StructuredBuffer<Vertex>    gVertices[];
Texture2D<float4>           gAlbedos[];
SamplerState                gSamplers[];

struct VSOutput {
    float4 position : SV_Position;
    float3 normal;
    float2 uv;
};

[shader("vertex")]
VSOutput vertMain(uint vid : SV_VertexID) {
    Vertex v = gVertices[0][vid];

    VSOutput output;
    output.position = mul(gFrameData.view_proj, float4(v.position, 1.0));
    output.normal   = v.normal;
    output.uv       = v.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return gAlbedos[0].Sample(gSamplers[0], input.uv);
}
