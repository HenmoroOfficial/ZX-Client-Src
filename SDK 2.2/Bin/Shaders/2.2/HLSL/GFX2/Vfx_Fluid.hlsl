/*-----------------------------------------------------------------
	流动
	Instruction: 15
	RTX :wei wang (Engine Center)
	Perfect World
-----------------------------------------------------------------*/

string entrynames = "ps_2_0 ps_main";

struct psIutput
{
	float4		Diffuse 	: COLOR0;
	float4		Tex0			: TEXCOORD0;
};

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
// for ART
texture texBase
<
	string UIName = "颜色图";
	string UIObject = "";
>;

sampler sBase
<
	string Texture = "texBase";
  string MipFilter = "LINEAR";
  string MinFilter = "LINEAR";
  string MagFilter = "LINEAR";
  string AddressU = "WRAP";
  string AddressV = "WRAP";
>;

texture texDisturb
<
	string UIName = "溶解贴图";
	string UIObject = "";
>;

sampler sDisturb
<
	string Texture = "texDisturb";
  string MipFilter = "LINEAR";
  string MinFilter = "LINEAR";
  string MagFilter = "LINEAR";
  string AddressU = "WRAP";
  string AddressV = "WRAP";
>;
//RG通道有效，R:Mask G:Alpha
texture texMask
<
	string UIName = "Mask贴图";
	string UIObject = "";
>;

sampler sMask
<
	string Texture = "texMask";
  string MipFilter = "LINEAR";
  string MinFilter = "LINEAR";
  string MagFilter = "LINEAR";
  string AddressU = "WRAP";
  string AddressV = "WRAP";
>;
//-----------------------------------------------------------------
// Parameter
//-----------------------------------------------------------------
float g_Blendfactor : register(c0)
<
   string UIName = "混合";
   string UIWidget = "Slider";
   float UIMin = 0.0;
   float UIMax = 1.0;
   float UIStep = 0.01;
> = 1.0f;

float g_Wave : register(c1)
<
   string UIName = "流动滑块";
   string UIWidget = "Slider";
   float UIMin = 0.0;
   float UIMax = 1.0;
   float UIStep = 0.01;
> = 0.0f;

float g_Alpha : register(c3)
<
   string UIName = "整体透明度";
   string UIWidget = "Slider";
   float UIMin = 0.0;
   float UIMax = 1.0;
   float UIStep = 0.01;
> = 1.0f;

float g_MaskInv : register(c4)
<
   string UIName = "反转Mask";
   string UIWidget = "Slider";
   string UIDesc = "翻转Mask贴图的R通道。说明：Mask贴图目前利用了R和G两个通道，G通道为模型本身Alpha，请将模型Alpha复制到此，如果原模型没有Alpha，请使用白色。R通道为Mask用途。";
   float UIMin = 0.0;
   float UIMax = 1.0;
   float UIStep = 1.0;
> = 0.0f;


//-----------------------------------------------------------------
// PS
//-----------------------------------------------------------------

float4 ps_main(psIutput input) : COLOR0
{
	float4 OrgDiffuseTex = tex2D(g_AlphaMap,input.Tex0.xy);
	
	float4 BaseOffset=tex2D(sDisturb,input.Tex0.xy);
	float2 MaskAlpha = tex2D(sMask,input.Tex0.xy).rg; 
	
	MaskAlpha.x =g_MaskInv>0.5f? 1.0f-MaskAlpha.x:MaskAlpha.x;
	
	float2 FinalOffset = g_Wave+BaseOffset.xy;
	float4 BaseColor= tex2D(sBase,FinalOffset);
	float4 BaseColor2=tex2D(sDisturb,input.Tex0.xy);
	
	float4 FinalColor =1.0f;
	FinalColor = lerp(BaseColor2,BaseColor,g_Blendfactor);
	FinalColor.a =FinalColor.a*MaskAlpha.x*MaskAlpha.y*OrgDiffuseTex.a*g_Alpha;
	return FinalColor;
}
