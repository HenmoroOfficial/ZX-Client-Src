/*
* FILE: CHullArchive.cpp
*
* DESCRIPTION: Load & Save Convex hull data
*
* CREATED BY: 
*
* HISTORY:
*
* 
*/

#include "BrushArchive.h"



static const DWORD dwBrushFileId = 0xfefefefe;
static const DWORD dwVersion = 2;//current version 2
#define ASSERTREADSUCESS(len) { if (len == 0) return false; } 

namespace CHBasedCD
{

BrushArchive::BrushArchive()
{
	m_nNumHull = 0;
	m_szModelFile[0] = 0;
}
BrushArchive::~BrushArchive()
{

}

const char* BrushArchive::GetModelFile() const
{
	return m_szModelFile;
}

//get hull number
int BrushArchive::GetNumHull() const
{
	return m_nNumHull;
}

void BrushArchive::SetModelFile(const char* szModelFile)
{
	if (szModelFile)
		strcpy(m_szModelFile, szModelFile); 
	else
		m_szModelFile[0] = 0;
}

bool BrushArchive::LoadHeader(AFile& af)
{
	DWORD dwRead = 0, dwLen = 0, dwDWSize = sizeof(DWORD);
	af.Read(&dwRead, dwDWSize, &dwLen);
	
	ASSERTREADSUCESS(dwLen);
	if (dwRead == dwBrushFileId)//new version brush file
	{
		af.Read(&dwRead, dwDWSize, &dwLen);
		ASSERTREADSUCESS(dwLen);
		if (dwRead != dwVersion)
			return false;
		
		af.Read(m_szModelFile, sizeof(m_szModelFile), &dwRead);
		
		af.Read(&m_nNumHull, sizeof(int), &dwRead);
		ASSERTREADSUCESS(dwLen);
	}
	else//old version
	{
		m_szModelFile[0] = 0;
		m_nNumHull = (int)dwRead;	
		//skip mesh data
		int nMeshID, IntSize = sizeof(int);
		for (int i = 0; i < m_nNumHull; i++)
		{
			af.Read(&nMeshID, IntSize, &dwLen);
			af.Seek(nMeshID * IntSize, AFILE_SEEK_CUR);
		}
	}

	return true;
}

bool BrushArchive::SaveHeader(AFile& af)
{
	DWORD dwLen, dwDWSize = sizeof(DWORD);
	af.Write((LPVOID)&dwBrushFileId, dwDWSize, &dwLen);
	af.Write((LPVOID)&dwVersion, dwDWSize, &dwLen);
	af.Write((LPVOID)m_szModelFile, sizeof(m_szModelFile), &dwLen);

	af.Write((LPVOID)&m_nNumHull, sizeof(int), &dwLen);
	return true;
}

}
