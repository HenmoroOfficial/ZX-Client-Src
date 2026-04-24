// Filename	:	DlgSettingUIBasic.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#include "A3DDevice.h"
#include "EL_BackMusic.h"
#include "DlgSettingAudioBasic.h"
#include "DlgChat.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_GameUIMan.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingAudioBasic, CDlgSetting)

AUI_ON_COMMAND("music",		OnCommandMusic)
AUI_ON_COMMAND("ambience",	OnCommandSFX)
AUI_ON_COMMAND("Sample",	OnCommandSample)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSettingAudioBasic, CDlgSetting)

AUI_END_EVENT_MAP()


CDlgSettingAudioBasic::CDlgSettingAudioBasic()
{
}

CDlgSettingAudioBasic::~CDlgSettingAudioBasic()
{
}

bool CDlgSettingAudioBasic::OnInitDialog()
{
	if (!CDlgSetting::OnInitDialog())
		return false;
	
	// init diaplay
	UpdateView();
	
	return true;
}

void CDlgSettingAudioBasic::OnShowDialog()
{
	CDlgSetting::OnShowDialog();
	
	// init diaplay
	UpdateView();
}

void CDlgSettingAudioBasic::DoDataExchange(bool bSave)
{
	CDlgSetting::DoDataExchange(bSave);
	
	DDX_Slider(bSave, "Slider_Music", m_stSystem.nMusicVol);
	DDX_Slider(bSave, "Slider_Ambience", m_stSystem.nSoundVol);
	
	if (bSave)
	{
		int nLevel = 0;
		DDX_Slider(bSave, "Slider_Sample", nLevel);
		m_stSystem.iSoundQuality = 2 - nLevel;
	}
	else
	{
		int nLevel = 2 - m_stSystem.iSoundQuality;
		DDX_Slider(bSave, "Slider_Sample", nLevel);
	}
}

void CDlgSettingAudioBasic::SetToDefault()
{
	GetGame()->GetConfigs()->DefaultSettingAudio(&m_stSystem);
	
	UpdateData(false);
	MakeChangeNow();
}

void CDlgSettingAudioBasic::Apply()
{
	UpdateData(true);
	
	// save setting
	GetGame()->GetConfigs()->SetSystemSettings(m_stSystem);
	
	MakeChangeNow();
}


void CDlgSettingAudioBasic::UpdateView()
{
	// read setting
	m_stGame = GetGame()->GetConfigs()->GetGameSettings();
	// show setting
	UpdateData(false);
	MakeChangeNow();
}

void CDlgSettingAudioBasic::OnCommandMusic(const char *szCommand)
{
	UpdateData(true);
	MakeChangeNow();
}

void CDlgSettingAudioBasic::OnCommandSFX(const char *szCommand)
{
	UpdateData(true);
	MakeChangeNow();
}

void CDlgSettingAudioBasic::OnCommandSample(const char *szCommand)
{
	UpdateData(true);
	MakeChangeNow();
}

void CDlgSettingAudioBasic::MakeChangeNow()
{
//	m_pA3DDevice->SetGammaLevel(m_stSystem.iGamma);
	GetGame()->GetBackMusic()->SetVolume(m_stSystem.nMusicVol);
	GetGame()->GetBackMusic()->SetSFXVolume(m_stSystem.nMusicVol);
	GetGame()->GetBackMusic()->SetCommentatorVolume(m_stSystem.nMusicVol);	
	m_pA3DEngine->GetAMSoundEngine()->SetVolume(m_stSystem.nSoundVol);
}