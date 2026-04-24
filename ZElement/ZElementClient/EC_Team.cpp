/*
 * FILE: EC_Team.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/13
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Team.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_Utility.h"
#include "EC_FixedMsg.h"
#include "EC_GPDataType.h"
#include "EC_Player.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_Friend.h"

//	#include "gnet.h"
#include "getplayerbriefinfo_re.hpp"

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
//	Implement CECTeamMember
//	
///////////////////////////////////////////////////////////////////////////

CECTeamMember::CECTeamMember(CECTeam* pTeam, int cid) : m_vPos(INVALIDPOS)
{
	m_bInfoReady	= false;
	m_pTeam			= pTeam;
	m_cid			= cid;
	m_iLevel		= 0;
	m_iDeityLevel	= 0;
	m_iCurHP		= 0;
	m_iCurMP		= 0;
	m_iCurDP		= 0;
	m_iCurRage		= 0;
	m_iMaxHP		= 0;
	m_iMaxMP		= 0;
	m_iMaxDP		= 0;
	m_bFight		= false;
	m_idInst		= 0;
	m_nFamilyId		= 0;
	m_nMasterId		= 0;
	
	m_iProfession	= PROF_1;
	m_iGender		= GENDER_MALE;
}

//	Get member name
const ACHAR* CECTeamMember::GetName()
{
	return g_pGame->GetGameRun()->GetPlayerName(m_cid, true);
}

//	Reset icon states
void CECTeamMember::ResetIconStates(const S2C::cmd_icon_state_notify& cmd)
{
	m_aIconStates.RemoveAll(false);

	if (cmd.count)
	{
		m_aIconStates.SetSize(cmd.count, 10);
		for (int i=0; i < cmd.count; i++)
		{
			BUFFINFO& bi = m_aIconStates[i];
			bi.id		= cmd.buff_list[i].state;
			bi.iLevel	= cmd.buff_list[i].level;
		}
		//memcpy(m_aIconStates.GetData(), aStates, iNum * sizeof (WORD));

		//	Sort icons
		//qsort(m_aIconStates.GetData(), m_aIconStates.GetSize(), sizeof (WORD), glb_WordCompare);
	}
}

bool CECTeamMember::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		char ready = m_bInfoReady ? 1 : 0;
		os << ready;

		if (m_bInfoReady)
		{
			os << m_cid;
			os << m_iLevel;
			os << m_iDeityLevel;
			os << m_iCurHP;
			os << m_iCurMP;
			os << m_iCurDP;
			os << m_iMaxHP;
			os << m_iMaxMP;
			os << m_iMaxDP;
			os << m_bFight;
			os << m_idInst;
			os << m_vPos.x;
			os << m_vPos.y;
			os << m_vPos.z;
			os << m_iProfession;
			os << m_iGender;
			os << m_nFamilyId;
			os << m_nMasterId;
			os << m_aIconStates.GetSize();

			for (int i = 0; i < m_aIconStates.GetSize(); i++)
			{
				os << m_aIconStates[i].id;
				os << m_aIconStates[i].iLevel;
			}		
		}
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

bool CECTeamMember::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		char ready;
		os >> ready;
		m_bInfoReady = (ready != 0);

		if (m_bInfoReady)
		{
			os >> m_cid;
			os >> m_iLevel;
			os >> m_iDeityLevel;
			os >> m_iCurHP;
			os >> m_iCurMP;
			os >> m_iCurDP;
			os >> m_iMaxHP;
			os >> m_iMaxMP;
			os >> m_iMaxDP;
			os >> m_bFight;
			os >> m_idInst;
			os >> m_vPos.x;
			os >> m_vPos.y;
			os >> m_vPos.z;
			os >> m_iProfession;
			os >> m_iGender;
			os >> m_nFamilyId;
			os >> m_nMasterId;
			int count;
			BUFFINFO state;
			os >> count;

			for (int i = 0; i < count; i++)
			{
				os >> state.id;
				os >> state.iLevel;
				m_aIconStates.Add(state);
			}
		}
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
 }

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECTeam
//	
///////////////////////////////////////////////////////////////////////////

CECTeam::CECTeam(int idLeader)
{
	m_idLeader	= idLeader;
	m_wPickFlag	= 0;
}

CECTeam::~CECTeam()
{
	//	Release all members
	for (int i = 0; i < m_aMembers.GetSize(); i++)
		delete m_aMembers[i];
}

//	Get member by player ID
CECTeamMember* CECTeam::GetMemberByID(int id, int* piIndex/* NULL */)
{
	for (int i=0; i < m_aMembers.GetSize(); i++)
	{
		CECTeamMember* pMember = m_aMembers[i];
		if (pMember->GetCharacterID() == id)
		{
			if (piIndex)
				*piIndex = i;

			return pMember;
		}
	}

	return NULL;
}

//	Update team data
void CECTeam::UpdateTeamData(const S2C::cmd_team_member_data& Data)
{
	using namespace S2C;

	int idHost = g_pGame->GetGameSession()->GetCharacterID();
	int i, iIndex;

	if (Data.member_count == Data.data_count)
	{
		//	Reset all members' information
		APtrArray<CECTeamMember*> aNewMembers;
		aNewMembers.SetSize(Data.data_count, 10);

		for (i=0; i < aNewMembers.GetSize(); i++)
			aNewMembers[i] = NULL;

		for (i=0; i < Data.data_count; i++)
		{
			const cmd_team_member_data::MEMBER& Member = Data.data[i];
			CECTeamMember* pMember = GetMemberByID(Member.idMember, &iIndex);
			if (!pMember)
			{
				//	Create new member
				if (!(pMember = new CECTeamMember(this, Member.idMember)))
					continue;

				// 将组队好友加入到临时好友列表
				const ACHAR *pTeamMateName = g_pGame->GetGameRun()->GetPlayerName(Member.idMember, false);
	            if (pTeamMateName)
				{
					CECFriendMan* pFriendMan =  g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
					pFriendMan->AddTempFriend(Member.idMember, pTeamMateName);
				}
			}

			aNewMembers[i] = pMember;

			pMember->SetCurHP(Member.hp);
			pMember->SetCurMP(Member.mp);
			pMember->SetCurDP(Member.dp);
			pMember->SetCurRage(Member.rage);
			pMember->SetLevel(Member.level);
			pMember->SetDeityLevel(Member.deityLevel);
			pMember->SetPKLev(Member.pk_level);
			pMember->SetWallowLev(Member.wallow_level);
			pMember->SetProfession(Member.class_id);
			pMember->SetMaxHP(Member.max_hp);
			pMember->SetMaxMP(Member.max_mp);
			pMember->SetMaxDP(Member.max_dp);
			pMember->SetFightFlag(Member.state ? true : false);
			pMember->SetFamilyId(Member.family_id);
			pMember->SetMasterId(Member.master_id);
			pMember->SetRebornCnt(Member.reborn_cnt);

			CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(Member.idMember);

			if (pPlayer)
				pPlayer->SetProfession(Member.class_id);
		}

		//	Reset player array
		for (i=0; i < m_aMembers.GetSize(); i++)
		{
			if (aNewMembers.Find(m_aMembers[i]) < 0)
				delete m_aMembers[i];
		}

		m_aMembers.RemoveAll(false);

		for (i=0; i < aNewMembers.GetSize(); i++)
			m_aMembers.Add(aNewMembers[i]);
	}
	else
	{
		for (i=0; i < Data.data_count; i++)
		{
			const cmd_team_member_data::MEMBER& Member = Data.data[i];
			CECTeamMember* pMember = GetMemberByID(Member.idMember, &iIndex);
			if (!pMember)
			{
				//	Create new member
				if (!(pMember = new CECTeamMember(this, Member.idMember)))
					continue;

				m_aMembers.Add(pMember);

				// 将组队好友加入到临时好友列表
				const ACHAR *pTeamMateName = g_pGame->GetGameRun()->GetPlayerName(Member.idMember, false);
	            if (pTeamMateName)
				{
					CECFriendMan* pFriendMan =  g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
					pFriendMan->AddTempFriend(Member.idMember, pTeamMateName);
				}
			}

			pMember->SetCurHP(Member.hp);
			pMember->SetCurMP(Member.mp);
			pMember->SetCurDP(Member.dp);
			pMember->SetCurRage(Member.rage);
			pMember->SetLevel(Member.level);
			pMember->SetDeityLevel(Member.deityLevel);
			pMember->SetPKLev(Member.pk_level);
			pMember->SetWallowLev(Member.wallow_level);
			pMember->SetProfession(Member.class_id);
			pMember->SetMaxHP(Member.max_hp);
			pMember->SetMaxMP(Member.max_mp);
			pMember->SetMaxDP(Member.max_dp);
			pMember->SetFightFlag(Member.state ? true : false);
			pMember->SetFamilyId(Member.family_id);
			pMember->SetMasterId(Member.master_id);
			pMember->SetRebornCnt(Member.reborn_cnt);

			CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(Member.idMember);

			if (pPlayer)
				pPlayer->SetProfession(Member.class_id);
		}
	}
}

//	Add a new member
void CECTeam::AddMember(int idMember)
{
	if (GetMemberByID(idMember))
	{
		//	Team member has been exist
		ASSERT(0);
		return;
	}

	CECTeamMember* pMember = new CECTeamMember(this, idMember);
	if (!pMember)
		return;

	// 将组队好友加入到临时好友列表
	const ACHAR *pTeamMateName = g_pGame->GetGameRun()->GetPlayerName(idMember, false);
	if (pTeamMateName)
	{
		CECFriendMan* pFriendMan =  g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan();
		pFriendMan->AddTempFriend(idMember, pTeamMateName);
	}
	
	// 有时候服务器不更新hostplayer信息，这儿暂时这么写，如果服务器保证更新，这个就不需要了
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (idMember==pHost->GetCharacterID())
	{
		pMember->SetProfession(pHost->GetProfession());
		pMember->SetRebornCnt(pHost->GetRebornCount());
	}

	m_aMembers.Add(pMember);
}

//	Remove member
void CECTeam::RemoveMember(int idMember)
{
	int iIndex;
	CECTeamMember* pMember = GetMemberByID(idMember, &iIndex);
	if (pMember)
	{
		delete pMember;
		m_aMembers.RemoveAt(iIndex);
	}
}

//	Update team member information
void CECTeam::UpdateMemberInfo(GNET::GetPlayerBriefInfo_Re& p)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	//	Update host's team member information
	for (int i=0; i < (int)p.playerlist.size(); i++)
	{
		const PlayerBriefInfo& Info = p.playerlist[i];
		CECTeamMember* pMember = GetMemberByID(Info.roleid);
		if (!pMember)
			continue;
		
		pMember->SetGender(Info.gender);
		pMember->SetInfoReadyFlag(true);
	
		//	Name-id pair got here always be right, so we force to
		//	overwrite old pair
		ACString strName((const ACHAR*)Info.name.begin(), Info.name.size() / sizeof (ACHAR));
		pGameRun->AddPlayerName(Info.roleid, strName, true);
			
		//	Remove player from unknown ID list
		for (int j=0; j < m_aUnknownIDs.GetSize(); j++)
		{
			if (m_aUnknownIDs[j] == Info.roleid)
			{
				m_aUnknownIDs.RemoveAtQuickly(j);
				pGameRun->AddFixedMessage(FIXMSG_JOINTEAM, strName);
				break;
			}
		}
	}
}

//	Get team leader
CECTeamMember* CECTeam::GetLeader()
{
	if (!m_aMembers.GetSize())
		return NULL;
	
	for (int i = 0; i < m_aMembers.GetSize(); i++)
	{
		CECTeamMember* pMember = m_aMembers[i];
		if (pMember && m_idLeader == pMember->GetCharacterID())
			return pMember;
	}
	
	return NULL;
}

bool CECTeam::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << m_idLeader;
		os << m_wPickFlag;
		os << m_aMembers.GetSize();

		for (int i = 0; i < m_aMembers.GetSize(); i++)
			m_aMembers[i]->marshal(os);
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

bool CECTeam::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os >> m_idLeader;
		os >> m_wPickFlag;
		int count;
		os >> count;

		for (int i = 0; i < count; i++)
		{
			CECTeamMember* pMem = new CECTeamMember(this, 0);
			pMem->unmarshal(os);
			m_aMembers.Add(pMem);
		}
	}
	catch (Marshal::Exception &)
	{
		for (int i = 0; i < m_aMembers.GetSize(); i++)
			delete m_aMembers[i];

		m_aMembers.RemoveAll();
		return false;
	}

	return true;
 }
