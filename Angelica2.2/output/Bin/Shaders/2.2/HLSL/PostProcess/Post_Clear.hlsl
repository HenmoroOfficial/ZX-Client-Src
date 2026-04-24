string entrynames = "ps_2_0 ps_main";
float4 g_clearColor;

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	return g_clearColor;
}
