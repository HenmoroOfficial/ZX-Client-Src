/*
 * FILE: EC_TaskInterface.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/2/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include <time.h>

#include "EC_Global.h"
#include "EC_TaskInterface.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_Skill.h"
#include "EC_Team.h"
#include "TaskTemplMan.h"
#include "EC_World.h"
#include "EC_Faction.h"
#include "EC_Achievement.h"
#include "ATime.h"
#include "ids.hxx"
#include "roleinfo" //Added 2011-04-11.

#include "EC_IvtrItem.h"
#include "elementdataman.h"

#include "AnmSystem.h"

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

static bool _task_dialog_is_show = false;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

ATaskTemplMan* GetTaskTemplMan()
{
	return g_pGame->GetTaskTemplateMan();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECTaskInterface
//	
///////////////////////////////////////////////////////////////////////////

CECTaskInterface::CECTaskInterface(CECHostPlayer* pHost)
{
	m_pHost	= pHost;

	m_pActiveListBuf		= NULL;
	m_pFinishedListBuf		= NULL;
	m_pFinishedTimeListBuf	= NULL;
}

CECTaskInterface::~CECTaskInterface()
{
}

//	Initialize object
bool CECTaskInterface::Init(void* pActiveListBuf, int iActiveListLen, void* pFinishedListBuf,
							int iFinishedListLen, void* pFinishedTimeListBuf, int iFinishedTimeListLen)
{
	if (!(m_pActiveListBuf = a_malloc(TASK_ACTIVE_LIST_BUF_SIZE)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECTaskInterface::Init", __LINE__);
		return false;
	}

	if (!(m_pFinishedListBuf = a_malloc(TASK_FINISHED_LIST_BUF_SIZE)))
	{
		a_free(m_pActiveListBuf);
		m_pFinishedListBuf = NULL;
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECTaskInterface::Init", __LINE__);
		return false;
	}

	if (!(m_pFinishedTimeListBuf = a_malloc(TASK_FINISH_TIME_LIST_BUF_SIZE)))
	{
		a_free(m_pActiveListBuf);
		a_free(m_pFinishedListBuf);
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECTaskInterface::Init", __LINE__);
		return false;
	}

	ASSERT(pActiveListBuf && pFinishedListBuf && pFinishedTimeListBuf);
	memcpy(m_pActiveListBuf, pActiveListBuf, iActiveListLen);
	memcpy(m_pFinishedListBuf, pFinishedListBuf, iFinishedListLen);
	memcpy(m_pFinishedTimeListBuf, pFinishedTimeListBuf, iFinishedTimeListLen);

	//	Clear rest buffer
	if (iActiveListLen < TASK_ACTIVE_LIST_BUF_SIZE)
		memset((BYTE*)m_pActiveListBuf+iActiveListLen, 0, TASK_ACTIVE_LIST_BUF_SIZE-iActiveListLen);

	if (iFinishedListLen < TASK_FINISHED_LIST_BUF_SIZE)
		memset((BYTE*)m_pFinishedListBuf+iFinishedListLen, 0, TASK_FINISHED_LIST_BUF_SIZE-iFinishedListLen);

	if (iFinishedTimeListLen < TASK_FINISH_TIME_LIST_BUF_SIZE)
		memset((BYTE*)m_pFinishedTimeListBuf+iFinishedTimeListLen, 0, TASK_FINISH_TIME_LIST_BUF_SIZE-iFinishedTimeListLen);

	ATaskTemplMan* pTaskMan = GetTaskTemplMan();
	pTaskMan->Release();
	pTaskMan->LoadTasksFromPack("data\\tasks.data", "data\\forbidden_task.txt", true);
	pTaskMan->FilterTaskData(this); //过滤任务数据Added 2011-11-17.
	pTaskMan->LoadNPCInfoFromPack("data\\task_npc.data");
	pTaskMan->VerifyDynTasksPack("userdata\\dyn_tasks.data");
	InitActiveTaskList();

	return true;
}

//	Release object
void CECTaskInterface::Release()
{
	if (m_pActiveListBuf)
	{
		a_free(m_pActiveListBuf);
		m_pActiveListBuf = NULL;
	}

	if (m_pFinishedListBuf)
	{
		a_free(m_pFinishedListBuf);
		m_pFinishedListBuf = NULL;
	}

	if (m_pFinishedTimeListBuf)
	{
		a_free(m_pFinishedTimeListBuf);
		m_pFinishedTimeListBuf = NULL;
	}
}

void TaskInterface::UpdateTaskUI()
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI) pGameUI->UpdateTask();
}

void TaskInterface::PopUpTaskMonsterInfo(unsigned long ulTask, unsigned long ulMonsterID)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI) pGameUI->PopUpMonsterInfo( ulTask, ulMonsterID);
}

void TaskInterface::PopupTaskFinishDialog(unsigned long ulTaskId, const talk_proc* pTalk)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (pGameUI && pTalk->num_window)
		pGameUI->PopupTaskFinishDialog(ulTaskId, const_cast<talk_proc*>(pTalk));
}

void TaskInterface::PopupTaskCheckInfoDialog(unsigned long ulTaskId, bool bRemove)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (pGameUI)
		pGameUI->PopupTaskCheckInfo(ulTaskId, bRemove);
}

void TaskInterface::ShowMessage(const wchar_t* szMsg, int nIndex)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI)
	{
		AString strName;
		strName.Format("TaskErr%03d", nIndex);
		pGameUI->ShowErrorMsg(szMsg, strName);
	}
}

void TaskInterface::ShowTaskMessage(unsigned long ulTask, int reason)
{
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (pGameUI)
		pGameUI->AddTaskHint(ulTask, reason);
	
	if (reason == TASK_MSG_SUCCESS)
		g_pGame->GetGameRun()->GetHostPlayer()->OnTaskFinished();
}

unsigned long CECTaskInterface::GetPlayerLevel()
{
	return (DWORD)m_pHost->GetBasicProps().iLevel;
}

int CECTaskInterface::GetTalismanValue()
{
	return m_pHost->GetTalismanValue();
}

void* CECTaskInterface::GetActiveTaskList()
{
	return m_pActiveListBuf;
}

void* CECTaskInterface::GetFinishedTaskList()
{
	return m_pFinishedListBuf;
}

void* CECTaskInterface::GetFinishedTimeList()
{
	return m_pFinishedTimeListBuf;
}

int CECTaskInterface::GetTaskItemCount(unsigned long ulTaskItem)
{
	CECInventory* pPack = m_pHost->GetTaskPack();
	return pPack ? pPack->GetItemTotalNum((int)ulTaskItem) : 0;
}

int CECTaskInterface::GetCommonItemCount(unsigned long ulCommonItem)
{
	CECInventory* pPack = m_pHost->GetPack();
	return pPack ? pPack->GetItemTotalNum((int)ulCommonItem) : 0;
}

int CECTaskInterface::GetReinforceItemCount(unsigned long ulItemId, unsigned long ulLevel, unsigned char cond_type)
{
	return m_pHost->GetRefineItemNum(ulItemId, ulLevel, cond_type);
}

bool CECTaskInterface::IsInFaction()
{
	return m_pHost->GetFactionID() != 0;
}

int CECTaskInterface::GetFactionLev()
{
	Faction_Info* info = g_pGame->GetFactionMan()->GetFaction(m_pHost->GetFactionID());
	return info ? info->GetLevel() : -1;
}

int CECTaskInterface::GetFactionRole()
{
	return m_pHost->GetFRoleID();
}

long CECTaskInterface::GetFactionContribution()
{
	return m_pHost->GetContribution();
}

int CECTaskInterface::GetFamilyContribution()
{
	return m_pHost->GetFamilyContrib();
}

int CECTaskInterface::GetBattleScore()
{
	return m_pHost->GetBattleScore();
}
// 要加战场类型判断
int CECTaskInterface::GetSJBattleScore()
{
	return m_pHost->GetSJBattleScore();
}

unsigned long CECTaskInterface::GetGoldNum()
{
	return (DWORD)m_pHost->GetMoneyAmount();
}

unsigned long TaskInterface::GetCurTime()
{
	return time(NULL) + g_pGame->GetTimeError();
}

bool TaskInterface::CalcFileMD5(const char* szFile, unsigned char md5[16])
{
	FILE* fp = fopen(szFile, "rb");

	if (!fp)
		return false;

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);

	if (sz == 0)
	{
		fclose(fp);
		return false;
	}

	fseek(fp, 0, SEEK_SET);
	char* buf = new char[sz];
	fread(buf, sz, 1, fp);
	fclose(fp);
	GNET::Octets input(buf, sz);
	GNET::Octets output = GNET::MD5Hash::Digest(input);
	memcpy(md5, output.begin(), 16);
	delete[] buf;
	return true;
}

int TaskInterface::GetTimeZoneBias()
{
	return g_pGame->GetTimeZoneBias();
}

void TaskInterface::WriteLog(int nPlayerId,	int nTaskId, int nType, const char* szLog)
{
}

void TaskInterface::WriteKeyLog(int nPlayerId, int nTaskId, int nType, const char* szLog)
{
}

long CECTaskInterface::GetReputation()
{
	return m_pHost->GetReputation();
}

long CECTaskInterface::GetRegionReputation(int index)
{
	return m_pHost->GetRegionReputation(index);
}

unsigned long CECTaskInterface::GetCurPeriod()
{
	return 0;
}

unsigned long CECTaskInterface::GetPlayerId()
{
	return m_pHost->GetCharacterID();
}

unsigned long CECTaskInterface::GetPlayerRace()
{
	return 0;
}

unsigned long CECTaskInterface::GetPlayerLifeAgainCnt()
{
	return m_pHost->GetRebornInfo().size();
}

unsigned long CECTaskInterface::GetPlayerLifeAgainOccup(int nLifeAgainIndex)
{
	if (nLifeAgainIndex >= 0 && nLifeAgainIndex < (int)m_pHost->GetRebornInfo().size())
		return m_pHost->GetRebornInfo()[nLifeAgainIndex].prof;

	return 0;
}

unsigned long CECTaskInterface::GetPlayerOccupation()
{
	return (DWORD)m_pHost->GetProfession();
}

long CECTaskInterface::GetPKValue()
{
	return m_pHost->GetPKValue();
}

bool CECTaskInterface::IsGM()
{
	return m_pHost->IsGM();
}

bool CECTaskInterface::HasTitle(short title)
{
	int nCount = m_pHost->GetTitleCount();

	for (int i = 0; i < nCount; i++)
		if (m_pHost->GetOneTitle(i) == title)
			return true;

	return false;
}

void CECTaskInterface::NotifyServer(const void* pBuf, size_t sz)
{
	g_pGame->GetGameSession()->c2s_CmdTaskNotify(pBuf, sz);
}

int CECTaskInterface::GetTeamMemberNum()
{
	CECTeam* pTeam = m_pHost->GetTeam();
	return pTeam ? pTeam->GetMemberNum() : 0;
}

void CECTaskInterface::GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo)
{
	CECTeam* pTeam = m_pHost->GetTeam();
	if (!pTeam)
	{
		memset(pInfo, 0, sizeof (task_team_member_info));
		return;
	}

	CECTeamMember* pMember = pTeam->GetMemberByIndex(nIndex);
	if (!pMember)
	{
		memset(pInfo, 0, sizeof (task_team_member_info));
		return;
	}

	pInfo->m_ulId			= pMember->GetCharacterID();
	pInfo->m_ulMasterID		= pMember->GetMasterid();
	pInfo->m_ulLevel		= pMember->GetLevel();
	pInfo->m_ulOccupation	= pMember->GetProfession();
	pInfo->m_ulWorldId		= pMember->GetInstanceID();
	pInfo->m_bMale			= (pMember->GetGender() == 0);
	pInfo->m_nFamilyId		= pMember->GetFamilyId();

	const A3DVECTOR3& v = pMember->GetPos();
	pInfo->m_Pos[0]	= v.x;
	pInfo->m_Pos[1]	= v.y;
	pInfo->m_Pos[2]	= v.z;
}

unsigned long CECTaskInterface::GetTeamMemberId(int nIndex)
{
	CECTeam* pTeam = m_pHost->GetTeam();
	if (!pTeam)
		return 0;
		
	CECTeamMember* pMember = pTeam->GetMemberByIndex(nIndex);
	return pMember ? (unsigned long)pMember->GetCharacterID() : 0;
}

bool CECTaskInterface::IsCaptain()
{
	CECTeam* pTeam = m_pHost->GetTeam();
	if (!pTeam)
		return false;

	return pTeam->GetLeaderID() == m_pHost->GetCharacterID();
}

bool CECTaskInterface::IsInTeam()
{
	return m_pHost->GetTeam() ? true : false;
}

bool CECTaskInterface::IsMale()
{
	return m_pHost->GetGender() == GENDER_MALE;
}

bool CECTaskInterface::IsMarried()
{
	return m_pHost->GetSpouse() != 0;
}

unsigned long CECTaskInterface::GetPos(float pos[3])
{
	A3DVECTOR3 vPos = m_pHost->GetPos();
	pos[0] = vPos.x;
	pos[1] = vPos.y;
	pos[2] = vPos.z;
	return g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
}

bool CECTaskInterface::HasLivingSkill(unsigned long ulSkill)
{
	return m_pHost->GetSkill(ulSkill) != 0;
}

long CECTaskInterface::GetLivingSkillProficiency(unsigned long ulSkill)
{
	return m_pHost->GetSkillAbility(ulSkill);
}

// long CECTaskInterface::GetLivingSkillLevel(unsigned long ulSkill)
// {
// 	CECSkill* pSkill = m_pHost->GetSkill(ulSkill);
// 	return pSkill ? pSkill->GetSkillLevel() : 0;
// }

int CECTaskInterface::GetPetCon()
{
	return m_pHost->GetPetConstruction();
}

int CECTaskInterface::GetPetCiv()
{
	return m_pHost->GetPetCivilization();
}

void CECTaskInterface::GetSpecailAwardInfo(special_award* p)
{
	*p = *GetTaskTemplMan()->GetSpecialAward();
}

bool CECTaskInterface::CanDeliverCommonItem(unsigned long ulTypes)
{
	return m_pHost->GetPack()->GetEmptySlotNum() >= static_cast<int>(ulTypes);
}

bool CECTaskInterface::CanDeliverTaskItem(unsigned long ulTypes)
{
	return m_pHost->GetTaskPack()->GetEmptySlotNum() >= static_cast<int>(ulTypes);
}

int CECTaskInterface::GetProduceSkillLev()
{
	return m_pHost->GetProduceSkillLevel();
}

int CECTaskInterface::GetProduceSkillExp()
{
	return m_pHost->GetProduceSkillExp();
}

bool CECTaskInterface::IsInFamily()
{
	return m_pHost->GetFamilyID() != 0;
}

int CECTaskInterface::GetFamilyID()
{
	return m_pHost->GetFamilyID();
}

#define FAMILY_SKILL_BASE 441

int CECTaskInterface::GetFamilySkillLevel(int nIndex)
{
	CECSkill* pSkill = m_pHost->GetSkill(FAMILY_SKILL_BASE + nIndex);
	return pSkill ? pSkill->GetSkillLevel() : 0;
}

int CECTaskInterface::GetFamilySkillProficiency(int nIndex)
{
	FamilySkillList& list = g_pGame->GetFactionMan()->GetFamilySkillList();

	for (size_t i = 0; i < list.size(); i++)
	{
		if (list[i].id == int(FAMILY_SKILL_BASE + nIndex))
			return list[i].ability;
	}

	return 0;
}

int CECTaskInterface::GetFamilyMonsterRecord(int nIndex)
{
	return g_pGame->GetFactionMan()->GetFamilyRecord(nIndex);
}

int CECTaskInterface::GetFamilyCommonValue(int nIndex)
{
	if (nIndex == enumTaskFamilyValue1)
	{
		Faction_Info* info = g_pGame->GetFactionMan()->GetFaction(m_pHost->GetFactionID());

		if (info)
			return info->GetNimbus();
	}

	return 0;
}

void CECTaskInterface::CameraMove(unsigned long ulCutSceneIndex)
{
	m_pHost->StartCutscene(ulCutSceneIndex);
}

void CECTaskInterface::PlayAnimation(unsigned long ulAnimationID)
{
	g_pGame->GetGameRun()->GetAnmSystem()->Play(ulAnimationID);
}

void CECTaskInterface::PlayAwardGfx(unsigned long ulGfxID)
{
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->PlayAwardGfx(ulGfxID);
}

void CECTaskInterface::PlayTinyGame(unsigned long ulGameID)
{
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->PlayTinyGame(ulGameID);
}

bool CECTaskInterface::IsMaster()
{
	return m_pHost->GetMaster() == m_pHost->GetCharacterID();
}

bool CECTaskInterface::IsPrentice()
{
	return m_pHost->GetMaster() && !IsMaster();
}

unsigned long CECTaskInterface::GetInMasterLev()
{
	return 0;
}

int CECTaskInterface::GetTreasureConsumed()
{
	return m_pHost->GetCashConsumed();
}

int CECTaskInterface::GetTotalCaseAdd()
{
	return m_pHost->GetCashAdd();
}

unsigned long  CECTaskInterface::GetAchievePoint()
{
	return m_pHost->GetAchievementMan()->GetAchievementPoint(); // 获得总的成就点数
}

unsigned long  CECTaskInterface::GetCircleGroupPoints()	// 获得总的圈子积分, 尚待实现
{
	return m_pHost->GetCircleScore(); 
}
int CECTaskInterface::GetCircleGroupId()				// 获取圈子ID
{
	return m_pHost->GetCircleID(); 
}
unsigned long CECTaskInterface::GetLastLogoutTime()		// 获取上次下线时间
{
	return m_pHost->GetLastLogoutTime();
}

unsigned long CECTaskInterface::GetTerritoryScore()		// 获取领土战积分
{
	return m_pHost->GetTerritoryScore();
}

unsigned long CECTaskInterface::GetFriendNum() // 获取好友数量
{
	return m_pHost->GetFriendNum();
}

unsigned long CECTaskInterface::GetCultivation()
{
	return m_pHost->GetCultivation();
}

bool CECTaskInterface::IsMaxInvSize()
{
	enum{ PACKMAXSIZE = 144, };
	return m_pHost->GetPack()->GetSize() == PACKMAXSIZE;
}

bool CECTaskInterface::IsAchievementFinish(unsigned short id)
{
	return m_pHost->GetAchievementMan()->GetAchievement(id)->bFinish;
}

int CECTaskInterface::GetTransformID()
{
	return m_pHost->GetTransfigureInfo().id;
}

int CECTaskInterface::GetTransformLevel()
{
	return m_pHost->GetTransfigureProp().level;
}

int CECTaskInterface::GetTransformExpLevel()
{
	return m_pHost->GetTransfigureProp().exp_level;
}

bool CECTaskInterface::IsInCrossServer()
{
	return (m_pHost->GetSrcServerID() != 0);
}

int CECTaskInterface::GetItemSlotCnt(unsigned long ulItemTemplID, unsigned long ulItemNum)
{
	if(ulItemNum == 0)
		return 0;

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(ulItemTemplID, 0, 1);
	if(!pItem)
		return 0;

	int iPileLimit = pItem->GetPileLimit();

	delete pItem;
	pItem = NULL;

	return ulItemNum/iPileLimit + ((ulItemNum%iPileLimit == 0) ? 0 : 1);
}

unsigned long CECTaskInterface::GetFengshenLevel()
{
	return m_pHost->GetBasicProps().iDeityLevel;
}

bool CECTaskInterface::IsFengshenExpFull()
{
	return m_pHost->IsDeityExpFull();
}

/* 国战 */
bool CECTaskInterface::HasKing()
{
	return m_pHost->GetKingdomKingId() != 0;
}

bool CECTaskInterface::IsKing()
{
	return m_pHost->GetKingdomTitle() == 1;
}

bool CECTaskInterface::IsQueen()
{
	return m_pHost->GetKingdomTitle() == 2;
}

bool CECTaskInterface::IsGeneral()
{
	return m_pHost->GetKingdomTitle() == 3;
}

bool CECTaskInterface::IsOfficial()
{
	return m_pHost->GetKingdomTitle() == 4;
}

bool CECTaskInterface::IsMember()
{
	return m_pHost->GetKingdomTitle() == 5;
}

bool CECTaskInterface::IsGuard()
{
	return m_pHost->GetKingdomTitle() == 6;
}

int CECTaskInterface::GetKingScore() 
{
	return m_pHost->GetKingdomPoints();
}

int CECTaskInterface::GetBuildingLevel(int iBuildId)
{
	return m_pHost->GetBuildingLevel(iBuildId);
}

int CECTaskInterface::GetFactionGoldNote()
{
	return m_pHost->GetFactionGoldNote();
}

int CECTaskInterface::GetFactionGrass()
{
	return m_pHost->GetFactionGrass();
}

int CECTaskInterface::GetFactionMine()
{
	return m_pHost->GetFactionMine();
}

int CECTaskInterface::GetFactionMonsterCore()
{
	return m_pHost->GetFactionMonsterCore();
}

int CECTaskInterface::GetFactionMonsterFood()
{
	return m_pHost->GetFactionMonsterFood();
}

int CECTaskInterface::GetFactionMoney()
{
	return m_pHost->GetFactionMoney();
}

bool CECTaskInterface::InInterAction(int iInterObjId)
{
	return m_pHost->GetInteractorInfo().id == iInterObjId;
}

int CECTaskInterface::GetBuildLevelInConstruct()
{
	return m_pHost->GetBuildLevelInConstruct();
}

//Added 2011-04-11.
unsigned long CECTaskInterface::GetCreateRoleTimeDuration()
{
	// 获得角色的创建时间
	int nCreateTime = g_pGame->GetGameRun()->GetSelectedRoleInfo().create_time;

	// 获得服务器的时间
	int nCurTime = g_pGame->GetServerGMTTime();
	
	// 判断合法性
	if ( nCurTime < nCreateTime )
	{
		return 0;
	}

	//返回角色创建的时间长度
	return nCurTime - nCreateTime;
}