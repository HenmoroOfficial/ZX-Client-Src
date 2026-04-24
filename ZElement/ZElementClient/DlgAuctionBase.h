// Filename	: DlgAuctionBase.h
// Creator	: Xiao Zhou
// Date		: 2005/11/10


#pragma once

#include "DlgBase.h"
#include "hashmap.h"
#include "IOLib\\Gnoctets.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIDialog.h"
#include "AUI\\AUIEditBox.h"

typedef abase::hash_map<int,int> AuctionIDMap;
typedef abase::hash_map<int,ACString> AuctionNameMap;
typedef abase::hash_map<int,GNET::Octets> AuctionHintMap;

class CDlgAuctionBase : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgAuctionBase();
	virtual ~CDlgAuctionBase();

	virtual void OnCommandCancel(const char* szCommand);
	virtual void OnCommandAuctionBuyList(const char* szCommand);
	virtual void OnCommandAuctionList(const char* szCommand);
	virtual void OnCommandAuctionSellList(const char* szCommand);
	void OnCommandBuy(const char* szCommand);
	void OnCommandBinBuy(const char* szCommand);
	void OnCommandSortID(const char* szCommand);
	void OnCommandSortName(const char* szCommand);
	void OnCommandSortTime(const char* szCommand);
	void OnCommandSortPrice(const char* szCommand);
	void OnCommandSortBinPrice(const char* szCommand);

	void OnEventLButtonDown_Lst_Item(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	virtual void UpdateList(void *pData);
	void UpdateHint(DWORD auctionid, DWORD id, int expire_date, DWORD count);
	void BidRe(void *pData);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	PAUISTILLIMAGEBUTTON	m_pBtn_AuctionBuyList;
	PAUISTILLIMAGEBUTTON	m_pBtn_AuctionList;
	PAUISTILLIMAGEBUTTON	m_pBtn_AuctionSellList;
	PAUILISTBOX				m_pLst_Item;
	PAUIEDITBOX				m_pTxt_Price;
	bool					m_bSortID;
	bool					m_bSortName;
	bool					m_bSortTime;
	bool					m_bSortPrice;
	bool					m_bSortBinPrice;
};

