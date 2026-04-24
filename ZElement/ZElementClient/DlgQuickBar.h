// Filename	: DlgQuickBar.h
// Creator	: Tom Zhou
// Date		: October 11, 2005

#pragma once

#include "DlgBase.h"
#include "vector.h"

class CECShortcut;
class CECSkill;
class AUICheckBox;

class CDlgQuickBar : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgQuickBar();
	virtual ~CDlgQuickBar();

	void OnCommand_combine(const char * szCommand);
	void OnCommand_out(const char * szCommand);
	void OnCommand_add(const char * szCommand);
	void OnCommand_minus(const char * szCommand);
	void OnCommand_switch(const char * szCommand);
	void OnCommand_lock(const char * szCommand);
	void OnCommand_QShop(const char * szCommand);
	void OnCommand_Talisman(const char * szCommand);
	void OnCommand_XpSkill(const char * szCommand);
	void OnCommand_QuickBarSetting(const char * szComand);
	void OnCommand_CANCEL(const char * szCommand);

	void OnEventLButtonDown_Item(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	bool UpdateShortcuts();
	bool IsQuickbarLocked();
	static int GetCurPanel();
	static void UpdateShortcut(CECShortcut *pSC, AUIImagePicture *pCell);

	void FlashXpSkillButton(); // xp技能可释放时，按钮闪烁

	void SetTeleportInfo(int iTeleportSkillId, int iTeleportNum);

	void UpdateGridState(bool bShowAll);

protected:
	virtual void OnShowDialog();

protected:
	static int m_nCurPanel;
	static int m_nExtraPanels;

	CECSkill*  m_pXpSkill;
	DWORD	   m_dwXpBtnLastUpdateTime;

	static int m_iTeleportSkillId;
	static int m_iTeleportNum;

	virtual bool OnInitDialog();
	virtual void OnTick();
};




class CDlgQuickBarSetting : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgQuickBarSetting(){}
	virtual ~CDlgQuickBarSetting(){}
	//清除快捷键输入状态
	void ClearHotKey();
	//设置快捷键
	void SetHotKey(int nHotKey, DWORD dwCtrl);


//	void UpdateQuickBarShowState(bool bShowAll);
protected:
	virtual void OnShowDialog();
	//设置默认的技能栏快捷键
	//void OnCommand_DefaultSkill(const char* szCommand);
	//设置默认的功能性快捷键
	//void OnCommand_DefaultOthers(const char* szCommand);
	//显示所有的技能栏格子或只显示有技能的技能栏格子
	void OnCommand_ShowQuickBarGrid(const char* szCommand);
	//显示/关闭右侧技能栏和自定义技能栏
	void OnCommand_ShowAdditionalQuickBar(const char* szCommand);
    //改变自定义技能栏的显示方式
	void OnCommand_ChangeCustomQuickBarType(const char* szCommand);
	//设置默认的快捷键
	void OnCommand_Default(const char * szCommand);
	//void OnCommand_Apply(const char * szCommand);
	//void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Close(const char * szCommand);
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	//virtual void OnTick();
private:
	//当前待输入快捷键的所在界面
	int  m_iCurrentPage;
};
class CDlgQuickBarSettingDetailFrame : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgQuickBarSettingDetailFrame(){}
	virtual ~CDlgQuickBarSettingDetailFrame(){}
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	void OnCommand_TogglePage(const char * szCommand);
	void ReArrangedPages();
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	virtual void OnTick();
private:
	int  m_srcPosY;
};
class CDlgQuickBarSettingDetail : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()

public:
	CDlgQuickBarSettingDetail(){}
	virtual ~CDlgQuickBarSettingDetail(){}

public:
	static  ACString GetLKName(int iLogicKey); 
	static  ACString GetVKName(int vk);
	static  ACString GetLKNameByQuickBarItem(AString strDlgName,int index);
	ACString GetLKNameByIndex(int index);
protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	//选择某个快捷键，进入输入状态
	void OnCommand_SelectQuickItem(const char * szCommand);
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
    
private:
	//当前界面所对应的快捷键逻辑码（根据界面中的功能描述一一对应）
	abase::vector<int>  m_vecLK;
	//当前操作的快捷键索引
	int              m_iCurrentKey;

protected:
	void SetItemString(int index);
	void CheckItem(int index,bool bCheck = true);
	void SetHotKey(int nHotKey, DWORD dwCtrl);

};
