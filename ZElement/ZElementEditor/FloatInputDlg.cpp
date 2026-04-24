// FloatInputDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "FloatInputDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFloatInputDlg dialog


CFloatInputDlg::CFloatInputDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFloatInputDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFloatInputDlg)
	m_fFloat = 0.0f;
	//}}AFX_DATA_INIT
}


void CFloatInputDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFloatInputDlg)
	DDX_Text(pDX, IDC_EDIT_FLOAT, m_fFloat);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFloatInputDlg, CDialog)
	//{{AFX_MSG_MAP(CFloatInputDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFloatInputDlg message handlers
void CFloatInputDlg::OnOK()
{
	UpdateData();
	CDialog::OnOK();
}