#include "..\Shadow\Shadow_common.h"
#include "litmodel_common.h"
#include "litmodel_mtl_common.h"
string entrynames = "ps_2_0 ps_main";	

float3 g_lightDir : LightDir;

float4 ps_main(PS_INPUT input) : COLOR0
{
	float fShadowMapSize = LitModelShadowMapSize;
	
	float fAlpha = LitModelAlpha;
	
	float4 vDiffuse = tex2D(sampDiffuse, input.uv);

	float4 vVertColor 		= float4(1,1,1,1);
	float4 vLitMapColor 	= float4(1,1,1,1);
	float4 vShadow 			= float4(1,1,1,1);
	
	if(LitModelUseVerlit > 0.5)
		vVertColor.rgb = input.diffuse.rgb;

	float3 normal = normalize(input.normal);

	if(LitModelUseLightMap > 0.5)
	{
		float4 r1 = tex2D(sampLitMap1, input.uvlm) * LitModelLightMapParam.x;
		float4 r2 = tex2D(sampLitMap2, input.uvlm) * LitModelLightMapParam.y;
		vLitMapColor = lerp(r1, r2, LitModelLightMapParam.a);
		
		// Add a little highlight
		//float NdotL = -dot(normal, g_lightDir);
	    //float fLit = max(0, NdotL - 0.5) * 2;//max(0, dot(vLitMapColor.rgb, float3(1, 1, 1)) / 6 - 0.5) * 2;
	    //fLit *= fLit;
	    //fLit *= fLit;
	    //vLitMapColor.rgb *= (1 + fLit * 0.5);

	}
	
	
	if( (LitModelUseShadowMap > 0.5) && dot(g_lightDir, normal) < 0)
	{
		vShadow = glb_ComputShadowColor(sampShadowMap, sampShadowMapFloat, input.shadowProj, 
			true, LitModelUseHWPCF > 0.5, fShadowMapSize, LitModelShadowSampleNum);	
	}
	
	float4 r;
	r.rgb = vDiffuse.rgb * vVertColor.rgb * vLitMapColor.rgb * vShadow.rgb;
	r.a = vDiffuse.a * fAlpha;
		
	r.rgb = lerp(input.fogParam.rgb, r.rgb, input.fogParam.a);

	return r;
}

