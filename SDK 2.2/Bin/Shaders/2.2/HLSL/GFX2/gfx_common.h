/*-----------------------------------------------------------------
	Gfx Common For VFX Shader
	Devleoped by wei wang (Engine Center)
	Perfect World, 2012-4
-----------------------------------------------------------------*/

float4 g_TexTrans;			//	used to translate the animated UV coordinates back to original uv
float4 g_Time;				  //	.x key point time (may loop as the key point repeated) .yzw 0
float4 g_FogColor;			//
float4 g_ScreenSize;		//	.xy width and height of screen (.zw 0)
float4 g_ScrLightDir;		//	light direction in screen space (x(right) y(up), z(inside))

float2 glb_GetOriginTexUV(float2 uvCur)
{
	return uvCur * g_TexTrans.xy + g_TexTrans.wz;
}

//	Get screen space UV coord by screen pos and screen size
//	you may get vScreenPos by VPOS register in ps 3.0 ONLY
float2 glb_GetScreenUV(float2 vScreenPos)
{
	return (vScreenPos + 0.5) / g_ScreenSize.xy;
}

float4 glb_GetEdgeColor(sampler GradientDissolveSampler,sampler edgeSampler,float2 TexCoord,float edgeContrast,float MaskSpan,float Timer)
{
	float3 GradientDissolveTex = tex2D(GradientDissolveSampler, TexCoord).rgb;
	float Mask1=GradientDissolveTex.r+MaskSpan;
	float Mask2=GradientDissolveTex.r;
	
	float M1= Timer >= Mask1 ? 1.0f : 0.0f;
	float M2= Timer >= Mask2 ? 1.0f : 0.0f;
	
	float OneZeroMask =M2-M1;
	
	float2 edgeUV;
	edgeUV.x = OneZeroMask > 0 ? (Timer - Mask2) / (MaskSpan + 0.001) : M2 * M1;
	edgeUV.y = 0;
	
	float4 edgeTex = tex2D(edgeSampler, edgeUV);
	float4 _edgeColor =0.0f;
	_edgeColor.rgb = float3(5, 2, 1) * pow(edgeTex.rgb,edgeContrast);	
	_edgeColor.a = edgeTex.a;
	return _edgeColor;
}

float4 glb_GetEdgeColorCustom(sampler GradientDissolveSampler,sampler edgeSampler,float2 TexCoord,float edgeContrast,float MaskSpan,float3 CustomColor,float Timer)
{
	float3 GradientDissolveTex = tex2D(GradientDissolveSampler, TexCoord).rgb;
	float Mask1=GradientDissolveTex.r+MaskSpan;
	float Mask2=GradientDissolveTex.r;
	
	float M1= Timer >= Mask1 ? 1.0f : 0.0f;
	float M2= Timer >= Mask2 ? 1.0f : 0.0f;
	
	float OneZeroMask =M2-M1;
	
	float2 edgeUV;
	edgeUV.x = OneZeroMask > 0 ? (Timer - Mask2) / (MaskSpan + 0.001) : M2 * M1;
	edgeUV.y = 0;
	
	float4 edgeTex = tex2D(edgeSampler, edgeUV);
	float4 _edgeColor =0.0f;
	_edgeColor.rgb = CustomColor * pow(edgeTex.rgb,edgeContrast);	
	_edgeColor.a = edgeTex.a;
	return _edgeColor;
}

float2 glb_GetRotUV(float time,float2 UV,float2 RotateCenter)
{
	float Local1=sin(time);
	float Local2=cos(time);
	
	float2 _UserInputParam0=float2(Local1,Local2);
	float2 _UserInputParam1=float2(Local2,-Local1);
	
	float2 uvToCenter=UV.xy-RotateCenter.xy;
	float  Local5=dot(uvToCenter,_UserInputParam0.xy);
	float  Local6=dot(uvToCenter,_UserInputParam1.xy);
	float2 newTexCoord=RotateCenter+float2(Local6,Local5);
	return newTexCoord;
}