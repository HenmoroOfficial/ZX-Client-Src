// Filename	: DlgAchivItem.h
// Creator	: Fu Chonggang
// Date		: 2010/1/27

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITreeView.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIProgress.h"


struct sAchievement;
class CDlgAchivItemMan;

class CDlgAchivItem : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();

public:
	CDlgAchivItem();
	virtual ~CDlgAchivItem();

	void OnCommandCancel(const char* szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void SetAchiv(sAchievement *pAchiv);
	const sAchievement * GetAchiv() { return m_pAchiv; }

protected:
	virtual	bool OnInitDialog();
	virtual bool Release();
	void FilterColor(ACString& str);
	
	PAUILABEL				m_pLab_Name;
	PAUILABEL				m_pLab_Desc;
	PAUILABEL				m_pLab_Point;
	PAUILABEL				m_pLab_Time;
	PAUIIMAGEPICTURE		m_pImg_Title;
	PAUIIMAGEPICTURE		m_pImg_Metal;
	PAUIIMAGEPICTURE		m_pImg_Point;
	PAUIEDITBOX				m_pEdt_Back;
	sAchievement*			m_pAchiv;
	CDlgAchivItemMan*		m_pDlgAchivitemMan;
};

class CDlgAchivItemExpand : public CDlgAchivItem
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	enum
	{
		ACHIV_PREV_MAX = 6,
		ACHIV_COND_MAX = 10,
	};
public:
	static bool ProgressHintRenderCallback(const POINT &pt, DWORD param1, DWORD param2, DWORD param3);

protected:
	virtual	bool OnInitDialog();
	virtual void OnShowDialog();

	PAUILABEL				m_pLab_Prev[ACHIV_PREV_MAX];
	PAUILABEL				m_pLab_Cond[ACHIV_COND_MAX];
	PAUIPROGRESS			m_pProg_Cond[ACHIV_COND_MAX];
	PAUILABEL				m_pLab_Prog[ACHIV_COND_MAX];
	PAUIIMAGEPICTURE		m_pImg_Award;
	PAUIEDITBOX				m_pEdt_Frame;
	int						m_nOrigHeight;
	int						m_nOrigPerHeight;
	int						m_nOrigPerLabHeight;
	int						m_nOrigPerProgHeight;
};

class CDlgAchivPop : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP();
	AUI_DECLARE_EVENT_MAP();
	
public:
	CDlgAchivPop();
	virtual ~CDlgAchivPop();
	void SetAchiv(sAchievement *pAchiv);

	void OnCommmandAchiv(const char* szCommand);
	
private:
	virtual	bool OnInitDialog();
	virtual bool Release();
	
	PAUILABEL				m_pLab_Name;
	PAUILABEL				m_pLab_Point;
	PAUIIMAGEPICTURE		m_pImg_Title;
	PAUIIMAGEPICTURE		m_pImg_Metal;
	PAUIIMAGEPICTURE		m_pImg_Point;
	sAchievement*			m_pAchiv;
};

class CDlgAchivItemOther : public CDlgAchivItem
{
public:
	void SetAchiv(sAchievement *pAchiv);
	const sAchievement * GetAchiv() { return m_pAchiv; }

protected:
	virtual	bool OnInitDialog();

private:
	PAUILABEL				m_pLab_Status;
};