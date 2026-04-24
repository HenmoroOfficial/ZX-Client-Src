// File		: DlgArrangeTeamSetting.h
// Creator	: Fu Chonggang
// Date		: 2009/10/16

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUILabel.h"

class CDlgArrangeTeamSetting : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgArrangeTeamSetting();
	virtual ~CDlgArrangeTeamSetting();
	void OnCommandAdd(const char * szCommand);
	void OnCommandDeleteOne(const char * szCommand);
	void OnCommandDeleteAll(const char * szCommand);
	void OnCommandAutoAll(const char * szCommand);
	void OnCommandAutoGuild(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandCancel(const char * szCommand);

protected:
	virtual void DoDataExchange(bool bSave);
	virtual void OnShowDialog();
	
	AUIListBox *			m_pLstName;
	AUIEditBox *			m_pTxtName2;
	PAUICHECKBOX			m_pChkAll;
	PAUICHECKBOX			m_pChkFriend;
	PAUICHECKBOX			m_pChkGuildServer;
	PAUICHECKBOX			m_pChkFamily;
	PAUILABEL				m_pLabAll;
	PAUILABEL				m_pLabFriend;
	PAUILABEL				m_pLabGuildServer;
	PAUILABEL				m_pLabFamily;

	const static int		NAMECOUNT;
};
