// ActionDisplayHideDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "ActionDisplayHideDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmDisplayHide.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionDisplayHideDlg dialog


CActionDisplayHideDlg::CActionDisplayHideDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionDisplayHideDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
    unsigned int eVisableType = pDisplayHide->GetVisableType();
    if (eVisableType == CAnmDisplayHide::VT_HIDE)
    {
        m_sVisableType = "Òþ²Ø";
    }
    else if (eVisableType == CAnmDisplayHide::VT_DISPLAY)
    {
        m_sVisableType = "ÏÔÊ¾";
    }
    else if (eVisableType == CAnmDisplayHide::VT_BLACK_AND_WHITE)
    {
        m_sVisableType = "ºÚ°×";
    }
	//{{AFX_DATA_INIT(CActionDisplayHideDlg)
	//}}AFX_DATA_INIT
}


void CActionDisplayHideDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionDisplayHideDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_VISABLE_TYPE, m_sVisableType);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionDisplayHideDlg, CDialog)
	//{{AFX_MSG_MAP(CActionDisplayHideDlg)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
    ON_CBN_SELCHANGE(IDC_VISABLE_TYPE, OnSelchangeVisableType)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionDisplayHideDlg message handlers

void CActionDisplayHideDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmDisplayHide *pDisplayHide = dynamic_cast<CAnmDisplayHide*>(pAction);
    if (m_sVisableType == "Òþ²Ø")
    {
        pDisplayHide->SetVisableType(CAnmDisplayHide::VT_HIDE);
    }
    else if (m_sVisableType == "ÏÔÊ¾")
    {
        pDisplayHide->SetVisableType(CAnmDisplayHide::VT_DISPLAY);
    }
    else if (m_sVisableType == "ºÚ°×")
    {
        pDisplayHide->SetVisableType(CAnmDisplayHide::VT_BLACK_AND_WHITE);
    }
    EndDialog(0);
}

void CActionDisplayHideDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionDisplayHideDlg::OnSelchangeVisableType() 
{
    // TODO: Add your control notification handler code here
    GetDlgItemText(IDC_VISABLE_TYPE, m_sVisableType);
}

void CActionDisplayHideDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("È·¶¨ÊÇ·ñÒªÉ¾³ý¸ÃÊÂ¼þ?"), _T("¶¯»­±à¼­Æ÷"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}
