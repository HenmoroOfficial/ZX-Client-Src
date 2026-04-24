#include "EC_Global.h"
#include "factioncreate_re.hpp"
#include "factionexpel_re.hpp"
#include "factionresign_re.hpp"
#include "factionappoint_re.hpp"
#include "factionleave_re.hpp"
#include "factionupgrade_re.hpp"
#include "factionlist_re.hpp"
#include "getfactionbaseinfo_re.hpp"
#include "factiondismiss_re.hpp"
#include "./network/rpcdata/factioninvitearg"
#include "FactionNickname_Re.hpp"
#include "factionchat.hpp"
#include "factioninvite.hrp"
#include "combatinvite.hrp"
#include "factionrecruit_re.hpp"
#include "factionannounce_re.hpp"
#include "familyrecord.hpp"
#include "territoryscoreupdate.hpp"
#include "changefactionname_re.hpp"
#include "gnetdef.h"
#include "ids.hxx"
#include "crssvrteamsinvite.hrp"

#include "EC_Faction.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_FixedMsg.h"
#include "EC_ElsePlayer.h"
#include "EC_ManPlayer.h"
#include "DlgGuildMan.h"
#include "AUI\\AUICommon.h"
#include "EC_GPDataType.h"
#include "EC_IvtrItem.h"
#include "EC_Achievement.h"

#define sizeof_arr(a) (sizeof(a) / sizeof((a)[0]))

const ACHAR* Faction_Mem_Info::GetName() const
{
	const ACHAR* szName = g_pGame->GetGameRun()->GetPlayerName(roleid, false);
	return szName ? szName : _AL("");
}

void Faction_Mem_Info::SetName(const void* s, int len)
{
	ACString name;
	_cp_str(name, s, len);
	SetName(name);
}

void Faction_Mem_Info::SetName(const ACHAR* szName)
{
	g_pGame->GetGameRun()->AddPlayerName(roleid, szName);
}

bool Faction_Info::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << m_id;
		GNET::Octets o((const ACHAR*)m_szName, (m_szName.GetLength() + 1) * sizeof(ACHAR));
		os << o;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

bool Faction_Info::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os >> m_id;
		GNET::Octets o;
		os >> o;
		m_szName = (const ACHAR*)o.begin();

		m_nLev = 0;
		m_nMemNum = 0;
		m_nOnlineMemNum = 0;
		m_nProsperity = 0;
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

const ACHAR* CECFactionMan::GetFRoleName(unsigned int iFRoleId)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	return pGameUI->GetStringFromTable(1300+iFRoleId);
}

void CECFactionMan::Release(bool bOnlyFreeMember)
{
	if (!bOnlyFreeMember)
	{
		FactionInfoMap::iterator it = m_FactionMap.begin();

		for (; it != m_FactionMap.end(); ++it)
			delete it->second;

		m_FactionMap.clear();
		it  = m_FamilyMap.begin();

		for (; it != m_FamilyMap.end(); ++it)
			delete it->second;

		m_FamilyMap.clear();
	}

	ResetMemInfoList();
	m_FamilyList.clear();
	m_FamilySkillList.clear();
	m_strProclaim.Empty();
	m_strFamilyProclaim.Empty();
	m_nDeleteTime = 0;
	m_nFamilyDeleteTime = 0;
	memset(m_FamilyRecords, 0, sizeof(m_FamilyRecords));
}

Faction_Info* CECFactionMan::GetFaction(unsigned int uId, bool bRequestFromServer, bool bFamily)
{
	FactionInfoMap& map = bFamily ? m_FamilyMap : m_FactionMap;
	FactionInfoMap::iterator it = map.find(uId);

	if (it == map.end())
	{
		if (bRequestFromServer)
			g_pGame->GetGameSession()->GetFactionInfo(1, reinterpret_cast<int*>(&uId), bFamily ? 1 : 0);

		return NULL;
	}

	return it->second;
}

inline void CECFactionMan::AddFixedMsg(int iMsg)
{
#define MSG_CASE(msg) \
{\
	case msg:\
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_##msg);\
		break;\
}

	switch (iMsg)
	{
	MSG_CASE(ERR_FC_NETWORKERR)
	MSG_CASE(ERR_FC_INVALID_OPERATION)
    MSG_CASE(ERR_FC_OP_TIMEOUT)
    MSG_CASE(ERR_FC_CREATE_ALREADY)
    MSG_CASE(ERR_FC_CREATE_DUP)
    MSG_CASE(ERR_FC_DBFAILURE)
    MSG_CASE(ERR_FC_NO_PRIVILEGE)
    MSG_CASE(ERR_FC_INVALIDNAME)
    MSG_CASE(ERR_FC_FULL)
    MSG_CASE(ERR_FC_APPLY_REJOIN)
    MSG_CASE(ERR_FC_JOIN_SUCCESS)
    MSG_CASE(ERR_FC_ACCEPT_REACCEPT)
    MSG_CASE(ERR_FC_FACTION_NOTEXIST)
    MSG_CASE(ERR_FC_NOTAMEMBER)
    MSG_CASE(ERR_FC_CHECKCONDITION)
    MSG_CASE(ERR_FC_DATAERROR)
	MSG_CASE(ERR_FC_OFFLINE)
	MSG_CASE(ERR_FC_INVITEELEVEL)
	MSG_CASE(ERR_FC_DISMISSWAITING)
	MSG_CASE(ERR_FC_INVITEENOFAMILY)
	MSG_CASE(ERR_FC_LEAVINGFAMILY)
	MSG_CASE(ERR_FC_CHANGENAME_CD)
	MSG_CASE(ERR_FC_CHANGEKINGDOM)
	MSG_CASE(ERR_FC_RECRUIT_FAMILY_CD)
	}
}

void CECFactionMan::ClearHostFaction(bool family)
{
	Release(true);
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (!pGameUI)
		return;

	if (family)
	{
		((CDlgGuildMan*)pGameUI->GetDialog("Win_FamilyManage"))->SetFirstOpen(false);
		((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->SetFirstOpen(false);
	}
	else
		((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->SetFirstOpen(false);
}

DWORD CECFactionMan::OnChat(const FactionChat* p)
{
	using namespace CHAT_S2C;
	const ACHAR* szName = GetMemName(p->src);
	CECIvtrItem* pIvtrItem = NULL;
	sAchievement* pAchievement = NULL;

	chat_equip_item* pItemInfo = (chat_equip_item*)p->data.begin();
	chat_achievement* pAchievementInfo = (chat_achievement*)p->data.begin();

	if (pItemInfo && pItemInfo->cmd_id == CHAT_EQUIP_ITEM)
	{
		const size_t sz = sizeof(chat_equip_item) - sizeof(pItemInfo->content);
		if (p->data.size() >= sz && sz + pItemInfo->content_length == p->data.size())
		{
			pIvtrItem = CECIvtrItem::CreateItem(pItemInfo->type, pItemInfo->expire_date, 1);
		
			if (pIvtrItem)
			{
				pIvtrItem->SetItemInfo((unsigned char*)pItemInfo->content, pItemInfo->content_length);
				pIvtrItem->SetStateFlag(pItemInfo->state);
				pIvtrItem->UpdateInfoMD5();
			}
		}
	}
	else if(pAchievementInfo && pAchievementInfo->cmd_id == CHAT_ACHIEVEMENT)
	{
		const size_t sz = sizeof(chat_achievement) - sizeof(pAchievementInfo->player_name);
		if (p->data.size() >= sz && sz + pAchievementInfo->player_name_len == p->data.size())
		{
			CECAchievementMan* pMan = g_pGame->GetGameRun()->GetHostPlayer()->GetAchievementMan();
			
			if (pMan->GetAchievement(pAchievementInfo->achievement_id))
			{
				pAchievement = new sAchievement(*pMan->GetAchievement(pAchievementInfo->achievement_id));
				pAchievement->iFinishTime = pAchievementInfo->finish_time;
				ACString playerName((ACHAR*)pAchievementInfo->player_name, pAchievementInfo->player_name_len/2);
				pAchievement->strOwnerName = playerName;
				pAchievement->SetFinish(pAchievement->iFinishTime != 0);
			}
		}
	}

	if (szName)
	{
		ACString strMsg;
		_cp_str(strMsg, p->msg.begin(), p->msg.size());
		
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if( pGameUI )
		{
			pGameUI->FilterBadWords(strMsg);
			pGameUI->FilterImages(strMsg);
		}
		if(p->src_route==FROM_GT)
		{
			EditBoxItemBase item(enumEIImage);
			item.SetImageIndex(CECGameUIMan::ICONLIST_GTICON);
			item.SetImageFrame(0);
			strMsg = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize() + ACString(_AL(" ")) + strMsg;
		}		

		ACHAR szText[80], szText1[1024];
		AUI_ConvertChatString(szName, szText);
		AUI_ConvertChatString(strMsg, szText1, false);

		ACString str;
		strMsg = szText1;
		ACHAR ch = 0xdbff;
		ACString strMark = ACString(ch,2);
		if((0 == strMsg.Find(_AL(":")+strMark)||0 == strMsg.Find(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8493)+strMark)) && 0 <= strMsg.Find(_AL("#N")))
		{
			strMsg = strMsg.Mid(3);
			int iLeft = strMsg.Find(_AL("#N"));
			while (iLeft>=0)
			{
				strMsg.Format(_AL("%s%s%s"), strMsg.Left(iLeft), szText, strMsg.Mid(iLeft+2));
				iLeft = strMsg.Find(_AL("#N"));
			}
			str.Format(_AL("%s%s"),g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(8494),strMsg);
		}
		else
		{
			str.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_CHAT), szText, szText1);
		}
		g_pGame->GetGameRun()->AddChatMessage(str, p->channel, p->src, szName, 0, p->emotion, pIvtrItem, pAchievement);
		return 0;
	}

	delete pIvtrItem;
	delete pAchievement;
	return FO_OPERATOR_UNK;
}

void CECFactionMan::OnFactionCreate(const FactionCreate_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		const Octets& o = p->factionname;
		g_pGame->GetFactionMan()->AddFaction(p->factionid, p->scale != 0, o.begin(), o.size(), 0, 0, 1);
		g_pGame->GetGameRun()->AddFixedMessage(p->scale ? FIXMSG_FAMILY_CREATE : FIXMSG_FC_CREATE);
	}
	else
	{
		if (p->retcode == GNET::ERR_DATABASE_TIMEOUT)
			g_pGame->GetGameSession()->OutputLinkSevError(GNET::ERR_DATABASE_TIMEOUT);
		else
			AddFixedMsg(p->retcode);
	}
}

DWORD CECFactionMan::OnAcceptJoin(const FactionRecruit_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		DWORD dwRet = 0;
		const ACHAR* szInviter = GetMemName(p->inviter);

		if (!szInviter)
			dwRet |= FO_OPERATOR_UNK;

		if (dwRet)
			return dwRet;

		ACString strInvitee;
		_cp_str(strInvitee, p->name.begin(), p->name.size());
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

		if (p->scale)
		{
			g_pGame->GetGameRun()->AddPlayerName(p->invitee, strInvitee);

			if (pHost->GetCharacterID() == p->invitee)
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_ACCEPT_SELF, szInviter);
			else
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_ACCEPT, szInviter, strInvitee);
		}
		else
		{
			if (!GetFaction(p->invitee, false, true))
				AddFaction(p->invitee, true, p->name.begin(), p->name.size(), 0, 0, 0);

			if (!HasFamily(p->invitee))
			{
				m_FamilyList.push_back(p->invitee);
				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->BuildFamilyList();
			}

			if (p->invitee == pHost->GetFamilyID())
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_ACCEPT_SELF, szInviter);
			else
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_ACCEPT_JOIN, szInviter, strInvitee);
		}
	}
	else if (p->retcode == ERR_FC_JOIN_REFUSE)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ERR_FC_JOIN_REFUSE);
	else
		AddFixedMsg(p->retcode);

	return 0;
}

bool CECFactionMan::RemoveMember(int iRoleId)
{
	bool bRet = false;

	for (size_t i = 0; i < m_MemList.size(); i++)
	{
		if (m_MemList[i]->GetID() == iRoleId)
		{
			m_MemList.erase(&m_MemList[i]);
			bRet = true;
			break;
		}
	}

	FactionMemMap::iterator it = m_MemMap.find(iRoleId);

	if (it != m_MemMap.end())
		m_MemMap.erase(it);

	return bRet;
}

bool CECFactionMan::RemoveFamily(int iFamily)
{
	for (size_t i = 0; i < m_FamilyList.size(); i++)
	{
		if ((int)m_FamilyList[i] == iFamily)
		{
			m_FamilyList.erase(m_FamilyList.begin()+i);
			return true;
		}
	}

	return false;
}

DWORD CECFactionMan::OnExpel(const FactionExpel_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

		if (p->scale)
		{
			if (pHost->GetCharacterID() == (int)p->expellee)
			{
				ClearHostFaction(true);
				const ACHAR* szName = GetMemName(p->roleid);

				if (szName == NULL)
					return FO_OPERATOR_UNK;

				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_EXPEL_SELF, szName);
			}
			else
			{
				DWORD dwRet = 0;
				const ACHAR* szOperator = GetMemName(p->roleid);

				if (!szOperator)
					dwRet |= FO_OPERATOR_UNK;

				if (dwRet)
					return dwRet;

				if (RemoveMember(p->expellee))
				{
					CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
					pGameUI->UpdateFactionList((int)p->familyid != pHost->GetFamilyID());
				}

				ACString strOperant;
				_cp_str(strOperant, p->name.begin(), p->name.size());
				g_pGame->GetGameRun()->AddPlayerName(p->expellee, strOperant);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_EXPEL, szOperator, strOperant);
			}
		}
		else
		{
			const ACHAR* szName = GetMemName(p->roleid);

			if (szName == NULL)
				return FO_OPERATOR_UNK;

			if (!GetFaction(p->familyid, false, true))
				AddFaction(p->familyid, true, p->name.begin(), p->name.size(), 0, 0, 0);

			if ((int)p->familyid == pHost->GetFamilyID())
			{
				ClearHostFaction(false);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_EXPEL_SELF, szName);
			}
			else
			{
				ACString strOperant;
				_cp_str(strOperant, p->name.begin(), p->name.size());
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_EXPEL, szName, strOperant);

				if (RemoveFamily(p->familyid))
				{
					CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
					((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->BuildFamilyList();
				}
			}
		}
	}
	else
		AddFixedMsg(p->retcode);

	return 0;
}

void CECFactionMan::OnBroadcastNotice(const FactionAnnounce_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		_cp_str(p->scale ? m_strFamilyProclaim : m_strProclaim, p->announce.begin(), p->announce.size());
		g_pGame->GetGameRun()->AddFixedMessage(p->scale ? FIXMSG_FAMILY_CHANGE_PROCLAIM : FIXMSG_FC_CHG_PROCLAIM);
	}
	else
		AddFixedMsg(p->retcode);
}

const ACHAR* CECFactionMan::GetMemName(int iRoleId)
{
	return g_pGame->GetGameRun()->GetPlayerName(iRoleId, false);
}

const ACHAR* CECFactionMan::GetMemNickName(int iRoleId)
{
	Faction_Mem_Info* pMem = GetMember(iRoleId);
	return pMem ? pMem->GetNickName() : NULL;
}

DWORD CECFactionMan::OnResign(const FactionResign_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		const ACHAR* szName = GetMemName(p->roleid);

		if (!szName)
			return FO_OPERANT_UNK;

		Faction_Mem_Info* pMem = GetMember(p->roleid);

		if (pMem)
			pMem->SetFRoleID(_R_HEADER);

		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_RESIGN, szName, GetFRoleName(p->title));
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->UpdateFactionList();
	}
	else
		AddFixedMsg(p->retcode);

	return 0;
}

DWORD CECFactionMan::OnAppoint(const FactionAppoint_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		DWORD dwRet = 0;
		const ACHAR* szOperator = GetMemName(p->roleid);

		if (!szOperator)
			dwRet |= FO_OPERATOR_UNK;

		const ACHAR* szOperant = GetMemName(p->candidate);

		if (!szOperant)
			dwRet |= FO_OPERANT_UNK;

		if (dwRet)
			return dwRet;

		Faction_Mem_Info* pMem = GetMember(p->candidate);
		bool bUpdate = false;

		if (pMem)
		{
			pMem->SetFRoleID(p->title);
			bUpdate = true;
		}

		if (p->scale || p->title == _R_MASTER)
		{
			pMem = GetMember(p->roleid);

			if (pMem)
			{
				pMem->SetFRoleID(p->scale ? _R_MEMBER : _R_HEADER);
				bUpdate = true;
			}
		}

		if (bUpdate)
		{
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->UpdateFactionList(p->scale == 0);
		}

		if (p->scale)
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_APPOINT, szOperator, szOperant, szOperant);
		else if (p->title == _R_MASTER)
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_APPOINT_MASTER, szOperator, szOperant, szOperant);
		else
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_APPOINT, szOperator, szOperant, GetFRoleName(p->title));
	}
	else
		AddFixedMsg(p->retcode);

	return 0;
}

DWORD CECFactionMan::OnLeave(const FactionLeave_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

		if (p->scale)
		{
			if (p->roleid == pHost->GetCharacterID())
			{
				ClearHostFaction(true);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_LEAVE_SELF);
			}
			else
			{
				if (RemoveMember(p->roleid))
				{
					CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
					pGameUI->UpdateFactionList(false);
				}
	
				ACString strOperant;
				_cp_str(strOperant, p->name.begin(), p->name.size());
				g_pGame->GetGameRun()->AddPlayerName(p->roleid, strOperant);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_LEAVE, strOperant);
			}
		}
		else
		{
			if (!GetFaction(p->familyid, false, true))
				AddFaction(p->familyid, true, p->name.begin(), p->name.size(), 0, 0, 0);

			if ((int)p->familyid == pHost->GetFamilyID())
			{
				ClearHostFaction(false);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_LEAVE_SELF);
			}
			else
			{
				ACString strOperant;
				_cp_str(strOperant, p->name.begin(), p->name.size());
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_LEAVE, strOperant);

				if (RemoveFamily(p->familyid))
				{
					CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
					((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->BuildFamilyList();
				}
			}
		}
	}
	else
		AddFixedMsg(p->retcode);

	return 0;
}

void CECFactionMan::OnUpgrade(const FactionUpgrade_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		Faction_Info* pFaction = GetFaction(pHost->GetFactionID());

		if (pFaction)
			pFaction->SetLevel(p->level);

		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_UPGRADE);
	}
}

void CECFactionMan::RefreshMemList(int nPage, unsigned int familyid, bool first_time)
{
	g_pGame->GetGameSession()->faction_listmember(nPage, familyid, first_time);
}

void CECFactionMan::OnListMember(const FactionList_Re* p)
{
	using namespace GNET;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	Faction_Info* pFaction = GetFaction(p->familyid ? p->familyid : pHost->GetFactionID(), false, p->familyid != 0);

	if (pFaction)
	{
		pFaction->SetMemNum(p->population);
		pFaction->SetOnlineMemNum(p->online);
		pFaction->SetProsperity(p->prosperity);
		pFaction->SetMoney(p->money);
		pFaction->SetBaseStatus(p->base_status);
		pFaction->SetActivity(p->activity);
	}

	if (p->familyid == 0)
		m_nDeleteTime = p->deletetime;
	else if ((int)p->familyid == pHost->GetFamilyID())
	{
		m_nFamilyDeleteTime = p->deletetime;
		m_FamilySkillList.clear();

		for (size_t i = 0; i < p->familyskill.size(); i++)
			m_FamilySkillList.push_back(p->familyskill[i]);
	}

	ResetMemInfoList();

	if (p->istree)
	{
		_cp_str(p->familyid ? m_strFamilyProclaim : m_strProclaim, p->announce.begin(), p->announce.size());
		m_nCurPage = 0;
		m_FamilyList.clear();

		for (size_t i = 0; i < p->familylist.size(); i++)
		{
			const GFamilyBrief* info = &p->familylist[i];
			Faction_Info* pInfo = GetFaction(info->fid, false, true);

			if (pInfo)
				pInfo->SetName(info->name.begin(), info->name.size());
			else
				AddFaction(info->fid, true, info->name.begin(), info->name.size(), 0, 0, 0);

			m_FamilyList.push_back(info->fid);
		}

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildManage"))->BuildFamilyList();
	}
	else
		m_nCurPage = p->page;

	for (size_t i = 0; i < const_cast<FactionList_Re*>(p)->members.size(); i++)
	{
		const MemberInfo& info = const_cast<FactionList_Re*>(p)->members[i];
		Faction_Mem_Info* pMem = new Faction_Mem_Info;
		pMem->SetFamilyID(info.familyid);
		pMem->SetID(info.roleid);
		pMem->SetLev(info.level);
		pMem->SetProf(info.occupation);
		pMem->SetFRoleID(info.title);
		pMem->SetName(info.name.begin(), info.name.size());
		pMem->SetNickName(info.nickname.begin(), info.nickname.size());
		pMem->SetLineId(info.lineid);
		pMem->SetContribution(info.contribution);
		pMem->SetDevotion(info.devotion);
		pMem->SetRebornCount(info.reborn_cnt);
		pMem->SetCultivation(info.cultivation);
		pMem->SetFacCouponAdd(info.fac_coupon_add);
		m_MemList.push_back(pMem);
		m_MemMap[pMem->GetID()] = pMem;
	}

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateFactionList(p->familyid == 0);
}

void CECFactionMan::OnBaseInfo(const GetFactionBaseInfo_Re* p)
{
	const Octets& o = p->faction_info.name;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	Faction_Info* pInfo = g_pGame->GetFactionMan()->AddFaction(
		p->faction_info.fid,
		p->scale != 0,
		o.begin(),
		o.size(),
		p->faction_info.level,
		p->faction_info.territoryscore,
		0
	);

	if (pInfo)
		pInfo->SetNimbus(p->faction_info.nimbus);

	if (p->scale != 0 && pHost && pHost->GetFamilyID() == (int)p->faction_info.fid)
	{
		GNET::FamilyRecord rec(pHost->GetFamilyID(), pHost->GetCharacterID(), 0, std::vector<int>(), 0);
		g_pGame->GetGameSession()->SendNetData(rec);
	}
}

void CECFactionMan::OnDismiss(const FactionDismiss_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		if (p->scale)
		{
			if (p->oper == 1)
			{
				ClearHostFaction(true);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_DISMISS);
			}
			else
			{
				m_nFamilyDeleteTime = 0;
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FAMILY_DISMISS_CANCEL);
			}
		}
		else
		{
			if (p->oper == 1)
			{
				ClearHostFaction(false);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_DISMISS);
			}
			else
			{
				m_nDeleteTime = 0;
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_DISMISS_CANCEL);
			}
		}
	}
	else if (p->retcode == ERR_FC_PREDELSUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(p->scale ? FIXMSG_FAMILY_DISMISS_OPRT : FIXMSG_FC_DISMISS_OPRT);
	else
		AddFixedMsg(p->retcode);
}

void CECFactionMan::OnInviteJoin(const FactionInvite* p)
{
	using namespace GNET;

	FactionInviteArg* pArg = static_cast<FactionInviteArg*>(p->GetArgument());
	ACString strMemName;
	_cp_str(strMemName, pArg->role_name.begin(), pArg->role_name.size());
	g_pGame->GetGameRun()->AddPlayerName(pArg->roleid, strMemName);
	ACString str;
	_cp_str(str, pArg->faction_name.begin(), pArg->faction_name.size());

	if (!GetFaction(pArg->factionid, false, pArg->scale != 0))
		AddFaction(pArg->factionid, pArg->scale != 0, pArg->faction_name.begin(), pArg->faction_name.size(), 0, 0, 0);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->PopupFactionInviteMenu(pArg->roleid, str, reinterpret_cast<DWORD>(p));
}


void CECFactionMan::OnInviteCombat(const CombatInvite* p)
{
	using namespace GNET;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CombatInviteArg* pArg = static_cast<CombatInviteArg*>(p->GetArgument());

	if (pArg->faction_name.size())
	{
		ACString strFactionName;
		_cp_str(strFactionName, pArg->faction_name.begin(), pArg->faction_name.size());
		pGameUI->PopupFactionCombatMenu(strFactionName, reinterpret_cast<DWORD>(p));
	}
}

DWORD CECFactionMan::OnRename(const FactionNickname_Re* p)
{
	if (p->retcode == ERR_SUCCESS)
	{
		Faction_Mem_Info* pMem = GetMember(p->receiver);
		if (pMem) pMem->SetNickName(p->nickname.begin(), p->nickname.size());
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

		if (pHost->GetCharacterID() == p->receiver)
		{
			pHost->SetNickname(p->nickname.begin(), p->nickname.size());
			const ACHAR* szOperator = GetMemName(p->roleid);
			if (!szOperator) return FO_OPERATOR_UNK;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_RENAME, szOperator, pHost->GetName(), pHost->GetNickname());
		}
		else
		{
			DWORD dwRet = 0;

			const ACHAR* szOperator = GetMemName(p->roleid);
			if (!szOperator) dwRet |= FO_OPERATOR_UNK;

			const ACHAR* szOperant = GetMemName(p->receiver);
			if (!szOperant) dwRet |= FO_OPERANT_UNK;

			if (dwRet) return dwRet;

			ACString strNickName;
			_cp_str(strNickName, p->nickname.begin(), p->nickname.size());
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FC_RENAME, szOperator, szOperant, strNickName);
		}

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->UpdateFactionList();
	}
	else
		AddFixedMsg(p->retcode);

	return 0;
}

void CECFactionMan::OnUpdateTerritoryScore(const TerritoryScoreUpdate* p)
{
	Faction_Info* pInfo = GetFaction(p->factionid);
	if (pInfo)
		pInfo->SetTerritoryScore(p->territoryscore);		
}

void CECFactionMan::OnFactionChangeName(const ChangeFactionName_Re* p)
{
	if (p->retcode!=ERR_SUCCESS)
	{
		AddFixedMsg(p->retcode);
		return;
	}
	Faction_Info* pInfo = GetFaction(p->fid);
	if (pInfo)
	{
		pInfo->SetName(p->newname.begin(), p->newname.size());
	}
}

void CECFactionMan::OnCrssvrTeamsJoin(const CrssvrTeamsInvite* p)
{
	using namespace GNET;
	//int roleid;
	//int invitee;
	//int teamid;
	//Octets team_name;
	//Octets role_name;

	CrssvrTeamsInviteArg* pArg = static_cast<CrssvrTeamsInviteArg*>(p->GetArgument());

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->PopupCrssvrTeamsInviteMenu(pArg->roleid, reinterpret_cast<DWORD>(p),pArg);
}


bool CECFactionMan::marshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		os << (unsigned short)m_FactionMap.size();

		for (FactionInfoMap::iterator it = m_FactionMap.begin(); it != m_FactionMap.end(); ++it)
		{
			if (!it->second->marshal(os))
				return false;
		}
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

bool CECFactionMan::unmarshal(GNET::Marshal::OctetsStream& os)
{
	try
	{
		unsigned short count;
		os >> count;

		for (unsigned short i = 0; i < count; i++)
		{
			Faction_Info* pInfo = new Faction_Info;

			if (!pInfo->unmarshal(os))
			{
				delete pInfo;
				return false;
			}
			else
				m_FactionMap[pInfo->GetID()] = pInfo;
		}
	}
	catch (Marshal::Exception &)
	{
		return false;
	}

	return true;
}

//	Check whether specified protocol can be processed
bool CECFactionMan::CanProcessPtrc(GNET::Protocol* pProtocol)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	switch (pProtocol->GetType())
	{
	case PROTOCOL_FACTIONCHAT:
	{
		FactionChat* p = static_cast<FactionChat*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->src, false))
			return false;

		break;
	}
	case PROTOCOL_FACTIONRECRUIT_RE:
	{
		FactionRecruit_Re* p = static_cast<FactionRecruit_Re*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->inviter, false))
			return false;

		break;
	}
	case PROTOCOL_FACTIONRESIGN_RE:
	{
		FactionResign_Re* p = static_cast<FactionResign_Re*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->roleid, false))
			return false;

		break;
	}
	case PROTOCOL_FACTIONNICKNAME_RE:
	{
		FactionNickname_Re* p = static_cast<FactionNickname_Re*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->roleid, false) ||
			!pGameRun->GetPlayerName(p->receiver, false))
			return false;

		break;
	}
	case PROTOCOL_FACTIONEXPEL_RE:
	{
		FactionExpel_Re* p = static_cast<FactionExpel_Re*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->roleid, false))
			return false;

		break;
	}
	case PROTOCOL_FACTIONAPPOINT_RE:
	{
		FactionAppoint_Re* p = static_cast<FactionAppoint_Re*>(pProtocol);

		if (!pGameRun->GetPlayerName(p->roleid, false) ||
			!pGameRun->GetPlayerName(p->candidate, false))
			return false;

		break;
	}
	}

	return true;
}
