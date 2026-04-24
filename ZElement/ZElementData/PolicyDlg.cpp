// PolicyDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "Global.h"
#include "ElementData.h"
#include "PolicyDlg.h"
#include "PolicyTriggerDlg.h"
#include "BaseDataIDMan.h"
#include "PolicyCreateDlg.h"
#include "Policy.h"
#include "VssOperation.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString strOperationList[];
/////////////////////////////////////////////////////////////////////////////
// CPolicyDlg dialog


CPolicyDlg::CPolicyDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPolicyDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CPolicyDlg)
	m_fHp = 0.0f;
	m_bSkipKillPlayer = FALSE;
	m_bSkipRandom = FALSE;
	m_bSkipStartAttack = FALSE;
	m_bSkipBorn = FALSE;
	m_bSkipDied = FALSE;
	//}}AFX_DATA_INIT
	m_pCurrentPolicy = NULL;
	m_bReadOnly = true;
	m_bStartTest =  false;
	m_bPathEndPoint = false;
	m_bAttackBySkill = false;
}


void CPolicyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPolicyDlg)
	DDX_Control(pDX, IDC_EDIT_RUN_LOG, m_editRunLog);
	DDX_Control(pDX, IDC_LIST_TRIGGER, m_listTrigger);
	DDX_Control(pDX, IDC_TREE_POLICY, m_Tree);
	DDX_Text(pDX, IDC_EDIT_HP, m_fHp);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPolicyDlg, CDialog)
	//{{AFX_MSG_MAP(CPolicyDlg)
	ON_NOTIFY(NM_RCLICK, IDC_TREE_POLICY, OnRclickTreePolicy)
	ON_COMMAND(ID_CREATE_POLICY, OnCreatePolicy)
	ON_COMMAND(ID_POLICY_CHECK_IN, OnPolicyCheckIn)
	ON_COMMAND(ID_POLICY_CHECK_OUT, OnPolicyCheckOut)
	ON_BN_CLICKED(IDC_BUTTON_DEL_TRIGGER, OnButtonDelTrigger)
	ON_BN_CLICKED(IDC_BUTTON_ADD_TRIGGER, OnButtonAddTrigger)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_UP, OnButtonMoveUp)
	ON_BN_CLICKED(IDC_BUTTON_MOVE_DOWN, OnButtonMoveDown)
	ON_BN_CLICKED(IDC_BUTTON_POLICY_SAVE, OnButtonPolicySave)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE_POLICY, OnDblclkTreePolicy)
	ON_LBN_DBLCLK(IDC_LIST_TRIGGER, OnDblclkListTrigger)
	ON_BN_CLICKED(IDC_BUTTON_RUN_TRIGGER, OnButtonRunTrigger)
	ON_BN_CLICKED(IDC_BUTTON_STOP_TRIGGER, OnButtonStopTrigger)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_EVENT_RANDOM, OnButtonEventRandom)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_KILLPLAYER, OnButtonEventKillplayer)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_ATTACKSTART, OnButtonEventAttackstart)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_BORN, OnButtonEventBorn)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_DIED, OnButtonEventDied)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_END_POINT, OnButtonEventEndPoint)
	ON_BN_CLICKED(IDC_BUTTON_EVENT_ATTACK_SKILL, OnButtonEventAttackSkill)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPolicyDlg message handlers
bool CPolicyDlg::LoadIDTemplate()
{
	g_PolicyIDMan.Release();
	if (g_PolicyIDMan.Load("PolicyData\\PolicyID.dat") != 0)
	{
		AfxMessageBox("打开TPolicyData\\PolicyID.dat文件失败", MB_ICONSTOP);
		return false;
	}
	return true;
}

void CPolicyDlg::EnumFiles(CString strPathName, HTREEITEM hTreeItemp)
{
	CTreeCtrl* pCtrl = NULL;
	pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_POLICY);
	ASSERT(pCtrl != NULL);
	
	CFileFind finder;
	CString strWildcard;
	strWildcard = strPathName + "\\*.ply";
	
	BOOL bWorking = finder.FindFile(strWildcard);
	int n = 0;
	while (bWorking)
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
		CString ext = finder.GetFileName();
		CString path = finder.GetFilePath();
		ext.Replace(g_szWorkDir,"");
		
		HTREEITEM insertAfter = TVI_FIRST;
		HTREEITEM hNewItem;
		
		hNewItem = pCtrl->InsertItem(ext,hTreeItemp,insertAfter);		
		
		if(FileIsReadOnly(AString(path)))
		{
			pCtrl->SetItemImage(hNewItem,4,4);
		}else
		{
			pCtrl->SetItemImage(hNewItem,5,5);
		}
	}
	finder.Close();
}

BOOL CPolicyDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	// TODO: Add extra initialization here
	m_ImageList.Create(IDB_LISTTAG, 16, 3, RGB(255, 0, 255));
    m_Tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	
	HTREEITEM insertAfter = TVI_FIRST;
	HTREEITEM hNewItem;
	hNewItem = m_Tree.InsertItem("怪物策略",NULL,insertAfter);
	m_Tree.SetItemImage(hNewItem,1,0);
	CString strPath(g_szWorkDir);
	strPath += "\\PolicyData";
	EnumFiles(strPath, hNewItem);
	LoadIDTemplate();
	m_editRunLog.SetLimitText(1024*100);

	SetTimer(0,1000,NULL);

	m_Tree.SetMultiSelect(FALSE);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPolicyDlg::OnRclickTreePolicy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	CPoint point;
	GetCursorPos(&point);
	POINT pt = {point.x, point.y};
	m_Tree.ScreenToClient(&pt);
	CPoint client(pt.x,pt.y);
	pt.x = point.x;
	pt.y = point.y;
	
	CMenu Menu;
	Menu.LoadMenu(IDR_MENU_POLICY);
	
	CMenu* pPopupMenu = Menu.GetSubMenu(0);
	ASSERT(pPopupMenu);
	
	
	UpdateMenu(pPopupMenu);
	pPopupMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	*pResult = 0;
}

void CPolicyDlg::UpdateMenu(CMenu *pMenu)
{
	/*
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_SORT_LIST);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	HTREEITEM oldItem = itemp;
	CString itemText = pCtrl->GetItemText(itemp);
	CString path = itemText;
	if(stricmp("附加属性",path)==0)
	{
		pMenu->EnableMenuItem(ID_CREATE_OBJECT_EXT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_CREATE_OBJECT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
		pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
		pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_GRAYED);
		pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
		return;
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
		
		if(FileIsReadOnly(AString(path)))
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_ENABLED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);	
		}else 
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_ENABLED);
		}
		pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_ENABLED);
	}else 
	{
		if(FileIsExist(AString(path + "\\" + itemText + ".tmpl")))
		{
			if(FileIsReadOnly(AString(path + "\\" + itemText + ".tmpl")))
			{
				pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_ENABLED);
				pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);	
			}else 
			{
				pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
				pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_ENABLED);
			}
			pMenu->EnableMenuItem(ID_EDIT_TEMPLATE_PROPERTY,MF_ENABLED);
		}else
		{
			pMenu->EnableMenuItem(ID_VSS_CHECK_OUT,MF_GRAYED);
			pMenu->EnableMenuItem(ID_VSS_CHECK_IN,MF_GRAYED);
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

	if(itemText.Find(".ext")!=-1)
	{	if(!FileIsReadOnly(AString(path)))
			pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_ENABLED);
		else pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
	}
	else pMenu->EnableMenuItem(ID_MODIFIED_OBJECT_EXT,MF_GRAYED);
	*/
}

void CPolicyDlg::OnCreatePolicy() 
{
	// TODO: Add your command handler code here
	CPolicyCreateDlg dlg;
	CPolicyData mPolicyData;
	dlg.m_bOverseasEnable = g_bOverseasEnable;
	if(IDOK == dlg.DoModal())
	{
		CString pathName;
		int k = 0;
		pathName.Format("PolicyData\\%s.ply",dlg.m_szPolicyName);
		
		//添加与海外ID预留相关控制代码 Added 2011-04-20.
		if ( g_bOverseasEnable )
		{
			m_nPolicyID = g_PolicyIDMan.CreateIDExt( dlg.m_ulOverseasPolicyID, AString(pathName) ); //手动输入ID而非程序自动生成
			//再判断下，好定位创建模板时的错误信息
			if ( !m_nPolicyID )
			{
				AfxMessageBox( "创建失败，输入模板名字错误或者输入模板的ID错误!" );
				return;
			}
		}
		else
		{
			m_nPolicyID = g_PolicyIDMan.CreateID(AString(pathName));
		}
		//Added end.

		if(m_nPolicyID==0) 
		{
			MessageBox("创建失败，可能已经有相同的策略名字存在!");
			return;
		}
		mPolicyData.SetID(m_nPolicyID);
		if(!mPolicyData.Save(pathName.GetBuffer(0)))
		{
			MessageBox("CPolicyDlg::OnCreatePolicy(),保存文件失败!");
			return;
		}
		pathName.ReleaseBuffer();
		g_VSS.SetProjectPath("PolicyData");
		g_VSS.AddFile(pathName);
		g_VSS.CheckInFile(pathName);
		pathName.Format("%s.ply",dlg.m_szPolicyName);
		HTREEITEM hNewItem;
		hNewItem = m_Tree.InsertItem(pathName,m_Tree.GetRootItem(),TVI_FIRST);
		pathName.Format("%sPolicyData\\%s.ply",g_szWorkDir,dlg.m_szPolicyName);
		if(FileIsReadOnly(AString(pathName)))
		{
			m_Tree.SetItemImage(hNewItem,4,4);
		}else
		{
			m_Tree.SetItemImage(hNewItem,5,5);
		}
		FreshTriggerList();
	}
	mPolicyData.Release();
}

void CPolicyDlg::OnPolicyCheckIn() 
{
	// TODO: Add your command handler code here
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_POLICY);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	CString fileName = pCtrl->GetItemText(itemp);
	CString pathName = "PolicyData\\" + fileName;
	if(-1==fileName.Find(".ply")) return;
	
	if(m_strCurrentPathName==pathName && m_pCurrentPolicy)
	{
		if(!m_bReadOnly && g_bPolicyModifyed) 
		{
			if(IDYES==AfxMessageBox("策略数据已经改变，你需要存盘吗?",MB_YESNO|MB_ICONQUESTION))
			{
				m_pCurrentPolicy->Save(m_strCurrentPathName.GetBuffer(0));
				g_bPolicyModifyed = false;
			}
		}
	}
	
	g_VSS.SetProjectPath("PolicyData");
	g_VSS.CheckInFile(fileName);
	fileName = "PolicyData\\" + fileName;
	if(FileIsReadOnly(AString(fileName)))
	{
		m_bReadOnly = true;
		pCtrl->SetItemImage(itemp,4,4);
	}else
	{
		pCtrl->SetItemImage(itemp,5,5);
	}	
	
}

void CPolicyDlg::OnPolicyCheckOut() 
{
	// TODO: Add your command handler code here
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_POLICY);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	CString fileName = pCtrl->GetItemText(itemp);
	if(-1==fileName.Find(".ply")) return;
	
	fileName = "PolicyData\\" + fileName;
	g_VSS.SetProjectPath("PolicyData");
	g_VSS.CheckOutFile(fileName);
	if(FileIsReadOnly(AString(fileName)))
	{
		pCtrl->SetItemImage(itemp,4,4);
	}else
	{
		pCtrl->SetItemImage(itemp,5,5);
	}	
}

void CPolicyDlg::OnButtonDelTrigger() 
{
	// TODO: Add your control notification handler code here
	OnButtonStopTrigger();
	if(m_pCurrentPolicy==NULL || m_bStartTest ) return;
	int sel = m_listTrigger.GetCurSel();
	if(sel == -1) return;
	DWORD id = m_listTrigger.GetItemData(sel);
	for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id==pData->GetID())
		{
			if(IDYES==AfxMessageBox("你确定要删除当前触发器吗?",MB_YESNO|MB_ICONQUESTION))
			{
				m_pCurrentPolicy->DelTriggerPtr(i);
				//这个本来是个比较费时的操作，不过对于数据量很少的策略来说是可以不计较的
				//关联的触发器也是需要删除的
				DelRedundancy();
			}
			break;
		}
	}
	FreshTriggerList();
	g_bPolicyModifyed =  true;
}

void CPolicyDlg::OnButtonAddTrigger() 
{
	// TODO: Add your control notification handler code here
	OnButtonStopTrigger();
	if(m_pCurrentPolicy==NULL || m_bStartTest) return;

	CPolicyTriggerDlg dlg;
	dlg.m_pCurrentPolicy = m_pCurrentPolicy;
	if(IDOK==dlg.DoModal())
	{
		if(dlg.m_pTriggerData)
		{
			dlg.m_pTriggerData->SetRunStatus(false);
			m_pCurrentPolicy->AddTriggerPtr(dlg.m_pTriggerData);
		}
		FreshTriggerList();
		g_bPolicyModifyed = true;
	}
	//上面操作可能产生多余的触发器，所以必须做清理操作
	DelRedundancy();
}

void CPolicyDlg::OnButtonMoveUp() 
{
	OnButtonStopTrigger();
	if(m_pCurrentPolicy==NULL || m_bStartTest) return;
	// TODO: Add your control notification handler code here
	int sel = m_listTrigger.GetCurSel();
	if(sel == -1 || sel == 0) return;
	
	void *pData1, *pData2;
	DWORD id1 = m_listTrigger.GetItemData(sel);
	DWORD id2 = m_listTrigger.GetItemData(sel-1);
	
	int idx1,idx2;
	int i;
	for( i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id1==pData->GetID())
		{
			pData1 = m_pCurrentPolicy->GetTriggerPtr(i);
			idx1 = i;
			break;
		}
	}
	for( i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id2==pData->GetID())
		{
			pData2 = m_pCurrentPolicy->GetTriggerPtr(i);
			idx2 = i;
			break;
		}
	}

	m_pCurrentPolicy->SetTriggerPtr(idx1,(CTriggerData*)pData2);
	m_pCurrentPolicy->SetTriggerPtr(idx2,(CTriggerData*)pData1);
	FreshTriggerList();
	m_listTrigger.SetCurSel(sel-1);
	g_bPolicyModifyed=  true;
}

void CPolicyDlg::OnButtonMoveDown() 
{
	OnButtonStopTrigger();
	if(m_pCurrentPolicy==NULL || m_bStartTest) return;
	// TODO: Add your control notification handler code here
	int sel = m_listTrigger.GetCurSel();
	if(sel == -1 || sel > m_listTrigger.GetCount()-2) return;
	void *pData1, *pData2;
	DWORD id1 = m_listTrigger.GetItemData(sel);
	DWORD id2 = m_listTrigger.GetItemData(sel+1);
	
	int idx1,idx2;
	int i;
	for( i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id1==pData->GetID())
		{
			pData1 = m_pCurrentPolicy->GetTriggerPtr(i);
			idx1 = i;
			break;
		}
	}
	for( i = 0; m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id2==pData->GetID())
		{
			pData2 = m_pCurrentPolicy->GetTriggerPtr(i);
			idx2 = i;
			break;
		}
	}

	m_pCurrentPolicy->SetTriggerPtr(idx1,(CTriggerData*)pData2);
	m_pCurrentPolicy->SetTriggerPtr(idx2,(CTriggerData*)pData1);
	FreshTriggerList();
	m_listTrigger.SetCurSel(sel+1);
	g_bPolicyModifyed =  true;
}

void CPolicyDlg::FreshTriggerList()
{
	if(m_pCurrentPolicy==NULL) return;
	int count = m_listTrigger.GetCount();
	int sel =m_listTrigger.GetCurSel();
	int i;
	for( i = 0; i < count; i++ )
		m_listTrigger.DeleteString(0);

	int n = m_pCurrentPolicy->GetTriggerPtrNum();
	for( i = 0; i < n; ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(!pData->IsRun()) 
		{
			int index = m_listTrigger.AddString(pData->GetName());
			m_listTrigger.SetItemData(index,pData->GetID());
		}
	}

	if(sel!=-1 && sel < m_listTrigger.GetCount()) m_listTrigger.SetCurSel(sel);
}

void CPolicyDlg::OnButtonPolicySave() 
{
	// TODO: Add your control notification handler code here
	if(m_pCurrentPolicy) 
	{
		if(!m_bReadOnly) 
		{
			m_pCurrentPolicy->Save(m_strCurrentPathName.GetBuffer(0));
			m_strCurrentPathName.ReleaseBuffer();
		}
	}
	g_bPolicyModifyed = false;
}

void CPolicyDlg::OnDblclkTreePolicy(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	OnButtonStopTrigger();
	
	CWnd *pWnd = GetDlgItem(IDC_STATIC_POLICYNAME);
	CTreeCtrl* pCtrl = (CTreeCtrl*) GetDlgItem(IDC_TREE_POLICY);
	HTREEITEM itemp = pCtrl->GetSelectedItem();
	CString fileName = pCtrl->GetItemText(itemp);
	if(-1==fileName.Find(".ply")) return;
	fileName = "PolicyData\\"+fileName;
	if(m_pCurrentPolicy) 
	{
		if(m_strCurrentPathName==fileName) return;
		
		if(!m_bReadOnly && g_bPolicyModifyed) 
		{
			if(IDYES==AfxMessageBox("策略数据已经改变，你需要存盘吗?",MB_YESNO|MB_ICONQUESTION))
			{
				m_pCurrentPolicy->Save(m_strCurrentPathName.GetBuffer(0));
				m_strCurrentPathName.ReleaseBuffer();
			}
		}
		m_pCurrentPolicy->Release();
		delete m_pCurrentPolicy;
	}
	
	g_bPolicyModifyed = false;
	if(FileIsReadOnly(AString(fileName)))
	{
		if(AfxMessageBox("文件没有做Check out,修改是无效的!你要继续打开吗",MB_YESNO|MB_ICONQUESTION)==IDNO)
		{
			m_pCurrentPolicy = NULL;
			return;
		}
		m_bReadOnly = true;
	}else
	{
		m_bReadOnly = false;
	}	
	

	m_pCurrentPolicy = new CPolicyData;
	if(!m_pCurrentPolicy->Load(fileName.GetBuffer(0)))
	{
		delete m_pCurrentPolicy;
		m_pCurrentPolicy = NULL;
		pWnd->SetWindowText("空文档");
		MessageBox("打开文件失败!");
		return;
	}

	CString strID;
	if(m_pCurrentPolicy) strID.Format("(ID=%d)",m_pCurrentPolicy->GetID());
	m_strCurrentPathName = fileName;
	pWnd->SetWindowText(m_strCurrentPathName + strID);
	FreshTriggerList();
	
	*pResult = 0;
}

void CPolicyDlg::OnCancel()
{
	OnButtonStopTrigger(); 
	if(m_pCurrentPolicy) 
	{
		if(!m_bReadOnly && g_bPolicyModifyed) 
		{
			if(IDYES==AfxMessageBox("策略数据已经改变，你需要存盘吗?",MB_YESNO|MB_ICONQUESTION))
			{
				m_pCurrentPolicy->Save(m_strCurrentPathName.GetBuffer(0));
				m_strCurrentPathName.ReleaseBuffer();
			}
		}
		m_pCurrentPolicy->Release();
		delete m_pCurrentPolicy;
	}
	CDialog::OnCancel();
}

void CPolicyDlg::OnDblclkListTrigger() 
{
	
	OnButtonStopTrigger();
	if(m_pCurrentPolicy==NULL || m_bStartTest ) return;
	int sel = m_listTrigger.GetCurSel();
	if(sel == -1) return;
	DWORD id = m_listTrigger.GetItemData(sel);
	for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id==pData->GetID())
		{
			CPolicyTriggerDlg dlg;
			dlg.m_pCurrentPolicy = m_pCurrentPolicy;
			dlg.m_bModifyed = true;
			dlg.m_pTriggerData = pData;
			if(IDOK==dlg.DoModal())
			{
				if(dlg.m_bIsChanged) g_bPolicyModifyed = true;
				FreshTriggerList();
			}
			break;
		}
	}
	
	//上面操作可能产生多余的触发器，所以必须做清理操作
	DelRedundancy();
}

bool CPolicyDlg::TraverselTree( void *pTree , CTriggerData *pTrigger)
{
	CTriggerData::_s_tree_item * temp = (CTriggerData::_s_tree_item*)pTree;
	bool bRes = false;
	bool bRes2 = false;
	switch(temp->mConditon.iType) 
	{
	case CTriggerData::c_time_come:
		bRes = IsTimeCome(((C_TIME_COME*)temp->mConditon.pParam)->uID);
		break;
	case CTriggerData::c_path_end_point:
		bRes = IsPathEndPoint();
		break;
	case CTriggerData::c_enmity_reach:
		return true;
	case CTriggerData::c_distance_reach:
		return true;
	case CTriggerData::c_attack_by_skill:
		bRes = IsAttackBySkill();
		break;
	case CTriggerData::c_hp_less:
		bRes = HpLess(((C_HP_LESS*)temp->mConditon.pParam)->fPercent);
		break;
	case CTriggerData::c_random:
		bRes = Random(((C_RANDOM*)temp->mConditon.pParam)->fProbability);
		break;
	case CTriggerData::c_and:
		if(temp->pLeft) bRes = TraverselTree(temp->pLeft,pTrigger);
		if(temp->pRight) bRes2= TraverselTree(temp->pRight,pTrigger);
		return bRes && bRes2;
	case CTriggerData::c_not:
		if(temp->pRight) bRes = TraverselTree(temp->pRight,pTrigger);
		return !bRes;
	case CTriggerData::c_or:
		if(temp->pLeft) bRes = TraverselTree(temp->pLeft,pTrigger);
		if(temp->pRight) bRes2= TraverselTree(temp->pRight,pTrigger);
		return bRes || bRes2;
		break;
	case CTriggerData::c_kill_player:
		return KillPlayer();
	case CTriggerData::c_born:
		return Born();
	case CTriggerData::c_died:
		return Died();
	case CTriggerData::c_start_attack:
		return StartAttack();
		break;
	default:
		return bRes;
	}
	return bRes;
}

//模拟函数
bool CPolicyDlg::IsTimeCome(unsigned int iId)
{
	for( int i = 0; i < listTimer.size(); ++i)
	{
		if(iId == listTimer[i].id)
		{
			if(listTimer[i].counter!=0)
			{
				if(listTimer[i].skipCounter>=listTimer[i].counter)
				{
					m_bTriggerDisable = true;
					return false;
				}
				if(listTimer[i].last_time > listTimer[i].period)
				{
					listTimer[i].skipCounter++;
					listTimer[i].last_time = 0;
					return true;
				}
			}else
			{
				if(listTimer[i].last_time > listTimer[i].period)
				{
					listTimer[i].last_time = 0;
					return true;
				}
			}
		}
	}
	
	return false;
}

bool CPolicyDlg::HpLess( float fLess)
{
	UpdateData(true);
	if(m_fHp < fLess) 
	{
		m_bTriggerDisable = true;
		return true;
	}
	else return false;
}

bool CPolicyDlg::KillPlayer()
{
	UpdateData(true);
	return (bool)m_bSkipKillPlayer;
}

bool CPolicyDlg::Born()
{
	UpdateData(true);
	return (bool)m_bSkipBorn;
}

bool CPolicyDlg::Died()
{
	UpdateData(true);
	return (bool)m_bSkipDied;
}

bool CPolicyDlg::IsPathEndPoint()
{
	UpdateData(true);
	return m_bPathEndPoint;
}

bool CPolicyDlg::IsAttackBySkill()
{
	UpdateData(true);
	return m_bAttackBySkill;
}

bool CPolicyDlg::Random(float fProbability)
{
	UpdateData(true);
	return (bool)m_bSkipRandom;
}

bool CPolicyDlg::StartAttack()
{
	UpdateData(true);
	return (bool)m_bSkipStartAttack;
}

void CPolicyDlg::OnButtonRunTrigger() 
{
	// TODO: Add your control notification handler code here
	CWnd *pWnd = NULL;
	if(m_pCurrentPolicy && !m_bStartTest)
	{
		m_bStartTest = true;
		m_editRunLog.SetSel(0,-1);
		m_editRunLog.ReplaceSel("");
		AddLogMsg("测试运行当前策略已经开始 ... ");
		CWnd *pWnd = GetDlgItem(IDC_BUTTON_RUN_TRIGGER);
		if(pWnd) pWnd->EnableWindow(false);
		pWnd = GetDlgItem(IDC_BUTTON_STOP_TRIGGER);
		if(pWnd) pWnd->EnableWindow(true);
		listGriggerStatus.clear();
		//取得出发触发器的状态保存

		for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
		{
			CTriggerData *pTrigger = m_pCurrentPolicy->GetTriggerPtr(i);
			TRIGGER_STATUS status;
			status.bActive = pTrigger->IsActive();
			status.id = pTrigger->GetID();
			status.bRun = false;
			listGriggerStatus.push_back(status);
		}
	}
}

void CPolicyDlg::OnButtonStopTrigger() 
{
	// TODO: Add your control notification handler code here
	if(m_pCurrentPolicy && m_bStartTest)
	{
		m_bStartTest = false;
		listTimer.clear();
		CWnd *pWnd = GetDlgItem(IDC_BUTTON_RUN_TRIGGER);
		if(pWnd) pWnd->EnableWindow(true);
		pWnd = GetDlgItem(IDC_BUTTON_STOP_TRIGGER);
		if(pWnd) pWnd->EnableWindow(false);
	}
}

bool CPolicyDlg::RunOperaion(void *pTriggerPtr, void *pOperation)
{
	CString msg;
	CTriggerData *pTrigger = (CTriggerData *)pTriggerPtr;
	CTriggerData::_s_operation *pTempOperation = (CTriggerData::_s_operation *)pOperation;
	void *pData = 0;
	char szTalk[MAX_PATH];
	switch(pTempOperation->iType) 
	{
	case CTriggerData::o_attact:
		pData = (O_ATTACK_TYPE *)pTempOperation->pParam;
		msg.Format("**操作[%s] [ID(%d)]",strOperationList[pTempOperation->iType],((O_ATTACK_TYPE*)pData)->uType);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_use_skill:
		pData = (O_USE_SKILL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [技能(%d),等级(%d)]",strOperationList[pTempOperation->iType],((O_USE_SKILL*)pData)->uSkill,((O_USE_SKILL*)pData)->uLevel);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_use_range_skill:
		pData = (O_USE_RANGE_SKILL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [技能(%d),等级(%d),范围(%g)]",strOperationList[pTempOperation->iType],((O_USE_RANGE_SKILL*)pData)->uSkill,((O_USE_RANGE_SKILL*)pData)->uLevel,((O_USE_RANGE_SKILL*)pData)->fRange);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_trigger_task:
		pData = (O_TRIGGER_TASK *)pTempOperation->pParam;
		msg.Format("**操作[%s] [任务ID(%d)]",strOperationList[pTempOperation->iType],((O_TRIGGER_TASK*)pData)->uTaskID);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_change_path:
		pData = (O_CHANGE_PATH *)pTempOperation->pParam;
		msg.Format("**操作[%s] [路径ID(%d)[类型(%d)]",strOperationList[pTempOperation->iType],((O_CHANGE_PATH*)pData)->uPathID,((O_CHANGE_PATH*)pData)->iType);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_sneer_monster:
		pData = (O_SNEER_MONSTER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [范围(%4.2f)]",strOperationList[pTempOperation->iType],((O_SNEER_MONSTER*)pData)->fRange);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_talk:
		pData = (O_TALK_TEXT *)pTempOperation->pParam;
		WideCharToMultiByte(CP_ACP,0,(LPWSTR)((O_TALK_TEXT*)pData)->szData,((O_TALK_TEXT*)pData)->uSize/2,szTalk,MAX_PATH,NULL,FALSE);
		msg.Format("**操作[%s] [内容(%s)]",strOperationList[pTempOperation->iType],szTalk);
		//msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_run_trigger:
		pData = (O_RUN_TRIGGER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [%s]",strOperationList[pTempOperation->iType],GetTriggerName(((O_RUN_TRIGGER*)pData)->uID));
		SetTriggerRunStatus(((O_ACTIVE_TRIGGER*)pData)->uID,true);
		//msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_reset_hate_list:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		break;
	case CTriggerData::o_stop_trigger:
		pData = (O_STOP_TRIGGER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [%s]",strOperationList[pTempOperation->iType],GetTriggerName(((O_STOP_TRIGGER*)pData)->uID));
		SetTriggerActiveStatus(((O_ACTIVE_TRIGGER*)pData)->uID,false);
		break;
	case CTriggerData::o_active_trigger:
		pData = (O_ACTIVE_TRIGGER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [%s]",strOperationList[pTempOperation->iType],GetTriggerName(((O_ACTIVE_TRIGGER*)pData)->uID));
		SetTriggerActiveStatus(((O_ACTIVE_TRIGGER*)pData)->uID,true);
		break;
	case CTriggerData::o_create_timer:
		pData = (O_CREATE_TIMER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [ID(%d) 间隔(%d), 次数(%d)]",strOperationList[pTempOperation->iType],((O_CREATE_TIMER*)pData)->uID,((O_CREATE_TIMER*)pData)->uPeriod,((O_CREATE_TIMER*)pData)->uCounter);
		TIMER_TEST test;
		test.counter = 0;
		test.id = ((O_CREATE_TIMER*)pData)->uID;
		test.last_time = 0;
		test.skipCounter = 0;
		test.period = ((O_CREATE_TIMER*)pData)->uPeriod;
		test.counter = ((O_CREATE_TIMER*)pData)->uCounter;
		listTimer.push_back(test);
		//msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_set_global:
		pData = (O_SET_GLOBAL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d, value=%d]",strOperationList[pTempOperation->iType],((O_SET_GLOBAL *)pData)->iID,((O_SET_GLOBAL *)pData)->iValue);
		break;
	case CTriggerData::o_revise_global:
		pData = (O_REVISE_GLOBAL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d, value=%d]",strOperationList[pTempOperation->iType],((O_REVISE_GLOBAL *)pData)->iID,((O_REVISE_GLOBAL *)pData)->iValue);
		break;
	case CTriggerData::o_assign_global:
		pData = (O_ASSIGN_GLOBAL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [iddst=%d, idsrc=%d]",strOperationList[pTempOperation->iType],((O_ASSIGN_GLOBAL *)pData)->iIDDst,((O_ASSIGN_GLOBAL *)pData)->iIDSrc);
		break;
	case CTriggerData::o_summon:
		pData = (O_SUMMON *)pTempOperation->pParam;
		WideCharToMultiByte(CP_ACP,0,(LPWSTR)((O_SUMMON*)pData)->szName,16,szTalk,MAX_PATH,NULL,FALSE);
		msg.Format("**操作[%s] [%s]",strOperationList[pTempOperation->iType],szTalk);
		break;
	case CTriggerData::o_summon_mineral:
		pData = (O_SUMMON_MINERAL *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d]",strOperationList[pTempOperation->iType],((O_SUMMON_MINERAL *)pData)->uMineralID);
		break;
	case CTriggerData::o_drop_item:
		pData = (O_DROP_ITEM *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d]",strOperationList[pTempOperation->iType],((O_DROP_ITEM *)pData)->uItemID);
		break;
	case CTriggerData::o_change_hate:
		pData = (O_CHANGE_HATE *)pTempOperation->pParam;
		msg.Format("**操作[%s] [改变仇恨值=%d]",strOperationList[pTempOperation->iType],((O_CHANGE_HATE *)pData)->iHateValue);
		break;
	case CTriggerData::o_start_event:
		pData = (O_START_EVENT *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d]",strOperationList[pTempOperation->iType],((O_START_EVENT *)pData)->iId);
		break;
	case CTriggerData::o_stop_event:
		pData = (O_STOP_EVENT *)pTempOperation->pParam;
		msg.Format("**操作[%s] [id=%d]",strOperationList[pTempOperation->iType],((O_STOP_EVENT *)pData)->iId);
		break;
	case CTriggerData::o_kill_timer:
		pData = (O_KILL_TIMER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [ID(%d)]",strOperationList[pTempOperation->iType],((O_KILL_TIMER*)pData)->uID);
		//msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_active_controller:
		pData = (O_ACTIVE_CONTROLLER *)pTempOperation->pParam;
		msg.Format("**操作[%s] [ID(%d)] [停止激活(%d)]",strOperationList[pTempOperation->iType],((O_ACTIVE_CONTROLLER*)pData)->uID,((O_ACTIVE_CONTROLLER*)pData)->bStop);
		//msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_flee:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		break;
	case CTriggerData::o_dispear:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		break;
	case CTriggerData::o_reset:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		break;
	case CTriggerData::o_set_hate_to_first:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_set_hate_to_last:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_set_hate_fifty_percent:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		msg += GetTargetStr(&pTempOperation->mTarget);
		break;
	case CTriggerData::o_skip_operation:
		msg.Format("**操作[%s]",strOperationList[pTempOperation->iType]);
		AddLogMsg((LPCSTR)msg);
		return false;
	default:
		break;
	}
	AddLogMsg((LPCSTR)msg);
	return true;
}

void CPolicyDlg::RunTrigger(unsigned int id)
{
	for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pTrigger = m_pCurrentPolicy->GetTriggerPtr(i);
		if(id==pTrigger->GetID())
		{
			if(pTrigger->IsRun())
			{
				if(TraverselTree(pTrigger->GetConditonRoot(),pTrigger))
				{//已经触发
					CString msg;
					msg.Format("==运行触发器 [%s], 执行下列操作:", pTrigger->GetName() );
					AddLogMsg((LPCTSTR)msg);
					for( int k = 0; k < pTrigger->GetOperaionNum(); k++)
					{//执行操作
						RunOperaion(pTrigger,pTrigger->GetOperaion(k));
					}
				}
				break;
			}
		}
	}
}

void CPolicyDlg::AddLogMsg(const char *szMsg)
{
	TCHAR szBuf[128];
	_stprintf(szBuf,_T("%s\n"),szMsg);
	//	Select all text at first then to cancel selection. This is the way to
	//	ensure caret is at the end of text when new text is added !!
	m_editRunLog.SetSel(0, -1);
	m_editRunLog.SetSel(-1, -1);
	m_editRunLog.ReplaceSel(szBuf);
    SetFocus();
}

void CPolicyDlg::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	int m = listTimer.size();
	for( int h = 0; h < m; ++h)
	{
		if(listTimer[h].skipCounter <= listTimer[h].counter)
			listTimer[h].last_time += 1;
	}

	CString msg;
	if(m_pCurrentPolicy && m_bStartTest)
	{
		for( int i = 0; i < m_pCurrentPolicy->GetTriggerPtrNum(); ++i)
		{
			m_bTriggerDisable = false;
			CTriggerData *pTrigger = m_pCurrentPolicy->GetTriggerPtr(i);
			if(!pTrigger->IsRun())
			{//模拟事件触发表
				if(GetTriggerActiveStatus(pTrigger->GetID()))
				{
					if(TraverselTree(pTrigger->GetConditonRoot(),pTrigger))
					{//已经触发
						if(m_bTriggerDisable) SetTriggerActiveStatus(pTrigger->GetID(),false);
						msg.Format("==运行触发器 [%s], 执行下列操作:", pTrigger->GetName() );
						AddLogMsg((LPCTSTR)msg);
						for( int k = 0; k < pTrigger->GetOperaionNum(); k++)
						{//执行操作
							if(!RunOperaion(pTrigger,pTrigger->GetOperaion(k)))
								break;
						}
					}
				}
			}else
			{//模拟执行触发表、
				if(GetTriggerActiveStatus(pTrigger->GetID()) && GetTriggerRunStatus(pTrigger->GetID()))
				{
					if(TraverselTree(pTrigger->GetConditonRoot(),pTrigger))
					{//已经触发
						if(m_bTriggerDisable) SetTriggerActiveStatus(pTrigger->GetID(),false);
						msg.Format("==运行触发器 [%s], 执行下列操作:", pTrigger->GetName() );
						AddLogMsg((LPCTSTR)msg);
						for( int k = 0; k < pTrigger->GetOperaionNum(); k++)
						{//执行操作
							if(!RunOperaion(pTrigger,pTrigger->GetOperaion(k)))
								break;
						}
					}
				}
			}
		}
	}

	//重新设置事件标志
	m_bSkipKillPlayer = FALSE;
	m_bSkipRandom = FALSE;
	m_bSkipStartAttack = FALSE;
	m_bSkipBorn = FALSE;
	m_bSkipDied = FALSE;
	m_bPathEndPoint = false;
	m_bAttackBySkill = false;
	CDialog::OnTimer(nIDEvent);
}

void CPolicyDlg::SetTriggerActiveStatus( unsigned int id, bool bActive)
{
	int n = listGriggerStatus.size();
	for( int i = 0; i < n; ++i)
	{
		if(listGriggerStatus[i].id == id)
			listGriggerStatus[i].bActive = bActive;
	}
}

void CPolicyDlg::SetTriggerRunStatus( unsigned int id, bool bRun)
{
	int n = listGriggerStatus.size();
	for( int i = 0; i < n; ++i)
	{
		if(listGriggerStatus[i].id == id)
			listGriggerStatus[i].bRun = bRun;
	}
}


bool CPolicyDlg::GetTriggerActiveStatus( unsigned int id)
{
	int n = listGriggerStatus.size();
	for( int i = 0; i < n; ++i)
	{
		if(listGriggerStatus[i].id == id)
			return listGriggerStatus[i].bActive;
	}
	return false;
}

bool CPolicyDlg::GetTriggerRunStatus( unsigned int id)
{
	int n = listGriggerStatus.size();
	for( int i = 0; i < n; ++i)
	{
		if(listGriggerStatus[i].id == id)
			return listGriggerStatus[i].bRun;
	}
	return false;
}

CString CPolicyDlg::GetTriggerName( unsigned int id)
{
	if(m_pCurrentPolicy)
	{
		int n = m_pCurrentPolicy->GetTriggerPtrNum();
		for( int i = 0; i < n; ++i)
		{
			CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
			if(pData->GetID() == id) return pData->GetName();
		}
	}
	return "错误的触发器ID";
}

void CPolicyDlg::OnButtonEventRandom() 
{
	// TODO: Add your control notification handler code here
	m_bSkipRandom = TRUE;
}

void CPolicyDlg::OnButtonEventKillplayer() 
{
	// TODO: Add your control notification handler code here
	m_bSkipKillPlayer = TRUE;
}

void CPolicyDlg::OnButtonEventAttackstart() 
{
	// TODO: Add your control notification handler code here
	m_bSkipStartAttack = TRUE;
	
}

//下面三个函数删除冗余的执行触发器
void CPolicyDlg::DelRedundancy()
{
	abase::vector<unsigned> listId;
	int n = m_pCurrentPolicy->GetTriggerPtrNum();
	int i;
	for( i = 0; i < n; ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(pData->IsRun())
		{
			if(!TriggerIsUse(pData->GetID()))
			listId.push_back(pData->GetID());
		}
	}

	n = listId.size();
	for ( i = 0; i < n; ++i)
	{
		int idx = m_pCurrentPolicy->GetIndex(listId[i]);
		if(idx!=-1) m_pCurrentPolicy->DelTriggerPtr(idx);
	}
	listId.clear();
}

bool CPolicyDlg::TriggerIsUse( unsigned int id)
{
	int n = m_pCurrentPolicy->GetTriggerPtrNum();
	for( int i = 0; i < n; ++i)
	{
		CTriggerData *pData = m_pCurrentPolicy->GetTriggerPtr(i);
		if(!pData->IsRun())
		{
			if(TraceTrigger(pData,id)) return true;
		}
	}
	return false;
}

bool CPolicyDlg::TraceTrigger( CTriggerData *pTrigger,unsigned int id)
{
	if(pTrigger->GetID()==id) return true;
	int num = pTrigger->GetOperaionNum();
	for( int j = 0; j < num; ++j)
	{
		CTriggerData::_s_operation *pOperation = pTrigger->GetOperaion(j);				
		if(pOperation->iType==CTriggerData::o_run_trigger)
		{
			int idx = m_pCurrentPolicy->GetIndex(((O_RUN_TRIGGER*)pOperation->pParam)->uID);
			if(idx!=-1)
			{
				CTriggerData *pNext = m_pCurrentPolicy->GetTriggerPtr(idx);
				if(pNext) if(TraceTrigger(pNext,id)) return true;
			}
		}
	}
	return false;
}

void CPolicyDlg::OnButtonEventBorn() 
{
	// TODO: Add your control notification handler code here
	m_bSkipBorn = TRUE;
}

void CPolicyDlg::OnButtonEventDied() 
{
	// TODO: Add your control notification handler code here
	m_bSkipDied = TRUE;
}

void CPolicyDlg::OnButtonEventEndPoint() 
{
	// TODO: Add your control notification handler code here
	m_bPathEndPoint = true;
}

void CPolicyDlg::OnButtonEventAttackSkill() 
{
	// TODO: Add your control notification handler code here
	m_bAttackBySkill = true;
}
