string entrynames = "ps_2_0 ps_main";

sampler g_sampBackBuffer				: register(s0);
sampler g_sampBloom							: register(s1);

float g_fGlowLevel = 0.25;
float g_fGlowPower = 0.25;
float4 g_vGlowColor = float4(1,1,1,1);

//c1.g = m_vGlowLevel;
//c1.b = m_vGlowPower;
	
//mul r2.rgb, r1, r1					//r2 = 模糊后颜色的平方
//lrp r1.rgb, c1.g, r1, r0				//r0:中心颜色   r1;模糊后颜色  r1 = lerp(r0, r1, g_fGlowLevel);
//mad r0.rgb, c1.b, r2, r1				// r0 = g_fGlowPower * r2 + r1

//mul r0.rgb, r0, c3					// r0 *= g_vGlowColor
//+mov r0.a, c3.a
float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{

	float3 cCenter 	= tex2D(g_sampBackBuffer, texcoord).xyz;
	float3 cBloom 	= tex2D(g_sampBloom, texcoord).xyz;
	
	float3 cBloomSqr = cBloom * cBloom;
	float3 cLerp = lerp(cCenter, cBloom, g_fGlowLevel);
	float3 cResult = cBloomSqr * g_fGlowPower + cLerp;
	cResult *= g_vGlowColor.rgb;
	float3 color = cCenter + cResult * g_vGlowColor.a;
	return float4(color, 1);
}
