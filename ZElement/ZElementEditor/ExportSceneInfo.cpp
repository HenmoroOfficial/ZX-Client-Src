// ExportSceneInfo.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "ExportSceneInfo.h"
#include "ElementMap.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include "SceneAIGenerator.h"
#include "SceneGatherObject.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//NPC:
//数据类型\地图号\NPC名字\选择NPC\NPC id\数据套数
//（例如npc\z1\2010活动怪\魔定点精英\100001\9）
const char _npc_info[] = "NPC\\%s\\%s\\%s\\%d\\%d";


/////////////////////////////////////////////////////////////////////////////
// CExportSceneInfo dialog


CExportSceneInfo::CExportSceneInfo(CWnd* pParent /*=NULL*/)
	: CDialog(CExportSceneInfo::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportSceneInfo)
	m_bExportNPC = FALSE;
	m_bExportGather = FALSE;
	m_strPath = _T("D:\\");
	//}}AFX_DATA_INIT
}


void CExportSceneInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportSceneInfo)
	DDX_Check(pDX, IDC_CHECK_EXPORT_NPC, m_bExportNPC);
	DDX_Check(pDX, IDC_CHECK_EXPORT_GATHER, m_bExportGather);
	DDX_Text(pDX, IDC_EXPORT_PATH, m_strPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportSceneInfo, CDialog)
	//{{AFX_MSG_MAP(CExportSceneInfo)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportSceneInfo message handlers

void CExportSceneInfo::OnButtonOpen() 
{
	char szPath[256];
	CString str;

	ZeroMemory(szPath, sizeof(szPath));

	BROWSEINFO bi;
	bi.hwndOwner = m_hWnd;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szPath;
	bi.lpszTitle = "请选择要导出的txt的存放目录";
	bi.ulFlags = 0;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	// 弹出选择目录对话框
	LPITEMIDLIST lp = SHBrowseForFolder(&bi);
	
	if(lp && SHGetPathFromIDList(lp, szPath))
	{
		m_strPath.Format("%s", szPath);
		UpdateData(FALSE);
	}
	else
	{
		AfxMessageBox("无效的目录，请重新选择");
	}
	
}

void CExportSceneInfo::OnOK() 
{
	if(m_strPath.GetLength() == 0)
	{
		AfxMessageBox("请选择一个导出目录");
		return;
	}

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL)
	{
		MessageBox("请先载入场景！");
		return; 
	}

	char szName[256];
	ZeroMemory(szName, sizeof(szName));
	strncpy(szName, m_strPath, m_strPath.GetLength()+1);
	if(szName[m_strPath.GetLength()] != '\\')
		strcat(szName, "\\");

	const char* szMapFile = strstr(pMap->GetResDir(), "\\");
	szMapFile = szMapFile+1;
	const char* szMapFile2 = strstr(szMapFile, "\\");
	int iMapNameLength = szMapFile2-szMapFile;

	char szMapName[32];
	ZeroMemory(szMapName, sizeof(szMapName));
	strncpy(szMapName, szMapFile, iMapNameLength);

	strcat(szName, szMapName);
	strcat(szName, ".txt");

	FILE* fp = fopen(szName, "w");
	if (!fp)
	{
		AfxMessageBox("无法保存文件");
		return;
	}

	UpdateData(TRUE);	

	CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();

	// NPC
	if(m_bExportNPC)
	{
		APtrList<CSceneObject *>* al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
		ALISTPOSITION pos = al->GetTailPosition();
		while( pos )
		{
			CSceneFixedNpcGenerator* pObj = (CSceneFixedNpcGenerator*)al->GetPrev(pos);
			
			fprintf(fp, "NPC\\%s\\%s\\%s\\%d\\%d", szMapName, pObj->GetObjectName(), pObj->GetNPCName(pObj->GetProperty().dwID), pObj->GetProperty().dwID, pObj->GetCopyFlags());
			fprintf(fp, "\r\n");
		}
	}

	if(m_bExportGather)
	{
		APtrList<CSceneObject *>* al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_GATHER);
		ALISTPOSITION pos = al->GetTailPosition();
		while( pos )
		{
			CSceneGatherObject* pObj = (CSceneGatherObject*)al->GetPrev(pos);
			
			if(pObj->GetGatherNum() > 0)
			{
				fprintf(fp, "可采集对象\\%s\\%s\\%s\\%d\\%d",szMapName, pObj->GetObjectName(), pObj->GetGather(0)->strName, pObj->GetGather(0)->dwId, pObj->GetCopyFlags());
				fprintf(fp, "\r\n");
			}
		}	
	}

	fclose(fp);

	CString notify;
	notify.Format("信息已成功导出到 %s!", szName);
	AfxMessageBox(notify);
}
