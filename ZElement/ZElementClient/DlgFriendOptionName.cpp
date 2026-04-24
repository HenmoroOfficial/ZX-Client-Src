// File		: DlgFriendOptionName.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#include "AUI\\AUICommon.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "DlgChat.h"
#include "DlgFriendOptionName.h"
#include "DlgFriendList.h"
#include "DlgFriendHistory.h"
#include "DlgFriendGroup.h"
//#include "DlgChannelChat.h"
#include "DlgFriendChat.h"
#include "DlgFlower.h"
#include "DlgInputName.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_HostPlayer.h"
#include "Network\\gnetdef.h"
#include "A3DDevice.h"
#include "EC_HTC2SRawCmds.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFriendOptionName, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCANCAL)
AUI_ON_COMMAND("Whisper",		OnCommandWhisper)
AUI_ON_COMMAND("Invite",		OnCommandInvite)
AUI_ON_COMMAND("Chat",			OnCommandChat)
AUI_ON_COMMAND("History",		OnCommandHistory)
AUI_ON_COMMAND("Update",		OnCommandUpdate)
AUI_ON_COMMAND("MoveGroup",		OnCommandMoveGroup)
AUI_ON_COMMAND("DeleteFriend",	OnCommandDeleteFriend)
AUI_ON_COMMAND("InviteChannel",	OnCommandInviteChannel)
AUI_ON_COMMAND("JoinChannel",	OnCommandJoinChannel)
AUI_ON_COMMAND("CopyName",		OnCommandCopyName)
AUI_ON_COMMAND("Garden",		OnCommandGarden)
AUI_ON_COMMAND("Btn_Rose",		OnCommandRose)
AUI_ON_COMMAND("Btn_EditName",	OnCommandAddRemark)
AUI_END_COMMAND_MAP()

CDlgFriendOptionName::CDlgFriendOptionName()
{
}

CDlgFriendOptionName::~CDlgFriendOptionName()
{
}

void CDlgFriendOptionName::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
}

void CDlgFriendOptionName::OnCommandWhisper(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	int idPlayer = pTree->GetItemData(pItem);
	ACString strName = pTree->GetItemText(pItem);
	if (strName[1]==_AL(' '))	// GT图标后面有一个空格
		strName.CutLeft(2);

	//
	{
	   strName = *(ACString*)(pTree->GetItemDataPtr(pItem));
	}
	GetGameUIMan()->SwitchToPrivateChat(strName);
	Show(false);
}

void CDlgFriendOptionName::OnCommandInvite(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	int idPlayer = pTree->GetItemData(pItem);
	ACString strName = pTree->GetItemText(pItem);
	if (strName[1]==_AL(' '))	// GT图标后面有一个空格
		strName.CutLeft(2);
	CECGameSession *pSession = GetGameSession();
	
	pSession->c2s_CmdTeamInvite(idPlayer);
	GetGameUIMan()->AddChatMessage(GetGameUIMan()->GetStringFromTable(203), GP_CHAT_MISC);
	Show(false);
}

void CDlgFriendOptionName::OnCommandChat(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	int idPlayer = pTree->GetItemData(pItem);
	ACString strPlayer = *((ACString*)(pTree->GetItemDataPtr(pItem)));
	ACString strName = pTree->GetItemText(pItem);
	if (strName[1]==_AL(' '))	// GT图标后面有一个空格
		strName.CutLeft(2);
	int i;
	char szName[40];
	PAUIDIALOG pDlgChat;

	for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
	{
		sprintf(szName, "Win_FriendChat%d", i);
		pDlgChat = GetGameUIMan()->GetDialog(szName);
		if( (int)pDlgChat->GetData() == idPlayer )
		{
			pDlgChat->Show(true);

			return;
		}
	}
	for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
	{
		sprintf(szName, "Win_FriendChat%d", i);
		pDlgChat = GetGameUIMan()->GetDialog(szName);
		if( (int)pDlgChat->GetData() == 0 )
		{
			ACHAR szUser[40];
			ACString strText;
			PAUIOBJECT pName = pDlgChat->GetDlgItem("Txt_ToWho");
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pDlgChat->GetDlgItem("Txt_Content");
			PAUITEXTAREA pText = (PAUITEXTAREA)pDlgChat->GetDlgItem("Txt_Chat");

			AUI_ConvertChatString(strName, szUser);
			((CDlgFriendChat *)pDlgChat)->SetFriendName(szUser);
			strText.Format(GetGameUIMan()->GetStringFromTable(548), szUser);
			pName->SetText(strText);

			pText->SetText(_AL(""));
			
			pEdit->SetText(_AL(""));
			pEdit->SetIsAutoReturn(true);
			
			pDlgChat->SetData(idPlayer);
			//pDlgChat->SetText(strName);
			pDlgChat->SetText(strPlayer);
			pDlgChat->Show(true);
			pDlgChat->ChangeFocus(pEdit);
			
			break;
		}
	}
	if( i >= CECGAMEUIMAN_MAX_CHATS )	// No free chat window.
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(540),
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	Show(false);
}

void CDlgFriendOptionName::OnCommandHistory(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	int idPlayer = pTree->GetItemData(pItem);
	
	CDlgFriendHistory* pDlgHis = GetGameUIMan()->m_pDlgFriendHistory;

	GetGameUIMan()->m_pDlgFriendList->BuildFriendList(pDlgHis);
	GetGameUIMan()->m_pDlgFriendHistory->BuildChatHistory(idPlayer);
	pDlgHis->SetData(idPlayer);
	pDlgHis->Show(true);
	Show(false);
}

void CDlgFriendOptionName::OnCommandMoveGroup(const char *szCommand)
{
	/*CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;
	
	int idPlayer = pTree->GetItemData(pItem);
	CECFriendMan::GROUP *pGroup;
	P_AUITREEVIEW_ITEM pItemGroup = pTree->GetParentItem(pItem);
	int i, idGroup = pTree->GetItemData(pItemGroup);
	CDlgFriendGroup* pDlgGroup = GetGameUIMan()->m_pDlgFriendGroup;
	PAUICOMBOBOX pCombo = pDlgGroup->m_pComboGroup;
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();

	pCombo->ResetContent();
	for( i = 0; i < pMan->GetGroupNum(); i++ )
	{
		pGroup = pMan->GetGroupByIndex(i);
		pCombo->AddString(pGroup->strName);
		pCombo->SetItemData(i, pGroup->iGroup);
		if( idGroup == pGroup->iGroup )
			pCombo->SetCurSel(i);
	}

	pDlgGroup->Show(true);
	pDlgGroup->SetData(idPlayer);
	Show(false);*/

	if(0 == GetGameUIMan()->CommonMessage("Win_FriendOptionName_Grouplist","open",0,0))
	{
	    Show(false);
	}
}

void CDlgFriendOptionName::OnCommandDeleteFriend(const char *szCommand)
{
	GetGameUIMan()->MessageBox("Game_DelFriend", GetGameUIMan()->GetStringFromTable(538),
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
	Show(false);
}

void CDlgFriendOptionName::OnCommandInviteChannel(const char *szCommand)
{
	Show(false);
// 	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
// 	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
// 	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
// 	if( !pItem ) return;
// 	
// 	int idPlayer = pTree->GetItemData(pItem);
// 	int idRoom = GetGameUIMan()->m_pDlgChannelChat->GetRoomID();
// 	if( idRoom != 0 )
// 	{
// 		GetGameSession()->chatroom_Invite(idRoom, idPlayer);
// 		GetGameUIMan()->AddChatMessage(GetStringFromTable(633), GP_CHAT_MISC);
// 	}
}

void CDlgFriendOptionName::OnCommandJoinChannel(const char *szCommand)
{
 	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
 	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
 	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;
	Show(false);
}

void CDlgFriendOptionName::OnCommandUpdate(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	ACString strName = pTree->GetItemText(pItem);
	if (strName[1]==_AL(' '))	// GT图标后面有一个空格
		strName.CutLeft(2);

	//
	{
	   strName = *(ACString*)(pTree->GetItemDataPtr(pItem));
	}
	
	GetGameSession()->SendPrivateChatData(strName,
		_AL("R"), GNET::CHANNEL_USERINFO, pTree->GetItemData(pItem));
	GetGameSession()->get_friend_gt_status(GetHostPlayer()->GetCharacterID(), pTree->GetItemData(pItem));
	Show(false);
}

void CDlgFriendOptionName::OnCommandCopyName(const char * szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;

	if( OpenClipboard(m_pA3DDevice->GetDeviceWnd()) )
	{
		const ACHAR * szName = pTree->GetItemText(pItem);

		if (szName[1]==_AL(' '))	// GT图标后面有一个空格
			szName += 2;

		HLOCAL hLocal = GlobalAlloc(LMEM_ZEROINIT, a_strlen(szName) * 2 + 2);
		if( hLocal )
		{
			ACHAR *pszName = (ACHAR *)GlobalLock(hLocal);
			a_strcpy(pszName, szName);
			EmptyClipboard();
#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, pszName);
#else
			SetClipboardData(CF_TEXT, pszName);
#endif
			GlobalUnlock(hLocal);
		}

		CloseClipboard();
	}
	Show(false);
}
void CDlgFriendOptionName::OnCommandGarden(const char * szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;
	htc2s_SendCmdGetCompoList(pTree->GetItemData(pItem));
	Show(false);
}

void CDlgFriendOptionName::OnCommandRose(const char * szCommand)
{
	Show(false);
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;
	
	ACString strName = pTree->GetItemText(pItem);
	if (strName[1]==_AL(' '))	// GT图标后面有一个空格
		strName.CutLeft(2);

	//
	{
	   strName = *(ACString*)(pTree->GetItemDataPtr(pItem));
	}
	int idPlayer = pTree->GetItemData(pItem);

	DlgFlowers*	pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
	if(pDlg)
	{
		pDlg->Show(true);
		pDlg->UpdateUI(idPlayer,strName);
		
	}
}
void CDlgFriendOptionName::OnCommandAddRemark(const char *szCommand)
{
	CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
	PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
	P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
	if( !pItem ) return;
	int idPlayer = pTree->GetItemData(pItem);

	PAUIDIALOG pInputDlg = GetGameUIMan()->GetDialog("Win_InputString12");
	if(NULL != pInputDlg)
	{
        PAUIEDITBOX  pEdtInput = (PAUIEDITBOX)(pInputDlg->GetDlgItem("DEFAULT_Txt_Input"));
		pEdtInput->SetMaxLength(8);
		pEdtInput->SetText(_AL(""));
		pInputDlg->SetData(CDlgInputName::INPUTNAME_ADD_FRIEND_REMARK);
		pInputDlg->SetDataPtr((void*)idPlayer,"int");
		pInputDlg->Show(true);
	}
}

void CDlgFriendOptionName::OnTick()
{
	CECGameUIMan * pGameUI = GetGameUIMan();
	if (pGameUI->GetActiveDialog()!=this && !pGameUI->GetDialog("Win_GamePoker_Hall")->IsShow() && !pGameUI->GetDialog("Win_GamePoker_Match")->IsShow())
		Show(false);
}



AUI_BEGIN_COMMAND_MAP(CDlgFriendMoveGroup, CDlgBase)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(CDlgFriendMoveGroup,CDlgBase)
AUI_ON_EVENT("Lst_Grouplist",	WM_LBUTTONDOWN,		OnEventLButtonDown_GroupList)
AUI_END_EVENT_MAP()

bool CDlgFriendMoveGroup::OnInitDialog()
{
    m_ListGroup = (AUIListBox*)(GetDlgItem("Lst_Grouplist"));
	return true;
}
int  CDlgFriendMoveGroup::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
     if(strMsg == "open")
	 {
		 CDlgFriendList* pDlgFriend = GetGameUIMan()->m_pDlgFriendList;
		 PAUITREEVIEW pTree = pDlgFriend->m_pTvFriendList;
		 P_AUITREEVIEW_ITEM pItem = pTree->GetSelectedItem();
		 CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
		 if( NULL == pItem || NULL == pMan || pMan->GetGroupNum() <= 0) 
			 return 1;

		 int idPlayer = pTree->GetItemData(pItem);
		 CECFriendMan::GROUP *pGroup;
		 P_AUITREEVIEW_ITEM pItemGroup = pTree->GetParentItem(pItem);
		 int idGroup = pTree->GetItemData(pItemGroup);
		 

		 m_ListGroup->ResetContent();
		 for( int i = 0; i < pMan->GetGroupNum(); i++ )
		 {
			 pGroup = pMan->GetGroupByIndex(i);
			 m_ListGroup->AddString(pGroup->strName);
			 m_ListGroup->SetItemData(i, pGroup->iGroup);
			 if( idGroup == pGroup->iGroup )
				m_ListGroup->SetCurSel(i);
		 }
         
		 Show(true);
		 SetData(idPlayer);
		 
	 }
     return 0;
}
void CDlgFriendMoveGroup::OnEventLButtonDown_GroupList(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	 if(m_ListGroup->GetCurSel() >= 0 && m_ListGroup->GetCurSel() < m_ListGroup->GetCount())
	 {
		 int idPlayer = GetData();
		 int idGroup = m_ListGroup->GetItemData(m_ListGroup->GetCurSel());

		 GetGameSession()->friend_SetGroup(idGroup, idPlayer);
		 Show(false);
	 }
}