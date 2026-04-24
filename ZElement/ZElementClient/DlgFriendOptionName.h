// File		: DlgFriendOptionName.h
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#pragma once

#include "DlgBase.h"

class AUIListBox;

class CDlgFriendOptionName : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgFriendOptionName();
	virtual ~CDlgFriendOptionName();
	virtual void OnTick();

	void OnCommandCANCAL(const char *szCommand);
	void OnCommandWhisper(const char *szCommand);
	void OnCommandInvite(const char *szCommand);
	void OnCommandChat(const char *szCommand);
	void OnCommandHistory(const char *szCommand);
	void OnCommandUpdate(const char *szCommand);
	void OnCommandMoveGroup(const char *szCommand);
	void OnCommandDeleteFriend(const char *szCommand);
	void OnCommandInviteChannel(const char *szCommand);
	void OnCommandJoinChannel(const char *szCommand);
	void OnCommandCopyName(const char *szCommand);
	void OnCommandGarden(const char *szCommand);
	void OnCommandRose(const char *szCommand);
	void OnCommandAddRemark(const char *szCommand);
};

class CDlgFriendMoveGroup : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgFriendMoveGroup(){}
	virtual ~CDlgFriendMoveGroup(){}
protected:
	virtual bool OnInitDialog();
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void   OnEventLButtonDown_GroupList(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
private:
	AUIListBox*    m_ListGroup;
};
