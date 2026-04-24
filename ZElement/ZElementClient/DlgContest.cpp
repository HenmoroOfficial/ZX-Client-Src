// File		: DlgContest.cpp
// Creator	: Xiao Zhou
// Date		: 2008/5/6

#include "DlgContest.h"
#include "DlgInfo.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "contestquestion.hpp"
#include "ContestPlaceNotify.hpp"
#include "contestanswer_re.hpp"
#include "contestbegin.hpp"
#include "contestend.hpp"
#include "contestinvite.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgContest, CDlgBase)

AUI_ON_COMMAND("answer*",		OnCommandAnswer)
AUI_ON_COMMAND("mini",			OnCommandMini)
AUI_ON_COMMAND("main",			OnCommandMain)
AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgContest, CDlgBase)

AUI_ON_EVENT("Img_LuckStar*",	WM_LBUTTONDOWN, OnEventLButtonDownStar)

AUI_END_EVENT_MAP()

CDlgContest::CDlgContest()
{
}

CDlgContest::~CDlgContest()
{
}

void CDlgContest::OnCommandAnswer(const char *szCommand)
{
	int answer = atoi(szCommand + strlen("answer")) - 1;
	GetGameSession()->contest_answer(GetData(), 1 << answer, m_bUseStar ? 1 : 0);
	if( m_bUseStar )
	{
		m_bUseStar = false;
		if( m_nStar > 0 )
			m_nStar--;
	}
	m_bAnswered = true;
	m_bAnswer_Replyed = false;
	for(int i = 0; i < CDLGCONTEST_ANWSER_NUMBER; i++)
		m_pChk_Answer[i]->Enable(false);
}

void CDlgContest::OnCommandMini(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->GetDialog("Win_ContestMini")->Show(true);
}

void CDlgContest::OnCommandMain(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->m_pDlgContest->Show(true);
}

void CDlgContest::OnCommandCancel(const char *szCommand)
{
	if (0 == stricmp(m_szName, "Win_Contest"))
	{
		if( !m_bEnd )
			GetGameUIMan()->MessageBox("Game_ContestExit", GetStringFromTable(1055), MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160));
		else
			Show(false);
	}
}

void CDlgContest::OnEventLButtonDownStar(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	if( !m_bAnswered && m_pChk_Answer[0]->IsEnabled() )
		m_bUseStar = true;
}

bool CDlgContest::OnInitDialog()
{
	if (0 == stricmp(m_szName, "Win_Contest"))
	{
		m_pImg_Result = (PAUIIMAGEPICTURE)GetDlgItem("Img_Result");
		m_pImg_Status = (PAUIIMAGEPICTURE)GetDlgItem("Img_Status");
		m_pLst_Place = (PAUILISTBOX)GetDlgItem("Lst_Place");
		m_pTxt_Question = (PAUILABEL)GetDlgItem("Txt_Question");
		m_pTxt_Time = (PAUILABEL)GetDlgItem("Lab_Time");
		m_pTxt_Score = (PAUILABEL)GetDlgItem("Txt_Score");
		m_pTxt_Hit = (PAUILABEL)GetDlgItem("Txt_Hit");
		m_pTxt_CorrectNum = (PAUILABEL)GetDlgItem("Txt_CorrectNum");

		int i;
		char szName[20];
		for (i = 0; i < CDLGCONTEST_ANWSER_NUMBER; ++i)
		{
			sprintf(szName, "Txt_Answer%d", i + 1);
			m_pTxt_Answer[i] = (PAUILABEL)GetDlgItem(szName);
			sprintf(szName, "Chk_Answer%d", i + 1);
			m_pChk_Answer[i] = (PAUICHECKBOX)GetDlgItem(szName);
			sprintf(szName, "Img_Right%d", i + 1);
			m_pImg_Right[i] = (PAUIIMAGEPICTURE)GetDlgItem(szName);
		}

		for (i = 0; i < CDLGCONTEST_STAR_NUMBER; ++i)
		{
			sprintf(szName, "Img_LuckStar%d", i + 1);
			m_pImg_Star[i] =  (PAUIIMAGEPICTURE)GetDlgItem(szName);
			m_pImg_Star[i]->FixFrame(1);
		}
	}

	m_dwStartTick = 0;
	m_bEnd = false;

	return true;
}

void CDlgContest::OnShowDialog()
{
	if (0 != stricmp(m_szName, "Win_Contest"))
		return;

	if (m_dwStartTick != 0) // Is Answering Question..
		return;

	// Init
	m_pTxt_Question->SetText(_AL(""));

	for(int i = 0; i < CDLGCONTEST_ANWSER_NUMBER; i++)
	{
		m_pChk_Answer[i]->Check(false);
		m_pChk_Answer[i]->Enable(false);
		m_pChk_Answer[i]->Show(false);
		m_pImg_Right[i]->Show(false);

		m_pTxt_Answer[i]->SetText(_AL(""));
	}
	m_dwStartTick = 0;
	m_pLst_Place->ResetContent();
	m_pTxt_Score->SetText(_AL("0"));
	m_pTxt_Hit->SetText(_AL("0"));
	m_pTxt_CorrectNum->SetText(_AL("0"));
	m_bAnswered = true;
	m_pImg_Result->Show(false);
	m_pImg_Status->Show(true);
	m_pImg_Status->FixFrame(0);
}

void CDlgContest::OnTick()
{
	if (0 == stricmp(m_szName, "Win_ContestMini"))
	{
		DWORD dwStartTick = GetGameUIMan()->m_pDlgContest->m_dwStartTick;
		int nRestTick = int(dwStartTick + 20000 - GetTickCount());
		GetDlgItem("Btn_ContestMain")->SetFlash(dwStartTick != 0 && nRestTick > 0);
	}

	if (0 != stricmp(m_szName, "Win_Contest"))
		return;

	for(int i = 0; i < CDLGCONTEST_STAR_NUMBER; i++)
	{
		m_pImg_Star[i]->Show(i < m_nStar);
		if( i == m_nStar - 1 && m_bUseStar )
		{
			m_pImg_Star[i]->Show(GetTickCount() / 300 % 2 == 1);
			m_pImg_Star[i]->FixFrame(0);
		}
		m_pImg_Star[i]->FixFrame(1);
	}
	ACString strText;
	if( m_dwStartTick == 0 )
	{
		m_pTxt_Time->SetText(_AL(""));
	}
	else
	{
		int nRestTick = int(m_dwStartTick + 20000 - GetTickCount());
		if( nRestTick > 0 )
		{
			strText.Format(GetStringFromTable(1053), nRestTick / 1000 + 1);
			m_pTxt_Time->SetText(strText);
			if( !m_bAnswered )
			{
				int i;
				for(i = 0; i < CDLGCONTEST_ANWSER_NUMBER; i++)
					m_pChk_Answer[i]->Enable(true);
				for(i = 0; i < CDLGCONTEST_STAR_NUMBER; i++)
					m_pImg_Star[i]->FixFrame(0);
			}
		}
		else
		{
			m_bUseStar = false;
			for(int i = 0; i < CDLGCONTEST_ANWSER_NUMBER; i++)
				m_pChk_Answer[i]->Enable(false);
			nRestTick += 5000;
			if( nRestTick > 0 )
			{
				strText.Format(GetStringFromTable(1054), nRestTick / 1000 + 1);
				m_pTxt_Time->SetText(strText);
			}
			else
				m_pTxt_Time->SetText(_AL(""));
			if( m_bAnswered && m_bAnswer_Replyed )
			{
				m_pImg_Result->Show(true);
				m_pImg_Result->FixFrame(m_pImg_Result->GetData() == 1 ? 1 : 0);
				ACString strText;
				strText.Format(_AL("%d"), m_pTxt_Score->GetData());
				m_pTxt_Score->SetText(strText);
				strText.Format(_AL("%d"), m_pTxt_Hit->GetData());
				m_pTxt_Hit->SetText(strText);
				strText.Format(_AL("%d"), m_pTxt_CorrectNum->GetData());
				m_pTxt_CorrectNum->SetText(strText);
			}
		}
	}
}

int CDlgContest::OnPrtc_ContestAnswerRe(GNET::Protocol* pProtocol)
{
	GNET::ContestAnswer_Re* p = (GNET::ContestAnswer_Re*)(pProtocol);
	m_nStar = p->star_amount;
	m_bUseStar = false;
	m_bAnswer_Replyed = true;
	m_pImg_Result->SetData(p->result);
	m_pTxt_Score->SetData(p->sum);
	m_pTxt_Hit->SetData(p->series_right);
	m_pTxt_CorrectNum->SetData(p->right_amount);

	if (p->answer&0x1)
	{
		m_pImg_Right[0]->Show(true);
	}
	if (p->answer&0x2)
	{
		m_pImg_Right[1]->Show(true);
	}
	if (p->answer&0x4)
	{
		m_pImg_Right[2]->Show(true);
	}
	if (p->answer&0x8)
	{
		m_pImg_Right[3]->Show(true);
	}

	return 1;
}

int CDlgContest::OnPrtc_ContestBegin(GNET::Protocol* pProtocol)
{
	GNET::ContestBegin* p = (GNET::ContestBegin*)(pProtocol);
	Show(true);
	m_nStar = p->star_amount;
	m_bUseStar = false;
	ACString strText;
	strText.Format(_AL("%d"), p->score);
	m_pTxt_Score->SetText(strText);
	strText.Format(_AL("%d"), 0);
	m_pTxt_Hit->SetText(strText);
	strText.Format(_AL("%d"), p->right_amount);
	m_pTxt_CorrectNum->SetText(strText);
	m_bEnd = false;

	return 1;
}

int CDlgContest::OnPrtc_ContestEnd(GNET::Protocol* pProtocol)
{
	m_bEnd = true;
	m_pImg_Result->Show(false);
	m_pImg_Status->Show(true);
	m_pImg_Status->FixFrame(1);

	return 1;
}

int CDlgContest::OnPrtc_ContestInvite(GNET::Protocol* pProtocol)
{
	Show(false);
	m_dwStartTick = 0;
	GNET::ContestInvite* p = (GNET::ContestInvite*)(pProtocol);
	GetGameUIMan()->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_ContestInvite", GetStringFromTable(1052), max(60000, p->remain_time * 1000), 0, 0, 0);

	return 1;
}

int CDlgContest::OnPrtc_ContestQuestion(GNET::Protocol* pProtocol)
{
	GNET::ContestQuestion* p = (GNET::ContestQuestion*)pProtocol;

	ACString strQuestion;
	strQuestion.Format(_AL("%d. %s"), p->questionid + 1, ACString((const ACHAR*)p->question.begin(), p->question.size() / sizeof (ACHAR)));
	GetDlgItem("Txt_Question")->SetText(strQuestion);

	for(int i = 0; i < CDLGCONTEST_ANWSER_NUMBER; i++)
	{
		m_pChk_Answer[i]->Check(false);
		m_pChk_Answer[i]->Enable(false);
		m_pImg_Right[i]->Show(false);
		if( i < (int)p->answerlist.size() ) {
			m_pTxt_Answer[i]->SetText(ACString((const ACHAR*)p->answerlist[i].begin(), p->answerlist[i].size() / sizeof(ACHAR)));
			m_pChk_Answer[i]->Show(true);
		}
		else {
			m_pTxt_Answer[i]->SetText(_AL(""));
			m_pChk_Answer[i]->Show(false);
		}
	}
	m_dwStartTick = GetTickCount();
	SetData(p->questionid);
	m_bAnswered = false;
	m_pImg_Result->Show(false);
	m_pImg_Status->Show(false);

	return 1;
}

int CDlgContest::OnPrtc_NotifyContestPlace(GNET::Protocol* pProtocol)
{
	GNET::ContestPlaceNotify* p = (GNET::ContestPlaceNotify*)pProtocol;

	m_pLst_Place->ResetContent();
	
	for(DWORD i = 0; i < p->placelist.size(); i++)
	{
		ACString strName((const ACHAR*)p->placelist[i].rolename.begin(), p->placelist[i].rolename.size() / sizeof(ACHAR));
		ACString strText;
		strText.Format(_AL("%s\t%d"), strName, p->placelist[i].score);
		m_pLst_Place->AddString(strText);
	}

	return 1;
}
