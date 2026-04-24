/*
 * FILE: EC_ScnBoxArea.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/10/20
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "..\\ZElementClient\\EC_SceneObject.h"
#include <A3DGeometry.h>
#include <AString.h>
#include <A3DTypes.h>
#include <time.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class AFile;
class CECScene;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECScnBoxArea
//	
///////////////////////////////////////////////////////////////////////////

class CECScnBoxArea : public CECSceneObject
{
public:		//	Types

	//	Area flags
	enum
	{
		FLAG_DIRLIGHT	= 0x0001,		//	Area has it's own directional light
		FLAG_RAIN		= 0x0002,		//	Area has snow effect
		FLAG_SNOW		= 0x0004,		//	Area has rain effect
		FLAG_SUN		= 0x0008,		//	Area has sun
		FLAG_MOON		= 0x0010,		//	Area has moon
	};

	//	Time info
	struct TIMEINFO
	{
		int		iYear;		//	正常区间 [1900-2100] -1 代表任意
		int		iMouth;		//	正常取值区间[0,11]  -1 代表任意
		int		iWeek;		//	正常取值区间[0,6]  0 代表星期日 -1 代表任意
		int		iDay;		//	正常取值区间[1,31]  -1 代表任意
		int		iHour;		//	正常取值区间[0,23]  -1 代表任意
		int		iMinute;	//	正常取值区间[0,59]

		bool	bValid;		//	true, time is valid

		void Clear()
		{ 
			iYear = iMouth = iWeek = iDay = iHour = iMinute = -1;
			bValid = false;
		}

		void CheckValid()
		{
			int t = iYear & iMouth & iWeek & iDay & iHour & iMinute;
			bValid = (t != -1);
		}
	};

public:		//	Constructor and Destructor

	CECScnBoxArea();
	virtual ~CECScnBoxArea();

public:		//	Attributes

public:		//	Operations

	//	Load data from file
	bool Load(CECScene* pScene, AFile* pFile, float fOffX, float fOffZ);

	//	Check whethat area is active in specified time
	bool IsInActiveTime();

	//	Set area ID
	void SetAreaID(DWORD dwID) { m_dwAreaID = dwID; }
	//	Get area ID
	DWORD GetAreaID() { return m_dwAreaID; }
	//	Get area flags
	DWORD GetAreaFlags() { return m_dwAreaFlags; }
	//	Get box area's priority
	int GetPriority() { return m_iPriority; }

	//	Is default area
	bool IsDefaultArea() { return m_bDefArea; }
	//	Get area box
	const A3DOBB& GetBox() { return m_obb; }
	//	Get fog parameters in daylight
	float GetFogStart() { return m_fFogStart; }
	float GetFogEnd() { return m_fFogEnd; }
	float GetFogDensity() { return m_fFogDensity; }
	DWORD GetFogColor() { return m_dwFogColor; }
	//	Get under water fog parameters in daylight
	float GetUWFogStart() { return m_fUWFogStart; }
	float GetUWFogEnd() { return m_fUWFogEnd; }
	float GetUWFogDensity() { return m_fUWFogDensity; }
	DWORD GetUWFogColor() { return m_dwUWFogColor; }
	//	Get under water fog parameters in night
	float GetUWFogStart_n() { return m_fUWFogStart_n; }
	float GetUWFogEnd_n() { return m_fUWFogEnd_n; }
	float GetUWFogDensity_n() { return m_fUWFogDensity_n; }
	DWORD GetUWFogColor_n() { return m_dwUWFogColor_n; }
	//	Get fog parameters in night
	float GetFogStart_n() { return m_fFogStart_n; }
	float GetFogEnd_n() { return m_fFogEnd_n; }
	float GetFogDensity_n() { return m_fFogDensity_n; }
	DWORD GetFogColor_n() { return m_dwFogColor_n; }
#ifndef BACK_VERSION
	//  Get Vol for paramters in daylight
	float GetGlbFogHeightStart() { return m_fGlbFogHeightStart; }
	float GetGlbFogHeightEnd() { return m_fGlbFogHeightEnd; }
	float GetGlbFogDensityStart() { return m_fGlbFogDensityStart; }
	float GetGlbFogDensityEnd() { return m_fGlbFogDensityEnd; }
	//  Get Vol for paramters in night
	float GetGlbFogHeightStart_n() { return m_fGlbFogHeightStartNight; }
	float GetGlbFogHeightEnd_n() { return m_fGlbFogHeightEndNight; }
	float GetGlbFogDensityStart_n() { return m_fGlbFogDensityStartNight; }
	float GetGlbFogDensityEnd_n() { return m_fGlbFogDensityEndNight; }

	DWORD GetFogColor2() { return m_dwFogColor2; }
	float GetFogColorDensityStart() { return m_fFogColorDensityStart; }
	float GetFogColorDensityEnd() { return m_fFogColorDensityEnd; }
	float GetFogHorzeDensity() { return m_fFogHorzeDensity; }
	float GetFogHorzePower() { return m_fFogHorzePower; }

	DWORD GetFogColor2Night() { return m_dwFogColor2Night; }
	float GetFogColorDensityStartNight() { return m_fFogColorDensityStartNight; }
	float GetFogColorDensityEndNight() { return m_fFogColorDensityEndNight; }
	float GetFogHorzeDensityNight() { return m_fFogHorzeDensityNight; }
	float GetFogHorzePowerNight() { return m_fFogHorzePowerNight; }
#endif
	//	Get light parameters in daylight
	DWORD GetAmbient() { return m_dwAmbient; }
	DWORD GetAmbient2() { return m_dwAmbient2; }
	DWORD GetDirLightColor() { return m_dwDirLtCol; }
	//	Get light parameters in night
	DWORD GetAmbient_n() { return m_dwAmbient_n; }
	DWORD GetDirLightColor_n() { return m_dwDirLtCol_n; }
	//	Get sun power
	float GetSunPower() { return m_fSunPower; }
	float GetSunPower_n() { return m_fSunPower_n; }
	//	Get light direction
	bool HasDirLight() { return (m_dwAreaFlags & FLAG_DIRLIGHT) ? true : false; }
	const A3DVECTOR3 GetLightDir() { return m_vLightDir; }
	//	Get sky move speed
	float GetSkySpeedU() { return m_fSkySpeedU; }
	float GetSkySpeedV() { return m_fSkySpeedV; }
	int	GetSkyType() { return m_nSkyType; }
	//	Get sky files in daylight
	const char* GetSkyFile0() { return m_strSkyFile0; }
	const char* GetSkyFile1() { return m_strSkyFile1; }
	const char* GetSkyFile2() { return m_strSkyFile2; }
	//	Get sky files in night
	const char* GetSkyFile0_n() { return m_strSkyFile0_n; }
	const char* GetSkyFile1_n() { return m_strSkyFile1_n; }
	const char* GetSkyFile2_n() { return m_strSkyFile2_n; }

	//	Get skybox files in daylight
	const char* GetSkyBoxRight() { return m_strSkyRight; }
	const char* GetSkyBoxLeft() { return m_strSkyLeft; }
	const char* GetSkyBoxTop() { return m_strSkyTop; }
	const char* GetSkyBoxBottom() { return m_strSkyBottom; }
	const char* GetSkyBoxFront() { return m_strSkyFront; }
	const char* GetSkyBoxBack() { return m_strSkyBack; }

	//	Get transform time in ms
	DWORD GetTransformTime() { return m_dwTransTime; }
	//	Get music file
	const char* GetMusicFile() { return m_strMusicFile; }
	//	Get rain properties
	bool HasRain() { return (m_dwAreaFlags & FLAG_RAIN) ? true : false; }
	int GetRainSpeed() { return m_iRainSpeed; }
	float GetRainSize() { return m_fRainSize; }
	DWORD GetRainColor() { return m_dwRainCol; }
	//	Get snow properties
	bool HasSnow() { return (m_dwAreaFlags & FLAG_SNOW) ? true : false; }
	int GetSnowSpeed() { return m_iSnowSpeed; }
	float GetSnowSize() { return m_fSnowSize; }
	DWORD GetSnowColor() { return m_dwSnowCol; }
	//	Get cloud color
	DWORD GetCloudAmb() { return m_dwCloudAmb; }
	DWORD GetCloudDirCol() { return m_dwCloudDirCol; }
	DWORD GetCloudAmb_n() { return m_dwCloudAmb_n; }
	DWORD GetCloudDirCol_n() { return m_dwCloudDirCol_n; }
	//	Sun / moon flag
	bool HasSun() { return (m_dwAreaFlags & FLAG_SUN) ? true : false; }
	bool HasMoon() { return (m_dwAreaFlags & FLAG_MOON) ? true : false; }
	//	Get area born time
	const TIMEINFO& GetBornTime() { return m_tmBorn; }
	//	Get area die time
	const TIMEINFO& GetDieTime() { return m_tmDie; }

protected:	//	Attributes

	DWORD		m_dwAreaID;			//	Area ID
	bool		m_bDefArea;			//	true, is default area
	A3DOBB		m_obb;				//	Area box
	DWORD		m_dwTransTime;		//	Transform time
	DWORD		m_dwAreaFlags;		//	Area flags
	int			m_iPriority;		//	Area's priority

	float		m_fFogStart;		//	Fog start distance in daylight
	float		m_fFogEnd;			//	Fog end distance in daylight
	float		m_fFogDensity;		//	Fog density in daylight
	DWORD		m_dwFogColor;		//	Fog color in daylight
	float		m_fUWFogStart;		//	Under water fog start in daylight
	float		m_fUWFogEnd;		//	Under water fog end in daylight
	float		m_fUWFogDensity;	//	Under water fog density in daylight
	DWORD		m_dwUWFogColor;		//	Under water fog color in daylight
	float		m_fFogStart_n;		//	Fog start distance in night
	float		m_fFogEnd_n;		//	Fog end distance in night
	float		m_fFogDensity_n;	//	Fog density in night
	DWORD		m_dwFogColor_n;		//	Fog color in night
	float		m_fUWFogStart_n;	//	Under water fog start in night
	float		m_fUWFogEnd_n;		//	Under water fog end in night
	float		m_fUWFogDensity_n;	//	Under water fog density in night
	DWORD		m_dwUWFogColor_n;	//	Under water fog color in night

	A3DVECTOR3	m_vLightDir;		//	Light direction
	DWORD		m_dwAmbient;		//	Ambient color in daylight
	DWORD		m_dwAmbient2;		//	Ambient color in daylight
	DWORD		m_dwDirLtCol;		//	Directional light color in daylight
	DWORD		m_dwAmbient_n;		//	Ambient color in night
	DWORD		m_dwDirLtCol_n;		//	Directional light color in night
	float		m_fSunPower;		//	Sun power
	float		m_fSunPower_n;
	DWORD		m_dwCloudAmb;		//	Cloud ambient color
	DWORD		m_dwCloudAmb_n;		//	Cloud ambient color at night
	DWORD		m_dwCloudDirCol;	//	Cloud directional color
	DWORD		m_dwCloudDirCol_n;	//	Cloud directional color at night

#ifndef BACK_VERSION
	float       m_fGlbFogHeightStart;
	float       m_fGlbFogHeightEnd;
	float       m_fGlbFogDensityStart;
	float       m_fGlbFogDensityEnd;
	float       m_fGlbFogHeightStartNight;
	float       m_fGlbFogHeightEndNight;
	float       m_fGlbFogDensityStartNight;
	float       m_fGlbFogDensityEndNight;

	DWORD       m_dwFogColor2;
	float       m_fFogColorDensityStart;
	float       m_fFogColorDensityEnd;
	float       m_fFogHorzeDensity;
	float       m_fFogHorzePower;

	DWORD       m_dwFogColor2Night;
	float       m_fFogColorDensityStartNight;
	float       m_fFogColorDensityEndNight;
	float       m_fFogHorzeDensityNight;
	float       m_fFogHorzePowerNight;
#endif

	float		m_fSkySpeedU;		//	Sky moving speed on U axis
	float		m_fSkySpeedV;		//	Sky moving speed on V axis
	AString		m_strSkyFile0;		//	Sphere sky texture 0 in daylight
	AString		m_strSkyFile1;		//	Sphere sky texture 1 in daylight
	AString		m_strSkyFile2;		//	Sphere sky texture 2 in daylight
	AString		m_strSkyFile0_n;	//	Sphere sky texture 0 in night
	AString		m_strSkyFile1_n;	//	Sphere sky texture 1 in night
	AString		m_strSkyFile2_n;	//	Sphere sky texture 2 in night
	AString		m_strMusicFile;		//	Music file name

	//sky box textures
	AString		m_strSkyTop;
	AString		m_strSkyBottom;
	AString		m_strSkyLeft;
	AString		m_strSkyRight;
	AString		m_strSkyFront;
	AString		m_strSkyBack;
	
	// 暂不生效，需要引擎添加支持
	AString		m_strSkyNightTop;
	AString		m_strSkyNightBottom;
	AString		m_strSkyNightLeft;
	AString		m_strSkyNightRight;
	AString		m_strSkyNightFront;
	AString		m_strSkyNightBack;

	int			m_nSkyType;		// Sky type, 0: skysphere, 1:skybox


	int			m_iRainSpeed;		//	Rain properties
	float		m_fRainSize;
	DWORD		m_dwRainCol;
	int			m_iSnowSpeed;		//	Snow properties
	float		m_fSnowSize;
	DWORD		m_dwSnowCol;

	TIMEINFO	m_tmBorn;			//	Area active time range
	TIMEINFO	m_tmDie;

protected:	//	Operations

	//	Calculate box area's priority
	void CalcPriority();
	//	Compare time
	int CompareTime(const TIMEINFO& t1, const tm& t2);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

