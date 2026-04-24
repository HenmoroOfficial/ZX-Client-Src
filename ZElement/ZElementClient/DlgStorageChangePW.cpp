/********************************************************************
	created:	2005/08/17
	created:	17:8:2005   10:24
	file name:	DlgStorageChangePW.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/



#include "DlgStorageChangePW.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"

AUI_BEGIN_COMMAND_MAP(CDlgStorageChangePW, CDlgBase)

AUI_ON_COMMAND("confirm", OnCommandConfirm)
AUI_ON_COMMAND("IDCANCEL", OnCommandIDCANCEL)

AUI_END_COMMAND_MAP()


CDlgStorageChangePW::CDlgStorageChangePW()
{

}

CDlgStorageChangePW::~CDlgStorageChangePW()
{

}

void CDlgStorageChangePW::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_EditBox(bSave, "DEFAULT_Txt_Input", m_strOldPW);
	DDX_EditBox(bSave, "Txt_New", m_strNewPW1);
	DDX_EditBox(bSave, "Txt_NewConfirm", m_strNewPW2);
}



void CDlgStorageChangePW::OnShowDialog()
{
	CDlgBase::OnShowDialog();

	m_strOldPW.Empty();
	m_strNewPW1.Empty();
	m_strNewPW2.Empty();
	UpdateData(false);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_SoftkbGame");
	pDlg->Show(true, false, false);

	pDlg->AlignTo(this,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
			AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
}

void CDlgStorageChangePW::CloseDialog()
{
	GetGameUIMan()->m_pCurNPCEssence = NULL;
	GetHostPlayer()->EndNPCService();
	Show(false);
}


void CDlgStorageChangePW::OnCommandIDCANCEL(const char *szCommand)
{
	CloseDialog();
	GetGameUIMan()->GetDialog("Win_SoftkbGame")->Show(false);
}

void CDlgStorageChangePW::OnCommandConfirm(const char *szCommand)
{
	UpdateData(true);

	if (m_strNewPW1 == m_strNewPW2)
	{
		GetGameUIMan()->GetDialog("Win_SoftkbGame")->Show(false);
		GetGameSession()->c2s_CmdNPCSevChgTrashPsw(
			AC2AS(m_strOldPW), AC2AS(m_strNewPW2));

		CloseDialog();
	}
	else
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(254),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
}
