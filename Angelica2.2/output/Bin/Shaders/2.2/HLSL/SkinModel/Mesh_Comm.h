#ifndef _MESH_COMMON_H_
#define _MESH_COMMON_H_
struct VS_OUTPUT
{
	float4 pos	            : POSITION0;
	float2 uvBase           : TEXCOORD0;
	float3 ViewDir          : TEXCOORD1;
	float3 LightDir         : TEXCOORD2;
	float4 PtLightDir       : TEXCOORD3;
	float4 tangent			: TEXCOORD4;
	float3 binormal			: TEXCOORD5;
	float4 fogParam			: TEXCOORD6;
	float2 highlightUV		: TEXCOORD7;
};
typedef VS_OUTPUT PS_INPUT;
#endif // _MESH_COMMON_H_