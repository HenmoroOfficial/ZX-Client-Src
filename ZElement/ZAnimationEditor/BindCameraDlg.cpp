// BindCameraDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "BindCameraDlg.h"
#include "Animator.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBindCameraDlg dialog


CBindCameraDlg::CBindCameraDlg(CWnd* pParent)
	: CDialog(CBindCameraDlg::IDD, pParent)
{
    A3DVECTOR3 vOffset = g_Animator.GetBindCameraOffset();
    m_fOffsetX = vOffset.x;
    m_fOffsetY = vOffset.y;
	m_fOffsetZ = vOffset.z;
	//{{AFX_DATA_INIT(CBindCameraDlg)
	//}}AFX_DATA_INIT
}


void CBindCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBindCameraDlg)
	DDX_Text(pDX, IDC_OFFSET_X, m_fOffsetX);
	DDX_Text(pDX, IDC_OFFSET_Y, m_fOffsetY);
	DDX_Text(pDX, IDC_OFFSET_Z, m_fOffsetZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBindCameraDlg, CDialog)
	//{{AFX_MSG_MAP(CBindCameraDlg)
	ON_BN_CLICKED(IDC_BUTTON_BIND_CAMERA_OK, OnButtonBindCameraOk)
	ON_BN_CLICKED(IDC_BUTTON_BIND_CAMERA_CANCEL, OnButtonBindCameraCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBindCameraDlg message handlers

void CBindCameraDlg::OnButtonBindCameraOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    A3DVECTOR3 vOffset(m_fOffsetX, m_fOffsetY, m_fOffsetZ);
    g_Animator.SetBindCameraOffset(vOffset);
	EndDialog(0);
}

void CBindCameraDlg::OnButtonBindCameraCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
