#ifndef __TEXT_FILE_MGR_H__
#define __TEXT_FILE_MGR_H__

#include "AAssist.h"
#include <hashmap.h>
#include <tchar.h>


typedef abase::hash_map<ACString, ACString> FILE_DATA_MAP; 
typedef abase::vector<ACString> FILE_NAME_LIST;

class CTextFileMgr
{
public:
	CTextFileMgr( const char *szWorkPath );
	~CTextFileMgr();

	bool LoadData();
	bool SaveData();

	FILE_DATA_MAP* GetFileDataMap(){ return &m_FileDataMap; }

private:
	void Release();
	FILE_DATA_MAP m_FileDataMap;
	char m_szWorkPath[MAX_PATH];
};

#endif