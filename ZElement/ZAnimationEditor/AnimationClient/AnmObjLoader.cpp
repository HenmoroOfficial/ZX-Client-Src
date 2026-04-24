#include "AnmObjLoader.h"
#include "AnmSystem.h"
#include "AnmObject.h"
#include "AnmNPC.h"
#include "AnmPlayer.h"
#include "AnmGFX.h"
#include "EC_GFXCaster.h"
#include "AnmUtility.h"
#include <Shlwapi.h>
#include <Process.h>

CAnmObjLoader::CAnmObjLoader()
{
	m_hThread	= 0;
	m_hEvent	= 0;
	m_dwThread	= 0;
	m_pBeginCB	= 0;
	m_pEndCB	= 0;

	InitializeCriticalSection( & m_csThread );
	m_hEvent = CreateSemaphore( 0, 0, 0x7FFF, 0 );
}

CAnmObjLoader::~CAnmObjLoader()
{
	Release();
	DeleteCriticalSection( & m_csThread );
	CloseHandle( m_hEvent );
}

bool CAnmObjLoader::Init()
{
	m_qObject.clear();
	if( ! m_hThread )
	{
		m_hThread = (HANDLE)_beginthreadex( 0, 0, ObjLoadLoop, this, 0, (unsigned int*)&m_dwThread );
		SetThreadPriority( m_hThread, THREAD_PRIORITY_NORMAL );
	}
	return true;
}

void CAnmObjLoader::Release()
{
	if( ! m_hThread )
		return;
	m_bRun = false;
	AddEvent();
	WaitForSingleObject( m_hThread, INFINITE );
	CloseHandle( m_hThread );
	m_hThread = 0;
	m_qObject.clear();
}

bool CAnmObjLoader::LoadObject( CAnmObject* pObj )
{
	if( ! pObj )
		return false;

#ifdef OBJECT_THREAD_LOAD
	pObj->SetLoadState( CAnmObject::LOADSTATE_NEEDLOAD );
	Lock();
	m_qObject.push_back( pObj );
	Unlock();
	AddEvent();
	return true;
#else
	if( ! Load( pObj ) )
		return false;
	pObj->OnLoaded();
	return true;
#endif
}

void CAnmObjLoader::UnloadObject( CAnmObject* pObj )
{
	if( ! pObj )
		return;
#if OBJECT_THREAD_LOAD
	pObj->SetLoadState( CAnmObject::LOADSTATE_NEEDRELEASE );
	Lock();
	m_qObject.push_back(pObj);
	Unlock();
	AddEvent();
#else
	Unload( pObj );
#endif
}

bool CAnmObjLoader::m_bRun = false;

unsigned int __stdcall CAnmObjLoader::ObjLoadLoop( void* pArg )
{
	CAnmObjLoader* pLoader = ( CAnmObjLoader * )pArg;
	while( m_bRun )
	{
		pLoader->WaitEvent();
		if( pLoader->GetObjQueueSize() )
		{
			CAnmObject* pObj = pLoader->SafePopObject();
			if( pObj->GetLoadState() == CAnmObject::LOADSTATE_NEEDLOAD )
			{
				BeginLoadFunc pBeginCB = pLoader->GetBeginLoadFunc();
				if( pBeginCB )
					pBeginCB( pObj );

				pObj->SetLoadState( CAnmObject::LOADSTATE_LOADWAIT );
				if( pLoader->Load( pObj ) )
					pObj->OnLoaded();

				CAnmObjLoader::EndLoadFunc pEndCB = pLoader->GetEndLoadFunc();
				if( pEndCB )
					pEndCB( pObj );
			}
			else if( pObj->GetLoadState() == CAnmObject::LOADSTATE_NEEDRELEASE )
				pLoader->Unload( pObj );
		}
	}
	return 0;
}

CAnmObject* CAnmObjLoader::SafePopObject()
{
	Lock();
	CAnmObject* pObj = m_qObject.front();
	m_qObject.pop_front();
	Unlock();
	return pObj;
}

bool CAnmObjLoader::Load( CAnmObject* pObj )
{
	switch( pObj->GetObjectType() )
	{
	case CAnmObject::TYPE_NPC:
		return LoadNPC( ( CAnmNPC* )pObj );
	case CAnmObject::TYPE_PLAYER:
		return LoadPlayer( ( CAnmPlayer* )pObj );
	case CAnmObject::TYPE_GFX:
		return LoadGFX( ( CAnmGFX* )pObj );
	default:
		return false;
	}
}

void CAnmObjLoader::Unload( CAnmObject* pObj )
{
	switch( pObj->GetObjectType() )
	{
	case CAnmObject::TYPE_NPC:
		UnloadNPC( ( CAnmNPC* )pObj );
		break;
	case CAnmObject::TYPE_PLAYER:
		UnloadPlayer( ( CAnmPlayer* )pObj );
		break;
	case CAnmObject::TYPE_GFX:
		UnloadGFX( ( CAnmGFX* )pObj );
		break;
	}
}

static void _ChangeExtension( char* szFile, char* szNewExt )
{
#ifdef _DEBUG
	int iLen = strlen( szFile );
	if( iLen <= 4 )
	{
		ASSERT( iLen > 4 );
		return;
	}
#endif
	char* pTemp = strrchr( szFile, '.' );
	if( ! pTemp )
	{
		strcat(szFile, ".");
		strcat(szFile, szNewExt);
	}
	else
		strcpy(pTemp+1, szNewExt);
}

bool CAnmObjLoader::LoadNPC( CAnmNPC* pNPC )
{
	UnloadNPC( pNPC );
	
	pNPC->m_pModel = new CECModel();
	char szModel[ MAX_PATH ];
	strncpy( szModel, pNPC->GetLoadFile(), MAX_PATH );

	AfxGetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	bool bRet = pNPC->m_pModel->Load( szModel, true, A3DSkinModel::LSF_NOSKIN, true );
	if( ! bRet )
	{
		a_LogOutput( 1, "CECNPCLoader::LoadNPCModel, Failed to load NPC model %s", szModel );
		strcpy( szModel, "models\\error\\error.ecm" );
		bRet = pNPC->m_pModel->Load( szModel );
		if( ! bRet )
		{
			A3DRELEASE( pNPC->m_pModel );
			AfxGetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
			return false;
		}
	}
	char szSkin[ MAX_PATH ];
	strncpy( szSkin, szModel, MAX_PATH );
	_ChangeExtension( szSkin, "ski" );

	A3DSkinMan* pSkinMan = AfxGetA3DEngine()->GetA3DSkinMan();
	A3DSkin* pSkin = pSkinMan->LoadSkinFile( szSkin );
	if( ! pSkin )
	{
		a_LogOutput( 1, "CECNPCLoader::LoadNPCModel, Failed to load skin %s", szSkin );
		pSkin = pSkinMan->LoadSkinFile( "Models\\error\\error.ski" );
		if( ! pSkin )
		{
			A3DRELEASE( pNPC->m_pModel );
			AfxGetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
			return false;
		}
	}
	A3DSkinModel* pSkinModel = pNPC->m_pModel->GetA3DSkinModel();
	if( ! pSkinModel )
	{
		AfxGetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return true;
	}
	pNPC->m_pModel->SetAABBType( CECModel::AABB_AUTOSEL );
	pNPC->m_pModel->AddSkin( pSkin, false );
	AfxGetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CAnmObjLoader::UnloadNPC( CAnmNPC* pNPC )
{
	if( ! pNPC->m_pModel )
		return;
	A3DSkinMan* pSkinMan = AfxGetA3DEngine()->GetA3DSkinMan();
	for( int i = 0; i < pNPC->m_pModel->GetSkinNum(); ++ i )
	{
		A3DSkin* pSkin = pNPC->m_pModel->GetA3DSkin(i);
		pSkinMan->ReleaseSkin( & pSkin );
	}
	A3DRELEASE( pNPC->m_pModel );
}

bool CAnmObjLoader::LoadPlayer( CAnmPlayer* pPlayer )
{
	LoadNPC( pPlayer );
	return true;
}

void CAnmObjLoader::UnloadPlayer( CAnmPlayer* pPlayer )
{
	UnloadNPC( pPlayer );
}

bool CAnmObjLoader::LoadGFX( CAnmGFX* pGFX )
{
	UnloadGFX( pGFX );

	A3DGFXExMan* pA3DGFXExMan = AfxGetGFXExMan();
	if( ! pA3DGFXExMan )
		return false;

	AString strFile = pGFX->GetLoadFile();
	strFile.MakeLower();
	if( strFile.Find("gfx\\") == 0 )
		strFile = strFile.Right( strFile.GetLength() - 4 );

	pGFX->m_pA3DGFX = pA3DGFXExMan->LoadGfx( AfxGetA3DDevice(), strFile );
	if( ! pGFX->m_pA3DGFX )
	{
		a_LogOutput( 1, "CAnmGFX::Load, Failed to load GFX %s", strFile );
		return false;
	}
	return true;
}

void CAnmObjLoader::UnloadGFX( CAnmGFX* pGFX )
{
	if( ! pGFX->m_pA3DGFX )
		return;

	A3DGFXExMan* pA3DGFXExMan = AfxGetGFXExMan();
	if( ! pA3DGFXExMan )
		return;

	pA3DGFXExMan->CacheReleasedGfx( pGFX->m_pA3DGFX );
	pGFX->m_pA3DGFX = 0;
}