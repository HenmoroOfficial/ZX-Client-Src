// ChildView.cpp : implementation of the CChildView class
//

#include "Global.h"
#include "ModelView.h"
#include "ChildView.h"
#include "render.h"
#include "A3d.h"
#include "A3DSkillGfxEvent.h"

#include "A3DGFXExMan.h"
#include "A3DSkillGfxComposer.h"



#define new A_DEBUG_NEW

#define DEFAULTFOV	56.0f
extern A3DVECTOR3 _target_pos;

/////////////////////////////////////////////////////////////////////////////
// CChildView

CChildView::CChildView()
{
	m_iWidth = 640;
	m_iHeight = 480;
	m_pA3DViewport = NULL;
	m_pA3DCamera = NULL;
	m_bLDrag = false;
	m_bRDrag = false;
}

CChildView::~CChildView()
{
	Aux_ReleaseObject();

	//modified by MaYuanzheng 2010.8.17
	_SGC::AfxGetSkillGfxEventMan()->Release();//add a name space
	_SGC::AfxGetSkillGfxComposerMan()->Release();//add a name space 
	//MaYuanzheng's modify end
	AfxGetGFXExMan()->Release();
	
	if(m_pA3DCamera)
	{
		m_pA3DCamera->Release();
		delete m_pA3DCamera;
		m_pA3DCamera = 0;
	}
	if(m_pA3DViewport)
	{
		m_pA3DViewport->Release();
		delete m_pA3DViewport;
		m_pA3DViewport = 0; 
	}
}


BEGIN_MESSAGE_MAP(CChildView,CWnd )
	//{{AFX_MSG_MAP(CChildView)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_SET_RESTORE_CAM, OnSetRestoreCam)
	ON_COMMAND(ID_SET_SHOW_AABB, OnSetShowAabb)
	ON_UPDATE_COMMAND_UI(ID_SET_SHOW_AABB, OnUpdateSetShowAabb)
	ON_COMMAND(ID_SET_SHOW_GRID, OnSetShowGrid)
	ON_UPDATE_COMMAND_UI(ID_SET_SHOW_GRID, OnUpdateSetShowGrid)
	ON_COMMAND(ID_SET_SHOW_WIRE, OnSetShowWire)
	ON_UPDATE_COMMAND_UI(ID_SET_SHOW_WIRE, OnUpdateSetShowWire)
	ON_WM_SETFOCUS()
	ON_COMMAND(ID_SET_AMB_CLR, OnSetAmbClr)
	ON_COMMAND(ID_SET_CLEAR_CLR, OnSetClearClr)
	ON_COMMAND(ID_SET_DIR_CLR, OnSetDirClr)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CChildView message handlers

BOOL CChildView::PreCreateWindow(CREATESTRUCT& cs) 
{
	if (!CWnd::PreCreateWindow(cs))
		return FALSE;

	cs.dwExStyle |= WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW|CS_DBLCLKS, 
		::LoadCursor(NULL, IDC_ARROW), HBRUSH(COLOR_WINDOW+1), NULL);

	return TRUE;
}

void CChildView::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	// TODO: Add your message handler code here
	Render();
	// Do not call CWnd::OnPaint() for painting messages
}

bool CChildView::Init()
{
	
	RECT rcClient;
	GetClientRect(&rcClient);
	m_iWidth = rcClient.right;
	m_iHeight = rcClient.bottom;
	//	Create A3D viewport
	if (!g_Render.GetA3DDevice()->CreateViewport(&m_pA3DViewport, 0, 0, 
					rcClient.right, rcClient.bottom, 0.0f, 1.0f, true, true, 0xff808080))
	{
		g_Log.Log("CRenderWnd::InitDeviceObjects: Failed to create A3D viewport.");
		return false;
	}
	//	Create camera
	if (!(m_pA3DCamera = new A3DCamera))
	{
		g_Log.Log("CPersViewport::Init: Failed to create camera object!");
		return false;
	}
	
	float fRatio = (float)m_iWidth / m_iHeight;
	if (!m_pA3DCamera->Init(g_Render.GetA3DDevice(), DEG2RAD(DEFAULTFOV), 0.1f, 3000.0f,fRatio))
	{
		g_Log.Log(0, "CPersViewport::Init: Failed to initialize camera.");
		return false;
	}

	m_pA3DCamera->SetPos(g_vOrigin - g_vAxisZ * 10.0f + A3DVECTOR3(0,2.0f,0));
	m_pA3DCamera->SetDirAndUp(-g_vAxisZ, g_vAxisY);
	return true;
}

void CChildView::SetCamaraPos( A3DVECTOR3 vPos )
{
	if(m_pA3DCamera) m_pA3DCamera->SetPos(vPos); 
}


void CChildView::OnSize(UINT nType, int cx, int cy) 
{
	CWnd ::OnSize(nType, cx, cy);
	// TODO: Add your message handler code here
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
	
	if (m_pA3DCamera)
	{
		m_iWidth = cx;
		m_iHeight = cy;
		//	Adjust camera
		float fRatio = (float)cx / cy;
		m_pA3DCamera->SetProjectionParam(DEG2RAD(DEFAULTFOV), 0.1f, 3000.0f, fRatio);
	}
}

void CChildView::Render()
{
	if(m_pA3DCamera==NULL || m_pA3DViewport==NULL) return;
	static DWORD dwLastTime = timeGetTime();
	DWORD curTime = timeGetTime();
	DWORD delta = curTime - dwLastTime;
	
	if(g_pRenderableObject && delta > 16)
	{
		delta  =  (DWORD)(delta * g_fPlaySpeed);
		if(delta <= 0 ) delta = 1.0f;
		g_pRenderableObject->Tick(delta);
		AfxGetGFXExMan()->Tick(delta);
		
		A3DVECTOR3 vDir = g_pRenderableObject->GetAbsoluteTM() * A3DVECTOR3(0, 0, 1.0f);
		vDir.Normalize();
		_target_pos = vDir * 10.0f;
		//modified by MaYuanzheng 2010.8.17
		_SGC::AfxGetSkillGfxEventMan()->Tick(delta); //add a namespace
		//MaYuanzheng's modify end
		dwLastTime = curTime;
	}
	//modified by MaYuanzheng 2010.8.17
	_SGC::AfxGetSkillGfxEventMan()->Render(); //add a namespace
	//Mayuanzheng's modify end
	
	if (!g_Render.BeginRender())
		return;
	//REMDER
	m_pA3DViewport->SetCamera(m_pA3DCamera);
	m_pA3DViewport->Active();
	m_pA3DViewport->ClearDevice();
	A3DEngine* pA3DEngine = g_Render.GetA3DEngine();
	A3DDevice* pA3DDevice = g_Render.GetA3DDevice();
	pA3DDevice->Clear(D3DCLEAR_TARGET,g_dwClearClr,0.0f,0);
	pA3DDevice->SetAlphaTestEnable(true);
	pA3DDevice->SetAlphaBlendEnable(false);
	pA3DDevice->SetAlphaFunction(A3DCMP_GREATEREQUAL);
	pA3DDevice->SetAlphaRef(84);
	//	Set render state
	if (g_bShowWire)
		pA3DDevice->SetFillMode(A3DFILL_WIREFRAME);
	else
		pA3DDevice->SetFillMode(A3DFILL_SOLID);
	if(g_pRenderableObject)
		g_pRenderableObject->Render(m_pA3DViewport);
	
	if(g_bShowGrid)
		DrawCenter();
	AfxGetGFXExMan()->RenderAllSkinModels(m_pA3DViewport);
	A3DSkinRender* pSkinRender = pA3DEngine->GetA3DSkinMan()->GetCurSkinRender();
	pSkinRender->Render(m_pA3DViewport, false);
	pSkinRender->RenderAlpha(m_pA3DViewport);
	
	pA3DDevice->SetAlphaTestEnable(false);
	pA3DDevice->SetAlphaBlendEnable(true);
	pA3DDevice->SetSourceAlpha(A3DBLEND_SRCALPHA);
	pA3DDevice->SetDestAlpha(A3DBLEND_INVSRCALPHA);
	pA3DDevice->SetTextureColorOP(0, A3DTOP_MODULATE);
	
	AfxGetGFXExMan()->RenderAllGfx(m_pA3DViewport);
	
	g_Render.EndRender();
}

void CChildView::DrawCenter()
{
	A3DVECTOR3 pVertices[20];
	WORD       pIndices[20];
	A3DMATRIX4 mat;
	if(g_pRenderableObject)
		mat = g_pRenderableObject->GetAbsoluteTM();
	
	for( int i = 0; i < 5; ++i)
	{
		pVertices[i] = A3DVECTOR3(-2.0f + i, 0, -2.0f) * mat;
		pVertices[i + 5] = A3DVECTOR3(-2.0f + i, 0, 2.0f) * mat;

		pVertices[i + 10] = A3DVECTOR3(-2.0f, 0, -2.0f + i) * mat;
		pVertices[i + 15] = A3DVECTOR3(2.0f, 0, -2.0f + i) *mat;
	}
	
	int x = 0;
	for(  i = 0; i < 5; ++i)
	{
		pIndices[x++] = i;
		pIndices[x++] = i+5;
		pIndices[x++] = i+10;
		pIndices[x++] = i+15;
	}
	
	A3DWireCollector *pWireCollector = g_Render.GetA3DEngine()->GetA3DWireCollector();
	if(pWireCollector)
	{
		pWireCollector->AddRenderData_3D(pVertices,20,pIndices,20,A3DCOLORRGB(0,255,0));
		pWireCollector->Flush();
	}
}

BOOL CChildView::OnEraseBkgnd(CDC* pDC) 
{
	// TODO: Add your message handler code here and/or call default
	return TRUE;
	//return CWnd ::OnEraseBkgnd(pDC);
}

int CChildView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CWnd ::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// TODO: Add your specialized creation code here
	return 0;
}

BOOL CChildView::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

BOOL CChildView::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pA3DViewport) 
	{
		m_pA3DViewport->Release();
		delete m_pA3DViewport;
		m_pA3DViewport = 0;
	}
	if(m_pA3DCamera)
	{
		m_pA3DCamera->Release();
		delete m_pA3DCamera;
		m_pA3DCamera = 0;
	}
	return CWnd ::DestroyWindow();
}

void CChildView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	m_bLDrag = true;
	m_ptLast = point;
	SetCapture();
	CWnd ::OnLButtonDown(nFlags, point);
}

void CChildView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bLDrag = false;
	ReleaseCapture();
	CWnd ::OnLButtonUp(nFlags, point);
}

void CChildView::OnMouseMove(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	if( m_pA3DCamera)
	{
		CPoint delta = point - m_ptLast;
		m_ptLast = point;
		if(m_bLDrag && g_pRenderableObject)
		{
			g_pRenderableObject->Rotate(A3DVECTOR3(delta.y*0.01f,-delta.x*0.01f,0.0f));
		}else if(m_bRDrag)
		{
			A3DVECTOR3 oldPos = m_pA3DCamera->GetPos();
			A3DVECTOR3 vDelta(delta.x * 0.01f,delta.y * 0.01f,0.0f);
			m_pA3DCamera->SetPos(oldPos + vDelta);	
		}
	}
	CWnd ::OnMouseMove(nFlags, point);
}

void CChildView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	SetFocus();
	m_ptLast = point;
	m_bRDrag = true;
	SetCapture();
	CWnd ::OnRButtonDown(nFlags, point);
}

void CChildView::OnRButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	m_bRDrag = false;
	ReleaseCapture();
	CWnd ::OnRButtonUp(nFlags, point);
}

BOOL CChildView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	// TODO: Add your message handler code here and/or call default
	MouseWheel(zDelta);
	return CWnd ::OnMouseWheel(nFlags, zDelta, pt);
}

void CChildView::MouseWheel(int delta)
{
	if(m_pA3DCamera) m_pA3DCamera->SetPos(m_pA3DCamera->GetPos() + g_vAxisZ*delta*0.001f);
}

void CChildView::OnSetRestoreCam() 
{
	// TODO: Add your command handler code here
	if(g_pRenderableObject && m_pA3DCamera)
		m_pA3DCamera->SetPos(g_pRenderableObject->GetDefaultCamaraPos());
}

void CChildView::OnSetShowAabb() 
{
	// TODO: Add your command handler code here
	g_bShowAABB = !g_bShowAABB;
}

void CChildView::OnUpdateSetShowAabb(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowAABB);
}

void CChildView::OnSetShowGrid() 
{
	// TODO: Add your command handler code here
	g_bShowGrid = !g_bShowGrid;
}

void CChildView::OnUpdateSetShowGrid(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowGrid);
}

void CChildView::OnSetShowWire() 
{
	// TODO: Add your command handler code here
	g_bShowWire = !g_bShowWire;
}

void CChildView::OnUpdateSetShowWire(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowWire);
}

LRESULT CChildView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	return CWnd ::DefWindowProc(message, wParam, lParam);
}

void CChildView::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd ::OnSetFocus(pOldWnd);
	
	// TODO: Add your message handler code here
	
}

void CChildView::OnSetAmbClr() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	if(IDOK==dlg.DoModal())
	{
		A3DLIGHTPARAM param = g_Render.GetDirLight()->GetLightparam();
		g_dwAmbClr = A3DCOLORRGB(GetRValue(dlg.GetColor()),GetGValue(dlg.GetColor()),GetBValue(dlg.GetColor()));
		g_Render.GetA3DDevice()->SetAmbient(g_dwAmbClr);
		g_Render.ChangeLight(param.Direction,g_dwDirClr,g_dwAmbClr);
	}
}

void CChildView::OnSetClearClr() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	if(IDOK==dlg.DoModal())
	{
		g_dwClearClr = A3DCOLORRGB(GetRValue(dlg.GetColor()),GetGValue(dlg.GetColor()),GetBValue(dlg.GetColor()));
	}
}

void CChildView::OnSetDirClr() 
{
	// TODO: Add your command handler code here
	CColorDialog dlg;
	if(IDOK==dlg.DoModal())
	{
		g_dwDirClr = A3DCOLORRGB(GetRValue(dlg.GetColor()),GetGValue(dlg.GetColor()),GetBValue(dlg.GetColor()));
		A3DLIGHTPARAM param = g_Render.GetDirLight()->GetLightparam();
		g_Render.ChangeLight(param.Direction,g_dwDirClr,g_dwAmbClr);
	}
}
