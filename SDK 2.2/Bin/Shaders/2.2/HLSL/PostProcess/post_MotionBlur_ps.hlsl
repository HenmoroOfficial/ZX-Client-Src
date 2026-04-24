string entrynames = "ps_2_0 ps_main";

sampler g_Sampler	: register(s0);
	
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

	float scaleD = 1.f;
	
	vPosLerp *= float2(scaleD, scaleD);

	float l = length(vPosLerp);
	float2 dir = vPosLerp / l;
	float2 nv =  dir * g_maxScreenSpeed;
	
	if( l > g_maxScreenSpeed)
		vPosLerp = nv;  

	float2 posStep = vPosLerp / fSamples;	

	float4 cMidCurr = tex2D(g_Sampler, pos);  

	if(l < 1e-5)
		return cMidCurr;
		
	float4 accum = cMidCurr;
	float totalWeight = 1;
	
	float2 tc0 = pos + posStep;
	float2 tc1 = pos - posStep;
	for(int i = 0; i < fSamples; i++)
	{																						
		accum += tex2D(g_Sampler, tc0);
		tc0 += posStep;
		
		accum += tex2D(g_Sampler, tc1);
		tc1 -= posStep;
		
		totalWeight += 2;
	}
	accum /= totalWeight;

	return float4(accum.rgb, 1);
}