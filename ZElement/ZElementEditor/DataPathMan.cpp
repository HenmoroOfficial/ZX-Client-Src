// DataPathMan.cpp: implementation of the DataPathMan class.
//
//////////////////////////////////////////////////////////////////////


#include "DataPathMan.h"

#define	PATHMAP_IDENTIFY		(('P'<<24) | ('M'<<16) | ('I'<<8) | 'D')

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

DataPathMan::DataPathMan()
{

}

DataPathMan::~DataPathMan()
{
	Release();
}

int DataPathMan::Load(const char *szPath)
{
	Release();
	
	FILE* pFile = fopen(szPath,"rb");
	if(pFile==NULL) return 0;
	
	unsigned long Identify;
	fread(&Identify,sizeof(unsigned long),1,pFile);
	if(Identify != PATHMAP_IDENTIFY) return 0;

	unsigned long id, usize;
	char szBuffer[256];
	fread(&usize,sizeof(unsigned long),1,pFile);
	for( unsigned long i = 0; i < usize; ++i)
	{
		unsigned int len = 0;
		memset(szBuffer,'\0',256);
		fread(&id,sizeof(unsigned long),1,pFile);
		fread(&len,sizeof(unsigned int),1,pFile);
		fread(szBuffer,sizeof(char)*len,1,pFile);

		AString strPath = szBuffer;
		if (m_PathIDMap.find(strPath) != m_PathIDMap.end()
		 || m_IDPathMap.find(id) != m_IDPathMap.end())
		{
			fclose(pFile);
			return 0;
		}
		m_IDPathMap[id] = strPath;
		m_PathIDMap[strPath] = id;
	}
	fclose(pFile);
	return 1;
}
