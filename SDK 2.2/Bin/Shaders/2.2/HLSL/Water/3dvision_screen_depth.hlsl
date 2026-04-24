string entrynames = "ps_2_0 ps; vs_2_0 vs";	

float4x4 matProj;
float fFarPlane;

void vs(in float4 position : POSITION,
		out float4 oPosition : POSITION,
		out float oDepth : TEXCOORD0)
{
	oPosition = mul(position, matProj);
	oDepth = position.z;
}

float4 ps(in float oDepth : TEXCOORD0) : COLOR0
{
	float f = oDepth; // / fFarPlane;
	return float4(f, 1, 0, 1);
	//return float4(1,0,0,1);
}