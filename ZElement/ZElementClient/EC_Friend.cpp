/*
 * FILE: EC_Friend.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/7/12
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma warning (disable: 4284)

#include "A3DMacros.h"

#include "EC_Friend.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_FixedMsg.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "gnet.h"
#include "EC_Configs.h"

// some Infomation saved in the dlg. 
#include "DlgFriendList.h"
#include "GFriendInfo"

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
//	Implement CECFriendMan
//	
///////////////////////////////////////////////////////////////////////////

CECFriendMan::CECFriendMan() : 
m_FriendTab(64),
m_TempFriendTab(64),
m_GTOnLineInfoTab(64),
m_FriendCallBckMap(64)
{
		bFriendCallBck = false;                 // 好友召回系统尚未启动
}

CECFriendMan::~CECFriendMan()
{
	CECFriendMan::RemoveAllFriends();
	CECFriendMan::RemoveAllTempFriends();
	CECFriendMan::RemoveAllOfflineMsgs();
}

//	Reset friend list
bool CECFriendMan::ResetFriends(GNET::GetFriends_Re& Data)
{
	using namespace GNET;

	//	Remove all exiting friends
	RemoveAllFriends();

	//	Build groups ...
	//	Create group 0 at first
	AddGroup(0, g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_MYFRIEND));

	//	Create other groups
	int i, iNumGroup = Data.groups.size();
	for (i=0; i < iNumGroup; i++)
	{
		const GGroupInfo& gi = Data.groups[i];
		if (gi.name.size())
		{
			ACString strName((const ACHAR*)gi.name.begin(), gi.name.size() / sizeof (ACHAR));
			AddGroup(gi.gid, strName);
		}
		else
		{
			AddGroup(gi.gid, NULL);
		}
	}

	//	Build friend list ...
	int iNumFriend = Data.friends.size();
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	APtrArray<FRIEND*> aFriends;
	aFriends.SetSize(iNumFriend, 10);

	for (i=0; i < iNumFriend; i++)
	{
		const GClientFriendInfo& cfi = Data.friends[i];
		FRIEND* pFriend = AddFriend(cfi);
		aFriends[i] = pFriend;
	}
	
	g_pGame->GetGameSession()->enemies_get(
		g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID(), 3);

	return true;
}

CECFriendMan::FRIEND* CECFriendMan::AddFriend(const GNET::GFriendInfo& fi)
{
	GNET::GClientFriendInfo cfi;
	
	cfi.rid				= fi.rid;
	cfi.lineid			= fi.lineid;
	cfi.gid				= fi.gid;
	cfi.name			= fi.name;
	return AddFriend(cfi);
}

//	Add a friend
CECFriendMan::FRIEND* CECFriendMan::AddFriend(const GNET::GClientFriendInfo& fi)
{
	if (!CheckInit())
		return NULL;

	//	In friend list, one player ID may have two names, so check the case here !
	FRIEND* pFriend = GetFriendByID(fi.rid);
	if (pFriend)
	{
		//	Remove the friend
		RemoveFriend(fi.rid);
	}

	if (!(pFriend = new FRIEND))
		return NULL;

	pFriend->id				= fi.rid;
	pFriend->iGroup			= fi.gid;
	pFriend->iLine			= fi.lineid;
	pFriend->nLevel			= -1;
	pFriend->idArea			= -1;
	pFriend->iProfession	= -1;
	pFriend->iStatGT		= 0;

	// 避免即将加为好友的人在黑名单列表中
	EC_BLACKLIST_SETTING setting = g_pGame->GetConfigs()->GetBlackListSettings();
	int nCount = 0;
	while( nCount < EC_BLACKLIST_LEN && setting.idPlayer[nCount] > 0 )
	{
		if (pFriend->id == setting.idPlayer[nCount])
			return NULL;
		nCount++;
	}

	if (fi.name.size())
	{
		pFriend->strName = ACString((const ACHAR*)fi.name.begin(), fi.name.size() / sizeof (ACHAR));

		//	Cache friend name if this function isn't called by ResetFriends(), because
		//	player name in friend list may not be right when ResetFriends()
		//  GT use name of under line friend id-name pair modified fuchonggang 2011-3-16
//		if (fi.lineid >= 0)
		{
			ACString strName((const ACHAR*)fi.name.begin(), fi.name.size() / sizeof (ACHAR));
			g_pGame->GetGameRun()->AddPlayerName(fi.rid, strName);
		}
	}

	//	Add friend to proper group
	int i;
	for (i=0; i < m_aGroups.GetSize(); i++)
	{
		if (m_aGroups[i]->iGroup == pFriend->iGroup)
		{
			m_aGroups[i]->aFriends.Add(pFriend);
			break;
		}
	}

	if (i >= m_aGroups.GetSize())
	{
		ASSERT(i < m_aGroups.GetSize());
		delete pFriend;
		return NULL;
	}

	//	gt 在线信息可能在好友信息之前，所以需要更新gt在线信息
	m_FriendTab.put(fi.rid, pFriend);
	GTOnLineInfo::pair_type Pair = m_GTOnLineInfoTab.get(fi.rid);
	if (Pair.second)
	{
		pFriend->iStatGT = *Pair.first;
		if (fi.name.size())
		{
			ACString strName((const ACHAR*)fi.name.begin(), fi.name.size() / sizeof (ACHAR));
			g_pGame->GetGameRun()->AddPlayerName(pFriend->id, pFriend->GetName());
		}
		m_GTOnLineInfoTab.erase(fi.rid);
	}

	return pFriend;
}

//	Remove a friend
void CECFriendMan::RemoveFriend(int idFriend)
{
	if (!CheckInit())
		return;

	//	Friend exists ?
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (!Pair.second)
		return;		//	Counldn't find this friend
	
	FRIEND* pFriend = *Pair.first;

	//	Remove from group
	int i;
	for (i=0; i < m_aGroups.GetSize(); i++)
	{
		GROUP* pGroup = m_aGroups[i];
		if (pGroup->iGroup == pFriend->iGroup)
		{
			for (int j=0; j < pGroup->aFriends.GetSize(); j++)
			{
				if (pFriend == pGroup->aFriends[j])
				{
					pGroup->aFriends.RemoveAt(j);
					break;
				}
			}

			break;
		}
	}

	ASSERT(i < m_aGroups.GetSize());

	//	Remove from table
	m_FriendTab.erase(pFriend->id);

	delete pFriend;
}

//	Get friend by ID
CECFriendMan::FRIEND* CECFriendMan::GetFriendByID(int idFriend)
{
	//	Friend exists ?
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
		return *Pair.first;

	return NULL;
}

//	Remove all friends
void CECFriendMan::RemoveAllFriends()
{
	if (!CheckInit())
		return;

	//	Release friend table
	FriendTable::iterator it = m_FriendTab.begin();
	for (; it != m_FriendTab.end(); ++it)
	{
		FRIEND* pFriend = *it.value();
		delete pFriend;
	}

	m_FriendTab.clear();

	//	Release groups
	for (int i=0; i < m_aGroups.GetSize(); i++)
	{
		delete m_aGroups[i];
	}

	m_aGroups.RemoveAll();
}

//	Change friend status
void CECFriendMan::ChangeFriendStatus(int idFriend, int lineid)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->iLine = lineid;
	}
}

//	Change friend status
void CECFriendMan::ChangeFriendGTStatus(int idFriend, int iStatus)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->iStatGT = iStatus;
	}
	else
	{
		m_GTOnLineInfoTab.put(idFriend, iStatus);
	}
}

//	Change friend's group
void CECFriendMan::ChangeFriendGroup(int idFriend, int idGroup)
{
	if (!CheckInit())
		return;

	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (!Pair.second)
		return;
	
	FRIEND* pFriend = *Pair.first;
	if (pFriend->iGroup == idGroup)
		return;

	//	Remove friend from old group
	GROUP* pGroup = GetGroupByID(pFriend->iGroup);
	if (pGroup)
	{
		for (int i=0; i < pGroup->aFriends.GetSize(); i++)
		{
			if (pFriend == pGroup->aFriends[i])
			{
				pGroup->aFriends.RemoveAt(i);
				break;
			}
		}
	}

	if (!(pGroup = GetGroupByID(idGroup)))
	{
		//	This shouldn't happen
		ASSERT(pGroup);
		delete pFriend;
		return;
	}

	//	Add to new group
	pFriend->iGroup = idGroup;
	pGroup->aFriends.Add(pFriend);
}

//	Add group
bool CECFriendMan::AddGroup(int idGroup, const ACHAR* szName)
{
	if (idGroup && !CheckInit())
		return false;

	GROUP* pGroup = new GROUP;
	if (!pGroup)
		return false;

	pGroup->strName = szName ? szName : _AL("Group");
	pGroup->iGroup	= idGroup;
	pGroup->color	= A3DCOLORRGB(255, 204, 0);

	m_aGroups.Add(pGroup);

	return true;
}

//	Remove group
void CECFriendMan::RemoveGroup(int idGroup)
{
	if (!CheckInit())
		return;

	if (!idGroup)
	{
		ASSERT(idGroup != 0);
		return;
	}

	//	Get group 0
	GROUP* pGroup0 = m_aGroups[0];
	if (!pGroup0)
		return;

	ASSERT(pGroup0->iGroup == 0);

	int i, j;
	for (i=0; i < m_aGroups.GetSize(); i++)
	{
		if (m_aGroups[i]->iGroup == idGroup)
			break;
	}

	if (i >= m_aGroups.GetSize())
		return;

	//	Remove all friends in this group to group 0
	GROUP* pGroup = m_aGroups[i];
	for (j=0; j < pGroup->aFriends.GetSize(); j++)
	{
		FRIEND* pFriend = pGroup->aFriends[j];
		pFriend->iGroup = 0;
		pGroup0->aFriends.Add(pFriend);
	}

	//	Delete group
	m_aGroups.RemoveAt(i);
	delete pGroup;
}

//	Change group name
void CECFriendMan::ChangeGroupName(int idGroup, const ACHAR* szName)
{
	if (!CheckInit())
		return;

	if (!szName || !szName[0] || !idGroup)
		return;

	for (int i=0; i < m_aGroups.GetSize(); i++)
	{
		if (m_aGroups[i]->iGroup == idGroup)
		{
			m_aGroups[i]->strName = szName;
			break;
		}
	}
}

//	Set group color
void CECFriendMan::SetGroupColor(int idGroup, A3DCOLOR color)
{
	if (!CheckInit())
		return;

	if (!idGroup)
		return;		//	Cann't change default group color

	for (int i=0; i < m_aGroups.GetSize(); i++)
	{
		if (m_aGroups[i]->iGroup == idGroup)
		{
			m_aGroups[i]->color = color;
			break;
		}
	}
}

int CECFriendMan::GetChatRouteType(int cid)
{
	//	TO_GT-2		TO_GAME-3	TO_GT_AND_GAME-4
	//										not-fd gt+game	game	gt	both-not
	static const int privateType[3][5] = {  {3,		4,		3,		2,	3},	// no msg
											{3,		3,		3,		2,	3},	// game
											{2,		2,		3,		2,	4}	};	// gt
	int lstMsgType = g_pGame->GetGameRun()->PlayerLstMsgType(cid);
	if (lstMsgType<0||lstMsgType>2)
	{
		return 3;
	}
	int fdType = 0;
	CECFriendMan::FRIEND* fd = GetFriendByID(cid);
	if (fd)
	{
		if (fd->iLine>=0&&fd->iStatGT>0)
			fdType = 1;
		else if (fd->iLine>=0)
			fdType = 2;
		else if (fd->iStatGT>0)
			fdType = 3;
		else
			fdType = 4;
	}
	return privateType[lstMsgType][fdType];
}

//	Get group by id
CECFriendMan::GROUP* CECFriendMan::GetGroupByID(int id)
{
	for (int i=0; i < m_aGroups.GetSize(); i++)
	{
		if (m_aGroups[i]->iGroup == id)
			return m_aGroups[i];
	}

	return NULL;
}

//	Reset offline message list
bool CECFriendMan::ResetOfflineMsgs(GNET::GetSavedMsg_Re& p)
{
	using namespace GNET;

	//	Remove all old offline messages
	RemoveAllOfflineMsgs();

	int i, iNumMsg = p.messages.size();
	for (i=0; i < iNumMsg; i++)
	{
		Message& src = p.messages[i];

		MESSAGE* pdst = new MESSAGE;
		if (!pdst)
			return true;

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ACString strName, strMsg;
		if( src.srcroleid == 11 )	// 离婚
		{
			int id;
			Octets name;
			try{ 
				Marshal::OctetsStream(src.msg)>>id>>name; 
				strName = ACString((const ACHAR*)name.begin(), name.size() / sizeof(ACHAR));
				strMsg.Format(pGameUI->GetStringFromTable(4011), 
					strName);
				pdst->idSender = 0;
			}catch(...){}
		}
		else if( src.srcroleid==1 && src.channel==CHANNEL_SERVERINFO )	// 举报外挂回复
		{
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(18211), GP_CHAT_MISC);
		}
		else
		{
			if (!src.src_name.size() || !src.msg.size())
				continue;
			
			strName = ACString((const ACHAR*)src.src_name.begin(), src.src_name.size() / sizeof (ACHAR));
			strMsg = ACString((const ACHAR*)src.msg.begin(), src.msg.size() / sizeof (ACHAR));
			pdst->idSender	= src.srcroleid;
		}

		pdst->strName	= strName;
		pdst->strMsg	= strMsg;
		pdst->byFlag	= src.channel;

		m_aOfflineMsgs.Add(pdst);
	}

	return true;
}

//	Remove all offline messages
void CECFriendMan::RemoveAllOfflineMsgs()
{
	for (int i=0; i < m_aOfflineMsgs.GetSize(); i++)
	{
		delete m_aOfflineMsgs[i];
	}

	m_aOfflineMsgs.RemoveAll();
}

// Set friend level
void CECFriendMan::SetFriendLevel(int idFriend, int nLevel)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);

	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->nLevel = nLevel;
	}
}

// Set friend area
void CECFriendMan::SetFriendArea(int idFriend, int idArea)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->idArea = idArea;
	}
}

void CECFriendMan::SetFriendProfession(int idFriend, int idProf)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->iProfession = idProf;
	}
}

void CECFriendMan::SetFriendCultivation(int idFriend, int idCult)
{
	FriendTable::pair_type Pair = m_FriendTab.get(idFriend);
	if (Pair.second)
	{
		FRIEND* pFriend = *Pair.first;
		pFriend->iCultivation = idCult;
	}
}

// add template friend
void CECFriendMan::AddTempFriend(int id, const ACHAR* szName)
{
	if ( id == g_pGame->GetGameRun()->GetHostPlayer()->GetCharacterID() )	return;//不添加自己

	const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(id, true);
	CECFriendMan::FRIEND* pFriend = g_pGame->GetGameRun()->GetHostPlayer()->GetFriendMan()->GetFriendByID(id);

	//	In friend list, one player ID may have two names, so check name here !
	if( pFriend && !a_strcmp(pszName, pFriend->GetName()) )
		return;	//已经在好友列表

	//  已经在临时好友中
	TempFriendTable::pair_type Pair = m_TempFriendTab.get(id);
	if (Pair.second)
		return;	

	// 避免即将加为临时好友的人在黑名单列表中
	EC_BLACKLIST_SETTING setting = g_pGame->GetConfigs()->GetBlackListSettings();
	int nCount = 0;
	while( nCount < EC_BLACKLIST_LEN && setting.idPlayer[nCount] > 0 )
	{
		if (id == setting.idPlayer[nCount])
			return;
		nCount++;
	}
	
	// 避免即将加为临时好友的人在宿敌列表中
	CDlgFriendList* pDlgFriendList = (CDlgFriendList *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_FriendList");
	if( pDlgFriendList->IsEnemy(id))
		return;

	TEMPFRIEND* ptp;
	if(!(ptp = new TEMPFRIEND))
		return;
	ptp->id = id;
	ptp->strName = szName;

	m_TempFriendTab.put(id, ptp);
}

//	Remove a friend
void CECFriendMan::RemoveTempFriend(int idTempFriend)
{
	//	Friend exists ?
	TempFriendTable::pair_type Pair = m_TempFriendTab.get(idTempFriend);
	if (!Pair.second)
		return;		//	Counldn't find this friend
	
	TEMPFRIEND* pFriend = *Pair.first;

	//	Remove from table
	m_TempFriendTab.erase(pFriend->id);

	delete pFriend;
}

//	Get friend by ID
CECFriendMan::TEMPFRIEND* CECFriendMan::GetTempFriendByID(int idTempFriend)
{
	//	Friend exists ?
	TempFriendTable::pair_type Pair = m_TempFriendTab.get(idTempFriend);
	if (Pair.second)
		return *Pair.first;

	return NULL;
}

//	Remove all friends
void CECFriendMan::RemoveAllTempFriends()
{
	//	Release friend table
	TempFriendTable::iterator it = m_TempFriendTab.begin();
	for (; it != m_TempFriendTab.end(); ++it)
	{
		TEMPFRIEND* pFriend = *it.value();
		delete pFriend;
	}

	m_TempFriendTab.clear();
}

bool CECFriendMan::StartFriendCallBck(const GNET::FriendCallbackInfo_Re* pFrdCallBckInfo){
		m_FriendCallBckMap.clear();
		GCooldownVector::const_iterator it = pFrdCallBckInfo->callback_limit.cooldown_list.begin();
		GCooldownVector::const_iterator itEnd = pFrdCallBckInfo->callback_limit.cooldown_list.end();
		for(;it != itEnd;it++){		
			int locolTime = it->send_time - g_pGame->GetTimeZoneBias()*60;
			m_FriendCallBckMap[it->roleid] = locolTime;
		}
		bFriendCallBck = true;	
	return bFriendCallBck;
}

void CECFriendMan::SetFrdLastSndtime(int fid, int iTime){
		m_FriendCallBckMap[fid] = iTime;
}

int CECFriendMan::GetFriendLastCallBckTimeByID(int id){
	if (m_FriendCallBckMap.find(id)!=m_FriendCallBckMap.end())
		return m_FriendCallBckMap[id];
	else
		return 0;
}