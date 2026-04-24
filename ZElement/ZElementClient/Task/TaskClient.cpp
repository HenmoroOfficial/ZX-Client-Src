#include "TaskInterface.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"

#ifdef _ELEMENTCLIENT
	#include "LuaEvent.h"
#endif

#ifdef _TASK_CLIENT

#define FINISH_DLG_SHOWN_TIME 60

void OnTaskCheckStatus(TaskInterface* pTask)
{
	if (!pTask->CheckVersion())
		return;

	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	ActiveTaskEntry* aEntries = pLst->m_TaskEntries;
	unsigned long ulCurTime = pTask->GetCurTime();
	unsigned char i;

	for (i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = aEntries[i];

		if (!CurEntry.m_ulTemplAddr)
			continue;

		const ATaskTempl* pTempl = CurEntry.GetTempl();
		
		// 若为普通任务（非跨服任务），且当前为跨服，则不可接、不可完成
		if(!pTempl->IsCrossServerTask() && pTask->IsInCrossServer())
			continue;

		if (!pTempl->IsValidState())
			continue;

		// 如果任务失败，则立刻完成
		if (!CurEntry.IsSuccess())
		{
			if (pTempl->RecursiveCheckAward(pTask, pLst, &CurEntry, ulCurTime, -1) == 0)
			{
				pTempl->IncValidCount();
				_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
			}

			continue;
		}

		// 超时判断
		if (pTempl->m_ulTimeLimit
		&& CurEntry.m_ulTaskTime + pTempl->m_ulTimeLimit < ulCurTime) // 超时
		{
			pTempl->IncValidCount();
			_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
			continue;
		}

		// 超过任务失效日期
		if (pTempl->m_bAbsFail)
		{
			long ulCurTime1 = ulCurTime;
			ulCurTime1 -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);

			if((long)ulCurTime1 < 0)
				ulCurTime1 = 0;
			tm cur = *gmtime((const time_t*)&ulCurTime1);			
			
			if(pTempl->m_tmAbsFailTime.before(&cur))
			{
 				pTempl->IncValidCount();
 				_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
 				continue;
			}
		}

		// 身上有或者没有指定物品则失败
		if(pTempl->m_bHaveItemFail && pTempl->HasFailItem(pTask) ||
			pTempl->m_bNotHaveItemFail && pTempl->NotHasFailItem(pTask))
		{
 			pTempl->IncValidCount();
 			_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
 			continue;
		}
		
		// 完成方式为特定时间完成
		if (pTempl->m_enumMethod == enumTMFixedTime && !pTempl->IsBeforeFixedTime(ulCurTime))
		{
			pTempl->IncValidCount();
			_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
			continue;
		}

		//物品数减为0
		if (pTempl->m_bTeamwork && pTempl->m_bMPTask && CurEntry.m_wItemsToGet == 0 && pTask->IsMaster())
		{
			pTempl->IncValidCount();
			_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
			continue;
		}
		
		//超出特定区域即失败
		if (pTempl->m_bOutZoneFail) 
		{
			float pos[3];
			unsigned long ulWorldID = pTask->GetPos(pos);
			
			if (ulWorldID != pTempl->m_ulOutZoneWorldID || !is_in_zone(pTempl->m_OutZoneMinVert, pTempl->m_OutZoneMaxVert, pos))
			{
				pTempl->IncValidCount();
				_notify_svr(pTask, TASK_CLT_NOTIFY_OUT_ZONE, (unsigned short)pTempl->GetID());
				continue;
			}				
		}
		
		//进入特定区域即失败
		if (pTempl->m_bEnterZoneFail) 
		{
			float pos[3];
			unsigned long ulWorldID = pTask->GetPos(pos);
			
			if (ulWorldID == pTempl->m_ulEnterZoneWorldID && is_in_zone(pTempl->m_EnterZoneMinVert, pTempl->m_EnterZoneMaxVert, pos))
			{
				pTempl->IncValidCount();
				_notify_svr(pTask, TASK_CLT_NOTIFY_ENTER_ZONE, (unsigned short)pTempl->GetID());
				continue;
			}				
		}

		if (pTempl->m_enumMethod == enumTMTalkToNPC || pTempl->m_bMarriage)
			continue;

		// 判断未完成的任务可否完成
		if (!CurEntry.IsFinished())
		{
			if (pTempl->m_enumMethod == enumTMReachSite) // 到达地点判断
			{
				if (ulCurTime - TaskInterface::GetFinishDlgShowTime() < FINISH_DLG_SHOWN_TIME && pTempl->m_bDisFinDlg)
					continue;

				float pos[3];
				unsigned long ulWorldId = pTask->GetPos(pos);

				if (ulWorldId == pTempl->m_ulReachSiteId && is_in_zone(pTempl->m_ReachSiteMin, pTempl->m_ReachSiteMax, pos))
				{
					const talk_proc* pTalk = &pTempl->m_AwardTalk;
					if (pTalk->num_window == 0 || pTalk->num_window == 1 && pTalk->windows[0].num_option == 0)
					{
						pTempl->IncValidCount();
						_notify_svr(pTask, TASK_CLT_NOTIFY_REACH_SITE, (unsigned short)pTempl->GetID());
					}
					else
					{
						pTask->PopupTaskFinishDialog(pTempl->m_ID, const_cast<talk_proc*>(&pTempl->m_AwardTalk));
						TaskInterface::SetFinishDlgShowTime(ulCurTime);
					}
				}

				continue;
			}
			else if (pTempl->m_enumMethod == enumTMLeaveSite) // 离开地点判断
			{
				if (ulCurTime - TaskInterface::GetFinishDlgShowTime() < FINISH_DLG_SHOWN_TIME)
					continue;

				float pos[3];
				unsigned long ulWorldId = pTask->GetPos(pos);

				if (ulWorldId != pTempl->m_ulLeaveSiteId || !is_in_zone(pTempl->m_LeaveSiteMin, pTempl->m_LeaveSiteMax, pos))
				{
					const talk_proc* pTalk = &pTempl->m_AwardTalk;
					if (pTalk->num_window == 0 || pTalk->num_window == 1 && pTalk->windows[0].num_option == 0)
					{
						pTempl->IncValidCount();
						_notify_svr(pTask, TASK_CLT_NOTIFY_LEAVE_SITE, (unsigned short)pTempl->GetID());
					}
					else
					{
						pTask->PopupTaskFinishDialog(pTempl->m_ID, const_cast<talk_proc*>(&pTempl->m_AwardTalk));
						TaskInterface::SetFinishDlgShowTime(ulCurTime);
					}
				}

				continue;
			}
		}

		if (pTempl->m_enumFinishType == enumTFTDirect
		 && !CurEntry.HasChildren()
		 && pTempl->CanFinishTask(pTask, &CurEntry, ulCurTime)
		 && pTempl->RecursiveCheckAward(pTask, pLst, &CurEntry, ulCurTime, -1) == 0
		 && pTempl->m_enumMethod != enumTMNPCAction)
		{
			if (pTempl->m_bDisFinDlg)
			{
				if (ulCurTime - TaskInterface::GetFinishDlgShowTime() < FINISH_DLG_SHOWN_TIME)
					continue;

				const talk_proc* pTalk = &pTempl->m_AwardTalk;
				if (pTalk->num_window == 0 || pTalk->num_window == 1 && pTalk->windows[0].num_option == 0)
				{
					pTempl->IncValidCount();
					_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, (unsigned short)pTempl->GetID());
				}
				else
				{
					pTask->PopupTaskFinishDialog(pTempl->m_ID, const_cast<talk_proc*>(&pTempl->m_AwardTalk));
					TaskInterface::SetFinishDlgShowTime(ulCurTime);
				}

			}
			else
			{
				pTempl->IncValidCount();
				_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH, CurEntry.m_ID);
			}			
		}
	}

	GetTaskTemplMan()->UpdateStatus(pTask);
}

void OnServerNotify(TaskInterface* pTask, const void* pBuf, size_t sz)
{
	if (!pTask)
		return;

	if (!pTask->CheckVersion())
		return;

	if (sz < sizeof(task_notify_base)) return;
	const task_notify_base* pNotify = static_cast<const task_notify_base*>(pBuf);

	const ATaskTempl* pTempl = NULL;
	ActiveTaskEntry* pEntry = NULL;

	if (pNotify->reason == TASK_SVR_NOTIFY_ERROR_CODE)
	{
		if (sz != sizeof(svr_task_err_code)) return;

#ifdef _ELEMENTCLIENT
		pEntry = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList())->GetEntry(pNotify->task);
		if (pEntry) pEntry->SetErrReported();
		TaskShowErrMessage(static_cast<const svr_task_err_code*>(pNotify)->err_code);
		if(static_cast<const svr_task_err_code*>(pNotify)->err_code == 5)
			LuaEvent::FirstTaskFull();
#endif

		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_FORGET_SKILL)
	{
		GetTaskTemplMan()->OnForgetLivingSkill(pTask);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_NEW)
	{
		//明确GetTaskTemplByID和GetTopTaskByID接口的区别，前者是所有任务模板，后者是主任务模板
		pTempl = GetTaskTemplMan()->GetTaskTemplByID(pNotify->task); // Modified 2011-04-28 From Function GetTopTaskByID

#ifdef _ELEMENTCLIENT
//		if (pTempl->m_bScriptFinishTask)
//		LuaEvent::AcceptScriptCompleteTask(pNotify->task);
		//在调用之前必须先判断该任务模板是否存在 Added 2011-04-28
		if (!pTempl)
		{
			assert(false);
			return;
		}
		//Added end.

		if(pTempl->IsKeyTask())
			LuaEvent::FirstAcceptMainTask();
		else
		{
			switch(pTempl->GetType())
			{
				case 0:
					LuaEvent::FirstAcceptTravelTask();
					break;
				case 1:
					LuaEvent::FirstAcceptChanllengeTask();
					break;
				case 2:
					LuaEvent::FirstAcceptKillMonsterTask();
					break;
				case 3:
					LuaEvent::FirstAcceptForgeTask();
					break;
				case 4:
					LuaEvent::FirstAcceptRiskTask();
					break;
				case 5:
					LuaEvent::FirstAcceptHeritTask();
					break;
				case 6:
					LuaEvent::FirstAcceptClassicalTask();
					break;
				case 7:
					LuaEvent::FirstAcceptAsceticTask();
					break;
				case 8:
					LuaEvent::FirstAcceptLupinTask();
					break;
				case 9:
					LuaEvent::FirstAcceptTeacherTask();
					break;
				case 10:
					LuaEvent::FirstAcceptGuildTask();
					break;
				case 11:
					LuaEvent::FirstAcceptTeasureTask();
					break;
				case 12:
					LuaEvent::FirstAcceptAdventureTask();
					break;
				case 13:
					LuaEvent::FirstAcceptLoveTask();
					break;
				default:
					break;
			}
		}
#endif

	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_DYN_TIME_MARK)
	{
		if (sz != sizeof(svr_task_dyn_time_mark)) return;
		GetTaskTemplMan()->OnDynTasksTimeMark(
			pTask,
			static_cast<const svr_task_dyn_time_mark*>(pNotify)->time_mark,
			static_cast<const svr_task_dyn_time_mark*>(pNotify)->version);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_DYN_DATA)
	{
		if (sz <= sizeof(task_notify_base)) return;
		GetTaskTemplMan()->OnDynTasksData(
			pTask,
			static_cast<const char*>(pBuf) + sizeof(task_notify_base),
			sz - sizeof(task_notify_base),
			pNotify->task != 0);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_STORAGE)
	{
		if (sz != sizeof(task_notify_base) + TASK_STORAGE_BUF_SIZE) return;
		GetTaskTemplMan()->OnStorageData(
			pTask,
			static_cast<const char*>(pBuf) + sizeof(task_notify_base)
			);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_SPECIAL_AWARD)
	{
		if (sz != sizeof(svr_task_special_award)) return;
		GetTaskTemplMan()->OnSpecialAward(&static_cast<const svr_task_special_award*>(pNotify)->sa);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_FINISH_COUNT)
	{
		if (sz != sizeof(task_finish_count))
			return;

		task_finish_count* p = (task_finish_count*)pNotify;
		TaskFinishTimeList* pList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();
		TaskFinishTimeEntry* pEntry = pList->Search(p->task);

		if (pEntry)
			pEntry->m_uFinishCount = p->count;
		else
			pList->AddFinishCount(p->task, p->count);

		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_CAMERA_MOVE)
	{
		if (sz != sizeof(task_camera_move))
			return;
		
		pTask->CameraMove(static_cast<const task_camera_move *>(pNotify)->ulIndex);
		return;
	}
	else if (pNotify->reason == TASK_SVR_NOTIFY_CLEAR_LIFEAGAIN_FIN_REC) 
	{
		GetTaskTemplMan()->RemoveLifeAgainTaskInList(pTask);
		
		return;
	}
	else if ( pNotify->reason == TASK_SVR_NOTIFY_RM_FINISH_TASK )//暂时仅仅用于调试命令Added 2011-03-23
	{
		//首先获得当前的完成任务列表，并根据参数值进行相应的操作（移出或添加）
		FinishedTaskList* pFinishList = static_cast< FinishedTaskList* >(pTask->GetFinishedTaskList());

		//执行删除操作
		pFinishList->RemoveTask( pNotify->task );
		return;
	}
	else if ( pNotify->reason == TASK_SVR_NOTIFY_ADD_FINISH_TASK )//暂时仅仅用于调试命令Added 2011-03-23
	{
		//首先获得当前的完成任务列表，并根据参数值进行相应的操作（移出或添加）
		FinishedTaskList* pFinishList = static_cast< FinishedTaskList* >(pTask->GetFinishedTaskList());
		
		//执行添加操作
		pFinishList->AddOneTask( pNotify->task, static_cast<const task_notify_add_finish_task*>(pNotify)->bTaskSuccess );
		return;
	}
	else if ( pNotify->reason == TASK_SVR_NOTIFY_CLEAR_STATE )//暂时仅仅用于调试命令Added 2013-02-21
	{
		ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
		ActiveTaskEntry* aEntries = pLst->m_TaskEntries;
		unsigned char i;

		for (i = 0; i < pLst->m_uTaskCount; i++)
		{
			ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];

			if (CurEntry.m_ID != pNotify->task
			|| !CurEntry.m_ulTemplAddr)
				continue;

			// clear state
			CurEntry.SetSuccess();

			break;
		}
		return;
	}	
	else
	{
		ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
		ActiveTaskEntry* aEntries = pLst->m_TaskEntries;
		unsigned char i;

		for (i = 0; i < pLst->m_uTaskCount; i++)
		{
			ActiveTaskEntry& CurEntry = pLst->m_TaskEntries[i];

			if (CurEntry.m_ID != pNotify->task
			|| !CurEntry.m_ulTemplAddr)
				continue;

			pTempl = CurEntry.GetTempl();
			pEntry = &CurEntry;
			break;
		}
	}

	if (!pTempl)
	{
		assert(false);
		return;
	}

	pTempl->ClearValidCount();
	pTempl->OnServerNotify(
		pTask,
		pEntry,
		pNotify,
		sz
		);
}

#endif
