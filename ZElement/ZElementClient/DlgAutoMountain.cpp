/*
 * FILE: DlgAutoMountain.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoMountain.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgCurve.h"
#include "DlgAutoChooseTexture.h"

#include "aui/AUILabel.h"
#include "aui/AUIManager.h"

#include "AutoTerrainCommon.h"

#include "A3DEngine.h"
#include "A3DViewport.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "AutoScene.h"
#include "AutoTerrain.h"

#include "windowsx.h"

#include "A3DMacros.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoMountain, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("EditHillPoly", OnCommandEditHillPoly)
AUI_ON_COMMAND("CreateHillPoly", OnCommandCreateHillPoly)
AUI_ON_COMMAND("EditHillLine", OnCommandEditHillLine)
AUI_ON_COMMAND("CreateHillLine",OnCommandCreateHillLine) 
AUI_ON_COMMAND("PerspectiveView", OnCommandPerspectiveView)
AUI_ON_COMMAND("OpenHeightCurveDlg", OnCommandOpenHeightCurveDlg)
AUI_ON_COMMAND("OpenLineCurveDlg", OnCommandOpenLineCurveDlg)
AUI_ON_COMMAND("OpenChooseTexture", OnCommandOpenChooseTextureDlg)
AUI_ON_COMMAND("ClearAllHill", OnCommandClearAllHill)
AUI_ON_COMMAND("Refresh", OnCommandRefresh)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoMountain, CDlgHomeBase)



AUI_END_EVENT_MAP()

CDlgAutoMountain::CDlgAutoMountain()
{
	m_pSliderMaxHeightHill = NULL;
	m_pSliderMainSmoothHill = NULL;
	m_pSliderPartSmoothHill = NULL;
	m_pSliderTerrainSeedHill = NULL;
	m_pSliderBridgeScope = NULL;
	m_pSliderBridgeDepth = NULL;
	m_pSliderBorderErode = NULL;
	m_pSliderLineHillWidth = NULL;

	m_pLabelMaxHeightHill = NULL;
	m_pLabelMainSmoothHill = NULL;
	m_pLabelPartSmoothHill = NULL;
	m_pLabelTerrainSeedHill = NULL;
	m_pLabelBridgeScope = NULL;
	m_pLabelBridgeDepth = NULL;
	m_pLabelBorderErode = NULL;
	m_pLabelLineHillWidth = NULL;

	m_nMaxHeightHill = 0;
	m_nMainSmoothHill = 0;
	m_nPartSmoothHill = 0;
	m_nTerrainSeedHill = 0;
	m_nBridgeScope = 0;
	m_nBridgeDepth = 0;
	m_nBorderErode = 0;
	m_nLineHillWidth = 0;

	m_bCheckHillUp = true;

	m_pCurveHeight = new CCurvePoint;
	m_pCurveLine = new CCurvePoint;

	float fSpriteValues[10];
	fSpriteValues[0] = 1.0f;
	fSpriteValues[1] = 0.94f;
	fSpriteValues[2] = 0.8f;
	fSpriteValues[3] = 0.6f;
	fSpriteValues[4] = 0.40f;
	fSpriteValues[5] = 0.25f;
	fSpriteValues[6] = 0.20f;
	fSpriteValues[7] = 0.30f;
	fSpriteValues[8] = 0.40f;
	fSpriteValues[9] = 0.5f;

	m_pCurveHeight->SetPointValues(7, fSpriteValues);

	fSpriteValues[0] = 0.6f;
	m_pCurveLine->SetPointValues(10, fSpriteValues);

	//memset(&m_texParamInfo, 0, sizeof(TextureParamInfo_t));

	m_bChangeHillPoly = true;
	m_bChangeHillLine = true;


}

CDlgAutoMountain::~CDlgAutoMountain()
{
	if( m_pCurveHeight)
	{
		delete m_pCurveHeight;
		m_pCurveHeight = NULL;
	}

	if(m_pCurveLine)
	{
		delete m_pCurveLine;
		m_pCurveLine = NULL;
	}
}

bool CDlgAutoMountain::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	return true;
}

void CDlgAutoMountain::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();

	SetCanMove(false);
}

AUIStillImageButton * CDlgAutoMountain::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseMountain;
}

void CDlgAutoMountain::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_MaxHeightHill", m_pSliderMaxHeightHill);
	DDX_Control("Slider_MainSmoothHill", m_pSliderMainSmoothHill);
	DDX_Control("Slider_PartSmoothHill", m_pSliderPartSmoothHill);
	DDX_Control("Slider_TerrainSeedHill", m_pSliderTerrainSeedHill);
	DDX_Control("Slider_BridgeScope", m_pSliderBridgeScope);
	DDX_Control("Slider_BridgeDepth", m_pSliderBridgeDepth);
	DDX_Control("Slider_BorderErode", m_pSliderBorderErode);
	DDX_Control("Slider_LineHillWidth", m_pSliderLineHillWidth);

	DDX_Control("Label_MaxHeightHill", m_pLabelMaxHeightHill);
	DDX_Control("Label_MainSmoothHill", m_pLabelMainSmoothHill);
	DDX_Control("Label_PartSmoothHill", m_pLabelPartSmoothHill);
	DDX_Control("Label_TerrainSeedHill", m_pLabelTerrainSeedHill);
	DDX_Control("Label_BridgeScope", m_pLabelBridgeScope);
	DDX_Control("Label_BridgeDepth", m_pLabelBridgeDepth);
	DDX_Control("Label_BorderErode", m_pLabelBorderErode);
	DDX_Control("Label_LineHillWidth", m_pLabelLineHillWidth);

	DDX_Slider(bSave, "Slider_MaxHeightHill", m_nMaxHeightHill);
	DDX_Slider(bSave, "Slider_MainSmoothHill", m_nMainSmoothHill);
	DDX_Slider(bSave, "Slider_PartSmoothHill", m_nPartSmoothHill);
	DDX_Slider(bSave, "Slider_TerrainSeedHill", m_nTerrainSeedHill);
	DDX_Slider(bSave, "Slider_BridgeScope", m_nBridgeScope);
	DDX_Slider(bSave, "Slider_BridgeDepth", m_nBridgeDepth);
	DDX_Slider(bSave, "Slider_BorderErode", m_nBorderErode);
	DDX_Slider(bSave, "Slider_LineHillWidth", m_nLineHillWidth);

	DDX_CheckBox(bSave, "Check_HillUp", m_bCheckHillUp);

	DDX_RadioButton(bSave, 1, m_nUpDownPri);
}

void CDlgAutoMountain::SetSliderBound()
{
	m_pSliderMaxHeightHill->SetTotal(800, 0);
	m_pSliderMainSmoothHill->SetTotal(1000, 100);
	m_pSliderPartSmoothHill->SetTotal(100, 0);
	m_pSliderTerrainSeedHill->SetTotal(100, 0);
	m_pSliderBridgeScope->SetTotal(100, 0);
	m_pSliderBridgeDepth->SetTotal(100, 0);
	m_pSliderBorderErode->SetTotal(100, 0);
	m_pSliderLineHillWidth->SetTotal(200, 1);
}
void CDlgAutoMountain::LoadDefault()
{
	m_nMaxHeightHill = 150;
	m_nMainSmoothHill = 300;
	m_nPartSmoothHill = 40;
	m_nTerrainSeedHill = 1;
	m_nBridgeScope = 60;
	m_nBridgeDepth = 20;
	m_nBorderErode = 40;
	m_nLineHillWidth = 100;

	m_bCheckHillUp = true;

	m_nUpDownPri = 0;

	m_texParamInfo.fTextureRatio	= 0.2f;
	m_texParamInfo.fMainTexture		= 0.4f;
	m_texParamInfo.fPatchTexture	= 1.2f;
	m_texParamInfo.fPatchDivision	= 30.0f;
	m_texParamInfo.dwTextureSeed	= 1;

	GetHomeDlgsMgr()->LoadTextureParamInfo(m_texParamInfo, TT_BRIDGE, TT_BRIDGE_PATCH);
}

void CDlgAutoMountain::SetLabelsText()
{
	ACHAR szName[100];
	
	swprintf(szName, _AL("%d"), m_nMaxHeightHill);
	m_pLabelMaxHeightHill->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nMainSmoothHill * 0.01f));
	m_pLabelMainSmoothHill->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nPartSmoothHill * 0.01f));
	m_pLabelPartSmoothHill->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTerrainSeedHill);
	m_pLabelTerrainSeedHill->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nBridgeScope * 0.01f));
	m_pLabelBridgeScope->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nBridgeDepth * 0.01f));
	m_pLabelBridgeDepth->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nBorderErode * 0.01f));
	m_pLabelBorderErode->SetText(szName);

	swprintf(szName, _AL("%d"), m_nLineHillWidth);
	m_pLabelLineHillWidth->SetText(szName);
}

void CDlgAutoMountain::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoMountain::OnCommandEditHillPoly(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();

	pAutoScene->SetOperationType(OT_HILL_POLY);
}

void CDlgAutoMountain::OnCommandCreateHillPoly(const char* szCommand)
{
	UpdateData(true);

	if( GetOperationType() != OT_HILL_POLY)
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÇëÏÈµãÉèÖÃ¶à±ßĞÎÉ½!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	CAutoHillPoly* pAutoHillPoly = pScene->GetAutoHillPoly();
	if (NULL == pAutoHillPoly)
		return;

	if (!pAutoHillPoly->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÇëÑ¡Ôñ·â±ÕµÄ¶à±ßĞÎÇøÓò!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if( !m_bChangeHillPoly)
	{
		pAutoHillPoly->StorePoints();
		pScene->UndoAction(TAT_HILL_POLY);
		pAutoHillPoly->RestorePoints();
	}
	 
	// Create height
	if (!pAutoHillPoly->CreateHillPoly(
			(float)m_nMaxHeightHill,
			(float)(m_nMainSmoothHill * 0.01f),
			(float)(m_nPartSmoothHill * 0.01f),
			m_nTerrainSeedHill,
			m_pCurveHeight->GetNumPoints(),
			m_pCurveHeight->GetPointValues(),
			m_bCheckHillUp,
			(float)(m_nBorderErode * 0.01f)))
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÔìÉ½Ê§°Ü!-_-b"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	if( m_bCheckHillUp)
	{
		if( !pAutoHillPoly->RenderHillTop(
			m_texParamInfo.fTextureRatio,
			m_texParamInfo.fMainTexture,
			m_texParamInfo.fPatchTexture,
			m_texParamInfo.fPatchDivision,
			(float)(m_nBridgeScope * 0.01f),
			(float)(m_nBridgeDepth * 0.01f)))
		{
			GetAUIManager()->MessageBox("´íÎó", _AL("äÖÈ¾É½¼¹Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}

	// Store action
	CPolygonMountain* pArea = pScene->GetAutoHillPoly()->GetArea();
	if (!pAction->AddActionHillPoly(
			(float)m_nMaxHeightHill,
			(float)(m_nMainSmoothHill * 0.01f),
			(float)(m_nPartSmoothHill * 0.01f),
			m_nTerrainSeedHill,
			m_pCurveHeight->GetNumPoints(),
			m_pCurveHeight->GetPointValues(), 
			m_bCheckHillUp,
			m_texParamInfo.fTextureRatio,
			m_texParamInfo.fMainTexture, 
			m_texParamInfo.fPatchTexture,
			m_texParamInfo.fPatchDivision,
			(float)(m_nBridgeScope * 0.01f),
			(float)(m_nBridgeDepth * 0.01f),
			pArea->GetNumPoints(),
			pArea->GetFinalPoints(),
			(float)(m_nBridgeDepth * 0.01f)))
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("±£´æ²Ù×÷Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	if (m_bCheckHillUp)
	{
		if (!pAction->AddActionHillTextureInfo(
				m_texParamInfo.fMainTextureU,
				m_texParamInfo.fMainTextureV,
				m_texParamInfo.fPatchTextureU,
				m_texParamInfo.fPatchTextureV,
				pTexture->GetTextureID(TT_BRIDGE),
				pTexture->GetTextureID(TT_BRIDGE_PATCH)))
		{
			GetAUIManager()->MessageBox("´íÎó", _AL("±£´æ²Ù×÷Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}

	// Clear hill bridge texture on down area.
	if (2 == m_nUpDownPri)// Down first
	{
		pTexture->ClearBridgeAtDownArea();
		// Save mask files
		pTexture->SaveAndReloadMask(TT_BRIDGE);
		pTexture->SaveAndReloadMask(TT_BRIDGE_PATCH);
	}

	pScene->RecreateTerrainRender(true);

	// Update buildings
	pScene->GetAutoTerrain()->Tick(0);// Update terrain height
	CAutoBuildingOperation* pBuildingOperation = pScene->GetAutoBuildingOperation();
	pBuildingOperation->AdjustModelsAfterTerrainChange();

	m_bChangeHillPoly = false;

	return;
}

void CDlgAutoMountain::OnCommandEditHillLine(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();

	pAutoScene->SetOperationType(OT_HILL_LINE);
}

void CDlgAutoMountain::OnCommandCreateHillLine(const char* szCommand)
{
	
	if (GetOperationType() != OT_HILL_LINE)
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÇëÏÈµãÉèÖÃÏßĞÎÉ½!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	CAutoHillLine* pAutoHillLine = pScene->GetAutoHillLine();
	if (NULL == pAutoHillLine)
		return;

	if (!pAutoHillLine->IsPointsValid())
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÇëÑ¡ÔñÂ·¾¶!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		return;
	}
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if( !m_bChangeHillLine)
	{
		pAutoHillLine->StorePoints();
		pScene->UndoAction(TAT_HILL_LINE);
		pAutoHillLine->RestorePoints();
	}

	if (!pAutoHillLine->CreateHillLine(
				(float)m_nMaxHeightHill,
				(float)m_nLineHillWidth,
				(float)(m_nMainSmoothHill * 0.01f),
				(float)(m_nPartSmoothHill * 0.01f),
				m_nTerrainSeedHill,
				m_pCurveHeight->GetNumPoints(),
				m_pCurveHeight->GetPointValues(),
				m_pCurveLine->GetNumPoints(),
				m_pCurveLine->GetPointValues(),
				m_bCheckHillUp,
				(float)(m_nBorderErode * 0.01f)))
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("ÔìÉ½Ê§°Ü!-_-b"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}	

	// Render top
	if (m_bCheckHillUp)
	{
		if (!pAutoHillLine->RenderHillTop(
				m_texParamInfo.fTextureRatio,
				m_texParamInfo.fMainTexture,
				m_texParamInfo.fPatchTexture,
				m_texParamInfo.fPatchDivision,
				(float)(m_nBridgeScope * 0.01f)))
		{
			GetAUIManager()->MessageBox("´íÎó", _AL("äÖÈ¾É½¼¹Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	
	// Store action
	
	if (!pAction->AddActionHillLine(
			(float)m_nMaxHeightHill,
			(float)m_nLineHillWidth,
			(float)(m_nMainSmoothHill * 0.01f),
			(float)(m_nPartSmoothHill * 0.01f),
			m_nTerrainSeedHill,
			m_pCurveHeight->GetNumPoints(),
			m_pCurveHeight->GetPointValues(),
			m_pCurveLine->GetNumPoints(),
			m_pCurveLine->GetPointValues(), 
			m_bCheckHillUp,
			pAutoHillLine->GetNumPoints(),
			pAutoHillLine->GetPoints(),
			m_texParamInfo.fTextureRatio,
			m_texParamInfo.fMainTexture, 
			m_texParamInfo.fPatchTexture,
			m_texParamInfo.fPatchDivision,
			(float)(m_nBridgeScope * 0.01f),
			(float)(m_nBorderErode * 0.01f)))
	{
		GetAUIManager()->MessageBox("´íÎó", _AL("±£´æ²Ù×÷Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if (m_bCheckHillUp)
	{
		if (!pAction->AddActionHillTextureInfo(
				m_texParamInfo.fMainTextureU,
				m_texParamInfo.fMainTextureV,
				m_texParamInfo.fPatchTextureU,
				m_texParamInfo.fPatchTextureV,
				pTexture->GetTextureID(TT_BRIDGE),
				pTexture->GetTextureID(TT_BRIDGE_PATCH)))
		{
			GetAUIManager()->MessageBox("´íÎó", _AL("±£´æ²Ù×÷Ê§°Ü!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	
	// Clear hill bridge texture on down area.
	if (2 == m_nUpDownPri)// Down first
	{
		pTexture->ClearBridgeAtDownArea();
		// Save mask files
		pTexture->SaveAndReloadMask(TT_BRIDGE);
		pTexture->SaveAndReloadMask(TT_BRIDGE_PATCH);
	}

	pScene->RecreateTerrainRender(true);

	// Update buildings
	pScene->GetAutoTerrain()->Tick(0);// Update terrain height
	CAutoBuildingOperation* pBuildingOperation = pScene->GetAutoBuildingOperation();
	pBuildingOperation->AdjustModelsAfterTerrainChange();

	m_bChangeHillLine = false;
}

void CDlgAutoMountain::OnCommandPerspectiveView(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_NORMAL);
}

void CDlgAutoMountain::OnLButtonDownScreen(WPARAM wParam, LPARAM lParam)
{
		
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_HILL_POLY)
	{
		if( pAutoScene->GetAutoHillPoly())
		{
			pAutoScene->GetAutoHillPoly()->OnLButtonDown(NULL, point);
			m_bChangeHillPoly = true;
		}
		return;
	}
	else if( nType == OT_HILL_LINE)
	{
		if(pAutoScene->GetAutoHillLine())
		{
			pAutoScene->GetAutoHillLine()->OnLButtonDown(NULL, point);
			m_bChangeHillLine = true;
		}
		return;
	}
	else
		return;
}

void CDlgAutoMountain::OnRButtonUpScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_HILL_POLY)
	{
		if( pAutoScene->GetAutoHillPoly())
		{
			pAutoScene->GetAutoHillPoly()->OnRButtonUp(NULL, point);
			m_bChangeHillPoly = true;
		}
		return;
	}
	else if( nType == OT_HILL_LINE)
	{
		if(pAutoScene->GetAutoHillLine())
		{
			pAutoScene->GetAutoHillLine()->OnRButtonUp(NULL, point);
			m_bChangeHillLine = true;
		}
		return;
	}
	else
		return;
}

void CDlgAutoMountain::OnRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_HILL_POLY)
	{
		if(pAutoScene->GetAutoHillPoly())
		{
			pAutoScene->GetAutoHillPoly()->OnRButtonDblClk(NULL, point);
			m_bChangeHillPoly = true;
		}
		return;
	}
	else if( nType == OT_HILL_LINE)
	{
		if(pAutoScene->GetAutoHillLine())
		{
			pAutoScene->GetAutoHillLine()->OnRButtonDblClk(NULL, point);
			m_bChangeHillLine = true;
		}
		return;
	}
	else
		return;
}

void CDlgAutoMountain::OnCommandOpenHeightCurveDlg(const char* szCommand)
{
	GetHomeDlgsMgr()->GetCurveDlg()->Attach(m_pCurveHeight, true);
	GetHomeDlgsMgr()->GetCurveDlg()->Show(true);
}
void CDlgAutoMountain::OnCommandOpenLineCurveDlg(const char* szCommand)
{
	GetHomeDlgsMgr()->GetCurveDlg()->Attach(m_pCurveLine, false);
	GetHomeDlgsMgr()->GetCurveDlg()->Show(true);

}

void CDlgAutoMountain::OnCommandOpenChooseTextureDlg(const char* szCommand)
{
	CDlgAutoChooseTexture* pDlg = GetHomeDlgsMgr()->GetAutoChooseTextureDlg();
	pDlg->InitTextureParamInfo(m_texParamInfo, CTIP_HILL);
	pDlg->Show(true);

}

void CDlgAutoMountain::OnCommandOnRadio(const char* szCommand)
{
	UpdateData(true);
	GetAutoScene()->GetAutoTerrain()->SetUpDownPri(m_nUpDownPri);
}

void CDlgAutoMountain::OnCommandClearAllHill(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	CAutoTerrain* pTerrain = pScene->GetAutoTerrain();
	CAutoTexture* pTexture = pTerrain->GetTexture();
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	
	pTerrain->RestoreBaseTerrain();
	pAction->ClearAllHillActions();

	pTexture->ResetHillTexture();

	pScene->RecreateTerrainRender(true);

	m_bChangeHillLine = true;
	m_bChangeHillPoly = true;
}

void CDlgAutoMountain::OnCommandRefresh(const char * szCommand)
{
	UpdateData(true);
	CAutoScene* pScene = GetAutoScene();
	pScene->GetAutoTerrain()->UpdateAbsHeight();

	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	pTexture->RestoreBridgeMask();

	// Clear hill bridge texture on down area.
	if (2 == m_nUpDownPri)// Down first
	{
		pTexture->ClearBridgeAtDownArea();
	}
	
	// Save mask files
	pTexture->SaveAndReloadMask(TT_BRIDGE);
	pTexture->SaveAndReloadMask(TT_BRIDGE_PATCH);

	pScene->RecreateTerrainRender(true);
}