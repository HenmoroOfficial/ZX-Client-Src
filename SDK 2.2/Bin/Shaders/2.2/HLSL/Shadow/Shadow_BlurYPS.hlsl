string entrynames = "ps_2_0 ps_main";

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);
sampler g_sampler	: register(s0);

#define SAMPLE_NUM 1

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	//const float weight_zh[9] = { 1,1,1,1,1,1,1,1,1};
	//float weight_zh[9] = {0.273, 0.256, 0.21, 0.151, 0.094, 0.05, 0.025, 0.01, 0.004}; //thita = 1.46
	//float weight_zh[5] = {0.273, 0.21, 0.094, 0.025, 0.004};
	//float weight_zh[5] = {5,4,3,2,1};
	float4 cTotal = float4(0,0,0,0);
	float2 vStep = float2(0, g_vPixelSize.y);
	
	cTotal += tex2D(g_sampler, texcoord);
	
	for(int i = 1; i <= SAMPLE_NUM; i += 1)
	{
		cTotal += tex2D(g_sampler, texcoord + vStep * i);
		cTotal += tex2D(g_sampler, texcoord - vStep * i);
	}
	return cTotal / (SAMPLE_NUM * 2 + 1);
}