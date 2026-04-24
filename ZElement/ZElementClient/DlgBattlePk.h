// File		: DlgBattlePk.h
// Creator	: Fuchonggang
// Date		: 2009/04/28

#pragma once

#include "DlgBase.h"

class AUITextArea;
class AUIStillImageButton;
class AUILabel;
class CDlgBattlePk : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBattlePk();
	virtual ~CDlgBattlePk();

	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventLButtonDownBattle(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void UpdatePkBattleInfo(void *pData);
	void UpdatePkBattleList(void *pData);
	void UpdatePkBattleScore(void *pData);

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

private:
	static int			m_iMapId;
	int			m_iEnterTime;

	AUIListBox	*m_pLstInfo;
	AUILabel	*m_pLabKill;
	AUILabel	*m_pLabKillScore;
	AUILabel	*m_pLabNumLeave;
	AUILabel	*m_pLabTitle;
	static AUIStillImageButton *m_pBtnJoinEnter;
	static AUILabel	*m_pLabTime;
};
