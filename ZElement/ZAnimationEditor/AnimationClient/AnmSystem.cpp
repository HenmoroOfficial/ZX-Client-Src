#include "AnmSystem.h"
#include "AnmUtility.h"
#include "Animator.h"
#include "AnmObjLoader.h"
#include "EC_Global.h"
#include "A3DRenderTarget.h"
#include "EC_Configs.h"
#include "EC_GameRun.h"
// #include "EC_ShadowRender.h"
#include "EC_FullGlowRender.h"
#include "EC_GFXCaster.h"
#include "EC_Instance.h"
#include "EC_World.h"
// #include "EC_SceneLoader.h"
#include "EC_SceneBlock.h"
// #include "EL_BackMusic.h"
#include "EC_SunMoon.h"
// #include "EC_ModelMan.h"
// #include "EC_SceneLoader.h"
#include <LuaState.h>
//#include <A3DFTFont.h>
#include <crtdbg.h>
#include <atlconv.h>

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#define CLEAR_COLOR 0xFF606060

const char* cfg_preload_scripts[] =
{
	"script\\instance_client.lua",
		"script\\instance_common.lua",
		NULL
};

static bool _LuaPreLoad( const char** preload_scripts, CLuaState* pState )
{
	ASSERT( preload_scripts );
	ASSERT( pState );
	
	pState->Lock();
	bool bRet = true;
	int i = 0;
	while( preload_scripts[i] )
	{
		if( af_IsFileExist( preload_scripts[i] ) )
		{
			CLuaScript* pScript = pState->RegisterFile( preload_scripts[i] );
			if( ! pScript )
			{
				ASSERT(0);
				bRet = false;
				break;
			}
		}		
		++ i;
	}
	pState->Unlock();
	return bRet;
}


CAnmSystem::CAnmSystem()
{
	m_hInstance				= 0;
	m_hWnd					= 0;
	m_pObjLoader			= 0;
	m_dwTime				= 0;
	m_dwFrames				= 0;
	m_dwDeltaTime			= 0;
	m_fPlayedTime			= 0;

	m_pEngine = 0;
	m_pDevice = 0;
	m_pECViewport = 0;
	m_pGfxExMan = 0;
	m_pWorld = 0;
	m_pDirLight = 0;
	m_pCamera = 0;
	m_pGfxCaster = 0;
	m_pSkinRender1 = 0;
	m_pSkinRender2 = 0;
}

bool CAnmSystem::Init( HINSTANCE hInst, HWND hWnd )
{
	m_hInstance		= hInst;
	m_hWnd			= hWnd;

	m_pEngine = AfxGetA3DEngine();
	m_pDevice = AfxGetA3DDevice();
	m_pECViewport = AfxGetViewport();
	m_pGfxExMan = AfxGetGFXExMan();
	m_pDirLight = AfxGetDirLight();
	m_pCamera = AfxGetA3DCamera();
	m_pViewport = m_pECViewport->GetA3DViewport();
	m_pSkinRender1 = AfxGetSkinRender1();
	m_pSkinRender2 = AfxGetSkinRender2();
	m_pGfxCaster = AfxGetGfxCaster();

// 	RECT rc;
// 	::GetClientRect( m_hWnd, & rc );
// 	m_iWndWidth		= rc.right  - rc.left;
// 	m_iWndHeight	= rc.bottom - rc.top;
// 
// 	GetCurrentDirectoryA( MAX_PATH, m_szWorkDir );
// 	af_SetBaseDir( m_szWorkDir );
// 
// 	a_InitRandom();
// 
// 	if( ! InitA3DEngine() )
// 		return false;
// 	if( ! InitGfxExMan() )
// 		return false;
// 	if( ! InitModelMan() )
// 		return false;
// 	if( ! InitSkinRender() )
// 		return false;
// 	if( ! InitShadowRender() )
// 		return false;
// 	if( ! InitGfxCaster() )
// 		return false;
// 	if( ! InitFullGlowRender() )
// 		return false;
// 	if( ! InitInstanceInfo() )
// 		return false;
// 	if( ! InitBackMusic() )
// 		return false;
// 
	m_pObjLoader = new CAnmObjLoader;
	if( ! m_pObjLoader->Init() )
		return false;

	g_Animator.Init( this );

	AIniFile iniFile;
	BYTE effectLevel = 4;
	if (iniFile.Open("configs\\animations\\anm.ini"))
	{
		effectLevel = iniFile.GetValueAsInt("VIDEO", "Effect", 4);
	}
	EC_VIDEO_SETTING vsTemp;
	vsTemp = AfxGetConfigs()->GetVideoSettings();
	vsTemp.nEffect = effectLevel;
	AfxGetConfigs()->SetVideoSettings(vsTemp);

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
// 	ZeroMemory( m_szWorkDir, sizeof( m_szWorkDir ) );
// 	m_dwFrames		= 0;
// 	m_dwDeltaTime	= 0;
// 	m_fPlayedTime	= 0;
// 	ReleaseInstanceInfo();
// 	ReleaseFullGlowRender();
// 	ReleaseBackMusic();
// 	ReleaseShadowRender();
// 	ReleaseGfxCaster();
// 	ReleaseGfxExMan();
// 	ReleaseModelMan();
// 	ReleaseSkinRender();
// 	ReleaseA3DEngine();
}

void CAnmSystem::Tick( DWORD dwDeltaTime )
{
	m_dwDeltaTime = dwDeltaTime;

	if( ! m_pWorld )
		return;
	
	if( ! m_pDevice->GetAutoResetFlag() && m_pDevice->GetNeedResetFlag() && m_pDevice->GetCanResetFlag() )
	{		
		if( m_pDevice->Reset() )
		{
			m_pEngine->BeginRender();
			m_pDevice->Clear( D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0x00000000, 1.0f, 0 );
			m_pEngine->EndRender();
			m_pEngine->Present();
		}
	}
	m_dwDeltaTime = dwDeltaTime;
	
	g_Animator.Tick( dwDeltaTime );
	TickWorld( dwDeltaTime );
	m_pGfxCaster->Tick(dwDeltaTime);
	m_pGfxExMan->Tick( dwDeltaTime );
	m_pEngine->TickAnimation();
	
	static DWORD dwEarTime = 0;
	DWORD dwEarUpdateTime = 50;
	if( ( dwEarTime += dwDeltaTime ) >= dwEarUpdateTime )
	{
		dwEarTime -= dwEarUpdateTime;
		A3DVECTOR3 vecDir = m_pCamera->GetDirH();
		A3DVECTOR3 vecPos = m_pCamera->GetPos();
		A3DVECTOR3 vecUp  = A3DVECTOR3( 0.0f, 1.0f, 0.0f );
		AM3DSoundDevice* pAM3DSoundDevice = m_pEngine->GetAMSoundEngine()->GetAM3DSoundDevice();
		if( pAM3DSoundDevice )
		{
			pAM3DSoundDevice->SetPosition( vecPos );
			pAM3DSoundDevice->SetOrientation( vecDir, vecUp );
			pAM3DSoundDevice->UpdateChanges();
		}
	}
	
	if( g_Animator.GetState() == CAnimator::STATE_PLAYING )
		m_fPlayedTime += 0.001f * dwDeltaTime;
}

void CAnmSystem::Render()
{
	if( ! m_pWorld )
		return;
	
	if( m_pDevice->GetNeedResetFlag() )
	{
		m_pDevice->BeginRender();
		m_pDevice->Clear( D3DCLEAR_TARGET, 0, 1, 0 );
		m_pDevice->EndRender();
		m_pDevice->Present();
		return;
	}
	m_pEngine->BeginRender();
		
	m_pECViewport->Activate( false );
	
	
	m_pDevice->Clear( D3DCLEAR_ZBUFFER, 0xff000000, 1, 0 );
	
//	g_Animator.Render();
	RenderWorld();
	
	m_pGfxCaster->Render( m_pECViewport );
	m_pEngine->RenderGFX( m_pECViewport->GetA3DViewport(), 0xffffffff );
	m_pGfxExMan->RenderAllSkinModels( m_pECViewport->GetA3DViewport() );
	

	m_pSkinRender1->Render( m_pViewport, false );
	m_pSkinRender1->ResetRenderInfo( false );
	m_pSkinRender1->RenderAlpha( m_pViewport );
	m_pSkinRender1->ResetRenderInfo( true );	
	
	m_pGfxExMan->RenderAllGfx( m_pViewport );
	m_pGfxExMan->RenderECMForMotionBlur( m_pViewport );
	
	m_pDevice->SetDeviceRenderState( D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA );
	
	
	m_pEngine->GetA3DWireCollector()->Flush();
	m_pEngine->GetA3DFlatCollector()->Flush();
	
	m_pGfxExMan->Register2DGfx();
	m_pGfxExMan->Render2DGfx( true );
	m_pGfxExMan->Render2DGfx( false );
		
	g_Animator.RenderWire( m_pViewport );
	if( ! g_Animator.IsDisabledCamera() )
		g_Animator.RenderMask( m_pViewport );
	g_Animator.RenderText( m_pViewport );
	RenderScreenInfo();
	
	m_pGfxExMan->ResetECMForMotionBlur();	
	
	m_pEngine->GetSystemFont()->Flush();
	
	if( m_pSkinRender1 )
	{
		m_pSkinRender1->ResetRenderInfo( false );
		m_pSkinRender1->ResetRenderInfo( true );
	}
	if( m_pSkinRender2 )
	{
		m_pSkinRender2->ResetRenderInfo( false );
		m_pSkinRender2->ResetRenderInfo( true );
	}
	m_pEngine->EndRender();
	m_pEngine->Present();
	
	
	m_pGfxExMan->ResetRender();
	
	++ m_dwFrames;
}

bool CAnmSystem::LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos )
{
// 	CECInstance* pInst = AfxGetGameRun()->GetInstance(iInstID);
// 	if (!pInst)
// 	{
// 		a_LogOutput( 1, "CAnmSystem::LoadWorld, ¼ÓÔØ%dºÅµØÍ¼Ê§°Ü£¡", iInstID );
// 		return false;
// 	}
	
	if(!AfxGetGameRun()->JumpToInstance(iInstID, c_vCameraPos))
	{
		a_LogOutput( 1, "CAnmSystem::LoadWorld, ³õÊ¼»¯%dºÅµØÍ¼Ê§°Ü£¡", iInstID );
		return false;
	}

	m_pWorld = AfxGetGameRun()->GetWorld();
	
	float fBlockW  = 16;
	float fHeight1 = m_pWorld->GetTerrainHeight( c_vCameraPos + A3DVECTOR3(-fBlockW,0, fBlockW) );
	float fHeight2 = m_pWorld->GetTerrainHeight( c_vCameraPos + A3DVECTOR3( fBlockW,0, fBlockW) );
	float fHeight3 = m_pWorld->GetTerrainHeight( c_vCameraPos + A3DVECTOR3(-fBlockW,0,-fBlockW) );
	float fHeight4 = m_pWorld->GetTerrainHeight( c_vCameraPos + A3DVECTOR3( fBlockW,0,-fBlockW) );
	float fAvgHei  = ( fHeight1 + fHeight2 + fHeight3 + fHeight4 ) / 4.0f;
	float fHeight  = m_pWorld->GetTerrainHeight( c_vCameraPos );
	if( fHeight <= fAvgHei )
		fHeight = fAvgHei;

	A3DVECTOR3 vPos = c_vCameraPos;
	vPos.y = fHeight + 10.0f;

	m_pCamera->SetPos( vPos );
	m_pCamera->SetDirAndUp( A3DVECTOR3( 0, -1.0f, -3.0f ), g_vAxisY );
	return true;
}

void CAnmSystem::UnloadWorld()
{
	if( ! m_pWorld )
		return;
	m_pWorld->Release();
	delete m_pWorld;
	m_pWorld = NULL;
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

#ifdef BACK_VERSION
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
#else
bool CAnmSystem::RenderObject( CAnmObject* pObject, A3DViewport* pA3DViewport, bool bRenderShadow )
{
	switch( pObject->GetObjectType() )
	{
	case CAnmObject::TYPE_NPC:
		return RenderNPC( ( CAnmNPC* )pObject, pA3DViewport, bRenderShadow );
	case CAnmObject::TYPE_PLAYER:
		return RenderPlayer( ( CAnmPlayer* )pObject, pA3DViewport, bRenderShadow );
	case CAnmObject::TYPE_GFX:
		return RenderGFX( ( CAnmGFX* )pObject, pA3DViewport, bRenderShadow );
	default:
		return false;
	}
}
#endif

CECInstance* CAnmSystem::GetInstance( int iInstID )
{
	return AfxGetGameRun()->GetInstance(iInstID);
}

void CAnmSystem::TickWorld( DWORD dwDeltaTime )
{
	if( ! m_pWorld )
		return;
	m_pEngine->GetA3DSkinMan()->SetWorldCenter(GetA3DCamera()->GetPos());
	m_pWorld->Tick( dwDeltaTime, m_pECViewport );
}

void CAnmSystem::RenderWorld()
{
#ifdef BACK_VERSION
	if( m_pWorld )
		m_pWorld->Render( m_pECViewport );
#endif
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

#ifdef BACK_VERSION
bool CAnmSystem::RenderNPC( CAnmNPC* pNPC, bool bRenderShadow )
#else
bool CAnmSystem::RenderNPC( CAnmNPC* pNPC, A3DViewport* pA3DViewport, bool bRenderShadow )
#endif
{
	if( ! pNPC->m_pModel )
		return true;
	if( ! pNPC->IsVisible() )
		return true;
	if( pNPC->m_bHooked )
		return true;
#ifdef BACK_VERSION
	A3DViewport* viewport = AfxGetViewport()->GetA3DViewport();
	pNPC->m_pModel->Render( viewport );
#else
	pNPC->m_pModel->Render( pA3DViewport );
#endif
	return true;
}

#ifdef BACK_VERSION
bool CAnmSystem::RenderPlayer( CAnmPlayer* pPlayer, bool bRenderShadow )
{
	return RenderNPC( pPlayer, bRenderShadow );
}

bool CAnmSystem::RenderGFX( CAnmGFX* pGFX, bool bRenderShadow )
#else

bool CAnmSystem::RenderPlayer( CAnmPlayer* pPlayer, A3DViewport* pA3DViewport, bool bRenderShadow )
{
	return RenderNPC( pPlayer, pA3DViewport, bRenderShadow );
}

bool CAnmSystem::RenderGFX( CAnmGFX* pGFX, A3DViewport* pA3DViewport, bool bRenderShadow )
#endif
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
	AfxGetGFXExMan()->RegisterGfx( pGFX->m_pA3DGFX );
	return true;
}

void CAnmSystem::NotifyWndSizeChanged( int iWndWidth,int iWndHeight )
{
//	SuspendLoadThread();
	if( m_pEngine )
		AfxGetA3DEngine()->SetDisplayMode(iWndWidth,iWndHeight, A3DFMT_X8R8G8B8,true, false, SDM_WIDTH | SDM_HEIGHT | SDM_FORMAT | SDM_WINDOW | SDM_VSYNC);

//	StartLoaderThread();

	if( m_pECViewport )
		m_pECViewport->Move(0, 0, iWndWidth, iWndHeight, false);
	
	if( m_pGfxExMan )
		m_pGfxExMan->Resize2DViewport();
}

void CAnmSystem::RenderScreenInfo()
{
	USES_CONVERSION;
	A3DViewport* pViewport = AfxGetViewport()->GetA3DViewport();
	A3DCamera*	pCamera = AfxGetA3DCamera();
	A3DFont* pFont = AfxGetA3DEngine()->GetSystemFont();

	A3DVIEWPORTPARAM* pParam = pViewport->GetParam();

	// FPS
	static char s_szFPS[16];
	DWORD dwCurTime = a_GetTime();
	if( dwCurTime - m_dwTime >= 1000 )
	{
		sprintf( s_szFPS, "FPS: %d", m_dwFrames );
		m_dwFrames = 0;
		m_dwTime = dwCurTime;
	}
	pFont->TextOut( pParam->X + pParam->Width - 50, pParam->Y + 10, A2T( s_szFPS ), 0xFFFF0000 );

	// timer
	static char s_szTimer[16];
	sprintf( s_szTimer, "Timer: %.3fs", m_fPlayedTime );
	pFont->TextOut( pParam->X + 10, pParam->Y + 10, A2T( s_szTimer ), 0xFFFF0000 );

	// position
	const A3DVECTOR3& c_vPos = pCamera->GetPos();
	static char s_szCoord[32];
	sprintf( s_szCoord, "Pos: %.2f    %.2f    %.2f", c_vPos.x, c_vPos.y, c_vPos.z );
	pFont->TextOut( pParam->X + 200, pParam->Y + 10, A2T( s_szCoord ), 0xFFFF0000 );
}


CAnmSystem& GetAnmSystem()
{
	static CAnmSystem s_System;
	return s_System;
}

A3DEngine* CAnmSystem::GetA3DEngine()
{
	return AfxGetA3DEngine();
}

A3DCamera* CAnmSystem::GetA3DCamera()
{
	return AfxGetA3DCamera();
}

A3DViewport* CAnmSystem::GetA3DViewport()
{
	return AfxGetViewport()->GetA3DViewport();
}
CECGameRun* CAnmSystem::GetGameRun()
{
    return AfxGetGameRun();
}

