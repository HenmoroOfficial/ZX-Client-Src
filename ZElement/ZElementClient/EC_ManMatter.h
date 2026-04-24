/*
 * FILE: EC_ManMatter.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/9
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
#include "AArray.h"
#include "hashtab.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECMatter;
class CECModel;

namespace CHBasedCD
{
	class BrushTraceInfo;
}

namespace S2C
{
	struct info_matter;
	struct battle_flag_change;
}

namespace GNET
{
	class Octets;
	class GamedataSend;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECMatterMan
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
class CECMatterMan : public CECManager
#else
class CECMatterMan : public ECManager
#endif
{
public:		//	Types

	//	Loaded matter model
	struct MATTERMODEL
	{
		int			mid;		//	Matter ID
		CECModel*	pModel;		//	Matter's model object
	};
	
	typedef abase::hashtab<CECMatter*, int, abase::_hash_function>	MatterTable;

public:		//	Constructor and Destructor

	CECMatterMan(CECGameRun* pGameRun);
	virtual ~CECMatterMan();

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
#ifdef BACK_VERSION
	virtual bool Render(CECViewport* pViewport);
#else
	virtual bool Render(ECViewport* pViewport);
	virtual bool RenderPost(ECViewport* pViewport) { return true; }
	//	RenderForReflected routine
	virtual bool RenderForReflect(ECViewport * pViewport);
	//	RenderForRefract routine
	virtual bool RenderForRefract(ECViewport * pViewport);
	//  RenderForShadow routine
	virtual bool RenderForShadow(ECViewport* pViewport);
#endif

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	On entering game world
	virtual bool OnEnterGameWorld();
	//	On leaving game world
	virtual bool OnLeaveGameWorld();

	//	Ray trace
	virtual bool RayTrace(ECRAYTRACE* pTraceInfo);

	//	Get matter throuth it's ID
	CECMatter* GetMatter(int mid, DWORD dwBornStamp=0);
	//	Find a matter which is near enough to host player
	CECMatter* FindMatterNearHost(float fDist, bool bPickCheck);
	int        MatterInRange(float fDist, bool bPickCheck);

	//	When world's loading center changed
	void OnLoadCenterChanged();
	//	Remove a matter
	void RemoveMatter(int idMatter) { MatterLeave(idMatter); }
	//	Get matter number
	int GetMatterNum() { return (int)m_MatterTab.size(); }

	//	Load matter model in loading thread
	bool ThreadLoadMatterModel(int mid, const char* szFile);
	
	GNET::GamedataSend gen_matter_list();

#ifndef BACK_VERSION
	//	Internal Render
	bool InternalRender(ECViewport* pViewport, int nRenderFlag);
#endif
protected:	//	Attributes

	MatterTable		m_MatterTab;	//	Matter table
	MatterTable		m_DynModelTab;

	AArray<MATTERMODEL, MATTERMODEL&>	m_aLoadedMats;	//	Loaded matter models
	CRITICAL_SECTION					m_csLoad;		//	m_aLoadedMats lock

protected:	//	Operations

	//	Create a matter
	CECMatter* CreateMatter(const S2C::info_matter& Info);
	//	Release a matter
	void ReleaseMatter(CECMatter* pMatter);
	//	One matter enter view area
	bool MatterEnter(const S2C::info_matter& Info);
	//	One matter leave view area
	void MatterLeave(int mid);
	//	Update matters in various ranges (Active, visible, etc.)
	void UpdateMatterInRanges(DWORD dwDeltaTime);
	//	Combine matter change state.
	void MatterChangeState();

	//	Deliver loaded matter models
	void DeliverLoadedMatterModels();
	
public:

	bool TraceWithBrush(CHBasedCD::BrushTraceInfo * pInfo);
	void OnBattleFlagChange(const S2C::battle_flag_change* pInfo);

	//	Messages handlers
	bool OnMsgMatterInfo(const ECMSG& Msg);
	bool OnMsgMatterDisappear(const ECMSG& Msg);
	bool OnMsgMatterEnterWorld(const ECMSG& Msg);
	bool OnMsgMatterOutOfView(const ECMSG& Msg);
	bool OnMsgInvalidObject(const ECMSG& Msg);
	bool OnMsgCombineMineStateChange(const ECMSG& Msg);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

