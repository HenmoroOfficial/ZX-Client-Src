

float4 glb_ComputeShadowProj(float4 vWorldPos, float4x4 matLightWVP)
{
	float4 pos = mul(vWorldPos, matLightWVP); 
	return pos;
}


	
float4 glb_ComputShadowColor(sampler shadowMap, sampler shadowMapFloat, 
								float4 shadowProj, bool bIsTSM, bool isHWPCF, float shadowMapSize, float sampleCount)
{
	float2 uvShadowMap  = shadowProj.xy / shadowProj.w;
	float depthCurPixel = shadowProj.z  / shadowProj.w;

	const float4 vIntensityInLight = float4(1,1,1,1);
	const float4 vIntensityInShadow = float4(0.2,0.2,0.2,1.0);
	const float4 vAmbientColor = float4(0,0,0,0.2);	
	
	float2 pixelSize = float2(1.0 / shadowMapSize, 1.0 / shadowMapSize);
	pixelSize *= 0.5;
	uvShadowMap += pixelSize;

#ifdef _SDR_3_0	
	int sampleNum = sampleCount;
#else
	const int sampleNum = 1;
#endif
	
	const float3 offset[16] = 
	{
		float3(0,					0,							1.0),
		float3( pixelSize.x,  		pixelSize.y, 				1.0),
		float3( -pixelSize.x,  		pixelSize.y,				1.0),
		float3( pixelSize.x,  		-pixelSize.y,				1.0),
		float3( -pixelSize.x, 	   -pixelSize.y,				1.0),
		
		float3( pixelSize.x * 1.8,  		pixelSize.y * 4.0, 				1.0),
		float3( pixelSize.x * 1.6,  		pixelSize.y * 2.5,				1.0),
		float3( pixelSize.x * 2.65,  		pixelSize.y * 1.75,				1.0),
		float3( pixelSize.x * (-1.55), 	   	pixelSize.y * 3.9,				1.0),
		float3( pixelSize.x * (-2.45),  	pixelSize.y * 1.65,				1.0),
		float3( pixelSize.x * (-0.7),  		pixelSize.y * 1.35,				1.0),
		float3( pixelSize.x * (-1.1),  		pixelSize.y * (-0.7),			1.0),
		float3( pixelSize.x *(-1.7), 	   	pixelSize.y * (-2.8),			1.0),
		float3( pixelSize.x * 0.7,  		pixelSize.y * (-0.8),			1.0),
		float3( pixelSize.x * 1.6,  		pixelSize.y * (-1.75),			1.0),
		float3( pixelSize.x * 2.3,  		pixelSize.y * (-2.95),			1.0),
	};
	
	float totalWeight = 0.f;
	float4 vShadow = float4(0,0,0,0);		

	if(isHWPCF)
	{
		const float twoLayerThreshold = 0.075f;
		
		for(int i = 0; i < sampleNum; i++)
		{
			float fShadow = tex2Dproj(shadowMap, float4(uvShadowMap + offset[i].xy, depthCurPixel, 1)).r;
			vShadow += lerp(vIntensityInShadow, vIntensityInLight, fShadow) * offset[i].z;
			totalWeight += offset[i].z;
		}	
		vShadow /= max(totalWeight, 1.f);
	}
	else 
	{
		//VSM 很不给力. 而且在边界处有问题
		/*
		float4 moments;
      	moments = tex2D(shadowMapFloat, uvShadowMap);
	
    	float rescaled_dist_to_light = 1 - depthCurPixel;
		
		float light_shadow_bias = 0.001;
    	rescaled_dist_to_light -= light_shadow_bias;
    	float lit_factor = (rescaled_dist_to_light <= moments.x);
    	
    	float E_x2 = moments.y;
    	float Ex_2 = moments.x * moments.x;
		float light_vsm_epsilon = 0.0001;
    	float variance = min(max(E_x2 - Ex_2, 0.0) + light_vsm_epsilon, 1.0);
    	float m_d = (moments.x - rescaled_dist_to_light);
    	float p = variance / (variance + m_d * m_d);

		//float s = rescaled_dist_to_light;
		//float s = lit_factor;
		//float s = max(lit_factor, p);
		float s = min(lit_factor, 1 - p);

		vShadow = float4(s, s, s, s);
		return vShadow;
		*/
		
		for(int i = 0; i < sampleNum; i++)
		{
			float3 ds = tex2D(shadowMapFloat, uvShadowMap + offset[i].xy).rgb;
			float fShadow = 1.f;
			if(ds.r < depthCurPixel && depthCurPixel < 0.9999)
				fShadow = 0.f;
			
			float invalid = saturate(depthCurPixel - tex2D(shadowMapFloat, uvShadowMap + offset[i].xy).r - 0.02) / 0.1f;
			float4 vShadowBlur = lerp(vIntensityInShadow, vIntensityInLight, fShadow);
			vShadowBlur = lerp(vShadowBlur, vIntensityInLight, invalid);
			vShadow += vShadowBlur * offset[i].z;
			totalWeight += offset[i].z;
		}
		
		vShadow /= max(totalWeight, 1.f);
	}	
	//float extend = max(abs(uvShadowMap.x), abs(uvShadowMap.y));
	//if(extend >=1.f)
	//	vShadow = vIntensityInLight;
	/*
	const float t = 0.8;
	if(extend > 0.8)
	{
		vShadow = lerp(vShadow, vIntensityInLight, (extend - t) / (1 - t));
	}
	*/
	float ac = vAmbientColor.a;
	vShadow = saturate(vShadow + float4(ac, ac, ac, ac));
	
	return vShadow;
}