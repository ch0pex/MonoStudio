// Vertex binding reflection: entry point uniform params must not produce bindings
//
// The vertex entry point has two parameters:
//   1. uniform ConstantBuffer<FrameData> — category: CONSTANT_BUFFER, not VARYING_INPUT → filtered out
//   2. VertexInput — category: VARYING_INPUT → produces a binding
//
// Expected reflection output: 1 binding
//
//   binding 0, byte_stride = 20, 2 attributes:
//     attr  format        offset  size
//     0     rgb32_sfloat  0       12   (position)
//     1     rg32_sfloat   12      8    (uv)
//
// Also useful for future reflection tests:
//   - Distinguishing uniform entry point params from vertex inputs
//   - Entry point parameter count and categories

struct FrameData {
    float4x4 view_proj;
    float3   camera_pos;
    uint     frame_index;
};

struct VertexInput {
    float3 position;
    float2 uv;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv;
};

[shader("vertex")]
VSOutput vertMain(uniform ConstantBuffer<FrameData> frameData, VertexInput vertex) {
    VSOutput output;
    output.position = mul(frameData.view_proj, float4(vertex.position, 1.0));
    output.uv       = vertex.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return float4(input.uv, 0.0, 1.0);
}
