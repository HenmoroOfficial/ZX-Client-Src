string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main;ps_2_0 ps_main_occ";

#define OCC_SAMPLES 6

float4x4 	g_matWVP;
float3      g_vSampleFlags; // { sample texture, sample occ}
float2      g_vSamplePos[OCC_SAMPLES];
float4      g_vBrightness;

sampler2D g_TextureSampler;

sampler2D g_OccSampler;


struct VS_INPUT
{
	float4 position     : POSITION0;
	float4 diffuse		: COLOR0;
	float2 tex0         : TEXCOORD0;
};

struct VS_OUTPUT
{
	float4 position     : POSITION0;
	float4 diffuse		: COLOR0;
	float2 tex0         : TEXCOORD0;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.position = mul(Input.position, g_matWVP);
	//Output.position.z = Output.position.w;	//	Push to infinite far
	
	Output.diffuse = Input.diffuse * g_vBrightness;
    Output.tex0 = Input.tex0;
	return Output;
}

float4 ps_main(VS_OUTPUT Input): COLOR
{
    //return float4(Input.tex0, 0, 1);
    float4 color = Input.diffuse;
    if (g_vSampleFlags.x)
    {
	    color *= tex2D(g_TextureSampler, Input.tex0);
	}
	if (g_vSampleFlags.y)
	{
	    color.rgb *= tex2D(g_OccSampler, float2(0.5f, 0.5f)).r;
	}
    return color;
}

float4 ps_main_occ(VS_OUTPUT Input): COLOR
{
    float fAlpha = 0;
    for (int i = 0; i < OCC_SAMPLES; i++)
    {
        fAlpha += tex2D(g_OccSampler, g_vSamplePos[i]).a;
    }
    fAlpha *= 0.3f;
    return float4(fAlpha, fAlpha, fAlpha, g_vBrightness.a);
}
