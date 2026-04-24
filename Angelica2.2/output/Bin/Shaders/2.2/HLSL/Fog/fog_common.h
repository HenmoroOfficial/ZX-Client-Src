float4 g_vFogParam1:FogParam1; 
#define g_FogStart           g_vFogParam1.x
#define g_FogRcpDist         g_vFogParam1.y
#define g_HeightFogTop       g_vFogParam1.z
#define g_HeightRcpIntensity g_vFogParam1.w 

float4 g_vFogParam2:FogParam2;   
#define g_FogDensityUpper    g_vFogParam2.x
#define g_FogDensityLower    g_vFogParam2.y
#define g_MidIntensity       g_vFogParam2.z
#define g_MidPower           g_vFogParam2.w 


float4 g_vFogColor1:FogColor1;
float4 g_vFogColor2:FogColor2;

float4 ComputeCompositeFog(
    float3 cameraToWorldPos, 
    float WorldPosHeight, 
    float cFogStart, 
    float cFogRcpDist, 
    float cHeightFogTop, 
    float cHeightRcpIntensity, 
    float4 cFogColorUpper, 
    float4 cFogColorLower, 
    float cFogDensityUpper,
	float cFogDensityLower, 
	float cMidIntensity, 
	float cMidPower)
{
	float3 g_FogDirection = float3(0, -1.0, 0);
	float dist = length(cameraToWorldPos);
	float3 viewVector = cameraToWorldPos / dist;
	
	
	// Part1: 视角相关雾
	float b = dot(g_FogDirection, viewVector);//viewVector 为当前像素点视线方向
	
	float midIntensity = pow(1.5 - abs(b), cMidIntensity);//对水平方向雾浓度进行调整
	float a =  pow(b * 0.5 + 0.5, cMidPower);//获得上下半球雾的插值系数
	float d = lerp(cFogDensityUpper, cFogDensityLower, a) * midIntensity;//对雾浓度进行插值
	float4 color = lerp(cFogColorUpper, cFogColorLower, a);	//对雾颜色进行插值

	float uniformDist = saturate((dist - cFogStart) * cFogRcpDist); // 距离转换到strat-end的0..1空间
	float expFog = (1 - 1 / exp(uniformDist * d));  //进行指数雾计算
	float fog = saturate(expFog * color.a);  

	// Part2: 额外垂直方向雾的计算
	float dist_h = length(cameraToWorldPos.xz);
	float uniformHeight = (cHeightFogTop - WorldPosHeight) * cHeightRcpIntensity; // 高度转换到0..1空间
	float distFactor = dist * 0.001;
	float expFogHeight = max(0, (1 - 1 / exp(uniformHeight * distFactor))); //进行指数雾计算
	fog += expFogHeight * abs(b) * 2;
	
	return float4(color.rgb, 1 - saturate(fog));//返回并做blend
}


/*
float ComputeGlobalFog(float z,                  // View 空间Z
                       float fEyeHeight,         // 摄象机位置的Y
                       float fWorldHeight, 		 // 摄象机位置的Y
                       float3 vStartParams, 	 // 同g_vFogStartParams
                       float3 vRcpParams)		 // 同g_vFogRcpParams
{
	// 用向量包装3个公式同时计算
	float3 vStart = vStartParams.xyy; // { fDistStart, fHeightStart, fHeightStart };
	float3 vRcp   = vRcpParams.xyy;   // { fDistRcp, fHeightRcp, fHeightRcp };
	float3 vValue = { z, fEyeHeight, fWorldHeight };
   
	float3 vFactor = saturate((vValue - vStart) * vRcp); // (x - start) / (end - start)

	float fDistFactor = vFactor.x;
	
	//vDensity = { 视点浓度， 物体点浓度 }
	float2 vDensity = lerp(vStartParams.z, vRcpParams.z, vFactor.yz);
	//fDensity = 视点浓度 * 0.3 + 物体点浓度 * 0.7
	float fDensity = dot(vDensity, float2(0.3, 0.7)); 
	
	// exp2 计算雾
	fDensity *= fDistFactor;
	float fFogValue = saturate(exp(-fDensity * fDensity));
	return fFogValue;
}


// 直接使用全局变量
float4 ComputeFog(float z, float fEyeHeight, float fWorldHeight)
{
//	if (g_vFogColor.w)
//		return float4(g_vFogColor.rgb, ComputeGlobalFog(z, fEyeHeight, fWorldHeight, g_vFogStartParams, g_vFogRcpParams));
//	else
		return 1;
}
*/

// 直接使用全局变量
float4 ComputeCompositeFogSimple(float3 cameraToWorldPos, float WorldPosHeight)
{
    //float y = (WorldPosHeight - 100) / 200;
    //return float4(y, y, y, 0);
	if (g_vFogColor1.w > 0 || g_vFogColor2.w > 0)
	{
		return ComputeCompositeFog(cameraToWorldPos, WorldPosHeight, 
	            	g_vFogParam1.x, g_vFogParam1.y, g_vFogParam1.z, g_vFogParam1.w,
	            	g_vFogColor1, g_vFogColor2,     
	            	g_vFogParam2.x, g_vFogParam2.y, g_vFogParam2.z, g_vFogParam2.w);
	}
	else
		return 1;
}
