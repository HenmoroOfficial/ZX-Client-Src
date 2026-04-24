// Filename	:	DlgSetting.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 12, 2009

#include "DlgSettingMain.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIEditBox.h"

#include "EC_GameUIMan.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingMain, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Close",			OnCommand_close)
AUI_ON_COMMAND("default",			OnCommand_default)
AUI_ON_COMMAND("apply",				OnCommand_apply)
AUI_ON_COMMAND("confirm",			OnCommand_confirm)
AUI_ON_COMMAND("Rdo_Setting*",		OnCommand_SettingType)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSettingMain, CDlgBase)


AUI_END_EVENT_MAP()

CDlgSettingMain::CDlgSettingMain()
{
}

CDlgSettingMain::~CDlgSettingMain()
{
}

bool CDlgSettingMain::Release()
{
	return CDlgBase::Release();
}

bool CDlgSettingMain::OnInitDialog()
{
	return true;
}

void CDlgSettingMain::OnShowDialog()
{
	CheckRadioButton(1,0);
}

void CDlgSettingMain::OnTick()
{
}

bool CDlgSettingMain::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

void CDlgSettingMain::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgSettingMain::OnCommand_close(const char * szCommand)
{
	Show(false);
}

void CDlgSettingMain::OnCommand_default(const char * szCommand)
{
}

void CDlgSettingMain::OnCommand_apply(const char * szCommand)
{
}

void CDlgSettingMain::OnCommand_confirm(const char * szCommand)
{
}

void CDlgSettingMain::OnCommand_SettingType(const char * szCommand)
{
	int iSettingType = atoi(szCommand + strlen("Rdo_Setting"));
	PAUIDIALOG pDlg = NULL;
	switch (iSettingType)
	{
	case 1:
		pDlg = GetGameUIMan()->GetDialog("Win_SettingVideo");
		break;
	case 2:
		pDlg = GetGameUIMan()->GetDialog("Win_SettingAudio");
		break;
	case 3:
		pDlg = GetGameUIMan()->GetDialog("Win_SettingUI");
		break;
	case 4:
		pDlg = GetGameUIMan()->GetDialog("Win_SettingOther");
		break;
	}
	
	if( pDlg )
	{
		Show(false);
		pDlg->Show(true);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
	}
}