/*
 * FILE: DlgAutoRoad.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoRoad.h"
#include "EC_HomeDlgsMgr.h"
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

AUI_BEGIN_COMMAND_MAP(CDlgAutoRoad, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OpenChooseTexture", OnCommandOpenChooseTexDlg)
AUI_ON_COMMAND("EditRoad", OnCommandEditRoad)
AUI_ON_COMMAND("EditFlat", OnCommandEditFlat)
AUI_ON_COMMAND("PerspectiveView", OnCommandPerspectiveView)
AUI_ON_COMMAND("CreateRoad", OnCommandCreateRoad)
AUI_ON_COMMAND("CreateFlat", OnCommandCreateFlat)
AUI_ON_COMMAND("ClearAllRoad", OnCommandClearAllRoad)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoRoad, CDlgHomeBase)

AUI_END_EVENT_MAP()


CDlgAutoRoad::CDlgAutoRoad()
{
	m_pSliderMaxRoadHeight = NULL;
	m_pSliderMainSmoothRoad = NULL;
	m_pSliderPartSmoothRoad = NULL;
	m_pSliderRoadWidth = NULL;
	m_pSliderSeedRoad = NULL;

	m_pLabelMaxRoadHeight = NULL;
	m_pLabelMainSmoothRoad = NULL;
	m_pLabelPartSmoothRoad = NULL;
	m_pLabelRoadWidth = NULL;
	m_pLabelSeedRoad = NULL;
	
	m_nMaxRoadHeight = 0;
	m_nMainSmoothRoad = 0;
	m_nPartSmoothRoad = 0;
	m_nRoadWidth = 0;
	m_nSeedRoad = 0;

	m_bChangeRoad = true;
}

CDlgAutoRoad::~CDlgAutoRoad()
{
}

bool CDlgAutoRoad::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	return true;
}

void CDlgAutoRoad::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();

	SetCanMove(false);
}

AUIStillImageButton * CDlgAutoRoad::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseRoad;
}

void CDlgAutoRoad::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_MaxRoadHeight", m_pSliderMaxRoadHeight);
	DDX_Control("Slider_MainSmoothRoad", m_pSliderMainSmoothRoad);
	DDX_Control("Slider_PartSmoothRoad", m_pSliderPartSmoothRoad);
	DDX_Control("Slider_RoadWidth", m_pSliderRoadWidth);
	DDX_Control("Slider_SeedRoad", m_pSliderSeedRoad);

	DDX_Control("Label_MaxRoadHeight", m_pLabelMaxRoadHeight);
	DDX_Control("Label_MainSmoothRoad", m_pLabelMainSmoothRoad);
	DDX_Control("Label_PartSmoothRoad", m_pLabelPartSmoothRoad);
	DDX_Control("Label_RoadWidth", m_pLabelRoadWidth);
	DDX_Control("Label_SeedRoad", m_pLabelSeedRoad);

	DDX_Slider(bSave, "Slider_MaxRoadHeight", m_nMaxRoadHeight);
	DDX_Slider(bSave, "Slider_MainSmoothRoad", m_nMainSmoothRoad);
	DDX_Slider(bSave, "Slider_PartSmoothRoad", m_nPartSmoothRoad);
	DDX_Slider(bSave, "Slider_RoadWidth", m_nRoadWidth);
	DDX_Slider(bSave, "Slider_SeedRoad", m_nSeedRoad);

	DDX_CheckBox(bSave, "Check_FlatRoad", m_bCheckFlatRoad);

}
void CDlgAutoRoad::SetSliderBound()
{
	m_pSliderMaxRoadHeight->SetTotal(20, 5);
	m_pSliderMainSmoothRoad->SetTotal(91, 1);
	m_pSliderPartSmoothRoad->SetTotal(100, 0);
	m_pSliderRoadWidth->SetTotal(30, 10);
	m_pSliderSeedRoad->SetTotal(100, 0);
}
void CDlgAutoRoad::LoadDefault()
{
	m_nMaxRoadHeight = 10;
	m_nMainSmoothRoad = 50;
	m_nPartSmoothRoad = 20;
	m_nRoadWidth = 20;
	m_nSeedRoad = 1;

	m_texParamInfo.fTextureRatio	= 0.2f;
	m_texParamInfo.fMainTexture		= 0.4f;
	m_texParamInfo.fPatchTexture	= 1.2f;
	m_texParamInfo.fPatchDivision	= 30.0f;
	m_texParamInfo.dwTextureSeed	= 1;

	m_bCheckFlatRoad = false;
	GetHomeDlgsMgr()->LoadTextureParamInfo(m_texParamInfo, TT_ROADBED, TT_ROAD);

}
void CDlgAutoRoad::SetLabelsText()
{
	ACHAR szName[100];
	
	swprintf(szName, _AL("%d"), m_nMaxRoadHeight);
	m_pLabelMaxRoadHeight->SetText(szName);

	swprintf(szName, _AL("%d"), m_nMainSmoothRoad);
	m_pLabelMainSmoothRoad->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nPartSmoothRoad * 0.01f));
	m_pLabelPartSmoothRoad->SetText(szName);

	swprintf(szName, _AL("%d"), m_nRoadWidth);
	m_pLabelRoadWidth->SetText(szName);

	swprintf(szName, _AL("%d"), m_nSeedRoad);
	m_pLabelSeedRoad->SetText(szName);

}

void CDlgAutoRoad::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoRoad::OnCommandOpenChooseTexDlg(const char* szCommand)
{
	CDlgAutoChooseTexture* pDlg = GetHomeDlgsMgr()->GetAutoChooseTextureDlg();
	pDlg->InitTextureParamInfo(m_texParamInfo, CTIP_ROAD);
	pDlg->Show(true);
}

void CDlgAutoRoad::OnCommandEditRoad(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	pScene->SetOperationType(OT_ROAD);
}

void CDlgAutoRoad::OnCommandEditFlat(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_TERRAIN_FLAT);
}

void CDlgAutoRoad::OnLButtonDownScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_ROAD)
	{
		if( pAutoScene->GetAutoRoad())
		{
			pAutoScene->GetAutoRoad()->OnLButtonDown(NULL, point);
			m_bChangeRoad = true;
		}

		return;
	}
	else if( nType == OT_TERRAIN_FLAT)
	{
		if(pAutoScene->GetAutoTerrainFlat())
			pAutoScene->GetAutoTerrainFlat()->OnLButtonDown(NULL, point);
		return;
	}
	else
		return;
}
void CDlgAutoRoad::OnRButtonUpScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_ROAD)
	{
		if( pAutoScene->GetAutoRoad())
		{
			pAutoScene->GetAutoRoad()->OnRButtonUp(NULL, point);
			m_bChangeRoad = true;
		}

		return;
	}
	else if( nType == OT_TERRAIN_FLAT)
	{
		if(pAutoScene->GetAutoTerrainFlat())
			pAutoScene->GetAutoTerrainFlat()->OnRButtonUp(NULL, point);
		return;
	}
	else
		return;
}
void CDlgAutoRoad::OnRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_ROAD)
	{
		if(pAutoScene->GetAutoRoad())
		{
			pAutoScene->GetAutoRoad()->OnRButtonDblClk(NULL, point);
			m_bChangeRoad = true;
		}
		return;
	}
	else if( nType == OT_TERRAIN_FLAT)
	{
		if(pAutoScene->GetAutoTerrainFlat())
			pAutoScene->GetAutoTerrainFlat()->OnRButtonDblClk(NULL, point);
		return;
	}
	else
		return;
}

void CDlgAutoRoad::OnCommandPerspectiveView(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_NORMAL);
}

void CDlgAutoRoad::OnCommandCreateRoad(const char* szCommand)
{
	if (GetOperationType() != OT_ROAD)
	{
		GetAUIManager()->MessageBox("错误", _AL("请先点设置路!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoRoad* pRoad = pScene->GetAutoRoad();
	if (NULL == pRoad)
		return;

	if (!pRoad->IsPointsValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择路径!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if( !m_bChangeRoad)
	{
		pRoad->StorePoints();
		pScene->UndoAction(TAT_ROAD);
		pRoad->RestorePoints();
	}
	
	if (!pRoad->CreateRoad((float)m_nRoadWidth, (float)m_nMaxRoadHeight, (float)m_nMainSmoothRoad, (float) (m_nPartSmoothRoad * 0.01f), m_nSeedRoad, m_bCheckFlatRoad))
	{
		GetAUIManager()->MessageBox("错误", _AL("开路失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	// Store action
	
	if (!pAction->AddActionRoad(
			(float)m_nRoadWidth,
			(float)m_nMaxRoadHeight,
			(float)m_nMainSmoothRoad,
			(float)(m_nPartSmoothRoad * 0.01f),
			m_nSeedRoad,
			pRoad->GetNumPoints(),
			pRoad->GetPoints(),
			m_bCheckFlatRoad ? 1 : 0))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作数据失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	CAutoTexture* pTexture = pScene->GetAutoTerrain()->GetTexture();
	if (!pAction->AddActionRoadTextureInfo(
			m_texParamInfo.fMainTextureU,
			m_texParamInfo.fMainTextureV,
			m_texParamInfo.fPatchTextureU,
			m_texParamInfo.fPatchTextureV,
			pTexture->GetTextureID(TT_ROADBED),
			pTexture->GetTextureID(TT_ROAD)))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	pScene->RecreateTerrainRender(true);

	m_bChangeRoad = false;

}
void CDlgAutoRoad::OnCommandClearAllRoad(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	CAutoTerrain* pTerrain = pScene->GetAutoTerrain();
	CAutoTexture* pTexture = pTerrain->GetTexture();

	// Clear road action
	pScene->GetTerrainAction()->ClearAllRoadActions();

	// Clear height
	CAutoTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();
	::ZeroMemory(pHeightMap->pAbsHeightRoad,
		pHeightMap->iWidth*pHeightMap->iHeight*sizeof(float));
	int i, j;
	int nPos;
	for (i=0; i<pHeightMap->iHeight; i++)
	{
		for (j=0; j<pHeightMap->iWidth; j++)
		{
			nPos = i * pHeightMap->iWidth + j;
			pTerrain->UpdateAbsHeightAtPos(nPos);
		}
	}
	if (CAutoTerrain::UDP_BOTH != pTerrain->GetUpDownPri())
		pTerrain->BlurArea(pTerrain->GetHeightMap()->pAbsHeight, pTerrain->GetHeightMap()->iWidth, pTerrain->GetHeightMap()->iHeight);
	pTerrain->CreateHeightData((float)MAX_HEIGHT);
	ARectF rect = pTerrain->GetTerrainRect();
	pTerrain->UpdateVertexNormals(rect);
	pTerrain->SetNeedRefreshHeight(true);

	// Save mask files
	pTexture->ResetRoadTexture();

	pScene->RecreateTerrainRender(true);

	m_bChangeRoad = true;
}
void CDlgAutoRoad::OnCommandCreateFlat(const char* szCommand)
{
	if (GetOperationType() != OT_TERRAIN_FLAT)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于平整地面操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoTerrainFlat* pAutoTerrainFlat = pScene->GetAutoTerrainFlat();
	if (NULL == pAutoTerrainFlat)
		return;

	if (!pAutoTerrainFlat->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择封闭的多边形区域!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	if (!pAutoTerrainFlat->FlatTerrain())
	{
		GetAUIManager()->MessageBox("错误", _AL("Sorry. 不知道怎么搞的，拍不平啊!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	// Store action
	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if (!pAction->AddActionTerrainFlat(
		pAutoTerrainFlat->GetArea()->GetNumPoints(),
		pAutoTerrainFlat->GetArea()->GetFinalPoints()))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	pScene->RecreateTerrainRender(true);

	// Update buildings
	pScene->GetAutoTerrain()->Tick(0);// Update terrain height
	CAutoBuildingOperation* pBuildingOperation = pScene->GetAutoBuildingOperation();
	pBuildingOperation->AdjustModelsAfterTerrainChange();
}