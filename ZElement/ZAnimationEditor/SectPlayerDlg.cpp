// SectPlayerDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "SectPlayerDlg.h"
#include "CustomActionDlg.h"
#include "Animator.h"
#include "AnmCommonActionManager.h"
#include "AnmObjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSectPlayerDlg dialog

CSectPlayerDlg::CSectPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSectPlayerDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSectPlayerDlg)
	m_bShowWeapon = FALSE;
	m_bUsePlayer = FALSE;
	//}}AFX_DATA_INIT
}


void CSectPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSectPlayerDlg)
	DDX_Control(pDX, IDC_COMBO_PLAYER_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_COMBO_PLAYER_SECT, m_comboSect);
	DDX_Check(pDX, IDC_SHOW_HOST_SECTPLAYER_WEAPON, m_bShowWeapon);
	DDX_Check(pDX, IDC_USE_HOST_SECTPLAYER, m_bUsePlayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSectPlayerDlg, CDialog)
	//{{AFX_MSG_MAP(CSectPlayerDlg)
	ON_BN_CLICKED(IDC_BUTTON_SECTPLAYER_OK, OnButtonSectplayerOk)
	ON_BN_CLICKED(IDC_BUTTON_SECTPLAYER_CANCEL, OnButtonSectplayerCancel)
	ON_BN_CLICKED(IDC_BUTTON_SECTPLAYER_EDIT, OnButtonSectplayerEdit)
	ON_BN_CLICKED(IDC_USE_HOST_SECTPLAYER, OnUseHostSectplayer)
	ON_BN_CLICKED(IDC_SHOW_HOST_SECTPLAYER_WEAPON, OnShowHostSectplayerWeapon)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSectPlayerDlg message handlers

void CSectPlayerDlg::OnButtonSectplayerOk() 
{
	// TODO: Add your control notification handler code here
	g_Animator.SetUseHostPlayer(m_bUsePlayer ? true : false);
	g_Animator.SetShowHostPlayerWeapon(m_bShowWeapon ? true : false);
    
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	CString playerFileName;
	if (const ACString* pECM = pCommonActionMgr->GetSectPhysique(m_comboSect.GetCurSel(), m_comboGender.GetCurSel() == 0))
		playerFileName = CString(WC2AS(*pECM));
	if (!playerFileName.IsEmpty() && playerFileName != m_sPlayerFileNameBack)
    {
		int iSect = m_comboSect.GetCurSel();
		int iGender = m_comboGender.GetCurSel();
		
        g_Animator.Stop();
        m_sPlayerFileNameBack = playerFileName;
        CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
        CAnmObject *pPlayer = pObjectManager->GetObject(1);
        pPlayer->Load(WC2AS(m_sPlayerFileNameBack));
        CAnmObject *pCamera = pObjectManager->GetObject(0);
        pPlayer->SetPos(pCamera->GetPos() + pCamera->GetDir());

		pCommonActionMgr->SetHostPlayerSect(iSect);
		pCommonActionMgr->SetHostPlayerGender(iGender == 0);
		pCommonActionMgr->SaveToFile();
    }
    theApp.OnChanged();

	EndDialog(0);
}

void CSectPlayerDlg::OnButtonSectplayerCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CSectPlayerDlg::OnButtonSectplayerEdit() 
{
	// TODO: Add your control notification handler code here
	CCustomActionDlg dlg;
	dlg.DoModal();
}

BOOL CSectPlayerDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	int sectCount = pCommonActionMgr->GetSectCount();
	for (int i = 0; i < sectCount; ++ i)
	{
		if (const ACString* pName = pCommonActionMgr->GetSectName(i))
			m_comboSect.AddString(*pName);
		else
			m_comboSect.AddString(_T("ERROR"));
	}
	m_comboSect.SetCurSel(pCommonActionMgr->GetHostPlayerSect());
	m_comboGender.SetCurSel(pCommonActionMgr->IsHostPlayerMale() ? 0 : 1);
	m_bUsePlayer = g_Animator.IsUseHostPlayer();
	m_bShowWeapon = g_Animator.IsShowHostPlayerWeapon();

	UpdateData(false);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSectPlayerDlg::OnUseHostSectplayer() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CSectPlayerDlg::OnShowHostSectplayerWeapon() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
