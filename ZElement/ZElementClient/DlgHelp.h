/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   13:04
	file base:	DlgHelp
	file ext:	h
	author:		yaojun
	changed:	Fu Chonggang 2009-08-20
	
	purpose:	
*********************************************************************/


#pragma once

#include "DlgBase.h"
#include <vector>
#include "hashmap.h"

class AUIStillImageButton;

#define CDLGHELL_MAINTYPE_MAX 12
#define CDLGHELP_SUBTYPE_MAX 25

class CDlgHelp : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()
	struct TableInfo
	{
		ACString	strName;
		ACString	strUrl;
	};
public:
	CDlgHelp();
	virtual ~CDlgHelp();

	void OnCommandRdoGroup1(const char * szCommand);
	void OnCommandRdoGroup2(const char * szCommand);
	void OnCommandPrev(const char * szCommand);
	void OnCommandNext(const char * szCommand);
	void OnCommandUpInfo(const char * szCommand);
	void OnCommandCancel(const char *szCommand);

	void OnLButtonDownHtml(WPARAM, LPARAM, AUIObject *);
	
protected:
	virtual void OnTick();
	virtual bool Render();
	// control variable

	int	m_idTable[CDLGHELL_MAINTYPE_MAX][CDLGHELP_SUBTYPE_MAX];
	abase::hash_map<int, TableInfo> m_mapTableInfo;
	int m_idCurTable;
protected:
	virtual void DoDataExchange(bool bSave);
	virtual bool OnInitDialog();

	bool BuildHelp(int nGroup, int nDoc);
};

