/*-----------------------------------------------------------------
	Freeze VFX Shader
	Devleoped by wei wang (Engine Center)
	Perfect World, 2012-10-11
	TEST PS 2.0/3.0
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

float g_SpecPower
<
	string UIName =  "高光面积";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 200;
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

float4 g_Color
<
	string UIName =  "颜色";
	string UIWidget = "Color";
> = {1.0f,1.0f,1.0f,1.0f};

float g_DetailNormalUVScale
<
	string UIName =  "细节法线UV倍增";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 10.0;
	float UIStep = 0.01;
> = 3.0f;

float g_DetailNormalPower
<
	string UIName =  "细节法线影响度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.5f;

float g_RefractionRatio
<
	string UIName =  "折射度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.0f;

float4 g_colOutLiner
<
	string UIName =  "外轮廓颜色";
	string UIWidget = "Color";
> = {0.5f, 0.5f, 0.5f, 1.0f};

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
	string UIName =  "外轮廓对比度";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 5.0;
	float UIStep = 0.01;
> = 2.0f;

float g_SelfIlum
<
	string UIName =  "自发光";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 5.0;
	float UIStep = 0.01;
> = 1.0f;

float g_Alpha
<
	string UIName =  "整体透明度";
	string UIWidget = "slider";
	float UIMin  = 0.0;
	float UIMax  = 1.0;
	float UIStep = 0.01;
> = 1.0;


//-----------------------------------------------------------------
// Texture and Sampler
//-----------------------------------------------------------------
// for PRG
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

sampler g_NormalSampler : NormalMap_GFX_Replace
<
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

//For Artist
texture g_ReflectTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "反射贴图";
>;

sampler g_ReflectSampler
<
    string Texture = "g_ReflectTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

texture g_DiffuseTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "漫反射贴图";
>;

sampler g_DiffuseSampler
<
    string Texture = "g_DiffuseTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

texture g_DetailNormalTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "细节法线贴图";
>;

sampler g_DetailNormalSampler
<
    string Texture = "g_DetailNormalTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

texture g_AlphaTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "整体透明贴图";
>;

sampler g_AlphaMapSampler
<
    string Texture = "g_AlphaTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

float3 _ScreenBlendSelf(float3 base,float3 blend)
{
	//photoshop screen :
	//(1.0f-(1.0f-base)*(1.0f-blend));
	return (1.0f-(1.0f-base)*(1.0f-blend));
}

//-----------------------------------------------------------------
// PS
//-----------------------------------------------------------------
float4 ps_main(psInput Input): COLOR0
{

	float4 ResultColor =0.0f;
	
	//Tangent space L N V
	float3 vLightDir = normalize(Input.LightDir.xyz);
	float3 ViewDir   = normalize(Input.ViewDir.xyz);

	float3 vNormal;
	vNormal.rg = tex2D(g_NormalSampler, Input.uvBase.xy).ag * 2 - 1;
	vNormal.b = sqrt(1 - dot(vNormal.rg, vNormal.rg));
		
	float3 vDetailNormal;
	vDetailNormal.rg = tex2D(g_DetailNormalSampler, Input.uvBase.xy*g_DetailNormalUVScale).ag * 2 - 1;
	vNormal.rg = vNormal.rg +vDetailNormal.rg*g_DetailNormalPower;
	vNormal = normalize(vNormal);
	
	//Lighting
	float  fNDotL = dot(vNormal,vLightDir);
	// Specular
	float3 vLightReflection = -reflect(vLightDir, vNormal.xyz);
	float  fRDotV = fNDotL > 0.0f ? saturate(dot(vLightReflection, ViewDir)) : 0.0f;
	float  Specular = pow(fRDotV, g_SpecPower) * g_SpecValue;
	
	float  fNDotV =  dot(vNormal,ViewDir);	
	float  OutLiner = pow(1-fNDotV,g_OutPower)-g_OutLineOffset;
	float4 OutLinerColor =OutLiner*g_colOutLiner;
	
	#ifdef _SDR_3_0
		float4 ScreenPos = Input.vScreenPos;
		float4 BackBuffer  = tex2D(g_FrameBuffer, glb_GetScreenUV(ScreenPos.xy)+vNormal.rg*g_RefractionRatio);
		float4 OBackBuffer = tex2D(g_FrameBuffer, glb_GetScreenUV(ScreenPos.xy));
	#endif

	float4 diffColor =tex2D(g_DiffuseSampler, Input.uvBase.xy);
	diffColor*=g_Alpha;

	float AlphaMaskMap = tex2D(g_AlphaMap, Input.uvBase.xy).a;
	
	#ifdef _SDR_3_0
		//BackBuffer =BackBuffer;
		ResultColor =(BackBuffer+Specular)*g_Color+OutLinerColor;
		//ResultColor.rgb =_ScreenBlendSelf(diffColor.rgb,ResultColor.rgb);
		//Alpha Control
		ResultColor.rgb = lerp(OBackBuffer.rgb,ResultColor.rgb,AlphaMaskMap*g_Alpha);
		
		ResultColor.a   = AlphaMaskMap;
	#else
		diffColor =diffColor+OutLinerColor;
		ResultColor =(diffColor+Specular)*g_Color;
		//ResultColor.rgb =_ScreenBlendSelf(diffColor.rgb,ResultColor.rgb);
		//Alpha Control
		//ResultColor.a   =  dot(tex2D(g_AlphaMap_GFX_Replace, Input.uvBase.xy),g_AlphaMask)*0.5;
		ResultColor.a  = AlphaMaskMap*g_Alpha;
	#endif
	
	return ResultColor;
}