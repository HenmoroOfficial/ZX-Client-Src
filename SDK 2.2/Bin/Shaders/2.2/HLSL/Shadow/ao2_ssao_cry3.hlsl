string entrynames = "ps_3_0 ps";

sampler g_RandomRotTextureSampler	: register(s0);
sampler g_GBufferSampler			: register(s1);
sampler g_SmallGBufferSampler		: register(s2);

sampler g_SmallGBufferSamplerLast	: register(s3);
sampler g_SSAOSamplerLast			: register(s4);

float4 g_param						: register(c0);
//float g_zFar;
//float g_R;
//float2 g_vPixelSize
float4 g_param2						: register(c1);
//float2 g_FocalLen
//float2 g_InvFocalLen;
float4 g_param3						: register(c2);
//float g_Contrast
//float g_Darkest

float4x4 g_DeltaMatrix				: register(c3);




#define radiusDepthScale 			0.04f

struct VSOut
{
    float4 pos  : POSITION ;   // vertex position 
    float2 uv  	: TEXCOORD0 ;  // vertex texture coords 
    float2 uv2  : TEXCOORD1 ;
	float3 deltaPos : TEXCOORD2 ;
};


float3 UV2Eye(float2 uv, float eyeZ, float2 g_InvFocalLen)
{
    uv = (uv * float2(2.0, -2.0) - float2(1.0, -1.0));
    return float3(uv * g_InvFocalLen * eyeZ, eyeZ);
}

float4 ps(VSOut In) : COLOR0
{

	const float g_ZFar			= g_param.x;
	const float g_R				= g_param.y;
	const float2 g_InvFocalLen	= g_param2.zw;
	const float g_Contrast		= g_param3.x;
	const float g_Darkest		= g_param3.y;
    float OUT;

	float2 rotSample = tex2D(g_RandomRotTextureSampler, In.uv2).xy;

	float2x2 rotMat ={		{rotSample.y, rotSample.x},
							{-rotSample.x, rotSample.y}};
	
	float fSceneDepth = tex2D( g_GBufferSampler, In.uv).r * g_ZFar;
	
	float iluTotal = 1.f;
	float weightTotal = 1.f;

	const float4 lineOffset[4] = {	float4(0.166, 	0, 			0.986,  0.986 * 0.986),
									float4(0, 		-0.333, 	0.943,	0.943 * 0.943),
									float4(0.667, 	0, 			0.745,	0.745 * 0.745),
									float4(0, 		-0.834, 	0.551,	0.551 * 0.551)
							};
							
	float R = g_R;  
	
	float2 radiusInView = R / (g_InvFocalLen * fSceneDepth);
	//float invalidScale = 1.f;  //5.0f;
	//float invalidDis = 10.f;
	//if(fSceneDepth < invalidDis)
	//	invalidScale = invalidScale * fSceneDepth / invalidDis;
	
	float diffScale = 1; //max(1.0, pow(fSceneDepth / 10.f, 0.5));
	
	for(int i = 0; i < 4; i++)
	{
		float2 ilum;
		float4 offset;
		offset.xy = mul(lineOffset[i].xy,rotMat) * radiusInView * 1.0; 
		offset.zw = -offset.xy;
		float height = lineOffset[i].z;	
        float sampleDepthInTexture1 = tex2D( g_SmallGBufferSampler, In.uv + offset.xy).r * g_ZFar;
        float sampleDepthInTexture2 = tex2D( g_SmallGBufferSampler,  In.uv + offset.zw).r * g_ZFar;
		float2 sampleDepthInTexture = float2(sampleDepthInTexture1, sampleDepthInTexture2);

		float2 diff = float2(fSceneDepth, fSceneDepth) - sampleDepthInTexture;
		
		
		float2 temp = (diff) / (R * lineOffset[i].z * diffScale);
		
		float2 rangeInvalid = saturate(diff);
		
		if(rangeInvalid.x > 0.999 || rangeInvalid.y > 0.999)
			continue;
		
		float2 tempModified = lerp( temp.xy, float2(-temp.y, -temp.x), rangeInvalid.xy);

		ilum = (height - tempModified) / height;
	
		float weight = lineOffset[i].w;
		float ilumT = ilum.x + ilum.y;
		
		const float threshold = 0.3 * 2;
		if(ilumT > threshold)
			ilumT = 2;
		
		iluTotal += ilumT * weight;
	
		weightTotal += weight * 2;
    }

	OUT = saturate(iluTotal / weightTotal);
	//调整对比度
	OUT = pow(OUT, 1 + g_Contrast);
	OUT = lerp(g_Darkest, 1, OUT);
	
	float3 posEye = UV2Eye(In.uv, fSceneDepth, g_InvFocalLen);

	float4 lastUV = mul(float4(posEye,1), g_DeltaMatrix);
	lastUV.xy /= lastUV.w;

	float accuFrameNum = 4;
	float4 result;
	if(length(lastUV.xy - In.uv) > 0.02)
		result = float4(OUT, OUT,OUT, 1);
	else
	{
		float last = tex2D(g_SSAOSamplerLast, lastUV.xy).r;
		result =  (float4(last, last,last,1) * (accuFrameNum - 1) + float4(OUT,OUT,OUT,1) ) / accuFrameNum ;
	}
	
	return result;
}