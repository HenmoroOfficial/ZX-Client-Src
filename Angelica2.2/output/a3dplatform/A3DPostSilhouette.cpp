/*
 * FILE: A3DPostSilhouette.h
 *
 * DESCRIPTION: 勾边后处理
 *
 * CREATED BY: Panyupin, 2013/1/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */

#include "A3DPostSilhouette.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_Edge_ps_hlsl = "shaders\\HLSL\\PostProcess\\Post_Warp_vs.hlsl";
static const char* szPost_Blur_ps_hlsl = "shaders\\HLSL\\PostProcess\\Post_Warp_ps.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_Edge_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Silhouette_Edge.hlsl";
static const char* szPost_Blur_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Silhouette_Blur.hlsl";
#endif

extern ALog g_A3DErrLog;

bool A3DPostSilhouette::Init(A3DPostSystem* pSystem)
{
	m_pSystem = pSystem;

	m_pHLSL_Edge = CreateA3DHLSL_Common(NULL, NULL, szPost_Edge_ps_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSL_Edge)
	{
		g_A3DErrLog.Log("A3DPostSilhouette::Init Failed! Post_Silhouette_Edge.hlsl load failed!");
		Release();
		return false;
	}

	m_pHLSL_Blur = CreateA3DHLSL_Common(NULL, NULL, szPost_Blur_ps_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSL_Blur)
	{
		g_A3DErrLog.Log("A3DPostSilhouette::Init Failed! Post_Silhouette_Blur.hlsl load failed!");
		Release();
		return false;
	}
	A3DFORMAT depthRTFMT = m_pSystem->GetSmallestFloatRFmt();
	if(depthRTFMT == A3DFMT_UNKNOWN)
		depthRTFMT = A3DFMT_A8R8G8B8;
	m_pPlayerDepthRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(0, 0, depthRTFMT);
	if(!m_pPlayerDepthRT)
	{
		g_A3DErrLog.Log("A3DPostSilhouette::Init Failed! Can not Create depth RT!");
		Release();
		return false;
	}

    m_pBlackTexture = A3DTexture::CreateColorTexture(m_pSystem->GetA3DDevice(), 8, 8, 0x00000000);
	ResetParam();

	return true;
}

void A3DPostSilhouette::ResetParam()
{
	m_fHardness = 0.8f;
	m_fBlurDist = 0.3f;	//调这个值改变勾边宽度
}

void A3DPostSilhouette::Release()
{
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pPlayerDepthRT);

	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_Edge);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_Blur);

    A3DRELEASE(m_pBlackTexture);
}

bool A3DPostSilhouette::Render(A3DViewport* pViewport)
{
    return DoRender(pViewport, true, m_pPlayerDepthRT, m_pSystem->GetSrcRT(), A3DVECTOR4(1, 0, 0, 0));
//	A3DRenderTarget* pEdgeRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(0, 0, m_pSystem->GetBackBufferFmt());
//	if(!pEdgeRT)
//		return false;
//	
//	if(!m_pSystem->GetLinearDepthRT())
//		return false;
//
///*	if(GetAsyncKeyState(VK_F5) & 0x8000)
//		ResetParam();
//
//	if(GetAsyncKeyState(VK_F1) & 0x8000)
//	{
//		m_fHardness -= 0.03f;
//	}
//	if(GetAsyncKeyState(VK_F2) & 0x8000)
//	{
//		m_fHardness += 0.03f;
//	}
//
//	if(GetAsyncKeyState(VK_F3) & 0x8000)
//	{
//		m_fBlurDist -=0.00001f;
//	}
//	if(GetAsyncKeyState(VK_F4) & 0x8000)
//	{
//		m_fBlurDist +=0.00001f;
//	}
//*/
//
//	A3DDevice* pDev = m_pSystem->GetA3DDevice();
//	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
//	A3DRenderTarget* pDst = m_pSystem->GetDstRT();
//
//	pDev->SetAlphaBlendEnable(false);
//	pDev->SetZTestEnable(false);
//	pDev->SetZWriteEnable(false);
//	pDev->SetFaceCull(A3DCULL_NONE);
//
//	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
//	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
//	
//	//渲染边界
//	pEdgeRT->ApplyToDevice();
//
//	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
//	m_pSystem->GetCommonVS()->Appear();
//	m_pHLSL_Edge->Setup2f("off", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
//	m_pHLSL_Edge->Appear();
//	
//	//m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
//	m_pPlayerDepthRT->AppearAsTexture(0);
//	
//	m_pSystem->RenderQuad();
//	m_pHLSL_Edge->Disappear();
//	m_pSystem->GetCommonVS()->Disappear();
//
//	pEdgeRT->WithdrawFromDevice();
//
//	//模糊
//	pDev->SetAlphaBlendEnable(true);
//	pDev->SetSourceAlpha(A3DBLEND_SRCALPHA);
//	pDev->SetDestAlpha(A3DBLEND_INVSRCALPHA);
//	
//	pSrc->ApplyToDevice();
//	
//	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
//	m_pSystem->GetCommonVS()->Appear();
//	m_pHLSL_Blur->Setup1f("zFar", pViewport->GetCamera()->GetZBack());
//	m_pHLSL_Blur->Setup1f("hardness", m_fHardness);
//	m_pHLSL_Blur->Setup1f("AA_SampleDist", m_fBlurDist / 1000.f);
//	m_pHLSL_Blur->SetupVec3("lineColor", m_vLineColor);
//
//	m_pHLSL_Blur->Appear();
//
//	pEdgeRT->AppearAsTexture(0);
//	m_pSystem->GetLinearDepthRT()->AppearAsTexture(1);
//	m_pSystem->RenderQuad();
//	m_pHLSL_Blur->Disappear();
//	m_pSystem->GetCommonVS()->Disappear();
//	pSrc->WithdrawFromDevice();
//
//	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pEdgeRT);
//
//	return true;
}

bool A3DPostSilhouette::DoRender(A3DViewport* pViewport, 
                                 bool bUseSceneDepth,
                                 A3DRenderTarget* pSrcTexture,
                                 A3DRenderTarget* pDstTexture,
                                 const A3DVECTOR4& vMask
                                 )
{
    int iWidth = pDstTexture->GetWidth();
    int iHeight = pDstTexture->GetHeight();
	A3DRenderTarget* pEdgeRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(
        iWidth, iHeight, A3DFMT_A8R8G8B8);
	if(!pEdgeRT)
		return false;
	
	if(bUseSceneDepth && !m_pSystem->GetLinearDepthRT())
		return false;

/*	if(GetAsyncKeyState(VK_F5) & 0x8000)
		ResetParam();

	if(GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_fHardness -= 0.03f;
	}
	if(GetAsyncKeyState(VK_F2) & 0x8000)
	{
		m_fHardness += 0.03f;
	}

	if(GetAsyncKeyState(VK_F3) & 0x8000)
	{
		m_fBlurDist -=0.00001f;
	}
	if(GetAsyncKeyState(VK_F4) & 0x8000)
	{
		m_fBlurDist +=0.00001f;
	}
*/

	A3DDevice* pDev = m_pSystem->GetA3DDevice();

	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
	
	//渲染边界
	pEdgeRT->ApplyToDevice();

	m_pSystem->SetupCommonVS(iWidth, iHeight);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_Edge->Setup2f("off", 1.f / iWidth, 1.f / iHeight);
    m_pHLSL_Edge->SetupVec4("mask", vMask);
	m_pHLSL_Edge->Appear();
	
	//m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
	pSrcTexture->AppearAsTexture(0);
	
	m_pSystem->RenderQuad();
	m_pHLSL_Edge->Disappear();
	m_pSystem->GetCommonVS()->Disappear();

	pEdgeRT->WithdrawFromDevice();

//    if (!bUseSceneDepth)
//        D3DXSaveSurfaceToFileA("edge.dds", D3DXIFF_DDS, pEdgeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);

	//模糊
	pDev->SetAlphaBlendEnable(true);
	pDev->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pDev->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	
	pDstTexture->ApplyToDevice();
	
	m_pSystem->SetupCommonVS(iWidth, iHeight);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_Blur->Setup1f("zFar", pViewport->GetCamera()->GetZBack());
	m_pHLSL_Blur->Setup1f("hardness", m_fHardness);
	m_pHLSL_Blur->Setup1f("AA_SampleDist", m_fBlurDist / 1000.f);
	m_pHLSL_Blur->SetupVec3("lineColor", m_vLineColor);

	m_pHLSL_Blur->Appear();

	pEdgeRT->AppearAsTexture(0);
	if (bUseSceneDepth)
        m_pSystem->GetLinearDepthRT()->AppearAsTexture(1);
    else if (m_pBlackTexture)
        m_pBlackTexture->Appear(1);
	m_pSystem->RenderQuad();
	m_pHLSL_Blur->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pDstTexture->WithdrawFromDevice();

	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pEdgeRT);

	return true;
}

A3DPOST_EFFECT A3DPostSilhouette::GetEffectType() const
{
	return A3DPOST_EFFECT_Silhouette;
}

bool A3DPostSilhouette::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

A3DPostSilhouette::~A3DPostSilhouette()
{

}

float A3DPostSilhouette::GetHardness() const
{
	return m_fHardness;
}

void A3DPostSilhouette::SetHardness(float fHardness)
{
	m_fHardness = fHardness;
	m_fHardness = max(0.f, m_fHardness);
	m_fHardness = min(2.f, m_fHardness);
}

float A3DPostSilhouette::GetBlurDist() const
{
	return m_fBlurDist;
}

void A3DPostSilhouette::SetBlurDist(float fBlurDist)
{
	m_fBlurDist = fBlurDist;
	m_fBlurDist = max(0.f, m_fBlurDist);
	m_fBlurDist = min(2.f, m_fBlurDist);
}

const A3DVECTOR3& A3DPostSilhouette::GetLineColor() const
{
	return m_vLineColor;
}

void A3DPostSilhouette::SetLineColor(const A3DVECTOR3& vLineColor)
{
	m_vLineColor = vLineColor;
}

bool A3DPostSilhouette::RenderForRT(A3DViewport* pViewport, A3DRenderTarget* pSrcRT, A3DRenderTarget* pDstRT, const A3DVECTOR4& vMask)
{
    return DoRender(pViewport, false, pSrcRT, pDstRT, vMask);
}

A3DRenderTarget* A3DPostSilhouette::ApplyEffect( A3DViewport* pViewport, A3DRenderTarget* pSrcRT )
{
    int iWidth = pSrcRT->GetWidth();
    int iHeight = pSrcRT->GetHeight();
	A3DRenderTarget* pEdgeRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(
        iWidth, iHeight, A3DFMT_A8R8G8B8);
	if(!pEdgeRT)
		return pSrcRT;
	
	if(!m_pSystem->GetLinearDepthRT())
		return pSrcRT;

    if (!m_pPlayerDepthRT)
        return pSrcRT;

/*	if(GetAsyncKeyState(VK_F5) & 0x8000)
		ResetParam();

	if(GetAsyncKeyState(VK_F1) & 0x8000)
	{
		m_fHardness -= 0.03f;
	}
	if(GetAsyncKeyState(VK_F2) & 0x8000)
	{
		m_fHardness += 0.03f;
	}

	if(GetAsyncKeyState(VK_F3) & 0x8000)
	{
		m_fBlurDist -=0.00001f;
	}
	if(GetAsyncKeyState(VK_F4) & 0x8000)
	{
		m_fBlurDist +=0.00001f;
	}
*/

	A3DDevice* pDev = m_pSystem->GetA3DDevice();

	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
	
	//渲染边界
	pEdgeRT->ApplyToDevice();
    A3DVECTOR4 vMask(1, 0, 0, 0);
	m_pSystem->SetupCommonVS(iWidth, iHeight);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_Edge->Setup2f("off", 1.f / iWidth, 1.f / iHeight);
    m_pHLSL_Edge->SetupVec4("mask", vMask);
	m_pHLSL_Edge->Appear();
	
	//m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
	m_pPlayerDepthRT->AppearAsTexture(0);
	
	m_pSystem->RenderQuad();
	m_pHLSL_Edge->Disappear();
	m_pSystem->GetCommonVS()->Disappear();

	pEdgeRT->WithdrawFromDevice();

//    if (!bUseSceneDepth)
//        D3DXSaveSurfaceToFileA("edge.dds", D3DXIFF_DDS, pEdgeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);

	//模糊
	pDev->SetAlphaBlendEnable(true);
	pDev->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pDev->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	
    //A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);

	pSrcRT->ApplyToDevice();
	
	m_pSystem->SetupCommonVS(iWidth, iHeight);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_Blur->Setup1f("zFar", pViewport->GetCamera()->GetZBack());
	m_pHLSL_Blur->Setup1f("hardness", m_fHardness);
	m_pHLSL_Blur->Setup1f("AA_SampleDist", m_fBlurDist / 1000.f);
	m_pHLSL_Blur->SetupVec3("lineColor", m_vLineColor);

	m_pHLSL_Blur->Appear();

	pEdgeRT->AppearAsTexture(0);
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(1);
	m_pSystem->RenderQuad();
	m_pHLSL_Blur->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pSrcRT->WithdrawFromDevice();

	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pEdgeRT);
    //m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

	return pSrcRT;
}
