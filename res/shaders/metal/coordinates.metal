#include <metal_stdlib>
using namespace metal;

struct mvp_data
{
    float4x4 view;
	float4x4 proj;
    float2 translate;
};

struct vertex_out
{
    float4 pos [[position]];
    float4 col;
};

constant float4 lines_verts[] =
{
	{0.f, 0.f, 0.f, 1.f},
	{50.f, 0.f, 0.f, 1.f},
    {0.f, 0.f, 0.f, 1.f},
	{0.f, 50.f, 0.f, 1.f},
    {0.f, 0.f, 0.f, 1.f},
	{0.f, 0.f, 50.f, 1.f}
};

constant float4 lines_col[] =
{
	// {1.f, 1.f, 1.f, 1.f},
	{1.f, 0.f, 0.f, 1.f},
	{0.f, 1.f, 0.f, 1.f},
	{0.f, 0.f, 1.f, 1.f}
};

vertex vertex_out v_main(uint vert_id [[vertex_id]], constant mvp_data* mvp)
{
    vertex_out out;
    float4 vert = lines_verts[vert_id];
    out.pos = (mvp->proj * mvp->view) * vert;
    out.pos.x += mvp->translate.x;
    out.pos.y += mvp->translate.y;

    out.col = lines_col[vert_id / 2];

    return out;
}

fragment float4 f_main(vertex_out in [[stage_in]])
{
	return in.col;
}