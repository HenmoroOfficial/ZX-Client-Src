// Filename	: DlgMailList.cpp
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#include "DlgMailList.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "DlgMailRead.h"
#include "DlgMailWrite.h"
#include "DlgMiniMap.h"
#include "DlgMailOption.h"
#include "EC_GameUIMisc.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICheckBox.h"
#include "LuaEvent.h"

#include "getmaillist_re.hpp"
#include "deletemail_re.hpp"
#include "preservemail_re.hpp"
#include "getmail_re.hpp"
#include "getmailattachobj_re.hpp"
#include "announcenewmail.hpp"
#include "gconsignmailinfo"


AUI_BEGIN_COMMAND_MAP(CDlgMailList, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)
AUI_ON_COMMAND("new",				OnCommandNew)
AUI_ON_COMMAND("read",				OnCommandRead)
AUI_ON_COMMAND("delete",			OnCommandDelete)
AUI_ON_COMMAND("sortsubject",		OnCommandSortSubject)
AUI_ON_COMMAND("sortsender",		OnCommandSortSender)
AUI_ON_COMMAND("sortattach",		OnCommandSortAttach)
AUI_ON_COMMAND("sortresttime",		OnCommandSortRestTime)
AUI_ON_COMMAND("Chk_RefuseToAccept",OnCommandCheck)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgMailList, CDlgBase)

AUI_ON_EVENT("Lst_Mail",		WM_RBUTTONUP,		OnEventRButtonUp_ListMail)

AUI_END_EVENT_MAP()

CDlgMailList::CDlgMailList()
{
	m_pLst_Mail = NULL;
	m_bSortSubject = false;
	m_bSortSender = false;
	m_bSortAttach = false;
	m_bSortRestTime = false;
	m_bChecked = 0;
}

CDlgMailList::~CDlgMailList()
{
}

bool CDlgMailList::OnInitDialog()
{
	DDX_Control("Lst_Mail", m_pLst_Mail);

	return true;
}

void CDlgMailList::OnShowDialog()
{
	m_pLst_Mail->ResetContent();
	GetGameSession()->mail_GetList();
	m_bChecked = ((AUICheckBox*) GetDlgItem("Chk_RefuseToAccept"))->IsChecked() ? 0 : 1;
}
void CDlgMailList::OnHideDialog(){
	unsigned char c = ((AUICheckBox*) GetDlgItem("Chk_RefuseToAccept"))->IsChecked() ? 0 : 1;
	if(c != m_bChecked){
		GetGameSession()->SendFriendCallbackSubscribe(c);
		m_bChecked = c;
	}
}

void CDlgMailList::OnCommandCheck(const char* szCommand){
	if(((AUICheckBox*) GetDlgItem("Chk_RefuseToAccept"))->IsChecked())
		GetGameUIMan()->MessageBox("FriendCallBack",GetStringFromTable(10527),MB_YESNO);
}

void CDlgMailList::OnCommandCancel(const char* szCommand)
{
	GetGameUIMan()->EndNPCService();
	if( GetGameUIMan()->m_pDlgMailRead->IsShow() )
		GetGameUIMan()->m_pDlgMailRead->OnCommandCancel("");
	if( GetGameUIMan()->m_pDlgMailWrite->IsShow() )
		GetGameUIMan()->m_pDlgMailWrite->OnCommandCancel("");
	Show(false);
}

void CDlgMailList::OnCommandNew(const char* szCommand)
{
	if( GetGameUIMan()->m_pDlgMailWrite->IsEnabled() )
		GetGameUIMan()->m_pDlgMailWrite->CreateNewMail();
}

void CDlgMailList::OnCommandRead(const char* szCommand)
{
	if( GetGameUIMan()->m_pDlgMailRead->IsEnabled() )
	{
		int nSel = m_pLst_Mail->GetCurSel();
		if( nSel >= 0 && nSel < m_pLst_Mail->GetCount() && 
			m_pLst_Mail->GetItemTextColor(nSel) != 0xFF7F7F7F )
		{
			SetWaitingResponse(m_pLst_Mail->GetItemData(nSel));
			GetGameSession()->mail_Get(m_pLst_Mail->GetItemData(nSel));
		}
	}
}

void CDlgMailList::OnCommandDelete(const char* szCommand)
{
	int nSel = m_pLst_Mail->GetCurSel();
	if( nSel >= 0 && nSel < m_pLst_Mail->GetCount() && 
		m_pLst_Mail->GetItemTextColor(nSel) != 0xFF7F7F7F )
	{
		OnEventRButtonUp_ListMail(0, 0, NULL);
		GetGameUIMan()->m_pDlgMailOption->OnCommand("Delete");
	}
}

void CDlgMailList::OnCommandSortSubject(const char* szCommand)
{
	m_bSortSubject = !m_bSortSubject;
	if( m_bSortSubject )
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_UNISTRING, 1);
	else
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_UNISTRING, 1);
}

void CDlgMailList::OnCommandSortSender(const char* szCommand)
{
	m_bSortSender = !m_bSortSender;
	if( m_bSortSender )
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_UNISTRING, 2);
	else
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_UNISTRING, 2);
}

void CDlgMailList::OnCommandSortAttach(const char* szCommand)
{
	m_bSortAttach = !m_bSortAttach;
	if( m_bSortAttach )
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_UNISTRING, 3);
	else
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_UNISTRING, 3);
}

void CDlgMailList::OnCommandSortRestTime(const char* szCommand)
{
	m_bSortRestTime = !m_bSortRestTime;
	if( m_bSortRestTime )
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_ASCENT, AUILISTBOX_SORTTYPE_INT, 4);
	else
		m_pLst_Mail->SortItems(AUILISTBOX_SORT_DESCENT, AUILISTBOX_SORTTYPE_INT, 4);
}

void CDlgMailList::OnEventRButtonUp_ListMail(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	int nSel = m_pLst_Mail->GetCurSel();
	int idMail = m_pLst_Mail->GetItemData(nSel, 0);
	int idSender = m_pLst_Mail->GetItemData(nSel, 1);
	if( nSel >= 0 && nSel < m_pLst_Mail->GetCount() && 
		m_pLst_Mail->GetItemTextColor(nSel) != 0xFF7F7F7F )
	{
		CSplit s(m_pLst_Mail->GetText(nSel));
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));

		PAUIDIALOG pMenu = (AUIDialog *)GetGameUIMan()->m_pDlgMailOption;
		if( a_stricmp(vec[3], GetStringFromTable(805)) == 0 )
			pMenu->SetData(idMail | 0x10000);
		else
			pMenu->SetData(idMail);
		pMenu->SetDataPtr((void*)idSender);

		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
		int x = GET_X_LPARAM(lParam) - p->X;
		int y = GET_Y_LPARAM(lParam) - p->Y;
		pMenu->SetPosEx(x, y);
		
		pMenu->SetText(m_pLst_Mail->GetText(nSel));

		if( a_stricmp(vec[3], GetStringFromTable(805)) == 0 )
		{
			EnableObject(pMenu, "Preserve", false);
			EnableObject(pMenu, "NoPreserve", false);
		}
		else if( a_stricmp(vec[4], GetStringFromTable(787)) == 0 )
		{
			EnableObject(pMenu, "Preserve", false);
			EnableObject(pMenu, "NoPreserve", true);
		}
		else
		{
			EnableObject(pMenu, "Preserve", true);
			EnableObject(pMenu, "NoPreserve", false);
		}
		
		EnableObject(pMenu, "Reply", idSender != 0);
		pMenu->Show(true);
	}
}

void CDlgMailList::MailAction(int idAction, void *pData)
{
	ACString strText;
	if( idAction == MAIL_ACTION_GETMAILLIST_RE )
	{
		if( ((GetMailList_Re *)pData)->retcode == ERR_SUCCESS )
		{
			GMailHeaderVector MailList = ((GetMailList_Re *)pData)->maillist;
			DWORD i;
			for( i = 0; i < MailList.size(); i++ )
			{
				GNET::GMailHeader mail = MailList[i];
				ACString strSubject;
				ACString strMailFrom;
				
				switch(mail.sndr_type)
				{
				case _MST_AUCTION:
					strMailFrom = GetStringFromTable(5001);
					switch(mail.sender)
					{
					case _AUCTION_ITEM_SOLD:
						strSubject = GetStringFromTable(5101);
						break;
					case _AUCTION_BID_WIN:
						strSubject = GetStringFromTable(5102);
						break;
					case _AUCTION_BID_LOSE:
						strSubject = GetStringFromTable(5103);
						break;
					case _AUCTION_BID_CANCEL:
						strSubject = GetStringFromTable(5104);
						break;
					case _AUCTION_BID_TIMEOUT:
						strSubject = GetStringFromTable(5105);
						break;
					default:
						strSubject = _AL(" ");
						break;
					}
					break;
					
				case _MST_LOCAL_CONSIGN:
					{
					strMailFrom = GetStringFromTable(5003);
					GConsignMailInfo cmi;
					Marshal::OctetsStream(mail.title) >> cmi;
					switch(mail.sender)
					{
					case _CONSIGN_BUY:
						strSubject = GetStringFromTable(10260);
						break;
					case _CONSIGN_SOLD:
						strSubject = GetStringFromTable(10263);
						break;
					case _CONSIGN_CANCEL:
						strSubject = GetStringFromTable(10266);
						break;
					case _CONSIGN_FORCED_CANCEL:
						strSubject = GetStringFromTable(10269);
						break;
					case _CONSIGN_EXPIRE:
						strSubject = GetStringFromTable(10272);
						break;
					case _CONSIGN_FAIL:
						strSubject = GetStringFromTable(10275);
						break;
					}
					break;
					}
				case _MST_ROLE_CONSIGN_NOTIFY:
					{
						strMailFrom = GetStringFromTable(5003);
						strSubject = GetStringFromTable(5000);
						break;
					}
				case _MST_FACTION_PK_BONUS_NOTIFY:
					{
						strMailFrom = GetStringFromTable(5004);
						strSubject = GetStringFromTable(5110);
						break;
					}
				case _MST_KINGDOM_AWARD:
					{
						strMailFrom = GetStringFromTable(5005);
						strSubject  = GetStringFromTable(5111);
						break;
					}
						
					default:
					strMailFrom = ACString((const ACHAR*)mail.sender_name.begin(), mail.sender_name.size() / sizeof (ACHAR));
					strSubject = ACString((const ACHAR*)mail.title.begin(), mail.title.size() / sizeof (ACHAR));
					ConvertChatString(strSubject);
					break;
				}
				ACString strAttach;
				ACString strPreserve;
				ACString strUnRead;
				if( mail.attribute & ( _MA_ATTACH_OBJ | _MA_ATTACH_MONEY) )
					strAttach = GetStringFromTable(805);
				else
					strAttach = _AL(" ");
				if( mail.attribute & _MA_PRESERVE )
					strPreserve = GetStringFromTable(787);
				else
					strPreserve = GetPreserveTime(mail.send_time, mail.attribute & _MA_UNREAD);

				if( mail.attribute & _MA_UNREAD )
					strUnRead = GetStringFromTable(788);
				else
					strUnRead = _AL(" ");

				if(strSubject.GetLength()==0)
					strSubject = _AL(" ");
				if(strMailFrom.GetLength()==0)
					strMailFrom = _AL(" ");
				strText.Format(_AL("%s\t%s\t  %s\t%s\t%s"),strUnRead, strSubject,
						strMailFrom, strAttach, strPreserve);
				m_pLst_Mail->AddString(strText);
				m_pLst_Mail->SetItemData(i, mail.id, 0);
				if( mail.sndr_type == _MST_PLAYER )
					m_pLst_Mail->SetItemData(i, mail.sender, 1);
				else
					m_pLst_Mail->SetItemData(i, 0, 1);
				m_pLst_Mail->SetItemData(i, mail.send_time, 2);
			}
		}
	}
	else if( idAction == MAIL_ACTION_GETMAIL_RE )
	{
		GetMail_Re *pMail = (GetMail_Re *)pData;
		if( pMail->retcode == ERR_SUCCESS )
		{
			int i;
			bool bUnRead = false;
			for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
			{
				CSplit s(m_pLst_Mail->GetText(i));
				CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
				if( m_pLst_Mail->GetItemData(i) == pMail->mail.header.id )
				{
					m_pLst_Mail->SetItemTextColor(i, 0xFFFFFFFF);
			
					if( a_stricmp(vec[0] , GetStringFromTable(788)) == 0 )
					{
						vec[0] = _AL(" ");
						m_pLst_Mail->SetItemData(i, GetGame()->GetServerGMTTime(), 2);
// 						if( a_stricmp(vec[3], GetStringFromTable(805)) != 0 )
// 							vec[4] = GetPreserveTime((int)m_pLst_Mail->GetItemData(i, 2), false);
						strText.Format(_AL("%s\t%s\t%s\t%s\t%s"),
								vec[0], vec[1], vec[2], vec[3], vec[4] );
						m_pLst_Mail->SetText(i, strText);
					}
				}
				if( a_stricmp(vec[0] , GetStringFromTable(788)) == 0 )
					bUnRead = true;
			}
			if( !bUnRead )
			{
				GetGameUIMan()->m_pDlgMiniMap->SetNewMail(-1);
				GetGameSession()->mail_CheckNew();
			}
		}
		if( GetGameUIMan()->m_pDlgMailRead->IsEnabled() )
			GetGameUIMan()->m_pDlgMailRead->ReadMail(pData);
	}
	else if( idAction == MAIL_ACTION_GETATTACH_RE )
	{
		GetMailAttachObj_Re *pAttach = (GetMailAttachObj_Re *)pData;
		if( pAttach->retcode == ERR_SUCCESS && 
			pAttach->item_left == 0 && pAttach->money_left == 0 )
		{
			int i;
			for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
			{
				CSplit s(m_pLst_Mail->GetText(i));
				CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
				if( m_pLst_Mail->GetItemData(i) == pAttach->mail_id )
				{
					vec[3] = _AL(" ");
					m_pLst_Mail->SetItemData(i, GetGame()->GetServerGMTTime(), 2);
//					vec[4] = GetPreserveTime((int)m_pLst_Mail->GetItemData(i, 2), false);
					strText.Format(_AL("%s\t%s\t%s\t%s\t%s"),
							vec[0], vec[1], vec[2], vec[3], vec[4] );
					m_pLst_Mail->SetText(i, strText);
					break;
				}
			}
		}
		GetGameUIMan()->m_pDlgMailRead->GetAttachRe(pData);
	}
	else if( idAction == MAIL_ACTION_DELETEMAIL_RE )
	{
		DeleteMail_Re *pDeleteMail = (DeleteMail_Re *)pData;
		if( pDeleteMail->retcode == ERR_SUCCESS )
		{
			if( pDeleteMail->mail_id == GetGameUIMan()->m_pDlgMailRead->m_idMail )
				GetGameUIMan()->m_pDlgMailRead->OnCommandCancel("");
			int i;
			for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
				if( m_pLst_Mail->GetItemData(i) == pDeleteMail->mail_id )
				{
					m_pLst_Mail->DeleteString(i);
					break;
				}
			bool bUnRead = false;
			for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
			{
				CSplit s(m_pLst_Mail->GetText(i));
				CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
				if( a_stricmp(vec[0] , GetStringFromTable(788)) == 0 )
				{
					bUnRead = true;
					break;
				}
			}
			if( !bUnRead )
			{
				GetGameUIMan()->m_pDlgMiniMap->SetNewMail(-1);
				GetGameSession()->mail_CheckNew();
			}
		}
	}
	else if( idAction == MAIL_ACTION_PRESERVEMAIL_RE )
	{
		PreserveMail_Re *pPreserveMail = (PreserveMail_Re *)pData;
		if( pPreserveMail->retcode == ERR_SUCCESS )
		{
			int i;
			for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
				if( m_pLst_Mail->GetItemData(i) == pPreserveMail->mail_id )
				{
					m_pLst_Mail->SetItemTextColor(i, 0xFFFFFFFF);
					CSplit s(m_pLst_Mail->GetText(i));
					CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
					if( pPreserveMail->blPreserve )
						vec[4] = GetStringFromTable(787);
					else
					{
						m_pLst_Mail->SetItemData(i, GetGame()->GetServerGMTTime(), 2);
						vec[4] = GetPreserveTime((int)m_pLst_Mail->GetItemData(i, 2), 
							a_stricmp(vec[0], GetStringFromTable(788)) == 0 );
					}
					strText.Format(_AL("%s\t%s\t%s\t%s\t%s"),
							vec[0], vec[1], vec[2], vec[3], vec[4] );
					m_pLst_Mail->SetText(i, strText);

					break;
				}
		}
	}
	else if( idAction == MAIL_ACTION_SENDMAIL_RE )
	{
		GetGameUIMan()->m_pDlgMailWrite->SendMailRe(pData);
	}
	else if( idAction == MAIL_ACTION_ANNOUNCENEWMAIL_RE)
	{
		AnnounceNewMail *p = (AnnounceNewMail *)pData;
		GetGameUIMan()->m_pDlgMiniMap->SetNewMail(p->remain_time);
		if( p->remain_time == 0 && IsShow() )
		{
			m_pLst_Mail->ResetContent();
			GetGameSession()->mail_GetList();
		}
	}
}

ACString CDlgMailList::GetPreserveTime(int nSendTime, bool bUnRead)
{
	ACString strTime;
	if( bUnRead )
		strTime.Format(GetStringFromTable(789), 7 - (GetGame()->GetServerGMTTime() - nSendTime) / (3600 * 24));
	else
		strTime.Format(GetStringFromTable(789), 7 - (GetGame()->GetServerGMTTime() - nSendTime) / (3600 * 24));
	return strTime;
}

void CDlgMailList::SetWaitingResponse(int idMail)
{
	int i;
	for( i = 0; i < m_pLst_Mail->GetCount(); i++ )
		if( m_pLst_Mail->GetItemData(i) == (DWORD)idMail )
		{
			m_pLst_Mail->SetItemTextColor(i, 0xFF7F7F7F);
			break;
		}
}

void CDlgMailList::EnableObject(PAUIDIALOG pDlg, const char *pszCommand, bool bEnable)
{
	PAUIOBJECT pObj = pDlg->GetDlgItem(pszCommand);
	if (pObj)
		pObj->Enable(bEnable);
	else
		assert(0);
}