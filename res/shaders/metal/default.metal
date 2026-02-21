#include <metal_stdlib>
using namespace metal;

// struct vertex
// {
// 	float4 pos;
// 	float4 col;
// 	float2 uv;
// };

using vec4 = float[4];

vertex float4
vertexFunction(uint vertexID [[vertex_id]],
             constant vec4* vertexPositions)
{
    float4 vertexOutPositions = float4(vertexPositions[vertexID][0],
                                       vertexPositions[vertexID][1],
                                       vertexPositions[vertexID][2],
                                       1.0f);
    return vertexOutPositions;
}

fragment float4 fragmentFunction(float4 vertexOutPositions [[stage_in]]) {
    return float4(182.0f/255.0f, 240.0f/255.0f, 228.0f/255.0f, 1.0f);
}