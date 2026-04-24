/********************************************************************
	created:	2010/7/8
	author:		lihengju
	
	purpose:	try to rewrite the helper in c++, then we can seal the interfaces for the lua
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_Matter.h"
#include "EC_Monster.h"
#include "ExpTypes.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "EC_HPWork.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkSpell.h"
#include "EC_ManMatter.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Skill.h"
#include "EC_AutoMove.h"
#include "EC_ManNPC.h"
#include "EC_NPC.h"
#include "EC_GameRecord.h"


#include "EC_GameUIMan.h"
#include "DlgTalisman.h"
#include "EC_UIManager.h"

#include "A3DFuncs.h"

#define  LOCAL_DIR "userdata\\helper"
extern game_record gr;

int Instantcoolskill[] =
{
	393,	// 鬼王御空术
	394,	// 合欢御空术
	395,	// 青云御空术
	396,	// 天音御空术
	1104,	// 鬼道御空术
	1853,	// 九黎空行
	1895,	// 烈山空行
	2426,	// 怀光空行
	2477,	// 天华空行
	527,	// 苍劫阵
	534,	// 忘情
	544,	// 天机印
	553,	// 慈航法愿
	1130,	// 巫毒斗瘴
	1871,	// 战歌
	1913,	// 天衍阵
	2444,	// 杀域
	1533,	// 苍劫阵II
	1542,	// 慈航法愿II
	2218,	// 战歌II
	2063,	// 巫毒斗瘴II 
	2065,	// 天机印II
	2070,	// 慈航普渡II
	1569,	// 蛊王姿态II
	1595,	// 魔魂姿态II 
	1097,	// 魔魂姿态 
	1106,	// 蛊王姿态
	2188,	// 狐影术
	2446,	// 影遁
	4783,
	4784,
	4785,
	4786,
	4787,
	4792,
	4808,
	4814,
	4877,
	5266,
	5270,
	5274,
};

CECAutoHelper::CECAutoHelper()
:m_bTransferEnable(false),
 m_bRunning(false),
 m_bAnswerQuestion(false),
 m_pMonsterSelector(NULL),
 m_pMatterSelector(NULL)
{
	m_Counter.SetPeriod(200);
	m_TransferCounter.SetPeriod(1800000);
	m_PickUpCnt.SetPeriod(10000);
	for (int i = 0; i< NUM_COUNTER; ++i)
	{
		m_Counters[i].SetPeriod(10000);
		m_Counters[i].Reset();
	}
	m_Counters[Cnt_HealHP].SetPeriod(2000);
	m_Counters[Cnt_HealMP].SetPeriod(2000);
	m_Counters[Cnt_HealDP].SetPeriod(2000);
	m_Counters[Cnt_HealStamina].SetPeriod(2000);
	m_Counters[Cnt_AutoMove].SetPeriod(500);
	m_Counters[Cnt_ReportCnt].SetPeriod(10000);
	m_Counters[Cnt_ReportCnt].Reset();
	m_Counters[Cnt_Obstacle].SetPeriod(2000);
	m_Counters[Cnt_PickUp].SetPeriod(500);		// 500ms一次来检查是否有物品可以捡
	m_ObstacleCnt.SetPeriod(10);
	m_ObstacleCnt.Reset();
	m_PickUpAdjustCnt.SetPeriod(10000);
	m_PickUpAdjustCnt.Reset();
	m_LiuDaoSkillCnt.SetPeriod(2500);
	m_LiuDaoSkillCnt.SetPeriod(true);
	m_iHpRate = 70;
	m_iMpRate = 70;
	m_iDpRate = 70;
	m_fStaminaRate = 0.5f;
	m_iCurRangeChoosen = 2;
	m_fCurRange = 50.0f;
	m_iCurTargetHp = 0;
	m_iBadID	= 0;
	m_bPickingUp	= false;
	m_iPickupNum	= 0;
	m_iPickupTimes	= 0;
	m_bPickupAdjust	= false;
	m_bCastASkill	= false;
	m_iWaitSkill	= 0;
}

CECAutoHelper::~CECAutoHelper()
{
	Release();
}

void CECAutoHelper::Release()
{
	if (m_pMatterSelector)
	{
		delete m_pMatterSelector;
		m_pMatterSelector = NULL;
	}

	if (m_pMonsterSelector)
	{
		delete	m_pMonsterSelector;
		m_pMonsterSelector = NULL;
	}
}

bool CECAutoHelper::Init()
{
	m_pMonsterSelector = new CMonsterSelector;
	m_pMatterSelector = new CMatterSelector;
	
	return true;
}


void CECAutoHelper::TickFly(DWORD dwDelta)
{
	m_Counters[Cnt_HealStamina].IncCounter(dwDelta);
	m_Counters[Cnt_HealHP].IncCounter(dwDelta);
	m_Counters[Cnt_HealMP].IncCounter(dwDelta);
	m_Counters[Cnt_HealDP].IncCounter(dwDelta);
	Heal();
}

void CECAutoHelper::Tick(DWORD dwDelta)
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return;
	 
	if (g_pGame->GetGameRun()->GetHostPlayer()->IsFlying())
	{
		if (m_Counter.IncCounter(dwDelta))
		{
			TickFly(m_Counter.GetCounter());
			m_Counter.Reset();
			return;
		}
	}	

	if (m_bRunning)
	{
		if(m_bTransferEnable && m_TransferCounter.IncCounter(dwDelta) )
		{
			// 天人合一状态下，将背包中的特定物品定时转移到随身包裹中
			g_pGame->GetGameRun()->GetHostPlayer()->MoveItemPack2Pocket();
			m_TransferCounter.Reset();
		}
		
		if(!m_LiuDaoSkillCnt.IsFull())
			m_LiuDaoSkillCnt.IncCounter(dwDelta);

		if( m_Counter.IncCounter(dwDelta))
		{
			TickAuto(m_Counter.GetCounter());				
			m_Counter.Reset();
		}
	}
}

void CECAutoHelper::TickAuto(DWORD dwDelta)
{
	int i;
	for(i=0; i< NUM_COUNTER; ++i )
	{
		m_Counters[i].IncCounter(dwDelta);
	}
	// Report to svr
	if (m_Counters[Cnt_ReportCnt].IsFull())
	{
		g_pGame->GetGameSession()->bot_run();
		m_Counters[Cnt_ReportCnt].Reset();
	}


	if (m_bPickupAdjust)
	{
		if (m_PickUpAdjustCnt.IncCounter(dwDelta))
		{
			m_bPickupAdjust = false;
			m_PickUpAdjustCnt.Reset();
		}
	}

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return;

	// 技能时间置冷却 	
	SetSkillCooling();

	// 使用item
	UseItem();
	
	// 没有药而且设置0s补药的情况下，会导致玩家不进行攻击
	Heal();

	// 释放技能之后，等待两个天人合一tick
	if (m_bCastASkill)
	{
		// trace object, the skill has not been casted. 
		if (!IsPreparingSkill())
		{
			m_iWaitSkill += 1;
		}
		if (m_iWaitSkill <= 2)
		{
			return;
		}
		else if (m_iWaitSkill > 2)
		{
			// 很多的buff状态限制了使用技能，但是并没有限制普攻
			// 客户端并没有对这些buff状态进行逻辑判断的处理

			m_iWaitSkill = 0;
			m_bCastASkill = false;
			if (!IsUsingSkill() && !IsAttcking())
			{
				pHost->CmdNormalAttack();
				return;
			}
		}
	}

	if (IsOperating())
		return;
	if (IsAutoMoving())
		return;
	if (IsUsingSkill())
		return;
	if (IsPreparingSkill())
		return;
	if (IsPickingUp())
		return;

	// 优先捡东西，捡东西过程中避免切换到打怪状态（因为有服务器选怪）
	if (m_bPickingUp && m_Counters[Cnt_PickUp].IsFull() && !m_bPickupAdjust)
	{
		int pickupNum;
		if ( (pickupNum = GetDrops(m_fCurRange)) > 0)
		{
			if (pickupNum == m_iPickupNum)
			{
				m_iPickupTimes += 1;
// 				AString str;
// 				str.Format("left matter num %d\n", pickupNum);
// 				OutputDebugStringA(str);

				if (m_iPickupTimes > 8)
				{
					m_bPickupAdjust = true;
					m_iPickupTimes = 0;
					m_Counters[Cnt_PickUp].Reset();
					m_PickUpCnt.Reset();
					m_bPickingUp = false;
					return;
				}
			}
			else
			{
				m_iPickupNum = pickupNum;
				m_iPickupTimes = 0;
			}

			if (m_PickUpCnt.IncCounter(dwDelta))
			{
				m_Counters[Cnt_PickUp].Reset();
				m_PickUpCnt.Reset();
			}
			pHost->CmdPickup();
			return;
		}
		else
		{
			m_bPickingUp = false;
			m_Counters[Cnt_PickUp].Reset();
			m_PickUpCnt.Reset();
		}
	}
	else
		m_bPickingUp = false;


	// 目标的再选定
	// CannotAttack, host in the special state, cannot attack
	if (pHost->GetSelectedTarget() && (pHost->AttackableJudge(pHost->GetSelectedTarget(), false) == 1 || pHost->CannotAttack()) )
	{
		// 新目标的攻击，如果不能用技能 则使用普攻
		if (UseSkill())
		{
			return;
		}
		else
		{
//			if (m_bNewTarget || !IsAttcking())
			if (!IsAttcking())
			{
// 				if (m_bNewTarget)
// 					OutputDebugStringA("new target\n");
// 				else
// 					OutputDebugStringA("not new target\n");
// 				static int times = 0;
// 				AString str;
// 				str.Format(" normal attack %d \n", times++);
// 				OutputDebugStringA(str);

				m_bNewTarget = false;
				pHost->CmdNormalAttack();
			}
		}
	}
	else
	{
		// now we can pick up and auto move
		// 当前没有怪物的情况下，考虑捡东西和自动寻径
		// 捡东西，当前没有在攻击目标的间隙再去捡东西
		// 一次最多可以连续捡10s
		if (m_Counters[Cnt_PickUp].IsFull() && !m_bPickupAdjust)
		{
			if ((m_iPickupNum = GetDrops(m_fCurRange)) > 0)
			{
				if (m_PickUpCnt.IncCounter(dwDelta))
				{
					m_Counters[Cnt_PickUp].Reset();
					m_PickUpCnt.Reset();
				}
				pHost->CmdPickup();
				m_bPickingUp = true;
				return;
			}
			else
			{
				m_bPickingUp = false;
				m_Counters[Cnt_PickUp].Reset();
				m_PickUpCnt.Reset();
			}
		}
		
		// auto move, also process in the no monster state, before select the new target
		A3DVECTOR3 pos = pHost->GetPos();
		if (m_Counters[Cnt_AutoMove].IsFull())
		{
			m_Counters[Cnt_AutoMove].Reset();
			if (SquareMagnitude(pos - m_vStartPos) > m_fCurRange*m_fCurRange)
			{
				g_ECAutoMove.SearchPath(pos, m_vStartPos, g_pGame->GetGameRun()->GetWorld()->GetInstanceID());
				return;
			}
		}
		
		g_pGame->GetGameRun()->PostMessage(MSG_HST_AUTOSELECT, MAN_PLAYER, 0, 1);
		m_bNewTarget = true;
	}
	
	ProcessObstacleMonster(dwDelta);

}

bool CECAutoHelper::IsAutoMoving()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	return (work_id == CECHPWork::WORK_AUTOMOVE);
}

// 正要捡东西或者正在捡东西
bool CECAutoHelper::IsPickingUp()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	if (work_id == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace *)pHost->GetHPWorkMan()->GetCurWork();
		if (pWorkTrace && pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_PICKUP)
		{
			return true;
		}
	}
	if (work_id == CECHPWork::WORK_PICKUP)
	{
		return true;
	}
	return false;
}

int CECAutoHelper::GetDrops(float curRange)
{
	if (curRange > 10.0f)
		curRange = 10.0f;

	int count = 0;
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	if (pWorld && pWorld->GetMatterMan())
	{
		count = pWorld->GetMatterMan()->MatterInRange(curRange, true);
	}
	return count;
}

bool CECAutoHelper::IsUsingSkill()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	
// 	if (work_id == CECHPWork::WORK_TRACEOBJECT)
// 	{
// 		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace*)pHost->GetHPWorkMan()->GetCurWork();
// 		if (pWorkTrace && pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_SPELL)
// 		{
// 			return true;
// 		}
// 	}
	if (work_id == CECHPWork::WORK_SPELLOBJECT || !m_LiuDaoSkillCnt.IsFull())
	{
		return true;
	}
	// 没有释放成功，服务器没有返回，这个时候这个技能指针一直存在，就会导致人物没有反应
//	if (pHost->GetPrepSkill())
//		return true;

	return false;
}

bool CECAutoHelper::IsPreparingSkill()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	
	if (work_id == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace*)pHost->GetHPWorkMan()->GetCurWork();
		if (pWorkTrace && pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_SPELL)
		{
			return true;
		}
	}
	return false;
}

bool CECAutoHelper::IsOperating()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	return (work_id == CECHPWork::WORK_GENERAL_OPRT || work_id == CECHPWork::WORK_USEITEM);
}

bool CECAutoHelper::IsAttcking()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	if (work_id == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace * pWorkTrace = (CECHPWorkTrace *)pHost->GetHPWorkMan()->GetCurWork();
		if (pWorkTrace && (pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_ATTACK || pWorkTrace->GetTraceReason() == CECHPWorkTrace::TRACE_SPELL) )
		{
			return true;
		}
	}
	if (work_id == CECHPWork::WORK_SPELLOBJECT)
	{
		return true;
	}
	if (work_id == CECHPWork::WORK_HACKOBJECT)
	{
		return true;
	}

	return false;
}

void CECAutoHelper::UseTalismanItem(int index)
{
	ASSERT(index>=1 && index <=4);

	CDlgTalisman *pDlg = (CDlgTalisman *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Talisman");
	if (pDlg)
	{
		pDlg->UseTalismanItem(index);
	}
}

bool CECAutoHelper::UseSkill()
{
	int i;
	if (m_bSkillDisabled)
		return false;
//	static SKILLCOUNT = 0;
	CDlgTalisman *pDlg = (CDlgTalisman *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Talisman");
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	
	for(i = Cnt_UseSkill1; i<= Cnt_UseSkill9; ++i)
	{
		int skill_id = 0;
		if (m_Counters[i].IsFull())
		{
			skill_id = pDlg->CheckTalismanSkill(i - Cnt_UseSkill1 +1);
			if(skill_id)
			{
				CECSkill* pSkill = pHost->GetSkill(skill_id);
				if (pSkill && pHost->CheckSkillCastCondition(pSkill) == 0 && pSkill->ReadyToCast())
				{						
					pDlg->UseTalismanSkill(i - Cnt_UseSkill1 +1);
					m_bCastASkill = true;
				//	m_Counters[i].Reset();
					return true;
				}
			}
			else
				m_Counters[i].Reset();
		}
	}
	return false;
}

bool CECAutoHelper::UseItem()
{
	int i;
	CDlgTalisman *pDlg = (CDlgTalisman *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Talisman");
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	for (i = Cnt_UseItem1; i<= Cnt_UseItem3; ++i)
	{
		if (m_Counters[i].IsFull())
		{
			pDlg->UseTalismanItem(i-Cnt_UseItem1+10);
			m_Counters[i].Reset();
//			return true;
		}
	}
	return false;
}

bool CECAutoHelper::Save(int playerId)
{
	abase::vector<AWString> * list = NULL;
	AString file_name;
	CreateDirectoryA(LOCAL_DIR, NULL);
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	CreateDirectoryA(sub_dir, NULL);
	file_name.Format("%s\\talisman.ini", sub_dir);
	AString strKey, strValue;
	int i;
	strValue.Format("%d", m_Counters[Cnt_HealHP].GetPeriod());
	WritePrivateProfileStringA("Settings", "HpDelay", strValue, file_name);
	strValue.Format("%d", GetHpRate());
	WritePrivateProfileStringA("Settings", "HpRate", strValue, file_name);
	strValue.Format("%d", m_Counters[Cnt_HealMP].GetPeriod());
	WritePrivateProfileStringA("Settings", "MpDelay", strValue, file_name);
	strValue.Format("%d", GetMpRate());
	WritePrivateProfileStringA("Settings", "MpRate", strValue, file_name);
	strValue.Format("%d", m_Counters[Cnt_HealDP].GetPeriod());
	WritePrivateProfileStringA("Settings", "DpDelay", strValue, file_name);
	strValue.Format("%d", GetDpRate());
	WritePrivateProfileStringA("Settings", "DpRate", strValue, file_name);
	strValue.Format("%d", m_bAnswerQuestion);
	WritePrivateProfileStringA("Settings", "AnswerQuestion", strValue, file_name);
	strValue.Format("%d", m_iCurRangeChoosen);
	WritePrivateProfileStringA("Settings", "AnchorRange", strValue, file_name);
	strValue.Format("%d", m_bSkillDisabled);
	WritePrivateProfileStringA("Settings", "SkillDisabled", strValue, file_name);
	for(i = 1; i <= 9; i++)
	{
		strKey.Format("SkillDelay%d", i);
		strValue.Format("%d", m_Counters[Cnt_UseSkill1+i-1].GetPeriod());
		WritePrivateProfileStringA("Settings", strKey, strValue, file_name);
	}
	for(i=1; i<=3; ++i)
	{
		strKey.Format("UseItemDelay%d", i);
		strValue.Format("%d", m_Counters[Cnt_UseItem1+i-1].GetPeriod());
		WritePrivateProfileStringA("Settings", strKey, strValue, file_name);
	}
	return true;
}

bool CECAutoHelper::Load(int playerId)
{
	AString strKey, strValue;
	AString file_name;
	AString sub_dir;
	sub_dir.Format("%s\\%d", LOCAL_DIR, playerId);
	file_name.Format("%s\\talisman.ini", sub_dir);
	int i;

	m_Counters[Cnt_HealHP].SetPeriod(GetPrivateProfileIntA("Settings", "HpDelay", m_Counters[Cnt_HealHP].GetPeriod(), file_name));
	m_iHpRate = GetPrivateProfileIntA("Settings", "HpRate", GetHpRate(), file_name);
	m_Counters[Cnt_HealMP].SetPeriod(GetPrivateProfileIntA("Settings", "MpDelay", m_Counters[Cnt_HealMP].GetPeriod(), file_name));
	m_iMpRate = GetPrivateProfileIntA("Settings", "MpRate", GetMpRate(), file_name);
	m_Counters[Cnt_HealDP].SetPeriod(GetPrivateProfileIntA("Settings", "DpDelay", m_Counters[Cnt_HealDP].GetPeriod(), file_name));
	m_iDpRate = GetPrivateProfileIntA("Settings", "DpRate", GetDpRate(), file_name);
	m_bAnswerQuestion = (GetPrivateProfileIntA("Settings", "AnswerQuestion", m_bAnswerQuestion, file_name) == 1);
	m_iCurRangeChoosen = GetPrivateProfileIntA("Settings", "AnchorRange", m_iCurRangeChoosen, file_name);
	SetAnchorRange(m_iCurRangeChoosen);
	m_bSkillDisabled = (GetPrivateProfileIntA("Settings", "SkillDisabled", m_bSkillDisabled, file_name) == 1);
	for(i = 1; i <= 9; i++)
	{
		strKey.Format("SkillDelay%d", i);
//		strValue.Format("%d", GetSkillDelay(i));
		m_Counters[Cnt_UseSkill1+i-1].SetPeriod(GetPrivateProfileIntA("Settings", strKey, m_Counters[Cnt_UseSkill1+i-1].GetPeriod(), file_name));
	}
	for(i = 1; i<=3; ++i)
	{
		strKey.Format("UseItemDelay%d", i);
//		strValue.Format("%d", GetSkillDelay(i));
		m_Counters[Cnt_UseItem1+i-1].SetPeriod(GetPrivateProfileIntA("Settings", strKey, m_Counters[Cnt_UseItem1+i-1].GetPeriod(), file_name));
	}
	
	return true;
}

int CECAutoHelper::GetHpDelay()
{
	return m_Counters[Cnt_HealHP].GetPeriod();
}

void CECAutoHelper::SetHpDelay(int delay)
{
	m_Counters[Cnt_HealHP].SetPeriod(delay);
}

void CECAutoHelper::SetAnchorRange(int range)
{
	ASSERT(range >=0 && range <= 3);
	if (range != m_iCurRangeChoosen)
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		m_vStartPos = pHost->GetPos();
	}
	if (range == 0)
	{
		m_fCurRange = 800.0f;
		m_iCurRangeChoosen = 0;
	}
	else if (range == 1)
	{
		m_fCurRange = 100.0f;
		m_iCurRangeChoosen = 1;
	}
	else if (range == 2)
	{
		m_fCurRange = 50.0f;
		m_iCurRangeChoosen = 2;
	}
	else if (range == 3)
	{
		m_fCurRange = 10.0f;
		m_iCurRangeChoosen = 3;
	}
	else
	{
		m_fCurRange = 50.0f;
		m_iCurRangeChoosen = 2;
	}
}

void CECAutoHelper::Start()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_vStartPos = pHost->GetPos();
	//reset
	m_bPickupAdjust = false;
	m_bPickingUp	= false;
	m_bCastASkill	= false;
	m_iPickupNum	= 0;
	m_iPickupTimes	= 0;
	m_iWaitSkill	= 0;

	m_bRunning = true;
}
void CECAutoHelper::Reset()
{
	m_bRunning = false;
}

bool CECAutoHelper::Heal()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return false;

	// 补血 补篮 补法宝精力
	ROLEBASICPROP &rbp =	pHost->GetBasicProps();
	ROLEEXTPROP   &rep =    pHost->GetExtendProps();
	float hpRate = rbp.iCurHP/(float)rep.max_hp;
	float mpRate = rbp.iCurMP/(float)rep.max_mp;
	float dpRate = rbp.iCurDP/(float)rep.max_dp;
	float staminaRate = 0.0f;
	CECInventory *pPack = pHost->GetPack(IVTRTYPE_EQUIPPACK);
	if( pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN) && pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN)->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
	{
		CECIvtrTalismanMain *pItem = (CECIvtrTalismanMain*)pPack->GetItem(EQUIPIVTR_MAIN_TALISMAN);
		
		int nNow, nMax;
		nNow = rbp.iTalismanStamina;
		nMax = pItem->GetEssence().ess.stamina;
		staminaRate = nNow/(float)nMax;	
	}

	// Heal 
	if (hpRate*100 < m_iHpRate && m_Counters[Cnt_HealHP].IsFull())
	{
		UseTalismanItem(1);
		m_Counters[Cnt_HealHP].Reset();	
	}
	if (mpRate*100 < m_iMpRate && m_Counters[Cnt_HealMP].IsFull())
	{
		UseTalismanItem(2);
		m_Counters[Cnt_HealMP].Reset();
	}
	if (!pHost->IsSkillFlying() && staminaRate < m_fStaminaRate && m_Counters[Cnt_HealStamina].IsFull())
	{	
		UseTalismanItem(3);
		m_Counters[Cnt_HealStamina].Reset();
	}
	if (dpRate*100 < m_iDpRate && m_Counters[Cnt_HealDP].IsFull())
	{
		UseTalismanItem(4);
		m_Counters[Cnt_HealDP].Reset();
	}
	return true;
}


// 处理障碍怪物，就是攻击无效的怪物
void CECAutoHelper::ProcessObstacleMonster(DWORD dwDelta)
{
	CECHostPlayer * pHost = g_pGame->GetGameRun()->GetHostPlayer();
	
	if (pHost->CannotAttack() || pHost->IsRooting())
	{
		m_Counters[Cnt_Obstacle].Reset();
		return;
	}

	if (pHost->GetSelectedTarget() == 0)
	{
		m_idTarget = 0;
		m_ObstacleCnt.Reset();
		m_iCurTargetHp = 0;
		return;
	}
	if (m_iBadID && m_iBadID == pHost->GetSelectedTarget())
	{
		g_pGame->GetGameRun()->PostMessage(MSG_HST_AUTOSELECT, MAN_PLAYER, 0, 1);
		m_bNewTarget = true;
		return;
	}

	if (m_Counters[Cnt_Obstacle].IsFull())
	{
		m_Counters[Cnt_Obstacle].Reset();
		if (m_idTarget != pHost->GetSelectedTarget())
		{
			m_idTarget = pHost->GetSelectedTarget();
			m_ObstacleCnt.Reset();
			m_iCurTargetHp = 0;
			return;
		}
		if (!ISNPCID(m_idTarget))
			return;	
		CECNPC* pNpc = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_idTarget);
		if (pNpc && pNpc->IsMonsterNPC())
		{
			ROLEBASICPROP &rbp =	pNpc->GetBasicProps();
			
			if (m_iCurTargetHp == rbp.iCurHP)
			{
				if (m_ObstacleCnt.IncCounter(1))
				{
					m_iBadID = m_idTarget;
					g_pGame->GetGameRun()->PostMessage(MSG_HST_AUTOSELECT, MAN_PLAYER, 0, 1);
					m_bNewTarget = true;
					m_ObstacleCnt.Reset();
					m_iCurTargetHp = 0;
				}	
			}
			else
			{
				m_iCurTargetHp = rbp.iCurHP;
				m_ObstacleCnt.Reset();
			}
		}
	}
}

void CECAutoHelper::SetSkillCooling()
{
	int i;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CDlgTalisman *pDlg = (CDlgTalisman *)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Talisman");
	
	int work_id = pHost->GetHPWorkMan()->GetCurWorkID();
	
	if (work_id == CECHPWork::WORK_SPELLOBJECT)
	{
		int curSkillid;
		CECHPWorkSpell * pWorkSpell = (CECHPWorkSpell *)pHost->GetHPWorkMan()->GetCurWork();
		if (pWorkSpell && pWorkSpell->GetSkill())
		{
			curSkillid = pWorkSpell->GetSkill()->GetSkillID();
		
			for(i = Cnt_UseSkill1; i<= Cnt_UseSkill9; ++i)
			{
				int skill_id = 0;
				skill_id = pDlg->CheckTalismanSkill(i - Cnt_UseSkill1 +1);
				if(skill_id == curSkillid && m_Counters[i].IsFull())
				{
					m_Counters[i].Reset();
				}	
			}
		}
	}
	
	int NUM = sizeof(Instantcoolskill)/sizeof(int);

	for(i = Cnt_UseSkill1; i<= Cnt_UseSkill9; ++i)
	{
		int skill_id = 0;
		skill_id = pDlg->CheckTalismanSkill(i - Cnt_UseSkill1 +1);
		if(skill_id)
		{
			CECSkill* pSkill = pHost->GetSkill(skill_id);
			for(int j =0; j< NUM; ++j)
			{
				if (Instantcoolskill[j] == skill_id)
				{
					if(!pSkill->ReadyToCast() && m_Counters[i].IsFull())
						m_Counters[i].Reset();
				}
			}
		}
	}
}

#undef  LOCAL_DIR