#include "default_shared.hlsli"

float4 main(PS_INPUT input) : SV_Target0
{
	float4 output = float4(input.position.xy, 0, 1);
	return output;
}