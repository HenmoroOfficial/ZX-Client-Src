#ifndef _ATASK_TEMPL_H_
#define _ATASK_TEMPL_H_

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <time.h>
#include <string>

#ifdef _ELEMENTCLIENT
	#include "LuaEvent.h"
	#include "../ZCommon/ExpTypes.h"
#elif defined LINUX
	#include "../template/exptypes.h"
#else
	#include "../ZElementData/exptypes.h"
#endif

#include "TaskInterface.h"
#include "TaskProcess.h"
#include "TaskAsyncData.h"
#include "TaskExpAnalyser.h"

#define	MAX_TASK_NAME_LEN		30
#define MAX_AWARD_NPC_NUM		8
#define MAX_PREM_TASK_COUNT		5
#define MAX_MUTEX_TASK_COUNT	5
#define MAX_TEAM_MEM_WANTED		8
#define MAX_TIMETABLE_SIZE		12
#define MAX_OCCUPATIONS			32
#define MAX_AWARD_SCALES		5
#define MAX_AWARD_CANDIDATES	16
#define MAX_LIVING_SKILLS		4
#define MAX_TITLE_WANTED		5
#define MAX_MPTASK_COUNT        8
#define MAX_AWARD_SEL_ROLE      32
#define MAX_PREM_SEL_CULT       8
#define MAX_PREM_ACCOMP_CNT     8
#define MAX_AWARD_MON_CTRL      8
#define MAX_KILLFAILMONSTERS	8
#define MAX_FAILITEMS			8
#define MAX_VARIABLE_NUM		3	//Added 2011-03-30

#define TASK_AWARD_TERMINATE_COUNT 8

#define TASK_GENDER_NONE		0
#define TASK_GENDER_MALE		1
#define TASK_GENDER_FEMALE		2 

#define TASK_MAX_PATH			260
#define MAX_TASK_EXP_LEN        64

#define INVALID_VAL	((unsigned long) -1)

#define TASK_MAX_LINE_LEN TASK_MAX_PATH

#define TASK_MAX_VALID_COUNT	6

enum DynTaskType
{
	enumDTTNone = 0,
	enumDTTSpecialAward,
	enumDTTNormal,
};

struct NPC_INFO
{
	unsigned long id;
	long map_id;
	short x;
	short y;
	short z;
};

struct storage_task_info
{
	float weight;
	bool refresh;
};

typedef unsigned short task_char;

#define STORAGE_TASK_TYPES	18

/*
 *	Server Notifications
 */

#define TASK_SVR_NOTIFY_NEW				1	// 新任务发放
#define TASK_SVR_NOTIFY_COMPLETE		2	// 任务完毕
#define TASK_SVR_NOTIFY_GIVE_UP			3	// 任务放弃
#define TASK_SVR_NOTIFY_MONSTER_KILLED	4	// 杀怪数量
#define TASK_SVR_NOTIFY_FINISHED		5	// 处于得到奖励状态
#define TASK_SVR_NOTIFY_ERROR_CODE		6	// 错误码
#define TASK_SVR_NOTIFY_FORGET_SKILL	7	// 遗忘生活技能
#define TASK_SVR_NOTIFY_DYN_TIME_MARK	8	// 动态任务时间标记
#define TASK_SVR_NOTIFY_DYN_DATA		9	// 动态任务数据
#define TASK_SVR_NOTIFY_SPECIAL_AWARD	10	// 特殊奖励信息
#define	TASK_SVR_NOTIFY_STORAGE			11	// 仓库数据
#define	TASK_SVR_NOTIFY_FINISH_COUNT	12	// 任务完成次数
#define TASK_SVR_NOTIFY_CLEAR_LIFEAGAIN_FIN_REC   13  // 清除完成列表中转生重置任务记录
#define TASK_SVR_NOTIFY_ITEMS_TO_GET    14  // 更新需要物品个数
#define TASK_SVR_NOTIFY_CAMERA_MOVE     15  // 摄像机移动
#define TASK_SVR_NOTIFY_RM_FINISH_TASK	16	// 从任务完成列表中删除某一任务
#define TASK_SVR_NOTIFY_ADD_FINISH_TASK	17	// 添加某一任务到完成任务列表中

/*
 *	Client Notifications
 */

#define TASK_CLT_NOTIFY_CHECK_FINISH		1
#define TASK_CLT_NOTIFY_CHECK_GIVEUP		2
#define TASK_CLT_NOTIFY_REACH_SITE			3
#define TASK_CLT_NOTIFY_AUTO_DELV			4
#define TASK_CLT_NOTIFY_MANUAL_TRIG			5
#define TASK_CLT_NOTIFY_FORCE_GIVEUP		6
#define TASK_CLT_NOTIFY_DYN_TIMEMARK		7
#define TASK_CLT_NOTIFY_DYN_DATA			8
#define TASK_CLT_NOTIFY_SPECIAL_AWARD		9
#define	TASK_CLT_NOTIFY_STORAGE				10
#define TASK_CLT_NOTIFY_OUT_ZONE			11
#define TASK_CLT_NOTIFY_GIVEUP_AUTO			12
#define TASK_CLT_NOTIFY_ENTER_ZONE			13
#define TASK_CLT_NOTIFY_LEAVE_SITE			14
#define TASK_CLT_NOTIFY_MANUAL_TRIG_STORAGE	15
#define	TASK_CLT_NOTIFY_RM_FINISH_TASK		150

/*
 *	Player Notifications
 */

#define TASK_PLY_NOTIFY_NEW_MEM_TASK	1	// 通知接收队员任务
#define TASK_PLY_NOTIFY_FORCE_FAIL		2	// 全队失败
#define TASK_PLY_NOTIFY_FORCE_SUCC		3	// 全队成功
#define	TASK_PLY_NOTIFY_LEAVE_FAMILY	4	// 通知队长队员退出家族
#define TASK_PLY_NOTIFY_M2P_NEW_TASK    5   // 通知接收徒弟任务
#define TASK_PLY_NOTIFY_P2M_REC_SUCC    6   // 通知师傅徒弟接收任务成功
#define TASK_PLY_NOTIFY_P2M_TASK_FAIL   7   // 通知师傅徒弟任务失败
#define TASK_PLY_NOTIFY_P2M_LEAVE_TEAM  8   // 通知师傅徒弟离开队伍

/*
 *	Global Data Reason
 */

#define TASK_GLOBAL_CHECK_RCV_NUM		1
#define TASK_GLOBAL_CHECK_COTASK		2
#define TASK_GLOBAL_CHECK_ADD_MEM		3
#define TASK_GLOBAL_CHECK_FINISH_COUNT	4
#define TASK_GLOBAL_ADD_FINISH_COUNT	5
#define TASK_GLOBAL_CLEAR_FINISH_COUNT	6
#define TASK_GLOBAL_CHECK_FINISH_COUNT_FOR_EXTRA_AWARD		7
#define TASK_GLOBAL_UPDATE_FINISH_COUNT	8

/*
 *	Cotask Condition
 */

#define COTASK_CORRESPOND				0
#define COTASK_ONCE						1

/* 完成方式 */
enum
{
	enumTMNone = 0,					// 无
	enumTMKillNumMonster,			// 杀数量怪
	enumTMCollectNumArticle,		// 获得数量道具
	enumTMTalkToNPC,				// 与特定NPC对话
	enumTMReachSite,				// 到达特定地点
	enumTMWaitTime,					// 等待特定时间
	enumTMAnswerQuestion,			// 选择问答
	enumTMTinyGame,					// 小游戏
	enumTMProtectNPC,				// 保护特定NPC
	enumTMNPCReachSite,				// NPC到达特定地点
	enumTMTitle,					// 收集称谓
	enumTMFixedTime,				// 特定时间完成
	enumTMAchievement,				// 成就满足一定值
	enumTMLeaveSite,				// 离开特定地点
	enumTMFriendNum,				// 好友人数达到一定值
};

/* 封神前提 */
enum
{
	enumPremFSAll = 0,			// 所有人均可领取
	enumPremFSNoneFengShenOnly,	// 仅非封神可以领取
	enumPremFSFengShenOnly,		// 仅封神可以领取
};


/* 完成条件 */
enum
{
	enumTFTDirect = 0,		// 直接完成
	enumTFTNPC
};

/* 奖励方式 */
enum
{
	enumTATNormal = 0,		// 普通
	enumTATEach,			// 按杀怪数目、获得物品乘
	enumTATRatio,			// 按时间比例
	enumTATItemCount,		// 按获得物品个数分不同档次
	enumTATFinishCount,		// 按完成次数分不同档次
};

/* 任务重复间隔 */
enum
{
	enumTAFNormal = 0,		// 普通
	enumTAFEachDay,			// 每天
	enumTAFEachWeek,		// 每周
	enumTAFEachMonth,		// 每月
	enumTAFEverySeconds,	// 每隔若干秒
};

/* 清空人数接收上限 */
enum
{
	enumClearEachDay = 0,   // 普通
	enumClearEachWeek,		// 每周
	enumClearEachMonth,		// 每月
	enumClearEverySeconds,	// 每隔若干秒
};


#pragma pack(1)

#define TASK_PACK_INFO_MAGIC	0x69340304
#define TASK_PACK_MAGIC			0x06934554

struct TASK_PACK_INFO
{
	unsigned long	magic;
	unsigned long	version;
	unsigned long	export_version;	
	unsigned long	item_count;
	unsigned long	pack_count;
};

struct TASK_PACK_HEADER
{
	unsigned long	magic;
	unsigned long	item_count;
};

struct task_md5
{
	unsigned char data[16];

	bool operator == (const unsigned char* m) const
	{
		for (int i = 0; i < 16; i++)
			if (data[i] != m[i])
				return false;

		return true;
	}

	bool operator != (const unsigned char* m) const
	{
		return !(*this == m);
	}
};

struct ZONE_VERT
{
	union
	{
		struct
		{
			float x;
			float y;
			float z;
		};

		float v[3];
	};

	bool great_than(const float v[3]) const { return x >= v[0] && y >= v[1] && z >= v[2]; }
	bool less_than (const float v[3]) const { return x <= v[0] && y <= v[1] && z <= v[2]; }
};

inline bool is_in_zone(
	const ZONE_VERT& _min,
	const ZONE_VERT& _max,
	const float pos[3])
{
	return _min.less_than(pos) && _max.great_than(pos);
}

struct MONSTER_WANTED
{
	unsigned long	m_ulMonsterTemplId;
	unsigned long	m_ulMonsterNum;
	unsigned long	m_ulDropItemId;
	unsigned long	m_ulDropItemCount;
	bool			m_bDropCmnItem;
	float			m_fDropProb;
	bool			m_bKillerLev;
};

/*
	dyn task version must be updated, if this struct has been changed
*/
struct ITEM_WANTED
{
	unsigned long			m_ulItemTemplId;
	bool					m_bCommonItem;
	unsigned long			m_ulItemNum;
	float					m_fProb;
	bool					m_bBind;
	long					m_lPeriod;
	bool					m_bTimetable;
	char					m_cDayOfWeek;
	char					m_cHour;
	char					m_cMinute;
	char					m_cRefineCond;
	unsigned long			m_ulRefineLevel;
};


struct MONSTER_SUMMONED
{
	unsigned long			m_ulMonsterTemplID;
	bool					m_bIsMonster;	// 怪物or矿
	unsigned long			m_ulMonsterNum;
	unsigned long			m_ulMapId;
	ZONE_VERT				m_SummonVert;
	long					m_lPeriod;
};

extern const unsigned long _race_occ_map[8];

struct TEAM_MEM_WANTED
{
	unsigned long	m_ulLevelMin;
	unsigned long	m_ulLevelMax;
	unsigned long	m_ulRace;
	unsigned long	m_ulOccupation;
	unsigned long	m_ulGender;
	unsigned long   m_ulLifeAgain; //转生次数
	bool			m_bSameFamily;
	unsigned long	m_ulMinCount;
	unsigned long	m_ulMaxCount;
	unsigned long	m_ulTask;

	bool IsMeetBaseInfo(const task_team_member_info* pInfo, int nFamilyId) const
	{
		if (m_ulLevelMin && pInfo->m_ulLevel < m_ulLevelMin)
			return false;

		if (m_ulLevelMax && pInfo->m_ulLevel > m_ulLevelMax)
			return false;

		if (m_ulRace)
		{
			if (pInfo->m_ulOccupation >= SIZE_OF_ARRAY(_race_occ_map))
				return false;

			if (_race_occ_map[pInfo->m_ulOccupation] != m_ulRace)
				return false;
		}
		else if (m_ulOccupation != INVALID_VAL && pInfo->m_ulOccupation != m_ulOccupation)
			return false;

#ifndef _TASK_CLIENT

		if (m_ulGender == TASK_GENDER_MALE   && !pInfo->m_bMale
		 || m_ulGender == TASK_GENDER_FEMALE && pInfo->m_bMale)
			return false;

		if (m_ulLifeAgain && pInfo->m_ulLifeAgainCnt < m_ulLifeAgain)
			return false;
#endif

		if (m_bSameFamily && (nFamilyId == 0 || pInfo->m_nFamilyId != nFamilyId))
			return false;

		return true;
	}

	bool IsMeetCount(unsigned long ulCount) const
	{
		if (m_ulMinCount && ulCount < m_ulMinCount) return false;

		if (m_ulMaxCount && ulCount > m_ulMaxCount) return false;

		return true;
	}

	void Init()
	{
		m_ulLevelMin	= 0;
		m_ulLevelMax	= 0;
		m_ulRace		= 0;
		m_ulOccupation	= INVALID_VAL;
		m_ulGender		= 0;
		m_ulLifeAgain   = 0;
		m_bSameFamily	= false;
		m_ulMinCount	= 0;
		m_ulMaxCount	= 0;
		m_ulTask		= 0;
	}

	bool IsValid() const
	{
		return !(
			m_ulLevelMin	== 0 &&
			m_ulLevelMax	== 0 &&
			m_ulRace		== 0 &&
			m_ulOccupation	== INVALID_VAL &&
			m_ulGender		== 0 &&
			m_ulLifeAgain   == 0 &&
			m_bSameFamily	== false &&
			m_ulMinCount	== 0 &&
			m_ulMaxCount	== 0 &&
			m_ulTask		== 0);
	}

	bool operator == (const TEAM_MEM_WANTED& src) const
	{
		return (
			m_ulLevelMin	== src.m_ulLevelMin &&
			m_ulLevelMax	== src.m_ulLevelMax &&
			m_ulRace		== src.m_ulRace &&
			m_ulOccupation	== src.m_ulOccupation &&
			m_ulGender		== src.m_ulGender &&
			m_ulLifeAgain   == src.m_ulLifeAgain &&
			m_bSameFamily	== src.m_bSameFamily &&
			m_ulMinCount	== src.m_ulMinCount &&
			m_ulMaxCount	== src.m_ulMaxCount &&
			m_ulTask		== src.m_ulTask
			);
	}
};

struct MASTER_PRENTICE_TASK
{
	unsigned long m_ulLevLimit;
	unsigned long m_ulTaskID;
};

struct MONSTER_CTRL
{
	long  lMonCtrlID;
	float fGetProb;
	bool  bOpenClose;
};

struct AWARD_ITEMS_CAND
{
	AWARD_ITEMS_CAND()
	{
		memset(this, 0, sizeof(*this));
	}
	~AWARD_ITEMS_CAND()
	{
		LOG_DELETE_ARR(m_AwardItems);
	}

	AWARD_ITEMS_CAND& operator = (const AWARD_ITEMS_CAND& src)
	{
		m_ulAwardItems		= src.m_ulAwardItems;
		m_ulAwardCmnItems	= src.m_ulAwardCmnItems;
		m_ulAwardTskItems	= src.m_ulAwardTskItems;
		m_bRandChoose		= src.m_bRandChoose;

		if (m_ulAwardItems)
		{
			m_AwardItems = new ITEM_WANTED[m_ulAwardItems];
			g_ulNewCount++;
		}
		else
			m_AwardItems = NULL;

		for (unsigned long i = 0; i < m_ulAwardItems; i++)
			m_AwardItems[i] = src.m_AwardItems[i];

		return *this;
	}

	unsigned long	m_ulAwardItems;
	unsigned long	m_ulAwardCmnItems;
	unsigned long	m_ulAwardTskItems;
	ITEM_WANTED*	m_AwardItems; //[MAX_ITEM_AWARD];
	bool			m_bRandChoose;

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		*p = m_bRandChoose;
		p++;

		char cnt = (char)m_ulAwardItems;
		*p = cnt;
		p++;

		size_t sz = sizeof(ITEM_WANTED) * cnt;
		if (sz)
		{
			memcpy(p, m_AwardItems, sz);
			p += sz;
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		m_bRandChoose = (*p != 0);
		p++;

		m_ulAwardItems = *p;
		p++;

		if (m_ulAwardItems)
		{
			m_AwardItems = new ITEM_WANTED[m_ulAwardItems];
			g_ulNewCount++;

			size_t sz = sizeof(ITEM_WANTED) * m_ulAwardItems;
			memcpy(m_AwardItems, p, sz);
			p += sz;

			for (unsigned long i = 0; i < m_ulAwardItems; i++)
			{
				if (m_AwardItems[i].m_bCommonItem)
					m_ulAwardCmnItems++;
				else
					m_ulAwardTskItems++;
			}
		}

		return p - pData;
	}
};

using namespace abase;

struct COMPARE_EXPRESSION
{
	vector<char>      strExpLeft;	
	TaskExpressionArr arrExpLeft;
	int				  nCompOper;
	vector<char>	  strExpRight;
	TaskExpressionArr arrExpRight;
};//挪动了一下该结构体的位置，为了使得AWARD_DATA也能适用。Modified 2011-03-02.

struct AWARD_DATA
{
	AWARD_DATA()
	{
		memset(this, 0, sizeof(*this));
		m_iTransformID = -1;
		m_iTransformLevel = -1;
		m_iTransformExpLevel = -1;
	}
	AWARD_DATA(const AWARD_DATA& src)
	{
		*this = src;
	}
	~AWARD_DATA()
	{
		LOG_DELETE_ARR(m_CandItems);
		LOG_DELETE(m_pAwardSpecifyRole);
		LOG_DELETE_ARR(m_pszParaExp);
		LOG_DELETE_ARR(m_pParaExpArr);
		LOG_DELETE_ARR(m_ExtraCandItems);
		LOG_DELETE_ARR(m_pwstrExtraTribute);
	}

	AWARD_DATA& operator = (const AWARD_DATA& src)
	{
		memcpy(this, &src, sizeof(*this));

		if (m_ulCandItems)
		{
			m_CandItems = new AWARD_ITEMS_CAND[m_ulCandItems];
			g_ulNewCount++;
		}
		else
			m_CandItems = NULL;

		for (unsigned long i = 0; i < m_ulCandItems; i++)
			m_CandItems[i] = src.m_CandItems[i];

		if (src.m_pAwardSpecifyRole)
		{
			m_pAwardSpecifyRole = new AWARD_DATA;
			*m_pAwardSpecifyRole = *src.m_pAwardSpecifyRole;
		}		

		if (m_ulParaExpSize)
		{
			m_pszParaExp = new char[m_ulParaExpSize];
			g_ulNewCount++;

			memcpy(m_pszParaExp, src.m_pszParaExp, m_ulParaExpSize);

			if (m_ulParaExpArrLen)
			{
				m_pParaExpArr = new TASK_EXPRESSION[m_ulParaExpArrLen];
				g_ulNewCount++;
				memcpy(m_pParaExpArr, src.m_pParaExpArr, m_ulParaExpArrLen*sizeof(TASK_EXPRESSION));
			}
		}	

		if (m_lChangeKeyArr.size())
		{
			memset(&m_lChangeKeyArr, 0, sizeof(m_lChangeKeyArr));
			memset(&m_lChangeKeyValueArr, 0, sizeof(m_lChangeKeyArr));
			memset(&m_bChangeTypeArr, 0, sizeof(m_lChangeKeyArr));

			m_lChangeKeyArr      = src.m_lChangeKeyArr;
			m_lChangeKeyValueArr = src.m_lChangeKeyValueArr;
			m_bChangeTypeArr     = src.m_bChangeTypeArr;
		}

		//Added 2011-03-04.
		if (m_ulExtraCandItems)
		{
			m_ExtraCandItems = new AWARD_ITEMS_CAND[m_ulExtraCandItems];
			g_ulNewCount++;
		}
		else
			m_ExtraCandItems = NULL;

		unsigned long k = 0;
		for(; k < m_ulExtraCandItems; k++)
		{
			m_ExtraCandItems[k] = src.m_ExtraCandItems[k];
		}

		//添加额外喊话奖励内容的字符串拷贝, Added 2011-04-02.
		//需要考虑跨平台的移植问题：在服务器端，wchar_t是四字节的，使用相关CRT库时需要小心 2011-04-08
		if ( src.m_pwstrExtraTribute )
		{
#ifdef _TASK_CLIENT
			m_pwstrExtraTribute = new task_char[wcslen((wchar_t*)(src.m_pwstrExtraTribute))+1];
			wcscpy( (wchar_t*)m_pwstrExtraTribute, (wchar_t*)(src.m_pwstrExtraTribute) );
#else
			//这是供服务器调用的代码，当前，其实服务器并不需要真正的字符串信息，因此在赋值重载时，
			//并不执行真正的拷贝，这样主要是不浪费服务器的计算时间，同时避免字符串的拷贝代码意想不到的
			//错误。如果今后在服务器必须使用该字符串，则需要添加新的代码
			m_pwstrExtraTribute = NULL;			
#endif
		}
		else
		{
			m_pwstrExtraTribute = NULL;
		}
		//Added end.

		//添加全局key/value的赋值 Added 2011-04-07.需要特别小心的地方。万恶的memcpy，需要引起注意
		memset(&m_GlobalCompareExpression, 0, sizeof(m_GlobalCompareExpression));
		m_GlobalCompareExpression.strExpLeft = src.m_GlobalCompareExpression.strExpLeft;
		m_GlobalCompareExpression.strExpRight = src.m_GlobalCompareExpression.strExpRight;
		m_GlobalCompareExpression.nCompOper = src.m_GlobalCompareExpression.nCompOper;
		m_GlobalCompareExpression.arrExpLeft = src.m_GlobalCompareExpression.arrExpLeft;
		m_GlobalCompareExpression.arrExpRight = src.m_GlobalCompareExpression.arrExpRight;
		//Added end.

		return *this;
	}

	unsigned long		m_ulGoldNum;
	unsigned long		m_ulExp;
	float               m_fExpCoef;
	float               m_fExpCoef2;
	float               m_fExpCoef3;
	unsigned long		m_ulNewTask;
	unsigned long       m_ulTerminateTaskCnt;
	unsigned long       m_ulTerminateTask[TASK_AWARD_TERMINATE_COUNT]; 
	unsigned long       m_ulCameraMove;
	unsigned long       m_ulCircleGroupPoint;
	unsigned long		m_ulSP;
	unsigned long		m_ulReputation;
	long				m_lContribution;
	int					m_nFamContrib;
	unsigned long		m_ulProsperity;
	long				m_lTitle;
	long				m_lPKValue;
	bool				m_bResetPKValue;
	bool				m_bDivorce;
	long				m_aFriendships[TASK_ZONE_FRIENDSHIP_COUNT];
	unsigned long       m_ulFriendshipResetSel;
	unsigned long		m_ulNewPeriod;
	unsigned long		m_ulNewRelayStation;
	unsigned long		m_ulStorehouseSize;
	unsigned long		m_ulFactionStorehouseSize;
	long				m_lInventorySize;
	long                m_lPocketSize;
	unsigned long		m_ulPetInventorySize;
	unsigned long		m_ulFuryULimit;
	bool				m_bSetProduceSkill;
	unsigned long		m_ulProduceSkillExp;
	unsigned long		m_ulNewProfession;
	unsigned long       m_ulLifeAgainCnt;     //转生次数
	unsigned long       m_ulLifeAgainFaction; //转生门派
	unsigned long		m_ulTransWldId;
	ZONE_VERT			m_TransPt;
	long				m_lMonsCtrl;
	bool				m_bTrigCtrl;
	long				m_lBuffId;
	long				m_lBuffLev;
	int					m_nFamilySkillProficiency;
	int					m_nFamilySkillLevel;
	int					m_nFamilySkillIndex;
	int					m_nFamilyMonRecordIndex;
	int					m_nFamilyValueIndex;
	int					m_nFamilyValue;
	bool				m_bSendMsg;
	int					m_nMsgChannel;
	unsigned long		m_ulClearCountTask;
	unsigned long		m_ulDoubleExpTime;

	unsigned long		m_ulCandItems;
	AWARD_ITEMS_CAND*	m_CandItems; //[MAX_AWARD_CANDIDATES];
	
	long                m_lMasterMoral;	   // 师德值
	bool                m_bOutMaster;      // 出师
    bool                m_bDeviateMaster;  // 叛师	
	bool                m_bOutMasterExp;   // 徒弟获取出师经验
	bool                m_bOutMasterMoral; // 师傅获取出师师德 

	// 直接给指定角色发奖
	bool                m_bAwardSpecifyRole;
	unsigned long       m_ulRoleSelected;
	AWARD_DATA          *m_pAwardSpecifyRole;
	// 设置造化值
	bool                m_bSetCult;
	unsigned long       m_ulSetCult;
	// 清空声望技能
	unsigned long       m_ulClearCultSkill;
	// 清空技能点
	bool                m_bClearSkillPoints;
	// 清空天书点
	bool                m_bClearBookPoints;
	// 按参数发放奖励表达式
	int                 m_nParaExpSel;
	unsigned long       m_ulParaExpSize;
	char*               m_pszParaExp;
	unsigned long       m_ulParaExpArrLen;
	TASK_EXPRESSION*    m_pParaExpArr;
	// 改变全局key/value
	vector<long>		m_lChangeKeyArr; //[TASK_AWARD_MAX_CHANGE_VALUE]
	vector<long>		m_lChangeKeyValueArr;
	vector<bool>		m_bChangeTypeArr;
	// 倍率全局变量
	long				m_lMultiGlobalKey;
	// 宋金战场
	int                 m_nSJBattleScore;
	bool                m_bResetSJBattleScore;
	// 清空非主线活动任务
	bool                m_bClearNoKeyActiveTask;
	// 怪物控制器
	unsigned long		m_ulMonCtrlCnt;
	MONSTER_CTRL		m_MonCtrl[MAX_AWARD_MON_CTRL];
	bool                m_bRanMonCtrl;
	// 形象
	int                 m_iTransformID;
	int                 m_iTransformDuration;
	int                 m_iTransformLevel;
	int                 m_iTransformExpLevel;
	bool				m_bTransformCover;

	// 封神
	unsigned long		m_ulFengshenExp;
	bool				m_bFengshenDujie;

	//奖励鸿利元宝和仙基（战场声望或者战场积分） Added 2011-02-18
	unsigned long		m_ulBonusNum;//鸿利元宝
	unsigned long		m_ulBattleScore;//仙基

	//额外奖励相关成员变量 Added 2011-03-01
	//额外奖励中的物品奖励。
	unsigned long		m_ulExtraCandItems;
	AWARD_ITEMS_CAND*	m_ExtraCandItems; //[MAX_AWARD_CANDIDATES];

	//额外奖励中开启/关闭怪物控制器。
	unsigned long		m_ulExtraMonCtrlCnt;
	MONSTER_CTRL		m_ExtraMonCtrl[MAX_AWARD_MON_CTRL];
	bool                m_bExtraRanMonCtrl;
	
	//额外奖励中开启/关闭系统喊话功能
	bool m_bSendExtraMsg;//是否开启喊话。
	int m_nExtraMsgChannel;//喊话的频道。
	task_char* m_pwstrExtraTribute;//喊话内容。

	//满足额外奖励的相关判断条件 Added 2011-03-02
	bool				m_bCheckGlobalFinishCount;//是否判断满足全服计数
	unsigned long		m_ulGlobalFinishCountPrecondition;//全服计数的前提条件(阈值)
	bool				m_bCheckGlobalCompareExpression;//是否判断全局表达式条件
	COMPARE_EXPRESSION  m_GlobalCompareExpression;//额外奖励的全局key/value表达式

	//变量值，目前仅仅作为全局key/value变量的key使用,暂时设置为MAX_VARIABLE_NUM = 3个 Added 2011-03-30
	long				m_lVariable[MAX_VARIABLE_NUM];
	
	bool HasAward() const
	{
		for (int i = 0; i < SIZE_OF_ARRAY(m_aFriendships); i++)
			if (m_aFriendships[i] > 0)
				return true;

		return m_ulGoldNum != 0
			|| m_ulExp != 0
			|| m_ulNewTask != 0
			|| m_ulSP != 0
			|| m_ulReputation != 0
			|| m_lContribution != 0
			|| m_nFamContrib != 0
			|| m_ulProsperity != 0
			|| m_lTitle != 0
			|| m_lPKValue != 0
			|| m_bResetPKValue
			|| m_bDivorce
			|| m_ulNewPeriod != 0
			|| m_ulNewRelayStation != 0
			|| m_ulStorehouseSize != 0
			|| m_ulFactionStorehouseSize != 0
			|| m_lInventorySize != 0
			|| m_lPocketSize != 0
			|| m_ulPetInventorySize != 0
			|| m_ulFuryULimit != 0
			|| m_bSetProduceSkill
			|| m_ulProduceSkillExp != 0
			|| m_ulNewProfession != 0
			|| m_ulLifeAgainCnt != 0
			|| m_ulLifeAgainFaction != 0
			|| m_ulCandItems != 0
			|| m_lBuffId != 0
			|| m_nFamilySkillProficiency != 0
			|| m_nFamilySkillLevel != 0
			|| m_nFamilyMonRecordIndex != 0
			|| m_nFamilyValue != 0
			|| m_bSendMsg
			|| m_ulDoubleExpTime
			|| m_ulClearCountTask != 0
			|| m_bOutMaster != 0
			|| m_bDeviateMaster != 0
			|| m_lMasterMoral
			|| m_nSJBattleScore
			|| m_iTransformID != -1
			|| m_iTransformLevel != -1
			|| m_iTransformExpLevel != -1
			|| m_iTransformDuration
			|| m_ulCameraMove != 0
			|| m_ulTerminateTaskCnt != 0
			|| m_ulCircleGroupPoint != 0
			|| m_ulFengshenExp != 0
			|| m_bFengshenDujie
			|| m_ulBonusNum != 0
			|| m_ulBattleScore != 0;
	}

	int MarshalBasicData(char* pData)
	{
		char* p = pData;

		long* _mask = (long*)p;
		*_mask = 0;
		p += 4;

		if (m_ulGoldNum)
		{
			*_mask |= 1;
			*(long*)p = m_ulGoldNum;
			p += sizeof(long);
		}

		if (m_ulExp)
		{
			*_mask |= 1 << 1;
			*(long*)p = m_ulExp;
			p += sizeof(long);
		}

		if (m_ulSP)
		{
			*_mask |= 1 << 2;
			*(long*)p = m_ulSP;
			p += sizeof(long);
		}

		if (m_ulReputation)
		{
			*_mask |= 1 << 3;
			*(long*)p = m_ulReputation;
			p += sizeof(long);
		}

		if (m_ulCandItems)
		{
			*_mask |= 1 << 4;

			char cnt = (char)m_ulCandItems;
			*p = cnt;
			p++;

			for (char i = 0; i < cnt; i++)
				p += m_CandItems[i].MarshalBasicData(p);
		}

		return p - pData;
	}

	int UnmarshalBasicData(const char* pData)
	{
		const char* p = pData;

		long _mask = *(long*)p;
		p += sizeof(long);

		if (_mask & 1)
		{
			m_ulGoldNum = *(long*)p;
			p += 4;
		}

		if (_mask & (1 << 1))
		{
			m_ulExp = *(long*)p;
			p += 4;
		}

		if (_mask & (1 << 2))
		{
			m_ulSP = *(long*)p;
			p += 4;
		}

		if (_mask & (1 << 3))
		{
			m_ulReputation = *(long*)p;
			p += 4;
		}

		if (_mask & (1 << 4))
		{
			m_ulCandItems = *p;
			p++;

			if (m_ulCandItems)
			{
				m_CandItems = new AWARD_ITEMS_CAND[m_ulCandItems];
				g_ulNewCount++;

				for (unsigned long i = 0; i < m_ulCandItems; i++)
					p += m_CandItems[i].UnmarshalBasicData(p);
			}
		}

		return p - pData;
	}
};

struct AWARD_RATIO_SCALE
{
	AWARD_RATIO_SCALE()
	{
		memset(this, 0, sizeof(*this));
	}
	~AWARD_RATIO_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	unsigned long	m_ulScales;
	float			m_Ratios[MAX_AWARD_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_SCALES];

	AWARD_RATIO_SCALE& operator = (const AWARD_RATIO_SCALE& src)
	{
		m_ulScales = src.m_ulScales;

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}
		else
			m_Awards = NULL;

		for (unsigned long i = 0; i < m_ulScales; i++)
		{
			m_Ratios[i] = src.m_Ratios[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned long i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};

struct AWARD_ITEMS_SCALE
{
	AWARD_ITEMS_SCALE()
	{
		memset(this, 0, sizeof(*this));
	}
	~AWARD_ITEMS_SCALE()
	{
		LOG_DELETE_ARR(m_Awards);
	}

	unsigned long	m_ulScales;
	unsigned long	m_ulItemId;
	unsigned long	m_Counts[MAX_AWARD_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_SCALES];

	AWARD_ITEMS_SCALE& operator = (const AWARD_ITEMS_SCALE& src)
	{
		m_ulScales = src.m_ulScales;
		m_ulItemId = src.m_ulItemId;

		if (m_ulScales)
		{
			m_Awards = new AWARD_DATA[m_ulScales];
			g_ulNewCount++;
		}
		else
			m_Awards = NULL;

		for (unsigned long i = 0; i < m_ulScales; i++)
		{
			m_Counts[i] = src.m_Counts[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned long i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};

struct AWARD_COUNT_SCALE
{
	AWARD_COUNT_SCALE()
	{
		memset(this, 0, sizeof(*this));
	}

	~AWARD_COUNT_SCALE()
	{
		delete[] m_Awards;
	}

	unsigned long	m_ulScales;
	unsigned long	m_Counts[MAX_AWARD_SCALES];
	AWARD_DATA*		m_Awards; //[MAX_AWARD_SCALES];

	AWARD_COUNT_SCALE& operator = (const AWARD_COUNT_SCALE& src)
	{
		m_ulScales = src.m_ulScales;

		if (m_ulScales)
			m_Awards = new AWARD_DATA[m_ulScales];
		else
			m_Awards = NULL;

		for (unsigned long i = 0; i < m_ulScales; i++)
		{
			m_Counts[i] = src.m_Counts[i];
			m_Awards[i] = src.m_Awards[i];
		}

		return *this;
	}

	bool HasAward() const
	{
		for (unsigned long i = 0; i < m_ulScales; i++)
			if (m_Awards[i].HasAward())
				return true;

		return false;
	}
};

struct FINISH_TASK_COUNT_INFO
{
	unsigned long task_id;
	unsigned short count;
};

enum task_tm_type
{
	enumTaskTimeDate = 0,
	enumTaskTimeMonth,
	enumTaskTimeWeek,
	enumTaskTimeDay
};

const long task_week_map[] =
{
	7,
	1,
	2,
	3,
	4,
	5,
	6
};

struct task_tm
{
	long year;
	long month;
	long day;
	long hour;
	long min;
	long wday;

	bool after(const tm* _tm) const
	{
		if (year < _tm->tm_year + 1900) return false;
		if (year > _tm->tm_year + 1900) return true;

		if (month < _tm->tm_mon + 1) return false;
		if (month > _tm->tm_mon + 1) return true;

		if (day < _tm->tm_mday) return false;
		if (day > _tm->tm_mday) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before(const tm* _tm) const
	{
		if (year > _tm->tm_year + 1900) return false;
		if (year < _tm->tm_year + 1900) return true;

		if (month > _tm->tm_mon + 1) return false;
		if (month < _tm->tm_mon + 1) return true;

		if (day > _tm->tm_mday) return false;
		if (day < _tm->tm_mday) return true;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_month(const tm* _tm, bool bLastDay) const
	{
		// 如果当前天大于设定天则为false
		if (day < _tm->tm_mday) return false;

		// 如果当前天是非最后一天，且小于设定天，则返回true
		if (!bLastDay && day > _tm->tm_mday) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_month(const tm* _tm, bool bLastDay) const
	{
		// 如果当前天大于设定天则为true
		if (day < _tm->tm_mday) return true;

		// 如果当前天是非最后一天，且小于设定天，则返回false
		if (!bLastDay && day > _tm->tm_mday) return false;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_week(const tm* _tm) const
	{
		long w = task_week_map[_tm->tm_wday];

		if (wday < w) return false;
		if (wday > w) return true;

		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_week(const tm* _tm) const
	{
		long w = task_week_map[_tm->tm_wday];

		if (wday > w) return false;
		if (wday < w) return true;

		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}

	bool after_per_day(const tm* _tm) const
	{
		if (hour < _tm->tm_hour) return false;
		return hour > _tm->tm_hour || min > _tm->tm_min;
	}

	bool before_per_day(const tm* _tm) const
	{
		if (hour > _tm->tm_hour) return false;
		return hour < _tm->tm_hour || min <= _tm->tm_min;
	}
};

inline bool judge_time_date(const task_tm* tmStart, const task_tm* tmEnd, unsigned long ulCurTime, task_tm_type tm_type)
{
	tm _time, _time_tomorrow;
	unsigned long ulTimeTomorrow;
	bool last_day;

#ifdef _TASK_CLIENT
	ulCurTime -= unsigned long(TaskInterface::GetTimeZoneBias() * 60);

	if ((long)(ulCurTime) < 0)
		ulCurTime = 0;

	_time = *gmtime((time_t*)&ulCurTime);
	ulTimeTomorrow = ulCurTime + 24 * 3600;
	_time_tomorrow = *gmtime((time_t*)&ulTimeTomorrow);
#else
	_time = *localtime((time_t*)&ulCurTime);
	ulTimeTomorrow = ulCurTime + 24 * 3600;
	_time_tomorrow = *localtime((time_t*)&ulTimeTomorrow);
#endif

	last_day = (_time.tm_mon != _time_tomorrow.tm_mon);

	switch (tm_type)
	{
	case enumTaskTimeDate:
		return tmStart->before(&_time) && tmEnd->after(&_time);
	case enumTaskTimeMonth:
		return tmStart->before_per_month(&_time, last_day) && tmEnd->after_per_month(&_time, last_day);
	case enumTaskTimeWeek:
		return tmStart->before_per_week(&_time) && tmEnd->after_per_week(&_time);
	case enumTaskTimeDay:
		return tmStart->before_per_day(&_time) && tmEnd->after_per_day(&_time);
	}

	return false;
}

#define MAX_SUB_TAGS 32

struct task_sub_tags
{
	union
	{
		unsigned short sub_task;
		unsigned char state;
	};

	unsigned char sz;
	unsigned char tags[MAX_SUB_TAGS];
	unsigned char cur_index; // for temporary use, dont take into account

	size_t get_size() const { return sz + 3; }
	bool valid_size(size_t _sz) const
	{
		if (_sz < 3) return false;
		return get_size() == _sz;
	}
};

struct task_notify_base
{
	unsigned char	reason;
	unsigned short	task;
};

struct task_player_notify : public task_notify_base
{
	unsigned long	param;
};

struct svr_monster_killed : public task_notify_base
{
	unsigned long	monster_id;
	unsigned short	monster_num;
};

struct svr_items_to_get : public task_notify_base
{
	unsigned short  items_num;
};

struct svr_new_task : public task_notify_base
{
	unsigned long	cur_time;
	unsigned long	cap_task;
	task_sub_tags	sub_tags;

	inline void set_data(
		unsigned long _cur_time,
		unsigned long _cap_task,
		const task_sub_tags& _sub_tags)
	{
		cur_time	= _cur_time;
		cap_task	= _cap_task;
		memcpy(&sub_tags, &_sub_tags, _sub_tags.get_size());
	}

	inline void get_data(
		unsigned long& _cur_time,
		unsigned long& _cap_task,
		task_sub_tags& _sub_tags) const
	{
		_cur_time	= cur_time;
		_cap_task	= cap_task;
		memcpy(&_sub_tags, &sub_tags, sub_tags.get_size());
	}

	inline size_t get_size() const { return sizeof(task_notify_base) + 8 + sub_tags.get_size(); }

	inline bool valid_size(size_t sz) const
	{
		const size_t base_sz = sizeof(task_notify_base) + 8;
		if (sz <= base_sz) return false;
		return sub_tags.valid_size(sz - base_sz);
	}
};

struct svr_task_complete : public task_notify_base
{
	unsigned long	cur_time;
	task_sub_tags	sub_tags;

	inline void set_data(
		unsigned long _cur_time,
		const task_sub_tags& _sub_tags
		)
	{
		cur_time	= _cur_time;
		memcpy(&sub_tags, &_sub_tags, _sub_tags.get_size());
	}

	inline void get_data(
		unsigned long& _cur_time,
		task_sub_tags& _sub_tags) const
	{
		_cur_time	= cur_time;
		memcpy(&_sub_tags, &sub_tags, sub_tags.get_size());
	}

	inline size_t get_size() const { return sizeof(task_notify_base) + 4 + sub_tags.get_size(); }

	inline bool valid_size(size_t sz) const
	{
		const size_t base_sz = sizeof(task_notify_base) + 4;
		if (sz <= base_sz) return false;
		return sub_tags.valid_size(sz - base_sz);
	}
};

struct svr_task_err_code : public task_notify_base
{
	unsigned long err_code;
};

struct svr_task_dyn_time_mark : public task_notify_base
{
	unsigned long time_mark;
	unsigned short version;
};

struct svr_task_special_award : public task_notify_base
{
	special_award sa;
};

struct task_finish_count : public task_notify_base
{
	unsigned short count;
};

struct task_camera_move : public task_notify_base
{
	unsigned long ulIndex;
};

struct task_notify_storage : public task_notify_base
{
	unsigned long ulStorageId;
};

struct task_notify_add_finish_task : public task_notify_base
{
	bool bTaskSuccess;
};

using namespace abase;

struct ATaskTemplFixedData // No Virtual Functions, No Embedded Class
{
	ATaskTemplFixedData()
	{
		memset(this, 0, sizeof(*this));
	}

	~ATaskTemplFixedData()
	{
		LOG_DELETE(m_Award_S);
		LOG_DELETE(m_Award_F);
		LOG_DELETE(m_AwByRatio_S);
		LOG_DELETE(m_AwByRatio_F);
		LOG_DELETE(m_AwByItems_S);
		LOG_DELETE(m_AwByItems_F);
		LOG_DELETE(m_AwByCount_S);
		LOG_DELETE(m_AwByCount_F);
		LOG_DELETE_ARR(m_pszSignature);
		LOG_DELETE_ARR(m_tmStart);
		LOG_DELETE_ARR(m_tmEnd);
		LOG_DELETE_ARR(m_PremItems);
		LOG_DELETE_ARR(m_MonsterSummoned);
		LOG_DELETE_ARR(m_GivenItems);
		LOG_DELETE_ARR(m_TeamMemsWanted);
		LOG_DELETE_ARR(m_ItemsWanted);
		LOG_DELETE_ARR(m_MonsterWanted);
		delete[] m_PremTitles;
	}

	/* 任务对象的属性 */

	// 任务id
	unsigned long	m_ID;
	// 任务名称
	task_char		m_szName[MAX_TASK_NAME_LEN];
	// 任务署名
	bool			m_bHasSign;
	task_char*		m_pszSignature;
	// 任务类型
	unsigned long	m_ulType;
	// 时间限制
	unsigned long	m_ulTimeLimit;
	// 线下时间记入任务时间
	bool			m_bAbsTime;
	// 时间段个数
	unsigned long	m_ulTimetable;
	// 时间方式
	char			m_tmType[MAX_TIMETABLE_SIZE];
	// 发放起始时间
	task_tm*		m_tmStart; //[MAX_TIMETABLE_SIZE];
	// 发放终止时间
	task_tm*		m_tmEnd; //[MAX_TIMETABLE_SIZE];
	// 发放频率
	long			m_lAvailFrequency;
	//	时间间隔
	long			m_lTimeInterval;
	// 选择单个子任务执行
	bool			m_bChooseOne;
	// 随机旋转单个子任务执行
	bool			m_bRandOne;
	// 子任务是否有顺序
	bool			m_bExeChildInOrder;
	// 失败后是否认为父任务也失败
	bool			m_bParentAlsoFail;
	// 子任务成功后父任务成功
	bool			m_bParentAlsoSucc;
	// 能否放弃此任务
	bool			m_bCanGiveUp;
	// 是否可重复完成
	bool			m_bCanRedo;
	// 失败后是否可重新完成
	bool			m_bCanRedoAfterFailure;
	// 放弃清空任务
	bool			m_bClearAsGiveUp;
	// 是否需要记录
	bool			m_bNeedRecord;
	// 玩家被杀死是否认为失败
	bool			m_bFailAsPlayerDie;
	// 接受者上限
	unsigned long	m_ulMaxReceiver;
	// 清空接受者上限方式
	int            m_nClearReceiverType;
	// 按每隔一段时间方式清空的时间间隔
	long            m_lClearReceiverTimeInterval;
	// 发放区域
	bool			m_bDelvInZone;
	unsigned long	m_ulDelvWorld;
	ZONE_VERT		m_DelvMinVert;
	ZONE_VERT		m_DelvMaxVert;
	// 传送到特定点
	bool			m_bTransTo;
	unsigned long	m_ulTransWldId;
	ZONE_VERT		m_TransPt;
	// 怪物控制器
	long			m_lMonsCtrl;
	bool			m_bTrigCtrl;
	unsigned long		m_ulMonCtrlCnt;
	MONSTER_CTRL		m_MonCtrl[MAX_AWARD_MON_CTRL];
	bool                m_bRanMonCtrl;
	// 满足条件自动触发
	bool			m_bAutoDeliver;
	bool			m_bDeliverWindowMode;	// 自动触发模式：0：普通， 1：窗口
	// 死亡触发
	bool			m_bDeathTrig;
	// 手动触发
	bool			m_bManualTrig;
	// 关注任务
	bool			m_bMustShown;
	// 是否清除所得物品
	bool			m_bClearAcquired;
	// 推荐级别
	unsigned long	m_ulSuitableLevel;
	// 是否显示提示
	bool			m_bShowPrompt;
	// 是否关键任务
	bool			m_bKeyTask;
	// 发放NPC
	unsigned long	m_ulDelvNPC;
	// 发奖NPC
	unsigned long	m_ulAwardNPC;
	// 是否生活技能任务
	bool			m_bSkillTask;
	// 能否被查找
	bool			m_bCanSeekOut;
	// 能否显示指示方向
	bool			m_bShowDirection;
	// 仓库任务权重
	float			m_fStorageWeight;
	// 难度等级
	unsigned long	m_ulRank;
	// 结婚
	bool			m_bMarriage;
	// 家族共享
	bool			m_bSharedByFamily;
	// 完成次数
	bool			m_bRecFinishCount;
	bool			m_bRecFinishCountGlobal;
	unsigned long	m_ulMaxFinishCount;
	task_tm			m_FinishClearTime;
	int				m_nFinishTimeType;
	// 转生重置
	bool            m_bLifeAgainReset;
	// 下线失败
	bool            m_bFailAfterLogout;
	unsigned long	m_ulLogoutFailTime;
	// 任务失败时间
	bool			m_bAbsFail;
	task_tm			m_tmAbsFailTime;
	// 徒第任务
	bool		    m_bPrenticeTask;
	// 隐藏任务
	bool            m_bHidden;	
	// 超出特定区域任务失败
	bool			m_bOutZoneFail;
	unsigned long	m_ulOutZoneWorldID;
	ZONE_VERT		m_OutZoneMinVert;
	ZONE_VERT		m_OutZoneMaxVert;
	// 进入特定区域任务失败
	bool			m_bEnterZoneFail;
	unsigned long	m_ulEnterZoneWorldID;
	ZONE_VERT		m_EnterZoneMinVert;
	ZONE_VERT		m_EnterZoneMaxVert;

	// 清除玩家非法状态
	bool            m_bClearSomeIllegalStates;
	// 改变全局key/value
	vector<long>    m_lChangeKeyArr; //[TASK_AWARD_MAX_CHANGE_VALUE]
	vector<long>    m_lChangeKeyValueArr;
	vector<bool>    m_bChangeTypeArr;

	// 杀死指定id的怪物，任务失败
	bool			m_bKillMonsterFail;
	unsigned long	m_ulKillFailMonster;
	unsigned long	m_KillFailMonsters[MAX_KILLFAILMONSTERS];

	// 身上有指定物品即失败
	bool			m_bHaveItemFail;
	unsigned long	m_ulHaveItemFail;
	unsigned long	m_HaveFailItems[MAX_FAILITEMS];
	bool			m_bHaveItemFailNotTakeOff;	// 押金模式，为true则不收取
	// 身上没有指定物品即失败
	bool			m_bNotHaveItemFail;
	unsigned long	m_ulNotHaveItemFail;
	unsigned long	m_NotHaveFailItems[MAX_FAILITEMS];

	// 摄像机移动
	unsigned long	m_ulCameraMove;

	// 变量设置，当前仅供任务普通喊话奖励中的全局变量值Key/value设置使用,MAX_VARIABLE_NUM = 3. Added 2011-04-11.
	long			m_lVariables[MAX_VARIABLE_NUM];


	/* 开启条件 */

	// 等级条件
	unsigned long	m_ulPremise_Lev_Min;
	unsigned long	m_ulPremise_Lev_Max;
	bool			m_bShowByLev;
	// 法宝熔炼值
	int				m_nTalismanValueMin;
	int				m_nTalismanValueMax;
	// 所需道具
	unsigned long	m_ulPremItems;
	ITEM_WANTED*	m_PremItems; //[MAX_ITEM_WANTED];
	bool			m_bShowByItems;
	bool			m_bItemNotTakeOff;	// 押金模式，仅校验，不收取

	// 怪物归属功能（在任务开启时刷出归属怪物或矿物）
	unsigned long	m_ulPremMonsterSummoned;
	unsigned long	m_ulSummonMode;	// 归属模式：0:默认的普通怪 1:自己 2:师傅 3:徒弟 4:仙侣 5:队伍 6:队伍或自己 7:队伍与自己 8:不限
	bool			m_bRandSelMonster; // 随机选择召唤怪物
	MONSTER_SUMMONED* m_MonsterSummoned; //[MAX_MONSTER_SUMMONED_CAND]	

	// 发放道具
	unsigned long	m_ulGivenItems;
	unsigned long	m_ulGivenCmnCount;
	unsigned long	m_ulGivenTskCount;
	ITEM_WANTED*	m_GivenItems; //[MAX_ITEM_WANTED];
	// 称谓
	short*			m_PremTitles;
	unsigned long	m_ulPremTitleCount;
	// 押金
	unsigned long	m_ulPremise_Deposit;
	bool			m_bShowByDeposit;
	// 声望
	long			m_lPremise_Reputation;
	bool			m_bRepuDeposit;
	bool			m_bShowByRepu;
	// 帮派贡献度
	long			m_lPremise_Contribution;
	bool			m_bDepositContribution;
	// 家族贡献度
	int				m_nPremise_FamContrib;
	int				m_nPremFamContribMax;
	bool			m_bDepositFamContrib;
	// 战场积分
	int				m_nPremBattleScoreMin;
	int				m_nPremBattleScoreMax;
	bool			m_bDepositBattleScore;
	// 宋金战场积分
	int             m_nPremSJBattleScore;
	bool            m_bSJDepostiBattleScore;
	// 区域友好度
	long			m_Premise_Friendship[TASK_ZONE_FRIENDSHIP_COUNT];
	bool			m_bFriendshipDeposit;
	// 完成特定任务（成功？失败？），Task ID最高位1表示条件为失败，0为成功
	unsigned long	m_ulPremise_Task_Count;
	unsigned long	m_ulPremise_Tasks[MAX_PREM_TASK_COUNT];
	bool			m_bShowByPreTask;
	// 特定任务完成次数
	unsigned long	m_ulPremFinishTaskCount;
	FINISH_TASK_COUNT_INFO m_PremFinishTasks[MAX_PREM_TASK_COUNT];
	// 全局任务
	unsigned long	m_ulPremGlobalCount;
	unsigned long	m_ulPremGlobalTask;
	// 达到特定时期
	unsigned long	m_ulPremise_Period;
	bool			m_bShowByPeriod;
	// 帮派
	unsigned long	m_ulPremise_Faction;
	bool			m_bShowByFaction;
	// 帮主
	bool			m_bPremise_FactionMaster;
	// 性别
	unsigned long	m_ulGender;
	bool			m_bShowByGender;
	// 职业限制
	unsigned long	m_ulOccupations;
	unsigned long	m_Occupations[MAX_OCCUPATIONS];
	bool			m_bShowByOccup;
	// 夫妻
	bool			m_bPremise_Spouse;
	bool			m_bShowBySpouse;
	// 关联任务
	unsigned long	m_ulPremise_Cotask;
	unsigned long	m_ulCoTaskCond;
	// 互斥任务
	unsigned long	m_ulMutexTaskCount;
	unsigned long	m_ulMutexTasks[MAX_MUTEX_TASK_COUNT];
	// 生活技能级别
	int             m_nSkillLev; 
	long			m_lSkillLev[MAX_LIVING_SKILLS];
	// 宠物工坊
	int             m_nPetCon;
	int             m_nPetCiv;
	// 动态任务类型
	char			m_DynTaskType;
	// 特殊奖励号，用于运营的活动
	unsigned long	m_ulSpecialAward;
	// PK值
	long			m_lPKValueMin;
	long			m_lPKValueMax;
	// GM
	bool			m_bPremise_GM;

	// 组队信息
	bool			m_bTeamwork;		// 组队任务
	bool			m_bRcvByTeam;		// 必须组队接收
	bool			m_bSharedTask;		// 新队员分享任务
	bool			m_bSharedAchieved;	// 分享杀怪、物品数量
	bool			m_bCheckTeammate;	// 新队员分享任务检查队友位置
	float			m_fTeammateDist;	// 新队员分享任务队友距离平方值
	bool			m_bAllFail;			// 任意队员失败则全部失败
	bool            m_bCapChangeAllFail;// 队长变更则全队失败
	bool			m_bCapFail;			// 队长失败则全部失败
	bool			m_bCapSucc;			// 队长成功则全队成功
	float			m_fSuccDist;		// 成功时队员的距离
	bool			m_bAllSucc;			// 任意队员成功则全部成功
	bool			m_bDismAsSelfFail;	// 队员离队自身失败
	bool			m_bRcvChckMem;		// 接任务时检查队员位置
	float			m_fRcvMemDist;		// 接任务时队员距离平方值
	bool			m_bCntByMemPos;		// 队员在有效范围内杀怪有效
	float			m_fCntMemDist;		// 队员有效的范围
	unsigned long	m_ulTeamMemsWanted;
	TEAM_MEM_WANTED*m_TeamMemsWanted; //[MAX_TEAM_MEM_WANTED];
	bool			m_bShowByTeam;
	
	// 师徒
	bool m_bMaster;
	bool m_bPrentice;
	long m_lMasterMoral;
	bool m_bMPTask;
	bool m_bOutMasterTask;
	unsigned long        m_ulMPTaskCnt;
	MASTER_PRENTICE_TASK m_MPTask[MAX_MPTASK_COUNT];  

	// 家族信息
	bool			m_bInFamily;
	bool			m_bFamilyHeader;
	int				m_nFamilySkillLevelMin;
	int				m_nFamilySkillLevelMax;
	int				m_nFamilySkillProficiencyMin;
	int				m_nFamilySkillProficiencyMax;
	int				m_nFamilySkillIndex;
	int				m_nFamilyMonRecordIndex;
	int				m_nFamilyMonRecordMin;
	int				m_nFamilyMonRecordMax;
	int				m_nFamilyValueIndex;
	bool			m_bDepositFamilyValue;
	int				m_nFamilyValueMin;
	int				m_nFamilyValueMax;

	// 转生相关
	bool m_bCheckLifeAgain;
	bool m_bSpouseAgain;
	unsigned long m_ulLifeAgainCnt;
	int           m_nLifeAgainCntCompare;
	bool m_bLifeAgainOneOccup[MAX_OCCUPATIONS];
	bool m_bLifeAgainTwoOccup[MAX_OCCUPATIONS];
	bool m_bLifeAgainThrOccup[MAX_OCCUPATIONS];

	// 造化
	unsigned long   m_ulPremCult;
	// 消费元宝数
	int   m_nConsumeTreasureMin;
	int   m_nConsumeTreasureMax;

	// 前提全局表达式
	bool				m_bPremNeedComp;
	int					m_nPremExp1AndOrExp2;		// 0:|  1:&
	COMPARE_EXPRESSION  m_PremCompExp1;
	COMPARE_EXPRESSION  m_PremCompExp2;
	
	// 前提成就
	unsigned long	m_ulPremAccompCnt;
	unsigned long   m_aPremAccompID[MAX_PREM_ACCOMP_CNT];
	bool	        m_bPremAccompCond;

	// 前提变身
	int m_iPremTransformID;
	int m_iPremTransformLevel;
	int m_iPremTransformExpLevel;

	// 脚本触发开启
	bool			m_bScriptOpenTask;

	// 成就范围
	unsigned long	m_ulPremAchievementMin;
	unsigned long	m_ulPremAchievementMax;

	// 圈子积分
	unsigned long	m_ulPremCircleGroupMin;
	unsigned long	m_ulPremCircleGroupMax;

	// 领土积分
	unsigned long	m_ulPremTerritoryScoreMin;
	unsigned long	m_ulPremTerritoryScoreMax;

	// 封神条件
	int				m_nPremFengshenType;	// 0:所有人  1:仅未封神   2:仅封神
	unsigned long	m_ulPremFengshenLvlMin;
	unsigned long	m_ulPremFengshenLvlMax;
	bool			m_bExpMustFull;
	bool			m_bShowByFengshenLvl;

	// 角色创建的时间长度 Added 2011-04-11.
	unsigned long	m_ulCreateRoleTimeDuration;

	/* 任务完成的方式及条件 */

	unsigned long	m_enumMethod;
	unsigned long	m_enumFinishType;	

	/* 任务方式 */

	unsigned long	m_ulMonsterWanted;
	bool			m_bSummonMonsterMode;	// 归属怪物模式
	MONSTER_WANTED*	m_MonsterWanted; //[MAX_MONSTER_WANTED];

	unsigned long	m_ulItemsWanted;
	ITEM_WANTED*	m_ItemsWanted; //[MAX_ITEM_WANTED];
	unsigned long	m_ulGoldWanted;
	bool			m_bAutoMoveForCollectNumItems;	//是否需要为收集数量物品而启动自动寻径功能， Added 2011-10-08.

	unsigned long	m_ulNPCToProtect;
	unsigned long	m_ulProtectTimeLen;

	unsigned long	m_ulNPCMoving;
	unsigned long	m_ulNPCDestSite;

	ZONE_VERT		m_ReachSiteMin;
	ZONE_VERT		m_ReachSiteMax;
	ZONE_VERT		m_AutoMoveDestPos;				// 在启动自动寻径情况下，寻径的终点坐标 Added 2011-10-08.
	bool			m_bAutoMoveForReachFixedSite;	// 是否需要为到达特定地点而启动自动寻径功能 Added 2011-10-08.
	task_char		m_szAutoMoveDestPosName[MAX_TASK_NAME_LEN];// 在启动自动寻径情况下，目的地名称（仅用于UI显示,长度不超过30） Added 2011-10-08.
	unsigned long	m_ulReachSiteId;
	unsigned long	m_ulWaitTime;
	bool			m_bShowWaitTime;				// 是否需要启动显示计时功能 Added 2011-10-08.

	unsigned long	m_ulLeaveSiteId;	// 离开特定区域
	ZONE_VERT		m_LeaveSiteMin;	
	ZONE_VERT		m_LeaveSiteMax;


	unsigned long	m_ulTitleWantedNum;
	short			m_TitleWanted[MAX_TITLE_WANTED];

	// 成就达到特定值以上
	unsigned long	m_ulFinishAchievement;

	// 好友人数
	unsigned long	m_ulFriendNum;

	unsigned long	m_ulFinishLev;
	bool            m_bDisFinDlg;
	bool			m_bScriptFinishTask;	// 脚本触发完成

	int				m_iFixedType;	// 时间方式
	task_tm			m_tmFixedTime;	// 特定时间完成	

	// 完成全局表达式
	bool				m_bFinNeedComp;
	int					m_nFinExp1AndOrExp2;		// 0:|  1:&
	COMPARE_EXPRESSION  m_FinCompExp1;
	COMPARE_EXPRESSION  m_FinCompExp2;


	/* 任务结束后的奖励 */

	unsigned long	m_ulAwardType_S;
	unsigned long	m_ulAwardType_F;

	/* 普通和按每个方式 */
	AWARD_DATA* m_Award_S;	/* 成功 */
	AWARD_DATA* m_Award_F;	/* 失败 */

	/* 时间比例方式 */
	AWARD_RATIO_SCALE* m_AwByRatio_S;
	AWARD_RATIO_SCALE* m_AwByRatio_F;

	/* 按获得物比例方式 */
	AWARD_ITEMS_SCALE* m_AwByItems_S;
	AWARD_ITEMS_SCALE* m_AwByItems_F;

	/* 按完成次数方式 */
	AWARD_COUNT_SCALE* m_AwByCount_S;
	AWARD_COUNT_SCALE* m_AwByCount_F;

	/* 层次结构 */
	unsigned long	m_ulParent;
	unsigned long	m_ulPrevSibling;
	unsigned long	m_ulNextSibling;
	unsigned long	m_ulFirstChild;

	void* GetFixedDataPtr() { return (void*)this; }
	void Init();
	bool LoadFixedDataFromTextFile(FILE* fp, unsigned long ulVersion);
	bool LoadFixedDataFromBinFile(FILE* fp);
	bool HasFailAward() const { return m_Award_F->HasAward() || m_AwByRatio_F->HasAward() || m_AwByItems_F->HasAward() || m_AwByCount_F->HasAward(); }
	bool HasSuccAward() const {	return m_Award_S->HasAward() || m_AwByRatio_S->HasAward() || m_AwByItems_S->HasAward() || m_AwByCount_S->HasAward(); }
	bool CanRedo() const { return (!m_bNeedRecord || m_bCanRedo || m_bCanRedoAfterFailure); }

	void PushPtr(ATaskTemplFixedData& tmp)
	{
		tmp.m_Award_S			= m_Award_S;
		tmp.m_Award_F			= m_Award_F;
		tmp.m_AwByRatio_S		= m_AwByRatio_S;
		tmp.m_AwByRatio_F		= m_AwByRatio_F;
		tmp.m_AwByItems_S		= m_AwByItems_S;
		tmp.m_AwByItems_F		= m_AwByItems_F;
		tmp.m_AwByCount_S		= m_AwByCount_S;
		tmp.m_AwByCount_F		= m_AwByCount_F;
		tmp.m_pszSignature		= m_pszSignature;
		tmp.m_tmStart			= m_tmStart;
		tmp.m_tmEnd				= m_tmEnd;
		tmp.m_PremItems			= m_PremItems;
		tmp.m_MonsterSummoned	= m_MonsterSummoned;
		tmp.m_GivenItems		= m_GivenItems;
		tmp.m_TeamMemsWanted	= m_TeamMemsWanted;
		tmp.m_ItemsWanted		= m_ItemsWanted;
		tmp.m_MonsterWanted		= m_MonsterWanted;
		tmp.m_PremTitles		= m_PremTitles;
		m_Award_S = 0;
		m_Award_F = 0;
		m_AwByRatio_S = 0;
		m_AwByRatio_F = 0;
		m_AwByItems_S = 0;
		m_AwByItems_F = 0;
		m_AwByCount_S = 0;
		m_AwByCount_F = 0;
		m_pszSignature = 0;
		m_tmStart = 0;
		m_tmEnd = 0;
		m_PremItems = 0;
		m_MonsterSummoned = 0;
		m_GivenItems = 0;
		m_TeamMemsWanted = 0;
		m_ItemsWanted = 0;
		m_MonsterWanted = 0;
		m_PremTitles = 0;
	}

	void PopPtr(ATaskTemplFixedData& tmp)
	{
		m_Award_S			= tmp.m_Award_S;
		m_Award_F			= tmp.m_Award_F;
		m_AwByRatio_S		= tmp.m_AwByRatio_S;
		m_AwByRatio_F		= tmp.m_AwByRatio_F;
		m_AwByItems_S		= tmp.m_AwByItems_S;
		m_AwByItems_F		= tmp.m_AwByItems_F;
		m_AwByCount_S		= tmp.m_AwByCount_S;
		m_AwByCount_F		= tmp.m_AwByCount_F;
		m_pszSignature		= tmp.m_pszSignature;
		m_tmStart			= tmp.m_tmStart;
		m_tmEnd				= tmp.m_tmEnd;
		m_PremItems			= tmp.m_PremItems;
		m_MonsterSummoned	= tmp.m_MonsterSummoned;
		m_GivenItems		= tmp.m_GivenItems;
		m_TeamMemsWanted	= tmp.m_TeamMemsWanted;
		m_ItemsWanted		= tmp.m_ItemsWanted;
		m_MonsterWanted		= tmp.m_MonsterWanted;
		m_PremTitles		= tmp.m_PremTitles;
		tmp.m_Award_S = 0;
		tmp.m_Award_F = 0;
		tmp.m_AwByRatio_S = 0;
		tmp.m_AwByRatio_F = 0;
		tmp.m_AwByItems_S = 0;
		tmp.m_AwByItems_F = 0;
		tmp.m_AwByCount_S = 0;
		tmp.m_AwByCount_F = 0;
		tmp.m_pszSignature = 0;
		tmp.m_tmStart = 0;
		tmp.m_tmEnd = 0;
		tmp.m_PremItems = 0;
		tmp.m_MonsterSummoned = 0;
		tmp.m_GivenItems = 0;
		tmp.m_TeamMemsWanted = 0;
		tmp.m_ItemsWanted = 0;
		tmp.m_MonsterWanted = 0;
		tmp.m_PremTitles = 0;
	}

#ifdef _TASK_CLIENT
	void SaveFixedDataToTextFile(FILE* fp);
	void SaveFixedDataToBinFile(FILE* fp);
	void SetName(const wchar_t* szName)
	{
		memset(m_szName, 0, sizeof(m_szName));
		wcsncpy((wchar_t*)m_szName, szName, MAX_TASK_NAME_LEN-1);
	}
	const wchar_t* GetName() const { return (const wchar_t*)m_szName; }
	//Added 2011-10-09.
	void SetAutoMoveDestPosNameForReachFixedSite( const wchar_t* szName )
	{
		memset(m_szAutoMoveDestPosName, 0, sizeof(m_szAutoMoveDestPosName));
		wcsncpy((wchar_t*)m_szAutoMoveDestPosName, szName, MAX_TASK_NAME_LEN-1);
	}
	const wchar_t* GetAutoMoveDestPosNameForReachFixedSite() const { return (const wchar_t*)m_szAutoMoveDestPosName; }
	//Added end.
#endif
};

#pragma pack()

inline void ATaskTemplFixedData::Init()
{
	m_bCanRedo				= true;
	m_bParentAlsoFail		= true;
	m_bCanGiveUp			= true;
	m_bCanRedoAfterFailure	= true;
	m_bClearAcquired		= true;
	m_enumMethod			= enumTMTalkToNPC;
	m_enumFinishType		= enumTFTNPC;
	m_bShowByDeposit		= true;
	m_bShowByGender			= true;
	m_bShowByItems			= true;
	m_bShowByLev			= true;
	m_bShowByOccup			= true;
	m_bShowByPreTask		= true;
	m_bShowByRepu			= true;
	m_bShowByTeam			= true;
	m_bShowByFaction		= true;
	m_bShowByPeriod			= true;
	m_bShowPrompt			= true;
	m_ulDelvWorld			= 1;
	m_ulTransWldId			= 1;
	m_ulReachSiteId			= 1;
	m_bCanSeekOut			= true;
	m_bShowDirection		= true;
	m_fStorageWeight		= 1.0f;
	m_ulRank				= 1;
	m_lPKValueMin			= -999;
	m_lPKValueMax			= 999;
	m_iPremTransformLevel  = -1;
	m_iPremTransformExpLevel = -1;
	m_bScriptOpenTask		= false;
	m_bScriptFinishTask		= false;
	m_ulPremAchievementMin	= 0;
	m_ulPremAchievementMax	= 0;
	m_ulPremCircleGroupMin	= 0;
	m_ulPremCircleGroupMax	= 0;
	m_ulPremTerritoryScoreMin = 0;
	m_ulPremTerritoryScoreMax = 0;

	m_bShowByFengshenLvl	= true;
}

class ATaskTempl : public ATaskTemplFixedData
{
public:
	ATaskTempl() :
	m_pwstrDescript(0),
	m_pwstrOkText(0),
	m_pwstrNoText(0),
	m_pwstrTribute(0),
	m_pwstrHintText(0),
	m_pwstrCanDeliverText(0),
	m_pParent(0),
	m_pPrevSibling(0),
	m_pNextSibling(0),
	m_pFirstChild(0),
	m_uDepth(1),
	m_ulMask(0),
	m_nSubCount(0),
	m_uValidCount(0)
	{
		m_szFilePath[0] = '\0';
		memset(&m_DelvTaskTalk, 0, sizeof(talk_proc));
		memset(&m_UnqualifiedTalk, 0, sizeof(talk_proc));
		memset(&m_DelvItemTalk, 0, sizeof(talk_proc));
		memset(&m_ExeTalk, 0, sizeof(talk_proc));
		memset(&m_AwardTalk, 0, sizeof(talk_proc));

		m_Award_S = new AWARD_DATA;
		m_Award_F = new AWARD_DATA;

		m_AwByRatio_S = new AWARD_RATIO_SCALE;
		m_AwByRatio_F = new AWARD_RATIO_SCALE;

		m_AwByItems_S = new AWARD_ITEMS_SCALE;
		m_AwByItems_F = new AWARD_ITEMS_SCALE;

		m_AwByCount_S = new AWARD_COUNT_SCALE;
		m_AwByCount_F = new AWARD_COUNT_SCALE;
		
		//Canceled by Ford.W 2010-06-28.
/*
		//Added by Ford.W 2010-06-25.
#ifdef TASK_TEMPL_EDITOR //Modified by Ford.W 2010-06-28 from macro _TASK_CLIENT.
		m_bCopySubTask = false;
#endif
		//Added end.
*/
		//Canceled end. 2010-06-28.
	}

	virtual ~ATaskTempl()
	{
		if (m_pParent && m_pParent->m_pFirstChild == this)
			m_pParent->m_pFirstChild = m_pNextSibling;

		if (m_pPrevSibling) m_pPrevSibling->m_pNextSibling = m_pNextSibling;
		if (m_pNextSibling) m_pNextSibling->m_pPrevSibling = m_pPrevSibling;

		ATaskTempl* pChild = m_pFirstChild;
		while (pChild)
		{
			ATaskTempl* pNext = pChild->m_pNextSibling;
			LOG_DELETE(pChild);
			pChild = pNext;
		}

		LOG_DELETE_ARR(m_pwstrDescript);
		LOG_DELETE_ARR(m_pwstrOkText);
		LOG_DELETE_ARR(m_pwstrNoText);
		LOG_DELETE_ARR(m_pwstrTribute);
		LOG_DELETE_ARR(m_pwstrHintText);
		LOG_DELETE_ARR(m_pwstrCanDeliverText);
	}

public:
	// 任务描述
	task_char*		m_pwstrDescript;
	task_char*		m_pwstrOkText;
	task_char*		m_pwstrNoText;
	task_char*		m_pwstrTribute;
	task_char*		m_pwstrHintText;		// 悬浮描述
	task_char*		m_pwstrCanDeliverText;	// 可接任务描述

	char			m_szFilePath[TASK_MAX_PATH];

	/* 层次结构 */
	ATaskTempl*		m_pParent;
	ATaskTempl*		m_pPrevSibling;
	ATaskTempl*		m_pNextSibling;
	ATaskTempl*		m_pFirstChild;
	unsigned char	m_uDepth;
	unsigned long	m_ulMask;
	int				m_nSubCount;
	mutable unsigned char m_uValidCount;

	struct TASK_TIME_MARK
	{
		unsigned long	m_ulPlayerId;
		unsigned long	m_ulStartTime;

		void SetData(unsigned long ulPlayerId, unsigned long ulStartTime)
		{
			m_ulPlayerId	= ulPlayerId;
			m_ulStartTime	= ulStartTime;
		}
	};

	// Talk data
	talk_proc m_DelvTaskTalk;
	talk_proc m_UnqualifiedTalk;
	talk_proc m_DelvItemTalk;
	talk_proc m_ExeTalk;
	talk_proc m_AwardTalk;

protected:
	bool Load(FILE* fp, unsigned long ulVersion, bool bTextFile);

#ifdef _TASK_CLIENT
	void Save(FILE* fp, bool bTextFile);
	void SaveBinary(FILE* fp);

#endif

public:
	static ATaskTempl* CreateEmptyTaskTempl();

	void AddSubTaskTempl(ATaskTempl* pSub);
	void InsertTaskTemplBefore(ATaskTempl* pInsert);
	void SynchID()
	{
		m_ulParent = (m_pParent ? m_pParent->m_ID : 0);
		m_ulNextSibling = (m_pNextSibling ? m_pNextSibling->m_ID : 0);
		m_ulPrevSibling = (m_pPrevSibling ? m_pPrevSibling->m_ID : 0);
		m_ulFirstChild = (m_pFirstChild ? m_pFirstChild->m_ID : 0);
		if (m_pFirstChild) m_enumMethod = enumTMNone;
	}
	ATaskTempl* GetSubById(unsigned long ulId)
	{
		ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (pChild->m_ID == ulId) return pChild;
			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetConstSubById(unsigned long ulId) const
	{
		const ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (pChild->m_ID == ulId) return pChild;
			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetSubByIndex(int nIndex) const
	{
		const ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			if (nIndex-- == 0)
				return pChild;

			pChild = pChild->m_pNextSibling;
		}

		return NULL;
	}
	const ATaskTempl* GetTopTask() const
	{
		const ATaskTempl* pTop = this;
		const ATaskTempl* pParent = m_pParent;

		while (pParent)
		{
			pTop = pParent;
			pParent = pParent->m_pParent;
		}

		return pTop;
	}
	int GetSubCount() const
	{
		int nCount = 0;
		ATaskTempl* pChild = m_pFirstChild;

		while (pChild)
		{
			nCount++;
			pChild = pChild->m_pNextSibling;
		}

		return nCount;
	}
	void CheckDepth()
	{
		ATaskTempl* pChild = m_pFirstChild;
		unsigned char uDepth = 0;
		bool bMaxChildDepth = m_bExeChildInOrder || m_bChooseOne || m_bRandOne;

		while (pChild)
		{
			pChild->CheckDepth();

			if (bMaxChildDepth)
			{
				if (uDepth < pChild->m_uDepth)
					uDepth = pChild->m_uDepth;
			}
			else
				uDepth += pChild->m_uDepth;

			pChild = pChild->m_pNextSibling;
		}

		m_uDepth += uDepth;
	}

	void CheckMask();
	bool LoadFromTextFile(FILE* fp, bool bLoadDescript);
	bool LoadFromTextFile(const char* szPath, bool bLoadDescript = true);
	bool LoadBinary(FILE* fp);
	bool LoadFromBinFile(FILE* fp);
	bool LoadDescription(FILE* fp);
	bool LoadDescriptionBin(FILE* fp);
	bool LoadTribute(FILE* fp);
	bool LoadTributeBin(FILE* fp);
	bool LoadHintText(FILE* fp);
	bool LoadHintTextBin(FILE* fp);
	bool LoadCanDeliverText(FILE* fp);
	bool LoadCanDeliverTextBin(FILE* fp);
	int UnmarshalKillMonster(const char* pData);
	int UnmarshalCollectItems(const char* pData);
	int UnmarshalDynTask(const char* pData);
	int UnmarshalSpecialAwardData(const char* pData);

	// inline funcs
	unsigned long GetID() const { return m_ID; }
	unsigned long GetType() const { return m_ulType; }
	unsigned long HasSignature() const { return m_bHasSign; }
	const task_char* GetSignature() const { return m_pszSignature; }
	bool CanShowDirection() const { return m_bShowDirection; }
	void IncValidCount() const { m_uValidCount++; }
	void ClearValidCount() const { m_uValidCount = 0; }
	bool IsValidState() const { return m_uValidCount < TASK_MAX_VALID_COUNT; }
	unsigned long GetRank() const { return m_ulRank; }
	bool CheckFinishLev(TaskInterface* pTask) const { return m_ulFinishLev == 0 || pTask->GetPlayerLevel() >= m_ulFinishLev; }
	bool IsBeforeFixedTime(unsigned long ulCurTime) const;

	bool IsCrossServerTask() const { return ((m_ulType >= 14) && (m_ulType <= 17 )); } // 跨服任务 Modified 2011-02-21

#ifdef _TASK_CLIENT
	const wchar_t* GetDescription() const { assert(m_pwstrDescript); return (wchar_t*)m_pwstrDescript; }
	const wchar_t* GetOkText() const { assert(m_pwstrOkText); return (wchar_t*)m_pwstrOkText; }
	const wchar_t* GetNoText() const { assert(m_pwstrNoText); return (wchar_t*)m_pwstrNoText; }
	const wchar_t* GetTribute() const { assert(m_pwstrTribute); return (wchar_t*)m_pwstrTribute; }
	const wchar_t* GetHintText() const { assert(m_pwstrHintText); return (wchar_t*)m_pwstrHintText; }
	const wchar_t* GetCanDeliverText() const { assert(m_pwstrCanDeliverText); return (wchar_t*)m_pwstrCanDeliverText; }


	const talk_proc* GetDeliverTaskTalk() const { return &m_DelvTaskTalk; }
	const talk_proc* GetUnqualifiedTalk() const { return &m_UnqualifiedTalk; }
	const talk_proc* GetDeliverItemTalk() const { return &m_DelvItemTalk; }
	const talk_proc* GetUnfinishedTalk() const { return &m_ExeTalk; }
	const talk_proc* GetAwardTalk() const { return &m_AwardTalk; }

	unsigned long GetDeliverNPC() const { return m_ulDelvNPC; }
	unsigned long GetAwardNPC() const { return m_ulAwardNPC; }

	void SaveToTextFile(FILE* fp);
	bool SaveToTextFile(const char* szPath);
	void SaveToBinFile(FILE* fp) { SaveBinary(fp); }
	void SaveDescription(FILE* fp);
	void SaveDescriptionBin(FILE* fp);
	void SaveTribute(FILE* fp);
	void SaveTributeBin(FILE* fp);
	void SaveHintText(FILE* fp);
	void SaveHintTextBin(FILE* fp);
	void SaveCanDeliverText(FILE* fp);
	void SaveCanDeliverTextBin(FILE* fp);
	void SaveAllText(FILE* fp);
	int MarshalKillMonster(char* pData);
	int MarshalCollectItems(char* pData);
	int MarshalDynTask(char* pData);
	int MarshalSpecialAwardData(char* pData);
	bool CheckValid(FILE* fpLog);

	ATaskTempl& operator= (const ATaskTempl& src);

#endif
	const char* GetFilePath() const { return m_szFilePath; }
	void SetFilePath(const char* szPath) { strcpy(m_szFilePath, szPath); }

//Modified by Ford.W 2010-06-28.
#ifdef TASK_TEMPL_EDITOR
	//Added by Ford.W 2010-06-25.
	ATaskTempl* CreateNewSubTask( const ATaskTempl& srcTaskTempl  );
	void CopyAllSubTask( const ATaskTempl& srcTaskTempl );
	static bool m_bCopySubTask;
	//Added end.
#endif
//Modified end.


	// Process Part, all const functions

public:
	ActiveTaskEntry* DeliverTask(
		TaskInterface* pTask,
		ActiveTaskList* pList,
		ActiveTaskEntry* pEntry,
		unsigned long ulCaptainTask,
		unsigned long& ulMask,
		unsigned long ulCurTime,
		const ATaskTempl* pSubTempl,
		task_sub_tags* pSubTag,
		TaskGlobalData* pGlobal,
		unsigned char uParentIndex = (unsigned char)0xff
		) const;

	void RecursiveAward				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime, int nChoice, task_sub_tags* pSubTag) const;
	void RecursiveCheckTimeLimit	(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime) const;
	bool RecursiveCheckParent		(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry) const;	
	bool HasAllItemsWanted			(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const;
	bool HasAllTitlesWanted			(TaskInterface* pTask) const;
	bool HasFinishAchievement		(TaskInterface* pTask) const;
	void CalcAwardDataByRatio		(AWARD_DATA* pAward, ActiveTaskEntry* pEntry, unsigned long ulTaskTime, unsigned long ulCurTime) const;
	void CalcAwardDataByItems		(TaskInterface* pTask, AWARD_DATA* pAward, ActiveTaskEntry* pEntry) const;
	void CalcAwardDataByCount		(TaskInterface* pTask, AWARD_DATA* pAward, ActiveTaskEntry* pEntry) const;
	bool CanAwardItems				(TaskInterface* pTask, const AWARD_ITEMS_CAND* pAward) const;
	void CalcAwardItemsCount		(TaskInterface* pTask, const AWARD_ITEMS_CAND* pAward, unsigned long& ulCmnCount, unsigned long& ulTskCount) const;
	unsigned long RecursiveCalcAward(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime, int nChoice, unsigned long& ulCmnCount, unsigned long& ulTskCount, unsigned long& ulTopCount, unsigned char& uBudget) const;

	unsigned long CheckFnshLst		(TaskInterface* pTask, unsigned long ulCurTime) const;
	unsigned long CheckLevel		(TaskInterface* pTask) const;
	unsigned long CheckTalismanValue(TaskInterface* pTask) const;
	unsigned long CheckConsumeTreasure(TaskInterface* pTask) const;
	unsigned long CheckRepu			(TaskInterface* pTask) const;
	unsigned long CheckFriendship	(TaskInterface* pTask) const;
	unsigned long CheckDeposit		(TaskInterface* pTask) const;
	unsigned long CheckContrib		(TaskInterface* pTask) const;
	unsigned long CheckItems		(TaskInterface* pTask) const;
	unsigned long CheckFaction		(TaskInterface* pTask) const;
	unsigned long CheckGender		(TaskInterface* pTask) const;
	unsigned long CheckOccupation	(TaskInterface* pTask) const;
	unsigned long CheckPeriod		(TaskInterface* pTask) const;
	unsigned long CheckTeamTask		(TaskInterface* pTask) const;
	unsigned long CheckMutexTask	(TaskInterface* pTask, unsigned long ulCurTime) const;
	unsigned long CheckInZone		(TaskInterface* pTask) const;
	unsigned long CheckGivenItems	(TaskInterface* pTask) const;
	unsigned long CheckLivingSkill	(TaskInterface* pTask) const;
	unsigned long CheckPetConAndCiv (TaskInterface* pTask) const;
	unsigned long CheckPreTask		(TaskInterface* pTask) const;
	unsigned long CheckTimetable	(unsigned long ulCurTime) const;
	unsigned long CheckDeliverTime	(TaskInterface* pTask, unsigned long ulCurTime) const;
	unsigned long CheckSpecialAward	(TaskInterface* pTask) const;
	unsigned long CheckPKValue		(TaskInterface* pTask) const;
	unsigned long CheckGM			(TaskInterface* pTask) const;
	unsigned long CheckLifeAgain    (TaskInterface* pTask) const;
	unsigned long CheckTitle		(TaskInterface* pTask) const;
	unsigned long CheckSpouse		(TaskInterface* pTask) const;
	unsigned long CheckFamily		(TaskInterface* pTask) const;
	unsigned long CheckFinishCount	(TaskInterface* pTask) const;
	unsigned long CheckOpenCount	(TaskInterface* pTask) const;
	unsigned long CheckPremFinishCount	(TaskInterface* pTask) const;
	unsigned long CheckBattleScore	(TaskInterface* pTask) const;
	unsigned long CheckSJBattleScore(TaskInterface* pTask) const;
	unsigned long CheckMasterPrentice(TaskInterface* pTask) const;
	unsigned long CheckCultivation  (TaskInterface* pTask) const;
	unsigned long CheckAccomp		(TaskInterface* pTask) const;
	unsigned long CheckTransform    (TaskInterface* pTask) const;
	unsigned long CheckAchievement	(TaskInterface* pTask) const;
	unsigned long CheckTerritoryScore(TaskInterface* pTask) const;
	unsigned long CheckFengshen		(TaskInterface* pTask) const;
	unsigned long CheckCreateRoleTime( TaskInterface* pTask ) const;

#ifdef _TASK_CLIENT
	bool HasFinishFriendNum			(TaskInterface* pTask) const;
	unsigned long CheckCircleGroupPoints(TaskInterface* pTask) const;
#else
	bool RemovePrerequisiteItem		(TaskInterface* pTask) const;
	bool SummonPremMonsters			(TaskInterface* pTask) const;
	unsigned long DeliverByAwardData(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, const AWARD_DATA* pAward, unsigned long ulCurTime, int nChoice) const;
	void DeliverTeamMemTask			(TaskInterface* pTask, TaskGlobalData* pGlobal, unsigned long ulCurTime) const;
	bool CheckTeamMemPos			(TaskInterface* pTask, ActiveTaskEntry* pEntry, float fSqrDist) const;
	void DeliverGivenItems			(TaskInterface* pTask) const;
	const ATaskTempl* RandOneChild	(TaskInterface* pTask, int& nSub) const;
	void DeliverMasterPrenticeTask  (TaskInterface* pTask) const;	
	void DeliverAwardToSpecifyRole  (TaskInterface* pTask, const AWARD_DATA *pAward, unsigned long ulRoleSelected) const; // 给指定角色类型发奖励
	unsigned long CheckGlobalExpression(TaskInterface* pTask, const COMPARE_EXPRESSION& CompExp) const;
	unsigned long CheckGlobalExpressions(TaskInterface* pTask, bool bPrem) const;
	unsigned long DeliverExtraAward(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, const AWARD_DATA* pAward, unsigned long ulCurTime, unsigned long& ulRet, abase::vector<Task_Log_AwardItem>& LogItemArr, unsigned long ulMulti ) const;
#endif

public:
	unsigned long CheckBudget		(ActiveTaskList* pList) const;
	unsigned long CheckPrerequisite	(TaskInterface* pTask, ActiveTaskList* pList, unsigned long ulCurTime, bool bCheckPrevTask = true, bool bCheckTeam = true, bool bCheckBudge = true, bool bStorageRefresh = false) const;
	unsigned long CheckMarriage		(TaskInterface* pTask) const;
	void DeliverTaskNoCheck			(TaskInterface* pTask,	ActiveTaskList* pList, unsigned long ulCurTime) const;
	bool CanFinishTask				(TaskInterface* pTask, const ActiveTaskEntry* pEntry, unsigned long ulCurTime) const;
	void OnServerNotify				(TaskInterface* pTask, ActiveTaskEntry* pEntry, const task_notify_base* pNotify, size_t sz) const;
	void CalcAwardData				(TaskInterface* pTask, AWARD_DATA* pAward, ActiveTaskEntry* pEntry, unsigned long ulTaskTime, unsigned long ulCurTime) const;
	unsigned long CalcAwardMulti	(TaskInterface* pTask, ActiveTaskEntry* pEntry, unsigned long ulTaskTime, unsigned long ulCurTime) const;
	float CalcAwardByParaExp		(TaskInterface* pTask, const AWARD_DATA* pAward) const;
	unsigned long GetMemTaskByInfo	(const task_team_member_info* pInfo, int nFamilyId) const;
	bool IsAutoDeliver				() const;
	unsigned long GetSuitableLevel	() { return GetTopTask()->m_ulSuitableLevel; }
	bool CanShowPrompt				() const { return GetTopTask()->m_bShowPrompt; }
	bool IsKeyTask					() const { return GetTopTask()->m_bKeyTask; }
	unsigned long HasAllTeamMemsWanted(TaskInterface* pTask, bool bStrict) const;
	unsigned long RecursiveCheckAward(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime, int nChoice) const;
	bool NeedFamilySkillData() const { return m_nFamilySkillLevelMin || m_nFamilySkillLevelMax || m_nFamilySkillProficiencyMin || m_nFamilySkillProficiencyMax; }
	unsigned long CheckFamilySkill	(int nLev, int nValue) const;
	unsigned long CheckFamilyMonsterRecord(int nRecord) const;
	unsigned long CheckFamilyValue(int nValue) const;
	bool HasAllMonsterWanted		(TaskInterface* pTask, const ActiveTaskEntry* pEntry) const;
	unsigned long GetFinishCount	(TaskInterface* pTask) const;
	bool HasFailItem				(TaskInterface* pTask) const;
	bool NotHasFailItem				(TaskInterface* pTask) const;
	//仅仅用于额外奖励发放功能中。Added 2011-03-17.
	void CalcAwardDataIndex			(TaskInterface* pTask, const AWARD_DATA* pAward, ActiveTaskEntry* pEntry, unsigned long ulTaskTime, unsigned long ulCurTime, int& param) const;
	void GetAwardData				( int param, AWARD_DATA* pAward );
	//Added end.

#ifdef _TASK_CLIENT
	bool CanShowTask				(TaskInterface* pTask) const;
	bool HasShowCond()				const;
#else
	void NotifyClient				(TaskInterface* pTask, const ActiveTaskEntry* pEntry, unsigned char uReason, unsigned long ulCurTime, unsigned long ulParam = 0) const;
	bool CheckGlobalRequired		(TaskInterface* pTask, unsigned long ulSubTaskId, const TaskPreservedData* pPreserve, const TaskGlobalData* pGlobal) const;
	bool QueryGlobalRequired		(TaskInterface* pTask, unsigned long ulSubTaskId, unsigned short reason) const;
	bool CheckKillMonster			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulTemplId, unsigned long ulLev, bool bTeam, float fRand) const;
	void CheckCollectItem			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckCollectTitle			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckFinishAchievement		(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckMonsterKilled			(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bAtNPC, int nChoice) const;
	void CheckMining				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry) const;
	void CheckWaitTime				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime, bool bAtNPC, int nChoice) const;
	void CheckFixedTime				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, unsigned long ulCurTime, bool bAtNPC, int nChoice) const;
	void GiveUpOneTask				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bForce) const;
	void OnSetFinished				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, bool bNotifyMem = true) const;
	bool DeliverAward				(TaskInterface* pTask, ActiveTaskList* pList, ActiveTaskEntry* pEntry, int nChoice, bool bNotifyTeamMem = true, TaskGlobalData* pGlobal = NULL) const;
	void RemoveAcquiredItem			(TaskInterface* pTask, bool bClearTask, bool bSuccess) const;
	void TakeAwayGivenItems			(TaskInterface* pTask) const;
	bool OnDeliverTeamMemTask		(TaskInterface* pTask, TaskGlobalData* pGlobal) const;
	unsigned long CheckDeliverTask	(TaskInterface* pTask, unsigned long ulSubTaskId, TaskGlobalData* pGlobal, bool bNotifyErr = true, bool bMemTask = false, unsigned long ulCapId = 0) const;
	void AwardNotifyTeamMem			(TaskInterface* pTask, ActiveTaskEntry* pEntry) const;
	bool CheckFinishTimeState		(TaskInterface* pTask, TaskFinishTimeEntry* pEntry, unsigned long ulCurTime) const;
	unsigned long GetFinishTimeLimit(TaskInterface* pTask, unsigned long ulCurTime) const;
	bool OnDelvierMasterPrenticeTask(TaskInterface* pTask) const;
#endif

};

#include "TaskTempl.inl"

#endif
