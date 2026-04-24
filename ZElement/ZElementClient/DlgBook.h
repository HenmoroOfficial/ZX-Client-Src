// Filename	: DlgBook.h
// Creator	: Xiao Zhou
// Date		: 2008/8/11

#pragma once

#include "DlgBase.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIVerticalText.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CLuaState;
class CECIvtrBook;

class CDlgBookBase : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgBookBase();
	virtual ~CDlgBookBase();
	
	void OnCommandNext(const char* szCommand);
	void OnCommandPrev(const char* szCommand);
	void OnCommandCancel(const char* szCommand);
	
	void SetBookInfo(int nIndex, CECIvtrBook* pBook);
	
protected:
	virtual	bool OnInitDialog();
	void Update();
	virtual void SetBookText(const ACHAR *pszText) = 0;
	
	PAUISTILLIMAGEBUTTON	m_pBtn_Next;
	PAUISTILLIMAGEBUTTON	m_pBtn_Prev;
	PAUISTILLIMAGEBUTTON	m_pBtn_Close;
	int						m_nPage;
	CECIvtrBook				*m_pItemBook;
};

class CDlgBookScroll : public CDlgBookBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgBookScroll();
	virtual ~CDlgBookScroll();

protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void SetBookText(const ACHAR *pszText);

	PAUITEXTAREA			m_pTxt_Content;
	PAUIIMAGEPICTURE		m_pImg_Book;
	PAUIIMAGEPICTURE		m_pImg_Content;
	DWORD					m_dwTick;
};

class CDlgBookType1 : public CDlgBookBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgBookType1();
	virtual ~CDlgBookType1();
	
protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void SetBookText(const ACHAR *pszText);
	
	PAUITEXTAREA			m_pTxt_Content;
};

class CDlgBookVertical : public CDlgBookBase  
{
	AUI_DECLARE_COMMAND_MAP();
public:
	CDlgBookVertical();
	virtual ~CDlgBookVertical();
	
protected:
	virtual void OnTick();
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void SetBookText(const ACHAR *pszText);
	
	PAUIVERTICALTEXT		m_pVeT_Content;
};