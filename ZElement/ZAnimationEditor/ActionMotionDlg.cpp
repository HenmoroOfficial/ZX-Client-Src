// ActionMotionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionMotionDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmMotion.h"
#include "AnmCommonActionManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionMotionDlg dialog


CActionMotionDlg::CActionMotionDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionMotionDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
    unsigned int eMotionType = pMotion->GetMotionType();
    if (eMotionType == CAnmMotion::MT_ONCE)
    {
        m_sMotionType = "单次";
    }
    else if (eMotionType == CAnmMotion::MT_LOOP)
    {
        m_sMotionType = "循环";
    }
    else if (eMotionType == CAnmMotion::MT_ONE_FRAME)
    {
        m_sMotionType = "单帧";
    }
    m_iStartFrame = pMotion->GetStartMotionFrame();
    m_iBlendFrames = pMotion->GetBlendFrames();
    m_fSpeed = pMotion->GetActionSpeed();
    m_iTotalFrames = pMotion->GetTotalFrames();
    m_bShowGFX = pMotion->IsShowGFX();
    m_sActionName = pMotion->GetActionName();
    unsigned int uiID = pMotion->GetObjectID();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(uiID);
    if (pObject)
    {
        pObject->GetActionList(m_vActionNameList);
        std::sort(m_vActionNameList.begin(), m_vActionNameList.end());
    }
	m_bHostPlayer = uiID == 1;
	if (m_bHostPlayer)
	{
		CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
		if (pCommonActionMgr->IsEnable())
		{
			int commonActionCount = pCommonActionMgr->GetCommonActionCount();
			for (int i = commonActionCount-1; i >= 0; -- i)
			{
				const ACString* pCommanAction = pCommonActionMgr->GetCommonActionName(i);
				if (pCommanAction)
					m_vActionNameList.insert(m_vActionNameList.begin(), *pCommanAction);
			}
		}
	}
	//{{AFX_DATA_INIT(CActionMotionDlg)
	//}}AFX_DATA_INIT
}

void CActionMotionDlg::SetActionNames()
{
    int index = 0;
    m_cActionName.ResetContent();

    for (unsigned int i=0; i<m_vActionNameList.size(); ++i)
    {
        const wchar_t *pStr = m_vActionNameList[i];
        if (m_sActionName == m_vActionNameList[i])
        {
            index = i;
        }
        m_cActionName.InsertString(-1, pStr);
        m_cActionName.SetItemData(i, i);
    }
    m_cActionName.SetCurSel(index);
}

void CActionMotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionMotionDlg)
    DDX_Control(pDX, IDC_ACTION_NAME, m_cActionName);
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_MOTION_TYPE, m_sMotionType);
    DDX_Text(pDX, IDC_START_FRAME, m_iStartFrame);
    DDX_Text(pDX, IDC_BLEND_FRAMES, m_iBlendFrames);
    DDX_Text(pDX, IDC_MOTION_SPEED, m_fSpeed);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_CBString(pDX, IDC_ACTION_NAME, m_sActionName);
	DDX_Check(pDX, IDC_SHOW_GFX, m_bShowGFX);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionMotionDlg, CDialog)
	//{{AFX_MSG_MAP(CActionMotionDlg)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
    ON_CBN_SELCHANGE(IDC_MOTION_TYPE, OnSelchangeMotionType)
    ON_CBN_DROPDOWN(IDC_ACTION_NAME, OnDropdownActionName)
    ON_CBN_SELCHANGE(IDC_ACTION_NAME, OnSelchangeActionName)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionMotionDlg message handlers

void CActionMotionDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmMotion *pMotion = dynamic_cast<CAnmMotion*>(pAction);
    if (m_sMotionType == "单次")
    {
        pMotion->SetMotionType(CAnmMotion::MT_ONCE);
    }
    else if (m_sMotionType == "循环")
    {
        pMotion->SetMotionType(CAnmMotion::MT_LOOP);
    }
    else if (m_sMotionType == "单帧")
    {
        pMotion->SetMotionType(CAnmMotion::MT_ONE_FRAME);
    }
    pMotion->SetStartMotionFrame(m_iStartFrame);
    pMotion->SetBlendFrames(m_iBlendFrames);
    pMotion->SetActionSpeed(m_fSpeed);
    pMotion->SetTotalFrames(m_iTotalFrames);
    pMotion->SetActionName(ACString(m_sActionName));
    pMotion->SetShowGFX(m_bShowGFX ? true : false);
    EndDialog(0);
}

void CActionMotionDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionMotionDlg::OnSelchangeMotionType() 
{
    // TODO: Add your control notification handler code here
    GetDlgItemText(IDC_MOTION_TYPE, m_sMotionType);
}

void CActionMotionDlg::OnDropdownActionName() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    SetActionNames();
}

void CActionMotionDlg::OnSelchangeActionName() 
{
    // TODO: Add your control notification handler code here
    UpdateData(TRUE);
    int index = m_cActionName.GetCurSel();
    int id = m_cActionName.GetItemData(index);
	m_sActionName = m_vActionNameList[id];
    UpdateData(0);
}

void CActionMotionDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}
