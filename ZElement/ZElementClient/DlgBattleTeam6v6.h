// Filename	: DlgBattleTeam6v6.h
// Creator	: Fu Chonggang
// Date		: 2012.7.30
// Desc		: 6v6副本各种小界面：最小化、报名。。。

#pragma once

#include "DlgTab3D.h"
#include "vector.h"

/************************************************************************
CDlgBattleTeam6v6ApplyMin
************************************************************************/
class CDlgBattleTeam6v6ApplyMin : public CDlgBase 
{
	AUI_DECLARE_COMMAND_MAP();	
public:
	CDlgBattleTeam6v6ApplyMin() {}
	virtual ~CDlgBattleTeam6v6ApplyMin() {}

	void OnCommandOpen(const char * szCommand);
	void OnCommandQuit(const char * szCommand);

};

class AUIObject;

/************************************************************************
CDlgBattleTeam6v6Apply
************************************************************************/
class CDlgBattleTeam6v6Apply : public CDlgTab3D  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgBattleTeam6v6Apply();
	virtual ~CDlgBattleTeam6v6Apply();
	
	void OnCommandOpenSelf(const char * szCommand);
	void OnCommandOpenTeam(const char * szCommand);
	void OnCommandAward(const char * szCommand);
	void OnCommandQuit(const char * szCommand);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	virtual void AddTextrures();
	virtual void OnSelOneItem();	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	bool HostSignUpBattle();
	bool HostInBattle();

	AUIObject	*m_pObjSelf;
	AUIObject	*m_pObjTeam;
	abase::vector<int>	m_vecTmplIds;
};

