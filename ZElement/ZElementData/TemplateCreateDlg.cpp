// TemplateCreateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "ElementData.h"
#include "TemplateCreateDlg.h"
#include "TemplIDSelDlg.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CTemplateCreateDlg dialog


CTemplateCreateDlg::CTemplateCreateDlg(bool bOverseasEnable /*= false*/,CWnd* pParent /*=NULL*/)
	: CDialog(CTemplateCreateDlg::IDD, pParent), m_bOverseasEnable(bOverseasEnable) //Added 2011-04-19.
{
	//{{AFX_DATA_INIT(CTemplateCreateDlg)
	m_strName = _T("");
	m_strSrcTempl = _T("");
	m_ulTemplID = 0;
	//}}AFX_DATA_INIT
}


void CTemplateCreateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTemplateCreateDlg)
	DDX_Control(pDX, IDC_EDIT_TEMPLATE_NAME, m_EditTmplName);
	DDX_Text(pDX, IDC_EDIT_TEMPLATE_NAME, m_strName);
	DDX_Text(pDX, IDC_SRC_TEMPL, m_strSrcTempl);
	DDX_Text(pDX, IDC_TEMPL_ID, m_ulTemplID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTemplateCreateDlg, CDialog)
	//{{AFX_MSG_MAP(CTemplateCreateDlg)
	ON_BN_CLICKED(IDC_SEL_COPY, OnSelCopy)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTemplateCreateDlg message handlers

void CTemplateCreateDlg::OnSelCopy() 
{
	CTemplIDSelDlg dlg;
	dlg.m_strRoot = m_strPath;
	dlg.m_strExt = ".tmpl";
	UpdateData();

	if (dlg.DoModal() == IDOK)
	{
		m_strSrcTempl = dlg.m_strFileName;
		UpdateData(FALSE);
	}
}

BOOL CTemplateCreateDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//添加与海外预留ID相关的控制代码 Added 2011-04-19.
	if ( m_bOverseasEnable )
	{
		//启动了海外预留ID，则输入模板ID的输入框不置灰
		GetDlgItem( IDC_TEMPL_ID_LABEL )->EnableWindow( TRUE );
		GetDlgItem( IDC_TEMPL_ID )->EnableWindow( TRUE );
	}
	else
	{
		//启动了海外预留ID，则输入模板ID的输入框需要置灰
		GetDlgItem( IDC_TEMPL_ID_LABEL )->EnableWindow( FALSE );
		GetDlgItem( IDC_TEMPL_ID )->EnableWindow( FALSE );
	}
	//Added end.

	m_EditTmplName.SetFocus();
	return FALSE;
}
