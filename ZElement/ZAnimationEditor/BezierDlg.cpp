// BezierDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Animationeditor.h"
#include "BezierDlg.h"
#include "AnmBezier.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierDlg dialog


CBezierDlg::CBezierDlg(CWnd* pParent, CAnmBezier *pBezier)
	: CDialog(CBezierDlg::IDD, pParent)
{
    m_pBezier = pBezier;
    m_uiID = m_pBezier->GetID();
    m_sDisplayName = m_pBezier->GetDisplayName();
    m_bVisable = m_pBezier->IsVisable();
	//{{AFX_DATA_INIT(CBezierDlg)
	//}}AFX_DATA_INIT
}


void CBezierDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBezierDlg)
	DDX_Text(pDX, IDC_BEZIER_ID, m_uiID);
	DDX_Text(pDX, IDC_BEZIER_NAME, m_sDisplayName);
	DDX_Check(pDX, IDC_BEZIER_VISABLE, m_bVisable);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBezierDlg, CDialog)
	//{{AFX_MSG_MAP(CBezierDlg)
	ON_BN_CLICKED(IDC_BUTTON_BEZIER_OK, OnButtonBezierOk)
	ON_BN_CLICKED(IDC_BUTTON_BEZIER_CANCEL, OnButtonBezierCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBezierDlg message handlers

void CBezierDlg::OnButtonBezierOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    m_pBezier->SetDisplayName(ACString(m_sDisplayName));
    m_pBezier->SetVisable(m_bVisable ? true : false);
    EndDialog(0);
}

void CBezierDlg::OnButtonBezierCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(-1);
}
