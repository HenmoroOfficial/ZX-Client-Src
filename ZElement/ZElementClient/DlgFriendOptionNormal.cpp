// File		: DlgFriendOptionNormal.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#include "AUI\\AUIEditBox.h"
#include "DlgFriendOptionNormal.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "DlgFriendList.h"
#include "DlgInputName.h"
#include "gnetdef.h"
#include "EC_NetDef.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFriendOptionNormal, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCAL)
AUI_ON_COMMAND("AddFriend",		OnCommandAddFriend)
AUI_ON_COMMAND("AddGroup",		OnCommandAddGroup)
AUI_ON_COMMAND("SetAfk",		OnCommandSetAfk)
AUI_ON_COMMAND("OnOffline",		OnCommandOnOffline)
AUI_ON_COMMAND("Btn__GtOnline",	OnCommandOnOffGT)

AUI_END_COMMAND_MAP()

CDlgFriendOptionNormal::CDlgFriendOptionNormal()
{
}

CDlgFriendOptionNormal::~CDlgFriendOptionNormal()
{
}

void CDlgFriendOptionNormal::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
}

void CDlgFriendOptionNormal::OnCommandAddFriend(const char *szCommand)
{
	GetGameUIMan()->m_pDlgFriendList->OnCommandAdd("add");
	Show(false);
}

void CDlgFriendOptionNormal::OnCommandAddGroup(const char *szCommand)
{
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
	if (pMan->GetGroupNum()>=CECGAMEUIMAN_MAX_GROUPS)
	{
		GetGameUIMan()->MessageBox(10390);
		return;
	}
	CDlgInputName* pDlg = GetGameUIMan()->m_pDlgInputName;
	pDlg->m_pTxtInput->SetText(_AL(""));
	pDlg->SetData(CDlgInputName::INPUTNAME_ADD_GROUP);
	pDlg->Show(true);
	Show(false);
}

void CDlgFriendOptionNormal::OnCommandSetAfk(const char *szCommand)
{
	GetGameUIMan()->m_bAutoReply = !GetGameUIMan()->m_bAutoReply;
	GetDlgItem("Img_Right1")->Show(GetGameUIMan()->m_bAutoReply);
	GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(
		GetGameUIMan()->m_bAutoReply ? 553 : 554), GP_CHAT_MISC);
	Show(false);
}

void CDlgFriendOptionNormal::OnCommandOnOffline(const char *szCommand)
{
	GetGameUIMan()->m_bOnlineNotify = !GetGameUIMan()->m_bOnlineNotify;
	GetDlgItem("Img_Right2")->Show(GetGameUIMan()->m_bOnlineNotify);
	GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(
		GetGameUIMan()->m_bOnlineNotify ? 551 : 552), GP_CHAT_MISC);
	Show(false);
}

void CDlgFriendOptionNormal::OnCommandOnOffGT(const char *szCommand)
{
	GetGameUIMan()->m_bGTOnLineNotify = !GetGameUIMan()->m_bGTOnLineNotify;
	GetDlgItem("Img_GtOnline")->Show(GetGameUIMan()->m_bGTOnLineNotify);
	GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(
		GetGameUIMan()->m_bGTOnLineNotify ? 1017 : 1018), GP_CHAT_MISC);
	Show(false);
}

void CDlgFriendOptionNormal::OnShowDialog()
{
	GetDlgItem("Img_Right1")->Show(GetGameUIMan()->m_bAutoReply);
	GetDlgItem("Img_Right2")->Show(GetGameUIMan()->m_bOnlineNotify);
	GetDlgItem("Img_GtOnline")->Show(false);//GetGameUIMan()->m_bGTOnLineNotify);
	GetDlgItem("Btn__GtOnline")->Show(false);
}

void CDlgFriendOptionNormal::OnTick()
{
	CECGameUIMan * pGameUI = GetGameUIMan();
	if (pGameUI->GetActiveDialog()!=this && !pGameUI->GetDialog("Win_GamePoker_Hall")->IsShow() && !pGameUI->GetDialog("Win_GamePoker_Match")->IsShow())
		Show(false);
}