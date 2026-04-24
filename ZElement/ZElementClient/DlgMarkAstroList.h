// Filename	:	DlgMarkAstroList.h
// Creator	:	Fu Chonggang
// Date		:	May 18, 2010

#pragma once


#include "DlgBase.h"
class AUILabel;
class AUIStillImageButton;
class AUIListBox;
class AUIImagePicture;
class CECIvtrTelePortation;

class CDlgMarkAstroList : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgMarkAstroList();
	virtual ~CDlgMarkAstroList();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	
	void OnEventLButtonDBClick_Transport(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_Transport(const char * szCommand);
	void OnCommand_Add(const char * szCommand);
	void OnCommand_Delete(const char * szCommand);
	void OnCommand_Edit(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	void UpdateInfo();
	bool CheckNameValid(ACHAR* szText);

	AUILabel *m_pTitle;
	AUIStillImageButton *m_pBtn_Transport;
	AUILabel *m_pLab_Num;
	AUILabel *m_pTxt_Num;
	AUIListBox *m_pLst_Marklist;
	AUIStillImageButton *m_pBtn_Add;
	AUIImagePicture *m_pImg_Item;

	int						m_nIndex;
	CECIvtrTelePortation*	m_pItem;
	bool					m_bNeedUpdate;
};
