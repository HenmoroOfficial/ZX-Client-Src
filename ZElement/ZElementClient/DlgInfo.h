// Filename	: DlgInfo.h
// Creator	: Tom Zhou
// Date		: October 19, 2005

#pragma once

#include "DlgBase.h"

class CECScriptContext;

class CDlgInfo : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()

public:
	enum
	{
		INFO_NULL = 0,
		INFO_QUESTION,
		INFO_SYSTEM,
		INFO_FRIEND,
		INFO_HELP,
		INFO_NUM
	};
	typedef struct _INFORMATION
	{
		AString strType;
		ACString strMsg;
		DWORD dwLife;
		DWORD dwLifeBegin;
		DWORD dwData1;
		DWORD dwData2;
		DWORD dwData3;
	} INFORMATION, * P_INFORMATION;

public:
	CDlgInfo();
	virtual ~CDlgInfo();

	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonUp_Icon(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonUp_List(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void BuildInfoList(int nType);
	bool AddInfo(int nType, INFORMATION Info);
	bool NeedAddPopTask(int nType, int id);
	void DeletePopTask(int nType, int id);
	
	void SetScriptContext(CECScriptContext * pContext);

protected:
	CECScriptContext * m_pScriptContext;
	static abase::vector<INFORMATION> m_vecInfo[INFO_NUM];
	
	virtual bool OnInitDialog();
	virtual void OnTick();
};
