// Filename	: DlgBase.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "AMemory.h"
#include "A3DMacros.h"
#include "AUI\\AUILuaDialog.h"

class CECGame;
class CECGameRun;
class CECGameSession;
class CECWorld;
class CECHostPlayer;
class CECGameUIMan;
class CECUIData;

class CDlgBase : public AUILuaDialog  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBase();
	virtual ~CDlgBase();

	void OnCommand_help(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_OpenQShop(const char* szCommand);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

protected:
	virtual bool OnInitDialog();

	CECGame * GetGame();
	CECGameRun * GetGameRun();
	CECGameSession * GetGameSession();
	CECWorld * GetWorld();
	CECHostPlayer * GetHostPlayer();
	CECGameUIMan * GetGameUIMan();
	CECUIData*	GetUIData();

	void SetItemText(const char* pItemName, const wchar_t* pText);
	void SetItemText(const char* pItemName, const int iNumber);
	const wchar_t* GetItemText(const char* pItemName);
	void SetItemHint(const char* pItemName, const wchar_t* pText);
	void SetItemHint(const char* pItemName, const int iNumber);
	void EnableItem(const char* pItemName, const bool bEnable);	
	bool IsItemEnabled(const char* pItemName);
	void ShowItem(const char* pItemName, const bool bShow);
	bool IsItemShow(const char* pItemName);
	void SetItemColor(const char* pItemName, A3DCOLOR color);	
	void SetItemPos(const char* pItemName, int x, int y);
	void SetItemSize(const char* pItemName, int wid, int hei);
	void SetItemData(const char* pItemName, DWORD dwData, AString strName = "");
	DWORD GetItemData(const char* pItemName, AString strName = "");
	void SetItemDataPtr(const char* pItemName, void * pvData, AString strName = "");
	void* GetItemDataPtr(const char* pItemName, AString strName = "");

	void SetItemProgress(const char* pItemName, const float fProgress, const bool bHint = true);
	void SetItemActionProgress(const char* pItemName, const float fProgress, const DWORD dwMSecs, const bool bHint = true);
	bool IsItemChecked(const char* pItemName);	
	void CheckItem(const char* pItemName, const bool bCheck);

	void PutControlAbove(PAUIOBJECT pControl, PAUIOBJECT pRelativeTo);
	void PutControlBelow(PAUIOBJECT pControl, PAUIOBJECT pRelativeTo);
	void BringControlToTop(PAUIOBJECT pControl) {PutControlAbove(pControl, NULL);}
	void PutControlToBottom(PAUIOBJECT pControl) {PutControlBelow(pControl, NULL);}
};

