// RenderWnd.cpp : implementation file
//

#include "Global.h"
#include "elementeditor.h"
#include "RenderWnd.h"
#include "Render.h"
#include "ViewFrame.h"
#include "Viewport.h"
#include "PersViewport.h"
#include "OrthoViewport.h"

#include "A3D.h"


//#define new A_DEBUG_NEW

#define TIME_TICKANIMATION	30
#define PACKETDATA	PK_CURSOR | PK_X | PK_Y | PK_BUTTONS |PK_NORMAL_PRESSURE
#define PACKETMODE	0
#include <pktdef.h>

/////////////////////////////////////////////////////////////////////////////
// CRenderWnd

CRenderWnd::CRenderWnd()
{
	m_pViewFrame	= NULL;
	m_pA3DViewport	= NULL;
	m_iActiveView	= -1;

	for (int i=0; i < NUM_VIEWPORT; i++)
		m_aViews[i] = NULL;

	m_hCtx = 0;
	m_pWTMutex = new CMutex( TRUE, NULL, NULL );
}

CRenderWnd::~CRenderWnd()
{
	if(m_pWTMutex) delete m_pWTMutex;
	if( m_hCtx ) WTClose( m_hCtx );
}


BEGIN_MESSAGE_MAP(CRenderWnd, CWnd)
	ON_MESSAGE(WT_PACKET, OnWTPacket)
	//{{AFX_MSG_MAP(CRenderWnd)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_CREATE()
	//ON_WM_MOUSEWHEEL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CRenderWnd message handlers

//	Create window
bool CRenderWnd::Create(const RECT& rc, CWnd* pParent, DWORD dwID)
{
	CString strClassName = AfxRegisterWndClass(CS_VREDRAW | CS_HREDRAW | CS_DBLCLKS,
								LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW)),
							//	(HBRUSH)COLOR_WINDOW);
								(HBRUSH)::GetStockObject(BLACK_BRUSH));
	
	DWORD dwWndStyles = WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS;
	if (!CWnd::CreateEx(0, strClassName, "Render", dwWndStyles,	rc, pParent, dwID, NULL))
		return false;

	m_pViewFrame = (CViewFrame*)pParent;

	return true;
}

//	Initlaize device objects
bool CRenderWnd::InitDeviceObjects()
{
	RECT rcClient;
	GetClientRect(&rcClient);

	//	Create A3D viewport
	if (!g_Render.GetA3DDevice()->CreateViewport(&m_pA3DViewport, 0, 0, 
					rcClient.right, rcClient.bottom, 0.0f, 1.0f, true, true, 0xff808080))
	{
		g_Log.Log("CRenderWnd::InitDeviceObjects: Failed to create A3D viewport.");
		return false;
	}

	//	Create viewports
	m_aViews[VIEW_XY] = new COrthoViewport(COrthoViewport::TYPE_XY);
	m_aViews[VIEW_XZ] = new COrthoViewport(COrthoViewport::TYPE_XZ);
	m_aViews[VIEW_YZ] = new COrthoViewport(COrthoViewport::TYPE_YZ);
	m_aViews[VIEW_PERSPECTIVE] = new CPersViewport;

	for (int i=0; i < NUM_VIEWPORT; i++)
	{
		if (!m_aViews[i]->Init(m_pViewFrame, rcClient.right, rcClient.bottom))
		{
			g_Log.Log("CRenderWnd::InitDeviceObjects: Failed to create viewport %d.", i);
			return false;
		}
	}

	m_iActiveView = VIEW_PERSPECTIVE;

	return true;
}

void CRenderWnd::OnDestroy() 
{
	CWnd::OnDestroy();

	//	Release all viewports
	for (int i=0; i < NUM_VIEWPORT; i++)
	{
		if (m_aViews[i])
		{
			m_aViews[i]->Release();
			delete m_aViews[i];
			m_aViews[i] = NULL;
		}
	}

	A3DRELEASE(m_pA3DViewport);
}

void CRenderWnd::OnSize(UINT nType, int cx, int cy) 
{
	CWnd::OnSize(nType, cx, cy);

	if (g_Render.EngineIsReady())
		g_Render.ResizeDevice(cx, cy);

	//	Adjust viewport
	if (m_pA3DViewport)
	{
		A3DVIEWPORTPARAM Params;
		Params.X		= 0;
		Params.Y		= 0;
		Params.Width	= cx;
		Params.Height	= cy;
		Params.MinZ		= 0.0f;
		Params.MaxZ		= 1.0f;

		m_pA3DViewport->SetParam(&Params);
	}

	for (int i=0; i < NUM_VIEWPORT; i++)
	{
		if (m_aViews[i])
			m_aViews[i]->Resize(cx, cy);
	}
}

BOOL CRenderWnd::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;	
//	return CWnd::OnEraseBkgnd(pDC);
}

//	Tick routine
bool CRenderWnd::FrameMove(DWORD dwDeltaTime)
{
	if (m_iActiveView < 0)
		return true;

	m_aViews[m_iActiveView]->FrameMove(dwDeltaTime);

	//	A3DEngine::TickAnimation trigger animation of many objects.
	//	For example: A3DSky objects, GFX objects etc.
	static DWORD dwAnimTime = 0;
	if ((dwAnimTime += dwDeltaTime) >= TIME_TICKANIMATION)
	{
		dwAnimTime -= TIME_TICKANIMATION;
		g_Render.GetA3DEngine()->TickAnimation();
	}

	return true;
}

//	Render routine
bool CRenderWnd::Render()
{
	if (m_iActiveView < 0)
		return true;

	if (!g_Render.BeginRender(true))
		return false;

	CViewport* pViewport = m_aViews[m_iActiveView];
	pViewport->Render(m_pA3DViewport);

	g_Render.EndRender(true);

	return true;
}

//	Change currently active viewport
bool CRenderWnd::ActivateViewport(int iNewView)
{
	if (m_iActiveView == iNewView)
		return true;

	m_iActiveView = iNewView;

	return true;
}

void CRenderWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT(m_iActiveView >= 0);
	PACKET pkt;
	WTPacketsGet( (HCTX)m_hCtx, 1, &pkt );
	m_aViews[m_iActiveView]->OnLButtonDown(point.x, point.y, pkt.pkNormalPressure);
	
	CWnd::OnLButtonDown(nFlags, point);
}

void CRenderWnd::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnLButtonUp(point.x, point.y, nFlags);
	
	CWnd::OnLButtonUp(nFlags, point);
}

void CRenderWnd::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_iActiveView<0)
	{
		return;
	}
	ASSERT(m_iActiveView >= 0);
	PACKET pkt;
	WTPacketsGet( (HCTX)m_hCtx, 1, &pkt );
	m_aViews[m_iActiveView]->OnMouseMove(point.x, point.y, pkt.pkNormalPressure);
	
	CWnd::OnMouseMove(nFlags, point);
}

void CRenderWnd::OnRButtonDown(UINT nFlags, CPoint point) 
{
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnRButtonDown(point.x, point.y, nFlags);
	
	CWnd::OnRButtonDown(nFlags, point);
}

void CRenderWnd::OnRButtonUp(UINT nFlags, CPoint point) 
{
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnRButtonUp(point.x, point.y, nFlags);
	
	CWnd::OnRButtonUp(nFlags, point);
}

void CRenderWnd::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnKeyDown(nChar, nRepCnt, nFlags);
	
	CWnd::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CRenderWnd::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnKeyUp(nChar, nRepCnt, nFlags);
	
	CWnd::OnKeyUp(nChar, nRepCnt, nFlags);
}

//	Reset camera position
void CRenderWnd::ResetCameraPos(float x, float y, float z)
{
	for (int i=0; i < NUM_VIEWPORT; i++)
		m_aViews[i]->ResetCameraPos(x, y, z);
}


int CRenderWnd::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	
	// Get default context information
	WTInfo( WTI_DEFSYSCTX, 0, &m_lc );

	// Open the context
	m_lc.lcPktData = PACKETDATA;
	m_lc.lcPktMode = PACKETMODE;
	//m_lc.lcOptions = CXO_MESSAGES;
	
	m_lc.lcMoveMask = PACKETDATA;
	m_lc.lcBtnUpMask = m_lc.lcBtnDnMask;
	
	m_lc.lcOutOrgX = 0;
	m_lc.lcOutExtX = GetSystemMetrics(SM_CXSCREEN);
	m_lc.lcOutOrgY = 0;
	/* the negative sign here reverses the sense of the Y axis. */
	/* WinTab uses a lower-left origin; MM_TEXT uses upper left. */
	m_lc.lcOutExtY = -GetSystemMetrics(SM_CYSCREEN);
	//hCtx = WTOpen( m_hWnd, &lc, TRUE );
	m_hCtx = WTOpen( m_hWnd, &m_lc, TRUE );
	WTQueueSizeSet(m_hCtx, 1);
	
	return 0;
}

LRESULT CRenderWnd::OnWTPacket(WPARAM wSerial, LPARAM hCtx)
{
	// Read the packet
	ASSERT(m_iActiveView >= 0);
	PACKET pkt;
	WTPacket( (HCTX)hCtx, wSerial, &pkt );

	// Process packets in order, one at a time
	CSingleLock lock( m_pWTMutex, TRUE );
	// Get window size
	RECT window_rect;
	GetWindowRect( &window_rect );
	POINT size;
	size.x = window_rect.right - window_rect.left;
	size.y = window_rect.bottom - window_rect.top;
	POINT csr;
	csr.x = (size.x * pkt.pkX) / m_lc.lcInExtX;
	csr.y = size.y - (size.y * pkt.pkY) / m_lc.lcInExtY;
	int press = pkt.pkNormalPressure;
	m_aViews[m_iActiveView]->OnWTPacket(csr.x,csr.y,pkt.pkButtons,press);
	
	return TRUE;
}

BOOL CRenderWnd::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: Add your message handler code here and/or call default
	ASSERT(m_iActiveView >= 0);
	m_aViews[m_iActiveView]->OnMouseWheel(nFlags, zDelta, pt);

	return true;
	//return CWnd::OnMouseWheel(nFlags, zDelta, pt);
}
