// Filename	: DlgFingerGuessingGame.h
// Creator	: Fu Chonggang
// Date		: 2013/7/15

#pragma once

#include "DlgBase.h"

class CDlgFGGame : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgFGGame();
	virtual ~CDlgFGGame();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
};

class CDlgFGGameGuess : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgFGGameGuess();
	virtual ~CDlgFGGameGuess();


	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
};

class CDlgFGGameResult : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgFGGameResult();
	virtual ~CDlgFGGameResult();

	void OnCommandAgain(const char * szCommand);

	void SetNumFix(int iNumFix) { m_iNumFix = iNumFix; }
	void SetNumGuessAdd(int iNumGuessAdd) { m_iNumGuess = m_iNumFix + iNumGuessAdd; }

protected:
	virtual void OnShowDialog();
	virtual void OnTick();

	int m_iNumFix;
	int m_iNumGuess;
	int m_iNumFixAI;
	int m_iNumGuessAI;

	static int ms_iAIWinCount;
	static const int ms_iNumMax;
};
