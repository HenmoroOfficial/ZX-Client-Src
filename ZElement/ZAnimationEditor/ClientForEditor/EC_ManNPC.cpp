/*
 * FILE: EC_ManNPC.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_ManNPC.h"
//#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_NPC.h"
#include "EC_Monster.h"
//#include "EC_NPCServer.h"
//#include "EC_NPCCarrier.h"
#include "EC_CDS.h"
#include "EC_ManPlayer.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_Utility.h"
#include "EC_Configs.h"
#include "EC_Model.h"
#include "EC_SceneLoader.h"
//#include "EC_UIManager.h"
//#include "EC_InputCtrl.h"
//#include "ECScriptMan.h"
//#include "ECScriptContext.h"
//#include "ECSCriptCheckState.h"
//#include "EC_GameRecord.h"
//#include "EC_Cutscene.h"
#include "EC_Pet.h"

#include "A3DCollision.h"
#include "A3DMacros.h"
#include "A3DSkin.h"
#include "A3DFuncs.h"
#include "elementdataman.h"
#include "ACSWrapper.h"
//#include "EC_Helper.h"
//#include "EC_AutoHelper.h"
//#include "LuaEvent.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


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
//	Implement CECNPCMan
//	
///////////////////////////////////////////////////////////////////////////

CECNPCMan::CECNPCMan(CECGameRun* pGameRun) :
CECManager(pGameRun),
m_NPCTab(512),
m_UkNPCTab(32),
m_ServerNPCName2IDTab(256)
{
	m_iManagerID = MAN_NPC;

	m_QueryCnt.SetPeriod(10000);
	
	InitializeCriticalSection(&m_csLoad);
}

CECNPCMan::~CECNPCMan()
{
	DeleteCriticalSection(&m_csLoad);
}

//	Release manager
void CECNPCMan::Release()
{
	OnLeaveGameWorld();
}

//	Tick routine
bool CECNPCMan::Tick(DWORD dwDeltaTime)
{
	//	Deliver loaded NPC models
	DeliverLoadedNPCModels();

	enum
	{
		SIZE_REMOVETAB = 64
	};

	//	Every tick maximum 64 NPCs can be removed
	CECNPC* aRemove[SIZE_REMOVETAB];
	int i, iRemoveCnt = 0;

	//	Tick all NPCs
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();

		if (pNPC->ShouldDisappear())
		{
			if (iRemoveCnt < SIZE_REMOVETAB)
				aRemove[iRemoveCnt++] = pNPC;
		}
		else if (!pNPC->IsCarrierNPC())
			pNPC->Tick(dwDeltaTime);	// Carrier NPC is ticked separately.
	}

	for (i=0; i < iRemoveCnt; i++)
		NPCLeave(aRemove[i]->GetNPCID());

	//	Tick all NPCs who are in disappear table
	iRemoveCnt = 0;

	for (i=0; i < m_aDisappearNPCs.GetSize(); i++)
	{
		CECNPC* pNPC = m_aDisappearNPCs[i];
		if (pNPC->ShouldDisappear())
		{
			if (iRemoveCnt < SIZE_REMOVETAB)
			{
				aRemove[iRemoveCnt++] = pNPC;
				m_aDisappearNPCs.RemoveAtQuickly(i);
				i--;
			}
		}
		else
			pNPC->Tick(dwDeltaTime);
	}

	for (i=0; i < iRemoveCnt; i++)
		ReleaseNPC(aRemove[i]);

	//	Update NPCs in various ranges (Active, visible, mini-map etc.)
	UpdateNPCInRanges(dwDeltaTime);

	//	Udpate unknown NPC table
	UpdateUnknownNPCs();

	return true; 
}

//	Tick carriers
bool CECNPCMan::TickCarriers(DWORD dwDeltaTime)
{
// 	int i;
// 	for (i=0; i < m_aCarriers.GetSize(); i++)
// 	{
// 		CECNPCCarrier* pCarrier = m_aCarriers[i];
// 		pCarrier->Tick(dwDeltaTime);
// 	}

	return true;
}

//	Render routine
bool CECNPCMan::Render(CECViewport* pViewport)
{
	CECNPC* pHHNPC = NULL;	//	The NPC will be high-light rendered
	int idHoverObject = 0;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost)
	{
		if (pHost->IsChangingFace())
			return true;

		idHoverObject = pHost->GetCursorHoverObject();
	}
	
	//	Render all NPCs in active table
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
		if (idHoverObject == pNPC->GetNPCID())
			pHHNPC = pNPC;
		else
			pNPC->Render(pViewport);
	}

	//	Render high-lighted NPC
	if (pHHNPC)
		pHHNPC->RenderHighLight(pViewport);

	//	Render all NPCs in disappear table
	for (int i=0; i < m_aDisappearNPCs.GetSize(); i++)
	{
		CECNPC* pNPC = m_aDisappearNPCs[i];
		pNPC->Render(pViewport);
	}

	return true; 
}

//	RenderForReflected routine
bool CECNPCMan::RenderForReflect(CECViewport * pViewport)
{
	// all npc are reflect visible, but no gfx should be register again;
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
		pNPC->Render(pViewport, CECObject::RD_REFLECT);
	}

	return true;
}

//	RenderForRefract routine
bool CECNPCMan::RenderForRefract(CECViewport * pViewport)
{
	// all npc are refract visible
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
		pNPC->Render(pViewport, CECObject::RD_REFRACT);
	}

	return true;
}

//	On entering game world
bool CECNPCMan::OnEnterGameWorld()
{
	return true; 
}

//	On leaving game world
bool CECNPCMan::OnLeaveGameWorld()
{
	//	Release all NPCs in active table
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
		ReleaseNPC(pNPC);
	}

	m_NPCTab.clear();

	//	Release all NPCs in disappear table
	int i;
	for (i=0; i < m_aDisappearNPCs.GetSize(); i++)
		ReleaseNPC(m_aDisappearNPCs[i]);
	
	m_aDisappearNPCs.RemoveAll();

	//	Release all loaded models
	ACSWrapper csa(&m_csLoad);

	for (i=0; i < m_aLoadedModels.GetSize(); i++)
	{
		NPCMODEL* pInfo = m_aLoadedModels[i];
		CECNPC::ReleaseNPCModel(pInfo->Ret);
		delete pInfo;
	}

	m_aLoadedModels.RemoveAll();
	m_aMMNPCs.RemoveAll(false);
	m_aTabSels.RemoveAll(false);
	m_ServerNPCName2IDTab.clear();

	return true;
}

//	Process message
bool CECNPCMan::ProcessMessage(const ECMSG& Msg)
{
	return true; 
}

//	Transmit message
bool CECNPCMan::TransmitMessage(const ECMSG& Msg)
{
	return true;
}

//	Messages handlers
bool CECNPCMan::OnMsgNPCInfo(const ECMSG& Msg)
{
	return true;
}

//	Message MSG_NM_NPCMOVE handler
bool CECNPCMan::OnMsgNPCMove(const ECMSG& Msg)
{
	return true;
}

//	Message MSG_NM_NPCRUNOUT handler
bool CECNPCMan::OnMsgNPCRunOut(const ECMSG& Msg)
{
	return true;
}

bool CECNPCMan::OnMsgNPCDied(const ECMSG& Msg)
{
	return true;
}

bool CECNPCMan::OnMsgNPCDisappear(const ECMSG& Msg)
{
	return true;
}

bool CECNPCMan::OnMsgNPCOutOfView(const ECMSG& Msg)
{
	NPCLeave((int)Msg.dwParam1);
	return true;
}

bool CECNPCMan::OnMsgNPCStopMove(const ECMSG& Msg)
{
	return true;
}

bool CECNPCMan::OnMsgInvalidObject(const ECMSG& Msg)
{
	return true;
}

//	Create a NPC
CECNPC* CECNPCMan::CreateNPC(const S2C::info_npc& Info, bool bBornInSight)
{
	CECNPC* pNPC = NULL;

	int tid = Info.tid;
	bool bCarrier = false;

	//	Get data type from database
	DATA_TYPE DataType = DT_INVALID;
	const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_NPC_ESSENCE && DataType != DT_MONSTER_ESSENCE && DataType != DT_PET_BEDGE_ESSENCE)
	{
		//	Try to create a default npc
		tid = 4249;
		DataType = g_pGame->GetElementDataMan()->get_data_type(tid, ID_SPACE_ESSENCE);
	}
	else if (DataType == DT_NPC_ESSENCE)
	{
		const NPC_ESSENCE* pEssence = (const NPC_ESSENCE*)pData;
		if (pEssence->is_vehicle)
			bCarrier = true;
	}

	switch (DataType)
	{
	case DT_NPC_ESSENCE:
	{
// 		if (bCarrier)
// 			pNPC = new CECNPCCarrier(this);
// 		else
// 			pNPC = new CECNPCServer(this);	
		
		break;
	}
	case DT_MONSTER_ESSENCE:	pNPC = new CECMonster(this);	break;
	case DT_PET_BEDGE_ESSENCE:	pNPC = new CECPet(this);		break;
	default:
	{
		ASSERT(0);
		return NULL;
	}
	}

	//	Set born stamp
	pNPC->SetBornStamp(g_pGame->GetGameRun()->GetWorld()->GetBornStamp());
	pNPC->SetBornInSight(bBornInSight);

	if (!pNPC || !pNPC->Init(tid, Info))
	{
		delete pNPC;
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECNPCMan::CreateNPC", __LINE__);
		return false;
	}

	return pNPC;
}

//	Release a NPC
void CECNPCMan::ReleaseNPC(CECNPC* pNPC)
{
	if (!pNPC)
		return;

	//	Remove from carrier list
	if (pNPC->IsCarrierNPC())
	{
		int iIndex = m_aCarriers.Find((CECNPCCarrier*)pNPC);
		if (iIndex >= 0)
			m_aCarriers.RemoveAtQuickly(iIndex);
	}

	//	Remove tab-selected array
	RemoveNPCFromTabSels(pNPC);

	pNPC->Release();
	delete pNPC;
	pNPC = NULL;
}

//	Put NPC into disappear table
void CECNPCMan::NPCDisappear(int nid, bool bAtOnce)
{
	CECNPC* pNPC = GetNPC(nid);
	if (pNPC)
	{
		pNPC->Disappear(bAtOnce);
		//	From npc from active table and add it to disappear table
		if (bAtOnce)
		{
			NPCLeave(nid, true, true);
		}
		else
		{
			NPCLeave(nid, true, false);
			m_aDisappearNPCs.Add(pNPC);
		}
	}
}

//	One NPC enter view area
bool CECNPCMan::NPCEnter(const S2C::info_npc& Info, bool bBornInSight)
{
	// this object is defined as npc in server, but processed as player in client
	if ( ISCLONEPLAYERID(Info.nid))
	{
		g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->ProcessNPC2PlayerMsg(Info);
		return true;
	}

	//	If NPC with specified id exists, delete it
	CECNPC* pNPC = GetNPC(Info.nid);
	ReleaseNPC(pNPC);
	m_NPCTab.erase(Info.nid);

	//	If this NPC id is in unknown table, remove it because this NPC
	//	won't be unknown anymore
	UkNPCTable::pair_type Pair = m_UkNPCTab.get(Info.nid);
	if (Pair.second)
		m_UkNPCTab.erase(Info.nid);

	//	Create a new NPC
	if (!(pNPC = CreateNPC(Info, bBornInSight)))
	{
		a_LogOutput(1, "CECNPCMan::NPCEnter, Failed to create NPC (%d)", Info.tid);
		return false;
	}
	if (pNPC->IsServerNPC())//should we check if the name is not default??
		AddNPCNameAndID(Info.nid, pNPC->GetName());

//	pNPC->SetTransparence(1.0f, 0.0f, 3000);

	//	Add NPC to table
	m_NPCTab.put(Info.nid, pNPC);

	if (pNPC->IsCarrierNPC())
		m_aCarriers.Add((CECNPCCarrier*)pNPC);

	return true;
}

//	One NPC leave view area
void CECNPCMan::NPCLeave(int nid, bool bUpdateMMArray/* true */, bool bRelease/* true */)
{
	//	Release NPC
	CECNPC* pNPC = GetNPC(nid);
	if (!pNPC)
		return;
	if (pNPC->IsServerNPC())
		DelNPCNameAndID(nid, pNPC->GetName());

	if (bUpdateMMArray)
		RemoveNPCFromMiniMap(pNPC);

	pNPC->m_iMMIndex = -1;

	//	If this NPC is selected by host, cancel the selection
// 	if (pNPC->GetNPCID() == g_pGame->GetGameRun()->GetHostPlayer()->GetSelectedTarget())
// 		g_pGame->GetGameSession()->c2s_CmdUnselect();

	//	Remove it from active NPC table
	m_NPCTab.erase(nid);

	//	Forbid reloading npc's resources
	DWORD dwBornStamp = pNPC->GetBornStamp();
	QueuePlayerUndoLoad(nid, dwBornStamp);

	//	Release NPC resource
	if (bRelease)
		ReleaseNPC(pNPC);
	else
		RemoveNPCFromTabSels(pNPC);
}

//	Remove npc from tab-selected array
void CECNPCMan::RemoveNPCFromTabSels(CECNPC* pNPC)
{
	for (int i=0; i < m_aTabSels.GetSize(); i++)
	{
		if (m_aTabSels[i] == pNPC)
		{
			m_aTabSels.RemoveAtQuickly(i);
			break;
		}
	}
}

//	Get NPC of specified ID
CECNPC* CECNPCMan::GetNPC(int nid, DWORD dwBornStamp/* 0 */)
{
	//	NPC exists ?
	NPCTable::pair_type Pair = m_NPCTab.get(nid);
	if (!Pair.second)
		return NULL;	//	Counldn't find this NPC

	if (dwBornStamp)
	{
		CECNPC* pNPC = *Pair.first;
		if (pNPC->GetBornStamp() != dwBornStamp)
			return NULL;
	}

	return *Pair.first;
}

//  Get NPC of specified template ID
CECNPC* CECNPCMan::GetNPCByTemplateID(int tid)
{
	//	Check all NPCs
	CECNPC* pNearestNPC = NULL;
	float fMinDist = 99999.0f;
	float MIN_DIST = 20.0f;

	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();

		if(pNPC->GetTemplateID() == tid)
		{
			float fDistToHostH = pNPC->GetDistToHostH();
			if(fDistToHostH < fMinDist)
			{
				fMinDist = fDistToHostH;
				pNearestNPC = pNPC;
			}
		}
	}

	if(fMinDist < MIN_DIST)
		return pNearestNPC;

	return NULL;
}


CECNPC* CECNPCMan::GetNPCFromAll(int nid)
{
	CECNPC* pNPC = GetNPC(nid);
	if (pNPC)
		return pNPC;

	//	Search from disappear array ?
	for (int i=0; i < m_aDisappearNPCs.GetSize(); i++)
	{
		CECNPC* pNPC = m_aDisappearNPCs[i];
		if (pNPC->GetNPCID() == nid)
			return pNPC;
	}

	return NULL;
}

//	Get NPC's name
const wchar_t* CECNPCMan::GetNPCName(int nid)
{
	CECNPC* pNPC = GetNPC(nid);
	if (pNPC)
		return pNPC->GetName();
	else
		return L"Unknown NPC";
}

//	Seek out NPC with specified id, if NPC doesn't exist, try to get from server
CECNPC* CECNPCMan::SeekOutNPC(int nid)
{
	NPCTable::pair_type Pair = m_NPCTab.get(nid);
	if (!Pair.second)
	{
		//	Counldn't find this NPC, put it into unkonwn NPC table
		m_UkNPCTab.put(nid, nid);
		return NULL;
	}

	return *Pair.first;
}

//	Ray trace
bool CECNPCMan::RayTrace(ECRAYTRACE* pTraceInfo)
{
	//	If alt is pressed, ignore all players
// 	if (pTraceInfo->dwKeyStates & EC_KSF_ALT)
// 		return false;

	bool bRet = false;

	//	Trace all NPCs
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();

		//	TRACEOBJ_LBTNCLICK is caused by player pressed left button
		if (pNPC->IsDead() && (pTraceInfo->iTraceObj == TRACEOBJ_LBTNCLICK || 
			pTraceInfo->iTraceObj == TRACEOBJ_TESTRAY))
			continue;
		
		//  if shift is pressed, only chose server npc
// 		if ((pTraceInfo->dwKeyStates & EC_KSF_SHIFT) && (!pNPC->IsServerNPC()) )
// 			continue;

		//  if F10 is pressed, pet npc is invisival, so do not chose pet npc
		if (g_pGame->GetGameRun()->IsHideElsePlayer() && pNPC->IsPetNPC())
			continue;
		
		// if npc can not be chosed anyway...
		if (pNPC->GetSelectStrategy() == 2)
			continue;

		if (pNPC->RayTrace(pTraceInfo))
		{
			bRet = true;
		}
	}

	return bRet;
}

// Trace for CD
bool CECNPCMan::TraceWithBrush(BrushTraceInfo * pInfo, bool bOnlyCarrier /* = false */)
{
	bool bCollide=false;
	int idCarrier = 0;
	CECHostPlayer* pHost = m_pGameRun->GetHostPlayer();

	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index
	float		fFraction = 100.0f;		//	Fraction
	A3DVECTOR3  vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist

	// now see if collide with dynamic scene building
	NPCTable::iterator it = m_NPCTab.begin();
   	A3DVECTOR3 resDelta(pInfo->vDelta);
    A3DVECTOR3 resStart(pInfo->vStart);
    PosPair StartPair = pHost->GetStartPosPair();
	PosPair	EndPair;
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();

		if (!pNPC)
			continue;

		if (bOnlyCarrier && !pNPC->IsCarrierNPC())
			continue;

		if (pNPC->IsCarrierNPC())
		{
			//	Carrier, 变换到载体的局部坐标系
			A3DVECTOR3 vDelta = pInfo->vDelta;
			A3DMATRIX4  ToLocal = InverseTM(pNPC->GetAbsoluteTM()); 
			vDelta = a3d_VectorMatrix3x3(vDelta, ToLocal);
            pInfo->vDelta.x = vDelta.x;
            pInfo->vDelta.y = vDelta.y;
            pInfo->vDelta.z = vDelta.z;

			EndPair = pHost->GetEndPosPair();
			if (StartPair.bRecorded && (pInfo->vStart == StartPair.vAbsPos) && (StartPair.iCarrierID == pNPC->GetNPCID()))
				pInfo->vStart = StartPair.vRelPos;
			else if (EndPair.bRecorded && (pInfo->vStart == EndPair.vAbsPos) && (EndPair.iCarrierID == pNPC->GetNPCID()))
				pInfo->vStart = EndPair.vRelPos;
			else
				pInfo->vStart = pInfo->vStart * ToLocal;
		}

		if ((fFraction > 0.0f) && pNPC && pNPC->TraceWithBrush(pInfo) && (pInfo->fFraction < fFraction)) 
		{
			fFraction = pInfo->fFraction;
			bAllSolid = pInfo->bAllSolid;
			bStartSolid = pInfo->bStartSolid;
			iClipPlane = pInfo->iClipPlane;
			bCollide = true;

			if (pNPC->IsCarrierNPC())
			{
				//	将结果变换回世界坐标系
				CHBasedCD::CHalfSpace HStemp;
				HStemp.SetNormal(pInfo->ClipPlane.GetNormal());
				HStemp.SetD(pInfo->ClipPlane.GetDist());
				HStemp.Transform(pNPC->GetAbsoluteTM());
				vNormal = HStemp.GetNormal();
				fDist = HStemp.GetDist();

				//	Recode end position
// 				PosPair tmpPair;
// 				tmpPair.bRecorded = true;
// 				tmpPair.vRelPos = pInfo->vStart + pInfo->vDelta * pInfo->fFraction;
// 				tmpPair.vAbsPos = resStart + resDelta * pInfo->fFraction;
// 				tmpPair.iCarrierID = pNPC->GetNPCID();
//				pHost->SetEndPosPair(tmpPair);
			}
			else
			{
				vNormal = pInfo->ClipPlane.GetNormal();
				fDist = pInfo->ClipPlane.GetDist();
			}
		
			idCarrier = pNPC->IsCarrierNPC() ? pNPC->GetNPCID() : 0;
		}

		if (pNPC->IsCarrierNPC())
		{
			//	恢复到世界坐标系
			if (pNPC->GetNPCID() == pHost->GetCarrierID() && (pInfo->fFraction >= fFraction))
			{
				//	当前的carrier
				float fFract;
				if (pInfo->fFraction > 1.0f)
					fFract = 1.0f;
				else
					fFract = pInfo->fFraction;
				
				//	Recode end position
				PosPair tmpPair;
				tmpPair.bRecorded = true;
				tmpPair.vRelPos = pInfo->vStart + pInfo->vDelta * fFract;
				tmpPair.vAbsPos = resStart + resDelta * fFract;
				tmpPair.iCarrierID = pNPC->GetNPCID();
				pHost->SetEndPosPair(tmpPair);
			}

			pInfo->vStart = resStart;
			pInfo->vDelta = resDelta;
		}
	}		

	//set back
	pInfo->fFraction = fFraction;
	pInfo->bStartSolid = bStartSolid;
	pInfo->bAllSolid = bAllSolid;
	pInfo->iClipPlane = iClipPlane;
	pInfo->ClipPlane.SetNormal(vNormal);
	pInfo->ClipPlane.SetD(fDist);

	if (idCarrier)
	{
#ifdef BACK_VERSION
		pInfo->iUserData1 = ECENT_CARRIER;
		pInfo->iUserData2 = idCarrier;
#else
		pInfo->dwUser1 = ECENT_CARRIER;
		pInfo->dwUser2 = idCarrier;
#endif
	}

	return bCollide;
}

//	Update NPCs in various ranges (Active, visible, mini-map etc.)
void CECNPCMan::UpdateNPCInRanges(DWORD dwDeltaTime)
{
	CECHostPlayer* pHost = m_pGameRun->GetHostPlayer();
	if (!pHost || !pHost->IsAllResReady())
		return;

	CECConfigs* pConfigs = g_pGame->GetConfigs();
	int idHoverObject = pHost->GetCursorHoverObject();
	int idSelected = pHost->GetSelectedTarget();
	bool bRenderNameByCamera = true;
// 	if (g_pGame->GetGameRun()->GetHostPlayer() && 
// 		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() && 
// 		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene() && 
// 		!g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsNameShow())
// 	{
// 		bRenderNameByCamera = false;
// 	}

	m_aMMNPCs.RemoveAll(false);
	
	//	Check all NPCs
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); )
	{
		CECNPC* pNPC = *it.value();
		float fDistToCamera = pNPC->GetDistToCamera();
		float fDistToHost = pNPC->GetDistToHost();
		float fDistToHostH = pNPC->GetDistToHostH();

		//	Check whether name is visible
		if (bRenderNameByCamera && (fDistToHost <= 20.0f || pNPC->GetNPCID() == idHoverObject || pNPC->GetNPCID() == idSelected) )
			pNPC->SetRenderNameFlag(true);
		else
			pNPC->SetRenderNameFlag(false);

		//	Set bars visible flags
		pNPC->SetRenderBarFlag(bRenderNameByCamera && pNPC->GetNPCID() == idSelected);

		//	Check whether NPC is in mini-map
		if (fDistToHostH > pConfigs->GetSevActiveRadius())
		{
			++it;
			NPCLeave(pNPC->GetNPCID(), false);
			continue;
		}
		else if (fDistToHostH <= pConfigs->GetSevActiveRadius())
			pNPC->m_iMMIndex = m_aMMNPCs.Add(pNPC);

		//	Check NPC's visibility
		pNPC->SetVisible(fDistToCamera <= pConfigs->GetSevActiveRadius() ? true : false);

		++it;
	}
}

//	Remove NPC from m_aMMNPCs array
void CECNPCMan::RemoveNPCFromMiniMap(CECNPC* pNPC)
{
	ASSERT(pNPC);
	int iIndex = pNPC->m_iMMIndex;
	if (iIndex < 0 || iIndex >= m_aMMNPCs.GetSize())
		return;

	CECNPC* pTempNPC = m_aMMNPCs[iIndex];
	if (pTempNPC != pNPC)
	{
		ASSERT(pTempNPC == pNPC);
		return;
	}

	//	Exchange with the last layer in array
	int iLastIdx = m_aMMNPCs.GetSize() - 1;
	if (m_aMMNPCs.GetSize() > 1 && iIndex != iLastIdx)
	{
		pTempNPC = m_aMMNPCs[iLastIdx];
		m_aMMNPCs[iIndex] = pTempNPC;
		pTempNPC->m_iMMIndex = iIndex;
		m_aMMNPCs.RemoveAt(iLastIdx);
	}
	else
		m_aMMNPCs.RemoveAtQuickly(iIndex);
}

//	When world's loading center changed
void CECNPCMan::OnLoadCenterChanged()
{
	m_aMMNPCs.RemoveAll(false);
}

//	Update unknown NPC table
void CECNPCMan::UpdateUnknownNPCs()
{
	DWORD dwRealTime = g_pGame->GetRealTickTime();
	if (!m_QueryCnt.IncCounter(dwRealTime))
		return;

	m_QueryCnt.Reset();

	if (!m_UkNPCTab.size())
		return;

	AArray<int, int> aIDs;

	UkNPCTable::iterator it = m_UkNPCTab.begin();
	for (; it != m_UkNPCTab.end(); ++it)
		aIDs.Add(*it.value());

//	g_pGame->GetGameSession()->c2s_CmdQueryNPCInfo1(aIDs.GetSize(), aIDs.GetData());

	m_UkNPCTab.clear();
}

//	Load NPC model in loading thread
bool CECNPCMan::ThreadLoadNPCModel(int nid, DWORD dwBornStamp, int tid, const char* szFile)
{
	NPCMODEL* pInfo = new NPCMODEL;
	pInfo->nid = nid;
	pInfo->dwBornStamp = dwBornStamp;

	if (!CECNPC::LoadNPCModel(tid, szFile, pInfo->Ret))
	{
		delete pInfo;
		return false;
	}

	ACSWrapper csa(&m_csLoad);
	m_aLoadedModels.Add(pInfo);

	return true;
}

//	Deliver loaded NPC models
void CECNPCMan::DeliverLoadedNPCModels()
{
	ACSWrapper csa(&m_csLoad);

	for (int i=0; i < m_aLoadedModels.GetSize(); i++)
	{
		NPCMODEL* pInfo = m_aLoadedModels[i];
		CECNPC* pNPC = GetNPC(pInfo->nid, pInfo->dwBornStamp);
		if (pNPC)
			pNPC->SetNPCLoadedResult(pInfo->Ret);
		else
			CECNPC::ReleaseNPCModel(pInfo->Ret);
	
		delete pInfo;
	}

	m_aLoadedModels.RemoveAll(false);
}

//	Auto-select a monster through 'Tab' key
int CECNPCMan::TabSelectMonster(int idCurSel)
{
	const float fSelDist = 20.0f;
	int i;

	if (!idCurSel)
	{
		//	Rebuild selected table
		m_aTabSels.RemoveAll(false);
	}
	else
	{
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (m_aTabSels[i]->GetDistToHost() > fSelDist)
			{
				m_aTabSels.RemoveAtQuickly(i);
				i--;
			}
		}
	}

	float fMinDist = 10000.0f;
	CECNPC* pCand = NULL;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	//	Trace all NPCs
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
//		if (!pNPC->IsMonsterNPC() || pNPC->IsDead() || pNPC->GetNPCID() == idCurSel || pNPC->GetSummonerID())
		if ( (pNPC->IsServerNPC() && !pNPC->IsAttackable()) || 
			pNPC->IsPetNPC() || pNPC->IsCarrierNPC() || 
			pNPC->IsDead() || pNPC->GetNPCID() == idCurSel || 
			pNPC->GetSummonerID())
			continue;
		// 不选同一个阵营的怪，观察者不能选怪
		if (pHost->IsInBattle() && (pHost->GetBattleCamp() == GP_BATTLE_CAMP_NONE || 
			(pHost->GetBattleCamp()== GP_BATTLE_CAMP_INVADER && pNPC->IsInBattleInvaderCamp() ) ||
			(pHost->GetBattleCamp() == GP_BATTLE_CAMP_DEFENDER && pNPC->IsInBattleDefenderCamp() ) || 
			(pHost->GetBattleCamp() == GP_BATTLE_CAMP_OBSERVER) ) )
			continue;
		
		// 仙佛魔阵营的判断未加
		// 人神pk的判断
		if ( ( pHost->GetPVPInfo().ucFreePVPType == 3) && 
			(pNPC->IsServerNPC() ||pNPC->IsMonsterNPC()) &&
			( (pHost->GetRace() == RACE_HUMAN && pNPC->IsHumanCamp() ) || (pHost->GetRace() == RACE_ORC && pNPC->IsOrcCamp()) ))
			continue;

		// 不能被tab选中
		if (pNPC->GetSelectStrategy() == 1 || pNPC->GetSelectStrategy() == 2)
			continue;

		float fDist = pNPC->GetDistToHost();
		if (fDist > fSelDist)
			continue;

		//	Check whether this npc is in selected array
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (pNPC == m_aTabSels[i])
				break;
		}

		if (i < m_aTabSels.GetSize())
			continue;	//	This npc has been in selected array


		//	Record the nearest one as candidate
		if (fDist < fMinDist)
		{
			fMinDist = fDist;
			pCand	 = pNPC;
		}
	}

	const int iMaxCand = 4;

	if (pCand)
	{
		if (m_aTabSels.GetSize() >= iMaxCand)
		{
			m_aTabSels.RemoveAt(m_aTabSels.GetSize()-1);
			m_aTabSels.InsertAt(0, pCand);
		}
		else
			m_aTabSels.Add(pCand);

		return pCand->GetNPCID();
	}
	else	//	No proper candidate was found
	{
		if (!m_aTabSels.GetSize())
			return idCurSel;	//	Return without change

		//	Try to select one which has been in selected array
		int iIndex = -1;
		for (i=0; i < m_aTabSels.GetSize(); i++)
		{
			if (m_aTabSels[i]->GetNPCID() == idCurSel)
			{
				iIndex = i;
				break;
			}
		}

		iIndex = (iIndex + 1) % m_aTabSels.GetSize();
		return m_aTabSels[iIndex]->GetNPCID();
	}
}

//	Calculate object's relative transform matrix to specified carrier
bool CECNPCMan::CalcCarrierRelMat(int idCarrier, const A3DMATRIX4& matAbs, A3DMATRIX4& matRel)
{
	CECNPC* pNPC = GetNPC(idCarrier);
	if (!pNPC || !pNPC->IsCarrierNPC())
		return false;

	A3DMATRIX4 matInvCarrier;
	a3d_InverseTM(pNPC->GetAbsoluteTM(), &matInvCarrier);
	matRel = matAbs * matInvCarrier;

	return true;
}

//	Get a world direction transformed in a carrier coordinate
bool CECNPCMan::GetCarrierRelDir(int idCarrier, const A3DVECTOR3& vAbsDir, A3DVECTOR3& vRelDir)
{
	CECNPC* pNPC = GetNPC(idCarrier);
	if (!pNPC || !pNPC->IsCarrierNPC())
		return false;
	
	A3DMATRIX4 matInvCarrier;
	a3d_InverseTM(pNPC->GetAbsoluteTM(), &matInvCarrier);
	vRelDir = a3d_VectorMatrix3x3(vAbsDir, matInvCarrier);

	return true;
}

//	Get a value of a world position transformed to a carrier coordinate
bool CECNPCMan::GetCarrierRelPos(int idCarrier, const A3DVECTOR3& vAbsPos, A3DVECTOR3& vRelPos)
{
	CECNPC* pNPC = GetNPC(idCarrier);
	if (!pNPC || !pNPC->IsCarrierNPC())
		return false;
	
	A3DMATRIX4 matInvCarrier;
	a3d_InverseTM(pNPC->GetAbsoluteTM(), &matInvCarrier);
	vRelPos = vAbsPos * matInvCarrier;

	return true;
}

//	Get object's pose on specified carrier
bool CECNPCMan::GetPoseOnCarrier(int idCarrier, const A3DMATRIX4& matRel, A3DVECTOR3& vPos, A3DVECTOR3& vDir)
{
	CECNPC* pNPC = GetNPC(idCarrier);
	if (!pNPC || !pNPC->IsCarrierNPC())
		return false;

	A3DMATRIX4 matAbs = matRel * pNPC->GetAbsoluteTM();
	vPos = matAbs.GetRow(3);
	vDir = matAbs.GetRow(2);

	vDir.y = 0.0f;
	vDir.Normalize();

	return true;
}

//	Check if a position in a specific valid carrier range
bool CECNPCMan::InValidCarrierRange(int idCarrier, const A3DVECTOR3& vPos)
{
	CECNPC* pNPC = GetNPC(idCarrier);
	if (!pNPC || !pNPC->IsCarrierNPC())
		return false;
	
	A3DMATRIX4 matInvCarrier;
	a3d_InverseTM(pNPC->GetAbsoluteTM(), &matInvCarrier);
	A3DVECTOR3 vRelPos = vPos * matInvCarrier;

	return false;		
}


int CECNPCMan::GetNPCIDbyName(const ACHAR* szName)
{
	NPCName2IDTab::pair_type Pair = m_ServerNPCName2IDTab.get(szName);
	return Pair.second ? *Pair.first : 0;
}

//	Add player name to table
void CECNPCMan::AddNPCNameAndID(int nid, const ACHAR* szName)
{
	//for test hongkan
	AString name = WC2AS(szName);
	
	NPCName2IDTab::pair_type Pair = m_ServerNPCName2IDTab.get(szName);
	if (Pair.second)
	{
		if (*Pair.first != nid)
		{
			m_ServerNPCName2IDTab.erase(szName);
			m_ServerNPCName2IDTab.put(szName, nid);
		}
		return;
	}
	
	//	Create new name
	ACString pstrName = szName;
	if (pstrName)
		m_ServerNPCName2IDTab.put(pstrName, nid);
}


void CECNPCMan::DelNPCNameAndID(int nid, const ACHAR* szName)
{	
	NPCName2IDTab::pair_type Pair = m_ServerNPCName2IDTab.get(szName);
	if (Pair.second)
	{
		if (*Pair.first == nid)
		{
			m_ServerNPCName2IDTab.erase(szName);
		}
		return;
	}
}

#ifndef BACK_VERSION
bool CECNPCMan::RenderForShadow( ECViewport* pViewport )
{
	// all npc are refract visible
	NPCTable::iterator it = m_NPCTab.begin();
	for (; it != m_NPCTab.end(); ++it)
	{
		CECNPC* pNPC = *it.value();
		pNPC->Render(pViewport, CECObject::RD_SHADOW);
	}

	return true;
}
#endif