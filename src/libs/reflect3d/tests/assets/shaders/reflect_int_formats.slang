// Vertex binding reflection: uint32, int32, uint16, int16 format coverage
//
// Note: 3-component uint16/int16 (rgb16_uint/sint) are not in the format enum
// and would be skipped — they are intentionally excluded from this struct.
//
// Expected reflection output:
//   1 binding, 14 attributes, byte_stride = 108
//
//   attr  format           offset  size
//   0     r32_uint         0       4
//   1     rg32_uint        4       8
//   2     rgb32_uint       12      12
//   3     rgba32_uint      24      16
//   4     r32_sint         40      4
//   5     rg32_sint        44      8
//   6     rgb32_sint       52      12
//   7     rgba32_sint      64      16
//   8     r16_uint         80      2
//   9     rg16_uint        82      4
//   10    rgba16_uint      86      8
//   11    r16_sint         94      2
//   12    rg16_sint        96      4
//   13    rgba16_sint      100     8

struct IntInput {
    uint      u32x1;
    uint2     u32x2;
    uint3     u32x3;
    uint4     u32x4;
    int       i32x1;
    int2      i32x2;
    int3      i32x3;
    int4      i32x4;
    uint16_t  u16x1;
    uint16_t2 u16x2;
    uint16_t4 u16x4;
    int16_t   i16x1;
    int16_t2  i16x2;
    int16_t4  i16x4;
};

struct VSOutput {
    float4 position : SV_Position;
};

[shader("vertex")]
VSOutput vertMain(IntInput input) {
    VSOutput output;
    output.position = float4(float(input.u32x1), float(input.i32x1), float(input.u16x1), 1.0);
    return output;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_Target {
    return float4(1.0, 1.0, 1.0, 1.0);
}
