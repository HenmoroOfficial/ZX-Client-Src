#include "..\Shadow\Shadow_common.h"
#include "..\fog\fog_common.h"

struct VS_INPUT
{
	float3 Position : POSITION;
	float3 Normal	: NORMAL;
	float2 uv0		: TEXCOORD0;
	float2 uv1		: TEXCOORD1;
	float2 uv2		: TEXCOORD2;
	float2 uv3		: TEXCOORD3;
};

struct VS_OUTPUT
{
	float4 oPos		: POSITION;
	float4 oSpecular: COLOR0;		// specular
	float4 uv01		: TEXCOORD0;
	float4 uv23		: TEXCOORD1;
	float4 uv45		: TEXCOORD2;
	float2 uvMask	: TEXCOORD3;
  	float4 oDiffuse	: TEXCOORD4;		// diffuse

	//Shadow
	float4	shadowProj					: TEXCOORD5;
    float4 fogParam : TEXCOORD6;
};

struct PS_INPUT
{
	float4 oSpecular: COLOR0;		// specular
	float4 uv01		: TEXCOORD0;
	float4 uv23		: TEXCOORD1;
	float4 uv45		: TEXCOORD2;
	float2 uvMask	: TEXCOORD3;
	float4 oDiffuse	: TEXCOORD4;		// diffuse
	    
	//Shadow
	float4	shadowProj					: TEXCOORD5;
    float4 fogParam : TEXCOORD6;
};



float4x4 g_matViewProjection;
float3   g_vecEyePos;
float3   g_vecMainLightDir;
float4   g_colMtlDiffuse;
float4   g_colMtlSpecular;
float    g_fSpecPower;
const float	g_fColTexMax = 1.9921875;	// See A3DTerrainBlk.h
float4x4 g_matLightWVP;	//Shadow

float4 g_colDiffuse : Diffuse;
float4 g_colAmbient : Ambient0;
float4 g_Ambient2 : Ambient1;
float g_fDiffusePower : DirPower;
float3 g_vLightDir : LightDir;


float glb_CalcHDotN(float3 vNormal, float3 vLightDir, float3 vViewDir)
{
	float3 vHalf = normalize(vLightDir + vViewDir);
	return saturate(dot(vHalf, vNormal));
}

float4 glb_CalcFinalOutput(float4 color, float3 lmColor, float3 specular, float3 diffuse, float4 shadowColor)
{
	float3 finalCol = color.rgb + color.a * specular * lmColor;
	finalCol = finalCol * diffuse;
	finalCol = finalCol * shadowColor.rgb;
	return float4(finalCol * lmColor, 1);
}

void glb_vs_Common(VS_INPUT input, inout VS_OUTPUT Output)
{
	Output.oPos = mul( float4(input.Position, 1), g_matViewProjection );
	float3 vCameraToWorldPos = input.Position - g_vecEyePos;
	
#ifdef _REAL_TIME_	
	float fNDotL = -dot(input.Normal, g_vLightDir);
	
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	fNDotL = pow(saturate(fNDotL), 0.5);

	float3 colDiffuse = (g_fDiffusePower * g_colDiffuse.rgb * fNDotL);
	float3 colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);
    float3 diffuse = colDiffuse.rgb + colAmbient.rgb;

	Output.oSpecular = 0;
	Output.oDiffuse = float4(diffuse, 1);
#else
	Output.oDiffuse = float4(1, 1, 1, 1);	//diffuse
	float3 vViewDir = -normalize(vCameraToWorldPos);
	float fHDotN = glb_CalcHDotN(input.Normal, -g_vLightDir, vViewDir);
	Output.oSpecular = pow(fHDotN, g_fSpecPower) * g_colMtlSpecular;	//specular
#endif

	
	//Shadow
	Output.shadowProj = glb_ComputeShadowProj(float4(input.Position, 1), g_matLightWVP);
	Output.fogParam = ComputeCompositeFogSimple(vCameraToWorldPos, input.Position.y);

}