/*
 * FILE: DlgAutoChooseModel.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoChooseModel.h"
#include "DlgAutoBuilding.h"

#include "aui/auiListBox.h"
#include "aui/AUIManager.h"

#include "A3DMacros.h"


AUI_BEGIN_COMMAND_MAP(CDlgAutoChooseModel, CDlgHomeBase)

AUI_ON_COMMAND("OK", OnCommandOK)
AUI_ON_COMMAND("Cancel", OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAutoChooseModel, CDlgHomeBase)

AUI_END_EVENT_MAP()


CDlgAutoChooseModel::CDlgAutoChooseModel()
{
	m_pListBoxModels = NULL;
	m_nModelIdx = 0;

	m_pModelInfos = NULL;
	m_nNumModelInfos = 0;
}

CDlgAutoChooseModel::~CDlgAutoChooseModel()
{
}

bool CDlgAutoChooseModel::OnInitContext()
{
	ClearListBox();
	return InitResource();
}

void CDlgAutoChooseModel::OnShowDialog()
{
	this->SetCanMove(false);
}

void CDlgAutoChooseModel::DoDataExchange(bool bSave)
{
	DDX_Control("ListBox_Models", m_pListBoxModels);
	DDX_ListBox(bSave,"ListBox_Models", m_nModelIdx);
}

bool CDlgAutoChooseModel::InitResource()
{
	CAutoScene* pScene = GetAutoScene();
	if(!pScene)
		return false;

	CAutoBuilding* pBuilding = pScene->GetAutoBuilding();

	if( !pBuilding)
		return false;

	m_pModelInfos = pBuilding->GetModelInfos();
	m_nNumModelInfos = pBuilding->GetNumModelInfos();

	ClearListBox();

	for( int i = 0; i < m_nNumModelInfos; i ++)
	{
		m_pListBoxModels->InsertString(i, AS2AC(m_pModelInfos[i].strName));
	}
	return true;
}

void CDlgAutoChooseModel::ClearListBox()
{
	m_pListBoxModels->ResetContent();
}

void CDlgAutoChooseModel::OnCommandOK(const char* szCommand)
{
	if (GetOperationType() != OT_BUILDING)
	{
		GetAUIManager()->MessageBox("错误", _AL("当前不处于建筑操作!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		return;
	}
	
	CAutoScene* pScene = GetAutoScene();
	if (pScene->GetAutoBuildingOperation()->GetBuildingOpera() == CAutoBuildingOperation::BP_ADD)
	{
		GetAUIManager()->MessageBox("错误", _AL("已经选择了模型，请在地图上选择模型位置!"), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	CAutoBuilding* pBuilding = pScene->GetAutoBuilding();
	CAutoBuildingOperation* pBuildingOpera = pScene->GetAutoBuildingOperation();
	pBuildingOpera->SetSelectedModelIndex(-1);
	// Clear contained models
	pBuildingOpera->SelectContainedModels();

	int nIdx = m_pListBoxModels->GetCurSel();
	DWORD dwID;
	if( nIdx < 0 || nIdx >= m_pListBoxModels->GetCount())
		return;
	dwID = m_pModelInfos[nIdx].dwID;
	
	pBuildingOpera->SetBuildingOpera(CAutoBuildingOperation::BP_ADD);
	pBuildingOpera->SetCurModelID(dwID);
	
	GetHomeDlgsMgr()->GetAutoBuildingDlg()->GetAddedModelID()->Add(dwID);
	GetHomeDlgsMgr()->GetAutoBuildingDlg()->UpdateModelInfos();
	
	this->Show(false);
}

void CDlgAutoChooseModel::OnCommandCancel(const char* szCommand)
{
	this->Show(false);
}