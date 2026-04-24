// TaskDataSmallToolDlg.cpp : implementation file
//

#include "stdafx.h"
#include "TaskDataSmallTool.h"
#include "TaskDataSmallToolDlg.h"

//===================================================================
#include "TaskTemplMan.h"
//===================================================================

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDataSmallToolDlg dialog

CTaskDataSmallToolDlg::CTaskDataSmallToolDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTaskDataSmallToolDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTaskDataSmallToolDlg)
	m_strResult = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	//===========================================
	m_pTaskTemplMan = NULL;
	//===========================================
}

void CTaskDataSmallToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTaskDataSmallToolDlg)
	DDX_Text(pDX, IDC_EDIT_RESULT, m_strResult);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTaskDataSmallToolDlg, CDialog)
	//{{AFX_MSG_MAP(CTaskDataSmallToolDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_READ_DATA1, OnButtonReadData1)
	ON_BN_CLICKED(IDC_BUTTON_ANALYZE, OnButtonAnalyze)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTaskDataSmallToolDlg message handlers

BOOL CTaskDataSmallToolDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CTaskDataSmallToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CTaskDataSmallToolDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CTaskDataSmallToolDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CTaskDataSmallToolDlg::OnButtonReadData1() 
{
	//
	if ( !m_pTaskTemplMan )
	{
		m_pTaskTemplMan = new ATaskTemplMan;
		if ( !m_pTaskTemplMan )
		{
			AfxMessageBox( "Alloc Mem Failled!" );
			return;
		}

		m_pTaskTemplMan->Init( NULL );

		// Read Task Data from the data pack.
		//CFileDialog fileDlg();
		if ( !m_pTaskTemplMan->LoadTasksFromPack( "data\\tasks.data", "data\\forbidden_task.txt", true ) )
		{
			AfxMessageBox( "Can not load the task data!" );
			return;
		}

		//
		TaskTemplMap& task_map = m_pTaskTemplMan->GetTopTemplMap();//GetAllTemplMap();
		size_t map_size = task_map.size();
		TaskTemplMap::iterator iterMap = task_map.begin();
		ATaskTempl* pCurTaskTempl = NULL;
		int nTestTaskCount = 0;
		CString strResult;
		while ( iterMap != task_map.end() )
		{
			pCurTaskTempl = iterMap->second;
			if ( !pCurTaskTempl )
			{
				++iterMap;
				continue;
			}
			CString strName = pCurTaskTempl->GetName();
			const int nPlayerLevel = 145;
			if ( /*(strName.Find( "测试", 0 ) >= 0) || */
				(pCurTaskTempl->m_ulPremise_Lev_Min < 150 && pCurTaskTempl->m_ulPremise_Lev_Max > 150) /*||
				(pCurTaskTempl->m_ulPremise_Lev_Max && (pCurTaskTempl->m_ulPremise_Lev_Max < nPlayerLevel) )
				!(nPlayerLevel >= pCurTaskTempl->m_ulPremise_Lev_Min && nPlayerLevel <= pCurTaskTempl->m_ulPremise_Lev_Max) ||
				pCurTaskTempl->m_bAutoDeliver*/ )
			{
				CString strItem;
				strItem.Format( "%s(ID:%d)", strName, pCurTaskTempl->GetID() );
				strResult += strItem;
				strResult += L"\r\n";
				nTestTaskCount++;
				/*TaskTemplMap::iterator iterTmp = iterMap;
				++iterMap;
				task_map.erase( iterTmp );*/
//				iterMap = task_map.erase( iterMap );
//				delete pCurTaskTempl;
			}
//			else
			{
				++iterMap;
			}
		}

		m_strResult = strResult;
		UpdateData( FALSE );
		CString strNum;
		strNum.Format( "符合处理条件的任务模板总数是：%d", nTestTaskCount );
		AfxMessageBox( strNum );

		//Release
		if ( m_pTaskTemplMan )
		{
			m_pTaskTemplMan->Release();
		}
		delete m_pTaskTemplMan;
		m_pTaskTemplMan = NULL;
	}
}

void CTaskDataSmallToolDlg::OnButtonAnalyze() 
{
		
}
