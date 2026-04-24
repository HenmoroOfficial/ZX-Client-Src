// NewActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "NewActionDlg.h"
#include "Animator.h"
#include "AnmActionChannel.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewNodeDlg dialog


CNewActionDlg::CNewActionDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CNewActionDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
	m_sActionType = _T("移动");
	//{{AFX_DATA_INIT(CNewActionDlg)
	//}}AFX_DATA_INIT
}


void CNewActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewActionDlg)
	DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_CBString(pDX, IDC_ACTION_TYPE, m_sActionType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewActionDlg, CDialog)
	//{{AFX_MSG_MAP(CNewActionDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ACTION_OK, OnButtonNewActionOk)
	ON_BN_CLICKED(IDC_BUTTON_NEW_ACTION_CANCEL, OnButtonNewActionCancel)
	ON_CBN_SELCHANGE(IDC_ACTION_TYPE, OnSelchangeActionType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewActionDlg message handlers


void CNewActionDlg::OnButtonNewActionOk() 
{
	// TODO: Add your control notification handler code here
  if (m_sActionType != "")
    {
        unsigned int uiID;
        if (m_sActionType == "移动")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_MOVE);
        }
        else if (m_sActionType == "跳跃")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_JUMP);
        }
        else if (m_sActionType == "Bezier移动")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_BEZIER_MOVE);
        }
        else if (m_sActionType == "动作")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_MOTION);
        }
        else if (m_sActionType == "显隐")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_DISPLAY_HIDE);
        }
        else if (m_sActionType == "对话")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_DIALOGUE);
        }
        else if (m_sActionType == "音效")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_SOUND);
        }
        else if (m_sActionType == "挂接")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_HOOK);
        }
        else if (m_sActionType == "速度控制")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_SPEED_CONTROL);
        }
        else if (m_sActionType == "旋转")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_ROTATE);
        }
        else if (m_sActionType == "缩放")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_SCALE);
        }
        else if (m_sActionType == "FOV")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_FOV);
        }
        else if (m_sActionType == "镜头过渡")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_CAMERA_FILTER);
        }
        else if (m_sActionType == "镜头抖动")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_CAMERA_OSCILLATORY);
        }
        else if (m_sActionType == "半透明")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_TRANSPARENT);
        }
        else if (m_sActionType == "隐藏场景")
        {
            uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_HIDE_WORLD_ACTION);
        }
		else if(m_sActionType == "镜头校准")
		{
			uiID = m_pChannel->GetChannel()->CreateAction(CAnmAction::TYPE_CAMERA_BIND);
		}
        CAnmAction *pAction = m_pChannel->GetChannel()->GetAction(uiID);
        pAction->SetStartTime(m_pNode->GetStartTime());
        pAction->BindObjectID(m_pChannel->GetObjectID());
        m_pNode->SetAction(pAction);
        g_Animator.Stop();
    }
    theApp.OnChanged();
	EndDialog(0);
}

void CNewActionDlg::OnButtonNewActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CNewActionDlg::OnSelchangeActionType() 
{
	// TODO: Add your control notification handler code here
	GetDlgItemText(IDC_ACTION_TYPE, m_sActionType);
}
