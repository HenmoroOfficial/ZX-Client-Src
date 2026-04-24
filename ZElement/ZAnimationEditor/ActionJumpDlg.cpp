// ActionJumpDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionJumpDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmJump.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionJumpDlg dialog


CActionJumpDlg::CActionJumpDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
: CDialog(CActionJumpDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
    m_fStartPosX = pJump->GetStartPos().x;
    m_fStartPosY = pJump->GetStartPos().y;
    m_fStartPosZ = pJump->GetStartPos().z;
    m_fEndPosX = pJump->GetEndPos().x;
    m_fEndPosY = pJump->GetEndPos().y;
    m_fEndPosZ = pJump->GetEndPos().z;
    m_iTotalFrames = pJump->GetTotalFrames();
    //{{AFX_DATA_INIT(CActionMoveDlg)
    //}}AFX_DATA_INIT
}

void CActionJumpDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionJumpDlg)
	DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
	DDX_Text(pDX, IDC_END_POS_X, m_fEndPosX);
	DDX_Text(pDX, IDC_END_POS_Y, m_fEndPosY);
	DDX_Text(pDX, IDC_END_POS_Z, m_fEndPosZ);
	DDX_Text(pDX, IDC_START_POS_X, m_fStartPosX);
	DDX_Text(pDX, IDC_START_POS_Y, m_fStartPosY);
	DDX_Text(pDX, IDC_START_POS_Z, m_fStartPosZ);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionJumpDlg, CDialog)
	//{{AFX_MSG_MAP(CActionJumpDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_CLOSE_TERRAIN, OnCloseTerrain)
	ON_BN_CLICKED(IDC_BUTTON_SET_START_POS, OnButtonSetStartPos)
	ON_BN_CLICKED(IDC_BUTTON_SET_END_POS, OnButtonSetEndPos)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionMoveDlg message handlers

void CActionJumpDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmJump *pJump = dynamic_cast<CAnmJump*>(pAction);
    pJump->SetStartPos(A3DVECTOR3(m_fStartPosX, m_fStartPosY, m_fStartPosZ));
    pJump->SetEndPos(A3DVECTOR3(m_fEndPosX, m_fEndPosY, m_fEndPosZ));
    pJump->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionJumpDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CActionJumpDlg::OnCloseTerrain() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
}

void CActionJumpDlg::OnButtonSetStartPos() 
{
	// TODO: Add your control notification handler code here
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    m_fStartPosX = pObject->GetPos().x;
    m_fStartPosY = pObject->GetPos().y;
    m_fStartPosZ = pObject->GetPos().z;
    UpdateData(0);
}

void CActionJumpDlg::OnButtonSetEndPos() 
{
	// TODO: Add your control notification handler code here
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    m_fEndPosX = pObject->GetPos().x;
    m_fEndPosY = pObject->GetPos().y;
    m_fEndPosZ = pObject->GetPos().z;
    UpdateData(0);
}

void CActionJumpDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}
