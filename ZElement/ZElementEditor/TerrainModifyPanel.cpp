// TerrainModifyPanel.cpp : implementation file
//

#include "global.h"
#include "MainFrm.h"
#include "elementeditor.h"
#include "TerrainModifyPanel.h"
#include "OperationManager.h"
#include "TerrainPage.h"
#include "ObstructOperation.h"
#include "ObstructOperation2 .h"
#include "ObstructOperation3 .h"
#include "pathmap\BitImage.h"
#include "pathmap\BlockImage.h"
#include "SceneObjectManager.h"
#include "PathMap/PlayerPassMapGenerator.h"
#include "PathMap/PlayerAirPassMapGenerator.h"
#include "SceneAIGenerator.h"
#include "FloatInputDlg.h"
#include "BrushTreeOperation .h"

//#define new A_DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// CTerrainModifyPanel dialog


CTerrainModifyPanel::CTerrainModifyPanel(CWnd* pParent /*=NULL*/)
	: CDialog(CTerrainModifyPanel::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTerrainModifyPanel)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CTerrainModifyPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTerrainModifyPanel)
	DDX_Control(pDX, IDC_COMBO_SEL_PASSMAP_LAYER, m_cbSelPassMapLayer);
	DDX_Control(pDX, IDC_COMBO_SEL_PPASSMAP_LAYER, m_cbSelPPassMapLayer);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTerrainModifyPanel, CDialog)
	//{{AFX_MSG_MAP(CTerrainModifyPanel)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_STRETCH, OnTerrainModifyStretch)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_EMPTY, OnTerrainModifyEmpty)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_SMOOTH, OnTerrainModifySmooth)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_NOISE, OnTerrainModifyNoise)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_LAYMASK, OnTerrainModifyLaymask)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_LAYMASK_SOFT, OnTerrainModifyLaymaskSoft)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_PLANT, OnTerrainModifyPlant)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_RED, OnTerrainObstructRed)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_GREEN, OnTerrainObstructGreen)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_CONVEX, OnTerrainObstructConvex)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_TRANS, OnTerrainObstructMapTrans)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED, OnTerrainObstructMapSetseed)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW, OnTerrainObstructYellow)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_SPACE_CREATE, OnTerrainObstructMapSpaceCreate)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_SAVE_ALL_MAP, OnTerrainObstructSaveAllMap)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_GREEN2, OnTerrainObstructGreen2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_RED2, OnTerrainObstructRed2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED2, OnTerrainObstructMapSetseed2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_SAVE_ALL_MAP2, OnTerrainObstructSaveAllMap2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW2, OnTerrainObstructYellow2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_MAP_RESET2, OnTerrainObstructMapReset2)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_NPC_UNREACHABLE, OnTerrainObstructNpcUnreachable)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_SKY_REACH, OnTerrainObstructSkyReach)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_SKY_UNREACH, OnTerrainObstructSkyUnreach)
	ON_BN_CLICKED(IDC_TERRAIN_OBSTRUCT_SKY_REACH_SET_HEIGHT, OnTerrainObstructSkyReachSetHeight)
	ON_BN_CLICKED(IDC_BRUSH_TREE_CHECK, OnBrushTreeCheck)
	ON_BN_CLICKED(IDC_BRUSH_TREE_UNCHECK, OnBrushTreeUncheck)
	ON_BN_CLICKED(IDC_BRUSH_TREE_SPLIT, OnBrushTreeSplit)
	ON_BN_CLICKED(IDC_BRUSH_TREE_MERGE, OnBrushTreeMerge)
	ON_BN_CLICKED(IDC_BRUSH_TREE_REBUILD, OnBrushTreeRebuild)
	ON_BN_CLICKED(IDC_BRUSH_TREE_RELOAD_MODEL, OnBrushTreeReloadModel)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_PASSMAP_LAYER, OnSelchangeComboSelPassmapLayer)
	ON_CBN_SELCHANGE(IDC_COMBO_SEL_PPASSMAP_LAYER, OnSelchangeComboSelPpassmapLayer)
	
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_COLORMAP, OnBnClickedTerrainModifyColormap)
	ON_BN_CLICKED(IDC_TERRAIN_MODIFY_COLOR_SMOOTH, OnBnClickedTerrainModifyColorSmooth)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTerrainModifyPanel message handlers


//该对话框所有的按纽都在这儿同时设置Check属性，以后加上去的按纽
//也最好在这儿加入，防止造成管理上的混乱。
void CTerrainModifyPanel::CheckOtherButtonCheck(CButton *pButton)
{
	CButton *pTemp;

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_STRETCH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_EMPTY);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_SMOOTH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_NOISE);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_LAYMASK);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_LAYMASK_SOFT);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_PLANT);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_RED);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_RED2);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	
	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_GREEN);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_GREEN2);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_CONVEX);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}



	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_TRANS);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED2);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	//Button
	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW2);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_RESET2);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SKY_REACH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SKY_UNREACH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_BRUSH_TREE_CHECK);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_BRUSH_TREE_UNCHECK);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_BRUSH_TREE_SPLIT);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_BRUSH_TREE_MERGE);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_COLORMAP);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}

	pTemp = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_COLOR_SMOOTH);
	if(pButton != pTemp)
	{
		if(pTemp->GetCheck()!=0)
			pTemp->SetCheck(0);
	}
}


void CTerrainModifyPanel::OnTerrainModifyStretch() 
{
	// TODO: Add your control notification handler code here
	
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_STRETCH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYSTRETCH};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_STRETCH);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainModifyEmpty() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_EMPTY);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

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

void CTerrainModifyPanel::OnTerrainModifySmooth() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_SMOOTH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				
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
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYSMOOTH};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_SMOOTH);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainModifyNoise() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_NOISE);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYNOISE};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_NOISE);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainModifyLaymask() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_LAYMASK);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYLAYMASK};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_PAINT);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
				pParent->m_TerrainModifyLayMaskPanel.UpdateLayList();
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainModifyLaymaskSoft() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_LAYMASK_SOFT);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYLAYSMOOTH};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_MASK_SMOOTH);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
				pParent->m_TerrainModifyLayMaskSmoothPanel.UpdateLayList();
			}
		}
	}
}

void CTerrainModifyPanel::RiseUpAllButton()
{
	CheckOtherButtonCheck(0);
	CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
	if(pMainFrame)
	{
		//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
		//if(pViewFrame) pViewFrame->SetCurrentOperation(0);//NULL Operation
		pMainFrame->SetOtherOperation(false);
	}
}

void CTerrainModifyPanel::OnTerrainModifyPlant() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_PLANT);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYPLANTS};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					AUX_GetMainFrame()->GetToolTabWnd()->UpdatePlantsPanel();
					pMainFrame->SetOtherOperation(true);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_PLANTS);
					//if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}
void CTerrainModifyPanel::OnTerrainObstructRed() 
{
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_RED);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_RED);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructGreen() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_GREEN);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_GREEN);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructConvex() 
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_CONVEX);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_CH_CANNOT_PASS);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

// compute the reachable map
void CTerrainModifyPanel::OnTerrainObstructMapTrans() 
{
	// TODO: Add your control notification handler code here
	// Generate the pass map!
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_TRANS);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				// 进入到NPC可达图显示状态
				CString strMsg=(pMap->GetPassMapGen()->GetPassMap())?"当前可达图已经存在，确认要重新计算吗？":"确认要计算可达图吗？";
				strMsg+= "这可能将花费很长时间！";
				if(MessageBox(strMsg, NULL, MB_OKCANCEL)==IDOK)
				{
					BeginWaitCursor();

					//  Initialize the global PassMap Generator object
					if(pMap->GetPassMapGen()->GetPassMap()) pMap->GetPassMapGen()->Release();
					pMap->GetPassMapGen()->SetSampleInfo();
					pMap->GetPassMapGen()->SetPFSphereInfo();
					
					// Compute the pass map
					pMap->GetPassMapGen()->ComputePassMap();

					EndWaitCursor();
				}
				else
					if(!pMap->GetPassMapGen()->GetPassMap())
					{
						pButton->SetCheck(0);
						return;	// 如果尚未有任何可达图，则直接返回
					}

				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					//((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_SET_SEED);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_START);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
	
	
}

void CTerrainModifyPanel::OnTerrainObstructMapSetseed() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
			
		}else 
		{
			// now, we enter the mode of set pass map generator's seed !
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPassMapGen()->GetPassMap())
				{
					pMap->GetPassMapGen()->SetSampleInfo();
				}
				pMap->GetPassMapGen()->SetPFSphereInfo();

				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_SET_SEED);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}	
}

void CTerrainModifyPanel::OnTerrainObstructYellow() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT);
					((CObstructOperation*)pOperation)->Init(CObstructOperation::OBSTRUCT_YELLOW);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}	
}

void CTerrainModifyPanel::OnTerrainObstructMapSpaceCreate() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CButton *pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_SPACE_CREATE);
		BeginWaitCursor();
		abase::vector<CConvexHullData *> CHs;
		SPOctree::CSpacePassableOctree* pass = pMap->GetSpacePass();
		pMap->GetSceneObjectMan()->GetAllConvexHulls(CHs);
		pass->Release();
		pass->Init(CHs);
		pass->BuildTree();
		pMap->SaveSpacePassMap();
		EndWaitCursor();
		if(pButton) pButton->SetCheck(0);
	}
}

void CTerrainModifyPanel::OnTerrainObstructSaveAllMap() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		if(!pMap->SaveAllPassMap())
			MessageBox("保存通过图失败! 详细情况请看ElementEditor.log。");
	}
	CButton *pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SAVE_ALL_MAP);
	pButton->SetCheck(0);
}

void CTerrainModifyPanel::OnTerrainObstructGreen2() 
{
	// TODO: Add your control notification handler code here
	//设置成可达点
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_GREEN2);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT2);
					((CObstructOperation2*)pOperation)->Init(CObstructOperation2::OBSTRUCT_GREEN);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructRed2() 
{
	// TODO: Add your control notification handler code here
	//‘设置成不可达点
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_RED2);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT2);
					((CObstructOperation2*)pOperation)->Init(CObstructOperation2::OBSTRUCT_RED);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructMapSetseed2() 
{
	// TODO: Add your control notification handler code here
	//设置可达图种子
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_SETSEED2);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
			
		}else 
		{
			// now, we enter the mode of set pass map generator's seed !
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerPassMapGen()->GetPassMap())
				{
					pMap->GetPlayerPassMapGen()->SetSampleInfo();
				}
				pMap->GetPlayerPassMapGen()->SetPFSphereInfo();

				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT2);
					((CObstructOperation2*)pOperation)->Init(CObstructOperation2::OBSTRUCT_SET_SEED);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}	
}



void CTerrainModifyPanel::OnTerrainObstructSaveAllMap2() 
{
	// TODO: Add your control notification handler code here
	//保存所有通过图
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		if(!pMap->SavePassMap2())
			MessageBox("保存通过图2失败! 详细情况请看ElementEditor.log。");
	}
	CButton *pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SAVE_ALL_MAP2);
	pButton->SetCheck(0);
}

void CTerrainModifyPanel::OnTerrainObstructYellow2() 
{
	// TODO: Add your control notification handler code here
	//设置成建筑可达点
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_YELLOW2);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT2);
					((CObstructOperation2*)pOperation)->Init(CObstructOperation2::OBSTRUCT_YELLOW);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}	
}

void CTerrainModifyPanel::OnTerrainObstructMapReset2() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_MAP_RESET2);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
				}
			}
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{

				if(!pMap->GetPlayerPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT2);
					((CObstructOperation2*)pOperation)->Init(CObstructOperation2::OBSTRUCT_RESET);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructNpcUnreachable() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap)
	{
		CPlayerPassMapGenerator *pGen = pMap->GetPlayerPassMapGen();
		if(pGen && pGen->GetPassMap())
		{
			CSceneObjectManager *pMan = pMap->GetSceneObjectMan();
			APtrList< CSceneObject*>* npcList = pMan->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
			ALISTPOSITION pos = npcList->GetHeadPosition();
			while( pos )
			{
				CSceneFixedNpcGenerator* pObj = (CSceneFixedNpcGenerator*) npcList->GetNext(pos);
				FIXED_NPC_DATA data = pObj->GetProperty();
				//if(data.nPathID==-1 && !data.bIsMounster)
				if(data.nPathID==-1 && data.cNpcInfo==0)
				{
					A3DVECTOR3 vPos = pObj->GetPos();
					PLAYER_SAMPLE_POS spos;
					pGen->Transf3DTo2D(vPos,spos);
					pGen->SetPosUnreachable(spos);
					//neighbors
					int NeighborD[] =
					{
							-1,  0,   //left
							+1,  0,   //right
							0,   +1,  //top
							0,   -1,  //bottom
							+1,  +1,  //topright
							+1,  -1,  //bottomright
							-1,  +1,  //topleft
							-1,  -1   //bottomleft
					};
					for (int i = 0; i < 8; ++i)
					{
						PLAYER_SAMPLE_POS sNeiPos;
						sNeiPos= spos;
						sNeiPos.u += NeighborD[i*2];
						sNeiPos.v += NeighborD[i*2+1];
						pGen->SetPosUnreachable(sNeiPos);
					}
				}
			}
		}
	}
	CButton *pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_NPC_UNREACHABLE);
	pButton->SetCheck(0);
}



void CTerrainModifyPanel::OnTerrainObstructSkyReach() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SKY_REACH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT3);
					((CObstructOperation3*)pOperation)->Init(CObstructOperation3::OBSTRUCT_GREEN);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructSkyUnreach() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_OBSTRUCT_SKY_UNREACH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_OBSTRUCT3);
					((CObstructOperation3*)pOperation)->Init(CObstructOperation3::OBSTRUCT_RED);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnTerrainObstructSkyReachSetHeight() 
{
	// TODO: Add your control notification handler code here
	CFloatInputDlg dlg;
	dlg.m_fFloat = g_Configs.fAirPassMapDelta;
	if(dlg.DoModal()==IDOK)
	{
		g_Configs.fAirPassMapDelta = dlg.m_fFloat;	
	}
}

void CTerrainModifyPanel::OnBrushTreeCheck() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_BRUSH_TREE_CHECK);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BRUSH_TREE);
					((CBrushTreeOperation*)pOperation)->Init(CBrushTreeOperation::BRUSH_CHECK);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
	
}

void CTerrainModifyPanel::OnBrushTreeUncheck() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_BRUSH_TREE_UNCHECK);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BRUSH_TREE);
					((CBrushTreeOperation*)pOperation)->Init(CBrushTreeOperation::BRUSH_UNCHECK);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnBrushTreeSplit() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_BRUSH_TREE_SPLIT);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BRUSH_TREE);
					((CBrushTreeOperation*)pOperation)->Init(CBrushTreeOperation::BRUSH_SPLIT);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
	
}

void CTerrainModifyPanel::OnBrushTreeMerge() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_BRUSH_TREE_MERGE);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}
			
		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				if(!pMap->GetPlayerAirPassMapGen()->GetPassMap())
				{
					pButton->SetCheck(0);
					return;
				}
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);

				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BRUSH_TREE);
					((CBrushTreeOperation*)pOperation)->Init(CBrushTreeOperation::BRUSH_MERGE);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
	
}

void CTerrainModifyPanel::OnBrushTreeRebuild() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	if(IDYES == AfxMessageBox("你确定让程序自动划分区域大小么？",MB_YESNO))
		pMap->GetSceneObjectMan()->RebuildBrushTree();
}

void CTerrainModifyPanel::OnBrushTreeReloadModel() 
{
	// TODO: Add your control notification handler code here
	CElementMap *pMap = AUX_GetMainFrame()->GetMap();
	if(pMap==NULL) return;
	pMap->GetSceneObjectMan()->ReloadHullToBrushTree();
}

void CTerrainModifyPanel::OnSelchangeComboSelPassmapLayer() 
{
	// TODO: Add your control notification handler code here
	int sel = m_cbSelPassMapLayer.GetCurSel();
	if(sel != -1)
	{
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap) 
		{
			CString str;
			str.Format("你需要保存层 %d 的数据么?",g_nCurPassmapLayer);
			if((!g_bReadOnly) && IDYES==AfxMessageBox(str,MB_YESNO)) pMap->SavePassMap();
			
			g_nCurPassmapLayer = sel;
			pMap->LoadPassMap();
		}
		
	}
}

void CTerrainModifyPanel::OnSelchangeComboSelPpassmapLayer() 
{
	// TODO: Add your control notification handler code here
	int sel = m_cbSelPPassMapLayer.GetCurSel();
	if(sel != -1)
	{
		CElementMap *pMap = AUX_GetMainFrame()->GetMap();
		if(pMap) 
		{
			CString str;
			str.Format("你需要保存层 %d 的数据么?",g_nCurPlayerPassmapLayer);
			if((!g_bReadOnly) && IDYES==AfxMessageBox(str,MB_YESNO)) pMap->SavePassMap2();
			
			g_nCurPlayerPassmapLayer = sel;
			pMap->LoadPassMap2();
		}
		
	}
}

BOOL CTerrainModifyPanel::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	m_cbSelPPassMapLayer.AddString("Layer0");
	m_cbSelPPassMapLayer.AddString("Layer1");
	m_cbSelPPassMapLayer.AddString("Layer2");
	m_cbSelPPassMapLayer.SetCurSel(g_nCurPlayerPassmapLayer);


	m_cbSelPassMapLayer.AddString("Layer0");
	m_cbSelPassMapLayer.AddString("Layer1");
	m_cbSelPassMapLayer.AddString("Layer2");
	m_cbSelPassMapLayer.SetCurSel(g_nCurPassmapLayer);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CTerrainModifyPanel::OnBnClickedTerrainModifyColormap()
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_COLORMAP);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}

		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYCOLORMAP};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLORMAP);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}

void CTerrainModifyPanel::OnBnClickedTerrainModifyColorSmooth()
{
	// TODO: Add your control notification handler code here
	CButton *pButton;
	AUX_GetMainFrame()->GetToolTabWnd()->RiseUpAllSceneObjectButton();
	pButton = (CButton *)GetDlgItem(IDC_TERRAIN_MODIFY_COLOR_SMOOTH);
	if(pButton)
	{
		CheckOtherButtonCheck(pButton);
		if(pButton->GetCheck()==0)
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[1] = {CTerrainPage::PANEL_MODIFY};
				bool aOpenFlags[1] = {true};
				pParent->ShowPanels(aIndices, aOpenFlags, 1);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(false);
					//CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					//if(pViewFrame) pViewFrame->SetCurrentOperation(0);
				}
			}

		}else 
		{
			CTerrainPage *pParent = (CTerrainPage*)(GetParent()->GetParent());
			if(pParent)
			{
				//	Hide panels
				pParent->HideAllPanels();
				int aIndices[2] = {CTerrainPage::PANEL_MODIFY,CTerrainPage::PANEL_MODIFYCOLORSMOOTH};
				bool aOpenFlags[2] = {true,true};
				pParent->ShowPanels(aIndices, aOpenFlags, 2);
				CMainFrame *pMainFrame = (CMainFrame*)AfxGetMainWnd();
				if(pMainFrame)
				{
					pMainFrame->SetOtherOperation(true);
					CViewFrame *pViewFrame = pMainFrame->GetViewFrame();
					COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_TERRAIN_COLOR_SMOOTH);
					if(pViewFrame) pViewFrame->SetCurrentOperation(pOperation);
				}
			}
		}
	}
}
