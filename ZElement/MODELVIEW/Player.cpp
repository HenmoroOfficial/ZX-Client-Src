/*
 * FILE: Player.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: 
 *
 * HISTORY:  2010.8  Updated by MaYuanzheng
 *
 * Copyright (c) 2010 ZhuXian Studio, All Rights Reserved.
 */

#include "Global.h"
#include "ModelView.h"
#include "Player.h"
#include "a3d.h"
#include "EC_Model.h"
#include "Render.h"
#include "Box3D.h"
#include "AFileImage.h" 
#include "AMImmWrapper.h"
#include "AMEngine.h"
#include "AMConfig.h"
#include "MODELVIEWSkill.h"

//modified by MaYuanzheng 2010.8.17
#include "A3DCombinedAction.h" //add a head file
//MaYuanzheng's modify end
#include "AMSoundBufferMan.h"

#include "..\ZElementClient\EC_RoleTypes.h"
#include "..\ZElementData\DataPathMan.h"
#include "MODELVIEWDataMan.h"
#include "Utility.h"


#define new A_DEBUG_NEW

//#define HEAD_INDEX_NUM  7//头型和发型的数量 see EC_Player.cpp(92):#define HEAD_INDEX_NUM  7


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

CPlayer* g_CPlayer = NULL;

static const char* _weapon_hanger		= "Weapon";
static const char* _right_weapon_hanger = "RightWeapon";
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


const char* _head_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\形象\\男\\头\\男头%02d.ski",	"Models\\Players\\形象\\女\\头\\女头%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头\\小体型男头%02d.ski",	"Models\\Players\\形象\\小体型女\\头\\小体型女头%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头\\中体型男头%02d.ski",	"Models\\Players\\形象\\中体型女\\头\\中体型女头%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头\\大体型男头%02d.ski",	"Models\\Players\\形象\\大体型女\\头\\大体型女头%02d.ski"},
};

const char* _hair_skin[PHYSIQUE_NUM][2] =
{
	{"Models\\Players\\形象\\男\\头发\\男头发%02d.ski",	"Models\\Players\\形象\\女\\头发\\女头发%02d.ski"},
	{"Models\\Players\\形象\\小体型男\\头发\\小体型男头发%02d.ski",	"Models\\Players\\形象\\小体型女\\头发\\小体型女头发%02d.ski"},
	{"Models\\Players\\形象\\中体型男\\头发\\中体型男头发%02d.ski",	"Models\\Players\\形象\\中体型女\\头发\\中体型女头发%02d.ski"},
	{"Models\\Players\\形象\\大体型男\\头发\\大体型男头发%02d.ski",	"Models\\Players\\形象\\大体型女\\头发\\大体型女头发%02d.ski"},
};

static const char* aModelFiles[PHYSIQUE_NUM][2] =
{
	{"Models\\Players\\形象\\男\\躯干\\男.ecm",		"Models\\Players\\形象\\女\\躯干\\女.ecm"},
	{"Models\\Players\\形象\\小体型男\\躯干\\小体型男.ecm",		"Models\\Players\\形象\\小体型女\\躯干\\小体型女.ecm"},	
	{"Models\\Players\\形象\\中体型男\\躯干\\中体型男.ecm",		"Models\\Players\\形象\\中体型女\\躯干\\中体型女.ecm"},	
	{"Models\\Players\\形象\\大体型男\\躯干\\大体型男.ecm",		"Models\\Players\\形象\\大体型女\\躯干\\大体型女.ecm"},	
};

static const char* _equipment_default_body_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服男.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服女.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s男.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服%s女.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华男.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服天华女.ski" },
	{"Models\\Players\\装备\\默认\\默认衣服\\默认衣服九黎男.ski",	"Models\\Players\\装备\\默认\\默认衣服\\默认衣服九黎女.ski" },
};

static const char* _equipment_default_shoe_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子男.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子女.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s男.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子%s女.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华男.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子天华女.ski"},
	{"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子九黎男.ski",	"Models\\Players\\装备\\默认\\默认鞋子\\默认鞋子九黎女.ski"},
};

static const char* _equipment_default_fashion_upper_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣男.ski","Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣%s男.ski","Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣%s女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣天华男.ski","Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣天华女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣九黎男.ski","Models\\Players\\装备\\默认\\默认时装上衣\\默认时装上衣九黎女.ski"},
};

static const char* _equipment_default_fashion_lower_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣男.ski",	"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣%s男.ski",	"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣%s女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣天华男.ski",	"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣天华女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣九黎男.ski",	"Models\\Players\\装备\\默认\\默认时装下衣\\默认时装下衣九黎女.ski"},
};

static const char* _equipment_default_fashion_shoe_skin[PHYSIQUE_NUM][2] = 
{
	{"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子男.ski",	"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子%s男.ski",	"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子%s女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子天华男.ski",	"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子天华女.ski"},
	{"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子九黎男.ski",	"Models\\Players\\装备\\默认\\默认时装鞋子\\默认时装鞋子九黎女.ski"},
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
};


static abase::vector<int> _male_eye_joint_indice;
static abase::vector<int> _female_eye_joint_indice;

static const char* _str_part[NUM_SKIN_INDEX] = 
{
	"[头]      ",
	"[头发]    ",
	"[普通衣服]",
	"[普通鞋子]",
	"[普通头盔]",
	//"[眼镜]    ",
	//"[鼻子]    ",
	//"[胡子]    ",
	//"[披风]    ",
	"[时装上衣]",
	//"[时装裤子]",
	"[时装鞋子]",
	"[时装头饰]"
};

static bool bLast = !g_bPause;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CPlayer::CPlayer(CAnimationBar* pABar,CCharactorBar *pCBar)
{
	m_pPlayerModel = 0;		//	Player model
	m_pChangedModel = 0;		//	Changed model
	m_pPetModel = 0;		//	Pet model
	m_pAnimationBar = pABar;
	m_pCharactorBar = pCBar;
	m_pLWeaponModel = 0;
	m_pRWeaponModel = 0;
	for( int i = 0; i < NUM_SKIN_INDEX; ++i)
		m_dwSkinSet[i] = 0;
	m_dwWeapon = 0;
	m_dwRide   = 0;
	m_nObjectType = RENDERABLE_OBJECT_PLAYER;
	m_nShowType = show_both;

	m_nGender = 0;
	m_iPhysique = 0;
	m_iSect = 0;
	m_iEarid = 0;
	m_iTailid = 0;
	m_iWeapon = WEAPON_NULL;
	m_bChanged = false;
}


CPlayer::~CPlayer()
{

}

void CPlayer::Release()
{
	if(m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride,false);
		m_pPetModel->Release();
		delete m_pPetModel;
		m_pPetModel = 0;
	}
	
	if(m_pPlayerModel)
	{
		//int cnum = m_pPlayerModel->GetChildCount();
		//for( int i = 0 ; i < cnum; i++)
		//m_pPlayerModel->RemoveChildModel(_weapon_hanger);//m_pPlayerModel->GetHangerName(i));
		//m_pPlayerModel->StopAllActions();
		g_bPause = false;
		g_bStop = true;
		m_pPlayerModel->Release();
		delete m_pPlayerModel;
		m_pPlayerModel = 0;
	}

	if(m_pChangedModel)
	{
		g_bPause = false;
		g_bStop = true;
		m_pChangedModel->Release();
		delete m_pChangedModel;
		m_pChangedModel = 0;

	}
	CRenderableObject::Release();
}

void CPlayer::Tick(DWORD dwTimeDelta)
{
	if(m_pPlayerModel)
	{
		bool bStop = m_pPlayerModel->IsActionStopped(ACT_CHANNEL_BODY);
		if (m_bChanged && m_pChangedModel) {
			bStop = !(m_pChangedModel->IsPlayingComAct());
		}


		//bool bAct = m_pPlayerModel->IsPlayingComAct();
		//bool isAtcStop = m_pPlayerModel->IsActionStopped(ACT_CHANNEL_BODY);
//		if (!bStatus&&g_pCurComAct&&!(g_pCurComAct->IsActionStopped())) {
//			int x = 1;
//			A3DCombinedAction* pComAct = g_pCurComAct->GetComAct();
//			pComAct->
//			g_pCurComAct->Stop(true,true);
//		}
		//A3DCombActDynData* pCurComAct = m_pPlayerModel->GetCurComAct();
		//if(!bStatus&&pCurComAct&&!pCurComAct->IsActionStopped())
		
			//pCurComAct->Stop(true,true);
		
		//if(!bStop && !g_bLoop) g_bPause = true;

		if(bStop)
		{
			g_bPause = false;
			if(m_pAnimationBar->IsPauseButtonEnabled())m_pAnimationBar->DisablePauseButton();//暂停按钮无效化
		}
		if(!bStop&&!m_pAnimationBar->IsPauseButtonEnabled())
		{
			m_pAnimationBar->EnablePauseButton();
		}
		if(bStop != g_bStop)
		{
			m_pAnimationBar->UpdateStopButton(bStop);
			g_bStop = bStop;
		}

		if(g_bPause) dwTimeDelta = 0;
		if(g_bLoop && bStop) PlayAction(m_strActionName);
		if(m_pPetModel) m_pPetModel->Tick(dwTimeDelta);
		else {
			if(m_bChanged && m_pChangedModel) m_pChangedModel->Tick(dwTimeDelta);
			else m_pPlayerModel->Tick(dwTimeDelta);
		}
	}
}

void CPlayer::Render(A3DViewport *pA3dViewPort)
{
	A3DLIGHTPARAM lightParams = g_Render.GetDirLight()->GetLightparam();
	A3DSkinModel::LIGHTINFO LightInfo;
	LightInfo.colAmbient	= g_Render.GetA3DDevice()->GetAmbientColor();
	LightInfo.vLightDir		= lightParams.Direction;//Direction of directional light
	LightInfo.colDirDiff	= lightParams.Diffuse;//Directional light's diffuse color
	LightInfo.colDirSpec	= lightParams.Specular;//	Directional light's specular color
	LightInfo.bPtLight		= false;//	false, disable dynamic point light,
	

	CECModel *pModel = m_pPlayerModel;
	if(pModel && m_pPetModel) pModel = m_pPetModel;
	if(pModel && m_bChanged)  pModel = m_pChangedModel;

	if(pModel) 
	{
		A3DMATRIX4 matRotate,matX,matY,matZ;
		matX.RotateX(m_vRotate.x);
		matY.RotateY(m_vRotate.y);
		matZ.RotateZ(m_vRotate.z);
		matRotate = matZ * matY * matX;
		A3DVECTOR3 vDir = g_vAxisZ;
		A3DVECTOR3 vUp = g_vAxisY;
		vDir = matRotate * vDir;
		vUp = matRotate * vUp;
		pModel->GetA3DSkinModel()->SetDirAndUp(vDir,vUp);
		
		CBox3D renderBox;
		renderBox.Init(&g_Render,1.0f,false);
		A3DAABB aabb = pModel->GetA3DSkinModel()->GetModelAABB();
		renderBox.SetSize(aabb.Extents.x,aabb.Extents.y,aabb.Extents.z);
		renderBox.SetPos(aabb.Center);
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
		if(g_bShowAABB)
			renderBox.Render();
		//player
		if(g_bFashion)
		{
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_UPPER_BODY_INDEX,true);
			//m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_LOWER_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_FOOT_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_HEADWEAR_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_EAR_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_TAIL_INDEX,true);
			
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_BODY_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FOOT_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HELM_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_EAR_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_TAIL_INDEX,false);
			
			A3DSkin *pSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_FASHION_HEADWEAR_INDEX);
			if(pSkin) m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX,false);
			else m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX,true);
			
			//if(m_nShowType == show_upper_with_lower)
				//m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_LOWER_INDEX,false);
		}else
		{
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_UPPER_BODY_INDEX,false);
			//m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_LOWER_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_FOOT_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_HEADWEAR_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_EAR_INDEX,false);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FASHION_TAIL_INDEX,false);

			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_BODY_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_FOOT_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HELM_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_EAR_INDEX,true);
			m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_TAIL_INDEX,true);

			A3DSkin *pSkin = m_pPlayerModel->GetA3DSkinModel()->GetA3DSkin(SKIN_HELM_INDEX);
			if(pSkin) m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX,false);
			else m_pPlayerModel->GetA3DSkinModel()->ShowSkin(SKIN_HAIR_INDEX,true);
		}	
		
		pModel->GetA3DSkinModel()->SetLightInfo(LightInfo);
		pModel->Render(pA3dViewPort);		
	}
}

A3DVECTOR3 CPlayer::GetDefaultCamaraPos()
{
	CECModel *pModel = m_pPlayerModel;
	if(m_pPetModel) pModel = m_pPetModel;
	if(m_bChanged && m_pChangedModel) pModel = m_pChangedModel; 
	
	if(pModel)
	{
		float fRadius = 0.0f;
		A3DAABB aabb = pModel->GetA3DSkinModel()->GetModelAABB();
		if(aabb.Extents.x > fRadius) fRadius = aabb.Extents.x;
		if(aabb.Extents.y > fRadius) fRadius = aabb.Extents.y;
		if(aabb.Extents.z > fRadius) fRadius = aabb.Extents.z;
		
		return A3DVECTOR3(aabb.Center.x,aabb.Center.y,fRadius*5);
	}else return g_vOrigin;
}

void CPlayer::SetDefaultSkin(CString* strSkin)
{
	CString temp;
	
	char szTemp[256];
	char * sSect;
	int  iHairFaceID = 1;
	if(m_iSect == SECT_11)
		sSect = "烈山";
	else if(m_iSect == SECT_13)
	{
		sSect = "怀光";
		iHairFaceID += HEAD_INDEX_NUM;//对默认怀光 头型和发型的修正（8-14）
	}
	if(m_nGender>=0)
	{
		sprintf(szTemp,_head_skin[m_iPhysique][m_nGender],iHairFaceID);
		strSkin[SKIN_HEAD_INDEX] = szTemp;
		sprintf(szTemp,_hair_skin[m_iPhysique][m_nGender],iHairFaceID);
		strSkin[SKIN_HAIR_INDEX] = szTemp;
		sprintf(szTemp,_equipment_default_body_skin[m_iPhysique][m_nGender],sSect);
		strSkin[SKIN_BODY_INDEX] = szTemp;
		sprintf(szTemp,_equipment_default_shoe_skin[m_iPhysique][m_nGender],sSect);
		strSkin[SKIN_FOOT_INDEX] =szTemp;
		sprintf(szTemp,_equipment_default_fashion_upper_skin[m_iPhysique][m_nGender],sSect);
		strSkin[SKIN_FASHION_UPPER_BODY_INDEX] =szTemp;
		//sprintf(szTemp,_equipment_default_fashion_lower_skin[m_iPhysique][m_nGender],sSect);
		//strSkin[SKIN_FASHION_LOWER_INDEX] = szTemp;
		sprintf(szTemp,_equipment_default_fashion_shoe_skin[m_iPhysique][m_nGender],sSect);
		strSkin[SKIN_FASHION_FOOT_INDEX] = szTemp;
	}
	int level = 0;//iLevel / 45;

		if (m_iSect == SECT_11)
		{
			// 耳朵的序号
			//if (m_iEarid & 0x80)
				//sprintf(szTemp, _optimal_ear_skin[m_nGender], (m_iEarid & 0x7f) +1);
			//else
				sprintf(szTemp, _ear_skin[m_nGender], level+1, m_iEarid+1 );
				strSkin[SKIN_EAR_INDEX] = szTemp;
			//A3DSkin* pEarSkin = g_pGame->LoadA3DSkin(szPath, true);
			//pA3DModel->ReplaceSkin(SKIN_EAR_INDEX, pEarSkin, false);
			// 尾巴的序号
			//if (iTailid & 0x80)
				//sprintf(szTemp, _optimal_tail_skin[m_nGender], (m_iTailid & 0x7f)+1);
			//else
				sprintf(szTemp, _tail_skin[m_nGender], level+1, m_iTailid+1 );
				strSkin[SKIN_TAIL_INDEX] = szTemp;
			//A3DSkin* pTailSkin = g_pGame->LoadA3DSkin(szPath, true);
			//pA3DModel->ReplaceSkin(SKIN_TAIL_INDEX, pTailSkin, false);
		}
}

void CPlayer::SaveConfig()
{
	char szStr[256], szSect[128], szID[256];
	
	for( int i = 0; i < NUM_SKIN_INDEX; ++i)
	{
		sprintf(szID,"%d", m_dwSkinSet[i]);
		sprintf(szStr, "%d", i);
		if(m_nGender == 1)
		{
			strcpy(szSect, "Female");
			WritePrivateProfileString(szSect,szStr, szID, g_szIniFile);
		}else if(m_nGender == 0)
		{
			strcpy(szSect, "Male");
			WritePrivateProfileString(szSect,szStr, szID, g_szIniFile);
		}
	}
	
	//weapon
	sprintf(szID,"%d", m_dwWeapon);
	if(m_nGender == 1)
	{
		strcpy(szSect, "Female");
		WritePrivateProfileString(szSect,"weapon", szID, g_szIniFile);
	}else if(m_nGender == 0)
	{
		strcpy(szSect, "Male");
		WritePrivateProfileString(szSect,"weapon", szID, g_szIniFile);
	}

	//ride
	sprintf(szID,"%d", m_dwRide);
	if(m_nGender == 1)
	{
		strcpy(szSect, "Female");
		WritePrivateProfileString(szSect,"ride", szID, g_szIniFile);
	}else if(m_nGender == 0)
	{
		strcpy(szSect, "Male");
		WritePrivateProfileString(szSect,"ride", szID, g_szIniFile);
	}

}

void CPlayer::LoadConfig(CString* strSkin)
{
	char szStr[256], szSect[128],szRes[256];
	for( int i = 0; i < NUM_SKIN_INDEX; ++i)
	{
		memset(szRes,'\0',256);
		sprintf(szStr, "%d", i);
		if(m_nGender == 1)
		{
			strcpy(szSect, "Female");
			GetPrivateProfileString(szSect,szStr, szRes, szRes, 256, g_szIniFile);
		}else if(m_nGender == 0)
		{
			strcpy(szSect, "Male");
			GetPrivateProfileString(szSect,szStr, szRes, szRes, 256, g_szIniFile);
		}
		CString temp(szRes);
		if( !temp.IsEmpty() && temp!="0") 
		{
			DWORD id = (DWORD)atof(temp);
			
			//对头和头发的特殊处理
			char tempPath[128];
			if(i==SKIN_HEAD_INDEX)
			{
				sprintf(tempPath, _head_skin[m_iPhysique][m_nGender], id);
				strSkin[i] = tempPath;
				m_dwSkinSet[i] = id;
			}else if( i== SKIN_HAIR_INDEX)
			{
				sprintf(tempPath, _hair_skin[m_iPhysique][m_nGender], id);
				strSkin[i] = tempPath;
				m_dwSkinSet[i] = id;
			}else 
			{
				strSkin[i] = GetPathByID(id);
				m_dwSkinSet[i] = id;
			}
		}
	}
	//weapon
	memset(szRes,'\0',256);
	if(m_nGender == 1)
	{
		strcpy(szSect, "Female");
		GetPrivateProfileString(szSect,"weapon", szRes, szRes, 256, g_szIniFile);
	}else if(m_nGender == 0)
	{
		strcpy(szSect, "Male");
		GetPrivateProfileString(szSect,"weapon", szRes, szRes, 256, g_szIniFile);
	}
	CString temp(szRes);
	if( !temp.IsEmpty()) 
	{
		m_dwWeapon = (DWORD)atof(temp);
		strSkin[i] = GetPathByID(m_dwWeapon);
		
	}else
	{
		m_dwWeapon = 0;
		strSkin[i] = "---None---";
	}

	//ride
	i++;
	memset(szRes,'\0',256);
	if(m_nGender == 1)
	{
		strcpy(szSect, "Female");
		GetPrivateProfileString(szSect,"ride", szRes, szRes, 256, g_szIniFile);
	}else if(m_nGender == 0)
	{
		strcpy(szSect, "Male");
		GetPrivateProfileString(szSect,"ride", szRes, szRes, 256, g_szIniFile);
	}
	temp = szRes;
	if( !temp.IsEmpty()) 
	{
		m_dwRide = (DWORD)atof(temp);
		strSkin[i] = GetRidePathByID(m_dwRide);
		
	}else
	{
		m_dwRide = 0;
		strSkin[i] = "---None---";
	}

}

CString CPlayer::GetPathByID(DWORD id)
{
	DATA_TYPE type;
	EQUIPMENT_ESSENCE* pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(id,ID_SPACE_ESSENCE,type);
	if(pData)
	{
		if(pData->equip_mask & 0x0800 && pData->equip_type == 3  && (pData->equip_location==2 || pData->equip_location==4))
		{//时装上衣
			m_nShowType = GetShowPart(pData->equip_location); 
		}
		if(m_nGender == 1)

			return g_DataPathMan.GetPathByID(pData->file_model_female);
		else return g_DataPathMan.GetPathByID(pData->file_model_male);

	}else return "";
}

CString CPlayer::GetRidePathByID(DWORD id)
{
	DATA_TYPE type;
	VEHICLE_ESSENCE* pData = (VEHICLE_ESSENCE*)g_DataMan.get_data_ptr(id,ID_SPACE_ESSENCE,type);
	if(pData)
	{
		return g_DataPathMan.GetPathByID(pData->file_model);
	}else return "";
}

void CPlayer::GetPartNameArray(CString *strSkinName)
{
	//NUM_SKIN_INDEX + 1 (1 weapon) 
	DATA_TYPE type;
	EQUIPMENT_ESSENCE* pData = 0;
	
	for( int i = 0; i < NUM_SKIN_INDEX; i++)
	{
		if(m_dwSkinSet[i]==0)
		{
			strSkinName[i] = "---None---";
			continue;
		}
		if(i == SKIN_HEAD_INDEX)
		{
			strSkinName[i].Format("头%d",m_dwSkinSet[i]);
			continue;
		}else if( i == SKIN_HAIR_INDEX)
		{
			strSkinName[i].Format("头发%d",m_dwSkinSet[i]);
			continue;
		}else if( i == SKIN_EAR_INDEX)
		{
			strSkinName[i].Format("耳朵%d",m_dwSkinSet[i]);
			continue;
		}else if( i == SKIN_TAIL_INDEX)
		{
			strSkinName[i].Format("尾巴%d",m_dwSkinSet[i]);
			continue;
		}else if( i == SKIN_FASHION_EAR_INDEX)
		{
			strSkinName[i].Format("时装耳朵%d",m_dwSkinSet[i]);
			continue;
		}else if( i == SKIN_FASHION_TAIL_INDEX)
		{
			strSkinName[i].Format("时装尾巴%d",m_dwSkinSet[i]);
			continue;
		}
		
		pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(	m_dwSkinSet[i],ID_SPACE_ESSENCE,type);
		if(pData)
			strSkinName[i] = WC2AS(pData->name);
		else strSkinName[i] = "---None---";
	}

	if(m_dwWeapon==0) strSkinName[i] = "---None---";
	else 
	{
		EQUIPMENT_ESSENCE* pData = (EQUIPMENT_ESSENCE*)g_DataMan.get_data_ptr(	m_dwWeapon,ID_SPACE_ESSENCE,type);
		strSkinName[i] = WC2AS(pData->name);
	}

	//i+1 ride
	if(m_dwRide==0) strSkinName[i+1] = "---None---";
	else
	{
		VEHICLE_ESSENCE* pData2 = (VEHICLE_ESSENCE*)g_DataMan.get_data_ptr(	m_dwRide,ID_SPACE_ESSENCE,type);
		strSkinName[i+1] = WC2AS(pData2->name);
	}
}

bool CPlayer::Load(int sect, int iFemail)
{
	//m_pPetModel = NULL;
	m_nGender = iFemail;
	m_iPhysique = GetPhysiqueBySect(sect);
	m_iSect = sect;
	
	if(m_pPlayerModel)
	{
		m_pPlayerModel->Release();
	}else
	{
		m_pPlayerModel = new CECModel;
		//m_pPlayerModel->LoadBoneScaleInfo("Configs\\angelica.cfg");
	}
	//m_pPlayerModel->Reset();


	if(m_pPlayerModel==0)return false;

	//读配置文件
	CString strSkinPathArray[NUM_SKIN_INDEX + 2];
	SetDefaultSkin(strSkinPathArray);
//	LoadConfig(strPathArray);
	g_Render.GetA3DEngine()->GetAMEngine()->GetAMConfig()->SetSoundQuality(AMSOUND_QUALITY_MEDIUM);
	g_Render.GetA3DEngine()->GetAMEngine()->GetAMConfig()->SetRunEnv(AMRUNENV_GFXEDITOR);

	const char* szFile = aModelFiles[m_iPhysique][m_nGender];
	bool bResult = m_pPlayerModel->Load(szFile,true, A3DSkinModel::LSF_NOSKIN, /*true*/false);


	if(!bResult)
	{
		g_Log.Log("CPlayer::Load(), Failed load %s", aModelFiles[m_iPhysique][m_nGender]);
		delete m_pPlayerModel;
		m_pPlayerModel = NULL;
		return false;
	}

	//clear all child models
	int cnum = m_pPlayerModel->GetChildCount();
	for( int i = 0 ; i < cnum; i++)
		m_pPlayerModel->RemoveChildModel(m_pPlayerModel->GetHangerName(i));
	
	A3DSkinModel* pA3DModel = m_pPlayerModel->GetA3DSkinModel();
	if(!pA3DModel)
	{
		g_Log.Log("CPlayer::Load(), Failed to get skin model!");
		return false;
	}
	
	//load no skin
	for (i = 0; i < NUM_SKIN_INDEX; i++)
	{
		pA3DModel->AddSkin(NULL, false);
	}

	//replace skin
	for( i = 0; i < NUM_SKIN_INDEX; i++)
	{
		if(!strSkinPathArray[i].IsEmpty())
		{
			g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
			A3DSkin* pSkin = LoadA3DSkin(strSkinPathArray[i]);
			if(pSkin==0) 
			{
				g_Log.Log("CPlayer::Load(), Failed to load skin %s!",strSkinPathArray[i]);
			}
			pA3DModel->ReplaceSkin(i, pSkin, false);
			g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
		}
	}


	//build action channels
	BuildActionList();
	A3DSMActionChannel* pChannel;


	if(m_nGender == 0){
		pChannel = pA3DModel->BuildActionChannel(ACT_CHANNEL_BODY,_male_eye_joint_indice.size(),
		_male_eye_joint_indice.begin(),
		true);
	}else if(m_nGender == 1){
		pChannel = pA3DModel->BuildActionChannel(ACT_CHANNEL_BODY,_female_eye_joint_indice.size(),
		_female_eye_joint_indice.begin(),
		true);
	}

	if(!pChannel)
	{
		g_Log.Log("CPlayer::Load(),Failed to Build action channel");
		return false;
	}
	pChannel->SetWeight(1.0f);


	m_pPlayerModel->GetA3DSkinModel()->SetAutoAABBType(A3DSkinModel::AUTOAABB_INITMESH);
	
	//set camera position
	if(g_bUseDefaultCamPos) m_pPlayerModel->SetPos(A3DVECTOR3(0,0,0));
	else 
	{
		m_pPlayerModel->SetPos(g_vLPos);
		m_vRotate = g_vLR;
	}
	

	//weapon
	if(!strSkinPathArray[i].IsEmpty())
	{
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		m_pPlayerModel->RemoveChildModel(_weapon_hanger);
		m_pPlayerModel->AddChildModel(
			_weapon_hanger,
			false,
			"HH_weapon",
			strSkinPathArray[i],
			"CC_weapon");
		
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}

	//ride
	i++;
	if(!strSkinPathArray[i].IsEmpty())
	{
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		
		if(m_pPetModel){
			delete m_pPetModel;
			m_pPetModel=0;
		}
		m_pPetModel = new CECModel;

		if(m_pPetModel)
		{
			if(!m_pPetModel->Load(strSkinPathArray[i]))
			{
				delete m_pPetModel;
				m_pPetModel = NULL;
			}

			m_pPetModel->AddChildModel(
				_hanger_ride,
				false,
				_hh_ride,
				m_pPlayerModel,
				_cc_ride);

				A3DCombinedAction *pAct = m_pPetModel->GetComActByIndex(0);
				AString strActName = pAct->GetName();
				//m_pPetModel->PlayActionByName(ACT_CHANNEL_BODY,strActName,1.0f,true, 200,false,-1);

		}
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}

		RecreateActionList();
		RecreateSkillList();
		RecreateShapeList();
		
		m_pAnimationBar->UpdateStopButton(g_bStop);

	//更新角色界面
	CString strTempSkinArray[NUM_SKIN_INDEX + 2];
	GetPartNameArray(strTempSkinArray);
	if(m_pCharactorBar) m_pCharactorBar->UpdatePart(strTempSkinArray);
	


	return	true;
}


bool CPlayer::LoadChangedModel(int id){
	if(m_pChangedModel){
		m_pChangedModel->Release();
		delete m_pChangedModel;
		m_pChangedModel = 0;
	}
	m_pChangedModel = new CECModel;
	if(m_pChangedModel == 0)
		return false;
	const char* szFile =  GetChangedModelFileName(id,m_nGender,m_iSect);
	bool bResult = m_pChangedModel->Load(szFile);
	
	if(!bResult)
	{
		g_Log.Log("CPlayer::LoadShapedModel(), Failed load %s", szFile);
		delete m_pChangedModel;
		m_pChangedModel = NULL;
		return false;
	}
	return true;
}

void CPlayer::RecreateActionList()
{
	if(m_pAnimationBar)
	{
		m_pAnimationBar->ClearActions();
		AString strActName = "无动作";
		m_pAnimationBar->AddAction(CString(strActName));//
		if(m_bChanged && m_pChangedModel){
			int n = m_pChangedModel->GetComActCount();
			for(int i = 0;i<n;i++){
				A3DCombinedAction *pAct = m_pChangedModel->GetComActByIndex(i);
				strActName = pAct->GetName();
				m_pAnimationBar->AddAction(CString(strActName));
			}
			
		}
		else if (m_pPlayerModel) {
			int n = m_pPlayerModel->GetComActCount();
			for( int i = 0; i < n; i ++ )
			{
				A3DCombinedAction *pAct = m_pPlayerModel->GetComActByIndex(i);
				strActName = pAct->GetName();
				if(m_dwRide > 0)
				{
					if(m_iSect == SECT_11 && (strActName.Find(_T("怀光"))!=-1 || strActName.Find(_T("镰"))!=-1))
						continue; //烈山模型筛掉怀光动作
					else if(m_iSect == SECT_13 && (strActName.Find(_T("狐妖"))!=-1 || strActName.Find(_T("弓"))!=-1))
						continue;//怀光模型筛掉烈山动作
					if(m_dwWeapon>0 && strActName.Find(_T("空"))!=-1)
						continue;//拿武器时筛掉空手动作
					if(strActName.Find(_T("骑"))!=-1) 
						m_pAnimationBar->AddAction(CString(strActName));
				}
				else{
					if(m_iSect == SECT_11 && (strActName.Find(_T("怀光"))!=-1 || strActName.Find(_T("镰"))!=-1))
						continue; 
					else if(m_iSect == SECT_13 && (strActName.Find(_T("狐妖"))!=-1 || strActName.Find(_T("弓"))!=-1))
						continue;
					//烈山武器，变为箭盒，背身后
					if(m_dwWeapon>0 && strActName.Find(_T("空"))!=-1)
						continue;
					m_pAnimationBar->AddAction(CString(strActName));
				}
			}
		}
			m_pAnimationBar->SetDefaultSel();
			AString strAction = AString(m_pAnimationBar->GetSelAction());
			if(m_pPetModel)PlayAction(strAction,g_bLoop);//换骑乘时，自动播放动作
	}
}

void CPlayer::RecreateShapeList()
{
	if(m_pCharactorBar){
		m_pCharactorBar->ClearShapes();
		m_pCharactorBar->GetComboShape()->AddString("0普通");
		m_pCharactorBar->GetComboShape()->SetItemData(0,0);
		switch(m_iSect) {
		case SECT_11:
			m_pCharactorBar->GetComboShape()->AddString("1小狐狸");
			m_pCharactorBar->GetComboShape()->SetItemData(1,4);
			m_pCharactorBar->GetComboShape()->AddString("2中狐狸");
			m_pCharactorBar->GetComboShape()->SetItemData(2,5);
			m_pCharactorBar->GetComboShape()->AddString("3大狐狸");
			m_pCharactorBar->GetComboShape()->SetItemData(3,6);
			break;
		case SECT_1:
			m_pCharactorBar->GetComboShape()->AddString("1魔魂");
			m_pCharactorBar->GetComboShape()->SetItemData(1,1);
			m_pCharactorBar->GetComboShape()->AddString("2蛊王");
			m_pCharactorBar->GetComboShape()->SetItemData(2,2);
			break;
		default:
			break;
		}
		m_pCharactorBar->SetDefaultShape();
	}
}

bool CPlayer::ReplaceFixedSkin(int nPart,int nGender,int nSkin,int iLevelPhrase)
{
	if(m_pPlayerModel==0) return false;
	
	A3DSkinModel* pA3DModel = m_pPlayerModel->GetA3DSkinModel();
	if(!pA3DModel)
	{
		g_Log.Log("CPlayer::ReplaceFixedSkin(), Failed to get skin model!");
		return false;
	}

	char tempPath[MAX_PATH];
	int iHairFaceID = nSkin;

	//怀光的头型和脸型修正 8-14
	if(m_iSect == SECT_13){
		//iHairFaceID %= HEAD_INDEX_NUM;
		iHairFaceID += HEAD_INDEX_NUM;
	}

	if(nPart==SKIN_HEAD_INDEX)
	{
		sprintf(tempPath, _head_skin[m_iPhysique][nGender], iHairFaceID);
	}else if( nPart== SKIN_HAIR_INDEX)
	{
		sprintf(tempPath, _hair_skin[m_iPhysique][nGender], iHairFaceID);
	}else if(nPart== SKIN_EAR_INDEX)
	{
		sprintf(tempPath,_ear_skin[nGender],iLevelPhrase,nSkin%10);
	}else if(nPart== SKIN_TAIL_INDEX)
	{
		sprintf(tempPath,_tail_skin[nGender],iLevelPhrase,nSkin%10);
	}else if(nPart== SKIN_FASHION_EAR_INDEX)
	{
		sprintf(tempPath,_optimal_ear_skin[nGender],nSkin%10);
	}else if(nPart== SKIN_FASHION_TAIL_INDEX)
	{
		sprintf(tempPath,_optimal_tail_skin[nGender],nSkin%10);
	}else return false;

	A3DSkin* pSkin = LoadA3DSkin(tempPath);
	if(pSkin==0) 
	{
		g_Log.Log("CPlayer::ReplaceSkin(), Failed to load %s!",tempPath);
		return false;
	}
	m_dwSkinSet[nPart] = nSkin;	
	pA3DModel->ReplaceSkin(nPart, pSkin, false);
	
	//更新角色界面
	CString strTempSkinArray[NUM_SKIN_INDEX + 2];
	GetPartNameArray(strTempSkinArray);
	if(m_pCharactorBar) m_pCharactorBar->UpdatePart(strTempSkinArray);
	return true;
}

int  CPlayer::GetShowPart(DWORD part)
{
	if(part==4) return show_upper_with_lower;
	else return show_both;
}

bool CPlayer::ReplaceSkin(int nPart, DWORD ptr)
{
	if(m_pPlayerModel==0) return false;
	if(g_CPlayer==0) return false;
	EQUIPMENT_ESSENCE *pData = (EQUIPMENT_ESSENCE *)ptr;
	int pathID = 0;
	if(pData!=0)
	{
		if(m_nGender==1)
		{
			switch(g_CPlayer->GetSect()) {
			case SECT_1:
				pathID = pData->file_model_female;
				break;
			case SECT_10:
				pathID = pData->file_model_big_female;
				break;
			case SECT_11:
			case SECT_13:
				pathID = pData->file_model_small_female;
				break;
			case SECT_15:
				pathID = pData->file_model_female;
				break;
			default:
				pathID = pData->file_model_female;
			}
		}else if(m_nGender == 0){
			switch(g_CPlayer->GetSect()) {
			case SECT_1:
				pathID = pData->file_model_male;
				break;
			case SECT_10:
				pathID = pData->file_model_big_male;
				break;
			case SECT_11:
			case SECT_13:
				pathID = pData->file_model_small_male;
				break;
			case SECT_15:
				pathID = pData->file_model_male;
				break;
			default:
				pathID = pData->file_model_male;
			}
		}
		
		if(pData->equip_mask & 0x0800 && pData->equip_type == 3 && (pData->equip_location==2 || pData->equip_location==4))
		{//时装上衣
			m_nShowType = GetShowPart(pData->equip_location); 
		}
	}else pathID = 0;
	const char *szPath = g_DataPathMan.GetPathByID(pathID);
	const char* path = szPath;
	CString Temp = szPath;
	if(Temp.IsEmpty())
	{//替换成默认路径
		path = NULL;
		switch(nPart) 
		{
		case SKIN_BODY_INDEX:
			path = _equipment_default_body_skin[m_iPhysique][m_nGender];
			break;
		case SKIN_FOOT_INDEX:
			path = _equipment_default_shoe_skin[m_iPhysique][m_nGender];
			break;
		case SKIN_FASHION_UPPER_BODY_INDEX:
			m_nShowType = show_both;
			path = _equipment_default_fashion_upper_skin[m_iPhysique][m_nGender];
			break;
		//case SKIN_FASHION_LOWER_INDEX:
			//path = _equipment_default_fashion_lower_skin[m_iPhysique][m_nGender];
			//break;
		case SKIN_FASHION_FOOT_INDEX:
			path = _equipment_default_fashion_shoe_skin[m_iPhysique][m_nGender];
			break;
		}
	}
	
	A3DSkinModel* pA3DModel = m_pPlayerModel->GetA3DSkinModel();
	if(!pA3DModel)
	{
		g_Log.Log("CPlayer::ReplaceSkin(), Failed to get skin model!");
		return false;
	}

	g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile((const char*)glb_ReplaceShader, A3DSkinMan::SHADERREPLACE_USERDEFINE);
	if(path!=NULL)
	{
		A3DSkin* pSkin = LoadA3DSkin(path);
		if(pSkin==0) 
		{
			g_Log.Log("CPlayer::ReplaceSkin(), Failed to load %s!",szPath);
			return false;
		}
		if(pData) m_dwSkinSet[nPart] = pData->id;	
		else m_dwSkinSet[nPart] = 0;
		pA3DModel->ReplaceSkin(nPart, pSkin, false);
	}else 
	{
		m_dwSkinSet[nPart] = 0;
		pA3DModel->ReplaceSkin(nPart, 0, false);
	}
	g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	
	//更新角色界面
	CString strTempSkinArray[NUM_SKIN_INDEX + 2];
	GetPartNameArray(strTempSkinArray);
	if(m_pCharactorBar) m_pCharactorBar->UpdatePart(strTempSkinArray);

	return true;
}

bool CPlayer::MountModel(const char* pos,DWORD ptr)
{
	if(stricmp(pos,"Weapon")==0) MountWeapon(pos,ptr);
	if(stricmp(pos,"Rider")==0) MountRide(pos,ptr);
	return true;
}

void CPlayer::MountWeapon(const char*pos, DWORD ptr)
{
	EQUIPMENT_ESSENCE *pData = (EQUIPMENT_ESSENCE *)ptr;
	int pathID = 0;
	const char* szPath,*szOtherhandPath;
	if(pData!=0&&m_pPlayerModel!=0)
	{
		if(m_nGender == 1)
		{
			switch(pData->sect_mask) {
			case (1<<NUM_HUMAN_SECT)-1:
			case (1<<SECT_15):
				pathID = pData->file_model_female;
				break;
			case 1<<SECT_10:
				pathID = pData->file_model_big_female;
				break;
			case 1<<SECT_11:
			case 1<<SECT_13:
				pathID = pData->file_model_small_female;
				break;
			default:
				pathID = pData->file_model_female;
				break;
			}
		}
		else if(m_nGender==0)
		{
			switch(pData->sect_mask) {
			case (1<<NUM_HUMAN_SECT)-1:
			case (1<<SECT_15):
				pathID = pData->file_model_male;
				break;
			case 1<<SECT_10:
				pathID = pData->file_model_big_male;
				break;
			case 1<<SECT_11:
			case 1<<SECT_13:
				pathID = pData->file_model_small_male;
				break;
			default:
				pathID = pData->file_model_male;
				break;
			}
		}//pathID = pData->file_model_male;
	}else pathID = 0;
	szPath = g_DataPathMan.GetPathByID(pathID);
	szOtherhandPath = "";
	if(pData&&pData->action_type == WEAPON_SICKLE){
		szOtherhandPath = g_DataPathMan.GetPathByID(pathID);
	}
	m_pPlayerModel->RemoveChildModel(pos);
	CString Temp = szPath;
	if(!Temp.IsEmpty())
	{
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		if(	stricmp(_weapon_hanger,pos)==0)
		{
			m_pPlayerModel->AddChildModel(
				_weapon_hanger,
				false,
				"HH_weapon",
				szPath,
				"CC_weapon");
			if(szOtherhandPath){
				m_pPlayerModel->AddChildModel(
					_right_weapon_hanger,
					false,
					"HH_righthandweapon",
					szOtherhandPath,
				"CC_weapon");
			}
		m_iWeapon = pData->action_type;
		};
		if(	stricmp(_wing,pos)==0){};	
		if(	stricmp(_cc_ride,pos)==0){};			
		if(	stricmp(_cc_ride2,pos)==0){};		
		if(	stricmp(_hh_ride,pos)==0){};				
		if(	stricmp(_hanger_ride,pos)==0){};		
		if(	stricmp(_hanger_ride2,pos)==0){};	
		if(	stricmp(_hanger_hug,pos)==0){};	
		if(	stricmp(_hh_bind,pos)==0){};			
		if(	stricmp(_cc_bind[0],pos)==0){};
		
		m_dwWeapon = pData->id;
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	}else m_dwWeapon = 0;
	RecreateActionList();
	RecreateSkillList();
	//更新角色界面
	CString strTempSkinArray[NUM_SKIN_INDEX + 2];
	GetPartNameArray(strTempSkinArray);
	if(m_pCharactorBar) m_pCharactorBar->UpdatePart(strTempSkinArray);
}

void CPlayer::MountRide(const char*pos, DWORD ptr)
{
	m_dwRide = 0;
	VEHICLE_ESSENCE *pData = (VEHICLE_ESSENCE *)ptr;
	int pathID = 0;
	if(pData!=0)
	{
		pathID = pData->file_model;
	}else pathID = 0;
	const char *szPath = g_DataPathMan.GetPathByID(pathID);
	
	if(m_pPetModel)
	{
		m_pPetModel->RemoveChildModel(_hanger_ride,false);
		m_pPetModel->Release();
		delete m_pPetModel;
		m_pPetModel = NULL;
	}
	CString Temp = szPath;
	if(!Temp.IsEmpty())
	{
		if(	stricmp(_hanger_ride,pos)==0)
		{
			g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
		
			m_pPetModel = new CECModel;
			if(!m_pPetModel->Load(szPath,true,0,false))
			{
				delete m_pPetModel;
				m_pPetModel = NULL;
			}

//			char szSkin[MAX_PATH];
//			strncpy(szSkin, szPath, MAX_PATH);
//			glb_ChangeExtension(szSkin, "ski");
//			A3DSkin * pPetSkin = LoadA3DSkin(szSkin);
//			if( !pPetSkin )
//			{
//				m_pPetModel->Release();
//				delete m_pPetModel;
//				m_pPetModel = NULL;
//			}
			if (m_pPetModel) {
				//m_pPetModel->GetA3DSkinModel()->AddSkin(pPetSkin, true);	
				m_pPetModel->AddChildModel(
					_hanger_ride,
					false,
					_hh_ride,
					m_pPlayerModel,
					_cc_ride);
				m_dwRide = pData->id;
				g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);	
			}
		}
	}
	RecreateActionList();
	RecreateSkillList();
	//更新角色界面
	CString strTempSkinArray[NUM_SKIN_INDEX + 2];
	GetPartNameArray(strTempSkinArray);
	if(m_pCharactorBar) m_pCharactorBar->UpdatePart(strTempSkinArray);
}

void CPlayer::PlayAction(AString strAction, bool bLoop)
{
	if(m_bChanged && m_pChangedModel)
	{
		if(bLoop)
			m_pChangedModel->PlayActionByName(strAction,1.0f,true,200,true,-1);
		else
			m_pChangedModel->PlayActionByName(strAction,1.0f,false);
	}else if(m_pPlayerModel)
	{
		if(m_pPetModel)
		{
			AString temp = strAction;
			int pos = temp.Find("_",0);
			temp = temp.Left(pos);
			AString strActName = FindRideAction(CString(temp));
			m_pPetModel->PlayActionByName(strActName,1.0f,true,200,true,-1);
		}
		if(bLoop){
			m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY,strAction,1.0f,true,200,true,-1);
		}
		
		else{
			m_pPlayerModel->PlayActionByName(ACT_CHANNEL_BODY,strAction,1.0f,false);
		}
	}
	m_strActionName = strAction;
}

CString CPlayer::FindRideAction(CString strAction)
{
	CString strList[] = {
			"站立",
			"休闲",
			"战斗站立",
			"行走",
			"奔跑",
			"起跳",
			"跳跃落地",
			"跳跃循环",
			};
	for( int i = 0; i < 10; ++i)
		if(strList[i] == strAction) return strAction;
	return "战斗站立";
}

void CPlayer::StopAction()
{

	if(m_pPlayerModel){
		//A3DCombActDynData* pCurComAct = m_pPlayerModel->GetCurComAct();
		//m_pPlayerModel->StopAllActions();
		m_pPlayerModel->StopChannelAction(ACT_CHANNEL_BODY,true);
	}
	if(m_pPetModel)
		m_pPetModel->StopAllActions();
	if(m_pChangedModel)
	{
		m_pChangedModel->StopAllActions();
	}
}

A3DMATRIX4 CPlayer::GetAbsoluteTM()
{
	A3DMATRIX4 mat;
	mat.Identity();
	
	CECModel *pModel = m_pPlayerModel;
	if(m_pPetModel) pModel = m_pPetModel;
	if(m_bChanged)  pModel = m_pChangedModel;
	
	if(pModel)
		return pModel->GetA3DSkinModel()->GetAbsoluteTM();
	else return mat;
}

void CPlayer::GetRotateAndPos(A3DVECTOR3& r, A3DVECTOR3& pos)
{
	CECModel *pModel = m_pPlayerModel;
	if(m_pPetModel) pModel = m_pPetModel;
	
	if(pModel)
	{
		pos = pModel->GetA3DSkinModel()->GetPos();
		r = m_vRotate;
	}
}

void CPlayer::GetPathList(abase::vector<AString>& listPath)
{
	AString strRes,temp;
	for( int i = 0; i < NUM_SKIN_INDEX; ++i)
	{
		DWORD id = m_dwSkinSet[i];
		
		//对头和头发的特殊处理
		char tempPath[128];
		if(m_nGender>=0){
			if(i==SKIN_HEAD_INDEX)
			{
				sprintf(tempPath, _head_skin[m_iPhysique][m_nGender], id);
				strRes = tempPath;
				
			}else if( i== SKIN_HAIR_INDEX)
			{
				sprintf(tempPath, _hair_skin[m_iPhysique][m_nGender], id);
				strRes = tempPath;
				
			}
		}else 
		{
			strRes = GetPathByID(id);
		}
		
		if(strRes=="") strRes = "---None---";
		temp = _str_part[i];
		temp +=" : ";
		temp = temp + strRes;
		listPath.push_back(temp);
	}
}

bool CPlayer::PlayAttackAction(const char* szActName, int idSkill)
{
	static unsigned char _att_serial = 1;
	if(m_pPlayerModel)
	{
		
		_att_serial++;
		m_pPlayerModel->PlayAttackAction(ACT_CHANNEL_BODY,szActName,_att_serial, 1, 2, NULL);
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY,1, 2, _att_serial, 0, 100);
		return m_pPlayerModel->IsPlayingComAct();
	}	
	return false;
}

void CPlayer::ReSetPlayer(CAnimationBar* pABar,CCharactorBar *pCBar)
{

}

int CPlayer::GetWeaponID()
{ 
	return m_dwWeapon; 
}

int CPlayer::GetPhysiqueBySect(int sect)
{
	if(sect < NUM_HUMAN_SECT) return PHYSIQUE_NORMAL;
	else if(sect>=SECT_10)
	{
		switch(sect) {
		case SECT_10:
			return PHYSIQUE_BIG;
			break;
		case SECT_11:
		case SECT_13:
			return PHYSIQUE_SMALL;
			break;
		case SECT_15:
			return PHYSIQUE_MIDDLE;
			break;
		default:
			return PHYSIQUE_NORMAL;
		}
	}
	return PHYSIQUE_NORMAL;
}


void CPlayer::BuildActionList()
{
	//	Load action names from file
//	if (!m_ActionNames.IsInitialized())
//	{
//		m_ActionNames.Init("Configs\\actions_player.txt", false);
//	}
//	if (!m_SkillActs.IsInitialized())
//	{
//		m_SkillActs.Init("Configs\\skillacts.txt", false);
//	}

	char szLine[1024];
	DWORD dwReadLen;
	AFileImage file;

	if( !file.Open("configs\\male_eyes_joints.txt", AFILE_OPENEXIST | AFILE_TEXT) )
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

	if( !file.Open("configs\\female_eyes_joints.txt", AFILE_OPENEXIST | AFILE_TEXT) )
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



const char* CPlayer::GetChangedModelFileName(int id, int gender, int sect)
{
	switch(id)
	{
		case 1://魔魂
		{
			if (gender == GENDER_MALE)
				return _changed_shape_path[0];
			else
				return _changed_shape_path[1];
			break;
		}
		case 2://蛊王
		{
			if (gender == GENDER_MALE)
				return _changed_shape_path[2];
			else
				return _changed_shape_path[3];
			break;
		}
		// 烈山变身成狐狸，按修真等级来变身：1、2重变身小狐狸；3、4重变身中狐狸；5重变身大狐狸
		case 4://小狐狸
				return _changed_shape_path[4];
				break;
		case 5://中狐狸
				return _changed_shape_path[5];
				break;
		case 6://大狐狸
				return _changed_shape_path[6];
				break;
		default:
				return NULL;
	}
}


void CPlayer::RecreateSkillList(){
	if(m_pAnimationBar)
	{
		if(m_bChanged && m_pChangedModel){
			//to do
		}else{	
			m_pAnimationBar->ClearSkill();
			GNET::LearnRequirement Info;
			Info.profession = GetHighestProfBySect(m_iSect);
			Info.cultivation = 0x11;//全阵营	
			int iSkillNum = g_ListSkill.size();
			int iCategory = 0;
			iCategory = m_pAnimationBar->GetSkillCategory();
			for( int i = 0; i < iSkillNum; i++)
			{	
				if(GNET::MODELVIEWSkill::CanLearnSkillInSomeCondition(g_ListSkill[i].id,Info,1000,iCategory)==0){
					m_pAnimationBar->AddSkill(g_ListSkill[i].szName,g_ListSkill[i].id);
				}
			}
			m_pAnimationBar->SetSkillDefaultSel();
		}
	}
	
}
int  CPlayer::GetHighestProfBySect(int iSect){
	switch(iSect) {
	case SECT_1:
		return 0;
		break;
	case SECT_2://鬼王宗
		return 14;
		break;
	case SECT_3://合欢
		return 17;
		break;
	case SECT_4://青云
		return 20;
		break;
	case SECT_5://天音
		return 23;
		break;
	case SECT_6://鬼道
		return 29;
		break;
	case SECT_7://焚香
		break;
	case SECT_10:
		return 37;
		break;
	case SECT_11:
		return 43;
		break;
	case SECT_12:
		break;
	case SECT_13:
		return 49;
		break;
	case SECT_14:
		break;
	case SECT_15:
		return 55;
		break;
	default:
		break;
	}

	return 0;
}

///////////////////////////////////////////////////////////////////////////////////////
//NPC
///////////////////////////////////////////////////////////////////////////////////////

CNpc::CNpc(CAnimationBar* pABar,CCharactorBar *pCBar)
{
	m_pAnimationBar = pABar;
	m_pCharactorBar = pCBar;
	m_pNpcModel = 0;
	m_nObjectType = RENDERABLE_OBJECT_NPC;
}

CNpc::~CNpc()
{

}

void CNpc::Tick(DWORD dwTimeDelta)
{
	if(m_pNpcModel) 
	{
		bool bStop = !m_pNpcModel->IsPlayingComAct();
		//if(!bStatus && !g_bLoop) g_bPause = true;
		
//		if(g_bPause != bLast)
//		{
//			m_pAnimationBar->UpdateButton();
//			bLast = g_bPause;
//		}
		if(bStop)
		{
			g_bPause = false;
			if(m_pAnimationBar->IsPauseButtonEnabled())	m_pAnimationBar->DisablePauseButton();//暂停按钮无效化
		}
		if(!bStop&&!m_pAnimationBar->IsPauseButtonEnabled())
		{
			m_pAnimationBar->EnablePauseButton();
		}
		if(bStop != g_bStop)
		{
			m_pAnimationBar->UpdateStopButton(bStop);
			g_bStop = bStop;
		}
		if(g_bPause) dwTimeDelta = 0;
		if(g_bLoop && bStop ) PlayAction(m_strActionName);
		
		m_pNpcModel->Tick(dwTimeDelta);
	}
}
void CNpc::Render(A3DViewport *pA3dViewPort)
{
	if(m_pNpcModel) 
	{
		A3DMATRIX4 matRotate,matX,matY,matZ;
		matX.RotateX(m_vRotate.x);
		matY.RotateY(m_vRotate.y);
		matZ.RotateZ(m_vRotate.z);
		matRotate = matZ * matY * matX;
		A3DVECTOR3 vDir = g_vAxisZ;
		A3DVECTOR3 vUp = g_vAxisY;
		vDir = matRotate * vDir;
		vUp = matRotate * vUp;
		m_pNpcModel->GetA3DSkinModel()->SetDirAndUp(vDir,vUp);

		CBox3D renderBox;
		renderBox.Init(&g_Render,1.0f,false);
		A3DAABB aabb = m_pNpcModel->GetA3DSkinModel()->GetModelAABB();
		renderBox.SetSize(aabb.Extents.x,aabb.Extents.y,aabb.Extents.z);
		renderBox.SetPos(aabb.Center);
		renderBox.SetColor(A3DCOLORRGB(255,0,0));
		if(g_bShowAABB) renderBox.Render();

		A3DLIGHTPARAM lightParams = g_Render.GetDirLight()->GetLightparam();
		A3DSkinModel::LIGHTINFO LightInfo;
		LightInfo.colAmbient	= g_Render.GetA3DDevice()->GetAmbientColor();
		LightInfo.vLightDir		= lightParams.Direction;//Direction of directional light
		LightInfo.colDirDiff	= lightParams.Diffuse;//Directional light's diffuse color
		LightInfo.colDirSpec	= lightParams.Specular;//	Directional light's specular color
		LightInfo.bPtLight		= false;//	false, disable dynamic point light,
		m_pNpcModel->GetA3DSkinModel()->SetLightInfo(LightInfo);
		m_pNpcModel->Render(pA3dViewPort);
	}
}
void CNpc::PlayAction(AString strAction, bool bLoop)
{
	if(m_pNpcModel)
	{
		if(bLoop) m_pNpcModel->PlayActionByName(strAction,1.0f,true,200,false,-1);
		else m_pNpcModel->PlayActionByName(strAction,1.0f);
	}
	m_strActionName = strAction;
}

void CNpc::StopAction()
{
	if(m_pNpcModel)
		m_pNpcModel->StopAllActions();
}

A3DVECTOR3 CNpc::GetDefaultCamaraPos()
{
	if(m_pNpcModel)
	{
		float fRadius = 0.0f;
		A3DAABB aabb = m_pNpcModel->GetA3DSkinModel()->GetModelAABB();
		if(aabb.Extents.x > fRadius) fRadius = aabb.Extents.x;
		if(aabb.Extents.y > fRadius) fRadius = aabb.Extents.y;
		if(aabb.Extents.z > fRadius) fRadius = aabb.Extents.z;

		return A3DVECTOR3(aabb.Center.x,aabb.Center.y,fRadius*5);
	}else return g_vOrigin;
}
void CNpc::Release()
{
	if(m_pNpcModel)
	{
		m_pNpcModel->Release();
		delete  m_pNpcModel;
		m_pNpcModel = 0;
	}
	CRenderableObject::Release();
}

bool CNpc::Load(const char*szPath)
{
	if(m_pNpcModel)
	{
		m_pNpcModel->Release();
	}else
	{
		m_pNpcModel = new CECModel;
	}
	if(!m_pNpcModel) return false;
	g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	bool bResult = m_pNpcModel->Load(szPath);
	if(!bResult)
	{
		g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("",0);
		g_Log.Log("CNpc::Load(), Failed load %s", szPath);
		delete m_pNpcModel;
		m_pNpcModel = NULL;
		return false;
	}
	g_Render.GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);

	m_pNpcModel->GetA3DSkinModel()->SetAutoAABBType(A3DSkinModel::AUTOAABB_INITMESH);
	if(g_bUseDefaultCamPos) m_pNpcModel->SetPos(A3DVECTOR3(0,0,0));
	else 
	{
		m_pNpcModel->SetPos(g_vLPos);
		m_vRotate = g_vLR;
	}
	if(m_pAnimationBar)
	{
		m_pAnimationBar->ClearActions();
		int n = m_pNpcModel->GetComActCount();
		for( int i = 0; i < n; i ++ )
		{
			A3DCombinedAction *pAct = m_pNpcModel->GetComActByIndex(i);
			AString strActName = pAct->GetName();
			m_pAnimationBar->AddAction(CString(strActName));
		}
		m_pAnimationBar->SetDefaultSel();
		AString strAction = AString(m_pAnimationBar->GetSelAction());
		//PlayAction(strAction,g_bLoop);
		m_pAnimationBar->ClearSkill();
	}
	//bLast = m_pNpcModel->IsPlayingComAct();
	return true;
}

A3DMATRIX4 CNpc::GetAbsoluteTM()
{
	A3DMATRIX4 mat;
	mat.Identity();
	if(m_pNpcModel)
		return m_pNpcModel->GetA3DSkinModel()->GetAbsoluteTM();
	else return mat;
}

void CNpc::GetRotateAndPos(A3DVECTOR3& r, A3DVECTOR3& pos)
{
	if(m_pNpcModel)
	{
		pos = m_pNpcModel->GetA3DSkinModel()->GetPos();
		r = m_vRotate;
	}
}

void CNpc::GetPathList(abase::vector<AString>& listPath)
{
	if(m_pNpcModel)
		listPath.push_back(AString(m_pNpcModel->GetFilePath()));
}
