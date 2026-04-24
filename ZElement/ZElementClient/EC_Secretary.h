/********************************************************************
	created:	2009/6/23
	author:		lihengju
	
	purpose:	use lua to contorl help system
	Copyright (C) 2006 - All Rights Reserved
*********************************************************************/

#pragma  once

#include "vector.h"
#include <AWString.h>
#include <ABaseDef.h>

class CLuaState;
#define FIRST_STATE_BLOCK 30		// 240个事件
#define SPECIAL_ID_BLOCK  20		// 160个特殊id

#define TRIGGER_OPEN_TASK 10
#define TRIGGER_COMPLETE_TASK 50

#define HELP_GUIDE_NUM 20  // 160个指引

class CECSecretary
{
public:		//	Types
	enum
	{

		FIRST_ENTER_GAME		,
		PLAYER_IS_MOVING		,
		PLAYER_ADJUST_ORIENT	,
		PLAYER_LOCK_VISION		,
		FIRST_EMOTION_ACTION	,
		FIRST_EMOTION_WORD		,
		FIRST_CAMERA_MODE		,
		FIRST_SAFELOCK		,
		FIRST_BOSSKEY		,
		FIRST_TALK_NPC		,
		FIRST_TASK_MAIN		,
		FIRST_TASK_TRAVEL		,
		FIRST_TASK_RISK		,
		FIRST_TASK_CLASSICAL	,
		FIRST_TASK_CHANLLENGE	,
		FIRST_TASK_KILL_MONSTER	,
		FIRST_TASK_FORGE		,
		FIRST_TASK_HERIT		,
		FIRST_TASK_ASCETIC		,
		FIRST_TASK_LUPIN		,
		FIRST_TASK_TEACHER		,
		FIRST_TASK_GUILD		,
		FIRST_TASK_TREASURE		,
		FIRST_TASK_ADVENTURE	,
		FIRST_TASK_LOVE		,
		FIRST_TASK_READ		,
		FIRST_OBJECT_PICK		,
		FIRST_TASK_FAIL		,
		FIRST_TASK_LIMIT		,
		FIRST_TASK_TRACK		,
		FIRST_MAP_AUTOTRACK		,
		FIRST_MAP_OPEN		,
		FIRST_TASK_FINISH		,
		FIRST_TASK_AWARD		,
		FIRST_TASK_SEARCH_LATEST,
		FIRST_BAG_OPEN		,
		FIRST_ITEM_EQUIP		,
		FIRST_MEDICINE_USE		,
		FIRST_ITEM_SPECIAL		,
		FIRST_BAG_NEATEN		,
		FIRST_BAG_EXPAND		,
		FIRST_ITEM_SPILT		,
		FIRST_BAG_SELL		,
		FIRST_EXCHANGE		,
		FIRST_CASH_EXCHANGE		,
		FIRST_DEPOT_OPEN		,
		FIRST_DEPOT_EXPAND		,
		FIRST_HORSE_EQUIP		,
		FIRST_ITEM_LOCK		,
		FIRST_BAG_PET		,
		FIRST_BAG_FASHDRESS		,
		FIRST_SKILL_SPECIAL		,
		FIRST_SKILL_POINT_ADD	,
		FIRST_SKILL_EDIT		,
		FIRST_SKILL_CLEAR		,
		FIRST_ATTACK		,
		FIRST_PK		,
		FIRST_PK_SETTING		,
		FIRST_PK_UP		,
		FIRST_PK_DOWN		,
		FIRST_BUFF		,
		FIRST_HP		,
		FIRST_DIE		,
		FIRST_DIE_PUNISH		,
		FIRST_REST		,
		FIRST_KILL_MONSTER		,
		FIRST_PICKUP		,
		FIRST_ABANDON		,
		FIRST_CHAT_OPEN		,
		FIRST_MONSTER_SPECIAL	,
		FIRST_ENTER_MAP_NEW		,
		FIRST_MAP_TRANSFER		,
		FIRST_TEAM_SETUP		,
		FIRST_TEAM_SETTING		,
		FIRST_BATTLE_GUILD		,
		FIRST_BATTLE_CITY		,
		FIRST_TALISMAN_MAIN		,
		FIRST_TALISMAN_VICE		,
		FIRST_TALISMAN_START		,
		FIRST_TALISMAN_UPDATE		,
		FIRST_TALISMAN_MEDIUM		,
		FIRST_TALISMAN_REFINE		,
		FIRST_TALISMAN_ENCHANT		,
		FIRST_TALISMAN_CLEAN		,
		FIRST_TALISMAN_COMBINE		,
		FIRST_FRIEND_ADD		,
		FIRST_HUG		,
		FIRST_MARRY		,
		FIRST_FAMILY_CREATE		,
		FIRST_FAMILY_JOIN		,
		FIRST_GUILD_CREATE		,
		FIRST_GUILD_JOIN		,
		FIRST_FRIEND_DELETE		,
		FIRST_BLACKLIST		,
		FIRST_ENEMY		,
		FIRST_STUDENT		,
		FIRST_STUDENT_OUT		,
		FIRST_TEACHER		,
		FIRST_STUDENT_ADD		,
		FIRST_FRIEND_SEEK		,
		FIRST_LUPIN_POINT		,
		FIRST_PET_GET		,
		FIRST_PET_SPECIAL		,
		FIRST_PET_USE		,
		FIRST_PET_EQUIPMENT		,
		FIRST_PET_EQUIPMENT_STRENGHTEN	,
		FIRST_PET_TAME		,
		FIRST_PET_COMBINE		,
		FIRST_PET_STAR_UP		,
		FIRST_PET_CHANGE_IMAGE	,
		FIRST_PET_CHANGE_NAME	,
		FIRST_PET_CHANGE_MODE	,
		FIRST_PET_FEED		,
		FIRST_PET_WEAK		,
		FIRST_PET_HUNGRY		,
		FIRST_PET_DIE		,
		FIRST_PET_LV_UP		,
		FIRST_PET_COLLECT		,
		FIRST_PET_PRODUCE		,
		FIRST_PET_CATCH		,
		FIRST_FLY_GET		,
		FIRST_FLY_MODE		,
		FIRST_FLY_FIGHT_MODE	,
		FIRST_FLY_LV_UP		,
		FIRST_FLY_SPEED_UP		,
		FIRST_ENCHASE_LV		,
		FIRST_ENCHASE_POINT		,
		FIRST_BLOOD_ENCHANSE	,
		FIRST_EQUIP_EXCHANGE	,
		FIRST_EQUIP_BREAK		,
		FIRST_EQUIP_HIDE		,
		FIRST_EQUIP_BIND		,
		FIRST_EQUIP_DEPART		,
		FIRST_EQUIP_DESTROY		,
		FIRST_EQUIP_SUIT		,
		FIRST_MAIL		,
		FIRST_DESC_JOIN		,
		LV_UP		,
		FIRST_QSHOP_OPEN		,
		QSHOP_ITEM_GET		,
		FIRST_RAFFLE		,
		FIRST_POPSHOP		,
		FIRST_CREDIT_ADD		,
		RANK_PERSON_DROPOUT		,
		RANK_PERSONALL_DROPOUT	,
		FIRST_HIDE_ELSEPLAYER,
		FIRST_DEITY_LEVELUP,
		FIRST_DEITY_POPHELP,
		FIRST_OPEN_RADIO,
		FIRST_ITEM_SPECIAL2,
		LV_UP2,
		FIRST_TALK_NPC2,
		FIRST_MAP_AUTOTRACK2,

		NUM_FIRST_STATES,
	};

	struct UserDefinedEvent
	{
		char	type;  
		char	week;
		bool	bEnable;
		int		start_time;
		int		end_time;
		char	content[32];   // 16个汉字
	};

public:		//	Constructor and Destructor

	CECSecretary();
	virtual ~CECSecretary();

public:		//	Attributes

public:		//	Operations
	bool Init();
	void Release();
	void SetLastEvent(int iEvent){ m_iLastEvent = iEvent; }
	int  GetLastEvent(){ return m_iLastEvent; }
	void SetEventState(int iEvent);
	bool CheckEventState(int iEvent);
	void ResetEventState(int iEvent);
	bool CheckSpecialState(int iIndex);
	void SetSpecialState(int iIndex);
	bool Tick(DWORD dwDeltaTime);
	void SaveStatesToServer();
	void LoadStatesFromServer(void* pDataBuf, int iSize);
	void SetURL(const char* url, const ACHAR* title = NULL);
	void SetEnable(bool bEnable){ m_bSecretaryEnable = bEnable; }
	bool IsEnable(){ return m_bSecretaryEnable; }
	void SetHelpGuideEnable(bool bEnable){m_bHelpGuideEnable = bEnable;}
	bool IsHelpGuideEnable(){return m_bHelpGuideEnable;}
	void SetReady(bool bReady){ m_bReady = bReady; }

	bool CheckOpenTaskState(int iIndex);
	bool CheckCompleteTaskState(int iIndex);
	void SetOpenTaskState(int iIndex);
	void SetCompleteTaskState(int iIndex);

	void SetGuideState(int iGuide);
	bool CheckGuideState(int iGuide);
	void ResetGuideState(int iGuide);
	
protected:	//	Attributes

	CLuaState * m_pState;
	unsigned char m_aFirstState[FIRST_STATE_BLOCK];
	unsigned char m_aSpecialState[SPECIAL_ID_BLOCK];

	unsigned char m_aTriggerOpenTask[TRIGGER_OPEN_TASK];
	unsigned char m_aTriggerCompleteTask[TRIGGER_COMPLETE_TASK];

	unsigned char m_aGuideState[HELP_GUIDE_NUM];
	
	int			m_iLastEvent;
	bool		m_bSecretaryEnable;
	bool        m_bHelpGuideEnable;
	double		m_backupExp;
	bool		m_bReady;

protected:	//	Operations

	void Reset();


};