// PlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "Animator.h"
#include "PlayerDlg.h"
#include "AnmObjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg dialog


CPlayerDlg::CPlayerDlg(CWnd* pParent )
	: CDialog(CPlayerDlg::IDD, pParent)
{
    m_bUseHostPlayer = g_Animator.IsUseHostPlayer();
    m_bShowHostPlayerWeapon = g_Animator.IsShowHostPlayerWeapon();
    CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
    CAnmObject *pPlayer = pObjectManager->GetObject(1);
    m_sPlayerFileName = pPlayer->GetName();
    m_sPlayerFileNameBack = m_sPlayerFileName;
	//{{AFX_DATA_INIT(CPlayerDlg)
	//}}AFX_DATA_INIT
}


void CPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPlayerDlg)
	DDX_Text(pDX, IDC_PLAYER_FILE_NAME, m_sPlayerFileName);
	DDX_Check(pDX, IDC_USE_HOST_PLAYER, m_bUseHostPlayer);
	DDX_Check(pDX, IDC_SHOW_HOST_PLAYER_WEAPON, m_bShowHostPlayerWeapon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CPlayerDlg)
    ON_BN_CLICKED(IDC_BUTTON_PLAYER_IMPORT, OnButtonPlayerImport)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER_OK, OnButtonPlayerOk)
    ON_BN_CLICKED(IDC_BUTTON_PLAYER_CANCEL, OnButtonPlayerCancel)
	ON_BN_CLICKED(IDC_USE_HOST_PLAYER, OnUseHostPlayer)
	ON_BN_CLICKED(IDC_SHOW_HOST_PLAYER_WEAPON, OnShowHostPlayerWeapon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlayerDlg message handlers

void CPlayerDlg::OnButtonPlayerImport()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CFileDialog dlg(TRUE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("Player Model file(*.ecm)|*.ecm|All files(*.*)|*.*||"), NULL);
    CString strDirPath;
    strDirPath.Format(_T("%s"), _T("models\\Players"));
    dlg.m_ofn.lpstrInitialDir = strDirPath;
    if (dlg.DoModal() != IDOK)
        return;
    m_sPlayerFileName = g_aGetRelativePath(dlg.GetPathName());
    UpdateData(0);
}

void CPlayerDlg::OnButtonPlayerOk()
{
    // TODO: Add your control notification handler code here
    g_Animator.SetUseHostPlayer(m_bUseHostPlayer ? true : false);
    g_Animator.SetShowHostPlayerWeapon(m_bShowHostPlayerWeapon ? true : false);

    if ((m_sPlayerFileName != _T("")) && (m_sPlayerFileName != m_sPlayerFileNameBack))
    {
        g_Animator.Stop();
        m_sPlayerFileNameBack = m_sPlayerFileName;
        CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
        CAnmObject *pPlayer = pObjectManager->GetObject(1);
        pPlayer->Load(WC2AS(m_sPlayerFileNameBack));
        CAnmObject *pCamera = pObjectManager->GetObject(0);
        pPlayer->SetPos(pCamera->GetPos() + pCamera->GetDir());
    }
    theApp.OnChanged();
    EndDialog(0);
}

void CPlayerDlg::OnButtonPlayerCancel()
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CPlayerDlg::OnUseHostPlayer()
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CPlayerDlg::OnShowHostPlayerWeapon() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
