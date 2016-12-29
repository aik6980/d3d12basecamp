#ifndef DEFAULT_SHARED__HLSLI
#define DEFAULT_SHARED__HLSLI

struct VS_OUTPUT
{
	float4 position : SV_Position;
};

#define PS_INPUT VS_OUTPUT

#endif
