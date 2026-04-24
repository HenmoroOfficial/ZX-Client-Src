/********************************************************************
	created:	2010/01/7
	author:		lihengju
	
	purpose:	use lua to config trigger task
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#include <AString.h>
#include <ABaseDef.h>
#include "LuaTriggerTask.h"
#include "ScriptValue.h"
#include "LuaState.h"
#include "LuaScript.h"
#include "LuaEvent.h"
#include "EC_HostPlayer.h"
#include "TaskTemplMan.h"
#include "EC_TaskInterface.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_Secretary.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define	 LUA_TRIGGER_TASK_TBL	"trigger_task"
#define  LUA_COMPLETE_TASK_TBL  "complete_task"
#define  LUA_OPEN_TASK_INDEX_TBL "triggerTaskIndex"
#define  LUA_COMPLETE_TASK_INDEX_TBL "completeTaskIndex"
#define  LUA_TRIGGER_TASK_FILE  "Interfaces\\script\\triggerTask.lua"
using namespace LuaBind;

CLuaTriggerTask::CLuaTriggerTask()
{
	m_pState = g_LuaStateMan.GetConfigState(); 
	int i;
	for (i =0; i< CECSecretary::NUM_FIRST_STATES; ++i)
	{
		m_TriggerTaskList[i] = NULL;
		m_CompleteTaskList[i] = NULL;
	}
}

CLuaTriggerTask::~CLuaTriggerTask()
{
	Release();
}

CLuaTriggerTask::TriggerTaskInfo::TriggerTaskInfo()
{
	eventid = 0;
	arg   = 0;
	taskid = 0;
	next = NULL;
}

bool CLuaTriggerTask::Init()
{
	assert(m_pState);

	m_pState->Lock();

	CLuaScript * pScript = m_pState->RegisterFile(LUA_TRIGGER_TASK_FILE);

	if (!pScript)
	{
		m_pState->Unlock();
		return false;
	}

	GetTriggerTaskList();
	GetCompleteTaskList();
	
	GetOpenTask2IndexMap();
	GetCompleteTask2IndexMap();

	m_pState->Unlock();
	return  true;	
}

void CLuaTriggerTask::Release()
{
	for(int i = 0; i< CECSecretary::NUM_FIRST_STATES; ++i)
	{
		TriggerTaskInfo* pList;
		TriggerTaskInfo* pCur = m_TriggerTaskList[i];
		while(pCur)
		{
			pList = pCur;
			pCur= pList->next;
			delete pList;
		}
		pCur = m_CompleteTaskList[i];
		while(pCur)
		{
			pList = pCur;
			pCur = pList->next;
			delete pList;
		}
	}

	abase::vector<AString> rmTbls;
	rmTbls.push_back(LUA_TRIGGER_TASK_TBL);
	rmTbls.push_back(LUA_COMPLETE_TASK_TBL);
	rmTbls.push_back(LUA_OPEN_TASK_INDEX_TBL);
	rmTbls.push_back(LUA_COMPLETE_TASK_INDEX_TBL);

	m_pState->Lock();
	m_pState->UnRegister(LUA_TRIGGER_TASK_FILE, &rmTbls);
	m_pState->Unlock();
}


bool CLuaTriggerTask::GetTriggerTaskList()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(LUA_TRIGGER_TASK_TBL, "GetTriggerTask", args, results))
	{
		return false;
	}
	 
	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;

		TriggerTaskInfo* pInfo = new TriggerTaskInfo;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				delete pInfo;
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "eventid") == 0)
			{
				pInfo->eventid = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "eventarg") == 0)
			{
				pInfo->arg =	ret0iv[j].GetInt();	
			}
			else if (strcmp(key, "taskid") == 0)
			{
				pInfo->taskid = ret0iv[j].GetInt();
			}
			else
			{
				delete pInfo;
				return false;				
			}
		}

		TriggerTaskInfo* pEvent2Task = m_TriggerTaskList[pInfo->eventid - LEVT_FIRST_ENTER_GAME];
		if (!pEvent2Task)
			m_TriggerTaskList[pInfo->eventid - LEVT_FIRST_ENTER_GAME] = pInfo;
		else
		{
			while(pEvent2Task->next)
			{
				pEvent2Task = pEvent2Task->next;
			}
			pEvent2Task->next = pInfo;
		}
	}

	return true;
}

bool CLuaTriggerTask::GetCompleteTaskList()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(LUA_COMPLETE_TASK_TBL, "GetCompleteTask", args, results))
	{
		return false;
	}
	 
	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;

		TriggerTaskInfo* pInfo = new TriggerTaskInfo; 

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				delete pInfo;
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "eventid") == 0)
			{
				pInfo->eventid = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "eventarg") == 0)
			{
				pInfo->arg =	ret0iv[j].GetInt();	
			}
			else if (strcmp(key, "taskid") == 0)
			{
				pInfo->taskid = ret0iv[j].GetInt();
			}
			else
			{
				delete pInfo;
				return false;
			}
		}
		TriggerTaskInfo* pEvent2Task = m_CompleteTaskList[pInfo->eventid -  LEVT_FIRST_ENTER_GAME];
		
		if(!pEvent2Task)
			m_CompleteTaskList[pInfo->eventid -  LEVT_FIRST_ENTER_GAME] = pInfo;
		else
		{
			while(pEvent2Task->next)
			{
				pEvent2Task = pEvent2Task->next;
			}
			pEvent2Task->next = pInfo;
		}
	}
	return true;
}

bool CLuaTriggerTask::GetOpenTask2IndexMap()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(LUA_OPEN_TASK_INDEX_TBL, "GetTriggerTaskIndex", args, results))
	{
		return false;
	}
	 
	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;

		int taskid, index;

		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "taskid") == 0)
			{
				taskid = ret0iv[j].GetInt();
			}
			else if(strcmp(key, "index") == 0)
			{
				index = ret0iv[j].GetInt();
			}
			else
			{
				return false;
			}
		}
		m_TriggerOpenTaskIndex[taskid] = index;
	}
	return true;
}

bool CLuaTriggerTask::GetCompleteTask2IndexMap()
{
	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	
	if (!m_pState->Call(LUA_COMPLETE_TASK_INDEX_TBL, "GetCompleteTaskIndex", args, results))
	{
		return false;
	}
	 
	CScriptValue& ret0 = results[0];

	unsigned int i, j;
	for (i = 0; i < ret0.m_ArrVal.size(); i++)
	{
		abase::vector<CScriptValue>& ret0ik = ret0.m_ArrVal[i].m_ArrKey;
		abase::vector<CScriptValue>& ret0iv = ret0.m_ArrVal[i].m_ArrVal;
		int taskid, index;
		for (j = 0; j < ret0ik.size(); j++)
		{
			CScriptValue& ret0ijk = ret0ik[j];
			if (ret0ijk.m_Type != CScriptValue::SVT_STRING)
			{
				return false;
			}
			AString key; 
			ret0ijk.RetrieveAString(key);
			if (strcmp(key, "taskid") == 0)
			{
				taskid = ret0iv[j].GetInt();
			}
			else if (strcmp(key, "index") == 0)
			{
				index =	ret0iv[j].GetInt();	
			}
			else
			{
				return false;
			}
		}
		m_TriggerCompleteTaskIndex[taskid] = index;
	}
	return true;
}

void CLuaTriggerTask::GetTriggerTaskID(int eventid, int arg, abase::vector<int>& vecTask)
{
	TriggerTaskInfo* pList;
	pList = m_TriggerTaskList[eventid - LEVT_FIRST_ENTER_GAME];
	while(pList)
	{
		if(pList->arg == arg )
			vecTask.push_back(pList->taskid);
		pList = pList->next;
	}
}

void CLuaTriggerTask::GetCompleteTaskID(int eventid, int arg, abase::vector<int>& vecTask)
{
	TriggerTaskInfo* pList;
	pList = m_CompleteTaskList[eventid - LEVT_FIRST_ENTER_GAME];
	while(pList)
	{
		if (pList->arg == arg)
			vecTask.push_back(pList->taskid);
		pList = pList->next;
	}
}

int CLuaTriggerTask::GetTriggerOpenTaskIndex(int iTask)
{
	Task2IndexMap::iterator it = m_TriggerOpenTaskIndex.find(iTask);
	if (it != m_TriggerOpenTaskIndex.end())
		return it->second;
	return 0;
}

int CLuaTriggerTask::GetTriggerCompleteTaskIndex(int iTask)
{
	Task2IndexMap::iterator it = m_TriggerCompleteTaskIndex.find(iTask);
	if (it != m_TriggerCompleteTaskIndex.end())
		return it->second;
	return 0;
}

bool CLuaTriggerTask::ProcessTriggerTask(int eventid, int arg)
{
	abase::vector<int> vecTask;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECTaskInterface* pTask = pHost->GetTaskInterface();
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	CECSecretary *pSecretary = g_pGame->GetGameRun()->GetSecretary();
	int i;
	int index;
	bool bCheck;
	GetTriggerTaskID(eventid, arg, vecTask);
	for(i = 0; i<(int)vecTask.size(); ++i)
	{
		ATaskTempl* pTemp = pMan->GetTaskTemplByID(vecTask[i]);
		index = GetTriggerOpenTaskIndex(vecTask[i]);
		// 检查任务掩码
		bCheck = index==0? true:pSecretary->CheckOpenTaskState(index);

		if(pTemp && pTemp->m_bScriptOpenTask && bCheck && pTask->CanDeliverTask(vecTask[i]) == 0)
		{
			_notify_svr(pTask, TASK_CLT_NOTIFY_AUTO_DELV, vecTask[i]);
			if(index != 0)	// 任务掩码置位
				pSecretary->SetOpenTaskState(index);
		}
	}

	vecTask.clear();
	GetCompleteTaskID(eventid, arg, vecTask);
	for(i =0; i<(int)vecTask.size(); ++i)
	{
		index = GetTriggerCompleteTaskIndex(vecTask[i]);
		bCheck = index == 0? true:pSecretary->CheckCompleteTaskState(index);

		if ( pHost->IsScriptFinishTask(vecTask[i]) && bCheck )
		{
			_notify_svr(pTask, TASK_CLT_NOTIFY_CHECK_FINISH,  vecTask[i]);
			pHost->EraseScriptFinishTask(vecTask[i]);

			if (index != 0)
				pSecretary->SetCompleteTaskState(index);
		}
	}
	return true;
}