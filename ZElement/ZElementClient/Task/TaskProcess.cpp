#include "TaskTempl.h"
#include "TaskInterface.h"
#include "TaskTemplMan.h"

#ifndef _TASK_CLIENT
#include "TaskServer.h"
#endif

#ifdef _ELEMENTCLIENT
	#include "LuaEvent.h"
#endif

#define TASK_ENTRY_DATA_CUR_VER 3

// version convert

static void convert_finishtimelist_v2(char* pData)
{
	char buf[TASK_FINISH_TIME_LIST_BUF_SIZE];
	memset(buf, 0, sizeof(buf));
	TaskFinishTimeList* lst = (TaskFinishTimeList*)buf;

	unsigned short count = *(unsigned short*)pData;
	_TaskFinishTimeEntry_V2* entries = (_TaskFinishTimeEntry_V2*)(pData + sizeof(count));
	lst->m_uCount = count;

	for (unsigned short i = 0; i < count; i++)
	{
		TaskFinishTimeEntry& entry_new = lst->m_aList[i];
		_TaskFinishTimeEntry_V2& entry_old = entries[i];

		entry_new.m_uTaskId = entry_old.m_uTaskId;
		entry_new.m_ulTimeMark = entry_old.m_ulTimeMark;
		entry_new.m_uFinishCount = 0;
		entry_new.m_ulUpdateTime = 0;
	}

	memcpy(pData, buf, sizeof(buf));
}

extern storage_task_info _storage_task_info[];

// Process Part

unsigned long ATaskTempl::CheckPrerequisite(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	unsigned long ulCurTime,
	bool bCheckPrevTask,
	bool bCheckTeam,
	bool bCheckBudge,
	bool bStorageRefresh) const
{
	unsigned long ulRet = 0;

	// 非跨服任务，在跨服服务器上无法接、无法完成
	if(!IsCrossServerTask() && pTask->IsInCrossServer())
	{
		if(m_ulType != 1 && m_ulType != 2 && m_ulType != 3 && m_ulType != 11 && m_ulType != 18 && m_ulType != 19 && m_ulType != 20) // 跨服仍然可以接原服的库任务，防止跨服库任务刷新时覆盖原服库任务
		{			
			ulRet = TASK_PREREQU_FAIL_IN_CROSS_SERVER;
			goto ret_here;
		}
	}

	// 跨服任务，且需要检查全局变量，则无法接，无法完成
	if(IsCrossServerTask() && (m_bPremNeedComp || m_bFinNeedComp || m_lChangeKeyArr.size() != 0 || m_Award_S->m_lChangeKeyArr.size() != 0))
	{
		ulRet = TASK_PREREQU_FAIL_CROSSSERVER_GLOBAL;
		goto ret_here;
	}

	if (GetTaskTemplMan()->IsForbiddenTask(m_ID))
	{
		ulRet = TASK_PREREQU_FAIL_INDETERMINATE;
		goto ret_here;
	}

	// 必须是根节点
	if (m_pParent)
	{
		ulRet = TASK_PREREQU_FAIL_NOT_ROOT; 
		goto ret_here;
	}

	if (bCheckBudge)
	{
		ulRet = CheckBudget(pList);
		if (ulRet) goto ret_here;
	}
	else if (pList->GetEntry(m_ID))
	{
		ulRet = TASK_PREREQU_FAIL_SAME_TASK;
		goto ret_here;
	}

	ulRet = CheckGivenItems(pTask);
	if (ulRet) goto ret_here;

	// 是否满足发放时间
	ulRet = CheckTimetable(ulCurTime);
	if (ulRet) goto ret_here;

#ifndef _TASK_CLIENT
	// 动态清空的时间为0, 则失败
	if (m_lDynFinishClearTime)
	{
		if (pTask->GetGlobalValue(m_lDynFinishClearTime) == 0)
		{
			ulRet = TASK_PREREQU_FAIL_DYNCLEAR_TIME_ZERO;
			goto ret_here;
		}
	}

	if (pTask->IsDynamicForbiddenTask(m_ID))
	{
		ulRet = TASK_PREREQU_FAIL_DYNFORBIDDEN;
		goto ret_here;
	}
#endif

	// 发放时间间隔
	ulRet = CheckDeliverTime(pTask, ulCurTime);
	if (ulRet) goto ret_here;

	ulRet = CheckFnshLst(pTask, ulCurTime);
	if (ulRet) goto ret_here;

	// Check Level
	ulRet = CheckLevel(pTask);
	if (ulRet) goto ret_here;
	
	// Check Talisman value
	ulRet = CheckTalismanValue(pTask);
	if (ulRet) return ulRet;

	// 声望
	ulRet = CheckRepu(pTask);
	if (ulRet) goto ret_here;

	// 友好度
	ulRet = CheckFriendship(pTask);
	if (ulRet) goto ret_here;

	// 押金
	ulRet = CheckDeposit(pTask);
	if (ulRet) goto ret_here;

	// 帮派贡献度
	ulRet = CheckContrib(pTask);
	if (ulRet) goto ret_here;

	// 所需物品
	//Modified 2011-03-11.由于库任务刷新时，也调用了整个接口，而天界库任务在刷新时
	//希望能不判断前提物品。但是，任务发放时是一定需要判断的。因此设置该特殊接口
	if( !bStorageRefresh || 
		( bStorageRefresh && (1 == m_ulPremItems) && (42929 != m_PremItems[0].m_ulItemTemplId && 53490 != m_PremItems[0].m_ulItemTemplId) ))
	{
		ulRet = CheckItems(pTask);
		if (ulRet) goto ret_here;
	}

	// 帮派
	ulRet = CheckFaction(pTask);
	if (ulRet) goto ret_here;

	// 性别
	ulRet = CheckGender(pTask);
	if (ulRet) goto ret_here;

	// 职业
	ulRet = CheckOccupation(pTask);
	if (ulRet) goto ret_here;

	// 到达特定时期
	ulRet = CheckPeriod(pTask);
	if (ulRet) goto ret_here;

	// 任务条件
	if (bCheckPrevTask)
	{
		ulRet = CheckPreTask(pTask);
		if (ulRet) goto ret_here;
	}

	// 互斥任务
	ulRet = CheckMutexTask(pTask, ulCurTime);
	if (ulRet) goto ret_here;

	// 区域触发
	ulRet = CheckInZone(pTask);
	if (ulRet) goto ret_here;

	// 组队任务（检查师徒任务）
	if (bCheckTeam)
	{
		ulRet = CheckTeamTask(pTask);
		if (ulRet) goto ret_here;
	}

	// 夫妻
	ulRet = CheckSpouse(pTask);
	if (ulRet) goto ret_here;

	// 婚姻
	ulRet = CheckMarriage(pTask);
	if (ulRet) goto ret_here;

	// 生活技能熟练度
	ulRet = CheckLivingSkill(pTask);
	if (ulRet) goto ret_here;

	// 宠物工坊
	ulRet = CheckPetConAndCiv(pTask);
	if (ulRet) goto ret_here;

	// 特殊活动奖励
	ulRet = CheckSpecialAward(pTask);
	if (ulRet) goto ret_here;

	// PK值
	ulRet = CheckPKValue(pTask);
	if (ulRet) goto ret_here;

	// 是否GM
	ulRet = CheckGM(pTask);
	if (ulRet) goto ret_here;
	
	// 转生
	ulRet = CheckLifeAgain(pTask);
	if (ulRet) goto ret_here;

	// 称谓
	ulRet = CheckTitle(pTask);
	if (ulRet) goto ret_here;

	// 家族
	ulRet = CheckFamily(pTask);
	if (ulRet) goto ret_here;

	// 任务完成次数
	ulRet = CheckFinishCount(pTask);
	if (ulRet) goto ret_here;

	// 任务开启次数
	ulRet = CheckOpenCount(pTask);
	if (ulRet) goto ret_here;

	// 前提任务完成次数
	ulRet = CheckPremFinishCount(pTask);
	if (ulRet) goto ret_here;

	// 战场积分
	ulRet = CheckBattleScore(pTask);
	if (ulRet) goto ret_here;

	// 宋金战场积分
	ulRet = CheckSJBattleScore(pTask);
	if (ulRet) goto ret_here;

	// 师徒身份
	ulRet = CheckMasterPrentice(pTask);
	if (ulRet) goto ret_here;

	// 造化
	ulRet = CheckCultivation(pTask);
	if (ulRet) goto ret_here;

	// 消费元宝
	ulRet = CheckConsumeTreasure(pTask);
	if (ulRet) goto ret_here;

	// 充值元宝
	ulRet = CheckPremTotalTreasure(pTask);
	if (ulRet) goto ret_here;

	// 成就id
	ulRet = CheckAccomp(pTask);
	if (ulRet) goto ret_here;

	// 成就范围
	ulRet = CheckAchievement(pTask);
	if (ulRet) goto ret_here;

	// 圈子积分
#ifdef _TASK_CLIENT
	ulRet = CheckCircleGroupPoints(pTask);
	if (ulRet) goto ret_here;
#endif

	// 领土积分范围
	ulRet = CheckTerritoryScore(pTask);
	if (ulRet) goto ret_here;

	// 变身
	ulRet = CheckTransform(pTask);
	if (ulRet) goto ret_here;

	// 封神
	ulRet = CheckFengshen(pTask);
	if (ulRet) goto ret_here;

	// 角色创建时间长度 Added 2011-04-11.
	ulRet = CheckCreateRoleTime(pTask);
	if (ulRet) goto ret_here;

	// 是否有国王
	ulRet = CheckHasKing(pTask);
	if (ulRet) goto ret_here;

	// 国王帮职位
	ulRet = CheckNationPosition(pTask);
	if (ulRet) goto ret_here;

	// 国王积分
	ulRet = CheckKingScore(pTask);
	if (ulRet) goto ret_here;
	
	//帮派任务
	ulRet = CheckFactionTask(pTask);
	if(ulRet) goto ret_here;
	
	//帮派建筑
	ulRet = CheckBuildLevel(pTask);
	if(ulRet) goto ret_here;

	//帮派金券
	ulRet = CheckFactionGoldNote(pTask);
	if(ulRet) goto ret_here;

	//帮派灵草
	ulRet = CheckFactionGrass(pTask);
	if(ulRet) goto ret_here;

	//帮派矿石
	ulRet = CheckFactionMine(pTask);
	if(ulRet) goto ret_here;

	//帮派兽粮
	ulRet = CheckFactionMonsterCore(pTask);
	if(ulRet) goto ret_here;
	
	//帮派兽核
	ulRet = CheckFactionMosnterFood(pTask);
	if(ulRet) goto ret_here;

	//帮派元宝
	ulRet = CheckFactionMoney(pTask);
	if(ulRet) goto ret_here;

	//正在建设的等级
	ulRet = CheckBuildLevelCon(pTask);
	if(ulRet) goto ret_here;

	//互动物品
	ulRet = CheckInterObj(pTask);
	if(ulRet) goto ret_here;

	// 全局key/value
#ifndef _TASK_CLIENT
	ulRet = CheckGlobalExpressions(pTask,true);
	if (ulRet) goto ret_here;
#endif

ret_here:

	if (GetTaskTemplMan()->IsLogEnable())
	{
		char log[1024];
		sprintf(log, "CheckPrerequisite: ret = %d", ulRet);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, log);
	}

	return ulRet;
}

unsigned long ATaskTempl::CheckMutexTask(TaskInterface* pTask, unsigned long ulCurTime) const
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	unsigned long i;

	for (i = 0; i < m_ulMutexTaskCount; i++)
	{
		// 是否已接收了互斥任务
		if (pLst->GetEntry(m_ulMutexTasks[i]))
			return TASK_PREREQU_FAIL_MUTEX_TASK;

		const ATaskTempl* pMutex = GetTaskTemplMan()->GetTopTaskByID(m_ulMutexTasks[i]);

		if (!pMutex)
			return TASK_PREREQU_FAIL_MUTEX_TASK;

		// 互斥任务是否满足发放时间
		if (pMutex->CheckDeliverTime(pTask, ulCurTime) != 0)
			return TASK_PREREQU_FAIL_MUTEX_TASK;

		// 互斥任务是否能重复完成
		if (pMutex->CheckFnshLst(pTask, ulCurTime) != 0)
			return TASK_PREREQU_FAIL_MUTEX_TASK;
	}

	return 0;
}

unsigned long ATaskTempl::CheckSpecialAward(TaskInterface* pTask) const
{
	if (m_DynTaskType != enumDTTSpecialAward) return 0;
	if (!m_ulSpecialAward) return TASK_PREREQU_FAIL_SPECIAL_AWARD;

	special_award sa;
	pTask->GetSpecailAwardInfo(&sa);

	return m_ulSpecialAward == sa.id ? 0 : TASK_PREREQU_FAIL_SPECIAL_AWARD;
}

void ATaskTempl::DeliverTaskNoCheck(TaskInterface* pTask, ActiveTaskList* pList, unsigned long ulCurTime) const
{
	if (CheckBudget(pList) == 0)
	{
		task_sub_tags tags;
		memset(&tags, 0, sizeof(tags));
		DeliverTask(pTask, pList, NULL, 0, *pTask->GetTaskMask(), ulCurTime, NULL, &tags, NULL);

#ifndef _TASK_CLIENT
		NotifyClient(pTask, 0, TASK_SVR_NOTIFY_NEW, ulCurTime, (unsigned long)&tags);
#endif
	}
}

ActiveTaskEntry* ATaskTempl::DeliverTask(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCaptainTask,
	unsigned long& ulMask,
	unsigned long ulCurTime,
	const ATaskTempl* pSubTempl,
	task_sub_tags* pSubTag,
	TaskGlobalData* pGlobal,
	unsigned char uParentIndex
	) const
{
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	if (!pEntry)
		pEntry = aEntries + pList->m_uTaskCount;

	unsigned char uIndex = static_cast<unsigned char>(pEntry - aEntries);

#ifdef DEBUG_LOG

	if (pEntry->m_ulTemplAddr)
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "Error: DeliverTask, Prev Not Empty");

#endif

	pEntry->m_ID				= static_cast<unsigned short>(m_ID);
	pEntry->m_ulTemplAddr		= reinterpret_cast<unsigned long>(this);
	pEntry->m_ParentIndex		= uParentIndex;
	pEntry->m_PrevSblIndex		= 0xff;
	pEntry->m_NextSblIndex		= 0xff;
	pEntry->m_ChildIndex		= 0xff;
	pEntry->m_uState			= 0;
	pEntry->m_ulTaskTime	= ulCurTime;

	if (ulCaptainTask)
	{
		pEntry->m_ulCapTemplAddr = reinterpret_cast<unsigned long>(GetTaskTemplMan()->GetTopTaskByID(ulCaptainTask));
		if (pEntry->m_ulCapTemplAddr) pEntry->m_uCapTaskId = static_cast<unsigned short>(ulCaptainTask);
		else
		{
			pEntry->m_uCapTaskId = 0;

			char log[1024];
			sprintf(log, "DeliverTask, Cant Find CapTask = %d", ulCaptainTask);
			TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);
		}
	}
	else
	{
		pEntry->m_uCapTaskId = 0;
		pEntry->m_ulCapTemplAddr = 0;
	}

	pEntry->SetSuccess();		// Later will check whether truly succeed
	memset(pEntry->m_BufData, 0, sizeof(pEntry->m_BufData));

#ifndef _TASK_CLIENT
	ulMask |= m_ulMask;
#endif

	pList->m_uTaskCount++;

	if (!m_pParent)
	{
		m_bHidden ? pList->m_uTopHideTaskCount++ : pList->m_uTopShowTaskCount++;
		pList->m_uUsedCount += m_uDepth;

#ifndef _TASK_CLIENT

		if (pGlobal)
		{
			pGlobal->AddRevNum();
			pGlobal->m_ulRcvUpdateTime = ulCurTime;
			TaskUpdateGlobalData(m_ID, pGlobal->buf);
		}

#endif

	}

	if (uParentIndex != 0xff)
	{
		ActiveTaskEntry& ParentEntry = aEntries[uParentIndex];
		if (ParentEntry.m_ChildIndex == 0xff) ParentEntry.m_ChildIndex = uIndex;
		else
		{
			unsigned char uChildEntry = ParentEntry.m_ChildIndex;
			while (aEntries[uChildEntry].m_NextSblIndex != 0xff)
				uChildEntry = aEntries[uChildEntry].m_NextSblIndex;
			aEntries[uChildEntry].m_NextSblIndex = uIndex;
			pEntry->m_PrevSblIndex = uChildEntry;
		}
	}

#ifndef _TASK_CLIENT

	if (!m_pParent)
		DeliverGivenItems(pTask);

	// 扣除国王积分
	if (m_nPremKingScoreCost > 0)
		pTask->CostKingScore(m_nPremKingScoreCost);

	// 清xp技能cd
	if (m_bClearXpCD)
		pTask->ClearXpSkillCD();

#endif
	
	pEntry++;

	if (pSubTempl)
	{
		return pSubTempl->DeliverTask(
			pTask,
			pList,
			pEntry,
			0,
			ulMask,
			ulCurTime,
			NULL,
			pSubTag,
			NULL,
			uIndex);
	}
	else if (m_bRandOne)
	{
#ifdef _TASK_CLIENT
		if (pSubTag->cur_index < pSubTag->sz)
		{
			pSubTempl = GetSubByIndex(pSubTag->tags[pSubTag->cur_index]);
			pSubTag->cur_index++;

			if (pSubTempl)
			{
				return pSubTempl->DeliverTask(
					pTask,
					pList,
					pEntry,
					0,
					ulMask,
					ulCurTime,
					NULL,
					pSubTag,
					NULL,
					uIndex);
			}
		}
#else
		int nSel;
		pSubTempl = RandOneChild(pTask, nSel);

		if (pSubTempl)
		{
			if (pSubTag->sz < MAX_SUB_TAGS)
				pSubTag->tags[pSubTag->sz++] = static_cast<unsigned char>(nSel);

			return pSubTempl->DeliverTask(
				pTask,
				pList,
				pEntry,
				0,
				ulMask,
				ulCurTime,
				NULL,
				pSubTag,
				NULL,
				uIndex);
		}
#endif
	}
	else
	{
		const ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			pEntry = pChild->DeliverTask(
				pTask,
				pList,
				pEntry,
				0,
				ulMask,
				ulCurTime,
				NULL,
				pSubTag,
				NULL,
				uIndex);

			if (m_bExeChildInOrder) return pEntry;
			pChild = pChild->m_pNextSibling;
		}
	}

//new log
#ifndef _TASK_CLIENT
	TaskInterface::WriteTaskDeliverLog(pTask->GetPlayerId(), m_ID);	
#endif
//new log

#ifdef _ELEMENTCLIENT
		LuaEvent::AcceptScriptCompleteTask(m_ID);	
#endif

	return pEntry;
}

unsigned long ATaskTempl::RecursiveCalcAward(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurTime,
	int nChoice,
	unsigned long& ulCmnCount,
	unsigned long& ulTskCount,
	unsigned long& ulTopCount,
	unsigned char& uBudget) const
{	
	if (pEntry->IsSuccess()) 
	{
		// 检查完成等级,失败时不检查
		if (!CheckFinishLev(pTask))
			return TASK_AWARD_FAIL_BELOW_LEV;

		// 检查师徒任务,使得队伍中被逐出师门的徒弟不能完成徒弟任务
		if (pTask->IsInTeam() && GetTopTask()->m_bPrenticeTask)
		{
			unsigned long ulMasterID = pTask->GetTeamMemberId(0);

			//在队伍中获得自己信息
			unsigned long ulPlayerID = pTask->GetPlayerId();
			int MemNum = pTask->GetTeamMemberNum();
			task_team_member_info Mem;
			for (int i=1; i<MemNum; i++)
			{
				pTask->GetTeamMemberInfo(i, &Mem);
				if (Mem.m_ulId == ulPlayerID)
					break;
			}

			if (Mem.m_ulMasterID != ulMasterID)
				return TASK_PREREQU_FAIL_NOT_PRENTICE;
		}

		//如果任务完成时需要判断封神经验，判断封神经验是否足够Added 2011-10-13.
		if ( (m_nPremFengshenType == enumPremFSFengShenOnly) && m_bExpMustFull && !pTask->IsFengshenExpFull())
		{
			return TASK_AWARD_FAIL_NO_MORE_DEITY_EXP;
		}
		//Added end.

		//判断完成任务时玩家的职业条件是否满足（和开启该任务时的职业限制条件一样）
		//修复这个bug的原因：在做转职任务时，某些转职是非法的。需要在任务完成时禁止。Added 2011-12-14.
		if ( CheckOccupation( pTask ) )
		{
			return TASK_AWARD_FAIL_INVALID_OCCUPATION;
		}
		//Added end.

#ifndef _TASK_CLIENT
		if (CheckGlobalExpressions(pTask, false))
			return TASK_PREREQU_FAIL_GLOBAL_EXP;
#endif
	}		

	unsigned long ulRet;

	AWARD_DATA ad;
	CalcAwardData(pTask, &ad, pEntry, pEntry->m_ulTaskTime,	ulCurTime);

	if (ad.m_ulCandItems)
	{
		if (nChoice < 0 || nChoice >= static_cast<int>(ad.m_ulCandItems))
			nChoice = 0;

		CalcAwardItemsCount(pTask,  pEntry, &ad.m_CandItems[nChoice], ulCmnCount, ulTskCount);
	}

//检查当前能否发放额外奖励，若能发放并且有候选物品奖励，那么计算总共的普通和任务物品数量.Added 2011-03-04
#ifndef _TASK_CLIENT
	if( pEntry->IsCanDeliverExtraAward() && ad.m_ulExtraCandItems )
	{
		CalcAwardItemsCount(pTask,  pEntry, &ad.m_ExtraCandItems[0], ulCmnCount, ulTskCount);
	}
#endif

	if (ad.m_ulNewTask)
	{
		ATaskTempl* pNewTask = GetTaskTemplMan()->GetTopTaskByID(ad.m_ulNewTask);

		if (pNewTask)
		{
			if ((ulRet = pNewTask->CheckPrerequisite(pTask, pList, ulCurTime, false)) != 0)
				return ulRet;

			ulCmnCount += pNewTask->m_ulGivenCmnCount;
			ulTskCount += pNewTask->m_ulGivenTskCount;

			//高四位存放可见主任务个数，低四位存放隐藏任务个数
			unsigned long ulTopShowCount = (ulTopCount & 0xffff0000) >> 16;
			unsigned long ulTopHideCount = ulTopCount & 0x0000ffff;
			
			pNewTask->m_bHidden ? ulTopHideCount++ : ulTopShowCount++;
			
			ulTopShowCount = ulTopShowCount << 16;
			ulTopCount = 0;
			ulTopCount = ulTopCount | ulTopShowCount | ulTopHideCount;
			
			uBudget += pNewTask->m_uDepth;
		}
	}

	if (pEntry->m_ParentIndex != 0xff)
	{
		ActiveTaskEntry ParentEntry = pList->m_TaskEntries[pEntry->m_ParentIndex];

		if (!pEntry->IsSuccess() && m_bParentAlsoFail)
		{
			ParentEntry.ClearSuccess();
			ParentEntry.SetFinished();

			if ((ulRet = m_pParent->RecursiveCalcAward(pTask, pList, &ParentEntry, ulCurTime, -1, ulCmnCount, ulTskCount, ulTopCount, uBudget)) != 0)
				return ulRet;
		}
		else if (pEntry->IsSuccess() && m_bParentAlsoSucc)
		{
			ParentEntry.SetFinished();

			if (m_pParent->m_enumFinishType == enumTFTDirect
			&& (ulRet = m_pParent->RecursiveCalcAward(pTask, pList, &ParentEntry, ulCurTime, -1, ulCmnCount, ulTskCount, ulTopCount, uBudget)) != 0)
				return ulRet;
		}
		else if (m_pParent->m_bExeChildInOrder && m_pNextSibling)
			return 0;
		else if (pEntry->m_PrevSblIndex == 0xff && pEntry->m_NextSblIndex == 0xff)
		{
			ParentEntry.SetFinished();

			if (m_pParent->m_enumFinishType == enumTFTDirect
			&& (ulRet = m_pParent->RecursiveCalcAward(pTask, pList, &ParentEntry, ulCurTime, -1, ulCmnCount, ulTskCount, ulTopCount, uBudget)) != 0)
				return ulRet;
		}
	}

	return 0;
}

unsigned long ATaskTempl::RecursiveCheckAward(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurTime,
	int nChoice) const
{
	unsigned long ulCmnCount = 0;
	unsigned long ulTskCount = 0;
	unsigned long ulTopCount = 0;
	unsigned char uBudget = 0;

	unsigned long ulRet = RecursiveCalcAward(pTask, pList, pEntry, ulCurTime, nChoice, ulCmnCount, ulTskCount, ulTopCount, uBudget);
	if (ulRet) return ulRet;

	unsigned long ulTopShowCount = (ulTopCount & 0xffff0000) >> 16;
	unsigned long ulTopHideCount = ulTopCount & 0x0000ffff;
	
	if (ulTopShowCount && pList->m_uTopShowTaskCount + ulTopShowCount > TASK_MAX_SIMULTANEOUS_COUT
	 || ulTopHideCount && pList->m_uTopHideTaskCount + ulTopHideCount > TASK_HIDDEN_COUNT)
		return TASK_PREREQU_FAIL_FULL;
	
	if (uBudget && pList->m_uUsedCount + uBudget > TASK_ACTIVE_LIST_MAX_LEN) 
		return TASK_PREREQU_FAIL_NO_SPACE;

	if (ulCmnCount && !pTask->CanDeliverCommonItem(ulCmnCount)
	 || ulTskCount && !pTask->CanDeliverTaskItem(ulTskCount))
		return TASK_AWARD_FAIL_GIVEN_ITEM;

	return 0;
}

void ATaskTempl::RecursiveCheckTimeLimit(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurTime) const
{
	if (m_ulTimeLimit && pEntry->m_ulTaskTime + m_ulTimeLimit < ulCurTime) // 超时
		pEntry->ClearSuccess();

	if (m_bAbsFail)
	{
		tm cur = *localtime((const time_t*)&ulCurTime);

		if(m_tmAbsFailTime.before(&cur))
		{
			pEntry->ClearSuccess();
		}
	}

	if (m_pParent && pEntry->m_ParentIndex != 0xff)
	{
		ActiveTaskEntry& ParentEntry = pList->m_TaskEntries[pEntry->m_ParentIndex];
		m_pParent->RecursiveCheckTimeLimit(pTask, pList, &ParentEntry, ulCurTime);
	}
}

bool ATaskTempl::RecursiveCheckParent(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry) const
{
	if (m_pParent && pEntry->m_ParentIndex != 0xff)
	{
		ActiveTaskEntry& ParentEntry = pList->m_TaskEntries[pEntry->m_ParentIndex];

		if (!ParentEntry.IsSuccess())
			return false;

		return m_pParent->RecursiveCheckParent(pTask, pList, &ParentEntry);
	}

	return true;
}

void ATaskTempl::RecursiveAward(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurtime,
	int nChoice,
	task_sub_tags* pSubTag) const
{
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;

	// 清空子任务
	pList->ClearChildrenOf(pTask, pEntry);

	if (!pEntry->m_ulTemplAddr)
		return; // must check it

	//-------------------------------------------------------------------------
	//先确定这次任务完成是否需要刷新第九库的库任务Added 2012-09-07.
	bool bNeedRefreshNinthStorage = IsNeedRefreshNinthStorage( pEntry );
	//-------------------------------------------------------------------------
	if (!m_pParent)
	{
		if (m_bNeedRecord)
			static_cast<FinishedTaskList*>(pTask->GetFinishedTaskList())->AddOneTask(m_ID, pEntry->IsSuccess());

#ifndef _TASK_CLIENT

		if ( (m_bRecFinishCount && pEntry->IsSuccess()) || 
			 (!m_bRecFinishCount && m_ulMaxFinishCount > 0)) // 若开启任务时也要限制次数,此处需要判断是否清空
		{
			TaskFinishTimeList* pFinish = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
			TaskFinishTimeEntry* p = (TaskFinishTimeEntry*)pFinish->Search(m_ID);
			unsigned long ulCount;

			if (p)
			{
				CheckFinishTimeState(pTask, p, ulCurtime);

				if(m_bRecFinishCount)
					p->IncFinishCount();
				else if(p->m_uFinishCount >= m_ulMaxFinishCount)  //已达到开启任务次数限制，需要清空(m_uFinishCount此时存的是已接收任务次数)
					p->m_uFinishCount = 0;

				ulCount = p->m_uFinishCount;
			}
			else
				ulCount = pFinish->AddFinishTime(m_ID, GetFinishTimeLimit(pTask, ulCurtime));

			NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_FINISH_COUNT, ulCurtime, ulCount);

			if (m_bRecFinishCountGlobal && m_bRecFinishCount)
			{
				TaskPreservedData tpd;
				tpd.reason			= TASK_GLOBAL_ADD_FINISH_COUNT;
				tpd.m_ulSrcTask		= m_ID;
				tpd.m_ulSubTaskId	= 0;
				tpd.m_ulRcvNum		= 0;
				TaskQueryGlobalData(m_ID, pTask->GetPlayerId(), &tpd, sizeof(tpd));
			}
		}
#endif

	}

#ifndef _TASK_CLIENT

	// 发放奖励
	AWARD_DATA ad;
	CalcAwardData(
		pTask,
		&ad,
		pEntry,
		pEntry->m_ulTaskTime,
		ulCurtime);

	unsigned long ulRet = DeliverByAwardData(pTask, pList, pEntry, &ad, ulCurtime, nChoice);

	if (ulRet)
	{
		char log[1024];
		sprintf(log, "RecursiveAward, ret = %d", ulRet);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);
	}

	// 去掉获得的或预先给的物品，在发放物品后执行
	// 身上有指定物品则失败 && 非押金模式，则也要收取
	if (m_bClearAcquired || (m_bHaveItemFail && !m_bHaveItemFailNotTakeOff))
		RemoveAcquiredItem(pTask, false, pEntry->IsSuccess());
	else if (!pEntry->IsSuccess())
		TakeAwayGivenItems(pTask);

#endif

	pEntry->m_ulTemplAddr = 0;
	pEntry->m_ID = 0;

	if (pList->m_uTaskCount)
		pList->m_uTaskCount--;
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "Award, TaskCount == 0");

	if (pEntry->m_ParentIndex != 0xff)
	{
		ActiveTaskEntry& ParentEntry = aEntries[pEntry->m_ParentIndex];

		if (pEntry->m_PrevSblIndex != 0xff)
			aEntries[pEntry->m_PrevSblIndex].m_NextSblIndex = pEntry->m_NextSblIndex;
		else
			ParentEntry.m_ChildIndex = pEntry->m_NextSblIndex;

		if (pEntry->m_NextSblIndex != 0xff)
			aEntries[pEntry->m_NextSblIndex].m_PrevSblIndex = pEntry->m_PrevSblIndex;

		if (!pEntry->IsSuccess() && m_bParentAlsoFail)
		{
			pList->RealignTask(pEntry, 0);
			ParentEntry.ClearSuccess();
			ParentEntry.SetFinished();
			m_pParent->RecursiveAward(pTask, pList, &ParentEntry, ulCurtime, -1, pSubTag);
		}
		else if (pEntry->IsSuccess() && m_bParentAlsoSucc)
		{
			pList->RealignTask(pEntry, 0);
			ParentEntry.SetFinished();
			pList->ClearChildrenOf(pTask, &ParentEntry);
			if (m_pParent->m_enumFinishType == enumTFTDirect)
				m_pParent->RecursiveAward(pTask, pList, &ParentEntry, ulCurtime, -1, pSubTag);
		}
		else if (m_pParent->m_bExeChildInOrder && m_pNextSibling)
		{
			if (ParentEntry.m_ChildIndex != 0xff || pList->GetEntry(m_pNextSibling->m_ID)) // 错误情况，正常情况为0xff
				pList->RealignTask(pEntry, 0);
			else
			{
				pList->RealignTask(pEntry, m_pNextSibling->m_uDepth);
				m_pNextSibling->DeliverTask(
					pTask,
					pList,
					pEntry,
					0,
					*pTask->GetTaskMask(),
					ulCurtime,
					NULL,
					pSubTag,
					NULL,
					pEntry->m_ParentIndex);
			}
		}
		else if (ParentEntry.m_ChildIndex == 0xff) // 当复合任务无子任务时，即为完成
		{
			pList->RealignTask(pEntry, 0);
			ParentEntry.SetFinished();
			if (m_pParent->m_enumFinishType == enumTFTDirect)
				m_pParent->RecursiveAward(pTask, pList, &ParentEntry, ulCurtime, -1, pSubTag);
		}
		else
			pList->RealignTask(pEntry, 0);
	}
	else // Root Node
	{

#ifndef _TASK_CLIENT
		bool bSuccess = pEntry->IsSuccess();
#endif

		pList->RealignTask(pEntry, 0);

		if (pList->m_uUsedCount >= m_uDepth)
			pList->m_uUsedCount -= m_uDepth;
		else
		{
			char log[1024];
			sprintf(log, "Award, Used = %d", pList->m_uUsedCount);
			TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);

			pList->m_uUsedCount = 0;
		}

		if (!m_bHidden && pList->m_uTopShowTaskCount)
			pList->m_uTopShowTaskCount--;
		else if (m_bHidden && pList->m_uTopHideTaskCount)
			pList->m_uTopHideTaskCount--;
		else
			TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "Award, TopCount == 0");

#ifndef _TASK_CLIENT

		if (bSuccess && m_ulType < STORAGE_TASK_TYPES && _storage_task_info[m_ulType].refresh)
			GetTaskTemplMan()->UpdateStorage(pTask, pList, ulCurtime, bNeedRefreshNinthStorage);	//库任务完成时，带上是否需要刷新第九库的库任务的参数 Added 2012-09-07.
#endif

	}

#ifndef _TASK_CLIENT
	if (ad.m_ulLifeAgainFaction)
	{
		if (ad.m_ulLifeAgainFaction < 32)
		{
			pTask->SetLifeAgainOccup(ad.m_ulLifeAgainFaction-1);
		}
		else
		{
			pTask->SetLifeAgainOccup(ad.m_ulLifeAgainFaction);		
		}
		
		//转生重置任务
		GetTaskTemplMan()->RemoveLifeAgainTaskInList(pTask);
		
		task_notify_base notify;
		notify.reason = TASK_SVR_NOTIFY_CLEAR_LIFEAGAIN_FIN_REC;
		notify.task = 0;
		pTask->NotifyClient(&notify, sizeof(notify));
	}


	
	// 清除当前任务列表中非主线任务
	if (ad.m_bClearNoKeyActiveTask)
	{
		GetTaskTemplMan()->ClearNoKeyActiveTask(pTask);		
	}
	
	// 通知成就系统任务完成和完成次数
	unsigned short uFinishCount = 1;
	TaskFinishTimeList* pFinish = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pEntry_fin = (TaskFinishTimeEntry*)pFinish->Search(m_ID);	
	if (pEntry_fin)
	{
		uFinishCount = 	pEntry_fin->m_uFinishCount;	
	}
	pTask->OnTaskComplete(m_ID, uFinishCount);
#endif
	
}

bool ATaskTempl::IsNeedRefreshNinthStorage(ActiveTaskEntry* pEntry) const
{
	if ( !pEntry )
	{
		return true;
	}

	if ( 17 == pEntry->m_ID )	//现在就是针对太一轮触发的17号任务,太一轮不能刷新9号库任务
	{
		return false;
	}

	return true;
}

void ActiveTaskList::UpdateUsedCount()
{
	m_uUsedCount = 0;
	for (unsigned char i = 0; i < m_uTaskCount; i++)
	{
		const ATaskTempl* pTempl = m_TaskEntries[i].GetTempl();
		if (!pTempl) continue;
		if (pTempl->m_pParent) continue;
		m_uUsedCount += pTempl->m_uDepth;
	}
}

#ifdef _TASK_CLIENT

bool ATaskTempl::CanShowTask(TaskInterface* pTask) const
{
	if (GetTaskTemplMan()->IsForbiddenTask(m_ID))
		return false;

	unsigned long ulCurTime = TaskInterface::GetCurTime();

	if (m_pParent
	 || static_cast<ActiveTaskList*>(pTask->GetActiveTaskList())->GetEntry(GetID())
	 || m_ulDelvNPC && IsAutoDeliver()
	 || CheckTimetable(ulCurTime)
	 || CheckDeliverTime(pTask, ulCurTime)
	 || CheckFnshLst(pTask, ulCurTime)
	 || CheckMutexTask(pTask, ulCurTime)
	 || CheckLivingSkill(pTask)
	 || CheckPetConAndCiv(pTask)
	 || CheckSpecialAward(pTask)
	 || CheckTalismanValue(pTask)
	 || CheckPremFinishCount(pTask)
	 || CheckBattleScore(pTask)
	 || CheckSJBattleScore(pTask)
	 || CheckMasterPrentice(pTask)
	 || CheckCultivation(pTask)
	 || CheckConsumeTreasure(pTask)
	 || CheckPremTotalTreasure(pTask)
	 || CheckAccomp(pTask)
	 || CheckTransform(pTask)
	 || CheckAchievement(pTask)
	 || CheckTerritoryScore(pTask)
	 || CheckHasKing(pTask)
	 || CheckNationPosition(pTask)
	 || CheckKingScore(pTask)
	 || CheckBuildLevelCon(pTask))
		return false;

	unsigned long ulRet;

	if (m_bShowByDeposit && CheckDeposit(pTask)) return false;
	if (m_bShowByGender && CheckGender(pTask)) return false;
	if (m_bShowByItems && CheckItems(pTask)) return false;
	ulRet = CheckLevel(pTask);
	if (ulRet == TASK_PREREQU_FAIL_ABOVE_LEVEL || (m_bShowByLev && ulRet)) return false;
	if (m_bShowByOccup && CheckOccupation(pTask)) return false;
	if (m_bShowByPreTask && CheckPreTask(pTask)) return false;
	if (m_bShowByRepu && CheckRepu(pTask)) return false;
	if (m_bShowByTeam && CheckTeamTask(pTask)) return false;
	if (m_bShowByFaction && CheckFaction(pTask)) return false;
	if (m_bShowByPeriod && CheckPeriod(pTask)) return false;

	if (m_bShowByFengshenLvl && CheckFengshen(pTask)) return false;
	if(m_bShowByFactionGoldNote && CheckFactionGoldNote(pTask)) return false;
	if(m_bShowByInterObjId && CheckInterObj(pTask)) return false;
	return true;
}

#else

inline void _DeliverItem(
	TaskInterface* pTask,
	unsigned long ulTask,
	long lCurTime,
	const ITEM_WANTED& wi,
	unsigned long ulMulti)
{
	unsigned long ulCount = wi.m_ulItemNum * ulMulti;

	char log[1024];
	sprintf(log, "DeliverItem: Item id = %d, Count = %d", wi.m_ulItemTemplId, ulCount);
	TaskInterface::WriteLog(pTask->GetPlayerId(), ulTask, 1, log);
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), ulTask, 1, log);

	if (wi.m_bCommonItem)
	{
		long lPeriod;

		if (wi.m_bTimetable)
		{
			tm tmCur = *localtime(&lCurTime);
			long diff_days = (long)wi.m_cDayOfWeek - task_week_map[tmCur.tm_wday];
			long t = lCurTime - tmCur.tm_hour * 3600 - tmCur.tm_min * 60 - tmCur.tm_sec;
			t += diff_days * (24 * 3600);
			t += wi.m_cHour * 3600 + wi.m_cMinute * 60;

			if (t <= lCurTime)
				t += 7 * 24 * 3600;

			lPeriod = t - lCurTime;

			if (lPeriod <= 0)
			{
				TaskInterface::WriteKeyLog(pTask->GetPlayerId(), ulTask, 1, "DeliverItem: Wrong period");
				return;
			}
		}
		else
			lPeriod = wi.m_lPeriod;

		if(wi.m_ulRefineLevel)
			pTask->DeliverReinforceItem(wi.m_ulItemTemplId, wi.m_ulRefineLevel, wi.m_bBind, ulCount > 1 ? 0 : lPeriod);
		else
			pTask->DeliverCommonItem(wi.m_ulItemTemplId, ulCount, wi.m_bBind, ulCount > 1 ? 0 : lPeriod);
	}
	else
		pTask->DeliverTaskItem(wi.m_ulItemTemplId, ulCount);
}

unsigned long ATaskTempl::DeliverByAwardData(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	const AWARD_DATA* pAward,
	unsigned long ulCurTime,
	int nChoice) const
{
	unsigned long ulRet = 0;
	ATaskTempl* pNewTask;
	int i = 0;

	unsigned long ulMulti = CalcAwardMulti(
		pTask,
		pEntry,
		pEntry->m_ulTaskTime,
		ulCurTime
		);

	if (!ulMulti) return 1;

	unsigned long ulGold	= pAward->m_ulGoldNum * ulMulti;
	double dbExp			= 1.0 * pAward->m_ulExp * ulMulti;
	unsigned long ulSP		= pAward->m_ulSP * ulMulti;
	unsigned long ulRepu	= pAward->m_ulReputation * ulMulti;
	unsigned long nFamContrib = pAward->m_nFamContrib * ulMulti;

	unsigned long ulFengshenExp = pAward->m_ulFengshenExp * ulMulti;

	int nBonus			=	pAward->m_ulBonusNum * ulMulti;//Added 2011-02-18.
	int nBattleScore	=	pAward->m_ulBattleScore * ulMulti;//Added 2011-02-18.

	int iFactionGoldNote	= pAward->m_iFactionGoldNote * ulMulti;
	int iFactionGrass		= pAward->m_iFactionGrass * ulMulti;
	int iFactionMine		= pAward->m_iFactionMine * ulMulti;
	int iFactionMonsterCore = pAward->m_iFactionMonsterCore * ulMulti;
	int iFactionMonsterFood = pAward->m_iFactionMonsterFood * ulMulti;
	int iFactionMoney		= pAward->m_iFactionMoney * ulMulti;

	//计算转生经验
	float fExpCoef = 1.0f;
	unsigned long ulLifeAgainCnt = pTask->GetPlayerLifeAgainCnt();
	switch(ulLifeAgainCnt)
	{
	case 1:
		fExpCoef = pAward->m_fExpCoef ? pAward->m_fExpCoef : 1;
		break;
	case 2:
		fExpCoef = pAward->m_fExpCoef2 ? pAward->m_fExpCoef2 : 1;
		break;
	case 3:
		fExpCoef = pAward->m_fExpCoef3 ? pAward->m_fExpCoef3 : 1;
		break;
	default:
		break;
	}

	dbExp *= fExpCoef;

	int64_t ulExp = int64_t(dbExp);
	if (pAward->m_ulGoldNum)	pTask->DeliverGold(ulGold);
	if (pAward->m_ulExp)		pTask->DeliverExperience(ulExp);
	if (pAward->m_ulSP)			pTask->DeliverSP(ulSP);
	if (pAward->m_ulReputation)	pTask->DeliverReputation(ulRepu);
	if (pAward->m_nFamContrib)  pTask->DeliverFamilyContribution(nFamContrib);

	if (pAward->m_ulBonusNum)		pTask->DeliverBonus(nBonus);//Added 2011-02-18.
	if (pAward->m_ulBattleScore)	pTask->DeliverBattleScore(nBattleScore);//Added 2011-02-18.

	if (pAward->m_ulNewPeriod)	pTask->SetCurPeriod(pAward->m_ulNewPeriod);
	if (pAward->m_ulNewRelayStation) pTask->SetNewRelayStation(pAward->m_ulNewRelayStation);
	if (pAward->m_ulStorehouseSize) pTask->SetStorehouseSize(pAward->m_ulStorehouseSize);
	if (pAward->m_ulFactionStorehouseSize) pTask->SetFactionStorehouseSize(pAward->m_ulFactionStorehouseSize);
	if (pAward->m_ulPetInventorySize) pTask->SetPetInventorySize(pAward->m_ulPetInventorySize);
	if (pAward->m_ulMountInventorySize) pTask->SetMountInventorySize(pAward->m_ulMountInventorySize);
	if (pAward->m_ulFuryULimit) pTask->SetFuryUpperLimit(pAward->m_ulFuryULimit);
	if (pAward->m_bSetProduceSkill) pTask->SetProduceSkillFirstLev();
	if (pAward->m_ulProduceSkillExp) pTask->AddProduceSkillExp(pAward->m_ulProduceSkillExp);
	if (pAward->m_ulNewProfession) pTask->SetNewProfession(pAward->m_ulNewProfession);
	if (pAward->m_ulDoubleExpTime) pTask->SetDoubleExpTime(pAward->m_ulDoubleExpTime);

	if (pAward->m_nKingScore) pTask->DeliverKingScore(pAward->m_nKingScore);

	if (pAward->m_iTransformID != -1 && pAward->m_iTransformDuration && pAward->m_iTransformLevel != -1 && pAward->m_iTransformExpLevel != -1)
	{
		pTask->SetTransform(pAward->m_iTransformID, pAward->m_iTransformDuration, pAward->m_iTransformLevel, pAward->m_iTransformExpLevel, pAward->m_bTransformCover);
	}

	if (pAward->m_lTitle) pTask->GiveOneTitle(pAward->m_lTitle);
	if (pAward->m_lInventorySize) pTask->SetInventorySize(pAward->m_lInventorySize);
	if (pAward->m_lPocketSize) pTask->SetPocketSize(pAward->m_lPocketSize);
	if (pAward->m_lContribution) pTask->DeliverFactionContribution(pAward->m_lContribution);	

	if (pAward->m_bResetPKValue)
		pTask->ResetPKValue();
	else if (pAward->m_lPKValue)
		pTask->DeliverPKValue(pAward->m_lPKValue);

	if (pAward->m_bDivorce)
		pTask->Divorce();

	if (pAward->m_bDeviateMaster && pTask->IsPrentice())
	{
		pTask->DeviateMaster();
	}

	if (pAward->m_bOutMaster && pTask->IsPrentice())
	{
		pTask->OutMaster();
	}
	
	int64_t ulOutMasterExp = 0;
	if (pAward->m_bOutMasterExp && pTask->IsPrentice() && pTask->IsInTeam() && pTask->GetTeamMemberNum() == 2)
	{
		unsigned long ulOutLev = pTask->GetPlayerLevel();
		unsigned long ulInLev  = pTask->GetInMasterLev();
		
		if (ulInLev > 90)
		{
			ulInLev = 15;
		}
		
		float fOutMasterExp = ((ulOutLev-ulInLev)*1.0/90)*(1.64*ulOutLev-96.6)*1000000;
		if (fOutMasterExp > 0)
		{
			ulOutMasterExp = int64_t(fOutMasterExp);
			pTask->DeliverExperience(ulOutMasterExp);
		}				
	}

	if (pAward->m_lBuffId)
		pTask->DeliverBuff(pAward->m_lBuffId, pAward->m_lBuffLev);

	abase::vector <Task_Log_AwardRegionRepu> LogRegionRepuArr;
	Task_Log_AwardRegionRepu LogRegionRepu = {0};

	for (i = 0; i < SIZE_OF_ARRAY(pAward->m_aFriendships); i++)
	{
		long lFriendship = pAward->m_aFriendships[i] * ulMulti;

		if (lFriendship > 0)
		{
			pTask->DeliverRegionReputation(i, lFriendship);
			
			LogRegionRepu.lRegionIndex = i;
			LogRegionRepu.lRegionReputation = lFriendship;
			LogRegionRepuArr.push_back(LogRegionRepu);
		}
	}

	if (pAward->m_ulFriendshipResetSel)
	{
		for (int i=0; i<TASK_ZONE_FRIENDSHIP_COUNT; i++)
		{
			if (pAward->m_ulFriendshipResetSel & (1<<i))
			{
				pTask->ResetRegionReputation(i);

				LogRegionRepu.lRegionIndex = i;
				LogRegionRepu.lRegionReputation = -(pTask->GetRegionReputation(i));
				LogRegionRepuArr.push_back(LogRegionRepu);
			}						
		}
	}

	if (pAward->m_bSetCult)
	{
		pTask->SetCultivation(pAward->m_ulSetCult);		
	}

	if (pAward->m_ulClearCultSkill)
	{
		pTask->ClearCultSkill(pAward->m_ulClearCultSkill);
	}

	if (pAward->m_nFamilyMonRecordIndex && m_enumMethod == enumTMKillNumMonster)
	{
		unsigned int uMonCount = 0;

		for (unsigned long i = 0; i < m_ulMonsterWanted; i++)
		{
			const MONSTER_WANTED& mw = m_MonsterWanted[i];

			if (!mw.m_ulDropItemId)
				uMonCount += pEntry->m_wMonsterNum[i];
		}

		if (uMonCount)
		{
			TaskPairData p(pAward->m_nFamilyMonRecordIndex-1, uMonCount, 0);
			TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyUpdateMonsterRecord, m_ID, enumTaskReasonUpdateMonsterRecord, 0, &p, 1);
			pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		}
	}
	
	if (pAward->m_nFamilyValue)
	{
		TaskPairData p(pAward->m_nFamilyValueIndex, pAward->m_nFamilyValue, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyAddCommonValue, m_ID, enumTaskReasonAddCommonValue, 0, &p, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
	}

	abase::vector <Task_Log_AwardItem> LogItemArr;
	Task_Log_AwardItem LogItem = {0};

	if (pAward->m_ulCandItems)
	{
		if (nChoice < 0 || nChoice >= static_cast<int>(pAward->m_ulCandItems))
			nChoice = 0;

		const AWARD_ITEMS_CAND& ic = pAward->m_CandItems[nChoice];

		if (CanAwardItems(pTask, pEntry, &ic))
		{
			if (ic.m_bRandChoose)
			{
				float fProb = pTask->UnitRand();
				bool bGiven = false;

				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];

					if (wi.m_fProb == 1.0f) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
					else if (!bGiven)
					{
						if (fProb <= wi.m_fProb)
						{
							_DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
							bGiven = true;

							LogItem.ulItemID  = wi.m_ulItemTemplId;
							LogItem.ulItemCnt = wi.m_ulItemNum;
							LogItemArr.push_back(LogItem);
						}
						else fProb -= wi.m_fProb;
					}
				}
			}
			else
			{
				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];
					if (pTask->UnitRand() <= wi.m_fProb) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
					
					LogItem.ulItemID  = wi.m_ulItemTemplId;
					LogItem.ulItemCnt = wi.m_ulItemNum;					
					LogItemArr.push_back(LogItem);
				}
			}
		}
		else
			ulRet = 2;
	}

	if (pAward->m_ulNewTask)
	{
		pNewTask = GetTaskTemplMan()->GetTopTaskByID(pAward->m_ulNewTask);

		if (pNewTask)
		{
			pNewTask->CheckDeliverTask(
				pTask,
				0,
				NULL
				);
		}
		else
		{
			TaskInterface::WriteLog(
				pTask->GetPlayerId(),
				pAward->m_ulNewTask,
				0,
				"DeliverNewTask, Cant Find Task");
		}
	}

	if (pAward->m_ulTerminateTaskCnt)
	{
		for (unsigned long i=0; i<pAward->m_ulTerminateTaskCnt; i++)
		{
			if (pAward->m_ulTerminateTask[i]) 
			{
				ActiveTaskEntry* pTerminateEntry = pList->GetEntry(pAward->m_ulTerminateTask[i]);
				
				if (pTerminateEntry != NULL)
				{
					pList->ClearTask(pTask, pTerminateEntry, true);
					pList->UpdateTaskMask(*pTask->GetTaskMask());
					
					const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(pAward->m_ulTerminateTask[i]);
					if (pTempl)
						pTempl->NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_GIVE_UP, 0);
				}
				else
				{
					TaskInterface::WriteLog(pTask->GetPlayerId(), pAward->m_ulTerminateTask[i], 0, "TerminateTask, Can't Terminate Task");		
				}
			}
		}
	}

	if (pAward->m_ulCameraMove)
	{
		NotifyClient(pTask, pEntry, TASK_SVR_NOTIFY_CAMERA_MOVE, ulCurTime, pAward->m_ulCameraMove);
	}

	if (pAward->m_ulCircleGroupPoint)
	{
		pTask->UpdateCircleGroupPoint(pAward->m_ulCircleGroupPoint);
	}

	if (pAward->m_bSendMsg)
	{
		//根据任务属性中的变量（Key，目前定为3个）取得全局key/value中的value值 Added 2011-04-11.
		long lVariables[MAX_VARIABLE_NUM];
		memset( lVariables, 0, sizeof(long)*MAX_VARIABLE_NUM ); // 初始化为0
		for( int k = 0; k < MAX_VARIABLE_NUM; ++k )
		{
			if( m_lVariables[k] > 0 ) //是否需要判断key的合法性？
			{
				lVariables[k] = pTask->GetGlobalValue(m_lVariables[k]);
			}
		}

		pTask->SendMessage(m_ID, pAward->m_nMsgChannel, 0, lVariables);//Modified 2011-04-11
	}

	if (pAward->m_ulClearCountTask)
	{
		const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(pAward->m_ulClearCountTask);

		if (pTempl && pTempl->m_bRecFinishCountGlobal)
		{
			TaskPreservedData tpd;
			tpd.reason			= TASK_GLOBAL_CLEAR_FINISH_COUNT;
			tpd.m_ulSrcTask		= pAward->m_ulClearCountTask;
			tpd.m_ulSubTaskId	= 0;
			tpd.m_ulRcvNum		= 0;
			TaskQueryGlobalData(pAward->m_ulClearCountTask, pTask->GetPlayerId(), &tpd, sizeof(tpd));
		}
		else
		{
			TaskFinishTimeList* pFinish = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
			TaskFinishTimeEntry* p = (TaskFinishTimeEntry*)pFinish->Search(pAward->m_ulClearCountTask);

			if (p)
			{
				p->m_uFinishCount = 0;
				p->m_ulUpdateTime = 0;
			}

			if (pTempl)
				pTempl->NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_FINISH_COUNT, ulCurTime, 0);
		}
	}

	//给指定角色发奖
	if (pAward->m_bAwardSpecifyRole)
	{
		DeliverAwardToSpecifyRole(pTask, pAward->m_pAwardSpecifyRole, pAward->m_ulRoleSelected);
	}

	if (pAward->m_bClearSkillPoints)
	{
		pTask->ClearNormalSkill();
	}

	if (pAward->m_bClearBookPoints)
	{
		pTask->ClearTalentSkill();
	}

	int64_t ulExpExt = 0;
	if (pAward->m_nParaExpSel)
	{
		float fRetVal = CalcAwardByParaExp(pTask, pAward)*ulMulti;
		switch(pAward->m_nParaExpSel)
		{
		case 1: //经验
			{
				ulExpExt = int64_t(fRetVal);
				pTask->DeliverExperience(ulExpExt);				
			}
			break;
		default:
		    break;
		}				
	}

	//改变全局Value
	for (i=0; i<pAward->m_lChangeKeyArr.size(); i++)
	{
		long lKey = pAward->m_lChangeKeyArr[i];
		
		if (lKey == 0)
			continue;
		
		bool bChangeType = pAward->m_bChangeTypeArr[i];
		long lValue = pAward->m_lChangeKeyValueArr[i];
		
		if (bChangeType)
		{
			pTask->ModifyGlobalValue(lKey, lValue);
		}
		else
		{
			pTask->PutGlobalValue(lKey, lValue);
		}
	}

	//宋金战场
	if (pAward->m_bResetSJBattleScore)
	{
		pTask->ResetSJBattleScore();
	}
	else if (pAward->m_nSJBattleScore)
	{
		pTask->DeliverSJBattleScore(pAward->m_nSJBattleScore * ulMulti);
	}

	if (pAward->m_ulTransWldId)
		pTask->TransportTo(pAward->m_ulTransWldId, pAward->m_TransPt.v, pAward->m_lMonsCtrl);
	else if (pAward->m_lMonsCtrl)
		pTask->SetMonsterController(pAward->m_lMonsCtrl, pAward->m_bTrigCtrl);

	if (pAward->m_ulMonCtrlCnt)
	{
		if (pAward->m_bRanMonCtrl)
		{
			float fProb = pTask->UnitRand();
			
			for (unsigned long i = 0; i < pAward->m_ulMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = pAward->m_MonCtrl[i];				
				
				if (MC.fGetProb == 1.0f)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
				else if	(MC.fGetProb >= fProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
					break;
				}
				else
					fProb -= MC.fGetProb;				
			}			
		}
		else
		{
			for (unsigned long i = 0; i < pAward->m_ulMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = pAward->m_MonCtrl[i];
				if (pTask->UnitRand() <= MC.fGetProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
			}
		}
	}	

	if(pAward->m_iPhaseCnt)
	{
		for(int i = 0; i < pAward->m_iPhaseCnt; i++)
			pTask->SetPhase(pAward->m_Phase[i].iPhaseID, pAward->m_Phase[i].bOpenClose);

	}

	if(pAward->m_iAuctionCnt)
	{
		for(int i = 0; i < pAward->m_iAuctionCnt; i++)
		{
			if(pAward->m_Auction[i].m_ulAuctionItemId)
				pTask->DeliverFactionAuctionItem(pAward->m_Auction[i].m_ulAuctionItemId, pAward->m_Auction[i].m_fAuctionProp);
		}
	}

	if(pAward->m_iTravelItemId && pAward->m_iTravelTime && pAward->m_fTravelSpeed >= 0 && pAward->m_iTravelPath)
		pTask->TravelRound(pAward->m_iTravelItemId, pAward->m_iTravelTime, pAward->m_fTravelSpeed, pAward->m_iTravelPath);

	if (pAward->m_ulFengshenExp) 
		pTask->DeliverFengshenExp(ulFengshenExp);

	if (pAward->m_bFengshenDujie)
		pTask->DeliverFengshenDujie();

	//发放帮派属性相关奖励
	if(pAward->m_iFactionGoldNote)
		pTask->DeliverFactionGoldNote(iFactionGoldNote);
	if(pAward->m_iFactionGrass)
		pTask->DeliverFactionGrass(iFactionGrass);
	if(pAward->m_iFactionMine)
		pTask->DeliverFactionMine(iFactionMine);
	if(pAward->m_iFactionMonsterCore)
		pTask->DeliverFactionMonsterCore(iFactionMonsterCore);
	if(pAward->m_iFactionMonsterFood)
		pTask->DeliverFactionMonsterFood(iFactionMonsterFood);
	if(pAward->m_iFactionMoney)
		pTask->DeliverFactionMoney(iFactionMoney);
	if(pAward->m_bBuildingProgress)
	{
		pTask->OnFBaseBuildingProgress(m_ID, 1);		
	}

	//Added 2012-04-18. 任务奖励开启元魂装备位
	if (pAward->m_bOpenSoulEquip)
	{
		pTask->DeliverOpenSoulEquip();
	}

	//发放额外奖励
	if( pEntry->IsCanDeliverExtraAward() )//能否发放额外奖励标志等于true,即全局条件满足
	{
		DeliverExtraAward( 
			pTask,
			pList,
			pEntry,
			pAward,
			ulCurTime,
			ulRet,
			LogItemArr,
			ulMulti);
	}
	
	//每天第一次完成第一个帮派任务 发放额外奖励
	if(m_bFaction)
	{
		TaskFinishTimeList* pFinish = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
		TaskFinishTimeEntry* pFinishEntry = (TaskFinishTimeEntry*)pFinish->Search(m_ID);

		if(pFinishEntry && pFinishEntry->m_uFinishCount == 1)
			DeliverFactionExtraAward( 
			pTask,
			pList,
			pEntry,
			pAward,
			ulCurTime,
			ulRet,
			LogItemArr,
			ulMulti);
	}

	char log[1024];
	sprintf(log,
		"DeliverByAwardData: success = %d, gold = %d, exp = %lld, sp = %d, reputation = %d",
		pEntry->IsSuccess() ? 1 : 0,
		ulGold,
		ulExp+ulOutMasterExp+ulExpExt,
		ulSP,
		ulRepu
		);
	
	TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, log);
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), m_ID, 1, log);

	//新日志
	TaskInterface::WriteTaskAwardLog(pTask->GetPlayerId(), m_ID, ulExp+ulOutMasterExp+ulExpExt, ulGold, LogRegionRepuArr, LogItemArr);

	return ulRet;
}

//Added 2011-03-04.
unsigned long ATaskTempl::DeliverExtraAward(
								TaskInterface* pTask, 
								ActiveTaskList* pList, 
								ActiveTaskEntry* pEntry, 
								const AWARD_DATA* pAward, 
								unsigned long ulCurTime,
								unsigned long& ulRet, 
								abase::vector<Task_Log_AwardItem>& LogItemArr,
								unsigned long ulMulti
								) const
{
	//定义并初始化发放奖励物品的log
	Task_Log_AwardItem LogItem = {0};
	
	//判断是否需要发放额外的奖励物品
	if(pAward->m_ulExtraCandItems)
	{
		const AWARD_ITEMS_CAND& ic = pAward->m_ExtraCandItems[0];//只能发放第一个候选项，没有选择机制

		if (CanAwardItems(pTask, pEntry, &ic))//这个判断好像有些多余，因为在实际发放奖励之前，已经进行了检测。//该函数也不太正确。
		{
			if (ic.m_bRandChoose)
			{
				float fProb = pTask->UnitRand();
				bool bGiven = false;

				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];

					if (wi.m_fProb == 1.0f) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
					else if (!bGiven)
					{
						if (fProb <= wi.m_fProb)
						{
							_DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
							bGiven = true;

							LogItem.ulItemID  = wi.m_ulItemTemplId;
							LogItem.ulItemCnt = wi.m_ulItemNum;
							LogItemArr.push_back(LogItem);
						}
						else fProb -= wi.m_fProb;
					}
				}
			}
			else
			{
				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];
					if (pTask->UnitRand() <= wi.m_fProb) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
					
					LogItem.ulItemID  = wi.m_ulItemTemplId;
					LogItem.ulItemCnt = wi.m_ulItemNum;					
					LogItemArr.push_back(LogItem);
				}
			}
		}
		else
			ulRet = 2;
	}
	
	//开启/关闭怪物控制器
	if(pAward->m_ulExtraMonCtrlCnt)
	{
		if(pAward->m_bExtraRanMonCtrl)//随机选择一个控制器
		{
			float fProb = pTask->UnitRand();
			
			for (unsigned long i = 0; i < pAward->m_ulExtraMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = pAward->m_ExtraMonCtrl[i];				
				
				if (MC.fGetProb == 1.0f)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
				else if	(MC.fGetProb >= fProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
					break;
				}
				else
					fProb -= MC.fGetProb;				
			}			
		}
		else
		{
			for (unsigned long i = 0; i < pAward->m_ulExtraMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = pAward->m_ExtraMonCtrl[i];
				if (pTask->UnitRand() <= MC.fGetProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
			}
		}
	}

	//开启系统喊话
	if( pAward->m_bSendExtraMsg )//需要喊话
	{
		//首先计算好param参数，然后调用发送消息函数
		int param = 0;
		CalcAwardDataIndex(
		pTask, 
		pAward, 
		pEntry, 
		pEntry->m_ulTaskTime, 
		ulCurTime, 
		param);

		//根据额外奖励中的变量（Key，目前定为3个）取得全局key/value中的value值 Added 2011-03-31.
		long lVariables[MAX_VARIABLE_NUM];
		memset( lVariables, 0, sizeof(long)*MAX_VARIABLE_NUM ); // 初始化为0
		for( int k = 0; k < MAX_VARIABLE_NUM; ++k )
		{
			if( pAward->m_lVariable[k] > 0 ) //是否需要判断key的合法性？
			{
				lVariables[k] = pTask->GetGlobalValue(pAward->m_lVariable[k]);
			}
		}

		//注意，SendMessage的param参数现在有不同的含义，将整个int变量按照4个字节分成4段，最高字节表示特殊标记，
		//如，最高位为1：表示额外喊话奖励，而次高字节表示当前任务是否成功的信息；再次高字节表示任务的奖励方式；
		//最低字节表示：在多个奖励数据数组中的索引index
		pTask->SendMessage(m_ID, pAward->m_nExtraMsgChannel, param, lVariables);
	}

}


unsigned long ATaskTempl::DeliverFactionExtraAward(
	TaskInterface* pTask, 
	ActiveTaskList* pList, 
	ActiveTaskEntry* pEntry, 
	const AWARD_DATA* pAward, 
	unsigned long ulCurTime,
	unsigned long& ulRet, 
	abase::vector<Task_Log_AwardItem>& LogItemArr,
	unsigned long ulMulti
	) const
{
	Task_Log_AwardItem LogItem = {0};

	if(pAward->m_ulFactionExtraCandItems)
	{
		const AWARD_ITEMS_CAND& ic = pAward->m_FactionExtraCandItems[0];

		if (CanAwardItems(pTask, pEntry, &ic))
		{
			if (ic.m_bRandChoose)
			{
				float fProb = pTask->UnitRand();
				bool bGiven = false;

				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];

					if (wi.m_fProb == 1.0f) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
					else if (!bGiven)
					{
						if (fProb <= wi.m_fProb)
						{
							_DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);
							bGiven = true;

							LogItem.ulItemID  = wi.m_ulItemTemplId;
							LogItem.ulItemCnt = wi.m_ulItemNum;
							LogItemArr.push_back(LogItem);
						}
						else fProb -= wi.m_fProb;
					}
				}
			}
			else
			{
				for (unsigned long i = 0; i < ic.m_ulAwardItems; i++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[i];
					if (pTask->UnitRand() <= wi.m_fProb) _DeliverItem(pTask, m_ID, ulCurTime, wi, ulMulti);

					LogItem.ulItemID  = wi.m_ulItemTemplId;
					LogItem.ulItemCnt = wi.m_ulItemNum;					
					LogItemArr.push_back(LogItem);
				}
			}
		}
		else
			ulRet = 2;
	}

	int iFactionExtraGoldNote	= pAward->m_iFactionExtraGoldNote * ulMulti;
	int iFactionExtraGrass		= pAward->m_iFactionExtraGrass * ulMulti;
	int iFactionExtraMine		= pAward->m_iFactionExtraMine * ulMulti;
	int iFactionExtraMonsterCore = pAward->m_iFactionExtraMonsterCore * ulMulti;
	int iFactionExtraMonsterFood = pAward->m_iFactionExtraMonsterFood * ulMulti;
	int iFactionExtraMoney		= pAward->m_iFactionExtraMoney * ulMulti;

	//发放帮派属性相关奖励
	if(pAward->m_iFactionGoldNote)
		pTask->DeliverFactionGoldNote(iFactionExtraGoldNote);
	if(pAward->m_iFactionGrass)
		pTask->DeliverFactionGrass(iFactionExtraGrass);
	if(pAward->m_iFactionMine)
		pTask->DeliverFactionMine(iFactionExtraMine);
	if(pAward->m_iFactionMonsterCore)
		pTask->DeliverFactionMonsterCore(iFactionExtraMonsterCore);
	if(pAward->m_iFactionMonsterFood)
		pTask->DeliverFactionMonsterFood(iFactionExtraMonsterFood);
	if(pAward->m_iFactionMoney)
		pTask->DeliverFactionMoney(iFactionExtraMoney);
}


void _deliver_award_to_role(TaskInterface* pTask, const AWARD_DATA *pAward, unsigned long ulPlayerID)
{
	if (pAward->m_ulGoldNum)	pTask->DeliverGold(pAward->m_ulGoldNum, ulPlayerID);
	if (pAward->m_ulExp)		pTask->DeliverExperience(pAward->m_ulExp, ulPlayerID);
	if (pAward->m_ulSP)			pTask->DeliverSP(pAward->m_ulSP, ulPlayerID);
	if (pAward->m_ulReputation)	pTask->DeliverReputation(pAward->m_ulReputation, ulPlayerID);
	if (pAward->m_lTitle)		pTask->GiveOneTitle(pAward->m_lTitle, ulPlayerID);		
	if (pAward->m_ulCandItems)
	{
		const AWARD_ITEMS_CAND& ic = pAward->m_CandItems[0];				
		for (int i = 0; i < ic.m_ulAwardItems; i++)
		{
			const ITEM_WANTED& wi = ic.m_AwardItems[i];
			
			if (wi.m_bCommonItem)
				pTask->DeliverCommonItem(wi.m_ulItemTemplId, wi.m_ulItemNum, wi.m_bBind, 0, ulPlayerID);
			else
				pTask->DeliverTaskItem(wi.m_ulItemTemplId, wi.m_ulItemNum, ulPlayerID);
		}				
	}
}

void ATaskTempl::DeliverAwardToSpecifyRole(TaskInterface* pTask, const AWARD_DATA *pAward, unsigned long ulRoleSelected) const
{
	for (int nRoleType=0; nRoleType<MAX_AWARD_SEL_ROLE; nRoleType++)
	{
		if (ulRoleSelected & (1<<nRoleType))
		{
			switch(nRoleType)
			{
			case 0: //向队长发奖励
				{
					if (!pTask->IsInTeam())
						continue;
					
					unsigned long ulCapID = pTask->GetTeamMemberId(0);
					_deliver_award_to_role(pTask, pAward, ulCapID);
				}
				break;
			case 1: //队员
				{
					if (!pTask->IsInTeam())
						continue;
					const int nMemNum = pTask->GetTeamMemberNum();					
					for (int i=1; i<nMemNum; i++)
					{
						unsigned long ulMemID = pTask->GetTeamMemberId(i);
						_deliver_award_to_role(pTask, pAward, ulMemID);					
					}
				}
				break;
			case 2: //师傅
				{
					if (!pTask->IsInTeam())
						continue;

					unsigned long ulCapID = pTask->GetTeamMemberId(0);

					_deliver_award_to_role(pTask, pAward, ulCapID);

					if (m_bPrenticeTask && pTask->IsPrentice())
					{
						if (pAward->m_lMasterMoral)
						{
							pTask->DeliverRegionReputation(8, pAward->m_lMasterMoral, ulCapID);	//发放师德值
						}

						if (pAward->m_bOutMasterMoral)
						{
							unsigned long ulOutLev = pTask->GetPlayerLevel();
							unsigned long ulInLev  = pTask->GetInMasterLev();

							if (ulInLev > 90)
							{
								ulInLev = 15;
							}

							unsigned long ulOutMasterMoral = 0;							
							float fOutMasterMoral = ((ulOutLev-ulInLev)*1.0/90)*(9.35*ulOutLev-552.4);
							if (fOutMasterMoral > 0)
							{
								ulOutMasterMoral = static_cast<unsigned long>(fOutMasterMoral);
							}

							pTask->DeliverRegionReputation(8, ulOutMasterMoral, ulCapID); //发放出师师德
						}
					}					
				}
				break;
			case 3: //徒弟
				{
					if (!pTask->IsInTeam())
						continue;

					unsigned long ulCapID = pTask->GetTeamMemberId(0);

					const int nMemNum = pTask->GetTeamMemberNum();
					for (int i=1; i<nMemNum; i++)
					{
						if (m_bMPTask)
						{
							task_team_member_info Mem;
							pTask->GetTeamMemberInfo(i, &Mem);
							if (Mem.m_ulMasterID != ulCapID)
								continue;
						}
						unsigned long ulMemID = pTask->GetTeamMemberId(i);
						_deliver_award_to_role(pTask, pAward, ulMemID);					
					}
				}
				break;
			case 4: //族员(包括族长)
				{
					if (!pTask->IsInFamily())
						continue;

					int nFamilyID = pTask->GetFamilyID();					
					if (pAward->m_ulExp) 
						pTask->DeliverFamilyExperience(nFamilyID, pAward->m_ulExp);
				}
				break;
			default: 
			    break;
			}
		}
	}
}
inline unsigned long ATaskTempl::CheckGlobalExpressions(TaskInterface* pTask, bool bPrem) const
{
	int nRet1, nRet2;

	if(bPrem && m_bPremNeedComp)
	{
		nRet1 = CheckGlobalExpression(pTask, m_PremCompExp1);
		nRet2 = CheckGlobalExpression(pTask, m_PremCompExp2);

		if(m_nPremExp1AndOrExp2 == 0)
		{
			if(nRet1 == 0 || nRet2 == 0)
				return 0;
			else
				return nRet1;
		}
		else 
		{
			if(nRet1 != 0)
				return nRet1;
			else if(nRet2 != 0)
				return nRet2;
			else 
				return 0;
		}
	}
	else if(m_bFinNeedComp)
	{
		nRet1 = CheckGlobalExpression(pTask, m_FinCompExp1);
		nRet2 = CheckGlobalExpression(pTask, m_FinCompExp2);

		if(m_nFinExp1AndOrExp2 == 0)
		{
			if(nRet1 == 0 || nRet2 == 0)
				return 0;
			else
				return nRet1;
		}
		else 
		{
			if(nRet1 != 0)
				return nRet1;
			else if(nRet2 != 0)
				return nRet2;
			else 
				return 0;
		}
	}

	return 0;
}
inline unsigned long ATaskTempl::CheckGlobalExpression(TaskInterface* pTask, const COMPARE_EXPRESSION& CompExp) const
{
	int nRetLeft, nRetRight;
	try
	{
		TaskExpAnalyser Analyser;
		nRetLeft  = Analyser.Run(pTask, CompExp.arrExpLeft, enumTaskCalcGlobalVal);
		nRetRight = Analyser.Run(pTask, CompExp.arrExpRight, enumTaskCalcGlobalVal);
	}
	catch (char* szErr)
	{
		char log[1024];
		sprintf(log, "CheckGlobalExpression, Expression run err: %s", szErr);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);
		
		return TASK_PREREQU_FAIL_EXP_RUN;
	}

	switch(CompExp.nCompOper)
	{
	case 0:
		{
			if (nRetLeft > nRetRight)
				return 0;
		}
		break;
	case 1:
		{
			if (nRetLeft == nRetRight)
				return 0;
		}
		break;
	case 2:
		{
			if (nRetLeft < nRetRight)
				return 0;
		}
		break;
	default:
		break;
	}
	
	return TASK_PREREQU_FAIL_GLOBAL_EXP;		
}

bool ATaskTempl::CheckTeamMemPos(TaskInterface* pTask, ActiveTaskEntry* pEntry, float fSqrDist) const
{
	const int nMemNum = pTask->GetTeamMemberNum();
	unsigned long ulPlayer = pTask->GetPlayerId();
	float fDist, self_pos[3], pos[3], x, y, z;
	int i;

	unsigned long ulWorldId = pTask->GetPos(self_pos);

	for (i = 0; i < nMemNum; i++)
	{
		unsigned long ulOther = pTask->GetTeamMemberId(i);
		if (ulOther == ulPlayer) continue;

		if (pTask->GetTeamMemberPos(i, pos) != ulWorldId)
			return false;

		x = self_pos[0] - pos[0];
		y = self_pos[1] - pos[1];
		z = self_pos[2] - pos[2];
		
		fDist = x * x + y * y + z * z;

		if (fDist > fSqrDist) return false;
	}

	return true;
}

bool ATaskTempl::OnDeliverTeamMemTask(
	TaskInterface* pTask,
	TaskGlobalData* pGlobal) const
{
	if (!m_bTeamwork || pTask->IsCaptain())
		return false;
 
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	unsigned long ulCurTime = pTask->GetCurTime();

	if (m_ulTeamMemsWanted)
	{
		unsigned long i;
		task_team_member_info MemInfo;
		int nFamilyId = 0;

		MemInfo.m_ulLevel		= pTask->GetPlayerLevel();
		MemInfo.m_ulOccupation	= pTask->GetPlayerOccupation();
		MemInfo.m_bMale			= pTask->IsMale();
		MemInfo.m_nFamilyId     = pTask->GetFamilyID();
		MemInfo.m_ulLifeAgainCnt= pTask->GetPlayerLifeAgainCnt();

		if (pTask->GetTeamMemberNum())
		{
			task_team_member_info CapInfo;
			pTask->GetTeamMemberInfo(0, &CapInfo);
			nFamilyId = CapInfo.m_nFamilyId;
		}

		for (i = 0; i < m_ulTeamMemsWanted; i++)
		{
			const TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];

			if (!tmw.IsMeetBaseInfo(&MemInfo, nFamilyId))
				continue;

			if (!tmw.m_ulTask) break; // 无队员任务则发放队长任务

			const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(tmw.m_ulTask);

			if (pTempl && pTempl->CheckDeliverTask(
				pTask,
				0,
				this == pTempl ? pGlobal : NULL,
				true,
				true,
				m_ID) == 0)
				return true;
			else
				return false;
		}

		if (i == m_ulTeamMemsWanted)
			return false;
	}

	return CheckDeliverTask(pTask, 0, pGlobal, true, true, 0) == 0;
}

bool ATaskTempl::OnDelvierMasterPrenticeTask(TaskInterface* pTask) const
{
	unsigned long ulPlayerLev = pTask->GetPlayerLevel();
	unsigned long ulTaskID = 0;
	
	//获取相应级别的任务
	for (int i = 0; i < m_ulMPTaskCnt; i++)
	{
		if (ulPlayerLev <= m_MPTask[i].m_ulLevLimit)
		{
			ulTaskID = m_MPTask[i].m_ulTaskID;
			break;
		}
	}

	if (ulTaskID == 0)
		return false;

	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskID);
	if (pTempl)
	{
		return pTempl->CheckDeliverTask(pTask, 0, NULL, true) == 0;
	}	

	return false;
}

bool ATaskTempl::CheckGlobalRequired(
	TaskInterface* pTask,
	unsigned long ulSubTaskId,
	const TaskPreservedData* pPreserve,
	const TaskGlobalData* pGlobal) const
{
	TaskPreservedData tpd;

	switch (pPreserve->reason)
	{
	case TASK_GLOBAL_CHECK_RCV_NUM:

		if (m_bTeamwork
		 && m_bRcvByTeam
		 && pTask->IsInTeam()
		 && pTask->GetTeamMemberNum() + pGlobal->m_ulReceiverNum > m_ulMaxReceiver || pGlobal->m_ulReceiverNum >= m_ulMaxReceiver)
		{
			TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, "CheckGlobal: Reach Max Num");
			return false;
		}

		if (m_ulPremise_Cotask)
		{
			switch (m_ulCoTaskCond)
			{
			case COTASK_CORRESPOND:
				return pGlobal->m_ulReceiverNum < pPreserve->m_ulRcvNum;
			case COTASK_ONCE:
				return pPreserve->m_ulRcvNum > 0;
			}
		}

		break;

	case TASK_GLOBAL_CHECK_ADD_MEM:

		return pGlobal->m_ulReceiverNum < m_ulMaxReceiver;

	case TASK_GLOBAL_CHECK_COTASK:

		tpd.reason			= TASK_GLOBAL_CHECK_RCV_NUM;
		tpd.m_ulSrcTask		= m_ID;
		tpd.m_ulSubTaskId	= pPreserve->m_ulSubTaskId;
		tpd.m_ulRcvNum		= pGlobal->m_ulReceiverNum;

		TaskQueryGlobalData(m_ID, pTask->GetPlayerId(), &tpd, sizeof(tpd));
		return false;

	case TASK_GLOBAL_CHECK_FINISH_COUNT:

		return pGlobal->m_ulFinishCount >= m_ulPremGlobalCount;
	}

	return true;
}

bool ATaskTempl::QueryGlobalRequired(TaskInterface* pTask, unsigned long ulSubTaskId, unsigned short reason) const
{
	unsigned long ulTask = m_ID;

	if (reason == TASK_GLOBAL_CHECK_RCV_NUM)
	{
		if (m_ulPremise_Cotask)
		{
			reason = TASK_GLOBAL_CHECK_COTASK;
			ulTask = m_ulPremise_Cotask;
		}
		else if (m_ulMaxReceiver == 0)
			return true;
	}
	else if (reason == TASK_GLOBAL_CHECK_ADD_MEM)
	{
		if (m_ulMaxReceiver == 0)
			return true;
	}
	else if (reason == TASK_GLOBAL_CHECK_FINISH_COUNT)
	{
		if (m_ulPremGlobalTask == 0)
			return true;

		ulTask = m_ulPremGlobalTask;
	}

	TaskPreservedData tpd;
	tpd.m_ulSrcTask		= m_ID;
	tpd.m_ulSubTaskId	= ulSubTaskId;
	tpd.reason			= reason;
	TaskQueryGlobalData(ulTask,	pTask->GetPlayerId(), &tpd,	sizeof(tpd));

	char log[1024];
	sprintf(log, "CheckGlobalSnd: QueryTask = %d, reason = %d", ulTask, reason);
	TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, log);
	return false;
}

unsigned long ATaskTempl::CheckDeliverTask(
	TaskInterface* pTask,
	unsigned long ulSubTaskId,
	TaskGlobalData* pGlobal,
	bool bNotifyErr,
	bool bMemTask,
	unsigned long ulCapId) const
{
	const ATaskTempl* pSubTempl = NULL;

	if (m_bChooseOne)
	{
		pSubTempl = GetConstSubById(ulSubTaskId);

		if (!pSubTempl)
			return TASK_PREREQU_FAIL_WRONG_SUB;
	}

	ActiveTaskList* pList = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	unsigned long ulCurTime = pTask->GetCurTime();


	unsigned long ulRet = CheckPrerequisite(pTask, pList, ulCurTime, true, !bMemTask);

	if (ulRet)
	{
		if (bNotifyErr && !IsAutoDeliver()) NotifyClient(
			pTask,
			NULL,
			TASK_SVR_NOTIFY_ERROR_CODE,
			0,
			ulRet
		);

		return ulRet;
	}
	
	// 清除玩家某些非法状态(仅自动发放任务有效)
	if (m_bAutoDeliver && m_bClearSomeIllegalStates)
	{
		pTask->ClearSomeIllegalStates();
	}

	if (!RemovePrerequisiteItem(pTask))
		return TASK_PREREQU_FAIL_INDETERMINATE;

	// 此处召唤出指定归属怪物
	if (!SummonPremMonsters(pTask))
		return TASK_PREREQU_FAIL_SUMMON_MONSTER;
	
	// 召唤出要护送的NPC
	if(m_enumMethod == enumTMProtectNPC)
		pTask->ProtectNPC(m_ID, m_ulNPCToProtect, m_ulProtectTimeLen);

	task_sub_tags tags;
	memset(&tags, 0, sizeof(tags));

	if (pSubTempl)
		tags.sub_task = static_cast<unsigned short>(pSubTempl->m_ID);

	DeliverTask(pTask, pList, NULL, ulCapId, *pTask->GetTaskMask(), ulCurTime, pSubTempl, &tags, pGlobal);

	if (m_lAvailFrequency != enumTAFNormal)
		static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList())->AddOrUpdateDeliverTime(m_ID, ulCurTime);
	
	if (!m_bRecFinishCount && m_ulMaxFinishCount > 0)	// 若要记录开启次数
		static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList())->AddOrUpdateFinishCount(m_ID);
	
	//记录动态任务对应的全局变量值
	if(m_lDynFinishClearTime)
	{
		TaskFinishTimeList* pFinish = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
		TaskFinishTimeEntry* pEntry = (TaskFinishTimeEntry*)pFinish->Search(m_ID);
		
		//第一次接动态任务的时候，把动态任务写到TaskFinishTimeList里面，与发放时间间隔限制不冲突
		pFinish->AddOrUpdateDeliverTime(m_ID, ulCurTime);

		//防止m_ulUpdateTime的值被修改， 任务每次发放完都设为全局变量的值
		pEntry = (TaskFinishTimeEntry*)pFinish->Search(m_ID);
		if(pEntry)
			pEntry->m_ulUpdateTime = pTask->GetGlobalValue(m_lDynFinishClearTime);
	}


	// 向客户端发出通知
	NotifyClient(
		pTask,
		reinterpret_cast<const ActiveTaskEntry*>(ulCapId),
		TASK_SVR_NOTIFY_NEW,
		ulCurTime,
		reinterpret_cast<unsigned long>(&tags));

	// 发放师徒任务
    if (m_bMPTask)
		DeliverMasterPrenticeTask(pTask);  
	else if (!bMemTask && m_bTeamwork && m_bRcvByTeam && !m_bSharedByFamily)
		DeliverTeamMemTask(pTask, pGlobal, ulCurTime);

	if(m_iPhaseCnt)
	{
		for(int i = 0; i < m_iPhaseCnt; i++)
			pTask->SetPhase(m_Phase[i].iPhaseID, m_Phase[i].bOpenClose);
	}

	if (m_bTransTo)
		pTask->TransportTo(m_ulTransWldId, m_TransPt.v, m_lMonsCtrl);
	else if (m_lMonsCtrl)
		pTask->SetMonsterController(m_lMonsCtrl, m_bTrigCtrl);

	if (m_ulMonCtrlCnt)
	{
		if (m_bRanMonCtrl)
		{
			float fProb = pTask->UnitRand();
			
			for (unsigned long i = 0; i < m_ulMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = m_MonCtrl[i];				
				
				if (MC.fGetProb == 1.0f)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
				else if	(MC.fGetProb >= fProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
					break;
				}
				else
					fProb -= MC.fGetProb;				
			}			
		}
		else
		{
			for (unsigned long i = 0; i < m_ulMonCtrlCnt; i++)
			{
				const MONSTER_CTRL& MC = m_MonCtrl[i];
				if (pTask->UnitRand() <= MC.fGetProb)
				{
					pTask->SetMonsterController(MC.lMonCtrlID, MC.bOpenClose);
				}
			}
		}
	}

	if (m_bSharedByFamily && _is_header(pTask->GetFactionRole()))
	{
		TaskPairData p(m_ID, ulCurTime, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyAddSharedTask, 0, enumTaskReasonAddSharedTask, 0, &p, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
	}
	
	//改变全局Value
	for (int i=0; i<m_lChangeKeyArr.size(); i++)
	{
		long lKey = m_lChangeKeyArr[i];
		
		if (lKey == 0)
			continue;
		
		bool bChangeType = m_bChangeTypeArr[i];
		long lValue = m_lChangeKeyValueArr[i];
		
		if (bChangeType)
		{
			pTask->ModifyGlobalValue(lKey, lValue);
		}
		else
		{
			pTask->PutGlobalValue(lKey, lValue);
		}
	}

	// 摄像机移动
	if (m_ulCameraMove)
	{
		NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_CAMERA_MOVE, 0, m_ulCameraMove);
	}

	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), m_ID, 1, "CheckDeliverTask");
	return 0;
}

bool ATaskTempl::DeliverAward(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	int nChoice,
	bool bNotifyTeamMem,
	TaskGlobalData* pGlobal) const
{
	char log[1024];
	sprintf(log, "DeliverAward: Choice = %d", nChoice);
	TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, log);
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), m_ID, 1, log);

	// 检查时间
	unsigned long ulCurTime = pTask->GetCurTime();
	RecursiveCheckTimeLimit(pTask, pList, pEntry, ulCurTime);

	if (pEntry->IsSuccess())
	{
		if (!RecursiveCheckParent(pTask, pList, pEntry))
			pEntry->ClearSuccess();
		else if (m_bFamilyHeader && !_is_header(pTask->GetFactionRole()) && !m_bSharedByFamily)
			pEntry->ClearSuccess();
	}

	// 处理组队失败情况
	if (!pEntry->IsAwardNotifyTeam() && bNotifyTeamMem && pEntry->GetCapOrSelf()->m_bTeamwork && !pEntry->IsSuccess())
	{
		AwardNotifyTeamMem(pTask, pEntry);
		pEntry->SetAwardNotifyTeam(); // Nofity only once
	}

	// 清除玩家某些非法状态
	if (m_bClearSomeIllegalStates)
	{
		pTask->ClearSomeIllegalStates();
	}

	if (!pTask->IsDeliverLegal()) // 玩家处于死亡，交易等特殊状态
		return false;

	if (pEntry->IsGiveUp() && m_bClearAsGiveUp)
	{
		pList->ClearTask(pTask, pEntry, true);
		pList->UpdateTaskMask(*pTask->GetTaskMask());
		NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_GIVE_UP, 0);
		return true;
	}

	pEntry->SetFinished();

#ifndef _TASK_CLIENT
	//Added 2011-03-04.
	AWARD_DATA ad;
	CalcAwardData(pTask, &ad, pEntry, pEntry->m_ulTaskTime,	ulCurTime);	
	if( ad.m_bCheckGlobalCompareExpression )//需要判断全局key/value表达式
	{
		unsigned long ulRet = CheckGlobalExpression( pTask, ad.m_GlobalCompareExpression );
		if( !ulRet )//判断全局key/value表达式成功
		{
			if( ad.m_bCheckGlobalFinishCount )//需要判断全服计数
			{
				if( !pEntry->IsHasDeliverExtraAward() )//还没有执行发放额外奖励的实际代码，那么请求全局变量
				{
					TaskPreservedData tpd;
					tpd.reason			= TASK_GLOBAL_CHECK_FINISH_COUNT_FOR_EXTRA_AWARD;
					tpd.m_ulSrcTask		= m_ID;
					tpd.m_ulSubTaskId	= bNotifyTeamMem;//暂时用这个保存bNotifyTeamMem变量
					tpd.m_ulRcvNum		= nChoice;//暂时用这个保存nChoice变量
					TaskQueryGlobalData(m_ID, pTask->GetPlayerId(), &tpd, sizeof(tpd));	
					return false;
				}
			}
			else//不需要判断全服计数
			{
				pEntry->SetCanDeliverExtraAward();//启动判断额外奖励物品相关包裹检测和实际额外物品奖励发放标示
			}
		}
	}
	else//不需要判断全局key/value表达式
	{
		if( ad.m_bCheckGlobalFinishCount )//需要判断全服计数
		{
			if( !pEntry->IsHasDeliverExtraAward() )//还没有执行发放额外奖励的实际代码，那么请求全局变量
			{
				TaskPreservedData tpd;
				tpd.reason			= TASK_GLOBAL_CHECK_FINISH_COUNT_FOR_EXTRA_AWARD;
				tpd.m_ulSrcTask		= m_ID;
				tpd.m_ulSubTaskId	= bNotifyTeamMem;//暂时用这个保存bNotifyTeamMem变量
				tpd.m_ulRcvNum		= nChoice;//暂时用这个保存nChoice变量
				TaskQueryGlobalData(m_ID, pTask->GetPlayerId(), &tpd, sizeof(tpd));	
				return false;
			}
		}
	}
	//Added end.
	unsigned long ulRet;
	if ((ulRet = RecursiveCheckAward(
		pTask,
		pList,
		pEntry,
		ulCurTime,
		nChoice
		)) != 0)
	{
		if (m_enumFinishType == enumTFTNPC || !pEntry->IsErrReported())
		{
			NotifyClient(
				pTask,
				NULL,
				TASK_SVR_NOTIFY_ERROR_CODE,
				0,
				ulRet
				);

			pEntry->SetErrReported();
		}

		char log[1024];
		sprintf(log, "DeliverAward: ret = %d", ulRet);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, log);
		return false;
	}

	if (pTask->IsInFamily() && !pEntry->IsAwardFamily() && GetTopTask()->NeedFamilySkillData())
	{
		AWARD_DATA ad;
		CalcAwardData(pTask, &ad, pEntry, pEntry->m_ulTaskTime,	ulCurTime);

		if (ad.m_nFamilySkillProficiency || ad.m_nFamilySkillLevel)
		{
			TaskPairData tp;
			tp.key = ad.m_nFamilySkillIndex;

			if (ad.m_nFamilySkillProficiency)
			{
				tp.value1 = 0;
				tp.value2 = ad.m_nFamilySkillProficiency;
			}
			else
			{
				tp.value1 = ad.m_nFamilySkillLevel;
				tp.value2 = 0;
			}

			TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyUpdateSkillInfo, m_ID, enumTaskReasonUpdateSkill, 0, &tp, 1);
			pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
			return false;
		}
	}

#endif

	// 处理组队成功情况
	if (!pEntry->IsAwardNotifyTeam() && bNotifyTeamMem && pEntry->GetCapOrSelf()->m_bTeamwork && pEntry->IsSuccess())
	{
		AwardNotifyTeamMem(pTask, pEntry);
		pEntry->SetAwardNotifyTeam(); // Nofity only once
	}

	task_sub_tags sub_tags;
	memset(&sub_tags, 0, sizeof(sub_tags));
	sub_tags.state = pEntry->m_uState;
	bool bSucc = pEntry->IsSuccess();

	// 完成任务
	RecursiveAward(pTask, pList, pEntry, ulCurTime, nChoice, &sub_tags);

	// 通知客户端
	NotifyClient(
		pTask,
		NULL,
		TASK_SVR_NOTIFY_COMPLETE,
		ulCurTime,
		reinterpret_cast<unsigned long>(&sub_tags));

	// 检查Mask
	pList->UpdateTaskMask(*pTask->GetTaskMask());

	if (m_bSharedByFamily && _is_header(pTask->GetFactionRole()))
	{
		TaskPairData tp(m_ID, 0, 0);
		TaskFamilyAsyncData d1(pTask->GetFamilyID(), enumTaskFamilyRemoveSharedTask, m_ID, enumTaskReasonRemoveSharedTask, 0, &tp, 1);
		pTask->QueryFamilyData(d1.GetBuffer(), d1.GetSize());

		if (m_bNeedRecord)
		{
			TaskPairData tp(m_ID, bSucc, ulCurTime);
			TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyAddFinishedTask, m_ID, enumTaskReasonAddFinishedTask, 0, &tp, 1);
			pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
		}

		TaskFamilyAsyncData d2(pTask->GetFamilyID(), enumTaskFamilyNotifyFinish, m_ID, bSucc ? enumTaskReasonNotifyFinishSucc : enumTaskReasonNotifyFinishFail, 0, 0, 0);
		pTask->QueryFamilyData(d2.GetBuffer(), d2.GetSize());
	}

	return true;
}

inline void ATaskTempl::AwardNotifyTeamMem(TaskInterface* pTask, ActiveTaskEntry* pEntry) const
{
	const ATaskTempl* pCap = pEntry->GetCapOrSelf();
	bool bCheckPos = false;
	unsigned long ulWorldId;
	float pos[3];
	unsigned long ulParam;
	bool bIsCap = pTask->IsCaptain();

	if (pEntry->IsSuccess())
	{
		if (!pCap->m_bAllSucc && (!bIsCap || !m_bCapSucc))
			return;

		ulParam = TASK_PLY_NOTIFY_FORCE_SUCC;

		if (m_fSuccDist > 0.0f)
		{
			bCheckPos = true;
			ulWorldId = pTask->GetPos(pos);
		}
	}
	else if (pCap->m_bAllFail || pCap->m_bCapFail && bIsCap)
		ulParam = TASK_PLY_NOTIFY_FORCE_FAIL;
	else
		return;

	const int nMemNum = pTask->GetTeamMemberNum();
	unsigned long ulId = pTask->GetPlayerId();
	int i;

	for (i = 0; i < nMemNum; i++)
	{
		unsigned long ulMemId = pTask->GetTeamMemberId(i);

		if (ulMemId == ulId)
			continue;

		if (bCheckPos)
		{
			float mem_pos[3];

			if (pTask->GetTeamMemberPos(i, mem_pos) != ulWorldId)
				continue;

			float x = mem_pos[0] - pos[0];
			float y = mem_pos[1] - pos[1];
			float z = mem_pos[2] - pos[2];

			if (x * x + y * y + z * z > m_fSuccDist)
				continue;
		}

		TaskNotifyPlayer(
			pTask,
			ulMemId,
			pCap->m_ID,
			(unsigned char)ulParam);
	}
}

unsigned long ATaskTempl::GetFinishTimeLimit(TaskInterface* pTask, unsigned long ulCurTime) const
{
	unsigned long ulLimit = 0;

	switch (m_nFinishTimeType)
	{
	case enumTAFEachWeek:	{

		tm tmCur = *localtime((time_t*)&ulCurTime);
		int nCurDay = task_week_map[tmCur.tm_wday];
		int nDiffDay = m_FinishClearTime.wday - nCurDay;
		int nCurSeconds = tmCur.tm_hour * 3600 + tmCur.tm_min * 60 + tmCur.tm_sec;
		int nLimitSeconds = m_FinishClearTime.hour * 3600 + m_FinishClearTime.min * 60;
		ulLimit = ulCurTime - nCurSeconds + nDiffDay * 86400 + nLimitSeconds;

		if (ulLimit <= ulCurTime)
			ulLimit += 86400 * 7;

		break;	}

	case enumTAFEachDay: {

		tm tmCur = *localtime((time_t*)&ulCurTime);
		int nCurSeconds = tmCur.tm_hour * 3600 + tmCur.tm_min * 60 + tmCur.tm_sec;
		int nLimitSeconds = m_FinishClearTime.hour * 3600 + m_FinishClearTime.min * 60;
		ulLimit = ulCurTime - nCurSeconds + nLimitSeconds;

		if (ulLimit <= ulCurTime)
			ulLimit += 86400;

		break; }

	default:

		ulLimit = 0;
	}

	return ulLimit;
}

bool ATaskTempl::CheckFinishTimeState(TaskInterface* pTask, TaskFinishTimeEntry* pEntry, unsigned long ulCurTime) const
{
	//动态清空任务对应的全局变量改变时，清空任务的完成次数
	if(m_lDynFinishClearTime && pEntry)
	{
			if(pTask->GetGlobalValue(m_lDynFinishClearTime) != pEntry->m_ulUpdateTime)
			{
				pEntry->m_uFinishCount = 0;
				pEntry->m_ulUpdateTime = pTask->GetGlobalValue(m_lDynFinishClearTime);
				return true;		
			}
		return false;
	}

	if (pEntry->m_ulUpdateTime)
	{
		if (ulCurTime >= pEntry->m_ulUpdateTime)
		{
			pEntry->m_ulUpdateTime = GetFinishTimeLimit(pTask, ulCurTime);
			pEntry->m_uFinishCount = 0;
			return true;
		}
	}
	else
		pEntry->m_ulUpdateTime = GetFinishTimeLimit(pTask, ulCurTime);

	return false;
}

void ActiveTaskList::UpdateTaskMask(unsigned long& ulMask) const
{
	ulMask = 0;
	for (unsigned char i = 0; i < m_uTaskCount; i++)
	{
		const ATaskTempl* pTempl = m_TaskEntries[i].GetTempl();
		if (!pTempl) continue;
		ulMask |= pTempl->m_ulMask;
	}
}

#endif

void TaskInterface::InitActiveTaskList()
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	FinishedTaskList* pFnsh = static_cast<FinishedTaskList*>(GetFinishedTaskList());
	TaskFinishTimeList* pFnshTime = static_cast<TaskFinishTimeList*>(GetFinishedTimeList());
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	ATaskTemplMan* pMan = GetTaskTemplMan();
	bool bVersionIsOk = true;

#ifdef _TASK_CLIENT

	if (!CheckVersion())
		bVersionIsOk = false;

#else

	if (pLst->m_Version > TASK_ENTRY_DATA_CUR_VER)
		bVersionIsOk = false;
	else if (pLst->m_Version == 1)
		bVersionIsOk = false;
	else if (pLst->m_Version == 2)
		convert_finishtimelist_v2((char*)pFnshTime);

#endif

	if (!bVersionIsOk || !pLst->IsValid() || !pFnsh->IsValid() || !pFnshTime->IsValid())
	{
		pLst->RemoveAll();
		pFnsh->RemoveAll();
		pFnshTime->RemoveAll();
		TaskInterface::WriteLog(0, 0, 0, "InitLst, list is invalid");
	}

#ifdef _TASK_CLIENT

//	FILE* fp = fopen("logs\\Tasks.log", "wb");
//  禁掉任务log
	FILE* fp = NULL;
	if (fp)
	{
		unsigned short magic = 0xfeff;
		fwrite(&magic, sizeof(magic), 1, fp);

		for (unsigned long n = 0; n < pFnsh->m_ulTaskCount; n++)
		{
			ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(pFnsh->m_aTaskList[n].m_uTaskId & 0x7fff);

			fwprintf(
				fp,
				L"task = %d, name = %s\r\n",
				pFnsh->m_aTaskList[n].m_uTaskId,
				pTempl ? pTempl->GetName() : L"");
		}
	
		for (unsigned short m = 0; m < pFnshTime->m_uCount; m++)
		{
			ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(pFnshTime->m_aList[m].m_uTaskId);
			TaskFinishTimeEntry& entry = pFnshTime->m_aList[m];
			unsigned long his_time = entry.m_ulTimeMark;
			his_time -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);

			if ((long)(his_time) < 0)
				his_time = 0;

			tm t = *gmtime((time_t*)&his_time);
			wchar_t buf[256];
			swprintf(buf, L"%d-%02d-%02d-%02d-%02d-%02d", t.tm_year + 1900, t.tm_mon + 1, t.tm_mday, t.tm_hour, t.tm_min, t.tm_sec);

			fwprintf(
				fp,
				L"task = %d, deliver time = %s, name = %s, count = %d\r\n",
				pFnshTime->m_aList[m].m_uTaskId,
				buf,
				pTempl ? pTempl->GetName() : L"",
				entry.m_uFinishCount);
		}

		fclose(fp);
	}

	if (!GetTaskTemplMan()->IsDynTasksVerified())
	{
		// 获取动态任务的时间标签
		_notify_svr(this, TASK_CLT_NOTIFY_DYN_TIMEMARK, 0);
	}
	else
	{
		// 清除特殊奖励信息
		GetTaskTemplMan()->ClearSpecailAward();
		_notify_svr(this, TASK_CLT_NOTIFY_SPECIAL_AWARD, 0);

		// 获取仓库数据
		_notify_svr(this, TASK_CLT_NOTIFY_STORAGE, 0);
	}

#else
	unsigned long ulCurTime = GetCurTime();
	const ATaskTempl* pTempl;
	pLst->m_Version = TASK_ENTRY_DATA_CUR_VER;
	bool bTimeMarkUpdated = pLst->IsTimeMarkUpdate();
	pLst->m_uTopShowTaskCount = 0;
	pLst->m_uTopHideTaskCount = 0;
#endif

	unsigned char i = 0;

	for (; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& entry = pEntries[i];

		if (!entry.IsValid(i, pLst->m_uTaskCount))
		{
			pLst->RemoveAll();
			TaskInterface::WriteLog(0, 0, 0, "InitLst, active list is invalid");
			break;
		}
	}

	i = 0;

	while (i < pLst->m_uTaskCount)
	{
		ActiveTaskEntry& entry = pEntries[i];

		if (entry.m_ParentIndex == 0xff)
			entry.m_ulTemplAddr = reinterpret_cast<unsigned long>(pMan->GetTopTaskByID(entry.m_ID));
		else
		{
			const ATaskTempl* pParent = pLst->m_TaskEntries[entry.m_ParentIndex].GetTempl();

			if (pParent)
				entry.m_ulTemplAddr = reinterpret_cast<unsigned long>(pParent->GetConstSubById(entry.m_ID));
			else
				entry.m_ulTemplAddr = 0;
		}

#ifndef _TASK_CLIENT

		if (!entry.m_ulTemplAddr)
		{
			TaskInterface::WriteLog(0, entry.m_ID, 0, "InitLst, Cant Find Task");

			pLst->ClearTask(this, &entry, false);
			continue;
		}

		// 若无子任务，且没完成，则清除
		if (entry.m_ChildIndex == 0xff
		 && entry.GetTempl()->m_enumMethod == enumTMNone
		 && !entry.IsFinished())
		{
			TaskInterface::WriteLog(0, entry.m_ID, 0, "InitLst, Task is Impossible");
			pLst->ClearTask(this, &entry, false);
			continue;
		}

#endif

		if (entry.m_uCapTaskId)
		{
			entry.m_ulCapTemplAddr = reinterpret_cast<unsigned long>(GetTaskTemplMan()->GetTopTaskByID(entry.m_uCapTaskId));
			if (!entry.m_ulCapTemplAddr)
			{
				entry.m_uCapTaskId = 0;
				TaskInterface::WriteLog(0, entry.m_uCapTaskId, 0, "InitLst, Cant Find CapTask");
			}
		}
		else
			entry.m_ulCapTemplAddr = 0;

#ifndef _TASK_CLIENT

		if (!bTimeMarkUpdated)
		{
			pTempl = entry.GetTempl();

			if (!pTempl->m_bAbsTime)
				entry.m_ulTaskTime = ulCurTime - entry.m_ulTaskTime;
		}

#endif

#ifndef _TASK_CLIENT
		// 初始化活动任务链表中的隐藏、显示任务个数
		if (entry.m_ParentIndex == 0xff)
		{		
			entry.GetTempl()->m_bHidden ? pLst->m_uTopHideTaskCount++ : pLst->m_uTopShowTaskCount++;
		}
#endif
		
		i++;
	}

#ifndef _TASK_CLIENT
	pLst->SetTimeMarkUpdate();
	pLst->UpdateTaskMask(*GetTaskMask());
#endif

	pLst->UpdateUsedCount();
}

bool TaskInterface::CheckSpecialTasksForConsign()
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	if ( !pLst )
	{
		return true;
	}

	//遍历已接任务列表
	ActiveTaskEntry* aEntries = pLst->m_TaskEntries;
	for ( int i = 0; i < pLst->m_uTaskCount; ++i )
	{
		ActiveTaskEntry& CurEntry = aEntries[i];
		if ( !CurEntry.m_ulTemplAddr )
		{
			continue;
		}

		const ATaskTempl* pTempl = CurEntry.GetTempl();
		if ( pTempl && ((18983 == pTempl->GetID()) || (24369 == pTempl->GetID())) )	//暂时写死,只有两个特殊任务.
		{
			return false;
		}
	}

	//默认
	return true;

}

unsigned long TaskInterface::GetActLstDataSize()
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	return TASK_ACTIVE_LIST_HEADER_LEN + sizeof(pLst->m_Storages) + sizeof(ActiveTaskEntry) * pLst->m_uTaskCount;
}

unsigned long TaskInterface::GetFnshLstDataSize()
{
	FinishedTaskList* pLst = static_cast<FinishedTaskList*>(GetFinishedTaskList());
	return sizeof(unsigned long) + sizeof(FnshedTaskEntry) * pLst->m_ulTaskCount;
}

unsigned long TaskInterface::GetFnshTimeLstDataSize()
{
	TaskFinishTimeList* pLst = static_cast<TaskFinishTimeList*>(GetFinishedTimeList());
	return sizeof(short) + sizeof(TaskFinishTimeEntry) * pLst->m_uCount;
}

bool TaskInterface::CheckVersion()
{
	return static_cast<ActiveTaskList*>(GetActiveTaskList())->m_Version == TASK_ENTRY_DATA_CUR_VER;
}

bool TaskInterface::HasTask(unsigned long ulTaskId)
{
	ActiveTaskEntry* pEntry = static_cast<ActiveTaskList*>(GetActiveTaskList())->GetEntry(ulTaskId);
	return pEntry && pEntry->GetTempl();
}


unsigned short TaskInterface::GetFactionTaskFinishedCount()
{
	TaskFinishTimeList* pFinishLst = static_cast<TaskFinishTimeList*>(GetFinishedTimeList());
	if(!pFinishLst)
		return MAX_FACTION_TASK_COUNT;

	unsigned short count = 0;
	for (unsigned short i = 0; i < pFinishLst->m_uCount; i++)
	{
		TaskFinishTimeEntry& entry = pFinishLst->m_aList[i];

		ATaskTempl* pTempl = GetTaskTemplMan()->GetTaskTemplByID(entry.m_uTaskId);

		if(pTempl && pTempl->m_bFaction && pTempl->m_bRecFinishCount)
			count += entry.m_uFinishCount;			
	}

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());

	for(unsigned char i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];
		const ATaskTempl *pTempl = CurEntry.GetTempl();
		if(pTempl && pTempl->m_bFaction)
			count++;
	}

	return count;
}


bool TaskInterface::CanDoMining(unsigned long ulTaskId)
{

#ifndef _TASK_CLIENT

	if (!IsDeliverLegal())
		return false;

	ActiveTaskEntry* pEntry = static_cast<ActiveTaskList*>(GetActiveTaskList())->GetEntry(ulTaskId);

	if (!pEntry)
		return false;

	const ATaskTempl* pTempl = pEntry->GetTempl();

	if (!pTempl)
		return false;

	if (pTempl->m_enumMethod != enumTMCollectNumArticle
	 ||	pTempl->m_ulItemsWanted == 0)
		return true;

	const ITEM_WANTED& iw = pTempl->m_ItemsWanted[0];
	unsigned long ulNum = _get_item_count(this, iw.m_ulItemTemplId, iw.m_bCommonItem);

	if (iw.m_ulItemNum && ulNum >= iw.m_ulItemNum)
		return false;

	if (iw.m_bCommonItem && !CanDeliverCommonItem(1) || !iw.m_bCommonItem && !CanDeliverTaskItem(1))
	{
		pTempl->NotifyClient(this, NULL, TASK_SVR_NOTIFY_ERROR_CODE, 0, TASK_PREREQU_FAIL_GIVEN_ITEM);
		return false;
	}

#endif

	return true;
}

#ifdef _TASK_CLIENT

long TaskInterface::m_tmFinishDlgShown = 0;

int TaskInterface::GetFirstSubTaskPosition(unsigned long ulParentTaskId)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];
		if (ulParentTaskId != CurEntry.m_ID) continue;
		if (CurEntry.m_ChildIndex == 0xff) return -1;
		else return (int)CurEntry.m_ChildIndex;
	}

	return -1;
}

unsigned long TaskInterface::GetNextSub(int& nPosition)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	if (nPosition < 0 || nPosition >= (int)pLst->m_uTaskCount) return 0;

	ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[nPosition];
	if (CurEntry.m_NextSblIndex == 0xff) nPosition = -1;
	else nPosition = (int)CurEntry.m_NextSblIndex;

	return CurEntry.m_ID;
}

unsigned long TaskInterface::GetSubAt(int nPosition)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	if (nPosition < 0 || nPosition >= (int)pLst->m_uTaskCount) return 0;

	return pLst->m_TaskEntries[nPosition].m_ID;
}

unsigned long TaskInterface::GetTaskCount()
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	unsigned long ulCount = 0;

	for (unsigned char i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];

		const ATaskTempl *pTempl = CurEntry.GetTempl();
		if (pTempl && CurEntry.m_ParentIndex == 0xff)
		{
			if (!pTempl->m_bHidden || pTempl->CanShowPrompt())
			{
				ulCount++;
			}
		}
	}

	return ulCount;
}


unsigned long TaskInterface::GetTaskId(unsigned long ulIndex)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	unsigned char uTopCount = 0, uCount = 0;
	
	while (uCount < pLst->m_uTaskCount)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[uCount];

		const ATaskTempl *pTempl = CurEntry.GetTempl();		
		if (pTempl && CurEntry.m_ParentIndex == 0xff)
		{
			if (!pTempl->m_bHidden || pTempl->CanShowPrompt())
			{
				if (ulIndex == uTopCount)
					return CurEntry.m_ID;
				else
					uTopCount++;
			}
		}
		
		uCount++;
	}

	return 0;
}

unsigned long TaskInterface::CanDeliverTask(unsigned long ulTaskId)
{
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);
	if (!pTempl) return TASK_PREREQU_FAIL_NO_TASK;
	return pTempl->CheckPrerequisite(this, static_cast<ActiveTaskList*>(GetActiveTaskList()), GetCurTime(), true, true, false);
}

bool TaskInterface::CanShowTask(unsigned long ulTaskId)
{
	const ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);
	return pTempl && pTempl->CanShowTask(this);
}

bool TaskInterface::CanFinishTask(unsigned long ulTaskId)
{
	ActiveTaskEntry* pEntry = static_cast<ActiveTaskList*>(GetActiveTaskList())->GetEntry(ulTaskId);

	if (!pEntry)
		return false;

	const ATaskTempl* pTempl = pEntry->GetTempl();

	if (!pTempl)
		return false;

	if (pTempl->m_bMarriage && !IsCaptain())
		return false;

	if (pTempl->m_bSharedByFamily && !_is_header(GetFactionRole()))
		return false;

	return pTempl->CanFinishTask(this, pEntry, GetCurTime());
}

void TaskInterface::GiveUpTask(unsigned long ulTaskId)
{	
	ActiveTaskEntry* pEntry = static_cast<ActiveTaskList*>(GetActiveTaskList())->GetEntry(ulTaskId);
	if (!pEntry)
		return;

	const ATaskTempl *pTempl = pEntry->GetTempl();
	if (!pTempl)
		return;

	// 非跨服任务，并且在跨服服务器上，并且有失败奖励，则不可放弃
	if(!pTempl->IsCrossServerTask() && IsInCrossServer() && pTempl->HasFailAward())
		return;

	_notify_svr(this, TASK_CLT_NOTIFY_CHECK_GIVEUP, static_cast<unsigned short>(pEntry->GetTempl()->GetTopTask()->GetID()));
}

void TaskInterface::GiveUpAutoDeliverTask(unsigned long ulTaskId)
{	
	TaskFinishTimeList* pFinishLst = static_cast<TaskFinishTimeList*>(GetFinishedTimeList());
	if(!pFinishLst)
		return;

	pFinishLst->AddOrUpdateDeliverTime(ulTaskId, TaskInterface::GetCurTime());

	_notify_svr(this, TASK_CLT_NOTIFY_GIVEUP_AUTO, static_cast<unsigned short>(ulTaskId));
}

const unsigned short* TaskInterface::GetStorageTasks(unsigned int uStorageId)
{
	assert(uStorageId != 0 && uStorageId <= TASK_STORAGE_COUNT);
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	return pLst->m_Storages[uStorageId-1];
}

void TaskInterface::GetTaskStateInfo(unsigned long ulTaskId, Task_State_info* pInfo)
{
	memset(pInfo, 0, sizeof(*pInfo));

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	unsigned long i, j, ulCurTime = GetCurTime();
	const ATaskTempl* pTempl;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];
		if (CurEntry.m_ID != ulTaskId || !CurEntry.m_ulTemplAddr) continue;

		pTempl = CurEntry.GetTempl();

		if (pTempl->CanFinishTask(this, &CurEntry, ulCurTime))
			pInfo->m_ulErrCode = pTempl->RecursiveCheckAward(this, pLst, &CurEntry, ulCurTime, -1);

		pInfo->m_ulTimePassed = ulCurTime > CurEntry.m_ulTaskTime ?
			ulCurTime - CurEntry.m_ulTaskTime : 0;

		if (pTempl->m_ulTimeLimit) pInfo->m_ulTimeLimit = pTempl->m_ulTimeLimit;

		if (pTempl->m_enumMethod == enumTMCollectNumArticle)
		{
			for (j = 0; j < pTempl->m_ulItemsWanted; j++)
			{
				const ITEM_WANTED& iw = pTempl->m_ItemsWanted[j];

				pInfo->m_ItemsWanted[j].m_ulItemId		= iw.m_ulItemTemplId;
				pInfo->m_ItemsWanted[j].m_ulReplaceItemId = iw.m_ulReplaceItemTemplId;
				pInfo->m_ItemsWanted[j].m_ulItemsToGet  = (pTempl->m_bMPTask) ? CurEntry.m_wItemsToGet : iw.m_ulItemNum;
				pInfo->m_ItemsWanted[j].m_ulItemsGained = _get_item_count(this, iw.m_ulItemTemplId, iw.m_bCommonItem);
				if(pTempl->m_bAutoMoveForCollectNumItems)
					pInfo->m_ItemsWanted[j].m_ulMonsterId		= iw.m_ulItemTemplId;
			}
		}
		else if (pTempl->m_enumMethod == enumTMKillNumMonster)
		{
			unsigned long ulItemCount = 0;
			unsigned long ulMonsterCount = 0;

			for (j = 0; j < pTempl->m_ulMonsterWanted; j++)
			{
				const MONSTER_WANTED& mw = pTempl->m_MonsterWanted[j];

				if (mw.m_ulDropItemId)
				{
					pInfo->m_ItemsWanted[ulItemCount].m_ulMonsterId = mw.m_ulMonsterTemplId;
					pInfo->m_ItemsWanted[ulItemCount].m_ulItemId = mw.m_ulDropItemId;
					pInfo->m_ItemsWanted[ulItemCount].m_ulItemsToGet = mw.m_ulDropItemCount;
					pInfo->m_ItemsWanted[ulItemCount].m_ulItemsGained = _get_item_count(this, mw.m_ulDropItemId, mw.m_bDropCmnItem);
					ulItemCount++;
				}
				else
				{
					pInfo->m_MonsterWanted[ulMonsterCount].m_ulMonsterId = mw.m_ulMonsterTemplId;
					pInfo->m_MonsterWanted[ulMonsterCount].m_ulMonstersToKill = mw.m_ulMonsterNum;
					pInfo->m_MonsterWanted[ulMonsterCount].m_ulMonstersKilled = CurEntry.m_wMonsterNum[j];
					ulMonsterCount++;
				}
			}
		}
		else if(pTempl->m_enumMethod == enumTMCollectNumInterObj)
		{
			for(j = 0; j < pTempl->m_ulInterObjWanted; j++)
			{
				const INTEROBJ_WANTED& wi = pTempl->m_InterObjWanted[j];
				
				pInfo->m_InterObjWanted[j].m_ulInterObjId = wi.m_ulInterObjId;
				pInfo->m_InterObjWanted[j].m_ulInterObjToCollect = wi.m_ulInterObjNum;
				pInfo->m_InterObjWanted[j].m_ulInterObjCollected = CurEntry.m_wInterObjNum[j];
			}
		}
		else if(pTempl->m_enumMethod == enumTMFactionProperty)
		{
			pInfo->m_iFactionPropWanted[0] = pTempl->m_iFactionGrassWanted;
			pInfo->m_iFactionPropWanted[1] = pTempl->m_iFactionMineWanted;
			pInfo->m_iFactionPropWanted[2] = pTempl->m_iFactionMonsterCoreWanted;
			pInfo->m_iFactionPropWanted[3] = pTempl->m_iFactionMonsterFoodWanted;
			pInfo->m_iFactionPropWanted[4] = pTempl->m_iFactionMoneyWanted;
		}
		else if (pTempl->m_enumMethod == enumTMProtectNPC)
		{
			pInfo->m_ulNPCToProtect	= pTempl->m_ulNPCToProtect;
			pInfo->m_ulProtectTime	= pTempl->m_ulProtectTimeLen;
		}
		else if (pTempl->m_enumMethod == enumTMWaitTime)
			if (pTempl->m_bShowWaitTime)
				pInfo->m_ulWaitTime = pTempl->m_ulWaitTime;

		return;
	}
}

void TaskInterface::GetTaskAwardPreview(unsigned long ulTaskId, Task_Award_Preview* p)
{
	memset(p, 0, sizeof(*p));

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	unsigned long i, j, ulCurTime = GetCurTime();
	const ATaskTempl* pTempl;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];
		if (CurEntry.m_ID != ulTaskId || !CurEntry.m_ulTemplAddr) continue;

		pTempl = CurEntry.GetTempl();

		AWARD_DATA ad;
		pTempl->CalcAwardData(this, &ad, &CurEntry, CurEntry.m_ulTaskTime, ulCurTime);

		unsigned long ulMulti = pTempl->CalcAwardMulti(
			this,
			&CurEntry,
			CurEntry.m_ulTaskTime,
			ulCurTime
			);

		if (!ulMulti) return;

		p->m_ulGold			= ad.m_ulGoldNum * ulMulti;
		p->m_ulExp			= ad.m_ulExp * ulMulti;
		p->m_ulSP			= ad.m_ulSP * ulMulti ;
		p->m_ulReputation	= ad.m_ulReputation * ulMulti;
		p->m_ulFengshenExp	= ad.m_ulFengshenExp * ulMulti;//Added 2011-02-18.
		p->m_ulBonus		= ad.m_ulBonusNum * ulMulti;//Added 2011-02-18.
		p->m_ulBattleScore	= ad.m_ulBattleScore * ulMulti;//Added 2011-02-18.
		p->m_nFamilySkillIndex		= ad.m_nFamilySkillIndex;
		p->m_nFamilySkillProficiency= ad.m_nFamilySkillProficiency;
		p->m_nFamilySkillLev		= ad.m_nFamilySkillLevel;

		p->m_iFactionGoldNote    = ad.m_iFactionGoldNote * ulMulti;
		p->m_iFactionGrass       = ad.m_iFactionGrass * ulMulti;
		p->m_iFactionMine	     = ad.m_iFactionMine * ulMulti;
		p->m_iFactionMonsterCore = ad.m_iFactionMonsterCore * ulMulti;
		p->m_iFactionMonsterFood = ad.m_iFactionMonsterFood * ulMulti;
		p->m_iFactionMoney		 = ad.m_iFactionMoney * ulMulti;

		for (int i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
			p->m_aFriendships[i] = ad.m_aFriendships[i] * ulMulti;

		if (ad.m_ulCandItems == 1)
		{
			p->m_bHasItem = true;
			p->m_bItemKnown = true;

			const AWARD_ITEMS_CAND& ic = ad.m_CandItems[0];

// 			if (ic.m_bRandChoose)
// 			{
// 				p->m_bItemKnown = false;
// 			}
// 			else
// 			{
				for (j = 0; j < ic.m_ulAwardItems; j++)
				{
					const ITEM_WANTED& wi = ic.m_AwardItems[j];

					if (!wi.m_bCommonItem)
					{
						if(wi.m_ulItemTemplId != 13425 && wi.m_ulItemTemplId != 19074)	// 任务奖励里特殊物品若是太极金丹或太极通宝，则要显示。
							continue;
						else
						{
							p->m_ItemsId[p->m_ulItemTypes] = wi.m_ulItemTemplId;
							p->m_ItemsNum[p->m_ulItemTypes] = wi.m_ulItemNum;
							p->m_ulItemTypes++;
						}
					}
					else if (wi.m_fProb > 0.0f && wi.m_fProb < 1.0f)
					{
						if(!wi.m_bCommonItem && wi.m_ulItemTemplId != 13425 && wi.m_ulItemTemplId != 19074)	// 任务奖励里特殊物品若是太极金丹或太极通宝，则要显示。
							continue;
						else
						{
							p->m_ItemsId2[p->m_ulItemTypes2] = wi.m_ulItemTemplId;
							p->m_ItemsNum2[p->m_ulItemTypes2] = wi.m_ulItemNum;
							p->m_ulItemTypes2++;
						}
					}
					else	// (wi.m_fProb == 0.0f || wi.m_fRrob == 1.0f), should be fixed award
					{
						p->m_ItemsId[p->m_ulItemTypes] = wi.m_ulItemTemplId;
						p->m_ItemsNum[p->m_ulItemTypes] = wi.m_ulItemNum;
						p->m_ulItemTypes++;
					}
				}
//			}
		}
		else if (ad.m_ulCandItems > 1)
			p->m_bHasItem = true;
		
		float fRetVal = pTempl->CalcAwardByParaExp(this, &ad)*ulMulti;
		switch(ad.m_nParaExpSel)
		{
		case 1:
			{
				unsigned long ulExpExt = (unsigned long)(MIN(fRetVal, 0x7FFFFFFF));
				p->m_ulExp += ulExpExt;
			}
			break;
		default:
			break;
		}
		
		return;
	}
}


void TaskInterface::GetCanDeliverTaskAwardPreview(unsigned long ulTaskId, Task_Award_Preview* p)
{
	memset(p, 0, sizeof(*p));

	ATaskTempl* pTempl = GetTaskTemplMan()->GetTopTaskByID(ulTaskId);
	if(!pTempl)
		return;

	AWARD_DATA ad;
	ad = *pTempl->m_Award_S;

	p->m_ulGold			= ad.m_ulGoldNum;
	p->m_ulExp			= ad.m_ulExp;
	p->m_ulSP			= ad.m_ulSP ;
	p->m_ulReputation	= ad.m_ulReputation;
	p->m_ulFengshenExp	= ad.m_ulFengshenExp;//Added 2011-02-18.
	p->m_ulBonus		= ad.m_ulBonusNum;//Added 2011-02-18.
	p->m_ulBattleScore	= ad.m_ulBattleScore;//Added 2011-02-18.
	p->m_nFamilySkillIndex		= ad.m_nFamilySkillIndex;
	p->m_nFamilySkillProficiency= ad.m_nFamilySkillProficiency;
	p->m_nFamilySkillLev		= ad.m_nFamilySkillLevel;


	p->m_iFactionGoldNote    = ad.m_iFactionGoldNote;
	p->m_iFactionGrass       = ad.m_iFactionGrass;
	p->m_iFactionMine	     = ad.m_iFactionMine;
	p->m_iFactionMonsterCore = ad.m_iFactionMonsterCore;
	p->m_iFactionMonsterFood = ad.m_iFactionMonsterFood;
	p->m_iFactionMoney		 = ad.m_iFactionMoney;


	for (int i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
		p->m_aFriendships[i] = ad.m_aFriendships[i];

	unsigned long j;
	if (ad.m_ulCandItems == 1)
	{
		p->m_bHasItem = true;
		p->m_bItemKnown = true;

		const AWARD_ITEMS_CAND& ic = ad.m_CandItems[0];

// 		if (ic.m_bRandChoose)
// 			p->m_bItemKnown = false;
// 		else
// 		{
			for (j = 0; j < ic.m_ulAwardItems; j++)
			{
				const ITEM_WANTED& wi = ic.m_AwardItems[j];

				if (!wi.m_bCommonItem && !ic.m_bRandChoose)
				{
					if(wi.m_ulItemTemplId != 13425 && wi.m_ulItemTemplId != 19074)	// 任务奖励里特殊物品若是太极金丹或太极通宝，则要显示。
						continue;
					else
					{
						p->m_ItemsId[p->m_ulItemTypes] = wi.m_ulItemTemplId;
						p->m_ItemsNum[p->m_ulItemTypes] = wi.m_ulItemNum;
						p->m_ulItemTypes++;
					}
				}
				else if ((wi.m_fProb != 1.0f && wi.m_fProb != 0.0f) || ic.m_bRandChoose)
				{
// 						p->m_bItemKnown = false;
// 						break;
					p->m_ItemsId2[p->m_ulItemTypes2] = wi.m_ulItemTemplId;
					p->m_ItemsNum2[p->m_ulItemTypes2] = wi.m_ulItemNum;
					p->m_ulItemTypes2++;
				}
				else
				{
					p->m_ItemsId[p->m_ulItemTypes] = wi.m_ulItemTemplId;
					p->m_ItemsNum[p->m_ulItemTypes] = wi.m_ulItemNum;
					p->m_ulItemTypes++;
				}
			}
//		}
	}
	else if (ad.m_ulCandItems > 1)
		p->m_bHasItem = true;
	
// 	float fRetVal = pTempl->CalcAwardByParaExp(this, &ad);
// 	switch(ad.m_nParaExpSel)
// 	{
// 	case 1:
// 		{
// 			unsigned long ulExpExt = (unsigned long)(MIN(fRetVal, 0x7FFFFFFF));
// 			p->m_ulExp += ulExpExt;
// 		}
// 		break;
// 	default:
// 		break;
// 	}
	
	return;
}

bool TaskInterface::GetAwardCandidates(unsigned long ulTaskId, AWARD_DATA* pAward)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTaskId);
	if (!pEntry || !pEntry->m_ulTemplAddr) return false;

	unsigned long ulCurTime = GetCurTime();
	pEntry->GetTempl()->CalcAwardData(
		this,
		pAward,
		pEntry,
		pEntry->m_ulTaskTime,
		ulCurTime);

	return true;
}

void TaskInterface::OnUIDialogEnd(unsigned long ulTask)
{
	TaskInterface::SetFinishDlgShowTime(0);
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTask);
	if (!pEntry || !pEntry->m_ulTemplAddr) return;
	const ATaskTempl* pTempl = reinterpret_cast<const ATaskTempl*>(pEntry->m_ulTemplAddr);

	switch (pTempl->m_enumMethod)
	{
	case enumTMReachSite:
		pTempl->IncValidCount();
		_notify_svr(this, TASK_CLT_NOTIFY_REACH_SITE, static_cast<unsigned short>(ulTask));
		break;
	case enumTMLeaveSite:
		pTempl->IncValidCount();
		_notify_svr(this, TASK_CLT_NOTIFY_LEAVE_SITE, static_cast<unsigned short>(ulTask));
		break;
	case enumTMWaitTime:
	case enumTMKillNumMonster:
	case enumTMCollectNumArticle:
	case enumTMTitle:
		pTempl->IncValidCount();
		_notify_svr(this, TASK_CLT_NOTIFY_CHECK_FINISH, static_cast<unsigned short>(ulTask));
		break;
	}
}

void TaskInterface::DirectFinishTask(unsigned long ulTask)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(GetActiveTaskList());
	ActiveTaskEntry* pEntry = pLst->GetEntry(ulTask);
	if (!pEntry || !pEntry->m_ulTemplAddr) return;
	const ATaskTempl* pTempl = reinterpret_cast<const ATaskTempl*>(pEntry->m_ulTemplAddr);

	if (pTempl->CanDirectFinish())
	{
		pTempl->IncValidCount();
		_notify_svr(this, TASK_CLT_NOTIFY_CHECK_FINISH, static_cast<unsigned short>(ulTask));
	}
}

#else

void TaskInterface::BeforeSaveData()
{
	ActiveTaskList* pList = static_cast<ActiveTaskList*>(GetActiveTaskList());

	if (!pList->IsTimeMarkUpdate())
		return;

	pList->ClearTimeMarkUpdate();
	ActiveTaskEntry* aEntries = pList->m_TaskEntries;
	unsigned long ulCurTime = GetCurTime();
	unsigned char i;
	const ATaskTempl* pTempl;

	for (i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];
		pTempl = CurEntry.GetTempl();

		if (pTempl && !pTempl->m_bAbsTime)
			CurEntry.m_ulTaskTime = ulCurTime - CurEntry.m_ulTaskTime;
	}
}

bool ClearFailAfterLogoutTask(TaskInterface* pTask, ActiveTaskList* pList)
{
	ActiveTaskEntry* pEntries = pList->m_TaskEntries;
	for (unsigned short i = 0; i < pList->m_uTaskCount; i++)
	{
		ActiveTaskEntry& entry = pEntries[i];
		
		if (!entry.IsSuccess())
			continue;

		const ATaskTempl *pTempl = entry.GetTempl();
		if (pTempl && pTempl->m_bFailAfterLogout)
		{
			unsigned long ulCurTime = pTask->GetCurTime();
			unsigned long ulTaskTime = entry.m_ulTaskTime;

			if(pTempl->m_ulLogoutFailTime == 0 ||
				((ulTaskTime < pTask->GetLastLogoutTime()) && (ulCurTime > pTask->GetLastLogoutTime() + pTempl->m_ulLogoutFailTime)) )
			{
				if(pTempl->m_bFailAfterLogout)
				TaskInterface::WriteLog(pTask->GetPlayerId(), entry.m_ID, 0, "PlayerEnterWorld, Task fail after logout");
				entry.ClearSuccess();
				pTempl->OnSetFinished(pTask, pList, &entry, false);

				return true;
			}
		}
	}
	
	return false;
}

void TaskInterface::PlayerEnterWorld()
{
	ActiveTaskList* pList = (ActiveTaskList*)GetActiveTaskList();

	if(!IsInCrossServer()) // 跨服服务器无家族信息，不需要删除家族任务
	{
		if (IsInFamily())
		{
			TaskFamilyAsyncData d(GetFamilyID(), enumTaskFamilyGetSharedTasks, 0, enumTaskReasonGetSharedTasks, 0, 0, 0);
			QueryFamilyData(d.GetBuffer(), d.GetSize());
		}
		else
		{		
			pList->ClearFamilyTask(this);
		}
	}

	// 下线失败
	while (ClearFailAfterLogoutTask(this, pList))
	{
	}

	OnTaskCheckState(this);
}

void TaskInterface::PlayerLeaveWorld()
{
	OnTeamMemberLeave(this);	
}

bool TaskInterface::OnCheckTeamRelationship(int nReason, TaskTeamInterface* pTeam)
{
	if (!IsDeliverLegal())
		return false;

	if (nReason == TASK_TEAM_RELATION_MARRIAGE)
	{
		if (IsMarried())
			return false;

		ActiveTaskList* pList = static_cast<ActiveTaskList*>(GetActiveTaskList());
		ActiveTaskEntry* aEntries = pList->m_TaskEntries;
		unsigned char i;
		const ATaskTempl* pTempl;

		for (i = 0; i < pList->m_uTaskCount; i++)
		{
			ActiveTaskEntry& CurEntry = aEntries[i];
			pTempl = CurEntry.GetTempl();

			if (pTempl && pTempl->m_bMarriage)
			{
				if (!CurEntry.IsSuccess())
					return false;

				unsigned long ulCurTime = GetCurTime();
				return pTempl->CanFinishTask(this, &CurEntry, ulCurTime) && pTempl->RecursiveCheckAward(this, pList, &CurEntry, ulCurTime, -1) == 0;
			}
		}
	}

	return false;
}

void TaskInterface::OnCheckTeamRelationshipComplete(int nReason, TaskTeamInterface* pTeam)
{
	if (nReason == TASK_TEAM_RELATION_MARRIAGE)
	{
		pTeam->SetMarriage(GetPlayerId());

		ActiveTaskList* pList = static_cast<ActiveTaskList*>(GetActiveTaskList());
		ActiveTaskEntry* aEntries = pList->m_TaskEntries;
		unsigned char i;
		const ATaskTempl* pTempl;

		for (i = 0; i < pList->m_uTaskCount; i++)
		{
			ActiveTaskEntry& CurEntry = aEntries[i];
			pTempl = CurEntry.GetTempl();

			if (pTempl && pTempl->m_bMarriage)
			{
				pTempl->DeliverAward(this, pList, &CurEntry, -1);
				return;
			}
		}
	}
}
	
// 获取任务完成次数
int TaskInterface::GetTaskFinishCount(unsigned long ulTask)
{
	TaskFinishTimeList* pFinish = (TaskFinishTimeList*)(GetFinishedTimeList());
	TaskFinishTimeEntry* p = (TaskFinishTimeEntry*)pFinish->Search(ulTask);
	if(p)
		return p->m_uFinishCount;

	return 0;
}

void TaskInterface::SetFamilyMonRecord(int nRecordIndex, int nValue, TaskFamilyAsyncData* pAsyncData)
{
	if(!pAsyncData)
		return;

	TaskFamilyAsyncData::_header* header = pAsyncData->GetHeader();

	for (int i = 0; i < header->nPairCount; i++)
	{
		TaskPairData& tp = pAsyncData->GetPair(i);

		if (tp.key+1 == nRecordIndex)
		{
			tp.value1 = nValue;
			return;
		}
	}
}

#endif

void TaskGlobalData::CheckRcvUpdateTime(const ATaskTempl* pTempl, unsigned long ulCurTime)
{
	if (m_ulRcvUpdateTime == 0)
		return;
	
	tm tmCur = *localtime((time_t*)&ulCurTime);
	tm tmRcv = *localtime((time_t*)&m_ulRcvUpdateTime);
	
	switch(pTempl->m_nClearReceiverType)
	{
	case enumClearEachDay:
		{
			if (tmCur.tm_year != tmRcv.tm_year || tmCur.tm_yday != tmRcv.tm_yday)
			{
				m_ulReceiverNum = 0;
			}				
		}
		break;
	case enumClearEachWeek:
		{
			if (!_is_same_week(&tmCur, &tmRcv, ulCurTime, m_ulRcvUpdateTime))
			{
				m_ulReceiverNum = 0;
			}
		}
		break;
	case enumClearEachMonth:
		{
			if (tmCur.tm_year != tmRcv.tm_year || tmCur.tm_mon  != tmRcv.tm_mon)
			{
				m_ulReceiverNum = 0;
			}
		}
		break;
	case enumClearEverySeconds:
		{
			if (ulCurTime - m_ulRcvUpdateTime >= (unsigned long)pTempl->m_lClearReceiverTimeInterval)
			{
				m_ulReceiverNum = 0;					
			}
		}
		break;
	default:
		break;
	}
}

const unsigned short _mask = 1 << 15;
#define MASK_TASK_ID(n) static_cast<unsigned long>((n) & (~_mask))

int FinishedTaskList::GetTaskPos(unsigned long ulID) const
{
	if (m_ulTaskCount == 0) return -1;
	else if (m_ulTaskCount == 1)
	{
		if (ulID == MASK_TASK_ID(m_aTaskList[0].m_uTaskId))
			return 0;
		else
			return -1;
	}

	unsigned long ulStart = 0;
	unsigned long ulEnd = m_ulTaskCount - 1;

	while (ulStart + 1 < ulEnd)
	{
		unsigned long ulMid = (ulStart + ulEnd) >> 1;
		unsigned long ulMasked = MASK_TASK_ID(m_aTaskList[ulMid].m_uTaskId);

		if (ulID == ulMasked)
			return ulMid;
		else if (ulID < ulMasked)
			ulEnd = ulMid;
		else
			ulStart = ulMid;
	}

	if (MASK_TASK_ID(m_aTaskList[ulStart].m_uTaskId) == ulID)
		return ulStart;

	if (MASK_TASK_ID(m_aTaskList[ulEnd].m_uTaskId) == ulID)
		return ulEnd;

	return -1;
}

unsigned long FinishedTaskList::GetRealTaskID(unsigned long ulIDInEntry)
{
	unsigned long ulRealTaskID = MASK_TASK_ID(ulIDInEntry);
	return ulRealTaskID;
}

int FinishedTaskList::SearchTask(unsigned long ulID) const
{
	/*
	 *	无此任务返回	-1
	 *	此任务成功返回	0
	 *	此任务失败返回	1
	 */

	int nPos = GetTaskPos(ulID);
	if (nPos < 0) return -1;

	return (m_aTaskList[nPos].m_uTaskId >> 15);
}

bool FinishedTaskList::RemoveTask(unsigned long ulID)
{
	unsigned long ulPos = GetTaskPos(ulID);
	if (static_cast<int>(ulPos) < 0) return false;

	m_ulTaskCount--;

	if (m_ulTaskCount <= ulPos)
		return true;

	memmove(
		&m_aTaskList[ulPos],
		&m_aTaskList[ulPos+1],
		(m_ulTaskCount-ulPos) * sizeof(FnshedTaskEntry));

	return true;
}

#define FNSH_LST_LOG
#define MAKE_TASK_ID(b, id) static_cast<unsigned short>((b) ? (id) : (id) | _mask)

void FinishedTaskList::AddOneTask(unsigned long ulID, bool bSuccess)
{
	if (m_ulTaskCount >= TASK_FINISHED_LIST_MAX_LEN)
		return;

	// 二分法插入
	if (m_ulTaskCount == 0)
	{
		m_aTaskList[0].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
		m_ulTaskCount++;
	}
	else if (m_ulTaskCount == 1)
	{
		unsigned long ulMasked = MASK_TASK_ID(m_aTaskList[0].m_uTaskId);

		if (ulID == ulMasked)
		{
			m_aTaskList[0].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
			FNSH_LST_LOG
			return;
		}
		else if (ulID > ulMasked)
			m_aTaskList[1].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
		else
		{
			m_aTaskList[1] = m_aTaskList[0];
			m_aTaskList[0].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
		}

		m_ulTaskCount++;
	}
	else
	{
		unsigned long ulStart = 0;
		unsigned long ulEnd = m_ulTaskCount - 1;

		while (ulStart + 1 < ulEnd)
		{
			unsigned long ulMid = (ulStart + ulEnd) >> 1;
			unsigned long ulMasked = MASK_TASK_ID(m_aTaskList[ulMid].m_uTaskId);

			if (ulID == ulMasked)
			{
				m_aTaskList[ulMid].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
				FNSH_LST_LOG
				return;
			}
			else if (ulID < ulMasked)
				ulEnd = ulMid;
			else
				ulStart = ulMid;
		}

		unsigned long ulMaskedStart = MASK_TASK_ID(m_aTaskList[ulStart].m_uTaskId);
		unsigned long ulMaskedEnd = MASK_TASK_ID(m_aTaskList[ulEnd].m_uTaskId);

		unsigned long ulInsert;
		if (ulID == ulMaskedStart)
		{
			m_aTaskList[ulStart].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
			FNSH_LST_LOG
			return;
		}
		else if (ulID == ulMaskedEnd)
		{
			m_aTaskList[ulEnd].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
			FNSH_LST_LOG
			return;
		}
		else if (ulID < ulMaskedStart) ulInsert = ulStart;
		else if (ulID > ulMaskedEnd) ulInsert = ulEnd + 1;
		else ulInsert = ulEnd;

		if (m_ulTaskCount > ulInsert)
		{
			memmove(
				&m_aTaskList[ulInsert+1],
				&m_aTaskList[ulInsert],
				(m_ulTaskCount - ulInsert) * sizeof(FnshedTaskEntry));
		}

		m_aTaskList[ulInsert].m_uTaskId = MAKE_TASK_ID(bSuccess, ulID);
		m_ulTaskCount++;
	}

	FNSH_LST_LOG
}

TaskFinishTimeEntry* TaskFinishTimeList::Search(unsigned long ulID)
{
	for (unsigned short i = 0; i < m_uCount; i++)
		if (m_aList[i].m_uTaskId == (unsigned short)ulID)
			return &m_aList[i];

	return 0;
}

void TaskFinishTimeList::AddDeliverTime(unsigned long ulID, unsigned long ulTime)
{
	if (m_uCount >= TASK_FINISH_TIME_MAX_LEN)
		return;

	TaskFinishTimeEntry& entry = m_aList[m_uCount];
	entry.m_uTaskId		= (unsigned short)ulID;
	entry.m_ulTimeMark	= ulTime;
	entry.m_uFinishCount= 0;
	entry.m_ulUpdateTime= 0;
	m_uCount++;
}

void TaskFinishTimeList::AddOrUpdateDeliverTime(unsigned long ulID, unsigned long ulTime)
{
	TaskFinishTimeEntry* pEntry = Search(ulID);

	if (pEntry)
		pEntry->m_ulTimeMark = ulTime;
	else
		AddDeliverTime(ulID, ulTime);
}

unsigned short TaskFinishTimeList::AddFinishTime(unsigned long ulID, unsigned long ulTime)
{
	if (m_uCount >= TASK_FINISH_TIME_MAX_LEN)
		return 0;

	TaskFinishTimeEntry& entry = m_aList[m_uCount];
	entry.m_uTaskId		= (unsigned short)ulID;
	entry.m_ulTimeMark	= 0;
	entry.m_uFinishCount= 1;
	entry.m_ulUpdateTime= ulTime;
	m_uCount++;
	return 1;
}

void TaskFinishTimeList::AddOrUpdateFinishCount(unsigned long ulID)
{
	TaskFinishTimeEntry* pEntry = Search(ulID);

	if (pEntry)
		pEntry->IncFinishCount();
	else
		AddFinishCount(ulID, 1);
}

//新增：增加或更新完成任务时间表函数，暂时仅用于调试接口，后续使用需要先看代码
void TaskFinishTimeList::AddOrUpdateFinishCountExt( unsigned long ulID, unsigned long count )
{
	//根据任务ID查找任务完成时间列表
	TaskFinishTimeEntry* pEntry = Search(ulID);
	
	if ( pEntry )//已经存在该任务了，则仅仅改变任务完成数
	{
		pEntry->m_uFinishCount = static_cast<unsigned short>(count);
	}
	else//目前还不存在该任务，则直接将新的项插入到列表中
	{
		AddFinishCount( ulID, static_cast<unsigned short>(count) );
	}
}

void TaskFinishTimeList::AddFinishCount(unsigned long ulID, unsigned short count)
{
	if (m_uCount >= TASK_FINISH_TIME_MAX_LEN)
		return;

	TaskFinishTimeEntry& entry = m_aList[m_uCount];
	entry.m_uTaskId		= (unsigned short)ulID;
	entry.m_ulTimeMark	= 0;
	entry.m_uFinishCount= count;
	entry.m_ulUpdateTime= 0;
	m_uCount++;
}

bool TaskFinishTimeList::RemoveTask(unsigned long ulID) 
{
	unsigned long ulPos = 0;
	while (ulPos < m_uCount)
	{
		if (m_aList[ulPos].m_uTaskId == ulID)
			break;

		ulPos++;
	}

	if (ulPos >= m_uCount)
		return false;

	m_uCount--;
	memmove(&m_aList[ulPos], &m_aList[ulPos+1], (m_uCount-ulPos) * sizeof(TaskFinishTimeEntry));

	return true;
}

void ActiveTaskList::RealignTask(ActiveTaskEntry* pEntry, unsigned char uReserve)
{
	unsigned char uCurIndex = static_cast<unsigned char>(pEntry - m_TaskEntries);
	unsigned long ulCount = m_uTaskCount - uCurIndex; // 剩余的任务数

	if (ulCount == 0) return; // 最后一个任务

	unsigned char uEmptyCount = 0;
	for (unsigned char uEmpty = uCurIndex; uEmpty < TASK_ACTIVE_LIST_MAX_LEN; uEmpty++)
	{
		if (!m_TaskEntries[uEmpty].m_ID) uEmptyCount++;
		else break;
	}

	if (uReserve == uEmptyCount) return;

	ActiveTaskEntry* pSrc = pEntry + uEmptyCount;
	ActiveTaskEntry* pInsert = pEntry + uReserve;

	// move it
	memmove(pInsert, pSrc, sizeof(ActiveTaskEntry) * ulCount);

	// clear reserve part
	ActiveTaskEntry *pClearStart, *pClearEnd;

	if (pInsert > pSrc)
	{
		pClearStart = pSrc;
		pClearEnd = pInsert;
	}
	else
	{
		pClearStart = pInsert + ulCount;
		pClearEnd = pSrc + ulCount;
	}

	while (pClearStart < pClearEnd)
	{
		pClearStart->m_ulTemplAddr = 0;
		pClearStart->m_ID = 0;
		pClearStart++;
	}

	// calc gap
	unsigned char uGap = static_cast<unsigned char>(pInsert - pSrc);
	unsigned long i = 0;

	for (; i < static_cast<unsigned long>(uCurIndex); i++)
	{
		// Parent, Prev小于uCurIndex
		ActiveTaskEntry& CurEntry = m_TaskEntries[i];

		if (CurEntry.m_ChildIndex != 0xff && CurEntry.m_ChildIndex >= uCurIndex)
			CurEntry.m_ChildIndex += uGap;
		if (CurEntry.m_NextSblIndex != 0xff && CurEntry.m_NextSblIndex >= uCurIndex)
			CurEntry.m_NextSblIndex += uGap;
	}

	for (i = 0; i < ulCount; i++)
	{
		ActiveTaskEntry& CurEntry = *(pInsert + i);

		if (CurEntry.m_ParentIndex != 0xff && CurEntry.m_ParentIndex >= uCurIndex)
			CurEntry.m_ParentIndex += uGap;
		if (CurEntry.m_PrevSblIndex != 0xff && CurEntry.m_PrevSblIndex >= uCurIndex)
			CurEntry.m_PrevSblIndex += uGap;
		if (CurEntry.m_ChildIndex != 0xff)
			CurEntry.m_ChildIndex += uGap;
		if (CurEntry.m_NextSblIndex != 0xff)
			CurEntry.m_NextSblIndex += uGap;
	}
}

void ActiveTaskList::RecursiveClearTask(
	TaskInterface* pTask,
	ActiveTaskEntry* pEntry,
	bool bRemoveItem,
	bool bRemoveAcquired,
	bool bClearTask)
{
	while (pEntry->m_ChildIndex != 0xff)
	{
		RecursiveClearTask(
			pTask,
			&m_TaskEntries[pEntry->m_ChildIndex],
			bRemoveItem,
			bRemoveAcquired,
			bClearTask);
	}

	const ATaskTempl* pTempl = pEntry->GetTempl();

	// 去掉获得的物品
#ifndef _TASK_CLIENT
	if (bRemoveItem && pTempl)
	{
		if (bRemoveAcquired || pTempl->m_bClearAcquired) pTempl->RemoveAcquiredItem(pTask, bClearTask, false);
		pTempl->TakeAwayGivenItems(pTask);
	}
#endif

	unsigned short uTaskId = pEntry->m_ID;

	pEntry->m_ulTemplAddr = 0;
	pEntry->m_ID = 0;

	if (m_uTaskCount)
		m_uTaskCount--;
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), uTaskId, 0, "ClearTask, TaskCount == 0");

	if (pEntry->m_ParentIndex != 0xff)
	{
		if (pEntry->m_PrevSblIndex != 0xff)
			m_TaskEntries[pEntry->m_PrevSblIndex].m_NextSblIndex = pEntry->m_NextSblIndex;
		else
			m_TaskEntries[pEntry->m_ParentIndex].m_ChildIndex = pEntry->m_NextSblIndex;
		if (pEntry->m_NextSblIndex != 0xff) m_TaskEntries[pEntry->m_NextSblIndex].m_PrevSblIndex = pEntry->m_PrevSblIndex;
	}
	else
	{
		if (pTempl)
		{
			if (!pTempl->m_bHidden && m_uTopShowTaskCount)
				m_uTopShowTaskCount--;
			else if (pTempl->m_bHidden && m_uTopHideTaskCount)
				m_uTopHideTaskCount--;
			else
				TaskInterface::WriteLog(pTask->GetPlayerId(), uTaskId, 0, "ClearTask, TopCount == 0");

			if (m_uUsedCount >= pTempl->m_uDepth)
				m_uUsedCount -= pTempl->m_uDepth;
			else
			{
				TaskInterface::WriteLog(pTask->GetPlayerId(), uTaskId, 0, "ClearTask, No Enough Used Count");
				m_uUsedCount = 0;
			}
		}
	}
}

void ActiveTaskList::ClearFamilyTask(TaskInterface* pTask)
{
	unsigned char i = 0;

	while (i < m_uTaskCount)
	{
		ActiveTaskEntry& CurEntry = m_TaskEntries[i];
		const ATaskTempl* pTempl = CurEntry.GetTempl();

		if (pTempl && (pTempl->m_bInFamily || pTempl->m_bSharedByFamily))
		{
#ifndef _TASK_CLIENT

			// 处理组队失败情况
			if (pTask->IsInTeam() && !CurEntry.IsAwardNotifyTeam() && CurEntry.GetCapOrSelf()->m_bTeamwork && !CurEntry.IsSuccess())
			{
				pTempl->AwardNotifyTeamMem(pTask, &CurEntry);
				CurEntry.SetAwardNotifyTeam(); // Nofity only once
			}

#endif
			ClearTask(pTask, &CurEntry, false);

#ifndef _TASK_CLIENT

			pTempl->NotifyClient(pTask, NULL, TASK_SVR_NOTIFY_GIVE_UP, 0);

#endif
			continue;
		}

		i++;
	}

#ifndef _TASK_CLIENT
	UpdateTaskMask(*pTask->GetTaskMask());
#endif
}


