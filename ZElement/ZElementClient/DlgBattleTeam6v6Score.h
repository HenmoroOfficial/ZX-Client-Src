// Filename	: DlgBAttleTeam6v6Score.h
// Creator	: Fu Chonggang
// Date		: 2012.8.8

#pragma once

#include "DlgBase.h"

class AUILabel;
class AUIListBox;

class CDlgBattleTeam6v6Score : public CDlgBase  
{
public:
	CDlgBattleTeam6v6Score();
	virtual ~CDlgBattleTeam6v6Score();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	AUILabel *m_pLabTime;
};

class CDlgBattleTeam6v6Record : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgBattleTeam6v6Record();
	virtual ~CDlgBattleTeam6v6Record();
		
	void OnCommandLeave(const char * szCommand);
	void OnCommandSortByKill(const char * szCommand);
	void OnCommandSortByDead(const char * szCommand);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	AUILabel *m_pLabTime;
	AUIListBox *m_pLstRecord;
};
