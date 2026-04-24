// TreasureDlg.cpp : implementation file
//

#include "StdAfx.h"
#include "global.h"
#include "elementdata.h"
#include "TreasureDlg.h"
#include "TreasureItemDlg.h"
#include "VssOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif






/////////////////////////////////////////////////////////////////////////////
// CTreasureDlg dialog


CTreasureDlg::CTreasureDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTreasureDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTreasureDlg)
	//}}AFX_DATA_INIT
	m_bReadOnly = true;
	m_bChanged = false;
}


void CTreasureDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTreasureDlg)
	DDX_Control(pDX, IDC_FLOAT_ICON, m_staticIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTreasureDlg, CDialog)
	//{{AFX_MSG_MAP(CTreasureDlg)
	ON_BN_CLICKED(ID_EXPORT, OnExport)
	ON_BN_CLICKED(ID_CHECK_OUT, OnCheckOut)
	ON_BN_CLICKED(ID_CHECK_IN, OnCheckIn)
	ON_BN_CLICKED(ID_FRESH_TREASURE, OnFreshTreasure)
	ON_BN_CLICKED(ID_MOVE_LAST, OnMoveLast)
	ON_BN_CLICKED(ID_MOVE_DOWN, OnMoveDown)
	ON_BN_CLICKED(ID_SLECTED_ALL, OnSlectedAll)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CTreasureDlg::DestroyWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_TreasureMan.Release();
	return CDialog::DestroyWindow();
}

void CTreasureDlg::FreshList()
{
	m_treeTreasue.DeleteAllItems();
	int i;
	for( i = 0; i < 8 ; ++i)
	{
		MAIN_TYPE *pList = m_TreasureMan.GetMainTypeList();
		HTREEITEM hParent = m_treeTreasue.InsertItem(pList[i].szName);
		m_treeTreasue.SetItemData(hParent,pList[i].id);
		m_treeTreasue.SetItemImage(hParent,0,0);
		for( int j = 0; j < pList[i].listSubType.size(); ++j)
		{
			
			HTREEITEM hItem = m_treeTreasue.InsertItem(pList[i].listSubType[j].szName,hParent);
			int sub_id = pList[i].listSubType[j].id;
			m_treeTreasue.SetItemData(hItem,sub_id);
			m_treeTreasue.SetItemImage(hItem,1,1);
			for( int k = 0; k < m_TreasureMan.GetTreasureCount(); ++k)
			{
				TREASURE_ITEM_VERSION11 *pItem = m_TreasureMan.GetTreasure(k);
				if(pItem->main_type == pList[i].id && pItem->sub_type == sub_id)
				{
					HTREEITEM hLeaf = m_treeTreasue.InsertItem(pItem->szName,hItem);
					m_treeTreasue.SetItemData(hLeaf,pItem->tid);
					m_treeTreasue.SetItemImage(hLeaf,2,2);
				}
			}
		}
	}
	HTREEITEM hNpcParent = m_treeTreasue.InsertItem("百宝阁NPC数据");
	m_treeTreasue.SetItemImage(hNpcParent,3,3);
	m_treeTreasue.SetItemData(hNpcParent,100000);
	abase::vector<MAIN_TYPE*>* pNpcData = m_TreasureMan.GetNpcMainTypeList();
	for( i = 0; i < pNpcData->size(); ++i)
	{
		MAIN_TYPE *pMain = (*pNpcData)[i];
		CString strName;
		strName.Format("%s(id=%d)",pMain->szName,pMain->id);
		HTREEITEM hMain = m_treeTreasue.InsertItem(strName,hNpcParent);
		m_treeTreasue.SetItemImage(hMain,0,0);
		m_treeTreasue.SetItemData(hMain,pMain->id);
		for(int j = 0; j < pMain->listSubType.size(); ++j)
		{
			HTREEITEM hSub = m_treeTreasue.InsertItem(pMain->listSubType[j].szName,hMain);
			m_treeTreasue.SetItemData(hSub,pMain->listSubType[j].id);
			m_treeTreasue.SetItemImage(hSub,1,1);
		
			for( int k = 0; k < m_TreasureMan.GetTreasureCount(); ++k)
			{
				TREASURE_ITEM_VERSION11 *pItem = m_TreasureMan.GetTreasure(k);
				if(pItem->main_type == pMain->id && pItem->sub_type == pMain->listSubType[j].id)
				{
					HTREEITEM hLeaf = m_treeTreasue.InsertItem(pItem->szName,hSub);
					m_treeTreasue.SetItemData(hLeaf,pItem->tid);
					m_treeTreasue.SetItemImage(hLeaf,2,2);
				}
			}
		}
	}
}





void CTreasureDlg::OnExport() 
{
	// TODO: Add your control notification handler code here
	//if(m_TreasureMan.GetTreasureCount() < 1) return;
	
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	
	bool bFremdness = false;
	if(IDYES != AfxMessageBox("是输出本国版本么？如果是国外的请选择否!",MB_YESNO|MB_ICONQUESTION))
	{
		bFremdness = true;
	}
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
	
	m_treeTreasue.SortTreasure();
	
	if (m_strData.Find("2") != -1)
		sprintf(szPath,"%s\\gshop2.data",szPath);
	else if(m_strData.Find("1") != -1)
		sprintf(szPath,"%s\\gshop1.data",szPath);
	else 
		sprintf(szPath,"%s\\gshop.data",szPath);
	if(!m_TreasureMan.Export(szPath,bFremdness))
		MessageBox("数据输出失败!");
}

BOOL CTreasureDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	CRect rc,rc2;
	GetWindowRect(rc2);
	GetClientRect(rc);
	int h = rc2.Height() - rc.Height() - 8;
	CWnd *pw = GetDlgItem(IDC_TREE_TREASURE);
	pw->GetWindowRect(rc);

	int width = rc.Width();
	int height = rc.Height();

	rc.left = rc.left - rc2.left;
	rc.top  = rc.top - rc2.top - h;
	rc.right = rc.left + width;
	rc.bottom = rc.top + height;

	m_treeTreasue.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT
   |TVS_EDITLABELS | TVS_SHOWSELALWAYS,
   rc, this, 0x1005);

	m_ImageList.Create (IDB_BITMAP_TREASURE_FLAG,16,4,RGB(255,0,255) );
	m_treeTreasue.SetImageList ( &m_ImageList,TVSIL_NORMAL );

	// TODO: Add extra initialization here
	m_IconWriteable =LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WRITEABLE));
	m_IconReadOnly = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_OBJECT));

	CString path;
	path.Format("TreasureData\\%s",m_strData);
	if(FileIsExist(AString(path)))
	{
		if(FileIsReadOnly(AString(path)))
		{
			CreateDirectory("TreasureData", NULL);
			g_VSS.SetProjectPath("TreasureData");
			g_VSS.GetAllFiles("TreasureData");
			m_staticIcon.SetIcon(m_IconReadOnly);
			m_bReadOnly = true;
		}else
		{
			m_staticIcon.SetIcon(m_IconWriteable);
			m_bReadOnly = false;
		}
		
		if(!m_TreasureMan.Load(m_strData))
		{
			CString msg;
			msg.Format("不能打开 %s",path);
			MessageBox(msg);
		}

		FreshList();
	}else 
	{
		m_staticIcon.SetIcon(m_IconWriteable);
		m_bReadOnly = false;
	}

	m_treeTreasue.SetTreasureMan(&m_TreasureMan);
	m_treeTreasue.m_bReadOnly = m_bReadOnly;
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreasureDlg::OnCheckOut() 
{
	// TODO: Add your control notification handler code here
	if(!m_bReadOnly) return;

	CString path;
	path.Format("TreasureData\\%s",m_strData);
	g_VSS.SetProjectPath("TreasureData");
	g_VSS.CheckOutFile(path);
	if(FileIsReadOnly(AString(path)))
	{
		MessageBox("取出文件失败！");
		return;
	}
	m_bReadOnly = false;
	m_staticIcon.SetIcon(m_IconWriteable);
	
	m_TreasureMan.Release();
	if(!m_TreasureMan.Load(m_strData))
	{
		CString msg;
		msg.Format("不能打开 %s", path);
		MessageBox(msg);
	}
	m_treeTreasue.m_bReadOnly = m_bReadOnly;
	FreshList();
}

void CTreasureDlg::OnCheckIn() 
{
	// TODO: Add your control notification handler code here
	if(m_bReadOnly) return;
	
	if(!m_bReadOnly)
	{
		if(m_bChanged || m_TreasureMan.IsChanged() || m_treeTreasue.m_bChanged)
		{
			if(IDYES==MessageBox("数据已经改动，你需要把改动后的数据保存到服务器吗？","存盘",MB_YESNO|MB_ICONQUESTION))
			{
				m_treeTreasue.SortTreasure();
				m_TreasureMan.Save(m_strData);
			}
			m_bChanged = false;
		}

		CString path;
		path.Format("TreasureData\\%s",m_strData);
		g_VSS.SetProjectPath("TreasureData");
		g_VSS.CheckInFile(m_strData);
		if(FileIsReadOnly(AString(path)))
		{
			m_bReadOnly = true;
			m_staticIcon.SetIcon(m_IconReadOnly);
		}else
		{ 
			m_bReadOnly = false;
		}
	}
	m_treeTreasue.m_bReadOnly = m_bReadOnly;
}

void CTreasureDlg::OnOK()
{
	if((m_bChanged || m_TreasureMan.IsChanged() || m_treeTreasue.m_bChanged) && !m_bReadOnly )
	{
		if(IDYES==MessageBox("数据已经改动，你需要存盘吗？","存盘",MB_YESNO|MB_ICONQUESTION))
		{
			m_treeTreasue.SortTreasure();
			m_TreasureMan.Save(m_strData);
		}
	}
	CDialog::OnOK();
}
void CTreasureDlg::OnCancel()
{
	OnOK();
}

void CTreasureDlg::OnFreshTreasure() 
{
	// TODO: Add your control notification handler code here
	CString path;
	path.Format("TreasureData\\%s",m_strData);
	if(FileIsExist(AString(path)))
	{
		if(!FileIsReadOnly(AString(path)))
		{
			return;
		}
	}else CreateDirectory("TreasureData", NULL);

	g_VSS.SetProjectPath("TreasureData");
	g_VSS.GetAllFiles("TreasureData");
	m_staticIcon.SetIcon(m_IconReadOnly);
	m_bReadOnly = true;
	m_TreasureMan.Release();
	m_TreasureMan.Load(m_strData);
	FreshList();
}

BOOL CTreasureDlg::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	// If edit control is visible in tree view control, when you send a
      // WM_KEYDOWN message to the edit control it will dismiss the edit
      // control. When the ENTER key was sent to the edit control, the
      // parent window of the tree view control is responsible for updating
      // the item's label in TVN_ENDLABELEDIT notification code.
      if (pMsg->message == WM_KEYDOWN &&
         pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE)
      {
         CEdit* edit = m_treeTreasue.GetEditControl();
         if (edit)
         {
            edit->SendMessage(WM_KEYDOWN, pMsg->wParam, pMsg->lParam);
            return TRUE;
         }
      }
      return CDialog::PreTranslateMessage(pMsg);
}

void CTreasureDlg::OnMoveLast() 
{
	// TODO: Add your control notification handler code here
	m_treeTreasue.MoveUp();
}

void CTreasureDlg::OnMoveDown() 
{
	// TODO: Add your control notification handler code here
	m_treeTreasue.MoveDown();
}

void CTreasureDlg::OnSlectedAll() 
{
	// TODO: Add your control notification handler code here
	if(IDYES == AfxMessageBox("你确定要把所有商品的出售方案么全选么",MB_YESNO))
	{
		for( int k = 0; k < m_TreasureMan.GetTreasureCount(); ++k)
		{
			TREASURE_ITEM_VERSION11 *pItem = m_TreasureMan.GetTreasure(k);
			pItem->others = pItem->others | 0x00ff0000;
		}
	}
}
