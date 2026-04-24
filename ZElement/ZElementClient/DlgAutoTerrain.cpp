/*
 * FILE: DlgAutoTerrain.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoTerrain.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoChooseTexture.h"

#include "aui/AUILabel.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrain.h"


AUI_BEGIN_COMMAND_MAP(CDlgAutoTerrain, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("CreateTerrain", OnCommandCreateTerrain)
AUI_ON_COMMAND("OpenChooseTexture", OnCommandOpenChooseTexture)
AUI_ON_COMMAND("CreateTexture", OnCommandCreateTexture)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoTerrain, CDlgHomeBase)

AUI_END_EVENT_MAP()


CDlgAutoTerrain::CDlgAutoTerrain()
{
	m_pSliderMaxHeight = NULL;
	m_pSliderMainSmooth = NULL;
	m_pSliderPartSmooth = NULL;
	m_pSliderTerrainSeed = NULL;

	m_pLabelMaxHeight = NULL;
	m_pLabelMainSmooth = NULL;
	m_pLabelPartSmooth = NULL;
	m_pLabelTerrainSeed = NULL;

	m_nMaxHeight = 0;
	m_nMainSmooth = 0;
	m_nPartSmooth = 0;
	m_nTerrainSeed = 0;
}

CDlgAutoTerrain::~CDlgAutoTerrain()
{
}

bool CDlgAutoTerrain::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();
	
	return true;
}

void CDlgAutoTerrain::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();
	SetCanMove(false);
}

AUIStillImageButton * CDlgAutoTerrain::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseTerrain;
}

void CDlgAutoTerrain::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_MaxHeight", m_pSliderMaxHeight);
	DDX_Control("Slider_MainSmooth", m_pSliderMainSmooth);
	DDX_Control("Slider_PartSmooth", m_pSliderPartSmooth);
	DDX_Control("Slider_TerrainSeed", m_pSliderTerrainSeed);

	DDX_Control("Label_MaxHeight", m_pLabelMaxHeight);
	DDX_Control("Label_MainSmooth", m_pLabelMainSmooth);
	DDX_Control("Label_PartSmooth", m_pLabelPartSmooth);
	DDX_Control("Label_TerrainSeed", m_pLabelTerrainSeed);

	DDX_Slider(bSave, "Slider_MaxHeight", m_nMaxHeight);
	DDX_Slider(bSave, "Slider_MainSmooth", m_nMainSmooth);
	DDX_Slider(bSave, "Slider_PartSmooth", m_nPartSmooth);
	DDX_Slider(bSave, "Slider_TerrainSeed", m_nTerrainSeed);
}

void CDlgAutoTerrain::SetSliderBound()
{
	m_pSliderMaxHeight->SetTotal(800, 0);
	m_pSliderMainSmooth->SetTotal(10, 1);
	m_pSliderPartSmooth->SetTotal(10, 0);
	m_pSliderTerrainSeed->SetTotal(100, 0);
}

void CDlgAutoTerrain::LoadDefault()
{
	m_nMaxHeight = 150;
	m_nMainSmooth = 3;
	m_nPartSmooth = 3;
	m_nTerrainSeed = 4;

	m_texParamInfo.fTextureRatio	= 0.2f;
	m_texParamInfo.fMainTexture		= 0.4f;
	m_texParamInfo.fPatchTexture	= 1.2f;
	m_texParamInfo.fPatchDivision	= 30.0f;
	m_texParamInfo.dwTextureSeed	= 1;

	GetHomeDlgsMgr()->LoadTextureParamInfo(m_texParamInfo, TT_MAIN, TT_MAIN_PATCH);
}

void CDlgAutoTerrain::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
	
}

void CDlgAutoTerrain::SetLabelsText()
{
	ACHAR szName[100];
	swprintf(szName, _AL("%d"), m_nMaxHeight);
	m_pLabelMaxHeight->SetText(szName);

	swprintf(szName, _AL("%d"), m_nMainSmooth);
	m_pLabelMainSmooth->SetText(szName);
	
	swprintf(szName, _AL("%-4.2f"), m_nPartSmooth * 0.1f);
	m_pLabelPartSmooth->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTerrainSeed);
	m_pLabelTerrainSeed->SetText(szName);

}

void CDlgAutoTerrain::OnCommandCreateTerrain(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	

	CAutoTerrain* pAutoTerrain = pAutoScene->GetAutoTerrain();


	pAutoTerrain->CreateTerrain((float)m_nMaxHeight, m_nMainSmooth, (float)(m_nPartSmooth * 0.1f), m_nTerrainSeed);

	pAutoScene->GetTerrainAction()->AddActionBaseHeightMap((float)m_nMaxHeight, m_nMainSmooth, (float)(m_nPartSmooth * 0.1f), m_nTerrainSeed);

	pAutoScene->RecreateTerrainRender(true);

	pAutoTerrain->Tick(0);// Update terrain height
	CAutoBuildingOperation* pBuildingOperation = pAutoScene->GetAutoBuildingOperation();
	pBuildingOperation->AdjustModelsAfterTerrainChange();
		
}

void CDlgAutoTerrain::OnCommandOpenChooseTexture(const char* szCommand)
{
	CDlgAutoChooseTexture* pDlg = GetHomeDlgsMgr()->GetAutoChooseTextureDlg();
	pDlg->InitTextureParamInfo(m_texParamInfo, CTIP_TERRAIN);
	pDlg->Show(true);
}

void CDlgAutoTerrain::OnCommandCreateTexture(const char* szCommand)
{	
	CAutoScene* pScene = GetAutoScene();
	
	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	
	if (!pTexture->CalculateMask(
		90, 
		m_texParamInfo.fTextureRatio, 
		m_texParamInfo.fPatchDivision, 
		m_texParamInfo.fPatchTexture, 
		m_texParamInfo.fMainTexture, 
		m_texParamInfo.dwTextureSeed))
	{
		AutoHomeAssert();
		a_LogOutput(1, "DlgAutoTerrain::OnCommandCreateTexture, Failed to Create Texture!");

		return;
	}

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	pAction->AddActionBaseTexture(
		m_texParamInfo.fTextureRatio, 
		m_texParamInfo.fPatchDivision, 
		m_texParamInfo.fPatchTexture, 
		m_texParamInfo.fMainTexture, 
		m_texParamInfo.dwTextureSeed);

	pAction->AddActionBaseTextureInfo(
		m_texParamInfo.fMainTextureU,
		m_texParamInfo.fMainTextureV,
		m_texParamInfo.fPatchTextureU,
		m_texParamInfo.fPatchTextureV,
		pTexture->GetTextureID(TT_MAIN),
		pTexture->GetTextureID(TT_MAIN_PATCH));

	if (!pScene->RecreateTerrainRender(false))
	{
		AutoHomeAssert();
		a_LogOutput(1, "DlgAutoTerrain::OnCommandCreateTexture, Failed to recreate terrain render!");
		return;
	}
}