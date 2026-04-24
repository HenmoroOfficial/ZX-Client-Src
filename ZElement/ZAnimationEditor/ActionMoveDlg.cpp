// ActionMoveDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionMoveDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmMove.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionMoveDlg dialog

CActionMoveDlg::CActionMoveDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionMoveDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
    m_fStartPosX = pMove->GetStartPos().x;
    m_fStartPosY = pMove->GetStartPos().y;
    m_fStartPosZ = pMove->GetStartPos().z;
    m_fEndPosX = pMove->GetEndPos().x;
    m_fEndPosY = pMove->GetEndPos().y;
    m_fEndPosZ = pMove->GetEndPos().z;
    A3DQUATERNION qOri = pMove->GetOrientation();
    m_fOriW = qOri.w;
    m_fOriX = qOri.x;
    m_fOriY = qOri.y;
    m_fOriZ = qOri.z;
    m_fRotateAngle = pMove->GetAngle();
    int axisType = pMove->GetRotateAxis();
    if (axisType == CAnmMove::ROTATE_AXIS_X)
    {
        m_sRotateAxisType = "XÖá";
    }
    else if (axisType == CAnmMove::ROTATE_AXIS_Y)
    {
        m_sRotateAxisType = "YÖá";
    }
    else if (axisType == CAnmMove::ROTATE_AXIS_Z)
    {
        m_sRotateAxisType = "ZÖá";
    }
	m_fAccelerationPercent = pMove->GetAccelerationPercent();
    m_iTotalFrames = pMove->GetTotalFrames();
    m_bCloseTerrain = pMove->GetCloseTerrain();
    //{{AFX_DATA_INIT(CActionMoveDlg)
	//}}AFX_DATA_INIT
}

void CActionMoveDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionMoveDlg)
	DDX_Check(pDX, IDC_CLOSE_TERRAIN, m_bCloseTerrain);
	DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_POS_X, m_fStartPosX);
    DDX_Text(pDX, IDC_START_POS_Y, m_fStartPosY);
	DDX_Text(pDX, IDC_START_POS_Z, m_fStartPosZ);
	DDX_Text(pDX, IDC_END_POS_X, m_fEndPosX);
	DDX_Text(pDX, IDC_END_POS_Y, m_fEndPosY);
	DDX_Text(pDX, IDC_END_POS_Z, m_fEndPosZ);
	DDX_Text(pDX, IDC_ACCELERATION_PERCENT, m_fAccelerationPercent);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_MOVE_ORI_W, m_fOriW);
	DDX_Text(pDX, IDC_MOVE_ORI_X, m_fOriX);
	DDX_Text(pDX, IDC_MOVE_ORI_Y, m_fOriY);
	DDX_Text(pDX, IDC_MOVE_ORI_Z, m_fOriZ);
	DDX_Text(pDX, IDC_ROTATE_ANGLE, m_fRotateAngle);
	DDX_CBString(pDX, IDC_ROTATE_AXIS_TYPE, m_sRotateAxisType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionMoveDlg, CDialog)
	//{{AFX_MSG_MAP(CActionMoveDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_CLOSE_TERRAIN, OnCloseTerrain)
	ON_BN_CLICKED(IDC_BUTTON_SET_START_POS, OnButtonSetStartPos)
	ON_BN_CLICKED(IDC_BUTTON_SET_END_POS, OnButtonSetEndPos)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_SET_ORIENTATION, OnButtonSetOrientation)
	ON_CBN_SELCHANGE(IDC_ROTATE_AXIS_TYPE, OnSelchangeRotateAxisType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionMoveDlg message handlers

void CActionMoveDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMove *pMove = dynamic_cast<CAnmMove*>(pAction);
    pMove->SetStartPos(A3DVECTOR3(m_fStartPosX, m_fStartPosY, m_fStartPosZ));
    pMove->SetEndPos(A3DVECTOR3(m_fEndPosX, m_fEndPosY, m_fEndPosZ));
    pMove->SetOrientation(A3DQUATERNION(m_fOriW, m_fOriX, m_fOriY, m_fOriZ));
    pMove->SetAngle(m_fRotateAngle);
    if (m_sRotateAxisType == "XÖá")
    {
        pMove->SetRotateAxis(CAnmMove::ROTATE_AXIS_X);
    }
    else if (m_sRotateAxisType == "YÖá")
    {
        pMove->SetRotateAxis(CAnmMove::ROTATE_AXIS_Y);
    }
    else if (m_sRotateAxisType == "ZÖá")
    {
        pMove->SetRotateAxis(CAnmMove::ROTATE_AXIS_Z);
    }
    pMove->SetAccelerationPercent(m_fAccelerationPercent);
    pMove->SetTotalFrames(m_iTotalFrames);
    pMove->SetCloseTerrain(m_bCloseTerrain ? true : false);
    EndDialog(0);
}

void CActionMoveDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CActionMoveDlg::OnCloseTerrain() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
}

void CActionMoveDlg::OnButtonSetStartPos() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    if (pObject)
    {
        m_fStartPosX = pObject->GetPos().x;
        m_fStartPosY = pObject->GetPos().y;
        m_fStartPosZ = pObject->GetPos().z;
    }
    UpdateData(0);
}

void CActionMoveDlg::OnButtonSetEndPos() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    if (pObject)
    {
        m_fEndPosX = pObject->GetPos().x;
        m_fEndPosY = pObject->GetPos().y;
        m_fEndPosZ = pObject->GetPos().z;
    }
    UpdateData(0);
}

void CActionMoveDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("È·¶¨ÊÇ·ñÒªÉ¾³ý¸ÃÊÂ¼þ?"), _T("¶¯»­±à¼­Æ÷"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionMoveDlg::OnButtonSetOrientation() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(pAction->GetObjectID());
    if (pObject)
    {
        pObject->UpdateOrientation();
        A3DQUATERNION qOri = pObject->GetOrientation();
        m_fOriW = qOri.w;
        m_fOriX = qOri.x;
        m_fOriY = qOri.y;
        m_fOriZ = qOri.z;
    }
    UpdateData(0);
}

void CActionMoveDlg::OnSelchangeRotateAxisType() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_ROTATE_AXIS_TYPE, m_sRotateAxisType);
}
