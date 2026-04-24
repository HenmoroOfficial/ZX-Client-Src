// File		: CDlgLoginPwdProtect.h
// Creator	: Fu Chonggang
// Date		: 2012/03/22
// Desc		: 密保卡输入完毕一个框后切换到下一个框

#pragma once

#include "AUI\\AUIDialog.h"

class AUIEditBox;

class CDlgLoginPwdProtect : public AUIDialog  
{
public:
	CDlgLoginPwdProtect();
	virtual ~CDlgLoginPwdProtect();
	
protected:
	virtual bool OnInitDialog();
	virtual void OnTick();

private:
	AUIEditBox	*m_pEdt1;
	AUIEditBox	*m_pEdt2;
	AUIEditBox	*m_pEdt3;

	AUIObject	*m_pLastFocus;
	int			m_iCurInputNum;
	
};
