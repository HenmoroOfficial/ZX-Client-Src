#include "../common/common.h"

string entrynames = "ps_2_0 ps_main";
float4 g_vTime : TimePack;
float4 ps_main() : COLOR0
{      
	return float4(g_vTime.y * 0.25 + 0.75, 0, 0, 1);
}


