/*-----------------------------------------------------------------
	Stone VFX Shader(2.2)
	Instruction: 61 (Warning)
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
};


//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------


float g_MaskSpan
<
	string UIName =  "消隐跨度";
	string UIWidget = "slider";
	float UIMin  = 0.0;
	float UIMax  = 1.0;
	float UIStep = 0.01;
> = 0.4;

float g_edgeContrast
<
	string UIName =  "边缘对比度";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 5.0;
	float UIStep = 0.01;
> = 1.0f;

float4 g_edgeColor
<
	string UIName =  "边缘颜色";
	string UIWidget = "Color";
> = {1.0, 1.0, 1.0, 0.0};


float g_DiffuseUVScale
<
	string UIName =  "石化贴图UV倍增";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 10;
	float UIStep = 0.01;
> = 1.0f;


float g_DiffuseNormalScale
<
	string UIName =  "石化法线强度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 2.0;
	float UIStep = 0.01;
> = 1.0f;

float g_SpecPower
<
	string UIName =  "高光面积";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 100;
	float UIStep = 0.01;
> = 50.0f;

float g_SpecValue
<
	string UIName =  "高光倍增";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 2;
	float UIStep = 0.01;
> = 1.0f;

float g_TimerValue
<
	string UIName =  "时间滑块";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.5;
	float UIStep = 0.01;
> = 1.5f;

//-----------------------------------------------------------------
// Texture and Sampler
//-----------------------------------------------------------------
// for PRG
sampler g_NormalMap : NormalMap_GFX_Replace
<
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


//For Artist
texture g_GradientDissolveTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Vfx\\GradientDissolve.dds";
    string UIName =  "石化渐变贴图";
>;

texture g_InnerTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "石化漫反射贴图";
>;

texture g_NormalTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "附加法线贴图";
>;

texture g_edgeTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Vfx\\edge.dds";
    string UIName =  "边缘控制贴图";
>;


sampler g_GradientDissolveSampler
<
    string Texture = "g_GradientDissolveTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler g_edgeSampler
<
    string Texture = "g_edgeTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "CLAMP";
    string AddressV = "CLAMP";
>;

sampler g_InnerSampler
<
    string Texture = "g_InnerTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler g_NormalSampler
<
    string Texture = "g_NormalTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

//-----------------------------------------------------------------
// PS
//-----------------------------------------------------------------
float4 ps_main(psInput Input) : COLOR0
{
	float4 edgeColor= 0.0f;
	float4 ResultColor =0.0f;

	float2 NewDiffuseUV=Input.uvBase.xy*g_DiffuseUVScale;
	//Tangent space L N V
	float3 vLightDir = normalize(Input.LightDir.xyz);
	float3 ViewDir   = normalize(Input.ViewDir.xyz);
	float3 vNormal;
	vNormal.rg = tex2D(g_NormalMap, Input.uvBase.xy).ag*2-1;
	vNormal.b  = sqrt(1 - dot(vNormal.rg, vNormal.rg));
	
	float3 vNormalDetail;
	vNormalDetail.rg = tex2D(g_NormalSampler, NewDiffuseUV).ag*2-1;
	vNormal.rg = vNormal.rg +vNormalDetail.rg*g_DiffuseNormalScale;
	vNormal = normalize(vNormal);
	
	//Using Vertex Light
	float fNDotL = dot(vNormal.xyz, vLightDir.xyz);
	float HalfLambert = fNDotL*0.5+0.5;
	// Specular
	float3 vLightReflection = -reflect(vLightDir, vNormal.xyz);
	float fRDotV = fNDotL > 0.0f ? saturate(dot(vLightReflection, ViewDir)) : 0.0f;
	float Specular = pow(fRDotV, g_SpecPower) * g_SpecValue;
	
	//Calculate Edge Color
	edgeColor =glb_GetEdgeColorCustom(g_GradientDissolveSampler,g_edgeSampler,Input.uvBase.xy,g_edgeContrast,g_MaskSpan,g_edgeColor,g_TimerValue);
	//Calculate Inner Color
  float4 InnerDiffuseTex = tex2D(g_InnerSampler, NewDiffuseUV);
  InnerDiffuseTex.xyz =InnerDiffuseTex.xyz*HalfLambert +Specular;
  
	//Comp
	ResultColor.rgb = edgeColor+InnerDiffuseTex;
	float alpha = tex2D(g_AlphaMap, Input.uvBase.xy).a;
 	alpha= alpha > 0.329411764 ? edgeColor.a:0.0; //0.329411764 =84/255 AlphaCompare
	ResultColor.a=alpha;
	
	//Fog
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	
	return ResultColor;
}