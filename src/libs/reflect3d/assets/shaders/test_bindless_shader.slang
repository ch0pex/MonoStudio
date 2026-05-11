// Bindless shader covering all resource kinds visible through Slang reflection:
//   ConstantBuffer  ->  TypeReflection::Kind::ConstantBuffer
//   Texture2D[]     ->  TypeReflection::Kind::Resource  (SRV, read-only)
//   RWTexture2D[]   ->  TypeReflection::Kind::Resource  (UAV, read-write)
//   StructuredBuffer->  TypeReflection::Kind::Resource  (SRV)
//   SamplerState[]  ->  TypeReflection::Kind::SamplerState

// --- Structs ---

struct Vertex {
    float3 position;
    float2 uv;
};

struct Material {
    uint  albedo_index;
    uint  normal_index;
    uint  sampler_index;
    float roughness;
};

struct FrameData {
    float4x4 view_proj;
    float3   camera_pos;
    uint     frame_index;
};

struct DrawData {
    uint vertex_buffer_index;
    uint material_index;
};

// --- Bindless resource tables ---

ConstantBuffer<FrameData>          gParameter0;
ConstantBuffer<DrawData>           gParameter1;

StructuredBuffer<Vertex>           gParameter2[];   // SRV  - bindless vertex buffers
StructuredBuffer<Material>         gParameter3[];       // SRV  - bindless material buffer
Texture2D<float4>                  gParameter4[];        // SRV  - bindless texture array
SamplerState                       gParameter5[];        // Sampler

// --- Shader stages ---

struct VSOutput {
    float4 pos   : SV_Position;
    float2 uv;
    uint   material_index;
};

[shader("vertex")]
VSOutput vertMain(uint vertex_id : SV_VertexID) {
    StructuredBuffer<Vertex> vb = gParameter2[gParameter1.vertex_buffer_index];
    Vertex v = vb[vertex_id];

    VSOutput out;
    out.pos            = mul(gParameter0.view_proj, float4(v.position, 1.0));
    out.uv             = v.uv;
    out.material_index = gParameter1.material_index;
    return out;
}

[shader("fragment")]
float4 fragMain(VSOutput input) : SV_TARGET {
    Material mat     = gParameter3[input.material_index][0];
    Texture2D albedo = gParameter4[mat.albedo_index];
    SamplerState smp = gParameter5[mat.sampler_index];
    return albedo.Sample(smp, input.uv);
}
