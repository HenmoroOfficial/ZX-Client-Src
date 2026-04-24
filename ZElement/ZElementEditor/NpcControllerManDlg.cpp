// DynamicObjectManDlg.cpp : implementation file
//

#include "Global.h"
#include "elementeditor.h"
#include "NpcControllerManDlg.h"
#include "NpcControllerDlg.h"
#include "VssOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int sort_column = 1;
bool method = true;

// 比较函数
static int CALLBACK MyCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	// 从参数中提取所需比较lc的两行数据
	 int row1 = (int) lParam1;
	 int row2 = (int) lParam2;

	 CListCtrl* lc = (CListCtrl*)lParamSort;

	 CString lp1 = lc->GetItemText(row1,sort_column);
	 CString lp2 = lc->GetItemText(row2,sort_column);

 	if (sort_column==0 || sort_column == 2)
 	{
		 if (method)
			return atoi(lp1)-atoi(lp2);
		 else
			return atoi(lp2)-atoi(lp1);
 	}

	return 0;
}

void InitControllerObj(CONTROLLER_OBJECT * pObj)
{
	pObj->id = 0;
	pObj->bActived = true;
	pObj->nWaitTime = 0;
	pObj->nStopTime = 0;
	pObj->bActiveTimeInvalid = true;
	pObj->bStopTimeInvalid = true;
	pObj->nControllerID = 0;
	InitControllerTime(&pObj->mActiveTime);
	InitControllerTime(&pObj->mStopTime);
	strcpy(pObj->szName,"Controller");
	pObj->nActiveTimeRange = 60;
	pObj->bRepeatActived = false;
	pObj->nTimeSegNum = 0;
}

void InitControllerTime(CONTROLLER_TIME* pObj)
{
	pObj->nYear = -1;
	pObj->nMouth = -1;
	pObj->nWeek = -1;
	pObj->nDay = -1;
	pObj->nHours = -1;
	pObj->nMinutes = -1;
}


CNpcControllerMan g_ControllerObjectMan;
/////////////////////////////////////////////////////////////////////////////
// CDynamicObjectManDlg dialog


CNpcControllerManDlg::CNpcControllerManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNpcControllerManDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDynamicObjectManDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_uMaxID = 0;
	m_bReadOnly = false;
	m_bChanged = false;
}


void CNpcControllerManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDynamicObjectManDlg)
	DDX_Control(pDX, IDC_STATIC_FLAG, m_FlagCtrl);
	DDX_Control(pDX, IDC_LIST_CONTROLLER_OBJECT, m_listObject);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CNpcControllerManDlg, CDialog)
	//{{AFX_MSG_MAP(CDynamicObjectManDlg)
	ON_BN_CLICKED(ID_CHECK_OUT, OnCheckOut)
	ON_BN_CLICKED(ID_CHECK_IN_CONTROLLER, OnCheckIn)
	ON_BN_CLICKED(IDC_UNDO_CHECKOUT, OnUndoCheckOut)
	ON_BN_CLICKED(ID_ADD_NEWCONTROLLEROBJECT, OnAddNewobject)
	ON_BN_CLICKED(ID_DEL_CONTROLLEROBJECT, OnDelObject)
	ON_BN_CLICKED(ID_EXPORT, OnExport)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_CONTROLLER_OBJECT, OnDblclkListControllerObject)
	ON_NOTIFY(LVN_COLUMNCLICK, IDC_LIST_CONTROLLER_OBJECT, OnLvnColumnClickControllerObject)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDynamicObjectManDlg message handlers

BOOL CNpcControllerManDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_IconWriteable =LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WRITEABLE));
	m_IconReadOnly = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_READONLY));
	// TODO: Add extra initialization here
	m_listObject.SetExtendedStyle(m_listObject.GetExtendedStyle() | LVS_EX_FULLROWSELECT);
	m_listObject.InsertColumn(0, "ID", LVCFMT_LEFT, 80);
	m_listObject.InsertColumn(1, "名字", LVCFMT_LEFT, 360);
	m_listObject.InsertColumn(2, "控制器ID", LVCFMT_LEFT, 80);
	if(g_VSS.IsCheckOutStatus("ResTable\\ControllerObject.dat"))
	{
		m_bReadOnly = false;
		m_FlagCtrl.SetIcon(m_IconWriteable);
	}else 
	{
		m_FlagCtrl.SetIcon(m_IconReadOnly);
		m_bReadOnly = true;
	}
	if(!Load()) MessageBox("加载控制器文件数据失败！");



	FreshObjectList();
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CNpcControllerManDlg::OnCheckOut() 
{
	// TODO: Add your control notification handler code here
	g_VSS.SetProjectPath("ResTable");
	g_VSS.CheckOutFile("ResTable\\ControllerObject.dat");
	CString rc = g_VSS.GetLastCommandResult();
	if(g_VSS.IsCheckOutStatus("ResTable\\ControllerObject.dat"))
	{
		m_bReadOnly = false;
		m_FlagCtrl.SetIcon(m_IconWriteable);
		if(!Load()) MessageBox("加载控制对象文件(ResTable\\ControllerObject.dat)失败！");
		g_ControllerObjectMan.Load();
		FreshObjectList();
	}else
	{
		m_bReadOnly = true;
		m_FlagCtrl.SetIcon(m_IconReadOnly);
		MessageBox(rc);
	}
}
void CNpcControllerManDlg::OnUndoCheckOut() 
{
	if(AfxMessageBox("确定要Undo Checkout? 如果控制器有改动，改动的数据将会丢失。\n\nVSS有风险，Undo须谨慎。",MB_YESNO|MB_ICONQUESTION) != IDYES)
		return;
			

	// TODO: Add your control notification handler code here
	g_VSS.SetProjectPath("ResTable");
	g_VSS.UndoCheckOut("ResTable\\ControllerObject.dat");
	CString rc = g_VSS.GetLastCommandResult();
	if(!g_VSS.IsCheckOutStatus("ResTable\\ControllerObject.dat"))
	{
		m_bReadOnly = true;
		m_FlagCtrl.SetIcon(m_IconReadOnly);
//		MessageBox(rc);
	}
	else
	{
		m_bReadOnly = false;
		m_FlagCtrl.SetIcon(m_IconWriteable);
		if(!Load()) MessageBox("加载控制对象文件(ResTable\\ControllerObject.dat)失败！");
		g_ControllerObjectMan.Load();
		FreshObjectList();
	}
}
void CNpcControllerManDlg::OnCheckIn() 
{
	// TODO: Add your control notification handler code here
	if(m_bReadOnly) return;
	if(!Save()) 
	{
		MessageBox("保存数据(ResTable\\ControllerObject.dat)失败!");
		return;
	}
	g_VSS.SetProjectPath("ResTable");
	g_VSS.CheckInFile("ControllerObject.dat");
	CString rc = g_VSS.GetLastCommandResult();
	if(g_VSS.IsCheckOutStatus("ResTable\\ControllerObject.dat"))
	{
		m_bReadOnly = false;
		m_FlagCtrl.SetIcon(m_IconWriteable);
		MessageBox(rc);
	}else 
	{
		m_FlagCtrl.SetIcon(m_IconReadOnly);
		m_bReadOnly = true;
	}
	g_ControllerObjectMan.Load();
}

void CNpcControllerManDlg::OnAddNewobject() 
{
	// TODO: Add your control notification handler code here
	if(m_bReadOnly) return;
	CNpcControllerDlg dlg;
	if(IDOK == dlg.DoModal())
	{
		dlg.m_Object.id = ++m_uMaxID;
		m_listControllerObject.push_back(dlg.m_Object);
		m_bChanged = true;
		FreshObjectList();
	}
}

void CNpcControllerManDlg::FreshObjectList()
{
	m_listObject.DeleteAllItems();
	int index = 0;
	for( int i = 0; i < m_listControllerObject.size(); ++i)
	{
		CString temp;
		temp.Format("%d",m_listControllerObject[i].id);
		int sel = m_listObject.InsertItem(index++,temp);
		m_listObject.SetItemText(sel, 1, m_listControllerObject[i].szName);

		temp.Format("%d", m_listControllerObject[i].nControllerID);
		m_listObject.SetItemText(sel, 2, temp);
	}
}

void CNpcControllerManDlg::OnDelObject() 
{
	// TODO: Add your control notification handler code here
	if(m_bReadOnly) return;
	POSITION pos = m_listObject.GetFirstSelectedItemPosition();
	if (pos == NULL) return;
	
	CString msg("你确认要删除下列资源:\n\n");
	while (pos)
	{
		int nItem = m_listObject.GetNextSelectedItem(pos);
		CString temp = m_listObject.GetItemText(nItem,1);
		msg +=temp;
		msg +="\n";
	}
	if(IDYES != AfxMessageBox(msg,MB_YESNO|MB_ICONQUESTION)) return;
	
	pos = m_listObject.GetFirstSelectedItemPosition();
	while(pos)
	{
		int nItem = m_listObject.GetNextSelectedItem(pos);
		CString temp = m_listObject.GetItemText(nItem,0);
		unsigned int id = (unsigned int)atof(temp);
		DelDynamicObj(id);
	}
	m_bChanged = true; 
	FreshObjectList();
}

void CNpcControllerManDlg::DelDynamicObj( unsigned int id)
{
	abase::vector<CONTROLLER_OBJECT>::iterator it = m_listControllerObject.begin();
	for( it; it != m_listControllerObject.end(); it++)
	{
		if((*it).id == id)
		{
			m_listControllerObject.erase(it);
			return;
		}
	}
}

void CNpcControllerManDlg::GetDynamicObj(unsigned int id, CONTROLLER_OBJECT &obj)
{
	abase::vector<CONTROLLER_OBJECT>::iterator it = m_listControllerObject.begin();
	for( it; it != m_listControllerObject.end(); it++)
	{
		if((*it).id == id)
		{
			obj = (*it);
			return;
		}
	}
	InitControllerObj(&obj);
}

void CNpcControllerManDlg::SetDynamicObj( unsigned int id,CONTROLLER_OBJECT &obj)
{
	abase::vector<CONTROLLER_OBJECT>::iterator it = m_listControllerObject.begin();
	for( it; it != m_listControllerObject.end(); it++)
	{
		if((*it).id == id)
		{
			memcpy(&(*it),&obj,sizeof(CONTROLLER_OBJECT_VERSION3));

			(*it).nTimeSegNum = obj.nTimeSegNum;
			(*it).aTimeSeg.clear();

			for(int i=0;i<obj.GetTimeSegNum(); i++)
			{
				(*it).aTimeSeg.push_back(obj.aTimeSeg[2*i]);
				(*it).aTimeSeg.push_back(obj.aTimeSeg[2*i+1]);
			}

			return;
		}
	}
}

void CNpcControllerManDlg::OnExport() 
{
	// TODO: Add your control notification handler code here
	if(m_listControllerObject.size() < 1) return;
	
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	//if (!szPath[0])
	//	strcpy(szPath, g_szWorkDir);

	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "请选择导出数据的存放路径：";
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN ;
	
	LPITEMIDLIST pPIDL;
	if (!(pPIDL = SHBrowseForFolder(&Info)))
		return;

	SHGetPathFromIDList(pPIDL, szPath);
	sprintf(szPath,"%s\\ControllerObjects.data",szPath);
	if(!Export(szPath))
		MessageBox("数据输出失败!");
}

void CNpcControllerManDlg::OnOK()
{
	if(m_bChanged && !m_bReadOnly)
	{
		if(IDYES==MessageBox("数据已经改变，你需要存盘吗？","询问",MB_YESNO | MB_ICONQUESTION))
		{
			Save();
			g_ControllerObjectMan.Load();
		}
	}
	CDialog::OnOK();
}
void CNpcControllerManDlg::OnCancel()
{
	OnOK();
}

bool CNpcControllerManDlg::Save()
{
	FILE *pFile = fopen("ResTable\\ControllerObject.dat","wb");
	if(pFile==NULL) return false;
	DWORD dwVersion = CONTROLLER_VERSION;
	fwrite(&dwVersion,sizeof(DWORD),1,pFile);
	int size = m_listControllerObject.size();
	fwrite(&size,sizeof(int),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		fwrite(&m_listControllerObject[i],sizeof(CONTROLLER_OBJECT_VERSION3),1,pFile);

		fwrite(&m_listControllerObject[i].nTimeSegNum, sizeof(int), 1, pFile);
		for(int j=0;j<m_listControllerObject[i].GetTimeSegNum(); j++)
		{
			CONTROLLER_TIME _time = m_listControllerObject[i].aTimeSeg[2*j];
			fwrite(&_time, sizeof(CONTROLLER_TIME), 1, pFile);

			_time = m_listControllerObject[i].aTimeSeg[2*j+1];
			fwrite(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
		}
	}
	fclose(pFile);
	return true;
}

bool CNpcControllerManDlg::Load()
{
	m_listControllerObject.clear();
	
	FILE *pFile = fopen("ResTable\\ControllerObject.dat","rb");
	if(pFile==NULL) return false;
	DWORD dwVersion;
	fread(&dwVersion,sizeof(DWORD),1,pFile);
	if(dwVersion==1)
	{
		int size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CONTROLLER_OBJECT_VERSION1 obj1;
			fread(&obj1,sizeof(CONTROLLER_OBJECT_VERSION1),1,pFile);
			
			CONTROLLER_OBJECT  obj;
			memcpy(&obj,&obj1,sizeof(CONTROLLER_OBJECT_VERSION1));
			obj.nActiveTimeRange = 60;
			obj.bRepeatActived = false;

			obj.nTimeSegNum = 0;
			
			if(obj.id > m_uMaxID) m_uMaxID = obj.id;
			m_listControllerObject.push_back(obj);
		}
	}else if(dwVersion == 2)
	{
		int size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CONTROLLER_OBJECT_VERSION2 obj2;
			fread(&obj2,sizeof(CONTROLLER_OBJECT_VERSION2),1,pFile);
			
			CONTROLLER_OBJECT  obj;
			memcpy(&obj,&obj2,sizeof(CONTROLLER_OBJECT_VERSION2));
			obj.bRepeatActived = false;
			
			obj.nTimeSegNum = 0;

			if(obj.id > m_uMaxID) m_uMaxID = obj.id;
			m_listControllerObject.push_back(obj);
		}
	}else if(dwVersion == 3)
	{
		int size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CONTROLLER_OBJECT_VERSION3 obj3;
			fread(&obj3,sizeof(CONTROLLER_OBJECT_VERSION3),1,pFile);
			
			CONTROLLER_OBJECT obj;
			memcpy(&obj,&obj3,sizeof(CONTROLLER_OBJECT_VERSION3));
			obj.nTimeSegNum = 0;

			if(obj.id > m_uMaxID) m_uMaxID = obj.id;
			m_listControllerObject.push_back(obj);
		}
	}
	else if(dwVersion == 4)
	{
		int size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CONTROLLER_OBJECT_VERSION3 obj3;
			fread(&obj3,sizeof(CONTROLLER_OBJECT_VERSION3),1,pFile);
			
			CONTROLLER_OBJECT obj;
			memcpy(&obj,&obj3,sizeof(CONTROLLER_OBJECT_VERSION3));

			int num;
			fread(&num, sizeof(int), 1, pFile);
			obj.nTimeSegNum = num;

			for(int j=0; j<obj.GetTimeSegNum(); j++)
			{
				CONTROLLER_TIME _time;
				fread(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
				obj.aTimeSeg.push_back(_time);

				fread(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
				obj.aTimeSeg.push_back(_time);
			}

			if(obj.id > m_uMaxID) m_uMaxID = obj.id;
			m_listControllerObject.push_back(obj);
		}
	}	
	else
	{
		fclose(pFile);
		return false;
	}
	fclose(pFile);
	return true;
}

#define	CONTROLL_IDENTIFY		(('C'<<24) | ('T'<<16) | ('I'<<8) | 'D')
bool CNpcControllerManDlg::Export(const char* szPath)
{
	FILE *pFile = fopen(szPath,"wb");
	if(pFile==NULL) return false;
	
	unsigned long Identify = CONTROLL_IDENTIFY;
	fwrite(&Identify,sizeof(unsigned long),1,pFile);
	
	unsigned long size = m_listControllerObject.size();
	fwrite(&size,sizeof(unsigned long),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		fwrite(&m_listControllerObject[i].id,sizeof(unsigned int),1,pFile);
		fwrite(&m_listControllerObject[i].nControllerID,sizeof(int),1,pFile);
		fwrite(&m_listControllerObject[i].bActived,sizeof(bool),1,pFile);			
		fwrite(&m_listControllerObject[i].nWaitTime,sizeof(int),1,pFile);			
		fwrite(&m_listControllerObject[i].nStopTime,sizeof(int),1,pFile);		
		fwrite(&m_listControllerObject[i].bActiveTimeInvalid,sizeof(bool),1,pFile); 
		fwrite(&m_listControllerObject[i].bStopTimeInvalid,sizeof(bool),1,pFile);
		fwrite(&m_listControllerObject[i].mActiveTime,sizeof(CONTROLLER_TIME),1,pFile);
		fwrite(&m_listControllerObject[i].mStopTime,sizeof(CONTROLLER_TIME),1,pFile);
		fwrite(&m_listControllerObject[i].nActiveTimeRange,sizeof(int),1,pFile);
		fwrite(&m_listControllerObject[i].bRepeatActived,sizeof(bool),1,pFile);	
		fwrite(&m_listControllerObject[i].nTimeSegNum,sizeof(int),1,pFile);	

		for(int j=0;j<m_listControllerObject[i].GetTimeSegNum(); j++)
		{
			CONTROLLER_TIME _time = m_listControllerObject[i].aTimeSeg[2*j];
			fwrite(&_time, sizeof(CONTROLLER_TIME), 1, pFile);

			_time = m_listControllerObject[i].aTimeSeg[2*j+1];
			fwrite(&_time, sizeof(CONTROLLER_TIME), 1, pFile);
		}
	}
	fclose(pFile);
	return true;
}

void CNpcControllerManDlg::OnDblclkListControllerObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	POINT pt;
	GetCursorPos(&pt);
	m_listObject.ScreenToClient(&pt);
	int sel = m_listObject.HitTest(pt);
	if(sel!=-1)
	{
		CString temp = m_listObject.GetItemText(sel,0);
		unsigned int id = (unsigned int)atof(temp);
		CONTROLLER_OBJECT obj;
		GetDynamicObj(id, obj);
		CNpcControllerDlg dlg;
		dlg.m_bReadOnly = m_bReadOnly;
	
		memcpy(&dlg.m_Object,&obj,sizeof(CONTROLLER_OBJECT_VERSION3));
		dlg.m_Object.nTimeSegNum = obj.nTimeSegNum;
		for(int i=0;i<obj.GetTimeSegNum(); i++)
		{
			dlg.m_Object.aTimeSeg.push_back(obj.aTimeSeg[2*i]);
			dlg.m_Object.aTimeSeg.push_back(obj.aTimeSeg[2*i+1]);
		}

		if(IDOK == dlg.DoModal())
		{
			SetDynamicObj(id,dlg.m_Object);
			m_bChanged = true;
			FreshObjectList();
			m_listObject.SetItemState(sel,LVIS_SELECTED, LVIS_SELECTED);
		}
	}
	*pResult = 0;
}


void CNpcControllerManDlg::OnLvnColumnClickControllerObject(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	sort_column = pNMLV->iSubItem;
	method = !method;

	int count = m_listObject.GetItemCount();
	for(int i=0;i<count; i++)
	{
		m_listObject.SetItemData(i,i);
	}

	m_listObject.SortItems(MyCompareProc, (DWORD_PTR)&m_listObject);//排序 其中 m_listObject为CListCtrl型变量

	*pResult = 0;
	
}