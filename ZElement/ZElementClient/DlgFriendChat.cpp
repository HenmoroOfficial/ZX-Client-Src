// File		: DlgFriendChat.cpp
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#include "AFI.h"
#include "ainifile.h"
#include "DlgFriendChat.h"
#include "DlgFriendList.h"
#include "DlgFriendHistory.h"
#include "DlgChat.h"
#include "DlgMinimizeBar.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
#include "time.h"
#include "Network\\gnetdef.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICommon.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgFriendChat, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)
AUI_ON_COMMAND("send",		OnCommandSend)
AUI_ON_COMMAND("popface",	OnCommandPopface)
AUI_ON_COMMAND("history",	OnCommandHistory)
AUI_ON_COMMAND("save",		OnCommandSave)
AUI_ON_COMMAND("delall",	OnCommandDelall)
AUI_ON_COMMAND("minimize",	OnCommandMinimize)
AUI_ON_COMMAND("Btn_Hotkey",OnCommandHotKey)

AUI_END_COMMAND_MAP()


AUI_BEGIN_EVENT_MAP(CDlgFriendChat, CDlgBase)

AUI_ON_EVENT("Txt_Content",	WM_SYSCHAR,		OnEventSysChar)
AUI_ON_EVENT("Txt_Content",	WM_KEYDOWN,		OnEventKeyDown)
AUI_ON_EVENT("Txt_Content",	WM_CHAR,		OnEventChar)

AUI_END_EVENT_MAP()

int		CDlgFriendChat::m_iHotkey = CDlgFriendChat::FCHK_CTRLENTER;

CDlgFriendChat::CDlgFriendChat()
{
	m_pTxtContent = NULL;
	m_pTxtChat = NULL;
	m_pChkSave = NULL;
	m_pBtnSend = NULL;
}

CDlgFriendChat::~CDlgFriendChat()
{
}

void CDlgFriendChat::DoDataExchange(bool bSave)
{
	CDlgBase::DoDataExchange(bSave);

	DDX_Control("Txt_Content", m_pTxtContent);
	DDX_Control("Txt_Chat", m_pTxtChat);
	DDX_Control("Chk_Save", m_pChkSave);
	DDX_Control("Btn_Send", m_pBtnSend);
}

bool CDlgFriendChat::OnInitDialog()
{
	PAUITEXTAREA pText = (PAUITEXTAREA)GetDlgItem("Txt_Chat");
	GetGameUIMan()->AttachEmotion(pText);
	PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Txt_Content");
	GetGameUIMan()->AttachEmotion(pEdit);
	if (GetGameUIMan()->GetControlImgLst()->size()>0)
		pText->SetImageList(GetGameUIMan()->GetControlImgLst());
	return true;
}

void CDlgFriendChat::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
	SetData(0);
}

void CDlgFriendChat::OnCommandMinimize(const char *szCommand)
{
	ACString szSubject = GetDlgItem("Txt_ToWho")->GetText();
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		szSubject.Mid(9, szSubject.GetLength() - 20), CDlgMinimizeBar::MINIMIZEBAR_TYPE_FRIEND);
}

void CDlgFriendChat::OnCommandHotKey(const char *szCommand)
{
	PAUIDIALOG pDlgHotKey = GetGameUIMan()->GetDialog("Win_FriendChatHotkey");
	PAUIOBJECT pObjBase = GetDlgItem("Btn_Hotkey");
	pDlgHotKey->SetPosEx(pObjBase->GetPos().x, pObjBase->GetPos().y+pObjBase->GetSize().cy);
	pDlgHotKey->Show(!pDlgHotKey->IsShow());
	pDlgHotKey->CheckRadioButton(1, m_iHotkey+1);
}

void CDlgFriendChat::OnCommandSend(const char *szCommand)
{
	CECGameSession *pSession = GetGameSession();
	int idPlayer = GetData();
	ACString strText = m_pTxtContent->GetText();

	if( strText.GetLength() > 0 )
	{
		tm tmTime = GetGame()->GetServerLocalTime();
		time_t t = mktime(&tmTime);
		ACString strTime(' ', 3);
		strTime[0] = 0xdbfe;
		strTime[1] = short(t>>16);
		t&=0xffff;
		strTime[2] = short(t!=0 ? t : 1);
		ACString strTimelocal;
		strTimelocal.Format(_AL("  %d:%02d"), tmTime.tm_hour, tmTime.tm_min);


		ACHAR szText[512], szName[40];
		ACString strTarget = GetText();
		CECHostPlayer *pHost = GetHostPlayer();

		GetGameUIMan()->FilterBadWords(strText);
		strText = FilterEmotionSet(strText, GetHostPlayer()->GetCurEmotionSet());
		AUI_ConvertChatString(strText, szText, false);
		AUI_ConvertChatString(pHost->GetName(), szName);
		if( GetGameUIMan()->m_bSaveHistory )
			GetGameUIMan()->AddHistory(idPlayer, time(NULL), pHost->GetName(), szText);

		if( a_strlen(m_pTxtChat->GetText()) > 0 )
			m_pTxtChat->AppendText(_AL("\r"));
		ACString strMsg = szText;
		ACHAR ch = 0xdbff;
		ACString strMark = ACString(ch,2);
		int iQk1 = strMsg.Find(_AL(":"));
		int iQk2 = strMsg.Find(GetGameUIMan()->GetStringFromTable(8493));
		int iQk = 0;
		if (iQk1<0)
			if (iQk2<0)
				iQk = -1;
			else
				iQk = iQk2;
		else
			if (iQk2<0)
				iQk = iQk1;
			else
				iQk = iQk1>iQk2 ? iQk2 : iQk1;
		UINT i=0;
		const abase::vector<CDlgChat::QUICK_TALK>* vecQuickTalk = CDlgChat::GetQuickTalkList();
		if (iQk>=0)
		{
			ACString strQuick = strText.Mid(iQk+1);
			for (i=0; i<vecQuickTalk->size(); i++)
			{
				CDlgChat::QUICK_TALK qkItem = (*vecQuickTalk)[i];
				if (strQuick == qkItem.strName)
				{
					ACString str;
					ACString strTemp;
					strMsg = qkItem.strTarget;
					strMsg = strMsg.Mid(iQk);
					int iLeft = strMsg.Find(_AL("#N"));
					
					while(iLeft>=0)
					{
						strMsg[iLeft] = _AL('%');
						strMsg[iLeft+1] = _AL('s');
						strTemp = strMsg;
						strMsg.Format(strTemp, szName);
						iLeft = strMsg.Find(_AL("#N"));
					}

					iLeft = strMsg.Find(_AL("#n"));
					while(iLeft>=0)
					{
						strMsg[iLeft] = _AL('%');
						strMsg[iLeft+1] = _AL('s');
						strTemp = strMsg;
						strMsg.Format(strTemp, m_strToWho);
						iLeft = strMsg.Find(_AL("#n"));
					}

					str = ACString(_AL("^80ffff")) + GetStringFromTable(8494) + strTimelocal + _AL("\r^ffff80") + strMsg;
					m_pTxtChat->AppendText(str);
					strText = _AL(":")+strMark+qkItem.strTarget;
					break;
				}
			}
		} 
		if (iQk<0 || i==vecQuickTalk->size())
		{
			m_pTxtChat->AppendText(_AL("^80ffff"));
			m_pTxtChat->AppendText(szName);
			m_pTxtChat->AppendText(strTimelocal);
			m_pTxtChat->AppendText(_AL("\r^ffff80"));
			m_pTxtChat->AppendText(szText);
		}
//		CECFriendMan *pMan = GetHostPlayer()->GetFriendMan();
//		if (pMan->GetChatRouteType(idPlayer)==TO_GAME)
//		{
			strText += strTime;
//		}
	
		m_pTxtChat->ScrollToTop();
		m_pTxtChat->ScrollToBottom();

		GetGameUIMan()->FilterBadWords(strText);
		pSession->SendPrivateChatData(strTarget,
			strText, GNET::CHANNEL_FRIEND, idPlayer, -1, 0, true);
		ChangeFocus(m_pTxtContent);
		m_pTxtContent->SetText(_AL(""));
	}
}

void CDlgFriendChat::OnCommandPopface(const char *szCommand)
{
	CDlgChat* pShow = (CDlgChat*)GetGameUIMan()->GetDialog("Win_Popface");
	
	if (pShow->GetPopType())
	{
		pShow = (CDlgChat*)GetGameUIMan()->GetDialog("Win_PopTalk");
	}
	
	pShow->Show(!pShow->IsShow());
	pShow->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
		AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
	pShow->SetDataPtr(this,"ptr_AUIDialog");
}

void CDlgFriendChat::OnCommandHistory(const char *szCommand)
{
	int idPlayer = GetData();

	GetGameUIMan()->m_pDlgFriendList->BuildFriendList(GetGameUIMan()->m_pDlgFriendHistory, idPlayer);
	GetGameUIMan()->m_pDlgFriendHistory->BuildChatHistory(idPlayer);
	GetGameUIMan()->m_pDlgFriendHistory->SetData(idPlayer);
	GetGameUIMan()->m_pDlgFriendHistory->Show(true);
}

void CDlgFriendChat::OnCommandSave(const char *szCommand)
{
	int i;
	char szName[40];
	PAUICHECKBOX pCheck = m_pChkSave;

	GetGameUIMan()->m_bSaveHistory = m_pChkSave->IsChecked();
	for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
	{
		sprintf(szName, "Win_FriendChat%d", i);
		pCheck = (PAUICHECKBOX)GetGameUIMan()->GetDialog(szName)->GetDlgItem("Chk_Save");
		pCheck->Check(GetGameUIMan()->m_bSaveHistory);
	}
}

void CDlgFriendChat::OnCommandDelall(const char *szCommand)
{
	GetGameUIMan()->MessageBox("Game_DelHistory", GetStringFromTable(566),
		MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgFriendChat::OnEventSysChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( (TCHAR(wParam) == 's' || TCHAR(wParam) == 'S') )
		OnCommandSend("send");
}

void CDlgFriendChat::OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( int(wParam) == VK_RETURN && 
		( (AUI_PRESS(VK_CONTROL)&&m_iHotkey==FCHK_CTRLENTER) ||
		(AUI_PRESS(VK_SHIFT)&&m_iHotkey==FCHK_SHIFTENTER) ) ) 
		OnCommandSend("send");
}

void CDlgFriendChat::OnEventChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	// shift + enter 会输入框内，将其去掉
	if (AUI_PRESS(VK_SHIFT) && m_iHotkey==FCHK_SHIFTENTER && wParam==ACHAR('\r'))
	{
		ACString strText = m_pTxtContent->GetText();
		strText.CutRight(1);
		m_pTxtContent->SetText(strText);
	}
}

void CDlgFriendChat::OnShowDialog()
{
	CDlgBase::OnShowDialog();
	
	GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(this);
	ChangeFocus(m_pTxtContent);
}

void CDlgFriendChat::OnTick()
{
	int battleType = GetHostPlayer()->GetBattleType();
	if ( battleType!=BATTLE_TYPE_ARENA && battleType!=BATTLE_TYPE_ARENAREBORN && battleType!=BATTLE_TYPE_CRSSVR )
	{
		m_pTxtContent->Enable(true);
		m_pBtnSend->Enable(true);
	}
	else
	{
		m_pTxtContent->Enable(false);
		m_pBtnSend->Enable(false);
	}
}

bool CDlgFriendChat::Release()
{
	if( stricmp(m_szName, "Win_FriendChat0") == 0 )
	{
		AIniFile IniFile;
		char szIniFile[MAX_PATH];
		sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
		IniFile.Open(szIniFile);
		IniFile.WriteIntValue("friendchat_hotkey", "type", m_iHotkey);

		IniFile.Save(szIniFile);
	}
	return CDlgBase::Release();
}

void CDlgFriendChat::SetFriendName(const ACHAR * strName)
{
	m_strToWho = strName;
}

int CDlgFriendChat::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="loadInfo")
	{	
		AIniFile IniFile;
		char szIniFile[MAX_PATH];
		sprintf(szIniFile, "%s\\UserData\\Layout\\UIData%d.ini", af_GetBaseDir(), GetHostPlayer()->GetCharacterID());
		if (IniFile.Open(szIniFile))
		{
			m_iHotkey = IniFile.GetValueAsInt("friendchat_hotkey", "type", 0);
		}
	}
	
	return true;
}


AUI_BEGIN_COMMAND_MAP(CDlgFriendChatHotKey, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",	OnCommandCANCAL)

AUI_END_COMMAND_MAP()

void CDlgFriendChatHotKey::OnCommandCANCAL(const char *szCommand)
{
	Show(false);
	CDlgFriendChat::SetHotKey(GetCheckedRadioButton(1)-1);
}
