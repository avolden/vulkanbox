#include <metal_stdlib>
using namespace metal;

struct vertex_in
{
	float4 pos;
	float4 col;
};

struct vertex_out
{
    float4 pos [[position]];
    float4 col;
};

struct vp_data
{
    float4x4 view;
    float4x4 proj;
};



using vec4 = float[4];

vertex vertex_out
vertexFunction(uint vertexID [[vertex_id]],
             constant vertex_in* vertices,
             constant vp_data* vp)
{
    vertex_out out;
    uint vertID = vertexID;
    if (vertexID >= 3) { vertID = vertexID - 3; }
    float4 pos = vertices[vertID].pos;
    if (vertexID >= 3) {
        pos.y += 2.f;
    }
    out.pos = (vp->proj * vp->view) * pos;
    out.col = vertices[vertID].col;
    return out;
}

fragment float4 fragmentFunction(vertex_out in [[stage_in]]) {
    return in.col;
}