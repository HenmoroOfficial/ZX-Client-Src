// Filename	: DlgGamePlaza.cpp
// Creator	: Chen Zhixin
// Date		: 2009/03/30

#include "DlgGamePlaza.h"
#include "DlgGamePoker.h"
#include "DlgMinimizeBar.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_FunGame.h"
#include "EC_GameSession.h"
#include "EC_FGPDataType.h"

#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\CSplit.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgGamePlaza, CDlgBase)

AUI_ON_COMMAND("Btn_Confirm",	OnCommand_Confirm)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Cancel",	OnCommand_Cancel)
AUI_ON_COMMAND("Btn_Guest*",	OnCommand_Guest)
AUI_ON_COMMAND("Btn_Chair*",	OnCommand_Join)
AUI_ON_COMMAND("Btn_Minimize",	OnCommand_Minimize)
AUI_ON_COMMAND("Btn_ReturnMatch",	OnCommand_ReturnGame)
AUI_ON_COMMAND("Btn_Help",		OnCommand_Help)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGamePlaza, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Img_Table*",		WM_LBUTTONDOWN,		OnEventLButtonDownTable)
// AUI_ON_EVENT("Btn_Chair*",		WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgGamePlaza::CDlgGamePlaza()
{
	m_iCurrentPage = 0;
	m_LastSel = -1;
}

CDlgGamePlaza::~CDlgGamePlaza()
{
}

bool CDlgGamePlaza::OnInitDialog()
{
	if (stricmp(m_szName, "Win_GamePoker_Hall")==0)
	{
		SetCanMove(false);
		int i, j;
		char szItemName[50];
		m_pScl_ViewTable = (PAUISCROLL)GetDlgItem("Scroll_ViewTable");
		m_pScl_ViewTable->SetScrollRange(0, (TOTAL_TABLES-1)*3/TABLE_PER_PAGE - 2);
		m_pScl_ViewTable->SetScrollStep(3);
		m_pScl_ViewTable->SetBarLevel(0);
		m_pLst_Player = (PAUILISTBOX)GetDlgItem("List_Player");
		m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
		m_pTxt_Title = (PAUILABEL)GetDlgItem("Txt_Title");
		m_pTxt_WinPercent = (PAUILABEL)GetDlgItem("Txt_WinPercent");
		m_pTxt_Score = (PAUILABEL)GetDlgItem("Txt_Score");
		
		m_pTxt_Title->SetText(_AL("0"));
		m_pTxt_WinPercent->SetText(_AL("0"));
		m_pTxt_Score->SetText(_AL("0"));
		GetDlgItem("Btn_Account")->Show(false);
		m_pImg_HeadIcon = (PAUIIMAGEPICTURE)GetDlgItem("Img_HeadIcon");
		for (i = 0; i < TABLE_PER_PAGE * PERSONS_PER_TABLE; i++)
		{
			sprintf(szItemName, "Img_Icon%d", i+1);
			m_pImg_Icon[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_Icon_M%d", i+1);
			m_pImg_IconM[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			// 		sprintf(szItemName, "Img_Ready%d", i+1);
			// 		m_pImg_Ready[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Lab_PlayerName%d", i+1);
			m_pLab_PlayerName[i] = (PAUILABEL)GetDlgItem(szItemName);
		}
		for (i = 0; i < TABLE_PER_PAGE; i++)
		{
			sprintf(szItemName, "Lab_Number%d", i+1);
			m_pLab_Num[i] = (PAUILABEL)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_Table%d", i+1);
			m_pImgTable[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
		}
		for(i=0; i<TABLE_PER_PAGE; i++)
			for (j=0; j<PERSONS_PER_TABLE; j++)
			{
				sprintf(szItemName, "Btn_Table%d_Chair%d", i+1, j+1);
				m_pBtn_Join[i][j] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szItemName);
			}
	}
	return true;
}

void CDlgGamePlaza::OnCommand_Confirm(const char * szCommand)
{

}

void CDlgGamePlaza::OnCommand_Cancel(const char * szCommand)
{
	CDlgGamePoker *pDlgGamePoker = (CDlgGamePoker*)GetGameUIMan()->GetDialog("Win_GamePoker_Match");
	if (pDlgGamePoker->IsInRoom())
	{
		GetGameUIMan()->AddChatMessage(GetStringFromTable(9193), GP_CHAT_MISC);
	}
	else
	{
		GetGameSession()->fgc2s_SendCmdPlayerLeaveHall(1);
		Show(false);
	}
}

void CDlgGamePlaza::OnCommand_Join(const char * szCommand)
{
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	int iTemp = atoi(szCommand + strlen("Btn_Chair")) - 1;
	int iTable = iTemp / PERSONS_PER_TABLE;
	int iChair = iTemp % PERSONS_PER_TABLE;
	int CurrenPage = m_pScl_ViewTable->GetBarLevel();
	int tableindex = iTable + CurrenPage * 3;
	const FG_ROOMINFO* pRoom = pPlaza->GetFGRoomInfo(tableindex);
	FG_PLAYERINFO PlayerInfo = pRoom->players[iChair];
	ACString strText;
	if (PlayerInfo.idPlayer > 0)
	{
		m_pTxt_Name->SetText(PlayerInfo.strName);
		m_pTxt_Title->SetText(GetTitle(PlayerInfo.iScore));
		int iTotalCount = PlayerInfo.iWinCount+PlayerInfo.iLoseCount+PlayerInfo.iDrawCount+PlayerInfo.iEscapeCount;
		strText.Format(_AL("%d%%"), iTotalCount>0 ? (1000*PlayerInfo.iWinCount/iTotalCount+5)/10 : 0);
		m_pTxt_WinPercent->SetText(strText);
		GetGameUIMan()->Int64ToStr(PlayerInfo.iScore, strText);
		m_pTxt_Score->SetText(strText);
	}
	else
	{
		if (m_LastSel >= 0)
		{
/*			GetGameSession()->fgc2s_SendCmdPlayerLeaveRoom(1, m_LastSel);*/
		}
		m_LastSel = tableindex;
		GetGameSession()->fgc2s_SendCmdPlayerEnterRoom(1, tableindex, iChair);
	}

}

void CDlgGamePlaza::OnCommand_Minimize(const char * szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameUIMan()->GetStringFromTable(9190), CDlgMinimizeBar::MINIMIZEBAR_TYPE_GAMEHALL);
}

void CDlgGamePlaza::OnCommand_ReturnGame(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GamePoker_Match")->Show(true);
}

void CDlgGamePlaza::OnCommand_Help(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GamePoker_Help");
	pDlg->Show(true);
}

void CDlgGamePlaza::OnTick()
{
	if (stricmp(m_szName, "Win_GamePoker_HallClose")==0)
	{
		AlignTo(GetGameUIMan()->GetDialog("Win_CharHead"),AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP, -1);
	}
	else if (stricmp(m_szName, "Win_GamePoker_Hall")==0)
	{
		UpdatePlaza();

		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_ChatInfo2"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_ChatInfo"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_Chat"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_KfpkChat"));
		static int num = 0;
		num = (num + 1) % 1100;
		m_pImg_HeadIcon->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], num/20);

		int iCurSel = m_pLst_Player->GetCurSel();
		if (iCurSel>=0 && iCurSel<m_pLst_Player->GetCount())
		{
			ACString strText = m_pLst_Player->GetText(iCurSel);
			CSplit s(strText);
			CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
			if (vec.size()>=4)
			{
				m_pTxt_Name->SetText(vec[0]);
				m_pTxt_Title->SetText(vec[1]);
				m_pTxt_WinPercent->SetText(vec[2]);
				m_pTxt_Score->SetText(vec[3]);
			}
		}
		GetDlgItem("Btn_ReturnMatch")->Show(((CDlgGamePoker*)GetGameUIMan()->GetDialog("Win_GamePoker_Match"))->IsInRoom());
	}
}

void CDlgGamePlaza::OnShowDialog()
{
	if (stricmp(m_szName, "Win_GamePoker_Hall")==0)
	{
		CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
		pPlaza->ClearFGPlaza();
		GetGameSession()->fgc2s_SendCmdPlayerEnterHall(1);
		for (int i=0; i<10; i++)
		{
			g_pGame->GetGameSession()->fgc2s_SendCmdGetRoomList(1, i);
		}
		UpdatePlaza();
	}
}

void CDlgGamePlaza::UpdatePlaza()
{
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	int i, j;
	int CurrenPage = m_pScl_ViewTable->GetBarLevel();
	ACHAR szText[50];
	for (i = 0; i < TABLE_PER_PAGE; i++)
	{
		int tableindex = i + CurrenPage * 3;
		a_sprintf(szText, _AL("- %d -"), tableindex+1);
		m_pLab_Num[i]->SetText(szText);
		if (tableindex >= TOTAL_TABLES)
		{
			for (j = 0; j < PERSONS_PER_TABLE; j++)
			{
				m_pLab_PlayerName[i*3+j]->SetText(_AL(""));
				m_pBtn_Join[i][j]->Show(false);
				m_pImg_Icon[i*3+j]->Show(false);
				m_pImg_IconM[i*3+j]->Show(false);
			}
			m_pImgTable[i]->Show(false);
			m_pLab_Num[i]->Show(false);
			break;
		}
		else
		{
			for (j = 0; j < PERSONS_PER_TABLE; j++)
			{
				m_pBtn_Join[i][j]->Show(true);
//				m_pImg_Ready[i*3+j]->Show(false);
			}
			m_pImgTable[i]->Show(true);
			m_pLab_Num[i]->Show(true);
		}
		const FG_ROOMINFO* pRoom = pPlaza->GetFGRoomInfo(tableindex);
		for (j = 0; j < PERSONS_PER_TABLE; j++)
		{
			m_pImg_Icon[i*3+j]->Show(false);
			m_pImg_IconM[i*3+j]->Show(false);
			if (pRoom->players[j].idPlayer > 0)
			{
// 				if(pRoom->players[j].idPlayer == GetHostPlayer()->GetCharacterID())
// 				{
// 					Show(false);
// 					GetGameUIMan()->GetDialog("Win_GamePoker_Match")->Show(true);
// 					return;
// 				}
				if (pRoom->players[j].cGender)
					m_pImg_Icon[i*3+j]->Show(true);
				else
					m_pImg_IconM[i*3+j]->Show(true);
				m_pBtn_Join[i][j]->Show(false);
				m_pLab_PlayerName[i*3+j]->SetText(pRoom->players[j].strName);
//				m_pImg_Ready[i*3+j]->Show(pRoom->players[j].iState == 1);
			}
			else
			{
				m_pLab_PlayerName[i*3+j]->SetText(_AL(""));
				m_pBtn_Join[i][j]->Show(true);
//				m_pImg_Ready[i*3+j]->Show(false);
			}
		}
	}

}

void CDlgGamePlaza::PlayerEnterHall(FG_PLAYERINFO *pData)
{
	ACString strText;
	ACString strScore;
	GetGameUIMan()->Int64ToStr(pData->iScore, strScore);
	int iTotalCount = pData->iWinCount+pData->iLoseCount+pData->iDrawCount+pData->iEscapeCount;
	strText.Format(_AL("%s\t%s\t%d%%\t%s"), pData->strName, GetTitle(pData->iScore), 
		iTotalCount>0 ? (1000*pData->iWinCount/iTotalCount+5)/10 : 0,
		strScore);
	if (pData->idPlayer == GetHostPlayer()->GetCharacterID())
	{
		m_pLst_Player->InsertString(0, strText);
		m_pLst_Player->SetItemData(0, pData->idPlayer);
		m_pLst_Player->SetCurSel(0);
	}
	else
	{
		m_pLst_Player->AddString(strText);
		m_pLst_Player->SetItemData(m_pLst_Player->GetCount()-1, pData->idPlayer);
	}
}

void CDlgGamePlaza::PlayerLeaveHall(int playerId)
{
	for (int i=0; i<m_pLst_Player->GetCount(); i++)
	{
		if ((int)m_pLst_Player->GetItemData(i) == playerId)
		{
			m_pLst_Player->DeleteString(i);
			break;
		}
	}
}

void CDlgGamePlaza::UpdateHallMember()
{
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	abase::vector<FG_PLAYERINFO>& members = pPlaza->GetHallMembers();
	m_pLst_Player->ResetContent();
	int idHost = GetHostPlayer()->GetCharacterID();
	ACString strText;
	for (UINT i=0; i<members.size(); i++) 
	{
		ACString strScore;
		GetGameUIMan()->Int64ToStr(members[i].iScore, strScore);
		int iTotalCount = members[i].iWinCount+members[i].iLoseCount+members[i].iDrawCount+members[i].iEscapeCount;
		strText.Format(_AL("%s\t%s\t%d%%\t%s"), members[i].strName, GetTitle(members[i].iScore), 
			iTotalCount>0 ? (1000*members[i].iWinCount/iTotalCount+5)/10 : 0,
		strScore);
		if (members[i].idPlayer == idHost)
		{
			m_pLst_Player->InsertString(0, strText);
			m_pLst_Player->SetItemData(0, members[i].idPlayer);
			m_pLst_Player->SetCurSel(0);
		}
		else
		{
			m_pLst_Player->AddString(strText);
			m_pLst_Player->SetItemData(m_pLst_Player->GetCount()-1, members[i].idPlayer);
		}
	}
}

void CDlgGamePlaza::OnEventLButtonDownTable(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
 	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
	int index = atoi(pObj->GetName()+strlen("Img_Table"));
	int CurrenPage = m_pScl_ViewTable->GetBarLevel();
	int tableindex = index - 1 + CurrenPage * 3;
	const FG_ROOMINFO* pRoom = pPlaza->GetFGRoomInfo(tableindex);
	for (int j = 0; j < PERSONS_PER_TABLE; j++)
	{
		if(pRoom->players[j].idPlayer == GetHostPlayer()->GetCharacterID())
			GetGameUIMan()->GetDialog("Win_GamePoker_Match")->Show(true);
	}
}

void CDlgGamePlaza::OnCommand_Guest(const char * szCommand)
{
	int iTemp = atoi(szCommand + strlen("Btn_Guest")) - 1;
	int CurrenPage = m_pScl_ViewTable->GetBarLevel();
	int tableindex = iTemp + CurrenPage * 3;
}

void CDlgGamePlaza::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (stricmp(m_szName, "Win_GamePoker_Hall")==0)
	{
		PAUIDIALOG pDlgMouseOn;
		PAUIOBJECT pObjMouseOn;
		GetGameUIMan()->GetMouseOn(&pDlgMouseOn, &pObjMouseOn);
		if (pObjMouseOn && pObjMouseOn == GetDlgItem("List_Player"))
		{
			return;
		}

		int x = GET_X_LPARAM(lParam);
		int y = GET_Y_LPARAM(lParam);
		int zDelta = (short)HIWORD(wParam);
		if( zDelta > 0 )
			zDelta = 1;
		else
			zDelta = -1;
		if( m_pScl_ViewTable->IsShow())
			m_pScl_ViewTable->SetBarLevel(m_pScl_ViewTable->GetBarLevel() - zDelta);
	}
}

ACString CDlgGamePlaza::GetTitle(INT64 iScore)
{
	int i;
	for (i=0; i<sizeof(ddzlevel)/sizeof(int); i++)
	{
		if (iScore<ddzlevel[i])
			return GetGameUIMan()->GetStringFromTable(9200+i);
	}
	return GetGameUIMan()->GetStringFromTable(9200+i);
}