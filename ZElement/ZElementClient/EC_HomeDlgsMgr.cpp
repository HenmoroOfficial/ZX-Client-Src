/*
 * FILE: EC_HomeDlgsMgr.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */
#include "EC_HomeDlgsMgr.h"


#include "aui/AUIManager.h"
#include "aui/AUIStillImageButton.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrainAction.h"
#include "render.h"
#include "A3DViewport.h"
#include "A3DCamera.h"
#include "A3DOrthoCamera.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#include "DlgAutoBuilding.h"
#include "DlgAutoLight.h"
#include "DlgAutoLocalTexture.h"
#include "DlgAutoMountain.h"
#include "DlgAutoPlant.h"
#include "DlgAutoRoad.h"
#include "DlgAutoTerrain.h"
#include "DlgAutoWater.h"
#include "DlgAutoChooseTexture.h"
#include "DlgCurve.h"
#include "DlgAutoChoosePlant.h"
#include "DlgAutoChooseModel.h"
#include "DlgAutoChooseSky.h"
#include "DlgAutoChooseColor.h"
#include "DlgAutoChooseLight.h"

CECHomeDlgsMgr::CECHomeDlgsMgr()
{
	m_pAUIMgr = NULL;

	m_pAutoScene = NULL;

	m_pDlgAutoBuilding = NULL;
	m_pDlgAutoLight = NULL;
	m_pDlgAutoLocalTexture = NULL;
	m_pDlgAutoMountain = NULL;
	m_pDlgAutoPlant = NULL;
	m_pDlgAutoRoad = NULL;
	m_pDlgAutoTerrain = NULL;
	m_pDlgAutoWater = NULL;
	m_pDlgAutoChooseTexture = NULL;
	m_pDlgCurve = NULL;
	m_pDlgAutoChoosePlant = NULL;
	m_pDlgAutoChooseModel = NULL;
	m_pDlgAutoChooseSky = NULL;
	m_pDlgAutoChooseColor = NULL;

	m_pCurDlg = NULL;

	m_pBtnChooseTerrain = NULL;
	m_pBtnChooseMountain = NULL;
	m_pBtnChooseWater = NULL;
	m_pBtnChooseRoad = NULL;
	m_pBtnChooseLocalTexture = NULL;
	m_pBtnChoosePlant = NULL;
	m_pBtnChooseBuilding = NULL;
	m_pBtnChooseLight = NULL;
	m_pBtnCurChoose = NULL;

	m_bInitReady = false;

	
}
CECHomeDlgsMgr::~CECHomeDlgsMgr()
{
	
}

bool CECHomeDlgsMgr::Init(HomeDlgsMgrInitParams_t params)
{
	if(m_bInitReady)
		return true;

	m_pAUIMgr = params.pAUIMgr;

	m_pAutoScene = g_pGame->GetGameRun()->GetWorld()->GetAutoScene();
	if( !m_pAutoScene)
	{
		a_LogOutput(1, "CECHomeDlgsMgr::Init, Failed to get CAutoScene!");
		AutoHomeAssert();
	}

	if( !InitDialogs())
		return false;
	
	if( !InitSwitchBtn())
		return false;



	m_bInitReady = true;

	return true;
}

//-------------------------------------------------------------------------------
void CECHomeDlgsMgr::HideAllOldDlg()
{
	int nNumDlg = m_pAUIMgr->GetDialogCount();
	for( int i = 0; i < nNumDlg; i++)
	{
		AUIDialog* pDlg = m_pAUIMgr->GetDialog(i);
		if( pDlg->IsShow())
		{
			m_vecOldDlg.push_back(pDlg);
			pDlg->Show(false);
		}
	}
}

//-------------------------------------------------------------------------------
void CECHomeDlgsMgr::ShowAllOldDlg()
{
	OldDlgVecIter_t iter = m_vecOldDlg.begin();
	for( ; iter != m_vecOldDlg.end(); iter++)
	{
		(*iter)->Show(true);
	}
	m_vecOldDlg.clear();
}

//-------------------------------------------------------------------------------
#define INIT_DIALOG_POINTER(variableName, className, dialogName) \
{ \
	variableName = static_cast<className *>(m_pAUIMgr->GetDialog(dialogName)); \
	ASSERT(variableName); \
}

//-------------------------------------------------------------------------------
bool CECHomeDlgsMgr::InitDialogs()
{
	INIT_DIALOG_POINTER(m_pDlgAutoBuilding , CDlgAutoBuilding, "Dlg_Building");
	INIT_DIALOG_POINTER(m_pDlgAutoLight, CDlgAutoLight, "Dlg_Light");
	INIT_DIALOG_POINTER(m_pDlgAutoLocalTexture, CDlgAutoLocalTexture, "Dlg_LocalTexture");
	INIT_DIALOG_POINTER(m_pDlgAutoMountain, CDlgAutoMountain, "Dlg_Mountain");
	INIT_DIALOG_POINTER(m_pDlgAutoPlant, CDlgAutoPlant, "Dlg_Plant");
	INIT_DIALOG_POINTER(m_pDlgAutoRoad, CDlgAutoRoad, "Dlg_Road");
	INIT_DIALOG_POINTER(m_pDlgAutoTerrain, CDlgAutoTerrain, "Dlg_Terrain");
	INIT_DIALOG_POINTER(m_pDlgAutoWater, CDlgAutoWater, "Dlg_Water");
	INIT_DIALOG_POINTER(m_pDlgAutoChooseTexture, CDlgAutoChooseTexture, "Dlg_ChooseTexture");

	INIT_DIALOG_POINTER(m_pDlgCurve, CDlgCurve, "Dlg_Curve");
	INIT_DIALOG_POINTER(m_pDlgAutoChoosePlant, CDlgAutoChoosePlant, "Dlg_ChoosePlant");
	INIT_DIALOG_POINTER(m_pDlgAutoChooseModel, CDlgAutoChooseModel, "Dlg_ChooseModel");
	INIT_DIALOG_POINTER(m_pDlgAutoChooseSky, CDlgAutoChooseSky, "Dlg_ChooseSky");
	INIT_DIALOG_POINTER(m_pDlgAutoChooseColor, CDlgAutoChooseColor, "Dlg_ChooseColor");
	INIT_DIALOG_POINTER(m_pDlgAutoChooseLight, CDlgAutoChooseLight, "Dlg_ChooseLight");

	const int nNumDlgs = 14;
	bool bOK[nNumDlgs];

	bOK[0] = m_pDlgAutoBuilding->InitContext(this);
	bOK[1] = m_pDlgAutoLight->InitContext(this);
	bOK[2] = m_pDlgAutoLocalTexture->InitContext(this);
	bOK[3] = m_pDlgAutoMountain->InitContext(this);
	bOK[4] = m_pDlgAutoPlant->InitContext(this);
	bOK[5] = m_pDlgAutoRoad->InitContext(this);
	bOK[6] = m_pDlgAutoTerrain->InitContext(this);
	bOK[7] = m_pDlgAutoWater->InitContext(this);
	bOK[8] = m_pDlgAutoChooseTexture->InitContext(this);
	bOK[9] = m_pDlgAutoChoosePlant->InitContext(this);
	bOK[10] = m_pDlgAutoChooseModel->InitContext(this);
	bOK[11] = m_pDlgAutoChooseSky->InitContext(this);
	bOK[12] = m_pDlgAutoChooseColor->InitContext(this);
	bOK[13] = m_pDlgAutoChooseLight->InitContext(this);

	bool bRet = true;
	for(int i = 0; i < nNumDlgs; i++)
	{
		bRet = bRet && bOK[i];
	}

	m_pCurDlg = m_pDlgAutoTerrain;

	return bRet;

}

void CECHomeDlgsMgr::Show(bool bShow /* = true*/)
{
	if(!m_pCurDlg)
		return ;
	
	if( bShow)
	{
//		if(!LoadAutoSceneAction("facedata\\autohome\\action\\simple.action"))
//			return;
		
		HideAllOldDlg();
		m_pCurDlg->Show(true);

		
	}
	else
	{
		m_pCurDlg->Show(false);
		ShowAllOldDlg();
	}


}

#define INIT_BUTTON_POINTER(variableName, dialogPointer, buttonName) \
{ \
	variableName = static_cast<AUIStillImageButton *>( \
		dialogPointer->GetDlgItem(buttonName)); \
	ASSERT(variableName); \
}

bool CECHomeDlgsMgr::InitSwitchBtn()
{
	INIT_BUTTON_POINTER(m_pBtnChooseTerrain, m_pDlgAutoTerrain, "Btn_TDlgTerrain");
	INIT_BUTTON_POINTER(m_pBtnChooseMountain, m_pDlgAutoMountain, "Btn_MDlgMountain");
	INIT_BUTTON_POINTER(m_pBtnChooseWater, m_pDlgAutoWater, "Btn_WDlgWater");
	INIT_BUTTON_POINTER(m_pBtnChooseRoad, m_pDlgAutoRoad, "Btn_RDlgRoad");
	INIT_BUTTON_POINTER(m_pBtnChooseLocalTexture, m_pDlgAutoLocalTexture, "Btn_LDlgLocalTexture");
	INIT_BUTTON_POINTER(m_pBtnChoosePlant, m_pDlgAutoPlant, "Btn_PDlgPlant");
	INIT_BUTTON_POINTER(m_pBtnChooseBuilding, m_pDlgAutoBuilding, "Btn_BDlgBuilding");
	INIT_BUTTON_POINTER(m_pBtnChooseLight, m_pDlgAutoLight, "Btn_LiDlgLight");

	m_pBtnChooseTerrain->SetPushLike(true);
	m_pBtnChooseMountain->SetPushLike(true);
	m_pBtnChooseWater->SetPushLike(true);
	m_pBtnChooseRoad->SetPushLike(true);
	m_pBtnChooseLocalTexture->SetPushLike(true);
	m_pBtnChoosePlant->SetPushLike(true);
	m_pBtnChooseBuilding->SetPushLike(true);
	m_pBtnChooseLight->SetPushLike(true);
	
	
	m_pBtnCurChoose = m_pBtnChooseTerrain;
	m_pBtnCurChoose->SetPushed(true);

	return true;
}

void CECHomeDlgsMgr::SwitchDialog(const char * szCommand)
{
	AString strCommand = szCommand;

	CDlgHomeBase *pDlg = NULL;

	if( strCommand == "ChooseTerrain")
		pDlg = m_pDlgAutoTerrain;
	else if ( strCommand == "ChooseMountain")
		pDlg = m_pDlgAutoMountain;
	else if( strCommand == "ChooseWater")
		pDlg = m_pDlgAutoWater;
	else if( strCommand == "ChooseRoad")
		pDlg = m_pDlgAutoRoad;
	else if( strCommand == "ChooseLocalTexture")
		pDlg = m_pDlgAutoLocalTexture;
	else if( strCommand == "ChoosePlant")
		pDlg = m_pDlgAutoPlant;
	else if( strCommand == "ChooseBuilding")
		pDlg = m_pDlgAutoBuilding;
	else if( strCommand == "ChooseLight")
		pDlg = m_pDlgAutoLight;
	
	
	// set button state
	m_pBtnCurChoose->SetPushed(false);
	
	if( pDlg->GetSwithButton())
	{
		m_pBtnCurChoose = pDlg->GetSwithButton();
	}

	m_pBtnCurChoose->SetPushed(true);

	// hide/show dialogs
	m_pCurDlg->Show(false);
	m_pCurDlg = pDlg;
	m_pCurDlg->Show(true);

}

bool CECHomeDlgsMgr::LoadAutoSceneAction(const char* szActionFile)
{
	CAutoScene* pAutoScene = NULL;
	pAutoScene = g_pGame->GetGameRun()->GetWorld()->GetAutoScene();
		if( !pAutoScene)
			return false;
	
	CAutoTerrainAction* pAction = NULL;
	pAction = pAutoScene->GetTerrainAction();
	if( !pAction)
		return false;

	if(!pAction->LoadActions(szActionFile))
		return false;

	if(!pAction->DoActions(pAutoScene))
		return false;

	if (!pAutoScene->RecreateTerrainRender(true))
	{
		return false;
	}

	return true;
}

bool CECHomeDlgsMgr::LoadTextureParamInfo(TextureParamInfo_t& param, TEXTURETYPE mainTexture, TEXTURETYPE patchTexture)
{
	CAutoTerrain* pAutoTerrain = m_pAutoScene->GetAutoTerrain();
	
	//Load main texture
	CTerrainLayer* pLayer = pAutoTerrain->GetLayer(mainTexture + 1);
	if( NULL == pLayer)
		return false;
	
	//Main Texture file
	param.strMainTextureFile = pLayer->GetTextureFile();

	GetTextureIDAndIdx(param.strMainTextureFile, param.nMainTexID, param.nMainTexIdx);

	//Main texture UV
	param.fMainTextureU = 1.0f / pLayer->GetScaleU();
	param.fMainTextureV = 1.0f / pLayer->GetScaleV();

	//Load Patch texture
	pLayer = pAutoTerrain->GetLayer(patchTexture + 1);

	//Patch texture
	param.strPatchTextureFile = pLayer->GetTextureFile();

	GetTextureIDAndIdx(param.strPatchTextureFile, param.nPatchTexID, param.nPatchTexIdx);
	
	param.fPatchTextureU = 1.0f / pLayer->GetScaleU();
	param.fPatchTextureV = 1.0f / pLayer->GetScaleV();

	
	return true;
}

bool CECHomeDlgsMgr::SaveTextureParamInfo(TextureParamInfo_t& param, TEXTURETYPE mainTexture, TEXTURETYPE patchTexture)
{
	CAutoTexture* pTexture = m_pAutoScene->GetAutoTerrain()->GetTexture();
	bool bOK = pTexture->ChangeTextureParam(
		mainTexture,
		patchTexture,
		param.fMainTextureU,
		param.fMainTextureV,
		param.fPatchTextureU,
		param.fPatchTextureV,
		param.strMainTextureFile,
		param.strPatchTextureFile);

	if( !bOK)
		return false;

	pTexture->SetTextureID(mainTexture, param.nMainTexID);
	pTexture->SetTextureID(patchTexture, param.nPatchTexID);
	
	return true;
}

bool CECHomeDlgsMgr::GetTextureIDAndIdx(const AString& strTexName, unsigned int& uTexID, int& uTexIdx)
{
	CAutoTexture* pAutoTexture = m_pAutoScene->GetAutoTerrain()->GetTexture();

	CAutoTexture::TEXTURETYPEINFO* pTexInfo = pAutoTexture->GetTextureInfos();
	if( !pTexInfo)
	{
		a_LogOutput(1, "CECHomeDlgsMgr::GetTextureIDAndIdx, Failed to Get CAutoTexture!");
		AutoHomeAssert();
	}

	int nNumTexInfos = pAutoTexture->GetNumTextureInfos();

	for( int i = 0; i < nNumTexInfos; ++i)
	{
		if( pTexInfo[i].strTexture == strTexName)
		{
			uTexID = pTexInfo[i].dwID;
			uTexIdx = i;
			return true;
		}
	}

	return false;

}