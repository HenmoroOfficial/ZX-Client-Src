// MainFrm.cpp : implementation of the CMainFrame class
//

#include "Global.h"
#include "ModelView.h"
#include "PathViewDlg.h"

#include "MainFrm.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_VIEW_MODEL_PATH, OnViewModelPath)
	ON_COMMAND(ID_USE_DEFUALT_CAM_POS, OnUseDefualtCamPos)
	ON_UPDATE_COMMAND_UI(ID_USE_DEFUALT_CAM_POS, OnUpdateUseDefualtCamPos)
	ON_WM_ACTIVATEAPP()
	ON_WM_NCHITTEST()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	// TODO: add member initialization code here
	m_bCreated = false;
	m_bActive = false;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	// create a view to occupy the client area of the frame
	if (!m_wndView.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	//SetWindowLong(m_hWnd,GWL_STYLE,GetWindowLong(m_hWnd,GWL_STYLE)&~WS_MAXIMIZEBOX);
	/*
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}*/
	
	
	
	m_wndAnimationBar.Create(this,IDD_DIALOG_ANIMATION_BAR,WS_CHILD | WS_VISIBLE | 
		CBRS_BOTTOM| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
		CBRS_SIZE_DYNAMIC,IDD_DIALOG_ANIMATION_BAR);

	m_wndCharactorBar.Create(this,IDD_DIALOG_CHARACTER,CBRS_ALIGN_RIGHT,IDD_DIALOG_CHARACTER);
	
	m_wndListBar.Create(this,IDD_DIALOG_LIST_BAR,WS_CHILD | WS_VISIBLE | 
		CBRS_LEFT| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY |
		CBRS_SIZE_DYNAMIC,IDD_DIALOG_LIST_BAR);

	// TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	//m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	m_wndAnimationBar.EnableDocking(CBRS_ALIGN_BOTTOM|CBRS_ALIGN_TOP);
	m_wndCharactorBar.EnableDocking(CBRS_ALIGN_LEFT|CBRS_ALIGN_RIGHT);
	
	//DockControlBar(&m_wndToolBar);
	DockControlBar(&m_wndAnimationBar);
	DockControlBar(&m_wndCharactorBar);

	//Part dialog creating
	m_wndPartDlg.Create(IDD_DIALOG_PART,this);
	//m_wndPartDlg.ShowWindow(SW_SHOW);
	//m_wndPartDlg.InitFilter();
	m_wndPartDlg.ShowWindow(SW_HIDE);
	m_wndCharactorBar.ShowWindow(SW_HIDE);
	
	m_wndAnimationBar.m_SpeedCtrl.SetRange(0,20);
	m_wndAnimationBar.m_SpeedCtrl.SetPos((int)(g_fPlaySpeed*10));
	m_wndCharactorBar.Init();
	m_bCreated = true;
	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndView.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndView.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}


BOOL CMainFrame::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	return CFrameWnd::OnMouseWheel(nFlags, zDelta, pt);
}

void CMainFrame::OnViewModelPath() 
{
	// TODO: Add your command handler code here
	CPathViewDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUseDefualtCamPos() 
{
	// TODO: Add your command handler code here
	g_bUseDefaultCamPos = !g_bUseDefaultCamPos;
}

void CMainFrame::OnUpdateUseDefualtCamPos(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!g_bUseDefaultCamPos);
}

LRESULT CMainFrame::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CFrameWnd::DefWindowProc(message, wParam, lParam);
}

void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
{
	CFrameWnd::OnActivateApp(bActive, hTask);
	
	// TODO: Add your message handler code here
	m_bActive = bActive ? true : false;
}

LRESULT CMainFrame::OnNcHitTest(CPoint point)
{
	if(CWnd::OnNcHitTest(point)==HTRIGHT
		||CWnd::OnNcHitTest(point)==HTLEFT
		||CWnd::OnNcHitTest(point)==HTTOP
		||CWnd::OnNcHitTest(point)==HTBOTTOM
		||CWnd::OnNcHitTest(point)==HTTOPLEFT
		||CWnd::OnNcHitTest(point)==HTTOPRIGHT
		||CWnd::OnNcHitTest(point)==HTBOTTOMLEFT
		||CWnd::OnNcHitTest(point)==HTBOTTOMRIGHT)
		return HTCLIENT;
	return CWnd::OnNcHitTest(point);
}