#include "default_shared.hlsli"

cbuffer cb0
{
	float4 color = float4(1,0,1,1);
};

float4 main(PS_INPUT input) : SV_Target0
{
	float4 output = float4(input.position.xy, 0, 1) * color;
	return output;
}