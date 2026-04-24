// Filename	:	DlgMarkAstro.h
// Creator	:	Fu Chonggang
// Date		:	Apr 23, 2010

#pragma once


#include "DlgBase.h"
class AUILabel;
class AUIStillImageButton;
class AUIImagePicture;

class CDlgMarkAstro : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgMarkAstro();
	virtual ~CDlgMarkAstro();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	void DragDropItem(int iSrc, PAUIOBJECT pObjOver);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_start(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUILabel *m_pTitle;
	AUIImagePicture *m_pItem_a;
	AUIImagePicture *m_pItem_d;
	AUIStillImageButton *m_pBtn_Charge;
	AUILabel *m_pLab_Hint;
	AUILabel *m_pLab_Astrolabe;
	AUILabel *m_pLab_Medium;
};
