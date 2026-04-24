// File		: DlgFriendHistory.h
// Creator	: Xiao Zhou
// Date		: 2005/8/18

#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUITreeView.h"

class CDlgFriendHistory : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgFriendHistory();
	virtual ~CDlgFriendHistory();
	
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void BuildChatHistory(int idPlayer);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void DoDataExchange(bool bSave);
	
	AUITextArea *	m_pTxtContent;
	AUIListBox *	m_pListMessage;
	AUITreeView *	m_pTvFriendList;
	P_AUITREEVIEW_ITEM m_pItemSelected;
	abase::vector<ACString> m_vecMsg;
};
