// Vertex binding reflection: edge case field filtering
//
// Two vertex entry points in the same module to test different filtering scenarios.
//
// --- Entry point: vertSvMixed ---
// Struct contains regular vertex attributes mixed with SV_* system values.
// SV_* fields must be skipped.
//
// Expected reflection output: 1 binding
//   binding 0, byte_stride = 20, 2 attributes:
//     attr  format        offset  size
//     0     rgb32_sfloat  0       12   (position)
//     1     rg32_sfloat   12      8    (uv)
//
// --- Entry point: vertUnknownFmt ---
// Struct contains a float4x4 field (Kind::Matrix → Format::unknown → skipped).
// Valid fields before and after the matrix are still reflected.
//
// Expected reflection output: 1 binding
//   binding 0, byte_stride = 20, 2 attributes:
//     attr  format        offset  size
//     0     rgb32_sfloat  0       12   (position)
//     1     rg32_sfloat   12      8    (uv)
//   Note: the float4x4 is skipped, so uv offset = 12 (not 12+64=76)

struct SvMixedInput {
    float3 position;
    float2 uv;
    uint   vid : SV_VertexID;
    uint   iid : SV_InstanceID;
};

struct UnknownFmtInput {
    float3   position;
    float4x4 transform;   // matrix → Kind::Matrix → Format::unknown → skipped
    float2   uv;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv;
};

[shader("vertex")]
VSOutput vertSvMixed(SvMixedInput input) {
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.uv       = input.uv;
    return output;
}

[shader("vertex")]
VSOutput vertUnknownFmt(UnknownFmtInput input) {
    VSOutput output;
    output.position = float4(input.position, 1.0);
    output.uv       = input.uv;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return float4(input.uv, 0.0, 1.0);
}
