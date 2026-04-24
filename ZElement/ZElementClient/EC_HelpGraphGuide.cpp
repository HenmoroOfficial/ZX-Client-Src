/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpGraphGuide.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIDialog.h"
#include "TaskProcess.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_Inventory.h"
#include "EC_Secretary.h"
#include "EC_TaskInterface.h"
#include "EC_HelpStep00Start.h"
#include "EC_HelpStep01Move.h"
#include "EC_HelpStep02Equip.h"
#include "EC_HelpStep03Task.h"
#include "EC_HelpStep04Trace.h"
#include "EC_HelpStep05Attack.h"
#include "EC_HelpStep06Medic.h"
#include "EC_HelpStep07Talisman.h"
#include "EC_HelpStep08Map.h"
#include "EC_HelpStep09Skill.h"
#include "EC_HelpStep10Transfer.h"
#include "EC_HelpStep11TaskZhuxian.h"

#define  SECRETARY_VERSION 4

const int	CECHelpGraphGuide::ms_iCheckHelpPeriod = 1000;

CECHelpGraphGuide::CECHelpGraphGuide()
{
	Reset();
	CreateHelpSteps();

	m_iTimer = 0;
	m_bLockAction	= false;
}

CECHelpGraphGuide::~CECHelpGraphGuide()
{
	int i;
	for (i=0; i<HELP_STAGE_NUM; ++i)
	{
		delete m_pHelpSteps[i];
	}
}

void CECHelpGraphGuide::Reset()
{
	memset(m_aHelpState, 0, HELP_GRAPHGUIDE_NUM);
	m_iCurHelpStep = -1;
	m_iCurHelpStage = -1;
}

void CECHelpGraphGuide::CreateHelpSteps()
{
	m_pHelpSteps[0] = new CECHelpStep00Start();
	m_pHelpSteps[1] = new CECHelpStep01Move();
	m_pHelpSteps[2] = new CECHelpStep02Equip();
	m_pHelpSteps[3] = new CECHelpStep03Task();
	m_pHelpSteps[4] = new CECHelpStep05Attack();
	m_pHelpSteps[5] = new CECHelpStep06Medic();
	m_pHelpSteps[6] = new CECHelpStep07Talisman();
	m_pHelpSteps[7] = new CECHelpStep08Map();
	m_pHelpSteps[8] = new CECHelpStep09Skill();
	m_pHelpSteps[9] = new CECHelpStep010Transfer();
	m_pHelpSteps[10] = new CECHelpStep11TaskZhuxian();
}

void CECHelpGraphGuide::Tick(unsigned long dwDeltaTime)
{
	return;
	// g_pGame->GetGameRun()->GetHostPlayer() && g_pGame->GetGameRun()->GetHostPlayer()->HostIsReady();
	if (!IsHelpOpen())
	{
		m_bLockAction = false;
		return;
	}

	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NewerHelp_VisionLockHint")->Show(m_bLockAction);

	m_iTimer += dwDeltaTime;
	// check triggler help or not 
	if (m_iTimer>ms_iCheckHelpPeriod)
	{
		m_iTimer = 0;
		if (!GetState(HELP2012_0_1_OPEN_ORNOT))
		{
			StartHelp(HELP2012_0_1_OPEN_ORNOT);
		}
		else if (GetState(HELP2012_1_1_WASD_MOVE) && !GetState(HELP2012_2_1_PACKAGE) && m_iCurHelpStage==-1)
		{
			StartHelp(HELP2012_2_1_PACKAGE);
		}
		else if (GetState(HELP2012_2_3_WEAPON))
		{
			abase::vector<int> vecIds;
			_GenerateCurTasks(vecIds);
			CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
			int idInstance = g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
			for( unsigned int i = 0; i < vecIds.size(); i++ )
			{
				// StartHelp(HELP2012_3_1_TRACK);
				if((vecIds[i]==23357 || vecIds[i]==23507) && !GetState(HELP2012_3_1_TRACK) && m_iCurHelpStep!=HELP2012_3_1_TRACK)
				{
					StartHelp(HELP2012_3_1_TRACK);
					break;
				}
				// StartHelp(HELP2012_4_1_TRACK);
/*				else if(vecIds[i]==23476 && GetState(HELP2012_3_4_TASK_UI) && !GetState(HELP2012_4_1_TRACK))
				{
					StartHelp(HELP2012_4_1_TRACK);
					break;
				}
*/				// StartHelp(HELP2012_5_1_ATTACK);
				else if((vecIds[i]==23477 || vecIds[i]==23818) && !GetState(HELP2012_5_1_ATTACK) && GetState(HELP2012_3_4_TASK_UI))
				{
					A3DRECT rc(229, 36, 241, 66);
					A3DVECTOR3 pos = pHost->GetPos();
					
					if (idInstance==1 && rc.PtInRect(int(pos.x), int(pos.z)) && m_iCurHelpStep!=HELP2012_5_1_ATTACK)
					{
						StartHelp(HELP2012_5_1_ATTACK);
						break;
					}
				}
				// StartHelp(HELP2012_6_1_MEDICINE_INV);
				else if((vecIds[i]==23481 || vecIds[i]==23547) && GetState(HELP2012_5_1_ATTACK))
				{
					if(pHost->GetPack()->FindItem(3999)>=0 && !GetState(HELP2012_6_1_MEDICINE_INV))
					{
						StartHelp(HELP2012_6_1_MEDICINE_INV);
						break;
					}
				}
				// StartHelp(HELP2012_8_MAP_HOTKEY);
				else if((vecIds[i]==23362 || vecIds[i]==23635) && GetState(HELP2012_7_5_TALISMAN_START))
				{
					if(idInstance==33 && !GetState(HELP2012_8_1_MAP_HOTKEY) && m_iCurHelpStep!=HELP2012_8_1_MAP_HOTKEY)
					{
						StartHelp(HELP2012_8_1_MAP_HOTKEY);
						break;
					}
				}
				// StartHelp(HELP2012_9_SKILL_ICON);
				else if((vecIds[i]==23391 || vecIds[i]==23567) && GetState(HELP2012_8_2_MAP_NPCMINE_LIST) && !GetState(HELP2012_9_1_SKILL_ICON)
					 && m_iCurHelpStep!=HELP2012_9_1_SKILL_ICON)
				{
					StartHelp(HELP2012_9_1_SKILL_ICON);
					break;
				}
				// StartHelp(HELP2012_10_TRANSFER);
				else if((vecIds[i]==23410 || vecIds[i]==23635) && GetState(HELP2012_9_3_SKILL_SHORTCUT) && !GetState(HELP2012_10_1_TRANSFER)
					&& m_iCurHelpStep!=HELP2012_10_1_TRANSFER)
				{
					StartHelp(HELP2012_10_1_TRANSFER);
					break;
				}
			}
			// StartHelp(HELP2012_7_1_TALISMAN_INV);
			if(pHost->GetPack()->FindItem(4703)>=0 && GetState(HELP2012_6_2_MEDICINE_HINT) && !GetState(HELP2012_7_1_TALISMAN_INV)
				&& m_iCurHelpStep!=HELP2012_7_1_TALISMAN_INV)
			{
				StartHelp(HELP2012_7_1_TALISMAN_INV);
			}
			// StartHelp(HELP2012_11_TASK_UI);
			else if ( idInstance==2 && GetState(HELP2012_10_2_TRANSFER_DESTPOS) && !GetState(HELP2012_11_1_TASK_UI) )
			{
				FinishedTaskList* pTaskFinished = static_cast<FinishedTaskList*>(pHost->GetTaskInterface()->GetFinishedTaskList());
				if (pTaskFinished->SearchTask(23406)==0 && m_iCurHelpStep!=HELP2012_11_1_TASK_UI)
				{
					StartHelp(HELP2012_11_1_TASK_UI);
				}
			}
		}
	}
}

void CECHelpGraphGuide::_GenerateCurTasks(abase::vector<int> &vecIds)
{
	vecIds.clear();
	
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECTaskInterface *pTask = pHost->GetTaskInterface();
	unsigned int iSize = pTask->GetTaskCount();
	unsigned int i;
	for (i=0; i<iSize; ++i)
	{
		vecIds.push_back(pTask->GetTaskId(i));
		_InsertSubTask(pTask->GetTaskId(i), vecIds);
	}
}

void CECHelpGraphGuide::_InsertSubTask(int id, abase::vector<int> &vecIds)
{
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECTaskInterface *pTask = pHost->GetTaskInterface();
	int nPos = pTask->GetFirstSubTaskPosition(id);
	while( nPos > -1 )
	{
		id = pTask->GetNextSub(nPos);
		if( id==0 ) break;

		vecIds.push_back(id);
		_InsertSubTask(id, vecIds);
	}

}

void CECHelpGraphGuide::SetState(int index)
{
	if (index >= HELP2012_NUM)
		return;

	int i, j;
	i = index/8;
	j = index%8;
	m_aHelpState[i] |= (1<<j);
}

void CECHelpGraphGuide::ClearState(int index)
{
	if (index >= HELP2012_NUM)
		return;
	
	int i, j;
	i = index/8;
	j = index%8;
	m_aHelpState[i] &= ~(1<<j);

}

bool CECHelpGraphGuide::GetState(int index)
{
	if (index >= HELP2012_NUM)
		return false;

	int i, j;
	i = index/8;
	j = index%8;
	if (m_aHelpState[i] & (1<<j))
		return true;
	return false;
}

void CECHelpGraphGuide::StartHelp(int index)
{
	// 开始的提示，逻辑不大相同，点了按钮以后才算完成这步帮助，所以没有放到统一的逻辑里
	int indexHelp = GetIndex(index);

	// 一个阶段帮助结束才能进行另一个，要不会乱
	if (m_iCurHelpStage!=indexHelp && m_iCurHelpStage!=-1)
	{
		return;
	}

	if (!m_pHelpSteps[indexHelp])
	{
		return;
	}
	m_iCurHelpStep = index;
	m_iCurHelpStage = indexHelp;
	if(index!=HELP2012_0_1_OPEN_ORNOT)
		m_bLockAction = true;
	m_pHelpSteps[indexHelp]->StartHelp(index);
}

void CECHelpGraphGuide::FinishHelp(int index, bool bSave)
{
	if (m_iCurHelpStep!=index)
		return;
	
	SetState(index);
	int indexHelp = GetIndex(index);
	if (!m_pHelpSteps[indexHelp])
	{
		return;
	}
	m_bLockAction = false;
	m_pHelpSteps[indexHelp]->FinishHelp(index);
	if (bSave)
	{
		g_pGame->GetGameRun()->GetSecretary()->SaveStatesToServer();
	}
}

int CECHelpGraphGuide::GetIndex(int index)
{
	int indexHelp = -1;
	if (index>=HELP2012_0_1_OPEN_ORNOT && index<=HELP2012_0_1_OPEN_ORNOT)
	{
		indexHelp = HELP_STAGE_START;
	}
	else if (index>=HELP2012_1_1_WASD_MOVE && index<=HELP2012_1_1_WASD_MOVE)
	{
		indexHelp = HELP_STAGE_MOVE;
	}
	else if (index>=HELP2012_2_1_PACKAGE && index<=HELP2012_2_3_WEAPON)
	{
		indexHelp = HELP_STAGE_EQUIP;
	}
	else if (index>=HELP2012_3_1_TRACK && index<=HELP2012_3_5_TASK_LIST)
	{
		indexHelp = HELP_STAGE_TASK;
	}
	else if (index>=HELP2012_5_1_ATTACK && index<=HELP2012_5_1_ATTACK)
	{
		indexHelp = HELP_STAGE_ATTACK;
	}
	else if (index>=HELP2012_6_1_MEDICINE_INV && index<=HELP2012_6_2_MEDICINE_HINT)
	{
		indexHelp = HELP_STAGE_MEDICINE;
	}
	else if (index>=HELP2012_7_1_TALISMAN_INV && index<=HELP2012_7_5_TALISMAN_START)
	{
		indexHelp = HELP_STAGE_TALISMAN;
	}
	else if (index>=HELP2012_8_1_MAP_HOTKEY && index<=HELP2012_8_2_MAP_NPCMINE_LIST)
	{
		indexHelp = HELP_STAGE_MAP;
	}
	else if (index>=HELP2012_9_1_SKILL_ICON && index<=HELP2012_9_3_SKILL_SHORTCUT)
	{
		indexHelp = HELP_STAGE_SKILL;
	}
	else if (index>=HELP2012_10_1_TRANSFER && index<=HELP2012_10_2_TRANSFER_DESTPOS)
	{
		indexHelp = HELP_STAGE_TRANSFER;
	}
	else if (index>=HELP2012_11_1_TASK_UI && index<=HELP2012_11_5_TASK_DELIVER)
	{
		indexHelp = HELP_STAGE_TASK_ZHUXIAN;
	}

	return indexHelp;
}

void CECHelpGraphGuide::FinishHelpAction(int index)
{
	SetState(index);
	
	int indexHelp = GetIndex(index);
	static const char* names[] = {
		"",
		"界面\\移动篇.gfx",
		"界面\\装备篇.gfx",
		"界面\\任务篇.gfx",
		"界面\\战斗篇.gfx",
		"界面\\药品篇.gfx",
		"界面\\法宝篇.gfx",
		"界面\\地图篇.gfx",
		"界面\\技能篇.gfx",
		"",
		"",
	};
	if (indexHelp>=0)
	{
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NewerHelp_Gfx")->GetDlgItem("Gfx_1");
		pImg->SetGfx(names[indexHelp]);
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_NewerHelp_Gfx")->Show(true);
	}
	m_iCurHelpStep = -1;
	m_iCurHelpStage = -1;
}

unsigned char* CECHelpGraphGuide::GetSaveData()
{
	return m_aHelpState;
}

void CECHelpGraphGuide::LoadData(void* pDataBuf, int iSize)
{
	// 换了角色登陆，需要reset
	Reset();

	if(!pDataBuf || !iSize)
		return ;
	if(iSize != HELP_GRAPHGUIDE_NUM)
		return;

	memcpy(m_aHelpState, pDataBuf, HELP_GRAPHGUIDE_NUM);

	// 第3步任务帮助掉线重入特殊处理
	if (GetState(HELP2012_3_1_TRACK) && (!GetState(HELP2012_3_2_FIRSTTASK) || !GetState(HELP2012_3_3_TASK_ACCEPT)))
	{
		ClearState(HELP2012_3_1_TRACK);
		ClearState(HELP2012_3_2_FIRSTTASK);
		ClearState(HELP2012_3_3_TASK_ACCEPT);
	}

	if (IsHelpOpen())
	{
		int i;
		for (i=HELP2012_0_1_OPEN_ORNOT; i<=HELP2012_11_5_TASK_DELIVER; ++i)
		{
			if (!GetState(i))
			{
				if (i!=HELP2012_0_1_OPEN_ORNOT &&
					i!=HELP2012_1_1_WASD_MOVE &&
					i!=HELP2012_2_1_PACKAGE &&
					i!=HELP2012_3_1_TRACK &&
					i!=HELP2012_5_1_ATTACK &&
					i!=HELP2012_6_1_MEDICINE_INV &&
					i!=HELP2012_7_1_TALISMAN_INV &&
					i!=HELP2012_8_1_MAP_HOTKEY &&
					i!=HELP2012_9_1_SKILL_ICON &&
					i!=HELP2012_10_1_TRANSFER &&
					i!=HELP2012_11_1_TASK_UI)
				{
					StartHelp(i);
				}
				break;
			}
		}
	}
}

bool CECHelpGraphGuide::IsHelpOpen()
{
	return g_pGame->GetGameRun()->GetHostPlayer()->GetBasicProps().iLevel<=15 && !GetState(HELP2012_CLOSE);
}

bool CECHelpGraphGuide::DealWndProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if (!m_bLockAction || m_iCurHelpStage<0)
	{
		return false;
	}

	if (g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetWindowMessageType(message)==WINDOWMESSAGE_TYPE_KEYBOARD)
	{
		if( m_bLockAction && message==WM_KEYDOWN && wParam==VK_ESCAPE )
		{
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Win_MsgBoxYesNoeCheck", "asdf", 0, 0);
			m_bLockAction = false;
			return true;
		}

		return !m_pHelpSteps[m_iCurHelpStage]->DealKeyMsg();
	}

	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);

	
	A3DRECT rc = m_pHelpSteps[m_iCurHelpStage]->GetValidArea();
	
	if (rc.PtInRect(x, y))
	{
		return false;
	}
	else
		return true;
}

void CECHelpGraphGuide::OpenInv()
{
	if (!IsHelpOpen())
		return;
	
	if (m_iCurHelpStep==-1)
		return;
	else if (m_iCurHelpStep==HELP2012_2_1_PACKAGE || 
		m_iCurHelpStep==HELP2012_6_1_MEDICINE_INV ||
		m_iCurHelpStep==HELP2012_7_1_TALISMAN_INV)
	{
		FinishHelp(m_iCurHelpStep);
	}
}

void CECHelpGraphGuide::UseItem(int id)
{
	if (!IsHelpOpen())
		return;

	if (m_iCurHelpStage==-1)
		return;

	m_pHelpSteps[m_iCurHelpStage]->UseItem(id, m_iCurHelpStep);
}

void CECHelpGraphGuide::SkipCurStage()
{
	int index = m_iCurHelpStep;
	int iEndIndex = -1;
	
	if (index>=HELP2012_1_1_WASD_MOVE && index<=HELP2012_1_1_WASD_MOVE)
	{
		iEndIndex = HELP2012_1_1_WASD_MOVE;
	}
	else if (index>=HELP2012_2_1_PACKAGE && index<=HELP2012_2_3_WEAPON)
	{
		iEndIndex = HELP2012_2_3_WEAPON;
	}
	else if (index>=HELP2012_3_1_TRACK && index<=HELP2012_3_5_TASK_LIST)
	{
		iEndIndex = HELP2012_3_5_TASK_LIST;
	}
	else if (index>=HELP2012_5_1_ATTACK && index<=HELP2012_5_1_ATTACK)
	{
		iEndIndex = HELP2012_5_1_ATTACK;
	}
	else if (index>=HELP2012_6_1_MEDICINE_INV && index<=HELP2012_6_2_MEDICINE_HINT)
	{
		iEndIndex = HELP2012_6_2_MEDICINE_HINT;
	}
	else if (index>=HELP2012_7_1_TALISMAN_INV && index<=HELP2012_7_5_TALISMAN_START)
	{
		iEndIndex = HELP2012_7_5_TALISMAN_START;
	}
	else if (index>=HELP2012_8_1_MAP_HOTKEY && index<=HELP2012_8_2_MAP_NPCMINE_LIST)
	{
		iEndIndex = HELP2012_8_2_MAP_NPCMINE_LIST;
	}
	else if (index>=HELP2012_9_1_SKILL_ICON && index<=HELP2012_9_3_SKILL_SHORTCUT)
	{
		iEndIndex = HELP2012_9_3_SKILL_SHORTCUT;
	}
	else if (index>=HELP2012_10_1_TRANSFER && index<=HELP2012_10_2_TRANSFER_DESTPOS)
	{
		iEndIndex = HELP2012_10_2_TRANSFER_DESTPOS;
	}
	else if (index>=HELP2012_11_1_TASK_UI && index<=HELP2012_11_5_TASK_DELIVER)
	{
		iEndIndex = HELP2012_11_5_TASK_DELIVER;
	}

	int i;
	for (i=index; i<=iEndIndex; ++i)
	{
		FinishHelp(i, false);
	}
	g_pGame->GetGameRun()->GetSecretary()->SaveStatesToServer();
}

void CECHelpGraphGuide::ResumeHelp()
{
	m_bLockAction = true;
}