// File		: DlgSystem.h
// Creator	: Xiao Zhou
// Date		: 2005/8/15

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIStillImageButton.h"

class CDlgSystem : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()

public:
	void OnCommandCANCAL(const char *szCommand);
	void OnCommandQuest(const char * szCommand);
	void OnCommandInventory(const char * szCommand);
	void OnCommandCharacter(const char * szCommand);
	void OnCommandSkill(const char * szCommand);
	void OnCommandCommunity(const char * szCommand);
	void OnCommandMain2(const char * szCommand);
	void OnCommandMain3(const char * szCommand);
	void OnCommandMini(const char * szCommand);
	void OnCommandMinimize(const char * szCommand);
	void OnCommandMain(const char * szCommand);
	void OnCommand_QShop(const char * szCommand);
	void OnCommand_Talisman(const char* szCommand);
	void OnCommand_All(const char * szCommand);
	void OnCommand_System(const char * szCommand);

	CDlgSystem();
	virtual ~CDlgSystem();

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);
	
	AUICheckBox *			m_pChktemp;
	AUIStillImageButton *	m_pBtnInventory;
	AUIStillImageButton *	m_pBtnCharacter;
	AUIStillImageButton *	m_pBtnFriend;
	AUIStillImageButton *	m_pBtnFunction;
	AUIStillImageButton *	m_pBtnSystem;
	AUIStillImageButton *	m_pBtnShop;
	AUIEditBox *			m_pFrameSystem2;
};


//界面改版，添加的新界面，整合原有的功能菜单
//目前实现依赖于原来的旧界面
class CDlgSystemAll : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
public:

	CDlgSystemAll(){}
	virtual ~CDlgSystemAll(){}
	virtual bool OnInitDialog();
	void OnCommandFunction(const char * szCommand);	
};
