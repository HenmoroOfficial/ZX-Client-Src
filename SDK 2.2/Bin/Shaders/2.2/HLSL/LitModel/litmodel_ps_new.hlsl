#include "..\Shadow\Shadow_common.h"
#include "litmodel_common_new.h"
#include "litmodel_mtl_common.h"
string entrynames = "ps_2_0 ps_main";	

float3 g_lightDir : LightDir;


float4 ps_main(VS_OUTPUT input) : COLOR0
{
	float fShadowMapSize = LitModelShadowMapSize;
	
	float4 vDiffuse = tex2D(sampDiffuse, input.uv);

    float4 color = vDiffuse * input.diffuse;
	

#ifdef _DETAIL_MAP_
    float fTile = g_vDetailParam.x;
	float fHardness = g_vDetailParam.y;

    float2 vDetailTex1 = input.uv * fTile;
    float2 vDetailTex2 = vDetailTex1 * 10;

    float4 detail = tex2D(sampDetail, vDetailTex1);
    
    color.rgb *= (detail.rgb - 0.5) * (fHardness * 0.5) + 1;

    float4 detail2 = tex2D(sampDetail, vDetailTex2);
    
    color.rgb *= (detail2.rgb - 0.5) * fHardness + 1;
#endif    
    
	float3 normal = input.normal;
    
#ifdef _LIGHT_MAP_
	float4 r1 = tex2D(sampLitMap1, input.uvlm) * LitModelLightMapParam.x;
	float4 r2 = tex2D(sampLitMap2, input.uvlm) * LitModelLightMapParam.y;
	float4 vLitMapColor = lerp(r1, r2, LitModelLightMapParam.a);
    //return ((vLitMapColor - 0.5) * 1.5 + 0.5) * 0.5;
    color *= vLitMapColor;	
	// Add a little highlight
	//float NdotL = -dot(normal, g_lightDir);
    //float fLit = max(0, NdotL - 0.5) * 2;//max(0, dot(vLitMapColor.rgb, float3(1, 1, 1)) / 6 - 0.5) * 2;
    //fLit *= fLit;
    //fLit *= fLit;
    //vLitMapColor.rgb *= (1 + fLit * 0.5);
#endif

#ifdef _SHADOW_MAP_	
    bool bUseHWPCF = false;
#ifdef _HWPCF_
    bUseHWPCF = true;    
#endif
    int nSampleCount = 1;
#ifdef _SHADOW_HIGH_
    nSampleCount = 5;
#endif    
	if(dot(g_lightDir, normal) < 0)
	{
		float4 vShadow = glb_ComputShadowColor(sampShadowMap, sampShadowMapFloat, input.shadowProj, 
			true, bUseHWPCF, fShadowMapSize, nSampleCount);	
        color *= vShadow;			
	}
#endif	
	
	color.rgb = lerp(input.fogParam.rgb, color.rgb, input.fogParam.a);

	return color;
}

