// File		: DlgArrangeTeam.h
// Creator	: Xiao Zhou
// Date		: 2005/8/17

#pragma once

//#include "DlgBase.h"
#include "DlgArrangeTeamSetting.h"
//#include "AUI\\AUIEditBox.h"
//#include "AUI\\AUIListBox.h"
//#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgArrangeTeam : public CDlgArrangeTeamSetting  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgArrangeTeam();
	virtual ~CDlgArrangeTeam();

	void OnCommandKick(const char * szCommand);
	void OnCommandInvite(const char * szCommand);
	void OnCommandLeave(const char * szCommand);
	void OnCommandRandom(const char * szCommand);
	void OnCommandSlogan(const char * szCommand);
	void OnCommandShowSelf(const char * szCommand);
	void OnCommandLfg(const char * szCommand);
	void OnCommandMakeLeader(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandAuto(const char * szCommand);
	void OnCommandAutoSetting(const char * szCommand);

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventRButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	bool UpdateTeam(bool bUpdateNear = false);
	
protected:
	virtual void DoDataExchange(bool bSave);
	virtual void OnShowDialog();
	
	AUIListBox *			m_pLstCurrentTeamMate;
	AUIListBox *			m_pLstNearbypp;
	AUIEditBox *			m_pTxtName;
	AUIEditBox *			m_pTxtGroupMsg;
	AUICheckBox *			m_pChkSlogan;
	AUICheckBox *			m_pChkShowself;
	AUIRadioButton *		m_pRdoLfgroup;
	AUIRadioButton *		m_pRdoLfmember;
	AUIRadioButton *		m_pRdoRandom;
	AUIRadioButton *		m_pRdoFree;
	AUIStillImageButton *	m_pBtnLeave;
	AUIStillImageButton *	m_pBtnKick;
	AUIStillImageButton *	m_pBtnInvite;
	AUIStillImageButton *	m_pBtnDisband;

private:
	int m_nPickupMode;
};
