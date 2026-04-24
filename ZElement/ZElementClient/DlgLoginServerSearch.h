// Filename	: DlgLoginServerSearch.h
// Creator	: Xiao Zhou
// Date		: 2005/11/4


#pragma once

#include "AUI\\AUIDialog.h"
#include "AUI\\AUITextArea.h"

class DlgLoginServerSearch : public AUIDialog  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	DlgLoginServerSearch();
	virtual ~DlgLoginServerSearch();

	void OnCommandSearch(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	void OnEventLButtonDown_Txt_Info(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMove_Txt_Info(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	bool OnItem() { return m_bOnItem;}
	void SetInfo(ACString strInfo);
protected:
	void GetItemLinkItemOn(int x, int y, PAUIOBJECT pObj, AUITEXTAREA_EDITBOX_ITEM * pLink);
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

	bool m_bOnItem;
	PAUITEXTAREA m_pTxtInfo;
	abase::vector<ACString> m_vecGroups;
	abase::vector<ACString> m_vecServers;
};

