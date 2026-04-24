// Filename	: DlgMailRead.h
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUITextArea.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIEditBox.h"

class CDlgMailRead : public CDlgBase  
{
	friend class CDlgMailList;

	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgMailRead();
	virtual ~CDlgMailRead();

	void OnCommandCancel(const char* szCommand);
	void OnCommandGetAttach(const char* szCommand);
	void OnCommandReply(const char* szCommand);

	void ReadMail(void *pData);
	void GetAttachRe(void *pData);

protected:
	virtual bool OnInitDialog();
	
	PAUILABEL				m_pTxt_MailFrom;
	PAUILABEL				m_pTxt_Subject;
	PAUILABEL				m_pTxt_NotSystemMail;
	PAUITEXTAREA			m_pTxt_Content;
	PAUIIMAGEPICTURE		m_pImg_AttachItem;
	PAUISTILLIMAGEBUTTON	m_pBtn_GetAttach;
	PAUISTILLIMAGEBUTTON	m_pBtn_Reply;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;


	int						m_idMail;
	int						m_idSender;

};
