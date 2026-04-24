/*
 * FILE: EC_Instance.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/8/15
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Instance.h"
#include "AWScriptFile.h"
#include <A3DMacros.h>

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
//	Implement CECInstance
//	
///////////////////////////////////////////////////////////////////////////

CECInstance::CECInstance()
{
	m_id		= 0;
	m_iRowNum	= 0;
	m_iColNum	= 0;
	m_colorWater = 0;
	m_fMaxFlyHeight = 0;
	m_vecInstInfo.clear();
}

//	Load instance information from file
bool CECInstance::Load(AWScriptFile* psf)
{
	//	Read name
	if (!psf->GetNextToken(true))
		return false;

	m_strName = psf->m_szToken;

	if (!psf->MatchToken(_AL("{"), true))
		return false;

	//	Read ID
	m_id = psf->GetNextTokenAsInt(true);

	//	Read Zone ID
	m_idZone = psf->GetNextTokenAsInt(true);

	//	Read path
	if (!psf->GetNextToken(true))
		return false;
	m_strPath = WC2AS(psf->m_szToken);
	
	//	Read data path
	if (!psf->GetNextToken(true))
		return false;
	m_strDataPath = WC2AS(psf->m_szToken);

	//	Read detail texture
	if (!psf->GetNextToken(true))
		return false;
	m_strDetailTex = WC2AS(psf->m_szToken);
	
	//	Read row and column number of map
	m_iRowNum = psf->GetNextTokenAsInt(true);
	m_iColNum = psf->GetNextTokenAsInt(false);

	//	Read water color
	int r = psf->GetNextTokenAsInt(true);
	int g = psf->GetNextTokenAsInt(true);
	int b = psf->GetNextTokenAsInt(false);
	m_colorWater = A3DCOLORRGB(r, g, b);

	m_fMaxFlyHeight = psf->GetNextTokenAsFloat(true);

	psf->GetNextToken(true);
	if (wcsicmp(psf->m_szToken, _AL("}"))==0)
		return true;

	int num = _wtoi(psf->m_szToken);
	for (int i=0; i<num; i++)
	{
		InstanceInfo info;
		info.nLeft = psf->GetNextTokenAsInt(true);
		info.nRight = psf->GetNextTokenAsInt(true);
		info.nBottom = psf->GetNextTokenAsInt(true);
		info.nTop = psf->GetNextTokenAsInt(true);
		psf->GetNextToken(true);
		info.strPathPost = AC2AS(psf->m_szToken);
		psf->GetNextToken(true);
		info.strName = psf->m_szToken;
		m_vecInstInfo.push_back(info);
	}

	if (!psf->MatchToken(_AL("}"), true))
		return false;

	return true;
}

const ACHAR* CECInstance::GetNameByCoord(float x, float z)
{
	for (unsigned int i=0; i<m_vecInstInfo.size(); i++)
	{
		InstanceInfo info = m_vecInstInfo[i];
		if ( x>info.nLeft && x<info.nRight && z>info.nBottom && z<info.nTop)
		{
			return info.strName;
		}
	}
	return m_strName;
}

const char*  CECInstance::GetSubPath(float x, float z)
{
	for (unsigned int i=0; i<m_vecInstInfo.size(); i++)
	{
		InstanceInfo info = m_vecInstInfo[i];
		if ( x>info.nLeft && x<info.nRight && z>info.nBottom && z<info.nTop)
		{
			return info.strPathPost;
		}
	}
	return "";
	
}

float  glb_GetAirDHeight(int instance)
{
	// todo,  refine me [4/14/2008 kuiwu]

	return 100.0f;
}