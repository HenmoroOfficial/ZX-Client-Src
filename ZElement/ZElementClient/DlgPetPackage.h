// File		: DlgPetPackage.h
// Creator	: Xiao Zhou
// Date		: 2008/4/3

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIListBox.h"

class CECShortcut;

class CDlgPetPackage : public CDlgBase  
{
	friend class CDlgPetEquip;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgPetPackage();
	virtual ~CDlgPetPackage();

	void OnCommandPro(const char * szCommand);
	void OnCommandDetail(const char * szCommand);
	void OnCommandStatus(const char * szCommand);
	void OnCommandMoveUp(const char * szCommand);
	void OnCommandMoveDown(const char * szCommand);
	void OnCommandWithdraw(const char * szCommand);
	void OnCommandSummon(const char * szCommand);
	void OnCommandEquip(const char * szCommand);
	void OnCommandFactory(const char * szCommand);
	void OnCommandRaise(const char * szCommand);
	void OnCommandRaiseAll(const char * szCommand);
	void OnCommandSetApperance(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDown_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDblclk_AutoFood(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	
	void SetAutoFood(CECShortcut *pSC);
	void UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot);
	void UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int iSameItem);
	void UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2);

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();

	void SetAutoFoodFromIndex(int index);
	
	PAUIIMAGEPICTURE		m_pImg_HeadPic;
	PAUILABEL				m_pTxt_Name;
	PAUILABEL				m_pTxt_Lv;
	PAUILABEL				m_pTxt_Hp;
	PAUILABEL				m_pTxt_Mp;
	PAUILABEL				m_pTxt_Horoscope;
	PAUILABEL				m_pTxt_Element;
	PAUILABEL				m_pTxt_Race;
	PAUISTILLIMAGEBUTTON	m_pBtn_Status;
	PAUISTILLIMAGEBUTTON	m_pBtn_Pro;
	PAUISTILLIMAGEBUTTON	m_pBtn_Detail;
	PAUILISTBOX				m_pLst_Pet;
};
