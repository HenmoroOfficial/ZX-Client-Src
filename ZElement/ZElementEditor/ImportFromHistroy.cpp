// ImportFromHistroy.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "ImportFromHistroy.h"
#include "ElementMap.h"
#include "MainFrm.h"
#include "SceneObjectManager.h"
#include "SceneObject.h"
#include "SceneAIGenerator.h"
#include "SceneDynamicObject.h"
#include "SceneGatherObject.h"
#include "SceneInstanceBox.h"
#include "ScenePrecinctObject.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CImportFromHistroy dialog


CImportFromHistroy::CImportFromHistroy(CWnd* pParent /*=NULL*/)
	: CDialog(CImportFromHistroy::IDD, pParent)
{
	//{{AFX_DATA_INIT(CImportFromHistroy)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pHisSceneObjectMan = NULL;
}

CImportFromHistroy::~CImportFromHistroy()
{
	if(m_pHisSceneObjectMan)
	{
		m_pHisSceneObjectMan->Release();
		delete m_pHisSceneObjectMan;
		m_pHisSceneObjectMan = NULL;
	}
}

void CImportFromHistroy::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CImportFromHistroy)
	DDX_Control(pDX, IDC_NPC_INFO_LIST, m_NPCInfoList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CImportFromHistroy, CDialog)
	//{{AFX_MSG_MAP(CImportFromHistroy)
	ON_BN_CLICKED(IDC_IMPORT_SCENE, OnImportScene)
	ON_BN_CLICKED(IDC_COMPARE_WITH_CURR, OnCompareWithCurr)
	ON_BN_CLICKED(ID_REFRESH, OnRefreshCurrScene)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CImportFromHistroy message handlers

BOOL CImportFromHistroy::OnInitDialog() 
{
	CDialog::OnInitDialog();	

	m_NPCInfoList.InsertColumn(0, "索引", LVCFMT_LEFT, 40);
	m_NPCInfoList.InsertColumn(1, "对象名", LVCFMT_LEFT, 150);
	m_NPCInfoList.InsertColumn(2, "国内版本套数", LVCFMT_RIGHT, 100);
	m_NPCInfoList.InsertColumn(3, "海外版本套数", LVCFMT_RIGHT, 100);
	
	m_NPCInfoList.SetExtendedStyle(m_NPCInfoList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	

//	ListView_SetExtendedListViewStyle(m_NPCInfoList.m_hWnd, m_NPCInfoList.GetStyle() | LVS_EX_GRIDLINES);
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CImportFromHistroy::OnImportScene() 
{
	char szFilter[] = "Element Editor Scene Files (*.scene)|*.scene||";
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, "*.scene", NULL, dwFlags, szFilter, NULL);

	if (FileDia.DoModal() == IDCANCEL)
		return;

	GetDlgItem(IDC_EDIT_HISTROY_SCENE)->SetWindowText(FileDia.GetPathName());		
}

// 加载指定的scene文件，并与国内版本的scene文件进行对比，对比结果输出到m_NPCInfoList
void CImportFromHistroy::OnCompareWithCurr() 
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL)
	{
		MessageBox("请先载入要更新的国内版本的场景！");
		return; 
	}

	CString strFile;
	GetDlgItem(IDC_EDIT_HISTROY_SCENE)->GetWindowText(strFile);
	if(strFile.GetLength() == 0)
	{
		MessageBox("请输入海外版本的场景文件所在路径！");
		return;
	}
	char str[256];
	memset(str, 0, sizeof(char)*256);
	strncpy(str, strFile, strFile.GetLength()+1);

	AFile File;
	if (!File.Open(str, AFILE_OPENEXIST))
	{
		MessageBox("无法打开海外版本的场景文件！");
		return;
	}

	if(m_pHisSceneObjectMan)
	{
		m_pHisSceneObjectMan->Release();
		delete m_pHisSceneObjectMan;
		m_pHisSceneObjectMan = NULL;
	}
	
	m_pHisSceneObjectMan = new CSceneObjectManager(NULL);
	if(!m_pHisSceneObjectMan)
		return;

	CELFileArchive ar(&File, false);
	bool bLoad = m_pHisSceneObjectMan->LoadAllObject(ar, LOAD_HISTORY_COMPARE);
	if(!bLoad)
	{
		MessageBox("加载海外版本场景对象错误！");
		return;
	}
	
	CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();

	// fixed npc
	APtrList<CSceneObject *>* al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
	ALISTPOSITION pos = al_his->GetTailPosition();
	int i = 0;
	while( pos )
	{
		CSceneFixedNpcGenerator* pHisObj = (CSceneFixedNpcGenerator*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneFixedNpcGenerator* pCurrObj = (CSceneFixedNpcGenerator*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}

	// AiGenerator
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_AIAREA);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneAIGenerator* pHisObj = (CSceneAIGenerator*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneAIGenerator* pCurrObj = (CSceneAIGenerator*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}

	// Gather
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_GATHER);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneGatherObject* pHisObj = (CSceneGatherObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneGatherObject* pCurrObj = (CSceneGatherObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}

	// Dynamic
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_DYNAMIC);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneDynamicObject* pHisObj = (CSceneDynamicObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneDynamicObject* pCurrObj = (CSceneDynamicObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}

	// Range
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_RANGE);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneRangeObject* pHisObj = (CSceneRangeObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneRangeObject* pCurrObj = (CSceneRangeObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}

	// InstanceBox
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_INSTANCE_BOX);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		SceneInstanceBox* pHisObj = (SceneInstanceBox*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		SceneInstanceBox* pCurrObj = (SceneInstanceBox*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数相同，不需要显示
		if(pCurrObj->GetCopyFlags() == pHisObj->GetCopyFlags())
			continue;
				
		// 索引
		CString strNum;
		strNum.Format("%d", i+1);
		int iItem = m_NPCInfoList.InsertItem(i, strNum);
		i++;

		// 对象名
		m_NPCInfoList.SetItemText(iItem, 1, strName);

		// 国内套数
		strNum.Format("%d", pCurrObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 2, strNum);

		// 海外套数
		strNum.Format("%d", pHisObj->GetCopyFlags());
		m_NPCInfoList.SetItemText(iItem, 3, strNum);
	}



}

// 从海外版本的scene文件更新国内版本
void CImportFromHistroy::OnRefreshCurrScene() 
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL)
	{
		MessageBox("请先载入要更新的最新场景！");
		return; 
	}

	if (!m_pHisSceneObjectMan)
	{
		MessageBox("请先加载海外版本的场景文件并进行对比！");
		return;
	}
	
	CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
	int iCount = 0;

	// fixed npc
	APtrList<CSceneObject *>* al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
	ALISTPOSITION pos = al_his->GetTailPosition();
	int i = 0;
	while( pos )
	{
		CSceneFixedNpcGenerator* pHisObj = (CSceneFixedNpcGenerator*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneFixedNpcGenerator* pCurrObj = (CSceneFixedNpcGenerator*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}

	// AiGenerator
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_AIAREA);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneAIGenerator* pHisObj = (CSceneAIGenerator*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneAIGenerator* pCurrObj = (CSceneAIGenerator*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}

	// Gather
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_GATHER);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneGatherObject* pHisObj = (CSceneGatherObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneGatherObject* pCurrObj = (CSceneGatherObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}

	// Dynamic
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_DYNAMIC);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneDynamicObject* pHisObj = (CSceneDynamicObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneDynamicObject* pCurrObj = (CSceneDynamicObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}

	// Range
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_RANGE);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		CSceneRangeObject* pHisObj = (CSceneRangeObject*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		CSceneRangeObject* pCurrObj = (CSceneRangeObject*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}

	// InstanceBox
	al_his =  m_pHisSceneObjectMan->GetSortObjectList(CSceneObject::SO_TYPE_INSTANCE_BOX);
	pos = al_his->GetTailPosition();
	while( pos )
	{
		SceneInstanceBox* pHisObj = (SceneInstanceBox*)al_his->GetPrev(pos);
		CString strName;
		strName = pHisObj->GetObjectName();

		// 国内场景中的对象
		SceneInstanceBox* pCurrObj = (SceneInstanceBox*)pSManager->FindSceneObject((AString)strName);
		if(!pCurrObj)
			continue;

		// 国内套数与海外套数不相同，更新国内的套数
		if(pCurrObj->GetCopyFlags() != pHisObj->GetCopyFlags())
		{
			pCurrObj->SetCopyFlags(pHisObj->GetCopyFlags());
			iCount++;
		}
	}	

	CString strFinish;
	strFinish.Format("国内版本场景对象套数信息更新完毕, 共更新了%d个场景对象的套数！", iCount);
	MessageBox(strFinish);
}
