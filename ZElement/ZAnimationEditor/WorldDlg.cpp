// WorldDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "Animator.h"
#include "WorldDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWorldDlg dialog


CWorldDlg::CWorldDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWorldDlg::IDD, pParent)
{
    m_iWorldInstance = g_Animator.GetWorldInstance();
    m_bSpecifyTime = g_Animator.IsSpecifyTime();
    m_iHour = g_Animator.GetHour();
    m_iMinute = g_Animator.GetMinute();
    m_iSecond = g_Animator.GetSecond();
    m_bSpecifyWeather = g_Animator.IsSpecifyWeahter();
    m_bRain = g_Animator.IsRain();
    m_bSnow = g_Animator.IsSnow();
	//{{AFX_DATA_INIT(CWorldDlg)
	//}}AFX_DATA_INIT
}


void CWorldDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWorldDlg)
	DDX_Text(pDX, IDC_WORLD_INSTANCE, m_iWorldInstance);
    DDX_Text(pDX, IDC_WORLD_TIME_HOUR, m_iHour);
    DDX_Text(pDX, IDC_WORLD_TIME_MINUTE, m_iMinute);
    DDX_Text(pDX, IDC_WORLD_TIME_SECOND, m_iSecond);
    DDX_Check(pDX, IDC_SPECIFY_TIME, m_bSpecifyTime);
    DDX_Check(pDX, IDC_SPECIFY_WEATHER, m_bSpecifyWeather);
    DDX_Check(pDX, IDC_RAIN, m_bRain);
    DDX_Check(pDX, IDC_SNOW, m_bSnow);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CWorldDlg, CDialog)
	//{{AFX_MSG_MAP(CWorldDlg)
	ON_BN_CLICKED(IDC_BUTTON_WORLD_IMPORT, OnButtonWorldImport)
	ON_BN_CLICKED(IDC_BUTTON_WORLD_OK, OnButtonWorldOk)
	ON_BN_CLICKED(IDC_BUTTON_WORLD_CANCEL, OnButtonWorldCancel)
	ON_BN_CLICKED(IDC_SPECIFY_TIME, OnSpecifyTime)
	ON_BN_CLICKED(IDC_SPECIFY_WEATHER, OnSpecifyWeather)
	ON_BN_CLICKED(IDC_RAIN, OnRain)
	ON_BN_CLICKED(IDC_SNOW, OnSnow)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWorldDlg message handlers

void CWorldDlg::OnButtonWorldImport()
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    CFileDialog dlg(TRUE, NULL, NULL,
        OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
        _T("EC world files(*.ecwld)|*.ecwld|All files(*.*)|*.*||"), NULL);
    CString strDirPath;
    strDirPath.Format(_T("%s"), _T("maps"));
    dlg.m_ofn.lpstrInitialDir = strDirPath;
    if (dlg.DoModal() != IDOK)
        return;
    UpdateData(0);
}

void CWorldDlg::OnButtonWorldOk()
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    g_Animator.SetSpecifyTime(m_bSpecifyTime ? true : false);
    g_Animator.SetWorldTime(m_iHour, m_iMinute, m_iSecond);
    g_Animator.SetSpecifyWeather(m_bSpecifyWeather ? true : false);
    g_Animator.SetWeatherRain(m_bRain ? true : false);
    g_Animator.SetWeatherSnow(m_bSnow ? true : false);
    if (m_iWorldInstance != g_Animator.GetWorldInstance())
    {
        CWaitCursor cursor;
        if(!g_Animator.LoadWorld(m_iWorldInstance, A3DVECTOR3(0.0f)))
			AfxMessageBox(L"º”‘ÿ ß∞‹!");
    }
    EndDialog(0);
}

void CWorldDlg::OnButtonWorldCancel()
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}

void CWorldDlg::OnSpecifyTime() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CWorldDlg::OnSpecifyWeather() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CWorldDlg::OnRain() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}

void CWorldDlg::OnSnow() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
}
