// Filename	: DlgBodyChange.h
// Creator	: Fu Chonggang
// Date		: 2009/08/19

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"

class CECSkill;

class CDlgBodyChange : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgBodyChange();
	virtual ~CDlgBodyChange();
	void OnCommand_Begin(const char * szCommand);
	void OnCommand_Close(const char * szCommand);

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventLButtonDblClk(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEventMouseMove(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);

	void DragDropItem(int iSrc, PAUIOBJECT pObjOver);
	void SetImage(AUIImagePicture *pImage, CECSkill *pSkill, bool bPositiveSkill = false);

protected:
	virtual void OnTick();
	virtual bool OnInitDialog();
	virtual void OnShowDialog();

	PAUISTILLIMAGEBUTTON	m_pBtn_Enchase;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;
	DWORD					m_dwShowTime;
};
