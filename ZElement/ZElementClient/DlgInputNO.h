// Filename	: DlgInputNO.h
// Creator	: Tom Zhou
// Date		: October 10, 2005

#pragma once

#include "DlgBase.h"

class CDlgInputNO : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgInputNO();
	virtual ~CDlgInputNO();

	void OnCommand_max(const char * szCommand);
	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_drop(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventKeyDown_Cancel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void SetType(int nType);

	enum
	{
		INPUTNO_NULL = 0,
		INPUTNO_BUY_ADD,
		INPUTNO_BUY_REMOVE,
		INPUTNO_SELL_ADD,
		INPUTNO_SELL_REMOVE,
		INPUTNO_DROP_ITEM,
		INPUTNO_DROP_MONEY,
		INPUTNO_TRADE_MONEY,
		INPUTNO_TRADE_MOVE,
		INPUTNO_TRADE_ADD,
		INPUTNO_TRADE_REMOVE,
		INPUTNO_MOVE_ITEM,
		INPUTNO_STORAGE_TRASH_MONEY,
		INPUTNO_STORAGE_IVTR_MONEY,
		INPUTNO_STORAGE_GET_ITEMS,
		INPUTNO_STORAGE_PUT_ITEMS,
		INPUTNO_STORAGE_MOVE_ITEMS,
		INPUTNO_PETINVO_GET_ITEMS,
		INPUTNO_PETINVO_PUT_ITEMS,
		INPUTNO_PETINVO_MOVE_ITEMS,
		INPUTNO_MOVE_MAILATTACH,
		INPUTNO_ATTACH_MONEY,
		INPUTNO_GOLDTRADE_MONEY,
		INPUTNO_GOLDACCOUNT_MONEY,
		INPUTNO_QSHOPITEM_BUY,
		INPUTNO_QSHOPITEM_BONUSBUY,
		INPUTNO_QSHOPITEM_CSSBUY,
		INPUTNO_GARDENQSHOPITEM_BUY,
		INPUTNO_GARDENQSHOPITEM_SELL,
		INPUTNO_COSIGN,
		INPUTNO_LITTLEPET,
		INPUTNO_REPUTATION_BUY_ADD,
		INPUTNO_GUILD_SHOP,
	};

protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual void OnTick();

	int m_nInputNO;
	DWORD m_dwTimeClickStart;
};
