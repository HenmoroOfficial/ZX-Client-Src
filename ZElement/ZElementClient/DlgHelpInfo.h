// Filename	: DlgHelpInfo.h
// Creator	: Wang Dongliang
// Date		: 2013/08/29
// Desc		: 帮助信息界面



#pragma once

#include "DlgBase.h"
#include "vector.h"
#include "hashmap.h"

class CLuaState;
class _AUITREEVIEW_ITEM;
class AUITreeView;
class AUISubDialog;
//目的引导子界面
class CDlgHelpInfo : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
    AUI_DECLARE_COMMAND_MAP()
public:
	enum SortType
	{
		ST_ID,   //
	};

	typedef struct _SSubHelpInfo 
	{
		int      iId;
	    ACString strSubName;
		AString  strDlg;
	}SSubHelpInfo;
	typedef struct _SHelpInfo
	{
		int      iId;
	    ACString  strMainName;
		abase::vector<SSubHelpInfo> vecSubInfo;
	}SHelpInfo;

	typedef abase::vector<SHelpInfo> HelpInfo;
	typedef abase::hash_map<AString,_AUITREEVIEW_ITEM*>  Name2TreeItemTable;
public:
	CDlgHelpInfo(){}
	virtual ~CDlgHelpInfo(){}
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Release();
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	bool LoadHelpinfo();
	void ReleaseHelpInfo();
	void RefreshHelpInfoTree();
	void SortHelpInfo(SortType st = ST_ID,bool bInc = true);

private:
	CLuaState         *m_pState;
	AUITreeView       *m_pTreeView;
	AUISubDialog      *m_pSubDlg;
	HelpInfo          m_HelpInfo;
	_AUITREEVIEW_ITEM*  m_pCurSelected;
	Name2TreeItemTable  m_Name2ItemTable;
	

};