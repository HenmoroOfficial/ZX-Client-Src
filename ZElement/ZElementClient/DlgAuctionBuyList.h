// Filename	: DlgAuctionBuyList.h
// Creator	: Xiao Zhou
// Date		: 2005/11/10


#pragma once

#include "DlgAuctionBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUIStillImageButton.h"

class AFileImage;
class elementdataman;

class CDlgAuctionBuyList : public CDlgAuctionBase  
{
	friend class CDlgAuctionSellList;
	friend class CDlgAuctionList;
	friend class CDlgAuctionBase;
	friend class CDlgAuctionSearch;
	friend class CDlgNPC;

	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgAuctionBuyList();
	virtual ~CDlgAuctionBuyList();

	void OnCommandSearchID(const char* szCommand);
	void OnCommandSearchMode(const char* szCommand);
	void OnCommandNext(const char* szCommand);
	void OnCommandLast(const char* szCommand);
	void OnCommandRefresh(const char* szCommand);
	
	void UpdateItem(void *pData);
	virtual void UpdateList(void *pData);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	
	PAUIEDITBOX				m_pTxt_AuctionID;
	PAUIEDITBOX				m_pTxt_PackGold;
	PAUILABEL				m_pTxt_RefreshStatus;
	PAUILABEL				m_pTxt_Search;
	AuctionIDMap			m_mapAuctionID;
	AuctionNameMap			m_mapAuctionName;
	AuctionHintMap			m_mapAuctionHint;
	int						m_nSearchID;
	DWORD					m_dwLastListTime;
	int						m_nPage;
	int						m_nPageNext;
	int						m_nIDNow;
	int						m_nIDNext;
};

