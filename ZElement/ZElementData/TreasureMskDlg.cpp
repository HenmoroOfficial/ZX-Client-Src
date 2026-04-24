// TreasureMskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "TreasureMskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTreasureMskDlg dialog


CTreasureMskDlg::CTreasureMskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreasureMskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreasureMskDlg)
	//}}AFX_DATA_INIT
	m_bCheck[0]= FALSE;
	m_bCheck[1]= FALSE;
	m_bCheck[2]= FALSE;
	m_bCheck[3]= FALSE;
	m_bCheck[4]= FALSE;
	m_bCheck[5]= FALSE;
	m_bCheck[6]= FALSE;
	m_bCheck[7]= FALSE;
	m_byteMask = 0;
}


void CTreasureMskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreasureMskDlg)
	//}}AFX_DATA_MAP
	DDX_Check(pDX, IDC_CHECK1, m_bCheck[0]);
	DDX_Check(pDX, IDC_CHECK2, m_bCheck[1]);
	DDX_Check(pDX, IDC_CHECK3, m_bCheck[2]);
	DDX_Check(pDX, IDC_CHECK4, m_bCheck[3]);
	DDX_Check(pDX, IDC_CHECK5, m_bCheck[4]);
	DDX_Check(pDX, IDC_CHECK6, m_bCheck[5]);
	DDX_Check(pDX, IDC_CHECK7, m_bCheck[6]);
	DDX_Check(pDX, IDC_CHECK8, m_bCheck[7]);
}


BEGIN_MESSAGE_MAP(CTreasureMskDlg, CDialog)
	//{{AFX_MSG_MAP(CTreasureMskDlg)
	ON_BN_CLICKED(IDC_MASK_SEL_ALL, OnMaskSelAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTreasureMskDlg message handlers

BOOL CTreasureMskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	for( int i  = 0;  i < 8; ++i)
		m_bCheck[i] = (m_byteMask & (1 << i)) ? true : false;
	
	
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreasureMskDlg::OnOK()
{
	UpdateData(true);
	
	for( int i  = 0;  i < 8; ++i)
	{
		if(m_bCheck[i]) m_byteMask = (m_byteMask | (1 << i));
		else m_byteMask = (m_byteMask & ~(1 << i));
	}
	
	CDialog::OnOK();
}

void CTreasureMskDlg::OnMaskSelAll() 
{
	// TODO: Add your control notification handler code here
	for( int i = 0; i < 8; ++i)
		m_bCheck[i] = true;

	UpdateData(FALSE);
}
