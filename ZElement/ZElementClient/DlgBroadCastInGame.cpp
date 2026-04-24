// Filename	: DlgBroadcastInGame.cpp
// Creator	: Fu Chonggang
// Date		: 2011/3/31

#include "AFI.h"
#include "AUI\\AUICheckBox.h"

#include "DlgBroadCastInGame.h"
#include "DlgMinimizeBar.h"
#include "EC_GameUIMan.h"
#include "EC_Configs.h"
#include "EC_Game.h"
#include "EL_BackMusic.h"
#include "EC_Global.h"
#include "ExplorerMan.h"
#include <A3DGFXExMan.h>
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgBroadCastInGame, CDlgBaseExplorer)

AUI_ON_COMMAND("Btn_Minimize",	OnCommand_Minimize)
AUI_ON_COMMAND("Chk_Switch",	OnCommand_Sound)
AUI_ON_COMMAND("Btn_Close",		OnCommand_CANCEL)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgBroadCastInGame, CDlgBaseExplorer)

AUI_END_EVENT_MAP()

CDlgBroadCastInGame::CDlgBroadCastInGame()
{
}

CDlgBroadCastInGame::~CDlgBroadCastInGame()
{
}

bool CDlgBroadCastInGame::OnInitDialog()
{
	m_pChkSound		= (PAUICHECKBOX)GetDlgItem("Chk_Switch");
	m_bDisableSound = false;
	m_bMinimized	= false;
	m_bNoDocScroll	 = true;
	return CDlgBaseExplorer::OnInitDialog();
}

void CDlgBroadCastInGame::OnShowDialog()
{
	m_bMinimized = false;
	m_pChkSound->Check(true);
	OnCommand_Sound("");
	CDlgBaseExplorer::OnShowDialog();
}

void CDlgBroadCastInGame::OnHideDialog()
{
	if(m_bMinimized)
		return;

	NavigateUrl(g_strBlankUrl);
	CDlgBaseExplorer::OnHideDialog();
}

void CDlgBroadCastInGame::OnTick()
{
	CDlgBaseExplorer::OnTick();
	
	A3DRECT rcWindow = GetGameUIMan()->GetRect();
	if (GetPos().y<0)
	{
		SetPosEx(GetPos().x, 0);
	}
	if (GetPos().y>(rcWindow.Height()-50))
	{
		SetPosEx(GetPos().x, (rcWindow.Height()-50));
	}
	if ((GetPos().x+GetSize().cx/2)<0)
	{
		SetPosEx(-GetSize().cx/2, GetPos().y);
	}
	
	if ((GetPos().x+GetSize().cx/2)>rcWindow.Width())
	{
		SetPosEx(rcWindow.Width()-GetSize().cx/2, GetPos().y);
	}
}

bool CDlgBroadCastInGame::Release()
{
	NavigateUrl(g_strBlankUrl);
	return CDlgBaseExplorer::Release();
}

void CDlgBroadCastInGame::OnCommand_Minimize(const char * szCommand)
{
	m_bMinimized = true;
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetStringFromTable(10501), CDlgMinimizeBar::MINIMIZEBAR_TYPE_BROADCAST);
}

void CDlgBroadCastInGame::OnCommand_Sound(const char * szCommand)
{
	bool bDisableSound = m_pChkSound->IsChecked();
	if (bDisableSound&&!m_bDisableSound)
	{
		GetGame()->GetBackMusic()->SetVolume(0);
		GetGame()->GetBackMusic()->SetSFXVolume(0);
		GetGame()->GetBackMusic()->SetCommentatorVolume(0);	
		m_pA3DEngine->GetAMSoundEngine()->SetVolume(0);

		EC_SYSTEM_SETTING sysSetting = GetGame()->GetConfigs()->GetSystemSettings();
		m_iMusicVol = sysSetting.nMusicVol;
		m_iSoundVol = sysSetting.nSoundVol;
		sysSetting.nMusicVol = 0;
		sysSetting.nSoundVol = 0;
		GetGame()->GetConfigs()->SetSystemSettings(sysSetting);
	}
	else if(!bDisableSound && m_bDisableSound)
	{
		EC_SYSTEM_SETTING sysSetting = GetGame()->GetConfigs()->GetSystemSettings();
		sysSetting.nMusicVol = m_iMusicVol;
		sysSetting.nSoundVol = m_iSoundVol;
		GetGame()->GetConfigs()->SetSystemSettings(sysSetting);

		GetGame()->GetBackMusic()->SetVolume(sysSetting.nMusicVol);
		GetGame()->GetBackMusic()->SetSFXVolume(sysSetting.nMusicVol);
		GetGame()->GetBackMusic()->SetCommentatorVolume(sysSetting.nMusicVol);	
		m_pA3DEngine->GetAMSoundEngine()->SetVolume(sysSetting.nSoundVol);
	}
	m_bDisableSound = bDisableSound;
}

void CDlgBroadCastInGame::OnCommand_CANCEL(const char * szCommand)
{
	m_pChkSound->Check(false);
	OnCommand_Sound("");
	Show(false);
	CDlgBaseExplorer::OnHideDialog();	// Í£Ö¹´«ÊäÊý¾Ý
}