#include "default_shared.hlsli"

struct VS_INPUT
{
	float3 position : POSITION;
};

VS_OUTPUT main(VS_INPUT input)
{
	VS_OUTPUT output;
	output.position = float4(input.position.xyz, 1.0);

	return output;
}