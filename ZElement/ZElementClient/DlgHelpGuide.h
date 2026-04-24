// Filename	: DlgHelpGuide.h
// Creator	: Wang Dongliang
// Date		: 2013/07/11
// Desc		: 指引相关界面

#pragma once

#include "DlgBase.h"
#include "vector.h"
class AUIScroll;
class AUISubDialog;
class AUIImagePicture;
enum
{
	EHGDT_COMMON,       //普通类型，脚本配置界面名称和控件名称
	EHGDT_TASKTRACE,    //任务追踪界面，脚本配置任务ID
	EHGDT_INVENTORY,    //包裹界面，脚本配置物品ID
	EHGDT_NPCTALK,       //npc对话界面，脚本配置对话选项的索引
	EHGDT_NPCTALKMAIN,   //npc主线对话界面，脚本配置对话选项的索引
	EHGDT_NUM,
};

//指引箭头界面

class CDlgHelpGuide : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpGuide();
	virtual ~CDlgHelpGuide(){}
	virtual void OnTick() ;
	
protected:
    virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
	PAUIOBJECT   m_pDestControl;
	PAUIDIALOG   m_pDestDlg;
	//箭头类型，上、下、左、右
	BYTE         m_iType;
	//界面类型 EHGDT_COMMON，EHGDT_TASKTRACE....
	BYTE         m_iDlgType;
	//任务ID
	int          m_iTaskId;
	//物品ID
	int          m_iItemId;
	//NPC对话选项索引
	int          m_iNpcIndex;
	//int          m_iGuide;
	//DWORD        m_dwStartTime;
	//步骤ID
	int          m_iStepId;

    void GetDestPos(int& offsetX,int& offsetY,POINT point,SIZE size1);
};


//指引欢迎词界面
class CDlgHelpWelcom : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpWelcom(){}
	virtual ~CDlgHelpWelcom(){}
protected:
	void OnCommand_Agree(const char * szCommand);
	void OnCommand_DisAgree(const char * szCommand);
private:
};



//指引移动界面
class CDlgHelpMove : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpMove(){}
	virtual ~CDlgHelpMove(){}
protected:
	virtual bool OnInitDialog();
    virtual void OnShowDialog();
	virtual void OnTick();
	void OnCommand_Confirm(const char * szCommand);
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
	AUIImagePicture* m_pImageKeyBoard;
	AUIImagePicture* m_pImageMouse;
	DWORD            m_dwStartTime;
};

//战斗指引界面
class CDlgHelpFight : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpFight(){}
	virtual ~CDlgHelpFight(){}
protected:
	void OnCommand_Close(const char * szCommand);
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
private:
};


//
//最小化进入界面
class CDlgHelpAimGuideMin : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpAimGuideMin(){}
	virtual ~CDlgHelpAimGuideMin(){}

	//打开目的引导入口界面
	void OnCommand_Open(const char * szCommand);

};
//目的引导入口界面
class CDlgHelpAimGuideEnter : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpAimGuideEnter(){}
	virtual ~CDlgHelpAimGuideEnter(){}
protected:
	void OnCommand_View(const char * szCommand);
	void OnCommand_Close(const char* szCommand);
private:
};

//目的引导界面
class CDlgHelpAimGuide : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	typedef abase::vector<int>  AimGuideIDs;
	CDlgHelpAimGuide(){}
	virtual ~CDlgHelpAimGuide(){}
	

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_Close(const char* szCommand);
	void OnCommand_Return(const char* szCommand);
	void RefreshAimGuideInfo();
private:
	AUIScroll            *m_pSclItem;
	int                  m_nFirstLine;
	AimGuideIDs          m_vecIndex;
	AUISubDialog        *m_pSubDlg1;
	AUISubDialog        *m_pSubDlg2;
	AUISubDialog        *m_pSubDlg3;
	//int                  m_iType;
};
//目的引导子界面
class CDlgHelpAimGuideDetail : public CDlgBase
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpAimGuideDetail(){}
	virtual ~CDlgHelpAimGuideDetail(){}
	//virtual bool OnInitDialog();
protected:
	virtual int  CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventLButtonDownMapPos(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseMoveMapPos(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	virtual bool RefreshAimGuideInfo(int index);
private:
	ACString m_strHintNpc;
	
};

//目的引导子界面2
class CDlgHelpAimGuideDetail2 : public CDlgHelpAimGuideDetail
{
	AUI_DECLARE_EVENT_MAP()
	AUI_DECLARE_COMMAND_MAP()
public:
	CDlgHelpAimGuideDetail2(){}
	virtual ~CDlgHelpAimGuideDetail2(){}
protected:
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnCommand_OpenDlg(const char * szCommand);
	virtual bool RefreshAimGuideInfo(int index);
private:
	AString m_strDlgName;

};