// ActionHideWorldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionHideWorldDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmHideWorldAction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionHideWorldDlg dialog


CActionHideWorldDlg::CActionHideWorldDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
: CDialog(CActionHideWorldDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
    m_iTotalFrames = pHideWorldAction->GetTotalFrames();
    unsigned int eHideWorldType = pHideWorldAction->GetHideWorldType();
    m_uiColor = (pHideWorldAction->GetColor()) & 0x00FFFFFF;
    m_uiColor = ((m_uiColor & 0xFF) << 16) | (m_uiColor & 0xFF00) | ((m_uiColor & 0xFF0000) >> 16);
    if (eHideWorldType == CAnmHideWorldAction::HWT_HIDE_ANIMATION_MODEL)
    {
        m_sHideWorldType = "隐藏场景动画模型";
    }
    else if (eHideWorldType == CAnmHideWorldAction::HWT_HIDE_WORLD)
    {
        m_sHideWorldType = "隐藏全部场景";
    }
    //{{AFX_DATA_INIT(CActionScaleDlg)
    //}}AFX_DATA_INIT
}


void CActionHideWorldDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CActionHideWorldDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_CBString(pDX, IDC_HIDE_WORLD_TYPE, m_sHideWorldType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionHideWorldDlg, CDialog)
//{{AFX_MSG_MAP(CActionHideWorldDlg)
ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
ON_BN_CLICKED(IDC_BUTTON_CHOOSE_BACKGROUND_COLOR, OnButtonChooseColor)
ON_WM_DRAWITEM()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CActionHideWorldDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionHideWorldDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmHideWorldAction *pHideWorldAction = dynamic_cast<CAnmHideWorldAction*>(pAction);
    if (m_sHideWorldType == "隐藏场景动画模型")
    {
        pHideWorldAction->SetHideWorldType(CAnmHideWorldAction::HWT_HIDE_ANIMATION_MODEL);
    }
    else if (m_sHideWorldType == "隐藏全部场景")
    {
        pHideWorldAction->SetHideWorldType(CAnmHideWorldAction::HWT_HIDE_WORLD);
    }
    pHideWorldAction->SetColor(0xFF000000 | ((m_uiColor & 0xFF) << 16) | (m_uiColor & 0xFF00) | ((m_uiColor & 0xFF0000) >> 16));
    pHideWorldAction->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionHideWorldDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionHideWorldDlg::OnButtonChooseColor() 
{
    // TODO: Add your control notification handler code here
    CColorDialog dlg;
    if (dlg.DoModal())
    {
        m_uiColor = dlg.GetColor();
        m_uiColor &= 0x00FFFFFF;
        InvalidateRect(NULL);
    }
}

void CActionHideWorldDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
    // TODO: Add your message handler code here and/or call default
    
    if (nIDCtl == IDC_BUTTON_CHOOSE_BACKGROUND_COLOR)
    {
        CRect rc;
        ::GetClientRect(lpDrawItemStruct->hwndItem, rc);
        CBrush brush(m_uiColor);
        HGDIOBJ hOldBrush = SelectObject(lpDrawItemStruct->hDC, brush);
        Rectangle(lpDrawItemStruct->hDC, rc.left, rc.top, rc.right, rc.bottom);
        SelectObject( lpDrawItemStruct->hDC, hOldBrush);
    }
    else
    {
        CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
    }
}

