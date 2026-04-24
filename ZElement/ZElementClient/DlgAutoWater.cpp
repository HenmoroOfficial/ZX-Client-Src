/*
 * FILE: DlgAutoWater.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoWater.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoChooseTexture.h"

#include "aui/AUISlider.h"
#include "aui/AUILabel.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrain.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoWater, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OpenChooseTexture", OnCommandOpenChooseTexture)
AUI_ON_COMMAND("CreateWater", OnCommandCreateWater)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoWater, CDlgHomeBase)

AUI_END_EVENT_MAP()

CDlgAutoWater::CDlgAutoWater()
{
	m_pSliderWaterHeight = NULL;
	m_pSliderSandWidth = NULL;

	m_pLabelWaterHeight = NULL;
	m_pLabelSandWidth = NULL;

	m_nWaterHeight = 0;
	m_nSandWidth = 0;
}

CDlgAutoWater::~CDlgAutoWater()
{
}

bool CDlgAutoWater::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	return true;
}

void CDlgAutoWater::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();

	SetCanMove(false);

	UpdateTerrainHeight();
}

AUIStillImageButton * CDlgAutoWater::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseWater;
}

void CDlgAutoWater::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_WaterHeight", m_pSliderWaterHeight);
	DDX_Control("Slider_SandWidth", m_pSliderSandWidth);
	
	DDX_Control("Label_WaterHeight", m_pLabelWaterHeight);
	DDX_Control("Label_SandWidth", m_pLabelSandWidth);

	DDX_Slider(bSave, "Slider_WaterHeight", m_nWaterHeight);
	DDX_Slider(bSave, "Slider_SandWidth", m_nSandWidth);

}

void CDlgAutoWater::SetSliderBound()
{
	m_pSliderWaterHeight->SetTotal(0, 0);
	m_pSliderSandWidth->SetTotal(200, 0);
}

void CDlgAutoWater::LoadDefault()
{
	m_nWaterHeight = 0;
	m_nSandWidth = 40;

	m_texParamInfo.fTextureRatio	= 0.2f;
	m_texParamInfo.fMainTexture		= 0.4f;
	m_texParamInfo.fPatchTexture	= 1.2f;
	m_texParamInfo.fPatchDivision	= 30.0f;
	m_texParamInfo.dwTextureSeed	= 1;

	GetHomeDlgsMgr()->LoadTextureParamInfo(m_texParamInfo, TT_RIVERBED, TT_RIVERSAND);

}

void CDlgAutoWater::SetLabelsText()
{
	ACHAR szName[100];
	swprintf(szName, _AL("%d"), m_nWaterHeight);
	m_pLabelWaterHeight->SetText(szName);

	swprintf(szName, _AL("%d"), m_nSandWidth);
	m_pLabelSandWidth->SetText(szName);

}

void CDlgAutoWater::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoWater::OnCommandCreateWater(const char* szCommand)
{
	
	CAutoScene* pAutoScene = GetAutoScene();
	CAutoTerrain* pAutoTerrain = pAutoScene->GetAutoTerrain();
	
	CAutoWater* pAutoWater = pAutoScene->GetAutoWater();
	
	
	if(!pAutoWater->CreateWater((float)m_nWaterHeight, (float)m_nSandWidth))
	{
		a_LogOutput(1, "CDlgAutoTerrain::OnCommandCreateWater, Failed to create water!");
		AutoHomeAssert();
		return;
	}
	
	// Store action
	CAutoTerrainAction* pAction = pAutoScene->GetTerrainAction();
	if (!pAction->AddActionWater((float)m_nWaterHeight, (float)m_nSandWidth))
	{
		a_LogOutput(1, "CDlgAutoTerrain::OnCommandCreateWater, Failed to AddActionWater!");
		AutoHomeAssert();
		return;
	}

	CAutoTexture* pTexture = pAutoScene->GetAutoTerrain()->GetTexture();
	if (!pAction->AddActionWaterTextureInfo(
			m_texParamInfo.fMainTextureU,
			m_texParamInfo.fMainTextureV,
			m_texParamInfo.fPatchTextureU,
			m_texParamInfo.fPatchTextureV,
			pTexture->GetTextureID(TT_RIVERBED),
			pTexture->GetTextureID(TT_RIVERSAND)))
	{
		a_LogOutput(1, "CDlgAutoTerrain::OnCommandCreateWater, Failed to AddActionWaterTextureInfo");
		AutoHomeAssert();
		return;
	}
	
	pAutoScene->RecreateTerrainRender(false);
	
}

void CDlgAutoWater::OnCommandOpenChooseTexture(const char* szCommand)
{
	CDlgAutoChooseTexture* pDlg = GetHomeDlgsMgr()->GetAutoChooseTextureDlg();
	pDlg->InitTextureParamInfo(m_texParamInfo, CTIP_WATER);
	pDlg->Show(true);
}

void CDlgAutoWater::UpdateTerrainHeight()
{
	CAutoScene* pScene = GetAutoScene();
	CAutoTerrain* pTerrain = pScene->GetAutoTerrain();
	CAutoTerrain::VERTEXINFO minVertex, maxVertex;
	pTerrain->GetExtremeVertices(minVertex, maxVertex);

	int nMaxHeight = (int)(maxVertex.fHeight + 0.5);
	int nMinHeight = (int)(minVertex.fHeight + 0.5);

	m_pSliderWaterHeight->SetTotal(nMaxHeight, nMinHeight);
	m_nWaterHeight = nMinHeight;
	UpdateData(false);
	SetLabelsText();

}