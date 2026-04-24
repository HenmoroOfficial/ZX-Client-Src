#include "../common/common.h"
#include "../SkinModel/mesh_comm.h"

string entrynames = "ps_2_0 ps_main";

//-----------------------------------------------------------------
// macro
//-----------------------------------------------------------------
//                PRG Macros
//Point Light
string macro0
<
	string name = "POINT_LIGHT_ENABLE";
	string desc = "点光源支持";
	string type = "PRG";
	string GroupID = "1";
>;

//Self-Illuminated
string macro1
<
	string Name = "SELF_ILLUMINATED_ENABLE";
	string Type = "PRG";
	string Desc = "使用自发光颜色";
	string GroupID = "2";
>;
//Color Tone Tuning
string macro2
<
	string Name = "COLOR_TONE_ENABLE";
	string Type = "PRG";
	string Desc = "程序色调调色";
	string GroupID = "3";
>;
//Color multiply Tuning
string macro12
<
	string Name = "COLOR_MULTIPLY_ENABLE";
	string Type = "PRG";
	string Desc = "程序正片叠底调色";
	string GroupID = "3";
>;
//---------------------------------------------
//                ART Macros

//Specular
string macro3
<
	string Name = "SPECULAR_ENABLE";
	string Type = "ART";
	string Desc = "使用高光";
	string GroupID = "4";
>;

string macro4
<
	string Name = "SPECULAR_MAP_ENABLE";
	string Type = "ART";
	string Desc = "使用高光贴图";
	string GroupID = "4";
>;


//Simple No Normal Map
string macro_No_Normal
<
	string Name = "NO_NORMAL_ENABLE";
	string Type = "ART";
	string Desc = "不使用法线贴图";
	string GroupID = "7";
>;

/*
//Use detail Normal Map
string macro_Detail_Normal
<
	string Name = "DETAIL_NORMAL_ENABLE";
	string Type = "ART";
	string Desc = "使用细节法线贴图";
	string GroupID = "7";
>;
*/

//#define POINT_LIGHT_ENABLE
//#define SPECULAR_MAP_ENABLE
//#define SPECULAR_ENABLE
//#define SELF_ILLUMINATED_MAP_ENABLE
//#define NO_NORMAL_ENABLE
//#define COLOR_TONE_ENABLE
//#define DETAIL_NORMAL_ENABLE

float4 g_colDiffuse : Diffuse = {1.0f, 1.0f,1.0f,1.0f};
float  g_fDiffusePower : DirPower;
float4 g_colAmbient : Ambient0 = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_Ambient2 : Ambient1;
float4 g_colPtDiffuse:PointDiffuse = {0.88,0.88,0.88,1};
float4x4 g_matColorTone : ColorToneMatrix = {1.0f, 0.0f, 0.0f, 0.0f,
																						 0.0f, 1.0f, 0.0f, 0.0f,		
																						 0.0f, 0.0f, 1.0f, 0.0f,
																						 0.0f, 0.0f, 0.0f, 1.0f};
float4 g_colDiffuseFactor : DiffuseFactor = float4(1,1,1,1);
float4 g_colEmissiveFactor : EmissiveFactor = float4(0,0,0,0);

//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------
//Bump
float g_smoothNess
<
	string UIName =  "表面光滑度";
	string UIWidget = "slider";
	float UIMin  = 1.0;
	float UIMax  = 8.0;
	float UIStep = 0.01;
> = 1.0;

//SSS
float g_Translucency
<
	string UIName =  "皮肤透光度";
	string UIWidget = "slider";
	float UIMin  = 0.0;
	float UIMax  = 1.0;
	float UIStep = 0.01;
> = 0.5;

//Rim Light
float4 g_RimColor
<
	string UIName =  "Rim Light外部颜色";
	string UIWidget = "color";
> = {0.0f, 0.5f, 0.5f, 1.0f};

//Specular 
float4 g_colMtlSpecular 
<
	string UIName =  "高光颜色";
	string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float g_Power
<
	string UIName =  "高光power";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 100.0;
	float UIStep = 0.1;
> = 50.0f;

float g_SpecularMultiple
<
	string UIName =  "高光增强倍数";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 5.0;
	float UIStep = 0.01;
> = 1.0f;

float g_uDetail
<
	string UIName =  "细节U重复倍数";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 100.0;
	float UIStep = 0.1;
> = 10.0f;

float g_vDetail
<
	string UIName =  "细节V重复倍数";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 100.0;
	float UIStep = 0.1;
> = 10.0f;

//SelfIlluminate

float4 g_SelfIlluminateColor
<
	string UIName =  "自发光颜色";
	string UIWidget = "Color";
> = {0.0, 0.0, 0.0, 0.0};

//Color Tone Tuning
float4 g_SrcColorTone
<
	string UIName =  "原始色调";
	string UIWidget = "color";
> = {1.0f, 0.0f, 0.0f, 1.0f};

float4 g_DestColorTone
<
	string UIName =  "目标色调";
	string UIWidget = "color";
> = {1.0f, 0.0f, 0.0f, 1.0f};

//-----------------------------------------------------------------
// Matrix
//-----------------------------------------------------------------
float4x4 g_matWorldInv      : WorldInverse;
float4x4 g_matWorldView     : WorldView;
float4x4 g_matWorldViewProj : WorldViewProjection;

//-----------------------------------------------------------------
// Texture and Sampler
//-----------------------------------------------------------------
texture g_DiffuseTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Common\\Fieldstone.dds";
    string UIName =  "基本贴图";
>;

texture g_NormalTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "法线贴图";
>;

texture g_DetailNormalTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\noise.dds";
    string UIName =  "细节法线贴图";
>;

texture g_MaskTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Common\\white.dds";
    string UIName =  "Mask贴图";
>;

texture g_SpecularTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Common\\Fieldstone_Specular.dds";
    string UIName =  "高光贴图";
>;

texture g_SkinWarpTexture
<
	  string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\SkinLookUpTable\\sss.tga";
    string UIName   = "皮肤查找表";
>;

texture g_RimLightTexture
<
	  string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\RimLookUpTable\\rim_out_0.dds";
    string UIName   = "RimLight外部贴图";
>;

sampler2D g_SkinWarpSampler
<
    string Texture = "g_SkinWarpTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "CLAMP";
    string AddressV = "CLAMP";
>;

sampler2D g_DiffuseSampler : DiffuseMap
<
    string Texture = "g_DiffuseTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_NormalSampler : NormalMap
<
    string Texture = "g_NormalTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_DetailNormalSampler
<
    string Texture = "g_DetailNormalTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_MaskSampler
<
    string Texture = "g_MaskTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_SpecularSampler
<
    string Texture = "g_SpecularTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_RimLightTextureSampler 
<
    string Texture = "g_RimLightTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "CLAMP";
    string AddressV = "CLAMP";
>;

//-----------------------------------------------------------------
// PS Entry
//-----------------------------------------------------------------
float4 ps_main(PS_INPUT Input) : COLOR0
{
  float3 Normal      = 0.0;
  float3 NormalDetail = 0.0;
	float3 colDiffuse  = 0.0;
	float3 colAmbient  = 0.0;
	float3 colSpecular = 0.0;
	
	float3 colDiffusePoint = 0.0f;
	float3 colSpecularPoint = 0.0f;
	
	float4 RimLight   = 0.0;

	float3 colSunDiffuse = g_colDiffuse.rgb * g_fDiffusePower;

#ifdef NO_NORMAL_ENABLE
	Normal.z=1.0;
#else
	Normal   = glb_SamplerNormalMap(g_NormalSampler, Input.uvBase, g_smoothNess);
	#ifdef DETAIL_NORMAL_ENABLE
		NormalDetail = tex2D(g_DetailNormalSampler, Input.uvBase * float2(g_uDetail, g_vDetail)).agg * 2 - 1;
		Normal = normalize(Normal + float3(NormalDetail.x, NormalDetail.y, 0));
	#endif
#endif

	float3 LightDir	= normalize(Input.LightDir);
	float3 ViewDir  = normalize(Input.ViewDir);
	float3 LightReflection = glb_CalcReflection(Normal, LightDir);
	
	//Sample Tex
	float4 colBaseTex     = tex2D(g_DiffuseSampler, Input.uvBase);
	float4 colSpecularTex = tex2D(g_SpecularSampler, Input.uvBase);
	float4 colMaskTex			= tex2D(g_MaskSampler, Input.uvBase);
	
	float fNDotL = dot(Normal, LightDir);
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	float fRDotV = fNDotL > 0.0f ? saturate(dot(LightReflection, ViewDir)) : 0.0f;
	float fNdotV = saturate(dot(Normal,ViewDir));
	
	fNDotL = fNDotL*0.5f+0.5f;

//SSS Warp Skin Color
	//float4 colSkinTex = tex2D(g_SkinWarpSampler, float2(fNDotL,g_Translucency));
	float4 colSkinTex = tex2D(g_SkinWarpSampler, float2(fNDotL,0.0f))*fNDotL;

#ifdef COLOR_TONE_ENABLE
	float4 colorDiff = g_DestColorTone - g_SrcColorTone;
	float4 colorToTune;
	colorToTune.rgb = colBaseTex.rgb;
	colorToTune.a = 1.0f;
	colorToTune = mul(colorToTune ,g_matColorTone);
	colBaseTex.rgb = lerp(colBaseTex.rgb, colorToTune.rgb, colMaskTex.b);
	colorToTune.rgb = colSkinTex.rgb;
	colorToTune.a = 1.0f;
	colorToTune = mul(colorToTune, g_matColorTone);
	colSkinTex.rgb = colorToTune.rgb + colorDiff.rgb * g_matColorTone._14; // the g_matColorTone._14 is always zero, this is used to let compiler not get rid of g_SrcColorTone and g_DestColorTone;	
#endif

#ifdef COLOR_MULTIPLY_ENABLE
	colBaseTex.rgb = lerp(colBaseTex.rgb, colBaseTex.rgb * g_DestColorTone, colMaskTex.b);
	colSkinTex.rgb *= g_DestColorTone;
#endif

//Point Light
#if (defined(_SDR_3_0) && defined(POINT_LIGHT_ENABLE))  //Edited By Panyupin, 2013.1.29, for ps2.0
	float3 PtLightDir        = normalize(Input.PtLightDir.rgb);
	float3 PtLightReflection = glb_CalcReflection(Normal, PtLightDir);
	float  fPtNDotL          = dot(Normal, PtLightDir);
	float  fPtRDotV          = fPtNDotL > 0.0f ? saturate(dot(PtLightReflection, ViewDir)) : 0.0f;
	fPtNDotL = fPtNDotL*0.5f+0.5f;;
  colDiffusePoint = g_colPtDiffuse.rgb * lerp(0.5, 1, dot(PtLightDir, LightDir) * 0.5 + 0.5) * fPtNDotL * Input.PtLightDir.w;
  #if defined(SPECULAR_MAP_ENABLE) || defined(SPECULAR_ENABLE)
  	colSpecularPoint = g_colPtDiffuse.rgb * pow(fPtRDotV, g_Power) * Input.PtLightDir.w;
	#endif
#endif

//Diffuse
//colDiffuse = colSunDiffuse * lerp(fNDotL, colSkinTex.rgb, colMaskTex.b) + colDiffusePoint;
colDiffuse = colSunDiffuse * colSkinTex.rgb + colDiffusePoint;

//Ambient
colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);

//Specular
#ifdef _SDR_3_0
	#ifdef SPECULAR_MAP_ENABLE
		colSpecular = g_SpecularMultiple * colSpecularTex.rgb * (colSunDiffuse*pow(fRDotV, g_Power) + colSpecularPoint);
	#endif

	#ifdef SPECULAR_ENABLE
		colSpecular = g_SpecularMultiple * g_colMtlSpecular.rgb *(colSunDiffuse*pow(fRDotV, g_Power) + colSpecularPoint);
	#endif
#endif
//Rim Light
	RimLight.rgb = tex2D(g_RimLightTextureSampler, float2(fNdotV,0.0)).rgb*g_RimColor.rgb;
//	colDiffuse += RimLight.rgb*2.0;

//Combine Shader
float3 ResultColor = (colDiffuse.rgb + colAmbient.rgb)* colBaseTex.rgb +RimLight.rgb;

#if defined(SPECULAR_MAP_ENABLE)|| defined(SPECULAR_ENABLE)
  ResultColor += colSpecular.rgb;
#endif

#ifdef SELF_ILLUMINATED_ENABLE
	ResultColor += g_SelfIlluminateColor.rgb;
#endif

	// Do fog here
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	
	return float4(ResultColor, colBaseTex.a) * g_colDiffuseFactor + g_colEmissiveFactor;
}