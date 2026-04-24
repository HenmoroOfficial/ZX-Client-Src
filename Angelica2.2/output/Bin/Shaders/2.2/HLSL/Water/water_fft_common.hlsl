
float4 GetWaterVolumetricFogColor(float waterDis, float fFogDensity, float fWaterFogScaleExp, float3 waterFogColor)
{
	float d = waterDis;
	d = d * 0.005;
	float density = fFogDensity * pow(2, fWaterFogScaleExp);
	float f = 1 / exp(d * d * density * density);
	float3 fogColor = waterFogColor;
	return float4(fogColor, f);
}

float GetCausticIllum(float4 fCausticLerpDis, float depthToWater)
{
	
	float fCausticIllum;
	if(depthToWater < fCausticLerpDis.x) //|| fCausticLerpDis > fCausticLerpDis.w)
		fCausticIllum = 0;
	else if( depthToWater < fCausticLerpDis.y)
		fCausticIllum = lerp(0, 1, (depthToWater - fCausticLerpDis.x) / (fCausticLerpDis.y - fCausticLerpDis.x));
	else if( depthToWater < fCausticLerpDis.z)
		fCausticIllum = 1;
	else if( depthToWater < fCausticLerpDis.w)
		fCausticIllum = lerp(1, 0, (depthToWater - fCausticLerpDis.z) / (fCausticLerpDis.w - fCausticLerpDis.z));
	else
		fCausticIllum = 0;
	return fCausticIllum;
}


float SunShape(float2 v)
{
	return saturate(1 - (abs(v.x / 2) + abs(v.y / 2)));
}

float ComputeCausticColor2(float3 vWorldPos, float3x3 lightViewMat, float2 uvOffset, sampler2D sampCaustic1, sampler2D sampCaustic2, float fLerp)
{
	float3 vProj = mul(vWorldPos, lightViewMat);
	float2 uv = (vProj.xy + uvOffset) * 0.2;
	float samp1 = tex2D(sampCaustic1, uv).r;
	float samp2 = tex2D(sampCaustic2, uv).r;
	return lerp(samp1, samp2, fLerp);

}

float ComputeCausticColor(float3 vWorldPos, float3 vSunLightDir, float3x3 lightViewMat, float fTime, sampler2D oceanBumpMap)
{
	float4 waveTC;
	float4 causticTC0;
	float4 causticTC1;

	float3x3 mLightView = lightViewMat;
	
	float3 vProj = mul(vWorldPos, mLightView);

	
	float2 animP = float2(fTime * 0.1, fTime * 0.1);
	float causticSize = 0.008; //·´±È
	waveTC.xy = vProj.xy * 2 * causticSize * 0.012 + animP * 0.06;
	waveTC.wz = vProj.xy * 2 * causticSize * 0.01 + animP * 0.05;
	causticTC0.xy =  vProj.xy * causticSize  + animP * 0.1;
	causticTC0.wz =  vProj.yx * causticSize - animP * 0.11;  
	causticTC1.xy =  vProj.xy * causticSize * 4.0 + animP * 0.1;
	causticTC1.wz =  vProj.yx * causticSize * 4.0 - animP * 0.11;   

	 // break movement, with random patterns
	float2 wave = tex2Dlod( oceanBumpMap, float4(waveTC.xy,0,0) ).xy;                                                
	wave.xy += tex2Dlod( oceanBumpMap, float4(waveTC.wz,0,0) ).xy;                                                 // 1 alu

	// put in normalized range (with constant scale)
	wave.xy = wave.xy*0.04 - 0.04;                                                                    // 1 alu

  
	float3 causticMapR = 1.0;

	float mipmapLevel = 0;
	
	causticMapR.xz = tex2Dlod( oceanBumpMap, float4(causticTC0.xy + wave.xy,0,mipmapLevel)).xz;
	causticMapR.xz += tex2Dlod( oceanBumpMap, float4(causticTC0.wz + wave.xy,0,mipmapLevel)).xy;

	float2 causticHighFreq = 0;
	causticHighFreq = tex2Dlod( oceanBumpMap, float4(causticTC1.xy + wave.xy,0,mipmapLevel) ).xy;
	causticHighFreq += tex2Dlod( oceanBumpMap, float4(causticTC1.wz + wave.xy,0,mipmapLevel) ).xy;

	causticMapR.xz = (causticMapR.xz + causticHighFreq * 3.0 - 4.0);
	// Get main vectors/coeficients	   
	
	float3 normalVec = normalize( causticMapR );       // 3 alu
	float2 vRefrR = refract( vSunLightDir, normalVec.xyz, 0.9 ).xz;                              // Displace refraction vectors to simulate color dispersion   

	float r = SunShape(vRefrR);
	return r;
}
