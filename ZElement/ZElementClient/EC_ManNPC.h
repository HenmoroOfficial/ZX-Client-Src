/*
 * FILE: EC_ManNPC.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_Counter.h"
#include "hashtab.h"
#include "AArray.h"
#include "aabbcd.h"
#include "A3DEngine.h"
#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

typedef abase::hashtab<int, ACString, abase::_hash_function> NPCName2IDTab;//name to nid

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECModel;
class A3DSkin;
class CECNPC;
class CECNPCCarrier;

#ifndef BACK_VERSION
#define CECViewport ECViewport
#endif

using namespace CHBasedCD;

//	NPC laod result structure used by CECNPC::LoadNPCModel
struct EC_NPCLOADRESULT
{
	CECModel*	pModel;
	A3DSkin*	pSkin;
	A3DSkin*	pLowSkin;
};

namespace S2C
{
	struct info_npc;
}

namespace GNET
{
	class Octets;
	class GamedataSend;
}

struct NPC_Reputation
{
	int type;
	int reputationvalue1;
	int reputationvalue2;
	int words1;
	int words2;
	AString action1;
	AString action2;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECNPCMan
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
class CECNPCMan : public CECManager
#else
class CECNPCMan : public ECManager
#endif
{
public:		//	Types

	//	Loaded NPC model information
	struct NPCMODEL
	{
		int	nid;	//	NPC's id
		DWORD dwBornStamp;
		EC_NPCLOADRESULT Ret;	//	Model loaded result
	};

	typedef abase::hashtab<CECNPC*, int, abase::_hash_function>	NPCTable;
	typedef abase::hashtab<int, int, abase::_hash_function> UkNPCTable;

public:		//	Constructor and Destructor

	CECNPCMan(CECGameRun* pGameRun);
	virtual ~CECNPCMan();

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport);
	//	RenderForReflected routine
	virtual bool RenderForReflect(CECViewport * pViewport);
	//	RenderForRefract routine
	virtual bool RenderForRefract(CECViewport * pViewport);
#ifndef BACK_VERSION
	//  RenderForShadow routine
	virtual bool RenderForShadow(ECViewport* pViewport);
	virtual bool RenderPost(ECViewport* pViewport) { return true; }
#endif
	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	On entering game world
	virtual bool OnEnterGameWorld();
	//	On leaving game world
	virtual bool OnLeaveGameWorld();
	
	//	Ray trace
	virtual bool RayTrace(ECRAYTRACE* pTraceInfo);
	// Trace for CD, if bOnlyCarrier = true, only trace carrier NPCs
	bool TraceWithBrush(BrushTraceInfo * pInfo, bool bOnlyCarrier = false);

	//	Tick carriers
	bool TickCarriers(DWORD dwDeltaTime);

	//	Get NPC of specified ID
	CECNPC* GetNPC(int nid, DWORD dwBornStamp=0);
	CECNPC* GetNPCFromAll(int nid);
	//  Get NPC of specified template ID
	CECNPC* GetNPCByTemplateID(int tid);
	//	Get NPC's name
	const wchar_t* GetNPCName(int nid);
	//	Get NPC number
	int GetNPCNum() { return (int)m_NPCTab.size(); }
	//	Get NPCs should appear in mini-map
	const APtrArray<CECNPC*>& GetNPCsInMiniMap() { return m_aMMNPCs; }
	//	When world's loading center changed
	void OnLoadCenterChanged();

	//	Auto-select a monster through 'Tab' key
	int TabSelectMonster(int idCurSel);

	//	Load NPC model in loading thread
	bool ThreadLoadNPCModel(int nid, DWORD dwBornStamp, int tid, const char* szFile);

	//	Calculate object's relative transform matrix to specified carrier
	bool CalcCarrierRelMat(int idCarrier, const A3DMATRIX4& matAbs, A3DMATRIX4& matRel);
	//	Get a value of a world direction transformed to a carrier coordinate
	bool GetCarrierRelDir(int idCarrier, const A3DVECTOR3& vAbsDir, A3DVECTOR3& vRelDir);
	//	Get a value of a world position transformed to a carrier coordinate
	bool GetCarrierRelPos(int idCarrier, const A3DVECTOR3& vAbsPos, A3DVECTOR3& vRelPos);
	//	Get object's pose on specified carrier
	bool GetPoseOnCarrier(int idCarrier, const A3DMATRIX4& matRel, A3DVECTOR3& vPos, A3DVECTOR3& vDir);	
	//	Check if a position in a specific valid carrier range
	bool InValidCarrierRange(int idCarrier, const A3DVECTOR3& vPos);
	
	GNET::GamedataSend gen_npc_list();
	
	int GetNPCIDbyName(const ACHAR* szName);
	
	void AddNPCNameAndID(int nid, const ACHAR* szName);
	
	void DelNPCNameAndID(int nid, const ACHAR* szName);
	
	// Get nearest npc of specified position
	int GetNearestEnemyNPC(const A3DVECTOR3& vTargetPos, float fRange, abase::vector<int>& vecTargets, float* fDistToTarget);
	
	bool HasSpecifiedSummonMonster(int tid);	// 检查HostPlayer是否有指定id的召唤兽
	abase::hash_map<int, NPC_Reputation>* GetNPCReputation() {return &m_npcReputation;}
	
	void SetNPCTeleprot(int tid, int iTeleportnum);	
protected:	//	Attributes

	NPCTable	m_NPCTab;		//	NPC table
	UkNPCTable	m_UkNPCTab;		//	Unknown NPC table
	CECCounter	m_QueryCnt;		//	Query unknown NPC time counter

	APtrArray<CECNPC*>			m_aMMNPCs;			//	NPCs who appear in mini-map
	APtrArray<NPCMODEL*>		m_aLoadedModels;	//	Loaded NPC models
	APtrArray<CECNPC*>			m_aDisappearNPCs;	//	NPCs who will disappear soon
	CRITICAL_SECTION			m_csLoad;			//	Loaded models lock

	APtrArray<CECNPC*>			m_aTabSels;			//	'Tab' key selected monsters

	APtrArray<CECNPCCarrier*>	m_aCarriers;		//	Carriers
    NPCName2IDTab           m_ServerNPCName2IDTab;
	
	abase::hash_map<int, NPC_Reputation> m_npcReputation;

protected:	//	Operations

	//	Create a NPC
	CECNPC* CreateNPC(const S2C::info_npc& Info, bool bBornInSight);
	//	Release a NPC
	void ReleaseNPC(CECNPC* pNPC);
	//	Put NPC into disappear table
	void NPCDisappear(int nid, bool bAtOnce);
	//	One NPC enter view area
	bool NPCEnter(const S2C::info_npc& Info, bool bBornInSight);
	//	One NPC leave view area
	void NPCLeave(int nid, bool bUpdateMMArray=true, bool bRelease=true);
	//	Update NPCs in various ranges (Active, visible, mini-map etc.)
	void UpdateNPCInRanges(DWORD dwDeltaTime);
	//	Remove NPC from m_aMMNPCs array
	void RemoveNPCFromMiniMap(CECNPC* pNPC);
	//	Seek out NPC with specified id, if NPC doesn't exist, try to get from server
	CECNPC* SeekOutNPC(int nid);
	//	Update unknown NPC table
	void UpdateUnknownNPCs();
	//	Remove npc from tab-selected array
	void RemoveNPCFromTabSels(CECNPC* pNPC);

	//	Deliver loaded NPC models
	void DeliverLoadedNPCModels();
	bool InitNPCReputation(const char* szFile);
public:

	//	Messages handlers
	bool OnMsgNPCInfo(const ECMSG& Msg);
	bool OnMsgNPCMove(const ECMSG& Msg);
	bool OnMsgNPCRunOut(const ECMSG& Msg);
	bool OnMsgNPCDied(const ECMSG& Msg);
	bool OnMsgNPCDisappear(const ECMSG& Msg);
	bool OnMsgNPCOutOfView(const ECMSG& Msg);
	bool OnMsgNPCStopMove(const ECMSG& Msg);
	bool OnMsgInvalidObject(const ECMSG& Msg);

	//	Transmit message
	bool TransmitMessage(const ECMSG& Msg);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

