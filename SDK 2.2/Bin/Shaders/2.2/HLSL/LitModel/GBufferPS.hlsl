string entrynames = "ps_2_0 ps_main";	

string macro0
<
	string name = "_ALPHA_TEST_";
	string desc = "ÊÇ·ñ¿ªÆôAlphaTest";
	string type = "PRG";
	string GroupID = "1";
>;

sampler g_Sampler  	: register(s0);
float4	g_transMask : register(c0);

#ifdef _ALPHA_TEST_
float4 ps_main(in float2 	depthZW  		: TEXCOORD0,
							 in float3 	normal 		: TEXCOORD1,
							 in	float2  texCoord	: TEXCOORD2) : COLOR0
{
	float4 color = tex2D(g_Sampler, texCoord);
	float a = color.a;
	if(dot(float4(1,1,1,1), g_transMask) > 2)
		a = 1.f;
	else
		a = dot(color, g_transMask);
		
	float d = depthZW.x / depthZW.y;    // / g_zFar;
	return float4(d,d,d, a);
}

#else
float4 ps_main(	in float2 	depthZW  	: TEXCOORD0,
				in float3 	normal 		: TEXCOORD1,
				in	float2  oTexCoord	: TEXCOORD2) : COLOR0
{
	float d = depthZW.x / depthZW.y;    // / g_zFar;
	return float4(d, d, d, d);
}

#endif
