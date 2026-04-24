#ifndef _TASKASYNCDATA_H_
#define _TASKASYNCDATA_H_

#pragma pack(1)

enum
{
	enumTaskFamilyGetSkillInfo = 0,
	enumTaskFamilyUpdateSkillInfo,
	enumTaskFamilyGetMonsterInfo,
	enumTaskFamilyUpdateMonsterInfo,
	enumTaskFamilyGetSharedTasks,
	enumTaskFamilyAddSharedTask,
	enumTaskFamilyRemoveSharedTask,
	enumTaskFamilyGetFinishedTasks,
	enumTaskFamilyAddFinishedTask,
	enumTaskFamilyNotifyFinish,
	enumTaskFamilyGetMonsterRecord,
	enumTaskFamilyUpdateMonsterRecord,
	enumTaskFamilyGetCommonValue,
	enumTaskFamilyAddCommonValue,
};

enum
{
	enumTaskReasonDeliver = 0,
	enumTaskReasonUpdateSkill,
	enumTaskReasonGetSharedTasks,
	enumTaskReasonAddSharedTask,
	enumTaskReasonGetFinishedTasks,
	enumTaskReasonAddFinishedTask,
	enumTaskReasonRemoveSharedTask,
	enumTaskReasonGetMonsterInfo,
	enumTaskReasonUpdateMonsterInfo,
	enumTaskReasonNotifyFinishSucc,
	enumTaskReasonNotifyFinishFail,
	enumTaskReasonGetMonsterRecord,
	enumTaskReasonUpdateMonsterRecord,
	enumTaskReasonGetCommonValue,
	enumTaskReasonAddCommonValue,
};

enum
{
	enumTaskProtectNPCSuccess = 0,
	enumTaskProtectNPCDie,
	enumTaskProtectNPCTooFar,
	enumTaskProtectNPCTimeOut,
};

enum
{
	enumTaskFamilyValue1 = 0,	// ¡È∆¯÷µ
};

struct TaskPairData
{
	int key;
	int value1;
	int value2;

	TaskPairData() : key(0), value1(0), value2(0) {}
	TaskPairData(int _key, int _value1, int _value2) : key(_key), value1(_value1), value2(_value2) {}

	bool operator<(const TaskPairData & o) const
    {
		return key < o.key;
    }
};

class TaskFamilyAsyncData
{

public:

	struct _header
	{
		int nFamilyId;
		int nReason;
		int nTask;
		int nTaskReason;
		int nMainKey;
		int nPairCount;
	};

	struct _data
	{
		_header header;
		TaskPairData pairs[1];

		void SetData(int _nFamilyId, int _nReason, int _nTask, int _nTaskReason, int _nMainKey, const TaskPairData* _pPairs, int _nCount)
		{
			header.nFamilyId	= _nFamilyId;
			header.nReason		= _nReason;
			header.nTask		= _nTask;
			header.nTaskReason	= _nTaskReason;
			header.nMainKey		= _nMainKey;
			header.nPairCount	= _nCount;

			if (_nCount)
				memcpy(pairs, _pPairs, sizeof(TaskPairData) * _nCount);
		}
	};

public:

	TaskFamilyAsyncData()
	{
		m_pData = 0;
		m_nSize = 0;
	}

	TaskFamilyAsyncData(int nFamilyId, int nReason, int nTask, int nTaskReason, int nMainKey, const TaskPairData* pPairs, int nCount)
	{
		Marshal(nFamilyId, nReason, nTask, nTaskReason, nMainKey, pPairs, nCount);
	}

	~TaskFamilyAsyncData()
	{
		delete[] m_pData;
	}

protected:

	char* m_pData;
	int m_nSize;

public:

	void Marshal(int nFamilyId, int nReason, int nTask, int nTaskReason, int nMainKey, const TaskPairData* pPairs, int nCount)
	{
		m_nSize = sizeof(_header) + sizeof(TaskPairData) * nCount;
		m_pData = new char[m_nSize];
		((_data*)m_pData)->SetData(nFamilyId, nReason, nTask, nTaskReason, nMainKey, pPairs, nCount);
	}

	bool Unmarshal(const void* pData, int nSize)
	{
		if (nSize < (int)sizeof(_header))
			return false;

		if (nSize != int(sizeof(_header) + ((_header*)pData)->nPairCount * sizeof(TaskPairData)))
			return false;

		m_pData = new char[nSize];
		m_nSize = nSize;
		memcpy(m_pData, pData, nSize);
		return true;
	}

	void* GetBuffer() { return m_pData; }
	int GetSize() { return m_nSize; }
	_data* GetData() { return (_data*)m_pData; }
	_header* GetHeader() { return (_header*)m_pData; }
	TaskPairData& GetPair(int nIndex) { return GetData()->pairs[nIndex]; }
};

class TaskCircleGroupAsyncData
{
public:
	struct _data
	{
		int nCircleGroupId;
		int nGroupLevelMin;
		int nGroupLevelMax;
		int nPoints;
		int nTask;
		int nCurrOnlineCnt;
		int nMaxMemberCnt;

		void SetData(int CircleGroupId, int TaskID, int GroupLevelMin, int GroupLevelMax, int Points, int CurrOnlineCnt, int MaxMemberCnt)
		{
			nCircleGroupId = CircleGroupId;
			nGroupLevelMin = GroupLevelMin;
			nGroupLevelMax = GroupLevelMax;
			nPoints = Points;
			nTask = TaskID;
			nCurrOnlineCnt = CurrOnlineCnt;
			nMaxMemberCnt = MaxMemberCnt;
		}
	};

public:

	TaskCircleGroupAsyncData()
	{
		m_pData = 0;
		m_nSize = 0;
	}

	TaskCircleGroupAsyncData(int CircleGroupId, int TaskID, int GroupLevelMin=0, int GroupLevelMax=0, int Points=0, int CurrOnlineCnt=0, int MaxMemberCnt=0)
	{
		Marshal(CircleGroupId, TaskID,  GroupLevelMin, GroupLevelMax, Points, CurrOnlineCnt, MaxMemberCnt);
	}

	~TaskCircleGroupAsyncData()
	{
		delete[] m_pData;
	}

protected:

	char* m_pData;
	int m_nSize;

public:

	void Marshal(int CircleGroupId,  int TaskID, int GroupLevelMin, int GroupLevelMax, int Points, int CurrOnlineCnt, int MaxMemberCnt)
	{
		m_nSize = sizeof(_data);
		m_pData = new char[m_nSize];
		((_data*)m_pData)->SetData(CircleGroupId,TaskID, GroupLevelMin, GroupLevelMax, Points, CurrOnlineCnt, MaxMemberCnt);
	}

	bool Unmarshal(const void* pData, int nSize)
	{
		if (nSize != int(sizeof(_data)))
			return false;

		m_pData = new char[nSize];
		m_nSize = nSize;
		memcpy(m_pData, pData, nSize);
		return true;
	}

	void* GetBuffer() { return m_pData; }
	int GetSize() { return m_nSize; }
	_data* GetData() { return (_data*)m_pData; }
};

#pragma pack()

#endif
