/*
 * FILE: EC_ManMatter.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "EC_Global.h"
#include "EC_ManMatter.h"
#include "EC_Matter.h"
#include "EC_CDS.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_HostPlayer.h"
#include "EC_Configs.h"
#include "EC_Inventory.h"
#include "EC_Model.h"
#include "EC_SceneLoader.h"
#include "EC_GPDataType.h"
#include "aabbcd.h"
#include "CDWithCH.h"
#include "EC_GameRecord.h"
#include "EC_Cutscene.h"

#include "A3DCollision.h"
#include "A3DFuncs.h"
#include "A3DSkinMan.h"
#include "A3DSkinRender.h"

#include "EC_Helper.h"
#include "EC_AutoHelper.h"

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
//	Implement CECMatterMan
//	
///////////////////////////////////////////////////////////////////////////

CECMatterMan::CECMatterMan(CECGameRun* pGameRun) :
CECManager(pGameRun),
m_MatterTab(512),
m_DynModelTab(512)
{
	m_iManagerID = MAN_MATTER;

	InitializeCriticalSection(&m_csLoad);
}

CECMatterMan::~CECMatterMan()
{
	DeleteCriticalSection(&m_csLoad);
}

//	Release manager
void CECMatterMan::Release()
{
	OnLeaveGameWorld();
}

//	Get matter throuth it's ID
CECMatter* CECMatterMan::GetMatter(int mid, DWORD dwBornStamp/* 0 */)
{
	//	Matter exists ?
	MatterTable::pair_type Pair = m_MatterTab.get(mid);
	if (!Pair.second)
		return NULL;	//	Counldn't find this matter

	if (dwBornStamp)
	{
		CECMatter* pMatter = *Pair.first;
		if (pMatter->GetBornStamp() != dwBornStamp)
			return NULL;
	}

	return *Pair.first;
}

//	Find a matter which is near enough to host player
CECMatter* CECMatterMan::FindMatterNearHost(float fDist, bool bPickCheck)
{
	CECHostPlayer* pHost = m_pGameRun->GetHostPlayer();
	CECInventory* pPack = pHost->GetPack(IVTRTYPE_PACK);

	CMatterSelector * pSelector = (m_pGameRun->GetHelper()&& m_pGameRun->GetHelper()->IsRunning())?
								  (m_pGameRun->GetHelper()->GetMatterSelector()):(NULL);

	float fNearest = fDist;
	CECMatter* pBest = NULL;

	//	Look all matters and find out the nearest one
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		float fMatterDist = pMatter->GetDistToHost();
		if (pMatter->IsMine() || fMatterDist > fNearest)
			continue;

		if (bPickCheck && !pHost->CanTakeItem(pMatter->GetTemplateID(), 1))
			continue;

		if (pSelector && !pSelector->IsTarget(pMatter))
		{
			continue;
		}
		
		fNearest = fMatterDist;
		pBest = pMatter;
	}

	return pBest;
}

//	Tick routine
bool CECMatterMan::Tick(DWORD dwDeltaTime)
{
	//	Deliver loaded matter models
	DeliverLoadedMatterModels();

	//	Tick all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		pMatter->Tick(dwDeltaTime);
	}

	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		pMatter->Tick(dwDeltaTime);
	}

	//	Update matters in various ranges (Active, visible, etc.)
	UpdateMatterInRanges(dwDeltaTime);

	return true; 
}

#ifdef BACK_VERSION
//	Render routine
bool CECMatterMan::Render(CECViewport* pViewport)
{
	CECMatter* pHHMatter = NULL;	//	The matter will be high-light rendered
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int idHoverObject = pHost ? pHost->GetCursorHoverObject() : 0;
	
	//	Render all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		if (idHoverObject == pMatter->GetMatterID())
			pHHMatter = pMatter;
		else
			pMatter->Render(pViewport);
	}
	
	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		pMatter->Render(pViewport);
	}
	
	//	Render high-lighted matter
	if (pHHMatter)
		pHHMatter->RenderHighLight(pViewport);
	
	return true; 
}
#else
bool CECMatterMan::InternalRender( ECViewport* pViewport, int nRenderFlag )
{
	CECMatter* pHHMatter = NULL;	//	The matter will be high-light rendered
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int idHoverObject = pHost ? pHost->GetCursorHoverObject() : 0;
	
	//	Render all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		if (idHoverObject == pMatter->GetMatterID())
			pHHMatter = pMatter;
		else
			pMatter->Render(pViewport, nRenderFlag);
	}
	
	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		pMatter->Render(pViewport, nRenderFlag);
	}
	
	//	Render high-lighted matter
	if (pHHMatter)
		pHHMatter->RenderHighLight(pViewport, nRenderFlag);
	
	return true; 
}

bool CECMatterMan::Render(ECViewport* pViewport)
{
	return InternalRender(pViewport, CECObject::RD_NORMAL);
}

bool CECMatterMan::RenderForReflect(ECViewport * pViewport)
{
	return InternalRender(pViewport, CECObject::RD_REFLECT);
}

bool CECMatterMan::RenderForRefract(ECViewport * pViewport)
{
	return InternalRender(pViewport, CECObject::RD_REFRACT);
}

bool CECMatterMan::RenderForShadow( ECViewport* pViewport )
{
	return InternalRender(pViewport, CECObject::RD_SHADOW);
}
#endif

//	On entering game world
bool CECMatterMan::OnEnterGameWorld()
{
	return true; 
}

//	On leaving game world
bool CECMatterMan::OnLeaveGameWorld()
{
	//	Release all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		ReleaseMatter(pMatter);
	}

	m_MatterTab.clear();

	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		ReleaseMatter(pMatter);
	}

	m_DynModelTab.clear();

	//	Release all un-delivered matter models
	ACSWrapper csa(&m_csLoad);

	for (int i=0; i < m_aLoadedMats.GetSize(); i++)
	{
		const MATTERMODEL& m = m_aLoadedMats[i];
		if (m.pModel)
			CECMatter::ReleaseMatterModel(m.pModel);
	}

	m_aLoadedMats.RemoveAll();

	return true; 
}

//	Process message
bool CECMatterMan::ProcessMessage(const ECMSG& Msg)
{
	ASSERT(Msg.iManager == MAN_MATTER);

	if (!Msg.iSubID)
	{
		switch (Msg.dwMsg)
		{
		case MSG_MM_MATTERINFO:			OnMsgMatterInfo(Msg);		break;
		case MSG_MM_MATTERDISAPPEAR:	OnMsgMatterDisappear(Msg);	break;
		case MSG_MM_MATTERENTWORLD:		OnMsgMatterEnterWorld(Msg);	break;
		case MSG_MM_MATTEROUTOFVIEW:	OnMsgMatterOutOfView(Msg);	break;
		case MSG_MM_INVALIDOBJECT:		OnMsgInvalidObject(Msg);	break;
		case MSG_MM_MINECHANGESTATE:	OnMsgCombineMineStateChange(Msg);break;
		}
	}

	return true; 
}

//	Create a matter
CECMatter* CECMatterMan::CreateMatter(const S2C::info_matter& Info)
{
	CECMatter* pMatter = new CECMatter(this);
	if (!pMatter)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECMatterMan::CreateMatter", __LINE__);
		return NULL;
	}

	if (!pMatter->Init(Info))
	{
		glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECMatterMan::CreateMatter", __LINE__);
		return NULL;
	}

	//	Set born stamp
	pMatter->SetBornStamp(g_pGame->GetGameRun()->GetWorld()->GetBornStamp());

	return pMatter;
}

//	Release a matter
void CECMatterMan::ReleaseMatter(CECMatter* pMatter)
{
	if (pMatter)
	{
		pMatter->Release();
		delete pMatter;
		pMatter = NULL;
	}
}

//	One matter enter view area
bool CECMatterMan::MatterEnter(const S2C::info_matter& Info)
{
	CECMatter* pMatter = GetMatter(Info.mid);
	if (pMatter)
		return true;

	//	Create a new matter
	if (!(pMatter = CreateMatter(Info)))
	{
		a_LogOutput(1, "CECMatterMan::MatterEnter, Failed to create matter (%d)", Info.tid);
		return false;
	}

	//	Add matter to table
	if (pMatter->IsDynModel())
		m_DynModelTab.put(Info.mid, pMatter);
	else
		m_MatterTab.put(Info.mid, pMatter);

	return true;
}

GNET::GamedataSend CECMatterMan::gen_matter_list()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(4000);
	cmd_header h;
	h.cmd = MATTER_INFO_LIST;
	add_to_octets<cmd_header>(o, h);
	unsigned short count = unsigned short(m_MatterTab.size() + m_DynModelTab.size());
	add_to_octets<unsigned short>(o, count);

	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		GNET::Octets o1 = (*it.value())->gen_matter_info();
		o.insert(o.end(), o1.begin(), o1.size());
	}

	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		GNET::Octets o1 = (*it.value())->gen_matter_info();
		o.insert(o.end(), o1.begin(), o1.size());
	}

	return gds;
}

//	One matter leave view area
void CECMatterMan::MatterLeave(int mid)
{
	//	Release matter
	CECMatter* pMatter = GetMatter(mid);

	if (pMatter)
	{
		ReleaseMatter(pMatter);

		//	Remove it from active matter table
		m_MatterTab.erase(mid);
		
		QueuePlayerUndoLoad(mid, 0);
	}
	else
	{
		MatterTable::pair_type Pair = m_DynModelTab.get(mid);

		if (Pair.second)
		{
			pMatter = *Pair.first;

			ReleaseMatter(pMatter);

			//	Remove it from active matter table
			m_DynModelTab.erase(mid);
		
			QueuePlayerUndoLoad(mid, 0);
		}
	}
}

bool CECMatterMan::OnMsgMatterInfo(const ECMSG& Msg)
{
	using namespace S2C;

	BYTE* pDataBuf = (BYTE*)Msg.dwParam1;
	ASSERT(pDataBuf);

	switch (Msg.dwParam2)
	{
	case MATTER_INFO_LIST:
	{
		cmd_matter_info_list cmd;
		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
			return false;

		for (int i=0; i < cmd.count; i++)
			MatterEnter(cmd.matters[i]);

		break;
	}
	}

	return true;
}

bool CECMatterMan::OnMsgMatterDisappear(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_disappear* pCmd = (cmd_object_disappear*)Msg.dwParam1;
	ASSERT(pCmd && ISMATTERID(pCmd->id));

	MatterLeave(pCmd->id);

	return true;
}

bool CECMatterMan::OnMsgMatterEnterWorld(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_matter_enter_world cmd;
	cmd.Info.FillData((void*)Msg.dwParam1, Msg.dwParam3);
	ASSERT(ISMATTERID(cmd.Info.mid));

	MatterEnter(cmd.Info);

	return true;
}

bool CECMatterMan::OnMsgMatterOutOfView(const ECMSG& Msg)
{
	MatterLeave((int)Msg.dwParam1);
	return true;
}

bool CECMatterMan::OnMsgInvalidObject(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_invalid_object* pCmd = (cmd_invalid_object*)Msg.dwParam1;
	MatterLeave(pCmd->id);
	return true;
}

bool CECMatterMan::OnMsgCombineMineStateChange(const ECMSG& Msg)
{
	using namespace S2C;
	
	combine_mine_state_change* pCmd = (combine_mine_state_change*)Msg.dwParam1;

	CECMatter* pMatter = GetMatter(pCmd->matter_id);
	if (!pMatter)
		return false;

	return pMatter->ChangeMineState(pCmd->state);
}

//	Update matters in various ranges (Active, visible, etc.)
void CECMatterMan::UpdateMatterInRanges(DWORD dwDeltaTime)
{
	CECHostPlayer* pHost = m_pGameRun->GetHostPlayer();
	if (!pHost || !pHost->IsAllResReady())
		return;

	CECConfigs* pConfigs = g_pGame->GetConfigs();
	int idHoverObject = pHost->GetCursorHoverObject();
	bool bRenderNameByCamera = true;
	if (g_pGame->GetGameRun()->GetHostPlayer() && 
		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene() && 
		g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsPlayingCutscene() && 
		!g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->IsNameShow())
	{
		bRenderNameByCamera = false;
	}

	//	Check all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); )
	{
		CECMatter* pMatter = *it.value();
		float fDistToCamera = pMatter->GetDistToCamera();
		float fDistToHost = pMatter->GetDistToHost();
		float fDistToHostH = pMatter->GetDistToHostH();

		//	Check whether name is visible
		if (bRenderNameByCamera && (fDistToHost <= 20.0f || pMatter->GetMatterID() == idHoverObject))
			pMatter->SetRenderNameFlag(true);
		else
			pMatter->SetRenderNameFlag(false);

		if (fDistToHostH > pConfigs->GetSevActiveRadius())
		{
			++it;
			MatterLeave(pMatter->GetMatterID());
			continue;
		}

		//	Check matter's visiblity
		pMatter->SetVisible(fDistToCamera <= pConfigs->GetSevActiveRadius() ? true : false);

		++it;
	}

	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); )
	{
		CECMatter* pMatter = *it.value();
		float fDistToCamera = pMatter->GetDistToCamera();
		float fDistToHost = pMatter->GetDistToHost();
		float fDistToHostH = pMatter->GetDistToHostH();

		//	Check whether name is visible
		if (bRenderNameByCamera && (fDistToHost <= 20.0f || pMatter->GetMatterID() == idHoverObject))
			pMatter->SetRenderNameFlag(true);
		else
			pMatter->SetRenderNameFlag(false);

		if (fDistToHostH > pConfigs->GetSevActiveRadius())
		{
			++it;
			MatterLeave(pMatter->GetMatterID());
			continue;
		}

		//	Check matter's visiblity
		pMatter->SetVisible(fDistToCamera <= pConfigs->GetSevActiveRadius() ? true : false);

		++it;
	}
}

//	When world's loading center changed
void CECMatterMan::OnLoadCenterChanged()
{
}

//	Load matter model in loading thread
bool CECMatterMan::ThreadLoadMatterModel(int mid, const char* szFile)
{
	MATTERMODEL Info;
	Info.mid = mid;

	CECModel* pModel = CECMatter::LoadMatterModel(szFile);
	if (!pModel)
		return false;
	
	//	Force to use mesh aabb
	pModel->SetAABBType(CECModel::AABB_INITMESH);

	Info.pModel = pModel;

	ACSWrapper csa(&m_csLoad);
	m_aLoadedMats.Add(Info);

	return true;
}

//	Deliver loaded matter models
void CECMatterMan::DeliverLoadedMatterModels()
{
	ACSWrapper csa(&m_csLoad);

	for (int i=0; i < m_aLoadedMats.GetSize(); i++)
	{
		const MATTERMODEL& m = m_aLoadedMats[i];

		//	Get matter object
		CECMatter* pMatter = GetMatter(m.mid);

		if (!pMatter)
		{
			MatterTable::pair_type Pair = m_DynModelTab.get(m.mid);

			if (Pair.second)
				pMatter = * Pair.first;
		}

		if (pMatter)
			pMatter->SetLoadedMatterModel(m.pModel);
		else
		{
			//	Counldn't find this matter, it may has been removed
			//	before resource is loaded !
			CECMatter::ReleaseMatterModel(m.pModel);
		}
	}

	m_aLoadedMats.RemoveAll(false);
}

//	Ray trace
bool CECMatterMan::RayTrace(ECRAYTRACE* pTraceInfo)
{
	bool bRet = false;

	//	Trace all matters
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();

		if(!pMatter->IsPhaseShow())
			continue;

		if (pMatter->RayTrace(pTraceInfo))
		{
			bRet = true;
		}
	}

	it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		
		if(!pMatter->IsPhaseShow())
			continue;

		if (pMatter->RayTrace(pTraceInfo))
		{
			bRet = true;
		}
	}

	return bRet;
}

bool CECMatterMan::TraceWithBrush(CHBasedCD::BrushTraceInfo * pInfo)
{
	bool bCollide=false;
	//save original result
	bool		bStartSolid = pInfo->bStartSolid;	//	Collide something at start point
	bool		bAllSolid = pInfo->bAllSolid;		//	All in something
	int			iClipPlane = pInfo->iClipPlane;		//	Clip plane's index
	float		fFraction = 100.0f;		//	Fraction
	A3DVECTOR3  vNormal = pInfo->ClipPlane.GetNormal(); //clip plane normal
	float       fDist = pInfo->ClipPlane.GetDist();	//clip plane dist
	
	// now see if collide with dynamic scene building
	MatterTable::iterator it = m_DynModelTab.begin();
	for (; it != m_DynModelTab.end(); ++it)
	{
		CECMatter* pNode = *it.value();

		if ((fFraction > 0.0f) && pNode
			&& !pNode->GetStopCollosionDetection()
			&& pNode->TraceWithBrush(pInfo) 
			&& (pInfo->fFraction < fFraction ) ) 
		{
			fFraction = pInfo->fFraction;
			bAllSolid = pInfo->bAllSolid;
			bStartSolid = pInfo->bStartSolid;
			iClipPlane = pInfo->iClipPlane;
			vNormal = pInfo->ClipPlane.GetNormal();
			fDist = pInfo->ClipPlane.GetDist();
			bCollide=true;
		}
	}	

	it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pNode = *it.value();

		if ((pNode->IsMine() || pNode->IsBattleFlag()) && 
			(fFraction > 0.0f) && pNode 
			&& !pNode->GetStopCollosionDetection()
			&& pNode->TraceWithBrush(pInfo) 
			&& (pInfo->fFraction < fFraction ) ) 
		{
			fFraction = pInfo->fFraction;
			bAllSolid = pInfo->bAllSolid;
			bStartSolid = pInfo->bStartSolid;
			iClipPlane = pInfo->iClipPlane;
			vNormal = pInfo->ClipPlane.GetNormal();
			fDist = pInfo->ClipPlane.GetDist();
			bCollide=true;
		}
	}		

	//set back
	pInfo->fFraction = fFraction;
	pInfo->bStartSolid = bStartSolid;
	pInfo->bAllSolid = bAllSolid;
	pInfo->iClipPlane = iClipPlane;
	pInfo->ClipPlane.SetNormal(vNormal);
	pInfo->ClipPlane.SetD(fDist);
	return bCollide;
}

void CECMatterMan::OnBattleFlagChange(const S2C::battle_flag_change* pInfo)
{
	CECMatter* pMatter = GetMatter(pInfo->matter_id);

	if (!pMatter || !pMatter->IsBattleFlag())
		return;

	pMatter->BattleFlagChange(pInfo->origin_state, pInfo->new_state);
}

int  CECMatterMan::MatterInRange(float fDist, bool bPickCheck)
{
	CECHostPlayer* pHost = m_pGameRun->GetHostPlayer();
	CECInventory* pPack = pHost->GetPack(IVTRTYPE_PACK);

	CMatterSelector * pSelector = (m_pGameRun->GetHelper()&& m_pGameRun->GetHelper()->IsRunning())?
								  (m_pGameRun->GetHelper()->GetMatterSelector()):(NULL);


	int count = 0;

	//	Look all matters and find out the nearest one
	MatterTable::iterator it = m_MatterTab.begin();
	for (; it != m_MatterTab.end(); ++it)
	{
		CECMatter* pMatter = *it.value();
		float fMatterDist = pMatter->GetDistToHost();
		if (pMatter->IsMine() || fMatterDist > fDist)
			continue;

		if (bPickCheck && !pHost->CanTakeItem(pMatter->GetTemplateID(), 1))
			continue;

		if (pSelector && !pSelector->IsTarget(pMatter))
		{
			continue;
		}
		++count;		
	}

	return count;
}