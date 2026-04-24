// ActionDialogueDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "ActionDialogueDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmDialogue.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionDialogueDlg dialog


CActionDialogueDlg::CActionDialogueDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionDialogueDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
    m_sDialogue = pDialogue->GetDialogue();
    m_iTotalFrames = pDialogue->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionDialogueDlg)
	//}}AFX_DATA_INIT
}


void CActionDialogueDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDialogueDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_DIALOGUE, m_sDialogue);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDialogueDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDialogueDlg)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDialogueDlg message handlers

void CActionDialogueDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDialogue *pDialogue = dynamic_cast<CAnmDialogue*>(pAction);
    pDialogue->SetTotalFrames(m_iTotalFrames);
    pDialogue->SetDialogue(ACString(m_sDialogue));
    EndDialog(0);
}

void CActionDialogueDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionDialogueDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}
