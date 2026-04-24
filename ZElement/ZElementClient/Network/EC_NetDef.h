/*
 * FILE: EC_NetDef.h
 *
 * DESCRIPTION: Net definitions
 *
 * CREATED BY: CuiMing, Duyuxin, 2006/2/7
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define ISPLAYERID(id)	(((id) && !((id) & 0x80000000)) || ISCLONEPLAYERID(id) || ISSPIRITPLAYERID(id))
#define ISNPCID(id)		(((id) & 0x80000000) && !((id) & 0x40000000))
#define ISMATTERID(id)	(((id) & 0xF0000000) == 0xC0000000)
// 玩家的分身，在客户端认为分身和人物一样，而服务器端把分身认为是怪物
// 以后的协议中，如果添加同时涉及到玩家和npc的协议，则要进行微处理，把对应的协议转发
#define ISCLONEPLAYERID(id)  (((id) & 0xF0000000) == 0xE0000000)

// 轩辕职业的灵分身
#define ISSPIRITPLAYERID(id)  (((id) & 0xF0000000) == 0xF0000000)


#define MAX_SKILL_ELEM_NUM 10
//	Move mode
enum
{
	GP_MOVE_WALK	= 0,
	GP_MOVE_RUN		= 1,
	GP_MOVE_STAND	= 2,
	GP_MOVE_FALL	= 3,
	GP_MOVE_SLIDE	= 4,
	GP_MOVE_PUSH	= 5,
	GP_MOVE_FLYFALL	= 6,
	GP_MOVE_RETURN	= 7,
	GP_MOVE_JUMP	= 8,
	GP_MOVE_QINGGONG= 9,
	GP_MOVE_PULLED	= 10,		//	拉扯状态的移动 Added 2011-07-29.
	GP_MOVE_MASK	= 0xf,

	GP_MOVE_TURN	= 0x10,		//	Turnaround
	GP_MOVE_DEAD	= 0x20,

	GP_MOVE_AIR		= 0x40,
	GP_MOVE_WATER	= 0x80,
	GP_MOVE_ENVMASK	= 0xc0,
};

//	Leave team reason
enum
{
	GP_LTR_LEAVE = 0,		//	Normal leave
	GP_LTR_KICKEDOUT,		//	Was kicked out
	GP_LTR_LEADERCANCEL,	//	Leader cancel the team
};

//	Team member pickup flag
enum
{
	GP_TMPU_RANDOM = 0,
	GP_TMPU_FREEDOM,
};

//	NPC service type
enum
{
	GP_NPCSEV_BUY = 1,			//	1, NPC buy from player
	GP_NPCSEV_SELL,				//	NPC sell to player
	GP_NPCSEV_REPAIR,
	GP_NPCSEV_HEAL,
	GP_NPCSEV_TRANSMIT,			//	Transmit to somewhere
	
	GP_NPCSEV_TASK_RETURN,		//	6, Return task
	GP_NPCSEV_TASK_ACCEPT,		//	Accept task
	GP_NPCSEV_TASK_MATTER,		//	Task matter
	GP_NPCSEV_LEARN,			//	Learn skill
	GP_NPCSEV_EMBED,			//	Embed stone

	GP_NPCSEV_CLEAR_TESSERA,	//	11, Clear tessear
	GP_NPCSEV_MAKEITEM,
	GP_NPCSEV_RESET_PK_VALUE,
	GP_NPCSEV_TRASHPSW,			//	Change trash password
	GP_NPCSEV_OPENTRASH,		//	Open trash

	GP_NPCSEV_OPENFACTIONTRASH,	//	16
	GP_NPCSEV_IDENTIFY,			//	Identify item
	GP_NPCSEV_FACTION,			//	About faction
	GP_NPCSEV_BOOTHSELL,		//	Player booth sell		
	GP_NPCSEV_TRAVEL,			//	Travel

	GP_NPCSEV_BOOTHBUY,			//	21, Player booth buy
	GP_NPCSEV_WAYPOINT,
	GP_NPCSEV_FORGETSKILL,
	GP_NPCSEV_FACECHANGE,
	GP_NPCSEV_MAIL,

	GP_NPCSEV_VENDUE,			//	26
	GP_NPCSEV_DBLEXPTIME,
	GP_NPCSEV_HATCHPET,
	GP_NPCSEV_RESTOREPET,
	GP_NPCSEV_BATTLE,
	
	GP_NPCSEV_BUILDTOWER,		//	31
	GP_NPCSEV_LEAVEBATTLE,
	GP_NPCSEV_RETURNSTATUSPT,
	GP_NPCSEV_ACCOUNTPOINT,
	GP_NPCSEV_BINDITEM,

	GP_NPCSEV_DESTROYBINDITEM,	//	36
	GP_NPCSEV_CANCELDESTROY,
	GP_NPCSEV_STOCK_TRANSACTION,
	GP_NPCSEV_STOCK_OPERATION,
	GP_NPCSEV_TALISMAN_REFINE,

	GP_NPCSEV_TALISMAN_UPGRADE,	// 41
	GP_NPCSEV_TALISMAN_REMOVE_OWNER,
	GP_NPCSEV_TALISMAN_COMBINE,
	GP_NPCSEV_TALISMAN_ENCHANT,
	GP_NPCSEV_WAR_CHALLENGE,

	GP_NPCSEV_WAR_CONSTRUCTION, // 46
	GP_NPCSEV_WAR_ARCHER,
	GP_NPCSEV_ADOPT_PET,
	GP_NPCSEV_FREE_PET,
	GP_NPCSEV_COMBINE_PET,
	
	GP_NPCSEV_RENAME_PET,		// 51
	GP_NPCSEV_VEHICLE_UPGRADE,
	GP_NPCSEV_ITEM_TRADE,
	GP_NPCSEV_LOCK_ITEM,
	GP_NPCSEV_UNLOCK_ITEM,

	GP_NPCSEV_REPAIR_DAMAGED_ITEM,
	GP_NPCSEV_BLEED_SACRIFICE,		//装备血祭
	GP_NPCSEV_EMBED_SOUL,			//器魄镶嵌
	GP_NPCSEV_CLEAR_SOUL_TESSERA,	//器魄拆除
	GP_NPCSEV_REFINE_SOUL,			//恢复魂力

	GP_NPCSEV_BREAKDOWN_EQUIP,		//61      //装备拆解 
	GP_NPCSEV_MELD_SOUL,			//器魄融合
	GP_NPCSEV_PET_EQUIP_ENCHANT,	//宠物装备强化
	GP_NPCSEV_CHANGE_FACE,			//形象美容
	GP_NPCSEV_REFINE_SHAPE_CARD,	//炼化幻化石（变身卡）包括：归元，强化，契合，轮回

	GP_NPCSEV_RECOVER_CARD_WAKAN,	//66 恢复幻化石（变身卡）的灵力值
	GP_NPCSEV_ARENA_JOIN,			//竞技场报名，以前走的是协议battle_jion，发协议给delivery, 因为要在报名的时候扣报名券，改成服务发协议给GS
	GP_NPCSEV_TERRITORY_CHALLENGE,	//领土战宣战
	GP_NPCSEV_TERRITORY_ENTER,		//进入领土战战场

	GP_NPCSEV_TERRITORY_AWARD,		//70 领土战奖励
	GP_NPCSEV_TELEPORTATION_CHARGE,	//星盘充能服务
	GP_NPCSEV_REPAIR_DAMAGED_ITEM2,	//修复破损物品的特殊服务
	GP_NPCSEV_UPGRADE_EQUIP,		//装备升级服务
	GP_NPCSEV_ONLINE_TRADE,			//网上交易

	GP_NPCSEV_TRANSMIT_TO_CROSS,	//75 传送到跨服服务器
	GP_NPCSEV_TRANSMIT_TO_NORMAL,	//从跨服服务器传送回普通服务器
	GP_NPCSEV_IDENTIFY_GEM_SLOTS,	//宝石插槽鉴定服务
	GP_NPCSEV_REBUILD_GEM_SLOTS,	//宝石插槽重铸服务
	GP_NPCSEV_CUSTOMIZE_GEM_SLOTS,	//宝石插槽定制服务

	GP_NPCSEV_EMBED_GEMS,			//80 宝石镶嵌服务
	GP_NPCSEV_REMOVE_GEMS,			//宝石拆除服务
	GP_NPCSEV_UPGRADE_GEMS,			//宝石升品服务
	GP_NPCSEV_REFINE_GEMS,			//宝石精炼服务
	GP_NPCSEV_EXTRACT_GEMS,			//宝石萃取服务

	GP_NPCSEV_SMELT_GEMS,			//85 宝石熔炼服务
	GP_OPEN_RAID_ROOM,				//开启副本房间服务 Added 2011-07-19.
	GP_JOIN_RAID_ROOM,				//报名加入副本房间服务 Added 2011-07-19.
	GP_PET_UPGRADE_WITH_XIANDOU,	//直接用仙豆提升宠物
	GP_CHANGE_NAME,					//改名服务

	GP_NPCSEV_CHANGE_FAMILYGUILD_NAME,	//90 帮派、家族改名
	GP_NPCSEV_TALISMAN_HOLEYLEVELU,		//法宝飞升
	GP_NPCSEV_TALISMAN_EMBEDSKILL,		//法宝技能镶嵌
	GP_NPCSEV_TALISMAN_SKILLREFINE,		//法宝技能融合
	GP_NPCSEV_TALISMAN_SKILLREFINE_RESULT,	//法宝技能融合是否接收	弃用，改为协议

	GP_NPCSEV_SHOP_REPUTATION,     //95  声望商店服务
	GP_NPCSEV_EQUIP_UPDATE2,		//装备升级为封神装, 废弃
	GP_NPCSEV_EQUIP_SLOT2,			//封神装备打孔
	GP_NPCSEV_ASTROLOGY_INSTALL,	//星座镶嵌
	GP_NPCSEV_ASTROLOGY_UNINSTALL,	//星座摘除

	GP_NPCSEV_ASTROLOGY_IDENTIFY,	//100星座鉴定, 废弃
	GP_NPCSEV_ASTROLOGY_UPGRADE,	//星座升级, 废弃
	GP_NPCSEV_ASTROLOGY_DESTORY,	//星座粉碎, 废弃
	GP_NPCSEV_ACTIVE_UI_TRANSFER,   //传送阵激活
	GP_NPCSEV_USE_UI_TRANSER,		//使用传送阵传送

	GP_NPCSEV_TRANSFER_SERVE = 105,		// 传送服务
	GP_NPCSEV_TRANSFER_SERVE_BATTLE_LEAVE,	// 从战场离开服务

	GP_NPCSEV_KINGDOM_ENTER = 107, //进入国战
	GP_NPCSEV_PRODUCE_JINFASHEN,	// 金法身快速制作
	GP_NPCSEV_PET_FLY,				// 宠物飞升
};

//	Player and NPC state
enum
{
    GP_STATE_SHAPE				= 0x00000001,
	GP_STATE_EMOTE				= 0x00000002,
	GP_STATE_INVADER			= 0x00000004,
	GP_STATE_FLY				= 0x00000008,
	GP_STATE_FAMILY				= 0x00000010,
    GP_STATE_SITDOWN			= 0x00000020,
    GP_STATE_EXTEND_PROPERTY	= 0x00000040,
    GP_STATE_CORPSE				= 0x00000080,

	//	Used only by player
	GP_STATE_TEAM				= 0x00000100,
	GP_STATE_TEAMLEADER			= 0x00000200,
	GP_STATE_ADV_MODE			= 0x00000400,
	GP_STATE_FACTION			= 0x00000800,
	GP_STATE_BOOTH				= 0x00001000,
	GP_STATE_FASHION			= 0x00002000,
	GP_STATE_GMFLAG				= 0x00004000,
	GP_STATE_PVP_ENABLE			= 0x00008000,
	GP_STATE_EFFECT				= 0x00010000,
	GP_STATE_INPVPCOMBAT		= 0x00020000,
	GP_STATE_IN_DUEL			= 0x00040000,	//	是否正在决斗中
	GP_STATE_IN_MOUNT			= 0x00080000,	//	正在骑乘中
	GP_STATE_IN_BIND			= 0x00100000,	//	和别人绑在一起
	GP_STATE_BC_INVADER			= 0x00200000,	//	Battle camp: invader
	GP_STATE_BC_DEFENDER		= 0x00400000,	//	Battle camp: defender
	GP_STATE_TITLE				= 0x00800000,	//	后面跟一个当前称谓id, short类型
	GP_STATE_SPOUSE				= 0x01000000,
	GP_STATE_SECT_MASTER_ID		= 0x08000000,
	GP_STATE_IN_SJBATTLE		= 0x10000000,	//  在宋金战场中
	GP_STATE_IN_VIPSTATE		= 0x20000000,	//	使用了vip卡片
	GP_STATE_IS_OBSERVER		= 0x40000000,	//  跨服pk的观战者模式
	GP_STATE_TRANSFIGURE		= 0x80000000,	//  变身状态
	
	//	Used only by NPC
	GP_STATE_NPC_DELAYDEAD		= 0x00000008,
	GP_STATE_NPC_ADDON1			= 0x00000100,
	GP_STATE_NPC_ADDON2			= 0x00000200,
	GP_STATE_NPC_ADDON3			= 0x00000400,
	GP_STATE_NPC_ADDON4			= 0x00000800,
	GP_STATE_NPC_ALLADDON		= 0x00000F00,
	GP_STATE_NPC_PET			= 0x00001000,	//	Pet flag
	GP_STATE_NPC_NAME			= 0x00002000,
	GP_STATE_NPC_SUMMON			= 0x00004000,	//  属于召唤怪
	GP_STATE_NPC_OWNER			= 0x00008000,	//  有归属的怪物
	GP_STATE_NPC_CLONE			= 0x00010000,	//  NPC是分身，客户端暂时不用这个状态
	GP_STATE_NPC_DIM			= 0x00020000,	//  NPC朦胧
	GP_STATE_NPC_FROZEN			= 0x00040000,	//	NPC冰冻状态
	GP_STATE_NPC_DIR_VISIBLE	= 0x00080000,	//	十字方向技能效果状态
	GP_STATE_NPC_INVISBLE       = 0x00100000,   //  NPC隐身
	GP_STATE_NPC_TELEPORT1      = 0x00200000,   //  传送类NPC（祝福）
	GP_STATE_NPC_TELEPORT2      = 0x00400000,   //  传送类NPC（诅咒）
	GP_STATE_NPC_PHASE			= 0x00800000,	//  相位标记NPC
};

enum
{
	GP_STATE_MATTER_OWNER		= 0x00000004,	// 有归属的矿物
	GP_STATE_MATTER_PHASE       = 0x00000008,   // 有相位标记的MATTER
	GP_STATE_COMBINE_MINE       = 0x00000020,   // 复合矿
};

enum
{
	GP_EXTRA_STATE_CARRIER			= 0x00000001,	//  玩家在载体上面
	GP_EXTRA_STATE_ANONYMOUS		= 0x00000002,	//  玩家处于匿名地图
	GP_EXTRA_STATE_INVISIBLE		= 0x00000004,	//  玩家隐身（自己看自己是半透明的，如果别人能看见自己的话，别人看自己也是半透明的）
	GP_EXTRA_STATE_DIM				= 0x00000008,	//  进入朦胧（效果是半透明，自己看或者别人看效果都是半透明）
	GP_EXTRA_STATE_CLONE			= 0x00000010,	//  存在一个分身
	GP_EXTRA_STATE_CIRCLE			= 0x00000020,	//  圈子信息
	GP_EXTRA_STATE_CROSSZONE		= 0x00000040,	//  大跨服
	GP_EXTRA_STATE_IGNITE			= 0x00000080,	//  点燃状态
	GP_EXTRA_STATE_FROZEN			= 0x00000100,   //  冰封状态
	GP_EXTRA_STATE_FURY				= 0x00000200,	//  神圣之怒状态
	GP_EXTRA_STATE_DARK				= 0x00000400,	//  黑暗之术
	GP_EXTRA_STATE_COLDINJURE		= 0x00000800,	//  冻伤状态
	GP_EXTRA_STATE_CIRCLEOFDOOM		= 0x00001000,	//	阵法状态，只有在阵眼（阵法发起者）身上才有 Added 2011-06-22.
	GP_EXTRA_STATE_BE_DRAGGED		= 0x00010000,	//	被牵引状态 //Added 2011-08-26.上面几个不连续的分别是：逆转正面，逆转负面，嗜血状态
	GP_EXTRA_STATE_BE_PULLED		= 0x00020000,	//	被拉扯状态 // Added 2011-08-26.
	GP_EXTRA_STATE_VIP_AWARD		= 0x00040000,	//	VIP奖励状态 Added 2012-01-16.
	GP_EXTRA_STATE_COLLISION_RAID	= 0x00080000,	//	处于碰撞副本时的状态	Added 2012-08-01.
	GP_EXTRA_STATE_XY_DARK_LIGHT	= 0x00100000,	//	轩辕光暗状态	Added 2012-08-16.
	GP_EXTRA_STATE_XY_SPIRITS		= 0x00200000,	//	轩辕灵体
	GP_EXTRA_STATE_MIRROR_IMAGE		= 0x00400000,	//	镜像
	GP_EXTRA_STATE_CHANGE_WING_COLOR= 0x00800000,	//	飞剑颜色变化 Added 2012-08-20.
	GP_EXTRA_STATE_DIR_VISIBLE_STATE= 0x01000000,	//	十字方向技能效果状态
	GP_EXTRA_STATE_EXTRA_EQUIP_EFFECT = 0X02000000,	//	装备额外光效
	GP_EXTRA_STATE_FLOW_BATTLE = 0X04000000,		//	流水席状态
	GP_EXTRA_STATE_PUPPET			= 0x08000000,   //  傀儡姿态
	GP_EXTRA_STATE_INTERACT			= 0x20000000,	//	互动状态
	GP_EXTRA_STATE_ACTIVE_EMOTE		= 0x40000000,	//	互动表情
	GP_EXTRA_STATE_TRAVEL			= 0x80000000,	//	自由飞行
};
//	Chat channel
enum
{
	//这个枚举定义如果改动的话，要通知所有人，包括服务器程序员
	//如非必要，尽可能在后面添加，避免删除和更改前面channel的顺序
	GP_CHAT_LOCAL = 0,
	GP_CHAT_FARCRY,
	GP_CHAT_TEAM,
	GP_CHAT_FACTION,
	GP_CHAT_WHISPER,
	GP_CHAT_DAMAGE,
	GP_CHAT_FIGHT,
	GP_CHAT_TRADE,
	GP_CHAT_SYSTEM,
	GP_CHAT_BROADCAST,
	GP_CHAT_MISC,
	GP_CHAT_FAMILY,
	GP_CHAT_CIRCLE,
	GP_CHAT_ZONE,
	GP_CHAT_SPEAKER,
	GP_CHAT_RUMOR,		// 匿名大喇叭捌档
	GP_CHAT_MAX,
};

//	The reason of droping item
enum
{
	GP_DROP_GM,
	GP_DROP_PLAYER,
	GP_DROP_TAKEOUT,
	GP_DROP_TASK,
	GP_DROP_RECHARGE,
	GP_DROP_DESTROY,
	GP_DROP_DEATH,
	GP_DROP_PRODUCE,
	GP_DROP_DECOMPOSE,
	GP_DROP_TRADEAWAY,
	GP_DROP_RESURRECT,
	GP_DROP_USED,
	GP_DROP_CONSUMERUNE,
	GP_DROP_EXPIRED,
	GP_DROP_BIND_DISAPPEAR,
	GP_DROP_DAMAGED,
};

//	Cool time index
enum
{
	GP_CT_NULL = 0,					//	空，保留
	GP_CT_EMOTE,                   	//	做表情的冷却时间 
	GP_CT_DROP_MONEY,              	//	往地上扔钱的冷却时间
	GP_CT_DROP_ITEM,               	//	往地上扔物品的冷却时间
	GP_CT_SWITCH_FASHION,          	//	切换时装模式的冷却时间
	// 5
	GP_CT_TRANSMIT_ROLL,			//	传送卷轴
	GP_CT_VIEWOTHEREQUIP,			//	查看别人的装备
	CP_CT_FIREWORKS,				//	施放烟花
	CP_CT_FARCRY,					//	far cry 频道说话
	CP_CT_SKILLMATTER,				//  技能物品
	// 10
	CP_CT_DOUBLEEXPMATTER,			//  双倍经验道具
	GP_CT_SKILLMATTER1,				//	技能物品冷却时间类型1
	GP_CT_SKILLMATTER2,				//	技能物品冷却时间类型2
	GP_CT_SKILLMATTER3,				//	技能物品冷却时间类型3
	GP_CT_RESERVED,					//	保留
	// 15
	GP_CT_VEHICLE,					//	召唤交通工具
	GP_CT_COUPLE_JUMPTO,			//	夫妻同心结
	GP_CT_LOTTERY,					//	彩票
	GP_CT_CAMRECORDER,				//	录像道具   
	GP_CT_CAMRECORDCHECK,			//	录像验证			
	// 20
	GP_CT_TEAM_RELATION,
	GP_CT_CD_ERASER,
	GP_CT_PET_COMBINE1,				//	宠物通灵			
	GP_CT_PET_COMBINE2,				//	宠物御宝			
	GP_CT_PET_PICKUP,				//	拾取
	// 25
	GP_CT_SET_FASHION_MASK,			//	
	GP_CT_GET_BATTLEINFO,			//	获取战场信息的冷却
	GP_CT_HP,						//  瞬回hp药品冷却时间
	GP_CT_MP,
	GP_CT_HP_MP,
	// 30
	GP_CT_SLOW_HP,					//  持续回hp药品冷却时间
	GP_CT_SLOW_MP,
	GP_CT_SLOW_HP_MP,
    GP_CT_QUERY_OTHER_ACHIEVMENT,	//  查看其他玩家成就冷却
    GP_CT_TRANSFIGURE,				//  变身冷却
	// 35
	GP_CT_NEWBIE_CHAT,				//  30级以下普通频道说话冷却   
	GP_CT_CHANGE_DS,				//  大跨服冷却
 	GP_CT_DP,						//  瞬回dp药品冷却时间
	GP_CT_SLOW_DP,					//	持续回dp药品冷却时间
	GP_CT_GET_MALL_PRICE,			//  取商城销售数据
	// 40
	GP_CT_GET_VIP_AWARD_INFO,		//	获取VIP奖励列表信息冷却时间 Added 2012-01-04.
	COOLDOWN_INDEX_RESET_SKILL_PROP,		// 90级以下技能重置冷却
	COOLDOWN_INDEX_RESET_SKILL_PROP_TALENT,	// 90级以下天书重置冷却
	COOLDOWN_INDEX_BATTLE_FLAG,				// 战旗使用冷却
	COOLDOWN_INDEX_PLAYER_FIRST_EXIT_REASON,	// 玩家首次退出游戏冷却
	// 45
	COOLDOWN_INDEX_BOTTLE_HP,
	COOLDOWN_INDEX_BOTTLE_MP,
	COOLDOWN_INDEX_BOTTLE_DP,
	GP_CT_SKILLMATTER4,
	GP_CT_SKILLMATTER5,
	//50
	COOLDOWN_INDEX_BATH,
	COOLDOWN_INDEX_KING_CALL_GUARD,
	COOLDOWN_INDEX_ARRANGE_INVENTORY,
	COOLDOWN_INDEX_ARRANGE_TRASHBOX,
	COOLDOWN_INDEX_TELEPORT1,      //祝福传送

	//55
	COOLDOWN_INDEX_TELEPORT2,		//诅咒传送
	COOLDOWN_INDEX_CHANGE_PUPPET_FIGHT_FORM,//傀儡战斗状态转换

	GP_CT_MAX,
	GP_CT_PRODUCE_START = 600,
	GP_CT_PRODUCE_END = 800,
	
	GP_CT_COMBINE_EDIT_START	= 900,
	GP_CT_COMBINE_EDIT_END		= 950,

	GP_CT_SKILL_START = 1024,
};

//	PVP mask
enum
{
	GP_PVPMASK_WHITE	= 0x0001,
	GP_PVPMASK_RED		= 0x0002,
	GP_PVPMASK_BLUE		= 0x0004,
	GP_PVPMASK_MAFIA	= 0x0008,
	GP_PVPMASK_ZONE		= 0x0010,	// 跨服后攻击同一个服务器的玩家
	GP_PVPMASK_ALL		= 0x001F,
};

enum
{
	GP_PET_SKILL_NUM	= 16
};

//	Pet type
enum
{   
	GP_PET_CLASS_INVALID = -1,
	GP_PET_CLASS_MOUNT = 0, //	骑宠
	GP_PET_CLASS_COMBAT,    //	战斗宠物
	GP_PET_CLASS_FOLLOW,    //	跟随宠物
	GP_PET_CLASS_MAX,
};

//	Player camp in battle
enum
{
	GP_BATTLE_CAMP_NONE = 0,
	GP_BATTLE_CAMP_INVADER,
	GP_BATTLE_CAMP_DEFENDER,
	GP_BATTLE_CAMP_OBSERVER,	// 观察者
	GP_BATTLE_CAMP_ARENA,		// 竞技场中无阵营，将阵营统一为4
};

// gt talk type
enum RouteType
{
	FROM_GT=0,
	FROM_GAME,
	TO_GT,
	TO_GAME,
    TO_GT_AND_GAME,
};

enum
{
    BATTLE_TYPE_NORMAL         = 0, //普通战场
    BATTLE_TYPE_NAMED_CRSSVR   = 1, //非匿名的跨服战场
	BATTLE_TYPE_INSTANCING	   = 2, //剧情战场	
    BATTLE_TYPE_REBORN         = 3, //飞升战场
    BATTLE_TYPE_SONJIN         = 4, //宋金战场
    BATTLE_TYPE_SONJINREBORN   = 5, //宋金飞升战场
	BATTLE_TYPE_ARENA		   = 6, //竞技场
	BATTLE_TYPE_ARENAREBORN	   = 7, //飞升竞技场
	BATTLE_TYPE_CRSSVR		   = 8,	//匿名跨服战场
	BATTLE_TYPE_CHALLENGE	   = 9, //挑战战场
	BATTLE_TYPE_CRSSVRTEAM	   = 10,//跨服小队pk战场 Added 2011-05-19
	BATTLE_TYPE_FLPW_CRSSVR   = 11,	//跨服流水席		
};
enum
{
    BATH_TYPE_NORMAL = 0, //泡澡地图 2013-04-19
};
enum COLLISION_SKILL_TYPE				//碰撞技能类型
{
	COLLISION_SKILL_MAXSPEED	= 0,	//加速技能
	COLLISION_SKILL_HALFSPEED,			//减速技能
	COLLISION_SKILL_ASHILL,				//不动如山技能
	COLLISION_SKILL_ROOT,				//定身技能
	COLLISION_SKILL_CHAOS,				//混乱技能
	COLLISION_SKILL_VOID,				//虚无技能
};

enum									//碰撞副本中技能后状态
{
	COLLISION_STATE_ASHILL		= 0x00000001,	//不动如山
	COLLISION_STATE_ROOT		= 0x00000002,	//定身状态
	COLLISION_STATE_CHAOS		= 0x00000004,	//混乱状态
	COLLISION_STATE_VOID		= 0x00000008,	//虚无状态
};

enum
{
	MAX_WING_COLOR_COUNT		= 6,			//飞剑变色的最大种类数目
	COLLISION_MAX_SPEED			= 15,			//碰撞副本中最大移动速度
	COLLISION_PLAYER_MAX_MASS	= 1000000,		//碰撞副本中最大质量，用于不动如山技能
	MAX_MULTILINE_SKILL_GFX_NUM	= 6,			//多线性技能攻击时，最大线条数
};

enum RAID_FACTION
{
     RF_NONE,                         //无阵营
     RF_ATTACKER,                     //攻击方
     RF_DEFENDER,                     //防守方
	 RF_VISITOR,                      //观察者
};
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
