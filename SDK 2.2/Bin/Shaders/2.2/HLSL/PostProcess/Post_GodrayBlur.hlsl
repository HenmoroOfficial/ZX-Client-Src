string entrynames = "ps_2_0 ps_main";

sampler srcTextureSampler		: register(s0);
float2 	g_vRadialBlurCenter 	= float2(0.5,0.5);
float2 	g_vScreenSize 			= float2(800,600);




float4 ps_main(in float2 texcoord  : TEXCOORD0) : COLOR0
{
/*	float2 blurVector = g_vRadialBlurCenter - texcoord;
	blurVector = normalize(blurVector);
	float blurLength = 0.1;
	

	
	float4 totalColor = tex2D(srcTextureSampler, texcoord);  
	float totalWeight = 1.f;
	
	float2 step1 = blurVector / sampleNum * blurLength * 0.5;
	float2 step2 = -step1;
	float2 currentTC1 = texcoord + step1;
	float2 currentTC2 = texcoord + step2;
	
	for(int i = 0; i < sampleNum; i++)
	{
		totalColor += tex2D(srcTextureSampler, currentTC1);  
		totalColor += tex2D(srcTextureSampler, currentTC2);  
		currentTC1 += step1;
		currentTC2 += step2;
		totalWeight += 2.f;
	}
	return totalColor / totalWeight;
*/
	const int sampleNum = 12;
	const float g_fCrysisRadiusAttenuation = 0.02; //0.02;
	const float g_fCrysisBlurStrength = 0.03; //0.03;
	
	float2 blurVector = g_vRadialBlurCenter - texcoord;
	float aspectRatio = 1.333 * g_vScreenSize.y / g_vScreenSize.x;
	float sunDist = length(blurVector * float2(1, aspectRatio));

	float blurStrength = g_fCrysisBlurStrength;
	float4 output;

	
	//float fInvRadius = g_fCrysisRadiusAttenuation;
	
	float blurDist = saturate( 1- dot( blurVector.xy * g_fCrysisRadiusAttenuation, blurVector.xy * g_fCrysisRadiusAttenuation)) ;
	blurStrength *= blurDist*blurDist;

	float sn = sampleNum;
	float4 blurred = 0;
	float2 currentTC = texcoord;
	float2 step = blurVector.xy * blurStrength * 16 / sampleNum;
	for(int i = 0; i < sampleNum; i++)
	{
		blurred += tex2D(srcTextureSampler, currentTC);   
		currentTC += step;
	}
	blurred /= sn;
	output = blurred;
	return output;
	
}

