// BezierPointBar.cpp : implementation file
//

#include "global.h"
#include "elementeditor.h"
#include "BezierPointBar.h"
#include "EditerBezierPoint.h"
#include "EditerBezier.h"
#include "ToolTabWnd.h"
#include "MainFrm.h"

#include "Render.h"
#include "a3d.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierPointBar dialog


CBezierPointBar::CBezierPointBar(CWnd* pParent /*=NULL*/)
	: CDialogBar()
{
	//{{AFX_DATA_INIT(CBezierPointBar)
	m_fCamFov = 0.0f;
	m_fCamSpeed = 0.0f;
	m_fCamX = 0.0f;
	m_fCamY = 0.0f;
	m_fCamZ = 0.0f;
	m_fCamRatio = 0.0f;
	m_fSpotRadius = 0.0f;
	//}}AFX_DATA_INIT
}


void CBezierPointBar::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBezierPointBar)
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_FOV, m_fCamFov);
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_SPEED, m_fCamSpeed);
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_X, m_fCamX);
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_Y, m_fCamY);
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_Z, m_fCamZ);
	DDX_Text(pDX, IDC_EDIT_BEZIER_CAM_RATIO, m_fCamRatio);
	DDX_Text(pDX, IDC_EDIT_BEZIER_SPOT_RADIUS, m_fSpotRadius);
	//}}AFX_DATA_MAP
}




BEGIN_MESSAGE_MAP(CBezierPointBar, CDialogBar)
	//{{AFX_MSG_MAP(CBezierPointBar)
	ON_BN_CLICKED(IDC_CHECK_CHANGE_CAM123, OnCheckChangeCam)
	ON_BN_CLICKED(IDC_BEZIER_MODIFIED, OnBezierModified)
	ON_UPDATE_COMMAND_UI(IDC_BEZIER_MODIFIED, OnUpdateBezierModified)
	ON_WM_WINDOWPOSCHANGED()
//	ON_BN_CLICKED(IDC_NEXT_POINT, OnNextPointModified)
	ON_UPDATE_COMMAND_UI(IDC_NEXT_POINT, OnNextPoint)
//	ON_BN_CLICKED(IDC_PREV_POINT, OnPrevPointModified)
	ON_UPDATE_COMMAND_UI(IDC_PREV_POINT, OnPrevPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBezierPointBar message handlers

void CBezierPointBar::Init(CEditerBezier* pObj,  CEditerBezierPoint *pBezierPoint)
{
	m_pBezier = pObj;
	m_pBezierPoint = pBezierPoint;
	
	A3DVECTOR3 dir = pBezierPoint->GetDirection();
	m_fCamX = dir.x;
	m_fCamY = dir.y;
	m_fCamZ = dir.z;

	m_fCamSpeed = pBezierPoint->GetSpeed();
	m_fCamFov = pBezierPoint->GetFov();
	m_fCamRatio = pBezierPoint->GetRatio();
	//m_fSpotRadius = pBezierPoint->GetSpotRadius();

	UpdateData(FALSE);
}

void CBezierPointBar::OnCheckChangeCam() 
{
	// TODO: Add your control notification handler code here
	if(g_Render.GetA3DEngine()->GetActiveCamera())
	{
		float parm[4];
		A3DCamera * pca = (A3DCamera*)g_Render.GetA3DEngine()->GetActiveCamera();
		pca->GetProjectionParam(&parm[0],&parm[1],&parm[2],&parm[3]);
		
		CButton *pButton = (CButton*)GetDlgItem(IDC_CHECK_CHANGE_CAM123);
		if(!pButton->GetCheck())
		{
//			g_bLockCam = false;
			AUX_GetMainFrame()->GetToolTabWnd()->ShowWindow(SW_SHOW);
			A3DVECTOR3 dir = g_Render.GetA3DEngine()->GetActiveCamera()->GetDir();
			pca->SetProjectionParam(m_fOldFov,parm[1],parm[2],m_fOldRadio);
			pca->SetFOVSet(m_fOldFov);
			m_fCamX = dir.x;
			m_fCamY = dir.y;
			m_fCamZ = dir.z;
			UpdateData(FALSE);
		}else
		{
//			g_bLockCam = true;
			m_fOldFov = parm[0];
			m_fOldRadio = parm[3];
			AUX_GetMainFrame()->GetToolTabWnd()->ShowWindow(SW_HIDE);
			UpdateData(TRUE);
			pca->SetProjectionParam(m_pBezierPoint->GetFov()/180.8*A3D_PI,parm[1],parm[2],m_pBezierPoint->GetRatio());
			pca->SetFOVSet(m_pBezierPoint->GetFov()/180.8*A3D_PI);
			g_Render.GetA3DEngine()->GetActiveCamera()->SetPos(m_pBezierPoint->GetPos());
			g_Render.GetA3DEngine()->GetActiveCamera()->SetDirAndUp(A3DVECTOR3(m_fCamX,m_fCamY,m_fCamZ),A3DVECTOR3(0,1.0f,0));
		}
	}
	
}

void CBezierPointBar::OnUpdateBezierModified(CCmdUI *pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

void CBezierPointBar::OnBezierModified() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);

	if(m_pBezierPoint)
	{
//		if(g_bLockCam)
		{
			A3DVECTOR3 dir = g_Render.GetA3DEngine()->GetActiveCamera()->GetDir();
			m_fCamX = dir.x;
			m_fCamY = dir.y;
			m_fCamZ = dir.z;
			UpdateData(FALSE);
		}

		if(m_pBezier)
		{
			CEditerBezierSegment *pSeg = NULL;
			CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
			CEditerBezierPoint *pPoint = NULL;
			m_pBezierPoint = m_pBezier->GetCurPoint(&pSegTail,&pSegHead);
 			if(m_pBezierPoint)
 			{//对点的处理
				m_pBezierPoint->SetPos(g_Render.GetA3DEngine()->GetActiveCamera()->GetPos());
				m_pBezierPoint->SetDirection(A3DVECTOR3(m_fCamX,m_fCamY,m_fCamZ));
				m_pBezierPoint->SetSpeed(m_fCamSpeed);
				m_pBezierPoint->SetFov(m_fCamFov);
				m_pBezierPoint->SetRatio(m_fCamRatio);	
				
				if(pSegTail)
				{
					pSegTail->SetAnchor2(m_pBezierPoint->GetPos());
					pSegTail->SetTail(m_pBezierPoint->GetPos());
				}

				if(pSegHead)
				{
					pSegHead->SetAnchor1(m_pBezierPoint->GetPos());
					pSegHead->SetHead(m_pBezierPoint->GetPos());
				}
			}
		}

		//m_pBezierPoint->SetSpotRadius(m_fSpotRadius);
		AUX_GetMainFrame()->GetMap()->SetModifiedFlag(true);
	}
}

void CBezierPointBar::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialogBar::OnWindowPosChanged(lpwndpos);
	
	// TODO: Add your message handler code here
	// should only be called once, when floated.
	if( IsFloating() )
	{
		CWnd* pParent = GetParent();
		pParent = pParent->GetParent();
		pParent->ModifyStyle( WS_SYSMENU, 0, 0 );
	}
	
}

void CBezierPointBar::OnNextPoint(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CBezierPointBar::OnPrevPoint(CCmdUI *pCmdUI) 
{
	pCmdUI->Enable(TRUE);
}

void CBezierPointBar::OnNextPointModified()
{
	// TODO: Add your control notification handler code here
	m_pBezier->SelectedPoint(m_pBezier->GetSelectedPoint()+1);	
	CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
	m_pBezierPoint = m_pBezier->GetCurPoint(&pSegTail,&pSegHead);
}
void CBezierPointBar::OnPrevPointModified()
{
	// TODO: Add your control notification handler code here
	m_pBezier->SelectedPoint(m_pBezier->GetSelectedPoint()-1);	
	CEditerBezierSegment *pSegHead = NULL,*pSegTail = NULL;
	m_pBezierPoint = m_pBezier->GetCurPoint(&pSegTail,&pSegHead);
}
