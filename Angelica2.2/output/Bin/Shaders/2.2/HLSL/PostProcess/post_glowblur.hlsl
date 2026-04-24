string entrynames = "ps_2_0 ps_main";

string macro0
<
	string name = "_BLUR_X_";
	string desc = "_BLUR_X_";
	string type = "PRG";
	string GroupID = "1";
>;
string macro1
<
	string name = "_BLUR_Y_";
	string desc = "_BLUR_Y_";
	string type = "PRG";
	string GroupID = "1";
>;
//float weight_zh[9] = {0.273, 0.256, 0.21, 0.151, 0.094, 0.05, 0.025, 0.01, 0.004}; //thita = 1.46
//float weight_zh[5] = {0.273, 0.21, 0.094, 0.025, 0.004};
//float weight_zh[5] = {5,4,3,2,1};


//float weight_zh[9] = {8,8,7,6,5,4,3,2,1};
//float weight_yh[7] = {462,462,330,165,55,11,11};

float g_blurWidth;					//1是整个贴图大小
float2 g_inputPixelSize;
sampler g_Sampler  	: register(s0);

#if !defined(_BLUR_X_) && !defined(_BLUR_Y_)
// 不用宏的时候使用这个空的ps_main
float4 ps_main() : COLOR0
{
	return 0;
}
#endif // #if !defined(_BLUR_X_) && !defined(_BLUR_Y_)


#ifdef _BLUR_X_
float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	const float weight_zh[9] = { 1,1,1,1,1,1,1,1,1};
	float wt = 0;
	float4 ct = float4(0,0,0,0);
	float Step = g_blurWidth / 14;
	Step = min(Step, 2 * g_inputPixelSize.x);
	
	float suv;
	float w;
	
	float2 luv = float2(texcoord.x, texcoord.y);
	w = weight_zh[0];
	ct += tex2D(g_Sampler, luv) * w;
	wt += w;
	
	//return tex2D(g_Sampler, luv);
	
	float2 ruv = luv;
	//float2 ruv = float2(texcoord.x + Step * 0.5, texcoord.y);
	//w = weight_zh[0];
	//ct += tex2D(g_Sampler, ruv) * w;
	//wt += w;

	for(int i = 1; i <= 8; i += 1)  //7月28日想修改, 降低采样数, 增加pass数目
	{
		w = weight_zh[i];
		luv.x -= Step;
		float2 uv = float2(luv.x, texcoord.y);
		float4 t = tex2D(g_Sampler, uv);
		
		ruv.x += Step;
		uv = float2(ruv.x, texcoord.y);
		t += tex2D(g_Sampler, uv);
		
		wt += w * 2;
		t *= w;
		ct += t;
	}
	return ct / wt;
}

#elif defined(_BLUR_Y_)

float4 ps_main(in float2 texcoord : TEXCOORD0) : COLOR0
{
	const float weight_zh[9] = { 1,1,1,1,1,1,1,1,1};
	float wt = 0;
	float4 ct = float4(0,0,0,0);
	
	float Step = g_blurWidth / 14 * g_inputPixelSize.y / g_inputPixelSize.x;
	Step = min(Step, 2 * g_inputPixelSize.y);
	
	float suv;
	float w;

	float2 duv = float2(texcoord.x, texcoord.y);
	w = weight_zh[0];
	ct += tex2D(g_Sampler, duv) * w;
	wt += w;
	
	//return tex2D(g_Sampler, duv);
	
	float2 uuv = duv;
	//float2 uuv = float2(texcoord.x, texcoord.y + Step * 0.5);
	//w = weight_zh[0];
	//ct += tex2D(g_Sampler, uuv) * w;
	//wt += w;

	for(int i = 1; i <= 8; i += 1)  //7月28日想修改, 降低采样数, 增加pass数目
	{
		w = weight_zh[i];
		
		duv.y -= Step;
		float2 uv = float2(texcoord.x, duv.y);
		float4 t = tex2D(g_Sampler, uv);
		
		uuv.y += Step;
		uv = float2(texcoord.x, uuv.y);
		t += tex2D(g_Sampler, uv);
		
		wt += w * 2;
		t *= w;
		ct += t;
	}

	return ct / wt;
}

#endif
