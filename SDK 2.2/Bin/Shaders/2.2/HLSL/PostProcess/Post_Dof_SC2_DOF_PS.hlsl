string entrynames = "ps_2_0 ps_main";

float g_fZFar;
float g_fNearDis;
float g_fFocusDis1;
float g_fFocusDis2;
float g_fFarDis;
float g_fClampBlurFar;
float g_vPixelSize;

sampler depthTextureSamplerPoint 		: register(s0);
sampler blurredDepthTextureSampler		: register(s1);
sampler backBufferTextureSampler 		: register(s2);
sampler blurredOneTextureSampler		: register(s3);


//根据距离计算模糊量,结果位于0~1之间,0.5表示是最清晰的(即在焦点内),0和1都为最模糊
float ComputeDepthBlur(float depth)
{
	float f;
	if(depth < g_fFocusDis1)
	{
		f = (depth - g_fFocusDis1) / (g_fFocusDis1 - g_fNearDis);
				f = clamp(f, -1.f, 0);
	}
	else if (depth > g_fFocusDis2)
	{
		f = (depth - g_fFocusDis2) / (g_fFarDis - g_fFocusDis2);
		f = clamp(f, 0, g_fClampBlurFar);
    }
    else
		f = 0;
    return f * 0.5f + 0.5f;
}

float4 Tex2DOffset(sampler s, float2 uv, float2 offset)
{
	float2 pixelSize = g_vPixelSize;
	return tex2D(s, uv + offset * pixelSize);
}
float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	//return float4(1,0,0,1);
	//float4 gb = tex2D(depthTextureSamplerPoint, texcoord);
	float depth = tex2D(depthTextureSamplerPoint, texcoord).r;
	float blurredDepth = tex2D(blurredDepthTextureSampler, texcoord).r;

	float4 RGB = tex2D(backBufferTextureSampler, texcoord);
	float4 BlurredAndCoC = tex2D(blurredOneTextureSampler, texcoord);

	float CoC = BlurredAndCoC.a;
	if(blurredDepth > depth)
	{
		float coc = ComputeDepthBlur(depth * g_fZFar);
		coc = abs(coc - 0.5) / 0.5;
		CoC = coc;
	}

	const float weight = 1;
	float3 s1 = Tex2DOffset(backBufferTextureSampler, texcoord, float2(0.5f, -1.5f)).rgb * weight;
	float3 s2 = Tex2DOffset(backBufferTextureSampler, texcoord, float2(-1.5f, -0.5f)).rgb * weight;
	float3 s3 = Tex2DOffset(backBufferTextureSampler, texcoord, float2(-0.5f, 1.5f)).rgb * weight;
	float3 s4 = Tex2DOffset(backBufferTextureSampler, texcoord, float2(1.5f, 0.5f)).rgb * weight;

	float3 smallBlur = (s1 + s2 + s3 + s4 + RGB.rgb) / 5.f;
	
/*	
	half d0 = 0.50f; 
    half d1 = 0.25f; 
    half d2 = 0.25f; 
    half4 weights = saturate( CoC * half4( -1 / d0, -1 / d1, -1 / d2, 1 / d2 ) +  
                       half4( 1, ( 1 - d2 ) / d1, 1 / d2, ( d2 - 1 ) / d2 ) ); 
    weights.yz = min( weights.yz, 1 - weights.xy ); 
 
    half3 cSmall    = smallBlur; 
    half3 cMed      = smallBlur; 
    half3 cLarge    = BlurredAndCoC.rgb; 
 
    half3 cColor = weights.y * cSmall + weights.z * cMed + weights.w * cLarge; 
    half fAlpha = dot( weights.yzw, half3( 16.0f / 17.0f, 1.0f, 1.0f ) ); 
    
	return float4(lerp(RGB.rgb, cColor, fAlpha), 1);
*/	
	
	float middle = 0.1;
	
	//return float4(CoC,CoC,CoC,1);
	//return float4( BlurredAndCoC.rgb, 1);
	//return float4( lerp(RGB.rgb, BlurredAndCoC.rgb, CoC), 1);
	//return float4(BlurredAndCoC.rgb, 1);//测试最模糊的结果
	
	if(CoC < middle)
		return float4( lerp(RGB.rgb, smallBlur, CoC / middle), 1);
	else
		return float4( lerp(smallBlur, BlurredAndCoC.rgb, (CoC - middle) / (1.f - middle)), 1);

}