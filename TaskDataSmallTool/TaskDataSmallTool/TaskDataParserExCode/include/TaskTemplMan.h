#ifndef _TASKTEMPLMAN_H_
#define _TASKTEMPLMAN_H_

#include "hashmap.h"
#include "vector.h"
#include "TaskTempl.h"

class elementdataman;
class _task_hash_func
{
public:
	inline unsigned long operator() (unsigned long data) const { return data; }
};

class ATaskTempl;
typedef abase::hash_map<unsigned long, ATaskTempl*, _task_hash_func> TaskTemplMap;
typedef abase::vector<ATaskTempl*> TaskTemplLst;
typedef abase::hash_map<unsigned long, NPC_INFO, _task_hash_func> TaskNPCInfoMap;

struct TaskInterface;

class ATaskTemplMan
{
public:

	ATaskTemplMan();
	~ATaskTemplMan();

protected:

	TaskTemplMap	m_TaskTemplMap;
	TaskTemplMap	m_AllTemplMap;
	TaskTemplMap	m_AutoDelvMap;
	TaskTemplMap	m_DeathTrigMap;
	TaskTemplMap	m_ProtectNPCMap;
	TaskTemplLst	m_SkillTaskLst;
	elementdataman*	m_pEleDataMan;
	abase::vector<int> m_Storages[TASK_STORAGE_COUNT];
	abase::hash_map<int, int> m_StorageTaskMap;
	abase::hash_map<int, int> m_ForbiddenTaskMap;

#ifdef _TASK_CLIENT
	char			m_szDynPackPath[512];
	bool			m_bDynTasksVerified;
	special_award	m_SpecialAward;
#else
	TaskTemplLst	m_TmLmtChkLst;
#endif

	TaskTemplMap	m_DynTaskMap;
	unsigned long	m_ulDynTasksTimeMark;
	char*			m_pDynTasksData;
	unsigned long	m_ulDynTasksDataSize;

	char*			m_pNPCInfoData;
	unsigned long	m_ulNPCInfoDataSize;
	unsigned long	m_ulNPCInfoTimeMark;
	TaskNPCInfoMap	m_NPCInfoMap;
	bool			m_bEnableLog;
	int				m_nExportVersion;

protected:

	void AddTaskToMap(ATaskTempl* pTempl);
	void UpdateDynDataNPCService();
	void LoadForbiddenTasks(const char* szForbiddenTaskPath);

#ifdef _TASK_CLIENT
	void CheckAutoDelv(TaskInterface* pTask);
#else
	void UpdateTimeLimitCheckList();
#endif

public:

	elementdataman* GetEleDataMan() { return m_pEleDataMan; }
	ATaskTempl* GetTopTaskByID(unsigned long ulID);
	ATaskTempl* GetTaskTemplByID(unsigned long ulID);
	TaskTemplMap& GetAllTemplMap() { return m_AllTemplMap; }
	TaskTemplMap& GetTopTemplMap() { return m_TaskTemplMap; }
	ATaskTempl* LoadOneTaskTempl(const char* szPath, bool bLoadDescript = true);
	void AddOneTaskTempl(ATaskTempl* pTask);
	void DelOneTaskTempl(ATaskTempl* pTask);
	const ATaskTempl* GetProtectNPCTask(unsigned long ulNPCId);
	bool UnmarshalDynTasks(const char* data, size_t data_size, bool header_only);
	bool UnmarshalNPCInfo(const char* data, size_t data_size, bool header_only);
	const NPC_INFO* GetTaskNPCInfo(unsigned long ulNPC)
	{
		TaskNPCInfoMap::iterator it = m_NPCInfoMap.find(ulNPC);
		if (it == m_NPCInfoMap.end()) return 0;
		return &it->second;
	}
	bool CheckNPCAppearInTask(unsigned long ulTaskId, unsigned long ulNPC);

	bool IsLogEnable() const { return m_bEnableLog; }
	void EnableLog(bool b) { m_bEnableLog = b; }
	bool IsStorageTask(unsigned long id)
	{
		return m_StorageTaskMap.find(id) != m_StorageTaskMap.end();
	}

	bool IsForbiddenTask(unsigned long ulTaskId)
	{
		return m_ForbiddenTaskMap.find((int)ulTaskId) != m_ForbiddenTaskMap.end();
	}

#ifdef TASK_TEMPL_EDITOR
	bool SaveTasksToPack(const char* szPackPath, int nExportVersion);
	bool SaveDynTasksToPack(const char* szPath, bool bMarshalAll = false);
	bool SaveNPCInfoToPack(const char* szPath);
	TaskNPCInfoMap& GetNPCInfoMap() { return m_NPCInfoMap; }
#endif

	bool CanGiveUpTask(unsigned long ulTaskId);
	void OnForgetLivingSkill(TaskInterface* pTask);
	void RemoveLifeAgainTaskInList(TaskInterface* pTask);
	void ClearNoKeyActiveTask(TaskInterface* pTask);

	// process part
#ifdef _TASK_CLIENT
	void UpdateStatus(TaskInterface* pTask);
	void GetAvailableTasks(TaskInterface* pPlayer, TaskTemplLst& lst);
	void ManualTrigTask(TaskInterface* pTask, unsigned long ulTask);
	void ManualTrigStorageTask(TaskInterface* pTask, unsigned long ulTask, unsigned long ulStorageId);//Added 2011-03-02
	void ForceGiveUpTask(TaskInterface* pTask, unsigned long ulTask);
	void ForceRemoveFinishTask(TaskInterface* pTask, unsigned long ulTask);
	bool IsDynTasksVerified() const { return m_bDynTasksVerified; }
	void SetDynTasksVerified(bool b) { m_bDynTasksVerified = b; }
	void OnDynTasksTimeMark(TaskInterface* pTask, unsigned long ulTimeMark, unsigned short version);
	void OnDynTasksData(TaskInterface* pTask, const void* data, size_t sz, bool ended);
	void OnStorageData(TaskInterface* pTask, const void* data);
	void OnSpecialAward(const special_award* p);
	void VerifyDynTasksPack(const char* szPath);
	const special_award* GetSpecialAward() const { return &m_SpecialAward; }
	void ClearSpecailAward() { memset(&m_SpecialAward, 0, sizeof(m_SpecialAward)); }
#else
	void CheckDeathTrig(TaskInterface* pTask);
	void OnTaskCheckAllTimeLimits(unsigned long ulCurTime);
	void OnTaskGetDynTasksTimeMark(TaskInterface* pTask);
	void OnTaskGetDynTasksData(TaskInterface* pTask);
	void OnTaskGetSpecialAward(TaskInterface* pTask);
	void OnTaskRemoveFinishTask(TaskInterface* pTask, unsigned long ulTask);
	void OnTaskUpdateStorage(TaskInterface* pTask, unsigned long ulCurTime);
	void UpdateStorage(TaskInterface* pTask, ActiveTaskList* pLst, unsigned long ulCurTime);
#endif

public:
	/*
	 *	export functions
	 */

	bool Init(elementdataman* pMan);
	void Release();

	// 从Dir中加载所有任务模板
	void LoadAllFromDir(const char* szDir, bool bLoadDescript);

	// 从包中读取所有任务模板
	bool LoadTasksFromPack(const char* szPackPath, const char* szForbiddenTaskPath, bool bLoadDescript);

	// 从包中读取动态任务模板
	bool LoadDynTasksFromPack(const char* szPath);

	// 从包中读取任务NPC信息
	bool LoadNPCInfoFromPack(const char* szPath);

	// 策划导出的版本
	int GetExportVersion() { return m_nExportVersion; }
};

ATaskTemplMan* GetTaskTemplMan();

#endif
