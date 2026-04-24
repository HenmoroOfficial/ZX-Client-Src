string entrynames = "ps_2_0 ps_main";

string macro1
<
	string name = "_ANGELICA_2_2";
	string desc = "是否是2.2";
	string type = "PRG";
	string GroupID = "1";
>;

sampler 		sampSceneColor 			: register(s0);
sampler3D		sampColorCorrection1 	: register(s1);
sampler3D		sampColorCorrection2 	: register(s2);

#ifdef _ANGELICA_2_2
float enableScreenIlum;
sampler			sampScreenIlum			: register(s3);
#endif

float fFactor = 0;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float3 color = saturate(tex2D(sampSceneColor, texcoord).xyz);
	
#if ((PROFILE == 3) || defined(_SDR_3_0))
	float3 color1 = tex3Dlod(sampColorCorrection1, float4(color.xyz,0)).xyz;
	float3 color2 = tex3Dlod(sampColorCorrection2, float4(color.xyz,0)).xyz;
#else
	float3 color1 = tex3D(sampColorCorrection1, color.xyz).xyz;
	float3 color2 = tex3D(sampColorCorrection2, color.xyz).xyz;
#endif

	
#ifdef _ANGELICA_2_2

return float4(lerp(color1, color2, fFactor), 1);	//暂时不开启随着屏幕亮度调整颜色

if(enableScreenIlum > 0.5)
{
	color = lerp(color1, color2, fFactor);

	float g_fExposure = 1.f; //0.5;
	float expo = g_fExposure;

	float fAveLum = tex2D(sampScreenIlum, float2(0.5f, 0.5f)).r;
	
	//return float4(fAveLum,fAveLum,fAveLum,1);

	float change = -min(fAveLum, 0.6) * 4.0 + 1;			// * 1.6
	if(change < 0)
		change = max(-0.01 *(g_fExposure + 5.f), change);
	
	//float change = 1 - min(fAveLum, 0.5) * 2;
	//expo = max(0.f, g_fExposure + change);
	
	//只让画面变亮, 而不变暗
	expo = min(1.f, expo);
	
	color = saturate(color * expo);

	return float4(color, 1);
}
else
	return float4(lerp(color1, color2, fFactor), 1);	
#else
	return float4(lerp(color1, color2, fFactor), 1);	
	
#endif	
}

