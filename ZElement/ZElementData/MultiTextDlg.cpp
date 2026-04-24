// MultiTextDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "MultiTextDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMultiTextDlg dialog


CMultiTextDlg::CMultiTextDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMultiTextDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMultiTextDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
}


void CMultiTextDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMultiTextDlg)
	DDX_Text(pDX, IDC_EDIT_DESC, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMultiTextDlg, CDialog)
	//{{AFX_MSG_MAP(CMultiTextDlg)
	ON_BN_CLICKED(ID_MODIFYED, OnModifyed)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMultiTextDlg message handlers

void CMultiTextDlg::OnModifyed() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CDialog::OnOK();
}
