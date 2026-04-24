/*
 * FILE: Render.cpp
 *
 * DESCRIPTION: Render class
 *
 * CREATED BY: Duyuxin, 2004/6/8
 *
 * HISTORY:
 *
 * Copyright (c) 2001 Archosaur Studio, All Rights Reserved.	
 */

#include "Global.h"
#include "A3D.h"
#include "Render.h"
#include "A3DGFXExMan.h"
#ifdef _ANGELICA22
#include "PlatformRenderer.h"
#include <A3DVolumeTexture.h>
#endif // #ifdef _ANGELICA22

//#define new A_DEBUG_NEW


///////////////////////////////////////////////////////////////////////////
//
//	Reference to External variables and functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Local Types and Variables and Global variables
//
///////////////////////////////////////////////////////////////////////////

CRender	g_Render;

#ifdef _ANGELICA22
static const D3DVERTEXELEMENT9 aDirectVertexShaderDecl[] =
{
	{0, 0,  D3DDECLTYPE_FLOAT4, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 16, D3DDECLTYPE_D3DCOLOR, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_COLOR, 0},
	{0, 20, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};

static const D3DVERTEXELEMENT9 aVertexShaderDecl[] =
{
	{0, 0,  D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_POSITION, 0},
	{0, 12, D3DDECLTYPE_FLOAT3, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_NORMAL, 0},
	{0, 24, D3DDECLTYPE_FLOAT2, D3DDECLMETHOD_DEFAULT, D3DDECLUSAGE_TEXCOORD, 0},
	D3DDECL_END()
};
#endif // #ifdef _ANGELICA22


///////////////////////////////////////////////////////////////////////////
//
//	Local functions
//
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//
//	Implement CRender
//
///////////////////////////////////////////////////////////////////////////

CRender::CRender()
{
	m_pA3DEngine	= NULL;
	m_pA3DDevice	= NULL;
	m_pDirLight		= NULL;
	m_pFont			= NULL;
	m_hInstance		= NULL;
	m_hRenderWnd	= NULL;
	m_bEngineOK		= false;
#ifdef _ANGELICA22
	m_bIsPresent	= true;
#endif // #ifdef _ANGELICA22
	m_GlowRadius	= 9.0f;
	m_color			= A3DCOLORRGB(255,255,255);
	m_colorMono		= A3DCOLORRGB(255,255,255);

	m_vGlowPower			= 0.0f;
	m_vGlowPowerSet			= 0.0f;

	m_vGlowLevel			= 0.0f;
	m_vGlowLevelSet			= 0.0f;

	m_vFadeLevel			= 0.0f;
	m_vFadeLevelSet			= 0.0f;

	m_vMotionBlurLevel		= 0.0f;
	m_vMotionBlurLevelSet	= 0.0f;

	m_vMonoBlurLevel		= 0.0f;
	m_vMonoBlurLevelSet		= 0.0f;
	m_vMonoBlurStep			= 0.0f;
	m_nMonoBlurTime			= 0;
	m_dwLastTime			= 0;

	m_nGlowPowerTime		= 0;
	m_nGlowLevelTime		= 0;
	m_nFadeTime				= 0;
	m_nMotionBlurTime		= 0;

	m_vMonoBlurLevelCur		= 0.0f;
	m_bMonoTick				= false;
	m_bStartMono			= false;

	m_fMinLum				= 0.2f;
	m_fFlashPower			= 0.0f;
	m_fFlashPowerSet		= 0.0f;
	m_fFlashPowerStep		= 0.0f;
	m_nFlashTime			= 0;
#ifdef _ANGELICA22
	m_pColorMappingPS		= NULL;
	m_pColorMappingVolTex	= NULL;
    m_pPlatformRender       = NULL;
#endif // #ifdef _ANGELICA22
}

/*	Initialize game

	Return true for success, otherwise return false.

	hInst: handle of instance.
	hWnd: handle of render window.
*/
bool CRender::Init(HINSTANCE hInst, HWND hWnd)
{
	m_hInstance		= hInst;
	m_hRenderWnd	= hWnd;

	if (!InitA3DEngine())
	{
		g_Log.Log("CRender::Init, Failed to initialize A3D engine!");
		return false;
	}

	if (!CheckDevice())
	{
		g_Log.Log("CRender::Init, Device isn't eligible !");
		return false;
	}

#ifdef _ANGELICA22
    m_pPlatformRender = new PlatformRenderer();
    if( !m_pPlatformRender->Init() )
	{
		g_Log.Log( __FUNCTION__, ", Failed to initialize platform render." );
		return false;
	}
#endif // #ifdef _ANGELICA22
	return true;
}

//	Release game
void CRender::Release()
{
#ifdef _ANGELICA22
    A3DRELEASE(m_pPlatformRender);
#endif // #ifdef _ANGELICA22
	ReleaseA3DEngine();
}

/*	Initialize A3D engine

	Return true for success, otherwise return false.
*/
bool CRender::InitA3DEngine()
{
	//	Init Engine
	if (!(m_pA3DEngine = new A3DEngine()))
	{
		g_Log.Log("CRender::InitA3DEngine: Not enough memory!");
		return false;
	}
	
	RECT Rect;
	GetClientRect(m_hRenderWnd, &Rect);

	m_iRenderWid = Rect.right;
	m_iRenderHei = Rect.bottom;
	
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= true;
	devFmt.nWidth		= m_iRenderWid;
	devFmt.nHeight		= m_iRenderHei;
	devFmt.fmtTarget	= A3DFMT_UNKNOWN;
	devFmt.fmtDepth		= A3DFMT_UNKNOWN;
	devFmt.bVSync       = false;

	DWORD dwDevFlags = A3DDEV_ALLOWMULTITHREAD;// | A3DDEV_FORCESOFTWARETL;	//	Force software T&L

	if (!m_pA3DEngine->Init(m_hInstance, m_hRenderWnd, &devFmt, dwDevFlags))
	{
		g_Log.Log("CRender::InitA3DEngine: Failed to initialize A3D engine.");
		return false;
	}
#ifdef _ANGELICA22
	g_pA3DConfig->SetFlagDoNotReplaceBySdrx(true);
#endif // #ifdef _ANGELICA22

	m_pA3DDevice = m_pA3DEngine->GetA3DDevice();

	//	Add a directional light into scene
	m_pA3DEngine->GetA3DLightMan()->CreateDirecionalLight(1, &m_pDirLight, A3DVECTOR3(0.0f, 0.0f, 0.0f),
					A3DVECTOR3(0.0f, -0.5f, 1.0f), A3DCOLORVALUE(0.8f, 0.8f, 0.9f, 1.0f), 
					A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f), A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f));

	//	Set some values
	m_pA3DDevice->SetAmbient(A3DCOLORRGB(255,255,255));
	m_pA3DDevice->SetFogStart(100.0f);
	m_pA3DDevice->SetFogEnd(5000.0f);
	m_pA3DDevice->SetFogEnable(false);
#ifdef _ANGELICA22
	m_pA3DDevice->SetAlphaBlendEnable(true);
	m_pA3DDevice->SetAlphaTestEnable(true);
#endif // #ifdef _ANGELICA22

	m_pFont = m_pA3DEngine->GetSystemFont();
//	m_pFont->EnableFilter(false);

	m_pA3DEngine->GetA3DSkinMan()->SetDirLight(m_pDirLight);

//	g_pA3DConfig->RT_SetShowPerformanceFlag(true);
	A3DSkinMan* pSkinMan = m_pA3DEngine->GetA3DSkinMan();
	if (!pSkinMan->LoadSkinModelVertexShaders(CONFIG_DIR"skinmodelvs.cfg"))
	{
		a_LogOutput(1, "CRender::InitA3DEngine, Failed to load skinmodelvs.cfg");
		return false;
	}

	pSkinMan->SetDirLight(m_pDirLight);
	pSkinMan->SetDPointLight(NULL);


	//	we need a frame target to be used as texture in gfx editor
	if( !CreateFrameTarget() )
	{
		g_Log.Log("CRender::InitA3DEngine(), Failed to create frame target!");
		return false;
	}

	// now load resource for TL space warps.
	m_nMaxTLWarpVerts = 1000;
	m_nMaxTLWarpIndices = 1500;
	m_pTLWarpStream = new A3DStream();
	if( !m_pTLWarpStream->Init(m_pA3DDevice, sizeof(A3DTLWARPVERTEX), A3DFVF_A3DTLWARPVERT, m_nMaxTLWarpVerts, m_nMaxTLWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to create warp stream!");
		return false;
	}

	WORD * pIndices;
	if( !m_pTLWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0) )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to lock warp stream's index buffer!");
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
	m_pTLWarpStream->UnlockIndexBuffer();
	
	m_pTLWarpShader = m_pA3DEngine->GetA3DShaderMan()->LoadPixelShader(SHADER_DIR"ps\\spacewarp_tl.txt", false);
	if( NULL == m_pTLWarpShader )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to load spacewarp_tl.txt");
		::MessageBox(NULL, "º”‘ÿ"SHADER_DIR"ps\\spacewarp_tl.txt ß∞‹", NULL, MB_OK);
		return false;
	}

	m_nMaxWarpVerts = 1000;
	m_nMaxWarpIndices = 1500;
	m_pWarpStream = new A3DStream();
	if( !m_pWarpStream->Init(m_pA3DDevice, sizeof(A3DWARPVERTEX), A3DFVF_A3DWARPVERT, m_nMaxWarpVerts, m_nMaxWarpIndices, A3DSTRM_REFERENCEPTR, A3DSTRM_STATIC) )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to create warp stream!");
		return false;
	}

	if( !m_pWarpStream->LockIndexBuffer(0, 0, (BYTE **)&pIndices, 0) )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to lock warp stream's index buffer!");
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
	m_pWarpStream->UnlockIndexBuffer();

	// now load vertex shader for blur stream
#ifdef _ANGELICA22
	m_pGaussinBlurVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader(SHADER_DIR"vs\\gaussin_blur_vs.txt", false, (D3DVERTEXELEMENT9*)aDirectVertexShaderDecl);
#else
	DWORD dwVSDecl[32];
	int n = 0;				
	dwVSDecl[n ++] = D3DVSD_STREAM(0);					// begin tokens.
	dwVSDecl[n ++] = D3DVSD_REG(0, D3DVSDT_FLOAT4);		// x, y, z and w in cuboid space
	dwVSDecl[n ++] = D3DVSD_REG(1, D3DVSDT_D3DCOLOR);	// diffuse color
	dwVSDecl[n ++] = D3DVSD_REG(2, D3DVSDT_FLOAT2);		// u, v texture coord
	dwVSDecl[n ++] = D3DVSD_END();						// end tokens.
	m_pGaussinBlurVS = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadVertexShader("Shaders\\vs\\gaussin_blur_vs.txt", false, dwVSDecl);
#endif // #ifdef _ANGELICA22
	if( NULL == m_pGaussinBlurVS )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load gaussin_blur_vs.txt");
		return false;
	}
	
	m_pWarpShader = m_pA3DEngine->GetA3DShaderMan()->LoadPixelShader(SHADER_DIR"ps\\spacewarp.txt", false);
	if( NULL == m_pWarpShader )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to load spacewarp.txt");
		::MessageBox(NULL, "º”‘ÿ"SHADER_DIR"ps\\spacewarp.txt ß∞‹", NULL, MB_OK);
		return false;
	}

	// now load pixel shader for render glow2
	
	// now load pixel shader for render glow1
	m_pGlow1Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader(SHADER_DIR"ps\\fullglow1_14.txt", false);
	if( NULL == m_pGlow1Shader )
	{
		g_Log.Log("CECFullGlowRender::LoadResource(), failed to load fullglow1.txt");
		return false;
	}
	
	m_pGlow2Shader = m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->LoadPixelShader(SHADER_DIR"ps\\fullglow2_14.txt", false);
	if( NULL == m_pGlow2Shader )
	{
		g_Log.Log("CRender::InitA3DEngine(), failed to load fullglow2.txt");
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

	m_vertsBlur[0] = A3DGLOWVERTEX(A3DVECTOR4(-1.0f, 1.0f, 0.0f, 1.0f), 0xffffffff, 0.0f, 0.0f);
	m_vertsBlur[1] = A3DGLOWVERTEX(A3DVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), 0xffffffff, 1.0f, 0.0f);
	m_vertsBlur[2] = A3DGLOWVERTEX(A3DVECTOR4(-1.0f, -1.0f, 0.0f, 1.0f), 0xffffffff, 0.0f, 1.0f);
	m_vertsBlur[3] = A3DGLOWVERTEX(A3DVECTOR4(1.0f, -1.0f, 0.0f, 1.0f), 0xffffffff, 1.0f, 1.0f);
	m_pStreamBlur->SetVerts((BYTE *) m_vertsBlur, 4);
	m_pStreamBlur->SetVertexRef((BYTE *) m_vertsBlur);
	m_pStreamBlur->SetIndices((BYTE*) m_indices, 6);
	m_pStreamBlur->SetIndexRef((void *) m_indices);

	CalculateGaussian(m_GlowRadius,m_GlowWeights);

	m_bEngineOK = true;

	return true;
}

//	Release A3D engine
void CRender::ReleaseA3DEngine()
{
	if (m_pDirLight)
	{
		m_pDirLight->Release();
		delete m_pDirLight;
		m_pDirLight = NULL;
	}

	if (m_pA3DEngine)
	{
		m_pA3DEngine->GetA3DShaderMan()->ReleaseVertexShader(&m_pGaussinBlurVS);
		m_pA3DEngine->GetA3DShaderMan()->ReleasePixelShader(&m_pTLWarpShader);
		m_pA3DEngine->GetA3DShaderMan()->ReleasePixelShader(&m_pWarpShader);
		m_pA3DEngine->GetA3DShaderMan()->ReleasePixelShader(&m_pGlow1Shader);
		m_pA3DEngine->GetA3DShaderMan()->ReleasePixelShader(&m_pGlow2Shader);
#ifdef _ANGELICA22
		if (m_pColorMappingPS)
		{
			m_pA3DDevice->GetA3DEngine()->GetA3DShaderMan()->ReleasePixelShader(&m_pColorMappingPS);
			m_pColorMappingPS = NULL;
		}
#endif // #ifdef _ANGELICA22
	}
#ifdef _ANGELICA22
	//if(m_pColorMapping) m_pColorMapping->Release();
	A3DRELEASE(m_pColorMappingVolTex);
#endif // #ifdef _ANGELICA22
	A3DRELEASE(m_pTLWarpStream);
	A3DRELEASE(m_pStreamBlur);
	A3DRELEASE(m_pWarpStream);
	A3DRELEASE(m_pFrameTarget);
	A3DRELEASE(m_pGlowTarget1);
	A3DRELEASE(m_pA3DEngine);
	
	m_pA3DDevice = NULL;
	m_bEngineOK = false;
}

//	Check device
bool CRender::CheckDevice()
{
	//	We must support pixel shader 1.1
	if (!m_pA3DDevice->TestPixelShaderVersion(1, 1))
	{
		g_Log.Log("CRender::CheckDevice, Device doesn't support pixel shader 1.1");
		return false;
	}

	//	Number of texture stage must >= 3
	if (m_pA3DDevice->GetMaxSimultaneousTextures() < 4)
	{
		g_Log.Log("CRender::CheckDevice, Device must support more than 3 texture stages");
		return false;
	}
	
	return true;
}

/*	Resize device

	Return true for success, otherwise return false.

	iWid, iHei: new size of render area
*/
bool CRender::ResizeDevice(int iWid, int iHei)
{
	if (!m_pA3DEngine)
		return false;

	m_pA3DEngine->SetDisplayMode(iWid, iHei, A3DFMT_UNKNOWN, 0, false, SDM_WIDTH | SDM_HEIGHT);

	m_iRenderWid = iWid;
	m_iRenderHei = iHei;

	if( !CreateFrameTarget() )
	{
		g_Log.Log("CRender::ResizeDevice(), failed to resize frame target!");
		return false;
	}

	return true;
}

//	Begin render
bool CRender::BeginRender(bool bDoWarps)
{
#ifdef _ANGELICA22
	bDoWarps = false;
#endif // #ifdef _ANGELICA22
	ASSERT(m_pA3DEngine);
	if( !m_pA3DEngine->BeginRender() )
		return false;
	
	if (bDoWarps) 
	{
#ifdef _ANGELICA22
		m_pFrameTarget->ApplyToDevice();
#else
		m_pA3DEngine->GetA3DDevice()->SetRenderTarget(m_pFrameTarget);
		m_pA3DEngine->GetA3DDevice()->SetRestoreToRenderTarget(m_pFrameTarget);
#endif // #ifdef _ANGELICA22
	}

	return true;
}

//	End render
bool CRender::EndRender(bool bDoWarps)
{
#ifdef _ANGELICA22
	bDoWarps = false;
#endif // #ifdef _ANGELICA22
	ASSERT(m_pA3DEngine);


	AfxGetGFXExMan()->ClearLightParamList();
	
	// we should move content from frame target to back target and then do warps.
	
	if (bDoWarps)
	{
#ifdef _ANGELICA22
		m_pFrameTarget->WithdrawFromDevice();

		m_pA3DEngine->GetActiveViewport()->Active();

		D3DXLoadSurfaceFromSurface(m_pA3DDevice->GetBackBuffer()->m_pD3DSurface, NULL, NULL, m_pFrameTarget->GetTargetSurface()->m_pD3DSurface, NULL, NULL, D3DX_DEFAULT, 0);

		DoWarps();

		D3DXLoadSurfaceFromSurface(m_pFrameTarget->GetTargetSurface()->m_pD3DSurface, NULL, NULL, m_pA3DDevice->GetBackBuffer()->m_pD3DSurface, NULL, NULL, D3DX_DEFAULT, 0);
#else
		m_pA3DEngine->GetA3DDevice()->SetDefaultRestoreToRenderTarget();
		m_pA3DEngine->GetA3DDevice()->RestoreRenderTarget();

		m_pA3DEngine->GetActiveViewport()->Active();

		HRESULT hval = m_pA3DDevice->GetD3DDevice()->CopyRects(m_pFrameTarget->GetTargetSurface(), NULL, 0,
			m_pA3DDevice->GetBackBuffer(), NULL);

		DoWarps();

		hval = m_pA3DDevice->GetD3DDevice()->CopyRects(m_pA3DDevice->GetBackBuffer(), NULL, 0,
			m_pFrameTarget->GetTargetSurface(), NULL);
#endif // #ifdef _ANGELICA22

		DoGlow();
	}

	if (!m_pA3DEngine->EndRender())
		return false;

#ifdef _ANGELICA22
	if(!m_bIsPresent)
		return true;
#endif // #ifdef _ANGELICA22
	return m_pA3DEngine->Present();
}

//	Output text
void CRender::TextOut(int x, int y, const char* szText, DWORD color)
{
	if (m_pFont)
		m_pFont->TextOut(x, y, szText, color);
}

//	Draw 2D rectangle
void CRender::Draw2DRect(const RECT& rc, DWORD dwCol, bool bFrame)
{
	if (bFrame)
	{
		A3DPOINT2 pt1, pt2;

		//	Top border
		pt1.Set(rc.left, rc.top);
		pt2.Set(rc.right, rc.top);
		g_pA3DGDI->Draw2DLine(pt1, pt2, dwCol);

		//	Left border
		pt1.Set(rc.left, rc.top);
		pt2.Set(rc.left, rc.bottom);
		g_pA3DGDI->Draw2DLine(pt1, pt2, dwCol);

		//	Right border
		pt1.Set(rc.right, rc.top);
		pt2.Set(rc.right, rc.bottom);
		g_pA3DGDI->Draw2DLine(pt1, pt2, dwCol);

		//	Bottom border
		pt1.Set(rc.left, rc.bottom);
		pt2.Set(rc.right, rc.bottom);
		g_pA3DGDI->Draw2DLine(pt1, pt2, dwCol);
	}
	else
	{
		A3DRECT rect(rc.left, rc.top, rc.right, rc.bottom);
		g_pA3DGDI->Draw2DRectangle(rect, dwCol);
	}
}

//	Set ambient color and diretional light's direction and color
void CRender::ChangeLight(const A3DVECTOR3& vDir, DWORD dwDirCol, DWORD dwAmbient)
{
	m_pA3DDevice->SetAmbient(dwAmbient | 0xff000000);

	if (!m_pDirLight)
		return;

	A3DLIGHTPARAM Params = m_pDirLight->GetLightparam();

	Params.Direction = a3d_Normalize(vDir);
	Params.Diffuse	 = dwDirCol | 0xff000000;

	m_pDirLight->SetLightParam(Params);
}

bool CRender::CreateFrameTarget()
{
	if( m_pFrameTarget ) A3DRELEASE(m_pFrameTarget);
	
	A3DDEVFMT devFmt = m_pA3DDevice->GetDevFormat();
	devFmt.bWindowed	= true;
#ifdef _ANGELICA22
	A3DRenderTarget::RTFMT rtg;
	rtg.fmtDepth = devFmt.fmtDepth;
	rtg.fmtTarget = devFmt.fmtTarget;
	rtg.iHeight = devFmt.nHeight;
	rtg.iWidth = devFmt.nWidth;

	m_pFrameTarget = new A3DRenderTarget();
	if( !m_pFrameTarget->Init(m_pA3DDevice, rtg, true, true) )
	{
		g_Log.Log("CRender::CreateFrameTarget(), failed to create frame target!");
		return false;
	}

	if(m_pGlowTarget1) A3DRELEASE(m_pGlowTarget1);

	m_pGlowTarget1 = new A3DRenderTarget();
	if( !m_pGlowTarget1->Init(m_pA3DDevice, rtg, true, true) )
	{
		g_Log.Log("CRender::CreateFrameTarget(), failed to create glow target!");
		return false;
	}
#else
	m_pFrameTarget = new A3DRenderTarget();
	if( !m_pFrameTarget->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CRender::CreateFrameTarget(), failed to create frame target!");
		return false;
	}

	if(m_pGlowTarget1) A3DRELEASE(m_pGlowTarget1);

	m_pGlowTarget1 = new A3DRenderTarget();
	if( !m_pGlowTarget1->Init(m_pA3DDevice, devFmt, true, false) )
	{
		g_Log.Log("CRender::CreateFrameTarget(), failed to create glow target!");
		return false;
	}
#endif // #ifdef _ANGELICA22

	return true;
}

bool CRender::DoWarps()
{
	// first TL space warps.
#ifdef _ANGELICA22
	m_pFrameTarget->AppearAsTexture(1);
#else
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pFrameTarget->GetTargetTexture());
#endif // #ifdef _ANGELICA22
	
	m_pTLWarpStream->Appear();
	m_pTLWarpShader->Appear();

	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	A3DCULLTYPE oldCull = m_pA3DDevice->GetFaceCull();
	m_pA3DDevice->SetFaceCull(A3DCULL_NONE);
	
	float	rw = 1.0f / m_iRenderWid;
	float	rh = 1.0f / m_iRenderHei;

	A3DGFXExMan * pGfxMan = AfxGetGFXExMan();

	GfxElementList& elementList = pGfxMan->GetWarpEleList();
	pGfxMan->LockWarpEleList();
	int i;
	for(i=0; i<(int)elementList.size(); i++)
	{
		A3DGFXElement * pElement = elementList[i];
		if( pElement  && pElement->IsTLVert())
		{
			A3DTLWARPVERTEX * pVerts;
			if( !m_pTLWarpStream->LockVertexBuffer(0, 0, (BYTE **)&pVerts, 0) )
			{
				break;
			}
			int nVerts = pElement->DrawToBuffer(m_pA3DEngine->GetActiveViewport(), pVerts, m_nMaxTLWarpVerts, rw, rh);
			m_pTLWarpStream->UnlockVertexBuffer();
				
			pElement->GetTexture()->Appear(0);
			m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
			pElement->GetTexture()->Disappear(0);
		}
	}

	m_pTLWarpShader->Disappear();

	A3DCameraBase * pCamera = m_pA3DEngine->GetActiveViewport()->GetCamera();

	// then do 3d warps.
	m_pWarpStream->Appear();
	m_pWarpShader->Appear();

	m_pA3DDevice->SetWorldMatrix(IdentityMatrix());
	A3DMATRIX4 matScale = IdentityMatrix();
	matScale._11 = 0.5f;
	matScale._22 = -0.5f;
	matScale._41 = 0.5f;
	matScale._42 = 0.5f;
	A3DMATRIX4 matProjectedView;
	matProjectedView = pCamera->GetProjectionTM() * matScale;
	m_pA3DDevice->SetTextureCoordIndex(1, D3DTSS_TCI_CAMERASPACEPOSITION);
	m_pA3DDevice->SetTextureTransformFlags(1, (A3DTEXTURETRANSFLAGS)(A3DTTFF_COUNT4 | A3DTTFF_PROJECTED));
	m_pA3DDevice->SetTextureMatrix(1, matProjectedView);


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
			int nVerts = pElement->DrawToBuffer(m_pA3DEngine->GetActiveViewport(), pVerts, m_nMaxWarpVerts);
			m_pWarpStream->UnlockVertexBuffer();

			pElement->GetTexture()->Appear(0);
			m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, nVerts, 0, nVerts >> 1);
			pElement->GetTexture()->Disappear(0);
		}
	}

	m_pA3DDevice->SetTextureCoordIndex(1, 1);
	m_pA3DDevice->SetTextureTransformFlags(1, A3DTTFF_DISABLE);
	m_pWarpShader->Disappear();

	pGfxMan->UnlockWarpEleList();
	pGfxMan->ClearWarpEleList();

	m_pA3DDevice->ClearTexture(1);

	m_pA3DDevice->SetFaceCull(oldCull);
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(g_Configs.bEnableFog);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetTextureAddress(1, A3DTADDR_WRAP, A3DTADDR_WRAP);

  
	return true;
}

// Set mono blur
void CRender::BeginMonoBlur(float fMonoBlurLevel, float fMonoBlurLevelSet, int nMonoBlurTime)
{
	m_vMonoBlurLevel = fMonoBlurLevel;
	m_vMonoBlurLevelSet = fMonoBlurLevelSet;
	m_nMonoBlurTime = nMonoBlurTime;
	m_vMonoBlurStep = (fMonoBlurLevelSet - fMonoBlurLevel)/nMonoBlurTime; 
	m_vMonoBlurLevelCur = m_vMonoBlurLevel;
	m_dwLastTime = a_GetTime();

	m_bStartMono = true;
	m_bMonoTick  = true;
};

void CRender::EndMonoBlur()
{	
	m_bStartMono = false; 
	m_bMonoTick = true; 
	m_dwLastTime = a_GetTime();
}

bool CRender::DoGlow()
{
#ifdef _ANGELICA22
	return false;
#endif // #ifdef _ANGELICA22
	if(m_bMonoTick)
	{
		DWORD dwCurTime = a_GetTime();
		DWORD dwTimeDela = dwCurTime - m_dwLastTime;
		if(m_vMonoBlurLevelCur < m_vMonoBlurLevelSet && m_bStartMono)
		{
			m_vMonoBlurLevelCur = m_vMonoBlurLevelCur + (dwTimeDela*m_vMonoBlurStep);
			if(m_vMonoBlurLevelCur > m_vMonoBlurLevelSet) m_vMonoBlurLevelCur = m_vMonoBlurLevelSet;
			m_dwLastTime = dwCurTime;
		}else if(m_vMonoBlurLevelCur > m_vMonoBlurLevel && !m_bStartMono)
		{
			m_vMonoBlurLevelCur = m_vMonoBlurLevelCur - (dwTimeDela*m_vMonoBlurStep);
			if(m_vMonoBlurLevelCur < m_vMonoBlurLevel) m_vMonoBlurLevelCur = m_vMonoBlurLevel;
			m_dwLastTime = dwCurTime;
		}else m_bMonoTick = false;
	}

	A3DVIEWPORTPARAM param;
	param.X = 0;
	param.Y = 0;
	param.Width = m_iRenderWid / 4;
	param.Height = m_iRenderHei / 4;
	param.MinZ = 0.0f;
	param.MaxZ = 1.0f;
	m_pA3DDevice->SetViewport(&param);

	m_pA3DDevice->SetLightingEnable(false);
	m_pA3DDevice->SetFogEnable(false);
	m_pA3DDevice->SetZWriteEnable(false);
	m_pA3DDevice->SetZTestEnable(false);

	// first blur 1 from target1 to target2
#ifdef _ANGELICA22
	m_pGlowTarget1->ApplyToDevice();
#else
	m_pA3DDevice->SetRenderTarget(m_pGlowTarget1);
#endif // #ifdef _ANGELICA22
	
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

#ifdef _ANGELICA22
	m_pFrameTarget->AppearAsTexture(0);
	m_pFrameTarget->AppearAsTexture(1);
	m_pFrameTarget->AppearAsTexture(2);
	m_pFrameTarget->AppearAsTexture(3);
	m_pFrameTarget->AppearAsTexture(4);
	m_pFrameTarget->AppearAsTexture(5);
#else
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pFrameTarget->GetTargetTexture());
#endif // #ifdef _ANGELICA22
	
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_ONE);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_ZERO);
	PrepareGlow1Pass(m_GlowWeights);
	m_pStreamBlur->Appear();
	m_pGaussinBlurVS->Appear();
	SetupVertexConsts(-1.0f/(m_iRenderWid/4), 1.0f/(m_iRenderHei/4), m_GlowRadius/3.0f/m_iRenderWid, 0.0f);
	//SetupPixelConsts();

	m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);

	m_pA3DDevice->ClearPixelShader();
	//m_pPresenter->AfterGlow1Pass();
#ifdef _ANGELICA22
	m_pGlowTarget1->WithdrawFromDevice();
#else
	m_pA3DDevice->RestoreRenderTarget();
#endif // #ifdef _ANGELICA22

	//g_pGame->GetViewport()->GetA3DViewport()->Active();
	GetA3DEngine()->GetActiveViewport()->Active();

	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	PrepareGlow2Pass(m_color,m_GlowWeights);
#ifdef _ANGELICA22
	m_pFrameTarget->AppearAsTexture(0);
	m_pGlowTarget1->AppearAsTexture(1);
	m_pGlowTarget1->AppearAsTexture(2);
	m_pGlowTarget1->AppearAsTexture(3);
	m_pGlowTarget1->AppearAsTexture(4);
	m_pGlowTarget1->AppearAsTexture(5);
#else
	m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pFrameTarget->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(1, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(2, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(3, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(4, m_pGlowTarget1->GetTargetTexture());
	m_pA3DDevice->GetD3DDevice()->SetTexture(5, m_pGlowTarget1->GetTargetTexture());
#endif // #ifdef _ANGELICA22
	SetupVertexConsts(-1.0f/m_iRenderWid, 1.0f/m_iRenderHei, 0.0f, m_GlowRadius/3.0f/m_iRenderHei);
	
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
	m_pA3DDevice->ClearPixelShader();
	
	m_pA3DDevice->SetLightingEnable(true);
	m_pA3DDevice->SetFogEnable(g_Configs.bEnableFog);
	m_pA3DDevice->SetZWriteEnable(true);
	m_pA3DDevice->SetZTestEnable(true);
	m_pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	m_pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);

#ifdef _ANGELICA22
	// For texture color mapping
	// Set color map
    bool bFogEnable = m_pA3DDevice->GetFogEnable();
    m_pA3DDevice->SetZTestEnable(false);
    m_pA3DDevice->SetZWriteEnable(false);
    m_pA3DDevice->SetFogEnable(false);
    m_pA3DDevice->SetAlphaBlendEnable(false);
    m_pA3DDevice->SetAlphaTestEnable(false);
    m_pA3DDevice->SetTextureFilterType(0, A3DTEXF_LINEAR);
    m_pA3DDevice->SetTextureAddress(0, A3DTADDR_CLAMP, A3DTADDR_CLAMP);
	m_pFrameTarget->AppearAsTexture(0);
    //m_pA3DDevice->GetD3DDevice()->SetTexture(0, m_pFrameTarget->GetTargetTexture());
    m_pA3DDevice->SetLightingEnable(false);
    m_pA3DDevice->SetSpecularEnable(false);
    
    //IDirect3DVolumeTexture9* pColorMappingTex = m_pColorMapping;
    
    if (m_pColorMappingVolTex)
    {
        m_pA3DDevice->SetTextureFilterType(1, A3DTEXF_LINEAR);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_CLAMP);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_CLAMP);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_CLAMP);
        //m_pA3DDevice->GetD3DDevice()->SetTexture(1, pColorMappingTex);
		m_pColorMappingVolTex->Appear(1);
        m_pColorMappingPS->Appear();
    }
    
    m_pA3DDevice->DrawIndexedPrimitive(A3DPT_TRIANGLELIST, 0, 4, 0, 2);
    
    if (m_pColorMappingVolTex)
    {
		m_pA3DDevice->ClearTexture(1);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
		m_pA3DDevice->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
        //m_pA3DDevice->GetD3DDevice()->SetTexture(1, NULL);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSU, D3DTADDRESS_WRAP);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSV, D3DTADDRESS_WRAP);
        //m_pA3DDevice->GetD3DDevice()->SetSamplerState(1, D3DSAMP_ADDRESSW, D3DTADDRESS_WRAP);
        m_pColorMappingPS->Disappear();
    }
    
    m_pA3DDevice->SetZTestEnable(true);
    m_pA3DDevice->SetZWriteEnable(true);
    m_pA3DDevice->SetFogEnable(bFogEnable);
    m_pA3DDevice->SetAlphaBlendEnable(true);
    m_pA3DDevice->SetLightingEnable(true);
#endif // #ifdef _ANGELICA22
	return true;
}

void CRender::PrepareGlow1Pass( float gaussin_weights[3] )
{
	A3DCOLORVALUE c0;
	c0.r = gaussin_weights[0];
	c0.g = gaussin_weights[1];
	c0.b = gaussin_weights[2];
	m_pA3DDevice->SetPixelShaderConstants(0, &c0, 1);

	A3DCOLORVALUE c1(0.0f);
	c1.a = 0.3f;
	m_pA3DDevice->SetPixelShaderConstants(1, &c1, 1);

	m_pGlow1Shader->Appear();
}

void CRender::PrepareGlow2Pass(A3DCOLOR colorNow, float gaussin_weights[3])
{
	A3DCOLORVALUE clNow = a3d_ColorRGBAToColorValue(colorNow);
	clNow.a = 1.0f;
	
	A3DCOLORVALUE c0;
	c0.r = gaussin_weights[0];
	c0.g = gaussin_weights[1];
	c0.b = gaussin_weights[2];
	m_pA3DDevice->SetPixelShaderConstants(0, &c0, 1);

	A3DCOLORVALUE c1(0.0f);
	c1.g = m_vGlowLevel;
	c1.b = m_vGlowPower;
	m_pA3DDevice->SetPixelShaderConstants(1, &c1, 1);

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
		clNow.a = 1.0f - m_vMotionBlurLevel;
	}
	m_pA3DDevice->SetPixelShaderConstants(3, &clNow, 1);

	//if( m_vMonoBlurLevelCur != 0.0f )
	{
		clNow = a3d_ColorRGBAToColorValue(m_colorMono);
		clNow.a = m_vMonoBlurLevelCur;
		m_pA3DDevice->SetPixelShaderConstants(4, &clNow, 1);
	}

	A3DCOLORVALUE flash(m_fMinLum, m_fFlashPower,0.0f, 0.f);
	m_pA3DDevice->SetPixelShaderConstants(6, &flash, 1);
}

void CRender::CalculateGaussian(float radius, float weights[3])
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

void CRender::SetupVertexConsts(float cx, float cy, float dx, float dy)
{
	A3DVECTOR4 c0(dx, dy, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(0, &c0, 1);
	A3DVECTOR4 c1(cx, cy, 0.0f, 0.0f);
	m_pA3DDevice->SetVertexShaderConstants(1, &c1, 1);
}

void CRender::SetFlashPower(float fMinLum, float vStart, float vEnd, int nTime)
{
	m_fMinLum = max(fMinLum,0.0f);
	m_fFlashPower = max(vStart, 0.0f);
	m_fFlashPowerSet = max(vEnd, 0.0f);
	m_fFlashPowerStep = (m_fFlashPowerSet - m_fFlashPower) / nTime;
	m_nFlashTime = nTime;
}

void CRender::Update(int iDeltaTime)
{
	if( m_nFlashTime > 0)
	{
		m_nFlashTime -=iDeltaTime;
		if( m_nFlashTime > 0)
			m_fFlashPower += m_fFlashPowerStep * iDeltaTime;
		else
		{
			m_fFlashPower = m_fFlashPowerSet;
			m_nFlashTime = 0;
		}
	}
}
#ifdef _ANGELICA22
void CRender::SetColorMappingTex(AString& strTexture)
{
	if(strTexture.IsEmpty())
	{
		//if(m_pColorMapping) m_pColorMapping->Release();
		//m_pColorMapping = NULL;
		A3DRELEASE(m_pColorMappingVolTex);
		m_strColorMapping = strTexture;
		return;
	}

	if(m_strColorMapping == strTexture) return;
	
	//if(m_pColorMapping) m_pColorMapping->Release();
	A3DRELEASE(m_pColorMappingVolTex);

	m_pColorMappingVolTex = new A3DVolumeTexture;

	if( ! m_pColorMappingVolTex->Init(m_pA3DDevice) ||
		! m_pColorMappingVolTex->Load("Textures\\" + strTexture)) {
		A3DRELEASE(m_pColorMappingVolTex);
		g_Log.Log("CRender::SetColorMappingTex(), …Ë÷√—’…´”≥…‰Œƒº˛%s ß∞‹£°", strTexture);
	}

	//if (D3D_OK != D3DXCreateVolumeTextureFromFileExA(
	//	m_pA3DDevice->GetD3DDevice(), "Textures\\" + strTexture, D3DX_DEFAULT, D3DX_DEFAULT, D3DX_DEFAULT, 1, 0, 
	//	D3DFMT_UNKNOWN, D3DPOOL_MANAGED, D3DX_DEFAULT, D3DX_DEFAULT, 0, NULL, NULL, &m_pColorMapping))
	//{
	//	g_Log.Log("CRender::SetColorMappingTex(), …Ë÷√—’…´”≥…‰Œƒº˛%s ß∞‹£°", strTexture);
	//	return;
	//}
	m_strColorMapping = strTexture;
}
#endif // #ifdef _ANGELICA22