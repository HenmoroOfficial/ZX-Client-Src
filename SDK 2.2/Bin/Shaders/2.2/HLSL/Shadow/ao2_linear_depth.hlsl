string entrynames = "ps_3_0 ps";		
		
sampler g_GBufferSampler	: register(s0);	//A3D_SAMPLER2D_DEF_SIMPLE(g_GBufferSampler, g_GBuffer, LINEAR, CLAMP)
float4 g_param				: register(c0);
//float zn;
//float zf;
//float2 g_InvFocalLen
float4 g_param2				: register(c1);
//float2 g_vPixelSize

float3 UV2Eye(float2 uv, float eyeZ, float2 g_InvFocalLen)
{
    uv = (uv * float2(2.0, -2.0) - float2(1.0, -1.0));
    return float3(uv * g_InvFocalLen * eyeZ, eyeZ);
}

float ComputeLinearZ(float unlinearZ, float zf, float zn)
{
	float dis = zf - zn;
	float z = (zn * zf) / ( dis * (zf / dis - unlinearZ)) / zf;	//转换为线性
	return z;
}

float4 ps( float2 vTexCoord	: TEXCOORD0) : COLOR0
{
	float zn 					= g_param.x;
	float zf 					= g_param.y;
	float2 g_InvFocalLen 		= g_param.zw;
	float2 g_vPixelSize			= g_param2.xy;
	
	float zr = tex2D(g_GBufferSampler, vTexCoord).r;
	float z = ComputeLinearZ(zr, zf, zn);
/*	
	float3 centerPos = UV2Eye(vTexCoord, z, g_InvFocalLen);		//得到eye空间的位置

	float2 leftUV 	= vTexCoord + float2(-g_vPixelSize.x, 0);
	float2 rightUV 	= vTexCoord + float2(g_vPixelSize.x, 0);
	float2 upUV 	= vTexCoord + float2(0, -g_vPixelSize.y);
	float2 downUV 	= vTexCoord + float2(0, g_vPixelSize.y);
	
	float leftDepth 	= tex2D( g_GBufferSampler, leftUV).r;
	float rightDepth 	= tex2D( g_GBufferSampler, rightUV).r;
	float upDepth 		= tex2D( g_GBufferSampler, upUV).r;
	float downDepth 	= tex2D( g_GBufferSampler, downUV).r;
	
	leftDepth = ComputeLinearZ(leftDepth, zf, zn);
	rightDepth = ComputeLinearZ(rightDepth, zf, zn);
	upDepth = ComputeLinearZ(upDepth, zf, zn);
	downDepth = ComputeLinearZ(downDepth, zf, zn);
	
	float3 uvDepth;
	float3 xOffPos, yOffPos;
	if (abs(leftDepth - z) > abs(rightDepth - z))
	{
		uvDepth.xy = rightUV;
		uvDepth.z = rightDepth;
	}
	else
	{
		uvDepth.xy = leftUV;
		uvDepth.z = leftDepth;
	}
	xOffPos = UV2Eye(uvDepth.xy, uvDepth.z, g_InvFocalLen);
	
	
	if (abs(upDepth - z) > abs(downDepth - z))
	{
		uvDepth.xy = downUV;
		uvDepth.z = downDepth;
	}
	else
	{
		uvDepth.xy = upUV;
		uvDepth.z = upDepth;
	}
	yOffPos = UV2Eye(uvDepth.xy, uvDepth.z, g_InvFocalLen);
	
	float3 eyeNormal = normalize(cross(xOffPos - centerPos, yOffPos - centerPos));
	
	if(dot(eyeNormal, centerPos) > 0)
		eyeNormal = -eyeNormal;
*/	
	return float4(z,z,z,z);
}
