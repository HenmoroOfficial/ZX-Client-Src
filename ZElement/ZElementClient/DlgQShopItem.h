// Filename	: DlgQShopItem.h
// Creator	: Xiao Zhou
// Date		: 2006/5/22

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"

class CECModel;
class A2DSprite;

class CDlgQShopItem : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgQShopItem();
	virtual ~CDlgQShopItem();

	void OnCommand_Buy(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void SetItem(int dlgpostion = -1, int itemindex = -1, int remain_time = -1);
	static void SetPropType(int type) { m_nDiscountType = type & 0xff000000; m_nSaleType = type & 0x00ff0000; }
	static int  GetDiscountType() { return m_nDiscountType; }
	static int  GetSaleType()     { return m_nSaleType; }

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual bool Render();
	virtual bool Release();

	ACString  GetFormatTime(int nTime);

	PAUILABEL				m_pTxt_ItemName;
	PAUILABEL				m_pTxt_Time;
	PAUILABEL				m_pTxt_Price;
	PAUILABEL				m_pTxt_Number;
	PAUILABEL				m_pTxt_Bonus;
	PAUILABEL				m_pTxt_SaleRemainTime;
	PAUISTILLIMAGEBUTTON	m_pBtn_Buy;
	PAUIIMAGEPICTURE		m_pImg_Model;

	A2DSprite*	m_pImagePresent;
	int			m_nHintImageIndex;
	int			m_nItemIndex;
	int			m_nDlgPostion;
	int			m_nPrice;
	bool		m_bDiscount;
	bool		m_bTimeLimit;
	static int	m_nDiscountType;
	static int	m_nSaleType;
};
