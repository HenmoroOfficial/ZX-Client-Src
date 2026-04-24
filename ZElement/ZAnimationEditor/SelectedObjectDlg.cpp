// SelectedObject.cpp : implementation file
//

#include "stdafx.h"
#include "Animationeditor.h"
#include "SelectedObjectDlg.h"
#include "Animator.h"
#include "AnmObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSelectedObjectDlg dialog


CSelectedObjectDlg::CSelectedObjectDlg(CWnd* pParent, CAnmObject *pObject)
	: CDialog(CSelectedObjectDlg::IDD, pParent)
{
    m_pObject = pObject;
    m_uiObjectID = m_pObject->GetObjectID();
    if (m_pObject->GetObjectType() == CAnmObject::TYPE_NPC)
    {
        m_sObjectType = "模型";
    }
    else if (m_pObject->GetObjectType() == CAnmObject::TYPE_GFX)
    {
        m_sObjectType = "GFX";
    }
    else
    {
        m_sObjectType = _T("");
    }
    A3DVECTOR3 offset = m_pObject->GetOffset();
    m_fOffsetX = offset.x;
    m_fOffsetY = offset.y;
	m_fOffsetZ = offset.z;
    m_sObjectDisplayName = m_pObject->GetDisplayName();
	m_sObjectFileName = m_pObject->GetLoadFile();
	//{{AFX_DATA_INIT(CSelectedObjectDlg)
	//}}AFX_DATA_INIT
}


void CSelectedObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelectedObjectDlg)
	DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
	DDX_Text(pDX, IDC_OBJECT_TYPE, m_sObjectType);
	DDX_Text(pDX, IDC_OBJECT_NAME, m_sObjectDisplayName);
	DDX_Text(pDX, IDC_OBJECT_FILE_NAME, m_sObjectFileName);
	DDX_Text(pDX, IDC_OFFSET_X, m_fOffsetX);
	DDX_Text(pDX, IDC_OFFSET_Y, m_fOffsetY);
	DDX_Text(pDX, IDC_OFFSET_Z, m_fOffsetZ);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSelectedObjectDlg, CDialog)
	//{{AFX_MSG_MAP(CSelectedObjectDlg)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_IMPORT, OnButtonObjectImport)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_OK, OnButtonObjectOk)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_CANCEL, OnButtonObjectCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelectedObjectDlg message handlers

void CSelectedObjectDlg::OnButtonObjectImport() 
{
	// TODO: Add your control notification handler code here
    UpdateData(true);
    if (m_sObjectType == "模型")
    {
        CFileDialog dlg(TRUE, NULL, NULL,
            OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
            _T("EC model files(*.ecm)|*.ecm|All files(*.*)|*.*||"), NULL);
        CString strDirPath;
        strDirPath.Format(_T("%s"), _T("models"));
        dlg.m_ofn.lpstrInitialDir = strDirPath;
        if (dlg.DoModal()!=IDOK)
            return;
        m_sObjectFileName = g_aGetRelativePath(dlg.GetPathName());
    }
    else if (m_sObjectType == _T("GFX"))
    {
        CFileDialog dlg(TRUE, NULL, NULL,
            OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,
            _T("GFX files(*.gfx)|*.gfx|All files(*.*)|*.*||"), NULL);
        CString strDirPath;
        strDirPath.Format(_T("%s"), _T("gfx"));
        dlg.m_ofn.lpstrInitialDir = strDirPath;
        if (dlg.DoModal()!=IDOK)
            return;
        m_sObjectFileName = g_aGetRelativePath(dlg.GetPathName());
    }
    UpdateData(0);
}

void CSelectedObjectDlg::OnButtonObjectOk() 
{
	// TODO: Add your control notification handler code here
	UpdateData(true);
    m_pObject->SetOffset(A3DVECTOR3(m_fOffsetX, m_fOffsetY, m_fOffsetZ));
    m_pObject->SetDisplayName(ACString(m_sObjectDisplayName));
	if (m_sObjectFileName != m_pObject->GetName())
    {
        g_Animator.Stop();
        A3DVECTOR3 vPos = m_pObject->GetPos();
        m_pObject->Load(WC2AS(m_sObjectFileName));
        m_pObject->SetPos(vPos);
    }
    theApp.OnChanged();
    EndDialog(0);
}

void CSelectedObjectDlg::OnButtonObjectCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(-1);
}
