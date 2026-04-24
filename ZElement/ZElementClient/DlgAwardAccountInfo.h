// Filename	: DlgAwardAccountInfo.h
// Creator	: ice
// Date		: 2012/06/28
// Desc		: ’À∫≈–≈œ¢ÕÍ…∆Ω±¿¯

#pragma once

#include "DlgBase.h"

class AUIImagePicture;
class AUIStillImageButton;
class AUILabel;

class CDlgAwardAccountInfo : public CDlgBase  
{
	AUI_DECLARE_COMMAND_MAP()
	AUI_DECLARE_EVENT_MAP()

public:
	CDlgAwardAccountInfo();
	virtual ~CDlgAwardAccountInfo();

 	void OnCommand_Link(const char * szCommand);
	void OnCommand_GetAward1(const char * szCommand);
	void OnCommand_GetAward2(const char * szCommand);

protected:
	virtual bool OnInitDialog();
	virtual bool Release();
	virtual bool Tick(void);

	AUILabel *m_pTxt_ItemName[2];

	AUIStillImageButton *	m_pBtnLink;

	AUIImagePicture*		m_pImg_Item[2];

};