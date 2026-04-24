/*
 * FILE: DlgAutoPlant.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoPlant.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoChoosePlant.h"

#include "aui/AUIManager.h"
#include "aui/AUISlider.h"
#include "aui/AUILabel.h"
#include "aui/AUIListBox.h"

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

AUI_BEGIN_COMMAND_MAP(CDlgAutoPlant, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OpenChooseTreeDlg", OnCommandOpenChooseTreeDlg)
AUI_ON_COMMAND("OpenChooseLandGrassDlg", OnCommandOpenChooseLandGrassDlg)
AUI_ON_COMMAND("OpenChooseWaterGrassDlg", OnCommandOpenChooseWaterGrassDlg)

AUI_ON_COMMAND("EditTreePoly", OnCommandEditTreePoly)
AUI_ON_COMMAND("EditTreeLine", OnCommandEditTreeLine)
AUI_ON_COMMAND("EditGrass", OnCommandEditGrass)
AUI_ON_COMMAND("PerspectiveView", OnCommandPerspectiveView)

AUI_ON_COMMAND("ClearAllTreeInfo", OnCommandClearAllTreeInfo)
AUI_ON_COMMAND("ClearAllLandGrassInfo", OnCommandClearAllLandGrassInfo)
AUI_ON_COMMAND("ClearAllWaterGrassInfo", OnCommandClearAllWaterGrassInfo)

AUI_ON_COMMAND("DelTreeInfo", OnCommandDelTreeInfo)
AUI_ON_COMMAND("DelLandGrassInfo", OnCommandDelLandGrassInfo)
AUI_ON_COMMAND("DelWaterGrassInfo", OnCommandDelWaterGrassInfo)

AUI_ON_COMMAND("CreateTreePoly", OnCommandCreateTreePoly)
AUI_ON_COMMAND("CreateTreeLine", OnCommandCreateTreeLine)
AUI_ON_COMMAND("CreateLandGrass", OnCommandCreateLandGrass)
AUI_ON_COMMAND("CreateWaterGrass", OnCommandCreateWaterGrass)

AUI_ON_COMMAND("ClearAllTrees", OnCommandClearAllTrees)
AUI_ON_COMMAND("ClearAllGrass", OnCommandClearAllGrass)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoPlant, CDlgHomeBase)

AUI_END_EVENT_MAP()


CDlgAutoPlant::CDlgAutoPlant()
{
	m_pSliderTreeSeed = NULL;
	m_pSliderTreeDensity = NULL;
	m_pSliderTreeSlope = NULL;
	m_pSliderTreeSpaceX = NULL;
	m_pSliderTreeSpaceZ = NULL;
	m_pSliderTreeSpaceLine = NULL;
	m_pSliderTreeSpaceNoise = NULL;
	m_pSliderGrassDensity = NULL;
	m_pSliderGrassSeed = NULL;

	m_pLabelTreeSeed = NULL;
	m_pLabelTreeDensity = NULL;
	m_pLabelTreeSlope = NULL;
	m_pLabelTreeSpaceX = NULL;
	m_pLabelTreeSpaceZ = NULL;
	m_pLabelTreeSpaceLine = NULL;
	m_pLabelTreeSpaceNoise = NULL;
	m_pLabelGrassDensity = NULL;
	m_pLabelGrassSeed = NULL;

	m_nTreeSeed = 0;
	m_nTreeDensity = 0;
	m_nTreeSlope = 0;
	m_nTreeSpaceX = 0;
	m_nTreeSpaceZ = 0;
	m_nTreeSpaceLine = 0;
	m_nTreeSpaceNoise = 0;
	m_nGrassDensity = 0;
	m_nGrassSeed = 0;

	m_pListBoxTrees = NULL;
	m_pListBoxLandGrass = NULL;
	m_pListBoxWaterGrass = NULL;

	m_bCheckTreeRegular = false;

	m_bChangeForestPoly = true;
	m_bChangeForestLine = true;
	m_bChangeGrassPoly = true;
}

CDlgAutoPlant::~CDlgAutoPlant()
{
}

bool CDlgAutoPlant::OnInitContext()
{
	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();

	return true;
}

void CDlgAutoPlant::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();
	SetCanMove(false);
}

AUIStillImageButton * CDlgAutoPlant::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChoosePlant;
}

void CDlgAutoPlant::DoDataExchange(bool bSave)
{
	DDX_Control("Slider_TreeSeed", m_pSliderTreeSeed);
	DDX_Control("Slider_TreeDensity", m_pSliderTreeDensity);
	DDX_Control("Slider_TreeSlope", m_pSliderTreeSlope);
	DDX_Control("Slider_TreeSpaceX", m_pSliderTreeSpaceX);
	DDX_Control("Slider_TreeSpaceZ", m_pSliderTreeSpaceZ);
	DDX_Control("Slider_TreeSpaceLine", m_pSliderTreeSpaceLine);
	DDX_Control("Slider_TreeSpaceNoise", m_pSliderTreeSpaceNoise);
	DDX_Control("Slider_GrassDensity", m_pSliderGrassDensity);
	DDX_Control("Slider_GrassSeed", m_pSliderGrassSeed);

	DDX_Control("Label_TreeSeed", m_pLabelTreeSeed);
	DDX_Control("Label_TreeDensity", m_pLabelTreeDensity);
	DDX_Control("Label_TreeSlope", m_pLabelTreeSlope);
	DDX_Control("Label_TreeSpaceX", m_pLabelTreeSpaceX);
	DDX_Control("Label_TreeSpaceZ", m_pLabelTreeSpaceZ);
	DDX_Control("Label_TreeSpaceLine", m_pLabelTreeSpaceLine);
	DDX_Control("Label_TreeSpaceNoise", m_pLabelTreeSpaceNoise);
	DDX_Control("Label_GrassDensity", m_pLabelGrassDensity);
	DDX_Control("Label_GrassSeed", m_pLabelGrassSeed);
	
	DDX_Slider(bSave, "Slider_TreeSeed", m_nTreeSeed);
	DDX_Slider(bSave, "Slider_TreeDensity", m_nTreeDensity);
	DDX_Slider(bSave, "Slider_TreeSlope", m_nTreeSlope);
	DDX_Slider(bSave, "Slider_TreeSpaceX", m_nTreeSpaceX);
	DDX_Slider(bSave, "Slider_TreeSpaceZ", m_nTreeSpaceZ);
	DDX_Slider(bSave, "Slider_TreeSpaceLine", m_nTreeSpaceLine);
	DDX_Slider(bSave, "Slider_TreeSpaceNoise", m_nTreeSpaceNoise);
	DDX_Slider(bSave, "Slider_GrassDensity", m_nGrassDensity);
	DDX_Slider(bSave, "Slider_GrassSeed", m_nGrassSeed);

	DDX_Control("ListBox_Trees", m_pListBoxTrees);
	DDX_Control("ListBox_LandGrass", m_pListBoxLandGrass);
	DDX_Control("ListBox_WaterGrass", m_pListBoxWaterGrass);

	DDX_CheckBox(bSave, "Check_TreeRegular", m_bCheckTreeRegular);
	
}

void CDlgAutoPlant::SetSliderBound()
{
	m_pSliderTreeSeed->SetTotal(100, 0);
	m_pSliderTreeDensity->SetTotal(100, 0); //* 0.01f
	m_pSliderTreeSlope->SetTotal(900, 0); // * 0.1f
	m_pSliderTreeSpaceX->SetTotal(50, 10);
	m_pSliderTreeSpaceZ->SetTotal(50, 10);
	m_pSliderTreeSpaceLine->SetTotal(50, 10);
	m_pSliderTreeSpaceNoise->SetTotal(50, 1);
	m_pSliderGrassDensity->SetTotal(100, 0); // * 0.01f
	m_pSliderGrassSeed->SetTotal(100, 0);
}
void CDlgAutoPlant::LoadDefault()
{
	m_nTreeSeed = 1;
	m_nTreeDensity = 10;
	m_nTreeSlope = 200;
	m_nTreeSpaceX = 20;
	m_nTreeSpaceZ = 20;
	m_nTreeSpaceLine = 15;
	m_nTreeSpaceNoise = 3;
	m_nGrassDensity = 20;
	m_nGrassSeed = 1;

	m_bCheckTreeRegular = false;
}
void CDlgAutoPlant::SetLabelsText()
{
	ACHAR szName[100];

	swprintf(szName, _AL("%d"), m_nTreeSeed);
	m_pLabelTreeSeed->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nTreeDensity * 0.01f));
	m_pLabelTreeDensity->SetText(szName);

	swprintf(szName, _AL("%-4.1f"), (float)(m_nTreeSlope * 0.1f));
	m_pLabelTreeSlope->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTreeSpaceX);
	m_pLabelTreeSpaceX->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTreeSpaceZ);
	m_pLabelTreeSpaceZ->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTreeSpaceLine);
	m_pLabelTreeSpaceLine->SetText(szName);

	swprintf(szName, _AL("%d"), m_nTreeSpaceNoise);
	m_pLabelTreeSpaceNoise->SetText(szName);

	swprintf(szName, _AL("%-4.2f"), (float)(m_nGrassDensity * 0.01f));
	m_pLabelGrassDensity->SetText(szName);

	swprintf(szName, _AL("%d"), m_nGrassSeed);
	m_pLabelGrassSeed->SetText(szName);
}

void CDlgAutoPlant::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoPlant::OnCommandOpenChooseTreeDlg(const char* szCommand)
{
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->InitChoosePlant(CDlgAutoChoosePlant::PT_TREE);
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->Show(true);
}
void CDlgAutoPlant::OnCommandOpenChooseLandGrassDlg(const char* szCommand)
{
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->InitChoosePlant(CDlgAutoChoosePlant::PT_LANDGRASS);
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->Show(true);
}
void CDlgAutoPlant::OnCommandOpenChooseWaterGrassDlg(const char* szCommand)
{
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->InitChoosePlant(CDlgAutoChoosePlant::PT_WATERGRASS);
	GetHomeDlgsMgr()->GetAutoChoosePlantDlg()->Show(true);
}

void CDlgAutoPlant::OnCommandEditTreePoly(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_TREE_POLY);
}
void CDlgAutoPlant::OnCommandEditTreeLine(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_TREE_LINE);
}
void CDlgAutoPlant::OnCommandEditGrass(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_GRASS_POLY);
}

void CDlgAutoPlant::OnCommandPerspectiveView(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_NORMAL);
}

void CDlgAutoPlant::OnLButtonDownScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TREE_POLY)
	{
		if (pAutoScene->GetAutoForestPoly())
		{
			pAutoScene->GetAutoForestPoly()->OnLButtonDown(NULL, point);
			m_bChangeForestPoly = true;
		}

		return;
	}
	else if( nType == OT_TREE_LINE)
	{
		if (pAutoScene->GetAutoForestLine())
		{
			pAutoScene->GetAutoForestLine()->OnLButtonDown(NULL, point);
			m_bChangeForestLine = true;
		}
		return;
	}
	else if( nType == OT_GRASS_POLY)
	{
		if (pAutoScene->GetAutoGrassPoly())
		{
			pAutoScene->GetAutoGrassPoly()->OnLButtonDown(NULL, point);
			m_bChangeGrassPoly = true;
		}
	}
	else
		return;
}

void CDlgAutoPlant::OnRButtonUpScreen(WPARAM wParam, LPARAM lParam)
{
		APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TREE_POLY)
	{
		if (pAutoScene->GetAutoForestPoly())
		{
			pAutoScene->GetAutoForestPoly()->OnRButtonUp(NULL, point);
			m_bChangeForestPoly = true;
		}
		return;
	}
	else if( nType == OT_TREE_LINE)
	{
		if (pAutoScene->GetAutoForestLine())
		{
			pAutoScene->GetAutoForestLine()->OnRButtonUp(NULL, point);
			m_bChangeForestLine = true;
		}
		return;
	}
	else if( nType == OT_GRASS_POLY)
	{
		if (pAutoScene->GetAutoGrassPoly())
		{
			pAutoScene->GetAutoGrassPoly()->OnRButtonUp(NULL, point);
			m_bChangeGrassPoly = true;
		}
	}
	else
		return;
}

void CDlgAutoPlant::OnRButtonDblClk(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	

	int nType = pAutoScene->GetOperationType();
	if( nType == OT_TREE_POLY)
	{
		if (pAutoScene->GetAutoForestPoly())
		{
			pAutoScene->GetAutoForestPoly()->OnRButtonDblClk(NULL, point);
			m_bChangeForestPoly = true;
		}
		return;
	}
	else if( nType == OT_TREE_LINE)
	{
		if (pAutoScene->GetAutoForestLine())
		{
			pAutoScene->GetAutoForestLine()->OnRButtonDblClk(NULL, point);
			m_bChangeForestLine = true;
		}
		return;
	}
	else if( nType == OT_GRASS_POLY)
	{
		if (pAutoScene->GetAutoGrassPoly())
		{
			pAutoScene->GetAutoGrassPoly()->OnRButtonDblClk(NULL, point);
			m_bChangeGrassPoly = true;
		}
	}
	else
		return;
}

void CDlgAutoPlant::ShowTreeInfos()
{
	CAutoForest* pForest = GetAutoScene()->GetAutoForest();
	CAutoForest::TREEINFO* pTypes = pForest->GetTreeInfos();

	m_pListBoxTrees->ResetContent();
	int nSize =  m_aTreeInfos.GetSize();

	for( int i = 0; i < nSize; i ++)
	{
		int nIdx = pForest->GetGlobalTreeIndexById(m_aTreeInfos[i].dwID);
		m_pListBoxTrees->AddString(AS2AC(pTypes[nIdx].strName));
	}
}

void CDlgAutoPlant::ShowLandGrassInfos()
{
	CAutoGrass* pGrass = GetAutoScene()->GetAutoGrass();
	CAutoGrass::GRASSINFO* pTypes = pGrass->GetLandGrassInfos();
	int nSize = m_aLandGrassInfos.GetSize();

	m_pListBoxLandGrass->ResetContent();
	
	for( int i = 0; i < nSize; i++)
	{
		int nIdx = pGrass->GetGlobalLandGrassIndexById(m_aLandGrassInfos[i].dwID);
		m_pListBoxLandGrass->AddString(AS2AC(pTypes[nIdx].strName));
	}
}

void CDlgAutoPlant::ShowWaterGrassInfos()
{
	CAutoGrass* pGrass = GetAutoScene()->GetAutoGrass();
	CAutoGrass::GRASSINFO* pTypes = pGrass->GetWaterGrassInfos();
	int nSize = m_aWaterGrassInfos.GetSize();
	int i;

	m_pListBoxWaterGrass->ResetContent();
	
	for (i=0; i<nSize; i++)
	{
		int nIdx = pGrass->GetGlobalWaterGrassIndexById(m_aWaterGrassInfos[i].dwID);
		m_pListBoxWaterGrass->AddString(AS2AC(pTypes[nIdx].strName));
	}
}

void CDlgAutoPlant::OnCommandClearAllTreeInfo(const char* szCommand)
{
	m_aTreeInfos.RemoveAll();
	m_pListBoxTrees->ResetContent();
}

void CDlgAutoPlant::OnCommandClearAllLandGrassInfo(const char* szCommand)
{
	m_aLandGrassInfos.RemoveAll();
	m_pListBoxLandGrass->ResetContent();
}

void CDlgAutoPlant::OnCommandClearAllWaterGrassInfo(const char* szCommand)
{
	m_aWaterGrassInfos.RemoveAll();
	m_pListBoxWaterGrass->ResetContent();
}


void CDlgAutoPlant::OnCommandDelTreeInfo(const char* szCommand)
{
	if( m_aTreeInfos.GetSize() == 0)
		return;

	int nIdx = m_pListBoxTrees->GetCurSel();
	if( nIdx < 0 || nIdx > m_pListBoxTrees->GetCount())
		return;
	m_aTreeInfos.RemoveAt(nIdx);
	ShowTreeInfos();

}

void CDlgAutoPlant::OnCommandDelLandGrassInfo(const char* szCommand)
{
	if( m_aLandGrassInfos.GetSize() == 0)
		return ;

	int nIdx = m_pListBoxLandGrass->GetCurSel();
	if( nIdx < 0 || nIdx > m_pListBoxLandGrass->GetCount())
		return;
	m_aLandGrassInfos.RemoveAt(nIdx);
	ShowLandGrassInfos();
}

void CDlgAutoPlant::OnCommandDelWaterGrassInfo(const char* szCommand)
{
	if(m_aWaterGrassInfos.GetSize() == 0)
		return;

	int nIdx = m_pListBoxWaterGrass->GetCurSel();
	if( nIdx < 0 || nIdx > m_pListBoxWaterGrass->GetCount())
		return;
	m_aWaterGrassInfos.RemoveAt(nIdx);
	ShowWaterGrassInfos();
}

void CDlgAutoPlant::OnCommandCreateTreePoly(const char* szCommand)
{
	UpdateData(true);

	

	if (GetOperationType() != OT_TREE_POLY)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于多边形区域树操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoForestPoly* pForestPoly = pScene->GetAutoForestPoly();
	if (NULL == pForestPoly)
		return;

	if (!pForestPoly->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择封闭的多边形区域!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	CAutoForest* pForest = pScene->GetAutoForest();

	int nTreeTypeCount = m_aTreeInfos.GetSize();
	if (nTreeTypeCount <= 0)
		return;
	
	DWORD* pIDs = new DWORD[nTreeTypeCount];
	float* pWeights = new float[nTreeTypeCount];
	for (int i=0; i<nTreeTypeCount; i++)
	{
		pIDs[i] = m_aTreeInfos[i].dwID;
		pWeights[i] = m_aTreeInfos[i].fWeight;
	}
	int nPlantType;
	float fDensityX, fDensityZ;
	if (m_bCheckTreeRegular)
	{
		nPlantType = CAutoForest::PT_REGULAR;
		fDensityX = float(m_nTreeSpaceX);
		fDensityZ = float(m_nTreeSpaceZ);
	}
	else
	{
		nPlantType = CAutoForest::PT_RANDOM;
		fDensityX = (float)(m_nTreeDensity * 0.01f);
		fDensityZ = (float)(m_nTreeDensity * 0.01f);
	}

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if(!m_bChangeForestPoly)
	{
		pForestPoly->StorePoints();
		pScene->UndoAction(TAT_FOREST_POLY);
		pForestPoly->RestorePoints();
	}

	if (!pForestPoly->CreateForestPoly(nPlantType, fDensityX, fDensityZ, (float)(m_nTreeSlope * 0.1f),
				nTreeTypeCount, pIDs, pWeights, m_nTreeSeed))
	{
		GetAUIManager()->MessageBox("错误", _AL("植树造林失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	// Store action
	
	CClosedArea* pArea = pForestPoly->GetArea();
	if (!pAction->AddActionForestPoly(
			nPlantType,
			fDensityX,
			fDensityZ,
			(float)(m_nTreeSlope * 0.1f),
			nTreeTypeCount,
			pIDs,
			pWeights,
			m_nTreeSeed,
			pArea->GetNumPoints(),
			pArea->GetFinalPoints()))

	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作数据失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	delete[] pIDs;
	delete[] pWeights;

	m_bChangeForestPoly = false;

}

void CDlgAutoPlant::OnCommandCreateTreeLine(const char* szCommand)
{
	UpdateData(true);

	if (GetOperationType() != OT_TREE_LINE)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于线形区域树操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoForestLine* pForestLine = pScene->GetAutoForestLine();
	if (NULL == pForestLine)
		return;

	if (!pForestLine->IsPointsValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("没有线怎么画啊!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if (float(m_nTreeSpaceLine) < float(m_nTreeSpaceNoise))
	{
		GetAUIManager()->MessageBox("错误", _AL("线间隔必须大于噪声!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	if (float(m_nTreeSpaceLine) <= 0)
	{
		GetAUIManager()->MessageBox("错误", _AL("线间隔必须大于0!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	if (float(m_nTreeSpaceNoise) < 0)
	{
		GetAUIManager()->MessageBox("错误", _AL("噪声不能小于0!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoForest* pForest = pScene->GetAutoForest();

	int nTreeTypeCount = m_aTreeInfos.GetSize();
	if (nTreeTypeCount <= 0)
		return;
	DWORD* pIDs = new DWORD[nTreeTypeCount];
	float* pWeights = new float[nTreeTypeCount];
	for (int i=0; i<nTreeTypeCount; i++)
	{
		pIDs[i] = m_aTreeInfos[i].dwID;
		pWeights[i] = m_aTreeInfos[i].fWeight;
	}

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if(!m_bChangeForestLine)
	{
		pForestLine->StorePoints();
		pScene->UndoAction(TAT_FOREST_LINE);
		pForestLine->RestorePoints();
	}

	if (!pForestLine->CreateForestLine((float)m_nTreeSpaceLine, (float)m_nTreeSpaceNoise, (float)(m_nTreeSlope * 0.1f),
				nTreeTypeCount, pIDs, pWeights, m_nTreeSeed))
	{
		GetAUIManager()->MessageBox("错误", _AL("植树造林失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	// Store action
	if (!pAction->AddActionForestLine(
			(float)m_nTreeSpaceLine, 
			(float)m_nTreeSpaceNoise, 
			(float)(m_nTreeSlope * 0.1f),
			nTreeTypeCount,
			pIDs,
			pWeights,
			m_nTreeSeed,
			pForestLine->GetNumPoints(),
			pForestLine->GetFinalPoints()))

	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作数据失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	delete[] pIDs;
	delete[] pWeights;

	m_bChangeForestLine = false;
}

void CDlgAutoPlant::OnCommandCreateLandGrass(const char* szCommand)
{
	UpdateData(true);

	if (GetOperationType() != OT_GRASS_POLY)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于多边形区域草操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoGrassPoly* pGrassPoly = pScene->GetAutoGrassPoly();
	if (NULL == pGrassPoly)
		return;

	if (!pGrassPoly->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择封闭的多边形区域!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	int nGrassTypeCount = m_aLandGrassInfos.GetSize();
	if (nGrassTypeCount <= 0)
		return;
	DWORD* pIDs = new DWORD[nGrassTypeCount];
	float* pWeights = new float[nGrassTypeCount];
	for (int i=0; i<nGrassTypeCount; i++)
	{
		pIDs[i] = m_aLandGrassInfos[i].dwID;
		pWeights[i] = m_aLandGrassInfos[i].fWeight;
	}

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if(!m_bChangeGrassPoly)
	{
		pGrassPoly->StorePoints();
		pScene->UndoAction(TAT_GRASS_POLY);
		pGrassPoly->RestorePoints();
	}

	if (!pGrassPoly->CreateGrassPoly(CAutoGrass::GK_LAND, (float)(m_nGrassDensity * 0.01f),
			nGrassTypeCount, pIDs, pWeights, m_nGrassSeed))
	{
		GetAUIManager()->MessageBox("错误", _AL("衰啊，种草失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	// Store action
	CAutoGrass* pGrass = pScene->GetAutoGrass();
	CClosedArea* pArea = pGrassPoly->GetArea();
	if (!pAction->AddActionGrassPoly(
			CAutoGrass::GK_LAND,
			(float)(m_nGrassDensity * 0.01f),
			nGrassTypeCount,
			pIDs,
			pWeights,
			m_nGrassSeed,
			pArea->GetNumPoints(),
			pArea->GetFinalPoints()))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作数据失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
		
	delete[] pIDs;
	delete[] pWeights;
}

void CDlgAutoPlant::OnCommandCreateWaterGrass(const char* szCommand)
{
	UpdateData(true);

	if (GetOperationType() != OT_GRASS_POLY)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于多边形区域草操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	CAutoScene* pScene = GetAutoScene();
	CAutoGrassPoly* pGrassPoly = pScene->GetAutoGrassPoly();
	if (NULL == pGrassPoly)
		return;

	if (!pGrassPoly->IsPolygonValid())
	{
		GetAUIManager()->MessageBox("错误", _AL("请选择封闭的多边形区域!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	int nGrassTypeCount = m_aWaterGrassInfos.GetSize();
	if (nGrassTypeCount <= 0)
		return;
	DWORD* pIDs = new DWORD[nGrassTypeCount];
	float* pWeights = new float[nGrassTypeCount];
	for (int i=0; i<nGrassTypeCount; i++)
	{
		pIDs[i] = m_aWaterGrassInfos[i].dwID;
		pWeights[i] = m_aWaterGrassInfos[i].fWeight;
	}

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	if(!m_bChangeGrassPoly)
	{
		pGrassPoly->StorePoints();
		pScene->UndoAction(TAT_GRASS_POLY);
		pGrassPoly->RestorePoints();
	}

	if (!pGrassPoly->CreateGrassPoly(CAutoGrass::GK_WATER, (float)(m_nGrassDensity * 0.01f),
			nGrassTypeCount, pIDs, pWeights, m_nGrassSeed))
	{
		GetAUIManager()->MessageBox("错误", _AL("衰啊，种草失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	// Store action
	CAutoGrass* pGrass = pScene->GetAutoGrass();
	CClosedArea* pArea = pGrassPoly->GetArea();
	if (!pAction->AddActionGrassPoly(
			CAutoGrass::GK_WATER,
			(float)(m_nGrassDensity * 0.01f),
			nGrassTypeCount,
			pIDs,
			pWeights,
			m_nGrassSeed,
			pArea->GetNumPoints(),
			pArea->GetFinalPoints()))
	{
		GetAUIManager()->MessageBox("错误", _AL("保存操作数据失败!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
		
	delete[] pIDs;
	delete[] pWeights;
}

void CDlgAutoPlant::OnCommandClearAllTrees(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	CAutoForest* pForest = pScene->GetAutoForest();
	pForest->DeleteAllTreeTypes();

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	pAction->ClearAllForestActions();

	m_bChangeForestLine = true;
	m_bChangeForestPoly = true;
}
void CDlgAutoPlant::OnCommandClearAllGrass(const char* szCommand)
{
	CAutoScene* pScene = GetAutoScene();
	CAutoGrass* pGrass = pScene->GetAutoGrass();
	pGrass->DeleteAllGrassTypes();

	CAutoTerrainAction* pAction = pScene->GetTerrainAction();
	pAction->ClearAllGrassActions();

	m_bChangeGrassPoly = true;
}