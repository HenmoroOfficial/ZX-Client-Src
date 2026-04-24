/*
 * FILE: TerrainExpImpB.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "Global.h"
#include "TerrainExpImpB.h"
#include "TerrainExportor.h"
#include "ExpTerrain.h"
#include "ExpElementMap.h"
#include "TerrainLayer.h"
#include "Bitmap.h"
#include "LightMapMerger.h"

#include "A3D.h"
#include "A3DTerrain2Loader.h"
#include "AFilePackage.h"
#ifdef _ANGELICA22
#include "dds/DdsTrans.h"
#endif // #ifdef _ANGELICA22

//#define new A_DEBUG_NEW

using namespace A3DTRN2LOADERB;

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////

extern bool	g_bTrnCompressed;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CTerrainExpImpB
//	
///////////////////////////////////////////////////////////////////////////

CTerrainExpImpB::CTerrainExpImpB() : 
m_aLayerMap(0, 100)
{
#ifdef _ANGELICA22
	m_aLMOffs		= NULL;
	m_fpVertexClr	= NULL;
#else
	m_fpTerrain		= NULL;
#endif // #ifdef _ANGELICA22
	m_pe			= NULL;
	m_fpMaskMap		= NULL;
	m_fpBlock		= NULL;
	m_aMaskAreas	= NULL;
	m_iProjMaskArea	= 0;
	m_iProjBlock	= 0;
	m_aBlockOffs	= NULL;
	m_aMaskAreaOffs	= NULL;
	m_aSubTerrains	= NULL;
	m_aProjLayers	= NULL;
	m_fProjTrnSize	= 0.0f;
	m_iProjHMSize	= 0;
	m_iProjMaskCol	= 0;
	m_iProjMaskRow	= 0;
	m_aFirstIdx		= NULL;
	m_iMaskMapSize	= 0;
	m_pCompHei		= NULL;
	m_pCompNormal	= NULL;
	m_pCompDiff		= NULL;
	m_pCompDiff1	= NULL;
	m_pLMBmp		= NULL;
	m_pLMBmp1		= NULL;
}

CTerrainExpImpB::~CTerrainExpImpB()
{
}

//	Do export
bool CTerrainExpImpB::DoExport(CTerrainExportor* pExporter, TRN2FILEHEADER* pTrnInfo)
{
	m_pe = pExporter;

	if (!ExportTerrainData())
		return false;

	if (pTrnInfo)
		*pTrnInfo = m_T2FileHeader;

	return true;
}

//	Add a texture file name and return texture's index
int CTerrainExpImpB::AddTextureFile(const char* szFile)
{
	if (!szFile || !szFile[0])
		return -1;
	
	//	Check whether this texture has existed
	for (int i=0; i < m_aTextureFile.GetSize(); i++)
	{
		if (!m_aTextureFile[i]->CompareNoCase(szFile))
			return i;
	}

	CString* pstrName = new CString(szFile);
	if (!pstrName)
		return -1;

	return m_aTextureFile.Add(pstrName);
}

//	Allocate exporting terrain resources
bool CTerrainExpImpB::AllocTerrainExportRes()
{
	//	Sub terrain information
	if (!(m_aSubTerrains = new TRN2FILESUBTRN [m_pe->m_iProjNum]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	//	Create layer map pairs
	if (!(m_aMaskAreas = new T2MKFILEMASKAREA [m_iProjMaskArea]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	//	Create block data offsets array
	if (!(m_aBlockOffs = new DWORD [m_iProjBlock]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	//	Create mask area data offsets array
	if (!(m_aMaskAreaOffs = new DWORD [m_iProjMaskArea]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	//	Create first indices array
	if (!(m_aFirstIdx = new int [m_iProjMaskArea]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	//	Create buffers used to store compressed block data
	int iNumVert = (m_T2FileHeader.iBlockGrid + 1) * (m_T2FileHeader.iBlockGrid + 1);
	if (!(m_pCompHei = new BYTE [iNumVert * sizeof (float) * 2]) ||
		!(m_pCompNormal = new BYTE [iNumVert * 3 * sizeof (float) * 2]) ||
		!(m_pCompDiff = new BYTE [iNumVert * sizeof (DWORD) * 2]) ||
		!(m_pCompDiff1 = new BYTE [iNumVert * sizeof (DWORD) * 2]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocTerrainExportRes, Not enough memory !");
		return false;
	}

	memset(m_aSubTerrains, 0, m_pe->m_iProjNum * sizeof (TRN2FILESUBTRN));

	return true;
}

//	Release resources used during exporting terrain
void CTerrainExpImpB::ReleaseTerrainExportRes()
{
	//	Release all texture file name
	for (int i=0; i < m_aTextureFile.GetSize(); i++)
		delete m_aTextureFile[i];

	m_aTextureFile.RemoveAll();

	if (m_aSubTerrains)
	{
		delete [] m_aSubTerrains;
		m_aSubTerrains = NULL;
	}

	if (m_aMaskAreas)
	{
		delete [] m_aMaskAreas;
		m_aMaskAreas = NULL;
	}

	if (m_aBlockOffs)
	{
		delete [] m_aBlockOffs;
		m_aBlockOffs = NULL;
	}

	if (m_aMaskAreaOffs)
	{
		delete [] m_aMaskAreaOffs;
		m_aMaskAreaOffs = NULL;
	}

	if (m_aFirstIdx)
	{
		delete [] m_aFirstIdx;
		m_aFirstIdx = NULL;
	}

	if (m_pCompHei)
	{
		delete [] m_pCompHei;
		m_pCompHei = NULL;
	}

	if (m_pCompNormal)
	{
		delete [] m_pCompNormal;
		m_pCompNormal = NULL;
	}

	if (m_pCompDiff)
	{
		delete [] m_pCompDiff;
		m_pCompDiff = NULL;
	}

	if (m_pCompDiff1)
	{
		delete [] m_pCompDiff1;
		m_pCompDiff1 = NULL;
	}
}

//	Check project name collision
//	Return true if there is name collision
bool CTerrainExpImpB::CheckProjNameCollision()
{
	//	If CTerrainExpImpB is used to export terrain data, project
	//	couldn't be used multi-times, that means each sub-terrain
	//	in whold should have different project name. If name collision
	//	occurs, all the project which have same name will be exported
	//	to same files
	for (int i=0; i < m_pe->m_iProjNum-1; i++)
	{
		CString* pstr1 = m_pe->m_aProjNames[i];

		for (int j=i+1; j < m_pe->m_iProjNum; j++)
		{
			CString* pstr2 = m_pe->m_aProjNames[j];
			if (!pstr1->CompareNoCase(*pstr2))
				return true;
		}
	}

	return false;
}

//	Export terrain data
bool CTerrainExpImpB::ExportTerrainData()
{
	if (CheckProjNameCollision())
	{
		AUX_ProgressLog("-- Project name collision found !!");
		return false;
	}

	//AUX_ProgressLog("-- Export terrain data ...");

	bool bRet = true;

#ifdef _ANGELICA22
#else
	//	Create terrain description file
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\%s.trn2", m_pe->m_strDestDir, m_pe->m_strName);

	if (!(m_fpTerrain = fopen(szFile, "wb+")))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportTerrainData, Failed to create terrain file %s", szFile);
		return false;
	}

	//	Write file version and identity of terrain file
	TRN2FILEIDVER IdVer;
	IdVer.dwIdentify = TRN2FILE_IDENTIFY;
	IdVer.dwVersion	 = TRN2FILE_VERSION;
	fwrite(&IdVer, 1, sizeof (IdVer), m_fpTerrain);
#endif // #ifdef _ANGELICA22
	//	Fill terrain file header
	memset(&m_T2FileHeader, 0, sizeof (m_T2FileHeader));
	m_T2FileHeader.iBlockGrid	= m_pe->m_InitData.iBlockGrid;
	m_T2FileHeader.iMaskGrid	= m_pe->m_InitData.iMaskTile;
	m_T2FileHeader.iMaskBlock	= m_pe->m_InitData.iMaskTile / m_pe->m_InitData.iBlockGrid;
	m_T2FileHeader.iNumSubTrn	= m_pe->m_aProjNames.GetSize();
	m_T2FileHeader.iSubTrnRow	= m_pe->m_InitData.iNumRow;
	m_T2FileHeader.iSubTrnCol	= m_pe->m_InitData.iNumCol;
	m_T2FileHeader.iLoaderID	= A3DTerrain2Loader::LOADER_B;
#ifdef _ANGELICA22
#else
	//	Write file header this time is only to hold place
	fwrite(&m_T2FileHeader, 1, sizeof (m_T2FileHeader), m_fpTerrain);
#endif // #ifdef _ANGELICA22
	m_iProjectCnt = 0;

	for (m_iProjRowCnt=0; m_iProjRowCnt < m_pe->m_InitData.iNumRow; m_iProjRowCnt++)
	{
		for (m_iProjColCnt=0; m_iProjColCnt < m_pe->m_InitData.iNumCol; m_iProjColCnt++)
		{
			if (!ExportProject(m_iProjectCnt))
			{
				AUX_ProgressLog("CTerrainExpImpB::ExportTerrainData, Failed to export project %d", m_iProjectCnt);
				bRet = false;
				goto End;
			}

			m_iProjectCnt++;
		}
	}
#ifdef _ANGELICA22
	if (!ExportTerrainFile())
#else
	if (!EndExportTerrainFile())
#endif // #ifdef _ANGELICA22
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportTerrainData, Failed to call EndExportTerrainFile()");
		bRet = false;
	}

End:

	//	Release resources
	ReleaseTerrainExportRes();
#ifdef _ANGELICA22
#else
	fclose(m_fpTerrain);
#endif // #ifdef _ANGELICA22

	return bRet;
}
#ifdef _ANGELICA22
//	End export terrain file
bool CTerrainExpImpB::ExportTerrainFile()
{

	//	Create terrain description file
	char szFile[MAX_PATH];
	sprintf(szFile, "%s\\%s.trn2", m_pe->m_strDestDir, m_pe->m_strName);

	FILE* fpTerrain = 0;		//	Terrain file pointer

	if (!(fpTerrain = fopen(szFile, "wb+")))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportTerrainFile, Failed to create terrain file %s", szFile);
		return false;
	}

	//	Write file version and identity of terrain file
	TRN2FILEIDVER IdVer;
	IdVer.dwIdentify = TRN2FILE_IDENTIFY;
	IdVer.dwVersion	 = TRN2FILE_VERSION;
	fwrite(&IdVer, 1, sizeof (IdVer), fpTerrain);

	//	Write file header this time is only to hold place
	fwrite(&m_T2FileHeader, 1, sizeof (m_T2FileHeader), fpTerrain);

	//	Write sub-terrain information
	fwrite(m_aSubTerrains, 1, m_pe->m_iProjNum * sizeof (TRN2FILESUBTRN), fpTerrain);

	m_T2FileHeader.iNumTexture	= m_aTextureFile.GetSize();
	m_T2FileHeader.dwTexOffset	= ftell(fpTerrain);

	int i;

	//	Write texture file names
	for (i=0; i < m_aTextureFile.GetSize(); i++)
		AUX_WriteString(fpTerrain, *m_aTextureFile[i]);

	//	Record loader section offset
	m_T2FileHeader.dwLoaderOff	= ftell(fpTerrain);

	TRN2FILELOADERINFO LoaderInfo;
	LoaderInfo.iMaskMapSize = m_iMaskMapSize;
	fwrite(&LoaderInfo, 1, sizeof (LoaderInfo), fpTerrain);

	//	Write sub-terrain name
	char szSubFile[MAX_PATH];
	for (i=0; i < m_pe->m_iProjNum; i++)
	{
		if (m_pe->m_InitData.bGameExp)
			sprintf(szSubFile, "%s_%s", m_pe->m_strName, *m_pe->m_aProjNames[i]);
		else
			strcpy(szSubFile, m_pe->m_strName);

		AUX_WriteString(fpTerrain, szSubFile);
	}

	//	Rewrite terrain file header
	fseek(fpTerrain, sizeof (TRN2FILEIDVER), SEEK_SET);
	fwrite(&m_T2FileHeader, 1, sizeof (m_T2FileHeader), fpTerrain);

	fclose(fpTerrain);
	
	return true;
}
#else
//	End export terrain file
bool CTerrainExpImpB::EndExportTerrainFile()
{
	m_T2FileHeader.iNumTexture	= m_aTextureFile.GetSize();
	m_T2FileHeader.dwTexOffset	= ftell(m_fpTerrain);

	int i;

	//	Write texture file names
	for (i=0; i < m_aTextureFile.GetSize(); i++)
		AUX_WriteString(m_fpTerrain, *m_aTextureFile[i]);

	//	Record loader section offset
	m_T2FileHeader.dwLoaderOff	= ftell(m_fpTerrain);

	TRN2FILELOADERINFO LoaderInfo;
	LoaderInfo.iMaskMapSize = m_iMaskMapSize;
	fwrite(&LoaderInfo, 1, sizeof (LoaderInfo), m_fpTerrain);

	//	Write sub-terrain name
	char szFile[MAX_PATH];
	for (i=0; i < m_pe->m_iProjNum; i++)
	{
		if (m_pe->m_InitData.bGameExp)
			sprintf(szFile, "%s_%s", m_pe->m_strName, *m_pe->m_aProjNames[i]);
		else
			strcpy(szFile, m_pe->m_strName);

		AUX_WriteString(m_fpTerrain, szFile);
	}

	//	Rewrite terrain file header
	fseek(m_fpTerrain, sizeof (TRN2FILEIDVER), SEEK_SET);
	fwrite(&m_T2FileHeader, 1, sizeof (m_T2FileHeader), m_fpTerrain);

	//	Rewrite sub-terrain infromation
	fwrite(m_aSubTerrains, 1, m_pe->m_iProjNum * sizeof (TRN2FILESUBTRN), m_fpTerrain);
	
	return true;
}
#endif // #ifdef _ANGELICA22
//	Allocate exporting project resources
bool CTerrainExpImpB::AllocProjectExportRes(int iProject, CExpTerrain* pTerrain)
{
	//	Clear buffers
	memset(m_aMaskAreas, 0, m_iProjMaskArea * sizeof (T2MKFILEMASKAREA));
	memset(m_aBlockOffs, 0, m_iProjBlock * sizeof (DWORD));
	memset(m_aMaskAreaOffs, 0, m_iProjMaskArea * sizeof (DWORD));
	memset(m_aFirstIdx, 0, m_iProjMaskArea * sizeof (int));

	//	------------ Create block data file
	char szFile[MAX_PATH];
	if (m_pe->m_InitData.bGameExp)
		sprintf(szFile, "%s\\%s_%s.t2bk", m_pe->m_strDestDir, m_pe->m_strName, *m_pe->m_aProjNames[iProject]);
	else
		sprintf(szFile, "%s\\%s.t2bk", m_pe->m_strDestDir, m_pe->m_strName);

	if (!(m_fpBlock = fopen(szFile, "wb+")))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocProjectExportRes, Failed to create block data file %s", szFile);
		return false;
	}

	//	write file version and identity of block data file
	TRN2FILEIDVER IdVer;
	IdVer.dwIdentify = T2BKFILE_IDENTIFY;
	IdVer.dwVersion	 = T2BKFILE_VERSION;
	fwrite(&IdVer, 1, sizeof (IdVer), m_fpBlock);

	//	Fill block file header
	memset(&m_BKFileHeader, 0, sizeof (m_BKFileHeader));
	m_BKFileHeader.iNumBlock	= m_iProjBlock;
	m_BKFileHeader.byCompressed	= T2BKCOMP_COL_ZIP | T2BKCOMP_HEI_ZIP | T2BKCOMP_NOR_ZIP | T2BKCOMP_NOR_DEG;
//	m_BKFileHeader.byCompressed	= T2BKCOMP_COL_ZIP | T2BKCOMP_HEI_ZIP | T2BKCOMP_NOR_ZIP | T2BKCOMP_HEI_INC | T2BKCOMP_NOR_DEG;
//	m_BKFileHeader.byCompressed	= T2BKCOMP_COL_ZIP | T2BKCOMP_HEI_INC | T2BKCOMP_NOR_DEG;
	//	Write file header this time
	fwrite(&m_BKFileHeader, 1, sizeof (m_BKFileHeader), m_fpBlock);

	//	Write block data offsets to hold place
	fwrite(m_aBlockOffs, 1, m_iProjBlock * sizeof (DWORD), m_fpBlock);

	//	------------- Create mask map file
	AUX_ChangeExtension(szFile, "t2mk");

	if (!(m_fpMaskMap = fopen(szFile, "wb+")))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocProjectExportRes, Failed to create mask map file %s", szFile);
		return false;
	}

	//	write file version and identity of mask map file
	IdVer.dwIdentify = T2MKFILE_IDENTIFY;
	IdVer.dwVersion	 = T2MKFILE_VERSION;
	fwrite(&IdVer, 1, sizeof (IdVer), m_fpMaskMap);

	//	Fill mask map file header
	memset(&m_MKFileHeader, 0, sizeof (m_MKFileHeader));
	m_MKFileHeader.iNumMaskArea	= m_iProjMaskArea;
	m_MKFileHeader.iNumLayer	= pTerrain->GetLayerNum();
	m_MKFileHeader.bCompressed	= g_bTrnCompressed;
	//	Write file header
	fwrite(&m_MKFileHeader, 1, sizeof (m_MKFileHeader), m_fpMaskMap);

	//	Write mask area information offsets to hold place
	fwrite(m_aMaskAreaOffs, 1, m_iProjMaskArea * sizeof (DWORD), m_fpMaskMap);

	//	Create layer buffer of this project
	if (!(m_aProjLayers = new T2MKFILELAYER [m_MKFileHeader.iNumLayer]))
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocProjectExportRes, Not enough memory! ");
		return false;
	}

	memset(m_aProjLayers, 0, m_MKFileHeader.iNumLayer * sizeof (T2MKFILELAYER));

	m_aLayerMap.RemoveAll(false);

	//	Load lightmap file from edit dir
	sprintf(szFile, "%s%s\\%s\\%srender.t2lm", g_szWorkDir, g_szEditMapDir, *m_pe->m_aProjNames[iProject], *m_pe->m_aProjNames[iProject]);
	CreateLightmap(szFile, true);
	strcat(szFile, "1");
	CreateLightmap(szFile, false);
#ifdef _ANGELICA22
	//-----------------------Create vertex color map file
	if (m_pe->m_InitData.bGameExp)
		sprintf(szFile, "%s\\%s_%s.t2dm", m_pe->m_strDestDir, m_pe->m_strName, *m_pe->m_aProjNames[iProject]);
	else
		sprintf(szFile, "%s\\%s.t2dm", m_pe->m_strDestDir, m_pe->m_strName);
	if (!(m_fpVertexClr = fopen(szFile, "wb+")))
	{
		AUX_ProgressLog("CTerrainExportor::ExportLightMaps, Failed to create lightmap file %s", szFile);
		return false;
	}

	//	Write version and identity of lightmap file
	TRN2FILEIDVER IdLitVer;
	IdLitVer.dwIdentify = T2LMFILE_IDENTIFY;
	IdLitVer.dwVersion	 = T2LMFILE_VERSION;
	fwrite(&IdLitVer, 1, sizeof (IdLitVer), m_fpVertexClr);

	bool b = false;
	if (b)
	{
		fflush(m_fpVertexClr);
	}

	//	Write file header to hold place
	memset(&m_CMFileHeader, 0, sizeof (m_CMFileHeader));
	m_CMFileHeader.iNumMaskArea	= m_iProjMaskArea;
	m_CMFileHeader.iMaskLMSize	= pTerrain->GetColorMap()->iWidth / (m_T2FileHeader.iSubTrnGrid / m_T2FileHeader.iMaskGrid);
	m_CMFileHeader.iLMDataSize	= m_CMFileHeader.iMaskLMSize * m_CMFileHeader.iMaskLMSize * 4;
	m_CMFileHeader.dwFlags = 0;//T2LMFILE_DDS;

	fwrite(&m_CMFileHeader, 1, sizeof (m_CMFileHeader), m_fpVertexClr);

	if (!AllocVertexColorMapExportRes())
	{
		AUX_ProgressLog("CTerrainExpImpB::AllocVertexColorMapExportRes, Failed to create resources");
		return false;
	}

	//	Write light map offsets to hold place
	fwrite(m_aLMOffs, 1, m_T2FileHeader.iNumMaskArea * sizeof (DWORD), m_fpVertexClr);

#endif // #ifdef _ANGELICA22
	return true;
}

//	Release resources used during exporting a project
void CTerrainExpImpB::ReleaseProjectExportRes()
{
#ifdef _ANGELICA22
	ReleaseVertexColorMapExportRes();
#endif // #ifdef _ANGELICA22
	if (m_aProjLayers)
	{
		delete [] m_aProjLayers;
		m_aProjLayers = NULL;
	}

	if (m_fpBlock)
	{
		fclose(m_fpBlock);
		m_fpBlock = NULL;
	}

	if (m_fpMaskMap)
	{
		fclose(m_fpMaskMap);
		m_fpMaskMap = NULL;
	}

	if (m_pLMBmp)
	{
		m_pLMBmp->Release();
		delete m_pLMBmp;
		m_pLMBmp = NULL;
	}

	if (m_pLMBmp1)
	{
		m_pLMBmp1->Release();
		delete m_pLMBmp1;
		m_pLMBmp1 = NULL;
	}
}

//	End export block data file
bool CTerrainExpImpB::EndBlockDataFile()
{
	//	Rewrite block data offsets
	fseek(m_fpBlock, sizeof (TRN2FILEIDVER) + sizeof (m_BKFileHeader), SEEK_SET);
	//	Rewrite block data offsets
	fwrite(m_aBlockOffs, 1, m_iProjBlock * sizeof (DWORD), m_fpBlock);

	return true;
}

//	End export mask map file
bool CTerrainExpImpB::EndExportMaskMapFile()
{
	//	Rewrite mask map file header
	fseek(m_fpMaskMap, sizeof (TRN2FILEIDVER) + sizeof (m_MKFileHeader), SEEK_SET);
	//	Rewrite mask area information offsets
	fwrite(m_aMaskAreaOffs, 1, m_iProjMaskArea * sizeof (DWORD), m_fpMaskMap);

	return true;
}

//	Export terrain data of a project
bool CTerrainExpImpB::ExportProject(int iProject)
{
	CString strProject = *m_pe->m_aProjNames[iProject];

	AUX_ProgressLog("-- Exporting terrain data of project [%d/%d] %s ...", iProject+1, m_pe->m_iProjNum, strProject);
	//AUX_ProgressLog("-- Loading terrain data ...");

	//	Make project's file name
	CString strFile;
	strFile.Format("%s%s\\%s\\%s.elproj", g_szWorkDir, g_szEditMapDir, strProject, strProject);

	//	Load project
	CExpElementMap Map;
	if (!Map.Load(strFile, LOAD_EXPTERRAIN))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProject, Failed to call CExpElementMap::Load");
		return false;
	}

	CExpTerrain* pExpTerrain = (CExpTerrain*)Map.GetTerrain();
	ASSERT(pExpTerrain);

	//	Is the first project ?
	if (!iProject)
	{
		//	Fill corresponding information in file header
		CTerrain::HEIGHTMAP* pHeightMap = pExpTerrain->GetHeightMap();
		
		m_fProjTrnSize	= pExpTerrain->GetTerrainSize();
		m_iProjHMSize	= pHeightMap->iWidth;

		m_T2FileHeader.fTerrainWid	= m_fProjTrnSize * m_pe->m_InitData.iNumCol;
		m_T2FileHeader.fTerrainHei	= m_fProjTrnSize * m_pe->m_InitData.iNumRow;
		m_T2FileHeader.fBlockSize	= pExpTerrain->GetTileSize() * m_pe->m_InitData.iBlockGrid;
		m_T2FileHeader.iTrnBlkRow	= (pHeightMap->iHeight - 1) / m_pe->m_InitData.iBlockGrid * m_pe->m_InitData.iNumRow;
		m_T2FileHeader.iTrnBlkCol	= (pHeightMap->iWidth - 1) / m_pe->m_InitData.iBlockGrid * m_pe->m_InitData.iNumCol;
		m_T2FileHeader.iNumBlock	= m_T2FileHeader.iTrnBlkRow * m_T2FileHeader.iTrnBlkCol;
		m_T2FileHeader.iSubTrnGrid	= pHeightMap->iWidth - 1;
		m_T2FileHeader.fGridSize	= pExpTerrain->GetTileSize();

		m_iProjBlock	= (pHeightMap->iHeight - 1) / m_pe->m_InitData.iBlockGrid * (pHeightMap->iWidth - 1) / m_pe->m_InitData.iBlockGrid;
		m_iProjMaskCol	= (pHeightMap->iWidth - 1) / m_pe->m_InitData.iMaskTile;
		m_iProjMaskRow	= (pHeightMap->iHeight - 1) / m_pe->m_InitData.iMaskTile;
		m_iProjMaskArea	= m_iProjMaskCol * m_iProjMaskRow;

		m_T2FileHeader.iNumMaskArea = m_iProjMaskArea * m_pe->m_InitData.iNumCol * m_pe->m_InitData.iNumRow;
		m_MKFileHeader.iNumMaskArea	= m_T2FileHeader.iNumMaskArea;

		//	Allocate exporting terrain resources
		if (!AllocTerrainExportRes())
		{
			AUX_ProgressLog("CTerrainExpImpB::ExportProject, Failed to call AllocExportRes() !");
			goto Error;
		}
#ifdef _ANGELICA22
#else
		//	Write data to file to hold place
		fwrite(m_aSubTerrains, 1, m_pe->m_iProjNum * sizeof (TRN2FILESUBTRN), m_fpTerrain);
#endif // #ifdef _ANGELICA22
	}
	else	//	Isn't the first project
	{
		//	Check project information
		CTerrain::HEIGHTMAP* pHeightMap = pExpTerrain->GetHeightMap();

		if (pExpTerrain->GetTerrainSize() != m_fProjTrnSize ||
			pHeightMap->iWidth != m_iProjHMSize)
		{
			AUX_ProgressLog("CTerrainExpImpB::ExportProject, Project %d terrain has different size !", iProject);
			goto Error;
		}
	}

	m_aSubTerrains[iProject].fMinHei	= pExpTerrain->GetYOffset();
	m_aSubTerrains[iProject].fMaxHei	= pExpTerrain->GetYOffset() + pExpTerrain->GetMaxHeight();

	//	Allocate resources for exporting this project
	if (!AllocProjectExportRes(iProject, pExpTerrain))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProject, Failed to allocate resources for project %d !", iProject);
		goto Error;
	}

	//AUX_ProgressLog("-- Export mask areas ...");
	AUX_ResetProgressBar(m_iProjMaskArea);

	//	Export mask areas
	if (!ExportProjectMaskAreas(pExpTerrain))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProject, Project %d failed to call ExportProjectMaskAreas() !");
		goto Error;
	}

	//AUX_ProgressLog("-- Export terrain blocks ...");
	AUX_ResetProgressBar(m_iProjBlock);

	//	Export blocks
	if (!ExportProjectBlocks(pExpTerrain))
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProject, Project %d failed to call ExportProjectBlocks() !");
		goto Error;
	}

	//	Do last exporting
	EndBlockDataFile();
	EndExportMaskMapFile();

	//	Release resources
	ReleaseProjectExportRes();
	Map.Release();

	return true;

Error:

	//	Release resources
	ReleaseProjectExportRes();
	Map.Release();

	return false;
}

//	Export all mask map areas in a project terrain
bool CTerrainExpImpB::ExportProjectMaskAreas(CExpTerrain* pTerrain)
{
	int i;

	//	Export layer information of this project
	ARectI rcMap;
	rcMap.left		= 0;
	rcMap.top		= 0;
	rcMap.right		= m_pe->m_InitData.iMaskTile;
	rcMap.bottom	= m_pe->m_InitData.iMaskTile;

	//	Get all mask map data in this mask area
	CExpTerrain::MASKMAPDATA* aMaskData = pTerrain->GetMaskAreaInfo(rcMap);
	if (!aMaskData)
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProjectMaskAreas, failed to call CExpTerrain::GetMaskAreaInfo !");
		return false;
	}

	for (i=0; i < pTerrain->GetLayerNum(); i++)
	{
		CTerrainLayer* pLayer = pTerrain->GetLayer(i);
		const CExpTerrain::MASKMAPDATA& LayerMask = aMaskData[i];
		T2MKFILELAYER& d = m_aProjLayers[i];

		d.iTextureIdx	= AddTextureFile(pLayer->GetTextureFile());
		d.iSpecMapIdx	= AddTextureFile(pLayer->GetSpecularMapFile());
		d.iMaskMapSize	= LayerMask.iWidth;
		d.byProjAxis	= (BYTE)pLayer->GetProjAxis();	
		d.fUScale		= pLayer->GetScaleU();
		d.fVScale		= pLayer->GetScaleV();	
		d.byWeight		= (BYTE)pLayer->GetLayerWeight();

		//	In current version, all mask map should have same size in order
		//	to merge maximum 4 mask maps into a 32-bit texture
		if (!m_iMaskMapSize)
			m_iMaskMapSize = d.iMaskMapSize;
		else if (d.iMaskMapSize)
		{
			ASSERT(m_iMaskMapSize == d.iMaskMapSize);
		}
	}

	//	Save layer information to file
	fwrite(m_aProjLayers, 1, pTerrain->GetLayerNum() * sizeof (T2MKFILELAYER), m_fpMaskMap);

	//	Release resources
	pTerrain->ReleaseMaskAreaInfo(aMaskData);
#ifdef _ANGELICA22
    if (!m_iMaskMapSize)
    {
        ASSERT( m_iProjMaskCol == m_iProjMaskRow );
//        ASSERT( g_Configs.iTExpMaskTile == DEFMASK_SIZE / m_iProjMaskRow );
        //?? how to calculate mask map size????
        m_iMaskMapSize = g_Configs.iTExpMaskTile;
    }
#endif // #ifdef _ANGELICA22
	//	Export mask areas
	for (int r=0; r < m_iProjMaskRow; r++)
	{
		int iMaskArea = r * m_iProjMaskCol;
		for (int c=0; c < m_iProjMaskCol; c++, iMaskArea++)
		{
			if (!ExportProjectMaskArea(pTerrain, iMaskArea, r, c))
				return false;
#ifdef _ANGELICA22
			// Export terrain vertex color map
			if(!ExportProjectVertexColorMap(pTerrain,iMaskArea,r,c))
				return false;
#endif // #ifdef _ANGELICA22
			//	Step progress bar
			AUX_StepProgressBar(1);
		}
	}

	return true;
}
#ifdef _ANGELICA22
/*	Get mask map size and check all layers to make sure they are the same!
*/
inline int GetMaskMapSize(CExpTerrain::MASKMAPDATA* aMaskData, int iNumLayers)
{
	int iWidth = 0;
	int iHeight = 0;
	for (int iLayerIdx = 0; iLayerIdx < iNumLayers; ++iLayerIdx)
	{
		if (!aMaskData[iLayerIdx].pData)
			continue;

		if (iWidth == 0)
			iWidth = aMaskData[iLayerIdx].iWidth;
		
		if (iHeight == 0)
			iHeight = aMaskData[iLayerIdx].iHeight;

		if (aMaskData[iLayerIdx].iWidth != iWidth)
			return -1;

		if (aMaskData[iLayerIdx].iHeight != iHeight)
			return -1;
	}

	if (iWidth != iHeight)
		return -1;

	return iWidth;
}


struct MASKDATA {
	BYTE rgb[3];
};

void FillMaskData(MASKDATA* pDstData, int iMaskMapSize, int iChannel, BYTE col)
{
	for (int i = 0; i < iMaskMapSize; ++i)
	{
		for (int j = 0; j < iMaskMapSize; ++j)
		{
			pDstData[j].rgb[iChannel] = col;
		}
		pDstData += iMaskMapSize;
	}
}
#endif // #ifdef _ANGELICA22
/*	Export one mask map area in a project terrain

	pTerrain: current project's terrain
	iMaskArea: mask area's index in current exporting terrain
	iRow, iCol: row and column in current project
*/
bool CTerrainExpImpB::ExportProjectMaskArea(CExpTerrain* pTerrain, int iMaskArea,
											 int iRow, int iCol)
{
	ARectI rcMap;
	rcMap.left		= iCol * m_pe->m_InitData.iMaskTile;
	rcMap.top		= iRow * m_pe->m_InitData.iMaskTile;
	rcMap.right		= rcMap.left + m_pe->m_InitData.iMaskTile;
	rcMap.bottom	= rcMap.top + m_pe->m_InitData.iMaskTile;

	//	Get all mask map data in this mask area
	CExpTerrain::MASKMAPDATA* aMaskData = pTerrain->GetMaskAreaInfo(rcMap);
	if (!aMaskData)
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProjectMaskArea, failed to call CExpTerrain::GetMaskAreaInfo !");
		return false;
	}

	//	Record mask area data's offset in file
	DWORD dwOffset = ftell(m_fpMaskMap);
	m_aMaskAreaOffs[iMaskArea] = dwOffset;

	int i;
	CString strFile;
	T2MKFILEMASKLAYER2 aMaskLayers[128];

	//	Throw off layers which have pure black mask map
	T2MKFILEMASKAREA& FileMaskArea = m_aMaskAreas[iMaskArea];
	FileMaskArea.iNumLayer = 0;

	m_aFirstIdx[iMaskArea] = m_aLayerMap.GetSize();

	for (i=0; i < pTerrain->GetLayerNum(); i++)
	{
		const CExpTerrain::MASKMAPDATA& LayerMask = aMaskData[i];
		CTerrainLayer* pLayer = pTerrain->GetLayer(i);

		if (pTerrain->IsPureBlackMask(LayerMask))
			continue;	//	Throw layer which has pure black mask
		
		T2MKFILEMASKLAYER2& d = aMaskLayers[FileMaskArea.iNumLayer];
		d.iLayer		= i;
		d.dwMapDataOff	= 0;
		d.dwDataSize	= 0;

		m_aLayerMap.Add(i);

		FileMaskArea.iNumLayer++;
	}
#ifdef _ANGELICA22
	const int iMaskMapSize = m_iMaskMapSize;//GetMaskMapSize(aMaskData, FileMaskArea.iNumLayer);
	if (iMaskMapSize < 0)
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProjectMaskArea, mask map size is not the same in all layers !");
		return false;
	}
#endif // #ifdef _ANGELICA22
/*	//	Note: All map data offset don't consider mask layer information,
	//		because we don't exactly how many layers are valid to this mask
	//		area just now. We should re-locate these offset here
	DWORD dwInfoSize = FileMaskArea.iNumLayer * sizeof (T2MKFILEMASKLAYER2);
	for (i=0; i < FileMaskArea.iNumLayer; i++)
		aMaskLayers[i].dwMapDataOff += dwInfoSize;
*/
	DWORD dwInfoSize = FileMaskArea.iNumLayer * sizeof (T2MKFILEMASKLAYER2);
	//	Write mask map area information to file
	fwrite(&FileMaskArea, 1, sizeof (FileMaskArea), m_fpMaskMap);
	//	Record current file pointer and write layer info (only to hold place)
	dwOffset = ftell(m_fpMaskMap);
	fwrite(aMaskLayers, dwInfoSize, 1, m_fpMaskMap);
#ifdef _ANGELICA22

	//	write mask map textures
	MASKDATA* aMaskBuf = new MASKDATA[iMaskMapSize * iMaskMapSize];
	memset(aMaskBuf, 0, sizeof(MASKDATA) * iMaskMapSize * iMaskMapSize);

	int iMaskChannel = 0;
	int iNumMaskTexs = (FileMaskArea.iNumLayer + 2) / 3;
	int iCurrentChannel = 0;
	int iMaskWriteCnt = 0;
	for (int i = 0; i < FileMaskArea.iNumLayer; ++i)
	{
		//	Get real layer index
		int iLayer = aMaskLayers[i].iLayer;
		const CExpTerrain::MASKMAPDATA& LayerMask = aMaskData[iLayer];

		iCurrentChannel = iMaskChannel % 3;
		if (!LayerMask.pData)	//	Default layer doesn't has mask
		{
			FillMaskData(aMaskBuf, iMaskMapSize, iCurrentChannel, 0xff);
			iMaskChannel++;
			continue;
		}

		BYTE* pLineData1 = LayerMask.pData;
		MASKDATA* pLineData2 = aMaskBuf;
		for (int r = 0; r < LayerMask.iHeight; ++r)
		{
			for (int c = 0; c < LayerMask.iWidth; ++c)
			{
				//	Default layer, mask is 0xff
				pLineData2[c].rgb[iCurrentChannel] = pLineData1[c];
			}
			pLineData1 += LayerMask.iPitch;
			pLineData2 += LayerMask.iWidth;
		}

		if (iCurrentChannel == 2)
		{
			// make texture here
			int iDataSize = 0;
			unsigned char* pCompressedDDS = DdsCompressBGR(reinterpret_cast<unsigned char*>(aMaskBuf), iDataSize, iMaskMapSize, 
				iMaskMapSize, 3, iMaskMapSize, k888, dGenerateMipMaps, 0);

			fwrite(&iDataSize, sizeof(int), 1, m_fpMaskMap);
			fwrite(pCompressedDDS, sizeof(unsigned char), iDataSize, m_fpMaskMap);
			memset(aMaskBuf, 0, sizeof(MASKDATA) * iMaskMapSize * iMaskMapSize);
			iMaskWriteCnt++;
		}

		iMaskChannel++;
	}

	//还有最后一张要写
	if (iMaskWriteCnt != iNumMaskTexs)
	{
		// make texture here
		int iDataSize = 0;
		unsigned char* pCompressedDDS = DdsCompressBGR(reinterpret_cast<unsigned char*>(aMaskBuf), iDataSize, iMaskMapSize, 
			iMaskMapSize, 3, iMaskMapSize, k888, dGenerateMipMaps, 0);

		fwrite(&iDataSize, sizeof(int), 1, m_fpMaskMap);
		fwrite(pCompressedDDS, sizeof(unsigned char), iDataSize, m_fpMaskMap);
		memset(aMaskBuf, 0, sizeof(MASKDATA) * iMaskMapSize * iMaskMapSize);
	}

	delete [] aMaskBuf;
#else
	//	Write mask map image data
	for (i=0; i < FileMaskArea.iNumLayer; i++)
	{
		//	Get real layer index
		int iLayer = aMaskLayers[i].iLayer;
		const CExpTerrain::MASKMAPDATA& LayerMask = aMaskData[iLayer];
		if (!LayerMask.pData)	//	Default layer doesn't has mask
			continue;

		//	Copy mask map data
		DWORD dwSize = LayerMask.iWidth * LayerMask.iHeight;
		BYTE* pSrcBuf = new BYTE [dwSize];

		BYTE* pLineData1 = LayerMask.pData;
		BYTE* pLineData2 = pSrcBuf;
		for (int r=0; r < LayerMask.iHeight; r++)
		{
			memcpy(pLineData2, pLineData1, LayerMask.iWidth * sizeof (BYTE));
			pLineData1 += LayerMask.iPitch;
			pLineData2 += LayerMask.iWidth;
		}

		if (g_bTrnCompressed)
		{
			DWORD dwDstSize = dwSize * 2;
			BYTE* pDstBuf = new BYTE [dwDstSize];
			AFilePackage::Compress(pSrcBuf, dwSize, pDstBuf, &dwDstSize);

			aMaskLayers[i].dwMapDataOff = ftell(m_fpMaskMap);
			aMaskLayers[i].dwDataSize	= dwDstSize;
			fwrite(pDstBuf, 1, dwDstSize, m_fpMaskMap);

			delete [] pDstBuf;
		}
		else
		{
			aMaskLayers[i].dwMapDataOff = ftell(m_fpMaskMap);
			aMaskLayers[i].dwDataSize	= dwSize;
			fwrite(pSrcBuf, 1, dwSize, m_fpMaskMap);
		}

		delete [] pSrcBuf;
	}
#endif // #ifdef _ANGELICA22

	//	Rewrite layer information, after this don't forget restore file pointer
	DWORD dwCurOff = ftell(m_fpMaskMap);
	fseek(m_fpMaskMap, dwOffset, SEEK_SET);
	fwrite(aMaskLayers, dwInfoSize, 1, m_fpMaskMap);
	fseek(m_fpMaskMap, dwCurOff, SEEK_SET);

	//	Release resources
	pTerrain->ReleaseMaskAreaInfo(aMaskData);

	return true;
}

//	Export blocks in a project terrain
bool CTerrainExpImpB::ExportProjectBlocks(CExpTerrain* pTerrain)
{
	int iProjBlock = (m_iProjHMSize-1) / m_pe->m_InitData.iBlockGrid;
	int iBaseRow = m_iProjRowCnt * iProjBlock;
	int iBaseCol = m_iProjColCnt * iProjBlock;

	for (int r=0; r < iProjBlock; r++)
	{
		int iBlock = r * iProjBlock;

		for (int c=0; c < iProjBlock; c++, iBlock++)
		{
			if (!ExportProjectBlock(pTerrain, iBlock, iBaseRow+r, iBaseCol+c, r, c))
				return false;

			//	Step progress bar
			AUX_StepProgressBar(1);
		}
	}

	return true;
}

/*	Export one block in a project terrain

	iBlock: block index in this project
	iTrnRow, iTrnCol: block row and column in whole exporting terrain
	iRow, iCol: row and column in current project
*/
bool CTerrainExpImpB::ExportProjectBlock(CExpTerrain* pTerrain, int iBlock, 
							int iTrnRow, int iTrnCol, int iRow, int iCol)
{
	int iMaskBlock = m_pe->m_InitData.iMaskTile / m_pe->m_InitData.iBlockGrid;

	//	Calculate index of the mask area which this block blongs to
	int iMaskPitch = m_T2FileHeader.iTrnBlkCol / iMaskBlock;
	int iMaskRow = iTrnRow / iMaskBlock;
	int iMaskCol = iTrnCol / iMaskBlock;
	int iMaskArea = iMaskRow * iMaskPitch + iMaskCol;

	T2BKFILEBLOCK6 FileBlock;
	FileBlock.iMaskArea		= iMaskArea;
	FileBlock.iRowInTrn		= iTrnRow;
	FileBlock.iColInTrn		= iTrnCol;
	FileBlock.dwLayerFlags	= 0;
	FileBlock.dwBlkFlags    = T2BKFLAG_DEFAULT;

	ARectI rcMap;
	rcMap.left		= iCol * m_pe->m_InitData.iBlockGrid;
	rcMap.top		= iRow * m_pe->m_InitData.iBlockGrid;
	rcMap.right		= rcMap.left + m_pe->m_InitData.iBlockGrid;
	rcMap.bottom	= rcMap.top + m_pe->m_InitData.iBlockGrid;

	//	Get all mask map data in this mask area
	CExpTerrain::MASKMAPDATA* aMaskData = pTerrain->GetMaskAreaInfo(rcMap);
	if (!aMaskData)
	{
		AUX_ProgressLog("CTerrainExpImpB::ExportProjectBlock, failed to call CExpTerrain::GetMaskAreaInfo !");
		return false;
	}

	int i, j, iFirstLayer, iProjMask;
	iMaskRow = iRow / iMaskBlock;
	iMaskCol = iCol / iMaskBlock;
	iProjMask = iMaskRow * m_iProjMaskCol + iMaskCol;

	int iFirstIdx = m_aFirstIdx[iProjMask];
	const T2MKFILEMASKAREA& MaskArea = m_aMaskAreas[iProjMask];

	//	Use the first pure white layer as block's first layer
	iFirstLayer = pTerrain->GetFirstPureWriteLayer(aMaskData);

	for (i=iFirstLayer; i < pTerrain->GetLayerNum(); i++)
	{
		const CExpTerrain::MASKMAPDATA& LayerMask = aMaskData[i];
		if (pTerrain->IsPureBlackMask(LayerMask))
			continue;	//	Throw layer which has pure black mask

		//	Re-map layer index
		for (j=0; j < MaskArea.iNumLayer; j++)
		{
			if (m_aLayerMap[iFirstIdx + j] == i)
				break;
		}

		ASSERT(j < MaskArea.iNumLayer);

		FileBlock.dwLayerFlags |= (1 << j);
	}

	//	Release resources
	pTerrain->ReleaseMaskAreaInfo(aMaskData);

	//	Record block information offset in file
	DWORD dwOffset = ftell(m_fpBlock);
	m_aBlockOffs[iBlock] = dwOffset;

	//	Get vertex height data and save it
	CExpTerrain::HEIGHTINFO HeightInfo;
	pTerrain->GetHeightData(rcMap, HeightInfo);
	BYTE* pLineData = (BYTE*)HeightInfo.pData;

	CTerrain::HEIGHTMAP* pHeightMap = pTerrain->GetHeightMap();
	int iNumBlockVert = HeightInfo.iWidth * HeightInfo.iHeight;

	float* aHeis = new float [iNumBlockVert];
	DWORD* aDiffs = new DWORD [iNumBlockVert];
	DWORD* aDiffs1 = new DWORD [iNumBlockVert];
	int iCount = 0;

	for (i=0; i < HeightInfo.iHeight; i++)
	{
		int vr = iRow * m_T2FileHeader.iBlockGrid + i;
		int vc = iCol * m_T2FileHeader.iBlockGrid;

		float* aScales = (float*)pLineData;
		pLineData += HeightInfo.iPitch;

		for (j=0; j < HeightInfo.iWidth; j++, iCount++, vc++)
		{
			aHeis[iCount] = pTerrain->GetYOffset() + aScales[j] * pTerrain->GetMaxHeight();

			float fr = (float)vr / (pHeightMap->iHeight-1);
			float fc = (float)vc / (pHeightMap->iWidth-1);
			aDiffs[iCount] = GetVertexColor(fr, fc, true);
			aDiffs1[iCount] = GetVertexColor(fr, fc, false);
		}
	}

	FileBlock.fLODScale = CalcBlockLODScale(aHeis, HeightInfo.iWidth, HeightInfo.iHeight);

	//	Compress height data ----------------------------
	if ((m_BKFileHeader.byCompressed & T2BKCOMP_HEI_ZIP) ||
		(m_BKFileHeader.byCompressed & T2BKCOMP_HEI_INC))
	{
		if (!(m_BKFileHeader.byCompressed & T2BKCOMP_HEI_INC))
		{	//	ZIP compress only
			DWORD dwSrcSize = HeightInfo.iWidth * HeightInfo.iHeight * sizeof (float);
			AFilePackage::Compress((BYTE*)aHeis, dwSrcSize, m_pCompHei, &dwSrcSize);
			FileBlock.dwHeiSize = dwSrcSize;
		}
		else if (!(m_BKFileHeader.byCompressed & T2BKCOMP_HEI_ZIP))
		{	//	Increamental compress only
			DWORD dwSrcSize;
			A3DTerrain2::CompressHeightMap(aHeis, iNumBlockVert, m_pCompHei, &dwSrcSize);
			FileBlock.dwHeiSize = dwSrcSize;
		}
		else
		{	//	Increamental compress and then ZIP compress
			DWORD dwSrcSize;
			BYTE* pTempHei = new BYTE [iNumBlockVert * 2 * sizeof (float)];
			A3DTerrain2::CompressHeightMap(aHeis, iNumBlockVert, pTempHei, &dwSrcSize);
			AFilePackage::Compress(pTempHei, dwSrcSize, m_pCompHei, &dwSrcSize);
			FileBlock.dwHeiSize = dwSrcSize;
			delete [] pTempHei;	
		}
	}
	else	//	No compression
	{
		FileBlock.dwHeiSize = iNumBlockVert * sizeof (float);
		memcpy(m_pCompHei, aHeis, FileBlock.dwHeiSize);
	}

	//	Compress color data -----------------
	if (m_BKFileHeader.byCompressed & T2BKCOMP_COL_ZIP)
	{
		DWORD dwSrcSize = HeightInfo.iWidth * HeightInfo.iHeight * sizeof (DWORD);
		AFilePackage::Compress((BYTE*)aDiffs, dwSrcSize, m_pCompDiff, &dwSrcSize);
		FileBlock.dwDiffSize = dwSrcSize;

		dwSrcSize = HeightInfo.iWidth * HeightInfo.iHeight * sizeof (DWORD);
		AFilePackage::Compress((BYTE*)aDiffs1, dwSrcSize, m_pCompDiff1, &dwSrcSize);
		FileBlock.dwDiffSize1 = dwSrcSize;

	/*	DWORD dwSrcSize;
		A3DTerrain2::CompressColorMap(aDiffs, iNumBlockVert, m_pCompDiff, &dwSrcSize);
		FileBlock.dwDiffSize = dwSrcSize;

		A3DTerrain2::CompressColorMap(aDiffs1, iNumBlockVert, m_pCompDiff1, &dwSrcSize);
		FileBlock.dwDiffSize1 = dwSrcSize;	*/
	}
	else	//	No compression
	{

		FileBlock.dwDiffSize = iNumBlockVert * sizeof (DWORD);
		memcpy(m_pCompDiff, aDiffs, FileBlock.dwDiffSize);

		FileBlock.dwDiffSize1 = iNumBlockVert * sizeof (DWORD);
		memcpy(m_pCompDiff1, aDiffs1, FileBlock.dwDiffSize1);
	}

	//	Get vertex normal data and save it
	pTerrain->GetNormalData(rcMap, HeightInfo);
	pLineData = (BYTE*)HeightInfo.pData;

	float* aNormals = new float [iNumBlockVert * 3];
	iCount = 0;

	for (i=0; i < HeightInfo.iHeight; i++)
	{
		A3DVECTOR3* pSrcLine = (A3DVECTOR3*)pLineData;
		pLineData += HeightInfo.iPitch;

		for (j=0; j < HeightInfo.iWidth; j++, iCount+=3)
		{
			aNormals[iCount+0] = pSrcLine[j].x;
			aNormals[iCount+1] = pSrcLine[j].y;
			aNormals[iCount+2] = pSrcLine[j].z;
		}
	}

	//	Compress normal data ------------------
	if ((m_BKFileHeader.byCompressed & T2BKCOMP_NOR_ZIP) ||
		(m_BKFileHeader.byCompressed & T2BKCOMP_NOR_DEG))
	{
		if (!(m_BKFileHeader.byCompressed & T2BKCOMP_NOR_DEG))
		{	//	ZIP compress only
			DWORD dwSrcSize = iNumBlockVert * sizeof (float) * 3;
			AFilePackage::Compress((BYTE*)aNormals, dwSrcSize, m_pCompNormal, &dwSrcSize);
			FileBlock.dwNormalSize = dwSrcSize;
		}
		else if (!(m_BKFileHeader.byCompressed & T2BKCOMP_NOR_ZIP))
		{	//	Convert to degree only
			DWORD dwSrcSize;
			A3DTerrain2::CompressNormalMap(aNormals, iNumBlockVert, m_pCompNormal, &dwSrcSize);
			FileBlock.dwNormalSize = dwSrcSize;
		}
		else
		{	//	Convert to degreen and then ZIP compress
			DWORD dwSrcSize;
			BYTE* pTempNormal = new BYTE [iNumBlockVert * 3 * sizeof (float) * 2];
			A3DTerrain2::CompressNormalMap(aNormals, iNumBlockVert, pTempNormal, &dwSrcSize);
			AFilePackage::Compress(pTempNormal, dwSrcSize, m_pCompNormal, &dwSrcSize);
			FileBlock.dwNormalSize = dwSrcSize;
			delete [] pTempNormal;
		}
	}
	else	//	No compression
	{
		FileBlock.dwNormalSize = iNumBlockVert * sizeof (float) * 3;
		memcpy(m_pCompNormal, aNormals, FileBlock.dwNormalSize);
	}

	//	Write block information
	fwrite(&FileBlock, 1, sizeof (FileBlock), m_fpBlock);
	//	Write vertex height data to file
	fwrite(m_pCompHei, 1, FileBlock.dwHeiSize, m_fpBlock);
	//	Write vertex normal data to file
	fwrite(m_pCompNormal, 1, FileBlock.dwNormalSize, m_fpBlock);
	//	Write vertex diffuse data for day to file
	fwrite(m_pCompDiff, 1, FileBlock.dwDiffSize, m_fpBlock);
	//	Write vertex diffuse data for night to file
	fwrite(m_pCompDiff1, 1, FileBlock.dwDiffSize1, m_fpBlock);

	delete [] aHeis;
	delete [] aNormals;
	delete [] aDiffs;
	delete [] aDiffs1;

	return true;
}

//	Calculate block's LOD scale
float CTerrainExpImpB::CalcBlockLODScale(float* aHeis, int iWid, int iHei)
{
	float fBlockSize = m_T2FileHeader.fBlockSize;

	/*	v0----v1
		|\	  |		Z
		| \   |		|
		|  \  |		|
		|	\ |		|
		v2----v3	------- X
	*/
	A3DVECTOR3 v0(0.0f, aHeis[0], 0.0f);
	A3DVECTOR3 v1(fBlockSize, aHeis[iWid-1], 0.0f);
	A3DVECTOR3 v2(0.0f, aHeis[(iWid-1)*iHei], -fBlockSize);
	A3DVECTOR3 v3(fBlockSize, aHeis[iWid*iHei-1], -fBlockSize);

	A3DPLANE p0, p1;
	p0.CreatePlane(v0, v1, v3);
	p1.CreatePlane(v0, v3, v2);

	//	0.866 = cos(30 deg)
	if (DotProduct(p0.vNormal, p1.vNormal) < 0.866f)
		return 1.0f;

	float fDelta0=0.0f, fDelta1=0.0f;
	float fStep = fBlockSize / iWid;
	A3DVECTOR3 vPos;
	int iCount = 0;

	for (int i=0; i < iHei; i++)
	{
		vPos.z = -i * fStep;

		for (int j=0; j < iWid; j++, iCount++)
		{
			vPos.x = j * fStep;
			vPos.y = aHeis[iCount];

			float fDelta;

			if (j < i)
			{
				fDelta = (float)fabs(DotProduct(vPos - v0, p1.vNormal));
				if (fDelta > fDelta1)
					fDelta1 = fDelta;
			}
			else
			{
				fDelta = (float)fabs(DotProduct(vPos - v0, p0.vNormal));
				if (fDelta > fDelta0)
					fDelta0 = fDelta;
			}
		}
	}

	float fLimit = fBlockSize * 0.4f;
	float fScale = a_Max(fDelta0, fDelta1);
	if (fScale > fLimit)
		return 1.0f;

	fScale = 1.0f - (fLimit - fScale) / fLimit;

	return fScale;
}

//	Create project lightmap
bool CTerrainExpImpB::CreateLightmap(const char* szFile, bool bDay)
{
	FILE* pLMFile = fopen(szFile, "rb");
	if (!pLMFile)
		return false;

	//	Read lightmap file version and Identity
	TRN2FILEIDVER IdVer;
	if (fread(&IdVer, 1, sizeof (IdVer), pLMFile) != sizeof (IdVer))
	{
		fclose(pLMFile);
		return false;
	}
	
	if (IdVer.dwIdentify != T2LMFILE_IDENTIFY || IdVer.dwVersion > T2LMFILE_VERSION)
	{
		fclose(pLMFile);
		return false;
	}

	int iNumMaskArea, iMaskLMSize;

	if (IdVer.dwVersion == 1)
	{
		T2LMFILEHEADER Header;
		fread(&Header, 1, sizeof (Header), pLMFile);
		iNumMaskArea = Header.iNumMaskArea;
		iMaskLMSize	 = Header.iMaskLMSize;
	}
	else	//	IdVer.dwVersion >= 2
	{
		T2LMFILEHEADER2 Header;
		fread(&Header, 1, sizeof (Header), pLMFile);

		//	Source lightmap file shouldn't be DDS format
		if (Header.dwFlags & T2LMFILE_DDS)
		{
			fclose(pLMFile);
			return false;
		}

		iNumMaskArea = Header.iNumMaskArea;
		iMaskLMSize	 = Header.iMaskLMSize;
	}

	int iLMSize	= CLightMapMerger::CalcLightmapSize(iNumMaskArea, iMaskLMSize);

	//	Create lightmap object
	CELBitmap* pLMBmp = new CELBitmap;
	if (!pLMBmp)
	{
		fclose(pLMFile);
		return false;
	}

	if (!pLMBmp->CreateBitmap(iLMSize, iLMSize, 24, NULL))
	{
		delete pLMBmp;
		fclose(pLMFile);
		return false;
	}

	pLMBmp->Clear();

	if (!CLightMapMerger::ReadLightmapData(pLMBmp, pLMFile, iNumMaskArea, iMaskLMSize))
	{
		pLMBmp->Release();
		delete pLMBmp;
	}

	fclose(pLMFile);

	if (bDay)
		m_pLMBmp = pLMBmp;
	else
		m_pLMBmp1 = pLMBmp;

	return true;
}

//	Get vertex color
DWORD CTerrainExpImpB::GetVertexColor(float fr, float fc, bool bDay)
{
	if ((bDay && !m_pLMBmp) || (!bDay && !m_pLMBmp1))
		return 0xffffffff;

	CELBitmap* pLMBmp = bDay ? m_pLMBmp : m_pLMBmp1;

	int r = (int)(fr * (pLMBmp->GetHeight()-1) + 0.5f);
	int c = (int)(fc * (pLMBmp->GetWidth()-1) + 0.5f);

	a_Clamp(r, 0, pLMBmp->GetHeight()-1);
	a_Clamp(c, 0, pLMBmp->GetWidth()-1);

	BYTE* p = pLMBmp->GetBitmapData() + pLMBmp->GetPitch() * r + c * (pLMBmp->GetBPP() >> 3);
	DWORD col = 0xff000000;
	col |= *p;
	col |= (*(p+1) << 8);
	col |= (*(p+2) << 16);

	return col;
}
#ifdef _ANGELICA22
/*	Export a project mask area's light map

iMaskArea: mask area's index in whole exporting terrain
iRow, iCol: row and column in current project
*/
bool CTerrainExpImpB::ExportProjectVertexColorMap(CExpTerrain* pTerrain, int iMaskArea, int iRow, int iCol)
{
	CELBitmap bitMap;
	CTerrain::VERTEXCOLOR* pVC = pTerrain->GetColorMap();
	if (!bitMap.CreateBitmap(pVC->iWidth, pVC->iWidth, 32, NULL))
	{
		g_Log.Log("CTerrainExpImpB::ExportProjectVertexColorMap, Failed to create bitmap");
		return false;
	}

	//	Fill bitmap data
	BYTE* pBmpLine = bitMap.GetBitmapData();
	BYTE* pLight = (BYTE*)pVC->pColor;
	for (int i=0; i < pVC->iWidth; i++)
	{
		BYTE* pPixel = pBmpLine;
		pBmpLine += bitMap.GetPitch();

		for (int j=0; j < pVC->iWidth; j++)
		{
			*(pPixel++) = *(pLight++);
			*(pPixel++) = *(pLight++);
			*(pPixel++) = *(pLight++);
			*(pPixel++) = *(pLight++);
		}
	}

	ARectI rcArea;
	rcArea.left		= iCol * m_CMFileHeader.iMaskLMSize;
	rcArea.top		= iRow * m_CMFileHeader.iMaskLMSize;
	rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
	rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

	//	Record mask area data's offset in file
	DWORD dwOffset = ftell(m_fpVertexClr);
	m_aLMOffs[iMaskArea] = dwOffset;

	CELBitmap::LOCKINFO LockInfo;
	bitMap.LockRect(rcArea, &LockInfo);

	CELBitmap::LOCKINFO aNeighborLockInfo[8];
	memset(aNeighborLockInfo, 0, sizeof(aNeighborLockInfo));
	int iProjMask = (m_iProjHMSize-1) / m_pe->m_InitData.iMaskTile;

	if (iCol > 0 && iRow > 0)
	{
		ARectI rcArea;
		rcArea.left		= (iCol - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[0]);
	}

	if (iRow > 0)
	{
		ARectI rcArea;
		rcArea.left		= iCol * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;
		bitMap.LockRect(rcArea, &aNeighborLockInfo[1]);
	}

	if (iRow > 0 && iCol + 1 < iProjMask)
	{
		ARectI rcArea;
		rcArea.left		= (iCol + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[2]);
	}

	if (iCol > 0)
	{
		ARectI rcArea;
		rcArea.left		= (iCol - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= iRow * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[3]);
	}

	if (iCol + 1 < iProjMask)
	{
		ARectI rcArea;
		rcArea.left		= (iCol + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= iRow * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[4]);
	}

	if (iRow + 1 < iProjMask && iCol > 0)
	{
		ARectI rcArea;
		rcArea.left		= (iCol - 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[5]);
	}

	if (iRow + 1 < iProjMask)
	{
		ARectI rcArea;
		rcArea.left		= iCol * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[6]);
	}

	if (iRow + 1 < iProjMask && iCol + 1 < iProjMask)
	{
		ARectI rcArea;
		rcArea.left		= (iCol + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.top		= (iRow + 1) * m_CMFileHeader.iMaskLMSize;
		rcArea.right	= rcArea.left + m_CMFileHeader.iMaskLMSize;
		rcArea.bottom	= rcArea.top + m_CMFileHeader.iMaskLMSize;

		bitMap.LockRect(rcArea, &aNeighborLockInfo[7]);
	}

	BYTE* pDstData = new BYTE[m_CMFileHeader.iMaskLMSize * 4];
	if (!pDstData)
	{
		AUX_ProgressLog("CTerrainExportor::ExportProjectMaskLightMap, Not enough memory !");
		return false;
	}

	//	Write mask map light map data file
	BYTE* pSrcData = LockInfo.pData;

	for (int i=0; i < LockInfo.iHeight; i++)
	{
		if (i == 0)
		{
			for (int x = 0; x < LockInfo.iWidth; x++)
			{
				int nNeighborCount = 0;
				DWORD aNeighborMaskVal[3];

				if (x == 0)
				{
					if (aNeighborLockInfo[0].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[0].pData[(LockInfo.iHeight - 1) * LockInfo.iPitch + (LockInfo.iWidth - 1) * 4];
					}

					if (aNeighborLockInfo[3].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[3].pData[(LockInfo.iWidth - 1) * 4];
					}
				}
				else if (x == LockInfo.iWidth - 1)
				{
					if (aNeighborLockInfo[2].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[2].pData[(LockInfo.iHeight - 1) * LockInfo.iPitch];
					}

					if (aNeighborLockInfo[4].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[4].pData[0];
					}
				}

				if (aNeighborLockInfo[1].pData)
					aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[1].pData[(LockInfo.iHeight - 1) * LockInfo.iPitch + x * 4];

				WORD wTotal[4];
				int off = x * 4;
				wTotal[0] = pSrcData[off];
				wTotal[1] = pSrcData[off + 1];
				wTotal[2] = pSrcData[off + 2];
				wTotal[3] = pSrcData[off + 3];

				for (int xx = 0; xx < nNeighborCount; xx++)
				{
					BYTE* pVal = (BYTE*)&aNeighborMaskVal[xx];
					wTotal[0] += pVal[0];
					wTotal[1] += pVal[1];
					wTotal[2] += pVal[2];
					wTotal[3] += pVal[3];
				}

				wTotal[0] /= WORD(nNeighborCount + 1);
				wTotal[1] /= WORD(nNeighborCount + 1);
				wTotal[2] /= WORD(nNeighborCount + 1);
				wTotal[3] /= WORD(nNeighborCount + 1);

				pDstData[off]	  = (BYTE)wTotal[0];
				pDstData[off + 1] = (BYTE)wTotal[1];
				pDstData[off + 2] = (BYTE)wTotal[2];
				pDstData[off + 3] = (BYTE)wTotal[3];
			}
		}
		else if (i == LockInfo.iHeight - 1)
		{
			for (int x = 0; x < LockInfo.iWidth; x++)
			{
				int nNeighborCount = 0;
				DWORD aNeighborMaskVal[3];

				if (x == 0)
				{
					if (aNeighborLockInfo[5].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[5].pData[(LockInfo.iWidth - 1) * 4];
					}

					if (aNeighborLockInfo[3].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[3].pData[(LockInfo.iHeight - 1) * LockInfo.iPitch + (LockInfo.iWidth - 1) * 4];
					}
				}
				else if (x == LockInfo.iWidth - 1)
				{
					if (aNeighborLockInfo[7].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[7].pData[0];
					}

					if (aNeighborLockInfo[4].pData)
					{
						aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[4].pData[(LockInfo.iHeight - 1) * LockInfo.iPitch];
					}
				}

				if (aNeighborLockInfo[6].pData)
					aNeighborMaskVal[nNeighborCount++] = *(DWORD*)&aNeighborLockInfo[6].pData[x * 4];

				WORD wTotal[4];
				int off = x * 4;
				wTotal[0] = pSrcData[off];
				wTotal[1] = pSrcData[off + 1];
				wTotal[2] = pSrcData[off + 2];
				wTotal[3] = pSrcData[off + 3];

				for (int xx = 0; xx < nNeighborCount; xx++)
				{
					BYTE* pVal = (BYTE*)&aNeighborMaskVal[xx];
					wTotal[0] += pVal[0];
					wTotal[1] += pVal[1];
					wTotal[2] += pVal[2];
					wTotal[3] += pVal[3];
				}

				wTotal[0] /= WORD(nNeighborCount + 1);
				wTotal[1] /= WORD(nNeighborCount + 1);
				wTotal[2] /= WORD(nNeighborCount + 1);
				wTotal[3] /= WORD(nNeighborCount + 1);

				pDstData[off]	  = (BYTE)wTotal[0];
				pDstData[off + 1] = (BYTE)wTotal[1];
				pDstData[off + 2] = (BYTE)wTotal[2];
				pDstData[off + 3] = (BYTE)wTotal[3];
			}
		}
		else
		{
			memcpy(pDstData, pSrcData, LockInfo.iWidth * 4);

			if (aNeighborLockInfo[3].pData)
			{
				DWORD d = *(DWORD*)&aNeighborLockInfo[3].pData[i * LockInfo.iPitch + (LockInfo.iWidth - 1) * 4];
				BYTE* p = (BYTE*)&d;

				for (int xxx = 0; xxx < 4; xxx++)
				{
					WORD w = pSrcData[xxx];
					w += p[xxx];
					pDstData[xxx] = BYTE(w / 2);
				}
			}

			if (aNeighborLockInfo[4].pData)
			{
				DWORD d = *(DWORD*)&aNeighborLockInfo[4].pData[i * LockInfo.iPitch];
				BYTE* p = (BYTE*)&d;
				int off = (LockInfo.iWidth - 1) * 4;

				for (int xxx = 0; xxx < 4; xxx++)
				{
					WORD w = pSrcData[off + xxx];
					w += p[xxx];
					pDstData[off + xxx] = BYTE(w / 2);
				}
			}
		}

		fwrite(pDstData, 1, LockInfo.iWidth * 4, m_fpVertexClr);
		pSrcData += LockInfo.iPitch;
	}

	delete [] pDstData;

	return true;
}



//	Allocate exporting light map resources
bool CTerrainExpImpB::AllocVertexColorMapExportRes()
{
	//	Create mask area light map data offsets array
	if (!(m_aLMOffs = new DWORD [m_T2FileHeader.iNumMaskArea]))
	{
		AUX_ProgressLog("CTerrainExportor::AllocLightmapExportRes, Not enough memory !");
		return false;
	}

	memset(m_aLMOffs, 0, m_T2FileHeader.iNumMaskArea * sizeof (DWORD));

	return true;
}

//	Release resources used during exporting light map
void CTerrainExpImpB::ReleaseVertexColorMapExportRes()
{
	if( m_fpVertexClr )
		fseek(m_fpVertexClr, sizeof (TRN2FILEIDVER) + sizeof (m_CMFileHeader), SEEK_SET);
	if( m_aLMOffs )
		fwrite( m_aLMOffs, 1, m_CMFileHeader.iNumMaskArea * sizeof (DWORD), m_fpVertexClr);

	if (m_fpVertexClr)
	{
		fclose(m_fpVertexClr);
		m_fpVertexClr = NULL;
	}

	if (m_aLMOffs)
	{
		delete [] m_aLMOffs;
		m_aLMOffs = NULL;
	}
}
#endif // #ifdef _ANGELICA22
