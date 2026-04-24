/*
 * FILE: LightMapMergerB.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/23
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AArray.h"
#include "A3DTerrain2File.h"

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

class CELBitmap;
class CLightMapMerger;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CLightMapMergerB
//	
///////////////////////////////////////////////////////////////////////////

class CLightMapMergerB
{
public:		//	Types
	enum
	{
		LM_DAY = 0,
		LM_NIGHT,
		LM_MANUAL, 
	};
public:		//	Constructor and Destructor

	CLightMapMergerB();
	virtual ~CLightMapMergerB();

public:		//	Attributes

public:		//	Operations

	//	Do export
	bool DoExport(CLightMapMerger* pMerger,  int iLMType);

protected:	//	Attributes

	CLightMapMerger*	m_pm;

	CELBitmap*	m_pLMBmp;			//	Light map bitmap
	int			m_iLMSize;			//	Sub-terrain Light map width and height in pixels
	BYTE*		m_pLMBuf;			//	Light map buffer
	int			m_iMaskLMSize;		//	Width and height of mask area lightmap
	int			m_iLMType;			//  0:light map for day; 1: lightmap for night; 2: light map for vertex color? .t2dm

	int			m_iProjectCnt;		//	Export project counter
	int			m_iProjRowCnt;		//	Project row counter
	int			m_iProjColCnt;		//	Project column counter
	DWORD*		m_aLMOffsets;		//	Offset of mask area lightmaps

protected:	//	Operations

	//	Allocate exporting resources
	bool AllocExportRes();
	//	Release resources used during exporting terrain
	void ReleaseExportRes();

	//	Export lightmap
	bool ExportLightmaps();
	//	Export lightmap data of a project
	bool ExportProject(int iProject);
	//	Generate project lightmap
	bool GenProjectLightmap(int iProject);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


