#include "../common/common.h"
#include "../SkinModel/mesh_comm.h"

string entrynames = "ps_2_0 ps_main; ps_2_0 ps_bloom_mask";
#define POINT_LIGHT_ENABLE

float4 g_colBloom
<
	string UIName =  "(测试使用)泛光颜色";
	string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

//float4 g_vEyePos : CameraPosition;
//float4 g_MainLightPos : LightPosition0;		//	main light's position
float4 g_colDiffuse : Diffuse = {1.0f, 1.0f,1.0f,1.0f};
//float4 g_colSpecular : LightColor1 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colAmbient : Ambient0 = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_Ambient2 : Ambient1 = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_colPtDiffuse : PointDiffuse;
float g_fDiffusePower : DirPower;

//float4 g_Ambient2 
//<
//	string UIName =  "测试环境光2";
//	string UIWidget = "Color";
//>
//= {0.0, 0.0, 0.0, 0.0};

float4x4 g_matWorldInv : WorldInverse;
float4x4 g_matWorldView : WorldView;
float4x4 g_matWorldViewProj : WorldViewProjection;
float4 g_colDiffuseFactor : DiffuseFactor = float4(1,1,1,1);
float4 g_colEmissiveFactor : EmissiveFactor = float4(0,0,0,0);

texture g_DiffuseTexture;

sampler2D g_DiffuseSampler : DiffuseMap
<
    string Texture = "g_DiffuseTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

//struct VS_OUTPUT 
//{
//	float4 Position : POSITION;
//	float2 uvBase : TEXCOORD0;
//	float3 ViewDir	: TEXCOORD1;
//	float3 LightDir	: TEXCOORD2;
//	float4 PtLightDir : TEXCOORD3;
//	float4 tangent : TEXCOORD4;
//	float3 binormal : TEXCOORD5;
//	float4 fogParam : TEXCOORD6;
//};

float4 ps_main(PS_INPUT Input) : COLOR0
{      
	float3 Normal = float3(0, 0, 1);
	
	float3 LightDir	= normalize(Input.LightDir);
	float4 colBaseTex = tex2D(g_DiffuseSampler, Input.uvBase);
	
	float fNDotL = dot(Normal, LightDir) * g_colBloom.a;
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	fNDotL = saturate(fNDotL);

	float3 colDiffusePoint = 0.0f;
	
#ifdef POINT_LIGHT_ENABLE
	float3 PtLightDir        = normalize(Input.PtLightDir.rgb);
	float  fPtNDotL          = dot(Normal, PtLightDir);
	fPtNDotL = saturate(fPtNDotL);
  colDiffusePoint = g_colPtDiffuse.rgb * fPtNDotL * Input.PtLightDir.w;
#endif
  
	float3 colDiffuse = (g_fDiffusePower * g_colDiffuse.rgb * fNDotL + colDiffusePoint);
	float3 colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);

	float3 ResultColor = (colDiffuse.rgb + colAmbient.rgb) * colBaseTex.rgb;
	
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	return float4(ResultColor, colBaseTex.a) * g_colDiffuseFactor + g_colEmissiveFactor;
}


float4 vBloomCol;

float4 ps_bloom_mask() : COLOR0
{
	return float4(vBloomCol.x, vBloomCol.y, vBloomCol.z, 1.0);
}