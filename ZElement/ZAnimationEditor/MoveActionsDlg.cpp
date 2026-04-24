// MoveActionsDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "MoveActionsDlg.h"
#include "Animator.h"
#include "AnmActionManager.h"
#include "AnmActionGroup.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMoveActionsDlg dialog


CMoveActionsDlg::CMoveActionsDlg(CWnd* pParent, CTrackGroup *pGroup)
	: CDialog(CMoveActionsDlg::IDD, pParent)
{
    m_pGroup = pGroup;
     m_dwStartFrame = 1 + g_Animator.GetStartTime() / 50;
 	m_dwEndFrame = g_Animator.GetEndTime() / 50;
    m_sMoveOperation = "当前选定通道";
	m_dwMoveToFrame = m_dwStartFrame;
    m_sLog = _T("Log:");
	//{{AFX_DATA_INIT(CMoveActionsDlg)
	//}}AFX_DATA_INIT
}


void CMoveActionsDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMoveActionsDlg)
	DDX_Text(pDX, IDC_START_TIME, m_dwStartFrame);
	DDX_Text(pDX, IDC_END_TIME, m_dwEndFrame);
	DDX_Text(pDX, IDC_MOVETO_FRAME, m_dwMoveToFrame);
	DDX_Text(pDX, IDC_MOVE_ACTIONS_LOG, m_sLog);
	DDX_CBString(pDX, IDC_MOVE_OPERATION, m_sMoveOperation);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMoveActionsDlg, CDialog)
	//{{AFX_MSG_MAP(CMoveActionsDlg)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_ACTIONS_OK, OnButtonMoveActionsOk)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_ACTIONS_CANCEL, OnButtonMoveActionsCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMoveActionsDlg message handlers

void CMoveActionsDlg::OnButtonMoveActionsOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);

    g_Animator.Stop();
    theApp.OnChanged();

    if (!m_dwMoveToFrame || !m_dwStartFrame || !m_dwStartFrame)
    {
        m_sLog = _T("Log: Error 帧数不能为0!");
        UpdateData(0);
        return;
    }

    if (m_dwMoveToFrame == m_dwStartFrame)
    {
        m_sLog = _T("Log: Error 没有发生移动!");
        UpdateData(0);
        return;
    }

    if (m_dwStartFrame > m_dwEndFrame || 50 * (m_dwEndFrame-1) > g_Animator.GetTotalTime())
    {
        m_sLog = _T("Log: Error 起始或终止帧数错误!");
        UpdateData(0);
        return;
    }
    bool bMoveLeft = false;
    DWORD dwOffset = m_dwMoveToFrame - m_dwStartFrame;
    DWORD dwLength = m_dwEndFrame - m_dwStartFrame;
    if (m_dwMoveToFrame < m_dwStartFrame)
    {
        bMoveLeft = true;
        dwOffset = m_dwStartFrame - m_dwMoveToFrame;
    }

    DWORD deleteStartFrame = m_dwMoveToFrame;
    DWORD deleteEndFrame = m_dwMoveToFrame + dwLength;

    if (50 * (deleteEndFrame-1) > g_Animator.GetTotalTime())
    {
        m_sLog = _T("Log: Error 移动超出了动画总长度!");
        UpdateData(0);
        return;
    }

    if (bMoveLeft)
    {
        if (m_dwStartFrame < deleteEndFrame)
        {
            deleteEndFrame = m_dwStartFrame;
        }
    }
    else
    {
        if (m_dwEndFrame > deleteStartFrame)
        {
            deleteStartFrame = m_dwEndFrame;
        }
    }

    CAnmActionManager *pManager = g_Animator.GetActionManager();
    if (m_sMoveOperation == "所有组")
    {
        int deleteActionsCount = pManager->GetActionsCount(50 * (deleteStartFrame-1), 50 * (deleteEndFrame-1));
        if (deleteActionsCount)
        {
            int nBox = MessageBox(_T("确定是否要覆盖已有事件?"), _T("事件批量移动"), MB_ICONQUESTION | MB_YESNO);
            if (IDNO == nBox)
            {
                m_sLog = _T("Log:");
                UpdateData(0);
                return;
            }
        }
        pManager->MoveActions(50 * (m_dwStartFrame-1), 50 * (m_dwEndFrame-1), 50 * dwOffset, bMoveLeft);
    }
    else if (m_sMoveOperation == "当前选定组" && m_pGroup && m_pGroup->GetGroup())
    {
        int deleteActionsCount = m_pGroup->GetGroup()->GetActionsCount(50 * (deleteStartFrame-1), 50 * (deleteEndFrame-1));
        if (deleteActionsCount)
        {
            int nBox = MessageBox(_T("确定是否要覆盖已有事件?"), _T("事件批量移动"), MB_ICONQUESTION | MB_YESNO);
            if (IDNO == nBox)
            {
                m_sLog = _T("Log:");
                UpdateData(0);
                return;
            }
        }
        m_pGroup->GetGroup()->MoveActions(50 * (m_dwStartFrame-1), 50 * (m_dwEndFrame-1), 50 * dwOffset, bMoveLeft);
    }
    else if (m_sMoveOperation == "当前选定通道" && m_pGroup && m_pGroup->GetGroup())
    {
        CTrackChannel *pChannel = m_pGroup->GetSelectChannel();
        if (pChannel && pChannel->GetChannel())
        {
            int deleteActionsCount = pChannel->GetChannel()->GetActionsCount(50 * (deleteStartFrame-1), 50 * (deleteEndFrame-1));
            if (deleteActionsCount)
            {
                int nBox = MessageBox(_T("确定是否要覆盖已有事件?"), _T("事件批量移动"), MB_ICONQUESTION | MB_YESNO);
                if (IDNO == nBox)
                {
                    m_sLog = _T("Log:");
                    UpdateData(0);
                    return;
                }
            }
            pChannel->GetChannel()->MoveActions(50 * (m_dwStartFrame-1), 50 * (m_dwEndFrame-1), 50 * dwOffset, bMoveLeft);
        }
        else
        {
            m_sLog = _T("Log: Error 没有选中的通道!");
            UpdateData(0);
            return;
        }
    }
    else
    {
        m_sLog = _T("Log: Error 没有选中的分组!");
        UpdateData(0);
        return;
    }

    EndDialog(-1);
}

void CMoveActionsDlg::OnButtonMoveActionsCancel() 
{
	// TODO: Add your control notification handler code here
    EndDialog(0);
}
