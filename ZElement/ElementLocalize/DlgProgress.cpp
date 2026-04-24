// DlgProgress.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "DlgProgress.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress dialog


CDlgProgress::CDlgProgress(THD_INFO* pThdInfo, CWnd* pParent /*=NULL*/)
	: CDialog(CDlgProgress::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgProgress)
		// NOTE: the ClassWizard will add member initialization here
	iPercent = 0;
	pProg = NULL;
	szMsg[0] = 0;
	ASSERT(pThdInfo);
	m_pThdInfo = pThdInfo;
	m_pThdInfo->bAbort = false;
	//}}AFX_DATA_INIT
}


void CDlgProgress::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgProgress)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgProgress, CDialog)
	//{{AFX_MSG_MAP(CDlgProgress)
		// NOTE: the ClassWizard will add message map macros here
		ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgProgress message handlers

BOOL CDlgProgress::OnInitDialog()
{
	CDialog::OnInitDialog();
	pProg = ((CProgressCtrl*)GetDlgItem(IDC_PROGRESS));
	pProg->SetRange(0, 100);
	pProg->SetPos(m_pThdInfo->iPercent);
	CString str;
	str.Format(_T("%d%%"), m_pThdInfo->iPercent);
	SetDlgItemText(IDC_STATIC_PERCENT, str);
	SetDlgItemText(IDC_STATIC_MSG, m_pThdInfo->szProgMsg);

	SetTimer(0, 50, NULL);
	return TRUE;
}

void CDlgProgress::OnCancel()
{
	if (IDOK != MessageBox(_T("Are you sure to cancel?"), _T(""), MB_OKCANCEL))
		return;
	m_pThdInfo->bAbort = true;
}


void CDlgProgress::OnTimer(UINT nIDEvent)
{
	if (!m_pThdInfo)
	{
		KillTimer(nIDEvent);
		OnOK();
	}
	else
	{
		switch((int)(m_pThdInfo->iThdState))
		{
		case THD_FIN:
			KillTimer(nIDEvent);
			OnOK();
			break;

		case THD_FAIL:
			KillTimer(nIDEvent);
			MessageBox(m_pThdInfo->szProgMsg);
			CDialog::OnCancel();
			break;

		case THD_ABORT:
			KillTimer(nIDEvent);
			CDialog::OnCancel();
			break;

		case THD_RUN:
			if (m_pThdInfo->bMsgUpdate)
			{
				SetDlgItemText(IDC_STATIC_MSG, m_pThdInfo->szProgMsg);
				m_pThdInfo->bMsgUpdate = false;
			}
			if (pProg->GetPos() != m_pThdInfo->iPercent)
			{
				CString str;
				str.Format(_T("%d%%"), m_pThdInfo->iPercent);
				SetDlgItemText(IDC_STATIC_PERCENT, str);
				pProg->SetPos(m_pThdInfo->iPercent);
			}
			break;
		}
	}
}