// SelectTransDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "SelectTransDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg dialog


CSelectTransDlg::CSelectTransDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSelectTransDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelectTransDlg)
	m_strOrg = _T("");
	m_strTrans1 = _T("");
	m_strTrans2 = _T("");
	m_strOrg2 = _T("");
	//}}AFX_DATA_INIT
	m_bApplyAll = false;
    m_bSelOne = true; 
	m_bReplaceOrg = false;
}


void CSelectTransDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectTransDlg)
	DDX_Text(pDX, IDC_EDIT_ORGTEXT, m_strOrg);
	DDX_Text(pDX, IDC_EDIT_TRANS1, m_strTrans1);
	DDX_Text(pDX, IDC_EDIT_TRANS2, m_strTrans2);
	DDX_Text(pDX, IDC_EDIT_ORGTEXT2, m_strOrg2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectTransDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectTransDlg)
	ON_BN_CLICKED(IDC_RADIO_ONE, OnRadioOne)
	ON_BN_CLICKED(IDC_RADIO_TWO, OnRadioTwo)
	ON_BN_CLICKED(IDC_CHECK_APPLYALL, OnCheckApplyall)
	ON_BN_CLICKED(IDC_CHECK_ORG, OnReplaceOrg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectTransDlg message handlers

BOOL CSelectTransDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_ONE);
	pButton->SetCheck(m_bSelOne);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_TWO);
	pButton->SetCheck(!m_bSelOne);
	GetDlgItem(IDC_CHECK_ORG)->EnableWindow(!m_bSelOne);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSelectTransDlg::OnRadioOne() 
{
	// TODO: Add your control notification handler code here
	m_bSelOne = true;
	
	UpdateCtrls();
}

void CSelectTransDlg::OnRadioTwo() 
{
	// TODO: Add your control notification handler code here
	m_bSelOne = false;
	
	UpdateCtrls();
}

void CSelectTransDlg::UpdateCtrls()
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_RADIO_ONE);
	pButton->SetCheck(m_bSelOne);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_TWO);
	pButton->SetCheck(!m_bSelOne);

	pButton = (CButton *)GetDlgItem(IDC_CHECK_ORG);
	m_bReplaceOrg &= !m_bSelOne;
	pButton->SetCheck(m_bReplaceOrg);
	pButton->EnableWindow(!m_bSelOne);
}

void CSelectTransDlg::OnCheckApplyall() 
{
	// TODO: Add your control notification handler code here
	m_bApplyAll = true;
}

void CSelectTransDlg::OnReplaceOrg()
{
	m_bReplaceOrg = (BST_CHECKED == ((CButton*)GetDlgItem(IDC_CHECK_ORG))->GetCheck());
}

void CSelectTransDlg::SetData(CString org1, CString org2, CString trans1, CString trans2)
{
	m_strOrg = org1;
	m_strOrg2 = org2;
	m_strTrans1 = trans1;
	m_strTrans2 = trans2;
}
