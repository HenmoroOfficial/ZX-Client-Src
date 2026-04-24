// Filename	: DlgGamePoker.cpp
// Creator	: Chen Zhixin
// Date		: 2009/03/30

#include "DlgGamePoker.h"
#include "DlgGamePlaza.h"
#include "DlgMinimizeBar.h"
#include "EC_Game.h"
#include "EC_Configs.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_FunGame.h"
#include "EC_GameSession.h"


#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AUIDef.h"
#include "AUI\\CSplit.h"

#define new A_DEBUG_NEW
#define TIMEINTERVAL 200
#define SHOWTIME 3000
#define RESULTTIME 3000
#define WAITINGTIME 30000

AUI_BEGIN_COMMAND_MAP(CDlgGamePoker, CDlgBase)

AUI_ON_COMMAND("Btn_Ready",		OnCommand_Ready)
AUI_ON_COMMAND("Btn_Exit",		OnCommand_Leave)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_Minimize)
AUI_ON_COMMAND("Btn_Close",		OnCommand_Leave)
AUI_ON_COMMAND("Btn_Discard",	OnCommand_Discard)
AUI_ON_COMMAND("Btn_Pass",		OnCommand_Pass)
AUI_ON_COMMAND("Btn_Point*",	OnCommand_Auction)
AUI_ON_COMMAND("Btn_Minimize",	OnCommand_Minimize)
AUI_ON_COMMAND("Btn_Help",		OnCommand_Help)
AUI_ON_COMMAND("Btn_ReturnHall",OnCommand_ReturnHall)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgGamePoker, CDlgBase)

AUI_ON_EVENT("Img_Char2_Card*",		WM_LBUTTONUP,		OnEventLButtonUp_Card)
AUI_ON_EVENT("List_Player",			WM_LBUTTONUP,		OnEventLButtonUp_List)

AUI_END_EVENT_MAP()

CDlgGamePoker::CDlgGamePoker()
{
	memset(m_bChoose, 0, MAX_CARDS * sizeof(bool));
	m_hostDir = -1;
	m_CurSel = -1;
	m_dwDispatchTime = 0;
	m_dwShowTime = 0;
	m_dwResultTime = 0;
	m_dwWaitingTime = 0;
	m_RoomId = -1;
	m_LandlordID = 0;
}

CDlgGamePoker::~CDlgGamePoker()
{
}

bool CDlgGamePoker::OnInitDialog()
{
	if (stricmp(m_szName, "Win_GamePoker_Match")==0)
	{
		SetCanMove(false);
		int i, j;
		char szItemName[50];
		m_pLst_Player = (PAUILISTBOX)GetDlgItem("List_Player");
		m_pTxt_Name = (PAUILABEL)GetDlgItem("Txt_Name");
		m_pTxt_Title = (PAUILABEL)GetDlgItem("Txt_Title");
		m_pTxt_WinPercent = (PAUILABEL)GetDlgItem("Txt_WinPercent");
		m_pTxt_Score = (PAUILABEL)GetDlgItem("Txt_Score");
		m_pImg_HeadIcon = (PAUIIMAGEPICTURE)GetDlgItem("Img_HeadIcon");
		m_pImg_PokerFinish = (PAUIIMAGEPICTURE)GetDlgItem("Img_PokerFinish");
		m_pImg_PokerFinish->Show(false);

		for (i = 0; i < 3; i++)
		{
			for (j = 0; j < MAX_CARDS; j++)
			{
				sprintf(szItemName, "Img_Char%d_Card%d", i+1, j+1);
				m_pImg_PlayPoker[i][j] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
				sprintf(szItemName, "Img_Char%d_Show%d", i+1, j+1);
				m_pImg_ShowPoker[i][j] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			}
			sprintf(szItemName, "Img_SelectPoker%d", i+1);
			m_pImg_SelectPoker[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_PlayerStatus%d", i+1);
			m_pImg_PlayerStatus[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_PlayerClock%d", i+1);
			m_pImg_PlayerClock[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Lab_PlayerStatus%d", i+1);
			m_pLab_PlayerStatus[i] = (PAUILABEL)GetDlgItem(szItemName);
			sprintf(szItemName, "Lab_PlayerName%d", i+1);
			m_pLab_PlayerName[i] = (PAUILABEL)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_PlayerHead%d", i+1);
			m_pImg_PlayerHead[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Img_PlayerHeadf%d", i+1);
			m_pImg_PlayerHeadF[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItemName);
			sprintf(szItemName, "Lab_Ready%d", i+1);
			m_pLab_Ready[i] = (PAUILABEL)GetDlgItem(szItemName);
			sprintf(szItemName, "Lab_CallPoint%d", i+1);
			m_pLab_CallPoint[i] = (PAUILABEL)GetDlgItem(szItemName);
			m_pLab_CallPoint[i]->Show(false);
		}
		for (i = 0; i < 4; i++)
		{
			sprintf(szItemName, "Btn_Point%d", i);
			m_pBtn_Order[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szItemName);
		}
		m_pBtn_Play[0] = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Pass");
		m_pBtn_Play[1] = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Discard");
		//m_pBtn_Play[2] = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Tip");
		m_pBtn_Ready = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Ready");
		m_pProg_Time = (PAUIPROGRESS)GetDlgItem("Prog_Time");
		for (i = 0; i < 3; i++)
		{
			ResetPlayerInfo(i);
		}
	}
	return true;
}

void CDlgGamePoker::OnCommand_Ready(const char * szCommand)
{
	assert(m_RoomId >= 0);
	GetGameSession()->fgc2s_SendCmdPlayerBeginGame(1, m_RoomId);
}

void CDlgGamePoker::OnCommand_Leave(const char * szCommand)
{
	if (m_bIsGaming)
		GetGameUIMan()->MessageBox("Game_QuitDdz", GetStringFromTable(9186), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
	else
		QuitGame();
}

void CDlgGamePoker::QuitGame()
{
	assert(m_RoomId >= 0);
	GetGameSession()->fgc2s_SendCmdPlayerLeaveRoom(1, m_RoomId);
	//暂时用来关闭界面
	for (int j = 0; j < MAX_CARDS; j++)
	{
		if (m_bChoose[j])
		{
			POINT pt  = m_pImg_PlayPoker[1][j]->GetPos(true);
			m_pImg_PlayPoker[1][j]->SetPos(pt.x, pt.y + 8);
			m_bChoose[j] = false;
		}
	}
	Show(false);
	CECFunGamePlaza* pPlaza = g_pGame->GetFunGamePlaza();
}

void CDlgGamePoker::RefreshResult(const FG_S2C::CMD::fgcmd_landlord_result_broadcast& fgci)
{
	for(int i=0; i<fgci.size; i++)
	{
		for (int j=0; j<3; j++)
		{
			if (fgci.results[i].roleid == m_playerinfo[j].roleid)
			{
				m_playerinfo[j].score += fgci.results[i].score;
				if (fgci.results[i].score>0)
					m_playerinfo[j].win_count++;
				else
					m_playerinfo[j].lose_count++;
			}
		}
	}
	RefreshPlayerDetails();
}

void CDlgGamePoker::OnCommand_Discard(const char * szCommand)
{
	int i;
	int num  = 0;
	BYTE aCard[MAX_CARDS];
	for (i = 0; i < (int)m_vecOwnCard.size(); i++)
	{
		if (m_bChoose[i])
		{
			aCard[num] = m_vecOwnCard[i]; 
			num++;
		}
	}
	if (num == 0)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8486), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
		GetGameSession()->fgc2s_SendCmdPlayerThrowNormalCard(1, m_RoomId, num, aCard);
}

void CDlgGamePoker::OnCommand_Auction(const char * szCommand)
{
	int type = atoi(szCommand + strlen("Btn_Point"));
	if (type == 0)
	{
		GetGameSession()->fgc2s_SendCmdPlayerGiveupNormalCard(1, m_RoomId);
	}
	else
		GetGameSession()->fgc2s_SendCmdPlayerAuction(1, m_RoomId, GetHostPlayer()->GetCharacterID(), type);
}

void CDlgGamePoker::OnCommand_Minimize(const char * szCommand)
{
	GetGameUIMan()->m_pDlgMinimizeBar[0]->MinimizeDialog(this, 
		GetGameUIMan()->GetStringFromTable(9191), CDlgMinimizeBar::MINIMIZEBAR_TYPE_GAMEDDZ);
}

void CDlgGamePoker::OnCommand_Help(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_GamePoker_Help");
	pDlg->Show(true);
}

void CDlgGamePoker::OnCommand_ReturnHall(const char * szCommand)
{
	GetGameUIMan()->GetDialog("Win_GamePoker_Hall")->Show(true);
}

void CDlgGamePoker::OnCommand_Pass(const char * szCommand)
{
	GetGameSession()->fgc2s_SendCmdPlayerGiveupNormalCard(1, m_RoomId);
}

void CDlgGamePoker::OnCommand_Cancel(const char * szCommand)
{
	Show(false);
}

void CDlgGamePoker::OnTick()
{
	if (stricmp(m_szName, "Win_GamePoker_MatchClose")==0)
	{
		AlignTo(GetGameUIMan()->GetDialog("Win_CharHead"),AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_RIGHT,
			AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_TOP, -1);
	}
	else if (stricmp(m_szName, "Win_GamePoker_Match")==0)
	{
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_ChatInfo2"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_ChatInfo"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_Chat"));
		GetGameUIMan()->BringWindowToTop(GetGameUIMan()->GetDialog("Win_KfpkChat"));
		int i,j;
		//发牌冷却时间
		if (m_dwDispatchTime > 0)
		{
			DWORD time0 = GetTickCount() - m_dwDispatchTime;
			int num = time0 / (TIMEINTERVAL*3);
			if (num <= 17)
			{
				int dir = (time0 % (TIMEINTERVAL*3)) / TIMEINTERVAL;
				UpdateCards(dir, num);	
			}
			else
			{
				m_dwDispatchTime = 0;
				for (i = 0; i < 3; i++)
				{
					m_pImg_SelectPoker[i]->Show(true);
				}
			}
			return;
		}
		//看底牌冷却时间
		else if (m_dwShowTime > 0)
		{
			if (GetTickCount() - m_dwShowTime >= SHOWTIME)
			{
				for (i = 0; i < 3; i++)
				{
					m_pImg_SelectPoker[i]->Show(false);
					//恢复成背面牌
				}
				m_dwShowTime = 0;
			}
			//return;
		}
		//看结果冷却时间
		else if (m_dwResultTime > 0)
		{
			if (GetTickCount() - m_dwResultTime >= RESULTTIME)
			{
				m_pImg_PokerFinish->Show(false);
				m_dwResultTime = 0;
			}
		}
		//players状态更新
		for (i = 0; i < 3;  i++)
		{
			int index = (i + 4 - m_hostDir) % 3;
			bool bShow = m_playerinfo[i].playerstate == PLAYERSTATE_GAME && m_LandlordID == m_playerinfo[i].roleid;
			m_pImg_PlayerStatus[index]->Show(bShow);
			m_pLab_PlayerStatus[index]->Show(m_LandlordID>0 && m_playerinfo[i].playerstate == PLAYERSTATE_GAME);
			int txIndex = 9183;
			if (m_LandlordID == m_playerinfo[i].roleid)
				txIndex = 9184;
			else if (m_LandlordID != GetHostPlayer()->GetCharacterID() && m_playerinfo[i].roleid != GetHostPlayer()->GetCharacterID())
				txIndex = 9192;
			m_pLab_PlayerStatus[index]->SetText(GetStringFromTable(txIndex));
			m_pLab_Ready[index]->Show(m_playerinfo[i].playerstate == PLAYERSTATE_READY);
			m_pImg_PlayerClock[index]->Show(false);
			if (m_pLab_CallPoint[index]->IsShow())
			{
				bShow = (m_playerinfo[i].playerstate == PLAYERSTATE_GAME) && (m_playerinfo[i].state == STATE_WAIT_CALL || m_playerinfo[i].roleid == m_LandlordID);
				m_pLab_CallPoint[index]->Show(bShow);
			}
			if (m_playerinfo[i].playerstate == 0)						//进入房间
			{
				
			}
			else if (m_playerinfo[i].playerstate == PLAYERSTATE_READY  || m_playerinfo[i].playerstate == -1)					//准备好
			{
				m_playerinfo[i].deskcard.clear();
				m_playerinfo[i].hidecard.clear();
				m_playerinfo[i].cardsize = 0;
				UpdateCards(i);
			}
			else if (m_playerinfo[i].playerstate == 2)					//正在游戏
			{
				if (STATE_INTURN_CALL == m_playerinfo[i].state || STATE_INTURN_DISCARD == m_playerinfo[i].state)
				{
					m_pImg_PlayerClock[index]->Show(true);
				}
			}
			else if (m_playerinfo[i].playerstate == 3)					//掉线
			{
				
			}
		}
		m_pProg_Time->Show(m_playerinfo[m_hostDir].playerstate == 2);	//在游戏中
		if (m_dwWaitingTime > 0)
		{
			if (WAITINGTIME + m_dwWaitingTime > GetTickCount())
			{
				m_pProg_Time->SetProgress((WAITINGTIME + m_dwWaitingTime - GetTickCount()) * 100 / WAITINGTIME);
			}
			else
			{
				m_pProg_Time->SetProgress(0);
			}
		}
		//Host按钮更新
		i = m_hostDir;
		bool bBtnOrderShow = m_playerinfo[i].state == STATE_INTURN_CALL || m_playerinfo[i].state == STATE_WAIT_CALL;
		bool bBtnOrderEnable = m_playerinfo[i].state == STATE_INTURN_CALL;
		bool bBtnPlayShow = m_playerinfo[i].state == STATE_INTURN_DISCARD || m_playerinfo[i].state == STATE_WAIT_DISCARD;
		bool bBtnPlayEnable = m_playerinfo[i].state == STATE_INTURN_DISCARD;
		for (j =0; j < 4; j++)
		{
			m_pBtn_Order[j]->Show(bBtnOrderShow && m_playerinfo[i].playerstate == 2);
			m_pBtn_Order[j]->Enable(bBtnOrderEnable && m_playerinfo[i].playerstate == 2);
		}
		for (j =0; j < 2/*3*/; j++)
		{
			m_pBtn_Play[j]->Show(bBtnPlayShow && m_playerinfo[i].playerstate == 2);
			m_pBtn_Play[j]->Enable(bBtnPlayEnable && m_playerinfo[i].playerstate == 2);
		}
		if (m_playerinfo[i].playerstate == 0)
		{
			m_pBtn_Ready->Enable(true);
			for (i = 0; i < 3; i++)
			{
				m_pImg_SelectPoker[i]->Show(false);
			}
		}
		else if (m_playerinfo[i].playerstate == 1)
		{
			m_pBtn_Ready->Enable(false);
			m_vecOwnCard.clear();
		}
		else if (m_playerinfo[i].playerstate == 2)
		{
			m_pBtn_Ready->Enable(false);
		}

	}
}

void CDlgGamePoker::OnShowDialog()
{
	if (stricmp(m_szName, "Win_GamePoker_Match")==0)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			UpdateCards(i);
		}
		ACString str;
		str.Format(GetStringFromTable(9185), m_RoomId + 1);
		((PAUILABEL)GetDlgItem("Lab_Num"))->SetText(str);
	}
}

void CDlgGamePoker::UpdateCards(int direction, int num)
{
	int j;
// 	int index = m_playerinfo[direction].index;
	//手中的牌
	int index = (direction + 4 - m_hostDir) % 3;
	for (j = 0; j < m_playerinfo[direction].cardsize; j++)
	{
		if (j < num)
		{
			m_pImg_PlayPoker[index][j]->Show(true);
			if (direction == m_hostDir)
			{
				m_pImg_PlayPoker[index][j]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], m_vecOwnCard[j]);
			}
			else
			{
				m_pImg_PlayPoker[index][j]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], 0);
			}
		}
	}
	for (; j < MAX_CARDS; j++)
	{
		m_pImg_PlayPoker[index][j]->Show(false);	
	}
	//桌面的牌
	int size = (int)m_playerinfo[direction].deskcard.size();
	int head = (MAX_CARDS - size)/2;
	for (j =0 ; j < MAX_CARDS; j++)
	{
		if (j >= head && j < head + size)
		{
			m_pImg_ShowPoker[index][j]->Show(true);
			m_pImg_ShowPoker[index][j]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], m_playerinfo[direction].deskcard[j-head]);
		}
		else
			m_pImg_ShowPoker[index][j]->Show(false);
	}
}

void CDlgGamePoker::OnEventLButtonUp_Card(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
 	int index = atoi(pObj->GetName() + strlen("Img_Char2_Card")) - 1;
	POINT pt = pObj->GetPos(true);
	pObj->SetPos(pt.x, pt.y + 8 * (m_bChoose[index]? 1:-1));
	m_bChoose[index] = !m_bChoose[index];
}

void CDlgGamePoker::OnEventLButtonUp_List(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("List_Player");
	if (!pList)
	{
		return;
	}
	
	int sel = pList->GetCurSel();
	if (sel >= 0 && sel < pList->GetCount())
	{
		ACString strText = m_pLst_Player->GetText(sel);
		
		CSplit s(strText);
		CSPLIT_STRING_VECTOR vec = s.Split(_AL("\t"));
		if (vec.size()>=4)
		{
			m_pTxt_Name->SetText(vec[0]);
			m_pTxt_Title->SetText(vec[1]);
			m_pTxt_WinPercent->SetText(vec[2]);
			m_pTxt_Score->SetText(vec[3]);
		}
		m_CurSel = sel;
	}
}

void CDlgGamePoker::PokerCmd_RoomDispatchNormalCard(char cardsize, char* card, int nextroleid)
{
	m_bIsGaming = true;
	m_dwWaitingTime = GetTickCount();
	m_vecOwnCard.clear();
	m_LandlordID = 0;
	int i;
	for (i = 0; i < cardsize; i++)
	{
		m_vecOwnCard.push_back(card[i]);
	}
	for (i = 0; i < 3; i++)
	{
		m_playerinfo[i].cardsize = cardsize;
	}
	SetPlayingState(nextroleid, STATE_INTURN_CALL, STATE_WAIT_CALL);
	for (i = 0; i < 3; i++)
	{
		m_pLab_Ready[i]->Show(false);
		m_pImg_SelectPoker[i]->Show(true);
		m_pImg_SelectPoker[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], 0);
	}
	//m_dwDispatchTime = GetTickCount();
	for (i=0; i<3; i++)
	{
		for (int j=0; j<17; j++)
		{
			UpdateCards(i, 17);	
		}
	}
}

void CDlgGamePoker::PokerCmd_PlayerInturnNormalCard(int roleid)
{
	m_dwWaitingTime = GetTickCount();
	for (int i = 0; i < 3; i++)
	{
		if (m_playerinfo[i].roleid == roleid)
		{
			int index = (i + 4 - m_hostDir) % 3;
			SetPlayingState(roleid, STATE_INTURN_DISCARD, STATE_WAIT_DISCARD);
		}
	}
}

void CDlgGamePoker::PokerCmd_PlayerThrowNormalCard(int roleid, int nextroleid, char card_size, char* cards)
{
	int i,j;
	int dir = 0;
	m_dwWaitingTime = GetTickCount();
	for (i = 0; i < 3; i++)
	{
		if (m_playerinfo[i].roleid == roleid)
		{
			dir = i;
			m_playerinfo[i].cardsize -= card_size;
			m_playerinfo[i].hidecard.clear();
			for (j = 0; j < (int)m_playerinfo[i].deskcard.size(); j++)
			{
				m_playerinfo[i].hidecard.push_back(m_playerinfo[i].deskcard[j]);
			}
			m_playerinfo[i].deskcard.clear();
			for (j = 0; j < card_size; j++)
			{
				m_playerinfo[i].deskcard.push_back(cards[j]);
			}
			break;
		}
	}
	if (GetHostPlayer()->GetCharacterID() == roleid)
	{
		for (j = 0; j < MAX_CARDS; j++)
		{
			if (m_bChoose[j])
			{
				POINT pt  = m_pImg_PlayPoker[1][j]->GetPos(true);
				m_pImg_PlayPoker[1][j]->SetPos(pt.x, pt.y + 8);
				m_bChoose[j] = false;
			}
		}
		for (i = 0; i < card_size; i++)
		{
			for (j = 0; j < (int)m_vecOwnCard.size(); j++)
			{
				if (cards[i] == m_vecOwnCard[j])
				{
					m_vecOwnCard.erase(m_vecOwnCard.begin() + j);
					break;
				}
			}
		}
	}
	UpdateCards(dir);
	SetPlayingState(nextroleid, STATE_INTURN_DISCARD, STATE_WAIT_DISCARD);
}

void CDlgGamePoker::PokerCmd_PlayerPickupNormalCard(char cardsize, char* card, int roleid)
{
	m_LandlordID = roleid;
	int i;
	m_dwWaitingTime = GetTickCount();
	m_dwShowTime = GetTickCount();
	if (roleid != GetHostPlayer()->GetCharacterID())
	{
		for (int k = 0; k  < 3; k++)
		{
			if(m_playerinfo[k].roleid == roleid)
			{
				m_playerinfo[k].cardsize += cardsize;
				UpdateCards(k);
			}
		}
		
		for (i = 0; i < 3; i++)
		{
			m_pImg_SelectPoker[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], card[i]);
		}
		SetPlayingState(roleid, STATE_INTURN_DISCARD, STATE_WAIT_DISCARD);
		return;
	}
	for (i = 0; i < cardsize; i++)
	{
		if (card[i] < m_vecOwnCard[0])
		{
			m_vecOwnCard.insert(m_vecOwnCard.begin(), card[i]);
			continue;
		}
		int j;
		for (j = 0; j < (int)m_vecOwnCard.size() -1 ; j++)
		{
			if (m_vecOwnCard[j] < card[i] && m_vecOwnCard[j+1] > card [i])
			{
				m_vecOwnCard.insert(m_vecOwnCard.begin() + j+1, card[i]);
				break;
			}
		}	
		if (j == (int)m_vecOwnCard.size() -1)
		{
			m_vecOwnCard.push_back(card[i]);
		}
	}
	m_playerinfo[m_hostDir].cardsize += cardsize;
	for (i = 0; i < 3; i++)
	{
		m_pImg_SelectPoker[i]->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_POKER], card[i]);
	}
	UpdateCards(m_hostDir);
	SetPlayingState(roleid, STATE_INTURN_DISCARD, STATE_WAIT_DISCARD);
}

void CDlgGamePoker::PokerCmd_PlayerGiveupNormalCard(int roleid, int nextroleid)
{
	int i;
	m_dwWaitingTime = GetTickCount();
	for (i = 0; i < 3; i++)
	{
		if (m_playerinfo[i].roleid == roleid)
		{
			int index = (i + 4 - m_hostDir) % 3;
			if (m_playerinfo[i].state == STATE_INTURN_CALL)
			{
				m_pLab_CallPoint[index]->SetText(_AL("不叫"));
				m_pLab_CallPoint[index]->Show(true);
				SetPlayingState(nextroleid, STATE_INTURN_CALL, STATE_WAIT_CALL);
			}
			else if(m_playerinfo[i].state == STATE_INTURN_DISCARD)
			{
				PokerCmd_PlayerThrowNormalCard(roleid, nextroleid, 0, NULL);
			}
		}
	}
	return;
}

void CDlgGamePoker::PokerCmd_PlayerAuction(int roleid, int nextroleid, int score)
{
	int i;
	m_dwWaitingTime = GetTickCount();
	for (i = 0; i < 3; i++)
	{
		int index = (i + 4 - m_hostDir) % 3;
		if (m_playerinfo[i].roleid == roleid)
		{
			ACString str;
			str.Format(_AL("%d分"), score);
			m_pLab_CallPoint[index]->SetText(str);
			m_pLab_CallPoint[index]->Show(true);
		}
	}
	SetPlayingState(nextroleid, STATE_INTURN_CALL, STATE_WAIT_CALL);
}

void CDlgGamePoker::PokerCmd_NotifyRoomInfo(const FG_S2C::INFO::room_info& fgri)
{
	using namespace FG_S2C::INFO;
	m_bIsGaming = false;
	m_RoomId = fgri.roomid;
	ACString str;
	str.Format(_AL("%d"), m_RoomId);
	((PAUILABEL)GetDlgItem("Lab_Num"))->SetText(str);
	for (int i = 0; i < fgri.player_size; i++)
	{
		player_base_info fgci = fgri.players[i];
		UpdateRoomInfo(fgci);
	}
	RefreshPlayerDetails();
}

void CDlgGamePoker::PokerCmd_PlayerEnterRoom(const FG_S2C::INFO::player_base_info& fgci)
{
	if (fgci.roleid == GetHostPlayer()->GetCharacterID())
	{
		return;
	}
	UpdateRoomInfo(fgci);
	RefreshPlayerDetails();
}

void CDlgGamePoker::PokerCmd_PlayerLeaveRoom(int roleid)
{
	int i;
	if (GetHostPlayer()->GetCharacterID() == roleid)
	{
		for (i = 0; i < 3; i++)
		{
			ResetPlayerInfo(i);
			RefreshPlayerDetails();
			Show(false);
		}
		m_RoomId = -1;
	}
	else
	{
		for (i = 0; i < 3;  i++)
		{
			if (roleid == m_playerinfo[i].roleid)
			{
				ResetPlayerInfo(i);
				RefreshPlayerDetails();
				return;
			}
		}
	}
}

void CDlgGamePoker::PokerCmd_RoomIsReady(int roomid)
{
	if (roomid == m_RoomId)
	{
		int i;
		for (i = 0; i < 3; i++)
		{
			UpdatePlayState(i, 0);
			m_playerinfo[i].cardsize = 0;
			m_playerinfo[i].deskcard.clear();
			m_playerinfo[i].hidecard.clear();
		}
		for (i = 0; i < 3; i++)
			UpdateCards(i);
	}

}

void CDlgGamePoker::PokerCmd_RoomGameEnd(int roleid, char reason)
{
	m_bIsGaming = false;
	switch(reason)
	{
		//正常结束	
	case 0:
		{
			bool bWin = false;
			if (roleid == GetHostPlayer()->GetCharacterID())
			{
				bWin = true;	//地主赢
			}
			else if (m_LandlordID != GetHostPlayer()->GetCharacterID() && m_LandlordID!=roleid)
			{
				bWin = true;	//人民赢
			}
			m_pImg_PokerFinish->FixFrame(bWin? 1:0);
			m_pImg_PokerFinish->Show(true);
			m_dwResultTime = GetTickCount();
			break;
		}
	case 1:
		{
			//某一方掉线则不作处理
// 			if (roleid != GetHostPlayer()->GetCharacterID())
// 			{
// 				m_pImg_PokerFinish->FixFrame(1);
// 				m_pImg_PokerFinish->Show(true);
 				m_dwResultTime = GetTickCount();
// 			}
		}
		break;
	}
		
}

void CDlgGamePoker::PokerCmd_LandlordReonlineDataSend(const FG_S2C::INFO::landlord_reonline_info& fgci)
{
	using namespace FG_S2C::INFO;
	int i;
	for (i = 0; i< 3; i++)
	{
		ResetPlayerInfo(i);
	}
	PokerCmd_NotifyRoomInfo(fgci.cur_room_info);
	m_LandlordID = m_playerinfo[fgci.lord_dir].roleid;
	m_vecOwnCard.clear();

	for (i = 0; i < fgci.self_card_size; i++)
	{
		m_vecOwnCard.push_back(fgci.self_cards[i]);
	}
	m_playerinfo[m_hostDir].cardsize = fgci.self_card_size;
	for (i = 0; i < fgci.desk_card_size; i++)
	{
		m_playerinfo[fgci.desk_dir].deskcard.push_back(fgci.desk_cards[i]);
	}
	m_playerinfo[fgci.cards[0].direct].cardsize = fgci.cards[0].card_size;
	m_playerinfo[fgci.cards[1].direct].cardsize = fgci.cards[1].card_size;

	SetPlayingState(m_playerinfo[fgci.next_dir].roleid, STATE_INTURN_DISCARD, STATE_WAIT_DISCARD);
	for (i = 0; i < 3; i++)
	{
		UpdateCards(i);
	}
	
}

void CDlgGamePoker::ResetPlayerInfo(int index)
{
	m_playerinfo[index].direction = -1;
	m_playerinfo[index].roleid = 0;
	m_playerinfo[index].index = -1;
	m_playerinfo[index].name = _AL("");
	m_playerinfo[index].state = 0;
	m_playerinfo[index].playerstate = 0;
	m_playerinfo[index].cardsize = 0;
	m_playerinfo[index].deskcard.clear();
	m_playerinfo[index].hidecard.clear();
	UpdateCards(index);
}

void CDlgGamePoker::UpdateRoomInfo(const FG_S2C::INFO::player_base_info& fgci)
{
	int direction = fgci.direction;
	m_playerinfo[direction].direction = fgci.direction;
	m_playerinfo[direction].roleid = fgci.roleid;
	AWString strTmp((ACHAR*)fgci.name, fgci.name_size / sizeof(ACHAR));
	m_playerinfo[direction].gender = fgci.gender;
	m_playerinfo[direction].score = fgci.score;
	m_playerinfo[direction].win_count = fgci.win_count;
	m_playerinfo[direction].lose_count = fgci.lose_count;
	m_playerinfo[direction].draw_count = fgci.draw_count;
	m_playerinfo[direction].escape_count = fgci.escape_count;
	m_playerinfo[direction].name = strTmp;
	m_playerinfo[direction].playerstate = fgci.state;
	if (fgci.roleid == GetHostPlayer()->GetCharacterID())
	{
		m_hostDir = fgci.direction;
		m_playerinfo[direction].index = 0;
	}
	else
	{	
		m_playerinfo[direction].index = (fgci.direction + 4 - m_hostDir) % 3;
	}
}

void CDlgGamePoker::UpdatePlayState(char direction, char state)
{
	m_playerinfo[direction].playerstate = state;
}

void CDlgGamePoker::RefreshPlayerDetails(void)
{
	int i;
	int index;
	ACString strText;
	m_pLst_Player->ResetContent();
	if (m_CurSel == -1)
	{
		m_CurSel = m_hostDir;
	}
	CDlgGamePlaza *pDlgGamePlaza = (CDlgGamePlaza*)GetGameUIMan()->GetDialog("Win_GamePoker_Hall");
	m_pTxt_Name->SetText(m_playerinfo[0].name);
	m_pTxt_Title->SetText(pDlgGamePlaza->GetTitle(m_playerinfo[0].score));
	int iTotalCount = m_playerinfo[0].win_count+m_playerinfo[0].lose_count+m_playerinfo[0].draw_count+m_playerinfo[0].escape_count;
	strText.Format(_AL("%d%%"), iTotalCount>0 ? (1000*m_playerinfo[0].win_count/iTotalCount+5)/10 : 0);
	m_pTxt_WinPercent->SetText(strText);
	GetGameUIMan()->Int64ToStr(m_playerinfo[0].score, strText);
	m_pTxt_Score->SetText(strText);

	for (i = 0; i < 3; i++)
	{
		index = (i + 4 - m_hostDir) % 3;
		m_pImg_PlayerHead[index]->Show(false);
		m_pImg_PlayerHeadF[index]->Show(false);
		if (m_playerinfo[i].roleid > 0)
		{
			if (m_playerinfo[i].gender == 0)
				m_pImg_PlayerHead[index]->Show(true);
			else
				m_pImg_PlayerHeadF[index]->Show(true);
			m_pLab_PlayerName[index]->SetText(m_playerinfo[i].name);
			ACString strScore;
			GetGameUIMan()->Int64ToStr(m_playerinfo[i].score, strScore);
			int iTotalCount = m_playerinfo[i].win_count+m_playerinfo[i].lose_count+m_playerinfo[i].draw_count+m_playerinfo[i].escape_count;
			strText.Format(_AL("%s\t%s\t%d%%\t%s"), m_playerinfo[i].name, pDlgGamePlaza->GetTitle(m_playerinfo[i].score), 
				iTotalCount>0 ? (1000*m_playerinfo[i].win_count/iTotalCount+5)/10 : 0,
			strScore);
			m_pLst_Player->AddString(strText);
			m_pLst_Player->SetItemData(m_pLst_Player->GetCount()-1, i);
			m_pLst_Player->SetItemData(m_pLst_Player->GetCount()-1, m_playerinfo[i].win_count, 1);
			m_pLst_Player->SetItemData(m_pLst_Player->GetCount()-1, m_playerinfo[i].lose_count, 2);
		}
		else
		{
			m_pLab_PlayerName[index]->SetText(_AL(""));	
		}
	}
	OnEventLButtonUp_List(0,0,NULL);
}

void CDlgGamePoker::SetPlayingState(int nextroleid, char state1, char state2)
{
	for (int i = 0; i < 3; i++)
	{
		if (m_playerinfo[i].roleid == nextroleid)
		{
			m_playerinfo[i].state = state1;
		}
		else
			m_playerinfo[i].state = state2;
	}
}

bool CDlgGamePoker::IsInRoom(void)
{
	return (m_RoomId > -1);
}