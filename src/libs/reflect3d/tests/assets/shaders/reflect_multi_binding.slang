// Vertex binding reflection: multiple vertex buffer bindings
//
// Two struct parameters → two separate bindings.
// Typical per-vertex + per-instance pattern.
//
// Expected reflection output:
//   2 bindings
//
//   binding 0 (per-vertex), byte_stride = 32, 3 attributes:
//     attr  format        offset  size
//     0     rgb32_sfloat  0       12    (position)
//     1     rgb32_sfloat  12      12    (normal)
//     2     rg32_sfloat   24      8     (uv)
//
//   binding 1 (per-instance), byte_stride = 80, 5 attributes:
//     attr  format         offset  size
//     0     rgba32_sfloat  0       16   (transform row 0)
//     1     rgba32_sfloat  16      16   (transform row 1)
//     2     rgba32_sfloat  32      16   (transform row 2)
//     3     rgba32_sfloat  48      16   (transform row 3)
//     4     rgba32_sfloat  64      16   (tint color)

struct PerVertex {
    float3 position;
    float3 normal;
    float2 uv;
};

struct PerInstance {
    float4 transform_row0;
    float4 transform_row1;
    float4 transform_row2;
    float4 transform_row3;
    float4 tint;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv;
    float4 tint;
};

[shader("vertex")]
VSOutput vertMain(PerVertex vertex, PerInstance instance) {
    float4x4 transform = float4x4(
        instance.transform_row0,
        instance.transform_row1,
        instance.transform_row2,
        instance.transform_row3
    );

    VSOutput output;
    output.position = mul(transform, float4(vertex.position, 1.0));
    output.uv       = vertex.uv;
    output.tint     = instance.tint;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return input.tint;
}
