// ActionFOVDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionFOVDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmFOV.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionFOVDlg dialog

CActionFOVDlg::CActionFOVDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionFOVDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
    m_fStartFOV = pFOV->GetStartFOV();
    m_fEndFOV = pFOV->GetEndFOV();
	m_iTotalFrames = pFOV->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionFOVDlg)
	//}}AFX_DATA_INIT
}


void CActionFOVDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionFOVDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_FOV, m_fStartFOV);
	DDX_Text(pDX, IDC_END_FOV, m_fEndFOV);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionFOVDlg, CDialog)
	//{{AFX_MSG_MAP(CActionFOVDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionFOVDlg message handlers

void CActionFOVDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionFOVDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmFOV *pFOV = dynamic_cast<CAnmFOV*>(pAction);
    pFOV->SetStartFOV(m_fStartFOV);
    pFOV->SetEndFOV(m_fEndFOV);
    pFOV->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionFOVDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

