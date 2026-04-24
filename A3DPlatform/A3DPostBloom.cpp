/*
 * FILE: A3DPostBloom.cpp
 *
 * DESCRIPTION: Bloom后处理
 *
 * CREATED BY: Panyupin, 2012/2/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostBloom.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_BloomThreshold_hlsl = "shaders\\HLSL\\PostProcess\\Post_BloomThreshold.hlsl";
static const char* szPost_BloomBlur_hlsl = "shaders\\HLSL\\PostProcess\\Post_BloomBlur.hlsl";
static const char* szPost_BloomMerge_hlsl = "shaders\\HLSL\\PostProcess\\Post_BloomMerge.hlsl";
static const char* szPost_bloomdownsamp_hlsl = "shaders\\HLSL\\PostProcess\\post_bloomdownsamp.hlsl";
static const char* szPost_bloombluradd_hlsl = "shaders\\HLSL\\PostProcess\\post_bloombluradd.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_BloomThreshold_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_BloomThreshold.hlsl";
static const char* szPost_BloomMask_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_BloomMask.hlsl";
static const char* szPost_BloomBlur_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_BloomBlur.hlsl";
static const char* szPost_BloomMerge_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_BloomMerge.hlsl";
static const char* szPost_bloomdownsamp_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_bloomdownsamp.hlsl";
static const char* szPost_bloombluradd_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_bloombluradd.hlsl";
#endif


extern ALog g_A3DErrLog;

A3DPostBloom::~A3DPostBloom()
{

}

bool A3DPostBloom::Init( A3DPostSystem* pSystem, bool bLocalBloom)
{
	m_pSystem = pSystem;

	if (!bLocalBloom)
	{
		m_pThresholdPS = CreateA3DHLSL_Common(NULL, NULL, szPost_BloomThreshold_hlsl, "ps_main", 0, NULL, 
			m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());
	}
	/*else
	{
	#ifndef _ANGELICA_2_1
		m_pMaskPS = CreateA3DHLSL_Common(NULL, NULL, szPost_BloomMask_hlsl, "ps_main", 0, NULL,
			m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());
	#endif
	}*/

	const char* szMacrosX[1];
	szMacrosX[0] = "_BLUR_X_";
	m_pBlurXPS = CreateA3DHLSL_Common(NULL, NULL, szPost_BloomBlur_hlsl, "ps_main", 1, szMacrosX,
		m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());
	
	const char* szMacrosY[1];
	szMacrosY[0] = "_BLUR_Y_";

	m_pBlurYPS = CreateA3DHLSL_Common(NULL, NULL, szPost_BloomBlur_hlsl, "ps_main", 1, szMacrosY,
		m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());


	m_pMergePS = CreateA3DHLSL_Common(NULL, NULL, szPost_BloomMerge_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());
		

	m_pDownSampPS = CreateA3DHLSL_Common(NULL, NULL, szPost_bloomdownsamp_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());
		
	
	m_pAddBlurPS = CreateA3DHLSL_Common(NULL, NULL, szPost_bloombluradd_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), pSystem->GetD3D9VertElement());

	/*if(!m_pThresholdPS && !m_pMaskPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_BloomThreshold.hlsl load failed!");
		goto FAILED;
	}*/
	if(!m_pBlurXPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_BloomBlur.hlsl load failed!");
		goto FAILED;
	}
	if(!m_pBlurYPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_BloomBlur.hlsl load failed!");
		goto FAILED;
	}
	if(!m_pMergePS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_BloomMerge.hlsl load failed!");
		goto FAILED;
	}
	if(!m_pDownSampPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! post_bloomdownsamp.hlsl load failed!");
		goto FAILED;
	}
	if(!m_pAddBlurPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! post_bloombluradd.hlsl load failed!");
		goto FAILED;
	}

	return true;

FAILED:
	Release();
	return false;
}
void A3DPostBloom::Release()
{
	/*if (m_pMaskPS)
		ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pMaskPS);*/
	if (m_pThresholdPS)
		ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pThresholdPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pBlurXPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pBlurYPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pMergePS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pDownSampPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pAddBlurPS);
}

bool A3DPostBloom::Render( A3DViewport* pViewport )
{
	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	A3DFORMAT fmt = m_pSystem->GetSmallestFloatRGBAFmt();
	if(fmt == A3DFMT_UNKNOWN)
		return false;

    fmt = A3DFMT_A8R8G8B8;

	int rtWidth = pViewport->GetParam()->Width;
	int rtHeight = pViewport->GetParam()->Height;
	int rtWidth_2 = rtWidth / 2;
	int rtHeight_2 = rtHeight / 2;
	int rtWidth_4 = rtWidth / 4;
	int rtHeight_4 = rtHeight / 4;
	int rtWidth_8 = rtWidth / 8;
	int rtHeight_8 = rtHeight / 8;

	A3DRenderTarget*	pLayer1BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmt);
	A3DRenderTarget*	pLayer1BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmt);

	A3DRenderTarget*	pLayer2BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmt);
	A3DRenderTarget*	pLayer2BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmt);

	A3DRenderTarget*	pLayer3BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_8, rtHeight_8, fmt);
	A3DRenderTarget*	pLayer3BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_8, rtHeight_8, fmt);

	//m_fBlurSize = 1.f;//m_fBlurSize;

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);
// 	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
// 	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

	for(int i = 0; i <=3; i++)
	{
		pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
		pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	}

	//提取阈值 (同时降采样)
	{
		if (m_pThresholdPS)
		{
			pLayer1BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
			m_pSystem->GetCommonVS()->Appear();

			m_pThresholdPS->SetValue1f("g_fThreshold", m_fBrightThreshold);
			m_pThresholdPS->SetValue1f("g_fBrightScale", m_fBrightScale);
			m_pThresholdPS->SetValue2f("g_vPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
			m_pThresholdPS->Appear();
			pSrc->AppearAsTexture(0);

			m_pSystem->RenderQuad();

			m_pThresholdPS->Disappear();
			pLayer1BlurRT1->WithdrawFromDevice();
		}
		/*else if (m_pMaskPS)
		{
			m_pMaskPS->SetValue1f("g_fBrightScale", m_fBrightScale);
			m_pMaskPS->Appear();
			if (NULL != m_pSystem->m_pBloomMeshRT)
				m_pSystem->m_pBloomMeshRT->AppearAsTexture(0);
			else
			{
				g_A3DErrLog.Log("A3DPostBloom::Render Failed! Bloom RT is NULL!");
				return false;
			}
		}*/


		if (m_pThresholdPS)
		{
			//降采样到1/4
			pLayer2BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
			m_pSystem->GetCommonVS()->Appear();
			m_pDownSampPS->SetValue2f("g_vPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
			m_pDownSampPS->Appear();
			pLayer1BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pDownSampPS->Disappear();
			pLayer2BlurRT1->WithdrawFromDevice();

			//降采样到1/8
			pLayer3BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 8, m_pSystem->GetHeight() / 8);
			m_pSystem->GetCommonVS()->Appear();
			m_pDownSampPS->SetValue2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
			m_pDownSampPS->Appear();
			pLayer2BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pDownSampPS->Disappear();
			pLayer3BlurRT1->WithdrawFromDevice();
		}

#if _POST_DEBUG_
//  		if(GetAsyncKeyState('1') & 0x8000)
//  		{ 		
//  			D3DXSaveSurfaceToFileA("m_pLayer1BlurRT1.dds", D3DXIFF_DDS, pLayer1BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, pLayer2BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1.dds", D3DXIFF_DDS, pLayer3BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		}
#endif
	}
	//模糊
	{
		//模糊
		if (m_pThresholdPS)
		{
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 8, m_pSystem->GetHeight() / 8);
			m_pSystem->GetCommonVS()->Appear();
			m_pBlurXPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
			//X方向
			pLayer3BlurRT2->ApplyToDevice();
			m_pBlurXPS->Appear();
			pLayer3BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurXPS->Disappear();
			pLayer3BlurRT2->WithdrawFromDevice();

			//Y方向
			pLayer3BlurRT1->ApplyToDevice();
			m_pBlurYPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
			m_pBlurYPS->Appear();
			pLayer3BlurRT2->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurYPS->Disappear();
			pLayer3BlurRT1->WithdrawFromDevice();
		}
		else
		{
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
			m_pSystem->GetCommonVS()->Appear();
			m_pBlurXPS->SetValue2f("g_inputPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
			//X方向
			pLayer1BlurRT2->ApplyToDevice();
			m_pBlurXPS->Appear();
			if (NULL != m_pSystem->m_pBloomMeshRT)
				m_pSystem->m_pBloomMeshRT->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurXPS->Disappear();
			pLayer1BlurRT2->WithdrawFromDevice();

			//Y方向
			pLayer1BlurRT1->ApplyToDevice();
			m_pBlurYPS->SetValue2f("g_inputPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
			m_pBlurYPS->Appear();
			pLayer1BlurRT2->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurYPS->Disappear();
			pLayer1BlurRT1->WithdrawFromDevice();
		}

#if _POST_DEBUG_
// 		if(GetAsyncKeyState('1') & 0x8000)
// 		{ 		
// 			D3DXSaveSurfaceToFileA("pLayer3BlurRT2_1.dds", D3DXIFF_DDS, pLayer3BlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
// 		}
#endif


		/*//X方向
		pLayer3BlurRT2->ApplyToDevice();
		m_pBlurXPS->Appear();
		pLayer3BlurRT1->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurXPS->Disappear();
		pLayer3BlurRT2->WithdrawFromDevice();
		
		//Y方向
		pLayer3BlurRT1->ApplyToDevice();
		m_pBlurYPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
		m_pBlurYPS->Appear();
		pLayer3BlurRT2->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurYPS->Disappear();
		pLayer3BlurRT1->WithdrawFromDevice();*/
	}
#if _POST_DEBUG_
// 	if(GetAsyncKeyState('1') & 0x8000)
// 	{ 		
// 		D3DXSaveSurfaceToFileA("pLayer3BlurRT1_2.dds", D3DXIFF_DDS, pLayer3BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
// 	}
#endif

	if (m_pThresholdPS)
	{
		//上采样, BlurAdd
		pLayer2BlurRT2->ApplyToDevice();
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
		m_pSystem->GetCommonVS()->Appear();
		m_pAddBlurPS->SetValue2f("samp0PixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
		m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
		m_pAddBlurPS->SetValue1f("fLastPass", 0.f);
		m_pAddBlurPS->Appear();
		pLayer2BlurRT1->AppearAsTexture(0);
		pLayer3BlurRT1->AppearAsTexture(1);
		m_pSystem->RenderQuad();
		m_pAddBlurPS->Disappear();
		pLayer2BlurRT2->WithdrawFromDevice();

		pLayer1BlurRT2->ApplyToDevice();
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
		m_pSystem->GetCommonVS()->Appear();
		m_pAddBlurPS->SetValue2f("samp0PixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
		m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
		m_pAddBlurPS->SetValue1f("fLastPass", 0.f);
		m_pAddBlurPS->Appear();
		pLayer1BlurRT1->AppearAsTexture(0);
		pLayer2BlurRT2->AppearAsTexture(1);
		m_pSystem->RenderQuad();
		m_pAddBlurPS->Disappear();
		pLayer1BlurRT2->WithdrawFromDevice();
	}

#if _POST_DEBUG_
// 	if(GetAsyncKeyState('1') & 0x8000)
// 	{ 		
// 		D3DXSaveSurfaceToFileA("pLayer1BlurRT2_2.dds", D3DXIFF_DDS, pLayer1BlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
// 	}
#endif

	pDst->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();
	m_pAddBlurPS->SetValue2f("samp0PixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
	m_pAddBlurPS->SetValue1f("fLastPass", 1.f);
	m_pAddBlurPS->Appear();
	pSrc->AppearAsTexture(0);
	if (m_pThresholdPS)
	{
		pLayer1BlurRT2->AppearAsTexture(1);
	}
	else
	{
		pLayer1BlurRT1->AppearAsTexture(1);
	}
	m_pSystem->RenderQuad();
	m_pAddBlurPS->Disappear();
	pDst->WithdrawFromDevice();

	//重置shader和rt
	pDev->ClearVertexShader();
	pDev->ClearPixelShader();

	m_pSystem->SwapSrcDstRT();

	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer1BlurRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer1BlurRT2);

	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer2BlurRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer2BlurRT2);

	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer3BlurRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer3BlurRT2);

	return true;
}

bool A3DPostBloom::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

void A3DPostBloom::SetBlurSize( float fBlurSize )
{
	fBlurSize = max(0.f, fBlurSize);
	fBlurSize = min(1.f, fBlurSize);
	m_fBlurSize = fBlurSize;
}

void A3DPostBloom::SetBrightThreshold( float fThreshold )
{
	fThreshold = max(0.f, fThreshold);
	fThreshold = min(1.f, fThreshold);
	m_fBrightThreshold = fThreshold;
}

void A3DPostBloom::SetBrightScale( float fScale )
{
	fScale = max(0.f, fScale);
	fScale = min(10.f, fScale);
	m_fBrightScale = fScale;
}

A3DPOST_EFFECT A3DPostBloom::GetEffectType() const
{
	if (m_pThresholdPS)
		return A3DPOST_EFFECT_Bloom;
	return A3DPOST_EFFECT_BloomLocal;
}

A3DRenderTarget* A3DPostBloom::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    A3DFORMAT fmt = A3DFMT_A8R8G8B8;

    int rtWidth = pViewport->GetParam()->Width;
    int rtHeight = pViewport->GetParam()->Height;
    int rtWidth_2 = rtWidth / 2;
    int rtHeight_2 = rtHeight / 2;
    int rtWidth_4 = rtWidth / 4;
    int rtHeight_4 = rtHeight / 4;
    int rtWidth_8 = rtWidth / 8;
    int rtHeight_8 = rtHeight / 8;

    A3DRenderTarget*	pLayer1BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmt);
    A3DRenderTarget*	pLayer1BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmt);

    A3DRenderTarget*	pLayer2BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmt);
    A3DRenderTarget*	pLayer2BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmt);

    A3DRenderTarget*	pLayer3BlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_8, rtHeight_8, fmt);
    A3DRenderTarget*	pLayer3BlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_8, rtHeight_8, fmt);

    //m_fBlurSize = 1.f;//m_fBlurSize;

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);
    // 	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    // 	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    for(int i = 0; i <=3; i++)
    {
        pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
        pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    }

    //提取阈值 (同时降采样)
    {
		if (m_pThresholdPS)
		{
			pLayer1BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
			m_pSystem->GetCommonVS()->Appear();

			m_pThresholdPS->SetValue1f("g_fThreshold", m_fBrightThreshold);
			m_pThresholdPS->SetValue1f("g_fBrightScale", m_fBrightScale);
			m_pThresholdPS->SetValue2f("g_vPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
			m_pThresholdPS->Appear();
			pSrcRT->AppearAsTexture(0);

			m_pSystem->RenderQuad();

			m_pThresholdPS->Disappear();
			pLayer1BlurRT1->WithdrawFromDevice();
		}
		/*else if (m_pMaskPS)
		{
			m_pMaskPS->SetValue1f("g_fBrightScale", m_fBrightScale);
			m_pMaskPS->Appear();
			if (NULL != m_pSystem->m_pBloomMeshRT)
				m_pSystem->m_pBloomMeshRT->AppearAsTexture(0);
			else
			{
				g_A3DErrLog.Log("A3DPostBloom::ApplyEffect Failed! Bloom RT is NULL!");
				return false;
			}
		}*/


		if (m_pThresholdPS)
		{
			//降采样到1/4
			pLayer2BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
			m_pSystem->GetCommonVS()->Appear();
			m_pDownSampPS->SetValue2f("g_vPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
			m_pDownSampPS->Appear();
			pLayer1BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pDownSampPS->Disappear();
			pLayer2BlurRT1->WithdrawFromDevice();

			//降采样到1/8
			pLayer3BlurRT1->ApplyToDevice();
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 8, m_pSystem->GetHeight() / 8);
			m_pSystem->GetCommonVS()->Appear();
			m_pDownSampPS->SetValue2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
			m_pDownSampPS->Appear();
			pLayer2BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pDownSampPS->Disappear();
			pLayer3BlurRT1->WithdrawFromDevice();
		}

#if _POST_DEBUG_
        //  		if(GetAsyncKeyState('1') & 0x8000)
        //  		{ 		
        //  			D3DXSaveSurfaceToFileA("m_pLayer1BlurRT1.dds", D3DXIFF_DDS, pLayer1BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
        //  			D3DXSaveSurfaceToFileA("m_pLayer2BlurRT1.dds", D3DXIFF_DDS, pLayer2BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
        //  			D3DXSaveSurfaceToFileA("m_pLayer3BlurRT1.dds", D3DXIFF_DDS, pLayer3BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
        //  		}
#endif
    }
    //模糊
    {
        //模糊
		if (m_pThresholdPS)
		{
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 8, m_pSystem->GetHeight() / 8);
			m_pSystem->GetCommonVS()->Appear();
			m_pBlurXPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
			//X方向
			pLayer3BlurRT2->ApplyToDevice();
			m_pBlurXPS->Appear();
			pLayer3BlurRT1->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurXPS->Disappear();
			pLayer3BlurRT2->WithdrawFromDevice();

			//Y方向
			pLayer3BlurRT1->ApplyToDevice();
			m_pBlurYPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
			m_pBlurYPS->Appear();
			pLayer3BlurRT2->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurYPS->Disappear();
			pLayer3BlurRT1->WithdrawFromDevice();
		}
		else
		{
			m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
			m_pSystem->GetCommonVS()->Appear();
			m_pBlurXPS->SetValue2f("g_inputPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
			//X方向
			pLayer1BlurRT2->ApplyToDevice();
			m_pBlurXPS->Appear();
			if (NULL != m_pSystem->m_pBloomMeshRT)
				m_pSystem->m_pBloomMeshRT->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurXPS->Disappear();
			pLayer1BlurRT2->WithdrawFromDevice();

			//Y方向
			pLayer1BlurRT1->ApplyToDevice();
			m_pBlurYPS->SetValue2f("g_inputPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
			m_pBlurYPS->Appear();
			pLayer1BlurRT2->AppearAsTexture(0);
			m_pSystem->RenderQuad();
			m_pBlurYPS->Disappear();
			pLayer1BlurRT1->WithdrawFromDevice();
		}

#if _POST_DEBUG_
        // 		if(GetAsyncKeyState('1') & 0x8000)
        // 		{ 		
        // 			D3DXSaveSurfaceToFileA("pLayer3BlurRT2_1.dds", D3DXIFF_DDS, pLayer3BlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
        // 		}
#endif


        /*//X方向
        pLayer3BlurRT2->ApplyToDevice();
        m_pBlurXPS->Appear();
        pLayer3BlurRT1->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurXPS->Disappear();
        pLayer3BlurRT2->WithdrawFromDevice();

        //Y方向
        pLayer3BlurRT1->ApplyToDevice();
        m_pBlurYPS->SetValue2f("g_inputPixelSize", 8.f / (float)m_pSystem->GetWidth(), 8.f / (float)m_pSystem->GetHeight());
        m_pBlurYPS->Appear();
        pLayer3BlurRT2->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurYPS->Disappear();
        pLayer3BlurRT1->WithdrawFromDevice();*/
    }
#if _POST_DEBUG_
    // 	if(GetAsyncKeyState('1') & 0x8000)
    // 	{ 		
    // 		D3DXSaveSurfaceToFileA("pLayer3BlurRT1_2.dds", D3DXIFF_DDS, pLayer3BlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    // 	}
#endif

	if (m_pThresholdPS)
	{
		//上采样, BlurAdd
		pLayer2BlurRT2->ApplyToDevice();
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
		m_pSystem->GetCommonVS()->Appear();
		m_pAddBlurPS->SetValue2f("samp0PixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
		m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
		m_pAddBlurPS->SetValue1f("fLastPass", 0.f);
		m_pAddBlurPS->Appear();
		pLayer2BlurRT1->AppearAsTexture(0);
		pLayer3BlurRT1->AppearAsTexture(1);
		m_pSystem->RenderQuad();
		m_pAddBlurPS->Disappear();
		pLayer2BlurRT2->WithdrawFromDevice();

		pLayer1BlurRT2->ApplyToDevice();
		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
		m_pSystem->GetCommonVS()->Appear();
		m_pAddBlurPS->SetValue2f("samp0PixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
		m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
		m_pAddBlurPS->SetValue1f("fLastPass", 0.f);
		m_pAddBlurPS->Appear();
		pLayer1BlurRT1->AppearAsTexture(0);
		pLayer2BlurRT2->AppearAsTexture(1);
		m_pSystem->RenderQuad();
		m_pAddBlurPS->Disappear();
		pLayer1BlurRT2->WithdrawFromDevice();
	}

#if _POST_DEBUG_
    // 	if(GetAsyncKeyState('1') & 0x8000)
    // 	{ 		
    // 		D3DXSaveSurfaceToFileA("pLayer1BlurRT2_2.dds", D3DXIFF_DDS, pLayer1BlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    // 	}
#endif

    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth, rtHeight, A3DFMT_A8R8G8B8);
    pDst->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();
    m_pAddBlurPS->SetValue2f("samp0PixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
    m_pAddBlurPS->SetValue1f("fBlurSize", m_fBlurSize);
    m_pAddBlurPS->SetValue1f("fLastPass", 1.f);
    m_pAddBlurPS->Appear();
    pSrcRT->AppearAsTexture(0);
	if (m_pThresholdPS)
	{
		pLayer1BlurRT2->AppearAsTexture(1);
	}
	else
	{
		pLayer1BlurRT1->AppearAsTexture(1);
	}
    m_pSystem->RenderQuad();
    m_pAddBlurPS->Disappear();
    pDst->WithdrawFromDevice();

    //重置shader和rt
    pDev->ClearVertexShader();
    pDev->ClearPixelShader();


    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer1BlurRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer1BlurRT2);

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer2BlurRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer2BlurRT2);

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer3BlurRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pLayer3BlurRT2);

    return pDst;
}
