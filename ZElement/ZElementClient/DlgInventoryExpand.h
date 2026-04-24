
#pragma once
#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#define INVENTORY_SIZEMAX 252

struct ExpandItem_Info
{
	int iMoney;
	int index;
	int iCount;
	struct 
	{
		int itemid;
		int itemindex;
		int itemcnt;
	}cost_item[2];
};

class CDlgInventoryExPand : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	
public:
	CDlgInventoryExPand();
	virtual ~CDlgInventoryExPand();

	void OnCommandCancel(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void OnCommandAdd(const char * szCommand);
	void OnCommandMinus(const char * szCommmand);

	void CalcExpandInventory(int nSzie, int nNum);
	void CalcExpandStorage(int nSize, int nNum);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();
	
	ExpandItem_Info         m_ExpandItem_info;
	int						m_nItem1Num;
	int						m_nItem2Num;
	PAUIIMAGEPICTURE        m_pImg_Item1;
	PAUIIMAGEPICTURE		m_pImg_Item2;
	PAUISTILLIMAGEBUTTON	m_pBtn_Confirm;
	PAUIEDITBOX				m_pEdit_Input;
	PAUILABEL				m_pTxt_Num;
	PAUILABEL				m_pTxt_Item1Num;
	PAUILABEL				m_pTxt_Item2Num;
	abase::vector<int>      m_vecInventoryNum;
};
