// InputID.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "InputID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInputID dialog


CInputID::CInputID(CWnd* pParent /*=NULL*/)
	: CDialog(CInputID::IDD, pParent)
{
	//{{AFX_DATA_INIT(CInputID)
	m_nID = 0;
	m_nGlobalID = 0;
	m_bShowGlobalID = false;
	//}}AFX_DATA_INIT
}


void CInputID::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CInputID)
	DDX_Text(pDX, IDC_EDIT_ID, m_nID);
	DDX_Text(pDX, IDC_PATH_GLOBAL_ID, m_nGlobalID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CInputID, CDialog)
	//{{AFX_MSG_MAP(CInputID)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInputID message handlers

BOOL CInputID::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CWnd *pWnd = NULL;
	if(m_bShowGlobalID)
	{
		pWnd = GetDlgItem(IDC_PATH_GLOBAL_ID);
		pWnd->EnableWindow(true);		
	}
	else
	{
		pWnd = GetDlgItem(IDC_PATH_GLOBAL_ID);
		pWnd->EnableWindow(false);
	}
	
	return TRUE; 
}
