string entrynames = "ps_2_0 ps_main";

#ifndef LAYERCOUNT
#define LAYERCOUNT 1
#endif // #ifndef LAYERCOUNT

sampler TexSampler[LAYERCOUNT] : register(s0);
	
struct VS_OUTPUT
{
	float4 position : POSITION;   // vertex position
	float4 color    : COLOR0;
	float2 texcoord : TEXCOORD0;  // vertex texture coords
};

float4 ps(VS_OUTPUT Input) : COLOR0
{
	float4 color = tex2D(TexSampler[0], Input.texcoord);
#if(LAYERCOUNT > 1)
	for(int i = 1; i < LAYERCOUNT; i++)
	{
		float4 c = tex2D(TexSampler[i], Input.texcoord);
		if(abs(c.w - 0.5) < 0.01 && abs(c.y - 0.5) < 0.01)
			c.wy = 0.5;
		color.wy = color.wy + c.wy - 0.5;
	}
	//color.wy = normalize(color.wy);
#endif // #if LAYERCOUNT > 1
	return color;
}
