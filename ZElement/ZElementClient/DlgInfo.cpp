// Filename	: DlgInfo.cpp
// Creator	: Tom Zhou
// Date		: October 19, 2005

#include "AUI\\AUICommon.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"
#include "DlgInfo.h"
#include "DlgMinimizeBar.h"
#include "DlgQuestion.h"
#include "DlgFriendChat.h"
#include "DlgFriendList.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Friend.h"
#include "EC_Circle.h"
#include "EC_Game.h"
#include "ECScriptContext.h"
#include "ECScriptController.h"
#include "ECScript.h"
#include "ECScriptUnitExecutor.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgInfo, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgInfo, CDlgBase)

AUI_ON_EVENT("F_*",		WM_LBUTTONUP,		OnEventLButtonUp_Icon)
AUI_ON_EVENT("List",	WM_LBUTTONUP,		OnEventLButtonUp_List)

AUI_END_EVENT_MAP()

abase::vector<CDlgInfo::INFORMATION> CDlgInfo::m_vecInfo[INFO_NUM];

CDlgInfo::CDlgInfo()
{
	m_pScriptContext = NULL;
	for( int i = 0; i < INFO_NUM; i++ )
		m_vecInfo[i].clear();
}

CDlgInfo::~CDlgInfo()
{
}

void CDlgInfo::OnCommand_CANCEL(const char * szCommand)
{
	m_pAUIManager->RespawnMessage();
}

void CDlgInfo::OnEventLButtonUp_Icon(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int nType = atoi(pObj->GetName() + strlen("F_"));
	CDlgInfo *pDlgList = GetGameUIMan()->m_pDlgInfo;

	if( pDlgList->IsShow() && (int)pDlgList->GetData() == nType )
	{
		pDlgList->Show(false);
		pDlgList->SetData(INFO_NULL);
	}
	else
	{
		pDlgList->BuildInfoList(nType);

		if( m_vecInfo[nType].size() > 1 )
		{
			pDlgList->Show(true);
			pDlgList->SetData(nType);
			pDlgList->AlignTo(this, AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
				AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);
		}
		else
		{
			PAUILISTBOX pList = (PAUILISTBOX)pDlgList->GetDlgItem("List");

			pList->SetCurSel(0);
			pDlgList->SetData(nType);
			pDlgList->OnEventLButtonUp_List(0, 0, pList);
			pDlgList->SetData(INFO_NULL);
		}
	}

	((PAUIIMAGEPICTURE)pObj)->FadeInOut(0);
}

void CDlgInfo::OnEventLButtonUp_List(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUILISTBOX pList = (PAUILISTBOX)pObj;
	int nCurSel = pList->GetCurSel();
	if( nCurSel < 0 || nCurSel >= pList->GetCount() ) return;

	char szName[40];
	AString strName;
	int nType = GetData();
	PAUIDIALOG pMsgBox = NULL, pDlgFound = NULL;
	INFORMATION Info, InfoThis = m_vecInfo[nType][nCurSel];

	if( 0 == stricmp(InfoThis.strType, "Game_FriendMsg") )
	{
		int i;

		for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
		{
			sprintf(szName, "Win_FriendChat%d", i);
			pMsgBox = GetGameUIMan()->GetDialog(szName);
			if( pMsgBox->GetData() == InfoThis.dwData1 )
			{
				pDlgFound = pMsgBox;
				break;
			}
		}

		if( !pDlgFound )
		{
			for( i = 0; i < CECGAMEUIMAN_MAX_CHATS; i++ )
			{
				sprintf(szName, "Win_FriendChat%d", i);
				pMsgBox = GetGameUIMan()->GetDialog(szName);
				if( pMsgBox->GetData() == 0 )
				{
					pDlgFound = pMsgBox;
					pMsgBox->GetDlgItem("Txt_Chat")->SetText(_AL(""));
					pMsgBox->GetDlgItem("Txt_Content")->SetText(_AL(""));
					break;
				}
			}
		}
		pMsgBox = pDlgFound;

		if( pDlgFound )
		{
			ACHAR szUser[40];
			ACString strText;
			PAUIOBJECT pName = pMsgBox->GetDlgItem("Txt_ToWho");
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pMsgBox->GetDlgItem("Txt_Content");
			PAUITEXTAREA pText = (PAUITEXTAREA)pMsgBox->GetDlgItem("Txt_Chat");
			const ACHAR *pszPlayer = GetGameRun()->GetPlayerName(InfoThis.dwData1, true);

			PAUIDIALOG pDlgChat;
			for( i = 0; i < pList->GetCount(); i++ )
			{
				Info = m_vecInfo[nType][i];
				if( Info.dwData1 != InfoThis.dwData1 ) continue;

				AUI_ConvertChatString(pszPlayer, szUser);
				{
					ACString strName;
					if(CDlgFriendList::GetFriendRemark(InfoThis.dwData1,strName))
					{
						AUI_ConvertChatString(strName, szUser);
					}
				}
				sprintf(szName, "Win_FriendChat%d", i);
				pDlgChat = GetGameUIMan()->GetDialog(szName);
				((CDlgFriendChat *)pDlgChat)->SetFriendName(szUser);
				strText.Format(GetStringFromTable(548), szUser);
				pName->SetText(strText);
				pEdit->SetIsAutoReturn(true);

				if( a_strlen(pText->GetText()) > 0 )
					pText->AppendText(_AL("\r"));
				pText->AppendText(Info.strMsg);

				pList->DeleteString(i);
				m_vecInfo[nType].erase(m_vecInfo[nType].begin() + i);
				i--;
			}

			pText->ScrollToTop();
			pText->ScrollToBottom();
			
			GetGameUIMan()->m_pDlgMinimizeBar[0]->RestoreDialog(pMsgBox);
			pMsgBox->Show(true);
			pMsgBox->ChangeFocus(pEdit);
			pMsgBox->SetText(pszPlayer);
			pMsgBox->SetData(Info.dwData1);
		}
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_EnterCircle") )
	{
		Circle_Info *cInfo = GetGame()->GetCircleMan()->GetCircle(GetHostPlayer()->GetCircleID(), true);
		if (cInfo)
		{
			ACString strText;
			strText.Format(GetGameUIMan()->GetStringFromTable(10234), cInfo->GetName());
			GetGameUIMan()->MessageBox("Game_OpenCircle", strText, MB_YESNO);
		}
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_HelpMsg") )
	{
		m_pScriptContext->GetController()->ForceActiveScript(InfoThis.dwData1);
		
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
		GetGameUIMan()->m_pDlgInfo->SetData(INFO_NULL);
		GetGameUIMan()->m_pDlgInfo->Show(false);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_InviteBind") )
	{
		GetGameUIMan()->MessageBox("Game_InviteBind", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_RequestBind") )
	{
		GetGameUIMan()->MessageBox("Game_RequestBind", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_InviteRideTogether") )
	{
		GetGameUIMan()->MessageBox("Game_InviteRideTogether", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_InviteDuel") )
	{
		GetGameUIMan()->MessageBox("Game_InviteDuel", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_AskToJoin") )
	{
		GetGameUIMan()->MessageBox("Game_AskToJoin", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(Info.dwData1);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_MsgBox") )
	{
		GetGameUIMan()->MessageBox("", pList->GetText(nCurSel), 
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_CombatInviteRe") )
	{
		GetGameUIMan()->MessageBox("Game_CombatInviteRe", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(Info.dwData1);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if (0 == stricmp(InfoThis.strType, "Game_ContestInvite") )
	{
		GetGameUIMan()->MessageBox("Game_ContestInvite", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_Question") )
	{
		GetGameUIMan()->m_pDlgQuestion->Show(true);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_TaskCheck") )
	{
		GetGameUIMan()->MessageBox("Game_TaskCheck", pList->GetText(nCurSel), 
			MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(InfoThis.dwData1);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_DailyPrompHint") )
	{
		GetGameUIMan()->GetDialog("Win_DailyPrompLoginHint")->Show(true);
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else if( 0 == stricmp(InfoThis.strType, "Game_Friend_ChangeName") )
	{
		GetGameUIMan()->MessageBox("", pList->GetText(nCurSel));
		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}
	else
	{
		if( 0 == stricmp(InfoThis.strType, "Game_InviteFriend") )
		{
			ACHAR szText[128];

			pMsgBox = GetGameUIMan()->GetDialog("Win_Message1");
			AUI_ConvertChatString(InfoThis.strMsg, szText, false);
			pMsgBox->GetDlgItem("Txt_Message")->SetText(szText);
			pMsgBox->GetDlgItem("Btn_Apply")->Enable(true);
			pMsgBox->GetDlgItem("Btn_Refuse")->Enable(true);
			if( GetHostPlayer()->GetFriendMan()->GetFriendByID(InfoThis.dwData1) )
			{
				pMsgBox->GetDlgItem("Btn_Add")->Enable(false);
				pMsgBox->GetDlgItem("Btn_BlackList")->Enable(false);
			}
			else
			{
				pMsgBox->GetDlgItem("Btn_Add")->Enable(true);
				pMsgBox->GetDlgItem("Btn_BlackList")->Enable(true);
			}
			pMsgBox->Show(true);
		}
		else if( 0 == stricmp(InfoThis.strType, "Game_OK") )
		{
			GetGameUIMan()->MessageBox(InfoThis.strType, InfoThis.strMsg,
				MB_OK, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else if( 0 == stricmp(InfoThis.strType, "Game_InviteChannel") )
		{
			int nPos = InfoThis.strMsg.ReverseFind(_AL(' '));
			int nPWLen = InfoThis.strMsg.Right(InfoThis.strMsg.GetLength() - nPos - 1).ToInt();
			ACString szPW = InfoThis.strMsg.Mid(nPos - nPWLen, nPWLen);
			GetGameUIMan()->MessageBox("Game_ChannelInvite", InfoThis.strMsg.Left(nPos - nPWLen),
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetText(szPW);
		}
		else if ( 0 == stricmp(InfoThis.strType, "Game_InviteActiveEmote") )
		{
			GetGameUIMan()->MessageBox(InfoThis.strType, InfoThis.strMsg,
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}
		else
		{
			GetGameUIMan()->MessageBox(InfoThis.strType, InfoThis.strMsg,
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		}

		pList->DeleteString(nCurSel);
		m_vecInfo[nType].erase(m_vecInfo[nType].begin() + nCurSel);
	}

	if( pMsgBox )
	{
		pMsgBox->SetIsModal(false);
		pMsgBox->SetData(InfoThis.dwData1);
		pMsgBox->SetDataPtr((void *)InfoThis.dwData2,"dword");
	}

	if( m_vecInfo[nType].size() <= 0 )
	{
		PAUIIMAGEPICTURE pImage;
		PAUIDIALOG pDlgPop = GetGameUIMan()->m_pDlgInfoIcon;

		sprintf(szName, "Back_%d", nType);
		pImage = (PAUIIMAGEPICTURE)pDlgPop->GetDlgItem(szName);
		pImage->Show(false);

		sprintf(szName, "F_%d", nType);
		pImage = (PAUIIMAGEPICTURE)pDlgPop->GetDlgItem(szName);
		pImage->Show(false);
		pImage->FadeInOut(0);

		Show(false);
	}
}

void ListStringConvert(const ACHAR *pszChat, ACHAR *pszConv, bool bName)
{
	int i, nLen = 0;
	int nLength = (int)a_strlen(pszChat);

	pszConv[0] = 0;
	for( i = 0; i < nLength; i++ )
	{
		if (pszChat[i] == '^' && (  i==0 || pszChat[i-1] != '^' ) )
		{
			i+=6;
		}
		else if( pszChat[i] == '&' && ( i==0 || pszChat[i-1] != '^' ) )
		{
			i ++;
		}
		else
		{
			pszConv[nLen] = pszChat[i];
			nLen++;
		}
	}
	pszConv[nLen] = 0;
}

void CDlgInfo::BuildInfoList(int nType)
{
	int i;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List");

	pList->ResetContent();
	for( i = 0; i < (int)m_vecInfo[nType].size(); i++ )
	{
		ACHAR szText[512];
		ListStringConvert(m_vecInfo[nType][i].strMsg, szText, false);
		pList->AddString(m_vecInfo[nType][i].strMsg);
		pList->SetItemHint(i, m_vecInfo[nType][i].strMsg);
	}
}

bool CDlgInfo::AddInfo(int nType, INFORMATION Info)
{
	if( nType == INFO_QUESTION && m_vecInfo[nType].size() >= 1 )
		m_vecInfo[nType].clear();
	if (Info.strType == "Game_TaskCheck")
	{
		// 周期发放任务只接受一次
		UINT nSize = m_vecInfo[nType].size();
		if (GetGameUIMan()->GetDialog("Game_TaskCheck") && GetGameUIMan()->GetDialog("Game_TaskCheck")->GetData() == Info.dwData1)
		{
			return false;
		}
		for (UINT i=0; i<nSize; i++)
		{
			if (m_vecInfo[nType][i].dwData1 == Info.dwData1)
			{
				return false;
			}
		}
	}
	m_vecInfo[nType].push_back(Info);
	return true;
}

bool CDlgInfo::NeedAddPopTask(int nType, int id)
{
	UINT nSize = m_vecInfo[nType].size();
	for(UINT i=0; i<nSize; i++)
	{
		if (m_vecInfo[nType][i].strType == "Game_TaskCheck" && id == (int)m_vecInfo[nType][i].dwData1)
		{
			return false;
		}
	}
	return true;
}

void CDlgInfo::DeletePopTask(int nType, int id)
{
	UINT nSize = m_vecInfo[nType].size();
	for(UINT i=0; i<nSize; i++)
	{
		if (m_vecInfo[nType][i].strType == "Game_TaskCheck" && id == (int)m_vecInfo[nType][i].dwData1)
		{
			m_vecInfo[nType].erase(&m_vecInfo[nType][i]);
			if( m_vecInfo[nType].size() <= 0 )
			{
				char szName[40];
				sprintf(szName, "Back_%d", nType);
				PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetGameUIMan()->m_pDlgInfoIcon->GetDlgItem(szName);
				pImage->Show(false);
				
				sprintf(szName, "F_%d", nType);
				pImage = (PAUIIMAGEPICTURE)GetGameUIMan()->m_pDlgInfoIcon->GetDlgItem(szName);
				pImage->Show(false);
				pImage->FadeInOut(0);
				int nCurType = (int)GetGameUIMan()->m_pDlgInfo->GetData();
				if( nType == nCurType )
					GetGameUIMan()->m_pDlgInfo->Show(false);
			}
			else
				BuildInfoList(nType);
			return;
		}
	}
}

void CDlgInfo::SetScriptContext(CECScriptContext * pContext)
{
	m_pScriptContext = pContext;
}

bool CDlgInfo::OnInitDialog()
{
	if (m_szName=="Win_Pop")
	{
		char szName[40];
		for( int i = 1; i <= 4; i++ )
		{
			sprintf(szName, "F_%d", i);
			GetDlgItem(szName)->Show(false);
			sprintf(szName, "Back_%d", i);
			GetDlgItem(szName)->Show(false);
		}
	}
	return true;
}

void CDlgInfo::OnTick()
{
	if( m_szName != "Win_Pop" ) return;

	char szName[40];
	PAUIIMAGEPICTURE pImage;
	int i, j;
	DWORD dwTimeNow = GetTickCount();
	int nCurType = (int)GetGameUIMan()->m_pDlgInfo->GetData();

	for( i = 0; i < INFO_NUM; i++ )
	{
		for( j = 0; j < (int)m_vecInfo[i].size(); j++ )
		{
			if( dwTimeNow - m_vecInfo[i][j].dwLifeBegin < m_vecInfo[i][j].dwLife )
				continue;

			if( i == INFO_QUESTION )
				GetGameUIMan()->m_pDlgQuestion->Show(true, false, false);
			m_vecInfo[i].erase(m_vecInfo[i].begin() + j);
			j--;

			if( nCurType == i )
				GetGameUIMan()->m_pDlgInfo->BuildInfoList(i);

			if( m_vecInfo[i].size() <= 0 )
			{
				sprintf(szName, "Back_%d", i);
				pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
				pImage->Show(false);

				sprintf(szName, "F_%d", i);
				pImage = (PAUIIMAGEPICTURE)GetDlgItem(szName);
				pImage->Show(false);
				pImage->FadeInOut(0);
			}

			if( m_vecInfo[i].size() <= 0 && nCurType == i )
				GetGameUIMan()->m_pDlgInfo->Show(false);
		}
	}
}