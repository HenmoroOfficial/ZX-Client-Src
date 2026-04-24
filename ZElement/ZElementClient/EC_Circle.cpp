/*
 * FILE: EC_ElsePlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Lihengju, 2010/4/29
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Circle.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_IvtrItem.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_FixedMsg.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_Achievement.h"
#include "DlgClassmateManage.h"
#include "DlgInfo.h"

#include "getcirclebaseinfo_re.hpp"
#include "circlelist_re.hpp"
#include "gcirclechat.hpp"
#include "circlenotify.hpp"

#define new A_DEBUG_NEW



inline void CECCircleMan::AddFixedMsg(int iMsg)
{
#define MSG_CASE(msg) \
{\
	case msg:\
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_##msg);\
		break;\
}

	switch (iMsg)
	{
// 	MSG_CASE(ERR_FC_NETWORKERR)
// 	MSG_CASE(ERR_FC_INVALID_OPERATION)
	default:
		break;
	}
}

bool CECCircleMan::RemoveMember(int iRoleId)
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

	CircleMemMap::iterator it = m_MemMap.find(iRoleId);

	if (it != m_MemMap.end())
		m_MemMap.erase(it);

	return bRet;
}

Circle_Info* CECCircleMan::GetCircle(unsigned int uId, bool bRequestFromServer)
{
	CircleInfoMap::iterator it = m_CircleMap.find(uId);
	if (it == m_CircleMap.end())
	{
		if (bRequestFromServer)
			g_pGame->GetGameSession()->circle_get_baseinfo(1, reinterpret_cast<int*>(&uId));

		return NULL;
	}

	return it->second;	
}


void Circle_Mem_Info::SetName(const ACHAR* szName)
{
	g_pGame->GetGameRun()->AddPlayerName(m_id, szName);
}

void Circle_Mem_Info::SetName(const void* s, int len)
{
	ACString name;
	_cp_str(name, s, len);
	SetName(name);	
}

const ACHAR* Circle_Mem_Info::GetName() const
{
	const ACHAR* szName = g_pGame->GetGameRun()->GetPlayerName(m_id, false);
	return szName ? szName : _AL("");	
}

void CECCircleMan::Release(bool bOnlyFreeMember)
{
	if (!bOnlyFreeMember)
	{
		CircleInfoMap::iterator it = m_CircleMap.begin();

		for (; it != m_CircleMap.end(); ++it)
			delete it->second;

		m_CircleMap.clear();

// 		CircleMemMap::iterator it2 = m_MemMap.begin();
// 		for (; it2 != m_MemMap.end(); ++it2)
// 			delete it2->second;
// 
// 		m_MemMap.clear();
	}

	ResetMemInfoList();	
	ResetCircleHistoryList();
}

void CECCircleMan::OnBaseInfo(const GetCircleBaseInfo_Re* p)
{
	ACString str;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	for (size_t i = 0; i< p->circleinfolist.size(); ++i)
	{
		str.Format(pGameUI->GetStringFromTable(10202 + p->circleinfolist[i].grade), p->circleinfolist[i].name);
		Circle_Info* pInfo = g_pGame->GetCircleMan()->AddCircle(
			p->circleinfolist[i].circleid,
			str,
			p->circleinfolist[i].grade,
			p->circleinfolist[i].master_roleid
		);
	}
}

void CECCircleMan::OnCircleList(const CircleList_Re* p)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	pHost->SetCircleID(p->circleid);
	m_iCreateTime = p->createtime;
	m_nOnlineNum = p->online_num;
	m_uCirclePoint = p->circlepoint;
	m_nGradBonus = p->grad_bonus;
	
	ResetMemInfoList();
	for (size_t i = 0; i< p->members.size(); ++i )
	{
		const CircleMemberInfo& member = p->members[i];

		const Octets& o = p->members[i].name;

		Circle_Mem_Info* pInfo = new Circle_Mem_Info;
		pInfo->SetID(member.roleid);
		pInfo->SetName(member.name.begin(), member.name.size());
		pInfo->SetLev(member.level);
		pInfo->SetRebornCount(member.reborn_cnt);
		pInfo->SetProf(member.occupation);
		pInfo->SetGender(member.gender);
		pInfo->SetGradTime(member.grad_time>0 ? member.grad_time - g_pGame->GetTimeZoneBias() * 60 : 0);
		pInfo->SetJoinTime(member.jointime>0 ? member.jointime - g_pGame->GetTimeZoneBias() * 60 : 0);
		pInfo->SetLineId(member.lineid);

		m_MemMap[member.roleid] = pInfo;
		m_MemList.push_back(pInfo);
	}

	ResetCircleHistoryList();
	ACString str;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	for(size_t j =0; j< p->history.size(); ++j )
	{
		const GCircleHistory& history = p->history[j];
		str.Format(pGameUI->GetStringFromTable(10202 + p->history[j].circlegrade), p->history[j].circlename);
		Circle_History_Info* pInfo = new Circle_History_Info;
		pInfo->SetID(history.circleid);
		pInfo->SetJoinTime(history.jointime>0 ? history.jointime - g_pGame->GetTimeZoneBias() * 60 : 0);
		pInfo->SetName(str);
		pInfo->SetGradTime(history.graduate_time>0 ? history.graduate_time - g_pGame->GetTimeZoneBias() * 60 : 0);
		pInfo->SetCircleLevel(history.circlegrade);
		pInfo->SetTitleMask(history.titlemask);
		m_HistoryList.push_back(pInfo);
	}
	
	CDlgClassmateManage* pDlg = (CDlgClassmateManage*)pGameUI->GetDialog("Win_ClassmateManage");
	pDlg->BuildList();
}

DWORD CECCircleMan::OnChat(const GCircleChat* p)
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
	else if (pAchievementInfo && pAchievementInfo->cmd_id == CHAT_ACHIEVEMENT)
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
	return 0;	
}

const ACHAR* CECCircleMan::GetMemName(int iRoleId)
{
	return g_pGame->GetGameRun()->GetPlayerName(iRoleId, false);
}

void CECCircleMan::OnCircleNotify(const CircleNotify* p)
{

	enum {
		CIRCLE_JOIN = 1,
		CIRCLE_GRADUATE,      
		CIRCLE_APPOINT_MASTER,
		CIRCLE_APPOINT_BOY,
		CIRCLE_APPOINT_GIRL,
        CIRCLE_POINT_OVER_THRESHOLD,
	};
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(p->roleid);
	if (pPlayer)
		pPlayer->SetCircleID(p->circleid);
	ACString str;
	ACString strName;
	strName.Format(pGameUI->GetStringFromTable(10202 + p->grade), p->name);
	switch(p->msgtype)
	{
	case CIRCLE_JOIN:
		{
			if (pGameUI->GetDialog("Win_ClassmateManage")->IsShow())
			{
				pGameUI->GetDialog("Win_ClassmateManage")->Show(true); // µ÷ÓÃOnShowDialog()
			}
			else
			{
				ACString strText;
				strText.Format(pGameUI->GetStringFromTable(10232), strName);
				pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_EnterCircle", strText, 20000, 0, 0, 0);
			}
		}
		break;
	case CIRCLE_GRADUATE:
		{
			Circle_Info * pInfo = GetCircle(g_pGame->GetGameRun()->GetHostPlayer()->GetCircleID(), true);
			if (!pInfo)
				return;
			if (pPlayer)
				pPlayer->SetCircleID(0);
			str.Format(pGameUI->GetStringFromTable(10200), pInfo->GetName());
			pGameUI->AddChatMessage(str, GP_CHAT_BROADCAST);
			break;
		}
	case CIRCLE_APPOINT_MASTER:
		if ((int)p->roleid == g_pGame->GetGameRun()->GetHostPlayer()->GetPlayerInfo().cid)
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(10201), GP_CHAT_BROADCAST);
		}
		else if (pPlayer)
		{
			str.Format(pGameUI->GetStringFromTable(10202), pPlayer->GetName());
			pGameUI->AddChatMessage(str, GP_CHAT_BROADCAST);
		}
		break;

	case CIRCLE_APPOINT_BOY:

		break;
	case CIRCLE_APPOINT_GIRL:

		break;
	case CIRCLE_POINT_OVER_THRESHOLD:
		pGameUI->MessageBox("GameOk_CircleCanGetAward", pGameUI->GetStringFromTable(10235), MB_OK);
		break;
	default:
		break;
	}
}

bool CECCircleMan::CanProcessPtrc(GNET::Protocol* pProtocol)
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	switch (pProtocol->GetType())
	{
	case PROTOCOL_GCIRCLECHAT:
	{
		GCircleChat* p = static_cast<GCircleChat*>(pProtocol);
		if (!pGameRun->GetPlayerName(p->src, false))
			return false;
// 		if (!g_pGame->GetCircleMan()->GetCircle(p->circleid))
// 			return false;
		break;
	}
	}
	return true;
}