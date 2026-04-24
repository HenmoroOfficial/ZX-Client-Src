/********************************************************************
	created:	2009/6/23
	author:		lihengju
	
	purpose:	use lua to contorl help system
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#include "EC_Secretary.h"
#include <AString.h>
#include <ABaseDef.h>
#include "LuaRankDisp.h"
#include "ScriptValue.h"
#include "LuaState.h"
#include "LuaScript.h"
#include "AUI\\AUILabel.h"

#include <AAssist.h>
#include "EC_Monster.h"
#include "ExpTypes.h"
#include <vector.h>

#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "LuaEvent.h"
#include "PWBrowser.h"
#include "DlgBaseExplorer.h"
#include "EC_IvtrConsume.h"
#include "EC_Inventory.h"
#include "EC_NewHelpGuide.h"

#define  SECRETARY_VERSION 5
 
CECSecretary::CECSecretary()
:m_pState(NULL)
{
	m_bSecretaryEnable = false;
	m_bHelpGuideEnable = false;
	Reset();
}

CECSecretary::~CECSecretary()
{
	Release();
}

void CECSecretary::Release()
{

}

void CECSecretary::Reset()
{
	memset(m_aFirstState, 0, sizeof(m_aFirstState));
	memset(m_aSpecialState, 0, sizeof(m_aSpecialState));
	memset(m_aTriggerOpenTask, 0, sizeof(m_aTriggerOpenTask));
	memset(m_aTriggerCompleteTask, 0, sizeof(m_aTriggerCompleteTask));
	memset(m_aGuideState,0,sizeof(m_aGuideState));
	
	m_iLastEvent = 0;
	m_backupExp = 0.0;
	m_bReady  = false;

	if(g_pGame->GetGameRun()->GetNewHelpGuide())
		g_pGame->GetGameRun()->GetNewHelpGuide()->Reset();
}

bool CECSecretary::Init()
{
	m_pState = g_LuaStateMan.GetAIState();
	if (!m_pState)
	{
		a_LogOutput(1, "CECSecretary::Init, null lua state\n");
		return false;
	}

	return true;
}

void CECSecretary::SetEventState(int iEvent)
{
	int i, j;
	i = iEvent/8;
	j = iEvent%8;
	if (i >= FIRST_STATE_BLOCK)
		return;
	m_aFirstState[i] |= (1<<j);
}

void CECSecretary::ResetEventState(int iEvent)
{
	int i, j;
	i = iEvent/8;
	j = iEvent%8;
	if (i >= FIRST_STATE_BLOCK)
		return;
	m_aFirstState[i] &= ~(1<<j);
}

bool CECSecretary::CheckEventState(int iEvent)
{
	int i, j;
	i = iEvent/8;
	j = iEvent%8;
	if (i>= FIRST_STATE_BLOCK)
		return false;
	if (m_aFirstState[i] & (1<<j))
		return false;
	return true;
}
void CECSecretary::SetGuideState(int iGuide)
{
	int i, j;
	i = iGuide/8;
	j = iGuide%8;
	if (i >= HELP_GUIDE_NUM)
		return;
	m_aGuideState[i] |= (1<<j);
}
bool CECSecretary::CheckGuideState(int iGuide)
{
	int i, j;
	i = iGuide/8;
	j = iGuide%8;
	if (i>= HELP_GUIDE_NUM)
		return false;
	if (m_aGuideState[i] & (1<<j))
		return false;
	return true;
}
void CECSecretary::ResetGuideState(int iGuide)
{
	int i, j;
	i = iGuide/8;
	j = iGuide%8;
	if (i >= HELP_GUIDE_NUM)
		return;
	m_aGuideState[i] &= ~(1<<j);
}
void CECSecretary::SetSpecialState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if (i >= SPECIAL_ID_BLOCK)
		return;
	m_aSpecialState[i] |= (1<<j);
}

bool CECSecretary::CheckSpecialState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if (i >= SPECIAL_ID_BLOCK)
		return false;
	if(m_aSpecialState[i] & (1<<j))
		return false;
	return true;
}

bool CECSecretary::CheckOpenTaskState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if(i >= TRIGGER_OPEN_TASK)
		return false;
	if(m_aTriggerOpenTask[i] & (1<<j))
		return false;

	return true;
}

bool CECSecretary::CheckCompleteTaskState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if(i >= TRIGGER_COMPLETE_TASK)
		return false;
	if(m_aTriggerCompleteTask[i] & (1<<j))
		return false;

	return true;
}

void CECSecretary::SetOpenTaskState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if (i >= TRIGGER_OPEN_TASK)
		return;
	m_aTriggerOpenTask[i] |= (1<<j);
}

void CECSecretary::SetCompleteTaskState(int iIndex)
{
	int i,j;
	i = iIndex/8;
	j = iIndex%8;
	if (i >= TRIGGER_COMPLETE_TASK)
		return;
	m_aTriggerCompleteTask[i] |= (1<<j);
}

bool CECSecretary::Tick(DWORD dwDeltaTime)
{
	if (!m_bReady)
		return true;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
//	if(pHost->IsJumping())
//		LuaEvent::PlayerIsJumping();

	if(pHost->IsAllResReady() && !pHost->IsPlayerMoving())
	{
		if(CheckEventState(CECSecretary::FIRST_ENTER_GAME))
			LuaEvent::FirstEnterGame();
	}
	if(pHost->IsAllResReady() && pHost->IsPlayerMoving())
	{
		LuaEvent::PlayerIsMoving();
	}
	if(pHost->IsTalkingWithNPC())
	{
		LuaEvent::FirstTalkNPC();
	}
	if(pHost->GetIconStateNum())
	{
		LuaEvent::FirstBUFF();
	}
	ROLEBASICPROP &rbp =	pHost->GetBasicProps();
	ROLEEXTPROP   &rep =    pHost->GetExtendProps();
	double rate = rbp.iCurHP/(double)rep.max_hp;
	if(rate < 0.5)
	{
		LuaEvent::FirstHalfHP();
	}
	if(m_backupExp > rbp.exp)
	{
		LuaEvent::FirstPunishDie();
	}
	else
		m_backupExp = rbp.exp;
	
	int inst = g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
	if(inst != 1 && inst != 23)
	{
		LuaEvent::FirstEnterNewMap();
	}
	
	if(pHost->GetCombatFaction())
	{
		LuaEvent::FirstFactionBattle();
	}

	if(pHost->IsInWar())
	{
		LuaEvent::FirstCityBattle();
	}
	if(pHost->GetRegionReputation(8) > 1500  && pHost->GetMaster() == pHost->GetCharacterID())
	{
		LuaEvent::IncDispileUpperLimit();
	}
	if(pHost->IsFlying())
	{
		LuaEvent::FirstFly();
	}
	if (pHost->GetCurPetIndex()>=0)
	{
		CECPetBedge *pPet = (CECPetBedge*)(pHost->GetPetPack()->GetItem(pHost->GetCurPetIndex()));
		if (pPet && pPet->GetEssence().level >= 60)
		{
			LuaEvent::FirstPetProduce();
		}
	}
	if (g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayerNum() > 40)
	{
		LuaEvent::FirstHideElsePlayer();
	}
	if (pHost->GetBasicProps().iDeityLevel > 0)
	{
		LuaEvent::FirstDeityLevelUp();
	}
	return true;
}

void CECSecretary::SaveStatesToServer()
{
	int iSize = sizeof(WORD) +		// version
				sizeof(WORD) +		// length
				sizeof(char) * FIRST_STATE_BLOCK +  // states
				sizeof(WORD) +      // length for special id
				sizeof(char) * SPECIAL_ID_BLOCK +
				sizeof(WORD) +		// length for trigger open task
				sizeof(char) * TRIGGER_OPEN_TASK +
				sizeof(WORD) +		// length for trigger complete task
				sizeof(char) * TRIGGER_COMPLETE_TASK +		
	            sizeof(WORD) +		// length for guide state
		        sizeof(char) * HELP_GUIDE_NUM;	
				

	void* pDataBuf = a_malloc(iSize);
	if (!pDataBuf)
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECSecretary::SaveStatesToServer", __LINE__);
		return;
	}
	BYTE* pData = (BYTE*)pDataBuf;
	*((WORD*)pData) = SECRETARY_VERSION;
	pData += sizeof(WORD);
	*((WORD*)pData) = FIRST_STATE_BLOCK;
	pData += sizeof(WORD);
	
	memcpy(pData, m_aFirstState, FIRST_STATE_BLOCK);
	pData += sizeof(char) * FIRST_STATE_BLOCK;

	*((WORD*)pData) = SPECIAL_ID_BLOCK;
	pData += sizeof(WORD);
	memcpy(pData, m_aSpecialState, SPECIAL_ID_BLOCK);
	pData += sizeof(char) * SPECIAL_ID_BLOCK;

	*((WORD*)pData) = TRIGGER_OPEN_TASK;
	pData += sizeof(WORD);
	memcpy(pData, m_aTriggerOpenTask, TRIGGER_OPEN_TASK);
	pData += sizeof(char) * TRIGGER_OPEN_TASK;

	*((WORD*)pData) = TRIGGER_COMPLETE_TASK;
	pData += sizeof(WORD);
	memcpy(pData, m_aTriggerCompleteTask, TRIGGER_COMPLETE_TASK);
	pData += sizeof(char) * TRIGGER_COMPLETE_TASK;

	*((WORD*)pData) = HELP_GUIDE_NUM;
	pData += sizeof(WORD);
	memcpy(pData, m_aGuideState, HELP_GUIDE_NUM);
	pData += sizeof(char) * HELP_GUIDE_NUM;

	g_pGame->GetGameSession()->SaveHelpStates(pDataBuf, iSize);
	a_free(pDataBuf);
}

void CECSecretary::LoadStatesFromServer(void* pDataBuf, int iSize)
{
	// 换了角色登陆，需要reset
	Reset();
	m_bReady = true;

	if(!pDataBuf || !iSize)
		return ;
	if(iSize < sizeof(WORD))
		return;

	BYTE * pData = (BYTE*)pDataBuf;
	int version = *(WORD*)pData;
// 	if(version > SECRETARY_VERSION)
// 		return;
//	if(version != SECRETARY_VERSION)
	if (version <= 2)
	{
		SaveStatesToServer();
		return;
	}
	pData += sizeof(WORD);
	int count = *(WORD*)pData;
	if (count < FIRST_STATE_BLOCK)
	{	
		ASSERT(0);
		return;
	}

	if( iSize < (sizeof(WORD) +sizeof(WORD) +sizeof(char) * FIRST_STATE_BLOCK) +sizeof(WORD) +sizeof(char)*SPECIAL_ID_BLOCK )
		return;

	pData += sizeof(WORD);
	memcpy(m_aFirstState, pData, FIRST_STATE_BLOCK);
	pData += sizeof(char)* FIRST_STATE_BLOCK;

	int specialCount = *(WORD*)pData;
	if (specialCount != SPECIAL_ID_BLOCK)
	{
		ASSERT(0);
		return;
	}

	pData += sizeof(WORD);
	memcpy(m_aSpecialState, pData, SPECIAL_ID_BLOCK);
	pData += sizeof(char) * SPECIAL_ID_BLOCK;

	// 加入了新手提示任务的任务掩码
	if (version > 3)
	{
		int opentaskCount = *(WORD*)pData;
		if (opentaskCount != TRIGGER_OPEN_TASK)
		{
			ASSERT(0);
			return;
		}
		pData += sizeof(WORD); 
		memcpy(m_aTriggerOpenTask, pData, TRIGGER_OPEN_TASK);
		pData += sizeof(char) * TRIGGER_OPEN_TASK;

		int completeTaskCount = *(WORD*)pData;
		if (completeTaskCount != TRIGGER_COMPLETE_TASK )
		{
			ASSERT(0);
			return;
		}
		pData += sizeof(WORD);
		memcpy(m_aTriggerCompleteTask, pData, TRIGGER_COMPLETE_TASK);
		pData += sizeof(char) * TRIGGER_COMPLETE_TASK;
	}
   
	// 加入新指引
	if(version > 4)
	{
		int guideCount = *(WORD*)pData;
		if (guideCount != HELP_GUIDE_NUM )
		{
			ASSERT(0);
			return;
		}
		pData += sizeof(WORD);
		memcpy(m_aGuideState, pData, HELP_GUIDE_NUM);
		pData += sizeof(char) * HELP_GUIDE_NUM;
	}

}

void CECSecretary::SetURL(const char* url, const ACHAR* title)
{
	CDlgBaseExplorer* pDlgExplorer = (CDlgBaseExplorer*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("win_explorer");
	PAUILABEL pLab = (PAUILABEL)pDlgExplorer->GetDlgItem("Title");
	if (title)
	{
		pLab->SetText(title);
	}
	else
		pLab->SetText(pDlgExplorer->GetStringFromTable(9126));
	pDlgExplorer->Show(true);
	pDlgExplorer->NavigateUrl(url);
}