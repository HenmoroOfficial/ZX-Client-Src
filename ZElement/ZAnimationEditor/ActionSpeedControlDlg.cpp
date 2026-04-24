// ActionSpeedControlDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionSpeedControlDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmSpeedControl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionSpeedControlDlg dialog


CActionSpeedControlDlg::CActionSpeedControlDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionSpeedControlDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmSpeedControl *pSpeedControl = dynamic_cast<CAnmSpeedControl*>(pAction);
    m_fSpeed = pSpeedControl->GetSpeed();
    m_iTotalFrames = pSpeedControl->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionSpeedControlDlg)
	//}}AFX_DATA_INIT
}


void CActionSpeedControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionSpeedControlDlg)
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    DDX_Text(pDX, IDC_SPEED, m_fSpeed);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionSpeedControlDlg, CDialog)
	//{{AFX_MSG_MAP(CActionSpeedControlDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionSpeedControlDlg message handlers

void CActionSpeedControlDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmSpeedControl *pSpeedControl = dynamic_cast<CAnmSpeedControl*>(pAction);
    pSpeedControl->SetSpeed(m_fSpeed);
    pSpeedControl->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionSpeedControlDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CActionSpeedControlDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

