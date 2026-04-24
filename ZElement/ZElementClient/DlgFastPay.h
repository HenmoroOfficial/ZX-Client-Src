// Filename	: DlgFastPay.h
// Creator	: ice
// Date		: 2012/07/04
// Desc		: 快捷充值

#pragma once

#include "DlgBase.h"

#include "AUI\\AUITextArea.h"

class AUIImagePicture;
class AUIStillImageButton;
class AUILabel;

typedef struct _DISCOUNT_GRADE
{
	int amount_begin;
	int discount;
} DISCOUNT_GRADE; 

class CDlgFastPay : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

typedef struct _MERCHANT_DISCOUNT
{
	int id; 
	abase::vector<DISCOUNT_GRADE> *pvecdiscountgrade;	
} MERCHANT_DISCOUNT;

public:
	CDlgFastPay();
	virtual ~CDlgFastPay();

 	void OnCommand_Confirm(const char * szCommand);

	void SetDiscount(int nDisCount) { m_nDiscount = nDisCount;}

	void AddBindInfo(int bindID);
	void AddMerchantDiscount(int id, abase::vector<DISCOUNT_GRADE> *pvecdiscountgrade);

	void SendFastPayCmd();

	bool HasBind(int idBind);
protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual bool Tick(void);
	virtual void OnShowDialog();
protected:
	PAUITEXTAREA m_pTxtInfo;

	unsigned int m_nYuanbaoNum;

	int m_nDiscount;	// 折扣信息， 0-100
	int m_npaymode;     // 支付方式， 1为支付宝
	abase::vector<DISCOUNT_GRADE> *m_pvecdiscountgrade; // 当前支付方式对应的折扣表
	abase::vector<MERCHANT_DISCOUNT> m_vecMerchantDiscount;
	abase::vector<int>	m_vecBindInfo;
};