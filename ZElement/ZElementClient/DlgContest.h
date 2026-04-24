// File		: DlgContest.h
// Creator	: Xiao Zhou
// Date		: 2008/5/6

#pragma once

#include "DlgBase.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIImagePicture.h"

#define CDLGCONTEST_ANWSER_NUMBER	4
#define CDLGCONTEST_STAR_NUMBER		3

namespace GNET
{
	class Protocol;
}

class CDlgContest : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgContest();
	virtual ~CDlgContest();

	void OnCommandAnswer(const char * szCommand);
	void OnCommandMini(const char * szCommand);
	void OnCommandMain(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDownStar(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	int OnPrtc_ContestAnswerRe(GNET::Protocol* pProtocol);
	int OnPrtc_ContestBegin(GNET::Protocol* pProtocol);
	int OnPrtc_ContestEnd(GNET::Protocol* pProtocol);
	int OnPrtc_ContestInvite(GNET::Protocol* pProtocol);
	int OnPrtc_ContestQuestion(GNET::Protocol* pProtocol);
	int OnPrtc_NotifyContestPlace(GNET::Protocol* pProtocol);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();

	PAUIIMAGEPICTURE	m_pImg_Result;
	PAUIIMAGEPICTURE	m_pImg_Status;
	PAUILISTBOX			m_pLst_Place;
	PAUILABEL			m_pTxt_Question;
	PAUILABEL			m_pTxt_Time;
	PAUILABEL			m_pTxt_Score;
	PAUILABEL			m_pTxt_Hit;
	PAUILABEL			m_pTxt_CorrectNum;
	PAUILABEL			m_pTxt_Answer[CDLGCONTEST_ANWSER_NUMBER];
	PAUICHECKBOX		m_pChk_Answer[CDLGCONTEST_ANWSER_NUMBER];
	PAUIIMAGEPICTURE	m_pImg_Right[CDLGCONTEST_ANWSER_NUMBER];
	PAUIIMAGEPICTURE	m_pImg_Star[CDLGCONTEST_STAR_NUMBER];
	DWORD				m_dwStartTick;
	bool				m_bAnswered;
	bool				m_bAnswer_Replyed;
	bool				m_bUseStar;
	bool				m_bEnd;
	int					m_nStar;
};
