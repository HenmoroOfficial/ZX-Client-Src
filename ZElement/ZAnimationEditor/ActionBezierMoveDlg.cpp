// ActionBezierMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionBezierMoveDlg.h"
#include "AnmBezierMove.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionBezierMoveDlg dialog


CActionBezierMoveDlg::CActionBezierMoveDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionBezierMoveDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
    m_bCloseTerrain = pBezierMove->GetCloseTerrain();
    m_uiBezierID = pBezierMove->GetBezierID();
	//{{AFX_DATA_INIT(CActionBezierMoveDlg)
	//}}AFX_DATA_INIT
}

void CActionBezierMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionBezierMoveDlg)
    DDX_Check(pDX, IDC_CLOSE_TERRAIN, m_bCloseTerrain);
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_BEZIER_ID, m_uiBezierID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionBezierMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CActionBezierMoveDlg)
    ON_BN_CLICKED(IDC_USE_ORIENTATION, OnButtonUseOrientation)
    ON_BN_CLICKED(IDC_CLOSE_TERRAIN, OnCloseTerrain)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionBezierMoveDlg message handlers

void CActionBezierMoveDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmBezierMove *pBezierMove = dynamic_cast<CAnmBezierMove*>(pAction);
    pBezierMove->SetBezierID(m_uiBezierID);
    pBezierMove->SetCloseTerrain(m_bCloseTerrain ? true : false);
    EndDialog(0);
}

void CActionBezierMoveDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionBezierMoveDlg::OnButtonUseOrientation()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
}

void CActionBezierMoveDlg::OnCloseTerrain() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
}

void CActionBezierMoveDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

