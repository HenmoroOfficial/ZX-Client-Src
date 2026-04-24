#ifndef _TASKTEMPL_INL_
#define _TASKTEMPL_INL_

// Process Part

#define MONSTER_PLAYER_LEVEL_MAX_DIFF 8

#ifdef _ELEMENTCLIENT
extern void TaskShowErrMessage(int nIndex);
#endif

inline bool _is_header(int frole)
{
	return frole >= 2 && frole <= 5;
}

inline bool ATaskTempl::IsAutoDeliver() const
{
	return m_bDeathTrig || m_bAutoDeliver;
}

inline unsigned long ATaskTempl::CheckBudget(ActiveTaskList* pList) const
{
	// 任务达到上限
	if (!m_bHidden && pList->m_uTopShowTaskCount >= TASK_MAX_SIMULTANEOUS_COUT
	  || m_bHidden && pList->m_uTopHideTaskCount >= TASK_HIDDEN_COUNT)	
		return TASK_PREREQU_FAIL_FULL;

	// Check Task List Empty Space
	if (pList->m_uUsedCount + m_uDepth > TASK_ACTIVE_LIST_MAX_LEN) return TASK_PREREQU_FAIL_NO_SPACE;

	// 是否已有相同任务
	if (pList->GetEntry(m_ID)) return TASK_PREREQU_FAIL_SAME_TASK;

	return 0;
}

inline unsigned long ATaskTempl::CheckFamilySkill(int nLev, int nValue) const
{
	if (m_nFamilySkillLevelMin && nLev < m_nFamilySkillLevelMin
	 || m_nFamilySkillLevelMax && nLev > m_nFamilySkillLevelMax)
		return TASK_PREREQU_FAIL_F_SKILL_LEV;

	if (m_nFamilySkillProficiencyMin && nValue < m_nFamilySkillProficiencyMin
	 || m_nFamilySkillProficiencyMax && nValue > m_nFamilySkillProficiencyMax)
		return TASK_PREREQU_FAIL_F_SKILL_PROF;

	return 0;
}

inline unsigned long ATaskTempl::CheckFamilyMonsterRecord(int nRecord) const
{
	if (m_nFamilyMonRecordMin && m_nFamilyMonRecordMin > nRecord
	 || m_nFamilyMonRecordMax && m_nFamilyMonRecordMax < nRecord)
		return TASK_PREREQU_FAIL_RECORD;
	
	return 0;
}

inline unsigned long ATaskTempl::CheckFamilyValue(int nValue) const
{
	if (m_nFamilyValueMin && m_nFamilyValueMin > nValue
	 || m_nFamilyValueMax && m_nFamilyValueMax < nValue)
		return TASK_PREREQU_FAIL_RECORD;

	return 0;
}

inline unsigned long ATaskTempl::GetMemTaskByInfo(const task_team_member_info* pInfo, int nFamilyId) const
{
	if (!m_ulTeamMemsWanted)
		return m_ID; // 无成员要求则返回本身

	unsigned long i;

	for (i = 0; i < m_ulTeamMemsWanted; i++)
	{
		const TEAM_MEM_WANTED& tmw = m_TeamMemsWanted[i];

		if (!tmw.IsMeetBaseInfo(pInfo, nFamilyId))
			continue;

		return tmw.m_ulTask == 0 ? m_ID : tmw.m_ulTask; // 成员任务为0则返回队长任务
	}

	return 0;
}

inline unsigned long _get_item_count(TaskInterface* pTask, unsigned long ulItemId, bool bCommon)
{
	return bCommon ? pTask->GetCommonItemCount(ulItemId) : pTask->GetTaskItemCount(ulItemId);
}

inline bool ATaskTempl::HasAllItemsWanted(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const
{
	if (m_ulGoldWanted && pTask->GetGoldNum() < m_ulGoldWanted 
	|| m_iFactionGoldNoteWanted && pTask->GetFactionGoldNote() < m_iFactionGoldNoteWanted)
		return false;

	unsigned long i = 0;

	for (; i < m_ulItemsWanted; i++)
	{
		const ITEM_WANTED& iw = m_ItemsWanted[i];
		unsigned long ulNum = _get_item_count(
			pTask,
			iw.m_ulItemTemplId,
			iw.m_bCommonItem);

		unsigned long ulReplaceNum = _get_item_count(
			pTask, 
			iw.m_ulReplaceItemTemplId,
			iw.m_bCommonItem);

		unsigned long ulTotal = ulNum + ulReplaceNum;
		unsigned long ulItemsToGet = m_bMPTask ? pEntry->m_wItemsToGet : iw.m_ulItemNum;
		if (!ulTotal || ulTotal < ulItemsToGet) return false;
	}

	return true;
}

inline bool ATaskTempl::HasFinishAchievement(TaskInterface* pTask) const
{
	if (pTask->GetAchievePoint() >= m_ulFinishAchievement)
		return true;

	return false;
}

inline bool ATaskTempl::CheckTotalTreasure(TaskInterface* pTask) const
{
	int nValue = pTask->GetTotalCaseAdd();
	
	if (m_nTotalCaseAddMin && nValue < m_nTotalCaseAddMin
	 || m_nTotalCaseAddMax && nValue > m_nTotalCaseAddMax)
		return false;

	return true;
}

inline bool ATaskTempl::HasAllFactionPropertyWanted(TaskInterface* pTask) const
{
	if(m_iFactionGrassWanted && pTask->GetFactionGrass() < m_iFactionGrassWanted
	|| m_iFactionMineWanted && pTask->GetFactionMine() < m_iFactionMineWanted
	|| m_iFactionMonsterCoreWanted && pTask->GetFactionMonsterCore() < m_iFactionMonsterCoreWanted
	|| m_iFactionMonsterFoodWanted && pTask->GetFactionMonsterFood() < m_iFactionMonsterFoodWanted
	|| m_iFactionMoneyWanted && pTask->GetFactionMoney() < m_iFactionMoneyWanted)
	return false;
	
	return true;
}

inline bool ATaskTempl::HasBuildToLevel(TaskInterface *pTask) const
{
	if(m_iBuildingIdWanted && m_iBuildingLevelWanted && pTask->GetBuildingLevel(m_iBuildingIdWanted) != m_iBuildingLevelWanted)
		return false;

	return true;
}

#ifdef _TASK_CLIENT
inline bool ATaskTempl::HasFinishFriendNum(TaskInterface* pTask) const
{
	if (pTask->GetFriendNum() >= m_ulFriendNum)
		return true;

	return false;
}
#endif

inline bool ATaskTempl::HasAllTitlesWanted(TaskInterface* pTask) const
{
	for (unsigned long i = 0; i < m_ulTitleWantedNum; i++)
	{
		if (!pTask->HasTitle(m_TitleWanted[i]))
			return false;
	}

	return true;
}

inline bool ATaskTempl::HasAllMonsterWanted(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const
{
	bool bHasOne = false;

	for (unsigned long i = 0; i < m_ulMonsterWanted; i++)
	{
		const MONSTER_WANTED& mw = m_MonsterWanted[i];

		if (mw.m_ulDropItemId)
		{
			unsigned long ulCount = _get_item_count(pTask, mw.m_ulDropItemId, mw.m_bDropCmnItem);
			if (ulCount < mw.m_ulDropItemCount) return false;
			if (ulCount) bHasOne = true;
		}
		else if (pEntry->m_wMonsterNum[i] < mw.m_ulMonsterNum)
			return false;
		else if (pEntry->m_wMonsterNum[i])
			bHasOne = true;
	}

	return bHasOne;
}

inline bool ATaskTempl::HasAllInterObjWanted(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const
{
	bool bHasOne = false;

	for(unsigned long i = 0; i < m_ulInterObjWanted; i++)
	{
		const INTEROBJ_WANTED& wi = m_InterObjWanted[i];
		
		if(pEntry->m_wInterObjNum[i] < wi.m_ulInterObjNum)
			return false;
		else if(pEntry->m_wInterObjNum[i])
			bHasOne = true;
	}
	
	
	return bHasOne;
}

inline unsigned long ATaskTempl::GetFinishCount(TaskInterface* pTask) const
{
	if (m_ulMaxFinishCount == 0)
		return 0;

	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pEntry = pTimeList->Search(m_ID);

	if (pEntry)
		return (unsigned long)pEntry->m_uFinishCount;

	return 0;
}

inline bool ATaskTempl::HasFailItem(TaskInterface* pTask) const
{
	if(m_ulHaveItemFail == 0)
		return false;

	for(unsigned int i=0;i<m_ulHaveItemFail; i++)
	{
		if(_get_item_count(pTask, m_HaveFailItems[i], true) > 0)
			return true;
	}

	return false;
}
inline bool ATaskTempl::NotHasFailItem(TaskInterface* pTask) const
{
	if(m_ulNotHaveItemFail == 0)
		return false;

	for(unsigned int i=0;i<m_ulNotHaveItemFail; i++)
	{
		if(_get_item_count(pTask, m_NotHaveFailItems[i], true) == 0)
			return true;
	}

	return false;
}

inline unsigned long ATaskTempl::HasAllTeamMemsWanted(TaskInterface* pTask, bool bStrict) const
{
	if (m_ulTeamMemsWanted)
	{
		unsigned long pMemEligibleNum[MAX_TEAM_MEM_WANTED];
		memset(pMemEligibleNum, 0, sizeof(pMemEligibleNum));
		const int nMemNum = pTask->GetTeamMemberNum();
		int i;
		unsigned long j;
		task_team_member_info MemInfo;
		float pos[3];
		unsigned long ulWorldId = pTask->GetPos(pos);

		// 检查基本信息
		for (i = 1; i < nMemNum; i++) // 跳过队长
		{
			pTask->GetTeamMemberInfo(i, &MemInfo);

#ifndef	_TASK_CLIENT

			if (bStrict && m_bRcvChckMem)
			{
				if (ulWorldId != MemInfo.m_ulWorldId)
					return TASK_PREREQU_FAIL_OUTOF_DIST;

				float x = pos[0] - MemInfo.m_Pos[0];
				float y = pos[1] - MemInfo.m_Pos[1];
				float z = pos[2] - MemInfo.m_Pos[2];
				float fDist = x * x + y * y + z * z;

				if (fDist > m_fRcvMemDist)
					return TASK_PREREQU_FAIL_OUTOF_DIST;
			}
#endif

			for (j = 0; j < m_ulTeamMemsWanted; j++)
			{
				if (m_TeamMemsWanted[j].IsMeetBaseInfo(&MemInfo, pTask->GetFamilyID()))
				{
					pMemEligibleNum[j]++;
					break;
				}
			}

			if (bStrict && j == m_ulTeamMemsWanted)
				return TASK_PREREQU_FAIL_ILLEGAL_MEM;
		}

		// 检查人数
		for (j = 0; j < m_ulTeamMemsWanted; j++)
			if (!m_TeamMemsWanted[j].IsMeetCount(pMemEligibleNum[j]))
				return TASK_PREREQU_FAIL_ILLEGAL_MEM;
	}
	else if (bStrict && m_bRcvChckMem)
	{
#ifndef	_TASK_CLIENT

		const int nMemNum = pTask->GetTeamMemberNum();
		int i;
		float pos[3], MemPos[3];
		unsigned long ulWorldId = pTask->GetPos(pos);

		for (i = 1; i < nMemNum; i++)
		{
			if (pTask->GetTeamMemberPos(i, MemPos) != ulWorldId)
				return TASK_PREREQU_FAIL_OUTOF_DIST;

			float x = pos[0] - MemPos[0];
			float y = pos[1] - MemPos[1];
			float z = pos[2] - MemPos[2];
			float fDist = x * x + y * y + z * z;

			if (fDist > m_fRcvMemDist) return TASK_PREREQU_FAIL_OUTOF_DIST;
		}

#endif
	}

	return 0;
}

inline bool ATaskTempl::CanFinishTask(
	TaskInterface* pTask,
	const ActiveTaskEntry* pEntry,
	unsigned long ulCurTime) const
{
	if (!pTask->IsDeliverLegal())
		return false;

	switch (m_enumMethod)
	{
	case enumTMCollectNumArticle:
		return HasAllItemsWanted(pTask, pEntry);
	case enumTMTitle:
		return HasAllTitlesWanted(pTask);
	case enumTMWaitTime:
		return pEntry->m_ulTaskTime + m_ulWaitTime < ulCurTime;
	case enumTMAchievement:
		return HasFinishAchievement(pTask);
	case enumTMTotalCaseAdd:
		return CheckTotalTreasure(pTask);
	case enumTMFactionProperty:
		return HasAllFactionPropertyWanted(pTask);
	case enumTMBuildingToLevel:
		return HasBuildToLevel(pTask);
	case enumTMTalkToNPC:
	case enumTMNPCAction:
		return true;
	}

	return pEntry->IsFinished();
}

inline unsigned long ATaskTempl::CheckTeamTask(TaskInterface* pTask) const
{
	if (m_bTeamwork && m_bRcvByTeam) // 组队接收
	{
		if (!pTask->IsCaptain())
			return TASK_PREREQU_FAIL_NOT_CAPTAIN;

		unsigned long ulRet = HasAllTeamMemsWanted(pTask, true);
		if (ulRet)
			return ulRet;
	} 

	// 检查师徒任务
	if (m_bMPTask)
	{
		if (!pTask->IsCaptain())
			return TASK_PREREQU_FAIL_NOT_CAPTAIN;
		
		if (!pTask->IsMaster())
			return TASK_PREREQU_FAIL_NOT_MASTER;
		
		const unsigned long ulPlayerID = pTask->GetPlayerId();
		
		const int nMemNum = pTask->GetTeamMemberNum();
		if (m_bOutMasterTask && nMemNum != 2)			
			return TASK_PREREQU_FAIL_TEAM_NUM;

		for (int i=1; i<nMemNum; i++)
		{
			task_team_member_info Mem;
			pTask->GetTeamMemberInfo(i, &Mem);

			if (m_bOutMasterTask && Mem.m_ulLevel < 90) // 90级才能出师
				return TASK_PREREQU_FAIL_PRENTICE_LEV;

			if (Mem.m_ulMasterID != ulPlayerID)
				return TASK_PREREQU_FAIL_NOT_P_IN_TEAM;				
		}
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckMarriage(TaskInterface* pTask) const
{
	if (m_bMarriage)
	{
		if (pTask->IsMarried())
			return TASK_PREREQU_FAIL_ILLEGAL_MEM;

		if (!pTask->IsInTeam())
			return TASK_PREREQU_FAIL_ILLEGAL_MEM;

		if (pTask->GetTeamMemberNum() != 2)
			return TASK_PREREQU_FAIL_ILLEGAL_MEM;

		task_team_member_info m1, m2;
		pTask->GetTeamMemberInfo(0, &m1);
		pTask->GetTeamMemberInfo(1, &m2);

		if (m1.m_bMale == m2.m_bMale)
			return TASK_PREREQU_FAIL_ILLEGAL_MEM;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckInZone(TaskInterface* pTask) const
{
	if (m_bDelvInZone)
	{
		float pos[3];
		unsigned long ulWorldId = pTask->GetPos(pos);

		if (ulWorldId != m_ulDelvWorld ||
		   !is_in_zone(
			m_DelvMinVert,
			m_DelvMaxVert,
			pos))
			return TASK_PREREQU_FAIL_NOT_IN_ZONE;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckGivenItems(TaskInterface* pTask) const
{
	if (m_ulGivenItems)
	{
		if (!pTask->IsDeliverLegal()) return TASK_PREREQU_FAIL_GIVEN_ITEM;

		if (m_ulGivenCmnCount && !pTask->CanDeliverCommonItem(m_ulGivenCmnCount)
		 || m_ulGivenTskCount && !pTask->CanDeliverTaskItem  (m_ulGivenTskCount))
			return TASK_PREREQU_FAIL_GIVEN_ITEM;
	}

	return 0;
}

inline bool _is_same_week(const tm* t1, const tm* t2, long l1, long l2)
{
	long d = abs(l1 - l2);

	if (d >= (7 * 24 * 3600))
		return false;

	int w1 = task_week_map[t1->tm_wday];
	int w2 = task_week_map[t2->tm_wday];

	if (w1 == w2)
		return d <= 24 * 3600;
	else if (w1 > w2)
		return l1 > l2;
	else
		return l1 < l2;
}

inline bool ATaskTempl::IsBeforeFixedTime(unsigned long ulCurTime) const
{
	tm tmCur;

#ifdef _TASK_CLIENT
	ulCurTime -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);

	if ((long)(ulCurTime) < 0)
		ulCurTime = 0;

	tmCur = *gmtime((time_t*)&ulCurTime);
#else
	tmCur = *localtime((time_t*)&ulCurTime);
#endif

	const task_tm& s = m_tmFixedTime;

	switch(m_iFixedType)
	{
	case 0: // normal
		if ( s.before(&tmCur) )	//如果当前时间大于任务模板中设定的时间
		{
			return false;
		}
		break;

	case 1: // per month
		if( s.before_per_month( &tmCur, false ) )
		{
			return false;
		}
		break;

	case 2:	// per week
		if( s.before_per_week( &tmCur ) )
			return false;
		break;

	case 3: // per day
		if ( s.before_per_day( &tmCur ) )
		{
			return false;
		}
		break;
	}

	return true;
}

inline unsigned long ATaskTempl::GetAwardItemCnt(unsigned long ulItemID) const
{
	if(!m_Award_S || !m_Award_S->HasAward())
		return 0;

	for(unsigned long i=0;i<m_Award_S->m_ulCandItems; i++)
	{
		AWARD_ITEMS_CAND& ic = m_Award_S->m_CandItems[i];

		for(unsigned long j=0;j<ic.m_ulAwardItems;j++)
		{
			const ITEM_WANTED& wi = ic.m_AwardItems[j];
			if(wi.m_ulItemTemplId == ulItemID)
				return wi.m_ulItemNum;
		}
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckFnshLst(TaskInterface* pTask, unsigned long ulCurTime) const
{
	if (!m_bCanRedo || !m_bCanRedoAfterFailure)
	{
		FinishedTaskList* pFinished = (FinishedTaskList*)pTask->GetFinishedTaskList();
		int nRet = pFinished->SearchTask(m_ID);

		// 成功后不能接此任务，或失败后不能重新接此任务
		if (nRet < 0) // 没找到
		{
			if (pFinished->IsFull())
				return TASK_PREREQU_FAIL_CANT_REDO;
		}
		else if (nRet == 0 && !m_bCanRedo || nRet == 1 && !m_bCanRedoAfterFailure)
			return TASK_PREREQU_FAIL_CANT_REDO;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckTimetable(unsigned long ulCurTime) const
{
	if (!m_ulTimetable) return 0;

	unsigned long i;

	for (i = 0; i < m_ulTimetable; i++)
		if (judge_time_date(&m_tmStart[i], &m_tmEnd[i], ulCurTime, (task_tm_type)m_tmType[i]))
			return 0;

	return TASK_PREREQU_FAIL_WRONG_TIME;
}

inline unsigned long ATaskTempl::CheckDeliverTime(TaskInterface* pTask, unsigned long ulCurTime) const
{
	if (m_lAvailFrequency == enumTAFNormal)
		return 0;

	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pEntry = pTimeList->Search(m_ID);

	if (pEntry == 0)
	{
		if (pTimeList->IsFull())
			return TASK_PREREQU_FAIL_WRONG_TIME;
		else
			return 0;
	}
	else if (pEntry->m_ulTimeMark == 0)
		return 0;

	unsigned long ulTaskTime = pEntry->m_ulTimeMark;

	if (m_lAvailFrequency == enumTAFEverySeconds)
	{
		if((long)(ulCurTime - ulTaskTime) >= m_lTimeInterval) // 已经超过上次接收任务时的时间段，肯定可以发放该任务
			return 0;
		else	// 还在当前发放时间段内，检查次数
		{
			if (!m_bRecFinishCount && m_ulMaxFinishCount > 0)  // 间隔一段时间发N次任务
			{
				if(pEntry->m_uFinishCount >= m_ulMaxFinishCount)  // 已经完成了N次
					return TASK_PREREQU_FAIL_FINISH_COUNT;
				else
					return 0;
			}

			return TASK_PREREQU_FAIL_WRONG_TIME;
		}
		
		//return (long)(ulCurTime - ulTaskTime) >= m_lTimeInterval ? 0 : TASK_PREREQU_FAIL_WRONG_TIME;
	}
	tm tmCur, tmTask;

#ifdef _TASK_CLIENT
	ulCurTime -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);
	ulTaskTime -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);
	
	if ((long)(ulCurTime) < 0)
		ulCurTime = 0;

	if ((long)(ulTaskTime) < 0)
		ulTaskTime = 0;

	tmCur = *gmtime((time_t*)&ulCurTime);
	tmTask = *gmtime((time_t*)&ulTaskTime);
#else
	tmCur = *localtime((time_t*)&ulCurTime);
	tmTask = *localtime((time_t*)&ulTaskTime);
#endif

	if (m_lAvailFrequency == enumTAFEachDay)
	{
		if (tmCur.tm_year == tmTask.tm_year
		 && tmCur.tm_yday == tmTask.tm_yday)
			return TASK_PREREQU_FAIL_WRONG_TIME;
	}
	else if (m_lAvailFrequency == enumTAFEachWeek)
	{
		if (_is_same_week(&tmCur, &tmTask, ulCurTime, ulTaskTime))
			return TASK_PREREQU_FAIL_WRONG_TIME;
	}
	else if (m_lAvailFrequency == enumTAFEachMonth)
	{
		if (tmCur.tm_year == tmTask.tm_year
		 && tmCur.tm_mon  == tmTask.tm_mon)
			return TASK_PREREQU_FAIL_WRONG_TIME;
	}
	else
		return TASK_PREREQU_FAIL_WRONG_TIME;

	return 0;
}

inline unsigned long ATaskTempl::CheckLevel(TaskInterface* pTask) const
{
	unsigned long ulLevel = pTask->GetPlayerLevel();

	if (m_ulPremise_Lev_Min && ulLevel < m_ulPremise_Lev_Min) return TASK_PREREQU_FAIL_BELOW_LEVEL;
	if (m_ulPremise_Lev_Max && ulLevel > m_ulPremise_Lev_Max) return TASK_PREREQU_FAIL_ABOVE_LEVEL;

	return 0;
}

inline unsigned long ATaskTempl::CheckTalismanValue(TaskInterface* pTask) const
{
	int nValue = pTask->GetTalismanValue();

	if (m_nTalismanValueMin && nValue < m_nTalismanValueMin
	 || m_nTalismanValueMax && nValue > m_nTalismanValueMax)
		return TASK_PREREQU_FAIL_TALISMAN_VAL;

	return 0;
}

inline unsigned long ATaskTempl::CheckConsumeTreasure(TaskInterface* pTask) const
{
	int nValue = pTask->GetTreasureConsumed();
	
	if (m_nConsumeTreasureMin && nValue < m_nConsumeTreasureMin
	 || m_nConsumeTreasureMax && nValue > m_nConsumeTreasureMax)
		return TASK_PREREQU_FAIL_CONSUME_TREAS;

	return 0;
}

inline unsigned long ATaskTempl::CheckPremTotalTreasure(TaskInterface* pTask) const
{
	int nValue = pTask->GetTotalCaseAdd();

	if (m_nPremTotalCaseAddMin && nValue < m_nPremTotalCaseAddMin
	 || m_nPremTotalCaseAddMax && nValue > m_nPremTotalCaseAddMax)
	 return TASK_PREREQU_FAIL_TOTAL_CASEADD;

	return 0;
}

inline unsigned long ATaskTempl::CheckRepu(TaskInterface* pTask) const
{
	if (m_lPremise_Reputation && pTask->GetReputation() < m_lPremise_Reputation) return TASK_PREREQU_FAIL_BELOW_REPU;
	return 0;
}

inline unsigned long ATaskTempl::CheckFriendship(TaskInterface* pTask) const
{
	for (size_t i = 0; i < SIZE_OF_ARRAY(m_Premise_Friendship); i++)
	{
		if (m_Premise_Friendship[i] > 0 && m_Premise_Friendship[i] > pTask->GetRegionReputation(i))
			return TASK_PREREQU_FAIL_FRIENDSHIP;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckDeposit(TaskInterface* pTask) const
{
	if (m_ulPremise_Deposit && pTask->GetGoldNum() < m_ulPremise_Deposit) return TASK_PREREQU_FAIL_NO_DEPOSIT;
	return 0;
}

inline unsigned long ATaskTempl::CheckContrib(TaskInterface* pTask) const
{
	if (m_lPremise_Contribution && pTask->GetFactionContribution() < m_lPremise_Contribution)
		return TASK_PREREQU_FAIL_CONTRIBUTION;

	if (m_nPremise_FamContrib && pTask->GetFamilyContribution() < m_nPremise_FamContrib)
		return TASK_PREREQU_FAIL_CONTRIBUTION;

	if (m_nPremFamContribMax && pTask->GetFactionContribution() > m_nPremFamContribMax)
		return TASK_PREREQU_FAIL_CONTRIBUTION;

	return 0;
}

inline unsigned long ATaskTempl::CheckItems(TaskInterface* pTask) const
{
	unsigned long i = 0;

	for (; i < m_ulPremItems; i++)
	{
		const ITEM_WANTED& wi = m_PremItems[i];

		if(wi.m_ulRefineLevel)
		{
			if(pTask->GetReinforceItemCount(wi.m_ulItemTemplId, wi.m_ulRefineLevel, wi.m_cRefineCond) == 0)
				return TASK_PREREQU_FAIL_NO_REINFORCE_ITEM;
		}
		else if (_get_item_count(pTask, wi.m_ulItemTemplId, wi.m_bCommonItem) < wi.m_ulItemNum)
			return TASK_PREREQU_FAIL_NO_ITEM;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckFaction(TaskInterface* pTask) const
{
	if (m_ulPremise_Faction)
	{
		 if (!pTask->IsInFaction() || (pTask->GetFactionLev() + 1) < (int)m_ulPremise_Faction)
			return TASK_PREREQU_FAIL_NOT_IN_CLAN;
	}

	if (m_bPremise_FactionMaster)
	{
		if (pTask->GetFactionRole() != 2)
			return TASK_PREREQU_FAIL_NOT_IN_CLAN;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckGender(TaskInterface* pTask) const
{
	bool bMale = pTask->IsMale();

	if (m_ulGender == TASK_GENDER_MALE && !bMale
	 || m_ulGender == TASK_GENDER_FEMALE && bMale)
		return TASK_PREREQU_FAIL_WRONG_GENDER;

	return 0;
}

inline unsigned long ATaskTempl::CheckOccupation(TaskInterface* pTask) const
{
	if (!m_ulOccupations) return 0;

	unsigned long i;
	unsigned long ulOccup = pTask->GetPlayerOccupation();

	for (i = 0; i < m_ulOccupations; i++)
		if (m_Occupations[i] == ulOccup)
			return 0;

	return TASK_PREREQU_FAIL_NOT_IN_OCCU;
}

inline unsigned long ATaskTempl::CheckPeriod(TaskInterface* pTask) const
{
	unsigned long cur = pTask->GetCurPeriod();

	// 当前修真级别大于或等于所需级别
	if (cur < m_ulPremise_Period) return TASK_PREREQU_FAIL_WRONG_PERIOD;

	// 0 - 19
	if (m_ulPremise_Period < 20)
		return 0;

	// 20 - 29
	if (m_ulPremise_Period < 30)
		return cur < 30 ? 0 : TASK_PREREQU_FAIL_WRONG_PERIOD;

	// 30 - 39
	if (m_ulPremise_Period < 40)
		return cur < 40 ? 0 : TASK_PREREQU_FAIL_WRONG_PERIOD;

	return TASK_PREREQU_FAIL_WRONG_PERIOD;
}

inline unsigned long ATaskTempl::CheckPreTask(TaskInterface* pTask) const
{
	unsigned long i;
	FinishedTaskList* pFinished = (FinishedTaskList*)pTask->GetFinishedTaskList();

	for (i = 0; i < m_ulPremise_Task_Count; i++)
		if (pFinished->SearchTask(m_ulPremise_Tasks[i]) != 0)
			return TASK_PREREQU_FAIL_PREV_TASK;

	return 0;
}

extern unsigned long _living_skill_ids[];

inline unsigned long ATaskTempl::CheckLivingSkill(TaskInterface* pTask) const
{
// 	unsigned long i;
// 
// 	for (i = 0; i < MAX_LIVING_SKILLS; i++)
// 	{
// 		if (m_lSkillLev[i] == 0)
// 			continue;
// 
// 		if (!pTask->HasLivingSkill(_living_skill_ids[i])
// 		  || pTask->GetLivingSkillLevel(_living_skill_ids[i]) < m_lSkillLev[i])
// 			return TASK_PREREQU_FAIL_LIVING_SKILL;
// 	}

	if (m_nSkillLev && pTask->GetProduceSkillLev() < m_nSkillLev)
		return TASK_PREREQU_FAIL_LIVING_SKILL;

	return 0;
}

inline unsigned long ATaskTempl::CheckPetConAndCiv(TaskInterface* pTask) const
{
	if (m_nPetCon && pTask->GetPetCon() < m_nPetCon
	 || m_nPetCiv && pTask->GetPetCiv() < m_nPetCiv)
	    return TASK_PREREQU_FAIL_PET;
	
	return 0;
}	 

inline unsigned long ATaskTempl::CheckPKValue(TaskInterface* pTask) const
{
	long lPKValue = pTask->GetPKValue();

	if (lPKValue >= m_lPKValueMin && lPKValue <= m_lPKValueMax)
		return 0;

	return TASK_PREREQU_FAIL_PK_VALUE;
}

inline unsigned long ATaskTempl::CheckGM(TaskInterface* pTask) const
{
	return m_bPremise_GM ? (pTask->IsGM() ? 0 : TASK_PREREQU_FAIL_GM) : 0;
}

inline unsigned long ATaskTempl::CheckLifeAgain(TaskInterface* pTask) const
{
	if (m_bCheckLifeAgain)
	{
		if (m_nLifeAgainCntCompare == 0) //大于等于，后加的代码
		{				
			if (m_ulLifeAgainCnt && pTask->GetPlayerLifeAgainCnt() < m_ulLifeAgainCnt)
				return TASK_PREREQU_FAIL_LIFEAGAIN_CNT;
		}
		else if (m_nLifeAgainCntCompare == 1) //只等于
		{
			if (pTask->GetPlayerLifeAgainCnt() != m_ulLifeAgainCnt)
				return TASK_PREREQU_FAIL_LIFEAGAIN_CNT;
		}
		else if (m_nLifeAgainCntCompare == 2) //小于等于
		{
			if (pTask->GetPlayerLifeAgainCnt() > m_ulLifeAgainCnt)
				return TASK_PREREQU_FAIL_LIFEAGAIN_CNT;
		}
		else
			return TASK_PREREQU_FAIL_INDETERMINATE;
			
		int nkMax = m_ulLifeAgainCnt;
		if (m_ulLifeAgainCnt > 3)
			nkMax = 3;
		
		const bool *pbLifeAgainOccup = NULL;
		for (int k=1; k<=nkMax; k++)
		{
			if (k == 1)
				pbLifeAgainOccup = m_bLifeAgainOneOccup;				
			else if (k == 2)
				pbLifeAgainOccup = m_bLifeAgainTwoOccup;
			else if (k == 3)
				pbLifeAgainOccup = m_bLifeAgainThrOccup;				
			
			int  nCnt  = 0;
			bool bFlag = false;
			unsigned long ulOccup = pTask->GetPlayerLifeAgainOccup(k-1);
			for (int i=0; i<MAX_OCCUPATIONS; i++)
			{
				if (pbLifeAgainOccup[i])
				{
					nCnt++;
					if ((int)ulOccup == i)
						bFlag = true;
				}
			}
			
			if (nCnt != 0 && bFlag == false)
				return TASK_PREREQU_FAIL_LIFEAGAIN_OCC;
		}			
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckTitle(TaskInterface* pTask) const
{
	if (m_ulPremTitleCount == 0)
		return 0;

	for (size_t i = 0; i < m_ulPremTitleCount; i++)
		if (pTask->HasTitle(m_PremTitles[i]))
			return 0;

	return TASK_PREREQU_FAIL_TITLE;
}

inline unsigned long ATaskTempl::CheckSpouse(TaskInterface* pTask) const
{
	if (m_bPremise_Spouse && !pTask->IsMarried())
		return TASK_PREREQU_FAIL_INDETERMINATE;

	return 0;
}

inline unsigned long ATaskTempl::CheckFamily(TaskInterface* pTask) const
{
	if (m_bInFamily && !pTask->IsInFamily())
		return TASK_PREREQU_FAIL_NOT_FAMILY;

	if ((m_bFamilyHeader || m_bSharedByFamily) && !_is_header(pTask->GetFactionRole()))
		return TASK_PREREQU_FAIL_NOT_HEADER;

#ifdef _TASK_CLIENT

	if (NeedFamilySkillData())
	{
		if (!pTask->IsInFamily())
			return TASK_PREREQU_FAIL_NOT_FAMILY;

		int nLev = pTask->GetFamilySkillLevel(m_nFamilySkillIndex);
		int nValue = pTask->GetFamilySkillProficiency(m_nFamilySkillIndex);
		unsigned long ulRet = CheckFamilySkill(nLev, nValue);

		if (ulRet)
			return ulRet;
	}

	if (m_nFamilyMonRecordIndex)
	{
		if (!pTask->IsInFamily())
			return TASK_PREREQU_FAIL_NOT_FAMILY;

		int nRecord = pTask->GetFamilyMonsterRecord(m_nFamilyMonRecordIndex-1);
		unsigned long ulRet = CheckFamilyMonsterRecord(nRecord);

		if (ulRet)
			return ulRet;
	}

	if (m_nFamilyValueMin || m_nFamilyValueMax)
	{
		int nValue = pTask->GetFamilyCommonValue(m_nFamilyValueIndex);
		unsigned long ulRet = CheckFamilyValue(nValue);

		if (ulRet)
			return ulRet;
	}

#endif

	return 0;
}

inline unsigned long ATaskTempl::CheckFinishCount(TaskInterface* pTask) const
{
	if (m_ulMaxFinishCount == 0)
		return 0;

	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pEntry = pTimeList->Search(m_ID);

	if (pEntry)
	{
		if ((unsigned long)pEntry->m_uFinishCount >= m_ulMaxFinishCount)
			return TASK_PREREQU_FAIL_FINISH_COUNT;
	}
	else if (pTimeList->IsFull())
		return TASK_PREREQU_FAIL_FINISH_COUNT;

	return 0;
}

inline unsigned long ATaskTempl::CheckOpenCount(TaskInterface* pTask) const
{
	if(m_bRecFinishCount)	// 此任务需要记录完成次数，因此不需要记录开启次数
		return 0;

	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pEntry = pTimeList->Search(m_ID);

	if (pEntry)
	{
		if (m_ulMaxFinishCount != 0 && (unsigned long)pEntry->m_uFinishCount >= m_ulMaxFinishCount)
			return TASK_PREREQU_FAIL_OPEN_COUNT;
	}
	else if (pTimeList->IsFull())
		return TASK_PREREQU_FAIL_OPEN_COUNT;

	return 0;
}

inline unsigned long ATaskTempl::CheckPremFinishCount(TaskInterface* pTask) const
{
	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();

	for (unsigned long i = 0; i < m_ulPremFinishTaskCount; i++)
	{
		const FINISH_TASK_COUNT_INFO& info = m_PremFinishTasks[i];
		TaskFinishTimeEntry* pEntry = pTimeList->Search(info.task_id);

		if (!pEntry || pEntry->m_uFinishCount < info.count)
			return TASK_PREREQU_FAIL_FINISH_COUNT;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckBattleScore(TaskInterface* pTask) const
{
	if (m_nPremBattleScoreMin && pTask->GetBattleScore() < m_nPremBattleScoreMin)
		return TASK_PREREQU_FAIL_BATTLE_SCORE;

	if (m_nPremBattleScoreMax && pTask->GetBattleScore() > m_nPremBattleScoreMax)
		return TASK_PREREQU_FAIL_BATTLE_SCORE;

	return 0;
}

inline unsigned long ATaskTempl::CheckSJBattleScore(TaskInterface* pTask) const
{
	if (m_nPremSJBattleScore && pTask->GetSJBattleScore() < m_nPremSJBattleScore)
		return TASK_PREREQU_FAIL_BATTLE_SCORE;

	return 0;
}

inline unsigned long ATaskTempl::CheckMasterPrentice(TaskInterface* pTask) const
{
	if (m_bMaster && !pTask->IsMaster())
		return TASK_PREREQU_FAIL_NOT_MASTER;	
	
	if (m_bPrentice && !pTask->IsPrentice())
		return TASK_PREREQU_FAIL_NOT_PRENTICE;

	return 0;
}

inline unsigned long ATaskTempl::CheckCultivation(TaskInterface* pTask) const
{
	if (m_ulPremCult == 0)
		return 0;

	unsigned long ulCult = pTask->GetCultivation();
	if (ulCult == 0)
		return TASK_PREREQU_FAIL_CULT;

	for (int i = 0; i < MAX_PREM_SEL_CULT; i++)
	{
		int bitValPlayer = ulCult & (1<<i);
		int bitValTempl  = m_ulPremCult & (1<<i);
		
		if (bitValPlayer == 0 && bitValTempl != 0)
			return TASK_PREREQU_FAIL_CULT;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckAccomp(TaskInterface* pTask) const
{
	if (m_ulPremAccompCnt == 0)
		return 0;
	
	if (m_bPremAccompCond)
	{
		for (size_t i = 0; i < m_ulPremAccompCnt; i++)
		{
			bool bHasAccomp = pTask->IsAchievementFinish((unsigned short)m_aPremAccompID[i]);
			if (!bHasAccomp)
				return TASK_PREREQU_FAIL_ACCOMP;
		}
		
		return 0;
	}
	else
	{
		for (size_t i = 0; i < m_ulPremAccompCnt; i++)
		{
			bool bHasAccomp = pTask->IsAchievementFinish((unsigned short)m_aPremAccompID[i]);
			if (bHasAccomp)
				return 0;
		}
		
		return TASK_PREREQU_FAIL_ACCOMP;
	}
}

inline unsigned long ATaskTempl::CheckAchievement(TaskInterface* pTask) const
{
	if(m_ulPremAchievementMax == 0 || pTask->GetAchievePoint() >= m_ulPremAchievementMin && pTask->GetAchievePoint() <= m_ulPremAchievementMax)
		return 0;

	return TASK_PREREQU_FAIL_ACHIEVEMENT;
}

#ifdef _TASK_CLIENT
inline unsigned long ATaskTempl::CheckCircleGroupPoints(TaskInterface* pTask) const
{
	if(pTask->GetCircleGroupPoints() >= m_ulPremCircleGroupMin && (m_ulPremCircleGroupMax == 0 || pTask->GetCircleGroupPoints() <= m_ulPremCircleGroupMax) )
		return 0;

	return TASK_PREREQU_FAIL_CIRCLEGROUP;
}
#endif

inline unsigned long ATaskTempl::CheckTerritoryScore(TaskInterface* pTask) const
{
	if(pTask->GetTerritoryScore() >= m_ulPremTerritoryScoreMin && (m_ulPremTerritoryScoreMax == 0 || pTask->GetTerritoryScore() <= m_ulPremTerritoryScoreMax) )
		return 0;

	return TASK_PREREQU_FAIL_TERRITORY_SCORE;
}

inline unsigned long ATaskTempl::CheckFengshen(TaskInterface* pTask) const
{
	if(m_nPremFengshenType == enumPremFSAll)
		return 0;
	else if(m_nPremFengshenType == enumPremFSNoneFengShenOnly)
	{
		if(pTask->GetFengshenLevel() == 0)	// 未封神，角色封神等级为0
			return 0;
	}
	else if(m_nPremFengshenType == enumPremFSFengShenOnly)
	{
		if(pTask->GetFengshenLevel() >= m_ulPremFengshenLvlMin && pTask->GetFengshenLevel() <= m_ulPremFengshenLvlMax)
		{
			if(!m_bExpMustFull ||
			   (m_bExpMustFull && pTask->IsFengshenExpFull()))
				return 0;
		}
	}

	return TASK_PREREQU_FAIL_FENGSHEN_LVL_OR_EXP;
}

//检查角色创建时间长度的先决条件Added 2011-04-11.
inline unsigned long ATaskTempl::CheckCreateRoleTime( TaskInterface* pTask ) const
{
	//如果任务模板中角色创建时间长度为0，则表示不需要判断
	if ( !m_ulCreateRoleTimeDuration )
	{
		return 0;
	}

	//首先，通过TaskInterface接口类获得当前角色的创建时间长度
	unsigned long nCreateRoleTimeDuration = pTask->GetCreateRoleTimeDuration();

	//将创建时间（当前为：秒）转化为小时
	nCreateRoleTimeDuration /= 3600;

	//和该任务模板中的时间长度比较，如果符合条件，返回0；否则返回错误码
	if ( nCreateRoleTimeDuration >= m_ulCreateRoleTimeDuration ) //注意，m_ulCreateRoleTimeDuration是以小时为单位
	{
		return 0;
	}

	return TASK_PREREQU_FAIL_CREATE_ROLE_TIME;

}


inline unsigned long ATaskTempl::CheckTransform(TaskInterface* pTask) const
{
	// 只有处于非变身状态，才能接到任务
	if(m_iPremTransformID == -1)
	{
		if(pTask->GetTransformID() != 0)
			return TASK_PREREQU_FAIL_TRANSFORM;
		else
			return 0;
	}

	if (m_iPremTransformID && m_iPremTransformID != pTask->GetTransformID())
		return TASK_PREREQU_FAIL_TRANSFORM;

	if (m_iPremTransformLevel != -1 && m_iPremTransformLevel > pTask->GetTransformLevel())
		return TASK_PREREQU_FAIL_TRANSFORM;

	if (m_iPremTransformExpLevel != -1 && m_iPremTransformExpLevel > pTask->GetTransformExpLevel())
		return TASK_PREREQU_FAIL_TRANSFORM;

	return 0;
}

inline void ATaskTempl::CalcAwardItemsCount(
	TaskInterface* pTask,
	ActiveTaskEntry* pEntry, 
	const AWARD_ITEMS_CAND* pAward,
	unsigned long& ulCmnCount,
	unsigned long& ulTskCount) const
{
	if (pAward->m_bRandChoose)
	{
		for (unsigned long i = 0; i < pAward->m_ulAwardItems; i++)
		{
			const ITEM_WANTED& iw = pAward->m_AwardItems[i];

			unsigned long ulItemNum = iw.m_ulItemNum;
			if (iw.m_fProb < 1.0f) continue;
			if (iw.m_bCommonItem)
				ulCmnCount += pTask->GetItemSlotCnt(iw.m_ulItemTemplId, ulItemNum);
			else 
				ulTskCount += pTask->GetItemSlotCnt(iw.m_ulItemTemplId, ulItemNum);
		}

		ulCmnCount++;
		ulTskCount++;
	}
	else
	{
		for (unsigned long i = 0; i < pAward->m_ulAwardItems; i++)
		{
			const ITEM_WANTED& iw = pAward->m_AwardItems[i];
			unsigned long ulItemNum = 0;
			
			unsigned ulType = (pEntry->IsSuccess() ? m_ulAwardType_S : m_ulAwardType_F);
			if(ulType == enumTATEach && m_enumMethod == enumTMCollectNumArticle)
			{
				// 计算收集的物品数量
				for (unsigned long k = 0; k < m_ulItemsWanted; k++)
				{
					ulItemNum += _get_item_count(
						pTask,
						m_ItemsWanted[k].m_ulItemTemplId,
						m_ItemsWanted[k].m_bCommonItem);
				}
			}
			else
				ulItemNum = iw.m_ulItemNum;

			if (iw.m_bCommonItem)
				ulCmnCount += pTask->GetItemSlotCnt(iw.m_ulItemTemplId, ulItemNum);
			else 
				ulTskCount += pTask->GetItemSlotCnt(iw.m_ulItemTemplId, ulItemNum);
		}
	}
}

inline unsigned long ATaskTempl::CheckNationPosition(TaskInterface* pTask) const
{
	if (m_ulPremNationPositionMask != 0)
	{
		unsigned long mask = (pTask->IsKing() ? 0x0001 : 0)
						   | (pTask->IsQueen() ? 0x0002 : 0)
						   | (pTask->IsGeneral() ? 0x0004 : 0)
						   | (pTask->IsOfficial() ? 0x0008 : 0)
						   | (pTask->IsMember() ? 0x0010 : 0)
						   | (pTask->IsGuard() ? 0x0020 : 0);
		
		if ((m_ulPremNationPositionMask & mask) == 0)
			return TASK_PREREQU_FAIL_NATION_POSTION;
	}

	return 0;
}

inline unsigned long ATaskTempl::CheckKingScore	 (TaskInterface* pTask) const
{
	int score = pTask->GetKingScore();
	if (m_nPremKingScoreCost > 0 && score < m_nPremKingScoreCost)
		return TASK_PREREQU_FAIL_KING_NOT_ENOUGH_SCORE;

	if (m_nPremKingScoreMax > 0 && score > m_nPremKingScoreMax)
		return TASK_PREREQU_FAIL_KING_ABOVE_SCORE;

	if (m_nPremKingScoreMin > 0 && score > m_nPremKingScoreMin)
		return TASK_PREREQU_FAIL_KING_BELOW_SCORE;

	return 0;
}

inline unsigned long ATaskTempl::CheckHasKing(TaskInterface* pTask) const
{
	if (m_bPremHasKing && !pTask->HasKing())
		return TASK_PREREQU_FAIL_HAS_KING;
	return 0;
}


inline unsigned long ATaskTempl::CheckBuildLevel(TaskInterface* pTask) const
{
	if(m_nBuildId && m_nBuildLevel && pTask->GetBuildingLevel(m_nBuildId) != m_nBuildLevel)
		return TASK_PREREQU_FAIL_FACTION_BUILDING;
	return 0;
}

inline unsigned long ATaskTempl::CheckBuildLevelCon(TaskInterface* pTask) const
{
	if(m_nBuildLevelInConstruct && pTask->GetBuildLevelInConstruct() != m_nBuildLevelInConstruct)
		return TASK_PREREQU_FAIL_BUILDINGCON;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionTask(TaskInterface* pTask) const
{
	if(m_bFaction && pTask->GetFactionTaskFinishedCount() >= MAX_FACTION_TASK_COUNT)
		return TASK_PREREQU_FAIL_FACTION_TASK_COUNT;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionGoldNote(TaskInterface* pTask) const
{	
	if(m_iPremise_FactionGoldNote && pTask->GetFactionGoldNote() < m_iPremise_FactionGoldNote)
		return TASK_PREREQU_FAIL_FACTION_GOLD_NOTE;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionGrass(TaskInterface* pTask) const
{
	if(m_iPremise_FactionGrass && pTask->GetFactionGrass() < m_iPremise_FactionGrass)
		return TASK_PREREQU_FAIL_FACTION_GRASS;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionMine(TaskInterface *pTask) const
{
	if(m_iPremise_FactionMine && pTask->GetFactionMine() < m_iPremise_FactionGrass)
		return TASK_PREREQU_FAIL_FACITON_MINE;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionMonsterCore(TaskInterface* pTask) const
{
	if(m_iPremise_FactionMonsterCore && pTask->GetFactionMonsterCore() < m_iPremise_FactionMonsterCore)
		return TASK_PREREQU_FAIL_FACTION_MONSTER_CORE;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionMosnterFood(TaskInterface* pTask) const
{
	if(m_iPremise_FactionMonsterFood && pTask->GetFactionMonsterFood() < m_iPremise_FactionMonsterFood)
		return TASK_PREREQU_FAIL_FACTION_MONSTER_FOOD;
	return 0;
}

inline unsigned long ATaskTempl::CheckFactionMoney(TaskInterface* pTask) const
{
	if(m_iPremise_FactionMoney && pTask->GetFactionMoney() < m_iPremise_FactionMoney)
		return TASK_PREREQU_FAIL_FACTION_MONEY;
	return 0;
}

inline unsigned long ATaskTempl::CheckInterObj(TaskInterface* pTask) const
{
	if(m_iInterObjId && pTask->InInterAction(m_iInterObjId))
		return TASK_PREREQU_FAIL_INTEROBJ;
	return 0;
}

inline bool ATaskTempl::CanAwardItems(TaskInterface* pTask, ActiveTaskEntry* pEntry, const AWARD_ITEMS_CAND* pAward) const
{
	unsigned long ulCmnCount = 0;
	unsigned long ulTskCount = 0;

	CalcAwardItemsCount(pTask, pEntry, pAward, ulCmnCount, ulTskCount);
	return pTask->CanDeliverCommonItem(ulCmnCount) && pTask->CanDeliverTaskItem(ulTskCount);
}

//Added 2011-03-17.
inline void ATaskTempl::CalcAwardDataIndex(
		TaskInterface* pTask, 
		const AWARD_DATA* pAward, 
		ActiveTaskEntry* pEntry, 
		unsigned long ulTaskTime, 
		unsigned long ulCurTime, 
		int& param) const
{
	int nSuccessOrNot = (pEntry->IsSuccess() ? 1 : 0), nAwardType = 0, nIndex = 0;

	//首先，获得当前可以奖励的方式。
	unsigned ulType = (pEntry->IsSuccess() ? m_ulAwardType_S : m_ulAwardType_F);
	nAwardType = ulType;

	//判断奖励方式，计算index。
	switch (ulType)
	{
		case enumTATNormal:
		case enumTATEach:
			nIndex = (pEntry->IsSuccess() ? 1 : 0);
			break;
		case enumTATRatio:
		{
			if (!m_ulTimeLimit) return;

			const AWARD_RATIO_SCALE* p = (pEntry->IsSuccess() ? m_AwByRatio_S : m_AwByRatio_F);
			float ratio = (float)(ulCurTime - ulTaskTime) / m_ulTimeLimit;
			unsigned long i;

			for (i = 0; i < p->m_ulScales; i++)
			{
				if (ratio <= p->m_Ratios[i])
				{
					nIndex = i;
					break;
				}
			}
			break;
		}
		case enumTATItemCount:
		{
			const AWARD_ITEMS_SCALE* p = (pEntry->IsSuccess() ? m_AwByItems_S : m_AwByItems_F);
			unsigned long ulCount = pTask->GetTaskItemCount(p->m_ulItemId), i;

			for (i = 0; i < p->m_ulScales; i++)
			{
				if (ulCount >= p->m_Counts[i])
				{
					nIndex = i;
					break;
				}
			}
			break;
		}
		case enumTATFinishCount:
		{
			const AWARD_COUNT_SCALE* p = (pEntry->IsSuccess() ? m_AwByCount_S : m_AwByCount_F);
			TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
			TaskFinishTimeEntry* pFinish = pTimeList->Search(m_ID);
			unsigned long ulCount = (pFinish ? pFinish->m_uFinishCount : 0);

			for (int i = (int)p->m_ulScales-1; i >= 0; i--)
			{
				if (ulCount >= p->m_Counts[i])
				{
					nIndex = i;
					break;
				}
			}
			break;
		}
		default:
			assert(false);
			break;
	}
	
	//将nSuccessOrNot，nAwardType和nIndex 压入到变量param中
	param = 0x80000000 | (nSuccessOrNot << 16) | (nAwardType << 8) | (nIndex & 0x000000ff);
}


inline void ATaskTempl::GetAwardData( int param, AWARD_DATA* pAward )
{
	//首先，通过param计算出各个字节的数值
	int nSuccessOrNot = 0, nAwardType = 0, nIndex = 0;
	nSuccessOrNot	= (param >> 16) & 0x000000ff;
	nAwardType		= (param >> 8) & 0x000000ff;
	nIndex			= param & 0x000000ff;

	//通过这3个索引，计算出具体的奖励数据pAward
	memset(pAward, 0, sizeof(AWARD_DATA));
	unsigned ulType = ((nSuccessOrNot == 1) ? m_ulAwardType_S : m_ulAwardType_F);
	//检测一下看传过来的nAwardType是否等于ulType
	if( ulType != (unsigned int)nAwardType )
	{
		return;
	}

	switch( ulType )
	{
		case enumTATNormal:
		case enumTATEach:
			*pAward = ((nSuccessOrNot == 1) ? *m_Award_S : *m_Award_F);
			break;
		case enumTATRatio:
		{
			const AWARD_RATIO_SCALE* p = ((nSuccessOrNot == 1) ? m_AwByRatio_S : m_AwByRatio_F);
			*pAward = p->m_Awards[nIndex];
			break;
		}
		case enumTATItemCount:
		{
			const AWARD_ITEMS_SCALE* p = ((nSuccessOrNot == 1) ? m_AwByItems_S : m_AwByItems_F);
			*pAward = p->m_Awards[nIndex];
			break;
		}
		case enumTATFinishCount:
		{
			const AWARD_COUNT_SCALE* p = ((nSuccessOrNot == 1) ? m_AwByCount_S : m_AwByCount_F);
			*pAward = p->m_Awards[nIndex];
			break;
		}
		default:
			assert(false);
			break;
	}
}

//Added end.

inline bool ATaskTempl::CanDirectFinish() const	//可以直接完成
{
	if(m_enumMethod == enumTMNPCAction || m_enumMethod == enumTMSpecialTrigger)
		return true;

	return false;
}

inline void ATaskTempl::CalcAwardData(
	TaskInterface* pTask,
	AWARD_DATA* pAward,
	ActiveTaskEntry* pEntry,
	unsigned long ulTaskTime,
	unsigned long ulCurTime) const
{
	memset(pAward, 0, sizeof(AWARD_DATA));
	unsigned ulType = (pEntry->IsSuccess() ? m_ulAwardType_S : m_ulAwardType_F);

	switch (ulType)
	{
	case enumTATNormal:
	case enumTATEach:
		*pAward = (pEntry->IsSuccess() ? *m_Award_S : *m_Award_F);
		break;
	case enumTATRatio:
		if (!m_lDynFinishClearTime)
			CalcAwardDataByRatio(pAward, pEntry, ulTaskTime, ulCurTime);
		else
			assert(false);
		break;
	case enumTATItemCount:
		CalcAwardDataByItems(pTask, pAward, pEntry);
		break;
	case enumTATFinishCount:
		CalcAwardDataByCount(pTask, pAward, pEntry);
		break;
	default:
		assert(false);
		break;
	}
}

inline void ATaskTempl::CalcAwardDataByRatio(
	AWARD_DATA* pAward,
	ActiveTaskEntry* pEntry,
	unsigned long ulTaskTime,
	unsigned long ulCurTime) const
{
	if (!m_ulTimeLimit) return;

	const AWARD_RATIO_SCALE* p = (pEntry->IsSuccess() ? m_AwByRatio_S : m_AwByRatio_F);
	float ratio = (float)(ulCurTime - ulTaskTime) / m_ulTimeLimit;
	unsigned long i;

	for (i = 0; i < p->m_ulScales; i++)
	{
		if (ratio <= p->m_Ratios[i])
		{
			*pAward = p->m_Awards[i];
			return;
		}
	}
}

inline void ATaskTempl::CalcAwardDataByItems(
	TaskInterface* pTask,
	AWARD_DATA* pAward,
	ActiveTaskEntry* pEntry) const
{
	const AWARD_ITEMS_SCALE* p = (pEntry->IsSuccess() ? m_AwByItems_S : m_AwByItems_F);
	unsigned long ulCount = pTask->GetTaskItemCount(p->m_ulItemId), i;

	for (i = 0; i < p->m_ulScales; i++)
	{
		if (ulCount >= p->m_Counts[i])
		{
			*pAward = p->m_Awards[i];
			return;
		}
	}
}

inline void ATaskTempl::CalcAwardDataByCount(
	TaskInterface* pTask,
	AWARD_DATA* pAward,
	ActiveTaskEntry* pEntry) const
{
	const AWARD_COUNT_SCALE* p = (pEntry->IsSuccess() ? m_AwByCount_S : m_AwByCount_F);
	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
	TaskFinishTimeEntry* pFinish = pTimeList->Search(m_ID);
	unsigned long ulCount = (pFinish ? pFinish->m_uFinishCount : 0);

	for (int i = (int)p->m_ulScales-1; i >= 0; i--)
	{
		if (ulCount >= p->m_Counts[i])
		{
			*pAward = p->m_Awards[i];
			return;
		}
	}
}

inline unsigned long ATaskTempl::CalcAwardMulti(
	TaskInterface* pTask,
	ActiveTaskEntry* pEntry,
	unsigned long ulTaskTime,
	unsigned long ulCurTime) const
{
	unsigned ulType = (pEntry->IsSuccess() ? m_ulAwardType_S : m_ulAwardType_F);
	if (ulType != enumTATEach) return 1;

	if (m_enumMethod == enumTMCollectNumArticle)
	{
		unsigned long ulCount = 0, i;

		// 计算收集的物品数量，算上替代品的数量
		for (i = 0; i < m_ulItemsWanted; i++)
		{
			ulCount += _get_item_count(
				pTask,
				m_ItemsWanted[i].m_ulItemTemplId,
				m_ItemsWanted[i].m_bCommonItem);

			if(m_ItemsWanted[i].m_ulReplaceItemTemplId != 0)
			{
				ulCount += _get_item_count(
					pTask,
					m_ItemsWanted[i].m_ulReplaceItemTemplId,
					m_ItemsWanted[i].m_bCommonItem);
			}
		}

		return ulCount;
	}
	else if (m_enumMethod == enumTMKillNumMonster)
	{
		unsigned long ulCount = 0;

		for (unsigned long i = 0; i < m_ulMonsterWanted; i++)
		{
			const MONSTER_WANTED& mw = m_MonsterWanted[i];

			if (mw.m_ulDropItemId)
				ulCount += _get_item_count(pTask, mw.m_ulDropItemId, mw.m_bDropCmnItem);
			else
				ulCount += pEntry->m_wMonsterNum[i];
		}

		return ulCount;
	}

	return 0;
}

inline float ATaskTempl::CalcAwardByParaExp(TaskInterface* pTask, const AWARD_DATA* pAward) const
{
	if (pAward->m_nParaExpSel == 0)
		return 1;	
	
	TaskExpAnalyser Analyser;
	TaskExpressionArr ExpArr;
	
	for (unsigned int i = 0; i < pAward->m_ulParaExpArrLen; i++)
		ExpArr.push_back(pAward->m_pParaExpArr[i]);
	
	float fRetValue = 1.0f;
	try
	{
		fRetValue = Analyser.Run(pTask, ExpArr, enumTaskCalcExp);
	}
	catch (char* szErr)
	{
		char log[1024];
		sprintf(log, "CalcAwardByParaExp, Expression run err: %s", szErr);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);
		
		return 1;
	}
	
	return fRetValue;	
}


#ifdef _TASK_CLIENT

inline void _notify_svr(TaskInterface* pTask, unsigned char uReason, unsigned short uTaskID)
{
	task_notify_base notify;
	notify.reason = uReason;
	notify.task = uTaskID;
	pTask->NotifyServer(&notify, sizeof(notify));
}

inline bool ATaskTempl::HasShowCond() const
{
	if (!m_bShowByDeposit
	|| !m_bShowByGender
	|| !m_bShowByItems
	|| !m_bShowByLev
	|| !m_bShowByOccup
	|| !m_bShowByPreTask
	|| !m_bShowByRepu
	|| !m_bShowByTeam
	|| !m_bShowByFaction
	|| !m_bShowByPeriod)
		return true;
	return false;
}

inline void ATaskTempl::OnServerNotify(
	TaskInterface* pTask,
	ActiveTaskEntry* pEntry,
	const task_notify_base* pNotify,
	size_t sz) const
{
	unsigned long ulTime, ulCaptainTask;
	ActiveTaskList* pLst;
	const ATaskTempl* pSub;
	task_sub_tags sub_tags;
	memset(&sub_tags, 0, sizeof(sub_tags));
	unsigned long i;
	const svr_monster_killed* pKilled;
	const svr_items_to_get*   pToGet;
	const svr_interobj_collected* pCollected;

	switch (pNotify->reason)
	{
	case TASK_SVR_NOTIFY_MONSTER_KILLED:
		if (sz != sizeof(svr_monster_killed)) break;
		if (m_enumMethod != enumTMKillNumMonster) break;

		pKilled = static_cast<const svr_monster_killed*>(pNotify);

		for (i = 0; i < m_ulMonsterWanted; i++)
		{
			const MONSTER_WANTED& mw = m_MonsterWanted[i];

			if (mw.m_ulMonsterTemplId == pKilled->monster_id)
			{
				pEntry->m_wMonsterNum[i] = pKilled->monster_num;
#ifdef _ELEMENTCLIENT
				TaskInterface::PopUpTaskMonsterInfo(m_ID, mw.m_ulMonsterTemplId);
#endif
				break;
			}
		}

		break;
	case TASK_SVR_NOTIFY_INTEROBJ_COLLECT:
		if(sz != sizeof(svr_interobj_collected)) break;
		if(m_enumMethod != enumTMCollectNumInterObj) break;
		
		pCollected = static_cast<const svr_interobj_collected*>(pNotify);

		for(i = 0; i < m_ulInterObjWanted; i++)
		{
			const INTEROBJ_WANTED& wi = m_InterObjWanted[i];
			
			if(wi.m_ulInterObjId == pCollected->interobj_id)
			{
				pEntry->m_wInterObjNum[i] = pCollected->interobj_num;
				break;
			}
		}

	case TASK_SVR_NOTIFY_ITEMS_TO_GET:
		if (sz != sizeof(svr_items_to_get)) break;
		if (m_enumMethod != enumTMCollectNumArticle) break;

		pToGet = static_cast<const svr_items_to_get*>(pNotify);		
		pEntry->m_wItemsToGet = pToGet->items_num;	
#ifdef _ELEMENTCLIENT
		LuaEvent::FirstCollectItem();
#endif
		break;
	case TASK_SVR_NOTIFY_NEW:
		if (!static_cast<const svr_new_task*>(pNotify)->valid_size(sz)) break;
		pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
		static_cast<const svr_new_task*>(pNotify)->get_data(
			ulTime,
			ulCaptainTask,
			sub_tags
			);

		if (sub_tags.sub_task)
		{
			pSub = GetConstSubById(sub_tags.sub_task);
			if (!pSub) break;
		}
		else
			pSub = NULL;

		if (CheckBudget(pLst)) break;

		DeliverTask(
			pTask,
			pLst,
			NULL,
			ulCaptainTask,
			*pTask->GetTaskMask(),
			ulTime,
			pSub,
			&sub_tags,
			NULL);
	
		if (m_lAvailFrequency != enumTAFNormal)
			static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList())->AddOrUpdateDeliverTime(m_ID, ulTime);

		if (!m_bRecFinishCount && m_ulMaxFinishCount > 0)	// 若要记录开启次数
			static_cast<TaskFinishTimeList*>(pTask->GetFinishedTimeList())->AddOrUpdateFinishCount(m_ID);

		if (CanShowPrompt()) TaskInterface::ShowTaskMessage(m_ID, TASK_MSG_NEW);
		if (m_enumMethod != enumTMTinyGame)
		{
			TaskInterface::UpdateTaskUI();
		}	
		
		if(m_ulAnimation)
		{
			pTask->PlayAnimation(m_ulAnimation);
		}

		if(m_ulTinyGameID != 0)
		{
			pTask->PlayTinyGame(m_ulTinyGameID);
		}

#ifdef _ELEMENTCLIENT
		LuaEvent::AcceptScriptCompleteTask(m_ID);
		//NewHelpGuide::TriggerGetTask();
#endif
		break;
	case TASK_SVR_NOTIFY_COMPLETE:
		if (!static_cast<const svr_task_complete*>(pNotify)->valid_size(sz)) break;
		static_cast<const svr_task_complete*>(pNotify)->get_data(
			ulTime,
			sub_tags
			);

		pEntry->m_uState = static_cast<const svr_task_complete*>(pNotify)->sub_tags.state;

		if (CanShowPrompt()) TaskInterface::ShowTaskMessage(
			m_ID,
			(pEntry->IsSuccess() && !pEntry->IsGiveUp()) ? TASK_MSG_SUCCESS : TASK_MSG_FAIL);

#ifdef _ELEMENTCLIENT
		if(pEntry->IsSuccess() && !pEntry->IsGiveUp())
		{
			LuaEvent::FirstFinishTask();
			LuaEvent::FirstTaskAward();
		}
		else
		{
			LuaEvent::FirstTaskFail();
		}
#endif
		RecursiveAward(
			pTask,
			static_cast<ActiveTaskList*>(pTask->GetActiveTaskList()),
			pEntry,
			ulTime,
			-1,
			&sub_tags);

		if (m_enumMethod != enumTMTinyGame)
		{
			TaskInterface::UpdateTaskUI();
		}

		if(m_Award_S->m_ulAnimation)
		{
			pTask->PlayAnimation(m_Award_S->m_ulAnimation);
		}

		if(m_Award_S->m_ulSpecialAwardType != 0)
		{
			if(m_Award_S->m_ulSpecialAwardType == 1)	// gfx
				pTask->PlayAwardGfx(m_Award_S->m_ulSpecialAwardID);
			else if(m_Award_S->m_ulSpecialAwardType == 2)	// tiny game
				pTask->PlayTinyGame(m_Award_S->m_ulSpecialAwardID);
		}

		break;
	case TASK_SVR_NOTIFY_GIVE_UP:
		pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
		pLst->ClearTask(pTask, pEntry, false);
		TaskInterface::UpdateTaskUI();
		break;
	case TASK_SVR_NOTIFY_FINISHED:
		pEntry->SetFinished();
		break;
	default:
		assert(false);
	}
}

#else // Linux Part

inline const ATaskTempl* ATaskTempl::RandOneChild(TaskInterface* pTask, int& nSub) const
{
	if (!m_nSubCount)
		return NULL;

	nSub = static_cast<int>(pTask->UnitRand() * m_nSubCount);
	if (nSub >= m_nSubCount) nSub = m_nSubCount - 1;
	return GetSubByIndex(nSub);
}

inline void ATaskTempl::DeliverGivenItems(TaskInterface* pTask) const
{
	if (CheckGivenItems(pTask) != 0) return;
	unsigned long i;

	for (i = 0; i < m_ulGivenItems; i++)
	{
		const ITEM_WANTED& iw = m_GivenItems[i];

		if (iw.m_bCommonItem)
			pTask->DeliverCommonItem(iw.m_ulItemTemplId, iw.m_ulItemNum, false, 0);
		else
			pTask->DeliverTaskItem(iw.m_ulItemTemplId, iw.m_ulItemNum);
	}
}

inline void ATaskTempl::TakeAwayGivenItems(TaskInterface* pTask) const
{
	unsigned long i, ulCount;

	for (i = 0; i < m_ulGivenItems; i++)
	{
		const ITEM_WANTED& wi = m_GivenItems[i];

		if (wi.m_bCommonItem)
		{
			ulCount = pTask->GetCommonItemCount(wi.m_ulItemTemplId);
			if (ulCount > wi.m_ulItemNum) ulCount = wi.m_ulItemNum;
			if (ulCount) pTask->TakeAwayCommonItem(wi.m_ulItemTemplId, ulCount);
		}
		else
		{
			ulCount = pTask->GetTaskItemCount(wi.m_ulItemTemplId);
			if (ulCount) pTask->TakeAwayTaskItem(wi.m_ulItemTemplId, ulCount);
		}
	}
}

inline bool ATaskTempl::RemovePrerequisiteItem(TaskInterface* pTask) const
{
	if (!pTask->IsDeliverLegal())
	{
		if (m_ulPremise_Deposit || m_ulPremItems)
			return false;
	}

	unsigned long i;

	if (m_ulPremise_Deposit)
		pTask->TakeAwayGold(m_ulPremise_Deposit);

	if (m_bDepositContribution && m_lPremise_Contribution > 0)
		pTask->DeliverFactionContribution(-m_lPremise_Contribution);

	if (m_bDepositFamContrib && m_nPremise_FamContrib > 0)
		pTask->DeliverFamilyContribution(-m_nPremise_FamContrib);

	if (m_bRepuDeposit && m_lPremise_Reputation > 0)
		pTask->DeliverReputation(-m_lPremise_Reputation);

	if (m_bDepositBattleScore && m_nPremBattleScoreMin > 0)
		pTask->DeliverBattleScore(-m_nPremBattleScoreMin);

	if (m_bSJDepostiBattleScore && m_nPremSJBattleScore > 0)
		pTask->DeliverSJBattleScore(-m_nPremSJBattleScore);

	if (m_bDepositFamilyValue && m_nFamilyValueMin > 0)
	{
		TaskPairData p(m_nFamilyValueIndex, -m_nFamilyValueMin, 0);
		TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyAddCommonValue, m_ID, enumTaskReasonAddCommonValue, 0, &p, 1);
		pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
	}

	if (m_bFriendshipDeposit)
	{
		for (i = 0; i < TASK_ZONE_FRIENDSHIP_COUNT; i++)
		{
			if (m_Premise_Friendship[i] > 0)
				pTask->DeliverRegionReputation(i, -m_Premise_Friendship[i]);
		}
	}

	if (!m_bItemNotTakeOff)
	{
		for (i = 0; i < m_ulPremItems; i++)
		{
			const ITEM_WANTED& wi = m_PremItems[i];

			if (wi.m_ulItemTemplId && wi.m_ulItemNum)
			{
				if(wi.m_ulRefineLevel)
					pTask->TakeAwayReinforceItem(wi.m_ulItemTemplId, wi.m_ulRefineLevel, wi.m_cRefineCond);
				else
					pTask->TakeAwayItem(wi.m_ulItemTemplId, wi.m_ulItemNum, wi.m_bCommonItem);
			}
		}
	}
	return true;
}

inline bool ATaskTempl::SummonPremMonsters(TaskInterface* pTask) const
{
	if (!pTask->IsDeliverLegal())
			return false;

	if(m_ulPremMonsterSummoned)
	{
		if(!m_bRandSelMonster)
		{
			for(int i=0; i<m_ulPremMonsterSummoned; i++)
			{
				MONSTER_SUMMONED& ms = m_MonsterSummoned[i];
				
				float pos[3];
				pos[0] = ms.m_SummonVert.x;
				pos[1] = ms.m_SummonVert.y;
				pos[2] = ms.m_SummonVert.z;

				pTask->SummonMonster(ms.m_ulMonsterTemplID, ms.m_bIsMonster, ms.m_ulMonsterNum, ms.m_ulMapId, pos,
									 ms.m_lPeriod, m_ulSummonMode);
			}
		}
		else	// 随机召唤一种怪
		{
			int k = static_cast<int>(pTask->UnitRand() * m_ulPremMonsterSummoned);
			if (k >= m_ulPremMonsterSummoned)
				k = m_ulPremMonsterSummoned - 1;

			MONSTER_SUMMONED& ms = m_MonsterSummoned[k];
			float pos[3];
			pos[0] = ms.m_SummonVert.x;
			pos[1] = ms.m_SummonVert.y;
			pos[2] = ms.m_SummonVert.z;

			pTask->SummonMonster(ms.m_ulMonsterTemplID, ms.m_bIsMonster, ms.m_ulMonsterNum, ms.m_ulMapId, pos,
								 ms.m_lPeriod, m_ulSummonMode);
		}
	}

	return true;
}


inline void ATaskTempl::RemoveAcquiredItem(TaskInterface* pTask, bool bClearTask, bool bSuccess) const
{
	if(m_bHaveItemFail && !m_bHaveItemFailNotTakeOff)
	{
		for(unsigned char i=0; i< m_ulHaveItemFail; i++)
		{
			unsigned long ulCount;
			ulCount = pTask->GetCommonItemCount(m_HaveFailItems[i]);
			if (!ulCount) continue;

			pTask->TakeAwayCommonItem(m_HaveFailItems[i], ulCount);
		}
	}

	if (m_enumMethod == enumTMCollectNumArticle)
	{
		for (unsigned char i = 0; i < m_ulItemsWanted; i++)
		{
			const ITEM_WANTED& wi = m_ItemsWanted[i];
			unsigned long ulCount, ulReplaceCount = 0;

			if (wi.m_bCommonItem)
			{
				if (bClearTask || !bSuccess) continue;
				ulCount = pTask->GetCommonItemCount(wi.m_ulItemTemplId);
				if(wi.m_ulReplaceItemTemplId)
					ulReplaceCount = pTask->GetCommonItemCount(wi.m_ulReplaceItemTemplId);
				if (!(ulCount+ulReplaceCount)) continue;

				if (wi.m_ulItemNum && ulCount > wi.m_ulItemNum) 
					ulCount = wi.m_ulItemNum;
				if (wi.m_ulItemNum && ulCount < wi.m_ulItemNum)
				{
					if(ulReplaceCount > wi.m_ulItemNum - ulCount)
						ulReplaceCount = wi.m_ulItemNum - ulCount;
				}

				pTask->TakeAwayCommonItem(wi.m_ulItemTemplId, ulCount);

				if(ulCount < wi.m_ulItemNum)
					pTask->TakeAwayCommonItem(wi.m_ulReplaceItemTemplId, ulReplaceCount);
			}
			else
			{
				ulCount = pTask->GetTaskItemCount(wi.m_ulItemTemplId);
				if (ulCount) pTask->TakeAwayTaskItem(wi.m_ulItemTemplId, ulCount);
			}
		}

		// 成功后把钱收走
		if (m_ulGoldWanted && !bClearTask && bSuccess)
		{
			unsigned long ulGold = pTask->GetGoldNum();
			if (ulGold > m_ulGoldWanted) ulGold = m_ulGoldWanted;
			pTask->TakeAwayGold(ulGold);
		}
		
		//成功后把帮派金券收走
		if(m_iFactionGoldNoteWanted && !bClearTask && bSuccess)
		{
			int iFactionGoldNote = pTask->GetFactionGoldNote();
			if(iFactionGoldNote > m_iFactionGoldNoteWanted)
				iFactionGoldNote = m_iFactionGoldNoteWanted;
			pTask->TakeAwayFactionGoldNote(iFactionGoldNote);
		}

	}
	else if (m_enumMethod == enumTMTitle)
	{
		if (bClearTask || !bSuccess)
			return;

		for (unsigned long i = 0; i < m_ulTitleWantedNum; i++)
			pTask->TakeAwayTitle(m_TitleWanted[i]);
	}
	else if(m_enumMethod == enumTMFactionProperty)
	{
		if (bClearTask || !bSuccess)
			return;
		
		int iFactionProperty;
		
		iFactionProperty = pTask->GetFactionGrass();
		if(iFactionProperty > m_iFactionGrassWanted)
			iFactionProperty = m_iFactionGoldNoteWanted;
		pTask->TakeAwayFactionGrass(iFactionProperty);

		iFactionProperty = pTask->GetFactionMine();
		if(iFactionProperty > m_iFactionMineWanted)
			iFactionProperty = m_iFactionMineWanted;
		pTask->TakeAwayFactionMine(iFactionProperty);

		iFactionProperty = pTask->GetFactionMonsterCore();
		if(iFactionProperty > m_iFactionMonsterCoreWanted)
			iFactionProperty = m_iFactionMonsterCoreWanted;
		pTask->TakeAwayFactionMonsterCore(iFactionProperty);

		iFactionProperty = pTask->GetFactionMonsterFood();
		if(iFactionProperty > m_iFactionMonsterFoodWanted)
			iFactionProperty = m_iFactionMonsterFoodWanted;
		pTask->TakeAwayFactionMosnterFood(iFactionProperty);

		iFactionProperty = pTask->GetFactionMoney();
		if(iFactionProperty > m_iFactionMoneyWanted)
			iFactionProperty = m_iFactionMoneyWanted;
		pTask->TakeAwayFactionMoney(iFactionProperty);
	}
	else if (m_enumMethod == enumTMKillNumMonster)
	{
		for (unsigned long i = 0; i < m_ulMonsterWanted; i++)
		{
			const MONSTER_WANTED& mw = m_MonsterWanted[i];
			if (!mw.m_ulDropItemId) continue;

			unsigned long ulCount;

			if (mw.m_bDropCmnItem)
			{
				ulCount = pTask->GetCommonItemCount(mw.m_ulDropItemId);
				if (mw.m_ulDropItemCount && ulCount > mw.m_ulDropItemCount) ulCount = mw.m_ulDropItemCount;
				if (ulCount) pTask->TakeAwayCommonItem(mw.m_ulDropItemId, ulCount);
			}
			else
			{
				ulCount = pTask->GetTaskItemCount(mw.m_ulDropItemId);
				if (ulCount) pTask->TakeAwayTaskItem(mw.m_ulDropItemId, ulCount);
			}
		}
	}
}

inline void ATaskTempl::CheckMask()
{
	if (m_enumMethod == enumTMKillNumMonster)
		m_ulMask = TASK_MASK_KILL_MONSTER;
	else if (m_enumMethod == enumTMCollectNumArticle)
		m_ulMask = TASK_MASK_COLLECT_ITEM;
	else if (m_enumMethod == enumTMTalkToNPC)
		m_ulMask = TASK_MASK_TALK_TO_NPC;
	else if(m_enumMethod == enumTMInterReachSite)
		m_ulMask = TASK_MASK_INTER_REACH_SITE;
	else if(m_enumMethod == enumTMInterLeaveSite)
		m_ulMask = TASK_MASK_INTER_LEAVE_SITE;
	else if(m_enumMethod == enumTMCollectNumInterObj)
		m_ulMask = TASK_MASK_COLLECT_INTEROBJ;
}

inline bool ATaskTempl::CheckKillMonster(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulTemplId,
	unsigned long ulLev,
	bool bTeam,
	float fRand,
	bool bShare) const
{
	if (m_enumMethod != enumTMKillNumMonster)
		return false;

	const ATaskTempl* pTempl = pEntry->GetTempl();

	if (bTeam != ((pTempl->m_bTeamwork || pTempl->GetTopTask()->m_bTeamwork) && pTask->IsInTeam()))
		return false;

	//非组队情况下，如果是共享任务 Added 2012-04-09.
	if ( bShare != (pTempl->m_bShareWork || pTempl->GetTopTask()->m_bShareWork) )
	{
		return false;
	}

	unsigned long i;
	if(m_bKillMonsterFail)
	{
		for(i=0; i < m_ulKillFailMonster; i++)
		{
			if(m_KillFailMonsters[i] == ulTemplId)
			{
				pEntry->ClearSuccess();
				OnSetFinished(pTask, pList, pEntry);
			}
		}
	}

	bool bRet = false;
	unsigned long ulPlayerLev = pTask->GetPlayerLevel();

	for (i = 0; i < m_ulMonsterWanted; i++)
	{
		const MONSTER_WANTED& mw = m_MonsterWanted[i];

		if (mw.m_ulMonsterTemplId && mw.m_ulMonsterTemplId != ulTemplId)
			continue;

		if (mw.m_bKillerLev && ulPlayerLev > ulLev + MONSTER_PLAYER_LEVEL_MAX_DIFF)
			continue;

		if (mw.m_ulDropItemId)
		{
			if (!pTask->IsDeliverLegal())
				continue;

			unsigned long ulCount = _get_item_count(pTask, mw.m_ulDropItemId, mw.m_bDropCmnItem);

			if (mw.m_ulDropItemCount && ulCount >= mw.m_ulDropItemCount)
			{
				if (!pEntry->IsFinished() && HasAllMonsterWanted(pTask, pEntry))
				{
					OnSetFinished(pTask, pList, pEntry);
					return true;
				}

				continue;
			}

			bRet = true;

			if (mw.m_fDropProb < fRand)
			{
				fRand -= mw.m_fDropProb;
				continue;
			}

			if (mw.m_bDropCmnItem)
			{
				if (pTask->CanDeliverCommonItem(1))
					pTask->DeliverCommonItem(mw.m_ulDropItemId, 1, false, 0);
			}
			else
			{
				if (pTask->CanDeliverTaskItem(1))
					pTask->DeliverTaskItem(mw.m_ulDropItemId, 1);
			}

			if (HasAllMonsterWanted(pTask, pEntry))
				OnSetFinished(pTask, pList, pEntry);

			return true;
		}
		else
		{
			if (mw.m_ulMonsterNum && pEntry->m_wMonsterNum[i] >= mw.m_ulMonsterNum)
			{
				if (!pEntry->IsFinished() && HasAllMonsterWanted(pTask, pEntry))
				{
					OnSetFinished(pTask, pList, pEntry);
					return true;
				}

				continue;
			}

			if (m_bSharedByFamily)
			{
				if (pTask->IsInFamily())
				{
					TaskPairData tp(mw.m_ulMonsterTemplId, 1, 0);
					TaskFamilyAsyncData d(pTask->GetFamilyID(), enumTaskFamilyUpdateMonsterInfo, m_ID, enumTaskReasonUpdateMonsterInfo, m_ID, &tp, 1);
					pTask->QueryFamilyData(d.GetBuffer(), d.GetSize());
				}
			}
			else
			{
				pEntry->m_wMonsterNum[i]++;
				NotifyClient(pTask, pEntry, TASK_SVR_NOTIFY_MONSTER_KILLED, 0, i);
			}

			if (HasAllMonsterWanted(pTask, pEntry))
				OnSetFinished(pTask, pList, pEntry);

			return true;
		}
	}

	return bRet;
}

inline void ATaskTempl::CheckCollectItem(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bAtNPC,
	int nChoice) const
{
	if (m_pFirstChild || !HasAllItemsWanted(pTask, pEntry))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "CollectItem Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckFinishAchievement(
		TaskInterface* pTask, 
		ActiveTaskList* pList,
		ActiveTaskEntry* pEntry,
		bool bAtNPC, 
		int nChoice) const
{
	if (m_pFirstChild || !HasFinishAchievement(pTask))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "FinishAchievement Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckCollectTitle(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bAtNPC,
	int nChoice) const
{
	if (m_pFirstChild || !HasAllTitlesWanted(pTask))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "CollectTitle Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckMonsterKilled(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bAtNPC,
	int nChoice) const
{
	if (!pEntry->IsFinished() || m_pFirstChild || !HasAllMonsterWanted(pTask, pEntry))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
		DeliverAward(pTask, pList, pEntry, nChoice);
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "Kill Monster Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckMining(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry) const
{
	if (m_enumMethod != enumTMCollectNumArticle
	 ||	m_ulItemsWanted == 0
	 || !pTask->IsDeliverLegal())
		return;

	const ITEM_WANTED& iw = m_ItemsWanted[0];
	unsigned long ulNum = _get_item_count(pTask, iw.m_ulItemTemplId, iw.m_bCommonItem);

	if (iw.m_ulItemNum && ulNum >= iw.m_ulItemNum)
		return;

	bool bDeliver = false;

	if (iw.m_bCommonItem)
	{
		if (pTask->CanDeliverCommonItem(1))
		{
			pTask->DeliverCommonItem(iw.m_ulItemTemplId, 1, false, 0);
			bDeliver = true;
		}
	}
	else
	{
		if (pTask->CanDeliverTaskItem(1))
		{
			pTask->DeliverTaskItem(iw.m_ulItemTemplId, 1);
			bDeliver = true;
		}
	}

	if (bDeliver)
	{
		char log[1024];
		sprintf(log, "DeliverMineItem: Item id = %d", iw.m_ulItemTemplId);
		TaskInterface::WriteKeyLog(pTask->GetPlayerId(), m_ID, 1, log);
	}

	if (HasAllItemsWanted(pTask, pEntry)) OnSetFinished(pTask, pList, pEntry);
}

inline void ATaskTempl::CheckWaitTime(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurTime,
	bool bAtNPC,
	int nChoice) const
{
	if (m_pFirstChild || pEntry->m_ulTaskTime + m_ulWaitTime >= ulCurTime)
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "WaitTime Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckFixedTime(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	unsigned long ulCurTime,
	bool bAtNPC,
	int nChoice) const
{
	if (m_pFirstChild || IsBeforeFixedTime(ulCurTime))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "FixedTime Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckTotalTreasure(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bAtNPC,
	int nChoice) const
{
	if (m_pFirstChild || !CheckTotalTreasure(pTask))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "FinishTotalTreasure Must At NPC, Not Direct");
}

inline void ATaskTempl::CheckFactionProperty(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry, 
	bool bAtNPC,
	int nChoice) const
{
	if(m_pFirstChild || !HasAllFactionPropertyWanted(pTask))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "FinishTotalTreasure Must At NPC, Not Direct");
}


inline void ATaskTempl::CheckBuildToLevel(
	TaskInterface* pTask, 
	ActiveTaskList* pList, 
	ActiveTaskEntry* pEntry, 
	bool bAtNPC, 
	int nChoice) const
{
	if(m_pFirstChild || !HasBuildToLevel(pTask))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
	{
		pEntry->SetFinished();
		DeliverAward(pTask, pList, pEntry, nChoice);
	}
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "FinishTotalTreasure Must At NPC, Not Direct");
}

inline void TaskNotifyPlayer(
	TaskInterface* pTask,
	unsigned long ulPlayerId,
	unsigned long ulTaskId,
	unsigned char uReason,
	unsigned long ulParam = 0)
{
	task_player_notify notify;
	notify.reason = uReason;
	notify.task = static_cast<unsigned short>(ulTaskId);
	notify.param = ulParam;
	pTask->NotifyPlayer(ulPlayerId, &notify, sizeof(notify));
}

inline void ATaskTempl::GiveUpOneTask(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bForce) const
{
	if (m_pParent || !m_bCanGiveUp)
		return;
	
	// 非跨服任务，并且在跨服服务器上，并且有失败奖励，则不可放弃
	if(!IsCrossServerTask() && pTask->IsInCrossServer() && HasFailAward())
		return;

	if(m_iPhaseCnt)
		for(int i = 0; i < m_iPhaseCnt; i++)
			if(m_Phase[i].bTrigger)
				pTask->SetPhase(m_Phase[i].iPhaseID, m_Phase[i].bVisual);

	pEntry->SetGiveUp();
	pEntry->ClearSuccess();

	//放弃徒弟任务，师傅需求物品-1
	if (m_bPrenticeTask && pTask->IsInTeam())
	{		
		TaskNotifyPlayer(
			pTask,	
			pTask->GetTeamMemberId(0),
			m_ID,
			TASK_PLY_NOTIFY_P2M_TASK_FAIL);		
		
		OnSetFinished(pTask, pList, pEntry, false);
	}
	else
	{	
		OnSetFinished(pTask, pList, pEntry);
	}

	TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 1, "GiveUpTask");
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), m_ID, 1, "GiveUpTask");
}

inline void ATaskTempl::OnSetFinished(
	TaskInterface* pTask,
	ActiveTaskList* pList,
	ActiveTaskEntry* pEntry,
	bool bNotifyMem) const
{
	pEntry->SetFinished();
	
	// 非跨服任务，在跨服服务器上无法接、无法完成 —— 避免杀数量怪自动完成bug
	if(!IsCrossServerTask() && pTask->IsInCrossServer())
	{
		// 若放弃任务，则进行响应
		if(!pEntry->IsGiveUp())
			return;
	}

	// 跨服任务，且需要检查全局变量，则无法接，无法完成
	if(IsCrossServerTask() && (m_bPremNeedComp || m_bFinNeedComp || m_lChangeKeyArr.size() != 0 || m_Award_S->m_lChangeKeyArr.size() != 0))
		return;

	// 通知客户端已完成
	NotifyClient(
		pTask,
		pEntry,
		TASK_SVR_NOTIFY_FINISHED,
		0);

	if (m_enumFinishType == enumTFTDirect || !pEntry->IsSuccess())
		DeliverAward(pTask, pList, pEntry, -1, bNotifyMem);
}

inline void ATaskTempl::NotifyClient(
	TaskInterface* pTask,
	const ActiveTaskEntry* pEntry,
	unsigned char uReason,
	unsigned long ulCurTime,
	unsigned long ulParam) const
{
	char log[1024];
	unsigned char buf[512];
	task_notify_base* pNotify = reinterpret_cast<task_notify_base*>(buf);
	size_t sz;

	pNotify->reason = uReason;
	pNotify->task = static_cast<unsigned short>(m_ID);
	bool bWriteLog = true;

	switch (uReason)
	{
	case TASK_SVR_NOTIFY_MONSTER_KILLED:
		static_cast<svr_monster_killed*>(pNotify)->monster_id = m_MonsterWanted[ulParam].m_ulMonsterTemplId;
		static_cast<svr_monster_killed*>(pNotify)->monster_num = pEntry->m_wMonsterNum[ulParam];
		sz = sizeof(svr_monster_killed);
		bWriteLog = false;
		break;
	case TASK_SVR_NOTIFY_INTEROBJ_COLLECT:
		static_cast<svr_interobj_collected*>(pNotify)->interobj_id = m_InterObjWanted[ulParam].m_ulInterObjId;
		static_cast<svr_interobj_collected*>(pNotify)->interobj_num = pEntry->m_wInterObjNum[ulParam];
		sz = sizeof(svr_interobj_collected);
		bWriteLog = false;
		break;
	case TASK_SVR_NOTIFY_ITEMS_TO_GET:
		static_cast<svr_items_to_get*>(pNotify)->items_num = pEntry->m_wItemsToGet;
		sz = sizeof(svr_items_to_get);
		break;
	case TASK_SVR_NOTIFY_NEW:
		static_cast<svr_new_task*>(pNotify)->set_data(
			ulCurTime,
			reinterpret_cast<unsigned long>(pEntry),
			*(reinterpret_cast<const task_sub_tags*>(ulParam))
			);
		sz = static_cast<svr_new_task*>(pNotify)->get_size();
		break;
	case TASK_SVR_NOTIFY_COMPLETE:
		static_cast<svr_task_complete*>(pNotify)->set_data(
			ulCurTime,
			*(reinterpret_cast<const task_sub_tags*>(ulParam))
			);
		sz = static_cast<svr_task_complete*>(pNotify)->get_size();
		break;
	case TASK_SVR_NOTIFY_GIVE_UP:
	case TASK_SVR_NOTIFY_FINISHED:
	case TASK_SVR_NOTIFY_CLEAR_STATE:
		sz = sizeof(task_notify_base);
		break;
	case TASK_SVR_NOTIFY_ERROR_CODE:
		static_cast<svr_task_err_code*>(pNotify)->err_code = ulParam;
		sz = sizeof(svr_task_err_code);
		break;
	case TASK_SVR_NOTIFY_FINISH_COUNT:
		static_cast<task_finish_count*>(pNotify)->count = (unsigned short)ulParam;
		sz = sizeof(task_finish_count);
		break;
	case TASK_SVR_NOTIFY_CAMERA_MOVE:
		static_cast<task_camera_move*>(pNotify)->ulIndex = ulParam;
		sz = sizeof(task_camera_move);
		break;
	case TASK_SVR_NOTIFY_RM_FINISH_TASK://删除任务完成列表中的某一个任务 Added 2011-03-23.
		sz = sizeof(task_notify_base);
		break;
	case TASK_SVR_NOTIFY_ADD_FINISH_TASK://添加某一个任务到完成任务列表中 Added 2011-03-23.
		static_cast<task_notify_add_finish_task*>(pNotify)->bTaskSuccess = (ulParam == 1);
		sz = sizeof(task_notify_add_finish_task);
		break;
	default:
		sprintf(log, "NotifyClient, Unknown Reason = %d", uReason);
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, log);
		return;
	}

	assert(sz <= sizeof(buf));
	pTask->NotifyClient(buf, sz);
}

extern void TaskUpdateGlobalData(unsigned long ulTaskId, const unsigned char pData[TASK_GLOBAL_DATA_SIZE]);

inline void ATaskTempl::DeliverTeamMemTask(
	TaskInterface* pTask,
	TaskGlobalData* pGlobal,
	unsigned long ulCurTime) const
{
	const int nMemNum = pTask->GetTeamMemberNum();

	for (int i = 1; i < nMemNum; i++) // 跳过队长
	{
		TaskNotifyPlayer(
			pTask,
			pTask->GetTeamMemberId(i),
			m_ID,
			TASK_PLY_NOTIFY_NEW_MEM_TASK,
			true);
	}
}


inline void ATaskTempl::DeliverMasterPrenticeTask(TaskInterface* pTask) const
{
	const int nMemNum = pTask->GetTeamMemberNum();

	for (int i=1; i< nMemNum; i++)
	{
		TaskNotifyPlayer(
			pTask,
			pTask->GetTeamMemberId(i),
			m_ID,
			TASK_PLY_NOTIFY_M2P_NEW_TASK);
	}
}

inline bool ATaskTempl::CheckCollectInterObj(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulTemplId) const
{

	if (m_enumMethod != enumTMCollectNumInterObj)
		return false;

	const ATaskTempl* pTempl = pEntry->GetTempl();

	unsigned long i;

	for(i = 0; i < m_ulInterObjWanted; i++)
	{
		const INTEROBJ_WANTED& wi = m_InterObjWanted[i];

		if(wi.m_ulInterObjId && wi.m_ulInterObjId != ulTemplId)
			continue;

		if(wi.m_ulInterObjNum && pEntry->m_wInterObjNum[i] >= wi.m_ulInterObjNum)
		{
			if(!pEntry->IsFinished() && HasAllInterObjWanted(pTask, pEntry))
			{
				OnSetFinished(pTask, pList, pEntry);
				return true;
			}
			
			continue;
		}

		pEntry->m_wInterObjNum[i]++;
		NotifyClient(pTask, pEntry, TASK_SVR_NOTIFY_INTEROBJ_COLLECT, 0, i);

		if(HasAllInterObjWanted(pTask, pEntry))
			OnSetFinished(pTask, pList, pEntry);

		return true;
	}
	
	return false;
}

inline void ATaskTempl::CheckInterObjCollected(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const
{
	if(!pEntry->IsFinished() || m_pFirstChild || !HasAllInterObjWanted(pTask, pEntry))
		return;

	if (bAtNPC || m_enumFinishType == enumTFTDirect)
		DeliverAward(pTask, pList, pEntry, nChoice);
	else
		TaskInterface::WriteLog(pTask->GetPlayerId(), m_ID, 0, "Collect InterObj Must At NPC, Not Direct");
}

#endif

#endif
