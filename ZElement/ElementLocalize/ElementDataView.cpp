// ElementDataView.cpp : implementation file
//

#include "stdafx.h"
#include "ElementLocalize.h"
#include "MainFrm.h"
#include "ElementLocalizeDoc.h"
#include "ElementLocalizeView.h"
#include "ElementDataView.h"
#include "DataManager.h"
#include "TemplateManager.h"
#include "TemplateBase.h"
#include "TextBase.h"
#include "FinderDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CFinderDlg g_FinderDlg;

/////////////////////////////////////////////////////////////////////////////
// CElementDataView

IMPLEMENT_DYNCREATE(CElementDataView, CFormView)

CElementDataView::CElementDataView()
	: CFormView(CElementDataView::IDD)
{
	//{{AFX_DATA_INIT(CElementDataView)
		// NOTE: the ClassWizard will add member initialization here
	for (int i=0; i<CHUNK_NUM; ++i)
		m_hItems[i] = NULL;
	m_nTextItem = 0;
	//}}AFX_DATA_INIT
}

CElementDataView::~CElementDataView()
{
}

void CElementDataView::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CElementDataView)
	DDX_Control(pDX, IDC_TREE_DATA, m_treeData);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CElementDataView, CFormView)
	//{{AFX_MSG_MAP(CElementDataView)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_DATA, OnSelchangedTreeData)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CElementDataView diagnostics

#ifdef _DEBUG
void CElementDataView::AssertValid() const
{
	CFormView::AssertValid();
}

void CElementDataView::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CElementDataView message handlers

void CElementDataView::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	if(m_treeData.m_hWnd)
	{
		RECT rc;
		GetClientRect(&rc);
		int cx = rc.right - rc.left - 1;
		int cy = rc.bottom - rc.top - 1;
		if(cx > 0 && cy > 0)
			m_treeData.SetWindowPos(NULL, 0, 0, cx, cy,SWP_NOMOVE);
	}
}

void CElementDataView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	// TODO: Add your specialized code here and/or call the base class
	m_ImageList.Create(IDB_LISTTAG, 16, 3, RGB(255, 0, 255));
	m_treeData.SetImageList(&m_ImageList,TVSIL_NORMAL);
	
	UpdateTree();
	
}

void CElementDataView::UpdateTree()
{
	m_treeData.DeleteAllItems();
	for (int i=0; i<CHUNK_NUM; ++i)
		m_hItems[i] = NULL;
	m_nTextItem = 0;
	
	CString strText;
	HTREEITEM hParent = m_treeData.InsertItem(_T("Text from template data"),2,2);
	m_hItems[CHUNK_ELEMENT_DATA] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementDataTM(),true);

	hParent = m_treeData.InsertItem(_T("Text from interface hint data"),2,2);
	m_hItems[CHUNK_ELEMENT_INTERFACEHINT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementInterfaceHintTM());

	hParent = m_treeData.InsertItem(_T("Text from interface string data"),2,2);
	m_hItems[CHUNK_ELEMENT_INTERFACESTRING] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementInterfaceStringTM());

	hParent = m_treeData.InsertItem(_T("Text from task data"),2,2);
	m_hItems[CHUNK_ELEMENT_TASK] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementTaskTM());

	hParent = m_treeData.InsertItem(_T("Text from policy data"),2,2);
	m_hItems[CHUNK_ELEMENT_POLICY] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementPolicyTM());

	hParent = m_treeData.InsertItem(_T("Text from item description data"),2,2);
	m_hItems[CHUNK_ELEMENT_ITEMDESC] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementItemDescTM());

	hParent = m_treeData.InsertItem(_T("Text from item description ext data"),2,2);
	m_hItems[CHUNK_ELEMENT_ITEMEXDESC] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementItemExDescTM());
	
	hParent = m_treeData.InsertItem(_T("Text from instance text data"),2,2);
	m_hItems[CHUNK_ELEMENT_INSTANCE_TEXT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementInstanceTextTM());

	hParent = m_treeData.InsertItem(_T("Text from skill description data"),2,2);
	m_hItems[CHUNK_ELEMENT_SKILLDESC] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementSkillDescTM());

	hParent = m_treeData.InsertItem(_T("Text from fixed message data"),2,2);
	m_hItems[CHUNK_ELEMENT_FIXMSG] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementFixMsgTM());

	hParent = m_treeData.InsertItem(_T("Text from server error data"),2,2);
	m_hItems[CHUNK_ELEMENT_SERVERERR] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementServerErrTM());

	hParent = m_treeData.InsertItem(_T("Text from buff description data"),2,2);
	m_hItems[CHUNK_ELEMENT_BUFFDESC] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementBuffDescTM());

	hParent = m_treeData.InsertItem(_T("Text from task error data"),2,2);
	m_hItems[CHUNK_ELEMENT_TASKERR] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementTaskErrTM());
	
	hParent = m_treeData.InsertItem(_T("Text from loading hint data"),2,2);
	m_hItems[CHUNK_ELEMENT_LOADINGHINT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementLoadingHintTM());
	
	hParent = m_treeData.InsertItem(_T("Text from pet talk data"),2,2);
	m_hItems[CHUNK_ELEMENT_PETTALK] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementPetTalkTM());
	
	hParent = m_treeData.InsertItem(_T("Text from addon string data"),2,2);
	m_hItems[CHUNK_ELEMENT_ADDONSTR] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementAddonStrTM());
	
	hParent = m_treeData.InsertItem(_T("Text from precinct data"),2,2);
	m_hItems[CHUNK_ELEMENT_PRECINCT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementPrecinctTM());

	hParent = m_treeData.InsertItem(_T("Text from help script data"),2,2);
	m_hItems[CHUNK_ELEMENT_HELPSCRIPT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementHelpScriptTM());

	hParent = m_treeData.InsertItem(_T("Text from help text data"),2,2);
	m_hItems[CHUNK_ELEMENT_HELPTEXT] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementHelpTextTM());

	hParent = m_treeData.InsertItem(_T("Text from gshop data"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementGShopTM());

	hParent = m_treeData.InsertItem(_T("Text from gshop types"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP_TYPES] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementGShopTypesTM());

	hParent = m_treeData.InsertItem(_T("Text from gshop1 data"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP1] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementGShop1TM());
	
	hParent = m_treeData.InsertItem(_T("Text from gshop1 types"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP_TYPES1] = hParent;		
	UpdateElementPart(hParent,g_DataManager.GetElementGShopTypes1TM());
	
	hParent = m_treeData.InsertItem(_T("Text from gshop2 data"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP2] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementGShop2TM());

	hParent = m_treeData.InsertItem(_T("Text from gshop2 types"),2,2);
	m_hItems[CHUNK_ELEMENT_GSHOP_TYPES2] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementGShopTypes2TM());

	hParent = m_treeData.InsertItem(_T("Text from domain data"),2,2);
	m_hItems[CHUNK_ELEMENT_DOMAININFOR] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementDomainInforTM());
	
	hParent = m_treeData.InsertItem(_T("Text from domain1 data"),2,2);
	m_hItems[CHUNK_ELEMENT_DOMAININFOR1] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementDomainInfor1TM());

	hParent = m_treeData.InsertItem(_T("Text from script file"),2,2);
	m_hItems[CHUNK_ELEMENT_TEXTFILE] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementTextFileTM());
	
	hParent = m_treeData.InsertItem(_T("Text from hometown data file"),2,2);
	m_hItems[CHUNK_ELEMENT_HOMETOWN] = hParent;
	UpdateElementPart(hParent,g_DataManager.GetElementHomeTownDataTM());
}

void CElementDataView::UpdateElementPart(HTREEITEM hParent,CTemplateManager *pMan,bool bSetTemplateID)
{
	CString strText;
	if(pMan)
	{
		HTREEITEM hPart = NULL; 
		int n = pMan->GetTempalteNum();
		for( int i = 0; i < n; ++i)
		{
			if(i%100 == 0)
			{
				strText.Format(_T("Part-%d"),i/100);
				hPart = m_treeData.InsertItem(strText,2,2,hParent);
				m_treeData.SetItemData(hPart,0);
			}
			
			CTemplateBase* pTemplate = pMan->GetTemplateByIdx(i);
			strText.Format(_T("Template(%d)-%d"),pTemplate->GetID(),i);
			HTREEITEM hTemplate = m_treeData.InsertItem(strText,2,2,hPart);
			if(bSetTemplateID) m_treeData.SetItemData(hTemplate,pTemplate->GetID());
			else m_treeData.SetItemData(hTemplate,0);
			int num = EnumSubTemplate(hParent,hPart,hTemplate,pTemplate);
			if (num == 0)
			{
				m_treeData.DeleteItem(hTemplate);
			}
		}
	}
}

int CElementDataView::EnumSubTemplate(HTREEITEM hParent,HTREEITEM hPart, HTREEITEM hTemplate, CTemplateBase *pParentTempl)
{
	int num = 0;
	int m = pParentTempl->GetTextNum();
	for( int j = 0; j < m; ++j)
	{
		CTextBase *pText = pParentTempl->GetTextByIdx(j);
		if(pText->GetOriginalText() == NULL) continue;
		CString csOrg(pText->GetOriginalText());		
		if(csOrg == _T("")) 
		{
			pText->SetStatus(CTextBase::status_normal);
		}
		int nIcon = 0; 
		if(pText->GetStatus()==CTextBase::status_new)
		{
			m_treeData.SetItemImage(hParent,0,0);
			m_treeData.SetItemImage(hPart,0,0);
			m_treeData.SetItemImage(hTemplate,0,0);
			nIcon = 5;
		}else if( pText->GetStatus()==CTextBase::status_normal)
		{
			nIcon = 3;
		}else 
		{
			nIcon = 4;
		}
		CString strText;
		strText.Format(_T("%s"),pText->GetTextName());
		HTREEITEM hText = m_treeData.InsertItem(strText,nIcon,nIcon,hTemplate,TVI_SORT);
		m_treeData.SetItemData(hText,(DWORD)pText);
		++num;
		++m_nTextItem;
	}
	
	for( j = 0; j < pParentTempl->GetChildNum();++j)
	{
		CTemplateBase *pChild = pParentTempl->GetChildByIndex(j);
		num += EnumSubTemplate(hParent,hPart,hTemplate,pChild);
	}	
	return num;
}

void CElementDataView::OnSelchangedTreeData(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	 HTREEITEM hSel = m_treeData.GetSelectedItem();
	 if(hSel!=NULL)
	 {
		CTextBase *pText = (CTextBase *)m_treeData.GetItemData(hSel); 
		int nImage,nSelImage;
		m_treeData.GetItemImage(hSel,nImage,nSelImage);
		if(pText!=NULL && nImage > 2)
		{
			CElementLocalizeView *pView = (CElementLocalizeView *)((CMainFrame*)AfxGetMainWnd())->GetRightView();
			CString temp = GetPath(hSel);
			if(!temp.IsEmpty()) 
			{
				CString txt;
				txt.Format(_T("ElementLocalize - [%s]"),temp);
				AfxGetMainWnd()->SetWindowText(txt);
			}else AfxGetMainWnd()->SetWindowText(_T("ElementLocalize"));
			pView->ChangeTextBase(pText,hSel);
		}
	 }
	*pResult = 0;
}

void CElementDataView::ChangeItemStatus(HTREEITEM hItem, bool bNew)
{
	if(bNew) m_treeData.SetItemImage(hItem,5,5);
	else m_treeData.SetItemImage(hItem,3,3);
	
	int nImage,nSelImage;
	HTREEITEM hParent = hItem;
	while(hParent = m_treeData.GetParentItem(hParent))
	{
		bool bIsNew = false;
		HTREEITEM hChild = m_treeData.GetChildItem(hParent);
		while(hChild)
		{
			m_treeData.GetItemImage(hChild,nImage,nSelImage);
			if(nImage == 5 || nImage == 0)
			{
				bIsNew = true;
				break;
			}
			hChild = m_treeData.GetNextItem(hChild,TVGN_NEXT);
		}
		if(bIsNew) m_treeData.SetItemImage(hParent,0,0);
		else m_treeData.SetItemImage(hParent,2,2);
	}
}

bool CElementDataView::FindNewItem()
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	HTREEITEM hOldItem = NULL;
	HTREEITEM hSelItem = hItem;
	bool bInited = false;
	bool bNoCur = true;
	CTextBase *pText = NULL; 
	while(true)
	{
		if(bNoCur && !bInited) goto lnext;
		if(hSelItem==NULL) return false;
		
		int nImage,nSelImage;
		m_treeData.GetItemImage(hSelItem,nImage,nSelImage);
		if(nImage > 2) 
		{
			
			pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
			if(pText!=NULL) 
			{
				if(pText->GetStatus()==CTextBase::status_new)
				{
					m_treeData.SelectItem(hSelItem);
					return true;
				}
			}
		}
lnext:
		hOldItem = hSelItem;
		hSelItem = FindNextItem(hOldItem);
		if(hSelItem==hOldItem) 
			return false;
		bInited = true;
	}
	return false;
}

bool CElementDataView::FindNewItemEx()
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	HTREEITEM hOldItem = NULL;
	HTREEITEM hSelItem = hItem;
	bool bInited = false;
	bool bNoCur = true;
	CTextBase *pText = NULL;
	while(true)
	{
		if(bNoCur && !bInited) goto nnext;
		if(hSelItem==NULL) return false;
		
		int nImage,nSelImage;
		m_treeData.GetItemImage(hSelItem,nImage,nSelImage);
		if(nImage > 2) 
		{
			pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
			if(pText!=NULL) 
			{
				if(pText->GetStatus()==CTextBase::status_new)
				{
					m_treeData.SelectItem(hSelItem);
					return true;
				}
			}
		}
nnext:
		hOldItem = hSelItem;
		hSelItem = FindLastItem(hSelItem);
		if(hSelItem==hOldItem) return false;
		bInited = true;
	}
	return false;
}

void CElementDataView::NextItem()
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	HTREEITEM hOldItem = hItem;
	HTREEITEM hSelItem = FindNextItem(hItem);
	if(hSelItem!=NULL && hSelItem!=hOldItem)
		m_treeData.SelectItem(hSelItem);
}

void CElementDataView::PrevItem()
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	HTREEITEM hOldItem = hItem;
	HTREEITEM hSelItem = FindLastItem(hItem);
	if(hSelItem!=NULL && hSelItem!=hOldItem)
		m_treeData.SelectItem(hSelItem);
}

HTREEITEM CElementDataView::EnumFindDown(HTREEITEM hParent)
{
	HTREEITEM hChild = m_treeData.GetChildItem(hParent);
	if(hChild==NULL) return hParent;
	
	while(hChild)
	{
		HTREEITEM hItem = EnumFindDown(hChild);
		if(hItem) return hItem;
		hChild = m_treeData.GetNextItem(hChild,TVGN_NEXT);
	}
	return NULL;
}

HTREEITEM CElementDataView::EnumFindUp(HTREEITEM hParent)
{
	HTREEITEM hChild = m_treeData.GetChildItem(hParent);
	if(hChild==NULL) return hParent;
	
	HTREEITEM hOldChild = NULL;
	while(hChild)
	{
		hOldChild = hChild;
		hChild = m_treeData.GetNextSiblingItem(hChild);
	}

	while(hOldChild)
	{
		HTREEITEM hItem = EnumFindUp(hOldChild);
		if(hItem) return hItem;
		hOldChild = m_treeData.GetPrevSiblingItem(hOldChild);
	}
	return NULL;
}

bool CElementDataView::FindUp(bool bNoCur)
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	HTREEITEM hOldItem = NULL;
	HTREEITEM hSelItem = hItem;
	bool bInited = false;
	CTextBase *pText = NULL;
	CString key = g_FinderDlg.m_strText;	
	int id = _ttoi(g_FinderDlg.m_strText);
	key.MakeLower();
	while(true)
	{
		if(bNoCur && !bInited) goto nnext;
		if(hSelItem==NULL) return false;
		
		if(!m_treeData.ItemHasChildren(hSelItem) && g_FinderDlg.m_nSelType!=eTempID)
		{
			int nImage,nSelImage;
			m_treeData.GetItemImage(hSelItem,nImage,nSelImage);
			if(nImage > 2) 
			{
				
				pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
				if(pText==NULL) goto nnext;
				if(g_FinderDlg.m_bOnlyNewItem)
				{
					if(pText->GetStatus()!=CTextBase::status_new)
						goto nnext;
				}
				
				CString strText;
				if(g_FinderDlg.m_nSelType==eOrginText) strText = pText->GetOriginalText();
				else if(g_FinderDlg.m_nSelType==eTransText) strText = pText->GetTranslationText();
				else if(g_FinderDlg.m_nSelType==eGlobalID) strText = pText->GetStrID();
				else goto nnext;
				strText.MakeLower();
				if(g_FinderDlg.m_bMatchWhole || g_FinderDlg.m_nSelType==eGlobalID)
				{
					if(key==strText)
					{
						m_treeData.SelectItem(hSelItem);
						return true;
					}
				}else
				{
					if(-1 != strText.Find(key,0))
					{
						m_treeData.SelectItem(hSelItem);
						return true;
					}
				}
			}
		}
		if (g_FinderDlg.m_nSelType==eTempID)
		{
			HTREEITEM hItemTemp = NULL;
			if (m_treeData.ItemHasChildren(hSelItem))
				hItemTemp = hSelItem;
			else
				hItemTemp = m_treeData.GetParentItem(hSelItem);
			if (hItemTemp && m_treeData.GetItemData(hItemTemp) == id)
			{
				m_treeData.SelectItem(hItemTemp);
				return true;
			}
		}
nnext:
		hOldItem = hSelItem;
		if (g_FinderDlg.m_nSelType == eGlobalID && pText)
		{
			int nTok = pText->GetStrID().Find(STRID_TOK);
			if (nTok > 0)
			{
				CString strID = pText->GetStrID().Left(nTok);
				CString strKey = key.Left(key.Find(STRID_TOK));
				if (strID != strKey)
					hOldItem = FindLastRoot(hOldItem);
				if (hOldItem == NULL)
					return FALSE;
			}
		}
		hSelItem = FindLastItem(hOldItem);
		if(hSelItem==hOldItem) return false;
		bInited = true;
	}
	return false;
}

bool CElementDataView::FindDown(bool bNoCur)
{
	HTREEITEM hItem = m_treeData.GetSelectedItem();
	if(hItem==NULL) hItem = m_treeData.GetChildItem(NULL);
	
	HTREEITEM hOldItem = NULL;
	HTREEITEM hSelItem = hItem;
	bool bInited = false;
	CTextBase *pText = NULL; 
	CString key = g_FinderDlg.m_strText;
	int id = _ttoi(g_FinderDlg.m_strText);
	key.MakeLower();
	while(true)
	{
		if(bNoCur && !bInited) goto lnext;
		if(hSelItem==NULL) return false;
		int nImage,nSelImage;
		m_treeData.GetItemImage(hSelItem,nImage,nSelImage);
		if(nImage > 2 && !m_treeData.ItemHasChildren(hSelItem) && g_FinderDlg.m_nSelType!=eTempID) 
		{
			pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
			if(pText==NULL) goto lnext;
			
			if(g_FinderDlg.m_bOnlyNewItem)
			{
				if(pText->GetStatus()!=CTextBase::status_new)
					goto lnext;
			}
			
			CString strText;
			if(g_FinderDlg.m_nSelType==eOrginText) strText = pText->GetOriginalText();
			else if(g_FinderDlg.m_nSelType==eTransText) strText = pText->GetTranslationText();
			else if(g_FinderDlg.m_nSelType==eGlobalID) strText = pText->GetStrID();
			else goto lnext;
			strText.MakeLower();
			if(g_FinderDlg.m_bMatchWhole || g_FinderDlg.m_nSelType==eGlobalID)
			{
				if(key==strText)
				{
					m_treeData.SelectItem(hSelItem);
					return true;
				}
			}else
			{
				if(-1 != strText.Find(key,0))
				{
					m_treeData.SelectItem(hSelItem);
					return true;
				}
			}
		}
		if (g_FinderDlg.m_nSelType==eTempID)
		{
			HTREEITEM hItemTemp = NULL;
			if (m_treeData.ItemHasChildren(hSelItem))
				hItemTemp = hSelItem;
			else
				hItemTemp = m_treeData.GetParentItem(hSelItem);
			if (hItemTemp && m_treeData.GetItemData(hItemTemp) == id)
			{
				m_treeData.SelectItem(hItemTemp);
				return true;
			}
		}
lnext:
		hOldItem = hSelItem;
		if (g_FinderDlg.m_nSelType == eGlobalID && pText)
		{
			int nTok = pText->GetStrID().Find(STRID_TOK);
			if (nTok > 0)
			{
				CString strID = pText->GetStrID().Left(nTok);
				CString strKey = key.Left(key.Find(STRID_TOK));
				if (strID != strKey)
					hOldItem = FindNextRoot(hOldItem);
				if (hOldItem == NULL)
					return FALSE;
			}
		}
		hSelItem = FindNextItem(hOldItem);
		if(hSelItem==hOldItem) 
			return false;
		bInited = true;
	}
	return false;
}

bool CElementDataView::FindDown_Ex(bool bNoCur)
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	HTREEITEM hItem = ((CElementLocalizeView*) pFrame->GetRightView())->GetCurItem();
	if(hItem==NULL) return false;

	HTREEITEM hOldItem = NULL;
	HTREEITEM hSelItem = hItem;
	bool bInited = false;
	CTextBase *pText = NULL; 
	pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
	if(pText==NULL) return false;
	CString key = pText->GetOriginalText();
	key.MakeLower();
	pText = NULL;

	while(true)
	{
		if(bNoCur && !bInited) goto lnext;
		if(hSelItem==NULL) return false;
		
		int nImage,nSelImage;
		m_treeData.GetItemImage(hSelItem,nImage,nSelImage);
		if(nImage > 2) 
		{
			pText = (CTextBase *)m_treeData.GetItemData(hSelItem);
			if(pText!=NULL) 
			{
				CString strText;
				if(g_FinderDlg.m_nSelType==eOrginText) strText = pText->GetOriginalText();
				else if(g_FinderDlg.m_nSelType==eTransText) strText = pText->GetTranslationText();
				else goto lnext;
				strText.MakeLower();
				
				if(key==strText)
				{
					m_treeData.SelectItem(hSelItem);
					return true;
				}
				
			}
		}
lnext:
		hOldItem = hSelItem;
		hSelItem = FindNextItem(hOldItem);
		if(hSelItem==hOldItem) 
			return false;
		bInited = true;
	}
	return false;
}


bool CElementDataView::FindByID()
{
	HTREEITEM hItem = EnumFindByID(NULL,_ttoi(g_FinderDlg.m_strText));
	if(hItem) 
	{
		m_treeData.SelectItem(hItem);
		return true;
	}
	return false;
}

HTREEITEM CElementDataView::EnumFindByID(HTREEITEM hParent, unsigned int id)
{
	if (hParent != NULL)
	{
		int iImage,iSelImage;
		m_treeData.GetItemImage(hParent,iImage,iSelImage);
		if(iImage > 2) return NULL;
		if(m_treeData.GetItemData(hParent)==id) return hParent;
	}	
	
	HTREEITEM hChild = m_treeData.GetChildItem(hParent);
	while(hChild)
	{
		HTREEITEM hr = EnumFindByID(hChild,id);
		if(hr!=NULL) return hr;
		hChild = m_treeData.GetNextItem(hChild,TVGN_NEXT);
	}
	
	return NULL;
}

HTREEITEM CElementDataView::FindNextItem(HTREEITEM hCurItem)
{
	HTREEITEM hItem = hCurItem;
	
	if(m_treeData.GetChildItem(hItem)!=NULL)
		return EnumFindDown(hItem);
	
	HTREEITEM hNext = m_treeData.GetNextItem(hItem,TVGN_NEXT);
	if(hNext) return hNext;
	else
	{
		HTREEITEM hParent = m_treeData.GetParentItem(hItem);
		while(hParent)
		{
			hNext = m_treeData.GetNextItem(hParent,TVGN_NEXT);
			if(hNext==NULL) hParent = m_treeData.GetParentItem(hParent);
			else return EnumFindDown(hNext);
		}
	}
	return NULL;
}
HTREEITEM CElementDataView::FindNextItem(HTREEITEM hCurItem, HTREEITEM hRootItem)
{
	HTREEITEM hParent = hCurItem;
	while (hParent && hParent != hRootItem)
		hParent = m_treeData.GetParentItem(hParent);
	if (hParent != hRootItem)
		return NULL;
	if(m_treeData.GetChildItem(hCurItem)!=NULL)
		return EnumFindDown(hCurItem);
	if (hCurItem == hRootItem)
		return NULL;
	HTREEITEM hNext = m_treeData.GetNextItem(hCurItem,TVGN_NEXT);
	if(hNext) return hNext;
	else
	{
		HTREEITEM hParent = m_treeData.GetParentItem(hCurItem);
		while(hParent)
		{
			hNext = m_treeData.GetNextItem(hParent,TVGN_NEXT);
			if(hNext==NULL) hParent = m_treeData.GetParentItem(hParent);
			else return EnumFindDown(hNext);
		}
	}
	return NULL;
}

HTREEITEM CElementDataView::FindLastItem(HTREEITEM hCurItem)
{
	HTREEITEM hItem = hCurItem;
	
	if(m_treeData.GetChildItem(hItem)!=NULL)
		return EnumFindUp(hItem);
		
	HTREEITEM hPrev = m_treeData.GetPrevSiblingItem(hItem);
	if(hPrev) return hPrev;
	else
	{
		HTREEITEM hParent = m_treeData.GetParentItem(hItem);
		while(hParent)
		{
			hPrev = m_treeData.GetPrevSiblingItem(hParent);
			if(hPrev==NULL) hParent = m_treeData.GetParentItem(hParent);
			else return EnumFindUp(hPrev);
		}
	}
	return NULL;
}


CString CElementDataView::GetPath(HTREEITEM hItem)
{
	if(hItem==NULL) return "";
	
	CString txt = m_treeData.GetItemText(hItem);
	
	HTREEITEM hParent = m_treeData.GetParentItem(hItem);
	while(hParent)
	{
		CString temp = m_treeData.GetItemText(hParent);
		txt = temp + _T("\\") + txt;
		hParent = m_treeData.GetParentItem(hParent);
	}
	return txt;
}

void CElementDataView::GoCurItem()
{
	CMainFrame *pFrame = (CMainFrame*)AfxGetMainWnd();
	HTREEITEM hCurItem = ((CElementLocalizeView*) pFrame->GetRightView())->GetCurItem();
	m_treeData.SelectItem(hCurItem);
}


HTREEITEM CElementDataView::FindNextRoot(HTREEITEM hCurItem)
{
	HTREEITEM hTemp = NULL;
	while (NULL != (hTemp=m_treeData.GetParentItem(hCurItem)))
	{
		hCurItem = hTemp;
	}
	return m_treeData.GetNextItem(hCurItem, TVGN_NEXT);
}

HTREEITEM CElementDataView::FindLastRoot(HTREEITEM hCurItem)
{
	HTREEITEM hTemp = NULL;
	while (NULL != (hTemp=m_treeData.GetParentItem(hCurItem)))
	{
		hCurItem = hTemp;
	}
	return m_treeData.GetNextItem(hCurItem, TVGN_PREVIOUS);
}
HTREEITEM CElementDataView::GetNextTextItem(HTREEITEM hItem, HTREEITEM hRootItem)
{
	HTREEITEM hNextleaf = FindNextItem(hItem, hRootItem);
	if (!hNextleaf)
		return NULL;
	int nImage,nSelImage;
	m_treeData.GetItemImage(hNextleaf,nImage,nSelImage);
	while (hNextleaf && nImage <= 2)
	{
		hNextleaf = FindNextItem(hNextleaf);
		m_treeData.GetItemImage(hNextleaf,nImage,nSelImage);
	}
	return hNextleaf;
}
