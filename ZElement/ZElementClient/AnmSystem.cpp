#include "AnmSystem.h"
#include "AnmUtility.h"
#include "Animator.h"
#include "AnmObjLoader.h"
#include "AnmObjectManager.h"
#include "AnmCommonActionManager.h"
#include "EC_Global.h"
#include "A3DRenderTarget.h"
#include "EC_GameRun.h"
// #include "EC_ShadowRender.h"
#include "EC_FullGlowRender.h"
#include "EC_GFXCaster.h"
#include "EC_Instance.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
// #include "EC_SceneLoader.h"
#include "EC_SceneBlock.h"
// #include "EL_BackMusic.h"
// #include "EC_ModelMan.h"
// #include "EC_SceneLoader.h"
#include "EC_NewHelpGuide.h"
#include <LuaState.h>
//#include <A3DFTFont.h>
#include <crtdbg.h>
#include <atlconv.h>
#include "DlgCameraPath.h"
#include "EC_Game.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "EC_SunMoon.h"
#else
#include "ECViewport.h"
#include "A3DSunMoon.h"
#include "ElementRender.h"
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

	m_bUseUILetterbox = true;
	m_bLastShowUI = true;
	m_bLastEnableTextAlpha = true;
	m_curAnmConf = NULL;
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

	LoadAnimatorConf("configs\\Animations\\animation_files.txt");

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
	m_vecAnmConfs.clear();

 	A3DRELEASE( m_pObjLoader );
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

	TickWorld( dwDeltaTime );
	g_Animator.Tick( dwDeltaTime );
	
	//m_pGfxCaster->Tick(dwDeltaTime);
	//m_pGfxExMan->Tick( dwDeltaTime );
	//m_pEngine->TickAnimation();
	
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

	switch(g_Animator.GetState())
	{
	case CAnimator::STATE_PLAYING:
		m_fPlayedTime += 0.001f * dwDeltaTime;
		break;
	case CAnimator::STATE_WAITING:
		RestoreScene();
		break;
	default:
		break;
	}
}
#ifdef BACK_VERSION
void CAnmSystem::Render(bool bPresent)
#else
void CAnmSystem::Render(ECViewport* pViewport)
#endif
{
	if( ! m_pWorld )
		return;
#ifdef BACK_VERSION
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
	
	g_Animator.Render(m_pECViewport->GetA3DViewport(), true);
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
	if(bPresent)
	 m_pEngine->Present();
#else
    g_Animator.Render(pViewport->GetA3DViewport(), true);
#endif
	
	//m_pGfxExMan->ResetRender();
	
	++ m_dwFrames;
}

bool CAnmSystem::LoadWorld( int iInstID, const A3DVECTOR3& c_vCameraPos )
{
// 	CECInstance* pInst = AfxGetGameRun()->GetInstance(iInstID);
// 	if (!pInst)
// 	{
// 		a_LogOutput( 1, "CAnmSystem::LoadWorld, 加载%d号地图失败！", iInstID );
// 		return false;
// 	}
	
	CECGameRun* pGameRun = AfxGetGameRun();
	OnBeforePlayAnm();
	if(!pGameRun->JumpToInstanceForAnm(iInstID, c_vCameraPos))
	{
		a_LogOutput( 1, "CAnmSystem::LoadWorld, 初始化%d号地图失败！", iInstID );
		OnAfterPlayAnm();
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

void CAnmSystem::UpdateText(const ACString* pText)
{
	if (!g_Animator.IsShowLetterbox() || !m_bUseUILetterbox)
		return;

	if (AfxGetGameRun()->GetUIManager() && 
		AfxGetGameRun()->GetUIManager()->GetInGameUIMan())
	{
		CECGameUIMan* pUI = AfxGetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pUI->GetDialog("Win_CameraPath");
		if (pDlgCameraPath)
		{
			ACString szText = pText ? *pText : _AL(" ");
			if (szText.IsEmpty())
				szText = _AL(" ");
			pDlgCameraPath->ShowText(szText);
		}
	}
}

bool CAnmSystem::RenderLetterboxAndText()
{
	if (!g_Animator.IsShowLetterbox() || !m_bUseUILetterbox)
		return false;

	//	Render UI
	if( AfxGetGameRun()->GetUIManager()->GetCurrentUIManPtr() )
		AfxGetGameRun()->GetUIManager()->GetCurrentUIManPtr()->GetA3DFTFontMan()->Flush();
	if (AfxGetGameRun()->GetUIManager())
		AfxGetGameRun()->GetUIManager()->Render();
	return true;
}

CECInstance* CAnmSystem::GetInstance( int iInstID )
{
	return AfxGetGameRun()->GetInstance(iInstID);
}

void CAnmSystem::TickWorld( DWORD dwDeltaTime )
{
	if( ! m_pWorld )
		return;
	//m_pEngine->GetA3DSkinMan()->SetWorldCenter(GetA3DCamera()->GetPos());
	m_pWorld->TickForAnmSystem( dwDeltaTime, m_pECViewport );
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
	if(!pNPC->m_pModel->Render( pA3DViewport ))
	{
	   assert(0 && "CAnmSystem::RenderNPC");
	}
#endif
	A3DVECTOR3 pos = pNPC->GetPos();
	A3DVECTOR3 dir = pNPC->GetDir();
	float scale = pNPC->GetScale();
	float back = g_Animator.GetA3DCamera()->GetZBack();
	float front = g_Animator.GetA3DCamera()->GetZFront();
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
	/*
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
	*/
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
A3DVECTOR3 CAnmSystem::GetCenterPos()
{
   return GetA3DCamera()->GetPos();
}
void CAnmSystem::OnBeforePlayAnm()
{
	m_vsOrigin      = AfxGetConfigs()->GetVideoSettings();
	m_sysOrigin     = AfxGetConfigs()->GetSystemSettings();
	EC_VIDEO_SETTING  vsTemp = AfxGetConfigs()->GetVideoSettings();
	EC_SYSTEM_SETTING sysTemp = AfxGetConfigs()->GetSystemSettings();

	if(m_curAnmConf)
	{
		if(vsTemp.nEffect < m_curAnmConf->nEffectLevel)
		{
			vsTemp.nEffect = m_curAnmConf->nEffectLevel;
			AfxGetConfigs()->SetVideoSettings(vsTemp);
		}
	}

	//此处功能没有对老版本进行编写，如有需要需更改
#ifdef BACK_VERSION
	if (AfxGetGameRun()->GetWorld()->GetTerrainWater()->CanRelfect())
#endif
		sysTemp.nWaterEffect = 2;
#ifdef BACK_VERSION
	if (AfxGetGame()->GetShadowRender()->CanDoShadow())
	{
		//m_SysSetting.bSimpleTerrain = false;
		sysTemp.bShadow = true;
		sysTemp.bSimpleShadow = false;
		sysTemp.nCloudDetail = 4;
	}
#endif
	sysTemp.nTreeDetail = 4;
	sysTemp.nGrassDetail = 4;
	sysTemp.bMipMapBias = true;
	if (AfxGetGameRun()->GetFullGlowRender()->CanDoFullGlow())
	{
		sysTemp.bSunFlare = true;
		sysTemp.bFullGlow = true;
		sysTemp.bAdvancedWater = true;
	}
	sysTemp.bSimpleTerrain = false;

	sysTemp.iTexDetail = 0;

	//此处需要一个经验值
	sysTemp.nSight = 50;
	sysTemp.iDist_tree = 1000.0f;
	sysTemp.iDist_Water = 1000.0f;
	sysTemp.iDist_Buiding = 1000.0f;
	sysTemp.iDist_BoxArea = 1000.0f;
	sysTemp.iDist_Grass = 1000.0f;
	sysTemp.iDist_Effect = 1000.0f;
	sysTemp.iDist_Ecmodel = 1000.0f;
	sysTemp.iDist_Critter = 1000.0f;
	sysTemp.iDist_Bezzier = 1000.0f;
	sysTemp.iDist_Sound  = 1000.0f;
	AfxGetConfigs()->SetSystemSettings(sysTemp);
}
void CAnmSystem::OnAfterPlayAnm()
{
	const EC_SYSTEM_SETTING& sysTemp     = AfxGetConfigs()->GetSystemSettings();
	m_sysOrigin.iRndWidth = sysTemp.iRndWidth;
	m_sysOrigin.iRndHeight = sysTemp.iRndHeight;
	AfxGetConfigs()->SetVideoSettings(m_vsOrigin);
	AfxGetConfigs()->SetSystemSettings(m_sysOrigin);
}
bool CAnmSystem::Play(int id)
{
	SAnimatorConf* pAnimatorConf = GetAnimatorConfById(id);
	if(pAnimatorConf)
	{
		ACString path;
		path.Format(_AL("configs\\Animations\\%s"), pAnimatorConf->strName);
		return Play(path);
	}
	return false;
}

bool CAnmSystem::Play(const wchar_t* szFile)
{
	if (IsPlaying())
		return false;

	CECGameRun* pGameRun = AfxGetGameRun();
	CECHostPlayer* pHostPlayer = pGameRun->GetHostPlayer();
	m_iOriginInstanceId = pGameRun->GetWorld()->GetInstanceID();
	m_vecOriginPos = pHostPlayer->GetPos();
	m_vecOriginDir = pHostPlayer->GetDir();
	m_vecOriginUp = pHostPlayer->GetUp();
	m_bOriginWeaponHide = pHostPlayer->IsWeaponHide();
	AWString path = szFile;
	AWString sepName = path.Mid(a_strlen(_AL("configs\\Animations\\")));
	m_curAnmConf = GetAnimatorConfByName(sepName);
	
    g_Animator.Init(this);
	if (g_Animator.LoadFileFromEditor(szFile, A3DVECTOR3(0.0f)))
	{
		CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
		if (CAnmObject *pPlayer = pObjectManager->GetObject(1))
		{
			if (g_Animator.IsUseHostPlayer())
				g_Animator.SetHostPlayerModel(pHostPlayer->GetPlayerModel());
			CAnmObject *pCamera = pObjectManager->GetObject(0);
			pPlayer->SetPos(pCamera->GetPos() + pCamera->GetDir());
			pHostPlayer->HideWeapon(!g_Animator.IsShowHostPlayerWeapon());
			pHostPlayer->UpdateEquipSkins(true);
		}
		g_Animator.Play();
		ShowLetterbox();
		return true;
	}
	return false;
}

void CAnmSystem::Stop()
{
	g_Animator.Stop();
	m_pWorld = NULL;
	m_fPlayedTime = 0.0f;
	RestoreScene();
	m_curAnmConf = NULL;
}

void CAnmSystem::Pause()
{
	if (IsPlaying())
		g_Animator.Pause();
}

bool CAnmSystem::IsPlaying()
{
	return g_Animator.GetState() == CAnimator::STATE_PLAYING;
}

void CAnmSystem::RestoreScene()
{
	if (m_iOriginInstanceId == 0)
		return;

	m_pWorld = NULL;
	g_Animator.Release();
	CECGameRun* pGameRun = AfxGetGameRun();
	CECHostPlayer* pHostPlayer = pGameRun->GetHostPlayer();
	OnAfterPlayAnm();
	pGameRun->JumpToInstanceForAnm(m_iOriginInstanceId, m_vecOriginPos);
	pHostPlayer->SetPos(m_vecOriginPos);
	pHostPlayer->SetDirAndUp(m_vecOriginDir, m_vecOriginUp);
	pHostPlayer->GetPlayerModel()->StopAllActions(true);
	pHostPlayer->HideWeapon(m_bOriginWeaponHide);
	pHostPlayer->UpdateEquipSkins(true);
	m_iOriginInstanceId = 0;
	m_bOriginWeaponHide = false;
	m_vecOriginPos.Clear();
	m_vecOriginDir.Clear();
	m_vecOriginUp.Clear();
	HideLetterbox();

	int iLevel = g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel;
	//开场动画
	if(iLevel == 1 && NULL != m_curAnmConf)
	{
	   if(m_curAnmConf->iId == ANIMATION_TERRAN ||
		  m_curAnmConf->iId == ANIMATION_PROTOSS ||
		  m_curAnmConf->iId == ANIMATION_ELDAR)
	   {
		   NewHelpGuide::StartGuide_Welcome();
	   }
	}
}

// 设置主角职业
void CAnmSystem::SetHostPlayerSect(unsigned int mask_sect)
{
	int i = -1;
	while (mask_sect)
	{
		i ++;
		mask_sect >>= 1;
	}
	g_Animator.GetCommonActionManager()->SetHostPlayerSect(i);
}

// 设置主角性别
void CAnmSystem::SetHostPlayerMale(bool bMale)
{
	g_Animator.GetCommonActionManager()->SetHostPlayerGender(bMale);
}

// 显示字幕上下黑边
void CAnmSystem::ShowLetterbox()
{
	if (!g_Animator.IsShowLetterbox() || !m_bUseUILetterbox)
		return;

	if (AfxGetGameRun()->GetUIManager() && 
		AfxGetGameRun()->GetUIManager()->GetInGameUIMan())
	{
		CECGameUIMan* pUI = AfxGetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pUI->GetDialog("Win_CameraPath");
		if (pDlgCameraPath)
		{
			m_bLastEnableTextAlpha = pDlgCameraPath->IsEnableTextAlpha();
			pDlgCameraPath->EnableTextAlpha(false);
			pDlgCameraPath->ShowText(_AL(" "));
		}

		m_bLastShowUI = pUI->IsShowAllPanels();
		if (m_bLastShowUI)
			pUI->SetShowAllPanels(false);
	}
}

// 隐藏字幕上下黑边
void CAnmSystem::HideLetterbox()
{
	if (!g_Animator.IsShowLetterbox() || !m_bUseUILetterbox)
		return;

	if (AfxGetGameRun()->GetUIManager() && 
		AfxGetGameRun()->GetUIManager()->GetInGameUIMan())
	{
		CECGameUIMan* pUI = AfxGetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pUI->GetDialog("Win_CameraPath");
		if (pDlgCameraPath && pDlgCameraPath->IsShow())
		{
			pDlgCameraPath->Show(false);
			pDlgCameraPath->EnableTextAlpha(m_bLastEnableTextAlpha);
		}
		
		if (m_bLastShowUI)
			pUI->SetShowAllPanels(m_bLastShowUI);
	}
}

void CAnmSystem::LoadAnimatorConf(const char* szFile)
{
   	AWScriptFile s;
	ACString str;
	m_vecAnmConfs.clear();
	if(s.Open(szFile))
	{
		while(s.PeekNextToken(true))
		{
			SAnimatorConf anmConf;
			s.GetNextToken(true);
			str = s.m_szToken;
            anmConf.iId = str.ToInt();
			s.GetNextToken(false);
			str = s.m_szToken;
			anmConf.strName = AC2WC(str);
			s.GetNextToken(false);
			str = s.m_szToken;
			anmConf.nEffectLevel = str.ToInt();
			m_vecAnmConfs.push_back(anmConf);
		}
		s.Close();
	}
}
CAnmSystem::SAnimatorConf* CAnmSystem::GetAnimatorConfById(int id)
{
	AnimatorConfVector::iterator pos;
	for (pos = m_vecAnmConfs.begin();pos!=m_vecAnmConfs.end();++pos)
	{
		if((*pos).iId == id)
			return &(*pos);
	}
	return NULL;
}
CAnmSystem::SAnimatorConf* CAnmSystem::GetAnimatorConfByName(const wchar_t* szFile)
{
    AnimatorConfVector::iterator pos;
	for (pos = m_vecAnmConfs.begin();pos!=m_vecAnmConfs.end();++pos)
	{
		if((*pos).strName == szFile)
			return &(*pos);
	}
	return NULL;
}
#ifndef BACK_VERSION


ECViewport* CAnmSystem::GetECViewport()
{
	return m_pECViewport;
}

void CAnmSystem::RenderPost( ECViewport* pViewport)
{
	g_Animator.RenderWire( m_pViewport );
	if( ! g_Animator.IsDisabledCamera() )
		g_Animator.RenderMask( m_pViewport );
	g_Animator.RenderText( m_pViewport );
	RenderScreenInfo();

	m_pGfxExMan->ResetECMForMotionBlur();	

	m_pEngine->GetSystemFont()->Flush();

}

#endif