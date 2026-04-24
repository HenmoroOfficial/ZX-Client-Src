// ActionRotateDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionRotateDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmRotate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionRotateDlg dialog

CActionRotateDlg::CActionRotateDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionRotateDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
    A3DQUATERNION qOri = pRotate->GetStartRotate();
    m_fStartRotateW = qOri.w;
    m_fStartRotateX = qOri.x;
    m_fStartRotateY = qOri.y;
    m_fStartRotateZ = qOri.z;
    qOri = pRotate->GetEndRotate();
    m_fEndRotateW = qOri.w;
    m_fEndRotateX = qOri.x;
    m_fEndRotateY = qOri.y;
    m_fEndRotateZ = qOri.z;
	m_iTotalFrames = pRotate->GetTotalFrames();
	//{{AFX_DATA_INIT(CActionRotateDlg)
	//}}AFX_DATA_INIT
}


void CActionRotateDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionRotateDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_ROTATE_START_W, m_fStartRotateW);
	DDX_Text(pDX, IDC_ROTATE_START_X, m_fStartRotateX);
	DDX_Text(pDX, IDC_ROTATE_START_Y, m_fStartRotateY);
	DDX_Text(pDX, IDC_ROTATE_START_Z, m_fStartRotateZ);
    DDX_Text(pDX, IDC_ROTATE_END_W, m_fEndRotateW);
	DDX_Text(pDX, IDC_ROTATE_END_X, m_fEndRotateX);
	DDX_Text(pDX, IDC_ROTATE_END_Y, m_fEndRotateY);
	DDX_Text(pDX, IDC_ROTATE_END_Z, m_fEndRotateZ);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionRotateDlg, CDialog)
	//{{AFX_MSG_MAP(CActionRotateDlg)
    ON_BN_CLICKED(IDC_BUTTON_SET_START_ROTATE, OnButtonSetStartRotate)
    ON_BN_CLICKED(IDC_BUTTON_SET_END_ROTATE, OnButtonSetEndRotate)
    ON_BN_CLICKED(IDC_USE_SQUAD, OnButtonUseSquad)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionRotateDlg message handlers

void CActionRotateDlg::OnButtonSetStartRotate() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    if (pObject)
    {
        pObject->UpdateOrientation();
        A3DQUATERNION qOri = pObject->GetOrientation();
        m_fStartRotateW = qOri.w;
        m_fStartRotateX = qOri.x;
        m_fStartRotateY = qOri.y;
        m_fStartRotateZ = qOri.z;
    }
    UpdateData(0);
}


void CActionRotateDlg::OnButtonSetEndRotate() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    if (pObject)
    {
        pObject->UpdateOrientation();
        A3DQUATERNION qOri = pObject->GetOrientation();
        m_fEndRotateW = qOri.w;
        m_fEndRotateX = qOri.x;
        m_fEndRotateY = qOri.y;
        m_fEndRotateZ = qOri.z;
    }
    UpdateData(0);
}

void CActionRotateDlg::OnButtonUseSquad()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
}

void CActionRotateDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionRotateDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmRotate *pRotate = dynamic_cast<CAnmRotate*>(pAction);
    pRotate->SetStartRotate(A3DQUATERNION(m_fStartRotateW, m_fStartRotateX, m_fStartRotateY, m_fStartRotateZ));
    pRotate->SetEndRotate(A3DQUATERNION(m_fEndRotateW, m_fEndRotateX, m_fEndRotateY, m_fEndRotateZ));
    pRotate->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionRotateDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

