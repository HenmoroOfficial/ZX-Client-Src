string entrynames = "ps_2_0 ps_main";

struct psIutput
{
	float4		Diffuse 	: COLOR0;
	float4		Tex0		: TEXCOORD0;
	float4		Tex1		: TEXCOORD7;
};

texture texBase
<
	string UIName = "基本贴图";
	string UIObject = "";
>;

sampler sBase
<
	string Texture = "texBase";
>;

texture texHilight
<
	string UIName = "高光贴图";
	string UIObject = "Gfx\\textures\\高光.tga";
>;

sampler sHilight
<
	string Texture = "texHilight";
	string MinFilter = "LINEAR";
	string MagFilter = "LINEAR";
	string MipFilter = "LINEAR";
	string AddressU = "WRAP";
	string AddressV = "WRAP";
>;

float4 ps_main(psIutput input) : COLOR0
{
	float4 col;
	float4 tex0 = tex2D(sBase, input.Tex0.xy);
	float4 tex1 = tex2D(sHilight, input.Tex1.xy);
	
	col = tex0 + tex1 - 0.5f;
	
	col.rgb = col.rgb * input.Diffuse.rgb;
	col.a = tex0.a * input.Diffuse.a;
		
	return col;
}