// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "Animator.h"
#include "AnmActionManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
    //{{AFX_MSG_MAP(CMainFrame)
    ON_WM_CREATE()
	ON_WM_ACTIVATEAPP()
    ON_WM_SETFOCUS()
    ON_COMMAND(ID_FILE_NEW, OnFileNew)
    ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
    ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_OBJECT_LIST, OnObjectList)
	ON_UPDATE_COMMAND_UI(ID_OBJECT_LIST, OnUpdateObjectList)
	ON_COMMAND(ID_TRACK_PANEL, OnTrackPanel)
	ON_UPDATE_COMMAND_UI(ID_TRACK_PANEL, OnUpdateTrackPanel)
	ON_COMMAND(ID_FILE_SAVE_AS, OnFileSaveAs)
	ON_COMMAND(ID_SHOW_FPS, OnShowFps)
	ON_UPDATE_COMMAND_UI(ID_SHOW_FPS, OnUpdateShowFps)
	ON_COMMAND(ID_CALCULATE_TOTAL_TIME, OnCalculateTotalTime)
	ON_COMMAND(ID_JUMP_BTN, OnFrameJump)
	ON_UPDATE_COMMAND_UI(ID_JUMP_BTN, OnUpdateFrameJump)
	ON_WM_CLOSE()
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
    m_pWndView = new CAnimationEditorView();
    m_bFullWindow = false;
}

CMainFrame::~CMainFrame()
{
    delete m_pWndView;
}

CAnimationEditorView* CMainFrame::GetAnimationView()
{
    return m_pWndView;
}

CObjectList* CMainFrame::GetObjectList()
{
    return (CObjectList*)m_wndObjectBar.GetView(RUNTIME_CLASS(CObjectList));
}

CBezierList* CMainFrame::GetBezierList()
{
    return (CBezierList*)m_wndObjectBar.GetView(RUNTIME_CLASS(CBezierList));
}

CTrackPanel* CMainFrame::GetTrackPanel()
{
    return (CTrackPanel*)m_wndTrackBar.GetActiveView();
}

void CMainFrame::SwitchWindow()
{
    m_bFullWindow = !m_bFullWindow;
    if (m_bFullWindow)
    {
        ShowControlBar(&m_wndObjectBar, FALSE, FALSE);
        ShowControlBar(&m_wndTrackBar, FALSE, FALSE);
    }
    else
    {
        ShowControlBar(&m_wndObjectBar, TRUE, FALSE);
        ShowControlBar(&m_wndTrackBar, TRUE, FALSE);
    }
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
        return -1;
    
    if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
        | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
        !m_wndToolBar.LoadToolBar(IDR_MAIN_TOOLBAR))
    {
        TRACE0("Failed to create toolbar\n");
        return -1;      // fail to create
    }
    
	CRect rect;
	m_wndToolBar.SetButtonInfo(43,ID_JUMP_TIP,TBBS_SEPARATOR,100);
	m_wndToolBar.GetItemRect(43,&rect);
	m_TxtJump.Create(_T("跳转到改帧："),WS_VISIBLE|WS_TABSTOP,rect,&m_wndToolBar,ID_JUMP_TIP);
	m_wndToolBar.SetButtonInfo(44,ID_JUMP_EDIT,TBBS_SEPARATOR,100);
	m_wndToolBar.GetItemRect(44,&rect);
	m_EdtJump.Create(WS_VISIBLE|WS_TABSTOP | ES_NUMBER,rect,&m_wndToolBar,ID_JUMP_EDIT);
	m_wndToolBar.SetButtonInfo(45,ID_JUMP_BTN,TBBS_SEPARATOR,100);
	m_wndToolBar.GetItemRect(45,&rect);
	m_BtnJump.Create(_T("跳转"),WS_CHILD | WS_VISIBLE|WS_TABSTOP | BS_FLAT | BS_PUSHBUTTON,rect,&m_wndToolBar,ID_JUMP_BTN);
    if (!m_wndStatusBar.Create(this) ||
        !m_wndStatusBar.SetIndicators(indicators,
          sizeof(indicators)/sizeof(UINT)))
    {
        TRACE0("Failed to create status bar\n");
        return -1;      // fail to create
    }

    // TODO: Delete these three lines if you don't want the toolbar to
    //  be dockable
    m_wndToolBar.EnableDocking(CBRS_ORIENT_HORZ);
    EnableDocking(CBRS_ALIGN_ANY);
    DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_BOTTOM);

    // Create image list.
    m_ImageList.Create(IDB_BITMAP_SYS, 13, 1, RGB(0,255,0));

	// Set Object Bar
    m_wndObjectBar.SetBtnImageList(&m_ImageList);
    if (!m_wndObjectBar.Create(this, ID_OBJECT_LIST,
        _T("对象列表"), CSize(224, 500), WS_CHILD|WS_VISIBLE|CBRS_ALIGN_LEFT))
    {
        TRACE0("Failed to create dialog bar m_wndObjectBar\n");
        return -1;		// fail to create
	}
	m_wndObjectBar.AddView(_T("对象列表"), RUNTIME_CLASS(CObjectList));
    m_wndObjectBar.AddView(_T("路径列表"), RUNTIME_CLASS(CBezierList));
    m_wndObjectBar.CalcFixedLayout(false, false);
    m_wndObjectBar.SetBarStyle(m_wndObjectBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    m_wndObjectBar.EnableDocking(CBRS_ALIGN_LEFT);
    EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndObjectBar, AFX_IDW_DOCKBAR_LEFT);

    // Set Track Bar
    m_wndTrackBar.SetBtnImageList (&m_ImageList);
    if (!m_wndTrackBar.Create(this, ID_TRACK_PANEL,
        _T("事件时间轴"), CSize(1280, 308), WS_CHILD|WS_VISIBLE|CBRS_ALIGN_BOTTOM))
    {
        TRACE0("Failed to create dialog bar m_wndTrackBar\n");
        return -1;		// fail to create
    }
    m_wndTrackBar.AddView(_T("事件时间轴"),  RUNTIME_CLASS(CTrackPanel));
    m_wndTrackBar.CalcFixedLayout(false, true);
    m_wndTrackBar.SetBarStyle(m_wndTrackBar.GetBarStyle() |
        CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC);
    m_wndTrackBar.EnableDocking(CBRS_ALIGN_BOTTOM);
    EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndTrackBar, AFX_IDW_DOCKBAR_BOTTOM);


    // create a view to occupy the client area of the frame
    if (!m_pWndView->Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
        CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
    {
        TRACE0("Failed to create view window\n");
        return -1;
    }

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
    m_pWndView->SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
    // let the view have first crack at the command
    if (m_pWndView->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
        return TRUE;

    // otherwise, do default handling
    return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnFileNew() 
{
    // TODO: Add your command handler code here
    theApp.OnFileNew();
}

void CMainFrame::OnFileOpen() 
{
    // TODO: Add your command handler code here
    theApp.OnFileOpen();
}

void CMainFrame::OnFileSave()
{
    theApp.OnFileSave();
}

void CMainFrame::OnFileSaveAs() 
{
    // TODO: Add your command handler code here
    theApp.OnFileSaveAs();
}

void CMainFrame::OnObjectList() 
{
	// TODO: Add your command handler code here
    if (m_wndObjectBar.IsVisible())
        ShowControlBar(&m_wndObjectBar, FALSE, FALSE);
    else
		ShowControlBar(&m_wndObjectBar, TRUE, FALSE);
}

void CMainFrame::OnUpdateObjectList(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_wndObjectBar.IsVisible())
        pCmdUI->SetCheck(TRUE);
    else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnTrackPanel() 
{
	// TODO: Add your command handler code here
    if (m_wndTrackBar.IsVisible())
        ShowControlBar(&m_wndTrackBar, FALSE, FALSE);
    else
		ShowControlBar(&m_wndTrackBar, TRUE, FALSE);
}

void CMainFrame::OnUpdateTrackPanel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
    if (m_wndTrackBar.IsVisible())
        pCmdUI->SetCheck(TRUE);
    else
		pCmdUI->SetCheck(FALSE);
}

void CMainFrame::OnShowFps() 
{
	// TODO: Add your command handler code here
	if (g_pA3DConfig->RT_GetShowFPSFlag())
        g_pA3DConfig->RT_SetShowFPSFlag(false);
    else
        g_pA3DConfig->RT_SetShowFPSFlag(true);
}

void CMainFrame::OnUpdateShowFps(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if (g_pA3DConfig->RT_GetShowFPSFlag())
        pCmdUI->SetCheck(TRUE);
    else
        pCmdUI->SetCheck(FALSE);
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
    if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE)
    {
        return true;
    }

	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pw = CWnd::GetFocus();
		if(pw == &m_EdtJump)
		{
			OnFrameJump();
		    return true;
		}
		
	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::OnCalculateTotalTime() 
{
	// TODO: Add your command handler code here
    CAnmActionManager *pActionManager = g_Animator.GetActionManager();
    if (pActionManager)
    {
        ACString str;
        str.Format(_T("动画总时间: %.3f秒"), pActionManager->CalculateTotalTime());
	    MessageBox(str, _T("动画总时间"), MB_ICONEXCLAMATION | MB_OK);
    }
}

void CMainFrame::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
    bool res = theApp.AskSaveWork();
	
	if (res)
    {
        CFrameWnd::OnClose();
    }
}

#ifdef BACK_VERSION
void CMainFrame::OnActivateApp( BOOL bActive, HTASK task )
{
	CFrameWnd::OnActivateApp(bActive, task);
	
	theApp.m_bAppActive = bActive;
}
#else
void CMainFrame::OnActivateApp(BOOL bActive, DWORD dwThreadID)
{
	CFrameWnd::OnActivateApp(bActive, dwThreadID);

	// TODO: Add your message handler code here
	theApp.m_bAppActive = bActive;
}
#endif 

void CMainFrame::OnFrameJump()
{
	CString strText;
	m_EdtJump.GetWindowText(strText);
	int frame = _ttoi(strText);
	m_EdtJump.SetWindowText(_T(""));
	if(frame<1) return;
	GetTrackPanel()->JumpToFrame((frame-1)*50);
}
void CMainFrame::OnUpdateFrameJump(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(g_Animator.GetState() != CAnimator::STATE_PLAYING);
}