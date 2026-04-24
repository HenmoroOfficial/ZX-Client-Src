#include "../common/common.h"
#include "../SkinModel/mesh_comm.h"
string entrynames = "ps_2_0 ps_main";
float4 g_vEyePos : CameraPosition;
float4 g_MainLightPos : LightPosition0;		//	main light's position
float4 g_colDiffuse : LightColor0 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colSpecular : LightColor1 = {1.0f, 1.0f,1.0f,1.0f};
float4 g_colAmbient = {0.0f, 0.0f, 0.0f, 0.0f};
float4 g_colPtDiffuse;


float4x4 g_matWorldInv : WorldInverse;
float4x4 g_matWorldView : WorldView;
float4x4 g_matWorldViewProj : WorldViewProjection;



float4 ps_main(VS_OUTPUT Input) : COLOR0
{
	float ErrorRed =abs(Input.tangent.w)<0.5 ? 1.0f : 0.0f;
	
  float4 ResultColor = float4(ErrorRed,0.0f,0.0f,1.0f);
	return ResultColor;
}
