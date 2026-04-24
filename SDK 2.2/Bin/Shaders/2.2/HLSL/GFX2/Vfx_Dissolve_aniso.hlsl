/*-----------------------------------------------------------------
	Stone VFX Shader(2.2)
	Instruction: 52
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

float4 g_colDiffuse    : Diffuse = {1.0f, 1.0f,1.0f,1.0f};
float  g_fDiffusePower : DirPower;
float4 g_colAmbient    : Ambient0 = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_Ambient2      : Ambient1;
//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------


float gEdgeRange
<
	string UIName =  "消隐软边缘跨度";
	string UIWidget = "slider";
	float UIMin  = 0.0;
	float UIMax  = 1.0;
	float UIStep = 0.01;
> = 0.0;

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

float4 g_colAniso
<
	string UIName =  "各项异性高光颜色";
	string UIWidget = "Color";
> = {1.0f, 1.0f, 1.0f, 1.0f};

float4 g_colDarkness
<
	string UIName =  "各项异性暗部颜色";
	string UIWidget = "Color";
> = {0.0f, 0.0f, 0.0f, 1.0f};

float g_AnisoMultiple
<
	string UIName =  "各项异性高光增强倍数";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 8.0;
	float UIStep = 0.01;
> = 2.0f;

float g_reflectContrast
<
	string UIName =  "反射对比度";
	string UIWidget = "slider";
	float UIMin = 1.0;
	float UIMax = 4.0;
	float UIStep = 0.01;
> = 1.0;

float g_DarkStrength
<
	string UIName =  "各项异性高光暗部倍数";
	string UIWidget = "slider";
	float UIMin = 0.0;
	float UIMax = 4.0;
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
    string UIName =  "溶解贴图";
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
    string UIObject = "Shaders\\Textures\\AnisoReflect\\armorreflect.dds";
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

sampler g_ReflectSampler
<
    string Texture = "g_ReflectTexture";
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "CLAMP";
    string AddressV = "CLAMP";
>;


//-----------------------------------------------------------------
// PS
//-----------------------------------------------------------------
float4 ps_main(psInput Input) : COLOR0
{
	float4 edgeColor= 0.0f;
	float4 ResultColor =1.0f;
	
	//Tangent space L N V
	float3 vLightDir = normalize(Input.LightDir.xyz);
	float3 ViewDir   = normalize(Input.ViewDir.xyz);
	float3 vNormal;
	vNormal.rg = tex2D(g_NormalMap, Input.uvBase.xy).ag*2-1;
	vNormal.b  = sqrt(1 - dot(vNormal.rg, vNormal.rg));
	vNormal = normalize(vNormal);
	
	float3x3 matT2C;
	matT2C[0] = Input.tangent.xyz;
	matT2C[1] = Input.binormal.xyz;
	matT2C[2] = cross(Input.tangent.xyz, Input.binormal.xyz) * Input.tangent.w;
	float3 camReflect = 0;
	float4 colReflectTex = 0;
	camReflect = mul(vNormal, matT2C);
	camReflect.x = (camReflect.x + 1) * 0.5;
	camReflect.y = (1 - camReflect.y) * 0.5;
	colReflectTex = tex2D(g_ReflectSampler, camReflect.xy);
	
	//Using Diffuse Light
	float fNDotL = dot(vNormal.xyz, vLightDir.xyz);
	float fA1 = saturate(-fNDotL);
	float fA2 = 1 - abs(fNDotL);
	fNDotL = fNDotL*0.5+0.5;
	float3 colDiffuse = (g_fDiffusePower * g_colDiffuse.rgb * fNDotL);
	//Specular(None)
	
	//Ambient
	float3 colAmbient = (g_colAmbient.rgb * fA1 + g_Ambient2.rgb * fA2);

	//Calculate Edge Alpha
	edgeColor = tex2D(g_GradientDissolveSampler, Input.uvBase.xy);
	float delta = g_TimerValue - edgeColor.r;
	float temp = saturate(-delta / gEdgeRange);
	edgeColor.a = delta >= 0 ? 0:temp;

	//Calculate Inner Color
  float4 InnerDiffuseTex = tex2D(g_AlphaMap, Input.uvBase.xy);
	float f = pow(colReflectTex.r, g_reflectContrast);
	float3 colAniso = g_AnisoMultiple * g_colAniso.rgb;

	colAniso.rgb = colAniso.rgb * f * InnerDiffuseTex.r;//we use diffuse maps r channel as Mask Texture
	colAniso.rgb = colAniso.rgb *  colDiffuse.rgb; //make the anisotropic color to have a environment color tone.
	InnerDiffuseTex.rgb = colAniso.rgb+colDiffuse.rgb*InnerDiffuseTex.rgb* lerp(1.0, lerp(g_colDarkness.rgb, 1.0, f), saturate(InnerDiffuseTex.r * g_DarkStrength));
  
	//Comp
	//ResultColor.rgb = InnerDiffuseTex.xyz + colAmbient.xyz; //if consider Ambient, use this
	ResultColor.rgb = InnerDiffuseTex.xyz;
	
	float alpha = InnerDiffuseTex.a;
 	alpha= alpha > 0.329411764 ? edgeColor.a:0.0; //0.329411764 =84/255 AlphaCompare
	ResultColor.a=alpha;
	//Fog
	ResultColor.rgb = lerp(Input.fogParam.rgb, ResultColor.rgb, Input.fogParam.a);
	
	return ResultColor;
}