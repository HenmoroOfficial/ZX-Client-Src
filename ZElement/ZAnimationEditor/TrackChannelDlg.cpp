// TrackChannelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "TrackChannelDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTrackChannelDlg dialog


CTrackChannelDlg::CTrackChannelDlg(CWnd* pParent, CTrackGroup *pGroup, CTrackChannel *pChannel)
	: CDialog(CTrackChannelDlg::IDD, pParent)
{
    m_pGroup = pGroup;
    m_pChannel = pChannel;
    m_uiID = pChannel->GetChannelID();
    m_sName = m_pChannel->GetChannelName();
	m_uiObjectID = m_pChannel->GetObjectID();
	//{{AFX_DATA_INIT(CTrackChannelDlg)
	//}}AFX_DATA_INIT
}


void CTrackChannelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTrackChannelDlg)
	DDX_Text(pDX, IDC_CHANNEL_NAME, m_sName);
	DDX_Text(pDX, IDC_CHANNEL_OBJECT, m_uiObjectID);
	DDX_Text(pDX, IDC_CHANNEL_ID, m_uiID);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTrackChannelDlg, CDialog)
	//{{AFX_MSG_MAP(CTrackChannelDlg)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_OK, OnButtonChannelOk)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_CANCEL, OnButtonChannelCancel)
	ON_BN_CLICKED(IDC_BUTTON_CHANNEL_REMOVE, OnButtonChannelRemove)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTrackChannelDlg message handlers

void CTrackChannelDlg::OnButtonChannelOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(TRUE);
	m_pChannel->SetChannelName(ACString(m_sName));
    m_pChannel->SetObjectID(m_uiObjectID);
    EndDialog(0);
}

void CTrackChannelDlg::OnButtonChannelCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CTrackChannelDlg::OnButtonChannelRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该通道?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
	{
        m_pGroup->DeleteChannel(m_pChannel->GetChannelID());
        EndDialog(-1);
    }
}
