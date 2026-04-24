float4x4 matViewProjection : register(c0);

struct VS_INPUT 
{
	float4 Position : POSITION0;
    float4 Normal   : NORMAL0;
    float4 Diffuse  : COLOR0;
	float2 Texcoord : TEXCOORD0;
	float2 LMcoord  : TEXCOORD1;
};

struct VS_OUTPUT 
{
	float4 Position : POSITION0;
    float4 Diffuse  : COLOR0;
	float2 Texcoord : TEXCOORD0;
	float2 LMcoord  : TEXCOORD1;
    float4 Normal   : TEXCOORD2;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;
	Output.Position = mul(matViewProjection, Input.Position);
	Output.Diffuse  = Input.Diffuse;
	Output.Texcoord = Input.Texcoord;
	Output.LMcoord  = Input.LMcoord;
	Output.Normal  = Input.Normal;
	return Output;
}

sampler2D BaseMap   : register(s0);
sampler2D LightMap : register(s1);
sampler2D LightTable : register(s2);
float4 g_fFlags   : register(c1);
float4 g_fVertFlags   : register(c2);

float4 ps_main(VS_OUTPUT Input): COLOR0
{
    if (g_fFlags.w)
    {
    	return  float4((Input.Normal.xyz + 1) / 2, 1);
    }

    float4 crDiffuse = tex2D(BaseMap, Input.Texcoord);
	clip(crDiffuse.w - 0.5f);
	float4 c = 1;
    float3 sunPower = 1.0 / g_fFlags.z / 2;
    if (g_fFlags.x)
    {
        //crDiffuse = pow(crDiffuse, 1 / g_fFlags.z);
        c *= crDiffuse;
    }
    if (g_fFlags.y && !g_fVertFlags.x)
    {
	    float4 crLit = tex2D(LightMap, Input.LMcoord);
        crLit.xyz = crLit.xyz / max(0.00001, 1 - crLit.xyz);
        crLit.xyz += sunPower * (crLit.w);// / max(0.00001, 1 - crLit.w));
        crLit.w = 1;
        c *= crLit;
        //c = c / (1 + c);
    }
    if (g_fFlags.y && g_fVertFlags.x)
    {
        c.xyz *= Input.Diffuse.xyz * 2;
    }
    //c = pow(saturate(c), g_fFlags.z);
    return c;
}