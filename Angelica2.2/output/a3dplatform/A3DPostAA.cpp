/*
 * FILE: A3DPostAA.cpp
 *
 * DESCRIPTION: 反走样后处理
 *
 * CREATED BY: Panyupin, 2012/2/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2012 Archosaur Studio, All Rights Reserved.
 */
#include "A3DPostAA.h"
#include "A3DPostSystem.h"
#include "A3DVertexShader.h"


#ifdef _ANGELICA_2_1
#include "A3DHLSLWrapper.h"
static const char* szPost_AAVS_HLSL = "shaders\\HLSL\\PostProcess\\Post_AAVS.hlsl";
static const char* szPost_AAPS_HLSL = "shaders\\HLSL\\PostProcess\\Post_AAPS.hlsl";
static const char* szSearchTex_dds = "shaders\\hlsl\\PostProcess\\SearchTex.dds";
static const char* szAreaTexDX9_dds = "shaders\\hlsl\\PostProcess\\AreaTexDX9.dds";
static const char* szPost_smaa_edge_HLSL = "shaders\\HLSL\\PostProcess\\post_smaa_edge.hlsl";
static const char* szPost_smaa_blend_HLSL =  "shaders\\HLSL\\PostProcess\\post_smaa_blend.hlsl";
static const char* szPost_smaa_neighbor_HLSL = "shaders\\HLSL\\PostProcess\\post_smaa_neighbor.hlsl";
#else
#include "A3DHLSL.h"
static const char* szPost_AAVS_HLSL = "shaders\\2.2\\HLSL\\PostProcess\\Post_AAVS.hlsl";
static const char* szPost_AAPS_HLSL = "shaders\\2.2\\HLSL\\PostProcess\\Post_AAPS.hlsl";
static const char* szSearchTex_dds = "shaders\\textures\\SearchTex.dds";
static const char* szAreaTexDX9_dds = "shaders\\textures\\AreaTexDX9.dds";
static const char* szPost_smaa_edge_HLSL = "shaders\\2.2\\HLSL\\PostProcess\\post_smaa_edge.hlsl";
static const char* szPost_smaa_blend_HLSL =  "shaders\\2.2\\HLSL\\PostProcess\\post_smaa_blend.hlsl";
static const char* szPost_smaa_neighbor_HLSL = "shaders\\2.2\\HLSL\\PostProcess\\post_smaa_neighbor.hlsl";
#endif

extern ALog g_A3DErrLog;

const static D3DVERTEXELEMENT9 s_vertexElements[3] = {
	{ 0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0 },
	{ 0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD,  0 },
	D3DDECL_END()
};

A3DPostAA::~A3DPostAA()
{

}

bool A3DPostAA::Init( A3DPostSystem* pSystem )
{
	m_pSystem = pSystem;

	m_pFXAAVS = CreateA3DHLSL_Common(szPost_AAVS_HLSL, "vs_main", NULL, NULL, 0, NULL, m_pSystem->GetA3DDevice(), s_vertexElements);
	if(!m_pFXAAVS)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! Post_AAVS.hlsl load failed!");
		Release();
		return false;
	}
	
	const char* fxaaMacros[1];
	fxaaMacros[0] = "_FXAA_V3_";
	m_pFXAAPS = CreateA3DHLSL_Common(NULL, NULL, szPost_AAPS_HLSL, "ps_main", 1, fxaaMacros, m_pSystem->GetA3DDevice(), s_vertexElements);
	if (!m_pFXAAPS)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! Post_AAPS.hlsl load failed!");
		Release();
		return false;
	}

	bool bTextureLoaded = false;
	bTextureLoaded = pSystem->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szSearchTex_dds, 
		66, 33, A3DFMT_L8, 0, &m_pSearchTex);
	if(!bTextureLoaded)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! SearchTex.dds load failed!");
		Release();
		return false;
	}

	bTextureLoaded = false;
	bTextureLoaded = pSystem->GetA3DEngine()->GetA3DTextureMan()->LoadTextureFromFile(szAreaTexDX9_dds, 
		160, 560, A3DFMT_A8L8, 0, &m_pAreaTex);
	if(!bTextureLoaded)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! AreaTexDX9.dds load failed!");
		Release();
		return false;
	}

	m_pVertexDeclaration = new A3DVertexDeclCommon;
	m_pVertexDeclaration->Init(m_pSystem->GetA3DDevice(), s_vertexElements);

	if(!LoadSMAAShader())
	{
		Release();
		return false;
	}
	return true;
}

void A3DPostAA::Release()
{
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pFXAAVS);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pFXAAPS);	
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pSMAAEdgeDetect);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pSMAABlend);
	ReleaseA3DHLSL_Common(m_pSystem->GetA3DDevice(), m_pSMAANeighbor);

	A3DRELEASE(m_pVertexDeclaration);

}

bool A3DPostAA::IsHardwareSupport(A3DDevice* pA3DDev)
{
	if (!pA3DDev->TestPixelShaderVersion(3, 0))
		return false;
	return true;
}

bool A3DPostAA::Render( A3DViewport* pViewport )
{
	/*
// #ifdef _DEBUG_POST_
	if(GetAsyncKeyState('1') & 0x8000)
		return true;

	if(GetAsyncKeyState('3') & 0x8000)
	{
		ReloadA3DHLSL_Common(m_pSMAAEdgeDetect);
		ReloadA3DHLSL_Common(m_pSMAABlend);
		ReloadA3DHLSL_Common(m_pSMAANeighbor);
	}
//#endif
	*/

	A3DDevice* pDev = m_pSystem->GetA3DDevice();
	A3DRenderTarget* pSrc = m_pSystem->GetSrcRT();
	A3DRenderTarget* pDst = m_pSystem->GetDstRT();

	ASSERT(pDev && pSrc && pDst);
    DoRender(pViewport, pSrc, pDst);
	//交换RT
	m_pSystem->SwapSrcDstRT();
	return true;

	//以下是FXAA, 不使用
	/*
	pDst->ApplyToDevice();

	m_pFXAAVS->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	m_pFXAAVS->Appear();
	m_pFXAAPS->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	pSrc->AppearAsTexture(0);
	m_pFXAAPS->Appear();

	m_pSystem->RenderQuad();

	m_pFXAAVS->Disappear();
	m_pFXAAPS->Disappear();

	pDst->WithdrawFromDevice();

	m_pSystem->SwapSrcDstRT();
	*/
	return true;
}

bool A3DPostAA::DoRender(A3DViewport* pViewport, A3DRenderTarget* pSrc, A3DRenderTarget* pDst)
{
	/*
// #ifdef _DEBUG_POST_
	if(GetAsyncKeyState('1') & 0x8000)
		return true;

	if(GetAsyncKeyState('3') & 0x8000)
	{
		ReloadA3DHLSL_Common(m_pSMAAEdgeDetect);
		ReloadA3DHLSL_Common(m_pSMAABlend);
		ReloadA3DHLSL_Common(m_pSMAANeighbor);
	}
//#endif
	*/

    int iWidth = pDst->GetWidth();
    int iHeight = pDst->GetHeight();
	A3DDevice* pDev = m_pSystem->GetA3DDevice();

	ASSERT(pDev && pSrc && pDst);

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetAlphaTestEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);
	pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
	//当前采用SMAA版本的反走样算法. 
	{
		int rtWidth = pViewport->GetParam()->Width;
		int rtHeight = pViewport->GetParam()->Height;

		A3DRenderTarget*	m_pEdgeRT;
		A3DRenderTarget*	m_pBlendRT;
		A3DRenderTarget*	m_pStencilRT;

		m_pEdgeRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(iWidth, iHeight, A3DFMT_A8R8G8B8);
		m_pBlendRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(iWidth, iHeight, A3DFMT_A8R8G8B8);
		m_pStencilRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetDepth(iWidth, iHeight, A3DFMT_D24S8);

		if(!m_pEdgeRT || !m_pBlendRT || !m_pStencilRT)
		{	
			m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pEdgeRT);
			m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlendRT);
			m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pStencilRT);
			return false;
		}
	
		m_pStencilRT->ApplyToDevice();

		//Edge	
		m_pEdgeRT->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

 		DWORD dwStencilPass = pDev->GetDeviceRenderState(D3DRS_STENCILPASS);
 		DWORD dwStencilFunc = pDev->GetDeviceRenderState(D3DRS_STENCILFUNC);
 		DWORD dwStencilRef = pDev->GetDeviceRenderState(D3DRS_STENCILREF);

 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, TRUE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, 1);
		
		m_pSMAAEdgeDetect->Setup2f("SMAA_PIXEL_SIZE", 1.f / iWidth, 1.f / iHeight);

		m_pSMAAEdgeDetect->Appear();

		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		pSrc->AppearAsTexture(0);
		m_pSystem->GetLinearDepthRT()->AppearAsTexture(2);

		DrawQuad(iWidth, iHeight);

		m_pSMAAEdgeDetect->Disappear();
		m_pEdgeRT->WithdrawFromDevice();

		//Blend	
		m_pBlendRT->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, TRUE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
 		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, 1);

		m_pSMAABlend->Setup2f("SMAA_PIXEL_SIZE", 1.f / iWidth, 1.f / iHeight);
		m_pSMAABlend->Appear();

		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		m_pEdgeRT->AppearAsTexture(3);
		m_pAreaTex->Appear(5);
		m_pSearchTex->Appear(6);

        DrawQuad(iWidth, iHeight);

		m_pSMAABlend->Disappear();
		m_pBlendRT->WithdrawFromDevice();


		//Neighbor
 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, FALSE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, dwStencilPass);
 		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, dwStencilFunc);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, dwStencilRef);

		pDst->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		m_pSMAANeighbor->Setup2f("SMAA_PIXEL_SIZE", 
			1.f / static_cast<float>(m_pSystem->GetWidth()), 1.f / static_cast<float>(m_pSystem->GetHeight()));
		m_pSMAANeighbor->Appear();
		pSrc->AppearAsTexture(0);
		m_pBlendRT->AppearAsTexture(4);
		
		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

        DrawQuad(iWidth, iHeight);
	
		m_pSMAANeighbor->Disappear();
		pDst->WithdrawFromDevice();
		m_pStencilRT->WithdrawFromDevice();

//#ifdef _DEBUG_POST_
//  		if(GetAsyncKeyState('2') & 0x8000)
// 		{
//  			D3DXSaveSurfaceToFileA("m_pEdgeRT.dds", D3DXIFF_DDS, m_pEdgeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("m_pBlendRT.dds", D3DXIFF_DDS, m_pBlendRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("AA_Input.dds", D3DXIFF_DDS, pSrc->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("AA_Output.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		}
//#endif

		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pEdgeRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlendRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pStencilRT);
		return true;
	}

	//以下是FXAA, 不使用
	/*
	pDst->ApplyToDevice();

	m_pFXAAVS->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	m_pFXAAVS->Appear();
	m_pFXAAPS->Setup2f("g_vPixelSize", 1.f / float(m_pSystem->GetWidth()), 1.f / float(m_pSystem->GetHeight()));
	pSrc->AppearAsTexture(0);
	m_pFXAAPS->Appear();

	m_pSystem->RenderQuad();

	m_pFXAAVS->Disappear();
	m_pFXAAPS->Disappear();

	pDst->WithdrawFromDevice();

	m_pSystem->SwapSrcDstRT();
	*/
	return true;
}


void A3DPostAA::DrawQuad(int iWidth, int iHeight)
{
	m_pVertexDeclaration->Appear();

	D3DXVECTOR2 pixelSize = D3DXVECTOR2(1.0f / iWidth, 1.0f / iHeight);
	float quad[4][5] =
	{
		{ -1.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 0.0f, 0.0f },
		{  1.0f - pixelSize.x,  1.0f + pixelSize.y, 0.5f, 1.0f, 0.0f },
		{ -1.0f - pixelSize.x, -1.0f + pixelSize.y, 0.5f, 0.0f, 1.0f },
		{  1.0f - pixelSize.x, -1.0f + pixelSize.y, 0.5f, 1.0f, 1.0f }
	};
	m_pSystem->GetA3DDevice()->DrawPrimitiveUP(A3DPT_TRIANGLESTRIP, 2, quad, sizeof(quad[0]));
}

bool A3DPostAA::LoadSMAAShader()
{
	m_pSMAAEdgeDetect = CreateA3DHLSL_Common(szPost_smaa_edge_HLSL, "vs", szPost_smaa_edge_HLSL, "ps", 0, NULL, 
		m_pSystem->GetA3DDevice(), s_vertexElements);
	if(!m_pSMAAEdgeDetect)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! post_smaa_edge.hlsl load failed!");
		return false;
	}

	m_pSMAABlend = CreateA3DHLSL_Common(szPost_smaa_blend_HLSL, "vs", szPost_smaa_blend_HLSL, "ps", 0, NULL, 
		m_pSystem->GetA3DDevice(), s_vertexElements);
	if(!m_pSMAABlend)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! post_smaa_blend.hlsl load failed!");
		return false;
	}

	m_pSMAANeighbor = CreateA3DHLSL_Common(szPost_smaa_neighbor_HLSL, "vs", szPost_smaa_neighbor_HLSL, "ps", 0, NULL, 
		m_pSystem->GetA3DDevice(), s_vertexElements);
	if(!m_pSMAANeighbor)
	{
		g_A3DErrLog.Log("A3DPostAA::Init Failed! post_smaa_neighbor.hlsl load failed!");
		return false;
	}

	return true;
}

A3DPOST_EFFECT A3DPostAA::GetEffectType() const
{
	return A3DPOST_EFFECT_AA;
}

bool A3DPostAA::RenderForRT(A3DViewport* pViewport, A3DRenderTarget* pSrc, A3DRenderTarget* pDst)
{
    return DoRender(pViewport, pSrc, pDst);
}

A3DRenderTarget* A3DPostAA::ApplyEffect(A3DViewport* pViewport, A3DRenderTarget* pSrcRT)
{
    int iWidth = pSrcRT->GetWidth();
    int iHeight = pSrcRT->GetHeight();
	A3DDevice* pDev = m_pSystem->GetA3DDevice();

	//ASSERT(pDev && pSrc && pDst);

	//设置渲染状态
	pDev->SetAlphaBlendEnable(false);
	pDev->SetAlphaTestEnable(false);
	pDev->SetZTestEnable(false);
	pDev->SetZWriteEnable(false);
	pDev->SetFaceCull(A3DCULL_NONE);
	pDev->SetDeviceRenderState(D3DRS_COLORWRITEENABLE,
		D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN
		| D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);

	
	//当前采用SMAA版本的反走样算法. 
	{
		int rtWidth = pViewport->GetParam()->Width;
		int rtHeight = pViewport->GetParam()->Height;

		A3DRenderTarget*	m_pEdgeRT;
		A3DRenderTarget*	m_pBlendRT;
		A3DRenderTarget*	m_pStencilRT;

		m_pEdgeRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(iWidth, iHeight, A3DFMT_A8R8G8B8);
		m_pStencilRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetDepth(iWidth, iHeight, A3DFMT_D24S8);

	
		m_pStencilRT->ApplyToDevice();

		//Edge	
		m_pEdgeRT->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET | D3DCLEAR_STENCIL | D3DCLEAR_ZBUFFER, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

 		DWORD dwStencilPass = pDev->GetDeviceRenderState(D3DRS_STENCILPASS);
 		DWORD dwStencilFunc = pDev->GetDeviceRenderState(D3DRS_STENCILFUNC);
 		DWORD dwStencilRef = pDev->GetDeviceRenderState(D3DRS_STENCILREF);

 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, TRUE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_REPLACE);
		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_ALWAYS);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, 1);
		
		m_pSMAAEdgeDetect->Setup2f("SMAA_PIXEL_SIZE", 1.f / iWidth, 1.f / iHeight);

		m_pSMAAEdgeDetect->Appear();

		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		pSrcRT->AppearAsTexture(0);
		m_pSystem->GetLinearDepthRT()->AppearAsTexture(2);

		DrawQuad(iWidth, iHeight);

		m_pSMAAEdgeDetect->Disappear();
		m_pEdgeRT->WithdrawFromDevice();

        //Blend	
        m_pBlendRT = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(iWidth, iHeight, A3DFMT_A8R8G8B8);
		m_pBlendRT->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);

 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, TRUE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, D3DSTENCILOP_KEEP);
 		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, D3DCMP_EQUAL);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, 1);

		m_pSMAABlend->Setup2f("SMAA_PIXEL_SIZE", 1.f / iWidth, 1.f / iHeight);
		m_pSMAABlend->Appear();

		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

		m_pEdgeRT->AppearAsTexture(3);
		m_pAreaTex->Appear(5);
		m_pSearchTex->Appear(6);

        DrawQuad(iWidth, iHeight);

		m_pSMAABlend->Disappear();
		m_pBlendRT->WithdrawFromDevice();
        m_pEdgeRT->DisappearAsTexture(3);
        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pEdgeRT);
        m_pEdgeRT = NULL;

		//Neighbor
 		pDev->SetDeviceRenderState(D3DRS_STENCILENABLE, FALSE);
 		pDev->SetDeviceRenderState(D3DRS_STENCILPASS, dwStencilPass);
 		pDev->SetDeviceRenderState(D3DRS_STENCILFUNC, dwStencilFunc);
 		pDev->SetDeviceRenderState(D3DRS_STENCILREF, dwStencilRef);


        A3DRenderTarget* pDst = m_pSystem->GetA3DDevice()->GetRenderTargetMan()->RentRenderTargetColor(iWidth, iHeight, A3DFMT_A8R8G8B8);

		pDst->ApplyToDevice();
		pDev->Clear(D3DCLEAR_TARGET, D3DCOLOR_ARGB(0, 0, 0, 0), 1.0f, 0);
		m_pSMAANeighbor->Setup2f("SMAA_PIXEL_SIZE", 
			1.f / static_cast<float>(m_pSystem->GetWidth()), 1.f / static_cast<float>(m_pSystem->GetHeight()));
		m_pSMAANeighbor->Appear();
		pSrcRT->AppearAsTexture(0);
		m_pBlendRT->AppearAsTexture(4);
		
		for(int i = 0; i <=5; i++)
		{
			pDev->SetTextureFilterType(i, A3DTEXF_LINEAR);
			pDev->SetTextureAddress(i, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
		}
		pDev->SetTextureFilterType(6, A3DTEXF_POINT);
		pDev->SetTextureAddress(6, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

        DrawQuad(iWidth, iHeight);
	
		m_pSMAANeighbor->Disappear();
		pDst->WithdrawFromDevice();
		m_pStencilRT->WithdrawFromDevice();
        m_pBlendRT->DisappearAsTexture(4);

//#ifdef _DEBUG_POST_
//  		if(GetAsyncKeyState('2') & 0x8000)
// 		{
//  			D3DXSaveSurfaceToFileA("m_pEdgeRT.dds", D3DXIFF_DDS, m_pEdgeRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("m_pBlendRT.dds", D3DXIFF_DDS, m_pBlendRT->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("AA_Input.dds", D3DXIFF_DDS, pSrc->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  			D3DXSaveSurfaceToFileA("AA_Output.dds", D3DXIFF_DDS, pDst->GetTargetSurface()->m_pD3DSurface, NULL, NULL);
//  		}
//#endif

        m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(pSrcRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pBlendRT);
		m_pSystem->GetA3DDevice()->GetRenderTargetMan()->ReturnRenderTarget(m_pStencilRT);
		return pDst;
	}
}
