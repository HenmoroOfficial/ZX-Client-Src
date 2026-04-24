// Filename	:	DlgSettingOtherChat.h
// Creator	:	Fu Chonggang
// Date		:	Sep 11, 2009

#pragma once

#include "DlgSetting.h"
#include "EC_Configs.h"

class AUISubDialog;

class CDlgSettingOtherChat : public CDlgSetting  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	CDlgSettingOtherChat();
	virtual ~CDlgSettingOtherChat();
	
	void UpdateView();
	void SetToDefault();
	void Apply();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void DoDataExchange(bool bSave);

public:
	void OnLButtonUpChat(WPARAM, LPARAM, AUIObject *);
	void OnCommandSet(const char *szCommand);

protected:
	int m_nChanel;
};

