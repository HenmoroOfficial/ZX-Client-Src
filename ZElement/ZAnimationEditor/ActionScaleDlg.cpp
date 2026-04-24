// ActionScaleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionScaleDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmScale.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg dialog



CActionScaleDlg::CActionScaleDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionScaleDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
    m_fStartScale = pScale->GetStartScale();
    m_fEndScale = pScale->GetEndScale();
	m_iTotalFrames = pScale->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionScaleDlg)
	//}}AFX_DATA_INIT
}


void CActionScaleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionScaleDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_SCALE, m_fStartScale);
	DDX_Text(pDX, IDC_END_SCALE, m_fEndScale);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionScaleDlg, CDialog)
	//{{AFX_MSG_MAP(CActionScaleDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg message handlers

void CActionScaleDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionScaleDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmScale *pScale = dynamic_cast<CAnmScale*>(pAction);
    pScale->SetStartScale(m_fStartScale);
    pScale->SetEndScale(m_fEndScale);
    pScale->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionScaleDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

