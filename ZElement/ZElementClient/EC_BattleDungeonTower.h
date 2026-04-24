// Filename	: EC_BattleDungeonTower.h
// Creator	: Fu Chonggang
// Date		: 2012.4.20
// Desc		: 人物爬塔副本相关数据

#pragma once

#ifndef __EC_BATTLEDUNGEON_TOWER__
#define __EC_BATTLEDUNGEON_TOWER__

#include "aassist.h"
#include "vector.h"
#include "hashmap.h"

struct TOWER_TRANSCRIPTION_CONFIG;

class CECBattleDungeonTower
{
public:
	struct AwardState
	{
		int	daily_award_delivered;		//该层的每日奖励是否领取 0：没有领取， 1：已经领取,	2:可以领取
		int	lifelong_award_delivered;	//该层得终生奖励是否领取 0：没有领取， 1：已经领取, 2:可以领取
	};
public:
	CECBattleDungeonTower();
	~CECBattleDungeonTower();
	
	bool Init();

	const TOWER_TRANSCRIPTION_CONFIG* GetConfig();
	int GetMapId();
	int GetTowerLevel() { return m_iTowerLevel; }
	int GetTodayStart() { return m_iTodayStart; }
	int GetTodayDest() { return m_iTodayDest; }
	int GetCurLevel() { return m_iCurLevel; }
	void SetCurLevel(int lv);
	int GetTimeUsed(int index);
	int GetMonster(int index);
	int GetMonsterCount();
	const wchar_t* GetMonsterDesc(int id);
	bool AwardCanDel(int lv, bool daily);

	void UpdateTowerData(void *p);
	void UpdateAwardState(void *p);
	void UpdateMonsters(void *p);
	void PassOneLevel(void *p);
	void AddMonster(int id);
protected:
	// 怪物图鉴
	abase::hash_map<int, ACString> m_ItemDesc;		// 图鉴id->描述
	abase::vector<int>		m_vecMonstersBook;

	int			m_iTowerLevel;		// 玩家当前爬到最大层 0-99
	int			m_iTodayStart;		// 今天选择爬拍层
	int			m_iTodayDest;		// 今天爬到了多少层
	int			m_iCurLevel;		// 当前正在打
	int			m_iTimeUse[10];		// 每10层的时间，没有时间则为0
	abase::vector<AwardState>	m_vecAwardStates;

	// 模板配置
	int			m_iMapId;
	
public:
	static const int	TOWER_TEMP_ID;
};

#endif //__EC_BATTLEDUNGEON_TOWER__