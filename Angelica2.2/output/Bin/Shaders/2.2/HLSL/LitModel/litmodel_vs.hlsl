#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"
#include "litmodel_common.h"

string entrynames = "vs_2_0 vs_main";	

string macro0
<
	string name = "_LIGHT_MAP_";
	string desc = "Has Light Map";
	string type = "PRG";
	string GroupID = "1";
>;


float4x4 matViewProj;
float4x4 matLightWVP;
float4x4 matWorld;
float3 g_vecEyePos;

struct VS_INPUT
{
	float3 pos          : POSITION;
	float3 normal		: NORMAL;
    float2 uv           : TEXCOORD0;
#ifdef _LIGHT_MAP_
	float2 uvlm			: TEXCOORD1;
#else
	float3 diffuse		: COLOR0;
#endif	
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	float4 ip = float4(input.pos, 1);
	
    float4 vWorldPos = mul(ip, matWorld);
	output.normal = mul(float4(input.normal, 0), matWorld).xyz;
	//output.normal = normalize(output.normal);
	output.pos = mul(vWorldPos, matViewProj);
	output.uv = input.uv;
#ifdef _LIGHT_MAP_
	output.uvlm = input.uvlm;
#else
    float maxLight = LitModelLightMapParam.z;
	output.diffuse = float4(input.diffuse * maxLight, 1);
	output.uvlm = 0;
#endif	
	output.shadowProj  = glb_ComputeShadowProj(float4(vWorldPos.xyz, 1), matLightWVP);
	output.fogParam = ComputeCompositeFogSimple(vWorldPos.xyz - g_vecEyePos, vWorldPos.y);

	return output;
}