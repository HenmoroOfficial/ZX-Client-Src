// ActionBezierPointDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "Animator.h"
#include "ActionBezierPointDlg.h"
#include "AnmBezierPoint.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionBezierPointDlg dialog


CActionBezierPointDlg::CActionBezierPointDlg(CWnd* pParent, CAnmBezierPoint *pPoint)
	: CDialog(CActionBezierPointDlg::IDD, pParent)
{
    m_pBezierPoint = pPoint;
    m_uiPointID = pPoint->GetID();
    m_iTotalFrames = pPoint->GetTotalFrames();
    m_fPosX = pPoint->GetPos().x;
    m_fPosY = pPoint->GetPos().y;
    m_fPosZ = pPoint->GetPos().z;
    m_fOriW = pPoint->GetOrientation().w;
    m_fOriX = pPoint->GetOrientation().x;
    m_fOriY = pPoint->GetOrientation().y;
	m_fOriZ = pPoint->GetOrientation().z;
	m_fFOV = pPoint->GetFOV();
	//{{AFX_DATA_INIT(CActionBezierPointDlg)
	//}}AFX_DATA_INIT
}


void CActionBezierPointDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionBezierPointDlg)
	DDX_Text(pDX, IDC_BEZIER_POINT_ID, m_uiPointID);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_Text(pDX, IDC_BEZIER_FOV, m_fFOV);
	DDX_Text(pDX, IDC_POS_X, m_fPosX);
	DDX_Text(pDX, IDC_POS_Y, m_fPosY);
	DDX_Text(pDX, IDC_POS_Z, m_fPosZ);
	DDX_Text(pDX, IDC_ORI_W, m_fOriW);
	DDX_Text(pDX, IDC_ORI_X, m_fOriX);
	DDX_Text(pDX, IDC_ORI_Y, m_fOriY);
	DDX_Text(pDX, IDC_ORI_Z, m_fOriZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionBezierPointDlg, CDialog)
	//{{AFX_MSG_MAP(CActionBezierPointDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionBezierPointDlg message handlers

void CActionBezierPointDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    g_Animator.Stop();
    m_pBezierPoint->SetTotalFrames(m_iTotalFrames);
    m_pBezierPoint->SetPos(A3DVECTOR3(m_fPosX, m_fPosY, m_fPosZ));
    m_pBezierPoint->SetOrientation(A3DQUATERNION(m_fOriW, m_fOriX, m_fOriY, m_fOriZ));
    m_pBezierPoint->SetFOV(m_fFOV);
    theApp.OnChanged();
    EndDialog(0);
}

void CActionBezierPointDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

