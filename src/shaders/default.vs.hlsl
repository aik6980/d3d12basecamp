#include "default_shared.hlsli"

static const float3 tri[] = 
{
	float3( 0.0, 0.5, 0.5),
	float3( 0.5, -0.5, 0.5),
	float3(-0.5, -0.5, 0.5)
};

VS_OUTPUT main(uint vertex_id : SV_VertexID)
{
	VS_OUTPUT output;
	output.position = float4(tri[vertex_id] * 0.25 + float3(0.5, 0.5, 0), 1.0);

	return output;
}
