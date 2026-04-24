/*
 * FILE: DlgAutoChoosePlant.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/16
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "DlgAutoChoosePlant.h"
#include "DlgAutoPlant.h"

#include "aui/AUILabel.h"
#include "aui/AUIListBox.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoChoosePlant, CDlgHomeBase)

AUI_ON_COMMAND("MoveSlider", OnCommandMoveSlider)
AUI_ON_COMMAND("OK", OnCommandOK)
AUI_ON_COMMAND("Cancel", OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgAutoChoosePlant, CDlgHomeBase)

AUI_END_EVENT_MAP()

CDlgAutoChoosePlant::CDlgAutoChoosePlant()
{
	m_pSliderWeight = NULL;
	m_nWeight = 0;
	m_fWeight = 0.0f;
	m_pLabelWeight = NULL;
	m_pListBoxPlants = NULL;

	m_PlantType = PT_NULL;

	m_pLandGrassInfo = NULL;
	m_nNumLandGrassInfos = 0;
	m_pWaterGrassInfo = NULL;
	m_nNumWaterGrassInfos = 0;

	m_pTreeInfo = NULL;
	m_nNumTreeInfos = 0;

	m_PT = PT_NULL;
}

CDlgAutoChoosePlant::~CDlgAutoChoosePlant()
{
}

void CDlgAutoChoosePlant::DoDataExchange(bool bSave)
{
	DDX_Control("ListBox_Plants", m_pListBoxPlants);
	DDX_Control("Label_Weight", m_pLabelWeight);
	DDX_Control("Slider_Weight", m_pSliderWeight);
	DDX_Slider(bSave, "Slider_Weight", m_nWeight);

	m_fWeight = float(m_nWeight * 0.01f);
}

bool CDlgAutoChoosePlant::OnInitContext()
{
	ClearListBox();

	if( !InitResource())
		return false;

	SetSliderBound();
	LoadDefault();
	UpdateData(false);
	SetLabelsText();
	
	return true;
}

void CDlgAutoChoosePlant::OnShowDialog()
{
	SetCanMove(false);
}

void CDlgAutoChoosePlant::SetSliderBound()
{
	m_pSliderWeight->SetTotal(100, 1);
}

void CDlgAutoChoosePlant::LoadDefault()
{
	m_nWeight = 100;
}

void CDlgAutoChoosePlant::SetLabelsText()
{
	ACHAR szName[100];
	swprintf(szName, _AL("%-4.2f"), m_fWeight);
	m_pLabelWeight->SetText(szName);
}

bool CDlgAutoChoosePlant::InitResource()
{
	CAutoScene* pScene = GetAutoScene();
	if( !pScene)
		return false;

	CAutoForest* pForest = pScene->GetAutoForest();
	if( !pForest)
		return false;

	m_pTreeInfo = pForest->GetTreeInfos();
	m_nNumTreeInfos = pForest->GetNumTreeInfos();

	CAutoGrass* pGrass = pScene->GetAutoGrass();
	if( !pGrass)
		return false;

	m_pLandGrassInfo = pGrass->GetLandGrassInfos();
	m_nNumLandGrassInfos = pGrass->GetNumLandGrassInfos();

	m_pWaterGrassInfo = pGrass->GetWaterGrassInfos();
	m_nNumWaterGrassInfos = pGrass->GetNumWaterGrassInfos();

	return true;
}

void CDlgAutoChoosePlant::ClearListBox()
{
	if( m_pListBoxPlants)
		m_pListBoxPlants->ResetContent();
}

void CDlgAutoChoosePlant::FillListBox(PlantType_e pt)
{
	ClearListBox();
	
	int i = 0;
	switch(pt)
	{
	case PT_TREE:
		{
			for( i = 0; i < m_nNumTreeInfos; i++)
			{
				m_pListBoxPlants->InsertString(i, AS2AC(m_pTreeInfo[i].strName));
			}
		}
		break;
	case PT_WATERGRASS:
		{
			for( i = 0; i< m_nNumWaterGrassInfos; i++)
			{
				m_pListBoxPlants->InsertString(i, AS2AC(m_pWaterGrassInfo[i].strName));
			}
		}
		break;
	case PT_LANDGRASS:
		{
			for( i = 0; i < m_nNumLandGrassInfos; i++)
			{
				m_pListBoxPlants->InsertString(i, AS2AC(m_pLandGrassInfo[i].strName));
			}
		}
		break;
	default:
		break;
	}

}

void CDlgAutoChoosePlant::OnCommandMoveSlider(const char * szCommand)
{
	UpdateData(true);
	SetLabelsText();
}

void CDlgAutoChoosePlant::InitChoosePlant(PlantType_e pt)
{
	FillListBox(pt);
	m_PT = pt;
}

void CDlgAutoChoosePlant::OnCommandOK(const char* szCommand)
{
	UpdateData(true);

	PLANTIDWEIGHTINFO info;
	int nIdx = m_pListBoxPlants->GetCurSel();

	switch( m_PT)
	{
	case PT_TREE:
		{
			info.dwID = m_pTreeInfo[nIdx].dwID;
			info.fWeight = m_fWeight;
			GetHomeDlgsMgr()->GetAutoPlantDlg()->GetTreeInfosArray()->Add(info);
			GetHomeDlgsMgr()->GetAutoPlantDlg()->ShowTreeInfos();
		}
		break;
	case PT_LANDGRASS:
		{
			info.dwID = m_pLandGrassInfo[nIdx].dwID;
			info.fWeight = m_fWeight;
			GetHomeDlgsMgr()->GetAutoPlantDlg()->GetLandGrassInfosArray()->Add(info);
			GetHomeDlgsMgr()->GetAutoPlantDlg()->ShowLandGrassInfos();
		}
		break;
	case PT_WATERGRASS:
		{
			info.dwID = m_pWaterGrassInfo[nIdx].dwID;
			info.fWeight = m_fWeight;
			GetHomeDlgsMgr()->GetAutoPlantDlg()->GetWaterGrassInfosArray()->Add(info);
			GetHomeDlgsMgr()->GetAutoPlantDlg()->ShowWaterGrassInfos();
		}
		break;
	default:
		break;
	}

	this->Show(false);
}

void CDlgAutoChoosePlant::OnCommandCancel(const char* szCommand)
{
	this->Show(false);
}