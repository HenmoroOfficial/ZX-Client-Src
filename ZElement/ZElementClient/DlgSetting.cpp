/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:03
	file name:	DlgSetting.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/

#include "A3DDevice.h"
#include "EC_Game.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_GameUIMan.h"
#include "DlgSetting.h"
#include "DlgSettingVideo.h"
#include "DlgSettingSystem.h"
#include "DlgSettingGame.h"
#include "aui/AUIStillImageButton.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSetting, CDlgBase)

AUI_ON_COMMAND("Rdo_Setting*",	OnCommandSetting)
AUI_ON_COMMAND("default",		OnCommandDefault)
AUI_ON_COMMAND("apply",			OnCommandApply)
AUI_ON_COMMAND("confirm",		OnCommandConfirm)
AUI_ON_COMMAND("Btn_Close",		OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSetting, CDlgBase)

AUI_END_EVENT_MAP()

CDlgSetting::CDlgSetting() :
	m_pBtnDefault(NULL),
	m_bValidateSucceed(false)
{

}

CDlgSetting::~CDlgSetting()
{

}

void CDlgSetting::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Btn_Default", m_pBtnDefault);
}

void CDlgSetting::OnCommandSetting(const char *szCommand)
{
	// get dlg to show
	CDlgSetting * pDlg = NULL;
	AString strCommand = szCommand;
	if (strCommand == "Rdo_Setting1")
		pDlg = (CDlgSetting*)GetGameUIMan()->GetDialog("Win_SettingSystem");
	else if (strCommand == "Rdo_Setting2")
		pDlg = (CDlgSetting*)GetGameUIMan()->GetDialog("Win_SettingVideo");
	else if (strCommand == "Rdo_Setting3")
		pDlg = (CDlgSetting*)GetGameUIMan()->GetDialog("Win_SettingGame");
	ASSERT(pDlg);

	// hide old dialog
	POINT pos = GetGameUIMan()->m_pDlgSettingCurrent->GetPos();
	GetGameUIMan()->m_pDlgSettingCurrent->Show(false);

	// show dialog
	GetGameUIMan()->m_pDlgSettingCurrent = pDlg;

	// old : pDlg->SetPos(pos.x, pos.y);
	pDlg->SetPosEx(pos.x, pos.y);

	GetGameUIMan()->m_pDlgSettingCurrent->Show(true);
}


void CDlgSetting::OnCommandDefault(const char *szCommand)
{
	GetGameUIMan()->m_pDlgChatSetting->SetToDefault();
	GetGameUIMan()->m_pDlgSettingGame->SetToDefault();
	GetGameUIMan()->m_pDlgSettingSystem->SetToDefault();
	GetGameUIMan()->m_pDlgSettingVideo->SetToDefault();
}

void CDlgSetting::OnCommandApply(const char *szCommand)
{
	m_bValidateSucceed = 
		GetGameUIMan()->m_pDlgChatSetting->Validate() && 
		GetGameUIMan()->m_pDlgSettingGame->Validate() &&
		GetGameUIMan()->m_pDlgSettingSystem->Validate() &&
		GetGameUIMan()->m_pDlgSettingVideo->Validate();
	if (!m_bValidateSucceed) return;

	// old param
	A3DViewport * pViewport = GetGame()->GetViewport()->GetA3DViewport();
	A3DVIEWPORTPARAM param1 = *pViewport->GetParam();

	// do apply
	if( stricmp(m_szName, "Win_SettingGame") == 0 )
		GetGameUIMan()->m_pDlgSettingGame->Apply();
	else if(stricmp(m_szName,"Win_Chat_Setting") == 0)
		GetGameUIMan()->m_pDlgChatSetting->Apply();
	else if( stricmp(m_szName, "Win_SettingSystem") == 0 )
	{
		GetGameUIMan()->m_pDlgSettingSystem->Apply();
		// save system setting to file
		GetGame()->GetConfigs()->SaveSystemSettings();
	}
	else if( stricmp(m_szName, "Win_SettingVideo") == 0 )
		GetGameUIMan()->m_pDlgSettingVideo->Apply();

	// new param
	A3DVIEWPORTPARAM param2 = *pViewport->GetParam();
	

	// update IME state
	GetGameUIMan()->SetHideIme(!m_pA3DDevice->GetDevFormat().bWindowed);
	
	// RearrangeWindows if res is changed
	if( param1.Width != param2.Width || param1.Height != param2.Height )
	{
		A3DRECT rcOld(0, 0, param1.Width, param1.Height);
		A3DRECT rcNew(0, 0, param2.Width, param2.Height);
		
		GetGameUIMan()->RearrangeWindows(rcOld, rcNew);
	}

}

void CDlgSetting::OnCommandConfirm(const char *szCommand)
{
	OnCommandApply(NULL);

	if (m_bValidateSucceed)	Show(false);
}

void CDlgSetting::OnCommandCancel(const char *szCommand)
{
	Show(false);
}





