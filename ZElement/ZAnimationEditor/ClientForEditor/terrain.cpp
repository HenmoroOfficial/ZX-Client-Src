/*
 * FILE: Terrain.cpp
 *
 * DESCRIPTION: header for terrain class on server side
 *
 * CREATED BY: Hedi, 2004/11/22 
 *
 * HISTORY:
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "terrain.h"

CTerrain g_Terrain;

/*
// used to init the static object s_Terrain;
class CTerrainLoader
{

public: 

	CTerrainLoader()
	{

		TERRAINCONFIG tConfig;
		tConfig.nNumAreas = 88;
		tConfig.nNumCols = 8;
		tConfig.nNumRows = 11;
		tConfig.nAreaHeight = 512;
		tConfig.nAreaWidth = 512;
		tConfig.vGridSize = 2.0f;
		tConfig.vHeightMax = 800.0f;
		tConfig.vHeightMin = 0.0f;
		strcpy(tConfig.szMapPath,"TerrainMap");

		s_Terrain.Init(tConfig, -10000,-10000,10000,10000);
	}
};

static CTerrainLoader s_TerrainLoader;


namespace NPCMove
{

float GetTerrainHeight(float x, float z)
{
	return s_Terrain.GetHeightAt(x,z);
}

};
*/

//////////////////////////////////////////////////////////////////////////
// Implement of CTerrain
//////////////////////////////////////////////////////////////////////////

CTerrain::CTerrain()
{
	m_pHeights		= NULL;
}

CTerrain::~CTerrain()
{
	Release();
}

bool CTerrain::Init(const TERRAINCONFIG& config, float xmin, float zmin, float xmax, float zmax)
{
	m_config = config;
	m_vGridSizeInv = 1.0f / config.vGridSize;

	// first determine the origin of the entire terrain, to be convinient, we set 
	// the origin to the left top corner of the entire terrain
	float t_ox = -config.nAreaWidth * config.nNumCols * config.vGridSize * 0.5f;
	float t_oz = config.nAreaHeight * config.nNumRows * config.vGridSize * 0.5f;
	
	float areawidth = config.nAreaWidth * config.vGridSize;
	float areaheight = config.nAreaHeight * config.vGridSize;

	int nAreaHStart = int((xmin - t_ox) / areawidth);
	if( nAreaHStart < 0 ) nAreaHStart = 0;
	int nAreaHEnd = int((xmax - t_ox) / areawidth);
	if( nAreaHEnd >= config.nNumCols ) nAreaHEnd = config.nNumCols - 1;

	int nAreaVStart = int((t_oz - zmax) / areaheight);
	if( nAreaVStart < 0 ) nAreaVStart = 0;
	int nAreaVEnd = int((t_oz - zmin) / areaheight);
	if( nAreaVEnd >= config.nNumRows ) nAreaVEnd = config.nNumRows - 1;

	m_ox = t_ox + nAreaHStart * config.nAreaWidth * config.vGridSize;
	m_oz = t_oz - nAreaVStart * config.nAreaHeight * config.vGridSize;

	m_nNumVertX = (nAreaHEnd - nAreaHStart + 1) * config.nAreaWidth + 1;
	m_nNumVertZ = (nAreaVEnd - nAreaVStart + 1) * config.nAreaHeight + 1;

	// now we should alloc the buffer
	m_pHeights = new float[m_nNumVertZ * m_nNumVertX];
	if( NULL == m_pHeights )
		return false;

	// Init the original terrain height to zero!
	memset(m_pHeights, 0, m_nNumVertZ * m_nNumVertX * sizeof(float));

	// then read from the hmap one by one
	char	szMapName[256];
	for(int v=nAreaVStart; v<=nAreaVEnd; v++)
	{
		for(int h=nAreaHStart; h<=nAreaHEnd; h++)
		{
			int idArea = v * config.nNumCols + h;

			sprintf(szMapName, "%s/%d.hmap", config.szMapPath, idArea + 1);
			FILE * fpHMap = fopen(szMapName, "rb");
// 			if( !fpHMap )
// 				continue;		// try next block!

			// return directly [6/11/2008 kuiwu]
			if (!fpHMap)
			{
				return false;
			}

			float * pHeight = m_pHeights + (v - nAreaVStart) * config.nAreaHeight * m_nNumVertX + (h - nAreaHStart) * config.nAreaWidth;
			for(int i=0; i<=config.nAreaHeight; i++)
			{
				fread(pHeight, sizeof(float), config.nAreaWidth + 1, fpHMap);

				pHeight += m_nNumVertX;
			}

			fclose(fpHMap);
		}
	}

	float range = config.vHeightMax - config.vHeightMin;
	float offset = config.vHeightMin;
	// convert from 0.0~1.0 to real height range
	for(int i=0; i<m_nNumVertZ * m_nNumVertX; i++)
		m_pHeights[i] = m_pHeights[i] * range + offset;
	
	return true;
}

bool CTerrain::Release()
{
	if (m_pHeights)
	{
		delete [] m_pHeights;
		m_pHeights = NULL;
	}
	
	return true;
}

float CTerrain::GetHeightAt(float x, float z)
{
	// note: check the valid [6/11/2008 kuiwu]
	if (!m_pHeights)
	{
		return 0.0f;
	}

	float		h = (x - m_ox) * m_vGridSizeInv;
	float		v = (m_oz - z) * m_vGridSizeInv;

	int		nX = (int) h;
	int		nZ = (int) v;
	float		dx = h - nX;
	float		dz = v - nZ;

	if( nX < 0 || nZ < 0 || nX >= m_nNumVertX || nZ >= m_nNumVertZ )
		return m_config.vHeightMin;
	
	int		v0 = nZ * m_nNumVertX + nX;
	float	h0, h1, h2;  
	//	0-----1
	//	| \   |
	//	|  \  |
	//	|   \ |
	//	2-----3
	if( dx <  dz )
	{
		// left-top triangle
		h0 = m_pHeights[v0 + m_nNumVertX];
		h1 = m_pHeights[v0 + m_nNumVertX + 1];
		h2 = m_pHeights[v0];
		dz = 1.0f - dz;
	}
	else
	{
		// right-bottom triangle
		h0 = m_pHeights[v0 + 1];
		h1 = m_pHeights[v0];
		h2 = m_pHeights[v0 + m_nNumVertX + 1];

		dx = 1.0f - dx;
	}

	return h0 + (h1 - h0) * dx + (h2 - h0) * dz;
}
