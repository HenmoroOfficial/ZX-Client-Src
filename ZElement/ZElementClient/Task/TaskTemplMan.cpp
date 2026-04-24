#include "TaskTemplMan.h"
#include "TaskInterface.h"

#ifdef LINUX
	#include "../template/elementdataman.h"
#else
	#include "elementdataman.h"
#endif

#ifdef _ELEMENTCLIENT
	#include "../EC_StringTab.h"
	#include "../EC_GameUIMan.h"
	CECStringTab _task_err;
#endif

#ifndef _TASK_CLIENT
	#include "TaskServer.h"
	#define TASK_SVR_MAGIC	0x9A45
#else
	#include <vector>
	#include <algorithm>
#endif

#if DEBUG_LOG == 1
	const char* _log_file = "task_log.txt";
#endif

#ifdef WIN32
	#include <io.h>
//#else
//	#include <dirent.h>
#endif

/*
 *	structs, definition
 */

#define DYN_TASK_CUR_VERSION	13

#define DYN_TASK_VERIFY_SVR		1404
#define DYN_TASK_DELIVER_SVR	1403


#define TASK_NPC_INFO_VERSION	2
#define TASK_PER_PACK_COUNT		300

struct DYN_TASK_PACK_HEADER
{
	unsigned long	pack_size;
	long			time_mark;
	unsigned short	version;
	unsigned short	task_count;
};

struct TASK_NPC_PACK_HEADER
{
	unsigned long	pack_size;
	long			time_mark;
	unsigned short	version;
	unsigned short	npc_count;
};

#ifndef _ELEMENTCLIENT
	ATaskTemplMan _task_templ_man;
	ATaskTemplMan* GetTaskTemplMan() { return &_task_templ_man; }
#endif

extern unsigned long _task_templ_cur_version;

ATaskTemplMan::ATaskTemplMan() :
m_pEleDataMan(0),
m_ulDynTasksTimeMark(0),
m_pDynTasksData(0),
m_ulDynTasksDataSize(0),
m_pNPCInfoData(0),
m_ulNPCInfoDataSize(0),
m_bEnableLog(true),
m_nExportVersion(0)
{
#ifdef _TASK_CLIENT
	memset(m_szDynPackPath, 0, sizeof(m_szDynPackPath));
	m_bDynTasksVerified = false;
	ClearSpecailAward();
#endif
}

ATaskTemplMan::~ATaskTemplMan()
{
}

#ifdef _TASK_CLIENT

void ATaskTemplMan::CheckAutoDelv(TaskInterface* pTask)
{
	const ATaskTempl* pTempl;
	TaskTemplMap::iterator it = m_AutoDelvMap.begin();
	unsigned long ulCurTime = TaskInterface::GetCurTime();
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();

	for (; it != m_AutoDelvMap.end(); ++it)
	{
		pTempl = it->second;

		if (!pTempl->IsValidState())
			continue;	
		
		if (pTempl->m_bScriptOpenTask)	// 脚本开启的任务，由客户端逻辑来触发
			continue;

		if (pTempl->CheckPrerequisite(pTask, pLst, ulCurTime) == 0)
		{
			if(!pTempl->m_bDeliverWindowMode)
			{
				pTempl->IncValidCount();
				_notify_svr(pTask, TASK_CLT_NOTIFY_AUTO_DELV, static_cast<unsigned short>(pTempl->m_ID));
			}
			else	// 自动发放任务窗口模式，弹出选择框
			{
#ifndef TASK_TEMPL_EDITOR
				TaskInterface::PopupTaskCheckInfoDialog(pTempl->m_ID);
#endif
			}
		}
		else if(pTempl->m_bDeliverWindowMode)	// 弹窗的任务不再满足发放条件，需要取消弹窗
		{
#ifndef TASK_TEMPL_EDITOR
			TaskInterface::PopupTaskCheckInfoDialog(pTempl->m_ID, true);
#endif
		}
	}
}

void ATaskTemplMan::UpdateStatus(TaskInterface* pTask)
{
	CheckAutoDelv(pTask);
}

void ATaskTemplMan::ManualTrigTask(TaskInterface* pTask, unsigned long ulTask)
{
	_notify_svr(pTask, TASK_CLT_NOTIFY_MANUAL_TRIG, static_cast<unsigned short>(ulTask));
}

void ATaskTemplMan::ManualTrigStorageTask(TaskInterface* pTask, unsigned long ulTask, unsigned long ulStorageId)
{
	task_notify_storage notify;
	notify.reason = TASK_CLT_NOTIFY_MANUAL_TRIG_STORAGE;
	notify.task = static_cast<unsigned short>(ulTask);
	notify.ulStorageId = ulStorageId;
	pTask->NotifyServer(&notify, sizeof(notify));
}

void ATaskTemplMan::ForceGiveUpTask(TaskInterface* pTask, unsigned long ulTask)
{
	_notify_svr(pTask, TASK_CLT_NOTIFY_FORCE_GIVEUP, static_cast<unsigned short>(ulTask));
}

void ATaskTemplMan::ForceRemoveFinishTask(TaskInterface* pTask, unsigned long ulTask)
{
#ifdef _DEBUG
	_notify_svr(pTask, TASK_CLT_NOTIFY_RM_FINISH_TASK, static_cast<unsigned short>(ulTask));
	FinishedTaskList* pList = static_cast<FinishedTaskList*>(pTask->GetFinishedTaskList());
	pList->RemoveTask(ulTask);
#endif
}

void ATaskTemplMan::OnDynTasksTimeMark(TaskInterface* pTask, unsigned long ulTimeMark, unsigned short version)
{
	if (version != DYN_TASK_CUR_VERSION)
		return;

	if (m_ulDynTasksTimeMark == ulTimeMark && LoadDynTasksFromPack(m_szDynPackPath))
	{
		SetDynTasksVerified(true);
		pTask->InitActiveTaskList();
		UpdateDynDataNPCService();
	}
	else
		_notify_svr(pTask, TASK_CLT_NOTIFY_DYN_DATA, 0);
}

void ATaskTemplMan::OnDynTasksData(TaskInterface* pTask, const void* data, size_t sz, bool ended)
{
	if (m_bDynTasksVerified)
	{
		assert(false);
		return;
	}

	size_t new_sz = sz + m_ulDynTasksDataSize;
	char* buf = new char[new_sz];
	g_ulNewCount++;

	if (m_pDynTasksData) memcpy(buf, m_pDynTasksData, m_ulDynTasksDataSize);
	memcpy(buf + m_ulDynTasksDataSize, data, sz);

	LOG_DELETE_ARR(m_pDynTasksData);
	m_pDynTasksData = buf;
	m_ulDynTasksDataSize = new_sz;

	if (ended)
	{
		if (UnmarshalDynTasks(m_pDynTasksData, m_ulDynTasksDataSize, false))
		{
			if (m_pDynTasksData)
			{
				FILE* fp = fopen(m_szDynPackPath, "wb");

				if (fp)
				{
					fwrite(m_pDynTasksData, 1, m_ulDynTasksDataSize, fp);
					fclose(fp);
				}
			}

			SetDynTasksVerified(true);
			pTask->InitActiveTaskList();
			UpdateDynDataNPCService();
		}

		LOG_DELETE_ARR(m_pDynTasksData);
		m_pDynTasksData = 0;
		m_ulDynTasksDataSize = 0;
	}
}

void ATaskTemplMan::OnStorageData(TaskInterface* pTask, const void* data)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	memcpy(pLst->m_Storages, data, sizeof(pLst->m_Storages));
}

void ATaskTemplMan::OnSpecialAward(const special_award* p)
{
	m_SpecialAward = *p;
}

void ATaskTemplMan::VerifyDynTasksPack(const char* szPath)
{
	strcpy(m_szDynPackPath, szPath);

	FILE* fp = fopen(szPath, "rb");
	if (fp == NULL) return;

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	const size_t header_sz = sizeof(DYN_TASK_PACK_HEADER);

	if (sz < header_sz)
	{
		fclose(fp);
		return;
	}

	char* buf = new char[header_sz];
	g_ulNewCount++;
	fread(buf, 1, header_sz, fp);
	fclose(fp);

	UnmarshalDynTasks(buf, header_sz, true);
	LOG_DELETE_ARR(buf);
}

//=============================================================================
/**
*@param: pTask: 当前任务接口，nPlayerLevel:玩家当前的等级，nFilterLevel:过滤和优化等级，数值越大，
*过滤越大，也越可能不稳定。在过滤等级为0时（默认）：玩家等级参数被忽略
*@return: void
*@usage: 过滤并优化任务模板数据，在过滤过程中，如果数据可以过滤则直接从内存删除。若在等级或其他条件发生
*变化后，可能需要重新从文件中读取整个任务数据，再进行重新过滤操作。这个过程比较耗费I/O时间，暂时不考虑
*优化方案。
**/
void ATaskTemplMan::FilterTaskData( TaskInterface* pTask, int nPlayerLevel, int nFilterLevel )
{
	TaskTemplMap::iterator iterMap = m_TaskTemplMap.begin();
	ATaskTempl* pCurTaskTempl = NULL;
	const int nMaxPlayerLevel = 160;
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();

	//遍历整个任务列表，过滤符合条件的任务模板数据，并释放内存空间
	while ( iterMap != m_TaskTemplMap.end() )
	{
		pCurTaskTempl = iterMap->second;
		if ( !pCurTaskTempl || pLst->GetEntry(pCurTaskTempl->m_ID) ) //如果该任务模板不存在，或者任务在活动任务列表中存在，则不过滤
		{
			++iterMap;
			continue;
		}

		//如果过滤等级为默认等级，则仅仅过滤永远不会使用的垃圾数据
		if ( pCurTaskTempl->m_ulPremise_Lev_Min > nMaxPlayerLevel || 
			 pCurTaskTempl->m_ulPremise_Lev_Max > nMaxPlayerLevel )
		{
			++iterMap;
			RemoveOneTaskFromAllMaps( pCurTaskTempl );
			delete pCurTaskTempl;
			pCurTaskTempl = NULL;
			continue;
		}

		//如果过滤等级为：1，则根据玩家等级过滤掉任务等级上限小于当前玩家等级的任务
		if ( (1 == nFilterLevel) && 
			(pCurTaskTempl->m_ulPremise_Lev_Max && (pCurTaskTempl->m_ulPremise_Lev_Max < nPlayerLevel)) )
		{
			++iterMap;
			RemoveOneTaskFromAllMaps( pCurTaskTempl );
			delete pCurTaskTempl;
			pCurTaskTempl = NULL;
			continue;
		}

		//没过滤
		++iterMap;
	}
}

/**
*@param: pTask: 需要从各个列表中删除的任务模板指针
*@return: bool:标示删除是否成功
*@usage: 从各种任务列表中删除该任务指针
**/
bool ATaskTemplMan::RemoveOneTaskFromAllMaps( ATaskTempl* pTask )
{
	//判断参数合法性
	if ( !pTask )
	{
		return false;
	}

	m_TaskTemplMap.erase( pTask->m_ID );
	if ( pTask->m_bDeathTrig )
	{
		m_DeathTrigMap.erase( pTask->m_ID );
	}
	else if ( pTask->m_bAutoDeliver )
	{
		m_AutoDelvMap.erase( pTask->m_ID );
	}

	if ( pTask->m_bSkillTask )
	{
		m_SkillTaskLst.erase( &pTask );
	}

	if ( pTask->m_DynTaskType )
	{
		m_DynTaskMap.erase( pTask->m_ID );
	}

	//递归删除任务模板指针
	RemoveOneTaskFromMap( pTask );

	//操作成功
	return true;
}


/**
*@param: pTask:需要从任务列表中删除的任务模板指针
*@return: void
*@usage: 主要是递归的删除m_ProtectNPCMap和m_AllTemplMap中的任务模板指针
**/
void ATaskTemplMan::RemoveOneTaskFromMap( ATaskTempl* pTask )
{
	if ( pTask->m_enumMethod == enumTMProtectNPC && pTask->m_ulNPCToProtect )
	{
		m_ProtectNPCMap.erase( pTask->m_ulNPCToProtect );
	}

	//
	m_AllTemplMap.erase( pTask->m_ID );
	ATaskTempl* pChildTask = pTask->m_pFirstChild;

	while ( pChildTask )
	{
		RemoveOneTaskFromMap( pChildTask );
		pChildTask = pChildTask->m_pNextSibling;
	}
}

//=============================================================================

#else

void ATaskTemplMan::CheckDeathTrig(TaskInterface* pTask)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	TaskTemplMap::iterator it = m_DeathTrigMap.begin();

	EnableLog(false);

	for (; it != m_DeathTrigMap.end(); ++it)
		if (it->second->m_bDeathTrig)
			it->second->CheckDeliverTask(
				pTask,
				0,
				NULL,
				false
				);

	EnableLog(true);
}

void ATaskTemplMan::OnTaskCheckAllTimeLimits(unsigned long ulCurTime)
{
}

void ATaskTemplMan::UpdateTimeLimitCheckList()
{
	m_TmLmtChkLst.clear();

	TaskTemplMap::iterator it = m_TaskTemplMap.begin();
	for (; it != m_TaskTemplMap.end(); ++it)
		if (it->second->m_ulMaxReceiver) m_TmLmtChkLst.push_back(it->second);
}

void ATaskTemplMan::OnTaskGetDynTasksTimeMark(TaskInterface* pTask)
{
	if (m_ulDynTasksTimeMark == 0) return;

	svr_task_dyn_time_mark data;
	data.reason		= TASK_SVR_NOTIFY_DYN_TIME_MARK;
	data.task		= 0;
	data.time_mark	= m_ulDynTasksTimeMark;
	data.version	= DYN_TASK_CUR_VERSION;
	pTask->NotifyClient(&data, sizeof(svr_task_dyn_time_mark));
}

void ATaskTemplMan::OnTaskGetSpecialAward(TaskInterface* pTask)
{
	svr_task_special_award data;

	data.reason = TASK_SVR_NOTIFY_SPECIAL_AWARD;
	data.task = 0;
	pTask->GetSpecailAwardInfo(&data.sa);
	pTask->NotifyClient(&data, sizeof(svr_task_special_award));
}

void ATaskTemplMan::OnTaskGetDynTasksData(TaskInterface* pTask)
{
	if (m_ulDynTasksDataSize == 0) return;

	const char* data = m_pDynTasksData;
	int sz = m_ulDynTasksDataSize;
	const int buf_size = 0x1000;
	const int data_size = buf_size - sizeof(task_notify_base);
	char buf[buf_size];
	task_notify_base* notify = (task_notify_base*)buf;
	char* data_part = buf + sizeof(task_notify_base);
	int send_total = 0;
	bool ended = false;

	while (true)
	{
		int send_size;

		if (send_total + data_size >= sz)
		{
			send_size = sz - send_total;
			ended = true;
		}
		else
			send_size = data_size;

		notify->reason = TASK_SVR_NOTIFY_DYN_DATA;
		notify->task = (ended ? 1 : 0);
		memcpy(data_part, data + send_total, send_size);
		pTask->NotifyClient(buf, send_size + sizeof(task_notify_base));
		if (ended) break;
		send_total += data_size;
	}
}

void ATaskTemplMan::OnTaskRemoveFinishTask(TaskInterface* pTask, unsigned long ulTask)
{
	FinishedTaskList* pList = static_cast<FinishedTaskList*>(pTask->GetFinishedTaskList());
	pList->RemoveTask(ulTask);

	char log[1024];
	sprintf(log, "RemoveFinishTask");
	TaskInterface::WriteKeyLog(pTask->GetPlayerId(), ulTask, 0, log);
}

inline void _SendPlayerStorageData(TaskInterface* pTask, ActiveTaskList* pLst)
{
	char buf[sizeof(task_notify_base) + sizeof(pLst->m_Storages)];
	reinterpret_cast<task_notify_base*>(buf)->reason = TASK_SVR_NOTIFY_STORAGE;
	memcpy(buf + sizeof(task_notify_base), pLst->m_Storages, sizeof(pLst->m_Storages));
	pTask->NotifyClient(buf, sizeof(buf));
}

void ATaskTemplMan::OnTaskUpdateStorage(TaskInterface* pTask, unsigned long ulCurTime)
{
	ActiveTaskList* pLst = static_cast<ActiveTaskList*>(pTask->GetActiveTaskList());
	tm _cur_time = *localtime((time_t*)&ulCurTime);
	const tm* _task_time = localtime((time_t*)&pLst->m_ulRefreshTime);

	if (_task_time == 0
	 || _task_time->tm_year	!= _cur_time.tm_year
	 || _task_time->tm_mon	!= _cur_time.tm_mon
	 || _task_time->tm_mday != _cur_time.tm_mday)
		UpdateStorage(pTask, pLst, ulCurTime);
}

void ATaskTemplMan::UpdateStorage(TaskInterface* pTask, ActiveTaskList* pLst, unsigned long ulCurTime, bool bNeedRefreshNinthStorage/* = true*/)
{
	// 更新刷新时间
	pLst->m_ulRefreshTime = ulCurTime;
	unsigned long id = pTask->GetPlayerId();

	//-------------------------------------------------------------------------
	//如果不需要刷新第九号库,则先保存九号库原来的值，Added 2012-09-07.
	unsigned short usStorageArray[TASK_STORAGE_LEN];
	const int nNinthIndex = 8;
	if ( !bNeedRefreshNinthStorage )	
	{
		memcpy( usStorageArray, pLst->m_Storages[nNinthIndex], sizeof(pLst->m_Storages[nNinthIndex]) );
	}
	//-------------------------------------------------------------------------
	memset(pLst->m_Storages, 0, sizeof(pLst->m_Storages));

	for (int i = 0; i < TASK_STORAGE_COUNT; i++)
	{
		//---------------------------------------------------------------------
		//如果不需要刷新第九库，则还原原来的9库值，然后第九库不进行刷新Added 2012-09-07.
		if ( !bNeedRefreshNinthStorage && (i == nNinthIndex) )
		{
			memcpy( pLst->m_Storages[nNinthIndex], usStorageArray, sizeof(pLst->m_Storages[nNinthIndex]) );
			continue;
		}
		//---------------------------------------------------------------------
		abase::vector<int>& lst = m_Storages[i];
		TaskTemplLst avail_lst;
		avail_lst.reserve(lst.size());
		size_t j;

		// disable log output
		EnableLog(false);

		// 遍历寻找所有能发放的任务
		for (j = 0; j < lst.size(); j++)
		{
			TaskTemplMap::iterator it = m_TaskTemplMap.find(lst[j]);

			if (it == m_TaskTemplMap.end())
				continue;

			ATaskTempl* pTempl = it->second;

			if (pTempl->CheckPrerequisite(pTask, pLst, ulCurTime, true, true, false, true) != 0)
				continue;

			avail_lst.push_back(pTempl);
		}

		// re-enable log output
		EnableLog(true);

		if (avail_lst.size() <= TASK_STORAGE_LEN)
		{
			// 如果可以发放的数量不大于最大数量，则直接赋值
			for (j = 0; j < avail_lst.size(); j++)
				pLst->m_Storages[i][j] = (unsigned short)avail_lst[j]->GetID();
		}
		else
		{
			float fTotalWeight = 0;

			// 计算权重总和
			for (j = 0; j < avail_lst.size(); j++)
				fTotalWeight += avail_lst[j]->m_fStorageWeight;

			// 已处理标记
			char* flags = new char[avail_lst.size()];
			memset(flags, 0, avail_lst.size());

			for (j = 0; j < TASK_STORAGE_LEN; j++)
			{
				float r = pTask->UnitRand() * fTotalWeight;

				for (size_t k = 0; k < avail_lst.size(); k++)
				{
					// 已处理则忽略
					if (flags[k])
						continue;

					ATaskTempl* pTempl = avail_lst[k];

					if (r <= pTempl->m_fStorageWeight)
					{
						pLst->m_Storages[i][j] = (unsigned short)pTempl->GetID();
						fTotalWeight -= pTempl->m_fStorageWeight;
						flags[k] = 1;
						break;
					}
					else
						r -= pTempl->m_fStorageWeight;
				}
			}

			delete[] flags;
		}
	}

	_SendPlayerStorageData(pTask, pLst);
	
	char log[1024];
	sprintf(log, "UpdateStroage called, bNeedRefreshNinthStorage = %d", bNeedRefreshNinthStorage);
	TaskInterface::WriteLog(pTask->GetPlayerId(), 0, 0, log);
}

#endif

//#ifndef WIN32
// int sel_templ(const struct dirent* file)
// {
// 	int nLen = strlen(file->d_name);
//    	if (nLen > 4 && memcmp((char*)(file->d_name) + nLen - 4, ".tkt", 4) == 0)
// 		return 1;
// 	return 0;
// }
//#endif

#ifdef TASK_TEMPL_EDITOR

static bool _compare(ATaskTempl* arg1, ATaskTempl* arg2)
{
	return arg1->GetID() < arg2->GetID();
}

bool ATaskTemplMan::SaveTasksToPack(const char* szPackPath, int nExportVersion)
{
	if (m_TaskTemplMap.size() == 0)
		return false;

	FILE* fp;
	std::vector<ATaskTempl*> TaskArr;
	TaskArr.reserve(2048);
	size_t dyn_task_count = 0;
	TaskTemplMap::iterator it = m_TaskTemplMap.begin();

	for (; it != m_TaskTemplMap.end(); ++it)
	{
		if (it->second->m_DynTaskType)
			dyn_task_count++;

		TaskArr.push_back(it->second);
	}

	std::sort(TaskArr.begin(), TaskArr.end(), _compare);
	size_t task_count = m_TaskTemplMap.size() - dyn_task_count;
	size_t pack_count = task_count / TASK_PER_PACK_COUNT;

	if (task_count > pack_count * TASK_PER_PACK_COUNT)
		pack_count++;

	TASK_PACK_INFO packinfo;
	packinfo.magic			= TASK_PACK_INFO_MAGIC;
	packinfo.version		= _task_templ_cur_version;
	packinfo.export_version	= nExportVersion;
	packinfo.item_count		= task_count;
	packinfo.pack_count		= pack_count;
	size_t cur_index = 0;
	abase::vector<task_md5> md5_vec;

	FILE* fpValidLog = fopen("task_错误_导出.txt", "wb");

	if (!fpValidLog)
		return false;

	bool bValid = true;

	size_t i;
	for (i = 0; i < pack_count; i++)
	{
		char buf[20];
		sprintf(buf, "%d", i + 1);
		char path[1024];
		strcpy(path, szPackPath);
		strcat(path, buf);

		fp = fopen(path, "wb");

		if (!fp)
		{
			fclose(fpValidLog);
			return false;
		}

		TASK_PACK_HEADER tph;
		tph.magic		= TASK_PACK_MAGIC;

		if (task_count >= TASK_PER_PACK_COUNT)
		{
			tph.item_count	= TASK_PER_PACK_COUNT;
			task_count -= TASK_PER_PACK_COUNT;
		}
		else
		{
			tph.item_count = task_count;
			task_count = 0;
		}

		long* pOffs = new long[tph.item_count];
		g_ulNewCount++;

		fwrite(&tph, sizeof(TASK_PACK_HEADER), 1, fp);

		fseek(fp, sizeof(long) * tph.item_count, SEEK_CUR);

		size_t i = 0;

		while (i < tph.item_count)
		{
			ATaskTempl* pTempl = TaskArr[cur_index];
			cur_index++;

			if (pTempl->m_DynTaskType)
				continue;

			if (!pTempl->CheckValid(fpValidLog))
				bValid = false;

			pOffs[i++] = ftell(fp);
			pTempl->SaveToBinFile(fp);
		}

		fseek(fp, sizeof(TASK_PACK_HEADER), SEEK_SET);
		fwrite(pOffs, sizeof(long), tph.item_count, fp);

		LOG_DELETE_ARR(pOffs);
		fclose(fp);

		task_md5 m;
	
		if (!TaskInterface::CalcFileMD5(path, m.data))
		{
			fclose(fpValidLog);
			return false;
		}

		md5_vec.push_back(m);
	}

	fclose(fpValidLog);

	if (!bValid)
		return false;

	fp = fopen(szPackPath, "wb");

	if (fp == NULL)
		return false;

	fwrite(&packinfo, sizeof(packinfo), 1, fp);

	for (i = 0; i < packinfo.pack_count; i++)
	{
		task_md5& m = md5_vec[i];
		fwrite(m.data, sizeof(m.data), 1, fp);
	}

	fclose(fp);

	return true;
}

bool ATaskTemplMan::SaveDynTasksToPack(const char* szPath, bool bMarshalAll)
{
	DYN_TASK_PACK_HEADER header;
	header.pack_size	= sizeof(DYN_TASK_PACK_HEADER);
	header.version		= DYN_TASK_CUR_VERSION;
	header.task_count	= 0;
	time(&header.time_mark);

	FILE* fp = fopen(szPath, "w+b");
	if (fp == NULL) return false;

	const int buf_size = 1000000;
	char* buf = new char[buf_size];
	g_ulNewCount++;

	fseek(fp, sizeof(DYN_TASK_PACK_HEADER), SEEK_SET);
	TaskTemplMap::iterator it = m_TaskTemplMap.begin();

	for (; it != m_TaskTemplMap.end(); ++it)
	{
		ATaskTempl* pTop = it->second;

		if (!bMarshalAll && !pTop->m_DynTaskType)
			continue;

		if (wcsstr(pTop->m_szName, L"测试任务_") != 0)
			continue;

		int sz = pTop->MarshalDynTask(buf);
		if (sz == 0) continue;

		assert(sz < buf_size);

		if (fwrite(buf, 1, sz, fp) != sz)
			assert(false);

		header.pack_size += sz;
		header.task_count++;
	}

	fseek(fp, 0, SEEK_SET);
	fwrite(&header, sizeof(DYN_TASK_PACK_HEADER), 1, fp);

	fclose(fp);
	LOG_DELETE_ARR(buf);
	return true;
}

bool ATaskTemplMan::SaveNPCInfoToPack(const char* szPath)
{
	TASK_NPC_PACK_HEADER header;
	header.pack_size	= sizeof(TASK_NPC_PACK_HEADER) + m_NPCInfoMap.size() * sizeof(NPC_INFO);
	header.version		= TASK_NPC_INFO_VERSION;
	header.npc_count	= m_NPCInfoMap.size();
	time(&header.time_mark);

	FILE* fp = fopen(szPath, "wb");
	if (fp == NULL) return false;

	const int buf_size = 1000000;
	char* buf = new char[buf_size];
	g_ulNewCount++;

	fwrite(&header, sizeof(TASK_NPC_PACK_HEADER), 1, fp);

	TaskNPCInfoMap::iterator it = m_NPCInfoMap.begin();
	for (; it != m_NPCInfoMap.end(); ++it)
	{
		const NPC_INFO& info = it->second;
		fwrite(&info, sizeof(info), 1, fp);
	}

	fclose(fp);
	LOG_DELETE_ARR(buf);
	return true;
}

#endif

bool ATaskTemplMan::CanGiveUpTask(unsigned long ulTaskId)
{
	const ATaskTempl* pTempl = GetTaskTemplByID(ulTaskId);
	if (!pTempl) return false;
	pTempl = pTempl->GetTopTask();
	return pTempl->m_bCanGiveUp;
}

#ifndef _TASK_CLIENT
extern void OnTaskGiveUpOneTask(TaskInterface* pTask, unsigned long ulTaskId, bool bForce);
#endif

void ATaskTemplMan::OnForgetLivingSkill(TaskInterface* pTask)
{
	FinishedTaskList* pList = static_cast<FinishedTaskList*>(pTask->GetFinishedTaskList());

	for (size_t i = 0; i < m_SkillTaskLst.size(); i++)
	{
		pList->RemoveTask(m_SkillTaskLst[i]->GetID());

#ifndef _TASK_CLIENT
		OnTaskGiveUpOneTask(pTask, m_SkillTaskLst[i]->GetID(), false);
#endif
	}

#ifndef _TASK_CLIENT

	task_notify_base notify;
	notify.reason = TASK_SVR_NOTIFY_FORGET_SKILL;
	notify.task = 0;
	pTask->NotifyClient(&notify, sizeof(notify));

#endif
}

#ifndef _TASK_CLIENT
bool _remove_active_task(TaskInterface* pTask)
{
	ActiveTaskList* pLst = (ActiveTaskList*)pTask->GetActiveTaskList();
	ActiveTaskEntry* pEntries = pLst->m_TaskEntries;
	
	for (int i = 0; i < pLst->m_uTaskCount; i++)
	{
		ActiveTaskEntry& CurEntry = pEntries[i];

		if (CurEntry.m_ParentIndex != 0xff)
			continue;

		if (!CurEntry.IsSuccess())
			continue;
		
		const ATaskTempl* pTempl = CurEntry.GetTempl();	
		if (pTempl && !pTempl->m_bKeyTask)
		{
			CurEntry.ClearSuccess();
			pTempl->OnSetFinished(pTask, pLst, &CurEntry, false);
			return true;
		}
	}

	return false;
}
#endif

void ATaskTemplMan::ClearNoKeyActiveTask(TaskInterface* pTask)
{
#ifndef _TASK_CLIENT	
	while (_remove_active_task(pTask)){}
#endif
}

void ATaskTemplMan::RemoveLifeAgainTaskInList(TaskInterface* pTask)
{
	// 清除当前任务列表中非主线任务
	ClearNoKeyActiveTask(pTask);

	// 清除完成任务列表中需要转生重置的任务
	FinishedTaskList* pFinishedList = (FinishedTaskList*)pTask->GetFinishedTaskList();
	TaskFinishTimeList* pTimeList = (TaskFinishTimeList*)pTask->GetFinishedTimeList();

	unsigned long i;
	for (i=0; i<pFinishedList->m_ulTaskCount; i++)
	{
		FnshedTaskEntry& CurEntry = pFinishedList->m_aTaskList[i];
		unsigned long ulTaskID = pFinishedList->GetRealTaskID(CurEntry.m_uTaskId);
		const ATaskTempl* pTempl = GetTopTaskByID(ulTaskID);
		if (pTempl && pTempl->m_bLifeAgainReset) 
		{
			bool bRemoveFlag = pFinishedList->RemoveTask(CurEntry.m_uTaskId);
			if (bRemoveFlag)
				i--;
		}
	}
	
	for (i=0; i<pTimeList->m_uCount; i++)
	{
		TaskFinishTimeEntry& CurEntry = pTimeList->m_aList[i];
		const ATaskTempl* pTempl = GetTopTaskByID(CurEntry.m_uTaskId);
		if (pTempl && pTempl->m_bLifeAgainReset) 
		{
			bool bRemoveFlag = pTimeList->RemoveTask(CurEntry.m_uTaskId);
			if (bRemoveFlag)
				i--;
		}
	}
}

#ifdef _ELEMENTCLIENT

void TaskShowErrMessage(int nIndex)
{
	const wchar_t* szMsg = _task_err.GetWideString(nIndex);
	if (szMsg) TaskInterface::ShowMessage(szMsg, nIndex);
}

void ATaskTemplMan::GetAvailableTasks(TaskInterface* pPlayer, TaskTemplLst& lst)
{
	lst.reserve(256);
	TaskTemplMap::iterator it = m_TaskTemplMap.begin();

	for (; it != m_TaskTemplMap.end(); ++it)
	{
		ATaskTempl* pTempl = it->second;

		if (pTempl->m_ulDelvNPC == 0 || !pTempl->m_bCanSeekOut)
			continue;

		if (pPlayer->CanDeliverTask(pTempl->GetID()) == 0)
			lst.push_back(pTempl);
	}
}

#endif

/*
 *	export functions
 */

bool ATaskTemplMan::Init(elementdataman* pMan)
{
	m_pEleDataMan = pMan;

// 初始化任务仓库列表
#ifndef _TASK_CLIENT

	DATA_TYPE dt;
	int id = pMan->get_first_data_id(ID_SPACE_ESSENCE, dt);

	while (id)
	{
		if (dt == DT_NPC_TASK_OUT_SERVICE)
		{
			NPC_TASK_OUT_SERVICE* pData = (NPC_TASK_OUT_SERVICE*)pMan->get_data_ptr(id, ID_SPACE_ESSENCE, dt);

			if (pData->id_task_set == 0)
			{
				id = pMan->get_next_data_id(ID_SPACE_ESSENCE, dt);
				continue;
			}

			if (pData->id_task_set > TASK_STORAGE_COUNT)
			{
				assert(0);
				return false;
			}

			abase::vector<int>& lst = m_Storages[pData->id_task_set - 1];

			for (size_t i = 0; i < SIZE_OF_ARRAY(pData->id_tasks); i++)
			{
				if (pData->id_tasks[i])
				{
					lst.push_back(pData->id_tasks[i]);
					m_StorageTaskMap[pData->id_tasks[i]] = 1;
				}
			}
		}

		id = pMan->get_next_data_id(ID_SPACE_ESSENCE, dt);
	}

#endif

	return true;
}

void ATaskTemplMan::Release()
{
	TaskTemplMap::iterator it = m_TaskTemplMap.begin();
	for (; it != m_TaskTemplMap.end(); ++it)
	{
		LOG_DELETE(it->second);
	}

	m_TaskTemplMap.clear();
	m_AllTemplMap.clear();
	m_AutoDelvMap.clear();
	m_DeathTrigMap.clear();
	m_ProtectNPCMap.clear();
	m_SkillTaskLst.clear();
	m_DynTaskMap.clear();
	m_ulDynTasksTimeMark = 0;
	m_ulDynTasksDataSize = 0;
	LOG_DELETE_ARR(m_pDynTasksData);
	m_pDynTasksData = 0;
	m_ulNPCInfoDataSize = 0;
	LOG_DELETE_ARR(m_pNPCInfoData);
	m_pNPCInfoData = 0;
	m_NPCInfoMap.clear();
	m_StorageTaskMap.clear();
	m_ForbiddenTaskMap.clear();

	for (int i = 0; i < TASK_STORAGE_COUNT; i++)
		m_Storages[i].clear();

#ifdef _TASK_CLIENT
	TaskInterface::SetFinishDlgShowTime(0);
	m_bDynTasksVerified = false;
	ClearSpecailAward();
#endif

#ifdef _ELEMENTCLIENT
	_task_err.Release();
#endif
}

void ATaskTemplMan::LoadAllFromDir(const char* szDir, bool bLoadDescript)
{
}

bool ATaskTemplMan::LoadTasksFromPack(const char* szPackPath, const char* szForbiddenTaskPath, bool bLoadDescript)
{
	TaskInterface::WriteLog(0, 0, 2, "LoadPack begin");
	FILE* fp = fopen(szPackPath, "rb");

	if (fp == NULL)
		return false;

	TASK_PACK_INFO packinfo;

	if (fread(&packinfo, sizeof(packinfo), 1, fp) != 1)
	{
		fclose(fp);
		return false;
	}

	m_nExportVersion = packinfo.export_version;
	size_t i;
	abase::vector<task_md5> md5_vec;
	md5_vec.reserve(64);

	for (i = 0; i < packinfo.pack_count; i++)
	{
		task_md5 m;

		if (fread(&m, sizeof(m), 1, fp) != 1)
		{
			fclose(fp);
			return false;
		}

		md5_vec.push_back(m);
	}

	fclose(fp);

	if (packinfo.magic != TASK_PACK_INFO_MAGIC || packinfo.version != _task_templ_cur_version)
		return false;

	size_t task_count = 0;

	for (i = 0; i < packinfo.pack_count; i++)
	{
		char path[1024];
		strcpy(path, szPackPath);
		char buf[20];
		sprintf(buf, "%d", i + 1);
		strcat(path, buf);

		unsigned char m[16];
		if (!TaskInterface::CalcFileMD5(path, m) || md5_vec[i] != m)
		{
			printf("task, md5 error! path=%s\n", path);
			return false;
		}

		fp = fopen(path, "rb");

		if (!fp)
		{
			printf("task, can't open file! path=%s\n", path);
			return false;
		}

		TASK_PACK_HEADER tph;
		fread(&tph, sizeof(tph), 1, fp);

		if (tph.magic != TASK_PACK_MAGIC)
			return false;

		task_count += tph.item_count;
		long* pOffs = new long[tph.item_count];
		g_ulNewCount++;

		fread(pOffs, sizeof(long), tph.item_count, fp);

		for (unsigned long i = 0; i < tph.item_count; i++)
		{
			if (fseek(fp, pOffs[i], SEEK_SET) != 0)
			{
				fclose(fp);
				delete[] pOffs;
				return false;
			}

			ATaskTempl* pTempl = new ATaskTempl;
			g_ulNewCount++;

			if (!pTempl->LoadFromBinFile(fp))
			{
				fclose(fp);
				delete[] pOffs;
				delete pTempl;
				return false;
			}

			AddOneTaskTempl(pTempl);
			TaskInterface::WriteLog(0, pTempl->m_ID, 2, "LoadTask");
		}

		LOG_DELETE_ARR(pOffs);
		fclose(fp);
	}

	if (task_count != packinfo.item_count)
		return false;

	char log[1024];
	sprintf(log, "LoadTask, Count = %d", m_TaskTemplMap.size());
	TaskInterface::WriteLog(0, 0, 2, log);

#ifndef _TASK_CLIENT
	UpdateTimeLimitCheckList();
#endif

#ifdef _ELEMENTCLIENT
	_task_err.Release();
	_task_err.Init("Configs\\task_err.txt", true);
#endif

	LoadForbiddenTasks(szForbiddenTaskPath);
	return true;
}

void ATaskTemplMan::AddTaskToMap(ATaskTempl* pTempl)
{
	if (pTempl->m_enumMethod == enumTMProtectNPC && pTempl->m_ulNPCToProtect)
		m_ProtectNPCMap[pTempl->m_ulNPCToProtect] = pTempl;

	m_AllTemplMap[pTempl->m_ID] = pTempl;
	ATaskTempl* pChild = pTempl->m_pFirstChild;

	while (pChild)
	{
		AddTaskToMap(pChild);
		pChild = pChild->m_pNextSibling;
	}
}

void ATaskTemplMan::AddOneTaskTempl(ATaskTempl* pTask)
{
	TaskTemplMap::iterator it = m_TaskTemplMap.find(pTask->m_ID);
	if (it != m_TaskTemplMap.end())
	{
		TaskInterface::WriteLog(0, pTask->m_ID, 0, "Dup Task Found");
		return;
	}

	m_TaskTemplMap[pTask->m_ID] = pTask;

	if (pTask->m_bDeathTrig) m_DeathTrigMap[pTask->m_ID] = pTask;
	else if (pTask->m_bAutoDeliver) m_AutoDelvMap[pTask->m_ID] = pTask;

	if (pTask->m_bSkillTask) m_SkillTaskLst.push_back(pTask);

	if (pTask->m_DynTaskType)
	{
		TaskTemplMap::iterator itDyn = m_DynTaskMap.find(pTask->m_ID);
		if (itDyn != m_DynTaskMap.end())
			TaskInterface::WriteLog(0, pTask->m_ID, 0, "Dup Dyn Task Found");

		m_DynTaskMap[pTask->m_ID] = pTask;
	}

	AddTaskToMap(pTask);
}

ATaskTempl* ATaskTemplMan::LoadOneTaskTempl(const char* szPath, bool bLoadDescript)
{
	ATaskTempl* pTask = new ATaskTempl;
	g_ulNewCount++;

	if (!pTask->LoadFromTextFile(szPath, bLoadDescript))
	{
		LOG_DELETE(pTask);
		return 0;
	}

	AddOneTaskTempl(pTask);
	return pTask;
}

void ATaskTemplMan::DelOneTaskTempl(ATaskTempl* pTask)
{
	TaskTemplMap::iterator it = m_TaskTemplMap.find(pTask->m_ID);
	if (it != m_TaskTemplMap.end()) m_TaskTemplMap.erase(pTask->m_ID);
	LOG_DELETE(pTask);
}

ATaskTempl* ATaskTemplMan::GetTopTaskByID(unsigned long ulID)
{
	TaskTemplMap::iterator it = m_TaskTemplMap.find(ulID);
	if (it == m_TaskTemplMap.end()) return 0;
	return it->second;
}

ATaskTempl* ATaskTemplMan::GetTaskTemplByID(unsigned long ulID)
{
	TaskTemplMap::iterator it = m_AllTemplMap.find(ulID);
	if (it == m_AllTemplMap.end()) return 0;
	return it->second;
}

const ATaskTempl* ATaskTemplMan::GetProtectNPCTask(unsigned long ulNPCId)
{
	TaskTemplMap::iterator it = m_ProtectNPCMap.find(ulNPCId);
	if (it != m_ProtectNPCMap.end()) return it->second;
	return NULL;
}

void mount_task_out_service(const ATaskTempl* task, NPC_TASK_OUT_SERVICE* svr)
{
	if (task->IsAutoDeliver())
		return;

	if (task->m_pParent && !task->m_pParent->m_bChooseOne)
		return;

	for (size_t i = 0; i < SIZE_OF_ARRAY(svr->id_tasks); i++)
	{
		if (svr->id_tasks[i]) continue;
		svr->id_tasks[i] = task->GetID();
		break;
	}

	const ATaskTempl* child = task->m_pFirstChild;
	while (child)
	{
		mount_task_out_service(child, svr);
		child = child->m_pNextSibling;
	}
}

void mount_task_in_service(const ATaskTempl* task, NPC_TASK_IN_SERVICE* svr)
{
	if (task->m_enumFinishType == enumTFTNPC)
	{
		for (size_t i = 0; i < SIZE_OF_ARRAY(svr->id_tasks); i++)
		{
			if (svr->id_tasks[i]) continue;
			svr->id_tasks[i] = task->GetID();
			break;
		}
	}

	const ATaskTempl* child = task->m_pFirstChild;
	while (child)
	{
		mount_task_in_service(child, svr);
		child = child->m_pNextSibling;
	}
}

void ATaskTemplMan::UpdateDynDataNPCService()
{
	assert(m_pEleDataMan);

	DATA_TYPE dt;
	NPC_TASK_IN_SERVICE* service = (NPC_TASK_IN_SERVICE*)m_pEleDataMan->get_data_ptr(
		DYN_TASK_VERIFY_SVR,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!service || dt != DT_NPC_TASK_IN_SERVICE)
	{
		char log[1024];
		sprintf(log, "UpdateDynDataNPCService, wrong service, dt = %d", dt);
		TaskInterface::WriteLog(0, 0, 0, log);
		return;
	}

	NPC_TASK_OUT_SERVICE* deliver = (NPC_TASK_OUT_SERVICE*)m_pEleDataMan->get_data_ptr(
		DYN_TASK_DELIVER_SVR,
		ID_SPACE_ESSENCE,
		dt
		);

	if (!deliver || dt != DT_NPC_TASK_OUT_SERVICE)
	{
		char log[1024];
		sprintf(log, "UpdateDynDataNPCService, wrong service, dt = %d", dt);
		TaskInterface::WriteLog(0, 0, 0, log);
		return;
	}

	memset(deliver->id_tasks, 0, sizeof(deliver->id_tasks));
	memset(service->id_tasks, 0, sizeof(service->id_tasks));

	TaskTemplMap::iterator it = m_DynTaskMap.begin();
	for (; it != m_DynTaskMap.end(); ++it)
	{
		const ATaskTempl* p = it->second;
		mount_task_out_service(p, deliver);
		mount_task_in_service(p, service);
	}
}

bool ATaskTemplMan::UnmarshalDynTasks(const char* data, size_t data_size, bool header_only)
{
	if (data_size < sizeof(DYN_TASK_PACK_HEADER))
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalDynTasks, wrong size");
		return false;
	}

	const char* p = data;
	DYN_TASK_PACK_HEADER* header = (DYN_TASK_PACK_HEADER*)p;
	p += sizeof(DYN_TASK_PACK_HEADER);

	if (header->version != DYN_TASK_CUR_VERSION)
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalDynTasks, wrong version");
		return false;
	}

	if (header->pack_size != data_size)
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalDynTasks, wrong header");
		return false;
	}

	m_ulDynTasksTimeMark = header->time_mark;

	if (header_only)
		return true;

	for (unsigned short i = 0; i < header->task_count; i++)
	{
		ATaskTempl* pTempl = new ATaskTempl;
		g_ulNewCount++;

		p += pTempl->UnmarshalDynTask(p);
		pTempl->m_ulDisplayType = 1; // 设置动态任务的显示类型为日常 2012/04/27
		AddOneTaskTempl(pTempl);

		TaskInterface::WriteLog(0, pTempl->GetID(), 2, "LoadDynTask");
	}

	assert(p == data + data_size);
	return true;
}

bool ATaskTemplMan::UnmarshalNPCInfo(const char* data, size_t data_size, bool header_only)
{
	if (data_size < sizeof(TASK_NPC_PACK_HEADER))
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalNPCInfo, wrong size");
		return false;
	}

	const char* p = data;
	TASK_NPC_PACK_HEADER* header = (TASK_NPC_PACK_HEADER*)p;
	p += sizeof(TASK_NPC_PACK_HEADER);

	if (header->version != TASK_NPC_INFO_VERSION)
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalNPCInfo, wrong version");
		return false;
	}

	if (header->pack_size != data_size)
	{
		TaskInterface::WriteLog(0, 0, 0, "UnmarshalNPCInfo, wrong header");
		return false;
	}

	m_ulNPCInfoTimeMark = header->time_mark;

	if (header_only)
		return true;

	const NPC_INFO* pInfos = (const NPC_INFO*)p;
	for (unsigned short i = 0; i < header->npc_count; i++)
	{
		const NPC_INFO& info = pInfos[i];
		m_NPCInfoMap[info.id] = info;
	}

	return true;
}

bool ATaskTemplMan::LoadDynTasksFromPack(const char* szPath)
{
	TaskInterface::WriteLog(0, 0, 2, "LoadDynPack begin");

	FILE* fp = fopen(szPath, "rb");
	if (fp == NULL)
	{
		TaskInterface::WriteLog(0, 0, 0, "LoadDynTasksFromPack, no such file");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (sz == 0)
	{
		fclose(fp);
		return false;
	}

	char* buf = new char[sz];
	g_ulNewCount++;
	fread(buf, 1, sz, fp);
	fclose(fp);

	if (!UnmarshalDynTasks(buf, sz, false))
	{
		LOG_DELETE_ARR(buf);
		return false;
	}

#ifdef _TASK_CLIENT
	LOG_DELETE_ARR(buf);
#else
	m_pDynTasksData = buf;
	m_ulDynTasksDataSize = sz;
	UpdateDynDataNPCService();
#endif

	return true;
}

bool ATaskTemplMan::LoadNPCInfoFromPack(const char* szPath)
{
	FILE* fp = fopen(szPath, "rb");
	if (fp == NULL)
	{
		TaskInterface::WriteLog(0, 0, 0, "LoadNPCInfoFromPack, no such file");
		return false;
	}

	fseek(fp, 0, SEEK_END);
	size_t sz = ftell(fp);
	fseek(fp, 0, SEEK_SET);

	if (sz == 0)
	{
		fclose(fp);
		return false;
	}

	char* buf = new char[sz];
	g_ulNewCount++;
	fread(buf, 1, sz, fp);
	fclose(fp);

	if (!UnmarshalNPCInfo(buf, sz, false))
	{
		LOG_DELETE_ARR(buf);
		return false;
	}

#ifdef _TASK_CLIENT
	LOG_DELETE_ARR(buf);
#else
	m_pNPCInfoData = buf;
	m_ulNPCInfoDataSize = sz;
#endif

	return true;
}

static bool ReadLine(FILE* fp, char* szLine)
{
	if (fgets(szLine, 1024, fp) == NULL)
		return false;

	szLine[strcspn(szLine, "\r\n")] = '\0';
	return true;
}

void ATaskTemplMan::LoadForbiddenTasks(const char* szForbiddenTaskPath)
{
	m_ForbiddenTaskMap.clear();

	FILE* fp = fopen(szForbiddenTaskPath, "rb");

	if (!fp)
		return;

	char line[1024];

	while (ReadLine(fp, line))
	{
		char* tail = strstr(line, "//");

		if (tail)
			*tail = 0;

		int id = atoi(line);

		if (id)
			m_ForbiddenTaskMap[id] = id;
	}

	fclose(fp);
}

// 判断指定NPC是否在指定任务中出现
bool ATaskTemplMan::CheckNPCAppearInTask(unsigned long ulTaskId, unsigned long ulNPC)
{
	ATaskTempl* pTempl = GetTaskTemplByID(ulTaskId);
	if(!pTempl)
		return false;

	if(ulNPC == pTempl->m_ulDelvNPC || ulNPC == pTempl->m_ulAwardNPC || ulNPC == pTempl->m_ulActionNPC) // 做表情动作的npc也需要考虑进去 Added 2013-5-10
		return true;

	for(unsigned int i=0;i<pTempl->m_ulMonsterWanted;i++)
	{
		const MONSTER_WANTED& mw = pTempl->m_MonsterWanted[i];

		if(ulNPC == mw.m_ulMonsterTemplId)
			return true;
	}

	//由于现在的需求是：希望收集物品时也能启动自动寻径，而物品相关的寻径终点位置也存储在NPC_INFO 列表中，因此都用此接口Added 2011-10-14.
	for ( unsigned int j = 0; j < pTempl->m_ulItemsWanted; ++j )
	{
		const ITEM_WANTED& ItemWanted = pTempl->m_ItemsWanted[j];

		if ( ulNPC == ItemWanted.m_ulItemTemplId ) // !!!注意!!! 参数ulNPC 也可能不是NPC的id， 而是物品的id
		{
			return true;
		}
	}
	//Added end.

	return false;
}
