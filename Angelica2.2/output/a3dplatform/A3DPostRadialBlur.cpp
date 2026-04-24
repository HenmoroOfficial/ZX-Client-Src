/*
 * FILE: A3DPostRadialBlur.cpp
 *
 * DESCRIPTION: 径向模糊后处理
 *
 * CREATED BY: Panyupin, 2012/2/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostRadialBlur.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_RadialBlur_hlsl = "shaders\\HLSL\\PostProcess\\Post_RadialBlur.hlsl";
static const char* szPost_RadialBlurDownsamp_ps_hlsl = "shaders\\HLSL\\PostProcess\\post_RadialBlurDownsamp_ps.hlsl";
static const char* szPost_RadialBlurMerge_hlsl = "shaders\\HLSL\\PostProcess\\Post_RadialBlurMerge.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_RadialBlur_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_RadialBlur.hlsl";
static const char* szPost_RadialBlurDownsamp_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\post_RadialBlurDownsamp_ps.hlsl";
static const char* szPost_RadialBlurMerge_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_RadialBlurMerge.hlsl";
#endif


extern ALog g_A3DErrLog;

A3DPostRadialBlur::~A3DPostRadialBlur()
{
}

bool A3DPostRadialBlur::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	m_pHLSLRB = CreateA3DHLSL_Common(NULL, NULL, szPost_RadialBlur_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSLRB)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! Post_RadialBlur.hlsl load failed!");
		Release();
		return false;
	}



	m_pHLSLDownSamp = CreateA3DHLSL_Common(NULL, NULL, szPost_RadialBlurDownsamp_ps_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSLDownSamp)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! post_RadialBlurDownsamp_ps.hlsl load failed!");
		Release();
		return false;
	}
		
	m_pHLSLMerge = CreateA3DHLSL_Common(NULL, NULL, szPost_RadialBlurMerge_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSLMerge)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! Post_RadialBlurMerge.hlsl load failed!");
		Release();
		return false;
	}

	return true;
}

void A3DPostRadialBlur::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSLRB);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSLDownSamp);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSLMerge);
}

bool A3DPostRadialBlur::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;

	return true;
}

bool A3DPostRadialBlur::Render( A3DViewport* pViewport )
{
	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	A3DRenderTarget* pHalfRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(-2, -2, A3DFMT_A8R8G8B8);
	A3DRenderTarget* pHalfRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(-2, -2, A3DFMT_A8R8G8B8);

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetAlphaTestEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	//DownSample
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	pHalfRT1->ApplyToDevice();
	pDev->Clear(D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
	m_pHLSLDownSamp->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
	m_pHLSLDownSamp->SetupTextureFromRT("g_Sampler", pSrc);
	m_pHLSLDownSamp->Appear();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pHLSLDownSamp->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pHalfRT1->WithdrawFromDevice();


	m_pHLSLRB->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
	m_pHLSLRB->Setup1f("g_fRbBlurLength", m_fBlurLength);
	m_pHLSLRB->Setup1f("g_fInFocusRadius", m_fInFocusRadius);
	m_pHLSLRB->Setup1f("g_fOutFocusRadius", m_fOutFocusRadius);
	//模糊三遍
	pHalfRT2->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT1);
	m_pHLSLRB->Appear();
	m_pSystem->RenderQuad();
	m_pHLSLRB->Disappear();
	pHalfRT2->WithdrawFromDevice();

	pHalfRT1->ApplyToDevice();
	m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT2);
	m_pHLSLRB->Appear();
	m_pSystem->RenderQuad();
	m_pHLSLRB->Disappear();
	pHalfRT1->WithdrawFromDevice();

	pHalfRT2->ApplyToDevice();
	m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT1);
	m_pHLSLRB->Appear();
	m_pSystem->RenderQuad();
	m_pHLSLRB->Disappear();
	pHalfRT2->WithdrawFromDevice();

	m_pSystem->GetCommonVS()->Disappear();

	//Merge
	pDst->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();

	m_pHLSLMerge->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
	m_pHLSLMerge->Setup1f("g_fRbBlurLength", m_fBlurLength);
	m_pHLSLMerge->Setup1f("g_fInFocusRadius", m_fInFocusRadius);
	m_pHLSLMerge->Setup1f("g_fOutFocusRadius", m_fOutFocusRadius);
	m_pHLSLMerge->SetupTextureFromRT("backBufferSampler", pSrc);
	m_pHLSLMerge->SetupTextureFromRT("bluredSampler", pHalfRT2);
	m_pHLSLMerge->Appear();

	m_pSystem->RenderQuad();

	m_pHLSLMerge->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pDst->WithdrawFromDevice();


#ifdef _DEBUG_POST_
//  	if(GetAsyncKeyState(VK_F7) & 0x8000)
//  	{
// 		D3DXSaveSurfaceToFileA("RaidiusBlur_M.dds", D3DXIFF_DDS, pHalfRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("RaidiusBlur_F.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  	}
#endif
	m_pSystem->SwapSrcDstRT();

	pDev->ClearVertexShader();
	pDev->ClearPixelShader();
	
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT2);

	return true;
}

void A3DPostRadialBlur::SetBlurCenterScreenSpace( float x, float y ) /* (0.5, 0.5)是屏?中间, (0,0)是左上角, (1,1)是右下角 */
{
	m_vBlurCenter.x = x;
	m_vBlurCenter.y = y;
}

void A3DPostRadialBlur::GetBlurCenterScreenSpace( float* pX, float* pY )
{
	*pX = m_vBlurCenter.x;
	*pY = m_vBlurCenter.y;
}

void A3DPostRadialBlur::SetBlurLength( float fBlurLength )
{
	fBlurLength = max(0.f, fBlurLength);
	//fBlurLength = min(1.f, fBlurLength);
	m_fBlurLength = fBlurLength;
}

float A3DPostRadialBlur::GetBlurLength()
{
	return m_fBlurLength;
}

void A3DPostRadialBlur::SetFocusRadius( float fInFocusRadius, float fOutFocusRadius )
{
	if(fInFocusRadius <= fOutFocusRadius)
	{
		m_fInFocusRadius = fInFocusRadius;
		m_fOutFocusRadius = fOutFocusRadius;
	}
	else
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::SetFocusRadius error!");
	}
}

void A3DPostRadialBlur::GetFocusRadius( float* pfInFocusRadius, float* pfOutFocusRadius )
{
	*pfInFocusRadius = m_fInFocusRadius;
	*pfOutFocusRadius = m_fOutFocusRadius;
}

void A3DPostRadialBlur::SetBlurCenterWorldSpace( const A3DVECTOR3& vWorldPos, A3DViewport* pViewPort )
{
	const A3DMATRIX4& matVP = pViewPort->GetCamera()->GetVPTM();
	vWorldPos * matVP;
	A3DVECTOR4 lOut = vWorldPos * matVP;
	lOut.x = lOut.x / lOut.w;
	lOut.y = lOut.y / lOut.w;
	lOut.x = (lOut.x + 1.f) / 2.f;
	lOut.y = (1.f - lOut.y) / 2.f;
	m_vBlurCenter.x = lOut.x;
	m_vBlurCenter.y = lOut.y;
}

A3DPOST_EFFECT A3DPostRadialBlur::GetEffectType() const
{
	return A3DPOST_EFFECT_RadialBlur;
}

A3DRenderTarget* A3DPostRadialBlur::ApplyEffect( A3DViewport* pViewport, A3DRenderTarget* pSrcRT )
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    int rtWidth		= pViewport->GetParam()->Width;
    int rtHeight	= pViewport->GetParam()->Height;
    int rtWidth_2	= rtWidth / 2;
    int rtHeight_2	= rtHeight / 2;

    A3DRenderTarget* pHalfRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, A3DFMT_A8R8G8B8);
    A3DRenderTarget* pHalfRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, A3DFMT_A8R8G8B8);


    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetAlphaTestEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    //DownSample
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    pHalfRT1->ApplyToDevice();
    pDev->Clear(D3DCLEAR_TARGET, 0xffffffff, 1.0f, 0);
    m_pHLSLDownSamp->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
    m_pHLSLDownSamp->SetupTextureFromRT("g_Sampler", pSrcRT);
    m_pHLSLDownSamp->Appear();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();
    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    m_pHLSLDownSamp->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    pHalfRT1->WithdrawFromDevice();


    m_pHLSLRB->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
    m_pHLSLRB->Setup1f("g_fRbBlurLength", m_fBlurLength);
    m_pHLSLRB->Setup1f("g_fInFocusRadius", m_fInFocusRadius);
    m_pHLSLRB->Setup1f("g_fOutFocusRadius", m_fOutFocusRadius);
    //模糊三遍
    pHalfRT2->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT1);
    m_pHLSLRB->Appear();
    m_pSystem->RenderQuad();
    m_pHLSLRB->Disappear();
    pHalfRT2->WithdrawFromDevice();

    pHalfRT1->ApplyToDevice();
    m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT2);
    m_pHLSLRB->Appear();
    m_pSystem->RenderQuad();
    m_pHLSLRB->Disappear();
    pHalfRT1->WithdrawFromDevice();

    pHalfRT2->ApplyToDevice();
    m_pHLSLRB->SetupTextureFromRT("backBufferSampler", pHalfRT1);
    m_pHLSLRB->Appear();
    m_pSystem->RenderQuad();
    m_pHLSLRB->Disappear();
    pHalfRT2->WithdrawFromDevice();

    m_pSystem->GetCommonVS()->Disappear();

    //Merge
    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);
    pDst->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();

    m_pHLSLMerge->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
    m_pHLSLMerge->Setup1f("g_fRbBlurLength", m_fBlurLength);
    m_pHLSLMerge->Setup1f("g_fInFocusRadius", m_fInFocusRadius);
    m_pHLSLMerge->Setup1f("g_fOutFocusRadius", m_fOutFocusRadius);
    m_pHLSLMerge->SetupTextureFromRT("backBufferSampler", pSrcRT);
    m_pHLSLMerge->SetupTextureFromRT("bluredSampler", pHalfRT2);
    m_pHLSLMerge->Appear();

    m_pSystem->RenderQuad();

    m_pHLSLMerge->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    pDst->WithdrawFromDevice();


#ifdef _DEBUG_POST_
    //  	if(GetAsyncKeyState(VK_F7) & 0x8000)
    //  	{
    // 		D3DXSaveSurfaceToFileA("RaidiusBlur_M.dds", D3DXIFF_DDS, pHalfRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //  		D3DXSaveSurfaceToFileA("RaidiusBlur_F.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //  	}
#endif

    pDev->ClearVertexShader();
    pDev->ClearPixelShader();

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pHalfRT2);

    return pDst;
}
