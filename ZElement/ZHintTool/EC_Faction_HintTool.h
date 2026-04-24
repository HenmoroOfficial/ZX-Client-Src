//
//
//
//
#pragma once
//-----------------------------------------------------------------------------
#include "hashmap.h"


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

	Faction_Info() : m_nNimbus(0), m_nNimBusStar(0) {}
	~Faction_Info() {}

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
//	bool marshal(GNET::Marshal::OctetsStream& os);
//	bool unmarshal(GNET::Marshal::OctetsStream& os);
};


typedef abase::hash_map<unsigned int, Faction_Info*> FactionInfoMap;
//-----------------------------------------------------------------------------
class CECFactionMan
{
public:
	//
	void Release(bool bOnlyFreeMember = false)
	{
		if (!bOnlyFreeMember)
		{
			FactionInfoMap::iterator it = m_FactionMap.begin();

			for (; it != m_FactionMap.end(); ++it)
				delete it->second;

			m_FactionMap.clear();
		}

	};

	CECFactionMan()
	{
	}

	~CECFactionMan() { Release(); }
//	void Release(bool bOnlyFreeMember = false);


protected:
		FactionInfoMap m_FactionMap;

public:

	// Operations
	Faction_Info* GetFaction(unsigned int uId, bool bRequestFromServer = false, bool bFamily = false)
	{
//		FactionInfoMap& map = bFamily ? m_FamilyMap : m_FactionMap;
		FactionInfoMap::iterator it = m_FactionMap.find(uId);

		if (it == m_FactionMap.end())
		{
			if (bRequestFromServer)
//				g_pGame->GetGameSession()->GetFactionInfo(1, reinterpret_cast<int*>(&uId), bFamily ? 1 : 0);
				;

			return NULL;
		}

		return it->second;
	}


};