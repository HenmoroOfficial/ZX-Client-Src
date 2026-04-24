string entrynames = "ps_3_0 ps_main";

sampler g_RandomRotTextureSampler	: register(s0);
sampler g_GBufferSampler			: register(s1);
sampler g_SmallGBufferSampler		: register(s2);

float4 g_param						: register(c0);
//float g_zFar;
//float g_R;
//float2 g_vPixelSize
float4 g_param2						: register(c1);
//float2 g_FocalLen
//float2 g_InvFocalLen;
float4 AO_RAND[32]					: register(c10);

#define radiusDepthScale 			0.02f
struct VSOut
{
    float4 pos  : POSITION;   // vertex position 
    float2 uv  	: TEXCOORD0;  // vertex texture coords 
    float2 uv2  : TEXCOORD1;
};

float3 UV2Eye(float2 uv, float eyeZ, float2 g_InvFocalLen)
{
    uv = (uv * float2(2.0, -2.0) - float2(1.0, -1.0));
    return float3(uv * g_InvFocalLen * eyeZ, eyeZ);
}

float2 Eye2UV(float3 pos, float2 g_FocalLen)
{
    float2 uv = pos.xy / pos.z * g_FocalLen;
    uv = (uv + float2(1.0, -1.0) ) * float2(0.5, -0.5);
    return uv;
}

float3 mirror( float3 vDir, float3 vPlane ) 
{
  return vDir - 2 * vPlane * dot(vPlane,vDir);
}

//星际争霸2的算法还是有问题. 当采用了法线贴图的时候,半球上的样本分布很难正确. z是平面, 而法线却变化很大.
//最好还是用只采深度的算法.
float4 ps_main(VSOut In) : COLOR0
{
	float g_ZFar			= g_param.x;
	float g_R				= g_param.y * 2;
	float2 g_vPixelSize		= g_param.zw;
	
	float2 g_FocalLen		= g_param2.xy;
	float2 g_InvFocalLen	= g_param2.zw;
	
	
    const int sampleNum = 32;
    float2 uv, uv2;
    uv = In.uv;
    uv2 = In.uv2;

    float4 gb =  tex2D( g_GBufferSampler, In.uv);
    float fSceneDepth = gb.a * g_ZFar;
	
	//是否随着距离变远而扩大采样范围
	float R = min(g_R, g_R * fSceneDepth * radiusDepthScale); //g_R;  //
	
	float radiusInView = R;

	//float3x3 nw2v = (float3x3)g_normalWorldToView;
	//float3 eyeNormal = normalize(mul(gb.rgb, nw2v));				//恢复到eye空间

    float3 centerPos = UV2Eye(uv, fSceneDepth, g_InvFocalLen);		//得到eye空间的位置
	float3 eyeNormal = gb.rgb;
	
    float3 rotSample = tex2D(g_RandomRotTextureSampler, uv2).rgb;

	float OUT = 0;

	float sn = 0.f;

	float bias = 0.002 * fSceneDepth;
    //当前像素的eye空间坐标
	for(int i = 0; i < sampleNum; i++)
	{
		float weight = 1;	//(1.f - AO_RAND[i].w);

		float3 offsetDir = mirror(AO_RAND[i].xyz, rotSample);
		float3 offsetDirN = normalize(offsetDir);

		float3 offset = offsetDir * radiusInView;
		
		float dotNormal = dot(offsetDirN, eyeNormal);
		if(dotNormal < 0 )
		{
			offset = -offset;
			dotNormal = -dotNormal;
		}		
		if(dotNormal < 0.2)
 			continue;
        
        float3 sampleP = centerPos + offset;
        float2 samplePUV = Eye2UV(sampleP, g_FocalLen);

        float2 uvs = samplePUV.xy;
		//对缩小一半的深度图采样
		float sampleDepthInTexture = tex2D(g_SmallGBufferSampler, uvs).r * g_ZFar;

		float fDistance = sampleP.z - sampleDepthInTexture;
		if(fDistance <= bias || fDistance > R)
			OUT += 1.f * weight;
		else
		{
			//float ratio = (fDistance - bias) / g_R;
			//OUT += ratio;
			OUT += 0.f;
		}
		sn += weight;
    }
	if(sn == 0)
		OUT = 1;
	else
		OUT /= sn;
	
	return float4(OUT,OUT,OUT,1);
}