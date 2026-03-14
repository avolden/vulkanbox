#include <metal_stdlib>
using namespace metal;

struct vertex_in
{
	float4 pos;
	float4 col;
    float2 uv;
};

struct vertex_out
{
    float4 pos [[position]];
    float4 col;
    float2 uv;
};

struct vp_data
{
    float4x4 view;
    float4x4 proj;
};

using vec4 = float[4];

vertex vertex_out
vertexFunction(uint vert_id [[vertex_id]],
             constant vertex_in* vertices,
             constant ushort* indices,
             constant vp_data* vp)
{
    vertex_out out;
    vertex_in vert_in = vertices[indices[vert_id]];

    out.pos = (vp->proj * vp->view) * vert_in.pos;
    out.col = vert_in.col;
    out.uv = vert_in.uv;
    return out;
}

fragment float4 fragmentFunction(vertex_out in [[stage_in]],
                                texture2d<float> tex [[texture(0)]])
{
    constexpr sampler tex_sampler (mag_filter::nearest, min_filter::nearest);
    return tex.sample(tex_sampler, in.uv) * in.col;
}