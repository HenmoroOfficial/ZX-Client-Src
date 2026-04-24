// Filename	: DlgModule.h
// Creator	: Fu Chonggang
// Date		: 2009/08/21

#pragma once

#include "DlgBase.h"
#include "vector.h"

class CDlgModule : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	
public:
	CDlgModule();
	virtual ~CDlgModule();
	
	void OnCommandInteractPage(const char* szCommand);
	// list modules
	void OnCommandGardenMain(const char* szCommand);
	void OnCommandGardenAddModule(const char* szCommand);
	// add module
	void OnCommandGardenAddModuleType(const char* szCommand);
	void OnCommand_Close(const char * szCommand);

	void OnPrtc_ModuleList(void *);
	void OnPrtc_AddModule(int id);
	bool SetIsOwn(bool bOwn=true) { m_bOwn = bOwn;}
	static void SetModuleMoney(INT64 money) { m_iMoney = money;}
	static INT64 GetModuleMoney() { return m_iMoney;}
	int GetModuleNum() { return m_iModuleNum;}
	bool IsHost() { return m_bOwn;}
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();

	static abase::vector<int> m_vecModules;		//目前只有1个组件，无用
	int m_iModuleNum;
	static INT64 m_iMoney;
	bool m_bOwn;
	int m_iOwnerId;
};
