// GeneralDataItemDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "GeneralDataItemDlg.h"
#include "GeneralDataUIWrapper.h"
#include "PropertyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataItemDlg dialog


CGeneralDataItemDlg::CGeneralDataItemDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralDataItemDlg::IDD, pParent), m_pGeneralDataUIWrapper(NULL), m_pList(NULL),
	m_bDataReadOnly(false)
{
	//{{AFX_DATA_INIT(CGeneralDataItemDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGeneralDataItemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralDataItemDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralDataItemDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralDataItemDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataItemDlg message handlers

BOOL CGeneralDataItemDlg::DestroyWindow() 
{
	// 
	
	return CDialog::DestroyWindow();
}

BOOL CGeneralDataItemDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//
	m_pList = new CPropertyList();

	CWnd *pWnd = GetDlgItem(IDC_LIST_TREASURE);
	CRect rc,rc1;
	pWnd->GetClientRect(&rc);

	int width = rc.Width();
	int length = rc.Height();

	pWnd->GetWindowRect(&rc);
	GetWindowRect(&rc1);
	rc.left = rc.left - rc1.left;
	rc.top = rc.top - rc1.top - 20;
	rc.right = rc.left + width;
	rc.bottom = rc.top + length;

	BOOL bResult = m_pList->Create("±¦Îï", WS_CHILD | WS_VISIBLE, rc, this, 121);

	if ( m_pGeneralDataUIWrapper )
	{
		m_pList->AttachDataObject(m_pGeneralDataUIWrapper->GetDynPropertyObject());
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CGeneralDataItemDlg::OnOK() 
{
	//
	UpdateData(TRUE);
	m_pList->UpdateData(true);
	if ( m_pGeneralDataUIWrapper )
	{
		m_pGeneralDataUIWrapper->UpdateProperty( true );
	}

	CDialog::OnOK();
}
