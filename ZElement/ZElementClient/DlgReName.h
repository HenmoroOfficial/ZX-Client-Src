// Filename	: DlgReName.h
// Creator	: Fu Chonggang
// Date		: 2012.7.27

#pragma once

#include "DlgBase.h"
#include "queryrolenamehistory_re.hpp"
#include "vector.h"

class AUIImagePicture;
class CECIvtrItem;

// 角色改名
// 帮派、家族改名
class CDlgReName : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgReName();
	virtual ~CDlgReName();
	
	void OnCommandConfirm(const char * szCommand);
	void SetItem(int iIndex, CECIvtrItem* pItem);
	
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
protected:
	virtual void OnShowDialog();
	virtual void OnHideDialog();
	
	int m_iIndex;
};

class AUIScroll;

class CDlgNameHistory : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgNameHistory() : m_iFirstLine(0) {}
	virtual ~CDlgNameHistory() {}
	
	void OnCommandConfirm(const char * szCommand);
	void OnCommandRdoChar(const char * szCommand);

	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	void UpdateDisplay();

	ACString m_strNameSearch;
	abase::vector<GNET::NameHistory>	m_vecNameHistory;
	AUIScroll	*m_pSclName;
	int m_iFirstLine;

	static const int ms_iMaxNameDisp;
};

class CDlgEquipReName : public CDlgBase
{
    AUI_DECLARE_COMMAND_MAP()
    AUI_DECLARE_EVENT_MAP()
  
public:
	CDlgEquipReName(){}
	virtual ~CDlgEquipReName(){}
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_CANCEL(const char * szCommand);
	void OnCommandConfirm(const char * szCommand);
	void ProcessEquipReName(bool isAgree);
		  
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	bool CheckNameValid(ACHAR* szText);
private:

	AUIImagePicture *m_pImgItem1;
	AUIImagePicture *m_pImgItem2;
			 				
};


