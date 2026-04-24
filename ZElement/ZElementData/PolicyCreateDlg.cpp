// PolicyCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementData.h"
#include "PolicyCreateDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPolicyCreateDlg dialog


CPolicyCreateDlg::CPolicyCreateDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPolicyCreateDlg::IDD, pParent), m_bOverseasEnable(false) //Added 2011-04-20.
{
	//{{AFX_DATA_INIT(CPolicyCreateDlg)
	m_szPolicyName = _T("");
	m_ulOverseasPolicyID = 0;
	//}}AFX_DATA_INIT
}


void CPolicyCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolicyCreateDlg)
	DDX_Text(pDX, IDC_EDIT_NAME, m_szPolicyName);
	DDX_Text(pDX, IDC_OVERSEAS_POLICY_ID, m_ulOverseasPolicyID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolicyCreateDlg, CDialog)
	//{{AFX_MSG_MAP(CPolicyCreateDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolicyCreateDlg message handlers

void CPolicyCreateDlg::OnOK()
{
	UpdateData(true);
	if(m_szPolicyName.IsEmpty())
	{
		MessageBox("策略的名字不能为空!");
		return;
	}
	CDialog::OnOK();
}

BOOL CPolicyCreateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// 添加与海外ID预留相关的代码 Added 2011-04-20.
	GetDlgItem( IDC_OVERSEAS_POLICY_ID_LABEL )->EnableWindow( m_bOverseasEnable );
	GetDlgItem( IDC_OVERSEAS_POLICY_ID )->EnableWindow( m_bOverseasEnable );
	//Added end.

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
