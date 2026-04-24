#ifndef _COMMON_H_
#define _COMMON_H_

float3 glb_CalcReflection(float3 N, float3 L)
{
	return 2.0f * dot(N, L) * N  - L;
}

//  return  the luminance value of color
float3 glb_desaturate(float3 color)
{
	return dot(color, float3(0.299, 0.587, 0.114));
}

//	return the fresnel result of VdotN
float glb_fresnel(float vdotn, float power, float bias)
{
	float fresnel = 1 - vdotn;
	fresnel = pow(fresnel, power);
	return saturate(fresnel + bias);
}

//	Calculate R dot V for specular calculation
float glb_CalcRDotV(float3 vNormal, float3 vLightDir, float3 vViewDir)
{
	float fNDotL = dot(vNormal, vLightDir);
	float3 vReflection = normalize((2.0f * fNDotL) * vNormal - vLightDir);
	return saturate(dot(vReflection, vViewDir));
}

//	Calculate H dot N for specular calculation
float glb_CalcHDotN(float3 vNormal, float3 vLightDir, float3 vViewDir)
{
	float3 vHalf = normalize(vLightDir + vViewDir);
	return saturate(dot(vHalf, vNormal));
}

// Calculate normal from normalmap
// normalmap format :_Y_X dxt5
float3 glb_SamplerNormalMap(sampler2D NormalSampler, float2 TexCoord, float smoothNess=1.0f)
{
	float3 Normal;
	Normal.xy = 2 * tex2D(NormalSampler, TexCoord).wy - 1;
	Normal.z = sqrt(saturate(1 - Normal.x * Normal.x - Normal.y * Normal.y));
	Normal.z *= smoothNess;
	return normalize(Normal);	//	If consider mipmaps, normal should be normalized
//	return Normal;
}

float2 g_vScreenTextureDim : ScreenTextureDimension;

//	Calculate screen-space texture coordinates, this function add the half-pixel offset
//	due to the texturing algorithm used by Direct3D
float4 glb_CalcScreenSpaceTexCoord(float4 vProjPos)
{
	float2 xy = float2(vProjPos.x + vProjPos.w, vProjPos.w - vProjPos.y);
	float4 vPos = float4(0.5f * (xy + vProjPos.w / g_vScreenTextureDim), vProjPos.zw);
	return vPos;
}

float4 g_vProjRatio : ProjectionRatio;
     
float glb_GetLinearDepth(float fDepth)
{
    return g_vProjRatio.y / (fDepth - g_vProjRatio.x);
}

float glb_GetNonLinearDepth(float fDepth)
{
    return g_vProjRatio.y / fDepth + g_vProjRatio.x;
}

#endif