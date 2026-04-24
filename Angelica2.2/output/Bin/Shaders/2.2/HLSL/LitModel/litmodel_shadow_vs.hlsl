#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"
float4x4 	matWorldViewProj;
float4x4	matLightWVP;
float3      g_vecEyePos;

string entrynames = "vs_2_0 vs_main";	

struct VS_INPUT
{
	float4 pos          : POSITION0;
	float3 diffuse		: COLOR0;
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
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 ip = input.pos;
	
	output.pos = mul(ip, matWorldViewProj);
	output.diffuse = float4(input.diffuse, 1);
	output.uv = input.uv;

	output.shadowProj  = glb_ComputeShadowProj(input.pos, matLightWVP);
	output.fogParam = ComputeCompositeFogSimple(input.pos - g_vecEyePos, input.pos.y);

	return output;
}
