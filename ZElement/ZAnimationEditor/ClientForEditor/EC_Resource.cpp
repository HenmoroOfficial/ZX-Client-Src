/*
 * FILE: EC_Resource.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/18
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Resource.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


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

//	Cursor resource
static const char* l_aCurFiles[NUM_RES_CURSOR] = 
{
	"Cursors\\normal.ani",
	"Cursors\\attack.ani",
	"Cursors\\talk.cur",
	"Cursors\\pick.ani",
	"Cursors\\repair.cur",
	"Cursors\\hand.cur",
	"Cursors\\flag.cur",
	"Cursors\\dig.ani",
	"Cursors\\drag.ani",
	"Cursors\\recordcut.cur",
	"Cursors\\recordpreview.cur",
	"Cursors\\split.ani",
	"Cursors\\point.ani",
	"Cursors\\unablepoint.ani",
	// 挖矿时的鼠标图标
	"Cursors\\axe.ani",
	"Cursors\\fire.ani",
	"Cursors\\geer.ani",
	"Cursors\\hammer.ani",
	"Cursors\\hook.ani",
	"Cursors\\key.ani",
	"Cursors\\knife.ani",
	// 庄园功能图标
	"Cursors\\weed1.ani",
	"Cursors\\weed2.ani",
	"Cursors\\water1.ani",
	"Cursors\\water2.ani",
	"Cursors\\steal1.ani",
	"Cursors\\steal2.ani",
	"Cursors\\seed1.ani",
	"Cursors\\seed2.ani",
	"Cursors\\plot1.ani",
	"Cursors\\plot2.ani",
	"Cursors\\pest1.ani",
	"Cursors\\pest2.ani",
	"Cursors\\harvest1.ani",
	"Cursors\\harvest2.ani",
	"Cursors\\choose1.ani",
	"Cursors\\choose2.ani",
	// zoom
	"Cursors\\ZoomIn.cur",
	"Cursors\\ZoomOut.cur",
	"Cursors\\Drag.cur",
	"Cursors\\drag1.ani",
};

//	GFX resource
static const char* l_aGFXFiles[NUM_RES_GFX] = 
{
	"程序联入\\角色升级人类.gfx",	//	RES_GFX_LEVELUP,
	"程序联入\\角色出现人类.gfx",	//	RES_GFX_PLAYERAPPEAR,
	"被击中\\被击中效果.gfx",		//	RES_GFX_HIT,
	"程序联入\\指向标.gfx",			//	RES_GFX_MOVETARGET,
	"程序联入\\金币效果.gfx",		//	RES_GFX_ITEMFLASH,
	"程序联入\\鼠标悬浮.gfx",		//	RES_GFX_CURSORHOVER,
	"程序联入\\目标被选中.gfx",		//	RES_GFX_SELECTED,
	"程序联入\\海底飘尘.gfx",		//	RES_GFX_FLOATING_DUST,
	"程序联入\\人物游动水圈.gfx",	//	RES_GFX_WATER_WAVE_STILL,
	"程序联入\\人物游动水波.gfx",	//	RES_GFX_WATER_WAVE_MOVE,
	"程序联入\\水中呼吸气泡.gfx",	//	RES_GFX_AIR_BUBBLE,
	"程序联入\\水中游动气泡.gfx",	//	RES_GFX_SWIM_BUBBLE,
	"程序联入\\白虎变.gfx",			//	RES_GFX_AIR_TRANSFORM,
	"程序联入\\hp瞬回.gfx",			//	RES_GFX_HP_MEDICINE,
	"程序联入\\mp瞬回.gfx",			//	RES_GFX_MP_MEDICINE,
	"程序联入\\hp持续.gfx",			//	RES_GFX_HP_SLOW_MEDICINE,
	"程序联入\\mp持续.gfx",			//	RES_GFX_MP_SLOW_MEDICINE,
	"程序联入\\物防符释放.gfx",		//	RES_GFX_PHYSIC_ARMORRUNE,
	"程序联入\\法防符释放.gfx",		//	RES_GFX_MAGIC_ARMORRUNE,
	"程序联入\\决斗状态.gfx",		//	RES_GFX_INDUEL,
	"程序联入\\人物头上旗子.gfx",	//	RES_GFX_TANKLEADER_RED,
	"程序联入\\人物头上旗子.gfx",	//	RES_GFX_TANKLEADER_BLUE,
	"程序联入\\一般任务完成.gfx",	//	RES_GFX_TASK_FINISHED,
	"程序联入\\角色换装.gfx",		//  RES_GFX_CHANGE_FASHION,
	"程序联入\\vip1.gfx",			//  RES_GFX_VIPSTATE1,
	"程序联入\\vip2.gfx",			//  RES_GFX_VIPSTATE2,
	"程序联入\\角色变身技能.gfx",	//  RES_GFX_TRANSFIGURE,
	"程序联入\\天华脚下效果_男.gfx",//  RES_GFX_FOOT_MALE,
	"程序联入\\天华脚下效果_女.gfx",//  RES_GFX_FOOT_FEMALE,
	"程序联入\\怀光拖影轨迹.gfx",	//  RES_GFX_SPINE_EFFECT,
	"程序联入\\dp瞬回.gfx",			//	RES_GFX_DP_MEDICINE,
	"程序联入\\dp持续.gfx",			//	RES_GFX_DP_SLOW_MEDICINE,
	"程序联入\\角色升级封神.gfx",	//	RES_GFX_DEITY_LEVELUP,
	"程序联入\\仙_星辰之光.gfx",	//	RES_GFX_DEITY_GOD,
	"程序联入\\魔_肃杀之光.gfx",	//	RES_GFX_DEITY_DEVIL,
	"程序联入\\佛_和谐之光.gfx",	//	RES_GFX_DEITY_BUDDHA,
	
};

//	Shader resource
static const char* l_aShaderFiles[NUM_RES_SHADER] =
{
	"Hair.sdr",		//	0
	"Goatee.sdr",
	"Face.sdr",
	"Eye.sdr",
	"Brow.sdr",

	"Mouth.sdr",	//	5
	"Nose.sdr",		
	"EyeBall.sdr",
	"Moustache.sdr",
	"Body.sdr"
};

//	Icon files
static const char* l_aIconFiles[NUM_RES_ICON] = 
{
	"打坐.dds",
	"走跑转换.dds",
	"普通攻击.dds",
	"寻找目标.dds",
	"协助攻击.dds",

	"邀请加入.dds",
	"脱离队伍.dds",
	"踢出队伍.dds",
	"寻找队伍.dds",
	"交易命令.dds",

	"摆摊卖.dds",
	"摆摊买.dds",
	"邀请加入.dds",
	"飞行.dds",
	"招手.dds",		//	RES_ICON_CMD_EXP_WAVE,

	"点头.dds",		//	RES_ICON_CMD_EXP_NOD,
	"摇头.dds",		//	RES_ICON_CMD_EXP_SHAKEHEAD,
	"耸肩膀.dds",	//	RES_ICON_CMD_EXP_SHRUG,	
	"大笑.dds",		//	RES_ICON_CMD_EXP_LAUGH,	
	"生气.dds",		//	RES_ICON_CMD_EXP_ANGRY,	

	"晕倒.dds",		//	RES_ICON_CMD_EXP_STUN,	
	"沮丧.dds",		//	RES_ICON_CMD_EXP_DEPRESSED,
	"飞吻.dds",		//	RES_ICON_CMD_EXP_KISSHAND,
	"害羞.dds",		//	RES_ICON_CMD_EXP_SHY,	
	"抱拳.dds",		//	RES_ICON_CMD_EXP_SALUTE,	

	"坐下.dds",		//	RES_ICON_CMD_EXP_SITDOWN,
	"冲锋.dds",		//	RES_ICON_CMD_EXP_ASSAULT,
	"思考.dds",		//	RES_ICON_CMD_EXP_THINK,	
	"挑衅.dds",		//	RES_ICON_CMD_EXP_DEFIANCE,
	"胜利.dds",		//	RES_ICON_CMD_EXP_VICTORY,

	"伸懒腰.dds",	//	RES_ICON_CMD_EXP_GAPE
	"亲吻.dds",		//	RES_ICON_CMD_EXP_KISS
	"战斗.dds",		//	RES_ICON_CMD_EXP_FIGHT,	
	"攻击1.dds",	//	RES_ICON_CMD_EXP_ATTACK1,
	"攻击2.dds",	//	RES_ICON_CMD_EXP_ATTACK2,

	"攻击3.dds",	//	RES_ICON_CMD_EXP_ATTACK3,
	"攻击4.dds",	//	RES_ICON_CMD_EXP_ATTACK4,
	"防御.dds",		//	RES_ICON_CMD_EXP_DEFENCE,
	"摔倒.dds",		//	RES_ICON_CMD_EXP_FALL,	
	"倒地.dds",		//	RES_ICON_CMD_EXP_FALLONGROUND,

	"张望.dds",		//	RES_ICON_CMD_EXP_LOOKAROUND,
	"舞蹈1.dds",	//	RES_ICON_CMD_EXP_DANCE,
	"拾取.dds",		//	RES_ICON_CMD_PICKUP
	"挖掘.dds",		//	RES_ICON_CMD_GATHER
	"加速飞行.dds",	//	RES_ICON_CMD_RUSHFLY

	"攻击1.dds",	//	RES_ICON_CMD_BINDBUDDY
	"召唤解散宠物.dds",
	"宠物通灵.dds",
	"宠物御宝.dds",
};

//	Model files
static const char* l_aModelFiles[NUM_RES_MODEL] =
{
	"Models\\Matters\\物品\\金币\\金币.ecm",
	"Models\\Matters\\物品\\元晶\\元晶.ecm",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
	"Models\\Players\\形象\\女\\躯干\\女.ecm",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
	"Models\\Players\\形象\\女\\躯干\\女.ecm",
	"",
	"",
	"",
	"Models\\Players\\形象\\女\\躯干\\女.ecm",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
	"",
	"",
	"",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
	"Models\\Players\\形象\\女\\躯干\\女.ecm",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
	"Models\\Players\\形象\\女\\躯干\\女.ecm",
	"Models\\Players\\形象\\白虎\\白虎.ecm",
	"Models\\Players\\形象\\摆摊\\男\\男.smd",
	"Models\\Players\\形象\\摆摊\\女\\女.smd",
};

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

const char* res_CursorFile(int n)
{
	ASSERT(n >= 0 && n < NUM_RES_CURSOR);
	return l_aCurFiles[n];
}

const char* res_GFXFile(int n)
{
	ASSERT(n >= 0 && n < NUM_RES_GFX);
	return l_aGFXFiles[n];
}

const char* res_SoundFile(int n)
{
	return NULL;
}

const char* res_TextureFile(int n)
{
	return NULL;
}

const char* res_ShaderFile(int n)
{
	ASSERT(n >= 0 && n < NUM_RES_SHADER);
	return l_aShaderFiles[n];
}

const char* res_IconFile(int n)
{
	ASSERT(n >= 0 && n < NUM_RES_ICON);
	return l_aIconFiles[n];
}

const char* res_ModelFile(int n)
{
	ASSERT(n >= 0 && n < NUM_RES_MODEL);
	return l_aModelFiles[n];
}
