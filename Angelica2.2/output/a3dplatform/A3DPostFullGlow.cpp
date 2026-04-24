/*
 * FILE: A3DPostFullGlow.cpp
 *
 * DESCRIPTION: FullGlow后处理
 *
 * CREATED BY: Panyupin, 2012/3/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostFullGlow.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_glowthreshold_hlsl = "shaders\\HLSL\\PostProcess\\post_glowthreshold.hlsl";
static const char* szPost_glowblur_hlsl = "shaders\\HLSL\\PostProcess\\post_glowblur.hlsl";
static const char* szPost_GlowMerge_hlsl = "shaders\\HLSL\\PostProcess\\Post_GlowMerge.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_glowthreshold_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_glowthreshold.hlsl";
static const char* szPost_glowblur_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_glowblur.hlsl";
static const char* szPost_GlowMerge_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_GlowMerge.hlsl";
#endif


extern ALog g_A3DErrLog;

A3DPostFullGlow::~A3DPostFullGlow()
{

}

bool A3DPostFullGlow::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	m_pThresholdPS = CreateA3DHLSL_Common(NULL, NULL, szPost_glowthreshold_hlsl, "ps_main", 0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pThresholdPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! post_glowthreshold.hlsl load failed!");
		Release();
		return false;
	}

	const char* macros[1];
	macros[0] = "_BLUR_X_";
	m_pBlurXPS = CreateA3DHLSL_Common(NULL, NULL, szPost_glowblur_hlsl, "ps_main", 1, macros,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	if (!m_pBlurXPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! post_glowblur.hlsl load failed!");
		Release();
		return false;
	}

	macros[0] = "_BLUR_Y_";
	m_pBlurYPS = CreateA3DHLSL_Common(NULL, NULL, szPost_glowblur_hlsl, "ps_main", 1, macros,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	if (!m_pBlurYPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! post_glowblur.hlsl load failed!");
		Release();
		return false;
	}

	m_pMergePS = CreateA3DHLSL_Common(NULL, NULL, szPost_GlowMerge_hlsl, "ps_main", 0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pMergePS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_GlowMerge.hlsl load failed!");
		Release();
		return false;
	}

	return true;
}
void A3DPostFullGlow::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pThresholdPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pBlurXPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pBlurYPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pMergePS);
}

bool A3DPostFullGlow::Render( A3DViewport* pViewport )
{
	/*
#ifdef _DEBUG_POST_
	if(GetAsyncKeyState('1') & 0x8000)
		return true;
#endif
	*/

	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	A3DRenderTarget*	m_pBlurRT1 = NULL;
	A3DRenderTarget*	m_pBlurRT2 = NULL;

	m_pBlurRT1 = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(-4, -4, A3DFMT_A8R8G8B8);
	m_pBlurRT2 = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(-4, -4, A3DFMT_A8R8G8B8);
	if(!m_pBlurRT1 || !m_pBlurRT2)
	{
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
		return false;
	}

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);
	
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

	//降采样
	{
		//设置RnederTarget
		m_pBlurRT1->ApplyToDevice();

		//设置Shader参数
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
		m_pSystem->GetCommonVS()->Appear();
		m_pThresholdPS->Setup1f("g_fThreshold", 0);
		m_pThresholdPS->Setup1f("g_fBrightScale", 1.f);
		m_pThresholdPS->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
		m_pThresholdPS->Appear();
		pSrc->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pThresholdPS->Disappear();
		m_pBlurRT1->WithdrawFromDevice();
	}

#ifdef _DEBUG_POST_
	//if(GetAsyncKeyState('2') & 0x8000)
	//{ 		
	//	D3DXSaveSurfaceToFileA("FullGlowInput.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface(), NULL, NULL);
	//	D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, m_pLayer2BlurRT1->GetTargetSurface(), NULL, NULL);
	//	D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1Blur2.dds", D3DXIFF_DDS, m_pLayer3BlurRT1->GetTargetSurface(), NULL, NULL);
	//}
#endif

	//模糊
	{
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
		m_pSystem->GetCommonVS()->Appear();
		m_pBlurXPS->Setup1f("g_blurWidth", m_fBlurSize);
		m_pBlurXPS->Setup2f("g_inputPixelSize", 4.f / m_pSystem->GetWidth(), 4.f / m_pSystem->GetHeight());

		//X方向
		m_pBlurRT2->ApplyToDevice();
		m_pBlurXPS->Appear();
		m_pBlurRT1->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurXPS->Disappear();
		m_pBlurRT2->WithdrawFromDevice();

		//Y方向
		m_pBlurRT1->ApplyToDevice();
		m_pBlurYPS->Appear();
		m_pBlurRT2->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurYPS->Disappear();
		m_pBlurRT1->WithdrawFromDevice();

		//X方向
		m_pBlurRT2->ApplyToDevice();
		m_pBlurXPS->Appear();
		m_pBlurRT1->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurXPS->Disappear();
		m_pBlurRT2->WithdrawFromDevice();

		//Y方向
		m_pBlurRT1->ApplyToDevice();
		m_pBlurYPS->Appear();
		m_pBlurRT2->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurYPS->Disappear();
		m_pBlurRT1->WithdrawFromDevice();

	}
#ifdef _DEBUG_POST_
	//if(GetAsyncKeyState('2') & 0x8000)
	//{ 		
	//	D3DXSaveSurfaceToFileA("FullGlowBlur.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface(), NULL, NULL);
	//	D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, m_pLayer2BlurRT1->GetTargetSurface(), NULL, NULL);
	//	D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1Blur2.dds", D3DXIFF_DDS, m_pLayer3BlurRT1->GetTargetSurface(), NULL, NULL);
#endif
	//}

	//Merge
	{
		pDst->ApplyToDevice();
		//设置Shader参数
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
		m_pSystem->GetCommonVS()->Appear();
		
		m_pMergePS->Setup1f("g_fGlowLevel", m_fGlowLevel);
		m_pMergePS->Setup1f("g_fGlowPower", m_fGlowPower);
		m_pMergePS->SetupVec4("g_vGlowColor", m_vGlowColor);
		m_pMergePS->Appear();
		pSrc->AppearAsTexture(0);
		m_pBlurRT1->AppearAsTexture(1);

		m_pSystem->RenderQuad();
		
		m_pMergePS->Disappear();
		pDst->WithdrawFromDevice();
	}
	m_pSystem->GetCommonVS()->Disappear();

	m_pSystem->SwapSrcDstRT();

	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);

	return true;
}

bool A3DPostFullGlow::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

void A3DPostFullGlow::SetBlurSize( float fBlurSize )
{
	fBlurSize = max(0.f, fBlurSize);
	fBlurSize = min(0.1f, fBlurSize);
	m_fBlurSize = fBlurSize;
}

A3DPOST_EFFECT A3DPostFullGlow::GetEffectType() const
{
	return A3DPOST_EFFECT_FullGlow;
}

A3DRenderTarget* A3DPostFullGlow::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    A3DRenderTarget*	m_pBlurRT1 = NULL;
    A3DRenderTarget*	m_pBlurRT2 = NULL;
    int rtWidth		= pSrcRT->GetWidth();
    int rtHeight	= pSrcRT->GetHeight();
    int rtWidth_4	= rtWidth / 4;
    int rtHeight_4	= rtHeight / 4;


    m_pBlurRT1 = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtWidth_4, A3DFMT_A8R8G8B8);
    m_pBlurRT2 = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtWidth_4, A3DFMT_A8R8G8B8);
    if(!m_pBlurRT1 || !m_pBlurRT2)
    {
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
        return pSrcRT;
    }

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    //降采样
    {
        //设置RnederTarget
        m_pBlurRT1->ApplyToDevice();

        //设置Shader参数
        m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
        m_pSystem->GetCommonVS()->Appear();
        m_pThresholdPS->Setup1f("g_fThreshold", 0);
        m_pThresholdPS->Setup1f("g_fBrightScale", 1.f);
        m_pThresholdPS->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
        m_pThresholdPS->Appear();
        pSrcRT->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pThresholdPS->Disappear();
        m_pBlurRT1->WithdrawFromDevice();
    }

#ifdef _DEBUG_POST_
    //if(GetAsyncKeyState('2') & 0x8000)
    //{ 		
    //	D3DXSaveSurfaceToFileA("FullGlowInput.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface(), NULL, NULL);
    //	D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, m_pLayer2BlurRT1->GetTargetSurface(), NULL, NULL);
    //	D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1Blur2.dds", D3DXIFF_DDS, m_pLayer3BlurRT1->GetTargetSurface(), NULL, NULL);
    //}
#endif

    //模糊
    {
        m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
        m_pSystem->GetCommonVS()->Appear();
        m_pBlurXPS->Setup1f("g_blurWidth", m_fBlurSize);
        m_pBlurXPS->Setup2f("g_inputPixelSize", 4.f / m_pSystem->GetWidth(), 4.f / m_pSystem->GetHeight());

        //X方向
        m_pBlurRT2->ApplyToDevice();
        m_pBlurXPS->Appear();
        m_pBlurRT1->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurXPS->Disappear();
        m_pBlurRT2->WithdrawFromDevice();

        //Y方向
        m_pBlurRT1->ApplyToDevice();
        m_pBlurYPS->Appear();
        m_pBlurRT2->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurYPS->Disappear();
        m_pBlurRT1->WithdrawFromDevice();

        //X方向
        m_pBlurRT2->ApplyToDevice();
        m_pBlurXPS->Appear();
        m_pBlurRT1->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurXPS->Disappear();
        m_pBlurRT2->WithdrawFromDevice();

        //Y方向
        m_pBlurRT1->ApplyToDevice();
        m_pBlurYPS->Appear();
        m_pBlurRT2->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurYPS->Disappear();
        m_pBlurRT1->WithdrawFromDevice();

    }
#ifdef _DEBUG_POST_
    //if(GetAsyncKeyState('2') & 0x8000)
    //{ 		
    //	D3DXSaveSurfaceToFileA("FullGlowBlur.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface(), NULL, NULL);
    //	D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, m_pLayer2BlurRT1->GetTargetSurface(), NULL, NULL);
    //	D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1Blur2.dds", D3DXIFF_DDS, m_pLayer3BlurRT1->GetTargetSurface(), NULL, NULL);
#endif
    //}

    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);
    //Merge
    {

        pDst->ApplyToDevice();
        //设置Shader参数
        m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
        m_pSystem->GetCommonVS()->Appear();

        m_pMergePS->Setup1f("g_fGlowLevel", m_fGlowLevel);
        m_pMergePS->Setup1f("g_fGlowPower", m_fGlowPower);
        m_pMergePS->SetupVec4("g_vGlowColor", m_vGlowColor);
        m_pMergePS->Appear();
        pSrcRT->AppearAsTexture(0);
        m_pBlurRT1->AppearAsTexture(1);

        m_pSystem->RenderQuad();

        m_pMergePS->Disappear();
        pDst->WithdrawFromDevice();
    }
    m_pSystem->GetCommonVS()->Disappear();

    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

    return pDst;
}
