/*
 * FILE: DlgAutoChooseSky.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Liyi, 2005/12/19
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */
#include "DlgAutoChooseSky.h"
#include "aui/AUIListBox.h"

AUI_BEGIN_COMMAND_MAP(CDlgAutoChooseSky, CDlgHomeBase)

AUI_ON_COMMAND("OK", OnCommandOK)
AUI_ON_COMMAND("Cancel", OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAutoChooseSky, CDlgHomeBase)

AUI_END_EVENT_MAP()

CDlgAutoChooseSky::CDlgAutoChooseSky()
{
	m_pListBoxSkys = NULL;
	m_nIdxSky = 0;
	m_pSkyInfos = NULL;
	m_nNumSkyInfos = 0;
	m_SkyType = ST_NULL;
}

CDlgAutoChooseSky::~CDlgAutoChooseSky()
{
}


bool CDlgAutoChooseSky::OnInitContext()
{	
	ClearListBox();
	return InitResource();
}

void CDlgAutoChooseSky::OnShowDialog()
{
	this->SetCanMove(false);
}

void CDlgAutoChooseSky::DoDataExchange(bool bSave)
{
	DDX_Control("ListBox_Skys", m_pListBoxSkys);
	DDX_ListBox(bSave, "ListBox_Skys", m_nIdxSky);
}
	
bool CDlgAutoChooseSky::InitResource()
{
	CAutoScene* pScene = GetAutoScene();
	CAutoSky* pSky = pScene->GetAutoSky();
	if(!pSky)
		return false;

	m_pSkyInfos = pSky->GetSkyInfos();
	m_nNumSkyInfos = pSky->GetNumSkyInfos();
	
	return true;
}

void CDlgAutoChooseSky::ClearListBox()
{
	m_pListBoxSkys->ResetContent();
}

void CDlgAutoChooseSky::InitChooseSky(SkyType_e st)
{
	FillListBox(st);
	m_SkyType = st;

}

void CDlgAutoChooseSky::FillListBox(SkyType_e st)
{
	ClearListBox();
	int i = 0;

	switch(st)
	{
	case ST_DAY:
	case ST_NIGHT:
		{
			for( i = 0; i < m_nNumSkyInfos; i++)
			{
				m_pListBoxSkys->InsertString(i, AS2AC(m_pSkyInfos[i].strName));
			}
		}
		break;
	default:
		break;
	}
}

void CDlgAutoChooseSky::OnCommandOK(const char* szCommand)
{
	UpdateData(true);

	CAutoSky* pSky = GetAutoScene()->GetAutoSky();
	CAutoTerrainAction* pAction = GetAutoScene()->GetTerrainAction();

	switch(m_SkyType)
	{
	case ST_DAY:
		{
			pSky->SetSkyById(m_pSkyInfos[m_nIdxSky].dwID, CAutoSky::ST_DAY);
			pSky->SetBackColorByCurSky();
			pAction->AddActionSky(pSky->GetDaySkyId(), pSky->GetNightSkyId());
		}
		break;
	case ST_NIGHT:
		{
			pSky->SetSkyById(m_pSkyInfos[m_nIdxSky].dwID, CAutoSky::ST_NIGHT);
			pSky->SetBackColorByCurSky();
			pAction->AddActionSky(pSky->GetDaySkyId(), pSky->GetNightSkyId());
		}
		break;
	default:
		break;
	}

	this->Show(false);
}

void CDlgAutoChooseSky::OnCommandCancel(const char* szCommand)
{
	this->Show(false);
}