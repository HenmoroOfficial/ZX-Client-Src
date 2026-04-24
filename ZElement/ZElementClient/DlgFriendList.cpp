// File		: DlgFriendList.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#include "AUI\\AUIEditBox.h"
#include "AUI\\CSplit.h"
#include "DlgFriendList.h"
#include "DlgInfo.h"
#include "DlgInputName.h"
#include "DlgSetting.h"
#include "DlgFriendOptionName.h"
#include "DlgFriendOptionGroup.h"
#include "DlgFriendOptionNormal.h"
#include "DlgEnemyOptionName.h"
#include "DlgFlower.h"
//#include "DlgChannelChat.h"
#include "DlgQuickAction.h"
#include "DlgChat.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_HostPlayer.h"
#include "EC_World.h"
#include "EC_Friend.h"
#include "EL_Precinct.h"
#include "EC_Global.h"
#include "EC_UIManager.h"
#include "EC_Configs.h"
#include "LuaEvent.h"
#include "Network\\gnetdef.h"
#include "getenemies_re.hpp"
#include "updateenemy_re.hpp"
#include "updatefriendgtstatus.hpp"
#include "gategetgtrolestatus_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFriendList, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)
AUI_ON_COMMAND("Btn_Blacklist",	OnCommandSetPage)
AUI_ON_COMMAND("enemy",		OnCommandSetPage)
AUI_ON_COMMAND("friend",	OnCommandSetPage)
AUI_ON_COMMAND("temp",		OnCommandSetPage)
AUI_ON_COMMAND("add",		OnCommandAdd)
AUI_ON_COMMAND("addfriend",	OnCommandAddFriend)
AUI_ON_COMMAND("remove",	OnCommandRemove)
AUI_ON_COMMAND("invite",	OnCommandInvite)
AUI_ON_COMMAND("chat",		OnCommandChat)
AUI_ON_COMMAND("preserve",	OnCommandPreserve)
AUI_ON_COMMAND("update",	OnCommandUpdate)
AUI_ON_COMMAND("setting",	OnCommandSetting)
AUI_ON_COMMAND("Btn_ShowClass",	OnCommandCircle)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgFriendList, CDlgBase)

AUI_ON_EVENT("Tv_FriendList",	WM_LBUTTONDBLCLK,	OnEventButtonDBCLK)
AUI_ON_EVENT("Tv_FriendList",	WM_RBUTTONUP,		OnEventRButtonUp)
AUI_ON_EVENT("Lst_EnemyList",	WM_RBUTTONUP,		OnEventRButtonUpEnemy)

AUI_END_EVENT_MAP()

CDlgFriendList::CDlgFriendList()
{
}

CDlgFriendList::~CDlgFriendList()
{
}

bool CDlgFriendList::OnInitDialog()
{
	m_pTvFriendList = (PAUITREEVIEW)GetDlgItem("Tv_FriendList");
	m_pLstEnemyList = (PAUILISTBOX)GetDlgItem("Lst_EnemyList");
	m_pBtn_Add = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Add");
	m_pBtn_AddFriend = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_AddFriend");
	m_pBtn_Preserve = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Preserve");
	m_pBtn_Invite = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Invite");
	m_pBtn_Update = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Update");
	m_pBtn_Chat = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Chat");
	m_pBtn_Setting = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Setting");
	m_pBtn_Circle = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ShowClass");
	m_pBtn_AddFriend->Show(false);
	m_pLstEnemyList->Show(false);
	m_pBtn_Preserve->Show(false);
	m_pBtn_Update->Show(false);
	CheckRadioButton(1, 1);
	m_szCurPage = "friend";
	m_mapEnemyList.clear();
	m_bGetEnemyList = false;
	m_dwBlackListGetNameTime = 0;
	if (m_szName=="Win_FriendList")
	{
		PAUITREEVIEW pTree = (PAUITREEVIEW)GetDlgItem("Tv_FriendList");
		if (GetGameUIMan()->GetControlImgLst()->size()>0)
		{
			pTree->SetImageList(GetGameUIMan()->GetControlImgLst());
			pTree->SetItemMask(0xffffffff);
		}	
	}

	return true;
}

void CDlgFriendList::OnShowDialog()
{
	if (GetDlgItem("Btn_ShowClass"))
	{
		GetDlgItem("Btn_ShowClass")->Show(GetHostPlayer()->GetSrcServerID()==0);
	}
}

void CDlgFriendList::OnTick()
{
	m_pBtn_Circle->Show(GetHostPlayer()->GetCircleID()!=0);

	if( m_vecBlackCache.size() == 0 )
		return;
	
	CECGameRun *pGameRun = GetGameRun();
	DWORD dwTick = GetTickCount();
	
	for(int i = 0; i < (int)m_vecBlackCache.size(); i++ )
	{
		int idRole = pGameRun->GetPlayerID(m_vecBlackCache[i].strName);
		if( idRole > 0 )
		{
			ACString strName = m_vecBlackCache[i].strName;
			m_vecBlackCache.erase(m_vecBlackCache.begin() + i);
			AddBlackList(strName);
			i--;
		}
		else if( dwTick - m_vecBlackCache[i].dwTime >= 10 * 1000 )
		{
			ACString strMsg;
			strMsg.Format(GetGameUIMan()->GetStringFromTable(261), m_vecBlackCache[i].strName);
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_MISC);
			m_vecBlackCache.erase(m_vecBlackCache.begin() + i);
			i--;
		}
	}
}

int CDlgFriendList::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "get_status")
	{
		GateGetGTRoleStatus_Re* pFriendGTStatus = (GateGetGTRoleStatus_Re*)wParam;
		for (UINT i=0; i<pFriendGTStatus->statuslist.size(); ++i)
		{
			GetHostPlayer()->GetFriendMan()->ChangeFriendGTStatus(pFriendGTStatus->statuslist[i].roleid, pFriendGTStatus->statuslist[i].status);
			FriendAction(pFriendGTStatus->statuslist[i].roleid, -1, FRIEND_ACTION_INFO_GT, pFriendGTStatus->statuslist[i].status);
		}
	}
	else if (strMsg == "update_status")
	{
		UpdateFriendGTStatus* pFriendGTStatus = (UpdateFriendGTStatus*)wParam;
		GetHostPlayer()->GetFriendMan()->ChangeFriendGTStatus(pFriendGTStatus->friendstatus.roleid, pFriendGTStatus->friendstatus.status);
		FriendAction(pFriendGTStatus->friendstatus.roleid, -1, FRIEND_ACTION_INFO_GT, pFriendGTStatus->friendstatus.status);
	}

	return 1;
}

void CDlgFriendList::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
}

void CDlgFriendList::OnCommandSetPage(const char *szCommand)
{
	if( m_szCurPage == szCommand && m_szCurPage != "enemy" )
		return;

	m_szCurPage = szCommand;
	if( strcmp(szCommand, "friend") == 0 )
	{
		m_pTvFriendList->DeleteAllItems();
		CheckRadioButton(1, 1);
		BuildFriendList();
		m_pTvFriendList->Show(true);
		m_pLstEnemyList->Show(false);
		m_pBtn_Add->Show(true);
		m_pBtn_AddFriend->Show(false);
		m_pBtn_Preserve->Show(false);
		if(NULL != m_pBtn_Invite)
		   m_pBtn_Invite->Show(true);
		m_pBtn_Update->Show(false);
		if(NULL != m_pBtn_Chat)
		   m_pBtn_Chat->Show(true);
		m_pBtn_Setting->Show(true);
	}
	else if( strcmp(szCommand, "Btn_Blacklist") == 0  )
	{
		m_pTvFriendList->DeleteAllItems();
		CheckRadioButton(1, 2);
		EC_BLACKLIST_SETTING setting = GetGame()->GetConfigs()->GetBlackListSettings();
		m_vecBlackCache.clear();
		CECGameRun *pGameRun = GetGame()->GetGameRun();
		for (int i = 0; i < EC_BLACKLIST_LEN && setting.idPlayer[i] > 0; i++)
		{
			m_pTvFriendList->InsertItem(pGameRun->GetPlayerName(setting.idPlayer[i], true),
				m_pTvFriendList->GetRootItem());
		}
		m_pTvFriendList->Show(true);
		m_pLstEnemyList->Show(false);
		m_pBtn_Add->Show(true);
		m_pBtn_AddFriend->Show(false);
		m_pBtn_Preserve->Show(false);
		if(NULL != m_pBtn_Invite)
		  m_pBtn_Invite->Show(false);
		m_pBtn_Update->Show(false);
		if(NULL != m_pBtn_Chat)
		  m_pBtn_Chat->Show(false);
		m_pBtn_Setting->Show(false);
		LuaEvent::FirstOpenBlackList();
	}
	else if( strcmp(szCommand, "enemy") == 0  )
	{
		CheckRadioButton(1, 3);
		if( !m_bGetEnemyList )
		{
			m_mapEnemyList.clear();
			GetGameSession()->enemies_get(GetHostPlayer()->GetCharacterID(), 1);
			m_bGetEnemyList = true;
		}
		m_pLstEnemyList->ResetContent();
		ACString strLine, strTime, strPreserve;
		abase::hash_map<int, ENEMY_INFO>::iterator it = m_mapEnemyList.begin();
		for(; it != m_mapEnemyList.end(); ++it )
		{
			long stime = it->second.pktime;
			stime -= GetGame()->GetTimeZoneBias() * 60; // localtime = UTC - bias, in which bias is in minutes
			tm *gtime = gmtime((const time_t*)&stime);
			strTime.Format(GetStringFromTable(814), gtime->tm_year + 1900,
				gtime->tm_mon + 1, gtime->tm_mday, gtime->tm_hour, gtime->tm_min);
			strLine = GetGameUIMan()->GetLine(it->second.lineid);
			if( it->second.status == 1 ) 
				strPreserve = GetStringFromTable(805);
			else
				strPreserve = _AL(" ");
			m_pLstEnemyList->AddString(it->second.name + _AL('\t') + 
				strLine + _AL('\t') + strPreserve);
			m_pLstEnemyList->SetItemData(m_pLstEnemyList->GetCount() - 1, it->second.id);
			m_pLstEnemyList->SetItemHint(m_pLstEnemyList->GetCount() - 1, strTime);
		}
		m_pTvFriendList->Show(false);
		m_pLstEnemyList->Show(true);
		m_pBtn_Add->Show(false);
		m_pBtn_AddFriend->Show(false);
		m_pBtn_Preserve->Show(true);
		if(NULL != m_pBtn_Invite)
		   m_pBtn_Invite->Show(false);
		m_pBtn_Update->Show(true);
		if(NULL != m_pBtn_Chat)
		  m_pBtn_Chat->Show(true);
		m_pBtn_Setting->Show(false);
		LuaEvent::FirstOpenEnemy();
	}
	else if( strcmp(szCommand, "temp") == 0  )
	{
		m_pTvFriendList->DeleteAllItems();
		CheckRadioButton(1, 4);
		CECFriendMan::TempFriendTable& tempFriendTable = GetHostPlayer()->GetFriendMan()->GetTempFriendTable();
		CECFriendMan::TempFriendTable::iterator iter = tempFriendTable.begin();
		for (; iter != tempFriendTable.end(); ++iter)
		{
			CECFriendMan::TempFriendTable::pair_type Pair = tempFriendTable.get(*iter.key());
			CECFriendMan::TEMPFRIEND *tempF = *Pair.first;
			P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->InsertItem( tempF->GetName(),m_pTvFriendList->GetRootItem());
			m_pTvFriendList->SetItemData(pItem, *iter.key());
		}

		m_pTvFriendList->Show(true);
		m_pLstEnemyList->Show(false);
		m_pBtn_Add->Show(false);
		m_pBtn_AddFriend->Show(true);
		m_pBtn_Preserve->Show(false);
		if(NULL != m_pBtn_Invite)
		  m_pBtn_Invite->Show(true);
		m_pBtn_Update->Show(false);
		if(NULL != m_pBtn_Chat)
		  m_pBtn_Chat->Show(true);
		m_pBtn_Setting->Show(false);
	}
}

void CDlgFriendList::OnCommandAdd(const char *szCommand)
{
	CDlgInputName* pDlg = GetGameUIMan()->m_pDlgInputName;
	pDlg->m_pTxtInput->SetText(_AL(""));
	if( m_szCurPage == "friend" )
		pDlg->SetData(CDlgInputName::INPUTNAME_ADD_FRIEND);
	else if( m_szCurPage == "Btn_Blacklist")
	{
		int nCount = m_pTvFriendList->GetCount();
		// blacklist length limit
		if( nCount + m_vecBlackCache.size() >= EC_BLACKLIST_LEN )
		{
			AUIDialog *pMsgBox = NULL;
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(262), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}
		pDlg->SetData(CDlgInputName::INPUTNAME_ADD_BLACKLIST);
	}
	pDlg->Show(true);
}

void CDlgFriendList::OnCommandAddFriend(const char *szCommand)
{
	P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();
	if (pItem)
	{
		int idTarget = m_pTvFriendList->GetItemData(pItem);
		if (idTarget>0)
		{
			const ACHAR *pszName = GetGameRun()->GetPlayerName(idTarget, true);
			CECFriendMan::FRIEND* pFriend = GetHostPlayer()->GetFriendMan()->GetFriendByID(idTarget);
			
			//	In friend list, one player ID may have two names, so check name here !
			if( pFriend && !a_strcmp(pszName, pFriend->GetName()) )
				GetGameUIMan()->AddChatMessage(GetStringFromTable(565), GP_CHAT_MISC);
			else
			{
				GetGameSession()->friend_Add(idTarget, pszName);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(533), GP_CHAT_MISC);
			}
		}
	}
}

void CDlgFriendList::OnCommandChat(const char *szCommand)
{
	if( m_szCurPage == "friend" )
	{
		P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();

		if( pItem )
		{
			{
				GetGameUIMan()->m_pDlgFriendOptionName->OnCommandChat("");
			}
		}
	}
	else if( m_szCurPage == "enemy" )
	{
		int nSel = m_pLstEnemyList->GetCurSel();
		if( nSel >= 0 && nSel < m_pLstEnemyList->GetCount() )
		{
			DWORD id = m_pLstEnemyList->GetItemData(nSel);
			PAUIDIALOG pDlgWhisper = (PAUIDIALOG)GetGameUIMan()->m_pDlgChatWhisper;
			if( pDlgWhisper->IsShow() && pDlgWhisper->GetData() == id ) return;

			pDlgWhisper->Show(true);
			pDlgWhisper->SetData(id);
			pDlgWhisper->GetDlgItem("DEFAULT_Txt_Speech")->SetText(_AL(""));

			PAUITEXTAREA pText = (PAUITEXTAREA)pDlgWhisper->GetDlgItem("Txt_Chat");
			pText->SetText(_AL(""));

			ACHAR szUser[40];
			ACString strText;
			PAUIOBJECT pName = pDlgWhisper->GetDlgItem("Txt_ToWho");
			CSplit s(m_pLstEnemyList->GetText(nSel));
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			AUI_ConvertChatString(vec[0], szUser);
			strText.Format(GetStringFromTable(548), szUser);
			if( GetGameRun()->GetPlayerID(vec[0]) == 0 )
				GetGameSession()->GetPlayerIDByName(vec[0], 0);
			pName->SetText(strText);
		}
	}
	else if( m_szCurPage == "temp" )
	{
		P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();
		if (pItem)
		{
			const ACHAR *pszName = m_pTvFriendList->GetItemText(pItem);
			GetGameUIMan()->SwitchToPrivateChat(pszName);
			Show(false);
		}
	}
}

void CDlgFriendList::OnCommandPreserve(const char *szCommand)
{
	int nSel = m_pLstEnemyList->GetCurSel();
	if( nSel >= 0 && nSel < m_pLstEnemyList->GetCount() )
	{
		GetGameSession()->enemies_update(GetHostPlayer()->GetCharacterID(),
			m_pLstEnemyList->GetItemData(nSel), 1);
	}
}

void CDlgFriendList::OnCommandUpdate(const char *szCommand)
{
	GetGameSession()->enemies_get(GetHostPlayer()->GetCharacterID(), 0);
}

void CDlgFriendList::OnCommandSetting(const char *szCommand)
{
	PAUIDIALOG pMenu = GetGameUIMan()->m_pDlgFriendOptionNormal;
	pMenu->SetPosEx(m_pBtn_Setting->GetPos().x, m_pBtn_Setting->GetPos().y - pMenu->GetSize().cy);
	pMenu->Show(true);
}

void CDlgFriendList::OnCommandCircle(const char *szCommand)
{
	CECHostPlayer* pHost = GetHostPlayer();
	if (pHost->GetCircleID()>0)
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ClassmateManage");
		pDlg->Show(!pDlg->IsShow());
	}
}

void CDlgFriendList::OnCommandInvite(const char *szCommand)
{
	P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();

	if( pItem )
	{
		if( m_szCurPage == "friend" )
		{
			GetGameUIMan()->m_pDlgFriendOptionName->OnCommandInvite("");
		}
		else if ( m_szCurPage == "temp" )
		{
			int idTarget = m_pTvFriendList->GetItemData(pItem);
			if (idTarget>0)
			{
				GetGameSession()->c2s_CmdTeamInvite(idTarget);
				GetGameUIMan()->AddChatMessage(GetStringFromTable(203), GP_CHAT_MISC);
			}
		}
	}
}

void CDlgFriendList::OnCommandRemove(const char *szCommand)
{
	P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();

	if( pItem )
	{
		if( m_szCurPage == "friend" )
		{
			if( m_pTvFriendList->GetParentItem(pItem) != m_pTvFriendList->GetRootItem() )
			{
				GetGameUIMan()->MessageBox("Game_DelFriend", GetGameUIMan()->GetStringFromTable(538),
					MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
				LuaEvent::FirstDeleteFriend();
			}
			
		}
		else if( m_szCurPage == "Btn_Blacklist" )
		{
			int nCount = m_pTvFriendList->GetCount();
			pItem = m_pTvFriendList->GetFirstChildItem(m_pTvFriendList->GetRootItem());
			const ACHAR *szName = m_pTvFriendList->GetItemText(pItem);
			int i = 0;
			while( pItem )
			{
				if( pItem == m_pTvFriendList->GetSelectedItem() )
				{
					P_AUITREEVIEW_ITEM pNextItem;
					pNextItem = m_pTvFriendList->GetNextSiblingItem(pItem);
					m_pTvFriendList->DeleteItem(pItem);
					EC_BLACKLIST_SETTING setting = GetGame()->GetConfigs()->GetBlackListSettings();
					if( i + 1 < nCount )
					{
						memmove(setting.idPlayer + i, setting.idPlayer + i + 1,
							sizeof(int) * (nCount - i - 1));
					}
					setting.idPlayer[nCount - 1] = 0;
					if (pNextItem)
					{
						m_pTvFriendList->SelectItem(pNextItem);
					}
					GetGame()->GetConfigs()->SetBlackListSettings(setting);
					break;
				}
				pItem = m_pTvFriendList->GetNextSiblingItem(pItem);
				i++;
			}
		}
		else if( m_szCurPage == "temp" )
		{
			P_AUITREEVIEW_ITEM pItem = m_pTvFriendList->GetSelectedItem();
			if( pItem )
			{
				GetHostPlayer()->GetFriendMan()->RemoveTempFriend(m_pTvFriendList->GetItemData(pItem));
				m_pTvFriendList->DeleteItem(pItem);
			}
		}
	}

	if( m_szCurPage == "enemy" )
	{
		int nSel = m_pLstEnemyList->GetCurSel();
		if( nSel >= 0 && nSel < m_pLstEnemyList->GetCount() )
		{
			GetGameSession()->enemies_update(GetHostPlayer()->GetCharacterID(),
				m_pLstEnemyList->GetItemData(nSel), 0);
		}
	}
}

void CDlgFriendList::OnEventButtonDBCLK(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	bool bOnIcon;
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
	P_AUITREEVIEW_ITEM pItem = pTree->HitTest(x, y, &bOnIcon);

	if( pItem && !bOnIcon && pTree->GetParentItem(pItem) != pTree->GetRootItem() )
	{
		PAUIDIALOG pDlgMailWrite = GetGameUIMan()->GetDialog("Win_MailWrite");
		if (pDlgMailWrite->IsShow())
		{
			ACString strName = pTree->GetItemText(pItem);
			if (strName[1]==_AL(' '))	// GT图标后面有一个空格
				strName.CutLeft(2);
			PAUIEDITBOX pEdt = (PAUIEDITBOX)pDlgMailWrite->GetDlgItem("Txt_MailTo");
			pEdt->SetText(strName);
		}
		else if(GetGameUIMan()->GetDialog("Win_Rose")->IsShow())
		{
			DlgFlowers* pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
			ACString strName = pTree->GetItemText(pItem);
			if (strName[1]==_AL(' '))	// GT图标后面有一个空格
				strName.CutLeft(2);
			pDlg->UpdateUI(0,strName);
		}
		else
			GetGameUIMan()->m_pDlgFriendOptionName->OnCommandChat("Chat");
	}
}

void CDlgFriendList::OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_szCurPage != "friend" )
		return;

	bool bOnIcon;
	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUITREEVIEW pTree = (PAUITREEVIEW)pObj;
	PAUIDIALOG pMenu = NULL;
	P_AUITREEVIEW_ITEM pItem = pTree->HitTest(x, y, &bOnIcon);
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();

	if( pItem && !bOnIcon )
	{
		if( pTree->GetParentItem(pItem) == pTree->GetRootItem() )
		{
			pMenu = GetGameUIMan()->m_pDlgFriendOptionGroup;
			if( pTree->GetItemData(pItem) == 0)
			{
				EnableObject(pMenu, "GroupColor", false);
				EnableObject(pMenu, "DeleteGroup", false);
			}
			else
			{
				EnableObject(pMenu, "GroupColor", true);
				EnableObject(pMenu, "DeleteGroup", true);
			}
		}
		else
		{
			pMenu = GetGameUIMan()->m_pDlgFriendOptionName;
			CECFriendMan::FRIEND *pFriend;
			pFriend = pMan->GetFriendByID(pTree->GetItemData(pItem));
			if( pTree->GetItemTextColor(pItem) == A3DCOLORRGB(128, 128, 128) || pFriend->iLine == 500)
			{
				EnableObject(pMenu, "Whisper", false);
				EnableObject(pMenu, "Invite", false);
				EnableObject(pMenu, "Update", false);
				EnableObject(pMenu, "JoinChannel", false);
			}
			else
			{
				EnableObject(pMenu, "Whisper", true);
				EnableObject(pMenu, "Invite", true);
				EnableObject(pMenu, "Update", true);
				EnableObject(pMenu, "JoinChannel", true);
			}
			tm tmNow = GetGame()->GetServerLocalTime();
			EnableObject(pMenu, "Btn_Rose", false);
			if ( tmNow.tm_year==113 &&	//2012年
				((tmNow.tm_mon== 2 && ((tmNow.tm_mday == 26 && tmNow.tm_hour >=10) || tmNow.tm_mday > 26))
				|| (tmNow.tm_mon== 3 && (tmNow.tm_mday < 2 || (tmNow.tm_mday == 2 && tmNow.tm_hour < 8)))))
			{
				if(GetHostPlayer()->GetSrcServerID() == 0)
				{
					EnableObject(pMenu, "Btn_Rose", true);
				}	
			}
		}
	}
	else
		pMenu = GetGameUIMan()->m_pDlgFriendOptionNormal;

	x = GET_X_LPARAM(lParam) - p->X;
	y = GET_Y_LPARAM(lParam) - p->Y;
	
	// old : pMenu->SetPos(x, y);
	pMenu->SetPosEx(x, y);
	
	pMenu->Show(true);
}

void CDlgFriendList::OnEventRButtonUpEnemy(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( m_szCurPage != "enemy" )
		return;

	PAUIDIALOG pDlgMouseOn;
	PAUIOBJECT pObjMouseOn;
	GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
	if (pObjMouseOn->GetType() != AUIOBJECT_TYPE_LISTBOX)
	{
		return;
	}
	int iCur = m_pLstEnemyList->GetCurSel();
	if ( iCur<0 || iCur>=m_pLstEnemyList->GetCount())
	{
		return;
	}

	POINT ptPos = pObj->GetPos();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	PAUIDIALOG pMenu = GetGameUIMan()->m_pDlgEnemyOptionName;

	x = GET_X_LPARAM(lParam) - p->X;
	y = GET_Y_LPARAM(lParam) - p->Y;
	
	// old : pMenu->SetPos(x, y);
	pMenu->SetPosEx(x, y);

	ACString strText = m_pLstEnemyList->GetText(iCur);
	int iFirst = strText.Find(_AL("\t"));
	int iSecond = strText.ReverseFind(_AL('\t'));
	if (iSecond-iFirst>2)
	{
		EnableObject(pMenu, "Whisper", true);
	}
	else
		EnableObject(pMenu, "Whisper", false);
	
	pMenu->Show(true);
}

void CDlgFriendList::AddBlackList(const ACHAR* szName)
{
	if( szName[0] == 0 )
		return;

	EC_BLACKLIST_SETTING setting = GetGame()->GetConfigs()->GetBlackListSettings();
	// get player id if he is online and valid
	int idRole = GetGameRun()->GetPlayerID(szName);

	// compute current count in effect
	int nCount = 0;
	while( nCount < EC_BLACKLIST_LEN && setting.idPlayer[nCount] > 0) 
		nCount++;

	// blacklist length limit
	if( nCount + m_vecBlackCache.size() >= EC_BLACKLIST_LEN )
	{
		AUIDialog *pMsgBox = NULL;
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(262), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetLife(3);
		return;
	}

	// avoid duplication 
	for (int i = 0; i < nCount; i++ )
	{
		AUIDialog *pMsgBox = NULL;
		if( idRole == setting.idPlayer[i] )
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(260), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetLife(3);
			return;
		}
	}

	if( idRole > 0 )	// online and valid
	{
		if( m_szCurPage == "Btn_Blacklist" )
			m_pTvFriendList->InsertItem(szName, m_pTvFriendList->GetRootItem());
		EC_BLACKLIST_SETTING setting = GetGame()->GetConfigs()->GetBlackListSettings();
		setting.idPlayer[nCount] = idRole;
		GetGame()->GetConfigs()->SetBlackListSettings(setting);
	}
	else // offline or invalid
	{
		GetGameSession()->GetPlayerIDByName(szName, 0);
		BLACK_CACHE bc;
		bc.dwTime = GetTickCount();
		bc.strName = szName;
		m_vecBlackCache.push_back(bc);
	}
}

void CDlgFriendList::FriendAction(int idPlayer, int idGroup, int idAction, int nCode)
{
	PAUITREEVIEW pTree = m_pTvFriendList;
	if( pTree->GetCount() <= 0 ) return;

	ACString strHint;
	P_AUITREEVIEW_ITEM pItem = NULL;
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();

	if( idAction == FRIEND_ACTION_INFO_LEVEL ||
		idAction == FRIEND_ACTION_INFO_AREA  ||
		idAction == FRIEND_ACTION_INFO_PROFESSION  ||
		idAction == FRIEND_ACTION_INFO_CULTIVATION )
	{
		P_AUITREEVIEW_ITEM pItemPlayer = SearchItem(idPlayer, true);
		if( !pItemPlayer ) return;

		if( idAction == FRIEND_ACTION_INFO_LEVEL )
			pMan->SetFriendLevel(idPlayer, nCode);
		else if( idAction == FRIEND_ACTION_INFO_AREA )
			pMan->SetFriendArea(idPlayer, nCode);
		else if( idAction == FRIEND_ACTION_INFO_PROFESSION )
			pMan->SetFriendProfession(idPlayer, nCode);
		else if( idAction == FRIEND_ACTION_INFO_CULTIVATION )
			pMan->SetFriendCultivation(idPlayer, nCode);

		ACString strLevel, strArea, strLine, strProf;
		CECFriendMan::FRIEND *pFriend = pMan->GetFriendByID(idPlayer);

		if( pFriend )
		{
			if( pFriend->nLevel < 0 )
				strLevel = GetStringFromTable(574);
			else
				strLevel.Format(_AL("%d"), pFriend->nLevel);
			
			if( pFriend->iProfession < 0 )
				strProf = GetGameUIMan()->GetStringFromTable(574);
			else
				strProf = ACString(GetStringFromTable(7000+pFriend->iCultivation)) + ACString(GetGameUIMan()->GetStringFromTable(pFriend->iProfession+2101));
			
			if( pFriend->idArea < 0 )
				strArea = GetStringFromTable(574);
			else
			{
				int k;
				CELPrecinctSet *pSet = GetWorld()->GetPrecinctSet();

				for( k = 0; k < pSet->GetPrecinctNum(); k++ )
				{
					if( (int)pSet->GetPrecinct(k)->GetID() != pFriend->idArea )
						continue;

					strArea = pSet->GetPrecinct(k)->GetName();
					break;
				}
			}

			strLine = GetGameUIMan()->GetLine(pFriend->iLine);
			strHint.Format(GetStringFromTable(573), strLine, strLevel, strArea, strProf);
			pTree->SetItemHint(pItemPlayer, strHint);
		}
	}
	else if( idAction == FRIEND_ACTION_INFO_REFRESH )
	{
		ACHAR szInfo[20];

		a_sprintf(szInfo, _AL("L%d"), GetHostPlayer()->GetBasicProps().iLevel);
		GetGameSession()->SendPrivateChatData(_AL(""), szInfo, GNET::CHANNEL_USERINFO, idPlayer);
		
		a_sprintf(szInfo, _AL("P%d"), GetHostPlayer()->GetProfession());
		GetGameSession()->SendPrivateChatData(_AL(""), szInfo, GNET::CHANNEL_USERINFO, idPlayer);
		
		a_sprintf(szInfo, _AL("C%d"), GetHostPlayer()->GetCultivation());
		GetGameSession()->SendPrivateChatData(_AL(""), szInfo, GNET::CHANNEL_USERINFO, idPlayer);

		if( GetGameUIMan()->m_pCurPrecinct )
		{
			a_sprintf(szInfo, _AL("A%d"), GetGameUIMan()->m_pCurPrecinct->GetID());
			GetGameSession()->SendPrivateChatData(_AL(""), szInfo, GNET::CHANNEL_USERINFO, idPlayer);
		}
	}
	else if( idAction == FRIEND_ACTION_FRIEND_ADD )
	{
		ACString strMsg;
		const ACHAR *pszName = GetGameRun()->GetPlayerName(idPlayer, true);
		P_AUITREEVIEW_ITEM pItemGroup = SearchItem(idGroup, false);

		if( m_szCurPage == "friend" )
		{
			CECFriendMan::FRIEND *pFriend = pMan->GetFriendByID(idPlayer);
			pItem = InsertFriend(pFriend, m_pTvFriendList, pItemGroup);
		}

		strMsg.Format(GetStringFromTable(555), pszName);
		GetGameUIMan()->AddInformation(CDlgInfo::INFO_SYSTEM,
			"Game_OK", strMsg, 0xFFFFFFF, idPlayer, 0, 0);
	}
	else if( idAction == FRIEND_ACTION_FRIEND_DELETE )
	{
		P_AUITREEVIEW_ITEM pItemPlayer = SearchItem(idPlayer, true);

		if( pItemPlayer ) pTree->DeleteItem(pItemPlayer);
        
		DeleteFriendRemark(idPlayer);
	}
	else if( idAction == FRIEND_ACTION_FRIEND_UPDATE )
	{
		ACString strLine = GetGameUIMan()->GetLine(nCode);
		ACString strMsg;
		if( idPlayer == GetHostPlayer()->GetSpouse() )
		{
			if( nCode < 0 )
				strMsg.Format(GetStringFromTable(816), GetGameRun()->GetPlayerName(idPlayer, true) );
			else
				strMsg.Format(GetStringFromTable(815), GetGameRun()->GetPlayerName(idPlayer, true), strLine);
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
		}

		P_AUITREEVIEW_ITEM pItemPlayer = SearchItem(idPlayer, true);
		if( !pItemPlayer ) return;

		CECFriendMan::FRIEND *pFriend = pMan->GetFriendByID(idPlayer);

		if( idGroup >= 0 )		// Move friend to another group.
		{
			P_AUITREEVIEW_ITEM pItemGroup = SearchItem(idGroup, false);
			pItem = InsertFriend(pFriend, m_pTvFriendList, pItemGroup);
			pTree->DeleteItem(pItemPlayer);
		}
		else					// Online or offline notify.
		{
			P_AUITREEVIEW_ITEM pItemGroup = pTree->GetParentItem(pItemPlayer);
			pItem = InsertFriend(pFriend, m_pTvFriendList, pItemGroup);
			pTree->DeleteItem(pItemPlayer);

			if( idPlayer != GetHostPlayer()->GetSpouse() && GetGameUIMan()->m_bOnlineNotify )
			{
				ACString strName = pFriend->GetName();
				if( nCode < 0 )
					strMsg.Format(GetStringFromTable(550), strName);
				else
					strMsg.Format(GetStringFromTable(549), strName, strLine);
				GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
			}
		}
	}
	else if( idAction == FRIEND_ACTION_GROUP_ADD )
	{
		CECFriendMan::GROUP *pGroup = pMan->GetGroupByID(idGroup);

		if( pGroup )
		{
			pItem = pTree->InsertItem(pGroup->strName);
			pTree->SetItemData(pItem, idGroup);
			pTree->SetItemTextColor(pItem, pGroup->color);
		}
	}
	else if( idAction == FRIEND_ACTION_GROUP_DELETE )
	{
		BuildFriendList();
	}
	else if( idAction == FRIEND_ACTION_GROUP_RENAME )
	{
		P_AUITREEVIEW_ITEM pItemGroup = SearchItem(idGroup, false);
		CECFriendMan::GROUP *pGroup = pMan->GetGroupByID(idGroup);

		if( pItemGroup && pGroup ) pTree->SetItemText(pItemGroup, pGroup->strName);
	}
	else if( idAction == FRIEND_ACTION_INFO_GT )
	{
		ACString strMsg;
		if( (idPlayer == GetHostPlayer()->GetSpouse()) && idPlayer>0 )	// 不知道为啥服务器会发过来idPlayer=0的情况，2013.7.29
		{
			strMsg.Format(GetStringFromTable(nCode==0?1016:1015), GetGameRun()->GetPlayerName(idPlayer, true) );
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
		}
		
		P_AUITREEVIEW_ITEM pItemPlayer = SearchItem(idPlayer, true);
		if( !pItemPlayer ) return;
		
		CECFriendMan::FRIEND *pFriend = pMan->GetFriendByID(idPlayer);
		P_AUITREEVIEW_ITEM pItemGroup = pTree->GetParentItem(pItemPlayer);
		pItem = InsertFriend(pFriend, m_pTvFriendList, pItemGroup);
		pTree->DeleteItem(pItemPlayer);

		if( idPlayer != GetHostPlayer()->GetSpouse() && GetGameUIMan()->m_bGTOnLineNotify )
		{
			strMsg.Format(GetStringFromTable(nCode == 0?1020:1019), pFriend->GetName());
			GetGameUIMan()->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
		}
	}
}

P_AUITREEVIEW_ITEM CDlgFriendList::InsertFriend(CECFriendMan::FRIEND* fd, PAUITREEVIEW pTree, P_AUITREEVIEW_ITEM pRoot, bool bSel)
{
	// find suit place
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
	P_AUITREEVIEW_ITEM pItemAfter = AUITREEVIEW_LAST;
	if (fd->iStatGT>0 && fd->iLine>=0)
	{
		pItemAfter = AUITREEVIEW_FIRST;
	}
	else if (fd->iStatGT==0 && fd->iLine>=0)
	{
		P_AUITREEVIEW_ITEM pItemTemp = pTree->GetFirstChildItem(pRoot);
		CECFriendMan::FRIEND *pFriendTest;
		while (pItemTemp)
		{
			pFriendTest = pMan->GetFriendByID(pTree->GetItemData(pItemTemp));
			if (pFriendTest->iLine<0)
			{
				pItemAfter = pTree->GetPrevSiblingItem(pItemTemp);
				break;
			}
			pItemTemp = pTree->GetNextSiblingItem(pItemTemp);
		}
	}
	else if (fd->iStatGT>0 && fd->iLine<0)
	{
		P_AUITREEVIEW_ITEM pItemTemp = pTree->GetFirstChildItem(pRoot);
		CECFriendMan::FRIEND *pFriendTest;
		while (pItemTemp)
		{
			pFriendTest = pMan->GetFriendByID(pTree->GetItemData(pItemTemp));
			if (pFriendTest->iStatGT==0 && pFriendTest->iLine<0)
			{
				pItemAfter = pTree->GetPrevSiblingItem(pItemTemp);
				break;
			}
			pItemTemp = pTree->GetNextSiblingItem(pItemTemp);
		}
	}
	if (pItemAfter==NULL)
	{
		pItemAfter = AUITREEVIEW_FIRST;
	}
	ACString strText;
	EditBoxItemBase item(enumEIImage);
	item.SetImageFrame(0);
	if (fd->iStatGT>0)
	{
		item.SetImageIndex(CECGameUIMan::ICONLIST_GTICON);
	}
	else
		item.SetImageIndex(CECGameUIMan::ICONLIST_GTOFFLINE);
	if (fd->iStatGT>0)
	{
		strText = (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
		strText += _AL(" ");
	}
	ACString strRemark;
	if(GetFriendRemark(fd->id,strRemark))
	{
	   strText += strRemark;
	}
	else
	{
	   strText += fd->GetName();
	}
	P_AUITREEVIEW_ITEM pItem = pTree->InsertItem(strText, pRoot, pItemAfter);
	if (bSel)
	{
		pTree->SelectItem(pItem);
	}

	// set info
	pTree->SetItemData(pItem, fd->id);
	pTree->SetItemDataPtr(pItem,(void*)(&(fd->strName)));
	ACString strLine, strLevel, strArea, strProf;
	CECWorld *pWorld = g_pGame->GetGameRun()->GetWorld();
	CELPrecinctSet *pSet = pWorld->GetPrecinctSet();
	if( fd->iLine >= 0 )
		pTree->SetItemTextColor(pItem, A3DCOLORRGB(255, 203, 74));
	else
		pTree->SetItemTextColor(pItem, A3DCOLORRGB(128, 128, 128));
	
	if( fd->nLevel < 0 )
		strLevel = GetGameUIMan()->GetStringFromTable(574);
	else
		strLevel.Format(_AL("%d"), fd->nLevel);
	
	if( fd->iProfession < 0 )
		strProf = GetGameUIMan()->GetStringFromTable(574);
	else
		strProf = ACString(GetStringFromTable(7000+fd->iCultivation)) + ACString(GetGameUIMan()->GetStringFromTable(fd->iProfession+2101));
	
	if(fd->iLine == 500)       //好友跨服在线
		strArea = GetGameUIMan()->GetStringFromTable(17501);
	else if( fd->idArea < 0 || fd->iLine < 0)
		strArea = GetGameUIMan()->GetStringFromTable(574);
	else
	{
		for( int k = 0; k < pSet->GetPrecinctNum(); k++ )
		{
			if( (int)pSet->GetPrecinct(k)->GetID() != fd->idArea )
				continue;
			
			strArea = pSet->GetPrecinct(k)->GetName();
			break;
		}
	}
	
	strLine = GetGameUIMan()->GetLine(fd->iLine);
	strText.Format(GetGameUIMan()->GetStringFromTable(573), strLine, strLevel, strArea, strProf);
	pTree->SetItemHint(pItem, strText);

	return pItem;
}

void CDlgFriendList::BuildFriendList(PAUIDIALOG pDlg, int curSel)
{
	int i, j;
	CECFriendMan::GROUP *pGroup;
	P_AUITREEVIEW_ITEM pRoot, pItem;
	ACString strText;
	if( !pDlg ) pDlg = this;
	CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
	PAUITREEVIEW pTree = (PAUITREEVIEW)pDlg->GetDlgItem("Tv_FriendList");

	pTree->DeleteAllItems();
	for( i = 0; i < pMan->GetGroupNum(); i++ )
	{
		pGroup = pMan->GetGroupByIndex(i);
		if( pGroup->strName.GetLength() > 0 )
			pRoot = pTree->InsertItem(pGroup->strName);
		else
			pRoot = pTree->InsertItem(GetStringFromTable(537));
		pTree->SetItemData(pRoot, pGroup->iGroup);
		if( pGroup->iGroup > 0 )
			pTree->SetItemTextColor(pRoot, pGroup->color);

		if( 0 == stricmp(pDlg->GetName(), "Win_FriendList") )
		{
			for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
			{
				CECFriendMan::FRIEND* fd = pGroup->aFriends[j];
				InsertFriend(fd, pTree, pRoot, curSel==fd->id);
			}
		}
		else //if( 0 == stricmp(pDlg->GetName(), "Win_FriendHistory") )
		{
			for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
			{
				strText = pGroup->aFriends[j]->GetName();
				ACString strRemark;
				if(GetFriendRemark(pGroup->aFriends[j]->id,strRemark))
				{
					strText = strRemark;
				}
				pItem = pTree->InsertItem(strText, pRoot);
				pTree->SetItemData(pItem, pGroup->aFriends[j]->id);
				if (curSel == pGroup->aFriends[j]->id)
					pTree->SelectItem(pItem);

			}
		}

		pTree->Expand(pRoot, AUITREEVIEW_EXPAND_EXPAND);
	}
}

P_AUITREEVIEW_ITEM CDlgFriendList::SearchItem(int id, bool bSearchPlayer)
{
	PAUITREEVIEW pTree = m_pTvFriendList;
	P_AUITREEVIEW_ITEM pRoot = pTree->GetRootItem(), pItemGroup, pItemFriend;

	pItemGroup = pTree->GetFirstChildItem(pRoot);
	while( pItemGroup )
	{
		if( bSearchPlayer )
		{
			pItemFriend = pTree->GetFirstChildItem(pItemGroup);
			while( pItemFriend )
			{
				if( id == (int)pTree->GetItemData(pItemFriend) )
					return pItemFriend;
				pItemFriend = pTree->GetNextSiblingItem(pItemFriend);
			}
		}
		else
		{
			if( id == (int)pTree->GetItemData(pItemGroup) )
				return pItemGroup;
		}
		pItemGroup = pTree->GetNextSiblingItem(pItemGroup);
	}

	return NULL;
}

void CDlgFriendList::EnemyListAction(int idAction, void *pData)
{
	if( idAction == ENEMYLIST_ACTION_UPDATELIST )
	{
		GetEnemies_Re *p = (GetEnemies_Re*)pData;
		if( p->fulllist == 1 )
		{
			m_mapEnemyList.clear();
			for(DWORD i = 0; i < p->enemies.size(); i++)
			{
				ENEMY_INFO enemy;
				enemy.id = p->enemies[i].rid;
				enemy.pktime = p->enemies[i].pktime;
				enemy.name = ACString((const ACHAR*)p->enemies[i].name.begin(), p->enemies[i].name.size() / sizeof(ACHAR));
				enemy.status = p->enemies[i].status;
				enemy.lineid = p->enemies[i].lineid;
				m_mapEnemyList[p->enemies[i].rid] = enemy;
			}
		}
		else if( p->fulllist == 0 )
		{
			abase::hash_map<int, ENEMY_INFO>::iterator it = m_mapEnemyList.begin();
			for(; it != m_mapEnemyList.end(); ++it )
				it->second.lineid = -1;
			for(DWORD i = 0; i < p->online.size(); i++)
			{
				if( m_mapEnemyList.find(p->online[i].rid) != m_mapEnemyList.end() )
					m_mapEnemyList[p->online[i].rid].lineid = p->online[i].lineid;
			}
		}
		else if( p->fulllist == 2 )
		{
			for(DWORD i = 0; i < p->enemies.size(); i++)
			{
				ENEMY_INFO enemy;
				enemy.id = p->enemies[i].rid;
				enemy.pktime = p->enemies[i].pktime;
				enemy.name = ACString((const ACHAR*)p->enemies[i].name.begin(), p->enemies[i].name.size() / sizeof(ACHAR));
				enemy.status = p->enemies[i].status;
				enemy.lineid = p->enemies[i].lineid;
				m_mapEnemyList[p->enemies[i].rid] = enemy;
			}
		}
		else if( p->fulllist == 3 )
		{
			m_mapEnemyList.clear();
			abase::hash_map<int, ENEMY_INFO>::iterator it = m_mapEnemyList.begin();
			for(DWORD i = 0; i < p->online.size(); i++)
			{
				ENEMY_INFO enemy;
				enemy.id = p->online[i].rid;
				enemy.pktime = 0;
				enemy.name = _AL("");
				enemy.status = 0;
				enemy.lineid = p->online[i].lineid;
				m_mapEnemyList[p->online[i].rid] = enemy;
			}
		}
	}
	else if( idAction == ENEMYLIST_ACTION_UPDATE )
	{
		UpdateEnemy_Re *p = (UpdateEnemy_Re*)pData;
		if( p->retcode == ERROR_SUCCESS )
		{
			if( p->oper == 0 )
			{
				m_mapEnemyList.erase(p->enemy);
			}
			else if( p->oper == 1 )
			{
				abase::hash_map<int, ENEMY_INFO>::iterator it = m_mapEnemyList.find(p->enemy);
				if( it != m_mapEnemyList.end() )
					it->second.status = 1 - it->second.status;
			}
		}
	}
	if( m_szCurPage == "enemy" )
		OnCommandSetPage("enemy");
}

bool CDlgFriendList::IsEnemy(int id)
{
	if( m_mapEnemyList.find(id) != m_mapEnemyList.end() )
		return true;
	else
		return false;
}

void CDlgFriendList::EnableObject(PAUIDIALOG pDlg, const char *pszCommand, bool bEnable)
{
	PAUIOBJECT pObj = pDlg->GetDlgItem(pszCommand);
	if (pObj)
		pObj->Enable(bEnable);
	else
		assert(0);
}


bool  CDlgFriendList::GetFriendRemark(int iPlayerId,ACString& strName)
{
	if(iPlayerId <=0)
	   return false;
    const EC_FRIENDSREMARKS& frs = g_pGame->GetConfigs()->GetFriendsRemarks();
	for (int i=0;i<EC_FRIENDS_NUM;++i)
	{
		if(frs.friendsaliases[i].iPlayerId <= 0)
			return false;
		else if(frs.friendsaliases[i].iPlayerId == iPlayerId)
		{
		    strName = frs.friendsaliases[i].szName;
			return true;
		}
	}
	return false;
}
bool  CDlgFriendList::AddFriendRemark(int iPlayerId, ACString& strName)
{
	if(iPlayerId <=0)
      return false;
    EC_FRIENDSREMARKS frs = g_pGame->GetConfigs()->GetFriendsRemarks();
	for(int i=0;i<EC_FRIENDS_NUM;++i)
	{
		if(frs.friendsaliases[i].iPlayerId == iPlayerId ||frs.friendsaliases[i].iPlayerId <= 0)
		{
            frs.friendsaliases[i].iPlayerId = iPlayerId;
			memset(frs.friendsaliases[i].szName,0,sizeof(frs.friendsaliases[i].szName));
			a_strcpy(frs.friendsaliases[i].szName,strName.GetBuffer(0));
            g_pGame->GetConfigs()->SetFriendsRemarks(frs);
		    return true;
		}
	}
    return false;
}
bool  CDlgFriendList::DeleteFriendRemark(int iPlayerId)
{  
	if(iPlayerId <=0)
       return false;
    EC_FRIENDSREMARKS frs = g_pGame->GetConfigs()->GetFriendsRemarks();

	int nLen = 0;
	while( frs.friendsaliases[nLen].iPlayerId > 0 && nLen < EC_FRIENDS_NUM )
		nLen++;
	for(int i=0;i<nLen;++i)
	{
		if(frs.friendsaliases[i].iPlayerId <= 0)
		    return false;
		else if(frs.friendsaliases[i].iPlayerId == iPlayerId)
		{
			memmove(frs.friendsaliases + i,frs.friendsaliases + i + 1,sizeof(EC_FRIENDALIASES) * (nLen - i - 1));
			frs.friendsaliases[nLen-1].iPlayerId = 0;
			memset(frs.friendsaliases[nLen-1].szName,0,sizeof(frs.friendsaliases[nLen-1].szName));
			g_pGame->GetConfigs()->SetFriendsRemarks(frs);
			return true;
		}
	}
	return false;
}