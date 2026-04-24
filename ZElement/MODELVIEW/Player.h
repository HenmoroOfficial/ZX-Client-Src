/*
 * FILE: Player.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: 
 *
 * HISTORY:  2010.8  Updated by MaYuanzheng
 *
 * Copyright (c) 2010 ZhuXian Studio, All Rights Reserved.
 */


#if !defined(AFX_PLAYER_H__449694F5_E9D6_438B_9E75_0E039E5B24E8__INCLUDED_)
#define AFX_PLAYER_H__449694F5_E9D6_438B_9E75_0E039E5B24E8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "RenderableObject.h"
#include "AnimationBar.h"
#include "CharactorBar.h"

class CECModel;
class CECSprite;
class A3DSkin;
class A3DViewport;


#define HEAD_INDEX_NUM  7//头型和发型的数量 see EC_Player.cpp(92):#define HEAD_INDEX_NUM  7
#define EAR_INDEX_NUM	6//烈山耳朵的颜色数
#define TAIL_INDEX_NUM  6//烈山尾巴的颜色数
#define FASHION_EAR_TAIL_INDEX_NUM  7

enum
{
	enumSkinShowNone = 0,
	enumSkinShowHead,
	enumSkinShowUpperBody,
	enumSkinShowLowerBody,
	enumSkinShowUpperAndLower,
	enumSkinShowFoot,
	enumSkinShowGlasses,
	enumSkinShowNose,
	enumSkinShowMustache,
	enumSkinShowCloak,
	enumNumSkinShow
};

enum
{
	show_both = 0,
	show_upper_with_lower,
	show_lower_with_foot,
};
//	Skin index
enum
{
	SKIN_HEAD_INDEX = 0,			// 头
	SKIN_HAIR_INDEX,				// 头发
	SKIN_EAR_INDEX,					// 耳朵  烈山 妖族专用
	SKIN_TAIL_INDEX,				// 尾巴  烈山 妖族专用
	SKIN_BODY_INDEX,				// 普通衣服
	SKIN_FOOT_INDEX,				// 普通鞋子
	SKIN_HELM_INDEX,				// 普通头盔
	//SKIN_GLASSES_INDEX,				// 眼镜
	//SKIN_NOSE_INDEX,				// 鼻子
	//SKIN_MUSTACHE_INDEX,			// 胡子
	//SKIN_CLOAK_INDEX,				// 披风
	SKIN_FASHION_UPPER_BODY_INDEX,	// 时装上衣
	//SKIN_FASHION_LOWER_INDEX,		// 时装裤子
	SKIN_FASHION_FOOT_INDEX,		// 时装鞋子
	SKIN_FASHION_HEADWEAR_INDEX,	// 时装头饰
	SKIN_FASHION_EAR_INDEX,					// 耳朵  烈山 妖族专用
	SKIN_FASHION_TAIL_INDEX,				// 尾巴  烈山 妖族专用
	NUM_SKIN_INDEX,
};
//	Equip mask
enum
{
	EQUIP_MASK_WEAPON			= 0x0001,
	EQUIP_MASK_HEAD				= 0x0002,
	EQUIP_MASK_BODY				= 0x0004,
	EQUIP_MASK_FOOT				= 0x0008,
	EQUIP_MASK_NECK				= 0x0010,
	EQUIP_MASK_FINGER1			= 0x0020,
	EQUIP_MASK_FINGER2			= 0x0040,
	EQUIP_MASK_FASHION_EYE		= 0x0080,
	EQUIP_MASK_FASHION_NOSE		= 0x0100,
	EQUIP_MASK_FASHION_LIP		= 0x0200,
	EQUIP_MASK_FASHION_HEAD		= 0x0400,
	EQUIP_MASK_FASHION_BODY		= 0x0800,
	EQUIP_MASK_FASHION_LEG		= 0x1000,
	EQUIP_MASK_FASHION_FOOT		= 0x2000,
	EQUIP_MASK_FASHION_BACK		= 0x4000,
	EQUIP_MASK_WING				= 0x8000,
	EQUIP_MASK_MAIN_TALISMAN	= 0x10000,
	EQUIP_MASK_SUB_TALISMAN1	= 0x20000,
	EQUIP_MASK_SUB_TALISMAN2	= 0x40000,
	EQUIP_MASK_SPEAKER			= 0x80000,
	EQUIP_MASK_AUX1				= 0x100000,
	EQUIP_MASK_AUX2				= 0x200000,
	EQUIP_MASK_AUX3				= 0x400000,
	EQUIP_MASK_AUX4				= 0x800000,
	EQUIP_MASK_AUX5				= 0x1000000,
	EQUIP_MASK_AUX6				= 0x2000000,
	EQUIP_MASK_ALL				= 0x3ffffff,
};
//Charactor Type (physique) 
//enum
//{
//	CHARACTOR_PHYSIQUE_NORMAL   =   0,
//	CHARACTOR_PHYSIQUE_SMALL,
//	CHARACTOR_PHYSIQUE_MIDDLE,
//	CHARACTOR_PHYSIQUE_BIG,
//	CHARACTOR_PHYSIQUE_NUM,
//};


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

		ACT_MAX,
		ACT_CASTSKILL,				// !! This is only a placeholder which represents skill actions
	};

enum
{
	ACT_CHANNEL_BODY	= 1,
	ACT_CHANNEL_EYE		= 2,
	ACT_CHANNEL_WOUND	= 3
};


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


class CPlayer :public CRenderableObject 
{
public:
	//	Player action type
	enum
	{
		ACTTYPE_SH = 0,	//	Single hand weapon
		ACTTYPE_FH,		//	Free hands
		ACTTYPE_BHF,	//	Both hands far range weapon
		ACTTYPE_BHN,	//	Both hands near range weapon
		NUM_ACTTYPE,
	};
public:
	CPlayer(CAnimationBar* pABar,CCharactorBar *pCBar);
	virtual ~CPlayer();

public:
	virtual void Tick(DWORD dwTimeDelta);
	virtual void Render(A3DViewport *pA3dViewPort);
	virtual void PlayAction(AString strAction, bool bLoop = false);
	virtual bool PlayAttackAction(const char* szActName, int idSkill);
	virtual void StopAction();
	virtual A3DVECTOR3 GetDefaultCamaraPos();
	virtual void Release();
	virtual A3DMATRIX4 GetAbsoluteTM();
	virtual void GetPathList(abase::vector<AString>& listPath);
	virtual void GetRotateAndPos(A3DVECTOR3& r, A3DVECTOR3& pos);
	virtual int GetWeaponID();

	bool ReplaceFixedSkin(int nPart,int nGender, int nSkin,int iLevelPhrase);
	bool ReplaceSkin(int nPart, DWORD ptr);
	bool MountModel(const char* pos,DWORD ptr);
	int  GetGender(){ return m_nGender; }
	bool GetRide(){if(m_pPetModel)return true; return false;}
	bool Load(int sect, int iFemail);
	bool LoadChangedModel(int id);
	void SaveConfig();
	bool IsMan(){ return (m_nGender==0)? true : false; }
	void RecreateActionList();
	void RecreateShapeList();
	void RecreateSkillList();
	int  GetHighestProfBySect(int iSect);
	void ReSetPlayer(CAnimationBar* pABar,CCharactorBar *pCBar);
	int  GetSect();
	void SetChangedModle(bool b){m_bChanged = b;}

protected:
	void MountWeapon(const char*pos, DWORD ptr);
	void MountRide(const char*pos, DWORD ptr);
	void SetDefaultSkin(CString *strSkin);
	void LoadConfig(CString *strSkin);
	void BuildActionList();
	void GetPartNameArray(CString *strSkinName);
	CString GetPathByID(DWORD id);
	CString GetRidePathByID(DWORD id);
	int     GetShowPart(DWORD part);
	CString FindRideAction(CString strAction);
	int GetPhysiqueBySect(int sect);
	const char* GetChangedModelFileName(int id, int gender, int prof);
private:
	CECModel*		m_pPlayerModel;		//	Player model
	CECModel*		m_pChangedModel;		//	Changed model
	CECModel*		m_pPetModel;		//	Pet model
	CECModel*       m_pLWeaponModel;    //  Weapon model of left hand
	CECModel*       m_pRWeaponModel;    //  Weapon model of right hand

	DWORD           m_dwSkinSet[NUM_SKIN_INDEX];
	DWORD           m_dwWeapon;
	DWORD           m_dwRide;
	

	A3DSkin*		m_aSkins[NUM_SKIN_INDEX][3];//	Player's skins, managed by player
	A3DSkin*		m_aCurSkins[NUM_SKIN_INDEX];//	currently used skins

	CAnimationBar*  m_pAnimationBar;
	CCharactorBar*  m_pCharactorBar;

	int             m_nGender;
	int             m_iPhysique;
	int             m_iSect;
	int             m_nShowType;
	int             m_iEarid;
	int             m_iTailid;
	int             m_iWeapon;
	bool            m_bChanged;

	AString         m_strActionName;
};

extern CPlayer* g_CPlayer;

class CNpc :public CRenderableObject 
{

public:
	CNpc(CAnimationBar* pABar,CCharactorBar *pCBar);
	virtual ~CNpc();

public:
	virtual void Tick(DWORD dwTimeDelta);
	virtual void Render(A3DViewport *pA3dViewPort);
	virtual void PlayAction(AString strAction, bool bLoop = false);
	virtual void StopAction();
	virtual A3DVECTOR3 GetDefaultCamaraPos();
	virtual void Release();
	virtual A3DMATRIX4 GetAbsoluteTM();
	virtual void GetPathList(abase::vector<AString>& listPath);
	virtual void GetRotateAndPos(A3DVECTOR3& r, A3DVECTOR3& pos);
	virtual bool PlayAttackAction(const char* szActName, int idSkill){ return false; }
	bool Load(const char*szPath);

private:
	CECModel*		m_pNpcModel;		//	Player model
	CAnimationBar*  m_pAnimationBar;
	CCharactorBar*  m_pCharactorBar;
	AString         m_strActionName;
};


//inline function
inline CPlayer::GetSect(){return m_iSect;}

#endif // !defined(AFX_PLAYER_H__449694F5_E9D6_438B_9E75_0E039E5B24E8__INCLUDED_)
