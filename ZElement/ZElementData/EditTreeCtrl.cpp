// EditTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "EditTreeCtrl.h"
#include "TreasureItemDlg.h"

const char *szMainType[] = 
{
	"礼包",
	"仙丹",
	"法器",
	"时装",
	"仙宠",
	"天材地宝",
	"其他",
	"下架物品"
};

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "InputSubTypeNameDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CEditTreeCtrl

CEditTreeCtrl::CEditTreeCtrl()
{
	m_pTreasureMan = NULL;
	m_bChanged = false;
	m_bReadOnly = false;
	m_bEditting = false;
	m_bDragging = false; 
}

CEditTreeCtrl::~CEditTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CEditTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CEditTreeCtrl)
	ON_WM_NCRBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_MENU_ADDSUB, OnMenuAddsub)
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_MENU_ADD_TREASURE, OnMenuAddTreasure)
	ON_COMMAND(ID_MENU_DEL_TREASURE, OnMenuDelTreasure)
	ON_COMMAND(ID_MENU_DELSUB, OnMenuDelsub)
	ON_COMMAND(ID_MENU_ADDMAIN, OnMenuAddmain)
	ON_COMMAND(ID_MENU_DELMAIN, OnMenuDelmain)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_COMMAND(ID_MENU_DOFF_TREASURE, OnMenuDoffTreasure)
	ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEditTreeCtrl message handlers



void CEditTreeCtrl::OnNcRButtonDown(UINT nHitTest, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTreeCtrl::OnNcRButtonDown(nHitTest, point);
}

void CEditTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint pc;
	GetCursorPos(&pc);
	POINT pt = {pc.x, pc.y};
    ScreenToClient(&pt);
	CPoint client(pt.x,pt.y);
	pt.x = pc.x;
	pt.y = pc.y;
	
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_ADD_SUB);
	
	CMenu* pPopupMenu = Menu.GetSubMenu(0);
	ASSERT(pPopupMenu);
	UpdateMenu(pPopupMenu);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	
	*pResult = 0;
}

void CEditTreeCtrl::UpdateMenu(CMenu *pMenu)
{
	HTREEITEM hSel = GetSelectedItem();
	if(hSel==NULL)
	{
		pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_ADDMAIN,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DELMAIN,MF_GRAYED);
		return;
	}

	pMenu->EnableMenuItem(ID_MENU_ADDMAIN,MF_GRAYED);
	pMenu->EnableMenuItem(ID_MENU_DELMAIN,MF_GRAYED);

	HTREEITEM htemp = GetParentItem(hSel);
	HTREEITEM hLast = hSel;
	while(htemp)
	{
		hLast = htemp;
		htemp = GetParentItem(htemp);
	}

	if(GetItemText(hLast)=="百宝阁NPC数据")
	{//单独处理百宝阁NPC数据
		
		pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_ADDMAIN,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MENU_DELMAIN,MF_GRAYED);
		
		if(GetParentItem(hSel)==NULL) 
		{
			pMenu->EnableMenuItem(ID_MENU_ADDMAIN,MF_ENABLED);
			return;
		}else pMenu->EnableMenuItem(ID_MENU_ADDMAIN,MF_GRAYED);
		
		HTREEITEM hParent = GetParentItem(hSel);
		HTREEITEM hPP = GetParentItem(hParent);
		
		if(hPP==NULL) 
		{
			pMenu->EnableMenuItem(ID_MENU_DELMAIN,MF_ENABLED);
			pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_ENABLED);
			return;
		}
		
		HTREEITEM hPPP = GetParentItem(hPP);
		if(hPPP==NULL) 
		{
			pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_ENABLED);
			pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_ENABLED);
			return;
		}
		pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_ENABLED);
		return;
	}


	if(GetParentItem(hSel)==NULL && GetChildItemsCount(hSel) < 8) 
		pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_ENABLED);
	else pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_GRAYED);
	

	HTREEITEM hParent = GetParentItem(hSel);
	HTREEITEM hPP = GetParentItem(hParent);

	if(hParent!=NULL && hPP==NULL) 
		pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_ENABLED);
	else pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_GRAYED);
		
	if(hParent!=NULL && hPP==NULL) 
		pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_ENABLED);
	else pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_GRAYED);
	

	if(hParent != NULL && hPP != NULL) pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_ENABLED);
	else pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_GRAYED);

	if(hParent != NULL && hPP != NULL && GetRejectItem()!=NULL && GetItemData(hPP) != 7) pMenu->EnableMenuItem(ID_MENU_DOFF_TREASURE,MF_ENABLED);
	else pMenu->EnableMenuItem(ID_MENU_DOFF_TREASURE,MF_GRAYED);
}

int  CEditTreeCtrl::GetChildItemsCount(HTREEITEM hParent)
{
	HTREEITEM child = GetChildItem(hParent);
	int n = 0;
	while(child)
	{
		n++;
		child = GetNextItem(child,TVGN_NEXT);
	}

	return n;
}

void CEditTreeCtrl::OnMenuAddsub() 
{
	// TODO: Add your command handler code here
	CInputSubTypeNameDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		if(!dlg.m_strSubTypeName.IsEmpty())
		{
			HTREEITEM hSel = GetSelectedItem();
			int mid = GetItemData(hSel);
			HTREEITEM hcur = InsertItem(dlg.m_strSubTypeName,GetSelectedItem());
			int id = m_pTreasureMan->GenSubID();
			SetItemData(hcur,id);
			SetItemImage(hcur,1,1);
			MAIN_TYPE *pType = FindMainType(mid);
			SUB_TYPE subType;
			subType.id = id;
			strcpy(subType.szName,dlg.m_strSubTypeName);
			pType->listSubType.push_back(subType);
			m_bChanged = true;
		}
	}
}

void CEditTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{


	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	if(pTVDispInfo->item.lParam==0)
		*pResult = 1;
	else
		*pResult = 0;

}

void CEditTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDI = (TV_DISPINFO*)pNMHDR;
	if(pTVDI->item.pszText!=NULL) //判断是否已经取消取消编辑
	{
		int nImage,nSelImage;
		GetItemImage(pTVDI->item.hItem,nImage,nSelImage);
		if(nImage==1)
		{
			SetItemText(pTVDI->item.hItem,pTVDI->item.pszText);
			ReplaceSubTextName(GetItemData(pTVDI->item.hItem),pTVDI->item.pszText);
			m_bChanged = true;
		}else if( nImage==0)
		{//必须是NPC百宝阁才能改主类型的名字
			HTREEITEM hParent = GetParentItem(pTVDI->item.hItem);
			if(hParent==NULL) return;
			if(GetItemText(hParent)=="百宝阁NPC数据")
			{
				CString strName;
				CString NewName = pTVDI->item.pszText;
				int pos = NewName.Find("(");
				if(pos!=-1)
					NewName = NewName.Left(pos);
				
				strName.Format("%s(id=%d)",NewName,GetItemData(pTVDI->item.hItem));
				SetItemText(pTVDI->item.hItem,strName);
				ReplaceMainTextName(GetItemData(pTVDI->item.hItem),NewName);
				m_bChanged = true;
			}
		}
	}
	//重置显示字符
	*pResult = 0;
}

void CEditTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		int nImage,nSelImage;
		GetItemImage(hsel,nImage,nSelImage);
		if(nImage != 2) return;

		int id = GetItemData(hsel);
		TREASURE_ITEM_VERSION11 *pObj = m_pTreasureMan->GetTreasureByID(id);
		if(pObj==NULL) return;
		CTreasureItemDlg dlg;
		memcpy(&dlg.m_Item,pObj,sizeof(TREASURE_ITEM_VERSION11));
		dlg.SetReadOnly(m_bReadOnly);
		if(dlg.DoModal()==IDOK)
		{
			if(ObjIsEqual(pObj,&dlg.m_Item)) return;
			memcpy(pObj,&dlg.m_Item,sizeof(TREASURE_ITEM_VERSION11));	
			SetItemText(hsel,pObj->szName);
			m_bChanged = true;
		}
	}
	*pResult = 0;
}

MAIN_TYPE* CEditTreeCtrl::FindMainType( int id)
{
	if(id < 20)
	{
		MAIN_TYPE *pList = m_pTreasureMan->GetMainTypeList();
		for( int i = 0; i < 8; ++i)
		{
			if(id == pList[i].id)
				return  &(pList[i]);
		}
	}else
	{
		abase::vector<MAIN_TYPE *>* pList = m_pTreasureMan->GetNpcMainTypeList();
		for( int i = 0; i < pList->size(); ++i)
		{
			if(id == (*pList)[i]->id)
				return  (*pList)[i];
		}
	}
	return NULL;
}

void CEditTreeCtrl::ReplaceSubTextName( int id, const char *szName)
{
	MAIN_TYPE *pList = m_pTreasureMan->GetMainTypeList();
	int i;
	for( i = 0; i < 8; ++i)
	{
		for( int j = 0; j < pList[i].listSubType.size(); ++j)
		{
			if(pList[i].listSubType[j].id == id)
			{
				strcpy(pList[i].listSubType[j].szName,szName);
				return;
			}
		}
	}


	abase::vector<MAIN_TYPE *>* pNpcList = m_pTreasureMan->GetNpcMainTypeList();
	for( i = 0; i < pNpcList->size(); ++i)
	{
		for( int j = 0; j < (*pNpcList)[i]->listSubType.size(); ++j)
		{
			if((*pNpcList)[i]->listSubType[j].id == id)
			{
				strcpy((*pNpcList)[i]->listSubType[j].szName,szName);
				return;
			}
		}
	}
}

void CEditTreeCtrl::ReplaceMainTextName( int id, const char *szName)
{

	abase::vector<MAIN_TYPE *>* pNpcList = m_pTreasureMan->GetNpcMainTypeList();
	for(int i = 0; i < pNpcList->size(); ++i)
	{
		if((*pNpcList)[i]->id == id)
			strcpy((*pNpcList)[i]->szName,szName);
	}
}

bool CEditTreeCtrl::ObjIsEqual(TREASURE_ITEM_VERSION11 *pObj1, TREASURE_ITEM_VERSION11 *pObj2)
{
	if(pObj1->count == pObj2->count &&
		pObj1->id == pObj2->id &&
		pObj1->others == pObj2->others &&
		pObj1->bonus == pObj2->bonus &&
		pObj1->discount == pObj2->discount &&
		pObj1->buy_fashion[0].price == pObj2->buy_fashion[0].price &&
		pObj1->buy_fashion[0].time == pObj2->buy_fashion[0].time &&
		stricmp(pObj1->szName,pObj2->szName)==0 &&
		pObj1->main_type == pObj2->main_type &&
		pObj1->sub_type == pObj2->sub_type &&
		pObj1->tid == pObj2->tid &&
		stricmp(pObj1->file_icon,pObj2->file_icon)==0 &&
		_wcsicmp((WCHAR*)pObj1->desc,(WCHAR*)pObj2->desc)==0 &&

		//新加赠品
		pObj1->bHasPresent == pObj2->bHasPresent &&
		stricmp(pObj1->szPresentName,pObj2->szPresentName)==0 &&
		pObj1->Presentid == pObj2->Presentid &&
		pObj1->Presentcount == pObj2->Presentcount &&
		pObj1->Presenttime == pObj2->Presenttime &&
		stricmp(pObj1->Present_file_icon,pObj2->Present_file_icon)==0 &&
		pObj1->bPresentBind == pObj2->bPresentBind &&
		_wcsicmp((WCHAR*)pObj1->Presentdesc,(WCHAR*)pObj2->Presentdesc)==0 &&

		//新增上架时间限制
		pObj1->valid_time.type == pObj2->valid_time.type &&
		pObj1->valid_time.start_time == pObj2->valid_time.start_time &&
		pObj1->valid_time.end_time == pObj2->valid_time.end_time &&
		pObj1->valid_time.param == pObj2->valid_time.param &&

		_wcsicmp((WCHAR*)pObj1->SearchKey,(WCHAR*)pObj2->SearchKey)==0
		
		) return true;
	return false;
}

void CEditTreeCtrl::OnMenuAddTreasure() 
{
	// TODO: Add your command handler code here
	HTREEITEM hSel = GetSelectedItem();
	if(hSel)
	{
		int nImage,nSelImage;
		GetItemImage(hSel,nImage,nSelImage);
		if(nImage != 1) return;
		if(m_bReadOnly) return;
		CTreasureItemDlg dlg;
		if(dlg.DoModal()==IDOK)
		{
			TREASURE_ITEM_VERSION11 *pNewItem = new TREASURE_ITEM_VERSION11;
			memcpy(pNewItem, &dlg.m_Item,sizeof(TREASURE_ITEM_VERSION11));
			pNewItem->tid = m_pTreasureMan->GenTID();
			pNewItem->sub_type = GetItemData(hSel);
			pNewItem->main_type = 0;
			HTREEITEM parent = GetParentItem(hSel);
			int mid = GetItemData(parent);
			pNewItem->main_type = mid;
			m_pTreasureMan->Add(pNewItem);
			HTREEITEM hNew = InsertItem(pNewItem->szName,hSel);
			SetItemData(hNew,pNewItem->tid);
			SetItemImage(hNew,2,2);
			m_bChanged = true;
		}
	}
}

void CEditTreeCtrl::OnMenuDelTreasure() 
{
	// TODO: Add your command handler code here
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		HTREEITEM parent = GetParentItem(hsel);
		if(parent==NULL) return;
		parent = GetParentItem(parent);
		if(parent==NULL) return;
		
		if(IDYES==AfxMessageBox("你确认要删除当前选中的商品吗？",MB_YESNO | MB_ICONQUESTION))
		{
			int id = GetItemData(hsel);
			m_pTreasureMan->DeleteByID(id);
			DeleteItem(hsel);
			m_bChanged = true;
		}
	}
}

void CEditTreeCtrl::OnMenuDelsub() 
{
	// TODO: Add your command handler code here
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		HTREEITEM parent = GetParentItem(hsel);
		if(parent==NULL) return;
	
		if(IDYES==AfxMessageBox("你确定要删除当前选中的子类型吗？",MB_YESNO | MB_ICONQUESTION))
		{
			int id = GetItemData(hsel);
			m_pTreasureMan->DeleteSubType(id);
			DeleteItem(hsel);
			m_bChanged = true;
		}
	}
}

void CEditTreeCtrl::EnumNode(HTREEITEM hParent,HTREEITEM hNewParent)
{
	HTREEITEM hChild = GetChildItem(hParent);
	while(hChild)
	{
		int a,b;
		CString txtChild = GetItemText(hChild);
		DWORD dwChild = GetItemData(hChild);
		GetItemImage(hChild,a,b);
		HTREEITEM hc = InsertItem(txtChild,hNewParent);
		SetItemData(hc,dwChild);
		SetItemImage(hc,a,b);

		EnumNode(hChild,hc);
		hChild = GetNextItem(hChild,TVGN_NEXT);
	}
}

void CEditTreeCtrl::MoveUp()
{
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		int nImage,nSelectImage;
		GetItemImage(hsel,nImage,nSelectImage);
		if(nImage==2)
		{
			//商品节点
			
			HTREEITEM hPrev = GetPrevSiblingItem(hsel);
			if(hPrev!=NULL)
			{
				CString txtPrev = GetItemText(hPrev);
				CString txtSel = GetItemText(hsel);
				DWORD dwPrev = GetItemData(hPrev);
				DWORD dwSel = GetItemData(hsel);

				SetItemText(hPrev, txtSel);
				SetItemText(hsel, txtPrev);

				SetItemData(hPrev, dwSel);
				SetItemData(hsel,dwPrev);

				SelectItem(hPrev);
				m_bChanged = true;
			}
			
		}else if(nImage==1)
		{
			//子类节点
			HTREEITEM hPrev = GetPrevSiblingItem(hsel);
			if(hPrev!=NULL)
			{
				int a,b;
				HTREEITEM hPPrev = GetPrevSiblingItem(hPrev);
				if(hPPrev==NULL) hPPrev = TVI_FIRST;

				CString txtSel = GetItemText(hsel);
				DWORD dwSel = GetItemData(hsel);
				UINT s = GetItemState(hsel,TVIS_EXPANDED);
				GetItemImage(hsel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hsel),hPPrev);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);

				

				DeleteItem(hsel);
				SelectItem(hNew);
				if(s & TVIS_EXPANDED) Expand(hNew,TVE_EXPAND);

				//处理内部数据
				MAIN_TYPE *pMainType = FindMainType(GetItemData(GetParentItem(hNew)));
				for( int i = 0; i < pMainType->listSubType.size(); ++i)
				{
					if(pMainType->listSubType[i].id == dwSel)
					{
						SUB_TYPE sub = pMainType->listSubType[i];
						if(i - 1 >= 0)
						{
							pMainType->listSubType[i] = pMainType->listSubType[i-1];
							pMainType->listSubType[i-1] = sub;
							break;
						}
					}
				}
				m_bChanged = true;
			}
		}else if( nImage == 0)
		{//对NPC主类数据的处理
			
			if(GetParentItem(hsel)==NULL) return;
			
			HTREEITEM hPrev = GetPrevSiblingItem(hsel);
			if(hPrev!=NULL)
			{
				int a,b;
				HTREEITEM hPPrev = GetPrevSiblingItem(hPrev);
				if(hPPrev==NULL) hPPrev = TVI_FIRST;

				CString txtSel = GetItemText(hsel);
				DWORD dwSel = GetItemData(hsel);
				UINT s = GetItemState(hsel,TVIS_EXPANDED);
				GetItemImage(hsel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hsel),hPPrev);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);
				EnumNode(hsel,hNew);
				DeleteItem(hsel);
				SelectItem(hNew);
				if(s & TVIS_EXPANDED) Expand(hNew,TVE_EXPAND);

				//处理内部数据
				abase::vector<MAIN_TYPE *>* pMainTypeList = m_pTreasureMan->GetNpcMainTypeList();
				for( int i = 0; i < pMainTypeList->size(); ++i)
				{
					if((*pMainTypeList)[i]->id == dwSel)
					{
						if(i-1 >= 0)
						{
							MAIN_TYPE *ptemp = (*pMainTypeList)[i - 1];
							(*pMainTypeList)[i - 1] = (*pMainTypeList)[i];
							(*pMainTypeList)[i] = ptemp;
						}
						break;
					}
				}
				m_bChanged = true;
			}
		}
	}
}

void CEditTreeCtrl::MoveDown()
{
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		int nImage,nSelectImage;
		GetItemImage(hsel,nImage,nSelectImage);
		
		if(nImage==2)
		{
			//商品节点
			
			HTREEITEM hNext = GetNextItem(hsel,TVGN_NEXT);
			if(hNext!=NULL)
			{
				CString txtNext = GetItemText(hNext);
				CString txtSel = GetItemText(hsel);
				DWORD dwNext = GetItemData(hNext);
				DWORD dwSel = GetItemData(hsel);

				SetItemText(hNext, txtSel);
				SetItemText(hsel, txtNext);

				SetItemData(hNext, dwSel);
				SetItemData(hsel,dwNext);

				SelectItem(hNext);
				m_bChanged = true;
			}
			
		}else if(nImage==1)
		{
			int a,b;
			//子类节点
			HTREEITEM hNext = GetNextItem(hsel,TVGN_NEXT);
			if(hNext!=NULL)
			{
				//HTREEITEM hNNext = GetNextItem(hNext,TVGN_NEXT);
				//if(hNNext==NULL) hNNext = TVI_LAST;

				CString txtSel = GetItemText(hsel);
				DWORD dwSel = GetItemData(hsel);
				UINT s = GetItemState(hsel,TVIS_EXPANDED);
				GetItemImage(hsel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hNext),hNext);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);

				HTREEITEM hChild = GetChildItem(hsel);
				while(hChild)
				{
					CString txtChild = GetItemText(hChild);
					DWORD dwChild = GetItemData(hChild);
					GetItemImage(hChild,a,b);
					HTREEITEM hc = InsertItem(txtChild,hNew);
					SetItemData(hc,dwChild);
					SetItemImage(hc,a,b);

					hChild = GetNextItem(hChild,TVGN_NEXT);
				}

				DeleteItem(hsel);
				SelectItem(hNew);
				if(s & TVIS_EXPANDED) Expand(hNew,TVE_EXPAND);

				//处理内部数据
				MAIN_TYPE *pMainType = FindMainType(GetItemData(GetParentItem(hNew)));
				for( int i = 0; i < pMainType->listSubType.size(); ++i)
				{
					int testid = pMainType->listSubType[i].id;
					if(pMainType->listSubType[i].id == dwSel)
					{
						SUB_TYPE sub = pMainType->listSubType[i];
						if(i + 1 < pMainType->listSubType.size())
						{
							pMainType->listSubType[i] = pMainType->listSubType[i+1];
							pMainType->listSubType[i+1] = sub;
							break;
						}
					}
				}
				m_bChanged = true;
			}
		}else if(nImage==0)
		{
			if(GetParentItem(hsel)==NULL) return;
			int a,b;
			//子类节点
			HTREEITEM hNext = GetNextItem(hsel,TVGN_NEXT);
			if(hNext!=NULL)
			{
				//HTREEITEM hNNext = GetNextItem(hNext,TVGN_NEXT);
				//if(hNNext==NULL) hNNext = TVI_LAST;

				CString txtSel = GetItemText(hsel);
				DWORD dwSel = GetItemData(hsel);
				UINT s = GetItemState(hsel,TVIS_EXPANDED);
				GetItemImage(hsel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hNext),hNext);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);
				EnumNode(hsel,hNew);
				DeleteItem(hsel);
				SelectItem(hNew);
				if(s & TVIS_EXPANDED) Expand(hNew,TVE_EXPAND);

				abase::vector<MAIN_TYPE *>* pMainTypeList = m_pTreasureMan->GetNpcMainTypeList();
				for( int i = 0; i < pMainTypeList->size(); ++i)
				{
					if((*pMainTypeList)[i]->id == dwSel)
					{
						if(i+1 < pMainTypeList->size())
						{
							MAIN_TYPE *ptemp = (*pMainTypeList)[i + 1];
							(*pMainTypeList)[i + 1] = (*pMainTypeList)[i];
							(*pMainTypeList)[i] = ptemp;
						}
						break;
					}
				}
				m_bChanged = true;
			}
		}
	}
}


bool CEditTreeCtrl::SortTreasure()
{
	abase::vector<TREASURE_ITEM_VERSION11*>* listTreasure =  m_pTreasureMan->GetTreasureList();
	abase::vector<TREASURE_ITEM_VERSION11*> temp;

	HTREEITEM hRoot = GetRootItem();
	while( hRoot)
	{
		EnumTree(hRoot,&temp);
		hRoot = GetNextItem(hRoot,TVGN_NEXT);
	}

	if(listTreasure->size() != temp.size())
		MessageBox("百宝阁商品中存在冗余数据，程序将清除冗余!");
		
	

	listTreasure->clear();
	for( int i = 0; i < temp.size(); ++i)
		listTreasure->push_back(temp[i]);

	return true;
}

void CEditTreeCtrl::EnumTree(HTREEITEM hParent, abase::vector<TREASURE_ITEM_VERSION11*>* pList)
{
	int nImage,nSelImage;
	GetItemImage(hParent,nImage,nSelImage);
	if(nImage==2)
	{
		DWORD dwid = GetItemData(hParent);
		TREASURE_ITEM_VERSION11 *pItem = m_pTreasureMan->GetTreasureByID(dwid);
		pList->push_back(pItem);
	}

	HTREEITEM hChild = GetChildItem(hParent);
	while(hChild)
	{
		EnumTree(hChild,pList);
		hChild = GetNextItem(hChild,TVGN_NEXT);
	}
}

void CEditTreeCtrl::OnMenuAddmain() 
{
	// TODO: Add your command handler code here
	CInputSubTypeNameDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		if(!dlg.m_strSubTypeName.IsEmpty())
		{
			
			HTREEITEM hSel = GetSelectedItem();
			CString text = GetItemText(hSel);
			MAIN_TYPE *pMain = m_pTreasureMan->AddNpcMainType(dlg.m_strSubTypeName);
			CString strName;
			strName.Format("%s(id=%d)",dlg.m_strSubTypeName, pMain->id);
			HTREEITEM hcur = InsertItem(strName,GetSelectedItem());
			
			SetItemData(hcur,pMain->id);
			SetItemImage(hcur,0,0);
			m_bChanged = true;
		}
	}
}

void CEditTreeCtrl::OnMenuDelmain() 
{
	// TODO: Add your command handler code here
	HTREEITEM hsel = GetSelectedItem();
	if(hsel!=NULL)
	{
		HTREEITEM parent = GetParentItem(hsel);
		if(parent==NULL) return;
		int mid = GetItemData(hsel);
		if(GetItemText(parent) != "百宝阁NPC数据") return;
		
		if(IDYES==AfxMessageBox("你确定要删除当前选中的主类型吗？",MB_YESNO | MB_ICONQUESTION))
		{
			m_pTreasureMan->DeleteNpcMainType(mid);
			DeleteItem(hsel);
			m_bChanged = true;
		}
	}
}

//拷贝分支
HTREEITEM CEditTreeCtrl::CopyBranch(HTREEITEM htiBranch, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	HTREEITEM  hChild;
	HTREEITEM  hNewItem = CopyItem( htiBranch,htiNewParent,htiAfter );
	hChild = GetChildItem( htiBranch );

	while( hChild != NULL )
	{
		CopyBranch( hChild,hNewItem,htiAfter );
		hChild = GetNextSiblingItem( hChild );
	}

	return  hNewItem;
}

//拷贝条目
HTREEITEM CEditTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT  tvstruct;
	HTREEITEM        hNewItem;
	CString          sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength ();
	tvstruct.item.pszText    = sText.LockBuffer ();

	//将条目插入到合适的位置
	tvstruct.hParent         = htiNewParent;
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer ();

	//限制拷贝条目数据和条目状态
	SetItemData( hNewItem,GetItemData(hItem) );
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}


void CEditTreeCtrl::OnTimer(UINT nIDEvent) 
{
	//鼠标敏感节点
	if( nIDEvent == m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
		HTREEITEM  trItem = 0;
		UINT  uFlag = 0;
		trItem = HitTest( m_HoverPoint,&uFlag );
		if( trItem && m_bDragging )
		{
			SelectItem( trItem );
			Expand( trItem,TVE_EXPAND );
		}
	}
	//处理拖曳过程中的滚动问题
	else if( nIDEvent == m_nScrollTimerID )
	{
		m_TimerTicks++;
		CPoint  pt;
		GetCursorPos( &pt );
		CRect  rect;
		GetClientRect( &rect );
		ClientToScreen( &rect );

		HTREEITEM  hItem = GetFirstVisibleItem();
		
		if( pt.y < rect.top +10 )
		{
			//向上滚动
			int  slowscroll = 6 - (rect.top + 10 - pt.y )/20;
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			{
				CImageList::DragShowNolock ( false );
				SendMessage( WM_VSCROLL,SB_LINEUP );
				SelectDropTarget( hItem );
				m_hItemDragD = hItem;
				CImageList::DragShowNolock ( true );
			}
		}
		else if( pt.y > rect.bottom - 10 )
		{
			//向下滚动
			int  slowscroll = 6 - (pt.y - rect.bottom + 10)/20;
			if( 0 == (m_TimerTicks % ((slowscroll > 0) ? slowscroll : 1)) )
			{
				CImageList::DragShowNolock ( false );
				SendMessage( WM_VSCROLL,SB_LINEDOWN );
				int  nCount = GetVisibleCount();
				for( int i=0 ; i<nCount-1 ; i++ )
					hItem = GetNextVisibleItem( hItem );
				if( hItem )
					SelectDropTarget( hItem );
				m_hItemDragD = hItem;
				CImageList::DragShowNolock ( true );
			}
		}
	}
	else
		CTreeCtrl::OnTimer(nIDEvent);
}

void CEditTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;
	if(m_bEditting) return;
	//如果是无意拖曳，则放弃操作
	if( (GetTickCount() - m_dwDragStart) < 60 )
		return;

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	
	//只能对第三层的对象拖动
	HTREEITEM hParent = GetParentItem(m_hItemDragS);
	if(NULL==hParent) return;
	if(NULL==GetParentItem(hParent)) return;
	
	m_hItemDragD = NULL;

	//得到用于拖动时显示的图象列表
	m_pDragImage = CreateDragImage( m_hItemDragS );
	if( !m_pDragImage )
		return;

	m_bDragging = true;
	m_pDragImage->BeginDrag ( 0,CPoint(8,8) );
	CPoint  pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter ( this,pt );  //"this"将拖曳动作限制在该窗口
	SetCapture();

	m_nScrollTimerID = SetTimer( 2,40,NULL );
}

void CEditTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	HTREEITEM  hItem;
	UINT       flags;

	//检测鼠标敏感定时器是否存在,如果存在则删除,删除后再定时
	if( m_nHoverTimerID )
	{
		KillTimer( m_nHoverTimerID );
		m_nHoverTimerID = 0;
	}
	m_nHoverTimerID = SetTimer( 1,800,NULL );  //定时为 0.8 秒则自动展开
	m_HoverPoint = point;

	if( m_bDragging )
	{
		CPoint  pt = point;
		CImageList::DragMove( pt );
		
		//鼠标经过时高亮显示
		CImageList::DragShowNolock( false );  //避免鼠标经过时留下难看的痕迹
		if( (hItem = HitTest(point,&flags)) != NULL )
		{
			//DWORD ptr = GetItemData(hItem);
			//CString txt;
			//txt.Format("%s",(char*)ptr);
			
			SelectDropTarget( hItem );
			m_hItemDragD = hItem;
			
			
			//CTalkModifyDlg *pDlg = (CTalkModifyDlg *)GetParent();
			//ASSERT(pDlg);
			bool bNoDrag = false;
			HTREEITEM  htiParent = m_hItemDragD;
			while( (htiParent = GetParentItem(htiParent)) != NULL )
			{
				if( htiParent == m_hItemDragS )
				{
					bNoDrag = true;
				}
			}
			
			//if(pDlg->IsLeaf(txt) || bNoDrag) SetCursor(g_hNoDrag);
			//else SetCursor(LoadCursor(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDC_ARROW)));
		}
		CImageList::DragShowNolock( true );
		
		//当条目被拖曳到左边缘时，将条目放在根下
		CRect  rect;
		GetClientRect( &rect );
		if( point.x < rect.left + 20 )
			m_hItemDragD = NULL;
	}

	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CEditTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	//处理无意拖曳
	m_dwDragStart = GetTickCount();
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CEditTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	
	CTreeCtrl::OnLButtonUp(nFlags, point);

	if( m_bDragging )
	{
		m_bDragging = FALSE;
		CImageList::DragLeave( this );
		CImageList::EndDrag();
		ReleaseCapture();
		delete m_pDragImage;

		SelectDropTarget( NULL );
		
		if( m_hItemDragS == m_hItemDragD )
		{
			KillTimer( m_nScrollTimerID );
			return;
		}

		Expand( m_hItemDragD,TVE_EXPAND );

		HTREEITEM  htiParent = m_hItemDragD;
		while( (htiParent = GetParentItem(htiParent)) != NULL )
		{
			if( htiParent == m_hItemDragS )
			{
				return;
			}
		}
		
	
		if(m_hItemDragD)
		{
			HTREEITEM hParent = GetParentItem(m_hItemDragD);
			if(hParent==NULL) return;
			if(GetParentItem(hParent)!=NULL) return;

			unsigned int id = GetItemData(m_hItemDragS);
			
			unsigned int mainid = 0;
			CString text = GetItemText(hParent);
			for( int i = 0; i < 8; ++i)
			{
				if(strcmp(szMainType[i],text)==0)
				{
					mainid = i;
					break;
				}
			}
			//unsigned int mainid = GetItemData(hParent);
			unsigned int subid = GetItemData(m_hItemDragD);
			TREASURE_ITEM_VERSION11 *pItem = m_pTreasureMan->GetTreasureByID(id);
			if(pItem==NULL) return;
			
			//对数据进行从新设置
			HTREEITEM  htiNew = CopyBranch( m_hItemDragS,m_hItemDragD,TVI_LAST );
			
			//如果shift按下
			SHORT s = GetKeyState(VK_LSHIFT);
			if(s >= 0) 
			{
				pItem->main_type = mainid;
				pItem->sub_type = subid;
				DeleteItem( m_hItemDragS );
			}
			else
			{
				TREASURE_ITEM_VERSION11 *pNewItem = new TREASURE_ITEM_VERSION11;
				memcpy(pNewItem, pItem,sizeof(TREASURE_ITEM_VERSION11));
				pNewItem->tid = m_pTreasureMan->GenTID();
				pNewItem->sub_type = subid;
				pNewItem->main_type = mainid;
				m_pTreasureMan->Add(pNewItem);
				SetItemData(htiNew,pNewItem->tid);
			}
			m_bChanged = true;
			SelectItem( htiNew );
		}
		
		KillTimer( m_nScrollTimerID );
	}
}

//取得下架物品item
HTREEITEM CEditTreeCtrl::GetRejectItem()
{
	HTREEITEM hRoot = GetRootItem();
	if(hRoot==NULL) return NULL;

	HTREEITEM hNext = hRoot;
	while(hNext)
	{
		int main_id = GetItemData(hNext);
		if(main_id==7)
			return GetChildItem(hNext);
		hNext = GetNextVisibleItem(hNext);
	}

	return NULL;
}

void CEditTreeCtrl::OnMenuDoffTreasure() 
{
	// TODO: Add your command handler code here
	HTREEITEM hSel = GetSelectedItem();
	if(hSel==NULL) return;

	unsigned int id = GetItemData(hSel);
	TREASURE_ITEM_VERSION11 *pItem = m_pTreasureMan->GetTreasureByID(id);
	if(pItem==NULL) return;

	HTREEITEM hDs = GetRejectItem();
	if(hDs==NULL) return;

	//对数据进行从新设置
	HTREEITEM  htiNew = CopyBranch(hSel,hDs,TVI_LAST );
	pItem->main_type = 7;
	pItem->sub_type = GetItemData(hDs);
	DeleteItem( hSel );
	m_bChanged = true;
}

void CEditTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	// TODO: Add your message handler code here and/or call default
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if(hItem) SelectItem(hItem);

	CTreeCtrl::OnRButtonDown(nFlags, point);
}
