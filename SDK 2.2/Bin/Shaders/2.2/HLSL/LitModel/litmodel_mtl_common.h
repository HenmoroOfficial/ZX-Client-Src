#ifndef _LITMODEL_MTL_COMMON_H_
#define _LITMODEL_MTL_COMMON_H_

float LitModelUseShadowMap 		: GlobalUseShadowMap : register(c0);
float LitModelUseHWPCF 			: GlobalUseHWPCF : register(c1);
float LitModelShadowMapSize 	: GlobalShadowMapSize : register(c2);
float LitModelShadowSampleNum	: GlobalShadowSampleNum : register(c3);

float LitModelUseLightMap 		: LitModelUseLightMap : register(c4);
float LitModelUseVerlit 		: LitModelUseVerlit : register(c5);
float LitModelAlpha 			: LitModelAlpha : register(c6);
float4 g_vDetailParam           : register(c7);

sampler sampDiffuse 	: LitModelDiffuseTexture : register(s0);
sampler sampLitMap1  	: LitModelLightMap1 : register(s1);
sampler sampLitMap2  	: LitModelLightMap2 : register(s2);
sampler	sampShadowMap			: GlobalShadowMap : register(s3);
sampler sampShadowMapFloat		: GlobalShadowMapFloat : register(s4);
sampler sampDetail		: register(s5);


#endif