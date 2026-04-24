
string entrynames = "ps_2_0 ps_main";	

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

#include "..\Shadow\Shadow_common.h"

#define _FOG_

sampler 		sampDiffuse 						: register(s4);
sampler			sampShadowMap						: register(s0);

struct PS_INPUT
{
	float4 pos      						: POSITION0;
	float4 diffuse  						: COLOR0;
	float2 uv       						: TEXCOORD0;
	float4 shadowProj						: TEXCOORD1;
	float4 fogParam                         : TEXCOORD2;
	float2 uvScreen						  : VPOS;
};

float4 ps_main(PS_INPUT input) : COLOR0
{
	//return float4(1,0,0,1);
	
	float4 vDiffuse = tex2D(sampDiffuse, input.uv);

	float4 vShadow = float4(1,1,1,1);
#ifdef _SHADOW_
	vShadow = glb_ComputShadowColor(sampShadowMap, input.shadowProj, input.uvScreen, true, true);	
#endif
	//float4 sp = input.shadowProj;
	//float3 ds = tex2D(sampShadowMap, float2(sp.x / sp.w, sp.y / sp.w) ).rgb;
	//return float4(ds, 1);
	
	float4 r;
	r.rgb = /*input.diffuse.rgb * */vDiffuse.rgb * vShadow;
	r.a = vDiffuse.a;
	
#ifdef _FOG_	
	r.rgb = lerp(input.fogParam.rgb, r.rgb, input.fogParam.a);
#endif

	return r;
}




