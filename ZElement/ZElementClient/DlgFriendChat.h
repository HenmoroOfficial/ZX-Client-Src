// File		: DlgFriendChat.h
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgFriendChat : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
public:
	enum FriendChatHotKey
	{
		FCHK_CTRLENTER	= 0,
		FCHK_SHIFTENTER	= 1,
	};
public:
	void OnCommandCANCAL(const char *szCommand);
	void OnCommandSend(const char *szCommand);
	void OnCommandPopface(const char *szCommand);
	void OnCommandHistory(const char *szCommand);
	void OnCommandSave(const char *szCommand);
	void OnCommandDelall(const char *szCommand);
	void OnCommandMinimize(const char *szCommand);
	void OnCommandHotKey(const char *szCommand);

	void OnEventSysChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventKeyDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventChar(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void SetFriendName(const ACHAR * strName);
	ACString GetFriendName() { return m_strToWho;}

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
	CDlgFriendChat();
	virtual ~CDlgFriendChat();

	static int	GetHotKey() { return m_iHotkey; }
	static void SetHotKey(int k) { m_iHotkey = k; }
	
protected:
	virtual void DoDataExchange(bool bSave);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();
	
	AUIEditBox *	m_pTxtContent;
	AUITextArea *	m_pTxtChat;
	AUICheckBox *	m_pChkSave;
	AUIStillImageButton *	m_pBtnSend;
	ACString		m_strToWho;

	static int		m_iHotkey;
};

class CDlgFriendChatHotKey : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:
	void OnCommandCANCAL(const char *szCommand);
	
	CDlgFriendChatHotKey(){}
	virtual ~CDlgFriendChatHotKey(){}
};