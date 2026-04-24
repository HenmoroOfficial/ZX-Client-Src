// ExportLitDlg.cpp : implementation file
//

#include "global.h"
#include "elementeditor.h"
#include "ExportLitDlg.h"
#include "SceneObjectManager.h"
#include "A3D.h"

//#define new A_DEBUG_NEW

bool IsLightSimiliar(const SCENELIGHT& light1, const SCENELIGHT& light2)
{
	if( light1.param.Type != light2.param.Type )
		return false;

	if( light1.t != light2.t )
		return false;

	if( a3d_Magnitude(light1.param.Position - light2.param.Position) > 3.0f )
		return false;

	A3DHSVCOLORVALUE d1, d2;
	a3d_RGBToHSV(a3d_ColorValueToColorRGBA(light1.param.Diffuse), &d1);
	a3d_RGBToHSV(a3d_ColorValueToColorRGBA(light2.param.Diffuse), &d2);
	if( fabs(d1.h - d2.h) > 0.05f )
		return false;

	A3DHSVCOLORVALUE a1, a2;
	a3d_RGBToHSV(a3d_ColorValueToColorRGBA(light1.param.Ambient), &a1);
	a3d_RGBToHSV(a3d_ColorValueToColorRGBA(light2.param.Ambient), &a2);
	if( fabs(a1.h - a2.h) > 0.05f )
		return false;

	return true;
}

int GetSimiliarLights(SCENELIGHT * pLights, int nLights, int idLightStart, const SCENELIGHT& src)
{
	for(int i=idLightStart; i<nLights; i++)
	{
		if( IsLightSimiliar(pLights[i], src) )
			return i;
	}

	return -1;
}

SCENELIGHT MergeLight(const SCENELIGHT& light1, const SCENELIGHT& light2)
{
	SCENELIGHT light = light1;
	
	light.param.Position = (light1.param.Position + light2.param.Position) * 0.5f;
	light.param.Diffuse = (light1.param.Diffuse + light2.param.Diffuse) * 0.5f;
	light.param.Diffuse.a	= 1.0f;
	light.param.Ambient = (light1.param.Ambient + light2.param.Ambient) * 0.5f;
	light.param.Ambient.a	= 1.0f;
	light.param.Falloff = (light1.param.Range + light2.param.Range) * 0.5f;

	return light;
}

/////////////////////////////////////////////////////////////////////////////
// CExportLitDlg dialog


CExportLitDlg::CExportLitDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExportLitDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExportLitDlg)
	m_strExpPath = _T("");
	//}}AFX_DATA_INIT
	m_pProgress = 0;
}


void CExportLitDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExportLitDlg)
	DDX_Control(pDX, IDC_PROGRESS_PROCESS, m_Progress);
	DDX_Text(pDX, IDC_EDIT_EXPORT_PATH, m_strExpPath);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CExportLitDlg, CDialog)
	//{{AFX_MSG_MAP(CExportLitDlg)
	ON_BN_CLICKED(IDC_BUTTON_BROWSE, OnButtonBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExportLitDlg message handlers

void CExportLitDlg::OnButtonBrowse() 
{
	// TODO: Add your control notification handler code here
	static char BASED_CODE szFilter[] = "Light list Files (*.dat)|*.dat";
	CFileDialog dlg(
		FALSE,
		NULL,
		NULL,
		OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR,szFilter);

	if (dlg.DoModal() == IDOK)
	{
		m_strExpPath = dlg.GetPathName();
		if(m_strExpPath.Find(".dat")==-1)
		{
			m_strExpPath +=".dat";
		}
		UpdateData(false);
	}
}

void  CExportLitDlg::OnOK()
{
	
	UpdateData(true);
	ExpLitData();
	CDialog::OnOK();
}

bool CExportLitDlg::ExpLitData()
{
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
		return false;
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
	if(m_pProgress==0)
	{
		m_Progress.SetRange(0,projectList.size());
	    m_Progress.SetStep(1);
	}else
	{
		m_pProgress->SetRange(0,projectList.size());
		m_pProgress->SetStep(1);
	}

	int iNumCol = dwCol;
	int iNumRow = projectList.size()/dwCol;
	
	int nLights = 0;
	SCENELIGHT *pLights = new SCENELIGHT[20000];
	ASSERT(pLights);
	A3DAABB aabb;
	// Proessing map
	int nProj = 0;
	abase::vector<CString>::iterator theIterator;
	for(theIterator = projectList.begin(); theIterator != projectList.end(); theIterator ++)
	{
		
		mapName.Format("%s%s\\%s\\%s.elproj",g_szWorkDir,g_szEditMapDir,(*theIterator),(*theIterator));
		if(!emap.Load(mapName))
		{
			CString error_msg;
			error_msg.Format("不能打开地图文件 :%s",mapName);
			MessageBox(error_msg);
			return false;
		}
		CTerrain *pExpTerrain = emap.GetTerrain();
		float m_fProjTrnSize	= pExpTerrain->GetTerrainSize();
		float m_fWorldWid		= m_fProjTrnSize * iNumCol;
		float m_fWorldLen		= m_fProjTrnSize * iNumRow;
		int m_iProjColCnt = nProj%iNumCol;
		int m_iProjRowCnt = nProj/iNumCol;
		float fSceneOffX = -m_fWorldWid * 0.5f + m_iProjColCnt * m_fProjTrnSize + m_fProjTrnSize * 0.5f;
		float fSceneOffZ = m_fWorldLen * 0.5f - m_iProjRowCnt * m_fProjTrnSize - m_fProjTrnSize * 0.5f;
		CSceneObjectManager *pMan =emap.GetSceneObjectMan();
		ALISTPOSITION pos = pMan->m_listSceneObject.GetTailPosition();
		while( pos )
		{
			CSceneObject* ptemp = pMan->m_listSceneObject.GetPrev(pos);
			int type = ptemp->GetObjectType();
			if(type==CSceneObject::SO_TYPE_LIGHT)
			{
				CSceneLightObject *pObj = (CSceneLightObject*)ptemp;
				LIGHTDATA dat;
				pObj->GetLightData(dat);
				A3DVECTOR3 vpos = pObj->GetPos();
				vpos.x += fSceneOffX;
				vpos.z += fSceneOffZ;
				
				SCENELIGHT light = ExpLitData(dat,vpos);
				if( light.t != 0 && light.param.Type == A3DLIGHT_POINT )
				{
					pLights[nLights ++] = light;
					
					A3DAABB subAABB;
					subAABB.Center = light.param.Position;
					subAABB.Extents = A3DVECTOR3(light.param.Range);
					subAABB.CompleteMinsMaxs();
					aabb.Merge(subAABB);
				}
			}
		}
		emap.Release();
		if(m_pProgress==0)
			m_Progress.StepIt();
		else m_pProgress->StepIt();
	    ++nProj;
	}

	//合并后输出
	aabb.CompleteCenterExts();
	
	int	nLightsInGrid = 0;
	SCENELIGHT * pLightsInGrid = new SCENELIGHT[nLights];
	int i;
	for(i=0; i<nLights; i++)
	{
		if( pLights[i].param.Type != A3DLIGHT_POINT )
			continue;
		
		SCENELIGHT light = pLights[i];
		int idSimiliar = i;
		idSimiliar = GetSimiliarLights(pLights, nLights, idSimiliar + 1, light);
		while(idSimiliar != -1)
		{
			light = MergeLight(light, pLights[idSimiliar]);
			pLights[idSimiliar].param.Type = A3DLIGHT_FORCE_DWORD;
			//printf("  merge light [%d] to [%d]\n", idSimiliar, i);
			idSimiliar = GetSimiliarLights(pLights, nLights, idSimiliar + 1, light);
		}
		pLightsInGrid[nLightsInGrid ++] = light;
	}
	
	//printf("done, total [%d] lights after merge!\n", nLightsInGrid);
	
	FILE * fpLightsGrid = fopen(m_strExpPath, "wb");
	if( NULL == fpLightsGrid )
	{
		delete [] pLightsInGrid;
		delete [] pLights;
		g_Log.Log("can't create %s\n", m_strExpPath);
		return false;
	}
	
	DWORD dwVersion = 0x80000001;
#ifdef _ANGELICA22
	dwVersion = 0x80000002;
#endif 
	fwrite(&dwVersion, sizeof(DWORD), 1, fpLightsGrid);
	fwrite(&nLightsInGrid, sizeof(int), 1, fpLightsGrid);
	
	for(i=0; i<nLightsInGrid; i++)
	{
		SCENELIGHT light = pLightsInGrid[i];
		A3DAABB subAABB;
		subAABB.Center = light.param.Position;
		subAABB.Extents = A3DVECTOR3(light.param.Range);
		subAABB.CompleteMinsMaxs();
		
		light.param.Diffuse = (light.param.Diffuse + light.param.Ambient) * 1.0f;
		light.param.Ambient = light.param.Diffuse * 0.2f;
		
		fwrite(&light.param.Position, sizeof(A3DVECTOR3), 1, fpLightsGrid);
		fwrite(&light.param.Range, sizeof(float), 1, fpLightsGrid);
		fwrite(&light.param.Attenuation0, sizeof(float), 1, fpLightsGrid);
		fwrite(&light.param.Attenuation1, sizeof(float), 1, fpLightsGrid);
		fwrite(&light.param.Attenuation2, sizeof(float), 1, fpLightsGrid);
		fwrite(&light.param.Diffuse, sizeof(A3DCOLORVALUE), 1, fpLightsGrid);
		fwrite(&light.param.Ambient, sizeof(A3DCOLORVALUE), 1, fpLightsGrid);
		fwrite(&subAABB, sizeof(A3DAABB), 1, fpLightsGrid);
		
		DWORD flags = 0;
		if( light.t == 1 )
			flags = 1;
		fwrite(&flags, sizeof(DWORD), 1, fpLightsGrid);
	}
	
	fclose(fpLightsGrid);
	
	delete [] pLightsInGrid;
	delete [] pLights;
	
	
	return true;
}

SCENELIGHT CExportLitDlg::ExpLitData(LIGHTDATA dat,A3DVECTOR3 pos)
{
	SCENELIGHT light;
	light.t = dat.m_nEffectType;
	
	A3DLIGHTPARAM lit;
	ZeroMemory(&lit, sizeof(lit));
	switch(dat.m_nType) 
	{
	case 0:
		lit.Type = A3DLIGHT_POINT;
		lit.Position = pos;
		lit.Ambient = a3d_ColorRGBAToColorValue(dat.m_dwAbient) * dat.m_fColorScale;
		lit.Diffuse = a3d_ColorRGBAToColorValue(dat.m_dwColor) * dat.m_fColorScale;
		lit.Range = dat.m_fRange;
		//如果是2.0版本升级上来的，那么计算下 A0,A1
		if( !dat.m_bUsed2_2IlluminationEquation)
		{	
			//B0 = 5.3333 * ln((4R^2*A2+8R*A1+16*A0)/(R^2*A2+4R*A1+16*A0))
			//	B1 = 4 * exp(0.25*B0) / (R2 * A2 + 2R * A1 + 4 * A0)
			//double e = 2.718281828f;
			double a = (4 * lit.Range * 4 * lit.Range ) * dat.m_fAttenuation2 + 8 * lit.Range * dat.m_fAttenuation1
				+ 16 * dat.m_fAttenuation0;
			double a1 = lit.Range * lit.Range * dat.m_fAttenuation2 + 4 * lit.Range * dat.m_fAttenuation1 + 4 * dat.m_fAttenuation0;
			a = a / a1;
			 double b0 = 5.333 * log( a );
			 a = 4 * exp( 0.25f * b0 );
			 a1 = lit.Range * lit.Range * dat.m_fAttenuation2 + 2 * lit.Range * dat.m_fAttenuation1 + 4 * dat.m_fAttenuation0;
			 double b1 = a / a1;

			 lit.Attenuation0 = b0;
			 lit.Attenuation1 = b1;
			 lit.Attenuation2 = 0.0f;

		}
		break;
	case 1:
		lit.Type = A3DLIGHT_DIRECTIONAL;
		lit.Position = pos;
		lit.Ambient = a3d_ColorRGBAToColorValue(dat.m_dwAbient) * dat.m_fColorScale;
		lit.Diffuse = a3d_ColorRGBAToColorValue(dat.m_dwColor) * dat.m_fColorScale;
		lit.Direction = dat.m_vDir;
		break;
	case 2:
		lit.Type = A3DLIGHT_SPOT;
		lit.Position = pos;
		lit.Ambient = a3d_ColorRGBAToColorValue(dat.m_dwAbient) * dat.m_fColorScale;
		lit.Diffuse = a3d_ColorRGBAToColorValue(dat.m_dwColor) * dat.m_fColorScale;
		lit.Attenuation0 = dat.m_fAttenuation0;
		lit.Attenuation1 = dat.m_fAttenuation1;
		lit.Attenuation2 = dat.m_fAttenuation2;
		lit.Falloff = dat.m_fFalloff;
		lit.Phi = dat.m_fPhi;
		lit.Theta = dat.m_fTheta;
		lit.Range = dat.m_fRange;
		break;
	default:
		break;
	}
	light.param = lit;
	return light;
}


bool ReadLitList()
{
	AFile file;
	if(!file.Open("bbbb.dat",AFILE_BINARY|AFILE_OPENEXIST)) 
	{
		return false;
	}
	int nLitNum = 0;
	DWORD nReaded;
	file.Read(&nLitNum,sizeof(int),&nReaded);
	for( int i = 0; i < nLitNum; ++i)
	{
		int nEffectType;//0 白天，1黑夜，2白天黑夜
		file.Read(&nEffectType,sizeof(int),&nReaded);
		A3DLIGHTPARAM params;
		file.Read(&params,sizeof(A3DLIGHTPARAM),&nReaded);
	}
	return true;
}

void  CExportLitDlg::OnCancel()
{
	ReadLitList();
	CDialog::OnCancel();
}




