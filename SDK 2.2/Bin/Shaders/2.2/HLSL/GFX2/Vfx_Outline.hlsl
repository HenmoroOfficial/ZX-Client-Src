/*-----------------------------------------------------------------
	OutLine VFX Shader(2.2)
	Instruction: 18
	Devleoped by wei wang (Engine Center)
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

//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------
float3 g_ColorOffset
<
	string UIName =   "勾边颜色增强";
	string UIWidget = "Vector";
> =  {3.0f, 2.0f, 1.0f};

float4 g_colOutLiner
<
	string UIName =  "外轮廓颜色";
	string UIWidget = "Color";
> = {0.5f, 0.5f, 0.5f, 1.0f};

float4 g_colInner
<
	string UIName =  "内轮廓颜色";
	string UIWidget = "Color";
> = {0.5f, 0.0f, 0.0f, 1.0f};


float g_OutLineOffset
<
	string UIName =  "轮廓偏移";
	string UIWidget = "slider";
	float UIMin  = -1.0;
	float UIMax  = 1.0;
	float UIStep = 0.01;
> = 1.0;

float g_OutPower
<
	string UIName =  "内外轮廓对比度";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 5.0;
	float UIStep = 0.01;
> = 2.0f;

float g_Alpha
<
	string UIName =  "整体透明度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 1.0f;


//-----------------------------------------------------------------
// Texture and Sampler
//-----------------------------------------------------------------
//For PRG
sampler g_AlphaMap : DiffuseMap_GFX_Replace
<
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;


//-----------------------------------------------------------------
// PS MAIN
//-----------------------------------------------------------------

float4 ps_main(psInput Input) : COLOR0
{
	float4 ResultColor =1.0f;
	float4 Outline     =1.0f;
	
	//Tangent space L N V
	float3 vLightDir = normalize(Input.LightDir.xyz);
	float3 ViewDir   = normalize(Input.ViewDir.xyz);
	float3 vNormal   = {0.0f,0.0f,1.0f};
	
	//saturate NdotV
	float  fNDotV = dot(vNormal,ViewDir);
	fNDotV = saturate(fNDotV);
	
	//Calculate Outline Color
	float OutLiner = pow(1-fNDotV,g_OutPower)-g_OutLineOffset;
	float4 OutLinerCol =lerp(g_colInner,g_colOutLiner,OutLiner);

	//Model Alpha Texture
	float alpha = tex2D(g_AlphaMap, Input.uvBase.xy).a;
 	alpha= alpha > 0.329411764 ? 1.0:0.0; //0.329411764 =84/255 AlphaCompare
	//Comp
	ResultColor.rgb = OutLinerCol.rgb;
	//Fog
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	ResultColor.a=alpha*g_Alpha*OutLinerCol.a;

	return ResultColor;
}