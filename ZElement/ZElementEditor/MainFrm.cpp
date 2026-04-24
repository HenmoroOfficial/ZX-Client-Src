// MainFrm.cpp : implementation of the CMainFrame class
//

#include "Stdafx.h"
#include "MainFrm.h"
#include "ElementMap.h"
#include "Terrain.h"
#include "MaskModifier.h"
#include "ViewFrame.h"
#include "Render.h"
#include "UndoMan.h"

#include "DlgNewMap.h"
#include "DlgHeightMap.h"
#include "DlgSurfaces.h"
#include "DlgSaveAs.h"
#include "DlgSelectFile.h"
#include "ExpGameDataDlg.h"
#include "DlgExpGameData.h"
#include "DlgLighting.h"
#include "DlgAutoPartHM.h"
#include "OperationManager.h"
#include "UndoLightDeleteAction.h"
#include "SceneObjectManager.h"
#include "BezierUndoAction.h"
#include "ModifiedProfileDlg.h"
#include "TerrainConnectDlg.h"
#include "TerrainBlockViewDlg.h"
#include "SceneBoxArea.h"
#include "BezierAddOperation.h"
#include "SceneObjectRotateOperation.h"
#include "LightMapTransition.h"
#include "ResManDlg.h"
#include "VssOptionDlg.h"
#include "VssOperation.h"
//#include "SCMOperation.h"
#include "TerrainResExpDlg.h"
#include "AScriptFile.h"
#include "MoveAllObjHeightDlg.h"
#include "Bitmap.h"
#include "DelObjectDlg.h"
#include "WorkSpaceSetDlg.h"
#include "TerrainRender.h"
#include "ScenePrecinctObject.h"
#include "SceneAIGenerator.h"
#include "MousterStatDlg.h"
#include "ModelChangedStatusDlg.h"
#include "ExportLitDlg.h"
#include "DynamicObjectManDlg.h"
#include "NpcControllerManDlg.h"
#include "UndoRotateAction.h"
#include "MousterStatFilterDlg.h"
#include "ElementEditor.h"
//#include "DlgModelFinder.h"
#include "BezierMergeDlg.h"
#include "LogDlg.h"
#include "ImportFromHistroy.h"
#include "ExportSceneInfo.h"
#include "StaticModelObject.h"
#include "DlgPreRadStep1.h"
#include "DlgPreRadStep2.h"
#include "RayTraceDlg.h"
#include "AC.h"
#include "A3D.h"

#include <vector>
#include <shlwapi.h>
#ifdef _ANGELICA22
#include "A3DHLSL.h"
#endif

extern CString sz_map_file_ex[];

extern CBezierMergeDlg g_BezierMergeDlg;
extern CLogDlg g_LogDlg;


//#define new A_DEBUG_NEW

//	Minimal size of frame window
enum
{
	MIN_WIDTH		= 300,
	MIN_HEIGHT		= 300,
	TOOLBAR_WIDTH	= 350,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_WM_SIZING()
	ON_WM_ACTIVATEAPP()
	ON_COMMAND(ID_FILE_NEW, OnFileNew)
	ON_COMMAND(ID_TERRAIN_HEIGHTMAP, OnTerrainHeightmap)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_HEIGHTMAP, OnUpdateTerrainHeightmap)
	ON_WM_DESTROY()
	ON_WM_SYSCOMMAND()
	ON_WM_ENTERIDLE()
	ON_COMMAND(ID_TERRAIN_SURACES, OnTerrainSuraces)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_SURACES, OnUpdateTerrainSuraces)
	ON_COMMAND(ID_FILE_SAVE, OnFileSave)
	ON_COMMAND(ID_FILE_OPEN, OnFileOpen)
	ON_COMMAND(ID_FILE_SAVEAS, OnFileSaveas)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVEAS, OnUpdateFileSaveas)
	ON_COMMAND(ID_FILE_CLOSE, OnFileClose)
	ON_UPDATE_COMMAND_UI(ID_FILE_CLOSE, OnUpdateFileClose)
	ON_COMMAND(ID_FILE_EXPORTGAMEDATA, OnFileExportgamedata)
	ON_COMMAND(ID_TERRAIN_LIGHTING, OnTerrainLighting)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_LIGHTING, OnUpdateTerrainLighting)
	ON_COMMAND(ID_TERRAIN_CREATESURFS, OnTerrainCreatesurfs)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_CREATESURFS, OnUpdateTerrainCreatesurfs)
	ON_COMMAND(ID_BRUSH_CIRCLE, OnBrushCircle)
	ON_COMMAND(ID_BRUSH_RADIUS_1, OnBrushRadius1)
	ON_COMMAND(ID_BRUSH_RADIUS_2, OnBrushRadius2)
	ON_COMMAND(ID_BRUSH_RADIUS_3, OnBrushRadius3)
	ON_COMMAND(ID_BRUSH_RADIUS_ADD, OnBrushRadiusAdd)
	ON_COMMAND(ID_BRUSH_RADIUS_SUB, OnBrushRadiusSub)
	ON_COMMAND(ID_BRUSH_RECT, OnBrushRect)
	ON_COMMAND(ID_OPERATION_REDO, OnOperationRedo)
	ON_COMMAND(ID_OPERATION_UNDO, OnOperationUndo)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_UNDO, OnUpdateOperationUndo)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_REDO, OnUpdateOperationRedo)
	ON_COMMAND(ID_OPERATION_SELECT, OnOperationSelect)
	ON_COMMAND(ID_OPERATION_MOVE, OnOperationMove)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_MOVE, OnUpdateOperationMove)
	ON_COMMAND(ID_OPERATION_ROTATE, OnOperationRotate)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_ROTATE, OnUpdateOperationRotate)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_SELECT, OnUpdateOperationSelect)
	ON_COMMAND(ID_OPERATION_SELECTMULTI, OnOperationSelectmulti)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_SELECTMULTI, OnUpdateOperationSelectmulti)
	ON_COMMAND(ID_OPERATION_SCALE, OnOperationScale)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_SCALE, OnUpdateOperationScale)
	ON_COMMAND(ID_OPERATION_DELETE, OnOperationDelete)
	ON_COMMAND(ID_BEZIER_ADDPOINT, OnBezierAddpoint)
	ON_COMMAND(ID_BEZIER_CUT, OnBezierCut)
	ON_COMMAND(ID_BEZIER_SMOOTH, OnBezierSmooth)
	ON_COMMAND(ID_BEZIER_SUBPOINT, OnBezierSubpoint)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_SUBPOINT, OnUpdateBezierSubpoint)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_CUT, OnUpdateBezierCut)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_ADDPOINT, OnUpdateBezierAddpoint)
	ON_COMMAND(ID_BEZIER_DRAG, OnBezierDrag)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_DRAG, OnUpdateBezierDrag)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_SMOOTH, OnUpdateBezierSmooth)
	ON_COMMAND(ID_BUTTON_NOSPEAK, OnButtonNospeak)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_NOSPEAK, OnUpdateButtonNospeak)
	ON_COMMAND(ID_BUTTON_SHOW_VERNIER, OnButtonShowVernier)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SHOW_VERNIER, OnUpdateButtonShowVernier)
	ON_COMMAND(ID_BEZIER_TEST, OnBezierTest)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_TEST, OnUpdateBezierTest)
	ON_COMMAND(ID_EDIT_OBJECT_COPY, OnEditObjectCopy)
	ON_COMMAND(ID_FILE_AUTOPARTHM, OnFileAutoparthm)
	ON_COMMAND(ID_FILE_SORT_MAP, OnFileSortMap)
	ON_COMMAND(ID_TERRAIN_MODIFIED_PROFILE, OnTerrainModifiedProfile)
	ON_COMMAND(ID_MAP_AUTO_MERGE, OnMapAutoMerge)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_MODIFIED_PROFILE, OnUpdateTerrainModifiedProfile)
	ON_UPDATE_COMMAND_UI(ID_MAP_AUTO_MERGE, OnUpdateMapAutoMerge)
	ON_COMMAND(ID_OPERATION_OBSTRUCT, OnOperationObstruct)
	ON_COMMAND(ID_BEZIER_MERGE, OnBezierMerge)
	ON_UPDATE_COMMAND_UI(ID_BEZIER_MERGE, OnUpdateBezierMerge)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_OBSTRUCT, OnUpdateOperationObstruct)
	ON_COMMAND(ID_EXP_AI_DATA, OnExpAiData)
	ON_COMMAND(ID_IMP_AI_DATA, OnImpAiData)
	ON_UPDATE_COMMAND_UI(ID_IMP_AI_DATA, OnUpdateImpAiData)
	ON_UPDATE_COMMAND_UI(ID_EXP_AI_DATA, OnUpdateExpAiData)
	ON_COMMAND(ID_TERRAIN_BLOCK_VIEW, OnTerrainBlockView)
	ON_COMMAND(ID_DAY_NIGHT, OnDayNight)
	ON_UPDATE_COMMAND_UI(ID_DAY_NIGHT, OnUpdateDayNight)
	ON_UPDATE_COMMAND_UI(ID_TERRAIN_BLOCK_VIEW, OnUpdateTerrainBlockView)
	ON_COMMAND(ID_BUTTON_RAIN, OnButtonRain)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_RAIN, OnUpdateButtonRain)
	ON_COMMAND(ID_BUTTON_SNOW, OnButtonSnow)
	ON_UPDATE_COMMAND_UI(ID_BUTTON_SNOW, OnUpdateButtonSnow)
	ON_COMMAND(ID_MENU_RES_MAN, OnMenuResMan)
	ON_COMMAND(ID_SET_VSS, OnSetVss)
	ON_COMMAND(ID_GET_VSS_ALL, OnGetVssAll)
	ON_COMMAND(VSS_TEST_MAP_CHECKIN, OnTestMapCheckin)
	ON_COMMAND(ID_EXPORT_GAME_RES, OnExportGameRes)
	ON_UPDATE_COMMAND_UI(VSS_TEST_MAP_CHECKIN, OnUpdateTestMapCheckin)
	ON_UPDATE_COMMAND_UI(ID_GET_VSS_ALL, OnUpdateGetVssAll)
	ON_UPDATE_COMMAND_UI(ID_MENU_RES_MAN, OnUpdateMenuResMan)
	ON_COMMAND(ID_MOVE_ALL_SCENE_OBJECT, OnMoveAllSceneObject)
	ON_COMMAND(ID_GET_ALL_MAP_DATA, OnGetAllMapData)
	ON_COMMAND(ID_EXP_TOPVIEW_MAP, OnExpTopviewMap)
	ON_COMMAND(ID_OPERATION_TEST_BEIZER_INTERSECT, OnOperationTestBeizerIntersect)
	ON_COMMAND(ID_TEST_PRECINCT, OnTestPrecinct)
	ON_COMMAND(ID_TEST_PRECINCT_ERROR, OnTestPrecinctError)
	ON_COMMAND(ID_SHOW_MODEL_PLANE, OnShowModelPlane)
	ON_UPDATE_COMMAND_UI(ID_SHOW_MODEL_PLANE, OnUpdateShowModelPlane)
	ON_COMMAND(ID_MERGE_TREE_TYPE, OnMergeTreeType)
	ON_COMMAND(ID_ROTATE_X, OnRotateX)
	ON_COMMAND(ID_ROTATE_Y, OnRotateY)
	ON_COMMAND(ID_ROTATE_Z, OnRotateZ)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_X, OnUpdateRotateX)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_Y, OnUpdateRotateY)
	ON_UPDATE_COMMAND_UI(ID_ROTATE_Z, OnUpdateRotateZ)
	ON_WM_SIZE()
	ON_CBN_SELCHANGE(ID_INDICATOR_COMBO, OnSelchangeCombo)
	ON_COMMAND(ID_SHOW_SEL_ONLY, OnShowSelOnly)
	ON_UPDATE_COMMAND_UI(ID_SHOW_SEL_ONLY, OnUpdateShowSelOnly)
	ON_COMMAND(ID_SET_WORKSPACE, OnSetWorkspace)
	ON_UPDATE_COMMAND_UI(ID_SET_WORKSPACE, OnUpdateSetWorkspace)
	ON_COMMAND(ID_SHOW_TERRAIN_LIGHT, OnShowTerrainLight)
	ON_UPDATE_COMMAND_UI(ID_SHOW_TERRAIN_LIGHT, OnUpdateShowTerrainLight)
	ON_COMMAND(ID_SHOW_DISTANCE, OnShowDistance)
	ON_UPDATE_COMMAND_UI(ID_SHOW_DISTANCE, OnUpdateShowDistance)
	ON_COMMAND(ID_INSERT_MOUNSTER_TO_MAP, OnInsertMounsterToMap)
	ON_COMMAND(ID_GET_CURRENT_PROJ_WORKSPACE_MAPS, OnGetCurrentProjWorkspaceMaps)
	ON_COMMAND(ID_MOUNSTER_STAT, OnMounsterStat)
	ON_COMMAND(ID_RELOAD_NPC, OnReloadNpcs)
	ON_COMMAND(ID_MODEL_CHANGED, OnModelChanged)
	ON_COMMAND(ID_EXPORT_SKY_INFOR, OnExportSkyInfor)
	ON_COMMAND(ID_EXP_LIGHT_INFOR_LIST, OnExpLightInforList)
	ON_COMMAND(ID_STAT_ALL_TRIANGLE, OnStatAllTriangle)
	ON_COMMAND(ID_STATUS_TRIANGLE_SEL, OnStatusTriangleSel)
	ON_COMMAND(ID_STATUS_BACK_TO_ORIGIN, OnStatusBackToOrigin)
	ON_COMMAND(ID_OPERATION_ROTATE2, OnOperationRotate2)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_ROTATE2, OnUpdateOperationRotate2)
	ON_COMMAND(ID_DYNAMIC_OBJECT_EDITOR, OnDynamicObjectEditor)
	ON_COMMAND(ID_MONSTER_CONTROLLER_EDITOR, OnMonsterControllerEditor)
	ON_COMMAND(ID_RELOAD_ALL_TEXTURES, OnReloadAllTextures)
	ON_UPDATE_COMMAND_UI(ID_OPERATION_DELETE, OnUpdateOperationDelete)
	ON_UPDATE_COMMAND_UI(ID_EDIT_OBJECT_COPY, OnUpdateEditObjectCopy)
	ON_UPDATE_COMMAND_UI(ID_VIEW_RESETCAMERA, OnUpdateViewResetcamera)
	ON_COMMAND(ID_STAT_MUSIC, OnStatMusic)
	ON_COMMAND(ID_IMPORT_HISTORY, OnImportHistory)
	ON_COMMAND(ID_EXPORT_INFO, OnExportInfo)
    ON_COMMAND(ID_RELOAD_SHADERS, OnReloadShaders)
	//}}AFX_MSG_MAP

	ON_COMMAND(ID_ENABLELITMAP, OnEnablelitmap)
	ON_UPDATE_COMMAND_UI(ID_ENABLELITMAP, OnUpdateEnablelitmap)
	ON_COMMAND(ID_ENABLE_REALTIME_LIGHT, OnEnableRealtimeLight)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_REALTIME_LIGHT, OnUpdateEnableRealtimeLight)
	ON_COMMAND(ID_ENABLE_DETAIL, OnEnableDetail)
	ON_UPDATE_COMMAND_UI(ID_ENABLE_DETAIL, OnUpdateEnableDetail)

	ON_COMMAND(ID_CALC_LIGHTMAP_RATE, OnCalcLightmapRate)
	ON_UPDATE_COMMAND_UI(ID_CALC_LIGHTMAP_RATE, OnUpdateCalcLightmapRate)
	ON_COMMAND(ID_DELSELOBJLITMAP, OnDelSelObjLitmap)
	ON_UPDATE_COMMAND_UI(ID_DELSELOBJLITMAP, OnUpdateDelSelObjLitmap)
	ON_COMMAND(ID_RELOADLITMAP, OnReloadLitmap)
	ON_UPDATE_COMMAND_UI(ID_RELOADLITMAP, OnUpdateReloadLitmap)
	ON_COMMAND(ID_RELOADLITMAP_NOVERT, OnReloadlitmapNovert)
	ON_UPDATE_COMMAND_UI(ID_RELOADLITMAP_NOVERT, OnUpdateReloadlitmapNovert)
	ON_COMMAND(IDM_LIGHTMAP_WIZARD, OnLightMapWizard)
	ON_UPDATE_COMMAND_UI(IDM_LIGHTMAP_WIZARD, OnUpdateLightMapWizard)
	ON_COMMAND(IDM_RAYTRACE_LIGHTMAP_WIZARD, OnRaytraceLightmapWizard)
	ON_UPDATE_COMMAND_UI(IDM_RAYTRACE_LIGHTMAP_WIZARD, OnUpdateRaytraceLightmapWizard)

	END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,  
	ID_INDICATOR_BRUSH_RADIUS,// status line indicator
	ID_INDICATOR_STATIC_X,
	ID_INDICATOR_EDIT_X,
	ID_INDICATOR_STATIC_Y,
	ID_INDICATOR_EDIT_Y,
	ID_INDICATOR_STATIC_Z,
	ID_INDICATOR_EDIT_Z,
	ID_INDICATOR_COMBO,
	ID_INDICATOR_PROGRESS,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
	m_bActive		= false;
	m_pMap			= NULL;
	m_dwLastFrame	= 0;
	m_dwTickTime	= 0;
	m_nSceneObjectOperation = 2;
	m_bOtherOperation = false;
	m_bShowMergeDlg = false;
	m_bEnableBezierOperation = false;
	m_bEnableBezierMerge = false;
	m_bEnableBezierSmooth = false;
	m_bEnableBezierTest = false;
	m_bBezierBarShow	= false;
}

CMainFrame::~CMainFrame()
{

}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	// create a view to occupy the client area of the frame
	if (!m_wndViewframe.Create(NULL, NULL, AFX_WS_DEFAULT_VIEW,
		CRect(0, 0, 0, 0), this, AFX_IDW_PANE_FIRST, NULL))
	{
		TRACE0("Failed to create view window\n");
		return -1;
	}
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if(!m_wndVssDlgBar.Create(this,IDD_VSS_LOG,CBRS_BOTTOM|CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,IDD_VSS_LOG))
	{
		TRACE0("Failed to create vss bar\n");
		return -1;      // fail to create
	}

	if(!m_BezierPointBar.Create(this,IDD_DIALOG_EDIT_POINT,CBRS_TOOLTIPS|CBRS_FLYBY|CBRS_SIZE_DYNAMIC,IDD_DIALOG_EDIT_POINT))
	{
		TRACE0("Failed to create bezier point bar\n");
		return -1;      // fail to create
	}

	// Create brush bar
	if (!m_wndBrushToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_LEFT  
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndBrushToolBar.LoadToolBar(IDR_BRUSH))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	//	TODO: Delete these three lines if you don't want the toolbar to
	//  be dockable
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_wndBrushToolBar.EnableDocking(CBRS_ALIGN_ANY);
	m_BezierPointBar.EnableDocking(0);   
	CRect toolBarRect;
	RecalcLayout(TRUE);
	m_wndToolBar.GetWindowRect(&toolBarRect);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar,CBRS_ALIGN_ANY,0);
	//toolBarRect.OffsetRect(1,0);
	DockControlBar(&m_wndBrushToolBar,CBRS_ALIGN_ANY,0);
	ShowControlBar(&m_BezierPointBar,  FALSE,   FALSE);   
	FloatControlBar(&m_BezierPointBar,CPoint(50,100));

	//	Initialize tool tab window
	if (!InitToolTabWnd())
	{
		g_Log.Log("CMainFrame::OnCreate, Failed to create tool tab window");
		return -1;
	}

	g_BezierMergeDlg.Create(IDD_BEZIER_MERGE,this);
	
	m_dwLastFrame = a_GetTime();
	
	m_StatusCombo.Create(&m_wndStatusBar, ID_INDICATOR_COMBO,
			 WS_VISIBLE | WS_BORDER | CBS_DROPDOWNLIST);
	m_StatusCombo.AddString("不显示相邻地图");
	m_StatusCombo.AddString("显示相邻地图(上)");
	m_StatusCombo.AddString("显示相邻地图(下)");
	m_StatusCombo.AddString("显示相邻地图(左)");
	m_StatusCombo.AddString("显示相邻地图(右)");
	m_StatusCombo.SetCurSel(0);


	m_StatusProgress.Create(&m_wndStatusBar, ID_INDICATOR_PROGRESS, WS_VISIBLE | PBS_SMOOTH);
	
	g_BlockViewDlg.Create(IDD_TERRAIN_BLOCK_VIEW_DLG,this);
	g_BlockViewDlg.ShowWindow(false);
	//把当前的操作设置为选择操作
	SetOtherOperation(false);
	m_DayAndNightDlg.Create(IDD_DIALOG_DAY_NIGHT,&m_wndStatusBar);
	m_DayAndNightDlg.ShowWindow(SW_SHOW);
	return 0;
}

void CMainFrame::OnDestroy() 
{
	CFrameWnd::OnDestroy();
	
	ReleaseMap();	
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if (!CFrameWnd::PreCreateWindow(cs))
		return FALSE;
	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);

	return TRUE;
}

//	Initialize tool tab window
bool CMainFrame::InitToolTabWnd()
{
	int iScrWid = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScrHei = GetSystemMetrics(SM_CYFULLSCREEN);
	CRect Rect(iScrWid-TOOLBAR_WIDTH, 0, iScrWid-3, iScrHei);

	if (!m_ToolTabWnd.Create(NULL, "工具栏", WS_POPUP | WS_CAPTION | WS_MINIMIZEBOX | WS_SYSMENU, Rect, this, NULL))
	{
		g_Log.Log("CMainForm::InitRenderWnd, Failed to create tool tab window.\n");
		return false;
	}

	//	This trigger WM_SIZE message for m_ToolTabWnd
	m_ToolTabWnd.MoveWindow(&Rect, FALSE);
	//m_ToolTabWnd.ShowWindow(SW_SHOWMINIMIZED);
	m_ToolTabWnd.ShowWindow(SW_SHOW);
	return true;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers
void CMainFrame::OnSetFocus(CWnd* pOldWnd)
{
	// forward focus to the view window
	m_wndViewframe.SetFocus();
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	// let the view have first crack at the command
	if (m_wndViewframe.OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
		return TRUE;

	// otherwise, do default handling
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnSizing(UINT fwSide, LPRECT pRect) 
{
	//	Prevent window's size to be too small
	if (MIN_WIDTH > pRect->right - pRect->left)
	{
		if (fwSide == WMSZ_BOTTOMLEFT || fwSide == WMSZ_LEFT || fwSide == WMSZ_TOPLEFT)
			pRect->left = pRect->right - MIN_WIDTH;
		else
			pRect->right = pRect->left + MIN_WIDTH;
	}

	if (MIN_HEIGHT > pRect->bottom - pRect->top)
	{
		if (fwSide == WMSZ_TOP || fwSide == WMSZ_TOPLEFT || fwSide == WMSZ_TOPRIGHT)
			pRect->top = pRect->bottom - MIN_HEIGHT;
		else
			pRect->bottom = pRect->top + MIN_HEIGHT;
	}

	CFrameWnd::OnSizing(fwSide, pRect);
}
#ifdef _ANGELICA22
void CMainFrame::OnActivateApp(BOOL bActive, DWORD hTask)
#else
void CMainFrame::OnActivateApp(BOOL bActive, HTASK hTask) 
#endif // #ifdef _ANGELICA22
{
	CFrameWnd::OnActivateApp(bActive, hTask);
	
	m_bActive = bActive ? true : false;
}

//	Create new map
bool CMainFrame::CreateMap()
{
	//	Release old map
	ReleaseMap();

	if (!(m_pMap = new CElementMap))
		return false;

	return true;
}

//	Release map
void CMainFrame::ReleaseMap()
{
	if (!m_pMap)
		return;

	//	Save edit record data to file
	m_pMap->SaveEditRecord();

	//	Reset undo manager
	g_UndoMan.Reset();

	m_pMap->Release();
	delete m_pMap;
	m_pMap = NULL;

	//	Unbind map and view frame
	m_wndViewframe.SetMap(NULL);
}

//	Create a new map
void CMainFrame::OnFileNew() 
{
/*	AString strFileInfo;
	strFileInfo.Format("%s%s\\info.txt", g_szWorkDir, g_szEditMapDir);
	pFileTest.Open(strFileInfo, AFILE_TEXT|AFILE_OPENEXIST);
	pCInfos = new CorrectInfo[19999];
	int i;
	char strRead[99];
	DWORD iLenRead;
	while(pFileTest.ReadLine(strRead, 99, &iLenRead))
	{
		int index;
		sscanf(strRead, "%d", &index);
		CorrectInfo &pCurInfo = pCInfos[index];
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vr%f,%f,%f", &pCurInfo.r[0], &pCurInfo.r[1], &pCurInfo.r[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vu%f,%f,%f", &pCurInfo.u[0], &pCurInfo.u[1], &pCurInfo.u[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vl%f,%f,%f", &pCurInfo.l[0], &pCurInfo.l[1], &pCurInfo.l[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vs%f,%f,%f", &pCurInfo.s[0], &pCurInfo.s[1], &pCurInfo.s[2]);
		else
			break;
		pCurInfo.bOK = true;
	}

	pFileTest.Close();

	CElementMap pMap;
	CString strProjName;
	strProjName.Format("%s%s\\1\\1.elproj", g_szWorkDir,g_szEditMapDir);
	if(!pMap.Load(strProjName))
		g_Log.Log("Open file %s failed!",strProjName);
	pMap.Release();

	delete []pCInfos;


	return;
*/	if (TellToSave() == IDCANCEL)
		return;

	CDlgNewMap NewMapDlg;

	CDlgNewMap::DATA DlgData;
	DlgData.bNewMap		= true;
	DlgData.fTileSize	= g_Configs.fTileSize;
	DlgData.fMinHeight	= g_Configs.fMinHeight;
	DlgData.fMaxHeight	= g_Configs.fMaxHeight;
	DlgData.strProjName	= "";

	NewMapDlg.SetData(DlgData);

	if (NewMapDlg.DoModal() == IDCANCEL)
		return;

	NewMapDlg.GetData(DlgData);

	g_Configs.fTileSize		= DlgData.fTileSize;
	g_Configs.fMinHeight	= DlgData.fMinHeight;
	g_Configs.fMaxHeight	= DlgData.fMaxHeight;
	
	//	Create new map object
	if (!CreateMap())
		return;

	CTerrain::INITDATA TrnInit;
	TrnInit.fTileSize	= DlgData.fTileSize;
	TrnInit.fMinHeight	= DlgData.fMinHeight;
	TrnInit.fMaxHeight	= DlgData.fMaxHeight;

	//	Initalize empty map
	if (!m_pMap->CreateEmpty(NewMapDlg.m_strProjName, TrnInit))
	{
		g_Log.Log("CMainFrame::OnFileNew, Failed to create empty map");
		return;
	}

	m_pMap->SetModifiedFlag(true);

	//	Bind map with view frame
	m_wndViewframe.SetMap(m_pMap);

	//	Update lay paint list\xqf
	m_ToolTabWnd.OnTrnLayerAddedOrDeleted();
	m_ToolTabWnd.HideObjectProperyPanel();
	//	Update main frame title
	UpdateMainFrameTitle();
	SetOtherOperation(false);
	m_ToolTabWnd.UpdatePlantsPanel();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	m_ToolTabWnd.RiseUpVernierButton();
	g_bReadOnly =           false;
}

//	Save current map
void CMainFrame::OnFileSave() 
{
	if(g_bReadOnly) return;

	CElementMap* pMap = m_wndViewframe.GetMap();
	if (!pMap)
		return;

	//	Get file name
	AString strFile = g_szWorkDir;
	strFile += pMap->GetMapFile();

	if (!SaveMap(strFile))
	{
		MessageBox("文件保存失败!");
		return;
	}

	if(!AUX_ModifyGlobalData(g_GlobalData))
		MessageBox("保存 Global data 失败!");
}

//	Save map as...
void CMainFrame::OnFileSaveas() 
{
	CElementMap* pMap = m_wndViewframe.GetMap();
	if (!pMap)
		return;

	CDlgSaveAs SaveDlg(pMap);
	if (SaveDlg.DoModal() == IDCANCEL)
		return;

	//	Get old project name
	CString strOldName = pMap->GetMapName();

	//	Change map name and save data
	m_pMap->ChangeMapName(SaveDlg.GetProjectName());
	OnFileSave();
	CString strOldFile;
	CString strNewFile;
	
	strOldFile.Format("%s%s\\%s\\%sRender.t2bk", g_szWorkDir, g_szEditMapDir, strOldName,strOldName);
	strNewFile.Format("%s%s\\%s\\%sRender.t2bk", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName(),SaveDlg.GetProjectName());
	CopyFile(strOldFile, strNewFile, FALSE);

	strOldFile.Format("%s%s\\%s\\%sRender.t2lm", g_szWorkDir, g_szEditMapDir, strOldName,strOldName);
	strNewFile.Format("%s%s\\%s\\%sRender.t2lm", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName(),SaveDlg.GetProjectName());
	CopyFile(strOldFile, strNewFile, FALSE);

	strOldFile.Format("%s%s\\%s\\%sRender.t2lm1", g_szWorkDir, g_szEditMapDir, strOldName,strOldName);
	strNewFile.Format("%s%s\\%s\\%sRender.t2lm1", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName(),SaveDlg.GetProjectName());
	CopyFile(strOldFile, strNewFile, FALSE);

	strOldFile.Format("%s%s\\%s\\%sRender.t2mk", g_szWorkDir, g_szEditMapDir, strOldName,strOldName);
	strNewFile.Format("%s%s\\%s\\%sRender.t2mk", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName(),SaveDlg.GetProjectName());
	CopyFile(strOldFile, strNewFile, FALSE);

	strOldFile.Format("%s%s\\%s\\%sRender.trn2", g_szWorkDir, g_szEditMapDir, strOldName,strOldName);
	strNewFile.Format("%s%s\\%s\\%sRender.trn2", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName(),SaveDlg.GetProjectName());
	CopyFile(strOldFile, strNewFile, FALSE);

	if (strOldName.CompareNoCase(SaveDlg.GetProjectName()))
	{
		//	Copy old light map file to new directory
		
		strOldFile.Format("%s%s\\%s\\LightMap.bmp", g_szWorkDir, g_szEditMapDir, strOldName);
		if (AUX_FileExist(strOldFile))
		{
			strNewFile.Format("%s%s\\%s\\LightMap.bmp", g_szWorkDir, g_szEditMapDir, SaveDlg.GetProjectName());
			CopyFile(strOldName, strNewFile, FALSE);
		}
	}

	//	Update main frame title
	UpdateMainFrameTitle();
}

//	Save map data to file
bool CMainFrame::SaveMap(const char* szFile)
{
	CElementMap* pMap = m_wndViewframe.GetMap();
	if (!pMap)
		return false;

	if (!pMap->Save(szFile))
	{
		AfxMessageBox("保存文件失败，请发log\\ElementEditor.log给程序");
	}
	
	pMap->SetModifiedFlag(false);

	return true;
}

//	Open a map
void CMainFrame::OnFileOpen()
{
	
	SetOtherOperation(false);
	m_ToolTabWnd.UpdatePlantsPanel();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	m_ToolTabWnd.RiseUpVernierButton();
	int n = m_StatusCombo.GetCount();
	if(n>0) m_StatusCombo.SetCurSel(0);
	m_strLastNeighborMap = "";
	//	Let user select a project file
	CString strPath = g_szWorkDir;
	strPath += g_szEditMapDir;

	CDlgSelectFile::PARAMS Params;
	Params.bMultiSelect	= false;
	Params.szFileFilter	= "*.elproj";
	Params.szPrompt		= "请选择一个项目文件";
	Params.szRootDir	= strPath;

	CDlgSelectFile FileDlg(Params);
	if (FileDlg.DoModal() != IDOK)
		return;

	if (TellToSave() == IDCANCEL)
		return;

	CString strFile = FileDlg.GetFullPathName();

	//	Create new map object
	if (!CreateMap())
		return;

/* repair probrom of model direction
 	AString strFileInfo;
	strFileInfo.Format("%s%s\\info.txt", g_szWorkDir, g_szEditMapDir);
	pFileTest.Open(strFileInfo, AFILE_TEXT|AFILE_OPENEXIST);
	pCInfos = new CorrectInfo[19999];
	int i;
	char strRead[99];
	DWORD iLenRead;
	while(pFileTest.ReadLine(strRead, 99, &iLenRead))
	{
		int index;
		sscanf(strRead, "%d", &index);
		CorrectInfo &pCurInfo = pCInfos[index];
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vr%f,%f,%f", &pCurInfo.r[0], &pCurInfo.r[1], &pCurInfo.r[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vu%f,%f,%f", &pCurInfo.u[0], &pCurInfo.u[1], &pCurInfo.u[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vl%f,%f,%f", &pCurInfo.l[0], &pCurInfo.l[1], &pCurInfo.l[2]);
		else
			break;
		if(pFileTest.ReadLine(strRead, 99, &iLenRead))
			sscanf(strRead, "vs%f,%f,%f", &pCurInfo.s[0], &pCurInfo.s[1], &pCurInfo.s[2]);
		else
			break;
		pCurInfo.bOK = true;
	}

	pFileTest.Close();
*/

	SetCursor(LoadCursor(NULL, IDC_WAIT));

	//	Load empty map
	if (!m_pMap->Load(strFile))
	{
		SetCursor(LoadCursor(NULL, IDC_ARROW));
		MessageBox("加载项目数据失败！");
		//	Reset undo manager
		g_UndoMan.Reset();
		
		m_pMap->Release();
		delete m_pMap;
		m_pMap = NULL;
		
		//	Unbind map and view frame
		m_wndViewframe.SetMap(NULL);
		return;
	}
	g_LogDlg.DoModal();
	SetCursor(LoadCursor(NULL, IDC_ARROW));

/* repair probrom of model direction
	delete []pCInfos;
*/
	//	Need to receate terrain render
	if (m_pMap->GetTerrain()->GetUpdateRenderFlag())
		MessageBox("地形渲染数据需要更新。");
	
	m_pMap->SetModifiedFlag(false);

	m_pMap->GetTerrain()->GetMaskModifier()->UpdateMaskMapToRender(ARectI(0,0,10000,10000));
#ifdef _ANGELICA22
	m_pMap->GetTerrain()->UpdateVertexColorMapToRender(ARectI(0,0,10000,10000)); 
#endif
	
	//	Bind map with view frame
	m_wndViewframe.SetMap(m_pMap);

	//	Set global directional light basing on map setting
	const LIGHTINGPARAMS& LtParams = m_pMap->GetLightingParams();
	g_Render.ChangeLight(LtParams.vSunDir, LtParams.dwSunCol, LtParams.dwAmbient);

	//Update lay paint list\xqf
	m_ToolTabWnd.OnTrnLayerAddedOrDeleted();
	m_ToolTabWnd.UpdateBrushHeightRect();
	
	//	Update main frame title
	UpdateMainFrameTitle();
	
	m_ToolTabWnd.HideObjectProperyPanel();

	g_GlobalData = AUX_GetGlobalData();

	// Release last g_PassMapGenerator object
}

//	Close current project
void CMainFrame::OnFileClose() 
{
	SetOtherOperation(false);
	m_ToolTabWnd.UpdatePlantsPanel();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	m_ToolTabWnd.RiseUpVernierButton();

	if (TellToSave() == IDCANCEL)
		return;

	//	Release map object
	ReleaseMap();

	//	Update main frame title
	UpdateMainFrameTitle();

	//  Hide all Panels
	m_ToolTabWnd.HideObjectProperyPanel();
	
	//  Reset scene objects list
	m_ToolTabWnd.UpdateSceneObjectList();
}

//	Edit terrain height map
void CMainFrame::OnTerrainHeightmap() 
{
	if (!m_wndViewframe.GetMap())
		return;
	
	CDlgHeightMap HMDlg(m_wndViewframe.GetMap()->GetTerrain());
	HMDlg.DoModal();

	if (HMDlg.GetModifiedFlag())
	{
		m_pMap->SetModifiedFlag(true);
		GetToolTabWnd()->UpdateBrushHeightRect();
	}
		
}

void CMainFrame::OnTerrainSuraces() 
{
	CElementMap* pMap = m_wndViewframe.GetMap();
	if (!pMap)
		return;

	if (pMap->GetTerrain()->GetMaskModifier()->HasModify())
	{
		//	Reload terrain render
		SetCursor(LoadCursor(NULL, IDC_WAIT));
		m_pMap->RecreateTerrainRender(false, false);
		SetCursor(LoadCursor(NULL, IDC_ARROW));
	}

	CDlgSurfaces SurfDlg(pMap->GetTerrain());
	SurfDlg.DoModal();

	if (SurfDlg.GetModifiedFlag())
	{
		pMap->SetModifiedFlag(true);

		if (SurfDlg.GetLayerAddDelFlag())
			m_ToolTabWnd.OnTrnLayerAddedOrDeleted();

		pMap->GetTerrain()->GetMaskModifier()->UpdateMaskMapToRender(ARectI(0,0,10000,10000));

	}
}

void CMainFrame::OnTerrainLighting() 
{
	CElementMap* pMap = m_wndViewframe.GetMap();
	if (!pMap)
		return;

	const LIGHTINGPARAMS& Params = pMap->GetLightingParams();
	const LIGHTINGPARAMS& NightParams = pMap->GetNightLightingParams();

	CDlgLighting::INITDATA Init;
	Init.bVecDir	= true;
	Init.dwAmbient	= Params.dwAmbient;
	Init.dwAmbient2 = Params.dwAmbient2;
	Init.dwSunCol	= Params.dwSunCol;
	Init.iMapSize	= Params.iMapSize;
	Init.vSunDir	= Params.vSunDir;
	Init.fSunPower  = Params.fSunPower;
	Init.fAmbientPower = Params.fAmbientPower;

	Init.dwNightAmbient	= NightParams.dwAmbient;
	Init.dwNightAmbient2= NightParams.dwAmbient2;
	Init.dwNightSunCol	= NightParams.dwSunCol;
	Init.iNightMapSize	= NightParams.iMapSize;
	Init.vNightSunDir	= NightParams.vSunDir;
	Init.fNightSunPower = NightParams.fSunPower;
	Init.fNightAmbientPower = NightParams.fAmbientPower;

	CDlgLighting LtDlg(Init);
	if (LtDlg.DoModal() == IDOK)
	{
		const LIGHTINGPARAMS& Result = LtDlg.GetParams();
		const LIGHTINGPARAMS& NightResult = LtDlg.GetNightParams();
		if(Result.iMapSize != NightResult.iMapSize)
		{
			MessageBox("白天夜晚的灯光图的尺寸必须一样！设置无效！");
			return;
		}
		m_pMap->SetLightingParams(Result);
		m_pMap->SetNightLightingParams(NightResult);
		m_pMap->SetModifiedFlag(true);
		if(g_bIsNight) g_Render.ChangeLight(NightResult.vSunDir, NightResult.dwSunCol, NightResult.dwAmbient);
		else g_Render.ChangeLight(Result.vSunDir, Result.dwSunCol, Result.dwAmbient);
		
#ifdef _ANGELICA22
		if(pMap->GetTerrain()->GetColorMap()->iWidth != Result.iMapSize)
			pMap->GetTerrain()->RecreateColorMap(Result.iMapSize,Result.iMapSize);
#endif
	}
}

void CMainFrame::OnTerrainCreatesurfs() 
{
	bool bLighting = MessageBox("更新渲染数据的同时重新生成光照图吗？这可能需要比较长的时间。", "提示", MB_YESNO) == IDYES ? true : false;
	
	//  Create EditorTempMap folder
	char szPath[128];
	sprintf(szPath,"%sEditMaps\\%s\\EditorTempMap",g_szWorkDir,g_Configs.szCurProjectName);
	if(!PathFileExists(szPath))
	{
		if(!CreateDirectory(szPath, NULL))
		{
			MessageBox("创建临时文件夹失败，无法进行地形光照计算和地形更新!");
			return;
		}
	}
	//	Reload terrain render
	SetCursor(LoadCursor(NULL, IDC_WAIT));
	m_pMap->RecreateTerrainRender(false, bLighting);
	m_pMap->SaveLitModel();
	SetCursor(LoadCursor(NULL, IDC_ARROW));
}

void CMainFrame::OnSysCommand(UINT nID, LPARAM lParam)
{
	if (nID == SC_CLOSE)
	{
		if (!OnAppExit())
			return;
	}

	CFrameWnd::OnSysCommand(nID, lParam);
}

//	Called when application is being closed
//	Return true to really exit application, return false to canel
bool CMainFrame::OnAppExit()
{
	if (TellToSave() == IDCANCEL)
		return false;

	//	Save all edit configs
	AUX_SaveConfigs();

	return true;
}

//	Initialize device objects
bool CMainFrame::InitDeviceObjects()
{
	A3DDevice* pA3DDevice = g_Render.GetA3DDevice();

	//	Apply fog parameters
	pA3DDevice->SetFogEnable(g_Configs.bEnableFog);
//	pA3DDevice->SetFogTableEnable(false);
	pA3DDevice->SetFogStart(50.0f);
	pA3DDevice->SetFogEnd(500.0f);
	pA3DDevice->SetFogDensity(0.5f);
	pA3DDevice->SetFogColor(A3DCOLORRGBA(255,255,255,255));
	
	//	Decide maximum texture size
	g_Configs.iMaxTexSize = a_Min(pA3DDevice->GetMaxTextureWidth(), pA3DDevice->GetMaxTextureHeight());

	if (!m_wndViewframe.InitDeviceObjects())
	{
		g_Log.Log("CMainFrame::InitDeviceObjects, Failed to call CViewFrame::InitDeviceObjects");
		return false;
	}

	if (!m_ToolTabWnd.InitDeviceObjects())
	{
		g_Log.Log("CMainFrame::InitDeviceObjects, Failed to call CToolTabWnd::InitDeviceObjects");
		return false;
	}

	return true;
}

//	When a model dialog or menu is active, OnEnterIdle will be called
void CMainFrame::OnEnterIdle(UINT nWhy, CWnd* pWho) 
{
	CFrameWnd::OnEnterIdle(nWhy, pWho);
	
	Tick();	
}

//	Tick routine
bool CMainFrame::Tick()
{
	if (!AppIsActive())
		return true;

	m_dwTickTime = m_dwLastFrame ? a_GetTime() - m_dwLastFrame : 0;

	if (m_dwTickTime)
	{
		m_dwLastFrame = a_GetTime();

		a_ClampRoof(m_dwTickTime, (DWORD)80);

		//	Do frame move
		if (m_pMap)
			m_pMap->FrameMove(m_dwTickTime);

		//	Do render
		m_wndViewframe.FrameMove(m_dwTickTime);
		m_wndViewframe.Render();
	}

	return true;
}

//	Tell user to save map file
int CMainFrame::TellToSave()
{
	if (m_pMap && m_pMap->GetModifiedFlag())
	{
		//	Notify user to save map data
		int iRet = MessageBox("地图文件已被修改，是否保存？", "提示", MB_YESNOCANCEL);
		if (iRet == IDYES)
		{
			//	Save map data
			OnFileSave();
		}

		return iRet;
	}

	return IDNO;
}

//	Export game data
void CMainFrame::OnFileExportgamedata() 
{

/* repair probrom of model direction
	// 修改编辑器导致老版本数据用新编辑器打开以后模型旋转角度和缩放大小信息丢失，这段代码统计下有多少有问题
	AString strFileInfo;
	strFileInfo.Format("%s%s\\info.txt", g_szWorkDir, g_szEditMapDir);
	pFileTest.Open(strFileInfo, AFILE_TEXT|AFILE_CREATENEW);

	CElementMap pMap;
	CString strProjName;
	strProjName.Format("%s%s\\1\\1.elproj", g_szWorkDir,g_szEditMapDir);
	if(!pMap.Load(strProjName))
		g_Log.Log("Open file %s failed!",strProjName);
	pMap.Release();

	pFileTest.Close();


	return;
*/
	CExpGameDataDlg ExpDlg;
	ExpDlg.DoModal();
}

void CMainFrame::OnFileAutoparthm() 
{
	CDlgAutoPartHM Dlg;
	Dlg.DoModal();
}

//Only set brush command
void CMainFrame::OnBrushCircle()
{
	g_Configs.bCircleBrush = true;
	m_ToolTabWnd.OnBrushOptionPanel();
}

//Only set brush command
void CMainFrame::OnBrushRadius1()
{
	g_Configs.iBrushRadius = 5;
	m_ToolTabWnd.OnBrushOptionPanel();
	UpdateBrushWidth();
}

//Only set brush command
void CMainFrame::OnBrushRadius2()
{
	g_Configs.iBrushRadius = 10;
	m_ToolTabWnd.OnBrushOptionPanel();
	UpdateBrushWidth();
}

//Only set brush command
void CMainFrame::OnBrushRadius3()
{
	g_Configs.iBrushRadius = 15;
	m_ToolTabWnd.OnBrushOptionPanel();
	UpdateBrushWidth();
}

//Only set brush command
void CMainFrame::OnBrushRadiusAdd()
{
	g_Configs.iBrushRadius++;
	if(g_Configs.iBrushRadius>100) g_Configs.iBrushRadius = 100;
	m_ToolTabWnd.OnBrushOptionPanel();
	UpdateBrushWidth();
}

//Only set brush command
void CMainFrame::OnBrushRadiusSub()
{
	g_Configs.iBrushRadius--;
	if(g_Configs.iBrushRadius<1) g_Configs.iBrushRadius = 1;
	m_ToolTabWnd.OnBrushOptionPanel();
	UpdateBrushWidth();
}

//Only set brush command
void CMainFrame::OnBrushRect()
{
	g_Configs.bCircleBrush = false;
	m_ToolTabWnd.OnBrushOptionPanel();
}

void CMainFrame::UpdateBrushWidth()
{
	CString str;
	str.Format("Brush radius = %dm", g_Configs.iBrushRadius);
	m_wndStatusBar.SetPaneText(1, str, TRUE);
}

void CMainFrame::UpdateBrushHeight(float fHeight)
{
	m_ToolTabWnd.UpdateBrushCenterHeight(fHeight);
}

void CMainFrame::OnOperationRedo() 
{
	// TODO: Add your command handler code here
	g_UndoMan.Redo();
}

void CMainFrame::OnOperationUndo() 
{
	// TODO: Add your command handler code here
	g_UndoMan.Undo();
}

void CMainFrame::OnUpdateOperationUndo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_UndoMan.GetUndoListActionNum()<=0) pCmdUI->Enable(false);
	else pCmdUI->Enable(!g_bLockCam);
}

void CMainFrame::OnUpdateOperationRedo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_UndoMan.GetRedoListActionNum()<=0) pCmdUI->Enable(false);
	else pCmdUI->Enable(!g_bLockCam);
}

//	Update main frame title
void CMainFrame::UpdateMainFrameTitle()
{
	CString str;

	CString strOversea;
	if(g_Configs.iIsOverseaVersion == 0)
	{
		strOversea = " ---- 【国内版】----";
	}
	else if(g_Configs.iIsOverseaVersion == 1)
	{
		strOversea = " ★★★★ 【海外版】 ★★★★ ";
	}
	else
		ASSERT(0);


	if (!m_pMap)
	{
		str.Format("诛仙 场景编辑器 v%s %s %s",g_szVersion, g_szCompileTime, strOversea);
		SetWindowText(str);
	}
	else
	{
		
		str.Format("诛仙 场景编辑器 v%s %s - %s%s\\%s %s",g_szVersion, g_szCompileTime, g_szWorkDir, g_szEditMapDir, m_pMap->GetMapName(), strOversea);
		SetWindowText(str);
	}
}


void CMainFrame::OnOperationSelect() 
{
	// TODO: Add your command handler code here
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	Aux_RestoreCamera();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SELECT);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 1;
	
	
}

void CMainFrame::OnOperationMove() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_MOVE);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 2;
	
}

void CMainFrame::OnUpdateOperationMove(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	
	if(m_nSceneObjectOperation==2) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::OnOperationRotate() 
{
	// TODO: Add your command handler code here
		Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 3;
	
}

void CMainFrame::OnUpdateOperationRotate(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	
	if(m_nSceneObjectOperation==3) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::OnUpdateOperationSelect(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_nSceneObjectOperation==1) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::OnOperationSelectmulti() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_MULTISELECT);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 4;
	
}

void CMainFrame::OnUpdateOperationSelectmulti(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_nSceneObjectOperation==4) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::SetOtherOperation(bool bUseOther)
{ 
	Aux_RestoreCamera();
	m_bOtherOperation = bUseOther; 
	if(m_bOtherOperation)
	{
		if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
		if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
		if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	}
	COperation *pOperation = NULL;
	if(m_nSceneObjectOperation == 1)
	{//selected
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SELECT);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else 
	if(m_nSceneObjectOperation == 2)
	{//MOVE
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_MOVE);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 3)
	{//ROTATE
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 4)
	{//Multi selected
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_MULTISELECT);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 5)
	{//Scale
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SCALE);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 6)
	{// bezier delete a point
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_DELETE_POINT);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 7)
	{// bezier cut segment
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_CUT_SEGMENT);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 8)
	{// bezier add point
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_ADD_POINT);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 9)
	{//drag bezier
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_DRAG);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 10)
	{//Test bezier
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_TEST);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 11)
	{// Bezier merge
		pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_MERGE);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	if(m_nSceneObjectOperation == 12)
	{// rotate2
		pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE2);
		m_wndViewframe.SetCurrentOperation(pOperation);
	}else
	{
		m_wndViewframe.SetCurrentOperation(0);
	}
};

void CMainFrame::EnableBezierOperation(bool bEnable)
{
	m_bEnableBezierOperation = bEnable;
}

/*
LRESULT CMainFrame::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(message==WM_KEYDOWN)
	{
		if(lParam == VK_DELETE)
		{
			if(IDOK==MessageBox("你要删除当前选择的对象吗？",NULL,MB_OKCANCEL))
			{

			}
		}
	}
	return CFrameWnd::WindowProc(message, wParam, lParam);
}
*/


void CMainFrame::OnOperationScale() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_SCALE);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 5;
}

void CMainFrame::OnUpdateOperationScale(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_nSceneObjectOperation==5) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::OnOperationDelete() 
{
	// TODO: Add your command handler code here
    if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount()>0)
		{
			Aux_RestoreCamera();
			CDelObjectDlg dlg;
			if(IDOK==dlg.DoModal())
			{
				CUndoSceneObjectDeleteAction *pUndo = new CUndoSceneObjectDeleteAction(m_pMap->GetSceneObjectMan());
				if(pUndo->GetSeletedListData())
				{
					g_UndoMan.AddUndoAction(pUndo);
					m_ToolTabWnd.UpdateSceneObjectList();
					m_ToolTabWnd.HideObjectProperyPanel();
				}else 
				{
					pUndo->Release();
					delete pUndo;
				}
			}
		}
	}
	
}


void CMainFrame::OnBezierAddpoint() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_ADD_POINT);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 8;
}

void CMainFrame::OnBezierCut() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_CUT_SEGMENT);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 7;
}

void CMainFrame::OnBezierSmooth() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_pMap)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				if(!((CEditerBezier*)pObj)->IsBezier()) return;
				//记录Undo
				CBezierUndoAction *pUndo = new CBezierUndoAction();
				pUndo->StartRecord();
				g_UndoMan.AddUndoAction(pUndo);
				((CEditerBezier *)pObj)->SmoothBezier();
			}
		}
	}
		
}

void CMainFrame::OnBezierSubpoint() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_DELETE_POINT);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 6;
}

void CMainFrame::OnUpdateBezierSubpoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				pCmdUI->Enable(true);
				
				if(m_nSceneObjectOperation==6) 
				{
					if(!m_bOtherOperation)
						pCmdUI->SetCheck(true);
					else pCmdUI->SetCheck(false);
				}
				else pCmdUI->SetCheck(false);

				return;
			}
		}
	}
	pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateBezierCut(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	/*
	if(m_pMap)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				pCmdUI->Enable(true);
				
				if(m_nSceneObjectOperation==7) 
				{
					if(!m_bOtherOperation)
						pCmdUI->SetCheck(true);
					else pCmdUI->SetCheck(false);
				}
				else pCmdUI->SetCheck(false);

				return;
			}
		}
	}*/
	pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateBezierAddpoint(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				pCmdUI->Enable(true);
				
				if(m_nSceneObjectOperation==8) 
				{
					if(!m_bOtherOperation)
						pCmdUI->SetCheck(true);
					else pCmdUI->SetCheck(false);
				}
				else pCmdUI->SetCheck(false);

				return;
			}
		}
	}
	pCmdUI->Enable(false);
}

void CMainFrame::OnBezierDrag() 
{
	// TODO: Add your command handler code here
	Aux_RestoreCamera();
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_DRAG);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 9;
	
}

void CMainFrame::OnUpdateBezierDrag(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code her
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				pCmdUI->Enable(true);
				
				if(m_nSceneObjectOperation==9) 
				{
					if(!m_bOtherOperation)
						pCmdUI->SetCheck(true);
					else pCmdUI->SetCheck(false);
				}
				else pCmdUI->SetCheck(false);

				return;
			}
		}
	}
	pCmdUI->Enable(false);
}

void CMainFrame::OnBezierTest() 
{
	// TODO: Add your command handler code here
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_TEST);
	m_wndViewframe.SetCurrentOperation(pOperation);
	if(m_nSceneObjectOperation != 10)
	{
		m_nSceneObjectOperation = 10;
		Aux_CaptureCamera();
		((CBezierTestOperation*)pOperation)->Play();
	}else 
	{
		m_nSceneObjectOperation = 2;
		((CBezierTestOperation*)pOperation)->Stop();
		SetOtherOperation(false);
		Aux_RestoreCamera();
	}
	
}

void CMainFrame::OnUpdateBezierTest(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				if(((CEditerBezier*)pObj)->IsBezier())
				{
					pCmdUI->Enable(true);
					
					if(m_nSceneObjectOperation==10) 
					{
						if(!m_bOtherOperation)
							pCmdUI->SetCheck(true);
						else pCmdUI->SetCheck(false);
					}
					else pCmdUI->SetCheck(false);
					
					return;
				}
			}
		}
	}
	pCmdUI->Enable(false);
}


void CMainFrame::OnUpdateBezierSmooth(CCmdUI* pCmdUI) 
{
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				if(((CEditerBezier*)pObj)->IsBezier())
				{
					pCmdUI->Enable(true);
					return;
				}
			}
		}
	}
	pCmdUI->Enable(false);
}

void CMainFrame::OnButtonNospeak() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		g_Configs.bEnableSpeak = !g_Configs.bEnableSpeak;
		if(!g_Configs.bEnableSpeak)
		{
			g_BackMusic.StopMusic(true,true);
			g_BackMusic.StopBackSFX();
			CScenePrecinctObject *pObj = m_pMap->GetSceneObjectMan()->GetCurrentPrecinct();
			if(pObj) pObj->StopBackMusic();
		}
	}
}

void CMainFrame::OnUpdateButtonNospeak(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(g_Configs.bEnableSpeak)
		pCmdUI->SetCheck(0);
	else pCmdUI->SetCheck(1);
}

void CMainFrame::OnButtonShowVernier() 
{
	// TODO: Add your command handler code here
	g_Configs.bShowVernier = !g_Configs.bShowVernier;
}

void CMainFrame::OnUpdateButtonShowVernier(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_Configs.bShowVernier);
}


void CMainFrame::OnEditObjectCopy() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		CSceneObjectManager *pMan = m_pMap->GetSceneObjectMan();
		if(IDYES==MessageBox("你确信要复制选择的所有对象吗？","",MB_YESNO))
		{
			
			std::vector<CSceneObject *> selObject;
			ALISTPOSITION pos = pMan->m_listSelectedObject.GetTailPosition();
			while(pos)
			{
				CSceneObject* ptemp = pMan->m_listSelectedObject.GetPrev(pos);
				CSceneObject* pNew = ptemp->CopyObject();
				if(pNew) 
				{
					pNew->SetSelected(false);
					selObject.push_back(pNew);
				}
			}
			
			pMan->ClearSelectedList();
			int n = selObject.size();
			for( int i = 0; i < n ; i++)
			{
				CSceneObject *ptemp = selObject.at(i);
				pMan->AddObjectPtrToSelected(ptemp);
			}
			selObject.clear();
		}
	}
}

void CMainFrame::OnFileSortMap() 
{
	// TODO: Add your command handler code here
	CDlgExpGameData ExpDlg;
	ExpDlg.DoModal();
}

void CMainFrame::OnTerrainModifiedProfile() 
{
	// TODO: Add your command handler code here
	if(g_bReadOnly) return;
	if(m_pMap)
	{
		CModifiedProfileDlg dlg;
		dlg.SetTerrain(m_pMap->GetTerrain());
		dlg.DoModal();	
	}
}

void CMainFrame::OnMapAutoMerge() 
{
	// TODO: Add your command handler code here
	if(g_bReadOnly) return;
	CTerrainConnectDlg dlg;
	dlg.DoModal();	
}

void CMainFrame::OnUpdateTerrainModifiedProfile(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateMapAutoMerge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CMainFrame::OnOperationObstruct() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				if(((CEditerBezier*)pObj)->IsBezier()) 
				{
					((CEditerBezier*)pObj)->ConvertToObstruct();
				}
			}
		}
	}
}

void CMainFrame::OnBezierMerge() 
{
	// TODO: Add your command handler code here
	//m_bShowMergeDlg = !m_bShowMergeDlg;
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	g_BezierMergeDlg.UpdateUI();
	g_BezierMergeDlg.ShowWindow(true);
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_BEZIER_MERGE);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 11;
}

void CMainFrame::OnUpdateBezierMerge(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap && !g_bLockCam)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 2)
		{
			CSceneObject * pObj1 = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			CSceneObject * pObj2 = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetHead();
			if(pObj1->GetObjectType() == CSceneObject::SO_TYPE_BEZIER && pObj2->GetObjectType() == CSceneObject::SO_TYPE_BEZIER)
			{
				if(((CEditerBezier*)pObj1)->IsBezier() == ((CEditerBezier*)pObj2)->IsBezier()) 
				{
					pCmdUI->Enable(true);
					if(m_nSceneObjectOperation==11) 
					{
						if(!m_bOtherOperation)
							pCmdUI->SetCheck(true);
						else pCmdUI->SetCheck(false);
					}
					return;
				}
			}
		}
	}
	pCmdUI->Enable(false);
	pCmdUI->SetCheck(false);
}



void CMainFrame::OnUpdateOperationObstruct(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_bEnableBezierOperation);
	if(!m_bEnableBezierOperation) return;
}

void CMainFrame::OnExpAiData() 
{
	if(!m_pMap) return;
	
	char szFilters[]= "AI Generator (*.TAI)|*.TAI|";
 
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (false, "TAI", "*.TAI",
      OFN_HIDEREADONLY|OFN_NOCHANGEDIR, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
	   CString pathName = fileDlg.GetPathName();
	   AFile File;
	   if (!File.Open(pathName, AFILE_CREATENEW))
	   {
		   g_Log.Log("CMainFrame::OnExpAiData, Failed to open file %s.", pathName);
		   return;
	   }
	   m_pMap->GetSceneObjectMan()->ExpAiData(&File);
	   File.Close();
	   
   }
}

void CMainFrame::OnImpAiData() 
{
	if(!m_pMap) return;
	
	// TODO: Add your command handler code here
	char szFilters[]= "AI Generator (*.TAI)|*.TAI|";
 
	// Create an Open dialog; the default file name extension is ".my".
	CFileDialog fileDlg (true, "TAI", "*.TAI",
      OFN_FILEMUSTEXIST| OFN_HIDEREADONLY|OFN_NOCHANGEDIR, szFilters, this);
   
   // Display the file dialog. When user clicks OK, fileDlg.DoModal() 
   // returns IDOK.
   if( fileDlg.DoModal ()==IDOK )
   {
	   CString pathName = fileDlg.GetPathName();
	   AFile File;
	   if (!File.Open(pathName, AFILE_OPENEXIST))
	   {
		   g_Log.Log("CMainFrame::OnImpAiData, Failed to open file %s.", pathName);
		   return;
	   }
	   m_pMap->GetSceneObjectMan()->ImpAiData(&File);
	   File.Close();
	   
   }
}

void CMainFrame::OnUpdateImpAiData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CMainFrame::OnUpdateExpAiData(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
}

void CMainFrame::OnTerrainBlockView() 
{
	if(m_pMap)
	{
		g_BlockViewDlg.ShowWindow(true);
		if(m_pMap->GetTerrain()->GetMaskModifier()->HasModify())
			m_pMap->RecreateTerrainRender(false,false);
		g_BlockViewDlg.UpdateDlg();
	}
}

void CMainFrame::OnDayNight() 
{
	// TODO: Add your command handler code here
	g_bShowCollideModelOnly = !g_bShowCollideModelOnly;
}

void CMainFrame::OnUpdateDayNight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowCollideModelOnly);
}

void CMainFrame::OnUpdateTerrainBlockView(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap)
		pCmdUI->Enable(true);
	else pCmdUI->Enable(false);
	
}

void CMainFrame::OnButtonRain() 
{
	// TODO: Add your command handler code here
	g_Configs.bShowRain = !g_Configs.bShowRain;
}

void CMainFrame::OnUpdateButtonRain(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_Configs.bShowRain);
}

void CMainFrame::OnButtonSnow() 
{
	// TODO: Add your command handler code here
	g_Configs.bShowSnow = !g_Configs.bShowSnow;
}

void CMainFrame::OnUpdateButtonSnow(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_Configs.bShowSnow);
}


void CMainFrame::OnMenuResMan() 
{
	// TODO: Add your command handler code here
	CResManDlg dlg;
	dlg.DoModal();
	
}

void CMainFrame::OnSetVss() 
{
	// TODO: Add your command handler code here
	CVssOptionDlg dlg;
	if(IDOK==dlg.DoModal())
	{
		((CElementEditorApp*)AfxGetApp())->InitVss();
	}
}

void CMainFrame::OnGetVssAll() 
{
	BeginWaitCursor();
	// TODO: Add your command handler code here
	CString str = CString(g_szWorkDir);
	g_VSS.SetWorkFolder(str);
	
	g_VSS.SetProjectPath("Building");
	g_VSS.GetAllFiles("Building");
	
	g_VSS.SetProjectPath("ResTable");
	g_VSS.GetAllFiles("ResTable");
	
	g_VSS.SetProjectPath("");
	g_DynamicObjectMan.Load();
	MessageBox("资源更新完成!");
	EndWaitCursor();
}

void CMainFrame::OnTestMapCheckin() 
{
	// TODO: Add your command handler code here
	if(m_pMap && !g_bReadOnly)
	{
		if(IDNO==MessageBox("你真的要把当前地图所有改动保存到服务器?","注意",MB_YESNO))
			return;
		OnFileSave(); 
		CString strMap = m_pMap->GetMapName();
		
		if(!AUX_ModifiedResTable())
			return;
		
		if(!AUX_IncreaseVersion(m_pMap->GetMapName()))
		{
			MessageBox("不能更新跟踪数据!请检查地图是否已经Checkout!");
			return;
		}
		CString dst,src;
		if(!AUX_CheckInMap(strMap.GetBuffer(0)))
		{//new a vss project
			if(IDYES == MessageBox("如果这是一个全新地图，你需要把它加入服务器请选是，否则选否？",NULL,MB_YESNO))
			{
				CString strTemp,str,str1;
				strTemp.Format("%s",g_szEditMapDir);
				g_VSS.SetProjectPath(strTemp);
				strTemp.Format("%s\\%s",g_szEditMapDir,m_pMap->GetMapName());
				g_VSS.CreateProject(m_pMap->GetMapName());
				g_VSS.SetProjectPath(strTemp);
				
				for( int i = 0; i < 26; ++i)
				{
					str1.Format("%s\\%s",g_szEditMapDir,m_pMap->GetMapName());
					str.Format(sz_map_file_ex[i],m_pMap->GetMapName());
					str = str1 + "\\" + str;
					if(!AUX_FileIsExist(str))
					{
						FILE *pFile = fopen(str,"wb");
						DWORD dwFlag = 2121212;
						fwrite(&dwFlag,sizeof(DWORD),1,pFile);
						if(pFile) fclose(pFile);
					}
					g_VSS.AddFile(str);
				}
				strTemp.Format("%s\\%s\\MaskMaps.data",g_szEditMapDir,m_pMap->GetMapName());
				g_VSS.AddFile(strTemp);
				
			}else return;

		}
		src.Format("%s%s\\%s",g_szWorkDir,g_szEditMapDir,strMap);
		dst.Format("%s\\MapsData\\%s\\%s",g_Configs.szServerPath,g_Configs.szCurProjectName,strMap);
		if(!AUX_CopyFolder(dst,src)) 
		{
			AUX_CheckOutMap(strMap.GetBuffer(0));
			return;
		}
		strMap.ReleaseBuffer();
		//注意：这儿把地图下多出的文件加入到VSS，比如MASK BITMAP,但是如果是减少就没发了哟。
		//这儿以后可以改动，这样就需要一个文件列表什么的保存到服务器。就总体来看，这些数据
		//只是为了编辑，而导出后数据不会有多余，因此也可以不改动。
		OnFileClose();
	}
}

void CMainFrame::OnExportGameRes() 
{
	// TODO: Add your command handler code here
	CTerrainResExpDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnUpdateTestMapCheckin(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_VSS.IsInited());
}

void CMainFrame::OnUpdateGetVssAll(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_VSS.IsInited());
}

void CMainFrame::OnUpdateMenuResMan(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(g_VSS.IsInited());
}

void CMainFrame::OnMoveAllSceneObject() 
{
	// TODO: Add your command handler code here
	CMoveAllObjHeightDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnGetAllMapData() 
{
	BeginWaitCursor();
	CString str;
	str.Format("%sEditMaps",g_szWorkDir);
	g_VSS.SetWorkFolder(str);
	g_VSS.SetProjectPath("EditMaps");
	g_VSS.GetFile("EditMaps\\workspace.ws");
	
	std::vector<CString> listWorkspace;
	// Create folder
	AScriptFile File;
	if (File.Open("EditMaps\\workspace.ws"))
	{//Clear current project list
		CString path;
		while (File.GetNextToken(true))
		{
			path.Format("%sEditMaps\\%s",g_szWorkDir,File.m_szToken);
			if(!PathFileExists(path))
			{
				if(!CreateDirectory(path, NULL))
				{
					MessageBox("CMainFrame::OnGetAllMapData() ,创建取出路径失败！");
					listWorkspace.clear();
					return;
				}
			}
			path.Format("%sEditMaps\\%s",g_szWorkDir,File.m_szToken);
			g_VSS.SetWorkFolder(path);
			path.Format("EditMaps\\%s",File.m_szToken);
			g_VSS.SetProjectPath(path);
			path.Format("EditMaps\\%s\\projlist.dat",File.m_szToken);
			g_VSS.GetFile(path);
			listWorkspace.push_back(CString(File.m_szToken));
		}
		File.Close();
	}
	// TODO: Add your command handler code here
	for( int i = 0; i < listWorkspace.size(); i++)
	{
		str.Format("%sEditMaps\\%s",g_szWorkDir,listWorkspace[i]);
		g_VSS.SetWorkFolder(str);
		str.Format("EditMaps\\%s",listWorkspace[i]);
		g_VSS.SetProjectPath(str);
		g_VSS.GetAllFiles(str);
		g_VSS.SetProjectPath("");
		// Load project file list
		DWORD dwCol;
		char szProj[256];
		sprintf(szProj,"EditMaps\\%s\\ProjList.dat",listWorkspace[i]);
		if (!File.Open(szProj))
		{
			g_Log.Log("Open file %s failed!",szProj);
			continue;
		}
		
		if(File.GetNextToken(true))
		{
			CString strCol(File.m_szToken);
			if(strCol.Find("col=")!=-1)
			{
				strCol.Replace("col=","");
				dwCol = (int)atof(strCol);
			}else
			{
				File.ResetScriptFile();
				dwCol = 1;
			}
		}
		
		while (File.GetNextToken(true))
		{
			CString dstPath,srcPath,strFilePathName;
			strFilePathName.Format("%sEditMaps\\%s\\%s\\%s.elproj",g_szWorkDir,listWorkspace[i],File.m_szToken,File.m_szToken);
			if(AUX_FileIsReadOnly(strFilePathName))
			{//如果文件已经被本地取出，本地就是最新的，就不能覆盖本地的地图文件
				dstPath.Format("%sEditMaps\\%s\\%s",g_szWorkDir,listWorkspace[i],File.m_szToken);
				srcPath.Format("%s\\MapsData\\%s\\%s",g_Configs.szServerPath,listWorkspace[i],File.m_szToken);
				AUX_CopyFolder(dstPath,srcPath);
			}
		}
		File.Close();
	}
	MessageBox("地图更新完成!");
	EndWaitCursor();
}


void CMainFrame::OnExpTopviewMap() 
{
	// TODO: Add your command handler code here
	CTerrainResExpDlg dlg;
	dlg.nExpType=exp_topview;
	bool bShowAIArea=g_Configs.bShowAIArea;
	bool bShowAudio=g_Configs.bShowAudio;
	bool bShowBoxArea=g_Configs.bShowBoxArea;
	bool bShowGrass=g_Configs.bShowGrass;
	bool bShowCritterGroup=g_Configs.bShowCritterGroup;
	bool bShowDummy=g_Configs.bShowDummy;
	bool bShowLight=g_Configs.bShowLight;
	bool bShowGather=g_Configs.bShowGather;
	bool bShowPath=g_Configs.bShowPath;
	bool bShowFixedNpc=g_Configs.bShowFixedNpc;
	bool bShowFixedNpcWire=g_Configs.bShowFixedNpcWire;
	bool bShowObjectName=g_Configs.bShowObjectName;
	bool bShowSkinModel=g_Configs.bShowSkinModel;
	bool bShowSpecially=g_Configs.bShowSpecially;
	bool bShowRange=g_Configs.bShowRange;
	bool bShowCloudBox=g_Configs.bShowCloudBox;
	bool bShowCloudLine=g_Configs.bShowCloudLine;
	bool bShowInstanceBox=g_Configs.bShowInstanceBox;
	bool bShowDynamic = g_Configs.bShowDynamic;
	
	g_Configs.bShowAIArea = false;
	g_Configs.bShowAudio = false;
	g_Configs.bShowBoxArea = false;
	g_Configs.bShowGrass = false;
	g_Configs.bShowCritterGroup = false;
	g_Configs.bShowDummy = false;
	g_Configs.bShowLight = false;
	g_Configs.bShowGather = false;
	g_Configs.bShowPath = false;
	g_Configs.bShowFixedNpc = false;
	g_Configs.bShowFixedNpcWire = false;
	g_Configs.bShowObjectName = false;
	g_Configs.bShowSkinModel = false;
	g_Configs.bShowSpecially = false;
	g_Configs.bShowRange = false;
	g_Configs.bShowCloudBox = false;
	g_Configs.bShowCloudLine = false;
	g_Configs.bShowInstanceBox = false;
	g_Configs.bShowDynamic = false;
	g_bShowPosDot = false;

	dlg.DoModal();

	g_Configs.bShowAIArea=bShowAIArea;
	g_Configs.bShowAudio=bShowAudio;
	g_Configs.bShowBoxArea=bShowBoxArea;
	g_Configs.bShowGrass=bShowGrass;
	g_Configs.bShowCritterGroup=bShowCritterGroup;
	g_Configs.bShowDummy=bShowDummy;
	g_Configs.bShowLight=bShowLight;
	g_Configs.bShowGather=bShowGather;
	g_Configs.bShowPath=bShowPath;
	g_Configs.bShowFixedNpc=bShowFixedNpc;
	g_Configs.bShowFixedNpcWire=bShowFixedNpcWire;
	g_Configs.bShowObjectName=bShowObjectName;
	g_Configs.bShowSkinModel=bShowSkinModel;
	g_Configs.bShowSpecially=bShowSpecially;
	g_Configs.bShowRange=bShowRange;
	g_Configs.bShowCloudBox=bShowCloudBox;
	g_Configs.bShowCloudLine=bShowCloudLine;
	g_Configs.bShowInstanceBox=bShowInstanceBox;
	g_Configs.bShowDynamic = bShowDynamic;
	g_bShowPosDot = true;
}

void CMainFrame::OnOperationTestBeizerIntersect() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount() == 1)
		{
			CSceneObject * pObj1 = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			if(pObj1->GetObjectType() == CSceneObject::SO_TYPE_BEZIER )
			{
				if(((CEditerBezier*)pObj1)->IntersectWithSelf())
					AfxMessageBox("该曲线自相交,请修改头点与尾点的位置！");
				else AfxMessageBox("该曲线正常!");
			}
		}
	}
}

void CMainFrame::OnTestPrecinct() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		bool error = false;
		CELBitmap *pPrecinctMap = m_pMap->ExpPrecinctMap();
		if(pPrecinctMap==NULL) 
		{
			MessageBox("检测失败!不能生成区域位图.");
			return;
		}
		BYTE *pData = pPrecinctMap->GetBitmapData();
		for( int w  = 0; w < pPrecinctMap->GetWidth(); w++)
			for( int h = 0; h < pPrecinctMap->GetHeight(); h++)
				if(pData[h*pPrecinctMap->GetWidth() + w]!=0)
				{
					error = true;
					break;
				}
		if(error)
		{
			pPrecinctMap->SaveToFile("precinct.bmp");
			MessageBox("区域未完全覆盖!请查看编辑器路径下的Precinct.bmp");
		}else MessageBox("区域覆盖所有范围!");
		pPrecinctMap->Release();
		delete pPrecinctMap;
	}
}
 

void CMainFrame::OnTestPrecinctError() 
{
	// TODO: Add your command handler code here
	CTerrainResExpDlg dlg;
	dlg.nExpType=exp_test_precinct;
	dlg.DoModal();
}

void CMainFrame::OnShowModelPlane() 
{
	// TODO: Add your command handler code here
	g_bShowModelPlane = !g_bShowModelPlane;
}

void CMainFrame::OnUpdateShowModelPlane(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowModelPlane);
}

void CMainFrame::OnMergeTreeType() 
{
	// TODO: Add your command handler code here
	if(m_pMap)
	{
		BeginWaitCursor();
#ifdef _ANGELICA22
#else
		m_pMap->MergeTreeType();
#endif // #ifdef _ANGELICA22
		m_pMap->SetModifiedFlag(true);
		GetToolTabWnd()->UpdatePlantsPanel();
		EndWaitCursor();
		MessageBox("树优化处理完成!");
	}
}

void CMainFrame::OnRotateX() 
{
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE_EX);
	((CSceneObjectRotateOperationEx*)pOperation)->SetRotateFlag(CSceneObjectRotateOperationEx::ROTATE_X);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 13;
	Aux_RestoreCamera() ;
	// TODO: Add your command handler code here
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditX.m_hWnd == NULL)
    { /* create it */
     m_StatusEditX.Create(&m_wndStatusBar, ID_INDICATOR_EDIT_X, WS_VISIBLE | WS_BORDER);
    } /* create it */
}

void CMainFrame::OnRotateY() 
{
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE_EX);
	((CSceneObjectRotateOperationEx*)pOperation)->SetRotateFlag(CSceneObjectRotateOperationEx::ROTATE_Y);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 13;
	Aux_RestoreCamera() ;
	// TODO: Add your command handler code here
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();

	if(m_StatusEditY.m_hWnd == NULL)
    { /* create it */
     m_StatusEditY.Create(&m_wndStatusBar, ID_INDICATOR_EDIT_Y, WS_VISIBLE | WS_BORDER);
    } /* create it */
}

void CMainFrame::OnRotateZ() 
{
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE_EX);
	((CSceneObjectRotateOperationEx*)pOperation)->SetRotateFlag(CSceneObjectRotateOperationEx::ROTATE_Z);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 13;
	Aux_RestoreCamera() ;
	// TODO: Add your command handler code here
	
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	
	if(m_StatusEditZ.m_hWnd == NULL)
    { /* create it */
     m_StatusEditZ.Create(&m_wndStatusBar, ID_INDICATOR_EDIT_Z, WS_VISIBLE | WS_BORDER);
    } /* create it */
}

void CMainFrame::OnUpdateRotateX(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_StatusEditX.m_hWnd == NULL) pCmdUI->SetCheck(0);
	else pCmdUI->SetCheck(1);
}

void CMainFrame::OnUpdateRotateY(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_StatusEditY.m_hWnd == NULL) pCmdUI->SetCheck(0);
	else pCmdUI->SetCheck(1);
}

void CMainFrame::OnUpdateRotateZ(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_StatusEditZ.m_hWnd == NULL) pCmdUI->SetCheck(0);
	else pCmdUI->SetCheck(1);
}

void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CFrameWnd::OnSize(nType, cx, cy);
	//RECT rc;
	//GetClientRect(&rc);
	//m_wndVssDlgBar.SetWindowPos(NULL,0,0,rc.right,50,SW_SHOW);
	// TODO: Add your message handler code here
	m_StatusEditX.Reposition();
	m_StatusEditY.Reposition();
	m_StatusEditZ.Reposition();
	m_StatusCombo.Reposition();
	m_StatusProgress.Reposition();
}

BOOL CMainFrame::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_RETURN)
		{
			CString txt;
			
			if(m_pMap && (m_StatusEditX.m_hWnd || m_StatusEditY.m_hWnd || m_StatusEditZ.m_hWnd))
			{
				
				float anglex, angley,anglez;
				anglex = angley = anglez = 0.0f;
				CString str;
				if(m_StatusEditX.m_hWnd) 
				{
					m_StatusEditX.GetWindowText(str);
					anglex = (float)atof(str) * A3D_PI / 180;
				}
				
				if(m_StatusEditY.m_hWnd) 
				{
					m_StatusEditY.GetWindowText(str);
					angley = (float)atof(str)*A3D_PI / 180;
				}
				
				if(m_StatusEditZ.m_hWnd) 
				{
					m_StatusEditZ.GetWindowText(str);
					anglez = (float)atof(str)*A3D_PI / 180;
				}
				
				
				ALISTPOSITION pos = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTailPosition();
				A3DVECTOR3 vCenter(0,0,0);
				while( pos )
				{
					CSceneObject* ptemp = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetPrev(pos);
					vCenter +=ptemp->GetPos();
				}
				vCenter = vCenter/m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount();
				
				
				CUndoRotateAction2 *pUndo = new CUndoRotateAction2(m_pMap->GetSceneObjectMan());
				pUndo->SetData();
				
				pos = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTailPosition();
				while(pos)
				{
					CSceneObject *test = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetPrev(pos);
				
					if(anglex != 0.0f)
					{
						_Delta(test, anglex, vCenter, CSceneObjectRotateOperation2::ROTATE_X);
					}else if(angley != 0.0f)
					{
						_Delta(test, angley, vCenter, CSceneObjectRotateOperation2::ROTATE_Y);
					}else if(anglez != 0.0f)
					{
						_Delta(test, anglez, vCenter, CSceneObjectRotateOperation2::ROTATE_Z);
					}

				}
				
				if(anglex != 0.0f || angley != 0.0f || anglez !=0.0f)
					g_UndoMan.AddUndoAction(pUndo);
				else { pUndo->Release(); delete pUndo;}

				SetFocus();
			}
		}

		if(pMsg->wParam == VK_TAB)
		{
			if(m_nSceneObjectOperation == 9)
				ShowOrHideBezierPointPro();
		}

		if(pMsg->wParam == VK_PRIOR)
		{
			if(m_nSceneObjectOperation == 9)
				m_BezierPointBar.OnPrevPointModified();
		}

		if(pMsg->wParam == VK_NEXT)
		{
			if(m_nSceneObjectOperation == 9)
				m_BezierPointBar.OnNextPointModified();
		}

	}
	return CFrameWnd::PreTranslateMessage(pMsg);
}

void CMainFrame::SetXValue(float x)
{
	CString str;
	str.Format("%4.2f", x);
	if(m_StatusEditX.m_hWnd) m_StatusEditX.SetWindowText(str);
	else m_wndStatusBar.SetPaneText(3, str, TRUE);
}

void CMainFrame::SetYValue(float y)
{
	CString str;
	str.Format("%4.2f", y);
	if(m_StatusEditY.m_hWnd) m_StatusEditY.SetWindowText(str);
	else m_wndStatusBar.SetPaneText(5, str, TRUE);
}

void CMainFrame::SetZValue(float z)
{
	CString str;
	str.Format("%4.2f", z);
	
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.SetWindowText(str);
	else m_wndStatusBar.SetPaneText(7, str, TRUE);
}

void CMainFrame::SwitchDayNight(bool bIsNight)
{
	if(bIsNight)
	{
		if(g_fNightFactor < 0.9999f)
			m_DayAndNightDlg.OnButtonNight();
	}
	else
	{
		if(g_fNightFactor > 0.0001f)
			m_DayAndNightDlg.OnButtonDay();
	}
}

void CMainFrame::OnSelchangeCombo()
{
	if(m_pMap==NULL) return;
	int n = m_StatusCombo.GetCurSel();
	if(n == CB_ERR) return;
	CString s;
	BeginWaitCursor();
	m_StatusCombo.GetLBText(n, s);
	if(m_strLastNeighborMap == s) return;
	if(s=="不显示相邻地图")
	{
		if(m_pMap->LoadNeighborTerrain(4))
			m_strLastNeighborMap = s;
		m_pMap->LoadNeighborModels(4);
	}
	if(s=="显示相邻地图(上)")
	{
		if(m_pMap->LoadNeighborTerrain(1))
			m_strLastNeighborMap = s;
		m_pMap->LoadNeighborModels(1);
	}
	
	if(s=="显示相邻地图(下)")
	{
		if(m_pMap->LoadNeighborTerrain(7))
			m_strLastNeighborMap = s;
		m_pMap->LoadNeighborModels(7);
	}
		
	if(s=="显示相邻地图(左)")
	{
		if(m_pMap->LoadNeighborTerrain(3))
			m_strLastNeighborMap = s;
		m_pMap->LoadNeighborModels(3);
	}
	
	if(s=="显示相邻地图(右)")
	{
		if(m_pMap->LoadNeighborTerrain(5))
			m_strLastNeighborMap = s;
		m_pMap->LoadNeighborModels(5);
	}
		
	EndWaitCursor();
	
	SetFocus();
}



void CMainFrame::OnShowSelOnly() 
{
	// TODO: Add your command handler code here
	g_bShowSelectedOnly = !g_bShowSelectedOnly;
}

void CMainFrame::OnUpdateShowSelOnly(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(g_bShowSelectedOnly);
}

void CMainFrame::OnSetWorkspace() 
{
	// TODO: Add your command handler code here
	CWorkSpaceSetDlg dlg;
	if(IDOK==dlg.DoModal())
	{
	}
}

void CMainFrame::OnUpdateSetWorkspace(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	if(m_pMap) pCmdUI->Enable(false);
	else pCmdUI->Enable(true);
}

void CMainFrame::OnShowTerrainLight() 
{
	// TODO: Add your command handler code here
	g_bShowTerrainLight = !g_bShowTerrainLight;
	
	if(m_pMap) 
	{
		if(m_pMap->GetTerrain())
		{
			CTerrainRender *pRender = m_pMap->GetTerrain()->GetRender();
			if(pRender) pRender->EnableLight(g_bShowTerrainLight);
		}
		if(m_pMap->GetNeighborTerrain())
		{
			CTerrainRender *pRender = m_pMap->GetNeighborTerrain()->GetRender();
			if(pRender) pRender->EnableLight(g_bShowTerrainLight);
		}
	}
}

void CMainFrame::OnUpdateShowTerrainLight(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->SetCheck(!g_bShowTerrainLight);
}

void CMainFrame::OnShowDistance() 
{
	// TODO: Add your command handler code here
	if(m_pMap) 
	{
		CString msg;
		if(m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetCount()==2)
		{
			CSceneObject *pObj1 = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetHead();
			CSceneObject *pObj2 = m_pMap->GetSceneObjectMan()->m_listSelectedObject.GetTail();
			A3DVECTOR3 v1 = pObj1->GetPos();
			A3DVECTOR3 v2 = pObj2->GetPos();
			v1 = v2 - v1;
			float dis = v1.Magnitude();
			msg.Format("%4.2fm",dis);
			MessageBox(msg);
		}
	}
}

void CMainFrame::OnUpdateShowDistance(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
}

void CMainFrame::OnInsertMounsterToMap() 
{
/*	const float delta = 20;
	// TODO: Add your command handler code here
	if(m_pMap==NULL) return;
	CTerrain *pTerrain = m_pMap->GetTerrain();
	if(pTerrain==NULL) return;
	ARectF rc = pTerrain->GetTerrainArea();
	float lx = rc.left + delta;
	float lz = rc.bottom + delta;

	char szFilter[] = "Resorce Files (*.txt)|*.txt||";
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, "*.txt", NULL, dwFlags, szFilter, NULL);
	if(FileDia.DoModal()==IDOK)
	{
		CString strPathName = FileDia.GetPathName();
		AScriptFile File;
		if (File.Open(strPathName))
		{
			AString objName;
			int num = 0;
			while (File.GetNextToken(true))
			{
				while(1)
				{
					objName.Format("TestMounster_%d",num++);
					if(m_pMap->GetSceneObjectMan()->FindSceneObject(objName)==NULL)
						break;
				}
				
				CSceneFixedNpcGenerator *pNew = m_pMap->GetSceneObjectMan()->CreateFixedNpc(objName);
				if(pNew)
				{
					
					if(lx > rc.right - delta)
					{	
						lx = rc.left + delta;
						lz += delta;
					}else lx += delta;
					A3DVECTOR3 vPos(lx,0,lz);
					float fh = pTerrain->GetPosHeight(vPos);
					vPos.Set(vPos.x,fh,vPos.z);
					pNew->SetPos(vPos);
					pNew->SetObjectID(m_pMap->GetSceneObjectMan()->GenerateObjectID());
					
					FIXED_NPC_DATA data = pNew->GetProperty();
					data.nType = 1;
					data.dwID = atof(File.m_szToken);
					pNew->SetProperty(data);
					data.strNpcName = pNew->GetNPCName(data.dwID);
					pNew->SetProperty(data);
					pNew->UpdateProperty(false);
					pNew->ReloadModel();
				}
				
			}
			File.Close();
			m_ToolTabWnd.UpdateSceneObjectList();
		}
	}
	*/
}

void CMainFrame::OnGetCurrentProjWorkspaceMaps() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	CString str;
	str.Format("%sEditMaps",g_szWorkDir);
	g_VSS.SetWorkFolder(str);
	g_VSS.SetProjectPath("EditMaps");
	g_VSS.GetFile("EditMaps\\workspace.ws");
	
	std::vector<CString> listWorkspace;
	// Create folder
	AScriptFile File;
	if (File.Open("EditMaps\\workspace.ws"))
	{//Clear current project list
		CString path;
		while (File.GetNextToken(true))
		{
			path.Format("%sEditMaps\\%s",g_szWorkDir,File.m_szToken);
			if(!PathFileExists(path))
			{
				if(!CreateDirectory(path, NULL))
				{
					MessageBox("CMainFrame::OnGetAllMapData() ,创建取出路径失败！");
					listWorkspace.clear();
					return;
				}
			}
			path.Format("%sEditMaps\\%s",g_szWorkDir,File.m_szToken);
			g_VSS.SetWorkFolder(path);
			path.Format("EditMaps\\%s",File.m_szToken);
			g_VSS.SetProjectPath(path);
			path.Format("EditMaps\\%s\\projlist.dat",File.m_szToken);
			g_VSS.GetFile(path);
			listWorkspace.push_back(CString(File.m_szToken));
		}
		File.Close();
	}
	listWorkspace.clear();
	listWorkspace.push_back(CString(g_Configs.szCurProjectName));
	// TODO: Add your command handler code here
	for( int i = 0; i < listWorkspace.size(); i++)
	{
		str.Format("%sEditMaps\\%s",g_szWorkDir,listWorkspace[i]);
		g_VSS.SetWorkFolder(str);
		str.Format("EditMaps\\%s",listWorkspace[i]);
		g_VSS.SetProjectPath(str);
		g_VSS.GetAllFiles(str);
		g_VSS.SetProjectPath("");
		// Load project file list
		DWORD dwCol;
		char szProj[256];
		sprintf(szProj,"EditMaps\\%s\\ProjList.dat",listWorkspace[i]);
		if (!File.Open(szProj))
		{
			g_Log.Log("Open file %s failed!",szProj);
			listWorkspace.clear();
			EndWaitCursor();
			return;
		}
		
		if(File.GetNextToken(true))
		{
			CString strCol(File.m_szToken);
			if(strCol.Find("col=")!=-1)
			{
				strCol.Replace("col=","");
				dwCol = (int)atof(strCol);
			}else
			{
				File.ResetScriptFile();
				dwCol = 1;
			}
		}
		
		while (File.GetNextToken(true))
		{
			CString dstPath,srcPath,strFilePathName;
			strFilePathName.Format("%sEditMaps\\%s\\%s\\%s.elproj",g_szWorkDir,listWorkspace[i],File.m_szToken,File.m_szToken);
			if(AUX_FileIsReadOnly(strFilePathName))
			{//如果文件已经被本地取出，本地就是最新的，就不能覆盖本地的地图文件
				dstPath.Format("%sEditMaps\\%s\\%s",g_szWorkDir,listWorkspace[i],File.m_szToken);
				srcPath.Format("%s\\MapsData\\%s\\%s",g_Configs.szServerPath,listWorkspace[i],File.m_szToken);
				AUX_CopyFolder(dstPath,srcPath);
			}
		}
		File.Close();
	}
	MessageBox("地图更新完成!");
	EndWaitCursor();
}

void CMainFrame::OnMounsterStat() 
{
	// TODO: Add your command handler code here
	CMousterStatFilterDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnModelChanged() 
{
	// TODO: Add your command handler code here
	CModelChangedStatusDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnExpLightData() 
{
	// TODO: Add your command handler code here
}

void CMainFrame::OnExportSkyInfor() 
{
	// TODO: Add your command handler code here
	if(IDYES != AfxMessageBox("你确定要输出当前工作空间中所用到的天空纹理路径吗？",MB_YESNO|MB_ICONQUESTION)) return;
	abase::vector<CString> listSky;

	
	ALog  m_Log;
	char szLogPath[MAX_PATH];
	sprintf(szLogPath,"%s_sky.log",g_Configs.szCurProjectName);
	m_Log.Init(szLogPath, "下面是项目中用到的天空纹理路径...");

	//逐一检测地图
	CString mapName;
	CElementMap emap;
	
	// Load project file list
	AScriptFile File;
	DWORD dwCol;
	char szProj[256];
	sprintf(szProj,"%s\\ProjList.dat",g_szEditMapDir);
	if (!File.Open(szProj))
	{
		g_Log.Log("Open file %s failed!",szProj);
		return;
	}
	
	if(File.GetNextToken(true))
	{
		CString strCol(File.m_szToken);
		if(strCol.Find("col=")!=-1)
		{
			strCol.Replace("col=","");
			dwCol = (int)atof(strCol);
		}else
		{
			File.ResetScriptFile();
			dwCol = 1;
		}
		
	}
	abase::vector<CString> projectList;
	while (File.GetNextToken(true))
	{
		CString strNew(File.m_szToken);
		projectList.push_back(strNew);
	}
	File.Close();
	
	// Proessing map
	BeginWaitCursor();
	abase::vector<CString>::iterator theIterator;
	for(theIterator = projectList.begin(); theIterator != projectList.end(); theIterator ++)
	{
		
		mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,(*theIterator),(*theIterator));
		if(!emap.Load(mapName))
		{
			CString error_msg;
			error_msg.Format("不能打开地图文件 :%s",mapName);
			MessageBox(error_msg);
			EndWaitCursor();
			return;
		}
		CSceneObjectManager *pMan =emap.GetSceneObjectMan();
		ALISTPOSITION pos = pMan->m_listSceneObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSceneObject.GetPrev(pos);
			int type = ptemp->GetObjectType();
			if(type==CSceneObject::SO_TYPE_AREA)
			{
				CSceneBoxArea *pArea = (CSceneBoxArea *)ptemp;
				BOX_AREA_PROPERTY pro;
				pArea->GetProperty(pro);
				
				CString pathArray[6];
				pathArray[0] = pro.m_strSky0;
				pathArray[1] = pro.m_strSky1;
				pathArray[2] = pro.m_strSky2;
				pathArray[3] = pro.m_strSkyNight0;
				pathArray[4] = pro.m_strSkyNight1;
				pathArray[5] = pro.m_strSkyNight2;
				
				for( int j = 0; j < 6; ++j)
				{
					CString temp = pathArray[j];
					int npos = temp.ReverseFind('\\');
					temp = temp.Left(npos);
					m_Log.Log("%s------%s",emap.GetMapName(),temp);
					bool bInsert = true;
					for( int i = 0; i < listSky.size(); ++i)
					{
						if(listSky[i] == temp)
						{
							bInsert = false;
							break;
						}
					}
					if(bInsert) listSky.push_back(temp);
				}
			}
		}
		emap.Release();
	}
	
	m_Log.Log("下面是用到过的天空纹理列表:");
	for( int k = 0; k < listSky.size(); ++k)
		m_Log.Log(listSky[k]);
	
	EndWaitCursor();
	m_Log.Release();
	MessageBox("处理完成!");
}

void CMainFrame::OnExpLightInforList() 
{
	// TODO: Add your command handler code here
	CExportLitDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnStatAllTriangle() 
{
	// TODO: Add your command handler code here
	if(m_pMap) 
	{
		UINT nTriangleNum = 0;
		UINT nMeshNum = 0;
		CSceneObjectManager *pMan = m_pMap->GetSceneObjectMan();
		ALISTPOSITION pos = pMan->m_listSceneObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSceneObject.GetPrev(pos);
			nTriangleNum += ptemp->GetTriangleNum();
			nMeshNum += ptemp->GetMeshNum();
		}

		CTerrain *pTerrain = m_pMap->GetTerrain();
		int nGrid =  (int)(pTerrain->GetTerrainSize()/pTerrain->GetTileSize());
		UINT nTerrain = nGrid*nGrid*4;
		CString strMsg;
		strMsg.Format("地型三角形总数: %d\n场景对象三角形总数: %d\n地图三角形总数:%d\nmesh数:%d",nTerrain,nTriangleNum,nTerrain + nTriangleNum, nMeshNum);
		MessageBox(strMsg);
	}
}

void CMainFrame::OnStatusTriangleSel() 
{
	// TODO: Add your command handler code here
	if(m_pMap) 
	{
		UINT nTriangleNum = 0;
		UINT nMeshNum = 0;
		CSceneObjectManager *pMan = m_pMap->GetSceneObjectMan();
		ALISTPOSITION pos = pMan->m_listSelectedObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSelectedObject.GetPrev(pos);
			nTriangleNum += ptemp->GetTriangleNum();
			nMeshNum += ptemp->GetMeshNum();
		}
		CString strMsg;
		strMsg.Format("被选中对象的三角形总数: %d\nMesh数: %d",nTriangleNum, nMeshNum);
		MessageBox(strMsg);
	}
}

void CMainFrame::OnStatusBackToOrigin()
{
	// 返回坐标原点 (0, 10, 0)
	A3DVECTOR3 defaultPos = A3DVECTOR3(0.0f, 10.0f, 0.0f);
	if(m_pMap)
		defaultPos.y = m_pMap->GetTerrain()->GetPosHeight(defaultPos) + 10.0f;

	g_Render.GetA3DEngine()->GetActiveCamera()->SetPos(defaultPos);
}

void CMainFrame::OnOperationRotate2() 
{
	// TODO: Add your command handler code here
	if(m_StatusEditX.m_hWnd) m_StatusEditX.DestroyWindow();
	if(m_StatusEditY.m_hWnd) m_StatusEditY.DestroyWindow();
	if(m_StatusEditZ.m_hWnd) m_StatusEditZ.DestroyWindow();
	Aux_RestoreCamera() ;
	m_ToolTabWnd.RiseUpAllSceneObjectButton();
	m_ToolTabWnd.RiseUpAllTerrainOperationButton();
	COperation *pOperation = g_OperationContainer.GetOperation(OPERATION_SCENE_OBJECT_ROTATE2);
	m_wndViewframe.SetCurrentOperation(pOperation);
	m_nSceneObjectOperation = 12;
}

void CMainFrame::OnUpdateOperationRotate2(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
	if(m_nSceneObjectOperation==12) 
	{
		if(!m_bOtherOperation)
			pCmdUI->SetCheck(true);
		else pCmdUI->SetCheck(false);
	}
	else pCmdUI->SetCheck(false);
}

void CMainFrame::OnDynamicObjectEditor() 
{
	// TODO: Add your command handler code here
	CDynamicObjectManDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnMonsterControllerEditor() 
{
	// TODO: Add your command handler code here
	CNpcControllerManDlg dlg;
	dlg.DoModal();
}

void CMainFrame::OnReloadAllTextures() 
{
	// TODO: Add your command handler code here
	BeginWaitCursor();
	g_Render.GetA3DEngine()->GetA3DTextureMan()->ReloadAllTextures(false);
	EndWaitCursor();
}

void CMainFrame::OnExportStatNpc() 
{
	// TODO: Add your command handler code here
	CElementMap *pMap = GetMap();
	if(pMap==NULL) return; 
	
	char szFilter[] = "Text Files (*.txt)|*.txt||";
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(FALSE, "*.txt", NULL, dwFlags, szFilter, NULL);
	CString text;
	if(FileDia.DoModal()==IDOK)
	{
		CString strPathName = FileDia.GetPathName();
		AFile file;
		if(!file.Open(strPathName,AFILE_CREATENEW | AFILE_TEXT | AFILE_NOHEAD)) return;
		
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		BeginWaitCursor();
		
		APtrList<CSceneObject *>* al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_AIAREA);
		ALISTPOSITION pos = al->GetTailPosition();
		while( pos )
		{
			CSceneAIGenerator* ptemp = (CSceneAIGenerator*)al->GetPrev(pos);
			if( !ptemp->IsDeleted())
			{
				int n = ptemp->GetNPCNum();
				for(int i = 0; i < n; i++)
				{
					NPC_PROPERTY* np = ptemp->GetNPCProperty(i);
					text.Format("Npc id = %d; Pos = (%4.2f,%4.2f,%4.2f)",np->dwID,ptemp->GetPos().x,ptemp->GetPos().y,ptemp->GetPos().z);
					file.WriteLine(text);
				}
			}
		}
		
		
		//for fixed mounster
		al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
		pos = al->GetTailPosition();
		while( pos )
		{
			CSceneFixedNpcGenerator* ptemp = (CSceneFixedNpcGenerator*)al->GetPrev(pos);
			if( !ptemp->IsDeleted())
			{
				FIXED_NPC_DATA dat = ptemp->GetProperty();
				text.Format("Npc id = %d; Pos = (%4.2f,%4.2f,%4.2f)",dat.dwID,ptemp->GetPos().x,ptemp->GetPos().y,ptemp->GetPos().z);
				file.WriteLine(text);
			}
		}
		file.Close();
		EndWaitCursor();
	}
}

void CMainFrame::OnReloadNpcs() 
{
	// TODO: Add your command handler code here
	CElementMap *pMap = GetMap();
	if(pMap==NULL) return; 
	
	CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
	BeginWaitCursor();

	APtrList<CSceneObject *>* al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_AIAREA);
	ALISTPOSITION pos = al->GetTailPosition();
	while( pos )
	{
		CSceneAIGenerator* ptemp = (CSceneAIGenerator*)al->GetPrev(pos);
		if( !ptemp->IsDeleted()) ptemp->ReloadModels();
	}
	
	al =  pSManager->GetSortObjectList(CSceneObject::SO_TYPE_FIXED_NPC);
	pos = al->GetTailPosition();
	while( pos )
	{
		CSceneFixedNpcGenerator* ptemp = (CSceneFixedNpcGenerator*)al->GetPrev(pos);
		if( !ptemp->IsDeleted()) ptemp->ReloadModel();
	}

	EndWaitCursor();
	
}

void CMainFrame::ShowBezierPointPro(CEditerBezier* pObj, CEditerBezierPoint *pPoint)
{
	m_BezierPointBar.Init(pObj, pPoint);
	ShowControlBar(&m_BezierPointBar,  TRUE,   TRUE);   
	m_bBezierBarShow = true;
}

void CMainFrame::HideBezierPointPro()
{
	ShowControlBar(&m_BezierPointBar,  FALSE,   TRUE);   
	m_bBezierBarShow = false;
}

void CMainFrame::ShowOrHideBezierPointPro()
{
	if(m_bBezierBarShow)
		HideBezierPointPro();
	else
	{
		ShowControlBar(&m_BezierPointBar,  TRUE,   TRUE);   
		m_bBezierBarShow = true;
	}
}


void CMainFrame::OnUpdateOperationDelete(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
}

void CMainFrame::OnUpdateEditObjectCopy(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
}

void CMainFrame::OnUpdateViewResetcamera(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(!g_bLockCam);
}

void CMainFrame::OnViewFinder() 
{
/*
	// TODO: Add your command handler code here
	CElementMap *pMap = GetMap();
	if(pMap==NULL) return; 
	
	CDlgModelFinder dlg;
	if(IDOK==dlg.DoModal())
	{
		BeginWaitCursor();
		CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
		CSceneObject* pObj = pSManager->Find(g_szFinderKey);
		if(pObj==NULL)
		{
			CString msg;
			msg.Format("没找到带\" %s \"的模型!",g_szFinderKey);
			MessageBox(msg);
		}else
		{
			A3DVECTOR3 vDir = (pObj->GetMax() - pObj->GetMin());
			vDir.Normalize();
			g_Render.GetA3DEngine()->GetActiveCamera()->SetPos(pObj->GetMax() + vDir * 50);
			g_Render.GetA3DEngine()->GetActiveCamera()->SetDirAndUp(-vDir,A3DVECTOR3(0,1.0f,0));
		}

		EndWaitCursor();
	}*/
}

void CMainFrame::OnViewFinderNext() 
{
	// TODO: Add your command handler code here
	/*
	CElementMap *pMap = GetMap();
	if(pMap==NULL) return; 
	
	BeginWaitCursor();
	CSceneObjectManager *pSManager = pMap->GetSceneObjectMan();
	if(strlen(g_szFinderKey) > 0 )
	{
		CSceneObject* pObj = pSManager->FindNext();
		if(pObj==NULL)
		{
			CString msg;
			msg.Format("没找到带\" %s \"的模型!",g_szFinderKey);
			MessageBox(msg);
		}else
		{
			A3DVECTOR3 vDir = (pObj->GetMax() - pObj->GetMin());
			vDir.Normalize();
			g_Render.GetA3DEngine()->GetActiveCamera()->SetPos(pObj->GetMax() + vDir * 50);
			g_Render.GetA3DEngine()->GetActiveCamera()->SetDirAndUp(-vDir,A3DVECTOR3(0,1.0f,0));
		}
	}
	
	EndWaitCursor();*/
}

void CMainFrame::OnTerrainLightingFast() 
{
	// TODO: Add your command handler code here
	/*
	CElementMap *pMap = GetMap();
	if(pMap==NULL) return; 
	
	if(IDYES==AfxMessageBox("你确定要进行快速光照计算么?",MB_YESNO))
	{
		BeginWaitCursor();
		pMap->GenerateLightMapFast();
		EndWaitCursor();
	}*/
}
void CMainFrame::OnStatMusic() 
{
	// TODO: Add your command handler code here
	if(m_pMap) 
	{
		
		char szFilter[] = "Text Files (*.txt)|*.txt||";
		DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_NOCHANGEDIR;
		CFileDialog	FileDia(FALSE, "*.txt", NULL, dwFlags, szFilter, NULL);
		if(FileDia.DoModal()==IDOK)
		{
			CString strPathName = FileDia.GetPathName();
			AFile file;
			if(!file.Open(strPathName,AFILE_CREATENEW | AFILE_TEXT | AFILE_NOHEAD)) return;
			
			APtrList<CSceneObject *>* pList = m_pMap->GetSceneObjectMan()->GetSortObjectList(CSceneObject::SO_TYPE_PRECINCT);
			
			ALISTPOSITION pos = pList->GetTailPosition();
			while( pos )
			{
				CScenePrecinctObject* ptemp = (CScenePrecinctObject*)pList->GetPrev(pos);
				int n = ptemp->GetMusicNum();
				file.WriteLine(ptemp->GetObjectName());
				for( int i = 0; i < n; ++i)
					file.WriteLine(ptemp->GetMusic(i));
			}

			file.Close();
		}
		
	}
}

void CMainFrame::OnImportHistory() 
{
	// just for test
	CImportFromHistroy dlg;
	dlg.DoModal();
}

void CMainFrame::OnExportInfo() 
{
	CExportSceneInfo dlg;
	dlg.DoModal();	
}

void CMainFrame::OnReloadShaders()
{
#ifdef _ANGELICA22
    g_Render.GetA3DEngine()->GetA3DHLSLMan()->RebuildAll();
#endif
}
void CMainFrame::OnEnablelitmap()
{
	// TODO: Add your command handler code here
	m_pMap->EnableLitmap(!m_pMap->IsEnableLitmap());
}

void CMainFrame::OnUpdateEnablelitmap(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMap != NULL);
	if(m_pMap != NULL)
		pCmdUI->SetCheck(m_pMap->IsEnableLitmap() ? 1 : 0);
}

void CMainFrame::OnEnableRealtimeLight()
{
	// TODO: Add your command handler code here
	if(m_pMap != NULL)
		m_pMap->EnableRealTimeLight(!m_pMap->IsRealTimeLight());
}

void CMainFrame::OnUpdateEnableRealtimeLight(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMap != NULL);
	if(m_pMap != NULL)
		pCmdUI->SetCheck(m_pMap->IsRealTimeLight() ? 1 : 0);
}

void CMainFrame::OnEnableDetail()
{
	// TODO: Add your command handler code here
	m_pMap->EnableDetail(!m_pMap->IsEnableDetail());
}

void CMainFrame::OnUpdateEnableDetail(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	pCmdUI->Enable(m_pMap != NULL);
	if(m_pMap != NULL)
		pCmdUI->SetCheck(m_pMap->IsEnableDetail() ? 1 : 0);
}

void CMainFrame::OnCalcLightmapRate()
{
	if (m_pMap)
	{
		CSceneObjectManager* pSceneObjectManager = m_pMap->GetSceneObjectMan();

		const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
		ALISTPOSITION pos = listModel->GetTailPosition();

		int nModelCount = 0;
		float fTotalRate = 0;
		// Output models
		while( pos )
		{
			CSceneObject* ptemp = listModel->GetPrev(pos);
			if( !ptemp->IsDeleted() )
			{
				// Lit this model
				CStaticModelObject * pStaticModel = (CStaticModelObject *) ptemp;
				if (pStaticModel == NULL || pStaticModel->GetA3DModel() == NULL || pStaticModel->GetA3DModel()->pModel == NULL)
					continue;
				if (pStaticModel->GetCollideFlags())
					continue;

				float fRate = pStaticModel->GetLightmapRate();

				// Vertex Light
				if (fRate == 0)
					continue;

				fTotalRate += fRate;
				nModelCount++;
			}
		}

		AString strMsg;
		strMsg.Format("光照图模型数：%d，光照图面积利用率：%.02f%%", 
			nModelCount, fTotalRate / nModelCount * 100);

		MessageBox(strMsg);
	}
}

void CMainFrame::OnUpdateCalcLightmapRate(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pMap != NULL);
}

void CMainFrame::OnDelSelObjLitmap()
{
	CSceneObjectManager* pSceneObjectManager = m_pMap->GetSceneObjectMan();
	const APtrList<CSceneObject *>* pSelList = &pSceneObjectManager->m_listSelectedObject;
	const CString strLitmapPath = CString(g_szWorkDir) + g_szEditMapDir + "\\" + m_pMap->GetMapName() + "\\LitData";

	ALISTPOSITION pos = pSelList->GetTailPosition();

	CString strLitName;
	CString strLitmapPathDF;
	CString strLitmapPathDX;
	CString strLitmapPathNF;
	CString strLitmapPathNX;
	char	buffer[MAX_PATH + 1];

	while( pos )
	{
		CSceneObject* ptemp = pSelList->GetPrev(pos);
		if( !ptemp->IsDeleted() && ptemp->GetObjectType() == CSceneObject::SO_TYPE_STATIC_MODEL)
		{
			strLitName.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRender*.*", strLitmapPath, 
				ptemp->GetObjectID(), m_pMap->GetMapName());

			memset(buffer, 0, sizeof(buffer));

			SHFILEOPSTRUCT	shfos;
			strcpy(buffer, strLitName);

			shfos.hwnd					= NULL;
			shfos.wFunc					= FO_DELETE;
			shfos.pFrom					= buffer;
			shfos.pTo					= NULL;
			shfos.fFlags				= FOF_NOCONFIRMATION;
			shfos.fAnyOperationsAborted = NULL;
			shfos.hNameMappings			= NULL;
			shfos.lpszProgressTitle		= "删除光照图缓存目录";
			SHFileOperation(&shfos);
		}
	}
	CDlgPreRadStep2::ReloadLitmap(m_pMap, false);
}

void CMainFrame::OnUpdateDelSelObjLitmap(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pMap != NULL);
}

void CMainFrame::OnReloadLitmap()
{
	if(m_pMap->IsEnableLitmap() == false)
	{
		m_pMap->EnableLitmap(true);
	}
	CDlgPreRadStep2::ReloadLitmap(m_pMap, true);
	CDlgPreRadStep2::ReloadTerrainLitmap(m_pMap);
}

void CMainFrame::OnUpdateReloadLitmap(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pMap != NULL);
}

void CMainFrame::OnReloadlitmapNovert()
{
	if(m_pMap->IsEnableLitmap() == false)
	{
		m_pMap->EnableLitmap(true);
	}
	CDlgPreRadStep2::ReloadLitmap(m_pMap, false);
	CDlgPreRadStep2::ReloadTerrainLitmap(m_pMap);
}

void CMainFrame::OnUpdateReloadlitmapNovert(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pMap != NULL);
}

void CMainFrame::OnLightMapWizard()
{
	CDlgPreRadStep1 Step1(m_pMap, NULL);
	if(Step1.DoModal() != IDOK)
		return;
	// 必须放在 Step1 返回后初始化
	CDlgPreRadStep2 Step2(m_pMap, Step1.GetParam(), NULL);
	Step2.DoModal();
}

void CMainFrame::OnUpdateLightMapWizard(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(m_pMap != NULL);
}

void CMainFrame::OnRaytraceLightmapWizard()
{
	CRayTraceDlg dlgRayTrace(m_pMap);
	dlgRayTrace.DoModal();
}

void CMainFrame::OnUpdateRaytraceLightmapWizard(CCmdUI* pCmdUI)
{
	//注掉打开离线渲染全部光照图
	//pCmdUI->Enable(m_pMap != NULL);
}
