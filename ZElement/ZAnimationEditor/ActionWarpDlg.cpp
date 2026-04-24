// ActionWarpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionWarpDlg.h"
// #include "Animator/Animator.h"
// #include "Animator/AnmObjectManager.h"
// #include "Animator/AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionWarpDlg dialog

/*
CActionWarpDlg::CActionWarpDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionWarpDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmWarp *pWarp = dynamic_cast<CAnmWarp*>(pAction);
    m_fStartWarpScale = pWarp->GetStartWarp().fScale;
    m_fStartWarpSpeed = pWarp->GetStartWarp().fSpeed;
    m_fEndWarpScale = pWarp->GetEndWarp().fScale;
    m_fEndWarpSpeed = pWarp->GetEndWarp().fSpeed;
	m_iTotalFrames = pWarp->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionWarpDlg)
	//}}AFX_DATA_INIT
}


void CActionWarpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionWarpDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_WARP_SCALE, m_fStartWarpScale);
	DDX_Text(pDX, IDC_START_WARP_SPEED, m_fStartWarpSpeed);
	DDX_Text(pDX, IDC_END_WARP_SCALE, m_fEndWarpScale);
	DDX_Text(pDX, IDC_END_WARP_SPEED, m_fEndWarpSpeed);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionWarpDlg, CDialog)
	//{{AFX_MSG_MAP(CActionWarpDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionWarpDlg message handlers

void CActionWarpDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionWarpDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmWarp *pWarp = dynamic_cast<CAnmWarp*>(pAction);
    A3DPOST_EFFECT_WARP_PARAM warp;
    warp.fScale = m_fStartWarpScale;
    warp.fSpeed = m_fStartWarpSpeed;
    pWarp->SetStartWarp(warp);
    warp.fScale = m_fEndWarpScale;
    warp.fSpeed = m_fEndWarpSpeed;
    pWarp->SetEndWarp(warp);
    pWarp->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionWarpDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

  */