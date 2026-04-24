// Filename	: DlgPetEquip.h
// Creator	: Fu Chonggang
// Date		: 2009/1/06

#pragma once

#include "DlgBase.h"
#include "EC_IvtrTypes.h"

class CECIvtrItem;

class CDlgSoul : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
		
public:
	CDlgSoul();
	virtual ~CDlgSoul();
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();

public:
	void OnCommand_start(const char * szCommand);
	void OnCommand_enchase(const char * szCommand);
	void OnCommand_split(const char * szCommand);
	void OnCommand_resume(const char * szCommand);
	void OnCommand_depart(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetItemDepart(CECIvtrItem *pItem, int nSlot);
	void SetItemEnchaseEquip(CECIvtrItem *pItem, int nSlot);
	void SetItemEnchaseSoul(CECIvtrItem *pItem, int nSlot);
	void SetItemToOwn(CECIvtrItem *pItem, int nSlot);
	void SetItemResume(CECIvtrItem *pItem, int nSlot);
	void SetItemSplit(CECIvtrItem *pItem, int nSlot);

	void ClearObject();
protected:
	static int	 m_iOwnItemIndex;
	static int	 m_iOwnItemId;
	int	 m_iEnchaseItemIndex;
	int	 m_iEnchaseItemId;
	int	 m_iEnchaseSoulIndex;
	int	 m_iEnchaseSoulId;
	int  m_iSoulSlot;
	int  m_iSoulSlotSuit;
	int	 m_iSplitItemIndex;
	int	 m_iSplitItemId;
	int	 m_iResumeItemIndex;
	int	 m_iResumeItemId;
	int	 m_iDepartItemIndex;
	int	 m_iDepartItemId;

	int  m_iEnchaseFee;
	int  m_iSplitFee;
	int  m_iResumeFee;
	int  m_iResumeFeeTotal;
	int  m_iDepartFee;
	
	CECIvtrItem *m_pCurEquip;
	CECIvtrItem *m_pItemSoul;
protected:
	int CurrentAvailableSoul(CECIvtrItem *pItem);
	bool HasSoul(CECIvtrItem *pItem);
};
