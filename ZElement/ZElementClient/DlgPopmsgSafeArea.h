// Filename	:	DlgPopmsgSafeArea.h
// Creator	:	Fu Chonggang
// Date		:	Aug 5, 2010

#pragma once


#include "DlgBase.h"
class AUILabel;
class AUIImagePicture;

class CDlgPopmsgSafeArea : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgPopmsgSafeArea();
	virtual ~CDlgPopmsgSafeArea();

	virtual int CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam);
	virtual bool Release();

	void OnCommand_CANCEL(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();

	AUILabel *m_pMessage;
	AUIImagePicture *m_pBack;
	DWORD m_dwLastTime;
};
