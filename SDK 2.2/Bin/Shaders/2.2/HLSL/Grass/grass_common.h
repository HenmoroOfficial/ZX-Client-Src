//	Grass shader common params
#include "../fog/fog_common.h"


float4 g_WindOffset[80];
float4x4 g_matViewProjection;
float4 g_vGenMaskUV;
float4 g_LitmapParams; // {scaleDay, scaleNight, 0, g_fDNFactor};
float g_InvSquaredSightRange;
float g_fSoftness;
sampler2D g_BaseSampler : register(s0);
sampler2D g_LMDaySampler : register(s1);
sampler2D g_LMNightSampler : register(s2);
float3 g_vecEyePos;

void _vs_CalcMaskUV(float3 vInPos, out float2 vMaskUV)
{
	vMaskUV.x = (vInPos.x - g_vGenMaskUV.x) * g_vGenMaskUV.z;
	vMaskUV.y = (g_vGenMaskUV.y - vInPos.z) * g_vGenMaskUV.z;
}

