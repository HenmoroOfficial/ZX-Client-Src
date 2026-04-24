// ListDlg.cpp : implementation file
//
#include <set>
#include "Global.h"
#include "ModelView.h"
#include "ListDlg.h"
#include "Player.h"
#include "MainFrm.h"
#include "..\ZElementClient\EC_RoleTypes.h"
#include "MODELVIEWDataMan.h"
#include "..\ZElementData\DataPathMan.h"
#include "a3d.h"

#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog


CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
{
	m_nameTree.Create(WS_VISIBLE|TVS_HASLINES,CRect(0,0,200,800),&m_tabSel,IDC_TREE_NAMELIST);
	m_pathNameTree.Create(WS_VISIBLE|TVS_HASLINES,CRect(0,0,200,800),&m_tabSel,IDC_TREE_PATHNAME); 

	//{{AFX_DATA_INIT(CListDlg)
	//}}AFX_DATA_INIT
}


void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDlg)
	DDX_Control(pDX, IDC_TREE_PATHNAME, m_pathNameTree);
	DDX_Control(pDX, IDC_TAB_SEL, m_tabSel);
	DDX_Control(pDX, IDC_TREE_NAMELIST, m_nameTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	//{{AFX_MSG_MAP(CListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_CLICK, IDC_TREE_NAMELIST, OnClickTreeObject)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_NAMELIST, OnSelchangedTreeObject)
	ON_NOTIFY(NM_SETFOCUS, IDC_TREE_NAMELIST, OnSetfocusTreeObject)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_SEL, OnSelchangeTabSel)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_PATHNAME, OnSelchangedTreePathname)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDlg message handlers

void CListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	
	CTabCtrl*  pTab = (CTabCtrl*)GetDlgItem(IDC_TAB_SEL);
	CTreeCtrl* pNameTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_NAMELIST);
	CTreeCtrl* pPathNameTree = (CTreeCtrl*)GetDlgItem(IDC_TREE_PATHNAME);
	CRect rect;
	//GetClientRect(&rect);
	GetWindowRect(&rect);
	if(pTab)
	{
		pTab->SetWindowPos(NULL,2,2,rect.Width() - 4,rect.Height() - 4,0);	
		pTab->GetWindowRect(&rect);
		if(pNameTree)
		{
			pNameTree->SetWindowPos(NULL,2,20,rect.Width() - 3,rect.Height()-21,0);	
		}
		if(pPathNameTree)
		{
			pPathNameTree->SetWindowPos(NULL,2,20,rect.Width() - 3,rect.Height()-21,0);
		}
	}
}

BOOL CListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	// TODO: Add extra initialization here
	m_tabSel.InsertItem(0,"名字模式");
	m_tabSel.InsertItem(1,"路径模式");
	if(g_bUsedPath) m_tabSel.SetCurSel(1);


	m_nameTree.ShowWindow(SW_SHOWNORMAL);
	m_pathNameTree.ShowWindow(SW_HIDE);

	LoadList();
		//m_nameTree.SetMultiSelect(false);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CListDlg::LoadList()
{
	m_nameTree.DeleteAllItems();
	m_pathNameTree.DeleteAllItems();
	
	HTREEITEM pNewItem = m_nameTree.InsertItem("角色",NULL,TVI_SORT);
		HTREEITEM pGItem = m_nameTree.InsertItem("男",pNewItem,TVI_SORT);
			HTREEITEM pNewCNameItem = m_nameTree.InsertItem("男（正常体型，人族）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_1);
			pNewCNameItem = m_nameTree.InsertItem("男（大体型，九黎）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_10);
			pNewCNameItem = m_nameTree.InsertItem("男（中体型，天华）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_15);
			pNewCNameItem = m_nameTree.InsertItem("男（小体型，烈山）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_11);
			pNewCNameItem = m_nameTree.InsertItem("男（小体型，怀光）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_13);
		pGItem = m_nameTree.InsertItem("女",pNewItem,TVI_SORT);
			pNewCNameItem = m_nameTree.InsertItem("女（正常体型，人族）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_1);
			pNewCNameItem = m_nameTree.InsertItem("女（大体型，九黎）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_10);
			pNewCNameItem = m_nameTree.InsertItem("女（中体型，天华）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_15);
			pNewCNameItem = m_nameTree.InsertItem("女（小体型，烈山）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_11);
			pNewCNameItem = m_nameTree.InsertItem("女（小体型，怀光）",pGItem,TVI_SORT);
			m_nameTree.SetItemData(pNewCNameItem,SECT_13);
	
	HTREEITEM pNewPathItem = m_pathNameTree.InsertItem("角色",NULL,TVI_SORT);
		pGItem = m_pathNameTree.InsertItem("男",pNewPathItem,TVI_SORT);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\男\\躯干\\男.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_1);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\大体型男\\躯干\\大体型男.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_10);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\中体型男\\躯干\\中体型男.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_15);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\小体型男\\躯干\\小体型男.ecm，烈山",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_11);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\小体型男\\躯干\\小体型男.ecm，怀光",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_13);
		pGItem = m_pathNameTree.InsertItem("女",pNewPathItem,TVI_SORT);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\女\\躯干\\女.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_1);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\大体型女\\躯干\\大体型女.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_10);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\中体型女\\躯干\\中体型女.ecm",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_15);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\小体型女\\躯干\\小体型女.ecm，烈山",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_11);
			pNewCNameItem = m_pathNameTree.InsertItem("Models\\Players\\形象\\小体型女\\躯干\\小体型女.ecm，怀光",pGItem,TVI_SORT);
			m_pathNameTree.SetItemData(pNewCNameItem,SECT_13);

	
	pNewItem = m_nameTree.InsertItem("NPC",NULL,TVI_SORT);
	HTREEITEM pNewNameItem2 = m_nameTree.InsertItem("怪物",NULL,TVI_SORT);
	pNewPathItem = m_pathNameTree.InsertItem("NPC",NULL,TVI_SORT);
	HTREEITEM pNewPathItem2 = m_pathNameTree.InsertItem("怪物",NULL,TVI_SORT);

	if(pNewItem)
	{
		std::set<CString> dataPathSet;
		dataPathSet.clear();
		DWORD dwNum = g_DataMan.GetNPCEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
		CString strName;
		CString strPath,s;
		for(int i = 0; i < dwNum; i++)
		{
			//DATA_TYPE type;
			//DWORD dwid = g_DataMan.get_data_id(ID_SPACE_ESSENCE,i,type);
			//if(type == DT_NPC_ESSENCE)
			{
				NPC_ESSENCE* pData;
				pData = (NPC_ESSENCE*)g_DataMan.GetNPCEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
				if(!pData)continue;
				const char *szPath = g_DataPathMan.GetPathByID(pData->file_model);
				strPath = szPath;
				strName = WC2AS(pData->name);
				if(strName.IsEmpty()||strPath.IsEmpty()) continue;
				//路径重复资源只加载一次
				s = strPath;
				s.MakeLower();
				if(dataPathSet.find(s)!=dataPathSet.end())continue;
				dataPathSet.insert(s);
				//路径重复资源只加载一次
				HTREEITEM temp = m_nameTree.InsertItem(strName,pNewItem,TVI_SORT);
				m_nameTree.SetItemData(temp,pData->file_model);
				HTREEITEM temp2 = m_pathNameTree.InsertItem(strPath,pNewPathItem,TVI_SORT);
				m_pathNameTree.SetItemData(temp2,pData->file_model);
			}
		}
		dwNum = g_DataMan.GetMonsterEssenceNum();//get_data_num(ID_SPACE_ESSENCE);
		dataPathSet.clear();
		for(i = 0; i < dwNum; i++){
//			else if(type == DT_MONSTER_ESSENCE)
			MONSTER_ESSENCE* pData;
			pData = (MONSTER_ESSENCE*)g_DataMan.GetMonsterEssencePtrByIndex(i);//get_data_ptr(dwid,ID_SPACE_ESSENCE,type);
			if(!pData)continue;
			const char *szPath = g_DataPathMan.GetPathByID(pData->file_model);
			strPath = szPath;
			strName = WC2AS(pData->name);
			if(strName.IsEmpty()||strPath.IsEmpty()) continue;
			//重复路径的资源只加载一次
			s = strPath;
			s.MakeLower();
			if(dataPathSet.find(s)!=dataPathSet.end())continue;
			dataPathSet.insert(s);
			//重复路径的资源只加载一次
			HTREEITEM temp = m_nameTree.InsertItem(strName,pNewNameItem2,TVI_SORT);
			m_nameTree.SetItemData(temp,pData->file_model);
			HTREEITEM temp2 = m_pathNameTree.InsertItem(strPath,pNewPathItem2,TVI_SORT);
			m_pathNameTree.SetItemData(temp2,pData->file_model);
		}
	}
}

//alter nameList/pathList
void CListDlg::ChangeList(){
	int sel = m_tabSel.GetCurSel();
	if(sel==0) //g_bUsedPath = false;
	{	
		g_bUsedPath = false;
		m_nameTree.ShowWindow(SW_SHOWNORMAL);
		m_pathNameTree.ShowWindow(SW_HIDE);
	}else //g_bUsedPath = true;
	{
		g_bUsedPath = true;
		m_nameTree.ShowWindow(SW_HIDE);
		m_pathNameTree.ShowWindow(SW_SHOWNORMAL);
	}
}


void CListDlg::OnClickTreeObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CListDlg::OnSelchangedTreeObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	if(!((CMainFrame*)AfxGetApp()->GetMainWnd())->IsCreated()) return;
	CAnimationBar* pABar = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetAnimationBar();
	CCharactorBar* pCBar = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetCharactorBar();
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_NAMELIST);
	HTREEITEM itemp = pNMTreeView->itemNew.hItem;
	HTREEITEM selItem = itemp;
	if(itemp==0) return;

	CString itemText = pCtrl->GetItemText(itemp);
	CString rootText;
	while(itemp)
	{
		rootText = pCtrl->GetItemText(itemp);
		itemp = pCtrl->GetParentItem(itemp);
	}

	if(rootText=="角色")
	{
		int iFemail = -1;
		if( pCtrl->GetItemText(pCtrl->GetParentItem(selItem)) == "女")      iFemail = 1;
		else if( pCtrl->GetItemText(pCtrl->GetParentItem(selItem)) == "男") iFemail = 0;
		if(iFemail >= 0){		
			g_CPlayer = new CPlayer(pABar,pCBar);
			if(g_CPlayer==0) return;
			Aux_ReleaseObject();
			if(g_CPlayer->Load(pCtrl->GetItemData(selItem),iFemail))
			{
				Aux_SetObject(g_CPlayer);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_SHOW);
				if(g_bUseDefaultCamPos) ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetChildView()->SetCamaraPos(g_CPlayer->GetDefaultCamaraPos());
				((CMainFrame*)AfxGetApp()->GetMainWnd())->RecalcLayout(true);
				//更新子窗口
				CPartDlg* pPartdlg = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetPartDlg();
				if (pPartdlg->IsWindowVisible()) {
					pPartdlg->SetPartType(pPartdlg->GetPart(),pPartdlg->GetOldSkin());
				}
			}else {
				delete g_CPlayer;
				g_CPlayer=0;
			}
		}
	}else if(rootText=="NPC" || rootText=="怪物")
	{
		DWORD dwPath = (DWORD)m_nameTree.GetItemData(selItem);
		const char *szPath = g_DataPathMan.GetPathByID(dwPath);
		CString path(szPath);
		if(!path.IsEmpty())
		{
			Aux_ReleaseObject();
			CNpc *pNpc = new CNpc(pABar,0);
			if(pNpc==0) return;
			if(!pNpc->Load(path))
			{
				delete pNpc;
				pNpc = 0;
				return;
			}
			Aux_SetObject(pNpc);
			((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_SHOW);
			if(g_bUseDefaultCamPos) ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetChildView()->SetCamaraPos(pNpc->GetDefaultCamaraPos());
		}
		
		((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetApp()->GetMainWnd())->RecalcLayout(true);	
	}
	
	*pResult = 0;
}

void CListDlg::OnSetfocusTreeObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	//((CMainFrame*)AfxGetApp()->GetMainWnd())->GetChildView()->SetFocus();
	*pResult = 0;
}

void CListDlg::OnSelchangeTabSel(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	BeginWaitCursor();
	ChangeList();
	//LoadList();
	Aux_ReleaseObject();
	((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_HIDE);
	((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_HIDE);
	((CMainFrame*)AfxGetApp()->GetMainWnd())->RecalcLayout(true);

	EndWaitCursor();

	*pResult = 0;
}

void CListDlg::OnSelchangedTreePathname(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(!((CMainFrame*)AfxGetApp()->GetMainWnd())->IsCreated()) return;
	CAnimationBar* pABar = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetAnimationBar();
	CCharactorBar* pCBar = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetCharactorBar();
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_PATHNAME);
	HTREEITEM itemp = pNMTreeView->itemNew.hItem;
	HTREEITEM selItem = itemp;
	if(itemp==0) return;

	CString itemText = pCtrl->GetItemText(itemp);
	CString rootText;
	while(itemp)
	{
		rootText = pCtrl->GetItemText(itemp);
		itemp = pCtrl->GetParentItem(itemp);
	}

	if(rootText=="角色")
	{
		int iFemail = 0;
		if( pCtrl->GetItemText(pCtrl->GetParentItem(selItem)) == "女")      iFemail = 1;
		else if( pCtrl->GetItemText(pCtrl->GetParentItem(selItem)) == "男") iFemail = 0;
		if(iFemail >= 0){
			g_CPlayer = new CPlayer(pABar,pCBar);
			if(g_CPlayer==0) return;
			Aux_ReleaseObject();
			if(g_CPlayer->Load(pCtrl->GetItemData(selItem),iFemail))
			{
				Aux_SetObject(g_CPlayer);
				((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_SHOW);
				if(g_bUseDefaultCamPos) ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetChildView()->SetCamaraPos(g_CPlayer->GetDefaultCamaraPos());
				((CMainFrame*)AfxGetApp()->GetMainWnd())->RecalcLayout(true);
				//更新子窗口
				CPartDlg* pPartdlg = ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetPartDlg();
				if (pPartdlg->IsWindowVisible()) {
					pPartdlg->SetPartType(pPartdlg->GetPart(),pPartdlg->GetOldSkin());
				}
			}else {
				delete g_CPlayer;
				g_CPlayer =NULL;
				return ;
			}
		}
	}else if(rootText=="NPC" || rootText=="怪物")
	{
		
		DWORD dwPath = (DWORD)m_nameTree.GetItemData(selItem);
		const char *szPath = g_DataPathMan.GetPathByID(dwPath);
		CString path(szPath);
		if(!path.IsEmpty())
		{
			CNpc *pNpc = new CNpc(pABar,0);
			if(pNpc==0) return;
			Aux_ReleaseObject();
			if(!pNpc->Load(path))
			{
				delete pNpc;
				pNpc = 0;
				return;
			}
			Aux_SetObject(pNpc);
			((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_SHOW);
			if(g_bUseDefaultCamPos) ((CMainFrame*)(AfxGetApp()->GetMainWnd()))->GetChildView()->SetCamaraPos(pNpc->GetDefaultCamaraPos());
		}
		
		((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndPartDlg.ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetApp()->GetMainWnd())->m_wndCharactorBar.ShowWindow(SW_HIDE);
		((CMainFrame*)AfxGetApp()->GetMainWnd())->RecalcLayout(true);	
	}
	*pResult = 0;
}
