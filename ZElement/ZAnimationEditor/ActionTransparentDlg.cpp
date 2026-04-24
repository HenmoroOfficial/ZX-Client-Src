// ActionTransparentDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionTransparentDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmTransparent.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg dialog


CActionTransparentDlg::CActionTransparentDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
: CDialog(CActionTransparentDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
    m_fStartTransparent = pTransparent->GetStartTransparent();
    m_fEndTransparent = pTransparent->GetEndTransparent();
    m_iTotalFrames = pTransparent->GetTotalFrames();
    //{{AFX_DATA_INIT(CActionScaleDlg)
    //}}AFX_DATA_INIT
}


void CActionTransparentDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CActionTransparentDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_START_TRANSPARENT, m_fStartTransparent);
    DDX_Text(pDX, IDC_END_TRANSPARENT, m_fEndTransparent);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    //}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionTransparentDlg, CDialog)
//{{AFX_MSG_MAP(CActionTransparentDlg)
ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CActionTransparentDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionTransparentDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmTransparent *pTransparent = dynamic_cast<CAnmTransparent*>(pAction);
    pTransparent->SetStartTransparent(m_fStartTransparent);
    pTransparent->SetEndTransparent(m_fEndTransparent);
    pTransparent->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionTransparentDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}
