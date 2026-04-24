/*
 * FILE: A3DPostGodRay.cpp
 *
 * DESCRIPTION: 天光后处理
 *
 * CREATED BY: Panyupin, 2012/2/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostGodRay.h"
#include "A3DPostSystem.h"


#ifdef _ANGELICA_2_1
#include "A3DSceneRenderConfig.h"
#include "A3DHLSLWrapper.h"
static const char* szPost_GodraySun_hlsl = "shaders\\HLSL\\PostProcess\\Post_GodraySun.hlsl";
static const char* szPost_GodrayBlur_hlsl = "shaders\\HLSL\\PostProcess\\Post_GodrayBlur.hlsl";
static const char* szPost_GodrayMerge_hlsl = "shaders\\HLSL\\PostProcess\\Post_GodrayMerge.hlsl";
static const char* szPost_GodRayDownScale_hlsl = "shaders\\HLSL\\PostProcess\\Post_GodRayDownScale.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_GodraySun_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_GodraySun.hlsl";
static const char* szPost_GodrayBlur_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_GodrayBlur.hlsl";
static const char* szPost_GodrayMerge_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_GodrayMerge.hlsl";
static const char* szPost_GodRayDownScale_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_GodRayDownScale.hlsl";
#endif


extern ALog g_A3DErrLog;

//#define _DEBUG_POST_

A3DPostGodRay::~A3DPostGodRay()
{

}

bool A3DPostGodRay::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;

	return true;
}
bool A3DPostGodRay::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	if(!LoadShader())
	{
		Release();
		return false;
	}
	return true;
}

bool A3DPostGodRay::LoadShader()
{
	m_pSunPS = CreateA3DHLSL_Common(NULL, NULL, szPost_GodraySun_hlsl, "ps_main", 0, NULL, 
								m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pSunPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_GodraySun.hlsl load failed!");
		return false;
	}

	m_pBlurPS = CreateA3DHLSL_Common(NULL, NULL, szPost_GodrayBlur_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pBlurPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_GodrayBlur.hlsl load failed!");
		return false;
	}

	m_pMergePS = CreateA3DHLSL_Common(NULL, NULL, szPost_GodrayMerge_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pMergePS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_GodrayMerge.hlsl load failed!");
		return false;
	}

	m_pDownSampPS = CreateA3DHLSL_Common(NULL, NULL, szPost_GodRayDownScale_hlsl, "ps_main", 0, NULL, 
		m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	if (!m_pDownSampPS)
	{
		g_A3DErrLog.Log("A3DPostBloom::Init Failed! Post_GodRayDownScale.hlsl load failed!");
		return false;
	}

	return true;
}


void A3DPostGodRay::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pSunPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pBlurPS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pMergePS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pDownSampPS);
}

bool A3DPostGodRay::Render( A3DViewport* pViewport )
{	

#ifdef _DEBUG_POST_
   	if(GetAsyncKeyState(VK_F1) & 0x8000)
   	{
		m_pSunPS->Reload();
		m_pBlurPS->Reload();
		m_pMergePS->Reload();
		m_pDownSampPS->Reload();
   	}
#endif
	if(!m_pSystem->IsLinearZReady())
		return false;

	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	int rtWidth		= pViewport->GetParam()->Width;
	int rtHeight	= pViewport->GetParam()->Height;
	int rtWidth_2	= rtWidth / 2;
	int rtHeight_2	= rtHeight / 2;
	int rtWidth_4	= rtWidth / 4;
	int rtHeight_4	= rtHeight / 4;
	int rtWidth_8	= rtWidth / 8;
	int rtHeight_8	= rtHeight / 8;

	A3DFORMAT colorFmt = A3DFMT_A8R8G8B8;//m_pSystem->GetSmallestFloatRGBAFmt();
	if(colorFmt == A3DFMT_UNKNOWN)
		return false;

	A3DRenderTarget*	m_pFullSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth, rtHeight, colorFmt);
	A3DRenderTarget*	m_pHalfSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, colorFmt);
	A3DRenderTarget*	m_pBlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, colorFmt);
	A3DRenderTarget*	m_pBlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, colorFmt);
	
	if(!(m_pFullSizeRT && m_pHalfSizeRT && m_pBlurRT1 && m_pBlurRT2))
	{
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);
		return true;
	}

	A3DVECTOR3 vSunDir = m_vLightDir;
	A3DVECTOR3 vLightDir = -vSunDir;

	vLightDir.Normalize();
	A3DVECTOR4 lOut;
	A3DMATRIX4 mat = pViewport->GetCamera()->GetVPTM();
	float dis = pViewport->GetCamera()->GetZBack();
	A3DVECTOR3 cameraPos = pViewport->GetCamera()->GetPos(); 
	lOut = (cameraPos + vLightDir * dis) * mat;
	lOut.x = lOut.x / lOut.w;
	lOut.y = lOut.y / lOut.w;
	lOut.x = (lOut.x + 1.f) / 2.f;
	lOut.y = (1.f - lOut.y) / 2.f;


	m_pSunPS->Setup2f("g_vRadialBlurCenter", lOut.x, lOut.y);
	m_vBlurCenter = lOut;

	float visibleAngle = m_fVisibleAngle;

	A3DVECTOR3 lookDir = pViewport->GetCamera()->GetDir();
	lookDir.Normalize();
	float angle = fabsf(acosf(DotProduct( lookDir, vLightDir)));
	m_fAngleScale = (visibleAngle - angle) / visibleAngle;
	m_fAngleScale = max2(0.f, m_fAngleScale);
	m_fAngleScale = min2(1.f, m_fAngleScale);

	if(fabsf(lOut.x) > 10.f || fabsf(lOut.y) > 10.f)
		m_fAngleScale = 0.f;

	if(m_fAngleScale < 1e-3)
	{
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
		pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);
		return true;
	}

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

	m_pSystem->GetA3DDevice()->SetZWriteEnable(false);
	m_pSystem->GetA3DDevice()->SetZTestEnable(false);

	m_pFullSizeRT->ApplyToDevice();

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();

	m_pSunPS->Setup1f("m_fOcclusionDepthRange_0_1", m_fOcclusionDepthRange / (float)pViewport->GetCamera()->GetZBack());
	m_pSunPS->Setup1f("m_fBloomThreshold", m_fBloomThreshold);

	m_pSunPS->Appear();	

	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	m_pSystem->GetSrcRT()->AppearAsTexture(0);
	m_pSystem->GetLinearDepthRT()->AppearAsTexture(1);

	m_pSystem->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);

	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pFullSizeRT->WithdrawFromDevice();

	m_pSunPS->Disappear();
	m_pSystem->GetCommonVS()->Disappear();

	//Down Sample
	m_pHalfSizeRT->ApplyToDevice();
	
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	
	m_pDownSampPS->Setup2f("g_srcPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
	m_pDownSampPS->Appear();
	m_pFullSizeRT->AppearAsTexture(0);

	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pDownSampPS->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	m_pHalfSizeRT->WithdrawFromDevice();

	m_pBlurRT1->ApplyToDevice();

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();

	m_pDownSampPS->Setup2f("g_srcPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / (float)m_pSystem->GetHeight());
	m_pDownSampPS->Appear();
	m_pHalfSizeRT->AppearAsTexture(0);

	m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pDownSampPS->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	m_pBlurRT1->WithdrawFromDevice();


#ifdef _POST_DEBUG_
//   	if(GetAsyncKeyState(VK_F7) & 0x8000)
//   	{
// 		D3DXSaveSurfaceToFileA("GodRay m_pFullSizeRT.dds", D3DXIFF_DDS, m_pFullSizeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("GodRay m_pHalfSizeRT.dds", D3DXIFF_DDS, m_pHalfSizeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//   		D3DXSaveSurfaceToFileA("GodRay m_pBlurRT1.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//   	}
#endif

	//模糊三次
	{
		pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
		pDev->SetTextureAddress(0, A3DTADDR_BORDER, A3DTADDR_BORDER);
		pDev->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x00000000);

		m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);//假定是屏幕大小
		m_pSystem->GetCommonVS()->Appear();

		m_pBlurPS->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
		m_pBlurPS->Setup2f("g_vScreenSize", float(m_pSystem->GetWidth()), float(m_pSystem->GetHeight()));
		
		
		//1
		m_pBlurRT2->ApplyToDevice();
		m_pBlurPS->Appear();
		m_pBlurRT1->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurPS->Disappear();
		m_pBlurRT2->WithdrawFromDevice();
		//2
		m_pBlurRT1->ApplyToDevice();
		m_pBlurPS->Appear();
		m_pBlurRT2->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurPS->Disappear();
		m_pBlurRT1->WithdrawFromDevice();

		//3
		m_pBlurRT2->ApplyToDevice();
		m_pBlurPS->Appear();
		m_pBlurRT1->AppearAsTexture(0);
		m_pSystem->RenderQuad();
		m_pBlurPS->Disappear();
		m_pBlurRT2->WithdrawFromDevice();

		m_pBlurPS->Disappear();
		m_pSystem->GetCommonVS()->Disappear();
	}
#ifdef _POST_DEBUG_
//    	if(GetAsyncKeyState(VK_F7) & 0x8000)
//    	{
//    		D3DXSaveSurfaceToFileA("GodRayBlured.dds", D3DXIFF_DDS, m_pBlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//    	}
#endif
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	pDev->SetTextureFilterType(2, A3DTEXF_POINT);
	pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	//设置RnederTarget
	pDst->ApplyToDevice();
	//设置Shader参数
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();
	
	

	m_pMergePS->Setup1f("m_fOcclusionDepthRange_0_1", m_fOcclusionDepthRange / (float)pViewport->GetCamera()->GetZBack());
	m_pMergePS->SetupVec3("m_vBloomTint", m_vBloomTint);
	m_pMergePS->Setup1f("m_fBloomScale", m_fBloomScale);
	m_pMergePS->Setup1f("m_fScreenBlendThreshold", m_fScreenBlendThreshold);
	m_pMergePS->Setup1f("m_fOcclusionDarkness", m_fOcclusionDarkness);
	m_pMergePS->Setup1f("m_fAngleScale", m_fAngleScale);
	m_pMergePS->Appear();
	
	pSrc->AppearAsTexture(0);
	m_pBlurRT2->AppearAsTexture(1);
	m_pSystem->GetLinearDepthRT()->AppearAsTexture(2);

#ifdef _ANGELICA_2_2
	//m_pSystem->GetScreenIlumRT()->AppearAsTexture(3);
#endif

	m_pSystem->RenderQuad();
	
	m_pMergePS->Disappear();
	m_pSystem->GetCommonVS()->Disappear();

	pDst->WithdrawFromDevice();
	m_pSystem->SwapSrcDstRT();

	pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
	pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);

	return true;
}

void A3DPostGodRay::SetVisibleAngle( float fAngle )
{
	fAngle = max(0.f, fAngle);
	fAngle = min(A3D_PI, fAngle);
	m_fVisibleAngle = fAngle;
}

void A3DPostGodRay::SetOcclusionDepthRange( float fRange )
{
	m_fOcclusionDepthRange = max(0.f, fRange);
}

float A3DPostGodRay::GetOcclusionDepthRange()
{
	return m_fOcclusionDepthRange;
}

void A3DPostGodRay::SetBloomScale( float fBloomScale )
{
	m_fBloomScale = max(0.f, fBloomScale);
}

float A3DPostGodRay::GetBloomScale()
{
	return m_fBloomScale;
}

void A3DPostGodRay::SetBloomThreshold( float fBloomThreshold )
{
	m_fBloomThreshold = max(0.f, fBloomThreshold);
	m_fBloomThreshold = min(1.f, m_fBloomThreshold);
}

float A3DPostGodRay::GetBloomThreshold()
{
	return m_fBloomThreshold;
}

void A3DPostGodRay::SetScreenBlendThreshold( float fScreenBlendThreshold )
{
	m_fScreenBlendThreshold = max(0.f, fScreenBlendThreshold);
	m_fScreenBlendThreshold = min(1.f, m_fScreenBlendThreshold);
}

float A3DPostGodRay::GetScreenBlendThreshold()
{
	return m_fScreenBlendThreshold;
}

void A3DPostGodRay::SetBloomTintColor( const A3DVECTOR3& vColor )
{
	m_vBloomTint = vColor;
}

A3DVECTOR3 A3DPostGodRay::GetBloomTintColor()
{
	return m_vBloomTint;
}

void A3DPostGodRay::SetOcclusionDarkness( float fOcclusionDarkness )
{
	m_fOcclusionDarkness = min(1.f, fOcclusionDarkness);
	m_fOcclusionDarkness = max(0.f, m_fOcclusionDarkness);
}

float A3DPostGodRay::GetOcclusionDarkness()
{
	return m_fOcclusionDarkness;
}

A3DPOST_EFFECT A3DPostGodRay::GetEffectType() const
{
	return A3DPOST_EFFECT_GodRay;
}

A3DRenderTarget* A3DPostGodRay::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    if(!m_pSystem->IsLinearZReady())
        return pSrcRT;

    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    int rtWidth		= pViewport->GetParam()->Width;
    int rtHeight	= pViewport->GetParam()->Height;
    int rtWidth_2	= rtWidth / 2;
    int rtHeight_2	= rtHeight / 2;
    int rtWidth_4	= rtWidth / 4;
    int rtHeight_4	= rtHeight / 4;
    int rtWidth_8	= rtWidth / 8;
    int rtHeight_8	= rtHeight / 8;

    A3DFORMAT colorFmt = A3DFMT_A8R8G8B8;//m_pSystem->GetSmallestFloatRGBAFmt();
    if(colorFmt == A3DFMT_UNKNOWN)
        return false;

    A3DRenderTarget*	m_pFullSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth, rtHeight, colorFmt);
    A3DRenderTarget*	m_pHalfSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, colorFmt);
    A3DRenderTarget*	m_pBlurRT1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, colorFmt);
    A3DRenderTarget*	m_pBlurRT2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, colorFmt);

    if(!(m_pFullSizeRT && m_pHalfSizeRT && m_pBlurRT1 && m_pBlurRT2))
    {
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);
        return pSrcRT;
    }

    A3DVECTOR3 vSunDir = m_vLightDir;
    A3DVECTOR3 vLightDir = -vSunDir;

    vLightDir.Normalize();
    A3DVECTOR4 lOut;
    A3DMATRIX4 mat = pViewport->GetCamera()->GetVPTM();
    float dis = pViewport->GetCamera()->GetZBack();
    A3DVECTOR3 cameraPos = pViewport->GetCamera()->GetPos(); 
    lOut = (cameraPos + vLightDir * dis) * mat;
    lOut.x = lOut.x / lOut.w;
    lOut.y = lOut.y / lOut.w;
    lOut.x = (lOut.x + 1.f) / 2.f;
    lOut.y = (1.f - lOut.y) / 2.f;


    m_pSunPS->Setup2f("g_vRadialBlurCenter", lOut.x, lOut.y);
    m_vBlurCenter = lOut;

    float visibleAngle = m_fVisibleAngle;

    A3DVECTOR3 lookDir = pViewport->GetCamera()->GetDir();
    lookDir.Normalize();
    float angle = fabsf(acosf(DotProduct( lookDir, vLightDir)));
    m_fAngleScale = (visibleAngle - angle) / visibleAngle;
    m_fAngleScale = max2(0.f, m_fAngleScale);
    m_fAngleScale = min2(1.f, m_fAngleScale);

    if(fabsf(lOut.x) > 10.f || fabsf(lOut.y) > 10.f)
        m_fAngleScale = 0.f;

    if(m_fAngleScale < 1e-3)
    {
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
        pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);
        return pSrcRT;
    }

    //设置渲染状态
    pDev->SetAlphaBlendEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);

    m_pSystem->GetA3DDevice()->SetZWriteEnable(false);
    m_pSystem->GetA3DDevice()->SetZTestEnable(false);

    m_pFullSizeRT->ApplyToDevice();

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();

    m_pSunPS->Setup1f("m_fOcclusionDepthRange_0_1", m_fOcclusionDepthRange / (float)pViewport->GetCamera()->GetZBack());
    m_pSunPS->Setup1f("m_fBloomThreshold", m_fBloomThreshold);

    m_pSunPS->Appear();	

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetTextureFilterType(1, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    pSrcRT->AppearAsTexture(0);
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(1);

    m_pSystem->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);

    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    m_pFullSizeRT->WithdrawFromDevice();

    m_pSunPS->Disappear();
    m_pSystem->GetCommonVS()->Disappear();

    //Down Sample
    m_pHalfSizeRT->ApplyToDevice();

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();

    m_pDownSampPS->Setup2f("g_srcPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / (float)m_pSystem->GetHeight());
    m_pDownSampPS->Appear();
    m_pFullSizeRT->AppearAsTexture(0);

    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    m_pDownSampPS->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    m_pHalfSizeRT->WithdrawFromDevice();

    m_pBlurRT1->ApplyToDevice();

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();

    m_pDownSampPS->Setup2f("g_srcPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / (float)m_pSystem->GetHeight());
    m_pDownSampPS->Appear();
    m_pHalfSizeRT->AppearAsTexture(0);

    m_pSystem->GetA3DDevice()->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    m_pDownSampPS->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    m_pBlurRT1->WithdrawFromDevice();


#ifdef _POST_DEBUG_
    //   	if(GetAsyncKeyState(VK_F7) & 0x8000)
    //   	{
    // 		D3DXSaveSurfaceToFileA("GodRay m_pFullSizeRT.dds", D3DXIFF_DDS, m_pFullSizeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //  		D3DXSaveSurfaceToFileA("GodRay m_pHalfSizeRT.dds", D3DXIFF_DDS, m_pHalfSizeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //   		D3DXSaveSurfaceToFileA("GodRay m_pBlurRT1.dds", D3DXIFF_DDS, m_pBlurRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //   	}
#endif

    //模糊三次
    {
        pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
        pDev->SetTextureAddress(0, A3DTADDR_BORDER, A3DTADDR_BORDER);
        pDev->SetSamplerState(0, D3DSAMP_BORDERCOLOR, 0x00000000);

        m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);//假定是屏幕大小
        m_pSystem->GetCommonVS()->Appear();

        m_pBlurPS->Setup2f("g_vRadialBlurCenter", m_vBlurCenter.x, m_vBlurCenter.y);
        m_pBlurPS->Setup2f("g_vScreenSize", float(m_pSystem->GetWidth()), float(m_pSystem->GetHeight()));


        //1
        m_pBlurRT2->ApplyToDevice();
        m_pBlurPS->Appear();
        m_pBlurRT1->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurPS->Disappear();
        m_pBlurRT2->WithdrawFromDevice();
        //2
        m_pBlurRT1->ApplyToDevice();
        m_pBlurPS->Appear();
        m_pBlurRT2->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurPS->Disappear();
        m_pBlurRT1->WithdrawFromDevice();

        //3
        m_pBlurRT2->ApplyToDevice();
        m_pBlurPS->Appear();
        m_pBlurRT1->AppearAsTexture(0);
        m_pSystem->RenderQuad();
        m_pBlurPS->Disappear();
        m_pBlurRT2->WithdrawFromDevice();

        m_pBlurPS->Disappear();
        m_pSystem->GetCommonVS()->Disappear();
    }
#ifdef _POST_DEBUG_
    //    	if(GetAsyncKeyState(VK_F7) & 0x8000)
    //    	{
    //    		D3DXSaveSurfaceToFileA("GodRayBlured.dds", D3DXIFF_DDS, m_pBlurRT2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
    //    	}
#endif
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    pDev->SetTextureFilterType(2, A3DTEXF_POINT);
    pDev->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    //设置RnederTarget

    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);
    pDst->ApplyToDevice();
    //设置Shader参数
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();



    m_pMergePS->Setup1f("m_fOcclusionDepthRange_0_1", m_fOcclusionDepthRange / (float)pViewport->GetCamera()->GetZBack());
    m_pMergePS->SetupVec3("m_vBloomTint", m_vBloomTint);
    m_pMergePS->Setup1f("m_fBloomScale", m_fBloomScale);
    m_pMergePS->Setup1f("m_fScreenBlendThreshold", m_fScreenBlendThreshold);
    m_pMergePS->Setup1f("m_fOcclusionDarkness", m_fOcclusionDarkness);
    m_pMergePS->Setup1f("m_fAngleScale", m_fAngleScale);
    m_pMergePS->Appear();

    pSrcRT->AppearAsTexture(0);
    m_pBlurRT2->AppearAsTexture(1);
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(2);

#ifdef _ANGELICA_2_2
    //m_pSystem->GetScreenIlumRT()->AppearAsTexture(3);
#endif

    m_pSystem->RenderQuad();

    m_pMergePS->Disappear();
    m_pSystem->GetCommonVS()->Disappear();

    pDst->WithdrawFromDevice();
    //m_pSystem->SwapSrcDstRT();

    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT1);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pBlurRT2);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pHalfSizeRT);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(m_pFullSizeRT);

    return pDst;
}
