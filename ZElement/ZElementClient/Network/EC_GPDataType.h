/*
 * FILE: EC_GPDataType.h
 *
 * DESCRIPTION: Game net command type
 *
 * CREATED BY: CuiMing, Duyuxin, 2004/9/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_NetDef.h"
#include "EC_RoleTypes.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define GP_S2C_CHECKDATA(s) \
	static bool CheckValid(const void* pData, int iLen) \
	{ \
		s data; \
		return data.FillData(pData, iLen); \
	}

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

namespace S2C
{
	//	Data type ---------------------------
	struct info_player_1
	{
		int cid;
		A3DVECTOR3 pos;
		unsigned short crc_e;
		unsigned char dir;		//	256的定点数表示度数
		unsigned char profession;
		unsigned char level;
		unsigned char reborn_count;
		unsigned char dt_level;
		char pk_level;
		char cultivation;
		int reputation;
		int hide_equip;
		int state;
		int extra_state;
		//	if (state & GP_STATE_ADV_MODE), 2 DWORD inserted here
		//	if (state & GP_STATE_SHAPE), 1 char inserted here
		//	if (state & GP_STATE_EMOTE), 1 char inserted here
		//	If (state & GP_STATE_EXTEND_PROPERTY), 1 DWORD inserted here
		//	If (state & GP_STATE_FACTION), 1 int inserted here
		//	If (state & GP_STATE_BOOTH), 1 char inserted here
		//	If (state & GP_STATE_EFFECT), effect data inserted here
		//	If (state & GP_STATE_PARIAH), 1 char inserted here
		//	If (state & GP_STATE_IN_MOUNT), 1 char + 1 int inserted here
		//	If (state & GP_STATE_IN_BIND), 1 char + 1 int inserted here
		//	If (state & GP_STATE_TITLE), 1 short inserted here
		//	If (state & GP_STATE_SPOUSE), 1 int inserted here
		//	If (state & GP_STATE_FAMILY), 1 int + 1 char inserted here
		//	If (state & GP_STATE_SECT_MASTER_ID), 1 int inserted here

		unsigned int adv_d1;
		unsigned int adv_d2;
		unsigned char shape;
		unsigned char emote;
		__int64 ext_states;
		int id_faction;
		int id_family;
		unsigned char fam_rank;
		unsigned char crc_booth;
		int	booth_item_id;
		unsigned char effect_num;
		short effects[256];
		unsigned char pariah_lvl;
		unsigned char riding_pet_lvl;
		int id_riding_pet;
		unsigned char bMultiRide;
		unsigned char bind_type;
		int id_buddy;
		unsigned char buddy_pos;
		bool bDisConnect;
		short title;
		int id_spouse;
		int master_id;
		unsigned char military_rank;
		unsigned char vip_state;
		int	id_transfiguredProp;	// 变身属性 模板id
		int id_carrier;				// 所在载体的id
		A3DVECTOR3 rpos;			// 相对载体的位置
		char rdir;					// 相对载体的方向
		int id_circle;				// 所在圈子的id
		unsigned char circle_role;	// 在圈子中的角色，圈长，圈花，圈草
		int id_orgserver;			// 原本的服务器id
		unsigned char	vip_award_level;	//VIP 奖励的级别 Added 2012-01-16.
		char		is_hide_vip_level;	// 是否让别的玩家看到自己vip信息
		int	collision_raid_transfigure_id;	//碰撞副本变身ID Added 2012-08-01.
		int	collision_raid_state;			//碰撞战场状态 Added 2012-08-01.
		char xuanyuan_dark_light_state;		//轩辕光暗状态 Added 2012-08-16.
		char dark_light_spirits[3];			//轩辕灵
		char mirror_image_cnt;				//当前镜像数量
		unsigned char change_color;			//当前飞剑变色索引 Added 2012-08-20.
		int visible_state_id;				//十字方向技能效果ID
		int	visible_state_dir;				//十字方向技能效果方向
		bool visible_state_on;				//十字方向技能开启情况
		char start_fly_type;				// 飞行状态的类型， 0：飞剑， 1：技能
		char weapon_effect_level;			//武器特效buff炼器等级
		short flow_battle_ckill;				// 流水席状态
		char   puppetid;						//傀儡姿态
		int interactor_id;				//互动物品ID
		char interact_pos;				//互动位置
		char interact_state;			//互动状态
		bool active_emote_inviter;		//是否是邀请方
		int active_emote_id;			//互动表情对象id
		int active_emote_type;			//互动表情类型
		int traval_vehicle_id;			//自由飞行坐骑物品id
		GP_S2C_CHECKDATA(info_player_1)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct info_player_2		//name ,customize data 不常改变
	{
		unsigned char size;
		char data[1];
	};

	struct info_player_3		//viewdata of equipments and special effect
	{
		unsigned char size;
		char  data[1];
	};

	struct info_player_4		//detail
	{
		unsigned short size;
		char	data[1];
	};

	struct info_npc
	{
		int nid;
		int tid;			//template id
		A3DVECTOR3 pos;
		unsigned short seed;		// seed of customize data
		unsigned char dir;
		int state;
		//	If (state & GP_STATE_EXTEND_PROPERTY), 1 DWORD inserted here
		//	If (state & GP_STATE_NPC_PET), 1 int inserted here
		//	If (state & GP_STATE_NPC_NAME), 1 BYTE + name string here

		__int64 ext_states;
		int id_master;
		char pet_star;
		char shape;
		char face;
		wchar_t name[16];
		int id_summoner;
		char owner_type;
		int  owner_id;

		int	visible_state_id;
		int visible_state_dir;
		bool visible_state_on;

		short phase_id;
		char  teleportnum;

		GP_S2C_CHECKDATA(info_npc)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct info_matter
	{
		int mid;
		int tid;
		int dropper_id;				// 掉落人的id							
		A3DVECTOR3 pos;
		unsigned char dir0;
        unsigned char dir1;
        unsigned char rad;
        unsigned char state;
		unsigned char flag;

		char owner_type;
		int  owner_id;
		
		short phase_id;
		int	mine_state;

		GP_S2C_CHECKDATA(info_matter)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct info_pet
	{
		int honor_point;        	//	好感度
		int hunger;					//	饥饿度
		int feed_time;				//	上次喂养到现在的时间
		int pet_tid;            	//	宠物的模板ID
		int pet_vis_tid;        	//	宠物的可见ID（如果为0，则表示无特殊可见ID）
		int pet_egg_tid;        	//	宠物蛋的ID
		int pet_class;          	//	宠物类型 战宠，骑宠，观赏宠
		float hp_factor;        	//	血量比例（复活和收回时使用） 0则为死亡
		short level;            	//	宠物级别
		short sec_level;        	//	第二级别
		int exp;                	//	宠物当前经验
		int skill_point;        	//	剩余技能点
		unsigned short color;   	//	颜色     目前此项无效
		unsigned short name_len;	//	名字长度 目前此项无效（因为策划尚无名字需求）
		char name[16];          	//	名字内容

		struct
		{
			int skill;
			int level;

		} skills [GP_PET_SKILL_NUM];
	};

	//	Commands ----------------------------

	enum	//	Command ID
	{
		PROTOCOL_COMMAND = -1,	//	Reserved for protocol
		
		PLAYER_INFO_1 = 0,
		PLAYER_INFO_2,
		PLAYER_INFO_3,
		PLAYER_INFO_4,
		PLAYER_INFO_1_LIST,

		PLAYER_INFO_2_LIST,		//	5
		PLAYER_INFO_3_LIST,
		PLAYER_INFO_23_LIST,
		SELF_INFO_1,
		NPC_INFO_LIST,

		MATTER_INFO_LIST,		//	10
		NPC_ENTER_SLICE,
		PLAYER_ENTER_SLICE,
		OBJECT_LEAVE_SLICE,
		NOTIFY_HOSTPOS,

		OBJECT_MOVE,			//	15
		NPC_ENTER_WORLD,
		PLAYER_ENTER_WORLD,
		MATTER_ENTER_WORLD,
		PLAYER_LEAVE_WORLD,
		
		NPC_DIED,				//	20
		OBJECT_DISAPPEAR,
		OBJECT_STARTATTACK,
		HOST_STOPATTACK,
		HOST_ATTACKRESULT,

		ERROR_MESSAGE,			//	25
		HOST_ATTACKED,
		PLAYER_DIED,
		HOST_DIED,
		PLAYER_REVIVE,

		PICKUP_MONEY,			//	30
		PICKUP_ITEM,
		PLAYER_INFO_00,
		NPC_INFO_00,
		OUT_OF_SIGHT_LIST,
		
		OBJECT_STOP_MOVE,		//	35
		RECEIVE_EXP,
		LEVEL_UP,
		SELF_INFO_00,
		UNSELECT,

		OWN_ITEM_INFO,			//	40, Own item information
		EMPTY_ITEM_SLOT,		//	Empty item slot notify
		OWN_IVTR_DATA,			//	Own inventory data
		OWN_IVTR_DETAIL_DATA,
		EXG_IVTR_ITEM,

		MOVE_IVTR_ITEM,			//	45
		PLAYER_DROP_ITEM,
		EXG_EQUIP_ITEM,
		EQUIP_ITEM,
		MOVE_EQUIP_ITEM,

		OWN_EXT_PROP,			//	50
		ADD_STATUS_POINT,
		SELECT_TARGET,
		PLAYER_EXT_PROP_BASE,
		PLAYER_EXT_PROP_MOVE,

		PLAYER_EXT_PROP_ATK,	//	55
		PLAYER_EXT_PROP_DEF,
		TEAM_LEADER_INVITE,
		TEAM_REJECT_INVITE,
		TEAM_JOIN_TEAM,

		TEAM_MEMBER_LEAVE,		//	60
		TEAM_LEAVE_PARTY,
		TEAM_NEW_MEMBER,
		TEAM_LEADER_CACEL_PARTY,
		TEAM_MEMBER_DATA,

		TEAM_MEMBER_POS,		//	65
		EQUIP_DATA,				//	用户的装备数据，影响表现
		EQUIP_DATA_CHANGED,
		EQUIP_DAMAGED,			//	装备损坏
		TEAM_MEMBER_PICKUP,		//	队友捡起装备

		NPC_GREETING,			//	70
		NPC_SERVICE_CONTENT,
		PURCHASE_ITEM,
		ITEM_TO_MONEY,
		REPAIR_ALL,

		REPAIR,					//	75
		RENEW,
		SPEND_MONEY,
		GAIN_MONEY_IN_TRADE,
		GAIN_ITEM_IN_TRADE,

		GAIN_MONEY_AFTER_TRADE,	//	80
		GAIN_ITEM_AFTER_TRADE,
		GET_OWN_MONEY,
		ATTACK_ONCE,
		HOST_START_ATTACK,

		OBJECT_CAST_SKILL,		//	85
		SKILL_INTERRUPTED,
		SELF_SKILL_INTERRUPTED,
		SKILL_PERFORM,
		OBJECT_BE_ATTACKED,		//	已作废

		SKILL_DATA,				//	90
		HOST_USE_ITEM,
		EMBED_ITEM,
		CLEAR_TESSERA,
		COST_SKILL_POINT,

		LEARN_SKILL,			//	95
		OBJECT_TAKEOFF,
		OBJECT_LANDING,
		FLYSWORD_TIME,
		HOST_OBTAIN_ITEM,

		PRODUCE_START,			//	100
		PRODUCE_ONCE,
		PRODUCE_END,
		DECOMPOSE_START,
		DECOMPOSE_END,

		TASK_DATA,				//	105
		TASK_VAR_DATA,
		OBJECT_START_USE,
		OBJECT_CANCEL_USE,
		OBJECT_USE_ITEM,

		OBJECT_START_USE_T,		//	110
		OBJECT_SIT_DOWN,
		OBJECT_STAND_UP,
		OBJECT_DO_EMOTE,
		SERVER_TIME,

		OBJECT_ROOT,			//	115
		HOST_DISPEL_ROOT,
		PK_LEVEL_NOTIFY,
		PK_VALUE_NOTIFY,
		PLAYER_CHANGE_CLASS,

		OBJECT_ATTACK_RESULT,	//	120
		BE_HURT,
		HURT_RESULT,
		HOST_STOP_SKILL,
		UPDATE_EXT_STATE,

		ICON_STATE_NOTIFY,		//	125
		PLAYER_GATHER_START,
		PLAYER_GATHER_STOP,
		TRASHBOX_PWD_CHANGED,
		TRASHBOX_PWD_STATE,

		TRASHBOX_OPEN,			//	130
		TRASHBOX_CLOSE, 
		TRASHBOX_WEALTH,
		EXG_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,

		EXG_TRASHBOX_IVTR,		//	135
		IVTR_ITEM_TO_TRASH,
		TRASH_ITEM_TO_IVTR,
		EXG_TRASH_MONEY,
		ENCHANT_RESULT,
		
		HOST_NOTIFY_ROOT,		//	140
		OBJECT_DO_ACTION,
		RECORD_START,
		OBJECT_SKILL_ATTACK_RESULT,
		HOST_SKILL_ATTACKED,

		PLAYER_SET_ADV_DATA,	//	145
		PLAYER_CLR_ADV_DATA,
		PLAYER_IN_TEAM,
		TEAM_ASK_TO_JOIN,
		OBJECT_EMOTE_RESTORE,

		CON_EMOTE_REQUEST,		//	150, concurrent emote request
		DO_CONCURRENT_EMOTE,
		MATTER_PICKUP,
		MAFIA_INFO_NOTIFY,
		MAFIA_TRADE_START,

		MAFIA_TRADE_END,		//	155
		TASK_DELIVER_ITEM,
		TASK_DELIVER_REP,
		TASK_DELIVER_EXP,
		TASK_DELIVER_MONEY,

		TASK_DELIVER_LEVEL2,	//	160
		PLAYER_REPUTATION,
		ITEM_IDENTIFY,
		PLAYER_CHGSHAPE,
		ENTER_SANCTUARY,

		LEAVE_SANCTUARY,		//	165
		PLAYER_OPEN_BOOTH,
		SELF_OPEN_BOOTH,
		PLAYER_CLOSE_BOOTH,
		PLAYER_BOOTH_INFO,

		BOOTH_TRADE_SUCCESS,	//	170
		BOOTH_NAME,
		PLAYER_START_TRAVEL,
		HOST_START_TRAVEL,
		PLAYER_END_TRAVEL,

		GM_INVINCIBLE,			//	175
		GM_INVISIBLE,
		HOST_CORRECT_POS,
		OBJECT_CAST_INSTANT_SKILL,
		ACTIVATE_WAYPOINT,

		WAYPOINT_LIST,			//	180
		UNFREEZE_IVTR_SLOT,		//	Unfreeze inventory item
		TEAM_INVITE_TIMEOUT,
		PLAYER_PVP_NO_PROTECT,
		PLAYER_DISABLE_PVP,

		HOST_PVP_COOLDOWN,		//	185
		COOLTIME_DATA,
		SKILL_ABILITY,
		OPEN_BOOTH_TEST,
		BREATH_DATA,

		HOST_STOP_DIVE,			//	190
		BOOTH_SELL_ITEM,
		PLAYER_ENABLE_FASHION,
		HOST_ENABLE_FREEPVP,
		INVALID_OBJECT,

		PLAYER_ENABLE_EFFECT,	//	195
		PLAYER_DISABLE_EFFECT,
		REVIVAL_INQUIRE,
		SET_COOLDOWN,
		CHANGE_TEAM_LEADER,

		EXIT_INSTANCE,			//	200
		CHANGE_FACE_START,
		CHANGE_FACE_END,
		PLAYER_CHG_FACE,		//	Player change face completed
		OBJECT_CAST_POS_SKILL,

		SET_MOVE_STAMP,			//	205
		INST_DATA_CHECKOUT,
		HOST_RUSH_FLY,
		TRASHBOX_SIZE,
		NPC_DIED2,
		
		PRODUCE_NULL,			//	210
		PVP_COMBAT,
		DOUBLE_EXP_TIME,
		AVAILABLE_DOUBLE_EXP_TIME,
		DUEL_RECV_REQUEST,

		DUEL_REJECT_REQUEST,	//	215
		DUEL_PREPARE,
		DUEL_CANCEL,
		HOST_DUEL_START,
		DUEL_STOP,

		DUEL_RESULT,			//	220
		PLAYER_BIND_REQUEST,
		PLAYER_BIND_INVITE,
		PLAYER_BIND_REQUEST_REPLY,         
		PLAYER_BIND_INVITE_REPLY,

		PLAYER_BIND_START,		//	225
		PLAYER_BIND_STOP,
		PLAYER_MOUNTING,
		PLAYER_EQUIP_DETAIL,
		PLAYER_DUEL_START,
		
		PARIAH_TIME,			//	230
		GAIN_PET,
		FREE_PET,
		SUMMON_PET,
		RECALL_PET,

		PLAYER_START_PET_OP,	//	235
		PLAYER_STOP_PET_OP,
		PET_RECEIVE_EXP,
		PET_LEVELUP,
		PET_ROOM,

		PET_ROOM_CAPACITY,		//	240
		PET_HONOR_POINT,
		PET_HUNGER_GAUGE,
		HOST_ENTER_BATTLE,
		TANK_LEADER_NOTIFY,
		
		BATTLE_RESULT,			//	245
		BATTLE_INFO,
		PLAYER_PET_DEAD,
		unknown_248,
		PLAYER_PET_HP_NOTIFY,

		PLAYER_PET_AI_STATE,			//	250
		PLAYER_RECIPE_DATA,
		PLAYER_LEARN_RECIPE,
		PLAYER_PRODUCE_SKILL_INFO,
		PLAYER_REFINE_RESULT,

		PLAYER_START_GENERAL_OPERATION,		// 255
		PLAYER_STOP_GENERAL_OPERATION,
		PLAYER_CASH,
		PLAYER_SKILL_ADDON,
		PLAYER_SKILL_COMMON_ADDON,

		PLAYER_EXTRA_SKILL,					//	260
		PLAYER_TITLE_LIST,
		PLAYER_ADD_TITLE,
		PLAYER_DEL_TITLE,
		PLAYER_REGION_REPUTATION,
		
		PLAYER_CHANGE_REGION_REPUTATION,	// 265
		PLAYER_CHANGE_TITLE,
		PLAYER_CHANGE_INVENTORY_SIZE,
		PLAYER_BIND_SUCCESS,
		PLAYER_CHANGE_SPOUSE,

		PLAYER_INVADER_STATE,				// 270
		PLAYER_MAFIA_CONTRIBUTION,
		LOTTERY_BONUS,
		RECORD_CHECK_RESULT,
		PLAYER_USE_ITEM_WITH_ARG,

		OBJECT_USE_ITEM_WITH_ARG,			// 275
		MAFIA_TRASHBOX_CAPACITY,
		NPC_SERVICE_RESULT,
		BATTLE_FLAG_CHANGE,
		BATTLE_START,

		BATTLE_FLAG_CHANGE_STATUS,			// 280
		TALISMAN_EXP_NOTIFY,
		TALISMAN_MERGE_NOTIFY,
		TALISMAN_VALUE_NOTIFY,
		BOT_BEGIN_NOTIFY,

		PLAYER_WALLOW_INFO,					// 285
		ANTI_CHEAT_BONUS,
		BATTLE_SCORE,
		GAIN_BATTLE_SCORE,
		SPEND_BATTLE_SCORE,

		BATTLE_PLAYER_INFO,					// 290
		TALENT_NOTIFY,
		BATTLE_LEAVE,
		NOTIFY_SAFE_LOCK,
		ENTER_WAR,

		LEAVE_WAR,							// 295
		WAR_INFO,
		WAR_START,
		WAR_END,
		WAR_CONSTRUCTION_INFO,

		WAR_CONTRIBUTION_INFO,				// 300
		EQUIP_PET_BEDGE,
		EQUIP_PET_EQUIP,
		PLAYER_SET_PET_STATUS,
		PLAYER_COMBINE_PET,

		PLAYER_UNCOMBINE_PET,				// 305
		PLAYER_PET_AGE_LIFE,
		PLAYER_PET_SET_AUTO_SKILL,
		PLAYER_PET_SET_SKILL_COOLDOWN,
		WAR_BUILDING_STATUS_CHANGE,

		PLAYER_SET_PET_RANK,				// 310
		PLAYER_REBORN_INFO,
		SCRIPT_MESSAGE,
		PLAYER_PET_CIVILIZATION,
		PLAYER_PET_CONSTRUCTION,
		
		PLAYER_MOVE_PET_BEDGE_ITEM,			// 315
		PLAYER_START_FLY,
		PLAYER_PET_PROP_ADDED,
		SECT_BECOME_DISCIPLE,
		CULTIVATION_NOTIFY,

		OFFLINE_AGENT_BONUS,				// 320
		OFFLINE_AGENT_TIME,
		EQUIP_SPIRIT_POWER_DECREASE,		// 魂力因攻击/被攻击而减少
		PLAYER_MULTI_BIND_INVITE,			// 多人骑乘
		PLAYER_MULTI_BIND_INVITE_REPLY,
		
		PLAYER_MULTI_BIND_JOIN,             // 325
		PLAYER_MULTI_BIND_LEAVE,
		PLAYER_MULTI_BIND_KICK,
		EXG_POCKET_ITEM,					// 随身包裹，交互物品
		MOVE_POCKET_ITEM,
		
		EXG_IVTR_POCKET_ITEM,				// 330
		POCKET_ITEM_TO_IVTR,
		IVTR_ITEM_TO_POCKET,
		CHANGE_POCKET_SIZE,
		POCKET_ITEM_ALL_TO_IVTR,

		SELF_BUFF_NOTIFY,					// 335
		NOTIFY_FASHION_HOTKEY,				// 时装换装快捷键设置通知
		EXG_FASHION_ITEM,					//	交换时装包裹中的物品
		EXG_IVTR_FASHION,					//  交换背包与时装包裹中的物品
		EXG_EQUIP_FASHION,					//	交换装备栏与时装包裹中的物品

		HOTKEY_CHANGE_FASHION,				// 340 根据hotkey的设置来更换整套时装
		PEEP_INFO,							// 某人正在观察你
		PLAYER_BONUS,						// 获取红利点数
		SELF_SJBATTLE_INFO,					// 宋金战场个人战绩
		GET_SJBATTLE_INFO,					// 获取宋金战场排行榜

		GET_ARENA_INFO,						// 345 竞技场信息
		MILITARY_RANK_CHANGE,				// 玩家军衔等级改变
		SJBATTLE_SCORE_CHANGE,				// 宋金战场积分改变
		PLAYER_CHANGE_FACE,					// 美容服务（换脸）
		PLAYER_CHANGE_VIPSTATE,				// 玩家改变VIP状态
		
		RECEIVE_BONUS_EXP,					// 350 获取奖励经验
		CROSSSERVER_BATTLE_SCORE,			// 跨服pk战场信息
		PLAYER_ACHIEVEMENT,	                
        PLAYER_ACHIEVEMENT_FINISH,                
		PLAYER_ACHIEVEMENT_ACTIVE,          
		
		PLAYER_PREMISS_DATA,				// 355            
        PLAYER_PREMISS_FINISH,              
		PLAYER_ACHIEVEMENT_MAP,
		ACHIEVEMENT_MESSAGE,
		SELF_INSTANCING_BATTLE_INFO,		// 个人剧情战场信息

		INSTANCING_BATTLE_INFO,				// 360 剧情战场信息
		SKILL_CONTINUE,						// 蓄力技能继续
		PLAYER_START_TRANSFIGURE,			// 开始变身
        MAGIC_DURATION_DECREASE,			// 幻化石灵力减少
        MAGIC_EXP_NOTIFY,					// 幻化石熟练度更新

		TRANSFIGURE_SKILL_DATA,				// 365 变身技能数据
		PLAYER_ENTER_CARRIER,				// 进入载体
		PLAYER_LEAVE_CARRIER,				// 离开载体
		PLAYER_MOVE_ON_CARRIER,				// 在载体上移动
		PLAYER_STOP_ON_CARRIER,				// 停止在载体上的移动
		
		SKILL_PROFICIENCY,					// 370
		NOTIFY_MALL_STRATEGY,				// 商城销售和打折方案
		LOCAL_PK_MESSAGE,					// 杀人消息 
		UPDATE_COMBO_STATE,					// 更新组合技
		ENTER_DIM_STATE,					// 进入朦胧状态

		ENTER_INVISIBLE_STATE,				// 375 进入隐身状态
		PLAYER_CHARGE,						// 冲锋
		ENTER_TERRITORY_BATTLE,				// 进入领土战战场
		LEAVE_TERRITORY_BATTLE,				// 离开领土战战场
		LAST_LOGOUT_TIME,					// 上次退出游戏时间
		
		COMBINE_SKILL_DATA,					// 380 连续技技能数据
		CLONE_PLAYER_EQUIP,					// 分身的基本信息和装备信息
		CIRCLE_INFO_NOTIFY,					// 圈子信息通知
		DELIVERY_EXP_NOTIFY,				//奖励经验 圈子 快雪
		DEITY_LEVELUP,						// 元神升级
		
		OBJECT_SPECIAL_STATE,				// 385 玩家特殊状态同步
		LOTTERY2_BONUS,
		GEM_NOTIFY,							// 宝石升品结果返回信息 Added 2011-03-22.
		CRSSVR_TEAM_BATTLE_INFO,			// 跨服小队pk的战场信息 Added 2011-05-19.
		CIRCLE_OF_DOOM_STARTUP,				// 通知队员结阵开始广播消息 Added 2011-06-16.

		CIRCLE_OF_DOOM_INFO,				// 390 当前阵法的结阵发起者id和成员id Added 2011-06-16.
		CIRCLE_OF_DOOM_STOP,				// 阵法结束 Added 2011-06-16.
		PLAYER_SCALE,						// 太昊归神/脱灵技能中缩放角色体型的消息 Added 2011-06-28.
		PLAYER_JAIL_CIRCLE_AREA,			// 焚香-火焰囚牢技能中囚牢相关信息的消息命令 Added 2011-07-08.
		ENTER_RAID,							// gs通知客户端进入副本 Added 2011-07-26.

		LEAVE_RAID,							// 395 gs通知客户端玩家离开副本 Added 2011-07-26.
		OBJECT_NOTIFY_RAID_POS,				// gs通知客户端玩家当前进入副本的位置 Added 2011-07-26.
		ENTER_RAID_COUNT,					// 玩家当天进入副本次数 Added 2011-07-27.
		OBJECT_BE_MOVED,					// 玩家对象被拉扯/牵引到目标位置，目前仅供焚香/太昊的新技能使用 Added 2011-07-27.
		PLAYER_PULL_STATE,					// 通知拉扯技能施法者当前的拉扯状态：开始拉扯/终止拉扯 Added 2011-07-28.

		PLAYER_BE_PULLED_INFO,				// 400 gs通知客户端各可视范围内玩家当前的拉扯玩家ID信息 Added 2011-07-28.
		LOTTERY3_BONUS,						// 新彩票的相关奖励信息 Added 2011-07-28.
		RAID_PLAYER_INFO,					// 进入副本后的玩家相关信息 Added 2011-08-03.
		TRIGGER_SKILL_TIME,					// 触发技能时间相关信息 Added 2011-08-03.
		PLAYER_STOP_SKILL,					// 通知ElsePlayer玩家停止当前技能 Added 2011-08-30.

		MALL_ITEM_PRICE,		            // 405 哪些限时商品正在销售
		PLAYER_ENTER_RAID_COUNT,			// gs返回客户端当前玩家进入某单个副本的次数 Added 2011-12-02.
		PLAYER_VIP_AWARD_INFO,				// gs通知客户端当前有哪些可以领取的VIP奖励信息 Added 2012-01-04.
		PLAYER_VIP_AWARD_UPDATE,			// gs通知客户端当前玩家的VIP奖励等级发生更新 Added 2012-01-16.
		ONLINE_AWARD_INFO,					// gs通知客户端当前玩家身上有哪些在线倒计时奖励 Added 2012-02-16.
		
		ONLINE_AWARD_PERMIT,				// 410 gs通知客户端某在线奖励能否可以领取 Added 2012-02-16.
		ADDON_PERMANENT_SKILL,				// 永久时装技能，单独走新协议 Added 2012-02-20.
		ADDON_PERMANENT_SKILL_DATA,			// 永久时装技能的相关数据 Added 2012-02-20.
		ZX_FIVE_ANNI_DATA,					// 5周年每日签到数据
		TREASURE_INFO,						//	gs通知客户端诛仙古卷挖宝信息 Added 2012-03-21.

		TREASURE_REGION_UPGRADE_RESULT,		// 415 挖宝区域升级结果信息 Added 2012-03-21.
		TREASURE_REGION_UNLOCK_RESULT,		// 挖宝区域解锁结果信息 Added 2012-03-21.
		TREASURE_REGION_DIG_RESULT,			// 对某区域挖宝后的结果 Added 2012-03-21.
		INC_TREASURE_DIG_COUNT,				// 挖宝次数改变，客户端打开挖宝界面并提示 2012-04-16.
		RAID_GLOBAL_VARIABLE,				// 服务器定期通知客户端副本全局变量 Added 2012-04-13.

		RANDOM_TOWER_MONSTER,				// 420 通知客户端弹出随机选怪界面 Added 2012-04-17.
		RANDOM_TOWER_MONSTER_RESULT,		// 服务器通知客户端随机选怪结果 Added 2012-04-17.
		PLAYER_TOWER_DATA,					// 玩家趴塔副本相关信息数据 Added 2012-04-17.
		PLAYER_TOWER_MONSTER_LIST,			// 玩家趴塔副本怪物图鉴列表 Added 2012-04-17.
		PLAYER_TOWER_GIVE_SKILLS,			// 趴塔副本技能手牌 Added 2012-04-17.

		SET_TOWER_GIVE_SKILL,				// 425 单个手牌使用后通知客户端改变状态 Added 2012-04-17.
		ADD_TOWER_MONSTER,					// 获得趴塔副本一个怪物图鉴 Added 2012-04-17.
		SET_TOWER_AWARD_STATE,				// 趴塔副本奖励领取成功后发送给客户端状态协议 Added 2012-04-17.
		LITTLEPET_INFO,						// 
		RUNE_INFO,							// 元魂相关信息

		RUNE_REFINE_RESULT,					// 430元魂洗练结果是否保留情况
		TOWER_LEVEL_PASS,					// 爬塔副本通过一关
		FILL_PLATFORM_MASK,					// 平台发过来的掩码，账号状态 Added 2012-05-18.
		PK_BET_DATA,						// pk竞猜相关数据
		PUT_ITEM,							// 包裹特定位置放物品
		
		OBJECT_START_SPECIAL_MOVE,			// 435玩家开始特殊移动 Added 2012-06-26.
		OBJECT_SPECIAL_MOVE,				// 玩家特殊移动消息 Added 2012-06-26.
		OBJECT_STOP_SPECIAL_MOVE,			// 玩家停止特殊移动 Added 2012-06-26.
		PLAYER_CHANGE_NAME,					// 改名local广播
		ENTER_COLLISION_RAID,				// 进入碰撞副本 Added 2012-08-01.

		COLLISION_RAID_SCORE,				// 440 碰撞副本积分信息 Added 2012-08-01.
		REPURCHASE_INV_DATA,				// 物品回购包裹信息
		COLLISION_SPECIAL_STATE_INFO,		// 碰撞副本中，受到技能击中后的状态信息 Added 2012-08-01.
		EXCHANGE_MOUNT_WING_ITEM,			// 坐骑飞剑包裹交换物品
		EXCHANGE_INVENTORY_MOUNTWING_ITEM,	// 普通包裹 <-> 坐骑飞剑包裹
		
		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM,	// 445 装备栏 <-> 坐骑飞剑包裹
		PLAYER_CHANGE_MOUNTWING_INV_SIZE,	// 坐骑飞剑包裹扩充
		PLAYER_DARK_LIGHT_VALUE,			// 轩辕当前光暗度值 Added 2012-08-16.
		PLAYER_DARK_LIGHT_STATE,			// 轩辕当前光暗状态 Added 2012-08-16.
		PLAYER_DARK_LIGHT_SPIRIT,			// 轩辕职业的灵

		MIRROR_IMAGE_CNT,					// 450 镜像数量
		CHANGE_FACTION_NAME,				// 帮派、家族改名广播
		PLAYER_WING_CHANGE_COLOR,			//	玩家飞剑颜色改变 Added 2012-08-20.
		UI_TRANSFER_OPENED_NOTIFY,	 	    // UI传送开启的传送点通告 Added 2012-08-21
		NOTIFY_ASTROLOGY_ENERGY,			// 星座精力值 Added 2012-08-23


		ASTROLOGY_UPGRADE_RESULT,			// 455 星座升级结果
	    ASTROLOGY_DESTROY_RESULT,			// 星座粉碎结果
        TALISMAN_REFINESKILL,				// 通知客户端法宝技能洗练结果
		COLLISION_RAID_RESULT,				// 碰撞副本最后输赢结果
		COLLISION_RAID_INFO,				// 个人碰撞副本积分信息

		DIR_VISIBLE_STATE,					//	460 方向技能攻击效果状态 Added 2012-09-05
		PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL,	// 玩家弥补未满级飞升数值 2012-10-16  by zy
		COLLISION_PLAYER_POS,				// 碰撞副本更新玩家位置
		LIVENESS_NOTIFY,					// 活跃度 2012-11-8 by zy
        ENTER_KINGDOM,                      //进入国战

		LEAVE_KINGDOM,                      // 465离开国战		
		ANCIENT_RAID_LEVEL_START,          //上古副本关卡开始
		ANCIENT_RAID_LEVEL_END,            //上古副本关卡结束
		ANCIENT_RAID_LEVEL_RESULT,         //上古副本关卡结果
		ANCIENT_RAID_LEVEL_INFO,           //上古副本关卡动态信息

		KINGDOM_HALF_END,                  // 470国战半场结束
		KINGDOM_END_RESULT,                //国战结束结果
		KINGDOM_MAFIA_INFO,                //国战帮派积分，杀敌数，死亡数
		SALE_PROMOTION_INFO,			   //已完成的台历道具前提条件(任务、成就)
		EVENT_START,					   //策略事件开始

		EVENT_STOP,						   // 475策略事件结束
		EVENT_INFO_ONLINE,				   //上线时已经激活的策略事件  
		CASH_GIFT_IDS,						// 商城陕赠品积分ID
		NOTIFY_CASH_GIFT,					// 商城积分兑奖通知
		PLAYER_PROPADD,						// 玩家基础属性增益

		PLAYER_GET_PROPADD_ITEM,			// 480 基础属性增益和合成 
		PLAYER_EXTRA_EQUIP_EFFECT,			// 武器炼器显示效果，非实际炼器等级
		KINGDOM_KEY_NPC_INFO,               //482 战场生命柱体显示
		NOTIFY_KINGDOM_INFO,				// 国王通知
		CSFLOW_BATTLE_PLAYER_INFO,			// 流水席战场

	

		PLAYER_BATH_INVITE = 485,            // 485 邀请搓澡
		PLAYER_BATH_INVITE_REPLAY,           // 邀请搓澡回复
		PLAYER_BATH_START,                   // 开始搓澡
		PLAYER_BATH_STOP,                    // 结束搓澡
		ENTER_BATH_WORLD,                    // 进入澡堂
		LEAVE_BATH_WORLD,                    // 490 离开澡堂
        BATH_GAIN_ITEM,                      // 玩家获得的奖励
        
	    CSFLOW_BATTLE_INFO = 492,			// 流水席战场信息
		KINGDOM_GATHER_MINE_START = 493,      //正在开启国王凭证
		FUWEN_COMPOSE_INFO,					// 符文碎片合成信息

		FUWEN_COMPOSE_END,					// 495符文碎片合成操作完成
		FUWEN_INSTALL_RESULT,				// 符文镶嵌结果
		FUWEN_UNINSTALL_RESULT,				// 符文移除结果
        MULTI_EXP_MODE = 498,                 //多倍经验
		EXCHANGE_ITEM_LIST_INFO,           //包裹整理物品交换列表
		WEB_ORDER_INFO,					   //WEB商城订单信息 

		BATTLE_OFF_LINE_INFO,				/// 501 跨服战场
		CONTROL_TRAP_INFO,					//可控陷阱
		NPC_INVISIBLE,						//NPC是否可见
		PUPPET_FORM,						//第几个傀儡

		TELEPORT_SKILL_INFO,				// 505 传送技能相关信息
		PLAYER_START_INTERACT,				// 可互动物品开始交互
		PLAYER_IN_INTERACT,					// 可互动物品在互动中
		PLAYER_END_INTERACT,				// 可互动物品结束互动
		PLAYER_FINISH_INTERACT,				// 可互动物品完成互动

		PLAYER_CANCEL_INTERACT,				// 510 可互动物品中断互动
		FASHION_COLORANT_PRODUCE,			// 合成物品返回
		FASHION_COLORANT_USE,				// 时装合成
		BLOW_OFF,						//击飞
		PLAYER_QILIN_INVITE,			//麒麟邀请
		
		PLAYER_QILIN_INVITE_REPLY,     //515 麒麟邀请恢复
		PLAYER_QILIN_START,				// 麒麟开始骑乘
		PLAYER_QILIN_STOP,				//麒麟结束骑乘
		PLAYER_QILIN_DISCONNECT,		//下麒麟
		PLAYER_QILIN_RECONNECT,			//上麒麟

		PHASE_INFO = 520,					// 相位信息
		PHASE_INFO_CHANGE,					// 相位改变信息
		OBTAIN_ACHIEVE_AWARD,				// 已获得成就奖励

		FAC_BUILDING_ADD,						// 帮派建筑添加
		FAC_BUILDING_UPGRADE_FAC,   // 帮派建筑升级
		
		FAC_BUILDING_REMOVE,			// 525 帮派建筑删除
		FAC_BUILDING_COMPLETE,			// 帮派完整?
		INVITE_ACTIVE_EMOTE,			// 邀请做互动表情
		REPLY_ACTIVE_EMOTE,				// 回复互动表情的邀请
		PLAY_ACTIVE_EMOTE,				// 播放互动表情

		STOP_ACTIVE_EMOTE,			// 530 取消互动表情
		PLAYER_FAC_BASE_INFO,
		DUNGEON_PROGRESS,				// 副本进度
		FAC_BASE_PROP_CHANGE,			// 如果基地的属性有变化，会主动通知基地内的所有成员。
		BE_TAUNTED2,					// 嘲讽

		COMBINE_MINE_STATE_CHANGE = 535,	// 535 改变采矿
		GET_FACBASE_MALL_INFO,				// 帮派商城 激活物品消息
		GET_FACTION_COUPON,					// 获取帮派金券
		GET_FACBASE_MONEY_ITEMS_INFO,	// 获取帮派元宝商城冷却时间
		PLAYER_CHANGE_PUPPET_FORM,		// 傀儡战斗状态

		BATTLE_DUNGEON_SSK_INFO = 540,	// 540 十神宝库副本信息
		NOTIFY_BLOODPOOL_STATUS = 541,	// 血槽
		PLAYER_FACBASE_AUCTION = 542,		// 返回帮派拍卖行列表
		FACBASE_AUCTION_UPDATE, // 基地拍卖信息变化
		FAC_COUPON_RETURN,  // 更新拍卖基地退回的金劵

		OBJECT_TRY_CHARGE = 545,	//试图冲锋
		OBJECT_BE_CHARGE_TO,     //被冲锋
		PLAYER_START_TRAVEL_AROUND, //新手村飞行效果开始
		PLAYER_STOP_TRAVEL_AROUND, // 新手村飞行效果结束
		PLAYER_GET_SUMMON_PETPROP,

		GET_FACBASE_CASH_ITEM_SUCCESS, // 550元宝商城购买成功
		FACBASE_AUCTION_ADD_HISTORY,   // 增加拍卖行记录
		FACBASE_MSG_UPDATE, // 更新基地留言
	};
	
	struct cmd_header
	{
        unsigned short cmd;
	};

	struct cmd_mall_item_price
	{
		int count;
		
		struct
		{
			int   good_index;     //在商城里的index
			int   good_id;        //物品id
			int   remaining_time; //限时销售的剩余时间
		}list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(list);
			if (buf_size < sz)
				return false;

			sz += count * sizeof(list);
			return buf_size >= sz;
		}
	};

	//	object 离开可见区域
	struct cmd_leave_slice
	{
		int id;
	};

	//	player更新位置
	struct cmd_notify_hostpos
	{
		A3DVECTOR3 vPos;
		int tag;
	};

	//	player list
	struct cmd_player_info_1_list
	{
		unsigned short count;
		info_player_1* players;

		GP_S2C_CHECKDATA(cmd_player_info_1_list)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~cmd_player_info_1_list();
	};

	struct cmd_player_info_2_list 
	{
		unsigned short count;
		/*
		   struct		//假设的代码，实际上由于info2是变长的结构，所以无法这样组织
		   {
		   int cid;
		   player_info_2 info;
		   }list[];
		 */
		char data[1];
	};

	struct cmd_player_info_3_list 
	{
		unsigned short count;
		/*
		   struct		//假设的代码，实际上由于info3是变长的结构，所以无法这样组织
		   {
		   int cid;
		   player_info_3 info;
		   }list[];
		 */
		char data[1];
	};

	struct cmd_player_info_23_list 
	{
		unsigned short count;
		/*
		   struct		//假设的代码，实际上由于info2/3是变长的结构，所以无法这样组织
		   {
		   int cid;
		   player_info_2 info2;
		   player_info_3 info3;
		   }list[];
		 */
		char data[1];
	};

	struct cmd_npc_info_list 	//	npc list
	{
		unsigned short count;
		info_npc* npcs;

		GP_S2C_CHECKDATA(cmd_npc_info_list)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~cmd_npc_info_list();
	};

	struct cmd_matter_info_list
	{
		unsigned short count;
		info_matter* matters;

		GP_S2C_CHECKDATA(cmd_matter_info_list)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~cmd_matter_info_list() { delete[] matters; }
	};

	struct cmd_object_move
	{
		int id;
		A3DVECTOR3 dest;
		unsigned short use_time;
		short sSpeed;				//	Move speed 8.8 fix-point
		unsigned char move_mode;
	};

	//玩家被牵引/拉扯到目标位置，由gs来不断改变位置 Added 2011-07-27.
	struct cmd_object_be_moved
	{
		int				id;			//玩家ID，目前仅仅供HostPlayer使用
		A3DVECTOR3		dest;		//所需要移动到的目标位置
		unsigned short	use_time;	//移动所需要花费的时间
		short			speed;		//移动速度，使用定点数存储
		unsigned char	move_mode;	//移动方式，目前不需要使用！！！！
	};
	//Added end.

	//-------------------------------------------------------------------------
	//服务器通知玩家 特殊移动 相关消息 Added 2012-06-26.
	//开始特殊移动
	struct cmd_object_start_special_move
	{
		int				id;				//玩家ID
		A3DVECTOR3		velocity;		//玩家当前的速度向量
		A3DVECTOR3		acceleration;	//玩家当前的加速度向量
		A3DVECTOR3		cur_pos;		//当前位置，用于校验
		char			collision_state;//当前碰撞状态 0：没有碰撞， 1：发生了碰撞
		int				time_stamp;		//时间戳，当前发协议的时间，单位秒，注意对时！
	};

	//移动消息
	struct cmd_object_special_move
	{
		int				id;				//玩家ID
		A3DVECTOR3		velocity;		//玩家当前速度向量
		A3DVECTOR3		acceleration;	//玩家当前加速度向量
		A3DVECTOR3		cur_pos;		//玩家当前位置向量
		char			collision_state;//碰撞状态 0：没有碰撞 1：发生了碰撞
		int				time_stamp;		//时间戳，当前发协议的时间，单位秒，注意对时！
	};

	//停止移动消息
	struct cmd_object_stop_special_move
	{
		int				id;				//玩家ID
		unsigned char	cur_dir;		//玩家当前的朝向
		A3DVECTOR3		cur_pos;		//玩家当前的位置
	};
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//服务器通知玩家 关于碰撞副本相关协议信息 Added 2012-08-01.
	//进入碰撞副本时相关信息.
	struct cmd_enter_collision_raid
	{
		int				id;				//玩家ID
		//int				transfigure_id;	//进入副本后变身ID
		//int				skill_id[2];	//所选择的技能ID
		unsigned char	raid_faction;	//0:中立 1：攻方 2：守方 3：旁观
		unsigned char	raid_type;		//
		int				raid_id;		//副本ID
		int				end_timestamp;	//结束时间	
	};

	//实时通知当前双方的比分
	struct cmd_collision_raid_score
	{
		int				attacker_score;	//红方得分
		int				defender_score;	//蓝方得分
		int				max_killer_id;
		int				count;
		struct 
		{
			int roleid;
			int faction;
			int kill_count;
			int dead_count;
		}member_info[1];
		
		
		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(member_info);
			if (buf_size < sz)
				return false;
			
			sz += count * sizeof(member_info);
			return buf_size >= sz;
		}
	};

	//受到击中技能后的状态信息
	struct cmd_collision_special_state_info
	{
		int				role_id;		//角色ID
		int				skill_type;		//技能类型ID
		char			on;				//0-停止， 1-开始
	};
	//Added end.
	//-------------------------------------------------------------------------

	//-------------------------------------------------------------------------
	//Added 2012-08-16.
	struct cmd_player_dark_light_value
	{
		int				id;					//玩家ID
		int				dark_light_value;	//当前光暗值
	};

	struct cmd_player_dark_light_state
	{
		int				id;					//玩家ID
		char			dark_light_state;	//0: 普通，1：初等暗使，2：初等光使， 3：高等暗使， 4：高等光使， 5：光暗合一(暗)，6：光暗合一(光)
	};
	//Added end.

	//Added 2012-09-05. 用于十字范围技能攻击状态效果特效控制
	struct cmd_dir_visible_state
	{
		int				role_id;			//技能释放者ID
		int				state_id;			//状态效果ID
		int				dir;				//0-360度
		bool			on;					//开启状态或者关闭状态
	};
	//-------------------------------------------------------------------------
	struct cmd_player_wing_change_color
	{
		int				id;					//玩家ID
		unsigned char	wing_color;			//飞剑变色索引值
	};
	//-------------------------------------------------------------------------

	struct cmd_player_dark_light_spirit
	{
		int				id;						//玩家ID
		char			dark_light_spirit[3];	//1: 暗灵，2：光灵，3：合一灵
	};

	struct cmd_mirror_image_cnt
	{
		int				id;					//玩家ID
		char			mirror_image_cnt;	//当前镜像的数量
	};
	
	struct cmd_change_faction_name
	{
		int				id;					//玩家ID
		int				fid;
		char			type;				// 0 -帮派  1 -家族
		int				name_len;			// 名字长度
		char			name[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(name);
			
			if (buf_size < sz)
				return false;
			
			sz += name_len;
			return buf_size >= sz;
		}
	};
	
	struct cmd_ui_transfer_opened_notify
	{
		unsigned short length;				// 长度
		//int opened_ui_transfer_indexs[];  // 接下来的数据是打开的传送点索引, 长度由length决定	
	};

	struct cmd_notify_astrology_energy
	{
		bool active;		// 是否开启精力槽的显示
		int  cur_energy;	// 当前星座精力值
	};

	struct cmd_astrology_upgrade_result
	{
		bool success;
		int  new_level;
	};

	struct cmd_astrology_destory_result 
	{
		int gain_energy;	// 粉碎星座获得的精力值
	};

	struct talisman_refineskill
	{
		int id;
		int index;
	};

	struct collision_raid_result
	{
		char result;		// 0-draw 1-win 2-lose
		int score_a;
		int score_b;
	};

	struct collision_raid_info
	{
		int score_a;
		int score_b;
		int win_num;
		int lost_num;
		int draw_num;
		int daily_award_timestamp;		// 上次领取每日奖励时间戳
		int last_collision_timestamp;	// 上次参加碰撞战场时间戳
		int day_win;					// 最近一次参加战场当天的胜场数
		int cs_personal_award_timestamp;  //跨服6v6个人跨服每周分档奖励 
		int cs_exchange_award_timestamp; //跨服6v6每周兑换币奖励
		int cs_team_award_timestamp;    //跨服6v6战队赛季奖励
	};

	//	player 离开游戏
	struct cmd_player_leave_world
	{
		int id;
	};

	struct cmd_npc_died
	{
		int id;
		int idKiller;
	};

	struct cmd_object_disappear
	{
		int id;
		unsigned char at_once;
	};

	struct cmd_host_stop_attack
	{
		int	iReason;		//	Stop reason
	};

	struct cmd_host_attack_result
	{
        int idTarget;
        int iDamage;				//	如果是0表示没有击中
        unsigned char attack_flag;	//	标记该攻击是否有攻击优化符和防御优化符和重击发生
		unsigned char attack_speed;
	};

	struct cmd_error_msg
	{
		int	iMessage;
	};

	struct cmd_host_attacked		// with complete information from object_attack_result
	{
		int idAttacker;
		int	iDamage;
		char cEquipment;	//	The equipment which is mangled, 高位代表这次攻击是不是应该变橙色

		unsigned char attack_flag;      //标记该攻击是否有攻击优化符和防御优化符和重击发生
		char speed;                     //攻击速度 speed * 50 ms
	};

	struct cmd_player_died
	{
		int idKiller;
		int idPlayer;
	};

	struct cmd_host_died
	{
		int idKiller;
		A3DVECTOR3 pos;
		int revive_countdown;	// 复活时间倒计时
	};

	struct cmd_player_revive
	{
		int idPlayer;
		short sReviveType;	//	Revive type
		A3DVECTOR3 pos;
	};

	struct cmd_pickup_money
	{
		int	iAmount;
	};

	struct cmd_pickup_item
	{
		int	tid;		//	id of template
		int expire_date; // expiration date
		unsigned short wAmount;		//	Total amoumt
		unsigned short wSlotAmount;	//	Last slot amount
		unsigned char byPackage;	
		unsigned char bySlot;		//	Last slot's index
		int state;
	};

	struct cmd_player_info_00
	{
		int	idPlayer;
		short sLevel;
		short sDeityLevel;		// 元神等级
		unsigned char State;
		unsigned char Level2;
		int	iHP;
		int	iMaxHP;
		int	iMP;
		int	iMaxMP;
		int iDP;				// 神力
		int iMaxDP;				// 神力上限
		int iRage;				// 怒气值
	};

	struct cmd_npc_info_00
	{
		int	idNPC;
		int	iHP;
		int	iMaxHP;
	};

	struct cmd_out_of_sight_list
	{
		unsigned int uCount;
		int* id_list;

		GP_S2C_CHECKDATA(cmd_out_of_sight_list)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct cmd_object_stop_move
	{
		int id;
		A3DVECTOR3 dest;
		short sSpeed;				//	Move speed 8.8 fix-point
		unsigned char dir;
		unsigned char move_mode;
	};

	struct cmd_matter_enter_world
	{
		info_matter	Info;
	};

	struct cmd_receive_exp
	{
		__int64 exp;
	};
	
	struct cmd_level_up
	{
		int idPlayer;
	};

	struct cmd_self_info_1
	{
		double exp;
		int cid;
		A3DVECTOR3 pos;
		unsigned short crc_e;
		unsigned char  dir;		//256的定点数表示度数
		unsigned char profession;
		unsigned char  level;
		unsigned char reborn_count;
		unsigned char dt_level;
		char pk_level;
		char cultivation;
		int reputation;
		int hide_equip;
		int state;
		int extra_state;
		//	if (state & GP_STATE_ADV_MODE), 2 DWORD inserted here
		//	if (state & GP_STATE_SHAPE), 1 char inserted here
		//	if (state & GP_STATE_EMOTE), 1 char inserted here
		//	If (state & GP_STATE_EXTEND_PROPERTY), 1 DWORD inserted here
		//	If (state & GP_STATE_FACTION), 1 int inserted here
		//	If (state & GP_STATE_BOOTH), 1 char inserted here
		//	If (state & GP_STATE_EFFECT), effect data inserted here
		//	If (state & GP_STATE_PARIAH), 1 char inserted here
		//	If (state & GP_STATE_IN_MOUNT), 1 char + 1 int inserted here
		//	If (state & GP_STATE_IN_BIND), 1 char + 1 int inserted here
		//	If (state & GP_STATE_TITLE), 1 short inserted here
		//	If (state & GP_STATE_SPOUSE), 1 int inserted here
		//	If (state & GP_STATE_FAMILY), 1 int + 1 char inserted here
		//	If (state & GP_STATE_SECT_MASTER_ID), 1 int inserted here

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			if (buf_size < sizeof(*this))
				return false;

			sz = sizeof(*this);

			if (state & GP_STATE_ADV_MODE)
				sz += sizeof(DWORD) * 2;

			if (state & GP_STATE_SHAPE)
				sz += sizeof(char);

			if (state & GP_STATE_EMOTE)
				sz += sizeof(char);

			if (state & GP_STATE_EXTEND_PROPERTY)
				sz += sizeof(__int64);
		
			if (state & GP_STATE_FACTION)
				sz += sizeof(int);
			
			if (state & GP_STATE_BOOTH)
			{
				sz += sizeof(char);
				sz += sizeof(int);
			}

			if (state & GP_STATE_EFFECT)
			{
				const BYTE* pData = reinterpret_cast<const BYTE*>(this) + sz;
				sz += sizeof(BYTE);

				if (buf_size < sz)
					return false;

				sz += *pData * sizeof(short);
			}

			if (state & GP_STATE_IN_MOUNT)
				sz += sizeof (char) + sizeof (int) + sizeof(char);

			if (state & GP_STATE_IN_BIND)
				sz += sizeof (char) + sizeof (int) + sizeof(char);

			if (state & GP_STATE_TITLE)
				sz += sizeof (short);

			if (state & GP_STATE_SPOUSE)
				sz += sizeof(int);

			if (state & GP_STATE_FAMILY)
				sz += sizeof(int) + sizeof(char);

			if (state & GP_STATE_SECT_MASTER_ID)
				sz += sizeof(int);

			if (state & GP_STATE_IN_SJBATTLE)
				sz += sizeof(char);
			
			if (state & GP_STATE_IN_VIPSTATE)
				sz += sizeof(char);
			
			if (state & GP_STATE_TRANSFIGURE)
				sz += sizeof(int);

			if (extra_state & GP_EXTRA_STATE_CARRIER)
				sz += sizeof(int) + 3*sizeof(float) +sizeof(char);
			
			if (extra_state & GP_EXTRA_STATE_CIRCLE)
				sz += sizeof(int) + sizeof(char);
			
			if (extra_state & GP_EXTRA_STATE_CROSSZONE)
				sz += sizeof(int);

			if (extra_state & GP_EXTRA_STATE_VIP_AWARD)	//Added 2012-01-16.
			{
				sz += sizeof(unsigned char);
				sz += sizeof(char);					// 2012-8-10
			}

			if (extra_state & GP_EXTRA_STATE_COLLISION_RAID)	//Added 2012-08-01.
			{
				sz += (sizeof(int) * 2);
			}

			if (extra_state & GP_EXTRA_STATE_XY_SPIRITS)		// 轩辕灵
			{
				sz += (sizeof(char)*3);
			}
			
			if (extra_state & GP_EXTRA_STATE_MIRROR_IMAGE)		// 镜像数量
			{
				sz += sizeof(char);
			}

			if (extra_state & GP_EXTRA_STATE_COLLISION_RAID)	//Added 2012-08-01.
			{
				sz += (sizeof(int) * 2);
			}

			if (extra_state & GP_EXTRA_STATE_CHANGE_WING_COLOR)	//改变飞剑颜色 Added 2012-08-20.
			{
				sz += sizeof(unsigned char);
			}
			
			if (extra_state & GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT)
			{
				sz += sizeof(unsigned char);
			}
			if (extra_state & GP_EXTRA_STATE_FLOW_BATTLE)
			{
				sz += sizeof(short);
			}
			if(extra_state & GP_EXTRA_STATE_PUPPET)
			{
				sz += sizeof(int);
			}
			if(extra_state & GP_EXTRA_STATE_INTERACT)
			{
				sz += sizeof(int);
				sz += sizeof(char);
				sz += sizeof(char);
			}
			if (extra_state & GP_EXTRA_STATE_ACTIVE_EMOTE)
			{
				sz += sizeof(bool);
				sz += sizeof(int);
				sz += sizeof(int);
			}
			return buf_size >= sz;
		}
	};

	struct cmd_self_info_00
	{
		short sLevel;
		short sDeityLevel;				// 元神等级
		unsigned char State;
		unsigned char Level2;
		int	iHP;
		int	iMaxHP;
		int	iMP;
		int	iMaxMP;
		int iDP;						// 神力
		int iMaxDP;						// 神力上限
		int iTalismanStamina;
		double exp;
		double deityExp;				// 封神修为
//		int	ip;						    // 墨水值
		int irage;						// 怒气值
	};

	struct cmd_own_ivtr_info
	{ 
		unsigned char byPackage;
		unsigned char ivtr_size;
		size_t content_length;
		void* content;

		GP_S2C_CHECKDATA(cmd_own_ivtr_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct cmd_own_ivtr_detail_info
	{
		unsigned char byPackage;
		unsigned char ivtr_size;
		size_t content_length;
		void* content;

		GP_S2C_CHECKDATA(cmd_own_ivtr_detail_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

    struct cmd_own_item_info
	{
		unsigned char byPackage;
		unsigned char bySlot;
		int type;
		int expire_date;
		int state;
		size_t count;
		unsigned short crc;
		unsigned short content_length;
		void* content;

		GP_S2C_CHECKDATA(cmd_own_item_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
    };

    struct cmd_empty_item_slot
    {
		unsigned char byPackage;
		unsigned char bySlot;
    };

	struct cmd_exg_ivtr_item
	{
		unsigned char index1;
		unsigned char index2;
	};
	
	struct cmd_move_ivtr_item
	{       
        unsigned char src;
        unsigned char dest;
        unsigned short count;
	};
	
	struct cmd_player_drop_item
	{
        unsigned char byPackage;
        unsigned char bySlot;
        unsigned short count;
        int tid;
		unsigned char reason;
	};

	struct cmd_exg_equip_item
	{
        unsigned char index1;
        unsigned char index2;
	};

	struct cmd_equip_item
	{
        unsigned char index_inv;
        unsigned char index_equip;
		unsigned short count_inv;
		unsigned short count_equip;
	};

	struct cmd_move_equip_item
	{
        unsigned char index_inv;
        unsigned char index_equip;
        unsigned short amount;
	};

	struct cmd_own_ext_prop
	{
		size_t status_point;
		ROLEEXTPROP prop;
	};

	struct cmd_add_status_point
	{
		size_t vitality;
		size_t energy;
		size_t strength;
		size_t agility;
		size_t remain;		
	};

	struct cmd_select_target
	{
		int idTarget;
	};

	//	PLAYER_EXT_PROP_BASE
	struct cmd_pep_base
	{
		int	idPlayer;
		ROLEEXTPROP_BASE ep_base;
	};

	//	PLAYER_EXT_PROP_MOVE
	struct cmd_pep_move
	{
		int idPlayer;
		ROLEEXTPROP_MOVE ep_move;
	};

	//	PLAYER_EXT_PROP_ATK
	struct cmd_pep_attack
	{
		int idPlayer;
		ROLEEXTPROP_ATK ep_attack;
	};

	//	PLAYER_EXT_PROP_DEF
	struct cmd_pep_def
	{
		int idPlayer;
		ROLEEXTPROP_DEF ep_def;
	};

	struct cmd_team_leader_invite
	{
		int idLeader;
		int seq;
		unsigned short wPickFlag;
        int familyid;				// 邀请者的家族id
        int mafiaid;				// 邀请者的帮派id
	};

	struct cmd_team_reject_invite
	{
		int idPlayer;		//	Who reject our invitation
	};

	struct cmd_team_join_team
	{
		int idLeader;
		unsigned short wPickFlag;
	};

	struct cmd_team_member_leave
	{
		int idLeader;
		int idMember;
		short reason;
	};

	//	表明自己离开了队伍
	struct cmd_team_leave_party
	{
		int idLeader;
		short reason;
	};

	struct cmd_team_new_member
	{
		int idMember;
	};

	struct cmd_team_leader_cancel_party
	{
		int idLeader;
	};

	struct cmd_team_member_data
	{
		unsigned char member_count;
		unsigned char data_count;
		int idLeader;

		struct MEMBER
		{
			int idMember;
			short level;
			short deityLevel;
			unsigned char state;
			char pk_level;
			char wallow_level;
			unsigned char class_id;
			int hp;
			int mp;
			int dp;
			int max_hp;
			int max_mp;
			int max_dp;
			int family_id;
			int master_id;
			char reborn_cnt;
			int rage;

		} data[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(data);

			if (buf_size < sz)
				return false;

			sz += data_count * sizeof(MEMBER);
			return buf_size >= sz;
		}
	};

	struct cmd_team_member_pos
	{
		int idMember;
		A3DVECTOR3 pos;
		int idInst;
	};

	struct cmd_equip_data
	{
		unsigned short crc;
		int idPlayer;
		int mask;
		int data[1];     //0 ~ 12

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(data);

			if (buf_size < sz)
				return false;

			for (int i = 0; i < 32; i++)
			{
				if (mask & (1 << i))
					sz += sizeof(int);
			}

			return buf_size >= sz;
		}
	};

	struct cmd_equip_data_changed
	{
		unsigned short crc;
		int idPlayer;
		int mask_add;
		int mask_del;
		int data_add[1]; //0 ~ 12

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(data_add);

			if (buf_size < sz)
				return false;

			for (int i = 0; i < 32; i++)
			{
				if (mask_add & (1 << i))
					sz += sizeof(int);
			}

			return buf_size >= sz;
		}
	};

	struct cmd_equip_damaged
	{
		unsigned char index;
	};

	struct cmd_team_member_pickup
	{
		int idMember;
		int tid;
		int count;
	};

	struct cmd_npc_greeting
	{
		int idObject;	//	ID of NPC or player
	};

	struct cmd_npc_sev_content
	{
		int idNPC;
		int type;       //	服务的类型
		size_t length;
		char data[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(data);

			if (buf_size < sz)
				return false;

			sz += length;
			return buf_size >= sz;
		}
	};

	struct cmd_purchase_item
	{
		int    trade_id;
		size_t cost;
		unsigned char flag;		//	1, booth_slot is valid
		unsigned short item_count;

		struct ITEM
		{
			int item_id;
			int expire_date;
			unsigned short count;
			unsigned short inv_index;
			unsigned char booth_slot;
			int item_state;

		} item_list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(item_list);

			if (buf_size < sz)
				return false;

			sz += item_count * sizeof(ITEM);
			return buf_size >= sz;
		}
	};

	struct cmd_item_to_money
	{
		unsigned short index;           //	在包裹栏里的索引号码
		int type;                       //	物品的类型
		size_t count;
		size_t money;
	};

	struct cmd_repair_all
	{
		size_t cost;
	};

	struct cmd_repair
	{
		unsigned char byPackage;
		unsigned char bySlot;
		size_t cost;
	};

	struct cmd_spend_money
	{
		size_t cost;
	};

	struct cmd_gain_money_in_trade
	{
		int amount;
	};

	struct cmd_gain_item_in_trade
	{
		int type;
		unsigned short amount;
	};

	struct cmd_gain_money_after_trade
	{
		size_t amount;
	};

	struct cmd_gain_item_after_trade
	{
		int tid;
		int expire_date;			// expiration date
		unsigned short amount;
		unsigned short wSlotAmount;	//	Last slot amount
		unsigned short index;
		int state;
	};

	struct cmd_get_own_money
	{
		size_t amount;
		size_t max_amount;
	};

	struct cmd_host_start_attack
	{
		int idTarget;		//	target id
		unsigned short ammo_remain;
		unsigned char attack_speed;
	};

	struct cmd_object_start_attack
	{
		int idAttacker;		//	attacker id
		int idTarget;		//	target id
		unsigned char attack_stamp;
	};

	struct cmd_object_cast_skill
	{
		int caster;
		int skill;
		unsigned short time;
		unsigned char level;
		unsigned char attack_stamp;
		unsigned char state;
		short cast_speed_rate;	//Added 2011-06-17.
		A3DVECTOR3 cast_skill_pos;	// Added 2012-05-04. 位置技能（玩家设定技能释放的中心位置）也使用这个接口
		char spirit_index;//0-2,灵的index
		int target_cnt;
		int targets[1];		
	};

	struct cmd_skill_interrupted
	{
		int caster;
		char spirit_index;	//0-2,灵的index
	};

	struct cmd_skill_perform
	{
		char spirit_index;	//0-2,灵的index
	};

	struct cmd_self_skill_interrupted
	{
		unsigned char reason;
		char spirit_index;	//0-2,灵的index
	};

	struct cmd_skill_data
	{
		size_t skill_count;

		struct SKILL
		{
			short id_skill;
			unsigned char level;

		} skill_list[1];
		
		size_t skill_count2;
		struct SKILL2 
		{
			short id_skill;
			int	proficiency; 
		}skill_list2[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			const BYTE* pData = (const BYTE*)this;
			size_t len;
			sz = sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
			pData += sizeof(size_t);
			sz += len*sizeof(SKILL);

			if (buf_size < sz)
				return false;

			pData += len*sizeof(SKILL);
			sz += sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
//			pData += sizeof(size_t);
			sz += len*sizeof(SKILL2);

			if (buf_size < sz)
				return false;

			return buf_size >= sz;

// 			sz = sizeof(*this) - sizeof(skill_list);
// 
// 			if (buf_size < sz)
// 				return false;
// 
// 			sz += skill_count * sizeof(SKILL);
// 			return buf_size >= sz;
		}
	};

	struct cmd_skill_elems_data
	{
		int count;
		struct SkillElems
		{
			int skill_id;
			int curr_num; 
			unsigned short ids[MAX_SKILL_ELEM_NUM];
		}skillelems[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			const BYTE* pData = (const BYTE*)this;
			size_t len;
			sz = sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
			pData += sizeof(size_t);
			sz += len*sizeof(SkillElems);

			if (buf_size < sz)
				return false;

			return buf_size >= sz;
		}
	};
	struct cmd_host_use_item
	{
		unsigned char byPackage;
		unsigned char bySlot;
		int item_id;
		unsigned short use_count;
	};

	struct cmd_embed_item
	{                                       
		unsigned char chip_idx;
		unsigned char equip_idx;
	};

	struct cmd_clear_tessera
	{
		unsigned short equip_idx;
		size_t cost;
	};

	struct cmd_cost_skill_point
	{
		int skill_point;
	};

	struct cmd_learn_skill
	{
		int skill_id;
		int skill_level;
	};

	struct cmd_object_takeoff
	{
		int object_id;
	};

	struct cmd_object_landing
	{
		int object_id;
	};

	struct cmd_flysword_time
	{
		unsigned char where;
		unsigned char index;
		int cur_time;
	};

	struct cmd_host_obtain_item
	{
		int type;
		int expire_date;
		unsigned short amount;
		unsigned short slot_amount;
		unsigned char where;            //在哪个包裹栏，0 标准，2 任务，1 装备
		unsigned char index;            //最后部分放在哪个位置
		int state;
	};      
 
	struct cmd_produce_start
	{
		unsigned short use_time;
		unsigned short count;
		int type;
	};

	struct cmd_produce_once
	{
		int type;
		unsigned short amount;
		unsigned short slot_amount;
		unsigned char where;            //在哪个包裹栏，0 标准，2 任务，1 装备
		unsigned char index;            //最后部分放在哪个位置
		int state;
	};
	
	struct cmd_decompose_start
	{
		unsigned short use_time;
		int type;
	};

	struct cmd_task_data
	{
		size_t active_list_size;
		char active_list[1];
		size_t finished_list_size;
		char finished_list[1];
		size_t finished_time_list_size;
		char finished_time_list[1];		

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			const BYTE* pData = (const BYTE*)this;
			size_t len;
			sz = sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
			pData += sizeof(size_t);
			sz += len;

			if (buf_size < sz)
				return false;

			pData += len;
			sz += sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
			pData += sizeof(size_t);
			sz += len;

			if (buf_size < sz)
				return false;

			pData += len;
			sz += sizeof(size_t);

			if (buf_size < sz)
				return false;

			len = *(size_t*)pData;
			pData += sizeof(size_t);
			sz += len;

			return buf_size >= sz;
		}
	};

	struct cmd_task_var_data
	{
		size_t size;
		char data[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(data);

			if (buf_size < sz)
				return false;

			sz += size;
			return buf_size >= sz;
		}	
	};

	struct cmd_object_start_use
	{
		int user;
		int item;
		unsigned short time;
	};

	struct cmd_object_cancel_use
	{
		int user;
	};

	struct cmd_object_use_item
	{
		int user;
		int item;
	};

	struct cmd_object_start_use_t
	{
		int user;
		int target;
		int item;
		unsigned short time;
	};

	struct cmd_object_sit_down
	{
		int id;
	};      

	struct cmd_object_stand_up
	{
		int id;
	};

	struct cmd_object_do_emote
	{       
		int id;
		unsigned short emotion;
	};

	struct cmd_server_time
	{
		int time;
		int timezone_bias;
		int lua_version;
	};

	struct cmd_object_root
	{
		int id;
		A3DVECTOR3 pos;
	};

	struct pk_level_notify        //广播 通知周围玩家自己PK值等级的变化
	{
         int id;
         char pk_level; //此level定义和player_info_1中的pk_level 一致
	};

	struct pk_value_notify      //只有自己收到，告知自己的PK值
	{       
         int id;
         int value;
	};

	struct player_change_class   //通知玩家职业的改变 这个消息是广播消息
	{
		 int id; 
		 unsigned char new_cls;
	};

	struct cmd_hurt_result
	{
		int target_id;
		int damage;
	};

	struct cmd_host_stop_skill
	{
		char spirit_index;	//0-2,灵的index
	};
	struct cmd_object_atk_result
	{
		int attacker_id;
		int target_id;
		int damage;
		unsigned char attack_flag;      //标记该攻击是否有攻击优化符和防御优化符和重击发生
		unsigned char attack_stamp;
	};

	struct cmd_be_hurt
	{
		int attacker_id;
		int damage;
		unsigned char flag;		//	1, attacker 变橙名
	};

	struct cmd_update_ext_state
	{
		int id; 
		__int64 dwStates;		// 状态扩展从32位扩展到64位
	};

	struct cmd_attack_once
	{
		unsigned char ammo_num;
	};

	struct cmd_icon_state_notify
	{
		int id;
		unsigned short count;
		struct _buff_t
		{
			unsigned short state;
			unsigned short level;
			char		   overlay_cnt;

		} *buff_list;

		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct cmd_player_gather_start
	{
		int pid;                //	player id
		int mid;                //	mine id
		unsigned char use_time; //	use time in sec;
	};

	struct cmd_player_gather_stop
	{
		int pid;                //	player id
	};

	struct cmd_trashbox_pwd_changed
	{
		unsigned char has_passwd;
	};

	struct cmd_trashbox_pwd_state
	{
		unsigned char has_passwd;
	};

	struct cmd_trashbox_open
	{
		unsigned char where;
		unsigned short slot_size;
	};

	struct cmd_trashbox_wealth
	{
		size_t money;
	};

	struct cmd_exg_trashbox_item
	{
		unsigned char where;
		unsigned char idx1;
		unsigned char idx2;
	};

	struct cmd_move_trashbox_item
	{
		unsigned char where;
		unsigned char src;
		unsigned char dest;
		size_t amount;
	};

	struct cmd_exg_trashbox_ivtr
	{
		unsigned char where;
		unsigned char idx_tra;
		unsigned char idx_inv;
	};

	struct cmd_trash_item_to_ivtr
	{
		unsigned char where;
		unsigned char src;
		unsigned char dest;
		size_t amount;
	};

	struct cmd_ivty_item_to_trash
	{
		unsigned char where;
		unsigned char src;
		unsigned char dest;
		size_t amount;
	};

	struct cmd_exg_trash_money
	{
		int inv_delta;		//	自己的钱数变化
		int tra_delta;		//	仓库的钱数变化
	};
	
	//新增随身包裹
	struct cmd_exg_pocket_item
	{
		unsigned char idx1;
		unsigned char idx2;
	};

	struct cmd_move_pocket_item
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};

	struct cmd_exg_pocket_ivtr
	{
		unsigned char idx_poc;
		unsigned char idx_inv;
	};

	struct cmd_pocket_item_to_ivtr
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};

	struct cmd_ivtr_item_to_pocket
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};
	
	struct cmd_change_pocket_size
	{
		int new_size;
	};

	struct cmd_enchant_result
	{
		int caster;
		int target;
		int skill;
		char level;
		char orange_name;
		char modifier;
		unsigned char attack_stamp;
		int damage;
	};

	struct cmd_host_notify_root
	{
		A3DVECTOR3 pos;
		unsigned char type;
	};

	struct cmd_host_dispel_root
	{
		unsigned char type;
	};

	struct cmd_obj_do_action
	{
        int id;
        unsigned char action;
	};

	struct record_start   //录像开始
	{
        int record_stamp; //本次录像起始索引，每次record_check_result都会使得本索引增1
	};

	struct cmd_object_skill_attack_result
	{
		int attacker_id;
		int target_id;
		int skill_id;
		int damage;
		int dt_damage;
		unsigned char attack_flag;      //标记该攻击是否有攻击优化符和防御优化符和重击发生
		unsigned char attack_stamp;
	};

	struct cmd_host_skill_attacked
	{
		int		idAttacker;
		int     idSkill;
		int		iDamage;
		char	cEquipment;				//	The equipment which is mangled, 高位代表这次攻击是不是应该变橙色

		unsigned char attack_flag;      //标记该攻击是否有攻击优化符和防御优化符和重击发生
		char speed;                     //攻击速度 speed * 50 ms
	};

	struct cmd_player_set_adv_data
	{
		int id;
		int data1;
		int data2;
	};

	struct cmd_player_clr_adv_data
	{
		int id;
	};

	struct cmd_player_in_team
	{
		int id;
		unsigned char state;    // 0 no team 1, leader, 2 member
	};

	struct cmd_team_ask_join
	{
		int idAsker;
        int familyid;			// asker family id
        int mafiaid;			// asker mafia id
	};

	struct cmd_object_emote_restore
	{
		int id;
		unsigned short emotion;
	};

	struct cmd_con_emote_request
	{
		int id;
		unsigned short emotion;
	};

	struct cmd_do_concurrent_emote
	{
		int id1;
		int id2;
		unsigned short emotion;
	};

	struct cmd_matter_pickup
	{
		int matter_id;
		int who;
	};

	struct cmd_mafia_info_notify
	{
		int idPlayer;				//	player id
		int idFaction;				//	帮派的 id
		int idFamily;
		unsigned char player_rank;	//	玩家在帮派内的等级
	};

	struct cmd_task_deliver_item
	{
		int type;
		int expire_date;
		unsigned short amount;
		unsigned short slot_amount;
		unsigned char where;            //在哪个包裹栏，0 标准，2 任务，1 装备
		unsigned char index;            //最后部分放在哪个位置
		int			state;				//item状态：绑定，装备后绑定，锁定 Added 2011-05-12
	};

	struct cmd_task_deliver_rep
	{
		int delta;
		int cur_reputaion;
	};

	struct cmd_task_deliver_exp
	{
		__int64 exp;
		int sp;
		int deity_exp;
	};

	struct cmd_task_deliver_money
	{
		size_t amount;
		size_t cur_money;
	};

	struct cmd_task_deliver_level2
	{
		int id_player;
		int level2;
	};

	struct cmd_player_reputation
	{
		int who;
		int reputation;
	};

	struct cmd_item_identify
	{
		unsigned char ivtr_index;
		unsigned char result;		//  0, success
	};

	struct cmd_player_chgshape
	{
		int idPlayer;
		unsigned char shape;
	};

	//	准备摆摊
	struct cmd_open_booth_test
	{
		int slot_index;
		int item_id;
	};

	struct cmd_player_open_booth
	{
		int item_id;
		int pid;
		unsigned char booth_crc;
		unsigned char name_len;		//	name length in bytes
		char name[1];				//	最大28字节

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(name);

			if (buf_size < sz)
				return false;

			sz += name_len;
			return buf_size >= sz;
		}
	};

	struct cmd_self_open_booth
	{
		int slot_index;
		int item_id;
		unsigned short count;

		struct item_t
		{
			int type;               //	物品类型
			unsigned short index;
			unsigned short count;   //	卖多少个
			size_t price;           //	单价

		} goods_list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(goods_list);

			if (buf_size < sz)
				return false;

			sz += count * sizeof(item_t);
			return buf_size >= sz;
		}
	};

	struct cmd_player_close_booth
	{
		int pid;
	};

	struct cmd_player_booth_info
	{
		int pid;
		int booth_id;
		size_t count;
		char goods_list[1];

	/*  each booth good struct
		union booth_goods
		{
			struct
			{
				int type;		//	物品类型, 0 表示用这个结构

			} empty_item;

			struct
			{
				int type;       //	物品类型
				int count;		//	> 0
				size_t price;   //	单价
				int expire_date;//  过期时间
				int state;		//	特殊状态
				unsigned short content_length;
				char content[];

			} sell_item;

			struct
			{
				int type;       //	物品类型
				int count;		//	< 0
				size_t price;   //	单价

			} buy_item;

		} item_list[];	*/

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			#define CHECK_DATA(type) \
			{\
				if (buf_size < sizeof(type))\
					return false;\
				buf_size -= sizeof(type);\
				sz += sizeof(type);\
			}

			sz = sizeof(*this) - sizeof(goods_list);

			if (buf_size < sz)
				return false;

			buf_size -= sz;
			const BYTE* pData = (const BYTE*)goods_list;
			size_t iCount = 0;

			while (iCount < count)
			{
				CHECK_DATA(int)
				int idItem = *(int*)pData;
				pData += sizeof(int);

				if (idItem)
				{
					CHECK_DATA(int)
					int iAmount = *(int*)pData;
					pData += sizeof (int);

					CHECK_DATA(int)
					int iPrice = *(int*)pData;
					pData += sizeof (int);

					if (iAmount > 0)
					{
						CHECK_DATA(int)
						int iExpireDate = *(int*)pData;
						pData += sizeof (int);

						CHECK_DATA(int)
						int iState = *(int*)pData;
						pData += sizeof (int);

						CHECK_DATA(WORD)
						WORD wDataLen = *(WORD*)pData;
						pData += sizeof (WORD);

						if (buf_size < wDataLen)
							return false;

						buf_size -= wDataLen;
						sz += wDataLen;
						pData += wDataLen;
					}
				}

				iCount++;
			}

			return true;
		}
	};

	struct cmd_booth_trade_success
	{
		int trader;
	};

	struct cmd_booth_name
	{
		int pid;
		unsigned char crc_name;		//	Booth name
		unsigned char name_len;		//	name length in bytes
		char name[1];				//	最大28字节

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(name);

			if (buf_size < sz)
				return false;

			sz += name_len;
			return buf_size >= sz;
		}
	};

	struct cmd_player_start_travel	//	广播
	{
		int pid;
		unsigned char vehicle;
	};

	struct cmd_host_start_travel
	{
		float speed;				//	速度增量（可能也可以从交通工具决定）
		A3DVECTOR3 dest;			//	目标点
		int line_no;				//	航线号
		unsigned char vehicle;		//	交通工具代号
	};

	struct cmd_player_end_travel	//	广播
	{
		int pid;
		unsigned char vehicle;
	};

	struct gm_cmd_invisible
	{
		unsigned char is_visible;
	};

	struct gm_cmd_invincible
	{
		unsigned char is_invincible;
	};

	struct cmd_host_correct_pos
	{
		A3DVECTOR3 pos;
		unsigned short stamp;
	};

	struct cmd_object_cast_instant_skill
	{
		int caster;
		int skill;
		unsigned char level;
		A3DVECTOR3 cast_skill_pos;		//Added 2012-05-04. 释放位置技能也使用这个接口
		char spirit_index;	//0-2,灵的index
		int target_cnt;
		int targets[1];
	};

	struct cmd_activate_waypoint
	{
		unsigned short waypoint;
	};

	struct cmd_waypoint_list
	{
		size_t count;                   
		unsigned short list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(list);

			if (buf_size < sz)
				return false;

			sz += count * sizeof(unsigned short);
			return buf_size >= sz;
		}
	};                                      

	struct cmd_unfreeze_ivtr_slot
	{
		unsigned char where;
		unsigned short index;
	};

	struct cmd_team_invite_timeout
	{
		 int who;
	};

	struct cmd_player_enable_pvp
	{
		 int who;
	};

	struct cmd_player_disable_pvp
	{
		 int who;
	};

	struct cmd_host_pvp_cooldown
	{
		 int cool_time;
		 int max_cool_time;
	};

	struct cmd_cooltime_data
	{
		unsigned short count;

		struct item_t
		{
			unsigned short idx;
			int cooldown;
			int max_cooltime;

		} list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(list);

			if (buf_size < sz)
				return false;

			sz += count * sizeof(item_t);
			return buf_size >= sz;
		}
	};

	struct cmd_skill_ability
	{
		int skill_id;
		int skill_ability;
	};

	struct cmd_breath_data
	{
		int breath;     
		int breath_capacity;
	};

	struct cmd_booth_sell_item
	{
		short inv_index;
		int item_type;
		size_t item_count;
		int idBuyer;
	};

	struct cmd_player_enable_fashion
	{
		int idPlayer;
		int fashion_mask;
		unsigned char is_enabble;
	};

	struct cmd_host_enable_freepvp
	{
		unsigned char enable_type;
	};

	struct cmd_invalid_object
	{
		int id;
	};

	struct cmd_player_enable_effect
	{
		short effect;
		int id;
	};

	struct cmd_player_disable_effect
	{
		short effect;
		int id;
	};

	struct cmd_revival_inquire
	{
		float exp_reduce;
	};

	struct cmd_set_cooldown
	{
		int cooldown_index;
		int cooldown_time;
	};

	struct cmd_change_team_leader
	{
		int old_leader;
		int new_leader;
	};

	struct cmd_exit_instance
	{
		int	idInst;
		int iLeftTime;
	};

	struct cmd_change_face_start
	{
		unsigned short ivtr_idx;
	};

	struct cmd_change_face_end
	{
		unsigned short ivtr_idx;
	};

	struct cmd_player_chg_face
	{
		unsigned short crc_c;
		int idPlayer;
	};

	struct cmd_object_cast_pos_skill
	{       
		int caster;
		A3DVECTOR3 pos;
		int skill;
		unsigned short time;
		unsigned char level;
	};
	
	struct cmd_set_move_stamp
	{
		unsigned short move_stamp;
	};

	struct cmd_inst_data_checkout
	{
		int idInst;
		unsigned int region_time_stamp;
		unsigned int precinct_time_stamp;
		unsigned int gshop_time_stamp;
	};

	struct cmd_host_rush_fly
	{
		unsigned char is_active;
	};

	struct cmd_trashbox_size
	{
		int iNewSize;
	};

	struct cmd_npc_died2
	{
		int id;
		int idKiller;
	};

	struct cmd_produce_null
	{
		int idRecipe;
	};

	struct cmd_pvp_combat
	{
		int idPlayer;
		char state;
	};

	struct cmd_double_exp_time
	{
		int mode;		//	非0表示双倍经验状态,0表示非双倍经验状态
		int end_time;	//	结束时间戳
	};

	struct cmd_available_double_exp_time
	{
		int available_time;	//	剩余时间,单位秒
	};

	struct cmd_duel_recv_request
	{
		int player_id;
	};

	struct cmd_duel_reject_request
	{
		int player_id;
		int reason;
	};

	struct cmd_duel_prepare
	{
		int player_id;
		int delay;              //几秒延时
	};

	struct cmd_duel_cancel
	{
		int player_id;
	};

	struct cmd_host_duel_start
	{
		int idOpponent;
	};

	struct cmd_duel_stop
	{
		int player_id;
	};

	struct cmd_duel_result
	{
		int id1;
		int id2;
		char result;    //1 id1 win 2 draw
	};

	struct cmd_player_bind_request
	{
		int who;
	};

	struct cmd_player_bind_invite
	{
		int who;
	};

	struct cmd_player_bind_request_reply
	{
		int who;
		int param;	//	只会非0 不同意
	};

	struct cmd_player_bind_invite_reply
	{
		int who;
		int param;	//	只会非0不同意
	};

	struct cmd_player_bind_start
	{
		int mule;
		int rider;
	};

	struct cmd_player_bind_stop
	{
		int who;
	};

	//  多人骑乘加入
	struct cmd_player_multi_bind_join
	{
		int  leader;		
		int  member;
		char pos;
	};
	//  多人骑乘退出
	struct cmd_player_multi_bind_leave
	{
		int  leader;
		int  member;
		char pos;
	};
	// 多人骑乘主骑踢人
	struct cmd_player_multi_bind_kick
	{
		int leader;
		char pos;
	};

	struct cmd_player_mounting
	{
		int id;
		int mount_id;
		char mount_level;
		char mount_type;    //0,普通坐骑 1，多人坐骑
	};

	struct cmd_player_equip_detail
	{       
		int id;
	};

	struct cmd_player_duel_start
	{
		int player_id;
	};

	struct cmd_pariah_time
	{
		int pariah_time;
	};

	struct cmd_gain_pet
	{
		int slot_index;		//	此宠物放于宠物栏内的位置
		info_pet data;		//	pet data
	};

	struct cmd_free_pet
	{
		int slot_index;
		int pet_id;			//	宠物的id，无大用
	};

	struct cmd_summon_pet
	{
        unsigned char pet_index;
        int pet_tid;    //宠物的模版id
        int pet_id;     //宠物的实际id
	};

	struct cmd_recall_pet
	{
        unsigned char pet_index;
        int pet_tid;    //宠物的模版id
        int pet_id;     //宠物的实际id
	};

	struct equip_pet_bedge
	{
		unsigned char inv_index;
		unsigned char pet_index;
	};

    struct equip_pet_equip
    {
        unsigned char inv_index;
        unsigned char pet_index;
    };

	struct player_set_pet_status
    {
        unsigned char pet_index;
        int pet_tid;
        unsigned char main_status;      // 0 战斗 1 采集 2 制造 3 休息
        unsigned char sub_status;       // 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
    };

	struct player_combine_pet
    {
        unsigned char pet_index;
        int pet_tid;                    //宠物的模版id
        unsigned char type;             //0 通灵 1 御宝
    };

	struct player_uncombine_pet
    {
        unsigned char pet_index;
        int pet_tid;                    //宠物的模版id
    };

	struct player_pet_age_life
    {
        unsigned char pet_index;
        int age;
        int life;
    };

	struct player_pet_set_auto_skill
    {
        unsigned char pet_index;
        int pet_id;
        int skill_id;
		unsigned char set_flag;
    };

	struct player_pet_set_skill_cooldown
    {
        unsigned char pet_index;
        int pet_id;
        int cooldown_index;
        int cooldown_time;
    };

	struct cmd_player_start_pet_op
	{
        unsigned char pet_index;
        int pet_id;
        int delay;              //延迟时间,单位是50ms的tick
        unsigned char op;       //操作类型 0:放出 1:召回 2:通灵 3:解除通灵 4:御宝 5:解除御宝
	};

    struct player_stop_pet_op
    {
        unsigned char op;       //操作类型 0:放出 1:召回 2:通灵 3:解除通灵 4:御宝 5:解除御宝
    };

	struct cmd_pet_receive_exp
	{
		unsigned char pet_index;
		int pet_id;
		int exp;
	};

	struct cmd_pet_levelup
	{
		unsigned char pet_index;
		int pet_id;
		int level;			//	新级别
		int exp;			//	当前的经验值 
	};

	struct player_pet_hp_notify
    {
        unsigned char pet_index;
        int cur_hp;
        int max_hp;
		int cur_vp;	//当前精力
        int max_vp;	//最大精力
    };

	struct player_pet_ai_state
    {
        unsigned char pet_index;
        unsigned char stay_state;
        unsigned char aggro_state;
    };

	struct player_pet_dead
    {
        unsigned char pet_index;
    };

	struct cmd_pet_room
	{
		unsigned short count;
	//	重复 count 次
	//	int index;
	//	info_pet data;
	};

	struct cmd_pet_room_capacity
	{
		unsigned char capacity;
	};

	struct cmd_pet_honor_point
	{
		unsigned char index;
		int cur_honor_point;
		int max_honor_point;
	};

	struct cmd_pet_hunger_gauge
	{
		unsigned char index;
		int cur_hunge_gauge;
		int max_hunge_point;
	};

	struct cmd_host_enter_battle
	{
		unsigned char battle_faction;   // 0 中立 1 攻方 2 守方 3 观战 4 竞技场
		unsigned char battle_type;		// 战场类型 0 普通战场 1 跨服pk 3 飞升战场 4 楚汉战场 5 飞升楚汉 6 竞技场 7 飞升竞技场 
        int battle_id;                  // 战场id
        int end_timestamp;              // 结束时间
	};

	struct cmd_tank_leader_notify
	{
		int idTank;
		int tidTank;
		int idLeader;
	};

	struct cmd_battle_result
	{
		int result;	// 1 攻方获胜 2 守方获胜 3 平手 4 战场结束
		int attacker_score;
		int defender_score;
	};

    struct cmd_battle_info
    {
        struct player_info_in_battle
        {
                int id;
                unsigned char battle_faction;	//1 攻方 2 守方
                unsigned int battle_score;
                unsigned short kill;    
                unsigned short death;
        };

        unsigned char attacker_building_left;	// 攻方剩余普通建筑
        unsigned char defender_building_left;	// 守方剩余普通建筑
        unsigned char player_count; 		// player_info_in_battle结构数量，人数
        player_info_in_battle* info;

		GP_S2C_CHECKDATA(cmd_battle_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~cmd_battle_info() { delete[] info; }
    };

	// 宋金战场个人战绩
	struct self_sjbattle_info 
	{
		int battle_score;	//积分
		int rank;   		//军衔
		int conKill; 		//连斩数
		int postion; 		//排名
	};

	// 宋金战场的战场排行榜
	struct cmd_sjbattle_info
	{
		struct player_info_in_battle
		{
			int id;
			unsigned char faction;
			int score;
			int rank;
			int con_kill;
		};

		int attacker_score;
		int defender_score;
		int attacker_count;
		int defender_count;
		int player_count;
		player_info_in_battle* info;

		GP_S2C_CHECKDATA(cmd_sjbattle_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~cmd_sjbattle_info() { delete[] info; }
	}; 

	// 竞技场战场实时信息
	struct arena_info
	{
		struct player_info_in_arena
        {
                int score;
                int kill_count;
        };

		int kill_count;
		int score;
		int apply_count; //报名人数
		int remain_count; //剩余人数
		int player_count;
		player_info_in_arena* info;

		GP_S2C_CHECKDATA(arena_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~arena_info() { delete[] info; }
	};


	struct player_recipe_data	//host player 的已经学会的配方数据 在刚进入游戏时会收到
	{
		unsigned short count;
		unsigned short buf[1]; // size == count
	};

	struct player_learn_recipe	//host player 学会了一个新配方
	{
		unsigned short recipe_id;
	};

	struct player_produce_skill_info	//host player的生产技能等级和当前经验 在刚进入游戏时会收到 在生产发生了经验变化时也会收到
	{
		int level;
		int exp;
	};

	struct player_refine_result //精炼结果
	{
		int item_index;
		int item_id;
		int result; // 0 成功 1 失败 删除材料 2失败删除材料 删除属性 3失败 装备爆掉	4拆卸 属性石 5拆卸技能石
	};

	struct player_start_general_operation //玩家开始进行通用动作 host player和else player都可能会收到
	{
		int player_id;
		int op_id;      //op_id的解释以后给我们
		int duration;   //以0.05秒为单位的tick
	};

	struct player_stop_general_operation     //玩家停止通用动作 host player 和 else player 都可能收到
	{
		int player_id;
		int op_id;
	};

	struct player_cash //告知玩家自身的游戏点数，此点数用于百宝阁购买物品
	{
		int cash_amount;
		int used_cash;
		int cash_add;
	};

	struct player_skill_addon       //某样技能额外增加的级别
	{       
		int skill;
		int level;
	};

	struct player_skill_common_addon  //所有技能额外增加的级别
	{       
		int addon;
	};

	struct player_extra_skill                //额外学会了某样技能
	{       
		int skill;
		int level;
	};

	struct player_title_list                     //player的称谓列表 在刚进入游戏时收到
	{
        size_t count;   //不是 byte size
        short title[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(title);

			if (buf_size < sz)
				return false;

			sz += count * sizeof(title);
			return buf_size >= sz;
		}
	};

	struct player_add_title                      //player新得到了一个称谓
	{
        short title;
	};

	struct player_del_title                      //被去除了一个称谓
	{
		short title;
	};

	struct player_region_reputation              //player 的区域友好度列表，在刚进入游戏时收到
	{
        int count;
        int reputation[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(reputation);

			if (buf_size < sz)
				return false;

			sz += count * sizeof(reputation);
			return buf_size >= sz;
		}
	};

	struct player_change_region_reputation       //player 的某个区域友好度更新为新的值
	{
        int index;
        int new_reputation;
	};

	struct player_change_title                 //广播消息，通知某个玩家其当前显示的称谓发生变化title为0则表示不显示称谓
	{
        int who;
        short title;
	};

	struct player_change_inventory_size
	{
		int new_size;
	};

	struct player_bind_success					//表示某物品成为绑定状态
	{
		unsigned short inv_index;				//包裹栏位置
		int item_id;							//物品ID
		int item_state;							//物品现在的绑定状态，和item的状态一致
	};

	struct player_change_spouse
	{
		int who;
		int spouse;
	};

	struct player_invader_state
	{
		int who;
		char state;
	};

	struct player_mafia_contribution
	{
        int contribution;
		int family_contrib;
	};

	struct lottery_bonus
	{
        int lottery_id;    //彩票 ID
        int bonus_level;   //奖励级别
        int bonus_item;    //奖励获得物品 可能没有(-1 或者0）
        int bonus_count;   //奖励物品数目i
        int bonus_money;   //奖励金钱数量 可能为0 
	};

	struct lottery2_bonus
	{
        int lottery_id;    //彩票 ID
		int bonux_index;   //奖励序号	
        int bonus_level;   //奖励级别
        int bonus_item;    //奖励获得物品 可能没有(-1 或者0）
        int bonus_count;   //奖励物品数目i
        int bonus_money;   //奖励金钱数量 可能为0 
	};

	struct lottery3_bonus	//Added 2011-07-28.
	{
		int lottery_id;		//彩票ID
		int bonus_index;	//奖励序号
		int bonus_level;	//奖励级别
		int	bonus_item;		//奖励获得物品 可能没有（-1 或者 0）
		int bonus_count;	//奖励物品数量
		int bonus_money;	//奖励金钱数量 可能为0
	};

	//宝石升品等返回结果信息Added 2011-03-22.
	struct gem_notify
	{
		int	gem_id;			//宝石的ID
		int msg;			//0: 宝石升品成功; 1: 宝石升品失败; 2: 宝石品质不符，无法进行熔炼
	};

	struct record_check_result  //检查录像的结果，本协议是 C2S::check_record的回应协议
	{
        char value[16];     //用于校验的16字节MD5值
	};

	struct player_use_item_with_arg          //玩家自己使用物品，带有自定义参数，只有自己收到
	{
        unsigned char where;            //哪里的物品
        unsigned char index;            //物品的位置
        int item_id;                    //物品的类型
        unsigned short use_count;       //使用的个数
		unsigned short size;
        char arg[1];                     //物品的自定参数
	};

	struct object_use_item_with_arg         //除自己外收到的自己定参数使用物品 
	{
        int user;
        int item;
		unsigned short size;
        char arg[1];
	};

	struct mafia_trashbox_capacity_notify  //通知客户端新的帮派仓库的大小改变
	{
        int cap;
	};

	struct npc_service_result              //npc服务操作结果，如果type为0，那么应结束当前的等待NPC状态
	{
        int type;
	};

	struct pvp_mode
	{
		unsigned char mode;
	};

	struct battle_flag_change         //当战场旗子改变颜色的时候会发此广播
	{
        int matter_id;            //是哪一个旗子改变
        char origin_state;        //改变前的颜色(取值范围0,1,2分别代表中立,攻方,守方)
        char new_state;           //改变后的颜色(取值范围0,1,2分别代表中立,攻方,守方)
	};

	struct battle_flag_change_status        //整个战场广播通知战场旗子的变化
	{
        int flag_id;
        char origin_state;
        char new_state;
	};

	struct talisman_exp_notify      // 法宝经验变化
	{
        unsigned short where;    //哪个包裹栏
        unsigned short index;    //包裹栏哪个位置
        int new_exp;             //变化后的经验
	};

	struct talisman_merge_notify	// 法宝熔炼
	{
		int id_player;
		int id_t1;
		int id_t2;
		int id_t_result;
	};

	struct talisman_value_notify
	{
		int value;
	};

	struct bot_begin_notify
	{
		char flag;
	};

	struct player_wallow_info  //玩家当前的沉迷信息, 刚进入游戏的时候服务器会主动发送，沉迷等级改变时也会发送，沉迷被GM开关若未引起沉迷等级变化，服务器不会主动发送
	{
        unsigned char anti_wallow_active;  //反沉迷是否激活
        unsigned char wallow_level;        //沉迷等级 0 1 2
		int msg;						   //0-身份证未成年 1-无身份证
		int play_time;
        int light_timestamp;               //中度沉迷的时间点
        int heavy_timestamp;               //重度沉迷的时间点
	};

	struct anti_cheat_bonus          //反外挂答题成功奖励类型
	{
        unsigned short bonus_type;     //取值范围 0 1 2 3，分别代表 血蓝奖励，经验奖励，炼器符奖励，双倍经验奖励
	};

    struct battle_score
    {
		size_t score;
	};

    struct gain_battle_score
    {
        size_t inc;
    };

	struct spend_battle_score
	{
        size_t dec;
	};

    struct battle_player_info
    {
        int id;
        unsigned char battle_faction;	//1 攻方 2 守方
        unsigned int battle_score;      
        unsigned short kill;
        unsigned short death;
    };

	struct talent_notify
	{
         int value;
	};

	struct notify_safe_lock         //安全锁是否打开
	{
        unsigned char active;
		int time;
		int total_time;
	};

	struct war_start
	{
		int time;
	};

	struct war_end
	{
		unsigned char result; // 1:攻方获胜 2:守方获胜
	};

	struct war_construction_info
    {
        struct node_t
        {
			int type;
            int key;

            struct attr_t
            {
                unsigned char attack_level;
                unsigned char defence_level;
                unsigned char range_level;
                unsigned char ai_level;
                int curHp_level;
            };

            attr_t attr;
        };

		int res_a;
        int res_b;
        unsigned char count;
        node_t* nodes;

		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~war_construction_info() { delete[] nodes; }
	};

	struct war_contribution_info
    {
        struct node_t
        {
            int player_id;
            int res_a;
            int res_b;
        };

        unsigned char page;			// 当前页号,0开头
		unsigned char max_page;     // 最大页号,0开头
        unsigned char count;		// 页数
        node_t* nodes;

		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~war_contribution_info() { delete[] nodes; }
     };

    struct enter_battlefield
    {
        int faction_id_attacker;
        int faction_id_defender;
        int faction_id_attacker_assistant;
        int battle_id;                  // 战场id
        int end_timestamp;              // 结束时间
    };

    struct war_info
    {
        unsigned char count;
        struct node_t
        {
			unsigned char key;
			unsigned char status; //0 death
			A3DVECTOR3 pos;
			float hp;
        };

		node_t* nodes;
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~war_info() { delete[] nodes; }
    };

	struct war_building_status_change
    {
        unsigned char key;
        unsigned char status; //0 death
        A3DVECTOR3 pos;
		float hp;
    };

    struct player_set_pet_rank
    {
        unsigned char pet_index;
        int pet_tid;
        unsigned char rank;
    };

	struct player_reborn_info
	{
		unsigned short count;   //转了几次，不是字节数

		struct info_t
		{
			short level;    //本次转生时的等级
			short prof;     //本次转生时的职业
		};

		info_t* info;
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~player_reborn_info() { delete[] info; }
	};

	struct script_message
    {
        int player_id;
        int pet_id;
        unsigned short channel_id;
        unsigned short msg_id;
    };

	struct player_pet_civilization
    {
        int civilization;       //宠物文明度
    };

	struct player_pet_construction
    {
        int construction;       //宠物工坊建设度
    };

	struct player_move_pet_bedge
    {
        unsigned char src_index;
        unsigned char dst_index;
    };

	struct player_start_fly
	{
		int who;            //谁飞
		char active;        //是开始飞还是停止飞
		char type;			// 0:飞剑    1：技能
	};

	struct player_pet_prop_added
    {
        unsigned char pet_index;
        unsigned short maxhp;
        unsigned short maxmp;
        unsigned short defence;
        unsigned short attack;
        unsigned short resistance[6];
        unsigned short hit;
        unsigned short jouk;
        float crit_rate;
        float crit_damage;
		int equip_mask;
    };

	struct sect_become_disciple
	{
        int who;
        int master_id;
	};

	struct cultivation_notify
	{
        int who;
        char new_cult;
	};

	struct offline_agent_bonus
	{
        int bonus_time;         //min
        int left_time;          //min
        double bonus_exp;
	};

	struct offline_agent_time        //离线托管剩余可用时间 
	{
        int left_time;          //min
	};

	struct cmd_spirit_power_decrease     //魂力因攻击/被攻击而减少
	{
        short index;                 //装备栏上的哪件装备
        short dec_amount;            //减少的点数
        int result;                  //减少后的结果是多少
	};

	struct self_buff_notify                 //SELF_BUFF_NOTIFY
	{ 
		int id;
		unsigned short count;

		struct _buff_t
		{
			unsigned short state;
			unsigned short level;
			int buff_endtime;
			char overlay_cnt;

		} *buff_list;

		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
//		~self_buff_notify(){ delete[] buff_list; }
	};
	
	struct notify_fashion_hotkey 
	{
		int count;
		
		struct fashion_hotkey
		{
			int index;
			int id_head;
			int id_cloth;
			int id_shoe;
		} *hotkey_list;

		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
//		~notify_fashion_hotkey(){ delete[] hotkey_list;}
	};

	// 新增时装包裹FASHION_INVENTORY
	// 时装包裹内部交换
	struct cmd_exg_fashion_item
    {
        unsigned char index1;
        unsigned char index2;
    };
	// 时装包裹与背包的物品交换
	struct cmd_exg_ivtr_fashion
	{
		unsigned char idx_ivr;
		unsigned char idx_fas;
	};
	// 时装包裹与装备栏的物品交换
	struct cmd_exg_equ_fashion
	{
		unsigned char idx_equ;
		unsigned char idx_fas;
	};
	// 使用快捷键来更换时装
	struct hotkey_change_fashion
	{
		unsigned char key_index;
		unsigned char idx1;		// 头饰
		unsigned char idx2;		// 衣服
		unsigned char idx3;		// 鞋子
	};

	struct peep_info
	{
		int id;
	};
	
	struct player_bonus
	{
		int bonus_amount;
		int bonus_used;
	};

	struct military_rank_change
	{
		int tid;					// player id
		unsigned char rank;			// new rank
	};
	
	struct sjbattle_score_change
	{
		int old_score;
		int new_score;
	};
	
	struct player_change_face
	{
		int tid;
		unsigned char faceid;
		unsigned char hairid;
		unsigned char earid;
		unsigned char tailid;
		unsigned char fashionid;
	};

	struct player_change_vipstate
	{
		int tid;					// player id
		unsigned char vip_state;	// 新的vip状态
	};

	struct cmd_receive_bonus_exp
	{
		double exp;
	};

	// 跨服pk实时信息通告
	struct crossserver_battle_score
	{
		int attacker_kill_num;		// 我方击杀总数
		int defender_kill_num;		// 敌方击杀总数
		int self_kill_num;			// 个人击杀数
		int self_death_num;			// 个人死亡数
		int attacker_off_line_score;	// 离线我方分数
		int defender_off_line_score;	// 离线敌方分数
		unsigned char attacker_count;	// 我方玩家总数
		unsigned char defender_count;	// 敌方玩家总数
		unsigned char min_player_deduct_score; // 离线惩罚人数限制
	};

	// 跨服小队pk实时信息结构 Added 2011-05-19.
	struct crossserver_team_battle_info
	{
		struct history_score
		{	
			int attacker_score;		//攻方得分
			int defender_score;		//守方得分
		};
		
		unsigned char	battle_state;			//战场当前状态，0表示休息， 1表示战斗
		int				cur_state_end_time;		//当前状态的结束时间，该值为服务器的绝对时间
		int				attacker_score;			//当前红方分数
		int				defender_score;			//当前蓝方分数
		int				best_killer_id;

		unsigned char	history_round_count;	//已经完成的pk战斗局数
		history_score	score_info[1];			//已经完成的pk战斗得分数据信息
	};

	// Added 2011-06-16.
	// 八凶(八荒)玄火阵，通知队员结阵的广播消息 
	struct cmd_circleofdoom_startup
	{
		int		sponsor_id;		//阵法发起者id
		char	faction_type;	//1表示按组队收集，2表示按帮派收集，3表示按家族收集
	};

	// 八凶(八荒)玄火阵，结阵成员id与阵眼成员id信息
	struct cmd_circleofdoom_info
	{
		int skill_id;		//阵法技能id -->特殊设计，用于处理在玩家上线后看到阵法特效-->从技能中检索阵法名称-->播放与该阵法相关的gfx. Added 2011-09-02.
		int	sponsor_id;		//阵法发起者id
		int	member_count;	//阵法成员个数，不包含阵法发起者
		int	member_id[1];	//阵法成员id
	};

	// 八凶(八荒)玄火阵，通知客户端技能效果结束
	struct cmd_circleofdoom_stop
	{
		int sponsor_id;		//阵法发起者的id
	};

	// Added end.

	// Added 2011-06-28.
	//太昊归神/脱灵技能中的缩放角色大小的消息
	struct cmd_player_scale
	{
		int  player_id;		//需要进行/停止缩放操作的角色id
		char switch_tag;	//0, 进行缩放；1，关闭缩放
		int  scale_ratio;	//缩放比例，百分数
	};
	// Added end.

	//Added 2011-07-08.
	//焚香-火焰囚牢技能中单个囚牢的相关信息
	struct cmd_player_jail_circle_area
	{
		char		mode;	//消息的操作模式，0：添加； 1：删除， 2：全部清除
		char		type;	//该囚牢相对于该角色来说的类型，0：能进入囚牢，但不能出去；1：能出去，但不能进入
		int			id;		//标示该囚牢的id
		float		radius;	//囚牢的半径
		A3DVECTOR3	center;	//囚牢的中心点
	};
	//Added end.

	//Added 2011-07-26.
	//gs 通知客户端玩家进入副本
	struct cmd_enter_raid
	{
		unsigned char	raid_faction;	//所在副本帮派：0：中立； 1：攻方；2：守方；3：旁观
		unsigned char	raid_type;		//副本类型，暂时只有一种
		int				raid_id;		//副本实例ID？？！
		int				end_timestamp;	//结束时间
	};

	//gs 通知客户端玩家进入副本地图的位置
	struct cmd_notify_raid_pos
	{
		A3DVECTOR3		pos;			//进入副本世界的位置
		int				raid_map_id;	//副本地图号，类似于之前NOTIFY_HOSTPOS的tag字段->Instance ID
		int				tag;			//副本实例ID ->tag.
	};
	//Added end.

	//Added 2011-07-27.
	//gs 通知客户端玩家当天进入各副本的次数
	struct cmd_enter_raid_count
	{
		int				raid_list_size;	//当天进入所有副本的类型
		struct
		{
			int		raid_map_id; //副本地图号
			int		enter_count; //当日进入该副本次数
		}enter_count_list[1];	 //列表
	};
	//Added end.

	//Added 2011-07-28.
	//gs 通知客户端拉扯技能的释放者当前拉扯的状态：开始拉扯/终止拉扯
	struct cmd_player_pull_state
	{
		bool	is_pull;		//true:开始拉扯；false:终止拉扯
	};

	//gs 通知客户端所有可视范围内玩家当前某拉扯技能效果状态的相关信息
	struct cmd_player_be_pulled_info
	{
		int				player_cast_id;		//释放拉扯/牵引技能的玩家ID
		int				player_target_id;	//被拉扯/被牵引的目标玩家的ID
		bool			is_pull;			//true:开始拉扯/牵引；false:结束拉扯/牵引
		unsigned char	type;				//当前技能类型0：拉扯，1：牵引
	};
	//Added end.

	//Added 2011-08-03.
	//gs 通知客户端进入副本的玩家相关信息
	struct raid_player_info
	{
		struct player_info_in_raid
		{
			int		player_id;		//玩家ID
			short	profession;		//职业ID
			short	player_level;	//玩家等级
			short	sec_level;		//飞升等级
		};

		unsigned char player_count;	//玩家数
		player_info_in_raid player_info[1];	//进入副本玩家信息列表
	};

	//gs 通知客户端触发技能时间信息
	struct trigger_skill_time
	{
		short	skill_id;		//技能ID
		short	time;			//触发时间，单位：秒
	};
	//Added end.

	//Added 2011-08-30.
	//gs通知客户端停止某玩家的当前技能
	struct player_stop_skill
	{
		int player_id;			//ElsePlayer 玩家ID
		char spirit_index;	//0-2,灵的index
	};
	//Added end.

	//Added 2011-12-02.
	//gs通知客户端玩家进入某副本的次数
	struct player_enter_raid_count
	{
		int		map_id;			//相应副本地图ID
		short	enter_count;	//进入副本次数
	};
	//Added end.

	//Added 2012-01-04.
	//gs通知客户端当前有哪些可以领取的VIP奖励相关信息
	struct player_vip_award_info
	{
		struct AWARD_DATA
		{
			int award_id;		//奖励id
			char is_obtained;	//本奖励是否已经领取：0表示未领取，1表示已领取
		};
		int		award_expire_time;		//vip特殊奖励领取的截止时间(每日奖励每日都清除了)
		int		recent_total_cash;		//近期（30天）的累计充值数
		int		award_count;			//奖励数量
		AWARD_DATA		award_id_list[1];		//当前可发的奖励id列表

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(award_id_list);

			if (buf_size < sz)
				return false;

			sz += award_count*sizeof(AWARD_DATA);
			return buf_size >= sz;
		}
	};
	//Added end.

	//Added 2012-01-16.
	//gs通知客户端某玩家的vip奖励等级更新信息
	struct player_vip_award_update
	{
		int				role_id;		//角色id
		unsigned char	vip_award_level;//vip奖励等级
		char			is_hide;
	};
	//Added end.

	//Added 2012-02-16.
	//gs通知客户端当前玩家身上有哪些在线倒计时奖励
	struct online_award_info
	{
		struct ONLINE_AWARD_INFO
		{
			int			award_id;		//在线倒计时奖励的大礼包id
			int			index;			//大礼包中的发放到了哪个小礼包的位置index
			int			small_gift_id;	//大礼包中的发放到了哪个小礼包的模板id
			int			remain_time;	//剩余多长时间，该小礼包就能够发放
		};

		char				force_update_flag;//强制客户端更新奖励信息标志，非0表示强制更新 Added and Modified 2012-04-01.
		int					award_count;	//在线奖励的大礼包数目
		ONLINE_AWARD_INFO	award_info_list[1];//在线奖励礼包列表

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(award_info_list);

			if (buf_size < sz)
			{
				return false;
			}

			sz += award_count*sizeof(ONLINE_AWARD_INFO);
			return buf_size >= sz;
		}
	};

	//gs通知客户端某在线奖励能否可以领取
	struct online_award_permit
	{
		int				award_id;		//在线奖励的模板id
		int				index;			//大礼包中需要发放的小礼包的位置index
		char			permit_code;	//0：表示可以领取奖励，1：该小礼包已经成功发放，可以进行下一轮倒计时，//-1 该礼包没激活, -2 该礼包已经领取, -3 没有找到该礼包
										//-4 该礼包对应的index没有小礼包信息, -5 无效的player，不能领取，//-6 无效的index（index表示倒计时到那一轮）//-7 本轮的倒计时还没有结束
	};
	//Added end.

	//Added 2012-02-20.
	//永久时装技能，单独走新协议
	struct addon_permanent_skill
	{
		int				skill_id;		//技能ID
		int				skill_level;	//技能等级
	};

	//永久时装技能的相关数据
	struct addon_permanent_skill_data
	{
		size_t			skill_count;	//技能数量
		struct SKILL
		{
			short			id_skill;	//技能ID
			unsigned char	level;		//技能等级
		}skill_list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(skill_list);
			if (buf_size < sz)
			{
				return false;
			}

			sz += skill_count*sizeof(SKILL);
			return buf_size >= sz;
		}
	};
	//Added end.

	//------------------------------------------------
	//Added 2012-03-21.
	//诛仙古卷挖宝区域信息
	struct treasure_info
	{
		int		remain_dig_count;		//剩余可探索次数
		int		region_count;			//区域数量
		struct Treasure_Info
		{
			char	type;				//0-无效，1-普通，2-隐藏
			char	status;				//0-无效，1-可探索，2-关闭(普通区域)，3-锁定(隐藏区域)
			char	level;				//区域等级
			int		id;					//区域模板ID
		}treasure_region[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(treasure_region);
			if (buf_size < sz)
			{
				return false;
			}

			sz += region_count * sizeof(Treasure_Info);
			return buf_size >= sz;
		}
	};

	//挖宝区域升级结果信息
	struct treasure_region_upgrade_result
	{
		int		region_id;				//挖宝区域ID
		int		region_pos;				//挖宝区域位置
		bool	success;				//挖宝区域升级结果，成功与否
		int		new_level;				//挖宝区域升级后的等级
	};

	//挖宝区域解锁结果信息
	struct treasure_region_unlock_result
	{
		int		region_id;				//挖宝区域ID
		int		region_pos;				//挖宝区域位置
		bool	success;				//解锁是否成功
	};

	//挖宝结果信息
	struct treasure_region_dig_result
	{
		int		region_id;				//挖宝区域ID
		int		region_pos;				//挖宝区域位置
		bool	success;				//挖宝是否成功
		char	event_id;				//奖励事件ID

		int		award_item_num;			//奖励物品数量
		struct AWARD_ITEM
		{
			int	item_id;				//奖励物品ID
			int item_count;				//该奖励物品的数量
		}award_item_list[1];			//奖励物品信息列表

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(award_item_list);
			if (buf_size < sz)
			{
				return false;
			}
			
			sz += award_item_num * sizeof( AWARD_ITEM );
			return buf_size >= sz;
		}
	};
	//------------------------------------------------

	//------------------------------------------------
	//Added 2012-04-13.
	//副本全局变量
	struct raid_global_variable
	{
		int		count;					//副本全局变量的个数

		struct VARIABLE_ITEM
		{
			int		key;
			int		value;
		}var_list[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(var_list);
			if (buf_size < sz)
			{
				return false;
			}

			sz += count * sizeof(VARIABLE_ITEM);
			return buf_size >= sz;
		}
	};

	struct inc_treasure_dig_count
	{
		int inc_count;
		int total_dig_count;
	};	
	//------------------------------------------------

	//------------------------------------------------
	//Added 2012-04-17.
	//趴塔副本相关协议
	//通知客户端随机怪物产生结果
	struct random_tower_monster
	{
		int			level;		//当前打哪一层
		char		reenter;	//0，否，1，是
	};

	struct random_tower_monster_result
	{
		int			monster_id;		//-1表示空
		char		click_index;
	};

	//通知客户端玩家趴塔副本信息
	struct player_tower_data
	{
		int			tower_level;		//当前趴到的最大塔层
		int			start_level_today;	//当日玩家选择的开始层
		int			end_level_today;	//当日玩家达到的最高层数
		int			best_pass_time[10];	//每10层的时间，没有时间则为0
		int			award_count;		//完成趴塔的奖励列表数目
		struct TOWER_AWARD_STATE
		{
			char	daily_award_delivered;		//该层的每日奖励是否领取 0：没有领取， 1：已经领取
			char	lifelong_award_delivered;	//该层得终生奖励是否领取 0：没有领取， 1：已经领取
		}tower_award_state[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(tower_award_state);
			if ( buf_size < sz )
			{
				return false;
			}

			sz += award_count * sizeof(TOWER_AWARD_STATE);
			return buf_size >= sz;
		}
	};

	//玩家趴塔副本怪物图鉴列表
	struct player_tower_monster_list
	{
		int		tower_monster_count;	//怪物图鉴数量
		int		tower_monster_list[1];	//怪物图鉴列表

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(tower_monster_list);
			if ( buf_size < sz )
			{
				return false;
			}

			sz += tower_monster_count * sizeof(int);
			return buf_size >= sz;
		}
	};

	//趴塔副本技能手牌
	struct player_tower_give_skills
	{
		int		skill_count;		//技能个数
		struct SKILL
		{
			int		skill_id;		//技能ID
			int		skill_level;	//技能等级
			bool	used;			//是否已经使用
		}skills[1];					//技能列表

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(skills);
			if ( buf_size < sz )
			{
				return false;
			}

			sz += skill_count * sizeof(SKILL);
			return buf_size >= sz;
		}
	};

	//单个手牌使用后通知客户端改变状态
	struct set_tower_give_skill
	{
		int		skill_id;
		int		skill_level;
		bool	used;
	};

	//获得趴塔副本一个怪物图鉴
	struct add_tower_monster
	{
		int		monster_id;
	};

	//趴塔副本奖励领取成功后发送给客户端状态协议
	struct set_tower_award_state
	{
		char	type;		//0: 每日， 1：终身
		int		level;		//塔层
		bool	delivered;	//奖励发送状态
	};

	struct littlepet_info 
	{
		char level;
		char cur_award_lvl;
		int cur_exp;
	};

	struct rune_info
	{
		bool active;
		int  rune_score;
		int  customize_info;
	};

	struct rune_refine_result
	{
		char type;	// 0 -洗练 1-接收洗练结果
	};

	// 爬塔副本通过一关
	struct tower_level_pass
	{
		int level;		// 通过关卡的挡闶
		int best_time;	// 最好时间
	};

	// 平台发过来的掩码，账号状态
	struct fill_platform_mask
	{
		// 0x01: 需要补填身份证+姓名
		// 0x10: 身份证+姓名补填奖励
		// 0x02: 需要补填账号+密码+邮箱（邮箱是修改密码的凭证）
		// 0x20: 账号+密码+邮箱补填奖励
		int mask;
	};

	struct pk_bet_data 
	{
		struct pk_bet
		{
			int bet_1st_num;
			bool top3;
		} pk_bets[8];
		bool top3_bet;
		bool first_bet_reward_deliveried;
		bool first_bet_reward_result_deliveried;
		bool top3_bet_reward_deliveried;
		bool top3_bet_reward_result_deliveried[3];
	};

	struct put_item
	{
		int type;
		int expire_date;
		unsigned char index;			// 放在哪个位置
		unsigned short amount;
		unsigned short slot_amount;
		unsigned char where;			// 在哪个包裹栏，0 标准 2 任务
		int state;
	};

	struct player_change_name
	{
		int id;
		int name_len;
		char name[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(char);
			if (buf_size<sz)
			{
				return false;
			}
			sz += name_len * sizeof(char);
			return buf_size >= sz;
		}
	};
	
    struct repurchase_inv_data
	{
		unsigned char ivtr_size;
		size_t content_length;
		void* content;
		
		GP_S2C_CHECKDATA(repurchase_inv_data)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
    };

	struct exchange_mount_wing_item
	{
		unsigned char index1;
		unsigned char index2;
	};
	
	struct exchange_Inventory_mountwing_item
	{
		unsigned char idx_inv;
		unsigned char idx_mv;
	};
	
	struct exchange_equipment_mountwing_item
	{
		unsigned char idx_equ;
		unsigned char idx_mv;
	};

	struct player_change_mountwing_inv_size
	{
		int new_size;
	};

	//------------------------------------------------

	struct zx_five_anni_data
	{
		char star;		// 当前多少星
		char state;		// 0:初始	1:点击恭喜按钮	2:点击领奖按钮
	};

	struct achievement
	{
		unsigned short achieve_id;	// 成就id 

		unsigned short premiss_mask;//标识每一个条件是否完成
		unsigned char premiss_count;// 条件数量
		struct _premiss			// 条件的数据结构
		{
			unsigned char premiss_id;	// 条件id;
			char data[1];		// 条件数据
		} premiss[1];
	};

    struct achievement_finish_time
    {
            unsigned short id;              
            int finish_time;
    };
	// 全部成就数据
	struct player_achievement
	{
		size_t 	map_count;			//成就map大小
		char 	map_data[1];		//成就map数据

		unsigned short  active_count;	//激活中的成就数量
		achievement achieve[1];			//激活中的成就数据
		int		achieve_point;			//总的成就点
		int		cur_achieve_point;		//当前的成就点

		size_t spec_count;                     
        achievement_finish_time info[1];

		size_t award_map_count;				//获得奖励map大小
		unsigned char award_map_data[1];	//获得奖励map数据
	};

	// 完成成就  
	struct player_achievement_finish
	{
		unsigned short achieve_id;	
		int			achieve_point;
		int			finish_time;	// 成就完成时间
	};
      
	// 激活成就(前提成就都完成了)
	struct player_achievement_active
	{
		achievement achieve;
	};
   
	//成就条件变化
	struct player_premiss_data
	{
		unsigned short achieve_id;	
		unsigned char premiss_id;
		char data[1];
	};

	// 完成成就条件
	struct player_premiss_finish
	{
		unsigned short achieve_id;	
		unsigned char premiss_id;
	};

	struct player_achievement_map
    {
        int     target;
		int		achieve_point;
        size_t  map_count;
        char    map_data[1];
    };

	struct achievement_message
    {
        unsigned short achieve_id;
        int param;
        unsigned char name_len;
        char name[1];
		int finish_time;
    };
	
	struct self_instancing_battle_info
	{
        int level_score;
        int monster_score;
        int time_score;
        int death_penalty_score;
	};

	struct instancing_battle_info 
	{
        size_t level_count;
		int	 cur_level;
        char level_status[1];
		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(level_status);

			if (buf_size < sz)
				return false;

			sz += level_count * sizeof(level_status);
			return buf_size >= sz;
		}
	};
	
	struct skill_continue 
	{
		int caster;
		int skill_id;
		char spirit_index;	//0-2,灵的index
	};

	//变身
    struct player_start_transfigure
    {
        int who;
        int template_id;
        char active;
        char type;
        char level;
        char expLevel;
    };
	
	// 幻化石减少灵气值
    struct magic_duration_decrease
    {
        short index;
        short dec_amount;
        int result;
    };
	// 通知熟练度
    struct magic_exp_notify
    {
        unsigned short where;
        unsigned short index;
        int new_exp;
    };

	// 载体相关协议
	struct cmd_player_enter_carrier  
	{
		int player_id;
		int carrier_id; 		// 想要乘坐的交通工具id
		A3DVECTOR3 rpos;		// 相对坐标和方向
		unsigned char rdir;
		unsigned char success;	//	0 失败； 1 成功
	};

	struct cmd_player_leave_carrier 
	{
		int player_id;
		int carrier_id; 		// 想要离开的交通工具id
		A3DVECTOR3 pos;			// 绝对坐标和方向
		unsigned char dir;
		unsigned char success;	// 0 失败； 1 成功
	};
	
	struct cmd_player_move_on_carrier	
	{
		int player_id;
		A3DVECTOR3 vRelDest;
		unsigned short use_time;
		short sSpeed;				//	Move speed 8.8 fix-point
		unsigned char move_mode;		
	};

	struct cmd_player_stop_on_carrier
	{
		int player_id;
		A3DVECTOR3 vRelDest;		//	在载体坐标系中的位置
		short sSpeed;				//	Move speed 8.8 fix-point
		unsigned char rdir;			//	在载体坐标系中的方向
		unsigned char move_mode;
	};

	struct cmd_player_start_interact
	{
		int player_id;
		int obj_id;
		int pos;
	};

	struct cmd_player_in_interact
	{
		int player_id;
		int obj_id;
		int pos;
	};

	struct cmd_player_end_interact
	{
		int player_id;
		int obj_id;
		int pos;
	};

	struct cmd_player_finish_interact
	{
		int			player_id;
		int			obj_id;
		int			pos;
		A3DVECTOR3	player_pos;
		A3DVECTOR3	obj_pos;
	};

	struct cmd_player_cancel_interact
	{
		int			player_id;
		int			obj_id;
		int			pos;
		A3DVECTOR3	player_pos;
		A3DVECTOR3	obj_pos;
	};

    struct cmd_skill_proficiency
    {
        int skill_id;
        int proficiency;
    };

	struct notify_mall_strategy 
	{
		int mall_strategy;
	};

	struct local_pk_message 
	{
        int killer;
        int deader;
	};

	struct update_combo_state               
    {
        char is_start;
        char color[5];  
    };

	struct enter_dim_state 
	{
		int id;
		unsigned char state;	// 0 非朦胧状态，1 朦胧状态
	};
	
	struct enter_invisible_state 
	{
		int id;
		unsigned char state;	// 0 非隐身状态，1 隐身状态
	};

	struct player_charge		// 玩家冲锋技能的释放	Modified 2011-07-22.
	{
		int id;
		char type;				// 0: 向目标瞬移； 1：向目标冲锋 Added 2011-07-22. /* 2:闪现，新加 Added 2012-09-13 */ 3：弹飞 4：吸引
		int target_id;
		A3DVECTOR3 target_pos;
	};

	struct enter_territory_battle 
	{
		unsigned char faction;		// 0: 中立，1攻方，2守方
		unsigned char battle_type;	// 战场级别
		int battle_id;				
		int end_timeStamp;			// 结束时间
	};

	struct last_logout_time 
	{
		int time;
	};

	struct clone_player_equip 
	{
		int id;
		info_player_1 info;
		unsigned short crc;
		unsigned int mask;
		int* data;

		GP_S2C_CHECKDATA(clone_player_equip)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
	};

	struct circle_info_notify
	{
		int idPlayer;				//	player id
		int idCircle;				//  circle id
		unsigned char cTitle;	//	玩家在帮派内的等级
	};

	struct receive_deliveryd_exp 
	{
		__int64 exp;
		unsigned int award_type;
		
	};

	struct deity_levelup 
	{
		int	idPlayer;
		short iLevel;			// 升到的级别
		unsigned char success;	// 0 失败 1 成功； 元神升级是否成功
	};

	struct  object_special_state
	{
		int id;					// object id
		int state;
		unsigned char on;		// 0 停止 1 开始
	};

	struct player_remedy_metempsychosis_level
	{
		//single_data_header header;
	};

	struct collision_player_pos 
	{
		int id;
		A3DVECTOR3 pos;
	};

    // by zhuyan 2012-11-07
	struct liveness_notify
	{       
		int cur_point;       // 当前的分数      
		char is_taken[4];    // 今天是否领取过了    
		int length;          // 完成的id数目
		int achieved_ids[1];  // 完成的ids
	}; 
    
	struct enter_kingdom
	{
	    unsigned char battle_faction; //0 中立 1 攻防 2 守方 3 旁观
		char kingdom_type; // 1-主战场 2-辅战场
		char cur_phase;
		int  end_timestamp; //结束时间
		int  defender_mafia_id;
		int  attacker_mafia_count;
		int  attacker_mafia_ids[1];
	};

	struct leave_kingdom
	{
	    char kingdom_type; // 1-主战场 2-辅战场
	};

	struct kingdom_half_end
	{
	    int win_faction;  //胜利方阵营
		int end_timestamp; //下一场结束的时间（如果守方获胜则直接结束）
	};

	struct kingdom_end_result 
	{
		int win_mafia_id;  //胜利方的帮派ID
	};
    struct kingdom_key_npc_info 
    {
		int cur_hp;
		int max_hp;
		int hp_add;
		int hp_dec;
    };
	struct kingdom_mafia_info
	{
		struct player_info_in_kingdom
		{
			int role_id;
			int mafia_id;
			int kill_count;
			int death_count;
		};
	    int player_count;
	    player_info_in_kingdom info[1];
	};

	struct player_bath_invite
	{
	    int who;
	};
	struct player_bath_invite_reply
	{
	     int who;
		 int param;
	};
	struct player_bath_start
	{
	     int invitee;     //被邀请的（被搓的）
		 int invitor;     //邀请者（搓澡的人）
	};
	struct player_bath_stop
	{
	     int who;
	};
	struct enter_bath_world
	{
	     int remain_bath_count;
	};
	struct bath_gain_item
	{
		 int roleid;
	     int item_id;
		 int item_count;
	};
	struct kingdom_gather_mine_start
	{
	     int roleid;
		 int mafia_id;
	};
	struct multi_exp_mode
	{
	     int multi_exp_factor; //多倍经验倍数
		 int timestamp; // 到期时间
	};
	struct ancient_raid_level_start
	{
		char level; // -1表示总挑战，0-9表示其他关卡
	};

	struct ancient_raid_level_end
	{
		char level; // -1表示总挑战，0-9表示其他关卡
		bool result;
	};

	struct ancient_raid_level_result
	{
		bool final;
		bool level_result[10];
		bool final_award_deliveried;
		bool level_award_deliveried[10];
	};

	struct ancient_raid_level_info
	{
		char level;
		short matter_cnt;
		int start_time;
	};

#pragma warning( push )
#pragma warning( disable : 4200 )
   struct sale_promotion_info // 已完成的台历道具前提条件
   {
       int task_count;
       int data[0]; // task ids
   };
#pragma warning( pop )
   
 	struct event_start
	{
		int event_id;
	};

	struct event_stop
	{
		int event_id;
	};

	struct event_info 
	{
		int event_count;
		int event[1];
	};  
	
	struct cash_gift_ids
	{
		int gift_id1;
		int gift_id2;
	};

	struct notify_cash_gift
	{
		int gift_id;
		int gift_index;
		int gift_score;
	};

	struct player_propadd
	{
		int add_prop[30];
	};

	struct player_get_propadd_item
	{
		int item_id;
		int item_index;
	};

	struct player_extra_equip_effect
	{
		int player_id;
		char weapon_effect_level;
	};

	struct fuwen_compose_info
	{
		int compose_count; // 当天合成次数
		int extra_compose_count; // 当天额外合成次数
	};

	struct fuwen_compose_end
	{
		// 无属性
	};

	struct fuwen_install_result
	{
		int src_index;
		int dst_index;
	};

	struct fuwen_uninstall_result
	{
		int fuwen_index;
		int inv_index;
	};

	struct cs_flow_player_info
	{
		int player_id;
		int score;
		int c_kill_cnt;
		bool m_kill_change;
		int m_kill_cnt;
		short kill_cnt;
		short death_cnt;
		short max_ckill_cnt;
		short max_mkill_cnt;
	};
	struct flow_battle_info
	{
		struct player_info_in_flow_battle
		{
			int id;
			short level;
			int cls;
			unsigned char battle_faction;
			unsigned short kill;
			unsigned short death;
			unsigned int zone_id;
			unsigned int flow_score;
		};
		unsigned char player_count;
		player_info_in_flow_battle* info;

		
		GP_S2C_CHECKDATA(flow_battle_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~flow_battle_info() { delete[] info; }
	};
	
	struct exchange_item_list_info
	{
		char type;
		size_t content_length;
		unsigned char content[1];
	};

	//WEB商城对应的商品信息
	struct web_order_goods
	{
		int goods_id;
		int goods_count;
		int goods_flag;
		int goods_time;
	};
	
	//WEB商城对应的一个礼包
	struct web_order_package
	{
		int package_id;
		size_t name_len;
		char package_name[1];
		int count;
		int price;
		int goods_count;
		web_order_goods * order_goods;
	};

	//WEB商城对应的一次订单
	struct web_order_info
	{
		__int64 order_id;
		int pay_type;  // 1:coupon 2:cash
		int status;    // 0:未领取 1:已领取
		int timestamp;
		int package_count;
		web_order_package * order_package;
	};

	struct web_order_list
	{
		int web_order_count;
		web_order_info * order_info;
	};

	struct battle_off_line_info
	{
		struct battle_off_line_list
		{
			unsigned char battle_faction;
			int		ntime;
			unsigned char off_line;
			unsigned char off_line_score;
		};
		unsigned char off_line_count;
		battle_off_line_list* info;

		
		GP_S2C_CHECKDATA(battle_off_line_info)
		bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~battle_off_line_info() { delete[] info; }
	};

	//可控陷阱信息
	struct control_trap_info
	{
		int id;
		int tid;
		int time;  
	};

	struct npc_invisible
	{
		int id;
		bool on;
	};

	struct puppet_form
	{
		bool on;
		int idplayer;
		int idpuppet;
	};

	struct teleport_skill_info
	{
		int idPlayer;
		int npc_tid;
		int idskill;
		int teleportnum;
	};

	struct fashion_colorant_produce
	{       
		int nIndex;
		int nColorItemID;
		int nQuality;
	};      

	struct fashion_colorant_use
	{       
		int nPackType;
		int nIndex;
		int nColorItemID;
		int nQuality;
	};

	struct player_obtain_achieve_award
	{
		unsigned short achieve_id;
	};

	struct phase_info
	{
		int phase_count;

		struct phase
		{
			int phase_id;
			bool on;
		};
		phase phase_list[1];
	};


	struct phase_info_change
	{
		int phase_id;
		bool on;
	};

	struct blow_off
	{
		int roleid;
	};

	struct player_qilin_invite
	{
		int who;
	};

	struct player_qilin_invite_reply
	{
		int who;
		int param;
	};

	struct player_qilin_start
	{
		int invitor;
		int invitee;
	};

	struct player_qilin_stop
	{
		int who;
	};

	struct player_qilin_disconnet
	{
		int who;
	};

	struct player_qilin_reconnect
	{
		int who;
		A3DVECTOR3 newpos;
	};
	
	struct fac_building_add
	{
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
	};
	struct fac_building_upgrade
	{
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
		int new_level;
	};
	struct fac_building_remove
	{
		int field_index; // 地块索引, 从1开始编号
	};
	struct fac_building_complete
	{
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
		int new_level;
	};

	struct dungeon_progress
	{
		unsigned short step_cnt;
		
		struct step_type
		{       
			unsigned char is_pass;
			unsigned char finish_challenge;
			unsigned char cur_count;
			unsigned char max_count;
		};
		step_type steps[1];

		bool CheckValid(size_t buf_size, size_t& sz) const
		{
			sz = sizeof(*this) - sizeof(step_type);
			if (buf_size<sz)
			{
				return false;
			}
			sz += step_cnt * sizeof(step_type);
			return buf_size >= sz;
		}
	};
	struct fac_base_prop
	{
		int grass; //草
		int mine;	// 矿
		int monster_food; // 兽粮食
		int monster_core;	// 妖核
		int money;				// 钱
		int task_id;			// 任务ID // 检出所需任务ID
		int task_count;		// 任务总数
		int task_need;		// 任务需要数量
	};
	struct fac_base_prop_change
	{
		fac_base_prop pValue;
		int noti_foleid;
	};
	
	struct player_fac_base_info
	{
		fac_base_prop  pValue;

		struct fac_field
		{
			int  index;
			int  tid;
			int  level;
			int  status;
		};
		int msg_size;			// 聊天长度
		char* msg;
		unsigned char field_count;
		fac_field* info;
		GP_S2C_CHECKDATA(player_fac_base_info)
			bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~player_fac_base_info() { delete[] info; delete []msg;}
	};
	struct combine_mine_state_change
	{
		int matter_id;
		int state;
	};
	struct facbase_mall_item_info
	{       
		unsigned short items_cnt;
		int actived_items[1];
	};   
	struct get_faction_coupon
	{    
		int faction_coupon;
	};   
	struct get_facbase_money_items_info
	{       
		unsigned short items_cnt;
		struct  
		{       
			int idx;
			int cooldown;
		}item_cooldown[1];
	};     

	struct be_taunted2
	{
		int idPlayer;
		int time;
	};

	struct player_change_puppet_form
	{
		int idPlayer;
	};

	struct invite_active_emote
	{
		int playerid;
		int type;
	};

	struct reply_active_emote
	{
		int playerid;
		int type;
		int answer;
	};

	struct play_active_emote
	{
		int playerid1;
		int playerid2;
		int type;
	};

	struct stop_active_emote
	{
		int playerid1;
		int playerid2;
		int type;
	};

	struct battle_dungeon_ssk_info
	{
		char	tower_index;
		char	monster_index;
		char	award_index;
		int		award_id;
		int		score;
	};

	struct object_try_charge
	{
		char type;
		int target_id;
		A3DVECTOR3 destpos;
	};

	struct object_be_charge_to
	{
		int	idPlayer;
		char type;
		int target_id;
		A3DVECTOR3 destpos;
	};

	struct notify_bloodpool_status
	{
		int idPlayer;
		bool on;
		int cur_hp;
		int max_hp;
	};

	struct player_start_travel_around
	{
		int player_id;
		int travel_id;
		float travel_speed;
		int travel_path_id;
	};

	struct player_stop_travel_around
	{
		int player_id;
	};

	struct fac_base_auc_item
	{
		int idx;
		int itemid;
		int winner; 
		int cost;
		int name_len;
		char playername[20];
		int status;
		int end_time;
	};
	struct fac_base_auc_history
	{
		enum
		{
			PUT_AUC = 1, // 上架
			WIN_AUC, // 获胜
			AUC_TIMEOUT,//流拍
		};
		int event_type;
		int name_len;
		char playername[20];
		int cost;
		int itemid;
	};

	struct player_facbase_auction
	{
		int return_coupon;
		int size;
		fac_base_auc_item* listItem;
		int historysize;
		fac_base_auc_history* phistoryList;
		GP_S2C_CHECKDATA(player_facbase_auction)
			bool FillData(const void* pData, int iBufLen, int* piRealLen=NULL);
		~player_facbase_auction() { delete[] listItem; delete []phistoryList; }
	};

	struct facbase_auction_update
	{
		int type;
		fac_base_auc_item listItem;
	};
	struct fac_coupon_return
	{
		int coupon_retrun;
	};

	struct player_get_summon_petprop
	{
		int pet_index;
		ROLEEXTPROP prop;
	};
	struct get_facbase_cash_item_success
	{
		int item_index;
	};
	struct facbase_auction_add_history
	{
		fac_base_auc_history   entry;
	};
	struct facbase_msg_update
	{
		int msg_size;
		char msg[1];
	};
}

namespace C2S
{
	//	Data type ---------------------------
	struct npc_trade_item
	{
		int tid;
		size_t index;
		size_t count;
	};
	
	struct npc_booth_item
	{
		int tid;
		unsigned short index;
		unsigned short inv_index;
		unsigned short count;
	};

	struct npc_produce_jinfashen_material
	{
		char  config_idx;
		char  type;
		int   item_id;
		short item_idx;
	};

	//	Commands ----------------------------
	enum
	{
		PLAYER_MOVE,		//	0
		LOGOUT,
		SELECT_TARGET,
		NORMAL_ATTACK,
		REVIVE_VILLAGE,		//	Revive in near village

		REVIVE_ITEM,		//	5, Revive by using item
		PICKUP,
		STOP_MOVE,
		UNSELECT,
		GET_ITEM_INFO,

		GET_IVTR,			//	10, Get inventory information
		GET_IVTR_DETAIL,	//	Get inventory detail information
		EXG_IVTR_ITEM,
		MOVE_IVTR_ITEM,
		DROP_IVTR_ITEM,

		DROP_EQUIP_ITEM,	//	15
		EXG_EQUIP_ITEM,
		EQUIP_ITEM,
		MOVE_ITEM_TO_EQUIP,
		GOTO,
		
		THROW_MONEY,		//	20
		GET_EXT_PROP,
		LEARN_SKILL,
		GET_EXT_PROP_BASE,
		GET_EXT_PROP_MOVE,

		GET_EXT_PROP_ATK,	//	25
		GET_EXT_PROP_DEF,
		TEAM_INVITE,
		TEAM_AGREE_INVITE,
		TEAM_REJECT_INVITE,

		TEAM_LEAVE_PARTY,	//	30
		TEAM_KICK_MEMBER,
		TEAM_MEMBER_POS,	//	Get team member's position
		GET_OTHER_EQUIP,
		TEAM_SET_PICKUP,	//	Change team pickup flag

		SEVNPC_HELLO,		//	35, say hello to service NPC
		SEVNPC_GET_CONTENT,
		SEVNPC_SERVE,
		GET_OWN_WEALTH,
		GET_ALL_DATA,		//  取得所有数据 进入游戏时使用，传回所有的包裹，金钱和技能

		USE_ITEM,			//	40, 使用一个物品
		CAST_SKILL,
		CANCEL_ACTION,
		CHARGE_E_FLYSWORD,	//	Charge flysword which is on equipment bar
		CHARGE_FLYSWORD,

		USE_ITEM_T,			//	45, use item with target
        SIT_DOWN,
        STAND_UP,
        EMOTE_ACTION,
        TASK_NOTIFY,

		ASSIST_SELECT,		//	50
		CONTINUE_ACTION,
		STOP_FALL,			//	终止跌落
		GET_ITEM_INFO_LIST,
		GATHER_MATERIAL,

		GET_TRASHBOX_INFO,	//	55
		EXG_TRASHBOX_ITEM,
		MOVE_TRASHBOX_ITEM,
		EXG_TRASHBOX_IVTR,
		TRASHBOX_ITEM_TO_IVTR,

		IVTR_ITEM_TO_TRASHBOX,	//	60
		EXG_TRASHBOX_MONEY,
		TRICK_ACTION,
        SET_ADV_DATA,
        CLR_ADV_DATA,

        TEAM_ASK_TO_JOIN,		//	65
        TEAM_REPLY_JOIN_ASK,
		QUERY_PLAYER_INFO_1,
		QUERY_NPC_INFO_1,
		SESSION_EMOTE,

		CON_EMOTE_REQUEST,		//	70
		CON_EMOTE_REPLY,
		CHANGE_TEAM_LEADER,		
		DEAD_MOVE,
		DEAD_STOP_MOVE,

        ENTER_SANCTUARY,		//	75
		OPEN_BOOTH,
		CLOSE_BOOTH,
		QUERY_BOOTH_NAME,
		COMPLETE_TRAVEL,

		CAST_INSTANT_SKILL,		//	80
		DESTROY_ITEM,
		ENABLE_PVP_STATE,
		DISABLE_PVP_STATE,
		OPEN_BOOTH_TEST,

		SWITCH_FASHION_MODE,	//	85
		ENTER_INSTANCE,
		REVIVAL_AGREE,
		NOTIFY_POS_IN_TEAM,
		CAST_POS_SKILL,

		ACTIVE_RUSH_FLY,		//	90
		QUERY_DOUBLE_EXP,
		DUEL_REQUEST,
		DUEL_REPLY,
		BIND_PLAYER_REQUEST,

		BIND_PLAYER_INVITE,		//	95
		BIND_PLAYER_REQUEST_REPLY,
		BIND_PLAYER_INVITE_REPLY,
		CANCEL_BIND_PLAYER,
		GET_OTHER_EQUIP_DETAIL,

		SUMMON_PET,				//	100
		RECALL_PET,
		BANISH_PET,
		PET_CTRL_CMD,
		PRODUCE_ITEM,

		MALL_SHOPPING,			// 105
		SELECT_TITLE,
		DEBUG_DELIVER_CMD,
		DEBUG_GS_CMD,			
		LOTTERY_CASHING,

		CHECK_RECORD,			//	110
		MALL_SHOPPING2,
		START_MOVE,
		USE_ITEM_WITH_ARG,
		BOT_BEGIN,

		BOT_RUN,				//	115
		GET_BATTLE_SCORE,
		GET_BATTLE_INFO,
		EQUIP_PET_BEDGE,
		EQUIP_PET_EQUIP,

		COMBINE_PET,			//	120
		UN_COMBINE_PET,
		SET_PET_STATUS,
		SET_PET_RANK,
		MOVE_PET_BEDGE_ITEM,

		START_FLY,				//	125
		STOP_FLY,
		SET_FASHION_MASK,
		START_ONLINE_AGENT, 
		JOIN_INSTANCING,        //  情景战场报名申请

		MULTI_BIND_INVITE,      //  130  多人骑乘		
		MULTI_BIND_INVITE_REPLY,
		MULTI_BIND_CANCEL,
		MULTI_BIND_KICK,
		PRESS_SNS_MESSAGE,		//	发布sns信息，征婚，交友，收徒，收帮派人员

		APPLY_SNS_MESSAGE,		//	135	  发布sns申请
		VOTE_SNS_MESSAGE,		//	投票：1支持；0反对
		RESPONSE_SNS_MESSAGE,	//	回复留言
		EXG_POCKET_ITEM,		//  交换随身包裹中的物品
		MOVE_POCKET_ITEM,		//	移动随身包裹中的物品

		EXG_POCKET_IVTR,		//	140 随身包裹与背包中物品的交互
		POCKET_ITEM_TO_IVTR,	//	随身包裹中的物品移动到背包中
		IVTR_ITEM_TO_POCKET,	//	包裹中的物品移动到随身包裹中
		POCKET_ITEM_ALL_TO_IVTR,//	将随身包裹中的所有物品移动到随身包裹中
		CAST_APOISE_SKILL,		//  物品技能，可以将物品使用到自己或者队友身上

		UPDATE_FASHION_HOTKEY,	//  145 编辑时装换装快捷键
		EXG_FASHION_ITEM,		//	交换时装包裹中的物品
		EXG_IVTR_FASHION,		//  交换背包与时装包裹中的物品
		EXG_EQUIP_FASHION,		//	交换装备栏与时装包裹中的物品
		HOTKEY_CHANGE_FASHION,  //  根据hotkey的设置来更换整套时装

		BONUSMALL_SHOPPING,		//  150 红利商城购买物品
		QUERY_OTHERS_ACHIEVEMENT, // 获取其他玩家的成就信息
		UPDATE_PVP_MASK,		// 更新pvp掩码
		START_TRANSFIGURE,		//  开始变身
		STOP_TRANSFIGURE,		//  结束变身

		CAST_TRANSFIGURE_SKILL,	//  155 释放变身技能
		ENTER_CARRIER,			//	进入载体
		LEAVE_CARRIER,			//	离开载体
		MOVE_ON_CARRIER,		//	在载体上移动
		STOP_ON_CARRIER,		//	在载体上停止移动

		EXCHG_HOMETOWN_MONEY,	//  160 庄园交互金钱
		GET_SERVER_TIME,		//  获取服务器时间
		TERRITORY_LEAVE,		//	离开领土战战场
		CAST_CHARGE_SKILL,		//  释放冲锋技能
		UPDATE_COMBINE_SKILL,	//  更新组合技能
		
		UNIQUE_BID,				//  165 唯一最低价投标
		UNIQUE_BID_GET_AWARD,	//  唯一最低价中标领取奖品
		GET_CLONE_EQUIP,		//  获取分身装备信息
		TASK_FLY_POS,			//  飞天符飞到任务位置
		ZONEMALL_SHOPPING,		//  跨服商城
		
		ACTIVITY_FLY_POS,		//  170 活动飞天
		DEITY_LEVELUP,			//  元神升级
		QUERY_CIRCLE_OF_DOOM_INFO, // 查询当前阵眼的阵法信息 
		RAID_LEAVE,				//	玩家主动离开副本 Added 2011-07-25.
		CANCEL_PULLING,			//	玩家取消拉扯技能 Added 2011-07-28.

		GET_RAID_INFO,			//	175 获得进入副本玩家信息 Added 2011-08-03.
		QUERY_BE_DRAGGED_INFO,	//	获取某个玩家被灵力牵引的信息 Added 2011-08-25.
		QUERY_BE_PULLED_INFO,	//	获取某个玩家被拉扯的信息 Added 2011-08-25.
		GET_MALL_SALETIME_ITEM, //	获取商城当前正在销售的限时销售物品  Added 2011-11-11.
		QUERY_RAID_ENTER_COUNT,	//	获取副本进入次数 Added 2011-12-02.

		GET_VIP_AWARD_INFO,		//	180 客户端请求gs获得当前可以领取的VIP奖励列表信息 Added 2012-01-04.
		OBTAIN_VIP_AWARD_BY_ID,	//	客户端向gs请求领取某一奖励 Added 2012-01-04.
		TRY_GET_ONLINE_AWARD,	//	客户端向gs请求验证当前某在线奖励是否能够发放 Added 2012-02-17.
		GET_ONLINE_AWARD,		//	客户端向gs请求领取某在线奖励 Added 2012-02-17.
		FIVE_ANNI_REQUEST,		//	5周年每日签到

		UPGRADE_TREASURE_REGION,//	185 客户端请求gs升级挖宝区域的等级 Added 2012-03-21.
		UNLOCK_TREASURE_REGION,	//	客户端请求gs解锁某挖宝区域 Added 2012-03-21.
		DIG_TREASURE_REGION,	//	客户端请求gs开始挖掘某区域宝物 Added 2012-03-21.
		START_RANDOM_TOWER_MONSTER,//请求服务器开始随机趴塔副本随机选取怪物　Added 2012-04-17.
		ADOPT_LITTLEPET,		// 领养诛小仙

		FEED_LITTLEPET,			// 190喂养诛小仙
		GET_LITTLEPET_AWARD,	// 获取诛小仙奖励
		RUNE_IDENTIFY,			// 元魂鉴定
		RUNE_COMBINE,			// 元魂合成
		RUNE_REFINE,			// 元魂洗练

		RUNE_RESET,				//195 元魂归元
		RUNE_DECOMPOSE,			// 元魂分解
		RUNE_LEVELUP,			// 元魂升级
		RUNE_OPENSLOT,			// 元魂开启符文	弃用
		RUNE_CHANGE_SLOT,		// 元魂孔位改变

		RUNE_ERASE_SLOT,		// 200 元魂擦写符语
		RUNE_INSTALL_SLOT,		// 镶嵌符文
		RUNE_REFINE_ACTION,		// 元魂洗练是否接收结果
		TOWER_REWARD,			// 领取爬塔副本奖励
		RESET_SKILL_PROP,		// 90级前免费洗技能天书

		GET_TASK_AWARD,			// 205 获取特殊任务奖励
		PK_1ST_GUESS,			// 
		PK_TOP3_GUESS,			//
		PK_1ST_GUESS_REWARD,	//
		PK_1ST_GUESS_RESULT_REWARD,	//

		PK_TOP3_GUESS_REWARD,	// 210
		PK_TOP3_GUESS_RESULT_REWARD,	// 
		GET_PLAYER_BET_DATA,	// 获取pk竞猜数据
		START_SPECIAL_MOVE,		// 通知服务器开始移动 Added 2012-06-26.
		SPECIAL_MOVE,			// 同步移动信息 Added 2012-06-26.

		STOP_SPECIAL_MOVE,		// 215 停止移动消息 Added 2012-06-26.
		COLLISION_BATTLEGROUND_APPLY,	// 碰撞副本报名
		GET_REPURCHASE_INV_DATA,		// 获取回购包裹
		EXCHANGE_MOUNT_WING_ITEM,		// 坐骑飞剑包裹里交换位置
		EXCHANGE_INVENTORY_MOUNTWING_ITEM,	// 普通包裹 <-> 坐骑飞剑包裹
		
		EXCHANGE_EQUIPMENT_MOUNTWING_ITEM,	// 220 装备栏 <->坐骑飞剑包裹
		HIDE_VIP_LEVEL,                     //隐藏VIP等级信息
		CHANGE_WING_COLOR,					//通知服务器改变飞剑颜色Added 2012-08-20.
		ASTROLOGY_IDENTIFY,					// 星座鉴定
		ASTROLOGY_UPGRADE,					// 星座升级
		
		ASTROLOGY_DESTORY,					// 225 星座粉碎
		JOIN_RAID_ROOM = 226,
		TALISMAN_REFINESKILL_RESULT,		// 客户端是否接受法宝技能洗练结果
		GET_COLLISION_AWARD,				// 碰撞副本积分兑换
		CANCEL_ICE_CRUST,					// 取消冰壳效果

		PLAYER_FIRST_EXIT_REASON,			// 230 首次登陆不足一小时下线原因调查
		REMEDY_METEMPSYCHOSIS_LEVEL,		// 弥补飞升等级(未满级飞升有数值损失)
		MERGE_POTION,						// 发送合计药瓶消息
		KINGDOM_LEAVE,		                //离开国战
		GET_COLLISION_PLAYER_POS,			// 获取玩家在碰撞副本中位置

		TAKE_LIVENESS_AWARD,				// 235活跃度
		ANCIENT_RAID_LEVEL_AWARD,           // 领取上古副本关卡的奖励
		GET_RAID_LEVEL_RESULT,              // 请求上古副本各关卡是否通关、发放奖励信息
		OPEN_TRASHBOX,						// 国王战远程仓库
		DELIVER_GIFT_BAG,					// 彩票赠送物品领奖

		GET_CASH_GIFT_AWARD,				// 240 商城赠品领取
		GEN_PROP_ADD_ITEM,					// 基础属性增益丹药炼化
		REBUILD_PROP_ADD_ITEM,				// 基础属性增益丹药重炼
		GET_PROPADD,						// 获取玩家基础属性增益
        BUY_KINGDOM_ITEM,                   // 购买积分物品
		GET_TOUCH_AWARD,                    // 245兑换TOUCH物品
		FLOW_BATTLE_LEAVE,					// 离开流水席

		KING_TRY_CALL_GUARD = 247,           //亲卫传送
		BATH_INVITE,                         //搓澡邀请
		BATH_INVITE_REPLY,                   //搓澡邀请回复

		DELIVER_KINGDOM_TASK,                // 250发布国王任务
		RECEIVE_KINGDOM_TASK,                //接受国王任务
		KINGDOM_FLY_BATH_POS,                //传到国战NPC位置
		FUWEN_COMPOSE,			          	// 碎片合成
		FUWEN_UPGRADE,						// 符文升级

		FUWEN_INSTALL,						// 255 符文镶嵌
		FUWEN_UNINSTALL,					// 符文拆除
		RESIZE_INVENTORY = 257,
		REGISTER_FLOW_BATTLE = 258,			 // 流水席报名请求
		ARRANGE_INVENTORY,                   //整理包裹

		ARRANGE_TRASHBOX,                    //260 整理仓库
		GET_WEB_ORDER,						 //领取WEB商城里礼包
		CUSTOMIZE_RUNE,                     //定制元婴
		LOOK_BATTLE_INFO = 263,					 // 请求跨服战场信息
		CONTROL_TRAP,							//引爆可控陷阱
		
		SUMMON_TELEPORT,					//265传送怪
		CREATE_CROSSVR_TEAM,			// 创建战队

		GET_KING_REWARD = 267,               //获得国王连任奖励
		INTERACT_WITH_OBJ,					//与互动物品互动
		CANCEL_INTERACT,					//请求取消互动

		FASHION_COLORANT_COMBINE,	// 270 个性物品合成
		FASHION_ADD_ITEM_COLOR,		// 合成时装
		ENTER_FACTION_BASE,			// 进入帮派基地
		QILIN_INVITE,				//麒麟邀请
		QILIN_INVITE_REPLY,			//麒麟邀请恢复

		QILIN_CANCEL,				//275 麒麟取消
		QILIN_DISCONNECT,			//下麒麟
		QILIN_RECONNECT,		    //上麒麟
		GET_ACHIEVEMENT_AWARD,		// 获取成就奖励
		ADD_FAC_BUILDING,			// 创建基地建筑

		UPGRADE_FAC_BUILDING,		/// 280 升级基地建筑
		REMOVE_FAC_BUILDING,		// 删除基地建筑
		PET_CHANGE_SHAPE,			// 宠物改变形象
		PET_REFINE_ATTR,			// 宠物飞升后属性升级
		INVITE_ACTIVE_EMOTE,		// 邀请互动表情

		REPLY_ACTIVE_EMOTE,			// 285 回复互动表情
		STOP_ACTIVE_EMOTE,		// 取消互动表情

		ZAIBIAN_RAID = 287,			//灾变副本
		GET_FAC_BASE_INFO = 288,
		GET_CS_6V6_AWARD,			//领取奖励信息

		GET_FACBASE_MALL_INFO,          //290 请求帮派商城
		SHOP_FROM_FACBASE_MALL,         //  从帮派商城购物
		CONTRIBUTE_FACBASE_MONEY,       // 捐献元宝
		GET_FACBASE_MONEY_ITEMS_INFO,   // 元宝商城 物品index 到映射时间冷却关系
		BUY_FACBASE_CASH_ITEM,				/// 元宝商城购买物品

		BID_ON_FACBASE,							// 295	帮派竞拍
		POST_FAC_BASE_MSG = 296,				// 帮派基地留言
		EXCHANGE_CS_6V6_AWARD ,				// 申请领取兑换奖励
		PUPPET_FORM_CHANGE,			  //  傀儡战斗状态
		FAC_BASE_TRANSFER,	//基地传送

		EXCHANGE_CS_6V6_MONEY = 300,	// 兑换币兑换
		WITHDRAW_FACBASE_AUCTION, // 领取基地拍卖物品
		WITHDRAW_FACBASE_COUPON,// 领取拍卖失败退回金劵
		GET_FACBASE_AUCTION,     // 客户端获取基地拍卖列表
		LEACE_FACTION_BASE,     // 离开帮派基地

		OBJECT_CHARGE_TO = 305, //冲锋到某个位置
		STOP_TRAVEL_AROUND,		// 新手村飞行结束
		GET_SUMMON_PETPROP,
		TASK_FLY_TO_AREA,		//到达地点完成任务飞天

		//	Below are GM commands
		GM_CMD_START = 1000,		//	1000
		GM_MOVETO_PLAYER,
		GM_CALLIN_PLAYER,
		GM_KICK_PLAYER,			//	需要选中目标,无参数
		GM_INVISIBLE,			//	切换自身隐身,无参数

		GM_INVINCIBLE,			//	1005, 切换自身无敌,无参数
		GM_GENERATE,
		GM_ACTIVE_SPAWNER,
		GM_GENERATE_MOB,
        GM_PLAYER_INC_EXP, 

        GM_RESURRECT,			//	1010
        GM_ENDUE_ITEM,    
        GM_ENDUE_SELL_ITEM,
        GM_REMOVE_ITEM,   
        GM_ENDUE_MONEY,   

        GM_ENABLE_DEBUG_CMD,	//	1015
        GM_RESET_PROP,      
		GM_TRANSFER_MAP,		//	GM跳转地图
	};

	struct cmd_header
	{
        unsigned short cmd;
	};

	struct cmd_player_move
	{
		A3DVECTOR3 vCurPos;
		A3DVECTOR3 vNextPos;
		unsigned short use_time;
		short sSpeed;				//	Move speed 8.8 fix-point
		unsigned char move_mode;	//	Walk run swim fly .... walk_back run_back
		unsigned short stamp;		//	move command stamp
	};

	struct cmd_stop_move
	{
		A3DVECTOR3 vCurPos;
		short sSpeed;				//	Moving speed in 8.8 fix-point
		unsigned char dir;			//	对象的方向
		unsigned char move_mode;	//	Walk run swim fly .... walk_back run_back
		unsigned short stamp;		//	move command stamp
		unsigned short use_time;
	};

	//-------------------------------------------------------------------------
	//玩家通知服务器关于特殊移动相关消息 Added 2012-06-26.
	//开始移动消息
	struct start_special_move
	{
		A3DVECTOR3		velocity;		//当前速度
		A3DVECTOR3		acceleration;	//当前加速度
		A3DVECTOR3		cur_pos;		//当前位置
		char			collision_state;//碰撞状态 0：没有碰撞 1：发生了碰撞
		int				time_stamp;		//时间戳，用于对时
	};

	//特殊移动消息
	struct special_move
	{
		A3DVECTOR3		velocity;		//当前速度
		A3DVECTOR3		acceleration;	//当前加速度
		A3DVECTOR3		cur_pos;		//当前位置信息
		unsigned short	stamp;			//序列号，服务器用？
		char			collision_state;//碰撞状态 0：没有碰撞 1：发生了碰撞
		int				time_stamp;		//移动消息的时间戳，用于对时同步
	};

	//停止特殊移动消息
	struct stop_special_move
	{
		unsigned char	cur_dir;		//玩家当前的朝向
		A3DVECTOR3		cur_pos;		//玩家当前的位置信息
		unsigned short	stamp;			//发送消息顺序时间戳
	};
	//-------------------------------------------------------------------------

	struct collision_battleground_apply
	{
		int map_id;
		int raid_template_id;
		bool is_team;
		char is_cross;		// 2013-8-5 by add zy 是否是跨服6v6
	};

	struct exchange_mount_wing_item
	{
		unsigned char index1;
		unsigned char index2;
	};

	struct exchange_inventory_mountwing_item
	{
		unsigned char idx_inv;
		unsigned char idx_mw;
	};
	
	struct exchange_equipment_mountwing_item
	{
		unsigned char idx_equ;
		unsigned char idx_mw;
	};

	struct hide_vip_level
	{
		char is_hide;
	};
	
	struct change_wing_color
	{
		unsigned char		wing_color;
	};

	//-------------------------------------------------------------------------

	struct cmd_player_logout
	{
		int iOutType;
		int iTrusteeMode;
	};
	
	struct cmd_select_target
	{
		int id;
	};

	struct cmd_normal_attack
	{
		unsigned char pvp_mask;
	};

	struct cmd_pickup
	{
		int	idItem;
		int tid;			//	id of template
	};

	struct cmd_get_item_info
	{
		unsigned char byPackage;
		unsigned char bySlot;
	};

	struct cmd_get_ivtr
	{
		unsigned char byPackage;
	};

	struct cmd_get_ivtr_detail
	{
		unsigned char byPackage;
	};

	struct cmd_exg_ivtr_item
	{
		unsigned char index1;
		unsigned char index2;
	};

	struct cmd_move_ivtr_item
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};

	struct cmd_drop_ivtr_item
	{
		unsigned char index;
		unsigned short amount;
	};

	struct cmd_drop_equip_item	
	{
		unsigned char index;
	};

	struct cmd_exg_equip_item
	{
		unsigned char idx1;
		unsigned char idx2;
	};

	struct cmd_equip_item
	{
		unsigned char idx_inv;
		unsigned char idx_eq;
	};

	struct cmd_move_item_to_equip
	{
		unsigned char idx_inv;  //src
		unsigned char idx_eq;   //dest
	};

	struct cmd_goto
	{
		A3DVECTOR3 vDest;
	};

	struct cmd_throw_money
	{
		size_t amount;
	};

	struct cmd_learn_skill
	{
		int skill_id;
	};

	struct cmd_team_invite
	{
		int idPlayer;	//	向谁发起邀请 
	};

	struct cmd_team_agree_invite
	{
		int idLeader;	//	谁进行的邀请
		int team_seq;
	};

	struct cmd_team_reject_invite
	{
		int idLeader;	//	谁进行的邀请
	};

	struct cmd_team_kick_member
	{
		int idMember;
	};

	struct cmd_team_member_pos
	{
		unsigned short wMemCnt;
		int aMemIDs[1];
	};

	struct cmd_get_other_equip
	{
		unsigned short size;
		int idlist[1];
	};

	struct cmd_team_set_pickup
	{
		short pickup_flag;
	};

	struct cmd_sevnpc_hello
	{
		int id;
	};

	struct cmd_sevnpc_get_content
	{
		int service_id;
	};

	struct cmd_sevnpc_serve
	{
		int service_type;
		size_t len;
	};

	struct cmd_get_own_wealth
	{
		BYTE byPack;	//	Get detail info. flag
		BYTE byEquip;
		BYTE byTask;
	};

	struct cmd_get_all_data
	{
		BYTE byPack;	//	Get detail info. flag
		BYTE byEquip;
		BYTE byTask;
//		BYTE byFashion;
	};

	struct cmd_use_item
	{
		unsigned char where;
		unsigned char byCount;
		unsigned short index;
		int item_id;
	};

	struct cmd_cast_skill
	{
		int skill_id;
		unsigned char pvp_mask;
		unsigned char target_count;
		A3DVECTOR3	cast_skill_pos;		//Added 2012-05-03.
		int targets[1];
	};

	struct cmd_charge_equipped_flysword
	{
		unsigned char element_index; 
		int count;
	};

	struct cmd_charge_flysword
	{
		unsigned char element_index;  
		unsigned char flysword_index; 
		int count;
		int flysword_id;
	};

	struct cmd_use_item_t
	{
		unsigned char where;
		unsigned char pvp_mask;		//	只对攻击性物品有效
		unsigned short index;
		int item_id;
	};

	struct cmd_emote_action
	{
		unsigned short action;
	};

	struct cmd_task_notify
	{
		unsigned int size;
		byte placeholder;	//	Task data ...
	}; 

	struct cmd_get_item_info_list
	{
		char where;
		unsigned char count;
		//	follows: unsigned char item_list[1];
	};

	struct cmd_assist_sel
	{
		int idTeamMember;
	};

	struct cmd_gather_material
	{
		int mid;
		unsigned short tool_pack;
		unsigned short tool_index;
		int tool_type;
		int id_task;
	};

	struct cmd_get_trashbox_info
	{
		char detail;
	};

	struct cmd_exg_trashbox_item
	{
		unsigned char index1;
		unsigned char index2;
	};

	struct cmd_move_trashbox_item
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};

	struct cmd_exg_trashbox_ivtr
	{
		unsigned char idx_tra;
		unsigned char idx_inv;
	};

	struct cmd_trashbox_item_to_ivtr
	{
		unsigned char idx_tra;
		unsigned char idx_inv;
		unsigned short amount;
	};

	struct cmd_ivtr_item_to_trashbox
	{
		unsigned char idx_inv;
		unsigned char idx_tra;
		unsigned short amount;
	};

	struct cmd_exg_trashbox_money
	{
		unsigned int inv_money;   //从身上取出的钱数
		unsigned int trashbox_money;  //从仓库中取得钱数
	};

	// 新增随身包裹
	struct cmd_exg_pocket_item
	{
		unsigned char index1;
		unsigned char index2;
	};

	struct cmd_move_pocket_item
	{
		unsigned char src;
		unsigned char dest;
		unsigned short amount;
	};

	struct cmd_exg_pocket_ivtr
	{
		unsigned char idx_poc;
		unsigned char idx_inv;
	};

	struct cmd_pocket_item_to_ivtr
	{
		unsigned char idx_poc;
		unsigned char idx_inv;
		unsigned short amount;
	};

	struct cmd_ivtr_item_to_pocket
	{
		unsigned char idx_inv;
		unsigned char idx_poc;
		unsigned short amount;
	};

	struct cmd_trick_action
	{
		unsigned char trick;
	};

	struct cmd_set_adv_data
	{
		int data1;
		int data2;
	};

	struct cmd_team_ask_join
	{
		int idTarget;
	};

	struct cmd_team_reply_join_ask
	{
		int idAsker;
		unsigned char result;
	};

	struct cmd_query_player_info_1
	{
		unsigned short count;
		int id[1];
	};

	struct cmd_query_npc_info_1
	{
		unsigned short count;
		int id[1];
	};

	struct cmd_session_emote
	{
		unsigned char action;
	};

	struct cmd_con_emote_request
	{
		unsigned short action;
		int target;
	};

	struct cmd_con_emote_reply
	{
		unsigned short result;
		unsigned short action;
		int target;
	};

	struct cmd_change_team_leader
	{
		int idLeader;
	};

	struct cmd_dead_move
	{
		float y;
		unsigned short use_time;
		unsigned short speed;
		unsigned char move_mode;
	};

	struct cmd_dead_stop_move
	{
		float y;
		unsigned short speed;
		unsigned char dir;
		unsigned char move_mode; 
	};

//	准备摆摊
	struct cmd_open_booth_test
	{
		int slot_index;
		int item_id;
	};

	struct cmd_open_booth
	{
		int slot_index;
		int item_id;
		unsigned short count;
		char name[28];

		struct entry_t
		{
			int type;
			size_t index;
			size_t count;
			size_t price;

		} list[1];
	};

	struct cmd_query_booth_name
	{       
		unsigned short count;
		int list[1];
	};

	struct cmd_enter_instance
	{
		int iTransIndex;
		int idInst;
	};

	struct cmd_cast_pos_skill
	{
		int skill_id;
		A3DVECTOR3 pos;
	};

	struct cmd_active_rush_fly
	{      
		int is_active;
	};

	struct cmd_duel_request
	{       
		int target;
	};

	struct cmd_duel_reply
	{
		int who;
		int param; // 0 同意  非0，拒绝的原因
	};

	struct cmd_bind_player_request
	{
		int target;
	};

	struct cmd_bind_player_invite
	{
		int target;
	};

	struct cmd_bind_player_request_reply
	{
		int who;
		int param;  //	0 同意
	};

	struct cmd_bind_player_invite_reply
	{
		int who;
		int param;  //	0 同意
	};

	// 多人骑乘的踢人操作
	struct cmd_multi_bind_kick
	{
		unsigned char pos;
	};

	// 互动物品
	struct cmd_interact
	{
		int npc_id;
		int tool_type;	//互动需要的物品
		int task_id;	//互动需要的任务
	};

	//	完成互动
	struct cmd_finish_interact
	{
		int npc_id;
	};

	//	取消互动
	struct cmd_cancel_interact
	{
		int npc_id;
	};

	struct cmd_get_other_equip_detail 
	{
		int target;
	};

	struct cmd_get_clone_equip
	{
		int idMaster;
		int idClone;
	};

	struct cmd_banish_pet
	{
		size_t pet_index;
	};

	struct produce_item     //	生产一次物品的命令
	{
         int recipe_id;
	};

	struct mall_shopping
	{
		unsigned int count;

		struct goods
		{
			short goods_id;
			short goods_index;
			short goods_pos;
		} list[1];
	};

	struct  select_title                       //玩家选择显示新的称谓， title_id是称谓列表中的id，若此值为0则表示其选择不显示任何称谓
	{
        short title_id;
	};

	struct cmd_debug_deliver_cmd
	{
		unsigned short type;
	//	char buf[];		//	command buffer
	};

	struct lottery_cashing       //兑奖彩票
	{
        int item_index;      //彩票在包裹栏的位置
	};

	struct check_record        //检查是否能够继续进行录像 
	{
        int item_index;    //包裹栏中录像道具的位置
	};

	struct mall_shopping_2   //百宝阁购买的新协议
	{
        unsigned short goods_id;     //要购买的物品ID
        unsigned short goods_index;  //要购买的物品在百宝阁中的索引
        unsigned short goods_slot;   //...
        unsigned short count;        //要购买几次
	};

	struct use_item_with_arg      //带参数使用物品
	{
		unsigned char where;  //物品在哪里
		unsigned char count;  //使用几个
		unsigned short index; //物品的位置
		int  item_id;         //物品的类型
		// char arg[];           //物品的自定义参数区域
	};

	struct equip_pet_bedge
    {
		unsigned char inv_index;
		unsigned char pet_index;
     };

    struct equip_pet_equip
    {
        unsigned char inv_index;
        unsigned char pet_index;
    };

	struct summon_pet
	{
		unsigned char pet_index;
	};

    struct recall_pet
    {
        unsigned char pet_index;
    };

    struct combine_pet
    {
		unsigned char pet_index;
        unsigned char combine_type;     //0 通灵 1 御宝
    };

    struct un_combine_pet
    {
		unsigned char pet_index;
    };

    struct set_pet_status
    {       
        unsigned char pet_index;
        int pet_tid;
        unsigned char main_status; // 0 战斗 1 采集 2 制造 3 休息
        unsigned char sub_status; // 只有main_status为 1时有用 ,0 种植 1 伐木 2 狩猎 3 钓鱼 4 采矿 5 考古
    };

	struct set_pet_rank
    {
        unsigned char pet_index;
        int pet_tid;
        unsigned char rank;
    };

	struct buy_kingdom_item
	{
	    char type;
        int  index;
	};

	struct get_touch_award
	{
	    char type;
		int  count;
	};
    struct bath_invite
	{
	     int who;
	};
	struct bath_invite_reply
	{
	     int who;
		 int param;
	};
	struct deliver_kingdom_task
	{
	     int task_type;   // 1-白色,2-绿色，3-蓝色，4-橙色
	};
	struct receive_kingdom_task
	{
	      int tasktype; // 1-白色,2-绿色，3-蓝色，4-橙色
	};
	struct fuwen_compose
	{
		bool consume_extra_item;	// 是否消耗额外道具增加合成次数
		int  extra_item_index;		// 消耗的额外道具index
	};

	struct fuwen_upgrade
	{
		int main_fuwen_index;			// 主符文
		int main_fuwen_where;			// 所在包裹(看包裹id定义): 0-包裹, IVTRTYPE_FUWEN-符文包
		int assist_count;
		int assist_fuwen_index[1];		// 辅助符文列表
	};

	struct fuwen_install
	{
		int src_index;		// 符文在包裹中的位置
		int dst_index;		// 要安装到符文包裹里面的目标位置
	};

	struct fuwen_uninstall 
	{
		int fuwen_index;	// 符文在符文包裹里面的位置
		int assist_index;	// 道具在普通包裹里的位置
		int inv_index;		// 符文拆除后在包裹里的位置
	};

	struct move_pet_bedge_item
    {
        unsigned char src_index;
        unsigned char dst_index;
    };
	
	struct cmd_join_instancing
	{
		int map_id;
	};

	//sns发布信息
	struct cmd_press_sns_message
	{
		char message_type;
		unsigned char occupation;
		char gender;
		int level;
		int faction_level;
		size_t message_len;
//		char message[1];
	};
	//sns发布申请
	struct cmd_apply_sns_message
	{
		char message_type; // 0 - marriage 1 - friend 2 - mentor 3 - faction
		int message_id;
		size_t message_len;
//		char message[1];
	};

	//sns投票支持或者反对
	struct cmd_vote_sns_message
	{
		char vote_type; // 0 - against   1 - support
		char message_type;
		int message_id;
	};
	//sns回复留言
	struct cmd_response_sns_message
	{
		char message_type;
		int message_id;
		short dst_index;
		size_t message_len;
//		char message[1];
	};
	
	//物品技能，可选择使用的对象
	struct cmd_cast_apoise_skill
    {
        int skill_id;
        char skill_type;  // 0 普通技能 1瞬发技能
        int item_id;
        short item_index;
        unsigned char force_attack;
        unsigned char target_count;
		int targets[1];
    };

	struct cmd_update_fashion_hotkey
	{
		int count;
		struct fashion_hotkey
		{
			int index;
			int id_head;
			int id_cloth;
			int id_shoe;
		}list[1];
	};

	// 新增时装包裹FASHION_INVENTORY
	// 时装包裹内部交换
	struct cmd_exg_fashion_item
    {
        unsigned char index1;
        unsigned char index2;
    };
	// 时装包裹与背包的物品交换
	struct cmd_exg_ivtr_fashion
	{
		unsigned char idx_ivr;
		unsigned char idx_fas;
	};
	// 时装包裹与装备栏的物品交换
	struct cmd_exg_equ_fashion
	{
		unsigned char idx_equ;
		unsigned char idx_fas;
	};
	// 使用快捷键来更换时装
	struct hotkey_change_fashion
	{
		unsigned char key_index;
		unsigned char idx1;		// 头饰
		unsigned char idx2;		// 衣服
		unsigned char idx3;		// 鞋子
	};
	
	struct query_others_achievement         
	{
		int target;
	};
	
	struct update_pvp_mask
	{
		char pvp_mask;
	};

	struct cast_transfigure_skill          
    {
        int skill_id;
        char skill_type; 
        unsigned char pvp_mask;
        unsigned char target_count;
		A3DVECTOR3 cast_skill_pos;	//无目标释放技能也使用这个接口 Modified 2012-05-05.
        int targets[1];
    };

	struct cmd_enter_carrier  
	{
		int carrier_id; 		// 想要乘坐的交通工具id
		A3DVECTOR3 rpos;		// 相对坐标
		unsigned char rdir;		// 相对方向
	};

	struct cmd_leave_carrier 
	{
		int carrier_id; 		// 想要离开的交通工具id
		A3DVECTOR3 pos;			// 绝对坐标和方向
		unsigned char dir;		// 绝对方向
	};
	
	struct cmd_move_on_carrier	
	{
		A3DVECTOR3 vCurRPos;
		A3DVECTOR3 vNextRPos;
		unsigned short use_time;
		short sSpeed;			// Move speed 8.8 fix-point
		unsigned char move_mode;// Walk run swim fly .... walk_back run_back
		unsigned short cmd_seq;	// 命令序号
	};

	struct cmd_stop_on_carrier
	{
		A3DVECTOR3 rCurPos;			//	在载体坐标系中的位置
		short sSpeed;				//	Moving speed in 8.8 fix-point
		unsigned char rdir;			//	载体坐标系中的方向
		unsigned char move_mode;	//	Walk run swim fly .... walk_back run_back
		unsigned short stamp;		//	move command stamp
		unsigned short use_time;
	};

	struct exchg_hometown_money  //  系统金钱兑换庄园货币                                                         
	{                                                                                                                                                 
		int amount;                                                                      
	};

	struct territory_leave 
	{
		int territory_id;
	};

	struct cast_charge_skill
    {
        int skill_id;
        unsigned char force_attack;
        int charge_target;
        A3DVECTOR3 charge_pos;
        unsigned char target_count;
        int  targets[1];
    };

	struct update_combine_skill
    {
        int skill_id;
        int element_count;
        int element_id[1];
    };

	struct unique_bid	//Modified 2011-02-16.
	{
		int nBidMoneyLowerLimit;//竞拍价格下限
		int nBidMoneyUpperLimit;//竞拍价格上限
	};

	//向服务器查询当前阵眼所在的阵法的信息 Added 2011-06-22.
	struct query_circleofdoom_info
	{
		int sponsor_id; //阵法发起者（阵眼）id
	};

	//向服务器查询当前某个拉扯技能当前状态的信息 Added 2011-08-25.
	struct query_be_dragged_info
	{
		int player_be_dragged_id;	//被拉扯的目标玩家的id ==〉原因是gs拉扯信息存在目标玩家身上
	};

	//向服务器查询当前某个牵引技能当前状态信息 Added 2011-08-25.
	struct query_be_pulled_info
	{
		int player_be_pulled_id;	//被牵引的目标玩家的id ==〉gs将牵引信息存在目标玩家身上
	};

	//向服务器查询进入某副本的次数 Added 2011-12-02.
	struct query_raid_enter_count
	{
		int	map_id;					//副本地图ID
	};

	//向服务器请求领取相应vip奖励 Added 2012-01-04.
	struct obtain_vip_award_by_id
	{
		int	award_id;				//奖励模板id
		int	item_id;				//奖励物品id
	};

	//客户端向gs请求验证当前某在线奖励是否能够发放,以及领取某在线奖励（走同一条消息结构） Added 2012-02-17.
	struct obtain_online_award
	{
		int	award_id;				//在线奖励模板id
		int award_index;			//大礼包中当前可发放的小礼包位置index
		int award_small_bag_id;		//大礼包中当前可发放得小礼包对应的模板id
	};

	//---------------------------------
	//Added 2012-03-21.
	//升级挖宝区域
	struct upgrade_treasure_region
	{
		int item_id;				//升级挖宝区域的道具ID
		int item_slot;				//物品在包裹中的位置
		int	region_id;				//区域ID，模板ID
		int region_pos;				//区域位置，0-8，普通，9-11，隐藏
	};
	
	//解锁挖宝区域
	struct unlock_treasure_region
	{
		int	item_id;				//解锁挖宝区域的道具ID
		int	item_slot;				//解锁道具在包裹中的位置
		int region_id;				//被解锁的挖宝区域ID
		int region_pos;				//被解锁的挖宝区域位置，，0-8，普通，9-11，隐藏
	};

	//探索挖宝区域
	struct dig_treasure_region
	{
		int region_id;				//挖宝区域ID
		int region_pos;				//挖宝区域位置
	};
	
	struct start_random_tower_monster
	{
		char click_index;			//点了哪个牌，传到服务器帮忙保存，掉线情况下传回来
	};

	//喂养诛小仙
	struct feed_littlepet
	{
		int item_id;
		int item_count;
	};

	// 元魂鉴定
	struct rune_identify
	{
		int rune_index;
		int assist_index;
	};

	// 元魂合成
	struct rune_combine
	{
		int rune_index;
		int assist_index;
	};

	// 元魂洗练
	struct rune_refine
	{
		int rune_index;
		int assist_index;
	};
	
	//195 元魂归元
	struct rune_reset
	{
		int rune_index;
		int assist_index;
	};

	// 元魂分解
	struct rune_decompose
	{
		int rune_index;
	};

	// 元魂升级
	struct rune_levelup
	{
		int rune_index;
	};

	// 元魂开启符文	弃用
	struct rune_openslot
	{
		int rune_index;
	};

	// 元魂孔位改变
	struct rune_change_slot
	{
		int rune_index;
		int assist_index;
	};
	
	// 元魂擦写符语
	struct rune_erase_slot
	{
		int rune_index;
		int stone_index;
	};

	struct tower_reward
	{
		int raid_template_id;
		int	level;
		int type;
	};

	// 90级前免费洗技能天书
	struct reset_skill_prop
	{
		unsigned char opcode;	// 1:技能点，2:天书点
	};

	struct rune_install_slot 
	{
		int rune_index;
		int slot_index;
		int stone_index;
		int stone_id;
	};

	struct rune_refine_action
	{
		bool accept_result;
		int rune_index;
	};

	// 205
	struct get_task_award
	{
		int type;	// 1:补填身份信息奖励；  2:补填账号以及邮箱信息奖励
	};

	struct pk_1st_guess
	{
		int area;
		int cnt;
		struct item_info
		{
			int index;
			int cnt;
		}items[1];
	};

	struct pk_top3_guess
	{
		bool cancel;
		int area;
		int cnt;
		struct item_info
		{
			int index;
			int cnt;
		}items[1];
	};

	struct pk_top3_guess_result_reward
	{
		char type;
	};


	//---------------------------------
	
	struct get_online_award
	{
		char request;	// 0:恭喜	1:领奖
	};

	struct task_fly_pos
	{
		int task_id;
		int npc_id;
	};
	
	struct activity_fly_pos
	{
		int activity_id;
	};

	struct astrology_identify 
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
	};

	struct astrology_upgrade 
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
		int stone_index;	// 道具 index
		int stone_id;		// 道具 id
	};

	struct astrology_destory 
	{
		int item_index;		// 物品 包裹装备
		int item_id;		// 物品 id
	};

	struct player_first_exit_reason 
	{
		unsigned char reason; // 原因编号, 按界面显示从上往下的顺序 0-5
	};

	struct gen_prop_add_item
	{
		int	  material_id;
		short material_idx;
		short material_cnt;
	};

	struct rebuild_prop_add_item
	{
		int   itemId;
		short itemIdx;
		int   forgeId;  // 辅助道具id, 配置中最多配三个, 按从前到后的优先顺序使用
		short forgeIdx;
	};

	///////////////////////////////////////////////////////////////////
	//
	//	GM commands
	//
	///////////////////////////////////////////////////////////////////

	struct gm_cmd_moveto_player
	{
		int pid;
	};

	struct gm_cmd_callin_player
	{
		int pid;
	};

	struct gm_cmd_generate
	{
		int tid;
	};

	struct gm_cmd_active_spawner
	{
		unsigned char is_active;
		int sp_id;		//	生成器ID值
	};

	struct gm_cmd_generate_mob
	{
		int mob_id;			//	生成那种怪物
		int vis_id;			//	期望这种怪物看起来什么样，0则无效
		short count;		//	生成的数量
		short life;			//	生成怪物的生存期，0为无限
		size_t name_len;
	//	char name[];
	};
	
	struct item_trade
	{
	    int item_type;
	    size_t index;
	    size_t count;
	};

	struct gm_cmd_transfer_map
	{
		int idMap;
		float x;
		float y;
		float z;
	};

    //加入副本，这里没有走NPC服务，现在用于帮战副本，因为帮众不需要报名
    struct  join_raid_room
    {
        int mapid;
		int raid_template_id;
		int roomid;
        char raid_faction;  //阵营：0 无阵营 1 攻 2 守 3 观察者
    };

	struct talisman_refineskill_result
	{
		int index;
		int id;
		char result;
	};
	
	struct get_collision_award
	{
		bool daily_award;
		int award_index;
	};

	struct remedy_metempsychosis_level // 2012-10-16 添加 飞升结构 add by zy
	{
		int item_index;
	};

	struct merge_medicine// 2012-10-16 添加药剂合并  add by zy
	{
		int bottle_id;
		int bottle_index;
		int potion_id;
		int potion_index;
	};
	

	struct get_collision_player_pos
		
	{
		int role_id;
	};

	// 2012-11-8 领取活跃度奖励  add by zy
	struct take_liveness_award
	{
		int grade;  // 0, 1, 2, 3表示档次
	};

	struct ancient_raid_level_award 
	{
		char level; //-1表示总挑战， 0-9表示其它关卡
	};

	struct deliver_gift_bag
	{
		int index;
		int item_id;
	};

	struct resize_inv
	{
		char type;
		int new_size;
		int cnt;
		struct Item_Info
		{
			int item_idx;
			int item_id;
			int item_cnt;
		}cost_item[1];		
	};
	
	struct arrange_inventory
	{
		char page;   //0 全部整理
	};

	struct arrange_trashbox
	{
		char page;  //0 全部整理  
	};

	struct get_web_order
	{
		__int64 order_id;
	};
	struct get_Pkbattle_info
	{
		int map_id;
	};

	struct customize_rune
	{
		int type;
		int count;
		int prop[1];
	};

	struct control_trap
	{
		int id;
		int tid;
	};

	struct summon_teleport
	{
		int npc_id;
	};
	struct create_crossvr_tem
	{
		int name_len;
		//char team_name;
	};
	

	struct fashion_colorant_combine
	{
		int colorant_index1;
		int colorant_index2;
		int colorant_index3;
	};

	struct fashion_add_color
	{
		int colorant_index;
		int nPackType;
		int fashion_item_index;
	};

	// 获取成就奖励
	struct get_achievement_award
	{
		unsigned short achieve_id;
		unsigned int award_id;
	};

	struct pet_change_shape         
	{
		int pet_index;
		int shape_id; //1-7     
	};

	struct pet_refine_attr          
	{
		int pet_index;          
		int attr_index; // 0 - 14
		int assist_id;          
		int assist_count; //1 10 100
	};

	struct qilin_invite
	{
		int who;
	};

	struct qilin_invite_reply
	{
		int who;
		int param;
	};

	struct add_fac_building
	{
		int fid;   // 帮派id
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
	};

	struct upgrade_fac_building
	{
		int fid;   // 帮派id
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
		int cur_level;   // 当前等级
	};

	struct remove_fac_building
	{
		int fid;   // 帮派id
		int field_index; // 地块索引, 从1开始编号
		int building_tid; // 建筑的模板ID
	};

	struct get_cs_6v6_award
	{
		int award_type;	/// -0-每周分档奖励 1-每周兑换币奖励 2-战队赛季奖励
		int award_level; // 奖励的档位
	};
	struct shop_from_facbase_mall
	{
		int item_index;
		int item_id;
		int cnt;
	};
	struct contribute_facbase_money
	{
		int cnt;
	};
	struct exchange_cs_6v6_award
	{
		int index;
		int item_id;
		int currency_id;
	};
	struct post_fac_base_msg
	{
		int fid;
		size_t msg_len;
		//char msg[];
	};
	struct fac_base_transfer
	{
		int region_index;
	};

	struct invite_active_emote
	{
		int playerid;
		int type;
	};

	struct reply_active_emote
	{
		int playerid;
		int type;
		int answer;
	};
	struct exchange_cs_6v6_money
	{
		int type;       //0: 金兑换银  1:金兑换铜  2:银兑换铜
		int count;      //参加兑换的数量
	};
	struct bid_on_facbase 
	{
		int item_index;
		int item_id;
		int coupon;
		int name_len;
		//char player_name[];
	};
	struct buy_facbase_cash_item
	{
		int item_index;
	};

	struct object_chartge_to
	{
		int type;
		int target_id;
		A3DVECTOR3 destpos;
	};
	struct withdraw_facbase_auction
	{
		int item_index;
	};

	struct task_fly_to_area
	{
		int taskid;
	};
};

namespace CHAT_S2C
{
	enum
	{
		CHAT_EQUIP_ITEM,
		CHAT_ACHIEVEMENT,
	};      
        
	struct chat_equip_item
	{       
		short cmd_id;
		int type;
		int expire_date;
		int state;
		unsigned short content_length;
		char content[1];
	};

	struct chat_achievement
	{
		short			cmd_id;
		unsigned short	achievement_id;
		int				finish_time;
		size_t			player_name_len;
		char			player_name[1];
	};
}                       
                    
namespace CHAT_C2S      
{
	enum            //CHAT_CMD
	{               
		CHAT_EQUIP_ITEM,
		CHAT_ACHIEVEMENT
	};      

	struct chat_equip_item
	{
		short cmd_id;
		char where;
		short index;
	};

	struct chat_achievement
	{
		short cmd_id;
		unsigned short achieve_id;
	};
}

#pragma pack()

