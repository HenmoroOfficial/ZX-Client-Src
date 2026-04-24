// Filename	:	DlgAuction.h
// Creator	:	Fu Chonggang
// Date		:	Apr 8, 2010
/*
	modified 2010/2/20
	增加范围出价
*/

#pragma once


#include "DlgBase.h"
#include "hashmap.h"
#include "vector.h"
#include "uniquebidhistory_re.hpp"

class AUIStillImageButton;
class AUIEditBox;
class AUILabel;

typedef abase::hash_map<int, UniquePriceInfo> UniquePriceMap;

class CDlgAuction : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgAuction();
	virtual ~CDlgAuction();

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownPrice(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventRButtonUpPlayer(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Buy(const char * szCommand);					// 单价输入界面
	void OnCommand_RangeBuy(const char * szCommand);			// 范围输入界面
	void OnCommand_ConsignSingle(const char * szCommand);		// 单价出价
	void OnCommand_ConsignRange(const char * szCommand);		// 范围出价
	void OnCommand_ConsignRangeConfirm(const char * szCommand);	// 范围出价确定
	void OnCommand_GetAward(const char * szCommand);			// 竞价成功，领取奖励

	void HistoryInfo(void*);									// 协议处理，竞拍相关所有信息

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	virtual void OnTick();
	virtual bool Render();
	
	bool PressStateAction(AUIStillImageButton* pBtn, AUIEditBox* pEdt, int mn=0, int mx=99, bool bAdd=true);

	AUIStillImageButton*	m_pBtnAdd[6];			// 2组金银铜
	AUIStillImageButton*	m_pBtnMinus[6];
	AUIEditBox*				m_pEdtPrice[6];
	AUILabel*				m_pLabItemNum;

	static const int		AUCTION_NUM_MAX;
	static int				m_siMoneyFloor;
	static UniquePriceMap	m_mapUniquePrice;
};
