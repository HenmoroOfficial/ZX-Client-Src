// UpdateLogDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "UpdateLogDlg.h"
#include "Global.h"
#include "VssOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// UpdateLogDlg dialog


UpdateLogDlg::UpdateLogDlg(CWnd* pParent /*=NULL*/)
	: CDialog(UpdateLogDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(UpdateLogDlg)
	m_strUpdateLog = _T("");
	//}}AFX_DATA_INIT
}


void UpdateLogDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(UpdateLogDlg)
	DDX_Text(pDX, IDC_UPDATELOG, m_strUpdateLog);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(UpdateLogDlg, CDialog)
	//{{AFX_MSG_MAP(UpdateLogDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// UpdateLogDlg message handlers

BOOL UpdateLogDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();


	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void UpdateLogDlg::UpdateLog()
{
	CString str = CString(g_szWorkDir);
	str = str + "BaseData\\";
	g_VSS.SetWorkFolder(str);
	g_VSS.GetFile("BaseData\\UpdateLog.txt");
	FILE* fp = fopen(_update_log_path, "r");
	if (!fp)
		return;
	char line[MAX_LINE_LEN];
	CString strText;
	while(ReadLine(fp, line) != 0)
	{
		strText += line;
		strText += "\r\n";
	}
	m_strUpdateLog = strText;
	fclose(fp);
}
