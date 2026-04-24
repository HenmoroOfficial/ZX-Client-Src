// PreRadiosityStep2.cpp: implementation of the CPreRadiosityStep2 class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
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
#include "Shlwapi.h"
#include "LightMapTransition.h"
#include "A3DTerrain2Env.h"
#include "AF.h"
#include <A3DMirrorPlane.h>
#include "MirrorPlaneObject.h"

#define GAMMA_MIN	0.5f
#define GAMMA_MAX	1.5f

PRERADSTEP2_CONFIG CDlgPreRadStep2::s_Config;
const static char *c_szDir[] = {"Day", "Night"};

BEGIN_MESSAGE_MAP(CDlgPreRadStep2, CDialog)
	//{{AFX_MSG_MAP(CNpcSelDlg)
	ON_BN_CLICKED(IDC_CHECK1, OnCheck1)
	ON_BN_CLICKED(IDC_CHECK2, OnCheck2)
	ON_BN_CLICKED(IDC_CHK_SELONLY, OnCheck_SelOnly)
	ON_BN_CLICKED(IDC_DAY, OnRadioButton)
	ON_BN_CLICKED(IDC_NIGHT, OnRadioButton)
	ON_BN_CLICKED(IDC_DAYNIGHT, OnRadioButton)
	ON_EN_UPDATE(IDC_EDIT1, OnEdit1Change)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDlgPreRadStep2::CDlgPreRadStep2(CElementMap* pMap, const PRSTEP1_PARAM* pStep1Param, PRERADSTEP2_CONFIG* pConfig)
: CDialog(CDlgPreRadStep2::IDD)
, m_pMap(pMap)
, m_Step1Param(*pStep1Param)
, m_pCurConfig(pConfig == NULL ? &s_Config : pConfig)
//, m_nGammaValue(0)
{

}

CDlgPreRadStep2::~CDlgPreRadStep2()
{

}

BOOL CDlgPreRadStep2::OnInitDialog()
{
	CButton*pCheck1 = (CButton*)GetDlgItem(IDC_CHECK1);		// 计算地形光照图
	CButton*pCheck2 = (CButton*)GetDlgItem(IDC_CHECK2);		// 切换到光照图渲染
	CButton*pChkSelOnly = (CButton*)GetDlgItem(IDC_CHK_SELONLY);	// 只计算选择的对象
	CSliderCtrl* pGamma = (CSliderCtrl*)GetDlgItem(IDC_GAMMA);

	CEdit*	pEdit1	= (CEdit*)GetDlgItem(IDC_EDIT1);		// 迭代次数

	if(m_pCurConfig->bGenTerrainLitmap != FALSE)
		pCheck1->SetCheck(BST_CHECKED);

	if(m_pCurConfig->bSwitchLitmapRender != FALSE)
		pCheck2->SetCheck(BST_CHECKED);

	if(m_pCurConfig->bGenSelectOnly != FALSE)
		pChkSelOnly->SetCheck(BST_CHECKED);

	SetDlgItemInt(IDC_EDIT1, m_pCurConfig->nIterCount, FALSE);
	pGamma->SetRange(0, 100);
	pGamma->SetPos((int)((m_pCurConfig->fGamma - GAMMA_MIN) / (GAMMA_MAX - GAMMA_MIN) * 100.f + 0.5f));
	pGamma->SetTicFreq(10);

	char buffer[64];
	sprintf(buffer, "%.2f", m_pCurConfig->fGamma);
	SetDlgItemText(IDS_GAMMA, buffer);

	if(m_pCurConfig->bReadOnly != FALSE)
	{
		//pCheck1->EnableWindow(FALSE);
		pCheck2->EnableWindow(FALSE);
		pChkSelOnly->EnableWindow(FALSE);
	}

	CheckRadioButton(IDC_DAY, IDC_DAYNIGHT, m_pCurConfig->idDayNight);
	return TRUE;
}

// void CDlgPreRadStep2::DoDataExchange(CDataExchange* pDX)
// {
// 	CDialog::DoDataExchange(pDX);
// 	//{{AFX_DATA_MAP(CDlgPreRadStep2)
//  	DDX_Slider(pDX, IDC_GAMMA, m_nGammaValue);
// 	//}}AFX_DATA_MAP
// }

void CDlgPreRadStep2::OnCheck1()
{
	CButton*pCheck1 = (CButton*)GetDlgItem(IDC_CHECK1);		// 计算地形光照图
	m_pCurConfig->bGenTerrainLitmap = pCheck1->GetCheck() == BST_CHECKED;
}

void CDlgPreRadStep2::OnCheck2()
{
	CButton*pCheck2 = (CButton*)GetDlgItem(IDC_CHECK2);		// 切换到光照图渲染
	m_pCurConfig->bSwitchLitmapRender = pCheck2->GetCheck() == BST_CHECKED;
}

void CDlgPreRadStep2::OnRadioButton()
{
	int idSel = GetCheckedRadioButton(IDC_DAY, IDC_DAYNIGHT);
	if(idSel != 0)
		m_pCurConfig->idDayNight = idSel;
}

void CDlgPreRadStep2::OnCheck_SelOnly()
{
	CButton* pChkSelOnly = (CButton*)GetDlgItem(IDC_CHK_SELONLY);	// 只计算选择的对象
	m_pCurConfig->bGenSelectOnly = pChkSelOnly->GetCheck() == BST_CHECKED;
}

void CDlgPreRadStep2::OnEdit1Change()
{
	m_pCurConfig->nIterCount = GetDlgItemInt(IDC_EDIT1, NULL, FALSE);
	if(m_pCurConfig->nIterCount < 0 || m_pCurConfig->nIterCount > 32)
	{
		m_pCurConfig->nIterCount = 1;
		SetDlgItemInt(IDC_EDIT1, m_pCurConfig->nIterCount, FALSE);
	}
}

void CDlgPreRadStep2::OnOK()
{
	CSliderCtrl* pGamma = (CSliderCtrl*)GetDlgItem(IDC_GAMMA);
	//ASSERT(m_nGammaValue == pGamma->GetPos());
	int nGammaValue = pGamma->GetPos();
	m_pCurConfig->fGamma = (float)nGammaValue * 0.01f * (GAMMA_MAX - GAMMA_MIN) + GAMMA_MIN;

	if(m_pCurConfig->bGenSelectOnly != FALSE && m_pCurConfig->bGenTerrainLitmap == FALSE)
	{
		APtrList<CSceneObject *>* pSelList = m_pMap->GetSceneObjectMan()->GetSceneObjectSelectedList();
		ALISTPOSITION pos = pSelList->GetHeadPosition();
		while(pos)
		{
			CSceneObject* ptemp = pSelList->GetNext(pos);
			if(ptemp->GetObjectType() == CSceneObject::SO_TYPE_STATIC_MODEL)
				goto BEGIN_EXPORT_SCENE;
		}
		MessageBox("木有选择任何静态对象用来计算光照图!", "忘了吧?", MB_OK | MB_ICONINFORMATION);
		CDialog::OnOK();
		return;
	}

	// 每次都导出新的 Terrain Diffuse 和 XML 文件
	// LitModel 则根据标识选择是否导出
BEGIN_EXPORT_SCENE:
	if(ExportTerrainDiffuse() == FALSE)
	{
		MessageBox("导出地形失败!", "错误", MB_OK);
		return;
	}

	ExportHeightMap();
	if( (m_pCurConfig->idDayNight == IDC_DAY && ExportXML(0, TRUE) != FALSE) ||
		(m_pCurConfig->idDayNight == IDC_NIGHT && ExportXML(1, TRUE) != FALSE) ||
		(ExportXML(0, TRUE) != FALSE && ExportXML(1, FALSE) != FALSE) )
	{
		GenLitmap();
		CDialog::OnOK();
	}
	else
	{
		MessageBox("导出场景失败!", "错误", MB_OK);
	}
}

void CDlgPreRadStep2::OnCancel()
{
	CDialog::OnCancel();
}

void CDlgPreRadStep2::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	char buffer[64];
	float fValue = 0;
	if(nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
	{
		fValue = (float)nPos * 0.01f * (GAMMA_MAX - GAMMA_MIN) + GAMMA_MIN;
	}
	else if(nSBCode == SB_ENDSCROLL)
	{
		CSliderCtrl* pGamma = (CSliderCtrl*)GetDlgItem(IDC_GAMMA);
		fValue = pGamma->GetPos() * 0.01f * (GAMMA_MAX - GAMMA_MIN) + GAMMA_MIN;
	}
	else
		return;

	sprintf(buffer, "%.2f", fValue);
	SetDlgItemText(IDS_GAMMA, buffer);
}

BOOL CDlgPreRadStep2::ExportTerrainDiffuse()
{	// 导出地面Diffuse图

	CElementMap emap;
	const char* pszMapName = m_pMap->GetMapName();
	CString mapName;
	mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,pszMapName, pszMapName);

	emap.m_fUoff = m_pMap->m_fUoff;
	emap.m_fVoff = m_pMap->m_fVoff;
	emap.Load(mapName);

	bool bShow[29];
	bShow[0] = g_Configs.bShowStaticModel;
	bShow[1] = g_Configs.bShowLight;
	bShow[2] = g_Configs.bShowAudio;
	bShow[3] = g_Configs.bShowBoxArea;
	bShow[4] = g_Configs.bShowDummy;
	bShow[5] = g_Configs.bShowSpecially;
	bShow[6] = g_Configs.bShowWater;
	bShow[7] = g_Configs.bShowAIArea;
	bShow[8] = g_Configs.bShowPath;
	bShow[9] = g_Configs.bShowSkinModel;
	bShow[10] = g_Configs.bShowCritterGroup;
	bShow[11] = g_Configs.bShowFixedNpc;
	//bShow[12] = g_Configs.bShowTerrainBlock;
	//bShow[13] = g_Configs.bShowSmallBlock;
	bShow[14] = g_Configs.bShowRain;
	bShow[15] = g_Configs.bShowSnow;
	bShow[16] = g_Configs.bShowGrass;
	bShow[17] = g_Configs.bShowTree;
	bShow[18] = g_Configs.bShowGather;
	//bShow[19] = g_Configs.bShowTerrain;
	bShow[20] = g_Configs.bShowRange;
	bShow[21] = g_Configs.bShowCloudBox;
	bShow[22] = g_Configs.bShowCloudLine;
	bShow[23] = g_Configs.bShowInstanceBox;
	bShow[24] = g_Configs.dwShowCopys;
	bShow[25] = g_Configs.bShowDynamic;
	bShow[26] = g_Configs.bShowBufRegion;
	bShow[27] = g_Configs.bShowMotionPath;
	bShow[28] = g_Configs.bShowMirrorPlane;


	if(/*m_bIsExpTexture*/true)
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
		g_Configs.bShowMotionPath	= false;
		g_Configs.bShowMirrorPlane	= false;
	}
	//m_pMap->GetSceneObjectMan()->ClearSelectedList();
	if(m_pCurConfig->bGenTerrainLitmap != FALSE && emap.GetTerrain()->GetRender())
		emap.GetTerrain()->GetRender()->EnableLight(false);

	emap.ExpTopView("LitmapCache\\Day", 1024, false);
	emap.ExpTopView("LitmapCache\\Night", 1024, true);

	g_Configs.bShowStaticModel	= bShow[0];
	g_Configs.bShowLight		= bShow[1];
	g_Configs.bShowAudio		= bShow[2];
	g_Configs.bShowBoxArea		= bShow[3];
	g_Configs.bShowDummy		= bShow[4];
	g_Configs.bShowSpecially	= bShow[5];
	g_Configs.bShowWater		= bShow[6];
	g_Configs.bShowAIArea		= bShow[7];
	g_Configs.bShowPath			= bShow[8];
	g_Configs.bShowSkinModel	= bShow[9];
	g_Configs.bShowCritterGroup = bShow[10];
	g_Configs.bShowFixedNpc		= bShow[11];
	//g_Configs.bShowTerrainBlock = bShow[12];
	//g_Configs.bShowSmallBlock	= bShow[13];
	g_Configs.bShowRain			= bShow[14];
	g_Configs.bShowSnow			= bShow[15];
	g_Configs.bShowGrass		= bShow[16];
	g_Configs.bShowTree			= bShow[17];
	g_Configs.bShowGather		= bShow[18];
	//g_Configs.bShowTerrain		= bShow[19];
	g_Configs.bShowRange		= bShow[20];
	g_Configs.bShowCloudBox		= bShow[21];
	g_Configs.bShowCloudLine	= bShow[22];
	g_Configs.bShowInstanceBox	= bShow[23];
	g_Configs.dwShowCopys		= bShow[24];
	g_Configs.bShowDynamic		= bShow[25];
	g_Configs.bShowBufRegion	= bShow[26];
	g_Configs.bShowMotionPath	= bShow[27];
	g_Configs.bShowMirrorPlane	= bShow[28];

	emap.Release();
	return TRUE;
}
BOOL CDlgPreRadStep2::ExportHeightMap()
{
	const CString c_strLitmapCacheDir		= CString(g_szWorkDir) + "LitmapCache";
	const CString c_strTerrainHeightPath	= c_strLitmapCacheDir + "\\Terrain.raw";

	CTerrain::HEIGHTMAP* pHeightMap = m_pMap->GetTerrain()->GetHeightMap();
	if (!pHeightMap->pHeightData)
		return FALSE;

	FILE* fp = fopen(c_strTerrainHeightPath, "wb+");
	if (!fp)
	{
		g_Log.Log("CDlgPreRadStep2::ExportXML(), 输出地形高度图失败 !");
		return FALSE;
	}

	WORD* pLineBuf = new WORD [pHeightMap->iWidth];
	if (!pLineBuf)
	{
		fclose(fp);
		g_Log.Log("CDlgPreRadStep2::ExportXML(), Not enough memory !");
		return FALSE;
	}

	float* pSrc = pHeightMap->pHeightData;

	for (int i=0; i < pHeightMap->iHeight; i++)
	{
		WORD* pDst = pLineBuf;

		for (int j=0; j < pHeightMap->iWidth; j++, pDst++, pSrc++)
			*pDst = (WORD)(*pSrc * 65535);

		fwrite(pLineBuf, 1, pHeightMap->iWidth * sizeof (WORD), fp);
	}

	delete [] pLineBuf;
	fclose(fp);
	return TRUE;
}

BOOL CDlgPreRadStep2::ExportXML(int nDayNight, BOOL bExportLitModel)
{
	if(nDayNight != 0 && nDayNight != 1)
		return FALSE;
	const CString c_strLitmapCacheDir		= CString(g_szWorkDir) + "LitmapCache";
	const CString c_strTerrainHeightPath	= c_strLitmapCacheDir + "\\Terrain.raw";
	const CString c_strLitmapPath			= CString(g_szWorkDir) + g_szEditMapDir + "\\" + m_pMap->GetMapName() + "\\LitData";
	const static char *cPrefix[2] = {"D","N"};


	//const int	idRenderSect = 0;
	const float fBackfaceArea = 0.5f;
	const float fSunRadius = 50.0f;	// TODO: 靠计算得出
	const float fNear = 0.1f;
	const float fFar = 1000.0f;

	{	// 导出静态模型文件
		const LIGHTINGPARAMS& LtParams = nDayNight == 0 
			? m_pMap->GetLightingParams()
			: m_pMap->GetNightLightingParams();
		const int nTerrainBlockSize = 64;

		CString strXML;
		strXML.Format("%sLitmapCache\\%s\\LitmapScene.xml", g_szWorkDir, c_szDir[nDayNight]);
		FILE* fp = fopen(strXML, "wb");

		const A3DVECTOR3 vSunDir = -Normalize(LtParams.vSunDir);

		A3DCOLORVALUE crSun = LtParams.dwSunCol;

		// 计算太阳光的亮度
		float fStrength = (2.0f * (fFar - fNear) * (fFar - fNear)) / (fSunRadius * fSunRadius);
		crSun = crSun * fStrength * LtParams.fSunPower;

		fwprintf(fp, 
			L"%c<Scene SunDir=\"%f,%f,%f\" Sunshine=\"%.3f,%.3f,%.3f\" AtmoScat=\"%x\" SunRadius=\"%.3f\" Near=\"%f\" Far=\"%.3f\" ", 
			0xFEFF, vSunDir.x, vSunDir.y, vSunDir.z, crSun.r, crSun.g, crSun.b, LtParams.dwAmbient, fSunRadius, fNear, fFar);

		if(m_pCurConfig->nIterCount > 1)
			fwprintf(fp, L"IteratorCount=\"%d\" ", m_pCurConfig->nIterCount);

		if(m_pCurConfig->fGamma != 1.0f)
			fwprintf(fp, L"Gamma=\"%.3f\" ", m_pCurConfig->fGamma);

		fwprintf(fp, L">\r\n");

		CTerrain* pTerrain = m_pMap->GetTerrain();
		CTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();

		int k = static_cast<int>(atof(m_pMap->GetMapName()));


		// 地形数据

		fwprintf(fp, 
			L"\t<Terrain Raw=\"%s\"\r\n\tMinHeight=\"%.3f\" MaxHeight=\"%.3f\" Width=\"%d\" Height=\"%d\"\r\n\t\t",
			AS2WC((const char*)c_strTerrainHeightPath), 
			pTerrain->GetYOffset(),
			pTerrain->GetMaxHeight(), pHeightMap->iWidth, pHeightMap->iHeight);

		fwprintf(fp, L"BlockWidth=\"%d\" BlockHeight=\"%d\"\r\n\t\t",
			nTerrainBlockSize, nTerrainBlockSize);

		int nNumWidth  = pHeightMap->iWidth  / nTerrainBlockSize;
		int nNumHeight = pHeightMap->iHeight / nTerrainBlockSize;
		int nTerrainLitmapSize = LtParams.iMapSize;

		if(m_Step1Param.emLitmapQuality == PRSTEP1_PARAM::QuadLitmap)
		{
			nTerrainLitmapSize = (nTerrainLitmapSize >> 1);
		}

		fwprintf(fp, L"LitmapWidth=\"%d\" LitmapHeight=\"%d\" ",
			nTerrainLitmapSize / nNumWidth, nTerrainLitmapSize / nNumHeight);

		if(m_pCurConfig->bGenTerrainLitmap == FALSE)
		{
			fwprintf(fp, L"GenLightMap=\"0\"\r\n\t\t");
		}

		static char *pPath[] ={
			"\\Day\\0",
			"\\Day\\",
			"\\Night\\0",
			"\\Night\\",
		};

		static char *pTerrainName[] = {"\\TerrainD.bmp", "\\TerrainN.bmp"};

		fwprintf(fp, 
			L"TileSize=\"%.3f\" Origin=\"%d,%d\"\r\n\tDiffuse=\"%s\"/>\r\n",
			pTerrain->GetTileSize(),
			pHeightMap->iWidth >> 1, pHeightMap->iHeight >> 1,
			AS2WC((const char*)(c_strLitmapCacheDir + ((k>=0 && k < 10) ? pPath[nDayNight * 2] : pPath[nDayNight * 2 + 1]) + m_pMap->GetMapName() + ".bmp"))
			);

		if(m_pCurConfig->bGenTerrainLitmap != FALSE)
		{	
			// 地形Output数据
			DeleteFile((const char*)(c_strLitmapPath + pTerrainName[0]));
			DeleteFile((const char*)(c_strLitmapPath + pTerrainName[1]));
			fwprintf(fp, 
				L"\t<TerrainOutput FilePath=\"%s\"\r\n\t\tSize=\"%d,%d\" FileFormat=\"BMP\" PixelFormat=\"RGB888\"/>\r\n",
				AS2WC((const char*)(c_strLitmapPath + pTerrainName[nDayNight])),
				LtParams.iMapSize, LtParams.iMapSize);
		}

		CSceneObjectManager* pSceneObjectManager = m_pMap->GetSceneObjectMan();

		const APtrList<CSceneObject *>* listModel = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
		const APtrList<CSceneObject *>* listLight = pSceneObjectManager->GetSortObjectList(CSceneObject::SO_TYPE_LIGHT);
		//const APtrList<CSceneObject *>* pSelList = &pSceneObjectManager->m_listSelectedObject;
		ALISTPOSITION pos = listModel->GetTailPosition();

		CString strName;
		CString strLitName;
		int		idRenderSect = 0;

		while( pos )
		{
			CSceneObject* ptemp = listModel->GetPrev(pos);
			if( !ptemp->IsDeleted() )
			{
				// Lit this model
				CStaticModelObject * pStaticModel = (CStaticModelObject *) ptemp;
				A3DLitModel * pModel = (pStaticModel->GetA3DModel()->pModel)->GetA3DLitModel();

				// 如果只计算选择对象, 则把没有选择的对象分到其他的渲染组去
				if(m_pCurConfig->bGenSelectOnly != FALSE)
				{
					idRenderSect = ptemp->IsSelected() != NULL ? 0 : 1;
				}

				strName.Format("%05d.Lit20", pStaticModel->GetObjectID());
				strLitName.Format("%s\\%05d_%sRender", c_strLitmapPath, 
					pStaticModel->GetObjectID(), m_pMap->GetMapName());

				WIN32_FIND_DATA FindFileData;
				HANDLE hFind = FindFirstFile(strName, &FindFileData);
				if(hFind != INVALID_HANDLE_VALUE)
				{
					ASSERT(0);
					FindClose(hFind);
				}

				int nlms = pStaticModel->GetCustomLightmapSize();
				if(nlms == -1) nlms = 256;
				if(m_Step1Param.emLitmapQuality == PRSTEP1_PARAM::QuadLitmap && nlms >= 64)
					nlms = (nlms >> 1);

				fwprintf(fp, L"\t<LitModel20 File=\"%s\"", AS2WC((const char*)(c_strLitmapCacheDir + "\\" + strName)));
				fwprintf(fp, L"\r\n\t\tLMWidth=\"%d\" LMHeight=\"%d\" AlphaRef=\"%d\" BackfaceArea=\"%.2f\" GenSectId=\"%d\"",
					nlms, nlms, 84, fBackfaceArea, idRenderSect);

				if(m_Step1Param.emLitmapQuality == PRSTEP1_PARAM::VertLit)
				{
					fwprintf(fp, L" UseLitmap=\"FALSE\" ");
				}

				fwprintf(fp, L"\r\n\t\tLightMap=\"%s\"", AS2WC((const char*)(strLitName + cPrefix[nDayNight])));
				fwprintf(fp, L"/>\r\n");

				strName = "LitmapCache\\" + strName;
				if(bExportLitModel == TRUE && (m_Step1Param.bExportModel != FALSE || PathFileExists(strName) == FALSE))
				{
					pModel->Save(strName);
				}
			}
		}

		pos = listLight->GetTailPosition();
		while(pos)
		{
			CSceneObject* ptemp = listModel->GetPrev(pos);

			if( !ptemp->IsDeleted() )
			{
				float		fRadius;
				LIGHTDATA	LightData;
				A3DVECTOR3	vPos;

				CSceneLightObject* pLight = (CSceneLightObject*)ptemp;
				pLight->GetLightData(LightData);

				if(LightData.m_nEffectTypeRange == 1)
					continue;
				if(LightData.m_nEffectType != 2)
				{
					if( LightData.m_nEffectType != nDayNight )
						continue;
				}

				vPos = pLight->GetPos();

				//A3DCOLORVALUE crLight = LightData.m_dwColor;

				// 将光照衰减参数转换为真实光照的发光球,求出其球体半径
				// 根据公式可以推导,只有当常量和一次衰减参数为0,二次衰减大于0时,
				// 两种光照方式计算的光照最为接近.所以对于某些不太靠谱的衰减参数,
				// 计算函数将返回-1.0f,认为该光源无效.
				fRadius = ComputeLightRadius(LightData);
				if(fRadius < 0.0f)
					continue;

				fwprintf(fp, L"\t<SphereLight Pos=\"%.3f,%.3f,%.3f\" Radius=\"%f\" Luminance=\"%f\" Color=\"%lx\"/>\r\n",
					vPos.x, vPos.y, vPos.z,
					fRadius, LightData.m_fColorScale, LightData.m_dwColor);


			}
		}
		fputws(L"</Scene>\r\n", fp);
		fclose(fp);
	}
	return TRUE;
}

float CDlgPreRadStep2::ComputeLightRadius(const LIGHTDATA& LightData)
{
	float fRadius = -1.0f;
	if(LightData.m_fAttenuation2 > 0.0f)
	{
		const float fSqrtIt = sqrt( 4 * (1.0f - LightData.m_fAttenuation0) * LightData.m_fAttenuation2 + LightData.m_fAttenuation1 * LightData.m_fAttenuation1);
		fRadius = fSqrtIt - LightData.m_fAttenuation1;
		if(fRadius < 0.0f)
			fRadius = -fSqrtIt - LightData.m_fAttenuation1;
		fRadius = fRadius / (2.0f * LightData.m_fAttenuation2);
	}
	else if(LightData.m_fAttenuation1 > 0.0f)
	{
		fRadius = (1 - LightData.m_fAttenuation0) / LightData.m_fAttenuation1;
	}
	return fRadius;
}

VOID CDlgPreRadStep2::GenLitmap()
{
	DWORD dwExitCode;
	char buffer[512];

	for(int i = 0; i < 2; i++)
	{
		if( (i == 0 && m_pCurConfig->idDayNight == IDC_DAY) ||
			(i == 1 && m_pCurConfig->idDayNight == IDC_NIGHT) ||
			m_pCurConfig->idDayNight == IDC_DAYNIGHT )
		{

			PROCESS_INFORMATION pi;
			STARTUPINFO si;
			memset(&si, 0, sizeof(si));
			memset(&pi, 0, sizeof(pi));

			wsprintf(buffer, " -xml:LitmapCache\\%s\\LitmapScene.xml -ui:0", c_szDir[i]);

			GetStartupInfo(&si);

			BOOL bRet = CreateProcess(
#ifdef _DEBUG
				"Radiosity_d.exe", 
#else
				"Radiosity.exe", 
#endif
				buffer, NULL, NULL, FALSE, 
				NULL, NULL, g_szWorkDir, &si, &pi);

			if(bRet == FALSE)
			{
				MessageBox("启动辐射度计算工具失败,请检查Bin目录下是否有Radiosity.exe文件!", "错误", MB_OK);
				return;
			}

			WaitForSingleObject(pi.hProcess, -1);
			GetExitCodeProcess(pi.hProcess, &dwExitCode);
			CloseHandle(pi.hProcess);
			CloseHandle(pi.hThread);

			ReloadTerrainLitmap(m_pMap, i);
		}
	}

	// 成功退出进程
	if(m_pMap->IsEnableLitmap() == false && m_pCurConfig->bSwitchLitmapRender != FALSE)
	{
		m_pMap->EnableLitmap(true);
	}

	ReloadLitmap(m_pMap, false);
}

void LoadMirrorPlaneVertLitDataFromFile(CMirrorPlaneObject* pMirrorPlane, const char* szVLitDataFormat, int nDayNight)
{
	AString		strLitDataFile;
	char		buffer[1024];
	A3DCOLORVALUE crVert;

	crVert.a = 1.0f;

	strLitDataFile.Format(szVLitDataFormat, 0);
	FILE* fp = fopen(strLitDataFile, "rt");
	if(fp != NULL)
	{
		fgets(buffer, 1024, fp);
		const unsigned int nNumVertices = atoi(buffer);
		if( nNumVertices > 0 )
		{
			AArray<A3DCOLOR> ArrayColors;
			ArrayColors.SetSize( nNumVertices, 0 );
			A3DCOLOR* pColors = ArrayColors.GetData();

			for(int n = 0; n < nNumVertices; n++)
			{
				fgets(buffer, 1024, fp);
				sscanf(buffer, "VD %f,%f,%f", &crVert.r, &crVert.g, &crVert.b);
				pColors[n] = crVert.ToRGBAColor();
			}

			if( nDayNight == 0 )
			{
				pMirrorPlane->SetVertexColorDay( pColors, nNumVertices );
			}
			else
			{
				pMirrorPlane->SetVertexColorNight( pColors, nNumVertices );
			}

			pMirrorPlane->UpdateStream();
		}

		fclose(fp);
	}
}

void CDlgPreRadStep2::ReloadLitmap(CElementMap* pMap, bool bLoadVertLight)
{
	const AString strLitmapPath = AString(g_szWorkDir) + g_szEditMapDir + "\\" + pMap->GetMapName() + "\\LitData";
	//static bool bUseJPG = false;
	//bUseJPG = !bUseJPG;

	//if (bUseJPG)
	//    OutputDebugStringA("JPG!\n");
	//else
	//    OutputDebugStringA("DDS!\n");

	DWORD dwTime = GetTickCount();
	// Loads light maps for static models.
	APtrList<CSceneObject *>* listModel = pMap->GetSceneObjectMan()->GetSortObjectList(CSceneObject::SO_TYPE_STATIC_MODEL);
	ALISTPOSITION pos = listModel->GetTailPosition();
	while( pos )
	{
		CSceneObject* ptemp = listModel->GetPrev(pos);
		if( !ptemp->IsDeleted() )
		{
			// Lit this model
			CStaticModelObject * pStaticModel = (CStaticModelObject *) ptemp;
			PESTATICMODEL pPEStaticModel = pStaticModel->GetA3DModel();
			if(pPEStaticModel == NULL || pPEStaticModel->pModel == NULL)
				continue;

			int nlms = pStaticModel->GetCustomLightmapSize();
			if (nlms == -1)
			{
				//nlms = m_pConfig->iDefautSize;
				nlms = pStaticModel->GetAdaptLightmapSize(STATIC_OBJ_LITMAP_SIZE_FACTOR);

			}
			A3DLitModel* pModel = pPEStaticModel->pModel->GetA3DLitModel();
			if (pModel == NULL)
				continue;

			if (nlms < 64)
			{
				for (int iMesh = 0; iMesh < pModel->GetNumMeshes(); iMesh++)
				{
					A3DLitMesh* pMesh = pModel->GetMesh(iMesh);
					pMesh->SetUseLightMap(false);
				}
			}
			else
			{
				for (int iMesh = 0; iMesh < pModel->GetNumMeshes(); iMesh++)
				{
					A3DLitMesh* pMesh = pModel->GetMesh(iMesh);
					if (pMesh->GetLightMapCoords())
						pMesh->SetUseLightMap(true);
				}
			}

			CString strLitNameD;
			CString strLitNameN;
			CString strLitData;

			strLitNameD.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderD_LM_X.%s", strLitmapPath, 
				pStaticModel->GetObjectID(), pMap->GetMapName(), "DDS");
			strLitNameN.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderN_LM_X.%s", strLitmapPath, 
				pStaticModel->GetObjectID(), pMap->GetMapName(), "DDS");

			if (bLoadVertLight)
			{
				strLitData.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderD_LM_S0_M%%d_VD.txt", 
					strLitmapPath, pStaticModel->GetObjectID(), pMap->GetMapName());
				LoadVertLitDataFromFile(pModel, strLitData, 0);

				strLitData.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderN_LM_S0_M%%d_VD.txt", 
					strLitmapPath, pStaticModel->GetObjectID(), pMap->GetMapName());
				LoadVertLitDataFromFile(pModel, strLitData, 1);
			}

			if (!af_IsFileExist(strLitNameD))
				strLitNameD = "";
			if (!af_IsFileExist(strLitNameN))
				strLitNameN = "";

			pModel->SetLightMap(strLitNameD);
			pModel->SetNightLightMap(strLitNameN);
		}
	}


	// Loads light maps for mirror planes.
	listModel = pMap->GetSceneObjectMan()->GetSortObjectList(CSceneObject::SO_TYPE_MIRROR_PLANE);
	pos = listModel->GetTailPosition();
	while( pos )
	{
		CSceneObject* ptemp = listModel->GetPrev(pos);
		if( !ptemp->IsDeleted() )
		{
			CMirrorPlaneObject* pMirrorPlane = (CMirrorPlaneObject*)ptemp;
			A3DMirrorPlane* pA3DMirrorPlane = pMirrorPlane->GetMirrorPlane();

			const int nObjectID = pMirrorPlane->GetObjectID();

			CString strLitNameD;
			CString strLitNameN;
			CString strLitData;
			strLitNameD.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderD_LM_X.DDS", strLitmapPath, 
				nObjectID, pMap->GetMapName());
			strLitNameN.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderN_LM_X.DDS", strLitmapPath, 
				nObjectID, pMap->GetMapName());

			if( pA3DMirrorPlane->GetMode() == A3DMirrorPlane::MODE_VERTEX_COLOR && bLoadVertLight )
			{
				strLitData.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderD_LM_S0_M%%d_VD.txt", 
					strLitmapPath, nObjectID, pMap->GetMapName());
				LoadMirrorPlaneVertLitDataFromFile(pMirrorPlane, strLitData, 0);

				strLitData.Format("%s\\"LIGHT_MAP_VERSION"%05d_%sRenderN_LM_S0_M%%d_VD.txt", 
					strLitmapPath, nObjectID, pMap->GetMapName());
				LoadMirrorPlaneVertLitDataFromFile(pMirrorPlane, strLitData, 1);
			}

			if (!af_IsFileExist(strLitNameD))
				strLitNameD = "";
			if (!af_IsFileExist(strLitNameN))
				strLitNameN = "";

			pMirrorPlane->SetDayLightMap(strLitNameD);
			pMirrorPlane->SetNightLightMap(strLitNameN);
		}
	}

	dwTime = GetTickCount() - dwTime;
	AString strTime;
	strTime.Format("time = %d ms\n", dwTime);
	OutputDebugStringA(strTime);
}

void CDlgPreRadStep2::ReloadTerrainLitmap(CElementMap* pMap)
{
	ReloadTerrainLitmap(pMap, 0);
	ReloadTerrainLitmap(pMap, 1);
}

void CDlgPreRadStep2::ReloadTerrainLitmap(CElementMap* pMap, int nDayNight)
{
	const CString c_strLitmapPath			= CString(g_szWorkDir) + g_szEditMapDir + "\\" + pMap->GetMapName() + "\\LitData";
	const CString c_strTerrainLitmapPath[2]	= {
		CString(g_szWorkDir) + g_szEditMapDir + "\\" + pMap->GetMapName() + "\\" + pMap->GetMapName() + "Render.t2lm",
		CString(g_szWorkDir) + g_szEditMapDir + "\\" + pMap->GetMapName() + "\\" + pMap->GetMapName() + "Render.t2lm1"};

		TRN2FILEIDVER IdVer;
		T2LMFILEHEADER2	LMFileHeader;

		const static char* pTerrainName[] = {"\\TerrainD.bmp", "\\TerrainN.bmp"};

		//	for(int i = 0; i < 2; i++)
		//	{
		CELBitmap* pBitmap = new CELBitmap;
		pBitmap->LoadAs32BitBitmap(c_strLitmapPath + pTerrainName[nDayNight], 0, 0);
		if(pBitmap->GetBitmap() == NULL)
		{
			A3DRELEASE(pBitmap);
			return;
		}

		int nNumOfRead;
		FILE* fp = fopen(c_strTerrainLitmapPath[nDayNight], "rb+");

		if(fp == NULL)
		{
			A3DRELEASE(pBitmap);
			return;
		}

		nNumOfRead = fread(&IdVer, 1, sizeof (IdVer), fp);
		if(	nNumOfRead != sizeof(IdVer) || 
			IdVer.dwIdentify != T2LMFILE_IDENTIFY || IdVer.dwVersion != T2LMFILE_VERSION)
		{
			fclose(fp);
			A3DRELEASE(pBitmap);
			return;
		}

		// 	//	Write file header to hold place
		// 	memset(&LMFileHeader, 0, sizeof (LMFileHeader));
		// 	LMFileHeader.iNumMaskArea	= TrnInfo.iNumMaskArea;
		// 	LMFileHeader.iMaskLMSize	= LtParams.iMapSize / (m_TrnInfo.iSubTrnGrid / m_TrnInfo.iMaskGrid);
		// 	LMFileHeader.iLMDataSize	= m_LMFileHeader.iMaskLMSize * m_LMFileHeader.iMaskLMSize * 3;

		nNumOfRead = fread(&LMFileHeader, 1, sizeof(LMFileHeader), fp);
		if(nNumOfRead != sizeof(LMFileHeader))
		{
			fclose(fp);
			A3DRELEASE(pBitmap);
			return;
		}

		int nMAStride = (int)(sqrt((double)LMFileHeader.iNumMaskArea) + 0.5f);
		CELBitmap::LOCKINFO lockinfo;
		pBitmap->LockRect(ARectI(0, 0, pBitmap->GetWidth(), pBitmap->GetHeight()), &lockinfo);
		BYTE* pBuffer = new BYTE[LMFileHeader.iLMDataSize];

		DWORD* aMaskOffset = new DWORD[LMFileHeader.iNumMaskArea];
		if(nMAStride * LMFileHeader.iMaskLMSize != pBitmap->GetWidth())
		{
			LMFileHeader.iMaskLMSize = pBitmap->GetWidth() / nMAStride;
			LMFileHeader.iLMDataSize = LMFileHeader.iMaskLMSize * LMFileHeader.iMaskLMSize * 3;

			fseek(fp, sizeof(TRN2FILEIDVER), 0);
			fwrite(&LMFileHeader, sizeof(LMFileHeader), 1, fp);

			delete pBuffer;
			pBuffer = new BYTE[LMFileHeader.iLMDataSize];
		}

		fseek(fp, sizeof(TRN2FILEIDVER) + sizeof(T2LMFILEHEADER2) + sizeof(DWORD) * LMFileHeader.iNumMaskArea, 0);

		for(int iMask = 0; iMask < LMFileHeader.iNumMaskArea; iMask++)
		{
			int n = 0;
			int x = (iMask % nMAStride) * LMFileHeader.iMaskLMSize;
			int y = (iMask / nMAStride) * LMFileHeader.iMaskLMSize;
			BYTE* pSrc = &lockinfo.pData[y * lockinfo.iPitch + x * sizeof(DWORD)];

			for(int i = 0; i < LMFileHeader.iMaskLMSize; i++)
			{
				for(int j = 0; j < LMFileHeader.iMaskLMSize; j++)
				{
					pBuffer[n++] = *pSrc++;
					pBuffer[n++] = *pSrc++;
					pBuffer[n++] = *pSrc++;
					pSrc++;
				}
				pSrc += (lockinfo.iPitch - LMFileHeader.iMaskLMSize * sizeof(DWORD));
			}

			aMaskOffset[iMask] = ftell(fp);
			fwrite(pBuffer, LMFileHeader.iLMDataSize, 1, fp);
		}

		fseek(fp, sizeof(TRN2FILEIDVER) + sizeof(T2LMFILEHEADER2), 0);
		fwrite(aMaskOffset, sizeof(DWORD) * LMFileHeader.iNumMaskArea, 1, fp);
		delete aMaskOffset;

		fclose(fp);
		A3DRELEASE(pBitmap);
		delete pBuffer;
		pBuffer = NULL;


		// 处理光照过渡
		bool const bNight = (nDayNight == 1);
		CLightMapTransition trans;
		CELBitmap *pCurBitmap = trans.LoadLightMap(1, pMap->GetMapName(), bNight);
		trans.WriteLightMap(pCurBitmap, pMap->GetMapName(),bNight);
		if(pCurBitmap) 
		{
			pCurBitmap->Release();
			delete pCurBitmap;
			pCurBitmap = 0;
		}

		if(nDayNight == 0)
			CopyFile(c_strTerrainLitmapPath[0], c_strTerrainLitmapPath[0] + "1", FALSE);

		//		pCurBitmap = trans.LoadLightMap(1, pMap->GetMapName(), true);
		//		trans.WriteLightMap(pCurBitmap, pMap->GetMapName(), true);
		//		if(pCurBitmap) 
		//		{
		//			pCurBitmap->Release();
		//			delete pCurBitmap;
		//		}
		//	}

		pMap->GetTerrain()->ReloadRender(false);
}