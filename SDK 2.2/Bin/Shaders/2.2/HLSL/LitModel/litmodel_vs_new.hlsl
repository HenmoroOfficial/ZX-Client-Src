#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"
#include "litmodel_common_new.h"

string entrynames = "vs_2_0 vs_main";	


float4x4 matViewProj;
float4x4 matLightWVP;
float4x4 matWorld;
float3 g_vecEyePos;
float  g_fGamma;

float4 g_colDiffuse : Diffuse;
//float4 g_colSpecular : LightColor1 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colAmbient : Ambient0;
float4 g_Ambient2 : Ambient1;
float3 g_vLightDir : LightDir;
float g_fDiffusePower : DirPower;
float g_LitModelAlpha : LitModelAlpha;
    
struct VS_INPUT
{
	float4 pos          : POSITION;
	float3 normal		: NORMAL;
    float2 uv           : TEXCOORD0;
#ifdef _LIGHT_MAP_
	float2 uvlm			: TEXCOORD1;
#endif	
#ifdef _VERT_LIT_
	float3 diffuse		: COLOR0;
#endif	
};

VS_OUTPUT vs_main(VS_INPUT input)
{
	VS_OUTPUT output = (VS_OUTPUT)0;
	
	float fAlpha = 1;
	
    float4 vWorldPos = mul(input.pos, matWorld);
	output.normal = mul(input.normal, (float3x3)matWorld);
	output.pos = mul(vWorldPos, matViewProj);
	output.uv = input.uv;

#if defined(_REAL_TIME_)// || defined(_LIGHT_MAP_)
    output.normal = normalize(output.normal);
	float fNDotL = -dot(output.normal, g_vLightDir);
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	fNDotL = pow(saturate(fNDotL), 0.5);

	float3 colDiffuse = (g_fDiffusePower * g_colDiffuse.rgb * fNDotL);
	float3 colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);
    float3 diffuse = colDiffuse.rgb + colAmbient.rgb;
    diffuse = pow(diffuse, g_fGamma);
	output.diffuse = float4(diffuse, fAlpha);
#elif defined(_LIGHT_MAP_)
	output.uvlm = input.uvlm;
	output.diffuse = float4(1, 1, 1, fAlpha);
#elif defined(_VERT_LIT_)	
    float maxLight = LitModelLightMapParam.z;
	output.diffuse = float4(input.diffuse * maxLight, fAlpha);
#else
	output.diffuse = float4(1, 1, 1, fAlpha);
#endif
    
#ifdef _SHADOW_MAP_	
	output.shadowProj  = glb_ComputeShadowProj(float4(vWorldPos.xyz, 1), matLightWVP);
#endif
	
	output.fogParam = ComputeCompositeFogSimple(vWorldPos.xyz - g_vecEyePos, vWorldPos.y);

	return output;
}