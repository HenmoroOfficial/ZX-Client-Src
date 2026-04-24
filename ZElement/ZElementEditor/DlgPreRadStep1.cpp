// PreRadiosityStep1.cpp: implementation of the CPreRadiosityStep1 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "ElementMap.h"
#include "DlgPreRadStep1.h"


BEGIN_MESSAGE_MAP(CDlgPreRadStep1, CDialog)
	//{{AFX_MSG_MAP(CNpcSelDlg)
	ON_BN_CLICKED(IDC_RADIO1, OnRadio1)
	ON_BN_CLICKED(IDC_RADIO2, OnRadio2)
	ON_BN_CLICKED(IDC_RADIO3, OnRadio3)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

PRERADSTEP1_CONFIG	CDlgPreRadStep1::s_Config;//s_PreRadStepConfig;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgPreRadStep1::CDlgPreRadStep1(CElementMap* pMap, PRERADSTEP1_CONFIG* pConfig)
: CDialog(CDlgPreRadStep1::IDD, NULL)
, m_pMap(pMap)
, m_pCurConfig(pConfig == NULL ? &s_Config : pConfig)
{

}

CDlgPreRadStep1::~CDlgPreRadStep1()
{

}

//void CDlgPreRadStep1::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//
// 	DDX_Control(pDX, IDC_RADIO1, m_Radio1);
// 	DDX_Control(pDX, IDC_RADIO2, m_Radio2);
// 	DDX_Control(pDX, IDC_RADIO3, m_Radio3);
// 
// 	DDX_Control(pDX, IDC_CHECK1, m_Check1);
// 	DDX_Control(pDX, IDC_CHECK2, m_Check2);
//}

BOOL CDlgPreRadStep1::OnInitDialog()
{
	const CString strLitmapDir = CString(g_szWorkDir) + g_szEditMapDir + "\\" + m_pMap->GetMapName() + "\\LitData";

	CButton*	pRadio1 = (CButton*)GetDlgItem(IDC_RADIO1);	// 顶点光照
	CButton*	pRadio2 = (CButton*)GetDlgItem(IDC_RADIO2);	// 1/4光照图
	CButton*	pRadio3 = (CButton*)GetDlgItem(IDC_RADIO3);	// 最终

	CButton*	pCheck1 = (CButton*)GetDlgItem(IDC_CHECK1);	// 重新导出光照图场景
	CButton*	pCheck2 = (CButton*)GetDlgItem(IDC_CHECK2);	// 全部重新计算光照图

	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, m_pCurConfig->emidGenLitmap);

	if(m_pCurConfig->bExportNewScene != FALSE)
		pCheck1->SetCheck(BST_CHECKED);

	if(m_pCurConfig->bDeleteOldLitmap != FALSE)
		pCheck2->SetCheck(BST_CHECKED);

	// 创建场景使用的光照图目录
	CreateDirectory(strLitmapDir, NULL);

	if(m_pCurConfig->bReadOnly != FALSE)
	{
		pRadio1->EnableWindow(FALSE);
		pRadio2->EnableWindow(FALSE);
		pRadio3->EnableWindow(FALSE);

		pCheck1->EnableWindow(FALSE);
		pCheck2->EnableWindow(FALSE);
	}

	return TRUE;
}

void CDlgPreRadStep1::OnOK()
{
RETRY_DELSCENECACHE:
	if(m_pCurConfig->bExportNewScene != FALSE || CheckExportScene())
	{
		m_Param.bExportModel = TRUE;
		if(DeleteLitmapSceneCache() == FALSE)
		{
			if(MessageBox("删除光照图计算场景失败!", "错误", MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
				goto RETRY_DELSCENECACHE;
			return;
		}
		else
		{
RETRY_CREATESCENECACHEDIR:
			if(CreateLitmapSceneCacheDir() == FALSE)
			{
				if(MessageBox("创建缓存目录失败!", "错误", MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
					goto RETRY_CREATESCENECACHEDIR;
				return;
			}
		}
	}
	else
		m_Param.bExportModel = FALSE;


RETRY_DELLITMAPCACHE:
	if(m_pCurConfig->bDeleteOldLitmap != FALSE)
	{
		if(DeleteLitmapCache() == FALSE)
		{
			if(MessageBox("删除原有光照图失败!", "错误", MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
				goto RETRY_DELLITMAPCACHE;
			return;
		}
	}

	m_pCurConfig->bDeleteOldLitmap = FALSE;	// 清除掉这个标志

	switch(m_pCurConfig->emidGenLitmap)
	{
	case IDC_RADIO1:
		m_Param.emLitmapQuality = PRSTEP1_PARAM::VertLit;
		break;
	case IDC_RADIO2:
		m_Param.emLitmapQuality = PRSTEP1_PARAM::QuadLitmap;
		break;
	case IDC_RADIO3:
		m_Param.emLitmapQuality = PRSTEP1_PARAM::Product;
		break;
	}

	CDialog::OnOK();
}

void CDlgPreRadStep1::OnCancel()
{
	CDialog::OnCancel();
}

afx_msg void CDlgPreRadStep1::OnRadio1()
{
	m_pCurConfig->emidGenLitmap = IDC_RADIO1;
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, m_pCurConfig->emidGenLitmap);	
}

afx_msg void CDlgPreRadStep1::OnRadio2()
{
	m_pCurConfig->emidGenLitmap = IDC_RADIO2;
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, m_pCurConfig->emidGenLitmap);
}

afx_msg void CDlgPreRadStep1::OnRadio3()
{
	m_pCurConfig->emidGenLitmap = IDC_RADIO3;
	CheckRadioButton(IDC_RADIO1, IDC_RADIO3, m_pCurConfig->emidGenLitmap);
}

afx_msg void CDlgPreRadStep1::OnCheck1()	// 重新导出光照图场景
{
	CButton*	pCheck1 = (CButton*)GetDlgItem(IDC_CHECK1);
	m_pCurConfig->bExportNewScene = (pCheck1->GetCheck() == BST_CHECKED);
}

afx_msg void CDlgPreRadStep1::OnCheck2()	// 全部重新计算光照图
{
	CButton*	pCheck2 = (CButton*)GetDlgItem(IDC_CHECK2);
	m_pCurConfig->bDeleteOldLitmap = (pCheck2->GetCheck() == BST_CHECKED);
}

BOOL CDlgPreRadStep1::CheckExportScene()
{
	const CString strLitmapSceneConfig = CString(g_szWorkDir) + "LitmapCache\\LitmapScene.txt";

	char szSceneCode[1024], szSceneCodeFromFile[1024];
	wsprintf(szSceneCode, "%s\\%s", g_szEditMapDir, m_pMap->GetMapName());

	FILE* fp = fopen(strLitmapSceneConfig, "rb");
	if(fp != NULL)
	{
		fgets(szSceneCodeFromFile, 1024, fp);
		fclose(fp);
	}
	if(fp == 0 || strcmp(szSceneCode, szSceneCodeFromFile) != 0)
	{
		return TRUE;
	}
	return FALSE;
}

BOOL CDlgPreRadStep1::DeleteLitmapSceneCache()
{
	const CString strLitmapCache = CString(g_szWorkDir) + "LitmapCache";
	const CString strDayPath     = strLitmapCache + "\\Day";
	const CString strNightPath   = strLitmapCache + "\\Night";

	WIN32_FIND_DATA FindFileData;

	HANDLE hFindFile = FindFirstFile(strLitmapCache, &FindFileData);
	if(hFindFile != INVALID_HANDLE_VALUE && FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFindFile);

		char buffer[MAX_PATH + 1];
		memset(buffer, 0, sizeof(buffer));

		SHFILEOPSTRUCT	shfos;
		strcpy(buffer, strLitmapCache);

		shfos.hwnd					= NULL;
		shfos.wFunc					= FO_DELETE;
		shfos.pFrom					= buffer;
		shfos.pTo					= NULL;
		shfos.fFlags				= FOF_NOCONFIRMATION;
		shfos.fAnyOperationsAborted = NULL;
		shfos.hNameMappings			= NULL;
		shfos.lpszProgressTitle		= "删除光照图缓存目录";
		return SUCCEEDED(SHFileOperation(&shfos));
	}
	return TRUE;
}

BOOL CDlgPreRadStep1::DeleteLitmapCache()
{
	const CString strLitmapDir = CString(g_szWorkDir) + g_szEditMapDir + "\\" + m_pMap->GetMapName() + "\\LitData";
	const CString strLitmapPath = strLitmapDir + "\\*.*";

	WIN32_FIND_DATA FindFileData;

	HANDLE hFindFile = FindFirstFile(strLitmapDir, &FindFileData);

	if(hFindFile != INVALID_HANDLE_VALUE && FindFileData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
	{
		FindClose(hFindFile);

		char buffer[MAX_PATH + 1];
		memset(buffer, 0, sizeof(buffer));

		SHFILEOPSTRUCT	shfos;
		strcpy(buffer, strLitmapPath);

		shfos.hwnd					= NULL;
		shfos.wFunc					= FO_DELETE;
		shfos.pFrom					= buffer;
		shfos.pTo					= NULL;
		shfos.fFlags				= FOF_NOCONFIRMATION;
		shfos.fAnyOperationsAborted = NULL;
		shfos.hNameMappings			= NULL;
		shfos.lpszProgressTitle		= "删除光照图文件";
		return SUCCEEDED(SHFileOperation(&shfos));
	}
	return TRUE;
}

BOOL CDlgPreRadStep1::CreateLitmapSceneCacheDir()
{
	const CString strLitmapCache = CString(g_szWorkDir) + "LitmapCache";
	const CString strDayPath     = strLitmapCache + "\\Day";
	const CString strNightPath   = strLitmapCache + "\\Night";

	if( CreateDirectory(strLitmapCache, NULL) == FALSE || 
		CreateDirectory(strDayPath, NULL) == FALSE ||
		CreateDirectory(strNightPath, NULL) == FALSE )
	{
		return FALSE;
	}

	// 输出用来区别光照图场景和当前编辑地图的标识文件
	CString strXML = CString(g_szWorkDir) + "LitmapCache\\LitmapScene.txt";
	FILE* fp = fopen(strXML, "wb");
	if(fp == NULL)
		return FALSE;

	fprintf(fp, "%s\\%s", g_szEditMapDir, m_pMap->GetMapName());
	fclose(fp);

	return TRUE;
}
