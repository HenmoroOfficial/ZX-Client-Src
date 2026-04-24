/*
ps.1.4

def c0, 1.0, 0.0, 0.5, 1.0

texld r0, t0
sub r3, r0.a, c0.b
phase
texkill r3
texld r1, t1
texld r2, t1
lrp r1, c1.a, r2, r1
mul_x2 r0.rgb, r1, r0
+mov r0.a, c0.a
*/

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
sampler g_Sampler0  	: register(s0);
sampler g_Sampler1  	: register(s1);
sampler g_Sampler2  	: register(s2);	

float4	g_c0 : register(c0) = float4(1.0, 0.0, 0.5, 1.0);
float4  g_c1 : register(c1);

sampler			sampShadowMap						: register(s3);

float4 ps_main(in float2 	uv0  											: TEXCOORD0,
							 in float2 	uv1 											: TEXCOORD1,
							 in float4	shadowProj								: TEXCOORD2,
							 in float4  fogParam                                : TEXCOORD3,
							 in float2  uvScreen									: VPOS) : COLOR0
{
	float4 r0 = tex2D(g_Sampler0, uv0);
	
	float r3 = r0.a - g_c0.b;
	clip(r3);
	
	float4 r1 = tex2D(g_Sampler1, uv1) * g_c1.x;
	float4 r2 = tex2D(g_Sampler2, uv1) * g_c1.y;
	r1 = lerp(r1, r2, g_c1.a);
	float4 ret;
	ret.rgb = (r1 * r0).rgb;
	ret.a = g_c0.a;

	float4 vShadow = float4(1,1,1,1);
	
#ifdef _SHADOW_
	vShadow = glb_ComputShadowColor(sampShadowMap, shadowProj, uvScreen, true, true);
#endif
	
	ret.rgb *= vShadow.rgb;

#ifdef _FOG_	
	ret.rgb = lerp(fogParam.rgb, ret.rgb, fogParam.a);
#endif
	
	return ret;
}