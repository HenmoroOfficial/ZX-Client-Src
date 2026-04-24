/*-----------------------------------------------------------------
	去色/饱和度 增强对比度
	Instruction: 15(3.0)
	RTX :wei wang (Engine Center)
	Perfect World
-----------------------------------------------------------------*/
string entrynames = "ps_2_0 ps_main";
//-----------------------------------------------------------------
// Include
//-----------------------------------------------------------------
#include "gfx_common.h"
#include "../SkinModel/Mesh_Comm.h"

//-----------------------------------------------------------------
// psInput
//-----------------------------------------------------------------
#ifdef _SDR_3_0
	struct psInput 
	{
		float4 pos	            : POSITION0;
		float2 uvBase           : TEXCOORD0;
		float3 ViewDir          : TEXCOORD1;
		float3 LightDir         : TEXCOORD2;
		float4 PtLightDir       : TEXCOORD3;
		float4 tangent					: TEXCOORD4;
		float3 binormal					: TEXCOORD5;
		float4 fogParam					: TEXCOORD6;
		float4 vScreenPos				: VPOS;
	};
#else
	struct psInput 
	{
		float4 pos	            : POSITION0;
		float2 uvBase           : TEXCOORD0;
		float3 ViewDir          : TEXCOORD1;
		float3 LightDir         : TEXCOORD2;
		float4 PtLightDir       : TEXCOORD3;
		float4 tangent					: TEXCOORD4;
		float3 binormal					: TEXCOORD5;
		float4 fogParam					: TEXCOORD6;
	};
#endif

//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------
float g_BlendFactor
<
	string UIName =  "颜色饱和度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.0f;

float g_Power
<
	string UIName =  "颜色对比度";
	string UIWidget = "slider";
	float UIMin = 0.5;
	float UIMax = 10.0;
	float UIStep = 0.01;
> = 1.0f;

float g_LightPower
<
	string UIName =  "亮度倍增";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 10.0;
	float UIStep = 0.01;
> = 1.0f;

//-----------------------------------------------------------------
// Texture and Sampler
//-----------------------------------------------------------------
sampler g_FrameBuffer:FrameBuffer_GFX_Replace
<
    string Texture = "g_FrameBufferTex";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler g_AlphaMap : DiffuseMap_GFX_Replace
<
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;


float4 ps_main(psInput Input): COLOR0
{
	float4 ResultColor =1.0f;
	#ifdef _SDR_3_0
		float4 ScreenPos =Input.vScreenPos;
		float4 BackImg = tex2D(g_FrameBuffer, glb_GetScreenUV(ScreenPos.xy));
		float3 GrayColor = dot(BackImg.rgb,float3(0.3,0.59,0.11));
		ResultColor.rgb = lerp(BackImg.rgb,GrayColor,g_BlendFactor);
		ResultColor.rgb = pow(ResultColor.rgb,g_Power)*g_LightPower;
	#else
		float4 InnerDiffuseTex = tex2D(g_AlphaMap, Input.uvBase.xy);
		float3 GrayColor = dot(InnerDiffuseTex.rgb,float3(0.3,0.59,0.11));
		ResultColor.rgb =  pow(GrayColor.rgb,g_Power)*g_LightPower;
		ResultColor.a    = InnerDiffuseTex.a;
	#endif	
	//Add Fog
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	return ResultColor;
}