// Filename	: DlgAchivMain.h
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "EC_Achievement.h"
#include "vector.h"

class CLuaState;
class CECPlayer;
class CDlgAchivItem;
class CDlgAchivItemExpand;
class CDlgAchivItemOther;
class AUISubDialog;

class CDlgAchivItemMan : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgAchivItemMan();
	virtual ~CDlgAchivItemMan();

	typedef abase::vector<sAchievement> vecAchivs;
	typedef abase::vector<CDlgAchivItem*> vecDlgItems;
	typedef abase::vector<AUISubDialog*> vecSubDialog;
	typedef abase::vector<CDlgAchivItemOther*> vecDlgItemsOther;

	vecAchivs& GetAchivs() { return m_vecAchivs; }
	CDlgAchivItemExpand* GetAchivItemExpand() { return m_pDlgItemExpand; }
	bool InitItemDlgs();
	bool ExpandItem(CDlgAchivItem* pDlg, bool bExpand = true);
	void ShowAchivItem(int nIndex);

protected:
	virtual void OnTick();
	virtual bool Render();
	virtual	bool OnInitDialog();
	virtual bool Release();

	vecAchivs				m_vecAchivs;		// 成就列表
	vecDlgItems				m_vecDlgItems;		// 对话框列表
	vecSubDialog			m_vecSubDialogs;	// 子对话框控件列表
	CDlgAchivItemExpand*	m_pDlgItemExpand;	// 展开的成就界面
	AUISubDialog*			m_pSubItem;			// 单个成就界面容器

	vecDlgItemsOther		m_vecDlgItemsOther;		// 比较成就窗口对话框列表
	vecSubDialog			m_vecSubDialogsOther;	// 比较成就窗口子对话框控件列表


	A3DRECT					m_rcWindowOld;
	int						m_nIndexCur;
};