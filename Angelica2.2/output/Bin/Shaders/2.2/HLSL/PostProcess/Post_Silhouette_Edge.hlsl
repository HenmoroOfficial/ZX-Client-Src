string entrynames = "ps_2_0 ps_main";

sampler ImageMap: register(s0);
// This shader applies a Sobel filter to detect edges in the image.
// The Sobel filter extracts the first order derivates of the image,
// that is, the slope. Where the slope is sharp there is an edge.
// These are the filter kernels:
//
//  SobelX       SobelY
//  1  0 -1      1  2  1
//  2  0 -2      0  0  0
//  1  0 -1     -1 -2 -1


// One pixel offset
const float2 off = float2(1.0 / 512.0, 1.0 / 512.0);
float4 mask;

float4 ps_main(float2 texCoord: TEXCOORD) : COLOR
{
 	float fD = dot(tex2D(ImageMap, texCoord), mask);
 	//return float4(fD, fD, fD, 1);

	// Sample neighbor pixels
	float s00 = dot(tex2D(ImageMap, texCoord + float2(-off.x, -off.y)), mask);
	float s01 = dot(tex2D(ImageMap, texCoord + float2( 0,     -off.y)), mask);
	float s02 = dot(tex2D(ImageMap, texCoord + float2( off.x, -off.y)), mask);

	float s10 = dot(tex2D(ImageMap, texCoord + float2(-off.x,      0)), mask);
	float s11 = dot(tex2D(ImageMap, texCoord + float2(0,           0)), mask);
	float s12 = dot(tex2D(ImageMap, texCoord + float2( off.x,      0)), mask);

	float s20 = dot(tex2D(ImageMap, texCoord + float2(-off.x,  off.y)), mask);
	float s21 = dot(tex2D(ImageMap, texCoord + float2( 0,      off.y)), mask);
	float s22 = dot(tex2D(ImageMap, texCoord + float2( off.x,  off.y)), mask);

	// Sobel filter in X direction
	float sobelX = s00 + 2 * s10 + s20 - s02 - 2 * s12 - s22;
	// Sobel filter in Y direction
	float sobelY = s00 + 2 * s01 + s02 - s20 - 2 * s21 - s22;

	float fSurroundSum = s00 + s01 + s02 + s10 + s12 + s20 + s21 + s22;


	// Find edge, skip sqrt() to improve performance ...
	float edgeSqr = (sobelX * sobelX + sobelY * sobelY);
	// ... and threshold against a squared value instead.
	return (edgeSqr > 0.00002) && (s11 * 8.0 < fSurroundSum);
}
