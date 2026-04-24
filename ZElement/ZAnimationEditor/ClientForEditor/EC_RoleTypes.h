/*
 * FILE: EC_RoleTypes.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/11/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DMacros.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define EC_MAXNOPKLEVEL		0	//	The maximum no PK level

//	Magic class
enum
{
	MAGICCLASS_GOLD	= 0,
	MAGICCLASS_WOOD,
	MAGICCLASS_WATER,
	MAGICCLASS_FIRE,
	MAGICCLASS_EARTH,
};

#ifndef NUM_MAGICCLASS
#define NUM_MAGICCLASS		6
#endif	//	NUM_MAGICCLASS

#define	NUM_GEN_COUNT		4

//	Extend properties class
enum
{
	EXTPROPIDX_BASE = 0,
	EXTPROPIDX_MOVE,
	EXTPROPIDX_ATTACK,
	EXTPROPIDX_DEF,
};

//	Profession
enum
{
	PROF_1 = 0,
	PROF_2,
	PROF_3,
	PROF_4,
	PROF_5,
	PROF_6,
	PROF_7,
	PROF_8,
	PROF_9,
	PROF_10,
	PROF_11,
	PROF_12,
	PROF_13,
	PROF_14,
	PROF_15,
	PROF_16,
	PROF_17,
	PROF_18,
	PROF_19,
	PROF_20,
	PROF_21,
	PROF_22,
	PROF_23,
	PROF_24,
	PROF_25,
	PROF_26,
	PROF_27,
	PROF_28,
	PROF_29,
	PROF_30,
	PROF_31,
	PROF_32,
	PROF_33,
	PROF_34,
	PROF_35,
	PROF_36,
	PROF_37,
	PROF_38,
	PROF_39,
	PROF_40,
	PROF_41,
	PROF_42,
	PROF_43,
	PROF_44,
	PROF_45,
	PROF_46,
	PROF_47,
	PROF_48,
	PROF_49,
	PROF_50,
	PROF_51,
	PROF_52,
	PROF_53,
	PROF_54,
	PROF_55,
	PROF_56,
	PROF_57,
	PROF_58,
	PROF_59,
	PROF_60,
	PROF_61,
	PROF_62,
	PROF_63,
	PROF_64,
	PROF_65,	//职业ID扩展，从原来的一个64位扩展为两个64位来标示 Added 2011-07-18.
	PROF_66,
	PROF_67,
	PROF_68,
	PROF_69,
	PROF_70,
	PROF_71,
	PROF_72,
	PROF_73,
	PROF_74,
	PROF_75,
	PROF_76,
	PROF_77,
	PROF_78,
	PROF_79,
	PROF_80,
	PROF_81,
	PROF_82,
	PROF_83,
	PROF_84,
	PROF_85,
	PROF_86,
	PROF_87,
	PROF_88,
	PROF_89,
	PROF_90,
	PROF_91,
	PROF_92,
	PROF_93,
	PROF_94,
	PROF_95,
	PROF_96,
	PROF_97,
	PROF_98,
	PROF_99,
	PROF_100,
	PROF_101,
	PROF_102,
	PROF_103,
	PROF_104,
	PROF_105,
	PROF_106,
	PROF_107,
	PROF_108,
	PROF_109,
	PROF_110,
	PROF_111,
	PROF_112,
	PROF_113,
	PROF_114,
	PROF_115,
	PROF_116,
	PROF_117,
	PROF_118,
	PROF_119,
	PROF_120,
	PROF_121,
	PROF_122,
	PROF_123,
	PROF_124,
	PROF_125,
	PROF_126,
	PROF_127,
	PROF_128,
		
	NUM_PROFESSION,
};

//	Sect
enum
{
	SECT_1 = 0,
	SECT_2,
	SECT_3,
	SECT_4,
	SECT_5,
	SECT_6,
	SECT_7,
	SECT_8,
	NUM_HUMAN_SECT,
	SECT_9 = NUM_HUMAN_SECT,		// 兽族 和 妖族 少侠？
	SECT_10,	// 九黎 大体型 兽族 
	SECT_11,	// 烈山 小体型 妖族
	SECT_12,	// 太昊 
	SECT_13,    // 怀光
	SECT_14,	// 东夷
	SECT_15,	// 天华
	NUM_SECT,
};

//	Gender
enum
{
	GENDER_MALE = 0,
	GENDER_FEMALE,
};

// Race 

enum
{
	RACE_HUMAN = 0,		// 人族
	RACE_ORC ,			// 兽族
//	RACE_EVILKIND,		// 妖族
};

// Physique

enum
{
	PHYSIQUE_NORMAL = 0,	// 正常体型，老体型
	PHYSIQUE_SMALL,			// 小体型，现在的烈山
	PHYSIQUE_MIDDLE,		// 中体型，新加的职业天华
	PHYSIQUE_BIG,           // 大体型，现在的九黎
	
	PHYSIQUE_NUM,           //多少种体型 the number of physique_type
};

//enum
//{
//	RACE_ORC = 0,	// 兽族
//	RACE_HUMAN,		// 人族
// };

//	Player name color
enum
{
	NAMECOL_RED1	= A3DCOLORRGBA(255, 183, 183, 255),
	NAMECOL_RED2	= A3DCOLORRGBA(255, 135, 135, 255),
	NAMECOL_RED3	= A3DCOLORRGBA(255, 100, 100, 255),
	NAMECOL_RED4	= A3DCOLORRGBA(255,  67,  67, 255),
	NAMECOL_RED5	= A3DCOLORRGBA(206,   0,  15, 255),
	NAMECOL_RED6	= A3DCOLORRGBA(158,   0,   0, 255),
	NAMECOL_PINK	= A3DCOLORRGBA(255,   192,   0, 255),
	NAMECOL_WHITE	= 0xffffffff,
	NAMECOL_BLUE1	= A3DCOLORRGBA(172, 209, 255, 255),
	NAMECOL_BLUE2	= A3DCOLORRGBA(116, 162, 255, 255),
	NAMECOL_BLUE3	= A3DCOLORRGBA(82,  139, 255, 255),
	NAMECOL_BLUE4	= A3DCOLORRGBA(29,  103, 255, 255),
	NAMECOL_BLUE5	= A3DCOLORRGBA(0,    40, 189, 255),
	NAMECOL_BLUE6	= A3DCOLORRGBA(0,    34, 113, 255),
	NAMECOL_MAUVE	= 0xff96c8ff,
	NAMECOL_BC_RED	= 0xffff2828,		//	Red camp in battle
	NAMECOL_BC_BLUE	= NAMECOL_MAUVE,	//	Blue camp in battle
};

//	Extend states
enum
{
	EXTST_NONE		= 0,
	NUM_EXTSTATE	= 32,
	NUM_ESBYTE		= (NUM_EXTSTATE+31) >> 5,
};

//	Player expressions
enum
{
	ROLEEXP_WAVE = 0,		//	招手
	ROLEEXP_NOD,			//	点头
	ROLEEXP_SHAKEHEAD,		//	摇头
	ROLEEXP_SHRUG,			//	耸肩膀
	ROLEEXP_LAUGH,			//	大笑

	ROLEEXP_ANGRY,			//	生气
	ROLEEXP_DEFIANCE,		//	挑衅
	ROLEEXP_VICTORY,		//	胜利
	ROLEEXP_FIGHT,			//	战斗
	ROLEEXP_SITDOWN,		//	坐下

	NUM_ROLEEXP,

	//	Not used
	ROLEEXP_STUN,			//	晕倒
	ROLEEXP_DEPRESSED,		//	沮丧
	ROLEEXP_KISSHAND,		//	飞吻
	ROLEEXP_SHY,			//	害羞
	ROLEEXP_SALUTE,			//	抱拳
	ROLEEXP_ASSAULT,		//	冲锋
	ROLEEXP_THINK,			//	思考
	ROLEEXP_GAPE,			//	伸懒腰
	ROLEEXP_ATTACK1,		//	攻击1
	ROLEEXP_ATTACK2,		//	攻击2
	ROLEEXP_ATTACK3,		//	攻击3
	ROLEEXP_ATTACK4,		//	攻击4
	ROLEEXP_DEFENCE,		//	防御
	ROLEEXP_FALL,			//	摔倒
	ROLEEXP_FALLONGROUND,	//	倒地
	ROLEEXP_LOOKAROUND,		//	张望
	ROLEEXP_DANCE,			//	舞蹈
	ROLEEXP_KISS,			// 亲吻
	ROLEEXP_TWO_KISS,		// 双人亲吻
	ROLEEXP_FIREWORK,		// 放烟花
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

#pragma pack(1)

//	Role (Player and NPC) base properties
struct ROLEBASICPROP
{
	int		iProfession;//	Profession
	int 	iLevel;		//	Level
	int		iDeityLevel;//  Deity Level
	int		iPKLevel;	//	PK level
	int		iCurHP;		//	Current HP
	int		iCurMP;		//	Current MP
	int		iCurDP;		//  Current DP
	int		iTalismanStamina;
	double	exp;		//	Experience
	double	deityExp;	//  deity experience
	int		iStatusPt;	//	Status point
};

//	Role (Player and NPC) extended properties, base part
struct ROLEEXTPROP_BASE
{
	/* 基础属性 */
	int 	max_hp;         //	最大hp
	int 	max_mp;         //	最大mp
	int		max_dp;			//  最大dp
};

//	Role (Player and NPC) extended properties, move part
struct ROLEEXTPROP_MOVE
{
	/* 运动速度*/
	float	walk_speed;     //	行走速度 单位  m/s
	float	run_speed;      //	奔跑速度 单位  m/s
};

//	Role (Player and NPC) extended properties, attack part
struct ROLEEXTPROP_ATK
{
	/* 攻击属性*/
	int 	attack;         //	攻击率 attack rate
	int 	damage_low;     //	最低damage
	int 	damage_high;    //	最大物理damage
	float	attack_range;   //	攻击范围
	int		crit_rate;		//	致命一击概率 千分数
	float	crit_damage;	//	致命一击时造成的额外伤害
};

//	Role (Player and NPC) extended properties, defense part
struct ROLEEXTPROP_DEF
{
	/* 防御属性 */
	int		resistance[NUM_MAGICCLASS];		//	魔法抗性
	int		defense;		//	防御力
	int		armor;			//	闪躲率（装甲等级）
};

enum { CULT_DEF_NUMBER = 6};

//	Role (Player and NPC) extended properties
struct ROLEEXTPROP
{
	int     max_hp;
    int     max_mp;
	int		max_dp;
    int     hp_gen[NUM_GEN_COUNT];
    int     mp_gen[NUM_GEN_COUNT];
	int		dp_gen[NUM_GEN_COUNT];
    float   walk_speed;
    float   run_speed;

    int     crit_rate;
    float   crit_damage;

    int     damage_low;
    int     damage_high;

    int     attack;
    int     defense;
    int     armor;

    int		resistance[NUM_MAGICCLASS];
    float	attack_range;
	
	int     anti_crit;              //暴击抵抗 千分数
    float   anti_crit_damage;       //暴击伤害抵抗 小数
    int     skill_attack_rate;      //技能命中概率修正  千分数
    int     skill_armor_rate;       //技能命中闪躲概率修正 千分数

	int		cult_defense[CULT_DEF_NUMBER];      //造化防御 仙 魔 佛 备选1 2 3
	int		cult_attack[CULT_DEF_NUMBER];		//造化克制 仙 魔 佛 备选1 2 3
	int     anti_transform;			//变身抗性
	int     invisible_rate;			//隐身能力
    int     anti_invisible_rate;	//反隐能力
	int		deity_power;			//神圣力
	int		anti_diet;				//禁食抗性
};

#pragma pack()

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

//	Set extend state
inline void glb_SetExtState(DWORD* p, int n)
{
	p[n >> 5] |= (1 << (n & 31));
}

//	Remove extend state
inline void glb_RemExtState(DWORD* p, int n)
{
	p[n >> 5] &= ~(1 << (n & 31));
}

//	Get extend state
inline bool glb_GetExtState(const DWORD* p, int n)
{
	return (p[n >> 5] & (1 << (n & 31))) ? true : false;
}


