// Filename	: DlgItemLock.h
// Creator	: Fuchonggang
// Date		: 2008/11/11

#pragma once

#include "DlgBase.h"

class CECIvtrItem;
class CDlgItemLock : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgItemLock();
	virtual ~CDlgItemLock();

	void OnCommandLock(const char* szCommand);
	void OnCommandUnLock(const char* szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void SetItemLock(CECIvtrItem *pItem, int nSlot);
	void SetItemUnLock(CECIvtrItem *pItem, int nSlot);

public:
	static A2DSprite* m_pA2DSpriteLock;

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();

protected:
	bool m_bLockType;	// 0: lock; 1: unlock	
	int	 m_iItemIndex;
	int	 m_iItemId;
};
