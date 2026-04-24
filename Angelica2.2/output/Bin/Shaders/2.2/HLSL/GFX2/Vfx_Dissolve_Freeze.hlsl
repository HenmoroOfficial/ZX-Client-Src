/*-----------------------------------------------------------------
	Freeze VFX Shader(2.2)
	Instruction: 64 for ps3.0 63 for 2.0
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

float g_UseAlphaMap
<
	string UIName =  "使用内层漫反射Alpha通道控制透明度";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 1.0;
> = 0.0f;


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
	float UIMin = 0.0;
	float UIMax = 5.0;
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

float g_offsetBias
<
	string UIName =  "视差偏移";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.1f;

float g_ReflectRatio
<
	string UIName =  "反射率";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 1.0;
	float UIStep = 0.01;
> = 0.5f;

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
    string UIName =  "冰冻溶解贴图";
>;

texture g_InnerTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "内层漫反射贴图";
>;

texture g_InnerMaskTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "内层蒙板贴图";
>;

texture g_InnerFreezeTexture 
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "内层反射贴图";
>;

texture g_edgeTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Vfx\\edge.dds";
    string UIName =  "边缘控制贴图";
>;

texture g_ReflectTexture
<
		string UIWidget = "Texture";
    string UIObject = "Shaders\\textures\\Common\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "反射贴图";
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

sampler g_InnerMaskSampler
<
    string Texture = "g_InnerMaskTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler g_InnerFreezeSampler
<
    string Texture = "g_InnerFreezeTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
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

//-----------------------------------------------------------------
// PS
//-----------------------------------------------------------------
float4 ps_main(psInput Input) : COLOR0
{
	float4 colFire  = 0.0f;
	float4 edgeColor= 0.0f;
	float4 ResultColor =1.0f;
	float3 EdgeColor ={1.0,1.2,2.0};
	
	//Tangent space L N V
	float3 vLightDir = normalize(Input.LightDir.xyz);
	float3 ViewDir   = normalize(Input.ViewDir.xyz);
	float3 vNormal;
	vNormal.rg = tex2D(g_NormalMap, Input.uvBase.xy).ag * 2 - 1;
	vNormal.b = sqrt(1 - dot(vNormal.rg, vNormal.rg));
	
	float  fNDotV = dot(vNormal,ViewDir);
	//Using Vertex Light
	float  fNDotL = dot(vNormal,vLightDir);
	float  Lighting =(fNDotL)*0.5+1.0;//Add one Only for Brighten
	// Specular
	float3 vLightReflection = -reflect(vLightDir, vNormal.xyz);
	float  fRDotV = fNDotL > 0.0f ? saturate(dot(vLightReflection, ViewDir)) : 0.0f;
	float  Specular = pow(fRDotV, g_SpecPower) * g_SpecValue;
	
	//Calculate Edge Color
	edgeColor =glb_GetEdgeColorCustom(g_GradientDissolveSampler,g_edgeSampler,Input.uvBase.xy,g_edgeContrast,g_MaskSpan,EdgeColor,g_TimerValue);
	
	//Calculate Inner Color
	//offset the texture coords based on the view vector
  float2 height = tex2D(g_InnerSampler,Input.uvBase.xy).ra;
  float2 offset = ViewDir * (height.x * 2.0 - 1.0) * g_offsetBias;
  float2 newTexCoord = Input.uvBase.xy + offset;
	float4 InnerDiffuseTex = tex2D(g_InnerSampler, newTexCoord);
	
	InnerDiffuseTex =InnerDiffuseTex*Lighting + Specular;
	
	float3 vViewReflection = -reflect(ViewDir, vNormal.xyz);
	float4 ReflectTex = tex2D(g_ReflectSampler, (vViewReflection.xy+1.0)*0.5)*g_ReflectRatio;
	
	//Model Alpha Texture
	float alpha = tex2D(g_AlphaMap, Input.uvBase.xy).a;
 	alpha= alpha > 0.329411764 ? edgeColor.a:0.0; //0.329411764 =84/255 AlphaCompare
 	
	//Comp
	float4 IceColor=edgeColor+InnerDiffuseTex+ReflectTex;
	ResultColor.rgb = IceColor.rgb;
	#ifdef _SDR_3_0
		ResultColor.a   = g_UseAlphaMap>0.5?alpha*height.y : alpha;
	#else
		ResultColor.a   = alpha;
	#endif
	//Fog
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);

	return ResultColor;
}