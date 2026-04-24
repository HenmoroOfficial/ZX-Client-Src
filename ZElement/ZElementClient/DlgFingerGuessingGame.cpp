// Filename	: DlgFingerGuessingGame.cpp
// Creator	: Fu Chonggang
// Date		: 2013/7/15

#include "AFI.h"
#include "AUI\\AUIImagePicture.h"
#include "EC_GameUIMan.h"
#include "DlgFingerGuessingGame.h"
#include "EC_Game.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW


AUI_BEGIN_EVENT_MAP(CDlgFGGame, CDlgBase)

AUI_ON_EVENT("Img_Self*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgFGGame::CDlgFGGame()
{
}

CDlgFGGame::~CDlgFGGame()
{
}

bool CDlgFGGame::OnInitDialog()
{
	AString strName;
	PAUIIMAGEPICTURE pImg;
	int i;
	for (i=0; i<6; ++i)
	{
		strName.Format("Img_System%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i);
		strName.Format("Img_Self%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i);
	}
	return true;
}

void CDlgFGGame::OnTick()
{

}

void CDlgFGGame::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int iIndex = atoi(pObj->GetName()+strlen("Img_Self"))-1;
	Show(false);
	CDlgFGGameResult* pDlg = (CDlgFGGameResult*)GetGameUIMan()->GetDialog("Win_Game_Guess_Result");
	pDlg->SetNumFix(iIndex);
	GetGameUIMan()->GetDialog("Win_Game_Guess")->Show(true);
	GetGameUIMan()->GetDialog("Win_Game_Guess")->SetPosEx(GetPos().x, GetPos().y);

	ACString strText;
	strText.Format(_AL("give:%d"), iIndex);
	GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
}


AUI_BEGIN_EVENT_MAP(CDlgFGGameGuess, CDlgBase)

AUI_ON_EVENT("Img_Self*",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

CDlgFGGameGuess::CDlgFGGameGuess()
{
}

CDlgFGGameGuess::~CDlgFGGameGuess()
{
}

bool CDlgFGGameGuess::OnInitDialog()
{
	AString strName;
	PAUIIMAGEPICTURE pImg;
	int i;
	for (i=0; i<6; ++i)
	{
		strName.Format("Img_System%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i);
		strName.Format("Img_Self%02d", i+1);
		pImg = (PAUIIMAGEPICTURE)GetDlgItem(strName);
		pImg->FixFrame(i);
	}
	return true;
}

void CDlgFGGameGuess::OnTick()
{

}

void CDlgFGGameGuess::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int iIndex = atoi(pObj->GetName()+strlen("Img_Self"))-1;

	ACString strText;
	strText.Format(_AL("guess add :%d"), iIndex);
	GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);

	Show(false);
	CDlgFGGameResult* pDlg = (CDlgFGGameResult*)GetGameUIMan()->GetDialog("Win_Game_Guess_Result");
	pDlg->SetNumGuessAdd(iIndex);
	pDlg->SetPosEx(GetPos().x, GetPos().y);
	pDlg->Show(true);
}



AUI_BEGIN_COMMAND_MAP(CDlgFGGameResult, CDlgBase)

AUI_ON_COMMAND("Btn_Again",		OnCommandAgain)

AUI_END_COMMAND_MAP()

int CDlgFGGameResult::ms_iAIWinCount = 0;
const int CDlgFGGameResult::ms_iNumMax = 5;

CDlgFGGameResult::CDlgFGGameResult()
{
}

CDlgFGGameResult::~CDlgFGGameResult()
{
}

void CDlgFGGameResult::OnShowDialog()
{
	if (m_szName!="Win_Game_Guess_Result")
	{
		return;
	}
	m_iNumFixAI = a_Random(0, ms_iNumMax);
	int iNumResult = m_iNumFix + m_iNumFixAI;
	if (ms_iAIWinCount>=3)	// 系统赢3次以后不再赢
	{
		m_iNumGuessAI = a_Random(m_iNumFixAI, m_iNumFixAI+ms_iNumMax-1);
		if (m_iNumGuessAI>=iNumResult)
			m_iNumGuessAI++;
	}
	else
		m_iNumGuessAI = a_Random(m_iNumFixAI, m_iNumFixAI+ms_iNumMax);

	ACString strText;
	strText.Format(_AL("ai give :%d"), m_iNumFixAI);
	GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);
	strText.Format(_AL("ai guess :%d"), m_iNumGuessAI);
	GetGameUIMan()->AddChatMessage(strText, GP_CHAT_MISC);

	((PAUIIMAGEPICTURE)GetDlgItem("Img_ResultSystem01"))->FixFrame(m_iNumFixAI);
	((PAUIIMAGEPICTURE)GetDlgItem("Img_ResultSystem02"))->FixFrame(m_iNumGuessAI);
	((PAUIIMAGEPICTURE)GetDlgItem("Img_ResultSelf01"))->FixFrame(m_iNumFix);
	((PAUIIMAGEPICTURE)GetDlgItem("Img_ResultSelf02"))->FixFrame(m_iNumGuess);
	((PAUIIMAGEPICTURE)GetDlgItem("Img_Result01"))->FixFrame(m_iNumFixAI+m_iNumFix);

	PAUIDIALOG pDlg;
	if ( m_iNumGuess == iNumResult && m_iNumGuessAI != iNumResult )
		pDlg = GetGameUIMan()->GetDialog("Win_Game_Guess_WinGfx");
	else if ( m_iNumGuess != iNumResult && m_iNumGuessAI == iNumResult )
	{
		pDlg = GetGameUIMan()->GetDialog("Win_Game_Guess_LoseGfx");
		ms_iAIWinCount++;
	}
	else
		pDlg = GetGameUIMan()->GetDialog("Win_Game_Guess_DeuceGfx");
	pDlg->Show(true);
	pDlg->AlignTo(this,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER);
}

void CDlgFGGameResult::OnTick()
{
}

void CDlgFGGameResult::OnCommandAgain(const char * szCommand)
{
	if ( m_szName=="Win_Game_Guess_LoseGfx" || m_szName=="Win_Game_Guess_DeuceGfx" )
	{
		GetGameUIMan()->GetDialog("Win_Game_Guess_Start")->Show(true);
	}
	else
		GetHostPlayer()->FinishTinyGameTask(3001);

	GetGameUIMan()->GetDialog("Win_Game_Guess_Result")->Show(false);
	Show(false);
}