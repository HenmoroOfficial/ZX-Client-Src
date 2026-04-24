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
#pragma once

#include "hashmap.h"
#include "vector.h"
#include "ABaseDef.h"
#include "AList2.h"
#include "AAssist.h"
#include "gcirclebaseinfo"
#include "gcirclehistory"
#include "circlememberinfo"

namespace GNET
{
class Protocol;
class CircleList_Re;
class GetCircleBaseInfo_Re;
class GCircleChat;
class CircleNotify;
}

extern void _cp_str(ACString& str, const void* s, int len);


class Circle_Info
{
public:

	Circle_Info() {}
	~Circle_Info() {}

protected:

	unsigned int	m_id;
	ACString		m_szName;
	int				m_iLevel;
	int				m_iMasterId; //È¦³¤id

public:

	unsigned int GetID() const { return m_id; }
	void SetID(unsigned int nId) { m_id = nId; }
	const ACHAR* GetName() const { return m_szName; }
	void SetName(const void* name, int len) { _cp_str(m_szName, name, len); }
	void SetName(const ACHAR* name){ m_szName = name; }
	int	 GetLevel() const { return m_iLevel; }
	void SetLevel(int iLev) { m_iLevel = iLev; }
	int	 GetMasterID() const { return m_iMasterId; }
	void SetMasterID(int id) { m_iMasterId = id; }
};

typedef abase::hash_map<unsigned int, Circle_Info*> CircleInfoMap;

class Circle_Mem_Info
{
public:

	Circle_Mem_Info() {}
	~Circle_Mem_Info() {}

protected:

	int m_id;
	int m_iLevel;
	int m_iProf;
	int m_iLineid;
//	ACString m_szName;
	int m_iRebornCount;
	int m_iGender;
	int m_iJoinTime;
	int m_iGradTime;

public:

	int GetID() const { return m_id; }
	int GetLev() const { return m_iLevel; }
	int GetProf() const { return m_iProf; }
	int GetLineId() const { return m_iLineid; }
	const ACHAR* GetName() const;
	int GetRebornCount() const { return m_iRebornCount; }
	int GetJoinTime() const { return m_iJoinTime; }
	int GetGradTime() const { return m_iGradTime; }
	int GetGender() const {return m_iGender; }

	void SetID(int id) { m_id = id; }
	void SetLev(int l) { m_iLevel = l; }
	void SetProf(int p) { m_iProf = p; }
	void SetLineId(int l) { m_iLineid = l; }
	void SetName(const void* s, int len);
	void SetName(const ACHAR* szName);
	void SetRebornCount(int i) { m_iRebornCount = i; }
	void SetJoinTime(int time) { m_iJoinTime = time; }
	void SetGradTime(int time) { m_iGradTime = time; }
	void SetGender(int gender) { m_iGender = gender; }
};

typedef abase::vector<Circle_Mem_Info*> CircleMemList;

class Circle_History_Info
{
public:

	Circle_History_Info() {}
	~Circle_History_Info() {}

protected:

	unsigned int	m_id;
	ACString		m_szName;
	unsigned char	m_cTitleMask;
	int				m_iJoinTime;
	int				m_iGradTime;
	int				m_iCircleLevel;

public:

	unsigned int GetID() const { return m_id; }
	void SetID(unsigned int nId) { m_id = nId; }
	const ACHAR* GetName() const { return m_szName; }
	void SetName(const void* name, int len) { _cp_str(m_szName, name, len); }
	void SetName(const ACHAR* name) { m_szName = name; }
	int  GetJoinTime() const { return m_iJoinTime; }
	void SetJoinTime(int time) { m_iJoinTime = time; } 
	int  GetGradTime() const { return m_iGradTime; }
	void SetGradTime(int time) { m_iGradTime = time; }
	int  GetCircleLevel() const { return m_iCircleLevel; }
	void SetCircleLevel(int level) { m_iCircleLevel = level; }
	int  GetTitleMask() const { return m_cTitleMask; }
	void SetTitleMask(int title) { m_cTitleMask = title; }
};

typedef abase::vector<Circle_History_Info*> CircleHistoryList;




class CECCircleMan
{
public:

	CECCircleMan() : m_MemMap(256), m_nCurPage(0)
	{
		m_MemList.reserve(256);
		m_HistoryList.reserve(8);
		m_iCreateTime	= 0;
		m_nOnlineNum	= 0;
		m_uCirclePoint	= 0;
		m_nGradBonus	= 0;
	}

	~CECCircleMan() { Release(); }
	void Release(bool bOnlyFreeMember = false);

protected:

	typedef abase::hash_map<int, Circle_Mem_Info*> CircleMemMap;

	CircleInfoMap	m_CircleMap;
	CircleMemList	m_MemList;
	CircleMemMap	m_MemMap;
	CircleHistoryList m_HistoryList;

	int m_iCreateTime;
	int m_nCurPage;
	int m_nOnlineNum;
	unsigned int m_uCirclePoint;
	INT64 m_nGradBonus;
protected:

	void AddFixedMsg(int iMsg);
	bool RemoveMember(int iRoleId);
	bool RemoveCircle(int iCircle);

	bool HasCircle(int iCircle)
	{
		CircleInfoMap::iterator it = m_CircleMap.find(iCircle);
		if (it != m_CircleMap.end())
			return true;

		return false;
	}
	const ACHAR* GetMemName(int iRoleId);

public:

	// Operations
	Circle_Info* GetCircle(unsigned int uId, bool bRequestFromServer = false);
	void RefreshMemList(int nPage, unsigned int iCircleid = 0, bool first_time = false);
	CircleMemList& GetMemList() { return m_MemList; }
	CircleHistoryList& GetHisList() { return m_HistoryList; }

	int GetCurPage() const { return m_nCurPage; }
	int GetCreateTime() const { return m_iCreateTime; }
	void SetCreateTime(int time) { m_iCreateTime = time; }
	int GetOnlineNum() const { return m_nOnlineNum; }
	unsigned int GetCirclePoint() { return m_uCirclePoint; }
	INT64 GetGradBonus() { return m_nGradBonus; }
	void SetGradBonus(INT64 bonus) { m_nGradBonus = bonus; }

	Circle_Info* AddCircle(unsigned int uId, const ACHAR* name, int iLevel, int idMaster)
	{
		CircleInfoMap::iterator it = m_CircleMap.find(uId);
		Circle_Info* p = (it == m_CircleMap.end() ? (new Circle_Info) : it->second);
		p->SetID(uId);
		p->SetName(name);
		p->SetLevel(iLevel);
		p->SetMasterID(idMaster);
		m_CircleMap[uId] = p;
		return p;
	}

	void ResetMemInfoList()
	{
		for (size_t i = 0; i < m_MemList.size(); i++)
			delete m_MemList[i];

		m_MemList.clear();
		m_MemMap.clear();
	}

	void ResetCircleHistoryList()
	{
		for (size_t i = 0; i < m_HistoryList.size(); i++)
			delete m_HistoryList[i];

		m_HistoryList.clear();
	}
	Circle_Mem_Info* GetMember(int nRoleId)
	{
		CircleMemMap::iterator it = m_MemMap.find(nRoleId);
		return it == m_MemMap.end() ? NULL : it->second;
	}

	bool CanProcessPtrc(GNET::Protocol* pProtocol);

	// Msgs
	void OnCircleList(const CircleList_Re* p);
	void OnBaseInfo(const GetCircleBaseInfo_Re* p);
	DWORD OnChat(const GCircleChat* p);
	void OnCircleNotify(const CircleNotify* p);
};

