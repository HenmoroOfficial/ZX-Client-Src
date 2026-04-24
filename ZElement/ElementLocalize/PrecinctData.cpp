/*
 * FILE: EL_Precinct.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/3/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "stdafx.h"
#include "el_precinct.h"
#include "PrecinctData.h"
#include <memory.h>
#include "AWScriptFile.h"
#include "AMemFile.h"
#include "AFileImage.h"
#include "AChar.h"

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

static int _StrLenInByte(const wchar_t* sz)
{
	return wcslen(sz) * sizeof (wchar_t);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CPrecinctData
//	
///////////////////////////////////////////////////////////////////////////

CPrecinctData::CPrecinctData(CPrecinctDataSet* pDataSet)
{
	m_pDataSet		= pDataSet;
	m_iPriority		= 0;
	m_idDstInst		= 0;
	m_idSrcInst		= 0;
	m_idDomain		= 0;
	m_iNameIdx		= -1;
	
	m_vCityPos.x 	= 0.0f;
	m_vCityPos.y 	= 0.0f;
	m_vCityPos.z	= 0.0f;
	
	m_dwID			= 0;
	m_iMusicInter	= 0;
	m_iMusicLoop	= 0;
	m_bNightSFX		= false;
}

CPrecinctData::~CPrecinctData()
{
	int i;

	for (i=0; i < m_aMarks.GetSize(); i++)
		delete m_aMarks[i];

	for (i=0; i < m_aMusicFiles.GetSize(); i++)
		delete m_aMusicFiles[i];
}

bool CPrecinctData::Load(AWScriptFile* pFile, int iVersion)
{
	//	Read name
	pFile->GetNextToken(true);
	m_iNameIdx = m_pDataSet->AddText(pFile->m_szToken);

	int iNumPoint;

	if (iVersion < 2)
	{
		m_dwID = 0;

		//	Read point number
		iNumPoint = pFile->GetNextTokenAsInt(true);
	}
	else	//	dwVersion >= 2
	{
		//	Read precinct ID
		m_dwID = (unsigned int)pFile->GetNextTokenAsInt(true);
		//	Read point number
		iNumPoint = pFile->GetNextTokenAsInt(false);
	}

	//	Read mark number
	int iNumMark = pFile->GetNextTokenAsInt(false);
	//	Read priority
	m_iPriority = pFile->GetNextTokenAsInt(false);
	//	Read destination instance ID
	m_idDstInst = pFile->GetNextTokenAsInt(false);
	//	Read music file number
	int iNumMusic = pFile->GetNextTokenAsInt(false);
	//	Read music interval
	m_iMusicInter = pFile->GetNextTokenAsInt(false);
	//	Read music loop type
	m_iMusicLoop = pFile->GetNextTokenAsInt(false);

	//	Source instance ID
	if (iVersion >= 4)
		m_idSrcInst = pFile->GetNextTokenAsInt(false);
	else
		m_idSrcInst = 1;

	//	ID of domain
	if (iVersion >= 6)
		m_idDomain = pFile->GetNextTokenAsInt(false);
	else
		m_idDomain = 0;

	//	Read city position
	m_vCityPos.x = pFile->GetNextTokenAsFloat(true);
	m_vCityPos.y = pFile->GetNextTokenAsFloat(false);
	m_vCityPos.z = pFile->GetNextTokenAsFloat(false);

	int i;
	//////////////////////////////////////////////////////////////////////////
	// version 7
	if(iVersion>=7)
	{
		m_iFeishengCnt = pFile->GetNextTokenAsInt(true);
		m_iAllowLevelMax = pFile->GetNextTokenAsInt(false);
		m_iAllowLevelMin = pFile->GetNextTokenAsInt(false);
		m_iKickCity = pFile->GetNextTokenAsInt(false);

		m_vKickCityPos.x = pFile->GetNextTokenAsFloat(true);
		m_vKickCityPos.y = pFile->GetNextTokenAsFloat(false);
		m_vKickCityPos.z = pFile->GetNextTokenAsFloat(false);
	}	
	//////////////////////////////////////////////////////////////////////////


	//	Read vertices ...
	for (i=0; i < iNumPoint; i++)
	{
		VECTOR3 v;

		//	Get position
		v.x = pFile->GetNextTokenAsFloat(true);
		v.y = pFile->GetNextTokenAsFloat(false);
		v.z = pFile->GetNextTokenAsFloat(false);
		
		m_aPoints.Add(v);
	}

	//	Read mark ...
	for (i=0; i < iNumMark; i++)
	{
		MARK* pMark = new MARK;

		pFile->GetNextToken(true);

		pMark->iNameIdx = m_pDataSet->AddText(pFile->m_szToken);

		pMark->vPos.x = pFile->GetNextTokenAsFloat(false);
		pMark->vPos.y = pFile->GetNextTokenAsFloat(false);
		pMark->vPos.z = pFile->GetNextTokenAsFloat(false);

		m_aMarks.Add(pMark);
	}

	//	Read sound file
	pFile->GetNextToken(true);
	m_strSound = pFile->m_szToken;

	//	Read music files ...
	for (i=0; i < iNumMusic; i++)
	{
		if (!pFile->GetNextToken(true))
		{
			ASSERT(0);
			return false;
		}

		ACString* pstr = new ACString(pFile->m_szToken);
		m_aMusicFiles.Add(pstr);
	}

	//	Read sound file at night
	if (iVersion >= 3)
	{
		pFile->GetNextToken(true);
		m_strSound_n = pFile->m_szToken;
	}

	return true;
}

//	Save data to file
bool CPrecinctData::Save(FILE* fp)
{
	//	Write precinct name
	const ACHAR* szName = m_pDataSet->GetTextByIndex(m_iNameIdx);
	if (!szName || !szName[0])
		szName = L"Unknown Area";

	fwprintf(fp, L"\"%s\"\n", szName);

	//	Write properties
	fwprintf(fp, L"%d  %d  %d  %d  %d  %d  %d  %d  %d  %d\n", 
			m_dwID, 
			m_aPoints.GetSize(), 
			m_aMarks.GetSize(), 
			m_iPriority, 
			m_idDstInst, 
			m_aMusicFiles.GetSize(),
			m_iMusicInter,
			m_iMusicLoop,
			m_idSrcInst,
			m_idDomain);

	fwprintf(fp, L"%f, %f, %f\n", m_vCityPos.x, m_vCityPos.y, m_vCityPos.z);

	fwprintf(fp, L"%d %d %d %d\n", m_iFeishengCnt, m_iAllowLevelMax, m_iAllowLevelMin, m_iKickCity);
	fwprintf(fp, L"%f, %f, %f\n", m_vKickCityPos.x, m_vKickCityPos.y, m_vKickCityPos.z);

	int i;

	//	Write point positions
	for (i=0; i < m_aPoints.GetSize(); i++)
	{
		const VECTOR3& v = m_aPoints[i];
		fwprintf(fp, L"%f  %f  %f\n", v.x, v.y, v.z);
	}

	fwprintf(fp, L"\n");

	//	Write marks
	for (i=0; i < m_aMarks.GetSize(); i++)
	{
		const MARK* pMark = m_aMarks[i];

		const ACHAR* szMarkName = m_pDataSet->GetTextByIndex(pMark->iNameIdx);
		if (!szName || !szName[0])
			szMarkName = L"Unknown Mark";

		const VECTOR3& v = pMark->vPos;

		fwprintf(fp, L"\"%s\"  %f  %f  %f\n", szMarkName, v.x, v.y, v.z);
	}

	fwprintf(fp, L"\n");

	//	Save music and sound files
	fwprintf(fp, L"\"%s\"\n", m_strSound);

	for (i=0; i < m_aMusicFiles.GetSize(); i++)
		fwprintf(fp, L"\"%s\"\n", *m_aMusicFiles[i]);

	fwprintf(fp, L"\"%s\"\n", m_strSound_n);
	
	fwprintf(fp, L"\n");

	return true;
}

bool CPrecinctData::Save(AMemFile& file)
{
	DWORD dwWrite;
	wchar_t sz[1024];

	//	Write precinct name
	const ACHAR* szName = m_pDataSet->GetTextByIndex(m_iNameIdx);
	if (!szName || !szName[0])
		szName = L"Unknown Area";

	swprintf(sz, L"\"%s\"\n", szName);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write properties
	swprintf(sz, L"%d  %d  %d  %d  %d  %d  %d  %d  %d  %d\n", 
			m_dwID, 
			m_aPoints.GetSize(), 
			m_aMarks.GetSize(), 
			m_iPriority, 
			m_idDstInst, 
			m_aMusicFiles.GetSize(),
			m_iMusicInter,
			m_iMusicLoop,
			m_idSrcInst,
			m_idDomain);

	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	swprintf(sz, L"%f, %f, %f\n", m_vCityPos.x, m_vCityPos.y, m_vCityPos.z);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	int i;

	//	Write point positions
	for (i=0; i < m_aPoints.GetSize(); i++)
	{
		const VECTOR3& v = m_aPoints[i];
		swprintf(sz, L"%f  %f  %f\n", v.x, v.y, v.z);
		file.Write(sz, _StrLenInByte(sz), &dwWrite);
	}

	swprintf(sz, L"\n");
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write marks
	for (i=0; i < m_aMarks.GetSize(); i++)
	{
		const MARK* pMark = m_aMarks[i];

		const ACHAR* szMarkName = m_pDataSet->GetTextByIndex(pMark->iNameIdx);
		if (!szName || !szName[0])
			szMarkName = L"Unknown Mark";

		const VECTOR3& v = pMark->vPos;

		swprintf(sz, L"\"%s\"  %f  %f  %f\n", szMarkName, v.x, v.y, v.z);
		file.Write(sz, _StrLenInByte(sz), &dwWrite);
	}

	swprintf(sz, L"\n");
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Save music and sound files
	swprintf(sz, L"\"%s\"\n", m_strSound);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	for (i=0; i < m_aMusicFiles.GetSize(); i++)
	{
		swprintf(sz, L"\"%s\"\n", *m_aMusicFiles[i]);
		file.Write(sz, _StrLenInByte(sz), &dwWrite);
	}

	swprintf(sz, L"\"%s\"\n", m_strSound_n);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);
	
	swprintf(sz, L"\n");
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	return true;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement CPrecinctDataSet
//	
///////////////////////////////////////////////////////////////////////////

CPrecinctDataSet::CPrecinctDataSet()
{
	m_dwTimeStamp = 0;
}

CPrecinctDataSet::~CPrecinctDataSet()
{
	int i;

	//	Release all precincts
	for (i=0; i < m_aPrecincts.GetSize(); i++)
		delete m_aPrecincts[i];

	//	Release string table
	for (i=0; i < m_aStrings.GetSize(); i++)
		delete m_aStrings[i];
}

//	Load data from file
bool CPrecinctDataSet::Load(const char* szFileName)
{
	AWScriptFile File;
	if (!File.Open(szFileName))
		return false;

	//	Get version
	if (!File.MatchToken(L"version", false))
		return false;

	int iVersion = File.GetNextTokenAsInt(false);
	if (iVersion > ELPCTFILE_VERSION)
		return false;

	if (iVersion >= 5)
		m_dwTimeStamp = (unsigned int)File.GetNextTokenAsInt(true);
	else
		m_dwTimeStamp = 0;

	while (File.PeekNextToken(true))
	{
		CPrecinctData* pPrecinct = new CPrecinctData(this);
		if (!pPrecinct->Load(&File, iVersion))
			return false;

		m_aPrecincts.Add(pPrecinct);
	}

	return true;
}

//	Save data to file
bool CPrecinctDataSet::Save(const char* szFileName)
{
/*	AMemFile file;
	if (!file.Attach(NULL, 0, 4096))
		return false;
	
	DWORD dwWrite;
	wchar_t sz[1024];

	//	Write unicode header
	wchar_t wcFlag = 0xfeff;
	swprintf(sz, L"%c", wcFlag);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write prompt string
	swprintf(sz, L"//  Element pricinct file (client version)\n\n");
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write version
	swprintf(sz, L"version  %d\n", ELPCTFILE_VERSION);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);
	//	Write time stamp
	swprintf(sz, L"%d\n\n", m_dwTimeStamp);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write precinct data
	for (int i=0; i < m_aPrecincts.GetSize(); i++)
	{
		CPrecinctData* pPrecinct = m_aPrecincts[i];
		pPrecinct->Save(file);
	}

	if (!AFileImage::WriteToPack(szFileName, file.GetFileBuffer(), file.GetFileLength()))
		return false;
*/
	FILE* fp = fopen(szFileName, "wb");
	if (!fp)
		return false;

	//	Write unicode header
	wchar_t wcFlag = 0xfeff;
	fwprintf(fp, L"%c", wcFlag);
	fwprintf(fp, L"//  Element pricinct file (client version)\n\n");

	//	Write version
	fwprintf(fp, L"version  %d\n", ELPCTFILE_VERSION);
	//	Write time stamp
	fwprintf(fp, L"%d\n\n", m_dwTimeStamp);

	//	Write precinct data
	for (int i=0; i < m_aPrecincts.GetSize(); i++)
	{
		CPrecinctData* pPrecinct = m_aPrecincts[i];
		pPrecinct->Save(fp);
	}

	fclose(fp);

	return true;
}

//	Add a string to string table
int CPrecinctDataSet::AddText(const ACHAR* szText)
{
	//	Check whether string has been existed
//	int iIndex = FindText(szText);
//	if (iIndex >= 0)
//		return iIndex;

	ACString* pstr = new ACString(szText);
	return m_aStrings.Add(pstr);
}

//	Find string in table
int CPrecinctDataSet::FindText(const ACHAR* szText)
{
	for (int i=0; i < m_aStrings.GetSize(); i++)
	{
		if (!a_strcmp(*m_aStrings[i], szText))
			return i;
	}

	return -1;
}

//	Change text content
void CPrecinctDataSet::SetTextByIndex(int iIndex, const ACHAR* szText)
{
	*m_aStrings[iIndex] = szText ? szText : _AL("");
}

//	Get string from table by index
const ACHAR* CPrecinctDataSet::GetTextByIndex(int iIndex)
{
	return *m_aStrings[iIndex];
}

