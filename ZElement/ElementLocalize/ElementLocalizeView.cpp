// ElementLocalizeView.cpp : implementation of the CElementLocalizeView class
//

#include "stdafx.h"
#include "ElementLocalize.h"

#include "ElementLocalizeDoc.h"
#include "ElementLocalizeView.h"
#include "MainFrm.h"
#include "ElementDataView.h"

#include "TextBase.h"
#include "Jb.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern bool g_bModified;
extern int           g_nTotalTextNum;
extern int           g_nNewTextNum;
extern int           g_nDisuseTextNum;
extern int           g_nTransTextNum;

#define EDIT_LIMIT 100000

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeView

IMPLEMENT_DYNCREATE(CElementLocalizeView, CFormView)

BEGIN_MESSAGE_MAP(CElementLocalizeView, CFormView)
	//{{AFX_MSG_MAP(CElementLocalizeView)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE, OnButtonReplace)
	ON_EN_CHANGE(IDC_EDIT_TRANS, OnChangeEditTrans)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_AUTO_TRANS, OnButtonAutoTrans)
	ON_BN_CLICKED(IDC_BUTTON_REPLACE_NEXT, OnButtonReplaceNext)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()


int Aux_GetLineNum(CString str)
{
	CString temp = str;
	if(temp.IsEmpty()) return 0;
	return temp.Replace(_T("\r\n"),NULL) + 1;
}
/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeView construction/destruction

CElementLocalizeView::CElementLocalizeView()
	: CFormView(CElementLocalizeView::IDD)
{
	//{{AFX_DATA_INIT(CElementLocalizeView)
	m_strOrg = _T("");
	m_strTrans = _T("");
	//}}AFX_DATA_INIT
	// TODO: add construction code here
	m_pText = NULL;

}

CElementLocalizeView::~CElementLocalizeView()
{
}

void CElementLocalizeView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElementLocalizeView)
	DDX_Control(pDX, IDC_STATIC_FLAG, m_Flag);
	DDX_Text(pDX, IDC_EDIT_ORG, m_strOrg);
	DDX_Text(pDX, IDC_EDIT_TRANS, m_strTrans);
	//}}AFX_DATA_MAP
}

BOOL CElementLocalizeView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CFormView::PreCreateWindow(cs);
}

void CElementLocalizeView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	GetParentFrame()->RecalcLayout();
	ResizeParentToFit();

	m_hDel = AfxGetApp()->LoadIcon(IDI_ICON_DEL);
	m_hNew = AfxGetApp()->LoadIcon(IDI_ICON_NEW);
	m_hNormal = AfxGetApp()->LoadIcon(IDI_ICON_NORMAL);
	m_Flag.SetIcon(m_hNormal);

	((CEdit*)GetDlgItem(IDC_EDIT_TRANS))->SetLimitText(EDIT_LIMIT);

}

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeView printing

BOOL CElementLocalizeView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CElementLocalizeView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CElementLocalizeView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CElementLocalizeView::OnPrint(CDC* pDC, CPrintInfo* /*pInfo*/)
{
	// TODO: add customized printing code here
}

void CElementLocalizeView::ChangeTextBase(CTextBase *pTextBase,HTREEITEM hSelItem)
{
	//先提示用户保存前面的
	if(m_pText)
	{
		UpdateData(true);
		TCHAR *pTrans = m_pText->GetTranslationText();
		if(!m_strTrans.IsEmpty())
		{
			CString strMsg;
			CElementDataView *pLeft = (CElementDataView *) ((CMainFrame *)AfxGetMainWnd())->GetLeftView();
			strMsg.Format(_T("You've just made changes to [%s]!\nDo you want to accept these changes?"),pLeft->GetPath(m_hSelItem));
			if(pTrans==NULL)
			{
				if(IDYES==AfxMessageBox(strMsg,MB_YESNO|MB_ICONQUESTION))
				{
					//m_pText->ReplaceTransText(m_strTrans.GetBuffer(0));
					Replace();
					
				}
			}else if(_tcscmp(pTrans,m_strTrans)!=0)
			{
				if(IDYES==AfxMessageBox(strMsg,MB_YESNO|MB_ICONQUESTION))
				{
					//m_pText->ReplaceTransText(m_strTrans.GetBuffer(0));
					Replace();
				}
			}
		}
	}

	//显示新的
	m_pText = pTextBase;
	m_hSelItem  = hSelItem;
	CString txt;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_LENGTH);
	txt.Format(_T("Length: %d"),_tcslen(m_pText->GetOriginalText()));
	pWnd->SetWindowText(txt);
	
	pWnd = GetDlgItem(IDC_STATIC_TEXT_LIMIT);
	if(m_pText->GetCharUpperLimit()==-1)txt.Format(_T("Upper Limit: No"));
	else txt.Format(_T("Upper Limit: %d"),m_pText->GetCharUpperLimit());
	pWnd->SetWindowText(txt);
	
	pWnd = GetDlgItem(IDC_STATIC_IS_MULTI_LINE);
	if(m_pText->IsMuiltiLine())
		txt.Format(_T("MultiLine: Yes"));
	else txt.Format(_T("MultiLine: No"));
	pWnd->SetWindowText(txt);

	pWnd = GetDlgItem(IDC_STATIC_LINE_NUM);
	txt.Format(_T("Line Num: %d"),m_pText->GetOriginalTextLineNum());
	pWnd->SetWindowText(txt);
	
	pWnd = GetDlgItem(IDC_EDIT_ID);
	txt.Format(_T("Unique ID: %s"),m_pText->GetStrID());
	pWnd->SetWindowText(txt);

	if(m_pText->GetStatus()==CTextBase::status_disuse)
		m_Flag.SetIcon(m_hDel);
	else if(m_pText->GetStatus()==CTextBase::status_new)
		m_Flag.SetIcon(m_hNew);
	else if(m_pText->GetStatus()==CTextBase::status_normal)
		m_Flag.SetIcon(m_hNormal);

	m_strOrg = m_pText->GetOriginalText();
	m_strTrans = m_pText->GetTranslationText();
	UpdateData(false);
	UpdateTransInfo();
}

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeView diagnostics

#ifdef _DEBUG
void CElementLocalizeView::AssertValid() const
{
	CFormView::AssertValid();
}

void CElementLocalizeView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CElementLocalizeDoc* CElementLocalizeView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CElementLocalizeDoc)));
	return (CElementLocalizeDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElementLocalizeView message handlers

void CElementLocalizeView::OnButtonReplace() 
{
	ReplaceText();
}

bool CElementLocalizeView::ReplaceText()
{
	if(m_pText)
	{
		TCHAR *pTrans = NULL;
		int nOldStatus = m_pText->GetStatus();
		bool bReplace = false;
		UpdateData(true);
		if(m_pText->GetOriginalText()==NULL || _tcscmp(m_pText->GetOriginalText(),_T(""))==0)
		{
			m_pText->SetStatus(CTextBase::status_normal);
			goto Rep;
		}
		
		pTrans = m_pText->GetTranslationText();
		if(!m_strTrans.IsEmpty())
		{
			int start, len;
			if(!m_pText->CheckSyntax(m_strTrans.GetBuffer(0),start,len,true))
			{
				CEdit *pEdit = (CEdit *)GetDlgItem(IDC_EDIT_ORG);
				pEdit->SetSel(start,start + len);
				return false;
			}
			bReplace = m_pText->ReplaceTransText(m_strTrans.GetBuffer(0));
			m_strTrans.ReleaseBuffer();
		}else
		{
			AfxMessageBox(_T("Translation text cannot empty!"));
			return false;
		}
		
		if(!bReplace) return true;
Rep:
		if(m_pText->GetStatus()==CTextBase::status_disuse)
			m_Flag.SetIcon(m_hDel);
		else if(m_pText->GetStatus()==CTextBase::status_new)
			m_Flag.SetIcon(m_hNew);
		else if(m_pText->GetStatus()==CTextBase::status_normal)
		{
			if(nOldStatus==CTextBase::status_new)
			{
				g_nNewTextNum--;
				g_nTransTextNum++;
				((CMainFrame*)AfxGetMainWnd())->UpdateTextStatus();
				m_Flag.SetIcon(m_hNormal);
				CElementDataView * pView = (CElementDataView *)((CMainFrame*)AfxGetMainWnd())->GetLeftView();
				pView->ChangeItemStatus(m_hSelItem,false);
			}
		}
		g_bModified = true;
	}
	return true;
}

void CElementLocalizeView::OnChangeEditTrans() 
{
	// TODO: If this is a RICHEDIT control, the control will not
	// send this notification unless you override the CFormView::OnInitDialog()
	// function and call CRichEditCtrl().SetEventMask()
	// with the ENM_CHANGE flag ORed into the mask.
	
	// TODO: Add your control notification handler code here
	UpdateData(true);
	UpdateTransInfo();
}

void CElementLocalizeView::UpdateTransInfo()
{
	CString txt;
	CWnd *pWnd = GetDlgItem(IDC_STATIC_TRANS_LENGTH);
	if(pWnd)
	{
		txt.Format(_T("Length: %d"),m_strTrans.GetLength());
		pWnd->SetWindowText(txt);
	}

	pWnd = GetDlgItem(IDC_STATIC_TRANS_LINE_NUM);
	if(pWnd)
	{
		txt.Format(_T("Line Num: %d"),Aux_GetLineNum(m_strTrans));
		pWnd->SetWindowText(txt);
	}
}

void CElementLocalizeView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CRect rc;
	GetClientRect(&rc);
	
	//page org
	CWnd *pWnd = GetDlgItem(IDC_STATIC_ORG);
	if(pWnd) pWnd->SetWindowPos(NULL,0,0,rc.Width() - 20,rc.Height()/2 - 30,SWP_NOMOVE);
	pWnd = GetDlgItem(IDC_EDIT_ORG);
	if(pWnd) 
	{
		pWnd->SetWindowPos(NULL,0,0,rc.Width() - 45,rc.Height()/2 - 115,SWP_NOMOVE);
		((CEdit*)pWnd)->SetLimitText(1024*500);
	}
	pWnd = GetDlgItem(IDC_STATIC_LENGTH);
	if(pWnd) pWnd->SetWindowPos(NULL,23,rc.Height()/2 - 80,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_STATIC_TEXT_LIMIT);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 100,rc.Height()/2 - 80,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_STATIC_IS_MULTI_LINE);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 200,rc.Height()/2 - 80,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_STATIC_LINE_NUM);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 300,rc.Height()/2 - 80,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_STATIC_FLAG);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 480,rc.Height()/2 - 80,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_EDIT_ID);
	if(pWnd) pWnd->SetWindowPos(NULL,23,rc.Height()/2 - 48,rc.Width() - 60, 25, SWP_SHOWWINDOW);
	
	//page trans
	pWnd = GetDlgItem(IDC_BUTTON_AUTO_TRANS);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 250,rc.Height()/2-15,10,10,SWP_NOSIZE);
	
	pWnd = GetDlgItem(IDC_STATIC_TRANS);
	if(pWnd) pWnd->SetWindowPos(NULL,11,rc.Height()/2 + 10,rc.Width() - 20,rc.Height()/2 - 30,SWP_SHOWWINDOW);
	pWnd = GetDlgItem(IDC_EDIT_TRANS);
	if(pWnd) 
	{
		pWnd->SetWindowPos(NULL,23,rc.Height()/2 + 35,rc.Width() - 45,rc.Height()/2 - 90,SWP_SHOWWINDOW);
		((CEdit*)pWnd)->SetLimitText(1024*500);
	}
	pWnd = GetDlgItem(IDC_STATIC_TRANS_LENGTH);
	if(pWnd) pWnd->SetWindowPos(NULL,23,rc.Height() - 53,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_STATIC_TRANS_LINE_NUM);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 100,rc.Height() - 53,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_BUTTON_REPLACE);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 250,rc.Height() - 53,10,10,SWP_NOSIZE);
	pWnd = GetDlgItem(IDC_BUTTON_REPLACE_NEXT);
	if(pWnd) pWnd->SetWindowPos(NULL,23 + 400,rc.Height() - 53,10,10,SWP_NOSIZE);
}

void CElementLocalizeView::OnButtonAutoTrans() 
{
	// TODO: Add your control notification handler code here
	if(m_pText)
	{
		if(m_pText->GetOriginalText())
		{
			if(IDYES == AfxMessageBox(_T("Do you want translate by machine ?"),MB_YESNO | MB_ICONQUESTION))
			{
				TCHAR *szBuffer = new TCHAR[_tcslen(m_pText->GetOriginalText()) + 1];
				j2b(m_pText->GetOriginalText(),szBuffer);
				m_strTrans = szBuffer;
				UpdateData(false);
				if(szBuffer) delete[] szBuffer;
			}
		}
	}
}

void CElementLocalizeView::OnButtonReplaceNext() 
{
	// TODO: Add your control notification handler code here
	if(!ReplaceText()) return;
	if(m_pText)
	{
		CString strTrans = m_strTrans;
		CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
		bool r = ((CElementDataView*) pFrame->GetLeftView())->FindDown_Ex(true);
		if(!r) 
		{
			MessageBox(_T("Cannot find next text!"));
			return;
		}
		m_strTrans = strTrans;
		UpdateData(false);
	}
}
