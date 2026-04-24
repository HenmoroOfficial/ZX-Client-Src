// SetTimeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "SetTimeDlg.h"
#include "Animator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSetTimeDlg dialog


CSetTimeDlg::CSetTimeDlg(CWnd* pParent)
	: CDialog(CSetTimeDlg::IDD, pParent)
{
 //   m_dwTotalTime = g_Animator.GetTotalTime() / 50;
	//{{AFX_DATA_INIT(CSetTimeDlg)
	//}}AFX_DATA_INIT
}


void CSetTimeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSetTimeDlg)
	DDX_Text(pDX, IDC_SET_TIME, m_dwTotalTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSetTimeDlg, CDialog)
	//{{AFX_MSG_MAP(CSetTimeDlg)
	ON_BN_CLICKED(IDC_BUTTON_SET_TIME_OK, OnButtonSetTimeOk)
	ON_BN_CLICKED(IDC_BUTTON_SET_TIME_CANCEL, OnButtonSetTimeCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSetTimeDlg message handlers

void CSetTimeDlg::OnButtonSetTimeOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    g_Animator.Stop();
    g_Animator.SetTotalTime(m_dwTotalTime * 50);
    theApp.GetTrackPanel()->SetSeconds((m_dwTotalTime + 19) / 20);
    theApp.OnChanged();
	EndDialog(0);
}

void CSetTimeDlg::OnButtonSetTimeCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
