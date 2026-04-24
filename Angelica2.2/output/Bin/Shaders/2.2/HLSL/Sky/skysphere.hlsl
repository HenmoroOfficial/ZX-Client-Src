string entrynames = "vs_2_0 vs_main;ps_2_0 ps_main";
string macro0
<
	string name = "_TRANSFORM_";
	string desc = "Matrix transform";
	string type = "PRG";
	string GroupID = "1";
>;

string macro1
<
	string name = "_BLEND_";
	string desc = "Texture blend";
	string type = "PRG";
	string GroupID = "2";
>;

float4x4 	g_matWVP;
float       g_fTexIn;

#ifdef _TRANSFORM_
float4x4    g_matCurCapCloud;
float4x4    g_matDstCapCloud;
#endif

sampler2D g_CurSampler
<
    string MipFilter = "LINEAR";
	string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
>;

sampler2D g_DestSampler
<
    string MipFilter = "LINEAR";
	string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
>;


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
#ifdef _BLEND_	
	float2 tex1         : TEXCOORD1;
#endif	
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output = (VS_OUTPUT)0;
	Output.position = mul(Input.position, g_matWVP);
	//Output.position.z = Output.position.w;	//	Push to infinite far
	
	Output.diffuse = Input.diffuse;
	
#ifdef _TRANSFORM_	
    Output.tex0 = mul(float4(Input.tex0, 1.0f, 0.0f), g_matCurCapCloud).xy;
  #ifdef _BLEND_
	Output.tex1 = mul(float4(Input.tex0, 1.0f, 0.0f), g_matDstCapCloud).xy;
  #endif
#else
	Output.tex0 = Input.tex0;
  #ifdef _BLEND_
	Output.tex1 = Input.tex0;
  #endif
#endif	
	return Output;
}

float4 ps_main(VS_OUTPUT Input): COLOR
{
    //return float4(Input.tex0, 0, 1);
	float4 CurTex = tex2D(g_CurSampler, Input.tex0);
#ifdef _BLEND_        	 
	float4 DestTex = tex2D(g_DestSampler, Input.tex1);
	float4 ResultColor = lerp(CurTex, DestTex, g_fTexIn);
#else
	float4 ResultColor = CurTex;
#endif	 
    float4 Result = ResultColor * Input.diffuse;
    return Result;
}
