float2 v2HPC : register(c0);

struct VS_INPUT 
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL;
    float3 Tangent  : TANGENT;
    float3 Binormal : BINORMAL;
	float2 Texcoord : TEXCOORD0;
	float2 LmCoord  : TEXCOORD1;
};

struct VS_OUTPUT 
{
	float4 Position : POSITION0;
	float2 Texcoord : TEXCOORD0;
	float3 vPos     : TEXCOORD1;
	float4 Normal   : TEXCOORD2;
    float3 Tangent  : TEXCOORD3;
    float3 Binormal : TEXCOORD4;
};

VS_OUTPUT vs_main(VS_INPUT Input)
{
	VS_OUTPUT Output;

	Output.Position = float4(
				(Input.LmCoord.x - 0.5) * 2 - v2HPC.x, 
				(0.5 - Input.LmCoord.y) * 2 + v2HPC.y, 0,1);
	Output.Normal = Input.Normal;
    Output.Tangent = Input.Tangent;
    Output.Binormal = Input.Binormal;
	Output.Texcoord = Input.Texcoord;
	//Output.LmCoord = Input.LmCoord;
	float4 vPos = Input.Position;
	Output.vPos = vPos.xyz / vPos.w;

	return Output;
}
struct PS_OUTPUT
{
	float4 vPos : COLOR0;
	float4 vNormal : COLOR1;
	float4 vDiffuse : COLOR2;
    float4 vEmission : COLOR3;
};

sampler2D BaseMap   : register(s0);
sampler2D EmitMap   : register(s1);
sampler2D NormalMap   : register(s2);
float4 g_fRef   : register(c1); // (0, isTwoSides, emitW, emitH)
float3 g_vLightPos : register(c2);
float4 g_vNormOpt   : register(c3); // (hasNormMap, NormalFactor, 0, 0)
int g_iPass : register(c4);
float3 g_vLightDir : register(c5);

float4 ps_main(VS_OUTPUT Input): COLOR
{
    //Output.vPos = float4(0, 1, 0, 1);
    //Output.vNormal = float4(1, 0, 0, 1);
    //Output.vDiffuse = float4(0, 1, 0, 1);;
    //Output.vEmission = 1;
    //return Output;
	float4 vDiffuse = tex2Dlod(BaseMap, float4(Input.Texcoord, 0, 0));

	//clip(vDiffuse.w - 0.1f);

  	float4 vEmission = tex2Dlod(EmitMap, float4(Input.Texcoord, 0, 0));

    float3 vLightDir = normalize(g_vLightPos - Input.vPos);
	float3 vNormal = normalize(Input.Normal.xyz);
    int iPass = (int) g_vNormOpt.z;
    if (iPass == 0)
	    return float4(Input.vPos, 1);

	//float fResult = dot(vLightDir, vNormal) > 0 ? 1 : -1;
	//vNormal.xyz = lerp(vNormal, vNormal * fResult, g_fRef.y);
    if (g_vNormOpt.x)
    {
        float3 T = normalize(Input.Tangent);
        float3 B = normalize(Input.Binormal);
        float3 N = vNormal.xyz;
        float3 norm = tex2Dlod(NormalMap, float4(Input.Texcoord, 0, 0)).xyz;
        norm = norm * 2 - 1;
        norm.xy *= g_vNormOpt.y;
        norm = normalize(norm);
        vNormal.xyz = normalize(norm.x * T + norm.y * B + norm.z * N);
    }
    
    //if (g_fRef.y > 0 && Output.vNormal.y < 0)
    //{
    //    Output.vNormal = -Output.vNormal;
    //}

    if (iPass == 1)
    {
        float af = Input.Normal.w;
        float PI2 = 2.0f / 3.14159265f;
        float q = (1 - PI2 * af) * (1 - PI2 * af) / (1 + 2 * (1 - PI2) * af);
        float b = dot(vNormal, g_vLightDir);
        float g = 1 + q * (b - 1);
        float r = sqrt(q * (1 + g) / (1 + b));
        float3 vReflect = (g + r * b) * vNormal - r * g_vLightDir;
        float3 vNorm = vReflect + g_vLightDir;
        if (dot(vNorm, vNormal) < 0)
            vNorm = -vNorm;
        vNorm = normalize(vNorm);
        return float4(vNorm.xyz, 1);
    }


    if (iPass == 2)
	    return vDiffuse;

    float2 ddxuv = ddx(Input.Texcoord);
    float2 ddyuv = ddy(Input.Texcoord);
    float fArea = abs(ddxuv.x * ddyuv.y - ddxuv.y * ddyuv.x);
	return float4(vEmission.xyz, fArea * g_fRef.z * g_fRef.w);
}