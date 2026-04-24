// ExtraAreaManDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "ExtraAreaManDlg.h"
#include "ExtraSelDlg.h"
#include "SceneGatherObject.h"

#include "ElementMap.h"
#include "SceneAIGenerator.h"
#include "SceneObjectManager.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CExtraAreaManDlg dialog


CExtraAreaManDlg::CExtraAreaManDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtraAreaManDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtraAreaManDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pSceneAiObject = 0;
	m_nObjectType = -1;
}


void CExtraAreaManDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtraAreaManDlg)
	DDX_Control(pDX, IDC_LIST_EXTRA, m_listExtra);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtraAreaManDlg, CDialog)
	//{{AFX_MSG_MAP(CExtraAreaManDlg)
	ON_BN_CLICKED(IDC_ADD_EXTRA, OnAddExtra)
	ON_BN_CLICKED(IDC_DEL_EXTRA, OnDelExtra)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtraAreaManDlg message handlers

void CExtraAreaManDlg::OnAddExtra() 
{
	// TODO: Add your control notification handler code here
	CExtraSelDlg dlg;
	dlg.Init(m_nObjectType);
	if(IDOK==dlg.DoModal())
	{
		bool bAdd = true;
		if(m_nObjectType==CSceneObject::SO_TYPE_AIAREA)
		{
			CSceneAIGenerator *pTemp = (CSceneAIGenerator*)m_pSceneAiObject;
			int n = pTemp->GetExtraAreaNum();	
			for( int i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				if(id==dlg.m_nExtraAreaID) 
				{
					bAdd = false;
					break;
				}
			}
			if(bAdd && dlg.m_nExtraAreaID != 0) pTemp->AddExtra(dlg.m_nExtraAreaID);
		}else if(m_nObjectType==CSceneObject::SO_TYPE_FIXED_NPC)
		{
			CSceneFixedNpcGenerator *pTemp = (CSceneFixedNpcGenerator*)m_pSceneAiObject;
			int n = pTemp->GetExtraAreaNum();	
			for( int i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				if(id==dlg.m_nExtraAreaID) 
				{
					bAdd = false;
					break;
				}
			}
			if(bAdd && dlg.m_nExtraAreaID != 0) pTemp->AddExtra(dlg.m_nExtraAreaID);
		}else if(m_nObjectType==CSceneObject::SO_TYPE_GATHER)
		{
			CSceneGatherObject *pTemp = (CSceneGatherObject*)m_pSceneAiObject;
			int n = pTemp->GetExtraAreaNum();	
			for( int i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				if(id==dlg.m_nExtraAreaID) 
				{
					bAdd = false;
					break;
				}
			}
			if(bAdd && dlg.m_nExtraAreaID != 0) pTemp->AddExtra(dlg.m_nExtraAreaID);
		}

		UpdateList();
	}
}

void CExtraAreaManDlg::OnDelExtra() 
{
	// TODO: Add your control notification handler code here
	int sel = m_listExtra.GetCurSel();
	if(sel==-1) return;
	if(m_nObjectType==CSceneObject::SO_TYPE_AIAREA) ((CSceneAIGenerator*)m_pSceneAiObject)->DelExtra(m_listExtra.GetItemData(sel));
	if(m_nObjectType==CSceneObject::SO_TYPE_FIXED_NPC) ((CSceneFixedNpcGenerator*)m_pSceneAiObject)->DelExtra(m_listExtra.GetItemData(sel));
	if(m_nObjectType==CSceneObject::SO_TYPE_GATHER) ((CSceneGatherObject*)m_pSceneAiObject)->DelExtra(m_listExtra.GetItemData(sel));
	UpdateList();
}

BOOL CExtraAreaManDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here

	UpdateList();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CExtraAreaManDlg::UpdateList()
{
	int n = m_listExtra.GetCount();
	int i;
	for( i = 0; i < n; ++i)
		m_listExtra.DeleteString(0);

	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		if(m_nObjectType==CSceneObject::SO_TYPE_AIAREA)
		{
			CSceneAIGenerator *pTemp = (CSceneAIGenerator*)m_pSceneAiObject;
			n = pTemp->GetExtraAreaNum();	
			for( i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				CSceneAIGenerator* pExtra = (CSceneAIGenerator*)pManager->FindAiAreaObject(id);
				if(pExtra)
				{
					int sel = m_listExtra.AddString(pExtra->GetObjectName());
					m_listExtra.SetItemData(sel,id);
				}
			}
		}else if(m_nObjectType==CSceneObject::SO_TYPE_FIXED_NPC)
		{
			CSceneFixedNpcGenerator *pTemp = (CSceneFixedNpcGenerator*)m_pSceneAiObject;
			n = pTemp->GetExtraAreaNum();	
			for( i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				CSceneFixedNpcGenerator* pExtra = (CSceneFixedNpcGenerator*)pManager->FindNpcObject(id);
				if(pExtra)
				{
					int sel = m_listExtra.AddString(pExtra->GetObjectName());
					m_listExtra.SetItemData(sel,id);
				}
			}
			
		}else if(m_nObjectType==CSceneObject::SO_TYPE_GATHER)
		{
			CSceneGatherObject *pTemp = (CSceneGatherObject*)m_pSceneAiObject;
			n = pTemp->GetExtraAreaNum();	
			for( i = 0; i < n; ++i)
			{
				int id = pTemp->GetExtraAreaID(i);
				CSceneGatherObject* pExtra = (CSceneGatherObject*)pManager->FindGatherObject(id);
				if(pExtra)
				{
					int sel = m_listExtra.AddString(pExtra->GetObjectName());
					m_listExtra.SetItemData(sel,id);
				}
			}
		}
	}
	
}

void CExtraAreaManDlg::OnOK()
{
}
