string entrynames = "ps_2_0 ps_main";

float2 g_vPixelSize;
sampler backBufferTextureSampler : register(s0);
	
#define blurWidth 0.75 //0.75
								
float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0 
{
	const float GW[5] = {	5.f  / 16.f, 
 						20.f / 16.f, 
 						30.f / 16.f, 
 						20.f / 16.f, 
 						5.f  / 16.f };
						
	float2 du1 = float2(0, g_vPixelSize.y) * blurWidth;
	float2 du2 = float2(0, g_vPixelSize.y * 2) * blurWidth;

	float4 s[5];
	s[0] = tex2D(backBufferTextureSampler, texcoord - du2);
	s[1] = tex2D(backBufferTextureSampler, texcoord - du1);
	s[2] = tex2D(backBufferTextureSampler, texcoord);
	s[3] = tex2D(backBufferTextureSampler, texcoord + du1);
	s[4] = tex2D(backBufferTextureSampler, texcoord + du2);

	//return s[2];

	float4 sw[5];
	
	float totalCoC = 0;
	for(int i = 0; i < 5; i++)
		totalCoC += s[i].a;
	
	if(totalCoC <= 0)
		return s[2];
		
	for(int i = 0; i < 5; i++)
		sw[i] = s[i].a / totalCoC;
		
	float3 blurredRGB = float3(0,0,0);
	float blurredCoC = 0;
	for(int i = 0; i < 5; i++)
	{
		blurredRGB += s[i].rgb * sw[i]; //* GW[i]
		blurredCoC += s[i].a * 0.2; //* GW[i] 
	}
		
		
	//float3 blurredRGB = ( s1.rgb * GW[0] * s1.a + s2.rgb * GW[1] * s2.a
	//					+ s3.rgb * GW[2] * s3.a + s4.rgb * GW[3] * s4.a + s5.rgb * GW[4] * s5.a)
	//					/ (s1.a + s2.a + s3.a + s4.a + s5.a);
	//float blurredCoC = (s1.a * GW[0] + s2.a * GW[1] + s3.a * GW[2] + s4.a * GW[3] + s5.a * GW[4]) / 5.f;
	
	return saturate(float4(blurredRGB, blurredCoC));
}