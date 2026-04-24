// BezierList.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "BezierList.h"
#include "BezierDlg.h"
#include "ActionBezierPointDlg.h"
#include "Animator.h"
#include "AnmBezierManager.h"
#include "AnmAxes.h"

#include "SmartCamera.h"

#include <vector>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBezierList

IMPLEMENT_DYNCREATE(CBezierList, CFormView)


CBezierList::CBezierList()
	: CFormView(CBezierList::IDD)
{
	//{{AFX_DATA_INIT(CBezierList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CBezierList::~CBezierList()
{
}

void CBezierList::UpdateList()
{
    std::vector<unsigned int> vList;
    CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
    pBezierManager->GetBezierIDList(vList);
    m_BezierList.ResetContent();
    for (unsigned int i=0; i<vList.size(); ++i)
    {
        CAnmBezier *pBezier = pBezierManager->GetBezier(vList[i]);
        CString text;
        text.Format(_T("B[%c]:%u-%s"), (pBezier->IsVisable() ? 'V' : ' '),
                    vList[i], pBezier->GetDisplayName());
        m_BezierList.AddString(text);
        m_BezierList.SetItemDataPtr(i, pBezier);
    }
    UpdateSelectBezier();
}

void CBezierList::UpdateSelectBezier()
{
    std::vector<unsigned int> vList;
    CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
    pBezierManager->GetBezierIDList(vList);

    CAnmBezier *pSelectedBezier = pBezierManager->GetSelectedBezier();
    if (pSelectedBezier)
    {
        for (unsigned int i=0; i<vList.size(); ++i)
        {
            CAnmBezier *pBezier = pBezierManager->GetBezier(vList[i]);
            if (pBezier->GetID() == pSelectedBezier->GetID())
            {
                m_BezierList.SetCurSel(i);
                break;
            }
        }
    }
    UpdateNodeList();
}

void CBezierList::UpdateNodeList()
{
    std::vector<unsigned int> vList;
    m_BezierNodeList.ResetContent();
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            CAnmBezierPoint *pSelectedBezierPoint = pBezier->GetSelectedPoint();
            pBezier->GetPointIDList(vList);
            for (unsigned int i=0; i<vList.size(); ++i)
            {
                CAnmBezierPoint *pBezierPoint = pBezier->GetPoint(vList[i]);
                CString text;
                text.Format(_T("NodeID:%u"), vList[i]);
                m_BezierNodeList.AddString(text);
                m_BezierNodeList.SetItemDataPtr(i, pBezierPoint);
                if (pSelectedBezierPoint)
                {
                    if (pBezierPoint->GetID() == pSelectedBezierPoint->GetID())
                    {
                        m_BezierNodeList.SetCurSel(i);
                    }
                }
            }
        }
    }
    UpdateData(0);
}

void CBezierList::DoDataExchange(CDataExchange* pDX)
{
    CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CBezierList)
	DDX_Control(pDX, IDC_BEZIER_LIST, m_BezierList);
    DDX_Control(pDX, IDC_BEZIER_NODE_LIST, m_BezierNodeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CBezierList, CFormView)
	//{{AFX_MSG_MAP(CBezierList)
	ON_LBN_SELCHANGE(IDC_BEZIER_LIST, OnSelchangeBezierList)
	ON_BN_CLICKED(IDC_BUTTON_NEW_BEZIER, OnButtonNewBezier)
    ON_BN_CLICKED(IDC_BUTTON_COPY_BEZIER, OnButtonCopyBezier)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_BEZIER, OnButtonRemoveBezier)
	ON_LBN_DBLCLK(IDC_BEZIER_LIST, OnDblclkBezierList)
	ON_LBN_DBLCLK(IDC_BEZIER_NODE_LIST, OnDblclkBezierNodeList)
	ON_LBN_SELCHANGE(IDC_BEZIER_NODE_LIST, OnSelchangeBezierNodeList)
	ON_BN_CLICKED(IDC_BUTTON_DISPLAY_BEZIER, OnButtonDisplayBezier)
	ON_BN_CLICKED(IDC_BUTTON_HIDE_BEZIER, OnButtonHideBezier)
	ON_BN_CLICKED(IDC_BUTTON_COPY_BEZIER_POINT_INFO, OnButtonCopyBezierPointInfo)
	ON_BN_CLICKED(IDC_BUTTON_PASTE_BEZIER_POINT_INFO, OnButtonPasteBezierPointInfo)
	ON_BN_CLICKED(IDC_BUTTON_INSERT_BEZIER_POINT, OnButtonInsertBezierPoint)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE_BEZIER_POINT, OnButtonRemoveBezierPoint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CBezierList diagnostics

#ifdef _DEBUG
void CBezierList::AssertValid() const
{
	CFormView::AssertValid();
}

void CBezierList::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CBezierList message handlers

void CBezierList::OnSelchangeBezierList() 
{
    // TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
            pBezierManager->SelectBezier(pBezier->GetID());
            if (pBezier->GetPointAmount())
            {
                CAnmAxes *pAxes = g_Animator.GetAxes();
                pAxes->SelectAxis(CAnmAxes::TYPE_NONE);
                pAxes->SetPos(pBezier->GetFirstPoint()->GetPos());
                pAxes->SetVisable(true);
            }
            else
            {
                CAnmAxes *pAxes = g_Animator.GetAxes();
                pAxes->SetVisable(false);
            }
        }
    }
    UpdateNodeList();
}

void CBezierList::OnButtonNewBezier() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
    unsigned int uiID = pBezierManager->CreateBezier();
    pBezierManager->SelectBezier(uiID);
    UpdateList();
    theApp.OnChanged();
}

void CBezierList::OnButtonCopyBezier() 
{
    // TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            g_Animator.GetBezierManager()->CopyBezier(pBezier->GetID());
        }
    }
    UpdateList();
}

void CBezierList::OnButtonRemoveBezier() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        int nBox = MessageBox(_T("确定是否要删除该路径?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
        if (IDYES == nBox)
        {
            CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
            if (pBezier)
            {
                CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
                pBezierManager->RemoveBezier(pBezier->GetID());
            }
        }
    }
    UpdateList();
    theApp.OnChanged();
}

void CBezierList::OnDblclkBezierList() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
            pBezierManager->SelectBezier(pBezier->GetID());
            CBezierDlg dlg(0, pBezier);
            dlg.DoModal();
        }
    }
    UpdateList();
}

void CBezierList::OnDblclkBezierNodeList() 
{
	// TODO: Add your control notification handler code here
    if (g_Animator.GetState() == CAnimator::STATE_PLAYING)
    {
        return;
    }
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    pBezier->SelectPoint(pBezierPoint->GetID());
                    CActionBezierPointDlg dlg(0, pBezierPoint);
                    dlg.DoModal();
                    pBezier->ReCalculateLength();
                }
            }
        }
    }
}

void CBezierList::OnSelchangeBezierNodeList() 
{
	// TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    pBezier->SelectPoint(pBezierPoint->GetID());
                    CAnmAxes *pAxes = g_Animator.GetAxes();
                    pAxes->SelectAxis(CAnmAxes::TYPE_NONE);
                    pAxes->SetPos(pBezierPoint->GetPos());
                    pAxes->SetVisable(true);

                    SmartCamera *pSmartCamera = g_Animator.GetSmartCamera();
                    if (pSmartCamera)
                    {
                        pSmartCamera->SetInit();
                        pSmartCamera->Play();
                        pSmartCamera->MoveTo(pBezierPoint->GetPos(), 2000);
                        pSmartCamera->RotateTo(pBezierPoint->GetOrientation(), 2000);
                        pSmartCamera->FOVTo(pBezierPoint->GetFOV(), 2000);
                    }
                }
            }
        }
    }
}

void CBezierList::OnButtonDisplayBezier() 
{
	// TODO: Add your control notification handler code here
	CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
    pBezierManager->SetAllVisable(true);
    UpdateList();

}

void CBezierList::OnButtonHideBezier() 
{
	// TODO: Add your control notification handler code here
	CAnmBezierManager *pBezierManager = g_Animator.GetBezierManager();
    pBezierManager->SetAllVisable(false);
    UpdateList();
}

void CBezierList::OnButtonCopyBezierPointInfo() 
{
	// TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    m_vPointPos = pBezierPoint->GetPos();
                    m_qPointOrientation = pBezierPoint->GetOrientation();
                }
            }
        }
    }
}

void CBezierList::OnButtonPasteBezierPointInfo() 
{
	// TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    pBezier->SetPointPos(pBezierPoint, m_vPointPos);
                    pBezierPoint->SetOrientation(m_qPointOrientation);
                }
            }
        }
    }
}

void CBezierList::OnButtonInsertBezierPoint() 
{
	// TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    pBezier->InsertPoint(pBezierPoint->GetID());
                    UpdateNodeList();
                }
            }
        }
    }
}

void CBezierList::OnButtonRemoveBezierPoint() 
{
	// TODO: Add your control notification handler code here
    int index = m_BezierList.GetCurSel();
    if (index != -1)
    {
        CAnmBezier *pBezier = static_cast<CAnmBezier*>(m_BezierList.GetItemDataPtr(index));
        if (pBezier)
        {
            index = m_BezierNodeList.GetCurSel();
            if (index != -1)
            {
                CAnmBezierPoint *pBezierPoint = static_cast<CAnmBezierPoint*>(m_BezierNodeList.GetItemDataPtr(index));
                if (pBezierPoint)
                {
                    pBezier->RemovePoint(pBezierPoint);
                    UpdateNodeList();
                }
            }
        }
    }
}
