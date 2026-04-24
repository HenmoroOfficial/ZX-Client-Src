// Filename	: DlgSpread.h
// Creator	: Fuchonggang
// Date		: 2009/04/24

#pragma once

#include "hashmap.h"
#include "DlgBase.h"

#define MAXBONUS 3394875

class AUIListBox;
class AUILabel;


typedef	abase::hash_map<int, ACString>	PageDesc;

class CDlgSpread : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

	struct ReferralBrief
	{
		int index;
		int roleid;
		ACString rolename;
		int level;
		int sub_level;
		int bonus_total1;
		int bonus_total2;
		int bonus_left;
		int bonus_avail;
		INT64 exp_total1;
		INT64 exp_total2;
		INT64 exp_left;
	};

public:
	CDlgSpread();
	virtual ~CDlgSpread();

	void OnCommandPrev(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandGetExp(const char * szCommand);
	void OnCommandGetBonus(const char * szCommand);
	void OnCommandPaste(const char * szCommand);
	void OnCommandRefreshWord(const char * szCommand);
	void OnCommandAddFriend(const char * szCommand);
	void OnCommandRefresh(const char * szCommand);
	void OnCommandHelp(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

public:
	void ReferAction(void *pData);
	void UpdatePage();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	virtual void OnTick();

protected:
	int	 m_iItemIndex;
	int	 m_iItemId;
	INT64	m_iBonusAvail;
	INT64	m_iExpAvail;
	int		m_iCurPage;
	int		m_iMaxPage;
	int		m_iCurWordIndex;

	AUIListBox *m_pLstAll;
	AUILabel *m_pLabExpAvail;
	AUILabel *m_pLabBonusAvail;
};
