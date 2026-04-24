// DlgToxmlOption.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "DlgToxmlOption.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgToxmlOption dialog


CDlgToxmlOption::CDlgToxmlOption(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgToxmlOption::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgToxmlOption)
		// NOTE: the ClassWizard will add member initialization here
	m_bOnlyUntrans = false;
	m_bIncludeID  = false;
	//}}AFX_DATA_INIT
}


void CDlgToxmlOption::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgToxmlOption)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgToxmlOption, CDialog)
	//{{AFX_MSG_MAP(CDlgToxmlOption)
//	ON_BN_CLICKED(IDC_CHECK_UNTRANS, OnCheckUntrans)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgToxmlOption message handlers
void CDlgToxmlOption::OnOK()
{
	m_bOnlyUntrans = (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK_UNTRANS))->GetCheck());
	m_bIncludeID = (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK_ID))->GetCheck());

	CDialog::OnOK();
}

void CDlgToxmlOption::OnCheckUntrans() 
{
	// TODO: Add your control notification handler code here
	if (BST_CHECKED==((CButton*)GetDlgItem(IDC_CHECK_UNTRANS))->GetCheck())
	{
		((CButton*)GetDlgItem(IDC_CHECK_ID))->SetCheck(BST_UNCHECKED);
		GetDlgItem(IDC_CHECK_ID)->EnableWindow(FALSE);
	}
	else
	{
		GetDlgItem(IDC_CHECK_ID)->EnableWindow(TRUE);
	}
}
