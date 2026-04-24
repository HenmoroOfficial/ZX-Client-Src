/********************************************************************
	created:	2010/01/7
	author:		lihengju
	
	purpose:	use lua to config trigger task
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "vector.h"
#include <AWString.h>
#include "hashmap.h"
#include "EC_Secretary.h"

class CLuaState;


class CLuaTriggerTask  
{
public:
	struct TriggerTaskInfo 
	{
		TriggerTaskInfo();
		
		int eventid;
		int taskid;
		int arg;
		TriggerTaskInfo* next;
	};

	typedef abase::hash_map<unsigned short,int> Task2IndexMap;
	
	CLuaTriggerTask();
	virtual ~CLuaTriggerTask();

	bool Init();
	void Release();

	TriggerTaskInfo *m_TriggerTaskList[CECSecretary::NUM_FIRST_STATES];

	TriggerTaskInfo *m_CompleteTaskList[CECSecretary::NUM_FIRST_STATES];

	void GetTriggerTaskID(int eventid, int arg, abase::vector<int>& vecTask);
	void GetCompleteTaskID(int eventid, int arg, abase::vector<int>& vecTask);
	bool ProcessTriggerTask(int eventid, int arg);

private:
	CLuaState * m_pState;

	Task2IndexMap m_TriggerOpenTaskIndex;
	Task2IndexMap m_TriggerCompleteTaskIndex;

	bool GetTriggerTaskList();
	bool GetCompleteTaskList();

	// 获取任务对应的序号，以掩码的形式保存
	bool GetOpenTask2IndexMap();
	bool GetCompleteTask2IndexMap();

	int GetTriggerOpenTaskIndex(int iTask);
	int GetTriggerCompleteTaskIndex(int iTask);
};