// ActionCameraFilterDlg.cpp : implementation file
//

#include "stdafx.h"
#include "AnimationEditor.h"
#include "ActionCameraFilterDlg.h"
#include "Animator.h"
#include "AnmObjectManager.h"
#include "AnmActionManager.h"
#include "AnmCameraFilter.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg dialog

CActionCameraFilterDlg::CActionCameraFilterDlg(CWnd* pParent, CTrackChannel *pChannel, CTrackNode *pNode)
	: CDialog(CActionCameraFilterDlg::IDD, pParent)
{
    m_pChannel = pChannel;
    m_pNode = pNode;
    m_uiObjectID = pNode->GetObjectID();
    m_uiActionID = pNode->GetActionID();
    m_fStartTime = float(pNode->GetStartTime()) / 1000.0f;
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmCameraFilter *pCameraFilter = dynamic_cast<CAnmCameraFilter*>(pAction);
    m_uiColor = (pCameraFilter->GetColor()) & 0x00FFFFFF;
    m_uiColor = ((m_uiColor & 0xFF) << 16) | (m_uiColor & 0xFF00) | ((m_uiColor & 0xFF0000) >> 16);
    unsigned int eFilterType = pCameraFilter->GetFilterType();
    if (eFilterType == CAnmCameraFilter::FILTER_TYPE_SOLID)
    {
        m_sFilterType = "纯色";
    }
    else if (eFilterType == CAnmCameraFilter::FILTER_TYPE_TWEENING_SOLID)
    {
        m_sFilterType = "渐变纯色";
    }
    else if (eFilterType == CAnmCameraFilter::FILTER_TYPE_TWEENING_TRANSPARENT)
    {
        m_sFilterType = "渐变透明";
    }
    else if (eFilterType == CAnmCameraFilter::FILTER_TYPE_SLOW_BLACK_WHITE)
    {
        m_sFilterType = "缓慢黑白";
    }
    else if (eFilterType == CAnmCameraFilter::FILTER_TYPE_FAST_BLACK_WHITE)
    {
        m_sFilterType = "快速黑白";
    }
	else if(eFilterType == CAnmCameraFilter::FILTER_TYPE_TWEENING_FRAME)
	{
		m_sFilterType = "镜头过度";
	}
    m_fTransparent = pCameraFilter->GetTransparent();
	m_iTotalFrames = pCameraFilter->GetTotalFrames();
    
	//{{AFX_DATA_INIT(CActionCameraFilterDlg)
	//}}AFX_DATA_INIT
}


void CActionCameraFilterDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CActionCameraFilterDlg)
    DDX_Text(pDX, IDC_OBJECT_ID, m_uiObjectID);
    DDX_Text(pDX, IDC_ACTION_ID, m_uiActionID);
    DDX_Text(pDX, IDC_START_TIME, m_fStartTime);
    DDX_Text(pDX, IDC_TOTAL_FRAMES, m_iTotalFrames);
	DDX_CBString(pDX, IDC_FILTER_TYPE, m_sFilterType);
	DDX_Text(pDX, IDC_TRANSPARENT, m_fTransparent);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CActionCameraFilterDlg, CDialog)
    //{{AFX_MSG_MAP(CActionCameraFilterDlg)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_REMOVE, OnButtonActionRemove)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_OK, OnButtonActionOk)
    ON_BN_CLICKED(IDC_BUTTON_ACTION_CANCEL, OnButtonActionCancel)
    ON_BN_CLICKED(IDC_BUTTON_CHOOSE_COLOR, OnButtonChooseColor)
	ON_WM_DRAWITEM()
	ON_CBN_SELCHANGE(IDC_FILTER_TYPE, OnSelchangeFilterType)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CActionScaleDlg message handlers

void CActionCameraFilterDlg::OnButtonActionRemove() 
{
    // TODO: Add your control notification handler code here
    int nBox = MessageBox(_T("确定是否要删除该事件?"), _T("动画编辑器"), MB_ICONQUESTION | MB_YESNOCANCEL);
    if (IDYES == nBox)
    {
        m_pChannel->DeleteAction(m_pNode->GetAction()->GetActionID());
        EndDialog(-1);
    }
}

void CActionCameraFilterDlg::OnButtonActionOk() 
{
    // TODO: Add your control notification handler code here
    UpdateData(true);
    CAnmAction *pAction = m_pNode->GetAction();
    CAnmCameraFilter *pCameraFilter = dynamic_cast<CAnmCameraFilter*>(pAction);

    pCameraFilter->SetColor(0xFF000000 | ((m_uiColor & 0xFF) << 16) | (m_uiColor & 0xFF00) | ((m_uiColor & 0xFF0000) >> 16));

    if (m_sFilterType == "纯色")
    {
        pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_SOLID);
    }
    else if (m_sFilterType == "渐变纯色")
    {
        pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_TWEENING_SOLID);
    }
    else if (m_sFilterType == "渐变透明")
    {
        pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_TWEENING_TRANSPARENT);
    }
    else if (m_sFilterType == "缓慢黑白")
    {
        pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_SLOW_BLACK_WHITE);
    }
    else if (m_sFilterType == "快速黑白")
    {
        pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_FAST_BLACK_WHITE);
    }
	else if(m_sFilterType == "镜头过度")
	{
		pCameraFilter->SetFilterType(CAnmCameraFilter::FILTER_TYPE_TWEENING_FRAME);
	}

    pCameraFilter->SetTransparent(m_fTransparent);
    pCameraFilter->SetTotalFrames(m_iTotalFrames);
    EndDialog(0);
}

void CActionCameraFilterDlg::OnButtonActionCancel() 
{
    // TODO: Add your control notification handler code here
    EndDialog(0);
}

void CActionCameraFilterDlg::OnButtonChooseColor() 
{
	// TODO: Add your control notification handler code here
    CColorDialog dlg;
    if (dlg.DoModal())
    {
        m_uiColor = dlg.GetColor();
        m_uiColor &= 0x00FFFFFF;
        InvalidateRect(NULL);
    }
}

void CActionCameraFilterDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your message handler code here and/or call default
	
    if (nIDCtl == IDC_BUTTON_CHOOSE_COLOR)
    {
        CRect rc;
        ::GetClientRect(lpDrawItemStruct->hwndItem, rc);
        CBrush brush(m_uiColor);
        HGDIOBJ hOldBrush = SelectObject(lpDrawItemStruct->hDC, brush);
        Rectangle(lpDrawItemStruct->hDC, rc.left, rc.top, rc.right, rc.bottom);
        SelectObject( lpDrawItemStruct->hDC, hOldBrush);
    }
    else
    {
	    CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
    }
}

BOOL CActionCameraFilterDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	GetDlgItem(IDC_BUTTON_CHOOSE_COLOR)->EnableWindow(!(m_sFilterType == "镜头过度"));
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CActionCameraFilterDlg::OnSelchangeFilterType() 
{
	// TODO: Add your control notification handler code here
	CComboBox* pCombox = (CComboBox*)GetDlgItem(IDC_FILTER_TYPE);
	int curSel = pCombox->GetCurSel();
	CString curType;
	pCombox->GetLBText(curSel,curType);
	GetDlgItem(IDC_BUTTON_CHOOSE_COLOR)->EnableWindow(!(curType == "镜头过度"));

	
}
