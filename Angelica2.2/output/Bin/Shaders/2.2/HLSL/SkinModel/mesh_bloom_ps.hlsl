// raochao, 2013/10/12

string entrynames = "ps_2_0 ps_main";
float4 vBloomCol : register(c0);

float4 ps_main() : COLOR0
{
	return float4(vBloomCol.x, vBloomCol.y, vBloomCol.z, 1.0);
}