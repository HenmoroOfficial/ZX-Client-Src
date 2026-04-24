// FindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "FindDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// FindDlg dialog


FindDlg::FindDlg(CWnd* pParent /*=NULL*/)
	: CDialog(FindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(FindDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void FindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(FindDlg)
	DDX_Text(pDX, IDC_FIND_EDIT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(FindDlg, CDialog)
	//{{AFX_MSG_MAP(FindDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// FindDlg message handlers
