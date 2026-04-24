// Filename	: EC_BattleDungeonTower.cpp
// Creator	: Fu Chonggang
// Date		: 2012.4.20

#include "EC_BattleDungeonTower.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_BattleDungeon.h"
#include "elementdataman.h"
#include "EC_GPDataType.h"
#include "AWScriptFile.h"
#include "achar.h"
#include "AUI\\AUICTranslate.h"

const int CECBattleDungeonTower::TOWER_TEMP_ID = 543;

CECBattleDungeonTower::CECBattleDungeonTower()
: m_vecMonstersBook(256, 0)
{
	m_iMapId = -1;
	m_iTowerLevel = -1;
	m_iTodayStart = -1;
	m_iTodayDest  = -1;

	memset(m_iTimeUse, 0, sizeof(m_iTimeUse));
}

CECBattleDungeonTower::~CECBattleDungeonTower()
{
}

bool CECBattleDungeonTower::Init()
{
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = GetConfig();
	m_iMapId = pConfig->map_id;
	
	bool bval = true;
	AWScriptFile s;
	
	bval = s.Open("Configs\\monster_desc.txt");
	if( !bval ) return false;
	
	while( !s.IsEnd() )
	{
		bval = s.GetNextToken(true);
		if( !bval ) break;		// End of file.
		int idString = a_atoi(s.m_szToken);
		
		bval = s.GetNextToken(true);
		if( !bval ) return false;
		
		AUICTranslate tran;
		ACString str(tran.Translate(s.m_szToken));
		m_ItemDesc[idString] = str;
	}
	
	s.Close();

	return true;
}

const TOWER_TRANSCRIPTION_CONFIG* CECBattleDungeonTower::GetConfig()
{
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	const void* p = pDB->get_data_ptr(TOWER_TEMP_ID, ID_SPACE_CONFIG, DataType);
	const TOWER_TRANSCRIPTION_CONFIG* pConfig = (const TOWER_TRANSCRIPTION_CONFIG*)p;
	assert(DataType==DT_TOWER_TRANSCRIPTION_CONFIG);
	return pConfig;
}

int CECBattleDungeonTower::GetMapId()	
{
	return m_iMapId; 
}

void CECBattleDungeonTower::SetCurLevel(int lv) 
{ 
	m_iCurLevel=lv;
	if (m_iTodayDest<(m_iCurLevel-1))
		m_iTodayDest = m_iCurLevel-1;
	if (m_iTowerLevel<(m_iCurLevel-1))
		m_iTowerLevel = m_iCurLevel-1;
}

int CECBattleDungeonTower::GetTimeUsed(int index)
{
	return m_iTimeUse[index]; 
}

int CECBattleDungeonTower::GetMonster(int index) 
{ 
	if(index>=0&&index<(int)m_vecMonstersBook.size())
		return m_vecMonstersBook[index]; 
	return 0;
}

int CECBattleDungeonTower::GetMonsterCount() 
{
	return m_vecMonstersBook.size(); 
}

const wchar_t* CECBattleDungeonTower::GetMonsterDesc(int id)
{
	return m_ItemDesc.find(id)==m_ItemDesc.end() ? NULL : m_ItemDesc[id];
}


bool CECBattleDungeonTower::AwardCanDel(int lv, bool daily)
{
	if (daily)
	{
		if (lv>m_iTodayDest		|| 
			lv<m_iTodayStart	||
			lv<(m_iTodayDest-9))
			return false;
		
		return m_vecAwardStates[lv].daily_award_delivered==0 &&
			!CECBattleDungeonMan::getSingletonPtr()->IsInTower();
	}
	else
	{
		if (lv>m_iTowerLevel || m_iTowerLevel<0)
		{
			return false;
		}
		else
			return m_vecAwardStates[lv].lifelong_award_delivered==0;
	}
}

void CECBattleDungeonTower::UpdateTowerData(void *p)
{
	S2C::player_tower_data *pData = (S2C::player_tower_data*)p;
	m_iTowerLevel	= pData->tower_level;
	m_iTodayStart	= pData->start_level_today;
	m_iTodayDest	= pData->end_level_today;
	int i;
	for (i=0; i<10; ++i)
	{
		m_iTimeUse[i] = pData->best_pass_time[i];
	}

	for (i=0; i<pData->award_count; ++i)
	{
		AwardState as;
		as.daily_award_delivered = pData->tower_award_state[i].daily_award_delivered ;
		as.lifelong_award_delivered = pData->tower_award_state[i].lifelong_award_delivered;
		m_vecAwardStates.push_back(as);
	}
}

void CECBattleDungeonTower::UpdateAwardState(void *p)
{
	S2C::set_tower_award_state *pData = (S2C::set_tower_award_state*)p;
	if (pData->level<(int)m_vecAwardStates.size())
	{
		if(pData->type==0)
			m_vecAwardStates[pData->level].daily_award_delivered = pData->delivered;
		else
			m_vecAwardStates[pData->level].lifelong_award_delivered = pData->delivered;
	}
}

void CECBattleDungeonTower::UpdateMonsters(void *p)
{
	elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE dt;
	S2C::player_tower_monster_list *pList = (S2C::player_tower_monster_list*)p;
	m_vecMonstersBook.clear();
	int i;
	for (i=0; i<pList->tower_monster_count; ++i)
	{
		MONSTER_ESSENCE *pEss = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
			pList->tower_monster_list[i], ID_SPACE_ESSENCE,dt); 
		if (dt != DT_MONSTER_ESSENCE)
		{
			continue ;
		}
		int j;
		for (j=0; j<(int)m_vecMonstersBook.size(); ++j)
		{
			if (pEss->big_icon==m_vecMonstersBook[j])
			{
				break;
			}
		}
		if(j==(int)m_vecMonstersBook.size())
			m_vecMonstersBook.push_back(pEss->big_icon);
	}
}

void CECBattleDungeonTower::PassOneLevel(void *p)
{
	S2C::tower_level_pass *pInfo = (S2C::tower_level_pass*)p;
	if (pInfo->level>m_iTowerLevel)
	{
		m_iTimeUse[pInfo->level/10] += pInfo->best_time;
	}

	//m_vecAwardStates[pInfo->level].daily_award_delivered = 0;
	SetCurLevel(pInfo->level+1);
}

void CECBattleDungeonTower::AddMonster(int id)
{
	elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE dt;
	MONSTER_ESSENCE *pEss = (MONSTER_ESSENCE*)pElementDataMan->get_data_ptr(
		id, ID_SPACE_ESSENCE,dt); 
	if (dt != DT_MONSTER_ESSENCE)
	{
		return ;
	}
	int i;
	for (i=0; i<(int)m_vecMonstersBook.size(); ++i)
	{
		if (pEss->big_icon==m_vecMonstersBook[i])
		{
			return;
		}
	}
	m_vecMonstersBook.push_back(pEss->big_icon);
}
