// DynamicObjectSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "NpcControllerSelDlg.h"
#include "NpcControllerManDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDynamicObjectSelDlg dialog


CNpcContollerSelDlg::CNpcContollerSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcContollerSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynamicObjectSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nDyObjID = 0;
}


void CNpcContollerSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicObjectSelDlg)
	DDX_Control(pDX, IDC_LIST_DY_OBJ, m_listObj);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcContollerSelDlg, CDialog)
	//{{AFX_MSG_MAP(CDynamicObjectSelDlg)
	ON_BN_CLICKED(ID_SET_NULL, OnSetNull)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicObjectSelDlg message handlers


BOOL CNpcContollerSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	int n = g_ControllerObjectMan.GetIdNum();
	int index = 0;
	
	int sel = m_listObj.InsertString(0,"[0]--None--");
	m_listObj.SetItemData(sel,0);
	int idx = 1;
	for( int i = 0; i < n; ++i)
	{
		CONTROLLER_OBJECT obj;
		InitControllerObj(&obj);
		g_ControllerObjectMan.GetObjByIndex(i,obj);
		if(obj.id!=0)
		{
			char name[256];
			sprintf(name,"[%d]%s",obj.id,obj.szName);
			sel = m_listObj.InsertString(idx++,name);
			m_listObj.SetItemData(sel,obj.id);
			if(obj.id == m_nDyObjID) index = sel;
		}
	}
	if(index!=-1) m_listObj.SetCurSel(index);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcContollerSelDlg::OnOK()
{
	int sel = m_listObj.GetCurSel();
	if(sel==-1) CDialog::OnCancel();
	m_nDyObjID = m_listObj.GetItemData(sel);

	CDialog::OnOK();
}

void CNpcContollerSelDlg::OnSetNull() 
{
	// TODO: Add your control notification handler code here
	m_nDyObjID = 0;
	CDialog::OnOK();
}
