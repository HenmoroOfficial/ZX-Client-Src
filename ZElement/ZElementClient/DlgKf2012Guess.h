// Filename	: CDlgKf2012Guess.h
// Creator	: Fu Chonggang
// Date		: 2012 ¿ç·þpk¾º²Â½çÃæ

#pragma once

#include "DlgBase.h"
#include "DlgTabContainer.h"

struct PK2012_GUESS_CONFIG;

class CDlgKf2012GuessBase : public CDlgBase, public CDlgTabContainer<CDlgKf2012GuessBase*>
{
public:
	CDlgKf2012GuessBase() {}
	virtual ~CDlgKf2012GuessBase() {}

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	static PK2012_GUESS_CONFIG* m_pConfig;
};


class CDlgKf2012GuessChampion : public CDlgKf2012GuessBase
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKf2012GuessChampion() {}
	virtual ~CDlgKf2012GuessChampion() {}

	void OnCommandRdoType(const char * szCommand);
	void OnCommandGuess(const char * szCommand);
	void OnCommandGetPackage(const char * szCommand);
	void OnCommandGetAward(const char * szCommand);
	void OnCommandLink(const char * szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	bool OnInitDialog();

public:
	static const int	ms_iMaxBetNum;

};


class CDlgKf2012GuessTop3 : public CDlgKf2012GuessBase
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKf2012GuessTop3() {}
	virtual ~CDlgKf2012GuessTop3() {}
	
	void OnCommandRdoType(const char * szCommand);
	void OnCommandGuess(const char * szCommand);
	void OnCommandGetBaseAward(const char * szCommand);
	void OnCommandGetAward(const char * szCommand);
	void OnCommandLink(const char * szCommand);
	
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	bool OnInitDialog();
	
};

class CDlgKf2012GuessChampionInput : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKf2012GuessChampionInput() {}
	~CDlgKf2012GuessChampionInput() {}
	
	void OnCommandAdd(const char * szCommand);
	void OnCommandSub(const char * szCommand);
	void OnCommandGuess(const char * szCommand);
protected:
	bool OnInitDialog();
	void OnTick();
	PK2012_GUESS_CONFIG* m_pConfig;
};


class CDlgKf2012GuessTop3Input : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgKf2012GuessTop3Input() {}
	~CDlgKf2012GuessTop3Input() {}
	
	void OnCommandGuess(const char * szCommand);
};