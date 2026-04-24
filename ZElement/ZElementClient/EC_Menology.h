/********************************************************************
	created:	2006/08/16
	author:		kuiwu
	
	purpose:	get config info from lua script
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "hashtab.h"
#include "AWString.h"
#include "ScriptValue.h"
#include "vector.h"
#include "EC_Calendar.h"

class CLuaState;

enum
{
	MENOLOGY_TASKID_MAX = 200,
};

struct MenologyAward
{
	int  iId;
	AString strIcon;
	AWString strDesc;
};
struct FestivalTime
{
	CECCalendar tmStart;
	CECCalendar tmEnd;
};
struct MenologyInfo
{
	int				iId;		// 唯一表示
	int				iType;		// TYPE_NUM
	AString			strIcon1;	// 36*36
	AString			strIcon2;	// 48*48(dailypromp) or 花边(menology)
	AString			strIcon3;	// 90*90 menology 大图
	int				iRewardType;// gold, exp, item
	int				iMask;		// something not used yet
	AWString		strName;
	
	bool			bMenology;
	int				iMenologyLevel;
	AWString		strTime;
	tm				tmStartTime;
	tm				tmEndTime;
	BOOL			bNoTimeLimit;
	unsigned long	ulMinLevel;
	unsigned long	ulMaxLevel;
	AWString		strLevelInfo;
	bool			bReborn;
	BOOL			bNoCountLimit;
	int				iPermitCount;
	AWString		strCondition;
	
	bool			bWeekly;
	AWString		strWeekDays;
	AWString		strDisplayDays;
	bool			bDays[7];
	AWString		strLineInfo;
	abase::vector<int>	vecLine;
	AWString		strNPC;
	
	AWString		strAwardIconPath[3];
	AWString		strAwardDesc[3];
	UINT			uiCommend;
	bool			bTaskNpc;
	AWString		strNpcDesc;
	int				iNpcMapid;
	AWString		strNpcPos;
	bool			bIsTransport;
	
	BOOL			bLoginShow;
	BOOL			bSpecial;
	
	bool			bTemplateAward;
	abase::vector<MenologyAward> vecAward;
	abase::vector<FestivalTime> vecTimes;
	
	UINT			uiNumID;
	UINT			uiTaskID[MENOLOGY_TASKID_MAX];
	BOOL			bSuit[MENOLOGY_TASKID_MAX];
	UINT			iCurTaskShow;
	AWString		strDesc;
	
	int				iFinishState;
	int				iBossLv;
	AWString		strBossPeriod;
	AWString		strBossRecommendLv;
	AWString		strBossRecommendPlayerNum;
};

typedef abase::vector<MenologyInfo> MenologyInfoVector;

class CECMenology  
{
public:
	enum InfoType
	{
		TYPE_RECOMMAND,
		TYPE_WEEKLY,
		TYPE_BATTLEFIELD,
		TYPE_MONSTER,
		TYPE_FESTIVAL,
		TYPE_DAILYDUPLICATE,
		TYPE_DAILYDUPLICATE2,
		TYPE_DAILYDUPLICATE3,
		TYPE_DAILYDUPLICATE4,
		TYPE_DAILYDUPLICATE5,
		TYPE_DAILYDUPLICATE6,
		TYPE_DAILYDUPLICATE7,
		TYPE_DAILYDUPLICATE8,
		TYPE_DAILYDUPLICATE9,	// 以上属于占位
		TYPE_NUM,
	};
	enum SortType
	{
		ST_MENOLOGY,
		ST_NAME,
		ST_TIME,
		ST_LV_RCOMMAND,
		ST_LV_BOSS,
		ST_COUNT,
		ST_CONDICTION,
		ST_PERIOD,
		ST_REWARD,
		ST_BOSS_RECLV,
		ST_BOSS_LINE,
		ST_BOSS_PERIOD,
		ST_STATUS,
	};
	static CECMenology* GetInstance()
	{
		return &m_Menology;
	}
	~CECMenology();
	bool Init();
	void Release();
	MenologyInfoVector& GetInfo(int type) { assert(type>=TYPE_RECOMMAND && type<TYPE_NUM); return m_vecInfo[type]; }
	void SortInfo(InfoType it, SortType st, bool bInc=true);

private:
	CECMenology();
	static CECMenology m_Menology;

	MenologyInfoVector m_vecInfo[TYPE_NUM];

	CLuaState * m_pState;
	bool _LoadRecommandInfo();
	bool _LoadWeeklyInfo();
	bool _LoadBattleFieldInfo();
	bool _LoadMonsterInfo();
	bool _LoadFestivalInfo();
	bool _LoadDynamicElement();
};


