/*
 * FILE: EC_LoginUIMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "AFI.h"
#include "AIniFile.h"
#include "A3DConfig.h"
#include "AUI\\AUICTranslate.h"

#include "EC_CDS.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_GameRecord.h"
#include "EC_IvtrTypes.h"
#include "EC_LoginUIMan.h"
#include "EC_ManPlayer.h"
#include "EC_Resource.h"
#include "EC_ShadowRender.h"
#include "EC_World.h"
#include "EC_Model.h"
#include "EC_LoginPlayer.h"
#include "EL_BackMusic.h"
#include "EC_InputCtrl.h"
#include "EC_PortraitRender.h"
#include "AUI\\CSplit.h"
#include "EC_Configs.h"
#include "EC_Utility.h"
#include "matrixchallenge.hpp"
#include "DlgSkillMan.h"
#include "DlgBaseExplorer.h"

#include "gnetdef.h"

#include "A3DEngine.h"
#include "A2DSprite.h"
#include "A3DCamera.h"
#include "A3DViewport.h"
#include "A3DFont.h"
#include "A3DSkinRender.h"
#include "A3DSkinMan.h"
#include "A3DCollision.h"
#include "A3DFuncs.h"
#include "A3DGfxExMan.h"
#include "AMVideoClip.h"
#include "AWScriptFile.h"
#include "A3DMathUtility.h"

#include "DlgLoginServerList.h"
#include "DlgLoginPage.h"
#include "DlgLoginRefId.h"
#include "DlgLoginServerSearch.h"
#include "DlgLoginPwdProtect.h"
#include "DlgBattleFactionWarControl.h"
#include "EC_GameUIMisc.h"

#include "CC_API.h"

using namespace CC_SDK;
extern CC_HANDLE	l_hSDK;
extern HMODULE		l_hDll;

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#include "A3DEnvironment.h"
#endif

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////
#define MSGBOX(sid) MessageBox("", GetStringFromTable(sid), MB_OK, A3DCOLORRGBA(255, 255, 255, 160))

///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////

extern game_record gr;

///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////
static BYTE aExistFlags[] = {
	1, 1,	// 人族
	1, 1,	// 烈山
	1, 1,	// 九黎
	1, 1,	// 怀光
	1, 1,	// 天华
	1, 1,	// 太昊
	1, 1,  // 辰皇
	1, 1,   // 人马
	1, 1   // 萝莉
};

static const float scaleRatio = 10.0f;
static const float modelPosAdjust[][2] = { {0.1f, 0.18f}, {0.16f, 0.1f}, {0.0f, 0.0f}, {0.38f, 0.3f} };

const int CECLoginUIMan::msi_EquipIds[14][SHOW_EQUIP_NUM][CECLOGINUIMAN_NUM_GENDERS][4]= 
{
	{
		{ {423,383,343,303}, {503,	463,	543,	303} },
		{ {20818,20698,	20938,	20618}, {21637,	21627,	21648,	21620} },
		{ {20820,	20700,	20940,	20619}, {20803,	20683,	20923,	20610} } 
	},
	
	{
		{ {435,395,355,315}, {512,472,552,312} },
		{ {20842,20722,20962,20630}, {515,475,555,315} },
		{ {20844,20724,20964,20631}, {20845,20725,20965,20631} } 
	},
		
	{
		{ {441,401,361,321}, {524,484,564,324} },
		{ {445,405,365,325}, {20853,20733,20973,20635} },
		{ {20866,20746,20986,20642}, {20869,20749,20989,20643} } 
	},
		
	{
		{ {455,415,375,332}, {530,490,570,332} },
		{ {20876,20756,20996,333}, {535,495,575,333} },
		{ {20892,20772,21012,335}, {20893,20773,21013,335} } 
	},
		
	{
		{ {14307,14680,14962,14589}, {14447,14820,15102,14588} },
		{ {20780,20660,20900,20599}, {20787,20667,20907,20602} },
		{ {20782,20662,20902,20600}, {20781,20661,20901,20599} } 
	},
		
	{
		{ {45359, 45361, 45363, 45358}, {45360, 45362, 45364, 45358}},
		{ {48170, 48248, 48326, 48097}, {48175, 48253, 48331, 48097} },
		{ {48172, 48250, 48328, 48099}, {48177, 48255, 48333, 48099} } 
	},
		
	{
		{ {25476,25516,25556,25623}, {25477,25517,25557,25623} },
		{ {25482,25522,25562,25626}, {25483,25523,25563,25626} },
		{ {26092,26886,26546,26389}, {26259,27053,26713,26389} } 
	},
		
	{
		{ {25500,25540,25580,25635}, {25489,25529,25569,25629} },
		{ {27245,28178,27834,27677}, {27424,28344,28004,27676} },
		{ {27241,28174,27830,27673}, {27421,28341,28001,27673} } 
	},
		
	{
		{ {35040,35060,35080,35094}, {35039,35059,35079,35093} },
		{ {35042,35062,35082,35095}, {35029,35049,35069,35088} },
		{ {36939,37279,37625,37780}, {36940,37280,37626,37780} } 
	},
		
	{
		{ {35753,36118,36464,36626}, {35754,36119,36465,36626} },
		{ {35755,36120,36466,36627}, {35756,36121,36467,36627} },
		{ {35773,36138,36484,36636}, {35774,36139,36485,36636} } 
	},
		
	{
		{ {45040, 45042, 45044, 45039}, {45041, 45043, 45045, 45039} },
		{ {48443, 48521, 48599, 48370}, {48448, 48526, 48604, 48370} },
		{ {48445, 48523, 48601, 48372}, {48450, 48528, 48606, 48372} } 
	},
		
	{
		{ {53004, 53006, 53008, 53003}, {53005, 53007, 53009, 53003} },
		{ {52962, 52964, 52966, 52961}, {52977, 52979, 52981, 52975} },
		{ {52997, 52999, 53001, 52996}, {53000, 52998, 53002, 52996} } 
	},
		
	{
		{ {59698, 59699, 59700, 59697}, {0, 0, 0, 0} },
		{ {59702, 59703, 59704, 59701}, {0, 0, 0, 0} },
		{ {59710, 59711, 59712, 59709}, {0, 0, 0, 0} } 
	},	

	{
		{ {59060, 59062, 59064, 59059}, {59061, 59063, 59065, 59059} },
		{ {59067, 59069, 59071, 59066}, {59068, 59070, 59072, 59066} },
		{ {59095, 59097, 59099, 59094}, {59096, 59098, 59100, 59094} } 
	},
};

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECLoginUIMan
//	
///////////////////////////////////////////////////////////////////////////

CECLoginUIMan::CECLoginUIMan()
{
	m_pLoginBack		= NULL;
	m_pLogo				= NULL;
	m_pKrLogo			= NULL;
	m_pIntroMovie		= NULL;
	m_pwdCurObj			= NULL;

	m_idCurRole = -1;
	m_fskStartPos = 0;

	m_nCurProfession = LOGIN_MODEL_NORMAL;
	m_nCurGender = 0;
	memset(m_aModel, 0, sizeof(CECLoginPlayer *) * CECLOGINUIMAN_NUM_GENDERS * LOGIN_MODEL_NUM);
	m_iCurCreate = CREATE_TYPE_MIN;

	m_bDragRole = false;
}

CECLoginUIMan::~CECLoginUIMan()
{
}

//	Initalize manager
bool CECLoginUIMan::Init(A3DEngine* pA3DEngine, A3DDevice* pA3DDevice, const char* szDCFile/* NULL */)
{
	int i;
	bool bval;
	float x, y, z;
	AIniFile theIni;
	char szFile[MAX_PATH];

	// load config from ini file
	sprintf(szFile, "%s\\Configs\\SceneCtrl.ini", af_GetBaseDir());
	theIni.Open(szFile);

	x = theIni.GetValueAsFloat("RoleList", "roleX", 0.0f);
	y = theIni.GetValueAsFloat("RoleList", "roleY", 0.0f);
	z = theIni.GetValueAsFloat("RoleList", "roleZ", 0.0f);
	m_PosRole[LOGIN_SCENE_SELCHAR] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("RoleList", "camX", 0.0f);
	y = theIni.GetValueAsFloat("RoleList", "camY", 0.0f);
	z = theIni.GetValueAsFloat("RoleList", "camZ", 0.0f);
	m_PosCamera[LOGIN_SCENE_SELCHAR] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("RoleList", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("RoleList", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("RoleList", "dirZ", 0.0f);
	m_DirCamera[LOGIN_SCENE_SELCHAR] = A3DVECTOR3(x, y, z);
	
	
	x = theIni.GetValueAsFloat("CreateHuman", "roleX", 0.0f);
	y = theIni.GetValueAsFloat("CreateHuman", "roleY", 0.0f);
	z = theIni.GetValueAsFloat("CreateHuman", "roleZ", 0.0f);
	m_PosRole[LOGIN_SCENE_CREATE_HUMAN] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CreateHuman", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CreateHuman", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CreateHuman", "camZ", 0.0f);
	m_PosCamera[LOGIN_SCENE_CREATE_HUMAN] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CreateHuman", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CreateHuman", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CreateHuman", "dirZ", 0.0f);
	m_DirCamera[LOGIN_SCENE_CREATE_HUMAN] = A3DVECTOR3(x, y, z);	
	
	
	x = theIni.GetValueAsFloat("CreateWild", "roleX", 0.0f);
	y = theIni.GetValueAsFloat("CreateWild", "roleY", 0.0f);
	z = theIni.GetValueAsFloat("CreateWild", "roleZ", 0.0f);
	m_PosRole[LOGIN_SCENE_CREATE_WILD] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CreateWild", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CreateWild", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CreateWild", "camZ", 0.0f);
	m_PosCamera[LOGIN_SCENE_CREATE_WILD] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CreateWild", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CreateWild", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CreateWild", "dirZ", 0.0f);
	m_DirCamera[LOGIN_SCENE_CREATE_WILD] = A3DVECTOR3(x, y, z);	


	x = theIni.GetValueAsFloat("CreateRaceQianJi", "roleX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceQianJi", "roleY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceQianJi", "roleZ", 0.0f);
	m_PosRole[LOGIN_SCENE_CREATE_QIANJI] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CreateRaceQianJi", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceQianJi", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceQianJi", "camZ", 0.0f);
	m_PosCamera[LOGIN_SCENE_CREATE_QIANJI] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CreateRaceQianJi", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceQianJi", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceQianJi", "dirZ", 0.0f);
	m_DirCamera[LOGIN_SCENE_CREATE_QIANJI] = A3DVECTOR3(x, y, z);	


	x = theIni.GetValueAsFloat("CreateRaceYingZhao", "roleX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceYingZhao", "roleY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceYingZhao", "roleZ", 0.0f);
	m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CreateRaceYingZhao", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceYingZhao", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceYingZhao", "camZ", 0.0f);
	m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CreateRaceYingZhao", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CreateRaceYingZhao", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CreateRaceYingZhao", "dirZ", 0.0f);
	m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO] = A3DVECTOR3(x, y, z);	

	
	x = theIni.GetValueAsFloat("CloseUpHuman", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpHuman", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpHuman", "camZ", 0.0f);
	m_CloseUpPosCamera[0][0] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpHuman", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpHuman", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpHuman", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][0] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpHumanFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpHumanFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpHumanFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][0] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpHumanFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpHumanFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpHumanFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][0] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildSmall", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildSmall", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildSmall", "camZ", 0.0f);
	m_CloseUpPosCamera[0][1] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildSmall", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildSmall", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildSmall", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][1] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][1] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildSmallFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][1] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildNormal", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildNormal", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildNormal", "camZ", 0.0f);
	m_CloseUpPosCamera[0][2] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildNormal", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildNormal", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildNormal", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][2] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][2] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildNormalFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][2] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildBig", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildBig", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildBig", "camZ", 0.0f);
	m_CloseUpPosCamera[0][3] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildBig", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildBig", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildBig", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][3] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildBigFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildBigFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildBigFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][3] = A3DVECTOR3(x, y, z);
	
	x = theIni.GetValueAsFloat("CloseUpWildBigFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpWildBigFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpWildBigFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][3] = A3DVECTOR3(x, y, z);


	x = theIni.GetValueAsFloat("CloseUpQianJi", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpQianJi", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpQianJi", "camZ", 0.0f);
	m_CloseUpPosCamera[0][4] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpQianJi", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpQianJi", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpQianJi", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][4] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpQianJiFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpQianJiFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpQianJiFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][4] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpQianJiFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpQianJiFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpQianJiFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][4] = A3DVECTOR3(x, y, z);


	x = theIni.GetValueAsFloat("CloseUpYingZhao", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpYingZhao", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpYingZhao", "camZ", 0.0f);
	m_CloseUpPosCamera[0][5] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpYingZhao", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpYingZhao", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpYingZhao", "dirZ", 0.0f);
	m_CloseUpDirCamera[0][5] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "camX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "camY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "camZ", 0.0f);
	m_CloseUpPosCamera[1][5] = A3DVECTOR3(x, y, z);

	x = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "dirX", 0.0f);
	y = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "dirY", 0.0f);
	z = theIni.GetValueAsFloat("CloseUpYingZhaoFemale", "dirZ", 0.0f);
	m_CloseUpDirCamera[1][5] = A3DVECTOR3(x, y, z);

	m_iShowTime = theIni.GetValueAsInt("ShowModel", "showtime", 1000);
	m_iStopTime = theIni.GetValueAsInt("ShowModel", "stoptime", 2000);
	m_iNewModelTime = theIni.GetValueAsInt("ShowModel", "newmodeltime", 1000);
	
	theIni.Close();

	// load resources
	m_pLoginBack = CreateSprite("LoginWorld\\LoginBack.dds");
	if (!m_pLoginBack) return false;

/*	m_pCreateBackHuman = CreateSprite("LoginWorld\\Creat_2.dds");
	if (!m_pCreateBackHuman) return false;
	
	m_pCreateBackWild = CreateSprite("LoginWorld\\Creat_3.dds");
	if (!m_pCreateBackWild) return false;
*/
	AfxGetGFXExMan()->SetPriority(5);

	m_idGfxLogin = CreateGfx("界面\\诛仙仙境最终.gfx");
	if (m_idGfxLogin==0) return false;
	
	m_idGfxCreateBack = CreateGfx("界面\\焚昊创建角色界面.gfx");
	if (m_idGfxCreateBack==0) return false;
	
/*	m_idGfxSelectBack = CreateGfx("界面\\仙魔人物选择界面.gfx");
	if (m_idGfxSelectBack==0) return false;
	
	m_idGfxCreateWild = CreateGfx("界面\\焚昊创建角色界面神族人物出现.gfx");
	if (m_idGfxCreateWild==0) return false;
	
	m_idGfxCreateHuman = CreateGfx("界面\\焚昊创建角色界面人族角色出现.gfx");
	if (m_idGfxCreateHuman==0) return false;
*/
	m_iGfxSelectRoleBack	 = CreateGfx("界面\\水墨选择人物界面背景.gfx");
	if(m_iGfxSelectRoleBack == 0) return false;
	m_iGfxHumanCreateBack	 = CreateGfx("界面\\水墨人族界面背景.gfx"); 
	if(m_iGfxHumanCreateBack == 0) return false;
	m_iGfxWildCreateBack	 = CreateGfx("界面\\水墨神裔界面背景.gfx");
	if(m_iGfxWildCreateBack == 0) return false;
	m_iGfxTianmaiCreateBack  = CreateGfx("界面\\水墨天脉界面背景.gfx");
	if(m_iGfxTianmaiCreateBack == 0) return false;
	
	m_pLogo = CreateSprite("Loading_Logo.tga");
	if (!m_pLogo) return false;
	m_pLogo->SetPosition(g_pGame->GetConfigs()->GetSystemSettings().iRndWidth/2, 0);
	m_pLogo->SetLocalCenterPos(m_pLogo->GetWidth()/2, 0);
	
	m_pKrLogo = CreateSprite("kr\\krtest.dds");
	if (!m_pKrLogo) return false;
	float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	m_pKrLogo->SetPosition(g_pGame->GetConfigs()->GetSystemSettings().iRndWidth - int((m_pKrLogo->GetWidth() + 40)*sx), g_pGame->GetConfigs()->GetSystemSettings().iRndHeight/3);

	bval = AUILuaManager::Init(pA3DEngine, pA3DDevice, szDCFile);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	
	LoadUserServer(false);

	// Load mail suffix list
	LoadMailSuffixList();
	

	memset(m_szUserName, 0, 256);
	// remember user name?
	GetPrivateProfileString(_AL("Server"), _AL("CurrentUserName"), _AL(""), 
		m_szUserName, 200, _AL("userdata\\currentserver.ini"));

	AUIListBox* pMailList = (AUIListBox*)GetDialog("Win_Login")->GetDlgItem("Lst_Mail");
	if(pMailList)
	{
		pMailList->Show(false);
	}

	if(wcscmp(m_szUserName, _AL("")) != 0)
	{
		GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->SetText(m_szUserName);
		((PAUICHECKBOX)GetDialog("Win_Login")->GetDlgItem("Chk_Account"))->Check(true);
	}

	InitLoginLayout();
	// 片头cg，Init调用可能会崩并且无法解决
	bool bSkipMovie = g_pGame->GetConfigs()->GetIsSkipMovie();
	g_pGame->GetConfigs()->SetIsSkipMovie(true);
	if (!bSkipMovie)
	{
		m_pIntroMovie = new AMVideoClip;
		if( !m_pIntroMovie->Init(g_pGame->GetA3DEngine()->GetA3DDevice(), g_pGame->GetA3DEngine()->GetAMVideoEngine(), "video\\intro.wmv") )
		{
			m_pIntroMovie->Release();
			delete m_pIntroMovie;
			m_pIntroMovie = NULL;
		}
	}
	g_pGame->GetConfigs()->SetIsSkipMovie(bSkipMovie);

	// task daily，感觉比较无聊的功能
	tm *time = glb_GetFormatLocalTime();
	int mstart = (time->tm_wday * 24 + time->tm_hour) * 60 + time->tm_min;
	int mend = mstart + 30;
	AWScriptFile s;
	if( s.Open("surfaces\\ingame\\taskdaily.txt") )
	{
		while( !s.IsEnd() )
		{
			if( !s.GetNextToken(true) )
				break;
			
			TaskDailyInfo TDInfo;
			DWORD i;
			for( i = 0; i<7; i++)
				TDInfo.bDays[i] = false;
			for( i = 0; i<15; i++)
				TDInfo.bLines[i] = false;
			TDInfo.taskType = CDlgTaskDaily::TASKDAILY_TYPE_MIN;
			bool bDisplay = false;
			
			BEGIN_FAKE_WHILE2
				CSplit sp = s.m_szToken;
			CSPLIT_STRING_VECTOR vec = sp.Split(_AL(","));
			for( i = 0; i < vec.size(); i++ )
			{
				TDInfo.bDays[a_atoi(vec[i])-1] = true;
			}
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strTime = s.m_szToken;
			swscanf(s.m_szToken,_AL("%d:%d-%d:%d"),&TDInfo.tmStartTime.tm_hour, 
				&TDInfo.tmStartTime.tm_min, &TDInfo.tmEncTime.tm_hour, &TDInfo.tmEncTime.tm_min);
			TDInfo.bNoTimeLimit = true;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strName = s.m_szToken;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strLevelInfo = s.m_szToken;
			int len = TDInfo.strLevelInfo.GetLength();
			int loc = TDInfo.strLevelInfo.Find('-');
			TDInfo.ulMinLevel = 1;
			TDInfo.ulMaxLevel = 10000;
			if(-1==loc)
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.ulMaxLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1062),TDInfo.ulMaxLevel);
			}
			else if(loc+1 == len)
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1063),TDInfo.ulMinLevel);
			}
			else
			{
				TDInfo.ulMinLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo = TDInfo.strLevelInfo.Mid(loc+1);
				TDInfo.ulMaxLevel = a_atoi(TDInfo.strLevelInfo);
				TDInfo.strLevelInfo.Format(GetStringFromTable(1064),TDInfo.ulMinLevel,TDInfo.ulMaxLevel);
			}
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.bLoginShow = a_atoi(s.m_szToken)>0? true:false;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.bSpecial = a_atoi(s.m_szToken)>0? true:false;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strNPC = s.m_szToken;
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strType = s.m_szToken;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.iNumMax = a_atoi(s.m_szToken);
			if( TDInfo.iNumMax<=0 )
				TDInfo.bNoCountLimit = true;
			else
				TDInfo.bNoCountLimit = false;
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.uiCommend = a_atoi(s.m_szToken);
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strLineInfo = s.m_szToken;
			
			CHECK_BREAK2(s.GetNextToken(true));
			sp = s.m_szToken;
			vec = sp.Split(_AL(","));
			TDInfo.uiNumID = vec.size();
			TDInfo.iCurTaskShow = 0;
			for( i = 0; i < TDInfo.uiNumID; i++ )
			{
				TDInfo.uiTaskID[i] = a_atoi(vec[i]);
			}
			
			CHECK_BREAK2(s.GetNextToken(true));
			TDInfo.strDescription = s.m_szToken;
			
			END_FAKE_WHILE2;
			
			if ( TDInfo.bLoginShow && TDInfo.uiCommend == 5 )
			{
				if (TDInfo.bDays[(time->tm_wday + 6) % 7] && 
					0 == TDInfo.tmStartTime.tm_hour && 0 == TDInfo.tmStartTime.tm_min &&
					24 == TDInfo.tmEncTime.tm_hour && 0 == TDInfo.tmEncTime.tm_min )
				{
					m_vecTaskInfoAllDay.push_back(TDInfo);
				}
				else
				{
					if( TDInfo.bDays[(time->tm_wday + 6) % 7] )
					{
						int mtaskstart = (time->tm_wday * 24 + TDInfo.tmStartTime.tm_hour) * 60 + TDInfo.tmStartTime.tm_min;
						int mtaskend = (time->tm_wday * 24 + TDInfo.tmEncTime.tm_hour) * 60 + TDInfo.tmEncTime.tm_min;
						if( mstart <= mtaskend && mend >= mtaskstart )
							m_vecTaskInfo.push_back(TDInfo);
					}
					if( TDInfo.bDays[time->tm_wday] )
					{
						int mtaskstart = ((time->tm_wday + 1)* 24 + TDInfo.tmStartTime.tm_hour) * 60 + TDInfo.tmStartTime.tm_min;
						int mtaskend = ((time->tm_wday + 1) * 24 + TDInfo.tmEncTime.tm_hour) * 60 + TDInfo.tmEncTime.tm_min;
						if( mstart <= mtaskend && mend >= mtaskstart )
							m_vecTaskInfo.push_back(TDInfo);
					}
				}
			}
			
		}
		s.Close();
	}


	// init dialogs
	char szText[20];
	for(i = 0; i < 8; i++ )
	{
		sprintf(szText, "Rdo_Char%d", i + 1);
		GetDialog("Win_CharList")->GetDlgItem(szText)->Show(false);
		sprintf(szText, "Img_Char%d", i + 1);
		GetDialog("Win_CharList")->GetDlgItem(szText)->Show(false);
		sprintf(szText, "Gfx_%d", i + 1);
		GetDialog("Win_CharList")->GetDlgItem(szText)->Show(false);
		sprintf(szText, "Img_Prof%d", i + 1);
		GetDialog("Win_CharList")->GetDlgItem(szText)->Show(false);
	}

	// 掉线以后显示
	ShowCombineDialogs("Login");

 	PAUIDIALOG pDlg = GetDialog("Win_CharList_Cover");
	GetDialog("Win_CharList_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover")->GetDlgItem("Img_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover")->SetPosEx(0,0);
	GetDialog("Win_CharList_Cover2")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover2")->SetPosEx((m_rcWindow.right - m_rcWindow.left)/2,0);
	GetDialog("Win_CharList_Cover2")->GetDlgItem("Img_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);

	PAUILABEL pLabKrText = (PAUILABEL)GetDialog("Win_Login")->GetDlgItem("Lab_Krtext");
	pLabKrText->Show(false);
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
 	if( 0 == stricmp(szCountry, "kr"))
 		pLabKrText->Show(true);

	m_iCameraPathWeight = 0;
	m_dwStartRaceTime = 0;
	m_bEnter = false;
	m_cPwWarn = 0;

	ChangeScene(LOGIN_SCENE_SELCHAR);

	return true;
}

void CECLoginUIMan::InitLoginLayout()
{
	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	bool isTWVersion = ( 0 == stricmp(szCountry, "tw")); 
	AUIDialog* pDlg = GetDialog("Win_Login");
	if(pDlg)
	{
	   AString strSuffix = "Rdo_";
	   for (int i =1; i<=3;++i)
	   {
		   AString strName ;
		   strName.Format("%s%d",strSuffix,i);
           AUIObject* pRdo = GetDialog("Win_Login")->GetDlgItem(strName);
		   if(pRdo)
			 pRdo->Show(isTWVersion);
	   }
	}
	if(isTWVersion)
	{
		int index = pDlg->GetCheckedRadioButton(1);
		if (index<0)
		{
	    	pDlg->CheckRadioButton(1, 0);
		}
	}
}
//	Release manager
bool CECLoginUIMan::Release(void)
{
	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
	float fFOV, fFront, fBack, fRatio;
	// 保证上次退出在摄像机模式下登陆游戏摄像机正常
	pCamera->GetProjectionParam(&fFOV, &fFront, &fBack, &fRatio);
	pCamera->SetProjectionParam(fFOV, fFront, 2000.0f, fRatio);
	// 登陆界面强行修改了游戏配置，进入游戏恢复
	g_pGame->GetConfigs()->ResumeSystemSettings();

	AfxGetGFXExMan()->Release2DGfx(m_idGfxLogin);
	AfxGetGFXExMan()->Release2DGfx(m_idGfxCreateBack);
/*	AfxGetGFXExMan()->Release2DGfx(m_idGfxSelectBack);
	AfxGetGFXExMan()->Release2DGfx(m_idGfxCreateHuman);
	AfxGetGFXExMan()->Release2DGfx(m_idGfxCreateWild);
*/
	AfxGetGFXExMan()->Release2DGfx(m_iGfxSelectRoleBack);
	AfxGetGFXExMan()->Release2DGfx(m_iGfxHumanCreateBack);
	AfxGetGFXExMan()->Release2DGfx(m_iGfxWildCreateBack);
	AfxGetGFXExMan()->Release2DGfx(m_iGfxTianmaiCreateBack);
	A3DRELEASE(m_pIntroMovie);
	A3DRELEASE(m_pLogo);
	A3DRELEASE(m_pKrLogo);
	A3DRELEASE(m_pLoginBack);
//	A3DRELEASE(m_pCreateBackHuman);
//	A3DRELEASE(m_pCreateBackWild);

	int i, j;

	for (i = 0; i < int(m_vecRoleModel.size()); i++)
		A3DRELEASE(m_vecRoleModel[i]);

	m_vecRoleModel.clear();
	abase::vector<GNET::RoleInfo>& vecRoleInfo = g_pGame->GetGameRun()->GetVecRoleInfo();
	vecRoleInfo = m_vecRoleInfo;
	m_vecRoleInfo.clear();
	
	for (i=0; i<LOGIN_MODEL_NUM; i++)
		for (j = 0; j < CECLOGINUIMAN_NUM_GENDERS; j++)
			A3DRELEASE(m_aModel[i][j]);

	AIniFile theIni;
	char szIniFile[MAX_PATH];

	
	
	theIni.WriteFloatValue("RoleList", "roleX", m_PosRole[LOGIN_SCENE_SELCHAR].x);
	theIni.WriteFloatValue("RoleList", "roleY", m_PosRole[LOGIN_SCENE_SELCHAR].y);
	theIni.WriteFloatValue("RoleList", "roleZ", m_PosRole[LOGIN_SCENE_SELCHAR].z);
	
	theIni.WriteFloatValue("RoleList", "camX", m_PosCamera[LOGIN_SCENE_SELCHAR].x);
	theIni.WriteFloatValue("RoleList", "camY", m_PosCamera[LOGIN_SCENE_SELCHAR].y);
	theIni.WriteFloatValue("RoleList", "camZ", m_PosCamera[LOGIN_SCENE_SELCHAR].z);
	
	theIni.WriteFloatValue("RoleList", "dirX", m_DirCamera[LOGIN_SCENE_SELCHAR].x);
	theIni.WriteFloatValue("RoleList", "dirY", m_DirCamera[LOGIN_SCENE_SELCHAR].y);
	theIni.WriteFloatValue("RoleList", "dirZ", m_DirCamera[LOGIN_SCENE_SELCHAR].z);
	
	
	theIni.WriteFloatValue("CreateHuman", "roleX", m_PosRole[LOGIN_SCENE_CREATE_HUMAN].x);
	theIni.WriteFloatValue("CreateHuman", "roleY", m_PosRole[LOGIN_SCENE_CREATE_HUMAN].y);
	theIni.WriteFloatValue("CreateHuman", "roleZ", m_PosRole[LOGIN_SCENE_CREATE_HUMAN].z);
	
	theIni.WriteFloatValue("CreateHuman", "camX", m_PosCamera[LOGIN_SCENE_CREATE_HUMAN].x);
	theIni.WriteFloatValue("CreateHuman", "camY", m_PosCamera[LOGIN_SCENE_CREATE_HUMAN].y);
	theIni.WriteFloatValue("CreateHuman", "camZ", m_PosCamera[LOGIN_SCENE_CREATE_HUMAN].z);
	
	theIni.WriteFloatValue("CreateHuman", "dirX", m_DirCamera[LOGIN_SCENE_CREATE_HUMAN].x);
	theIni.WriteFloatValue("CreateHuman", "dirY", m_DirCamera[LOGIN_SCENE_CREATE_HUMAN].y);
	theIni.WriteFloatValue("CreateHuman", "dirZ", m_DirCamera[LOGIN_SCENE_CREATE_HUMAN].z);
	
	
	theIni.WriteFloatValue("CreateWild", "roleX", m_PosRole[LOGIN_SCENE_CREATE_WILD].x);
	theIni.WriteFloatValue("CreateWild", "roleY", m_PosRole[LOGIN_SCENE_CREATE_WILD].y);
	theIni.WriteFloatValue("CreateWild", "roleZ", m_PosRole[LOGIN_SCENE_CREATE_WILD].z);
	
	theIni.WriteFloatValue("CreateWild", "camX", m_PosCamera[LOGIN_SCENE_CREATE_WILD].x);
	theIni.WriteFloatValue("CreateWild", "camY", m_PosCamera[LOGIN_SCENE_CREATE_WILD].y);
	theIni.WriteFloatValue("CreateWild", "camZ", m_PosCamera[LOGIN_SCENE_CREATE_WILD].z);
	
	theIni.WriteFloatValue("CreateWild", "dirX", m_DirCamera[LOGIN_SCENE_CREATE_WILD].x);
	theIni.WriteFloatValue("CreateWild", "dirY", m_DirCamera[LOGIN_SCENE_CREATE_WILD].y);
	theIni.WriteFloatValue("CreateWild", "dirZ", m_DirCamera[LOGIN_SCENE_CREATE_WILD].z);

	theIni.WriteFloatValue("CreateRaceQianJi", "roleX", m_PosRole[LOGIN_SCENE_CREATE_QIANJI].x);
	theIni.WriteFloatValue("CreateRaceQianJi", "roleY", m_PosRole[LOGIN_SCENE_CREATE_QIANJI].y);
	theIni.WriteFloatValue("CreateRaceQianJi", "roleZ", m_PosRole[LOGIN_SCENE_CREATE_QIANJI].z);

	theIni.WriteFloatValue("CreateRaceQianJi", "camX", m_PosCamera[LOGIN_SCENE_CREATE_QIANJI].x);
	theIni.WriteFloatValue("CreateRaceQianJi", "camY", m_PosCamera[LOGIN_SCENE_CREATE_QIANJI].y);
	theIni.WriteFloatValue("CreateRaceQianJi", "camZ", m_PosCamera[LOGIN_SCENE_CREATE_QIANJI].z);

	theIni.WriteFloatValue("CreateRaceQianJi", "dirX", m_DirCamera[LOGIN_SCENE_CREATE_QIANJI].x);
	theIni.WriteFloatValue("CreateRaceQianJi", "dirY", m_DirCamera[LOGIN_SCENE_CREATE_QIANJI].y);
	theIni.WriteFloatValue("CreateRaceQianJi", "dirZ", m_DirCamera[LOGIN_SCENE_CREATE_QIANJI].z);

	theIni.WriteFloatValue("CreateRaceYingZhao", "roleX", m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO].x);
	theIni.WriteFloatValue("CreateRaceYingZhao", "roleY", m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO].y);
	theIni.WriteFloatValue("CreateRaceYingZhao", "roleZ", m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO].z);

	theIni.WriteFloatValue("CreateRaceYingZhao", "camX", m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO].x);
	theIni.WriteFloatValue("CreateRaceYingZhao", "camY", m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO].y);
	theIni.WriteFloatValue("CreateRaceYingZhao", "camZ", m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO].z);

	theIni.WriteFloatValue("CreateRaceYingZhao", "dirX", m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].x);
	theIni.WriteFloatValue("CreateRaceYingZhao", "dirY", m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].y);
	theIni.WriteFloatValue("CreateRaceYingZhao", "dirZ", m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].z);


	theIni.WriteFloatValue("CloseUpHuman", "camX", m_CloseUpPosCamera[0][0].x);
	theIni.WriteFloatValue("CloseUpHuman", "camY", m_CloseUpPosCamera[0][0].y);
	theIni.WriteFloatValue("CloseUpHuman", "camZ", m_CloseUpPosCamera[0][0].z);
	
	theIni.WriteFloatValue("CloseUpHuman", "dirX", m_CloseUpDirCamera[0][0].x);
	theIni.WriteFloatValue("CloseUpHuman", "dirY", m_CloseUpDirCamera[0][0].y);
	theIni.WriteFloatValue("CloseUpHuman", "dirZ", m_CloseUpDirCamera[0][0].z);
	
	theIni.WriteFloatValue("CloseUpHumanFemale", "camX", m_CloseUpPosCamera[1][0].x);
	theIni.WriteFloatValue("CloseUpHumanFemale", "camY", m_CloseUpPosCamera[1][0].y);
	theIni.WriteFloatValue("CloseUpHumanFemale", "camZ", m_CloseUpPosCamera[1][0].z);
	
	theIni.WriteFloatValue("CloseUpHumanFemale", "dirX", m_CloseUpDirCamera[1][0].x);
	theIni.WriteFloatValue("CloseUpHumanFemale", "dirY", m_CloseUpDirCamera[1][0].y);
	theIni.WriteFloatValue("CloseUpHumanFemale", "dirZ", m_CloseUpDirCamera[1][0].z);
	
	theIni.WriteFloatValue("CloseUpWildSmall", "camX", m_CloseUpPosCamera[0][1].x);
	theIni.WriteFloatValue("CloseUpWildSmall", "camY", m_CloseUpPosCamera[0][1].y);
	theIni.WriteFloatValue("CloseUpWildSmall", "camZ", m_CloseUpPosCamera[0][1].z);
	
	theIni.WriteFloatValue("CloseUpWildSmall", "dirX", m_CloseUpDirCamera[0][1].x);
	theIni.WriteFloatValue("CloseUpWildSmall", "dirY", m_CloseUpDirCamera[0][1].y);
	theIni.WriteFloatValue("CloseUpWildSmall", "dirZ", m_CloseUpDirCamera[0][1].z);
	
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "camX", m_CloseUpPosCamera[1][1].x);
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "camY", m_CloseUpPosCamera[1][1].y);
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "camZ", m_CloseUpPosCamera[1][1].z);
	
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "dirX", m_CloseUpDirCamera[1][1].x);
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "dirY", m_CloseUpDirCamera[1][1].y);
	theIni.WriteFloatValue("CloseUpWildSmallFemale", "dirZ", m_CloseUpDirCamera[1][1].z);
	
	theIni.WriteFloatValue("CloseUpWildNormal", "camX", m_CloseUpPosCamera[0][2].x);
	theIni.WriteFloatValue("CloseUpWildNormal", "camY", m_CloseUpPosCamera[0][2].y);
	theIni.WriteFloatValue("CloseUpWildNormal", "camZ", m_CloseUpPosCamera[0][2].z);
	
	theIni.WriteFloatValue("CloseUpWildNormal", "dirX", m_CloseUpDirCamera[0][2].x);
	theIni.WriteFloatValue("CloseUpWildNormal", "dirY", m_CloseUpDirCamera[0][2].y);
	theIni.WriteFloatValue("CloseUpWildNormal", "dirZ", m_CloseUpDirCamera[0][2].z);
	
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "camX", m_CloseUpPosCamera[1][2].x);
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "camY", m_CloseUpPosCamera[1][2].y);
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "camZ", m_CloseUpPosCamera[1][2].z);
	
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "dirX", m_CloseUpDirCamera[1][2].x);
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "dirY", m_CloseUpDirCamera[1][2].y);
	theIni.WriteFloatValue("CloseUpWildNormalFemale", "dirZ", m_CloseUpDirCamera[1][2].z);
		
	theIni.WriteFloatValue("CloseUpWildBig", "camX", m_CloseUpPosCamera[0][3].x);
	theIni.WriteFloatValue("CloseUpWildBig", "camY", m_CloseUpPosCamera[0][3].y);
	theIni.WriteFloatValue("CloseUpWildBig", "camZ", m_CloseUpPosCamera[0][3].z);
	
	theIni.WriteFloatValue("CloseUpWildBig", "dirX", m_CloseUpDirCamera[0][3].x);
	theIni.WriteFloatValue("CloseUpWildBig", "dirY", m_CloseUpDirCamera[0][3].y);
	theIni.WriteFloatValue("CloseUpWildBig", "dirZ", m_CloseUpDirCamera[0][3].z);
	
	theIni.WriteFloatValue("CloseUpWildBigFemale", "camX", m_CloseUpPosCamera[1][3].x);
	theIni.WriteFloatValue("CloseUpWildBigFemale", "camY", m_CloseUpPosCamera[1][3].y);
	theIni.WriteFloatValue("CloseUpWildBigFemale", "camZ", m_CloseUpPosCamera[1][3].z);
	
	theIni.WriteFloatValue("CloseUpWildBigFemale", "dirX", m_CloseUpDirCamera[1][3].x);
	theIni.WriteFloatValue("CloseUpWildBigFemale", "dirY", m_CloseUpDirCamera[1][3].y);
	theIni.WriteFloatValue("CloseUpWildBigFemale", "dirZ", m_CloseUpDirCamera[1][3].z);


	theIni.WriteFloatValue("CloseUpQianJi", "camX", m_CloseUpPosCamera[0][4].x);
	theIni.WriteFloatValue("CloseUpQianJi", "camY", m_CloseUpPosCamera[0][4].y);
	theIni.WriteFloatValue("CloseUpQianJi", "camZ", m_CloseUpPosCamera[0][4].z);

	theIni.WriteFloatValue("CloseUpQianJi", "dirX", m_CloseUpDirCamera[0][4].x);
	theIni.WriteFloatValue("CloseUpQianJi", "dirY", m_CloseUpDirCamera[0][4].y);
	theIni.WriteFloatValue("CloseUpQianJi", "dirZ", m_CloseUpDirCamera[0][4].z);

	theIni.WriteFloatValue("CloseUpQianJiFemale", "camX", m_CloseUpPosCamera[1][4].x);
	theIni.WriteFloatValue("CloseUpQianJiFemale", "camY", m_CloseUpPosCamera[1][4].y);
	theIni.WriteFloatValue("CloseUpQianJiFemale", "camZ", m_CloseUpPosCamera[1][4].z);

	theIni.WriteFloatValue("CloseUpQianJiFemale", "dirX", m_CloseUpDirCamera[1][4].x);
	theIni.WriteFloatValue("CloseUpQianJiFemale", "dirY", m_CloseUpDirCamera[1][4].y);
	theIni.WriteFloatValue("CloseUpQianJiFemale", "dirZ", m_CloseUpDirCamera[1][4].z);

	theIni.WriteFloatValue("CloseUpYingZhao", "camX", m_CloseUpPosCamera[0][5].x);
	theIni.WriteFloatValue("CloseUpYingZhao", "camY", m_CloseUpPosCamera[0][5].y);
	theIni.WriteFloatValue("CloseUpYingZhao", "camZ", m_CloseUpPosCamera[0][5].z);

	theIni.WriteFloatValue("CloseUpYingZhao", "dirX", m_CloseUpDirCamera[0][5].x);
	theIni.WriteFloatValue("CloseUpYingZhao", "dirY", m_CloseUpDirCamera[0][5].y);
	theIni.WriteFloatValue("CloseUpYingZhao", "dirZ", m_CloseUpDirCamera[0][5].z);

	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "camX", m_CloseUpPosCamera[1][5].x);
	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "camY", m_CloseUpPosCamera[1][5].y);
	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "camZ", m_CloseUpPosCamera[1][5].z);

	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "dirX", m_CloseUpDirCamera[1][5].x);
	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "dirY", m_CloseUpDirCamera[1][5].y);
	theIni.WriteFloatValue("CloseUpYingZhaoFemale", "dirZ", m_CloseUpDirCamera[1][5].z);

	theIni.WriteIntValue("ShowModel", "showtime", m_iShowTime);
	theIni.WriteIntValue("ShowModel", "stoptime", m_iStopTime);
	theIni.WriteIntValue("ShowModel", "newmodeltime", m_iNewModelTime);
	
	sprintf(szIniFile, "%s\\Configs\\SceneCtrl.ini", af_GetBaseDir());
	theIni.Save(szIniFile);
	theIni.Close();

	return AUILuaManager::Release();
}

//	On command
bool CECLoginUIMan::OnCommand(const char* szCommand, AUIDialog* pDlg)
{
	if (AUILuaManager::OnCommand(szCommand, pDlg))
		return true;
	
	if ( 0 == stricmp(pDlg->GetName(), "Win_Agreement") )
	{
		return OnCommand_Agreement(szCommand, pDlg);
	}
	if( 0 == stricmp(pDlg->GetName(), "Win_CharList") ||
		0 == stricmp(pDlg->GetName(), "Win_CharEnterWorld") ||
		0 == stricmp(pDlg->GetName(), "Win_CharCreate") ||
		0 == stricmp(pDlg->GetName(), "Win_CharReturn") ||
		0 == stricmp(pDlg->GetName(), "Win_CharDelete") ||
		0 == stricmp(pDlg->GetName(), "Win_CharList") ||
		0 == stricmp(pDlg->GetName(), "Win_Char_SelectLine") ||
		0 == stricmp(pDlg->GetName(), "Win_Char_SelectLineList") ||
		0 == stricmp(pDlg->GetName(), "Win_CharTaskTip") ||
		0 == stricmp(pDlg->GetName(), "Win_CharUpdateLog") ||
		0 == stricmp(pDlg->GetName(), "Win_Char_Rotate") ||
		0 == stricmp(pDlg->GetName(), "Win_PwdProtectHint"))
	{
		return OnCommand_SelChar(szCommand, pDlg);
	}
	if( 0 == stricmp(pDlg->GetName(), "Win_Login") ||
		0 == stricmp(pDlg->GetName(), "Win_LoginLogOut") ||
		0 == stricmp(pDlg->GetName(), "Win_LoginReplay") ||
		0 == stricmp(pDlg->GetName(), "Win_LoginReplaySelect") ||
		0 == stricmp(pDlg->GetName(), "Win_LoginWait")  ||
		0 == stricmp(pDlg->GetName(), "Win_Login_Link") ||
		0 == stricmp(pDlg->GetName(), "Win_Login_Anime") )
	{
		return OnCommand_Login(szCommand, pDlg);
	}
	if( 0 == stricmp(pDlg->GetName(), "Win_Softkb") ||
		0 == stricmp(pDlg->GetName(), "Win_Softkb2") )
		return OnCommand_Softkb(szCommand, pDlg);
	if( 0 == stricmp(pDlg->GetName(), "Win_Create") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Human") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Wild") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Race03") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Prof01") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Prof02") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_Prof03") ||
		0 == stricmp(pDlg->GetName(), "Win_CreateName") ||
		0 == stricmp(pDlg->GetName(), "Win_PopID") ||
		0 == stricmp(pDlg->GetName(), "Win_CreateConfirm") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_HumanEnter") ||
		0 == stricmp(pDlg->GetName(), "Win_Create_WildEnter"))
		return OnCommand_NewChar(szCommand, pDlg);
	if( 0 == stricmp(pDlg->GetName(), "Win_PwdProtect") ||
		0 == stricmp(pDlg->GetName(), "Win_PwdProtect1")||
		0 == stricmp(pDlg->GetName(), "Win_PwdProtect_Phone1") )
		return OnCommand_PwdProtect(szCommand, pDlg);
	if(0 == stricmp(pDlg->GetName(), "Win_PwdProtect_Phone2"))
		return OnCommand_PwdProtectResponse(szCommand, pDlg);

	return false;
}

bool CECLoginUIMan::OnCommand_Login(const char* szCommand, AUIDialog* pDlg)
{
	if( 0 == stricmp(szCommand, "login") )
	{
		if( 0 == stricmp(pDlg->GetName(), "Win_Login") && 
			 0 == a_stricmp(pDlg->GetDlgItem("Txt_CurrentServer")->GetText(), _AL("")) )
		{
			MessageBox("", GetStringFromTable(202), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return true;
		}
		if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0])
		{
			pDlg->GetDlgItem("DEFAULT_Txt_Account")->SetText(AS2AC(g_pGame->GetGameInit().szUserName));
			pDlg->GetDlgItem("Txt_PassWord")->SetText(AS2AC(g_pGame->GetGameInit().szToken));
		}
		else if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0])
		{
			pDlg->GetDlgItem("DEFAULT_Txt_Account")->SetText(AS2AC(g_pGame->GetGameInit().szUserName));
			pDlg->GetDlgItem("Txt_PassWord")->SetText(AS2AC(g_pGame->GetGameInit().szPassword));
			PAUICHECKBOX pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_Forcelog");
			pCheck->Check(true);
		}
		ACString strName = pDlg->GetDlgItem("DEFAULT_Txt_Account")->GetText();
		ACString strPass = pDlg->GetDlgItem("Txt_PassWord")->GetText();

		pDlg->GetDlgItem("DEFAULT_Txt_Account")->SetText(_AL(""));
		pDlg->GetDlgItem("Txt_PassWord")->SetText(_AL(""));
		pDlg->ChangeFocus(pDlg->GetDlgItem("DEFAULT_Txt_Account"));

		if( strName.GetLength() > 0 && strPass.GetLength() > 0 )
		{
			CECGameSession *pSession = g_pGame->GetGameSession();
			bool bKickUserFlag;
			PAUICHECKBOX pCheck = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_Forcelog");
			bKickUserFlag = pCheck->IsChecked();
			strName.MakeLower();
			CDlgLoginServerList *pDlgServerList = (CDlgLoginServerList *)GetDialog("Win_LoginServerList");
			pDlgServerList->ResetPort();
			int userNameSuffix = 0;
            const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
 	        if( 0 == stricmp(szCountry, "tw"))
			{
			   AUIDialog* pDlg = GetDialog("Win_Login");
			   if(pDlg)
			   {
			      userNameSuffix = pDlg->GetCheckedRadioButton(1);
			   }
			   if(userNameSuffix < 0)
				   userNameSuffix = 0;
			}
			pSession->SetUserName(AC2AS(strName),userNameSuffix);
			pSession->SetUserPassword(AC2AS(strPass));
			pSession->SetKickUserFlag(bKickUserFlag);
			pSession->Open();

//			GetDialog("Win_Login")->Show(false);
			g_pGame->GetGameRun()->ResetWallowInfo();
			g_pGame->GetGameRun()->SetLastLoginId(0);
			ShowCombineDialogs("Login", false);
			GetDialog("Win_Softkb")->Show(false);
			GetDialog("Win_LoginWait")->Show(true);
		}
		else
		{
			MSGBOX(203);
		}

		if(((PAUICHECKBOX)(pDlg->GetDlgItem("Chk_Account")))->IsChecked())
		{
			wcscpy(m_szUserName, strName);
		}
		else	// write empty string
		{
			wcscpy(m_szUserName, _AL(""));
		}
		
		WritePrivateProfileString(_AL("Server"), _AL("CurrentUserName"), m_szUserName, 
			_AL("userdata\\currentserver.ini"));		
	}
	else if ( strstr(szCommand, "Btn_Login3rd_") )
	{		
		AString strText = "";
		AIniFile iniFile;
		if(iniFile.Open("configs\\url.ini"))
		{
			strText = iniFile.GetValueAsString("Login", "login3rd", "");
			iniFile.Close();
		}
		if( strText != "" )
		{
			AString strUrl;
			strUrl.Format(strText, szCommand+strlen("Btn_Login3rd_"));
			ShowCombineDialogs("Login", false);
			CDlgLoginOtherPlatform *pLoginOtherPlat = (CDlgLoginOtherPlatform*)GetDialog("Win_Login_explorer");
			pLoginOtherPlat->Show(true);
			pLoginOtherPlat->NavigateUrl(strUrl);
		}
	}
	else if( 0 == stricmp(szCommand, "changeserver") )
	{
		GetDialog("Win_LoginServerList")->Show(true);
		GetDialog("Win_LoginServerList")->ChangeFocus(
			GetDialog("Win_LoginServerList")->GetDlgItem("Lst_Server"));
	}
	else if( 0 == stricmp(szCommand, "logout") )
	{
		PostQuitMessage(0);
	}
	else if( 0 == stricmp(szCommand, "replay") )
	{
		record_info_vec infovec;
		infovec.clear();
		EnumRecordFiles("userdata\\gamesave", infovec);
		GetDialog("Win_LoginReplaySelect")->Show(true, true);
		PAUILISTBOX pList = (PAUILISTBOX)GetDialog("Win_LoginReplaySelect")->GetDlgItem("List_Record");
		pList->ResetContent();
		for(DWORD i = 0; i < infovec.size(); i++)
		{
			ACString strTime, strText;
			strText = infovec[i].title;
			strTime.Format(_AL("%02d:%02d:%02d"), infovec[i].total_time / 3600000, infovec[i].total_time / 60000 % 60, infovec[i].total_time % 60000 / 1000);
			strText += _AL("\t") + strTime;
			long stime = infovec[i].save_time;
			stime -= g_pGame->GetTimeZoneBias() * 60; // localtime = UTC - bias, in which bias is in minutes
			tm *gtime = gmtime((const time_t*)&stime);
			strTime.Format(GetStringFromTable(221), gtime->tm_year + 1900,
				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
			strText += _AL("\t") + strTime;
			pList->AddString(strText);
			pList->SetItemHint(pList->GetCount() - 1, GetStringFromTable(225) + AS2AC(infovec[i].path));
		}
	}
	else if( 0 == stricmp(szCommand, "replayselect") )
	{
		GetDialog("Win_LoginReplaySelect")->Show(false);
		PAUILISTBOX pList = (PAUILISTBOX)GetDialog("Win_LoginReplaySelect")->GetDlgItem("List_Record");

		gr.set_file_to_open(AC2AS(pList->GetItemHint(pList->GetCurSel()) + a_strlen(GetStringFromTable(225))));
		gr.set_start_replay_flag();
	}
	else if( 0 == stricmp(szCommand, "IDCANCEL") )
	{
		if (m_pIntroMovie && m_pIntroMovie->IsPlaying())
		{
			m_pIntroMovie->Stop();
		}
		else if( 0 == stricmp(pDlg->GetName(), "Win_Login") )
			PostQuitMessage(0);
		else if( 0 == stricmp(pDlg->GetName(), "Win_LoginWait") )
		{
			GetDialog("Win_LoginWait")->Show(false);
//			GetDialog("Win_Login")->Show(true);
			ShowCombineDialogs("Login");
			g_pGame->GetGameSession()->Close();
		}
		else if( 0 == stricmp(pDlg->GetName(), "Win_LoginReplaySelect") )
			GetDialog("Win_LoginReplaySelect")->Show(false);
	}
	else if( 0 == stricmp(szCommand, "Btn_Signin") )
	{
		OpenURL("Login", "Register");
	}
	else if( 0 == stricmp(szCommand, "Btn_Website") )
	{
		OpenURL("Login", "official");
	}
	else if( 0 == stricmp(szCommand, "Btn_Account") )
	{
		OpenURL("Login", "accountsafe");
	}
	else if( 0 == stricmp(szCommand, "Btn_PwdBack") )
	{
		OpenURL("Login", "pwdBack");
	}
	else if ( 0 == stricmp(szCommand, "Chk_Anime") )
	{
		PAUICHECKBOX pChk = (PAUICHECKBOX)pDlg->GetDlgItem("Chk_Anime");
		g_pGame->GetConfigs()->SetIsSkipMovie(!pChk->IsChecked());
	}
	else if ( 0 == stricmp(szCommand, "Btn_Replay") )
	{
		if (m_pIntroMovie)
		{
			m_pIntroMovie->Release();
			delete m_pIntroMovie;
		}
		m_pIntroMovie = new AMVideoClip;
		if( !m_pIntroMovie->Init(g_pGame->GetA3DEngine()->GetA3DDevice(), g_pGame->GetA3DEngine()->GetAMVideoEngine(), "video\\intro.wmv") )
		{
			m_pIntroMovie->Release();
			delete m_pIntroMovie;
			m_pIntroMovie = NULL;
		}
		else
		{
			m_pIntroMovie->SetAutoScaleFlags(AMVID_FIT_X | AMVID_FIT_Y | AMVID_FIT_KEEPCONSTRAINT);
			m_pIntroMovie->Start(false);
		}
	}

	return true;
}

bool CECLoginUIMan::OpenURL(const char* szSect, const char* szKey)
{
	AIniFile theIni;
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\configs\\url.ini", af_GetBaseDir());
	AString strText = "";
	if( theIni.Open(szFile) )
	{	
		strText = theIni.GetValueAsString(szSect, szKey, "");
		theIni.Close();
	}
	if( strText != "" )
	{
		g_pGame->OpenURL(strText);
		return true;
	}

	return false;
}

bool CECLoginUIMan::ChangeScene(LOGIN_SCENE scene)
{
	m_iCurScene = scene;
	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
	
	ASSERT(scene >= 0 && scene < LOGIN_SCENE_NUM);
	
	pCamera->SetPos(m_PosCamera[scene]);
	pCamera->SetDirAndUp(m_DirCamera[scene], g_vAxisY);

	if (scene == LOGIN_SCENE_SELCHAR)
	{
        if(m_idCurRole>=0)
		{
		   CECLoginPlayer* pCurRole = m_vecRoleModel[m_idCurRole];

		   if(NULL != pCurRole && PROFRACE_YINGZHAO == pCurRole->GetProfRace())
		   {
			   pCamera->SetPos(m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO]);
			   pCamera->SetDirAndUp(m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO], g_vAxisY);
		   }
		}
	}
	float fFOV, fFront, fBack, fRatio;
	pCamera->GetProjectionParam(&fFOV, &fFront, &fBack, &fRatio);
	pCamera->SetProjectionParam(fFOV, fFront, 200.0f, fRatio);

	float t = (12 + 0 / 60.0f) / 24.0f; // 白天
	if (scene==LOGIN_SCENE_CREATE_WILD)
		t = 0;	// 晚上
	g_pGame->GetGameRun()->GetWorld()->SetTimeOfDay(t);
	if (m_iCurScene>=LOGIN_SCENE_CREATE_HUMAN)
	{
		GetDialog("Win_CreateName")->CheckRadioButton(1, 1);
	}
	
	return true;
}

bool CECLoginUIMan::OnCommand_Softkb(const char* szCommand, AUIDialog* pDlg)
{
	if (GetDialog("Win_Login")->IsShow())
	{
		PAUIDIALOG pDlgL = GetDialog("Win_Login");
		PAUIOBJECT pObj = pDlgL->GetDlgItem("Txt_PassWord");
		
		if( strstr(szCommand, "Char_") )
		{
			PAUIOBJECT pBtn = pDlg->GetDlgItem(szCommand);
			ACString strText = pObj->GetText();
			
			strText += pBtn->GetText();
			pObj->SetText(strText);
		}
		else if( 0 == stricmp(szCommand, "switch") )
		{
			ACHAR szText[40];
			PAUIOBJECT pObj = pDlg->GetDlgItem("Switch");
			DWORD dwUpper = 1 - pObj->GetData();
			char c, szName[40], cBase = dwUpper ? 'A' : 'a';
			
			for( c = 'a'; c <= 'z'; c++ )
			{
				sprintf(szName, "Char_%c", c);
				a_sprintf(szText, _AL("%c"), cBase + c - 'a');
				pDlg->GetDlgItem(szName)->SetText(szText);
			}
			pObj->SetData(dwUpper);
		}
		
		BringWindowToTop(pDlgL);
		pDlgL->ChangeFocus(pObj);
	}
	else if (GetDialog("Win_PwdProtect")->IsShow())
	{
		PAUIDIALOG pDlgL = GetDialog("Win_PwdProtect");
		if (!m_pwdCurObj)
		{
			return true;
		}
		PAUIEDITBOX pObj = (PAUIEDITBOX)m_pwdCurObj;
		AWString strText = pObj->GetText();
		if (strText.GetLength()==2)
		{
			return true;
		}
		
		if( strstr(szCommand, "Char_") )
		{
			PAUIOBJECT pBtn = pDlg->GetDlgItem(szCommand);
			ACString strText = pObj->GetText();
			const char *szText = pBtn->GetName();
			const char cTxt = szText[5];
			if (cTxt<='9' && cTxt>='0')
			{
				strText += (ACHAR)cTxt;
				pObj->SetText(strText);
			}
		}
		else if( 0 == stricmp(szCommand, "switch") )
		{
			ACHAR szText[40];
			PAUIOBJECT pObj = pDlg->GetDlgItem("Switch");
			DWORD dwUpper = 1 - pObj->GetData();
			char c, szName[40], cBase = dwUpper ? 'A' : 'a';
			
			for( c = 'a'; c <= 'z'; c++ )
			{
				sprintf(szName, "Char_%c", c);
				a_sprintf(szText, _AL("%c"), cBase + c - 'a');
				pDlg->GetDlgItem(szName)->SetText(szText);
			}
			pObj->SetData(dwUpper);
		}
		
		BringWindowToTop(pDlgL);
		pDlgL->ChangeFocus(pObj);
	}

	return true;
}

bool CECLoginUIMan::OnCommand_Agreement(const char* szCommand, AUIDialog* pDlg)
{
	if( stricmp(pDlg->GetName() , "Win_Agreement") == 0 )
	{
		if( 0 == stricmp(szCommand, "agree") )
		{
			pDlg->Show(false);
			ShowCombineDialogs("Login");
			LoadUserServer();
		}
		else if( 0 == stricmp(szCommand, "decline") )
		{
			PostQuitMessage(0);
		}
	}

	return true;
}

bool CECLoginUIMan::OnCommand_SelChar(const char* szCommand, AUIDialog* pDlg)
{
	if( 0 == stricmp(szCommand, "IDCANCEL") )
	{
		m_idCurRole = -1;

		m_vecRoleInfo.clear();
		for( int i = 0; i < int(m_vecRoleModel.size()); i++ )
			A3DRELEASE(m_vecRoleModel[i]);
		m_vecRoleModel.clear();
		m_vecRoleInfo.clear();
//		RebuildRoleList();
		RefreshPlayerList();
	
		ShowCombineDialogs("CharList", false);
		ShowCombineDialogs("Login");
		GetDialog("Win_Char_SelectLine")->Show(false);
		GetDialog("Win_Char_SelectLineList")->Show(false);
		GetDialog("Win_CharTaskTip")->Show(false);
		GetDialog("Win_CharUpdateLog")->Show(false);
		GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->SetText(m_szUserName);
		GetDialog("Win_Login")->GetDlgItem("Txt_PassWord")->SetText(_AL(""));

		g_pGame->GetGameSession()->Close();
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Win_CharList") && strstr(szCommand, "Btn_CharConsignCancel_"))
	{
		ShellExecuteA(NULL, "open", AC2AS(GetStringFromTable(3000)), NULL, NULL, SW_SHOWNORMAL);
	}
	else if ( 0 == stricmp(pDlg->GetName(), "Win_PwdProtectHint") && 0 == stricmp(szCommand, "Btn_Confirm"))
	{
		ShellExecuteA(NULL,"open", AC2AS(GetStringFromTable(256)), NULL, NULL, SW_NORMAL);
		return true;
	}
	else if( 0 == stricmp(szCommand, "selectline") )
	{
		PAUILISTBOX pList = (PAUILISTBOX)GetDialog("Win_Char_SelectLineList")->GetDlgItem("Lst_Line");
		if( pList->GetCurSel() >= 0 && pList->GetCurSel() < pList->GetCount() )
		{
			pList->SetData(pList->GetCurSel(), "cur_sel");
			PAUILABEL pLabel = (PAUILABEL)GetDialog("Win_Char_SelectLine")->GetDlgItem("Lab_Line");
			CSplit s = pList->GetText(pList->GetCurSel());
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			pLabel->SetText(vec[0]);
			pLabel->SetData(pList->GetItemData(pList->GetCurSel()));
			WritePrivateProfileString(_AL("Server"), _AL("CurrentLine"), pLabel->GetText(), 
				_AL("userdata\\currentserver.ini"));
		}
	}
	else if( 0 == stricmp(szCommand, "Chk_Expand") )	// 恢复checkbox状态，统一在onevent里面处理
	{
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Char_SelectLine")->GetDlgItem("Chk_Expand");
		pChk->Check(!pChk->IsChecked());
	}
	else if( 0 == stricmp(szCommand, "poplinelist") )
	{
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Char_SelectLine")->GetDlgItem("Chk_Expand");
		if (pChk->IsChecked())
		{
			pChk->Check(false);
			GetDialog("Win_Char_SelectLineList")->Show(false);
		}
		else
		{
			pChk->Check(true);
			OnCommand_SelChar("changeline", GetDialog("Win_Char_SelectLine"));
		}
	}
	else if( 0 == stricmp(szCommand, "tasktipok") )
	{
		GetDialog("Win_CharTaskTip")->Show(false);
		GetDialog("Win_CharUpdateLog")->Show(false);
		RefreshPlayerList();
		ShowCombineDialogs("CharList");
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Char_SelectLine")->GetDlgItem("Chk_Expand");
		pChk->Check(true);
		OnCommand_SelChar("changeline", GetDialog("Win_Char_SelectLine"));
	}
	else if( 0 == stricmp(szCommand, "changeline") )
	{
		CECGameRun::LineInfoVec lineinfo = g_pGame->GetGameRun()->GetLinesInfo();
		PAUILABEL pLabel = (PAUILABEL)GetDialog("Win_Char_SelectLine")->GetDlgItem("Lab_Line");
		PAUIDIALOG pDlg = GetDialog("Win_Char_SelectLineList");
		pDlg->AlignTo(GetDialog("Win_Char_SelectLine"),  AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Char_SelectLine")->GetDlgItem("Chk_Expand");
		pDlg->Show(pChk->IsChecked());
		PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Line");
		pList->ResetContent();
		int i;
		for(i = 0; i < (int)lineinfo.size(); i++ )
		{
//			int n = rand() % (pList->GetCount() + 1);
			ACString strLoad;
			A3DCOLOR color;
			if( lineinfo[i].attribute < 50 )
			{
				strLoad = GetStringFromTable(222);
				color = 0xFF00FF7F;
			}
			else if(  lineinfo[i].attribute < 90 )
			{
				strLoad = GetStringFromTable(223);
				color = 0xFFFFFF7F;
			}
			else
			{
				strLoad = GetStringFromTable(224);
				color = 0xFFFF0000;
			}
				
			pList->InsertString(i, lineinfo[i].name + _AL("\t") + strLoad);
			pList->SetItemTextColor(i, color, 1);
			pList->SetItemData(i, lineinfo[i].id);
		}
		for(i = 0; i < pList->GetCount(); i++ )
		{
			ACString strLine = pList->GetText(i);
			int n = strLine.Find('\t');
			if( n != -1 )
				strLine = strLine.Left(n);
			if( a_stricmp(strLine, pLabel->GetText()) == 0 )
			{
				pList->BringItemToTop(i);
				pList->SetItemTextColor(0, 0xFFFFCB4A, 0);
				break;
			}
		}
		pList->SetCurSel(0);
	}
	else if( 0 == stricmp(szCommand, "enterworld") )
	{
		if( m_idCurRole >= 0 && GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->IsEnabled() )
		{
			if( g_pGame->GetGameSession()->GetNetManager()->IsUseUsbKey() && g_pGame->GetGameSession()->GetNetManager()->IsUsbKeyExisting() )
			{
				MessageBox("", GetStringFromTable(228), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
			else
			{
				if(m_vecRoleInfo[m_idCurRole].status != _ROLE_STATUS_FROZEN)
					g_pGame->GetGameRun()->SetSelectedRoleInfo(m_vecRoleInfo[m_idCurRole]);
				PAUILABEL pLabel = (PAUILABEL)GetDialog("Win_Char_SelectLine")->GetDlgItem("Lab_Line");
				g_pGame->GetGameRun()->SetCurLine(pLabel->GetData());
				int nCurLine = pLabel->GetData();
				// 设置该线路是否开启了PK保护
				CECGameRun::LineInfoVec lineinfo = g_pGame->GetGameRun()->GetLinesInfo();
				for (size_t n = 0; n < lineinfo.size(); n++)
				{
					CECGameRun::LineInfo& info = lineinfo[n];

					if (info.id == nCurLine)
					{
						g_pGame->GetGameRun()->SetNoPKLine(info.bNoPKLine);
						break;
					}
				}
				g_pGame->GetGameSession()->SelectRole(m_vecRoleInfo[m_idCurRole].roleid, pLabel->GetData());
				m_bEnter = true;
//				OnCommand_SelChar("Rdo_Char2", GetDialog("Win_CharList"));
				CDlgBattleFactionWarControl::ResetFactionWarInfo();
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(false);
			}
		}
	}
	else if( 0 == stricmp(szCommand, "Btn_Create") )
	{
//		g_pGame->GetConfigs()->MakeSystemSettingsHighest(false);
		ShowCombineDialogs("CharList", false);
		ShowCombineDialogs("CreatePrev");
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(false);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(false);

		GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(false);
		RefreshPlayerList();
	}
	else if( 0 == stricmp(szCommand, "Btn_Delete") )
	{
		MessageBox("SelChar_DelChar", GetStringFromTable(206), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( 0 == stricmp(szCommand, "Btn_Restore") )
	{
		g_pGame->GetGameSession()->RestoreRole(m_vecRoleInfo[m_idCurRole].roleid);
	}
	else if( strstr(szCommand, "Rdo_Char") )
	{
		if (m_bEnter)
		{
			pDlg->CheckRadioButton(1, m_idCurRole + 1);
			return true;
		}
		int idRole = atoi(szCommand + strlen("Rdo_Char")) - 1;

		if( idRole < (int)m_vecRoleModel.size() && idRole != m_idCurRole )
		{
			if( !m_vecRoleModel[idRole] )
			{
				bool bval;
				CECLoginPlayer *pRole = new CECLoginPlayer(NULL);

				bval = pRole->Load(m_vecRoleInfo[idRole]);
				if( !bval ) AUI_ReportError(__LINE__, __FILE__);

				pRole->SetPos(m_PosRole[LOGIN_SCENE_SELCHAR]);
				A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_SELCHAR].x, 0, -m_DirCamera[LOGIN_SCENE_SELCHAR].z);
				vDirRole.Normalize();
				pRole->SetDirAndUp(vDirRole, g_vAxisY);

				
				if(PROFRACE_YINGZHAO == pRole->GetProfRace())
				{
				    pRole->SetPos(m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO]);
				    A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].x, 0, -m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].z);
				    vDirRole.Normalize();
				    pRole->SetDirAndUp(vDirRole, g_vAxisY);
				}
				if( m_vecRoleInfo[idRole].status == _ROLE_STATUS_NORMAL )
					pRole->Stand();
				else
					pRole->SitDown();
				m_vecRoleModel[idRole] = pRole;
			}

			GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->SetHint(_AL(""));
			if( m_vecRoleInfo[idRole].status == _ROLE_STATUS_NORMAL )
			{
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(true);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(true);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(false);
			}
			else if( m_vecRoleInfo[idRole].status == _ROLE_STATUS_FROZEN )
			{
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(true);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->SetHint(GetStringFromTable(231));
				GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(false);
			}
			else if( m_vecRoleInfo[idRole].status == _ROLE_STATUS_SELLING )
			{
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(false);
			}
			else
			{
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(true);
			}
			m_idCurRole = idRole;
			RefreshPlayerList();
			pDlg->CheckRadioButton(1, idRole + 1);
			A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();

			m_nCurProfession = m_vecRoleModel[idRole]->GetPhysique();
			if (m_vecRoleModel[idRole]->GetProfession()>=PROF_46 && m_vecRoleModel[idRole]->GetProfession()<=PROF_51)
			{
				m_nCurProfession = LOGIN_MODEL_SMALLNOEAR;
			}
			else if (m_vecRoleModel[idRole]->GetProfession()>=PROF_97 && m_vecRoleModel[idRole]->GetProfession()<=PROF_101)
			{
				m_nCurProfession = LOGIN_MODEL_BIG2;
			}
			else if (m_vecRoleModel[idRole]->GetProfession()>=PROF_57 && m_vecRoleModel[idRole]->GetProfession()<=PROF_61)
			{
				m_nCurProfession = LOGIN_MODEL_MIDDLE2;
			}
			if (m_vecRoleModel[idRole]->IsRidingOnPet())
			{
				pCamera->Move(pCamera->GetDirH() * -1.0f + A3DVECTOR3(0, .5f, 0));
				pCamera->TurnAroundAxis(A3DVECTOR3(0), A3DVECTOR3(0, 1.0f, 0), DEG2RAD(20.f));
			}

			m_vecRoleModel[idRole]->ShowPose();
#ifdef BACK_VERSION
			// for update terrain water's current water height data
			g_pGame->GetGameRun()->GetWorld()->GetTerrainWater()->UpdateWaterHeight();
#endif
		}
	}

	return true;
}

bool CECLoginUIMan::OnCommand_NewChar(const char* szCommand, AUIDialog* pDlg)
{
	int i, j;

	if ((0 == stricmp(szCommand, "Btn_Cancel")||0 == stricmp(szCommand, "IDCANCEL")) && 
		(GetDialog("Win_Create_Human")->IsShow()||GetDialog("Win_Create_Wild")->IsShow() || GetDialog("Win_Create_Race03")->IsShow()))
	{
//		g_pGame->GetConfigs()->MakeSystemSettingsHighest(false);
		m_iCurCreate = CREATE_TYPE_MIN;
		m_nCurProfession = -1;
		ShowCombineDialogs("CreateMain", false);
		ShowCombineDialogs("CreatePrev");

		
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(false);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(false);

		RefreshPlayerList();
	}
	else if (pDlg == GetDialog("Win_Create_HumanEnter") && strstr(szCommand, "Btn_"))
	{
		OnCommand_NewChar("confirmhuman", GetDialog("Win_Create"));
		GetDialog("Win_Create_Human")->CheckRadioButton(1, atoi(szCommand+strlen("Btn_")));
		OnCommand_NewChar("change", GetDialog("Win_Create_Human"));
	}
	else if (pDlg == GetDialog("Win_Create_WildEnter") && strstr(szCommand, "Btn_"))
	{
		OnCommand_NewChar("confirmwild", GetDialog("Win_Create"));
		GetDialog("Win_Create_Wild")->CheckRadioButton(1, atoi(szCommand+strlen("Btn_")));
		OnCommand_NewChar("change", GetDialog("Win_Create_Wild"));
	}
	else if( (0 == stricmp(szCommand, "Btn_Cancel") ||0 == stricmp(szCommand, "IDCANCEL"))
		&& pDlg == GetDialog("Win_Create"))
	{
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(false);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(false);
		m_iCurCreate = CREATE_TYPE_MIN;

		if( m_iCurCreate == CREATE_TYPE_MIN )
		{
//			g_pGame->GetConfigs()->MakeSystemSettingsHighest();
			ShowCombineDialogs("CreatePrev", false);
			ShowCombineDialogs("CharList");

			char szCommand[40] = "";
			sprintf(szCommand, "Rdo_Char%d", m_idCurRole + 1);
			m_idCurRole = -1;
			OnCommand_SelChar(szCommand, GetDialog("Win_CharList"));
			RefreshPlayerList();
		}
		else
		{
			m_iCurCreate = CREATE_TYPE_MIN;
			m_nCurProfession = -1;
			m_dwStartRaceTime = GetTickCount();
			GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(false);
			GetDialog("Win_Create_HumanEnter")->Show(false);
			GetDialog("Win_Create_WildEnter")->Show(false);
			//GetDialog("Win_Create_Race03")->Show(false);
		}
	}
	else if( 0 == stricmp(szCommand, "Btn_Human"))
	{
		if( m_iCurCreate == CREATE_TYPE_HUMAN )
			return 0;
		
		m_nCurProfession = LOGIN_MODEL_NORMAL;
		m_dwStartRaceTime = GetTickCount();
		GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
		//GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(false);
		//if( m_iCurCreate == CREATE_TYPE_WILD  || m_iCurCreate == CREATE_TYPE_RACE03)
		//{
		//	m_iCurCreate = CREATE_TYPE_MIN;
		//}
		//else
		{
			m_iCurCreate = CREATE_TYPE_HUMAN;
			//AfxGetGFXExMan()->Stop2DGfx(m_idGfxCreateHuman);
			//AfxGetGFXExMan()->Play2DGfx(m_idGfxCreateHuman);
			GetDialog("Win_CharList_Cover")->Show(false);
			GetDialog("Win_CharList_Cover2")->Show(false);
		}
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(true);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(false);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown())
		   ((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown())
		   ((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(true);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(false);
		
		GetDialog("Win_Create_HumanEnter")->Show(false);
		GetDialog("Win_Create_WildEnter")->Show(false);


		return 0;
	}
	else if( 0 == stricmp(szCommand, "Btn_Wild"))
	{
		if( m_iCurCreate == CREATE_TYPE_WILD )
			return 0;
		
		m_nCurProfession = LOGIN_MODEL_BIG;
		m_dwStartRaceTime = GetTickCount();
		GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
		//GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(false);
		//if( m_iCurCreate == CREATE_TYPE_HUMAN || m_iCurCreate == CREATE_TYPE_RACE03)
		//{
		//	m_iCurCreate = CREATE_TYPE_MIN;
		//}
		//else
		{
			m_iCurCreate = CREATE_TYPE_WILD;
			//AfxGetGFXExMan()->Stop2DGfx(m_idGfxCreateWild);
			//AfxGetGFXExMan()->Play2DGfx(m_idGfxCreateWild);
			GetDialog("Win_CharList_Cover")->Show(false);
			GetDialog("Win_CharList_Cover2")->Show(false);
		}
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(true);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(false);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Race03")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(true);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(false);
		//GetDialog("Win_Create_HumanEnter")->Show(false);
		GetDialog("Win_Create_WildEnter")->Show(false);

		return 0;
	}
	else if( 0 == stricmp(szCommand, "Btn_Race03"))
	{
		if( m_iCurCreate == CREATE_TYPE_RACE03 )
			return 0;

		m_nCurProfession = LOGIN_MODEL_NORMAL2;
		m_dwStartRaceTime = GetTickCount();
		GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
		//GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(false);
		//if(  m_iCurCreate == CREATE_TYPE_HUMAN || m_iCurCreate == CREATE_TYPE_WILD )
		//{
		//	m_iCurCreate = CREATE_TYPE_MIN;
		//}
		//else
		{
			m_iCurCreate = CREATE_TYPE_RACE03;
			//AfxGetGFXExMan()->Stop2DGfx(m_idGfxCreateWild);
			//AfxGetGFXExMan()->Play2DGfx(m_idGfxCreateWild);
			GetDialog("Win_CharList_Cover")->Show(false);
			GetDialog("Win_CharList_Cover2")->Show(false);
		}
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Human"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Wild"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDialog("Win_Create")->GetDlgItem("Btn_Race03"))->SetPushed(true);
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Human")))->GetGfxDown()->Stop();
		if(((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown())
			((PAUISTILLIMAGEBUTTON)(GetDialog("Win_Create")->GetDlgItem("Btn_Wild")))->GetGfxDown()->Stop();
		//GetDialog("Win_Create")->GetDlgItem("Gfx_1")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_2")->Show(false);
		//GetDialog("Win_Create")->GetDlgItem("Gfx_3")->Show(true);
		GetDialog("Win_Create_HumanEnter")->Show(false);
		GetDialog("Win_Create_WildEnter")->Show(false);

		return 0;
	}
	else if ( 0 == stricmp(szCommand, "DEFAULT_CharName") )
	{
		OnCommand_NewChar("confirm", NULL);
	}
	else if( 0 == stricmp(szCommand, "confirmhuman") )
	{
//		g_pGame->GetConfigs()->MakeSystemSettingsHighest();
		pDlg->Show(false);
		ShowCombineDialogs("CreatePrev",false);
		ShowCombineDialogs("CreateMain");
		RefreshPlayerList();
		GetDialog("Win_Create_Human")->CheckRadioButton(1,1);
		GetDialog("Win_Create_Human")->CheckRadioButton(2,1);
		GetDialog("Win_Create_Prof01")->CheckRadioButton(1,1);
		for (i=0; i<LOGIN_MODEL_NUM; i++)
		{
			for (j=0; j<CECLOGINUIMAN_NUM_GENDERS; j++)
				m_bStand[i][j] = 0;
		}
		
		m_nCurGender = 1;
		OnCommand_NewChar("change", GetDialog("Win_Create_Human"));
		GetDialog("Win_CreateName")->GetDlgItem("DEFAULT_CharName")->SetText(_AL(""));
		
		GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
	}
	else if( 0 == stricmp(szCommand, "confirmwild") )
	{
//		g_pGame->GetConfigs()->MakeSystemSettingsHighest();
		pDlg->Show(false);
		ShowCombineDialogs("CreatePrev",false);
		ShowCombineDialogs("CreateMain");
		RefreshPlayerList();
		GetDialog("Win_Create_Wild")->CheckRadioButton(1,1);
		GetDialog("Win_Create_Wild")->CheckRadioButton(2,1);
		GetDialog("Win_Create_Prof02")->CheckRadioButton(1,1);
		for (i=0; i<LOGIN_MODEL_NUM; i++)
		{
			for (j=0; j<CECLOGINUIMAN_NUM_GENDERS; j++)
				m_bStand[i][j] = 0;
		}
		
		m_nCurGender = 1;
		OnCommand_NewChar("change", GetDialog("Win_Create_Wild"));
		GetDialog("Win_CreateName")->GetDlgItem("DEFAULT_CharName")->SetText(_AL(""));
		
		GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
	}
	else if(0 == stricmp(szCommand,"confirmrace03"))
	{
		//g_pGame->GetConfigs()->MakeSystemSettingsHighest();
		pDlg->Show(false);
		ShowCombineDialogs("CreatePrev",false);
		ShowCombineDialogs("CreateMain");
		RefreshPlayerList();
		GetDialog("Win_Create_Race03")->CheckRadioButton(1,1);
		GetDialog("Win_Create_Race03")->CheckRadioButton(2,1);
		GetDialog("Win_Create_Prof03")->CheckRadioButton(1,1);
		for (i=0; i<LOGIN_MODEL_NUM; i++)
		{
			for (j=0; j<CECLOGINUIMAN_NUM_GENDERS; j++)
				m_bStand[i][j] = 0;
		}

		m_nCurGender = 1;
		OnCommand_NewChar("change", GetDialog("Win_Create_Race03"));
		GetDialog("Win_CreateName")->GetDlgItem("DEFAULT_CharName")->SetText(_AL(""));

		GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
	}
	else if( 0 == stricmp(szCommand, "Btn_Random") && (pDlg == GetDialog("Win_Create_Human")))
	{
		PAUICOMBOBOX pCombo1 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		PAUICOMBOBOX pCombo2 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		PAUICOMBOBOX pCombo3 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		int rand1, rand2, rand3;
		do
		{
			rand1 = rand() % pCombo1->GetCount();
			rand2 = rand() % pCombo2->GetCount();
			rand3 = rand() % pCombo3->GetCount();
		} while( rand1 == pCombo1->GetCurSel() && rand2 == pCombo2->GetCurSel() && rand3 == pCombo3->GetCurSel() );
		pCombo1->SetCurSel(rand1);
		pCombo2->SetCurSel(rand2);
		pCombo3->SetCurSel(rand3);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_Random") && (pDlg == GetDialog("Win_Create_Wild")))
	{
		PAUICOMBOBOX pCombo1 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		PAUICOMBOBOX pCombo2 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		PAUICOMBOBOX pCombo3 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
		PAUICOMBOBOX pCombo4 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
		PAUICOMBOBOX pCombo5 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		int rand1, rand2, rand3, rand4, rand5;
		do
		{
			rand1 = rand() % pCombo1->GetCount();
			rand2 = rand() % pCombo2->GetCount();
			rand3 = rand() % pCombo3->GetCount();
			rand4 = rand() % pCombo4->GetCount();
			rand5 = rand() % pCombo5->GetCount();
		} while( rand1 == pCombo1->GetCurSel() && rand2 == pCombo2->GetCurSel() &&
			rand3 == pCombo3->GetCurSel() && rand4 == pCombo4->GetCurSel() && rand5 == pCombo5->GetCurSel());
		pCombo1->SetCurSel(rand1);
		pCombo2->SetCurSel(rand2);
		pCombo3->SetCurSel(rand3);
		pCombo4->SetCurSel(rand4);
		pCombo5->SetCurSel(rand5);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_Random") && (pDlg == GetDialog("Win_Create_Race03")))
	{
		PAUICOMBOBOX pCombo1 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		PAUICOMBOBOX pCombo2 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		PAUICOMBOBOX pCombo3 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		//PAUICOMBOBOX pCombo4 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
		//PAUICOMBOBOX pCombo5 = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		int rand1, rand2, rand3, rand4, rand5;
		do
		{
			rand1 = rand() % pCombo1->GetCount();
			rand2 = rand() % pCombo2->GetCount();
			rand3 = rand() % pCombo3->GetCount();
			//rand4 = rand() % pCombo4->GetCount();
			//rand5 = rand() % pCombo5->GetCount();
		} while( rand1 == pCombo1->GetCurSel() && rand2 == pCombo2->GetCurSel() &&
			rand3 == pCombo3->GetCurSel() /*&& rand4 == pCombo4->GetCurSel() && rand5 == pCombo5->GetCurSel()*/);
		pCombo1->SetCurSel(rand1);
		pCombo2->SetCurSel(rand2);
		pCombo3->SetCurSel(rand3);
		//pCombo4->SetCurSel(rand4);
		//pCombo5->SetCurSel(rand5);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "confirm") )
	{
		CECLoginPlayer* pPlayer = m_aModel[m_nCurProfession][m_nCurGender];
		if( !pPlayer )
		{
			MSGBOX(210);
			return true;
		}

		ACString strName = GetDialog("Win_CreateName")->GetDlgItem("DEFAULT_CharName")->GetText();
		if( strName.GetLength() <= 0 )
		{
			MSGBOX(207);
			return true;
		}

		GNET::RoleInfo Info;
		PAUIDIALOG pDlg = GetDialog("Win_Create_Human");
		if (GetDialog("Win_Create_Wild")->IsShow())
		{
			pDlg = GetDialog("Win_Create_Wild");
		}
		else if(GetDialog("Win_Create_Race03")->IsShow())
		{
		    pDlg = GetDialog("Win_Create_Race03");
		}
		
		PAUICOMBOBOX pCombo;
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		Info.faceid = pPlayer->FaceHairIndex2Id(pCombo->GetCurSel());
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		Info.hairid = pPlayer->FaceHairIndex2Id(pCombo->GetCurSel());
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		Info.fashionid = pCombo->GetCurSel();
		if ( GetDialog("Win_Create_Wild")->IsShow())
		{
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
			Info.earid = pCombo->GetCurSel();
		}
		else
			Info.earid = 0;
		if (GetDialog("Win_Create_Wild")->IsShow())
		{
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
			Info.tailid = pCombo->GetCurSel();
		}
		else
			Info.tailid = 0;

		Info.gender = pDlg->GetCheckedRadioButton(2)-1;//m_nCurGender;
		if (GetDialog("Win_Create_Wild")->IsShow())
		{
			//switch(GetDialog("Win_Create_Wild")->GetCheckedRadioButton(1))
			switch(GetDialog("Win_Create_Prof02")->GetCheckedRadioButton(1))
			{
			case 1:
				Info.occupation = PROF_34;
				break;
			case 2:
				Info.occupation = PROF_40;
				break;
			case 3:
				Info.occupation = PROF_52;
				break;
			case 4:
				Info.occupation = PROF_46;
				break;
			case 5:
				Info.occupation = PROF_97;
				break;
			case 6:
				Info.occupation = PROF_57;
				break;
			default:
				Info.occupation = PROF_1;
			}
		}
		else if(GetDialog("Win_Create_Race03")->IsShow())
		{
			//switch(GetDialog("Win_Create_Race03")->GetCheckedRadioButton(1))
			switch(GetDialog("Win_Create_Prof03")->GetCheckedRadioButton(1))
			{
			case 1:
				Info.occupation = PROF_109;
				break;
			case 2:
				Info.occupation = PROF_103;
				break;
			default:
				Info.occupation = PROF_1;
			}
		}
		else
			Info.occupation = PROF_1;

		Info.name.replace((const ACHAR *)strName, strName.GetLength() * sizeof(ACHAR));
		PAUILABEL pEdit;
		pEdit = (PAUILABEL)GetDialog("Win_PopID")->GetDlgItem("Txt_1");
		const ACHAR *szText = pEdit->GetText();
		
		CECGameSession *pSession = g_pGame->GetGameSession();
		if (GetDialog("Win_PopID")->IsShow() && a_strlen(szText)==0)
		{
			MessageBox("Game_No_PopID", GetStringFromTable(257), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if (pSession->IsHasReferrer() || a_strlen(szText)==0)
		{
			pSession->CreateRole(Info,NULL);
		}
		else
			pSession->CreateRole(Info,szText);
	}
	else if( 0 == stricmp(szCommand, "facel") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "facer") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "hairl") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "hairr") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "earl") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "earr") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "taill") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "tailr") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_ClothL") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
		pCombo->SetCurSel(0);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_ClothR") )
	{
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
		pCombo->SetCurSel(0);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_EquipL") )
	{
		pDlg->CheckRadioButton(3, 1);
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
		int nSel = pCombo->GetCurSel() - 1;
		if( nSel < 0 ) nSel = pCombo->GetCount() - 1;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Btn_EquipR") )
	{
		pDlg->CheckRadioButton(3, 1);
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
		int nSel = pCombo->GetCurSel() + 1;
		if( nSel >= pCombo->GetCount() ) nSel = 0;
		pCombo->SetCurSel(nSel);
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Combo_Equip") )
	{
		pDlg->CheckRadioButton(3, 1);
		PAUICOMBOBOX pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
		OnCommand_NewChar("change", pDlg);
	}
	else if( 0 == stricmp(szCommand, "Rdo_Chenhuang1") || 0 == stricmp(szCommand, "Rdo_Chenhuang2") )
	{
		m_aModel[m_nCurProfession][m_nCurGender]->UpdateEquipmentsSkin(pDlg->GetCheckedRadioButton(3)-1);
	}
	else if( 0 == stricmp(szCommand, "change") && (pDlg == GetDialog("Win_Create_Human") || pDlg == GetDialog("Win_Create_Prof01")))
	{
	
		PAUIDIALOG pInfoDlg = GetDialog("Win_Create_Prof01");
		//PAUILABEL pLabIntro = (PAUILABEL)pInfoDlg->GetDlgItem("Lab_ProIntro");
		//pLabIntro->SetText(GetStringFromTable(1006+pInfoDlg->GetCheckedRadioButton(1)));

		static const char*  ImagePath[]  = 
		{
           "login\\icon\\profintro_01.tga",
		   "login\\icon\\profintro_02.tga",
		   "login\\icon\\profintro_03.tga",
		   "login\\icon\\profintro_04.tga",
		   "login\\icon\\profintro_05.tga",
		   "login\\icon\\profintro_08.tga",
		};
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)(pInfoDlg->GetDlgItem("Img_Intro"));
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, ImagePath[pInfoDlg->GetCheckedRadioButton(1)-1]);
		pImg->SetProperty("Image File", &p);

		
		pDlg = GetDialog("Win_Create_Human");
		PAUICOMBOBOX pCombo;
		if( (pDlg->GetCheckedRadioButton(2)-1) != m_nCurGender )
		{
			m_nCurGender = pDlg->GetCheckedRadioButton(2)-1;
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
			pCombo->ResetContent();
			CSplit s = GetStringFromTable(401 + m_nCurGender);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			DWORD i;
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
			pCombo->ResetContent();
			s = GetStringFromTable(403 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
			pCombo->ResetContent();
			s = GetStringFromTable(410 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int iCurSel = pCombo->GetCurSel();
			if(iCurSel<0)
				iCurSel = 0;
			pCombo->ResetContent();
			s = GetStringFromTable(412 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(iCurSel);
		}
		if( aExistFlags[m_nCurProfession * CECLOGINUIMAN_NUM_GENDERS + m_nCurGender] )
		{
			A3DVECTOR3 vecDirCam;
			if( !m_aModel[m_nCurProfession][m_nCurGender] )
			{
				m_aModel[m_nCurProfession][m_nCurGender] = new CECLoginPlayer(NULL);
				if( !m_aModel[m_nCurProfession][m_nCurGender] )
					return AUI_ReportError(__LINE__, __FILE__);

				bool bval;
				GNET::RoleInfo Info;

				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				Info.faceid = pCombo->GetCurSel();
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				Info.hairid = pCombo->GetCurSel();
				Info.earid = 0;
				Info.tailid = 0;
				Info.gender = m_nCurGender;
				Info.occupation = PROF_1;
				DWORD t1 = GetTickCount();
				bval = m_aModel[m_nCurProfession][m_nCurGender]->Load(Info);
				if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
				A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(GetDialog("Win_CreateName")->GetData()) / 6.0f);
				A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
				m_aModel[m_nCurProfession][m_nCurGender]->Stand();
				A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_CREATE_HUMAN].x, 0, -m_DirCamera[LOGIN_SCENE_CREATE_HUMAN].z);
				vDirRole.Normalize();
				m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vDirRole, g_vAxisY);
				m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_HUMAN]);
			}
			else
			{
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeFace(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeHair(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeDefaultSkin(pCombo->GetCurSel());
			}
			
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int aEquips[SIZE_EQUIPIVTR];
			memcpy(aEquips, m_aModel[m_nCurProfession][m_nCurGender]->GetEquips(), sizeof(aEquips));
			int iRace = pInfoDlg->GetCheckedRadioButton(1) - 1;
			if (pCombo->GetCurSel()==0)
			{
				aEquips[EQUIPIVTR_WEAPON] = 0;
				aEquips[EQUIPIVTR_HEAD] = 0;
				aEquips[EQUIPIVTR_BODY] = 0;
				aEquips[EQUIPIVTR_FOOT] = 0;
			}
			else
			{
				aEquips[EQUIPIVTR_WEAPON] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][3];
				aEquips[EQUIPIVTR_HEAD] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][0];
				aEquips[EQUIPIVTR_BODY] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][1];
				aEquips[EQUIPIVTR_FOOT] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][2];
			}
			m_aModel[m_nCurProfession][m_nCurGender]->ShowEquipments(aEquips, false, true);
			
			A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
			PAUIDIALOG pDlg = GetDialog("Win_CreateName");
			if (pDlg->GetCheckedRadioButton(1)==2)
			{
				pCamera->SetPos(m_CloseUpPosCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()]);
				pCamera->SetDirAndUp(m_CloseUpDirCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()], g_vAxisY);
			}
		}
		if( m_aModel[m_nCurProfession][m_nCurGender] )
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
		}
		else
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(false);
		}
		
		RefreshPlayerList();
	}
	else if( 0==stricmp(szCommand, "change") && (pDlg==GetDialog("Win_Create_Wild")|| pDlg == GetDialog("Win_Create_Prof02")) )
	{
		

		PAUIDIALOG pInfoDlg = GetDialog("Win_Create_Prof02");
		//PAUILABEL pLabIntro = (PAUILABEL)pInfoDlg->GetDlgItem("Txt_Intro");
		//pLabIntro->SetText(GetStringFromTable(1000+pInfoDlg->GetCheckedRadioButton(1)));

		static const char*  ImagePath[]  = 
		{
			"login\\icon\\profintro_06.tga",
			"login\\icon\\profintro_07.tga",
			"login\\icon\\profintro_12.tga",
			"login\\icon\\profintro_10.tga",
			"login\\icon\\profintro_09.tga",
			"login\\icon\\profintro_11.tga",
		};
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)(pInfoDlg->GetDlgItem("Img_Intro"));
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, ImagePath[pInfoDlg->GetCheckedRadioButton(1)-1]);
		pImg->SetProperty("Image File", &p);

		
		pDlg = GetDialog("Win_Create_Wild");
		int iStrFace, iStrHair;
		switch(pInfoDlg->GetCheckedRadioButton(1))
		{
		case 1:
			m_nCurProfession = LOGIN_MODEL_BIG;
			iStrFace = 416;
			iStrHair = 414;
			break;
		case 2:
			m_nCurProfession = LOGIN_MODEL_SMALL;
			iStrFace = 424;
			iStrHair = 422;
			break;
		case 3:
			m_nCurProfession = LOGIN_MODEL_MIDDLE;
			iStrFace = 428;
			iStrHair = 426;
			break;
		case 4:
			m_nCurProfession = LOGIN_MODEL_SMALLNOEAR;
			iStrFace = 420;
			iStrHair = 418;
			break;
		case 5:
			m_nCurProfession = LOGIN_MODEL_BIG2;
			iStrFace = 416;
			iStrHair = 414;
			break;
		case 6:
			m_nCurProfession = LOGIN_MODEL_MIDDLE2;
			iStrFace = 434;
			iStrHair = 432;
			break;
		default:
			m_nCurProfession = LOGIN_MODEL_SMALL;
		}

		PAUICOMBOBOX pCombo;
		if( (pDlg->GetCheckedRadioButton(2)-1) != m_nCurGender )
		{
			m_nCurGender = pDlg->GetCheckedRadioButton(2)-1;
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
			pCombo->ResetContent();
			CSplit s = GetStringFromTable(iStrFace + m_nCurGender);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			DWORD i;
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo->SetData(pInfoDlg->GetCheckedRadioButton(1), "race");
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
			pCombo->ResetContent();
			s = GetStringFromTable(iStrHair + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
			pCombo->ResetContent();
			s = GetStringFromTable(406 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for( i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
			pCombo->ResetContent();
			s = GetStringFromTable(408 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
			pCombo->ResetContent();
			s = GetStringFromTable(430 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int iCurSel = pCombo->GetCurSel();
			if(iCurSel<0)
				iCurSel = 0;
			pCombo->ResetContent();
			s = GetStringFromTable(412 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(iCurSel);
		}
		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		if ((int)pCombo->GetData("race")!=pInfoDlg->GetCheckedRadioButton(1))
		{
			pCombo->ResetContent();
			CSplit s = GetStringFromTable(iStrFace + m_nCurGender);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			DWORD i;
			for( i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo->SetData(pInfoDlg->GetCheckedRadioButton(1), "race");
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
			pCombo->ResetContent();
			s = GetStringFromTable(iStrHair + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
		}

		if (pInfoDlg->GetCheckedRadioButton(1) == 2)
		{
			pDlg->GetDlgItem("Combo_Ear")->Show(true);
			pDlg->GetDlgItem("Combo_Tail")->Show(true);
			pDlg->GetDlgItem("earl")->Show(true);
			pDlg->GetDlgItem("earr")->Show(true);
			pDlg->GetDlgItem("taill")->Show(true);
			pDlg->GetDlgItem("tailr")->Show(true);
		}
		else
		{
			pDlg->GetDlgItem("Combo_Ear")->Show(false);
			pDlg->GetDlgItem("Combo_Tail")->Show(false);
			pDlg->GetDlgItem("earl")->Show(false);
			pDlg->GetDlgItem("earr")->Show(false);
			pDlg->GetDlgItem("taill")->Show(false);
			pDlg->GetDlgItem("tailr")->Show(false);
		}

		if (pInfoDlg->GetCheckedRadioButton(1) == 6)
		{
			pDlg->GetDlgItem("Rdo_Chenhuang1")->Show(true);
			pDlg->GetDlgItem("Rdo_Chenhuang2")->Show(true);
		}
		else
		{
			pDlg->GetDlgItem("Rdo_Chenhuang1")->Show(false);
			pDlg->GetDlgItem("Rdo_Chenhuang2")->Show(false);
		}

		
		
		if( aExistFlags[m_nCurProfession * CECLOGINUIMAN_NUM_GENDERS + m_nCurGender] )
		{
			A3DVECTOR3 vecDirCam;
			if( !m_aModel[m_nCurProfession][m_nCurGender] )
			{
				m_aModel[m_nCurProfession][m_nCurGender] = new CECLoginPlayer(NULL);
				if( !m_aModel[m_nCurProfession][m_nCurGender] )
					return AUI_ReportError(__LINE__, __FILE__);

				bool bval;
				GNET::RoleInfo Info;
				switch(pInfoDlg->GetCheckedRadioButton(1))
				{
				case 1:
					Info.occupation = PROF_34;
					break;
				case 2:
					Info.occupation = PROF_40;
					break;
				case 3:
					Info.occupation = PROF_52;
					break;
				case 4:
					Info.occupation = PROF_46;
					break;
				case 5:
					Info.occupation = PROF_97;
					break;
				case 6:
					Info.occupation = PROF_57;
					break;
				default:
					Info.occupation = PROF_1;
				}
				
				m_aModel[m_nCurProfession][m_nCurGender]->SetProfession(Info.occupation);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				Info.faceid = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				Info.hairid = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
				Info.earid = pCombo->GetCurSel();
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
				Info.tailid = pCombo->GetCurSel();
				Info.gender = m_nCurGender;
				bval = m_aModel[m_nCurProfession][m_nCurGender]->Load(Info);
				if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeDefaultSkin(pCombo->GetCurSel());
				A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(GetDialog("Win_CreateName")->GetData()) / 6.0f);
				A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
				m_aModel[m_nCurProfession][m_nCurGender]->Stand();
				A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_CREATE_WILD].x, 0, -m_DirCamera[LOGIN_SCENE_CREATE_WILD].z);
				vDirRole.Normalize();
				m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vDirRole, g_vAxisY);
				m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_WILD]);
			}
			else
			{
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				int idFace = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeFace(idFace);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				int idHair = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeHair(idHair);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
				if (pCombo->IsShow())
					m_aModel[m_nCurProfession][m_nCurGender]->ChangeEar(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
				if (pCombo->IsShow())
					m_aModel[m_nCurProfession][m_nCurGender]->ChangeTail(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeDefaultSkin(pCombo->GetCurSel());
			}

			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int aEquips[SIZE_EQUIPIVTR];
			memcpy(aEquips, m_aModel[m_nCurProfession][m_nCurGender]->GetEquips(), sizeof(aEquips));
			
			int iRace = pInfoDlg->GetCheckedRadioButton(1)+5;
			if (pCombo->GetCurSel()==0)
			{
				aEquips[EQUIPIVTR_WEAPON] = 0;
				aEquips[EQUIPIVTR_HEAD] = 0;
				aEquips[EQUIPIVTR_BODY] = 0;
				aEquips[EQUIPIVTR_FOOT] = 0;
			}
			else
			{
				aEquips[EQUIPIVTR_WEAPON] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][3];
				aEquips[EQUIPIVTR_HEAD] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][0];
				aEquips[EQUIPIVTR_BODY] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][1];
				aEquips[EQUIPIVTR_FOOT] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][2];
			}
			m_aModel[m_nCurProfession][m_nCurGender]->ShowEquipments(aEquips, false, true);

			A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
			PAUIDIALOG pDlg = GetDialog("Win_CreateName");
			if (pDlg->GetCheckedRadioButton(1)==1)
			{
				pCamera->SetPos(m_PosCamera[LOGIN_SCENE_CREATE_WILD]);
				pCamera->SetDirAndUp(m_DirCamera[LOGIN_SCENE_CREATE_WILD], g_vAxisY);
			}
			else
			{
				pCamera->SetPos(m_CloseUpPosCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()]);
				pCamera->SetDirAndUp(m_CloseUpDirCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()], g_vAxisY);
			}
		}

		if( m_aModel[m_nCurProfession][m_nCurGender] )
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
		}
		else
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(false);
		}
		
		RefreshPlayerList();
	}
	else if( 0 == stricmp(szCommand, "change") && (pDlg == GetDialog("Win_Create_Race03") || pDlg == GetDialog("Win_Create_Prof03")))
	{
		
		
		PAUIDIALOG pInfoDlg = GetDialog("Win_Create_Prof03");
		//PAUILABEL pLabIntro = (PAUILABEL)pInfoDlg->GetDlgItem("Txt_Intro");
		//pLabIntro->SetText(GetStringFromTable(1015+pInfoDlg->GetCheckedRadioButton(1)));

		static const char*  ImagePath[]  = 
		{
			"login\\icon\\profintro_13.tga",
			"login\\icon\\profintro_14.tga",
		};
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)(pInfoDlg->GetDlgItem("Img_Intro"));
		AUIOBJECT_SETPROPERTY p;
		sprintf(p.fn, ImagePath[pInfoDlg->GetCheckedRadioButton(1)-1]);
		pImg->SetProperty("Image File", &p);

		int iStrFace, iStrHair;
		switch(pInfoDlg->GetCheckedRadioButton(1))
		{
		case 1:
			m_nCurProfession = LOGIN_MODEL_NORMAL2;
			iStrFace = 438;
			iStrHair = 436;
			break;
		case 2:
			m_nCurProfession = LOGIN_MODEL_SMALL2;
			iStrFace = 442;
			iStrHair = 440;
			break;
		default:
			m_nCurProfession = LOGIN_MODEL_NORMAL;
		}

		
		pDlg = GetDialog("Win_Create_Race03");
		pDlg->GetDlgItem("Combo_Ear")->Show(false);
		pDlg->GetDlgItem("Combo_Tail")->Show(false);
		pDlg->GetDlgItem("earl")->Show(false);
		pDlg->GetDlgItem("earr")->Show(false);
		pDlg->GetDlgItem("taill")->Show(false);
		pDlg->GetDlgItem("tailr")->Show(false);

		//英招没有女性角色
		if(pInfoDlg->GetCheckedRadioButton(1) == 1)
		{
		   if(pDlg->GetCheckedRadioButton(2) != 1)
              pDlg->CheckRadioButton(2,1);
		}
	
		PAUICOMBOBOX pCombo;
		if( (pDlg->GetCheckedRadioButton(2)-1) != m_nCurGender )
		{
			m_nCurGender = pDlg->GetCheckedRadioButton(2)-1;
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
			pCombo->ResetContent();
			CSplit s = GetStringFromTable(iStrFace + m_nCurGender);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			DWORD i;
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo->SetData(pInfoDlg->GetCheckedRadioButton(1), "race");
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
			pCombo->ResetContent();
			s = GetStringFromTable(iStrHair + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
			pCombo->ResetContent();
			s = GetStringFromTable(444 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int iCurSel = pCombo->GetCurSel();
			if(iCurSel<0)
				iCurSel = 0;
			pCombo->ResetContent();
			s = GetStringFromTable(446 + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(iCurSel);
		}

		pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
		if ((int)pCombo->GetData("race")!=pInfoDlg->GetCheckedRadioButton(1))
		{
			pCombo->ResetContent();
			CSplit s = GetStringFromTable(iStrFace + m_nCurGender);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			DWORD i;
			for( i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
			pCombo->SetData(pInfoDlg->GetCheckedRadioButton(1), "race");
			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
			pCombo->ResetContent();
			s = GetStringFromTable(iStrHair + m_nCurGender);
			vec = s.Split(_AL("\t"));
			for(i = 0; i < vec.size(); i++ )
				pCombo->AddString(vec[i]);
			pCombo->SetCurSel(0);
		}
		if( aExistFlags[m_nCurProfession * CECLOGINUIMAN_NUM_GENDERS + m_nCurGender] )
		{
			A3DVECTOR3 vecDirCam;
			if( !m_aModel[m_nCurProfession][m_nCurGender] )
			{
				m_aModel[m_nCurProfession][m_nCurGender] = new CECLoginPlayer(NULL);
				if( !m_aModel[m_nCurProfession][m_nCurGender] )
					return AUI_ReportError(__LINE__, __FILE__);

				bool bval;
				GNET::RoleInfo Info;
				switch(pInfoDlg->GetCheckedRadioButton(1))
				{
				case 1:
					Info.occupation = PROF_109;
					break;
				case 2:
					Info.occupation = PROF_103;
					break;
				default:
					Info.occupation = PROF_1;
				}

				m_aModel[m_nCurProfession][m_nCurGender]->SetProfession(Info.occupation);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				Info.faceid = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				Info.hairid = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				//pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
				//Info.earid = pCombo->GetCurSel();
				//pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
				//Info.tailid = pCombo->GetCurSel();
				Info.earid = 0;
				Info.tailid = 0;
				Info.gender = m_nCurGender;
				bval = m_aModel[m_nCurProfession][m_nCurGender]->Load(Info);
				if( !bval ) return AUI_ReportError(__LINE__, __FILE__);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeDefaultSkin(pCombo->GetCurSel());
				A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(GetDialog("Win_CreateName")->GetData()) / 6.0f);
				A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
				m_aModel[m_nCurProfession][m_nCurGender]->Stand();

				//牵机
				if(pInfoDlg->GetCheckedRadioButton(1) == 2)
				{
					A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_CREATE_QIANJI].x, 0, -m_DirCamera[LOGIN_SCENE_CREATE_QIANJI].z);
					vDirRole.Normalize();
					m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vDirRole, g_vAxisY);
					m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_QIANJI]);
				}
				//英招
				else
				{
					A3DVECTOR3 vDirRole(-m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].x, 0, -m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO].z);
					vDirRole.Normalize();
					m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vDirRole, g_vAxisY);
					m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_YINGZHAO]);
				}
			}
			else
			{
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Face");
				int idFace = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeFace(idFace);
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Hair");
				int idHair = m_aModel[m_nCurProfession][m_nCurGender]->FaceHairIndex2Id(pCombo->GetCurSel());
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeHair(idHair);
				//pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Ear");
				//if (pCombo->IsShow())
				//	m_aModel[m_nCurProfession][m_nCurGender]->ChangeEar(pCombo->GetCurSel());
				//pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Tail");
				//if (pCombo->IsShow())
				//	m_aModel[m_nCurProfession][m_nCurGender]->ChangeTail(pCombo->GetCurSel());
				pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Cloth");
				m_aModel[m_nCurProfession][m_nCurGender]->ChangeDefaultSkin(pCombo->GetCurSel());
			}

			pCombo = (PAUICOMBOBOX)pDlg->GetDlgItem("Combo_Equip");
			int aEquips[SIZE_EQUIPIVTR];
			memcpy(aEquips, m_aModel[m_nCurProfession][m_nCurGender]->GetEquips(), sizeof(aEquips));

			int iRace = pInfoDlg->GetCheckedRadioButton(1)+11;
			if (pCombo->GetCurSel()==0)
			{
				aEquips[EQUIPIVTR_WEAPON] = 0;
				aEquips[EQUIPIVTR_HEAD] = 0;
				aEquips[EQUIPIVTR_BODY] = 0;
				aEquips[EQUIPIVTR_FOOT] = 0;
			}
			else
			{
				aEquips[EQUIPIVTR_WEAPON] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][3];
				aEquips[EQUIPIVTR_HEAD] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][0];
				aEquips[EQUIPIVTR_BODY] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][1];
				aEquips[EQUIPIVTR_FOOT] = msi_EquipIds[iRace][pCombo->GetCurSel()-1][m_nCurGender][2];
			}
			m_aModel[m_nCurProfession][m_nCurGender]->ShowEquipments(aEquips, false, true);

			A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
			PAUIDIALOG pDlg = GetDialog("Win_CreateName");
			if (pDlg->GetCheckedRadioButton(1)==1)
			{
				if(pInfoDlg->GetCheckedRadioButton(1) == 2)
				{
				    pCamera->SetPos(m_PosCamera[LOGIN_SCENE_CREATE_QIANJI]);
				    pCamera->SetDirAndUp(m_DirCamera[LOGIN_SCENE_CREATE_QIANJI], g_vAxisY);
				}
				else
				{
					pCamera->SetPos(m_PosCamera[LOGIN_SCENE_CREATE_YINGZHAO]);
					pCamera->SetDirAndUp(m_DirCamera[LOGIN_SCENE_CREATE_YINGZHAO], g_vAxisY);
				}
			}
			else
			{
				pCamera->SetPos(m_CloseUpPosCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()]);
				pCamera->SetDirAndUp(m_CloseUpDirCamera[m_aModel[m_nCurProfession][m_nCurGender]->GetGender()][m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique()], g_vAxisY);
			}
		}
		if( m_aModel[m_nCurProfession][m_nCurGender] )
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(true);
		}
		else
		{
			GetDialog("Win_CreateConfirm")->GetDlgItem("Btn_Confirm")->Enable(false);
		}

		RefreshPlayerList();
	}
	else if( 0 == stricmp(szCommand, "Btn_Link") )
	{
		OpenURL("spread", "url");
	}

	return true;
}
bool CECLoginUIMan::OnCommand_PwdProtectResponse(const char* szCommand, AUIDialog* pDlg)
{
   if( 0 == stricmp(szCommand, "confirm"))
   {
       pDlg->Show(false);
   }
   else if(0 == stricmp(szCommand, "link"))
   {
      OpenURL("Login", "official");
   }
   return true;
}
bool CECLoginUIMan::OnCommand_PwdProtect(const char* szCommand, AUIDialog* pDlg)
{
	bool b;
	if( stricmp(pDlg->GetName(), "Win_PwdProtect") == 0 )
		b = true;
	else
		b = false;
	if( 0 == stricmp(szCommand, "clear") )
	{
		pDlg->GetDlgItem("DEFAULT_Input_1")->SetText(_AL(""));
		if( b )
		{
			pDlg->GetDlgItem("Input_2")->SetText(_AL(""));
			pDlg->GetDlgItem("Input_3")->SetText(_AL(""));
		}
		pDlg->ChangeFocus(pDlg->GetDlgItem("DEFAULT_Input_1"));
	}
	else if( 0 == stricmp(szCommand, "confirm") || 0 == stricmp(szCommand, "Input_3"))
	{
		if( !g_pGame->GetGameSession()->IsConnected() )
		{
			pDlg->Show(false);
			ShowCombineDialogs("Login", true);
			MessageBox("", GetStringFromTable(226), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		else
		{
			int ret;
			if( b )
			{
				int i1 = a_atoi(pDlg->GetDlgItem("DEFAULT_Input_1")->GetText());
				int i2 = a_atoi(pDlg->GetDlgItem("Input_2")->GetText());
				int i3 = a_atoi(pDlg->GetDlgItem("Input_3")->GetText());
				ret = i1 * 10000 + i2 * 100 + i3;
			}
			else
			{
				if(stricmp(pDlg->GetName(), "Win_PwdProtect_Phone1") == 0 && a_strlen(pDlg->GetDlgItem("DEFAULT_Input_1")->GetText()) != 6)
				{
					MessageBox("", GetStringFromTable(232), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
					return true;
				}
				ret = a_atoi(pDlg->GetDlgItem("DEFAULT_Input_1")->GetText());
			}

			g_pGame->GetGameSession()->matrix_response(ret);
			pDlg->Show(false);
			GetDialog("Win_Softkb2")->Show(false);
			GetDialog("Win_LoginWait")->Show(true);
		}
	}
	else if( 0 == stricmp(szCommand, "link") )
	{
		if( b )				
			OpenURL("PWDPROTECT", "Link2");
		else
		{
			if(stricmp(pDlg->GetName(), "Win_PwdProtect_Phone1") == 0)
			    OpenURL("PWDPROTECT", "Link4");
			else
			    OpenURL("PWDPROTECT", "Link3");
		}
	}
	else if( 0 == stricmp(szCommand, "IDCANCEL") )
	{
		MessageBox("PwdProtect_Cancel", GetStringFromTable(227), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	}

	return true;
}

//	On message box
bool CECLoginUIMan::OnMessageBox(int iRetVal, AUIDialog* pDlg)
{
	if( 0 == stricmp(pDlg->GetName(), "SelChar_DelChar") && IDYES == iRetVal )
	{
		g_pGame->GetGameSession()->DeleteRole(m_vecRoleInfo[m_idCurRole].roleid);
	}
	else if( 0 == stricmp(pDlg->GetName(), "PwdProtect_Cancel") && IDYES == iRetVal )
	{
		GetDialog("Win_PwdProtect")->Show(false);
		GetDialog("Win_PwdProtect1")->Show(false);
		GetDialog("Win_PwdProtect_Phone1")->Show(false);
		GetDialog("Win_Softkb")->Show(false);
		ShowCombineDialogs("Login");
		g_pGame->GetGameSession()->Close();
	}
	else if( 0 == stricmp(pDlg->GetName(), "Game_No_PopID") && IDYES == iRetVal )
	{
		GetDialog("Win_PopID")->Show(false);
		OnCommand_NewChar("confirm", NULL);
	}
	else if (0 == stricmp(pDlg->GetName(), "Login_SearchServer"))
	{
		GetDialog("Win_LoginServerListSearch")->Show(true);
	}
	else if( GetDialog("Win_Login")->IsShow() && !GetDialog("Win_LoginServerList")->IsShow() )
	{
		GetDialog("Win_Login")->Show(true);		// To focus on username editbox.
	}

	return AUILuaManager::OnMessageBox(iRetVal, pDlg);
}

//	Handle windows message
bool CECLoginUIMan::DealWindowsMessage(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static int iLastMouseX;
	static int iLastMouseY;
	static bool bLDown = false;
	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
	AUIListBox* pMailList = (AUIListBox*)GetDialog("Win_Login")->GetDlgItem("Lst_Mail");
	if( uMsg == WM_MOUSEMOVE )
	{
		m_iMouseX = GET_X_LPARAM(lParam);
		m_iMouseY = GET_Y_LPARAM(lParam);
	}
	else if( WM_KEYDOWN == uMsg)
	{
		if((int)wParam==VK_ESCAPE && m_pIntroMovie && m_pIntroMovie->IsPlaying() )
		{
			m_pIntroMovie->Stop();
			return true;
		}

		//check if is entering user name		
		if(GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->IsFocus() && pMailList->IsShow())
		{
			if((int)wParam == VK_UP || (int)wParam == VK_DOWN)
			{
				pMailList->OnDlgItemMessage(uMsg, wParam, lParam);
				return true;
			}
			else if((int)wParam == VK_RETURN)
			{
				// get selected string
				int nCurSel = pMailList->GetCurSel();
				ACString szSuffix = pMailList->GetText(nCurSel);
				const ACHAR* szName = GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->GetText();

				if(szSuffix.GetLength() > 0)
				{
					const ACHAR* suffixPos = a_strstr(szName, _AL("@"));
					ACHAR szNewName[256];
					memset(szNewName, 0, 256);
					a_strncpy(szNewName, szName, suffixPos - szName);
					a_strcat(szNewName, szSuffix);
					if(suffixPos)
					{
						//a_strcpy(suffixPos, szSuffix);
						GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->SetText(szNewName);
					}
				}

				pMailList->Show(false);
				GetDialog("Win_Login")->ChangeFocus(GetDialog("Win_Login")->GetDlgItem("Txt_PassWord"));

				return true;
			}
		}
	}
	else if(WM_LBUTTONDBLCLK == uMsg)
	{
	    PAUIDIALOG pDlgCreateRole = GetDialog("Win_Create");
		if(pDlgCreateRole && pDlgCreateRole->IsShow())
		{
		   PAUISTILLIMAGEBUTTON  pHumman = (PAUISTILLIMAGEBUTTON)(pDlgCreateRole->GetDlgItem("Btn_Human"));
		   PAUISTILLIMAGEBUTTON  pWild = (PAUISTILLIMAGEBUTTON)(pDlgCreateRole->GetDlgItem("Btn_Wild"));
		   PAUISTILLIMAGEBUTTON  pRace03 = (PAUISTILLIMAGEBUTTON)(pDlgCreateRole->GetDlgItem("Btn_Race03"));
		   m_iMouseX = GET_X_LPARAM(lParam);
		   m_iMouseY = GET_Y_LPARAM(lParam);
		   if(pHumman->IsInHitArea(m_iMouseX, m_iMouseY))
		   {
		        OnCommand_NewChar("confirmhuman",pDlgCreateRole);
		   }
		   else if(pWild->IsInHitArea(m_iMouseX, m_iMouseY))
		   {
		        OnCommand_NewChar("confirmwild",pDlgCreateRole);
		   }
		   else if(pRace03->IsInHitArea(m_iMouseX, m_iMouseY))
		   {
			    OnCommand_NewChar("confirmrace03",pDlgCreateRole);
		   }
		}
	}

	if(pMailList->IsShow())
	{
		if(uMsg == WM_LBUTTONDOWN || uMsg == WM_MOUSEMOVE)
		{
			pMailList->OnDlgItemMessage(uMsg, wParam, lParam);
			return true;
		}
		else if(uMsg == WM_LBUTTONUP && m_pObjMouseOn == pMailList)
		{
			m_iMouseX = GET_X_LPARAM(lParam);
			m_iMouseY = GET_Y_LPARAM(lParam);

			if(pMailList->GetHitArea(m_iMouseX, m_iMouseY) != AUILISTBOX_RECT_TEXT)
			{
				// get selected string
				int nCurSel = pMailList->GetCurSel();
				ACString szSuffix = pMailList->GetText(nCurSel);
				const ACHAR* szName = GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->GetText();

				if(szSuffix.GetLength() > 0)
				{
					const ACHAR* suffixPos = a_strstr(szName, _AL("@"));
					ACHAR szNewName[256];
					memset(szNewName, 0, 256);
					a_strncpy(szNewName, szName, suffixPos - szName);
					a_strcat(szNewName, szSuffix);
					if(suffixPos)
					{
						//a_strcpy(suffixPos, szSuffix);
						GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->SetText(szNewName);
					}
				}

				pMailList->Show(false);
				GetDialog("Win_Login")->ChangeFocus(GetDialog("Win_Login")->GetDlgItem("Txt_PassWord"));

				return true;
			}
		}
	}

	if( AUILuaManager::DealWindowsMessage(uMsg, wParam, lParam) )
		return true;
	else if( uMsg == WM_RBUTTONDOWN && AUI_PRESS('R') )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		AdjustRole(GET_X_LPARAM(lParam) - p->X, GET_Y_LPARAM(lParam) - p->Y);
	}
	else if( uMsg == WM_RBUTTONDOWN )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		m_bDragRole = true;
		m_ptDragStart.x = GET_X_LPARAM(lParam) - p->X;
		m_ptDragStart.y = GET_Y_LPARAM(lParam) - p->Y;
		g_pGame->ShowCursor(false);
	}

	else if( uMsg == WM_MOUSEMOVE )
	{
		m_iMouseX = GET_X_LPARAM(lParam);
		m_iMouseY = GET_Y_LPARAM(lParam);


		if( !(wParam & MK_RBUTTON) )
		{
			m_bDragRole = false;
			g_pGame->ShowCursor(true);
		}
		else
		{
			/*
			A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
			int mx = GET_X_LPARAM(lParam) - p->X;
			int my = GET_Y_LPARAM(lParam) - p->Y;

			A3DMATRIX4 matRotate = RotateY((m_ptDragStart.x - mx) * 0.003f);

			A3DSkinModel * pSkinModel;
			if( !GetDialog("Win_Create")->IsShow() )
				pSkinModel = m_vecRoleModel[m_idCurRole]->GetA3DSkinModel();
			else
				pSkinModel = m_aModel[m_nCurProfession][m_nCurGender]->GetA3DSkinModel();
			A3DVECTOR3 vecDir = pSkinModel->GetAbsoluteTM().GetRow(2);
			A3DVECTOR3 vecUp = pSkinModel->GetAbsoluteTM().GetRow(1);
			vecDir = a3d_VectorMatrix3x3(vecDir, matRotate);
			vecUp = a3d_VectorMatrix3x3(vecUp, matRotate);

			pSkinModel->SetDirAndUp(vecDir, vecUp);
			*/
		}
	}
	else if( uMsg == WM_RBUTTONUP )
	{
		m_bDragRole = false;
		g_pGame->ShowCursor(true);
	}
	else
		return false;

	return true;
}

//	Handle event
bool CECLoginUIMan::OnEvent(UINT uMsg, WPARAM wParam, LPARAM lParam, AUIDialog* pDlg, AUIObject* pObj)
{
	if (AUILuaManager::OnEvent(uMsg, wParam, lParam, pDlg, pObj))
		return true;

	if( 0 == stricmp(pDlg->GetName(), "Win_CharTaskTip") )
	{
		if( uMsg == WM_KEYDOWN && wParam == VK_RETURN )
		{
			OnCommand_SelChar("tasktipok", pDlg);
		}
		return true;
	}
	else if (0 == stricmp(pDlg->GetName(), "Win_CharList_Cover") && uMsg == WM_LBUTTONDBLCLK)
	{
		OnCommand_NewChar("Btn_Human", GetDialog("Win_Create"));
	}
	else if (0 == stricmp(pDlg->GetName(), "Win_CharList_Cover2") && uMsg == WM_LBUTTONDBLCLK)
	{
		OnCommand_NewChar("Btn_Wild", GetDialog("Win_Create"));
	}
	else if (0 == stricmp(pDlg->GetName(), "Win_Char_SelectLineList") && 
		pObj==pDlg->GetDlgItem("Lst_Line") )
	{
		if(	(uMsg == WM_KEYDOWN && wParam == VK_RETURN) && GetDialog("Win_CharList")->IsShow())
		{
			OnCommand_SelChar("enterworld", GetDialog("Win_CharEnterWorld"));
		}
		if ( uMsg == WM_LBUTTONUP )
		{
			A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
			POINT ptPos = pObj->GetPos();
			int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
			int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
			PAUILISTBOX pLst = (PAUILISTBOX)pObj;
			if(pLst->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT)
			{
				OnCommand_SelChar("selectline", GetDialog("Win_Char_SelectLineList"));
			}
		}
		if ( uMsg==WM_LBUTTONDBLCLK )
		{
			OnCommand_SelChar("poplinelist", GetDialog("Win_Char_SelectLine"));
		}
	}
	else if( pDlg==GetDialog("Win_Char_SelectLine") && uMsg == WM_LBUTTONUP )
	{
		OnCommand_SelChar("poplinelist", GetDialog("Win_Char_SelectLine"));		
	}
	else if( GetDialog("Win_CharList")->IsShow() ) 
	{
		char szText[20];
		if( uMsg == WM_KEYDOWN && wParam == VK_RETURN ||
			uMsg == WM_LBUTTONDBLCLK && 
			pObj && strstr(pObj->GetName(), "Rdo_Char") &&
			GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->IsEnabled()) 
		{
			OnCommand_SelChar("enterworld", GetDialog("Win_CharEnterWorld"));
		}
		if( m_vecRoleInfo.size() > 0 )
		{
			if( uMsg == WM_KEYDOWN && wParam == VK_UP )
			{
				int nSel = GetDialog("Win_CharList")->GetCheckedRadioButton(1);
				if( nSel > 1 )
				{
					GetDialog("Win_CharList")->CheckRadioButton(1, nSel - 1);
					sprintf(szText, "Rdo_Char%d", nSel - 1);
				}
				else
				{
					GetDialog("Win_CharList")->CheckRadioButton(1, m_vecRoleInfo.size());
					sprintf(szText, "Rdo_Char%d", m_vecRoleInfo.size());
				}
				GetDialog("Win_CharList")->OnCommand(szText);
			}
			if( uMsg == WM_KEYDOWN && wParam == VK_DOWN )
			{
				int nSel = GetDialog("Win_CharList")->GetCheckedRadioButton(1);
				if( nSel < (int)m_vecRoleInfo.size() )
				{
					GetDialog("Win_CharList")->CheckRadioButton(1, nSel + 1);
					sprintf(szText, "Rdo_Char%d", nSel + 1);
				}
				else
				{
					GetDialog("Win_CharList")->CheckRadioButton(1, 1);
					sprintf(szText, "Rdo_Char%d", 1);
				}
				GetDialog("Win_CharList")->OnCommand(szText);
			}
		}

		return true;
	}


	return true;
}

bool CECLoginUIMan::LaunchCharacter(bool bLogout)
{
	ShowCombineDialogs("Login", false);
	GetDialog("Win_LoginWait")->Show(false);

	GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(false);
	GetDialog("Win_CharList")->GetDlgItem("Btn_Create")->Enable(false);
	GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(false);
	GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(false);

	ACHAR szLineName[200];
	GetPrivateProfileString(_AL("Server"), _AL("CurrentLine"), _AL(""), 
		szLineName, 200, _AL("userdata\\currentserver.ini"));
	CECGameRun::LineInfoVec lineinfo = g_pGame->GetGameRun()->GetLinesInfo();
	PAUIDIALOG pDlg = GetDialog("Win_Char_SelectLineList");
	PAUILISTBOX pList = (PAUILISTBOX)pDlg->GetDlgItem("Lst_Line");
	pList->ResetContent();

	int i;
	for(i = 0; i < (int)lineinfo.size(); i++ )
	{
//		int n = rand() % (pList->GetCount() + 1);
		ACString strLoad;
		A3DCOLOR color;
		if( lineinfo[i].attribute < 50 )
		{
			strLoad = GetStringFromTable(222);
			color = 0xFF00FF7F;
		}
		else if(  lineinfo[i].attribute < 90 )
		{
			strLoad = GetStringFromTable(223);
			color = 0xFFFFFF7F;
		}
		else
		{
			strLoad = GetStringFromTable(224);
			color = 0xFFFF0000;
		}
			
		pList->InsertString(i, lineinfo[i].name + _AL("\t") + strLoad);
		pList->SetItemTextColor(i, color, 1);
		pList->SetItemData(i, lineinfo[i].id);
	}
	for(i = 0; i < pList->GetCount(); i++ )
	{
		ACString strLine = pList->GetText(i);
		int n = strLine.Find('\t');
		if( n != -1 )
			strLine = strLine.Left(n);
		if( a_stricmp(strLine, szLineName) == 0 )
		{
			pList->BringItemToTop(i);
			pList->SetItemTextColor(0, 0xFFFFCB4A, 0);
			break;
		}
	}
	pList->SetCurSel(0);
//	pDlg->ChangeFocus(pDlg->GetDlgItem("Lst_Line"));
	pDlg->OnCommand("selectline");
	if( bLogout )
	{
		RefreshPlayerList();
		ShowCombineDialogs("CharList");
		PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Char_SelectLine")->GetDlgItem("Chk_Expand");
		pChk->Check(true);
		OnCommand_SelChar("changeline", GetDialog("Win_Char_SelectLine"));
	}
	else
	{
		pDlg->Show(false);
		pDlg = GetDialog("Win_CharTaskTip");
		pDlg->Show(true);
		ChangeScene(LOGIN_SCENE_SELCHAR);
		
		ACString strText;
		int index1;
		AUICTranslate trans;
		if( m_vecTaskInfo.size() > 0 )
		{
			index1 = rand()%m_vecTaskInfo.size();
			strText.Format(_AL("%d:%02d-%d:%02d"), m_vecTaskInfo[index1].tmStartTime.tm_hour, m_vecTaskInfo[index1].tmStartTime.tm_min,
				m_vecTaskInfo[index1].tmEncTime.tm_hour, m_vecTaskInfo[index1].tmEncTime.tm_min);
			pDlg->GetDlgItem("Txt_Time")->SetText(m_vecTaskInfo[index1].strTime);
			pDlg->GetDlgItem("Txt_EventTitle")->SetText(m_vecTaskInfo[index1].strName);
			pDlg->GetDlgItem("Txt_Type")->SetText(m_vecTaskInfo[index1].strType);
			pDlg->GetDlgItem("Txt_NPC")->SetText(m_vecTaskInfo[index1].strNPC);
			pDlg->GetDlgItem("Txt_Description")->SetText(trans.Translate(m_vecTaskInfo[index1].strDescription));
		}
		else if( m_vecTaskInfoAllDay.size() > 0 )
		{
			index1 = rand()%m_vecTaskInfoAllDay.size();
			if (m_vecTaskInfoAllDay[index1].bNoCountLimit)
			{
				strText = GetStringFromTable(502);
			}
			else
				strText.Format(GetStringFromTable(503), m_vecTaskInfoAllDay[index1].iNumMax);
			pDlg->GetDlgItem("Txt_Time")->SetText(strText);
			pDlg->GetDlgItem("Txt_EventTitle")->SetText(m_vecTaskInfoAllDay[index1].strName);
			pDlg->GetDlgItem("Txt_Type")->SetText(m_vecTaskInfoAllDay[index1].strType);
			pDlg->GetDlgItem("Txt_NPC")->SetText(m_vecTaskInfoAllDay[index1].strNPC);
			pDlg->GetDlgItem("Txt_Description")->SetText(trans.Translate(m_vecTaskInfoAllDay[index1].strDescription));
		}
		else
		{
			pDlg->GetDlgItem("Txt_Time")->SetText(_AL(""));
			pDlg->GetDlgItem("Txt_EventTitle")->SetText(_AL(""));
			pDlg->GetDlgItem("Txt_Type")->SetText(_AL(""));
			pDlg->GetDlgItem("Txt_NPC")->SetText(_AL(""));
			pDlg->GetDlgItem("Txt_Description")->SetText(_AL(""));
		}
		//显示更新日志
		AFile file;
		char szFile[MAX_PATH];
		sprintf(szFile, "%s\\UpdateLog.txt", af_GetBaseDir());
		if( file.Open(szFile, AFILE_TEXT|AFILE_OPENEXIST|AFILE_TEMPMEMORY))
		{
			PAUIDIALOG pDlgLog = GetDialog("Win_CharUpdateLog");
			pDlgLog->Show(true);
			pDlgLog->AlignTo(pDlg,  AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
									AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP);
			pDlg->Show(true);
			PAUITEXTAREA pTxt = (PAUITEXTAREA)pDlgLog->GetDlgItem("Txt_Content");
			pTxt->SetText(_AL(""));
			char line[AFILE_LINEMAXLEN];
			DWORD  read_len;
			
			while (file.ReadLine(line, AFILE_LINEMAXLEN, &read_len))
			{
				pTxt->AppendText(AS2WC(line));
				pTxt->AppendText(_AL("\r"));
			}
			file.Close();
		}
		
	}

	return true;
}

bool CECLoginUIMan::CreateCharacter(GNET::RoleInfo &ri)
{
	bool bval;
	char szCommand[40];
	ShowCombineDialogs("CreatePrev", false);
	ShowCombineDialogs("CreateMain", false);
	ShowCombineDialogs("CharList");
	
	bval = AddCharacter(&ri);
	if( !bval ) return AUI_ReportError(__LINE__, __FILE__);

	PAUIDIALOG pDlg = GetDialog("Win_CharList");
	if( m_vecRoleInfo.size() >= CECLOGINUIMAN_MAX_ROLES )
		pDlg->GetDlgItem("Btn_Create")->Enable(false);
	else
		pDlg->GetDlgItem("Btn_Create")->Enable(true);

	RebuildRoleList();
	pDlg = GetDialog("Win_CharList");
	sprintf(szCommand, "Rdo_Char%d", m_vecRoleInfo.size());
	OnCommand_SelChar(szCommand, pDlg);
	RefreshPlayerList();

	return true;
}

bool CECLoginUIMan::AddCharacter(GNET::RoleInfo* pri)
{
	if( pri )
	{
		if (m_vecRoleInfo.size() < CECLOGINUIMAN_MAX_ROLES)
		{
			m_vecRoleInfo.push_back(*pri);
			m_vecRoleModel.push_back(NULL);
		}
	}
	else
	{
		char szCommand[40] = "";
		int i, idRole = -1, nLastLoginTime = 0;
		PAUIDIALOG pDlg = GetDialog("Win_CharList");

		if( m_vecRoleInfo.size() >= CECLOGINUIMAN_MAX_ROLES )
			GetDialog("Win_CharList")->GetDlgItem("Btn_Create")->Enable(false);
		else
			GetDialog("Win_CharList")->GetDlgItem("Btn_Create")->Enable(true);

		RebuildRoleList();

		for( i = 0; i < (int)m_vecRoleInfo.size(); i++ )
		{
			if( m_vecRoleInfo[i].lastlogin_time > nLastLoginTime )
			{
				nLastLoginTime = m_vecRoleInfo[i].lastlogin_time;
				idRole = i;
			}
		}
		if( idRole >= 0 && idRole<8)
			sprintf(szCommand, "Rdo_Char%d", idRole + 1);
		else if( m_vecRoleInfo.size() > 0 )
			strcpy(szCommand, "Rdo_Char1");

		if( strlen(szCommand) > 0 )
			OnCommand_SelChar(szCommand, pDlg);
	}
	
	return true;
}

bool CECLoginUIMan::DelCharacter(int nResult, int idRole)
{
	if( nResult == ERR_SUCCESS )
	{
		for( int i = 0; i < (int)m_vecRoleInfo.size(); i++ )
		{
			if( idRole == m_vecRoleInfo[i].roleid )
			{
				GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(false);
				GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(true);
				m_vecRoleInfo[i].status = _ROLE_STATUS_READYDEL;
				m_vecRoleInfo[i].delete_time = time(NULL);
				m_vecRoleModel[i]->SitDown();
				
				break;
			}
		}
		MSGBOX(209);
		RebuildRoleList();
		RefreshPlayerList();
	}
	else
	{
		MSGBOX(208);
	}

	return true;
}

bool CECLoginUIMan::RestoreCharacter(int nResult, int idRole)
{
	if( nResult == ERR_SUCCESS )
	{
		for( int i = 0; i < (int)m_vecRoleInfo.size(); i++ )
		{
			if( idRole != m_vecRoleInfo[i].roleid ) continue;

			GetDialog("Win_CharEnterWorld")->GetDlgItem("Btn_EnterWorld")->Enable(true);
//			if( !m_vecRoleModel[i]->HasEffectType(CECPlayer::EFF_FACEPILL) &&
//				time(NULL) - m_vecRoleInfo[i].create_time < 2 * 24 * 3600 )
//			{
//				GetDialog("Win_Select")->GetDlgItem("Btn_Modify")->Enable(true);
//			}
//			else
//				GetDialog("Win_Select")->GetDlgItem("Btn_Modify")->Enable(false);
			GetDialog("Win_CharList")->GetDlgItem("Btn_Delete")->Enable(true);
			GetDialog("Win_CharList")->GetDlgItem("Btn_Restore")->Enable(false);
			m_vecRoleInfo[i].status = _ROLE_STATUS_NORMAL;
			m_vecRoleModel[i]->StandUp();
			
			break;
		}
		MSGBOX(212);
		RebuildRoleList();
		RefreshPlayerList();
	}
	else
		MSGBOX(211);

	return true;
}

bool CECLoginUIMan::ChangeCustomizeData(int idRole)
{
	return true;
}

bool CECLoginUIMan::LaunchPreface()
{
	FILE *file;
	bool bShow = false;
	char szFile[MAX_PATH];

	sprintf(szFile, "%s\\Info\\Agreement.txt", af_GetBaseDir());
	file = fopen(szFile, "rb");
	if( file )
	{
		int nLen;
		BYTE szHeader[2] = { 0, 0 };

		fread(szHeader, sizeof(BYTE), 2, file);
		if( szHeader[0] == 0xFF && szHeader[1] == 0xFE )
		{
			fseek(file, 0, SEEK_END);
			nLen = ftell(file) - 2;
			if( nLen > 0 )
			{
				ACHAR *pszText = new ACHAR[nLen / 2 + 1];

				fseek(file, 2, SEEK_SET);
				fread(pszText, sizeof(ACHAR), nLen / 2, file);
				pszText[nLen / 2] = 0;
				GetDialog("Win_Agreement")->GetDlgItem(
					"Txt_Agreement")->SetText(pszText);

				delete [] pszText;
			}
		}
		fclose(file);
		ShowCombineDialogs("Login", false);
		GetDialog("Win_Agreement")->Show(true);
	}
	else
	{
		ShowCombineDialogs("Login");
		LoadUserServer();
	}
	
	if( m_pIntroMovie )
	{
		m_pIntroMovie->SetAutoScaleFlags(AMVID_FIT_X | AMVID_FIT_Y | AMVID_FIT_KEEPCONSTRAINT);
		//m_pIntroMovie->SetVidPos(A3DPOINT2(0, 0));	
		m_pIntroMovie->Start(false);
	}

	return true;
}

extern bool begin_save_game_record();
bool CECLoginUIMan::LaunchLoading()
{
	OutputDebugStringA("Start game 1...\n");

	GNET::RoleInfo ri = m_vecRoleInfo[m_idCurRole];
	A3DVECTOR3 vecPos(ri.posx, ri.posy, ri.posz);

	if (!g_pGame->GetGameRun()->StartGame(ri.worldtag, vecPos))
	{
		//	Fatal error. Exit game directly
		a_LogOutput(1, "CECLoginUIMan::LaunchLoading, Failed to start game !");
		::PostMessage(g_pGame->GetGameInit().hWnd, WM_QUIT, 0, 0);
		return false;
	}

	g_pGame->GetGameSession()->StartGame();
	OutputDebugStringA("Start game 2...\n");
	return true;
}

bool CECLoginUIMan::LaunchMatrixChallenge(void* pData)
{
	MatrixChallenge* p = (MatrixChallenge*)pData;
	PAUIDIALOG pDlg = GetDialog("Win_PwdProtect");
	PAUIDIALOG pDlg1 = GetDialog("Win_PwdProtect1");
	PAUIDIALOG pDlgsk = GetDialog("Win_Softkb2");
	PAUIDIALOG pDlgPhone = GetDialog("Win_PwdProtect_Phone1");
	enum
	{
		ALGORITHM_NONE      =  0x0,				// 仅密码
		ALGORITHM_CARD      =  0x00010000,		// 密保卡
		ALGORITHM_HANDSET   =  0x00020000,		// 手机密保
		ALGORITHM_USBKEY    =  0x00030000,		// 一代神盾
		ALGORITHM_PHONE     =  0x00040000,		// 电话密保
		ALGORITHM_USBKEY2   =  0x00050000,		// 二代神盾
		ALGORITHM_PHONETOKEN=  0x00060000,		// 手机令牌
		ALGORITHM_MASK_HIGH =  0x000F0000,
	};
	if (pDlgPhone && (p->algorithm&ALGORITHM_MASK_HIGH)==ALGORITHM_PHONETOKEN)
	{
		pDlgPhone->GetDlgItem("DEFAULT_Input_1")->SetText(_AL(""));
		pDlgPhone->Show(true);
		pDlgPhone->ChangeFocus(pDlgPhone->GetDlgItem("DEFAULT_Input_1"));
		GetDialog("Win_LoginWait")->Show(false);
	}
	else if( pDlg && (p->algorithm&ALGORITHM_MASK_HIGH)==ALGORITHM_CARD )
	{
		ACString strText;
		int pos = p->nonce;
		unsigned int idx0, idx1, idx2;
		idx0 = pos % 80;
		pos /= 80; 
		idx1 = pos % 80;
		pos /= 80; 
		idx2 = pos % 80;
		strText.Format(_AL("%c%d"), (idx0 >> 3) + 'A', (idx0 & 7) + 1);
		pDlg->GetDlgItem("txt_1")->SetText(strText);
		strText.Format(_AL("%c%d"), (idx1 >> 3) + 'A', (idx1 & 7) + 1);
		pDlg->GetDlgItem("txt_2")->SetText(strText);
		strText.Format(_AL("%c%d"), (idx2 >> 3) + 'A', (idx2 & 7) + 1);
		pDlg->GetDlgItem("txt_3")->SetText(strText);
		pDlg->GetDlgItem("DEFAULT_Input_1")->SetText(_AL(""));
		pDlg->GetDlgItem("Input_2")->SetText(_AL(""));
		pDlg->GetDlgItem("Input_3")->SetText(_AL(""));
		((PAUIEDITBOX)pDlg->GetDlgItem("DEFAULT_Input_1"))->SetIsNumberOnly(true);
		((PAUIEDITBOX)pDlg->GetDlgItem("Input_2"))->SetIsNumberOnly(true);
		((PAUIEDITBOX)pDlg->GetDlgItem("Input_3"))->SetIsNumberOnly(true);
		pDlgsk->Show(true);
		pDlg->Show(true);
		pDlgsk->AlignTo(pDlg,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
		m_fskStartPos = 0;
		pDlg->ChangeFocus(pDlg->GetDlgItem("DEFAULT_Input_1"));
		GetDialog("Win_LoginWait")->Show(false);
	}
	else if( pDlg1 && (p->algorithm&ALGORITHM_MASK_HIGH)==ALGORITHM_HANDSET )
	{
		ACString strText;
		strText.Format(_AL("%d"), p->nonce);
		pDlg1->GetDlgItem("txt_1")->SetText(strText);
		pDlg1->GetDlgItem("DEFAULT_Input_1")->SetText(_AL(""));
		pDlg1->GetDlgItem("txt_1")->SetAlign(AUIFRAME_ALIGN_CENTER);
		((PAUIEDITBOX)pDlg1->GetDlgItem("DEFAULT_Input_1"))->SetIsNumberOnly(true);
		pDlg1->Show(true);
		pDlgsk->Show(true);
		pDlgsk->AlignTo(pDlg1,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
		m_fskStartPos = 0;
		pDlg1->ChangeFocus(pDlg1->GetDlgItem("DEFAULT_Input_1"));
		GetDialog("Win_LoginWait")->Show(false);
	}
	return true;
}

bool CECLoginUIMan::Tick(DWORD dwDeltaTime)
{
	static const float startDegree[] = { 0, 36, 72, 108, 144, 180, 216, 252, 288, 324,};
	static DWORD dwTime = GetTickCount();
	DWORD dwTimeNow = GetTickCount();
	if( m_pIntroMovie && m_pIntroMovie->IsPlaying() )
	{
		m_pIntroMovie->Tick();
		return true;
	}

	if(m_pIntroMovie && m_pIntroMovie->IsPlaying())
	{
		g_pGame->GetBackMusic()->PauseMusic(true);
	}
	else
	{
		g_pGame->GetBackMusic()->PauseMusic(false);
	}

	PAUIDIALOG pDlgPwd = GetDialog("Win_PwdProtect");
	PAUIDIALOG pDlgPwd1 = GetDialog("Win_PwdProtect1");
	PAUIDIALOG pDlgL = GetDialog("Win_PwdProtect");

	DlgLoginServerSearch *pServerListSearch = (DlgLoginServerSearch*)GetDialog("Win_LoginServerListSearch");
	if( pServerListSearch->IsShow() && pServerListSearch->OnItem() || 
		m_pObjMouseOn && (m_pObjMouseOn == pDlgPwd->GetDlgItem("Btn_Link") || 
		m_pObjMouseOn == pDlgPwd1->GetDlgItem("Btn_Link") )
		)
		g_pGame->ChangeCursor(RES_CUR_HAND);
	else
		g_pGame->ChangeCursor(RES_CUR_NORMAL);

	if( pDlgPwd && pDlgPwd->IsShow() )
	{
		pDlgPwd->GetDlgItem("Btn_Decide")->Enable(
			a_strlen(pDlgPwd->GetDlgItem("DEFAULT_Input_1")->GetText()) > 0 &&
			a_strlen(pDlgPwd->GetDlgItem("Input_2")->GetText()) > 0 &&
			a_strlen(pDlgPwd->GetDlgItem("Input_3")->GetText()) > 0);
				PAUIDIALOG pDlgMouseOn;
		PAUIOBJECT pObjMouseOn;
		PAUIDIALOG pDlgSoftkb2 = GetDialog("Win_Softkb2");
		GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
		if (pObjMouseOn)
		{
			for (int i=0; i<10; i++)
			{
				char szName[20];
				AUIOBJECT_SETPROPERTY p;
				sprintf(szName, "Char_%d", i);
				sprintf(p.fn, "BUTTON\\PwprotectHover.tga");
				pDlgSoftkb2->GetDlgItem(szName)->SetProperty("Frame Image", &p);
				pDlgSoftkb2->GetDlgItem(szName)->SetText(_AL(""));
			}
		}
		else
		{
			for (int i=0; i<10; i++)
			{
				char szName[20];
				ACHAR szText[20];
				AUIOBJECT_SETPROPERTY p;
				sprintf(szName, "Char_%d", i);
				a_sprintf(szText, _AL("%d"), i);
				sprintf(p.fn, "BUTTON\\Pwprotect.tga");
				pDlgSoftkb2->GetDlgItem(szName)->SetProperty("Frame Image", &p);
				pDlgSoftkb2->GetDlgItem(szName)->SetText(szText);
			}
		}
		int x, y, dist;
		int iWid = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth;
		int iHei = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight;
		float fAspect = (float)iHei / iWid;
		
		float fScaleH = iWid / 1024.0f;
		float fScaleV = iHei / 768.0f;
		if (fScaleH<fScaleV)
		{
			fScaleV = fScaleH;
		}
		else
		{
			fScaleH = fScaleV;
		}

		for (int i=0; i<10; i++)
		{
			char szName[20];
			float deg = m_fskStartPos+startDegree[i];
			x = (int)(fScaleH*(130+80*g_pA3DMathUtility->SIN(deg)-15));
			y = (int)(fScaleV*(130-80*g_pA3DMathUtility->COS(deg)-15));
			sprintf(szName, "Char_%d", i);
			pDlgSoftkb2->GetDlgItem(szName)->SetPos(x, y);
			sprintf(szName, "Img_%d", i);
			pDlgSoftkb2->GetDlgItem(szName)->SetPos(x, y);
		}
		if (0==m_iMouseX && 0==m_iMouseY)
		{
			x=0;
			y=0;
		}
		else
		{
			x = (int)(m_iMouseX - pDlgSoftkb2->GetPos().x-125*fScaleH);
			y = (int)(m_iMouseY - pDlgSoftkb2->GetPos().y-125*fScaleV);
		}
		if (m_iMouseX>354 || m_iMouseY>453)
		{
			int asd = 3;
			int bb= asd;
		}
		dist = (int)sqrtf(float(x*x+y*y));
		dist -= (int)(90*sqrtf((fScaleH*fScaleH + fScaleV*fScaleV)/2));
		if (dist<0)
		{
			dist = 0;
		}
		m_fskStartPos += (float)sqrtf((float)dist)/5;
		if (m_fskStartPos>=360)
		{
			m_fskStartPos -= 360;
		}
	}
	else if( pDlgPwd1 && pDlgPwd1->IsShow() )
	{
		pDlgPwd1->GetDlgItem("Btn_Decide")->Enable(
			a_strlen(pDlgPwd1->GetDlgItem("DEFAULT_Input_1")->GetText()) > 0);
	}
	else
		GetDialog("Win_Softkb2")->Show(false);

	if (GetDialog("Win_PwdProtect")->IsShow())
	{
		if (pDlgL->GetDlgItem("DEFAULT_Input_1")->IsFocus())
		{
			m_pwdCurObj = pDlgL->GetDlgItem("DEFAULT_Input_1");
		}
		else if (pDlgL->GetDlgItem("Input_2")->IsFocus())
		{
			m_pwdCurObj = pDlgL->GetDlgItem("Input_2");
		}
		else if (pDlgL->GetDlgItem("Input_3")->IsFocus())
		{
			m_pwdCurObj = pDlgL->GetDlgItem("Input_3");
		}
	}

	if( GetDialog("Win_Agreement")->IsShow() )
	{
		PAUIDIALOG pDlg = GetDialog("Win_Agreement");
		PAUITEXTAREA pText = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_Agreement");
		if( pText->GetFirstLine() + pText->GetLinesPP() == pText->GetLines() )
			pDlg->GetDlgItem("Btn_Agree")->Enable(true);
		else
			pDlg->GetDlgItem("Btn_Agree")->Enable(false);
	}
	
	if( m_bDragRole )
	{	// Restore cursor position to start position
		POINT pt = {m_ptDragStart.x, m_ptDragStart.y};
		::ClientToScreen(g_pGame->GetGameInit().hWnd, &pt);
		::SetCursorPos(pt.x, pt.y);
	}

	if( GetDialog("Win_CharList")->IsShow() )
	{
 		char szName[40];
		ACString strLevel;
		PAUIOBJECT pRadio;
		PAUIDIALOG pDlg = GetDialog("Win_CharList");
		int i, nDay, nHour, nMin, nSec, nTime = time(NULL);
		
		for( i = 0; i < CECLOGINUIMAN_MAX_ROLES; i++ )
		{
			if( i >= (int)m_vecRoleInfo.size() )
				continue;

			sprintf(szName, "Rdo_Char%d", i + 1);
			pRadio = pDlg->GetDlgItem(szName);

			if( m_vecRoleInfo[i].status==_ROLE_STATUS_NORMAL || m_vecRoleInfo[i].status==_ROLE_STATUS_SELLING )
			{
				pRadio->SetHint(_AL(""));
			}
			else if (m_vecRoleInfo[i].status == _ROLE_STATUS_FROZEN)
			{
				pRadio->SetHint(GetStringFromTable(230));
			}
			else
			{
				nSec = max(7 * 24 * 3600 - (nTime - m_vecRoleInfo[i].delete_time), 0);
//				nSec = max(60 - (nTime - m_vecRoleInfo[i].delete_time), 0);
				if( nSec >= 60 * 60 * 24 )
				{
					nDay = nSec / (60 * 60 * 24);
					strLevel.Format(GetStringFromTable(213), nDay);
				}
				else
				{
					nMin = nSec / 60;
					nSec -= nMin * 60;
					nHour = nMin / 60;
					nMin -= nHour * 60;
					strLevel.Format(GetStringFromTable(214), nHour, nMin, nSec);
				}
				pRadio->SetHint(strLevel);
			}
		}

		if( !g_pGame->GetGameSession()->IsConnected() )
			GetDialog("Win_CharList")->GetDlgItem("Btn_Create")->Enable(false);

		if (m_cPwWarn>0 && !GetDialog("Win_PwdProtectHint")->IsShow())
		{
			PAUIOBJECT pObjPw = GetDialog("Win_PwdProtectHint")->GetDlgItem("Lab_Password");
			PAUIOBJECT pObjPc = GetDialog("Win_PwdProtectHint")->GetDlgItem("Lab_PasswordCard");
			pObjPw->Show((m_cPwWarn&1)>0);
			pObjPc->Show((m_cPwWarn&2)>0);
			int yUp = pObjPw->GetPos(true).y < pObjPc->GetPos(true).y ? pObjPw->GetPos(true).y : pObjPc->GetPos(true).y;
			int yDown = pObjPw->GetPos(true).y > pObjPc->GetPos(true).y ? pObjPw->GetPos(true).y : pObjPc->GetPos(true).y;
			if ((m_cPwWarn&1)==0)
			{
				pObjPw->SetPos(pObjPw->GetPos(true).x, yUp);
				pObjPc->SetPos(pObjPc->GetPos(true).x, yDown);
			}
			else if ((m_cPwWarn&2)==0)
			{
				pObjPw->SetPos(pObjPw->GetPos(true).x, yDown);
				pObjPc->SetPos(pObjPc->GetPos(true).x, yUp);
			}
			GetDialog("Win_PwdProtectHint")->Show(true);
		}
		else if (m_cPwWarn==0 && GetDialog("Win_PwdProtectHint")->IsShow())
		{
			GetDialog("Win_PwdProtectHint")->Show(false);
		}
		if (pDlg->GetFocus()&&strstr(pDlg->GetFocus()->GetName(), "Rdo_Char"))
		{
			pDlg->ChangeFocus(NULL);
		}
		// 角色旋转
		if (m_idCurRole>=0 && (int)m_vecRoleModel.size()>m_idCurRole)
		{
			pDlg = GetDialog("Win_Char_Rotate");
			if( ((PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem("Btn_Left"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			{
				int nDeg = pDlg->GetData() + g_pGame->GetTickTime();
				while( nDeg >= 2160 ) nDeg -= 2160;
				pDlg->SetData(nDeg);
				A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(pDlg->GetData()) / 6.0f);
				A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
//				m_vecRoleModel[m_idCurRole]->Stand();
				m_vecRoleModel[m_idCurRole]->SetDirAndUp(vecDir, g_vAxisY);
			}
			else if( ((PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem("Btn_Right"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
			{
				int nDeg = pDlg->GetData() - g_pGame->GetTickTime();
				while( nDeg < 0 ) nDeg += 2160;
				pDlg->SetData(nDeg);
				A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(pDlg->GetData()) / 6.0f);
				A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
//				m_vecRoleModel[m_idCurRole]->Stand();
				m_vecRoleModel[m_idCurRole]->SetDirAndUp(vecDir, g_vAxisY);
			}
		}
	}
	else if( GetDialog("Win_Login")->IsShow() )
	{
		PAUIDIALOG pDlgL = GetDialog("Win_Login");
		PAUIDIALOG pDlgS = GetDialog("Win_Softkb");

		if( pDlgL->IsActive() && pDlgL->GetDlgItem("Txt_PassWord")->IsFocus() )
		{
			if( !pDlgS->IsShow() )
			{
				pDlgS->Show(true);
				pDlgS->AlignTo(pDlgL, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM, 0, 20);
				BringWindowToTop(pDlgL);
			}
		}
		else if( pDlgS->IsShow() && !pDlgS->IsActive() )
			pDlgS->Show(false);
	}
	else if( GetDialog("Win_LoginWait")->IsShow() )
	{
		ACString strText;
		DWORD dwTotalTime, dwCnt;
		PAUIDIALOG pDlg = GetDialog("Win_LoginWait");
		PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Txt_Message");

		if( g_pGame->GetGameSession()->GetOvertimeCnt(dwTotalTime, dwCnt) )
		{
			dwTotalTime /= 1000;
			dwCnt /= 1000;
			strText.Format(GetStringFromTable(217), dwTotalTime, dwCnt);
		}
		else
			strText = GetStringFromTable(216);

		pLabel->SetText(strText);
		pLabel->SetAlign(AUIFRAME_ALIGN_CENTER);
	}
	else if( GetDialog("Win_Create")->IsShow())
	{
		BringWindowToTop(GetDialog("Win_Create"));
	}
	else if( GetDialog("Win_Create_Human")->IsShow() || GetDialog("Win_Create_Wild")->IsShow() || GetDialog("Win_Create_Race03")->IsShow())
	{
		PAUIDIALOG pDlg = GetDialog("Win_CreateName");
		if( ((PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem("Btn_Left"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		{
			int nDeg = pDlg->GetData() + g_pGame->GetTickTime();
			while( nDeg >= 2160 ) nDeg -= 2160;
			pDlg->SetData(nDeg);
			A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(pDlg->GetData()) / 6.0f);
			A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
			m_aModel[m_nCurProfession][m_nCurGender]->Stand();
			m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vecDir, g_vAxisY);
		}
		else if( ((PAUISTILLIMAGEBUTTON)pDlg->GetDlgItem("Btn_Right"))->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		{
			int nDeg = pDlg->GetData() - g_pGame->GetTickTime();
			while( nDeg < 0 ) nDeg += 2160;
			pDlg->SetData(nDeg);
			A3DMATRIX4 mat = a3d_RotateY(DEG2RAD(pDlg->GetData()) / 6.0f);
			A3DVECTOR3 vecDir = A3DVECTOR3(0, 0, 1.0f) * mat;
			m_aModel[m_nCurProfession][m_nCurGender]->Stand();
			m_aModel[m_nCurProfession][m_nCurGender]->SetDirAndUp(vecDir, g_vAxisY);
		}
		
		A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();
		if (pDlg->GetCheckedRadioButton(1)==1&&m_iCameraPathWeight!=0)
		{
			m_iCameraPathWeight -= (dwTimeNow-dwTime);
			if (m_iCameraPathWeight<0)
				m_iCameraPathWeight = 0;

			int iGender = m_aModel[m_nCurProfession][m_nCurGender]->GetGender();
			int iPhysique = m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique();
			pCamera->SetPos(m_PosCamera[m_iCurScene]*(100-m_iCameraPathWeight)/100.f+m_CloseUpPosCamera[iGender][iPhysique]*m_iCameraPathWeight/100.f);
			pCamera->SetDirAndUp(m_DirCamera[m_iCurScene]*(100-m_iCameraPathWeight)/100.f+m_CloseUpDirCamera[iGender][iPhysique]*m_iCameraPathWeight/100.f, g_vAxisY);
		}
		else if (pDlg->GetCheckedRadioButton(1)==2&&m_iCameraPathWeight!=100)
		{
			m_iCameraPathWeight += (dwTimeNow-dwTime);
			if (m_iCameraPathWeight>100)
				m_iCameraPathWeight = 100;
			
			int iGender = m_aModel[m_nCurProfession][m_nCurGender]->GetGender();
			int iPhysique = m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique();
			pCamera->SetPos(m_PosCamera[m_iCurScene]*(100-m_iCameraPathWeight)/100.f+m_CloseUpPosCamera[iGender][iPhysique]*m_iCameraPathWeight/100.f);
			pCamera->SetDirAndUp(m_DirCamera[m_iCurScene]*(100-m_iCameraPathWeight)/100.f+m_CloseUpDirCamera[iGender][iPhysique]*m_iCameraPathWeight/100.f, g_vAxisY);
		}
	}

	if(GetDialog("Win_Create_Prof03")->IsShow() && GetDialog("Win_Create_Race03")->IsShow())
	{
	    bool isYingZhao = GetDialog("Win_Create_Prof03")->GetCheckedRadioButton(1) == 1;
	    GetDialog("Win_Create_Race03")->GetDlgItem("Rdo_Gender1")->Show(!isYingZhao);
	  
	}

	//第三方登录按钮显隐调整
	if(GetDialog("Win_Login_Link")->IsShow())
	{
	    bool bShow = GetDialog("Win_Login")->IsShow();
        PAUIDIALOG pDlgLoginLink = GetDialog("Win_Login_Link");
		if(pDlgLoginLink->GetDlgItem("Lab_LoginOther"))
		  pDlgLoginLink->GetDlgItem("Lab_LoginOther")->Show(bShow);
		if(pDlgLoginLink->GetDlgItem("Btn_Login3rd_sina"))
			pDlgLoginLink->GetDlgItem("Btn_Login3rd_sina")->Show(bShow);
		if(pDlgLoginLink->GetDlgItem("Btn_Login3rd_qq"))
			pDlgLoginLink->GetDlgItem("Btn_Login3rd_qq")->Show(bShow);
		if(pDlgLoginLink->GetDlgItem("Btn_Login3rd_360"))
			pDlgLoginLink->GetDlgItem("Btn_Login3rd_360")->Show(bShow);
		if(pDlgLoginLink->GetDlgItem("Btn_Login3rd_taobao"))
			pDlgLoginLink->GetDlgItem("Btn_Login3rd_taobao")->Show(bShow);
		if(pDlgLoginLink->GetDlgItem("Btn_Login3rd_renren"))
			pDlgLoginLink->GetDlgItem("Btn_Login3rd_renren")->Show(bShow);
	}
	
	//调整一下Win_LoginLogOut的位置
	if(GetDialog("Win_LoginLogOut")->IsShow())
	{
		PAUIDIALOG pDlgLogOut = GetDialog("Win_LoginLogOut");	
	    if(GetDialog("Win_Login")->IsShow())
		{
			PAUIDIALOG pDlgLogin = GetDialog("Win_Login");	
			int x = pDlgLogin->GetDlgItem("Btn_Login")->GetPos().x;
			SIZE s = pDlgLogOut->GetSize();
			A3DRECT rc = pDlgLogin->GetRect();
			pDlgLogOut->SetPosEx(x, rc.bottom-50*GetWindowScale());
		}
		else
		{
			int x = -1;
			int y = -50;
			y = GetWindowScale()*y;
			pDlgLogOut->TranslatePosForAlign(x,y);
			pDlgLogOut->SetPosEx(x,y);
		}
	}
	
	AdjustCamera();

	PAUIDIALOG pDlg;
	const static int iMoveSpeed = 1000;
	if (m_dwStartRaceTime > 0 && GetDialog("Win_Create")->IsShow() )
	{

		GetDialog("Win_CharList_Cover")->Show(false);
		GetDialog("Win_CharList_Cover2")->Show(false);
		pDlg = GetDialog("Win_Create");
		POINT ptPos = pDlg->GetPos();
		SIZE szDlg = pDlg->GetSize();
		A3DRECT rect = GetRect();
		if (CREATE_TYPE_MIN == m_iCurCreate)
		{
#if 0
			bool bArrive = false;
			if (ptPos.x < (rect.Width() - szDlg.cx) / 2)
			{
				ptPos.x += iMoveSpeed * (dwTimeNow- dwTime) / 1000;
				if (ptPos.x >= (rect.Width() - szDlg.cx) / 2)
					bArrive = true;
			}
			else
			{
				ptPos.x -= iMoveSpeed * (dwTimeNow- dwTime) / 1000;
				if (ptPos.x <=(rect.Width() - szDlg.cx) / 2)
					bArrive = true;
			}
			if( bArrive )
			{
				ptPos.x = (rect.Width() - szDlg.cx) / 2;
				m_dwStartRaceTime = 0;
				if( m_nCurProfession == LOGIN_MODEL_NORMAL )
					GetDialog("Win_Create")->OnCommand("Btn_Human");
				else if( m_nCurProfession == LOGIN_MODEL_BIG )
					GetDialog("Win_Create")->OnCommand("Btn_Wild");
				else if(m_nCurProfession == LOGIN_MODEL_NORMAL2)
					GetDialog("Win_Create")->OnCommand("Btn_Race03");
/*				else
				{
					GetDialog("Win_CharList_Cover")->Show(true);
					GetDialog("Win_CharList_Cover2")->Show(true);
				}
*/			}
			pDlg->SetPosEx(ptPos.x, ptPos.y);

#endif
			{
			  m_dwStartRaceTime = 0;
				if( m_nCurProfession == LOGIN_MODEL_NORMAL )
					GetDialog("Win_Create")->OnCommand("Btn_Human");
				else if( m_nCurProfession == LOGIN_MODEL_BIG )
					GetDialog("Win_Create")->OnCommand("Btn_Wild");
				else if(m_nCurProfession == LOGIN_MODEL_NORMAL2)
					GetDialog("Win_Create")->OnCommand("Btn_Race03");
			}
		}
		else if (CREATE_TYPE_HUMAN == m_iCurCreate)
		{
#if 0		
			if (ptPos.x + szDlg.cx<rect.Width())
			{
				ptPos.x += iMoveSpeed * (dwTimeNow- dwTime) / 1000;
			}
			if (ptPos.x + szDlg.cx>=rect.Width())
			{
				ptPos.x = rect.Width() - szDlg.cx;

				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmhuman");
				//GetDialog("Win_Create_HumanEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
			pDlg->SetPosEx(ptPos.x, ptPos.y);
#endif
			{
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmhuman");
				//GetDialog("Win_Create_HumanEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
			
		}
		else if (CREATE_TYPE_WILD == m_iCurCreate)
		{
#if 0
			if (ptPos.x > 0)
			{
				ptPos.x -= iMoveSpeed * (dwTimeNow- dwTime) / 1000;
			}
			if (ptPos.x <=0)
			{
				ptPos.x = 0;

				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmwild");
				//GetDialog("Win_Create_WildEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
			pDlg->SetPosEx(ptPos.x, ptPos.y);
#endif
			{
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmwild");
				//GetDialog("Win_Create_WildEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
		}
		else if(CREATE_TYPE_RACE03 == m_iCurCreate)
		{
#if 0
			if (ptPos.x > 0)
			{
				ptPos.x -= iMoveSpeed * (dwTimeNow- dwTime) / 1000;
			}
			if (ptPos.x <=0)
			{
				ptPos.x = 0;

				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmrace03");
				//GetDialog("Win_Create_WildEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
			pDlg->SetPosEx(ptPos.x, ptPos.y);
#endif
			{
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(true);
				GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->SetCommand("confirmrace03");
				//GetDialog("Win_Create_WildEnter")->Show(true);
				m_dwStartRaceTime = 0;
			}
		}
		else
			assert(0);
	}
	//鼠标移入高亮
/*	if (GetDialog("Win_Create")->IsShow())
	{
		PAUIDIALOG pDlgCover1 = GetDialog("Win_CharList_Cover");
		PAUIDIALOG pDlgCover2 = GetDialog("Win_CharList_Cover2");
		PAUIIMAGEPICTURE pImgCover1 = (PAUIIMAGEPICTURE)pDlgCover1->GetDlgItem("Img_Cover");
		PAUIIMAGEPICTURE pImgCover2 = (PAUIIMAGEPICTURE)pDlgCover2->GetDlgItem("Img_Cover");
		if ( CREATE_TYPE_MIN == m_iCurCreate)
		{
			if (pImgCover1->GetAlpha()<255 && (m_pDlgMouseOn==GetDialog("Win_Create") || m_pDlgMouseOn==pDlgCover2))
			{
				int iAlpha = pImgCover1->GetAlpha();
				iAlpha += (dwTimeNow- dwTime);
				iAlpha = iAlpha>255 ? 255 : iAlpha;
				pImgCover1->SetAlpha(iAlpha);
			}
			else if (pImgCover2->GetAlpha()<255 && (m_pDlgMouseOn==GetDialog("Win_Create") || m_pDlgMouseOn==pDlgCover1))
			{
				int iAlpha = pImgCover2->GetAlpha();
				iAlpha += (dwTimeNow- dwTime);
				iAlpha = iAlpha>255 ? 255 : iAlpha;
				pImgCover2->SetAlpha(iAlpha);
			}
			else if (m_pDlgMouseOn == pDlgCover1 && pImgCover1->GetAlpha()>0 )
			{
				int iAlpha = pImgCover1->GetAlpha();
				iAlpha -= (dwTimeNow- dwTime);
				iAlpha = iAlpha<0 ? 0 : iAlpha;
				pImgCover1->SetAlpha(iAlpha);
			}
			else if (m_pDlgMouseOn == pDlgCover2 && pImgCover2->GetAlpha()>0 )
			{
				int iAlpha = pImgCover2->GetAlpha();
				iAlpha -= (dwTimeNow- dwTime);
				iAlpha = iAlpha<0 ? 0 : iAlpha;
				pImgCover2->SetAlpha(iAlpha);
			}
		}
		else
		{
			pImgCover1->SetAlpha(255);
			pImgCover2->SetAlpha(255);
		}
	}
*/
	// 上下选择线路
	if (GetDialog("Win_Char_SelectLineList")->IsShow())
	{
		PAUILISTBOX pList = (PAUILISTBOX)GetDialog("Win_Char_SelectLineList")->GetDlgItem("Lst_Line");
		if( pList->GetCurSel() != (int)pList->GetData("cur_sel") )
		{
			OnCommand_SelChar("selectline", GetDialog("Win_Char_SelectLineList"));
		}
	}

	// Tick: check if user input '@'
	ACString strName = GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->GetText();
	AUIListBox* pMailList = (AUIListBox*)GetDialog("Win_Login")->GetDlgItem("Lst_Mail");
	ACString szSuffix = a_strstr(strName, _AL("@"));

	static ACHAR szOldName[256];

	if(GetDialog("Win_Login")->GetDlgItem("DEFAULT_Txt_Account")->IsFocus() && m_vecMailList.size() != 0)
	{
		if(szSuffix.GetLength() != 0)	// found '@'
		{
			// calc mail suffix
			size_t i;

			// check whether need to change content
			if(a_strcmp(strName, szOldName) != 0)
			{
				// clear list box first
				pMailList->ResetContent();

				for(i=0;i<m_vecMailList.size();i++)
				{			
					if(wcsncmp(szSuffix, m_vecMailList[i], szSuffix.GetLength()) == 0)
					{
						pMailList->AddString(m_vecMailList[i]);
					}
				}

				a_strcpy(szOldName, strName);
			}

			if(pMailList && pMailList->GetCount() != 0)
			{
				pMailList->Show(true);
			}
			else
			{
				pMailList->Show(false);
			}
		}
		else
			pMailList->Show(false);
	}
	else
	{
		if(pMailList)
			pMailList->Show(false);
	}

	dwTime = dwTimeNow;

	return AUILuaManager::Tick(dwDeltaTime);
}

bool CECLoginUIMan::RenderBackground(void)
{
	// when playing intro movie, we don't want to show any other things on the back.
	if( m_pIntroMovie && m_pIntroMovie->IsPlaying() )
		return true;
		
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	CECViewport* pViewport = g_pGame->GetViewport();
#ifdef BACK_VERSION
	A3DViewport* pA3DViewport = pViewport->GetA3DViewport();
#else
	pA3DEngine->GetA3DEnvironment()->SetGlobalFogEnable(false);
	pViewport = g_pGame->GetViewport();
#endif

	if( GetDialog("Win_LoginLogOut")->IsShow())
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxLogin, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
 		AfxGetGFXExMan()->Show2DGfx(m_idGfxLogin, false);
	}
/*	else if (GetDialog("Win_LoginPage")->IsShow() )
	{
		if( m_pLoginBack )
			m_pLoginBack->DrawToBack(0, 0);
	}
	else if( GetDialog("Win_CharTaskTip")->IsShow() )
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxSelectBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxSelectBack, false);
	}
*/	
	//暂定
	else if(GetDialog("Win_CharList")->IsShow())
	{
		//g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxSelectRoleBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxSelectRoleBack, false);
	}
	else if (GetDialog("Win_Create_Human")->IsShow())
	{
		//g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxHumanCreateBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxHumanCreateBack, false);
	}
	else if (GetDialog("Win_Create_Wild")->IsShow())
	{
		//g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxWildCreateBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxWildCreateBack, false);
	}
	else if(GetDialog("Win_Create_Race03")->IsShow())
	{
		//g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxTianmaiCreateBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_iGfxTianmaiCreateBack, false);
	}
	else if( GetDialog("Win_Create")->IsShow())
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateBack, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateBack, false);
	}else if( GetDialog("Win_Create")->IsShow() && m_iCurCreate == CREATE_TYPE_MIN)
	{
 		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
 		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateBack, true);
 		//AfxGetGFXExMan()->Register2DGfx();
		//AfxGetGFXExMan()->Render2DGfx(true);
 		//AfxGetGFXExMan()->Render2DGfx(false);
 		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateBack, false);
	}
	else if( GetDialog("Win_Create")->IsShow() &&  m_iCurCreate == CREATE_TYPE_HUMAN)
	{
	    g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateHuman, true);
		//AfxGetGFXExMan()->Register2DGfx();
		//AfxGetGFXExMan()->Render2DGfx(true);
		//AfxGetGFXExMan()->Render2DGfx(false);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateHuman, false);
	}
	else if( GetDialog("Win_Create")->IsShow() &&  m_iCurCreate == CREATE_TYPE_WILD)
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateWild, true);
		//AfxGetGFXExMan()->Register2DGfx();
		//AfxGetGFXExMan()->Render2DGfx(true);
		//AfxGetGFXExMan()->Render2DGfx(false);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateWild, false);
	}
	else if( GetDialog("Win_Create")->IsShow() &&  m_iCurCreate == CREATE_TYPE_RACE03)
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateWild, true);
		//AfxGetGFXExMan()->Register2DGfx();
		//AfxGetGFXExMan()->Render2DGfx(true);
		//AfxGetGFXExMan()->Render2DGfx(false);
		//AfxGetGFXExMan()->Show2DGfx(m_idGfxCreateWild, false);
	}
/*	else if (GetDialog("Win_Create_Human")->IsShow())
	{
		if (m_pCreateBackHuman)
			m_pCreateBackHuman->DrawToBack(0, 0);
	}
	else if (GetDialog("Win_Create_Wild")->IsShow())
	{
		if (m_pCreateBackWild)
			m_pCreateBackWild->DrawToBack(0, 0);
	}
*/	else if (GetDialog("Win_LoginWait")->IsShow())
	{
		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		if (m_pLoginBack)
			m_pLoginBack->DrawToBack(0, 0);
	}
	//暂定
	else
	{
#ifdef BACK_VERSION
		CECViewport* pViewport = g_pGame->GetViewport();
		g_pGame->GetGameRun()->GetWorld()->Render(pViewport);
#endif

		g_pGame->GetA3DDevice()->Clear(D3DCLEAR_TARGET, 0xff000000, 1.0f, 0);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxLogin, true);
		AfxGetGFXExMan()->Register2DGfx();
		AfxGetGFXExMan()->Render2DGfx(true);
		AfxGetGFXExMan()->Render2DGfx(false);
		AfxGetGFXExMan()->Show2DGfx(m_idGfxLogin, false);
	}

	return true;
}

bool CECLoginUIMan::Render(void)
{
	A3DEngine* pA3DEngine = g_pGame->GetA3DEngine();
	
#ifdef BACK_VERSION
	CECViewport* pViewport = g_pGame->GetViewport();
#else
	ECViewport* pViewport = g_pGame->GetViewport();
#endif
	A3DViewport* pA3DViewport = pViewport->GetA3DViewport();
#ifndef BACK_VERSION
	bool bZTestEnable = pA3DEngine->GetA3DDevice()->GetZTestEnable();
	pA3DEngine->GetA3DDevice()->SetZTestEnable(false);
#endif

	if( m_pIntroMovie && m_pIntroMovie->IsPlaying() )
	{
		pA3DEngine->GetA3DDevice()->Clear(D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0);
		m_pIntroMovie->DrawToBack();
		m_pLogo->DrawToBack();
		return true;
	}

	const char *szCountry = g_pGame->GetConfigs()->GetCountryRegion();
	if( GetDialog("Win_Login")->IsShow() && 0 == stricmp(szCountry, "kr"))
		m_pKrLogo->DrawToBack();
	if( GetDialog("Win_Login")->IsShow() ||
		GetDialog("Win_LoginServerList")->IsShow())
	{
		//m_pLogo->DrawToBack();
	}

	if( GetDialog("Win_Login")->IsShow() ||
		GetDialog("Win_LoginWait")->IsShow() ||
		GetDialog("Win_LoginServerList")->IsShow())
	{

		ACHAR szText[40];
		DWORD dwVersion = g_pGame->GetGameVersion();
		DWORD dwVer1 = (dwVersion & 0x00FF0000) >> 16;
		DWORD dwVer2 = (dwVersion & 0x0000FF00) >> 8;
		DWORD dwVer3 = (dwVersion & 0x000000FF);
		A3DVIEWPORTPARAM *p = pA3DViewport->GetParam();

		a_sprintf(szText, _AL("Ver %d.%d.%d"), dwVer1, dwVer2, dwVer3);
		m_pA3DEngine->GetSystemFont()->TextOut(p->Width - 160,
			p->Y + p->Height - 20, szText, A3DCOLORRGB(0, 180, 255));

		DWORD dwBuild = g_pGame->GetGameBuild();
		a_sprintf(szText, _AL("Build %d"), dwBuild);
		m_pA3DEngine->GetSystemFont()->TextOut(p->Width - 75,
			p->Y + p->Height - 20, szText, A3DCOLORRGB(0, 180, 255));

		m_pA3DEngine->GetSystemFont()->TextOut(25,
			p->Y + p->Height - 20, _AL("ISBN: 978-7-89989-022-6"), A3DCOLORRGB(255, 255, 255));

		m_pA3DEngine->GetSystemFont()->Flush();
	}
	else if( GetDialog("Win_CharList")->IsShow() )
	{
		/*
		bool bMonth = false;
		ACString szText;
		long stime = g_pGame->GetGameSession()->GetFreeGameEndTime();
		if( g_pGame->GetGameSession()->GetFreeGameEndTime() == -1 )
		{
			GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Month")->SetText(GetStringFromTable(219));
			bMonth = true;
		}
		else if( g_pGame->GetGameSession()->GetFreeGameTime() > 0 )
		{
			stime += 8 * 3600;
			tm *gtime = gmtime(&stime);
			szText.Format(GetStringFromTable(218), gtime->tm_year + 1900, gtime->tm_mon + 1, 
				gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
			if( g_pGame->GetGameSession()->GetFreeGameTime() > 10 * 3600 * 1000 )
				GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Month")->SetText(szText);
			else
				GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Month")->SetText(_AL("^FF0000") + szText);
			bMonth = true;
		}
		else
			GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Month")->SetText(_AL(""));


		DWORD nSec = DWORD(g_pGame->GetGameSession()->GetAccountTime() / 1000);
		nSec = max(nSec, 0);
		DWORD nMin = nSec / 60;
		nSec -= nMin * 60;
		DWORD nHour = nMin / 60;
		nMin -= nHour * 60;
//		DWORD nDay = nHour / 24;
//		nHour -= nDay * 24;
		szText.Format(GetStringFromTable(215), nHour, nMin);
		if( !bMonth )
			GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Point")->SetText(szText);
		else if( nHour < 10 )
			GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Point")->SetText(_AL("^FF0000") + szText);
		else
			GetDialog("Win_LoginAccountTime")->GetDlgItem("Txt_Point")->SetText(_AL("^007F00") + szText);

//		GetDialog("Win_Select")->GetDlgItem("Txt_Timeleft")->SetText(szText);
		*/
	}
//	if( GetDialog("Win_Create")->IsShow() && CREATE_TYPE_MIN == m_iCurCreate && m_dwStartRaceTime == 0 )
//		RenderShowModel();

	bool bRet = AUILuaManager::Render();
#ifndef BACK_VERSION
	pA3DEngine->GetA3DDevice()->SetZTestEnable(bZTestEnable);	
#endif


	return bRet;
}

int CECLoginUIMan::PickRole(int x, int y)
{
	float fFraction;
	A3DVECTOR3 vecHit, vecNormal;
	A3DVECTOR3 vecPos(float(x), float(y), 1.0f);
	A3DVECTOR3 vecCam = g_pGame->GetViewport()->GetA3DCamera()->GetPos();
	g_pGame->GetViewport()->GetA3DViewport()->InvTransform(vecPos, vecPos);

	for( int i = 0; i < int(m_vecRoleModel.size()); i++ )
	{
		const A3DAABB& aabb = m_vecRoleModel[i]->GetPlayerAABB();
		if( CLS_RayToAABB3(vecCam, vecPos - vecCam, aabb.Mins, aabb.Maxs, vecHit, &fFraction, vecNormal) )
			return i;
	}

	return -1;
}

void CECLoginUIMan::ShowErrorMsg(const ACHAR *pszMsg, const char *pszName)
{
	if( GetDialog("Win_LoginWait")->IsShow() )
	{
		GetDialog("Win_LoginWait")->Show(false);
		ShowCombineDialogs("Login");
	}
	
    const char* pszErrId = NULL;
	int errId = -1;
	if ((pszErrId = strstr(pszName, "LinkSevError")))
	{
		sscanf(pszErrId, "LinkSevError%d", &errId);
		//手机令牌
		if(errId == 137)
		{
			PAUIDIALOG pDlgPhone = GetDialog("Win_PwdProtect_Phone2");
			if(pDlgPhone)
			{
				pDlgPhone->Show(true,true);
				
			}
			return;
		}

	}
	MessageBox(pszName, pszMsg, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

bool CECLoginUIMan::UIControlCursor()
{
	return false;
}

bool CECLoginUIMan::RefreshPlayerList()
{
	CECPlayerMan::LoginPlayerTable& players = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetLoginPlayerTable();
	players.clear();

	if (GetDialog("Win_Create")->IsShow())
	{
	}
	else if( GetDialog("Win_Create_Human")->IsShow() || GetDialog("Win_Create_Wild")->IsShow() || GetDialog("Win_Create_Race03")->IsShow())
	{
		if( m_aModel[m_nCurProfession][m_nCurGender] )
		{
			players.push_back(m_aModel[m_nCurProfession][m_nCurGender]);
			A3DVECTOR3 dir = m_aModel[m_nCurProfession][m_nCurGender]->GetDir();
			int nDeg = (int)RAD2DEG((float)atan2(dir.x, dir.z));
			while(nDeg<0) nDeg += 2160;
			GetDialog("Win_CreateName")->SetData(nDeg*6);
			/*if (CREATE_TYPE_HUMAN == m_iCurCreate)
				m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_HUMAN]);
			else
				m_aModel[m_nCurProfession][m_nCurGender]->SetPos(m_PosRole[LOGIN_SCENE_CREATE_WILD]);
		*/}
	}
	else if( !GetDialog("Win_CharTaskTip")->IsShow()  )
	{
		if( m_idCurRole >= 0 )
		{
			players.push_back(m_vecRoleModel[m_idCurRole]);	
			A3DVECTOR3 dir = m_vecRoleModel[m_idCurRole]->GetDir();
			int nDeg = (int)RAD2DEG((float)atan2(dir.x, dir.z));
			GetDialog("Win_Char_Rotate")->SetData(nDeg*6);
		//	m_vecRoleModel[m_idCurRole]->SetPos(m_PosRole[LOGIN_SCENE_SELCHAR]);
			ChangeScene(LOGIN_SCENE_SELCHAR);
		}
	}
	return true;
}

void CECLoginUIMan::AdjustCamera()
{
	if( !AUI_PRESS('C') || !AUI_PRESS('V') ) return;

	static float fDeltaDeg = 0.05f;
	static float fDeltaDist = 0.02f;
	A3DCamera * pCamera = g_pGame->GetViewport()->GetA3DCamera();

	if( AUI_PRESS(VK_MENU) )
		fDeltaDist = 0.002f;
	else
		fDeltaDist = 0.02f;

	if( AUI_PRESS(VK_HOME) || AUI_PRESS(VK_UP) )
	{
		if( AUI_PRESS(VK_CONTROL) )
			pCamera->PitchDelta(fDeltaDeg);
		else
			pCamera->MoveFront(fDeltaDist);
	}
	else if( AUI_PRESS(VK_END) || AUI_PRESS(VK_DOWN) )
	{
		if( AUI_PRESS(VK_CONTROL) )
			pCamera->PitchDelta(-fDeltaDeg);
		else
			pCamera->MoveBack(fDeltaDist);
	}
	else if( AUI_PRESS(VK_LEFT) )
	{
		if( AUI_PRESS(VK_CONTROL) )
			pCamera->DegDelta(-fDeltaDeg);
		else
			pCamera->MoveLeft(fDeltaDist);
	}
	else if( AUI_PRESS(VK_RIGHT) )
	{
		if( AUI_PRESS(VK_CONTROL) )
			pCamera->DegDelta(fDeltaDeg);
		else
			pCamera->MoveRight(fDeltaDist);
	}
	else if( AUI_PRESS(VK_PRIOR) )
		pCamera->Move(g_vAxisY * fDeltaDist);
	else if( AUI_PRESS(VK_NEXT) )
		pCamera->Move(-g_vAxisY * fDeltaDist);

	PAUIDIALOG pDlg = GetDialog("Win_CreateName");
	if (pDlg->IsShow()&&pDlg->GetCheckedRadioButton(1)==2)
	{
		int iGender = m_aModel[m_nCurProfession][m_nCurGender]->GetGender();
		int iPhysique = m_aModel[m_nCurProfession][m_nCurGender]->GetPhysique();
		m_CloseUpPosCamera[iGender][iPhysique] =  pCamera->GetPos();
		m_CloseUpDirCamera[iGender][iPhysique] = pCamera->GetDir();
	}
	else
	{
		m_PosCamera[m_iCurScene] = pCamera->GetPos();
		m_DirCamera[m_iCurScene] = pCamera->GetDir();
	}
/*
	m_vecCamPos = pCamera->GetPos();
	m_vecCamDir = pCamera->GetDir();
	m_vecCamUp = pCamera->GetUp();
*/}

void CECLoginUIMan::AdjustRole(int x, int y)
{
	RAYTRACERT TraceRt;
	CECCDS *pCDS = g_pGame->GetGameRun()->GetWorld()->GetCDS();
	A3DCamera *pCamera = g_pGame->GetViewport()->GetA3DCamera();
	A3DViewport *pView = g_pGame->GetViewport()->GetA3DViewport();
	A3DVECTOR3 vecStart, vecDest(float(x), float(y), 1.0f), vecDelta, vecDir;

	pView->InvTransform(vecDest, vecDest);
	vecStart = pCamera->GetPos();
	vecDelta = vecDest - vecStart;

	if( !pCDS->RayTrace(vecStart, vecDelta, 1.0f, &TraceRt, TRACEOBJ_SCENE, 0) )
		return;

	vecDir = pCamera->GetPos() - TraceRt.vPoint;
	vecDir.y = 0.0f;
}

void CECLoginUIMan::RebuildRoleList()
{
	int i;
	bool bSwapped;
	GNET::RoleInfo ri;
	CECLoginPlayer *pModel;

	do
	{
		bSwapped = false;
		for( i = 0; i < (int)m_vecRoleInfo.size() - 1; i++ )
		{
			if( m_vecRoleInfo[i].create_time > m_vecRoleInfo[i + 1].create_time )
			{
				ri = m_vecRoleInfo[i];
				m_vecRoleInfo[i] = m_vecRoleInfo[i + 1];
				m_vecRoleInfo[i + 1] = ri;

				pModel = m_vecRoleModel[i];
				m_vecRoleModel[i] = m_vecRoleModel[i + 1];
				m_vecRoleModel[i + 1] = pModel;
				
				bSwapped = true;
			}
		}
	} while( bSwapped );

 	char szName[40];
	ACString strLevel;
	PAUIOBJECT pRadio, pName, pLevel, pAgent, pGfxCrossServer, pToSell, pToSellImg;
	PAUIIMAGEPICTURE pProf;
	PAUIDIALOG pDlg = GetDialog("Win_CharList");
	
	for( i = 0; i < CECLOGINUIMAN_MAX_ROLES; i++ )
	{
		sprintf(szName, "Rdo_Char%d", i + 1);
		pRadio = pDlg->GetDlgItem(szName);

		sprintf(szName, "Txt_Name%d", i + 1);
		pName = pDlg->GetDlgItem(szName);
		
		sprintf(szName, "Txt_Level%d", i + 1);
		pLevel = pDlg->GetDlgItem(szName);

		sprintf(szName, "Img_Char%d", i + 1);
		pAgent = pDlg->GetDlgItem(szName);

		sprintf(szName, "Gfx_%d", i + 1);
		pGfxCrossServer = pDlg->GetDlgItem(szName);

		sprintf(szName, "Btn_CharConsignCancel_%d", i + 1);
		pToSell = pDlg->GetDlgItem(szName);
		
		sprintf(szName, "Img_CharConsign%d", i + 1);
		pToSellImg = pDlg->GetDlgItem(szName);
		
		sprintf(szName, "Img_Prof%d", i + 1);
		pProf = (PAUIIMAGEPICTURE)pDlg->GetDlgItem(szName);

		if( i < (int)m_vecRoleInfo.size() )
		{
			pRadio->Show(true);

			ACHAR szText[256];
			ACString strName((const ACHAR *)m_vecRoleInfo[i].name.begin(),
				m_vecRoleInfo[i].name.size() / sizeof(ACHAR));
			AUI_ConvertChatString(strName, szText);
			pName->SetText(szText);
			pName->Show(true);

			if( m_vecRoleInfo[i].status == _ROLE_STATUS_NORMAL )
				pName->SetColor(A3DCOLORRGB(255, 203, 74));
			else if( m_vecRoleInfo[i].status == _ROLE_STATUS_FROZEN )
				pName->SetColor(A3DCOLORRGB(128, 255, 255));
			else
				pName->SetColor(A3DCOLORRGB(128, 128, 128));

			strLevel.Format(_AL("%s%s  %d"), GetStringFromTable(600 + m_vecRoleInfo[i].cultivation),
				GetStringFromTable(PROFESSION_START + m_vecRoleInfo[i].occupation),
				m_vecRoleInfo[i].level);
			pLevel->SetText(strLevel);
			pLevel->Show(true);

			bool bTrusteeMode = false;
			int* aModes = (int*)m_vecRoleInfo[i].charactermode.begin();
			int iNumMode = m_vecRoleInfo[i].charactermode.size() / 8;
			if (aModes && iNumMode)
			{
				for (int j=0; j < iNumMode; j++)
				{
					int iKey = *aModes++;
					int iValue = *aModes++;
					
					switch (iKey)
					{
					case 4: bTrusteeMode = iValue ? true : false; break;
					default:
						break;
					}
				}
			}

			pAgent->Show(bTrusteeMode);
			pGfxCrossServer->Show(m_vecRoleInfo[i].status == _ROLE_STATUS_FROZEN);
			pProf->Show(true);
			pProf->FixFrame(CDlgSkillMan::Prof2Index(m_vecRoleInfo[i].occupation));
			pToSell->Show(m_vecRoleInfo[i].status == _ROLE_STATUS_SELLING);
			pToSellImg->Show(m_vecRoleInfo[i].status == _ROLE_STATUS_SELLING);
		}
		else
		{
			pRadio->Show(false);
			pName->Show(false);
			pLevel->Show(false);
			pAgent->Show(false);
			pGfxCrossServer->Show(false);
			pProf->Show(false);
			pToSell->Show(false);
			pToSellImg->Show(false);
		}
	}
	m_bEnter = false;
}

void CECLoginUIMan::LoadUserServer(bool bSel)
{
	AIniFile theIni;
	ACHAR szGroupName[200];
	ACHAR szServerName[200];
	//ACHAR szServerAddress[200];
	GetPrivateProfileString(_AL("Server"), _AL("CurrentGroup"),
		_AL(""), szGroupName, 200,  _AL("userdata\\currentserver.ini"));
	GetPrivateProfileString(_AL("Server"), _AL("CurrentServer"),
		_AL(""), szServerName, 200,  _AL("userdata\\currentserver.ini"));
	// 因为现在一个服务器可能有多个地址，所以这个记录已经弃用
	//GetPrivateProfileString(_AL("Server"), _AL("CurrentServerAddress"),
	//	_AL(""), szServerAddress, 200,  _AL("userdata\\currentserver.ini"));
	g_pGame->GetGameRun()->SetCurGroup(szGroupName);
	g_pGame->GetGameRun()->SetServerName(szServerName);
	
	CDlgLoginServerList *pDlgServerList = (CDlgLoginServerList *)GetDialog("Win_LoginServerList");
	pDlgServerList->SetServerAddress(szServerName, szGroupName);	
	pDlgServerList->Show(bSel);
	pDlgServerList->ChangeFocus(pDlgServerList->GetDlgItem("Lst_Server"));
}

void CECLoginUIMan::SaveServer(ACString strServerName, ACString strAddr)
{	
	g_pGame->GetGameRun()->SetServerName(strServerName);
	WritePrivateProfileString(_AL("Server"), _AL("CurrentGroup"), g_pGame->GetGameRun()->GetCurGroup(), 
		_AL("userdata\\currentserver.ini"));
	WritePrivateProfileString(_AL("Server"), _AL("CurrentServer"), strServerName, 
		_AL("userdata\\currentserver.ini"));
	// 因为现在一个服务器可能有多个地址，所以这个记录已经弃用
	//WritePrivateProfileString(_AL("Server"), _AL("CurrentServerAddress"), strAddr, 
	//	_AL("userdata\\currentserver.ini"));
}

void CECLoginUIMan::LoadMailSuffixList()
{	
	AFile file;
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\configs\\mail_suffix.txt", af_GetBaseDir());
	if( file.Open(szFile, AFILE_TEXT|AFILE_OPENEXIST|AFILE_TEMPMEMORY))
	{
		char line[AFILE_LINEMAXLEN];
		DWORD  read_len;
		
		while (file.ReadLine(line, AFILE_LINEMAXLEN, &read_len))
		{
			m_vecMailList.push_back(AS2WC(line));
		}
		file.Close();
	}

}
PAUIDIALOG CECLoginUIMan::CreateDlgInstance(const AString strTemplName)
{
// code template: 
// if (strTemplName == "???.xml")
//		return new CDlg???;

// if ...

	PAUIDIALOG pDlg;
	if( strTemplName == "LoginServerList.xml")
		pDlg = new CDlgLoginServerList;
	else if( strTemplName == "LoginPage.xml")
		pDlg = new CDlgLoginPage;
	else if( strTemplName == "PopID.xml")
		pDlg = new CDlgLoginCommon;
	else if( strTemplName == "Login.xml")
		pDlg = new CDlgLoginCommon;
	else if( strTemplName == "LoginServerListSearch.xml")
		pDlg = new DlgLoginServerSearch;
	else if( strTemplName == "PwdProtect.xml")
		pDlg = new CDlgLoginPwdProtect;
	else if( strTemplName == "Login_explorer.xml")
		pDlg = new CDlgLoginOtherPlatform;
	else
		pDlg = new AUIDialog;

	pDlg->SetOptimizeResourceLoad(true);
	return pDlg;
}

void CECLoginUIMan::RearrangeWindows(A3DRECT rcOld, A3DRECT rcNew)
{
	if( m_pIntroMovie )
	{
		m_pIntroMovie->SetAutoScaleFlags(AMVID_FIT_X | AMVID_FIT_Y | AMVID_FIT_KEEPCONSTRAINT);
	}

	float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	float sy = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 768.0f;
	m_pLogo->SetScaleX(sx);
	m_pLogo->SetScaleY(sy);
	m_pLogo->SetPosition(g_pGame->GetConfigs()->GetSystemSettings().iRndWidth/2, 0);
	m_pLogo->SetLocalCenterPos(m_pLogo->GetWidth()/2, 0);
	m_pKrLogo->SetScaleX(sx);
	m_pKrLogo->SetScaleY(sy);
	m_pKrLogo->SetPosition(g_pGame->GetConfigs()->GetSystemSettings().iRndWidth - int((m_pLogo->GetWidth() + 40)*sx), g_pGame->GetConfigs()->GetSystemSettings().iRndHeight/3);

	m_pLoginBack->SetScaleX(sx);
	m_pLoginBack->SetScaleY(sy);	

/*	m_pCreateBackHuman->SetScaleX(sx);
	m_pCreateBackHuman->SetScaleY(sy);	
	m_pCreateBackWild->SetScaleX(sx);
	m_pCreateBackWild->SetScaleY(sy);	
*/
	AUILuaManager::RearrangeWindows(rcOld, rcNew);
	
	if (m_idGfxLogin)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_idGfxLogin, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_idGfxLogin, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}

	if (m_idGfxCreateBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_idGfxCreateBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_idGfxCreateBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}

/*	if (m_idGfxSelectBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_idGfxSelectBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_idGfxSelectBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
*/	if (m_idGfxCreateHuman)
	{
		//AfxGetGFXExMan()->Set2DGfxScale(m_idGfxCreateHuman, m_fWindowScale);
		//AfxGetGFXExMan()->Set2DGfxPos(m_idGfxCreateHuman, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	if (m_idGfxCreateWild)
	{
		//AfxGetGFXExMan()->Set2DGfxScale(m_idGfxCreateWild, m_fWindowScale);
		//AfxGetGFXExMan()->Set2DGfxPos(m_idGfxCreateWild, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	if(m_iGfxSelectRoleBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_iGfxSelectRoleBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_iGfxSelectRoleBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	if(m_iGfxHumanCreateBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_iGfxHumanCreateBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_iGfxHumanCreateBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	if(m_iGfxWildCreateBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_iGfxWildCreateBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_iGfxWildCreateBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	if(m_iGfxTianmaiCreateBack)
	{
		AfxGetGFXExMan()->Set2DGfxScale(m_iGfxTianmaiCreateBack, m_fWindowScale);
		AfxGetGFXExMan()->Set2DGfxPos(m_iGfxTianmaiCreateBack, (float)rcNew.CenterPoint().x, (float)rcNew.CenterPoint().y);
	}
	PAUIDIALOG pDlg = GetDialog("Win_CharList_Cover");
	GetDialog("Win_CharList_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover")->GetDlgItem("Img_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover")->SetPosEx(0,0);
	GetDialog("Win_CharList_Cover2")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover2")->GetDlgItem("Img_Cover")->SetSize((m_rcWindow.right - m_rcWindow.left)/2, m_rcWindow.bottom - m_rcWindow.top);
	GetDialog("Win_CharList_Cover2")->SetPosEx((m_rcWindow.right - m_rcWindow.left)/2,0);
}

void CECLoginUIMan::ShowCombineDialogs(const char *szName, bool bShow)
{
	if( stricmp(szName, "Login") == 0 )
	{
		GetDialog("Win_LoginLogOut")->Show(bShow);
		GetDialog("Win_LoginReplay")->Show(false);
		GetDialog("Win_Login_Link")->Show(bShow);
		GetDialog("Win_Login")->Show(bShow);
		GetDialog("Win_Login_Anime")->Show(bShow);

		PAUIDIALOG pDlgLogin = GetDialog("Win_Login");
		if (bShow)
		{
			if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szToken[0])
			{
				pDlgLogin->Show(false);
			}
			else if (g_pGame->GetGameInit().szUserName[0] && g_pGame->GetGameInit().szPassword[0])
			{
				pDlgLogin->Show(false);
			}
			else
			{
				pDlgLogin->Show(true);
			}
			
			PAUICHECKBOX pChk = (PAUICHECKBOX)GetDialog("Win_Login_Anime")->GetDlgItem("Chk_Anime");
			pChk->Check(!g_pGame->GetConfigs()->GetIsSkipMovie());
		}
		else
			pDlgLogin->Show(false);
	}
	else if( stricmp(szName, "CharList") == 0 )
	{
		GetDialog("Win_LoginAccountTime")->Show(bShow);
		GetDialog("Win_CharEnterWorld")->Show(bShow);
		GetDialog("Win_CharReturn")->Show(bShow);
		GetDialog("Win_CharList")->Show(bShow);
		if(bShow)
			RebuildRoleList();
		GetDialog("Win_Char_Rotate")->Show(bShow);
		GetDialog("Win_PwdProtectHint")->Show(bShow);
		GetDialog("Win_Char_SelectLineList")->Show(bShow);
		GetDialog("Win_Char_SelectLine")->Show(bShow);
		if (bShow)
		{
			ChangeScene(LOGIN_SCENE_SELCHAR);
		}
	}
	else if( stricmp(szName, "CreatePrev") == 0 )
	{
		if (bShow)
		{
			m_iCurCreate = CREATE_TYPE_MIN;
			m_nCurProfession = -1;
		}
		GetDialog("Win_Create")->Show(bShow);
		GetDialog("Win_Create")->GetDlgItem("Btn_Human")->Enable(true);
		GetDialog("Win_Create")->GetDlgItem("Btn_Wild")->Enable(true);
		GetDialog("Win_Create")->GetDlgItem("Btn_Race03")->Enable(true);
		GetDialog("Win_Create")->GetDlgItem("Btn_Enter")->Show(false);
		GetDialog("Win_Create")->GetDlgItem("Btn_Cancel")->SetCommand("IDCANCEL");
//		GetDialog("Win_CharList_Cover")->Show(bShow);
//		GetDialog("Win_CharList_Cover2")->Show(bShow);
		int x = -1;
		int y = 0;
		GetDialog("Win_Create")->TranslatePosForAlign(x,y);
		GetDialog("Win_Create")->SetPosEx(x,y);
		GetDialog("Win_Create_HumanEnter")->Show(false);
		GetDialog("Win_Create_WildEnter")->Show(false);
		//GetDialog("Win_Create_Race03")->Show(false);
	}
	else if( stricmp(szName, "CreateMain") == 0 )
	{
		if (bShow)
		{
			GetDialog("Win_Create_Human")->Show(CREATE_TYPE_HUMAN == m_iCurCreate);
			GetDialog("Win_Create_Prof01")->Show(CREATE_TYPE_HUMAN == m_iCurCreate);
			GetDialog("Win_Create_Wild")->Show(CREATE_TYPE_WILD == m_iCurCreate);
			GetDialog("Win_Create_Prof02")->Show(CREATE_TYPE_WILD == m_iCurCreate);
			GetDialog("Win_Create_Race03")->Show(CREATE_TYPE_RACE03 == m_iCurCreate);
			GetDialog("Win_Create_Prof03")->Show(CREATE_TYPE_RACE03 == m_iCurCreate);
			if (CREATE_TYPE_HUMAN == m_iCurCreate)
			{
				ChangeScene(LOGIN_SCENE_CREATE_HUMAN);
				GetDialog("Win_PopID")->SetPosEx(10, m_rcWindow.Height()-GetDialog("Win_PopID")->GetSize().cy-10);
			}
			else if(CREATE_TYPE_WILD == m_iCurCreate)
			{
				ChangeScene(LOGIN_SCENE_CREATE_WILD);
				GetDialog("Win_PopID")->SetPosEx(m_rcWindow.Width()-GetDialog("Win_PopID")->GetSize().cx-10, m_rcWindow.Height()-GetDialog("Win_PopID")->GetSize().cy-10);
			}
			else if(CREATE_TYPE_RACE03 == m_iCurCreate)
			{
				ChangeScene(LOGIN_SCENE_CREATE_QIANJI);
				GetDialog("Win_PopID")->SetPosEx(m_rcWindow.Width()-GetDialog("Win_PopID")->GetSize().cx-10, m_rcWindow.Height()-GetDialog("Win_PopID")->GetSize().cy-10);
			}
		}
		else
		{
			m_iCurCreate = CREATE_TYPE_MIN;
			GetDialog("Win_Create_Human")->Show(bShow);
			GetDialog("Win_Create_Wild")->Show(bShow);
			GetDialog("Win_Create_Race03")->Show(bShow);
			GetDialog("Win_Create_Prof01")->Show(bShow);
			GetDialog("Win_Create_Prof02")->Show(bShow);
			GetDialog("Win_Create_Prof03")->Show(bShow);
		}

		GetDialog("Win_CreateConfirm")->Show(bShow);
		GetDialog("Win_CreateName")->Show(bShow);
		if ( g_pGame->GetConfigs()->IsSpreadOpen() )
		{
			GetDialog("Win_PopID")->Show(bShow);
			if (bShow)
			{
				PAUILABEL pEdt = (PAUILABEL)GetDialog("Win_PopID")->GetDlgItem("Txt_1");
				if (m_vecRoleInfo.size()>0&&!(g_pGame->GetGameSession()->IsHasReferrer()))
				{
					GetDialog("Win_PopID")->Show(false);
					pEdt->SetText(_AL(""));
				}
				else if ( m_vecRoleInfo.size()==0 && !(g_pGame->GetGameSession()->IsHasReferrer()) )
				{
					GetDialog("Win_PopID")->GetDlgItem("Lab_1")->Show(true);
					pEdt->SetText(_AL(""));
					pEdt->SetAcceptMouseMessage(true);
				}
				else
				{
					GetDialog("Win_PopID")->GetDlgItem("Lab_1")->Show(false);
					pEdt->SetText(GetStringFromTable(250));
					pEdt->SetAcceptMouseMessage(false);
				}
			}
		}
	}
}

A2DSprite* CECLoginUIMan::CreateSprite(AString path)
{
	float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	float sy = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 768.0f;
	
	A2DSprite* pSprite = new A2DSprite;
	pSprite->SetDynamicTex(true);
	if (!pSprite->Init(g_pGame->GetA3DDevice(), path, 0))
	{
		A3DRELEASE(pSprite);
		a_LogOutput(1, "CECLoginUIMan::CreateSprite, Failed to load file %s !", path);
		return NULL;
	}
	pSprite->SetZPos(1.0f - 1.0f / 65535.0f);
	pSprite->SetScaleX(sx);
	pSprite->SetScaleY(sy);
	return pSprite;
}

int CECLoginUIMan::CreateGfx(AString path)
{
	//float sx = g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 1024.0f;
	float sy = g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 768.0f;
	int idGfx = AfxGetGFXExMan()->Add2DGfx(path);
	if( idGfx == 0 )
	{
		a_LogOutput(1, "CECLoginUIMan::CreateGfx, Failed to load file %s !", path);
		return 0;
	}
	else
	{
		AfxGetGFXExMan()->Set2DGfxScale(idGfx, sy);
		AfxGetGFXExMan()->Set2DGfxPos(idGfx, (float)g_pGame->GetConfigs()->GetSystemSettings().iRndWidth / 2, (float)g_pGame->GetConfigs()->GetSystemSettings().iRndHeight / 2);
		AfxGetGFXExMan()->Show2DGfx(idGfx, false);
	}
	return idGfx;
}
