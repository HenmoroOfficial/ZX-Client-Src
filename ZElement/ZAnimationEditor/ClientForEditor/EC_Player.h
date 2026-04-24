/*
 * FILE: EC_Player.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_MsgDef.h"
#include "EC_StringTab.h"
#include "EC_Object.h"
#include "EC_NetDef.h"
#include "EC_RoleTypes.h"
#include "EC_Counter.h"
#include "EC_IvtrTypes.h"
//#include "EC_ChangePill.h"

#include "AAssist.h"
#include "AArray.h"
#include "AList2.h"
#include "A3DGeometry.h"

#include "ExpTypes.h"
#include "hashmap.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECPlayerMan;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif
class CECModel;
class CECTeam;
class CECIconDecal;
class CECSkill;
class CECPateText;
class CECSprite;
class CECSpriteDecal;
class CECNPC;
class A3DViewport;
class A3DGFXEx;
class A3DSkin;
class A3DShader;
class A3DSkinModel;
class CECBubbleDecalList;
class CECIvtrItem;
class CECAchievementMan;

struct EC_PLAYERLOADRESULT;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPlayer
//	
///////////////////////////////////////////////////////////////////////////

enum ProfRace
{
	PROFRACE_UNKNOWN	= -1,
	PROFRACE_SHAOXIA	= 0,
	PROFRACE_GUIWANG	= 1,
	PROFRACE_HEHUAN		= 2,
	PROFRACE_QINGYUN	= 3,
	PROFRACE_TIANYIN	= 4,
	PROFRACE_GUIDAO		= 5,
	PROFRACE_FENGXIANG	= 6,
	PROFRACE_JIULI		= 7,
	PROFRACE_LIESHAN	= 8,
	PROFRACE_HUAIGUANG	= 9,
	PROFRACE_TIANHUA	= 10,
	PROFRACE_DONGYI		= 11,
	PROFRACE_TAIHAO		= 12,
	PROFRACE_HUMAN		= 100,
	PROFRACE_DEITY		= 200,
};

// equipment location
enum
{
	enumSkinShowNone = 0,			// 不显示
	enumSkinShowHead,				// 头部
	enumSkinShowUpperBody,			// 上衣
	enumSkinShowLowerBody,			// 下衣
	enumSkinShowUpperAndLower,		// 上衣带下衣
	enumSkinShowFoot,				// 鞋子
	enumSkinShowGlasses,			// 眼镜
	enumSkinShowNose,				// 鼻子
	enumSkinShowMustache,			// 胡子
	enumSkinShowCloak,				// 背饰
	enumSkinShowWing,				// 翅膀
	enumNumSkinShow
};

//	Skin index
enum
{
	SKIN_HEAD_INDEX = 0,			// 头
	SKIN_HAIR_INDEX,				// 头发
	SKIN_BODY_INDEX,				// 普通衣服
	SKIN_FOOT_INDEX,				// 普通鞋子
	SKIN_HELM_INDEX,				// 普通头盔
	SKIN_GLASSES_INDEX,				// 眼镜
	SKIN_NOSE_INDEX,				// 鼻子
	SKIN_MUSTACHE_INDEX,			// 胡子
	SKIN_CLOAK_INDEX,				// 披风
	SKIN_FASHION_UPPER_BODY_INDEX,	// 时装上衣
	SKIN_FASHION_LOWER_INDEX,		// 时装裤子
	SKIN_FASHION_FOOT_INDEX,		// 时装鞋子
	SKIN_FASHION_HEADWEAR_INDEX,	// 时装头饰
	SKIN_EAR_INDEX,					// 耳朵  烈山 妖族专用
	SKIN_TAIL_INDEX,				// 尾巴  烈山 妖族专用
	NUM_SKIN_INDEX,
};

// Portrait skin index
enum
{
	PORTRAIT_SKIN_HEAD = 0,
	PORTRAIT_SKIN_HAIR,
	PORTRAIT_SKIN_HELM,
	PORTRAIT_SKIN_HEADWEAR,
	PORTRAIT_SKIN_GLASSES,
	PORTRAIT_SKIN_NOSE,
	PORTRAIT_SIN_MUSTACHE,
	NUM_PORTAIT_SKIN
};

//	Action channel index
enum
{
	ACT_CHANNEL_BODY	= 1,
	ACT_CHANNEL_EYE		= 2,
	ACT_CHANNEL_WOUND	= 3
};

enum
{
	// multi ride capacity
	MULTI_RIDE_CAPACITY = 6,
};

// weapon type
enum
{
	WEAPON_NULL		= 0,	// 空
	WEAPON_SWORD	= 1,	// 剑
	WEAPON_BROADSWORD =2,	// 刀
	WEAPON_WHEAL	=3,		// 轮
	WEAPON_STAFF	=4,		// 杖
	WEAPON_CLAW		=5,		// 爪
	WEAPON_RULER	=6,		// 尺
	WEAPON_AXE		=7,		// 斧
	WEAPON_BOW		=8,		// 弓
	WEAPON_BOOK		=9,		// 书
	WEAPON_SICKLE	=10,	// 镰
	WEAPON_PEARL	=11,	// 珠
	WEAPON_LYRA		=12,	// 琴
	NUM_WEAPON,
};

//Added 2011-06-23.
#ifndef IS_EQUAL
#define IS_EQUAL(x,y) ((x-y) < 0.0001f)
#endif
//Added end.

class CECPlayer : public CECObject
{
public:		//	Types

	struct EquipsLoadResult
	{
		A3DSkin*		aSkins[NUM_SKIN_INDEX][3];
		int				aEuips[SIZE_EQUIPIVTR];
		DWORD			dwSkinChangeMask;
		DWORD			dwShowMask;
		DWORD			dwFashionShowMask;
		bool			bWeaponChanged;
		CECModel*		pLeftHandWeapon;
		CECModel*		pRightHandWeapon;
		CECModel*		pBackWeapon;	//背后新增武器 Added 2011-08-09.
		bool			bHookChanged;	//很shit的搞法。之前左右手的挂点很混乱，焚香和太昊的左手挂点不再叫HH_righthandweapon而是：HH_lefthandweapon1，因此通过此标志来区别处理
		unsigned int	uAttackType;
		bool			bTalismanChanged;
		CECModel*		pTalisman;
		int				talisman_quality;
		bool			bWingChanged;
		int				nFlyMode;
		CECModel*		pWing;
		bool			bTuneChanged;
		CECModel*		pTune;
		BYTE			stoneWeapon;
		BYTE			stoneHead;
		BYTE			stoneBody;
		BYTE			stoneFoot;
	};

	//	Player action type
	enum
	{
		ACTTYPE_SH = 0,	//	Single hand weapon
		ACTTYPE_FH,		//	Free hands
		ACTTYPE_BHF,	//	Both hands far range weapon
		ACTTYPE_BHN,	//	Both hands near range weapon
		NUM_ACTTYPE,
	};

	//	Player action index
	enum PLAYER_ACTION_TYPE
	{
		// 0
		ACT_STAND = 0,				// 站立
		ACT_FIGHTSTAND,				// 战斗站立
		ACT_WALK,					// 行走
		ACT_RUN,					// 奔跑
		ACT_JUMP_START,				// 起跳

		// 5
		ACT_JUMP_LOOP,				// 起跳空中循环
		ACT_JUMP_LAND,				// 跳跃落地
		ACT_SWIM,					// 游动
		ACT_HANGINWATER,			// 水中漂浮
		ACT_TAKEOFF_WING,			// 翅膀起飞 should be ACT_TAKEOFF_WING

		// 10
		ACT_HANGINAIR_WING,			// 翅膀悬停 should be ACT_HANGINAIR_WING
		ACT_FLY_WING,				// 翅膀前进 should be ACT_FLY_WING
		ACT_FLYDOWN_WING_HIGH,		// 翅膀高空下降 should be ACT_FLYDOWN_WING_HIGH
		ACT_FLYDOWN_WING_LOW,		// 翅膀低空下降 should be ACT_FLYDOWN_WING_LOW
		ACT_LANDON,					// 翅膀落地 should be ACT_LAND_WING

		// 15
		ACT_TAKEOFF_SWORD,			// 飞剑起飞
		ACT_HANGINAIR_SWORD,		// 飞剑悬停
		ACT_FLY_SWORD,				// 飞剑前进
		ACT_FLYDOWN_SWORD_HIGH,		// 飞剑高空下降
		ACT_FLYDOWN_SWORD_LOW,		// 飞剑低空下降

		// 20
		ACT_LANDON_SWORD, 			// 飞剑落地 
		ACT_SITDOWN,				// 打坐
		ACT_SITDOWN_LOOP,			// 打坐循环
		ACT_STANDUP,				// 打坐站起
		ACT_WOUNDED,				// 受伤

		// 25
		ACT_GROUNDDIE,				// 陆地死亡
		ACT_GROUNDDIE_LOOP,			// 死亡地面循环
		ACT_WATERDIE,				// 水中死亡
		ACT_WATERDIE_LOOP,			// 死亡水中循环
		ACT_AIRDIE_ST,				// 空中死亡

		// 30
		ACT_AIRDIE,					// 空中死亡下落循环
		ACT_AIRDIE_ED,				// 空中死亡落地
		ACT_AIRDIE_LAND_LOOP,		// 死亡落地循环
		ACT_REVIVE,					// 复活
		ACT_CUSTOMIZE,				// 长休闲动作

		// 35
		ACT_STRIKEBACK,				// 被击退
		ACT_STRIKEDOWN,				// 被击倒
		ACT_STRIKEDOWN_LOOP,		// 被击倒倒地循环
		ACT_STRIKEDOWN_STANDUP,		// 被击倒站起
		ACT_PICKUP,					// 采摘

		// 40
		ACT_PICKUP_LOOP,			// 采摘植物循环
		ACT_PICKUP_STANDUP,			// 采摘站起
		ACT_PICKUP_MATTER,			// 捡东西
		ACT_GAPE,					// 伸懒腰
		ACT_LOOKAROUND,				// 四处张望

		// 45
		ACT_PLAYWEAPON,				// 转动兵器
		ACT_EXP_WAVE,				// 招手
		ACT_EXP_NOD,				// 点头
		ACT_EXP_SHAKEHEAD,			// 摇头
		ACT_EXP_SHRUG,				// 耸肩膀

		// 50
		ACT_EXP_LAUGH,				// 大笑
		ACT_EXP_ANGRY,				// 生气
		ACT_EXP_STUN,				// 晕倒
		ACT_EXP_DEPRESSED,			// 沮丧
		ACT_EXP_KISSHAND,			// 飞吻

		// 55
		ACT_EXP_SHY,				// 害羞
		ACT_EXP_SALUTE,				// 抱拳
		ACT_EXP_SITDOWN,			// 坐下
		ACT_EXP_SITDOWN_LOOP,		// 坐下循环
		ACT_EXP_SITDOWN_STANDUP,	// 坐下站起

		// 60
		ACT_EXP_ASSAULT,			// 冲锋
		ACT_EXP_THINK,				// 思考
		ACT_EXP_DEFIANCE,			// 挑衅
		ACT_EXP_VICTORY,			// 胜利
		ACT_EXP_KISS,				// 亲吻

		// 65
		ACT_EXP_KISS_LOOP,			// 亲吻循环
		ACT_EXP_KISS_END,			// 亲吻结束
		ACT_ATTACK_1,				// 普攻1
		ACT_ATTACK_2,				// 普攻2
		ACT_ATTACK_3,				// 普攻3

		// 70
		ACT_ATTACK_4,				// 普攻4
		ACT_ATTACK_TOSS,			// 放暗器
		ACT_TRICK_RUN,				// 跑动中的花招
		ACT_TRICK_JUMP,				// 跳跃中的花招
		ACT_FLY_GLIDE,				// 翅膀滑翔

		// 75
		ACT_FLY_GLIDE_SWORD,		// 飞剑滑翔
		ACT_EXP_FIGHT,				// 战斗
		ACT_EXP_ATTACK1,			// 攻击1
		ACT_EXP_ATTACK2,			// 攻击2
		ACT_EXP_ATTACK3,			// 攻击3

		// 80
		ACT_EXP_ATTACK4,			// 攻击4
		ACT_EXP_DEFENCE,			// 防御
		ACT_EXP_FALL,				// 摔倒
		ACT_EXP_FALLONGROUND,		// 倒地
		ACT_EXP_LOOKAROUND,			// 张望

		// 85
		ACT_EXP_DANCE,				// 舞蹈
		ACT_USEITEM,				// 通用的使用物品动作
		ACT_USEITMELOOP,			// 通用的使用物品循环动作
		ACT_TWO_KISS,				// 双人亲吻
		ACT_BLINK,					// 眨眼

		// 90
		ACT_GENERAL_OPRT,
		ACT_QINGGONG_START,			// 轻功起
		ACT_QINGGONG_LOOP,			// 轻功循环
		ACT_FIGHT_JUMP_START,		// 起跳
		ACT_FIGHT_JUMP_LOOP,		// 起跳空中循环

		// 95
		ACT_FIGHT_JUMP_LAND,		// 跳跃落地
		ACT_FIGHT_RUN,				// 战斗奔跑
		ACT_FIGHT_QINGGONG_LOOP,	// 战斗轻功循环
		ACT_LOGIN_POSE,
		ACT_BACKUP,

		// 100
		// these are the action suffix, always at the end
		ACT_SUFFIX_0,				// 第0种后缀
		ACT_SUFFIX_1,				// 第1种后缀
		ACT_SUFFIX_2,				// 第2种后缀
		ACT_SUFFIX_3,				// 第3种后缀
		ACT_SUFFIX_4,				// 第4种后缀
		
		// 105
		ACT_SUFFIX_5,				// 第5种后缀
		ACT_SUFFIX_6,				// 第6种后缀
		ACT_SUFFIX_7,				// 第7种后缀
		ACT_SUFFIX_8,				// 第8种后缀
		ACT_SUFFIX_9,				// 第9种后缀 
		
		// 110
		ACT_SUFFIX_10,				// 第10种后缀 
		ACT_SUFFIX_11,				// 第11种后缀 
		ACT_SUFFIX_12,				// 第12种后缀 
		ACT_SUFFIX_13,				// 第13种后缀 备用
		ACT_SUFFIX_14,				// 第14种后缀 备用

		// 飞骑
		ACT_TAKEOFF_FEIQI,			// 飞骑起飞
		ACT_HANGINAIR_FEIQI,		// 飞骑悬停
		ACT_FLY_FEIQI,				// 飞骑前进
		//ACT_FLY_GLIDE_FEIQI,		// 飞骑滑翔   滑翔动作与飞行动作已经组合
		ACT_FLYDOWN_FEIQI_HIGH,		// 飞骑高空下降
		ACT_FLYDOWN_FEIQI_LOW,		// 飞骑低空下降

		// 120
		ACT_STAND_SHOW,				// 登陆站立
		ACT_DEBUT_SHOW,				// 登陆亮相
		ACT_STAND_WEAK,				// 站立_虚弱
		ACT_FIGHTSTAND_WEAK,		// 战斗站立_虚弱
		ACT_RUN_WEAK,				// 奔跑_虚弱
		
		// 125
		ACT_FIGHT_RUN_WEAK,			// 战斗奔跑_虚弱
		ACT_WALK_WEAK,				// 行走_虚弱
		ACT_LOGIN_SITDOWN,			// 打坐界面用
		ACT_LOGIN_STANDUP,			// 打坐站起界面用
		ACT_LOGIN_SITDOWN_LOOP,		// 打坐循环界面用
		
		// 130
		ACT_FLYUP_WING,				// 翅膀上升
		ACT_FIGHT_FLY_WING,			// 战斗翅膀飞行
		ACT_FIGHT_HANGINAIR_WING,		// 战斗翅膀悬停
		ACT_FIGHT_FLY_SWORD,		// 战斗飞剑飞行
		ACT_FIGHT_HANGINAIR_SWORD,		// 战斗飞剑悬停

		ACT_FIGHT_FLY_FEIQI,		// 战斗飞骑飞行
		ACT_FIGHT_HANGINAIR_FEIQI,		// 战斗飞骑悬停
		ACT_WALK_SNEAK,				// 行走潜行 
		ACT_FIGHT_WALK_SNEAK,		// 战斗行走潜行
		ACT_FIGHTSTAND_SNEAK,		// 战斗站立潜行	
	
		// 140
		ACT_STAND_SNEAK,			// 站立潜行
		ACT_FIGHT_CHARGE,			// 战斗冲锋 Added 2011-08-19.
		ACT_FIGHT_PULL_LOOP,		// 战斗擒龙决循环动作 Added 2011-08-24.

		ACT_MAX,
		ACT_CASTSKILL,				// !! This is only a placeholder which represents skill actions
	};

	//	Move mode
	enum
	{
		MOVE_STAND = 0,
		MOVE_MOVE,		//	Normal move, walk, run, swim or fly
		MOVE_JUMP,
		MOVE_QINGGONG,
		MOVE_FREEFALL,
		MOVE_SLIDE,
	};

	//	Move environment
	enum
	{
		MOVEENV_GROUND = 0,	//	Move on ground
		MOVEENV_WATER,
		MOVEENV_AIR,
	};

	//	Player resources ready flag
	enum
	{
		RESFG_SKELETON	= 0x01,
		RESFG_SKIN		= 0x02,
		RESFG_ASSEMBLED	= 0x04,
		RESFG_ALL		= 0x0f,
	};

	//	Render Name Flag
	enum
	{
		RNF_NAME		= 0x01,
		RNF_TITLE		= 0x02,
		RNF_FACTION		= 0x04,
		RNF_SELL		= 0x08,
		RNF_BUY			= 0x10,
		RNF_WORDS		= 0x20,
		RNF_FAMILY		= 0x40,
		RNF_CIRCLE		= 0x80,
		RNF_SERVER		= 0x100,
		RNF_ALL			= 0x1ff,
	};

	//	Bubble text
	enum
	{
		BUBBLE_DAMAGE = 0,
		BUBBLE_SKILL_DAMAGE,
		BUBBLE_EXP,
		BUBBLE_SP,
		BUBBLE_REPUTATION,
		BUBBLE_REGION_REPU,
		BUBBLE_MONEY,
		BUBBLE_LEVELUP,
		BUBBLE_HITMISSED,
		BUBBLE_INVALIDHIT,
		BUBBLE_IMMUNE,
		BUBBLE_HPWARN,
		BUBBLE_MPWARN,
		BUBBLE_HEAL,
		BUBBLE_SKILL_SPEC,
		BUBBLE_RESIST,
		BUBBLE_DEITY_EXP,
		BUBBLE_DEITY_DAMAGE,
		BUBBLE_DPWARN,
		BUBBLE_DEITY_LEVELUP,
	};

	// Player Attach mode
	enum AttachMode
	{
		enumAttachNone,
		enumAttachRideOnPet,
		enumAttachRideOnPlayer,
		enumAttachHugPlayer,
	};

	//	Effect type
	enum
	{
		EFF_FACEPILL = 1,
	};

	//	Duel state
	enum
	{
		DUEL_ST_NONE = 0,
		DUEL_ST_PREPARE,
		DUEL_ST_INDUEL,
		DUEL_ST_STOPPING,
	};
	
	enum 
	{
		FLYMODE_SWORD = 0,
		FLYMODE_WING,
		FLYMODE_FEIQI,
	};
	//  VIP state
	enum
	{
		NO_VIP_STATE = 0,
		SILVER_CARD,	// 白银卡
		GOLD_CARD,		// 黄金卡
		PLATINUM_CARD,	// 白金卡
	};

	enum 
	{
		SPECIAL_STATE_IGNITE	= 0x00000001,	// 点燃状态
		SPECIAL_STATE_FROZEN	= 0x00000002,	// 冰封状态
		SPECIAL_STATE_FURY		= 0x00000004,	// 神圣之怒状态
		SPECIAL_STATE_DARK		= 0x00000008,	// 黑暗状态
	};	
	//	GM flags
	enum
	{
		GMF_IAMGM		= 0x0001,	//	I'm GM
		GMF_INVISIBLE	= 0x0002,
		GMF_INVINCIBLE	= 0x0004,
	};
	
	//	Player information got from server
	struct INFO
	{
		int		cid;	//	Character ID
		int		crc_e;	//	Equipment data crc
	};

	//	Team requirment
	struct TEAMREQ
	{
		bool	bShowReq;		//	true, show team requirment
		bool	bShowLevel;		//	true, show host's level and profession
		int		iType;			//	0: search team; 1: search member
		int		iLevel;			//	Player's level
		int		iProfession;	//	Player's profession
	};
	
	//	Player model resources
	struct MODELRES
	{
		CECModel*	pPlayerModel;
		CECModel*	pDummyModel;
		A3DSkin*	pBodySkin;
		AString		strHeadSkin;
	};

	//	Pate content render info
	struct PATECONTENT
	{
		int		iVisible;	//	Visible flag. 0, not set; 1, not visible; 2, visible
		int		iBaseX;		//	Base x
		int		iBaseY;		//	Base y
		int		iCurY;		//	Current y
		float	z;			//	z value
		float	rhw;
	};

	//	Delayed bubble text
	struct BUBBLETEXT
	{
		int		iIndex;
		DWORD	dwNum;
		DWORD	dwWaitTime;
		int		p1;
		DWORD	dwLatentTime;
		DWORD	dwTimeCnt;
	};
	
	//	PVP infomation
	struct PVPINFO
	{
		bool	bEnable;		//	PVP switch
		DWORD	dwCoolTime;
		DWORD	dwMaxCoolTime;
		unsigned char	ucFreePVPType;		//	Free PVP flag, ignore bEnable flag
		bool	bInPVPCombat;	//	true, in PVP combat
		int		iDuelState;		//	Duel state
		int		idDuelOpp;		//	Duel opponent
		int		iDuelTimeCnt;	//	Duel time counter
		int		iDuelRlt;		//	Duel result. 0, no defined; 1-win; 2-lose; 3-draw
	};
	
	//	Constants used in moving control
	struct MOVECONST
	{
		float	fStepHei;		//	Maximum step height
		float	fMinAirHei;		//	Minimum distance to terrain (or water) when fly
		float	fMinWaterHei;	//	Minimum distance to terrain when swim
		float	fShoreDepth;	//	Shore depth
		float	fWaterSurf;		//	Water surface depth
	};

	//	Riding pet info.
	struct RIDINGPET
	{
		int		id;
		int		iLevel;
		bool    bMultiRider;
		int		iMultiRideMode;	// 多人骑乘模式：0龙蛇模式，1车马模式
	};

	struct TRANSFIGUREINFO
	{
		int		id;
		bool	bReplaceName;
		bool	bIsFly;
		int		action_type;		// 动作方案
		int		stand_mode;			// 站立方式，四足，两足，翅膀，只有四足的需要使用地面法向
	};

	struct SECT_LEVEL2_INFO
	{
		unsigned int mask_sect;
		int level2;
	};

	//	Buff info
	struct BUFFINFO
	{
		int		id;			//	Buff id
		int		iLevel;		//	Buff level
		int		iEndTime;	//	End time
	};
	
	struct TANKINFO
	{
		int		nid;
		int		tid;
	};
	typedef abase::hash_map<int, SECT_LEVEL2_INFO> SectLevel2Map;
	typedef abase::hashtab<AWString*, int, abase::_hash_function> WStrTable;
	
	friend class CECElsePlayer;
public:		//	Constructor and Destructor

	CECPlayer(CECPlayerMan* pPlayerMan);
	virtual ~CECPlayer();

public:		//	Attributes

public:		//	Operations

	//	Release object
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0) { return true; }
	//	Render when player is opening booth
	virtual bool RenderForBooth(CECViewport* pViewport, int iRenderFlag=0) { return true; }

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	Set absolute position
	virtual void SetPos(const A3DVECTOR3& vPos);
	//	Set absolute forward and up direction
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);
	//	Player was killed
	virtual void Killed(int idKiller) {};

	//	Search the full suite
	virtual int SearchFullSuite() { return 0; }
	//	Get number of equipped items of specified suite
	virtual int GetEquippedSuiteItem(int idSuite, int* aItems=NULL) { return 0; }

	//	Set loaded model to player object, this function is used in multithread loading process
	virtual bool SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret);
	bool SetEquipsLoadedResult(EquipsLoadResult& ret, bool bUpdateAtOnce);
	virtual bool SetPetLoadResult(CECModel* pPetModel);
	virtual bool SetChangedModelLoadResult(CECModel* pChangedModel);
	virtual	bool SetTransfigureModelLoadResult(CECModel* pTransfiguredModel);
	virtual bool SetAircraftModelLoadResult(CECModel* pModel);
	//	Set player's transparence
	void SetTransparent(float fTrans);

	//	Get player information got from server
	const INFO& GetPlayerInfo() { return m_PlayerInfo; }
	//	Get basic properties
	ROLEBASICPROP& GetBasicProps() { return m_BasicProps; }
	//	Get extend properties
	ROLEEXTPROP& GetExtendProps() { return m_ExtProps; }
	//	Set part extend properties
	void SetPartExtendProps(int iPropIdx, void* pData);

	//	Get player name
	const ACHAR* GetName() { return m_strName; }
	//	Get player name color
	DWORD GetNameColor();
	//	Decide name color
	static DWORD DecideNameColor(int iPKLev);
	//	Get character ID
	int GetCharacterID() { return m_PlayerInfo.cid; }
	//	Get move mode
	int GetMoveMode() { return m_iMoveMode; }
	//	Set move mode
	void SetMoveMode(int iMode) { m_iMoveMode = iMode; }
	//	Get move environment
	int GetMoveEnv() { return m_iMoveEnv; }
	//	Set move environment
	void SetMoveEnv(int iEnv) { m_iMoveEnv = iEnv; }
	//	Get player's AABB
	const A3DAABB& GetPlayerAABB() { return m_aabb; }
	//test
	void ScalePlayerAABB( float fRatio ){ m_aabb.Extents *= fRatio; };
	//	Get player's pick AABB
	const A3DAABB& GetPlayerPickAABB();
	//	Get player's AABB from the skin model
	const A3DAABB & GetModelAABB();
	//	Get player's skin model object
	A3DSkinModel * GetA3DSkinModel();
	//	Get touch radius
	float GetTouchRadius() { return m_fTouchRad; }
	//	Set money amount
	void SetMoneyAmount(int iMoneyCnt) { m_iMoneyCnt = iMoneyCnt; }
	//	Get money amount
	int GetMoneyAmount() { return m_iMoneyCnt; }
	//	Add money amount
	int AddMoneyAmount(int iAmount);
	//	Get maximum money amount
	int GetMaxMoneyAmount() { return m_iMaxMoney; }
	//	Get reborn count
	int GetRebornCount() const { return m_nRebornCount; }
	void SetRebornCount(int n) { m_nRebornCount = n; }
	//	Get walk-run flag
	bool GetWalkRunFlag() { return m_bWalkRun; }
	//	Set walk-run flag
	void SetWalkRunFlag(bool bFlag) { m_bWalkRun = bFlag; }
	//	Get rush fly flag
	bool GetRushFlyFlag() { return m_bRushFly; }
	//	Set rush fly flag
	void SetRushFlyFlag(bool bRushFly) { m_bRushFly = bRushFly; }
	//	Set / Get name rendering flag
	void SetRenderNameFlag(bool bTrue) { m_bRenderName = bTrue; }
	bool GetRenderNameFlag() { return m_bRenderName; }
	//	Set / Get bars rendering flag
	void SetRenderBarFlag(bool bTrue) { m_bRenderBar = bTrue; }
	bool GetRenderBarFlag() { return m_bRenderBar; }
	//	Get profession
	int GetProfession() { return m_BasicProps.iProfession; }
	//	Set profession
	void SetProfession(int iProf) { m_BasicProps.iProfession = iProf; }	
	// 烈山、怀光id比较奇怪
	int FaceHairId2Index(int id);
	int FaceHairIndex2Id(int index);
	static int	FaceHairId2FileIndex(int id, int iProf);
	void CheckIndexBound(int& id);
	//	Get face id
	int GetFaceid()	{ return m_iFaceid; }
	//	Get hair id
	int GetHairid() { return m_iHairid; }
	//  Get ear id
	int GetEarid() { return m_iEarid & 0x7f; }
	//	Get tail id
	int GetTailid() { return m_iTailid & 0x7f; }
	//	Get default skin id
	int GetDefaultSkinid() { return m_iDefaultSkin; }
	//  是否是高级的耳朵
	bool IsAdvancedEar() { return m_iEarid & 0x80 ? true:false; }
	//  是否是高级的尾巴
	bool IsAdvancedTail() { return m_iTailid & 0x80 ? true:false; }
	//	Get gender
	int GetGender() { return m_iGender; }
	//  Get race
	int GetRace() { return m_iRace; }
	//  Get physique
	int GetPhysique() { return m_iPhysique; } 

	void UpdateRace();
//	CECSkill * GetCurSkill() { return m_pCurSkill; }
	//	Set last said words
	void SetLastSaidWords(const ACHAR* szWords, int nEmotionSet, CECIvtrItem* pItem);
	//	Get player's reputation
	int GetReputation() { return m_iReputation; }
	//	Set player's reputation
	void SetReputation(int iRep)
	{
		if (IsHostPlayer())
		{
			int diff = iRep - m_iReputation;
			m_iReputation = iRep;
			BubbleText(BUBBLE_REPUTATION, diff);
		}
		else
			m_iReputation = iRep;
	}
	int GetCultivation() const { return m_iCultivation; }
	void SetCultivation(int n) { m_iCultivation = n; }
	//	Get player's title
	int GetTitle() { return m_iTitle; }
	//	Set player's reputation
	void SetTitle(int iTitle);
	//	format the title string
	bool FormatTitle(ACString & title);
	//	Get spouse
	int GetSpouse() { return m_idSpouse; }
	//	Set spouse
	void SetSpouse(int idSpouse);
	int GetMaster() const { return m_idMaster; }
	void SetMaster(int id) { m_idMaster = id; }
	//	Get PVP information
	const PVPINFO& GetPVPInfo() { return m_pvp; }
	//	Is PVP open ?
	bool IsPVPOpen() { return m_pvp.bEnable; }
	//	Is player in duel ?
	bool IsInDuel() { return m_pvp.iDuelState == DUEL_ST_INDUEL; }
	//	Set duel result
	void SetDuelResult(int iResult) { m_pvp.iDuelRlt = iResult; }
	//	Is host player ?
	bool IsHostPlayer();
	//	Is in fashion mode ?
	bool InFashionMode() { return m_bFashionMode; }
	//	Set fashion mode
	void SetFashionMode(bool b) { m_bFashionMode = b; }
	//	Get faction ID
	int GetFactionID() { return m_idFaction; }
	//	Set faction ID
	void SetFactionID(int id);
	//	Get family ID
	int GetFamilyID() { return m_idFamily; }
	//	Set Family ID
	void SetFamilyID(int id);
	//	Get faction role
	int GetFRoleID() { return m_idFRole; }
	//	Set faction role
	void SetFRoleID(int role);
	//	On start binding buddy
	virtual void OnStartBinding(int idMule, int idRider);
	//	Get buddy state
	int GetBuddyState()
	{
		if (m_bHangerOn) return 2;
		else if (m_iBuddyId) return 1;
		else return 0;
	}
	//	Get buddy id
	int GetBuddyID() { return m_iBuddyId; }
	
	//	Get battle camp this player belongs to
	int GetBattleCamp() { return m_iBattleCamp; }
	//	Set battle camp this player belongs to
	void SetBattleCamp(int iCamp) { m_iBattleCamp = iCamp; }
	//	Check whether player in a same battle camp
	bool InSameBattleCamp(CECPlayer* pPlayer);
	bool InSameBattleCamp(CECNPC* pNPC);
	//	Is player in battle
	bool IsInBattle() { return m_iBattleCamp != GP_BATTLE_CAMP_NONE; }
	//	Change player's tank leader state
	void ChangeTankLeader(int idTank, int tidTank, bool bBecomeLeader);
	//	Update player's tank leader state
	void UpdateTankLeader();
	//	Get number of player controlled tank
	int GetBattleTankNum() { return m_aBattleTanks.GetSize(); }
	//	Get tank id controlled by player
	int GetBattleTank(int n) { return m_aBattleTanks[n].nid; }

	//	Get booth state
	int GetBoothState() { return m_iBoothState; }
	//	Set booth state
	void SetBoothState(int iState) { m_iBoothState = iState; }
	//	Set booth name
	void SetBoothName(const ACHAR* szName);
	//	Get booth name
	const ACHAR* GetBoothName() { return m_strBoothName; }
	//	Get booth CRC
	int GetBoothCRC() { return m_crcBooth; }

	//	Get player state
	void SetState(int state) { m_dwStates |= state; }
	void ClearState(int state) { m_dwStates &= ~state; }
	bool IsDead() { return (m_dwStates & GP_STATE_CORPSE) ? true : false; }
	bool IsFlying() { return (m_dwStates & GP_STATE_FLY) ? true : false; }
	bool IsSitting() { return (m_dwStates & GP_STATE_SITDOWN) ? true : false; }
	bool IsInvader() { return (m_dwStates & GP_STATE_INVADER) ? true : false; }
	void SetInvaderState() { m_dwStates |= GP_STATE_INVADER; }
	void ClearInvaderState() { m_dwStates &= ~GP_STATE_INVADER; }
	bool IsInSanctuary() { return m_bInSanctuary; }
	//	Is resources ready ?
	bool IsSkeletonReady() { return (m_dwResFlags & RESFG_SKELETON) ? true : false; }
	bool IsSkinReady() { return (m_dwResFlags & RESFG_SKIN) ? true : false; }
	bool IsAllResReady() { return (m_dwResFlags & RESFG_ALL) == RESFG_ALL; }

	bool IsAboutToDie() { return m_bAboutToDie; }
	void SetAboutToDie(bool bFlag) { m_bAboutToDie = bFlag; }
	//	Check whether a action index is valid
	bool IsValidAction(int iIndex) { return (iIndex >= 0 && iIndex < ACT_MAX) ? true : false; }

	//	Is host a GM
	bool IsGM() { return (m_dwGMFlags & GMF_IAMGM) ? true : false; }
	bool IsGMInvisible() { return (m_dwGMFlags & GMF_INVISIBLE) ? true : false; }
	bool IsGMInvincible() { return (m_dwGMFlags & GMF_INVINCIBLE) ? true : false; }

	//	Get team to which this player belongs to
//	CECTeam* GetTeam() { return m_pTeam; }
	//	Set team to which this player belongs to
//	void SetTeam(CECTeam* pTeam) { m_pTeam = pTeam; } 
	//	Is specified id a member of our team ?
	bool IsTeamMember(int idPlayer);
	//	Get team requirement
	const TEAMREQ& GetTeamRequire() { return m_TeamReq; }
	//	Set team requirement
	void SetTeamRequireText(const TEAMREQ& Req);
	void SetTeamRequire(const TEAMREQ& Req, bool bConfirm);
	//	Get team requirement string
	const ACHAR* GetTeamReqText() { return m_strTeamReq; }

	//	Set extend state
	void SetExtState(int n) { m_dwExtStates |= (UINT64(1) << n); }
	//	Remove extend state
	void RemExtState(int n) { m_dwExtStates &= ~(UINT64(1) << n); }
	//	Check extend state
	bool GetExtState(int n) { return (m_dwExtStates & ( UINT64(1) << n)) ? true : false; }
	//	Get icon state number
	int GetIconStateNum() { return m_aIconStates.GetSize(); }
	//	Get icon state
	BUFFINFO& GetIconState(int n) { return m_aIconStates[n]; }
	//  has specified buff id
	bool IsInDomainState();
	//	Is in fighting state ?
	virtual bool IsFighting() { return m_bFight; }
	void SetFightFlag(bool bTrue) { m_bFight = bTrue; }
	//	Check whether player has effect of specified type
	bool HasEffectType(int iEffType);
	float GetPortraitCameraScale() { return m_vPortraitCamScale; }

	//	Get move speed
	float GetFlySpeed() { return m_ExtProps.run_speed; }
	float GetSwimSpeed() { return 3.0f; }
//	float GetGroundSpeed() { return m_bWalkRun ? m_ExtProps.mv.run_speed : m_ExtProps.mv.walk_speed; }
	float GetGroundSpeed();

	CECModel *	GetPlayerModel()		{ return m_pPlayerModel; }
	CECModel *	GetBackupModel()		{ return m_pBackupModel; }
	CECModel *	GetDummyModel()			{ return m_pDummyModel; }
	CECModel *	GetPetModel()			{ return m_pPetModel; }
	CECModel *  GetChangedModel()		{ return m_pChangedModel; }
	CECModel *  GetTransfiguredModel()	{ return m_pTransfiguredModel; }
	CECModel *  GetCurUsedModel()		{ return m_pTransfiguredModel? m_pTransfiguredModel:m_pBackupModel; }
	CECSprite *	GetSpriteModel()		{ return m_pSprite; }
	CECModel *	GetWingModel();
	A3DSkinModel* GetPortraitModel()	{ return m_pPortraitModel; }
	A3DSkinModel* GetHeadPortraitModel(){ return m_pHeadPortraitModel;}
	/*
	 *	Add By Zhangyu, 12.22.04
	 */
	void ShowEquipments(const int* pEquipmentID, bool bHide = true, bool bLoadAtOnce = false);
	//	Show / hide wing
	void ShowWing(bool bShow);
	//	Get id of full suite
	int GetFullSuiteID() { return m_idFullSuite; }

	//	Initialize static resources
	static bool InitStaticRes();
	//	Release static resources
	static void ReleaseStaticRes();
	//	Build action list
	static void BuildActionList();
	//	Load player model
	static bool LoadPlayerModel(int iPhysique, int iProfession, int iLevel, int iEarid, int iTailid, int iFaceid, int iHairid, int iDefaultSkin,
		int iGender, const int* pEquips, const char* szPetPath, const char* szChangedPath, EC_PLAYERLOADRESULT& Ret);
	//	Load player equips
	static bool LoadPlayerEquips(int iPhysique, int iProfession, int iDefaultSkin, int iGender, unsigned int uMask, const int* pEquips, EquipsLoadResult& Ret);
	//	Load pet
	static bool LoadPetModel(const char* szPetPath, EC_PLAYERLOADRESULT& Ret);
	//	Load changed model
	static bool LoadChangedModel(const char * szPath, EC_PLAYERLOADRESULT& Changed);
	//  Load Transfigured model
	static bool LoadTransfiguredModel(const char * szPath, EC_PLAYERLOADRESULT& Ret);
	//	Load aircraft model
	static bool LoadAircraftModel(const char* szPath, EC_PLAYERLOADRESULT& ret);
	//	Release player model
	static void ReleasePlayerModel(EC_PLAYERLOADRESULT& Ret);
	//	Get exp of specified level
	static double GetLevelUpExp(int iLevel, int iRebornCount);
	static double GetDeityLevelUpExp(int iLevel);
	static int GetTalismanLevelUpExp(int iLevel);
	static int GetTalismanTotalExp(int iLevel);
	static int GetProduceUpExp(int iLevel);
	static double GetPetLevelUpExp(int iLevel);
	static int GetReputationLevel(int nReputation);
	//	Build riding pet file name
	static const char* GetRidingPetFileName(int idPet, int iLevel);
	//	Get changed model file name
	static const char* GetChangedModelFileName(int id, int gender, int prof);
	//  Get transfigured model file name
	static const char* GetTransfiguredModelFileName(int id);
	//	Get sect info
	static SECT_LEVEL2_INFO* GetSectInfoByProf(int idProf);
	static int GetRaceByProf(int idProf);
	static int GetPhysiqueByProf(int idProf);

//	CECAchievementMan* GetAchievementMan(){ return m_pAchievementMan; }
	bool IsClothesOn();

	void HideHead(bool bHide);
	void HideBody(bool bHide);
	void HideFoot(bool bHide);
	bool IsHeadHide()   { return (m_dwHideEquipMask & (1<<EQUIPIVTR_HEAD)) != 0; }
	bool IsBodyHide()   { return (m_dwHideEquipMask & (1<<EQUIPIVTR_BODY)) != 0; }
	bool IsFootHide()	{ return (m_dwHideEquipMask & (1<<EQUIPIVTR_FOOT)) != 0; }
	DWORD GetHideEquipMask() { return m_dwHideEquipMask; }
	
	bool IsMultiRideLeader() { return m_bMultiRideLeader; }
	bool IsMultiRideFull() 
	{
		for (int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if( m_iMultiBuddyId[i]==0)
				return false;
		}
		return true;
	}
	bool IsMultiRideHasMember()
	{
		if(!m_bMultiRideLeader)
			return false;
		for(int i =1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if(m_iMultiBuddyId[i] != 0)
				return true;
		}
		return false;
	}
	bool IsMultiRideMember() { return m_bMultiRideMember; }
	int  GetMultiRideLeaderId() { return m_iLeaderId; }
	void RemoveMultiRideMember(int idTarget);
	int  GetMultiRidePos() { return m_iMultiRidePos; }
	const int* GetMultiRideMemberId() { return m_iMultiBuddyId; }
	int GetBattleInstanceState() { return m_iBattleInstanceState; }
	int GetApplyBattleType() { return m_iApplyBattleType; }
	int	 GetBattleType() { return m_iBattleType; }
	void SetMilitaryRank(int rank);
	const ACHAR* GetMilitaryRankName(int rank);
	void ChangeFace(int faceid, int hairid, int skinid);
	void ChangeFeature(int iEarid, int iTailid);	//烈山更换耳朵和尾巴
	void LoadHeadPortraitModel();
	void ChangeHeadPortraitModel(int iFaceid, int iHairid);
	void ChangeVipState(int iVipState){ m_iVipState = iVipState;}
	int  GetVipState() const { return m_iVipState; }
	//	Get incant counter
//	CECCounter& GetIncantCnt() { return m_IncantCnt; }

	//	Carrier about
	int GetCarrierID() { return m_idCarrier; }
	void SetCarrierID(int bIdCarrier) { m_idCarrier = bIdCarrier; }
	A3DVECTOR3 GetRelPosOnCarrier() { return m_matRelCarrier.GetRow(3); }
	A3DVECTOR3 GetRelDirOnCarrier() { return m_matRelCarrier.GetRow(2); }
	void ReloadModel();
	int GetCloneID() { return m_iCloneID; }
	void SetCloneID( int id) { m_iCloneID = id; } 
	// ISCLONEPLAYERID()
	int GetSummonerID(){  return m_idSummoner; }
	//   仅用于怀光，重置连击的动作，连击情况下，提前打死怪物的时候调用
	void ResetDoubleHitState();
	void SetUpdateCloneEquip() { m_bUpdateCloneEquip = true; }
	void SetCircleID(int id);
	void SetCRole(int id);
	int  GetCircleID()  { return m_idCircle; }
	int  GetCRole()	{ return m_idCRole; }
	//   仅用于天华，重置弹琴的动作
	void ResetPlayLuteAction();	
	int  GetSrcServerID() { return m_idOrgServer; }		// 跨服后，玩家来自哪个服务器的id 
	void UpdateDeityStateGFX(int old_cult, int new_cult);
	int GetProfRace();
	
	unsigned char GetVipLv() { return m_ucVIPLevel; }

	bool IsRuneEquipOpen()  { return m_bEquipRuneOpen; }
	
protected:

	static void ChangeDefaultUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin);
	static void ChangeDefaultFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin);
	static void ChangeDefaultFashionUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin);
	static void ChangeDefaultFashionLower(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin);
	static void ChangeDefaultFashionFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin);

protected:	//	Attributes
	static CECStringTab		m_ActionNames;				// Action name table
	static CECStringTab		m_SkillActs;				// Skill action name table
	static PLAYER_LEVELEXP_CONFIG m_PlayerLevelUpExp;	// level up exp
	static SectLevel2Map	m_SectLevel2Map;			// Profession sect map
	static int				m_TalismanTotalExp[100];
	static WStrTable		m_MilitaryRanks;			// 军衔名称
//    static DWORD			m_HideEquipMaskInArena;		// 竞技场中的装备隐藏掩码
	static const int		m_EquipsInArena[][4];

	float					m_vPortraitCamScale;	//	portrait camera scale when using change pill
	__int64					m_dwExtStates;			//	Visible extend states
	AArray<BUFFINFO>		m_aIconStates;			//	Icon states (un-visible extend states)
	AArray<int, int>		m_aCurEffects;			//	Current effects

	CECPlayerMan*	m_pPlayerMan;		//	Player manager
	CECModel*		m_pPlayerModel;		//	Player model
	CECModel*		m_pDummyModel;		//	Dummy model
	CECModel*		m_pBackupModel;		//	Backup Model to save Player Model
	CECModel*		m_pPetModel;		//	Pet model
	CECModel*		m_pChangedModel;	//	Model after changed
	CECModel*		m_pTransfiguredModel;	//	Model after transfigured
	A3DSkinModel*	m_pPortraitModel;	//	Portrait model
	A3DSkinModel*   m_pHeadPortraitModel; // Head Portrait for change face

	CECSprite *		m_pSprite;			//	sprite model
	CECSprite *		m_pSpriteTune;

	A3DSkin*		m_aSkins[NUM_SKIN_INDEX][3];//	Player's skins, managed by player
	A3DSkin*		m_aCurSkins[NUM_SKIN_INDEX];//	currently used skins

	A3DShader*		m_pBodyShader[3];	//	Player's body skin shader
	INFO			m_PlayerInfo;		//	Player information got from server
	ROLEBASICPROP	m_BasicProps;		//	Basic properties
	ROLEEXTPROP		m_ExtProps;			//	Extend properties
	int				m_nRebornCount;
	int				m_iMoneyCnt;		//	Money count
	int				m_iMaxMoney;		//	Maximum money value
	int				m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
	int				m_aHideEquips[SIZE_EQUIPIVTR];  //  Equipment item ID array for hide equip or fashion 
	int				m_iReputation;		//	Player's reputation
	int				m_iCultivation;		//  造化值（仙魔佛）
	int				m_iTitle;			//	Player's current title
	float			m_fDistToCamera;	//	Distance to camera
	bool			m_bInSanctuary;		//	true, player is in sanctuary
	PVPINFO			m_pvp;				//	pvp information
	int				m_iBoothState;		//	Booth state. 0, none; 1, prepare; 2, open booth; 3, visite other's booth
	int				m_crcBooth;			//	Booth crc
	bool			m_bFashionMode;		//	true, in fashion mode
	int				m_idFaction;		//	ID of player's faction
	int				m_idFamily;			//	ID of player's family
	int				m_idFRole;			//	ID of player's faction role
	RIDINGPET		m_RidingPet;		//	Riding pet information
	TRANSFIGUREINFO	m_TransfigureInfo;	//	Transfigure information
	int				m_iBattleCamp;		//	Battle this player belongs to
	AArray<TANKINFO, TANKINFO&> m_aBattleTanks;	//	Battle tanks controlled by this player
	int				m_idFullSuite;		//	id of the full suite
	int				m_idSpouse;			//	id of spouse
	int				m_idMaster;
	A3DCOLOR		m_colorTitle;		//	title color
	A3DCOLOR		m_clCityOwner;		//	city owner color

	ACString		m_strName;			//	Player name
	ACString		m_strReplacedName;  //  Player name in transfigured mode
	int				m_iFaceid;			//	Face id
	int				m_iHairid;			//	Hair id
	int				m_iEarid;			//  Ear id
	int				m_iTailid;			//  Tail id
	int				m_iDefaultSkin;		//  默认皮肤
	int				m_iGender;			//	Gender
	int				m_iRace;			//	Race
	int				m_iPhysique;		//  Physique
	DWORD			m_dwGMFlags;		//	GM flags

	int				m_iMoveMode;		//	Player's move mode
	int				m_iMoveEnv;			//	Move environment
	bool			m_bWalkRun;			//	Walk-run switch, 0-walk, 1-run
	bool			m_bRushFly;			//	true, in rush fly mode
	MOVECONST		m_MoveConst;		//	Const used when moving control

	A3DAABB			m_aabb;				//	Player's aabb
	DWORD			m_dwStates;			//	Player's basic states
	bool			m_bCastShadow;		//	flag indicates whether it will cast shadow
	float			m_fTouchRad;		//	Touch radius
	bool			m_bRenderName;		//	Render name flag
	bool			m_bRenderBar;		//	Render HP, MP, EXP bars
	DWORD			m_dwResFlags;		//	Resources ready flag
	bool			m_bFight;			//	true, Is fighting
	TEAMREQ			m_TeamReq;			//	Team requirment
	PATECONTENT		m_PateContent;		//	Pate content

//	CECTeam*		m_pTeam;			//	The team to which player belongs
	A3DGFXEx*		m_pLevelUpGFX;		//	Level up gfx
	A3DGFXEx*		m_pWaterWaveStill;
	A3DGFXEx*		m_pWaterWaveMove;
	A3DGFXEx*		m_pTransformGfx;
	A3DGFXEx*		m_pAirBubble;
	A3DGFXEx*		m_pSwimBubble;
//	CECSkill*		m_pCurSkill;		//	Current used skill
	int				m_idCurSkillTarget;	//	Current skill target id
	A3DGFXEx*		m_pDuelStateGFX;	//	a gfx standing for in duel state.
	A3DGFXEx*		m_pVipStateGFX;		//  a gfx standing for in vip state.

// 	CECPateText*	m_pPateName;		//	Player name
// 	CECPateText*	m_pPateTitle;		//	Title
// 	CECPateText*	m_pPateCityOwner;	//	City owner;
// 	CECPateText*	m_pPateFamily;		//	Family
// 	CECPateText*	m_pPateLastWords1;	//	The words player said last time, line 1
// 	CECPateText*	m_pPateLastWords2;	//	The words player said last time, line 2
// 	CECCounter		m_strLastSayCnt;	//	Time counter of last said words
// 	CECCounter		m_IncantCnt;		//	Incant counter
// 	CECPateText*	m_pPateTeamReq;		//	Team requirement
// 	CECPateText*	m_pPateBooth;		//	Booth name
// 	CECPateText*	m_pPateFaction;		//	Faction name
// 	CECPateText*    m_pPateRankName;	//  military rank name in SONGJIN battle
// 	CECPateText*    m_pPateCircle;		//  circle name
	CECSpriteDecal*	m_pFactionDecal;	//	Faction icon
	ACString		m_strTeamReq;		//	Team requirement string
	ACString		m_strBoothName;		//	Booth name string

	// stones effects.
	BYTE			m_stoneBody;		//	stone effects on body
	BYTE			m_stoneHead;		//	stone effects on wrist
	BYTE			m_stoneFoot;		//	stone effects on foot
	BYTE			m_stoneWeapon;		//	stone effects on weapon
	
	CECBubbleDecalList*	m_pBubbleTexts;	//	Bubble text list

	// 下身的装配方法
	int				m_nLowerEquipMethod;
	// 时装下身的装配方法
	int				m_nLowerFashionEquipMethod;
	// 当前攻击方式
	unsigned int	m_uAttackType;
	// 飞行
	int				m_nFlyMode;
	// 已收到并处理了deadly_strike标志为true的attack_result
	bool			m_bAboutToDie;
	// 依附类型
	AttachMode		m_AttachMode;
	// 是否是依附者
	bool			m_bHangerOn;
	// 依附者或被依附者id
	int				m_iBuddyId;
	int				m_idCandBuddy;		//	ID of candidate buddy
	bool			m_bCandHangerOn;
	RIDINGPET		m_CandPet;
	TRANSFIGUREINFO	m_CandTransfigure;	
	float			m_fPetHeight;
	// 成就管理
//	CECAchievementMan* m_pAchievementMan;

	// 多人骑乘
	int				m_iMultiBuddyId[MULTI_RIDE_CAPACITY];
	int				m_iMultiCandBuddyId[MULTI_RIDE_CAPACITY];
	int				m_iLeaderId;        //   ID of multi ride leader
	int				m_iCandLeaderId;    //   ID of candidate multi ride leader
	unsigned char   m_iCandMultiRidePos;//   candidate position 
	bool			m_bMultiRideMember; //   是否多人骑乘中的成员
	bool			m_bMultiRideLeader; //   是否多人骑乘中的主角
	unsigned char   m_iMultiRidePos;    //   多人骑乘中在的位置
	
	int				m_idCarrier;		//	ID of carrier on which this player is standing
	A3DMATRIX4		m_matRelCarrier;	//	Transform matrix from player space to carrier space
	
	// 变身
	int				m_iChangeShape;
	int				m_iCandChangeShape;

	// flag indicates whether or not we need update pate title
	bool			m_bPateNeedUpdate;

	// Load flag
	bool			m_bLoadFlag;

	// flag indicates whether render family name
	bool			m_bRenderFamily;
	bool			m_bRequestFamilyName;
	bool			m_bRequestCircleName;
	bool			m_bRequestFactionName;

	// flag indicates whether is using talisman skill
	bool			m_bUsingTalismanSkill; 

	// hide equipment or fashion, show default equipment.
	DWORD			m_dwHideEquipMask;
	// 真正的隐藏结果，将无法装备的装备隐藏，同时兼顾玩家设置的装备隐藏
	DWORD			m_dwHideEquipMaskInUse;
	
	int				m_iBattleInstanceState;	// 剧情战场报名情况 0-未报名 1-报名 2-战场中
	int				m_iApplyBattleType;	// 报名的战场，未进
	int				m_iBattleType;    // 所在战场的类型
	ACString		m_strMilitaryRankName;	// 军衔名称
	int				m_iMilitaryRank;
	bool			m_bHasChangedEquips;
	bool			m_bFashionModeBackUp;
	DWORD			m_dwHideEquipMaskBackUp;
	int				m_iVipState;
	float			m_fTransHeight;		// 变身后的高度，显示头顶名字
	bool			m_bAnonymous;		// 是否处于匿名地图
	CECCounter      m_ScaleWeaponCnt;
	bool			m_bScaleWeapon;		// 是否放缩武器
	bool			m_bInvisible;		// 处于隐身状态
	bool			m_bDim;				// 处于朦胧状态
	int				m_iCloneID;			// 分身的id
	int				m_idSummoner;		// 召唤者，主人的id
	int				m_iDoubleHit;		// 连击	
	bool			m_bUpdateCloneEquip;// 需要更新分身的装备信息
	int				m_idCircle;			// 圈子id
	int				m_idCRole;			// 圈子里面担任的角色
	int				m_idOrgServer;		// 跨服状态下，玩家原始的服务器id
	int				m_iSpecialState;	// 由buff附加的状态，以后统一放在这里面进行处理

	//Added 2011-06-23.
	float			m_fScaleShapeRatio;		// 用于太昊的新技能：脱灵和归神， 缩放倍数，默认状态下为0.0f
	//Added end.
	//Added 2011-08-26.
	int				m_iDelayedOperateState;	// 用于记录那些需要延后处理的状态
	//Added end.
	//Added 2012-01-16.
	unsigned char	m_ucVIPLevel;			// 用于记录VIP奖励当前的等级，只用于VIP奖励，不用于普通奖励
	//Added end.

	bool			m_bEquipRuneOpen;

public:
	static const int	FACEHAIRTYPE_NUM;

protected:	//	Operations
	
	//	When all resources are ready, this function will be called
	virtual void OnAllResourceReady();
	//	When weapon has been changed, this function will be called
	virtual void OnWeaponChanged() {}
	//	Apply effect on player
	virtual bool ApplyEffect(int iEffect, bool bCheckArray);
	//	Discard effect from player
	virtual void DiscardEffect(int iEffect);

	//	Load host's skeleton
	bool LoadPlayerSkeleton(const int* pEquips, bool bLoadAtOnce=false, bool bHideEquip=true);

	bool QueueLoadEquips(const int* pEquips, DWORD dwMask, bool bAtOnce);
	bool QueueLoadPetModel(const char* szPetPath, bool bAtOnce);
	bool QueueLoadChangedModel(const char* szPath, bool bAtOnce);
	bool QueueLoadTransfiguredModel(const char* szPath, bool bAtOnce);
	bool QueueLoadAircraftModel(const char* szPath, bool bAtOnce);

	//	Update current skins
	bool UpdateCurSkins();
	//	Update God Evil sprite
	bool UpdateTalismanSprite(CECModel* pModel);
	// 更新元魂模型
	bool UpdateTuneSprite(CECModel* pModel);

	//	Release player model
	void ReleasePlayerModel();
	//	Release dummy model
	void ReleaseDummyModel();
	// Play model action by weapon and relative action index
	bool PlayAction(int iAction, float fWeight, bool bRestart=true, int iTransTime=200, bool bQueue=false);
	bool PlayStartUseItemAction(int tid);
	// Play use item effect
	bool PlayUseItemEffect(int tid, const void* pData = NULL, size_t sz = 0);
	//	Caculate player's AABB base on profession and gender
	void CalcPlayerAABB();
	//	Get move or stand action
	int GetMoveStandAction(bool bMove, bool bFight=false);
	//	Check whether current action is moving action
	bool CurActionIsMoving();
	//	Fill pate content
	bool FillPateContent(CECViewport* pViewport);
	//	Check water moving environment
	bool CheckWaterMoveEnv(const A3DVECTOR3& vPos, float fWaterHei, float fGndHei);

	//	Set resources ready flag
	void SetResReadyFlag(DWORD dwFlag, bool bSet);
	//	Render titles / names / talks above player's header
	bool RenderName(CECViewport* pViewport, DWORD dwFlags);
	//	Render bar above player's header
	bool RenderBars(CECViewport* pViewport);
	//	Render equipments for bloom
	bool RenderEquipBloom(CECViewport * pViewport);
	//	Render booth name
	int RenderBoothName(CECViewport* pViewport, int y);
	//	Set new visible extend states
	void SetNewExtendStates(__int64 dwNewStates);
	//	Decompress advertisement data
	void DecompAdvData(int iData1, int iData2);
	
	void RemoveArmorStones(int nEquipIndex, BYTE status);
	void AddArmorStones(int nEquipIndex, BYTE status);
	void RemoveWeaponStones(BYTE status);
	void AddWeaponStones(BYTE status);
	void RemoveFullSuiteGFX(int idFullSuite);
	void AddFullSuiteGFX(int idFullSuite);

	void ShowArmorStones(int nEquipIndex, BYTE status, bool bVisible);
	void ShowFullSuiteGFX(int idFullSuite, bool bVisible);
	
	void AddFootGFX(int iGender);
	void AddSpineGFX();
	void AddDeityGFX();

	//	Message handlers
	virtual void OnMsgPlayerExtState(const ECMSG& Msg);
	virtual void OnMsgEnchantResult(const ECMSG& Msg);
	virtual void OnMsgPlayerAdvData(const ECMSG& Msg);
	virtual void OnMsgPlayerPVP(const ECMSG& Msg);
	virtual void OnMsgSwitchFashionMode(const ECMSG& Msg);
	virtual void OnMsgPlayerEffect(const ECMSG& Msg);
	virtual void OnMsgChangeNameColor(const ECMSG& Msg);
	virtual void OnMsgPlayerMount(const ECMSG& Msg);
	virtual void OnMsgPlayerLevel2(const ECMSG& Msg);
	virtual void OnMsgPlayerScaleShape(const ECMSG& Msg);	//缩放角色整体大小，Added 2011-06-28.
	virtual void OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg); //VIP奖励等级更新 Added 2012-01-17.

	//	Get distance to camera
	float GetDistToCamera() { return m_fDistToCamera; } 
	bool  CanEquipThis(int id);
	void  UpdateHideEquipMask(const int* pEquipmentID);
	
public:
	//	Play a damaged effect
	void Damaged(int nDamage, int nDivisions, DWORD dwModifier=0);
	//	Print bubble text
	void BubbleText(int iIndex, DWORD dwNum, DWORD dwWaitTime=0, int p1=0, bool bLastOne=true, bool bRandPos=false);

	//	Play an attack effect
	void PlayAttackEffect(int idTarget, int idSkill, int skillLevel, unsigned char cSerialId, int nDamage, DWORD dwModifier, int* piAttackTime=NULL);

	// 变身
	bool IsShapeChanged() const { return m_iChangeShape != 0; }
	void TransformShape(int form, bool bPlayerGfx = false);
	void Transfigure(int id, bool bPlayGfx = false);

	// 依附关系
	virtual void AttachBuddy(int iBuddy);
	virtual void DetachBuddy(CECPlayer* pBuddy = NULL);
	AttachMode GetAttachMode() const { return m_AttachMode; }

	virtual void AttachMultiBuddy(int iBuddy, int pos);
	virtual void DetachMultiBuddy(int pos, CECPlayer* pBuddy = NULL);
	// 骑乘
	virtual void RideOnPet(int id, int iLev,int type);
	virtual void GetOffPet();
	bool IsRidingOnPet() { return m_RidingPet.id ? true : false; }
	//  Is Transfigured
	bool IsTransfigured() { return m_TransfigureInfo.id ? true : false; }
	//	Get riding pet info.
	const RIDINGPET& GetRidingPetInfo() { return m_RidingPet; }
	//  Get transfigure info
	const TRANSFIGUREINFO& GetTransfigureInfo() { return m_TransfigureInfo; }
	//	Do emote action
	virtual bool DoEmote(int idEmote) { return true; }

	virtual void StartGeneralOprt(int op_id, int duration) {}
	virtual void StopGeneralOprt(int op_id) {}

	virtual bool IsPlayerMoving() { return false; }
	bool IsHangerOn() const { return m_bHangerOn; }

	int GetWeaponID()	{ return m_aEquips[EQUIPIVTR_WEAPON] & 0xffff; }
	const int* GetEquips() const { return m_aEquips; }
	const int* GetHideEquips() const { return m_aHideEquips; }

	void SetRenderFamily(bool bRender) { m_bRenderFamily = bRender; }
	bool IsUsingTalismanSkill() { return m_bUsingTalismanSkill; }
	void ClearUsingTalismanSkill() { m_bUsingTalismanSkill = false; }
	int  GetFlyMode() { return m_nFlyMode; }
	bool IsInAnonymousMap() const { return m_bAnonymous; }
	void SetAnonymousState(bool bAnonymous ) { m_bAnonymous = bAnonymous; } 
	void EnterInvisibleState(unsigned char state);
	void EnterDimState(unsigned char state);
	virtual void DeityLevelUp(bool bSuccess, int iLevel){}
	virtual void UpdateSpecialState(int state, bool on);
	bool ScaleObjects( CECModel* pModel, float fScaleRatio = 1.0f ); //Added 2011-08-17.

	// static funcs
	static bool ChangeWeapon	(EquipsLoadResult* pResult, const char* szLeft, const char* szRight, const char* szBack = ""); //添加对背部武器的处理接口Modified 2011-08-09.
	static void ChangeWeapon	(EquipsLoadResult* pResult, int iPhysique, int nGender, const EQUIPMENT_ESSENCE* pWeapon);
	static void ChangeTalisman	(EquipsLoadResult* pResult, const char* szPath);
	static bool ChangeWing		(EquipsLoadResult* pResult, const char* szModeFile);
	static bool ChangeTune		(EquipsLoadResult* pResult, const char* szPath);
	static bool LoadPlayerSkin	(A3DSkin* aSkins[3], int iIndex, const char* szFile);
	static bool ChangeArmor		(const char* strSkinFile, A3DSkin* aSkins[3], int nLocation, bool bDefault, int iPhysique, int iProfession, int nGender, int iDefaultSkin);
	static void ChangeArmor		(const EQUIPMENT_ESSENCE* pArmor, DWORD& dwShowMask, DWORD& dwSkinChangeMask, int iPhysique, int iProfession, int nGender, int iSkin, A3DSkin* pSkins[][3]);
	static bool ChangeFashion	(const char* strSkinFile, A3DSkin* aSkins[3], int nLocation, bool bDefault, int iPhysique, int iProfession, int nGender, A3DCOLOR color, int iDefaultSkin);
	static void ChangeFashion	(const EQUIPMENT_ESSENCE* pFashion, DWORD& dwShowMask, DWORD& dwSkinChangeMask, int iPhysique, int iProfession, int nGender, int iDefaultSkin, A3DSkin* pSkins[][3], A3DCOLOR color);
	static void ShowEquipments	(int iPhysique, int iProfession, int iDefaultSkin, int nGender, bool bHasDummy, const int* pEquipmentID, unsigned int uChangeMask, EquipsLoadResult* pResult);
	static int GetEquipPathId	(int nEquipId, int nGender, int nPhysique);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

bool PlayerRenderForShadow(A3DViewport * pViewport, void * pArg);
void PlayerRenderPortrait(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
void PlayerRenderDemonstration(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);
void PlayerRenderHeadPortrait(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3);

