#include "../common/common.h"
#include "../SkinModel/mesh_comm.h"

string macro01
<
	string name = "ENABLE_NORMALMAP";
	string type = "ART";
	string desc = "法线贴图";
	string groupid = "1";
>;
string macro02
<
	string name = "ENABLE_CUBEMAP";
	string type = "ART";
	string desc = "CUBE测试";
	string groupid = "1,3";
>;
string macro03
<
	string name = "ENABLE_POINTLIGHT";
	string type = "ART";
	string desc = "点光源支持";
	string groupid = "3";
>;
string entrynames = "ps_2_0 ps_main";
//struct PS_INPUT
//{
//	float2 Texcoord         : TEXCOORD0;
//	float3 ViewDirection    : TEXCOORD1;
//    float3 LightDirection   : TEXCOORD2;
//	float4 PtLightDirection : TEXCOORD3;
//};

texture g_DiffuseTexture
<
	string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Fieldstone.dds";
    string UIName =  "基本贴图";
>;

texture g_NormalTexture 
<
	string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "法线贴图";
>;

texture g_CubeTexture 
<
	string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName =  "环境贴图";
>;

sampler2D g_sampBaseTex : DiffuseMap
<
	string Texture = "g_DiffuseTexture";
    string MipFilter = "LINEAR";
	string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

sampler2D g_sampNormalTex : NormalMap
<
	string Texture = "g_NormalTexture";
    string MipFilter = "LINEAR";
	string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

samplerCUBE g_sampCubeTex : CubeMap
<
	string Texture = "g_CubeTexture";
    string MipFilter = "LINEAR";
	string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

float4 g_colAmbient0:Ambient0 = float4(0.36,0.36,0.36,1);
float4 g_colAmbient1:Ambient1;
float g_DiffusePower :DirPower;
//float4 g_colSpecular = float4(0.48,0.48,0.48,1);
float4 g_colDiffuse :Diffuse = float4(0.88,0.88,0.88,1);
float4 g_colPtDiffuse:PointDiffuse = float4(0.88,0.88,0.88,1);
float4 g_colDiffuseFactor : DiffuseFactor = float4(1,1,1,1);
float4 g_colEmissiveFactor : EmissiveFactor = float4(0,0,0,0);

float fSpecularPower<
	string UIName =  "高光强度";
	string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 100.0f;
    float UIStep = 0.01f;
> = 25;

int nTest
<
	string UIName = "测试整数";
	string UIWidget = "int";
> = 128;

int4 vTest
<
	string UIName = "测试整数向量";
	string UIWidget = "int";
> = int4(128,0,0,0);

bool bTest
<
	string UIName = "测试bool";
	string UIWidget = "bool";
> = true;

bool4 bvTest
<
	string UIName = "测试bool向量";
	string UIWidget = "bool";
> = bool4(true, false,true,true);

//float4 colAnyone<
//	string UIName =  "随机颜色";
//	string UIWidget = "Color";
//
//> = float4(0,0,1,1);

float4 ps_main(PS_INPUT Input) : COLOR0
{
	float3 fvLightDirection = normalize( Input.LightDir );
	//float3 fvNormal       = normalize( ( tex2D( g_sampNormalTex, Input.Texcoord ).xyz * 2.0f ) - 1.0f );
#ifdef ENABLE_NORMALMAP
	float3 fvNormal         = glb_SamplerNormalMap(g_sampNormalTex, Input.uvBase);
#else
	float3 fvNormal         = float3(0,0,1);
#endif // ENABLE_NORMALMAP
	float  fNDotL           = dot( fvNormal, fvLightDirection ); 
   
	float3 fvReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fNDotL ) ) - fvLightDirection ); 
	float3 fvViewDirection  = normalize( Input.ViewDir );
	float  fRDotV           = fNDotL > 0 ? saturate( dot( fvReflection, fvViewDirection ) ) : 0;
	fNDotL = saturate(fNDotL);

#ifdef ENABLE_POINTLIGHT
	float3 fvPtLightDirection = normalize( Input.PtLightDir );
	float  fPtNDotL           = dot( fvNormal, fvPtLightDirection ); 
	float3 fvPtReflection     = normalize( ( ( 2.0f * fvNormal ) * ( fPtNDotL ) ) - fvPtLightDirection ); 
	float  fPtRDotV           = fPtNDotL > 0 ? saturate( dot( fvPtReflection, fvViewDirection ) ) : 0;
	fPtNDotL = saturate(fPtNDotL);
#endif // ENABLE_POINTLIGHT
   
	float4 fvBaseColor      = tex2D( g_sampBaseTex, Input.uvBase );

#ifdef ENABLE_POINTLIGHT
	float4 vPtDiffuse       = g_colPtDiffuse * fPtNDotL * fvBaseColor * Input.PtLightDir.w;
	float4 vPtSpecular      = g_colDiffuse * pow( fPtRDotV, fSpecularPower ) * Input.PtLightDir.w;

	float4 fvTotalAmbient   = g_colAmbient0 * fvBaseColor;
	float4 fvTotalDiffuse   = saturate( g_colDiffuse * fNDotL * fvBaseColor + vPtDiffuse );
	float4 fvTotalSpecular  = g_colDiffuse * pow( fRDotV, fSpecularPower ) + vPtSpecular;
#else
	float4 fvTotalAmbient   = g_colAmbient0 * fvBaseColor;
	float4 fvTotalDiffuse   = saturate( g_colDiffuse * fNDotL * fvBaseColor);
	float4 fvTotalSpecular  = g_colDiffuse * pow( fRDotV, fSpecularPower );
#endif // ENABLE_POINTLIGHT
   
	float4 c = saturate( fvTotalAmbient + fvTotalDiffuse + fvTotalSpecular);
#ifdef ENABLE_CUBEMAP
	c.xyz = texCUBE(g_sampCubeTex, fvViewDirection);
#endif // ENABLE_CUBEMAP
	//c.xyz *= colAnyone.xyz;
	//if(bvTest.y)
	//	c.xyz = float3(255,0,0);
	//else
	//	c.xyz = float3(0,255,0);
	//c.xyz = vTest / 255.0f;
	c.w = 1;
	return c * g_colDiffuseFactor + g_colEmissiveFactor;
}