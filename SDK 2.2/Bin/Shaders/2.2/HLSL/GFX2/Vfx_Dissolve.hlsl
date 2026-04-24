/*-----------------------------------------------------------------
	溶解/软边缘/边缘颜色
	粒子或面片材质 All in One
	Instruction: 21
	RTX :wei wang (Engine Center)
	Perfect World
-----------------------------------------------------------------*/

string entrynames = "ps_2_0 ps_main";

struct psIutput
{
	float4		Diffuse 	: COLOR0;
	float4		Tex0        : TEXCOORD0;
	float4      fogParam    : TEXCOORD6;	
};

texture texBase
<
	string UIName = "底图";
	string UIObject = "";
>;

sampler sBase
<
	string Texture = "texBase";
>;

texture texDissolve
<
	string UIName = "溶解贴图";
	string UIObject = "";
>;

sampler sDissolve
<
	string Texture = "texDissolve";
	string MinFilter = "LINEAR";
	string MagFilter = "LINEAR";
	string MipFilter = "LINEAR";
	string AddressU = "WRAP";
	string AddressV = "WRAP";
>;

float gThreshold
<
   string UIName = "溶解阈值";
   string UIDesc = "溶解贴图r值小于此值的点显示为透明";
   string UIWidget = "Slider";
   float UIMin = 0.0;
   float UIMax = 1.0;
   float UIStep = 0.01;
> = 0.0f;

float gEdgeRange
<
   string UIName = "软边缘范围";
   string UIDesc = "边缘的软硬程度,0表示没有软边缘，没有边缘颜色影响";
   string UIWidget = "Slider";
   float  UIMin = 0.0;
   float  UIMax = 1.0;
   float  UIStep = 0.01;
> = 0.0f;


float4 gEdgeColor
<
   string UIName = "边缘颜色";
   string UIDesc = "软边缘边缘颜色";
   string UIWidget = "Color";
> ={1.0f,1.0f,1.0f,1.0f};

float gEdgePower
<
   string UIName = "边缘颜色对比";
   string UIDesc = "边缘的颜色对比范围调节";
   string UIWidget = "Slider";
   float  UIMin = 0.0;
   float  UIMax = 15.0;
   float  UIStep = 0.01;
> = 1.0f;

float4 ps_main(psIutput input) : COLOR0
{
	float4 TexColor = tex2D(sBase, input.Tex0.xy);
	float4 DissolveColor = tex2D(sDissolve, input.Tex0.xy);
	
	//soft edge
	float delta = gThreshold - DissolveColor.r;
	float temp =saturate(-delta / gEdgeRange) * TexColor.a;
	//Gray value form diffuse for Cut off Edge color
	float GrayValue =dot(TexColor.rgb, float3(0.299, 0.587, 0.114));
	GrayValue =GrayValue>0.01f? 1.0:0.0;
	//comp
	TexColor.rgb = gEdgeRange<0.01f? TexColor.rgb : TexColor.rgb+gEdgeColor.rgb*(1-pow(temp,gEdgePower))*GrayValue;
	TexColor.a  = delta > 0.0f? 0.0f:temp;
	TexColor.rgb*=temp;//Fit for Every Blend Mode
	TexColor *= input.Diffuse;

    TexColor.rgb = lerp(input.fogParam.rgb, TexColor.rgb, input.fogParam.a);

	return TexColor;
}
