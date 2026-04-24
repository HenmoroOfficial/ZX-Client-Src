/*
 * FILE: EC_Player.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Model.h"
#include "EC_Player.h"
#include "EC_RTDebug.h"
#include "EC_IvtrTypes.h"
//#include "EC_IvtrItem.h"
//#include "EC_IvtrScroll.h"
#include "EC_Resource.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#endif
#include "EC_Utility.h"
#include "EC_Decal.h"
#include "EC_ManDecal.h"
#include "EC_World.h"
#include "EC_GameRun.h"
#include "EC_ImageRes.h"
#include "EC_GFXCaster.h"
#include "EC_Resource.h"
#include "EC_FixedMsg.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_ElsePlayer.h"
//#include "EC_Team.h"
//#include "EC_Faction.h"
//#include "EC_Skill.h"
//#include "ElementSkill.h"
#include "EC_PortraitRender.h"
//#include "EC_PateText.h"
#include "EC_Configs.h"
//#include "EC_GameSession.h"
#include "EC_SceneLoader.h"
//#include "FWAssemblySet.h"
//#include "FWTemplate.h"
#include "EC_NPC.h"
#include "EC_Sprite.h"
#include "EC_ManNPC.h"
//#include "EC_GameUIMan.h"
//#include "EC_UIManager.h"
#include "EC_Utility.h"
#include "A3DSkillGfxComposer.h"
#include "A3DLight.h"

//#include "EC_ChangePill.h"
//#include "EC_Achievement.h"
//#include "EC_Circle.h"

#include "elementdataman.h"
// #include "privilege.hxx"
// #include "ids.hxx"

#include "A2DSprite.h"
#include "A2DSpriteItem.h"
#include "A3DCombinedAction.h"
#include "A3DSkin.h"
#include "A3DSkinMan.h"
#include "A3DShaderMan.h"
#include "A3DSkinMesh.h"
#include "A3DShader.h"
#include "A3DSkinModel.h"
#include "A3DSkinModelAct.h"
#include "A3DSkinModelAux.h"
#include "A3DTerrainWater.h"
#include "A3DCamera.h"
#include "A3DSkeleton.h"
#include "A3DBone.h"
#include "A3DFlatCollector.h"
#include "AAssist.h"
#include "AFileImage.h"
#include "A3DConfig.h" //Added 2011-06-28.

//#include "ConfigFromLua.h"
//#include "LuaEvent.h"

#define new A_DEBUG_NEW

#define WEAPON_POWER	15.0f
#define WEAPON_SPECULAR	0xffffffff

#define ACTION_SWITCH_INDEX	13    // 普通动作与骑乘动作分界线

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
// 注：在老职业中，武器都是右手的，在新职业烈山中，武器弓是在左手的，但是统一都使用了
//HH_Weapon作为挂点，所以在新加的怀光双手武器中，也是认定以前的HH_Weapon是左手的，
//老的职业中，仍然认为HH_Weapon 是右手的武器和右手的挂点
static const char* _weapon_hanger		= "Weapon";
static const char* _right_weapon_hanger = "RightWeapon";
static const char* _back_weapon_hanger	= "BackWeapon";	//背部武器的hanger标示名字
static const char* _wing				= "Wing";
static const char* _cc_ride				= "CC_Ride";
static const char* _cc_ride2			= "CC_Ride2";
static const char* _hh_ride				= "HH_Ride";
static const char* _hanger_ride			= "Rider";
static const char* _hanger_ride2		= "Rider2";
static const char* _hanger_hug			= "Hug";
static const char* _hh_bind				= "HH_Bind";
static const char* _cc_bind[4]			=
{
	"CC_Bind",
	"CC_Bind_xiao",
	"CC_Bind",
	"CC_Bind_da",
};

// hanger name
static const char* _multi_hanger_name[6] =
{
	"Rider",
	"Rider1",
	"Rider2",
	"Rider3",
	"Rider4",
	"Rider5",
};

// 多人骑乘 挂点名字
static const char* _multi_hh_ride[6] = 
{
	"HH_Ride",
	"HH_Ride01",
	"HH_Ride02",
	"HH_Ride03",
	"HH_Ride04",
	"HH_Ride05",
};

static const char* _hh_fly[4] =
{
	"HH_feijian",
	"HH_chibang",
	"HH_feiqi",
	"HH_ride",
};

static const char* _cc_fly[4] =
{
	"CC_feijian",
	"CC_chibang",
	"CC_feiqi",
	"CC_ride",
};

const char* _ear_skin[2] = 
{
	"Models\\Players\\形象\\小体型男\\特征\\烈山耳朵男%d%d.ski",	
	"Models\\Players\\形象\\小体型女\\特征\\烈山耳朵女%d%d.ski",
};

const char* _tail_skin[2] =
{
	"Models\\Players\\形象\\小体型男\\特征\\烈山尾巴男%d%d.ski",	
	"Models\\Players\\形象\\小体型女\\特征\\烈山尾巴女%d%d.ski",
};

const char* _optimal_ear_skin[2] = 
{
	"Models\\Players\\时装\\特征\\高级耳朵男%02d.ski",
	"Models\\Players\\时装\\特征\\高级耳朵女%02d.ski",	
};

const char* _optimal_tail_skin[2] =
{
	"Models\\Players\\时装\\特征\\高级尾巴男%02d.ski",
	"Models\\Players\\时装\\特征\\高级尾巴女%02d.ski",
};

const char* _head_skin[4][2] = 
{
	{"Models\\Players\\形象\\男\\头\\男头%02d.ski",	"Models\\Players\\形象\\女\\头\\女头%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头\\小体型男头%02d.ski",	"Models\\Players\\形象\\小体型女\\头\\小体型女头%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头\\中体型男头%02d.ski",	"Models\\Players\\形象\\中体型女\\头\\中体型女头%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头\\大体型男头%02d.ski",	"Models\\Players\\形象\\大体型女\\头\\大体型女头%02d.ski"},
};

const char* _hair_skin[4][2] =
{
	{"Models\\Players\\形象\\男\\头发\\男头发%02d.ski",	"Models\\Players\\形象\\女\\头发\\女头发%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头发\\小体型男头发%02d.ski",	"Models\\Players\\形象\\小体型女\\头发\\小体型女头发%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头发\\中体型男头发%02d.ski",	"Models\\Players\\形象\\中体型女\\头发\\中体型女头发%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头发\\大体型男头发%02d.ski",	"Models\\Players\\形象\\大体型女\\头发\\大体型女头发%02d.ski"},
};

static const char* _equipment_default_body_skin[4][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_shoe_skin[4][2] = 
{
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
};

static const char* _equipment_default_fashion_upper_skin[4][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_fashion_lower_skin[4][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_fashion_shoe_skin[4][2] = 
{
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
};

static const char* _changed_shape_path[] =
{
	"Models\\Players\\形象\\男鬼道魔魂形态\\男鬼道魔魂形态.ecm",
	"Models\\Players\\形象\\女鬼道魔魂形态\\女鬼道魔魂形态.ecm",
	"Models\\Players\\形象\\男鬼道蛊王形态\\男鬼道蛊王形态.ecm",
	"Models\\Players\\形象\\女鬼道蛊王形态\\女鬼道蛊王形态.ecm",
	"Models\\Npcs\\怪物\\野兽\\主角狐狸变身\\烈山小狐狸\\烈山小狐狸.ecm",
	"Models\\Npcs\\怪物\\野兽\\主角狐狸变身\\烈山中狐狸\\烈山中狐狸.ecm",
	"Models\\Npcs\\怪物\\野兽\\主角狐狸变身\\烈山大狐狸\\烈山大狐狸.ecm",
	"Models\\Npcs\\怪物\\元素\\竞技场怪物a\\怀光黑雾怪.ecm",
	"Models\\Npcs\\怪物\\元素\\焚香火焰状态\\焚香火焰状态.ecm",
};

static const char* _skin_model_path[4][2] =
{
	{"Models\\Players\\形象\\男\\躯干\\男.smd",		"Models\\Players\\形象\\女\\躯干\\女.smd"},
	{"Models\\Players\\形象\\小体型男\\躯干\\小体型男.smd",		"Models\\Players\\形象\\小体型女\\躯干\\小体型女.smd"},
	{"Models\\Players\\形象\\中体型男\\躯干\\中体型男.smd",		"Models\\Players\\形象\\中体型女\\躯干\\中体型女.smd"},
	{"Models\\Players\\形象\\大体型男\\躯干\\大体型男.smd",		"Models\\Players\\形象\\大体型女\\躯干\\大体型女.smd"},
};

static abase::vector<int> _male_eye_joint_indice;
static abase::vector<int> _female_eye_joint_indice;

static float r = 0.1f;
static float g = 0.0f;
static float b = 0.0f;

static A3DCOLORVALUE l_ecolors[] = 
{
 	A3DCOLORVALUE(0.0f,	0.0f, 0.0f, 1.0f),
	A3DCOLORVALUE(0.15f, 0.15f, 0.25f, 1.0f),
	A3DCOLORVALUE(0.15f, 0.25f, 0.15f, 1.0f),
	A3DCOLORVALUE(0.20f, 0.20f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.25f, 0.15f, 0.15f, 1.0f),
	A3DCOLORVALUE(0.20f, 0.00f, 0.20f, 1.0f),
	A3DCOLORVALUE(0.00f, 0.10f, 0.30f, 1.0f),
	A3DCOLORVALUE(0.00f, 0.00f, 0.40f, 1.0f),
	A3DCOLORVALUE(0.00f, 0.30f, 0.30f, 1.0f),
	A3DCOLORVALUE(0.00f, 0.30f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.25f, 0.40f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.50f, 0.30f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.50f, 0.20f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.45f, 0.05f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.50f, 0.00f, 0.00f, 1.0f),
	A3DCOLORVALUE(0.40f, 0.20f, 0.40f, 1.0f),
	A3DCOLORVALUE(1.00f, 0.05f, 0.30f, 1.0f),
	A3DCOLORVALUE(0.35f, 0.35f, 1.00f, 1.0f),
	A3DCOLORVALUE(1.00f, 0.35f, 0.35f, 1.0f),
	A3DCOLORVALUE(0.30f, 1.00f, 0.30f, 1.0f),
	A3DCOLORVALUE(0.35f, 0.35f, 0.35f, 1.0f),
};

const int fly_act[3] =
{	
	CECPlayer::ACT_FLY_SWORD,
	CECPlayer::ACT_FLY_WING,
	CECPlayer::ACT_FLY_FEIQI,
};

const int fly_hang_act[3] =
{
	CECPlayer::ACT_HANGINAIR_SWORD,
	CECPlayer::ACT_HANGINAIR_WING,
	CECPlayer::ACT_HANGINAIR_FEIQI,
};

const int fly_takeoff_act[3] = 
{
	CECPlayer::ACT_TAKEOFF_SWORD,
	CECPlayer::ACT_TAKEOFF_WING,
	CECPlayer::ACT_TAKEOFF_FEIQI,
};

const int fly_low_fall_act[3] = 
{
	CECPlayer::ACT_FLYDOWN_SWORD_LOW,
	CECPlayer::ACT_FLYDOWN_WING_LOW,
	CECPlayer::ACT_FLYDOWN_FEIQI_LOW,
};

const int fly_high_fall_act[3] = 
{
	CECPlayer::ACT_FLYDOWN_SWORD_HIGH,
	CECPlayer::ACT_FLYDOWN_WING_HIGH,
	CECPlayer::ACT_FLYDOWN_FEIQI_HIGH,
};

#ifndef SIZE_OF_ARRAY
#define SIZE_OF_ARRAY(x) (sizeof(x) / sizeof((x)[0]))
#endif

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

inline void _ReleaseLoadModel(EC_PLAYERLOADRESULT& Ret)
{
	if (Ret.pPlayerModel)
	{
		Ret.pPlayerModel->Release();
		delete Ret.pPlayerModel;
	}

	if (Ret.pDummyModel)
	{
		Ret.pDummyModel->Release();
		delete Ret.pDummyModel;
	}

	if (Ret.pPetModel)
	{
		Ret.pPetModel->Release();
		delete Ret.pPetModel;
	}

	for (int i = 0; i < NUM_SKIN_INDEX; i++)
	{
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][0]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][1]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[i][2]);
	}
}

//	Build pate faction text
static void _BuildPateFactionText(int idFaction, int iRank, ACString& strText)
{
	strText.Empty();

	if (!idFaction)
		return;


}

static void _BuildPateCircleText(int idCircle, int iRank, ACString& strText)
{
	strText.Empty();
}

static int GetMappedAction(int iAction)
{
	int iMappedAct = iAction;

	switch(iAction)
	{
	case CECPlayer::ACT_STAND:
		iMappedAct = CECNPC::ACT_STAND;
		break;

	case CECPlayer::ACT_FIGHTSTAND:
		iMappedAct = CECNPC::ACT_GUARD;
		break;

	case CECPlayer::ACT_WALK: 
		iMappedAct = CECNPC::ACT_WALK;
		break;

	case CECPlayer::ACT_RUN:
		iMappedAct = CECNPC::ACT_RUN;
		break;

	case CECPlayer::ACT_JUMP_START:
		iMappedAct = CECNPC::ACT_JUMP_START;
		break;

	case CECPlayer::ACT_JUMP_LOOP:
		iMappedAct = CECNPC::ACT_JUMP_LOOP;
		break;

	case CECPlayer::ACT_JUMP_LAND:
		iMappedAct = CECNPC::ACT_JUMP_LAND;
		break;

	case CECPlayer::ACT_GROUNDDIE:
		iMappedAct = CECNPC::ACT_DIE;
		break;

	case CECPlayer::ACT_GROUNDDIE_LOOP:
		iMappedAct = CECNPC::ACT_DIE;
		break;

	case CECPlayer::ACT_WATERDIE:
		iMappedAct = CECNPC::ACT_DIE;
		break;

	case CECPlayer::ACT_WATERDIE_LOOP:
		iMappedAct = CECNPC::ACT_DIE;
		break;

	case CECPlayer::ACT_ATTACK_1:
		iMappedAct = CECNPC::ACT_ATTACK1;
		break;

	case CECPlayer::ACT_ATTACK_2:
		iMappedAct = CECNPC::ACT_ATTACK2;
		break;

	case CECPlayer::ACT_ATTACK_3:
		iMappedAct = CECNPC::ACT_ATTACK1;
		break;

	case CECPlayer::ACT_ATTACK_4:
		iMappedAct = CECNPC::ACT_ATTACK2;
		break;
    case CECPlayer::ACT_WOUNDED:
		iMappedAct = CECNPC::ACT_WOUNDED;
		break;
	case CECPlayer::ACT_SWIM:
		iMappedAct = CECNPC::ACT_RUN;
		break;

	default:
		iMappedAct = CECNPC::ACT_STAND;
	}

	return iMappedAct;
}

const int	CECPlayer::FACEHAIRTYPE_NUM = 11;

int CECPlayer::GetReputationLevel(int nReputation)
{
	int level = 0;
	if( nReputation < 5000 )
		level = 0;
	else if( nReputation < 25000 )
		level = 1;
	else if( nReputation < 50000 )
		level = 2;
	else if( nReputation < 100000 )
		level = 3;
	else if( nReputation < 200000 )
		level = 4;
	else if( nReputation < 500000 )
		level = 5;
	else if( nReputation < 1000000 )
		level = 6;
	else if( nReputation < 2000000 )
		level = 7;
	else if( nReputation < 5000000 )
		level = 8;
	else
		level = 9;

	return level;
}

void CECPlayer::ChangeDefaultUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin)
{
	AString str;
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);
	if (!pInfo)
	{
		ASSERT(0);
		return;
	}
	if ( (pInfo->mask_sect&(1<<SECT_11)) || (pInfo->mask_sect&(1<<SECT_13)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_11))  // 烈山默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "烈山", nSkin+1);
		else								// 怀光默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "怀光", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "太昊", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_body_skin[iPhysique][nGender], nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
}

void CECPlayer::ChangeDefaultFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin)
{
	AString str;
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);
	if (!pInfo)
	{
		ASSERT(0);
		return;
	}
	if ( (pInfo->mask_sect&(1<<SECT_11)) || (pInfo->mask_sect&(1<<SECT_13)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_11))  // 烈山默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "烈山", nSkin+1);
		else								// 怀光默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "怀光", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "太昊", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_shoe_skin[iPhysique][nGender], nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
}

void CECPlayer::ChangeDefaultFashionUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin)
{
	AString str;
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);
	if (!pInfo)
	{
		ASSERT(0);
		return;
	}
	if ( (pInfo->mask_sect&(1<<SECT_11)) || (pInfo->mask_sect&(1<<SECT_13)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_11))  // 烈山默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "烈山", nSkin+1);
		else								// 怀光默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "怀光", nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "太昊", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
}

void CECPlayer::ChangeDefaultFashionLower(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin)
{
	AString str;
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);
	if (!pInfo)
	{
		ASSERT(0);
		return;
	}
	if ( (pInfo->mask_sect&(1<<SECT_11)) || (pInfo->mask_sect&(1<<SECT_13)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_11))  // 烈山默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "烈山", nSkin+1);
		else								// 怀光默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "怀光", nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "太昊", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
}

void CECPlayer::ChangeDefaultFashionFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin)
{
	AString str;
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);
	if (!pInfo)
	{
		ASSERT(0);
		return;
	}
	if ( (pInfo->mask_sect&(1<<SECT_11)) || (pInfo->mask_sect&(1<<SECT_13)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_11))  // 烈山默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "烈山", nSkin+1);
		else								// 怀光默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "怀光", nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "太昊", nSkin+1);
		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_shoe_skin[iPhysique][nGender], nSkin+1);
		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
}

bool CECPlayer::LoadPlayerEquips(
	int iPhysique,
	int iProfession,
	int iDefaultSkin, 
	int iGender,
	unsigned int uMask,
	const int* pEquips,
	EquipsLoadResult& Ret)
{
	memset(&Ret, 0, sizeof(Ret));
	memset(&Ret.aEuips, 0xff, sizeof(Ret.aEuips));

	ShowEquipments(
		iPhysique,
		iProfession,
		iDefaultSkin,
		iGender,
		false,
		pEquips,
		uMask,
		&Ret
	);

	return true;
}

//	Load player model
bool CECPlayer::LoadPlayerModel(
	int iPhysique,
	int iProfession,
	int iLevel,
	int iEarid,
	int iTailid,
	int	iFaceid,
	int	iHairid,
	int iDefaultSkin,
	int iGender,
	const int* pEquips,
	const char* szPetPath,
	const char* szChangedPath,
	EC_PLAYERLOADRESULT& Ret)
{
	static const char* aModelFiles[4][2] =
	{
		{"Models\\Players\\形象\\男\\躯干\\男.ecm",		"Models\\Players\\形象\\女\\躯干\\女.ecm"},
		{"Models\\Players\\形象\\小体型男\\躯干\\小体型男.ecm",		"Models\\Players\\形象\\小体型女\\躯干\\小体型女.ecm"},	
		{"Models\\Players\\形象\\中体型男\\躯干\\中体型男.ecm",		"Models\\Players\\形象\\中体型女\\躯干\\中体型女.ecm"},	
		{"Models\\Players\\形象\\大体型男\\躯干\\大体型男.ecm",		"Models\\Players\\形象\\大体型女\\躯干\\大体型女.ecm"},	
	};

	memset(&Ret, 0, sizeof(Ret));
	memset(&Ret.EquipResult.aEuips, 0xff, sizeof(Ret.EquipResult.aEuips));

	//	Load player model ----------------------------
	if (!(Ret.pPlayerModel = new CECModel))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::LoadPlayerModel", __LINE__);
		_ReleaseLoadModel(Ret);
		return false;
	}

	//	Load skeleton without skins
	const char* szFile = aModelFiles[iPhysique][iGender];
	if (!Ret.pPlayerModel->Load(szFile, true, A3DSkinModel::LSF_NOSKIN, /*true*/false))
	{
		a_LogOutput(1, "CECPlayer::LoadPlayerModel, Failed to load player model %s", szFile);
		_ReleaseLoadModel(Ret);
		return false;
	}

	A3DSkinModel* pA3DModel = Ret.pPlayerModel->GetA3DSkinModel();

	if (!pA3DModel)
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	// build action channels
	A3DSMActionChannel* pChannel;

	if (iGender == 0) // male
	{
		// body
		pChannel = pA3DModel->BuildActionChannel(
			ACT_CHANNEL_BODY,
			_male_eye_joint_indice.size(),
			_male_eye_joint_indice.begin(),
			true);

		if (!pChannel)
		{
			_ReleaseLoadModel(Ret);
			return false;
		}

		pChannel->SetWeight(1.0f);

		// eye
		pChannel = pA3DModel->BuildActionChannel(
			ACT_CHANNEL_EYE,
			_male_eye_joint_indice.size(),
			_male_eye_joint_indice.begin());

		if (!pChannel)
		{
			_ReleaseLoadModel(Ret);
			return false;
		}

		pChannel->SetWeight(1.0f);
	}
	else
	{
		// body
		pChannel = pA3DModel->BuildActionChannel(
			ACT_CHANNEL_BODY,
			_female_eye_joint_indice.size(),
			_female_eye_joint_indice.begin(),
			true);

		if (!pChannel)
		{
			_ReleaseLoadModel(Ret);
			return false;
		}

		pChannel->SetWeight(1.0f);

		// eye
		pChannel = pA3DModel->BuildActionChannel(
			ACT_CHANNEL_EYE,
			_female_eye_joint_indice.size(),
			_female_eye_joint_indice.begin());

		if (!pChannel)
		{
			_ReleaseLoadModel(Ret);
			return false;
		}

		pChannel->SetWeight(1.0f);
	}

	// set action event mask
	Ret.pPlayerModel->SetActionEventMask(ACT_CHANNEL_EYE, EVENT_MASK_MATCHG);
	
	// wound channel
	A3DSkeleton* pSkeleton = pA3DModel->GetSkeleton();
	int iNumJoint = pSkeleton->GetJointNum();
	int* aJoints = new int [iNumJoint];

	int i;
	for (i = 0; i < iNumJoint; i++)
		aJoints[i] = i;

	pChannel = pA3DModel->BuildActionChannel(
		ACT_CHANNEL_WOUND,
		iNumJoint,
		aJoints);

	delete[] aJoints;

	if (!pChannel)
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	pChannel->SetPlayMode(A3DSMActionChannel::PLAY_COMBINE);

	for (i = 0; i < NUM_SKIN_INDEX; i++)
		pA3DModel->AddSkin(NULL, false);

	char szPath[MAX_PATH];
	//  对于头型和发型的序号修正
	SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(iProfession);

	sprintf(szPath, _head_skin[iPhysique][iGender], FaceHairId2FileIndex(iFaceid, iProfession));
	A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);
	pA3DModel->ReplaceSkin(SKIN_HEAD_INDEX, pHeadSkin, false);

	sprintf(szPath, _hair_skin[iPhysique][iGender], FaceHairId2FileIndex(iHairid, iProfession));
	A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(szPath, true);
	pA3DModel->ReplaceSkin(SKIN_HAIR_INDEX, pHairSkin, false);

	int level = iLevel / 45;
	if (iPhysique == PHYSIQUE_SMALL)
	{

		if (pInfo && (pInfo->mask_sect&(1<<SECT_11)))
		{
			// 耳朵的序号
			if (iEarid & 0x80)
				sprintf(szPath, _optimal_ear_skin[iGender], (iEarid & 0x7f) +1);
			else
				sprintf(szPath, _ear_skin[iGender], level+1, iEarid+1 );
			A3DSkin* pEarSkin = g_pGame->LoadA3DSkin(szPath, true);
			pA3DModel->ReplaceSkin(SKIN_EAR_INDEX, pEarSkin, false);
			// 尾巴的序号
			if (iTailid & 0x80)
				sprintf(szPath, _optimal_tail_skin[iGender], (iTailid & 0x7f)+1);
			else
				sprintf(szPath, _tail_skin[iGender], level+1, iTailid+1 );
			A3DSkin* pTailSkin = g_pGame->LoadA3DSkin(szPath, true);
			pA3DModel->ReplaceSkin(SKIN_TAIL_INDEX, pTailSkin, false);
		}
	}

	if (pEquips)
	{
		ShowEquipments(
			iPhysique,
			iProfession,
			iDefaultSkin,
			iGender,
			false,
			pEquips,
			EQUIP_MASK_ALL,
			&Ret.EquipResult
		);
	}

	Ret.pPortraitModel = g_pGame->LoadA3DSkinModel(_skin_model_path[iPhysique][iGender], A3DSkinModel::LSF_NOSKIN);

	if (!Ret.pPortraitModel)
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	for (i = 0; i < NUM_PORTAIT_SKIN; i++)
		Ret.pPortraitModel->AddSkin(NULL, false);

	sprintf(szPath, _head_skin[iPhysique][iGender], FaceHairId2FileIndex(iFaceid, iProfession));
	pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);
	Ret.pPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HEAD, pHeadSkin, false);

	sprintf(szPath, _hair_skin[iPhysique][iGender], FaceHairId2FileIndex(iHairid, iProfession));
	pHairSkin = g_pGame->LoadA3DSkin(szPath, true);
	Ret.pPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HAIR, pHairSkin, false);

	if (szPetPath && szPetPath[0] && !LoadPetModel(szPetPath, Ret))
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	if (szChangedPath && szChangedPath[0] && !LoadChangedModel(szChangedPath, Ret))
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	return true;
}

bool CECPlayer::LoadPetModel(const char* szPetPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pPetModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	if (!Ret.pPetModel->Load(szPetPath, true, A3DSkinModel::LSF_NOSKIN, false))
	{
		delete Ret.pPetModel;
		Ret.pPetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}

	char szSkin[MAX_PATH];
	strncpy(szSkin, szPetPath, MAX_PATH);
	glb_ChangeExtension(szSkin, "ski");
	A3DSkin * pPetSkin = g_pGame->LoadA3DSkin(szSkin, false);
	if( !pPetSkin )
	{
		Ret.pPetModel->Release();
		delete Ret.pPetModel;
		Ret.pPetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}
	Ret.pPetModel->GetA3DSkinModel()->AddSkin(pPetSkin, true);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

bool CECPlayer::LoadChangedModel(const char* szPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pChangedModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	if (!Ret.pChangedModel->Load(szPath, true, A3DSkinModel::LSF_NOSKIN, false))
	{
		delete Ret.pChangedModel;
		Ret.pChangedModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}

	char szSkin[MAX_PATH];
	strncpy(szSkin, szPath, MAX_PATH);
	glb_ChangeExtension(szSkin, "ski");
	A3DSkin * pSkin = g_pGame->LoadA3DSkin(szSkin, false);
	if( !pSkin )
	{
		Ret.pChangedModel->Release();
		delete Ret.pChangedModel;
		Ret.pChangedModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}
	Ret.pChangedModel->GetA3DSkinModel()->AddSkin(pSkin, true);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

bool CECPlayer::LoadTransfiguredModel(const char * szPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pTransfiguredModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	if (!Ret.pTransfiguredModel->Load(szPath, true, A3DSkinModel::LSF_NOSKIN, false))
	{
		delete Ret.pTransfiguredModel;
		Ret.pTransfiguredModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}

	char szSkin[MAX_PATH];
	strncpy(szSkin, szPath, MAX_PATH);
	glb_ChangeExtension(szSkin, "ski");
	A3DSkin * pSkin = g_pGame->LoadA3DSkin(szSkin, false);
	if( !pSkin )
	{
		Ret.pTransfiguredModel->Release();
		delete Ret.pTransfiguredModel;
		Ret.pTransfiguredModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}
	Ret.pTransfiguredModel->GetA3DSkinModel()->AddSkin(pSkin, true);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;	
}

bool CECPlayer::LoadAircraftModel(const char* szPath, EC_PLAYERLOADRESULT& ret)
{
	ret.pAircraftModel = new CECModel();

	if (!ret.pAircraftModel->Load(szPath))
	{
		delete ret.pAircraftModel;
		ret.pAircraftModel = 0;
		return false;
	}

	return true;
}

//	Release player model
void CECPlayer::ReleasePlayerModel(EC_PLAYERLOADRESULT& Ret)
{
	if (Ret.pPlayerModel)
	{
		A3DSkin* pSkin;
		A3DSkinModel* pSkinModel = Ret.pPlayerModel->GetA3DSkinModel();
		pSkin = pSkinModel->GetA3DSkin(SKIN_HEAD_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);
		pSkin = pSkinModel->GetA3DSkin(SKIN_HAIR_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);
		pSkin = pSkinModel->GetA3DSkin(SKIN_EAR_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);
		pSkin = pSkinModel->GetA3DSkin(SKIN_TAIL_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);
		QueueECModelForRelease(Ret.pPlayerModel);
		Ret.pPlayerModel = NULL;
	}

	if (Ret.pDummyModel)
	{
		QueueECModelForRelease(Ret.pDummyModel);
		Ret.pDummyModel = NULL;
	}

	if (Ret.pPortraitModel)
	{
		A3DSkin* pSkin;

		for (int i = 0; i < NUM_PORTAIT_SKIN; i++)
		{
			pSkin = Ret.pPortraitModel->GetA3DSkin(i);
			g_pGame->ReleaseA3DSkin(pSkin);
		}

		g_pGame->ReleaseA3DSkinModel(Ret.pPortraitModel);
		Ret.pPortraitModel = NULL;
	}

	A3DRELEASE(Ret.pPetModel);
	A3DRELEASE(Ret.pChangedModel);
	A3DRELEASE(Ret.pTransfiguredModel);
	A3DRELEASE(Ret.pAircraftModel);
	A3DRELEASE(Ret.EquipResult.pLeftHandWeapon);
	A3DRELEASE(Ret.EquipResult.pRightHandWeapon);
	A3DRELEASE(Ret.EquipResult.pTalisman);
	A3DRELEASE(Ret.EquipResult.pWing);
	A3DRELEASE(Ret.EquipResult.pTune);

	for (int i = 0; i < 3; i++)
	{
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_HEAD_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_HAIR_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FOOT_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_HELM_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_GLASSES_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_NOSE_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_MUSTACHE_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_CLOAK_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_UPPER_BODY_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_LOWER_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_FOOT_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_FASHION_HEADWEAR_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_EAR_INDEX][i]);
		g_pGame->ReleaseA3DSkin(Ret.EquipResult.aSkins[SKIN_TAIL_INDEX][i]);
	}
}

//	Get exp of specified level
double CECPlayer::GetLevelUpExp(int iLevel, int iRebornCount)
{
	switch (iRebornCount)
	{
	case 0:
		return m_PlayerLevelUpExp.exp[iLevel - 1];
	case 1:
		return m_PlayerLevelUpExp.exp_lev_1[iLevel - 1];
	case 2:
		return m_PlayerLevelUpExp.exp_lev_2[iLevel - 1];
	default:
		return m_PlayerLevelUpExp.exp_lev_3[iLevel - 1];
	}
}

double CECPlayer::GetDeityLevelUpExp(int iLevel)
{
	return m_PlayerLevelUpExp.deity[iLevel - 1].deity_exp;
}

int CECPlayer::GetProduceUpExp(int iLevel)
{
	return m_PlayerLevelUpExp.prod_exp_need[iLevel - 1];
}

int CECPlayer::GetTalismanLevelUpExp(int iLevel)
{
	return m_PlayerLevelUpExp.talisman_exp[iLevel - 1];
}

int CECPlayer::GetTalismanTotalExp(int iLevel)
{
	return m_TalismanTotalExp[iLevel - 1];
}

double CECPlayer::GetPetLevelUpExp(int iLevel)
{
	return m_PlayerLevelUpExp.pet_exp[iLevel - 1];
}

//	Build riding pet file name
const char* CECPlayer::GetRidingPetFileName(int idPet, int iLevel)
{
	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(idPet, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_VEHICLE_ESSENCE)
		return "";

	const VEHICLE_ESSENCE* pData = (const VEHICLE_ESSENCE*)pDataPtr;
	const char* sz = g_pGame->GetDataPath(pData->file_model);
	if (!sz)
		return "";

	return sz;
}

const char* CECPlayer::GetChangedModelFileName(int id, int gender, int prof)
{
	if (id == 1)
	{
		if (gender == GENDER_MALE)
			return _changed_shape_path[0];
		else
			return _changed_shape_path[1];
	}
	else if (id ==2)
	{
		if (gender == GENDER_MALE)
			return _changed_shape_path[2];
		else
			return _changed_shape_path[3];
	}
	// 烈山变身成狐狸，按修真等级来变身：1、2重变身小狐狸；3、4重变身中狐狸；5重变身大狐狸
	else if(id ==3)
	{
		if (prof == PROF_40 || prof == PROF_41 )
			return _changed_shape_path[4];
		else if (prof == PROF_42 || prof == PROF_43)
			return _changed_shape_path[5];
		else if (prof == PROF_44)
			return _changed_shape_path[6];
		else
			return NULL;
	}
	else if (id == 4)
	{
		// 怀光专用变身
		return _changed_shape_path[7];
	}
	else if ( 5 == id ) //  焚香变身-原灵术 Added 2011-08-17.
	{
		return _changed_shape_path[8];
	}
	else
		return NULL;
}

const char* CECPlayer::GetTransfiguredModelFileName(int id)
{
	DATA_TYPE DataType;
	
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(id, ID_SPACE_CONFIG, DataType);
	if (DataType != DT_CHANGE_SHAPE_PROP_CONFIG)
		return "";
	const CHANGE_SHAPE_PROP_CONFIG* pData = (const CHANGE_SHAPE_PROP_CONFIG*)pDataPtr;
	
	const MONSTER_ESSENCE* pMonster = (const MONSTER_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pData->monster_id, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_MONSTER_ESSENCE)
		return "";

	const char* sz = g_pGame->GetDataPath(pMonster->file_model);
	if (!sz)
		return "";

	return sz;
}

static const char* GetTransformGfxPath(int id)
{
	if (id == 1)
		return "人物\\技能\\鬼道\\转化魔魂形态.gfx";
	else if (id == 2)
		return "人物\\技能\\鬼道\\蛊王形态转化.gfx";
	else if (id == 3)
		return "程序联入\\烈山变身技能_狐影术.gfx";
	else if (5 == id) //Added 2011-08-25.
	{
		return "程序联入\\焚香火焰变身.gfx";
	}
	else
		return "";
}

//	Initialize static resources
bool CECPlayer::InitStaticRes()
{
	//	Element data man must has been initialized
	elementdataman* pdb = g_pGame->GetElementDataMan();
	if (!pdb)
	{
		ASSERT(pdb);
		return false;
	}

	BuildActionList();

	// Initialize level up exp table
	DATA_TYPE dt;
	int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);

	while (idEssence)
	{
		if (dt == DT_PLAYER_LEVELEXP_CONFIG)
			m_PlayerLevelUpExp = *(PLAYER_LEVELEXP_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
		else if (dt == DT_CHARACTER_CLASS_CONFIG)
		{
			CHARACTER_CLASS_CONFIG* p = (CHARACTER_CLASS_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			SECT_LEVEL2_INFO info;
			info.mask_sect = p->sect_mask;
			info.level2 = p->level2;
			assert(m_SectLevel2Map.find(p->character_class_id) == m_SectLevel2Map.end());
			m_SectLevel2Map[p->character_class_id] = info;
		}
		else if( dt == DT_SCORE_TO_RANK_CONFIG )
		{
			SCORE_TO_RANK_CONFIG* p = (SCORE_TO_RANK_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
			for(int i=0; i<10; ++i)
			{
				AWString* pstr = new AWString(p->score2rank[i].rank_name);
				if (!pstr)
				{
					glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::InitStaticRes", __LINE__);
					return false;
				}

				if (!m_MilitaryRanks.put(i+1, pstr))
				{
					delete pstr;
					ASSERT(0);
				}				
			}
		}
		idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
	}

	int exp = 0;

	for (int i = 0; i < sizeof(m_TalismanTotalExp) / sizeof(m_TalismanTotalExp[0]); i++)
	{
		exp += m_PlayerLevelUpExp.talisman_exp[i];
		m_TalismanTotalExp[i] = exp;
	}
	
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_HEAD);
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_FASHION_HEAD);
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_BODY);
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_FASHION_BODY);		
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_FASHION_LEG);
//	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_FOOT);
// 	m_HideEquipMaskInArena |= (1<<EQUIPIVTR_FASHION_FOOT);	

	return true;
}

//	Get sect info
CECPlayer::SECT_LEVEL2_INFO* CECPlayer::GetSectInfoByProf(int idProf)
{
	SectLevel2Map::iterator it = m_SectLevel2Map.find(idProf);

	if (it == m_SectLevel2Map.end())
		return NULL;
	else
		return &it->second;
}

int CECPlayer::GetRaceByProf(int idProf)
{
	return ((idProf < 32) || ((idProf >= 64)&&(idProf <= 95))) ? RACE_HUMAN:RACE_ORC; //Modified 2011-08-01.
// 	SectLevel2Map::iterator it = m_SectLevel2Map.find(idProf);
// 
// 	if( it == m_SectLevel2Map.end())
// 		return RACE_HUMAN;
// 	else
// 	{
// 		CECPlayer::SECT_LEVEL2_INFO* pInfo = &it->second;
// 		for(int i = 0; i< NUM_SECT; ++i)
// 		{
// 			if(pInfo && (pInfo->mask_sect & (1<<i)) )
// 			{
// 				if( i == SECT_10)   // 兽族
// 					return RACE_ORC;
// 				if( i == SECT_11)	// 妖族
// 					return RACE_EVILKIND;
// 				return RACE_HUMAN;
// 			}
// 		}
// 		return RACE_HUMAN;
// 	}
}

int CECPlayer::GetPhysiqueByProf(int idProf)
{
	SectLevel2Map::iterator it = m_SectLevel2Map.find(idProf);
	if (it == m_SectLevel2Map.end())
		return PHYSIQUE_NORMAL;
	else
	{
		CECPlayer::SECT_LEVEL2_INFO* pInfo = &it->second;
		for(int i = 0; i < NUM_SECT; ++i)
		{
			if (pInfo && (pInfo->mask_sect & (1<<i)))
			{		
				if (i == SECT_10)
					return PHYSIQUE_BIG;
				if (i == SECT_11 || i == SECT_13)
					return PHYSIQUE_SMALL;
				if ( i == SECT_12 )	//太昊，神族 大体型 Added 2011-08-05.
				{
					return PHYSIQUE_BIG;
				}
				if (i == SECT_15)
					return PHYSIQUE_MIDDLE;
				return PHYSIQUE_NORMAL;
			}
		}
		return PHYSIQUE_NORMAL;
	}
}

//	Release static resources
void CECPlayer::ReleaseStaticRes()
{
	m_ActionNames.Release();
	m_SkillActs.Release();

	if (m_MilitaryRanks.size())
	{
		WStrTable::iterator it = m_MilitaryRanks.begin();
		for (; it != m_MilitaryRanks.end(); ++it)
		{
			AWString* pstr = *it.value();
			delete pstr;
		}
	}
	m_MilitaryRanks.clear();
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECPlayer
//	
///////////////////////////////////////////////////////////////////////////

CECStringTab CECPlayer::m_ActionNames;		//	Action name table
CECStringTab CECPlayer::m_SkillActs;		//	Skill action table
PLAYER_LEVELEXP_CONFIG	CECPlayer::m_PlayerLevelUpExp;	// Level up exp needed
CECPlayer::SectLevel2Map CECPlayer::m_SectLevel2Map;
int CECPlayer::m_TalismanTotalExp[100];
CECPlayer::WStrTable CECPlayer::m_MilitaryRanks(16);
//DWORD CECPlayer::m_HideEquipMaskInArena = 0;
//玩家在竞技场中的默认装备
const int CECPlayer::m_EquipsInArena[][4] = 
{
	// 武器，帽子，衣服，鞋子
	// 少侠、鬼王宗、合欢派、青云门、天音寺、    鬼道、备用、备用、
	//（不用的兽族少侠、九黎（10）、烈山（11）、太昊（12，备用）、怀光（13）、东夷（14）、天华（15） （男、女）
	// 人族
	{320, 440, 400,	360},
	{320, 520, 480, 560},
	{300, 420, 380, 340},
	{300, 500, 460, 540},
	{310, 430, 390, 350},
	{310, 510, 470, 550},
	{320, 440, 400,	360},
	{320, 520, 480, 560},
	{330, 450, 410, 370},
	{330, 530, 490, 570},
	{14584, 14302, 14675, 14957},	
	{14584, 14443, 14816, 15098},

	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},
	{0,0,0,0},

//  神族
	{0,0,0,0},
	{0,0,0,0},
	
	{25621,25472,25512,25552},
	{25621,25473,25513,25553},
	{25631,25492,25532,25572},
	{25631,25493,25533,25573},
	{0,0,0,0},
	{0,0,0,0},
	// 怀光
	{35023,34968,34988,35008},
	{35023,34969,34989,35009},
	{0,0,0,0},
	{0,0,0,0},
	// 天华
	{35093,35038,35058,35078},
	{35093,35039,35059,35079},
};

CECPlayer::CECPlayer(CECPlayerMan* pPlayerMan) : m_matRelCarrier(A3DMATRIX4::IDENTITY)
{
	m_iCID				= OCID_PLAYER;
	m_pPlayerMan		= pPlayerMan;
	m_pPlayerModel		= NULL;
	m_pDummyModel		= NULL;
	m_pBackupModel		= NULL;
	m_pPetModel			= NULL;
	m_pChangedModel		= NULL;
	m_pTransfiguredModel= NULL;
	m_pPortraitModel	= NULL;
	m_pHeadPortraitModel= NULL;
	m_pSprite			= NULL;
	m_pSpriteTune		= NULL;
	memset(m_pBodyShader, 0, sizeof(A3DShader *) * 3);
	m_iMoveMode			= MOVE_STAND;
	m_iMoveEnv			= MOVEENV_GROUND;
	m_bCastShadow		= false;
	m_pLevelUpGFX		= NULL;
	m_pWaterWaveStill	= NULL;
	m_pWaterWaveMove	= NULL;
	m_pAirBubble		= NULL;
	m_pSwimBubble		= NULL;
	m_pTransformGfx		= NULL;
	m_pDuelStateGFX		= NULL;
	m_pVipStateGFX		= NULL;
	m_fTouchRad			= 0.3f;
	m_iMoneyCnt			= 0;
	m_iMaxMoney			= 1000;
	m_bWalkRun			= 1;
//	m_pTeam				= NULL;
	m_iFaceid			= 0;
	m_iHairid			= 0;
	m_iEarid			= 0;
	m_iTailid			= 0;
	m_iDefaultSkin		= 0;
	m_iGender			= GENDER_MALE;
	m_iRace				= RACE_HUMAN;
	m_iPhysique			= PHYSIQUE_NORMAL;	// 体型
//	m_iRace				= g_pGame->GetGameInit().iRaceTest;
	m_bRenderName		= false;
	m_bRenderBar		= true;
	m_dwStates			= 0;
	m_dwResFlags		= 0;
	m_uAttackType		= 0;
	m_bAboutToDie		= false;
	m_dwExtStates		= 0;
//	m_pCurSkill			= NULL;
	m_idCurSkillTarget	= 0;
	m_bFight			= false;
	m_iReputation		= 0;
	m_iCultivation		= 0;
	m_iTitle			= 0;
	m_idSpouse			= 0;
	m_idMaster			= 0;
	m_bInSanctuary		= false;
	m_iBoothState		= 0;
	m_crcBooth			= 0;
	m_bFashionMode		= false;
	m_idFaction			= 0;
	m_idFamily			= 0;
//	m_idFRole			= GNET::_R_UNMEMBER;
	m_bRushFly			= false;
	m_AttachMode		= enumAttachNone;
	m_bHangerOn			= false;
	m_iBuddyId			= 0;
	m_idCandBuddy		= 0;
	m_CandPet.id		= 0;
	m_CandPet.iLevel	= 0;
	m_bCandHangerOn		= false;
	m_nFlyMode			= 0;
	m_fPetHeight		= 0;
	m_pFactionDecal		= NULL;
	m_iBattleCamp		= GP_BATTLE_CAMP_NONE;
	m_dwGMFlags			= 0;
	m_bLoadFlag			= false;
	m_idFullSuite		= 0;
	m_bPateNeedUpdate	= false;
	m_colorTitle		= 0xffffffff;
	m_clCityOwner		= 0xffffffff;
	m_bRenderFamily		= false;
	m_bRequestFamilyName= false;
	m_bRequestFactionName = false;
	m_bRequestCircleName= false;
	m_bUsingTalismanSkill = false;
	m_dwHideEquipMask   = 0;
	m_dwHideEquipMaskInUse = 0;
	m_iChangeShape		= 0;
	m_iCandChangeShape	= 0;
	m_nRebornCount		= 0;
//	m_pAchievementMan   = NULL;
	m_idCarrier			= 0;
	
	m_fDistToCamera		= 0.0f;

// 	if ((m_pPateName = new CECPateText))
// 		m_pPateName->EnableBorder(true);

// 	m_pPateTitle = new CECPateText;
// 	m_pPateCityOwner = new CECPateText;
// 	m_pPateFamily = new CECPateText;
// 
// 	if ((m_pPateBooth = new CECPateText))
// 		m_pPateBooth->EnableBorder(true);
// 
// 	if ((m_pPateFaction = new CECPateText))
// 		m_pPateFaction->EnableBorder(true);
// 	m_pPateRankName = new CECPateText;
// 	
// 	if ((m_pPateCircle = new CECPateText))
// 		m_pPateCircle->EnableBorder(true);
// 
// 	m_pPateLastWords1 = new CECPateText;
// 	m_pPateLastWords1->EnableBackground(true);
// 	m_pPateLastWords2 = new CECPateText;
// 	m_pPateTeamReq = new CECPateText;
 	m_pBubbleTexts = new CECBubbleDecalList;

	m_nLowerEquipMethod			= enumSkinShowNone;
	m_nLowerFashionEquipMethod	= enumSkinShowNone;

	m_aabb.Center	= g_vOrigin;
	m_aabb.Extents.Set(0.3f, 0.9f, 0.3f);
	m_MoveConst.fStepHei		= 0.8f;
	m_MoveConst.fMinAirHei		= 1.6f;	
	m_MoveConst.fMinWaterHei	= 0.3f;
	m_MoveConst.fShoreDepth		= 1.6f;
	m_MoveConst.fWaterSurf		= 0.6f;	

	m_vPortraitCamScale	= 1.0f;

	memset(&m_PlayerInfo, 0, sizeof (m_PlayerInfo));
	memset(&m_BasicProps, 0, sizeof (m_BasicProps));
	memset(&m_ExtProps, 0, sizeof (m_ExtProps));
	memset(m_aEquips, 0xff, sizeof (m_aEquips));
	memset(m_aHideEquips, 0xff, sizeof(m_aHideEquips));
	memset(&m_TeamReq, 0, sizeof (m_TeamReq));
	memset(m_aSkins, 0, sizeof (m_aSkins));
	memset(m_aCurSkins, 0, sizeof (m_aCurSkins));
	memset(&m_pvp, 0, sizeof (m_pvp));
	memset(&m_RidingPet, 0, sizeof (m_RidingPet));
	memset(m_iMultiBuddyId, 0, sizeof(m_iMultiBuddyId));
	memset(m_iMultiCandBuddyId, 0, sizeof(m_iMultiCandBuddyId));
	memset(&m_CandTransfigure, 0, sizeof(m_CandTransfigure));
	memset(&m_TransfigureInfo, 0, sizeof(m_TransfigureInfo));
	m_iLeaderId = 0;
	m_iCandLeaderId = 0;
	m_iCandMultiRidePos = 0;
	m_bMultiRideMember = false;
	m_bMultiRideLeader = false;
	m_iApplyBattleType	= -1;
	m_iBattleType		= -1;    // 没在战场赋值为-1
	m_iMilitaryRank		= 0;
	m_bHasChangedEquips = false; 
	m_bFashionModeBackUp = false;
	m_dwHideEquipMaskBackUp = 0;
	m_iVipState			= NO_VIP_STATE;
	m_fTransHeight		= 0;
	m_bAnonymous		= false;
	m_bScaleWeapon		= false;
	m_bInvisible		= false;
	m_bDim				= false;
	m_iCloneID			= 0;
	m_idSummoner		= 0;
	m_iDoubleHit		= 0;
	m_bUpdateCloneEquip = false;

	m_PateContent.iVisible = 0;

	m_stoneWeapon		= 0;
	m_stoneBody			= 0;
	m_stoneHead			= 0;
	m_stoneFoot			= 0;
	
	m_idCircle			= 0;
	m_idCRole			= 0;
	m_idOrgServer		= 0;
	m_iSpecialState		= 0;
	m_fScaleShapeRatio	= 0.0f;//Added 2011-06-23.
	m_iDelayedOperateState = 0;
	m_ucVIPLevel		= 0;
	m_bEquipRuneOpen	= false;

//	m_strLastSayCnt.SetPeriod(20000);
// 	m_IncantCnt.SetPeriod(1000);
// 	m_IncantCnt.Reset(true);
// 	m_ScaleWeaponCnt.SetPeriod(1000);
}

CECPlayer::~CECPlayer()
{
}

//#define OUTPUT_ACTIONS

void CECPlayer::BuildActionList()
{
	//	Load action names from file
	if (!m_ActionNames.IsInitialized())
	{
		m_ActionNames.Init("Configs\\actions_player.txt", false);
	}
	if (!m_SkillActs.IsInitialized())
	{
		m_SkillActs.Init("Configs\\skillacts.txt", false);
	}

	char szLine[1024];
	DWORD dwReadLen;
	AFileImage file;

	if( !file.Open("configs\\male_eye_joints.txt", AFILE_OPENEXIST | AFILE_TEXT) )
		return;
	while( file.ReadLine(szLine, 1024, &dwReadLen) )
	{
		if( strlen(szLine) == 0 )
			break;
		int id = atoi(szLine);
		if( id < 0 )
			continue;
		_male_eye_joint_indice.push_back(id);
	}
	file.Close();

	if( !file.Open("configs\\female_eye_joints.txt", AFILE_OPENEXIST | AFILE_TEXT) )
		return;
	while( file.ReadLine(szLine, 1024, &dwReadLen) )
	{
		if( strlen(szLine) == 0 )
			break;
		int id = atoi(szLine);
		if( id < 0 )
			continue;
		_female_eye_joint_indice.push_back(id);
	}
	file.Close();
}

//	Release object
void CECPlayer::Release()
{
	DetachBuddy();
	
	if(m_bMultiRideLeader)
	{
		for(int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if(m_iMultiBuddyId[i])
				DetachMultiBuddy(i);
		}
	}
	else if (m_bMultiRideMember)
	{
		CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);
		if(pLeader && pLeader->IsMultiRideLeader())
		{
			pLeader->RemoveMultiRideMember(m_PlayerInfo.cid);
		}
 	}

	//	Clear extend states before model is released
	SetNewExtendStates(0);
	m_aIconStates.RemoveAll();

// 	if (m_pPateName)
// 	{
// 		delete m_pPateName;
// 		m_pPateName = NULL;
// 	}
// 
// 	if (m_pPateTitle)
// 	{
// 		delete m_pPateTitle;
// 		m_pPateTitle = NULL;
// 	}
// 
// 	if (m_pPateCityOwner)
// 	{
// 		delete m_pPateCityOwner;
// 		m_pPateCityOwner = NULL;
// 	}
// 
// 	if (m_pPateFamily)
// 	{
// 		delete m_pPateFamily;
// 		m_pPateFamily = NULL;
// 	}
// 
// 	if (m_pPateLastWords1)
// 	{
// 		delete m_pPateLastWords1;
// 		m_pPateLastWords1 = NULL;
// 	}
// 
// 	if (m_pPateLastWords2)
// 	{
// 		delete m_pPateLastWords2;
// 		m_pPateLastWords2 = NULL;
// 	}
// 
// 	if (m_pPateTeamReq)
// 	{
// 		delete m_pPateTeamReq;
// 		m_pPateTeamReq = NULL;
// 	}
// 
// 	if (m_pPateBooth)
// 	{
// 		delete m_pPateBooth;
// 		m_pPateBooth = NULL;
// 	}
// 
// 	if (m_pPateFaction)
// 	{
// 		delete m_pPateFaction;
// 		m_pPateFaction = NULL;
// 	}
// 
// 	if(m_pPateRankName)
// 	{
// 		delete m_pPateRankName;
// 		m_pPateRankName = NULL;
// 	}
// 	
// 	if (m_pPateCircle)
// 	{
// 		delete m_pPateCircle;
// 		m_pPateCircle = NULL;
// 	}
// 
// 	if (m_pFactionDecal)
// 	{
// 		delete m_pFactionDecal;
// 		m_pFactionDecal = NULL;
// 	}
// 
// 	if (m_pBubbleTexts)
// 	{
// 		delete m_pBubbleTexts;
// 		m_pBubbleTexts = NULL;
// 	}

	if (m_pLevelUpGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pLevelUpGFX);
		m_pLevelUpGFX = NULL;
	}

	if (m_pWaterWaveStill)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pWaterWaveStill);
		m_pWaterWaveStill = NULL;
	}
	
	if (m_pWaterWaveMove)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pWaterWaveMove);
		m_pWaterWaveMove = NULL;
	}
	
	if (m_pAirBubble)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAirBubble);
		m_pAirBubble = NULL;
	}

	if (m_pSwimBubble)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSwimBubble);
		m_pSwimBubble = NULL;
	}

	if (m_pTransformGfx)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pTransformGfx);
		m_pTransformGfx = NULL;
	}

	if (m_pDuelStateGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pDuelStateGFX);
		m_pDuelStateGFX = NULL;
	}

	if (m_pVipStateGFX)
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pVipStateGFX);
		m_pVipStateGFX = NULL;
	}
	
	if (m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride, false);
		A3DRELEASE(m_pPetModel);
	}

// 	if (m_pAchievementMan)
// 	{
// 		delete m_pAchievementMan;
// 		m_pAchievementMan = NULL;
// 	}
	
	A3DRELEASE(m_pChangedModel);
	
	A3DRELEASE(m_pTransfiguredModel);

	//	Release dummy model
	ReleaseDummyModel();

	//	Release player model
	ReleasePlayerModel();

	//	Clear resource ready flags
	SetResReadyFlag(RESFG_ALL, false);

	m_bLoadFlag = false;
	m_bAboutToDie = false;
}

bool CECPlayer::LoadPlayerSkin(
	A3DSkin* aSkins[3],
	int iIndex,
	const char* szFile)
{
	ASSERT(iIndex >= 0 && iIndex < NUM_SKIN_INDEX);
	
#ifdef _DEBUG
	static int _reentrant = 0;
	assert(_reentrant++ == 0);
#endif

	memset(aSkins, 0, sizeof(A3DSkin*) * 3);

	if( szFile )
	{
		const char * suffix1[] = {"一级", "二级", "三级"};
		const char * suffix2[] = {"", "二级", "三级"};
		const char ** pSuffixes = suffix2;

		char strSkinPath[MAX_PATH];
		int i = 0;
		// for(i=0; i<3; i++)
		{
			//sprintf(strSkinPath, "%s%s.ski", szFile, pSuffixes[i]);

			A3DSkin * pSkin = g_pGame->LoadA3DSkin(szFile, true);
			if (pSkin == NULL)
			{
				a_LogOutput(1, "CECPlayer::ReplacePlayerSkin, Falied to load skin %s", strSkinPath);
#ifdef _DEBUG
				assert(--_reentrant == 0);
#endif
				return false;
			}

			aSkins[i] = pSkin;
		}
	}

#ifdef _DEBUG
	assert(--_reentrant == 0);
#endif

	return true;
}

//	Release player model
void CECPlayer::ReleasePlayerModel()
{
	if( m_pSprite )
	{
		m_pSprite->Release();
		delete m_pSprite;
		m_pSprite = NULL;
	}
	if( m_pSpriteTune )
	{
		m_pSpriteTune->Release();
		delete m_pSpriteTune;
		m_pSpriteTune = NULL;
	}

	//	Release player skins
	for (int i=0; i < NUM_SKIN_INDEX; i++)
	{
		g_pGame->ReleaseA3DSkin(m_aSkins[i][0]);
		g_pGame->ReleaseA3DSkin(m_aSkins[i][1]);
		g_pGame->ReleaseA3DSkin(m_aSkins[i][2]);
		m_aCurSkins[i] = NULL;
	}
	memset(m_aSkins, 0, sizeof (m_aSkins));
	memset(m_aEquips, 0xff, sizeof(m_aEquips));
	memset(m_aHideEquips, 0xff, sizeof(m_aHideEquips));

	if (m_pBackupModel)
	{
		A3DSkin* pSkin;

		pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HEAD_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);

		pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HAIR_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);
		
		pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_EAR_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);

		pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_TAIL_INDEX);
		g_pGame->ReleaseA3DSkin(pSkin);

		if (IsLoadThreadReady())
		{
			QueueECModelForRelease(m_pBackupModel);
		}
		else
		{
			m_pBackupModel->Release();
			delete m_pBackupModel;
		}

		m_pPlayerModel = NULL;
		m_pBackupModel = NULL;
	}

	if (m_pPortraitModel)
	{
		A3DSkin* pSkin;

		for (int i = 0; i < NUM_PORTAIT_SKIN; i++)
		{
			pSkin = m_pPortraitModel->GetA3DSkin(i);
			g_pGame->ReleaseA3DSkin(pSkin);
		}

		g_pGame->ReleaseA3DSkinModel(m_pPortraitModel);
		m_pPortraitModel = NULL;
	}
	
	if(m_pHeadPortraitModel)
	{
		A3DSkin* pSkin;
		for (int i = 0; i< NUM_PORTAIT_SKIN; ++i)
		{
			pSkin = m_pHeadPortraitModel->GetA3DSkin(i);
			g_pGame->ReleaseA3DSkin(pSkin);
		}

		g_pGame->ReleaseA3DSkinModel(m_pHeadPortraitModel);
		m_pHeadPortraitModel = NULL;
	}

	SetResReadyFlag(RESFG_SKELETON | RESFG_SKIN, false);
}

//	Release dummy model
void CECPlayer::ReleaseDummyModel()
{
	if (!m_pDummyModel)
		return;

	if (IsLoadThreadReady())
	{
		QueueECModelForRelease(m_pDummyModel);
	}
	else
	{
		m_pDummyModel->Release();
		delete m_pDummyModel;
	}

	m_pDummyModel = NULL;
}

void CECPlayer::TransformShape(int form, bool bPlayerGfx)
{
	if (form)
	{
		m_iChangeShape = form;
		m_iCandChangeShape = form;
	}
	else
	{
		A3DRELEASE(m_pChangedModel);

		if (m_pPlayerModel && bPlayerGfx)
			m_pPlayerModel->PlayGfx(GetTransformGfxPath(m_iChangeShape), NULL);

		m_iChangeShape = 0;
		m_iCandChangeShape = 0;
	}
}

void CECPlayer::Transfigure(int id, bool bPlayGfx)
{
	m_CandTransfigure.id = id;
	m_TransfigureInfo.id = id;
	
	if (id != 0)
	{
		DATA_TYPE dt;
		const CHANGE_SHAPE_PROP_CONFIG * pDBConfig = (const CHANGE_SHAPE_PROP_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(m_TransfigureInfo.id, ID_SPACE_CONFIG, dt);
		if (dt == DT_CHANGE_SHAPE_PROP_CONFIG)
		{
			m_TransfigureInfo.bReplaceName	= pDBConfig->is_replace_name? true:false;
			m_TransfigureInfo.bIsFly		= pDBConfig->is_fly? true:false;
			m_TransfigureInfo.action_type   = pDBConfig->action_type;
			const MONSTER_ESSENCE* pMonster = (const MONSTER_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pDBConfig->monster_id, ID_SPACE_ESSENCE, dt);
			if (dt != DT_MONSTER_ESSENCE)
				return ;
			if(m_TransfigureInfo.bReplaceName)
			{
				m_strReplacedName = ACString(pMonster->name);
			}
			m_TransfigureInfo.stand_mode = pMonster->stand_mode;
		}
		else
		{
			ASSERT(0);
			return ;
		}
	}
	else if (id == 0)
	{
		A3DRELEASE(m_pTransfiguredModel);
		SetUseGroundNormal(false);

		//	Re-calculate player's AABB
		CalcPlayerAABB();
		SetPos(GetPos());

		if (m_pChangedModel && bPlayGfx)
			m_pChangedModel->PlayGfx(res_GFXFile(RES_GFX_TRANSFIGURE), NULL);
		else if(m_pPlayerModel && bPlayGfx)
			m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_TRANSFIGURE), NULL);

// 		if (m_TransfigureInfo.bReplaceName)
// 			m_pPateName->SetText(m_strName, false);

		m_TransfigureInfo.bReplaceName  = false;
		m_TransfigureInfo.bIsFly		= false;
		m_TransfigureInfo.action_type	= 0;
	}
}

void CECPlayer::AttachBuddy(int iBuddy)
{
	if (!m_pBackupModel || m_TransfigureInfo.id)
		return;

	if (GetGender() != GENDER_MALE)
		return;

	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(iBuddy);

	if (!pBuddy || pBuddy->GetGender() != GENDER_FEMALE || pBuddy->IsShapeChanged() || pBuddy->IsTransfigured())
		return;

	CECModel* pBuddyModel = pBuddy->GetPlayerModel();

	if (!pBuddyModel)
		return;

	if (m_pPetModel)
	{
		if (!m_pPetModel->AddChildModel(_hanger_ride2, false, _hh_ride, pBuddyModel, _cc_ride2))
			return;

		A3DSkinModelHanger* pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_hanger_ride2);
		if (pHanger) pHanger->SetOffsetMat(a3d_Translate(0, 0, 0));
		pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_hanger_ride);
		if (pHanger) pHanger->SetOffsetMat(a3d_Translate(0, 0, 0));

		m_AttachMode = enumAttachRideOnPet;
		m_bHangerOn = false;
		m_iBuddyId = pBuddy->m_PlayerInfo.cid;
		pBuddy->m_AttachMode = enumAttachRideOnPet;
		pBuddy->m_bHangerOn = true;
		pBuddy->m_iBuddyId = m_PlayerInfo.cid;
	}
	else if (IsShapeChanged())
	{
		return;
	}
	else
	{
		if (!m_pBackupModel->AddChildModel(_hanger_hug, false, _hh_bind, pBuddyModel, _cc_bind[m_iPhysique]))
			return;

		m_AttachMode = enumAttachHugPlayer;
		m_bHangerOn = false;
		m_iBuddyId = pBuddy->m_PlayerInfo.cid;
		pBuddy->m_AttachMode = enumAttachHugPlayer;
		pBuddy->m_bHangerOn = true;
		pBuddy->m_iBuddyId = m_PlayerInfo.cid;
	}

	m_idCandBuddy = 0;
	m_bCandHangerOn = false;
	pBuddy->m_idCandBuddy = 0;
	pBuddy->m_bCandHangerOn = false;
	pBuddy->SetPos(GetPos());

	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
}

void CECPlayer::DetachBuddy(CECPlayer* pBuddy)
{
	if (m_AttachMode == enumAttachNone)
		return;

	if (m_bHangerOn)
	{
		if (!pBuddy)
			pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if (pBuddy)
			pBuddy->DetachBuddy(this);
		else
		{
			assert(false);
			m_AttachMode = enumAttachNone;
			m_bHangerOn = false;
			m_iBuddyId = 0;
		}
	}
	else
	{
		if (!pBuddy) pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if (m_AttachMode == enumAttachRideOnPlayer)
		{
			if (m_pDummyModel)
				m_pDummyModel->RemoveChildModel(_hanger_ride, false);
		}
		else if (m_AttachMode == enumAttachHugPlayer)
		{
			if(m_pBackupModel)
				m_pBackupModel->RemoveChildModel(_hanger_hug, false);
		}
		else if (m_AttachMode == enumAttachRideOnPet)
		{
			if (m_pPetModel)
			{
				A3DSkinModelHanger* pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_hanger_ride);
				if (pHanger) pHanger->SetOffsetMat(a3d_IdentityMatrix());
				m_pPetModel->RemoveChildModel(_hanger_ride2, false);
			}
		}

		//如果该成员当时处于太昊归神/脱灵技能状态下，那么解除绑定后需要恢复其模型缩放比例Added 2011-09-27.
		if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
		{
			ScaleObjects( pBuddy->GetPlayerModel() );
		}
		//Added end.

		m_AttachMode = enumAttachNone;
		m_bHangerOn = false;
		m_iBuddyId = 0;

		if (pBuddy)
		{
			pBuddy->m_AttachMode = enumAttachNone;
			pBuddy->m_bHangerOn = false;
			pBuddy->m_iBuddyId = 0;
			pBuddy->SetPos(pBuddy->GetPos());
			pBuddy->SetDirAndUp(pBuddy->GetDir(), pBuddy->GetUp());
			pBuddy->PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
		}
		PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, true);
	}
}

void CECPlayer::AttachMultiBuddy(int iBuddy, int pos)   //iBuddy(多人骑乘的成员)
{
	if (!m_pBackupModel || !m_bMultiRideLeader || m_TransfigureInfo.id)
		return;

	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(iBuddy);

	if (!pBuddy || pBuddy->IsShapeChanged() || pBuddy->IsTransfigured())
		return;

	CECModel* pBuddyModel = pBuddy->GetPlayerModel();

	if (!pBuddyModel)
		return;

	if (m_pPetModel)
	{
		// 服务器端与客户端不一致，先将原来成员解除绑定
		if( m_iMultiBuddyId[pos] )
			DetachMultiBuddy(pos);
		if (!m_pPetModel->AddChildModel(_multi_hanger_name[pos], false, _multi_hh_ride[pos], pBuddyModel, _cc_ride))
			return;

		A3DSkinModelHanger* pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_multi_hanger_name[pos]);
		if (pHanger) pHanger->SetOffsetMat(a3d_Translate(0, 0, 0));
		pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_multi_hanger_name[0]);
		if (pHanger) pHanger->SetOffsetMat(a3d_Translate(0, 0, 0));

		m_iMultiCandBuddyId[pos] = 0;
		m_iMultiBuddyId[pos] = iBuddy;
		pBuddy->SetPos(GetPos());
		pBuddy->m_iLeaderId = m_PlayerInfo.cid;
		pBuddy->m_bMultiRideMember = true;
		pBuddy->m_iMultiRidePos = pos;
	}
	else if (IsShapeChanged())
	{
		return;
	}
	
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
}

void CECPlayer::DetachMultiBuddy(int pos, CECPlayer* pBuddy)    //pBuddy (多人骑乘的成员)
{
	if (!m_bMultiRideLeader)
		return;

	if (!pBuddy) pBuddy = m_pPlayerMan->GetPlayer(m_iMultiBuddyId[pos]);
	if (!pBuddy) return;

	if (m_pPetModel)
	{
		A3DSkinModelHanger* pHanger = m_pPetModel->GetA3DSkinModel()->GetSkinModelHanger(_multi_hanger_name[pos]);
		if (pHanger) pHanger->SetOffsetMat(a3d_IdentityMatrix());
		m_pPetModel->RemoveChildModel(_multi_hanger_name[pos], false);    // 通过hanger name来删除
	}

	m_iMultiBuddyId[pos] = 0;

	//如果该成员当时处于太昊归神/脱灵技能状态下，那么解除多人坐骑后需要恢复其模型缩放比例Added 2011-08-19.
	if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
	{
		ScaleObjects( pBuddy->GetPlayerModel() );
	}
	//Added end.
	if (pBuddy)
	{
		pBuddy->m_iLeaderId = 0;
		pBuddy->m_bMultiRideMember = false;
		pBuddy->m_iMultiRidePos = 0;
		pBuddy->SetPos(pBuddy->GetPos());
		pBuddy->SetDirAndUp(pBuddy->GetDir(), pBuddy->GetUp());
		pBuddy->PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
	}
	PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, true);	
}


//	Load player's skeleton
bool CECPlayer::LoadPlayerSkeleton(const int* pEquips, bool bLoadAtOnce/* false */, bool bHideEquip/* true */)
{
	CheckIndexBound(m_iFaceid);
	CheckIndexBound(m_iHairid);
	m_bLoadFlag = false;

	if (pEquips)
	{
		//	Fill equipments
		if (bHideEquip)
			UpdateHideEquipMask(pEquips);
		for (int i = 0; i < SIZE_EQUIPIVTR; i++)
		{
			m_aEquips[i] = pEquips[i];
			if(m_dwHideEquipMaskInUse & (1<<i))
				m_aHideEquips[i] = 0;
			else
				m_aHideEquips[i] = pEquips[i]; 
		}
	}

	const char* szPetPath = NULL;
	const char* szChangedPath = NULL;

	if (m_RidingPet.id)
		szPetPath = GetRidingPetFileName(m_RidingPet.id, m_RidingPet.iLevel);

	if (m_iChangeShape)
		szChangedPath = GetChangedModelFileName(m_iChangeShape, m_iGender, m_BasicProps.iProfession);

	if (bLoadAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;

		if (!LoadPlayerModel(m_iPhysique, m_BasicProps.iProfession, m_BasicProps.iLevel, m_iEarid, m_iTailid, m_iFaceid, m_iHairid, 
			m_iDefaultSkin, m_iGender, pEquips? m_aHideEquips:NULL, szPetPath, szChangedPath, Ret))
		{
			a_LogOutput(1, "CECPlayer::Init, failed to call LoadPlayerModel() !");
			return false;
		}

		SetPlayerLoadedResult(Ret);
	}
	else
	{
			QueueECModelForLoad(
				MTL_ECM_PLAYER,
				m_PlayerInfo.cid,
				m_dwBornStamp,
				GetPos(),
				szPetPath,
				szChangedPath,
				m_iFaceid,
				m_iHairid,
				(m_iPhysique << 16) + m_iGender,
				pEquips? const_cast<int*>(m_aHideEquips):NULL,
				m_BasicProps.iLevel,
				m_iEarid,
				m_iTailid,
				m_BasicProps.iProfession,
				m_iDefaultSkin);
	}

	return true;
}

bool CECPlayer::QueueLoadEquips(const int* pEquips, DWORD dwMask, bool bAtOnce)
{
	m_bLoadFlag = false;

	if (bAtOnce || !IsLoadThreadReady())
	{
		SuspendLoadThread();
		EquipsLoadResult Ret;

		if (!LoadPlayerEquips(m_iPhysique, m_BasicProps.iProfession, m_iDefaultSkin, m_iGender, dwMask, pEquips, Ret))
		{
			StartLoaderThread();
			return false;
		}

		StartLoaderThread();
		SetEquipsLoadedResult(Ret, true);
	}
	else
	{
		QueueECModelForLoad(
			MTL_ECM_PLAYER_EQUIP,
			m_PlayerInfo.cid,
			m_dwBornStamp,
			GetPos(),
			NULL,
			NULL,
			m_iPhysique,
			m_iGender,
			dwMask,
			const_cast<int*>(pEquips),
			m_BasicProps.iProfession,
			0,
			0,
			0,
			m_iDefaultSkin);
	}

	return true;
}

bool CECPlayer::QueueLoadPetModel(const char* szPetPath, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));

		if (!LoadPetModel(szPetPath, Ret) || !Ret.pPetModel)
			return false;

		if (!SetPetLoadResult(Ret.pPetModel))
		{
			A3DRELEASE(Ret.pPetModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_PET, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPetPath, NULL);
	}

	return true;
}

bool CECPlayer::QueueLoadChangedModel(const char* szPath, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));

		if (!LoadChangedModel(szPath, Ret) || !Ret.pChangedModel)
			return false;

		if (!SetChangedModelLoadResult(Ret.pChangedModel))
		{
			A3DRELEASE(Ret.pChangedModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_CHANGEDMODEL, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPath, NULL);
	}

	return true;
}

bool CECPlayer::QueueLoadTransfiguredModel(const char* szPath, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));

		if (!LoadTransfiguredModel(szPath, Ret) || !Ret.pTransfiguredModel)
			return false;

		if (!SetTransfigureModelLoadResult(Ret.pTransfiguredModel))
		{
			A3DRELEASE(Ret.pTransfiguredModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_TRANSFIGUREDMODEL, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPath, NULL);
	}

	return true;
}

bool CECPlayer::QueueLoadAircraftModel(const char* szPath, bool bAtOnce)
{
	if (bAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));

		if (!LoadAircraftModel(szPath, Ret) || !Ret.pAircraftModel)
			return false;

		if (!SetAircraftModelLoadResult(Ret.pAircraftModel))
		{
			A3DRELEASE(Ret.pAircraftModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_AIRCRAFT, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPath, NULL);
	}

	return true;
}

void CECPlayer::RideOnPet(int id, int iLev, int type)
{
	m_CandPet.id		= id;
	m_CandPet.iLevel	= iLev;
	m_RidingPet.id		= id;
	m_RidingPet.iLevel	= iLev;
	m_RidingPet.bMultiRider = (type? true:false);
	if (m_RidingPet.bMultiRider)
	{
		m_bMultiRideLeader = true;
		//在多人骑乘状态下，解除相依相偎（防止接收协议错序：出现先上多人坐骑，后解除绑定的情况）
		if(m_iBuddyId)
			DetachBuddy(NULL);
	}
	ShowWing(false);
}

void CECPlayer::GetOffPet()
{
	int iBuddyId = 0;
	m_CandPet.id = 0;
	m_CandPet.iLevel = 0;

	if(m_RidingPet.bMultiRider)
	{	
		// 防止在多人骑乘的状态下，仍然出现了相依相偎的情况
		if (m_AttachMode != enumAttachNone)
		{
			DetachBuddy();
		}

		if (m_pPetModel)
		{
			for(int i =1; i< MULTI_RIDE_CAPACITY; ++i)
			{
				if (m_iMultiBuddyId[i])
					DetachMultiBuddy(i);
			}
			m_pPetModel->RemoveChildModel(_hanger_ride, false);
			A3DRELEASE(m_pPetModel);
		}

		m_RidingPet.id		= 0;
		m_RidingPet.iLevel  = 0;
		m_RidingPet.bMultiRider = false;
		m_RidingPet.iMultiRideMode = 0;
		m_bMultiRideLeader = false;
		//	Re-calculate player's AABB
		CalcPlayerAABB();
		SetUseGroundNormal(IsShapeChanged());
		SetPos(GetPos());
		
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

		if( m_pPlayerModel )
			m_pPlayerModel->PlayGfx("程序联入\\下骑乘.gfx", NULL);
	}
	else
	{		
		if (m_AttachMode != enumAttachNone)
		{
			iBuddyId = m_iBuddyId;
			DetachBuddy();
		}

		if (m_pPetModel)
		{
			m_pPetModel->RemoveChildModel(_hanger_ride, false);
			A3DRELEASE(m_pPetModel);
		}

		m_RidingPet.id		= 0;
		m_RidingPet.iLevel	= 0;

		//	Re-calculate player's AABB
		CalcPlayerAABB();
		SetUseGroundNormal(IsShapeChanged());
		SetPos(GetPos());

		if (iBuddyId)
			AttachBuddy(iBuddyId);
		else
			PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

		if( m_pPlayerModel )
			m_pPlayerModel->PlayGfx("程序联入\\下骑乘.gfx", NULL);
	}
}

#define SET_SKIN_SHOW_MASK(index, b) \
{ \
	dwSkinShowMask |= (1 << (index)); \
	if (b) \
		dwSkinShowFlag |= (1 << (index)); \
	else \
		dwSkinShowFlag &= ~(1 << (index)); \
}

inline void _get_skin_show_mask(
	DWORD& dwChangeMask,
	int& nLowerMethod,
	DWORD& dwSkinShowMask,
	DWORD& dwSkinShowFlag,
	bool bFashion)
{
	for (int i = enumSkinShowNone + 1; i < enumNumSkinShow; i++)
	{
		if ((1 << i) & dwChangeMask)
		{
			switch (i)
			{
			case enumSkinShowHead:

				break;

			case enumSkinShowUpperBody:

				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, true)
				}

				break;
			case enumSkinShowUpperAndLower:

				if (bFashion)
				{
					SET_SKIN_SHOW_MASK(SKIN_FASHION_LOWER_INDEX, false)
				}

				break;
			}
		}
	}
}

static const int _portrait_skin_map[NUM_SKIN_INDEX] =
{
	-1,
	-1,
	-1,
	-1,
	PORTRAIT_SKIN_HELM,
	PORTRAIT_SKIN_GLASSES,
	PORTRAIT_SKIN_NOSE,
	PORTRAIT_SIN_MUSTACHE,
	-1,
	-1,
	-1,
	-1,
	PORTRAIT_SKIN_HEADWEAR,
	-1, 
	-1,
};

bool CECPlayer::SetEquipsLoadedResult(EquipsLoadResult& Result, bool bUpdateAtOnce)
{
	if (!m_pBackupModel || !m_pPortraitModel)
		return false;

	A3DSkinModel* pA3DModel = m_pBackupModel->GetA3DSkinModel();
	DWORD dwSkinShowMask = 0, dwSkinShowFlag = 0;

	_get_skin_show_mask(Result.dwShowMask, m_nLowerEquipMethod, dwSkinShowMask, dwSkinShowFlag, false);
	_get_skin_show_mask(Result.dwFashionShowMask, m_nLowerFashionEquipMethod, dwSkinShowMask, dwSkinShowFlag, true);

	for (int i = 0; i < NUM_SKIN_INDEX; i++)
	{
		DWORD dwMask = (1 << i);

		if (dwMask & Result.dwSkinChangeMask)
		{
			g_pGame->ReleaseA3DSkin(m_aSkins[i][0]);
			g_pGame->ReleaseA3DSkin(m_aSkins[i][1]);
			g_pGame->ReleaseA3DSkin(m_aSkins[i][2]);

			m_aSkins[i][0] = Result.aSkins[i][0];
			m_aSkins[i][1] = Result.aSkins[i][1];
			m_aSkins[i][2] = Result.aSkins[i][2];

			const int nPortraitIndex = _portrait_skin_map[i];

			if (nPortraitIndex >= 0)
			{
				A3DSkin* pSkin = m_pPortraitModel->GetA3DSkin(nPortraitIndex);
				g_pGame->ReleaseA3DSkin(pSkin);
				m_pPortraitModel->ReplaceSkin(nPortraitIndex, NULL, false);

				if (m_aSkins[i][0])
				{
					A3DSkin* pNewSkin = g_pGame->LoadA3DSkin(m_aSkins[i][0]->GetFileName(), true);

					if (pNewSkin)
						m_pPortraitModel->ReplaceSkin(nPortraitIndex, pNewSkin, false);
				}
			}
		}

		if (dwMask & dwSkinShowMask)
			pA3DModel->ShowSkin(i, (dwMask & dwSkinShowFlag) != 0);
	}

	if (bUpdateAtOnce)
		UpdateCurSkins();

	if (Result.bWeaponChanged)
	{
		m_uAttackType = Result.uAttackType;
		m_pBackupModel->RemoveChildModel(_weapon_hanger);
		m_pBackupModel->RemoveChildModel(_right_weapon_hanger);
		m_pBackupModel->RemoveChildModel(_back_weapon_hanger); //Added 2011-08-09.

		if (Result.pLeftHandWeapon)
		{
			m_pBackupModel->AddChildModel(
				_weapon_hanger,
				false,
				"HH_weapon",
				Result.pLeftHandWeapon,
				"CC_weapon");
		}
		if (Result.pRightHandWeapon)
		{
			if ( Result.bHookChanged )	//需要改变左手挂点，新职业中的焚香和太昊 Added 2011-08-09.
			{
				m_pBackupModel->AddChildModel(
					_right_weapon_hanger,
					false,
					"HH_lefthandweapon1",	//新挂点的名字
					Result.pRightHandWeapon,
					"CC_weapon");
			}
			else	//走原来的逻辑，貌似只有怀光的双手镰刀武器才使用这个
			{
				m_pBackupModel->AddChildModel(
					_right_weapon_hanger,
					false,
					"HH_righthandweapon",
					Result.pRightHandWeapon,
					"CC_weapon");
			}
		}
		if (Result.pBackWeapon)	//背部武器 Added 2011-08-09.
		{
			m_pBackupModel->AddChildModel(
				_back_weapon_hanger,
				false,
				"HH_back",	//新挂点名字
				Result.pBackWeapon, //背部武器模型指针
				"CC_back");	//新的子挂点名字
		}
		OnWeaponChanged();
	}

	if (Result.bWingChanged)
	{
		m_nFlyMode = Result.nFlyMode;
		m_pBackupModel->RemoveChildModel(_wing);

		if (Result.pWing)
		{
			m_pBackupModel->AddChildModel(
				_wing,
				false,
				_hh_fly[m_nFlyMode],
				Result.pWing,
				_cc_fly[m_nFlyMode]);
		}

		OnWeaponChanged();
	}

	ShowWing(IsFlying());

	if (Result.bTalismanChanged)
		UpdateTalismanSprite(Result.pTalisman);

	if (Result.bTuneChanged)
		UpdateTuneSprite(Result.pTune);

	int idFullSuite = SearchFullSuite();
	if( idFullSuite == 0 )
	{
		RemoveFullSuiteGFX(m_idFullSuite);
		m_idFullSuite = 0;
	}
	else if( idFullSuite != m_idFullSuite )
	{
		RemoveFullSuiteGFX(m_idFullSuite);
		AddFullSuiteGFX(idFullSuite);
		m_idFullSuite = idFullSuite;
	}

	if( Result.stoneBody != 0xff && m_stoneBody != Result.stoneBody )
	{
		RemoveArmorStones(EQUIPIVTR_BODY, m_stoneBody);
		m_stoneBody = Result.stoneBody;
		AddArmorStones(EQUIPIVTR_BODY, m_stoneBody);
	}
	if( Result.stoneHead != 0xff && m_stoneHead != Result.stoneHead )
	{
		RemoveArmorStones(EQUIPIVTR_HEAD, m_stoneHead);
		m_stoneHead = Result.stoneHead;
		AddArmorStones(EQUIPIVTR_HEAD, m_stoneHead);
	}
	if( Result.stoneFoot != 0xff && m_stoneFoot != Result.stoneFoot )
	{
		RemoveArmorStones(EQUIPIVTR_FOOT, m_stoneFoot);
		m_stoneFoot = Result.stoneFoot;
		AddArmorStones(EQUIPIVTR_FOOT, m_stoneFoot);
	}
	if( Result.stoneWeapon != 0xff && m_stoneWeapon != Result.stoneWeapon )
	{
		RemoveWeaponStones(m_stoneWeapon);
		m_stoneWeapon = Result.stoneWeapon;
		AddWeaponStones(m_stoneWeapon);
	}

	m_bLoadFlag = true;
	return true;
}

bool CECPlayer::SetPetLoadResult(CECModel* pPetModel)
{
	if (!m_RidingPet.id || !m_pBackupModel || m_CandPet.id)
		return false;

	if (m_pPetModel)
	{
		RIDINGPET pet = m_RidingPet;
		GetOffPet();
		m_RidingPet = pet;
	}

	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(m_RidingPet.id, ID_SPACE_ESSENCE, DataType);

	if (DataType != DT_VEHICLE_ESSENCE)
		m_fPetHeight = 1.0f;
	else
	{
		const VEHICLE_ESSENCE* pData = (const VEHICLE_ESSENCE*)pDataPtr;
		m_fPetHeight = pData->height > 0 ? pData->height : 1.0f;
		m_RidingPet.iMultiRideMode	=  pData->multi_ride_mode;
	}

	const A3DVECTOR3 vCurPos = GetPos();
	m_aabb.Extents.y *= 1.7f;
	m_aabb.Center = vCurPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();

	m_pPetModel = pPetModel;
	SetUseGroundNormal(true);
	m_pPetModel->SetAffectedByParent(false);
	m_pPetModel->SetPos(vCurPos);
	m_pPetModel->SetDirAndUp(GetDir(), GetUp());

	int iBuddyId = m_iBuddyId;
	if (m_AttachMode != enumAttachNone)
		DetachBuddy();

	m_pPetModel->AddChildModel(_hanger_ride, false, _hh_ride, m_pBackupModel, _cc_ride);
	m_pPetModel->GetA3DSkinModel()->Update(0);

	if (iBuddyId)
		AttachBuddy(iBuddyId);
	else
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

	return true;
}

bool CECPlayer::SetChangedModelLoadResult(CECModel* pChangedModel)
{
	if (!m_iChangeShape || !m_pBackupModel)
		return false;

	A3DRELEASE(m_pChangedModel);
	m_pChangedModel = pChangedModel;
	m_pChangedModel->SetPos(GetPos());
	m_pChangedModel->SetDirAndUp(GetDir(), GetUp());
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
	m_pChangedModel->PlayGfx(GetTransformGfxPath(m_iChangeShape), NULL);

	return true;
}

bool CECPlayer::SetTransfigureModelLoadResult(CECModel* pTransfiguredModel)
{
	if (!m_TransfigureInfo.id || !m_pBackupModel || m_CandTransfigure.id)
		return false;
	
	if( m_pTransfiguredModel )
	{
		m_pTransfiguredModel->Release();
		delete m_pTransfiguredModel;
		m_pTransfiguredModel = NULL;
	}

	const A3DVECTOR3 vCurPos = GetPos();
	m_pTransfiguredModel = pTransfiguredModel;
	if (m_TransfigureInfo.stand_mode == 0)
		SetUseGroundNormal(true);
	else
		SetUseGroundNormal(false);

	const A3DAABB& aabb = m_pTransfiguredModel->GetModelAABB();
	m_fTransHeight = aabb.Maxs.y - aabb.Mins.y; 
	
	m_pTransfiguredModel->SetPos(vCurPos);
	m_pTransfiguredModel->SetDirAndUp(GetDir(), GetUp());

	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

	if (m_pTransfiguredModel)
		m_pTransfiguredModel->PlayGfx(res_GFXFile(RES_GFX_TRANSFIGURE), NULL);
// 	if(m_TransfigureInfo.bReplaceName)
// 		m_pPateName->SetText(m_strReplacedName,false);
	return true;
}

bool CECPlayer::SetAircraftModelLoadResult(CECModel* pModel)
{
	return true;
}

//	Set loaded model to player object, this function is used in multithread loading process
bool CECPlayer::SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret)
{
	if (m_pBackupModel)
		return false;

	m_pPortraitModel = Ret.pPortraitModel;
	m_pPortraitModel->SetPos(g_vOrigin);
	m_pPortraitModel->SetDirAndUp(g_vAxisZ, g_vAxisY);
	m_pPortraitModel->PlayActionByNameDC("眨眼_通用", -1);
	m_pPortraitModel->Update(1);

	m_pPlayerModel	= Ret.pPlayerModel;
	m_pDummyModel	= Ret.pDummyModel;
	m_pBackupModel	= m_pPlayerModel;
	m_pBackupModel->SetAffectedByParent(false);
	m_pBackupModel->GetA3DSkinModel()->SetInheritTransFlag(false);

	if (m_pDummyModel)
		m_pDummyModel->SetAffectedByParent(false);

	if (Ret.pPetModel && !SetPetLoadResult(Ret.pPetModel))
		A3DRELEASE(Ret.pPetModel);

	if (Ret.pChangedModel && !SetChangedModelLoadResult(Ret.pChangedModel))
		A3DRELEASE(Ret.pChangedModel);

	if (Ret.pTransfiguredModel && !SetTransfigureModelLoadResult(Ret.pTransfiguredModel))
		A3DRELEASE(Ret.pTransfiguredModel);

	for (int i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (m_aHideEquips[i] > 0)
			m_pBackupModel->OnScriptChangeEquip(m_aHideEquips[i], 1, m_bFashionMode, GetEquipPathId(m_aHideEquips[i], m_iGender, m_iPhysique));
	}

	SetEquipsLoadedResult(Ret.EquipResult, false);

	//	Set model's position and orientation
	m_pPlayerModel->SetPos(GetPos());
	m_pPlayerModel->SetDirAndUp(GetDir(), GetUp());

	//	Force to update model once completely to avoid rendering error
	m_pPlayerModel->SetAutoUpdateFlag(false);
	m_pPlayerModel->Tick(1);
	m_pPlayerModel->SetAutoUpdateFlag(true);

	m_dwResFlags = RESFG_ALL;
	OnAllResourceReady();
	UpdateCurSkins();

	m_bLoadFlag = true;
	// 半透明的效果
	if (m_bInvisible || m_bDim)
		m_pPlayerModel->SetTransparent(0.7f);
	
	SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
	if (pInfo && (pInfo->mask_sect&(1<<SECT_15)))
	{
		AddFootGFX(m_iGender);
	}

	if (m_iCultivation != 0 && m_BasicProps.iDeityLevel > 0)
	{
		AddDeityGFX();
	}

// 	if (pInfo && (pInfo->mask_sect&(1<<SECT_13)))
// 	{
// 		AddSpineGFX();
// 	}

	return true;
}

bool CECPlayer::UpdateTalismanSprite(CECModel* pModel)
{
	if( m_pSprite)
	{
		m_pSprite->Release();
		delete m_pSprite;
		m_pSprite = NULL;
	}

	if (pModel)
	{
		m_pSprite = new CECSprite();

		if( !m_pSprite->Init(this, pModel) )
		{
			m_pSprite->Release();
			delete m_pSprite;
			m_pSprite = NULL;
			a_LogOutput(1, "CECPlayer::UpdateGodEvilSprite(), failed to load sprite!");
		}
		else
			m_pSprite->SetPos(GetPos() + A3DVECTOR3(0.0f, 2.5f, 0.0f));
	}

	return true;
}

bool CECPlayer::UpdateTuneSprite(CECModel* pModel)
{
	if( m_pSpriteTune)
	{
		m_pSpriteTune->Release();
		delete m_pSpriteTune;
		m_pSpriteTune = NULL;
	}
	
	if (pModel)
	{
		m_pSpriteTune = new CECSprite();
		
		if( !m_pSpriteTune->Init(this, pModel) )
		{
			m_pSpriteTune->Release();
			delete m_pSpriteTune;
			m_pSpriteTune = NULL;
			a_LogOutput(1, "CECPlayer::UpdateGodEvilSprite(), failed to load sprite!");
		}
		else
			m_pSpriteTune->SetPos(GetPos() + A3DVECTOR3(0.0f, 2.5f, 0.0f));
	}
	return true;
}

inline void _fade_out_gfx(A3DGFXEx*& pGfx, float fDeltaTime)
{
	if (!pGfx) return;

	if (pGfx->GetState() != ST_STOP)
	{
		float fAlpha = pGfx->GetAlpha();
		fAlpha -= fDeltaTime;

		if (fAlpha <= 0)
		{
			pGfx->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(pGfx);
			pGfx = NULL;
		}
		else
			pGfx->SetAlpha(fAlpha);
	}
	else
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(pGfx);
		pGfx = NULL;
	}
}

//	Tick routine
bool CECPlayer::Tick(DWORD dwDeltaTime)
{
	CECObject::Tick(dwDeltaTime);

	DWORD dwRealTime = g_pGame->GetRealTickTime();

	UpdateCurSkins();

	//	Incant time counter
// 	if (m_IncantCnt.IncCounter(dwRealTime))
// 		m_IncantCnt.Reset(true);
	//	Update last said words
// 	if (m_pPateLastWords1 && m_pPateLastWords1->GetItemNum())
// 	{
// 		if (m_strLastSayCnt.IncCounter(dwDeltaTime))
// 		{
// 			//	Clear string
// 			m_strLastSayCnt.Reset();
// 			m_pPateLastWords1->Clear();
// 			m_pPateLastWords2->Clear();
// 		}
// 		else
// 		{
// 			m_pPateLastWords1->Tick(dwDeltaTime);
// 			m_pPateLastWords2->Tick(dwDeltaTime);
// 		}
// 	}
	
// 	if (m_bScaleWeapon && m_pPlayerModel)
// 	{
// 		CECModel* pWeapon = m_pPlayerModel->GetChildModel(_weapon_hanger);
// 		if(m_ScaleWeaponCnt.IncCounter(dwDeltaTime))
// 		{
// 			m_ScaleWeaponCnt.Reset(true);
// 			m_bScaleWeapon = false;
// 			if (pWeapon)
// 				pWeapon->ScaleBoneEx(pWeapon->GetA3DSkinModel()->GetSkeleton()->GetBone(0)->GetName(), A3DVECTOR3(1.0f, 1.0f, 1.0f));
// 		}
// 		else
// 		{
// 			if (pWeapon)
// 			{
// 				float scale = (float)m_ScaleWeaponCnt.GetCounter() * 2.0f/m_ScaleWeaponCnt.GetPeriod();
// 				pWeapon->ScaleBoneEx(pWeapon->GetA3DSkinModel()->GetSkeleton()->GetBone(0)->GetName(), A3DVECTOR3(1.0f + scale, 1.0f + scale, 1.0f + scale));
// 			}
// 		}
// 	}

	if( m_bPateNeedUpdate )
		SetTitle(m_iTitle);

#ifdef BACK_VERSION
	A3DTerrainWater* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#else
	A3DTerrainWater2* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#endif
	float fDeltaTime = dwDeltaTime / 1000.0f;

	A3DVECTOR3 vPos = GetPos();
	float fWaterHei = pWater->GetWaterHeight(vPos);
	float fGrndHei = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos);
	float fWaterOff = vPos.y - fWaterHei;

	if (fWaterHei > fGrndHei  + .02f &&
		((fWaterOff > -10.f && fWaterOff <= 0) || (fWaterOff > 0 && fWaterOff < 10.0f && IsFlying())))
	{
		A3DMATRIX4 mat;

		if (pWater->IsUnderWater(g_pGame->GetViewport()->GetA3DCamera()->GetPos()))
			vPos.y = fWaterHei - .01f;
		else
			vPos.y = fWaterHei + .01f;

		A3DVECTOR3 vDir = GetDir();
		vDir.y = 0;
		vDir.Normalize();
		mat = a3d_TransformMatrix(vDir, g_vAxisY, vPos);

		if (IsPlayerMoving())
		{
			if (!m_pWaterWaveMove)
				m_pWaterWaveMove = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_MOVE));

			if (m_pWaterWaveMove)
			{
				if (m_pWaterWaveMove->GetState() == ST_STOP
				 || m_pWaterWaveMove->GetAlpha() < 1.0f)
				{
					m_pWaterWaveMove->SetAlpha(1.0f);
					float fSpeed = GetFlySpeed() / GetSwimSpeed();
					if (fSpeed > 2.0f) fSpeed = 2.0f;
					if (IsFlying()) m_pWaterWaveMove->SetPlaySpeed(fSpeed);
					else m_pWaterWaveMove->SetPlaySpeed(1.0f);
					m_pWaterWaveMove->Start(true);
				}

				m_pWaterWaveMove->SetParentTM(mat);
			}

			_fade_out_gfx(m_pWaterWaveStill, fDeltaTime);
		}
		else
		{
			if (!m_pWaterWaveStill)
				m_pWaterWaveStill = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_WATER_WAVE_STILL));

			if (m_pWaterWaveStill)
			{
				if (m_pWaterWaveStill->GetState() == ST_STOP
				 || m_pWaterWaveStill->GetAlpha() < 1.0f)
				{
					m_pWaterWaveStill->SetAlpha(1.0f);
					m_pWaterWaveStill->Start(true);
				}

				m_pWaterWaveStill->SetParentTM(mat);
			}

			_fade_out_gfx(m_pWaterWaveMove, fDeltaTime);
		}
	}
	else
	{
		_fade_out_gfx(m_pWaterWaveStill, fDeltaTime);
		_fade_out_gfx(m_pWaterWaveMove, fDeltaTime);
	}

	bool bFadeAirBubble = true;
	bool bFadeSwimBubble = true;

	if (!pWater->IsUnderWater(g_pGame->GetViewport()->GetA3DCamera()->GetPos()))
	{
		if (m_pAirBubble)
		{
			m_pAirBubble->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pAirBubble);
			m_pAirBubble = NULL;
		}

		if (m_pSwimBubble)
		{
			m_pSwimBubble->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSwimBubble);
			m_pSwimBubble = NULL;
		}

		bFadeAirBubble = false;
		bFadeSwimBubble = false;
	}
	else if (m_iMoveEnv == MOVEENV_WATER)
	{
		if (IsPlayerMoving())
		{
			if (!m_pSwimBubble)
				m_pSwimBubble = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_SWIM_BUBBLE));

			if (m_pSwimBubble)
			{
				if (m_pSwimBubble->GetState() == ST_STOP
				 || m_pSwimBubble->GetAlpha() < 1.0f)
				{
					m_pSwimBubble->SetAlpha(1.0f);
					m_pSwimBubble->Start(true);
				}

				A3DMATRIX4 mat = GetAbsoluteTM();
				A3DVECTOR3 vOff = GetPos() + GetDir() * .8f;
				vOff.y += 1.1f;
				mat.SetRow(3, vOff);
				m_pSwimBubble->SetParentTM(mat);
			}

			bFadeSwimBubble = false;
		}
		else
		{
			if (!m_pAirBubble)
				m_pAirBubble = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_AIR_BUBBLE));

			if (m_pAirBubble)
			{
				if (m_pAirBubble->GetState() == ST_STOP
				 || m_pAirBubble->GetAlpha() < 1.0f)
				{
					m_pAirBubble->SetAlpha(1.0f);
					m_pAirBubble->Start(true);
				}

				A3DMATRIX4 mat = GetAbsoluteTM();
				A3DVECTOR3 vOff = GetPos() + GetDir() * .1f;
				vOff.y += 1.6f;
				mat.SetRow(3, vOff);
				m_pAirBubble->SetParentTM(mat);
			}

			bFadeAirBubble = false;
		}
	}

	if (bFadeAirBubble) _fade_out_gfx(m_pAirBubble, fDeltaTime);
	if (bFadeSwimBubble) _fade_out_gfx(m_pSwimBubble, fDeltaTime);

	//	Update bubble text
	if (m_pBubbleTexts)
		m_pBubbleTexts->Tick(dwDeltaTime);

	if( m_pvp.iDuelState == DUEL_ST_INDUEL )
	{
		if (!m_pDuelStateGFX)
			m_pDuelStateGFX = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_INDUEL));

		if (m_pDuelStateGFX)
		{
			if (m_pDuelStateGFX->GetState() == ST_STOP || m_pDuelStateGFX->GetAlpha() < 1.0f)
			{
				m_pDuelStateGFX->SetAlpha(1.0f);
				m_pDuelStateGFX->Start(true);
			}

			A3DMATRIX4 mat = GetAbsoluteTM();
			m_pDuelStateGFX->SetParentTM(mat);
		}
	}
	else
	{
		if (m_pDuelStateGFX)
		{
			m_pDuelStateGFX->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pDuelStateGFX);
			m_pDuelStateGFX = NULL;
		}
	}

	// 使用了VIP卡，播放相应的特效
	if( m_iVipState != NO_VIP_STATE)
	{
		if(!m_pVipStateGFX)
			m_pVipStateGFX = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_VIPSTATE1+m_iVipState-1));

		if(m_pVipStateGFX)
		{
			if (m_pVipStateGFX->GetState() == ST_STOP || m_pVipStateGFX->GetAlpha() < 1.0f )
			{
				m_pVipStateGFX->SetAlpha(1.0f);
				m_pVipStateGFX->Start(true);
			}

			A3DMATRIX4 mat = GetAbsoluteTM();
			m_pVipStateGFX->SetParentTM(mat);
		}
	}
	else
	{
		if(m_pVipStateGFX)
		{
			m_pVipStateGFX->Stop();
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pVipStateGFX);
			m_pVipStateGFX = NULL;
		}
	}	

	if( m_pSprite )
		m_pSprite->Tick(dwDeltaTime);
	
	if( m_pSpriteTune )
		m_pSpriteTune->Tick(dwDeltaTime);

	if (m_idCandBuddy)
		AttachBuddy(m_idCandBuddy);
	
	if(m_bMultiRideLeader)
	{
		for (int i = 1; i< 6; ++i)
		{
			if (m_iMultiCandBuddyId[i])
				AttachMultiBuddy(m_iMultiCandBuddyId[i], i);
		}
	}
	
	if(m_iCandLeaderId && m_pBackupModel)
	{
		CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iCandLeaderId);
		if(pLeader && pLeader->GetPlayerModel())
		{
			pLeader->AttachMultiBuddy(m_PlayerInfo.cid, m_iCandMultiRidePos);
			m_iCandLeaderId = 0;
		}
	}

	if (m_CandPet.id && m_pBackupModel)
	{
		const char* szPetPath = GetRidingPetFileName(m_CandPet.id, m_CandPet.iLevel);
		QueueLoadPetModel(szPetPath, false);
		m_CandPet.id = 0;
		m_CandPet.iLevel = 0;
	}

	if (m_iCandChangeShape && m_pBackupModel)
	{
		const char* szPath = GetChangedModelFileName(m_iChangeShape, m_iGender, m_BasicProps.iProfession);
		QueueLoadChangedModel(szPath, false);
		m_iCandChangeShape = 0;
	}

	if (m_CandTransfigure.id && m_pBackupModel)
	{
		const char* szPath = GetTransfiguredModelFileName(m_CandTransfigure.id);
		QueueLoadTransfiguredModel(szPath, false);
		m_CandTransfigure.id = 0;
	}

	// 如果正处于归神/脱灵技能状态下（m_fScaleShapeRatio不为0），不断处理模型的缩放
	// 原因是：玩家在很多状态下都可能会改变子模型/主模型，而如果分散处理这些由于模型
	// 改变而重新进行缩放操作的话，程序结构变得很难维护。（当然，除非使用新的设计模式如：
	// 观察者模式等）
	if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
	{
		//处理缩放/还原
		ScaleObjects( m_pPlayerModel, m_fScaleShapeRatio );

		//如果此时有坐骑，则缩放/还原坐骑 Added 2011-08-17.
		if ( m_pPetModel )
		{
			ScaleObjects( m_pPetModel, m_fScaleShapeRatio );
		}
	}

	return true;
}

//	Process message
bool CECPlayer::ProcessMessage(const ECMSG& Msg)
{
	return true;
}

//	Play a damaged effect
void CECPlayer::Damaged(int nDamage, int nDivisions, DWORD dwModifier/* 0 */)
{
	if ( dwModifier & MOD_DEITY_DAMAGE  )
	{
		if ( nDamage > 0)
		{
			int index = BUBBLE_DEITY_DAMAGE;
			bool bDeadlyStrike = (dwModifier & MOD_DT_CRITICAL_STRIKE) ? true : false;
			bool bRetort = (dwModifier & MOD_RETORT) ? true : false;
			PlayAction(ACT_WOUNDED, 1.0f);	

			int p1 = 0;
			if (bDeadlyStrike)
				p1 |= 0x0004;
			else if (bRetort)
				p1 |= 0x0002;
			
			if (nDivisions < 2)
			{
				BubbleText(index, (DWORD)nDamage,0, p1);
			}
			else
			{
				int* p = new int[nDivisions];
				_get_rand_damage(nDamage, nDivisions, p);

				for (int i = 0; i < nDivisions; i++)
					BubbleText(index, p[i], 0, p1, i + 1 == nDivisions, true);

				delete[] p;
			}
		}
		return;
	}


	if (nDamage == -2)
	{
		// this message is caused by a help skill, so don't use a wounded action here
		if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
	}
	else if (nDamage == -1)
	{
		// when else player hit this player iDamage is -1, 
		// Just play a wounded action
		PlayAction(ACT_WOUNDED, 1.0f);
		if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
	}
	else
	{
		//	Popup a damage decal
		if (nDamage > 0)
		{
			PlayAction(ACT_WOUNDED, 1.0f);

			int p1 = 0;
			if (dwModifier & MOD_CRITICAL_STRIKE)
				p1 |= 0x0001;

			if (dwModifier & MOD_RETORT)
				p1 |= 0x0002;

			int index = dwModifier & MOD_SKILL ? BUBBLE_SKILL_DAMAGE:BUBBLE_DAMAGE;

			if (nDivisions < 2)
				BubbleText(index, nDamage, 0, p1);
			else
			{
				int* p = new int[nDivisions];
				_get_rand_damage(nDamage, nDivisions, p);

				for (int i = 0; i < nDivisions; i++)
					BubbleText(index, p[i], 0, p1, i + 1 == nDivisions, true);

				delete[] p;
			}
			
			if (dwModifier & MOD_IMMUNE)
				BubbleText(BUBBLE_IMMUNE, 0);
			else if (dwModifier & MOD_RESIST)
				BubbleText(BUBBLE_RESIST, 0);
		}
		else if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if (dwModifier & MOD_RESIST)
			BubbleText(BUBBLE_RESIST, 0);
		else if (dwModifier & MOD_NULLITY)
			BubbleText(BUBBLE_INVALIDHIT, 0);
		else
			BubbleText(BUBBLE_HITMISSED, 0);
	}
}

//	Play an attack effect
void CECPlayer::PlayAttackEffect(int idTarget, int idSkill, int skillLevel, unsigned char cSerialId, int nDamage,
							DWORD dwModifier, int* piAttackTime/* NULL */)
{
	if (!IsAllResReady())
		return;

	if (m_iSpecialState & SPECIAL_STATE_FROZEN)
	{
		return;
	}
}

//	Set absolute position
void CECPlayer::SetPos(const A3DVECTOR3& vPos)
{
	if (_isnan(vPos.x) || _isnan(vPos.y) || _isnan(vPos.z))
		return;

	CECObject::SetPos(vPos);

	m_aabb.Center = vPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();

	if (m_pTransfiguredModel)
		m_pTransfiguredModel->SetPos(vPos);

	if (m_pChangedModel)
		m_pChangedModel->SetPos(vPos);

	if (m_pPetModel)
		m_pPetModel->SetPos(vPos);

	if (m_pBackupModel)
		m_pBackupModel->SetPos(vPos);

	if (m_pDummyModel)
		m_pDummyModel->SetPos(vPos);

	if (m_iBuddyId && !m_bHangerOn)
	{
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
		if (pBuddy)
			pBuddy->SetPos(vPos);
	}
	
	if (m_bMultiRideLeader)
	{
		for(int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if(m_iMultiBuddyId[i])
			{		
				CECPlayer* pMember = m_pPlayerMan->GetPlayer(m_iMultiBuddyId[i]);
				if (pMember)
					pMember->SetPos(vPos);
			}
		}
 	}
	//	test code ...
	/*
	if (g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		float fHei = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos);
		if (fHei > vPos.y + 0.001f)
		{
			g_pGame->RuntimeDebugInfo(0xffffffff, _AL("%f"), fHei - vPos.y);
		}
	}
	*/
}

//	Set absolute forward and up direction
void CECPlayer::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	CECObject::SetDirAndUp(vDir, vUp);

	A3DVECTOR3 vNewDir = GetDir();
	A3DVECTOR3 vNewUp = GetUp();

	if (m_pTransfiguredModel)
		m_pTransfiguredModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pChangedModel)
		m_pChangedModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pPetModel)
		m_pPetModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pBackupModel)
		m_pBackupModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pDummyModel)
		m_pDummyModel->SetDirAndUp(vNewDir, vNewUp);
}

//	Play model action by weapon and relative action index
bool CECPlayer::PlayAction(int iAction, float fWeight, bool bRestart/* true */, int iTransTime/* 200 */, bool bQueue/* false */)
{
	// GetCurActionUserData(ACT_CHANNEL_BODY)  may return -1;
	if (iAction < 0)
		iAction = 0;

	if (m_iSpecialState & SPECIAL_STATE_FROZEN)
	{
		return true;
	}

	if (m_pTransfiguredModel)
	{
		if (iAction >= ACT_ATTACK_1 && iAction <= ACT_ATTACK_4 )
		// attack action should not played by this function
			return false;

		int iNPCAction = GetMappedAction(iAction);

		if (!bQueue)
		{
			m_pTransfiguredModel->PlayActionByName(CECNPC::GetActionName(iNPCAction), fWeight, bRestart, iTransTime, true, iAction);
		}
		else
		{
			m_pTransfiguredModel->QueueAction(CECNPC::GetActionName(iNPCAction), iTransTime, iAction, bRestart ? true : false);
			m_pTransfiguredModel->SetComActFlag(NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);
		}

		return true;		
	}

	if (IsFighting())
	{
		if (iAction == ACT_JUMP_START)
			iAction = ACT_FIGHT_JUMP_START;
		else if (iAction == ACT_JUMP_LOOP)
			iAction = ACT_FIGHT_JUMP_LOOP;
		else if (iAction == ACT_JUMP_LAND)
			iAction = ACT_FIGHT_JUMP_LAND;
		else if (iAction == ACT_RUN)
			iAction = ACT_FIGHT_RUN;
		else if (iAction == ACT_QINGGONG_LOOP)
			iAction = ACT_FIGHT_QINGGONG_LOOP;
		else if (iAction == ACT_FLY_WING)
			iAction = ACT_FIGHT_FLY_WING;
		else if (iAction == ACT_HANGINAIR_WING)
			iAction = ACT_FIGHT_HANGINAIR_WING;
// 		else if (iAction == ACT_FLY_SWORD)
// 			iAction = ACT_FIGHT_FLY_SWORD;
// 		else if (iAction == ACT_HANGINAIR_SWORD)
// 			iAction = ACT_FIGHT_HANGINAIR_SWORD;
// 		else if (iAction == ACT_FLY_FEIQI)
// 			iAction = ACT_FIGHT_FLY_FEIQI;
// 		else if (iAction == ACT_HANGINAIR_FEIQI)
// 			iAction = ACT_FIGHT_HANGINAIR_FEIQI;
	}
	int iAction2= iAction;
	// 虚弱状态 非骑乘、绑定状态下有效
	if (m_BasicProps.iCurHP * 2 < m_ExtProps.max_hp )
	{
		if (iAction == ACT_RUN)
			iAction2 = ACT_RUN_WEAK;
		else if (iAction == ACT_FIGHT_RUN)
			iAction2 = ACT_FIGHT_RUN_WEAK;
		else if (iAction == ACT_STAND)
			iAction2 = ACT_STAND_WEAK;
		else if (iAction == ACT_FIGHTSTAND)
			iAction2 = ACT_FIGHTSTAND_WEAK;
		else if (iAction == ACT_WALK)
			iAction2 = ACT_WALK_WEAK;
	}
	
	if (m_bInvisible)
	{
		if (iAction == ACT_RUN)
			iAction2 = ACT_WALK_SNEAK;
		else if (iAction == ACT_FIGHT_RUN)
			iAction2 = ACT_FIGHT_WALK_SNEAK;
		else if (iAction == ACT_STAND)
			iAction2 = ACT_STAND_SNEAK;
		else if (iAction == ACT_FIGHTSTAND)
			iAction2 = ACT_FIGHTSTAND_SNEAK;
		else if (iAction == ACT_WALK)
			iAction2 = ACT_WALK_SNEAK;
	}

	if (m_pChangedModel)
	{
		if( iAction >= ACT_ATTACK_1 && iAction <= ACT_ATTACK_4 )
		{
			// attack action should not be played by this function
			return false;
		}

		//Added 2011-09-16.	在播放动作之前，先判断下动作是否存在，如果不存在，则不需再执行播放动作接口，避免SetComActFlag清除之前动作的特效，特殊需要。。
		A3DCombinedAction* pComAct = m_pChangedModel->GetComActByName(m_ActionNames.GetANSIString(iAction));
		if ( !pComAct )
		{
			if ( !bQueue )	//变身状态下，如果非循环动作，则使用默认动作,避免第一次播放一个不存在的动作时,没有特效(怀光坐上飞剑,变身后无任何形象的问题)
			{
				iAction = ACT_STAND;//默认动作
			}
			else			//否则，返回
			{
				return false;
			}
		}
		//Added end.

		if (!bQueue)
		{
			m_pChangedModel->PlayActionByName(m_ActionNames.GetANSIString(iAction), fWeight, bRestart, iTransTime, true, iAction);
		}
		else
		{ 
			m_pChangedModel->QueueAction(m_ActionNames.GetANSIString(iAction), iTransTime, iAction, bRestart ? true : false);
			m_pChangedModel->SetComActFlag(NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX); //该代码清除了前一个动作的gfx？导致焚香变身火焰后，播放一些动作时，火焰消失
		}

		return true;
	}
	else if (m_pPlayerModel)
	{
		if (iAction != ACT_WOUNDED)
		{
			if( iAction >= ACT_ATTACK_1 && iAction <= ACT_ATTACK_4 )
			{
				// attack action should not be played by this function
				return false;
			}
			else
			{
				char szAct[64];

				if( iAction == ACT_TWO_KISS )
				{
					// 亲亲密密的修改，由于增加了两个体型，对女的动作进行调整
					if (m_AttachMode == enumAttachHugPlayer)
					{
						if (m_iGender == GENDER_FEMALE)
						{
							if (m_iPhysique == PHYSIQUE_NORMAL)
								sprintf(szAct, "亲吻_绑定_老男");
							else if (m_iPhysique == PHYSIQUE_BIG)
								sprintf(szAct, "亲吻_绑定_大男");
							else if (m_iPhysique == PHYSIQUE_MIDDLE)
								sprintf(szAct, "亲吻_绑定_中男");
							else if (m_iPhysique == PHYSIQUE_SMALL)
								sprintf(szAct, "亲吻_绑定_小男");
						}
						else
							sprintf(szAct, "亲吻_绑定_通用");
						
					}
					else if (m_AttachMode == enumAttachRideOnPet)
					{
						if (m_iGender == GENDER_FEMALE)
						{
							if (m_iPhysique == PHYSIQUE_NORMAL)
								sprintf(szAct, "亲吻_双骑_老男");
							else if (m_iPhysique == PHYSIQUE_BIG)
								sprintf(szAct, "亲吻_双骑_大男");
							else if (m_iPhysique == PHYSIQUE_MIDDLE)
								sprintf(szAct, "亲吻_双骑_中男");
							else if (m_iPhysique == PHYSIQUE_SMALL)
								sprintf(szAct, "亲吻_双骑_小男");
						}
						else
							sprintf(szAct, "亲吻_双骑_通用");
					}
				}
				else if (iAction == ACT_PICKUP_MATTER)
				{
					if (m_AttachMode == enumAttachNone && !m_pPetModel)
						sprintf(szAct, m_ActionNames.GetANSIString(iAction));
					else
						return true;
				}
				else if (IsInDomainState())
				{
					if (m_pPetModel)										// 骑乘状态
						sprintf(szAct, "天华_骑乘_领域");
					else if (IsFlying() && m_nFlyMode == FLYMODE_FEIQI)		// 飞骑，用骑乘动作
					{
						sprintf(szAct, "天华_骑乘_领域");
					}
					else if (IsFlying() && m_nFlyMode == FLYMODE_WING)		// 翅膀
					{
						sprintf(szAct, "天华_翅膀_领域");
					}
					else													// 地面或者飞剑上面
						sprintf(szAct, "天华_领域");	
				}
				else
				{
					int nActType = m_uAttackType;
					if( nActType < 0 ) nActType = 0;
					if( nActType > 12 ) nActType = 12;
					const char * szSuffix = m_ActionNames.GetANSIString(nActType + ACT_SUFFIX_0);

					if (m_AttachMode == enumAttachHugPlayer) // male player hug female player
					{
						sprintf(szAct, "%s_绑定_通用",m_ActionNames.GetANSIString(iAction));
					}
					else if (m_AttachMode == enumAttachRideOnPet) // male player hug female player on a vehicle
						sprintf(szAct, "%s_双骑_通用",m_ActionNames.GetANSIString(iAction));
					else if( m_pPetModel ) // is riding alone
						sprintf(szAct, "%s_骑乘_%s",m_ActionNames.GetANSIString(iAction), szSuffix);
					else if( m_bMultiRideMember)
					{
						CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);
						if (pLeader && pLeader->GetRidingPetInfo().iMultiRideMode)    //多人骑乘之车马模式，乘客播放“乘坐”动作
							sprintf(szAct, "乘坐_%s", szSuffix);
						else
							sprintf(szAct, "%s_骑乘_%s", m_ActionNames.GetANSIString(iAction), szSuffix);
					}
					else // normal
					{
						SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
						// 空手模式下，将怀光和烈山的动作区分，在怀光的空手动作的后面加上后缀"_怀光"
						if (pInfo && (pInfo->mask_sect&(1<<SECT_13)) && (m_uAttackType == 0) && !IsFlying() )
						{
							sprintf(szAct, "%s_%s_怀光",m_ActionNames.GetANSIString(iAction2), szSuffix);
						}
						else if ( pInfo && (pInfo->mask_sect&(1<<SECT_12)) && (m_uAttackType == 0) && !IsFlying() )
						{
							//空手模式下，区别太昊和九黎的动作，将太昊的空手动作后面加上后缀"_太昊"
							sprintf(szAct, "%s_%s_太昊",m_ActionNames.GetANSIString(iAction2), szSuffix);
						}
						else
						{
							sprintf(szAct, "%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix);	
						}
					}	
				}

				const char * szPetAct = NULL;

				if (m_pPetModel)
				{
					if (iAction == ACT_STAND)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_STAND);
					else if (iAction == ACT_RUN || iAction == ACT_FIGHT_RUN)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_RUN);
					else if (iAction == ACT_WALK)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_WALK);
					else if (iAction == ACT_JUMP_START || iAction == ACT_FIGHT_JUMP_START)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_JUMP_START);
					else if (iAction == ACT_JUMP_LAND || iAction == ACT_FIGHT_JUMP_LAND)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_JUMP_LAND);
					else if (iAction == ACT_JUMP_LOOP || iAction == ACT_FIGHT_JUMP_LOOP)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_JUMP_LOOP);
					else if (iAction == ACT_FIGHTSTAND)
						szPetAct = CECNPC::GetActionName(CECNPC::ACT_GUARD);
				}

				if( !bQueue )
				{
					m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY, szAct, fWeight, bRestart, iTransTime, true, iAction);
					char szEyeAct[64];
					sprintf(szEyeAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
					m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szEyeAct, fWeight, bRestart, iTransTime, true, iAction);

					if (m_pPetModel && szPetAct)
						m_pPetModel->PlayActionByName(szPetAct, fWeight, bRestart, iTransTime, true);
				}
				else
				{ 
					m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, iTransTime, iAction, bRestart ? true : false);
					m_pPlayerModel->SetComActFlag(ACT_CHANNEL_BODY, NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);

					char szEyeAct[64];
					sprintf(szEyeAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
					m_pPlayerModel->QueueAction(ACT_CHANNEL_EYE, szEyeAct, iTransTime, iAction, bRestart ? true : false);
					m_pPlayerModel->SetComActFlag(ACT_CHANNEL_EYE, NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);

					if (m_pPetModel && szPetAct)
						m_pPetModel->QueueAction(szPetAct, iTransTime);
				}

				// now let the buddy acts the same as me.
				if (m_iBuddyId && !m_bHangerOn)
				{
					CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
					if (pBuddy)
						pBuddy->PlayAction(iAction, fWeight, bRestart, iTransTime, bQueue);
				}
				// 如果是多人骑乘，所有成员的动作与主骑相同
				if(m_bMultiRideLeader)
				{
					for (int i =1; i< 6; ++i)
					{
						if( m_iMultiBuddyId[i])
						{
							CECPlayer* pMember = m_pPlayerMan->GetPlayer(m_iMultiBuddyId[i]);
							if(pMember)
								pMember->PlayAction(iAction, fWeight, bRestart, iTransTime, bQueue);
						}
					}
				}
			}
		}
		else
		{
			m_pPlayerModel->GetA3DSkinModel()->PlayActionByName(m_ActionNames.GetANSIString(iAction), ACT_CHANNEL_WOUND);
		}

		return true;
	}
	else
		return false;
}

//	Set part extend properties
void CECPlayer::SetPartExtendProps(int iPropIdx, void* pData)
{
	switch (iPropIdx)
	{
	case EXTPROPIDX_BASE:
	
		m_ExtProps.max_hp = ((ROLEEXTPROP_BASE*)pData)->max_hp;
		m_ExtProps.max_mp = ((ROLEEXTPROP_BASE*)pData)->max_mp;
		if (GetCloneID())
		{
			CECPlayer* pClone = m_pPlayerMan->GetPlayer(GetCloneID());
			if (pClone)
			{
				ROLEEXTPROP& ep2 = pClone->GetExtendProps();
				ep2.max_hp		= ((ROLEEXTPROP_BASE*)pData)->max_hp;
				ep2.max_mp		= ((ROLEEXTPROP_BASE*)pData)->max_mp;
			}
		}
		break;
	
	case EXTPROPIDX_MOVE:
	
		m_ExtProps.walk_speed = ((ROLEEXTPROP_MOVE*)pData)->walk_speed;
		m_ExtProps.run_speed = ((ROLEEXTPROP_MOVE*)pData)->run_speed;
		break;
	
	case EXTPROPIDX_ATTACK:
	
		m_ExtProps.attack		= ((ROLEEXTPROP_ATK*)pData)->attack;
		m_ExtProps.damage_low	= ((ROLEEXTPROP_ATK*)pData)->damage_low;
		m_ExtProps.damage_high	= ((ROLEEXTPROP_ATK*)pData)->damage_high;
		m_ExtProps.attack_range	= ((ROLEEXTPROP_ATK*)pData)->attack_range;
		m_ExtProps.crit_rate	= ((ROLEEXTPROP_ATK*)pData)->crit_rate;
		m_ExtProps.crit_damage	= ((ROLEEXTPROP_ATK*)pData)->crit_damage;
		break;
	
	case EXTPROPIDX_DEF:

		memcpy(m_ExtProps.resistance, ((ROLEEXTPROP_DEF*)pData)->resistance, sizeof(m_ExtProps.resistance));
		m_ExtProps.defense		= ((ROLEEXTPROP_DEF*)pData)->defense;
		m_ExtProps.armor		= ((ROLEEXTPROP_DEF*)pData)->armor;
		break;
	
	default:
		ASSERT(0);
		return;
	}
}

/*
 *	Add By Zhangyu, 12.22.04
 */

void CECPlayer::ShowEquipments(
	int iPhysique,
	int iProfession, 
	int iDefaultSkin,
	int nGender,
	bool bHasDummy,
	const int* pEquipmentID,
	unsigned int uChangeMask,
	EquipsLoadResult* pResult)
{
	DATA_TYPE	dt;
	A3DCOLOR	color = 0;

	// keep old things.
	pResult->stoneWeapon	= 0xff;
	pResult->stoneHead		= 0xff;
	pResult->stoneBody		= 0xff;
	pResult->stoneFoot		= 0xff;

	for (int i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (((1 << i) & uChangeMask))
		{
			int idEquipment = pEquipmentID[i];

			/*
			if( i >= EQUIPIVTR_FASHION_BODY && i <= EQUIPIVTR_FASHION_WRIST )
			{
				WORD wColor = (idEquipment & 0xffff0000) >> 16;
				idEquipment &= 0x0000ffff;
				color = FASHION_WORDCOLOR_TO_A3DCOLOR(wColor);
			}
			else */
			{
				if( i == EQUIPIVTR_WEAPON )
				{
					pResult->stoneWeapon = (idEquipment & 0x00ff0000) >> 16;
				}
				if( i == EQUIPIVTR_HEAD )
				{
					pResult->stoneHead = (idEquipment & 0x00ff0000) >> 16;
				}
				if( i == EQUIPIVTR_BODY )
				{
					pResult->stoneBody = (idEquipment & 0x00ff0000) >> 16;
				}
				if( i == EQUIPIVTR_FOOT )
				{
					pResult->stoneFoot = (idEquipment & 0x00ff0000) >> 16;
				}
				if( i == EQUIPIVTR_MAIN_TALISMAN || i == EQUIPIVTR_WING)
				{
					pResult->talisman_quality = max(0, (min(4, (idEquipment & 0x00ff0000) >> 16)-1));
				}
				idEquipment &= 0x0000ffff;
			}

			pResult->aEuips[i] = pEquipmentID[i];

			if (idEquipment /*&& iRace == RACE_HUMAN*/)
			{
				const EQUIPMENT_ESSENCE* pEquip = static_cast<const EQUIPMENT_ESSENCE*>(g_pGame->GetElementDataMan()->get_data_ptr(
					idEquipment,
					ID_SPACE_ESSENCE,
					dt));

				if (pEquip)
				{
					if (dt == DT_EQUIPMENT_ESSENCE)
					{
						switch (pEquip->equip_type)
						{
						case 0:		// 武器
							ChangeWeapon(pResult, iPhysique, nGender, pEquip);
							continue;
						case 1:		// 防具
							ChangeArmor(
								pEquip,
								pResult->dwShowMask,
								pResult->dwSkinChangeMask,
								iPhysique,
								iProfession,
								nGender,
								iDefaultSkin,
								pResult->aSkins);
							continue;

						case 3:		// 时装
							ChangeFashion(
								pEquip,
								pResult->dwFashionShowMask,
								pResult->dwSkinChangeMask,
								iPhysique,
								iProfession,
								nGender,
								iDefaultSkin,
								pResult->aSkins,
								color
								);
							continue;

						default:
							continue;
						}
					}
					else if (dt == DT_TALISMAN_MAINPART_ESSENCE)
					{
						TALISMAN_MAINPART_ESSENCE* pTalisman = (TALISMAN_MAINPART_ESSENCE*)pEquip;

						if (pTalisman->is_aircraft)
						{
							pResult->nFlyMode = pTalisman->fly_mode;
							ChangeWing(pResult, g_pGame->GetDataPath(pTalisman->file_model[pResult->talisman_quality]));
						}
						else
							ChangeTalisman(pResult, g_pGame->GetDataPath(pTalisman->file_model[pResult->talisman_quality]));

						continue;
					}
					else if (dt == DT_RUNE_EQUIP_ESSENCE)
					{
						RUNE_EQUIP_ESSENCE* pTune = (RUNE_EQUIP_ESSENCE*)pEquip;
						ChangeTune(pResult, g_pGame->GetDataPath(pTune->file_model));
					}
				}
				else
					a_LogOutput(1, "Equipment Invent: WrongEquipType: Id = %d", idEquipment);
			}

			if (i == EQUIPIVTR_BODY)
			{
				pResult->dwShowMask |= (1 << enumSkinShowUpperBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_BODY_INDEX);
				pResult->stoneBody = 0;
				ChangeDefaultUpper(pResult->aSkins[SKIN_BODY_INDEX], iPhysique, iProfession, nGender, iDefaultSkin);
			}
			else if (i == EQUIPIVTR_FOOT)
			{
				pResult->dwShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FOOT_INDEX);
				pResult->stoneFoot = 0;
				ChangeDefaultFoot(pResult->aSkins[SKIN_FOOT_INDEX], iPhysique, iProfession, nGender, iDefaultSkin);
			}
			else if (i == EQUIPIVTR_WEAPON)
			{
				pResult->bWeaponChanged = true;	
				pResult->stoneWeapon = 0;
				pResult->uAttackType = 0;
			}
			else if (i == EQUIPIVTR_MAIN_TALISMAN)
			{
				pResult->bTalismanChanged = true;
			}
			else if (i == EQUIPIVTR_FASHION_BODY)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowUpperBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_UPPER_BODY_INDEX);
				ChangeDefaultFashionUpper(pResult->aSkins[SKIN_FASHION_UPPER_BODY_INDEX], iPhysique, iProfession, nGender, iDefaultSkin);
			}
			else if (i == EQUIPIVTR_FASHION_LEG)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowLowerBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_LOWER_INDEX);
				ChangeDefaultFashionLower(pResult->aSkins[SKIN_FASHION_LOWER_INDEX], iPhysique, iProfession, nGender, iDefaultSkin);
			}
			else if (i == EQUIPIVTR_FASHION_FOOT)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_FOOT_INDEX);
				ChangeDefaultFashionFoot(pResult->aSkins[SKIN_FASHION_FOOT_INDEX], iPhysique, iProfession, nGender, iDefaultSkin);
			}
			else if (i == EQUIPIVTR_FASHION_EYE)
			{
				pResult->dwFashionShowMask &= ~(1 << enumSkinShowGlasses);
				pResult->dwSkinChangeMask |= (1 << SKIN_GLASSES_INDEX);
			}
			else if (i == EQUIPIVTR_FASHION_NOSE)
			{
				pResult->dwFashionShowMask &= ~(1 << enumSkinShowNose);
				pResult->dwSkinChangeMask |= (1 << SKIN_NOSE_INDEX);
			}
			else if (i == EQUIPIVTR_FASHION_LIP)
			{
				pResult->dwFashionShowMask &= ~(1 << enumSkinShowMustache);
				pResult->dwSkinChangeMask |= (1 << SKIN_MUSTACHE_INDEX);
			}
			else if (i == EQUIPIVTR_FASHION_BACK)
			{
				pResult->dwFashionShowMask &= ~(1 << enumSkinShowCloak);
				pResult->dwSkinChangeMask |= (1 << SKIN_CLOAK_INDEX);
			}
			else if (i == EQUIPIVTR_HEAD)
			{
				pResult->dwShowMask &= ~(1 << enumSkinShowHead);
				pResult->dwSkinChangeMask |= (1 << SKIN_HELM_INDEX);
				pResult->stoneHead = 0;
			}
			else if (i == EQUIPIVTR_FASHION_HEAD)
			{
				pResult->dwFashionShowMask &= ~(1 << enumSkinShowHead);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_HEADWEAR_INDEX);
			}
			else if (i == EQUIPIVTR_WING)
				pResult->bWingChanged = true;
		}
	}
}

void CECPlayer::ShowEquipments(const int* pEquipmentID, bool bHide, bool bLoadAtOnce)
{
	if( !m_pBackupModel || !m_pBackupModel->GetA3DSkinModel())
		return;

	DWORD dwMask = 0; 
	int aTempEquips[SIZE_EQUIPIVTR];
	int* pOld;
	const int* pNew;
	UpdateHideEquipMask(pEquipmentID);

	int i;
    for(i = 0; i < SIZE_EQUIPIVTR; ++i)
	{
		if(m_dwHideEquipMaskInUse & (1<<i))
			aTempEquips[i] = 0;
		else
			aTempEquips[i] = pEquipmentID[i];
	}

	if(bHide)
	{
		pOld = m_aHideEquips;
		pNew = aTempEquips;
	}
	else
	{
		pOld = m_aEquips;
		pNew = pEquipmentID;
	}

	for ( i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (pNew[i] != pOld[i])
		{
			if (pOld[i] > 0)
				m_pBackupModel->OnScriptChangeEquip(pOld[i], 0, m_bFashionMode, GetEquipPathId(pOld[i], m_iGender, m_iPhysique));

			if (pNew[i] > 0)
				m_pBackupModel->OnScriptChangeEquip(pNew[i], 1, m_bFashionMode, GetEquipPathId(pNew[i], m_iGender, m_iPhysique));

			dwMask |= (1 << i);
			pOld[i] = pNew[i];
		}
		m_aEquips[i] = pEquipmentID[i];             // necessary? 
	}

	QueueLoadEquips(pNew, dwMask, bLoadAtOnce);
}

int CECPlayer::GetEquipPathId(int nEquipId, int nGender, int nPhysique)
{
	DATA_TYPE dt;
	const EQUIPMENT_ESSENCE* pEquip = static_cast<const EQUIPMENT_ESSENCE*>(g_pGame->GetElementDataMan()->get_data_ptr((nEquipId&0x0000ffff), ID_SPACE_ESSENCE, dt));

	if (pEquip)
	{
		if (dt == DT_EQUIPMENT_ESSENCE)
		{
			if (nPhysique == PHYSIQUE_NORMAL || nPhysique == PHYSIQUE_MIDDLE)
			{
				return (nGender == 0) ? pEquip->file_model_male : pEquip->file_model_female;
			}
			else if(nPhysique == PHYSIQUE_BIG)
			{
				return (nGender == 0) ? pEquip->file_model_big_male : pEquip->file_model_big_female;
			}
			else if(nPhysique == PHYSIQUE_SMALL)
			{
				return (nGender == 0) ? pEquip->file_model_small_male : pEquip->file_model_small_female;
			}
			else
				return 0;
// 			if (nGender == 0)
// 				return pEquip->file_model_male;
// 			else
// 				return pEquip->file_model_female;
		}
	}

	return 0;
}

//	Show / hide wing
void CECPlayer::ShowWing(bool bShow)
{
	if (m_pBackupModel)
	{
		CECModel* pWing = m_pBackupModel->GetChildModel(_wing);

		if (pWing)
			pWing->Show(bShow);
	}
}

inline void _SetWeaponMaterial(A3DSkinModel* pModel)
{
	// Don't set material automatically, the artist should tune the material in 3DSMax
	return;

	if (!pModel) return;

	int nNumSkin = pModel->GetSkinNum();
	A3DSkin * pSkin;

	for (int i = 0; i < nNumSkin; i++)
	{
		pSkin = pModel->GetA3DSkin(i);

		if( pSkin )
		{
			int nNumMaterial = pSkin->GetMaterialNum();

			for (int i = 0; i < nNumMaterial; i++)
			{
				A3DMATERIALPARAM param = pSkin->GetMaterial(i)->GetMaterialParam();
				param.Specular = WEAPON_SPECULAR;
				param.Power = WEAPON_POWER;
				pSkin->GetMaterial(i)->SetMaterialParam(param);
			}
		}
	}
}

bool CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, const char* szLeft, const char* szRight, const char* szBack)
{
	pResult->bWeaponChanged = true;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);

	if (szLeft[0])
	{
		CECModel* pWeapon = new CECModel;

		if (pWeapon->Load(szLeft, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pLeftHandWeapon = pWeapon;
		}
		else
			delete pWeapon;
	}

	if (szRight[0])
	{
		CECModel* pWeapon = new CECModel;

		if (pWeapon->Load(szRight, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pRightHandWeapon = pWeapon;
		}
		else
			delete pWeapon;
	}

	//Added 2011-08-09.
	//增加对背部武器的加载的处理
	if ( szBack[0] )
	{
		CECModel* pWeapon = new CECModel;

		if ( pWeapon->Load(szBack, true, A3DSkinModel::LSF_UNIQUESKIN) )
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pBackWeapon = pWeapon;
		}
		else
		{
			delete pWeapon;
		}
	}
	//Added end.

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, int iPhysique, int nGender, const EQUIPMENT_ESSENCE* pWeapon)
{
	pResult->uAttackType = pWeapon->action_type;

	const char* szWeaponFile;
	const char* szWeaponRightFile = "";
	const char* szWeaponBackFile = "";
	pResult->bHookChanged = false;//初始化操作？使得每次只有在特定的情况下在改变挂点标示

	if (iPhysique == PHYSIQUE_NORMAL || iPhysique == PHYSIQUE_MIDDLE)
	{
		szWeaponFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_male) : g_pGame->GetDataPath(pWeapon->file_model_female);
		if ( pWeapon->action_type == WEAPON_RULER ) //刃，焚香职业用，双武器, 同时背部背一武器 Added 2011-08-09.
		{
			szWeaponRightFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_male) : g_pGame->GetDataPath(pWeapon->file_model_female);	
			szWeaponBackFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_back_male) : g_pGame->GetDataPath(pWeapon->file_model_back_female);
			pResult->bHookChanged = true; //标示szWeaponRightFile这个所表示的挂点需要改变
		}
	}
	else if(iPhysique == PHYSIQUE_BIG)
	{
		szWeaponFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_big_male) : g_pGame->GetDataPath(pWeapon->file_model_big_female);
		if ( pWeapon->action_type == WEAPON_PEARL ) //珠11，太昊职业用，双武器，同时背部背一武器 Added 2011-08-09.
		{
			szWeaponRightFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_big_male_left) : g_pGame->GetDataPath(pWeapon->file_model_big_female_left);
			szWeaponBackFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_back_male) : g_pGame->GetDataPath(pWeapon->file_model_back_female);
			pResult->bHookChanged = true;	//标示szWeaponRightFile这个所表示的挂点需要改变
		}
	}
	else if(iPhysique == PHYSIQUE_SMALL)
	{
		szWeaponFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_small_male) : g_pGame->GetDataPath(pWeapon->file_model_small_female);
		if (pWeapon->action_type == WEAPON_SICKLE) // 镰刀，双手武器
			szWeaponRightFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_small_male) : g_pGame->GetDataPath(pWeapon->file_model_small_female);	
	}
	else
		return;

	ChangeWeapon(pResult, szWeaponFile, szWeaponRightFile, szWeaponBackFile); //Modified 2011-08-09.
}

void CECPlayer::ChangeTalisman(EquipsLoadResult* pResult, const char* szPath)
{
	pResult->bTalismanChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	CECModel* pTalisman = new CECModel;

	if (pTalisman->Load(szPath, true, A3DSkinModel::LSF_UNIQUESKIN))
	{
		_SetWeaponMaterial(pTalisman->GetA3DSkinModel());
		pResult->pTalisman = pTalisman;
	}
	else
		delete pTalisman;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
}

bool CECPlayer::ChangeWing(EquipsLoadResult* pResult, const char* szModeFile)
{
	pResult->bWingChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	CECModel* pWing = new CECModel;

	if (pWing->Load(szModeFile, true, A3DSkinModel::LSF_UNIQUESKIN))
	{
		_SetWeaponMaterial(pWing->GetA3DSkinModel());
		pResult->pWing = pWing;
	}
	else
		delete pWing;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

bool CECPlayer::ChangeTune(EquipsLoadResult* pResult, const char* szPath)
{
	pResult->bTuneChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	CECModel* pTune = new CECModel;
	
	if (pTune->Load(szPath, true, A3DSkinModel::LSF_UNIQUESKIN))
	{
		_SetWeaponMaterial(pTune->GetA3DSkinModel());
		pResult->pTune = pTune;
	}
	else
		delete pTune;
	
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CECPlayer::ChangeArmor(
	const EQUIPMENT_ESSENCE* pArmor,
	DWORD& dwShowMask,
	DWORD& dwSkinChangeMask,
	int iPhysique,
	int iProfession,
	int nGender,
	int iSkin,
	A3DSkin* pSkins[][3]
	)
{
	const char* szFile;
	if (iPhysique == PHYSIQUE_NORMAL || iPhysique == PHYSIQUE_MIDDLE)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pArmor->file_model_male) : g_pGame->GetDataPath(pArmor->file_model_female);
	}
	else if(iPhysique == PHYSIQUE_BIG)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pArmor->file_model_big_male) : g_pGame->GetDataPath(pArmor->file_model_big_female);
	}
	else if(iPhysique == PHYSIQUE_SMALL)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pArmor->file_model_small_male) : g_pGame->GetDataPath(pArmor->file_model_small_female);
	}
	else
		return;

	if (szFile[0] == 0)
		return;

	static const int _location_skin_map[] =
	{
		0,							// enumSkinShowNone = 0,
		SKIN_HELM_INDEX,			// enumSkinShowHead,
		SKIN_BODY_INDEX,			// enumSkinShowUpperBody,
		0,							// enumSkinShowLowerBody,
		SKIN_BODY_INDEX,			// enumSkinShowUpperAndLower,
		SKIN_FOOT_INDEX,			// enumSkinShowFoot,
		SKIN_GLASSES_INDEX,			// enumSkinShowGlasses,
		SKIN_NOSE_INDEX,			// enumSkinShowNose,
		SKIN_MUSTACHE_INDEX,		// enumSkinShowMustache,
		SKIN_CLOAK_INDEX,			// enumSkinShowCloak,
		0,							// enumSkinShowWing
	};

	int nLocation = pArmor->equip_location;
	if (nLocation < 0 || nLocation >= sizeof(_location_skin_map) / sizeof(_location_skin_map[0]))
		return;

	int nSkinIndex = _location_skin_map[nLocation];
	dwShowMask |= (1 << nLocation);
	dwSkinChangeMask |= (1 << nSkinIndex);
	
	ChangeArmor(
		szFile,	
		pSkins[nSkinIndex],
		nLocation,
		false,
		iPhysique,
		iProfession,
		nGender,
		iSkin
		);
}

bool CECPlayer::ChangeArmor(
	const char* strSkinFile,
	A3DSkin* aSkins[3],
	int nLocation,
	bool bDefault,
	int iPhysique,
	int iProfession,
	int nGender, 
	int iDefaultSkin)
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	switch (nLocation)
	{
	case enumSkinShowUpperBody:
	case enumSkinShowUpperAndLower:

		if (!LoadPlayerSkin(aSkins, SKIN_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFoot(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;
		
	case enumSkinShowHead:

		if (!LoadPlayerSkin(aSkins, SKIN_HELM_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowNone:

		break;

	default:
		a_LogOutput(1, "Equipment Invent: Wrong Location: %d", nLocation);
		break;
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CECPlayer::ChangeFashion(
	const EQUIPMENT_ESSENCE* pFashion,
	DWORD& dwShowMask,
	DWORD& dwSkinChangeMask,
	int iPhysique,
	int iProfession,
	int nGender,
	int iDefaultSkin,
	A3DSkin* pSkins[][3],
	A3DCOLOR color)
{
	const char* szFile;
	if (iPhysique == PHYSIQUE_NORMAL || iPhysique == PHYSIQUE_MIDDLE)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pFashion->file_model_male) : g_pGame->GetDataPath(pFashion->file_model_female);
	}
	else if(iPhysique == PHYSIQUE_BIG)
	{
		if ( (iProfession > PROF_96 && iProfession < PROF_103) && nGender )	//太昊女时装专用 Added 2011-08-09.
		{
			szFile = g_pGame->GetDataPath(pFashion->file_model_female_taihao);
		}
		else
		{
			szFile = (nGender == 0) ? g_pGame->GetDataPath(pFashion->file_model_big_male) : g_pGame->GetDataPath(pFashion->file_model_big_female);
		}
	}
	else if(iPhysique == PHYSIQUE_SMALL)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pFashion->file_model_small_male) : g_pGame->GetDataPath(pFashion->file_model_small_female);
	}
	else
		return;

	if (szFile[0] == 0)
		return;

	static const int _location_skin_map[] =
	{
		0,								// enumSkinShowNone = 0,
		SKIN_FASHION_HEADWEAR_INDEX,	// enumSkinShowHead,
		SKIN_FASHION_UPPER_BODY_INDEX,	// enumSkinShowUpperBody,
		SKIN_FASHION_LOWER_INDEX,		// enumSkinShowLowerBody,
		SKIN_FASHION_UPPER_BODY_INDEX,	// enumSkinShowUpperAndLower,
		SKIN_FASHION_FOOT_INDEX,		// enumSkinShowFoot,
		SKIN_GLASSES_INDEX,				// enumSkinShowGlasses,
		SKIN_NOSE_INDEX,				// enumSkinShowNose,
		SKIN_MUSTACHE_INDEX,			// enumSkinShowMustache,
		SKIN_CLOAK_INDEX,				// enumSkinShowCloak,
	};

	int nLocation = pFashion->equip_location;
	if (nLocation < 0 || nLocation >= sizeof(_location_skin_map) / sizeof(_location_skin_map[0]))
		return;

	int nSkinIndex = _location_skin_map[nLocation];
	dwShowMask |= (1 << nLocation);
	dwSkinChangeMask |= (1 << nSkinIndex);

	ChangeFashion(
		szFile,
		pSkins[nSkinIndex],
		nLocation,
		false,
		iPhysique,
		iProfession,
		nGender,
		color,
		iDefaultSkin
		);
}

bool CECPlayer::ChangeFashion(
	const char* strSkinFile,
	A3DSkin* aSkins[3],
	int nLocation,
	bool bDefault,
	int iPhysique,
	int iProfession,
	int nGender,
	A3DCOLOR color,
	int iDefaultSkin)
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	switch (nLocation)
	{
	case enumSkinShowUpperBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowLowerBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionLower(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionFoot(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowUpperAndLower:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowGlasses:

		if (!LoadPlayerSkin(aSkins, SKIN_GLASSES_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowNose:

		if (!LoadPlayerSkin(aSkins, SKIN_NOSE_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowMustache:

		if (!LoadPlayerSkin(aSkins, SKIN_MUSTACHE_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowCloak:

		if (!LoadPlayerSkin(aSkins, SKIN_CLOAK_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowHead:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_HEADWEAR_INDEX, strSkinFile))
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);

		break;

	case enumSkinShowNone:

		break;

	default:
		a_LogOutput(1, "Equipment Invent: Wrong Location: %d", nLocation);
		break;
	}

	// now get the shader and set color to it.
	for(int i=0; i<3; i++)
	{
		A3DSkin * pSkin = aSkins[i];
		if (!pSkin) continue;

		for(int idTex=0; idTex<pSkin->GetTextureNum(); idTex ++)
		{
			A3DTexture * pTex = pSkin->GetTexture(idTex);
			if( pTex && pTex->IsShaderTexture() )
			{
				A3DShader * pShader = (A3DShader *) pTex;
				pShader->GetGeneralProps().dwTFactor = color;
			}
		}
	}

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

//	Add money amount
int CECPlayer::AddMoneyAmount(int iAmount)
{
	m_iMoneyCnt += iAmount;
	if (m_iMoneyCnt < 0)
	{
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Player money is negative"));
		m_iMoneyCnt = 0;
	}

	return m_iMoneyCnt;
}

//	Render titles / names / talks above player's header
bool CECPlayer::RenderName(CECViewport* pViewport, DWORD dwFlags)
{
	if (!FillPateContent(pViewport))
		return true;

	return true;
}

//	Render booth name
int CECPlayer::RenderBoothName(CECViewport* pViewport, int y)
{
	return 0;
}

//	Render bar above player's header
bool CECPlayer::RenderBars(CECViewport* pViewport)
{
	if (!FillPateContent(pViewport))
		return true;

	int x, y, cx=100, cy=3;
	A3DFlatCollector* pFlat = g_pGame->GetA3DEngine()->GetA3DFlatCollector();
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	DWORD colEmpty = A3DCOLORRGB(234, 159, 90);

	x = m_PateContent.iBaseX - (cx >> 1);
	y = m_PateContent.iCurY;

	if (IsHostPlayer())
	{
/*		//	Draw exp bar useless by fuchonggang
		if (pConfigs->GetVideoSettings().bPlayerEXP)
		{
			if (m_BasicProps.exp < 0)
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
			else
			{
				int iLen = (int)ceil(cx * ((float)m_BasicProps.exp / GetLevelUpExp(m_BasicProps.iLevel + 1, m_nRebornCount)));
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
				pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(219, 0, 249));
			}

			y -= cy + 1;
		}
*/
		//	Draw DP bar
		if (pConfigs->GetVideoSettings().bPlayerDP)
		{
			if (m_ExtProps.max_dp && m_BasicProps.iCurDP)
			{
				int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurDP / m_ExtProps.max_dp));
				a_Clamp(iLen, 1, cx);
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
				pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(90, 5, 190));
			}
			
			y -= cy + 1;
		}

		//	Draw MP bar
		if (pConfigs->GetVideoSettings().bPlayerMP)
		{
			if (m_ExtProps.max_mp && m_BasicProps.iCurMP)
			{
				int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurMP / m_ExtProps.max_mp));
				a_Clamp(iLen, 1, cx);
				pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
				pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(75, 205, 255));
			}

			y -= cy + 1;
		}
		
	}

	//	Draw HP bar
	if (pConfigs->GetVideoSettings().bPlayerHP)
	{
		if (m_ExtProps.max_hp && m_BasicProps.iCurHP)
		{
			int iLen = (int)ceil(cx * ((float)m_BasicProps.iCurHP / m_ExtProps.max_hp));
			a_Clamp(iLen, 1, cx);
			pFlat->AddRect_2D(x, y, x+cx-1, y+cy-1, colEmpty);
			pFlat->AddRect_2D(x, y, x+iLen-1, y+cy-1, A3DCOLORRGB(255, 57, 62));
		}

		y -= cy + 1;
	}

	m_PateContent.iCurY	= y;

	return true;
}

//	Fill pate content
//	Return false if pate content is un-visible
bool CECPlayer::FillPateContent(CECViewport* pViewport)
{
	//	If m_PateContent.iVisible != 0, pate content has been filled
	//	for this frame
	if (m_PateContent.iVisible)
		return m_PateContent.iVisible == 2 ? true : false;

	A3DVIEWPORTPARAM* pViewPara = pViewport->GetA3DViewport()->GetParam();

	//	Calculate basic center position on screen
	A3DVECTOR3 vPos = GetPos();

	if(m_bMultiRideLeader)
 	{
		A3DSkinModel* pModel = GetA3DSkinModel();
		if(pModel)
			vPos = pModel->GetPos();
 	}
 	else if (m_bMultiRideMember)
 	{
		A3DSkinModel* pModel = GetA3DSkinModel();
		if (pModel)
			vPos = pModel->GetPos();
 	}

	if (m_pTransfiguredModel)
	{
		A3DAABB aabb;
		if (m_pTransfiguredModel->HasCHAABB())
		{
			aabb.Center = GetPos() + g_vAxisY * m_pTransfiguredModel->GetCHAABB().Center.y;
			aabb.Extents = m_pTransfiguredModel->GetCHAABB().Extents;
			aabb.CompleteMinsMaxs();
		}
		else
			aabb= m_pTransfiguredModel->GetModelAABB();

		vPos.y = aabb.Center.y + aabb.Extents.y * 1.15f;
//		vPos = vPos + g_vAxisY * m_fTransHeight; 
	}
	else if (m_pChangedModel)
		vPos = vPos + g_vAxisY * 2.3f;
	else if (GetGender() == 0)
	{
		if (GetPhysique() == PHYSIQUE_NORMAL)
			vPos = vPos + g_vAxisY * 2.1f;
		else if (GetPhysique() == PHYSIQUE_BIG)
		{
			if (m_dwStates & GP_STATE_SITDOWN)  // IsSitting() 打坐
				vPos = vPos + g_vAxisY * 3.5f;
			else
				vPos = vPos + g_vAxisY * 2.4f;
		}
		else if (GetPhysique() == PHYSIQUE_SMALL)
		{
			if (m_dwStates & GP_STATE_SITDOWN)
				vPos = vPos + g_vAxisY * 2.9f;
			else
				vPos = vPos + g_vAxisY * 1.9f;
		}
		else if (GetPhysique() == PHYSIQUE_MIDDLE)
		{
			vPos = vPos + g_vAxisY * 2.5f;
		}
	}
	else
	{
		if (GetPhysique() == PHYSIQUE_NORMAL)
			vPos = vPos + g_vAxisY * 1.95f;
		else if (GetPhysique() == PHYSIQUE_BIG)
		{
			if (m_dwStates & GP_STATE_SITDOWN)
				vPos = vPos + g_vAxisY * 3.1f;
			else
				vPos = vPos + g_vAxisY * 2.2f;
		}
		else if (GetPhysique() == PHYSIQUE_SMALL)
		{	
			if (m_dwStates & GP_STATE_SITDOWN)
				vPos = vPos + g_vAxisY * 2.5f;
			else
				vPos = vPos + g_vAxisY * 1.8f;
		}
		else if (GetPhysique() == PHYSIQUE_MIDDLE)
		{
			vPos = vPos + g_vAxisY * 2.4f;
		}
	}	

	if (m_bHangerOn) // 女被抱
	{
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if (pBuddy)
		{
			vPos.y += .15f;

			if (pBuddy->m_pPetModel)
				vPos.y += pBuddy->m_fPetHeight;
		}
	}
	else
	{
		if (m_pPetModel)
			vPos.y += m_fPetHeight;

		if (m_AttachMode != enumAttachNone)
			vPos.y += .65f;
	}

	if(m_bMultiRideMember)
	{
		CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);

		if (pLeader)
		{
			if (pLeader->m_pPetModel)
				vPos.y += pLeader->m_fPetHeight;
		}
	}
	
	if (IsFlying() && m_nFlyMode == FLYMODE_WING)
	{
		if (m_iPhysique == PHYSIQUE_SMALL)
			vPos =  vPos + g_vAxisY * 0.6f;
		else if (m_iPhysique == PHYSIQUE_NORMAL)
			vPos = vPos + g_vAxisY * 0.5f;
		else if (m_iPhysique == PHYSIQUE_BIG)
			vPos = vPos + g_vAxisY * 0.4f;
	}

	//太昊的归神与脱灵技能下，在角色体积变大/变小后需要改变头顶文字的位置Added 2011-06-23.
	if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
	{
//		float fDelta = ( m_fScaleShapeRatio > 1.0f ) ? 1.35f : -1.35f;
		vPos += A3DVECTOR3(0.0f, (m_fScaleShapeRatio - 1.0f)*(m_aabb.Extents.y+0.14f)*2.0f/*+fDelta*/, 0.0f);
	}
	//Added end.

	A3DMATRIX4 matVPS = pViewport->GetA3DViewport()->GetCamera()->GetVPTM() * pViewport->GetA3DViewport()->GetViewScale();
	A3DVECTOR4 vScrPos = TransformToScreen(vPos, matVPS);

	if (vScrPos.z < pViewPara->MinZ || vScrPos.z > pViewPara->MaxZ)
	{
		m_PateContent.iVisible = 1;		//	In-visible
		return false;
	}

	m_PateContent.iVisible	= 2;
	m_PateContent.iBaseX	= (int)vScrPos.x;
	m_PateContent.iBaseY	= (int)vScrPos.y;
	m_PateContent.iCurY		= m_PateContent.iBaseY;
	m_PateContent.z			= vScrPos.z;
	m_PateContent.rhw		= vScrPos.w;

	return true;
}

float CECPlayer::GetGroundSpeed()
{
//	return m_bWalkRun ? g_pGame->GetConfigs()->GetHostRunSpeed() : m_ExtProps.mv.walk_speed;
	float speed = m_bWalkRun ? m_ExtProps.run_speed : m_ExtProps.walk_speed;
	return max(speed, g_pGame->GetConfigs()->GetHostRunSpeed());
}

//	Set resources ready flag
void CECPlayer::SetResReadyFlag(DWORD dwFlag, bool bSet)
{
	if (bSet)
	{
		bool bTemp = IsAllResReady();
		m_dwResFlags |= dwFlag;
	//	if (!bTemp && IsAllResReady())
	//		OnAllResourceReady();
	}
	else
		m_dwResFlags &= ~dwFlag;
}

//	When all resources are ready, this function will be called
void CECPlayer::OnAllResourceReady()
{
	SetTransparent(-1.0f);
}

//	Caculate player's AABB base on profession and gender
void CECPlayer::CalcPlayerAABB()
{
	int iIndex = m_iGender;

	static const A3DVECTOR3 aExts[] =
	{
		A3DVECTOR3(0.3f, 0.9f, 0.3f),	// 男
		A3DVECTOR3(0.3f, 0.9f, 0.3f),	// 女
	};

	m_aabb.Extents = aExts[iIndex];

	static const MOVECONST aMoveConsts[16] = 
	{
		//	fStepHei	fMinAirHei	fMinWaterHei	fShoreDepth	fWaterSurf
		{	0.8f,		1.6f,		0.3f,			1.6f,		0.6f	},
		{	0.8f,		1.6f,		0.3f,			1.5f,		0.55f	},
	};

	m_MoveConst = aMoveConsts[iIndex];
}

//	Get player's model aabb
const A3DAABB & CECPlayer::GetModelAABB()
{
	return GetPlayerPickAABB();
}

//	Get player's skin model
A3DSkinModel * CECPlayer::GetA3DSkinModel()
{
	if( m_pPlayerModel )
		return m_pPlayerModel->GetA3DSkinModel();
	else
		return NULL;
}

//	Get player's pick AABB
const A3DAABB& CECPlayer::GetPlayerPickAABB()
{
	if (m_pPlayerModel && m_pPlayerModel->GetA3DSkinModel())
		return m_pPlayerModel->GetA3DSkinModel()->GetModelAABB();
	else
		return m_aabb;
}

//	Get property move or stand action
int CECPlayer::GetMoveStandAction(bool bMove, bool bFight/* false */)
{
	int iMoveEnv = m_iMoveEnv;
	if (m_AttachMode != enumAttachNone)
	{
		bFight = false;
		if( m_bHangerOn )
			iMoveEnv = MOVEENV_GROUND;
	}

	int iAction = ACT_STAND;

	if (bMove)
	{
		//	Play appropriate actions
		if (iMoveEnv == MOVEENV_GROUND)
		{
			if (m_bWalkRun)
				iAction = ACT_RUN;
			else
				iAction = ACT_WALK;
		}
		else if (iMoveEnv == MOVEENV_AIR)
		{
			iAction = fly_act[m_nFlyMode];
			if (iAction == ACT_FLY_WING ) // 翅膀增加了向上飞的动作
			{  
				if (IsHostPlayer())
				{				
					if (g_pGame->GetGameRun()->GetHostPlayer()->m_dwMoveRelDir == CECHostPlayer::MD_ABSUP)
						iAction = ACT_FLYUP_WING;	
				}
				else
				{
					CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
					if (pPlayer && fabs (pPlayer->m_vMoveDir.y - 1.0f ) < 0.01f )
						iAction = ACT_FLYUP_WING;
				}
			}
		}
		else if (iMoveEnv == MOVEENV_WATER)
			iAction = ACT_SWIM;
	}
	else
	{
		//	Play appropriate actions
		if (iMoveEnv == MOVEENV_GROUND)
		{
			if (bFight)
				iAction = ACT_FIGHTSTAND;
			else
				iAction = ACT_STAND;
		}
		else if (iMoveEnv == MOVEENV_AIR)
		{
			iAction = fly_hang_act[m_nFlyMode];
		}
		else if (iMoveEnv == MOVEENV_WATER)
			iAction = ACT_HANGINWATER;
	}

	return iAction;
}

//	Check whether current action is moving action
bool CECPlayer::CurActionIsMoving()
{
	if (!m_pPlayerModel)
		return false;

	int iAction = (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY);
	switch (iAction)
	{
	case ACT_RUN:
	case ACT_WALK:
	case ACT_FLY_SWORD:
	case ACT_FLY_WING:
	case ACT_FLY_FEIQI:
	case ACT_SWIM:

		return true;
	}

	return false;
}

//	Check water moving environment
bool CECPlayer::CheckWaterMoveEnv(const A3DVECTOR3& vPos, float fWaterHei, float fGndHei)
{
	return false;

	bool bRet = false;
/*	if (vPos.y < fWaterHei - m_MoveConst.fShoreDepth)
		bRet = true;
	else if (vPos.y < fWaterHei && fWaterHei - fGndHei > m_MoveConst.fShoreDepth)
		bRet = true;
*/
//	if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf - m_aabb.Extents.y)
//	if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf + 0.01f)

/*
	if (vPos.y < fWaterHei - m_MoveConst.fShoreDepth)
		bRet = true;
	else if (vPos.y < fWaterHei - m_MoveConst.fWaterSurf + 0.01f && fWaterHei - fGndHei > m_MoveConst.fShoreDepth)
		bRet = true;
*/

	//@note : modify the water test. By Kuiwu[12/10/2005]
	if (vPos.y + m_aabb.Extents.y  <  fWaterHei - m_MoveConst.fWaterSurf - 0.001f)
	{
		bRet = true;
	}
	//else if (vPos.y + m_aabb.Extents.y < fWaterHei- m_MoveConst.fWaterSurf + 1E-4f  &&  vPos.y - fGndHei >= 0.2f )
	else if (vPos.y + m_aabb.Extents.y < fWaterHei- m_MoveConst.fWaterSurf + 1E-4f  &&  vPos.y - fGndHei >= 0.01f )
	{
		bRet = true;
	}

	return bRet;

}

//	Set last said words
void CECPlayer::SetLastSaidWords(const ACHAR* szWords, int nEmotionSet, CECIvtrItem* pItem)
{

}

//	Set faction ID
void CECPlayer::SetFactionID(int id)
{
	m_idFaction = id;

}

//	Set family ID
void CECPlayer::SetFamilyID(int id)
{
	m_idFamily = id;

}

void CECPlayer::SetTitle(int iTitle)
{
	// first see if the title contain ext props, if so we need update player's ext props from the server
	if( g_pGame->GetTitlePropTable().find(m_iTitle) != g_pGame->GetTitlePropTable().end() ||
		g_pGame->GetTitlePropTable().find(iTitle) != g_pGame->GetTitlePropTable().end() )
	{
//		g_pGame->GetGameSession()->c2s_CmdGetExtProps();
	}

	m_iTitle = iTitle;
}

bool CECPlayer::FormatTitle(ACString & title)
{
	// See if the title cotains some variables need to be replaced
	int nStart =0;
	int nVar = title.Find(_AL("$"), nStart);
	while( nVar >= 0 )
	{
		ACString strLeft = title.Left(nVar);
		ACString strRight = title.Right(title.GetLength() - nVar - 2);
		title.CutLeft(nVar + 1);
		wchar_t control = title.GetLength() ? title[0] : ' ';
		switch(control)
		{
		case L'M':
			title = strLeft + m_strName + strRight;
			nStart = strLeft.GetLength() + m_strName.GetLength();
			break;

		case L'S':
			
			break;

		case L'T':
		
			break;

		default:
			title = strLeft + strRight;
			nStart = strLeft.GetLength();
			break;
		}
		nVar = title.Find(_AL("$"), nStart);
	}

	return true;
}

void CECPlayer::SetSpouse(int idSpouse)
{
	 m_idSpouse = idSpouse; 

	 // spouse may change the pate title, so trigger the flag.
	 m_bPateNeedUpdate = true;
}

//	Set faction role
void CECPlayer::SetFRoleID(int role)
{ 
	m_idFRole = role; 

}

//	On start binding buddy
void CECPlayer::OnStartBinding(int idMule, int idRider)
{
	m_bCandHangerOn = (idRider == GetCharacterID()) ? true : false;
	m_idCandBuddy = m_bCandHangerOn ? idMule : idRider;
}

//	Set booth name
void CECPlayer::SetBoothName(const ACHAR* szName)
{
	m_strBoothName = szName;

}

//	Is specified id a member of our team ?
bool CECPlayer::IsTeamMember(int idPlayer)
{
	return false;
}

//	Set new visible extend states
void CECPlayer::SetNewExtendStates(__int64 dwNewStates)
{
	static const char* szBasePath = "策划联入\\状态效果\\";

	m_dwExtStates = dwNewStates;
}

void CECPlayer::SetTeamRequireText(const TEAMREQ& Req)
{
	//	Build pate string
	CECStringTab* pFixStr = g_pGame->GetFixedMsgTab();
	m_strTeamReq = _AL("");

	if (Req.bShowLevel)
	{
		const ACHAR* szProf = pFixStr->GetWideString(FIXMSG_PROF_1+Req.iProfession);
		m_strTeamReq.Format(pFixStr->GetWideString(FIXMSG_LEVELANDPROF), Req.iLevel, szProf);
		m_strTeamReq += _AL(" ");
	}

	if (!Req.iType)
		m_strTeamReq += pFixStr->GetWideString(FIXMSG_SEARCHTEAM);
	else
		m_strTeamReq += pFixStr->GetWideString(FIXMSG_SEARCHTEAMMEM);
}

void CECPlayer::SetTeamRequire(const TEAMREQ& Req, bool bConfirm)
{
	m_TeamReq = Req;

	//	Build pate string
	SetTeamRequireText(Req);


}

//	Decompress advertisement data
void CECPlayer::DecompAdvData(int iData1, int iData2)
{
	TEAMREQ Req;
	Req.bShowReq	= (iData1 & (1 << 31)) ? true : false;
	Req.bShowLevel	= (iData1 & (1 << 30)) ? true : false;
	Req.iType		= (iData1 & 0x00ff0000) >> 16;
	Req.iLevel		= (iData1 & 0x0000ff00) >> 8;
	Req.iProfession	= iData1 & 0xff;

	SetTeamRequire(Req, false);
}

//	Message handlers
void CECPlayer::OnMsgPlayerExtState(const ECMSG& Msg)
{
	
}

void CECPlayer::OnMsgEnchantResult(const ECMSG& Msg)
{

}

void CECPlayer::OnMsgPlayerAdvData(const ECMSG& Msg)
{

}

void CECPlayer::OnMsgPlayerPVP(const ECMSG& Msg)
{

}

void CECPlayer::OnMsgSwitchFashionMode(const ECMSG& Msg)
{
	
}

void CECPlayer::OnMsgPlayerEffect(const ECMSG& Msg)
{

}

void CECPlayer::OnMsgChangeNameColor(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgPlayerMount(const ECMSG& Msg)
{

}

void CECPlayer::OnMsgPlayerLevel2(const ECMSG& Msg)
{

}

/**
*@param:ECMSG,封装之后的消息体。
*@return:void
*@usage:处理gs发来的缩放角色大小的消息处理函数；目前用于太昊归神/脱灵技能的客户端效果
**/
void CECPlayer::OnMsgPlayerScaleShape(const ECMSG& Msg)
{
	
}

void CECPlayer::OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg)
{

}

// player's render for shadow call back function
bool PlayerRenderForShadow(A3DViewport * pViewport, void * pArg)
{
	CECPlayer * pPlayer = (CECPlayer *) pArg;
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	g_pGame->GetA3DDevice()->SetAlphaRef(84);

	CECViewport viewport;
	viewport.InitFromA3D(pViewport);

	if (pPlayer->GetTransfiguredModel())
	{
		pPlayer->GetTransfiguredModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
	}
	else if (pPlayer->GetChangedModel())
	{
		pPlayer->GetChangedModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
	}
	else if (pPlayer->GetPetModel())
	{
		pPlayer->GetPetModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
	}
	else if(pPlayer->GetPlayerModel())
	{
		pPlayer->GetPlayerModel()->GetA3DSkinModel()->RenderAtOnce(pViewport, A3DSkinModel::RAO_NOMATERIAL, false);
	}

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	g_pGame->GetA3DDevice()->SetZTestEnable(true);
	g_pGame->GetA3DDevice()->SetZWriteEnable(true);
	return true;
}

void PlayerRenderPortrait(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist[2] = { 0.79f, 0.7f };
	static const float _height[2] = { 0.08f, 0.08f };

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	float		vDeg = (int)param2 * 0.01f;
	
	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);
	A3DSkinModel * pFaceModel = pPlayer->GetPortraitModel();
	pFaceModel->Update(g_pGame->GetTickTime());
	A3DBone * pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
	A3DMATRIX4 mat = pFaceModel->GetAbsoluteTM();
	mat.SetRow(3, pBoneUp->GetAbsoluteTM().GetRow(3));

	A3DMATRIX4 matHead = a3d_RotateY(vDeg) * mat;
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));

	int index = pPlayer->GetGender();
	pCamera->SetPos(vecPos + vecDir * _dist[index] + vecUp * _height[index]);
	pCamera->SetDirAndUp(-vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(0.8f, 0.8f, 0.8f, 1.0f);
	LightInfo.vLightDir		= Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
	LightInfo.colDirDiff	= A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.bPtLight		= false;

	pFaceModel->SetLightInfo(LightInfo);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	pFaceModel->RenderAtOnce(pPortraitViewport, 0);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	pCurViewport->Active();
}

void PlayerRenderDemonstration(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist[4][2] = { {-6.0f, -5.8f}, {-5.9f, -5.7f}, {-6.3f, -6.0f}, {-7.3f, -6.2f},  };
	static const float _height[4][2] = { {0.9f, 0.9f}, {0.9f, 0.8f}, {1.3f, 1.2f}, {1.1f, 0.95f},  };
	
	// 头部特写摄像机位置
	static const float _dist_head[4][2] = { { -1.5f, -1.4f }, { -1.5f, -1.5f }, { -1.5f, -1.5f }, { -1.5f, -1.5f },  };
	static const float _height_head[4][2] = { {1.7f, 1.6f}, {1.65f, 1.55f}, {2.1f, 2.0f}, {2.1f, 1.8f},  };

	if( param1 == 0 )
		return;

	CECPlayer * pPlayer = (CECPlayer *) param1;
	float vDeg = DEG2RAD(param2);

	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();

	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();

	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);

	int nGender = pPlayer->GetGender();
	int nPhysique	= pPlayer->GetPhysique();
	if (param3>100)
		param3 = 100;
	float _h = (_height[nPhysique][nGender]*(100-param3)+_height_head[nPhysique][nGender]*param3)/100;
	float _d = (_dist[nPhysique][nGender]*(100-param3)+_dist_head[nPhysique][nGender]*param3)/100;
	A3DMATRIX4 mat = a3d_RotateY(vDeg);
	A3DVECTOR3 vecPos = A3DVECTOR3(0, _h, _d) * mat;
	A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
	A3DVECTOR3 vecUp = A3DVECTOR3(0, 1.0f, 0);

	pCamera->SetPos(vecPos);
	pCamera->SetDirAndUp(vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	CECModel* pModel = pPlayer->GetPetModel() ? pPlayer->GetPetModel() : pPlayer->GetPlayerModel();
	pModel->Render(pPortraitViewport, true);
// 	CECViewport viewport;
// 	viewport.InitFromA3D(pPortraitViewport);
// 	pPlayer->RenderEquipBloom(&viewport);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport,true);

	pCurViewport->Active();
}

void PlayerRenderHeadPortrait(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist[2] = { 1.2f, 1.1f };
	static const float _height[2] = { 0.08f, 0.08f };
	
	if( param1 == 0 )
		return;
	
	CECPlayer * pPlayer = (CECPlayer *) param1;
	float		vDeg = DEG2RAD(param2);
	
	A3DViewport * pCurViewport = g_pGame->GetA3DEngine()->GetActiveViewport();
	
	A3DViewport * pPortraitViewport = g_pGame->GetPortraitRender()->GetPortraitViewport();
	A3DCameraBase * pCamera = pPortraitViewport->GetCamera();
	
	A3DVIEWPORTPARAM viewParam = *pPortraitViewport->GetParam();
	viewParam.X = rc.left;
	viewParam.Y = rc.top;
	viewParam.Width = rc.right - rc.left;
	viewParam.Height = rc.bottom - rc.top;
	pPortraitViewport->SetParam(&viewParam);
	((A3DCamera *) pCamera)->SetProjectionParam(-1.0f, -1.0f, -1.0f, ((float)viewParam.Width) / viewParam.Height);
	A3DSkinModel * pFaceModel = pPlayer->GetHeadPortraitModel();
	if(!pFaceModel)
		return;
	pFaceModel->Update(g_pGame->GetTickTime());
	A3DBone * pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
	A3DMATRIX4 mat = pFaceModel->GetAbsoluteTM();
	mat.SetRow(3, pBoneUp->GetAbsoluteTM().GetRow(3));
	
	A3DMATRIX4 matHead = a3d_RotateY(vDeg) * mat;
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));
	
	int index = pPlayer->GetGender();
	pCamera->SetPos(vecPos + vecDir * _dist[index] + vecUp * _height[index]);
	pCamera->SetDirAndUp(-vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();
	
	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(0.8f, 0.8f, 0.8f, 1.0f);
	LightInfo.vLightDir		= Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
	LightInfo.colDirDiff	= A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.bPtLight		= false;
	
	pFaceModel->SetLightInfo(LightInfo);
	
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	pFaceModel->RenderAtOnce(pPortraitViewport, 0);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	
	pCurViewport->Active();
}

CECModel* CECPlayer::GetWingModel()
{
	if (m_pPlayerModel)
		return m_pPlayerModel->GetChildModel(_wing);
	else
		return NULL;
}

//	Print bubble text
void CECPlayer::BubbleText(int iIndex, DWORD dwNum, DWORD dwWaitTime/* 0 */, int p1/* 0 */, bool bLastOne/* true */, bool bRandPos/* false */)
{
	if (!m_pBubbleTexts)
		return;

}

bool CECPlayer::IsClothesOn()
{
	if( m_bFashionMode )
	{
		if( !m_aCurSkins[SKIN_FASHION_UPPER_BODY_INDEX] ||
//			!m_aCurSkins[SKIN_FASHION_LOWER_INDEX] ||	//Modified 2012-02-23. 时装中，不再有“下衣”，而改为普通的装备：勋章，因此不需要换肤
			!m_aCurSkins[SKIN_FASHION_FOOT_INDEX] )
			return false;
	}
	else
	{
		if( !m_aCurSkins[SKIN_BODY_INDEX] ||
			!m_aCurSkins[SKIN_FOOT_INDEX] )
			return false;
	}
	return true;
}

bool CECPlayer::UpdateCurSkins()
{
	if (m_pBackupModel)
	{
		int	i;
		for(i=0; i<NUM_SKIN_INDEX; i++)
		{
			int lodLevel = 0;
			/*
			//	first of all update lod based skin
			if( m_fDistToCamera > 20.0f )
				lodLevel = 2;
			else if( m_fDistToCamera > 10.0f )
				lodLevel = 1;
				*/
			
			if( m_bFashionMode )
			{
				if (i == SKIN_BODY_INDEX
				 || i == SKIN_FOOT_INDEX
				 || i == SKIN_HELM_INDEX)
				{
					if( m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(i) )
					{
						m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, NULL, false);
						m_aCurSkins[i] = NULL;
					}
				}
				else
				{
					if (m_aCurSkins[i] != m_aSkins[i][lodLevel])
					{
						m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, m_aSkins[i][lodLevel], false);
						m_aCurSkins[i] = m_aSkins[i][lodLevel];
					}
				}
			}
			else
			{
				if (i == SKIN_FASHION_UPPER_BODY_INDEX
				 || i == SKIN_FASHION_LOWER_INDEX
				 || i == SKIN_FASHION_FOOT_INDEX
				 || i == SKIN_FASHION_HEADWEAR_INDEX)
				{
					if( m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(i) )
					{
						m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, NULL, false);
						m_aCurSkins[i] = NULL;
					}
				}
				else
				{
					if (m_aCurSkins[i] != m_aSkins[i][lodLevel])
					{
						m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, m_aSkins[i][lodLevel], false);
						m_aCurSkins[i] = m_aSkins[i][lodLevel];
					}
				}
			}
		}

		// make stone effects visible or not
		if( m_bFashionMode )
		{
			m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX, m_aCurSkins[SKIN_FASHION_HEADWEAR_INDEX] == NULL);
			if (m_iPhysique == PHYSIQUE_SMALL)
			{		
				m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_EAR_INDEX, false);
				m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_TAIL_INDEX, false);
			}

			ShowArmorStones(EQUIPIVTR_HEAD, m_stoneHead, false);
			ShowArmorStones(EQUIPIVTR_BODY, m_stoneBody, false);
			ShowArmorStones(EQUIPIVTR_FOOT, m_stoneFoot, false);
			ShowFullSuiteGFX(m_idFullSuite, false);
		}
		else
		{
			m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX, m_aCurSkins[SKIN_HELM_INDEX] == NULL);
			if (m_iPhysique == PHYSIQUE_SMALL)
			{
				SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
				if (pInfo && pInfo->mask_sect & (1<<SECT_11)) // 烈山
				{
					m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_EAR_INDEX, true);
					m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_TAIL_INDEX, true);
				}
				else
				{
					m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_EAR_INDEX, false);
					m_pBackupModel->GetA3DSkinModel()->ShowSkin(SKIN_TAIL_INDEX, false);
				}
			}
			ShowArmorStones(EQUIPIVTR_HEAD, m_stoneHead, true);
			ShowArmorStones(EQUIPIVTR_BODY, m_stoneBody, true);
			ShowArmorStones(EQUIPIVTR_FOOT, m_stoneFoot, true);
			ShowFullSuiteGFX(m_idFullSuite, true);
		}
	}

	if (m_pPortraitModel)
	{
		if (m_bFashionMode)
		{
			bool bShowHair = (m_pPortraitModel->GetA3DSkin(PORTRAIT_SKIN_HEADWEAR) == NULL);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HAIR, bShowHair);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HEADWEAR, !bShowHair);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HELM, false);
		}
		else
		{
			bool bShowHair = (m_pPortraitModel->GetA3DSkin(PORTRAIT_SKIN_HELM) == NULL);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HAIR, bShowHair);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HELM, !bShowHair);
			m_pPortraitModel->ShowSkin(PORTRAIT_SKIN_HEADWEAR, false);
		}
	}

	return true;
}

//	Apply effect on player
bool CECPlayer::ApplyEffect(int iEffect, bool bCheckArray)
{
	if (bCheckArray)
	{
		//	Check whether player has had this effect
		for (int i=0; i < m_aCurEffects.GetSize(); i++)
		{
			if (m_aCurEffects[i] == iEffect)
				return true;
		}

		m_aCurEffects.UniquelyAdd(iEffect);
	}

	return true;
}

//	Discard effect from player
void CECPlayer::DiscardEffect(int iEffect)
{

}

//	Check whether player has effect of specified type
bool CECPlayer::HasEffectType(int iEffType)
{
	switch (iEffType)
	{
	case 0:
	default:
		break;
	}

	return false;
}

// Play start use item action
bool CECPlayer::PlayStartUseItemAction(int tid)
{
	DATA_TYPE dt;
	const void * pDBEssence = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, dt);

	switch(dt)
	{
	case DT_MEDICINE_ESSENCE:
		break;

		/*
	case DT_TOWNSCROLL_ESSENCE:
	case DT_UNIONSCROLL_ESSENCE:
		PlayAction(ACT_USEITEM, 1.0f);
		PlayAction(ACT_USEITMELOOP, 1.0f, true, 0, true);
		break;
		*/

	case DT_REVIVESCROLL_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			// play an action use revive scroll to revive somebody else
			PlayAction(ACT_USEITEM, 1.0f);
			PlayAction(ACT_USEITMELOOP, 1.0f, true, 0, true);
		}
		break;

		/*
	case DT_WAR_TANKCALLIN_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			// play an action use revive scroll to revive somebody else
			PlayAction(ACT_USEITEM, 1.0f);
			PlayAction(ACT_USEITMELOOP, 1.0f, true, 0, true);
		}
		break;
		*/

	case DT_FIREWORKS_ESSENCE:
		if( IsDead() )
			break;
		else
		{
			PlayAction(ACT_EXP_ASSAULT, 1.0f);
		}
		break;
	}

	return true;
}

// Play use item effect
bool CECPlayer::PlayUseItemEffect(int tid, const void* pData, size_t sz)
{
	DATA_TYPE dt;
	const void * pDBEssence = g_pGame->GetElementDataMan()->get_data_ptr(tid, ID_SPACE_ESSENCE, dt);

	switch(dt)
	{
	case DT_MEDICINE_ESSENCE:
		{
			if( !GetPlayerModel() )
				return true;

			const MEDICINE_ESSENCE * pMedicine = (MEDICINE_ESSENCE *) pDBEssence;
			switch(pMedicine->type)
			{
			case 0: //瞬回HP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_HP_MEDICINE), NULL);
				break;
				
			case 1: //瞬回MP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MP_MEDICINE), NULL);
				break;
				
			case 2: //瞬回HP及MP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_HP_MEDICINE), NULL);
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MP_MEDICINE), NULL);
				break;

			case 3: //持续回HP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_HP_SLOW_MEDICINE), NULL);
				break;
				
			case 4: //持续回MP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MP_SLOW_MEDICINE), NULL);
				break;
				
			case 5: //持续回HP及MP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_HP_SLOW_MEDICINE), NULL);
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MP_SLOW_MEDICINE), NULL);
				break;
			case 6:	//瞬回DP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_DP_MEDICINE), NULL);
				break;
			case 7: //持续回DP
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_DP_SLOW_MEDICINE), NULL);
				break;

			}
		}

	case DT_TOWNSCROLL_ESSENCE:
		break;

		/*
	case DT_ARMORRUNE_ESSENCE:
		{
			if( !GetPlayerModel() )
				return true;

			const ARMORRUNE_ESSENCE * pArmorRune = (ARMORRUNE_ESSENCE *) pDBEssence;
			switch(pArmorRune->id_sub_type)
			{
			case 405: // 物防符
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_PHYSIC_ARMORRUNE), NULL);
				break;

			case 406: // 法防符
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MAGIC_ARMORRUNE), NULL);
				break;
			}
			break;
		}
		break;
		*/

	case DT_FIREWORKS_ESSENCE:
		{
		
		}
		break;


	case DT_TEXT_FIREWORKS_ESSENCE:
		{
		
		}
		break;
	}
	return true;
}

void CECPlayer::AddArmorStones(int nEquipIndex, BYTE status)
{
	if( status == 0 )
		return;

	const char * szHH = "";
	
	float vScale = 1.0f;
	if( m_iGender == 1 )
		vScale = 0.95f;

	const char * szGFX = "程序联入\\装备发光.gfx";
	switch(nEquipIndex)
	{
	case EQUIPIVTR_BODY:
		szHH = "HH_Spine";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 2.0f);
		}
		szHH = "HH_tui01";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 1.5f);
		}
		szHH = "HH_tui02";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 1.5f);
		}
		break;

	case EQUIPIVTR_HEAD:
		szHH = "HH_toufa";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
		}
		break;

	case EQUIPIVTR_FOOT:
		szHH = "HH_xiaotui01";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
		}
		szHH = "HH_xiaotui02";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
		}
		szHH = "HH_jiao01";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
		}
		szHH = "HH_jiao02";
		if( m_pBackupModel )
		{
			m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
		}
		break;
	}
}

void CECPlayer::RemoveArmorStones(int nEquipIndex, BYTE status)
{
	if( status == 0 )
		return;

	const char * szGFX = "程序联入\\装备发光.gfx";
	const char * szHH = "";

	switch(nEquipIndex)
	{
	case EQUIPIVTR_BODY:
		szHH = "HH_Spine";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		szHH = "HH_tui01";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		szHH = "HH_tui02";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		break;

	case EQUIPIVTR_HEAD:
		szHH = "HH_toufa";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		break;

	case EQUIPIVTR_FOOT:
		szHH = "HH_xiaotui01";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		szHH = "HH_xiaotui02";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		szHH = "HH_jiao01";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		szHH = "HH_jiao02";
		if( m_pBackupModel )
			m_pBackupModel->RemoveGfx(szGFX, szHH);
		break;
	}		
}

void CECPlayer::AddWeaponStones(BYTE status)
{
	return;
	if( status == 0 )
		return;

	const char * szHH = "";
	
	const char * szColors[] = {"无", "红", "白", "金", "蓝", "紫", "绿", "混"};

	int			idColor1 = status & 0x7;
	int			idColor2 = (status >> 3) & 0x7;
	
	if( idColor1 > idColor2 )
	{
		int temp = idColor1;
		idColor1 = idColor2;
		idColor2 = temp;
	}

	float vScale = 1.0f;
	if( m_iGender == 1 )
		vScale = 0.6f;
	else if( m_BasicProps.iProfession == 4 )
		vScale = 1.3f;

	char szGFX[MAX_PATH];

	sprintf(szGFX, "程序联入\\武器宝石镶嵌\\%s%s.gfx", szColors[idColor1], szColors[idColor2]);
	if( m_pBackupModel )
	{
		CECModel* pWeapon = m_pBackupModel->GetChildModel(_weapon_hanger);
		if (pWeapon)
			m_pBackupModel->PlayGfx(szGFX, "HH_weapon", vScale);
		CECModel* pRightWeapon = m_pBackupModel->GetChildModel(_right_weapon_hanger);
		if (pRightWeapon)
			m_pBackupModel->PlayGfx(szGFX, "HH_righthandweapon", vScale);
	}
}
void CECPlayer::RemoveWeaponStones(BYTE status)
{
	return;
	if( status == 0 )
		return;

	const char * szHH = "";
	
	const char * szColors[] = {"无", "红", "白", "金", "蓝", "紫", "绿", "混"};

	int			idColor1 = status & 0x7;
	int			idColor2 = (status >> 3) & 0x7;
	
	if( idColor1 > idColor2 )
	{
		int temp = idColor1;
		idColor1 = idColor2;
		idColor2 = temp;
	}

	char szGFX[MAX_PATH];

	sprintf(szGFX, "程序联入\\武器宝石镶嵌\\%s%s.gfx", szColors[idColor1], szColors[idColor2]);
	if( m_pBackupModel )
	{
		// force remove both left and right hand effects.
		m_pBackupModel->RemoveGfx(szGFX, "HH_lefthandweapon");
		m_pBackupModel->RemoveGfx(szGFX, "HH_righthandweapon");
	}	
}

void CECPlayer::ShowArmorStones(int nEquipIndex, BYTE status, bool bVisible)
{
	if( status <= 0 )
		return;

	const char * szHH = "";
	const char * szGFX = "程序联入\\装备发光.gfx";

	A3DCOLORVALUE color = l_ecolors[min(status, 20)];
	if( color == A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f) )
		bVisible = false;

	color.r = (float)pow(color.r, 0.8f);
	color.g = (float)pow(color.g, 0.8f);
	color.b = (float)pow(color.b, 0.8f);

	switch(nEquipIndex)
	{
	case EQUIPIVTR_BODY:
		color.a = 1.0f;
		szHH = "HH_Spine";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		color.a = 0.6f;
		szHH = "HH_tui01";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		szHH = "HH_tui02";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		break;

	case EQUIPIVTR_HEAD:
		color.a = 0.8f;
		szHH = "HH_toufa";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		break;

	case EQUIPIVTR_FOOT:
		color.a = 0.8f;
		szHH = "HH_xiaotui01";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		szHH = "HH_xiaotui02";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		szHH = "HH_jiao01";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		szHH = "HH_jiao02";
		if( m_pBackupModel )
		{
			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
			if( pGfx )
			{
				pGfx->SetVisible(bVisible);
				pGfx->SetOuterColor(color);
			}
		}
		break;
	}
}

void CECPlayer::RemoveFullSuiteGFX(int idFullSuite)
{
	if( idFullSuite == 0 )
		return;

	DATA_TYPE dt;
	SUITE_ESSENCE * pSuite = (SUITE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(idFullSuite, ID_SPACE_ESSENCE, dt);
	if( dt != DT_SUITE_ESSENCE )
		return;

	if( pSuite->file_gfx[0] )
	{
		if( m_pBackupModel )
		{
			const char * szHH[] = 
			{
				"HH_Head",
				"HH_toufa",
				"HH_jian01",
				"HH_jian02",
				"HH_bind",
				"HH_spine",
				"HH_shou01",
				"HH_shou02",
				"HH_righthandweapon",
				"HH_lefthandweapon",
				"HH_base",
				"HH_tui01",
				"HH_tui02",
				"HH_xiaotui01",
				"HH_xiaotui02",
				"HH_jiao01",
				"HH_jiao02",
			};

			m_pBackupModel->RemoveGfx(pSuite->file_gfx + 4, szHH[pSuite->hh_type]);
		}	
	}
}

void CECPlayer::AddFullSuiteGFX(int idFullSuite)
{
	float vScale = 1.0f;
	if( m_iGender == 1 )
		vScale = 0.6f;
	
	if( idFullSuite == 0 )
		return;

	DATA_TYPE dt;
	SUITE_ESSENCE * pSuite = (SUITE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(idFullSuite, ID_SPACE_ESSENCE, dt);
	if( dt != DT_SUITE_ESSENCE )
		return;

	if( pSuite->file_gfx[0] )
	{
		if( m_pBackupModel )
		{
			const char * szHH[] = 
			{
				"HH_Head",
				"HH_toufa",
				"HH_jian01",
				"HH_jian02",
				"HH_bind",
				"HH_spine",
				"HH_shou01",
				"HH_shou02",
				"HH_righthandweapon",
				"HH_lefthandweapon",
				"HH_base",
				"HH_tui01",
				"HH_tui02",
				"HH_xiaotui01",
				"HH_xiaotui02",
				"HH_jiao01",
				"HH_jiao02",
			};

			m_pBackupModel->PlayGfx(pSuite->file_gfx + 4, szHH[pSuite->hh_type], vScale);
		}	
	}
}

void CECPlayer::ShowFullSuiteGFX(int idFullSuite, bool bVisible)
{
	if( idFullSuite == 0 )
		return;

	DATA_TYPE dt;
	SUITE_ESSENCE * pSuite = (SUITE_ESSENCE *) g_pGame->GetElementDataMan()->get_data_ptr(idFullSuite, ID_SPACE_ESSENCE, dt);
	if( dt != DT_SUITE_ESSENCE )
		return;

	if( pSuite->file_gfx[0] )
	{
		if( m_pBackupModel )
		{
			const char * szHH[] = 
			{
				"HH_Head",
				"HH_toufa",
				"HH_jian01",
				"HH_jian02",
				"HH_bind",
				"HH_spine",
				"HH_shou01",
				"HH_shou02",
				"HH_righthandweapon",
				"HH_lefthandweapon",
				"HH_base",
				"HH_tui01",
				"HH_tui02",
				"HH_xiaotui01",
				"HH_xiaotui02",
				"HH_jiao01",
				"HH_jiao02",
			};

			A3DGFXEx * pGfx = m_pBackupModel->GetGfx(pSuite->file_gfx + 4, szHH[pSuite->hh_type]);
			if( pGfx )
				pGfx->SetVisible(bVisible);
		}
	}
}

// 在新职业天华的脚下加上特效
void CECPlayer::AddFootGFX(int iGender)
{
	const char* szFile;
	if (iGender == GENDER_MALE)
		szFile = res_GFXFile(RES_GFX_FOOT_MALE);
	else
		szFile = res_GFXFile(RES_GFX_FOOT_FEMALE);
	if (m_pBackupModel)
	{
		m_pBackupModel->PlayGfx(szFile, "HH_jiao01");
		m_pBackupModel->PlayGfx(szFile, "HH_jiao02");
	}
}

// 在新职业怀光的胸口加上特效
void CECPlayer::AddSpineGFX()
{
	const char* szFile = res_GFXFile(RES_GFX_SPINE_EFFECT);
	if (m_pBackupModel)
	{
		m_pBackupModel->PlayGfx(szFile, "HH_Spine");
	}
}

// 封神特效
void CECPlayer::AddDeityGFX()
{
	const char* szFile = NULL;
	if (m_iCultivation == 1)
		szFile = res_GFXFile(RES_GFX_DEITY_GOD);
	else if (m_iCultivation == 2)
		szFile = res_GFXFile(RES_GFX_DEITY_DEVIL);
	else if (m_iCultivation == 4)
		szFile = res_GFXFile(RES_GFX_DEITY_BUDDHA);
	if (m_pBackupModel)
	{
		m_pBackupModel->PlayGfx(szFile, "HH_Spine");
	}
}

//	Set player's transparence
void CECPlayer::SetTransparent(float fTrans)
{
	if (m_pChangedModel)
		m_pChangedModel->SetTransparent(fTrans);

	if (m_pTransfiguredModel)
		m_pTransfiguredModel->SetTransparent(fTrans);

	if (m_pBackupModel)
	{
		m_pBackupModel->SetTransparent(fTrans);

		if( fTrans >= 0.0f )
		{
		}
		else
		{
			// now restore hair's fTrans to 0.0f
			A3DSkin * pSkin;
			int i;
// 			pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HAIR_INDEX);
// 			if( pSkin )
// 			{
// 				for(i=0; i<pSkin->GetSkinMeshNum(); i++)
// 				{
// 					A3DSkinMeshRef& Ref = pSkin->GetSkinMeshRef(i);
// 					if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
// 					{
// 						Ref.fTrans = 0.0f;
// 					}
// 				}
// 			}
// 			pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_FASHION_HEADWEAR_INDEX);
// 			if( pSkin )
// 			{
// 				for(i=0; i<pSkin->GetSkinMeshNum(); i++)
// 				{
// 					A3DSkinMeshRef& Ref = pSkin->GetSkinMeshRef(i);
// 					if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
// 					{
// 						Ref.fTrans = 0.0f;
// 					}
// 				}
// 			}
// 			pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HELM_INDEX);
// 			if( pSkin )
// 			{
// 				for(i=0; i<pSkin->GetSkinMeshNum(); i++)
// 				{
// 					A3DSkinMeshRef& Ref = pSkin->GetSkinMeshRef(i);
// 					if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
// 					{
// 						Ref.fTrans = 0.0f;
// 					}
// 				}
// 			}
			pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_EAR_INDEX);
			if( pSkin )
			{
				for(i=0; i<pSkin->GetSkinMeshNum(); i++)
				{
					A3DSkinMeshRef& Ref = pSkin->GetSkinMeshRef(i);
					if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
					{
						Ref.fTrans = 0.0f;
					}
				}
			}
			pSkin = m_pBackupModel->GetA3DSkinModel()->GetA3DSkin(SKIN_TAIL_INDEX);
			if( pSkin )
			{
				for(i=0; i<pSkin->GetSkinMeshNum(); i++)
				{
					A3DSkinMeshRef& Ref = pSkin->GetSkinMeshRef(i);
					if (Ref.pMesh->GetTextureIndex() == 0) // if it is use the hair texture, just set to use alpha
					{
						Ref.fTrans = 0.0f;
					}
				}
			}
		}
	}
}

DWORD CECPlayer::DecideNameColor(int iPKLev)
{
	DWORD dwNameCol;

	switch (iPKLev)
	{
	case 6:
		dwNameCol = NAMECOL_RED6;
		break;
	case 5:
		dwNameCol = NAMECOL_RED5;
		break;
	case 4:
		dwNameCol = NAMECOL_RED4;
		break;
	case 3:
		dwNameCol = NAMECOL_RED3;
		break;
	case 2:
		dwNameCol = NAMECOL_RED2;
		break;
	case 1:
		dwNameCol = NAMECOL_RED1;
		break;
	case 0:
		dwNameCol = NAMECOL_WHITE;
		break;
	case -1:
		dwNameCol = NAMECOL_BLUE1;
		break;
	case -2:
		dwNameCol = NAMECOL_BLUE2;
		break;
	case -3:
		dwNameCol = NAMECOL_BLUE3;
		break;
	case -4:
		dwNameCol = NAMECOL_BLUE4;
		break;
	case -5:
		dwNameCol = NAMECOL_BLUE5;
		break;
	case -6:
		dwNameCol = NAMECOL_BLUE6;
		break;
	default:
		dwNameCol = NAMECOL_WHITE;
		break;
	}

	return dwNameCol;
}

//	Get player name color
DWORD CECPlayer::GetNameColor()
{
	DWORD dwNameCol;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (IsInBattle())	//	Player is in battle
	{
		if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
			dwNameCol = NAMECOL_BC_RED;
		else	//	(m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER)
			dwNameCol = NAMECOL_BC_BLUE;

		return dwNameCol;
	}
	else if (pHost->GetPVPInfo().ucFreePVPType == 2)
	{
		if (GetCultivation() != pHost->GetCultivation())
			return NAMECOL_BC_RED;
	}
	else if (m_idOrgServer)
	{
		if(m_idOrgServer == pHost->GetSrcServerID())
			return NAMECOL_WHITE;
		else 
			return NAMECOL_BC_RED;
	}

	
	if (m_BasicProps.iPKLevel <= 0 && IsInvader())
		return NAMECOL_PINK;

	return DecideNameColor(m_BasicProps.iPKLevel);
}

//	Check whether player in a same battle camp
bool CECPlayer::InSameBattleCamp(CECPlayer* pPlayer)
{
	if (!pPlayer || m_iBattleCamp == GP_BATTLE_CAMP_NONE ||
		m_iBattleCamp != pPlayer->GetBattleCamp())
		return false;

	return true;
}

//	Check whether specified npc in a same battle camp
bool CECPlayer::InSameBattleCamp(CECNPC* pNPC)
{
	if (!pNPC || m_iBattleCamp == GP_BATTLE_CAMP_NONE ||
		(m_iBattleCamp == GP_BATTLE_CAMP_INVADER && !pNPC->IsInBattleInvaderCamp()) ||
		(m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER && !pNPC->IsInBattleDefenderCamp()))
		return false;

	return true;
}

//	Change player's tank leader state
void CECPlayer::ChangeTankLeader(int idTank, int tidTank, bool bBecomeLeader)
{
	CECModel* pModel = GetPlayerModel();
	if (!pModel || !IsInBattle())
		return;

	static const char* szHeadHook = "HH_Head";

	//	Get effect file name base on player's battle camp
	const char* szGFXFile;
	if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
		szGFXFile = res_GFXFile(RES_GFX_TANKLEADER_RED);
	else
		szGFXFile = res_GFXFile(RES_GFX_TANKLEADER_BLUE);

	//	When player leave battle, idTank == 0
	if (!idTank)
	{
		//	Remove effect if there is one
		pModel->RemoveGfx(szGFXFile, szHeadHook);
		m_aBattleTanks.RemoveAll(false);
		return;
	}
	TANKINFO tank;
	tank.nid = idTank;
	tank.tid = tidTank;

	if (bBecomeLeader)
	{
		//	No matter how many tanks are controlled by player, ensure only
		//	one effect is played
		if (!m_aBattleTanks.GetSize())
			pModel->PlayGfx(szGFXFile, szHeadHook);

		int iIndex = -1;
		for (int i = 0; i< m_aBattleTanks.GetSize(); ++i)
		{
			const TANKINFO& info = m_aBattleTanks[i];
			if (info.nid == idTank)
			{
				iIndex = i;
				break;
			}	
		}
		if (iIndex < 0)
		{
			m_aBattleTanks.Add(tank);

			if (IsHostPlayer())
			{
				CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
				CECNPC* pNPC = pNPCMan->GetNPC(idTank);
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_OBTAINTANK, pNPC->GetName());
			}
		}
	}
	else
	{
		if (m_aBattleTanks.GetSize())
		{
			//	Check whether tank has been controlled by us

			int iIndex = -1;
			for (int i = 0; i< m_aBattleTanks.GetSize(); ++i)
			{
				const TANKINFO& info = m_aBattleTanks[i];
				if (info.nid == idTank)
				{
					iIndex = i;
					break;
				}	
			}
			if (iIndex >= 0)
			{
				m_aBattleTanks.RemoveAtQuickly(iIndex);

				if (IsHostPlayer())
				{
					CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
					CECNPC* pNPC = pNPCMan->GetNPC(idTank);
					if (pNPC)
						g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK, pNPC->GetName());
					else
					{
						DATA_TYPE dateType;
						const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(tidTank, ID_SPACE_ESSENCE, dateType);
						if(pData)
						{
							if (dateType == DT_MONSTER_ESSENCE)
							{
								const MONSTER_ESSENCE* pEss = (const MONSTER_ESSENCE*)pData;
								g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK, pEss->name);
							}
							else if (dateType == DT_NPC_ESSENCE)
							{
								const NPC_ESSENCE* pEss = (const NPC_ESSENCE*)pData;				
								g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK, pEss->name);
							}
						}
					}
				}
			}

			//	Remove effect only when no tank is controlled by player
			if (!m_aBattleTanks.GetSize())
				pModel->RemoveGfx(szGFXFile, szHeadHook);
		}
	}
}

//	Update player's tank leader state
void CECPlayer::UpdateTankLeader()
{
	if (!IsInBattle() || !m_aBattleTanks.GetSize())
		return;
	
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();

	for (int i=0; i < m_aBattleTanks.GetSize(); i++)
	{
		const TANKINFO& info = m_aBattleTanks[i];
		if (!pNPCMan->GetNPC(info.nid))
		{
			//	If tank has left player's eye shot, remove it
			m_aBattleTanks.RemoveAtQuickly(i);
			i--;

			if (IsHostPlayer())
			{
				DATA_TYPE dateType;
				const void* pData = g_pGame->GetElementDataMan()->get_data_ptr(info.tid, ID_SPACE_ESSENCE, dateType);
				if(pData)
				{
					if (dateType == DT_MONSTER_ESSENCE)
					{
						const MONSTER_ESSENCE* pEss = (const MONSTER_ESSENCE*)pData;
						g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK, pEss->name);
					}
					else if (dateType == DT_NPC_ESSENCE)
					{
						const NPC_ESSENCE* pEss = (const NPC_ESSENCE*)pData;				
						g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSETANK, pEss->name);
					}
				}
			}
		}
	}

	if (!m_aBattleTanks.GetSize())
	{
		//	Remove effect if no tank is controlled by player
		ChangeTankLeader(0, 0, false);
	}
}

//	Is host player ?
bool CECPlayer::IsHostPlayer()
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	if (!pGameRun->GetHostPlayer())
		return false;

	return m_PlayerInfo.cid == pGameRun->GetHostPlayer()->GetCharacterID();
}

//	Render equipments bloom
bool CECPlayer::RenderEquipBloom(CECViewport * pViewport)
{
	if( GetBoothState() != 0 || IsShapeChanged() || (IsTransfigured() && m_pTransfiguredModel)) //变身后，如果模型没有换，则还是需要渲染武器装备Modified 2011-09-01.
		return true;

	if (m_pBackupModel && m_stoneWeapon != 0xff && m_stoneWeapon > 0)
	{
		A3DCOLORVALUE c1 = l_ecolors[min(m_stoneWeapon, 20)];
		
// 		if( 0/*GetAsyncKeyState(VK_CONTROL) & 0x8000*/ )
// 		{
// 			bool bUpdate = false;
// 			static float r = 0.1f;
// 			static float g = 0.0f;
// 			static float b = 0.0f;
// 			if( GetAsyncKeyState('1') & 0x8000 )
// 			{
// 				r -= 0.01f;
// 				bUpdate = true;
// 			}
// 			else if( GetAsyncKeyState('2') & 0x8000 )
// 			{
// 				r += 0.01f;
// 				bUpdate = true;
// 			}
// 			if( GetAsyncKeyState('3') & 0x8000 )
// 			{
// 				g -= 0.01f;
// 				bUpdate = true;
// 			}
// 			else if( GetAsyncKeyState('4') & 0x8000 )
// 			{
// 				g += 0.01f;
// 				bUpdate = true;
// 			}
// 			if( GetAsyncKeyState('5') & 0x8000 )
// 			{
// 				b -= 0.01f;
// 				bUpdate = true;
// 			}
// 			else if( GetAsyncKeyState('6') & 0x8000 )
// 			{
// 				b += 0.01f;
// 				bUpdate = true;
// 			}
// 			a_Clamp(r, 0.0f, 1.0f);
// 			a_Clamp(g, 0.0f, 1.0f);
// 			a_Clamp(b, 0.0f, 1.0f);
// 
// 			if( bUpdate )
// 			{
// 				ACHAR szMsg[100];
// 				a_sprintf(szMsg, _AL("%.2f, %.2f, %.2f"), r, g, b);
// 				g_pGame->GetRTDebug()->OutputDebugInfo(0xffffff00, szMsg);
// 			}
// 			mat.Emissive = A3DCOLORVALUE(r, g, b, 1.0f);
// 		} 

		CECModel* pWeapon = m_pBackupModel->GetChildModel(_weapon_hanger);
		if (pWeapon)
		{	
			A3DSkinModel * pSkinModel = pWeapon->GetA3DSkinModel();
			A3DMATERIALPARAM matOld = pSkinModel->GetUniformMaterial().GetMaterialParam();
			A3DMATERIALPARAM mat;

			mat.Emissive = c1;
			mat.Ambient = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Diffuse = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Specular = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Power = 0.0f;
			if( pSkinModel->GetTransparent() > 0.0f )
			{
				mat.Emissive = mat.Emissive * (1.0f - pSkinModel->GetTransparent());
				mat.Emissive.a = 1.0f;
			}
			// see if need not to render
			if( mat.Emissive == A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f) )
				return true;
			pSkinModel->GetUniformMaterial().SetMaterialParam(mat);
			int oldMethod = pSkinModel->GetMaterialMethod();
			pSkinModel->SetMaterialMethod(A3DSkinModel::MTL_UNIFORM);  
			
			pSkinModel->RenderAtOnce(pViewport->GetA3DViewport(), A3DSkinModel::RAO_NOTEXTURE);

			pSkinModel->SetMaterialMethod(oldMethod);
			pSkinModel->GetUniformMaterial().SetMaterialParam(matOld);
		}

		pWeapon = m_pBackupModel->GetChildModel(_right_weapon_hanger);
		if (pWeapon)
		{
			A3DSkinModel * pSkinModel = pWeapon->GetA3DSkinModel();
			A3DMATERIALPARAM matOld = pSkinModel->GetUniformMaterial().GetMaterialParam();
			A3DMATERIALPARAM mat;

			mat.Emissive = c1;

			mat.Ambient = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Diffuse = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Specular = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Power = 0.0f;
			if( pSkinModel->GetTransparent() > 0.0f )
			{
				mat.Emissive = mat.Emissive * (1.0f - pSkinModel->GetTransparent());
				mat.Emissive.a = 1.0f;
			}
			// see if need not to render
			if( mat.Emissive == A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f) )
				return true;
			pSkinModel->GetUniformMaterial().SetMaterialParam(mat);
			int oldMethod = pSkinModel->GetMaterialMethod();
			pSkinModel->SetMaterialMethod(A3DSkinModel::MTL_UNIFORM);  
			
			pSkinModel->RenderAtOnce(pViewport->GetA3DViewport(), A3DSkinModel::RAO_NOTEXTURE);

			pSkinModel->SetMaterialMethod(oldMethod);
			pSkinModel->GetUniformMaterial().SetMaterialParam(matOld);
		}

		//Added 2011-08-31.
		pWeapon = m_pBackupModel->GetChildModel(_back_weapon_hanger);
		if (pWeapon)
		{
			A3DSkinModel * pSkinModel = pWeapon->GetA3DSkinModel();
			A3DMATERIALPARAM matOld = pSkinModel->GetUniformMaterial().GetMaterialParam();
			A3DMATERIALPARAM mat;

			mat.Emissive = c1;

			mat.Ambient = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Diffuse = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Specular = A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f);
			mat.Power = 0.0f;
			if( pSkinModel->GetTransparent() > 0.0f )
			{
				mat.Emissive = mat.Emissive * (1.0f - pSkinModel->GetTransparent());
				mat.Emissive.a = 1.0f;
			}
			// see if need not to render
			if( mat.Emissive == A3DCOLORVALUE(0.0f, 0.0f, 0.0f, 1.0f) )
				return true;
			pSkinModel->GetUniformMaterial().SetMaterialParam(mat);
			int oldMethod = pSkinModel->GetMaterialMethod();
			pSkinModel->SetMaterialMethod(A3DSkinModel::MTL_UNIFORM);  
			
			pSkinModel->RenderAtOnce(pViewport->GetA3DViewport(), A3DSkinModel::RAO_NOTEXTURE);

			pSkinModel->SetMaterialMethod(oldMethod);
			pSkinModel->GetUniformMaterial().SetMaterialParam(matOld);
		}
	}

	return true;
}

void CECPlayer::HideHead(bool bHide)
{
	if(bHide)
	{
		m_dwHideEquipMask |= (1<<EQUIPIVTR_HEAD);
		m_dwHideEquipMask |= (1<<EQUIPIVTR_FASHION_HEAD);
	}
	else
	{
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_HEAD);
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_FASHION_HEAD);
	}
}

void CECPlayer::HideBody(bool bHide)
{
	if(bHide)
	{
		m_dwHideEquipMask |= (1<<EQUIPIVTR_BODY);
		m_dwHideEquipMask |= (1<<EQUIPIVTR_FASHION_BODY);		
		m_dwHideEquipMask |= (1<<EQUIPIVTR_FASHION_LEG);
	}
	else
	{
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_BODY);
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_FASHION_BODY);
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_FASHION_LEG);
	}
}

void CECPlayer::HideFoot(bool bHide)
{
	if(bHide)
	{
		m_dwHideEquipMask |= (1<<EQUIPIVTR_FOOT);
		m_dwHideEquipMask |= (1<<EQUIPIVTR_FASHION_FOOT);		
	}
	else
	{
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_FOOT);
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_FASHION_FOOT);		
	}
}

void CECPlayer::RemoveMultiRideMember(int idTarget)
{
	for(int i =1; i< MULTI_RIDE_CAPACITY; ++i)
	{
		if( m_iMultiBuddyId[i] == idTarget)
		{
			DetachMultiBuddy(i);
			m_iMultiBuddyId[i] = 0;
			break;
		}
	}	
}

void CECPlayer::SetMilitaryRank(int rank)
{
	m_iMilitaryRank = rank;

}

const ACHAR* CECPlayer::GetMilitaryRankName(int rank)
{
	WStrTable::pair_type Pair = m_MilitaryRanks.get(rank);
	if (Pair.second)
		return **Pair.first;
	else
		return NULL;
}

void CECPlayer::ChangeFace(int faceid, int hairid, int skinid)
{
	if( !m_pPlayerModel )
		return ;
	if( !m_pPortraitModel)
		return ;

	char szPath[MAX_PATH];

	if(faceid != m_iFaceid)
	{
		m_iFaceid = faceid;
		CheckIndexBound(m_iFaceid);
		sprintf(szPath, _head_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iFaceid, GetProfession()));
		A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);

		A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HEAD_INDEX);
		g_pGame->ReleaseA3DSkin(pOldSkin);
		m_pPlayerModel->ReplaceSkin(SKIN_HEAD_INDEX, pHeadSkin, false);

		pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);
		m_pPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HEAD, pHeadSkin, false);
	}

	if(hairid != m_iHairid)
	{	
		m_iHairid = hairid;
		CheckIndexBound(m_iHairid);

		sprintf(szPath, _hair_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iHairid, GetProfession()));
		A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(szPath, true);

		A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HAIR_INDEX);
		g_pGame->ReleaseA3DSkin(pOldSkin);
		m_pPlayerModel->ReplaceSkin(SKIN_HAIR_INDEX, pHairSkin, false);

		pHairSkin = g_pGame->LoadA3DSkin(szPath, true);
		m_pPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HAIR, pHairSkin, false);
	}

	if(skinid != m_iDefaultSkin)
	{
		m_iDefaultSkin = skinid;
		DWORD dwMask = 0; 
		dwMask |= (1 << EQUIPIVTR_HEAD);
		dwMask |= (1 << EQUIPIVTR_BODY);
		dwMask |= (1 << EQUIPIVTR_FOOT);
		dwMask |= (1 << EQUIPIVTR_FASHION_HEAD);
		dwMask |= (1 << EQUIPIVTR_FASHION_BODY);
		dwMask |= (1 << EQUIPIVTR_FASHION_LEG);
		dwMask |= (1 << EQUIPIVTR_FASHION_FOOT);
		
		QueueLoadEquips(m_aEquips, dwMask, true);
	}

	UpdateCurSkins();
}

// 只适用于烈山一族
void CECPlayer::ChangeFeature(int iEarid, int iTailid)
{
	if( !m_pPlayerModel )
		return ;
	if( !m_pPortraitModel)
		return ;
	if (m_iPhysique != PHYSIQUE_SMALL)
		return ;
	SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
	if (pInfo && !(pInfo->mask_sect & (1<<SECT_11)) )
		return;
		
	char szPath[MAX_PATH];

	int level = m_BasicProps.iLevel/45;
	if(iEarid != m_iEarid)
	{
		m_iEarid = iEarid;
		if (m_iEarid & 0x80)
			sprintf(szPath, _optimal_ear_skin[m_iGender], (m_iEarid & 0x7f) + 1);
		else
			sprintf(szPath, _ear_skin[m_iGender], level+1, m_iEarid+1);
		A3DSkin* pEarSkin = g_pGame->LoadA3DSkin(szPath, true);

		A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_EAR_INDEX);
		g_pGame->ReleaseA3DSkin(pOldSkin);
		m_pPlayerModel->ReplaceSkin(SKIN_EAR_INDEX, pEarSkin, false);

//		pEarSkin = g_pGame->LoadA3DSkin(szPath, true);
//		m_pPortraitModel->ReplaceSkin(PORTRAIT_SKIN_EAR, pEarSkin, false);
	}

	if(iTailid != m_iTailid)
	{	
		m_iTailid = iTailid;

		if (m_iTailid & 0x80)
			sprintf(szPath, _optimal_tail_skin[m_iGender], (m_iTailid & 0x7f) + 1);
		else
			sprintf(szPath, _tail_skin[m_iGender], level+1, m_iTailid+1);
		A3DSkin* pTailSkin = g_pGame->LoadA3DSkin(szPath, true);

		A3DSkin* pOldSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_TAIL_INDEX);
		g_pGame->ReleaseA3DSkin(pOldSkin);
		m_pPlayerModel->ReplaceSkin(SKIN_TAIL_INDEX, pTailSkin, false);
	}
	UpdateCurSkins();
}

static const int _portrait_skin_map2[NUM_SKIN_INDEX] =
{
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	PORTRAIT_SKIN_NOSE,
	PORTRAIT_SIN_MUSTACHE,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
	-1,
};
void CECPlayer::LoadHeadPortraitModel()
{
	if(!m_pHeadPortraitModel)
	{
		m_pHeadPortraitModel = g_pGame->LoadA3DSkinModel(_skin_model_path[m_iPhysique][m_iGender], A3DSkinModel::LSF_NOSKIN);

		if (!m_pHeadPortraitModel)
		{
			return;
		}

		for (int i = 0; i < NUM_PORTAIT_SKIN; i++)
			m_pHeadPortraitModel->AddSkin(NULL, false);
	}

	for (int i = 0; i < NUM_SKIN_INDEX; i++)
	{
		const int nPortraitIndex = _portrait_skin_map2[i];

		if (nPortraitIndex >= 0)
		{
			A3DSkin* pSkin = m_pHeadPortraitModel->GetA3DSkin(nPortraitIndex);
			g_pGame->ReleaseA3DSkin(pSkin);
			if(m_pPortraitModel)
				m_pPortraitModel->ReplaceSkin(nPortraitIndex, NULL, false);

			if (m_aSkins[i][0])
			{
				A3DSkin* pNewSkin = g_pGame->LoadA3DSkin(m_aSkins[i][0]->GetFileName(), true);

				if (pNewSkin)
					m_pHeadPortraitModel->ReplaceSkin(nPortraitIndex, pNewSkin, false);
			}
		}
	}

	char szPath[MAX_PATH];
	sprintf(szPath, _head_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iFaceid, GetProfession()));
	A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);
	m_pHeadPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HEAD, pHeadSkin, false);

	sprintf(szPath, _hair_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(m_iHairid, GetProfession()));
	A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(szPath, true);
 	m_pHeadPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HAIR, pHairSkin, false);

	if(m_pHeadPortraitModel)
	{	
		m_pHeadPortraitModel->SetPos(g_vOrigin);
		m_pHeadPortraitModel->SetDirAndUp(g_vAxisZ, g_vAxisY);
		m_pHeadPortraitModel->PlayActionByNameDC("眨眼_通用", -1);
		m_pHeadPortraitModel->Update(1);
	}
	// 显示方案
	if(m_pHeadPortraitModel)
	{
		m_pHeadPortraitModel->ShowSkin(PORTRAIT_SKIN_HAIR, true);
	}	
}

void CECPlayer::ChangeHeadPortraitModel(int iFaceid, int iHairid)
{
	if(m_pHeadPortraitModel)
	{
		char szPath[MAX_PATH];
		sprintf(szPath, _head_skin[m_iPhysique][m_iGender], FaceHairId2FileIndex(iFaceid, GetProfession()));
		A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(szPath, true);
		m_pHeadPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HEAD, pHeadSkin, false);

		sprintf(szPath, _hair_skin[m_iPhysique][m_iGender], (iHairid));
		A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(szPath, true);
		m_pHeadPortraitModel->ReplaceSkin(PORTRAIT_SKIN_HAIR, pHairSkin, false);
	}
}

bool CECPlayer::CanEquipThis(int id)
{
	DATA_TYPE DataType;
    const EQUIPMENT_ESSENCE* pData = static_cast<const EQUIPMENT_ESSENCE*>(g_pGame->GetElementDataMan()->get_data_ptr(id, ID_SPACE_ESSENCE, DataType));
	
	if (pData && DataType == DT_EQUIPMENT_ESSENCE)
	{
		if (m_BasicProps.iLevel < pData->require_level)
			return false;
		if (((m_BasicProps.iProfession < 64)&&(!(pData->character_combo_id & (UINT64 (1) << m_BasicProps.iProfession)))) ||
			((m_BasicProps.iProfession >= 64)&&(!(pData->character_combo_id2 & (UINT64 (1) << (m_BasicProps.iProfession - 64))))) )
			return false;
		if ((int)(pData->require_gender) != 2 && (int)(pData->require_gender) != m_iGender)
			return false;
	}
	return true;
}

void CECPlayer::UpdateHideEquipMask(const int* pEquipmentID)
{
	m_dwHideEquipMaskInUse = m_dwHideEquipMask;

	if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
		return;
	for (int i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (pEquipmentID[i] && (!CanEquipThis(pEquipmentID[i] & 0xffff)) )
			m_dwHideEquipMaskInUse |= (1<<i);			
	}
}

void CECPlayer::ReloadModel()
{
	DetachBuddy();
	
	if(m_bMultiRideLeader)
	{
		for(int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if(m_iMultiBuddyId[i])
				DetachMultiBuddy(i);
		}
	}
	else if (m_bMultiRideMember)
	{
		CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);
		if(pLeader && pLeader->IsMultiRideLeader())
		{
			pLeader->RemoveMultiRideMember(m_PlayerInfo.cid);
		}
 	}
	if (m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride, false);
		A3DRELEASE(m_pPetModel);
	}
	ReleasePlayerModel();
	LoadPlayerSkeleton(m_aHideEquips, true);
}
// 飞升之后，玩家的种族会发生改变
void CECPlayer::UpdateRace()
{
	m_iRace = CECPlayer::GetRaceByProf(m_BasicProps.iProfession);
	m_iPhysique = CECPlayer::GetPhysiqueByProf(m_BasicProps.iProfession);
}

void CECPlayer::EnterInvisibleState(unsigned char state)
{
	m_bInvisible = state? true:false;
	if (m_bInvisible)
		SetTransparent(0.7f);
	else
		SetTransparent(-1.0f);
	//Else Player need update current action
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
}

void CECPlayer::EnterDimState(unsigned char state)
{
	m_bDim = state? true:false;
	if (m_bDim)
		SetTransparent(0.7f);
	else
		SetTransparent(-1.0f);
	//Else Player need update current action
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
}

void CECPlayer::ResetDoubleHitState()
{
	// reset
	if (m_pPlayerModel)
	{	
		SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
		if (pInfo && pInfo->mask_sect & (1<<SECT_13))  // 怀光
		{
			char szAct[100];
			szAct[0] = '\0';
			int nActType = m_uAttackType;
			if( nActType < 0 ) nActType = 0;
			if( nActType > 12 ) nActType = 12;
			const char * szSuffix = m_ActionNames.GetANSIString(nActType + ACT_SUFFIX_0);
			m_iDoubleHit = 0;

			if( m_pPetModel )	// Is riding
			{	
				sprintf(szAct, "%s_骑乘_%s", "战斗站立", szSuffix);
				m_pPetModel->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_GUARD), 1.0f, false);
			}
			else if( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) // 玩家在飞骑上面播放动作复用坐骑的动作
			{
				sprintf(szAct, "%s_骑乘_%s", "战斗站立", szSuffix);
			}
			else if (m_nFlyMode == FLYMODE_WING && IsFlying()) // 翅膀飞行状态播放对应的普攻动作和战斗站立动作
			{
				sprintf(szAct, "%_翅膀_%s", "战斗站立", szSuffix);
			}
			else
			{
				sprintf(szAct, "%s_%s", "战斗站立", szSuffix);
			}
			m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY, szAct, 1.0f, false, 300);	
		}
	}
}

void CECPlayer::SetCircleID(int id)
{
	m_idCircle = id;
}

void CECPlayer::SetCRole(int id)
{
	m_idCRole = id;
}

bool CECPlayer::IsInDomainState()
{
	// 第27 - 32位都是领域状态位
	__int64 mask = 0xFC000000;
	if (m_dwExtStates & mask)
	{
		return true;
	}
	else
		return false;
}

// 重置天华的弹琴动作
void CECPlayer::ResetPlayLuteAction()
{
	if (m_pPlayerModel)
	{	
	
	}
}

void CECPlayer::UpdateSpecialState(int state, bool on)
{
	if (on)
		m_iSpecialState |= (1 << state);
	else 
		m_iSpecialState &= ~(1<< state);
}

void CECPlayer::UpdateDeityStateGFX(int old_cult, int new_cult)
{
	if (old_cult == new_cult)
		return;
	if (m_BasicProps.iDeityLevel == 0)
		return;

	const char* szFile = NULL;
	const char* szFile2 = NULL;
	if (old_cult == 1)
		szFile = res_GFXFile(RES_GFX_DEITY_GOD);
	else if (old_cult == 2)
		szFile = res_GFXFile(RES_GFX_DEITY_DEVIL);
	else if (old_cult == 4)
		szFile = res_GFXFile(RES_GFX_DEITY_BUDDHA);

	if (new_cult == 1)
		szFile2 = res_GFXFile(RES_GFX_DEITY_GOD);
	else if (new_cult == 2)
		szFile2 = res_GFXFile(RES_GFX_DEITY_DEVIL);
	else if (new_cult == 4)
		szFile2 = res_GFXFile(RES_GFX_DEITY_BUDDHA);
	if (m_pBackupModel)
	{
		m_pBackupModel->RemoveGfx(szFile, "HH_Spine");
		m_pBackupModel->PlayGfx(szFile2, "HH_Spine");
	}
}

int CECPlayer::GetProfRace()
{
	int iProfRace = PROFRACE_UNKNOWN;
	int iProf = GetProfession();
	if (iProf<=12)
		iProfRace = (iProf+2)/3;
	else if (iProf<=24)
		iProfRace = (iProf-12+2)/3;
	else if (iProf<=30)
		iProfRace = PROFRACE_GUIDAO;
	else if (iProf<=38)
		iProfRace = PROFRACE_JIULI;
	else if (iProf<=44)
		iProfRace = PROFRACE_LIESHAN;
	else if (iProf<=50)
		iProfRace = PROFRACE_HUAIGUANG;
	else if (iProf<=55)
		iProfRace = PROFRACE_TIANHUA;
	else if (iProf<=68&&iProf>=64)
		iProfRace = PROFRACE_FENGXIANG;
	else if (iProf<=100&&iProf>=96)
		iProfRace = PROFRACE_TAIHAO;
	return iProfRace;
}

/*
人族	1,2,3 ...

烈山	1,2,3,4,5,6,7  31,32,33,34 ...
怀光	8,9,10,11,12,13,14,15 ... 30

  
九黎	1,2,3,4, ... 30
太昊	31,32,33, ...
	
天华	1,2,3 ...
	  
比如太昊头：大体型男头31.ski ...
*/
int CECPlayer::FaceHairId2Index(int id)
{
	int ret = id;
	if (GetProfRace()==PROFRACE_HUAIGUANG)
	{
		ret -= 7;
	}

	return ret;
}

int CECPlayer::FaceHairIndex2Id(int index)
{
	int ret = index;
	if (GetProfRace()==PROFRACE_HUAIGUANG)
	{
		ret += 7;
	}
	
	return ret;
}

int	CECPlayer::FaceHairId2FileIndex(int id, int iProf)
{
	int ret = id;
	if (iProf>=39&&iProf<=44)	// 烈山
	{
		if (id>6)
			ret = id+23;
	}
	else if (iProf>=96&&iProf<=101)
	{
		ret = id+30;
	}

	ret += 1;
	
	return ret;
}

void CECPlayer::CheckIndexBound(int& id)
{
	if (GetProfRace()==PROFRACE_HUAIGUANG)
	{
		if (id<7||id>=7+FACEHAIRTYPE_NUM)
			id = 7;
	}
	else
	{
		if (id<0 || id>=FACEHAIRTYPE_NUM)
			id = 0;
	}
}

/**
*@param: pModel:需要缩放的模型指针；fScaleRatio: 缩放比例
*@return: bool: 缩放操作成功与否
*@usage: 对具体的模型进行缩放
**/
bool CECPlayer::ScaleObjects( CECModel* pModel, float fScaleRatio )
{
	//判断输入参数的合法性
	if ( !pModel )
	{
		return false;
	}

	//设置新骨骼缩放标示布尔变量
	g_pA3DConfig->SetFlagNewBoneScale(true);

	//对该模型进行缩放
	pModel->ScaleAllRootBonesAndGfx(fScaleRatio);

	//对所有子模型进行缩放操作
	int iChildModelCount = pModel->GetChildCount();
	for ( int k = 0; k < iChildModelCount; ++k )
	{
		CECModel* pChildModel = pModel->GetChildModel(k);
		if ( pChildModel )
		{
			pChildModel->ScaleAllRootBonesAndGfx(fScaleRatio);
		}
	}

	//
//	pModel->Tick(1);
//	g_pA3DConfig->SetFlagNewBoneScale(false);

	//
	return true;
}
