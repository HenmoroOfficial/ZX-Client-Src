/*
 * FILE: EC_ScnBoxArea.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/10/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_ScnBoxArea.h"
#include "EC_WorldFile.h"
#include "EC_Scene.h"
#include "EC_Game.h"

#include <AF.h>
#include <A3DLight.h>
#include <A3DFuncs.h>

#define new A_DEBUG_NEW

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECScnBoxArea
//	
///////////////////////////////////////////////////////////////////////////

CECScnBoxArea::CECScnBoxArea() : CECSceneObject(TYPE_BOXAREA)
{
	m_dwAreaID	= 0;
	m_iPriority	= 0;
	m_nSkyType = 0;
}

CECScnBoxArea::~CECScnBoxArea()
{
}

//	Load data from file
bool CECScnBoxArea::Load(CECScene* pScene, AFile* pFile, float fOffX, float fOffZ)
{
	DWORD dwRead;

	if (pScene->GetBSDFileVersion() < 6)
	{
		ECBSDFILEBOXAREA Data;

		if (!pFile->Read(&Data, sizeof (Data), &dwRead))
			return false;

		m_bDefArea		= Data.iDefArea ? true : false;
		m_dwTransTime	= Data.dwTransTime;
		m_dwAreaFlags	= 0;

		m_fFogStart		= Data.fFogStart;
		m_fFogEnd		= Data.fFogEnd;
		m_fFogDensity	= Data.fFogDensity;
		m_dwFogColor	= Data.dwFogColor;
		m_fUWFogStart	= Data.fUWFogStart;
		m_fUWFogEnd		= Data.fUWFogEnd;
		m_fUWFogDensity	= Data.fUWFogDensity;
		m_dwUWFogColor	= Data.dwUWFogCol;
		m_fFogStart_n	= m_fFogStart;
		m_fFogEnd_n		= m_fFogEnd;
		m_fFogDensity_n	= m_fFogDensity;
		m_dwFogColor_n	= m_dwFogColor;

		m_dwAmbient		= Data.dwAmbient;
		m_dwDirLtCol	= Data.dwDirLtCol;
		m_dwAmbient_n	= m_dwAmbient;
		m_dwDirLtCol_n	= m_dwDirLtCol;
	
#ifndef BACK_VERSION
		A3DCOLORVALUE testAmbient = A3DCOLORVALUE(m_dwAmbient);
		A3DCOLORVALUE testAmbientN = A3DCOLORVALUE(m_dwAmbient_n);
#endif
		
		m_fSkySpeedU	= Data.vSkySpeed[0];
		m_fSkySpeedV	= Data.vSkySpeed[1];

		m_vLightDir		= g_pGame->GetDirLight()->GetLightparam().Direction;

		m_iRainSpeed		= 0;
		m_fRainSize			= 0.0f;
		m_dwRainCol			= 0;
		m_iSnowSpeed		= 0;
		m_fSnowSize			= 0.0f;
		m_dwSnowCol			= 0;
		m_fSunPower			= 1.0f;
		m_fSunPower_n		= 1.0f;	
		m_dwCloudAmb		= m_dwAmbient;
		m_dwCloudDirCol		= m_dwDirLtCol;
		m_dwCloudAmb_n		= m_dwCloudAmb;
		m_dwCloudDirCol_n	= m_dwCloudDirCol;
		m_fUWFogStart_n		= Data.fUWFogStart;
		m_fUWFogEnd_n		= Data.fUWFogEnd;
		m_fUWFogDensity_n	= Data.fUWFogDensity;
		m_dwUWFogColor_n	= Data.dwUWFogCol;

		m_obb.Center.Set(Data.vPos[0]+fOffX, Data.vPos[1], Data.vPos[2]+fOffZ);
		m_obb.Extents.Set(Data.vExt[0], Data.vExt[1], Data.vExt[2]);
		m_obb.XAxis.Set(Data.vAxisX[0], Data.vAxisX[1], Data.vAxisX[2]);
		m_obb.YAxis.Set(Data.vAxisY[0], Data.vAxisY[1], Data.vAxisY[2]);
		m_obb.ZAxis.Set(Data.vAxisZ[0], Data.vAxisZ[1], Data.vAxisZ[2]);

		m_obb.CompleteExtAxis();

		m_tmBorn.Clear();
		m_tmDie.Clear();

		//	Read sky texture file names
		pFile->ReadString(m_strSkyFile0);
		pFile->ReadString(m_strSkyFile1);
		pFile->ReadString(m_strSkyFile2);
		m_strSkyFile0_n = m_strSkyFile0;
		m_strSkyFile1_n = m_strSkyFile1;
		m_strSkyFile2_n = m_strSkyFile2;
		
		//	Read music file name
		pFile->ReadString(m_strMusicFile);

		//	Calculate area's priority
		m_iPriority = 1;

		return true;
	}

	//	pScene->GetBSDFileVersion() >= 6 ...
	ECBSDFILEBOXAREA17 Data;
	if (pScene->GetBSDFileVersion() < 8)
	{
		ECBSDFILEBOXAREA6 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.fSunPower			= 1.0f;
		Data.fSunPower_n		= 1.0f;
		Data.dwCloudAmb			= TempData.dwAmbient;
		Data.dwCloudDir			= TempData.dwDirLtCol;
		Data.fUWFogStart_n		= Data.fUWFogStart;
		Data.fUWFogEnd_n		= Data.fUWFogEnd;
		Data.fUWFogDensity_n	= Data.fUWFogDensity;
		Data.dwUWFogCol_n		= Data.dwUWFogCol;

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = -1000.0f;
		Data.fGlbFogHeightEnd = -900.0f;
		Data.fGlbFogDensityStart = 2.0f;
		Data.fGlbFogDensityEnd = 0.2f;
		Data.fGlbFogHeightStartNight = 0.0f;
		Data.fGlbFogHeightEndNight = 300.0f;
		Data.fGlbFogDensityStartNight = 2.0f;
		Data.fGlbFogDensityEndNight = 0.2f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(128, 128, 128);
		Data.fFogColorDensityStart = 2.0f;
		Data.fFogColorDensityEnd = 0.2f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(0, 0, 0);
		Data.fFogColorDensityStartNight = 2.0f;
		Data.fFogColorDensityEndNight = 0.2f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;

		Data.tmBorn.Clear();
		Data.tmDie.Clear();
	}
	else if (pScene->GetBSDFileVersion() < 9)
	{
		ECBSDFILEBOXAREA8 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.dwCloudAmb			= TempData.dwAmbient;
		Data.dwCloudDir			= TempData.dwDirLtCol;
		Data.fUWFogStart_n		= Data.fUWFogStart;
		Data.fUWFogEnd_n		= Data.fUWFogEnd;
		Data.fUWFogDensity_n	= Data.fUWFogDensity;
		Data.dwUWFogCol_n		= Data.dwUWFogCol;

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = 0.0f;
		Data.fGlbFogHeightEnd = 300.0f;
		Data.fGlbFogDensityStart = 2.0f;
		Data.fGlbFogDensityEnd = 0.2f;
		Data.fGlbFogHeightStartNight = 0.0f;
		Data.fGlbFogHeightEndNight = 300.0f;
		Data.fGlbFogDensityStartNight = 2.0f;
		Data.fGlbFogDensityEndNight = 0.2f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(128, 128, 128);
		Data.fFogColorDensityStart = 2.0f;
		Data.fFogColorDensityEnd = 0.2f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(0, 0, 0);
		Data.fFogColorDensityStartNight = 2.0f;
		Data.fFogColorDensityEndNight = 0.2f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;

		Data.tmBorn.Clear();
		Data.tmDie.Clear();
	}
	else if (pScene->GetBSDFileVersion() < 10)
	{
		ECBSDFILEBOXAREA9 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.dwCloudAmb_n		= Data.dwCloudAmb;
		Data.dwCloudDir_n		= Data.dwCloudDir;
		Data.fUWFogStart_n		= Data.fUWFogStart;
		Data.fUWFogEnd_n		= Data.fUWFogEnd;
		Data.fUWFogDensity_n	= Data.fUWFogDensity;
		Data.dwUWFogCol_n		= Data.dwUWFogCol;

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = 0.0f;
		Data.fGlbFogHeightEnd = 300.0f;
		Data.fGlbFogDensityStart = 2.0f;
		Data.fGlbFogDensityEnd = 0.2f;
		Data.fGlbFogHeightStartNight = 0.0f;
		Data.fGlbFogHeightEndNight = 300.0f;
		Data.fGlbFogDensityStartNight = 2.0f;
		Data.fGlbFogDensityEndNight = 0.2f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(128, 128, 128);
		Data.fFogColorDensityStart = 2.0f;
		Data.fFogColorDensityEnd = 0.2f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(0, 0, 0);
		Data.fFogColorDensityStartNight = 2.0f;
		Data.fFogColorDensityEndNight = 0.2f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;

		Data.tmBorn.Clear();
		Data.tmDie.Clear();
	}
	else if (pScene->GetBSDFileVersion() < 11)
	{
		ECBSDFILEBOXAREA10 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.fUWFogStart_n		= Data.fUWFogStart;
		Data.fUWFogEnd_n		= Data.fUWFogEnd;
		Data.fUWFogDensity_n	= Data.fUWFogDensity;
		Data.dwUWFogCol_n		= Data.dwUWFogCol;

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = 100.0f;
		Data.fGlbFogHeightEnd = 300.0f;
		Data.fGlbFogDensityStart = 2.0f;
		Data.fGlbFogDensityEnd = 1.2f;
		Data.fGlbFogHeightStartNight = 100.0f;
		Data.fGlbFogHeightEndNight = 300.0f;
		Data.fGlbFogDensityStartNight = 2.0f;
		Data.fGlbFogDensityEndNight = 1.2f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStart = 2.0f;
		Data.fFogColorDensityEnd = 1.2f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStartNight = 2.0f;
		Data.fFogColorDensityEndNight = 1.2f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;

		Data.tmBorn.Clear();
		Data.tmDie.Clear();
	}
	else if (pScene->GetBSDFileVersion() < 14)
	{
		ECBSDFILEBOXAREA11 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = 300.0f;
		Data.fGlbFogHeightEnd = 1300.0f;
		Data.fGlbFogDensityStart = 1.0f;
		Data.fGlbFogDensityEnd = 1.0f;
		Data.fGlbFogHeightStartNight = 300.0f;
		Data.fGlbFogHeightEndNight = 1300.0f;
		Data.fGlbFogDensityStartNight = 1.0f;
		Data.fGlbFogDensityEndNight = 1.0f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStart = 1.0f;
		Data.fFogColorDensityEnd = 1.0f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStartNight = 1.0f;
		Data.fFogColorDensityEndNight = 1.0f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;

		Data.tmBorn.Clear();
		Data.tmDie.Clear();
	}
	else if (pScene->GetBSDFileVersion() < 17)
	{
		ECBSDFILEBOXAREA14 TempData;
		if (!pFile->Read(&TempData, sizeof (TempData), &dwRead))
			return false;

		memcpy(&Data, &TempData, sizeof (TempData));

		Data.fCapTileSpeed = 8.0f;
		Data.fCapRotSpeed = 0.0f;

		Data.nPriority = 1;

		Data.fGlbFogHeightStart = 300.0f;
		Data.fGlbFogHeightEnd = 1300.0f;
		Data.fGlbFogDensityStart = 1.0f;
		Data.fGlbFogDensityEnd = 1.0f;
		Data.fGlbFogHeightStartNight = 300.0f;
		Data.fGlbFogHeightEndNight = 1300.0f;
		Data.fGlbFogDensityStartNight = 1.0f;
		Data.fGlbFogDensityEndNight = 1.0f;

		Data.dwAmbient2 = A3DCOLORRGB(128, 128, 128);
		Data.dwAmbient2_n = A3DCOLORRGB(0, 0, 0);

		Data.dwFogColor2 = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStart = 1.0f;
		Data.fFogColorDensityEnd = 1.0f;
		Data.fFogHorzeDensity = 1.0f;
		Data.fFogHorzePower = 1.0f;

		Data.dwFogColor2Night = A3DCOLORRGB(256, 256, 256);
		Data.fFogColorDensityStartNight = 1.0f;
		Data.fFogColorDensityEndNight = 1.0f;
		Data.fFogHorzeDensityNight = 1.0f;
		Data.fFogHorzePowerNight = 1.0f;
	}
	else	//	pScene->GetBSDFileVersion() >= 14
	{
		if (!pFile->Read(&Data, sizeof (Data), &dwRead))
			return false;
	}

	m_bDefArea			= Data.iDefArea ? true : false;
	m_dwTransTime		= Data.dwTransTime;
	m_dwAreaFlags		= Data.dwAreaFlags;

	m_fFogStart			= Data.fFogStart;
	m_fFogEnd			= Data.fFogEnd;
	m_fFogDensity		= Data.fFogDensity;
	m_dwFogColor		= Data.dwFogColor;
	m_fUWFogStart		= 1.0f;//Data.fUWFogStart; // 1.0f;
	m_fUWFogEnd			= 50.0f;//Data.fUWFogEnd; // 150.0f;
	m_fUWFogDensity		= Data.fUWFogDensity;
	m_dwUWFogColor		= A3DCOLORRGB(24, 62, 54); // Data.dwUWFogCol; //A3DCOLORRGB(23, 37, 47);
	m_fFogStart_n		= Data.fFogStart_n;
	m_fFogEnd_n			= Data.fFogEnd_n;
	m_fFogDensity_n		= Data.fFogDensity_n;
	m_dwFogColor_n		= Data.dwFogColor_n;
	
	m_dwAmbient			= Data.dwAmbient;
	m_dwAmbient2        = Data.dwAmbient2;
	m_dwDirLtCol		= Data.dwDirLtCol;
	m_dwAmbient_n		= Data.dwAmbient_n;
	m_dwDirLtCol_n		= Data.dwDirLtCol_n;
	m_fSunPower			= Data.fSunPower;
	m_fSunPower_n		= Data.fSunPower_n;
	m_dwCloudAmb		= Data.dwCloudAmb;
	m_dwCloudDirCol		= Data.dwCloudDir;
	m_dwCloudAmb_n		= Data.dwCloudAmb_n;
	m_dwCloudDirCol_n	= Data.dwCloudDir_n;
	m_fUWFogStart_n		= 1.0f;//Data.fUWFogStart_n; // 1.0f;
	m_fUWFogEnd_n		= 40.0f;//Data.fUWFogEnd_n; // 100.0f;
	m_fUWFogDensity_n	= Data.fUWFogDensity_n;
	m_dwUWFogColor_n	= A3DCOLORRGB(24, 62, 54); //Data.dwUWFogCol_n; //A3DCOLORRGB(23, 37, 47);

#ifndef BACK_VERSION
	m_fGlbFogHeightStart = Data.fGlbFogHeightStart;
	m_fGlbFogHeightEnd = Data.fGlbFogHeightEnd;
	m_fGlbFogDensityStart = Data.fGlbFogDensityStart;
	m_fGlbFogDensityEnd = Data.fGlbFogDensityEnd;
	m_fGlbFogHeightStartNight = Data.fGlbFogHeightStartNight;
	m_fGlbFogHeightEndNight = Data.fGlbFogHeightEndNight;
	m_fGlbFogDensityStartNight = Data.fGlbFogDensityStartNight;
	m_fGlbFogDensityEndNight = Data.fGlbFogDensityEndNight;

	m_dwFogColor2 = Data.dwFogColor2;
	m_fFogColorDensityStart = Data.fFogColorDensityStart;
	m_fFogColorDensityEnd = Data.fFogColorDensityEnd;
	m_fFogHorzeDensity = Data.fFogHorzeDensity;
	m_fFogHorzePower = Data.fFogHorzePower;

	m_dwFogColor2Night = Data.dwFogColor2Night;
	m_fFogColorDensityStartNight = Data.fFogColorDensityStartNight;
	m_fFogColorDensityEndNight = Data.fFogColorDensityEndNight;
	m_fFogHorzeDensityNight = Data.fFogHorzeDensityNight;
	m_fFogHorzePowerNight = Data.fFogHorzePowerNight;

	A3DCOLORVALUE testAmbient = A3DCOLORVALUE(m_dwAmbient);
	A3DCOLORVALUE testAmbientN = A3DCOLORVALUE(m_dwAmbient_n);
#endif

	m_fSkySpeedU	= Data.vSkySpeed[0];
	m_fSkySpeedV	= Data.vSkySpeed[1];

	m_vLightDir.Set(Data.vLightDir[0], Data.vLightDir[1], Data.vLightDir[2]);

	m_iRainSpeed	= Data.iRainSpeed;
	m_fRainSize		= Data.fRainSize;
	m_dwRainCol		= Data.dwRainCol;
	m_iSnowSpeed	= Data.iSnowSpeed;
	m_fSnowSize		= Data.fSnowSize;
	m_dwSnowCol		= Data.dwSnowCol;

	m_obb.Center.Set(Data.vPos[0]+fOffX, Data.vPos[1], Data.vPos[2]+fOffZ);
	m_obb.Extents.Set(Data.vExt[0], Data.vExt[1], Data.vExt[2]);
	m_obb.XAxis.Set(Data.vAxisX[0], Data.vAxisX[1], Data.vAxisX[2]);
	m_obb.YAxis.Set(Data.vAxisY[0], Data.vAxisY[1], Data.vAxisY[2]);
	m_obb.ZAxis.Set(Data.vAxisZ[0], Data.vAxisZ[1], Data.vAxisZ[2]);

	m_obb.CompleteExtAxis();

	m_tmBorn.iYear		= Data.tmBorn.iYear;
	m_tmBorn.iMouth		= Data.tmBorn.iMouth;
	m_tmBorn.iWeek		= Data.tmBorn.iWeek;
	m_tmBorn.iDay		= Data.tmBorn.iDay;
	m_tmBorn.iHour		= Data.tmBorn.iHour;
	m_tmBorn.iMinute	= Data.tmBorn.iMinute;
	m_tmBorn.CheckValid();

	m_tmDie.iYear		= Data.tmDie.iYear;
	m_tmDie.iMouth		= Data.tmDie.iMouth;
	m_tmDie.iWeek		= Data.tmDie.iWeek;
	m_tmDie.iDay		= Data.tmDie.iDay;
	m_tmDie.iHour		= Data.tmDie.iHour;
	m_tmDie.iMinute		= Data.tmDie.iMinute;
	m_tmDie.CheckValid();

	//	Read sky texture file names
	pFile->ReadString(m_strSkyFile0);
	pFile->ReadString(m_strSkyFile1);
	pFile->ReadString(m_strSkyFile2);
	pFile->ReadString(m_strSkyFile0_n);
	pFile->ReadString(m_strSkyFile1_n);
	pFile->ReadString(m_strSkyFile2_n);
	
	//	Read music file name
	pFile->ReadString(m_strMusicFile);

	if (pScene->GetBSDFileVersion() > 14)
	{
		// read sky type
		pFile->Read(&m_nSkyType, sizeof(m_nSkyType), &dwRead);

		//	Read skybox texture file names
		pFile->ReadString(m_strSkyTop);
		pFile->ReadString(m_strSkyBottom);
		pFile->ReadString(m_strSkyLeft);
		pFile->ReadString(m_strSkyRight);
		pFile->ReadString(m_strSkyFront);
		pFile->ReadString(m_strSkyRight);

		pFile->ReadString(m_strSkyNightTop);
		pFile->ReadString(m_strSkyNightBottom);
		pFile->ReadString(m_strSkyNightLeft);
		pFile->ReadString(m_strSkyNightRight);
		pFile->ReadString(m_strSkyNightFront);
		pFile->ReadString(m_strSkyNightRight);
	}

	if( m_fFogEnd < m_fFogStart + 1.0f )
		m_fFogEnd = m_fFogStart + 1.0f;
	if( m_fFogEnd_n < m_fFogStart_n + 1.0f )
		m_fFogEnd_n = m_fFogStart_n + 1.0f;
	if( m_fUWFogStart > 400.0f )
		m_fUWFogStart = 400.0f;
	if( m_fUWFogEnd < m_fUWFogStart + 1.0f )
		m_fUWFogEnd = m_fUWFogStart + 1.0f;
	if( m_fUWFogStart_n > 400.0f )
		m_fUWFogStart_n = 400.0f;
	if( m_fUWFogEnd_n < m_fUWFogStart_n + 1.0f )
		m_fUWFogEnd_n = m_fUWFogStart_n + 1.0f;

	//	Calculate area's priority
	CalcPriority();

	return true;
}

//	Calculate box area's priority
void CECScnBoxArea::CalcPriority()
{
	if (!m_tmBorn.bValid && !m_tmDie.bValid)
		m_iPriority = 1;
	else
		m_iPriority = 2;
}

//	Check whethat area is active in specified time
bool CECScnBoxArea::IsInActiveTime()
{
	//	Area is always active if it's a default area
	if (m_bDefArea)
		return true;

	tm time = g_pGame->GetServerLocalTime();

	//	Check born time
	if (m_tmBorn.bValid)
	{
		int d = CompareTime(m_tmBorn, time);
		if (d > 0)
			return false;
	}

	if (!m_tmDie.bValid)
		return true;	//	Don't set die time

	//	Check die time
	int d = CompareTime(m_tmDie, time);
	return d >= 0 ? true : false;
}

//	Compare time
int CECScnBoxArea::CompareTime(const TIMEINFO& t1, const tm& t2)
{
	int d1, d2, d3;

	d1 = t1.iYear - t2.tm_year;
	d2 = t1.iMouth - t2.tm_mon;

	if (t1.iWeek >= 0)
		d3 = t1.iWeek - t2.tm_wday;
	else
		d3 = t1.iDay - t2.tm_mday;

	if (!d1 && t1.iYear != -1)
	{
		if (!d2 && t1.iMouth != -1)
		{
			if (d3 && t1.iDay != -1)
			{
				return d3 < 0 ? -1 : 1;
			}
		}
		else
		{
			return d2 < 0 ? -1 : 1;
		}
	}
	else
	{
		return d1 < 0 ? -1 : 1;
	}

	//	Check hour/minute
	d1 = t1.iHour - t2.tm_hour;
	d2 = t1.iMinute - t2.tm_min;

	if (!d1 && t1.iHour != -1)
	{
		if (d2 && t1.iMinute != -1)
			return d2 < 0 ? -1 : 1;
		else
			return 0;
	}
	else
	{
		return d1 < 0 ? -1 : 1;
	}
}
