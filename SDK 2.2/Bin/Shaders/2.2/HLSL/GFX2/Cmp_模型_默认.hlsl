#include "gfx_common.h"
#include "../SkinModel/Mesh_Comm.h"

string entrynames = "ps_2_0 ps_main";

struct psInput 
{
	float4 pos	            : POSITION0;
	float2 uvBase           : TEXCOORD0;
};

float4 g_GfxDiffuse = {1.0f, 1.0f,1.0f,1.0f};

sampler g_DiffuseMap : DiffuseMap_GFX_Replace
<
    string MipFilter = "LINEAR";
    string MinFilter = "LINEAR";
    string MagFilter = "LINEAR";
    string AddressU = "WRAP";
    string AddressV = "WRAP";
>;

float4 ps_main(psInput Input) : COLOR0
{
	float4 col;

	col = tex2D(g_DiffuseMap, Input.uvBase) * g_GfxDiffuse;
	
	return col;
}