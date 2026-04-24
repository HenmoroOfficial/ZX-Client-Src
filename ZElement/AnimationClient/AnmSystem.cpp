#include "AnmSystem.h"
#include "AnmUtility.h"
#include "AnmObjLoader.h"
#include "Animator.h"
#include "EC_Global.h"
// #include "EC_ShadowRender.h"
// #include "EC_FullGlowRender.h"
// #include "EC_GFXCaster.h"
// #include "EC_Instance.h"
 #include "EC_Viewport.h"
// #include "EC_World.h"
// #include "EC_SceneLoader.h"
// #include "EC_SceneBlock.h"
// #include "EL_BackMusic.h"
// #include "EC_SunMoon.h"
// #include "EC_ModelMan.h"
// #include "EC_SceneLoader.h"
#include <LuaState.h>
#include <A3DFTFont.h>
#include <crtdbg.h>
#include <atlconv.h>

#define CLEAR_COLOR 0xFF606060


CAnmSystem::CAnmSystem()
{
	m_hInstance				= 0;
	m_hWnd					= 0;
	m_iWndWidth				= 1024;
	m_iWndHeight			= 768;
	m_pA3DEngine				= 0;
	m_pA3DDevice				= 0;
	m_pDirLight				= 0;
	m_pDynamicLight			= 0;
	m_pvsSMEdge				= 0;
	m_pvsRMEdge				= 0;
	m_pvsObstructSkinModel	= 0;
	m_pvsObstructRigidModel	= 0;
	m_pEdgeObPS				= 0;
	m_pEdgePS				= 0;
	m_pGlowPS				= 0;
	m_pEdgeGlowShader		= 0;
	m_pSkinRender1			= 0;
	m_pSkinRender2			= 0;
	m_pShadowRender			= 0;
	m_pFullGlowRender		= 0;
	m_pGFXCaster			= 0;
	m_pFTFontMan			= 0;
	m_pDefaultFont			= 0;
	m_pBackMusic			= 0;
	m_pGfxExMan				= 0;
	m_pModelMan				= 0;
	m_dwAmbient				= 0xFFCCCCCC;
	m_bFogEnable			= true;
	m_fFogStart				= 60;
	m_fFogEnd				= 400;
	m_fFogDensity			= 1;
	m_dwFogColor			= 0xFFCCCCCC;
	m_dwCammaLevel			= 100;
	m_vDirLightDir			= A3DVECTOR3( 0.15f, -0.7f, 0.695f );
	m_bMipMapBias			= true;
	m_bUpdateShadowMap		= false;
	m_bOptimizeShadowMap	= false;
	ZeroMemory( m_szWorkDir, sizeof( m_szWorkDir ) );
	m_dwTime				= 0;
	m_dwFrames				= 0;
	m_dwDeltaTime			= 0;
	m_fPlayedTime			= 0;
	m_pECViewport			= 0;
	m_pViewport				= 0;
	m_pCamera				= 0;
	m_pObjLoader			= 0;
	m_pWorld				= 0;
}

bool CAnmSystem::Init( HINSTANCE hInst, HWND hWnd )
{
	m_hInstance		= hInst;
	m_hWnd			= hWnd;

	RECT rc;
	::GetClientRect( m_hWnd, & rc );
	m_iWndWidth		= rc.right  - rc.left;
	m_iWndHeight	= rc.bottom - rc.top;

	GetCurrentDirectoryA( MAX_PATH, m_szWorkDir );
	af_SetBaseDir( m_szWorkDir );

	a_InitRandom();

	if( ! InitA3DEngine() )
		return false;
	if( ! InitFTFontMan() )
		return false;
	if( ! InitGfxExMan() )
		return false;
	if( ! InitModelMan() )
		return false;
	if( ! InitSkinRender() )
		return false;
	if( ! InitShadowRender() )
		return false;
	if( ! InitGfxCaster() )
		return false;
	if( ! InitFullGlowRender() )
		return false;
	if( ! InitInstanceInfo() )
		return false;
	if( ! InitBackMusic() )
		return false;

	m_pObjLoader = new CAnmObjLoader;
	if( ! m_pObjLoader->Init() )
		return false;

	g_Animator.Init( this );

#ifdef _DEBUG
	const ACHAR* szCmdLine = ::GetCommandLine();
	if( ! szCmdLine )
		return true;
	const ACHAR* szCmd;
	char szPDBFilePath[1024] = {0};
	if( ( szCmd = a_strstr( szCmdLine, _AL( "pdb:" ) ) ) )
	{
		ACString strPath( szCmd + 4 );
		if( strPath != _AL("y") )
			strcpy( szPDBFilePath, AC2AS( strPath ).GetBuffer(0) );
		EnablePDBMemDebug( szPDBFilePath );
	}
#endif
	return true;
}

void CAnmSystem::Release()
{
	g_Animator.Release();

	A3DRELEASE( m_pObjLoader );
	ZeroMemory( m_szWorkDir, sizeof( m_szWorkDir ) );
	m_dwFrames		= 0;
	m_dwDeltaTime	= 0;
	m_fPlayedTime	= 0;
	ReleaseInstanceInfo();
	ReleaseFullGlowRender();
	ReleaseBackMusic();
	ReleaseShadowRender();
	ReleaseGfxCaster();
	ReleaseGfxExMan();
	ReleaseModelMan();
	ReleaseSkinRender();
	ReleaseFTFontMan();
	ReleaseA3DEngine();
}

void CAnmSystem::Tick( DWORD dwDeltaTime )
{
}

void CAnmSystem::Render()
{

}

bool CAnmSystem::LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos )
{

	return true;
}

void CAnmSystem::UnloadWorld()
{

}

bool CAnmSystem::LoadObject( CAnmObject* pObject ) 
{ 
	return m_pObjLoader->LoadObject( pObject ); 
}

void CAnmSystem::UnloadObject( CAnmObject* pObject ) 
{
	m_pObjLoader->UnloadObject( pObject ); 
}

bool CAnmSystem::TickObject( CAnmObject* pObject, DWORD dwDeltaTime )
{
	switch( pObject->GetObjectType() )
	{
	case CAnmObject::TYPE_NPC:
		return TickNPC( ( CAnmNPC* )pObject, dwDeltaTime );
	case CAnmObject::TYPE_PLAYER:
		return TickPlayer( ( CAnmPlayer* )pObject, dwDeltaTime );
	case CAnmObject::TYPE_GFX:
		return TickGFX( ( CAnmGFX* )pObject, dwDeltaTime );
	default:
		return false;
	}
}

bool CAnmSystem::RenderObject( CAnmObject* pObject, bool bRenderShadow )
{
	switch( pObject->GetObjectType() )
	{
	case CAnmObject::TYPE_NPC:
		return RenderNPC( ( CAnmNPC* )pObject, bRenderShadow );
	case CAnmObject::TYPE_PLAYER:
		return RenderPlayer( ( CAnmPlayer* )pObject, bRenderShadow );
	case CAnmObject::TYPE_GFX:
		return RenderGFX( ( CAnmGFX* )pObject, bRenderShadow );
	default:
		return false;
	}
}

bool CAnmSystem::RenderHightLightModelList( A3DViewport* pViewport )
{
	if( m_vHLModel.empty() )
		return true;
	m_pA3DDevice->SetTextureFilterType( 0, A3DTEXF_LINEAR );
	for( size_t i = 0; i < m_vHLModel.size(); ++ i )
	{
		const HLModel& model = m_vHLModel[i];
		//glb_RenderHightLightModel_Edge( pViewport, model.pModel, model.clr, model.fEdgeLen );
	}
	m_vHLModel.clear();
	return true;
}

bool CAnmSystem::InitA3DEngine()
{
	if( m_pA3DEngine )
		return true;

	//	Init Engine
	if (!(m_pA3DEngine = new A3DEngine))
	{
//		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}
	
	A3DDEVFMT devFmt;
	devFmt.bWindowed	= true;
	devFmt.nWidth		= m_iWndWidth;
	devFmt.nHeight		= m_iWndHeight;
	devFmt.fmtTarget	= A3DFMT_UNKNOWN;
	devFmt.fmtDepth		= A3DFMT_UNKNOWN;
	devFmt.bVSync		= false;
//	devFmt.bVSync		= false;
						
	// A3DDEV_ALPHATARGETFIRST: if we use alpha target first and the desktop display mode is just what we use 
	// as the full-screen mode for the game, the game may not minimized when switched off. To solve this, we should
	// add a manual call to ChangeDisplaySetting when we receive WM_ACTIVATEAPP
	DWORD devFlags = A3DDEV_ALLOWMULTITHREAD;
//	if( g_d3dcaps.PixelShaderVersion >= D3DPS_VERSION(1, 4) )
		devFlags |= A3DDEV_ALPHATARGETFIRST;

// 	if (gr.get_state() == game_record::enum_gr_record
// 	 || gr.get_state() == game_record::enum_gr_replay)
// 		devFlags |= A3DDEV_FPU_PRESERVE;

	if (!m_pA3DEngine->Init(m_hInstance, m_hWnd, &devFmt, devFlags))
	{
//		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}

	g_pA3DConfig->RT_SetShowFPSFlag(false);
	g_pA3DConfig->SetFlagLinearSprite(true);

	g_pA3DConfig->SetTextureQuality((A3DTEXTURE_QUALITY)A3DTEXTURE_QUALITY_MEDIUM);
	m_pA3DEngine->GetAMEngine()->GetAMConfig()->SetSoundQuality((AMSOUND_QUALITY)AMSOUND_QUALITY_MEDIUM);

	m_pA3DDevice = m_pA3DEngine->GetA3DDevice();

	// In element client we use multithread to load terrain meshes etc.
	// so the reset operation can not be done by the device automatically, or we may not release all unmanaged
	// device objects before we reset, so reset will fail.
	m_pA3DDevice->SetAutoResetFlag(false);

	// show a black screen without loading text
	m_pA3DDevice->BeginRender();
	m_pA3DDevice->Clear(D3DCLEAR_TARGET, 0x00000000, 1.0f, 0);
	m_pA3DDevice->EndRender();
	m_pA3DDevice->Present();

	//	Create viewport
	if (!(m_pECViewport = new CECViewport))
	{
//		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}

	CECViewport::INITPARAMS ViewParams;
	memset(&ViewParams, 0, sizeof (ViewParams));

	ViewParams.rcView.left		= 0;
	ViewParams.rcView.top		= 0;
	ViewParams.rcView.right		= m_iWndWidth;
	ViewParams.rcView.bottom	= m_iWndHeight;
	ViewParams.fMinZ			= 0.0f;
	ViewParams.fMaxZ			= 1.0f;
	ViewParams.bClearColor		= false;
	ViewParams.bClearZ			= true;
	ViewParams.colClear			= 0x00000000;
	ViewParams.fFOV				= DEG2RAD(DEFCAMERA_FOV);
	ViewParams.fNearPlane		= 0.2f;
	ViewParams.fFarPlane		= 2000.0f;


	float w = (float)m_iWndWidth;
	float h = (float)m_iWndHeight;
	ViewParams.bOrthoCamera	= true;
	ViewParams.fOCLeft		= -w;
	ViewParams.fOCRight		= w;
	ViewParams.fOCBottom	= -h;
	ViewParams.fOCTop		= h;
	ViewParams.fOCZNear		= -1500.0f;
	ViewParams.fOCZFar		= 1500.0f;

	if (!m_pECViewport->Init(ViewParams))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECGame::InitA3DEngine", __LINE__);
		return false;
	}
	m_pECViewport->Activate(false);

	//	Create directional light
	A3DLIGHTPARAM LightParams;
	memset(&LightParams, 0, sizeof (LightParams));

	//	Set default light parameters
	LightParams.Type = A3DLIGHT_DIRECTIONAL;
	LightParams.Direction = m_vDirLightDir;
	LightParams.Diffuse = 0xFFCCCCCC;
	LightParams.Specular = 0xFFCCCCCC;
	LightParams.Attenuation0 = 1.0f;
	if(!m_pA3DEngine->GetA3DLightMan()->CreateLight(0, &m_pDirLight))
	{
		return false;
	}
	m_pDirLight->SetLightParam(LightParams);
	m_pDirLight->TurnOn();
	
	//	Create dynamic point light
	LightParams.Type = A3DLIGHT_POINT;
	LightParams.Specular = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
	if(!m_pA3DEngine->GetA3DLightMan()->CreateLight(1, &m_pDynamicLight))
	{
		return false;
	}
	m_pDynamicLight->SetLightParam(LightParams);
	m_pDynamicLight->TurnOff();
	
	//	Set default ambient value and fog parameters
	m_pA3DDevice->SetAmbient( m_dwAmbient );
	m_pA3DDevice->SetFogEnable( m_bFogEnable );
	m_pA3DDevice->SetFogStart( m_fFogStart );
	m_pA3DDevice->SetFogEnd( m_fFogEnd );
	m_pA3DDevice->SetFogDensity( m_fFogDensity );
	m_pA3DDevice->SetFogColor( m_dwFogColor );
	m_pA3DDevice->SetGammaLevel( m_dwCammaLevel );
	
	//	Load skin model shaders
	A3DSkinMan* pSkinMan = m_pA3DEngine->GetA3DSkinMan();
	if (!pSkinMan->LoadSkinModelVertexShaders("Configs\\skinmodelvs.cfg"))
	{
		a_LogOutput(1, "CECGame::InitA3DEngine, Failed to load skinmodelvs.cfg");
		return false;
	}
	
	pSkinMan->SetDirLight(m_pDirLight);
	pSkinMan->SetDPointLight(m_pDynamicLight);
	
	float v = -1.0f;
	for(int i=0; i<m_pA3DDevice->GetMaxSimultaneousTextures(); i++)
	{
		m_pA3DDevice->SetDeviceTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *(DWORD *)&v);
	}

	return true;
}

void CAnmSystem::ReleaseA3DEngine()
{
	if( ! m_pA3DEngine )
		return;
}

bool CAnmSystem::InitFTFontMan()
{
	return true;
}

void CAnmSystem::ReleaseFTFontMan()
{
	m_pDefaultFont = 0;
	A3DRELEASE( m_pFTFontMan );
}

bool CAnmSystem::InitGfxExMan()
{
	return true;
}

void CAnmSystem::ReleaseGfxExMan()
{
	A3DRELEASE( m_pGfxExMan );
}

bool CAnmSystem::InitModelMan()
{
	return true;
}

void CAnmSystem::ReleaseModelMan()
{
	A3DRELEASE( m_pModelMan );
}

bool CAnmSystem::InitSkinRender()
{
	return true;
}

void CAnmSystem::ReleaseSkinRender()
{
	m_pSkinRender1 = 0;
	A3DRELEASE( m_pSkinRender2 );			
}

bool CAnmSystem::InitFullGlowRender()
{
	return true;
}

void CAnmSystem::ReleaseFullGlowRender()
{

}

bool CAnmSystem::InitGfxCaster()
{
	
	return true;
}

void CAnmSystem::ReleaseGfxCaster()
{
	
}

bool CAnmSystem::InitShadowRender()
{	
	return true;
}

void CAnmSystem::ReleaseShadowRender()
{
	
}

bool CAnmSystem::InitBackMusic()
{
	return true;
}

void CAnmSystem::ReleaseBackMusic()
{

}

bool CAnmSystem::InitInstanceInfo()
{
	
	return true;
}

void CAnmSystem::ReleaseInstanceInfo()
{
	
}

CECInstance* CAnmSystem::GetInstance( int iInstID )
{
	InstanceMap::const_iterator c_iterInst = m_mInstance.find( iInstID );
	return c_iterInst == m_mInstance.end() ? 0 : c_iterInst->second;
}

CECInstance* CAnmSystem::GetInstance( const char* szFile )
{
	return 0;
}

void CAnmSystem::TickWorld( DWORD dwDeltaTime )
{
	
}

void CAnmSystem::RenderWorld()
{
	
}

bool CAnmSystem::TickNPC( CAnmNPC* pNPC, DWORD dwDeltaTime )
{
	if( ! pNPC->m_pModel )
		return false;
	if( pNPC->m_bHooked )
		return true;
	pNPC->m_pModel->Tick( dwDeltaTime );
	return true;
}

bool CAnmSystem::TickPlayer( CAnmPlayer* pPlayer, DWORD dwDeltaTime )
{
	return TickNPC( pPlayer, dwDeltaTime );
}

bool CAnmSystem::TickGFX( CAnmGFX* pGFX, DWORD dwDeltaTime )
{
	if( pGFX->m_pA3DGFX->GetState() != ST_STOP )
		pGFX->m_pA3DGFX->TickAnimation( dwDeltaTime );
	return true;
}

bool CAnmSystem::RenderNPC( CAnmNPC* pNPC, bool bRenderShadow )
{
	if( ! pNPC->m_pModel )
		return true;
	if( ! pNPC->IsVisible() )
		return true;
	if( pNPC->m_bHooked )
		return true;
	pNPC->m_pModel->Render( m_pViewport );
	return true;
}

bool CAnmSystem::RenderPlayer( CAnmPlayer* pPlayer, bool bRenderShadow )
{
	return RenderNPC( pPlayer, bRenderShadow );
}

bool CAnmSystem::RenderGFX( CAnmGFX* pGFX, bool bRenderShadow )
{
	if( ! pGFX->m_pA3DGFX )
		return true;
	if( ! pGFX->IsVisible() )
		return true;
	if( pGFX->m_pA3DGFX->GetState() == ST_STOP )
		return true;
	if( ! pGFX->m_pA3DGFX->Is2DRender() )
	{
		pGFX->m_pA3DGFX->SetScale( pGFX->m_fScale );
		pGFX->m_pA3DGFX->SetAlpha( pGFX->m_fTransparent );
	}
	m_pGfxExMan->RegisterGfx( pGFX->m_pA3DGFX );
	return true;
}

void CAnmSystem::NotifyWndSizeChanged( int iWndWidth,int iWndHeight )
{
//	SuspendLoadThread();

	if( m_pA3DEngine )
		AfxGetA3DEngine()->SetDisplayMode(iWndWidth,iWndHeight, A3DFMT_X8R8G8B8,true, false, SDM_WIDTH | SDM_HEIGHT | SDM_FORMAT | SDM_WINDOW | SDM_VSYNC);

//	StartLoaderThread();

	if( m_pGfxExMan )
		m_pGfxExMan->Resize2DViewport();
}

void CAnmSystem::RenderScreenInfo()
{
	USES_CONVERSION;
	A3DVIEWPORTPARAM* pParam = m_pViewport->GetParam();

	// FPS
	static char s_szFPS[16];
	DWORD dwCurTime = a_GetTime();
	if( dwCurTime - m_dwTime >= 1000 )
	{
		sprintf( s_szFPS, "FPS: %d", m_dwFrames );
		m_dwFrames = 0;
		m_dwTime = dwCurTime;
	}
	m_pDefaultFont->TextOut( pParam->X + pParam->Width - 50, pParam->Y + 10, A2T( s_szFPS ), 0xFFFF0000 );

	// timer
	static char s_szTimer[16];
	sprintf( s_szTimer, "Timer: %.3fs", m_fPlayedTime );
	m_pDefaultFont->TextOut( pParam->X + 10, pParam->Y + 10, A2T( s_szTimer ), 0xFFFF0000 );

	// position
	const A3DVECTOR3& c_vPos = m_pCamera->GetPos();
	static char s_szCoord[32];
	sprintf( s_szCoord, "Pos: %.2f    %.2f    %.2f", c_vPos.x, c_vPos.y, c_vPos.z );
	m_pDefaultFont->TextOut( pParam->X + 200, pParam->Y + 10, A2T( s_szCoord ), 0xFFFF0000 );
}


CAnmSystem& GetAnmSystem()
{
	static CAnmSystem s_System;
	return s_System;
}
