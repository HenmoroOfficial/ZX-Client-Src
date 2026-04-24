// SceneObjectPanel.cpp : implementation file
//

#include "stdafx.h"
#include "Global.h"
#include "MainFrm.h"
#include "elementeditor.h"
#include "SceneObjectPanel.h"
#include "ObjectPage.h"
#include "OperationManager.h"
#include "SkySetDialog.h"
#include "SceneObjectManager.h"
#include "SceneLightAddOperation.h"
#include "SceneWaterAddOperation.h"
#include "SceneAreaAddOperation.h"
#include "BezierAddOperation.h"
#include "AudioAddOperation.h"
#include "SceneModelAddOperation.h"
#include "SceneDummyAddOperation.h"
#include "SpeciallyAddOperation.h"
#include "AIGeneratorOperation.h"
#include "CommonFileDlg.h"
#include "Render.h"
#include "RollUpPanel.h"


//#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CSceneObjectPanel dialog


CSceneObjectPanel::CSceneObjectPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CSceneObjectPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSceneObjectPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSceneObjectPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSceneObjectPanel)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSceneObjectPanel, CDialog)
	//{{AFX_MSG_MAP(CSceneObjectPanel)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_LIGHT, OnSceneObjectLight)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_MODEL, OnSceneObjectModel)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_SKYBOX, OnSceneObjectSkybox)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_WATER, OnSceneObjectWater)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_AREA, OnSceneObjectArea)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_PATH, OnSceneObjectPath)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_AUDIO, OnSceneObjectAudio)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_AI, OnSceneObjectAi)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_VERNIER, OnSceneObjectVernier)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_DUMMY, OnSceneObjectDummy)
	ON_BN_CLICKED(SCENE_OBJECT_MODEL_RELOAD, OnObjectModelReload)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_SKINMODEL, OnSceneObjectSkinmodel)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_CRITTER, OnSceneObjectCritter)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_FIXNPC, OnSceneObjectFixnpc)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_PRECINCT, OnSceneObjectPrecinct)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_GATHER, OnSceneObjectGather)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_RANGE, OnSceneObjectRange)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_CLOUD_SET, OnSceneObjectCloudSet)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_CLOUD_BOX, OnSceneObjectCloudBox)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_BOX_INSTANCE, OnSceneObjectBoxInstance)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_DYNAMIC, OnSceneObjectDynamic)
	ON_BN_CLICKED(IDC_SCENE_OBJECT_BUFF_REGION, OnSceneObjectBuffRegion)
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_SCENE_OBJECT_GUILDBUILDING, &CSceneObjectPanel::OnBnClickedSceneObjectGuildbuilding)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSceneObjectPanel message handlers

//该对话框所有的按纽都在这儿同时设置Check属性，以后加上去的按纽
//也最好在这儿加入，防止造成管理上的混乱。
void CSceneObjectPanel::CheckOtherButtonCheck(CButton *pButton)
{
	CButton *pTemp;

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_LIGHT);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_MODEL);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKYBOX);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_WATER);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AREA);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PATH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AUDIO);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AI);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	//button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_VERNIER);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DUMMY);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKINMODEL);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CRITTER);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_FIXNPC);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PRECINCT);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GATHER);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_RANGE);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_SET);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_BOX);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BOX_INSTANCE);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DYNAMIC);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BUFF_REGION);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GUILDBUILDING);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}	
}

void CSceneObjectPanel::RiseUpAllButton()
{
	//把所有的按纽都恢复
	CButton *pTemp;
	//Button
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_LIGHT);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_MODEL);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKYBOX);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_WATER);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AREA);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PATH);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AUDIO);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AI);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_VERNIER);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DUMMY);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKINMODEL);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CRITTER);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_FIXNPC);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PRECINCT);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GATHER);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_RANGE);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_SET);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_BOX);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BOX_INSTANCE);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DYNAMIC);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BUFF_REGION);
	if(pTemp) pTemp->SetCheck(0);
	pTemp = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GUILDBUILDING);
	if(pTemp) pTemp->SetCheck(0);
	
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame)
	{
		pMainFrame->SetOtherOperation(false);
	}
}

void CSceneObjectPanel::OnSceneObjectLight() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_LIGHT);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
			
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_LIGHT_ADD);
					((CSceneLightAddOperation*)pOperation)->CreateLight();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}




void CSceneObjectPanel::OnSceneObjectModel() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_MODEL);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
			
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowListPlanel(true);
				pParent->ShowPanel(CObjectsPage::PANEL_MODEL_LIST_OBJECT,true);
				//这是全场唯一不需要先加入操作的地方
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
		}
	}
}


//这儿是加特效的按纽按下对应的消息
void CSceneObjectPanel::OnSceneObjectSkybox() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKYBOX);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				AUX_GetMainFrame()->GetToolTabWnd()->UpdateSkyBoxPanel();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_SPECIALLY_ADD);
					((CSpeciallyAddOperation*)pOperation)->CreateSpecially();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectWater() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_WATER);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
			
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_WATER_ADD);
					((CSceneWaterAddOperation*)pOperation)->CreateWater();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectArea() 
{
	// TODO: Add your control notification handler code here

	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AREA);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_AREA_ADD);
					((CSceneAreaAddOperation*)pOperation)->CreateArea();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectPath() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PATH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_ADD);
					((CBezierAddOperation*)pOperation)->CreateBezier();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectAudio() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AUDIO);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_AUDIO_ADD);
					((CAudioAddOperation*)pOperation)->CreateAudio();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectAi() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_AI);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPEAATION_SCENE_AIGENERATOR_ADD);
					((CAIGeneratorOperation*)pOperation)->CreateAI();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectVernier() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_VERNIER);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				pParent->ShowPanel(CObjectsPage::PANEL_VERNIER_OPT,true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				pMainFrame->GetToolTabWnd()->UpdateVernierDlg();
				pMainFrame->GetToolTabWnd()->RiseUpVernierButton();
				pMainFrame->SetOtherOperation(true);
				CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
				if(pViewFrame) pViewFrame->SetCurrentOperation(NULL);
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectDummy() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DUMMY);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_DUMMY_ADD);
					((CSceneDummyAddOperation*)pOperation)->CreateDummy();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

//这儿把所有的模型重新加载一次，保留原来的任何变换信息.
void CSceneObjectPanel::OnObjectModelReload() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		if(IDOK==MessageBox("重新加载模型后将丢失所有已经加入到场景中模型的光照信息！\n你确认要重新加载吗？","警告",MB_OKCANCEL))
		{
			CSceneObjectManager *pManager = pMap->GetSceneObjectMan();
			CButton *pButton = (CButton *)GetDlgItem(SCENE_OBJECT_MODEL_RELOAD);
			pButton->EnableWindow(false);
			SetCursor(LoadCursor(NULL, IDC_WAIT));
			pManager->ReloadLitModels();
			pButton->EnableWindow(true);
			SetCursor(LoadCursor(NULL, IDC_ARROW));
			pMap->SaveLitModel();
		}
	}
}

void CSceneObjectPanel::ShowPropertyPanel(bool bShow)
{
	CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
	CRollUpPanel *pRoll = pParent->GetPanel(CObjectsPage::PANEL_SCENE_OBJECT);
	
	ShowListPlanel(true);

	if(pRoll->IsOpen())
		pParent->ShowPanel(CObjectsPage::PANEL_SCENE_OBJECT,true);
	else pParent->ShowPanel(CObjectsPage::PANEL_SCENE_OBJECT,false);
	
	if(bShow)
	{
		pRoll = pParent->GetPanel(CObjectsPage::PANEL_PROPERTY_OBJECT);
		if(pRoll->IsOpen())
			pParent->ShowPanel(CObjectsPage::PANEL_PROPERTY_OBJECT,true);
		else pParent->ShowPanel(CObjectsPage::PANEL_PROPERTY_OBJECT,false);
	}else
	{
		pParent->HidePanel(CObjectsPage::PANEL_PROPERTY_OBJECT);	
	}
}

void CSceneObjectPanel::ShowListPlanel(bool bShow)
{
	CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
	CRollUpPanel *pRoll = pParent->GetPanel(CObjectsPage::PANEL_SCENE_OBJECT);
	
	if(pRoll->IsOpen())
		pParent->ShowPanel(CObjectsPage::PANEL_SCENE_OBJECT,true);
	else pParent->ShowPanel(CObjectsPage::PANEL_SCENE_OBJECT,false);

	if(bShow)
	{
		pRoll = pParent->GetPanel(CObjectsPage::PANEL_SCENE_LIST_OBJECT);
		if(pRoll->IsOpen())
			pParent->ShowPanel(CObjectsPage::PANEL_SCENE_LIST_OBJECT,true);
		else pParent->ShowPanel(CObjectsPage::PANEL_SCENE_LIST_OBJECT,false);
	}else
	{
		pParent->HidePanel(CObjectsPage::PANEL_SCENE_LIST_OBJECT);
	}
}

void CSceneObjectPanel::OnSceneObjectSkinmodel() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_SKINMODEL);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					CCommonFileDlg FileDlg(g_Render.GetA3DEngine(), "Models","*.ecm");
					if (FileDlg.DoModal() != IDOK)
						return; 
					AString strPathName = FileDlg.GetFullFileName();
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SKINMODEL_ADD);
					((CSceneSkinModelAddOperation*)pOperation)->SetSkinModel(strPathName);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectCritter() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CRITTER);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
				}
			}
			
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_CRITTER_ADD);
					((CSceneCritterAddOperation*)pOperation)->CreateCritter();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}


void CSceneObjectPanel::OnSceneObjectFixnpc() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_FIXNPC);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
			
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);

			// for test
			CElementMap *pMap = AUX_GetMainFrame()->GetMap();
			CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
			pSManager->ClearSelectedList();

			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_NPCGENERATOR_ADD);
					((CFixedNpcGeneratorOperation*)pOperation)->CreateNpc();
					((CFixedNpcGeneratorOperation*)pOperation)->TransObstructData();
					if(pViewFrame)
						pViewFrame->SetCurrentOperation(pOperation);

// 					CRenderWnd *pRWnd = pViewFrame->GetRenderWnd();
// 					if(pRWnd->GetActiveViewport() == VIEW_PERSPECTIVE)
// 					{
// 						pRWnd->GetViewport(VIEW_PERSPECTIVE)->
// 				
//					}
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectPrecinct() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_PRECINCT);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_PRECINCT_ADD);
					((CPrecinctOperation*)pOperation)->CreatePrecinct();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectGather() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GATHER);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_GATHER_ADD);
					((CSceneGatherOperation*)pOperation)->CreateGather();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectRange() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_RANGE);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_RANGE_ADD);
					((CRangeOperation*)pOperation)->CreateRange();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectCloudSet() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_SET);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				pParent->ShowPanel(CObjectsPage::PANEL_CLOUD_SET,true);
				pParent->UpdateCloudPanel();
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				//pMainFrame->GetToolTabWnd()->UpdateVernierDlg();
				//pMainFrame->GetToolTabWnd()->RiseUpVernierButton();
				pMainFrame->SetOtherOperation(true);
				CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
				if(pViewFrame) pViewFrame->SetCurrentOperation(NULL);
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectCloudBox() 
{
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_CLOUD_BOX);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_CLOUD_BOX_ADD);
					((CCloudBoxOperation*)pOperation)->CreateCloud();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectBoxInstance() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BOX_INSTANCE);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_INSTANCE_BOX_ADD);
					((CSceneInstanceBoxAddOperation*)pOperation)->CreateInstanceBox();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectDynamic() 
{
	// TODO: Add your control notification handler code here
		// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_DYNAMIC);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_DYNAMIC_ADD);
					((CDynamicObjectOperation*)pOperation)->CreateDynamic();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CSceneObjectPanel::OnSceneObjectBuffRegion() 
{
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_BUFF_REGION);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
			    pParent->HideAllPanels();
				ShowPropertyPanel(true);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_BUFF_REGION_ADD);
					((CBuffRegionOperation*)pOperation)->CreateBuffRegion();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
	
}

void CSceneObjectPanel::OnBnClickedSceneObjectGuildbuilding()
{
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllTerrainOperationButton();
	pButton = (CButton *)GetDlgItem(IDC_SCENE_OBJECT_GUILDBUILDING);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(false);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			AUX_GetMainFrame()->EnableBezierOperation(false);
			AUX_GetMainFrame()->EnableBezierMerge(false);
			CObjectsPage *pParent = (CObjectsPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				ShowPropertyPanel(true);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_GUILDBUILDING);
					((CGuildBuildingOperation*)pOperation)->CreateGuildBuilding();
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}
