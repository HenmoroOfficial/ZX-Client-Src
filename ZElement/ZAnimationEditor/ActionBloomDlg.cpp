// ActionBloomDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionBloomDlg.h"
// #include "Animator/Animator.h"
// #include "Animator/AnmObjectManager.h"
// #include "Animator/AnmActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionBloomDlg dialog

/*
CActionBloomDlg::CActionBloomDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionBloomDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmBloom *pBloom = dynamic_cast<CAnmBloom*>(pAction);
    m_fStartBloomBlurSize = pBloom->GetStartBloom().fBlurSize;
    m_fStartBloomBrightThreshold = pBloom->GetStartBloom().fBrightThreshold;
    m_fStartBloomBrightScale = pBloom->GetStartBloom().fBrightScale;
    m_fEndBloomBlurSize = pBloom->GetEndBloom().fBlurSize;
    m_fEndBloomBrightThreshold = pBloom->GetEndBloom().fBrightThreshold;
    m_fEndBloomBrightScale = pBloom->GetEndBloom().fBrightScale;
	m_iTotalFrames = pBloom->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionBloomDlg)
	//}}AFX_DATA_INIT
}


void CActionBloomDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionBloomDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_START_BLUR_SIZE, m_fStartBloomBlurSize);
	DDX_Text(pDX, IDC_START_BRIGHT_THRESHOLD, m_fStartBloomBrightThreshold);
	DDX_Text(pDX, IDC_START_BRIGHT_SCALE, m_fStartBloomBrightScale);
	DDX_Text(pDX, IDC_END_BLUR_SIZE, m_fEndBloomBlurSize);
	DDX_Text(pDX, IDC_END_BRIGHT_THRESHOLD, m_fEndBloomBrightThreshold);
	DDX_Text(pDX, IDC_END_BRIGHT_SCALE, m_fEndBloomBrightScale);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionBloomDlg, CDialog)
	//{{AFX_MSG_MAP(CActionBloomDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionBloomDlg message handlers

void CActionBloomDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionBloomDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmBloom *pBloom = dynamic_cast<CAnmBloom*>(pAction);
    A3DPOST_EFFECT_BLOOM_PARAM bloom;
    bloom.fBlurSize = m_fStartBloomBlurSize;
    bloom.fBrightThreshold = m_fStartBloomBrightThreshold;
    bloom.fBrightScale = m_fStartBloomBrightScale;
    pBloom->SetStartBloom(bloom);
    bloom.fBlurSize = m_fEndBloomBlurSize;
    bloom.fBrightThreshold = m_fEndBloomBrightThreshold;
    bloom.fBrightScale = m_fEndBloomBrightScale;
    pBloom->SetEndBloom(bloom);
    pBloom->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionBloomDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

*/