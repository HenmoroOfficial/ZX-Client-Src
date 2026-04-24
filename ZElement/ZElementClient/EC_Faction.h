#pragma once

#include "hashmap.h"
#include "vector.h"
#include "AList2.h"
#include "AAssist.h"
#include "gfactionbaseinfo"
#include "familyskill"

namespace GNET
{
	class Protocol;
	class FactionCreate_Re;
	class FactionRecruit_Re;
	class FactionExpel_Re;
	class FactionAnnounce_Re;
	class FactionResign_Re;
	class FactionAppoint_Re;
	class FactionLeave_Re;
	class FactionUpgrade_Re;
	class FactionChat;
	class FactionList_Re;
	class GetFactionBaseInfo_Re;
	class FactionDismiss_Re;
	class FactionInvite;
	class CombatInvite;
	class FactionNickname_Re;
	class TerritoryScoreUpdate;
	class ChangeFactionName_Re;
	class CrssvrTeamsInvite;
}

#define FO_OPERATOR_UNK		1
#define FO_OPERANT_UNK		2

inline void _cp_str(ACString& str, const void* s, int len)
{
	len /= sizeof(ACHAR);
	ACHAR* p = str.GetBuffer(len + 1);
	memcpy(p, s, len * sizeof(ACHAR));
	p[len] = _AL('\0');
	str.ReleaseBuffer();
}

class Faction_Info
{
public:

	Faction_Info() : m_nNimbus(0), m_nNimBusStar(0),m_nMoney(0) {}
	~Faction_Info() {}

	enum
	{
		ST_FAC_BASE_OPEN = 0x01,
		ST_FAC_BASE_START = 0x02,
	};
protected:

	unsigned int m_id;
	ACString m_szName;
	int m_nLev;
	int m_nMemNum;
	int m_nOnlineMemNum;
	int m_nProsperity;
	int m_nNimbus;
	int m_nNimBusStar;
	int m_nTerritoryScore;
	unsigned int m_nMoney;
	int m_nActivity;	// 帮派活跃度
	unsigned int m_nBaseStatus;	// 基地是否分配GS,是否可以进入	
public:

	unsigned int GetID() const { return m_id; }
	void SetID(unsigned int nId) { m_id = nId; }
	const ACHAR* GetName() const { return m_szName; }
	void SetName(const void* name, int len) { _cp_str(m_szName, name, len); }
	int GetLevel() const { return m_nLev; }
	void SetLevel(int nLev) { m_nLev = nLev; }
	int GetMemNum() const { return m_nMemNum; }
	void SetMemNum(int nNum) { m_nMemNum = nNum; }
	int GetOnlineMemNum() const { return m_nOnlineMemNum; }
	void SetOnlineMemNum(int nNum) { m_nOnlineMemNum = nNum; }
	int GetProsperity() const { return m_nProsperity; }
	void SetProsperity(int nProsperity) { m_nProsperity = nProsperity; }
	int GetNimbus() const { return m_nNimbus; }
	int GetNimbusStar() const { return m_nNimBusStar; }
	void SetNimbus(int n)
	{
		m_nNimbus = n;

		if (m_nNimbus < 5000)
			m_nNimBusStar = 0;
		else if (m_nNimbus < 20000)
			m_nNimBusStar = 1;
		else if (m_nNimbus < 60000)
			m_nNimBusStar = 2;
		else if (m_nNimbus < 120000)
			m_nNimBusStar = 3;
		else if (m_nNimbus < 300000)
			m_nNimBusStar = 4;
		else
			m_nNimBusStar = 5;
	}
	void SetTerritoryScore(int nScore) { m_nTerritoryScore = nScore; }
	int  GetTerritoryScore() const { return m_nTerritoryScore; }
	void SetMoney(unsigned int nMoney){m_nMoney = nMoney;}
	unsigned int  GetMoney(){return m_nMoney;}
	bool marshal(GNET::Marshal::OctetsStream& os);
	bool unmarshal(GNET::Marshal::OctetsStream& os);

	unsigned int GetActivity() const { return m_nActivity; }
	void SetActivity(unsigned int val) { m_nActivity = val; }
	unsigned int GetBaseStatus() const { return m_nBaseStatus; }
	void SetBaseStatus(unsigned int val) { m_nBaseStatus = val; }

	bool IsSelfCreate() 
	{
		if((m_nBaseStatus& 0x01) > 0)
		{
			return true;
		}
		return false;
	}
	bool IsSelfEnter() 
	{
		if((m_nBaseStatus& 0x02) > 0)
		{
			return true;
		}
		return false;
	}

};

typedef abase::hash_map<unsigned int, Faction_Info*> FactionInfoMap;

class Faction_Mem_Info
{
public:
	
	Faction_Mem_Info() {}
	~Faction_Mem_Info() {}

protected:

	int roleid;
	int familyid;
	int froleid;
	int lev;
	int prof;
	int contribution;
	int lineid;
	int devotion;
	int reborncount;
	int cultivation;
	int64_t fac_coupon_add;

	ACString nickname;

public:

	int GetID() const { return roleid; }
	int GetFamilyID() const { return familyid; }
	int GetFRoleID() const { return froleid; }
	int GetLev() const { return lev; }
	int GetProf() const { return prof; }
	int GetContribution() const { return contribution; }
	int GetLineId() const { return lineid; }
	const ACHAR* GetName() const;
	const ACHAR* GetNickName() const { return nickname; }
	int GetDevotion() const { return devotion; }
	int GetRebornCount() const { return reborncount; }
	int GetCultivation() const { return cultivation; }
	int64_t GetFacCouponAdd() const { return fac_coupon_add; }

	void SetID(int id) { roleid = id; }
	void SetFamilyID(int id) { familyid = id; }
	void SetFRoleID(int id) { froleid = id; }
	void SetLev(int l) { lev = l; }
	void SetProf(int p) { prof = p; }
	void SetContribution(int l) { contribution = l; }
	void SetLineId(int l) { lineid = l; }
	void SetName(const void* s, int len);
	void SetName(const ACHAR* szName);
	void SetNickName(const void* s, int len) { _cp_str(nickname, s, len); }
	void SetDevotion(int i) { devotion = i; }
	void SetRebornCount(int i) { reborncount = i; }
	void SetCultivation(int i) { cultivation = i; }
	void SetFacCouponAdd(int64_t val) { fac_coupon_add = val; }
};

typedef abase::vector<Faction_Mem_Info*> FactionMemList;
typedef abase::vector<unsigned int> FamilyList;
typedef abase::vector<GNET::FamilySkill> FamilySkillList;
#define FAMILY_RECORD_COUNT 9

class CECFactionMan
{
public:

	CECFactionMan() : m_MemMap(256), m_nCurPage(0), m_nDeleteTime(0), m_nFamilyDeleteTime(0)
	{
		m_MemList.reserve(256);
		m_FamilyList.reserve(64);
		memset(m_FamilyRecords, 0, sizeof(m_FamilyRecords));
	}

	~CECFactionMan() { Release(); }
	void Release(bool bOnlyFreeMember = false);

protected:

	typedef abase::hash_map<int, Faction_Mem_Info*> FactionMemMap;

	FactionInfoMap m_FactionMap;
	FactionInfoMap m_FamilyMap;
	FactionMemList m_MemList;
	FactionMemMap m_MemMap;
	FamilyList m_FamilyList;
	ACString m_strProclaim;
	ACString m_strFamilyProclaim;
	int m_nCurPage;
	int m_nDeleteTime;
	int m_nFamilyDeleteTime;
	FamilySkillList m_FamilySkillList;
	int m_FamilyRecords[FAMILY_RECORD_COUNT];

protected:

	void AddFixedMsg(int iMsg);
	bool RemoveMember(int iRoleId);
	bool RemoveFamily(int iFamily);

	bool HasFamily(int iFamily)
	{
		for (size_t i = 0; i < m_FamilyList.size(); i++)
			if ((int)m_FamilyList[i] == iFamily)
				return true;

		return false;
	}

	void ClearHostFaction(bool family);
	const ACHAR* GetMemName(int iRoleId);
	const ACHAR* GetMemNickName(int iRoleId);

public:

	// Operations
	Faction_Info* GetFaction(unsigned int uId, bool bRequestFromServer = false, bool bFamily = false);
	void RefreshMemList(int nPage, unsigned int familyid = 0, bool first_time = false);
	FactionMemList& GetMemList() { return m_MemList; }
	FamilyList& GetFamilyList() { return m_FamilyList; }
	FamilySkillList& GetFamilySkillList() { return m_FamilySkillList; }
	const ACHAR* GetProclaim() const { return m_strProclaim; }
	const ACHAR* GetFamilyProclaim() const { return m_strFamilyProclaim; }
	int GetCurPage() const { return m_nCurPage; }
	int GetDeleteTime() const { return m_nDeleteTime; }
	int GetFamilyDeleteTime() const { return m_nFamilyDeleteTime; }
	int GetFamilyRecord(int nIndex) { return m_FamilyRecords[nIndex]; }
	void SetFamilyRecord(int nIndex, int nValue) { m_FamilyRecords[nIndex] = nValue; }

	Faction_Info* AddFaction(unsigned int uId, bool bFamily, const void* name, int len, int nLev, int iScore, int nMemNum)
	{
		FactionInfoMap& map = bFamily ? m_FamilyMap : m_FactionMap;
		FactionInfoMap::iterator it = map.find(uId);
		Faction_Info* p = (it == map.end() ? (new Faction_Info) : it->second);
		p->SetID(uId);
		p->SetName(name, len);
		p->SetLevel(nLev);
		p->SetMemNum(nMemNum);
		p->SetProsperity(0);
		p->SetOnlineMemNum(0);
		p->SetNimbus(0);
		p->SetTerritoryScore(iScore);
		map[uId] = p;
		return p;
	}

	void ResetMemInfoList()
	{
		for (size_t i = 0; i < m_MemList.size(); i++)
			delete m_MemList[i];

		m_MemList.clear();
		m_MemMap.clear();
	}

	Faction_Mem_Info* GetMember(int nRoleId)
	{
		FactionMemMap::iterator it = m_MemMap.find(nRoleId);
		return it == m_MemMap.end() ? NULL : it->second;
	}

	static const ACHAR* GetFRoleName(unsigned int iFRoleId);
	bool marshal(GNET::Marshal::OctetsStream& os);
	bool unmarshal(GNET::Marshal::OctetsStream& os);

	// Msgs
	void OnFactionCreate(const FactionCreate_Re* p);
	void OnBroadcastNotice(const FactionAnnounce_Re* p);
	DWORD OnChat(const FactionChat* p);
	DWORD OnAcceptJoin(const FactionRecruit_Re* p);
	DWORD OnExpel(const FactionExpel_Re* p);
	DWORD OnResign(const FactionResign_Re* p);
	DWORD OnAppoint(const FactionAppoint_Re* p);
	DWORD OnLeave(const FactionLeave_Re* p);
	DWORD OnRename(const FactionNickname_Re* p);
	void OnUpgrade(const FactionUpgrade_Re* p);
	void OnListMember(const FactionList_Re* p);
	void OnBaseInfo(const GetFactionBaseInfo_Re* p);
	void OnDismiss(const FactionDismiss_Re*);
	void OnInviteJoin(const FactionInvite* p);
	void OnInviteCombat(const CombatInvite* p);
	void OnUpdateTerritoryScore(const TerritoryScoreUpdate* p);
	void OnFactionChangeName(const ChangeFactionName_Re* p);
	void OnCrssvrTeamsJoin(const CrssvrTeamsInvite* p);

	//	Check whether specified protocol can be processed
	bool CanProcessPtrc(GNET::Protocol* pProtocol);
};

