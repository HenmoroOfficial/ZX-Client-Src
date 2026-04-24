/*
 * FILE: EC_Team.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/13
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AArray.h"
#include "A3DVector.h"
#include "AAssist.h"
#include "gameclient.h"

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


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

class CECTeam;

namespace GNET
{
	class GetPlayerBriefInfo_Re;
}

namespace S2C
{
	struct cmd_team_member_data;
	struct cmd_icon_state_notify;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECTeamMember
//	
///////////////////////////////////////////////////////////////////////////

class CECTeamMember
{
public:		//	Types
		//	Buff info
	struct BUFFINFO
	{
		int		id;			//	Buff id
		int		iLevel;		//	Buff level
	};
public:		//	Constructor and Destructor

	CECTeamMember(CECTeam* pTeam, int cid);
	virtual ~CECTeamMember() {}

public:		//	Attributes

public:		//	Operations

	//	Get member name
	const ACHAR* GetName();

	//	Is all information ready ?
	bool IsInfoReady() { return m_bInfoReady; }
	void SetInfoReadyFlag(bool bTrue) { m_bInfoReady = bTrue; }

	//	Set / Get properties
	int GetCharacterID() { return m_cid; }
	void SetLevel(int iLevel) { m_iLevel = iLevel; }
	int GetLevel() { return m_iLevel; }
	void SetDeityLevel(int iLevel) { m_iDeityLevel = iLevel; }
	int GetDeityLevel() { return m_iDeityLevel; }
	void SetPKLev(int iLev) { m_iPKLev = iLev; }
	int GetPKLev() { return m_iPKLev; }
	void SetWallowLev(int iLev) { m_iWallowLev = iLev; }
	int GetWallowLev() { return m_iWallowLev; }
	void SetCurHP(int iHP) { m_iCurHP = iHP; }
	int GetCurHP() { return m_iCurHP; }
	void SetCurMP(int iMP) { m_iCurMP = iMP; }
	int GetCurMP() { return m_iCurMP; }
	void SetCurDP(int iDP) { m_iCurDP = iDP; }
	int GetCurDP() { return m_iCurDP; }
	void SetCurRage(int iRage) { m_iCurRage = iRage; }
	int GetCurRage() { return m_iCurRage; }
	void SetMaxHP(int iHP) { m_iMaxHP = iHP; }
	int GetMaxHP() { return m_iMaxHP; }
	void SetMaxMP(int iMP) { m_iMaxMP = iMP; }
	int GetMaxMP() { return m_iMaxMP; }
	void SetMaxDP(int iDP) { m_iMaxDP = iDP; }
	int GetMaxDP() { return m_iMaxDP; }
	void SetPos(int idInst, const A3DVECTOR3& vPos) { m_idInst = idInst; m_vPos = vPos; }
	const A3DVECTOR3& GetPos() { return m_vPos; }
	int GetInstanceID() { return m_idInst; }
	void SetProfession(int iProfession) { m_iProfession = iProfession; }
	int GetProfession() { return m_iProfession; }
	bool IsFighting() { return m_bFight; }
	void SetFightFlag(bool bTrue) { m_bFight = bTrue; }
	void SetGender(int iGender) { m_iGender = iGender; }
	int GetGender() { return m_iGender; }
	void SetFamilyId(int id) { m_nFamilyId = id; }
	int GetFamilyId() const { return m_nFamilyId; }
	void SetMasterId(int id) { m_nMasterId = id; }
	int GetMasterid() const { return m_nMasterId; }
	void SetRebornCnt(int i) { m_iRebornCnt = i; }
	int GetRebornCnt() const { return m_iRebornCnt; }

	//	Reset icon states
	void ResetIconStates(const S2C::cmd_icon_state_notify& cmd);
	//	Get icon state number
	int GetIconStateNum() { return m_aIconStates.GetSize(); }
	//	Get icon state
	BUFFINFO& GetIconState(int n) { return m_aIconStates[n]; }

	bool marshal(GNET::Marshal::OctetsStream& os);
	bool unmarshal(GNET::Marshal::OctetsStream& os);

protected:	//	Attributes

	bool		m_bInfoReady;	//	true, this member's all info is ready

	CECTeam*	m_pTeam;		//	Team object
	int			m_cid;			//	Character ID
	int			m_iLevel;
	int			m_iDeityLevel;
	int			m_iPKLev;
	int			m_iWallowLev;
	int			m_iCurHP;
	int			m_iCurMP;
	int			m_iCurDP;
	int			m_iCurRage;
	int			m_iMaxHP;
	int			m_iMaxMP;
	int			m_iMaxDP;
	bool		m_bFight;		//	true, in fight state

	int			m_idInst;		//	ID of instance
	A3DVECTOR3	m_vPos;			//	Position
	int			m_iProfession;	//	Profession
	int			m_iGender;		//	Gender
	int			m_nFamilyId;
	int			m_nMasterId;
	int			m_iRebornCnt;

	AArray<BUFFINFO>	m_aIconStates;	//	Icon states (un-visible extend states)

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECTeam
//	
///////////////////////////////////////////////////////////////////////////

class CECTeam
{
public:		//	Types

public:		//	Constructor and Destructor

	CECTeam(int idLeader);
	virtual ~CECTeam();

public:		//	Attributes

public:		//	Operations

	//	Add a new member
	void AddMember(int idMember);
	//	Remove member
	void RemoveMember(int idMember);

	//	Get member number
	int GetMemberNum() { return m_aMembers.GetSize(); }
	//	Get member by player ID
	CECTeamMember* GetMemberByID(int id, int* piIndex=NULL);
	//	Get member by index
	CECTeamMember* GetMemberByIndex(int n)
	{
		if (n >= 0 && n < m_aMembers.GetSize())
			return m_aMembers[n]; 
		else
			return NULL;
	}
	//	Update team data
	void UpdateTeamData(const S2C::cmd_team_member_data& Data);
	//	Update team member information
	void UpdateMemberInfo(GNET::GetPlayerBriefInfo_Re& p);

	//	Get team leader's ID
	int GetLeaderID() { return m_idLeader; }
	//	Set team leader's ID
	void SetLeaderID(int idLeader) { m_idLeader = idLeader; }
	//	Get team leader
	CECTeamMember* GetLeader();
	//	Set pickup flag
	void SetPickupFlag(WORD wFlag) { m_wPickFlag = wFlag; }
	//	Get pickup flag
	WORD GetPickupFlag() { return m_wPickFlag; }

	//	Add ID of team member whose name is still unknown 
	void AddUnknownID(int id) { m_aUnknownIDs.UniquelyAdd(id); }

	bool marshal(GNET::Marshal::OctetsStream& os);
	bool unmarshal(GNET::Marshal::OctetsStream& os);

protected:	//	Attributes

	int		m_idLeader;		//	ID of team leader
	WORD	m_wPickFlag;	//	Pickup flag

	APtrArray<CECTeamMember*>	m_aMembers;
	AArray<int, int>			m_aUnknownIDs;	//	ID of players we only known whose ID but not name

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


