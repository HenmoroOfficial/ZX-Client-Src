/*
 * FILE: EC_Instance.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/8/15
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AString.h"
#include "AAssist.h"
#include <A3DTypes.h>
#include <vector.h>

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

class AWScriptFile;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECInstance
//	
///////////////////////////////////////////////////////////////////////////

class CECInstance
{
public:		//	Types

public:		//	Constructor and Destructor

	CECInstance();

public:		//	Attributes
	struct InstanceInfo
	{
		int nId;
		int nLeft;
		int nRight;
		int nBottom;
		int nTop;
		AString strPathPost;
		ACString strName;
	};
	typedef abase::vector<InstanceInfo> vecInstanceInfo;

public:		//	Operations

	//	Load instance information from file
	bool Load(AWScriptFile* psf);

	//	Get instance ID
	int GetID() const { return m_id; }
	//	Get zone id
	int GetZoneID() const { return m_idZone; }
	//	Get instance name
	const ACHAR* GetName() const { return m_strName; }
	const ACHAR* GetNameByCoord(float x, float z);
	const char*  GetSubPath(float x, float z);
	//	Get instance data path
	const char* GetPath() const { return m_strPath; }
	//	Get instance data path
	const char* GetDataPath() const { return m_strDataPath; }
	//	Get detail texture map
	const char* GetDetailTex() const { return m_strDetailTex; }

	//	Get row and column number of map
	int GetRowNum() { return m_iRowNum; }
	int GetColNum() { return m_iColNum; }
	A3DCOLOR GetColorWater() { return m_colorWater; }
	float GetMaxFlyHeight() const { return m_fMaxFlyHeight; }

protected:	//	Attributes

	int			m_id;		//	Instance ID
	int			m_idZone;	//	Zone Id;
	ACString	m_strName;	//	Instance name
	AString		m_strPath;	//	Path
	AString		m_strDataPath;	//	Data Path
	AString		m_strDetailTex; // Detail texture
	int			m_iRowNum;	//	Number of map row
	int			m_iColNum;	//	Number of map column
	A3DCOLOR	m_colorWater; // color of water surface in this map
	float		m_fMaxFlyHeight;
	vecInstanceInfo m_vecInstInfo;

protected:	//	Operations

};


float glb_GetAirDHeight(int instance);

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

