// ActionHookDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionHookDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmHook.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionHookDlg dialog


CActionHookDlg::CActionHookDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionHookDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
    m_iTotalFrames = pHook->GetTotalFrames();
    m_sHangerName = pHook->GetHangerName();
    m_sHookName = pHook->GetHookName();
    m_uiHookObjectID = pHook->GetHookObjectID();
    m_sElseHookName = pHook->GetElseHookName();
    unsigned int uiID = pHook->GetObjectID();
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(uiID);
    if (pObject)
    {
        if (pObject->GetObjectType() == CAnmObject::TYPE_NPC)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            pNPC->GetHookList(m_vHookNameList);
        }
    }
	//{{AFX_DATA_INIT(CActionHookDlg)
	//}}AFX_DATA_INIT
}

void CActionHookDlg::SetHookNames()
{
    int index = 0;
    m_cHookName.ResetContent();
    for (unsigned int i=0; i<m_vHookNameList.size(); ++i)
    {
        const wchar_t *pStr = m_vHookNameList[i];
        if (m_sHookName == m_vHookNameList[i])
        {
            index = i;
        }
        m_cHookName.InsertString(-1, pStr);
        m_cHookName.SetItemData(i, i);
    }
    m_cHookName.SetCurSel(index);
}

void CActionHookDlg::SetElseHookNames()
{
    int index = 0;
    m_cElseHookName.ResetContent();
    UpdateData(TRUE);
    CAnmObject *pObject = g_Animator.GetObjectManager()->GetObject(m_uiHookObjectID);
    if (pObject)
    {
        if (pObject->GetObjectType() == CAnmObject::TYPE_NPC)
        {
            CAnmNPC *pNPC = dynamic_cast<CAnmNPC*>(pObject);
            pNPC->GetHookList(m_vElseHookNameList);
        }
    }
    for (unsigned int i=0; i<m_vElseHookNameList.size(); ++i)
    {
        const wchar_t *pStr = m_vElseHookNameList[i];
        if (m_sElseHookName == m_vElseHookNameList[i])
        {
            index = i;
        }
        m_cElseHookName.InsertString(-1, pStr);
        m_cElseHookName.SetItemData(i, i);
    }
    m_cElseHookName.SetCurSel(index);
}


void CActionHookDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionHookDlg)
    DDX_Control(pDX, IDC_HOOK_NAME, m_cHookName);
    DDX_Control(pDX, IDC_ELSE_HOOK_NAME, m_cElseHookName);
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_HANGER_NAME, m_sHangerName);
    DDX_CBString(pDX, IDC_HOOK_NAME, m_sHookName);
    DDX_Text(pDX, IDC_HOOK_OBJECT_ID, m_uiHookObjectID);
    DDX_CBString(pDX, IDC_ELSE_HOOK_NAME, m_sElseHookName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionHookDlg, CDialog)
	//{{AFX_MSG_MAP(CActionHookDlg)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_CBN_DROPDOWN(IDC_HOOK_NAME, OnDropdownHookName)
	ON_CBN_DROPDOWN(IDC_ELSE_HOOK_NAME, OnDropdownElseHookName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionHookDlg message handlers

void CActionHookDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmHook *pHook = dynamic_cast<CAnmHook*>(pAction);
    pHook->SetTotalFrames(m_iTotalFrames);
    pHook->SetHangerName(ACString(m_sHangerName));
    pHook->SetHookName(ACString(m_sHookName));
    pHook->SetHookObjectID(m_uiHookObjectID);
    pHook->SetElseHookName(ACString(m_sElseHookName));
    EndDialog(0);
}

void CActionHookDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CActionHookDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionHookDlg::OnDropdownHookName() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    SetHookNames();
}

void CActionHookDlg::OnDropdownElseHookName() 
{
	// TODO: Add your control notification handler code here
	UpdateData(TRUE);
    SetElseHookNames();
}

