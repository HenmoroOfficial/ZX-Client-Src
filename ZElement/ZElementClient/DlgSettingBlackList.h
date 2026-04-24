/********************************************************************
	created:	2005/08/11
	created:	11:8:2005   17:50
	file name:	DlgSettingBlackList.h
	author:		yaojun
	
	purpose:	
*********************************************************************/


#pragma once

#include "EC_Configs.h"
#include "DlgSetting.h"
#include <vector.h>
#include <AString.h>

class AUIListBox;
class AUIEditBox;

#define CDLGSETTINGBLACKLIST_BLACKLIST_LEN		50

class CDlgSettingBlackList : public CDlgSetting  
{
	friend class CDlgFriendRequest;

	AUI_DECLARE_COMMAND_MAP()

protected:
	typedef struct _BLACK_CACHE
	{
		DWORD dwTime;
		ACString strName;
	} BLACK_CACHE, * P_BLACK_CACHE;

	EC_BLACKLIST_SETTING m_setting;
	abase::vector<BLACK_CACHE> m_vecBlackCache;

	// DDX member
	AUIListBox * m_pLbxBlackList;
	AUIEditBox * m_pEbxName;
	ACString m_strName;

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	void DoDataExchange(bool bSave);

public:
	void OnCommandChooseName(const char * szCommand);
	void OnCommandRemove(const char * szCommand);
	void OnCommandAdd(const char * szCommand);

	virtual void OnTick(void);
	virtual void UpdateView();

	CDlgSettingBlackList();
	virtual ~CDlgSettingBlackList();
};

