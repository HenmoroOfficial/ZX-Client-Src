// RayTraceDlg.cpp : implementation file
//

#include "stdafx.h"
#include "elementeditor.h"
#include "RayTraceDlg.h"
#include "Global.h"
#include "resource.h"
#include "DlgPreRadStep1.h"
#include "DlgPreRadStep2.h"
#include "ElementMap.h"
#include "SceneObject.h"
#include "SceneObjectManager.h"
#include "TerrainRender.h"

#include "StaticModelObject.h"
#include "SceneLightObject.h"
#include "EL_Building.h"
#include "Bitmap.h"
#include <A3DLitModel.h>
#include <A3DFuncs.h>
#include "MirrorPlaneObject.h"
#include "Shlwapi.h"
#include "LightMapTransition.h"
#include "Render.h"
#include <A3DEngine.h>
#include <vector>
#include <AF.h>
#include "ExportOrderGen.h"
#include "EL_Archive.h"

#define RAYTRACE_TOOL_VERSION "2013-01-06.001"

static RAYTRACE_CONFIG defaultConfig =
{
	TRUE,	//BOOL bEnableDirect;
	FALSE,	//BOOL bMainLightOnly;
	1.0f,	//float fShadowHardness;
	4,		//int nSampleForTerrain;
	1,		//int nSampleForModel;
	0.0f,	//float fContantColor;
    0.3,	//float fOrenNayarRho;
	TRUE,	//BOOL bEnableAO;
	300,	//int nAOSample;
    2.0f,	//float fAOEnhance;
	TRUE,	//bool bAOAllDir;
	10.0f,	//float fAODist;
    0.46f,	//float fAOStart;
	FALSE,	//BOOL bEnableIndirect;
	3000,	//int nEmitPhoton;
	3,		//int nMaxBounce;
	48,		//int nGatherSample;
	0.3f,	//float fMaxCacheErr;
	TRUE,	//bool bSkipBright;
	1.6f,	//float fDirectScale;
	1.6f,	//float fIndirectScale;
	1.6f,	//float fAOScale;
	0.6f,	//float fGamma;
	3,		//int iDayNight;
	256,	//int iDefautSize;
	FALSE,	//BOOL bSelectOnly;
	TRUE,	//BOOL bGenTerrain;
	TRUE,	//BOOL bReloadMap;
    FALSE,	//BOOL bOnlyTerrain;
	-1,		//int iStartObjIdx;
	-1,		//int iEndObjIdx;
	FALSE,	//BOOL bHDREncode;
    1e9F,	//float fMaxVolume;
    5000.0f,//float fMaxSize;
    TRUE,	//BOOL bBlockRender;
    128.0f,	//float fBlockSize;
    FALSE,	//BOOL bVertLitOnly;
/*
	TRUE, FALSE, 1.0f, 4, 1, 0.0f, 0.75f,
	TRUE, 100, 2, TRUE, 10.0f, 0.0f,
	FALSE, 3000, 3, 48, 0.3f, TRUE,
	1.0f, 1.0f, 1.0f, 1.0f,
	1, 256, FALSE, TRUE, TRUE, FALSE, -1, -1, FALSE, 1e9f, 500.0f, TRUE, 128.0f, FALSE,
*/	
};

static char szToolPath[MAX_PATH] = "Raytrace\\RayTraceLightMapGen.exe";

#define RAYTRACE_CONFIG_VERSION 1

struct RTExportParamsDesc 
{
	CElementMap* pMap;
	CString strMapName;
	CString strTerrainHeightPath;
	CString strTerrainDiffusePath;
	CString strLitmapCacheDir;
	CString strLitmapXmlFile;
	CString strLitmapPath;
	CString cPrefix[2];
	int nTerrainBlockSize;
	LIGHTINGPARAMS LtParamsDay;
	LIGHTINGPARAMS LtParamsNight;

	RTExportParamsDesc(CElementMap* pMap, int index)
	{
		this->pMap = pMap;
		strMapName = pMap->GetMapName();
		strLitmapCacheDir.Format("%sRayTraceLitmapCache%d", g_szWorkDir, index);
		strTerrainHeightPath = strLitmapCacheDir + "\\Terrain.raw";
		int k = (int)atof(strMapName);
		strTerrainDiffusePath =	strLitmapCacheDir + ((k>=0 && k < 10) ? "\\0" : "\\") + strMapName + ".bmp";
		strLitmapPath = CString(g_szWorkDir) + g_szEditMapDir + "\\" + strMapName + "\\LitData";
		cPrefix[0] = "D";
		cPrefix[1] = "N";
		nTerrainBlockSize = 64;
		LtParamsDay = pMap->GetLightingParams();
		LtParamsNight = pMap->GetNightLightingParams();
	}
};

static BOOL DeleteDirectory(LPCTSTR pszFileName)
{
	if (PathFileExists(pszFileName) == FALSE)
		return TRUE;

	char buffer[MAX_PATH + 1];
	memset(buffer, 0, sizeof(buffer));
	
	SHFILEOPSTRUCT	shfos;
	strcpy(buffer, pszFileName);
	int len = (int)strlen(buffer);
	if (buffer[len - 1] == '\\')
		buffer[len - 1] = 0;

	shfos.hwnd					= NULL;
	shfos.wFunc					= FO_DELETE;
	shfos.pFrom					= buffer;
	shfos.pTo					= NULL;
	shfos.fFlags				= FOF_NOCONFIRMATION;
	shfos.fAnyOperationsAborted = NULL;
	shfos.hNameMappings			= NULL;
	shfos.lpszProgressTitle		= "Deleting";
	return SUCCEEDED(SHFileOperation(&shfos));
}

static bool LoadProjectList(std::vector<CString>& lMaps, DWORD& dwCols)
{
	AScriptFile File;
	char szProj[256];
	sprintf(szProj, "%s\\ProjList.dat", g_szEditMapDir);
	if (!File.Open(szProj))
	{
		g_Log.Log("Open file %s failed!",szProj);
		return false;
	}
	
	if(File.GetNextToken(true))
	{
		CString strCol(File.m_szToken);
		if(strCol.Find("col=")!=-1)
		{
			strCol.Replace("col=","");
			dwCols = (int)atof(strCol);
		}else
		{
			File.ResetScriptFile();
			dwCols = 1;
		}
	}
	lMaps.clear();
	while (File.GetNextToken(true))
	{
		CString pathName;
		pathName.Format("%s%s\\%s\\%s.elproj", g_szWorkDir, g_szEditMapDir, File.m_szToken, File.m_szToken);
		lMaps.push_back(pathName);
	}
	File.Close();
	return true;
}


/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg dialog


CRayTraceDlg::CRayTraceDlg(CElementMap* pMap)
	: CDialog(CRayTraceDlg::IDD), m_pCurMap(pMap)
{
	if (pMap && pMap->m_pRayTraceConfig)
    {
        m_pConfig = pMap->m_pRayTraceConfig;
    }
	else	// 离线渲染从第一张地图中读取渲染参数
	{
		CElementMap pMapConfig;

		AFile File;
		CString strFile;
		strFile.Format("%s%s\\1\\1.elproj", g_szWorkDir, g_szEditMapDir);
		if (File.Open(strFile, AFILE_OPENEXIST))
		{
			CELFileArchive ar(&File, false);
			pMapConfig.LoadGeneralMapData(ar);
			if (pMapConfig.m_pRayTraceConfig)
			{
				defaultConfig = *pMapConfig.m_pRayTraceConfig;
				m_pConfig = &defaultConfig;
			}
		}
	}

	if(!m_pConfig)
	{
		m_pConfig = &defaultConfig;
		MessageBox("读取工作空间光照图信息失败，将采用默认设置", "提醒", MB_OK | MB_ICONINFORMATION);
	}

	//{{AFX_DATA_INIT(CRayTraceDlg)
	//}}AFX_DATA_INIT
}


void CRayTraceDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CRayTraceDlg)
    DDX_Control(pDX, IDC_CHECK_NIGHT, m_chkNight);
    DDX_Control(pDX, IDC_CHECK_DAY, m_chkDay);
    DDX_Control(pDX, IDC_COMBO_AO_SAMPLE, m_cmbAOSample);
    DDX_Control(pDX, IDC_CHK_AO, m_chkEnableAO);
    DDX_Control(pDX, IDC_COMBO_AO_ENHANCE, m_cmbAOEnhance);
    DDX_Control(pDX, IDC_COMBO_AO_DIST, m_cmbAODist);
    DDX_Control(pDX, IDC_AO_SCALE, m_sldAOScale);
    DDX_Control(pDX, IDC_COMBO_TERRAIN_SAMPLE, m_cmbTerrainSample);
    DDX_Control(pDX, IDC_HARDNESS, m_sldHardness);
    DDX_Control(pDX, IDC_COMBO_MODEL_SAMPLE, m_cmbModelSample);
    DDX_Control(pDX, IDC_INDIRECT_SCALE, m_sldIndirectScale);
    DDX_Control(pDX, IDC_GAMMA, m_sldGamma);
    DDX_Control(pDX, IDC_EDIT_EMIT_PHOTON, m_edtEmitPhoton);
    DDX_Control(pDX, IDC_EDIT_TOOL, m_edtTool);
    DDX_Control(pDX, IDC_DIRECT_SCALE, m_sldDirectScale);
    DDX_Control(pDX, IDC_COMBO_GATHER_SAMPLE, m_cmbGatherSample);
    DDX_Control(pDX, IDC_COMBO_BOUNCE, m_cmbBounce);
    DDX_Control(pDX, IDC_CHK_INDIRECTLIGHT, m_chkEnableIndirect);
    DDX_Control(pDX, IDC_CHK_SELONLY, m_chkSelOnly);
    DDX_Control(pDX, IDC_CHECK_GEN_TERRAIN, m_chkGenTerrain);
    DDX_Control(pDX, IDC_CHECK_RELOAD_MAP, m_chkReloadMap);
    DDX_Control(pDX, IDC_CHECK_ONLYTERRAIN, m_chkOnlyTerrain);
    DDX_Control(pDX, IDC_CHK_DIRECTLIGHT, m_chkEnableDirect);
    DDX_Control(pDX, IDC_CONSTANT_LIGHT, m_sldConstantColor);
    DDX_Control(pDX, IDC_EDIT_START_OBJ, m_edtStartObj);
    DDX_Control(pDX, IDC_EDIT_END_OBJ, m_edtEndObj);
    DDX_Control(pDX, IDC_CHECK_AO_ALLDIR, m_chkAOAllDir);
    DDX_Control(pDX, IDC_COMBO_MAXERR, m_cmbMaxErr);
    DDX_Control(pDX, IDC_BTN_EXPORT, m_btnExport);
    //DDX_Control(pDX, IDC_CHECK_HDR, m_chkHDR);
    DDX_Control(pDX, IDC_CHECK_MAIN_ONLY, m_chkMainLightOnly);
    //DDX_Control(pDX, IDC_COMBO_DEFAUT_SIZE, m_cmbDefautSize);
    DDX_Control(pDX, IDOK, m_btnOK);
    DDX_Control(pDX, IDC_CHECK_SKIPBRIGHT, m_chkSkipBright);
    //}}AFX_DATA_MAP
    DDX_Control(pDX, IDC_EDIT_MAX_VOLUME, m_edtMaxVolume);
    DDX_Control(pDX, IDC_BUTTON_CALC_SIZE, m_btnCalcSize);
    DDX_Control(pDX, IDC_EDIT_SIZE_LIMIT, m_edtSizeLimit);
    DDX_Control(pDX, IDC_EDIT_SIZE_LIMIT_PASSWORD, m_edtSizeLitmitPassword);
    DDX_Control(pDX, IDC_AO_START, m_sldAOStart);
    DDX_Control(pDX, IDC_CHECK_BLOCK, m_chkBlock);
    DDX_Control(pDX, IDC_EDIT_BLOCK_LEN, m_edtBlockSize);
    DDX_Control(pDX, IDC_CHECK_VERTLIT_ONLY, m_chkVertLitOnly);
    DDX_Control(pDX, IDC_ORENNAYAR_RHO, m_sldOrenNayarRho);
}


BEGIN_MESSAGE_MAP(CRayTraceDlg, CDialog)
	//{{AFX_MSG_MAP(CRayTraceDlg)
	ON_BN_CLICKED(IDC_CHK_DIRECTLIGHT, OnChkDirectlight)
	ON_BN_CLICKED(IDC_CHK_INDIRECTLIGHT, OnChkIndirectlight)
	ON_BN_CLICKED(IDC_CHK_AO, OnChkAo)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnBrowse)
	ON_BN_CLICKED(IDC_BTN_EXPORT, OnExport)
	//}}AFX_MSG_MAP
    ON_BN_CLICKED(IDC_BUTTON_CALC_SIZE, &CRayTraceDlg::OnBnClickedButtonCalcSize)
    ON_BN_CLICKED(IDC_BUTTON2, &CRayTraceDlg::OnBnClickedButton2)
    ON_BN_CLICKED(IDC_CHECK_BLOCK, &CRayTraceDlg::OnBnClickedCheckBlock)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRayTraceDlg message handlers


BOOL CRayTraceDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	m_sldHardness.SetRange(0, 100);
	m_sldConstantColor.SetRange(0, 50);
	m_cmbTerrainSample.AddString("1");
	m_cmbTerrainSample.AddString("4");
	m_cmbTerrainSample.AddString("10");
	m_cmbTerrainSample.AddString("30");
	m_cmbTerrainSample.AddString("60");
	m_cmbModelSample.AddString("1");
	m_cmbModelSample.AddString("4");
	m_cmbModelSample.AddString("10");
	m_cmbModelSample.AddString("30");
	m_cmbModelSample.AddString("50");
	m_cmbAOSample.AddString("50");
	m_cmbAOSample.AddString("100");
	m_cmbAOSample.AddString("150");
	m_cmbAOSample.AddString("200");
	m_cmbAOSample.AddString("300");
	m_cmbAOEnhance.AddString("1");
	m_cmbAOEnhance.AddString("1.5");
	m_cmbAOEnhance.AddString("2");
	m_cmbAOEnhance.AddString("3");
	m_cmbAOEnhance.AddString("4");
	m_cmbAODist.AddString("1");
	m_cmbAODist.AddString("5");
	m_cmbAODist.AddString("10");
	m_cmbAODist.AddString("20");
	m_cmbAODist.AddString("50");
	m_cmbBounce.AddString("1");
	m_cmbBounce.AddString("2");
	m_cmbBounce.AddString("3");
	m_cmbBounce.AddString("4");
	m_cmbBounce.AddString("6");
	m_cmbBounce.AddString("8");
	m_cmbGatherSample.AddString("12");
	m_cmbGatherSample.AddString("27");
	m_cmbGatherSample.AddString("48");
	m_cmbGatherSample.AddString("75");
	m_cmbGatherSample.AddString("108");
	m_cmbGatherSample.AddString("192");
	m_cmbGatherSample.AddString("300");
	m_cmbMaxErr.AddString("0.03");
	m_cmbMaxErr.AddString("0.05");
	m_cmbMaxErr.AddString("0.1");
	m_cmbMaxErr.AddString("0.3");
	m_cmbMaxErr.AddString("0.5");
	m_sldDirectScale.SetRange(0, 20);
	m_sldIndirectScale.SetRange(0, 20);
	m_sldAOScale.SetRange(0, 20);
	m_sldGamma.SetRange(5, 10);
    m_sldAOStart.SetRange(0, 110);
    m_sldAOStart.SetTicFreq(1);
    m_sldOrenNayarRho.SetRange(0, 10);
	//m_cmbDefautSize.AddString("64");
	//m_cmbDefautSize.AddString("128");
	//m_cmbDefautSize.AddString("256");
	//m_cmbDefautSize.AddString("512");
	m_btnOK.EnableWindow(m_pCurMap != NULL);
    m_btnCalcSize.EnableWindow(m_pCurMap != NULL);
    m_chkEnableIndirect.EnableWindow(FALSE);
    m_chkEnableAO.EnableWindow(FALSE);
    m_edtSizeLimit.EnableWindow(FALSE);

    m_edtBlockSize.AddString("128");
    m_edtBlockSize.AddString("256");
    m_edtBlockSize.AddString("512");

    OnChkAo();
    OnChkIndirectlight();
	ConfigToControl(m_pConfig);
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CRayTraceDlg::ConfigToControl(RAYTRACE_CONFIG* pConfig)
{
	CString strTxt;
	m_chkEnableDirect.SetCheck(pConfig->bEnableDirect ? BST_CHECKED : BST_UNCHECKED);
	m_chkMainLightOnly.SetCheck(pConfig->bMainLightOnly ? BST_CHECKED : BST_UNCHECKED);
	m_sldHardness.SetPos((int)(sqrt(pConfig->fShadowHardness) * 100));
	m_sldConstantColor.SetPos((int)(pConfig->fContantColor * 100));
    m_sldOrenNayarRho.SetPos((int)(pConfig->fOrenNayarRho * 10));

    strTxt.Format("%d", pConfig->nSampleForModel);
	m_cmbModelSample.SetWindowText(strTxt);
	strTxt.Format("%d", pConfig->nSampleForTerrain);
	m_cmbTerrainSample.SetWindowText(strTxt);
	m_chkAOAllDir.SetCheck(pConfig->bAOAllDir ? BST_CHECKED : BST_UNCHECKED);
	
	m_chkEnableAO.SetCheck(pConfig->bEnableAO ? BST_CHECKED : BST_UNCHECKED);
	strTxt.Format("%d", pConfig->nAOSample);
	m_cmbAOSample.SetWindowText(strTxt);
	strTxt.Format("%g", pConfig->fAOEnhance);
	m_cmbAOEnhance.SetWindowText(strTxt);
	strTxt.Format("%g", pConfig->fAODist);
	m_cmbAODist.SetWindowText(strTxt);
    m_sldAOStart.SetPos((int)(pConfig->fAOStart * 100));

	m_chkEnableIndirect.SetCheck(pConfig->bEnableIndirect ? BST_CHECKED : BST_UNCHECKED);
	strTxt.Format("%d", pConfig->nEmitPhoton);
	m_edtEmitPhoton.SetWindowText(strTxt);
	strTxt.Format("%d", pConfig->nMaxBounce);
	m_cmbBounce.SetWindowText(strTxt);
	strTxt.Format("%d", pConfig->nGatherSample);
	m_cmbGatherSample.SetWindowText(strTxt);
	strTxt.Format("%g", pConfig->fMaxCacheErr);
	m_cmbMaxErr.SetWindowText(strTxt);
	m_chkSkipBright.SetCheck(pConfig->bSkipBright ? BST_CHECKED : BST_UNCHECKED);

	char szFullPath[MAX_PATH];

	GetFullPathName(szToolPath, MAX_PATH - 1, szFullPath, NULL);
	m_edtTool.SetWindowText(szFullPath);
	
	m_sldDirectScale.SetPos((int)(pConfig->fDirectScale * 10));
	m_sldIndirectScale.SetPos((int)(pConfig->fIndirectScale * 10));
	m_sldAOScale.SetPos((int)(pConfig->fAOScale * 10));
	m_sldGamma.SetPos((int)(pConfig->fGamma * 10));
    
	m_sldHardness.EnableWindow(pConfig->bEnableDirect);
	m_chkMainLightOnly.EnableWindow(pConfig->bEnableDirect);
	m_cmbModelSample.EnableWindow(pConfig->bEnableDirect);
	m_cmbTerrainSample.EnableWindow(pConfig->bEnableDirect);
	m_sldConstantColor.EnableWindow(pConfig->bEnableDirect);

	m_cmbAOSample.EnableWindow(pConfig->bEnableAO);
	m_cmbAOEnhance.EnableWindow(pConfig->bEnableAO);
	m_chkAOAllDir.EnableWindow(pConfig->bEnableAO);
	m_cmbAODist.EnableWindow(pConfig->bEnableAO);
    //m_cmbAOSample.EnableWindow(FALSE);
    //m_cmbAOEnhance.EnableWindow(FALSE);
    //m_chkAOAllDir.EnableWindow(FALSE);
    //m_cmbAODist.EnableWindow(FALSE);

	m_edtEmitPhoton.EnableWindow(pConfig->bEnableIndirect);
	m_cmbBounce.EnableWindow(pConfig->bEnableIndirect);
	m_cmbGatherSample.EnableWindow(pConfig->bEnableIndirect);
	m_cmbMaxErr.EnableWindow(pConfig->bEnableIndirect);
	if (pConfig->iDayNight & 1)
	{
		m_chkDay.SetCheck(BST_CHECKED);
	}
	if (pConfig->iDayNight & 2)
	{
		m_chkNight.SetCheck(BST_CHECKED);
	}
	//strTxt.Format("%d", pConfig->iDefautSize);
	//m_cmbDefautSize.SetWindowText(strTxt);

	strTxt.Format("%d", pConfig->iStartObjIdx);
	m_edtStartObj.SetWindowText(strTxt);
	strTxt.Format("%d", pConfig->iEndObjIdx);
	m_edtEndObj.SetWindowText(strTxt);
    strTxt.Format("%g", pConfig->fMaxVolume);
    m_edtMaxVolume.SetWindowText(strTxt);
    strTxt.Format("%g", pConfig->fMaxSize);
    m_edtSizeLimit.SetWindowText(strTxt);

	m_chkSelOnly.SetCheck(pConfig->bSelectOnly ? BST_CHECKED : BST_UNCHECKED);
	m_chkGenTerrain.SetCheck(pConfig->bGenTerrain ? BST_CHECKED : BST_UNCHECKED);
	m_chkReloadMap.SetCheck(pConfig->bReloadMap ? BST_CHECKED : BST_UNCHECKED);
	//m_chkHDR.SetCheck(pConfig->bHDREncode ? BST_CHECKED : BST_UNCHECKED);
    m_chkOnlyTerrain.SetCheck(pConfig->bOnlyTerrain ? BST_CHECKED : BST_UNCHECKED);

    m_chkBlock.SetCheck(pConfig->bBlockRender ? BST_CHECKED : BST_UNCHECKED);
    strTxt.Format("%g", pConfig->fBlockSize);
    m_edtBlockSize.SetCurSel((int)(log(pConfig->fBlockSize) / log(2.0f)) - 7);
    m_chkVertLitOnly.SetCheck(pConfig->bVertLitOnly ? BST_CHECKED : BST_UNCHECKED);


}

void CRayTraceDlg::ControlToConfig(RAYTRACE_CONFIG* pConfig)
{
	CString strTxt;
	pConfig->bEnableDirect = m_chkEnableDirect.GetCheck() == BST_CHECKED;
	pConfig->bMainLightOnly = m_chkMainLightOnly.GetCheck() == BST_CHECKED;
	pConfig->fShadowHardness = (m_sldHardness.GetPos() * m_sldHardness.GetPos() / 10000.0f);
	pConfig->fContantColor = m_sldConstantColor.GetPos() / 100.0f;
    pConfig->fOrenNayarRho = m_sldOrenNayarRho.GetPos() / 10.0f;
	m_cmbModelSample.GetWindowText(strTxt);
	pConfig->nSampleForModel = atoi(strTxt);
	m_cmbTerrainSample.GetWindowText(strTxt);
	pConfig->nSampleForTerrain = atoi(strTxt);

	pConfig->bEnableAO = m_chkEnableAO.GetCheck() == BST_CHECKED;
	m_cmbAOSample.GetWindowText(strTxt);
	pConfig->nAOSample = atoi(strTxt);
	m_cmbAOEnhance.GetWindowText(strTxt);
	pConfig->fAOEnhance = (float) atof(strTxt);
	pConfig->bAOAllDir = m_chkAOAllDir.GetCheck() == BST_CHECKED;
	m_cmbAODist.GetWindowText(strTxt);
	pConfig->fAODist = (float) atof(strTxt);
	pConfig->fAOStart = m_sldAOStart.GetPos() / 100.0f;

	pConfig->bEnableIndirect = m_chkEnableIndirect.GetCheck() == BST_CHECKED;
	m_edtEmitPhoton.GetWindowText(strTxt);
	pConfig->nEmitPhoton = atoi(strTxt);
	m_cmbBounce.GetWindowText(strTxt);
	pConfig->nMaxBounce = atoi(strTxt);
	m_cmbGatherSample.GetWindowText(strTxt);
	pConfig->nGatherSample = atoi(strTxt);
	m_cmbMaxErr.GetWindowText(strTxt);
	pConfig->fMaxCacheErr = (float)atof(strTxt);
	pConfig->bSkipBright = m_chkSkipBright.GetCheck() == BST_CHECKED;
	m_edtTool.GetWindowText(szToolPath, MAX_PATH - 1);

	pConfig->fDirectScale = m_sldDirectScale.GetPos() / 10.0f;
	pConfig->fIndirectScale = m_sldIndirectScale.GetPos() / 10.0f;
	pConfig->fAOScale = m_sldAOScale.GetPos() / 10.0f;
	pConfig->fGamma = m_sldGamma.GetPos() / 10.0f;

	pConfig->iDayNight = 0;
	if (m_chkDay.GetCheck() == BST_CHECKED)
		pConfig->iDayNight += 1;
	if (m_chkNight.GetCheck() == BST_CHECKED)
		pConfig->iDayNight += 2;
	
	//m_cmbDefautSize.GetWindowText(strTxt);
	//pConfig->iDefautSize = atoi(strTxt);
	m_edtStartObj.GetWindowText(strTxt);
	pConfig->iStartObjIdx = atoi(strTxt);
	m_edtEndObj.GetWindowText(strTxt);
	pConfig->iEndObjIdx = atoi(strTxt);
    m_edtMaxVolume.GetWindowText(strTxt);
    pConfig->fMaxVolume = (float)atof(strTxt);
    m_edtSizeLimit.GetWindowText(strTxt);
    pConfig->fMaxSize = (float)atof(strTxt);

	pConfig->bSelectOnly = m_chkSelOnly.GetCheck() == BST_CHECKED;
	pConfig->bGenTerrain = m_chkGenTerrain.GetCheck() == BST_CHECKED;
	pConfig->bReloadMap = m_chkReloadMap.GetCheck() == BST_CHECKED;
	//pConfig->bHDREncode = m_chkHDR.GetCheck() == BST_CHECKED;
    pConfig->bOnlyTerrain = m_chkOnlyTerrain.GetCheck() == BST_CHECKED;

    pConfig->bBlockRender = m_chkBlock.GetCheck() == BST_CHECKED;
    m_edtBlockSize.GetWindowText(strTxt);
    pConfig->fBlockSize = (float)atof(strTxt);
    pConfig->bVertLitOnly = m_chkVertLitOnly.GetCheck() == BST_CHECKED;
}

void CRayTraceDlg::OnChkDirectlight() 
{
	BOOL bEnabled = m_chkEnableDirect.GetCheck() == BST_CHECKED;
	m_chkMainLightOnly.EnableWindow(bEnabled);
	m_sldHardness.EnableWindow(bEnabled);
	m_cmbModelSample.EnableWindow(bEnabled);
	m_cmbTerrainSample.EnableWindow(bEnabled);
	m_sldConstantColor.EnableWindow(bEnabled);
}

void CRayTraceDlg::OnChkIndirectlight() 
{
	BOOL bEnabled = m_chkEnableIndirect.GetCheck() == BST_CHECKED;
	m_edtEmitPhoton.EnableWindow(bEnabled);
	m_cmbBounce.EnableWindow(bEnabled);
	m_cmbGatherSample.EnableWindow(bEnabled);
	m_cmbMaxErr.EnableWindow(bEnabled);
    m_chkSkipBright.EnableWindow(bEnabled);
}

void CRayTraceDlg::OnChkAo() 
{
	BOOL bEnabled = FALSE;//m_chkEnableAO.GetCheck() == BST_CHECKED;
	m_cmbAOSample.EnableWindow(bEnabled);
	m_cmbAOEnhance.EnableWindow(bEnabled);
	m_chkAOAllDir.EnableWindow(bEnabled);
	m_cmbAODist.EnableWindow(bEnabled);
}

void CRayTraceDlg::OnBrowse() 
{
	char szFilter[] = "RayTraceLightMapGen.exe|RayTraceLightMapGen.exe|所有程序文件(*.exe)|*.exe||";
	DWORD dwFlags = OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
	CFileDialog	FileDia(TRUE, "RayTraceLightMapGen.exe", NULL, dwFlags, szFilter, NULL);
	
	if (FileDia.DoModal() == IDCANCEL)
		return;
	
	m_edtTool.SetWindowText(FileDia.GetPathName());
}

BOOL CRayTraceDlg::ExportAll(bool bUpdateConfig)
{
	if (bUpdateConfig)
		ControlToConfig(m_pConfig);
	m_lXmlFiles.clear();
	if (m_pCurMap)
	{
		if(m_pConfig->bSelectOnly != FALSE && m_pConfig->bGenTerrain == FALSE)
		{
			APtrList<CSceneObject *>* pSelList = m_pCurMap->GetSceneObjectMan()->GetSceneObjectSelectedList();
			ALISTPOSITION pos = pSelList->GetHeadPosition();
			while(pos)
			{
				CSceneObject* ptemp = pSelList->GetNext(pos);
				if(ptemp->GetObjectType() == CSceneObject::SO_TYPE_STATIC_MODEL)
					goto BEGIN_EXPORT_SCENE;
			}
			MessageBox("木有选择任何静态对象用来计算光照图!", "忘了吧?", MB_OK | MB_ICONINFORMATION);
			return FALSE;
		}
	}
	

BEGIN_EXPORT_SCENE:	
    m_bError = false;
	m_strToolPath = szToolPath;
	int pos1 = m_strToolPath.ReverseFind('\\');
	int pos2 = m_strToolPath.ReverseFind('/');
	if (pos1 < pos2) pos1 = pos2;
	if (pos1 >= 0)
	{
		m_strToolFile = m_strToolPath.Mid(pos1 + 1);
		m_strToolPath = m_strToolPath.Left(pos1);
	}
	else
	{
		m_strToolFile = m_strToolPath;
		m_strToolPath = "";
	}

	if (m_pCurMap)
	{
		RTExportParamsDesc params(m_pCurMap, 0);
		if (ExportCache(&params) == FALSE)
		{
			g_Log.Log("CRayTraceDlg::ExportAll(), ExportCache fail !");
			return FALSE;
		}
		if (ExportXMLForSingleMap(&params) == FALSE)
		{
			g_Log.Log("CRayTraceDlg::ExportAll(), ExportXMLForSingleMap fail !");
			return FALSE;
		}
	}
	else
	{
		std::vector<CString> lMaps;
		DWORD dwCol = 1;
		LoadProjectList(lMaps, dwCol);
		DWORD dwCount = (DWORD) lMaps.size();
		if (dwCount == 0)
		{
			MessageBox("当前工作空间没有任何地图！", "错误");
			return FALSE;
		}
		DWORD dwRow = (dwCount + dwCol - 1) / dwCol;
		CElementMap* pMap = new CElementMap;
		if (!pMap->Load(lMaps[0], true))
		{
			MessageBox("打开地图出错！", "错误");
			delete pMap;
			return FALSE;
		}
		DWORD dwRealCol = dwCol;
		if (dwRow < 2) dwRow = 2;
		if (dwCol < 2) dwCol = 2;

		CExportOrderGen order(dwRow, dwCol, pMap->GetLightingParams().vSunDir);
		pMap->Release();
		delete pMap;
		pMap = NULL;

		for (int iRow = 0; iRow < (int)dwRow - 1; iRow++)
		{
			for (int iCol = 0; iCol < (int)dwCol - 1; iCol++)
			{
				CString strGenMaps[4] = {"", "", "", ""};
				bool bGen[4] = {false, false, false, false};
				for (int iMap = 0; iMap < 4; iMap++)
				{
					if (order.iNeighbours[iMap].x >= (int)dwRealCol)
						continue;
					int index = order.iNeighbours[iMap].y * dwRealCol + order.iNeighbours[iMap].x;
					if (index < (int)dwCount)
					{
						strGenMaps[iMap] = lMaps[index];
						bGen[iMap] = order.bGenLit[iMap];
					}
				}
				ExportXMLFor4Maps(iRow * (dwCol - 1) + iCol, strGenMaps, bGen);
				order.Next();
			}
		}
	}
	
	return !m_bError;
}


void CRayTraceDlg::OnExport() 
{
	if (ExportAll() == FALSE)
	{
		return;
	}

	if (ExportBAT() == FALSE)
	{
		return;
	}
	
	CDialog::OnOK();
}

void CRayTraceDlg::OnOK() 
{
	if (m_pCurMap == NULL)
		return;

	if (GetObjCount(m_pCurMap) > 600)
	{
		if (IDNO == MessageBox("建议点击【仅导出场景】按钮，然后关闭编辑器采用离线方式算光，\n否则可能造成计算过程中资源不足。\n\n请选择是否按当前模式继续？（建议选否）", "你的场景规模较大！", MB_YESNO))
			return;
	}

	g_Log.Log("CRayTraceDlg::OnOK(), start ExportAll !");
	if (ExportAll() == FALSE)
	{
		return;
	}
	RTExportParamsDesc params(m_pCurMap, 0);
	g_Log.Log("CRayTraceDlg::OnOK(), start GenLightMap !");
	if (GenLightMap(&params) == FALSE)
	{
		return;
	}

	g_Log.Log("CRayTraceDlg::OnOK(), finish GenLightMap !");
	CDialog::OnOK();
}

BOOL CRayTraceDlg::ExportXMLForSingleMap(RTExportParamsDesc* pParams)
{
	// 导出XML文件
	CString strXML;
			
	strXML.Format("%s\\%s.xml", m_strToolPath, "LitmapSceneRayTrace");
	m_strXMLFile = strXML;
	m_lXmlFiles.push_back(strXML);
	FILE* fp = fopen(strXML, "wb");
	if (fp == NULL)
	{
		MessageBox("无法建立场景导出文件!", "错误", MB_OK);
		return FALSE;
	}

	// 开始写
	// 输出场景
	fprintf(fp,"<Scene Gamma=\"%g\" BaseDir=\"%s\" CacheDir=\"%s\" TerrainSample=\"%d\" ModelSample=\"%d\" DayScale=\"%g\" NightScale=\"%g\">\r\n", 
		m_pConfig->fGamma, g_szWorkDir, pParams->strLitmapCacheDir,  
        m_pConfig->nSampleForTerrain, m_pConfig->nSampleForModel, g_GlobalData.fBrightness, g_GlobalData.fBrightness_n);

	ExportXML_Camera(fp, g_Render.GetA3DEngine()->GetActiveCamera());
	ExportXML_Solvers(fp);
	ExportXML_Terrain(fp, pParams, A3DVECTOR3(0, 0, 0));
	ExportXML_Models(fp, pParams, A3DVECTOR3(0, 0, 0));
	ExportXML_MirrorPlanes(fp, pParams, A3DVECTOR3(0, 0, 0));
	ExportXML_MainLights(fp, pParams);
	ExportXML_Lights(fp, pParams, A3DVECTOR3(0, 0, 0));

	fputs("</Scene>\r\n", fp);
	fclose(fp);

	return TRUE;
}

BOOL CRayTraceDlg::ExportXMLFor4Maps(int index, CString strMaps[4], bool bGenMap[4])
{
	// 导出XML文件
	CString strXML;
	if (index == 0)	
		strXML.Format("%s\\%s.xml", m_strToolPath, "LitmapSceneRayTrace");
	else
		strXML.Format("%s\\%s-%d.xml", m_strToolPath, "LitmapSceneRayTrace", index);
	m_strXMLFile = strXML;
	m_lXmlFiles.push_back(strXML);
	FILE* fp = fopen(strXML, "wb");
	if (fp == NULL)
	{
		MessageBox("无法建立场景导出文件!", "错误", MB_OK);
		return FALSE;
	}

	CElementMap* pMap = new CElementMap;
	if (!pMap->Load(strMaps[0]))
	{
		MessageBox("无法打开地图文件!", "错误", MB_OK);

		delete pMap;
		return FALSE;
	}

	RTExportParamsDesc paramStart(pMap, 0);
	// 开始写
	// 输出场景
	fprintf(fp,"<Scene Gamma=\"%g\" BaseDir=\"%s\" CacheDir=\"%s\" TerrainSample=\"%d\" ModelSample=\"%d\" DayScale=\"%g\" NightScale=\"%g\">\r\n", 
		m_pConfig->fGamma, g_szWorkDir, paramStart.strLitmapCacheDir,  m_pConfig->nSampleForTerrain, m_pConfig->nSampleForModel,
        g_GlobalData.fBrightness, g_GlobalData.fBrightness_n);
	ExportXML_Camera(fp, g_Render.GetA3DEngine()->GetActiveCamera());
	ExportXML_Solvers(fp);

	CTerrain* pTerrain = pMap->GetTerrain();
	CTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();
	float fTerrainWidth = pTerrain->GetTileSize() * (pHeightMap->iWidth - 1);
	float fTerrainHeight = pTerrain->GetTileSize() * (pHeightMap->iHeight - 1);

	float fOffsetX[4] = {0, fTerrainWidth, 0, fTerrainWidth};
	float fOffsetZ[4] = {0, 0, -fTerrainHeight, -fTerrainHeight};

	for (int i = 0; i < 4; i++)
	{
		if (strMaps[i] == "")
			continue;
		if (pMap == NULL)
		{
			pMap = new CElementMap;
			if (!pMap->Load(strMaps[i]))
			{
				delete pMap;
				pMap = NULL;
				continue;
			}
		}
		m_pConfig->bGenTerrain = bGenMap[i];
		m_pConfig->bSelectOnly = !bGenMap[i];
		RTExportParamsDesc params(pMap, i + index * 4);
		ExportCache(&params);
		ExportXML_Terrain(fp, &params, A3DVECTOR3(fOffsetX[i], 0, fOffsetZ[i]));
		ExportXML_Models(fp, &params, A3DVECTOR3(fOffsetX[i], 0, fOffsetZ[i]));
		if (i == 0)
			ExportXML_MainLights(fp, &params);
		ExportXML_Lights(fp, &params, A3DVECTOR3(fOffsetX[i], 0, fOffsetZ[i]));

		pMap->Release();
		delete pMap;
		pMap = NULL;
	}	
	fputs("</Scene>\r\n", fp);
	fclose(fp);

	
	return TRUE;
}


BOOL CRayTraceDlg::ExportXML_Camera(FILE* fp, A3DCameraBase* pCamera)
{
	// 摄像机
	A3DVECTOR3 vCameraPos, vCameraUp, vCameraDir;
	vCameraPos = pCamera->GetPos();
	vCameraUp = pCamera->GetUp();
	vCameraDir = pCamera->GetDir();
	
	fprintf(fp,"\t<Camera Pos=\"%g,%g,%g\" Up=\"%g,%g,%g\" Dir=\"%g,%g,%g\"/>\r\n",
		vCameraPos.x, vCameraPos.y, vCameraPos.z, vCameraUp.x, vCameraUp.y, vCameraUp.z,
		vCameraDir.x, vCameraDir.y, vCameraDir.z);

	return TRUE;
}

BOOL CRayTraceDlg::ExportXML_Solvers(FILE* fp)
{
	// 应用算法
	if (m_pConfig->bEnableDirect)
	{
        float rho = m_pConfig->fOrenNayarRho;
        if (rho < 0.001f)
            rho = 0.001f;
        rho *= rho;
        rho = 1.0f / rho;

		fprintf(fp,"\t<DirectLightSolver ConstLight=\"%g\" MainOnly=\"%d\" Rho=\"%g\" Scale=\"%g\"/>\r\n", 
			m_pConfig->fContantColor * 0.5f, m_pConfig->bMainLightOnly, rho, m_pConfig->fDirectScale);
	}
	
	if (m_pConfig->bEnableAO)
	{
		fprintf(fp,"\t<AmbientOcclusionSolver Sample=\"%d\" Enhancement=\"%g\" AllDir=\"%d\" Dist=\"%g\" Scale=\"%g\" />\r\n", 
			m_pConfig->nAOSample, m_pConfig->fAOEnhance, m_pConfig->bAOAllDir, m_pConfig->fAODist, m_pConfig->fAOScale);
	}
	
	if (m_pConfig->bEnableIndirect)
	{
		fprintf(fp,"\t<PhotonMappingSolver EmitPhoton=\"%d\" MaxBounce=\"%d\" FinalGatherSample=\"%d\" MaxErr=\"%g\" SkipBright=\"%d\" Scale=\"%g\"/>\r\n", 
			m_pConfig->nEmitPhoton, m_pConfig->nMaxBounce, m_pConfig->nGatherSample, m_pConfig->fMaxCacheErr, m_pConfig->bSkipBright, m_pConfig->fIndirectScale);
	}

	return TRUE;
}

BOOL CRayTraceDlg::ExportXML_Terrain(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset)
{
	CTerrain* pTerrain = pParams->pMap->GetTerrain();
	CTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();
	
    int iWidth = pHeightMap->iWidth;
    int iHeight = pHeightMap->iHeight;
    int iSkip = 1;
    while (iWidth > 513)
    {
        iWidth = iWidth / 2 + 1;
        iHeight = iHeight / 2 + 1;
        iSkip *= 2;
    }
	// 地形数据
	fprintf(fp, 
		"\t<Terrain Raw=\"%s\"\r\n\t\tMinHeight=\"%g\" MaxHeight=\"%g\" Width=\"%d\" Height=\"%d\"\r\n\t\t",
		pParams->strTerrainHeightPath, pTerrain->GetYOffset(),
		pTerrain->GetMaxHeight(), iWidth, iHeight);
	
	fprintf(fp, "BlockWidth=\"%d\" BlockHeight=\"%d\"\r\n\t\t",
		pParams->nTerrainBlockSize, pParams->nTerrainBlockSize);
	
	int nNumWidth  = iWidth  / pParams->nTerrainBlockSize;
	int nNumHeight = iHeight / pParams->nTerrainBlockSize;
	int nTerrainLitmapSize = pParams->LtParamsDay.iMapSize;
	
	fprintf(fp, "LitmapWidth=\"%d\" LitmapHeight=\"%d\" GenMap=\"%d\" \r\n\t\t",
		pParams->LtParamsDay.iMapSize, pParams->LtParamsDay.iMapSize, m_pConfig->bGenTerrain ? 1 : 0);
	
	static char *pTerrainName[] = {"\\TerrainD.bmp", "\\TerrainN.bmp"};
	
	fprintf(fp, "OutputDay=\"%s\"\r\n\t\t",
		pParams->strLitmapPath + pTerrainName[0]);
	
	fprintf(fp, "OutputNight=\"%s\"\r\n\t\t",
		pParams->strLitmapPath + pTerrainName[1]);
	
	fprintf(fp, "Offset=\"%g,%g,%g\"\r\n\t\t",
		vOffset.x, vOffset.y, vOffset.z);

	fprintf(fp, "TileSize=\"%g\" OriginX=\"%d\" OriginZ=\"%d\"\r\n\tDiffuse=\"%s\"/>\r\n",
		pTerrain->GetTileSize() * iSkip,
		iWidth >> 1, iHeight >> 1,
		pParams->strTerrainDiffusePath);

	return TRUE;
}

BOOL CRayTraceDlg::ExportXML_Models(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset)
{
	CSceneObjectManager* pSceneObjectManager = pParams->pMap->GetSceneObjectMan();
	
	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
	ALISTPOSITION pos = listModel->GetTailPosition();
	
	CString strName;
	CString strLitName;
	int		idRenderSect = 0;
	AArray<CStaticModelObject*> aLargeObjects;
    
    float fSizeLimit = m_pConfig->fMaxSize + 10;
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
            A3DLitModel * pModel = (pStaticModel->GetA3DModel()->pModel)->GetA3DLitModel();
            if (pModel == NULL || pModel->GetNumMeshes() == 0)
                continue;

			if (pStaticModel->GetCollideFlags())
				continue;
            
			strName.Format("%05d.Lit20", pStaticModel->GetObjectID());
			strLitName.Format("%s\\" LIGHT_MAP_VERSION "%05d_%sRender", pParams->strLitmapPath, 
				pStaticModel->GetObjectID(), pParams->strMapName);
			
			idRenderSect = ptemp->IsSelected() ? 1 : 0;
			if (m_pConfig->bSelectOnly == FALSE)
				idRenderSect = 1;
			if (m_pConfig->bOnlyTerrain)
                idRenderSect = 0;

			int nlms = pStaticModel->GetCustomLightmapSize();
			if (nlms == -1)
            {
                //nlms = m_pConfig->iDefautSize;
                nlms = pStaticModel->GetAdaptLightmapSize(STATIC_OBJ_LITMAP_SIZE_FACTOR);
                
            }
            
            if (nlms < 64)
            {
                for (int iMesh = 0; iMesh < pModel->GetNumMeshes(); iMesh++)
                {
                    A3DLitMesh* pMesh = pModel->GetMesh(iMesh);
                    pMesh->SetUseLightMap(false);
                }
            }

            if (idRenderSect == 1 && pModel->GetMesh(0)->GetLightMapCoords() != NULL)
            {
                float fVolume = pStaticModel->ComputeVolume();
                
                if (fVolume > m_pConfig->fMaxVolume)
                {
                    aLargeObjects.Add(pStaticModel);
                }
                else
                {
                    A3DOBB obb = pStaticModel->GetOBB();
                    A3DVECTOR3 vSize = obb.Extents * 2 * pModel->GetScale().x;
                    // make sure X,Y are minimal axises
                    if (vSize.x > vSize.z && vSize.x > vSize.y) vSize.x = vSize.z;
                    if (vSize.y > vSize.z && vSize.y > vSize.x) vSize.y = vSize.z;

                    if (vSize.x > fSizeLimit || vSize.y > fSizeLimit)
                    {
                        A3DAABB aabb = pStaticModel->GetRealAABB();
                        aabb.CompleteCenterExts();
                        A3DVECTOR3 vExtents = aabb.Extents * 2;
                        if (vExtents.x > fSizeLimit || vExtents.z > fSizeLimit)
                            aLargeObjects.Add(pStaticModel);
                    }
                }
                //else if (nlms > 512)
                //{
                //    aLargeObjects.Add(pStaticModel);
                //}
            }

			float fScales[3];
			pStaticModel->GetLitScales(fScales, fScales + 1, fScales + 2);
			
			AString strScales = "";
			if (m_pConfig->bEnableDirect)
				strScales += AString().Format("%g,", fScales[0]);
			if (m_pConfig->bEnableAO)
				strScales += AString().Format("%g,", fScales[2]);
			if (m_pConfig->bEnableIndirect)
				strScales += AString().Format("%g,", fScales[1]);
			if (strScales.GetLength() > 0)
				strScales.CutRight(1);
			
			fprintf(fp, "\t<Model File=\"%s\"", pParams->strLitmapCacheDir + "\\" + strName);
			fprintf(fp, "\r\n\t\tLMWidth=\"%d\" LMHeight=\"%d\" GenMap=\"%d\" LitScales=\"%s\"",
				nlms, nlms, idRenderSect, strScales);
			
			fprintf(fp, "\r\n\t\tOffset=\"%g,%g,%g\"",
				vOffset.x, vOffset.y, vOffset.z);
            fprintf(fp, "\r\n\t\tName=\"%s\"", pStaticModel->GetObjectName());
			fprintf(fp, "\r\n\t\tLightMapDay=\"%s\"", strLitName + pParams->cPrefix[0]);
			fprintf(fp, "\r\n\t\tLightMapNight=\"%s\"", strLitName + pParams->cPrefix[1]);
            fprintf(fp, "\r\n\t\tCastShadow=\"%d\"", pStaticModel->IsNoShadow() ? 0 : 1);
            fprintf(fp, "\r\n\t\tConsistentNormal=\"%d\"", pStaticModel->GetUseConsistentNormal() ? 1 : 0);
            fprintf(fp, "\r\n\t\tIgnoreInnerShadow=\"%d\"", pStaticModel->GetIgnoreInnerShadow() ? 1 : 0);
            fprintf(fp, ">");
			LIGHTDATA emissive;
			AString strEmissive;
			int nSamples;
			pStaticModel->GetEmissiveData(&emissive, &strEmissive, &nSamples);
			if (strEmissive != "")
			{
				fprintf(fp, "\r\n\t\t<Emission Map=\"%s\" Sample=\"%d\"", strEmissive, nSamples);
				fprintf(fp, "\r\n\t\t\tAttenuation=\"%g,%g,%g\" Range=\"%g\" Luminance=\"%g\" DayNight=\"%d\"", 
					emissive.m_fAttenuation0, emissive.m_fAttenuation1, emissive.m_fAttenuation2,
					emissive.m_fRange, emissive.m_fColorScale, emissive.m_nEffectType + 1);
				fprintf(fp, "/>\r\n");
			}
			
			fprintf(fp, "\t</Model>\r\n");
		}
	}

    if (aLargeObjects.GetSize() > 0)
    {
        AString strMsg;
        strMsg.Format("错误！%d个物体的体积过大：\n", aLargeObjects.GetSize());
        for (int i = 0; i < aLargeObjects.GetSize(); i++)
        {
            strMsg += aLargeObjects[i]->GetObjectName();
            A3DOBB obb = aLargeObjects[i]->GetOBB();
            A3DVECTOR3 vSize = obb.Extents * 2 * aLargeObjects[i]->GetA3DModel()->pModel->GetScale().x;
            // make sure X,Y are minimal axises
            if (vSize.x > vSize.z && vSize.x > vSize.y) vSize.x = vSize.z;
            if (vSize.y > vSize.z && vSize.y > vSize.x) vSize.y = vSize.z;

            //A3DAABB aabb = aLargeObjects[i]->GetRealAABB();
            //aabb.CompleteCenterExts();
            AString strVolume;
            strVolume.Format(":X = %f, Z = %f", vSize.x, vSize.y);
            strMsg += strVolume;
            strMsg += "\n";
        }
        MessageBox(strMsg);
        m_bError = true;
        return FALSE;
    }
	return TRUE;
}

BOOL CRayTraceDlg::ExportXML_MirrorPlanes(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset)
{
	CSceneObjectManager* pSceneObjectManager = pParams->pMap->GetSceneObjectMan();

	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_MIRROR_PLANE);
	ALISTPOSITION pos = listModel->GetTailPosition();

	CString strName;
	CString strLitName;
	int		idRenderSect = 0;

	float fSizeLimit = m_pConfig->fMaxSize + 10;
	// Output models
	while( pos )
	{
		CSceneObject* ptemp = listModel->GetPrev(pos);
		if( !ptemp->IsDeleted() )
		{
			// Lit this model
			CMirrorPlaneObject * pMirrorPlane = (CMirrorPlaneObject *) ptemp;
			if (pMirrorPlane == NULL || pMirrorPlane->GetMirrorPlane() == NULL)
				continue;

			strName.Format("%05d.Lit20", pMirrorPlane->GetObjectID());
			strLitName.Format("%s\\" LIGHT_MAP_VERSION "%05d_%sRender", pParams->strLitmapPath, 
				pMirrorPlane->GetObjectID(), pParams->strMapName);

			idRenderSect = ptemp->IsSelected() ? 1 : 0;
			if (m_pConfig->bSelectOnly == FALSE)
				idRenderSect = 1;
			if (m_pConfig->bOnlyTerrain)
				idRenderSect = 0;

			int nlms = pMirrorPlane->GetCustomLightmapSize();
			if (nlms == -1)
			{
				nlms = pMirrorPlane->GetAdaptLightmapSize(MIRROR_PLANE_OBJ_LITMAP_SIZE_FACTOR);
			}

			float fScales[3];
			fScales[0] = pMirrorPlane->GetLitDirectScale();
			fScales[1] = pMirrorPlane->GetLitIndirectScale();
			fScales[2] = pMirrorPlane->GetLitAOScale();

			AString strScales = "";
			if (m_pConfig->bEnableDirect)
				strScales += AString().Format("%g,", fScales[0]);
			if (m_pConfig->bEnableAO)
				strScales += AString().Format("%g,", fScales[2]);
			if (m_pConfig->bEnableIndirect)
				strScales += AString().Format("%g,", fScales[1]);
			if (strScales.GetLength() > 0)
				strScales.CutRight(1);

			fprintf(fp, "\t<Model File=\"%s\"", pParams->strLitmapCacheDir + "\\" + strName);
			fprintf(fp, "\r\n\t\tLMWidth=\"%d\" LMHeight=\"%d\" GenMap=\"%d\" LitScales=\"%s\"",
				nlms, nlms, idRenderSect, strScales);

			fprintf(fp, "\r\n\t\tOffset=\"%g,%g,%g\"",
				vOffset.x, vOffset.y, vOffset.z);
			fprintf(fp, "\r\n\t\tName=\"%s\"", pMirrorPlane->GetObjectName());
			fprintf(fp, "\r\n\t\tLightMapDay=\"%s\"", strLitName + pParams->cPrefix[0]);
			fprintf(fp, "\r\n\t\tLightMapNight=\"%s\"", strLitName + pParams->cPrefix[1]);
			fprintf(fp, "\r\n\t\tCastShadow=\"%d\"", 0 );
			fprintf(fp, ">");

			fprintf(fp, "\t</Model>\r\n");
		}
	}

	return TRUE;
}

void CRayTraceDlg::ComputeLightMapSize(CElementMap* pMap)
{
    CSceneObjectManager* pSceneObjectManager = pMap->GetSceneObjectMan();

    const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
    ALISTPOSITION pos = listModel->GetTailPosition();

    CString strName;
    CString strLitName;
    int		idRenderSect = 0;
    int nLightMapSize = 0;
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
            A3DLitModel * pModel = (pStaticModel->GetA3DModel()->pModel)->GetA3DLitModel();
            if (pModel == NULL || pModel->GetNumMeshes() == 0)
                continue;
            if (pStaticModel->GetCollideFlags())
                continue;
            
            // Vertex Light
            if (pModel->GetMesh(0)->GetLightMapCoords() == NULL)
                continue;

            int nlms = pStaticModel->GetAdaptLightmapSize(8.0f);
            
            // Compute mip-map
            while (nlms > 0)
            {
                nLightMapSize += nlms * nlms / 2;
                nlms >>= 1;
            }
            nlms += 128; // dds header
        }
    }
    int nTerrainLightMapSize = pMap->GetLightingParams().iMapSize;
    nTerrainLightMapSize *= nTerrainLightMapSize * 4;

    AString strMsg;
    strMsg.Format("模型光照图尺寸：%gM，地形光照图尺寸：%gM，总尺寸：%gM\n", 
        nLightMapSize / 1024.0f / 1024.0f * 2, 
        nTerrainLightMapSize / 1024.0f / 1024.0f * 2, 
        (nLightMapSize + nTerrainLightMapSize) / 1024.0f / 1024.0f * 2
        );

    MessageBox(strMsg);
}


BOOL CRayTraceDlg::ExportXML_MainLights(FILE* fp, RTExportParamsDesc* pParams)
{
	// Output directional light

	const A3DVECTOR3 vSunDir = -Normalize(pParams->LtParamsDay.vSunDir);

	float AmbientDay[3] = 
	{
		((pParams->LtParamsDay.dwAmbient & 0x00FF0000) >> 16) / 255.0f * pParams->LtParamsDay.fAmbientPower,
		((pParams->LtParamsDay.dwAmbient & 0x0000FF00) >> 8) / 255.0f * pParams->LtParamsDay.fAmbientPower,
		((pParams->LtParamsDay.dwAmbient & 0x000000FF)) / 255.0f * pParams->LtParamsDay.fAmbientPower,
	};
	
	float AmbientNight[3] = 
	{
		((pParams->LtParamsNight.dwAmbient & 0x00FF0000) >> 16) / 255.0f * pParams->LtParamsNight.fAmbientPower,
		((pParams->LtParamsNight.dwAmbient & 0x0000FF00) >> 8) / 255.0f * pParams->LtParamsNight.fAmbientPower,
		((pParams->LtParamsNight.dwAmbient & 0x000000FF)) / 255.0f * pParams->LtParamsNight.fAmbientPower,
	};

    float Ambient2Day[3] = 
    {
        ((pParams->LtParamsDay.dwAmbient2 & 0x00FF0000) >> 16) / 255.0f * pParams->LtParamsDay.fAmbientPower,
        ((pParams->LtParamsDay.dwAmbient2 & 0x0000FF00) >> 8) / 255.0f * pParams->LtParamsDay.fAmbientPower,
        ((pParams->LtParamsDay.dwAmbient2 & 0x000000FF)) / 255.0f * pParams->LtParamsDay.fAmbientPower,
    };

    float Ambient2Night[3] = 
    {
        ((pParams->LtParamsNight.dwAmbient2 & 0x00FF0000) >> 16) / 255.0f * pParams->LtParamsNight.fAmbientPower,
        ((pParams->LtParamsNight.dwAmbient2 & 0x0000FF00) >> 8) / 255.0f * pParams->LtParamsNight.fAmbientPower,
        ((pParams->LtParamsNight.dwAmbient2 & 0x000000FF)) / 255.0f * pParams->LtParamsNight.fAmbientPower,
    };

    fprintf(fp, "\t<DirectionalLight Direction=\"%g,%g,%g\" Luminance=\"%g\" Color=\"%lx\" Delta=\"%g\" DayNight=\"1\" Ambient=\"%g,%g,%g\" Ambient2=\"%g,%g,%g\"/>\r\n", 
		vSunDir.x, vSunDir.y, vSunDir.z, pParams->LtParamsDay.fSunPower, pParams->LtParamsDay.dwSunCol, 0.5f * (1 - m_pConfig->fShadowHardness),
		AmbientDay[0], AmbientDay[1], AmbientDay[2], Ambient2Day[0], Ambient2Day[1], Ambient2Day[2]);

	fprintf(fp, "\t<DirectionalLight Direction=\"%g,%g,%g\" Luminance=\"%g\" Color=\"%lx\" Delta=\"%g\" DayNight=\"2\" Ambient=\"%g,%g,%g\" Ambient2=\"%g,%g,%g\"/>\r\n", 
		vSunDir.x, vSunDir.y, vSunDir.z, pParams->LtParamsNight.fSunPower, pParams->LtParamsNight.dwSunCol, 0.5f * (1 - m_pConfig->fShadowHardness),
		AmbientNight[0], AmbientNight[1], AmbientNight[2], Ambient2Night[0], Ambient2Night[1], Ambient2Night[2]);
	return TRUE;
}

BOOL CRayTraceDlg::ExportXML_Lights(FILE* fp, RTExportParamsDesc* pParams, const A3DVECTOR3& vOffset)
{
	// Output point lights
	CSceneObjectManager* pSceneObjectManager = pParams->pMap->GetSceneObjectMan();
	const APtrList<CSceneObject *>* listLight = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_LIGHT);

	ALISTPOSITION pos = listLight->GetTailPosition();
	while(pos)
	{
		CSceneObject* ptemp = listLight->GetPrev(pos);

		if( !ptemp->IsDeleted() )
		{
			//float		fRadius;
			LIGHTDATA	LightData;
			A3DVECTOR3	vPos;

			CSceneLightObject* pLight = (CSceneLightObject*)ptemp;
			pLight->GetLightData(LightData);

			if(LightData.m_nEffectTypeRange == 1)
				continue;
//				if(LightData.m_nEffectType != 2)
//				{
//					if( LightData.m_nEffectType != nDayNight )
//						continue;
//				}
			
			vPos = pLight->GetPos() + vOffset;

			switch (LightData.m_nType)
			{
			case 0: // point light
				fprintf(fp, "\t<ShapeLight Attenuation=\"%g,%g,%g\" Range=\"%g\" Luminance=\"%g\" Color=\"%lx\" DayNight=\"%d\" Shadow=\"%d\">\r\n",
					LightData.m_fAttenuation0, LightData.m_fAttenuation1, LightData.m_fAttenuation2,
                    LightData.m_fRange, LightData.m_fColorScale, LightData.m_dwColor, LightData.m_nEffectType + 1, LightData.m_bShadow ? 1 : 0);
				fprintf(fp, "\t\t<Sphere Pos=\"%g,%g,%g\" Radius=\"%g\"/>\r\n", vPos.x, vPos.y, vPos.z, 1.0f);
				fprintf(fp, "\t</ShapeLight>\r\n");
				break;
			case 1: // directional light
				fprintf(fp, "\t<LocalDirLight Direction=\"%g,%g,%g\" Luminance=\"%g\" Color=\"%lx\" DayNight=\"%d\" Shadow=\"%d\"/>\r\n",
					LightData.m_vDir.x, LightData.m_vDir.y, LightData.m_vDir.z, LightData.m_fColorScale, 
					LightData.m_dwColor, LightData.m_nEffectType + 1, LightData.m_bShadow ? 1 : 0);
				break;
			case 2: // spot light
				fprintf(fp, "\t<SpotLight Pos=\"%g,%g,%g\" Dir= \"%g,%g,%g\" Attenuation=\"%g,%g,%g\" Range=\"%g\"\r\n",
					vPos.x, vPos.y, vPos.z, LightData.m_vDir.x, LightData.m_vDir.y, LightData.m_vDir.z,
					LightData.m_fAttenuation0, LightData.m_fAttenuation1, LightData.m_fAttenuation2,
					LightData.m_fRange);
				fprintf(fp, "\t\tLuminance=\"%g\" Color=\"%lx\" DayNight=\"%d\"\r\n", 
					LightData.m_fColorScale, LightData.m_dwColor, LightData.m_nEffectType + 1);
				fprintf(fp, "\t\tOuter=\"%g\" Inner=\"%g\" Shadow=\"%d\"/>\r\n", 
					LightData.m_fPhi, LightData.m_fTheta, LightData.m_bShadow ? 1 : 0);
				break;
			}
		}
	}

	return TRUE;
}

BOOL CRayTraceDlg::ExportCache(RTExportParamsDesc* pParams)
{
	CreateDirectory(pParams->strLitmapPath, NULL);
	
	if (PrepareCacheDir(pParams) == FALSE)
	{
		g_Log.Log("CRayTraceDlg::ExportCache(), PrepareCacheDir fail !");
		return FALSE;
	}
	if (ExportHeightMap(pParams) == FALSE)
	{
		g_Log.Log("CRayTraceDlg::ExportCache(), ExportHeightMap fail !");
		return FALSE;
	}
	if (ExportTerrainDiffuse(pParams) == FALSE)
	{
		g_Log.Log("CRayTraceDlg::ExportCache(), ExportTerrainDiffuse fail !");
		return FALSE;
	}
	if (ExportModelCache(pParams) == FALSE)
	{
		g_Log.Log("CRayTraceDlg::ExportCache(), ExportModelCache fail !");
		return FALSE;
	}
	if (ExportMirrorPlaneCache(pParams) == FALSE)
	{
		g_Log.Log("CRayTraceDlg::ExportCache(), ExportMirrorPlaneCache fail !");
		return FALSE;
	}
	return TRUE;
}

BOOL CRayTraceDlg::ExportModelCache(RTExportParamsDesc* pParams)
{
	CSceneObjectManager* pSceneObjectManager = pParams->pMap->GetSceneObjectMan();
	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
	ALISTPOSITION pos = listModel->GetTailPosition();

	CString strName;
	
	// Output models
	while( pos )
	{
		CSceneObject* ptemp = listModel->GetPrev(pos);
		if( !ptemp->IsDeleted() )
		{
			CStaticModelObject * pStaticModel = (CStaticModelObject *) ptemp;
			if (pStaticModel == NULL || pStaticModel->GetA3DModel() == NULL || pStaticModel->GetA3DModel()->pModel == NULL)
                continue;
            A3DLitModel * pModel = (pStaticModel->GetA3DModel()->pModel)->GetA3DLitModel();
            if (pModel == NULL)
                continue;
			strName.Format("%05d.Lit20", pStaticModel->GetObjectID());
			strName = pParams->strLitmapCacheDir + "\\" + strName;
			pModel->Save(strName);
		}
	}

	return TRUE;
}

BOOL CRayTraceDlg::ExportMirrorPlaneCache(RTExportParamsDesc* pParams)
{
	CSceneObjectManager* pSceneObjectManager = pParams->pMap->GetSceneObjectMan();
	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_MIRROR_PLANE);
	ALISTPOSITION pos = listModel->GetTailPosition();

	CString strName;

	while( pos )
	{
		CSceneObject* pSceneObject = listModel->GetPrev(pos);
		if( !pSceneObject->IsDeleted() )
		{
			CMirrorPlaneObject* pMirrorPlaneObject = (CMirrorPlaneObject*)pSceneObject;
			if( pMirrorPlaneObject == NULL || pMirrorPlaneObject->GetMirrorPlane() == NULL )
				continue;

			const int nObjectID = pMirrorPlaneObject->GetObjectID();
			strName.Format( "%05d.Lit20", nObjectID );
			strName = pParams->strLitmapCacheDir + "\\" + strName;
			if( !pMirrorPlaneObject->OutputBakingGeometryFile( strName ) )
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CRayTraceDlg::PrepareCacheDir(RTExportParamsDesc* pParams)
{
RETRY_DELSCENECACHE:
	if(DeleteDirectory(pParams->strLitmapCacheDir) == FALSE)
	{
		if(MessageBox("删除光照图计算场景失败!", "错误", MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
			goto RETRY_DELSCENECACHE;
		return FALSE;
	}
	Sleep(500);
RETRY_CREATESCENECACHEDIR:
	if(CreateDirectory(pParams->strLitmapCacheDir, NULL) == FALSE)
	{
		if(MessageBox("创建缓存目录失败!", "错误", MB_RETRYCANCEL | MB_ICONERROR) == IDRETRY)
			goto RETRY_CREATESCENECACHEDIR;
		return FALSE;
	}

	return TRUE;
}

BOOL CRayTraceDlg::ExportHeightMap(RTExportParamsDesc* pParams)
{
	CTerrain::HEIGHTMAP* pHeightMap = pParams->pMap->GetTerrain()->GetHeightMap();
	if (!pHeightMap->pHeightData)
		return FALSE;
	
	FILE* fp = fopen(pParams->strTerrainHeightPath, "wb+");
	if (!fp)
	{
		g_Log.Log("CRayTraceDlg::ExportXML(), 输出地形高度图失败 !");
		return FALSE;
	}
	
    int iWidth = pHeightMap->iWidth;
    int iHeight = pHeightMap->iHeight;
    int iSkip = 1;
    while (iWidth > 513)
    {
        iWidth = iWidth / 2 + 1;
        iHeight = iHeight / 2 + 1;
        iSkip *= 2;
    }


	WORD* pLineBuf = new WORD [iWidth];
	if (!pLineBuf)
	{
		fclose(fp);
		g_Log.Log("CRayTraceDlg::ExportXML(), Not enough memory !");
		return FALSE;
	}
	
	float* pSrc = pHeightMap->pHeightData;
	
	for (int i=0; i < iHeight; i++)
	{
		WORD* pDst = pLineBuf;
		float* pSrcLine = pSrc + pHeightMap->iWidth * i * iSkip;
		for (int j=0; j < iWidth; j++, pDst++, pSrcLine += iSkip)
			*pDst = (WORD)(*pSrcLine * 65535);
		
		fwrite(pLineBuf, 1, iWidth * sizeof (WORD), fp);
	}
	
	delete [] pLineBuf;
	fclose(fp);
	return TRUE;
}

BOOL CRayTraceDlg::ExportTerrainDiffuse(RTExportParamsDesc* pParams)
{	// 导出地面Diffuse图
    return TRUE;

	CElementMap emap;
	const char* pszMapName = pParams->pMap->GetMapName();
	CString mapName;
	mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,pszMapName, pszMapName);

	emap.m_fUoff = pParams->pMap->m_fUoff;
	emap.m_fVoff = pParams->pMap->m_fVoff;
	emap.Load(mapName, true);
	EDITCONFIG oldConfig = g_Configs;

	if(true)
	{
		g_Configs.bShowStaticModel	= false;
		g_Configs.bShowLight		= false;
		g_Configs.bShowAudio		= false;
		g_Configs.bShowBoxArea		= false;
		g_Configs.bShowDummy		= false;
		g_Configs.bShowSpecially	= false;
		g_Configs.bShowWater		= false;
		g_Configs.bShowAIArea		= false;
		g_Configs.bShowPath			= false;
		g_Configs.bShowSkinModel	= false;
		g_Configs.bShowCritterGroup = false;
		g_Configs.bShowFixedNpc		= false;
		//g_Configs.bShowTerrainBlock = false;
		//g_Configs.bShowSmallBlock	= false;
		g_Configs.bShowRain			= false;
		g_Configs.bShowSnow			= false;
		g_Configs.bShowGrass		= false;
		g_Configs.bShowTree			= false;
		g_Configs.bShowGather		= false;
		//g_Configs.bShowTerrain		= false;
		g_Configs.bShowRange		= false;
		g_Configs.bShowCloudBox		= false;
		g_Configs.bShowCloudLine	= false;
		g_Configs.bShowInstanceBox	= false;
		g_Configs.dwShowCopys		= 0;
		g_Configs.bShowDynamic		= false;
		g_Configs.bShowBufRegion	= false;
		g_Configs.bHDRLitmap		= false;
		g_Configs.bShowMotionPath	= false;
        g_Configs.bEnableSky        = false;
        g_Configs.bShowSunMoon      = false;
	}
	//m_pMap->GetSceneObjectMan()->ClearSelectedList();
	//if(m_pCurConfig->bGenTerrainLitmap != FALSE && emap.GetTerrain()->GetRender())
		emap.GetTerrain()->GetRender()->EnableLight(false);

	emap.ExpTopView(pParams->strLitmapCacheDir, 256, false);

	g_Configs = oldConfig;
	emap.Release();
	return TRUE;
}

BOOL CRayTraceDlg::GenLightMap(RTExportParamsDesc* pParams)
{
	DWORD dwExitCode;
	char buffer[300];

	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	memset(&si, 0, sizeof(si));
	memset(&pi, 0, sizeof(pi));
	
	sprintf(buffer, " -xml \"%s\" -preview 0 -start %d -end %d -autostart 1 -autoquit 1 -daynight %d -hdr %d -version %s -aostart %f -%s %f %s", 
		m_strXMLFile, m_pConfig->iStartObjIdx, m_pConfig->iEndObjIdx, m_pConfig->iDayNight, m_pConfig->bHDREncode, 
        RAYTRACE_TOOL_VERSION, m_pConfig->fAOStart, m_pConfig->bBlockRender ? "outcore" : "nooutcore", m_pConfig->fBlockSize,
        m_pConfig->bVertLitOnly ? "-vertlitonly" : "");
	
	GetStartupInfo(&si);

	g_Log.Log("CRayTraceDlg::GenLightMap()1, start CreateProcess !");
	BOOL bRet = CreateProcess(
		szToolPath,
		buffer, NULL, NULL, FALSE, 
		NULL, NULL, m_strToolPath, &si, &pi);
	
	//MessageBox(m_strToolPath + "\n" + m_pConfig->szToolPath);

	if(bRet == FALSE)
	{
		MessageBox("启动光线追踪计算工具失败,请检查工具路径是否正确!", "错误", MB_OK);
		return FALSE;
	}
	g_Log.Log("CRayTraceDlg::GenLightMap()1, finish CreateProcess !");
	
	if (m_pConfig->bReloadMap)
	{
		WaitForSingleObject(pi.hProcess, -1);
		GetExitCodeProcess(pi.hProcess, &dwExitCode);
	}
	g_Log.Log("CRayTraceDlg::GenLightMap()2, WaitForSingleObject finish !");
	CloseHandle(pi.hProcess);
	CloseHandle(pi.hThread);

	if (m_pConfig->bReloadMap)
	{
		// 成功退出进程
		if(pParams->pMap->IsEnableLitmap() == false)
		{
			pParams->pMap->EnableLitmap(true);
		}
        //MessageBox("Begin model");
		CDlgPreRadStep2::ReloadLitmap(pParams->pMap, true);
        //MessageBox("Begin terrain 0");
		CDlgPreRadStep2::ReloadTerrainLitmap(pParams->pMap, 0);
        //MessageBox("Begin terrain 1");
		CDlgPreRadStep2::ReloadTerrainLitmap(pParams->pMap, 1);
	}

	return TRUE;
}

int CRayTraceDlg::GetObjCount(CElementMap* pMap)
{
	CSceneObjectManager* pSceneObjectManager = pMap->GetSceneObjectMan();

	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
	return listModel->GetCount();
}

BOOL CRayTraceDlg::ExportBAT()
{
	CString strBAT;
				
	strBAT.Format("%s\\离线计算.bat", m_strToolPath);
	FILE* fp = fopen(strBAT, "w");
	if (fp == NULL)
	{
		MessageBox("无法创建批处理文件!", "错误", MB_OK);
		return FALSE;
	}

	CString strCommand;
	fprintf(fp, "@echo off\n");
	
	for (int i = 0; i < (int) m_lXmlFiles.size(); i++)
	{
		strCommand.Format("%s -xml \"%s\" -preview 0 -start %d -end %d -autostart 1 -autoquit 1 -daynight %d -hdr %d -version %s -aostart %g -%s %g %s\n", 
			m_strToolFile, m_lXmlFiles[i], m_pConfig->iStartObjIdx, m_pConfig->iEndObjIdx, m_pConfig->iDayNight, m_pConfig->bHDREncode, 
            RAYTRACE_TOOL_VERSION, m_pConfig->fAOStart, m_pConfig->bBlockRender ? "outcore" : "nooutcore", m_pConfig->fBlockSize, m_pConfig->bVertLitOnly ? "-vertlitonly" : "");
		fprintf(fp, strCommand);
	}
	fclose(fp);
	return TRUE;
}

AString CRayTraceDlg::CheckLightMapResources(CElementMap* pMap)
{
	CSceneObjectManager* pSceneObjectManager = pMap->GetSceneObjectMan();
	
	const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
	ALISTPOSITION pos = listModel->GetTailPosition();
	
	AString strLoss = "";
	
	std::vector<AString> lStored;

	// Output models
	while( pos )
	{
		CSceneObject* ptemp = listModel->GetPrev(pos);
		if( !ptemp->IsDeleted() )
		{
			// Lit this model
			CStaticModelObject * pStaticModel = (CStaticModelObject *) ptemp;
			if(pStaticModel->GetA3DModel()==NULL || pStaticModel->GetA3DModel()->pModel==NULL) continue;
			A3DLitModel * pModel = (pStaticModel->GetA3DModel()->pModel)->GetA3DLitModel();
			LIGHTDATA emissive;
			AString strEmissive;
			int nSamples;
			pStaticModel->GetEmissiveData(&emissive, &strEmissive, &nSamples);
			if (strEmissive != "" && !AUX_FileExist(strEmissive))
			{
				bool found = false;
				for (size_t i = 0; i < lStored.size(); i++)
				{
					if (lStored[i] == strEmissive)
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					strLoss += "\n";
					strLoss += strEmissive;
					lStored.push_back(strEmissive);
				}
			}
		}
	}
	return strLoss;
}

void CRayTraceDlg::OnBnClickedButtonCalcSize()
{

    if (m_pCurMap)
    {
        ComputeLightMapSize(m_pCurMap);
    }
}

void CRayTraceDlg::OnBnClickedButton2()
{
    CString strPassword;
    m_edtSizeLitmitPassword.GetWindowText(strPassword);
    m_edtSizeLimit.EnableWindow(strPassword == "IMBOSS");
}

void CRayTraceDlg::OnBnClickedCheckBlock()
{
    // TODO: Add your control notification handler code here
}

RAYTRACE_CONFIG* CRayTraceDlg::CreateDefaultConfig()
{
    RAYTRACE_CONFIG* pConfig = new RAYTRACE_CONFIG;
    memcpy(pConfig, &defaultConfig, sizeof(RAYTRACE_CONFIG));
    return pConfig;
}

bool CRayTraceDlg::LoadConfig(CELArchive& ar, RAYTRACE_CONFIG* pConfig)
{
    DWORD dwVersion;
    if (!ar.Read(&dwVersion, sizeof(dwVersion))) return false;
    if (dwVersion > RAYTRACE_CONFIG_VERSION)
    {
        g_Log.Log("CRayTraceDlg::LoadConfig, program version is old");
        return false;
    }
    DWORD dwLen;
    if (!ar.Read(&dwLen, sizeof(dwLen))) return false;
    if (dwLen > sizeof(RAYTRACE_CONFIG))
    {
        g_Log.Log("CRayTraceDlg::LoadConfig, structure length is too long");
        return false;
    }
    
    if (!ar.Read(pConfig, dwLen)) return false;

    // set default values
    pConfig->bBlockRender = true;
    pConfig->bReloadMap = true;
    pConfig->bGenTerrain = true;
    pConfig->bSelectOnly = false;
    pConfig->bVertLitOnly = false;
    pConfig->iEndObjIdx = -1;
    pConfig->iStartObjIdx = -1;
    pConfig->fMaxSize = defaultConfig.fMaxSize;
    pConfig->fMaxVolume = defaultConfig.fMaxVolume;
    return true;
}

bool CRayTraceDlg::SaveConfig(CELArchive& ar, const RAYTRACE_CONFIG* pConfig)
{
    DWORD dwVersion = RAYTRACE_CONFIG_VERSION;
    if (!ar.Write(&dwVersion, sizeof(dwVersion))) return false;
    DWORD dwLen = sizeof(RAYTRACE_CONFIG);
    if (!ar.Write(&dwLen, sizeof(dwLen))) return false;
    if (!ar.Write(pConfig, dwLen)) return false;
    return true;
}