// TrackGroupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "TrackPanel.h"
#include "TrackGroupDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackGroupDlg dialog


CTrackGroupDlg::CTrackGroupDlg(CWnd* pParent, CTrackPanel *pPanel, CTrackGroup *pGroup)
	: CDialog(CTrackGroupDlg::IDD, pParent)
{
    m_pPanel = pPanel;
    m_pGroup = pGroup;
    m_uiID = pGroup->GetGroupID();
	m_sName = pGroup->GetGroupName();
	//{{AFX_DATA_INIT(CTrackGroupDlg)
	//}}AFX_DATA_INIT
}


void CTrackGroupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackGroupDlg)
	DDX_Text(pDX, IDC_GROUP_ID, m_uiID);
	DDX_Text(pDX, IDC_GROUP_NAME, m_sName);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CTrackGroupDlg, CDialog)
	//{{AFX_MSG_MAP(CTrackGroupDlg)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_CREATE_CHANNEL, OnButtonGroupCreateChannel)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_REMOVE, OnButtonGroupRemove)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_OK, OnButtonGroupOk)
	ON_BN_CLICKED(IDC_BUTTON_GROUP_CANCEL, OnButtonGroupCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackGroupDlg message handlers

void CTrackGroupDlg::OnButtonGroupCreateChannel() 
{
	// TODO: Add your control notification handler code here
}

void CTrackGroupDlg::OnButtonGroupRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该组?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
	{
        m_pPanel->DeleteGroup(m_pGroup->GetGroupID());
        EndDialog(-1);
    }
}

void CTrackGroupDlg::OnButtonGroupOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	m_pGroup->SetGroupName(ACString(m_sName));
    EndDialog(0);
}

void CTrackGroupDlg::OnButtonGroupCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
