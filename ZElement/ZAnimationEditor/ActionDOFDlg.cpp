// ActionDOFDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionDOFDlg.h"
// #include "Animator/Animator.h"
// #include "Animator/AnmObjectManager.h"
// #include "Animator/AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionDOFDlg dialog

/*
CActionDOFDlg::CActionDOFDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionDOFDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDOF *pDOF = dynamic_cast<CAnmDOF*>(pAction);
    m_fStartDOFNearBlur = pDOF->GetStartDOF().fNearBlur;
    m_fStartDOFNearFocus = pDOF->GetStartDOF().fNearFocus;
    m_fStartDOFFarFocus = pDOF->GetStartDOF().fFarFocus;
    m_fStartDOFFarBlur = pDOF->GetStartDOF().fFarBlur;
    m_fStartDOFClampBlur = pDOF->GetStartDOF().fClampBlurFar;
    m_fEndDOFNearBlur = pDOF->GetEndDOF().fNearBlur;
    m_fEndDOFNearFocus = pDOF->GetEndDOF().fNearFocus;
    m_fEndDOFFarFocus = pDOF->GetEndDOF().fFarFocus;
    m_fEndDOFFarBlur = pDOF->GetEndDOF().fFarBlur;
    m_fEndDOFClampBlur = pDOF->GetEndDOF().fClampBlurFar;
	m_iTotalFrames = pDOF->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionDOFDlg)
	//}}AFX_DATA_INIT
}


void CActionDOFDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDOFDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_NEAR_BLUR, m_fStartDOFNearBlur);
	DDX_Text(pDX, IDC_START_NEAR_FOCUS, m_fStartDOFNearFocus);
	DDX_Text(pDX, IDC_START_FAR_FOCUS, m_fStartDOFFarFocus);
	DDX_Text(pDX, IDC_START_FAR_BLUR, m_fStartDOFFarBlur);
	DDX_Text(pDX, IDC_START_CLAMP_BLUR, m_fStartDOFClampBlur);
	DDX_Text(pDX, IDC_END_NEAR_BLUR, m_fEndDOFNearBlur);
	DDX_Text(pDX, IDC_END_NEAR_FOCUS, m_fEndDOFNearFocus);
	DDX_Text(pDX, IDC_END_FAR_FOCUS, m_fEndDOFFarFocus);
	DDX_Text(pDX, IDC_END_FAR_BLUR, m_fEndDOFFarBlur);
	DDX_Text(pDX, IDC_END_CLAMP_BLUR, m_fEndDOFClampBlur);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDOFDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDOFDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDOFDlg message handlers

void CActionDOFDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionDOFDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDOF *pDOF = dynamic_cast<CAnmDOF*>(pAction);
    A3DPOST_EFFECT_DOF_PARAM DOF;
    DOF.fNearBlur = m_fStartDOFNearBlur;
    DOF.fNearFocus = m_fStartDOFNearFocus;
    DOF.fFarFocus = m_fStartDOFFarFocus;
    DOF.fFarBlur = m_fStartDOFFarBlur;
    DOF.fClampBlurFar = m_fStartDOFClampBlur;
    pDOF->SetStartDOF(DOF);
    DOF.fNearBlur = m_fEndDOFNearBlur;
    DOF.fNearFocus = m_fEndDOFNearFocus;
    DOF.fFarFocus = m_fEndDOFFarFocus;
    DOF.fFarBlur = m_fEndDOFFarBlur;
    DOF.fClampBlurFar = m_fEndDOFClampBlur;
    pDOF->SetEndDOF(DOF);
    pDOF->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionDOFDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
*/
