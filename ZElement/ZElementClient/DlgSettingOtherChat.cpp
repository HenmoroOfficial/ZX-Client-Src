// Filename	:	DlgSettingOtherChat.cpp
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#include "DlgSettingOtherChat.h"
#include "DlgChat.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUISubDialog.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_GameRun.h"
#include "EC_HostInputFilter.h"
#include "EC_InputCtrl.h"
#include "EC_GameUIMan.h"
#include "LuaEvent.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSettingOtherChat, CDlgBase)

AUI_ON_COMMAND("set*",	OnCommandSet)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgSettingOtherChat, CDlgBase)

AUI_ON_EVENT("Ch_*", WM_LBUTTONUP, OnLButtonUpChat)

AUI_END_EVENT_MAP()

//	强制关闭频道的某些聊天信息设置
bool g_bChannelEnable[EC_USERCHANNEL_NUM][GP_CHAT_MAX] =
{	// 本地  呐喊   组队   帮派   密语   伤害   战斗   交易   系统   广播   其它   家族
	{ false, false, true,  true,  true,  true,  true,  true,  false, false, false, true },// 普通
	{ false, false, false, true,  true,  true,  true,  true,  false, false, false, true },// 组队
	{ true,  false, true,  true,  false, true,  true,  true,  false, false, false, true },// 密语
	{ true,  true,  true,  false, true,  true,  true,  true,  false, false, false, false},// 帮派
	{ false, false, false, true,  true,  true,  true,  true,  false, false, false, true },// 系统
	{ true,  false, true,  true,  true,  true,  true,  true,  false, false, false, true } // 世界
};

CDlgSettingOtherChat::CDlgSettingOtherChat()
{
	m_nChanel = 1;
}

CDlgSettingOtherChat::~CDlgSettingOtherChat()
{
}

bool CDlgSettingOtherChat::OnInitDialog()
{
	// init diaplay
	UpdateView();

	return true;
}

void CDlgSettingOtherChat::OnShowDialog()
{
	CDlgSetting::OnShowDialog();
	// init diaplay
	UpdateView();
}

void CDlgSettingOtherChat::DoDataExchange(bool bSave)
{
	DDX_RadioButton(bSave, 1, m_nChanel);
	for (int nChatIndex = 0; nChatIndex < GP_CHAT_MAX; nChatIndex++)
	{
		ASSERT(m_nChanel >= 1 && m_nChanel <= EC_USERCHANNEL_NUM);
		AString strCheckBoxName;
		strCheckBoxName.Format("Ch_%d", nChatIndex + 1);
		if( !GetDlgItem(strCheckBoxName) )
			continue;
		
		GetDlgItem(strCheckBoxName)->Enable(g_bChannelEnable[m_nChanel - 1][nChatIndex]);
		DDX_CheckBox(bSave, strCheckBoxName, m_stGame.bChannel[m_nChanel - 1][nChatIndex]);
	}
	DDX_CheckBox(bSave, "Chk_Autoreply", m_stGame.bAutoReply);
	DDX_EditBox(bSave, "Txt_Reply", m_stGame.szAutoReply);
}

void CDlgSettingOtherChat::SetToDefault()
{
	GetGame()->GetConfigs()->DefaultSettingOtherChat(&m_stGame);

	UpdateData(false);
}

void CDlgSettingOtherChat::Apply()
{
	UpdateData(true);
	
	// save setting
	GetGame()->GetConfigs()->SetGameSettings(m_stGame);

	// todo : the following lines should be rewritten
// 	AUIOBJECT_SETPROPERTY sp;
// 	CDlgChat *pChat;
// 	PAUITEXTAREA pText;
// 	
// 	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatInfo");
// 	pText = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
// 	pText->GetProperty("Font", &sp);
// 	sp.font.nHeight = m_settingGame.nFontSize + 10;	// 10 to 14.
// 	pText->SetProperty("Font", &sp);
// 	pChat->RebuildChatContents(pText, pChat->GetActiveChannelSet());
// 	
// 	pChat = (CDlgChat *)GetGameUIMan()->GetDialog("Win_ChatSmall");
// 	pText = (PAUITEXTAREA)pChat->GetDlgItem("Txt_Content");
// 	pText->GetProperty("Font", &sp);
// 	sp.font.nHeight = m_settingGame.nFontSize + 10;	// 10 to 14.
// 	pText->SetProperty("Font", &sp);
}


void CDlgSettingOtherChat::UpdateView()
{
	// read setting
	m_stGame = GetGame()->GetConfigs()->GetGameSettings();

	// show setting
	UpdateData(false);
}

void CDlgSettingOtherChat::OnCommandSet(const char *szCommand)
{
	AString strNum = szCommand + strlen("set");
	m_nChanel = strNum.ToInt();

	UpdateData(false);
}

void CDlgSettingOtherChat::OnLButtonUpChat(WPARAM, LPARAM, AUIObject *)
{
	UpdateData(true);
}
