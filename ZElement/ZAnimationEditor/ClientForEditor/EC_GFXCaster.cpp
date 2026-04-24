/*
 * FILE: EC_GFXCaster.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_GFXCaster.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "EC_Model.h"

#include "A3DGFXExMan.h"
#include "A3DGFXEx.h"
#include "A3DCamera.h"
#include "AFile.h"
#include "AFileImage.h"

#include "FWTemplate.h"
#include "FWAssemblySet.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define TIME_CHECKVIS		4000

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECGFXCaster
//	
///////////////////////////////////////////////////////////////////////////

CECGFXCaster::CECGFXCaster(A3DGFXExMan* pGFXMan) :
m_AutoGFXTab(256),
m_GFXTab(256),
m_FWTemplateTab(256),
m_FWTab()
{
	m_pGFXMan	= pGFXMan;
	//AfxInitFWEditor(g_pGame->GetA3DDevice());
	m_pCachedFWBody = NULL;
}

CECGFXCaster::~CECGFXCaster()
{
	ReleaseAllGFX();
	//ReleaseAllFW();
	//AfxReleaseFWEditor();

	if( m_pCachedFWBody )
	{
		m_pCachedFWBody->Release();
		delete m_pCachedFWBody;
		m_pCachedFWBody = NULL;
	}
}

//	Reset
bool CECGFXCaster::Reset()
{
	ReleaseAllGFX();
	//ReleaseAllFW();
	return true;
}

//	Release all gfx
void CECGFXCaster::ReleaseAllGFX()
{
	//	Release all auto GFXs
	AutoGFXTable::iterator it1 = m_AutoGFXTab.begin();
	for (; it1 != m_AutoGFXTab.end(); ++it1)
	{
		AUTOGFXSLOT* pSlot = *it1.value();
		for (int i=0; i < pSlot->aGFXs.GetSize(); i++)
		{
			A3DGFXEx* pGFX = pSlot->aGFXs[i]->pGFX;
			pGFX->Release();
			delete pGFX;
			delete pSlot->aGFXs[i];
		}

		delete pSlot;
	}
	
	m_AutoGFXTab.clear();

	//	Release all normal GFXs
	GFXTable::iterator it2 = m_GFXTab.begin();
	for (; it2 != m_GFXTab.end(); ++it2)
	{
		NORMALGFX* pNode = *it2.value();
		pNode->pGFX->Release();
		delete pNode->pGFX;
		delete pNode;
	}

	m_GFXTab.clear();
}

/*	Play a GFX automatically

	szFile: GFX file name
	dwWaitTime: wait time (s) in stop state before released. 0 means
		wait forever
	fScale: 0.0f means using the gfx own factor.
*/
bool CECGFXCaster::PlayAutoGFXEx(const char* szFile, DWORD dwWaitTime, const A3DVECTOR3& vPos,
				 const A3DVECTOR3& vDir, const A3DVECTOR3& vUp, float fScale/* 0.0f */)
{
	ASSERT(szFile && szFile[0]);
	ASSERT(m_pGFXMan);

	//	Build GFX ID from file name
	DWORD dwID = a_MakeIDFromLowString(szFile);
	A3DGFXEx* pGFX = NULL;

	AUTOGFXSLOT* pSlot = GetAutoGFXSlot(dwID);
	if (!pSlot)
	{
		//	Create a new slot
		if (!(pSlot = new AUTOGFXSLOT))
			return false;

		pSlot->dwID	= dwID;

		//	Add slot to table
		m_AutoGFXTab.put((int)dwID, pSlot);
	}

	//	Search a stopped GFX
	for (int i=0; i < pSlot->aGFXs.GetSize(); i++)
	{
		AUTOGFX* pAutoGFX = pSlot->aGFXs[i];
		if (pAutoGFX->pGFX->GetState() == ST_STOP)
		{
			pAutoGFX->dwTimeCnt		= 0;
			pAutoGFX->dwWaitTime	= dwWaitTime * 1000;
			pAutoGFX->bVisible		= true;
			pAutoGFX->dwVisTimeCnt	= TIME_CHECKVIS;

			pGFX = pAutoGFX->pGFX;
			break;
		}
	}

	if (!pGFX)
	{
		//	Create a new auto GFX
		AUTOGFX* pAutoGFX = new AUTOGFX;
		if (!pAutoGFX)
			return false;

		pAutoGFX->dwTimeCnt		= 0;
		pAutoGFX->dwWaitTime	= dwWaitTime * 1000;
		pAutoGFX->bVisible		= true;
		pAutoGFX->dwVisTimeCnt	= TIME_CHECKVIS;

		if (!(pAutoGFX->pGFX = m_pGFXMan->LoadGfx(g_pGame->GetA3DDevice(), szFile, false)))
		{
			a_LogOutput(1, "CECGFXCaster::PlayAutoGFXEx, Failed to load GFX %s", szFile);
			delete pAutoGFX;
			return false;
		}

		pSlot->aGFXs.Add(pAutoGFX);
		pGFX = pAutoGFX->pGFX;
	}

	if (fScale > 0.0f)
		pGFX->SetScale(fScale);

	A3DMATRIX4 mat = a3d_TransformMatrix(vDir, vUp, vPos);
	pGFX->SetParentTM(mat);
	pGFX->Start(true);

	return true;
}

//	Load normal gfx
A3DGFXEx* CECGFXCaster::LoadGFXEx(const char* szFile)
{
	ASSERT(m_pGFXMan);

	A3DGFXEx* pGFX = m_pGFXMan->LoadGfx(g_pGame->GetA3DDevice(), szFile);
	if (!pGFX)
	{
		a_LogOutput(1, "CECGFXCaster::LoadGFXEx, Failed to load GFX %s", szFile);
		return NULL;
	}

	NORMALGFX* pNode = new NORMALGFX;
	pNode->pGFX	= pGFX;
	pNode->dwVisTimeCnt = TIME_CHECKVIS;
	pNode->bVisible = true;

	//	Add to table
	m_GFXTab.put((int)pGFX, pNode);

	return pGFX;
}

//	Release normal gfx
void CECGFXCaster::ReleaseGFXEx(A3DGFXEx* pGFX, bool bCacheRelease/* true */)
{
	if (!pGFX)
		return;

	//	GFX exists ?
	GFXTable::pair_type Pair = m_GFXTab.get((int)pGFX);
	if (!Pair.second)
		return;	//	Counldn't find this GFX

	NORMALGFX* pNode = *Pair.first;
	m_GFXTab.erase((int)pGFX);

	if (bCacheRelease)
	{
		ASSERT(m_pGFXMan);
		m_pGFXMan->CacheReleasedGfx(pGFX);
	}
	else
	{
		pNode->pGFX->Release();
		delete pNode->pGFX;
	}

	delete pNode;
}

//	Tick routine
bool CECGFXCaster::Tick(DWORD dwDeltaTime)
{
	//	Tick all auto GFX
	AutoGFXTable::iterator it1 = m_AutoGFXTab.begin();
	for (; it1 != m_AutoGFXTab.end(); )
	{
		AUTOGFXSLOT* pSlot = *it1.value();

		for (int i=0; i < pSlot->aGFXs.GetSize(); i++)
		{
			AUTOGFX* pAutoGFX = pSlot->aGFXs[i];
			A3DGFXEx* pGfxEx = pAutoGFX->pGFX;

			//	Check whether GFX is out of date
			if (pAutoGFX->dwWaitTime && pGfxEx->GetState() == ST_STOP)
			{
				pAutoGFX->dwTimeCnt += dwDeltaTime;
				if (pAutoGFX->dwTimeCnt >= pAutoGFX->dwWaitTime)
				{
					//	Release this auto GFX item
					pGfxEx->Release();
					delete pGfxEx;
					delete pAutoGFX;
					pSlot->aGFXs.RemoveAtQuickly(i);
					i--;
				}
			}
			else
				pGfxEx->TickAnimation(dwDeltaTime);
		}

		//	Release empty slot
		if (!pSlot->aGFXs.GetSize())
		{
			it1 = m_AutoGFXTab.erase(it1);
			delete pSlot;
		}
		else
			++it1;
	}

	//	Tick all normal GFXs
	GFXTable::iterator it2 = m_GFXTab.begin();
	for (; it2 != m_GFXTab.end(); ++it2)
	{
		NORMALGFX* pNode = *it2.value();
		if (pNode->pGFX->GetState() != ST_STOP)
			pNode->pGFX->TickAnimation(dwDeltaTime);
	}

	//	Tick all fire works
	/*unsigned int i;
	for(i=0; i<m_FWTab.size();) 
	{
		FIREWORKNODE * pFWNode = m_FWTab[i];
		if( pFWNode )
		{
			// see if it should stop here
			pFWNode->pFW->TickAnimation(dwDeltaTime);
			if( pFWNode->pFW->IsAllAtState(ST_STOP) )
			{
				if( pFWNode->pFW )
				{
					pFWNode->pFW->Stop();
					delete pFWNode->pFW;
				}

				delete pFWNode;
				m_FWTab.erase(m_FWTab.begin() + i);
				continue;
			}
		}

		i ++;
	}*/

	return true;
}

#ifdef BACK_VERSION
void CECGFXCaster::Render(CECViewport* pViewport)
#else
bool CECGFXCaster::Render(const ECRENDERPARAM* pRenderParam)
{
	return Render(pRenderParam->pViewport);
}
bool CECGFXCaster::Render(ECViewport* pViewport)
#endif
{
	ASSERT(m_pGFXMan);

	//	Get current camera
#ifdef BACK_VERSION
	A3DCameraBase* pCamera = pViewport->GetA3DCamera();
#else
	A3DCameraBase* pCamera = pViewport->GetA3DCameraBase();
#endif
	A3DVECTOR3 vCamPos = pCamera->GetPos();

	A3DVECTOR3 vGfxPos;
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	DWORD dwDeltaTime = g_pGame->GetTickTime();

	//	Render all auto GFXs
	AutoGFXTable::iterator it1 = m_AutoGFXTab.begin();
	for (; it1 != m_AutoGFXTab.end(); ++it1)
	{
		AUTOGFXSLOT* pSlot = *it1.value();
		int iNumGFX = pSlot->aGFXs.GetSize();
		
		for (int i=0; i < iNumGFX; i++)
		{
			AUTOGFX* pAutoGFX = pSlot->aGFXs[i];
			A3DGFXEx* pGfxEx = pAutoGFX->pGFX;

			pAutoGFX->dwVisTimeCnt += dwDeltaTime;
			if (!pAutoGFX->bVisible || pAutoGFX->dwVisTimeCnt >= TIME_CHECKVIS)
			{
				pAutoGFX->dwVisTimeCnt = 0;
				vGfxPos = pGfxEx->GetAABB().Center;
#ifdef BACK_VERSION
				if (a3d_Magnitude(vGfxPos - vCamPos) > pConfigs->GetSceneLoadRadius())
#else
				if (a3d_MagnitudeH(vGfxPos - vCamPos) > pConfigs->GetSceneLoadRadius())
#endif
					pAutoGFX->bVisible = false;
				else
					pAutoGFX->bVisible = true;
			}

			if (pAutoGFX->bVisible && pGfxEx->GetState() != ST_STOP)
				m_pGFXMan->RegisterGfx(pGfxEx);
		}
	}

	//	Render all normal GFXs
	GFXTable::iterator it2 = m_GFXTab.begin();
	for (; it2 != m_GFXTab.end(); ++it2)
	{
		NORMALGFX* pNode = *it2.value();
		A3DGFXEx* pGfxEx = pNode->pGFX;

		pNode->dwVisTimeCnt += dwDeltaTime;
		if (!pNode->bVisible || pNode->dwVisTimeCnt >= TIME_CHECKVIS)
		{
			pNode->dwVisTimeCnt = 0;
			vGfxPos = pGfxEx->GetAABB().Center;
#ifdef BACK_VERSION
			if (a3d_Magnitude(vGfxPos - vCamPos) > pConfigs->GetSceneLoadRadius())
#else
			if (a3d_MagnitudeH(vGfxPos - vCamPos) > pConfigs->GetSceneLoadRadius())
#endif
				pNode->bVisible = false;
			else
				pNode->bVisible = true;
		}

		if (pNode->bVisible && pGfxEx->GetState() != ST_STOP)
			m_pGFXMan->RegisterGfx(pGfxEx);
	}

	//	Render all fire works
/*	int i;
	int nCount = m_FWTab.size();
	for(i=0; i<nCount; i++)
	{
		FIREWORKNODE * pFWNode = m_FWTab[i];
		if( pFWNode )
		{
			// has started.
			if( pFWNode->pFW )
				pFWNode->pFW->RegisterGfx(m_pGFXMan);
		}
	}
*/
#ifndef BACK_VERSION
	return true;
#endif
}

//	Play a normal gfx
bool CECGFXCaster::PlayGFXEx(A3DGFXEx* pGFX, const A3DVECTOR3& vPos, const A3DVECTOR3& vDir, const A3DVECTOR3& vUp, float fScale)
{
	if (!pGFX)
		return false;

	A3DMATRIX4 mat = a3d_TransformMatrix(vDir, vUp, vPos);
	pGFX->SetParentTM(mat);
	pGFX->SetScale(fScale);
	pGFX->Start(true);
	return true;
}

bool CECGFXCaster::PlayGFXEx(A3DGFXEx* pGFX, const A3DVECTOR3& vPos, const A3DVECTOR3& vDir, float fScale)
{
	if (!pGFX)
		return false;

	A3DMATRIX4 mat = a3d_TransformMatrix(vDir, g_vAxisY, vPos);
	pGFX->SetParentTM(mat);
	pGFX->SetScale(fScale);
	pGFX->Start(true);
	return true;
}
/*
FWTemplate * CECGFXCaster::LoadFWTemplate(const char * szFile)
{
	FWTemplateTable::iterator it = m_FWTemplateTab.find(szFile);

	if( it != m_FWTemplateTab.end() )
	{
		return *it.value();
	}

	AFileImage fileTemplate;
	if( !fileTemplate.Open(szFile, AFILE_OPENEXIST | AFILE_BINARY | AFILE_TEMPMEMORY) )
		return NULL;

	FWTemplate * pTemplate = new FWTemplate;
	if( !pTemplate->LoadFrom(&fileTemplate) )
	{
		delete pTemplate;
		return NULL;
	}

	fileTemplate.Close();

	m_FWTemplateTab.put(szFile, pTemplate);
	return pTemplate;
}

bool CECGFXCaster::PlayFW(const char * szFile, const A3DVECTOR3& vecPos, const A3DVECTOR3& vecFaceToDir, const wchar_t* pszText)
{
	FIREWORKNODE * pFWNode = new FIREWORKNODE;
	// start right now, not need to load the body model.
	FWTemplate * pTemplate = LoadFWTemplate(szFile);
	if( !pTemplate )
	{
		delete pFWNode;
		return false;
	}
	
	if( pszText )
	{
		pTemplate->SetText(pszText);
	}

	A3DVECTOR3 vecDir = vecFaceToDir;
	vecDir.y = 0.0f;
	vecDir.Normalize();
	A3DVECTOR3 vecUp = A3DVECTOR3(0.0f, 1.0f, 0.0f);
	A3DMATRIX4 matLaunch = RotateX(DEG2RAD(50.0f)) * TransformMatrix(vecDir, vecUp, vecPos);
	A3DVECTOR3 vecMainPos = A3DVECTOR3(0.0f, pTemplate->GetBlastHeight(), 0.0f) * matLaunch;
	vecMainPos.y -= pTemplate->GetBlastHeight();
	A3DMATRIX4 matMain = TransformMatrix(vecDir, vecUp, vecMainPos);
	pTemplate->SetParentTM(matLaunch, matMain);

	FWAssemblySet * pFW = new FWAssemblySet;
	if( !pFW->CreateFromTemplate(pTemplate) )
	{
		delete pFW;
		delete pFWNode;
		return false;
	}

	pFWNode->pFW = pFW;
	pFWNode->pFW->Start(true);

	pFWNode->vecPos = vecPos;
	pFWNode->szFile[0] = '\0';
	
	m_FWTab.push_back(pFWNode);
	return true;
}

//	Release all fire works
void CECGFXCaster::ReleaseAllFW()
{
	int i;
	int nCount = m_FWTab.size();
	for(i=0; i<nCount; i++)
	{
		FIREWORKNODE * pFWNode = m_FWTab[i];
		if( pFWNode )
		{
			if( pFWNode->pFW )
			{
				pFWNode->pFW->Stop();
				delete pFWNode->pFW;
			}
			delete pFWNode;
		}
	}
	m_FWTab.clear();

	FWTemplateTable::iterator it = m_FWTemplateTab.begin();
	for(; it!=m_FWTemplateTab.end(); ++it)
	{
		FWTemplate* pTemplate = *it.value();
		delete pTemplate;
	}
	m_FWTemplateTab.clear();
}
*/