#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"

string entrynames = "vs_2_0 vs_main";	

float4x4 	matWorldViewProj;
float4x4	matLightWVP;
float3      g_vecEyePos;
float4x4	matWorld : register(c8);

struct VS_INPUT
{
	float3 pos          : POSITION0;
	float3 diffuse			: COLOR0;
    float2 uv           : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 pos      						: POSITION0;
	float4 diffuse  						: COLOR0;
	float2 uv       						: TEXCOORD0;
	float4 shadowProj						: TEXCOORD1;
	float4 fogParam                         : TEXCOORD2;
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output;
	float4 ip = float4(input.pos, 1);
	
    float4 vWorldPos = mul(matWorld, ip);
	output.pos = mul(vWorldPos, matWorldViewProj);
	output.diffuse = float4(input.diffuse, 1);
	output.uv = input.uv;
	
	output.shadowProj  = glb_ComputeShadowProj(float4(input.pos, 1), matLightWVP);
	output.fogParam = ComputeCompositeFogSimple(vWorldPos.xyz - g_vecEyePos, vWorldPos.y);

	return output;
}