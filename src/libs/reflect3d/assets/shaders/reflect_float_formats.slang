// Vertex binding reflection: float32 and float16 format coverage
//
// Expected reflection output:
//   1 binding, 8 attributes, byte_stride = 60
//
//   attr  format           offset  size
//   0     r32_sfloat       0       4
//   1     rg32_sfloat      4       8
//   2     rgb32_sfloat     12      12
//   3     rgba32_sfloat    24      16
//   4     r16_sfloat       40      2
//   5     rg16_sfloat      42      4
//   6     rgb16_sfloat     46      6
//   7     rgba16_sfloat    52      8

struct FloatInput {
    float  f32x1;
    float2 f32x2;
    float3 f32x3;
    float4 f32x4;
    half   f16x1;
    half2  f16x2;
    half3  f16x3;
    half4  f16x4;
};

struct VSOutput {
    float4 position : SV_Position;
    float2 uv;
};

[shader("vertex")]
VSOutput vertMain(FloatInput input) {
    VSOutput output;
    output.position = input.f32x4 + float4(input.f32x3, 0.0) + float4(input.f32x2, 0.0, 0.0) + input.f32x1;
    output.uv       = float2(input.f16x1, 0.0) + input.f16x2;
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return float4(input.uv, 0.0, 1.0);
}
