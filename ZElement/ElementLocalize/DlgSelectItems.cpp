// DlgSelectItems.cpp : implementation file
//

#include "stdafx.h"
#include "elementlocalize.h"
#include "DlgSelectItems.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectItems dialog


CDlgSelectItems::CDlgSelectItems(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgSelectItems::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDlgSelectItems)
	for (int i=0; i<CHUNK_NUM; ++i)
		m_bDataSwitch[i] = FALSE;
	m_csLabel = _T("default");
	m_bCKEnableLabel = FALSE;

	m_bEnableLabel = TRUE;
	//}}AFX_DATA_INIT
}


void CDlgSelectItems::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDlgSelectItems)
	DDX_Check(pDX, IDC_ADDON_STRING, m_bDataSwitch[CHUNK_ELEMENT_ADDONSTR]);
	DDX_Check(pDX, IDC_BUFF_DESC, m_bDataSwitch[CHUNK_ELEMENT_BUFFDESC]);
	DDX_Check(pDX, IDC_FIX_MSG, m_bDataSwitch[CHUNK_ELEMENT_FIXMSG]);
	DDX_Check(pDX, IDC_GSHOP, m_bDataSwitch[CHUNK_ELEMENT_GSHOP]);
	DDX_Check(pDX, IDC_GSHOP_TYPES, m_bDataSwitch[CHUNK_ELEMENT_GSHOP_TYPES]);
	DDX_Check(pDX, IDC_GSHOP1, m_bDataSwitch[CHUNK_ELEMENT_GSHOP1]);
	DDX_Check(pDX, IDC_GSHOP1_TYPES, m_bDataSwitch[CHUNK_ELEMENT_GSHOP_TYPES1]);
	DDX_Check(pDX, IDC_GSHOP2, m_bDataSwitch[CHUNK_ELEMENT_GSHOP2]);
	DDX_Check(pDX, IDC_GSHOP2_TYPES, m_bDataSwitch[CHUNK_ELEMENT_GSHOP_TYPES2]);
	DDX_Check(pDX, IDC_HELP_SCRIPT, m_bDataSwitch[CHUNK_ELEMENT_HELPSCRIPT]);
	DDX_Check(pDX, IDC_HELP_TEXT, m_bDataSwitch[CHUNK_ELEMENT_HELPTEXT]);
	DDX_Check(pDX, IDC_INSTANCE_TEXT, m_bDataSwitch[CHUNK_ELEMENT_INSTANCE_TEXT]);
	DDX_Check(pDX, IDC_INTERFACE_HINT, m_bDataSwitch[CHUNK_ELEMENT_INTERFACEHINT]);
	DDX_Check(pDX, IDC_INTERFACE_STRING, m_bDataSwitch[CHUNK_ELEMENT_INTERFACESTRING]);
	DDX_Check(pDX, IDC_ITEM_DESC, m_bDataSwitch[CHUNK_ELEMENT_ITEMDESC]);
	DDX_Check(pDX, IDC_ITEM_DESC_EXT, m_bDataSwitch[CHUNK_ELEMENT_ITEMEXDESC]);
	DDX_Check(pDX, IDC_LOADING_HINT, m_bDataSwitch[CHUNK_ELEMENT_LOADINGHINT]);
	DDX_Check(pDX, IDC_PET_TALK, m_bDataSwitch[CHUNK_ELEMENT_PETTALK]);
	DDX_Check(pDX, IDC_POLICY, m_bDataSwitch[CHUNK_ELEMENT_POLICY]);
	DDX_Check(pDX, IDC_PRECINCT, m_bDataSwitch[CHUNK_ELEMENT_PRECINCT]);
	DDX_Check(pDX, IDC_SERVER_ERROR, m_bDataSwitch[CHUNK_ELEMENT_SERVERERR]);
	DDX_Check(pDX, IDC_SKILL_DESC, m_bDataSwitch[CHUNK_ELEMENT_SKILLDESC]);
	DDX_Check(pDX, IDC_TASK, m_bDataSwitch[CHUNK_ELEMENT_TASK]);
	DDX_Check(pDX, IDC_TASK_ERROR, m_bDataSwitch[CHUNK_ELEMENT_TASKERR]);
	DDX_Check(pDX, IDC_TEMPLATE, m_bDataSwitch[CHUNK_ELEMENT_DATA]);
	DDX_Check(pDX, IDC_SCRIPT, m_bDataSwitch[CHUNK_ELEMENT_TEXTFILE]);
	DDX_Check(pDX, IDC_HOMETOWN, m_bDataSwitch[CHUNK_ELEMENT_HOMETOWN]);
	DDX_Check(pDX, IDC_DOMAIN_INFO, m_bDataSwitch[CHUNK_ELEMENT_DOMAININFOR]);
	DDX_Check(pDX, IDC_DOMAIN_INFO2, m_bDataSwitch[CHUNK_ELEMENT_DOMAININFOR1]);
	DDX_Check(pDX, IDC_SELECT_ALL, m_bSelectAll);
	DDX_Check(pDX, IDC_CK_LABEL, m_bCKEnableLabel);
	DDX_Text(pDX, IDC_EDIT_LABEL, m_csLabel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDlgSelectItems, CDialog)
	//{{AFX_MSG_MAP(CDlgSelectItems)
	ON_BN_CLICKED(IDC_SELECT_ALL, OnSelectAll)
	ON_BN_CLICKED(IDC_CK_LABEL, OnCkLabel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDlgSelectItems message handlers

void CDlgSelectItems::OnOK() 
{
	UpdateData(true);	
	CDialog::OnOK();
}

void CDlgSelectItems::OnSelectAll() 
{
	UpdateData(true);
	for (int i=0; i<CHUNK_NUM; ++i)
		m_bDataSwitch[i] = m_bSelectAll;
	UpdateData(false);
}

void CDlgSelectItems::OnCkLabel() 
{
	UpdateData(true);
	if(m_bCKEnableLabel)
		GetDlgItem(IDC_EDIT_LABEL)->EnableWindow(true);
	else
		GetDlgItem(IDC_EDIT_LABEL)->EnableWindow(false);
	
}

BOOL CDlgSelectItems::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_bEnableLabel)
	{
		GetDlgItem(IDC_EDIT_LABEL)->ShowWindow(SW_NORMAL);
		GetDlgItem(IDC_CK_LABEL)->ShowWindow(SW_NORMAL);
	}
	else
	{
		GetDlgItem(IDC_EDIT_LABEL)->ShowWindow(SW_HIDE);
		GetDlgItem(IDC_CK_LABEL)->ShowWindow(SW_HIDE);
	}
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CDlgSelectItems::GetDataSwitch(int datachunk)
{
	if (datachunk <0 || datachunk >= CHUNK_NUM)
		return FALSE;
	return m_bDataSwitch[datachunk];
}