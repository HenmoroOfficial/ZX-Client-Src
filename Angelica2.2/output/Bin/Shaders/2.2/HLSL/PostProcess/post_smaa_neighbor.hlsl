string entrynames = "vs_3_0 vs;ps_3_0 ps";

#define SMAA_PRESET_ULTRA 1

/**
 * Setup mandatory defines. Use a real macro here for maximum performance!
 */
 
float2 SMAA_PIXEL_SIZE;
//#define SMAA_PIXEL_SIZE float2(1.0 / 1548.0, 1.0 / 1024.0)


//#ifndef SMAA_PIXEL_SIZE // It's actually set on runtime, this is for compilation time syntax checking.
//#define SMAA_PIXEL_SIZE float2(1.0 / 1280.0, 1.0 / 600.0)
//#endif

/**
 * This can be ignored; its purpose is to support interactive custom parameter
 * tweaking.
 */
float threshold;
float maxSearchSteps;
float maxSearchStepsDiag;

#ifdef SMAA_PRESET_CUSTOM
#define SMAA_THRESHOLD threshold
#define SMAA_MAX_SEARCH_STEPS maxSearchSteps
#define SMAA_MAX_SEARCH_STEPS_DIAG maxSearchStepsDiag
#define SMAA_FORCE_DIAGONALS 1
#endif

// Set the HLSL version:
#define SMAA_HLSL_3 1

// And include our header!
#include "post_smaa.h"


/**
 * Input vars and textures.
 */
/*
texture2D colorTex2D;
texture2D depthTex2D;
texture2D edgesTex2D;
texture2D blendTex2D;
texture2D areaTex2D;
texture2D searchTex2D;
*/

/**
 * DX9 samplers.
 */
 
sampler2D colorTex : register(s0);
/*
{
    Texture = <colorTex2D>;
    AddressU  = Clamp; AddressV = Clamp;
    MipFilter = Point; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = true;
};
*/
sampler2D colorTexG : register(s1);
/*
{
    Texture = <colorTex2D>;
    AddressU  = Clamp; AddressV = Clamp;
    MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = false;
};
*/

sampler2D depthTex : register(s2);
/*
{
    Texture = <depthTex2D>;
    AddressU  = Clamp; AddressV = Clamp;
    MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = false;
};
*/

sampler2D edgesTex : register(s3);
/*
{
    Texture = <edgesTex2D>;
    AddressU = Clamp; AddressV = Clamp;
    MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = false;
};
*/

sampler2D blendTex : register(s4);
/*
{
    Texture = <blendTex2D>;
    AddressU = Clamp; AddressV = Clamp;
    MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = false;
};
*/

sampler2D areaTex : register(s5);
/*
{
    Texture = <areaTex2D>;
    AddressU = Clamp; AddressV = Clamp; AddressW = Clamp;
    MipFilter = Linear; MinFilter = Linear; MagFilter = Linear;
    SRGBTexture = false;
};
*/

sampler2D searchTex : register(s6);

/*
{
    Texture = <searchTex2D>;
    AddressU = Clamp; AddressV = Clamp; AddressW = Clamp;
    MipFilter = Point; MinFilter = Point; MagFilter = Point;
    SRGBTexture = false;
};
*/

/**
 * Function wrappers
 */


void vs(inout float4 position : POSITION,
                                    inout float2 texcoord : TEXCOORD0,
                                    out float4 offset[2] : TEXCOORD1) {
    SMAANeighborhoodBlendingVS(position, position, texcoord, offset);
}

float4 ps(float4 position : SV_POSITION,
                                      float2 texcoord : TEXCOORD0,
                                      float4 offset[2] : TEXCOORD1
                                      /*uniform SMAATexture2D colorTex,
                                      uniform SMAATexture2D blendTex*/) : COLOR {
    return SMAANeighborhoodBlendingPS(texcoord, offset, colorTex, blendTex);
}

/*

technique LumaEdgeDetection {
    pass LumaEdgeDetection {
        VertexShader = compile vs_3_0 DX9_SMAAEdgeDetectionVS();
        PixelShader = compile ps_3_0 DX9_SMAALumaEdgeDetectionPS(colorTexG);
        ZEnable = false;        
        SRGBWriteEnable = false;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;

        // We will be creating the stencil buffer for later usage.
        StencilEnable = true;
        StencilPass = REPLACE;
        StencilRef = 1;
    }
}

technique ColorEdgeDetection {
    pass ColorEdgeDetection {
        VertexShader = compile vs_3_0 DX9_SMAAEdgeDetectionVS();
        PixelShader = compile ps_3_0 DX9_SMAAColorEdgeDetectionPS(colorTexG);
        ZEnable = false;        
        SRGBWriteEnable = false;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;

        // We will be creating the stencil buffer for later usage.
        StencilEnable = true;
        StencilPass = REPLACE;
        StencilRef = 1;
    }
}

technique DepthEdgeDetection {
    pass DepthEdgeDetection {
        VertexShader = compile vs_3_0 DX9_SMAAEdgeDetectionVS();
        PixelShader = compile ps_3_0 DX9_SMAADepthEdgeDetectionPS(depthTex);
        ZEnable = false;        
        SRGBWriteEnable = false;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;

        // We will be creating the stencil buffer for later usage.
        StencilEnable = true;
        StencilPass = REPLACE;
        StencilRef = 1;
    }
}

technique BlendWeightCalculation {
    pass BlendWeightCalculation {
        VertexShader = compile vs_3_0 DX9_SMAABlendingWeightCalculationVS();
        PixelShader = compile ps_3_0 DX9_SMAABlendingWeightCalculationPS(edgesTex, areaTex, searchTex);
        ZEnable = false;
        SRGBWriteEnable = false;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;

        // Here we want to process only marked pixels.
        StencilEnable = true;
        StencilPass = KEEP;
        StencilFunc = EQUAL;
        StencilRef = 1;
    }
}

technique NeighborhoodBlending {
    pass NeighborhoodBlending {
        VertexShader = compile vs_3_0 DX9_SMAANeighborhoodBlendingVS();
        PixelShader = compile ps_3_0 DX9_SMAANeighborhoodBlendingPS(colorTex, blendTex);
        ZEnable = false;
        SRGBWriteEnable = true;
        AlphaBlendEnable = false;
        AlphaTestEnable = false;

        // Here we want to process all the pixels.
        StencilEnable = false;
    }
}
*/

