string entrynames = "ps_2_0 ps_main";

string macro1
<
	string name = "_ALPHA_TEST_";
	string desc = "AlphaTest支持";
	string type = "PRG";
	string GroupID = "1";
>;

string macro2
<
	string name = "_SILHOUETTE_DEPTH_";
	string desc = "输出渲染勾边需要的深度";
	string type = "PRG";
	string GroupID = "2";
>;

sampler g_Sampler  	: register(s0);
float4	g_transMask : register(c0);
float   g_zFar		: register(c1);

#ifdef _ALPHA_TEST_
float4 ps_main(in float2 	depthZW  		: TEXCOORD2,
							 in float3 	normal 		: TEXCOORD3,
							 in	float2  texCoord	: TEXCOORD0) : COLOR0
{
	float4 color = tex2D(g_Sampler, texCoord);
	float a = color.a;
	if(dot(float4(1,1,1,1), g_transMask) > 2)
		a = 1.f;
	else
		a = dot(color, g_transMask);
		
	float d = depthZW.x / depthZW.y;   // / g_zFar;

#ifdef _SILHOUETTE_DEPTH_
	d = depthZW.y / g_zFar;
#endif

	return float4(d, d, d, a);
}

#else
float4 ps_main(in float2 	depthZW  		: TEXCOORD2,
							 in float3 	normal 		: TEXCOORD3,
							 in	float2  oTexCoord	: TEXCOORD0) : COLOR0
{
	float d = depthZW.x / depthZW.y;   // / g_zFar;

#ifdef _SILHOUETTE_DEPTH_
	d = depthZW.y / g_zFar;
#endif

	return float4(d, d, d, 1);
}

#endif
