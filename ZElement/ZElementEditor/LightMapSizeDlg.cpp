// CLightMapSizeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementEditor.h"
#include "LightMapSizeDlg.h"

#define SET_LIGHT_MAP_PASSWORD "IMBOSS"

// CLightMapSizeDlg dialog

IMPLEMENT_DYNAMIC(CLightMapSizeDlg, CDialog)

CLightMapSizeDlg::CLightMapSizeDlg(CWnd* pParent /*=NULL*/)
: CDialog(CLightMapSizeDlg::IDD, pParent)
{

}

CLightMapSizeDlg::~CLightMapSizeDlg()
{
}

void CLightMapSizeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cmbSize);
	DDX_Control(pDX, IDC_EDIT1, m_edtPassword);
}


BEGIN_MESSAGE_MAP(CLightMapSizeDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO1, &CLightMapSizeDlg::OnCbnSelchangeCombo1)
	ON_BN_CLICKED(IDC_BUTTON_UNLOCK, &CLightMapSizeDlg::OnBnClickedButtonUnlock)
	ON_BN_CLICKED(IDOK, &CLightMapSizeDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CLightMapSizeDlg message handlers

void CLightMapSizeDlg::OnCbnSelchangeCombo1()
{
	// TODO: Add your control notification handler code here
}

BOOL CLightMapSizeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_cmbSize.AddString("使用默认值");
	m_cmbSize.AddString("1024 x 1024");
	m_cmbSize.AddString("512 x 512");
	m_cmbSize.AddString("256 x 256");
	m_cmbSize.AddString("128 x 128");
	m_cmbSize.AddString("64 x 64");
	m_cmbSize.AddString("使用顶点光");

	if (m_nSize < 0)
		m_cmbSize.SetCurSel(0);
	else if (m_nSize < 64)
		m_cmbSize.SetCurSel(6);
	else if (m_nSize < 128)
		m_cmbSize.SetCurSel(5);
	else if (m_nSize < 256)
		m_cmbSize.SetCurSel(4);
	else if (m_nSize < 512)
		m_cmbSize.SetCurSel(3);
	else if (m_nSize < 1024)
		m_cmbSize.SetCurSel(2);
	else
		m_cmbSize.SetCurSel(1);

	return TRUE;
}
void CLightMapSizeDlg::OnBnClickedButtonUnlock()
{
	CString strPassword;
	m_edtPassword.GetWindowText(strPassword);
	m_cmbSize.EnableWindow(strPassword == SET_LIGHT_MAP_PASSWORD);
}

void CLightMapSizeDlg::OnBnClickedOk()
{
	CString strPassword;
	m_edtPassword.GetWindowText(strPassword);
	if (strPassword == SET_LIGHT_MAP_PASSWORD)
	{
		switch (m_cmbSize.GetCurSel())
		{
		case 0:
			m_nSize = -1;
			break;
		case 1:
			m_nSize = 1024;
			break;
		case 2:
			m_nSize = 512;
			break;
		case 3:
			m_nSize = 256;
			break;
		case 4:
			m_nSize = 128;
			break;
		case 5:
			m_nSize = 64;
			break;
		case 6:
			m_nSize = 1;
			break;
		}
	}
	OnOK();
}
