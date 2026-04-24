/*
 * FILE: EC_Resource.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/18
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

//	Cursor resource
enum
{
	RES_CUR_NORMAL = 0,
	RES_CUR_ATTACK,
	RES_CUR_TALK,
	RES_CUR_PICKUP,
	RES_CUR_REPAIR,
	RES_CUR_HAND,
	RES_CUR_FLAG,
	RES_CUR_DIG,
	RES_CUR_DRAG,
	RES_CUR_REC_CUT,
	RES_CUR_REC_REVIEW,
	RES_CUR_SPLIT,
	RES_CUR_POINT,
	RES_CUR_UNABLEPOINT,
	// 挖矿时的鼠标图标
	RES_CUR_AXE,
	RES_CUR_FIRE,
	RES_CUR_GEER,
	RES_CUR_HAMMER,
	RES_CUR_HOOK,
	RES_CUR_KEY,
	RES_CUR_KNIFE,
	// 庄园光标
	RES_CUR_WEED1,
	RES_CUR_WEED2,
	RES_CUR_WATER1,
	RES_CUR_WATER2,
	RES_CUR_STEAL1,
	RES_CUR_STEAL2,
	RES_CUR_SEED1,
	RES_CUR_SEED2,
	RES_CUR_PLOT1,
	RES_CUR_PLOT2,
	RES_CUR_PEST1,
	RES_CUR_PEST2,
	RES_CUR_HARVEST1,
	RES_CUR_HARVEST2,
	RES_CUR_CHOOSE1,
	RES_CUR_CHOOSE2,
	// zoom
	RES_CUR_ZOOMIN,
	RES_CUR_ZOOMOUT,
	RES_CUR_MOVE,
	RES_CUR_DRAG1,
	NUM_RES_CURSOR,
};

//	GFX resource
enum
{
	RES_GFX_LEVELUP = 0,
	RES_GFX_PLAYERAPPEAR,
	RES_GFX_HIT,
	RES_GFX_MOVETARGET,
	RES_GFX_ITEMFLASH,
	RES_GFX_CURSORHOVER,
	RES_GFX_SELECTED,
	RES_GFX_FLOATING_DUST,
	RES_GFX_WATER_WAVE_STILL,
	RES_GFX_WATER_WAVE_MOVE,
	RES_GFX_AIR_BUBBLE,
	RES_GFX_SWIM_BUBBLE,
	RES_GFX_AIR_TRANSFORM,
	RES_GFX_HP_MEDICINE,
	RES_GFX_MP_MEDICINE,
	RES_GFX_HP_SLOW_MEDICINE,
	RES_GFX_MP_SLOW_MEDICINE,
	RES_GFX_PHYSIC_ARMORRUNE,
	RES_GFX_MAGIC_ARMORRUNE,
	RES_GFX_INDUEL,
	RES_GFX_TANKLEADER_RED,
	RES_GFX_TANKLEADER_BLUE,
	RES_GFX_TASK_FINISHED,
	RES_GFX_CHANGE_FASHION,
	RES_GFX_VIPSTATE1,
	RES_GFX_VIPSTATE2,
	RES_GFX_TRANSFIGURE,
	RES_GFX_FOOT_MALE,
	RES_GFX_FOOT_FEMALE,
	RES_GFX_SPINE_EFFECT,
	RES_GFX_DP_MEDICINE,
	RES_GFX_DP_SLOW_MEDICINE,
	RES_GFX_DEITY_LEVELUP,
	RES_GFX_DEITY_GOD,
	RES_GFX_DEITY_DEVIL,
	RES_GFX_DEITY_BUDDHA,
	NUM_RES_GFX,
};

//	Sound resource

//	Texture resource

//	Shader resource
enum
{
	RES_SHD_HAIR = 0,
	RES_SHD_GOATEE,
	RES_SHD_FACE,
	RES_SHD_EYE,
	RES_SHD_BROW,

	RES_SHD_MOUTH,	//	5
	RES_SHD_NOSE,		
	RES_SHD_EYEBALL,
	RES_SHD_MOUSTACHE,
	RES_SHD_BODY,

	NUM_RES_SHADER,	// 10
};

//	Icon resource
enum
{
	RES_ICON_CMD_SITDOWN = 0,
	RES_ICON_CMD_WALKRUN,
	RES_ICON_CMD_NORMALATTACK,
	RES_ICON_CMD_FINDTARGET,
	RES_ICON_CMD_ASSISTATTACK,

	RES_ICON_CMD_INVITETOTEAM,		//	5
	RES_ICON_CMD_LEAVETEAM,	
	RES_ICON_CMD_KICKTEAMMEM,
	RES_ICON_CMD_FINDTEAM,
	RES_ICON_CMD_STARTTRADE,

	RES_ICON_CMD_SELLBOOTH,			//	10
	RES_ICON_CMD_BUYBOOTH,
	RES_ICON_CMD_INVITETOFACTION,
	RES_ICON_CMD_FLY,
	RES_ICON_CMD_EXP_WAVE,

	RES_ICON_CMD_EXP_NOD,			//	15
	RES_ICON_CMD_EXP_SHAKEHEAD,
	RES_ICON_CMD_EXP_SHRUG,	
	RES_ICON_CMD_EXP_LAUGH,	
	RES_ICON_CMD_EXP_ANGRY,	

	RES_ICON_CMD_EXP_STUN,			//	20
	RES_ICON_CMD_EXP_DEPRESSED,
	RES_ICON_CMD_EXP_KISSHAND,
	RES_ICON_CMD_EXP_SHY,	
	RES_ICON_CMD_EXP_SALUTE,	

	RES_ICON_CMD_EXP_SITDOWN,		//	25
	RES_ICON_CMD_EXP_ASSAULT,
	RES_ICON_CMD_EXP_THINK,	
	RES_ICON_CMD_EXP_DEFIANCE,
	RES_ICON_CMD_EXP_VICTORY,

	RES_ICON_CMD_EXP_GAPE,			//	30
	RES_ICON_CMD_EXP_KISS,	
	RES_ICON_CMD_EXP_FIGHT,	
	RES_ICON_CMD_EXP_ATTACK1,
	RES_ICON_CMD_EXP_ATTACK2,

	RES_ICON_CMD_EXP_ATTACK3,		//	35
	RES_ICON_CMD_EXP_ATTACK4,
	RES_ICON_CMD_EXP_DEFENCE,
	RES_ICON_CMD_EXP_FALL,	
	RES_ICON_CMD_EXP_FALLONGROUND,

	RES_ICON_CMD_EXP_LOOKAROUND,	//	40
	RES_ICON_CMD_EXP_DANCE,
	RES_ICON_CMD_PICKUP,
	RES_ICON_CMD_GATHER,
	RES_ICON_CMD_RUSHFLY,
	
	RES_ICON_CMD_BINDBUDDY,
	RES_ICON_CMD_PET_CALL,
	RES_ICON_CMD_PET_COMBINE1,
	RES_ICON_CMD_PET_COMBINE2,
	NUM_RES_ICON,
};

//	Model resource
enum
{
	RES_MOD_GOLD = 0,
	RES_MOD_TEMP,
	RES_MOD_WARRIOR_M,
	RES_MOD_WARRIOR_F,
	RES_MOD_MAGE_M,
	RES_MOD_MAGE_F,
	RES_MOD_MONK_M,
	RES_MOD_MONK_F,
	RES_MOD_HAG_M,
	RES_MOD_HAG_F,
	RES_MOD_ORC_M,
	RES_MOD_ORC_F,
	RES_MOD_GHOST_M,
	RES_MOD_GHOST_F,
	RES_MOD_ARCHOR_M,
	RES_MOD_ARCHOR_F,
	RES_MOD_ANGEL_M,
	RES_MOD_ANGEL_F,
	RES_MOD_ORC_TIGER,
	RES_MOD_BOOTH_M,
	RES_MOD_BOOTH_F,
	NUM_RES_MODEL,
};

//	Some unicode string resources
enum
{
	RES_FONT_TITLE = 0,
	RES_FONT_DAMAGE,
	NUM_RES_FONT,
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

const char* res_CursorFile(int n);
const char* res_GFXFile(int n);
const char* res_SoundFile(int n);
const char* res_TextureFile(int n);
const char* res_ShaderFile(int n);
const char* res_IconFile(int n);
const char* res_ModelFile(int n);

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

