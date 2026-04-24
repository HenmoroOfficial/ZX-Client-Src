/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:50
	file name:	DlgSettingGame.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgSetting.h"
#include "EC_Configs.h"

class AUILabel;
class AUICheckBox;
class AUIStillImageButton;
class AUIComboBox;

class CDlgSettingGame : public CDlgSetting  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
protected:

	// member used for DDX
	int m_nChanel;
	bool m_bFilterLocalChat;
	int  m_nLocalChatLevel;
	bool m_bFilterWhisper;
	int  m_nWhisperLevel;
	int  m_nFont;
	int  m_nTransparent;

	AUIComboBox* m_pCmbFilter;

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	
	void DoDataExchange(bool bSave);
	
public:
	void OnLButtonUpChat(WPARAM, LPARAM, AUIObject *);
	void OnCommandSet(const char *szCommand);
	void OnCommand_LocalChatSet(const char *szCommand);

	CDlgSettingGame();
	virtual ~CDlgSettingGame();

	virtual void SetToDefault();
	virtual void Apply();
	virtual void UpdateView();
};

