/*
 * FILE: PrecinctData.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#ifndef _PRECINCTDATA_H_
#define _PRECINCTDATA_H_

#include <stdio.h>
#include "AAssist.h"
#include "AArray.h"

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
class AMemFile;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CPrecinctData
//	
///////////////////////////////////////////////////////////////////////////

class CPrecinctData
{
public:		//	Types

	//	Music loop type
	enum
	{
		LOOP_NONE = 0,
		LOOP_WHOLE,
		LOOP_SKIPFIRST,
	};

	struct VECTOR3
	{
		float x, y, z;
	};

	struct MARK
	{
		int		iNameIdx;
		VECTOR3	vPos;
	};

	friend class CPrecinctDataSet;

public:		//	Constructor and Destructor

	CPrecinctData(CPrecinctDataSet* pDataSet);
	virtual ~CPrecinctData();

public:		//	Attributes

public:		//	Operations

protected:	//	Attributes

	CPrecinctDataSet*	m_pDataSet;

	int					m_iNameIdx;
	APtrArray<MARK*>	m_aMarks;
	ACString			m_strSound;
	ACString			m_strSound_n;
	APtrArray<ACString*>m_aMusicFiles;
	DWORD 				m_dwID;
	int					m_iMusicInter;		//	Music interval
	int					m_iMusicLoop;		//	Music loop
	int					m_iFeishengCnt;
	int					m_iAllowLevelMax;
	int					m_iAllowLevelMin;
	int					m_iKickCity;

	bool				m_bNightSFX;		//	flag indicates current night sfx is activated

	int		m_idDstInst;	//	ID of instance m_vCityPos belongs to
	int		m_idSrcInst;	//	ID of source instance
	int		m_iPriority;	//	Precinct priority
	VECTOR3	m_vCityPos;		//	City position
	VECTOR3	m_vKickCityPos;		//	Kick city position
	int		m_idDomain;		//	ID of domain
	
	AArray<VECTOR3, VECTOR3&>	m_aPoints;	//	Precinct points

protected:	//	Operations

	//	Load data from file
	bool Load(AWScriptFile* pFile, int iVersion);
	//	Save data to file
	bool Save(FILE* fp);
	bool Save(AMemFile& file);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CPrecinctDataSet
//	
///////////////////////////////////////////////////////////////////////////

class CPrecinctDataSet
{
public:		//	Types

	friend class CPrecinctData;

public:		//	Constructor and Destructor

	CPrecinctDataSet();
	virtual ~CPrecinctDataSet();

public:		//	Attributes

public:		//	Operations

	//	Load data from file
	bool Load(const char* szFileName);
	//	Save data to file
	bool Save(const char* szFileName);

	//	Get string text number
	int GetTextNum() { return m_aStrings.GetSize(); }
	//	Get string from table
	const ACHAR* GetTextByIndex(int iIndex);
	//	Change text content
	void SetTextByIndex(int iIndex, const ACHAR* szText);
	//	Find string in table
	int FindText(const ACHAR* szText);

protected:	//	Attributes

	DWORD	m_dwTimeStamp;		//	Time stamp of precinct data

	APtrArray<CPrecinctData*>	m_aPrecincts;	//	Precinct array
	APtrArray<ACString*>		m_aStrings;		//	String table

protected:	//	Operations

	//	Add a string to string table
	int AddText(const ACHAR* szText);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#endif	//	_PRECINCTDATA_H_
