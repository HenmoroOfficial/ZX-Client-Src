/*
 * FILE: A3DPostSystem.cpp
 *
 * DESCRIPTION: 后处理系统
 *
 * CREATED BY: Panyupin, 2012/2/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "ElementRender.h"
#include "A3DPostSystem.h"
#include "A3DHLSL.h"
#include "A3DPostCC.h"
#include "A3DPostGodRay.h"
#include "A3DPostBloom.h"
#include "A3DPostAA.h"
#include "A3DPostDof.h"
#include "A3DPostRadialBlur.h"
#include "A3DPostMotionBlur.h"
#include "A3DPostWarp.h"
#include "A3DPostFullGlow.h"
#include "A3DPostSunMoon.h"
#include "A3DSunMoon.h"
#include "A3DPostFlare.h"
#include "A3DPostRadialBlurFast.h"
#include "A3DPostSilhouette.h"
#include "A3DSSAO.h"
#include "A3DDeviceCaps.h"

extern ALog g_A3DErrLog;

#define ENABLE_SCREEN_ILLUM 0;

bool A3DPostSystem::Init( A3DDevice* pDev, ElementRender* pElementRender)
{
	m_dwCanInit = 0xFFFFFFFF;

	m_pA3DDevice = pDev;
	m_pA3DEngine = m_pA3DDevice->GetA3DEngine();
	m_pElementRender = pElementRender;

	m_bDeviceSupportRGBA8RT = true;
	m_bDeviceSupportRGBA16RT = pDev->GetA3DDeviceCaps()->SupportABGR16F_RT();
	m_bDeviceSupportRGBA32RT = pDev->GetA3DDeviceCaps()->SupportABGR32F_RT();

	if(m_bDeviceSupportRGBA16RT)
		m_fmtSmallestFloatRGBA = A3DFMT_A16B16G16R16F;
	else if(m_bDeviceSupportRGBA32RT)
		m_fmtSmallestFloatRGBA = A3DFMT_A32B32G32R32F;

	if(pDev->GetA3DDeviceCaps()->SupportR16F_RT())
		m_fmtSmallestFloatR = A3DFMT_R16F;
	else if(pDev->GetA3DDeviceCaps()->SupportR32F_RT())
		m_fmtSmallestFloatR = A3DFMT_R32F;

	m_pCommonVertexShader = m_pA3DEngine->GetA3DHLSLMan()->LoadShader("shaders\\2.2\\HLSL\\PostProcess\\Post_CommonVS.hlsl", NULL, NULL);
	m_pCommonPixelShader = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_Draw.hlsl", NULL);
	m_pIntZ2FloatPixelShader = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_IntZ2Float.hlsl", NULL);
	m_pClearPixelShader = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_Clear.hlsl", NULL);
	m_pScreenIlumPS1 = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_ScreenIlum1.hlsl", NULL);
	m_pScreenIlumPS2 = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_ScreenIlum2.hlsl", NULL);
	m_pScreenIlumPS3 = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_ScreenIlum3.hlsl", NULL);
	m_pScreenIlumAccum = m_pA3DEngine->GetA3DHLSLMan()->LoadShader(NULL, "shaders\\2.2\\HLSL\\PostProcess\\Post_ScreenIlumAccum.hlsl", NULL);
	
	if(!m_pCommonVertexShader)
	{
		g_A3DErrLog.Log("A3DPostSystem::Init failed! shaders\\2.2\\HLSL\\PostProcess\\Post_CommonVS.hlsl load failed");
		Release();
		return false;
	}
	if(!m_pCommonPixelShader)
	{
		g_A3DErrLog.Log("A3DPostSystem::Init failed! shaders\\2.2\\HLSL\\PostProcess\\Post_Draw.hlsl load failed");
		Release();
		return false;
	}
	if(!m_pIntZ2FloatPixelShader)
	{
		g_A3DErrLog.Log("A3DPostSystem::Init failed! shaders\\2.2\\HLSL\\PostProcess\\Post_IntZ2Float.hlsl load failed");
		Release();
		return false;
	}
	if(!m_pClearPixelShader)
	{
		g_A3DErrLog.Log("A3DPostSystem::Init failed! shaders\\2.2\\HLSL\\PostProcess\\Post_Clear.hlsl load failed");
		Release();
		return false;
	}
	if(!m_pScreenIlumPS1 || !m_pScreenIlumPS2 || !m_pScreenIlumPS3 || !m_pScreenIlumAccum)
	{
		g_A3DErrLog.Log("A3DPostSystem::Init failed! shaders\\2.2\\HLSL\\PostProcess\\Post_ScreenIlum.hlsl load failed");
		Release();
		return false;
	}
		
	if(!CreateResource())
		return false;
	m_pA3DDevice->AddUnmanagedDevObject(this);

#if ENABLE_SCREEN_ILLUM
	if(m_fmtSmallestFloatRGBA != A3DFMT_UNKNOWN && ENABLE_SCREEN_ILLUM)
	{
		m_pScreenIlumRT64 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(64, 64, m_fmtSmallestFloatRGBA);
		m_pScreenIlumRT16 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(16, 16, m_fmtSmallestFloatRGBA);
		m_pScreenIlumRT4 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(4, 4, m_fmtSmallestFloatRGBA);
		m_pScreenIlumRT1 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(1, 1, m_fmtSmallestFloatRGBA);
		m_pScreenIlumRT1_accum1 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(1, 1, m_fmtSmallestFloatRGBA);
		m_pScreenIlumRT1_accum2 = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(1, 1, m_fmtSmallestFloatRGBA);

		if(m_pScreenIlumRT1_accum1 != NULL)
		{
			m_pScreenIlumRT1_accum1->ApplyToDevice();
			m_pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);
			m_pScreenIlumRT1_accum1->WithdrawFromDevice();
		}

		if(m_pScreenIlumRT1_accum2 != NULL)
		{
			m_pScreenIlumRT1_accum2->ApplyToDevice();
			m_pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);
			m_pScreenIlumRT1_accum2->WithdrawFromDevice();
		}	
	}
#endif
	const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();
	m_nScreenWidth = d3dpp.BackBufferWidth;
	m_nScreenHeight = d3dpp.BackBufferHeight;

	return true;
}

void A3DPostSystem::Release()
{
	ReleaseResource();
	m_pA3DEngine->GetA3DHLSLMan()->ReleaseShader(m_pCommonVertexShader);
	m_pA3DEngine->GetA3DHLSLMan()->ReleaseShader(m_pCommonPixelShader);
	m_pA3DEngine->GetA3DHLSLMan()->ReleaseShader(m_pIntZ2FloatPixelShader);
	m_pA3DEngine->GetA3DHLSLMan()->ReleaseShader(m_pClearPixelShader);
	
	A3DRELEASE(m_pPostCC);
	A3DRELEASE(m_pPostGodRay);
	A3DRELEASE(m_pPostBloom);
	A3DRELEASE(m_pPostAA);
	A3DRELEASE(m_pPostDof);
	A3DRELEASE(m_pPostRadialBlur);
	A3DRELEASE(m_pPostMotionBlur);
	A3DRELEASE(m_pPostWarp);
	A3DRELEASE(m_pPostFullGlow);
    A3DRELEASE(m_pPostSunMoon);
    A3DRELEASE(m_pPostFlare);
	A3DRELEASE(m_pPostRadialBlurFast);
	A3DRELEASE(m_pPostSilhouette);

	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT64);
	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT16);
	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT4);
	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT1);
	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT1_accum1);
	m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(m_pScreenIlumRT1_accum2);

	m_pA3DDevice->RemoveUnmanagedDevObject(this);
}
void A3DPostSystem::SetupCommonVS(int nTargetWidth, int nTargetHeight)
{
	ASSERT(m_pCommonVertexShader);
	ASSERT(nTargetWidth > 0);
	ASSERT(nTargetHeight > 0);
	m_pCommonVertexShader->SetValue2f("g_vPixelSize", 1.f / float(nTargetWidth), 1.f / float(nTargetHeight));
}


bool A3DPostSystem::BeforeDeviceReset()
{
	return true;
}

bool A3DPostSystem::AfterDeviceReset()
{
	m_bNeedReset = true;
	return true;
}

bool A3DPostSystem::SetEffectMask( DWORD dwFlags )
{
	m_aPostList.clear();

	//添加	
	if(dwFlags & A3DPOST_EFFECT_Silhouette)
	{
		if(!m_pPostSilhouette && A3DPostSilhouette::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_Silhouette))
		{
			m_pPostSilhouette = new A3DPostSilhouette();
			if(!m_pPostSilhouette->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_Silhouette;
				A3DRELEASE(m_pPostSilhouette);
			}
		}
		if(m_pPostSilhouette)
			m_aPostList.push_back(m_pPostSilhouette);
	}

	if(dwFlags & A3DPOST_EFFECT_AA)	//先做AA,会对景深等有好处.
	{
		if(!m_pPostAA && A3DPostAA::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_AA))
		{
			m_pPostAA = new A3DPostAA();
			if(!m_pPostAA->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_AA;
				A3DRELEASE(m_pPostAA);
			}
		}
		if(m_pPostAA)
			m_aPostList.push_back(m_pPostAA);
	}

	if(dwFlags & A3DPOST_EFFECT_GodRay)
	{
		if(!m_pPostGodRay && A3DPostGodRay::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_GodRay))
		{
			m_pPostGodRay = new A3DPostGodRay();
			if(!m_pPostGodRay->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_GodRay;
				A3DRELEASE(m_pPostGodRay);
			}
		}
		if(m_pPostGodRay)
			m_aPostList.push_back(m_pPostGodRay);
	}

	if(dwFlags & A3DPOST_EFFECT_Bloom)
	{
		if(!m_pPostBloom && A3DPostBloom::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_Bloom))
		{
			m_pPostBloom = new A3DPostBloom();
			if(!m_pPostBloom->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_Bloom;			
				A3DRELEASE(m_pPostBloom);
			}
		}
		if(m_pPostBloom)
			m_aPostList.push_back(m_pPostBloom);
	}

    if(dwFlags & A3DPOST_EFFECT_SunMoon)
    {
        if(!m_pPostSunMoon && A3DPostSunMoon::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_SunMoon))
        {
            m_pPostSunMoon = new A3DPostSunMoon();
            if(!m_pPostSunMoon->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_SunMoon;	
				A3DRELEASE(m_pPostSunMoon);
			}

        }
        if(m_pPostSunMoon)
            m_aPostList.push_back(m_pPostSunMoon);
    }

	if(dwFlags & A3DPOST_EFFECT_Dof)
	{
		if(!m_pPostDof && A3DPostDof::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_Dof))
		{
			m_pPostDof = new A3DPostDof();
			if(!m_pPostDof->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_Dof;
				A3DRELEASE(m_pPostDof);
			}
		}
		if(m_pPostDof)
			m_aPostList.push_back(m_pPostDof);
	}
	
	if(dwFlags & A3DPOST_EFFECT_FullGlow)
	{
		if(!m_pPostFullGlow && A3DPostFullGlow::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_FullGlow))
		{
			m_pPostFullGlow = new A3DPostFullGlow();
			if(!m_pPostFullGlow->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_FullGlow;			
				A3DRELEASE(m_pPostFullGlow);
			}
		}
		if(m_pPostFullGlow)
			m_aPostList.push_back(m_pPostFullGlow);
	}

	if(dwFlags & A3DPOST_EFFECT_RadialBlur)
	{
		if(!m_pPostRadialBlur && A3DPostRadialBlur::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_RadialBlur))
		{
			m_pPostRadialBlur = new A3DPostRadialBlur();
			if(!m_pPostRadialBlur->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_RadialBlur;
				A3DRELEASE(m_pPostRadialBlur);
			}
		}
		if(m_pPostRadialBlur)
			m_aPostList.push_back(m_pPostRadialBlur);
	}

	if(dwFlags & A3DPOST_EFFECT_RadialBlurFast)
	{
		if(!m_pPostRadialBlurFast && A3DPostRadialBlurFast::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_RadialBlurFast))
		{
			m_pPostRadialBlurFast = new A3DPostRadialBlurFast();
			if(!m_pPostRadialBlurFast->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_RadialBlurFast;
				A3DRELEASE(m_pPostRadialBlurFast);
			}
		}
		if(m_pPostRadialBlurFast)
			m_aPostList.push_back(m_pPostRadialBlurFast);
	}

	if(dwFlags & A3DPOST_EFFECT_MotionBlur)
	{
		if(!m_pPostMotionBlur && A3DPostMotionBlur::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_MotionBlur))
		{
			m_pPostMotionBlur = new A3DPostMotionBlur();
			if(!m_pPostMotionBlur->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_MotionBlur;
				A3DRELEASE(m_pPostMotionBlur);
			}
		}
		if(m_pPostMotionBlur)
			m_aPostList.push_back(m_pPostMotionBlur);
	}
	if(dwFlags & A3DPOST_EFFECT_Warp)
	{
		if(!m_pPostWarp && A3DPostWarp::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_Warp))
		{
			m_pPostWarp = new A3DPostWarp();
			if(!m_pPostWarp->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_Warp;
				A3DRELEASE(m_pPostWarp);
			}
		}
		if(m_pPostWarp)
			m_aPostList.push_back(m_pPostWarp);
	}
    if(dwFlags & A3DPOST_EFFECT_Flare)
    {
        if(!m_pPostFlare && A3DPostFlare::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_Flare))
        {
            m_pPostFlare = new A3DPostFlare();
            if(!m_pPostFlare->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_Flare;		
				A3DRELEASE(m_pPostFlare);
			}

        }
        if(m_pPostFlare)
            m_aPostList.push_back(m_pPostFlare);
    }

	if(dwFlags & A3DPOST_EFFECT_CC)
	{
		if(!m_pPostCC && A3DPostCC::IsHardwareSupport(m_pA3DDevice) && (m_dwCanInit & A3DPOST_EFFECT_CC))
		{
			m_pPostCC = new A3DPostCC();
			if(!m_pPostCC->Init(this))
			{
				m_dwCanInit &= ~A3DPOST_EFFECT_CC;
				A3DRELEASE(m_pPostCC);
			}
		}
		if(m_pPostCC)
			m_aPostList.push_back(m_pPostCC);
	}

	//删除	为防止某些效果频繁开关, 不进行删除

	m_dwFlags = 0;
	if(m_pPostAA)
		m_dwFlags |= A3DPOST_EFFECT_AA;
	if(m_pPostDof)
		m_dwFlags |= A3DPOST_EFFECT_Dof;	
	if(m_pPostBloom)
		m_dwFlags |= A3DPOST_EFFECT_Bloom;	
	if(m_pPostGodRay)
		m_dwFlags |= A3DPOST_EFFECT_GodRay;
	if(m_pPostRadialBlur)
		m_dwFlags |= A3DPOST_EFFECT_RadialBlur;
	if(m_pPostRadialBlurFast)
		m_dwFlags |= A3DPOST_EFFECT_RadialBlurFast;
	if(m_pPostMotionBlur)
		m_dwFlags |= A3DPOST_EFFECT_MotionBlur;
	if(m_pPostFullGlow)
		m_dwFlags |= A3DPOST_EFFECT_FullGlow;
	if(m_pPostCC)
		m_dwFlags |= A3DPOST_EFFECT_CC;
	if(m_pPostWarp)
		m_dwFlags |= A3DPOST_EFFECT_Warp;
    if(m_pPostSunMoon)
        m_dwFlags |= A3DPOST_EFFECT_SunMoon;

	return m_dwFlags == dwFlags;
}

bool A3DPostSystem::Render(A3DRenderTarget* pInputRT, A3DViewport* pViewport, bool bUseIntZ, bool bApplyPostEffect)
{	
	//A3DRenderTarget* pOriColorTarget0 = m_pColorTarget[0];
	//A3DRenderTarget* pOriColorTarget1 = m_pColorTarget[1];

    A3DRenderTarget* pRT = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(GetWidth(), GetHeight(), A3DFMT_A8R8G8B8);
	if(!pInputRT)// 拷贝帧缓存到内部RT
	{
		HRESULT hval = S_OK;
		hval = m_pA3DDevice->StretchRect(m_pA3DDevice->GetBackBuffer()->m_pD3DSurface, NULL, 
			pRT->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_POINT);
		if(FAILED(hval))
		{
			ASSERT(0);
			g_A3DErrLog.Log("A3DPostSystem::Issue stretch copy surface failed.");
		}
	}
	else
	{
		//拷贝InputRT到内部RT
		HRESULT hval = S_OK;
		hval = m_pA3DDevice->StretchRect(pInputRT->GetTargetSurface()->m_pD3DSurface, NULL, 
			pRT->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_POINT);
		if(FAILED(hval))
		{
			ASSERT(0);
			g_A3DErrLog.Log("A3DPostSystem::Issue stretch copy surface failed.");
		}

		//m_pColorTarget[0] = pInputRT;
	}
	
	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	//先把当前状态保存下来.
	bool bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bOldZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bOldZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE oldCullType = m_pA3DDevice->GetFaceCull();
	DWORD dwOldColorWrite = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bool bOldFogEnable = m_pA3DDevice->GetFogEnable();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN);
	m_pA3DDevice->SetFogEnable(false);


	if(bApplyPostEffect == false || m_aPostList.size() == 0 )
		goto end;

#if ENABLE_SCREEN_ILLUM
	ComputeScreenIlum();
#endif

	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);


	for(int i = 0; i < MAX_SAMPLE_LAYERS; i++)
		m_pA3DDevice->ClearTexture(i);


	//	依次渲染后处理
	for(size_t i = 0; i < m_aPostList.size(); i++)
	{		
		A3DPost* pPost = m_aPostList[i];
		if(m_bAAAfterUI)
		{
			if(pPost->GetEffectType() == A3DPOST_EFFECT_AA)
				continue;
		}
		if(m_bCCAfterUI)
		{
			if(pPost->GetEffectType() == A3DPOST_EFFECT_CC)
				continue;
		}

		m_pStream->Appear(0, false);
		m_pStreamDecl->Appear();
		pRT = pPost->ApplyEffect(pViewport, pRT);
	}
end:
	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	//把结果拷贝到BackBuffer
	SetupCommonVS(m_nScreenWidth, m_nScreenHeight);
	m_pCommonVertexShader->Appear();
	m_pCommonPixelShader->SetTexture("sampSrcColor", pRT);
	m_pCommonPixelShader->Appear();
	RenderQuad();
	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();
    GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pRT);

	//m_pColorTarget[0] = pOriColorTarget0;
	//m_pColorTarget[1] = pOriColorTarget1;

	//还原渲染状态
	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwOldColorWrite);
	m_pA3DDevice->SetFogEnable(bOldFogEnable);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
	return true;
}

void A3DPostSystem::ReleaseResource()
{
	A3DRELEASE(m_pStream);
	A3DRELEASE(m_pStreamDecl);
	A3DRELEASE(m_pColorTarget[0]);
	A3DRELEASE(m_pColorTarget[1]);

}

bool A3DPostSystem::CreateResource()
{
	m_aVertices[0].Set(A3DVECTOR3(-1, -1, 0), A3DCOLORRGB(255, 255, 255), 0, 0, 1, 0, 1);
	m_aVertices[1].Set(A3DVECTOR3( 1, -1, 0), A3DCOLORRGB(255, 255, 255), 0, 1, 1, 1, 1);
	m_aVertices[2].Set(A3DVECTOR3(-1,  1, 0), A3DCOLORRGB(255, 255, 255), 0, 0, 0, 0, 0);
	m_aVertices[3].Set(A3DVECTOR3( 1,  1, 0), A3DCOLORRGB(255, 255, 255), 0, 1, 0, 1, 0);

	m_aVertices[4] = m_aVertices[0];
	m_aVertices[5] = m_aVertices[1];
	m_aVertices[6] = m_aVertices[2];
	m_aVertices[7] = m_aVertices[3];
	static WORD aIndices[] = {0, 2, 1, 1, 2, 3};

	m_pStream = new A3DStream;
	if(!m_pStream->Init(m_pA3DDevice, A3DVT_IBLVERTEX, 8, 6, A3DSTRM_STATIC, A3DSTRM_STATIC))
		return false;

	m_pStream->SetVerts((BYTE*)m_aVertices, 8);
	m_pStream->SetIndices(aIndices, 6);

	D3DVERTEXELEMENT9 aDecl[MAX_FVF_DECL_SIZE];
	a3d_MakeVSDeclarator(A3DVT_IBLVERTEX, 0, aDecl);
	m_pStreamDecl = new A3DVertexDecl();
    m_pStreamDecl->Init(m_pA3DDevice, aDecl);

    /*
	A3DFORMAT fmtColorRT = A3DFMT_A8R8G8B8;

	A3DRenderTarget::RTFMT fmt;
	fmt.iWidth = 0;
	fmt.iHeight = 0;
	fmt.fmtTarget = fmtColorRT;
	fmt.fmtDepth = A3DFMT_UNKNOWN;

	m_pColorTarget[1] = new A3DRenderTarget();
	if(!m_pColorTarget[1]->Init(m_pA3DDevice, fmt, true, false))
	{
		g_A3DErrLog.Log("A3DPostSystem::CreateResource create m_pColorTarget failed!");
		ASSERT(0);
		return false;
	}

	m_pColorTarget[0] = new A3DRenderTarget();
	if(!m_pColorTarget[0]->Init(m_pA3DDevice, fmt, true, false))
	{
		g_A3DErrLog.Log("A3DPostSystem::CreateResource create m_pColorTarget failed!");
		ASSERT(0);
		return false;
	}
    */

	return true;
}

void A3DPostSystem::RenderQuad()
{
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 0, 4, 0, 2);
}

void A3DPostSystem::OutputIntZ(A3DViewport* pViewport)
{
	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	//渲染得到线性深度纹理
	bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE cullType = m_pA3DDevice->GetFaceCull();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	m_pElementRender->GetLinearZRT()->ApplyToDevice();

	SetupCommonVS(m_nScreenWidth, m_nScreenHeight);
	m_pCommonVertexShader->Appear();

	m_pIntZ2FloatPixelShader->SetValue1f("zn", pViewport->GetCamera()->GetZFront());
	m_pIntZ2FloatPixelShader->SetValue1f("zf", pViewport->GetCamera()->GetZBack());
	m_pIntZ2FloatPixelShader->Appear();

	RenderQuad();
	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();
	
	m_pElementRender->GetLinearZRT()->WithdrawFromDevice();
	
	m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
	m_pA3DDevice->SetFaceCull(cullType);

	D3DXSaveSurfaceToFileA("LinearIntZ.dds", D3DXIFF_DDS, m_pElementRender->GetLinearZRT()->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
}

bool A3DPostSystem::ApplyParam(const A3DPOST_EFFECT_PARAM* pParam)
{
	// 根据当前参数开关后处理特效
	SetEffectMask(pParam->dwPostEffectMask);

	m_bAAAfterUI = pParam->aaParam.bAfterUI;
	m_bCCAfterUI = pParam->ccParam.bAfterUI;

	if(m_pPostBloom)
	{
		m_pPostBloom->SetBlurSize(pParam->bloomParam.fBlurSize);
		m_pPostBloom->SetBrightScale(pParam->bloomParam.fBrightScale);
		m_pPostBloom->SetBrightThreshold(pParam->bloomParam.fBrightThreshold);
	}
	if(m_pPostCC)
	{
		if(pParam->ccParam.bUseManualParam1)
			m_pPostCC->SetCCTexture1ByManualParam(pParam->ccParam.manualParam1);
		else
			m_pPostCC->SetCCTexture1(pParam->ccParam.szTex1);
		
		if(pParam->ccParam.bUseManualParam2)
			m_pPostCC->SetCCTexture2ByManualParam(pParam->ccParam.manualParam2);
		else
			m_pPostCC->SetCCTexture2(pParam->ccParam.szTex2);
		//m_pPostCC->SetCCTexture(pParam->ccParam.szTex1, pParam->ccParam.szTex2);
		m_pPostCC->SetFactor(pParam->ccParam.fFactor);
	}
	if(m_pPostDof)
	{
		m_pPostDof->SetClampBlurFar(pParam->dofParam.fClampBlurFar);
		m_pPostDof->SetFocus(pParam->dofParam.fNearBlur, pParam->dofParam.fNearFocus, pParam->dofParam.fFarFocus, pParam->dofParam.fFarBlur);
	}
	if(m_pPostGodRay)
	{
		m_pPostGodRay->SetLightDir(
			m_pElementRender->GetWorldObjs().pSunMoon ? m_pElementRender->GetWorldObjs().pSunMoon->ComputeSunDir() : m_pElementRender->GetParams()->paramLight.Direction);
		m_pPostGodRay->SetVisibleAngle(pParam->godRayParam.fVisibleAngle);
		m_pPostGodRay->SetOcclusionDepthRange(pParam->godRayParam.fOcclusionDepthRange);
		m_pPostGodRay->SetBloomScale(pParam->godRayParam.fBloomScale);
		m_pPostGodRay->SetBloomThreshold(pParam->godRayParam.fBloomThreshold);
		m_pPostGodRay->SetScreenBlendThreshold(pParam->godRayParam.fScreenBlendThreshold);
		m_pPostGodRay->SetBloomTintColor(pParam->godRayParam.vBloomTintColor);
		m_pPostGodRay->SetOcclusionDarkness(pParam->godRayParam.fOcclusionDarkness);
		
	}
	if(m_pPostRadialBlur)
	{
		m_pPostRadialBlur->SetBlurCenterScreenSpace(pParam->radiusBlurParam.fBlurCenterX, pParam->radiusBlurParam.fBlurCenterY);
		m_pPostRadialBlur->SetBlurLength(pParam->radiusBlurParam.fBlurLength);
		m_pPostRadialBlur->SetFocusRadius(pParam->radiusBlurParam.fInFocusRadius, pParam->radiusBlurParam.fOutFocusRadius);
	}
	if(m_pPostRadialBlurFast)
	{
		m_pPostRadialBlurFast->SetBlurCenterScreenSpace(pParam->radiusBlurFastParam.fBlurCenterX, pParam->radiusBlurFastParam.fBlurCenterY);
		m_pPostRadialBlurFast->SetBlurLength(pParam->radiusBlurFastParam.fBlurLength);
		m_pPostRadialBlurFast->SetFocusRadius(pParam->radiusBlurFastParam.fInFocusRadius, pParam->radiusBlurFastParam.fOutFocusRadius);
	}
	if(m_pPostMotionBlur)
	{
		m_pPostMotionBlur->SetBlurMax(pParam->motionBlurParam.fBlurMax);
		m_pPostMotionBlur->SetBlurScale(pParam->motionBlurParam.fBlurScale);
		m_pPostMotionBlur->SetBlurScalePosition(pParam->motionBlurParam.fBlurScalePosition);
		m_pPostMotionBlur->SetBlurScaleRotation(pParam->motionBlurParam.fBlurScaleRotation);
		m_pPostMotionBlur->SetInFocusDistance(pParam->motionBlurParam.fInFocusDistance);
		m_pPostMotionBlur->SetOutFocusDistance(pParam->motionBlurParam.fOutFocusDistance);
	}
	if(m_pPostWarp)
	{
		m_pPostWarp->SetScale(pParam->warpParam.fScale);
		m_pPostWarp->SetSpeed(pParam->warpParam.fSpeed);
		m_pPostWarp->EnableMask(pParam->warpParam.bEnableMask);
		m_pPostWarp->SetMaskTexture(pParam->warpParam.strMaskTextureName);
	}
	
	if(m_pPostFullGlow)
	{
		m_pPostFullGlow->SetBlurSize(pParam->fullGlowParam.fBlurSize);
		m_pPostFullGlow->SetGlowColor(pParam->fullGlowParam.vGlowColor);
		m_pPostFullGlow->SetGlowLevel(pParam->fullGlowParam.fGlowLevel);
		m_pPostFullGlow->SetGlowPower(pParam->fullGlowParam.fGlowPower);
	}

    if (m_pPostSunMoon)
    {
        m_pPostSunMoon->SetSunMoon(m_pElementRender->GetParams()->bRenderSunMoon ? m_pElementRender->GetWorldObjs().pSunMoon : NULL);
    }

    if (m_pPostFlare)
    {
        m_pPostFlare->SetSunMoon(m_pElementRender->GetParams()->bRenderSunMoon ? m_pElementRender->GetWorldObjs().pSunMoon : NULL);
    }

	if(m_pPostSilhouette)
	{
		m_pPostSilhouette->SetBlurDist(pParam->silhouetteParam.fBlurDist);
		m_pPostSilhouette->SetHardness(pParam->silhouetteParam.fHardness);
		m_pPostSilhouette->SetLineColor(pParam->silhouetteParam.vLineColor);
	}
	return true;
}

bool A3DPostSystem::RetriveParam( A3DPOST_EFFECT_PARAM* pParam )
{
	pParam->dwPostEffectMask = m_dwFlags;
	if(m_pPostBloom)
	{
		pParam->bloomParam.fBlurSize			= m_pPostBloom->GetBlurSize();
		pParam->bloomParam.fBrightScale			= m_pPostBloom->GetBrightScale();
		pParam->bloomParam.fBrightThreshold		= m_pPostBloom->GetBrightThreshold();
	}
	if(m_pPostCC)
	{
		pParam->ccParam.szTex1 = m_pPostCC->GetCCTexture1();
		pParam->ccParam.szTex2 = m_pPostCC->GetCCTexture2();
		pParam->ccParam.fFactor = m_pPostCC->GetFactor();
		pParam->ccParam.bUseManualParam1 = m_pPostCC->IsCC1Manual();
		pParam->ccParam.bUseManualParam2 = m_pPostCC->IsCC2Manual();
		pParam->ccParam.manualParam1 = m_pPostCC->GetManualParam1();
		pParam->ccParam.manualParam2 = m_pPostCC->GetManualParam2();
	}
	if(m_pPostDof)
	{
		pParam->dofParam.fClampBlurFar = m_pPostDof->GetClampBlurFar();
		m_pPostDof->GetFocus(&pParam->dofParam.fNearBlur, &pParam->dofParam.fNearFocus, &pParam->dofParam.fFarFocus, &pParam->dofParam.fFarBlur);
	}
	if(m_pPostGodRay)
	{
		pParam->godRayParam.fVisibleAngle = m_pPostGodRay->GetVisibleAngle();
		pParam->godRayParam.fOcclusionDepthRange = m_pPostGodRay->GetOcclusionDepthRange();
		pParam->godRayParam.fBloomScale = m_pPostGodRay->GetBloomScale();
		pParam->godRayParam.fBloomThreshold = m_pPostGodRay->GetBloomThreshold();
		pParam->godRayParam.fScreenBlendThreshold = m_pPostGodRay->GetScreenBlendThreshold();
		pParam->godRayParam.vBloomTintColor = m_pPostGodRay->GetBloomTintColor();
		pParam->godRayParam.fOcclusionDarkness = m_pPostGodRay->GetOcclusionDarkness();
	}
	if(m_pPostRadialBlur)
	{
		m_pPostRadialBlur->GetBlurCenterScreenSpace(&pParam->radiusBlurParam.fBlurCenterX, &pParam->radiusBlurParam.fBlurCenterY);
		pParam->radiusBlurParam.fBlurLength = m_pPostRadialBlur->GetBlurLength();
		m_pPostRadialBlur->GetFocusRadius(&pParam->radiusBlurParam.fInFocusRadius, &pParam->radiusBlurParam.fOutFocusRadius);
	}
	if(m_pPostRadialBlurFast)
	{
		m_pPostRadialBlurFast->GetBlurCenterScreenSpace(&pParam->radiusBlurFastParam.fBlurCenterX, &pParam->radiusBlurFastParam.fBlurCenterY);
		pParam->radiusBlurFastParam.fBlurLength = m_pPostRadialBlurFast->GetBlurLength();
		m_pPostRadialBlurFast->GetFocusRadius(&pParam->radiusBlurFastParam.fInFocusRadius, &pParam->radiusBlurFastParam.fOutFocusRadius);
	}
	if(m_pPostMotionBlur)
	{
		pParam->motionBlurParam.fBlurMax = m_pPostMotionBlur->GetBlurMax();
		pParam->motionBlurParam.fBlurScale = m_pPostMotionBlur->GetBlurScale();
		pParam->motionBlurParam.fBlurScalePosition = m_pPostMotionBlur->GetBlurScalePosition();
		pParam->motionBlurParam.fBlurScaleRotation = m_pPostMotionBlur->GetBlurScaleRotation();
		pParam->motionBlurParam.fInFocusDistance = m_pPostMotionBlur->GetInFocusDistance();
		pParam->motionBlurParam.fOutFocusDistance = m_pPostMotionBlur->GetOutFocusDistance();
	}
	if(m_pPostWarp)
	{
		pParam->warpParam.fScale = m_pPostWarp->GetScale();
		pParam->warpParam.fSpeed = m_pPostWarp->GetSpeed();
		pParam->warpParam.bEnableMask = m_pPostWarp->IsEnableMask();
		pParam->warpParam.strMaskTextureName = m_pPostWarp->GetMaskTextureName();
	}
	if(m_pPostFullGlow)
	{
		pParam->fullGlowParam.fBlurSize = m_pPostFullGlow->GetBlurSize();
		pParam->fullGlowParam.vGlowColor = m_pPostFullGlow->GetGlowColor();
		pParam->fullGlowParam.fGlowLevel = m_pPostFullGlow->GetGlowLevel();
		pParam->fullGlowParam.fGlowPower = m_pPostFullGlow->GetGlowPower();
	}
	if(m_pPostSilhouette)
	{
		pParam->silhouetteParam.fHardness = m_pPostSilhouette->GetHardness();
		pParam->silhouetteParam.fBlurDist = m_pPostSilhouette->GetBlurDist();
		pParam->silhouetteParam.vLineColor = m_pPostSilhouette->GetLineColor();
	}
	return true;
}

bool A3DPostSystem::ClearRenderTarget( A3DRenderTarget* pRenderTarget, int iWidth, int iHeight, const A3DVECTOR4& vColor)
{
	if(!pRenderTarget)
		return false;
	
	bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE cullType = m_pA3DDevice->GetFaceCull();


	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	pRenderTarget->ApplyToDevice();
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	SetupCommonVS(iWidth, iHeight);
	GetCommonVS()->Appear();
	m_pClearPixelShader->SetValue4f("g_clearColor", &vColor);
	m_pClearPixelShader->Appear();
	RenderQuad();

	pRenderTarget->WithdrawFromDevice();

	m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
	m_pA3DDevice->SetFaceCull(cullType);

	return true;
}

bool A3DPostSystem::BeforeRender()
{
	// 检查是否发生了设备重置
	if(m_bNeedReset)
	{
		const D3DPRESENT_PARAMETERS& d3dpp = m_pA3DDevice->GetD3DPP();

		if(m_pScreenIlumRT1_accum1 != NULL)
		{
			m_pScreenIlumRT1_accum1->ApplyToDevice();
			m_pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);
			m_pScreenIlumRT1_accum1->WithdrawFromDevice();
		}

		if(m_pScreenIlumRT1_accum2 != NULL)
		{
			m_pScreenIlumRT1_accum2->ApplyToDevice();
			m_pA3DDevice->Clear(D3DCLEAR_TARGET, A3DCOLORRGBA(0,0,0,0), 1.0f, 0);
			m_pScreenIlumRT1_accum2->WithdrawFromDevice();
		}

		m_nScreenWidth = d3dpp.BackBufferWidth;
		m_nScreenHeight = d3dpp.BackBufferHeight;

		m_bNeedReset = false;
	}
	return true;
}

bool A3DPostSystem::RenderLinearZ(A3DViewport* pViewport, A3DRenderTarget* pLinearZRT, A3DRenderTarget* pUnLinearZRT, bool bUseIntZ)
{
	//如果输出线性RT为空，或者想使用IntZ但是没有IntZRT，或者不使用IntZ但是没有传入非线性深度RT
	if(!pLinearZRT || (bUseIntZ && !m_pA3DDevice->GetIntZRenderTarget())
		|| (!bUseIntZ && !pUnLinearZRT))
		return false;

	//先把当前状态保存下来.
	bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE cullType = m_pA3DDevice->GetFaceCull();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN);

	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	pLinearZRT->ApplyToDevice();

	SetupCommonVS(m_nScreenWidth, m_nScreenHeight);
	m_pCommonVertexShader->Appear();

	m_pIntZ2FloatPixelShader->SetValue1f("zn", pViewport->GetCamera()->GetZFront());
	m_pIntZ2FloatPixelShader->SetValue1f("zf", pViewport->GetCamera()->GetZBack());
	m_pIntZ2FloatPixelShader->Appear();

	if(bUseIntZ && m_pA3DDevice->GetIntZRenderTarget())
	{
		m_pA3DDevice->GetIntZRenderTarget()->AppearAsTexture(0, true);
	}
	else if(pUnLinearZRT)
	{
		pUnLinearZRT->AppearAsTexture(0);
	}

	RenderQuad();
	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();

	pLinearZRT->WithdrawFromDevice();

	m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
	m_pA3DDevice->SetFaceCull(cullType);

	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	return true;
}

A3DRenderTarget* A3DPostSystem::GetLinearDepthRT()
{
	return m_pElementRender->GetLinearZRT();
}

A3DFORMAT A3DPostSystem::GetLinearDepthFmt()
{
	return m_pElementRender->GetLinearZRT()->GetFormat().fmtTarget;
}

bool A3DPostSystem::IsLinearZReady()
{
	return m_pElementRender->IsLinearZReady();
}

A3DFORMAT A3DPostSystem::GetBackBufferFmt()
{
	return m_pA3DDevice->GetDevFormat().fmtAdapter;
}

void A3DPostSystem::ComputeScreenIlum()
{
	if(!m_pScreenIlumRT64 || !m_pScreenIlumRT16 || !m_pScreenIlumRT4
		|| !m_pScreenIlumRT1 || !m_pScreenIlumRT1_accum1 || !m_pScreenIlumRT1_accum2)
		return;

	bool bAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bool bAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bool bZTestEnable = m_pA3DDevice->GetZTestEnable();
	bool bZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	A3DCULLTYPE cullType = m_pA3DDevice->GetFaceCull();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);


	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);

	float fExpChangeSpeed = 0.7f;
	float g_fElapsedTime = fExpChangeSpeed / GetA3DEngine()->GetFPS();

	A3DRenderTarget* input = GetSrcRT();

	m_pScreenIlumRT64->ApplyToDevice();
	m_pScreenIlumPS1->SetTexture("g_SamplerLinear", input);
	m_pScreenIlumPS1->SetTexture("g_SamplerLInearDepth", GetLinearDepthRT());
	m_pScreenIlumPS1->Appear();
	SetupCommonVS(64, 64);
	GetCommonVS()->Appear();
	RenderQuad();
	GetCommonVS()->Disappear();
	m_pScreenIlumPS1->Disappear();
	m_pScreenIlumRT64->WithdrawFromDevice();

	m_pScreenIlumRT16->ApplyToDevice();
	m_pScreenIlumPS2->SetTexture("g_SamplerLinear", m_pScreenIlumRT64);
	m_pScreenIlumPS2->SetValue2f("gpost_vPixelSize", 16.f, 16.f);
	m_pScreenIlumPS2->Appear();
	SetupCommonVS(16, 16);
	GetCommonVS()->Appear();
	RenderQuad();
	GetCommonVS()->Disappear();
	m_pScreenIlumPS2->Disappear();
	m_pScreenIlumRT16->WithdrawFromDevice();

	m_pScreenIlumRT4->ApplyToDevice();
	m_pScreenIlumPS2->SetTexture("g_SamplerLinear", m_pScreenIlumRT16);
	m_pScreenIlumPS2->SetValue2f("gpost_vPixelSize", 4.f, 4.f);
	m_pScreenIlumPS2->Appear();
	SetupCommonVS(4, 4);
	GetCommonVS()->Appear();
	RenderQuad();
	GetCommonVS()->Disappear();
	m_pScreenIlumPS2->Disappear();
	m_pScreenIlumRT4->WithdrawFromDevice();

	m_pScreenIlumRT1->ApplyToDevice();
	m_pScreenIlumPS3->SetTexture("g_SamplerLinear", m_pScreenIlumRT4);
	m_pScreenIlumPS3->SetValue2f("gpost_vPixelSize", 1.f, 1.f);
	m_pScreenIlumPS3->Appear();
	SetupCommonVS(1, 1);
	GetCommonVS()->Appear();
	RenderQuad();
	GetCommonVS()->Disappear();
	m_pScreenIlumPS3->Disappear();
	m_pScreenIlumRT1->WithdrawFromDevice();


	A3DRenderTarget* ptemp = m_pScreenIlumRT1_accum1;
	m_pScreenIlumRT1_accum1 = m_pScreenIlumRT1_accum2;
	m_pScreenIlumRT1_accum2 = ptemp;

	m_pScreenIlumRT1_accum1->ApplyToDevice();
	m_pScreenIlumAccum->SetTexture("g_SamplerLast", m_pScreenIlumRT1_accum2);
	m_pScreenIlumAccum->SetTexture("g_SamplerCur", m_pScreenIlumRT1);
	m_pScreenIlumAccum->SetValue1f("g_fElapsedTime", g_fElapsedTime);
	m_pScreenIlumAccum->Appear();
	SetupCommonVS(1, 1);
	GetCommonVS()->Appear();
	RenderQuad();
	GetCommonVS()->Disappear();
	m_pScreenIlumAccum->Disappear();
	m_pScreenIlumRT1_accum1->WithdrawFromDevice();

	m_pA3DDevice->SetAlphaBlendEnable(bAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bZWriteEnable);
	m_pA3DDevice->SetFaceCull(cullType);

#if _POST_DEBUG_
//  	if(GetAsyncKeyState(VK_F7) & 0x8000)
//  	{
//  		D3DXSaveSurfaceToFileA("input.dds", D3DXIFF_DDS, input->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT64.dds", D3DXIFF_DDS, m_pScreenIlumRT64->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT16.dds", D3DXIFF_DDS, m_pScreenIlumRT16->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT4.dds", D3DXIFF_DDS, m_pScreenIlumRT4->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT1.dds", D3DXIFF_DDS, m_pScreenIlumRT1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT1_accum1.dds", D3DXIFF_DDS, m_pScreenIlumRT1_accum1->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		D3DXSaveSurfaceToFileA("m_pScreenIlumRT1_accum2.dds", D3DXIFF_DDS, m_pScreenIlumRT1_accum2->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  
//  	}
#endif

}

A3DRenderTarget* A3DPostSystem::GetScreenIlumRT()
{
	return m_pScreenIlumRT1_accum1;
}

static const D3DVERTEXELEMENT9 s_aVertDecl[] = 
{
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 1},
	{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
	D3DDECL_END()
};

const D3DVERTEXELEMENT9* A3DPostSystem::GetD3D9VertElement() const
{
	return s_aVertDecl;
}

bool A3DPostSystem::RenderAfterUI( A3DRenderTarget* pInputRT, A3DViewport* pViewport, bool bUseIntZ, bool bApplyPostEffect /*= true*/ )
{
	//A3DRenderTarget* pOriColorTarget0 = m_pColorTarget[0];
	//A3DRenderTarget* pOriColorTarget1 = m_pColorTarget[1];
    A3DRenderTarget* pRT = m_pA3DDevice->GetRenderTargetMan()->RentRenderTargetColor(GetWidth(), GetHeight(), A3DFMT_A8R8G8B8);

	if(!pInputRT)// 拷贝帧缓存到内部RT
	{
		HRESULT hval = S_OK;
		hval = m_pA3DDevice->StretchRect(m_pA3DDevice->GetBackBuffer()->m_pD3DSurface, NULL, 
			pRT->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_POINT);
		if(FAILED(hval))
		{
			ASSERT(0);
			g_A3DErrLog.Log("A3DPostSystem::Issue stretch copy surface failed.");
		}
	}
	else
	{
        //拷贝InputRT到内部RT
        HRESULT hval = S_OK;
        hval = m_pA3DDevice->StretchRect(pInputRT->GetTargetSurface()->m_pD3DSurface, NULL, 
            pRT->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_POINT);
        if(FAILED(hval))
        {
            ASSERT(0);
            g_A3DErrLog.Log("A3DPostSystem::Issue stretch copy surface failed.");
        }

		//pRT = pInputRT;
	}

	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	//先把当前状态保存下来.
	bool bOldAlphaBlendEnable;
	bool bOldAlphaTestEnable;
	bool bOldZTestEnable;
	bool bOldZWriteEnable;
	A3DCULLTYPE oldCullType;
	DWORD dwColorWrite;
	bool bOldFogEnable;

	bOldAlphaBlendEnable = m_pA3DDevice->GetAlphaBlendEnable();
	bOldAlphaTestEnable = m_pA3DDevice->GetAlphaTestEnable();
	bOldZTestEnable = m_pA3DDevice->GetZTestEnable();
	bOldZWriteEnable = m_pA3DDevice->GetZWriteEnable();
	oldCullType = m_pA3DDevice->GetFaceCull();
	dwColorWrite = m_pA3DDevice->GetDeviceRenderState(D3DRS_COLORWRITEENABLE);
	bOldFogEnable = m_pA3DDevice->GetFogEnable();

	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pA3DDevice->SetAlphaTestEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_ALPHA | D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_GREEN);
	m_pA3DDevice->SetFogEnable(false);


	if(bApplyPostEffect == false || m_aPostList.size() == 0 )
		goto end;

	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
	//	依次渲染后处理
	for(size_t i = 0; i < m_aPostList.size(); i++)
	{
		if(m_bAAAfterUI || m_bCCAfterUI)
		{
			if(		(m_aPostList[i]->GetEffectType() == A3DPOST_EFFECT_AA && m_bAAAfterUI)
				||	(m_aPostList[i]->GetEffectType() == A3DPOST_EFFECT_CC && m_bCCAfterUI))
			{
				m_pStream->Appear(0, false);
				m_pStreamDecl->Appear();

				pRT = m_aPostList[i]->ApplyEffect(pViewport, pRT);
			}
		}
	}
end:
	m_pStream->Appear(0, false);
	m_pStreamDecl->Appear();

	//把结果拷贝到BackBuffer
	SetupCommonVS(m_nScreenWidth, m_nScreenHeight);
	m_pCommonVertexShader->Appear();
	m_pCommonPixelShader->SetTexture("sampSrcColor", pRT);
	m_pCommonPixelShader->Appear();
	RenderQuad();
	m_pA3DDevice->ClearVertexShader();
	m_pA3DDevice->ClearPixelShader();

    m_pA3DDevice->GetRenderTargetMan()->ReturnRenderTarget(pRT);

	//m_pColorTarget[0] = pOriColorTarget0;
	//m_pColorTarget[1] = pOriColorTarget1;

	m_pA3DDevice->SetAlphaBlendEnable(bOldAlphaBlendEnable);
	m_pA3DDevice->SetAlphaTestEnable(bOldAlphaTestEnable);
	m_pA3DDevice->SetZTestEnable(bOldZTestEnable);
	m_pA3DDevice->SetZWriteEnable(bOldZWriteEnable);
	m_pA3DDevice->SetFaceCull(oldCullType);
	m_pA3DDevice->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, dwColorWrite);
	m_pA3DDevice->SetFogEnable(bOldFogEnable);

	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR );
	m_pA3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR );

	return true;
}

A3DRenderTarget* A3DPostSystem::GetPlayerSilhousetteRT() const
{
	if(m_pPostSilhouette)
		return m_pPostSilhouette->GetPlayerSilhousetteRT();
	return NULL;
}

bool A3DPostSystem::IsActive(A3DPost* pPost) const
{
    if (!pPost)
        return false;
    for(size_t i = 0; i < m_aPostList.size(); i++)
    {
        if (m_aPostList[i] == pPost)
            return true;
    }

    return false;
}

void A3DPostSystem::SwapSrcDstRT()
{
    //不应该调用
    ASSERT(0);
    //A3DRenderTarget* pTemp = m_pColorTarget[0]; 
    //m_pColorTarget[0] = m_pColorTarget[1];
    //m_pColorTarget[1] = pTemp;
}
