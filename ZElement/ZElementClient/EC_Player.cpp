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
#include "EC_IvtrItem.h"
#include "EC_IvtrScroll.h"
#include "EC_Achievement.h"
#include "EC_Resource.h"
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
#include "EC_Team.h"
#include "EC_Faction.h"
#include "EC_Skill.h"
#include "ElementSkill.h"
#include "EC_PortraitRender.h"
#include "EC_PateText.h"
#include "EC_Configs.h"
#include "EC_GameSession.h"
#include "EC_SceneLoader.h"
#include "FWAssemblySet.h"
#include "FWTemplate.h"
#include "EC_NPC.h"
#include "EC_Interactor.h"
#include "EC_Sprite.h"
#include "EC_ManNPC.h"
#include "EC_GameUIMan.h"
#include "EC_UIManager.h"
#include "EC_Utility.h"
#include "A3DSkillGfxComposer.h"
#include "A3DLight.h"
#include "AFI.h"
#include "EC_HPWork.h"
#include "EC_ChangePill.h"
#include "EC_Achievement.h"
#include "EC_Circle.h"
#include "A3DHLSL.h"
#include "A3DEffect.h"

#include "elementdataman.h"
#include "privilege.hxx"
#include "ids.hxx"

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
#include "A3DCamera.h"
#include "A3DSkeleton.h"
#include "A3DBone.h"
#include "A3DFlatCollector.h"
#include "AAssist.h"
#include "AFileImage.h"
#include "A3DConfig.h" //Added 2011-06-28.
//#include "EC_CDR.h"		//Added 2012-06-14.
//#include "CDWithCH.h"	//Added 2012-06-14.
#include <A3DCollision.h>//Added 2012-06-14.
#include "A3DGFXFuncs.h"	//Added 2012-09-11.
#include "A3DEnvironment.h"
#include "A3DEngine.h"
#include "ConfigFromLua.h"
#include "LuaEvent.h"
#include "EC_LoginPlayer.h"
#include "EC_CDR.h"
#include "A3DTerrain2.h"
#include "elementdataman.h"
#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#endif

#define new A_DEBUG_NEW

#define WEAPON_POWER	15.0f
#define WEAPON_SPECULAR	0xffffffff

#define ACTION_SWITCH_INDEX	15    // 普通动作与骑乘动作分界线

//using namespace CHBasedCD;	//Added 2012-06-14.

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
int			g_nLoginPlayerDarkLightState = -1; //登陆时供用户切换光暗装备

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
static const char* _new_back_weapon_hanger = "NewBackWeapon";	//新的背部武器的hanger表示名字，非战斗状态
static const char* _wing				= "Wing";
static const char* _cc_ride				= "CC_Ride";
static const char* _cc_ride2			= "CC_Ride2";
static const char* _hh_ride				= "HH_Ride";
static const char* _hanger_ride			= "Rider";
static const char* _hanger_ride2		= "Rider2";
static const char* _hanger_hug			= "Hug";
static const char* _hanger_bath         = "Bath";
static const char* _hanger_puppet		= "Puppet";
static const char* _hh_kuilei			= "HH_kuilei";
static const char* _cc_kuilei			= "CC_kuilei";
static const char* _hh_bind				= "HH_Bind";
static const char* _hanger_interactor	= "Interactor";
static const char* _cc_bind[6]			= 
{
	"CC_Bind",
	"CC_Bind_xiao",
	"CC_Bind",
	"CC_Bind_da",
	"CC_Bind",
	"CC_Bind",
};

// active emote hanger
static const char* _hanger_emote = "Emote";

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

const char* _head_skin[6][2] = 
{
	{"Models\\Players\\形象\\男\\头\\男头%02d.ski",	"Models\\Players\\形象\\女\\头\\女头%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头\\小体型男头%02d.ski",	"Models\\Players\\形象\\小体型女\\头\\小体型女头%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头\\中体型男头%02d.ski",	"Models\\Players\\形象\\中体型女\\头\\中体型女头%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头\\大体型男头%02d.ski",	"Models\\Players\\形象\\大体型女\\头\\大体型女头%02d.ski"},
	{"Models\\Players\\形象\\微体型男\\头\\灵夙男头%02d.ski",	"Models\\Players\\形象\\微体型女\\头\\灵夙女头%02d.ski"}, //暂时
	{"Models\\Players\\形象\\四足男\\头\\英招男头%02d.ski",		"Models\\Players\\形象\\四足女\\头\\英招女头%02d.ski"},
};

const char* _hair_skin[6][2] =
{
	{"Models\\Players\\形象\\男\\头发\\男头发%02d.ski",	"Models\\Players\\形象\\女\\头发\\女头发%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头发\\小体型男头发%02d.ski",	"Models\\Players\\形象\\小体型女\\头发\\小体型女头发%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头发\\中体型男头发%02d.ski",	"Models\\Players\\形象\\中体型女\\头发\\中体型女头发%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头发\\大体型男头发%02d.ski",	"Models\\Players\\形象\\大体型女\\头发\\大体型女头发%02d.ski"},
	{"Models\\Players\\形象\\微体型男\\头发\\灵夙男头发%02d.ski",	"Models\\Players\\形象\\微体型女\\头发\\灵夙女头发%02d.ski"},
	{"Models\\Players\\形象\\四足男\\头发\\英招男头发%02d.ski",		"Models\\Players\\形象\\四足女\\头发\\英招女头发%02d.ski"},
};

static const char* _equipment_default_body_skin[6][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_shoe_skin[6][2] = 
{
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
};

static const char* _equipment_default_fashion_upper_skin[6][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_fashion_lower_skin[6][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女%02d.ski" },
};

static const char* _equipment_default_fashion_shoe_skin[6][2] = 
{
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男%02d.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女%02d.ski"},
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
	"Models\\Players\\形象\\英招变身麒麟\\英招变身麒麟.ecm",
	"Models\\Players\\形象\\男\\躯干\\男.ecm",
};

static const char* _puppet_model[] = 
{
	"models\\npcs\\宠物\\傀儡\\毒蝎傀儡\\毒蝎傀儡.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡炎虎\\傀儡炎虎.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡鲲鹏鸟\\傀儡鲲鹏鸟.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡双生菩提\\傀儡双生菩提.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡筑炮石人\\傀儡筑炮石人.ecm",	
};

static const char* _puppet_model2[] = 
{
	"models\\npcs\\宠物\\傀儡\\傀儡毒蝎2\\傀儡毒蝎2.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡炎虎2\\傀儡炎虎2.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡鲲鹏鸟2\\傀儡鲲鹏鸟2.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡双生菩提2\\傀儡双生菩提2.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡筑炮石人2\\傀儡筑炮石人2.ecm",
		
};

static const char* _puppet_model3[] = 
{
	"models\\npcs\\宠物\\傀儡\\傀儡毒蝎3\\傀儡毒蝎3.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡炎虎3\\傀儡炎虎3.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡鲲鹏鸟3\\傀儡鲲鹏鸟3.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡双生菩提3\\傀儡双生菩提3.ecm",
	"models\\npcs\\宠物\\傀儡\\傀儡筑炮石人3\\傀儡筑炮石人3.ecm",			
};

static const char* _skin_model_path[6][2] =
{
	{"Models\\Players\\形象\\男\\躯干\\男.smd",		"Models\\Players\\形象\\女\\躯干\\女.smd"},
	{"Models\\Players\\形象\\小体型男\\躯干\\小体型男.smd",		"Models\\Players\\形象\\小体型女\\躯干\\小体型女.smd"},
	{"Models\\Players\\形象\\中体型男\\躯干\\中体型男.smd",		"Models\\Players\\形象\\中体型女\\躯干\\中体型女.smd"},
	{"Models\\Players\\形象\\大体型男\\躯干\\大体型男.smd",		"Models\\Players\\形象\\大体型女\\躯干\\大体型女.smd"},
	{"Models\\Players\\形象\\微体型男\\躯干\\微体型男.smd",		"Models\\Players\\形象\\微体型女\\躯干\\微体型女.smd"},
	{"Models\\Players\\形象\\四足男\\躯干\\四足男.smd",			"Models\\Players\\形象\\四足男\\躯干\\四足男.smd"},
};

//四足体型变成人模型时的默认皮肤
static const char* _ski_path[] =
{
	"Models\\Players\\形象\\男\\头\\英招人形态男头.ski",
	"Models\\Players\\形象\\男\\头发\\英招人形态盔男.ski",
	"Models\\Players\\装备\\英招男\\英招人形态甲男\\英招人形态甲男.ski",
	"Models\\Players\\装备\\英招男\\英招人形态靴男\\英招人形态靴男.ski",

};

//四足体型的默认时装
static const char* _tetrapod_ski_path[] = 
{
	"models\\players\\装备\\默认\\默认衣服\\默认衣服英招男.ski",
	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子英招男.ski",
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

static A3DCOLOR l_fashionColor[] = 
{
	A3DCOLORRGB(255, 255, 255),  // 0
	A3DCOLORRGB(46,	139,	87), 
	A3DCOLORRGB(32,	178,	170), 
	A3DCOLORRGB(0,255,	0), 
	A3DCOLORRGB(50,	205,	50), 
	A3DCOLORRGB(34	,139,	34),  //5
	A3DCOLORRGB(238	,232,	170),
	A3DCOLORRGB(255	,255,	0),
	A3DCOLORRGB(205,	205,	180),
	A3DCOLORRGB(139,	139,	122),
	A3DCOLORRGB(255,	215,	0), //10
	A3DCOLORRGB(255,	193,	37),
	A3DCOLORRGB(139,	105,	20),
	A3DCOLORRGB(105,	105,	105),
	A3DCOLORRGB(211	,211,	211),
	A3DCOLORRGB(72,	61	,139), //15
	A3DCOLORRGB(	106,	90,	205),
	A3DCOLORRGB(65,	105,	225),
	A3DCOLORRGB(30,	144,	255),
	A3DCOLORRGB(0,	191,	255),
	A3DCOLORRGB(0	,0	,139), //20
	A3DCOLORRGB(99	,184,	255),
	A3DCOLORRGB(79,	148,	205),
	A3DCOLORRGB(54,	100,	139),
	A3DCOLORRGB(255,	187,	255),
	A3DCOLORRGB(139,	102,	139), //25
	A3DCOLORRGB(180	,82,	205),
	A3DCOLORRGB(122	,55	,139),
	A3DCOLORRGB(154,	50,	205),
	A3DCOLORRGB(155,	48,	255),
	A3DCOLORRGB(85,	26,	139),//30
	A3DCOLORRGB(171,	130,	255),
	A3DCOLORRGB(96,	123,	139),
	A3DCOLORRGB(224,	255,	255),
	A3DCOLORRGB(255	,225	,255),
	A3DCOLORRGB(139,	123,	139), // 35
	A3DCOLORRGB(28,	28,	28),
	A3DCOLORRGB(79	,	79	,	79),
	A3DCOLORRGB(130	,	130	,	130),
	A3DCOLORRGB(181	,	181	,	181),
	A3DCOLORRGB(232	,	232	,	232), // 40
	A3DCOLORRGB(0	,	0	,	0),
	A3DCOLORRGB(105	,	105	,	105),
	A3DCOLORRGB(0	,	139	,	139),
	A3DCOLORRGB(82	,	139	,	139),
	A3DCOLORRGB(155	,	205	,	155), // 45
	A3DCOLORRGB(105	,	139	,	105),
	A3DCOLORRGB(238	,	221	,	130),
	A3DCOLORRGB(218	,	165	,	32),
	A3DCOLORRGB(205	,	92	,	92),
	A3DCOLORRGB(255	,	106	,	106), // 50
	A3DCOLORRGB(139	,	58	,	58),
	A3DCOLORRGB(255	,	130	,	71),
	A3DCOLORRGB(139	,	71	,	38),
	A3DCOLORRGB(255	,	127	,	0),
	A3DCOLORRGB(238	,	106	,	80), // 55
	A3DCOLORRGB(139	,	62	,	47),
	A3DCOLORRGB(255	,	99	,	71),
	A3DCOLORRGB(238	,	64	,	0),
	A3DCOLORRGB(238	,	0	,	0),
	A3DCOLORRGB(176	,	48	,	96), // 60
	A3DCOLORRGB(199	,	21	,	133),
	A3DCOLORRGB(186	,	85	,	211),
	A3DCOLORRGB(138	,	43	,	226),
	A3DCOLORRGB(147	,	112	,	219),
	A3DCOLORRGB(238	,	59	,	59), // 65
	A3DCOLORRGB(255	,	140	,	105),
	A3DCOLORRGB(255	,	165	,	0),
	A3DCOLORRGB(205	,	133	,	0),
	A3DCOLORRGB(255	,	127	,	0),
	A3DCOLORRGB(238	,	59	,	59), // 70
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

	int i,k;
	for (i = 0; i < NUM_SKIN_INDEX; i++)
	{
		for(k=0;k<6;k++)
		{
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[k].aSkins[i][0]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[k].aSkins[i][1]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[k].aSkins[i][2]);
		}
	}

	for(i=0;i<6;i++)
	{
		if(Ret.pMirrorModel[i])
		{
			Ret.pMirrorModel[i]->Release();
			delete Ret.pMirrorModel[i];
		}
	}
}

//	Build pate faction text
static void _BuildPateFactionText(int idFaction, int iRank, ACString& strText)
{
	strText.Empty();

	if (!idFaction)
		return;

	//	Reset faction name
	Faction_Info* pFaction = g_pGame->GetFactionMan()->GetFaction(idFaction);
	if (!pFaction)
		return;

	strText = pFaction->GetName();

	//	Add faction rank
	if (iRank >= _R_MASTER && iRank <= _R_HEADER)
	{
		strText += _AL(" ");
		CECGameUIMan* pUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

		if (pUI)
			strText += pUI->GetStringFromTable(1300+iRank);
	}
}

static void _BuildPateCircleText(int idCircle, int iRank, ACString& strText)
{
	strText.Empty();
	if (!idCircle)
		return;

	Circle_Info* pCircle = g_pGame->GetCircleMan()->GetCircle(idCircle);
	if (!pCircle)
		return;

	strText = pCircle->GetName();	
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

void CECPlayer::ChangeDefaultUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin, int nXYDarkLightState)
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
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "太昊", nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_14)) || (pInfo->mask_sect&(1<<SECT_15)) )
	{
		if ( pInfo->mask_sect&(1<<SECT_14) )	//轩辕默认装备
		{
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "轩辕", nSkin+1);
			AString strLeft = str.Left( str.GetLength() - 4 );
			AString strRight = str.Right( 4 );
			if ( !nXYDarkLightState )			//光明状态
			{
				str = strLeft + "1" + strRight;
			}
			else if ( 1 == nXYDarkLightState )	//黑暗状态
			{
				str = strLeft + "2" + strRight;
			}
		}
		else									//天华默认装备
		{
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "天华", nSkin+1);
		}
	}
	else if((pInfo->mask_sect & (1 << SECT_16)) || (pInfo->mask_sect & (1 << SECT_17)))
	{
		if(pInfo->mask_sect & (1 << SECT_16))    //灵夙默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "灵夙", nSkin + 1);
		else								    //英招默认装备
			str.Format(_equipment_default_body_skin[iPhysique][nGender], "英招", nSkin + 1);
	}
	else
	{
		str.Format(_equipment_default_body_skin[iPhysique][nGender], nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
}

void CECPlayer::ChangeDefaultFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin, int nXYDarkLightState)
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
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "太昊", nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_14)) || (pInfo->mask_sect&(1<<SECT_15)) )
	{
		if ( pInfo->mask_sect&(1<<SECT_14) )	//轩辕默认装备
		{
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "轩辕", nSkin+1);
			AString strLeft = str.Left( str.GetLength() - 4 );
			AString strRight = str.Right( 4 );
			if ( !nXYDarkLightState )			//光明状态
			{
				str = strLeft + "1" + strRight;
			}
			else if ( 1 == nXYDarkLightState )	//黑暗状态
			{
				str = strLeft + "2" + strRight;
			}			
		}
		else									//天华默认装备
		{
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "天华", nSkin+1);
		}
	}
	else if((pInfo->mask_sect & (1 << SECT_16)) || (pInfo->mask_sect & (1 << SECT_17)))
	{
		if(pInfo->mask_sect & (1 << SECT_16))       //灵夙默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "灵夙", nSkin+1);
		else										//英招默认装备
			str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "英招", nSkin+1);
	}
	else
	{
		str.Format(_equipment_default_shoe_skin[iPhysique][nGender], nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	ChangeArmor(str, aSkins, enumSkinShowFoot, true, iPhysique, iProfession, nGender, nSkin+1);
}

void CECPlayer::ChangeDefaultFashionUpper(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin, int nXYDarkLightState)
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
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "太昊", nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_14)) || (pInfo->mask_sect&(1<<SECT_15)) )
	{
		if ( pInfo->mask_sect&(1<<SECT_14) )	//轩辕默认装备
		{
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "轩辕", nSkin+1);
			AString strLeft = str.Left( str.GetLength() - 4 );
			AString strRight = str.Right( 4 );
			if ( !nXYDarkLightState )			//光明状态
			{
				str = strLeft + "1" + strRight;
			}
			else if ( 1 == nXYDarkLightState )	//黑暗状态
			{
				str = strLeft + "2" + strRight;
			}
		}
		else									//天华默认装备
		{
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "天华", nSkin+1);
		}
	}
	else if((pInfo->mask_sect & (1 << SECT_16)) || (pInfo->mask_sect & (1 << SECT_17)))
	{
		if(pInfo->mask_sect & (1 << SECT_16))      //灵夙默认装备
			str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "灵夙", nSkin+1);
		else if(pInfo->mask_sect & (1 << SECT_17) && iPhysique == PHYSIQUE_NORMAL)    //四组体型变为人形时的默认装备
		{
			str.Format("%s", _ski_path[2]);
		}
		else																			//英招默认装备
		{
				str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], "英招", nSkin+1);
		}
	}
	else
	{
		str.Format(_equipment_default_fashion_upper_skin[iPhysique][nGender], nSkin+1);
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1, nXYDarkLightState);
}

void CECPlayer::ChangeDefaultFashionLower(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin, int nXYDarkLightState)
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
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "太昊", nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_14)) || (pInfo->mask_sect&(1<<SECT_15)) )
	{
		if ( pInfo->mask_sect&(1<<SECT_14) )	//轩辕默认装备
		{
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "轩辕", nSkin+1);
			AString strLeft = str.Left( str.GetLength() - 4 );
			AString strRight = str.Right( 4 );
			if ( !nXYDarkLightState )			//光明状态
			{
				str = strLeft + "1" + strRight;
			}
			else if ( 1 == nXYDarkLightState )	//黑暗状态
			{
				str = strLeft + "2" + strRight;
			}
		}
		else									//天华默认装备
		{
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "天华", nSkin+1);
		}
	}
	else if((pInfo->mask_sect & (1 << SECT_16)) || (pInfo->mask_sect & (1 << SECT_17)))
	{
		if(pInfo->mask_sect & (1 << SECT_16))    //灵夙默认装备
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "灵夙", nSkin+1);
		else if(pInfo->mask_sect & (1 << SECT_17) && iPhysique == PHYSIQUE_NORMAL)
		{
			str.Format("%s", _ski_path[2]);
		}
		else							        //英招默认装备
		{
			str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], "英招", nSkin+1);
		}
	
	}
	else
	{
		str.Format(_equipment_default_fashion_lower_skin[iPhysique][nGender], nSkin+1);
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1, nXYDarkLightState);
}

void CECPlayer::ChangeDefaultFashionFoot(A3DSkin* aSkins[3], int iPhysique, int iProfession, int nGender, int nSkin, int nXYDarkLightState)
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
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_10)) || (pInfo->mask_sect&(1<<SECT_12)) ) 
	{
		if (pInfo->mask_sect&(1<<SECT_10))  // 九黎默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "九黎", nSkin+1);
		else								// 太昊默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "太昊", nSkin+1);
//		ChangeArmor(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, nSkin+1);
	}
	else if ( (pInfo->mask_sect&(1<<SECT_14)) || (pInfo->mask_sect&(1<<SECT_15)) )
	{
		if ( pInfo->mask_sect&(1<<SECT_14) )	//轩辕默认装备
		{
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "轩辕", nSkin+1);
			AString strLeft = str.Left( str.GetLength() - 4 );
			AString strRight = str.Right( 4 );
			if ( !nXYDarkLightState )			//光明状态
			{
				str = strLeft + "1" + strRight;
			}
			else if ( 1 == nXYDarkLightState )	//黑暗状态
			{
				str = strLeft + "2" + strRight;
			}
		}
		else									//天华默认装备
		{
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "天华", nSkin+1);
		}
	}
	else if((pInfo->mask_sect & (1 << SECT_16)) || (pInfo->mask_sect & (1 << SECT_17)))
	{
		if(pInfo->mask_sect & (1 << SECT_16))			//灵夙默认装备
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "灵夙", nSkin+1);
		else if(pInfo->mask_sect & (1 << SECT_17) && iPhysique == PHYSIQUE_NORMAL)
		{
			str.Format("%s", _ski_path[3]); 
		}
		else                                            		//英招默认装备
		{
			str.Format(_equipment_default_fashion_shoe_skin[iPhysique][nGender], "英招", nSkin+1);

		}
	}
	else
	{
		str.Format(_equipment_default_shoe_skin[iPhysique][nGender], nSkin+1);
//		ChangeFashion(str, aSkins, enumSkinShowUpperBody, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1);
	}
	ChangeFashion(str, aSkins, enumSkinShowFoot, true, iPhysique, iProfession, nGender, 0xffffffff, nSkin+1, nXYDarkLightState);
}

bool CECPlayer::LoadPlayerEquips(
	int iRoleID,	//!!!!!Modified 2012-08-21!!!!!
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
		iRoleID,
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
	int iRoleID,
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
	static const char* aModelFiles[6][2] =
	{
		{"Models\\Players\\形象\\男\\躯干\\男.ecm",		"Models\\Players\\形象\\女\\躯干\\女.ecm"},
		{"Models\\Players\\形象\\小体型男\\躯干\\小体型男.ecm",		"Models\\Players\\形象\\小体型女\\躯干\\小体型女.ecm"},	
		{"Models\\Players\\形象\\中体型男\\躯干\\中体型男.ecm",		"Models\\Players\\形象\\中体型女\\躯干\\中体型女.ecm"},	
		{"Models\\Players\\形象\\大体型男\\躯干\\大体型男.ecm",		"Models\\Players\\形象\\大体型女\\躯干\\大体型女.ecm"},	
		{"Models\\Players\\形象\\微体型男\\躯干\\微体型男.ecm",		"Models\\Players\\形象\\微体型女\\躯干\\微体型女.ecm"},
		{"Models\\Players\\形象\\四足男\\躯干\\四足男.ecm",			"Models\\Players\\形象\\四足女\\躯干\\四足女.ecm"},
	};

	memset(&Ret, 0, sizeof(Ret));
	memset(&Ret.EquipResult[0].aEuips, 0xff, sizeof(Ret.EquipResult[0].aEuips));

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
		DWORD dwMask = EQUIP_MASK_ALL;
		//-------------------------------------------------------------------------
		//如果装备了时装武器，则不显示武器;如果不装备时装武器，重新显示武器。
		if (pEquips[EQUIPIVTR_FASHION_WEAPON] == 0)
			dwMask &= ~(1 << EQUIPIVTR_FASHION_WEAPON);
		else
			dwMask &= ~(1 << EQUIPIVTR_WEAPON);

		ShowEquipments(
			iRoleID,
			iPhysique,
			iProfession,
			iDefaultSkin,
			iGender,
			false,
			pEquips,
			dwMask,
			&Ret.EquipResult[0]
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

//	Load player mirror model
bool CECPlayer::LoadMirrorModel(
	int iRoleID,
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

	for(int i=0;i<6;i++)
	{
		memset(&Ret.EquipResult[i].aEuips, 0xff, sizeof(Ret.EquipResult[i].aEuips));

		//	Load mirror model ----------------------------
		if (!(Ret.pMirrorModel[i] = new CECModel))
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECPlayer::LoadMirrorModel", __LINE__);
			_ReleaseLoadModel(Ret);
			return false;
		}

		//	Load skeleton without skins
		const char* szFile = aModelFiles[iPhysique][iGender];
		if (!Ret.pMirrorModel[i]->Load(szFile, true, A3DSkinModel::LSF_NOSKIN, /*true*/false))
		{
			a_LogOutput(1, "CECPlayer::LoadPlayerModel, Failed to load player model %s", szFile);
			_ReleaseLoadModel(Ret);
			return false;
		}

		A3DSkinModel* pA3DModel = Ret.pMirrorModel[i]->GetA3DSkinModel();

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
		Ret.pMirrorModel[i]->SetActionEventMask(ACT_CHANNEL_EYE, EVENT_MASK_MATCHG);
		
		// wound channel
		A3DSkeleton* pSkeleton = pA3DModel->GetSkeleton();
		int iNumJoint = pSkeleton->GetJointNum();
		int* aJoints = new int [iNumJoint];

		for (int m = 0; m < iNumJoint; m++)
			aJoints[m] = m;

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

		for (int n = 0; n < NUM_SKIN_INDEX; n++)
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
				iRoleID,
				iPhysique,
				iProfession,
				iDefaultSkin,
				iGender,
				false,
				pEquips,
				EQUIP_MASK_ALL,
				&Ret.EquipResult[i]
			);
		}

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

	}

	return true;
}

bool CECPlayer::LoadPetModel(const char* szPetPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pPetModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
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
	//变身为人形
	if(strstr(szPath, "Models\\Players\\形象\\男\\躯干\\男.ecm"))
	{
		LoadCharacterModel(szPath, Ret);
		return true;	
	}
	else
	{
		Ret.pChangedModel = new CECModel();

		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
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
}

bool CECPlayer::LoadCharacterModel(const char* szPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pChangedModel = new CECModel();
	Ret.pChangedModel->Load(szPath, true, A3DSkinModel::LSF_DEFAULT, true);

	A3DSkinModel* pA3DModel = Ret.pChangedModel->GetA3DSkinModel();

	if (!pA3DModel)
	{
		_ReleaseLoadModel(Ret);
		return false;
	}

	A3DSMActionChannel* pChannel;

	pChannel = pA3DModel->BuildActionChannel(
		ACT_CHANNEL_BODY,
		_male_eye_joint_indice.size(),
		_male_eye_joint_indice.begin(),
		true);
	pChannel->SetWeight(1.0f);
	pChannel = pA3DModel->BuildActionChannel(
		ACT_CHANNEL_EYE,
		_male_eye_joint_indice.size(),
		_male_eye_joint_indice.begin());
	pChannel->SetWeight(1.0f);
	Ret.pChangedModel->SetActionEventMask(ACT_CHANNEL_EYE, EVENT_MASK_MATCHG);

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

	pChannel->SetPlayMode(A3DSMActionChannel::PLAY_COMBINE);

	for (int i = 0; i < NUM_SKIN_INDEX; i++)
		Ret.pChangedModel->AddSkin(NULL, false);

	A3DSkin* pHeadSkin = g_pGame->LoadA3DSkin(_ski_path[0], true);
	A3DSkin* pHairSkin = g_pGame->LoadA3DSkin(_ski_path[1], true);

	if(Ret.pChangedModel->GetA3DSkinModel())
	{
		Ret.pChangedModel->GetA3DSkinModel()->ReplaceSkin(SKIN_HEAD_INDEX, pHeadSkin, true);
		Ret.pChangedModel->GetA3DSkinModel()->ReplaceSkin(SKIN_HAIR_INDEX, pHairSkin, true);

	}
	return true;
}

bool CECPlayer::LoadTransfiguredModel(const char * szPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pTransfiguredModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
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

bool CECPlayer::LoadPuppetModel(const char* szPath, EC_PLAYERLOADRESULT& Ret)
{
	Ret.pPuppetModel = new CECModel();

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	if(!Ret.pPuppetModel->Load(szPath, true, A3DSkinModel::LSF_NOSKIN, false))
	{
		delete Ret.pPuppetModel;
		Ret.pPuppetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}

	char szSkin[MAX_PATH];
	strncpy(szSkin, szPath, MAX_PATH);;
	glb_ChangeExtension(szSkin, "ski");
	A3DSkin* pSkin = g_pGame->LoadA3DSkin(szSkin, false);
	if(!pSkin)
	{
		Ret.pPuppetModel->Release();
		delete Ret.pPuppetModel;
		Ret.pPuppetModel = NULL;
		g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		return false;
	}
	Ret.pPuppetModel->GetA3DSkinModel()->AddSkin(pSkin, true);
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

//	Release player model
void CECPlayer::ReleasePlayerModel(EC_PLAYERLOADRESULT& Ret)
{
	int i;
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

		for (i = 0; i < NUM_PORTAIT_SKIN; i++)
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
	A3DRELEASE(Ret.pPuppetModel);

	for (i = 0; i < 6; i++)
	{
		A3DRELEASE(Ret.pMirrorModel[i]);

		A3DRELEASE(Ret.EquipResult[i].pLeftHandWeapon);
		A3DRELEASE(Ret.EquipResult[i].pRightHandWeapon);
		A3DRELEASE(Ret.EquipResult[i].pBackWeapon);
		A3DRELEASE(Ret.EquipResult[i].pNewBackWeapon);//Added 2012-08-02.
		A3DRELEASE(Ret.EquipResult[i].pTalisman);
		A3DRELEASE(Ret.EquipResult[i].pWing);
		A3DRELEASE(Ret.EquipResult[i].pTune);
	}

	for (i = 0; i < 6; i++)
	{
		for(int j=0;j<3;j++)
		{
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_HEAD_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_HAIR_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_BODY_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_FOOT_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_HELM_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_GLASSES_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_NOSE_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_MUSTACHE_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_CLOAK_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_FASHION_UPPER_BODY_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_FASHION_LOWER_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_FASHION_FOOT_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_FASHION_HEADWEAR_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_EAR_INDEX][j]);
			g_pGame->ReleaseA3DSkin(Ret.EquipResult[i].aSkins[SKIN_TAIL_INDEX][j]);
		}
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
	else if(7 == id)
	{
		return _changed_shape_path[9];
	}
	else if(8 == id)
		return _changed_shape_path[10];
	else
		return NULL;
}

const char* CECPlayer::GetTransfiguredModelFileName(int id, char cType)
{
	DATA_TYPE DataType;
	const char* sz = "";

	if ( !cType )	//普通变身时模型路径获取逻辑
	{
		const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(id, ID_SPACE_CONFIG, DataType);
		if (DataType != DT_CHANGE_SHAPE_PROP_CONFIG)
			return "";
		const CHANGE_SHAPE_PROP_CONFIG* pData = (const CHANGE_SHAPE_PROP_CONFIG*)pDataPtr;
		
		const MONSTER_ESSENCE* pMonster = (const MONSTER_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pData->monster_id, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_MONSTER_ESSENCE)
			return "";

		sz = g_pGame->GetDataPath(pMonster->file_model);
		if (!sz)
			return "";
	}
	else			//处于碰撞副本中的变身时模型路径获取逻辑 Added 2012-08-16.
	{
		DATA_TYPE dt;
		int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);
		const int MAX_TRANSFIGURE_INFO_SIZE = 20;

		while (idEssence)
		{
			if (dt == DT_COLLISION_RAID_TRANSFIGURE_CONFIG)
			{
				const COLLISION_RAID_TRANSFIGURE_CONFIG* pData = (const COLLISION_RAID_TRANSFIGURE_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
				for ( int i = 0; i < MAX_TRANSFIGURE_INFO_SIZE; ++i )
				{
					if ( pData->transfigure_info_list[i].transfigure_id == (unsigned int)id )
					{
						sz = g_pGame->GetDataPath(pData->transfigure_info_list[i].file_matter);
						break;
					}
				}
				break;
			}
			idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
		}
		//---------------------------------------------------------------------
		//Test
//		if ( !sz || sz == "" )
//		{
//			sz = "models\\npcs\\npcs\\碰撞诛小仙陆雪琪\\碰撞诛小仙陆雪琪.ecm";
//		}
		//---------------------------------------------------------------------
	}

	return sz;
}

const char* CECPlayer::GetPuppetModelFileName(int iRoleid, int id)
{
	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(iRoleid);
	
	int iPuppetState = pPlayer->GetPuppetState();

	if(id >=1 && id <=5)
	{
		if(iPuppetState == PUPPET_STATE_FIGHT)
			return _puppet_model[id - 1];
		else if(iPuppetState == PUPPET_STATE_NORMAL)
			return _puppet_model2[id - 1];
		else if(iPuppetState == PUPPET_STATE_ANIM)
			return _puppet_model3[id - 1];
	}

	return "";
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
	if((idProf>=0 && idProf<=30) ||
	   (idProf>=64 && idProf<=69))
	   return RACE_HUMAN;
	else if((idProf>=32 && idProf<=60) ||
		    (idProf>=96 && idProf<=100))
	   return RACE_ORC;
	else if((idProf>=102 && idProf<=106) ||
		    (idProf>=108 && idProf<=112))
	   return RACE_EVILKIND;
	else
	{
	   assert(0 && "error prof");
	   return RACE_HUMAN;
	}
	//return ((idProf < 32) || ((idProf >= 64)&&(idProf <= 95))) ? RACE_HUMAN:RACE_ORC; //Modified 2011-08-01.
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
				if (i == SECT_15 || i == SECT_14)//轩辕和天华同为中等体型 Modified 2012-08-14.
					return PHYSIQUE_MIDDLE;
				if(i == SECT_16)
					return PHYSIQUE_TINY;
				if(i == SECT_17)
					return PHYSIQUE_TETRAPOD;

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

//玩家在泡澡地图中的时装
const int CECPlayer::m_EquipsInBath[][14] = 
{
	{0,0,
	 0,0,
	 0,0,
	 0,0,
	 0,0,
	 58206,58204,
	 0,58208},
	{0,0,
	 0,0,
	 0,0,
	 0,0,
	 0,0,
	 58207,58205,
	 0,58210},
};

CECPlayer::CECPlayer(CECPlayerMan* pPlayerMan) : m_matRelCarrier(A3DMATRIX4::IDENTITY)
{
	m_iCID				= OCID_PLAYER;
	m_pPlayerMan		= pPlayerMan;
	m_pPlayerModel		= NULL;
	m_pDummyModel		= NULL;
	m_pBackupModel		= NULL;
	m_pCacheModel		= NULL;
	m_pPetModel			= NULL;
	m_pChangedModel		= NULL;
	m_pTransfiguredModel= NULL;
	m_pInteractorModel	= NULL;
	m_pPuppetModel		= NULL;
	m_pPortraitModel	= NULL;
	m_pHeadPortraitModel= NULL;
	m_pBoothModel		= NULL;
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
	m_pTeam				= NULL;
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
	m_uAttackTypeBackUp	= 0;
	m_bAboutToDie		= false;
	m_dwExtStates		= 0;
	m_pCurSkill			= NULL;
	m_idCurSkillTarget	= 0;
	m_bFight			= false;
	m_iReputation		= 0;
	m_iCultivation		= 0;
	m_iTitle			= 0;
	m_iTitle            = TITLE_NONE;
	m_bUpdateFrameCount = false;
	m_TitleInfo.iCurrentframe = 0;
	m_TitleInfo.iTitleFrameNum = -1;
	m_TitleInfo.pSprite = NULL;
	m_idSpouse			= 0;
	m_idMaster			= 0;
	m_bInSanctuary		= false;
	m_iBoothState		= 0;
	m_crcBooth			= 0;
	m_idBoothItem       = -1;
	m_pSpeBoothModel    = NULL;
	m_bFashionMode		= false;
	m_idFaction			= 0;
	m_idFamily			= 0;
	m_idFRole			= GNET::_R_UNMEMBER;
	m_bRushFly			= false;
	m_AttachMode		= enumAttachNone;
	m_bHangerOn			= false;
	m_iBuddyId			= 0;
	m_idCandBuddy		= 0;
	m_idCandBathBuddy   = 0;
	m_idCandQilinBuddy  = 0;
	m_idQilinConnectBuddy = 0;
	m_bQilinConnectHangerOn = false;
	m_bAddQilinLinkGfx  = false;
	m_CandPet.id		= 0;
	m_CandPet.iLevel	= 0;
	m_bCandHangerOn		= false;
	m_nFlyMode			= 0;
	m_fPetHeight		= 0;
	m_pFactionDecal		= NULL;
	m_pTitleDecal       = NULL;
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
	m_dwTetrapodFashionMask = 0;
	m_iChangeShape		= 0;
	m_iCandChangeShape	= 0;
	m_bPlayerModelIsChangedModel = false;
	m_iPupPetState		= PUPPET_STATE_NONE;
	m_iPuppetId			= 0;
	m_iCandPupPetId		= 0;
	m_PuppetStateCount.SetPeriod(60000);
	m_PuppetStateCount.Reset(true);
	m_PuppetNormalStandCnt.SetPeriod(25000);
	m_PuppetNormalStandCnt.Reset(false);
	m_PuppetAnimCnt.SetPeriod(8000);
	m_PuppetAnimCnt.Reset(false);
	m_nRebornCount		= 0;
	m_pAchievementMan   = NULL;
	m_idCarrier			= 0;
	
	m_fDistToCamera		= 0.0f;

	if ((m_pPateName = new CECPateText))
		m_pPateName->EnableBorder(true);

	m_pPateTitle = new CECPateText;
	m_pPateCityOwner = new CECPateText;
	m_pPateFamily = new CECPateText;

	if ((m_pPateBooth = new CECPateText))
		m_pPateBooth->EnableBorder(true);

	if ((m_pPateFaction = new CECPateText))
		m_pPateFaction->EnableBorder(true);
	m_pPateRankName = new CECPateText;
	
	if ((m_pPateCircle = new CECPateText))
		m_pPateCircle->EnableBorder(true);

	m_pPateLastWords1 = new CECPateText;
	m_pPateLastWords1->EnableBackground(true);
	m_pPateLastWords2 = new CECPateText;
	m_pPateTeamReq = new CECPateText;
	m_pBubbleTexts = new CECBubbleDecalList;

	if ((m_pKillNum = new CECPateText))
		m_pKillNum->EnableBorder(true);

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
	memset(&m_CandInteractorInfo, 0, sizeof(m_CandInteractorInfo));
	memset(&m_InteractorInfo, 0, sizeof(m_InteractorInfo));
	memset(&m_CandEmoteInfo, 0, sizeof(m_CandEmoteInfo));
	m_iLeaderId = 0;
	m_iCandLeaderId = 0;
	m_iCandMultiRidePos = 0;
	m_bMultiRideMember = false;
	m_bMultiRideLeader = false;
	m_iApplyBattleType	= -1;
	m_iBattleType		= -1;    // 没在战场赋值为-1
	m_iBathType         = -1;
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
	m_cIsHideVip		= 0;
	m_bEquipRuneOpen	= false;
	m_nRuneQuality		= -1;
	m_bSpecialMoving	= false;//Added 2012-07-20.
	m_iCollisionSkillState	= 0;	//Added 2012-08-01.
	m_iBlackAndLightValue	= 0;	//Added 2012-08-03.	
	m_dwForceChangeMask	= 0;		//Added 2012-08-06.
	m_bCollisionState	= false;	//Added 2012-08-17.
	m_cXuanYuanDarkLightState	= 0;//Added 2012-08-20.
	m_ucWingChangeColor			= 0;//Added 2012-08-20.	
	m_iMass						= 1;//质量不能为0，至少为1 Added 2012-08-22.
	m_pDirVisibleStateGFX		= NULL;
	m_bSkillFly					= false;
	m_iExtraEquipEffect			= 0;
	m_iFlowBattle				= 0;
	memset( m_pMultiLineSkillGfx, 0, sizeof(m_pMultiLineSkillGfx) );//初始化多线性技能攻击特效指针 Added 2012-09-11.

	m_iMirrorImageCnt			= 0;
	m_iBattleKill = 0;
	m_nScore = 0;

	m_bInteractiveSubject		= false;
	m_bInteractiveRenderSubject	= false;

	m_dwPoseOnPet				= POSE_ON_PET_NORMAL;

	int i;
	for(i=0;i<3;i++)
	{
		m_cDarkLightSpirits[i] = 0;
		m_pSpiritsGFX[i] = NULL;

		m_SpiritModels[i] = NULL;
	}
	m_fSpiritsAngle	= 0.0f;

	for(i=0;i<6;i++)
	{
		m_MirrorModels[i] = NULL;
	}

	m_strLastSayCnt.SetPeriod(20000);
	m_IncantCnt.SetPeriod(1000);
	m_IncantCnt.Reset(true);
	m_ScaleWeaponCnt.SetPeriod(1000);

	m_bQilinFlashMove = false;

	m_iEmoteType = 0;
	m_iEmoteActionType = 0;
	m_iTravalVehicleId = 0;

	m_bBloodPool = false;
	m_iCurBloodPool = 0;
	m_iMaxBloodPool = 0;

	m_bAttachedInteractor = false;
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

	if( !file.Open("configs\\male_eye_joints.txt", AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY) )
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

	if( !file.Open("configs\\female_eye_joints.txt", AFILE_OPENEXIST | AFILE_TEXT | AFILE_TEMPMEMORY) )
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
	DetachQilinBuddy();
	StopActiveEmote();

	//	Cancel interaction.
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_InteractorInfo.id);
	if (pInteractor)
	{
		CancelInteract(m_InteractorInfo.id);
		pInteractor->CancelInteract(m_PlayerInfo.cid);
	}

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

	if (m_pPateName)
	{
		delete m_pPateName;
		m_pPateName = NULL;
	}

	if (m_pPateTitle)
	{
		delete m_pPateTitle;
		m_pPateTitle = NULL;
	}

	if (m_pPateCityOwner)
	{
		delete m_pPateCityOwner;
		m_pPateCityOwner = NULL;
	}

	if (m_pPateFamily)
	{
		delete m_pPateFamily;
		m_pPateFamily = NULL;
	}

	if (m_pPateLastWords1)
	{
		delete m_pPateLastWords1;
		m_pPateLastWords1 = NULL;
	}

	if (m_pPateLastWords2)
	{
		delete m_pPateLastWords2;
		m_pPateLastWords2 = NULL;
	}

	if (m_pPateTeamReq)
	{
		delete m_pPateTeamReq;
		m_pPateTeamReq = NULL;
	}

	if (m_pPateBooth)
	{
		delete m_pPateBooth;
		m_pPateBooth = NULL;
	}

	if (m_pPateFaction)
	{
		delete m_pPateFaction;
		m_pPateFaction = NULL;
	}

	if(m_pPateRankName)
	{
		delete m_pPateRankName;
		m_pPateRankName = NULL;
	}
	
	if (m_pPateCircle)
	{
		delete m_pPateCircle;
		m_pPateCircle = NULL;
	}

	if (m_pFactionDecal)
	{
		delete m_pFactionDecal;
		m_pFactionDecal = NULL;
	}

	if (m_pTitleDecal)
	{
		delete m_pTitleDecal;
		m_pTitleDecal = NULL;
	}

	if (m_pBubbleTexts)
	{
		delete m_pBubbleTexts;
		m_pBubbleTexts = NULL;
	}
	
	if (m_pKillNum)
	{
		delete m_pKillNum;
		m_pKillNum = NULL;
	}


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

	if ( m_pDirVisibleStateGFX )
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pDirVisibleStateGFX);
		m_pDirVisibleStateGFX = NULL;
	}

	if(m_pPlayerModel)
		m_pPlayerModel->RemoveGfx(res_GFXFile(RES_GFX_HIGHLEVEL), "HH_Spine");

	// Release the Multiline skill gfx
	for ( int j = 0; j < MAX_MULTILINE_SKILL_GFX_NUM; ++j )
	{
		g_pGame->GetGFXCaster()->ReleaseGFXEx( m_pMultiLineSkillGfx[j] );
		m_pMultiLineSkillGfx[j] = NULL;
	}

	if (m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride, false);
		A3DRELEASE(m_pPetModel);
	}

	if (m_pAchievementMan)
	{
		delete m_pAchievementMan;
		m_pAchievementMan = NULL;
	}

	A3DRELEASE(m_pChangedModel);
	
	A3DRELEASE(m_pTransfiguredModel);

	if(m_pPuppetModel)
	{
		m_pPuppetModel->RemoveChildModel(_hanger_puppet, false);
		A3DRELEASE(m_pPuppetModel);
	}

	if(m_pBackupModel && m_iChangeShape == SHAPE_HUMAN && m_bPlayerModelIsChangedModel)
	{
		m_pBackupModel->RemoveChildModel(_wing);
		m_pBackupModel->Release();
		delete m_pBackupModel;
		m_pPlayerModel = m_pCacheModel;
		m_pBackupModel = m_pCacheModel;	
	}

	//	Release dummy model
	ReleaseDummyModel();

	//	Release player model
	ReleasePlayerModel();

	int i;
	// clear mirror players first
	for(i=0;i<6;i++)
	{
		CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
		if(pMirrorPlayer)
			A3DRELEASE(pMirrorPlayer);
	}

	for(i=0;i<3;i++)
	{
		if(m_pSpiritsGFX[i])
		{
			g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSpiritsGFX[i]);
			m_pSpiritsGFX[i] = NULL;
		}

		if(m_SpiritModels[i])
			A3DRELEASE(m_SpiritModels[i]);
	}

	//	Clear resource ready flags
	SetResReadyFlag(RESFG_ALL, false);

	m_bLoadFlag = false;
	m_bAboutToDie = false;
}

bool CECPlayer::LoadBoothSkinModel()
{
	int iResIdx = (m_iGender == GENDER_MALE) ? RES_MOD_BOOTH_M : RES_MOD_BOOTH_F;
	m_pBoothModel = g_pGame->LoadA3DSkinModel(res_ModelFile(iResIdx));
	if (!m_pBoothModel)
		return false;
	
	m_pBoothModel->EnableSpecular(true);
	return true;
}

bool CECPlayer::UpdateBoothModel()
{
	if (m_iBoothState != 2)
		return false;
	
	if (NULL == m_pBoothModel)
	{
		LoadBoothSkinModel();
	}
	/*else
	{
		AString strMaleAct = _EA("动作");
		AString strFemaleAct = _EA("新动作");
		AString strAct = (m_iGender == GENDER_MALE) ? strMaleAct : strFemaleAct;
		m_pBoothModel->PlayActionByName(strAct, 1.0f, 200, false);
	}*/
	return true;
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
	if (m_pBoothModel)
	{
		g_pGame->ReleaseA3DSkinModel(m_pBoothModel);
		m_pBoothModel = NULL;
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

void CECPlayer::SummonPupPet(int idPuppet)
{
	if(idPuppet)
	{
		if(m_bFight)
			m_iPupPetState = PUPPET_STATE_FIGHT;
		else
			m_iPupPetState = PUPPET_STATE_NORMAL;
		m_iPuppetId = idPuppet;
		m_iCandPupPetId = idPuppet;
	}
	else
	{
		if(m_pPuppetModel)
		{
			m_pPuppetModel->StopChildrenAct();
			m_pPuppetModel->RemoveChildModel(_hanger_puppet, false);
			A3DRELEASE(m_pPuppetModel);
		}
		m_iPupPetState = PUPPET_STATE_NONE;
		m_iPuppetId = 0;
		m_iCandPupPetId = 0;
		SetUseGroundNormal(false);
		if(m_pPlayerModel)
			m_pPlayerModel->SetAffectedByParent(false);
		CalcPlayerAABB();
		SetPos(GetPos());
		if(IsHostPlayer())
		{
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			if(pHost)
			{
				CDR_INFO*	pCDRInfo = pHost->GetCDRInfo();
				pCDRInfo->vExtent = m_aabb.Extents;
			}
		}
		else
		{
			CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
			if(pPlayer)
			{
				OtherPlayer_Move_Info* pCdr = pPlayer->GetOtherPlayerMoveInfo();
				pCdr->vExts = m_aabb.Extents;
			}
		}
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
	}
}

void CECPlayer::PupPetShow()
{
	bool bStand = true;
	if(IsHostPlayer())
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if(pHost->GetHPWorkMan()->GetCurWorkID() != CECHPWork::WORK_STAND)
			bStand = false;
	}
	else
	{
		CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
		if(pPlayer && pPlayer->GetCurWork() != CECElsePlayer::WORK_STAND)
			bStand = false;
		
	}

	m_PuppetAnimCnt.Reset(false);

	if(!IsFighting() && bStand && m_iPuppetId)
	{
		if(m_iPupPetState == PUPPET_STATE_ANIM)
		{
			m_pPuppetModel->PlayActionByName("站立", 1.0f, false);
		}
		else
		{
			m_iPupPetState = PUPPET_STATE_ANIM;
			m_iCandPupPetId = m_iPuppetId;
		}
	}
}

//更新傀儡的状态
void CECPlayer::UpdatePupPetState(DWORD dwDeltaTime)
{
	if(m_iPuppetId)
	{
		bool bStand = true;
		if(IsHostPlayer())
		{
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			if(pHost->GetHPWorkMan()->GetCurWorkID() != CECHPWork::WORK_STAND)
				bStand = false;
		}
		else
		{
			CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
			if(pPlayer && pPlayer->GetCurWork() != CECElsePlayer::WORK_STAND)
				bStand = false;
		}
		
		if(IsFighting())
		{
			m_PuppetStateCount.Reset(false);
		}
		
		m_PuppetStateCount.IncCounter(dwDeltaTime);

		if(!bStand || (IsFighting() || !m_PuppetStateCount.IsFull()))
		{
			m_PuppetNormalStandCnt.SetPeriod(25000);
			m_PuppetNormalStandCnt.Reset(false);
			m_PuppetAnimCnt.SetPeriod(8000);
			m_PuppetAnimCnt.Reset(false);
		}

		if(bStand && !IsFighting() && m_PuppetStateCount.IsFull())
		{
			if(m_iPupPetState == PUPPET_STATE_NORMAL)
			{
				m_PuppetNormalStandCnt.IncCounter(dwDeltaTime);
				if(m_PuppetNormalStandCnt.IsFull())
				{
					if(m_PuppetNormalStandCnt.GetPeriod() >= 3600000)
					{
						m_PuppetNormalStandCnt.SetPeriod(25000);
						m_PuppetNormalStandCnt.Reset(false);
					}
					else
					{
						DWORD dwPeriod = m_PuppetNormalStandCnt.GetPeriod();
						m_PuppetNormalStandCnt.SetPeriod(dwPeriod * 2);
						m_PuppetNormalStandCnt.Reset(false);
					}
					PupPetShow();
				}
			}

			if(m_iPupPetState == PUPPET_STATE_ANIM)
			{
				m_PuppetAnimCnt.IncCounter(dwDeltaTime);
				if(m_PuppetAnimCnt.IsFull())
				{
					m_iPupPetState = PUPPET_STATE_NORMAL;
					m_iCandPupPetId = m_iPuppetId;
				}
			}
		}
		
		if(!bStand && m_iPupPetState == PUPPET_STATE_ANIM)
		{
			if(IsFighting())
				m_iPupPetState = PUPPET_STATE_FIGHT;
			else
				m_iPupPetState = PUPPET_STATE_NORMAL;
			m_iCandPupPetId = m_iPuppetId;
		}
		else if((IsFighting() || !m_PuppetStateCount.IsFull()) && m_iPupPetState != PUPPET_STATE_FIGHT)
		{
			m_iPupPetState = PUPPET_STATE_FIGHT;
			m_iCandPupPetId = m_iPuppetId;
		}
		else if(!IsFighting() && m_PuppetStateCount.IsFull() && m_iPupPetState != PUPPET_STATE_NORMAL && m_iPupPetState != PUPPET_STATE_ANIM)
		{
			m_iPupPetState = PUPPET_STATE_NORMAL;
			m_iCandPupPetId = m_iPuppetId;
		}
	}
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
		if(m_iChangeShape == SHAPE_HUMAN)
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
				
			if(m_pPetModel)
			{
				m_pPetModel->RemoveChildModel(_hanger_ride, false);
				A3DRELEASE(m_pPetModel);
			}

			CECModel* pTemp = m_pBackupModel;
			m_pBackupModel = m_pCacheModel;
			m_pPlayerModel = m_pCacheModel;
			if(m_pBackupModel)
				m_pBackupModel->SetPos(GetPos());
			
			m_bPlayerModelIsChangedModel = false;
			m_iChangeShape = 0;
			m_iCandChangeShape = 0;

			if(pTemp)
				pTemp->RemoveChildModel(_wing);

			if (IsLoadThreadReady())
			{

				if(pTemp && pTemp != m_pCacheModel)
				{
					QueueECModelForRelease(pTemp);
				}		
			}
			else
			{
				if(pTemp && pTemp != m_pCacheModel)
				{
					pTemp->Release();
					delete pTemp;
				}

			}
			
			m_uAttackType = m_uAttackTypeBackUp;
			m_dwHideEquipMask = m_dwHideEquipMaskBackUp;
			
			DWORD dwMask = 0; 
			dwMask |= (1 << EQUIPIVTR_FASHION_HEAD);
			dwMask |= (1 << EQUIPIVTR_FASHION_BODY);
			dwMask |= (1 << EQUIPIVTR_FASHION_LEG);
			dwMask |= (1 << EQUIPIVTR_FASHION_FOOT);
			dwMask |= (1 << EQUIPIVTR_WING);
			QueueLoadEquips(m_aEquips, dwMask, true); //强制更新时装
			
			CalcPlayerAABB();
			SetPos(GetPos());

			//更新可能改变的装备 装备和时装分开更新
			if(IsHostPlayer())
			{
				CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
				if(pHost)
				{
					pHost->UpdateEquipSkins(true);
					CDR_INFO*	pCDRInfo = pHost->GetCDRInfo();
					pCDRInfo->vExtent = m_aabb.Extents;
				}
			}
			else
			{
				CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
				if(pPlayer)
				{
					pPlayer->LoadPlayerEquipments();
					OtherPlayer_Move_Info* pCdr = pPlayer->GetOtherPlayerMoveInfo();
					pCdr->vExts = m_aabb.Extents;
				}
			}

			m_bFashionMode = m_bFashionModeBackUp;
			return;
		}
		else
		{
			if(m_pChangedModel && m_iChangeShape == SHAPE_QILIN)
				m_pChangedModel->RemoveChildModel(_hanger_ride, false);
			
			A3DRELEASE(m_pChangedModel);

			if (m_pPlayerModel && bPlayerGfx)
				m_pPlayerModel->PlayGfx(GetTransformGfxPath(m_iChangeShape), NULL);

			m_iChangeShape = 0;
			m_iCandChangeShape = 0;
		}
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
				m_strReplacedName = pMonster->name;
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

		if (m_TransfigureInfo.bReplaceName)
			m_pPateName->SetText(m_strName, false);

		m_TransfigureInfo.bReplaceName  = false;
		m_TransfigureInfo.bIsFly		= false;
		m_TransfigureInfo.action_type	= 0;
	}
}

void CECPlayer::NewTransfigure( int id, bool bPlayGfx )
{
	m_CandTransfigure.id = id;
	m_TransfigureInfo.id = id;
	m_TransfigureInfo.bCollisionRaid = true;
	
	if ( !id )
	{
		A3DRELEASE(m_pTransfiguredModel);
		SetUseGroundNormal(false);
		//	Re-calculate player's AABB
		CalcPlayerAABB();
		SetPos(GetPos());

		if (m_pChangedModel && bPlayGfx)
		{
			m_pChangedModel->PlayGfx(res_GFXFile(RES_GFX_TRANSFIGURE), NULL);
		}
		else if(m_pPlayerModel && bPlayGfx)
		{
			m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_TRANSFIGURE), NULL);
		}
	}
	else
	{
		DATA_TYPE dt;
		int idEssence = g_pGame->GetElementDataMan()->get_first_data_id(ID_SPACE_CONFIG, dt);
		const int MAX_TRANSFIGURE_INFO_SIZE = 20;

		while (idEssence)
		{
			if (dt == DT_COLLISION_RAID_TRANSFIGURE_CONFIG)
			{
				const COLLISION_RAID_TRANSFIGURE_CONFIG* pData = (const COLLISION_RAID_TRANSFIGURE_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr(idEssence, ID_SPACE_CONFIG, dt);
				for ( int i = 0; i < MAX_TRANSFIGURE_INFO_SIZE; ++i )
				{
					if ( pData->transfigure_info_list[i].transfigure_id == (unsigned int)id )
					{
						//-----------------------------------------------------
						//设置player的质量，特殊代码!!
						SetMass( pData->transfigure_info_list[i].mass );
						//-----------------------------------------------------
						break;
					}
				}
				break;
			}
			idEssence = g_pGame->GetElementDataMan()->get_next_data_id(ID_SPACE_CONFIG, dt);
		}
	}
}

void CECPlayer::AttachQilinBuddy(int iBuddy)
{
	if(m_bCandHangerOn)
		return;

	if(!m_pBackupModel || m_TransfigureInfo.id || m_RidingPet.id || m_iChangeShape != SHAPE_QILIN || !m_pChangedModel)
		return;

	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(iBuddy);
	if(!pBuddy || pBuddy->IsShapeChanged() || pBuddy->IsTransfigured() || pBuddy->IsRidingOnPet())
		return;

	CECModel* pBuddyModel = pBuddy->GetPlayerModel();
	if(!pBuddyModel)
		return;

	m_pChangedModel->AddChildModel(_hanger_ride, false, _hh_ride, pBuddyModel, _cc_ride);

	A3DSkinModelHanger* pHanger = m_pChangedModel->GetA3DSkinModel()->GetSkinModelHanger(_hanger_ride);
	if (pHanger) pHanger->SetOffsetMat(a3d_Translate(0, 0, 0));

	m_AttachMode = enumAttachRideOnQilin;
	m_bHangerOn = false;
	m_iBuddyId = pBuddy->m_PlayerInfo.cid;
	pBuddy->m_AttachMode = enumAttachRideOnQilin;
	pBuddy->m_bHangerOn = true;
	pBuddy->m_iBuddyId = m_PlayerInfo.cid;

	m_idCandQilinBuddy = 0;
	m_bCandHangerOn = false;
	pBuddy->m_idCandQilinBuddy = 0;
	pBuddy->m_bCandHangerOn = false;
	pBuddy->SetPos(GetPos());
	PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, true);
}

void CECPlayer::DetachQilinBuddy(CECPlayer* pBuddy /* = NULL */)
{
	if(m_AttachMode == enumAttachNone || m_AttachMode != enumAttachRideOnQilin)
		return;

	if(m_bHangerOn)
	{
		if(!pBuddy)
			pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if(pBuddy)
			pBuddy->DetachQilinBuddy(this);
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
		if(!pBuddy)
			pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if(m_pChangedModel)
			m_pChangedModel->RemoveChildModel(_hanger_ride, false);

		m_AttachMode = enumAttachNone;
		m_bHangerOn = false;
		m_iBuddyId = 0;

		if(pBuddy)
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

void CECPlayer::ReAttachQilin()
{
	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_idQilinConnectBuddy);
	if(pBuddy)
	{
		pBuddy->m_idCandQilinBuddy = pBuddy->m_idQilinConnectBuddy;
		pBuddy->m_bCandHangerOn = pBuddy->m_bQilinConnectHangerOn;
	}
	m_idCandQilinBuddy = m_idQilinConnectBuddy;
	m_bCandHangerOn = m_bQilinConnectHangerOn;
}

void CECPlayer::AddQilinLinkGfx()
{
	if(m_bQilinConnectHangerOn)
		return;

	if(!m_pBackupModel || m_TransfigureInfo.id || m_RidingPet.id || m_iChangeShape != SHAPE_QILIN || !m_pChangedModel)
		return;

	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_idQilinConnectBuddy);
	if(!pBuddy || pBuddy->IsShapeChanged() || pBuddy->IsTransfigured() || pBuddy->IsRidingOnPet())
		return;

	CECModel* pBuddyModel = pBuddy->GetPlayerModel();
	if(!pBuddyModel)
		return;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if(pHost)
	{
		pHost->AddPullGfxInfo(m_PlayerInfo.cid, m_idQilinConnectBuddy, 2);
		m_bAddQilinLinkGfx = false;
	}

}

void CECPlayer::AttachBathBuddy(int iBuddy)
{
	if(m_bCandHangerOn)
		return;
	if (!m_pBackupModel || m_TransfigureInfo.id || m_RidingPet.id || (m_iChangeShape != 0 && m_iChangeShape != SHAPE_HUMAN))
		 return;
	CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(iBuddy);
	
	if (!pBuddy || pBuddy->GetGender() == GetGender() || (pBuddy->IsShapeChanged() && pBuddy->GetShapeChangedId() != SHAPE_HUMAN) || pBuddy->IsTransfigured() || pBuddy->IsRidingOnPet())
		return;

	CECModel* pBuddyModel = pBuddy->GetPlayerModel();

	if (!pBuddyModel)
		return;

	if (!m_pBackupModel->AddChildModel(_hanger_bath, false, _hh_fly[2], pBuddyModel, _cc_ride))
		return;
	
	m_AttachMode = enumAttachBathPlayer;
	m_bHangerOn = false;
	m_iBuddyId = pBuddy->m_PlayerInfo.cid;
	pBuddy->m_AttachMode = enumAttachBathPlayer;
	pBuddy->m_bHangerOn = true;
	pBuddy->m_iBuddyId = m_PlayerInfo.cid;


   	m_idCandBathBuddy = 0;
	m_bCandHangerOn = false;
	pBuddy->m_idCandBathBuddy = 0;
	pBuddy->m_bCandHangerOn = false;
	pBuddy->SetPos(GetPos());

	DoEmote(ROLEEXP_BATH);

}
void CECPlayer::DetachBathBuddy(CECPlayer* pBuddy)
{
	if (m_bHangerOn)
	{
		if (!pBuddy)
			pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
		
		if (pBuddy)
			pBuddy->DetachBathBuddy(this);
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
	
     
		if(m_pBackupModel)
			m_pBackupModel->RemoveChildModel(_hanger_bath, false);
		
		//如果该成员当时处于太昊归神/脱灵技能状态下，那么解除绑定后需要恢复其模型缩放比例Added 2011-09-27.
		if ( pBuddy && !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
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
void CECPlayer::AttachBuddy(int iBuddy,bool isBath /*= false*/)
{
	 if(isBath)
     {
		 AttachBathBuddy(iBuddy);
		 return;
	 }

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
	else if (IsShapeChanged() && m_iChangeShape != SHAPE_HUMAN)
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
	if (m_AttachMode == enumAttachNone || m_AttachMode == enumAttachRideOnQilin)
		return;
	if(m_AttachMode == enumAttachBathPlayer)
	{
		DetachBathBuddy(pBuddy);
		return;
	}

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
		else if (m_AttachMode == enumAttachActiveEmote)
		{
			if (!m_bHangerOn)
				m_pBackupModel->RemoveChildModel(_hanger_emote, false);
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

	if (!pBuddy || (pBuddy->IsShapeChanged() && pBuddy->GetShapeChangedId() != SHAPE_HUMAN) || pBuddy->IsTransfigured())
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

void CECPlayer::AttachInteractorModel()
{
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_InteractorInfo.id, 0);
	if (!pInteractor || m_InteractorInfo.pos >= 6)
		return;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(pInteractor->GetDBEssence()->hook[m_InteractorInfo.pos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return;

	if (!m_bInteractiveSubject)
		return;

	m_pInteractorModel = pInteractor->GetNPCModel();

	const A3DVECTOR3 vCurPos = GetPos();

	//	Calculate aabb.
	m_aabb.Extents.x = max(m_aabb.Extents.x, m_pInteractorModel->GetCHAABB().Extents.x);
	m_aabb.Extents.y = max(m_aabb.Extents.y, m_pInteractorModel->GetCHAABB().Extents.y);
	m_aabb.Extents.z = max(m_aabb.Extents.z, m_pInteractorModel->GetCHAABB().Extents.z);

	m_aabb.Center = vCurPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();

	//	Set interactor's pos and dir.
	SetUseGroundNormal(true);
	m_pInteractorModel->SetAffectedByParent(false);
	m_pInteractorModel->SetPos(vCurPos);
	m_pInteractorModel->SetDirAndUp(GetDir(), GetUp());

	pInteractor->SetPos(vCurPos);

	//	Attach model.
	if (m_bInteractiveRenderSubject)
	{
		m_pBackupModel->AddChildModel(_hanger_interactor, false, WC2AS(pHookData->character_hook_name), m_pInteractorModel, WC2AS(pHookData->object_hook_name));
		m_pBackupModel->GetA3DSkinModel()->Update(0);
	}
	else
	{
		m_pInteractorModel->AddChildModel(_hanger_interactor, false, WC2AS(pHookData->object_hook_name), m_pBackupModel, WC2AS(pHookData->character_hook_name));
		m_pInteractorModel->GetA3DSkinModel()->Update(0);
	}
}

void CECPlayer::DetachInteractorModel()
{
	if (m_InteractorInfo.id == 0)
		return;

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_InteractorInfo.id, 0);

	//	Re-calculate player's AABB
	CalcPlayerAABB();
	SetUseGroundNormal(IsShapeChanged());

	SetPos(GetPos());
	SetDirAndUp(GetDir(), GetUp());

	if (!m_bInteractiveSubject) 
		return;

	if (m_bInteractiveRenderSubject)
	{
		if (m_pBackupModel)
			m_pBackupModel->RemoveChildModel(_hanger_interactor, false);
	}
	else
	{
		if (m_pInteractorModel)
			m_pInteractorModel->RemoveChildModel(_hanger_interactor, false);
	}

	m_pInteractorModel = NULL;
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
	
	bool bLoadPet = (m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112  && m_iChangeShape != SHAPE_HUMAN) ? false : true;
	if (m_RidingPet.id && bLoadPet)
		szPetPath = GetRidingPetFileName(m_RidingPet.id, m_RidingPet.iLevel);

	if (m_iChangeShape)
		szChangedPath = GetChangedModelFileName(m_iChangeShape, m_iGender, m_BasicProps.iProfession);

	if (bLoadAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;

		if (!LoadPlayerModel(GetCharacterID(), m_iPhysique, m_BasicProps.iProfession, m_BasicProps.iLevel, m_iEarid, m_iTailid, m_iFaceid, m_iHairid, 
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

		if (!LoadPlayerEquips(m_PlayerInfo.cid, m_iPhysique, m_BasicProps.iProfession, m_iDefaultSkin, m_iGender, dwMask, pEquips, Ret))
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

bool CECPlayer::QueueLoadPuppetModel(const char* szPath, bool bAtOnce)
{
	if(bAtOnce || !IsLoadThreadReady())
	{
		EC_PLAYERLOADRESULT Ret;
		memset(&Ret, 0, sizeof(Ret));

		if(!LoadPuppetModel(szPath, Ret) || !Ret.pPuppetModel)
			return false;

		if(!SetPuppetModelLoadResult(Ret.pPuppetModel))
		{
			A3DRELEASE(Ret.pPuppetModel);
			return false;
		}
	}
	else
	{
		QueueECModelForLoad(MTL_ECM_PUPPET, m_PlayerInfo.cid, m_dwBornStamp, GetPos(), szPath, NULL);
	}

	return true;
}

void CECPlayer::RideOnPet(int id, int iLev, int type)
{
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

	if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112 && (m_iChangeShape != SHAPE_HUMAN || m_pBackupModel == m_pCacheModel))   //人马形态下骑乘 没有表现
		return;
	
	m_CandPet.id		= id;
	m_CandPet.iLevel	= iLev;
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
		SetUseGroundNormal(IsShapeChanged() && m_iChangeShape != SHAPE_HUMAN);
		SetPos(GetPos());
		
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

		if( m_pPlayerModel )
			m_pPlayerModel->PlayGfx("程序联入\\下骑乘.gfx", NULL);
	}
	else
	{		
		bool isBath = m_AttachMode == enumAttachBathPlayer;
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
		SetUseGroundNormal(IsShapeChanged() && m_iChangeShape != SHAPE_HUMAN);
		SetPos(GetPos());

		if (iBuddyId)
			AttachBuddy(iBuddyId,isBath);
		else
			PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

		if( m_pPlayerModel )
			m_pPlayerModel->PlayGfx("程序联入\\下骑乘.gfx", NULL);
	}
}

bool CECPlayer::StartInteract(int iObj, int iPos)
{
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(iObj, 0);
	ASSERT(pInteractor);
	if (!pInteractor || iPos >= 6)
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(pInteractor->GetDBEssence()->hook[iPos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

//	AttachInteractor(iObj, iPos);

	if (m_InteractorInfo.id != iObj || iPos != m_InteractorInfo.pos)
	{
		// Cancel old interaction.
		CECInteractor* pOldInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_InteractorInfo.id, 0);
		if (pOldInteractor)
		{
			CancelInteract(m_InteractorInfo.id);
			pOldInteractor->CancelInteract(m_PlayerInfo.cid);
		}

		m_bInteractiveSubject = pInteractor->GetDBEssence()->main_body == 1;
		m_bInteractiveRenderSubject = pInteractor->GetDBEssence()->client_main_body == 1;

		m_InteractorInfo.id		= iObj;
		m_InteractorInfo.pos	= iPos;
		m_pInteractorModel		= NULL;
	}

	m_InteractorInfo.state = 0;
	m_InteractorInfo.attachTime = pHookData->prepare_time;
	m_InteractorInfo.detachTime = -1;
	m_bAttachedInteractor = false;

	if (!m_bInteractiveSubject)
		return true;

	// Play actions.
	m_sPlayerInteractiveAction = WC2AS(pHookData->start_character_action);
	if (!m_sPlayerInteractiveAction.IsEmpty() && m_pBackupModel)
		m_pBackupModel->PlayActionByName(ACT_CHANNEL_BODY, m_sPlayerInteractiveAction, 1.0f, false );

	m_sObjInteractiveAction = WC2AS(pHookData->start_object_action);
	if (!m_sObjInteractiveAction.IsEmpty() && m_pInteractorModel)
		m_pInteractorModel->PlayActionByName(m_sObjInteractiveAction, 1.0f, false);

	return true;
}

bool CECPlayer::InInteract(int iObj, int iPos )
{
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(iObj, 0);
	ASSERT(pInteractor);
	if (!pInteractor || iPos >= 6)
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(pInteractor->GetDBEssence()->hook[iPos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

//	AttachInteractor(iObj, iPos);

	if (m_InteractorInfo.id != iObj || iPos != m_InteractorInfo.pos)
	{
		// Cancel old interaction.
		CECInteractor* pOldInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_InteractorInfo.id, 0);
		if (pOldInteractor)
		{
			CancelInteract(m_InteractorInfo.id);
			pOldInteractor->CancelInteract(m_PlayerInfo.cid);
		}

		m_bInteractiveSubject = pInteractor->GetDBEssence()->main_body == 1;
		m_bInteractiveRenderSubject = pInteractor->GetDBEssence()->client_main_body == 1;

		m_InteractorInfo.id		= iObj;
		m_InteractorInfo.pos	= iPos;
		m_pInteractorModel		= NULL;
	}

	m_InteractorInfo.state	= 1;
	m_InteractorInfo.attachTime = 0;
	m_InteractorInfo.detachTime = -1;
	m_bAttachedInteractor = false;

	if (!m_bInteractiveSubject)
		return true;

	m_sPlayerInteractiveAction = WC2AS(pHookData->static_character_action);
	m_sPlayerInteractiveMoveAction = WC2AS(pHookData->move_character_action);
//	if (!m_sPlayerInteractiveAction.IsEmpty())
//		m_pBackupModel->PlayActionByName(ACT_CHANNEL_BODY, m_sPlayerInteractiveAction, 1.0f, false );

	m_sObjInteractiveAction = WC2AS(pHookData->static_object_action);
	m_sObjInteractiveMoveAction = WC2AS(pHookData->move_object_action);
//	if (!m_sObjInteractiveAction.IsEmpty())
//		m_pInteractorModel->PlayActionByName(m_sObjInteractiveAction, 1.0f, false);

	return true;
}

bool CECPlayer::EndInteract(int iObj, int iPos)
{
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(iObj, 0);
	ASSERT(pInteractor);
	if (!pInteractor || iPos >= 6)
		return false;

	DATA_TYPE DataType;
	INTERACTION_OBJECT_HOOK_CONFIG* pHookData = (INTERACTION_OBJECT_HOOK_CONFIG*) g_pGame->GetElementDataMan()->get_data_ptr(pInteractor->GetDBEssence()->hook[iPos], ID_SPACE_CONFIG, DataType);
	if (DataType != DT_INTERACTION_OBJECT_HOOK_CONFIG)
		return false;

//	AttachInteractor(iObj, iPos);

	if (m_InteractorInfo.id != iObj || iPos != m_InteractorInfo.pos)
	{
		// Cancel old interaction.
		CECInteractor* pOldInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_InteractorInfo.id, 0);
		if (pOldInteractor)
		{
			CancelInteract(m_InteractorInfo.id);
			pOldInteractor->CancelInteract(m_PlayerInfo.cid);
		}

		m_bInteractiveSubject = pInteractor->GetDBEssence()->main_body == 1;
		m_bInteractiveRenderSubject = pInteractor->GetDBEssence()->client_main_body == 1;

		m_InteractorInfo.id		= iObj;
		m_InteractorInfo.pos	= iPos;
		m_pInteractorModel		= NULL;
	}

	m_InteractorInfo.state	= 2;
	m_InteractorInfo.attachTime = 0;
	//	test unit 00.
	m_InteractorInfo.detachTime = 500;
	//	test unit 00 end.
	m_bAttachedInteractor = false;

	if (!m_bInteractiveSubject)
		return true;

	m_sPlayerInteractiveAction = WC2AS(pHookData->end_character_action);
	if (!m_sPlayerInteractiveAction.IsEmpty() && m_pBackupModel)
		m_pBackupModel->PlayActionByName(ACT_CHANNEL_BODY, m_sPlayerInteractiveAction, 1.0f, false );

	m_sObjInteractiveAction = WC2AS(pHookData->end_object_action);
	if (!m_sObjInteractiveAction.IsEmpty() && m_pInteractorModel)
		m_pInteractorModel->PlayActionByName(m_sObjInteractiveAction, 1.0f, false);

	return true;
}

bool CECPlayer::FinishInteract(int iObj, int iPos)
{
	DetachInteractorModel();

	m_InteractorInfo.id = 0;
	m_InteractorInfo.pos = 0;
	m_InteractorInfo.state = 0;
	m_bAttachedInteractor = false;

	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

	return true;
}

bool CECPlayer::CancelInteract(int iObj)
{
	DetachInteractorModel();

	m_InteractorInfo.id = 0;
	m_InteractorInfo.pos = 0;
	m_InteractorInfo.state = 0;
	m_bAttachedInteractor = false;

	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

	return true;
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
			bool bPhysiqueMatch = true;
			if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112 && i >= SKIN_FASHION_UPPER_BODY_INDEX && i <= SKIN_FASHION_HEADWEAR_INDEX)
			{
				if(dwMask & Result.dwTetrapodFashionMask)
				{
					if(m_pBackupModel != m_pCacheModel)
					{
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][0]);
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][1]);
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][2]);
						continue;
					}
					else
						m_dwTetrapodFashionMask |= (1 << i);
				}
				else
				{
					if(m_pBackupModel == m_pCacheModel)
					{
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][0]);
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][1]);
						g_pGame->ReleaseA3DSkin(Result.aSkins[i][2]);
						continue;
					}
					else
					{
						m_dwTetrapodFashionMask &= ~(1 << i);
						bPhysiqueMatch = false;
					}
				}
			}
						
			g_pGame->ReleaseA3DSkin(m_aSkins[i][0]);
			g_pGame->ReleaseA3DSkin(m_aSkins[i][1]);
			g_pGame->ReleaseA3DSkin(m_aSkins[i][2]);

			m_aSkins[i][0] = Result.aSkins[i][0];
			m_aSkins[i][1] = Result.aSkins[i][1];
			m_aSkins[i][2] = Result.aSkins[i][2];

			const int nPortraitIndex = _portrait_skin_map[i];

			if (nPortraitIndex >= 0 && bPhysiqueMatch)
			{
				A3DSkin* pSkin = m_pPortraitModel->GetA3DSkin(nPortraitIndex);
				g_pGame->ReleaseA3DSkin(pSkin);
				m_pPortraitModel->ReplaceSkin(nPortraitIndex, NULL, false);

				if (m_aSkins[i][0])
				{
					A3DSkin* pNewSkin = g_pGame->LoadA3DSkin(m_aSkins[i][0]->GetFileName(), true);
					if(i == SKIN_FASHION_HEADWEAR_INDEX)
					{
						int nIndex = (Result.aEuips[EQUIPIVTR_FASHION_HEAD] & 0x00ff0000) >> 16;
						if(nIndex > 70)
						{
							nIndex = 0;
						}
						A3DCOLOR	color = l_fashionColor[nIndex];	

						for(int idTex=0; idTex<pNewSkin->GetTextureNum(); idTex ++)
						{
							A3DTexture * pTex = pNewSkin->GetTexture(idTex);
							A3DEffect* pEffect = dynamic_cast<A3DEffect*>(pTex);
							if(pEffect)
							{
								A3DCOLORVALUE cv(color);
								A3DVECTOR4 v4(cv.r, cv.g, cv.b, cv.a);
								pEffect->GetHLSL()->SetValue4f("g_colHair", &v4);
							}
						}
					}
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
		m_pBackupModel->RemoveChildModel(_new_back_weapon_hanger);	//Added 2012-08-02.

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
		if (Result.pBackWeapon && !Result.pNewBackWeapon)	//背部武器 Added 2011-08-09.
		{
			m_pBackupModel->AddChildModel(
				_back_weapon_hanger,
				false,
				"HH_back",	//新挂点名字
				Result.pBackWeapon, //背部武器模型指针
				"CC_back");	//新的子挂点名字
		}
		else if ( Result.pBackWeapon && Result.pNewBackWeapon )	//背部两个武器都加载成功 Added 2012-07-26.
		{
			m_pBackupModel->AddChildModel(
				_back_weapon_hanger,
				false,
				"HH_qi2",			//新挂点名字
				Result.pBackWeapon,	//背部武器模型指针
				"CC_weapon");		//新的子挂点名字

			m_pBackupModel->AddChildModel(
				_new_back_weapon_hanger,
				false,
				"HH_qi3",				//新挂点名字
				Result.pNewBackWeapon,	//新的背部武器模型指针
				"CC_weapon");			//新的子挂点名字
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

//		OnWeaponChanged();
	}

	if(IsSkillFlying())
		ShowWing(false);
	else
		ShowWing(IsFlying());

	if (Result.bTalismanChanged)
		UpdateTalismanSprite(Result.pTalisman);

	if (Result.bTuneChanged)
	{
//		UpdateTuneSprite(Result.pTune);	//Canceled 2012-06-06.
		UpdateRuneStateGFX( Result.nRuneQuality );//Added 2012-06-06.
	}

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

	//-------------------------------------------------------------------------
	//!!!!!!无论如何，加载完毕后都需要清除掉m_dwForceChangeMask
	SetForceChangeMask(0);
	//-------------------------------------------------------------------------
	m_bLoadFlag = true;
	return true;
}

bool CECPlayer::SetMirrorEquipsLoadedResult(EC_PLAYERLOADRESULT& Ret, bool bUpdateAtOnce)
{
	for(int i=0;i<6;i++)
	{
		if(m_MirrorModels[i])
		{
			if(!m_MirrorModels[i]->SetMirrorEquips(Ret.EquipResult[i], bUpdateAtOnce))
				return false;	
		}
	}

	return true;
}
bool CECPlayer::SetMirrorEquips(EquipsLoadResult& Result, bool bUpdateAtOnce)
{
	if (!m_pBackupModel )
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
		m_pBackupModel->RemoveChildModel(_new_back_weapon_hanger);	//Added 2012-08-02.

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
		if (Result.pBackWeapon && !Result.pNewBackWeapon)	//背部武器 Added 2011-08-09.
		{
			m_pBackupModel->AddChildModel(
				_back_weapon_hanger,
				false,
				"HH_back",	//新挂点名字
				Result.pBackWeapon, //背部武器模型指针
				"CC_back");	//新的子挂点名字
		}
		else if ( Result.pBackWeapon && Result.pNewBackWeapon )	//背部两个武器都加载成功 Added 2012-07-26.
		{
			m_pBackupModel->AddChildModel(
				_back_weapon_hanger,
				false,
				"HH_qi2",			//新挂点名字
				Result.pBackWeapon,	//背部武器模型指针
				"CC_weapon");		//新的子挂点名字

			m_pBackupModel->AddChildModel(
				_new_back_weapon_hanger,
				false,
				"HH_qi3",				//新挂点名字
				Result.pNewBackWeapon,	//新的背部武器模型指针
				"CC_weapon");			//新的子挂点名字
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

//		OnWeaponChanged();
	}

	if(IsSkillFlying())
		ShowWing(false);
	else
		ShowWing(IsFlying());

	if (Result.bTalismanChanged)
		UpdateTalismanSprite(Result.pTalisman);

	if (Result.bTuneChanged)
	{
//		UpdateTuneSprite(Result.pTune);	//Canceled 2012-06-06.
		UpdateRuneStateGFX( Result.nRuneQuality );//Added 2012-06-06.
	}

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

	//-------------------------------------------------------------------------
	//!!!!!!无论如何，加载完毕后都需要清除掉m_dwForceChangeMask
	SetForceChangeMask(0);
	//-------------------------------------------------------------------------
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

	if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112 && (m_iChangeShape != SHAPE_HUMAN || m_pBackupModel == m_pCacheModel))
	{
		A3DRELEASE(pPetModel);
		return true;
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
	bool isBath = m_AttachMode == enumAttachBathPlayer;
	if (m_AttachMode != enumAttachNone)
		DetachBuddy();

	m_pPetModel->AddChildModel(_hanger_ride, false, _hh_ride, m_pBackupModel, _cc_ride);
	m_pPetModel->GetA3DSkinModel()->Update(0);

	if (iBuddyId)
		AttachBuddy(iBuddyId,isBath);
	else
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

	return true;
}

bool CECPlayer::SetChangedModelLoadResult(CECModel* pChangedModel, bool bMultiThreadLoaded)
{
	if (!m_iChangeShape || !m_pBackupModel)
		return false;

	A3DRELEASE(m_pChangedModel);

	//四足体型变形为人时，执行Player的逻辑
	if(m_iChangeShape == SHAPE_HUMAN)
	{
		
		//正常情况，四足模型不会产生绑定关系， 为安全和逻辑清晰起见，变为人形时还是先解除四组模型的绑定关系
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

		if(m_pPetModel && !bMultiThreadLoaded)
		{
			m_pPetModel->RemoveChildModel(_hanger_ride, false);
			A3DRELEASE(m_pPetModel);
		}		
		m_pBackupModel = pChangedModel;
		m_pPlayerModel = pChangedModel;
		m_pPlayerModel->SetPos(GetPos());
		m_pPlayerModel->SetDirAndUp(GetDir(), GetUp());
		m_pPlayerModel->SetAffectedByParent(false);
		m_pPlayerModel->PlayGfx(GetTransformGfxPath(m_iChangeShape), NULL);
		m_bPlayerModelIsChangedModel = true;
		m_uAttackTypeBackUp = m_uAttackType;
		m_uAttackType = 0;	 //人形姿态下，只能空手
		DWORD dwMask = 0; 
		dwMask |= (1 << EQUIPIVTR_FASHION_HEAD);
		dwMask |= (1 << EQUIPIVTR_FASHION_BODY);
		dwMask |= (1 << EQUIPIVTR_FASHION_LEG);
		dwMask |= (1 << EQUIPIVTR_FASHION_FOOT);
		dwMask |= (1 << EQUIPIVTR_WING);
		m_dwHideEquipMaskBackUp = m_dwHideEquipMask;
		m_bFashionModeBackUp	= m_bFashionMode;
		m_bFashionMode			= true;   //人形姿态下，只能穿时装
		m_dwHideEquipMask = 0;

		QueueLoadEquips(m_aEquips, dwMask, false);

		if(m_pBackupModel && m_RidingPet.id && !bMultiThreadLoaded)
		{
			const char* szPetPath = GetRidingPetFileName(m_RidingPet.id, m_RidingPet.iLevel);
			QueueLoadPetModel(szPetPath, false);
		}
		PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, true);
		return true;
	}


	m_pChangedModel = pChangedModel;
	m_pChangedModel->SetPos(GetPos());
	m_pChangedModel->SetDirAndUp(GetDir(), GetUp());
	PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
	m_pChangedModel->PlayGfx(GetTransformGfxPath(m_iChangeShape), NULL);

	return true;
}

bool CECPlayer::SetMirrorModelLoadResult(EC_PLAYERLOADRESULT& Ret)
{
	for(int index = 0;index < 6;index++)
	{
		if(m_MirrorModels[index])
		{
			m_MirrorModels[index]->SetPlayerModel(Ret.pMirrorModel[index]);
		
			m_MirrorModels[index]->SetPos(GetPos());
			m_MirrorModels[index]->SetDirAndUp(GetDir(), GetUp());
			m_MirrorModels[index]->PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
		}
		else
			return false;
	}


	// 加载镜像模型的装备
	SetMirrorEquipsLoadedResult(Ret, true);
/*
	memcpy(m_aEquipsTmp, m_aEquips, SIZE_EQUIPIVTR*sizeof(int));
	m_aEquipsTmp[EQUIPIVTR_MAIN_TALISMAN] = 0;
	QueueECModelForLoad(
		MTL_ECM_MIRROR_PLAYER_EQUIP,
		m_PlayerInfo.cid,
		m_dwBornStamp,
		GetPos(),
		NULL,
		NULL,
		m_iPhysique,
		m_iGender,
		EQUIP_MASK_ALL,
		m_aEquipsTmp,
		m_BasicProps.iProfession,
		0,
		0,
		0,
		m_iDefaultSkin);
*/

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
	if(m_TransfigureInfo.bReplaceName)
		m_pPateName->SetText(m_strReplacedName,false);
	return true;
}

bool CECPlayer::SetAircraftModelLoadResult(CECModel* pModel)
{
	return true;
}

bool CECPlayer::SetPuppetModelLoadResult(CECModel* pPuppetModel)
{
	if(!m_iPuppetId || !m_pBackupModel || m_iCandPupPetId ||!pPuppetModel)
		return false;

	if(m_pPuppetModel)
	{
		m_pPuppetModel->RemoveChildModel(_hanger_puppet,false);
		A3DRELEASE(m_pPuppetModel);
	}
	 
	const A3DVECTOR3 vCurPos = GetPos();
	A3DAABB aabb;
	if(pPuppetModel->HasCHAABB())
		aabb = pPuppetModel->GetCHAABB();	
	else
		aabb = pPuppetModel->GetModelAABB();
	
	CalcPlayerAABB();
	m_aabb.Extents.y = max(m_aabb.Extents.y, aabb.Extents.y);	
	m_aabb.Extents.y *= 1.5f;
	m_aabb.Center = vCurPos + A3DVECTOR3(0.0f, m_aabb.Extents.y, 0.0f);
	m_aabb.CompleteMinsMaxs();

	if(IsHostPlayer())
	{
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if(pHost)
		{
			CDR_INFO*	pCDRInfo = pHost->GetCDRInfo();
			pCDRInfo->vExtent = m_aabb.Extents;
		}
	}
	else
	{
		CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(m_PlayerInfo.cid);
		if(pPlayer)
		{
			OtherPlayer_Move_Info* pCdr = pPlayer->GetOtherPlayerMoveInfo();
			pCdr->vExts = m_aabb.Extents;
		}
	}

	m_pPuppetModel = pPuppetModel;;
	SetUseGroundNormal(false);
	//切换武器位置 模型作为子物体时，武器显示脚本好像失效
	if(m_iPupPetState == PUPPET_STATE_FIGHT)
		m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY, "战斗站立_笔", 1.0f, false );
	else
		m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY, "站立_笔", 1.0f, false );

	m_pPlayerModel->Tick(1);
	m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY, true);
	m_pPlayerModel->SetAffectedByParent(true);
	m_pPuppetModel->SetAffectedByParent(false);
	m_pPuppetModel->SetPos(GetPos());
	m_pPuppetModel->SetDirAndUp(GetDir(), GetUp());
	const char* CCHook;
	if(m_iPupPetState == PUPPET_STATE_NORMAL)
		CCHook = _cc_ride;
	else
		CCHook = _cc_kuilei;
	m_pPuppetModel->AddChildModel(_hanger_puppet, false, _hh_kuilei, m_pBackupModel, CCHook);
	if(m_iPupPetState == PUPPET_STATE_ANIM)
	{
		m_pPuppetModel->PlayActionByName("站立", 1.0f, false);
	}
	else
		PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
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
	m_pCacheModel	= m_pPlayerModel;
	m_pBackupModel->SetAffectedByParent(false);
	m_pBackupModel->GetA3DSkinModel()->SetInheritTransFlag(false);

	if (m_pDummyModel)
		m_pDummyModel->SetAffectedByParent(false);

	if (Ret.pTransfiguredModel && !SetTransfigureModelLoadResult(Ret.pTransfiguredModel))
		A3DRELEASE(Ret.pTransfiguredModel);

	int i;
	for (i = 0; i < SIZE_EQUIPIVTR; i++)
	{
		if (m_aHideEquips[i] > 0)
			m_pBackupModel->OnScriptChangeEquip(m_aHideEquips[i], 1, m_bFashionMode, GetEquipPathId(m_aHideEquips[i], m_iGender, m_iPhysique));
	}

	SetEquipsLoadedResult(Ret.EquipResult[0], false);
	
	if (Ret.pChangedModel && !SetChangedModelLoadResult(Ret.pChangedModel, Ret.pPetModel)) 
		A3DRELEASE(Ret.pChangedModel);

	if (Ret.pPetModel && !SetPetLoadResult(Ret.pPetModel))
		A3DRELEASE(Ret.pPetModel);

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
	SetFlowBattle(m_iFlowBattle);

	if(m_pPlayerModel)
		m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_HIGHLEVEL), "HH_Spine", 1.35f);

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

bool CECPlayer::TraceWithBrush(BrushTraceInfo * pInfo)
{
	if ( !pInfo )
	{
		return false;
	}

/*	CECModel* pModel = ( m_pTransfiguredModel ? m_pTransfiguredModel : m_pBackupModel );
	if ( !pModel || !pModel->HasCHAABB() ) //|| !m_nBrushes
	{
		return false;
	}

	BrushTraceInfo info = *pInfo;

	A3DAABB aabb = pModel->GetCHAABB();
	aabb.Center += GetPos();
	aabb.CompleteMinsMaxs();   // it is necessary
	if ( pModel->GetA3DSkinModel() )
	{
		A3DAABB aabb2 = pModel->GetA3DSkinModel()->GetModelAABB();
		aabb.Merge(aabb2);
	}
*/
	A3DAABB aabb = GetPlayerPickAABB();

	A3DVECTOR3	vPoint, vNormal;
	float		fFraction;		//	Fraction
	if (pInfo->bIsPoint && !CLS_RayToAABB3(pInfo->vStart, pInfo->vDelta, aabb.Mins, aabb.Maxs, vPoint, &fFraction, vNormal ) )
	{
		return false;
	}
	if (!pInfo->bIsPoint && !CLS_AABBAABBOverlap(aabb.Center, aabb.Extents, pInfo->BoundAABB.Center, pInfo->BoundAABB.Extents) )
	{
		return false;
	}

	//
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
	
	UpdatePupPetState(dwDeltaTime);

	//	Incant time counter
	if (m_IncantCnt.IncCounter(dwRealTime))
		m_IncantCnt.Reset(true);
	//	Update last said words
	if (m_pPateLastWords1 && m_pPateLastWords1->GetItemNum())
	{
		if (m_strLastSayCnt.IncCounter(dwDeltaTime))
		{
			//	Clear string
			m_strLastSayCnt.Reset();
			m_pPateLastWords1->Clear();
			m_pPateLastWords2->Clear();
		}
		else
		{
			m_pPateLastWords1->Tick(dwDeltaTime);
			m_pPateLastWords2->Tick(dwDeltaTime);
		}
	}
	
	if(m_bUpdateFrameCount)
	{
		if(m_TitleFrameCount.IncCounter(dwDeltaTime))
		{
			m_TitleFrameCount.Reset();
			if(m_TitleInfo.iCurrentframe + 1 >= m_TitleInfo.iTitleFrameNum)
				m_TitleInfo.iCurrentframe = 0;
			else 
				m_TitleInfo.iCurrentframe++;
			m_pTitleDecal->ChangeSpriteIcon(m_TitleInfo.pSprite, m_TitleInfo.iCurrentframe);
		}
	}

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

	if(m_pPlayerModel)
	{
		A3DGFXEx * pGfx = m_pPlayerModel->GetGfx(res_GFXFile(RES_GFX_HIGHLEVEL), "HH_Spine");
		if(pGfx)
		{
			if(m_BasicProps.iLevel > 150 && m_nRebornCount > 0 && g_pGame->GetConfigs()->GetVideoSettings().nEffect > 1)
			{
				if(pGfx->GetState() == ST_STOP)
					pGfx->Start();
			}
			else if(pGfx->GetState() != ST_STOP)
			{
				pGfx->Stop();
			}
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

	// 轩辕灵效果，播放相应特效
	m_fSpiritsAngle += 2.0f;
	if(m_fSpiritsAngle > 360.0f)
		m_fSpiritsAngle = 0.0f;

	for(int i=0;i<3;i++)
	{
		if(m_cDarkLightSpirits[i])
		{
			if(!m_pSpiritsGFX[i] && m_cDarkLightSpirits[i] != 0)
			{
				m_pSpiritsGFX[i] = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_SPIRIT_01+(m_cDarkLightSpirits[i]-1)));
			}

			if(m_pSpiritsGFX[i])
			{
				if (m_pSpiritsGFX[i]->GetState() == ST_STOP || m_pSpiritsGFX[i]->GetAlpha() < 1.0f )
				{
					m_pSpiritsGFX[i]->SetAlpha(1.0f);
					m_pSpiritsGFX[i]->Start(true);
				}

				A3DMATRIX4 mat = GetAbsoluteTM();
				m_pSpiritsGFX[i]->SetParentTM(mat);

				float fLength = 1.5f;
				A3DVECTOR3 vSpiritPos = GetPos();
				vSpiritPos.x += fLength*cosf(DEG2RAD((i+1)* 360.0f/3.0f + m_fSpiritsAngle));
				vSpiritPos.z += fLength*sinf(DEG2RAD((i+1)* 360.0f/3.0f + m_fSpiritsAngle));

				if(m_pPlayerModel)
					vSpiritPos.y += 2.0f + 0.5f*sinf(DEG2RAD(m_fSpiritsAngle));

				m_pSpiritsGFX[i]->SetPos(vSpiritPos);
			}			
		}
		else
		{
			if(m_pSpiritsGFX[i])
			{
				m_pSpiritsGFX[i]->Stop();
				g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSpiritsGFX[i]);
				m_pSpiritsGFX[i] = NULL;
			}
		}
	}

	if( m_pSprite )
		m_pSprite->Tick(dwDeltaTime);
	
	if( m_pSpriteTune )
		m_pSpriteTune->Tick(dwDeltaTime);

	if (m_idCandBuddy)
		AttachBuddy(m_idCandBuddy);
	if (m_idCandBathBuddy)
		AttachBuddy(m_idCandBathBuddy,true);
	if(m_idCandQilinBuddy)
		AttachQilinBuddy(m_idCandQilinBuddy);

	if(m_bAddQilinLinkGfx)
	{
		AddQilinLinkGfx();
	}

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

		m_dwPoseOnPet = g_pGame->GetPoseOnPet(szPetPath);

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
		const char* szPath = GetTransfiguredModelFileName(m_CandTransfigure.id, m_TransfigureInfo.bCollisionRaid);
		QueueLoadTransfiguredModel(szPath, false);
		m_CandTransfigure.id = 0;
	}

	if(m_iCandPupPetId && m_pBackupModel)
	{
		const char* szPath = GetPuppetModelFileName(m_PlayerInfo.cid, m_iCandPupPetId);
		QueueLoadPuppetModel(szPath, false);
		m_iCandPupPetId = 0;
	}

	if(m_CandInteractorInfo.id != 0)
	{
		CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(m_CandInteractorInfo.id, 0);
		if (m_pPlayerModel != NULL && pInteractor != NULL && pInteractor->GetNPCModel() != NULL)
		{
			switch(m_CandInteractorInfo.state)
			{
			case 0:
				pInteractor->StartInteract(m_PlayerInfo.cid, m_CandInteractorInfo.pos);
				StartInteract(m_CandInteractorInfo.id, m_CandInteractorInfo.pos);
				break;

			case 1:
				pInteractor->InInteract(m_PlayerInfo.cid, m_CandInteractorInfo.pos);
				InInteract(m_CandInteractorInfo.id, m_CandInteractorInfo.pos);
				break;

			case 2:
				pInteractor->EndInteract(m_PlayerInfo.cid, m_CandInteractorInfo.pos);
				EndInteract(m_CandInteractorInfo.id, m_CandInteractorInfo.pos);
				break;

			default:
				ASSERT(false);
			}

			m_CandInteractorInfo.id = 0;
		}
	}

	if (m_InteractorInfo.id != 0)
	{
		if (m_InteractorInfo.attachTime >= 0)
		{
			if (m_InteractorInfo.attachTime < (int) dwDeltaTime)
			{
				if (m_bInteractiveSubject)
					AttachInteractorModel();

				m_bAttachedInteractor = true;
			}
			m_InteractorInfo.attachTime -= dwDeltaTime;
		}

		if (m_InteractorInfo.detachTime >= 0)
		{
			if (m_InteractorInfo.detachTime < (int) dwDeltaTime)
			{
				if (m_bInteractiveSubject)
					DetachInteractorModel();

				m_bAttachedInteractor = false;
			}
			m_InteractorInfo.detachTime -= dwDeltaTime;
		}
	}

	if(m_CandEmoteInfo.id != 0)
	{	
		CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_CandEmoteInfo.id);
		if (m_pBackupModel && pPlayer && pPlayer->GetPlayerModel())
		{
			PlayActiveEmote(m_CandEmoteInfo);
			m_CandEmoteInfo.id = 0;
			m_CandEmoteInfo.type = 0;
		}
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
		else if(dwModifier & MOD_IMMUNE_DAMAGE)
			BubbleText(BUBBLE_IMMUNE_DAMAGE, 0);
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
		else if(dwModifier & MOD_IMMUNE_DAMAGE)
			BubbleText(BUBBLE_IMMUNE_DAMAGE, 0);
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
			else if(dwModifier & MOD_IMMUNE_DAMAGE)
				BubbleText(BUBBLE_IMMUNE_DAMAGE, 0);
			else if (dwModifier & MOD_RESIST)
				BubbleText(BUBBLE_RESIST, 0);
		}
		else if (dwModifier & MOD_IMMUNE)
			BubbleText(BUBBLE_IMMUNE, 0);
		else if(dwModifier & MOD_IMMUNE_DAMAGE)
			BubbleText(BUBBLE_IMMUNE_DAMAGE, 0);
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
							DWORD dwModifier, int* piAttackTime/* NULL */, int spirit_index/* -1 */, A3DVECTOR3* pCastSkillPos/* = NULL*/)
{
	if (!IsAllResReady())
		return;

	if (m_iSpecialState & SPECIAL_STATE_FROZEN)
	{
		return;
	}

	// 某些情况下，需要让灵模型播放Attack Effect
	CECModel* pPlayerModelBak = m_pPlayerModel;
	if(spirit_index >= 0)
	{
		pPlayerModelBak = m_pPlayerModel;
		m_pPlayerModel = GetSpiritModel(spirit_index)->GetPlayerModel();
		if(!m_pPlayerModel)
			return;
	}

	int nActType = m_uAttackType;
	if( nActType < 0 ) nActType = 0;
	if( nActType > 14 ) nActType = 14;
	const char * szSuffix = m_ActionNames.GetANSIString(nActType + ACT_SUFFIX_0);

	if( !idSkill )
	{
		// melee attack
		if( !IsDead() && (dwModifier & MOD_RETORT) == 0 && m_pPlayerModel )
		{
			char szAct[100];
			szAct[0] = '\0';
			// 怀光连击的初步实验
			SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
			if (pInfo && pInfo->mask_sect & (1<<SECT_13))  // 怀光
			{
				m_iDoubleHit += 1;
				if (m_iDoubleHit == 1)
				{

				}
				else if (m_iDoubleHit == 2)	// 第二击忽略
				{
					m_iDoubleHit = 0;
					return;
				}

			}

			if (m_pTransfiguredModel)
			{
				m_pTransfiguredModel->PlayAttackAction(CECNPC::GetActionName(GetMappedAction(ACT_ATTACK_1 + (rand() % 4))), cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				m_pTransfiguredModel->QueueAction(CECNPC::GetActionName(GetMappedAction(ACT_FIGHTSTAND)), 300, ACT_ATTACK_1);
			}
			else if (m_pChangedModel)
			{
				sprintf(szAct, "%s%d", "普通攻击", 1 + (rand() % 3));
				m_pChangedModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				m_pChangedModel->QueueAction(m_ActionNames.GetANSIString(ACT_FIGHTSTAND), 300, ACT_ATTACK_1);
			}
			else if(m_pPuppetModel)
			{
				sprintf(szAct, "%s%d", "普通攻击", 1 + (rand() % 3));
				m_pPuppetModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				m_pPuppetModel->QueueAction(m_ActionNames.GetANSIString(ACT_FIGHTSTAND), 300, ACT_ATTACK_1);
			}
			else if( m_pPetModel )	// Is riding
			{
				sprintf(szAct, "%s%d_骑乘_%s", "普通攻击", 1 + (rand() % 3), szSuffix);
				m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);

				sprintf(szAct, "%s_骑乘_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
				m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f, false);
				
				// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
				sprintf(szAct, "%s_骑乘_%s", "战斗站立", szSuffix);
				m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_ATTACK_1);

				if( (rand() % 5) == 0 )
				{
					m_pPetModel->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_NPC_PETFIGHT1 + (rand() % 3)), 1.0f, false);
					m_pPetModel->QueueAction(CECNPC::GetActionName(CECNPC::ACT_GUARD));
				}
				else
					m_pPetModel->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_GUARD), 1.0f, false);
			}
			else if( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) // 玩家在飞骑上面播放动作复用坐骑的动作
			{
				sprintf(szAct, "%s%d_骑乘_%s", "普通攻击", 1 + (rand() % 3), szSuffix);
				m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);

				sprintf(szAct, "%s_骑乘_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
				m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f, false);
				
				// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
				sprintf(szAct, "%s_骑乘_%s", "战斗站立", szSuffix);
				m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_ATTACK_1);
			}
			else if (m_nFlyMode == FLYMODE_WING && IsFlying()) // 翅膀飞行状态播放对应的普攻动作和战斗站立动作
			{
				sprintf(szAct, "%s%d_翅膀_%s", "普通攻击", 1+(rand()%3), szSuffix);
				m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);

				sprintf(szAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
				m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f, false);
				
				// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
				sprintf(szAct, "%_翅膀_%s", "战斗站立", szSuffix);
				m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_ATTACK_1);
			}
			else
			{
				//-------------------------------------------------------------
				//对于轩辕（辰皇）职业，需要处理光系和暗系动作 Added 2012-09-05.
				SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
				if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)) )	
				{
					if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
					{
						sprintf(szAct, "%s%d_%s_%s", "普通攻击", 1 + (rand() % 3), szSuffix, "光");
					}
					else
					{
						sprintf(szAct, "%s%d_%s_%s", "普通攻击", 1 + (rand() % 3), szSuffix, "暗");
					}
				}
				else
				{
					sprintf(szAct, "%s%d_%s", "普通攻击", 1 + (rand() % 3), szSuffix);
				}
				//-------------------------------------------------------------
				m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, pCastSkillPos);

				// for mirror models 201208
				int i;
				for(i=0;i<6;i++)
				{
					CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
					if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
					{
						pMirrorPlayer->GetPlayerModel()->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					}
				}
				
// 				CECModel* pWeapon = m_pPlayerModel->GetChildModel(_weapon_hanger);
// 				if (pWeapon)
// 				{
// //					A3DSkinModelActionCore* pAction = m_pPlayerModel->GetA3DSkinModel()->GetAction(szAct);
// 					A3DCombinedAction* pAction =  m_pPlayerModel->GetComActByName(szAct);
// 					m_ScaleWeaponCnt.SetPeriod(pAction->GetMinComActTimeSpan());
// 					m_ScaleWeaponCnt.Reset();
// 					m_bScaleWeapon = true;	
// 					pWeapon->ScaleBoneEx(pWeapon->GetA3DSkinModel()->GetSkeleton()->GetBone(0)->GetName(), A3DVECTOR3(1.0f, 1.0f, 1.0f));
// 				}

				sprintf(szAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
				m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f, false);

				// for mirror models 201208
				for(i=0;i<6;i++)
				{
					CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
					if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
					{
						pMirrorPlayer->GetPlayerModel()->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f, false);
					}
				}
				
				// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
				//-------------------------------------------------------------
				if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)) )	
				{
					if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
					{
						sprintf(szAct, "%s_%s_%s", "战斗站立", szSuffix, "光");
					}
					else
					{
						sprintf(szAct, "%s_%s_%s", "战斗站立", szSuffix, "暗");
					}
				}
				else
				//-------------------------------------------------------------
				{
					sprintf(szAct, "%s_%s", "战斗站立", szSuffix);
				}
				m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_ATTACK_1);

				for(i=0;i<6;i++)
				{
					CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
					if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
					{
						pMirrorPlayer->GetPlayerModel()->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_ATTACK_1);
					}
				}
			} 

		}
	}
	else
	{
		if( skillLevel == 0 )
		{
			if( m_pCurSkill )
				skillLevel = m_pCurSkill->GetSkillLevel();
			else
				skillLevel = 1;
		}

		if (m_pCurSkill && m_pSprite && m_pCurSkill->IsCategory(CATEGORY_TALISMAN) )
			m_pSprite->SetNoRender(true);

		//  attack and curse skill let player be in combat state, other skill could not.
		char type = GNET::ElementSkill::GetType(idSkill);
		// begin a skill attack
		if( !IsDead() && (dwModifier & MOD_RETORT) == 0 && m_pPlayerModel )
		{
			char szAct[100];
			szAct[0] = '\0';
			if (m_pTransfiguredModel)
			{				
				int t = GNET::ElementSkill::GetExecuteTime(idSkill, skillLevel);
				int index;

				if ( m_TransfigureInfo.action_type == 0)  // 普通怪动作方案
				{
					if (t <= 1000)
						index = 0;
					else if (t <= 2000)
						index = 1;
					else if (t <= 3000)
						index = 0;
					else
						index = 1;
				}
				else if (m_TransfigureInfo.action_type == 1)
				{
					if (t <= 1000)
						index = 0;
					else if (t <= 2000)
						index = 1;
					else if (t <= 3000)
						index = 2;
					else 
						index = 3; // 3000以上
				}
				// 优先使用技能编辑器里面编辑的技能动作，其次再进行随即匹配
				if ( m_pCurSkill->IsChargeable() )	//处理蓄力技能 Added 2012-05-21.
				{
					if ( m_pCurSkill->IsChanting() )
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "起"); 
					}
					else if( m_pCurSkill->IsCharging() )
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "蓄力");
					}
					else if( m_pCurSkill->IsEmission() )
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "落");
					}
					m_pTransfiguredModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				}
				else
				{
					sprintf(szAct, "%s", GNET::ElementSkill::GetAction(idSkill, 30));
					if (szAct[0] != '0')
						m_pTransfiguredModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					else
						m_pTransfiguredModel->PlayAttackAction(CECNPC::GetActionName(CECNPC::ACT_NPC_SKILL_ATTACK1+index), cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					
					if (type == GNET::TYPE_ATTACK || type == GNET::TYPE_CURSE || IsFighting())
						m_pTransfiguredModel->QueueAction(CECNPC::GetActionName(GetMappedAction(ACT_FIGHTSTAND)), 300, ACT_ATTACK_1);
					else
						m_pTransfiguredModel->QueueAction(CECNPC::GetActionName(GetMappedAction(ACT_STAND)), 300, ACT_ATTACK_1);	
				}
			}
			else if (m_pChangedModel)
			{
				if(m_pCurSkill->IsChargeable())
				{
					if(m_pCurSkill->IsChanting())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "起"); 
					else if(m_pCurSkill->IsCharging())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "蓄力");
					else if(m_pCurSkill->IsEmission())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "落");
					m_pChangedModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				}
				else
				{				
					sprintf(szAct, "%s", GNET::ElementSkill::GetAction(idSkill, 5));
					m_pChangedModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					if (type == GNET::TYPE_ATTACK || type == GNET::TYPE_CURSE || IsFighting())
						m_pChangedModel->QueueAction("战斗站立", 300, ACT_CASTSKILL);			
					else
						m_pChangedModel->QueueAction("站立", 300, ACT_CASTSKILL);
				}
			}
			else if (m_pPuppetModel)
			{
				if(m_pCurSkill->IsChargeable())
				{
					if(m_pCurSkill->IsChanting())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "起"); 
					else if(m_pCurSkill->IsCharging())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "蓄力");
					else if(m_pCurSkill->IsEmission())
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "落");
					m_pPuppetModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
				}
				else
				{				
					sprintf(szAct, "%s", GNET::ElementSkill::GetAction(idSkill, 0)); //与策划商定
					m_pPuppetModel->PlayAttackAction(szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					if (type == GNET::TYPE_ATTACK || type == GNET::TYPE_CURSE || IsFighting())
						m_pPuppetModel->QueueAction("战斗站立", 300, ACT_CASTSKILL);			
					else
						m_pPuppetModel->QueueAction("站立", 300, ACT_CASTSKILL);
				}
			}

			else
			{
				if( m_pPetModel ) // IsRiding
					m_pPetModel->PlayActionByName(CECNPC::GetActionName(CECNPC::ACT_GUARD), 1.0f, false);
				
				if(m_pCurSkill->IsChargeable())
				{
					if(m_pCurSkill->IsChanting())
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying())) ? nActType + ACTION_SWITCH_INDEX : nActType), "起");  // 还要考虑到是否骑乘
					}
					else if(m_pCurSkill->IsCharging())
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "蓄力");
					}	
					else if(m_pCurSkill->IsEmission())
					{
						sprintf(szAct, "%s_%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || ( m_nFlyMode == FLYMODE_FEIQI && IsFlying()) ) ? nActType + ACTION_SWITCH_INDEX : nActType), "落");				
					}
					m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
					
					if( m_pSprite && m_pSprite->GetModel() )
						m_pSprite->GetModel()->PlayActionByName(szAct, 1.0f, true);

					// for mirror models 201208
					for(int i=0;i<6;i++)
					{
						CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
						if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
						{
							pMirrorPlayer->GetPlayerModel()->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, NULL);
						}
					}
				}
				else
				{	
					// 飞骑播放的技能动作复用坐骑的技能动作
					// 飞骑和坐骑复用一套动作，飞剑上和地面复用一套动作，翅膀则单独使用一套动作，动作名字是地面动作中间插入一个"_翅膀_"
					sprintf(szAct, "%s", GNET::ElementSkill::GetAction(idSkill, (m_pPetModel || (m_nFlyMode == FLYMODE_FEIQI && IsFlying()) || (m_AttachMode == enumAttachRideOnQilin && m_bHangerOn) ) ? nActType + ACTION_SWITCH_INDEX : nActType));
					if (m_nFlyMode == FLYMODE_WING && IsFlying())
					{
						AString s(szAct);
						int n = s.Find('_');
						sprintf(szAct, "%s_翅膀%s", AString(szAct, n), AString(szAct+n));
					}
					m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, szAct, cSerialId, GetPlayerInfo().cid, idTarget, pCastSkillPos);

				    //法宝传承技能动作与体型不匹配时，播放默认动作
					AString strAction(szAct);
					if(strAction.Find("_法宝_") >= 0)
					{
						int iPhysique = -1;
						if(strAction.Find("青云门_") >=0 || strAction.Find("合欢派_") >=0 || strAction.Find("鬼王宗_") >=0 || strAction.Find("天音寺_") >=0 || strAction.Find("鬼道_") >=0 ||strAction.Find("焚香谷_") >=0)
							iPhysique = PHYSIQUE_NORMAL;
						else if(strAction.Find("兽神_") >=0 || strAction.Find("太昊_") >=0)
							iPhysique = PHYSIQUE_BIG;
						else if(strAction.Find("轩辕_") >=0 || strAction.Find("天华_") >=0)
							iPhysique = PHYSIQUE_MIDDLE;
						else if(strAction.Find("狐妖_") >=0 || strAction.Find("怀光_") >=0)
							iPhysique = PHYSIQUE_SMALL;
						if(iPhysique != -1 && m_iPhysique != iPhysique) 
						{
							SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
							if(pInfo)
							{
								AString strPro;
								if((pInfo->mask_sect & (1<<SECT_2)) || (pInfo->mask_sect & (1<<SECT_1)))
									strPro = "鬼王宗";
								else if(pInfo->mask_sect & (1<<SECT_3))
									strPro = "合欢派";
								else if(pInfo->mask_sect & (1<<SECT_4))
									strPro = "青云门";
								else if(pInfo->mask_sect & (1<<SECT_5))
									strPro = "天音寺";
								else if(pInfo->mask_sect & (1<<SECT_6))
									strPro = "鬼道";
								else if(pInfo->mask_sect & (1<<SECT_7))
									strPro = "焚香谷";
								else if(pInfo->mask_sect & (1<<SECT_10))
									strPro = "兽神";
								else if(pInfo->mask_sect & (1<<SECT_11))
									strPro = "狐妖";
								else if(pInfo->mask_sect & (1<<SECT_12))
									strPro = "太昊";
								else if(pInfo->mask_sect & (1<<SECT_13))
									strPro = "怀光";
								else if(pInfo->mask_sect & (1<<SECT_14))
									strPro = "轩辕";
								else if(pInfo->mask_sect & (1<<SECT_15))
									strPro = "天华";
								AString strDefault = "默认";
								int n;
								n = strAction.ReverseFind('_');
								strAction = strAction.Left(n + 1);
								n = strAction.Find('_');
								strAction.CutLeft(n);
								strAction = strPro + strAction + strDefault;
								m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY, strAction, cSerialId, GetPlayerInfo().cid, idTarget, pCastSkillPos);
							}
						}
					}

					if( m_pSprite && m_pSprite->GetModel() )
						m_pSprite->GetModel()->PlayActionByName(szAct, 1.0f, true);
				}

				sprintf(szAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
				m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f);

				for(int i=0;i<6;i++)
				{
					CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
					if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
					{
						pMirrorPlayer->GetPlayerModel()->PlayActionByName(ACT_CHANNEL_EYE, szAct, 1.0f);
					}
				}

				if (m_pPetModel || (m_AttachMode == enumAttachRideOnQilin && m_bHangerOn))
				{
					// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
					if (type == GNET::TYPE_ATTACK || type == GNET::TYPE_CURSE || IsFighting())
						sprintf(szAct, "%s_骑乘_%s", "战斗站立", szSuffix);
					else
						sprintf(szAct, "%s_骑乘_%s", "站立", szSuffix);

					m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_CASTSKILL);
				}
				else
				{
					//对于轩辕（辰皇）职业，针对地面系统动作需要处理光系和暗系动作 Added 2012-09-13.
					SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(m_BasicProps.iProfession);

					// make attack action end up with a stand to eliminate the still animation if a long delay occurs.
					if (type == GNET::TYPE_ATTACK || type == GNET::TYPE_CURSE || IsFighting())
					{
						if (m_nFlyMode == FLYMODE_WING && IsFlying())
							sprintf(szAct, "%s_翅膀_%s", "战斗站立", szSuffix);
						else
						{
							if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)) )
							{
								if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
								{
									sprintf(szAct, "%s_%s_%s", "战斗站立", szSuffix, "光");
								}
								else
								{
									sprintf(szAct, "%s_%s_%s", "战斗站立", szSuffix, "暗");
								}
							}
							else
							{
								sprintf(szAct, "%s_%s", "战斗站立", szSuffix);
							}
						}
					}
					else
					{
						if (m_nFlyMode == FLYMODE_WING && IsFlying())
							sprintf(szAct, "%s_翅膀_%s", "站立", szSuffix);
						else
						{
							if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)) )
							{
								if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
								{
									sprintf(szAct, "%s_%s_%s", "站立", szSuffix, "光");
								}
								else
								{
									sprintf(szAct, "%s_%s_%s", "站立", szSuffix, "暗");
								}
							}
							else
							{
								sprintf(szAct, "%s_%s", "站立", szSuffix);
							}
						}
					}
					m_pPlayerModel->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_CASTSKILL);

					for(int i=0;i<6;i++)
					{
						CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
						if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
						{
							pMirrorPlayer->GetPlayerModel()->QueueAction(ACT_CHANNEL_BODY, szAct, 300, ACT_CASTSKILL);
						}
					}
				}
			}
		}
	}
	if(spirit_index >= 0)
		m_pPlayerModel = pPlayerModelBak;
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

	if(m_pPuppetModel)
		m_pPuppetModel->SetPos(vPos);

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

	// If player is interactive subject.
	if (m_InteractorInfo.id && m_bInteractiveSubject)
	{
		CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_InteractorInfo.id);
		ASSERT(pInteractor);
		if (pInteractor)
			pInteractor->SetPos(vPos);
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

	if (m_iBuddyId && !m_bHangerOn && m_AttachMode == enumAttachRideOnQilin )
	{
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
		if (pBuddy)
			pBuddy->SetDirAndUp(vNewDir, vNewUp);
		}

	if (m_pPetModel)
		m_pPetModel->SetDirAndUp(vNewDir, vNewUp);
	
	if(m_pPuppetModel)
		m_pPuppetModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pBackupModel)
		m_pBackupModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pDummyModel)
		m_pDummyModel->SetDirAndUp(vNewDir, vNewUp);

	if (m_pInteractorModel)
		m_pInteractorModel->SetDirAndUp(vNewDir, vNewUp);
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
	else if(m_pPuppetModel)
	{
		if( iAction >= ACT_ATTACK_1 && iAction <= ACT_ATTACK_4 )
		{
			// attack action should not be played by this function
			return false;
		}

		if(m_iPupPetState == PUPPET_STATE_FIGHT && iAction == ACT_STAND)
		{
			iAction = ACT_FIGHTSTAND;
		}

		int iNPCAction = GetMappedAction(iAction);

		if(!bQueue)
		{
			if(m_iPupPetState != PUPPET_STATE_ANIM)
				m_pPuppetModel->PlayActionByName(CECNPC::GetActionName(iNPCAction), fWeight, bRestart, iTransTime, true, iAction);
		}
		else
		{
			m_pPuppetModel->QueueAction(CECNPC::GetActionName(iNPCAction), iTransTime, iAction, bRestart ? true : false);
			m_pPuppetModel->SetComActFlag(NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);
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
			if (IsDead()) // 播放死亡动作以后不能播放默认动作，会导致看起来死亡动作不播放
			{
				return true;
			}
			else if ( !bQueue )	//变身状态下，如果非循环动作，则使用默认动作,避免第一次播放一个不存在的动作时,没有特效(怀光坐上飞剑,变身后无任何形象的问题)
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
			if(m_iChangeShape == SHAPE_QILIN && m_iBuddyId)
			{
				CECPlayer* pBuddy= m_pPlayerMan->GetPlayer(m_iBuddyId);
				if(pBuddy && !pBuddy->IsSpellingMagic())
					pBuddy->PlayAction(iAction, fWeight, bRestart, iTransTime, bQueue);

			}
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
				else if(iAction == ACT_BATH)
				{
				    if(!m_bHangerOn)
					 sprintf(szAct, "被按摩通用");
					else
					{
                        CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
						assert(NULL != pBuddy);
						if (pBuddy->m_iPhysique == PHYSIQUE_NORMAL || pBuddy->m_iPhysique == PHYSIQUE_MIDDLE || pBuddy->m_iPhysique == PHYSIQUE_TETRAPOD)
							sprintf(szAct, "按摩中体型");
						else if (pBuddy->m_iPhysique == PHYSIQUE_BIG)
							sprintf(szAct, "按摩大体型");
						else if (pBuddy->m_iPhysique == PHYSIQUE_SMALL)
							sprintf(szAct, "按摩小体型");
						else if(pBuddy->m_iPhysique == PHYSIQUE_TINY)
                            sprintf(szAct, "按摩微体型");
					}
				}
				else if (iAction == ACT_PICKUP_MATTER)
				{
					if (m_AttachMode == enumAttachNone && !m_pPetModel)
						sprintf(szAct, m_ActionNames.GetANSIString(iAction));
					else
						return true;
				}
				else if (IsInDomainState() && !m_pCurSkill)
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
				// 互动时播放动作
				else if (m_InteractorInfo.id && m_bInteractiveSubject && !m_sPlayerInteractiveAction.IsEmpty())
				{
					if (m_InteractorInfo.state != 1)
					{
						sprintf(szAct, "%s", "");
					}
					else
					{
						if (iAction == ACT_WALK || iAction == ACT_RUN)
							sprintf(szAct, "%s", m_sPlayerInteractiveMoveAction);
						else
							sprintf(szAct, "%s", m_sPlayerInteractiveAction);
					}
				}
				else if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112 && (m_iSpecialState & SPECIAL_STATE_XUANFENG))
				{
					if(iAction == ACT_STAND || iAction == ACT_FIGHTSTAND)
						sprintf(szAct, "%s", "英招_移动旋风_站立");
					else if(iAction == ACT_WALK || iAction == ACT_RUN || iAction == ACT_FIGHT_RUN)
						sprintf(szAct, "%s", "英招_移动旋风_奔跑");
				}
				else
				{
					int nActType = m_uAttackType;
					if( nActType < 0 ) nActType = 0;
					if( nActType > 14 ) nActType = 14;
					const char * szSuffix = m_ActionNames.GetANSIString(nActType + ACT_SUFFIX_0);

					if (m_AttachMode == enumAttachHugPlayer) // male player hug female player
					{
						sprintf(szAct, "%s_绑定_通用",m_ActionNames.GetANSIString(iAction));
					}
					else if (m_AttachMode == enumAttachRideOnPet) // male player hug female player on a vehicle
						sprintf(szAct, "%s_双骑_通用",m_ActionNames.GetANSIString(iAction));
					else if( m_pPetModel || (m_AttachMode == enumAttachRideOnQilin && m_bHangerOn) ) // is riding alone
						sprintf(szAct, "%s_骑乘_%s",m_ActionNames.GetANSIString(iAction), szSuffix);
					else if( m_bMultiRideMember)
					{
						CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);
						if (pLeader && pLeader->GetRidingPetInfo().iMultiRideMode)    //多人骑乘之车马模式，乘客播放“乘坐”动作
							sprintf(szAct, "乘坐_%s", szSuffix);
						else
							sprintf(szAct, "%s_骑乘_%s", m_ActionNames.GetANSIString(iAction), szSuffix);
					}
					else if	(m_AttachMode == enumAttachActiveEmote)
					{
						if (m_bHangerOn)
							sprintf(szAct, "被%s",m_ActionNames.GetANSIString(iAction));
						else
							sprintf(szAct, "%s",m_ActionNames.GetANSIString(iAction));
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
						else if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)) && (!IsFlying() || (IsFlying() && (iAction2 == ACT_SITDOWN || iAction2 == ACT_SITDOWN_LOOP))) )	//轩辕新职业地面系统相关动作分为光系和暗系 注意：飞剑，飞骑，乘骑，翅膀状态下动作都不分光系和暗系 Modified 2012-08-15.
						{
							if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
							{
								sprintf(szAct, "%s_%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix, "光");
							}
							else
							{
								sprintf(szAct, "%s_%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix, "暗");
							}
							
						}
						else if (m_AttachMode == enumAttachActiveEmote)
						{
							sprintf(szAct, "%s",m_ActionNames.GetANSIString(iAction2));
						}
						else	// 飞行状态
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

				char  szMirrorAct[256];
				SECT_LEVEL2_INFO * pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
				int nActType = m_uAttackType;
				if( nActType < 0 ) nActType = 0;
				if( nActType > 14 ) nActType = 14;
				const char * szSuffix = m_ActionNames.GetANSIString(nActType + ACT_SUFFIX_0);
				if ( pInfo && (pInfo->mask_sect&(1<<SECT_14)))	//轩辕新职业地面系统相关动作分为光系和暗系 注意：飞剑，飞骑，乘骑，翅膀状态下动作都不分光系和暗系 Modified 2012-08-15.
				{
					if(!IsFlying())
					{
						if ( GetDarkLightState() == 0 )	//g_nDebugBlackAndLightValue	!!!!!!TEST!!!!!!!!!!
						{
							sprintf(szMirrorAct, "%s_%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix, "光");
						}
						else
						{
							sprintf(szMirrorAct, "%s_%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix, "暗");
						}
					}
					else	// 若是在飞行状态，且飞行道具为飞骑，则镜像播放站立，与主角不一致；  若飞行道具为飞剑和翅膀，则镜像和主角一致，播放相应飞剑翅膀动作
					{
						if(m_nFlyMode == FLYMODE_FEIQI)
						{
							if ( GetDarkLightState() == 0 )
								sprintf(szMirrorAct, "%s_%s_%s",m_ActionNames.GetANSIString(ACT_STAND), szSuffix, "光");
							else
								sprintf(szMirrorAct, "%s_%s_%s",m_ActionNames.GetANSIString(ACT_STAND), szSuffix, "暗");
						}
						else
							sprintf(szMirrorAct, "%s_%s",m_ActionNames.GetANSIString(iAction2), szSuffix);
					}
					
				}

				if( !bQueue )
				{
					m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY, szAct, fWeight, bRestart, iTransTime, true, iAction);

					char szEyeAct[64];
					sprintf(szEyeAct, "%s_%s",m_ActionNames.GetANSIString(ACT_BLINK), "通用");
					m_pPlayerModel->PlayActionByName(ACT_CHANNEL_EYE, szEyeAct, fWeight, bRestart, iTransTime, true, iAction);

					if (m_pPetModel && szPetAct)
						m_pPetModel->PlayActionByName(szPetAct, fWeight, bRestart, iTransTime, true);

					if (m_InteractorInfo.id && m_pInteractorModel)
					{
						if (m_InteractorInfo.state == 1 && (iAction == ACT_WALK || iAction == ACT_RUN))
							m_pInteractorModel->PlayActionByName(m_sObjInteractiveMoveAction, fWeight, bRestart, iTransTime, true);
						else
							m_pInteractorModel->PlayActionByName(m_sObjInteractiveAction, fWeight, bRestart, iTransTime, true);
					}
				
					for(int i=0;i<6;i++)
					{
						CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
						if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
						{
							pMirrorPlayer->GetPlayerModel()->PlayActionByName(ACT_CHANNEL_BODY, szMirrorAct, fWeight, bRestart, iTransTime, true, iAction);
							pMirrorPlayer->GetPlayerModel()->PlayActionByName(ACT_CHANNEL_EYE, szEyeAct, fWeight, bRestart, iTransTime, true, iAction);
						}
					}
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

					if (m_InteractorInfo.id && m_pInteractorModel)
					{
						if (m_InteractorInfo.state == 1 && (iAction == ACT_WALK || iAction == ACT_RUN))
							m_pInteractorModel->QueueAction(m_sObjInteractiveMoveAction, iTransTime);
						else
							m_pInteractorModel->QueueAction(m_sObjInteractiveAction, iTransTime);
					}

					for(int i=0;i<6;i++)
					{
						CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
						if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
						{
							pMirrorPlayer->GetPlayerModel()->QueueAction(ACT_CHANNEL_BODY, szMirrorAct, iTransTime, iAction, bRestart ? true : false);
							pMirrorPlayer->GetPlayerModel()->SetComActFlag(ACT_CHANNEL_BODY, NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);

							pMirrorPlayer->GetPlayerModel()->QueueAction(ACT_CHANNEL_EYE, szEyeAct, iTransTime, iAction, bRestart ? true : false);
							pMirrorPlayer->GetPlayerModel()->SetComActFlag(ACT_CHANNEL_EYE, NULL, COMACT_FLAG_MODE_ONCE_IGNOREGFX);
						}
					}
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
	int iRoleID,	//Added 2012-08-21.
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

	//四足体型变身为人形时，穿人族的时装
	int iDefaultHair = 0;
	CECPlayer* pPlayerSpe = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(iRoleID);
	if(pPlayerSpe)
	{
		iDefaultHair = pPlayerSpe->GetHairid();
		if(pPlayerSpe->GetShapeChangedId() == SHAPE_HUMAN && iProfession >= 108 && iProfession <= 112 && pPlayerSpe->GetBackupModel() != pPlayerSpe->GetCacheModel())
			iPhysique = PHYSIQUE_NORMAL;
	}


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

			
			if( i >= EQUIPIVTR_FASHION_EYE && i <= EQUIPIVTR_FASHION_BACK 
				|| i == EQUIPIVTR_FASHION_WEAPON)
			{
				int nIndex = (idEquipment & 0x00ff0000) >> 16;
				if(nIndex > 70)
				{
					nIndex = 0;
				}
				color = l_fashionColor[nIndex];	
				idEquipment &= 0x0000ffff;
			}
			else 
			{
				if( i == EQUIPIVTR_WEAPON )
				{
					pResult->stoneWeapon = (idEquipment & 0x00ff0000) >> 16;
				}
				else if( i == EQUIPIVTR_HEAD )
				{
					pResult->stoneHead = (idEquipment & 0x00ff0000) >> 16;
				}
				else if( i == EQUIPIVTR_BODY )
				{
					pResult->stoneBody = (idEquipment & 0x00ff0000) >> 16;
				}
				else if( i == EQUIPIVTR_FOOT )
				{
					pResult->stoneFoot = (idEquipment & 0x00ff0000) >> 16;
				}
				else if( i == EQUIPIVTR_MAIN_TALISMAN || i == EQUIPIVTR_WING)
				{
					pResult->talisman_quality = max(0, (min(7, (idEquipment & 0x00ff0000) >> 16)-1));
				}
				else if ( i == EQUIPIVTR_RUNE )	//元婴，
				{
					pResult->nRuneQuality = (idEquipment & 0x00ff0000) >> 16;//max( 0, (min(6, (idEquipment & 0x00ff0000) >> 16)-1) );
					//-----------------------------------------------------
					pResult->bTuneChanged = true;
					//-----------------------------------------------------
				}
				idEquipment &= 0x0000ffff;
			}

			//-------------------------------------------------
			//添加处理轩辕职业的光暗状态的值，供换装备使用
			int nXYDarkLightState = 0;
			bool bHideXYMask = false;
			//新增对LoginPlayer换装备时黑暗光明变化 Added 2012-09-05.
			if ( !g_nLoginPlayerDarkLightState )			//光明
			{
				nXYDarkLightState = 0;
			}
			else if ( 1 == g_nLoginPlayerDarkLightState )	//黑暗
			{
				nXYDarkLightState = 1;
			}
			//Added end.
			if ( g_pGame->GetGameRun() && g_pGame->GetGameRun()->GetWorld() && g_pGame->GetGameRun()->GetWorld()->GetPlayerMan() )
			{
				CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer( iRoleID );
				if ( pPlayer )
				{
					nXYDarkLightState	= pPlayer->GetDarkLightState();
					bHideXYMask			= pPlayer->IsMaskHide();
				}
			}
			//-------------------------------------------------

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
						{
							ChangeArmor(
								pEquip,
								pResult->dwShowMask,
								pResult->dwSkinChangeMask,
								iPhysique,
								iProfession,
								nGender,
								iDefaultSkin,
								pResult->aSkins,
								nXYDarkLightState,
								bHideXYMask);
							continue;
						}
						case 3:		// 时装
							ChangeFashion(
								pEquip,
								pResult->dwFashionShowMask,
								pResult->dwSkinChangeMask,
								pResult->dwTetrapodFashionMask,
								iPhysique,
								iProfession,
								nGender,
								iDefaultSkin,
								iDefaultHair,
								pResult->aSkins,
								color,
								nXYDarkLightState
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
							CECPlayer* pPlayer = NULL;
							const char* szModelFile = "";
							int iChangeShape = 0;

							if(g_pGame->GetGameRun() && g_pGame->GetGameRun()->GetWorld() && g_pGame->GetGameRun()->GetWorld()->GetPlayerMan())
								pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(iRoleID);

							if(pPlayer)
								iChangeShape = pPlayer->GetShapeChangedId();

							//四足体型使用非翅膀类型的飞剑时，使用默认的翅膀模型
							if(iProfession >= 108 && iProfession <= 112 && pPlayer && pPlayer->GetShapeChangedId() != SHAPE_HUMAN && pTalisman->fly_mode != FLYMODE_WING)
							{
								pResult->nFlyMode = FLYMODE_WING;
								szModelFile = "Models\\Players\\通用装备\\翅膀\\四足翅膀\\四足翅膀.ecm";
							}
							else
							{
								pResult->nFlyMode = pTalisman->fly_mode;
								szModelFile = g_pGame->GetDataPath(pTalisman->file_model[pResult->talisman_quality]);
								AString strNewModelFile;
								if ( pTalisman->color_change)
								{
									if ( pPlayer )
									{
										GetUpdatedWingFile( szModelFile, strNewModelFile, pPlayer->GetWingChangeColor() );
										szModelFile = (const char*)strNewModelFile;
									}
								}
							}

							ChangeWing(pResult, szModelFile, iProfession, iChangeShape);
						}
						else
							ChangeTalisman(pResult, g_pGame->GetDataPath(pTalisman->file_model[pResult->talisman_quality]));

						continue;
					}
					else if (dt == DT_RUNE_EQUIP_ESSENCE)
					{
						//Canceled 2012-06-06.
						//RUNE_EQUIP_ESSENCE* pTune = (RUNE_EQUIP_ESSENCE*)pEquip;
						//ChangeTune(pResult, g_pGame->GetDataPath(pTune->file_model));
						//UpdateRuneStateGFX( nRunQuality );
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
				ChangeDefaultUpper(pResult->aSkins[SKIN_BODY_INDEX], iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
			}
			else if (i == EQUIPIVTR_FOOT)
			{
				pResult->dwShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FOOT_INDEX);
				pResult->stoneFoot = 0;
				ChangeDefaultFoot(pResult->aSkins[SKIN_FOOT_INDEX], iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
			}
			else if (i == EQUIPIVTR_WEAPON || i == EQUIPIVTR_FASHION_WEAPON)
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
				if(iProfession >= 108 && iProfession <= 112)
				{
					if(iPhysique == PHYSIQUE_TETRAPOD)
						pResult->dwTetrapodFashionMask |= (1 << SKIN_FASHION_UPPER_BODY_INDEX);
					else
						pResult->dwTetrapodFashionMask &= ~(1<<SKIN_FASHION_UPPER_BODY_INDEX);
				}
				ChangeDefaultFashionUpper(pResult->aSkins[SKIN_FASHION_UPPER_BODY_INDEX], iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
			}
			else if (i == EQUIPIVTR_FASHION_LEG)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowLowerBody);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_LOWER_INDEX);
				if(iProfession >= 108 && iProfession <= 112)
				{
					if(iPhysique == PHYSIQUE_TETRAPOD)
						pResult->dwTetrapodFashionMask |= (1 << SKIN_FASHION_LOWER_INDEX);
					else
						pResult->dwTetrapodFashionMask &= ~(1<<SKIN_FASHION_LOWER_INDEX);
				}
				ChangeDefaultFashionLower(pResult->aSkins[SKIN_FASHION_LOWER_INDEX], iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
			}
			else if (i == EQUIPIVTR_FASHION_FOOT)
			{
				pResult->dwFashionShowMask |= (1 << enumSkinShowFoot);
				pResult->dwSkinChangeMask |= (1 << SKIN_FASHION_FOOT_INDEX);
				if(iProfession >= 108 && iProfession <= 112)
				{
					if(iPhysique == PHYSIQUE_TETRAPOD)
						pResult->dwTetrapodFashionMask |= (1 << SKIN_FASHION_FOOT_INDEX);
					else
						pResult->dwTetrapodFashionMask &= ~(1<<SKIN_FASHION_FOOT_INDEX);
				}
				ChangeDefaultFashionFoot(pResult->aSkins[SKIN_FASHION_FOOT_INDEX], iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
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
				if(iProfession >= 108 && iProfession <= 112)
				{
					if(iPhysique == PHYSIQUE_TETRAPOD)
						pResult->dwTetrapodFashionMask |= (1 << SKIN_FASHION_HEADWEAR_INDEX);
					else
						pResult->dwTetrapodFashionMask &= ~(1<<SKIN_FASHION_HEADWEAR_INDEX);
				}
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
    for( i = 0; i < SIZE_EQUIPIVTR; ++i)
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
		if(m_iChangeShape == SHAPE_HUMAN && i != EQUIPIVTR_FASHION_HEAD && i != EQUIPIVTR_FASHION_BODY && i != EQUIPIVTR_FASHION_LEG && i != EQUIPIVTR_FASHION_FOOT && i != EQUIPIVTR_WING)	//四足体型变形为人型时，只能加载时装和飞剑
		{
		}
		else
		{
			if (pNew[i] != pOld[i])
			{
				if (pOld[i] > 0)
					m_pBackupModel->OnScriptChangeEquip(pOld[i], 0, m_bFashionMode, GetEquipPathId(pOld[i], m_iGender, m_iPhysique));

				//-----------------------------------------------------------------
				//针对辰皇新职业，需要在黑暗状态并且带面罩头盔情况下显示面罩特效, Added 2012-09-14.
				bool bNeedChange = ( ( m_BasicProps.iProfession < 56 || m_BasicProps.iProfession > 60 ) || ( (m_BasicProps.iProfession >= 56 && m_BasicProps.iProfession <= 60) && (GetDarkLightState()==1) && !IsMaskHide() ) ) ? true : false;
				//-----------------------------------------------------------------
				if (pNew[i] > 0 && bNeedChange)
					m_pBackupModel->OnScriptChangeEquip(pNew[i], 1, m_bFashionMode, GetEquipPathId(pNew[i], m_iGender, m_iPhysique));

				dwMask |= (1 << i);
				pOld[i] = pNew[i];
			}
			//---------------------------------------------------------------------
			//针对辰皇新职业，如果需要强制改变头盔，则同样需要执行换装脚本 Added 2012-09-14.
			else if ( (i == EQUIPIVTR_HEAD) && (m_BasicProps.iProfession >= 56 && m_BasicProps.iProfession <= 60) && (m_dwForceChangeMask | EQUIPIVTR_HEAD) )
			{
				if ( (GetDarkLightState() == 1 || g_nLoginPlayerDarkLightState == 1) && !IsMaskHide() )	//处于黑暗状态以及带面罩头盔
				{
					m_pBackupModel->OnScriptChangeEquip(pNew[i], 1, m_bFashionMode, GetEquipPathId(pNew[i], m_iGender, m_iPhysique));
				}
				else
				{
					m_pBackupModel->OnScriptChangeEquip(pNew[i], 0, m_bFashionMode, GetEquipPathId(pNew[i], m_iGender, m_iPhysique));
				}
			}
			//---------------------------------------------------------------------
			m_aEquips[i] = pEquipmentID[i];             // necessary? 
		}
	}

	//-------------------------------------------------------------------------
	//在别处相应的逻辑处理时，修改强制改变装备掩码
	dwMask |= m_dwForceChangeMask;

	//-------------------------------------------------------------------------
	//如果装备了时装武器，则不显示武器;如果不装备时装武器，重新显示武器。
	if ((dwMask & (1 << EQUIPIVTR_FASHION_WEAPON)) && pNew[EQUIPIVTR_FASHION_WEAPON] == 0)
	{
		dwMask &= ~(1 << EQUIPIVTR_FASHION_WEAPON);
		dwMask |= 1 << EQUIPIVTR_WEAPON;
	}
	else if ((dwMask & (1 << EQUIPIVTR_WEAPON)) && pNew[EQUIPIVTR_FASHION_WEAPON] != 0)
	{
		dwMask &= ~(1 << EQUIPIVTR_WEAPON);
	}
	
	//对于轩辕处于暗系状态下，需要加入一个特效挂在身上，Added 2012-09-14.
	if ( m_BasicProps.iProfession >= 56 && m_BasicProps.iProfession <= 60 )
	{
		if ( GetDarkLightState() == 1 || g_nLoginPlayerDarkLightState == 1 )	//暗系状态
		{
			m_pBackupModel->PlayGfx( "人物\\通用\\轩辕暗身上黑烟.gfx", "HH_Spine" );
		}
		else																	//光系状态
		{
			m_pBackupModel->RemoveGfx( "人物\\通用\\轩辕暗身上黑烟.gfx", "HH_Spine" );
		}
	}
	//-------------------------------------------------------------------------

	QueueLoadEquips(pNew, dwMask, bLoadAtOnce);

	if(m_MirrorModels[0])
	{
		memcpy(m_aEquipsTmp, pNew, SIZE_EQUIPIVTR*sizeof(int));
		m_aEquipsTmp[EQUIPIVTR_MAIN_TALISMAN] = 0;
		QueueECModelForLoad(
			MTL_ECM_MIRROR_PLAYER_EQUIP,
			m_PlayerInfo.cid,
			m_dwBornStamp,
			GetPos(),
			NULL,
			NULL,
			m_iPhysique,
			m_iGender,
			dwMask,
			m_aEquipsTmp,
			m_BasicProps.iProfession,
			0,
			0,
			0,
			m_iDefaultSkin);
	}
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
			else if(nPhysique == PHYSIQUE_TINY)
			{
				return (nGender == 0) ? pEquip->file_model_tiny_male : pEquip->file_model_tiny_female;
			}
			else if(nPhysique == PHYSIQUE_TETRAPOD)
				return pEquip->file_model_tetrapod_male;
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

void CECPlayer::GetUpdatedWingFile( const char* szModelFile, AString& strNewWingFile, unsigned char ucWingChangeColor )
{
	strNewWingFile = "";

	//首先判断和检查输入参数合法性
	if ( !szModelFile )
	{
		return;
	}

	strNewWingFile = szModelFile;
	
	//根据变色飞剑索引值计算新的模型路径
	static const char*	szColorName[MAX_WING_COLOR_COUNT] = {
																"红",
																"粉",
																"蓝",
																"绿",
																"青",
																"紫",
															};

	if ( ucWingChangeColor > 0 && ucWingChangeColor <= MAX_WING_COLOR_COUNT )
	{
		AString strLeft		= strNewWingFile.Left( strNewWingFile.GetLength() - 6 );
		AString strRight	= strNewWingFile.Right( 6 );
		strNewWingFile = strLeft + szColorName[ucWingChangeColor-1] + strRight;
	}
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

bool CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, const char* szLeft, const char* szRight, const char* szBack, const char* szNewBack)
{
	pResult->bWeaponChanged = true;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

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

	//-------------------------------------------------------------------------
	//Added 2012-07-26.
	if ( szNewBack[0] )
	{
		CECModel* pWeapon = new CECModel;

		if ( pWeapon->Load(szNewBack, true, A3DSkinModel::LSF_UNIQUESKIN) )
		{
			_SetWeaponMaterial(pWeapon->GetA3DSkinModel());
			pResult->pNewBackWeapon = pWeapon;
		}
		else
		{
			delete pWeapon;
		}

		//Delete the szNewBack.....----> It is very special.
		delete (char*)szNewBack;
	}
	//-------------------------------------------------------------------------

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

void CECPlayer::ChangeWeapon(EquipsLoadResult* pResult, int iPhysique, int nGender, const EQUIPMENT_ESSENCE* pWeapon)
{
	pResult->uAttackType = pWeapon->action_type;

	const char* szWeaponFile;
	const char* szWeaponRightFile = "";
	const char* szWeaponBackFile = "";
	const char* szWeaponNewBackFile = ""; //Added 2012-07-26.
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
		else if ( pWeapon->action_type == WEAPON_BOOK )	//器，轩辕职业用，背部武器分为战斗状态和非战斗状态 Added 2012-07-26.
		{
			szWeaponBackFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_back_male) : g_pGame->GetDataPath(pWeapon->file_model_back_female);
			int nFileSize = strlen(szWeaponBackFile);
			if ( nFileSize > 5 )	//路径均为：XXX1.ecm形式
			{
				char* szBackFile = new char[nFileSize+1];
				memcpy( szBackFile, szWeaponBackFile, nFileSize );
				szBackFile[nFileSize] = '\0';
				szBackFile[nFileSize - 5] = '3';
				szWeaponNewBackFile = szBackFile;
			}
			else
			{
				ASSERT(false);
			}
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
	else if(iPhysique == PHYSIQUE_TINY)
	{
		szWeaponFile = (nGender == 0) ? g_pGame->GetDataPath(pWeapon->file_model_tiny_male) : g_pGame->GetDataPath(pWeapon->file_model_tiny_female);
	}

	else if(iPhysique == PHYSIQUE_TETRAPOD)
	{
		szWeaponFile = g_pGame->GetDataPath(pWeapon->file_model_tetrapod_male);
	}
	else
		return;

	ChangeWeapon(pResult, szWeaponFile, szWeaponRightFile, szWeaponBackFile, szWeaponNewBackFile); //Modified 2011-08-09.
}

void CECPlayer::ChangeTalisman(EquipsLoadResult* pResult, const char* szPath)
{
	pResult->bTalismanChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
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

bool CECPlayer::ChangeWing(EquipsLoadResult* pResult, const char* szModeFile, int iProfession, int iChangeShape)
{
	//四足专属翅膀
	AString strNewFile = szModeFile;
	const char* szDefaultModel = "Models\\Players\\通用装备\\翅膀\\四足翅膀\\四足翅膀.ecm";
	if(iProfession >= 108 && iProfession <= 112 && iChangeShape != SHAPE_HUMAN && !strstr(szModeFile, szDefaultModel))
	{
		const char*  szText = "_四足";
		AString strLeft = strNewFile.Left(strNewFile.GetLength() - 4);
		AString strRight = strNewFile.Right(4);
		strNewFile = strLeft + szText + strRight;
		szModeFile = (const char*)strNewFile;
	}
	
	pResult->bWingChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	CECModel* pWing = new CECModel;

	if (pWing->Load(szModeFile, true, A3DSkinModel::LSF_UNIQUESKIN))
	{
		_SetWeaponMaterial(pWing->GetA3DSkinModel());
		pResult->pWing = pWing;
	}
	else if(iProfession >= 108 && iProfession <= 112 && iChangeShape != SHAPE_HUMAN )
	{
		if(pWing->Load(szDefaultModel, true, A3DSkinModel::LSF_UNIQUESKIN))
		{
			_SetWeaponMaterial(pWing->GetA3DSkinModel());
			pResult->pWing = pWing;
		}
		else
			delete pWing;
	}
	else
		delete pWing;

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

bool CECPlayer::ChangeTune(EquipsLoadResult* pResult, const char* szPath)
{
	pResult->bTuneChanged = true;
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
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
	A3DSkin* pSkins[][3],
	int nXYDarkLightState,	//NEW ADDED JUST FOR XUANYUAN
	bool bHideXYMask		//NEW ADDED JUST FOR XUANYUAN
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
	else if(iPhysique == PHYSIQUE_TINY)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pArmor->file_model_tiny_male) : g_pGame->GetDataPath(pArmor->file_model_tiny_female);
	}
	else if(iPhysique == PHYSIQUE_TETRAPOD)
	{
		szFile = g_pGame->GetDataPath(pArmor->file_model_tetrapod_male);
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
		iSkin,
		nXYDarkLightState,
		bHideXYMask
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
	int iDefaultSkin,
	int nXYDarkLightState,	//NEW ADDED JUST FOR XUANYUAN
	bool bHideXYMask		//NEW ADDED JUST FOR XUANYUAN
	)
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	switch (nLocation)
	{
	case enumSkinShowUpperBody:
	case enumSkinShowUpperAndLower:
		//-------------------------------------------------------------------------
		//Test
		if ( (iProfession >= 56 && iProfession <= 60) && (iPhysique == PHYSIQUE_MIDDLE) && ( nXYDarkLightState == 1) && ( !bDefault ) )//g_nDebugBlackAndLightValue
		{
			AString strFile = strSkinFile;
			if ( strFile.Find( ".ski" ) < 0 )
			{
				ASSERT(false);
			}
			AString strLeft = strFile.Left(strFile.GetLength() - 5);
			AString strRight = strFile.Right(4);
			AString strNewFile = strLeft + "2" +strRight;//"1"
			if (!LoadPlayerSkin(aSkins, SKIN_BODY_INDEX, strNewFile))
			{
				a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strNewFile);
				if (!bDefault) ChangeDefaultUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
			}

		}
		else
		//-------------------------------------------------------------------------	
		if (!LoadPlayerSkin(aSkins, SKIN_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;

	case enumSkinShowFoot:
		//-------------------------------------------------------------------------
		//Test
		if ( (iProfession >= 56 && iProfession <= 60) && (iPhysique == PHYSIQUE_MIDDLE) && (nXYDarkLightState == 1) && ( !bDefault ) )//g_nDebugBlackAndLightValue
		{
			AString strFile = strSkinFile;
			if ( strFile.Find( ".ski" ) < 0 )
			{
				ASSERT(false);
			}
			AString strLeft = strFile.Left(strFile.GetLength() - 5);
			AString strRight = strFile.Right(4);
			AString strNewFile = strLeft + "2" + strRight;//+ "1"
			if (!LoadPlayerSkin(aSkins, SKIN_FOOT_INDEX, strNewFile))
			{
				a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
				if (!bDefault) ChangeDefaultUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
			}

		}
		else
		//-------------------------------------------------------------------------	
		if (!LoadPlayerSkin(aSkins, SKIN_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFoot(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
		}

		break;
		
	case enumSkinShowHead:
		//-------------------------------------------------------------------------
		//Test
		if ( (iProfession >= 56 && iProfession <= 60) && (iPhysique == PHYSIQUE_MIDDLE) )//g_nDebugBlackAndLightValue
		{
			static const char* szSuffix[4] = { "11", "12", "21", "22" };
			const char* szCurrentSuffix = NULL;
			if (nXYDarkLightState == 1)	//	黑暗
			{
				if ( bHideXYMask )	//隐藏面具
				{
					szCurrentSuffix = szSuffix[3];	//黑暗不戴面具
				}
				else
				{
					szCurrentSuffix = szSuffix[2];	//黑暗戴面具
				}
			}
			else
			{
				if ( bHideXYMask )	//隐藏面具
				{
					szCurrentSuffix = szSuffix[1];	//光明不戴面具
				}
				else
				{
					szCurrentSuffix = szSuffix[0];	//光明戴面具
				}				
			}

			AString strFile = strSkinFile;
			if ( strFile.Find( ".ski" ) < 0 )
			{
				ASSERT(false);
			}
			AString strLeft = strFile.Left(strFile.GetLength() - 6);
			AString strRight = strFile.Right(4);
			AString strNewFile = strLeft + szCurrentSuffix + strRight;
//			strFile[strFile.GetLength() - 5] = '2';
			if (!LoadPlayerSkin(aSkins, SKIN_HELM_INDEX, strNewFile))
			{
				a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
				if (!bDefault) ChangeDefaultUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin);
			}

		}
		else
		//-------------------------------------------------------------------------	
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
	DWORD& dwTetrapodFashionMask,
	int iPhysique,
	int iProfession,
	int nGender,
	int iDefaultSkin,
	int iDefaultHair,
	A3DSkin* pSkins[][3],
	A3DCOLOR color,
	int nXYDarkLightState
	)
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
	else if(iPhysique == PHYSIQUE_TINY)
	{
		szFile = (nGender == 0) ? g_pGame->GetDataPath(pFashion->file_model_tiny_male) : g_pGame->GetDataPath(pFashion->file_model_tiny_female);
	}
	else if(iPhysique == PHYSIQUE_TETRAPOD)
	{
		szFile = g_pGame->GetDataPath(pFashion->file_model_tetrapod_male);
	}
	else
		return;

	if (szFile[0] == 0)
	{
		if(iProfession >= 108 && iProfession <= 112)    //szFile临时用 等资源和装备模板
		{
			if(iPhysique == PHYSIQUE_NORMAL)       //四足体型变身为人形，时装装备位为四足的装备时，穿默认的人形时装
			{
				if(pFashion->equip_location == enumSkinShowUpperBody || pFashion->equip_location == enumSkinShowUpperAndLower || pFashion->equip_location == enumSkinShowLowerBody)
					szFile = _ski_path[2];
				else if(pFashion->equip_location == enumSkinShowFoot)
					szFile = _ski_path[3];
				else if(pFashion->equip_location == enumSkinShowHead)
					szFile = _ski_path[1];
			}
			else if(iPhysique == PHYSIQUE_TETRAPOD)  //四足体型，时装装备位为人形的装备时， 穿默认的四足体型时装
			{
				AString str;		
				if(pFashion->equip_location == enumSkinShowUpperBody || pFashion->equip_location == enumSkinShowUpperAndLower || pFashion->equip_location == enumSkinShowLowerBody)
					szFile = str.Format(_equipment_default_body_skin[iPhysique][nGender], "英招", iDefaultSkin + 1);
				else if(pFashion->equip_location == enumSkinShowFoot)
					szFile = str.Format(_equipment_default_shoe_skin[iPhysique][nGender], "英招", iDefaultSkin + 1);
				else if(pFashion->equip_location == enumSkinShowHead)
					szFile = str.Format(_hair_skin[iPhysique][nGender], iDefaultHair + 1);
			}
		}
		else 
			return;
	}

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
	if(iProfession >= 108 && iProfession <= 112)
	{
		if( iPhysique == PHYSIQUE_TETRAPOD)
			dwTetrapodFashionMask |= (1 << nSkinIndex);
		else 
			dwTetrapodFashionMask &= ~(1 << nSkinIndex);
	}

	ChangeFashion(
		szFile,
		pSkins[nSkinIndex],
		nLocation,
		false,
		iPhysique,
		iProfession,
		nGender,
		color,
		iDefaultSkin,
		nXYDarkLightState
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
	int iDefaultSkin,
	int nXYDarkLightState
	)
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);

	switch (nLocation)
	{
	case enumSkinShowUpperBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
		}

		break;

	case enumSkinShowLowerBody:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_LOWER_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionLower(aSkins, iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
		}

		break;

	case enumSkinShowFoot:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_FOOT_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionFoot(aSkins, iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
		}

		break;

	case enumSkinShowUpperAndLower:

		if (!LoadPlayerSkin(aSkins, SKIN_FASHION_UPPER_BODY_INDEX, strSkinFile))
		{
			a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
			if (!bDefault) ChangeDefaultFashionUpper(aSkins, iPhysique, iProfession, nGender, iDefaultSkin, nXYDarkLightState);
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
		{
			if (!LoadPlayerSkin(aSkins, SKIN_FASHION_HEADWEAR_INDEX, strSkinFile))
				a_LogOutput(1, "Equipment Invent: Wrong Skin: %s", strSkinFile);
		}
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
			A3DEffect* pEffect = dynamic_cast<A3DEffect*>(pTex);
			if(pEffect)
			{
				//AString strHLSLName = pEffect->GetHLSLFilename();
				//AString strMtlName = pEffect->GetMtlFilename();
				A3DCOLORVALUE cv(color);
				A3DVECTOR4 v4(cv.r, cv.g, cv.b, cv.a);
				pEffect->GetHLSL()->SetValue4f("g_colHair", &v4);
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

	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	float fScale = pUIMan->GetWindowScale();
	int index = g_pGame->GetConfigs()->GetVideoSettings().cNameSize;
	a_Clamp(index, 0, 2);
	float fScaleName = pUIMan->GetWindowScale()*(14.0f + 2.0f*index)/12.0f;

	CECImageRes* pImageRes = g_pGame->GetImageRes();
	int x, y=m_PateContent.iCurY, cx, cy;

	//	Decide name color 
	DWORD dwNameCol;
	if (ISCLONEPLAYERID(m_PlayerInfo.cid))
	{
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(m_idSummoner);
		if (pPlayer)
			dwNameCol = pPlayer->GetNameColor();
		else
			dwNameCol = GetNameColor();
	}
	else
		dwNameCol = GetNameColor();

	if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
	{
		// 竞技场，不显示玩家信息
		return true;
	}
	// 处于匿名地图，不用显示玩家的名字
	if (m_bAnonymous && !IsHostPlayer())
		return true;

	if(m_iBattleType == BATTLE_TYPE_SONJIN || m_iBattleType == BATTLE_TYPE_SONJINREBORN)
	{
		if (m_pPateName && (dwFlags & RNF_NAME))
		{
			if (g_pGame->GetPrivilege()->Has_Toggle_NameID())
			{
				ACString strText;
				if (g_pGame->GetConfigs()->GetShowIDFlag())
					strText.Format(_AL("%u"), m_PlayerInfo.cid);
				else
					strText = m_strName;

				m_pPateName->SetText(strText, false);
			}

			m_pPateName->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			x = m_PateContent.iBaseX - (cx >> 1);
			y -= cy;
			m_pPateName->RegisterRender(x, y, dwNameCol, m_PateContent.z, m_PateContent.rhw);
		}
		
		if(m_pPateRankName && m_pPateRankName->GetItemNum() > 0)
		{
			m_pPateRankName->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			x = m_PateContent.iBaseX - (cx >> 1);
			y -= cy;
			m_pPateRankName->RegisterRender(x, y, dwNameCol, m_PateContent.z, m_PateContent.rhw);
		}
		return true;
	}

	//	Draw name string
	if (m_pPateName && (dwFlags & RNF_NAME))
	{
		if (g_pGame->GetPrivilege()->Has_Toggle_NameID())
		{
			ACString strText;
			if (g_pGame->GetConfigs()->GetShowIDFlag())
				strText.Format(_AL("%u"), m_PlayerInfo.cid);
			else
				strText = m_strName;

			m_pPateName->SetText(strText, false);
		}

		m_pPateName->GetExtent(&cx, &cy);
		cx = int(cx * fScaleName);
		cy = int(cy * fScaleName);
		x = m_PateContent.iBaseX - (cx >> 1);
		y -= cy;
	//	m_pPateName->Render(pViewport, x, y+2, dwNameCol, m_PateContent.z);
		m_pPateName->RegisterRender(x, y, dwNameCol, m_PateContent.z, m_PateContent.rhw, CECPateText::TYPE_NAME);
		
		//变身情况下且被替换了名字，则只显示替换后的怪物名字
		if (m_pTransfiguredModel && m_TransfigureInfo.bReplaceName) 
			return true;

		int tx = x + cx + 4;


		//king
		{
			CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
			if(pHost && pHost->GetKingdomKingId() == GetCharacterID())
			{
				pImageRes->GetImageItemSize(CECImageRes::IMG_KINGDOM_KING, 0, &cx, &cy);
				cx = int(cx * fScaleName);
				cy = int(cy * fScaleName);
				pImageRes->RegisterDraw(CECImageRes::IMG_KINGDOM_KING, x - cx, y, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx, cy);
				x -= cx;
			}
		}

		if ( (m_cIsHideVip==0&&GetVipLv()==6) || (GetVipLv()!=6&&GetVipLv()>0) )
		{
			pImageRes->GetImageItemSize(CECImageRes::IMG_VIP, 0, &cx, &cy);
			cx = int(cx * fScaleName);
			cy = int(cy * fScaleName);
			pImageRes->RegisterDraw(CECImageRes::IMG_VIP, x - cx, y, 0xffffffff, GetVipLv()-1, m_PateContent.z, m_PateContent.rhw, cx, cy);
			x -= cx;
		}
		
		// see if show reputation icon here
		if( GetBasicProps().iLevel >= 60 )
		{
			int iReputationLevel = GetReputationLevel(GetReputation());
			pImageRes->GetImageItemSize(CECImageRes::IMG_REPUTATIONICONS, 0, &cx, &cy);
			cx = int(cx * fScaleName);
			cy = int(cy * fScaleName);
			pImageRes->RegisterDraw(CECImageRes::IMG_REPUTATIONICONS, x - cx, y, 0xffffffff, iReputationLevel, m_PateContent.z, m_PateContent.rhw, cx, cy);
		}
       
	


		
		//	Draw team leader icon
		//	TODO: It's better to use m_pTeam rather host's team object, but in
		//		current version, else player's m_pTeam is always NULL
		CECTeam* pTeam = g_pGame->GetGameRun()->GetHostPlayer()->GetTeam();
		if (pTeam)
		{
			int iIcon = -1;
			if (pTeam->GetLeaderID() == m_PlayerInfo.cid)
				iIcon = CECImageRes::IMG_TEAMLEADER;
			else if (pTeam->GetMemberByID(m_PlayerInfo.cid))
				iIcon = CECImageRes::IMG_TEAMMATE;

			if (iIcon >= 0)
			{
				pImageRes->GetImageItemSize(iIcon, 0, &cx, &cy);
				cx = int(cx * fScaleName);
				cy = int(cy * fScaleName);
				pImageRes->RegisterDraw(iIcon, tx, y, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx, cy);
				tx += cx;
			}
		}

		//	Draw PK state flag
		if (m_pvp.bInPVPCombat)
		{
			pImageRes->GetImageItemSize(CECImageRes::IMG_PKSTATE, 0, &cx, &cy);
			cx = int(cx * fScaleName);
			cy = int(cy * fScaleName);
			pImageRes->RegisterDraw(CECImageRes::IMG_PKSTATE, tx, y, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx, cy);
			tx += cx;
		}

		//	Draw GM flag
		if (IsGM())
		{
			pImageRes->GetImageItemSize(CECImageRes::IMG_GMFLAG, 0, &cx, &cy);
			cx = int(cx * fScaleName);
			cy = int(cy * fScaleName);
			pImageRes->RegisterDraw(CECImageRes::IMG_GMFLAG, tx, y, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx, cy);
			tx += cx;
		}
	}
	
	if (m_idOrgServer && m_pPateFaction && (dwFlags & RNF_SERVER) )
	{
		if (!m_pPateFaction->GetItemNum())
		{
			CECGame::SERVERINFO* pInfo = g_pGame->GetSrcServerInfo(m_idOrgServer);
			if (pInfo)
			{
				ACString servername;
				servername.Format(_AL("[%s]"), AS2AC(pInfo->szServerName) );
				m_pPateFaction->SetText(servername, false);	
			}
		}

		if (m_pPateFaction->GetItemNum())
		{
			//	Get text size
			m_pPateFaction->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);

			y -= cy+2;
			x = m_PateContent.iBaseX - (cx >> 1);
			m_pPateFaction->RegisterRender(x, y, m_idOrgServer == g_pGame->GetGameRun()->GetHostPlayer()->GetSrcServerID() ? A3DCOLORRGBA(255, 0, 255, 255) : A3DCOLORRGBA(0, 255, 255, 255), m_PateContent.z, m_PateContent.rhw);
		}
	}

	//	Does player join a faction ?
	if (m_idOrgServer == 0 && m_idFaction && m_pPateFaction && (dwFlags & RNF_FACTION))
	{
		//	Check faction name
		if (!m_pPateFaction->GetItemNum())
		{
			Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(m_idFaction);
			if (!pInfo)
			{
				g_pGame->GetFactionMan()->GetFaction(m_idFaction, !m_bRequestFactionName);
				m_bRequestFactionName = true;
			}
			if (pInfo)
			{
				ACString strText;
				_BuildPateFactionText(m_idFaction, m_idFRole, strText);
				m_pPateFaction->SetText(strText, false, false);
			}
		}

		//	Draw faction icon and name
		if (m_pPateFaction->GetItemNum())
		{
			//	Get text size
			m_pPateFaction->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);

			//	Get icon size
			int cx1=0, cy1=0, cx0=0, cy0=0;
			int star = 0;

			Faction_Info* pFaction = g_pGame->GetFactionMan()->GetFaction(m_idFaction);
			if (pFaction && pFaction->GetNimbusStar())
			{
				star = pFaction->GetNimbusStar()-1+CECImageRes::IMG_FACTION_NIMBUS_1;
				pImageRes->GetImageItemSize(star, 0, &cx0, &cy0);
				cx0 = int(cx0 * fScale);
				cy0 = int(cy0 * fScale);
			}

			if (m_pFactionDecal)
				m_pFactionDecal->GetExtent(&cx1, &cy1);

			cx1 = int(cx1 * fScale);
			cy1 = int(cy1 * fScale);

			//	Reset line height
			int ty, lh = a_Max(cy, cy1, cy0);	//	Line height

			//	Calculate start position
			x = m_PateContent.iBaseX - ((cx + cx1 + cx0) >> 1);
			y -= lh + 2;

			if (star)
			{
				pImageRes->RegisterDraw(star, x, y + ((lh - cy0) >> 1) + 1, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx0, cy0);
				x += cx0 + 2;
			}

			//	Draw faction name
			ty = y + ((lh - cy) >> 1);
			m_pPateFaction->RegisterRender(x, ty, m_idFaction == g_pGame->GetGameRun()->GetHostPlayer()->GetCombatFaction() ? A3DCOLORRGBA(255, 0, 255, 255) : A3DCOLORRGBA(0, 255, 255, 255), m_PateContent.z, m_PateContent.rhw);
			x += cx + 2;

			if (m_pFactionDecal)
			{
				//	Draw faction icon
				ty = y + ((lh - cy1) >> 1);
				m_pFactionDecal->SetZValue(m_PateContent.z, m_PateContent.rhw);
				m_pFactionDecal->SetScreenPos(x, ty);
				m_pFactionDecal->SetScaleX(fScale);
				m_pFactionDecal->SetScaleY(fScale);
				//m_pFactionDecal->Render(pViewport);
				g_pGame->GetGameRun()->GetWorld()->GetDecalMan()->RegisterDecal(m_pFactionDecal);
			}
		}
	}

	//	Draw family name
	if (m_idOrgServer == 0 && m_bRenderFamily && m_idFamily && m_pPateFamily && (dwFlags & RNF_FAMILY))
	{
		if (!m_pPateFamily->GetItemNum())
		{
			Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(m_idFamily, !m_bRequestFamilyName, true);
			m_bRequestFamilyName = true;

			if (pInfo)
				m_pPateFamily->SetText(pInfo->GetName(), false, false);
		}

		if (m_pPateFamily->GetItemNum() > 0)
		{
			Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(m_idFamily, false, true);
			m_pPateFamily->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);

			//	Get icon size
			int cx1=0, cy1=0;
			int star = 0;

			if (pInfo && pInfo->GetLevel())
			{
				star = pInfo->GetLevel()-1+CECImageRes::IMG_FAMILY_RECORD_1;
				pImageRes->GetImageItemSize(star, 0, &cx1, &cy1);
				cx1 = int(cx1 * fScale);
				cy1 = int(cy1 * fScale);
			}

			int lh = a_Max(cy, cy1);
			y -= lh + 2;
			x = m_PateContent.iBaseX - ((cx + cx1) >> 1);

			if (star)
			{
				pImageRes->RegisterDraw(star, x, y + ((lh - cy1) >> 1) + 1, 0xffffffff, 0, m_PateContent.z, m_PateContent.rhw, cx1, cy1);
				x += cx1 + 2;
			}

			m_pPateFamily->RegisterRender(x, y + ((lh - cy) >> 1), A3DCOLORRGBA(19, 169, 255, 255), m_PateContent.z, m_PateContent.rhw);
		}
	}

	//	Draw title
	if (m_pPateTitle && (dwFlags & RNF_TITLE) && m_pPateTitle->GetItemNum() > 0)
	{
		m_pPateTitle->GetExtent(&cx, &cy);
		cx = int(cx * fScale);
		cy = int(cy * fScale);
		
		int cx1, cy1;
		
		if (m_pTitleDecal)
		{
			m_pTitleDecal->GetExtent(&cx1, &cy1);

			cx1 = int(cx1 * fScale);
			cy1 = int(cy1 * fScale);
		}
		
		if(m_iTitleState == TITLE_NONE)
		{
			y -= cy + 2;
			x = m_PateContent.iBaseX - (cx >> 1);
			m_pPateTitle->RegisterRender(x, y, m_colorTitle, m_PateContent.z, m_PateContent.rhw);
		}
		else
		{
			if (m_pTitleDecal)
			{
				x = m_PateContent.iBaseX - (cx1 >> 1);
				y -= cy1 + 2;
				m_pTitleDecal->SetZValue(m_PateContent.z, m_PateContent.rhw);
				m_pTitleDecal->SetScreenPos(x, y);
				m_pTitleDecal->SetScaleX(fScale);
				m_pTitleDecal->SetScaleY(fScale);
				//m_pTitleDecal->Render(pViewport);

				g_pGame->GetGameRun()->GetWorld()->GetDecalMan()->RegisterDecal(m_pTitleDecal);

				if(m_iBattleKill > 0 && m_pKillNum)
				{
					
					m_pKillNum->GetExtent(&cx, &cy);
					cx = int(cx * fScale);
					cy = int(cy * fScale);
					ACString strText;
					strText.Format(_AL("%d"), m_iBattleKill);
					m_pKillNum->SetText(strText,false);
					int nPos = y - 8;
					nPos += cy1 / 2 ;
					m_pKillNum->RegisterRender(m_PateContent.iBaseX - ((cx) >> 1),nPos, 0xffffffff, m_PateContent.z, m_PateContent.rhw);
					
				}
			}
			if(m_iTitleState == TITLE_IMAGE_TEXT || m_iTitleState == TITLE_ANIM_TEXT)
			{
				x = m_PateContent.iBaseX - (cx >> 1);
				m_pPateTitle->RegisterRender(x, y, m_colorTitle, m_PateContent.z, m_PateContent.rhw);
			}
		}
	}
	
	if (m_idOrgServer == 0 && m_idCircle && m_pPateCircle && (dwFlags & RNF_CIRCLE))
	{
		//	Check faction name
		if (!m_pPateCircle->GetItemNum())
		{
			Circle_Info* pInfo = g_pGame->GetCircleMan()->GetCircle(m_idCircle);
			if (!pInfo)
			{
				g_pGame->GetCircleMan()->GetCircle(m_idCircle, !m_bRequestCircleName);
				m_bRequestCircleName = true; 
			}	
			if (pInfo)
			{	
				ACString strText;
				_BuildPateCircleText(m_idCircle, m_idCRole, strText);
				m_pPateCircle->SetText(strText, false, false);
			}
		}

		//	Draw faction icon and name
		if (m_pPateCircle->GetItemNum())
		{
			//	Get text size
			m_pPateCircle->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);

			y -= cy+2;
			x = m_PateContent.iBaseX - (cx >> 1);
			m_pPateCircle->RegisterRender(x, y, 0xffffffff, m_PateContent.z, m_PateContent.rhw);
		}
	}

	if (m_pPateCityOwner)
	{
		bool bOwnerFaction;

		if (m_idFaction && m_idFaction == g_pGame->GetGameRun()->GetCityOwnerFaction())
			bOwnerFaction = true;
		else
			bOwnerFaction = false;

		if (bOwnerFaction)
		{
			if (m_pPateCityOwner->GetItemNum() == 0)
			{
				CECStringTab* pFixStr = g_pGame->GetFixedMsgTab();
				const ACHAR* szTitle;

				switch (m_idFRole)
				{
				case GNET::_R_MEMBER:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_MEMBER);
					m_clCityOwner = 0xff00c60a;
					break;
				case GNET::_R_MASTER:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_MASTER);
					m_clCityOwner = 0xff000000;
					break;
				case GNET::_R_VICEMASTER:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_VICEMASTER);
					m_clCityOwner = 0xffff0000;
					break;
				case GNET::_R_BODYGUARD:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_BODYGUARD);
					m_clCityOwner = 0xffff5587;
					break;
				case GNET::_R_HEADER:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_HEADER);
					m_clCityOwner = 0xff0096ff;
					break;
				default:
					szTitle = pFixStr->GetWideString(FIXMSG_CITYOWNER_MEMBER);
					m_clCityOwner = 0xff00c60a;
					break;
				}

				m_pPateCityOwner->SetText(szTitle, false, false);
			}
		}
		else if (m_pPateCityOwner->GetItemNum() > 0)
			m_pPateCityOwner->Clear();

		if (m_pPateCityOwner->GetItemNum() > 0)
		{
			m_pPateCityOwner->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			y -= cy + 2;
			x = m_PateContent.iBaseX - (cx >> 1);
			m_pPateCityOwner->RegisterRender(x, y, m_clCityOwner, m_PateContent.z, m_PateContent.rhw);
		}
	}

	if (dwFlags & RNF_WORDS)
	{
		//	Draw last said words, line 2 first
		if (m_pPateLastWords2 && m_pPateLastWords2->GetItemNum())
		{
			m_pPateLastWords1->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			y -= cy + 2;
			x = m_PateContent.iBaseX - (cx >> 1);
		//	m_pPateLastWords2->Render(pViewport, x, y, 0xffffffff, m_PateContent.z);
			m_pPateLastWords2->RegisterRender(x, y, 0xffffffff, m_PateContent.z, m_PateContent.rhw);
		}

		//	Last said words, line 1
		if (m_pPateLastWords1 && m_pPateLastWords1->GetItemNum())
		{
			m_pPateLastWords1->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			y -= cy + 2;
			x = m_PateContent.iBaseX - (cx >> 1);
		//	m_pPateLastWords1->Render(pViewport, x, y, 0xffffffff, m_PateContent.z);
			m_pPateLastWords1->RegisterRender(x, y, 0xffffffff, m_PateContent.z, m_PateContent.rhw);
		}
	}

	//	Draw trade words / team requirement
	if (m_iBoothState == 2 && ((dwFlags & RNF_BUY) || (dwFlags & RNF_SELL)))
	{
	//	if (m_pPateBooth && m_pPateBooth->GetItemNum())
	//		y = RenderBoothName(pViewport, y);
		if (m_pPateBooth && m_pPateBooth->GetItemNum())
		{
			m_pPateBooth->GetExtent(&cx, &cy);
			cx = int(cx * fScale);
			cy = int(cy * fScale);
			y -= cy + 2;
			x = m_PateContent.iBaseX - (cx >> 1);
		//	m_pPateBooth->Render(pViewport, x, y, A3DCOLORRGB(255, 156, 0), m_PateContent.z);
			m_pPateBooth->RegisterRender(x, y, A3DCOLORRGB(255, 156, 0), m_PateContent.z, m_PateContent.rhw);
		}
	}
	else if (m_TeamReq.bShowReq && m_pPateTeamReq && m_pPateTeamReq->GetItemNum())
	{
		m_pPateTeamReq->GetExtent(&cx, &cy);
		cx = int(cx * fScale);
		cy = int(cy * fScale);
		y -= cy + 2;
		x = m_PateContent.iBaseX - (cx >> 1);
	//	m_pPateTeamReq->Render(pViewport, x, y, A3DCOLORRGB(255, 228, 0), m_PateContent.z);
		m_pPateTeamReq->RegisterRender(x, y, A3DCOLORRGB(255, 228, 0), m_PateContent.z, m_PateContent.rhw);
	}

	m_PateContent.iCurY	= y;

	return true;
}

//	Render booth name
int CECPlayer::RenderBoothName(CECViewport* pViewport, int y)
{
	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	float fScale = pUIMan->GetWindowScale();

	int cx, cy, cx1, cy1;
	m_pPateBooth->GetExtent(&cx, &cy);
	cx = int(cx * fScale);
	cy = int(cy * fScale);

	//	Draw background
	cx1 = cx + 10;
	cy1 = cy + 10;
	
	y -= cy1 + 2;
	int iNewY = y;
	int x = m_PateContent.iBaseX - (cx1 >> 1);

	float z = m_PateContent.z + 0.00001f;
	
	CECImageRes* pImageRes = g_pGame->GetImageRes();
	DWORD dwCol = 0xffffffff;

	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x, y, dwCol, 0, z, m_PateContent.rhw, 5, 5);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5, y, dwCol, 1, z, m_PateContent.rhw, cx, 5);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5+cx, y, dwCol, 2, z, m_PateContent.rhw, 5, 5);

	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x, y+5, dwCol, 3, z, m_PateContent.rhw, 5, cy);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5, y+5, dwCol, 4, z, m_PateContent.rhw, cx, cy);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5+cx, y+5, dwCol, 5, z, m_PateContent.rhw, 5, cy);

	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x, y+5+cy, dwCol, 6, z, m_PateContent.rhw, 5, 5);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5, y+5+cy, dwCol, 7, z, m_PateContent.rhw, cx, 5);
	pImageRes->DrawImage(CECImageRes::IMG_BOOTHBAR, x+5+cx, y+5+cy, dwCol, 8, z, m_PateContent.rhw, 5, 5);

	//	Draw text
	y += 5;
	x = m_PateContent.iBaseX - (cx >> 1);
//	m_pPateBooth->Render(pViewport, x, y, A3DCOLORRGB(255, 203, 74), m_PateContent.z);
	m_pPateBooth->RegisterRender(x, y, A3DCOLORRGB(255, 203, 74), m_PateContent.z, m_PateContent.rhw);

	return iNewY;
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

		if(m_bBloodPool && m_iCurBloodPool && m_iMaxBloodPool)
		{
			int iLen = (int)ceil(cx * ((float)m_iCurBloodPool / m_iMaxBloodPool));
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
	else if(m_iBoothState == 2 && m_idBoothItem != -1)
	{
		A3DAABB aabb;
		if(m_pSpeBoothModel)
		{
			if(m_pSpeBoothModel->HasCHAABB())
			{
				aabb.Center = GetPos() + g_vAxisY * m_pSpeBoothModel->GetCHAABB().Center.y;
				aabb.Extents = m_pSpeBoothModel->GetCHAABB().Extents;
				aabb.CompleteMinsMaxs();	
			}
			else
				aabb = m_pSpeBoothModel->GetModelAABB();
			vPos.y = aabb.Center.y + aabb.Extents.y * 2.1f;
		}
	}
	else if (GetGender() == 0)
	{
		if (GetPhysique() == PHYSIQUE_NORMAL || m_iChangeShape == SHAPE_HUMAN)
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
		else if(GetPhysique() == PHYSIQUE_TINY)
		{
			vPos = vPos + g_vAxisY * 1.2f;
		}
		else if(GetPhysique() == PHYSIQUE_TETRAPOD)
		{
			vPos = vPos + g_vAxisY * 3.0f;
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
		else if(GetPhysique() == PHYSIQUE_TINY)
		{
			vPos = vPos + g_vAxisY * 1.0f;
		}
	}	

	if (m_bHangerOn) // 女被抱 或骑乘麒麟
	{
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);

		if (pBuddy)
		{
			if(m_AttachMode == enumAttachRideOnQilin)
			{
				vPos.y += 1.15f;
			}
			else
			{
				vPos.y += .15f;

				if (pBuddy->m_pPetModel)
					vPos.y += pBuddy->m_fPetHeight;
			}
		}
	}
	else
	{
		if (m_pPetModel)
		{
			vPos.y += m_fPetHeight;
			if(m_iPhysique == PHYSIQUE_TINY)
				vPos.y += .2f;
		}

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
		else if(m_iPhysique == PHYSIQUE_TETRAPOD)
			vPos = vPos + g_vAxisY * 1.6f;
		else if(m_iPhysique == PHYSIQUE_TINY)
			vPos = vPos + g_vAxisY * 0.5f;
	}

	//太昊的归神与脱灵技能下，在角色体积变大/变小后需要改变头顶文字的位置Added 2011-06-23.
	if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) )
	{
//		float fDelta = ( m_fScaleShapeRatio > 1.0f ) ? 1.35f : -1.35f;
		vPos += A3DVECTOR3(0.0f, (m_fScaleShapeRatio - 1.0f)*(m_aabb.Extents.y+0.14f)*2.0f/*+fDelta*/, 0.0f);
	}


    //搓澡或互动时
	{
		if( (m_AttachMode == enumAttachBathPlayer || m_InteractorInfo.id != 0  || m_iPuppetId != 0) && m_pBackupModel) 			
		{
			A3DSkinModel* pModel = GetA3DSkinModel();
			if(pModel && pModel->GetSkeleton())
			{
				int index;
				A3DBone* pHeadBone = pModel->GetSkeleton()->GetBone("Bip01 Head", &index);
				if( pHeadBone )
				{
					A3DMATRIX4 matHead = pHeadBone->GetAbsoluteTM();
					vPos = matHead.GetRow(3);
					vPos = vPos + g_vAxisY * 0.5f;
				}
			}
		}
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
	if (m_pChangedModel && m_pChangedModel->GetA3DSkinModel())
		return m_pChangedModel->GetA3DSkinModel()->GetModelAABB();
	else if (m_pPlayerModel && m_pPlayerModel->GetA3DSkinModel())
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
			if(IsSkillFlying())
				iAction = CECPlayer::ACT_FLY_WING;
			else
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
			if(IsSkillFlying())
				iAction = CECPlayer::ACT_HANGINAIR_WING;
			else
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
void CECPlayer::SetLastSaidWords(const ACHAR* szWords, int nEmotionSet, CECIvtrItem* pItem, sAchievement* pAchiv)
{
	if (!m_pPateLastWords1 || !m_pPateLastWords2)
	{
		delete pItem;
		delete pAchiv;
		return;
	}
	
	
	ACString str = FilterEmotionSet(szWords, nEmotionSet);
	szWords = str;
	str = FilterCoordColor(szWords, A3DCOLORRGBA(108, 251, 75,  255));
	szWords = str;
	str = FilterCoordString(szWords);
	szWords = str;

	m_pPateLastWords1->SetText(szWords, true, true, pItem, pAchiv);
	m_pPateLastWords2->Clear();
	m_strLastSayCnt.Reset();
}

//	Set faction ID
void CECPlayer::SetFactionID(int id)
{
	m_idFaction = id;

	if (m_pPateCityOwner)
		m_pPateCityOwner->Clear();

	if (!m_idFaction)
		return;

	//	Clear pate faction text, it will be rebuilt in RenderName()
	if (m_pPateFaction)
		m_pPateFaction->Clear();

	//	Reset faction icon
	if (!m_pFactionDecal)
		m_pFactionDecal = new CECSpriteDecal;

	int iIndex;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	A2DSprite* pIcon = pGameUI->GetFactionIcon(g_pGame->GetGameInit().iServerID, m_idFaction, &iIndex);
	m_pFactionDecal->ChangeSpriteIcon(pIcon, iIndex);
}

//	Set family ID
void CECPlayer::SetFamilyID(int id)
{
	m_idFamily = id;

	if (m_pPateFamily)
		m_pPateFamily->Clear();
}

void CECPlayer::SetTitle(int iTitle)
{
	// first see if the title contain ext props, if so we need update player's ext props from the server
	if( g_pGame->GetTitlePropTable().find(m_iTitle) != g_pGame->GetTitlePropTable().end() ||
		g_pGame->GetTitlePropTable().find(iTitle) != g_pGame->GetTitlePropTable().end() )
	{
		g_pGame->GetGameSession()->c2s_CmdGetExtProps();
	}

	m_iTitle = iTitle;
	m_iTitleState = TITLE_NONE;
	m_bUpdateFrameCount = false;
	
	if (m_pPateTitle)
	{
		m_pPateTitle->Clear();
		
		if (m_iTitle > 0)
		{
			m_bPateNeedUpdate = true;
			ACString title = _AL("......");

			// see if is a special title
			if( m_iTitle < 1000 )
			{
				if( m_iTitle == 1 ) // 威望固定称谓
				{
					int level = GetReputationLevel(GetReputation());
					title = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetStringFromTable(level + 1100);
				}
			}
			else // 普通称谓，从策划的表里取名字
			{
				CConfigFromLua * pConfigFromLua = g_pGame->GetConfigFromLua();
				if( pConfigFromLua )
				{
					CConfigFromLua::TitleDefTab::iterator it = pConfigFromLua->m_TitleDef.find(m_iTitle);
					if( it != pConfigFromLua->m_TitleDef.end() )
					{
						title = *it.value();
						if( !FormatTitle(title) )
							return;
					}
				}
			}

			// see if it begins with a color definition
			m_colorTitle = 0xffffffff;
			if( title.GetLength() >= 7 )
			{
				if( title[0] == _AL('^') )
				{
					ACString szColor = title.Left(7);
					a_sscanf(szColor, _AL("^%x"), &m_colorTitle);
					m_colorTitle |= 0xff000000;
					title = title.Right(title.GetLength() - 7);
				}
			}
			if(title.Find(_AL("&image")) >=0)
				m_iTitleState = TITLE_IMAGE_ONLY;
			else if(title.Find(_AL("#image")) >= 0)
				m_iTitleState = TITLE_IMAGE_TEXT;
			else if(title.Find(_AL("&anim")) >= 0)
				m_iTitleState = TITLE_ANIM_ONLY;
			else if(title.Find(_AL("#anim")) >= 0)
				m_iTitleState = TITLE_ANIM_TEXT;
				
			if(m_iTitleState)
			{
				int i = 0;

				while(title[i] != _AL('#') && title[i] != _AL('&')) i++;
				int start, end;
				start = title.Find(_AL('('));
				end = title.Find(_AL(')'));
				ACString imagefile = title.Mid(start + 1, end - start - 1);
				AString imagefile1 = AC2AS(imagefile);
				if(m_iTitleState == TITLE_ANIM_ONLY || m_iTitleState == TITLE_ANIM_TEXT)
				{
					start = title.Find(_AL('<'));
					end = title.Find(_AL('>'));
					ACString frametime = title.Mid(start + 1, end - start -1);
					if(frametime.ToInt() > 0)
					m_TitleFrameCount.SetPeriod(frametime.ToInt());
					
					start = title.Find(_AL('['));
					end = title.Find(_AL(']'));
					ACString framenum = title.Mid(start + 1, end - start -1);
					m_TitleInfo.iTitleFrameNum = framenum.ToInt();
				}

				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				if(!m_pTitleDecal)
					m_pTitleDecal = new CECSpriteDecal;

				if(m_iTitleState == TITLE_IMAGE_ONLY || m_iTitleState == TITLE_IMAGE_TEXT)
				{
					A2DSprite* pSprite = pGameUI->LoadTitleImage(imagefile1, 1);
					if(pSprite)
						m_pTitleDecal->ChangeSpriteIcon(pSprite, 0);
					else
						m_iTitleState = TITLE_NONE;
				}
				else if(m_iTitleState == TITLE_ANIM_ONLY || m_iTitleState == TITLE_ANIM_TEXT)
				{
					if(m_TitleInfo.iTitleFrameNum >= 0)
					{
						A2DSprite* pSprite = pGameUI->LoadTitleImage(imagefile1, m_TitleInfo.iTitleFrameNum);
						if(pSprite)
						{
							m_pTitleDecal->ChangeSpriteIcon(pSprite, 0);
							m_TitleInfo.iCurrentframe = 0;
							m_TitleInfo.pSprite = pSprite;
							m_bUpdateFrameCount = true;
						}
						else
							m_iTitleState = TITLE_NONE;
					}
					else 
						m_iTitleState = TITLE_NONE;						
				}
				title = title.Left(i);
			}

			ACString szTitle;
			szTitle.Format(_AL("<%s>"), title);
			m_pPateTitle->SetText(szTitle, false, false);
			m_bPateNeedUpdate = false;
		}
	}
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
			if( GetSpouse() )
			{
				const wchar_t * szSpouseName = g_pGame->GetGameRun()->GetPlayerName(GetSpouse(), false);
				if( !szSpouseName )
				{
					static DWORD timeLast = 0;
					if( timeLast + 1000 < timeGetTime() )
					{
						int idPlayer = GetSpouse();
						g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
						timeLast = timeGetTime();
					}
					title = strLeft + _AL("XXXX") + strRight;
					nStart = strLeft.GetLength() + 4;
					return false;
				}
				title = strLeft + szSpouseName + strRight;
				nStart = strLeft.GetLength() + a_strlen(szSpouseName);
			}
			else
			{
				title = strLeft + _AL("XXXX") + strRight; // 《武林外传》二次修订文档之婚姻系统P3.3中特殊要求这样的
				nStart = strLeft.GetLength() + 4;
			}
			break;

		case L'T':
			if( GetMaster() )
			{
				const wchar_t * szMasterName = g_pGame->GetGameRun()->GetPlayerName(GetMaster(), false);
				if( !szMasterName )
				{
					static DWORD timeLast = 0;
					if( timeLast + 1000 < timeGetTime() )
					{
						int idPlayer = GetMaster();
						g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
						timeLast = timeGetTime();
					}
					title = strLeft + _AL("XXXX") + strRight;
					nStart = strLeft.GetLength() + 4;
					return false;
				}
				title = strLeft + szMasterName + strRight;
				nStart = strLeft.GetLength() + a_strlen(szMasterName);
			}
			else
			{
				title = strLeft + _AL("XXXX") + strRight; // 《武林外传》二次修订文档之婚姻系统P3.3中特殊要求这样的
				nStart = strLeft.GetLength() + 4;
			}
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

	//	Clear pate faction text, it will be rebuilt in RenderName()
	if (m_pPateFaction)
		m_pPateFaction->Clear();

	if (m_pPateCityOwner)
		m_pPateCityOwner->Clear();
}

//	On start binding buddy
void CECPlayer::OnStartBinding(int idMule, int idRider,bool isBath /*= false*/)
{
	m_bCandHangerOn = (idRider == GetCharacterID()) ? true : false;
	if(isBath)
       m_idCandBathBuddy = m_bCandHangerOn ? idMule : idRider;
	else
	   m_idCandBuddy = m_bCandHangerOn ? idMule : idRider;
}

void CECPlayer::OnStartQiLinBinding(int idInvitor, int idInvitee)
{
	m_bCandHangerOn = (idInvitee == GetCharacterID()) ? true : false;
	m_idCandQilinBuddy = m_bCandHangerOn ? idInvitor : idInvitee;
	m_bQilinConnectHangerOn = m_bCandHangerOn;
	m_idQilinConnectBuddy = m_idCandQilinBuddy;
}


//	Set booth name
void CECPlayer::SetBoothName(const ACHAR* szName)
{
	m_strBoothName = szName;
	if (m_pPateBooth)
		m_pPateBooth->SetText(szName, false, false);
}

//	Is specified id a member of our team ?
bool CECPlayer::IsTeamMember(int idPlayer)
{
	if (!m_pTeam)
		return false;

	return m_pTeam->GetMemberByID(idPlayer) ? true : false;
}

//	Set new visible extend states
void CECPlayer::SetNewExtendStates(__int64 dwNewStates)
{
	static const char* szBasePath = "策划联入\\状态效果\\";

	if (IsAllResReady() && m_pBackupModel)
	{
		AString strGFXFile;

		for (int i=0; i < 64; i++)
		{
			__int64 temp = 1;
			__int64 dwMask = temp << i;
			__int64 dwFlag1 = m_dwExtStates & dwMask;
			__int64 dwFlag2 = dwNewStates & dwMask;

			if ((!dwFlag1 && !dwFlag2) || (dwFlag1 && dwFlag2))
				continue;

			const GNET::VisibleState* pvs = GNET::VisibleState::Query(m_BasicProps.iProfession, i);
			if (!pvs)
				continue;

			strGFXFile = pvs->GetEffect();
			if (!strGFXFile.GetLength())
				continue;

			strGFXFile = szBasePath + strGFXFile;

			if (dwFlag1)
			{
				//	Remove old state
				m_pBackupModel->RemoveGfx(strGFXFile, pvs->GetHH());

				if (m_pDummyModel)
					m_pDummyModel->RemoveGfx(strGFXFile, pvs->GetHH());
			}
			else
			{
				//	Add new state
				const char* szHH = pvs->GetHH();
				float fScale;

				if (stricmp("HH_Head", szHH) == 0)
					fScale = m_pBackupModel->GetOuterData()[0];
				else if (stricmp("HH_Spine", szHH) == 0)
					fScale = m_pBackupModel->GetOuterData()[1];
				else
					fScale = 1.0f;

				m_pBackupModel->PlayGfx(strGFXFile, pvs->GetHH(), fScale);

				if (m_pDummyModel)
					m_pDummyModel->PlayGfx(strGFXFile, pvs->GetHH(), fScale);
			}
		}
	}

	m_dwExtStates = dwNewStates;
}

void CECPlayer::SetDirVisibleStates( int iVisibleStateID, int iDir, bool bOn )
{
	if ( 1 )	//IsAllResReady() && m_pBackupModel
	{
		if ( bOn )	//开启状态
		{
			if ( !m_pDirVisibleStateGFX )	//如果不存在，则加载
			{
				m_pDirVisibleStateGFX = g_pGame->GetGFXCaster()->LoadGFXEx( res_VisibleStateGFXFile(iVisibleStateID) );
			}

			if ( m_pDirVisibleStateGFX )
			{
				A3DVECTOR3 vRight, vNormal = g_vAxisY;
				vRight.CrossProduct(vNormal, g_vAxisX);
				vRight.Normalize();

				A3DMATRIX4 mat = a3d_TransformMatrix(vRight, vNormal, GetPos() + vNormal * 0.2f);				

				float fRot = 3.1415926f * ( (float)iDir / 360.0f );
				A3DMATRIX4 matRot = a3d_RotateY( fRot );

				mat *= matRot;
				float dh = GetPos().y - g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetPosHeight(GetPos());
				if( dh < 0.05f )
				{
					m_pDirVisibleStateGFX->SetMatchGround(true);
				}
				else
				{
					m_pDirVisibleStateGFX->SetMatchGround(false);
				}

				m_pDirVisibleStateGFX->SetParentTM(mat);
				m_pDirVisibleStateGFX->Start(true);
			}
		}
		else		//关闭状态
		{
			if ( m_pDirVisibleStateGFX )
			{
				m_pDirVisibleStateGFX->Stop();
				g_pGame->GetGFXCaster()->ReleaseGFXEx( m_pDirVisibleStateGFX );
				m_pDirVisibleStateGFX = NULL;
			}
		}
	}
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

	if (m_pPateTeamReq)
	{
		if (m_strTeamReq.GetLength())
			m_pPateTeamReq->SetText(m_strTeamReq, false, true);
		else
			m_pPateTeamReq->Clear();
	}

	//	If this is host player, notify server
	if (bConfirm && IsHostPlayer())
	{
		//	Compress data
		int iData1=0, iData2=0;

		if (Req.bShowReq)
			iData1 |= (1 << 31);

		if (Req.bShowLevel)
		{
			iData1 |= (1 << 30);
			iData1 |= (Req.iLevel << 8);
			iData1 |= Req.iProfession;
		}

		iData1 |= (Req.iType << 16);

		g_pGame->GetGameSession()->c2s_CmdSetAdvData(iData1, iData2);
	}
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
	using namespace S2C;

	if (Msg.dwParam2 == UPDATE_EXT_STATE)
	{
		cmd_update_ext_state* pCmd = (cmd_update_ext_state*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);
		SetNewExtendStates(pCmd->dwStates);
		ResetPlayLuteAction();
	}
	else if (Msg.dwParam2 == ICON_STATE_NOTIFY)
	{		
		cmd_icon_state_notify cmd;

		if (!cmd.FillData((void*)Msg.dwParam1, Msg.dwParam3))
			return;

		m_aIconStates.RemoveAll(false);

		if (cmd.count)
		{
			m_aIconStates.SetSize(cmd.count, 10);

			for (int i=0; i < cmd.count; i++)
			{
				BUFFINFO& bi = m_aIconStates[i];
				bi.id		= cmd.buff_list[i].state;
				bi.iLevel	= cmd.buff_list[i].level;
				bi.iEndTime	= 0;
				bi.iOverlayCnt = cmd.buff_list[i].overlay_cnt;
			}
//			memcpy(m_aIconStates.GetData(), pCmd->state, pCmd->count * sizeof (WORD));

			//	Sort icons
//			qsort(m_aIconStates.GetData(), m_aIconStates.GetSize(), sizeof (WORD), glb_WordCompare);
		}
	}
}

void CECPlayer::OnMsgEnchantResult(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_enchant_result* pCmd = (cmd_enchant_result*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->caster == m_PlayerInfo.cid);

	if (m_pPlayerModel)
		m_pPlayerModel->AddOneAttackDamageData(
			ACT_CHANNEL_BODY,
			pCmd->caster,
			pCmd->target,
			pCmd->attack_stamp,
			pCmd->modifier | MOD_SKILL,
			GNET::ElementSkill::GetType(pCmd->skill) == GNET::TYPE_ATTACK ? -1 : -2);

	if (pCmd->damage)
	{
		if (pCmd->target == m_PlayerInfo.cid)
			BubbleText(BUBBLE_HEAL, pCmd->damage);
		else
		{
			CECPlayer* pTarget = m_pPlayerMan->GetPlayer(pCmd->target);

			if (pTarget)
				pTarget->BubbleText(BUBBLE_HEAL, pCmd->damage);
		}
	}

	/*
	char mask = (char)0xff;
	// we should filter out these things that will cause bubble texts
	if (pCmd->target != m_pPlayerMan->GetHostPlayer()->GetCharacterID() &&
		!IsHostPlayer())
		mask &= MOD_PHYSIC_ATTACK_RUNE | MOD_MAGIC_ATTACK_RUNE | MOD_CRITICAL_STRIKE;

	if( GNET::ElementSkill::GetType(pCmd->skill) == GNET::TYPE_ATTACK )
	{
		// only attack skill will cause wounded action, when damage is -1
		PlayAttackEffect(pCmd->target, pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, pCmd->modifier & mask);
	}
	else
	{
		// other skills will not cause wounded action, so we set damage to -2 which will be discarded to play wounded action
		PlayAttackEffect(pCmd->target, pCmd->skill, pCmd->level, pCmd->attack_stamp, -2, pCmd->modifier & mask);
	}
	*/
}

void CECPlayer::OnMsgPlayerAdvData(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == PLAYER_SET_ADV_DATA)
	{
		cmd_player_set_adv_data* pCmd = (cmd_player_set_adv_data*)Msg.dwParam1;
		DecompAdvData(pCmd->data1, pCmd->data2);
	}
	else
	{
		ASSERT(Msg.dwParam2 == PLAYER_CLR_ADV_DATA);
		cmd_player_clr_adv_data* pCmd = (cmd_player_clr_adv_data*)Msg.dwParam1;

		m_TeamReq.bShowReq = false;
	}
}

void CECPlayer::OnMsgPlayerPVP(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case PLAYER_PVP_NO_PROTECT:	m_pvp.bEnable = true;	break;
	case PLAYER_DISABLE_PVP:	m_pvp.bEnable = false;	break;

	case PVP_COMBAT:
	{
		cmd_pvp_combat* pCmd = (cmd_pvp_combat*)Msg.dwParam1;
		m_pvp.bInPVPCombat = pCmd->state ? true : false;
		break;
	}
	}

	if (GetCloneID())
	{
		CECPlayer* pClone = m_pPlayerMan->GetPlayer(GetCloneID());
		if (pClone)
		{
			pClone->m_pvp = m_pvp;
		}
	}
}

void CECPlayer::OnMsgSwitchFashionMode(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_enable_fashion* pCmd = (cmd_player_enable_fashion*)Msg.dwParam1;
	bool bFashion = pCmd->is_enabble ? true : false;

	if (m_bFashionMode != bFashion)
	{
		if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1 || m_iChangeShape == SHAPE_HUMAN)
		{
			m_bFashionModeBackUp = bFashion;
		}
		else
		{	
			m_bFashionMode = bFashion;
			// 如果切换到时装模式，暂时先不进行显示，因为现在的时装模式有多套时装候选
			// 切换到时装模式时，会出现切换到旧的时装备选，然后又立刻变换到新的时装
			// if(!m_bFashionMode)
				UpdateCurSkins();

			// update mirror models

			for(int i=0;i<6;i++)
			{
				if(m_MirrorModels[i])
				{
					m_MirrorModels[i]->SetFashionMode(m_bFashionMode);
				}
			}
		}
	}
	else
	{
		if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1 || m_iChangeShape == SHAPE_HUMAN)
		{
			m_dwHideEquipMaskBackUp = pCmd->fashion_mask;
			//	m_dwHideEquipMask = m_HideEquipMaskInArena;
		}
		else
		{
			//-----------------------------------------------------------------
			if ( (pCmd->fashion_mask & (1<<EQUIPIVTR_FASHION_EYE)) ^ (m_dwHideEquipMask & (1<<EQUIPIVTR_FASHION_EYE)) )	//轩辕职业需要隐藏面罩，也就是显示没有面罩的头盔
			{
				DWORD dwForceChangeMask = GetForceChangeMask();
				dwForceChangeMask |= ( 1<<EQUIPIVTR_HEAD );
				SetForceChangeMask( dwForceChangeMask );
			}
			//-----------------------------------------------------------------
			m_dwHideEquipMask = pCmd->fashion_mask;
			ShowEquipments(m_aEquips);
		}
	}
	
	if (m_pBackupModel && !(GetForceChangeMask() & (1<<EQUIPIVTR_HEAD)))	//轩辕职业隐藏面罩时，不需要再次调用换装备脚本，因为在ShowEquipments接口中已经完成了脚本调用
	{
		for (int i = 0; i < SIZE_EQUIPIVTR; i++)
		{
			if (m_aEquips[i] > 0)
				m_pBackupModel->OnScriptChangeEquip(m_aEquips[i], 1, m_bFashionMode, GetEquipPathId(m_aEquips[i], m_iGender, m_iPhysique));
		}
	}
}

void CECPlayer::OnMsgPlayerEffect(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == PLAYER_ENABLE_EFFECT)
	{
		cmd_player_enable_effect* pCmd = (cmd_player_enable_effect*)Msg.dwParam1;
		ApplyEffect(pCmd->effect, true);
	}
	else if (Msg.dwParam2 == PLAYER_DISABLE_EFFECT)
	{
		cmd_player_disable_effect* pCmd = (cmd_player_disable_effect*)Msg.dwParam1;
		DiscardEffect(pCmd->effect);
	}
}

void CECPlayer::OnMsgChangeNameColor(const ECMSG& Msg)
{
}

void CECPlayer::OnMsgPlayerMount(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_player_mounting* pCmd = (const cmd_player_mounting*)Msg.dwParam1;

	if (pCmd->mount_id)
	{
		RideOnPet(pCmd->mount_id, pCmd->mount_level,pCmd->mount_type);
		LuaEvent::FirstEquipHorse();
	}
	else
		GetOffPet();
}

void CECPlayer::OnMsgPlayerLevel2(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_task_deliver_level2* pCmd = (cmd_task_deliver_level2*)Msg.dwParam1;
}

/**
*@param:ECMSG,封装之后的消息体。
*@return:void
*@usage:处理gs发来的缩放角色大小的消息处理函数；目前用于太昊归神/脱灵技能的客户端效果
**/
void CECPlayer::OnMsgPlayerScaleShape(const ECMSG& Msg)
{
	using namespace S2C;
	
	//判断消息命令号
	if ( Msg.dwParam2 == PLAYER_SCALE )
	{
		const cmd_player_scale* pCmd = (const cmd_player_scale*)Msg.dwParam1;
		if ( !pCmd )
		{
			return;
		}

		float fScaleRatio = (float)pCmd->scale_ratio / 100.00f;
		if ( pCmd->switch_tag ) //停止缩放功能，还原
		{
			fScaleRatio	= 1.0f;
			m_fTransHeight /= m_fScaleShapeRatio;
			m_fScaleShapeRatio = 0.0f;
		}
		else	//进行缩放操作
		{
			m_fScaleShapeRatio = fScaleRatio;
			m_fTransHeight *= m_fScaleShapeRatio;
		}

		//处理缩放/还原
		ScaleObjects( m_pPlayerModel, fScaleRatio );

		//如果此时有坐骑，则缩放/还原坐骑 Added 2011-08-17.
		if ( m_pPetModel )
		{
			ScaleObjects( m_pPetModel, fScaleRatio );
		}

		//	ScalePlayerAABB(1.5f);
		//	m_pCDRInfo->vExtent *= 1.5f;
		if ( pCmd->switch_tag ) // 还原后，模型位置又有了改变，需要再次还原位置。
		{
			if(m_pPlayerModel)
				m_pPlayerModel->SetPos(GetPos() /*+ A3DVECTOR3(0.0f, (1.0f - fPrevRatio)*(m_aabb.Extents.y+0.14f), 0.0f)*/);
			
			//首先强制刷新模型，设置好缩放数据，然后再重置新骨骼缩放标示 Added 2011-08-18.
//			if(m_pPlayerModel)
//				m_pPlayerModel->Tick(1);
//			if ( m_pPetModel )
//			{
//				m_pPetModel->Tick(1);
//			}
//			g_pA3DConfig->SetFlagNewBoneScale(false);
		}

	}
}

void CECPlayer::OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg)
{
	using namespace S2C;

	if ( Msg.dwParam2 == PLAYER_VIP_AWARD_UPDATE )
	{
		const player_vip_award_update* pCmd = (const player_vip_award_update*)Msg.dwParam1;
		if ( !pCmd )
		{
			return;
		}

		m_ucVIPLevel = pCmd->vip_award_level;
		m_cIsHideVip = pCmd->is_hide;
	}

}

void CECPlayer::OnMsgHstFlowBattlePlayerInfo(const ECMSG& Msg)
{
	using namespace S2C;
	const cs_flow_player_info* pCmd = (const cs_flow_player_info*)Msg.dwParam1;
	if ( !pCmd )
	{
		return;
	}
	m_nScore = pCmd->score;
	SetFlowBattle(pCmd->c_kill_cnt);
	if(pCmd->c_kill_cnt < 3 && pCmd->c_kill_cnt > -5)
	{
		SetTitle(0);
	}
}

#ifdef BACK_VERSION
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
#endif

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
	A3DBone * pBoneUp;
	if(pPlayer->GetPhysique() == PHYSIQUE_TETRAPOD) 
		pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip02 Head", NULL);
	else
		pBoneUp = pFaceModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
	A3DMATRIX4 mat = pFaceModel->GetAbsoluteTM();
	mat.SetRow(3, pBoneUp->GetAbsoluteTM().GetRow(3));

	A3DMATRIX4 matHead = a3d_RotateY(vDeg) * mat;
	A3DVECTOR3 vecPos = matHead.GetRow(3);
	A3DVECTOR3 vecDir = Normalize(matHead.GetRow(2));
	A3DVECTOR3 vecUp = Normalize(matHead.GetRow(1));

	float dist = 0.75f;
	int index = pPlayer->GetGender();
	if(pPlayer->GetPhysique() == PHYSIQUE_TETRAPOD)
		dist = 1.45f;
	else if(pPlayer->GetPhysique() == PHYSIQUE_TINY)
	{	
		if(index == 0)
			dist = 0.85f;
		else
			dist = 0.65f;
	}
	else
		dist = _dist[index];
	
	pCamera->SetPos(vecPos + vecDir * dist + vecUp * _height[index]);
	pCamera->SetDirAndUp(-vecDir, vecUp);
	pPortraitViewport->Active();
	pPortraitViewport->ClearDevice();

	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= A3DCOLORVALUE(0.8f, 0.8f, 0.8f, 1.0f);
	LightInfo.vLightDir		= Normalize(A3DVECTOR3(-1.0f, -1.0f, -1.0f));
	LightInfo.colDirDiff	= A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	LightInfo.bPtLight		= false;

	pFaceModel->SetLightInfo(LightInfo);
	A3DEnvironment::DIRLIGHT dirLightParams = *(g_pGame->GetA3DEngine()->GetA3DEnvironment()->GetDirLightParam());
	A3DEnvironment::DIRLIGHT dirLightParamsOld = *(g_pGame->GetA3DEngine()->GetA3DEnvironment()->GetDirLightParam());
	dirLightParams.Ambient0 = A3DCOLORVALUE(1.0f, 1.0f, 1.0f, 1.0f);
	dirLightParams.DirPower = 1.f;//m_Lighting.fSunPower * d + m_NightLighting.fSunPower * n;
	g_pGame->GetA3DEngine()->GetA3DEnvironment()->SetDirLightParam(&dirLightParams);

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(false);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);
	pFaceModel->RenderAtOnce(pPortraitViewport, 0);
	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);

	g_pGame->GetA3DEngine()->GetA3DEnvironment()->SetDirLightParam(&dirLightParamsOld);

	pCurViewport->Active();
}

void PlayerRenderDemonstration(const A3DRECT &rc, DWORD param1, DWORD param2, DWORD param3)
{
	static const float _dist[6][2] = { {-6.0f, -5.8f}, {-5.9f, -5.7f}, {-6.3f, -6.0f}, {-7.3f, -6.2f}, {-3.7f, -3.15f}, {-9.6f, -7.9f},};
	static const float _height[6][2] = { {0.9f, 0.9f}, {0.9f, 0.8f}, {1.3f, 1.2f}, {1.1f, 0.95f},  {0.55f, 0.45f},{1.2f, 1.4f},};
	
	// 头部特写摄像机位置
	static const float _dist_head[6][2] = { { -1.5f, -1.4f }, { -1.5f, -1.5f }, { -1.5f, -1.5f }, { -1.5f, -1.5f }, {-1.2f, -1.2f}, {-1.7f, -1.7f}, };
	static const float _height_head[6][2] = { {1.7f, 1.6f}, {1.65f, 1.55f}, {2.1f, 2.0f}, {2.1f, 1.8f},  {1.0f, 1.0f}, {2.5f, 2.5f},};

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

	g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(true);	// 这个保证gfx渲染没问题
	A3DCULLTYPE cullTypeOld = g_pGame->GetA3DDevice()->GetFaceCull();
	g_pGame->GetA3DDevice()->SetFaceCull(A3DCULL_NONE);
	CECModel* pModel = pPlayer->GetPetModel() ? pPlayer->GetPetModel() : pPlayer->GetPlayerModel();
	pModel->Render(pPortraitViewport, true);
// 	CECViewport viewport;
// 	viewport.InitFromA3D(pPortraitViewport);
// 	pPlayer->RenderEquipBloom(&viewport);
	//g_pGame->GetA3DDevice()->SetAlphaBlendEnable(true);
	g_pGame->GetA3DDevice()->SetAlphaTestEnable(false);
	AfxGetGFXExMan()->RenderAllGfx(pPortraitViewport,true);

	g_pGame->GetA3DDevice()->SetFaceCull(cullTypeOld);
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
	A3DBone * pBoneUp;
	if(pPlayer->GetPhysique() == PHYSIQUE_TETRAPOD) 
		pBoneUp	= pFaceModel->GetSkeleton()->GetBone("Bip02 Head", NULL);
	else
		pBoneUp	= pFaceModel->GetSkeleton()->GetBone("Bip01 Head", NULL);
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
void CECPlayer::BubbleGetSpecialItem(int itemid,int itemcount)
{
	if (!m_pBubbleTexts)
		return;
	 CECIvtrItem* pIvtrItem = NULL;
	 pIvtrItem = CECIvtrItem::CreateItem(itemid, 0, 1);
	 if(pIvtrItem == NULL) return;
	 CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	 AString strFile;
	 af_GetFileTitle(pIvtrItem->GetIconFile(), strFile);
	 strFile.MakeLower();
	 delete pIvtrItem;
	 pIvtrItem = NULL;
    
	 A3DVECTOR3 vPos = GetPos() + g_vAxisY * (m_aabb.Extents.y * 2.5f);


	 
	 if((m_AttachMode == enumAttachBathPlayer || m_iPuppetId) && /*!m_bHangerOn && */m_pBackupModel)
	 {
		 int index;
		 A3DBone* pHeadBone = m_pBackupModel->GetA3DSkinModel()->GetSkeleton()->GetBone("Bip01 Head", &index);
		 if( pHeadBone )
		 {
		    A3DMATRIX4 matHead = pHeadBone->GetAbsoluteTM();
		    vPos = matHead.GetRow(3);
		    vPos = vPos + g_vAxisY * 0.5f;
		 }
	 }

	 
	 CECBubbleDecal* pBubbleDecal = m_pBubbleTexts->AddDecal(vPos, CECDecal::DCID_SPRITEDECAL, true);

	 pBubbleDecal->SetLifeTime(1600);
	 pBubbleDecal->SetMoveInfo(A3DVECTOR3(0.0f, 1.0, 0.0f), 1300);
	 pBubbleDecal->SetGfx(res_GFXFile(RES_GFX_MP_SLOW_MEDICINE));
	 
	 CECSpriteDecal* pDecal = (CECSpriteDecal*)pBubbleDecal->GetDecal();
	 A2DSprite* pSprite = pGameUI->GetIconSet(CECGameUIMan::ICONS_INVENTORY);
	 int id = pGameUI->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile);
	 pDecal->ChangeSpriteIcon(pSprite,id);
	 float fScale = pGameUI->GetWindowScale();
	 pDecal->SetScaleX(fScale*1.2f);
	 pDecal->SetScaleY(fScale*1.2f);
	 pDecal->EnableScreenPos(false);
	 pBubbleDecal->SetJumpOut(true);
}
//	Print bubble text
void CECPlayer::BubbleText(int iIndex, __int64 dwNum, DWORD dwWaitTime/* 0 */, int p1/* 0 */, bool bLastOne/* true */, bool bRandPos/* false */)
{
	if (!m_pBubbleTexts)
		return;

	bool bHost = IsHostPlayer();
//	if (iIndex == BUBBLE_EXP || iIndex == BUBBLE_SP)
//		bHost = false;

	DWORD dwCol = A3DCOLORRGB(255, 255, 255);

	if (bHost)
	{
		//	Print notify text into chat dialog
		if (iIndex == BUBBLE_EXP)
			g_pGame->GetGameRun()->AddFixedChannelMsg(dwNum > 0 ? FIXMSG_GOTEXP : FIXMSG_LOSTEXP, CLIENT_CHAT_EXPERIENCE, dwNum > 0 ? dwNum : -dwNum);
		else if (iIndex == BUBBLE_SP)
			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_GOTSP, GP_CHAT_SYSTEM, (int)dwNum);
		else if (iIndex == BUBBLE_REPUTATION)
			g_pGame->GetGameRun()->AddFixedChannelMsg((int)dwNum > 0 ? FIXMSG_GOTREPUTATION : FIXMSG_LOSTREPUTATION, GP_CHAT_SYSTEM, abs((int)dwNum));
		else if (iIndex == BUBBLE_REGION_REPU)
		{
			CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
			if (p1 >= 0 && p1 < 32)
			{
				if ((int)dwNum != 0)
				{				
					g_pGame->GetGameRun()->AddFixedChannelMsg((int)dwNum > 0 ? FIXMSG_GOT_REGION_REPU : FIXMSG_LOST_REGION_REPU, GP_CHAT_SYSTEM, abs((int)dwNum), pStrTab->GetWideString(FIXMSG_REGION_REPU_1+p1));
				}
			}
		}
		else if (iIndex == BUBBLE_DEITY_EXP)
		{
			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_GOT_DEITY_EXP, GP_CHAT_SYSTEM, (int)dwNum);
		}
// 		else if (iIndex == BUBBLE_DAMAGE)
// 		{
// 			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_GOT_REGION_REPU, GP_CHAT_SYSTEM, abs((int)dwNum) );
// 		}
	}

	A3DVECTOR3 vPos = GetPos() + g_vAxisY * (m_aabb.Extents.y * 2.5f);

	if (bRandPos)
		vPos += a3d_RandDirH() * m_aabb.Extents.x;

	CECBubbleDecal* pBubbleDecal = m_pBubbleTexts->AddDecal(vPos, CECDecal::DCID_ICONDECAL, bLastOne);
	CECIconDecal* pDecal = (CECIconDecal*)pBubbleDecal->GetDecal();

	AUIManager* pUIMan = g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIManPtr();
	assert(pUIMan);
	
	float fScale1 = pUIMan->GetWindowScale() * 0.7f;
	float fScale2 = pUIMan->GetWindowScale();
	float fScale3 = pUIMan->GetWindowScale() * 1.5f;
	float fScale = fScale2;

	fScale1 = fScale2;

	switch (iIndex)
	{
	case BUBBLE_DAMAGE:
		if (!bHost)
		{
			dwCol = A3DCOLORRGB(255, 255, 255); // WHITE
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);  // RED
			fScale = fScale1;	// small font
		}
		
		if (p1 & 0x0001)
		{
			pDecal->AddIcon(CECImageRes::IMG_DEADLYSTRIKE, 0, dwCol);
			fScale *= 1.5f;	// large font
			pBubbleDecal->SetJumpOut(true);
		}
		else if (p1 & 0x0002)
		{
			pDecal->AddIcon(CECImageRes::IMG_RETORT, 0, dwCol);
			pBubbleDecal->SetJumpOut(true);
		}

		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol, bHost);
		break;

	case BUBBLE_SKILL_SPEC:
		if (!bHost)
		{
			dwCol = A3DCOLORRGB(255, 205, 75); // YELLOW
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(255, 205, 75); // YELLOW
			fScale = fScale1;	// small font
		}
		
		pDecal->AddIcon(CECImageRes::IMG_SKILL_SPEC0 + p1, 0, 0xffffffff);
		fScale *= 1.5f;	// large font
		pBubbleDecal->SetJumpOut(true);
		break;

	case BUBBLE_SKILL_DAMAGE:
		if (!bHost)
		{
			dwCol = A3DCOLORRGB(255, 205, 75); // YELLOW
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0); // RED
			fScale = fScale1;	// small font
		}
		
		if (p1 & 0x0001)
		{
			pDecal->AddIcon(CECImageRes::IMG_DEADLYSTRIKE, 0, dwCol);
			fScale *= 1.5f;	// large font
			pBubbleDecal->SetJumpOut(true);
		}
		else if (p1 & 0x0002)
		{
			pDecal->AddIcon(CECImageRes::IMG_RETORT, 0, dwCol);
			pBubbleDecal->SetJumpOut(true);
		}

		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol, bHost);
		break;

	case BUBBLE_EXP:

		dwCol = A3DCOLORRGB(255, 0, 255); // PURPLE
		pDecal->AddIcon(CECImageRes::IMG_GOTEXP, 0, dwCol);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
	//	pDecal->SetScreenPos(80, 70);
	//	pDecal->EnableScreenPos(true);
		break;

	case BUBBLE_SP:

		pDecal->AddIcon(CECImageRes::IMG_GOTSP, 0, dwCol);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
	//	pDecal->SetScreenPos(80, 90);
	//	pDecal->EnableScreenPos(true);
		break;

	case BUBBLE_REPUTATION:

		/*
		dwCol = A3DCOLORRGB(255, 0, 255); // PURPLE
		pDecal->AddIcon(CECImageRes::IMG_GOTREPUTATION, 0, dwCol);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
		*/
		break;

	case BUBBLE_REGION_REPU:

		/*
		dwCol = A3DCOLORRGB(255, 0, 255); // PURPLE
		pDecal->AddIcon(CECImageRes::IMG_GOTREGIONREPU, 0, dwCol);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
		*/
		break;

	case BUBBLE_HEAL:

		dwCol = A3DCOLORRGB(128, 255, 128);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
		break;

		/*
	case BUBBLE_MONEY:

		pDecal->AddIcon(CECImageRes::IMG_GOTMONEY, 0, dwCol);
		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
		break;
		*/

	case BUBBLE_LEVELUP:

		dwCol = A3DCOLORRGB(255, 255, 0); 
		pDecal->AddNumIcons(CECImageRes::IMG_LEVELUP, dwNum, dwCol);
		break;

	case BUBBLE_HITMISSED:
		
		if (!bHost)
		{
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);
			fScale = fScale1;	// small font
		}

		pDecal->AddNumIcons(CECImageRes::IMG_HITMISSED, dwNum, dwCol);
		pBubbleDecal->SetJumpOut(true);
		break;

	case BUBBLE_INVALIDHIT:

		if (!bHost)
		{
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);
			fScale = fScale1;	// small font
		}

		pDecal->AddNumIcons(CECImageRes::IMG_INVALIDHIT, dwNum, dwCol);
		break;

	case BUBBLE_IMMUNE:
		
		if (!bHost)
		{
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);
			fScale = fScale1;	// small font
		}

		pDecal->AddNumIcons(CECImageRes::IMG_IMMUNE, dwNum, dwCol);
		break;
	
	case BUBBLE_IMMUNE_DAMAGE:
		
		if(!bHost)
		{
			fScale = fScale2;
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);
			fScale = fScale1;
		}

		pDecal->AddNumIcons(CECImageRes::IMG_IMMUNE_DAMAGE, dwNum, dwCol);
		break;
	
	case BUBBLE_RESIST:
		
		if (!bHost)
		{
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(237, 56, 0);
			fScale = fScale1;	// small font
		}

		pDecal->AddNumIcons(CECImageRes::IMG_RESIST, dwNum, dwCol);
		break;

	case BUBBLE_HPWARN:

		dwCol = A3DCOLORRGB(255, 255, 255);
		pDecal->AddNumIcons(CECImageRes::IMG_HPWARN, dwNum, dwCol);
		break;

	case BUBBLE_MPWARN:

		dwCol = A3DCOLORRGB(255, 255, 255);
		pDecal->AddNumIcons(CECImageRes::IMG_MPWARN, dwNum, dwCol);
		break;
	
	case BUBBLE_DPWARN:
		
		dwCol = A3DCOLORRGB(255, 255, 255);
		pDecal->AddNumIcons(CECImageRes::IMG_DPWARN, dwNum, dwCol);
		break;

// 	case BUBBLE_DEITY_EXP:
// 
// 		dwCol = A3DCOLORRGB(255, 0, 255); // PURPLE
// 		pDecal->AddIcon(CECImageRes::IMG_GOTDEITYEXP, 0, dwCol);
// 		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol);
// 		break;

	case BUBBLE_DEITY_LEVELUP:
		
		dwCol = A3DCOLORRGB(255, 255, 0); 
		pDecal->AddNumIcons(CECImageRes::IMG_DEITY_LEVELUP, dwNum, dwCol);
		break;
	
	case BUBBLE_DEITY_DAMAGE:
		if (!bHost)
		{
			dwCol = A3DCOLORRGB(113, 0, 225); // Blue
			fScale = fScale2;	// medium font
		}
		else
		{
			dwCol = A3DCOLORRGB(113, 0, 225); // Blue
			fScale = fScale1;	// small font
		}
		
		if (p1 & 0x0004)
		{
			pDecal->AddIcon(CECImageRes::IMG_DT_DEADLYSTRIKE, 0, dwCol);
			fScale *= 1.5f;	// large font
			pBubbleDecal->SetJumpOut(true);
		}
		else if (p1 & 0x0002)
		{
			pDecal->AddIcon(CECImageRes::IMG_RETORT, 0, dwCol);
			pBubbleDecal->SetJumpOut(true);
		}

		pDecal->AddNumIcons(CECImageRes::IMG_POPUPNUM, dwNum, dwCol, bHost);
		break;
	default:
		return;
	}

	pDecal->SetScaleX(fScale);
	pDecal->SetScaleY(fScale);
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
				{	if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112 &&  i >= SKIN_FASHION_UPPER_BODY_INDEX && i <= SKIN_FASHION_HEADWEAR_INDEX)
					{
						DWORD dwMask = (1 << i);
						if(m_pBackupModel == m_pCacheModel)
						{
							if(m_aCurSkins[i] != m_aSkins[i][lodLevel] && (dwMask & m_dwTetrapodFashionMask))
							{
								m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, m_aSkins[i][lodLevel], false);
								m_aCurSkins[i] = m_aSkins[i][lodLevel];
							}
						}
						else
						{
							if(m_aCurSkins[i] != m_aSkins[i][lodLevel] && !(dwMask & m_dwTetrapodFashionMask))
							{
								m_pBackupModel->GetA3DSkinModel()->ReplaceSkin(i, m_aSkins[i][lodLevel], false);
								m_aCurSkins[i] = m_aSkins[i][lodLevel];
							}
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

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(iEffect, 0, 1);
	if (!pItem)
		return false;

	switch (pItem->GetClassID())
	{
	case 0:
	default:
		break;
	}

	delete pItem;
	return true;
}

//	Discard effect from player
void CECPlayer::DiscardEffect(int iEffect)
{
	//	From from effect records
	for (int i=0; i < m_aCurEffects.GetSize(); i++)
	{
		if (m_aCurEffects[i] == iEffect)
		{
			m_aCurEffects.RemoveAtQuickly(i);
			break;
		}
	}

	CECIvtrItem* pItem = CECIvtrItem::CreateItem(iEffect, 0, 1);
	if (!pItem)
		return;

	switch (pItem->GetClassID())
	{
	case 0:
	default:
		break;
	}

	delete pItem;
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
			const FIREWORKS_ESSENCE * pFireWorks = (FIREWORKS_ESSENCE *) pDBEssence;
			const char* szFWFile = g_pGame->GetDataPath(pFireWorks->file_fw);
			if (szFWFile && szFWFile[0])
			{
				g_pGame->GetGFXCaster()->PlayFW(szFWFile, GetPos() + GetDir() * 0.5f, GetDir(), NULL);
			}

			DoEmote(ROLEEXP_FIREWORK);
		}
		break;


	case DT_TEXT_FIREWORKS_ESSENCE:
		{
			const TEXT_FIREWORKS_ESSENCE * pFireWorks = (TEXT_FIREWORKS_ESSENCE *) pDBEssence;
			const char* szFWFile = g_pGame->GetDataPath(pFireWorks->file_fw);
			if (szFWFile && szFWFile[0])
			{
				g_pGame->GetGFXCaster()->PlayFW(szFWFile, GetPos() + GetDir() * 0.5f, GetDir(), AWString((const wchar_t*)pData, sz/sizeof(wchar_t)));
			}

			DoEmote(ROLEEXP_FIREWORK);
		}
		break;
	case DT_CASH_MEDIC_MERGE_ITEM_ESSENCE:	//2012-10-22 add 添加药品合计播放特效 by zy
		{
			if( !GetPlayerModel() )
				return true;

			const CASH_MEDIC_MERGE_ITEM_ESSENCE * pMedicine = (CASH_MEDIC_MERGE_ITEM_ESSENCE *) pDBEssence;
			switch(pMedicine->type)
			{
			case 0: //瞬回HP
			case 3:
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_HP_MEDICINE), NULL);
				break;
				
			case 1: //瞬回MP
			case 4:
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_MP_MEDICINE), NULL);
				break;
				
			case 2: // 元力
			case 5:
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_DP_MEDICINE), NULL);
				break;
			}
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

		//人马骨骼特殊
		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_Spine1";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 2.5f);
			}
			szHH = "HH_chest";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 2.5f);
			}
			szHH = "HH_maduzi";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 3.5f);
			}
			szHH = "HH_zuomatui2";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 1.5f);
			}
			szHH = "HH_youmatui2";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale * 1.5f);
			}				
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
		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_zuomatui3";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
			}		
			szHH = "HH_zuomatui4";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
			}
			szHH = "HH_youmatui3";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
			}
			szHH = "HH_youmatui4";
			if( m_pBackupModel )
			{
				m_pBackupModel->PlayGfx(szGFX, szHH, vScale);
			}
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
		
		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_Spine1";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_chest";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_maduzi";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_zuomatui2";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_youmatui2";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}				
		}

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

		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_zuomatui3";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}		
			szHH = "HH_zuomatui4";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_youmatui3";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
			szHH = "HH_youmatui4";
			if( m_pBackupModel )
			{
				m_pBackupModel->RemoveGfx(szGFX, szHH);
			}
		}

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

		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_Spine1";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			szHH = "HH_chest";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			color.a = 1.0f;
			szHH = "HH_maduzi";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			color.a = 0.8f;
			szHH = "HH_zuomatui2";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			szHH = "HH_youmatui2";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
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
		
		if(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112)
		{
			szHH = "HH_zuomatui3";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}		
			szHH = "HH_zuomatui4";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			szHH = "HH_youmatui3";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
			}
			szHH = "HH_youmatui4";
			if( m_pBackupModel )
			{
				A3DGFXEx * pGfx = m_pBackupModel->GetGfx(szGFX, szHH);
				if( pGfx )
				{
					pGfx->SetVisible(bVisible);
					pGfx->SetOuterColor(color);
				}
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

void CECPlayer::SetName(const ACHAR *szName) 
{
	m_strName = szName;
	m_pPateName->SetText(m_strName, false); 
}

//	Get player name color
DWORD CECPlayer::GetNameColor()
{
	DWORD dwNameCol;
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
    
	if(pHost->GetKingdomWarType() != 0)
	{
		//国战主战场第一阶段
		if(pHost->GetKingdomWarType() == 1)
		{
			if(m_iBattleCamp != pHost->GetBattleCamp())
				dwNameCol = NAMECOL_BC_RED;
			else
				dwNameCol = NAMECOL_WHITE;
		}
		//国战主战场第二阶段和辅战场
		else
		{
			if(pHost->GetFactionID() != GetFactionID())
				dwNameCol = NAMECOL_BC_RED;
			else
				dwNameCol = NAMECOL_WHITE;
			
		}
		return dwNameCol;
	}
	else if (IsInBattle())	//	Player is in battle
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

	int iStoneWeapon = m_iExtraEquipEffect>0 ? m_iExtraEquipEffect : m_stoneWeapon;

	if (m_pBackupModel && iStoneWeapon != 0xff && iStoneWeapon > 0)
	{
		A3DCOLORVALUE c1 = l_ecolors[min(iStoneWeapon, 20)];
		
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

		//---------------------------------------------------------------------
		//Bloom 渲染代码优化Added end Modified 2012-09-25.
		const int MAX_WEAPON_HANGER_COUNT = 4;	//注意：如果增加武器挂点，此处也需要修改！
		static const char* szWeaponHanger[MAX_WEAPON_HANGER_COUNT] = 
																	{
																		_weapon_hanger,
																		_right_weapon_hanger,
																		_back_weapon_hanger,
																		_new_back_weapon_hanger,
																	};
		for ( int i = 0; i < MAX_WEAPON_HANGER_COUNT; ++i )
		{
			CECModel* pWeapon = m_pBackupModel->GetChildModel(szWeaponHanger[i]);
			if ( pWeapon )
			{
				A3DSkinModel * pSkinModel = pWeapon->GetA3DSkinModel();
				if ( !pSkinModel )
				{
					continue;
				}
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
				{
					continue;//!!!!!!!!!!
				}
				pSkinModel->GetUniformMaterial().SetMaterialParam(mat);
				int oldMethod = pSkinModel->GetMaterialMethod();
				pSkinModel->SetMaterialMethod(A3DSkinModel::MTL_UNIFORM);  
				
				pSkinModel->RenderAtOnce(pViewport->GetA3DViewport(), A3DSkinModel::RAO_NOTEXTURE);

				pSkinModel->SetMaterialMethod(oldMethod);
				pSkinModel->GetUniformMaterial().SetMaterialParam(matOld);
			}
		}
		//---------------------------------------------------------------------
/*			//进行优化，下面的代码过于臃肿 Canceled And Modified 2012-09-25.
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

		//Added 2012-09-25. 渲染非战斗状态下背部武器的Bloom效果
		pWeapon = m_pBackupModel->GetChildModel(_new_back_weapon_hanger);
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
*/
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

void CECPlayer::HideWeapon(bool bHide)
{
	if(bHide)
	{
		m_dwHideEquipMask |= (1<<EQUIPIVTR_WEAPON);
	}
	else
	{
		m_dwHideEquipMask &= ~(1<<EQUIPIVTR_WEAPON);		
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
	WStrTable::pair_type Pair = m_MilitaryRanks.get(rank);
	if (Pair.second)
	{
		m_strMilitaryRankName =  **Pair.first;
		m_pPateRankName->SetText(m_strMilitaryRankName,false,false);
	}
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

	//-------------------------------------------------------------------------
	//Added 2012-08-27.对于轩辕新职业，由于头盔有两套：全部包住头的头盔（有面具）,不包住头的头盔(无面具)
	//因此，在隐藏装备时，需要应用装备位的EQUIPIVTR_FASHION_EYE位来作为标示，而实际处理的是EQUIPIVTR_HEAD位
	if ( m_dwHideEquipMaskInUse & (1<<EQUIPIVTR_FASHION_EYE) )	//处理一下
	{
		m_dwHideEquipMaskInUse &= ~(1<<EQUIPIVTR_FASHION_EYE);
	}
	//-------------------------------------------------------------------------

	if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1)
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
	DetachQilinBuddy();

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
	if (m_pPateCircle)
		m_pPateCircle->Clear();
}

void CECPlayer::SetCRole(int id)
{
	m_idCRole = id;
	if (m_pPateCircle)
		m_pPateCircle->Clear();
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
		SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
		if (pInfo && pInfo->mask_sect & (1<<SECT_15))  // 天华
		{
			if (IsInDomainState() && !IsHostPlayer())
			{
				// 当前技能是领域技能，已经释放完毕，可以删除了，目的是为了后面正确的播放动作
				// 因为后续的弹琴动作是在领域技能动作之后播放的。
				if (m_pCurSkill)
				{
					//	Release current skill
					delete m_pCurSkill;
					m_pCurSkill = NULL;
				}
				m_idCurSkillTarget = 0;	
			}
			if (!IsHostPlayer() && !IsDead())
				PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
		}
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

void CECPlayer::UpdateRuneStateGFX( int nRuneQuality )
{
	if ( nRuneQuality < 0 )
	{
//		return;
	}
	else if ( nRuneQuality < 21 )
	{
		nRuneQuality = -1;
	}
	else if ( nRuneQuality < 41 )
	{
		nRuneQuality = 0;
	}
	else if ( nRuneQuality < 56 )
	{
		nRuneQuality = 1;
	}
	else if ( nRuneQuality < 71 )
	{
		nRuneQuality = 2;
	}
	else if ( nRuneQuality < 86 )
	{
		nRuneQuality = 3;
	}
	else if ( nRuneQuality < 96 )
	{
		nRuneQuality = 4;
	}
	else
	{
		nRuneQuality = 5;
	}

	if ( nRuneQuality == m_nRuneQuality )
	{
		return;
	}

	const char* szFileOld = NULL;
	const char* szFileNew = NULL;
	const int MAX_RUNE_QUALITY = 6;

	if ( m_nRuneQuality >= 0 && m_nRuneQuality < MAX_RUNE_QUALITY )
	{
		szFileOld = res_GFXFile( RES_GFX_RUNE_01 + m_nRuneQuality );
	}
	if ( nRuneQuality >= 0 && nRuneQuality < MAX_RUNE_QUALITY )
	{
		szFileNew = res_GFXFile( RES_GFX_RUNE_01 + nRuneQuality );
	}

	if ( m_pBackupModel )
	{
		if ( szFileOld )
		{
			m_pBackupModel->RemoveGfx( szFileOld, "HH_Spine" );
		}
		if ( szFileNew )
		{
			m_pBackupModel->PlayGfx( szFileNew, "HH_Spine");
		}
	}

	m_nRuneQuality = nRuneQuality;
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
	else if (iProf<=60&&iProf>=56)
		iProfRace = PROFRACE_CHENHUANG;
	else if(iProf<=106&&iProf>=102)
		iProfRace = PROFRACE_QIANJI;
	else if(iProf<=112&&iProf>=108)
		iProfRace = PROFRACE_YINGZHAO;
	return iProfRace;
}

/*
人族	1,2,3 ...

烈山	1,2,3,4,5,6,7  31,32,33,34 ...
怀光	8,9,10,11,12,13,14,15 ... 30

  
九黎	1,2,3,4, ... 30
太昊	31,32,33, ...
	
天华	1,2,3 ...
辰皇	31,32,33,34,35
	  
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
	else if (iProf>=96&&iProf<=101)	// 太昊
	{
		ret = id+30;
	}
	else if (iProf>=56&&iProf<=60)	// 辰皇
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
void CECPlayer::SetMirrorModelCnt(int iMirrorImageCnt) 
{
	int i;

	// set invisible first
	for(i=0;i<6;i++)
	{
		if(m_MirrorModels[i])
		{
			m_MirrorModels[i]->SetVisible(false);
			m_MirrorModels[i]->SetFashionMode(m_bFashionMode);
//			m_MirrorModels[i]->SetHideEquipMask(m_dwHideEquipMask);
		}
		//	A3DRELEASE(m_MirrorModels[i]);
	}

	if(iMirrorImageCnt == 0)	// 需要清空周围镜像
	{		
		m_iMirrorImageCnt = 0;
		return;
	}
	else
	{
		bool bLoaded  = false;
		for(i=0;i<6;i++)
		{
			if(m_MirrorModels[i])
			{
				bLoaded = true;
				break;
			}
		}

		if(!bLoaded)	// 镜像模型还未加载，多线程加载镜像模型
		{
			for(i=0;i<6;i++)
			{
				m_MirrorModels[i] = new CECLoginPlayer(NULL);
				m_MirrorModels[i]->SetVisible(false);
				m_MirrorModels[i]->SetFashionMode(m_bFashionMode);
//				m_MirrorModels[i]->SetHideEquipMask(m_dwHideEquipMask);
			}

			memcpy(m_aEquipsTmp, m_aHideEquips, SIZE_EQUIPIVTR*sizeof(int));
			m_aEquipsTmp[EQUIPIVTR_MAIN_TALISMAN] = 0;
			QueueECModelForLoad(
				MTL_ECM_MIRROR_MODEL,
				m_PlayerInfo.cid,
				m_dwBornStamp,
				GetPos(),
				NULL,
				NULL,
				m_iFaceid,
				m_iHairid,
				(m_iPhysique << 16) + m_iGender,
				m_aEquipsTmp,
				m_BasicProps.iLevel,
				m_iEarid,
				m_iTailid,
				m_BasicProps.iProfession,
				m_iDefaultSkin);
		}
		
	}

	m_iMirrorImageCnt = iMirrorImageCnt;

	// set visible
	for(i=0;i<6;i++)
	{
		m_MirrorModels[i]->PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);

		UpdateMirrorAndSpiritModels(1);

		if(i<iMirrorImageCnt)
			m_MirrorModels[i]->SetVisible(true);
	}
}

void CECPlayer::SetDarkLightSpirit(int index, char value) 
{
	if(m_pSpiritsGFX[index])	// 已经有灵的gfx
	{
		if(m_cDarkLightSpirits[index] == value)	// 新的灵与原有的灵相同
			return;

		m_pSpiritsGFX[index]->Stop();
		g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSpiritsGFX[index]);
		m_pSpiritsGFX[index] = NULL;
	}

	m_cDarkLightSpirits[index] = value;

	if(m_cDarkLightSpirits[index])
	{
		// 重新加载灵的gfx
		m_pSpiritsGFX[index] = g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_SPIRIT_01+(m_cDarkLightSpirits[index]-1)));
	}
}


// 创建一个新的Spirit Model并返回
CECLoginPlayer* CECPlayer::CreateSpiritModel(int index)
{
	if(index < 0 || index > 3)
		return NULL;

	if(m_SpiritModels[index] != NULL)
		return m_SpiritModels[index];

	// 创建新的Spirit Model
	m_SpiritModels[index] = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	if(!m_SpiritModels[index])
	{
		return NULL;
	}

	m_SpiritModels[index]->SetPos(GetPos());
	if (!m_SpiritModels[index]->Load(CECPlayer::GetPhysiqueByProf(m_BasicProps.iProfession),m_BasicProps.iProfession, m_iFaceid, m_iHairid, m_iGender, NULL, true))
	{
		A3DRELEASE(m_SpiritModels[index]);
		return NULL;
	}

	// generate spirit player id
	int nPlayerID = GetCharacterID();
	nPlayerID |= 0xf0000000;
	nPlayerID |= (index << 26);

	m_SpiritModels[index]->SetPlayerCharacterID(nPlayerID);

	m_SpiritModels[index]->SetPos(GetPos());
	m_SpiritModels[index]->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));

	if(m_SpiritModels[index]->GetPlayerModel())
		m_SpiritModels[index]->GetPlayerModel()->SetAutoUpdateFlag(true);

	m_SpiritModels[index]->ShowEquipments(m_aEquips, false, true);
	m_SpiritModels[index]->SetFashionMode(true);

	return m_SpiritModels[index];
}


void CECPlayer::UpdateMirrorAndSpiritModels(DWORD dwDeltaTime)	// 更新镜像和灵的玩家模型
{
	int i=0;
//====================================================== Tick mirror models
	// 获取Player离地高度
	float fDistToGround = 0.0f;
	A3DVECTOR3 vNormal, vGndPos0, vTestPos0 = GetPos() + g_vAxisY * m_aabb.Extents.y;
	VertRayTrace(vTestPos0, vGndPos0, vNormal);
	fDistToGround = GetPos().y - vGndPos0.y;

	for(i=0;i<m_iMirrorImageCnt;i++)
	{
		CECLoginPlayer* pMirrorPlayer = m_MirrorModels[i];
		if(pMirrorPlayer && pMirrorPlayer->GetPlayerModel())
		{
			A3DVECTOR3 _vPos = GetPos();

			float fLength = 1.5f;
			
			float xOffset = fLength*cos(DEG2RAD((i+1)* 360.0f/m_iMirrorImageCnt));
			float zOffset = fLength*sin(DEG2RAD((i+1)* 360.0f/m_iMirrorImageCnt));

			_vPos.x += xOffset;
			_vPos.z += zOffset;
			_vPos.y = _vPos.y + 2.0f;

			CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();

			A3DVECTOR3 vGndPos, vTestPos = _vPos + g_vAxisY * m_aabb.Extents.y;
			VertRayTrace(vTestPos, vGndPos, vNormal);
			_vPos.y = vGndPos.y + fDistToGround;

			pMirrorPlayer->SetPos(_vPos);
			pMirrorPlayer->Tick(dwDeltaTime);
			pMirrorPlayer->SetDirAndUp(GetDir(), GetUp());
		}
	}

//====================================================== Tick spirit models
	for(i=0;i<3;i++)
	{
		CECLoginPlayer* pSpiritPlayer = m_SpiritModels[i];
		if(pSpiritPlayer)
		{
			pSpiritPlayer->Tick(dwDeltaTime);
		}
	}
}


//
void CECPlayer::UpdateMultilineSkillGfx( const int* pTargets, int nTargetNum )
{
	if ( !pTargets || nTargetNum > MAX_MULTILINE_SKILL_GFX_NUM )
	{
		assert( nTargetNum <= MAX_MULTILINE_SKILL_GFX_NUM );
		return;
	}

	//先初始化，并在更新线性特效之前，先停止所有线性特效
	for ( int i = 0; i < MAX_MULTILINE_SKILL_GFX_NUM; ++i )
	{
		if ( !m_pMultiLineSkillGfx[i] )
		{
			m_pMultiLineSkillGfx[i] = g_pGame->GetGFXCaster()->LoadGFXEx("程序联入\\线性光群总.gfx");//人物\\技能\\轩辕\\线性光群总.gfx
		}
		if ( m_pMultiLineSkillGfx[i] && (m_pMultiLineSkillGfx[i]->GetState() != ST_STOP) )
		{
			m_pMultiLineSkillGfx[i]->Stop();
		}
	}
	
	//根据目标数目更新特效
	for ( int j = 0; j < nTargetNum; ++j )
	{
		CECObject* pTarget = g_pGame->GetGameRun()->GetWorld()->GetObject(pTargets[j], 1);
		if ( pTarget )
		{
			A3DVECTOR3 vCasterPos = GetPos() + A3DVECTOR3(0.0f, 2.0f, 0.0f), vTargetPos = pTarget->GetPos() + A3DVECTOR3(0.0f, 2.0f, 0.0f);
			//计算两个位置的方向
			A3DVECTOR3 vDir = a3d_Normalize( vTargetPos - vCasterPos );
			//计算转换矩阵
			A3DMATRIX4 mat = a3d_TransformMatrix(g_vAxisY, vDir, vCasterPos);	//pTarget->GetPos()
			//设置该gfx的转换矩阵
			m_pMultiLineSkillGfx[j]->SetParentTM(mat);
			//设置该gfx的两个端点位置
			A3DVECTOR3 aEdge[2] = { vCasterPos, vCasterPos + (vDir * 20.0f) };	//pTarget->GetPos()
			//针对每一个gfx的元素，都设置其两头端点位置。
			int nElementCount = m_pMultiLineSkillGfx[j]->GetElementCount();
			for ( int nCurIndex = 0; nCurIndex < nElementCount; ++nCurIndex )
			{
				A3DGFXElement* pCurElement = m_pMultiLineSkillGfx[j]->GetElement( nCurIndex );
				if ( !pCurElement || ( pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNING && pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNINGEX ) )
				{
					continue;
				}
				const char* szElementName = pCurElement->GetName();
				bool ret = GFX_UpdateLightingEdgePos(m_pMultiLineSkillGfx[j], szElementName, aEdge);
			}

			if ( m_pMultiLineSkillGfx[j]->GetState() == ST_STOP )
			{
				m_pMultiLineSkillGfx[j]->Start(true);
			}
		}
	}
}

void CECPlayer::SetFlowBattle(int iFlowBattle)
{
	m_iBattleKill = iFlowBattle;
	
	if(GetPlayerModel())
	{
		A3DGFXEx* pGFX = GetPlayerModel()->GetGfx(res_GFXFile(RES_GFX_FLOW_BATTLE), "HH_Spine");
		if(pGFX)
		{
			pGFX->Stop(true);
		}
	}
	
	
	if(m_iBattleKill >= 3)
	{
		
		if(m_iBattleKill >= 15 && m_iBattleKill < 30)
		{
			SetTitle(12001);
		}
		else if(m_iBattleKill >= 30 && m_iBattleKill < 50)
		{
			SetTitle(12002);
		}
		else if(m_iBattleKill >= 50 && m_iBattleKill < 100)
		{
			SetTitle(12003);
		}
		else if(m_iBattleKill >= 100)
		{
			SetTitle(12004);
			
			if(GetPlayerModel())
			{
				GetPlayerModel()->PlayGfx(res_GFXFile(RES_GFX_FLOW_BATTLE), "HH_Spine");
			}
			
		}
	}
	else if(m_iBattleKill <= -5)
	{
		SetTitle(12000);
	}
}

void CECPlayer::ClearFlowBattle()
{
	if(GetPlayerModel())
	{
		A3DGFXEx* pGFX = GetPlayerModel()->GetGfx(res_GFXFile(RES_GFX_FLOW_BATTLE), "HH_Spine");
		if(pGFX)
		{
			pGFX->Stop(true);
		}
	}
	SetTitle(0);
}


bool CECPlayer::PlayActiveEmote(ACTIVEEMOTEINFO info)
{
	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(info.id);
	if (pPlayer == NULL)
		return false;

	CECModel* pPlayerModel = pPlayer->GetPlayerModel();
	if (pPlayerModel == NULL)
		return false;

	char *_hh_emote, *_cc_emote;
	switch(info.type)
	{
		case ROLEEXP_AP_PROPOSE:
			_hh_emote = "HH_qiuhun";
			_cc_emote = "CC_qiuhun";
		break;

		default:
			_hh_emote = "";
			_cc_emote = "";
	}

	if (!m_pBackupModel->AddChildModel(_hanger_emote, false, _hh_emote, pPlayerModel, _cc_emote))
		return false;

	m_AttachMode = enumAttachActiveEmote;
	m_iBuddyId = pPlayer->GetPlayerInfo().cid;
	m_bHangerOn = false;
	m_iEmoteType = info.type;
	
	pPlayer->SetPos(GetPos());
	pPlayer->m_AttachMode = enumAttachActiveEmote;
	pPlayer->m_iBuddyId = GetPlayerInfo().cid;
	pPlayer->m_bHangerOn = true;
	pPlayer->m_iEmoteType = info.type;

	switch(info.type)
	{
	case ROLEEXP_AP_PROPOSE:
		m_iEmoteActionType = pPlayer->m_iEmoteActionType = ACT_PROPOSE;

	default:
		m_iEmoteActionType = pPlayer->m_iEmoteActionType = ACT_PROPOSE;
	}

	PlayAction(m_iEmoteActionType, 1.0f);
	return true;
}

void CECPlayer::StopActiveEmote()
{
	if (m_AttachMode != enumAttachActiveEmote)
		return;

	CECPlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(m_iBuddyId);
	if (pPlayer == NULL)
		return;

	if (m_bHangerOn)
	{
		pPlayer->StopActiveEmote();
		return;
	}
	
	if (m_pBackupModel)
		m_pBackupModel->RemoveChildModel(_hanger_emote, false);

	m_AttachMode = enumAttachNone;
	m_iBuddyId = 0;
	m_iEmoteType = 0;
	m_iEmoteActionType = 0;
	m_CandEmoteInfo.id = 0;
	PlayAction(GetMoveStandAction(CurActionIsMoving(), IsFighting()), 1.0f, true);

	pPlayer->m_AttachMode = enumAttachNone;
	pPlayer->m_bHangerOn = false;
	pPlayer->m_iBuddyId = 0;
	pPlayer->m_iEmoteType = 0;
	pPlayer->m_iEmoteActionType = 0;
	pPlayer->m_CandEmoteInfo.id = 0;
	pPlayer->SetPos(pPlayer->GetPos());
	pPlayer->SetDirAndUp(pPlayer->GetDir(), pPlayer->GetUp());
	pPlayer->PlayAction(GetMoveStandAction(false, IsFighting()), 1.0f, true);
}

void CECPlayer::SetBloodPool(bool bBloodPool, int iCurBloodPool, int iMaxBloodPool)
{
	m_bBloodPool	= bBloodPool;
	m_iCurBloodPool = iCurBloodPool;
	m_iMaxBloodPool = iMaxBloodPool;
}
