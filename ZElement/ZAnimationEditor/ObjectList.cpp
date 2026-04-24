// ObjectList.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ObjectList.h"
#include "NewObjectDlg.h"
#include "WorldDlg.h"
#include "PlayerDlg.h"
#include "SectPlayerDlg.h"
#include "SelectedObjectDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmAxes.h"
#include "AnmCommonActionManager.h"
#include "AUTOCA/Utilities.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CObjectList

IMPLEMENT_DYNCREATE(CObjectList, CFormView)

CObjectList::CObjectList()
	: CFormView(CObjectList::IDD)
{
	//{{AFX_DATA_INIT(CObjectList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CObjectList::~CObjectList()
{
}

void CObjectList::UpdateList()
{
    std::vector<unsigned int> vList;

    CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
    pObjectManager->GetObjectIDList(vList);
    m_ObjectList.ResetContent();
    for (unsigned int i=0; i<vList.size(); ++i)
    {
        CAnmObject *pObject = pObjectManager->GetObject(vList[i]);
        CString text;
        text.Format(_T("OBJ:%u-%s"), vList[i], pObject->GetDisplayName());
        m_ObjectList.AddString(text);
        m_ObjectList.SetItemDataPtr(i, pObject);
    }
    UpdateData(0);

    UpdateSelectObject();
}

void CObjectList::UpdateSelectObject()
{
    std::vector<unsigned int> vList;
    
    CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
    pObjectManager->GetObjectIDList(vList);

    CAnmObject *pSelectedObject = pObjectManager->GetSelectedObject();
    if (pSelectedObject)
    {
        for (unsigned int i=0; i<vList.size(); ++i)
        {
            CAnmObject *pObject = pObjectManager->GetObject(vList[i]);
            if (pObject->GetObjectID() == pSelectedObject->GetObjectID())
            {
                m_ObjectList.SetCurSel(i);
                break;
            }
        }
    }
}

void CObjectList::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CObjectList)
    DDX_Control(pDX, IDC_OBJECT_LIST, m_ObjectList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CObjectList, CFormView)
	//{{AFX_MSG_MAP(CObjectList)
	ON_BN_CLICKED(IDC_BUTTON_WORLD, OnButtonWorld)
    ON_LBN_SELCHANGE(IDC_OBJECT_LIST, OnSelchangeObjectList)
	ON_BN_CLICKED(IDC_BUTTON_NEW_OBJECT, OnButtonNewObject)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_OBJECT, OnButtonRemoveObject)
	ON_BN_CLICKED(IDC_BUTTON_PLAYER, OnButtonPlayer)
	ON_LBN_DBLCLK(IDC_OBJECT_LIST, OnDblclkObjectList)
	ON_BN_CLICKED(IDC_BUTTON_COPY_OBJECT, OnButtonCopyObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CObjectList diagnostics

#ifdef _DEBUG
void CObjectList::AssertValid() const
{
	CFormView::AssertValid();
}

void CObjectList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CObjectList message handlers

void CObjectList::OnSelchangeObjectList() 
{
	// TODO: Add your control notification handler code here
    int index = m_ObjectList.GetCurSel();
    if (index != -1)
    {
        CAnmObject *pObject = static_cast<CAnmObject*>(m_ObjectList.GetItemDataPtr(index));
        if (pObject)
        {
            CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
            pObjectManager->SelectObject(pObject->GetObjectID());
            const A3DAABB objectAABB = pObject->GetAABB();
            CAnmAxes *pAxes = g_Animator.GetAxes();
            pAxes->SelectAxis(CAnmAxes::TYPE_NONE);
            pAxes->SetPos(pObject->GetPos()+A3DVECTOR3(0.0f, objectAABB.Maxs.y-objectAABB.Mins.y, 0.0f));
            pAxes->SetVisable(true);
        }
    }
}

void CObjectList::OnButtonWorld() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    CWorldDlg dlg;
    dlg.DoModal();
}

void CObjectList::OnButtonNewObject() 
{
    // TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    CNewObjectDlg dlg;
    dlg.DoModal();
    UpdateList();
    theApp.OnChanged();
}

void CObjectList::OnButtonCopyObject() 
{
    // TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_ObjectList.GetCurSel();
    CAnmObject *pObject = 0;
    if (index != -1)
    {
        pObject = static_cast<CAnmObject*>(m_ObjectList.GetItemDataPtr(index));
    }
    else
    {
        pObject = g_Animator.GetObjectManager()->GetSelectedObject();
    }
    if (pObject)
    {
        CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
        unsigned int uiCopyObjectID = pObjectManager->CopyObject(pObject->GetObjectID());
        if (uiCopyObjectID)
        {
            CAnmObject *pCopyObject = pObjectManager->GetObject(uiCopyObjectID);
            if (pCopyObject)
            {
                A3DVECTOR3 offset(frandom01() * 2.0f - 1.0f, 0.0f,
                    frandom01() * 2.0f - 1.0f);
                float dis = 1.0f + offset.Magnitude();
                offset.Normalize();
                offset *= dis;
                pCopyObject->SetPos(pCopyObject->GetPos() + offset);
            }
        }
    }
    UpdateList();
}

void CObjectList::OnButtonRemoveObject() 
{
    // TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_ObjectList.GetCurSel();
    if (index != -1)
    {
        int nBox = MessageBox(_T("确定是否要删除该对象?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
        if (IDYES == nBox)
        {
            CAnmObject *pObject = static_cast<CAnmObject*>(m_ObjectList.GetItemDataPtr(index));
            if (pObject)
            {
                CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
                pObjectManager->RemoveObject(pObject->GetObjectID());
            }
        }
    }
    UpdateList();
    theApp.OnChanged();
}

void CObjectList::OnButtonPlayer() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }

	CAnmCommonActionManager* pCommonActionMgr = g_Animator.GetCommonActionManager();
	if (pCommonActionMgr->IsEnable())
	{
		CSectPlayerDlg dlg;
		dlg.DoModal();
	}
	else
	{
		CPlayerDlg dlg;
		dlg.DoModal();
	}
}

void CObjectList::OnDblclkObjectList() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_ObjectList.GetCurSel();
    if (index != -1)
    {
        CAnmObject *pObject = static_cast<CAnmObject*>(m_ObjectList.GetItemDataPtr(index));
        if (pObject)
        {
            CAnmObjectManager *pObjectManager = g_Animator.GetObjectManager();
            pObjectManager->SelectObject(pObject->GetObjectID());
            CSelectedObjectDlg dlg(0, pObject);
            dlg.DoModal();
        }
    }
    UpdateList();
}
