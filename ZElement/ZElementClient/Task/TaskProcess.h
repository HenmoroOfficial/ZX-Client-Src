#ifndef TASKPROCESS_H_
#define TASKPROCESS_H_

#include <time.h>
#include "TaskInterface.h"

extern unsigned long g_ulNewCount;
extern unsigned long g_ulDelCount;

#define LOG_DELETE(p) \
if (p)\
{\
	delete p;\
	p = NULL;\
	g_ulDelCount++;\
}

#define LOG_DELETE_ARR(p) \
if (p)\
{\
	delete[] p;\
	p = NULL;\
	g_ulDelCount++;\
}

#define MAX(a,b) ((a) > (b) ? (a) : (b))
#define MIN(a,b) ((a) < (b) ? (a) : (b))

#define DEBUG_MODE

#ifdef _ELEMENTCLIENT
#define DEBUG_LOG		0
#else
#define DEBUG_LOG		1
#endif

#if DEBUG_LOG == 1
	extern const char* _log_file;
#endif

#if DEBUG_LOG == 1
	inline void WriteLog(const char* szLine)
	{
		FILE* fp = fopen(_log_file, "ab");
		if (fp)
		{
			time_t tm;
			time(&tm);
			char buf[50];
			strcpy(buf, ctime(&tm));
			buf[strlen(buf)-1] = 0;
			fprintf(fp, "%s: %s\r\n", buf, szLine);
			fclose(fp);
		}
	}
	#define TASK_WRITE_LOG0(szLine) WriteLog(szLine);
	#define TASK_WRITE_LOG1(szFormat, p1) \
	{ \
		char szBuf[1024]; \
		sprintf(szBuf, szFormat, p1); \
		WriteLog(szBuf); \
	}
	#define TASK_WRITE_LOG2(szFormat, p1, p2) \
	{ \
		char szBuf[1024]; \
		sprintf(szBuf, szFormat, p1, p2); \
		WriteLog(szBuf); \
	}
	#define TASK_WRITE_LOG3(szFormat, p1, p2, p3) \
	{ \
		char szBuf[1024]; \
		sprintf(szBuf, szFormat, p1, p2, p3); \
		WriteLog(szBuf); \
	}
	#define TASK_WRITE_LOG4(szFormat, p1, p2, p3, p4) \
	{ \
		char szBuf[1024]; \
		sprintf(szBuf, szFormat, p1, p2, p3, p4); \
		WriteLog(szBuf); \
	}
	#define TASK_WRITE_LOG5(szFormat, p1, p2, p3, p4, p5) \
	{ \
		char szBuf[1024]; \
		sprintf(szBuf, szFormat, p1, p2, p3, p4, p5); \
		WriteLog(szBuf); \
	}
#else
#define TASK_WRITE_LOG0(szLine) {}
#define TASK_WRITE_LOG1(szFormat, p1) {}
#define TASK_WRITE_LOG2(szFormat, p1, p2) {}
#define TASK_WRITE_LOG3(szFormat, p1, p2, p3) {}
#define TASK_WRITE_LOG4(szFormat, p1, p2, p3, p4) {}
#define TASK_WRITE_LOG5(szFormat, p1, p2, p3, p4, p5) {}
#endif

#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(x) (sizeof(x) / sizeof((x)[0]))
#endif

class ATaskTempl;

#pragma pack(1)

#define		TASK_STATE_FINISHED						0x01	// 是否完成
#define		TASK_STATE_SUCCESS						0x02	// 是否成功
#define		TASK_STATE_GIVEUP						0x04	// 是否放弃
#define		TASK_STATE_ERR_REPORTED					0x08	// 是否已把错误发送到客户端
#define		TASK_STATE_AWARD_NOTIFY_TEAM			0x10	// 是否已把奖励通知队员
#define		TASK_STATE_AWARD_FAMILY					0x20	// 是否已发放家族相关奖励
#define		TASK_STATE_CAN_DELIVER_EXTRA_AWARD		0x40	// 能否发放额外奖励//Added 2011-03-04
#define		TASK_STATE_HAS_DELIVER_EXTRA_AWARD		0x80	// 是否已经发放额外奖励//Added 2011-03-04

// Cur Size 21 bytes
struct TASK_ENTRY_FIXED_DATA
{
	unsigned short		m_ID;				// ID
	unsigned char		m_ParentIndex;		// 父节点索引
	unsigned char		m_PrevSblIndex;		// 兄弟节点索引
	unsigned char		m_NextSblIndex;		// 兄弟节点索引
	unsigned char		m_ChildIndex;		// 子节点索引
	unsigned char		m_uState;			// 任务状态
	unsigned long		m_ulTaskTime;		// 时间标记
	unsigned short		m_uCapTaskId;		// 队长任务
	unsigned long		m_ulTemplAddr;		// 模板地址
	unsigned long		m_ulCapTemplAddr;	// 队长任务地址
};

 // 大小为TASK_DATA_BUF_MAX_LEN
struct ActiveTaskEntry : public TASK_ENTRY_FIXED_DATA
{
	// !!! IMPORTANT: 数据区定义为联合，大小不能超过m_BufData
	union
	{
		unsigned char	m_BufData[TASK_DATA_BUF_MAX_LEN-sizeof(TASK_ENTRY_FIXED_DATA)];
		// Actual Data
		unsigned short	m_wMonsterNum[MAX_MONSTER_WANTED];
		unsigned short  m_wItemsToGet;
		unsigned short  m_wInterObjNum[MAX_INTEROBJ_WANTED];
	};

	bool IsFinished() const { return (m_uState & TASK_STATE_FINISHED) != 0; }
	bool IsSuccess() const { return (m_uState & TASK_STATE_SUCCESS) != 0; }
	bool IsGiveUp() const { return (m_uState & TASK_STATE_GIVEUP) != 0; }
	bool IsErrReported() const { return (m_uState & TASK_STATE_ERR_REPORTED) != 0; }
	bool IsAwardNotifyTeam() const { return (m_uState & TASK_STATE_AWARD_NOTIFY_TEAM) != 0; }
	bool IsAwardFamily() const { return (m_uState & TASK_STATE_AWARD_FAMILY) != 0; }
	bool IsCanDeliverExtraAward() const { return (m_uState & TASK_STATE_CAN_DELIVER_EXTRA_AWARD) != 0; }//Added 2011-03-04
	bool IsHasDeliverExtraAward() const { return (m_uState & TASK_STATE_HAS_DELIVER_EXTRA_AWARD) != 0; }//Added 2011-03-04

	void SetFinished() { m_uState |= TASK_STATE_FINISHED; }
	void ClearFinished() { m_uState &= ~TASK_STATE_FINISHED; }
	void SetSuccess() { m_uState |= TASK_STATE_SUCCESS; }
	void ClearSuccess() { m_uState &= ~TASK_STATE_SUCCESS; }
	void SetGiveUp() { m_uState |= TASK_STATE_GIVEUP; }
	void ClearGiveUp() { m_uState &= ~TASK_STATE_GIVEUP; }
	void SetErrReported() { m_uState |= TASK_STATE_ERR_REPORTED; }
	void ClearErrReported() { m_uState &= ~TASK_STATE_ERR_REPORTED; }
	void SetAwardNotifyTeam() { m_uState |= TASK_STATE_AWARD_NOTIFY_TEAM; }
	void ClearAwardNotifyTeam() { m_uState &= ~TASK_STATE_AWARD_NOTIFY_TEAM; }
	void SetAwardFamily() { m_uState |= TASK_STATE_AWARD_FAMILY; }
	void ClearAwardFamily() { m_uState &= ~TASK_STATE_AWARD_FAMILY; }
	void SetCanDeliverExtraAward(){ m_uState |= TASK_STATE_CAN_DELIVER_EXTRA_AWARD; }//Added 2011-03-04
	void ClearCanDeliverExtraAward(){ m_uState &= ~TASK_STATE_CAN_DELIVER_EXTRA_AWARD; }//Added 2011-03-04
	void SetHasDeliverExtraAward(){ m_uState |= TASK_STATE_HAS_DELIVER_EXTRA_AWARD; }//Added 2011-03-04
	void ClearHasDeliverExtraAward(){ m_uState &= ~TASK_STATE_HAS_DELIVER_EXTRA_AWARD; }//Added 2011-03-04

	const ATaskTempl* GetTempl() const { return reinterpret_cast<const ATaskTempl*>(m_ulTemplAddr); }
	const ATaskTempl* GetCap() const { return reinterpret_cast<const ATaskTempl*>(m_ulCapTemplAddr); }
	const ATaskTempl* GetCapOrSelf() const
	{
		if (m_ulCapTemplAddr) return GetCap();
		else return GetTempl();
	}
	bool HasParent() const { return m_ParentIndex != 0xff; }
	bool HasChildren() const { return m_ChildIndex != 0xff; }
	bool IsValid(unsigned char uIndex, unsigned char uMaxCount) const
	{
		if (m_ParentIndex != 0xff)
		{
			if (m_ParentIndex >= uIndex || m_ParentIndex >= uMaxCount)
				return false;
		}

		if (m_PrevSblIndex != 0xff)
		{
			if (m_PrevSblIndex >= uIndex || m_PrevSblIndex >= uMaxCount)
				return false;
		}

		if (m_NextSblIndex != 0xff)
		{
			if (m_NextSblIndex <= uIndex || m_NextSblIndex >= uMaxCount)
				return false;
		}

		if (m_ChildIndex != 0xff)
		{
			if (m_ChildIndex <= uIndex || m_ChildIndex >= uMaxCount)
				return false;
		}

		return true;
	}
};

#define TLIST_STATE_UPDATE_TIME_MARK	1

struct ActiveTaskList
{
	union
	{
		unsigned char header[TASK_ACTIVE_LIST_HEADER_LEN];

		struct
		{
			unsigned char		m_uTaskCount;
			unsigned char		m_uUsedCount;
			unsigned short		m_Version;
			unsigned char		m_uTopShowTaskCount;
			unsigned char		m_uListState;
			unsigned long		m_ulRefreshTime;
			unsigned char		m_uTopHideTaskCount;
			unsigned char		m_uUnused3;
		};
	};

	unsigned short		m_Storages[TASK_STORAGE_COUNT][TASK_STORAGE_LEN];
	ActiveTaskEntry		m_TaskEntries[TASK_ACTIVE_LIST_MAX_LEN];

	void UpdateTaskMask(unsigned long& ulMask) const;
	void UpdateUsedCount();
	void RealignTask(ActiveTaskEntry* pEntry, unsigned char uReserve);
	void ClearTask(TaskInterface* pTask, ActiveTaskEntry* pEntry, bool bRemoveItem);
	void RecursiveClearTask(TaskInterface* pTask, ActiveTaskEntry* pEntry, bool bRemoveItem, bool bRemoveAcquired, bool bClearTask);
	void ClearChildrenOf(TaskInterface* pTask, ActiveTaskEntry* pParent);
	void ClearFamilyTask(TaskInterface* pTask);
	ActiveTaskEntry* GetEntry(unsigned long ulId)
	{
		for (unsigned char i = 0; i < m_uTaskCount; i++)
			if (m_TaskEntries[i].m_ID == ulId)
				return &m_TaskEntries[i];

		return NULL;
	}
	void RemoveAll()
	{
		unsigned short ver = m_Version;
		memset(this, 0, sizeof(*this));
		m_Version = ver;
	}
	bool IsValid() const { return m_uTaskCount <= TASK_ACTIVE_LIST_MAX_LEN; }
	bool IsTimeMarkUpdate() const { return (m_uListState & TLIST_STATE_UPDATE_TIME_MARK) != 0; }
	void SetTimeMarkUpdate() { m_uListState |= TLIST_STATE_UPDATE_TIME_MARK; }
	void ClearTimeMarkUpdate() { m_uListState &= ~TLIST_STATE_UPDATE_TIME_MARK; }
};

struct FnshedTaskEntry
{
	unsigned short m_uTaskId;
};

struct FinishedTaskList
{
	union
	{
		struct
		{
			unsigned long		m_ulTaskCount;
			FnshedTaskEntry		m_aTaskList[TASK_FINISHED_LIST_MAX_LEN];
		};
		unsigned char m_Buf[TASK_FINISHED_LIST_BUF_SIZE];
	};

protected:

	int GetTaskPos(unsigned long ulID) const;

public:

	void AddOneTask(unsigned long ulID, bool bSuccess);
	bool RemoveTask(unsigned long ulID);
	int SearchTask(unsigned long ulID ) const;
	void RemoveAll() { memset(this, 0, sizeof(*this)); }
	bool IsValid() const { return m_ulTaskCount <= TASK_FINISHED_LIST_MAX_LEN; }
	bool IsFull() const { return m_ulTaskCount >= TASK_FINISHED_LIST_MAX_LEN; }
	unsigned long GetRealTaskID(unsigned long ulIDInEntry);
};

struct TaskPreservedData
{
	unsigned short	reason;
	unsigned long	m_ulSrcTask;
	unsigned long	m_ulRcvNum;

	union
	{
		unsigned long	m_ulSubTaskId;
		unsigned long	m_ulKillTime;
	};
};

union TaskGlobalData
{
	unsigned char buf[TASK_GLOBAL_DATA_SIZE];

	struct
	{
		unsigned long m_ulReceiverNum;
		unsigned long m_ulRcvUpdateTime;
		unsigned long m_ulFinishCount;
		unsigned long m_ulFnshUpdateTime;
	};

	void AddRevNum() { m_ulReceiverNum++; }
	void CheckRcvUpdateTime(const ATaskTempl* pTempl, unsigned long ulCurTime);
};

struct _TaskFinishTimeEntry_V2
{
	unsigned short	m_uTaskId;
	unsigned long	m_ulTimeMark;
};

struct TaskFinishTimeEntry
{
	unsigned short	m_uTaskId;
	unsigned long	m_ulTimeMark; // Deliver time
	unsigned short	m_uFinishCount;
	unsigned long	m_ulUpdateTime;

	void IncFinishCount()
	{
		m_uFinishCount++;

		if (m_uFinishCount == 0)
			m_uFinishCount = 0xffff;
	}
};

struct TaskFinishTimeList
{
	unsigned short m_uCount;
	TaskFinishTimeEntry m_aList[TASK_FINISH_TIME_MAX_LEN];

public:

	TaskFinishTimeEntry* Search(unsigned long ulID);
	void AddDeliverTime(unsigned long ulID, unsigned long ulTime);
	void AddOrUpdateDeliverTime(unsigned long ulID, unsigned long ulTime);
	unsigned short AddFinishTime(unsigned long ulID, unsigned long ulTime);
	void AddOrUpdateFinishCount(unsigned long ulID);
	void AddOrUpdateFinishCountExt( unsigned long ulID, unsigned long count );//Added 2011-03-23.
	void AddFinishCount(unsigned long ulID, unsigned short count);
	void RemoveAll() { memset(this, 0, sizeof(*this)); }
	bool IsValid() const { return m_uCount <= TASK_FINISH_TIME_MAX_LEN; }
	bool IsFull() const { return m_uCount >= TASK_FINISH_TIME_MAX_LEN; }
	bool RemoveTask(unsigned long ulID);
};

#pragma pack()

inline void ActiveTaskList::ClearTask(TaskInterface* pTask, ActiveTaskEntry* pEntry, bool bRemoveItem)
{
	RecursiveClearTask(pTask, pEntry, bRemoveItem, true, true);
	RealignTask(pEntry, 0);
}

inline void ActiveTaskList::ClearChildrenOf(TaskInterface* pTask, ActiveTaskEntry* pParent)
{
	while (pParent->m_ChildIndex != 0xff)
	{
		ActiveTaskEntry& ChildEntry = m_TaskEntries[pParent->m_ChildIndex];
		RecursiveClearTask(pTask, &ChildEntry, true, true, false);
		RealignTask(&ChildEntry, 0);
	}
}

#endif
