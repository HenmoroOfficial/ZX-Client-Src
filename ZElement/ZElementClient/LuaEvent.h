/********************************************************************
  created:	   17/5/2006  
  filename:	   LuaEvent.h
  author:      Wangkuiwu  
  description: 
  Copyright (c) , All Rights Reserved.
*********************************************************************/

#ifndef _LUA_EVENT_H_
#define  _LUA_EVENT_H_


//#include <AWString.h>
//#include <A3DTypes.h>
//#include <AString.h>
//#include <vector.h>
//#include "AAssist.h"
//
//namespace LuaBind
//{
//	enum
//	{
//		LUA_EVENT_FIRST  = WM_USER+1,
//
//		CHATROOM_GET_LIST,  //lua-> c++
//		CHATROOM_LIST_COMING, //c++ -> lua
//		CHATROOM_INFO,    //c++ -> lua
//		DLG_SHOW_EPEQUIP,
//
//		LUA_EVENT_LAST,
//	};
// }

enum
{

	LEVT_FIRST_ENTER_GAME		=	30000	   , 
	LEVT_PLAYER_IS_MOVING		=	30001	   , 
	LEVT_PLAYER_ADJUST_ORIENT	=	30002	   , 
	LEVT_PLAYER_LOCK_VISION		=	30003	   , 
	LEVT_FIRST_EMOTION_ACTION	=	30004	   , 
	LEVT_FIRST_EMOTION_WORD		=	30005	   , 
	LEVT_FIRST_CAMERA_MODE		=	30006	   , 
	LEVT_FIRST_SAFELOCK			=	30007	   , 
	LEVT_FIRST_BOSSKEY			=	30008	   , 
	LEVT_FIRST_TALK_NPC			=	30009	   , 
	LEVT_FIRST_TASK_MAIN		=	30010	   , 
	LEVT_FIRST_TASK_TRAVEL		=	30011	   , 
	LEVT_FIRST_TASK_RISK		=	30012	   , 
	LEVT_FIRST_TASK_CLASSICAL	   =	30013	   , 
	LEVT_FIRST_TASK_CHANLLENGE	   =	30014	   , 
	LEVT_FIRST_TASK_KILL_MONSTER	   =	30015	   , 
	LEVT_FIRST_TASK_FORGE	   =	30016	   , 
	LEVT_FIRST_TASK_HERIT	   =	30017	   , 
	LEVT_FIRST_TASK_ASCETIC	   =	30018	   , 
	LEVT_FIRST_TASK_LUPIN	   =	30019	   , 
	LEVT_FIRST_TASK_TEACHER	   =	30020	   , 
	LEVT_FIRST_TASK_GUILD	   =	30021	   , 
	LEVT_FIRST_TASK_TREASURE	   =	30022	   , 
	LEVT_FIRST_TASK_ADVENTURE	   =	30023	   , 
	LEVT_FIRST_TASK_LOVE	   =	30024	   , 
	LEVT_FIRST_TASK_READ	   =	30025	   , 
	LEVT_FIRST_OBJECT_PICK	   =	30026	   , 
	LEVT_FIRST_TASK_FAIL	   =	30027	   , 
	LEVT_FIRST_TASK_LIMIT	   =	30028	   , 
	LEVT_FIRST_TASK_TRACK	   =	30029	   , 
	LEVT_FIRST_MAP_AUTOTRACK	   =	30030	   , 
	LEVT_FIRST_MAP_OPEN	   =	30031	   , 
	LEVT_FIRST_TASK_FINISH	   =	30032	   , 
	LEVT_FIRST_TASK_AWARD	   =	30033	   , 
	LEVT_FIRST_TASK_SEARCH_LATEST	   =	30034	   , 
	LEVT_FIRST_BAG_OPEN	   =	30035	   , 
	LEVT_FIRST_ITEM_EQUIP	   =	30036	   , 
	LEVT_FIRST_MEDICINE_USE	   =	30037	   , 
	LEVT_FIRST_ITEM_SPECIAL	   =	30038	   , 
	LEVT_FIRST_BAG_NEATEN	   =	30039	   , 
	LEVT_FIRST_BAG_EXPAND	   =	30040	   , 
	LEVT_FIRST_ITEM_SPILT	   =	30041	   , 
	LEVT_FIRST_BAG_SELL	   =	30042	   , 
	LEVT_FIRST_EXCHANGE	   =	30043	   , 
	LEVT_FIRST_CASH_EXCHANGE	   =	30044	   , 
	LEVT_FIRST_DEPOT_OPEN	   =	30045	   , 
	LEVT_FIRST_DEPOT_EXPAND	   =	30046	   , 
	LEVT_FIRST_HORSE_EQUIP	   =	30047	   , 
	LEVT_FIRST_ITEM_LOCK	   =	30048	   , 
	LEVT_FIRST_BAG_PET	   =	30049	   , 
	LEVT_FIRST_BAG_FASHDRESS	   =	30050	   , 
	LEVT_FIRST_SKILL_SPECIAL	   =	30051	   , 
	LEVT_FIRST_SKILL_POINT_ADD	   =	30052	   , 
	LEVT_FIRST_SKILL_EDIT	   =	30053	   , 
	LEVT_FIRST_SKILL_CLEAR	   =	30054	   , 
	LEVT_FIRST_ATTACK	   =	30055	   , 
	LEVT_FIRST_PK	   =	30056	   , 
	LEVT_FIRST_PK_SETTING	   =	30057	   , 
	LEVT_FIRST_PK_UP	   =	30058	   , 
	LEVT_FIRST_PK_DOWN	   =	30059	   , 
	LEVT_FIRST_BUFF	   =	30060	   , 
	LEVT_FIRST_HP	   =	30061	   , 
	LEVT_FIRST_DIE	   =	30062	   , 
	LEVT_FIRST_DIE_PUNISH	   =	30063	   , 
	LEVT_FIRST_REST	   =	30064	   , 
	LEVT_FIRST_KILL_MONSTER	   =	30065	   , 
	LEVT_FIRST_PICKUP	   =	30066	   , 
	LEVT_FIRST_ABANDON	   =	30067	   , 
	LEVT_FIRST_CHAT_OPEN	   =	30068	   , 
	LEVT_FIRST_MONSTER_SPECIAL	   =	30069	   , 
	LEVT_FIRST_ENTER_MAP_NEW	   =	30070	   , 
	LEVT_FIRST_MAP_TRANSFER	   =	30071	   , 
	LEVT_FIRST_TEAM_SETUP	   =	30072	   , 
	LEVT_FIRST_TEAM_SETTING	   =	30073	   , 
	LEVT_FIRST_BATTLE_GUILD	   =	30074	   , 
	LEVT_FIRST_BATTLE_CITY	   =	30075	   , 
	LEVT_FIRST_TALISMAN_MAIN	   =	30076	   , 
	LEVT_FIRST_TALISMAN_VICE	   =	30077	   , 
	LEVT_FIRST_TALISMAN_START	   =	30078	   , 
	LEVT_FIRST_TALISMAN_UPDATE	   =	30079	   , 
	LEVT_FIRST_TALISMAN_MEDIUM	   =	30080	   , 
	LEVT_FIRST_TALISMAN_REFINE	   =	30081	   , 
	LEVT_FIRST_TALISMAN_ENCHANT	   =	30082	   , 
	LEVT_FIRST_TALISMAN_CLEAN	   =	30083	   , 
	LEVT_FIRST_TALISMAN_COMBINE	   =	30084	   , 
	LEVT_FIRST_FRIEND_ADD	   =	30085	   , 
	LEVT_FIRST_HUG	   =	30086	   , 
	LEVT_FIRST_MARRY	   =	30087	   , 
	LEVT_FIRST_FAMILY_CREATE	   =	30088	   , 
	LEVT_FIRST_FAMILY_JOIN	   =	30089	   , 
	LEVT_FIRST_GUILD_CREATE	   =	30090	   , 
	LEVT_FIRST_GUILD_JOIN	   =	30091	   , 
	LEVT_FIRST_FRIEND_DELETE	   =	30092	   , 
	LEVT_FIRST_BLACKLIST	   =	30093	   , 
	LEVT_FIRST_ENEMY	   =	30094	   , 
	LEVT_FIRST_STUDENT	   =	30095	   , 
	LEVT_FIRST_STUDENT_OUT	   =	30096	   , 
	LEVT_FIRST_TEACHER	   =	30097	   , 
	LEVT_FIRST_STUDENT_ADD	   =	30098	   , 
	LEVT_FIRST_FRIEND_SEEK	   =	30099	   , 
	LEVT_FIRST_LUPIN_POINT	   =	30100	   , 
	LEVT_FIRST_PET_GET	   =	30101	   , 
	LEVT_FIRST_PET_SPECIAL	   =	30102	   , 
	LEVT_FIRST_PET_USE	   =	30103	   , 
	LEVT_FIRST_PET_EQUIPMENT	   =	30104	   , 
	LEVT_FIRST_PET_EQUIPMENT_STRENGHTEN	   =	30105	   , 
	LEVT_FIRST_PET_TAME	   =	30106	   , 
	LEVT_FIRST_PET_COMBINE	   =	30107	   , 
	LEVT_FIRST_PET_STAR_UP	   =	30108	   , 
	LEVT_FIRST_PET_CHANGE_IMAGE	   =	30109	   , 
	LEVT_FIRST_PET_CHANGE_NAME	   =	30110	   , 
	LEVT_FIRST_PET_CHANGE_MODE	   =	30111	   , 
	LEVT_FIRST_PET_FEED	   =	30112	   , 
	LEVT_FIRST_PET_WEAK	   =	30113	   , 
	LEVT_FIRST_PET_HUNGRY	   =	30114	   , 
	LEVT_FIRST_PET_DIE	   =	30115	   , 
	LEVT_FIRST_PET_LV_UP	   =	30116	   , 
	LEVT_FIRST_PET_COLLECT	   =	30117	   , 
	LEVT_FIRST_PET_PRODUCE	   =	30118	   , 
	LEVT_FIRST_PET_CATCH	   =	30119	   , 
	LEVT_FIRST_FLY_GET	   =	30120	   , 
	LEVT_FIRST_FLY_MODE	   =	30121	   , 
	LEVT_FIRST_FLY_FIGHT_MODE	   =	30122	   , 
	LEVT_FIRST_FLY_LV_UP	   =	30123	   , 
	LEVT_FIRST_FLY_SPEED_UP	   =	30124	   , 
	LEVT_FIRST_ENCHASE_LV	   =	30125	   , 
	LEVT_FIRST_ENCHASE_POINT	   =	30126	   , 
	LEVT_FIRST_BLOOD_ENCHANSE	   =	30127	   , 
	LEVT_FIRST_EQUIP_EXCHANGE	   =	30128	   , 
	LEVT_FIRST_EQUIP_BREAK	   =	30129	   , 
	LEVT_FIRST_EQUIP_HIDE	   =	30130	   , 
	LEVT_FIRST_EQUIP_BIND	   =	30131	   , 
	LEVT_FIRST_EQUIP_DEPART	   =	30132	   , 
	LEVT_FIRST_EQUIP_DESTROY	   =	30133	   , 
	LEVT_FIRST_EQUIP_SUIT	   =	30134	   , 
	LEVT_FIRST_MAIL	   =	30135	   , 
	LEVT_FIRST_DESC_JOIN	   =	30136	   , 
	LEVT_LV_UP	   =	30137	   , 
	LEVT_FIRST_QSHOP_OPEN	   =	30138	   , 
	LEVT_QSHOP_ITEM_GET	   =	30139	   , 
	LEVT_FIRST_RAFFLE	   =	30140	   , 
	LEVT_FIRST_POPSHOP	   =	30141	   , 
	LEVT_FIRST_CREDIT_ADD	   =	30142	   , 
	LEVT_RANK_PERSON_DROPOUT	   =	30143	   , 
	LEVT_RANK_PERSONALL_DROPOUT	   =	30144	   , 
	LEVT_FIRST_HIDE_ELSEPLAYER     =    30145      ,
	LEVT_FIRST_DEITY_LEVELUP		=   30146	   ,
	LEVT_FIRST_DEITY_POPHELP		=	30147		,
	LEVT_FIRST_OPEN_RADIO			=	30148,
	LEVT_FIRST_ITEM_SPECIAL2		=	30149,
	LEVT_LV_UP2						=	30150,
	LEVT_FIRST_TALK_NPC2			=	30151,
	LEVT_FIRST_MAP_AUTOTRACK2		=	30152,
	
};

namespace LuaEvent
{

	void FirstEnterGame();
	void PlayerIsMoving();
	void PlayerIsAdjustOrient();
	void FirstLockVision();
	void FirstEmotionAction();
	void FirstEmotionWord();
	void FirstCameraMode();
	void FirstSafeLock();
	void FirstBossKey();
	void FirstTalkNPC();
	void FirstTalkNPC2();
	void FirstAcceptMainTask();
	void FirstAcceptTravelTask();
	void FirstAcceptRiskTask();
	void FirstAcceptInterestTask();
	void FirstAcceptWantedTask();
	void FirstAcceptClassicalTask();
	void FirstAcceptChanllengeTask();
	void FirstAcceptKillMonsterTask();
	void FirstAcceptForgeTask();
	void FirstAcceptHeritTask();
	void FirstAcceptAsceticTask();
	void FirstAcceptLupinTask();
	void FirstAcceptTeacherTask();
	void FirstAcceptGuildTask();
	void FirstAcceptTeasureTask();
	void FirstAcceptAdventureTask();
	void FirstAcceptLoveTask();
	// 查看任务	;
	void FirstViewTask();
	// 采集物品	;
	void FirstCollectItem();
	void FirstTaskFail();
	void FirstTaskFull();
	void FirstTraceTask();
	void FirstAutoMove();
	void FirstAutoMove2();
	void FirstOpenMap();
	void FirstFinishTask();
	void FirstTaskAward();
	void FirstSearchLatestTask();
	void FirstOpenPack();
	void FirstEquipItem();
	void FirstUseMedicine();
	void FirstObtainSpecialItem(int tid);
	void FirstObtainSpecialItem2(int tid);
	void FirstNeatenPack();
	void FirstExpandPack();
	void FirstSplitItem();
	// 第一次摆摊	;
	void FirstBooth();
	// 第一次交易	;
	void FirstTrade();
	// 第一次元宝交易	;
	void FirstCashTrade();
	void FirstOpenTrash();
	void FirstExpandTrash();
	// 第一次装备坐骑	;
	void FirstEquipHorse();
	void FirstLockItem();
	void FirstOpenPetPack();
	void FirstOpenFashionPack();
	void FirstCastSpecialSkill(int skillid);
	void FirstAddSkillPoint();
	// 编辑连续技	;
	void FirstEditContinuousSkill();
	// 洗点	;
	void FirstClearSkill();
	void FirstAttack();
	void FirstPK();
	// pk 设置	;
	void FirstPKSetting();
	// pk值 增加	;
	void FirstIncPKValue();
	void FirstDecPKValue();
	void FirstBUFF();
	// 第一次hp小于50%	;
	void FirstHalfHP();
	void FirstDie();
	void FirstPunishDie();
	void FirstSitDown();
	void FirstKillMonster();
	void FirstPickUpItem();
	void FirstDiscardItem();
	void FirstOpenChat();
	// 第一次选中特殊属性的怪物	;
	void FirstClickSpecialMonster(int id);
	void FirstEnterNewMap();
	void FirstTransmit();
	void FirstTeam();
	void FirstTeamSetting();
	void FirstFactionBattle();
	// 第一次参加城战	;
	void FirstCityBattle();
	void FirstObtainTalisman();
	void FirstObtainTalismanPart();
	// 第一次使用内挂	;
	void FirstRunHelper();
	//	;
	void FirstTalismanUpgrade();
	// 获得法宝灵媒	;
	void FirstObtainTalismanMedium();
	// 聚气	;
	void FirstRefineTalisman();
	// 灌魔	;
	void FirstEnchantTalisman();
	// 归元	;
	void FirstCleanTalisman();
	// 血炼	;
	void FirstCombineTalisman();
	void FirstAddFriend();
	// 相依相偎	;
	void FirstHug();
	void FirstMarry();
	void FirstCreateFamily();
	void FirstJoinFamily();
	void FirstCreateFaction();
	void FirstJoinFaction();
	void FirstDeleteFriend();
	void FirstOpenBlackList();
	void FirstOpenEnemy();
	// 拜师	;
	void FirstBeStudent();
	// 出师	;
	void FirstLeaveMaster();
	// 收徒	;
	void FirstbeMaster();
	// 收徒数量上限增加	;
	void IncDispileUpperLimit();
	void FirstOpenSNS();
	// 狭义值	;
	void FirstObtainErrantry();
	void FirstObtainPet();
	void FirstObtainSpecialPet(int id);
	void FirstUsePet();
	void FirstObtainPetEquip();
	void FirstEnhancePetEquip();
	void FirstTamePet();
	void FirstRefinePet();
	void FirstIncPetStar();
	void FirstChangePetShape();
	void FirstChangePetName();
	// 宠物模式切换	;
	void FirstChangePetMode();
	void FirstFeedPet();
	void FirstPetWeak();
	void FirstPetHungry();
	void FirstPetDie();
	void FirstPetLvUp();
	void FirstPetCollect();
	void FirstPetProduce();
	// 第一次捕捉宠物	;
	void FirstCatchPet();
	void FirstObtainFlySword();
	void FirstFly();
	void FirstFlyFight();
	// 第一次提高飞行法宝成长度	;
	void FirstFlySwordGrowthRateUp();
	void FirstFlySowrdSpeedUp();
	// 第一次炼器	;
	void FirstHarden();
	// 灌魔	;
	void FirstEnhancePoint();
	// 血祭	;
	void FirstBleedScarifice();
	// 兑换装备	;
	void FirstExchangeEquip();
	void FirstBreakEquip();
	void FirstHideEquip();
	void FirstBindEquip();
	void FirstDepartEquip();
	void FirstDestroyEquip();
	void FirstSuitEquip();
	void FirstSendMail();
	void FirstReceiveMail();
	// 第一次加入门派	;
	void FirstJoinSect(int prof);
	void LevelUp(int level);
	void LevelUp2(int level);
	void FirstOpenQshop();
	void FirstObtainQshopItem();
	void FirstLottery();
	void FirstOpenQshop2();
	void FirstIncReputation();
	void RankDropOut();
	void RepuRankDropOut();
	void FirstHideElsePlayer();
	void FirstDeityLevelUp();
	
	void FirstSpecialOperation(int iEvent);
	void FirstSpecialOperationWithArg(int iEvent, int arg);

	// 接到脚本触发完成的任务

	void AcceptScriptCompleteTask(int iTask);
}

#endif