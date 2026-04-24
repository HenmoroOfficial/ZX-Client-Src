string entrynames = "ps_2_0 ps_main";

string macro0
<
	string name = "_MASK_";
	string desc = "ÊÇ·ñÓÐmask";
	string type = "PRG";
	string GroupID = "1";
>;

sampler g_InputSampler : register(s0);
sampler pertubeSampler : register(s1);
sampler maskSampler	   : register(s2);	
float g_fWarpScale;

struct WarpVSOut
{
	float4 position  : POSITION;   // vertex position 
	float2 texcoord  : TEXCOORD0;  // vertex texture coords 
	float2 pertubeTC : TEXCOORD1;
};

float4 ps_main(WarpVSOut input) : COLOR0
{
	float2 pertube = tex2D(pertubeSampler, input.pertubeTC).xy;
	
	pertube = (pertube - float2(0.5, 0.5)) * 2;

#ifdef _MASK_
	pertube *= tex2D(maskSampler, input.texcoord).r;
#endif

	float2 tc = float2(-1, 1) * float2(g_fWarpScale, g_fWarpScale) * pertube.xy + input.texcoord;

	float4 color = tex2D(g_InputSampler,tc);
	return color;
}
