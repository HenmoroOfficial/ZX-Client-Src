// FinderDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "FinderDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFinderDlg dialog


CFinderDlg::CFinderDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFinderDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFinderDlg)
	m_strText = _T("");
	//}}AFX_DATA_INIT
	m_bUp = false;
	m_nSelType = eOrginText;
	m_bMatchWhole = false;
	m_bOnlyNewItem = false;
}


void CFinderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFinderDlg)
	DDX_Text(pDX, IDC_EDIT_TEXT, m_strText);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFinderDlg, CDialog)
	//{{AFX_MSG_MAP(CFinderDlg)
	ON_COMMAND(ID_FIND_TEXT, OnFindText)
	ON_BN_CLICKED(IDC_RADIO_TRANS, OnRadioTrans)
	ON_BN_CLICKED(IDC_RADIO_ORG, OnRadioOrg)
	ON_BN_CLICKED(IDC_RADIO_USE_TEMP_ID, OnRadioUseTempId)
	ON_BN_CLICKED(IDC_RADIO_UP, OnRadioUp)
	ON_BN_CLICKED(IDC_RADIO_DOWN, OnRadioDown)
	ON_BN_CLICKED(IDC_CHECK_MATCH_WHOLE, OnCheckMatchWhole)
	ON_BN_CLICKED(IDC_CHECK_ONLY_NEW_ITEM, OnCheckOnlyNewItem)
	ON_BN_CLICKED(IDC_RADIO_USE_ID, OnRadioUseId)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFinderDlg message handlers

void CFinderDlg::OnFindText() 
{
	// TODO: Add your command handler code here
	
}

BOOL CFinderDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	UpdateButton();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CFinderDlg::UpdateButton()
{
	CButton *pButton = (CButton *)GetDlgItem(IDC_CHECK_MATCH_WHOLE);
	pButton->SetCheck(m_bMatchWhole);
	
	
	pButton = (CButton *)GetDlgItem(IDC_RADIO_ORG);
	if(m_nSelType==eOrginText) pButton->SetCheck(true);
	else pButton->SetCheck(false);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_TRANS);
	if(m_nSelType==eTransText) pButton->SetCheck(true);
	else pButton->SetCheck(false);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_USE_ID);
	if(m_nSelType==eGlobalID) pButton->SetCheck(true);
	else pButton->SetCheck(false);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_USE_TEMP_ID);
	if(m_nSelType==eTempID) pButton->SetCheck(true);
	else pButton->SetCheck(false);

	pButton = (CButton *)GetDlgItem(IDC_RADIO_UP);
	pButton->SetCheck(m_bUp);
	pButton = (CButton *)GetDlgItem(IDC_RADIO_DOWN);
	pButton->SetCheck(!m_bUp);
	pButton = (CButton *)GetDlgItem(IDC_CHECK_ONLY_NEW_ITEM);
	pButton->SetCheck(m_bOnlyNewItem);
	
}

void CFinderDlg::OnRadioTrans() 
{
	// TODO: Add your control notification handler code here
	m_nSelType = eTransText;
	UpdateButton();
}

void CFinderDlg::OnRadioOrg() 
{
	// TODO: Add your control notification handler code here
	m_nSelType = eOrginText;
	UpdateButton();
}

void CFinderDlg::OnRadioUp() 
{
	// TODO: Add your control notification handler code here
	m_bUp = true;
	UpdateButton();
}

void CFinderDlg::OnRadioDown() 
{
	// TODO: Add your control notification handler code here
	m_bUp = false;
	UpdateButton();
}

void CFinderDlg::OnCheckMatchWhole() 
{
	// TODO: Add your control notification handler code here
	m_bMatchWhole = !m_bMatchWhole;
	UpdateButton();
}

void CFinderDlg::OnOK()
{
	UpdateData(true);
	CDialog::OnOK();
}

void CFinderDlg::OnCheckOnlyNewItem() 
{
	// TODO: Add your control notification handler code here
	m_bOnlyNewItem = !m_bOnlyNewItem;
	UpdateButton();
}

void CFinderDlg::OnRadioUseId() 
{
	// TODO: Add your control notification handler code here
	m_nSelType = eGlobalID;
	UpdateButton();
}

void CFinderDlg::OnRadioUseTempId() 
{
	// TODO: Add your control notification handler code here
	m_nSelType = eTempID;
	UpdateButton();
}
