// ActionSoundDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionSoundDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmSound.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionSoundDlg dialog


CActionSoundDlg::CActionSoundDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionSoundDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
    m_sSoundFileName = pSound->GetSoundFileName();
	m_iTotalFrames = pSound->GetTotalFrames();
    m_iStartSeconds = pSound->GetStartSeconds();
    m_dwVolume = pSound->GetVolume();
	//{{AFX_DATA_INIT(CActionSoundDlg)
	//}}AFX_DATA_INIT
}


void CActionSoundDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CActionSoundDlg)
	DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
	DDX_Text(pDX, IDC_SOUND_FILE_NAME, m_sSoundFileName);
	DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
	DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_Text(pDX, IDC_VOLUME, m_dwVolume);
	DDX_Text(pDX, IDC_START_SECONDS, m_iStartSeconds);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionSoundDlg, CDialog)
	//{{AFX_MSG_MAP(CActionSoundDlg)
	ON_BN_CLICKED(IDC_BUTTON_SOUND_IMPORT, OnButtonSoundImport)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
	ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionSoundDlg message handlers

void CActionSoundDlg::OnButtonSoundImport() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CFileDialog dlg(TRUE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("OGG file(*.ogg)|*.ogg|MP3 file(*.mp3)|*.mp3|WAV file(*.wav)|*.wav|All files(*.*)|*.*||"), NULL);
    CString strDirPath;
    strDirPath.Format(_T("%s"), _T("musics"));
    dlg.m_ofn.lpstrInitialDir = strDirPath;
    if (dlg.DoModal() != IDOK)
        return;
    m_sSoundFileName = g_aGetRelativePath(dlg.GetPathName());
    UpdateData(0);
}

void CActionSoundDlg::OnButtonActionRemove() 
{
	// TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionSoundDlg::OnButtonActionOk() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmSound *pSound = dynamic_cast<CAnmSound*>(pAction);
    pSound->SetTotalFrames(m_iTotalFrames);
    pSound->SetStartSeconds(m_iStartSeconds);
    pSound->SetSoundFileName(ACString(m_sSoundFileName));
    pSound->SetVolume(m_dwVolume);
    EndDialog(0);
}

void CActionSoundDlg::OnButtonActionCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
