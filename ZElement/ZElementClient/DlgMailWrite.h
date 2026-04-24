// Filename	: DlgMailWrite.h
// Creator	: Xiao Zhou
// Date		: 2005/10/31

#pragma once

#include "DlgBase.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"

class CECIvtrItem;

class CDlgMailWrite : public CDlgBase  
{
	AUI_DECLARE_EVENT_MAP();
	AUI_DECLARE_COMMAND_MAP();

public:
	CDlgMailWrite();
	virtual ~CDlgMailWrite();

	void OnCommandCancel(const char* szCommand);
	void OnCommandFriend(const char * szCommand);
	void OnCommandSend(const char* szCommand);
	void OnCommandChoosemoney(const char * szCommand);

	void OnEvent_LButtonDown_AttachItem(WPARAM wParam, LPARAM lParam, AUIObject * pObj);
	void OnEvent_LButtonDown_Money(WPARAM wParam, LPARAM lParam, AUIObject * pObj);

	void CreateNewMail(const ACHAR *strMailto = NULL, ACString szSubject = _AL(""));
	void SetAttachItem(CECIvtrItem *pItem, int nItemNumber, int nItemPos);
	void SendMail(int idPlayer);
	void SendMailRe(void *pData);

protected:
	virtual bool OnInitDialog();
	virtual void OnTick();
	virtual void OnShowDialog();

	PAUIEDITBOX				m_pTxt_MailTo;
	PAUIEDITBOX				m_pTxt_Subject;
	PAUIEDITBOX				m_pTxt_Content;
	PAUIIMAGEPICTURE		m_pImg_AttachItem;
	PAUISTILLIMAGEBUTTON	m_pBtn_Send;
	PAUILABEL				m_pTxt_Money1;
	PAUILABEL				m_pTxt_Money2;
	PAUILABEL				m_pTxt_Money3;
	PAUILABEL				m_pLab_Money1;
	PAUILABEL				m_pLab_Money2;
	PAUILABEL				m_pLab_Money3;
	int						m_idItem;
	int						m_nItemNumber;
	int						m_nItemPos;

};
