// DataEditTreeCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "elementdata.h"
#include "DataEditTreeCtrl.h"
#include "GeneralDataManUtil.h"
#include "GeneralDataMan.h"
#include "InputSubTypeNameDlg.h"
#include "GeneralDataItemDlg.h"
#include "BaseDataIDMan.h"
#include "GeneralDataUIWrapper.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDataEditTreeCtrl

CDataEditTreeCtrl::CDataEditTreeCtrl() : m_bDataChanged( false ), m_bDataReadOnly(false),
m_pGeneralDataMan(NULL), m_bDragging(FALSE), m_bEditting(FALSE)
{
}

CDataEditTreeCtrl::~CDataEditTreeCtrl()
{
}


BEGIN_MESSAGE_MAP(CDataEditTreeCtrl, CTreeCtrl)
	//{{AFX_MSG_MAP(CDataEditTreeCtrl)
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(ID_MENU_ADDSUB, OnMenuAddsub)
	ON_COMMAND(ID_MENU_ADD_TREASURE, OnMenuAddDataItem)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_COMMAND(ID_MENU_DEL_TREASURE, OnMenuDelDataItem)
	ON_COMMAND(ID_MENU_DOFF_TREASURE, OnMenuDoffDataItem)
	ON_COMMAND(ID_MENU_DELSUB, OnMenuDelsub)
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_TIMER()
	ON_NOTIFY_REFLECT(TVN_BEGINLABELEDIT, OnBeginlabeledit)
	ON_NOTIFY_REFLECT(TVN_ENDLABELEDIT, OnEndlabeledit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDataEditTreeCtrl message handlers

void CDataEditTreeCtrl::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//
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
	UpdateMenu( pPopupMenu );
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);

	*pResult = 0;
}

void CDataEditTreeCtrl::UpdateMenu(CMenu *pMenu)
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

	if ( GetParentItem(hSel)==NULL && (GetChildItemsCount(hSel) < nSubTypeMaxNum) )
	{
		pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_ENABLED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_MENU_ADDSUB,MF_GRAYED);
	}

	HTREEITEM hParent = GetParentItem(hSel);
	HTREEITEM hPP = GetParentItem(hParent);

	if(hParent!=NULL && hPP==NULL)
	{
		pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_ENABLED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_MENU_DELSUB,MF_GRAYED);
	}
		
	if(hParent!=NULL && hPP==NULL)
	{
		pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_ENABLED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_MENU_ADD_TREASURE,MF_GRAYED);
	}
	

	if(hParent != NULL && hPP != NULL)
	{
		pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_ENABLED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_MENU_DEL_TREASURE,MF_GRAYED);
	}

	if(hParent != NULL && hPP != NULL && GetRejectItem()!=NULL && GetItemData(hPP) != (nVIPAwardMainTypeCount - 1))
	{
		pMenu->EnableMenuItem(ID_MENU_DOFF_TREASURE,MF_ENABLED);
	}
	else
	{
		pMenu->EnableMenuItem(ID_MENU_DOFF_TREASURE,MF_GRAYED);
	}
}

int CDataEditTreeCtrl::GetChildItemsCount(HTREEITEM hParent)
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

void CDataEditTreeCtrl::OnMenuAddsub() 
{
	//
	CInputSubTypeNameDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		if(!dlg.m_strSubTypeName.IsEmpty())
		{
			HTREEITEM hSel = GetSelectedItem();
			int mid = GetItemData(hSel);
			HTREEITEM hcur = InsertItem(dlg.m_strSubTypeName,GetSelectedItem());
			int id = m_pGeneralDataMan->GenSubID();
			SetItemData(hcur,id);
			SetItemImage(hcur,1,1);
			m_pGeneralDataMan->AddGeneralDataSubType( dlg.m_strSubTypeName, mid, id );
			m_bDataChanged = true;
		}
	}
}

void CDataEditTreeCtrl::OnMenuAddDataItem() 
{
	//
	HTREEITEM hSel = GetSelectedItem();
	if(hSel)
	{
		int nImage,nSelImage;
		GetItemImage(hSel,nImage,nSelImage);
		if( nImage != 1 )
		{
			return;
		}
		if( m_bDataReadOnly )
		{
			return;
		}
		//创建新模板
		BaseDataTempl* pNewDataItemTempl = m_pGeneralDataMan->CreateDefaultDataItemTempl( m_strOrgPath );
		if ( !pNewDataItemTempl )
		{
			AfxMessageBox( "创建模板失败！" );
			return;
		}

		//初始化DataUIWrapper
		CGeneralDataUIWrapper generalDataUIWrapper( pNewDataItemTempl );

		//创建并初始化模板对话框
		CGeneralDataItemDlg dlg;
		dlg.Init( &generalDataUIWrapper );//一定要记得先Init

		if ( dlg.DoModal() == IDOK )
		{
			BaseDataTempl* pDataItemTempl = m_pGeneralDataMan->CreateDefaultDataItemTempl( m_strOrgPath, true );
			if ( !pDataItemTempl )
			{
				AfxMessageBox( "创建模板失败！" );
				return;
			}
			*pDataItemTempl = *pNewDataItemTempl;
			pDataItemTempl->SetName( pNewDataItemTempl->GetName() ); //模板的赋值重载函数写得很有局限性，没有包含模板ID和模板名称的赋值操作，需要单独处理
			HTREEITEM hSelParent = GetParentItem(hSel);
			int nMainID = GetItemData(hSelParent);
			int nSubID = GetItemData(hSel);
			m_pGeneralDataMan->AddDataItem( pDataItemTempl, nMainID, nSubID );
			HTREEITEM hNew = InsertItem(pDataItemTempl->GetName(),hSel);
			SetItemData(hNew,pDataItemTempl->GetID());
			SetItemImage(hNew,2,2);
			m_bDataChanged = true;
		}

		//释放资源
		if ( pNewDataItemTempl )
		{
			delete pNewDataItemTempl;
			pNewDataItemTempl = NULL;
		}
	}
}

void CDataEditTreeCtrl::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	//
	HTREEITEM hsel = GetSelectedItem();
	if ( hsel != NULL )
	{
		int nImage, nSelImage;
		GetItemImage(hsel,nImage,nSelImage);
		if ( nImage != 2 )
		{
			return;
		}

		int id = GetItemData(hsel);
		BaseDataTempl *pDataItemTempl = m_pGeneralDataMan->GetDataItemTemplByID( id );
		if ( !pDataItemTempl )
		{
			return;
		} 

		//创建UIWrapper
		CGeneralDataUIWrapper generalDataUIWrapper( pDataItemTempl );

		//创建DataItemDlg对话框
		CGeneralDataItemDlg dlg;
		dlg.Init( &generalDataUIWrapper );//一定要记得先Init
		dlg.SetDataReadOnly( m_bDataReadOnly );
		if ( dlg.DoModal() == IDOK )
		{
			SetItemText(hsel,pDataItemTempl->GetName());
			m_bDataChanged = true;
		}
	}
	
	*pResult = 0;
}

void CDataEditTreeCtrl::OnMenuDelDataItem() 
{
	//
	HTREEITEM hsel = GetSelectedItem();
	if ( hsel != NULL )
	{
		HTREEITEM parent = GetParentItem(hsel);
		if ( parent == NULL )
		{
			return;
		}
		int nSubID = GetItemData( parent );

		parent = GetParentItem(parent);
		if ( parent == NULL )
		{
			return;
		}
		int nMainID = GetItemData( parent );
		
		if ( IDYES == AfxMessageBox("你确认要删除当前选中的物品吗？",MB_YESNO | MB_ICONQUESTION) )
		{
			int id = GetItemData(hsel);
			m_pGeneralDataMan->RemoveDataItem( id, nMainID, nSubID );
			DeleteItem( hsel );
			m_bDataChanged = true;
		}
	}
}

HTREEITEM CDataEditTreeCtrl::GetRejectItem()
{
	HTREEITEM hRoot = GetRootItem();
	if ( NULL == hRoot )
	{
		return NULL;
	}

	HTREEITEM hNext = hRoot;
	while ( hNext )
	{
		int nMainID = GetItemData( hNext );
		if ( nMainID == (nVIPAwardMainTypeCount - 1) )
		{
			return GetChildItem( hNext );
		}
		hNext = GetNextSiblingItem( hNext );
	}

	return NULL;
}

void CDataEditTreeCtrl::OnMenuDoffDataItem() 
{
	//
	HTREEITEM hSel = GetSelectedItem();
	if ( hSel == NULL )
	{
		return;
	}

	//根据所选择项的ID找到具体的模板实例
	unsigned int id = GetItemData(hSel);
	BaseDataTempl *pDataItemTempl = m_pGeneralDataMan->GetDataItemTemplByID( id );
	if ( !pDataItemTempl )
	{
		return;
	}
	
	//取得当前所选择的item的父item
	HTREEITEM parent = GetParentItem(hSel);
	if ( parent == NULL )
	{
		return;
	}
	int nSubID = GetItemData( parent );

	parent = GetParentItem( parent );
	if ( parent == NULL )
	{
		return;
	}
	int nMainID = GetItemData( parent );
	
	//找到下架类型目录的第一个子目录
	HTREEITEM hDoffItem = CDataEditTreeCtrl::GetRejectItem();
	if ( NULL == hDoffItem )
	{
		return;
	}
	int nNewSubID = GetItemData( hDoffItem );

	HTREEITEM hDoffParent = GetParentItem( hDoffItem );
	if ( NULL == hDoffParent )
	{
		return;
	}
	int nNewMainID = GetItemData( hDoffParent );

	//将数据项从当前树形目录拷贝到下架目录下的第一个子目录，并设置MainType中SubType的ID属性
	HTREEITEM  htiNew = CopyBranch(hSel,hDoffItem,TVI_LAST );
	//删除之前类型表中所存的模板ID
	m_pGeneralDataMan->RemoveMainAndSubTypeID( id, nMainID, nSubID );
	//添加被移到下架目录下的物品模板ID
	m_pGeneralDataMan->AddMainAndSubTypeID( id, nNewMainID, nNewSubID );
	//更新该id所对应的物品的位置------〉使得客户端所显示的物品位置和编辑器中显示的位置顺序一致 Added 2012-02-27.
	m_pGeneralDataMan->MoveItemToLastPosByID( id );

	DeleteItem( hSel );
	m_bDataChanged = true;

}

HTREEITEM CDataEditTreeCtrl::CopyBranch(HTREEITEM htiBranch,HTREEITEM htiNewParent,HTREEITEM htiAfter)
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

HTREEITEM CDataEditTreeCtrl::CopyItem(HTREEITEM hItem, HTREEITEM htiNewParent, HTREEITEM htiAfter)
{
	TV_INSERTSTRUCT  tvstruct;
	HTREEITEM        hNewItem;
	CString          sText;

	//得到源条目的信息
	tvstruct.item.hItem = hItem;
	tvstruct.item.mask  = TVIF_CHILDREN|TVIF_HANDLE|TVIF_IMAGE|TVIF_SELECTEDIMAGE;
	GetItem( &tvstruct.item );
	sText = GetItemText( hItem );
	tvstruct.item.cchTextMax = sText.GetLength();
	tvstruct.item.pszText    = sText.LockBuffer();

	//将条目插入到合适的位置
	tvstruct.hParent         = htiNewParent;
	tvstruct.hInsertAfter    = htiAfter;
	tvstruct.item.mask       = TVIF_IMAGE|TVIF_SELECTEDIMAGE|TVIF_TEXT;
	hNewItem = InsertItem( &tvstruct );
	sText.ReleaseBuffer();

	//限制拷贝条目数据和条目状态
	SetItemData( hNewItem,GetItemData(hItem) );
	SetItemState( hNewItem,GetItemState(hItem,TVIS_STATEIMAGEMASK),TVIS_STATEIMAGEMASK);

	return hNewItem;
}

void CDataEditTreeCtrl::OnMenuDelsub() 
{
	//
	HTREEITEM hSel = GetSelectedItem();
	if ( hSel != NULL )
	{
		HTREEITEM parent = GetParentItem( hSel );
		if ( parent == NULL )
		{
			return;
		}
		int nMainID = GetItemData( parent );
	
		if ( IDYES == AfxMessageBox("你确定要删除当前选中的子类型吗？",MB_YESNO | MB_ICONQUESTION) )
		{
			int nSubID = GetItemData( hSel );
			m_pGeneralDataMan->RemoveSubTypeAndDataItem( nMainID, nSubID );
			DeleteItem( hSel );
			m_bDataChanged = true;
		}
	}	
}

void CDataEditTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//保证及时更新所选中数据项
	UINT uFlags;
	HTREEITEM hItem = HitTest(point, &uFlags);
	if ( hItem ) 
	{
		SelectItem(hItem);
	}
	
	CTreeCtrl::OnRButtonDown(nFlags, point);
}

void CDataEditTreeCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//处理无意拖曳
	m_dwDragStart = GetTickCount();
	
	CTreeCtrl::OnLButtonDown(nFlags, point);
}

void CDataEditTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	//
	if( m_bDragging )
	{
		m_bDragging = FALSE;
		CImageList::DragLeave( this );
		CImageList::EndDrag();
		ReleaseCapture();
		if ( m_pDragImage )
		{
			delete m_pDragImage;
			m_pDragImage = NULL;
		}

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
			if ( hParent==NULL )
			{
				return;
			}
			if( GetParentItem(hParent) != NULL )
			{
				return;
			}
			int nNewMainID = GetItemData( hParent );//!!!!!?????!!!!!!
			int nNewSubID = GetItemData(m_hItemDragD);
	
			HTREEITEM hSubDragS = GetParentItem( m_hItemDragS );
			if ( NULL == hSubDragS )
			{
				return;
			}
			int nSubID = GetItemData( hSubDragS );
			HTREEITEM hMainDragS = GetParentItem( hSubDragS );
			if ( NULL == hMainDragS )
			{
				return;
			}
			int nMainID = GetItemData( hMainDragS );

			unsigned int id = GetItemData(m_hItemDragS);
			BaseDataTempl* pDataTempl = m_pGeneralDataMan->GetDataItemTemplByID( id );
			if( !pDataTempl )
			{
				return;
			}
			
			//对数据进行从新设置
			HTREEITEM  htiNew = CopyBranch( m_hItemDragS,m_hItemDragD,TVI_LAST );
			
			//如果shift按下
			SHORT s = GetKeyState(VK_LSHIFT);
			if (s >= 0) 
			{
				//删除之前类型表中所存的模板ID
				m_pGeneralDataMan->RemoveMainAndSubTypeID( id, nMainID, nSubID );
				//添加被移到目标目录下的物品模板ID
				m_pGeneralDataMan->AddMainAndSubTypeID( id, nNewMainID, nNewSubID );
				//更新该id所对应的物品的位置------〉使得客户端所显示的物品位置和编辑器中显示的位置顺序一致 Added 2012-02-27.
				m_pGeneralDataMan->MoveItemToLastPosByID( id );

				DeleteItem( m_hItemDragS );
			}
			else
			{
				BaseDataTempl* pDataItemTempl = m_pGeneralDataMan->CreateDefaultDataItemTempl( m_strOrgPath, true );
				if ( !pDataItemTempl )
				{
					AfxMessageBox( "创建模板失败！" );
					return;
				}
				*pDataItemTempl = *pDataTempl;
				pDataItemTempl->SetName( pDataTempl->GetName() );				
				m_pGeneralDataMan->AddDataItem( pDataItemTempl, nNewMainID, nNewSubID );
				SetItemData(htiNew,pDataItemTempl->GetID());
			}
			m_bDataChanged = true;
			SelectItem( htiNew );
		}
		
		KillTimer( m_nScrollTimerID );
	}	
	
	//
	CTreeCtrl::OnLButtonUp(nFlags, point);
}

void CDataEditTreeCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	//
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
			
			bool bNoDrag = false;
			HTREEITEM  htiParent = m_hItemDragD;
			while( (htiParent = GetParentItem(htiParent)) != NULL )
			{
				if( htiParent == m_hItemDragS )
				{
					bNoDrag = true;
				}
			}
			
		}
		CImageList::DragShowNolock( true );
		
		//当条目被拖曳到左边缘时，将条目放在根下
		CRect  rect;
		GetClientRect( &rect );
		if( point.x < rect.left + 20 )
		{
			m_hItemDragD = NULL;
		}
	}
	
	//
	CTreeCtrl::OnMouseMove(nFlags, point);
}

void CDataEditTreeCtrl::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	//
	if ( m_bEditting )
	{
		return;
	}
	//如果是无意拖曳，则放弃操作
	if( (GetTickCount() - m_dwDragStart) < 60 )
	{
		return;
	}

	m_hItemDragS = pNMTreeView->itemNew.hItem;
	
	//只能对第三层的对象拖动
	HTREEITEM hParent = GetParentItem(m_hItemDragS);
	if(NULL==hParent) return;
	if(NULL==GetParentItem(hParent)) return;
	
	m_hItemDragD = NULL;

	//得到用于拖动时显示的图象列表
	m_pDragImage = CreateDragImage( m_hItemDragS );
	if ( !m_pDragImage )
	{
		return;
	}

	m_bDragging = true;
	m_pDragImage->BeginDrag( 0,CPoint(8,8) );
	CPoint  pt = pNMTreeView->ptDrag;
	ClientToScreen( &pt );
	m_pDragImage->DragEnter( this, pt );  //"this"将拖曳动作限制在该窗口
	SetCapture();

	m_nScrollTimerID = SetTimer( 2,40,NULL );
	
	*pResult = 0;
}

void CDataEditTreeCtrl::OnTimer(UINT nIDEvent) 
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
	{
		CTreeCtrl::OnTimer(nIDEvent);
	}
}

void CDataEditTreeCtrl::MoveUp()
{
	HTREEITEM hSel = GetSelectedItem();
	if ( hSel != NULL )
	{
		int nImage,nSelectImage;
		GetItemImage(hSel, nImage, nSelectImage);
		if ( nImage == 2 )
		{
			//物品节点
			HTREEITEM hPrev = GetPrevSiblingItem(hSel);
			if ( hPrev != NULL )
			{
				CString txtPrev = GetItemText(hPrev);
				CString txtSel = GetItemText(hSel);
				DWORD dwPrev = GetItemData(hPrev);
				DWORD dwSel = GetItemData(hSel);

				SetItemText(hPrev, txtSel);
				SetItemText(hSel, txtPrev);

				SetItemData(hPrev, dwSel);
				SetItemData(hSel,dwPrev);

				SelectItem(hPrev);
				
				//处理数据管理器中的内部数据 Added and modified 2012-02-22.
				HTREEITEM hSubItem = GetParentItem( hSel );
				HTREEITEM hMainItem = GetParentItem( hSubItem );
				int nSubID = GetItemData( hSubItem );
				int nMainID = GetItemData( hMainItem );
				m_pGeneralDataMan->SwapItemPosByID( nMainID, nSubID, dwSel, dwPrev );
				m_bDataChanged = true;
			}
			
		}
		else if ( nImage == 1 )
		{
			//子类节点
			HTREEITEM hPrev = GetPrevSiblingItem(hSel);
			if ( hPrev != NULL )
			{
				int a,b;
				HTREEITEM hPPrev = GetPrevSiblingItem(hPrev);
				if ( hPPrev == NULL )
				{
					hPPrev = TVI_FIRST;
				}
				int nPrevSubID = GetItemData( hPrev );
				int nMainID = GetItemData( GetParentItem(hSel) );

				CString txtSel = GetItemText(hSel);
				DWORD dwSel = GetItemData(hSel);
				UINT s = GetItemState(hSel,TVIS_EXPANDED);
				GetItemImage(hSel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hSel),hPPrev);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);

				HTREEITEM hChild = GetChildItem(hSel);
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

				DeleteItem(hSel);
				SelectItem(hNew);
				if (s & TVIS_EXPANDED)
				{
					Expand(hNew,TVE_EXPAND);
				}

				//处理内部数据
				m_pGeneralDataMan->SwapSubType( nMainID, dwSel, nPrevSubID );

				m_bDataChanged = true;
			}
		}
	}
}

void CDataEditTreeCtrl::MoveDown()
{
	HTREEITEM hSel = GetSelectedItem();
	if ( hSel != NULL )
	{
		int nImage,nSelectImage;
		GetItemImage(hSel, nImage, nSelectImage);
		if ( nImage == 2 )
		{
			//物品节点
			HTREEITEM hNext = GetNextSiblingItem(hSel);
			if ( hNext != NULL )
			{
				CString txtNext = GetItemText(hNext);
				CString txtSel = GetItemText(hSel);
				DWORD dwNext = GetItemData(hNext);
				DWORD dwSel = GetItemData(hSel);

				SetItemText(hNext, txtSel);
				SetItemText(hSel, txtNext);

				SetItemData(hNext, dwSel);
				SetItemData(hSel,dwNext);

				SelectItem(hNext);

				//处理数据管理器中的内部数据 Added and modified 2012-02-22.
				HTREEITEM hSubItem = GetParentItem( hSel );
				HTREEITEM hMainItem = GetParentItem( hSubItem );
				int nSubID = GetItemData( hSubItem );
				int nMainID = GetItemData( hMainItem );
				m_pGeneralDataMan->SwapItemPosByID( nMainID, nSubID, dwSel, dwNext );

				m_bDataChanged = true;
			}
			
		}
		else if ( nImage == 1 )
		{
			//子类节点
			HTREEITEM hNext = GetNextSiblingItem(hSel);
			if ( hNext != NULL )
			{
				int a,b;
//				HTREEITEM hPNext = GetPrevSiblingItem(hPrev);
//				if(hPPrev==NULL) hPPrev = TVI_FIRST;
				int nNextSubID = GetItemData( hNext );
				int nMainID = GetItemData( GetParentItem(hSel) );

				CString txtSel = GetItemText(hSel);
				DWORD dwSel = GetItemData(hSel);
				UINT s = GetItemState(hSel,TVIS_EXPANDED);
				GetItemImage(hSel,a,b);
				HTREEITEM hNew = InsertItem(txtSel,GetParentItem(hSel),hNext);
				SetItemData(hNew, dwSel);
				SetItemImage(hNew,a,b);

				HTREEITEM hChild = GetChildItem(hSel);
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

				DeleteItem(hSel);
				SelectItem(hNew);
				if (s & TVIS_EXPANDED)
				{
					Expand(hNew,TVE_EXPAND);
				}

				//处理内部数据
				m_pGeneralDataMan->SwapSubType( nMainID, dwSel, nNextSubID );

				m_bDataChanged = true;
			}
		}
	}
}



void CDataEditTreeCtrl::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( 0 == pTVDispInfo->item.lParam )
	{
		*pResult = 1;
	}
	else
	{
		*pResult = 0;
	}
}

void CDataEditTreeCtrl::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	if ( pTVDispInfo->item.pszText != NULL ) //判断是否已经取消取消编辑
	{
		int nImage,nSelImage;
		GetItemImage(pTVDispInfo->item.hItem,nImage,nSelImage);
		if ( 1 == nImage )
		{
			HTREEITEM hParent = GetParentItem(pTVDispInfo->item.hItem);
			if ( hParent == NULL )
			{
				return;
			}
			SetItemText(pTVDispInfo->item.hItem,pTVDispInfo->item.pszText);
			//更新改子类型的名称
			m_pGeneralDataMan->SetSubTypeName(GetItemData(hParent), GetItemData(pTVDispInfo->item.hItem),pTVDispInfo->item.pszText);
			m_bDataChanged = true;
		}
	}
	
	*pResult = 0;
}
