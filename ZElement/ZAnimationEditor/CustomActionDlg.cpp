// CustomActionDlg.cpp : implementation file
//

#include "stdafx.h"
#include "animationeditor.h"
#include "Animator.h"
#include "CustomActionDlg.h"
#include "AnmCommonActionManager.h"
#include "AnmNPC.h"
#include <algorithm>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCustomActionDlg dialog


CCustomActionDlg::CCustomActionDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CCustomActionDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CCustomActionDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CCustomActionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CCustomActionDlg)
	DDX_Control(pDX, IDC_BUTTON_CUSTOM_ACTION_BIND, m_btnBind);
	DDX_Control(pDX, IDC_BUTTON_CUSTOM_ACTION_ADD, m_btnAdd);
	DDX_Control(pDX, IDC_COMBO_CUSTOM_ACTION_GENDER, m_comboGender);
	DDX_Control(pDX, IDC_COMBO_CUSTOM_ACTION_SECT, m_comboSect);
	DDX_Control(pDX, IDC_COMBO_CUSTOM_ACTION_BIND, m_comboBindAction);
	DDX_Control(pDX, IDC_COMBO_CUSTOM_ACTION, m_comboAction);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CCustomActionDlg, CDialog)
	//{{AFX_MSG_MAP(CCustomActionDlg)
	ON_CBN_EDITCHANGE(IDC_COMBO_CUSTOM_ACTION, OnEditchangeComboCustomAction)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_ACTION, OnSelchangeComboCustomAction)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_ACTION_BIND, OnSelchangeComboCustomActionBind)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_ACTION_SECT, OnSelchangeComboCustomActionSect)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_ACTION_ADD, OnButtonCustomActionAdd)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_ACTION_BIND, OnButtonCustomActionBind)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_ACTION_CANCEL, OnButtonCustomActionCancel)
	ON_BN_CLICKED(IDC_BUTTON_CUSTOM_ACTION_OK, OnButtonCustomActionOk)
	ON_CBN_SELCHANGE(IDC_COMBO_CUSTOM_ACTION_GENDER, OnSelchangeComboCustomActionGender)
	ON_CBN_EDITCHANGE(IDC_COMBO_CUSTOM_ACTION_BIND, OnEditchangeComboCustomActionBind)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCustomActionDlg message handlers

void CCustomActionDlg::OnEditchangeComboCustomAction() 
{
	// TODO: Add your control notification handler code here
	bool bNewName = false;
	CString editText;
	m_comboAction.GetWindowText(editText);
	if (!editText.IsEmpty())
	{
		bNewName = CB_ERR == m_comboAction.FindStringExact(0, editText);
	}

	m_btnAdd.EnableWindow(bNewName);
}

void CCustomActionDlg::OnSelchangeComboCustomAction() 
{
	// TODO: Add your control notification handler code here
	if (m_iCommonName == m_comboAction.GetCurSel())
		return;
	m_iCommonName = m_comboAction.GetCurSel();
	UpdateComboBindActionSel();
}

void CCustomActionDlg::OnSelchangeComboCustomActionBind() 
{
	// TODO: Add your control notification handler code here
	if (m_iRealName == m_comboBindAction.GetCurSel())
		return;
	if (m_iRealName == CB_ERR || m_iCommonName == CB_ERR)
	{
		m_btnBind.EnableWindow(FALSE);
		return;
	}
	m_iRealName = m_comboBindAction.GetCurSel();
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	CString commonName, realName;
	m_comboAction.GetWindowText(commonName);
	m_comboBindAction.GetLBText(m_iRealName, realName);
	if ( const ACString* pRealName = pCommonActionMgr->GetRealActionName(m_iSect, m_iGender == 0, commonName.GetBuffer(commonName.GetLength())) )
		m_btnBind.EnableWindow(realName != *pRealName);
	else
		m_btnBind.EnableWindow(TRUE);
}

void CCustomActionDlg::OnSelchangeComboCustomActionSect() 
{
	// TODO: Add your control notification handler code here
	if (m_iSect == m_comboSect.GetCurSel())
		return;
	m_iSect = m_comboSect.GetCurSel();
	UpdateComboBindAction();
	UpdateComboBindActionSel();
}

void CCustomActionDlg::OnButtonCustomActionAdd() 
{
	// TODO: Add your control notification handler code here
	CString editText;
	m_comboAction.GetWindowText(editText);
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	pCommonActionMgr->AddCommonAction(editText.GetBuffer(editText.GetLength()));
	m_comboAction.SetCurSel(m_comboAction.AddString(editText));
	OnSelchangeComboCustomAction();
	m_btnAdd.EnableWindow(FALSE);
}

void CCustomActionDlg::OnButtonCustomActionBind() 
{
	// TODO: Add your control notification handler code here
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	CString commonName, realName;
	m_comboAction.GetWindowText(commonName);
	m_comboBindAction.GetWindowText(realName);
	pCommonActionMgr->MapCommonActionToRealAction(m_iSect, m_iGender==0,
		commonName.GetBuffer(commonName.GetLength()), realName.GetBuffer(realName.GetLength()));
	m_btnBind.EnableWindow(FALSE);
}

void CCustomActionDlg::OnButtonCustomActionCancel() 
{
	// TODO: Add your control notification handler code here
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	pCommonActionMgr->Reset();

	EndDialog(0);
}

void CCustomActionDlg::OnButtonCustomActionOk() 
{
	// TODO: Add your control notification handler code here
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	pCommonActionMgr->SaveToFile();
	
	EndDialog(0);
}

BOOL CCustomActionDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	int nActionCount = pCommonActionMgr->GetCommonActionCount();
	int i;
	for (i = 0; i < nActionCount; ++ i)
	{
		if (const ACString* pName = pCommonActionMgr->GetCommonActionName(i))
			m_comboAction.AddString(*pName);
		else
			m_comboAction.AddString(_T("ERROR"));
	}

	int sectCount = pCommonActionMgr->GetSectCount();
	for (i = 0; i < sectCount; ++ i)
	{
		if (const ACString* pName = pCommonActionMgr->GetSectName(i))
			m_comboSect.AddString(*pName);
		else
			m_comboSect.AddString(_T("ERROR"));
	}

	m_iCommonName = m_comboAction.SetCurSel(0);
	m_iGender = m_comboSect.SetCurSel(0);
	m_iSect = m_comboGender.SetCurSel(0);
	m_iRealName = 0;
	m_btnAdd.EnableWindow(FALSE);
	m_btnBind.EnableWindow(FALSE);

	UpdateComboBindAction();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CCustomActionDlg::UpdateComboBindAction()
{
	CString originText;
	m_comboBindAction.GetWindowText(originText);
	m_comboBindAction.ResetContent();
	m_comboBindAction.Clear();

	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	const ACString* pECM = pCommonActionMgr->GetSectPhysique(m_iSect, m_iGender == 0);

	if (!pECM)
	{
		m_iRealName = CB_ERR;
		return;
	}
	
	const ACString* pCommonActionName = pCommonActionMgr->GetCommonActionName(m_iCommonName);
	ACString commonAction = pCommonActionName ? *pCommonActionName : _AL("");
	const ACString* pRealAction = pCommonActionMgr->GetRealActionName(m_iSect, m_iGender==0, commonAction);
	CString actionName = pRealAction ? CString(*pRealAction) : originText;

	std::vector<ACString> actionList;
	CAnmNPC anmNpc(-1);
	anmNpc.Load(WC2AS(*pECM));
	anmNpc.GetActionList(actionList);

	std::sort(actionList.begin(), actionList.end());

	int index = 0;
	int actionCount = (int)actionList.size();
	for (int i = 0; i < actionCount; ++ i)
	{
		const ACString& name = actionList[i];
		m_comboBindAction.AddString(name);
		if (actionName.Compare(name) == 0)
			index = i;
	}
	m_iRealName = m_comboBindAction.SetCurSel(index == CB_ERR ? 0 : index);
}

void CCustomActionDlg::UpdateComboBindActionSel()
{
	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	CString commonName;
	m_comboAction.GetLBText(m_iCommonName, commonName);
	if ( const ACString* pRealName = pCommonActionMgr->GetRealActionName(m_iSect, m_iGender == 0, commonName.GetBuffer(commonName.GetLength())) )
	{
		int index = m_comboBindAction.FindStringExact(0, *pRealName);
		if (index != CB_ERR)
		{
			m_comboBindAction.SetCurSel(index);
			m_btnBind.EnableWindow(FALSE);
		}
	}
	else
	{
		m_btnBind.EnableWindow(TRUE);
	}
}

void CCustomActionDlg::OnSelchangeComboCustomActionGender() 
{
	// TODO: Add your control notification handler code here
	if (m_iGender == m_comboGender.GetCurSel())
		return;
	m_iGender = m_comboGender.GetCurSel();
	UpdateComboBindAction();
	UpdateComboBindActionSel();
}

void CCustomActionDlg::OnEditchangeComboCustomActionBind() 
{
	// TODO: Add your control notification handler code here
	CString editText;
	m_comboBindAction.GetWindowText(editText);
	int index = m_comboBindAction.FindStringExact(0, editText);
	if (index == CB_ERR)
		m_btnBind.EnableWindow(FALSE);
	else
	{
		m_comboBindAction.SetCurSel(index);
		OnSelchangeComboCustomActionBind();
	}
}
