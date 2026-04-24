#include "..\Shadow\Shadow_common.h"

string entrynames = "ps_3_0 ps_main";	

string macro0
<
	string name = "_FOG_";
	string desc = "是否开启雾";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_SHADOW_";
	string desc = "是否开阴影";
	string type = "PRG";
	string GroupID = "2";
>;

string macro2
<
	string name = "_HWPCF_";
	string desc = "是否开硬件PCF";
	string type = "PRG";
	string GroupID = "3";
>;

sampler 		sampDiffuse 				: register(s0);
sampler			sampShadowMap						: register(s3);

struct PS_INPUT
{
	float4 pos      						: POSITION0;
	float3 normal   						: NORMAL0;
	float4 diffuse  						: COLOR0;
	float2 uv       						: TEXCOORD0;
	float4 shadowProj						: TEXCOORD1;
	float4 fogParam                         : TEXCOORD2;
	float2 uvScreen						  : VPOS;
};

float4 ps_main(PS_INPUT input) : COLOR0
{
	float4 vDiffuse = tex2D(sampDiffuse, input.uv);
	vDiffuse.rgb = vDiffuse.rgb * input.diffuse.rgb * 2;

	float4 vShadow = float4(1,1,1,1);
#ifdef _SHADOW_
	vShadow = glb_ComputShadowColor(sampShadowMap, input.shadowProj, input.uvScreen, true, true);	
#endif

	float4 r = vDiffuse * vShadow;
	r.a = vDiffuse.a;
	
	r.rgb = lerp(input.fogParam.rgb, r.rgb, input.fogParam.a);

	return r;
}