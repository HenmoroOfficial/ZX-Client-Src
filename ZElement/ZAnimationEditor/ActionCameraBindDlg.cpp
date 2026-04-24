// ActionCameraBindDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "ActionCameraBindDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmCameraBind.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionCameraBindDlg dialog


CActionCameraBindDlg::CActionCameraBindDlg(CWnd* pParent,CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionCameraBindDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CActionCameraBindDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmCameraBind *pCamBind = dynamic_cast<CAnmCameraBind*>(pAction);
	m_iTotalFrames = pCamBind->GetTotalFrames();
	m_uiFollowObj  = pCamBind->GetFllowObj();
	m_fOffSetX   = pCamBind->GetDistance().x;
	m_fOffSetY   = pCamBind->GetDistance().y;
	m_fOffSetZ   = pCamBind->GetDistance().z;
}


void CActionCameraBindDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionCameraBindDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
	DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_Text(pDX, IDC_FOLLOW_OBJ,m_uiFollowObj);
	DDX_Text(pDX, IDC_START_POS_X, m_fOffSetX);
    DDX_Text(pDX, IDC_START_POS_Y, m_fOffSetY);
	DDX_Text(pDX, IDC_START_POS_Z, m_fOffSetZ);
}


BEGIN_MESSAGE_MAP(CActionCameraBindDlg, CDialog)
	//{{AFX_MSG_MAP(CActionCameraBindDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionCameraBindDlg message handlers

void CActionCameraBindDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
	CAnmObject* pCam = g_Animator.GetObjectManager()->GetObject(0);
	CAnmObject* pObj = g_Animator.GetObjectManager()->GetObject(m_uiFollowObj);
	if(pCam && pObj)
	{
	   CAnmAction *pAction = m_pNode->GetAction();
	   CAnmCameraBind *pCamBind = dynamic_cast<CAnmCameraBind*>(pAction);
	   pCamBind->SetFllowObj(m_uiFollowObj);
	   pCamBind->SetTotalFrames(m_iTotalFrames);
	   pCamBind->SetDistance(A3DVECTOR3(m_fOffSetX,m_fOffSetY,m_fOffSetZ));
	}
    EndDialog(0);
}

void CActionCameraBindDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
	
}

void CActionCameraBindDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
	
}
