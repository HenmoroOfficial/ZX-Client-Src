#include "EC_BattleDungeon.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_Global.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_BattleDungeonTower.h"

#include "raidvoteresult.hpp"
#include "raidbeginvotes.hpp"
#include "ExpTypes.h"
#include "elementdataman.h"

CECBattleDungeon::CECBattleDungeon(int id)
{
	Init(id);
}

void CECBattleDungeon::Init(int id)
{
	m_iId = id;
	elementdataman* pElementDataMan = g_pGame->GetElementDataMan();
	DATA_TYPE dt;
	m_pConfigData = (TRANSCRIPTION_CONFIG*)pElementDataMan->get_data_ptr(
		m_iId, ID_SPACE_CONFIG,dt); 
	if (dt != DT_TRANSCRIPTION_CONFIG)
	{
		m_pConfigData = NULL;
	}

	if (m_pConfigData)
	{
		m_iMapId = m_pConfigData->map_id;
	}
	else
		m_iMapId = 0;
}


CECBattleDungeonMan CECBattleDungeonMan::ms_Singleton;

void CECBattleDungeonMan::Init(int id)
{
	if (id<0)
	{
		m_pCurBattle = 0;
		return;
	}
	
	if (CECBattleDungeonTower::TOWER_TEMP_ID==id)
	{
		m_pCurBattle = 0;
		m_bInTower = true;
		return;
	}

	m_bInTower = false;
	if ( m_mapBattleDungeon.find(id)==m_mapBattleDungeon.end() )
	{
		m_mapBattleDungeon[id] = new CECBattleDungeon(id);
	}

	m_pCurBattle = m_mapBattleDungeon[id];
}

void CECBattleDungeonMan::InitFromMapId(int id)
{
	if (g_pGame->GetGameRun()->GetHostPlayer()->GetBattleDungeonTower()->GetConfig()->map_id==id)
	{
		m_pCurBattle = 0;
		m_bInTower = true;
		return;
	}
	m_bInTower = false;
	elementdataman* pDB = g_pGame->GetElementDataMan();
	DATA_TYPE DataType = DT_INVALID;
	int tid = pDB->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while (tid)
	{
		const void* pData = pDB->get_data_ptr(tid, ID_SPACE_CONFIG, DataType);
		
		if (DataType == DT_TRANSCRIPTION_CONFIG)
		{
			const TRANSCRIPTION_CONFIG* pConf = (const TRANSCRIPTION_CONFIG*)pData;
			if (pConf->map_id==id)
			{
				Init(tid);
				break;
			}
		}
		
		//	Get next item
		tid = pDB->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}
}

CECBattleDungeon* CECBattleDungeonMan::GetBattle(int mapId)
{
	CECBattleDungeon *pRet = 0;

	BattleDungeonMap::iterator it = m_mapBattleDungeon.begin();
	for (; it!=m_mapBattleDungeon.end(); ++it)
	{
		CECBattleDungeon* p = it->second;
		if (p->m_iMapId==mapId)
		{
			pRet = p;
			break;
		}
	}
	return pRet;
}

void CECBattleDungeonMan::JoinBattle(int mapId)
{
	m_pInTeamBattle = 0;
	if ( mapId==-1 )
	{
		return;
	}

	BattleDungeonMap::iterator it = m_mapBattleDungeon.begin();
	for (; it!=m_mapBattleDungeon.end(); ++it)
	{
		CECBattleDungeon* p = it->second;
		if (p->m_iMapId==mapId)
		{
			m_pInTeamBattle = p;
			break;
		}
	}
}

void CECBattleDungeonMan::ReportError(int id)
{
	if (g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan()==CECUIManager::UIMAN_INGAME)
	{
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->MessageBox(11200+id);
	}
}
void CECBattleDungeonMan::ReportFactionPKError(int id)
{
	int errcode = id;
	//由于历史代码的限制，需要转换下错误码
	if(errcode >= ERR_RAID_NOT_VALID_FACTITLE && errcode <= ERR_RAID_FACTION_JOIN_GROUP)
		errcode = 4320+errcode-ERR_RAID_NOT_VALID_FACTITLE;
	ReportError(errcode);
}
void CECBattleDungeonMan::AddHint(void *pData)
{
	RaidVoteResult* p = (RaidVoteResult*)pData;
	ACString strText;
	ACString strName = g_pGame->GetGameRun()->GetPlayerName(p->voted_roleid, false);
	if (2*p->agree_number>p->total_number)
	{
		strText.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(15019),
			p->agree_number, p->total_number, strName);
	}
	else
	{
		strText.Format(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(15020), 
			p->agree_number, p->total_number, strName);
	}

	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
}
	
void CECBattleDungeonMan::KickPlayer(void *pData)
{
	RaidBeginVotes* p = (RaidBeginVotes*)pData;
	const ACHAR *pszName = g_pGame->GetGameRun()->GetPlayerName(p->voted_roleid, false);
	if (!pszName) 
		g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &p->voted_roleid, 4);
	else
	{
		DoKickPlayer(p->voted_roleid);
	}
}

void CECBattleDungeonMan::DoKickPlayer(int id)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strName = g_pGame->GetGameRun()->GetPlayerName(id, false);
	ACString strText;
	strText.Format(pGameUI->GetStringFromTable(15018), strName);
	pGameUI->CommonMessage("Win_Message9", AC2AS(strText), 45, 1);
	PAUIDIALOG pMsgBox = pGameUI->GetDialog("Win_Message9");
	pMsgBox->SetData(id);
}