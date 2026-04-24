// ActionMotionBlurDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionMotionBlurDlg.h"
// #include "Animator/Animator.h"
// #include "Animator/AnmObjectManager.h"
// #include "Animator/AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionMotionBlurDlg dialog

/*
CActionMotionBlurDlg::CActionMotionBlurDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionMotionBlurDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMotionBlur *pMotionBlur = dynamic_cast<CAnmMotionBlur*>(pAction);
    m_fStartMotionBlurBlurMax = pMotionBlur->GetStartMotionBlur().fBlurMax;
    m_fStartMotionBlurBlurScale = pMotionBlur->GetStartMotionBlur().fBlurScale;
    m_fStartMotionBlurBlurScalePosition = pMotionBlur->GetStartMotionBlur().fBlurScalePosition;
    m_fStartMotionBlurBlurScaleRotation = pMotionBlur->GetStartMotionBlur().fBlurScaleRotation;
    m_fStartMotionInFocusDistance = pMotionBlur->GetStartMotionBlur().fInFocusDistance;
    m_fStartMotionOutFocusDistance = pMotionBlur->GetStartMotionBlur().fOutFocusDistance;
    m_fEndMotionBlurBlurMax = pMotionBlur->GetEndMotionBlur().fBlurMax;
    m_fEndMotionBlurBlurScale = pMotionBlur->GetEndMotionBlur().fBlurScale;
    m_fEndMotionBlurBlurScalePosition = pMotionBlur->GetEndMotionBlur().fBlurScalePosition;
    m_fEndMotionBlurBlurScaleRotation = pMotionBlur->GetEndMotionBlur().fBlurScaleRotation;
    m_fEndMotionInFocusDistance = pMotionBlur->GetEndMotionBlur().fInFocusDistance;
    m_fEndMotionOutFocusDistance = pMotionBlur->GetEndMotionBlur().fOutFocusDistance;
	m_iTotalFrames = pMotionBlur->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionMotionBlurDlg)
	//}}AFX_DATA_INIT
}


void CActionMotionBlurDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionMotionBlurDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_MAX, m_fStartMotionBlurBlurMax);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_SCALE, m_fStartMotionBlurBlurScale);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_SCALE_POSITION, m_fStartMotionBlurBlurScalePosition);
	DDX_Text(pDX, IDC_START_BLUR_BLUR_SCALE_ROTATION, m_fStartMotionBlurBlurScaleRotation);
	DDX_Text(pDX, IDC_START_IN_FOCUS_DISTANCE, m_fStartMotionInFocusDistance);
	DDX_Text(pDX, IDC_START_OUT_FOCUS_DISTANCE, m_fStartMotionOutFocusDistance);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_MAX, m_fEndMotionBlurBlurMax);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_SCALE, m_fEndMotionBlurBlurScale);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_SCALE_POSITION, m_fEndMotionBlurBlurScalePosition);
	DDX_Text(pDX, IDC_END_BLUR_BLUR_SCALE_ROTATION, m_fEndMotionBlurBlurScaleRotation);
	DDX_Text(pDX, IDC_END_IN_FOCUS_DISTANCE, m_fEndMotionInFocusDistance);
	DDX_Text(pDX, IDC_END_OUT_FOCUS_DISTANCE, m_fEndMotionOutFocusDistance);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionMotionBlurDlg, CDialog)
	//{{AFX_MSG_MAP(CActionMotionBlurDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionMotionBlurDlg message handlers

void CActionMotionBlurDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionMotionBlurDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMotionBlur *pMotionBlur = dynamic_cast<CAnmMotionBlur*>(pAction);
    A3DPOST_EFFECT_MOTIONBLUR_PARAM motionBlur;
    motionBlur.fBlurMax = m_fStartMotionBlurBlurMax;
    motionBlur.fBlurScale = m_fStartMotionBlurBlurScale;
    motionBlur.fBlurScalePosition = m_fStartMotionBlurBlurScalePosition;
    motionBlur.fBlurScaleRotation = m_fStartMotionBlurBlurScaleRotation;
    motionBlur.fInFocusDistance = m_fStartMotionInFocusDistance;
    motionBlur.fOutFocusDistance = m_fStartMotionOutFocusDistance;
    pMotionBlur->SetStartMotionBlur(motionBlur);
    motionBlur.fBlurMax = m_fEndMotionBlurBlurMax;
    motionBlur.fBlurScale = m_fEndMotionBlurBlurScale;
    motionBlur.fBlurScalePosition = m_fEndMotionBlurBlurScalePosition;
    motionBlur.fBlurScaleRotation = m_fEndMotionBlurBlurScaleRotation;
    motionBlur.fInFocusDistance = m_fEndMotionInFocusDistance;
    motionBlur.fOutFocusDistance = m_fEndMotionOutFocusDistance;
    pMotionBlur->SetEndMotionBlur(motionBlur);
    pMotionBlur->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionMotionBlurDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
*/