// ProcTypeMaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "ProcTypeMaskDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static LPCTSTR _mask_name[14] = {
	_T("死亡时不能掉落"),
	_T("不可以扔在地上"),
	_T("不可以卖给NPC"),
	_T("是人民币物品"),
	_T("不可以玩家间交易"),
	_T("是任务相关物品"),
	_T("装备后绑定"),
	_T("保留，切勿选择"),
	_T("产生GUID"),
	_T("可以回收"),
	_T("跨服可交易"),
	_T("仅在跨服可用"),
	_T("跨服无法使用"),
	_T("不可放入仓库"),
};

BOOL CALLBACK ProcTypeMask::OnActivate(void)
{
	CProcTypeMaskDlg dlg;
	dlg.m_ulProcTypeMask = m_var;
	if (dlg.DoModal() != IDOK) return FALSE;
	m_var = dlg.m_ulProcTypeMask;
	return TRUE;
}

LPCTSTR CALLBACK ProcTypeMask::OnGetShowString(void) const
{
	AString str;
	AVariant var = m_var;
	unsigned long ulMask = var;
	for (unsigned long i = 0; i < sizeof(_mask_name) / sizeof(LPCTSTR); i++)
		if (ulMask & (1 << i))
		{
			str += _mask_name[i];
			str += ";";
		}
	return str;
}

AVariant CALLBACK ProcTypeMask::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK ProcTypeMask::OnSetValue(const AVariant& var)
{
	m_var = var;
}

/////////////////////////////////////////////////////////////////////////////
// CProcTypeMaskDlg dialog


CProcTypeMaskDlg::CProcTypeMaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CProcTypeMaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CProcTypeMaskDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CProcTypeMaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CProcTypeMaskDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CProcTypeMaskDlg, CDialog)
	//{{AFX_MSG_MAP(CProcTypeMaskDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CProcTypeMaskDlg message handlers

BOOL CProcTypeMaskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	for (UINT i = 0; i < sizeof(_mask_name) / sizeof(LPCTSTR); i++)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pButton->SetWindowText(_mask_name[i]);
		if (m_ulProcTypeMask & (1 << i))
			pButton->SetCheck(BST_CHECKED);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CProcTypeMaskDlg::OnOK() 
{
	UpdateData();
	m_ulProcTypeMask = 0;

	for (UINT i = 0; i < sizeof(_mask_name) / sizeof(LPCTSTR); i++)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		if (pButton->GetCheck() == BST_CHECKED)
			m_ulProcTypeMask |= 1 << i;
	}

	CDialog::OnOK();
}
