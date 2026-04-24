/*
 * FILE: EC_FullGlowRender.cpp
 *
 * DESCRIPTION: Fullscreen glow for the Element Client
 *
 * CREATED BY: Hedi, 2004/10/9 
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_FullGlowRender.h"
#include "EC_Global.h"
#include "EC_Game.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include <A3DTerrainWater.h>
#else
#include "A3DHLSL.h"
#include "ECViewport.h"
#include <A3DTerrainWater2.h>
#endif
#include "EC_Configs.h"
#include "EC_GameRun.h"
#include "EC_World.h"

#include <AChar.h>

#include <A3DCamera.h>
#include <A3DDevice.h>
#include <A3DEngine.h>
#include <A3DFuncs.h>
#include <A3DMacros.h>
#include <A3DShaderMan.h>
#include <A3DRenderTarget.h>
#include <A3DPixelShader.h>
#include <A3DStream.h>
#include <A3DTexture.h>
#include <A3DTextureMan.h>
#include <A3DVertex.h>
#include <A3DViewport.h>
#include <A3DVertexShader.h>

#include "A3DGfxExMan.h"
#include "A3DGFXElement.h"

#define new A_DEBUG_NEW

CECFullGlowRender::CECFullGlowRender()
{
	m_bRenderToBack		= false;

	m_bCanDoFullGlow	= false;
	m_bGlowOn			= false;
	m_bBloomOn			= false;
	m_bWarpOn			= true;
	m_bFlareOn			= false;
	m_bSharpenOn		= false;
	m_bBloomMotionBlur	= true;

	m_pRenderForBloom	= NULL;
	m_pRenderForBloomArg= NULL;

	m_pBackTarget		= NULL;
	m_pBloomTarget		= NULL;
	m_pGlowTarget1		= NULL;	
	m_pGlowTarget2		= NULL;	

	m_pGlow1Shader		= NULL;
	m_pGlow2Shader		= NULL;
	m_pBloom1Shader		= NULL;
	m_pBloom2Shader		= NULL;
	
	m_pStreamStretchCopy= NULL;
	m_pStreamBlur		= NULL;
	m_GlowRadius		= 9.0f;
	m_BloomRadius		= 10.0f;
	m_pGaussinBlurVS	= NULL;

	m_pBumpUnderWater	= NULL;

	m_glowType			= FULLGLOW_TYPE_NULL;
	m_color				= 0xffffffff;

	m_pTLWarpShader		= NULL;
	m_pTLWarpStream		= NULL;
	m_nMaxTLWarpVerts	= 0;
	m_nMaxTLWarpIndices	= 0;

	m_pWarpShader		= NULL;
	m_pWarpVertexShader	= NULL;
	m_pWarpStream		= NULL;
	m_nMaxWarpVerts		= 0;
	m_nMaxWarpIndices	= 0;

	m_pSharpenShader	= NULL;
	m_pStreamSharpen	= NULL;

	m_pPresenter		= NULL;

	m_bContrast	  = false;
	m_pContrastVS = NULL;
	m_pContrastPS = NULL;
	m_fContrastPower = 1.0f;
	m_fContrastMidLum = 0.5f;
}

CECFullGlowRender::~CECFullGlowRender()
{
	Release();
}

bool IsATIShittingCard(DWORD deviceID, DWORD vendorID)
{
	if( vendorID != 0x1002 )
		return false;

	switch(deviceID)
	{
	case 0x00004966: // 9000 SERIES, RV250
		return true;

	case 0x0000496E: // 9000 SERIES - Secondary, RV250
		return true;

	case 0x00005960:
	case 0x00005961: // 9200 SERIES, RV280
		return true;

	case 0x00005940:
	case 0x00005941: // 9200 SERIES - Secondary
		return true;

	case 0x00005144: // RADEON
		return true; 

	case 0x0000514C: // 8500
		return true;

	case 0x00004242: // 8500DV
		return true;

	case 0x00005157: // RADEON VE (7500)
		return true;

	case 0x00004C66:
	case 0x00004C6E: // FireMV 2400
		return true;

	case 0x00005964:
	case 0x00005D44: // 9200SE
		return true;

	case 0x0000514D: // 9100
		return true;

	case 0x00005C41:
	case 0x00005C61: // MOBILITY / RADEON 9200/9250 Series
		return true;

	case 0x00007834: // ATI RADEON 9000/9100  PRO IGP Series
		return true;

	case 0x00005834: // RADEON 9100 IGP
		return true;
	}

	return false;
}

bool CECFullGlowRender::Init(A3DDevice * pA3DDevice)
{
	m_bCanDoFullGlow	= false;
	m_bGlowOn			= false;
	m_bBloomOn			= false;
	m_bWarpOn			= false;
	m_bFlareOn			= false;
	m_pA3DDevice		= pA3DDevice;

	m_pPresenter = new CECFullGlowPresenter();

	m_bResourceLoaded = false;

#ifdef BACK_VERSION
	D3DADAPTER_IDENTIFIER8 idAdapter;
	pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, D3DENUM_NO_WHQL_LEVEL, &idAdapter);
#else
	D3DADAPTER_IDENTIFIER9 idAdapter;
	pA3DDevice->GetA3DEngine()->GetD3D()->GetAdapterIdentifier(D3DADAPTER_DEFAULT, 0/*D3DENUM_NO_WHQL_LEVEL*/, &idAdapter);
#endif
	if( IsATIShittingCard(idAdapter.DeviceId, idAdapter.VendorId) )
	{
		g_Log.Log("CECFullGlowRender::Init(), skip ATI low end series!");
	}
	else
	{
		if( !LoadResource() )
		{
			g_Log.Log("CECFullGlowRender::Init(), failed to create resource!");
			ReleaseResource();
		}
	}
	
	m_bGlowOn			= false;//m_bCanDoFullGlow;
	m_bBloomOn			= m_bCanDoFullGlow;
	m_bWarpOn			= true;//m_bCanDoFullGlow;
	m_bFlareOn			= m_bCanDoFullGlow;
	m_bSharpenOn		= false;//m_bCanDoFullGlow;
	
	return true;
}

bool CECFullGlowRender::Release()
{
	ReleaseResource();

	if( m_pPresenter )
	{
		delete m_pPresenter;
		m_pPresenter = NULL;
	}

	return true;
}

bool CECFullGlowRender::LoadResource()
{
	if( m_pA3DDevice->GetDevFormat().fmtTarget != A3DFMT_A8R8G8B8 )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), we need device target to be A8R8G8B8 format!");
		return false;
	}

	m_nWidth			= m_pA3DDevice->GetDevFormat().nWidth;
	m_nHeight			= m_pA3DDevice->GetDevFormat().nHeight;

	CalculateGaussian(m_GlowRadius, m_GlowWeights);
	CalculateGaussian(m_BloomRadius, m_BloomWeights);

#ifdef BACK_VERSION
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= true;
	devFmt.nWidth		= m_nWidth;
	devFmt.nHeight		= m_nHeight;
	devFmt.fmtTarget	= A3DFMT_A8R8G8B8;
	devFmt.fmtDepth		= A3DFMT_D24X8;
#else
	A3DRenderTarget::RTFMT devFmt;
	devFmt.iWidth		= m_nWidth;
	devFmt.iHeight		= m_nHeight;
	devFmt.fmtTarget	= A3DFMT_A8R8G8B8;
	devFmt.fmtDepth		= A3DFMT_D24X8;
	//A3DDEVFMT devFmt;
	//devFmt.bWindowed	= true;
#endif

	m_pBackTarget = new A3DRenderTarget();
	if( !m_pBackTarget->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create glow back target!");
		return false;
	}

	m_pBloomTarget = new A3DRenderTarget();
	if( !m_pBloomTarget->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create glow back target!");
		return false;
	}

#ifdef BACK_VERSION
	devFmt.nWidth		= m_nWidth / 4;
	devFmt.nHeight		= m_nHeight / 4;
#else
	devFmt.iWidth		= m_nWidth / 4;
	devFmt.iHeight		= m_nHeight / 4;
#endif
	m_pGlowTarget1 = new A3DRenderTarget();
	if( !m_pGlowTarget1->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create glow render target1!");
		return false;
	}
#ifdef BACK_VERSION
	devFmt.nWidth		= m_nWidth / 4;
	devFmt.nHeight		= m_nHeight / 4;
#else
	devFmt.iWidth		= m_nWidth / 4;
	devFmt.iHeight		= m_nHeight / 4;
#endif
	m_pGlowTarget2 = new A3DRenderTarget();
	if( !m_pGlowTarget2->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create glow render target2!");
		return false;
	}

	// now load pixel shader for render glow1
	m_pGlow1Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\fullglow1_14.txt", false);
	if( NULL == m_pGlow1Shader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load fullglow1.txt");
		return false;
	}
	// now load pixel shader for render glow2
	m_pGlow2Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\fullglow2_14.txt", false);
	if( NULL == m_pGlow2Shader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load fullglow2.txt");
		return false;
	}
	// now load pixel shader for render bloom 1
	m_pBloom1Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\bloom1_14.txt", false);
	if( NULL == m_pBloom1Shader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load bloom1_14.txt");
		return false;
	}
	// now load pixel shader for render bloom 2
	m_pBloom2Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\bloom2_14.txt", false);
	if( NULL == m_pBloom2Shader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load bloom2_14.txt");
		return false;
	}
	
	// now create streams
	m_pStreamStretchCopy = new A3DStream();
	if( !m_pStreamStretchCopy->Init(m_pA3DDevice, A3DVT_TLVERTEX, 4, 6, A3DSTRM_REFERENCEPTR, A3DSTRM_REFERENCEPTR) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create stretch copy stream!");
		return false;
	}

	m_pStreamBlur = new A3DStream();
	if( !m_pStreamBlur->Init(m_pA3DDevice, sizeof(A3DGLOWVERTEX), A3DGLOWVERT_FVF, 4, 6, A3DSTRM_REFERENCEPTR, A3DSTRM_REFERENCEPTR) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create blur x stream!");
		return false;
	}

	m_indices[0] = 0; m_indices[1] = 1; m_indices[2] = 2;
	m_indices[3] = 2; m_indices[4] = 1; m_indices[5] = 3;

	int w = m_nWidth / 4;
	int h = m_nHeight / 4;

	m_vertsStretchCopy[0] = A3DTLVERTEX(A3DVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f), 0xffffffff, 0xff000000, 0.0f, 0.0f);
	m_vertsStretchCopy[1] = A3DTLVERTEX(A3DVECTOR4((float)w - 0.5f, -0.5f, 0.0f, 1.0f), 0xffffffff, 0xff000000, 1.0f, 0.0f);
	m_vertsStretchCopy[2] = A3DTLVERTEX(A3DVECTOR4(-0.5f, (float)h - 0.5f, 0.0f, 1.0f), 0xffffffff, 0xff000000, 0.0f, 1.0f);
	m_vertsStretchCopy[3] = A3DTLVERTEX(A3DVECTOR4((float)w - 0.5f, (float)h - 0.5f, 0.0f, 1.0f), 0xffffffff, 0xff000000, 1.0f, 1.0f);
	m_pStreamStretchCopy->SetVerts((BYTE *) m_vertsStretchCopy, 4);
	m_pStreamStretchCopy->SetVertexRef((BYTE *) m_vertsStretchCopy);
	m_pStreamStretchCopy->SetIndices((BYTE*) m_indices, 6);
	m_pStreamStretchCopy->SetIndexRef((void *) m_indices);

	m_vertsBlur[0] = A3DGLOWVERTEX(A3DVECTOR4(-1.0f, 1.0f, 0.0f, 1.0f), 0xffffffff, 0.0f, 0.0f);
	m_vertsBlur[1] = A3DGLOWVERTEX(A3DVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), 0xffffffff, 1.0f, 0.0f);
	m_vertsBlur[2] = A3DGLOWVERTEX(A3DVECTOR4(-1.0f, -1.0f, 0.0f, 1.0f), 0xffffffff, 0.0f, 1.0f);
	m_vertsBlur[3] = A3DGLOWVERTEX(A3DVECTOR4(1.0f, -1.0f, 0.0f, 1.0f), 0xffffffff, 1.0f, 1.0f);
	m_pStreamBlur->SetVerts((BYTE *) m_vertsBlur, 4);
	m_pStreamBlur->SetVertexRef((BYTE *) m_vertsBlur);
	m_pStreamBlur->SetIndices((BYTE*) m_indices, 6);
	m_pStreamBlur->SetIndexRef((void *) m_indices);

	// now load vertex shader for blur stream
#ifdef BACK_VERSION
	DWORD dwVSDecl[32];
	int n = 0;				
	dwVSDecl[n ++] = D3DVSD_STREAM(0);					// begin tokens.
	dwVSDecl[n ++] = D3DVSD_REG(0, D3DVSDT_FLOAT4);		// x, y, z and w in cuboid space
	dwVSDecl[n ++] = D3DVSD_REG(1, D3DVSDT_D3DCOLOR);	// diffuse color
	dwVSDecl[n ++] = D3DVSD_REG(2, D3DVSDT_FLOAT2);		// u, v texture coord
	dwVSDecl[n ++] = D3DVSD_END();						// end tokens.
	m_pGaussinBlurVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\gaussin_blur_vs.txt", false, dwVSDecl);
#else
	static D3DVERTEXELEMENT9 aVertexShaderDecl[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
		{0, 16, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		D3DDECL_END()
	};
#endif
	if( NULL == m_pGaussinBlurVS )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load gaussin_blur_vs.txt");
		return false;
	}

	// Init contrast shader
#ifdef BACK_VERSION
	m_pContrastPS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\contrast_ps.txt", false);
#else
	m_pContrastVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\contrast_vs.txt", false, aVertexShaderDecl);
#endif
	if( NULL == m_pContrastPS )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load contrast_ps.txt");
		return false;
	}
	
#ifdef BACK_VERSION
	m_pContrastVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\contrast_vs.txt", false, dwVSDecl);
#else
	m_pContrastVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\contrast_vs.txt", false, aVertexShaderDecl);
#endif
	if( NULL == m_pContrastVS )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load contrast_vs.txt");
		return false;
	}


	// now load resource for space warps.
	m_nMaxWarpVerts = 1000;
	m_nMaxWarpIndices = 1500;
	m_pWarpStream = new A3DStream();
	if( !m_pWarpStream->Init(m_pA3DDevice, sizeof(A3DWARPVERTEX), A3DFVF_A3DWARPVERT, m_nMaxWarpVerts, m_nMaxWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create warp stream!");
		return false;
	}

	WORD * pIndices;
	if( !m_pWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to lock warp stream's index buffer!");
		return false;
	}
	int i;
	for(i=0; i<250; i++)
	{
		pIndices[i * 6 + 0] = i * 4 + 0;		
		pIndices[i * 6 + 1] = i * 4 + 1;		
		pIndices[i * 6 + 2] = i * 4 + 2;		
		pIndices[i * 6 + 3] = i * 4 + 2;		
		pIndices[i * 6 + 4] = i * 4 + 1;		
		pIndices[i * 6 + 5] = i * 4 + 3;		
	}
	m_pWarpStream->UnlockIndexBuffer();
	
	m_pWarpShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\spacewarp.txt", false);
	if( NULL == m_pWarpShader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load spacewarp.txt");
		return false;
	}

	// now load vertex shader for blur stream
#ifdef BACK_VERSION
	n = 0;				
	dwVSDecl[n ++] = D3DVSD_STREAM(0);					// begin tokens.
	dwVSDecl[n ++] = D3DVSD_REG(0, D3DVSDT_FLOAT3);		// x, y, z 
	dwVSDecl[n ++] = D3DVSD_REG(1, D3DVSDT_FLOAT2);		// u, v texture coord
	dwVSDecl[n ++] = D3DVSD_REG(2, D3DVSDT_FLOAT2);		// u, v texture coord
	dwVSDecl[n ++] = D3DVSD_REG(3, D3DVSDT_FLOAT2);		// u, v texture coord
	dwVSDecl[n ++] = D3DVSD_END();						// end tokens.
	m_pWarpVertexShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\warp3d_vs.txt", false, dwVSDecl);
#else
	static D3DVERTEXELEMENT9 aVertexShaderDecl2[] =
	{
		{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
		{0, 12, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
		{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 1},
		{0, 28, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 2},
		D3DDECL_END()
	};
	
	m_pWarpVertexShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\warp3d_vs.txt", false, aVertexShaderDecl2);
#endif
	if( NULL == m_pWarpVertexShader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load warp_3d_vs.txt");
		return false;
	}

	// now load resource for TL space warps.
	m_nMaxTLWarpVerts = 1000;
	m_nMaxTLWarpIndices = 1500;
	m_pTLWarpStream = new A3DStream();
	if( !m_pTLWarpStream->Init(m_pA3DDevice, sizeof(A3DTLWARPVERTEX), A3DFVF_A3DTLWARPVERT, m_nMaxTLWarpVerts, m_nMaxTLWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create blur y stream!");
		return false;
	}

	if( !m_pTLWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to lock warp stream's index buffer!");
		return false;
	}
	for(i=0; i<250; i++)
	{
		pIndices[i * 6 + 0] = i * 4 + 0;		
		pIndices[i * 6 + 1] = i * 4 + 1;		
		pIndices[i * 6 + 2] = i * 4 + 2;		
		pIndices[i * 6 + 3] = i * 4 + 2;		
		pIndices[i * 6 + 4] = i * 4 + 1;		
		pIndices[i * 6 + 5] = i * 4 + 3;		
	}
	m_pTLWarpStream->UnlockIndexBuffer();
	
	m_pTLWarpShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\spacewarp_tl.txt", false);
	if( NULL == m_pTLWarpShader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load spacewarp_tl.txt");
		return false;
	}

	if( !CreateBumpMap() )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create bump map for under water");
		return false;
	}

	// now load resource for sharpen
	m_pSharpenShader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader("Shaders\\ps\\fullsharpen_14.txt", false);
	if( NULL == m_pSharpenShader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load fullsharpen_14.txt");
		return false;
	}

	m_pStreamSharpen = new A3DStream();
	if( !m_pStreamSharpen->Init(m_pA3DDevice, sizeof(A3DGLOWVERTEX), A3DGLOWVERT_FVF, 4, 6, A3DSTRM_STATIC, A3DSTRM_STATIC) )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to create sharpen stream!");
		return false;
	}

	float du = 1.0f / m_nWidth;
	float dv = 1.0f / m_nHeight;
	A3DGLOWVERTEX sharpenVerts[4];
	sharpenVerts[0] = A3DGLOWVERTEX(A3DVECTOR4(-0.5f, -0.5f, 0.0f, 1.0f), 0xffffffff, 0.0f, 0.0f);
	sharpenVerts[1] = A3DGLOWVERTEX(A3DVECTOR4((float)m_nWidth - 0.5f, -0.5f, 0.0f, 1.0f), 0xffffffff, 1.0f, 0.0f);
	sharpenVerts[2] = A3DGLOWVERTEX(A3DVECTOR4(-0.5f, (float)m_nHeight - 0.5f, 0.0f, 1.0f), 0xffffffff, 0.0f, 1.0f);
	sharpenVerts[3] = A3DGLOWVERTEX(A3DVECTOR4((float)m_nWidth - 0.5f, (float)m_nHeight - 0.5f, 0.0f, 1.0f), 0xffffffff, 1.0f, 1.0f);
	m_pStreamSharpen->SetVerts((BYTE *) sharpenVerts, 4);
	m_pStreamSharpen->SetIndices((BYTE*) m_indices, 6);

	m_pPresenter->SetData(m_pGlow1Shader, m_pGlow2Shader);

	m_bCanDoFullGlow	= true;
	m_bResourceLoaded	= true;


	
	return true;
}

bool CECFullGlowRender::ReleaseResource()
{
	if( m_pPresenter )
		m_pPresenter->SetData(NULL, NULL);

	if( m_pStreamStretchCopy )
	{
		m_pStreamStretchCopy->Release();
		delete m_pStreamStretchCopy;
		m_pStreamStretchCopy = NULL;
	}

	if( m_pGaussinBlurVS )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleaseVertexShader(&m_pGaussinBlurVS);
		m_pGaussinBlurVS = NULL;
	}

	if( m_pStreamBlur )
	{
		m_pStreamBlur->Release();
		delete m_pStreamBlur;
		m_pStreamBlur = NULL;
	}

	if( m_pGlowTarget1 )
	{
		m_pGlowTarget1->Release();
		delete m_pGlowTarget1;
		m_pGlowTarget1 = NULL;
	}

	if( m_pGlowTarget2 )
	{
		m_pGlowTarget2->Release();
		delete m_pGlowTarget2;
		m_pGlowTarget2 = NULL;
	}

	if( m_pBloomTarget )
	{
		m_pBloomTarget->Release();
		delete m_pBloomTarget;
		m_pBloomTarget = NULL;
	}

	if( m_pBackTarget )
	{
		m_pBackTarget->Release();
		delete m_pBackTarget;
		m_pBackTarget = NULL;
	}

	if( m_pGlow1Shader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pGlow1Shader);
		m_pGlow1Shader = NULL;
	}

	if( m_pGlow2Shader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pGlow2Shader);
		m_pGlow2Shader = NULL;
	}

	if( m_pBloom1Shader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pBloom1Shader);
		m_pBloom1Shader = NULL;
	}

	if( m_pBloom2Shader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pBloom2Shader);
		m_pBloom2Shader = NULL;
	}

	if( m_pWarpVertexShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleaseVertexShader(&m_pWarpVertexShader);
		m_pWarpVertexShader = NULL;
	}

	if( m_pWarpShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pWarpShader);
		m_pWarpShader = NULL;
	}

	if( m_pWarpStream )
	{
		m_pWarpStream->Release();
		delete m_pWarpStream;
		m_pWarpStream = NULL;
	}

	if( m_pTLWarpShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pTLWarpShader);
		m_pTLWarpShader = NULL;
	}

	if( m_pTLWarpStream )
	{
		m_pTLWarpStream->Release();
		delete m_pTLWarpStream;
		m_pTLWarpStream = NULL;
	}

	if( m_pBumpUnderWater )
	{
		m_pBumpUnderWater->Release();
		delete m_pBumpUnderWater;
		m_pBumpUnderWater = NULL;
	}

	if( m_pSharpenShader )
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pSharpenShader);
		m_pSharpenShader = NULL;
	}

	if( m_pStreamSharpen )
	{
		m_pStreamSharpen->Release();
		delete m_pStreamSharpen;
		m_pStreamSharpen = NULL;
	}

	if( m_pContrastVS)
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleaseVertexShader(&m_pContrastVS);
		m_pContrastVS = NULL;
	}
	if( m_pContrastPS)
	{
		m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pContrastPS);
		m_pContrastVS = NULL;
	}

	m_bResourceLoaded = false;
	return true;
}



bool CECFullGlowRender::BeginGlow()
{
	if( !m_bCanDoFullGlow || !m_bResourceLoaded )
		return true;

	if( m_pA3DDevice->GetDevFormat().fmtTarget != A3DFMT_A8R8G8B8 )
		m_bRenderToBack = false;
	else
		m_bRenderToBack = true;
	
	return true;
}

void CECFullGlowRender::CalculateGaussian(float radius, float weights[3])
{
	double	size = radius * 2.0f;
	double	sigma2;
    int		i;
	double	sum;
    
    sigma2 = -(size + 1) * (size + 1) / log(1.0 / 255.0);
    
    static int t[3] = {-2, -1, 0};
	static int n[3] = {2, 2, 1};
    for(i=0; i<3; i++)
	{
		double f = radius * t[i] / 3.0f;
	    weights[i] = (float)exp(-(f * f) / sigma2);
	}

	sum = 0.0f;
	for(i=0; i<3; i++)
		sum += weights[i] * n[i];
	double rh = 1.0f / sum;
	for(i=0; i<3; i++)
		weights[i] = (float)(weights[i] * rh);
}

bool CECFullGlowRender::Update(int nDeltaTime)
{
	// update parameter for glow, so if glow is not on, we need not to update that
	if( !m_bResourceLoaded || !m_bCanDoFullGlow )
		return true;

	m_pPresenter->Update(nDeltaTime);

	return true;
}

bool CECFullGlowRender::EndGlow(A3DViewport * pViewport)
{
	if( !m_bCanDoFullGlow || !m_bResourceLoaded || !m_bRenderToBack )
		return true;

	// move content from back buffer to glow target;
#ifdef BACK_VERSION
	g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBackTarget->GetTargetSurface(), NULL);
#else
	g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBackTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif

	if( m_bWarpOn )
	{
		bool bHasDoneWarp = DoWarps(pViewport);

		if( bHasDoneWarp )
		{
			if( m_bBloomOn || m_bGlowOn || m_bSharpenOn )
			{
				// then move content from back to glow target again;
#ifdef BACK_VERSION
				g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBackTarget->GetTargetSurface(), NULL);
#else
				g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBackTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif
			}
		}
	}

	if( m_bBloomOn )
	{
		DoBloom(pViewport);

		if(m_bContrast)
		{
			DoContrast();
		}

		if( m_bGlowOn || m_bSharpenOn )
		{
			// then move content from back to glow target again;
#ifdef BACK_VERSION
			g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBackTarget->GetTargetSurface(), NULL);
#else
			g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBackTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif
		}
	}

	// glow and sharpen will not work together
	if( m_bGlowOn )
	{
		DoGlow(pViewport);
	}
	else if( m_bSharpenOn )
	{
		DoSharpen(pViewport);
	}

	return true;

}

bool CECFullGlowRender::DoGlow(A3DViewport * pViewport)
{
	m_pPresenter->SetData(m_pGlow1Shader, m_pGlow2Shader);
	
	A3DVIEWPORTPARAM param;
	param.X = 0;
	param.Y = 0;
	param.Width = m_nWidth / 4;
	param.Height = m_nHeight / 4;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&param);

	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);

	// first blur 1 from target1 to target2
#ifdef BACK_VERSION
	m_pA3DDevice->SetRenderTarget(m_pGlowTarget1);
#else
	m_pGlowTarget1->ApplyToDevice();
#endif

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(4, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(5, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(5, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pBackTarget->GetTargetTexture());
#else
	m_pBackTarget->AppearAsTexture(0);
	m_pBackTarget->AppearAsTexture(1);
	m_pBackTarget->AppearAsTexture(2);
	m_pBackTarget->AppearAsTexture(3);
	m_pBackTarget->AppearAsTexture(4);
	m_pBackTarget->AppearAsTexture(5);
#endif

	m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_ZERO);
	m_pPresenter->PrepareGlow1Pass(m_GlowWeights);
	m_pStreamBlur->Appear();
	m_pGaussinBlurVS->Appear();
	SetupVertexConsts(-1.0f/(m_nWidth/4), 1.0f/(m_nHeight/4), m_GlowRadius/3.0f/m_nWidth, 0.0f);
	SetupPixelConsts();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);

	m_pPresenter->AfterGlow1Pass();
#ifdef BACK_VERSION
	m_pA3DDevice->RestoreRenderTarget();
#else
	m_pGlowTarget1->WithdrawFromDevice();
#endif
	g_pGame->GetViewport()->GetA3DViewport()->Active();
	
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	m_pPresenter->PrepareGlow2Pass(m_color, m_GlowWeights);
#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pGlowTarget1->GetTargetTexture());
#else
	m_pBackTarget->AppearAsTexture(0);
	m_pGlowTarget1->AppearAsTexture(1);
	m_pGlowTarget1->AppearAsTexture(2);
	m_pGlowTarget1->AppearAsTexture(3);
	m_pGlowTarget1->AppearAsTexture(4);
	m_pGlowTarget1->AppearAsTexture(5);
#endif
	SetupVertexConsts(-1.0f/m_nWidth, 1.0f/m_nHeight, 0.0f, m_GlowRadius/3.0f/m_nHeight);
	SetupPixelConsts();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->ClearTexture(1);
	m_pA3DDevice->ClearTexture(2);
	m_pA3DDevice->ClearTexture(3);
	m_pA3DDevice->ClearTexture(4);
	m_pA3DDevice->ClearTexture(5);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(5, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pPresenter->AfterGlow2Pass();	
	
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);

	return true;
}

bool CECFullGlowRender::DoContrast()
{
	//copy frame buffer
#ifdef BACK_VERSION
	g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBackTarget->GetTargetSurface(), NULL);
#else
	g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBackTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif

	//set render state
	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);
	m_pA3DDevice->SetAlphaBlendEnable(false);

	//set texture state
#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBackTarget->GetTargetTexture());
#else
	m_pBackTarget->AppearAsTexture(0);
#endif
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

	//set shaders and stream
	m_pStreamBlur->Appear(0, false);
	m_pContrastVS->Appear();
	m_pContrastPS->Appear();

	//set VS constants
	A3DVECTOR4 c0(-1.0f/m_nWidth, 1.0f/m_nHeight, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(0, &c0, 1);

	A3DVECTOR4 c1(m_fContrastPower, m_fContrastPower / m_fContrastMidLum, m_fContrastPower / (1.0f - m_fContrastMidLum), 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(1, &c1, 1);

	//set PS constants
	c1 = A3DVECTOR4(m_fContrastMidLum, 0.0f, 0.0f, 0.0f);
	m_pA3DDevice->SetPixelShaderConstants(1, &c1, 1);
	
	//draw
	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);


	//restore 
	m_pContrastVS->Disappear();
	m_pContrastPS->Disappear();

	//restore texture state
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);

	//restore render state
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetAlphaBlendEnable(true);


	return true;
}

bool CECFullGlowRender::DoBloom(A3DViewport * pViewport)
{
	if( !m_pRenderForBloom )
		return true;

	m_pPresenter->SetData(m_pBloom1Shader, m_pBloom2Shader);

	// first of all clear backbuffer to be pure black, and not clear z buffer
	g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0, 1.0f, 0);

	(*m_pRenderForBloom)(pViewport, m_pRenderForBloomArg);

	// then copy back buffer to bloom texture
#ifdef BACK_VERSION
	g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBloomTarget->GetTargetSurface(), NULL);
#else
	g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBloomTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif

	A3DVIEWPORTPARAM param;
	param.X = 0;
	param.Y = 0;
	param.Width = m_nWidth / 4;
	param.Height = m_nHeight / 4;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&param);

	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);

	// then blur 1 from target1 to target2
#ifdef BACK_VERSION
	m_pA3DDevice->SetRenderTarget(m_pGlowTarget2);
#else
	m_pGlowTarget2->ApplyToDevice();
#endif

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(4, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(5, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(5, A3DTADDR_CLAMP, A3DTADDR_CLAMP);

#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pBloomTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pBloomTarget->GetTargetTexture());
#else
	for(int i = 0; i <= 5; i++)
		m_pBloomTarget->AppearAsTexture(i);
#endif
	
	if( m_pGlowTarget2->HasFilled() && m_bBloomMotionBlur )
	{
		// consider motion blur, which can make a trail
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	}
	else
	{
		// not filled target yet, so just copy to it
		m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
		m_pA3DDevice->SetDestAlpha(A3DBLEND_ZERO);
		m_pGlowTarget2->SetHasFilled();
	}
	m_pPresenter->PrepareGlow1Pass(m_BloomWeights);
	m_pStreamBlur->Appear();
	m_pGaussinBlurVS->Appear();
	SetupVertexConsts(-1.0f/(m_nWidth/4), 1.0f/(m_nHeight/4), m_BloomRadius/2.0f/m_nWidth, 0.0f);
	SetupPixelConsts();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);

	m_pPresenter->AfterGlow1Pass();
#ifdef BACK_VERSION
	m_pA3DDevice->RestoreRenderTarget();
#else
	m_pGlowTarget2->WithdrawFromDevice();
#endif
	g_pGame->GetViewport()->GetA3DViewport()->Active();
	
	m_pA3DDevice->SetAlphaBlendEnable(false);
	m_pPresenter->PrepareGlow2Pass(m_color, m_BloomWeights);
#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pGlowTarget2->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pGlowTarget2->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pGlowTarget2->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pGlowTarget2->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pGlowTarget2->GetTargetTexture());
#else
	m_pBackTarget->AppearAsTexture(0);
	for(int i = 1; i <= 5; i++)
		m_pGlowTarget2->AppearAsTexture(i);
#endif
	SetupVertexConsts(-1.0f/m_nWidth, 1.0f/m_nHeight, 0.0f, m_BloomRadius/2.0f/m_nHeight);
	SetupPixelConsts();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);

	m_pA3DDevice->SetAlphaBlendEnable(true);
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->ClearTexture(1);
	m_pA3DDevice->ClearTexture(2);
	m_pA3DDevice->ClearTexture(3);
	m_pA3DDevice->ClearTexture(4);
	m_pA3DDevice->ClearTexture(5);
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(5, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pPresenter->AfterGlow2Pass();	
	
	m_pGaussinBlurVS->Disappear();
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	return true;
}

bool CECFullGlowRender::DoWarps(A3DViewport * pViewport)
{
	bool bUnderWater = false;
	if( g_pGame->GetGameRun()->GetWorld()->GetTerrainWater() )
	{
		bUnderWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater()->IsUnderWater(g_pGame->GetViewport()->GetA3DCamera()->GetPos());
	}

	A3DGFXExMan * pGfxMan = g_pGame->GetA3DGFXExMan();
	GfxElementList& elementList = pGfxMan->GetWarpEleList();

	if( !bUnderWater && elementList.size() == 0 )
		return false;

#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pBackTarget->GetTargetTexture());
#else
	m_pBackTarget->AppearAsTexture(1);
#endif
	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	A3DCULLTYPE oldCull = m_pA3DDevice->GetFaceCull();
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);

	
	pGfxMan->LockWarpEleList();

	// first TL space warps.
	m_pTLWarpStream->Appear();
	m_pTLWarpShader->Appear();
	
	if( bUnderWater )
	{
		A3DTLWARPVERTEX * pVerts;
		if( !m_pTLWarpStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, 0) )
		{
			return false;
		}

		float w = m_nWidth * 1.0f;
		float h = m_nHeight * 1.0f;

		float	xt = 3;
		float	yt = m_nHeight * xt / m_nWidth;
		static float yd = 0.0f;
		yd += g_pGame->GetTickTime() * 0.001f * 0.5f;
		if( yd > 1.0f ) yd -= 1.0f;

		float	xw = 0.005f;
		float	yw = m_nHeight * xw / m_nHeight;
		pVerts[0] = A3DTLWARPVERTEX(A3DVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), 0.0f, yd, 0.0f, 0.0f, xw, yw);
		pVerts[1] = A3DTLWARPVERTEX(A3DVECTOR4(w, 0.0f, 0.0f, 1.0f), xt, yd, 1.0f, 0.0f, xw, yw);
		pVerts[2] = A3DTLWARPVERTEX(A3DVECTOR4(0.0f, h, 0.0f, 1.0f), 0.0f, yt + yd, 0.0f, 1.0f, xw, yw);
		pVerts[3] = A3DTLWARPVERTEX(A3DVECTOR4(w, h, 0.0f, 1.0f), xt, yt + yd, 1.0f, 1.0f, xw, yw);

		m_pTLWarpStream->UnlockVertexBuffer();

		m_pBumpUnderWater->Appear(0);
		m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
		m_pBumpUnderWater->Disappear(0);

#ifdef BACK_VERSION
		g_pGame->GetA3DDevice()->GetD3DDevice()->CopyRects(g_pGame->GetA3DDevice()->GetBackBuffer(), NULL, 0, m_pBackTarget->GetTargetSurface(), NULL);
#else
		g_pGame->GetA3DDevice()->StretchRect(g_pGame->GetA3DDevice()->GetBackBuffer()->m_pD3DSurface, NULL, m_pBackTarget->GetTargetSurface()->m_pD3DSurface, NULL, A3DTEXF_LINEAR);
#endif
	}

	int		i;
	float	rw = 1.0f / m_nWidth;
	float	rh = 1.0f / m_nHeight;

	for(i=0; i<(int)elementList.size(); i++)
	{
		A3DGFXElement * pElement = elementList[i];
		if( pElement && pElement->IsTLVert() )
		{
			A3DTLWARPVERTEX * pVerts;
			if( !m_pTLWarpStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, 0) )
			{
				break;
			}
			int nVerts = pElement->DrawToBuffer(pViewport, pVerts, m_nMaxTLWarpVerts, rw, rh);
			m_pTLWarpStream->UnlockVertexBuffer();

			pElement->GetTexture()->Appear(0);
			m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
			pElement->GetTexture()->Disappear(0);
		}
	}
	
	m_pTLWarpShader->Disappear();

	A3DCameraBase * pCamera = pViewport->GetCamera();

	// then do 3d warps.
	m_pWarpStream->Appear();
	m_pWarpShader->Appear();
	m_pWarpVertexShader->Appear();

	// set vertex shader constants here
	A3DMATRIX4 matVP = m_pA3DDevice->GetViewMatrix() * m_pA3DDevice->GetProjectionMatrix();
	matVP.Transpose();
	m_pA3DDevice->SetVertexShaderConstants(0, &matVP, 4);

	A3DVECTOR4 c5(1.0f, 0.0f, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(5, &c5, 1);

	A3DMATRIX4 matScale = IdentityMatrix();
	matScale._11 = 0.5f;
	matScale._22 = -0.5f;
	matScale._41 = 0.5f;
	matScale._42 = 0.5f;
	A3DMATRIX4 matProjectedView;
	matProjectedView = pViewport->GetCamera()->GetVPTM() * matScale;
	matProjectedView.Transpose();
	m_pA3DDevice->SetVertexShaderConstants(6, &matProjectedView, 4);
	
	for(i=0; i<(int)elementList.size(); i++)
	{
		A3DGFXElement * pElement = elementList[i];
		if( pElement && !pElement->IsTLVert() )
		{
			A3DWARPVERTEX * pVerts;
			if( !m_pWarpStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, 0) )
			{
				break;
			}
			int nVerts = pElement->DrawToBuffer(pViewport, pVerts, m_nMaxWarpVerts);
			m_pWarpStream->UnlockVertexBuffer();

			pElement->GetTexture()->Appear(0);
			m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
			pElement->GetTexture()->Disappear(0);
		}
	}
	m_pWarpVertexShader->Disappear();
	m_pWarpShader->Disappear();
	
	pGfxMan->UnlockWarpEleList();
	pGfxMan->ClearWarpEleList();

	// restore render states.
	m_pA3DDevice->ClearTexture(1);
	m_pA3DDevice->SetFaceCull(oldCull);
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	return true;
}

bool CECFullGlowRender::DoSharpen(A3DViewport * pViewport)
{
	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);

	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_POINT);
	m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(2, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(3, A3DTEXF_LINEAR);
	m_pA3DDevice->SetTextureFilterType(4, A3DTEXF_LINEAR);
	
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	
#ifdef BACK_VERSION
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pBackTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pBackTarget->GetTargetTexture());
#else
	for(int i = 0; i <= 4; i++)
		m_pBackTarget->AppearAsTexture(i);
#endif
	
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_ZERO);
	m_pStreamSharpen->Appear();
	m_pSharpenShader->Appear();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);

	m_pSharpenShader->Disappear();
	m_pA3DDevice->ClearTexture(0);
	m_pA3DDevice->ClearTexture(1);
	m_pA3DDevice->ClearTexture(2);
	m_pA3DDevice->ClearTexture(3);
	m_pA3DDevice->ClearTexture(4);
	m_pA3DDevice->SetTextureAddress(0, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(2, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(3, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureAddress(4, A3DTADDR_WRAP, A3DTADDR_WRAP);
	m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
	
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);

	return true;
}

bool CECFullGlowRender::SetGlowType(FULLGLOW_TYPE type, A3DCOLOR color)
{
	if( !m_bGlowOn || !m_bCanDoFullGlow )
		return true;

	if( m_glowType == type && m_color == color )
		return true;

	m_glowType = type;
	m_color = color;

	switch(m_glowType)
	{
	case FULLGLOW_TYPE_DAYLIGHT:
		m_pPresenter->StopFade();
		m_pPresenter->SetGlowLevel(-1.0f, 0.25f, 5000);
		m_pPresenter->SetGlowPower(-1.0f, 0.25f, 5000);
		break;

	case FULLGLOW_TYPE_NIGHT:
		m_pPresenter->StopFade();
		m_pPresenter->SetGlowLevel(-1.0f, 0.25f, 5000);
		m_pPresenter->SetGlowPower(-1.0f, 0.25f, 5000);
		break;

	case FULLGLOW_TYPE_UNDERWATER:
		m_pPresenter->StopFade();
		m_pPresenter->SetGlowLevel(-1.0f, 0.5f, 2000);
		m_pPresenter->SetGlowPower(-1.0f, 0.3f, 2000);
		break;

	case FULLGLOW_TYPE_DEAD:
		m_pPresenter->SetGlowLevel(-1.0f, 0.6f, 2000);
		m_pPresenter->SetGlowPower(-1.0f, 0.4f, 2000);
		break;

	case FULLGLOW_TYPE_LOGIN:
		m_pPresenter->StopFade();
		m_pPresenter->SetGlowLevel(-1.0f, 0.25f, 1000);
		m_pPresenter->SetGlowPower(-1.0f, 0.25f, 1000);
		break;
	}

	return true;
}

void CECFullGlowRender::SetFade(float vStart, float vEnd, int nTime)
{
	if( !m_bGlowOn || !m_bCanDoFullGlow )
		return;

	m_pPresenter->SetFade(vStart, vEnd, nTime);
}

void CECFullGlowRender::SetMotionBlur(float vStart, float vEnd, int nTime)
{
	// currently we render to device back buffer directly, so motion blur can not be achieved without extra processing
	return;

	if( !m_bGlowOn || !m_bCanDoFullGlow )
		return;

	m_pPresenter->SetMotionBlur(vStart, vEnd, nTime);
}

bool CECFullGlowRender::SetGlowOn(bool bFlag)
{
	if( !m_bCanDoFullGlow )
		return true;

	if( m_bGlowOn == bFlag )
		return true;

	m_bGlowOn = bFlag;

	if( m_bGlowOn )
	{
		if( !m_bResourceLoaded && !LoadResource() )
		{
			ReleaseResource();
			m_bGlowOn = false;
			return false;
		}
	}
	else
	{
		if( m_bResourceLoaded && !GetNeedResource() )
			ReleaseResource();
	}

	return true;
}

bool CECFullGlowRender::SetBloomOn(bool bFlag, LPRENDERFORBLOOM pRenderForBloom, void * pArg)
{
	if( !m_bCanDoFullGlow )
		return true;

	m_pRenderForBloom = pRenderForBloom;
	m_pRenderForBloomArg = pArg;

	if( m_bBloomOn == bFlag )
		return true;

	m_bBloomOn = bFlag;
	
	if( m_bBloomOn )
	{
		if( !m_bResourceLoaded && !LoadResource() )
		{
			ReleaseResource();
			m_bBloomOn = false;
			return false;
		}
	}
	else
	{
		if( m_bResourceLoaded && !GetNeedResource() )
			ReleaseResource();
	}

	return true;
}

bool CECFullGlowRender::SetWarpOn(bool bFlag)
{
	return true;
	if( !m_bCanDoFullGlow )
		return true;

	if( m_bWarpOn == bFlag )
		return true;

	m_bWarpOn = bFlag;

	if( m_bWarpOn )
	{
		if( !m_bResourceLoaded && !LoadResource() )
		{
			ReleaseResource();
			m_bWarpOn = false;
			return false;
		}
	}
	else
	{
		if( m_bResourceLoaded && !GetNeedResource() )
			ReleaseResource();
	}

	return true;
}

bool CECFullGlowRender::SetFlareOn(bool bFlag)
{
	if( !m_bCanDoFullGlow )
		return true;

	if( m_bFlareOn == bFlag )
		return true;

	m_bFlareOn = bFlag;

	if( m_bFlareOn )
	{
		if( !m_bResourceLoaded && !LoadResource() )
		{
			ReleaseResource();
			m_bFlareOn = false;
			return false;
		}
	}
	else
	{
		if( m_bResourceLoaded && !GetNeedResource() )
			ReleaseResource();
	}

	return true;
}

bool CECFullGlowRender::SetSharpenOn(bool bFlag)
{
	return true;
	if( !m_bCanDoFullGlow )
		return true;

	if( m_bSharpenOn == bFlag )
		return true;

	m_bSharpenOn = bFlag;

	if( m_bSharpenOn )
	{
		if( !m_bResourceLoaded && !LoadResource() )
		{
			ReleaseResource();
			m_bSharpenOn = false;
			return false;
		}
	}
	else
	{
		if( m_bResourceLoaded && !GetNeedResource() )
			ReleaseResource();
	}

	return true;
}

bool CECFullGlowRender::ReloadResource()
{
	if( m_bResourceLoaded )
	{
		ReleaseResource();
		if( !LoadResource() )
		{
			m_bGlowOn = false;
			m_bBloomOn = false;
			m_bWarpOn = false;
			m_bFlareOn = false;
			m_bRenderToBack = false;
		}
	}
	
	return true;
}

bool CECFullGlowRender::CreateBumpMap()
{
	m_pBumpUnderWater = new A3DTexture;
	if( !m_pBumpUnderWater->Create(g_pGame->GetA3DDevice(), 256, 256, A3DFMT_A8R8G8B8, 1) )
		return false;

    // Fill the bumpmap texels to simulate a lens
	BYTE *	pBits;
	int		iPitch;
	if( !m_pBumpUnderWater->LockRect(NULL, (void **)&pBits, &iPitch, 0) )
		return false;

	DWORD * pDst = (DWORD*)pBits;

    for(int y=0; y<256; y++)
    {
		pDst = (DWORD *)(pBits + y * iPitch);
        for(int x=0; x<256; x++)
        {
            FLOAT fx = x * (1.0f / 256) - 0.5f;
            FLOAT fy = y * (1.0f / 256) - 0.5f;
            FLOAT r  = sqrtf(fx*fx + fy*fy);

            int iDu = (int)(32*cosf(4.0f*(fx+fy)*A3D_PI)) + 128;
            int iDv = (int)(32*sinf(4.0f*(fx+fy)*A3D_PI)) + 128;

            pDst[x] = A3DCOLORRGBA(iDu, iDv, 0, 0xff);
        }
    }

    m_pBumpUnderWater->UnlockRect();
	return true;
}

void CECFullGlowRender::SetupVertexConsts(float cx, float cy, float dx, float dy)
{
	A3DVECTOR4 c0(dx, dy, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(0, &c0, 1);
	A3DVECTOR4 c1(cx, cy, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(1, &c1, 1);
}

void CECFullGlowRender::SetupPixelConsts()
{
}

//////////////////////////////////////////////////////////////////////////////////////
// class CECFullGlowPresenter 
//////////////////////////////////////////////////////////////////////////////////////
CECFullGlowPresenter::CECFullGlowPresenter()
{
	Reset();
}

void CECFullGlowPresenter::SetData(A3DPixelShader * pGlow1Shader, A3DPixelShader * pGlow2Shader)
{
	m_pGlow1Shader		= pGlow1Shader;
	m_pGlow2Shader		= pGlow2Shader;
}
	
void CECFullGlowPresenter::Reset()
{
	m_vGlowPower			= 0.0f;
	m_vGlowPowerSet			= 0.0f;

	m_vGlowLevel			= 0.0f;
	m_vGlowLevelSet			= 0.0f;

	m_vFadeLevel			= 0.0f;
	m_vFadeLevelSet			= 0.0f;

	m_vMotionBlurLevel		= 0.0f;
	m_vMotionBlurLevelSet	= 0.0f;

	m_nGlowPowerTime		= 0;
	m_nGlowLevelTime		= 0;
	m_nFadeTime				= 0;
	m_nMotionBlurTime		= 0;
}

void CECFullGlowPresenter::SetMotionBlur(float vStart, float vEnd, int nTime)
{
	m_vMotionBlurLevel		= vStart;
	m_vMotionBlurLevelSet	= vEnd;
	m_vMotionBlurStep		= (vEnd - vStart) / nTime;
	m_nMotionBlurTime		= nTime;
}

void CECFullGlowPresenter::SetFade(float vStart, float vEnd, int nTime)
{
	m_vFadeLevel			= vStart;
	m_vFadeLevelSet			= vEnd;
	m_vFadeStep				= (vEnd - vStart) / nTime;
	m_nFadeTime				= nTime;
}

void CECFullGlowPresenter::SetGlowPower(float vStart, float vEnd, int nTime)
{
	if( vStart >= 0.0f )
		m_vGlowPower = vStart;
	else
		vStart = m_vGlowPower;

	m_vGlowPowerSet			= vEnd;
	m_vGlowPowerStep		= (vEnd - vStart) / nTime;
	m_nGlowPowerTime		= nTime;
}

void CECFullGlowPresenter::SetGlowLevel(float vStart, float vEnd, int nTime)
{
	if( vStart >= 0.0f )
		m_vGlowLevel = vStart;
	else
		vStart = m_vGlowLevel;

	m_vGlowLevelSet			= vEnd;
	m_vGlowLevelStep		= (vEnd - vStart) / nTime;
	m_nGlowLevelTime		= nTime;
}

void CECFullGlowPresenter::Update(int nDeltaTime)
{
	if( m_nMotionBlurTime )
	{
		m_nMotionBlurTime -= nDeltaTime;
		if( m_nMotionBlurTime > 0 )
			m_vMotionBlurLevel += m_vMotionBlurStep * nDeltaTime;
		else
		{
			m_vMotionBlurLevel = m_vMotionBlurLevelSet;
			m_nMotionBlurTime = 0;
		}
	}

	if( m_nFadeTime )
	{
		m_nFadeTime -= nDeltaTime;
		if( m_nFadeTime > 0 )
			m_vFadeLevel += m_vFadeStep * nDeltaTime;
		else
		{
			m_vFadeLevel = m_vFadeLevelSet;
			m_nFadeTime = 0;
		}
	}

	if( m_nGlowPowerTime )
	{
		m_nGlowPowerTime -= nDeltaTime;
		if( m_nGlowPowerTime > 0 )
			m_vGlowPower += m_vGlowPowerStep * nDeltaTime;
		else
		{
			m_vGlowPower = m_vGlowPowerSet;
			m_nGlowPowerTime = 0;
		}
	}

	if( m_nGlowLevelTime )
	{
		m_nGlowLevelTime -= nDeltaTime;
		if( m_nGlowLevelTime > 0 )
			m_vGlowLevel += m_vGlowLevelStep * nDeltaTime;
		else
		{
			m_vGlowLevel = m_vGlowLevelSet;
			m_nGlowLevelTime = 0;
		}
	}
}

void CECFullGlowPresenter::PrepareGlow1Pass(float gaussin_weights[3])
{
	A3DDevice * pA3DDevice = g_pGame->GetA3DDevice();
	
	A3DCOLORVALUE c0;
	c0.r = gaussin_weights[0];
	c0.g = gaussin_weights[1];
	c0.b = gaussin_weights[2];
	pA3DDevice->SetPixelShaderConstants(0, &c0, 1);

	A3DCOLORVALUE c1(0.0f);
	c1.a = 0.3f;
	pA3DDevice->SetPixelShaderConstants(1, &c1, 1);

	m_pGlow1Shader->Appear();
}

void CECFullGlowPresenter::AfterGlow1Pass()
{
	A3DDevice * pA3DDevice = g_pGame->GetA3DDevice();
	pA3DDevice->ClearPixelShader();
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
}

void CECFullGlowPresenter::PrepareGlow2Pass(A3DCOLOR colorNow, float gaussin_weights[3])
{
	A3DDevice * pA3DDevice = g_pGame->GetA3DDevice();
	A3DCOLORVALUE clNow = a3d_ColorRGBAToColorValue(colorNow);
	clNow.a = 1.0f;
	
	A3DCOLORVALUE c0;
	c0.r = gaussin_weights[0];
	c0.g = gaussin_weights[1];
	c0.b = gaussin_weights[2];
	pA3DDevice->SetPixelShaderConstants(0, &c0, 1);

	A3DCOLORVALUE c1(0.0f);
	c1.g = m_vGlowLevel;
	c1.b = m_vGlowPower;
	pA3DDevice->SetPixelShaderConstants(1, &c1, 1);

	m_pGlow2Shader->Appear();

	if( m_vFadeLevel != 0.0f )
	{
		// this can affect the whole color, except the alpha channel
		clNow.r = clNow.r * (1.0f - m_vFadeLevel);
		clNow.g = clNow.g * (1.0f - m_vFadeLevel);
		clNow.b = clNow.b * (1.0f - m_vFadeLevel);
	}

	if( m_vMotionBlurLevel != 0.0f )
	{
		clNow.a = clNow.a * (1.0f - m_vMotionBlurLevel);
	}

	pA3DDevice->SetPixelShaderConstants(3, &clNow, 1);
}

void CECFullGlowPresenter::AfterGlow2Pass()
{
	A3DDevice * pA3DDevice = g_pGame->GetA3DDevice();
	pA3DDevice->ClearPixelShader();
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
}

bool CECFullGlowPresenter::IsFading()
{
	if( m_vFadeLevel != 0.0f )
		return true;
	else
		return false;
}

bool CECFullGlowPresenter::IsMontionBluring()
{
	if( m_vMotionBlurLevel != 0.0f )
		return true;
	else
		return false;
}

void CECFullGlowPresenter::StopMotionBlur()
{
	SetMotionBlur(m_vMotionBlurLevel, 0.0f, 1000);
}

void CECFullGlowPresenter::StopFade(bool bToWhite)
{
	m_nFadeTime = 1000;

	if( bToWhite )
	{
		SetFade(m_vFadeLevel, 0.0f, 1000);
	}
	else
	{
		SetFade(m_vFadeLevel, 1.0f, 1000);
	}
}

