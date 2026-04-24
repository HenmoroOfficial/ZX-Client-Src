// Filename	: DlgSpeakerSpecial.h
// Creator	: Fu Chonggang
// Date		: 2012.6.5

#pragma once

#include "DlgBase.h"

class AUIEditBox;

class CDlgSpeakerSpecial : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP()

public:

	CDlgSpeakerSpecial();
	virtual ~CDlgSpeakerSpecial();
	
	void OnCommandPopface(const char *szCommand);
	void OnCommandSend(const char * szCommand);
	void OnCommandHotKey(const char *szCommand);
	void OnCommandCancel(const char * szCommand);

	void OnEventSysChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	
	AUIEditBox	*m_pTxtContent;
	
	int		m_iChannel;
};
