// 编不过!!!注释掉了!!!
// 替代入口
string entrynames = "ps_2_0 ps_main";
float4 ps_main() : COLOR0
{
	return 0;
}
// 编不过!!!注释掉了!!!

///*ps.1.4
//
//def c0, 1.0, 0.0, 0.5, 1.0
//
//texld r0, t0
//texld r1, t1
//texld r2, t1
//lrp r3, c1.a, r2, r1
//
//
//mov r4, r3.g
//mov r5, r3.b
//sub r2, r0.a, c0.b
//
//phase
//
//texkill r2
//texld r3, r3
//texld r4, r4
//texld r5, r5
//
//mov_x8 r3.g, r3.g
//add r2.r, r3.r, r3.g
//
//mov_x8 r4.g, r4.g
//add r2.g, r4.r, r4.g
//
//mov_x8 r5.g, r5.g
//add r2.b, r5.r, r5.g
//
//mul r0.rgb, r2, r0
//+mov r0.a, c0.a
//*/
//string entrynames = "ps_2_0 ps_main";
//
//sampler g_Sampler0  	: register(s0);
//sampler g_Sampler1  	: register(s1);
//sampler g_Sampler2  	: register(s2);	
//
//float4	g_c0 : register(c0) = float4(1.0, 0.0, 0.5, 1.0);
//float4  g_c1 : register(c1);
//
//float4 ps_main(in float2 	uv0  		: TEXCOORD0,
//               in float2 	uv1 		: TEXCOORD1) : COLOR0
//{
//	float4 r0 = tex2D(g_Sampler0, uv0);
//	float4 r1 = tex2D(g_Sampler1, uv1);
//	float4 r2 = tex2D(g_Sampler2, uv1);
//	float4 r3 = lerp(r2, r1, g_c1.a);
//	
//	float r4 = r3.g;
//	float r5 = r3.b;
//	float r2 =  r0.a - g_c0.b;
//	clip(r2);
//	
//	
//	float4 ret;
//	ret.rgb = r1 * r0; ???
//	ret.a = g_c0.a;
//	return ret;
//}

// 编不过!!!注释掉了!!!