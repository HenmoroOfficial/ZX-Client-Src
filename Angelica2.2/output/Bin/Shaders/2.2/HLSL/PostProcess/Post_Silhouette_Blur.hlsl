string entrynames = "ps_2_0 ps_main";



sampler EdgeMap: register(s0);
sampler DepthMap : register(s1);

float zFar;
// float minHardness = 0.1;
// float maxHardness = 0.8;
float hardness = 0.5;
float AA_SampleDist = 0.0002;
float3 lineColor = float3(0,0,0);

const float2 samples[12] =
{
   -0.326212, -0.405805,
   -0.840144, -0.073580,
   -0.695914,  0.457137,
   -0.203345,  0.620716,
    0.962340, -0.194983,
    0.473434, -0.480026,
    0.519456,  0.767022,
    0.185461, -0.893124,
    0.507431,  0.064425,
    0.896420,  0.412458,
   -0.321940, -0.932615,
   -0.791559, -0.597705,
};

float ComputeIntensity(float fDepth, float fMinDis, float fMaxDis)
{
	return lerp(1.f, 0.f, saturate( (fDepth - fMinDis) / (fMaxDis - fMinDis)));
}

float4 ps_main(float2 texCoord: TEXCOORD) : COLOR
{
	float fDepth = tex2D(DepthMap, texCoord).r * zFar;
	
	//float hardness = 0.5;//lerp(maxHardness, minHardness, saturate((fDepth - 0.1f) / 10.f));
	 
	float sampleDist = AA_SampleDist * ComputeIntensity(fDepth, 5.f, 10.f);
	//float hardness = 0.8;
   // Apply a simple blur filter to get rid of aliasing and
   // get a wider spread of the edges and a softer image.
	float sum = tex2D(EdgeMap, texCoord).r;
	for (int i = 0; i < 12; i++)
	{
	   sum += tex2D(EdgeMap, texCoord + sampleDist * samples[i]).r;
	}

	float fResult = hardness * sum;
	

	fResult *= pow(ComputeIntensity(fDepth, 5.f, 40.f), 3);

	//float f = ComputeIntensity(fDepth, 5.f, 30.f);
	//return float4(f, f, f, 1);
 	//fResult =  tex2D(EdgeMap, texCoord).r;
 	//return float4(fResult, fResult, fResult, 1);
	//return float4(0, 0, 0, fResult * 0.5);
	
	return float4(lineColor, fResult);
}


