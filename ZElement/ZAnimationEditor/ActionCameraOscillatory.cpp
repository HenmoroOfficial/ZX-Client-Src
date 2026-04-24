// ActionCameraOscillatory.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionCameraOscillatory.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmCameraOscillatory.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionCameraOscillatory dialog


CActionCameraOscillatory::CActionCameraOscillatory(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
: CDialog(CActionCameraOscillatory::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmCameraOscillatory *pCameraOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);

    unsigned int eOscillatory = pCameraOscillatory->GetOscillatoryType();
    if (eOscillatory == CAnmCameraOscillatory::OSCILLATORY_TYPE_DIRECTION)
    {
        m_sOscillatoryType = "定向";
    }
    else if (eOscillatory == CAnmCameraOscillatory::OSCILLATORY_TYPE_RANDOM)
    {
        m_sOscillatoryType = "随机";
    }

    int eOscillatoryAxis = pCameraOscillatory->GetOscillatoryAxis();
    if (eOscillatoryAxis == CAnmCameraOscillatory::OSCILLATORY_AXIS_X)
    {
        m_sOscillatoryAxis = "X轴";
    }
    else if (eOscillatoryAxis == CAnmCameraOscillatory::OSCILLATORY_AXIS_Y)
    {
        m_sOscillatoryAxis = "Y轴";
    }
    else if (eOscillatoryAxis == CAnmCameraOscillatory::OSCILLATORY_AXIS_Z)
    {
        m_sOscillatoryAxis = "Z轴";
    }

    int eOscillatoryAttenuation = pCameraOscillatory->GetAttenuation();
    if (eOscillatoryAttenuation == CAnmCameraOscillatory::OSCILLATORY_ATTENUATION_NONE)
    {
        m_sOscillatoryAttenuation = "恒定";
    }
    else if (eOscillatoryAttenuation == CAnmCameraOscillatory::OSCILLATORY_AXIS_Y)
    {
        m_sOscillatoryAttenuation = "递增";
    }
    else if (eOscillatoryAttenuation == CAnmCameraOscillatory::OSCILLATORY_AXIS_Z)
    {
        m_sOscillatoryAttenuation = "衰减";
    }

    m_fDisplacement = pCameraOscillatory->GetDisplacement();
    m_fFrequency = pCameraOscillatory->GetFrequency();
    m_iTotalFrames = pCameraOscillatory->GetTotalFrames();
    //{{AFX_DATA_INIT(CActionCameraOscillatory)
	//}}AFX_DATA_INIT
}


void CActionCameraOscillatory::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CActionCameraOscillatory)
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_CBString(pDX, IDC_OSCILLATORY_TYPE, m_sOscillatoryType);
    DDX_CBString(pDX, IDC_OSCILLATORY_AXIS, m_sOscillatoryAxis);
	DDX_CBString(pDX, IDC_OSCILLATORY_ATTENUATION, m_sOscillatoryAttenuation);
	DDX_Text(pDX, IDC_DISPLACEMENT, m_fDisplacement);
	DDX_Text(pDX, IDC_FREQUENCY, m_fFrequency);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionCameraOscillatory, CDialog)
//{{AFX_MSG_MAP(CActionCameraOscillatory)
ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionCameraOscillatory message handlers

void CActionCameraOscillatory::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmCameraOscillatory *pCameraOscillatory = dynamic_cast<CAnmCameraOscillatory*>(pAction);

    if (m_sOscillatoryType == "定向")
    {
        pCameraOscillatory->SetOscillatoryType(CAnmCameraOscillatory::OSCILLATORY_TYPE_DIRECTION);
    }
    else if (m_sOscillatoryType == "随机")
    {
        pCameraOscillatory->SetOscillatoryType(CAnmCameraOscillatory::OSCILLATORY_TYPE_RANDOM);
    }

    if (m_sOscillatoryAxis == "X轴")
    {
        pCameraOscillatory->SetOscillatoryAxis(CAnmCameraOscillatory::OSCILLATORY_AXIS_X);
    }
    else if (m_sOscillatoryAxis == "Y轴")
    {
        pCameraOscillatory->SetOscillatoryAxis(CAnmCameraOscillatory::OSCILLATORY_AXIS_Y);
    }
    else if (m_sOscillatoryAxis == "Z轴")
    {
        pCameraOscillatory->SetOscillatoryAxis(CAnmCameraOscillatory::OSCILLATORY_AXIS_Z);
    }

    if (m_sOscillatoryAttenuation == "恒定")
    {
        pCameraOscillatory->SetAttenuation(CAnmCameraOscillatory::OSCILLATORY_ATTENUATION_NONE);
    }
    else if (m_sOscillatoryAttenuation == "递增")
    {
        pCameraOscillatory->SetAttenuation(CAnmCameraOscillatory::OSCILLATORY_ATTENUATION_UP);
    }
    else if (m_sOscillatoryAttenuation == "衰减")
    {
        pCameraOscillatory->SetAttenuation(CAnmCameraOscillatory::OSCILLATORY_ATTENUATION_DOWN);
    }

    pCameraOscillatory->SetDisplacement(m_fDisplacement);
    pCameraOscillatory->SetFrequency(m_fFrequency);
    pCameraOscillatory->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionCameraOscillatory::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionCameraOscillatory::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

