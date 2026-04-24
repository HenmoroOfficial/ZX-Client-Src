#pragma once

#include "DlgTaskBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUITextArea.h"
#include "TaskTemplMan.h"
#include "AUI\\AUIScroll.h"
#include "AUI\\AUIStillImageButton.h"
#include "AIniFile.h"
#include "A2DSprite.h"
#define CECDLGSTASK_ANICIENT_LINEMAX 2
#define CECDLGTASK_ANICIENT_NUMMAX 4

class CDlgTaskAncient : public CDlgTaskBase
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP()
		
public:
	CDlgTaskAncient();
	virtual ~CDlgTaskAncient();
	void OnCommand_Accept(const char * szCommand);
	void OnCommand_CANCEL(const char * szCommand);
	virtual bool Tick(void);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
protected:
	PAUITEXTAREA m_pTxt_Content;
	PAUITEXTAREA m_pTxt_Item;
	PAUIIMAGEPICTURE m_pImgU[4];
	PAUIIMAGEPICTURE m_pImg_Item[CDlgTaskBase::CDLGTASK_AWARDITEM_MAX];
	PAUIIMAGEPICTURE m_pImg_Checked;
	PAUIEDITBOX m_pEdit[4];
	PAUIEDITBOX m_pEditSelect;
	ATaskTempl* m_pTemp;
	int m_nFirstLine;
	PAUISCROLL	m_pScl_Item;
	AUIStillImageButton *m_pBtn_AcceptTask;
	abase::hash_map<int, AString> m_TaskICONMap;
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	bool InitICON(const char* szFile);
};