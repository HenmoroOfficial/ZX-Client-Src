#include "../common/common.h"
#include "../SkinModel/mesh_comm.h"
string entrynames = "ps_2_0 ps_main";
float4 g_vEyePos : CameraPosition;
float4 g_MainLightPos : LightPosition0;		//	main light's position
float4 g_colDiffuse : LightColor0 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colSpecular : LightColor1 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colAmbient = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_colPtDiffuse;

//	Primary specular shift
float g_fSpecShift1
<
	string UIName =  "spec shift 1";
	string UIWidget = "slider";
    float UIMin = -1.0f;
    float UIMax = 1.0f;
    float UIStep = -0.01f;
	
> = 0.0f;

//	Secondary specular shift 
float g_fSpecShift2
<
	string UIName =  "spec shift 2";
	string UIWidget = "slider";
    float UIMin = -1.0f;
    float UIMax = 1.0f;
    float UIStep = -0.01f;
	
> = 0.0f;

//	Hair color
float4 g_colHair
<
	string UIName =  "头发颜色";
	string UIWidget = "Color";
	
> = {0.2f, 0.2f, 0.2f, 1.0f};

//	Specular color 1
float4 g_colSpec1
<
	string UIName =  "高光颜色1";
	string UIWidget = "Color";
	
> = {1.0f, 1.0f, 1.0f, 1.0f};

//	Specular color 2
float4 g_colSpec2
<
	string UIName =  "高光颜色2";
	string UIWidget = "Color";

> = {0.5f, 0.5f, 0.5f, 1.0f};

//	Specular power 1
float g_fSpecPower1
<
	string UIName =  "高光范围1";
	string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 200.0f;
    float UIStep = 1.0f;
	
> = 100.0f;

//	Specular power 2
float g_fSpecPower2
<
	string UIName =  "高光范围2";
	string UIWidget = "slider";
    float UIMin = 0.0f;
    float UIMax = 200.0f;
    float UIStep = 1.0f;
	
> = 60.0f;

texture g_DiffuseTexture
<
	string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\Fieldstone.dds";
    string UIName =  "基本贴图";
>;

texture g_SpecularTexture
<
	string UIWidget = "Texture";
    string UIObject = "Shaders\\Textures\\FieldstoneBumpDOT3_DXT5n.dds";
    string UIName = "Specular shfit";
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

sampler2D g_SpecularSampler
<
    string Texture = "g_SpecularTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

//struct VS_OUTPUT 
//{
//	float4 Position   : POSITION;
//	float2 uvBase     : TEXCOORD0;
//	float3 ViewDir	  : TEXCOORD1;
//	float3 LightDir	  : TEXCOORD2;
//	float4 PtLightDir : TEXCOORD3;
//};

float3 _ShfitTangent(float3 T, float3 N, float shift)
{
	float3 vNewT = T + N * shift;
	return normalize(vNewT);
}

float _StrandSpecular(float3 T, float3 V, float3 L, float fExp)
{
	float3 H = normalize(L + V);
	float fDotTH = dot(T, H);
	float fSinTH = sqrt(1.0f - fDotTH * fDotTH);
	float fDirAtten = smoothstep(-1.0f, 0.0f, dot(V, L));
	return fDirAtten * pow(fSinTH, fExp);
//	return pow(fSinTH, fExp);
}

float4 ps_main(PS_INPUT Input) : COLOR0
{
	//	In tangent space, normal is z axis and tangent is x axis
	float3 vNormal = float3(0.0, 0.0, 1.0);
	float3 vTangent = float3(0.0, 1.0, 0.0);

	//	Specular texture channels:
	//	.r: specular scale
	//	.g: specular noise
	//	.b: specular shift
	float4 colBaseTex = tex2D(g_DiffuseSampler, Input.uvBase);
	float4 colSpecTex = tex2D(g_SpecularSampler, Input.uvBase);
	
	float fBaseShift = colSpecTex.b - 0.5f;
	float3 t1 = _ShfitTangent(vTangent, vNormal, fBaseShift + g_fSpecShift1);
	float3 t2 = _ShfitTangent(vTangent, vNormal, fBaseShift + g_fSpecShift2);

	//	Diffuse lighting
	float3 vLightDir = normalize(Input.LightDir);
	float3 vViewDir = normalize(Input.ViewDir);
	float fDotNL = dot(vNormal, vLightDir);
	float3 colDiffuse = saturate(lerp(0.25f, 1.0f, max(fDotNL, 0.0f)));

	float3 vPtLightDir   = normalize(Input.PtLightDir.xyz);
	float  fPtDotNL      = dot(vNormal, vPtLightDir);
	float3 colPtDiffuse  = saturate(lerp(0.25f, 1.0f, max(fPtDotNL, 0.0f))) * g_colPtDiffuse * Input.PtLightDir.w;

	colDiffuse.rgb = (g_colAmbient.rgb + colDiffuse.rgb * g_colDiffuse.rgb + colPtDiffuse) * g_colHair.rgb;

	//	Specular lighting
	float3 colSpecular1 = g_colSpec1.rgb * _StrandSpecular(t1, vViewDir, vLightDir, g_fSpecPower1);
	float3 colSpecular2 = g_colSpec2.rgb * _StrandSpecular(t2, vViewDir, vLightDir, g_fSpecPower2);
	//	Add some noise to specular 2
	float3 colSpecular = (colSpecular1 + colSpecular2 * colSpecTex.g) * colSpecTex.r;

	float3 colOut = (colDiffuse + colSpecular) * colBaseTex.rgb * g_colSpecular.rgb;
	
	return float4(colOut, colBaseTex.a);
}
