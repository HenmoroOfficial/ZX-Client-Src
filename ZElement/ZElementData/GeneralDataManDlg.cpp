// GeneralDataManDlg.cpp : implementation file
//

#include "stdafx.h"
#include "global.h"
#include "elementdata.h"
#include "GeneralDataManDlg.h"
#include "GeneralDataManUtil.h"
#include "VssOperation.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataManDlg dialog


CGeneralDataManDlg::CGeneralDataManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CGeneralDataManDlg::IDD, pParent), m_pGeneralDataMan(NULL), m_bReadOnly(false), m_nGeneralDataTemplTypeID(0)
{
	//{{AFX_DATA_INIT(CGeneralDataManDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CGeneralDataManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CGeneralDataManDlg)
	DDX_Control(pDX, IDC_FLOAT_ICON, m_staticIcon);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CGeneralDataManDlg, CDialog)
	//{{AFX_MSG_MAP(CGeneralDataManDlg)
	ON_BN_CLICKED(ID_EXPORT, OnExport)
	ON_BN_CLICKED(ID_FRESH_TREASURE, OnFreshGeneralData)
	ON_BN_CLICKED(ID_CHECK_OUT, OnCheckOut)
	ON_BN_CLICKED(ID_CHECK_IN, OnCheckIn)
	ON_BN_CLICKED(ID_MOVE_LAST, OnMoveLast)
	ON_BN_CLICKED(ID_MOVE_DOWN, OnMoveDown)
	ON_BN_CLICKED(IDC_EXPORT_ATTRIBUTE, OnExportAttribute)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGeneralDataManDlg message handlers

BOOL CGeneralDataManDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	//设置窗口名称
	SetWindowText( _T("VIP奖励数据编辑器") );

	//创建树结构对象
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

	m_treeDataMan.Create(WS_VISIBLE | WS_TABSTOP | WS_CHILD | WS_BORDER
   | TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT
   |TVS_EDITLABELS | TVS_SHOWSELALWAYS,
   rc, this, 0x1005);

	m_ImageList.Create (IDB_BITMAP_TREASURE_FLAG,16,4,RGB(255,0,255) );
	m_treeDataMan.SetImageList ( &m_ImageList,TVSIL_NORMAL );

	// TODO: Add extra initialization here
	m_IconWriteable =LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_WRITEABLE));
	m_IconReadOnly = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_OBJECT));

	//
	CString path;
	path.Format("GeneralData\\%s", m_strDataFileName);
	if( FileIsExist(AString(path)) )
	{
		if(FileIsReadOnly(AString(path)))
		{
			CreateDirectory("GeneralData", NULL);
			g_VSS.SetProjectPath("GeneralData");
			g_VSS.GetAllFiles("GeneralData");
			m_staticIcon.SetIcon(m_IconReadOnly);
			m_bReadOnly = true;
		}
		else
		{
			m_staticIcon.SetIcon(m_IconWriteable);
			m_bReadOnly = false;
		}
		if ( m_pGeneralDataMan )
		{
			delete m_pGeneralDataMan;
			m_pGeneralDataMan = NULL;
		}
		m_pGeneralDataMan = new CGeneralDataMan( m_szMainTypes, m_nMainTypesCount, m_strTemplIDFileName, m_strOrgPath, (GENERAL_DATA_TEMPL_TYPE_ID)m_nGeneralDataTemplTypeID );
		if ( !m_pGeneralDataMan )
		{
			AfxMessageBox( "初始化数据管理器失败!" );
		}
		if ( !m_pGeneralDataMan->Load( path ) )
		{
			CString msg;
			msg.Format("不能打开 %s",path);
			MessageBox(msg);
		}
		RefreshTreeList();
	}
	else
	{
		m_staticIcon.SetIcon(m_IconWriteable);
		m_bReadOnly = false;
	}

	m_treeDataMan.SetDataMan( m_pGeneralDataMan );
	m_treeDataMan.m_bDataReadOnly = m_bReadOnly;
	m_treeDataMan.m_strOrgPath = m_strOrgPath;

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


void CGeneralDataManDlg::RefreshTreeList( void )
{
	m_treeDataMan.DeleteAllItems();
	abase::vector<GENERAL_DATA_MAIN_TYPE*>* pMainTypeList = m_pGeneralDataMan->GetDataManMainTypeList();
	for ( int i = 0; i < pMainTypeList->size(); ++i )
	{
		HTREEITEM hParent = m_treeDataMan.InsertItem( (*pMainTypeList)[i]->szName );
		m_treeDataMan.SetItemData(hParent,(*pMainTypeList)[i]->id);
		m_treeDataMan.SetItemImage(hParent,0,0);
		for ( int j = 0; j < (*pMainTypeList)[i]->listSubType.size(); ++j )
		{
			HTREEITEM hItem = m_treeDataMan.InsertItem( (*pMainTypeList)[i]->listSubType[j].szName, hParent );
			int sub_id = (*pMainTypeList)[i]->listSubType[j].id;
			m_treeDataMan.SetItemData(hItem,sub_id);
			m_treeDataMan.SetItemImage(hItem,1,1);

			abase::vector<int> listSubTypeItemID = (*pMainTypeList)[i]->listSubType[j].listSubTypeItemID;
			for( int k = 0; k < listSubTypeItemID.size(); ++k )
			{
				BaseDataTempl* pDataTempl = m_pGeneralDataMan->GetDataItemTemplByID( listSubTypeItemID[k] );
				if ( pDataTempl )
				{
					HTREEITEM hLeaf = m_treeDataMan.InsertItem(pDataTempl->GetName(),hItem);
					m_treeDataMan.SetItemData(hLeaf,pDataTempl->GetID());
					m_treeDataMan.SetItemImage(hLeaf,2,2);
				}
			}
		}
	}
}

void CGeneralDataManDlg::Release()
{
	if ( m_pGeneralDataMan )
	{
		delete m_pGeneralDataMan;
		m_pGeneralDataMan = NULL;
	}
}


BOOL CGeneralDataManDlg::DestroyWindow() 
{
	//
	Release();
	
	return CDialog::DestroyWindow();
}

void CGeneralDataManDlg::OnOK() 
{
	//
	if ( !m_bReadOnly )
	{
		if ( IDYES == MessageBox("你需要存盘吗？","存盘",MB_YESNO|MB_ICONQUESTION) )
		{
			m_pGeneralDataMan->Save( "GeneralData\\" + m_strDataFileName );
		}
	}
	CDialog::OnOK();
}

void CGeneralDataManDlg::OnCancel()
{
	OnOK();
}

void CGeneralDataManDlg::OnExport() 
{
	//
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	
	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "请选择导出数据的存放路径：";
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN ;
	
	LPITEMIDLIST pPIDL;
	if ( !(pPIDL = SHBrowseForFolder(&Info)) )
	{
		return;
	}

	SHGetPathFromIDList(pPIDL, szPath);	

	sprintf(szPath,"%s\\VIPAward.data",szPath);
	if ( !m_pGeneralDataMan->Export( szPath ) )
	{
		AfxMessageBox( "数据导出失败！" );
	}
	
}

void CGeneralDataManDlg::OnFreshGeneralData() 
{
	//
	CString path;
	path.Format("GeneralData\\%s",m_strDataFileName);
	if ( FileIsExist(AString(path)) )
	{
		if( !FileIsReadOnly(AString(path)) )
		{
			return;
		}
	}
	else 
	{
		CreateDirectory("GeneralData", NULL);
	}

	g_VSS.SetProjectPath("GeneralData");
	g_VSS.GetAllFiles("GeneralData");
	m_staticIcon.SetIcon(m_IconReadOnly);
	m_bReadOnly = true;

	m_pGeneralDataMan->ClearAllDataTemplItems();
	m_pGeneralDataMan->Load(path);
	RefreshTreeList();
}

void CGeneralDataManDlg::OnCheckOut() 
{
	//
	if ( !m_bReadOnly )
	{
		return;
	}

	CString path;
	path.Format("GeneralData\\%s",m_strDataFileName);
	g_VSS.SetProjectPath("GeneralData");
	g_VSS.CheckOutFile(path);
	if( FileIsReadOnly(AString(path)) )
	{
		MessageBox("取出文件失败！");
		return;
	}
	m_bReadOnly = false;
	m_staticIcon.SetIcon(m_IconWriteable);

	m_pGeneralDataMan->ClearAllDataTemplItems();
	if ( !m_pGeneralDataMan->Load(path) )
	{
		CString msg;
		msg.Format("不能打开 %s", path);
		MessageBox(msg);
	}
	m_treeDataMan.m_bDataReadOnly = m_bReadOnly;
	RefreshTreeList();
}

void CGeneralDataManDlg::OnCheckIn() 
{
	//
	if ( m_bReadOnly )
	{
		return;
	}
	
	if ( IDYES == MessageBox("你需要存盘吗？","存盘",MB_YESNO|MB_ICONQUESTION) )
	{
		m_pGeneralDataMan->Save( "GeneralData\\" + m_strDataFileName );
	}
	
	CString path;
	path.Format("GeneralData\\%s",m_strDataFileName);
	g_VSS.SetProjectPath("GeneralData");
	g_VSS.CheckInFile(m_strDataFileName);
	if ( FileIsReadOnly(AString(path)) )
	{
		m_bReadOnly = true;
		m_staticIcon.SetIcon(m_IconReadOnly);
	}
	else
	{ 
		m_bReadOnly = false;
	}
	m_treeDataMan.m_bDataReadOnly = m_bReadOnly;
}



void CGeneralDataManDlg::OnMoveLast() 
{
	//
	m_treeDataMan.MoveUp();
	
}

void CGeneralDataManDlg::OnMoveDown() 
{
	//
	m_treeDataMan.MoveDown();

}

void CGeneralDataManDlg::OnExportAttribute() 
{
	//
	BROWSEINFO Info;
	memset(&Info, 0, sizeof (Info));

	static char szPath[MAX_PATH] = "\0";
	
	Info.hwndOwner		= m_hWnd;
	Info.pidlRoot		= NULL;
	Info.pszDisplayName	= szPath;
	Info.lpszTitle		= "请选择导出数据的存放路径：";
	Info.ulFlags		= BIF_RETURNONLYFSDIRS | BIF_STATUSTEXT | BIF_DONTGOBELOWDOMAIN ;
	
	LPITEMIDLIST pPIDL;
	if ( !(pPIDL = SHBrowseForFolder(&Info)) )
	{
		return;
	}

	SHGetPathFromIDList(pPIDL, szPath);	

	sprintf(szPath,"%s\\DataAttribute.txt",szPath);
	if ( !m_pGeneralDataMan->ExportAllItemAttribute( szPath ) )
	{
		AfxMessageBox( "数据属性导出失败！" );
	}
}
