// Filename	: DlgCamera.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/18

#include "DlgCamera.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_Configs.h"
#include "A3DCamera.h"
#include "A3DFuncs.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgCamera, CDlgBase)

AUI_ON_COMMAND("printscr",		OnCommand_PrintScr)
AUI_ON_COMMAND("type",			OnCommand_Type)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

CDlgCamera::CDlgCamera()
{
	m_pBtn_CameraMoveLeft = NULL;
	m_pBtn_CameraMoveRight = NULL;
	m_pBtn_CameraMoveUp = NULL;
	m_pBtn_CameraMoveDown = NULL;
	m_pBtn_CameraMoveFront = NULL;
	m_pBtn_CameraMoveBack = NULL;
	m_pBtn_CameraFarFocus = NULL;
	m_pBtn_CameraNearFocus = NULL;
	m_pChk_HideUI = NULL;
	m_pCombo_Scale = NULL;
	m_pCombo_PrintScreenType = NULL;
}

CDlgCamera::~CDlgCamera()
{
}

void CDlgCamera::OnCommand_PrintScr(const char * szCommand)
{
	if( m_pChk_HideUI->IsChecked() )
		GetGameUIMan()->SetHideUIPrintMode(true);
	GetGameRun()->CaptureScreen(m_pCombo_Scale->GetCurSel() + 1);
	GetGameUIMan()->SetHideUIPrintMode(false);
}

void CDlgCamera::OnCommand_Type(const char * szCommand)
{
	EC_GAME_SETTING setting = GetGame()->GetConfigs()->GetGameSettings();
	setting.nPrintScreenType = m_pCombo_PrintScreenType->GetCurSel();
	GetGame()->GetConfigs()->SetGameSettings(setting);
}

void CDlgCamera::OnCommand_CANCEL(const char * szCommand)
{
}

bool CDlgCamera::OnInitDialog()
{
	DDX_Control("Btn_CameraMoveLeft", m_pBtn_CameraMoveLeft);
	DDX_Control("Btn_CameraMoveRight", m_pBtn_CameraMoveRight);
	DDX_Control("Btn_CameraMoveUp", m_pBtn_CameraMoveUp);
	DDX_Control("Btn_CameraMoveDown", m_pBtn_CameraMoveDown);
	DDX_Control("Btn_CameraMoveFront", m_pBtn_CameraMoveFront);
	DDX_Control("Btn_CameraMoveBack", m_pBtn_CameraMoveBack);
	DDX_Control("Btn_CameraFarFocus", m_pBtn_CameraFarFocus);
	DDX_Control("Btn_CameraNearFocus", m_pBtn_CameraNearFocus);
	DDX_Control("Chk_HideUI", m_pChk_HideUI);
	DDX_Control("Combo_Scale", m_pCombo_Scale);
	DDX_Control("Combo_PrintScreenType", m_pCombo_PrintScreenType);
	for(int i = 0; i < 8; i++ )
	{
		ACString strText;
		strText.Format(GetStringFromTable(761), i + 1);
		m_pCombo_Scale->AddString(strText);
	}
	m_pCombo_Scale->SetCurSel(0);
	m_pCombo_PrintScreenType->ResetContent();
	m_pCombo_PrintScreenType->AddString(_AL("BMP"));
	m_pCombo_PrintScreenType->AddString(_AL("JPG"));
	m_pChk_HideUI->Check(true);
	SetCanOutOfWindow(true);

	return true;
}

void CDlgCamera::OnTick()
{
	if( m_pCombo_Scale->GetCurSel() > 0 )
	{
		m_pChk_HideUI->Check(true);
		m_pCombo_PrintScreenType->Enable(false);
		m_pCombo_PrintScreenType->SetCurSel(0);
	}
	else
	{
		m_pCombo_PrintScreenType->Enable(true);
	}
	
	if( !GetGameRun()->GetHostPlayer()->GetCameraCtrl()->SceneryMode_Get() )
	{
		Show(false);
		return;
	}
	bool bState[7];

	bState[0] = m_pBtn_CameraMoveLeft->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[1] = m_pBtn_CameraMoveRight->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[2] = m_pBtn_CameraMoveFront->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK
				|| m_pBtn_CameraNearFocus->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[3] = m_pBtn_CameraMoveBack->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK
				|| m_pBtn_CameraFarFocus->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[4] = m_pBtn_CameraMoveUp->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[5] = m_pBtn_CameraMoveDown->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	bState[6] = m_pBtn_CameraFarFocus->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK
				|| m_pBtn_CameraNearFocus->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK;
	
	GetGameRun()->GetHostPlayer()->GetCameraCtrl()->SetVirtualKeyState(bState);
}

void CDlgCamera::OnShowDialog()
{
	SetPosEx(0 ,0, alignCenter, alignCenter);
	EC_GAME_SETTING setting = GetGame()->GetConfigs()->GetGameSettings();
	m_pCombo_PrintScreenType->SetCurSel(setting.nPrintScreenType);
}