// Filename	:	DlgSettingOther.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#include "AUI\\AUISubDialog.h"
#include "DlgSettingOther.h"
#include "DlgSetting.h"
#include "EC_GameUIMan.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingOther, CDlgBase)

AUI_ON_COMMAND("default",			OnCommandDefault)
AUI_ON_COMMAND("apply",				OnCommandApply)
AUI_ON_COMMAND("confirm",			OnCommandConfirm)
AUI_ON_COMMAND("Btn_Return",		OnCommandBack)
AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)
AUI_ON_COMMAND("Btn_Close",			OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSettingOther, CDlgBase)

AUI_END_EVENT_MAP()

CDlgSettingOther::CDlgSettingOther()
{
}

CDlgSettingOther::~CDlgSettingOther()
{
}

bool CDlgSettingOther::OnInitDialog()
{
	m_pDlgCurSetting = (CDlgSetting*)((PAUISUBDIALOG)GetDlgItem("Sub_Other_Chat"))->GetSubDialog();

	return true;
}

void CDlgSettingOther::OnShowDialog()
{
	m_pDlgCurSetting->UpdateView();
}

void CDlgSettingOther::OnCommandDefault(const char *szCommand)
{
	m_pDlgCurSetting->SetToDefault();
}

void CDlgSettingOther::OnCommandApply(const char *szCommand)
{
	m_pDlgCurSetting->Apply();
}

void CDlgSettingOther::OnCommandConfirm(const char *szCommand)
{
	OnCommandApply("");
	OnCommandCancel("");
}

void CDlgSettingOther::OnCommandCancel(const char *szCommand)
{
	Show(false);
}

void CDlgSettingOther::OnCommandBack(const char *szCommand)
{
	Show(false);
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Setting");
	pDlg->Show(true);
	pDlg->SetPosEx(GetPos().x, GetPos().y);
}