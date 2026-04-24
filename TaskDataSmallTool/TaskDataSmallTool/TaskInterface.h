//
//Just for compile usage!!!!
//
#ifndef __TASK_INTERFACE__
#define __TASK_INTERFACE__
#include "stdafx.h"


///////////////////////////////////////////////////////////////////////////////
#define TASK_PREREQU_FAIL_INDETERMINATE	1
#define TASK_PREREQU_FAIL_NOT_ROOT		2
#define TASK_PREREQU_FAIL_SAME_TASK		3
#define TASK_PREREQU_FAIL_NO_SPACE		4
#define TASK_PREREQU_FAIL_FULL			5
#define TASK_PREREQU_FAIL_CANT_REDO		6
#define TASK_PREREQU_FAIL_BELOW_LEVEL	7
#define TASK_PREREQU_FAIL_ABOVE_LEVEL	8
#define TASK_PREREQU_FAIL_NO_ITEM		9
#define TASK_PREREQU_FAIL_BELOW_REPU	10
#define TASK_PREREQU_FAIL_NOT_IN_CLAN	11
#define TASK_PREREQU_FAIL_WRONG_GENDER	12
#define TASK_PREREQU_FAIL_NOT_IN_OCCU	13
#define TASK_PREREQU_FAIL_WRONG_PERIOD	14
#define TASK_PREREQU_FAIL_PREV_TASK		15
#define TASK_PREREQU_FAIL_MAX_RCV		16
#define TASK_PREREQU_FAIL_NO_DEPOSIT	17
#define TASK_PREREQU_FAIL_NO_TASK		18
#define TASK_PREREQU_FAIL_NOT_CAPTAIN	19
#define TASK_PREREQU_FAIL_ILLEGAL_MEM	20
#define TASK_PREREQU_FAIL_WRONG_TIME	21
#define TASK_PREREQU_FAIL_NO_SUCH_SUB	22
#define TASK_PREREQU_FAIL_MUTEX_TASK	23
#define TASK_PREREQU_FAIL_NOT_IN_ZONE	24
#define TASK_PREREQU_FAIL_WRONG_SUB		25
#define TASK_PREREQU_FAIL_OUTOF_DIST	26
#define TASK_PREREQU_FAIL_GIVEN_ITEM	27
#define TASK_PREREQU_FAIL_LIVING_SKILL	28
#define TASK_PREREQU_FAIL_SPECIAL_AWARD	29
#define TASK_PREREQU_FAIL_PK_VALUE		30
#define TASK_PREREQU_FAIL_GM			31
#define TASK_PREREQU_FAIL_FRIENDSHIP	32
#define	TASK_PREREQU_FAIL_TITLE			33
#define	TASK_PREREQU_FAIL_CONTRIBUTION	34
#define	TASK_PREREQU_FAIL_NOT_FAMILY	35
#define	TASK_PREREQU_FAIL_NOT_HEADER	36
#define	TASK_PREREQU_FAIL_F_SKILL_LEV	37
#define	TASK_PREREQU_FAIL_F_SKILL_PROF	38
#define	TASK_PREREQU_FAIL_RECORD		39
#define TASK_PREREQU_FAIL_TALISMAN_VAL	40
#define	TASK_PREREQU_FAIL_FINISH_COUNT	41
#define TASK_PREREQU_FAIL_BATTLE_SCORE	42
#define TASK_PREREQU_FAIL_GLOBAL_COUNT	43
#define TASK_PREREQU_FAIL_LIFEAGAIN_CNT 44
#define TASK_PREREQU_FAIL_LIFEAGAIN_OCC 45
#define TASK_PREREQU_FAIL_PET           46
#define TASK_PREREQU_FAIL_NOT_MASTER    47
#define TASK_PREREQU_FAIL_NOT_PRENTICE  48
#define TASK_PREREQU_FAIL_NOT_P_IN_TEAM 49
#define TASK_PREREQU_FAIL_MASTER_MORAL  53
#define TASK_PREREQU_FAIL_CULT          54
#define TASK_PREREQU_FAIL_TEAM_NUM      55
#define TASK_PREREQU_FAIL_PRENTICE_LEV  56
#define TASK_PREREQU_FAIL_CONSUME_TREAS 57
#define TASK_PREREQU_FAIL_GLOBAL_EXP    58
#define TASK_PREREQU_FAIL_EXP_RUN       59
#define TASK_PREREQU_FAIL_ACCOMP		60
#define TASK_PREREQU_FAIL_TRANSFORM     61
#define TASK_PREREQU_FAIL_NO_REINFORCE_ITEM	62
#define	TASK_PREREQU_FAIL_OPEN_COUNT	63
#define TASK_PREREQU_FAIL_ACHIEVEMENT	64
#define TASK_PREREQU_FAIL_SUMMON_MONSTER 65
#define TASK_PREREQU_FAIL_CIRCLEGROUP	66
#define TASK_PREREQU_FAIL_TERRITORY_SCORE	67
#define TASK_PREREQU_FAIL_IN_CROSS_SERVER	68
#define TASK_PREREQU_FAIL_CROSSSERVER_GLOBAL	69
#define TASK_PREREQU_FAIL_FENGSHEN_LVL_OR_EXP 70
#define TASK_PREREQU_FAIL_CREATE_ROLE_TIME		71

#define TASK_AWARD_FAIL_GIVEN_ITEM		50
#define TASK_AWARD_FAIL_NEW_TASK		51
#define TASK_AWARD_FAIL_BELOW_LEV		52
#define TASK_AWARD_FAIL_NO_MORE_DEITY_EXP		72		
#define TASK_AWARD_FAIL_INVALID_OCCUPATION		73		

#define TASK_TEAM_RELATION_MARRIAGE		1

// Task messages
#define TASK_MSG_NEW					1
#define TASK_MSG_SUCCESS				2
#define TASK_MSG_FAIL					3

#define TASK_MAX_SIMULTANEOUS_COUT		20
#define TASK_HIDDEN_COUNT               3
#define	TASK_MAX_DELIVER_COUNT			5
#define	TASK_STORAGE_COUNT				8
#define	TASK_STORAGE_LEN				10
#define	TASK_STORAGE_BUF_SIZE			(TASK_STORAGE_COUNT * TASK_STORAGE_LEN * sizeof(short))

#define TASK_ACTIVE_LIST_HEADER_LEN		12
#define TASK_ACTIVE_LIST_MAX_LEN		50
#define TASK_FINISHED_LIST_MAX_LEN		2040
#define TASK_DATA_BUF_MAX_LEN			32
#define TASK_FINISH_TIME_MAX_LEN		1700

#define TASK_ACTIVE_LIST_BUF_SIZE		(TASK_ACTIVE_LIST_MAX_LEN * TASK_DATA_BUF_MAX_LEN + TASK_ACTIVE_LIST_HEADER_LEN + TASK_STORAGE_BUF_SIZE)
#define TASK_FINISHED_LIST_BUF_SIZE		4096
#define TASK_GLOBAL_DATA_SIZE			256
#define TASK_FINISH_TIME_LIST_BUF_SIZE	20480

#define	TASK_MASK_KILL_MONSTER			0x00000001
#define TASK_MASK_COLLECT_ITEM			0x00000002
#define TASK_MASK_TALK_TO_NPC			0x00000004
#define TASK_MASK_REACH_SITE			0x00000008
#define TASK_MASK_ANSWER_QUESTION		0x00000010
#define TASK_MASK_TINY_GAME				0x00000020

#define MAX_MONSTER_WANTED				3	
#define MAX_ITEM_WANTED					8
#define MAX_ITEM_AWARD					32
#define TASK_ZONE_FRIENDSHIP_COUNT		32

#define MAX_MONSTER_SUMMONED_CAND		32 

#define TASK_MSG_CHANNEL_LOCAL			0
#define TASK_MSG_CHANNEL_WORLD			1
#define TASK_MSG_ChANNEL_FACTION        3
#define TASK_MSG_CHANNEL_BROADCAST		9


#define TASK_AWARD_MAX_CHANGE_VALUE     32
///////////////////////////////////////////////////////////////////////////////
struct task_team_member_info
{
	unsigned long	m_ulId;
	unsigned long   m_ulMasterID;
	unsigned long	m_ulLevel;
	unsigned long	m_ulOccupation;
	bool			m_bMale;
	unsigned long   m_ulLifeAgainCnt; 
	int				m_nFamilyId;
	unsigned long	m_ulWorldId;
	float			m_Pos[3];
};

struct special_award
{
	unsigned long	id;
	unsigned long	param;
};


struct TaskInterface
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

	virtual void NotifyClient(const void* pBuf, size_t sz) {}
	virtual void NotifyPlayer(unsigned long ulPlayerId, const void* pBuf, size_t sz) {}
	virtual float UnitRand() { return 1.0f; }
	virtual void GetTeamMemberPos(int nIndex, float pos[3]) {}

	static unsigned long GetCurTime();
	static void WriteLog(int nPlayerId, int nTaskId, int nType, const char* szLog);
	static bool CalcFileMD5(const char* szFile, unsigned char md5[16]);
	static int GetTimeZoneBias();
};

unsigned long TaskInterface::GetCurTime() { return 1; }
void TaskInterface::WriteLog(int nPlayerId,	int nTaskId, int nType, const char* szLog) {}

/*bool TaskInterface::CalcFileMD5(const char* szFile, unsigned char md5[16])
{
	
	do nothing!!!!
	just for compile!
	
	return true;
}*/

int TaskInterface::GetTimeZoneBias()
{
	return 1;
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


struct Task_Log_AwardItem
{
	unsigned long ulItemID;
	unsigned long ulItemCnt;
};

///////////////////////////////////////////////////////////////////////////////


#endif //__TASK_INTERFACE__