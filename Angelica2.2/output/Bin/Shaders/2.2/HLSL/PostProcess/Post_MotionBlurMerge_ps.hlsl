string entrynames = "ps_2_0 ps_main";

sampler g_Sampler	: register(s0);
sampler g_DepthSamp	: register(s1);
sampler g_SamplerBlured : register(s2);
	
float g_ZFar;
float g_maxScreenSpeed = 0.01;
float g_blurLengthScale = 0.2;
float g_fInFocusDistance = 50.f;
float g_fOutFocusDistance = 100.f;

struct MotionBlurVSOut
{
	float4 HPosition  : POSITION;
	float4 tcProj     : TEXCOORD0;
	float4 tcNewPos   : TEXCOORD1;
	float4 tcPrevPos  : TEXCOORD2;
};

float4 ps_main(MotionBlurVSOut IN) :COLOR0
{  
	const int fSamples = 4;

	float2 posPrev  = (IN.tcPrevPos.xy / IN.tcPrevPos.w);
	float2 pos 		= (IN.tcNewPos.xy  / IN.tcNewPos.w);									

	float2 vPosLerp = (posPrev - pos) * g_blurLengthScale;			
	
	float depth = tex2D(g_DepthSamp, IN.tcProj.xy / IN.tcProj.w).r;
	depth = depth * g_ZFar;
	
	float scaleD = 1.f;
	
	vPosLerp *= float2(scaleD, scaleD);

	float l = length(vPosLerp);
	float2 dir = vPosLerp / l;
	float2 nv =  dir * g_maxScreenSpeed;
	
	if( l > g_maxScreenSpeed)
		vPosLerp = nv;  
	
	float4 cMidCurr = tex2D(g_Sampler, pos);  
	
	if(l < 1e-5)
		return cMidCurr;
	
	float4 accum = tex2D(g_SamplerBlured, pos);

	float disItem = smoothstep(g_fInFocusDistance, g_fOutFocusDistance, depth);
	float blurItem = saturate(length(vPosLerp) / 0.01);
	
	float3 OUT = lerp(cMidCurr.rgb, accum.rgb, min(disItem, blurItem));

	return float4(OUT, 1);
}