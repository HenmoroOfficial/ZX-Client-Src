// File		: CDlgLoginPwdProtect.cpp
// Creator	: Fu Chonggang
// Date		: 2012/03/22

#include "DlgLoginPwdProtect.h"
#include "EC_LoginUIMan.h"
#include "AUI\\AUIEditBox.h"
#include "A3DGFXEx.h"

#define new A_DEBUG_NEW

CDlgLoginPwdProtect::CDlgLoginPwdProtect()
{
}

CDlgLoginPwdProtect::~CDlgLoginPwdProtect()
{
}

bool CDlgLoginPwdProtect::OnInitDialog()
{
	m_pEdt1 = (PAUIEDITBOX)GetDlgItem("DEFAULT_Input_1");
	m_pEdt2 = (PAUIEDITBOX)GetDlgItem("Input_2");
	m_pEdt3 = (PAUIEDITBOX)GetDlgItem("Input_3");
	return true;
}

// 如果当前框输入达到2个字符，跳到下个框
void CDlgLoginPwdProtect::OnTick()
{
	if (m_pObjFocus && m_pObjFocus->GetType()==AUIOBJECT_TYPE_EDITBOX)
	{
		PAUIEDITBOX pEdt = (PAUIEDITBOX)m_pObjFocus;
		if(m_pObjFocus!=m_pLastFocus)
		{
			m_pLastFocus = m_pObjFocus;
		}
		else
		{
			int curInputNum = a_strlen(pEdt->GetText());
			if (m_iCurInputNum<2&&curInputNum==2)
			{
				if (m_pObjFocus==m_pEdt1)
					ChangeFocus(m_pEdt2);
				else if (m_pObjFocus==m_pEdt2)
					ChangeFocus(m_pEdt3);
			}
		}
		m_iCurInputNum = a_strlen(pEdt->GetText());
	}
}