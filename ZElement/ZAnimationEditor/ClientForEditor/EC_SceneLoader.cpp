#include "EC_Global.h"
#include "EC_SceneLoader.h"
#include "..\\ZElementClient\\EC_SceneThreadObj.h"

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManNPC.h"
#include "EC_ManOrnament.h"
#include "EC_ManMatter.h"
#include "EC_ManPlayer.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_Player.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"

#include "A3DFuncs.h"
#include "A3DTerrain2.h"
#include "A3DTerrain2LoaderB.h"
#include "AList2.h"
#include "AString.h"
#include "AArray.h"
#include "A3DFont.h"

volatile bool g_bIsLoadersInited = false;
extern HANDLE g_hToSuspendMain;
extern HANDLE g_hMainThread;
extern HANDLE g_hWaittoHandleACP;
static const float _max_speed = 20.0f * 2.0f; // terrain or ornament must be loaded in middle distance

//	Multi-thread loaded terrain block info
struct MTL_TRNBLOCK
{
	int	r;
	int	c;
	int	iBlock;
};

//	Multi-thread loaded EC model info
struct MTL_ECMODEL
{
	int		iType;		//	MTL_ECM_xxx
	int		idObject;	//	Object ID
	DWORD	dwBornStamp;//	Born stamp
	float	fDist;
	AString	strFile;	//	EC model file
	AString strFile2;	//	EC model file 2
	int		p1;			//	Parameters
	int		p2;
	int		p3;
	int		p4[SIZE_EQUIPIVTR];
	int		p5;			// level
	int		p6;			// Ear
	int		p7;			// Tail
	int		p8;			// Profession
	int		p9;			// default skin
};

static HANDLE g_hToExit			= NULL;
static HANDLE g_hToSuspend		= NULL;

static DWORD WINAPI LoaderThreadFunc(LPVOID lpParam);

class LoaderThreadModul
{
protected:
	LoaderThreadModul(int nPriority) :
	m_nPriority(nPriority),
	m_hHandle(0),
	m_dwID(0),
	m_nSuspendCount(1)
	{}

	virtual ~LoaderThreadModul() {}

protected:
	int		m_nPriority;
	HANDLE	m_hHandle;
	DWORD	m_dwID;

	volatile int m_nSuspendCount;

public:
	void Create();
	void Release();
	void Start();
	bool IsSuspended() const { return m_nSuspendCount != 0; }
	void Suspend()
	{
		if (!m_hHandle)
			return;

		m_nSuspendCount++;
		::SuspendThread(m_hHandle);
	}

	virtual void LoaderFunc(bool bInLoaderThread) = 0;
	virtual void RemoveAllData() {}
};

inline void LoaderThreadModul::Create()
{
	if (m_hHandle)
		return;

	m_hHandle = ::CreateThread(NULL, 0, LoaderThreadFunc, this, CREATE_SUSPENDED, &m_dwID);
	::SetThreadPriority(m_hHandle, m_nPriority);
	::SetThreadPriorityBoost(m_hHandle, FALSE);
}

inline void LoaderThreadModul::Release()
{
	if (m_hHandle == 0)
	{
		RemoveAllData();
		return;
	}

	::WaitForSingleObject(m_hHandle, INFINITE);
	m_hHandle = 0;
	m_dwID = 0;
	m_nSuspendCount = 1;
}

inline void LoaderThreadModul::Start()
{
	if (!m_hHandle)
		return;

	m_nSuspendCount--;
	int nCount = 0;
	while( ::ResumeThread(m_hHandle) == 0 && m_nSuspendCount == 0 && nCount < 2 )
	{
		Sleep(30);
		nCount++;
	}
}

class TerrLoaderModule : public LoaderThreadModul
{
public:
	TerrLoaderModule() :
	LoaderThreadModul(THREAD_PRIORITY_NORMAL),
	m_aBlkLoads(256, 64),
	m_aToLoad(256),
	m_aToRelease(256),
	m_ScnObjLoadList(512),
	m_ScnObjReleaseList(512)
	{
		::InitializeCriticalSection(&m_csTrnBlock);
		::InitializeCriticalSection(&m_csScnObj);
	}

	~TerrLoaderModule()
	{
		::DeleteCriticalSection(&m_csTrnBlock);
		::DeleteCriticalSection(&m_csScnObj);
	}

protected:
	AArray<MTL_TRNBLOCK, MTL_TRNBLOCK&> m_aBlkLoads;
	APtrList<CECSceneThreadObj*> m_aToLoad;
	APtrList<CECSceneThreadObj*> m_aToRelease;
	APtrList<CECSceneThreadObj*> m_ScnObjLoadList;
	APtrList<CECSceneThreadObj*> m_ScnObjReleaseList;

	CRITICAL_SECTION m_csTrnBlock;
	CRITICAL_SECTION m_csScnObj;

public:
	void LoadTerrainData(bool bInLoaderThread);
	void LoadOrnament(bool bInLoaderThread);
	void LoaderFunc(bool bInLoaderThread);
	void RemoveAllData()
	{
		::EnterCriticalSection(&m_csTrnBlock);
		m_aBlkLoads.RemoveAll();
		::LeaveCriticalSection(&m_csTrnBlock);

		::EnterCriticalSection(&m_csScnObj);
		m_ScnObjLoadList.RemoveAll();

		ALISTPOSITION pos = m_ScnObjReleaseList.GetHeadPosition();

		while (pos)
		{
			CECSceneThreadObj* pObj = m_ScnObjReleaseList.GetNext(pos);
			if (pObj->IsLoaded()) pObj->ReleaseInThread();
			delete pObj;
		}

		m_ScnObjReleaseList.RemoveAll();

		m_aToLoad.RemoveAll();
		m_aToRelease.RemoveAll();

		::LeaveCriticalSection(&m_csScnObj);
	}
	void QueueTerrainBlock(MTL_TRNBLOCK blk)
	{
		::EnterCriticalSection(&m_csTrnBlock);
		m_aBlkLoads.Add(blk);
		::LeaveCriticalSection(&m_csTrnBlock);
	}
	void QueueLoadScnObj(CECSceneThreadObj* pObj)
	{
		::EnterCriticalSection(&m_csScnObj);
		m_ScnObjLoadList.AddTail(pObj);
		::LeaveCriticalSection(&m_csScnObj);
	}
	void QueueReleaseScnObj(CECSceneThreadObj* pObj)
	{
		::EnterCriticalSection(&m_csScnObj);
		pObj->SetToRelease();
		m_ScnObjReleaseList.AddTail(pObj);
		::LeaveCriticalSection(&m_csScnObj);
	}
};

inline void TerrLoaderModule::LoadTerrainData(bool bInLoaderThread)
{
	//	Load terrain data
	A3DTerrain2* pTerrain = g_pGame->GetGameRun()->GetWorld()->GetTerrain();
	if (!pTerrain->IsMultiThreadLoad() || !pTerrain->GetDataLoadFlag()) return;

	DWORD dwMaxTick = DWORD(pTerrain->GetActRadius() * (1000.f / _max_speed));
	bool bSuspendMain = false;

	while (true)
	{
		A3DTerrain2Loader* pLoader = pTerrain->GetTerrainLoader();
		if (pLoader->GetLoaderID() == A3DTerrain2Loader::LOADER_B)
			((A3DTerrain2LoaderB*)pLoader)->DeactivateIdleSubTerrains();

		//	Release unloaded blocks ---------------------------
		A3DTerrain2Block* pBlock = pTerrain->GetNextBlockToBeUnloaded();
		while (pBlock)
		{
			pTerrain->ThreadUnloadBlock(pBlock);
			pBlock = pTerrain->GetNextBlockToBeUnloaded();
		}

		//	Load candidate blocks ------------------------------
		::EnterCriticalSection(&m_csTrnBlock);

		if (m_aBlkLoads.GetSize() == 0)
		{
			::LeaveCriticalSection(&m_csTrnBlock);

			if (bInLoaderThread)
				::Sleep(100);

			break;
		}

		AArray<MTL_TRNBLOCK, MTL_TRNBLOCK&> aBlkLoads(64, 64);

		int i;
		//	Transfer candidate terrain blocks
		for (i = 0; i < m_aBlkLoads.GetSize(); i++)
			aBlkLoads.Add(m_aBlkLoads[i]);

		m_aBlkLoads.RemoveAll(false);

		::LeaveCriticalSection(&m_csTrnBlock);

		/*
		char buf[100];
		sprintf(buf, "load count = %d\n", aBlkLoads.GetSize());
		OutputDebugStringA(buf);
		*/

		DWORD dwStart = ::GetTickCount();
		DWORD dwOnceStart = dwStart;

		//	Load candidate terrain blocks ...
		for (i=0; i < aBlkLoads.GetSize(); i++)
		{
			const MTL_TRNBLOCK& Blk = aBlkLoads[i];
			pTerrain->ThreadLoadBlock(Blk.r, Blk.c, Blk.iBlock);

			if (bInLoaderThread && !bSuspendMain)
			{
				DWORD dwCur = ::GetTickCount();
				DWORD dwSpan = dwCur - dwOnceStart;
				DWORD dwTotalSpan = dwCur - dwStart;

				if (dwTotalSpan >= dwMaxTick)
				{
  					bSuspendMain = true;
					::SetEvent(g_hToSuspendMain);
				}
				else if (dwSpan > 100)
				{
					::Sleep(100);
					dwOnceStart = ::GetTickCount();
				}
			}
		}
	}

	if (bSuspendMain && ::WaitForSingleObject(g_hToSuspendMain, 0) != WAIT_OBJECT_0)
	{
		::SetEvent(g_hWaittoHandleACP); //In renderthreadfunc
//		::ResumeThread(g_hMainThread);
	}

}

inline void TerrLoaderModule::LoadOrnament(bool bInLoaderThread)
{
	A3DTerrain2* pTerrain = g_pGame->GetGameRun()->GetWorld()->GetTerrain();
	DWORD dwMaxTick = DWORD(pTerrain->GetViewRadius() * (1000.f / _max_speed));
	bool bSuspendMain = false;
	static const float _min_square_dist = 2500.0f;

	while (true)
	{
		::EnterCriticalSection(&m_csScnObj);

		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		ALISTPOSITION pos = m_ScnObjLoadList.GetHeadPosition();

		if (pHost && bInLoaderThread)
		{
			A3DVECTOR3 vHost = pHost->GetPos();

			while (pos)
			{
				CECSceneThreadObj* pObj = m_ScnObjLoadList.GetNext(pos);
				A3DVECTOR3 vLoadPos = pObj->GetLoadPos();
				float xOff = vLoadPos.x - vHost.x;
				float zOff = vLoadPos.z - vHost.z;
				float fDist = xOff * xOff + zOff * zOff;
				pObj->SetDistToHost(fDist);

				if (bInLoaderThread && !bSuspendMain && fDist < _min_square_dist)
				{
					bSuspendMain = true;
					::SetEvent(g_hToSuspendMain);
				}

				ALISTPOSITION posLoad = m_aToLoad.GetHeadPosition();
				while (posLoad)
				{
					ALISTPOSITION posCur = posLoad;
					CECSceneThreadObj* p = m_aToLoad.GetNext(posLoad);

					if (fDist < p->GetDistToHost())
					{
						posLoad = posCur;
						m_aToLoad.InsertBefore(posLoad, pObj);
						break;
					}
				}

				if (posLoad == NULL)
					m_aToLoad.AddTail(pObj);
			}
		}
		else while (pos)
			m_aToLoad.AddTail(m_ScnObjLoadList.GetNext(pos));

		m_ScnObjLoadList.RemoveAll();

		pos = m_ScnObjReleaseList.GetHeadPosition();
		while (pos) m_aToRelease.AddTail(m_ScnObjReleaseList.GetNext(pos));
		m_ScnObjReleaseList.RemoveAll();

		::LeaveCriticalSection(&m_csScnObj);

		if (m_aToLoad.GetCount() == 0 && m_aToRelease.GetCount() == 0)
		{
			if( bInLoaderThread )
				::Sleep(100);
			break;
		}

		// 先加载再释放

		if (m_aToLoad.GetCount())
		{
			/*
			char buf[100];
			sprintf(buf, "orn count = %d\n", m_aToLoad.GetCount());
			OutputDebugStringA(buf);
			*/

			DWORD dwStart = ::GetTickCount();
			DWORD dwOnceStart = dwStart;
			ALISTPOSITION posLoad = m_aToLoad.GetHeadPosition();

			while (posLoad)
			{
				CECSceneThreadObj* pToLoad = m_aToLoad.GetNext(posLoad);

				if (pToLoad->IsToRelease())
					continue;
				else
				{
					pToLoad->LoadInThread(bInLoaderThread);
					pToLoad->SetLoaded();

					if (bInLoaderThread && !bSuspendMain)
					{
						DWORD dwCur = ::GetTickCount();
						DWORD dwSpan = dwCur - dwOnceStart;
						DWORD dwTotalSpan = dwCur - dwStart;

						if (dwTotalSpan >= dwMaxTick)
						{
  							bSuspendMain = true;
							::SetEvent(g_hToSuspendMain);
						}
						else if (dwSpan > 100)
						{
							::Sleep(100);
							dwOnceStart = ::GetTickCount();
						}
					}
				}
			}

			/*
			sprintf(buf, "orn time = %d\n", ::GetTickCount() - dwStart);
			OutputDebugStringA(buf);
			*/

			m_aToLoad.RemoveAll();
		}

		if (m_aToRelease.GetCount())
		{
			ALISTPOSITION posRelease = m_aToRelease.GetHeadPosition();

			while (posRelease)
			{
				CECSceneThreadObj* pToRelease = m_aToRelease.GetNext(posRelease);
				if (pToRelease->IsLoaded()) pToRelease->ReleaseInThread();
				delete pToRelease;
			}

			m_aToRelease.RemoveAll();
		}
	}

	if (bSuspendMain && ::WaitForSingleObject(g_hToSuspendMain, 0) != WAIT_OBJECT_0)
	{
		::SetEvent(g_hWaittoHandleACP); //In renderthreadfunc
//		::ResumeThread(g_hMainThread);
	}

}

void TerrLoaderModule::LoaderFunc(bool bInLoaderThread)
{
	LoadTerrainData(bInLoaderThread);
	LoadOrnament(bInLoaderThread);
}

class ECModelLoaderModule : public LoaderThreadModul
{
protected:
	ECModelLoaderModule(int nPriority) :
	LoaderThreadModul(nPriority),
	m_aModelLoads(512),
	m_aDelModels(512, 64),
	m_aTemplDel(64, 64),
	m_aTemplLoad(256, 256)
	{
		::InitializeCriticalSection(&m_csDelECModel);
		::InitializeCriticalSection(&m_csECModel);
	}

	virtual ~ECModelLoaderModule()
	{
		::DeleteCriticalSection(&m_csDelECModel);
		::DeleteCriticalSection(&m_csECModel);
	}

protected:
	APtrList<MTL_ECMODEL*>	m_aModelLoads;
	APtrArray<CECModel*>	m_aDelModels;
	APtrArray<CECModel*>	m_aTemplDel;
	APtrArray<MTL_ECMODEL*> m_aTemplLoad;

	CRITICAL_SECTION		m_csDelECModel;
	CRITICAL_SECTION		m_csECModel;

public:
	void LoadAndReleaseECModels(bool bInLoaderThread);
	void QueueECModelForLoad(int iType, int idObject, DWORD dwBornStamp, const A3DVECTOR3& vPos, const char* szFile, const char* szFile2, 
		int p1, int p2, int p3, int* p4, int p5, int p6, int p7, int p8, int p9);
	void QueuePlayerUndoLoad(int cid, DWORD dwBornStamp);
	void QueueECModelForRelease(CECModel* pModel);
	void LoaderFunc(bool bInLoaderThread);
	void RemoveOldData()
	{
		::EnterCriticalSection(&m_csDelECModel);

		for (int i = 0; i < m_aDelModels.GetSize(); i++)
		{
			CECModel* pModel = m_aDelModels[i];
			pModel->Release();
			delete pModel;
		}

		m_aDelModels.RemoveAll(false);
		::LeaveCriticalSection(&m_csDelECModel);
	}
	void RemoveAllData()
	{
		RemoveOldData();

		::EnterCriticalSection(&m_csECModel);
		ALISTPOSITION pos = m_aModelLoads.GetHeadPosition();
		while (pos) delete m_aModelLoads.GetNext(pos);
		m_aModelLoads.RemoveAll();
		::LeaveCriticalSection(&m_csECModel);
	}
};

inline void ECModelLoaderModule::QueueECModelForLoad(
	int iType,
	int idObject,
	DWORD dwBornStamp,
	const A3DVECTOR3& vPos,
	const char* szFile,
	const char* szFile2,
	int p1,
	int p2,
	int p3,
	int* p4,
	int p5, 
	int p6, 
	int p7,
	int p8,
	int p9)
{
	::EnterCriticalSection(&m_csECModel);

	MTL_ECMODEL* Info = new MTL_ECMODEL;
	Info->iType			= iType;
	Info->idObject		= idObject;
	Info->dwBornStamp	= dwBornStamp;
	Info->fDist			= SquareMagnitude(vPos - g_pGame->GetGameRun()->GetHostPlayer()->GetPos());

	if (szFile)
		Info->strFile = szFile;

	if (szFile2)
		Info->strFile2 = szFile2;

	Info->p1			= p1;
	Info->p2			= p2;
	Info->p3			= p3;

	if (p4)
	{
		for (int i = 0; i < SIZE_EQUIPIVTR; i++)
			Info->p4[i] = p4[i];
	}
	else
		memset(Info->p4, 0xff, sizeof(Info->p4));
	
	Info->p5			= p5;
	Info->p6			= p6;
	Info->p7			= p7;
	Info->p8			= p8;
	Info->p9			= p9;

	m_aModelLoads.AddTail(Info);
	::LeaveCriticalSection(&m_csECModel);
}

inline void ECModelLoaderModule::QueuePlayerUndoLoad(int cid, DWORD dwBornStamp)
{
	::EnterCriticalSection(&m_csECModel);

	// 判断加载队列是否已有此对象

	ALISTPOSITION pos = m_aModelLoads.GetHeadPosition(), posCur;

	while (pos)
	{
		posCur = pos;
		const MTL_ECMODEL* pInfo = m_aModelLoads.GetNext(pos);
		if (pInfo->idObject == cid && pInfo->dwBornStamp == dwBornStamp)
		{
			delete pInfo;
			m_aModelLoads.RemoveAt(posCur);
		}
	}

	::LeaveCriticalSection(&m_csECModel);
}

inline void ECModelLoaderModule::QueueECModelForRelease(CECModel* pModel)
{
	::EnterCriticalSection(&m_csDelECModel);
	m_aDelModels.Add(pModel);
	::LeaveCriticalSection(&m_csDelECModel);
}

static const int _load_per_time = 4;

//#include "EC_GameUIMan.h"
//#include "EC_UIManager.h"

inline void ECModelLoaderModule::LoadAndReleaseECModels(bool bInLoaderThread)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	CECNPCMan* pNPCMan = pWorld->GetNPCMan();
	CECOrnamentMan* pOnmtMan = pWorld->GetOrnamentMan();
	CECMatterMan* pMatterMan = pWorld->GetMatterMan();
	CECPlayerMan* pPlayerMan = pWorld->GetPlayerMan();

	//	Delete at first -------------------------
	::EnterCriticalSection(&m_csDelECModel);
	m_aTemplDel.RemoveAll(false);

	int i;
	for (i = 0; i < m_aDelModels.GetSize(); i++)
		m_aTemplDel.Add(m_aDelModels[i]);

	m_aDelModels.RemoveAll(false);
	::LeaveCriticalSection(&m_csDelECModel);

	for (i = 0; i < m_aTemplDel.GetSize(); i++)
	{
		CECModel* pModel = m_aTemplDel[i];
		pModel->Release();
		delete pModel;
	}

	//	Load -------------------------------------

	//	Transfer candidate NPC models
	::EnterCriticalSection(&m_csECModel);
	m_aTemplLoad.RemoveAll(false);

	if (m_aModelLoads.GetCount())
	{
		if (bInLoaderThread)
		{
			int nLoadCount = 0;
			ALISTPOSITION pos = m_aModelLoads.GetHeadPosition();
			ALISTPOSITION posCur;

			struct
			{
				MTL_ECMODEL* p;
				ALISTPOSITION pos;
			} _tmp[_load_per_time + 1];

			memset(_tmp, 0, sizeof(_tmp));

			while (pos)
			{
				posCur = pos;
				MTL_ECMODEL* p = m_aModelLoads.GetNext(pos);
				bool bReplace = false;

				for (int i = 0; i < nLoadCount; i++)
				{
					if (p->fDist < _tmp[i].p->fDist)
					{
						memmove(&_tmp[i+1], &_tmp[i], (nLoadCount - i) * sizeof(_tmp[0]));
						_tmp[i].p = p;
						_tmp[i].pos = posCur;
						if (nLoadCount < _load_per_time) nLoadCount++;
						bReplace = true;
						break;
					}
				}

				if (!bReplace && nLoadCount < _load_per_time)
				{
					_tmp[nLoadCount].p = p;
					_tmp[nLoadCount].pos = posCur;
					nLoadCount++;
				}
			}

			for (int i = 0; i < nLoadCount; i++)
			{
				m_aTemplLoad.Add(_tmp[i].p);
				m_aModelLoads.RemoveAt(_tmp[i].pos);
			}
		}
		else
		{
			ALISTPOSITION pos = m_aModelLoads.GetHeadPosition();
			while (pos) m_aTemplLoad.Add(m_aModelLoads.GetNext(pos));
			m_aModelLoads.RemoveAll();
		}
	}

	::LeaveCriticalSection(&m_csECModel);

	if (m_aTemplLoad.GetSize() > 0)
	{
		DWORD dwStart = ::GetTickCount();

		//	Load EC models
		for (i = 0; i < m_aTemplLoad.GetSize(); i++)
		{
			MTL_ECMODEL& Info = *m_aTemplLoad[i];

			switch (Info.iType)
			{
			case MTL_ECM_PLAYER:

				pPlayerMan->ThreadLoadPlayerModel(Info.idObject, Info.dwBornStamp, Info.p3 >> 16, Info.p8, Info.p5, Info.p6, Info.p7, Info.p1, Info.p2, 
					Info.p9, Info.p3 & 0xffff, Info.p4, Info.strFile, Info.strFile2);
				break;

			case MTL_ECM_PLAYER_EQUIP:

				pPlayerMan->ThreadLoadPlayerEquips(Info.idObject, Info.dwBornStamp, Info.p1, Info.p5, Info.p9, Info.p2, Info.p4, Info.p3);
				break;

			case MTL_ECM_PET:

				pPlayerMan->ThreadLoadPetModel(Info.idObject, Info.dwBornStamp, Info.strFile);
				break;
				
			case MTL_ECM_CHANGEDMODEL:

				pPlayerMan->ThreadLoadChangedModel(Info.idObject, Info.dwBornStamp, Info.strFile);
				break;
			case MTL_ECM_TRANSFIGUREDMODEL:

				pPlayerMan->ThreadLoadTransfiguredModel(Info.idObject, Info.dwBornStamp, Info.strFile);
				break;
			case MTL_ECM_AIRCRAFT:

				pPlayerMan->ThreadLoadAircraftModel(Info.idObject, Info.dwBornStamp, Info.strFile);
				break;

			case MTL_ECM_NPC:

				pNPCMan->ThreadLoadNPCModel(Info.idObject, Info.dwBornStamp, Info.p1, Info.strFile);
				break;

			case MTL_ECM_SCNMODEL:

				pOnmtMan->ThreadLoadECModel((DWORD)Info.idObject, Info.strFile);
				break;

			case MTL_ECM_MATTER:

				pMatterMan->ThreadLoadMatterModel(Info.idObject, Info.strFile);
				break;
			}

			delete &Info;

			if (bInLoaderThread && ::GetTickCount() - dwStart > 100)
			{
				Sleep(100);
				dwStart = ::GetTickCount();
			}
		}
	}
	else if (bInLoaderThread)
	{
		::Sleep(100);
	}
}

void ECModelLoaderModule::LoaderFunc(bool bInLoaderThread)
{
	LoadAndReleaseECModels(bInLoaderThread);
}

class NPCLoaderModule : public ECModelLoaderModule
{
public:
	NPCLoaderModule() : ECModelLoaderModule(THREAD_PRIORITY_NORMAL) {}
	~NPCLoaderModule() {}
};

class PlayerLoaderModule : public ECModelLoaderModule
{
public:
	PlayerLoaderModule() : ECModelLoaderModule(THREAD_PRIORITY_NORMAL) {}
	~PlayerLoaderModule() {}

public:
	void CalcPlayersRelDist();
};

inline void PlayerLoaderModule::CalcPlayersRelDist()
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	if (!pWorld) return;

	const A3DVECTOR3& vHost = g_pGame->GetGameRun()->GetHostPlayer()->GetPos();
	CECPlayerMan* pMan = pWorld->GetPlayerMan();

	::EnterCriticalSection(&m_csECModel);

	ALISTPOSITION pos = m_aModelLoads.GetHeadPosition();

	while (pos)
	{
		MTL_ECMODEL* p = m_aModelLoads.GetNext(pos);
		CECElsePlayer* pPlayer = pMan->GetElsePlayer(p->idObject);

		if (pPlayer)
			p->fDist = SquareMagnitude(pPlayer->GetPos() - vHost);
	}

	::LeaveCriticalSection(&m_csECModel);
}

// Loader Modules

TerrLoaderModule	g_TerrLoaderModule;
NPCLoaderModule		g_NPCLoaderModule;
PlayerLoaderModule	g_PlayerLoaderModule;

static DWORD WINAPI _LoaderThreadFuncImp(LPVOID lpParam)
{
	LoaderThreadModul* pLoader = static_cast<LoaderThreadModul*>(lpParam);

	while (true)
	{
		if (::WaitForSingleObject(g_hToExit, 0) == WAIT_OBJECT_0)
			break;

		pLoader->LoaderFunc(true);

		if (::WaitForSingleObject(g_hToSuspend, 0) == WAIT_OBJECT_0)
			pLoader->Suspend();
	}

	pLoader->RemoveAllData();
	return 0;
}

static DWORD WINAPI LoaderThreadFunc(LPVOID lpParam)
{
	DWORD dwRet = -1;

#ifndef _NOMINIDUMP

	__try
	{

#endif
	
	dwRet = _LoaderThreadFuncImp(lpParam);

#ifndef _NOMINIDUMP

	}	//	__try

	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred... mini dumped in loader thread!");
		::ExitProcess(-1);
	}

#endif

	return dwRet;
}

bool InitLoaderThread()
{
	if (g_bIsLoadersInited) return true;
	g_hToExit = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	g_hToSuspend = ::CreateEvent(NULL, TRUE, FALSE, NULL);
	g_TerrLoaderModule.Create();
	g_NPCLoaderModule.Create();
	g_PlayerLoaderModule.Create();
	g_bIsLoadersInited = true;
	return true;
}

void StartLoaderThread()
{
	if (!g_bIsLoadersInited) return;
	g_TerrLoaderModule.Start();
	g_NPCLoaderModule.Start();
	g_PlayerLoaderModule.Start();
}

void ExitLoaderThread()
{
	if (!g_bIsLoadersInited)
		return;

	g_TerrLoaderModule.Start();
	g_NPCLoaderModule.Start();
	g_PlayerLoaderModule.Start();

	::SetEvent(g_hToExit);

	g_TerrLoaderModule.Release();
	g_NPCLoaderModule.Release();
	g_PlayerLoaderModule.Release();

	::CloseHandle(g_hToExit);
	::CloseHandle(g_hToSuspend);

	g_hToExit = 0;
	g_hToSuspend = 0;
	g_bIsLoadersInited = false;
}

void QueueLoadInThread(CECSceneThreadObj* pObj)
{
	g_TerrLoaderModule.QueueLoadScnObj(pObj);
}

void QueueReleaseInThread(CECSceneThreadObj* pObj)
{
	g_TerrLoaderModule.QueueReleaseScnObj(pObj);
}

void QueueTerrainBlock(int r, int c, int iBlock)
{
	MTL_TRNBLOCK Blk = {r, c, iBlock};
	g_TerrLoaderModule.QueueTerrainBlock(Blk);
}

void QueueECModelForLoad(
	int iType,
	int idObject,
	DWORD dwBornStamp,
	const A3DVECTOR3& vPos,
	const char* szFile,
	const char* szFile2,
	int p1/* 0 */,
	int p2/* 0 */,
	int p3/* 0 */,
	int* p4,
	int p5, 
	int p6,
	int p7,
	int p8,
	int p9)
{
	switch (iType)
	{
	case MTL_ECM_PLAYER:
	case MTL_ECM_PLAYER_EQUIP:
		g_PlayerLoaderModule.QueueECModelForLoad(iType, idObject, dwBornStamp, vPos, szFile, szFile2, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		break;
	default:
		g_NPCLoaderModule.QueueECModelForLoad(iType, idObject, dwBornStamp, vPos, szFile, szFile2, p1, p2, p3, p4, p5, p6, p7, p8, p9);
		break;
	}
}

// called in main thread
void CalcPlayersRelDist()
{
	g_PlayerLoaderModule.CalcPlayersRelDist();
}

void QueuePlayerUndoLoad(int cid, DWORD dwBornStamp, bool bPlayer)
{
	if (bPlayer)
		g_PlayerLoaderModule.QueuePlayerUndoLoad(cid, dwBornStamp);
	else
		g_NPCLoaderModule.QueuePlayerUndoLoad(cid, dwBornStamp);
}

void QueueECModelForRelease(CECModel* pModel)
{
	g_PlayerLoaderModule.QueueECModelForRelease(pModel);
}

void SuspendLoadThread()
{
	if (!g_bIsLoadersInited) return;
	::SetEvent(g_hToSuspend);

	while (!g_TerrLoaderModule.IsSuspended()
	 || !g_NPCLoaderModule.IsSuspended()
	 || !g_PlayerLoaderModule.IsSuspended())
	{
		Sleep(100);
	}

	::ResetEvent(g_hToSuspend);
}

void LoadInMainThread(bool bForceLoad)
{
	if (bForceLoad)
		SuspendLoadThread();
	else if (!g_TerrLoaderModule.IsSuspended() || !g_NPCLoaderModule.IsSuspended() || !g_PlayerLoaderModule.IsSuspended())
	{
		static DWORD _tick = 0;
		if (_tick++ % 4 == 0)
			CalcPlayersRelDist();
		return;
	}

	//	Load terrain data
	A3DTerrain2* pTerrain = g_pGame->GetGameRun()->GetWorld()->GetTerrain();
	if (pTerrain && pTerrain->IsMultiThreadLoad() && pTerrain->GetDataLoadFlag())
	{
		//	Release unloaded blocks
		A3DTerrain2Block* pBlock = pTerrain->GetNextBlockToBeUnloaded();
		while (pBlock)
		{
			pTerrain->ThreadUnloadBlock(pBlock);
			pBlock = pTerrain->GetNextBlockToBeUnloaded();
		}

		//	Terrain laoding will occupy about 45% loading progress, calculate
		//	each block's component
		float fBlockComp = pTerrain->GetCandidateBlockNum() ? 45.0f / pTerrain->GetCandidateBlockNum() : 0.17f;
		float fProgCnt = 0.0f;

		//	Load candidate terrain blocks ...
		int r, c, iBlock;
		while (pTerrain->GetNextCandidateBlock(r, c, iBlock))
		{
			pTerrain->ThreadLoadBlock(r, c, iBlock);

			//	Step loading progress
			fProgCnt += fBlockComp;
			int iStep = (int)fProgCnt;
			if (iStep)
			{
				g_pGame->GetGameRun()->StepLoadProgress(iStep);
				fProgCnt -= (float)iStep;
			}
		}
		
		pTerrain->ActivateLoadedBlocks();
	}

	g_NPCLoaderModule.RemoveOldData();
	g_PlayerLoaderModule.RemoveOldData();

	g_TerrLoaderModule.LoadOrnament(false);
	g_NPCLoaderModule.LoaderFunc(false);
	g_PlayerLoaderModule.LoaderFunc(false);
}

bool IsLoadThreadReady()
{
	return g_bIsLoadersInited;
}

void ThreadRemoveAllLoaded()
{
	g_TerrLoaderModule.RemoveAllData();
	g_NPCLoaderModule.RemoveAllData();
	g_PlayerLoaderModule.RemoveAllData();
}
