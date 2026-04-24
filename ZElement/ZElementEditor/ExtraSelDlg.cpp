// ExtraSelDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "ExtraSelDlg.h"
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

/////////////////////////////////////////////////////////////////////////////
// CExtraSelDlg dialog


CExtraSelDlg::CExtraSelDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExtraSelDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExtraSelDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_nExtraAreaID = 0;
	m_nObjectType = -1;
}


void CExtraSelDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExtraSelDlg)
	DDX_Control(pDX, IDC_LIST_SEL_EXTRA, m_listSelExtra);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExtraSelDlg, CDialog)
	//{{AFX_MSG_MAP(CExtraSelDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExtraSelDlg message handlers

void CExtraSelDlg::OnOK()
{
	int sel = m_listSelExtra.GetCurSel();
	if(sel!=-1)
	{
		m_nExtraAreaID = m_listSelExtra.GetItemData(sel);
	}
	CDialog::OnOK();
}

void CExtraSelDlg::UpdateList()
{
	int n = m_listSelExtra.GetCount();
	for( int i = 0; i < n; ++i)
		m_listSelExtra.DeleteString(0);
	
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		int index = 0;
		int cur = 0;
		CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
		APtrList<CSceneObject*>* listAiArea = pManager->GetSortObjectList(m_nObjectType);
		ALISTPOSITION pos = listAiArea->GetHeadPosition();
		while(pos)
		{
			if(m_nObjectType==CSceneObject::SO_TYPE_AIAREA)
			{
				CSceneAIGenerator *pAi = (CSceneAIGenerator*)listAiArea->GetNext(pos);
				if(pAi->IsExtraArea())
				{
					int sel = m_listSelExtra.InsertString(index++, pAi->GetObjectName());
					if( m_nExtraAreaID == pAi->GetObjectID()) cur = sel;
					m_listSelExtra.SetItemData(sel,pAi->GetObjectID());
				}
			}else if(m_nObjectType==CSceneObject::SO_TYPE_FIXED_NPC)
			{
				CSceneFixedNpcGenerator *pAi = (CSceneFixedNpcGenerator*)listAiArea->GetNext(pos);
				if(pAi->IsExtraArea())
				{
					int sel = m_listSelExtra.InsertString(index++, pAi->GetObjectName());
					if( m_nExtraAreaID == pAi->GetObjectID()) cur = sel;
					m_listSelExtra.SetItemData(sel,pAi->GetObjectID());
				}
			}else if( m_nObjectType==CSceneObject::SO_TYPE_GATHER)
			{
				CSceneGatherObject *pAi = (CSceneGatherObject*)listAiArea->GetNext(pos);
				if(pAi->IsExtraArea())
				{
					int sel = m_listSelExtra.InsertString(index++, pAi->GetObjectName());
					if( m_nExtraAreaID == pAi->GetObjectID()) cur = sel;
					m_listSelExtra.SetItemData(sel,pAi->GetObjectID());
				}
			}
		}
		m_listSelExtra.SetCurSel(cur);
	}
}

BOOL CExtraSelDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	UpdateList();
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
