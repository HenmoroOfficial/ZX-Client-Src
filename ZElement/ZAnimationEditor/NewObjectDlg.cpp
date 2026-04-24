// NewObjectDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "NewObjectDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNewObjectDlg dialog


CNewObjectDlg::CNewObjectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNewObjectDlg::IDD, pParent)
{
    m_sObjectFileName = _T("");
	m_sObjectType = _T("模型");
	//{{AFX_DATA_INIT(CNewObjectDlg)
	//}}AFX_DATA_INIT
}


void CNewObjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNewObjectDlg)
	DDX_Text(pDX, IDC_NEW_OBJECT_FILE_NAME, m_sObjectFileName);
	DDX_CBString(pDX, IDC_NEW_OBJECT_TYPE, m_sObjectType);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNewObjectDlg, CDialog)
	//{{AFX_MSG_MAP(CNewObjectDlg)
	ON_BN_CLICKED(IDC_BUTTON_NEW_OBJECT_IMPORT, OnButtonNewObjectImport)
	ON_BN_CLICKED(IDC_BUTTON_NEW_OBJECT_OK, OnButtonNewObjectOk)
	ON_BN_CLICKED(IDC_BUTTON_NEW_OBJECT_CANCEL, OnButtonNewObjectCancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNewObjectDlg message handlers

void CNewObjectDlg::OnButtonNewObjectImport() 
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
    else if (m_sObjectType == "GFX")
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

void CNewObjectDlg::OnButtonNewObjectOk() 
{
	// TODO: Add your control notification handler code here
    if (m_sObjectFileName != "")
    {
        CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
        unsigned int uiID;
        if (m_sObjectType == "模型")
        {
            uiID = pObjectManager->CreateObject(CAnmObject::TYPE_NPC);
        }
        else if (m_sObjectType == "GFX")
        {
            uiID = pObjectManager->CreateObject(CAnmObject::TYPE_GFX);
        }
        CAnmObject *pObject = pObjectManager->GetObject(uiID);
        A3DVECTOR3 vPos = g_Animator.GetA3DCamera()->GetPos() + g_Animator.GetA3DCamera()->GetDir();
        if (!pObject->Load(WC2AS(m_sObjectFileName)))
        {
            pObjectManager->RemoveObject(uiID);
        }
        else
        {
            pObjectManager->SelectObject(uiID);
            pObject->SetPos(vPos);
        }
    }
    theApp.OnChanged();
	EndDialog(0);
}

void CNewObjectDlg::OnButtonNewObjectCancel() 
{
	// TODO: Add your control notification handler code here
	EndDialog(0);
}
