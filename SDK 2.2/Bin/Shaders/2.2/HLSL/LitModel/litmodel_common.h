#ifndef _LITMODEL_COMMON_H_
#define _LITMODEL_COMMON_H_

float4 LitModelLightMapParam 	: GlobalLightMapParam;
    
struct VS_OUTPUT
{
	float4 pos      						: POSITION0;
	float3 normal							: TEXCOORD4;
	float4 diffuse  						: TEXCOORD5;
	float2 uv       						: TEXCOORD0;
	float2 uvlm								: TEXCOORD1;
	float4 shadowProj						: TEXCOORD2;
	float4 fogParam                         : TEXCOORD3;
};

struct PS_INPUT
{
	float4 pos      						: POSITION0;
	float3 normal							: TEXCOORD4;
	float4 diffuse  						: TEXCOORD5;
	float2 uv       						: TEXCOORD0;
	float2 uvlm								: TEXCOORD1;
	float4 shadowProj						: TEXCOORD2;
	float4 fogParam                         : TEXCOORD3;
};


#endif // _LITMODEL_COMMON_H_