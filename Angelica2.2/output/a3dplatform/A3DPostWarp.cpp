/*
 * FILE: A3DPostWarp.cpp
 *
 * DESCRIPTION: 扭曲后处理
 *
 * CREATED BY: Panyupin, 2012/3/6
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostWarp.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_Warp_vs_hlsl = "shaders\\HLSL\\PostProcess\\Post_Warp_vs.hlsl";
static const char* szPost_Warp_ps_hlsl = "shaders\\HLSL\\PostProcess\\Post_Warp_ps.hlsl";
static const char* szMaskTexture = "Shaders\\textures\\WarpMask.tga";
#else
#include "A3DHLSL.h"
static const char* szPost_Warp_vs_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Warp_vs.hlsl";
static const char* szPost_Warp_ps_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Warp_ps.hlsl";
static const char* szMaskTexture = "shaders\\Textures\\WarpMask.tga";
#endif

extern ALog g_A3DErrLog;

bool A3DPostWarp::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	m_pHLSL = CreateA3DHLSL_Common(szPost_Warp_vs_hlsl, "vs_main", szPost_Warp_ps_hlsl, "ps_main",0, NULL,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSL)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! Post_Warp_ps.hlsl load failed!");
		Release();
		return false;
	}
		
	const char* szMacro[1];
	szMacro[0] = "_MASK_";

	m_pHLSL_Mask = CreateA3DHLSL_Common(szPost_Warp_vs_hlsl, "vs_main", szPost_Warp_ps_hlsl, "ps_main", 1, szMacro,
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if(!m_pHLSL_Mask)
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Failed! Post_Warp_ps.hlsl load failed!");
		Release();
		return false;
	}

	m_pWarpBumpTexture = new A3DTexture;
	if( !m_pWarpBumpTexture->Create(m_pSystem->GetA3DDevice(), 256, 256, A3DFMT_A8R8G8B8, 1) )
	{
		Release();
		return false;
	}
	BYTE *	pBits;
	int		iPitch;
	if( !m_pWarpBumpTexture->LockRect(NULL, (void **)&pBits, &iPitch, 0) )
	{
		Release();
		return false;
	}
	DWORD * pDst = (DWORD*)pBits;

	for(int y = 0; y < 256; y++)
	{
		pDst = (DWORD *)(pBits + y * iPitch);
		for(int x = 0; x < 256; x++)
		{
			FLOAT fx = x * (1.0f / 256) - 0.5f;
			FLOAT fy = y * (1.0f / 256) - 0.5f;
			FLOAT r  = sqrtf(fx * fx + fy * fy);

			int iDu = (int)(32 * cosf(4.0f * (fx + fy) * A3D_PI)) + 128;
			int iDv = (int)(32 * sinf(4.0f * (fx + fy) * A3D_PI)) + 128;

			pDst[x] = A3DCOLORRGBA(iDu, iDv, 0, 0xff);
		}
	}

	m_pWarpBumpTexture->UnlockRect();

	if(!m_pSystem->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szMaskTexture, &m_pWarpMaskTexture))
	{
		g_A3DErrLog.Log("A3DPostRadialBlur::Init Can not Load m_pWarpMaskTexture!");
		m_pWarpMaskTexture = NULL;
	}
	return true;
}

void A3DPostWarp::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_Mask);
	A3DRELEASE(m_pWarpBumpTexture);
}

bool A3DPostWarp::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) || !pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	return true;
}

bool A3DPostWarp::Render( A3DViewport* pViewport )
{
	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	ASSERT(pDev && pSrc && pDst);

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_WRAP);

	pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_WRAP);

	//设置RnederTarget
	pDst->ApplyToDevice();

	A3DHLSL* pHLSL;
	if(m_bEnableMask && m_pWarpMaskTexture)
		pHLSL = m_pHLSL_Mask;
	else
		pHLSL = m_pHLSL;

	//设置Shader参数
	pHLSL->Setup2f("g_vPixelSize", 1.f / (float)m_pSystem->GetWidth(), 1.f / (float)m_pSystem->GetHeight());
	float fCurTime = (float)a_GetTime() * 0.001f;
	pHLSL->Setup1f("g_fTime", fCurTime);
	pHLSL->Setup1f("g_fWarpSpeed", m_fSpeed);
	pHLSL->Setup1f("g_fWarpScale", m_fScale);
	
	pHLSL->Appear();	
	
	pSrc->AppearAsTexture(0);
	m_pWarpBumpTexture->Appear(1);
	if(m_bEnableMask && m_pWarpMaskTexture)
		m_pWarpMaskTexture->Appear(2);
	//绘制方块
	m_pSystem->RenderQuad();
	
	pDst->WithdrawFromDevice();

	pHLSL->Disappear();

	//交换RT
	m_pSystem->SwapSrcDstRT();

	pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	return true;

}

A3DPostWarp::~A3DPostWarp()
{

}

A3DPOST_EFFECT A3DPostWarp::GetEffectType() const
{
	return A3DPOST_EFFECT_Warp;
}

void A3DPostWarp::SetMaskTexture( const AString& str )
{
	if(str != m_strMaskTexture)
	{
		m_strMaskTexture = str;
		m_pSystem->GetA3DEngine()->GetA3DTextureMan()->ReleaseTexture(m_pWarpMaskTexture);
		m_pWarpMaskTexture = NULL;
		const char* sz = str;
		if(str == "")
			sz = szMaskTexture;
		m_pSystem->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(sz, &m_pWarpMaskTexture);
	}

}

A3DRenderTarget* A3DPostWarp::ApplyEffect( A3DViewport* pViewport, A3DRenderTarget* pSrcRT )
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_WRAP);

    pDev->SetTextureFilterType(2, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_WRAP);

    //设置RnederTarget
    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);

    pDst->ApplyToDevice();

    A3DHLSL* pHLSL;
    if(m_bEnableMask && m_pWarpMaskTexture)
        pHLSL = m_pHLSL_Mask;
    else
        pHLSL = m_pHLSL;

    //设置Shader参数
    pHLSL->Setup2f("g_vPixelSize", 1.f / (float)m_pSystem->GetWidth(), 1.f / (float)m_pSystem->GetHeight());
    float fCurTime = (float)a_GetTime() * 0.001f;
    pHLSL->Setup1f("g_fTime", fCurTime);
    pHLSL->Setup1f("g_fWarpSpeed", m_fSpeed);
    pHLSL->Setup1f("g_fWarpScale", m_fScale);

    pHLSL->Appear();	

    pSrcRT->AppearAsTexture(0);
    m_pWarpBumpTexture->Appear(1);
    if(m_bEnableMask && m_pWarpMaskTexture)
        m_pWarpMaskTexture->Appear(2);
    //绘制方块
    m_pSystem->RenderQuad();

    pDst->WithdrawFromDevice();

    pHLSL->Disappear();

    pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

    return pDst;

}
