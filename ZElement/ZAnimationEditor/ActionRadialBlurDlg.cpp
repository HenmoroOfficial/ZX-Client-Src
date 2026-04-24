// ActionRadialBlurDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionRadialBlurDlg.h"
// #include "Animator/Animator.h"
// #include "Animator/AnmObjectManager.h"
// #include "Animator/AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionRadialBlurDlg dialog

/*
CActionRadialBlurDlg::CActionRadialBlurDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionRadialBlurDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmRadialBlur *pRadialBlur = dynamic_cast<CAnmRadialBlur*>(pAction);
    m_fStartRadialBlurBlurCenterX = pRadialBlur->GetStartRadialBlur().fBlurCenterX;
    m_fStartRadialBlurBlurCenterY = pRadialBlur->GetStartRadialBlur().fBlurCenterY;
    m_fStartRadialBlurBlurLength = pRadialBlur->GetStartRadialBlur().fBlurLength;
    m_fStartRadialBlurInFocusRadius = pRadialBlur->GetStartRadialBlur().fInFocusRadius;
    m_fStartRadialBlurOutFocusRadius = pRadialBlur->GetStartRadialBlur().fOutFocusRadius;
    m_fEndRadialBlurBlurCenterX = pRadialBlur->GetEndRadialBlur().fBlurCenterX;
    m_fEndRadialBlurBlurCenterY = pRadialBlur->GetEndRadialBlur().fBlurCenterY;
    m_fEndRadialBlurBlurLength = pRadialBlur->GetEndRadialBlur().fBlurLength;
    m_fEndRadialBlurInFocusRadius = pRadialBlur->GetEndRadialBlur().fInFocusRadius;
    m_fEndRadialBlurOutFocusRadius = pRadialBlur->GetEndRadialBlur().fOutFocusRadius;
	m_iTotalFrames = pRadialBlur->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionRadialBlurDlg)
	//}}AFX_DATA_INIT
}


void CActionRadialBlurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionRadialBlurDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_CENTER_X, m_fStartRadialBlurBlurCenterX);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_CENTER_Y, m_fStartRadialBlurBlurCenterY);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_LENGTH, m_fStartRadialBlurBlurLength);
	DDX_Text(pDX, IDC_START_BLUR_IN_FOCUS_RADIUS, m_fStartRadialBlurInFocusRadius);
	DDX_Text(pDX, IDC_START_BLUR_OUT_FOCUS_RADIUS, m_fStartRadialBlurOutFocusRadius);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_CENTER_X, m_fEndRadialBlurBlurCenterX);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_CENTER_Y, m_fEndRadialBlurBlurCenterY);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_LENGTH, m_fEndRadialBlurBlurLength);
	DDX_Text(pDX, IDC_END_BLUR_IN_FOCUS_RADIUS, m_fEndRadialBlurInFocusRadius);
	DDX_Text(pDX, IDC_END_BLUR_OUT_FOCUS_RADIUS, m_fEndRadialBlurOutFocusRadius);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionRadialBlurDlg, CDialog)
	//{{AFX_MSG_MAP(CActionRadialBlurDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionRadialBlurDlg message handlers

void CActionRadialBlurDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionRadialBlurDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmRadialBlur *pRadialBlur = dynamic_cast<CAnmRadialBlur*>(pAction);
    A3DPOST_EFFECT_RADIUSBLUR_PARAM RadialBlur;
    RadialBlur.fBlurCenterX = m_fStartRadialBlurBlurCenterX;
    RadialBlur.fBlurCenterY = m_fStartRadialBlurBlurCenterY;
    RadialBlur.fBlurLength = m_fStartRadialBlurBlurLength;
    RadialBlur.fInFocusRadius = m_fStartRadialBlurInFocusRadius;
    RadialBlur.fOutFocusRadius = m_fStartRadialBlurOutFocusRadius;
    pRadialBlur->SetStartRadialBlur(RadialBlur);
    RadialBlur.fBlurCenterX = m_fEndRadialBlurBlurCenterX;
    RadialBlur.fBlurCenterY = m_fEndRadialBlurBlurCenterY;
    RadialBlur.fBlurLength = m_fEndRadialBlurBlurLength;
    RadialBlur.fInFocusRadius = m_fEndRadialBlurInFocusRadius;
    RadialBlur.fOutFocusRadius = m_fEndRadialBlurOutFocusRadius;
    pRadialBlur->SetEndRadialBlur(RadialBlur);
    pRadialBlur->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionRadialBlurDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
*/
