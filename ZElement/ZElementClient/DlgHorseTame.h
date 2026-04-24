// File		: DlgHorseTame.h
// Creator	: Xiao Zhou
// Date		: 2008/5/7

#pragma once

#include "DlgBase.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"

class CDlgHorseTame : public CDlgBase  
{
	friend class CECGameUIMan;

	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgHorseTame();
	virtual ~CDlgHorseTame();

	void OnCommandCANCEL(const char * szCommand);
	void OnCommandStart(const char * szCommand);
	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void DragDropItem(int iSrc, PAUIOBJECT pObjOver);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

	void CheckItem(PAUIOBJECT pObj);
	
	PAUIIMAGEPICTURE		m_pItemHorse;
	PAUIIMAGEPICTURE		m_pItemAssitant;
	PAUISTILLIMAGEBUTTON	m_pBtn_Enchase;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;
};
