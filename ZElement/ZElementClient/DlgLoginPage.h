// File		: DlgLoginPage.h
// Creator	: Xiao Zhou
// Date		: 2005/11/23

#pragma once

#include "AUI\\AUIDialog.h"

class AUIObject;
class AUIComboBox;
class AUIStillImageButton;
class AUILabel;

class CDlgLoginPage : public AUIDialog  
{
	friend class ObjectList;

	AUI_DECLARE_EVENT_MAP()

public:
	CDlgLoginPage();
	virtual ~CDlgLoginPage();
	
	void ChangeSize(int nWidth,int nHeight);

	void OnEvent_mousewheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEvent_mousemove(WPARAM wParam, LPARAM lParam, AUIObject *pObj);
	void OnEvent_lbuttonup(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

protected:
	virtual void OnShowDialog();
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual void OnTick();
	virtual void Resize(A3DRECT rcOld, A3DRECT rcNew);

	bool					m_bUseNewFontSize;
	AString					m_aInitUrl;
	bool					m_bFirstOpen;
	bool					m_bLoaded;
};
