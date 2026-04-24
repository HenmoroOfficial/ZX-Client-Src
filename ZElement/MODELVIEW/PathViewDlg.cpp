// PathViewDlg.cpp : implementation file
//

#include "Global.h"
#include "modelview.h"
#include "PathViewDlg.h"
#include "vector.h"

#define new A_DEBUG_NEW
/////////////////////////////////////////////////////////////////////////////
// CPathViewDlg dialog


CPathViewDlg::CPathViewDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPathViewDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPathViewDlg)
	m_strPaths = _T("");
	//}}AFX_DATA_INIT
}


void CPathViewDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPathViewDlg)
	DDX_Text(pDX, IDC_EDIT_PATH, m_strPaths);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPathViewDlg, CDialog)
	//{{AFX_MSG_MAP(CPathViewDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPathViewDlg message handlers

BOOL CPathViewDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	abase::vector<AString> pathList;

	if(g_pRenderableObject)
	{
		g_pRenderableObject->GetPathList(pathList);
		for( int i = 0; i < pathList.size(); ++i)
		{
			m_strPaths += pathList[i];
			m_strPaths += "\r\n";
		}
	}
	pathList.clear();
	UpdateData(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
