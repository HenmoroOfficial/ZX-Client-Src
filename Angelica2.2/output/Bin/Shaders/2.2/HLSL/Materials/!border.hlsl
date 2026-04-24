#include "../common/common.h"

string entrynames = "ps_2_0 ps_main";
float4 g_Color : register(c0);
float4 ps_main() : COLOR0
{      
	return g_Color;
}


