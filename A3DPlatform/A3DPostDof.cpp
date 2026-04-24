/*
 * FILE: A3DPostDof.cpp
 *
 * DESCRIPTION: 景深后处理
 *
 * CREATED BY: Panyupin, 2012/2/7
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostDof.h"
#include "A3DPostSystem.h"

#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_Dof_SC2_FillAlphaWithCoC_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_FillAlphaWithCoC.hlsl";
static const char* szPost_Dof_SC2_DownScaleRGB_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_DownScaleRGB.hlsl";
static const char* szPost_Dof_SC2_BlurMaxX_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_BlurMaxX.hlsl";
static const char* szPost_Dof_SC2_BlurMaxY_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_BlurMaxY.hlsl";
static const char* szPost_Dof_SC2_DownScaleDepth_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_DownScaleDepth.hlsl";
static const char* szPost_Dof_SC2_BlurSmallDepthX_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_BlurSmallDepthX.hlsl";
static const char* szPost_Dof_SC2_BlurSmallDepthY_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_BlurSmallDepthY.hlsl";
static const char* szPost_Dof_SC2_DOF_PS_hlsl = "shaders\\HLSL\\PostProcess\\Post_Dof_SC2_DOF_PS.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_Dof_SC2_FillAlphaWithCoC_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_FillAlphaWithCoC.hlsl";
static const char* szPost_Dof_SC2_DownScaleRGB_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_DownScaleRGB.hlsl";
static const char* szPost_Dof_SC2_BlurMaxX_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_BlurMaxX.hlsl";
static const char* szPost_Dof_SC2_BlurMaxY_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_BlurMaxY.hlsl";
static const char* szPost_Dof_SC2_DownScaleDepth_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_DownScaleDepth.hlsl";
static const char* szPost_Dof_SC2_BlurSmallDepthX_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_BlurSmallDepthX.hlsl";
static const char* szPost_Dof_SC2_BlurSmallDepthY_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_BlurSmallDepthY.hlsl";
static const char* szPost_Dof_SC2_DOF_PS_hlsl = "shaders\\2.2\\HLSL\\PostProcess\\Post_Dof_SC2_DOF_PS.hlsl";
#endif


extern ALog g_A3DErrLog;

A3DPostDof::~A3DPostDof()
{

}

bool A3DPostDof::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;
	m_pHLSL_FillAlphaWithCoC = CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_FillAlphaWithCoC_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	m_pHLSL_DownScaleRGBCoC	= CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_DownScaleRGB_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	m_pHLSL_BlurMaxRGBCoCX = CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_BlurMaxX_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
		
	m_pHLSL_BlurMaxRGBCoCY = CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_BlurMaxY_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	m_pHLSL_DownScaleDepth	= CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_DownScaleDepth_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	m_pHLSL_BlurDepthX = CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_BlurSmallDepthX_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
	
	m_pHLSL_BlurDepthY	= CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_BlurSmallDepthY_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());

	m_pHLSL_SC2_Dof = CreateA3DHLSL_Common(NULL, NULL, szPost_Dof_SC2_DOF_PS_hlsl, "ps_main",
		0, NULL, m_pSystem->GetA3DDevice(), m_pSystem->GetD3D9VertElement());
		
	if(!m_pHLSL_FillAlphaWithCoC	 ||
		!m_pHLSL_DownScaleRGBCoC	 ||
		!m_pHLSL_BlurMaxRGBCoCX		 ||
		!m_pHLSL_BlurMaxRGBCoCY		 ||
		!m_pHLSL_DownScaleDepth	 ||
		!m_pHLSL_BlurDepthX	 ||
		!m_pHLSL_BlurDepthY	 ||
		!m_pHLSL_SC2_Dof)
	{
		g_A3DErrLog.Log("A3DPostDof::Init Failed! Can not load HLSL files!");
		Release();
		return false;
	}

	return true;
}

void A3DPostDof::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_FillAlphaWithCoC);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_DownScaleRGBCoC);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_BlurMaxRGBCoCX);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_BlurMaxRGBCoCY);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_DownScaleDepth);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_BlurDepthX);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_BlurDepthY);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pHLSL_SC2_Dof);		
}

bool A3DPostDof::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestVertexShaderVersion(2, 0) ||
		!pA3DDev->TestPixelShaderVersion(2, 0))
		return false;
	
	return true;
}

bool A3DPostDof::Render( A3DViewport* pViewport )
{
	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();
	
	A3DFORMAT fmtColor = A3DFMT_A8R8G8B8;//m_pSystem->GetSmallestFloatRGBAFmt();
	A3DFORMAT fmtDepth = m_pSystem->GetSmallestFloatRFmt();

	if(fmtColor == A3DFMT_UNKNOWN || fmtDepth == A3DFMT_UNKNOWN)
		return false;

	if(!m_pSystem->IsLinearZReady())
		return false;

	int rtWidth		= 0;
	int rtHeight	= 0;
	int rtWidth_2	= -2;
	int rtHeight_2	= -2;
	int rtWidth_4	= -4;
	int rtHeight_4	= -4;
	int rtWidth_8	= -8;
	int rtHeight_8	= -8;

	A3DRenderTarget*	m_pColorAndCoCRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth, rtHeight, fmtColor);
	A3DRenderTarget* 	m_pRGBCoCHalfSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmtColor);
	A3DRenderTarget*	m_pDepthHalfSizeRT  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmtDepth);
	A3DRenderTarget* 	m_pRGBCoCQuarterSize1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtColor);
	A3DRenderTarget* 	m_pRGBCoCQuarterSize2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtColor);
	A3DRenderTarget* 	m_pDepthQuarterSizeRT1  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtDepth);
	A3DRenderTarget* 	m_pDepthQuarterSizeRT2  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtDepth);
	
	if(!(m_pRGBCoCQuarterSize1 && m_pRGBCoCQuarterSize2 && m_pDepthQuarterSizeRT1 && m_pDepthQuarterSizeRT2 &&
		m_pRGBCoCHalfSizeRT && m_pDepthHalfSizeRT && m_pColorAndCoCRT))
	{
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize1);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize2);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT1);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT2);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCHalfSizeRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthHalfSizeRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pColorAndCoCRT);

		return false;
	}




	pDev->SetAlphaBlendEnable(false);
	pDev->SetAlphaTestEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);
	 
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	
	//填充CoC
	m_pColorAndCoCRT->ApplyToDevice();
	pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fZFar", pViewport->GetCamera()->GetZBack());
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fNearDis", m_fNearBlur);
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFocusDis1", m_fNearFocus);
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFocusDis2", m_fFarFocus);
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFarDis", m_fFarBlur);
	m_pHLSL_FillAlphaWithCoC->Setup1f("g_fClampBlurFar", m_fClampBlurFar);
	m_pHLSL_FillAlphaWithCoC->Appear();
	m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
	pSrc->AppearAsTexture(1);
	m_pSystem->RenderQuad();
	m_pHLSL_FillAlphaWithCoC->Disappear();
	m_pColorAndCoCRT->WithdrawFromDevice();

	//缩小back buffer
	m_pRGBCoCHalfSizeRT->ApplyToDevice();
	pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_DownScaleRGBCoC->Setup2f("g_srcPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	m_pHLSL_DownScaleRGBCoC->Appear();
	m_pColorAndCoCRT->AppearAsTexture(0);

	m_pSystem->RenderQuad();
	m_pHLSL_DownScaleRGBCoC->Disappear();
	m_pRGBCoCHalfSizeRT->WithdrawFromDevice();
#ifdef _DEBUG_POST_
// 	if(GetAsyncKeyState(VK_F7) & 0x8000)
// 	{
// 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurHalfSizeRT.dds", D3DXIFF_DDS, m_pDepthFieldBlurHalfSizeRT->GetTargetSurface(), NULL, NULL);
// 	}
#endif

	//继续缩小
	m_pRGBCoCQuarterSize1->ApplyToDevice();	
	pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_DownScaleRGBCoC->Setup2f("g_srcPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
	m_pHLSL_DownScaleRGBCoC->Appear();
	m_pRGBCoCHalfSizeRT->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_DownScaleRGBCoC->Disappear();
	m_pRGBCoCQuarterSize1->WithdrawFromDevice();
#ifdef _DEBUG_POST_
// 	if(GetAsyncKeyState(VK_F7) & 0x8000)
// 	{
// 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT1_1.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
// 	}
#endif

	//模糊
	//X
	m_pRGBCoCQuarterSize2->ApplyToDevice();

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();

	
	m_pHLSL_BlurMaxRGBCoCX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
	m_pHLSL_BlurMaxRGBCoCX->Appear();
	m_pRGBCoCQuarterSize1->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_BlurMaxRGBCoCX->Disappear();
	m_pRGBCoCQuarterSize2->WithdrawFromDevice();
#ifdef _DEBUG_POST_
// 	if(GetAsyncKeyState(VK_F7) & 0x8000)
// 	{
// 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT2_2.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT2->GetTargetSurface(), NULL, NULL);
// 	}
#endif

	//Y
	m_pRGBCoCQuarterSize1->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();
	
	m_pHLSL_BlurMaxRGBCoCY->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
	m_pHLSL_BlurMaxRGBCoCY->Appear();
	m_pRGBCoCQuarterSize2->AppearAsTexture(0);
	m_pSystem->RenderQuad();//模糊的结果存在了RT1之上
	m_pHLSL_BlurMaxRGBCoCY->Disappear();
	m_pRGBCoCQuarterSize1->WithdrawFromDevice();
#ifdef _DEBUG_POST_
// 	if(GetAsyncKeyState(VK_F7) & 0x8000)
// 	{
// 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT1_2.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
// 	}
#endif

	//缩小depth buffer

	m_pDepthHalfSizeRT->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_DownScaleDepth->Setup2f("g_srcPixelSize", 1.f / (float)m_pSystem->GetWidth(), 1.f / (float)m_pSystem->GetHeight());
	m_pHLSL_DownScaleDepth->Appear();
	m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_DownScaleDepth->Disappear();
	m_pDepthHalfSizeRT->WithdrawFromDevice();
	
	//继续缩小
	m_pDepthQuarterSizeRT1->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_DownScaleDepth->Setup2f("g_srcPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
	m_pHLSL_DownScaleDepth->Appear();
	m_pDepthHalfSizeRT->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_DownScaleDepth->Disappear();
	m_pDepthQuarterSizeRT1->WithdrawFromDevice();

	//模糊
	//X
	m_pDepthQuarterSizeRT2->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_BlurDepthX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
	m_pHLSL_BlurDepthX->Appear();
	m_pDepthQuarterSizeRT1->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_BlurDepthX->Disappear();
	m_pDepthQuarterSizeRT2->WithdrawFromDevice();

	//Y
	m_pDepthQuarterSizeRT1->ApplyToDevice();
	m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_BlurDepthX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
	m_pHLSL_BlurDepthX->Appear();
	m_pDepthQuarterSizeRT2->AppearAsTexture(0);
	m_pSystem->RenderQuad();
	m_pHLSL_BlurDepthX->Disappear();
	m_pDepthQuarterSizeRT1->WithdrawFromDevice();

	//最终
	pDst->ApplyToDevice();
	for(int i = 0; i < 4; i++)
	{
		pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
		pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	}

	pDev->SetTextureFilterType(0, A3DTEXF_POINT);

	m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
	m_pSystem->GetCommonVS()->Appear();
	m_pHLSL_SC2_Dof->Setup1f("g_fZFar", pViewport->GetCamera()->GetZBack());
	m_pHLSL_SC2_Dof->Setup1f("g_fNearDis", m_fNearBlur);
	m_pHLSL_SC2_Dof->Setup1f("g_fFocusDis1", m_fNearFocus);
	m_pHLSL_SC2_Dof->Setup1f("g_fFocusDis2", m_fFarFocus);
	m_pHLSL_SC2_Dof->Setup1f("g_fFarDis", m_fFarBlur);
	m_pHLSL_SC2_Dof->Setup1f("g_fClampBlurFar", m_fClampBlurFar);
	m_pHLSL_SC2_Dof->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	m_pHLSL_SC2_Dof->Appear();
	m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
	m_pDepthQuarterSizeRT1->AppearAsTexture(1);
	m_pColorAndCoCRT->AppearAsTexture(2);
	m_pRGBCoCQuarterSize1->AppearAsTexture(3);
	m_pSystem->RenderQuad();

	m_pHLSL_SC2_Dof->Disappear();
	m_pSystem->GetCommonVS()->Disappear();
	pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
	pDst->WithdrawFromDevice();

	m_pSystem->SwapSrcDstRT();

#ifdef _DEBUG_POST_
//   	if(GetAsyncKeyState(VK_F7) & 0x8000)
//   	{
//   		D3DXSaveSurfaceToFileA("LinearZ.dds", D3DXIFF_DDS, m_pSystem->GetLinearDepthRT()->GetTargetSurface(), NULL, NULL);
//   		D3DXSaveSurfaceToFileA("BlurDepth.dds", D3DXIFF_DDS, m_pDepthBlurRT1->GetTargetSurface(), NULL, NULL);
//   		D3DXSaveSurfaceToFileA("BlurDof.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
//   		D3DXSaveSurfaceToFileA("RGBCOC.dds", D3DXIFF_DDS, m_pColorAndCoCRT->GetTargetSurface(), NULL, NULL);
//   	}
#endif

	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize1);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize2);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT1);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT2);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCHalfSizeRT);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthHalfSizeRT);
	m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pColorAndCoCRT);
	return true;	

}

void A3DPostDof::SetClampBlurFar( float fBlurFar )
{
	fBlurFar = max(0.f, fBlurFar);
	fBlurFar = min(1.f, fBlurFar);
	m_fClampBlurFar = fBlurFar;
}

void A3DPostDof::SetFocus( float fNearBlur, float fNearFocus, float fFarFocus, float fFarBlur )
{
	if(fNearBlur <= fNearFocus && fNearFocus <= fFarFocus && fFarFocus <= fFarBlur)
	{
		m_fNearBlur = fNearBlur;
		m_fNearFocus = fNearFocus;
		m_fFarFocus = fFarFocus;
		m_fFarBlur = fFarBlur;
	}
	else
		return;
		//g_A3DErrLog.Log("A3DPostDof::SetFocus parameter error!");
}

void A3DPostDof::GetFocus( float* pfNearBlur, float* pfNearFocus, float* pfFarFocus, float* pfFarBlur )
{
	*pfNearBlur = m_fNearBlur;
	*pfNearFocus = m_fNearFocus;
	*pfFarFocus = m_fFarFocus;
	*pfFarBlur = m_fFarBlur;
}

A3DPOST_EFFECT A3DPostDof::GetEffectType() const
{
	return A3DPOST_EFFECT_Dof;
}

A3DRenderTarget* A3DPostDof::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    A3DDevice* pDev = m_pSystem->GetA3DDevice();

    A3DFORMAT fmtColor = A3DFMT_A8R8G8B8;//m_pSystem->GetSmallestFloatRGBAFmt();
    A3DFORMAT fmtDepth = m_pSystem->GetSmallestFloatRFmt();

    if(fmtColor == A3DFMT_UNKNOWN || fmtDepth == A3DFMT_UNKNOWN)
        return pSrcRT;

    if(!m_pSystem->IsLinearZReady())
        return pSrcRT;

    int rtWidth		= pSrcRT->GetWidth();
    int rtHeight	= pSrcRT->GetHeight();
    int rtWidth_2	= rtWidth / 2;
    int rtHeight_2	= rtHeight / 2;
    int rtWidth_4	= rtWidth / 4;
    int rtHeight_4	= rtHeight / 4;
    int rtWidth_8	= rtWidth / 8;
    int rtHeight_8	= rtHeight / 8;

    A3DRenderTarget*	m_pColorAndCoCRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth, rtHeight, fmtColor);
    A3DRenderTarget* 	m_pRGBCoCHalfSizeRT = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmtColor);
    A3DRenderTarget*	m_pDepthHalfSizeRT  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_2, rtHeight_2, fmtDepth);
    A3DRenderTarget* 	m_pRGBCoCQuarterSize1 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtColor);
    A3DRenderTarget* 	m_pRGBCoCQuarterSize2 = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtColor);
    A3DRenderTarget* 	m_pDepthQuarterSizeRT1  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtDepth);
    A3DRenderTarget* 	m_pDepthQuarterSizeRT2  = pDev->GetRenderTargetMan()->RentRenderTargetColor(rtWidth_4, rtHeight_4, fmtDepth);

    if(!(m_pRGBCoCQuarterSize1 && m_pRGBCoCQuarterSize2 && m_pDepthQuarterSizeRT1 && m_pDepthQuarterSizeRT2 &&
        m_pRGBCoCHalfSizeRT && m_pDepthHalfSizeRT && m_pColorAndCoCRT))
    {
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize1);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize2);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT1);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT2);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCHalfSizeRT);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthHalfSizeRT);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pColorAndCoCRT);

        return pSrcRT;
    }




    pDev->SetAlphaBlendEnable(false);
    pDev->SetAlphaTestEnable(false);
    pDev->SetZTestEnable(false);
    pDev->SetZWriteEnable(false);
    pDev->SetFaceCull(A3DCULL_NONE);

    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDev->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

    //填充CoC
    m_pColorAndCoCRT->ApplyToDevice();
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fZFar", pViewport->GetCamera()->GetZBack());
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fNearDis", m_fNearBlur);
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFocusDis1", m_fNearFocus);
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFocusDis2", m_fFarFocus);
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fFarDis", m_fFarBlur);
    m_pHLSL_FillAlphaWithCoC->Setup1f("g_fClampBlurFar", m_fClampBlurFar);
    m_pHLSL_FillAlphaWithCoC->Appear();
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
    pSrcRT->AppearAsTexture(1);
    m_pSystem->RenderQuad();
    m_pHLSL_FillAlphaWithCoC->Disappear();
    m_pColorAndCoCRT->WithdrawFromDevice();

    //缩小back buffer
    m_pRGBCoCHalfSizeRT->ApplyToDevice();
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_DownScaleRGBCoC->Setup2f("g_srcPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
    m_pHLSL_DownScaleRGBCoC->Appear();
    m_pColorAndCoCRT->AppearAsTexture(0);

    m_pSystem->RenderQuad();
    m_pHLSL_DownScaleRGBCoC->Disappear();
    m_pRGBCoCHalfSizeRT->WithdrawFromDevice();
#ifdef _DEBUG_POST_
    // 	if(GetAsyncKeyState(VK_F7) & 0x8000)
    // 	{
    // 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurHalfSizeRT.dds", D3DXIFF_DDS, m_pDepthFieldBlurHalfSizeRT->GetTargetSurface(), NULL, NULL);
    // 	}
#endif

    //继续缩小
    m_pRGBCoCQuarterSize1->ApplyToDevice();	
    pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE, 
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_DownScaleRGBCoC->Setup2f("g_srcPixelSize", 2.f / float(m_pSystem->GetWidth()), 2.f / float(m_pSystem->GetHeight()));
    m_pHLSL_DownScaleRGBCoC->Appear();
    m_pRGBCoCHalfSizeRT->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_DownScaleRGBCoC->Disappear();
    m_pRGBCoCQuarterSize1->WithdrawFromDevice();
#ifdef _DEBUG_POST_
    // 	if(GetAsyncKeyState(VK_F7) & 0x8000)
    // 	{
    // 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT1_1.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
    // 	}
#endif

    //模糊
    //X
    m_pRGBCoCQuarterSize2->ApplyToDevice();

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();


    m_pHLSL_BlurMaxRGBCoCX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
    m_pHLSL_BlurMaxRGBCoCX->Appear();
    m_pRGBCoCQuarterSize1->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_BlurMaxRGBCoCX->Disappear();
    m_pRGBCoCQuarterSize2->WithdrawFromDevice();
#ifdef _DEBUG_POST_
    // 	if(GetAsyncKeyState(VK_F7) & 0x8000)
    // 	{
    // 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT2_2.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT2->GetTargetSurface(), NULL, NULL);
    // 	}
#endif

    //Y
    m_pRGBCoCQuarterSize1->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();

    m_pHLSL_BlurMaxRGBCoCY->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
    m_pHLSL_BlurMaxRGBCoCY->Appear();
    m_pRGBCoCQuarterSize2->AppearAsTexture(0);
    m_pSystem->RenderQuad();//模糊的结果存在了RT1之上
    m_pHLSL_BlurMaxRGBCoCY->Disappear();
    m_pRGBCoCQuarterSize1->WithdrawFromDevice();
#ifdef _DEBUG_POST_
    // 	if(GetAsyncKeyState(VK_F7) & 0x8000)
    // 	{
    // 		D3DXSaveSurfaceToFileA("m_pDepthFieldBlurRT1_2.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
    // 	}
#endif

    //缩小depth buffer

    m_pDepthHalfSizeRT->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 2, m_pSystem->GetHeight() / 2);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_DownScaleDepth->Setup2f("g_srcPixelSize", 1.f / (float)m_pSystem->GetWidth(), 1.f / (float)m_pSystem->GetHeight());
    m_pHLSL_DownScaleDepth->Appear();
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_DownScaleDepth->Disappear();
    m_pDepthHalfSizeRT->WithdrawFromDevice();

    //继续缩小
    m_pDepthQuarterSizeRT1->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_DownScaleDepth->Setup2f("g_srcPixelSize", 2.f / (float)m_pSystem->GetWidth(), 2.f / (float)m_pSystem->GetHeight());
    m_pHLSL_DownScaleDepth->Appear();
    m_pDepthHalfSizeRT->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_DownScaleDepth->Disappear();
    m_pDepthQuarterSizeRT1->WithdrawFromDevice();

    //模糊
    //X
    m_pDepthQuarterSizeRT2->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_BlurDepthX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
    m_pHLSL_BlurDepthX->Appear();
    m_pDepthQuarterSizeRT1->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_BlurDepthX->Disappear();
    m_pDepthQuarterSizeRT2->WithdrawFromDevice();

    //Y
    m_pDepthQuarterSizeRT1->ApplyToDevice();
    m_pSystem->SetupCommonVS(m_pSystem->GetWidth() / 4, m_pSystem->GetHeight() / 4);
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_BlurDepthX->Setup2f("g_vPixelSize", 4.f / float(m_pSystem->GetWidth()), 4.f / float(m_pSystem->GetHeight()));
    m_pHLSL_BlurDepthX->Appear();
    m_pDepthQuarterSizeRT2->AppearAsTexture(0);
    m_pSystem->RenderQuad();
    m_pHLSL_BlurDepthX->Disappear();
    m_pDepthQuarterSizeRT1->WithdrawFromDevice();

    A3DRenderTarget* pDst = pDev->GetRenderTargetMan()->RentRenderTargetColor(m_pSystem->GetWidth(), m_pSystem->GetHeight(), A3DFMT_A8R8G8B8);

    //最终
    pDst->ApplyToDevice();
    for(int i = 0; i < 4; i++)
    {
        pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
        pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
    }

    pDev->SetTextureFilterType(0, A3DTEXF_POINT);

    m_pSystem->SetupCommonVS(m_pSystem->GetWidth(), m_pSystem->GetHeight());
    m_pSystem->GetCommonVS()->Appear();
    m_pHLSL_SC2_Dof->Setup1f("g_fZFar", pViewport->GetCamera()->GetZBack());
    m_pHLSL_SC2_Dof->Setup1f("g_fNearDis", m_fNearBlur);
    m_pHLSL_SC2_Dof->Setup1f("g_fFocusDis1", m_fNearFocus);
    m_pHLSL_SC2_Dof->Setup1f("g_fFocusDis2", m_fFarFocus);
    m_pHLSL_SC2_Dof->Setup1f("g_fFarDis", m_fFarBlur);
    m_pHLSL_SC2_Dof->Setup1f("g_fClampBlurFar", m_fClampBlurFar);
    m_pHLSL_SC2_Dof->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
    m_pHLSL_SC2_Dof->Appear();
    m_pSystem->GetLinearDepthRT()->AppearAsTexture(0);
    m_pDepthQuarterSizeRT1->AppearAsTexture(1);
    m_pColorAndCoCRT->AppearAsTexture(2);
    m_pRGBCoCQuarterSize1->AppearAsTexture(3);
    m_pSystem->RenderQuad();

    m_pHLSL_SC2_Dof->Disappear();
    m_pSystem->GetCommonVS()->Disappear();
    pDev->SetTextureFilterType(0, A3DTEXF_LINEAR);
    pDst->WithdrawFromDevice();

#ifdef _DEBUG_POST_
    //   	if(GetAsyncKeyState(VK_F7) & 0x8000)
    //   	{
    //   		D3DXSaveSurfaceToFileA("LinearZ.dds", D3DXIFF_DDS, m_pSystem->GetLinearDepthRT()->GetTargetSurface(), NULL, NULL);
    //   		D3DXSaveSurfaceToFileA("BlurDepth.dds", D3DXIFF_DDS, m_pDepthBlurRT1->GetTargetSurface(), NULL, NULL);
    //   		D3DXSaveSurfaceToFileA("BlurDof.dds", D3DXIFF_DDS, m_pDepthFieldBlurRT1->GetTargetSurface(), NULL, NULL);
    //   		D3DXSaveSurfaceToFileA("RGBCOC.dds", D3DXIFF_DDS, m_pColorAndCoCRT->GetTargetSurface(), NULL, NULL);
    //   	}
#endif

    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize1);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCQuarterSize2);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT1);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthQuarterSizeRT2);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pRGBCoCHalfSizeRT);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pDepthHalfSizeRT);
    m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pColorAndCoCRT);
    pDev->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);

    return pDst;	
}
