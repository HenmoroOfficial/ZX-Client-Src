// File		: DlgTalismanService.h
// Creator	: Xiao Zhou
// Date		: 2007/4/19

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "hashmap.h"
#include "AString.h"

class CDlgTalismanService : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgTalismanService();
	virtual ~CDlgTalismanService();

	void OnCommandCANCEL(const char * szCommand);
	void OnCommandStart(const char * szCommand);
	void OnCommandEnchantStart(const char* szCommand);
	void OnCommandEnchantStop(const char* szCommand);
	void OnCommandShopExchange(const char * szCommand);
	void OnCommandHelp(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void DragDropItem(int iSrc, PAUIOBJECT pObjOver);

	int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void ContinueEnchant();

    virtual void OnShowDialog() ;
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	void CheckItem(PAUIOBJECT pObj);
	bool IsEnchantGreen();
	void ClearImagePicControl(AUIObject * pObj);
	
	PAUIIMAGEPICTURE		m_pItema;
	PAUIIMAGEPICTURE		m_pItemb;
	PAUIIMAGEPICTURE		m_pItemc;
	PAUIIMAGEPICTURE		m_pItemd;
	PAUIIMAGEPICTURE		m_pIteme;
	PAUIIMAGEPICTURE		m_pItemf;
	PAUISTILLIMAGEBUTTON	m_pBtn_Enchase;
	PAUISTILLIMAGEBUTTON	m_pBtn_Exchange;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;
	PAUISTILLIMAGEBUTTON    m_pBtn_EnchantStart;
	PAUISTILLIMAGEBUTTON    m_pBtn_EnchantStop;

	bool                    m_bEnchangting;
	bool                    m_bEnchangtStart;

	bool					m_bLock;		// 法宝技能融合过程中不能更换物品，打补丁做法

	abase::hash_map<AString, int> m_ItemID;
};
