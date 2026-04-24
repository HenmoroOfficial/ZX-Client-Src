//
//
#include "stdafx.h"
#include "TaskTemplMan.h"
#include "BaseDataIDMan.h"
#include "VssOperation.h"
#include "BaseDataTemplate.h"
#include "shlwapi.h"
#include "AVariant.h"
#include "gnoctets.h"
#include "gnsecure.h"


///////////////////////////////////////////////////////////////////////////////
extern CVssOperation g_VSS;
extern bool FileIsReadOnly(AString szFilePathName);

struct test_task : public TaskInterface
{
	virtual unsigned long GetPlayerLevel() { return 1; }
	virtual void* GetActiveTaskList()
	{
		static char _list[TASK_ACTIVE_LIST_BUF_SIZE];
		static int i = 0;
		if (i++ == 0) memset(&_list, 0, sizeof(_list));
		return &_list;
	}
	virtual void* GetFinishedTaskList()
	{
		static char _list[TASK_FINISHED_LIST_BUF_SIZE];
		static int i = 0;
		if (i++ == 0) memset(&_list, 0, sizeof(_list));
		return &_list;
	}
	virtual void* GetFinishedTimeList()
	{
		static char _list[TASK_FINISH_TIME_LIST_BUF_SIZE];
		static int i = 0;
		if (i++ == 0) memset(&_list, 0, sizeof(_list));
		return &_list;
	}
	virtual unsigned long* GetTaskMask()
	{
		static unsigned long _mask = 0;
		return &_mask;
	}
	virtual void DeliverGold(unsigned long ulGoldNum) {}
	virtual void DeliverExperience(unsigned long ulExp) {}
	virtual void DeliverSP(unsigned long ulSP) {}
	virtual void DeliverReputation(unsigned long ulReputation) {}
	virtual void DeliverRegionReputation(int index, long lValue) {}
	virtual void DeliverZoneFriendship(int index, long lValue) {}
	virtual int GetTaskItemCount(unsigned long ulTaskItem) { return 1; }
	virtual int GetCommonItemCount(unsigned long ulCommonItem) { return 1; }
	virtual bool IsInFaction() { return true; }
	virtual unsigned long GetGoldNum() { return 10000; }
	virtual void TakeAwayGold(unsigned long ulNum) {}
	virtual void TakeAwayTaskItem(unsigned long ulTemplId, unsigned long ulNum) {}
	virtual void TakeAwayCommonItem(unsigned long ulTemplId, unsigned long ulNum) {}
	virtual long GetReputation() { return 1; }
	virtual long GetRegionReputation(int index) { return 1; }
	virtual long GetZoneFriendship(int index) { return 1; }
	virtual unsigned long GetCurPeriod() { return 1; }
	virtual unsigned long GetPlayerId() { return 1; }
	virtual unsigned long GetPlayerRace() { return 1; }
	virtual unsigned long GetPlayerOccupation() { return 1; }
	virtual bool CanDeliverCommonItem(unsigned long) { return true; }
	virtual bool CanDeliverTaskItem(unsigned long) { return true; }
	virtual void DeliverCommonItem(unsigned long ulItem, unsigned long, bool) {}
	virtual void DeliverTaskItem(unsigned long ulItem, unsigned long) {}
	virtual int GetTeamMemberNum() { return 1; }
	virtual void GetTeamMemberInfo(int nIndex, task_team_member_info* pInfo)
	{
		pInfo->m_ulId		= 1;
		pInfo->m_ulLevel	= 1;
		pInfo->m_bMale		= true;
		pInfo->m_ulOccupation	= 1;
	}
	virtual bool IsDeliverLegal() { return true; }
	virtual bool IsCaptain() { return true; }
	virtual bool IsInTeam() { return true; }
	virtual unsigned long GetTeamMemberId(int nIndex) { return 1; }
	virtual bool IsMale() { return true; }
	virtual unsigned long GetPos(float pos[3]) { return 1;}
	virtual bool HasLivingSkill(unsigned long) { return false; }
	virtual long GetLivingSkillProficiency(unsigned long) { return 0; }
	virtual long GetLivingSkillLevel(unsigned long) { return 0; }
	virtual void GetSpecailAwardInfo(special_award* p) { p->id = 1; p->param = 0; }
	virtual int GetProduceSkillLev() { return 0; }
	virtual void SetProduceSkillFirstLev() {}
	virtual int GetProduceSkillExp() { return 0; }
	virtual void AddProduceSkillExp(int nExp) {}
	virtual long GetPKValue() { return 0; }
	virtual bool IsGM() { return true; }
	virtual bool HasTitle(short title) { return true; }
	virtual void DeliverPKValue(long lVal) {}
	virtual void ResetPKValue() {}
	virtual long GetFactionContribution() { return 1; }
	virtual void DeliverFactionContribution(long lVal) {}

#ifdef _TASK_CLIENT
	virtual void NotifyServer(const void* pBuf, size_t sz) {}
#else
	virtual void NotifyClient(const void* pBuf, size_t sz) {}
	virtual void NotifyPlayer(unsigned long ulPlayerId, const void* pBuf, size_t sz) {}
	virtual float UnitRand() { return 1.0f; }
	virtual void GetTeamMemberPos(int nIndex, float pos[3]) {}
#endif
};

#ifdef _TASK_CLIENT
	void TaskInterface::UpdateTaskUI() {}
	void TaskInterface::ShowTaskMessage(unsigned long ulTask, int reason) {}
	void TaskInterface::PopupTaskFinishDialog(unsigned long ulTaskId, const talk_proc* pTalk) {}
	int TaskInterface::GetTimeZoneBias() { return 0; }
#endif

unsigned long TaskInterface::GetCurTime() { return 1; }
void TaskInterface::WriteLog(int nPlayerId,	int nTaskId, int nType, const char* szLog) {}

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


// History Part
const TCHAR _his_file_format[] = _T("_gfx_op_%d.his");
extern CString g_strTempHisDir;

BOOL IsReadOnly(LPCTSTR szFile)
{
	DWORD dwFlag = GetFileAttributes(szFile);
	if (dwFlag == (DWORD)-1) return FALSE;
	return dwFlag & FILE_ATTRIBUTE_READONLY;
}

///////////////////////////////////////////////////////////////////////////////