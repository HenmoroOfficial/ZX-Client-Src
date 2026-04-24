// ListBar.cpp : implementation file
//

#include "stdafx.h"
#include "ModelView.h"
#include "ListBar.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CListBar dialog


CListBar::CListBar(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CListBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CListBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListBar, CDialogBar)
	//{{AFX_MSG_MAP(CListBar)
	ON_WM_SIZE()
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListBar message handlers

void CListBar::OnSize(UINT nType, int cx, int cy) 
{
	CDialogBar::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rect;
	GetClientRect(&rect);
	
	m_ListDlg.SetWindowPos(NULL,2,10,rect.Width()-2,rect.Height()-12,0);
}

int CListBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CDialogBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	m_ListDlg.Create(IDD_DIALOG_LIST,this); 
	m_ListDlg.ShowWindow(SW_SHOW); 
	return 0;
}
