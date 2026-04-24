// Filename	: DlgFastPayCode.h
// Creator	: ice
// Date		: 2012/07/06
// Desc		: 快捷充值--短信验证码

#pragma once

#include "DlgBase.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIStillImageButton.h"

class AUIImagePicture;
class AUIStillImageButton;
class AUILabel;

struct MERCHANT_DISCOUNT
{
	int id;
	int discount;
};

class CDlgFastPayCode : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgFastPayCode();
	virtual ~CDlgFastPayCode();

 	void OnCommand_Confirm(const char * szCommand);
	void OnCommand_Get(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual void OnShowDialog();
	virtual bool Release();
	virtual bool Tick(void);

protected:

	AUILabel* m_pLabelSeconds;
	AUIStillImageButton* m_pBtnGet;

	unsigned int m_nSeconds;

	DWORD m_dwRefreshTime;

};