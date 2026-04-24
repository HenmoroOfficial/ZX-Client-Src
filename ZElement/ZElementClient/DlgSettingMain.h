// Filename	:	DlgSetting.h
// Creator	:	Fu Chonggang
// Date		:	Sep 12, 2009

#pragma once


#include "DlgBase.h"
class AUILabel;
class AUIStillImageButton;
class AUIImagePicture;
class AUIRadioButton;
class AUIEditBox;

class CDlgSettingMain : public CDlgBase
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgSettingMain();
	virtual ~CDlgSettingMain();

	virtual bool Release();

	void OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj);

	void OnCommand_CANCEL(const char * szCommand);
	void OnCommand_close(const char * szCommand);
	void OnCommand_default(const char * szCommand);
	void OnCommand_apply(const char * szCommand);
	void OnCommand_confirm(const char * szCommand);
	void OnCommand_SettingType(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual void OnTick();
	virtual bool Render();
};
