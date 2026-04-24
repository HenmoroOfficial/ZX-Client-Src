/*
 * FILE: EC_Friend.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/7/12
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "hashtab.h"
#include "hashmap.h"
#include "AArray.h"
#include "AAssist.h"
#include "A3DTypes.h"

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

namespace GNET
{
	class GFriendInfo;
	class GClientFriendInfo;
	class GetFriends_Re;
	class GetSavedMsg_Re;
	class FriendCallbackInfo_Re;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECFriendMan
//	
///////////////////////////////////////////////////////////////////////////

class CECFriendMan
{
public:		//	Types

	struct FRIEND
	{
		int			id;
		int			iProfession;
		int			iCultivation;
		int			iGroup;			//	Group id
		int			iLine;			//	line id
		int			nLevel;
		int			idArea;
		int			iStatGT;
		ACString	strName;

		const ACHAR* GetName() { return strName; }
	};

	struct TEMPFRIEND
	{
		int			id;
		ACString	strName;
		const ACHAR* GetName() { return strName; }
	};

	struct GROUP
	{
		ACString	strName;	//	Group name
		int			iGroup;		//	Group id
		A3DCOLOR	color;		//	Group color
		
		APtrArray<FRIEND*>	aFriends;
	};

	struct MESSAGE
	{
		ACString	strName;	//	Sender name
		int			idSender;	//	Sender id
		ACString	strMsg;		//	Message
		BYTE		byFlag;		//	Flag
	};

	typedef abase::hashtab<FRIEND*, int, abase::_hash_function>	FriendTable;
	typedef abase::hashtab<TEMPFRIEND*, int, abase::_hash_function> TempFriendTable;
	typedef abase::hashtab<int, int, abase::_hash_function> GTOnLineInfo;
	typedef abase::hash_map<int, int>	FriendCallBckMap;//friend id, lastsendtime

public:		//	Constructor and Destructor

	CECFriendMan();
	virtual ~CECFriendMan();

public:		//	Attributes

public:		//	Operations

	//	Check whether manager has been initiailized
	bool CheckInit() { return m_aGroups.GetSize() ? true : false; }
	
	//	Reset friend list
	bool ResetFriends(GNET::GetFriends_Re& Data);
	//	Add a friend
	FRIEND* AddFriend(const GNET::GFriendInfo& fi);
	//	Add a friend
	FRIEND* AddFriend(const GNET::GClientFriendInfo& fi);
	//	Remove a friend
	void RemoveFriend(int idFriend);
	//	Remove all friends
	void RemoveAllFriends();
	//	Change friend status
	void ChangeFriendStatus(int idFriend, int lineid);
	//	Change friend GT status
	void ChangeFriendGTStatus(int idFriend, int iStatus);
	//	Change friend's group
	void ChangeFriendGroup(int idFriend, int idGroup);
	//	Get friend by ID
	FRIEND* GetFriendByID(int idFriend);
	// Set friend level
	void SetFriendLevel(int idFriend, int nLevel);
	// Set friend area
	void SetFriendArea(int idFriend, int idArea);
	// Set friend profession
	void SetFriendProfession(int idFriend, int idProf);
	// Set friend cultivation
	void SetFriendCultivation(int idFriend, int idCult);

	//	Add group
	bool AddGroup(int idGroup, const ACHAR* szName);
	//	Remove group
	void RemoveGroup(int idGroup);
	//	Change group name
	void ChangeGroupName(int idGroup, const ACHAR* szName);
	//	Set group color
	void SetGroupColor(int idGroup, A3DCOLOR color);
	//	Get group number
	int GetGroupNum() { return m_aGroups.GetSize(); }
	//	Get group by index
	GROUP* GetGroupByIndex(int n) { return m_aGroups[n]; }
	//	Get group by id
	GROUP* GetGroupByID(int id);
	//  Get Friend num
	int  GetFriendNum(){ return m_FriendTab.size(); }
	int GetChatRouteType(int cid);

	//	Reset offline message list
	bool ResetOfflineMsgs(GNET::GetSavedMsg_Re& p);
	//	Remove all offline messages
	void RemoveAllOfflineMsgs();
	//	Get offline message number
	int GetOfflineMsgNum() { return m_aOfflineMsgs.GetSize(); }
	//	Get offline message
	MESSAGE* GetOfflineMsg(int n) { return m_aOfflineMsgs[n]; }
	
	//  add friend to template friend list
	void AddTempFriend(int id, const ACHAR* szName);
	//	Remove template friend
	void RemoveTempFriend(int idTempFriend);
	//  Remove all template friends
	void RemoveAllTempFriends();
	//  Get Template friend by ID
	TEMPFRIEND* GetTempFriendByID(int idTempFriend);
	// Get Template friend table
	TempFriendTable& GetTempFriendTable() { return m_TempFriendTab; }

	//好友召回系统初始化
	bool StartFriendCallBck(const GNET::FriendCallbackInfo_Re* pFrdCallBckInfo);
	void SetFrdLastSndtime(int fid, int itime = 0);//设置上次邀请好友的时间(传入GMT-timezonebias)，如无此信息，添加并设为0
	int  GetFriendLastCallBckTimeByID(int id);//如果没有该好友的CD期，添加并设为0，然后返回

protected:	//	Attributes

	APtrArray<GROUP*>	m_aGroups;		//	Group info
	FriendTable			m_FriendTab;	//	Friend table
	TempFriendTable		m_TempFriendTab;//	Template friend table
	GTOnLineInfo		m_GTOnLineInfoTab;
	APtrArray<MESSAGE*>	m_aOfflineMsgs;	//	Offline messages

	bool				bFriendCallBck;//  can Friend Callback Function On
	FriendCallBckMap  m_FriendCallBckMap;

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



