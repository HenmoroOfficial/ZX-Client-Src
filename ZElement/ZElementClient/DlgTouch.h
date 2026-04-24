// Filename	: DlgTouch.h
// Creator	: Wang Dongliang
// Date		: 2013/04/10
// Desc		: TOUCH合作相关界面

#pragma once

#include "DlgBase.h"
class AUIEditBox;
class CDlgTouchMin : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgTouchMin(){}
	virtual ~CDlgTouchMin(){}
    void OnCommand_Open(const char * szCommand);	
};
class CDlgTouchInfo : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgTouchInfo(){}
	virtual ~CDlgTouchInfo(){}
	virtual bool OnInitDialog();
	virtual void OnTick() ;
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
    void OnCommand_Open(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	void OnCommand_ExchangeGold(const char * szCommand);
	void OnCommand_ExchangeSilver(const char * szCommand);
	void ResetOpenFlag(){m_bPointReady = false;m_bCashReady = false;}
	bool CanOpen(){return m_bPointReady && m_bCashReady;}
	int64_t GetPoint(){return m_iPoint;}
private:
	bool m_bPointReady;
	bool m_bCashReady;
	int64_t m_iPoint;
};
class CDlgTouchExchange : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgTouchExchange(){}
	virtual ~CDlgTouchExchange(){}
	virtual bool OnInitDialog();
	virtual void OnShowDialog() ;
	virtual void OnTick() ;
	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Cancel(const char * szCommand);
	void OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_add(const char * szCommand);
	void OnCommand_AddMinus(const char * szCommand);
	int  GetItemPrice();
	void ProcessTouchExchange(bool isAgree,int type);
private:
	DWORD m_dwTimeClickStart;
	AUIEditBox* m_pEdt;

};