// Filename	: DlgAuctionSearch.h
// Creator	: Xiao Zhou
// Date		: 2005/11/10


#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUIStillImageButton.h"

class AFileImage;
class elementdataman;

class CDlgAuctionSearch : public CDlgBase
{
	friend class CDlgAuctionBuyList;

	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgAuctionSearch();
	virtual ~CDlgAuctionSearch();

	void OnCommandSearchAll(const char* szCommand);
	void OnCommandSearchName(const char* szCommand);
	void OnCommandSearchItem(const char* szCommand);
	
	void OnEventLButtonDown_Tv_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_Lst_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDBLCLK_Tv_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	PAUITREEVIEW			m_pTv_Item;
	PAUILISTBOX				m_pLst_Item;
	PAUIEDITBOX				m_pTxt_Name;
	PAUISTILLIMAGEBUTTON	m_pBtn_SearchItem;
};

