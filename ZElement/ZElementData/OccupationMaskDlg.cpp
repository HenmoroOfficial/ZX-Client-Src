// OccupationMaskDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "OccupationMaskDlg.h"

#ifdef _DEBUG
#define new A_DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


BOOL CALLBACK OccupationMask::OnActivate(void)
{
	COccupationMaskDlg dlg;
	dlg.m_ulOccupationMask = m_var;
	dlg.m_pMaskNames = m_pMaskNames;
	dlg.m_nNameCount = m_nNameCount;
	if (dlg.DoModal() != IDOK) return FALSE;
	m_var = dlg.m_ulOccupationMask;
	return TRUE;
}

LPCTSTR CALLBACK OccupationMask::OnGetShowString(void) const
{
	AString str;
	AVariant var = m_var;
	unsigned __int64 ulMask = var;
	__int64 temp = 1;
	for (unsigned long i = 0; i < m_nNameCount; i++)
		if (ulMask & (temp << i))
		{
			str += m_pMaskNames[i];
			str += ";";
		}
	return str;
}


AVariant CALLBACK OccupationMask::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK OccupationMask::OnSetValue(const AVariant& var)
{
	m_var = var;
}

/////////////////////////////////////////////////////////////////////////////
// COccupationMaskDlg dialog


COccupationMaskDlg::COccupationMaskDlg(CWnd* pParent /*=NULL*/)
	: CDialog(COccupationMaskDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(COccupationMaskDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void COccupationMaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(COccupationMaskDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(COccupationMaskDlg, CDialog)
	//{{AFX_MSG_MAP(COccupationMaskDlg)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SELECTALL, OnButtonSelectAll)
	ON_BN_CLICKED(IDC_UNSELECTALL, OnButtonUnselectAll)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COccupationMaskDlg message handlers

void COccupationMaskDlg::OnOK() 
{
	UpdateData();
	m_ulOccupationMask = 0;

	__int64 temp = 1;
	for (UINT i = 0; i < 64; i++)
	{
		if (i >= m_nNameCount) break;
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		if (pButton->GetCheck() == BST_CHECKED)
			m_ulOccupationMask |= temp << i;
	}
	CDialog::OnOK();
}

BOOL COccupationMaskDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	__int64 temp = 1;
	for (UINT i = 0; i < 64; i++)
	{
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		if (i >= m_nNameCount)
			pButton->ShowWindow(SW_HIDE);
		else
		{
			pButton->SetWindowText(m_pMaskNames[i]);		
			if (m_ulOccupationMask & (temp << i))
				pButton->SetCheck(BST_CHECKED);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void COccupationMaskDlg::OnButtonSelectAll()
{
	__int64 temp = 1;
	for (UINT i = 0; i < 64; i++)
	{
		if (i >= m_nNameCount) break;
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pButton->SetCheck(BST_CHECKED);
		m_ulOccupationMask |= temp << i;
	}
}

void COccupationMaskDlg::OnButtonUnselectAll()
{
	__int64 temp = 1;
	for (UINT i = 0; i < 64; i++)
	{
		if (i >= m_nNameCount) break;
		CButton* pButton = (CButton*)GetDlgItem(IDC_CHECK1 + i);
		pButton->SetCheck(BST_UNCHECKED);
		m_ulOccupationMask &= ~(temp << i);
	}
}
