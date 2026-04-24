//
//
//
//-----------------------------------------------------------------------------
#pragma once

#include "AAssist.h"
#include "AArray.h"
#include "AList2.h"
#include "A3DGeometry.h"

#include "ExpTypes.h"
#include "hashmap.h"

//-----------------------------------------------------------------------------
class CECPlayer 
{
public:		//	Types

	struct SECT_LEVEL2_INFO
	{
		unsigned int mask_sect;
		int level2;
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

	typedef abase::hash_map<int, SECT_LEVEL2_INFO> SectLevel2Map;

public:	//ops
	//	Get sect info
	static SECT_LEVEL2_INFO* GetSectInfoByProf(int idProf)
	{
		SectLevel2Map::iterator it = m_SectLevel2Map.find(idProf);

		if (it == m_SectLevel2Map.end())
			return NULL;
		else
			return &it->second;
	};

	static int GetTalismanTotalExp(int iLevel)
	{
		return 0;
	};

protected:
	static SectLevel2Map	m_SectLevel2Map;

};