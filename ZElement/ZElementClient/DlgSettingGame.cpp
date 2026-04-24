/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:50
	file name:	DlgSettingGame.cpp
	author:		yaojun
	
	purpose:	
*********************************************************************/


#include "DlgSettingGame.h"
#include "DlgChat.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIComboBox.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_GameUIMan.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingGame, CDlgSetting)

AUI_ON_COMMAND("set*",	OnCommandSet)
AUI_ON_COMMAND("Ch_Up*",		OnCommand_LocalChatSet)
AUI_ON_COMMAND("Ch_Down*",		OnCommand_LocalChatSet)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSettingGame, CDlgSetting)

AUI_ON_EVENT("Ch_*", WM_LBUTTONUP, OnLButtonUpChat)
AUI_ON_EVENT("Chk_*", WM_LBUTTONUP, OnLButtonUpChat)

AUI_END_EVENT_MAP()

//	强制关闭频道的某些聊天信息设置
bool g_bChannelEnable[EC_USERCHANNEL_NUM][CLIENT_CHAT_MAX] =
{	// 本地  呐喊   组队   帮派   密语   伤害   战斗   交易   系统   广播   其它   家族		圈子	同服	保留1	保留2	金钱	经验	拾取	杀人
	{ false, false, true,  true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 普通
	{ false, false, false, true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 组队
	{ true,  false, true,  true,  false, true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 密语
	{ true,  true,  true,  false, true,  true,  true,  true,  false, false, false, false,	true,	true,	true,	true,	true,	true,	true,	true },// 帮派
	{ false, false, false, true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 系统
	{ true,  false, true,  true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 世界
	{ true,  true,  true,  true,  true,  true,  true,  true,  false, false, false, true,	true,	false,	true,	true,	true,	true,	true,	true },// 同服
	{ false, false, true,  true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 自定义1
	{ false, false, true,  true,  true,  true,  true,  true,  false, false, false, true,	true,	true,	true,	true,	true,	true,	true,	true },// 自定义2
};

CDlgSettingGame::CDlgSettingGame()
{
	m_nChanel = 1;
	memset(&m_GameSetting, 0, sizeof(m_GameSetting));
	m_bFilterLocalChat = false;
	m_nLocalChatLevel = 0;
	m_bFilterWhisper = false;
	m_nWhisperLevel = 0;
	m_nFont = 0;
	m_nTransparent = 0;

}

CDlgSettingGame::~CDlgSettingGame()
{
}

bool CDlgSettingGame::OnInitDialog()
{
	if (!CDlgSetting::OnInitDialog())
		return false;

	m_pCmbFilter = (PAUICOMBOBOX)GetDlgItem("Combo_Lv");
	m_pCmbFilter->AddString(_AL("1"));
	m_pCmbFilter->AddString(_AL("5"));
	m_pCmbFilter->AddString(_AL("10"));
	m_pCmbFilter->AddString(_AL("20"));
	m_pCmbFilter->AddString(_AL("30"));
	m_pCmbFilter->AddString(_AL("40"));
	m_pCmbFilter->AddString(_AL("50"));
	m_pCmbFilter->AddString(_AL("60"));
	// init diaplay
	UpdateView();

	
	return true;
}

void CDlgSettingGame::OnShowDialog()
{
	CDlgSetting::OnShowDialog();

	// init control
	CheckRadioButton(2,3);

	// init diaplay
	UpdateView();
}

void CDlgSettingGame::DoDataExchange(bool bSave)
{
	CDlgSetting::DoDataExchange(bSave);

	DDX_CheckBox(bSave, "Chk_Autoreply", m_GameSetting.bAutoReply);
	DDX_Slider(bSave, "Slider_ChatFsize", m_GameSetting.nFontSize);
	DDX_Slider(bSave,"Prgs_Clarity",m_nTransparent);
	DDX_EditBox(bSave, "Txt_Reply", m_GameSetting.szAutoReply);
	DDX_RadioButton(bSave, 1, m_nChanel);
	for (int nChatIndex = 0; nChatIndex < CLIENT_CHAT_MAX; nChatIndex++)
	{
		ASSERT(m_nChanel >= 1 && m_nChanel <= EC_USERCHANNEL_NUM);
		AString strCheckBoxName;
		strCheckBoxName.Format("Ch_%d", nChatIndex + 1);
		if( !GetDlgItem(strCheckBoxName) )
			continue;
		
		GetDlgItem(strCheckBoxName)->Enable(g_bChannelEnable[m_nChanel - 1][nChatIndex]);
		if (bSave)
			m_GameSetting.bChannel[m_nChanel - 1][nChatIndex] = ((PAUICHECKBOX)GetDlgItem(strCheckBoxName))->IsChecked();
		else
		{
			((PAUICHECKBOX)GetDlgItem(strCheckBoxName))->Check(m_GameSetting.bChannel[m_nChanel - 1][nChatIndex]);
			// 防止数据错误，勾不上
			if (!g_bChannelEnable[m_nChanel - 1][nChatIndex])
			{
				((PAUICHECKBOX)GetDlgItem(strCheckBoxName))->Check(true);
			}
		}
	}

	{
	    EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
		for(int i=1;i<=9;++i)
		{
		   AString  strName;
		   strName.Format("Chk_Show%02d",i);
		   if(bSave)
		   {
			  ls.local_chat.bShowChannel[i-1] = ((PAUICHECKBOX)GetDlgItem(strName))->IsChecked();
		   }
		   else
		   {
		      ((PAUICHECKBOX)GetDlgItem(strName))->Check(ls.local_chat.bShowChannel[i-1]);
		   }

		}

		if(bSave)
		{
			ls.local_chat.bChannel[5] = ((PAUICHECKBOX)GetDlgItem("Ch_Up6"))->IsChecked();
			ls.local_chat.bChannel[8] = ((PAUICHECKBOX)GetDlgItem("Ch_Up9"))->IsChecked();
			ls.local_chat.bChannel[9] = ((PAUICHECKBOX)GetDlgItem("Ch_Up10"))->IsChecked();
			ls.local_chat.bChannel[10] = ((PAUICHECKBOX)GetDlgItem("Ch_Up11"))->IsChecked();
			ls.local_chat.bChannel[16] = ((PAUICHECKBOX)GetDlgItem("Ch_Up17"))->IsChecked();
			ls.local_chat.bChannel[17] = ((PAUICHECKBOX)GetDlgItem("Ch_Up18"))->IsChecked();
			ls.local_chat.bChannel[18] = ((PAUICHECKBOX)GetDlgItem("Ch_Up19"))->IsChecked();
			ls.local_chat.bChannel[19] = ((PAUICHECKBOX)GetDlgItem("Ch_Up20"))->IsChecked();
		}
		else
		{

			((PAUICHECKBOX)GetDlgItem("Ch_Up6"))->Check(ls.local_chat.bChannel[5]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up9"))->Check(ls.local_chat.bChannel[8]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up10"))->Check(ls.local_chat.bChannel[9]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up11"))->Check(ls.local_chat.bChannel[10]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up17"))->Check(ls.local_chat.bChannel[16]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up18"))->Check(ls.local_chat.bChannel[17]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up19"))->Check(ls.local_chat.bChannel[18]);
			((PAUICHECKBOX)GetDlgItem("Ch_Up20"))->Check(ls.local_chat.bChannel[19]);


			((PAUICHECKBOX)GetDlgItem("Ch_Down6"))->Check(!ls.local_chat.bChannel[5]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down9"))->Check(!ls.local_chat.bChannel[8]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down10"))->Check(!ls.local_chat.bChannel[9]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down11"))->Check(!ls.local_chat.bChannel[10]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down17"))->Check(!ls.local_chat.bChannel[16]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down18"))->Check(!ls.local_chat.bChannel[17]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down19"))->Check(!ls.local_chat.bChannel[18]);
			((PAUICHECKBOX)GetDlgItem("Ch_Down20"))->Check(!ls.local_chat.bChannel[19]);
		}


		if(bSave)
		  g_pGame->GetConfigs()->SetLocalSettings(ls);
	}

	if(bSave)
	{
	   m_nFont = GetCheckedRadioButton(3)-1;
	}
	else
	{
	   CheckRadioButton(3,m_nFont+1);
	}
	
// 	DDX_CheckBox(bSave, "Chk_ShieldNormal", m_bFilterLocalChat);
// 	DDX_EditBox(bSave, "Edt_ShieldNormalLv", m_nLocalChatLevel);
// 	GetDlgItem("Edt_ShieldNormalLv")->Enable(m_bFilterLocalChat);
	DDX_CheckBox(bSave, "Chk_ShieldPrivate", m_bFilterWhisper);
	DDX_ComboBox(bSave, "Combo_Lv", m_nWhisperLevel);
	GetDlgItem("Combo_Lv")->Enable(m_bFilterWhisper);
}

void CDlgSettingGame::SetToDefault()
{
	GetGame()->GetConfigs()->DefaultUserSettings(NULL, NULL, &m_GameSetting);
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	ls.local_chat.bFilterLocalChat = false;
	ls.local_chat.bFilterWhisper = false;
	for (int i=0;i<7;++i)
	{
		ls.local_chat.bShowChannel[i] = true;
	}
	ls.local_chat.bShowChannel[7] = false;
	ls.local_chat.bShowChannel[8] = false;
	m_nFont = 0;
    m_nTransparent = 5;
	m_GameSetting.nFontSize = 2;
	g_pGame->GetConfigs()->SetLocalSettings(ls);
	UpdateData(false);
}

void CDlgSettingGame::Apply()
{
	UpdateData(true);
	
	// save setting
	GetGame()->GetConfigs()->SetGameSettings(m_GameSetting);
	
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	ls.local_chat.bFilterLocalChat = m_bFilterLocalChat;
	ls.local_chat.nLocalChatLevel = m_nLocalChatLevel;
	ls.local_chat.bFilterWhisper  = m_bFilterWhisper;
	ls.local_chat.nWhisperLevel   = m_nWhisperLevel;
	ls.local_chat.nFont           = m_nFont;
	CDlgChat::m_nCurChatAlpha     = m_nTransparent;
	g_pGame->GetConfigs()->SetLocalSettings(ls);

	static  ACHAR * fontname[] =
	{
	  _AL("方正细黑一简体"),
	  _AL("方正隶二简体"),
	  _AL("方正隶变简体")
	};
	// todo : the following lines should be rewritten
	AUIOBJECT_SETPROPERTY sp;
	CDlgChat *pChat;
	PAUITEXTAREA pText;
	
	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatInfo");
	pText = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
	pText->GetProperty("Font", &sp);
	sp.font.nHeight = m_GameSetting.nFontSize + 10;	// 10 to 14.
	a_strcpy(sp.font.szName,fontname[m_nFont]);
	pText->SetProperty("Font", &sp);
	
 	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatInfo2");
 	pText = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
 	pText->GetProperty("Font", &sp);
 	sp.font.nHeight = m_GameSetting.nFontSize + 10;	// 10 to 14.
	a_strcpy(sp.font.szName,fontname[m_nFont]);
	pText->SetProperty("Font", &sp);
	pChat->RebuildChatContents();

	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatSpecInfo");
	pText = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
	pText->GetProperty("Font", &sp);
	sp.font.nHeight = m_GameSetting.nFontSize + 10;	// 10 to 14.
	a_strcpy(sp.font.szName,fontname[m_nFont]);
	pText->SetProperty("Font", &sp);

	
	{
		pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_Chat");
	    pChat->RebuildChatContents();
	    pChat->RefreshChannelTab();
	}
	//pChat->UpdateChatSetInfo();
}


void CDlgSettingGame::UpdateView()
{
	// read setting
	m_GameSetting = GetGame()->GetConfigs()->GetGameSettings();
	EC_LOCAL_SETTING ls = g_pGame->GetConfigs()->GetLocalSettings();
	
	m_bFilterLocalChat = ls.local_chat.bFilterLocalChat;
	m_nLocalChatLevel  = ls.local_chat.nLocalChatLevel;
	m_bFilterWhisper   = ls.local_chat.bFilterWhisper;
	m_nWhisperLevel    = ls.local_chat.nWhisperLevel;
	m_nFont            = ls.local_chat.nFont;
	m_nTransparent     = CDlgChat::m_nCurChatAlpha;

	// show setting
	UpdateData(false);
}

void CDlgSettingGame::OnCommandSet(const char *szCommand)
{
	AString strNum = szCommand + strlen("set");
	m_nChanel = strNum.ToInt();

	UpdateData(false);
}

void CDlgSettingGame::OnLButtonUpChat(WPARAM, LPARAM, AUIObject *pObj)
{
	UpdateData(true);
	if (pObj == GetDlgItem("Chk_ShieldNormal"))
	{
		ChangeFocus(GetDlgItem("Edt_ShieldNormalLv"));
	}
	else if (pObj == GetDlgItem("Chk_ShieldPrivate"))
	{
		ChangeFocus(GetDlgItem("Edt_ShieldPrivateLv"));
	}
}

void CDlgSettingGame::OnCommand_LocalChatSet(const char *szCommand)
{
	AString strName;

	if (strstr(szCommand, "Ch_Up"))
	{
		int index = atoi(szCommand + strlen("Ch_Up"));
		strName.Format("Ch_Down%d", index);
		PAUICHECKBOX pChkUp = (PAUICHECKBOX)GetDlgItem(szCommand);
		PAUICHECKBOX pChkDown = (PAUICHECKBOX)GetDlgItem(strName);
		pChkDown->Check(!pChkUp->IsChecked());
	}
	else if (strstr(szCommand, "Ch_Down"))
	{
		int index = atoi(szCommand + strlen("Ch_Down"));
		strName.Format("Ch_Up%d", index);
		PAUICHECKBOX pChkUp = (PAUICHECKBOX)GetDlgItem(strName);
		PAUICHECKBOX pChkDown = (PAUICHECKBOX)GetDlgItem(szCommand);
		pChkUp->Check(!pChkDown->IsChecked());
	}
}
