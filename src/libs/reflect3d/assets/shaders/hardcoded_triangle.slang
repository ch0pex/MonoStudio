struct VertexOutput {
    float3 color;
    float4 sv_position : SV_Position;
};

float2 getPosition(uint vid) {
    switch(vid) {
        case 0: return float2(0.0, -0.5);
        case 1: return float2(0.5, 0.5);
        default: return float2(-0.5, 0.5);
    }
}

float3 getColor(uint vid) {
    switch(vid) {
        case 0: return float3(1.0, 0.0, 0.0);
        case 1: return float3(0.0, 1.0, 0.0);
        default: return float3(0.0, 0.0, 1.0);
    }
}

[shader("vertex")]
VertexOutput vertMain(uint vid : SV_VertexID) {
    VertexOutput output;

    output.sv_position = float4(getPosition(vid), 0.0, 1.0);
    output.color = getColor(vid);

    return output;
}

[shader("fragment")]
float4 fragMain(VertexOutput inVert) : SV_Target
{
    return float4(inVert.color, 1.0);
}
