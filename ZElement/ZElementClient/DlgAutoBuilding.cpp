/*
 * FILE: DlgAutoBuilding.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoBuilding.h"
#include "EC_HomeDlgsMgr.h"
#include "DlgAutoChooseModel.h"

#include "aui/AUIManager.h"
#include "aui/AUIListBox.h"

#include "A3DMacros.h"
#include "A3DEngine.h"
#include "A3DViewport.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"

#include "windowsx.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoBuilding, CDlgHomeBase)

AUI_ON_COMMAND("OpenChooseModelDlg", OnCommandOpenChooseModelDlg)
AUI_ON_COMMAND("EditBuilding", OnCommandEditBuilding)
AUI_ON_COMMAND("PerspectiveView", OnCommandPerspectiveView)
AUI_ON_COMMAND("DelModelInfo", OnCommandDelModelInfo)
AUI_ON_COMMAND("ClearAllModelInfo", OnCommandClearAllModelInfo)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoBuilding, CDlgHomeBase)

AUI_END_EVENT_MAP()

CDlgAutoBuilding::CDlgAutoBuilding()
{
	m_pListBoxBuildings = NULL;
	m_nBuildingIdx = 0;
}

CDlgAutoBuilding::~CDlgAutoBuilding()
{
}

bool CDlgAutoBuilding::OnInitContext()
{
	return true;
}

void CDlgAutoBuilding::OnShowDialog()
{
	CDlgHomeBase::OnShowDialog();
		
	SetCanMove(false);

}

void CDlgAutoBuilding::DoDataExchange(bool bSave)
{
	DDX_Control("ListBox_Buildings", m_pListBoxBuildings);

	DDX_ListBox(bSave, "ListBox_Buildings", m_nBuildingIdx);
}

AUIStillImageButton * CDlgAutoBuilding::GetSwithButton()
{
	return GetHomeDlgsMgr()->m_pBtnChooseBuilding;
}


void CDlgAutoBuilding::OnCommandOpenChooseModelDlg(const char* szCommand)
{
	if (GetOperationType() != OT_BUILDING)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于建筑操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		return;
	}
	
	GetHomeDlgsMgr()->GetAutoChooseModelDlg()->Show(true);
}

void CDlgAutoBuilding::OnCommandDelModelInfo(const char* szCommand)
{

	UpdateData(true);

	if (m_nBuildingIdx < 0)
		return;
	
	if(m_aAddedModelId.GetSize() == 0)
		return;

	CAutoScene* pScene = GetAutoScene();

	CAutoBuildingOperation* pBuildingOpera = pScene->GetAutoBuildingOperation();
	if (pBuildingOpera->GetBuildingOpera() == CAutoBuildingOperation::BP_ADD)
	{
		GetAUIManager()->MessageBox("错误", _AL("已经选择了模型，请在地图上选择模型位置!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		return;
	}

	// Delete model in list
	m_aAddedModelId.RemoveAt(m_nBuildingIdx);

	// Delete real model
	pBuildingOpera->DeleteModel(m_nBuildingIdx);

	// Delete model collision
	pBuildingOpera->DeleteCollision(m_nBuildingIdx);

	UpdateModelInfos();
}

void CDlgAutoBuilding::OnCommandClearAllModelInfo(const char* szCommand)
{


	if (GetAutoScene()->GetAutoBuildingOperation()->GetBuildingOpera() == CAutoBuildingOperation::BP_ADD)
	{
		GetAUIManager()->MessageBox("错误", _AL("已经选择了模型，请在地图上选择模型位置!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	UpdateData(true);

	DeleteAllBuildings();

	UpdateModelInfos();
}

void CDlgAutoBuilding::OnCommandEditBuilding(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_BUILDING);
}

void CDlgAutoBuilding::OnCommandPerspectiveView(const char* szCommand)
{
	CAutoScene* pAutoScene = GetAutoScene();
	pAutoScene->SetOperationType(OT_NORMAL);
}

void CDlgAutoBuilding::ShowModelInfos()
{
	CAutoScene* pScene = GetAutoScene();
	CAutoBuilding* pBuilding = pScene->GetAutoBuilding();
	CAutoBuilding::MODELINFO* pTypes = pBuilding->GetModelInfos();
	int nCount = m_aAddedModelId.GetSize();
	int i;
	int nIndex;

	m_pListBoxBuildings->ResetContent();
	AString strModel;
	CAutoBuildingOperation* pBuildingOpera =pScene->GetAutoBuildingOperation();
	for (i=0; i<nCount; i++)
	{
		nIndex = pBuilding->GetGlobalModelIndexById(m_aAddedModelId[i]);
		strModel = pTypes[nIndex].strName;
		if (CAutoBuildingOperation::BP_ADD != pBuildingOpera->GetBuildingOpera())
		{
			if (pBuildingOpera->IsInCollision(i))
				strModel = "*" + strModel;
		}
		m_pListBoxBuildings->AddString(AS2AC(strModel));
	}
}

bool CDlgAutoBuilding::UpdateModelInfos()
{
	CAutoScene* pScene = GetAutoScene();
	CAutoBuildingOperation* pBuildingOpera = pScene->GetAutoBuildingOperation();
	if (pBuildingOpera->GetModelFailed())
	{
		DeleteLastBuilding();
		pBuildingOpera->SetModelFailed(false);
	}
	ShowModelInfos();
	
	UpdateData(true);
	int nIndex = pBuildingOpera->GetSelectedModelIndex();
	if (nIndex >m_aAddedModelId.GetSize())
		return false;
	m_nBuildingIdx = nIndex;
	UpdateData(true);

	return true;
}

bool CDlgAutoBuilding::DeleteLastBuilding()
{
	int nSize = m_aAddedModelId.GetSize();
	if (nSize <= 0)
		return false;
	m_aAddedModelId.RemoveAt(nSize-1);
	ShowModelInfos();	
	CAutoBuildingOperation* pBuildingOpera = GetAutoScene()->GetAutoBuildingOperation();
	pBuildingOpera->SetSelectedModelIndex(-1);

	return true;
}

bool CDlgAutoBuilding::DeleteAllBuildings()
{
	m_aAddedModelId.RemoveAll();

	CAutoScene* pScene = GetAutoScene();

	// Delete real model
	CAutoBuilding* pBuilding = pScene->GetAutoBuilding();
	CAutoBuildingOperation* pBuildingOpera = pScene->GetAutoBuildingOperation();
	pBuildingOpera->DeleteAllModels();
	pBuildingOpera->DeleteAllCollisions();

	return true;
}

void CDlgAutoBuilding::OnLButtonDownScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();
	
	int nType = pAutoScene->GetOperationType();
	if( nType == OT_BUILDING)
	{
		if (pAutoScene->GetAutoBuildingOperation())
		{
//			this->SetCapture();
//			UpdateCoordinate(point.x, point.y, nFlags);
//			m_ptLastMou.x = point.x;
//			m_ptLastMou.y = point.y;
//			m_fRotateRad = 0;
//			m_vAxis = A3DVECTOR3(0.0f);
//			if (IsOnCoordinate())// If on coordinate, don't select model
//				break;
			pAutoScene->GetAutoBuildingOperation()->OnLButtonDown(NULL, point);
			UpdateModelInfos();
//			CAutoBuildingOperation* pBuildingOpera = pAutoScene->GetAutoBuildingOperation();
//			int nModelIndex = pBuildingOpera->GetSelectedModelIndex();
//			if (nModelIndex >= 0)
//				SetCoordinatePosBySelectedModel();
		}
	}
	else
		return;
}

void CDlgAutoBuilding::OnLButtonUpScreen(WPARAM wParam, LPARAM lParam)
{
	APointI point;

	A3DVIEWPORTPARAM *p = g_pGame->GetA3DEngine()->GetActiveViewport()->GetParam();
	point.x = GET_X_LPARAM(lParam)  - p->X;
	point.y = GET_Y_LPARAM(lParam)  - p->Y;

	CAutoScene* pAutoScene = GetAutoScene();

	int nType = pAutoScene->GetOperationType();
	if (nType == OT_BUILDING)
	{

//		::ReleaseCapture();
//		m_Coordinate.OnLButtonUp(point.x, point.y, nFlags);
		CAutoBuildingOperation* pBuildingOpera = pAutoScene->GetAutoBuildingOperation();
		pBuildingOpera->OnLButtonUp(NULL, point);
		UpdateModelInfos();
	}
}