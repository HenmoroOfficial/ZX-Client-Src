string entrynames = "ps_2_0 ps; vs_2_0 vs";	

float2 g_vPixelSize = float2(1.f / 800.f, 1.f / 600.f);

void vs(in float4 position : POSITION,
			 in float2 texcoord : TEXCOORD0,
			 out float4 oPosition : POSITION,
			 out float2 oTexcoord : TEXCOORD0)
{
	oPosition = position;
	oPosition.x -=  g_vPixelSize.x;
	oPosition.y +=  g_vPixelSize.y;
	oTexcoord = texcoord;
}

sampler 		sampIntZ 				: register(s0);
float zn;
float zf;

float4 ps(in float2 texcoord : TEXCOORD0) : COLOR0
{
	float zr = tex2D(sampIntZ, texcoord).r;      //Z buffer读取的结果. 非线性.
	float dis = zf - zn;
	//float z = (zn * zf) / ( dis * (zf / dis - zr)) / zf;	//转换为线性
	float z = zn / (zf -  dis * zr);	//转换为线性
	return float4(z,z,z,z);
}