/********************************************************************
	created:	2005/08/16
	created:	16:8:2005   20:35
	file name:	DlgStoragePW.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/


#include "DlgStoragePW.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_GameUIMan.h"


AUI_BEGIN_COMMAND_MAP(CDlgStoragePW, CDlgBase)

AUI_ON_COMMAND("confirm", OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL", OnCommandIDCANCEL)

AUI_END_COMMAND_MAP()

CDlgStoragePW::CDlgStoragePW()
{

}

CDlgStoragePW::~CDlgStoragePW()
{

}

void CDlgStoragePW::OnShowDialog()
{
	CDlgBase::OnShowDialog();

	m_strText.Empty();
	UpdateData(false);

	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SoftkbGame");
	pDlg->Show(true, false, false);

	pDlg->AlignTo(this,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgStoragePW::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_EditBox(bSave, "DEFAULT_Txt_Input", m_strText);
}

void CDlgStoragePW::OnCommandConfirm(const char *szCommand)
{
	UpdateData(true);
	GetGameUIMan()->GetDialog("Win_SoftkbGame")->Show(false);
	
	if(this->GetData() == 1)
	{
		GetGameSession()->c2s_CmdOpenTrashBox(AC2AS(m_strText));
		this->Show(false);
	}
	else
	{
	   GetGameSession()->c2s_CmdNPCSevOpenTrash(AC2AS(m_strText), m_bFaction);
	   CloseDialog();
    }
}

void CDlgStoragePW::OnCommandIDCANCEL(const char *szCommand)
{   
	if(this->GetData() == 1)
	{
		this->Show(false);
	}
	else
	{
	    CloseDialog();
	}
	GetGameUIMan()->GetDialog("Win_SoftkbGame")->Show(false);
}

void CDlgStoragePW::CloseDialog()
{
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetHostPlayer()->EndNPCService();
	Show(false);
}
