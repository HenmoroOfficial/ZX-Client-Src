// ListDlg.cpp : implementation file
//

#include "stdafx.h"
#include "VssOperation.h"
#include "Global.h"
#include "ElementData.h"
#include "PropertyList.h"
#include "AObject.h"
#include "AVariant.h"
#include "ListDlg.h"
#include "BaseDataTemplate.h"
#include "TemplateDlg.h"
#include "TemplateCreateDlg.h"
#include "TemplateExtDlg.h"
#include "ExtendDataTempl.h"
#include "ElementDataDoc.h"
#include "TreeFlagMan.h"
#include "AFile.h"
#include "FindDlg.h"

#define new A_DEBUG_NEW
#define FD_OPENED 1923

/////////////////////////////////////////////////////////////////////////////
// CListDlg dialog


CListDlg::CListDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CListDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CListDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	bRclick = false;
}


void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CListDlg)
	DDX_Control(pDX, IDC_TREE_SORT_LIST, m_Tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CListDlg, CDialog)
	//{{AFX_MSG_MAP(CListDlg)
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_TREE_SORT_LIST, OnRclickTreeSortList)
	ON_COMMAND(ID_CREATE_OBJECT, OnCreateObject)
	ON_NOTIFY(NM_CLICK, IDC_TREE_PROPERTY, OnClickTreeProperty)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_SORT_LIST, OnDblclkTreeSortList)
	ON_NOTIFY(NM_RDBLCLK, IDC_TREE_PROPERTY, OnRdblclkTreeProperty)
	ON_NOTIFY(NM_RDBLCLK, IDC_TREE_SORT_LIST, OnRdblclkTreeSortList)
	ON_COMMAND(ID_CREATE_OBJECT_EXT, OnCreateObjectExt)
	ON_UPDATE_COMMAND_UI(ID_CREATE_OBJECT_EXT, OnUpdateCreateObjectExt)
	ON_UPDATE_COMMAND_UI(ID_CREATE_OBJECT, OnUpdateCreateObject)
	ON_COMMAND(ID_VSS_CHECK_IN, OnVssCheckIn)
	ON_COMMAND(ID_VSS_CHECK_OUT, OnVssCheckOut)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE_SORT_LIST, OnSelchangedTreeSortList)
	ON_NOTIFY(TVN_SELCHANGING, IDC_TREE_SORT_LIST, OnSelchangingTreeSortList)
	ON_COMMAND(ID_EDIT_TEMPLATE_PROPERTY, OnEditTemplateProperty)
	ON_WM_DESTROY()
	ON_COMMAND(ID_MODIFIED_OBJECT_EXT, OnModifiedObjectExt)
	ON_WM_TIMER()
	ON_COMMAND(ID_VSS_CHECK_OUT_ALL, OnVssCheckOutAll)
	ON_COMMAND(ID_VSS_CHECK_IN_ALL, OnVssCheckInAll)
	ON_NOTIFY(NM_CLICK, IDC_TREE_SORT_LIST, OnClickTreeSortList)
	ON_NOTIFY(TVN_ITEMEXPANDED, IDC_TREE_SORT_LIST, OnItemexpandedTreeSortList)
	ON_COMMAND(ID_VSS_UNDO_CHECK_OUT, OnVssUndoCheckOut)

	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CListDlg message handlers

void CListDlg::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	CMultiTree* pTree = (CMultiTree*)GetDlgItem(IDC_TREE_SORT_LIST);
	CRect rect;
	GetClientRect(&rect);
	if(pTree)
	{
		pTree->SetWindowPos(NULL,1,20,rect.Width() - 2,rect.Height()-21,0);	
	}
}

void CListDlg::DeleteAllItems()
{
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	if(pCtrl) pCtrl->DeleteAllItems();
}

void CListDlg::EnumObjects( CString strPathName , HTREEITEM hTreeItemp, bool bProperty,int iCount,bool bOnlyFirst)
{
	if( iCount <=0 ) return;
	int nLevel = iCount - 1;
	CMultiTree* pCtrl = NULL; 
	if(bProperty) pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_PROPERTY);
	else pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	ASSERT(pCtrl != NULL);

	bool bSkip = false;
	CFileFind finder;
	CString OldPath;
	CString StrExt = strPathName + "\\*.*";
	BOOL bWorking = finder.FindFile(StrExt);
	
	CString extPath = strPathName;
	extPath.Replace(g_szWorkDir,"");
	
	while (bWorking && !bSkip)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			if (finder.GetFileName() == "TaskTemplate") continue;
			CString name = finder.GetFilePath();
			name.Replace(strPathName,"");
			name.Delete(0,1);
			
			HTREEITEM insertAfter = TVI_SORT;
			HTREEITEM hNewItem;
		
			hNewItem = pCtrl->InsertItem(name,hTreeItemp,insertAfter);		
			pCtrl->SetItemData(hNewItem,0);
			if(bOnlyFirst) bSkip = true;

			UpdateItemState(hNewItem, extPath + "\\" + name, name + ".tmpl", -1);
			EnumObjects(finder.GetFilePath(),hNewItem,bProperty,nLevel);
		}
	}
	if(!bSkip) EnumFiles(strPathName,hTreeItemp,bProperty,bOnlyFirst);
	finder.Close();
}

void CListDlg::EnumFiles(CString strPathName, HTREEITEM hTreeItemp, bool bProperty, bool bOnlyFirst)
{
	CMultiTree* pCtrl = NULL;
	if(bProperty) pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_PROPERTY);
	else pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	ASSERT(pCtrl != NULL);
	
	bool bSkip = false;
	CFileFind finder;
	CString msg;
	CString strWildcard;
	strWildcard = strPathName + "\\*.*";
	
	BOOL bWorking = finder.FindFile(strWildcard);
	while (bWorking && !bSkip)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			continue;
		}
		// 是文件.
		msg = finder.GetFileName();
		CString ext = finder.GetFileName();
		CString extPath = strPathName;
		extPath.Replace(g_szWorkDir,"");
		if(ext.Find(".tmpl")==-1 && ext.Find(".ext")==-1) continue;
		CString parentName = pCtrl->GetItemText(hTreeItemp);
		if(strstr(ext,parentName + ".tmpl")) continue;	// 前面可能有版本前缀
		if(strstr(ext,parentName + ".ext")) continue;

		HTREEITEM insertAfter = TVI_SORT;
		HTREEITEM hNewItem;
		hNewItem = pCtrl->InsertItem(ext,hTreeItemp,insertAfter);		
		if(bOnlyFirst) bSkip = true;
		HTREEITEM itemp = hNewItem;
		CString path = msg;
		while(itemp = pCtrl->GetParentItem(itemp))
		{
			CString pathsub = pCtrl->GetItemText(itemp);
			path = pathsub + "\\" + path;	
		}
		path = "BaseData\\" + path;	
		
		int iVersion = TemplVersionState(AString(path));
		if (iVersion==VS_1 && path.Find(".ext")==-1)
			iVersion = VS_OK;
		UpdateItemState(hNewItem, "", path, iVersion);
		msg = finder.GetFilePath();
	}
	finder.Close();
}

BOOL CListDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_ImageList.Create(IDB_LISTTAG, 16, 3, RGB(255, 0, 255));
    
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	ASSERT(pCtrl != NULL);
	pCtrl->SetImageList(&m_ImageList,TVSIL_NORMAL);
	// Sort the tree control's items using my
	// callback procedure.
	CString strWildcard;
	strWildcard = g_szWorkDir;
	strWildcard += "BaseData";
	FreshObjectList(strWildcard,NULL,2);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CListDlg::FreshObjectList(CString strPathName , HTREEITEM hTreeItemp, int iCount)
{
	if( iCount <=0 ) return;
	int nLevel = iCount;
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	ASSERT(pCtrl != NULL);

	bool bSkip = false;
	CFileFind finder;
	CString OldPath;
	CString StrExt = strPathName + "\\*.*";
	BOOL bWorking = finder.FindFile(StrExt);
	
	CString extPath = strPathName;
	extPath.Replace(g_szWorkDir,"");
	bool bExpand = false;
	
	if(g_Record.IsOpen(extPath))
	{
		bExpand = true;
		pCtrl->SetItemData(hTreeItemp,FD_OPENED);
	}else nLevel = nLevel-1;

  	while (bWorking && !bSkip)
	{
		bWorking = finder.FindNextFile();
		
		//是"."或是".." 跳过.
		if (finder.IsDots())
			continue;
		// 是文件夹，继续搜寻.
		if (finder.IsDirectory())
		{
			if (finder.GetFileName() == "TaskTemplate") continue;
			CString name = finder.GetFilePath();
			name.Replace(strPathName,"");
			name.Delete(0,1);
			
			HTREEITEM insertAfter = TVI_SORT;
			HTREEITEM hNewItem;
			
			if(CanShowTemplate(extPath + "\\" + name))
				hNewItem = pCtrl->InsertItem(name,hTreeItemp,insertAfter);
			else
				continue;
			pCtrl->SetItemData(hNewItem,0);
			if(!bExpand && iCount < 2) bSkip = true;
		
			UpdateItemState(hNewItem, extPath + "\\" + name, name + ".tmpl", -1);
			FreshObjectList(finder.GetFilePath(),hNewItem,nLevel);
		}
	}
	if(bExpand)
	{ 
		pCtrl->Expand(hTreeItemp,TVE_EXPAND);
		EnumFiles(strPathName,hTreeItemp,false,false);
	}else if( !bSkip ) EnumFiles(strPathName,hTreeItemp,false,true);
	
	finder.Close();
}

HTREEITEM CListDlg::InsertItem(CTreeCtrl *pTree, HTREEITEM hRoot, CString name)
{
	HTREEITEM hSibling = hRoot;
	CString str = pTree->GetItemText(hSibling);
	if(stricmp(str,name)==0) return hRoot;
	hSibling = pTree->GetChildItem(hRoot);
	while(hSibling)
	{	
		str = pTree->GetItemText(hSibling);
		if(stricmp(str,name)==0)
		{
			return hSibling;
		}
		hSibling = pTree->GetNextSiblingItem(hSibling);
	}
	HTREEITEM hItem = pTree->InsertItem(name, hRoot, TVI_SORT);
	return hItem;
}

void CListDlg::OnRclickTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;
	GetCursorPos(&point);
	POINT pt = {point.x, point.y};
	m_Tree.ScreenToClient(&pt);
	CPoint client(pt.x,pt.y);
	pt.x = point.x;
	pt.y = point.y;

	UINT uFlags;
	HTREEITEM itemp;
	
	if(m_Tree.GetSelectedCount() < 2)
	{
		itemp = m_Tree.HitTest(client,&uFlags);
		if(itemp == NULL) return; 

		bool bSkip = false;
		
		if(m_Tree.GetParentItem(itemp)==NULL)
		{
			if(stricmp(m_Tree.GetItemText(itemp),"附加属性")==0)
			{
				bSkip = true;
			}
		}
		CString ext = m_Tree.GetItemText(itemp);
		if(ext.Find(".ext")!=-1) bSkip = true;

		if(!bSkip)
		{
			CString path = m_Tree.GetItemText(itemp);
			while(itemp = m_Tree.GetParentItem(itemp))
			{
				CString pathsub = m_Tree.GetItemText(itemp);
				path = pathsub + "\\" + path;	
			}
			path = "BaseData\\" + path;
			bool bPass = false;
			for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
			{
				AString strOrgPath = n->first;
				
				if(path.Find(strOrgPath) != -1)
				{
					bPass = true;
				}
			}
			if(!bPass) return;
		}
	}

	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_CREATE_OBJECT);
	
	CMenu* pPopupMenu = Menu.GetSubMenu(0);
	ASSERT(pPopupMenu);
	
	
	UpdateMenu(pPopupMenu);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	
	*pResult = 1; 
}

void CListDlg::OnCreateObject() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString path = pCtrl->GetItemText(itemp);
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}

	BaseDataIDManBase* pIDMan;
	if (path.Left(4) == "配方")
		pIDMan = &g_RecipeIDMan;
	else if (path.Left(6) == "Config")
		pIDMan = &g_ConfigIDMan;
	else
		pIDMan = &g_BaseIDMan;

	path = "BaseData\\" + path;
	
	CTemplateCreateDlg dlg(g_bOverseasEnable); //Modified 2011-04-19.
	dlg.m_strPath = path;
	if(dlg.DoModal()==IDOK)
	{
		dlg.m_strName.TrimLeft();
		dlg.m_strName.TrimRight();

		if(dlg.m_strName.Find("\\") != -1 || dlg.m_strName.IsEmpty())
		{
			MessageBox("模板创建失败,模板名字不能为空或含有回车");
			return;
		}

		BaseDataTempl tmp;
		CString folder = path + "\\" +dlg.m_strName;
		tmp.SetName(LPCTSTR(dlg.m_strName));
		if(!tmp.CreateTempl(folder + "\\" + dlg.m_strName + ".tmpl","", pIDMan, true, dlg.m_ulTemplID)) //Modified 2011-04-19
		{
			MessageBox("创建基本模板失败!");
			return;
		}
		if(0 != CreateDirectory(folder,NULL))
		{
			if (!dlg.m_strSrcTempl.IsEmpty())
			{
				BaseDataTempl src;
				if (src.Load(dlg.m_strSrcTempl) == 0)
				{
					tmp = src;
					src.Release();
				}
			}
			tmp.Save(folder + "\\" + dlg.m_strName + ".tmpl");
			tmp.Release();
#ifdef _MD5_CHECK
			WriteMd5CodeToTemplate(folder + "\\" + dlg.m_strName + ".tmpl");
#endif
		}else 
		{
			CString msg("创建文件夹失败--");
			msg += folder;
			MessageBox(msg);
			return;
		}
		//HTREEITEM hNewItem = pCtrl->InsertItem(dlg.m_strName,oldItem,TVI_SORT);
		//
		HTREEITEM insertAfter = TVI_FIRST;
		HTREEITEM hNewItem;
		HTREEITEM child = pCtrl->GetChildItem(oldItem);
		while(child)
		{
			CString ItemName = pCtrl->GetItemText(child);
			if( dlg.m_strName == "附加属性" )
			{
				insertAfter = TVI_FIRST;
				break;
			}
			else if(stricmp(ItemName, dlg.m_strName) < 0 || "附加属性" == ItemName)
			{
				insertAfter = child;
				child = pCtrl->GetNextItem(child,TVGN_NEXT);
				continue;
			}
			else
				break;
		}
		hNewItem = pCtrl->InsertItem(dlg.m_strName,oldItem,insertAfter);
		pCtrl->SelectItem(hNewItem);
		//
		if(!IsCreateFolder(path)) 
		{
			pCtrl->SetItemImage(hNewItem, FS_FILE_CHECKOUT, FS_FILE_CHECKOUT);
		}else
		{
			pCtrl->SetItemImage(hNewItem, FS_FOLDER_CHECKOUT, FS_FOLDER_CHECKOUT);
		}
		//把文件放到服务器上
		CString strPathName = path +"\\"+ dlg.m_strName + "\\" + dlg.m_strName + ".tmpl";
		g_VSS.SetProjectPath(path);
		g_VSS.CreateProject(dlg.m_strName);
		g_VSS.SetProjectPath(path + "\\" + dlg.m_strName);
		g_VSS.AddFile(strPathName);
		g_VSS.CheckOutFile(strPathName);
		(CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(strPathName);
	}
}

void CListDlg::OnClickTreeProperty(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0;
}

void CListDlg::OnDblclkTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 0; 
}

void CListDlg::OnRdblclkTreeProperty(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	
	*pResult = 0;
}

void CListDlg::OnRdblclkTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CListDlg::OnCreateObjectExt() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	
	bool bExtItem = false;
	HTREEITEM hRoot = pCtrl->GetRootItem();
	while(hRoot)
	{
		CString itemText = pCtrl->GetItemText(hRoot);
		if(stricmp(itemText,"附加属性")==0)
		{
			bExtItem = true;
			break;
		}
		hRoot = pCtrl->GetNextItem(hRoot,TVGN_NEXT);
	}
	if(!bExtItem) 
	{
		MessageBox("创建失败: 没有附加属性文件夹！");
		return;
	}

	CString path = "BaseData";
	
	CTemplateExtDlg dlg;
	dlg.m_strDir = path;
	dlg.m_bOverseasEnable = g_bOverseasEnable; //添加海外ID预留控制变量Added 2011-04-19
	if(dlg.DoModal()==IDOK)
	{
		if(dlg.m_strName.IsEmpty()) 
		{
			MessageBox("模板创建失败,模板名字不能为空");
			return;
		}
		ExtendDataTempl extTmp;
		extTmp.SetName(LPCTSTR(dlg.m_strName));
		path = path + "\\附加属性";
		CString newPath = path;
		path = path + "\\" + dlg.m_strName + ".ext";
		if(!extTmp.CreateTempl(path,dlg.m_strName, &g_ExtBaseIDMan, true, dlg.m_ulTemplID)) //Modified 2011-04-19. 
		{
			MessageBox("创建附加属性失败!");
			return;	
		}
		AString itemName[4] = {dlg.m_strItemName1,dlg.m_strItemName2,dlg.m_strItemName3,dlg.m_strItemName4};
		extTmp.AddItem(AString("模板类型"),"ext_type");
		for(int i = 0; i< dlg.m_nItemNum; i++)
		{
			AString itemType(dlg.m_strItemType[i]);
			extTmp.AddItem(itemName[i],itemType);
		}
		extTmp.Save(path);
#ifdef _MD5_CHECK
		WriteMd5CodeToTemplate(path);
#endif
		extTmp.Release();
		
		CString strNewItemName = dlg.m_strName + ".ext";
		HTREEITEM hNewItem = pCtrl->InsertItem(strNewItemName,hRoot,TVI_SORT);
		pCtrl->SetItemImage(hNewItem, FS_FILE_CHECKOUT, FS_FILE_CHECKOUT);
		pCtrl->SelectItem(hNewItem);

		//把文件放到服务器上
		g_VSS.SetProjectPath("");
		g_VSS.SetProjectPath(newPath);
		g_VSS.AddFile(path);
		g_VSS.CheckOutFile(path);
		(CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path);
	}
}

void CListDlg::OnUpdateCreateObjectExt(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CListDlg::OnUpdateCreateObject(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CListDlg::OnVssCheckIn() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString path;
	CString fileName = pCtrl->GetItemText(itemp);
	
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;
	int num = path.GetLength();
	path = path.Left(num-1);
	
	bool bIsFolder = false;
	if(fileName.Find(".tmpl")==-1 && fileName.Find(".ext")==-1)
	{//不是文件
		path = path + "\\" + fileName;
		fileName = fileName + ".tmpl";
		if(!FileIsExist(AString(path + "\\" + fileName))) return;
		bIsFolder = true;
	}

	int iVersion = VS_OK;
	if (fileName.Find(".ext")==-1)
	{
		iVersion = TemplVersionState(AString(path + "\\" + fileName));
		if (iVersion==VS_1)
		{
			CString strPrefix;
			strPrefix.Format(_T("%x"), g_ulVersion);
			path = strPrefix+path;
		}
	}

	CString strSave = path + "\\" + fileName;
	CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(strSave);

	if (!pDoc)
	{
		((CElementDataApp *)AfxGetApp())->OpenDocumentFile(strSave);
		pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(strSave);
	}

	if(pDoc)
		pDoc->OnSaveDocument(strSave);
	
	g_VSS.SetProjectPath(path);
	g_VSS.CheckInFile(fileName);

	UpdateItemState(oldItem, path, fileName, iVersion, bIsFolder);

	if(pDoc)
	{
		if(pDoc) pDoc->OnCloseDocument();
		((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path + "\\" + fileName);
	}
}

void CListDlg::EnumCheck(HTREEITEM root, bool bCheckOut, bool bAppend)
{
	bool pass = m_Tree.GetItemState(root,TVIS_SELECTED) & TVIS_SELECTED || bAppend;
	if(pass)
	{
		HTREEITEM itemp = root;
		HTREEITEM oldItem = root;
		CString path;
		CString fileName = m_Tree.GetItemText(itemp);
		g_Log.Log(fileName);
		while(itemp = m_Tree.GetParentItem(itemp))
		{
			CString pathsub = m_Tree.GetItemText(itemp);
			path = pathsub + "\\" + path;	
		}
		path = "BaseData\\" + path;
		int num = path.GetLength();
		path = path.Left(num-1);
		
		bool bIsFolder = false;
		if(fileName.Find(".tmpl")==-1 && fileName.Find(".ext")==-1)
		{//不是文件
			path = path + "\\" + fileName;
			fileName = fileName + ".tmpl";
			if(!FileIsExist(AString(path + "\\" + fileName))) goto sub_1;
			bIsFolder = true;
		}
		
		int iVersion = VS_OK;
		if (fileName.Find(".ext")==-1)
		{
			iVersion = TemplVersionState(AString(path + "\\" + fileName));
		}

		if (iVersion!=VS_OK&&iVersion!=VS_1)
		{
		}		
		else if(bCheckOut)
		{
			CheckOut(oldItem, path, fileName);
			CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(path + "\\" + fileName);
			if(pDoc)
			{
				if(pDoc) pDoc->OnCloseDocument();
				((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path + "\\" + fileName);
			}	
		}
		else
		{
			if (iVersion==VS_1)
			{
				CString strPrefix;
				strPrefix.Format(_T("%x"), g_ulVersion);
				path = strPrefix+path;
			}
			CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(path + "\\" + fileName);
			if(pDoc)
				pDoc->OnSaveDocument(path + "\\" + fileName);
		
			g_VSS.SetProjectPath(path);
			g_VSS.CheckInFile(fileName);
			
			UpdateItemState(oldItem, path, fileName, iVersion, bIsFolder);
		}
		
	}
	
	//Child item
sub_1:
	HTREEITEM hChild = m_Tree.GetChildItem(root);
	while (hChild)
	{
		EnumCheck(hChild,bCheckOut,pass);
		hChild = m_Tree.GetNextItem(hChild, TVGN_NEXT);
	}	
}

void CListDlg::UpdateMenu(CMenu *pMenu)
{
// 	pMenu->EnableMenuItem(ID_FIND_BYID, MF_GRAYED);
// 	pMenu->EnableMenuItem(ID_FIND_BYFILE, MF_GRAYED);
// 	pMenu->EnableMenuItem(ID_FIND_NEXT, MF_GRAYED);
	if(m_Tree.GetSelectedCount() >1)
	{
		pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN_ALL,MF_ENABLED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT_ALL,MF_ENABLED);
		pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
		return;
	}else
	{
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN_ALL,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT_ALL,MF_GRAYED);
	}
	
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString itemText = pCtrl->GetItemText(itemp);
	CString path = itemText;

	HTREEITEM hChild = itemp;
	if(NULL!=pCtrl->GetChildItem(hChild))
	{
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN_ALL,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT_ALL,MF_GRAYED);
	}

	if(stricmp("附加属性",path)==0)
	{
		pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_ENABLED);
		pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
		return;
	}else
	{
		pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_GRAYED);
	}
	
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;

	if(path.Find(".tmpl")!=-1 || path.Find(".ext")!=-1)
	{
		pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_GRAYED);
		
		int iVersion = TemplVersionState(AString(path));		
		if (iVersion!=VS_OK&&iVersion!=VS_1)
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
		}
		else if(FileIsReadOnly(AString(path)))
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_ENABLED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
		}else 
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_ENABLED);
			pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_ENABLED);
		}
		pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_ENABLED);
	}else 
	{
		if(FileIsExist(AString(path + "\\" + itemText + ".tmpl")))
		{
			int iVersion = TemplVersionState(AString(path + "\\" + itemText + ".tmpl"));
			AString strFile = AString(path + "\\" + itemText + ".tmpl");
			if (iVersion==VS_1)
			{
				CString strPrefix;
				strPrefix.Format(_T("%x"), g_ulVersion);
				if (FileIsExist(AString(strPrefix + path + "\\" + itemText + ".tmpl")))
					strFile = AString(strPrefix + path + "\\" + itemText + ".tmpl");
			}
			if (iVersion!=VS_OK&&iVersion!=VS_1)
			{
				pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
				pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
				pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
			}
			else if(FileIsReadOnly(strFile))
			{
				pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_ENABLED);
				pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
				pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
			}else 
			{
				pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
				pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_ENABLED);
				pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_ENABLED);
			}
			pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_ENABLED);
		}
		else
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_UNDO_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_GRAYED);
		}
		
		if(!IsHideFiles(path))
		{
			pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_GRAYED);
		}else pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_ENABLED);
	}

	bool bPass = false;
	for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		AString strOrgPath = n->first;
		
		if(stricmp(path,strOrgPath) == 0)
		{
			bPass = true;
		}
	}
	
	/*
	if(bPass)
	{
		bPass = false;
		itemp = oldItem;
		HTREEITEM child = pCtrl->GetChildItem(itemp);
		CString txt = pCtrl->GetItemText(child);
		if(stricmp(txt,"附加属性")==0) bPass = true;
		while(child && !bPass)
		{
			child = pCtrl->GetNextItem(child,TVGN_NEXT);
			if(child)
			{
				txt = pCtrl->GetItemText(child);
				if(stricmp(txt,"附加属性")==0)
				{
					bPass = true;
					break;
				}
			}
		}
	}
	if(!bPass) pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_GRAYED);
	else pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_ENABLED);
	*/

	if(itemText.Find(".ext")!=-1)
	{	if(!FileIsReadOnly(AString(path)))
			pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_ENABLED);
		else pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
	}
	else pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
}

void CListDlg::CheckOut(HTREEITEM item, CString pathName, CString fileName)
{
	if(!FileIsExist(AString(pathName + "\\" + fileName)))
		return;
	
	int iVersion = VS_OK;
	if (fileName.Find(".ext")==-1)
	{		
		iVersion = TemplVersionState(AString(pathName + "\\" + fileName));
		if (iVersion==VS_1)
		{
			CString strPrefix;
			strPrefix.Format(_T("%x"), g_ulVersion);
			AssureDirectory(strPrefix+pathName);
			g_VSS.SetProjectPath(strPrefix+pathName);
			g_VSS.GetFile(strPrefix+pathName + "\\" + fileName);
			if(!FileIsExist(AString(strPrefix+pathName + "\\" + fileName)))
			{
				AFile file;	
				if (file.Open(AString(pathName + "\\" + fileName), AFILE_OPENEXIST | AFILE_TEXT))
				{
					AFile fileNew;
					if (fileNew.Open(AString(strPrefix + pathName + "\\" + fileName), AFILE_CREATENEW | AFILE_TEXT))
					{
						char szBuffer[4098];
						DWORD readLen, writeLen;
						while(file.Read(szBuffer, 4096, &readLen))
						{
							if(readLen==0)
								break;
							fileNew.Write(szBuffer, readLen, &writeLen);
						}
						fileNew.Close();
						pathName = strPrefix+pathName;
						AssureVSSPath(pathName);
						g_VSS.SetProjectPath(pathName);
						g_VSS.AddFile(pathName + "\\" + fileName);
					}
					else
						return;
					file.Close();
				}
				else
					return;				
			}
			else
				pathName = strPrefix+pathName;
		}
	}

	g_VSS.SetProjectPath(pathName);
	g_VSS.CheckOutFile(pathName + "\\" + fileName);
	
	UpdateItemState(item, pathName, fileName, iVersion);
	
	CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(pathName + "\\" + fileName);
	if(pDoc) pDoc->OnCloseDocument();
	((CElementDataApp *)AfxGetApp())->OpenDocumentFile(pathName + "\\" + fileName);
}

void CListDlg::CheckIn(HTREEITEM item, CString pathName, CString fileName)
{

}

void CListDlg::UpdateItemState(HTREEITEM item, CString pathName, CString fileName, int iVersion, bool bIsFolder)
{
	// 附加属性不分版本
	if (pathName.Find(".ext")!=-1 || fileName.Find(".ext")!=-1)
	{
		iVersion = VS_OK;
	}

	// 对目录或不确定版本的模板操作
	if (iVersion<0)
	{
		if(IsFixedItem(pathName))
		{
			m_Tree.SetItemImage(item, FS_FOLDER_FIX, FS_FOLDER_FIX);
			return;
		}
		else if (!FileIsExist(AString(pathName + "\\" + fileName)))
		{
			m_Tree.SetItemImage(item, FS_FOLDER_EMPTY, FS_FOLDER_EMPTY);
			return;
		}
		
		iVersion = TemplVersionState(AString(pathName + "\\" + fileName));
		if (iVersion==VS_1)
		{
			CString strPrefix;
			strPrefix.Format(_T("%x"), g_ulVersion);
			
			if(FileIsExist(AString(strPrefix + pathName + fileName)))
				pathName = strPrefix + pathName;
		}
	}

	if (iVersion==VS_2)
	{
		m_Tree.SetItemImage(item, FS_UNEDITABLE, FS_UNEDITABLE);
	}
	else if(!bIsFolder)	// 不是目录则是文件
	{
		if(FileIsReadOnly(AString(pathName + "\\" + fileName)))
		{
			m_Tree.SetItemImage(item, FS_FILE, FS_FILE);
		}
		else
		{
			m_Tree.SetItemImage(item, FS_FILE_CHECKOUT, FS_FILE_CHECKOUT);
		}
	}
	else if(IsHideFiles(pathName))
	{
		if(FileIsReadOnly(AString(pathName + "\\" + fileName)))
		{
			m_Tree.SetItemImage(item, FS_FOLDER, FS_FOLDER);
		}
		else
		{
			m_Tree.SetItemImage(item, FS_FOLDER_CHECKOUT, FS_FOLDER_CHECKOUT);
		}
	}
	else if (iVersion==VS_1)
	{
		CString strPrefix;
		strPrefix.Format(_T("%x"), g_ulVersion);
		if(pathName.Find(strPrefix)==0 && !FileIsReadOnly(AString(pathName + "\\" + fileName)))
		{
			m_Tree.SetItemImage(item, FS_NE_OF_CHECKOUT, FS_NE_OF_CHECKOUT);
		}
		else
		{
			m_Tree.SetItemImage(item, FS_NE_OF, FS_NE_OF);
		}
	}
	else
	{
		if(FileIsReadOnly(AString(pathName + "\\" + fileName)))
		{
			m_Tree.SetItemImage(item, FS_FILE, FS_FILE);
		}
		else
		{
			m_Tree.SetItemImage(item, FS_FILE_CHECKOUT, FS_FILE_CHECKOUT);
		}
	}
}

void CListDlg::OnVssCheckOut() 
{
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString fileName = pCtrl->GetItemText(itemp);
	CString path;// = fileName;
	
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;
	int num = path.GetLength();
	path = path.Left(num-1);
	
	bool bIsFolder = false;
	int iVersion = -1;
	if(fileName.Find(".tmpl")==-1 && fileName.Find(".ext")==-1)
	{//不是文件
		path = path + "\\" + fileName;
		fileName = fileName + ".tmpl";
	}
	CheckOut(oldItem, path, fileName);
}

bool CListDlg::IsCreateFolder(CString path)
{
	CString str;
	for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		AString strOrgPath = n->first;
		
		if(path.Find(strOrgPath) != -1)
		{
			str = strOrgPath;
			break;
		}
	}
	if(str.IsEmpty()) return false;
	
	CString temp(path);
	temp.Replace(str,"");
	
	int num = 0;
	while(1)
	{
		int index = temp.Find("\\");
		if(index == -1)
			break;
		temp.Delete(0,index + 1);
		num++;
	}
	num = num + 2;

	CString org;
	org.Format("%d.org",num);
	org = str + "\\" + org;
	AFile file;
	if(file.Open(org,AFILE_OPENEXIST))
	{
		file.Close();
		return true;
	}

	return false;
}

bool CListDlg::IsHideFiles(CString path)
{
	CString str;
	ORG_PATHS::iterator n;
	for (n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		AString strOrgPath = n->first;
		
		if(path.Find(strOrgPath) != -1)
		{
			str = strOrgPath;
			break;
		}
	}
	if(str.IsEmpty()) return false;

	// 老版本编辑器，新模板不显示
	if (g_ulVersion==g_ulVersionCurrent)
	{
		AString strOrgPath = path;
		ORG_PATHS::iterator itVersion = _org_versions.find(strOrgPath);
		if (itVersion!=_org_versions.end())
		{
			int iVersion = n->second;
			if (iVersion>g_ulVersionCurrent)
			{
				return true;
			}
		}
	}
	
	CString temp(path);
	temp.Replace(str,"");
	
	int num = 0;
	while(1)
	{
		int index = temp.Find("\\");
		if(index == -1)
			break;
		temp.Delete(0,index + 1);
		num++;
	}
	num = num + 1;

	CString org;
	org.Format("%d.org",num);
	org = str + "\\" + org;
	AFile file;
	if(file.Open(org,AFILE_OPENEXIST))
	{
		file.Close();
		return true;
	}

	return false;
}

bool CListDlg::CanShowTemplate(CString path)
{
	// 老版本编辑器，新模板不显示
	if (g_ulVersion==g_ulVersionCurrent)
	{
		AString strOrgPath = path;
		ORG_PATHS::iterator itVersion = _org_versions.find(strOrgPath);
		if (itVersion!=_org_versions.end())
		{
			int iVersion = itVersion->second;
			if (iVersion>g_ulVersionCurrent)
			{
				return false;
			}
		}
	}
	return true;
}

bool CListDlg::IsFixedItem(CString path)
{
	bool bPass = false;
	for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		AString strOrgPath = n->first;
		
		if(strOrgPath.Find(path) != -1)
		{
			bPass = true;
			break;
		}
	}
	return bPass;
}

void CListDlg::OnSelchangedTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CListDlg::OnSelchangingTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	*pResult = 0;
}

void CListDlg::OnEditTemplateProperty() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	CString itemText = pCtrl->GetItemText(itemp);
	CString path = itemText;
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;
	if(path.Find(".tmpl")!=-1 || path.Find(".ext")!=-1)
	{
		CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path);
	}else
	{
		path = path + "\\" + itemText + ".tmpl";
		if(FileIsExist(AString(path)))
			CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path);
	}
}

void CListDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	UpdateRecord();
}

void CListDlg::UpdateRecord()
{
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	g_Record.GetTreeList(pCtrl);
}



void CListDlg::OnModifiedObjectExt() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	CString fileName = pCtrl->GetItemText(itemp);
	CString path;
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;

	int iVersion = TemplVersionState(AString(path + "\\" + fileName));
	if (iVersion==VS_1)
	{
		CString strPrefix;
		strPrefix.Format(_T("%x"), g_ulVersion);
		path = strPrefix+path;
	}
	path = path + "\\" + fileName;

	CTemplateExtDlg dlg;
	dlg.m_strDir = path;
	dlg.m_bModified = true;
	if(dlg.DoModal()==IDOK)
	{
		CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(path);
		if(pDoc)
		{
			pDoc->OnCloseDocument();
		}
		AString itemName[4] = {dlg.m_strItemName1,dlg.m_strItemName2,dlg.m_strItemName3,dlg.m_strItemName4};
		for(int i = 0; i< dlg.m_nItemNum; i++)
		{
			AString itemType(dlg.m_strItemType[i]);
			dlg.m_extTmp.AddItem(itemName[i],itemType);
		}
		dlg.m_extTmp.Save(path);
#ifdef _MD5_CHECK		
		WriteMd5CodeToTemplate((LPCSTR)path);
#endif
		if(pDoc) ((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path);
	}
	dlg.m_extTmp.Release();
}


void CListDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	CDialog::OnTimer(nIDEvent);
}

BOOL CListDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_TAB)
		return false;
	return CDialog::PreTranslateMessage(pMsg);
}

void CListDlg::OnVssCheckOutAll() 
{
	// TODO: Add your command handler code here
	CTreeItemList selList;
	m_Tree.GetSelectedList(selList);
	POSITION pos = selList.GetHeadPosition();
	while( pos )
	{
		HTREEITEM hitem = selList.GetNext(pos);
		EnumCheck(hitem,true,true);
	}
}

void CListDlg::OnVssCheckInAll() 
{
	// TODO: Add your command handler code here
	CTreeItemList selList;
	m_Tree.GetSelectedList(selList);
	POSITION pos = selList.GetHeadPosition();
	while( pos )
	{
		HTREEITEM hitem = selList.GetNext(pos);
		EnumCheck(hitem,false,true);
	}
}

void CListDlg::OnClickTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	if(m_Tree.GetSelectedCount() >1 || m_Tree.GetSelectedCount()==0) return;
	
#ifdef _DEBUG 
	DWORD lastTime = a_GetTime();
	CString strDebug;
#endif
	
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	POINT pt;
	GetCursorPos(&pt);
	m_Tree.ScreenToClient(&pt);
	HTREEITEM test = m_Tree.HitTest(pt);
	if(test!=itemp) return;

	CString fileName = pCtrl->GetItemText(itemp);

	CString path = fileName;
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;

	if (fileName.Find(".ext")==-1)
	{
		int iVersion = TemplVersionState(AString(path + "\\" + fileName + ".tmpl"));
		if (iVersion==VS_1)
		{
			CString strPrefix;
			strPrefix.Format(_T("%x"), g_ulVersion);
			if(FileIsExist(AString(strPrefix + path + "\\" + fileName + ".tmpl")))
				path = strPrefix+path;
		}
	}
	
#ifdef _DEBUG
	strDebug.Format("Ready time: %d\n", a_GetTime()-lastTime);
	lastTime = a_GetTime();
	TRACE(strDebug);
#endif 
	
	if(path.Find(".tmpl")!=-1 || path.Find(".ext")!=-1)
	{
		CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path);
	}else
	{
		if(!IsFixedItem(path))
		{
			if(!IsHideFiles(path))
				if(FileIsExist(AString(path + "\\" + fileName + ".tmpl")))
				CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path + "\\" + fileName + ".tmpl");
		}
	}

#ifdef _DEBUG
	strDebug.Format("Open total time: %d\n", a_GetTime()-lastTime);
	lastTime = a_GetTime();
	TRACE(strDebug);
#endif 	
	*pResult = 0;
}

void CListDlg::OnItemexpandedTreeSortList(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	// TODO: Add your control notification handler code here
	if(pNMTreeView->action == TVE_EXPAND)
	{
		AfxGetApp()->BeginWaitCursor();

		HTREEITEM hItem = pNMTreeView->itemNew.hItem;
		DWORD data = m_Tree.GetItemData(hItem);
		if(!(data & FD_OPENED))
		{
			//删除原来有的一条
			m_Tree.DeleteItem(m_Tree.GetChildItem(hItem));
			//枚举即将显示的ITEM
			CString path = GetItemPath(hItem);
			EnumObjects(path, hItem,false,1);
			HTREEITEM hChild = m_Tree.GetChildItem(hItem);
			m_Tree.SetItemData(hItem,FD_OPENED);
			while(hChild)
			{
				path = GetItemPath(hChild);
				EnumObjects(path, hChild,false,1,true);
				hChild = m_Tree.GetNextSiblingItem(hChild);
			}
		}
		AfxGetApp()->EndWaitCursor();
	}
	*pResult = 0;
}

CString CListDlg::GetItemPath(HTREEITEM hItem)
{
	if(hItem==0) return "";
	CString itemText = m_Tree.GetItemText(hItem);
	CString path = itemText;
	while(hItem = m_Tree.GetParentItem(hItem))
	{
		CString pathsub = m_Tree.GetItemText(hItem);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;
	path = g_szWorkDir + path;
	return path;
}

void CListDlg::AssureDirectory(CString path)
{
	int index1=0;
	int index2=0;
	while((index2=path.Find('\\', index1))>=0)
	{
		CString strPath = path.Mid(0, index2);
		CreateDirectory(strPath, NULL);
		index1 = index2+1;
	}
	CreateDirectory(path, NULL);
}

void CListDlg::AssureVSSPath(CString path)
{
	int index1=0;
	int index2=0;
	while((index2=path.Find('\\', index1))>=0)
	{
		CString str1 = path.Mid(0, index1);
		CString str2 = path.Mid(index1, index2-index1);
		g_VSS.SetProjectPath(str1);
		g_VSS.CreateProject(str2);
		index1 = index2+1;
	}
	CString str1 = path.Mid(0, index1);
	CString str2 = path.Mid(index1, path.GetLength()-index1);
	g_VSS.SetProjectPath(str1);
	g_VSS.CreateProject(str2);
}

void CListDlg::OnVssUndoCheckOut() 
{
	// TODO: Add your command handler code here
	CMultiTree* pCtrl = (CMultiTree*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString path;
	CString fileName = pCtrl->GetItemText(itemp);
	
	while(itemp = pCtrl->GetParentItem(itemp))
	{
		CString pathsub = pCtrl->GetItemText(itemp);
		path = pathsub + "\\" + path;	
	}
	path = "BaseData\\" + path;
	int num = path.GetLength();
	path = path.Left(num-1);
	
	bool bIsFolder = false;
	if(fileName.Find(".tmpl")==-1 && fileName.Find(".ext")==-1)
	{//不是文件
		path = path + "\\" + fileName;
		fileName = fileName + ".tmpl";
		if(!FileIsExist(AString(path + "\\" + fileName))) return;
		bIsFolder = true;
	}
	
	int iVersion = VS_OK;
	
	if (fileName.Find(".ext")==-1)
	{
		iVersion = TemplVersionState(AString(path + "\\" + fileName));
		if (iVersion==VS_1)
		{
			CString strPrefix;
			strPrefix.Format(_T("%x"), g_ulVersion);
			path = strPrefix+path;
		}
	}

	g_VSS.SetProjectPath(path);
	g_VSS.UndoCheckOut(path + "\\" + fileName);

	UpdateItemState(oldItem, path, fileName, iVersion, bIsFolder);

	CElementDataDoc *pDoc = (CElementDataDoc *)((CElementDataApp*)AfxGetApp())->GetDocument(path + "\\" + fileName);
	if(pDoc)
	{
		if(pDoc) pDoc->OnCloseDocument();
		((CElementDataApp *)AfxGetApp())->OpenDocumentFile(path + "\\" + fileName);
	}
}

// 根据上次的输入来查找下一个
void CListDlg::OnFindNext(const CString& str)
{
	if(str.IsEmpty())
		return;
	HTREEITEM hChild = m_Tree.GetSelectedItem();
	if (hChild == NULL)
		FindTempl(TVI_ROOT, str);
	else
	{
		if (FindTempl(hChild, str))
			return;

		if (FindNext(hChild, str) == NULL)
			FindTempl(TVI_ROOT, str);
	}	
}

HTREEITEM CListDlg::FindTempl(HTREEITEM hParent, const CString& strTempl)
{
	HTREEITEM hChild = m_Tree.GetChildItem(hParent);
	HTREEITEM itemp;
	while(hChild)
	{
		CString path = m_Tree.GetItemText(hChild);
		itemp = hChild;
		while(itemp = m_Tree.GetParentItem(itemp))
		{
			CString pathsub = m_Tree.GetItemText(itemp);
			path = pathsub + "\\" + path;	
		}
		if (path.Find(strTempl)!=-1)
		{
			m_Tree.Select(hChild, TVGN_CARET);
			m_Tree.Expand(hChild, TVE_EXPAND);
			return hChild;
		}

		HTREEITEM h = FindTempl(hChild, strTempl);
		if (h) return h;

		hChild = m_Tree.GetNextSiblingItem(hChild);
	}
	return NULL;
}

HTREEITEM CListDlg::FindTemplByID(HTREEITEM hParent, unsigned long ulID)
{
	HTREEITEM hChild = m_Tree.GetChildItem(hParent);
	HTREEITEM itemp;
	while(hChild)
	{
		CString path = m_Tree.GetItemText(hChild);
		CString path2 = path;
		itemp = hChild;
		while(itemp = m_Tree.GetParentItem(itemp))
		{
			CString pathsub = m_Tree.GetItemText(itemp);
			path = pathsub + "\\" + path;	
		}
		path = "BaseData\\" + path + "\\" + path2 + ".tmpl";
		if (ulID == g_BaseIDMan.GetIDByPath(AString(path)))
		{
			m_Tree.Select(hChild, TVGN_CARET);
			m_Tree.Expand(hChild, TVE_EXPAND);
			return hChild;
		}

		HTREEITEM h = FindTemplByID(hChild, ulID);
		if (h) return h;

		hChild = m_Tree.GetNextSiblingItem(hChild);

	}
	return NULL;
}

HTREEITEM CListDlg::FindNext(HTREEITEM hPrev, const CString& strTempl)
{
	HTREEITEM hNext = m_Tree.GetNextSiblingItem(hPrev);
	HTREEITEM itemp;
	while (hNext)
	{
		CString path = m_Tree.GetItemText(hNext);
		itemp = hNext;
		while(itemp = m_Tree.GetParentItem(itemp))
		{
			CString pathsub = m_Tree.GetItemText(itemp);
			path = pathsub + "\\" + path;	
		}
		if (path.Find(strTempl)!=-1)
		{
			m_Tree.Select(hNext, TVGN_CARET);
			m_Tree.Expand(hNext, TVE_EXPAND);
			return hNext;
		}

		HTREEITEM h = FindTempl(hNext, strTempl);
		if (h) return h;
		hNext = m_Tree.GetNextSiblingItem(hNext);
	}

	HTREEITEM hParent = m_Tree.GetParentItem(hPrev);
	if (hParent)
		return FindNext(hParent, strTempl);

	return NULL;
}
