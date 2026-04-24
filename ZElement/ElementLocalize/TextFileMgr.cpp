#include "StdAfx.h"
#include "TextFileMgr.h"
#include "AFileImage.h"
#include "AMemFile.h"

const WCHAR* szPathArray[] = 
{
	L"Interfaces\\script\\rank_disp_u.lua",
	L"Interfaces\\script\\config\\title_def_u.lua",
	L"configs\\toptable.txt",
	L"Interfaces\\script\\lottery_desc.lua",
	L"Interfaces\\script\\LevelUpHelp.lua",
	L"Interfaces\\script\\interfaces\\SecretaryData.lua",
	L"interfaces\\script\\config\\menology_info.lua",
	L"configs\\monster_desc.txt",
	L"configs\\dreamheyang.txt",
	L"configs\\zhuxiantask.txt"
};

const DWORD dwType[] = 
{
	CP_UTF8,
	CP_UTF8,
	0,
	CP_UTF8,
	CP_UTF8,
	CP_UTF8,
	CP_UTF8,
	0,
	0,
	0
};

const int iTextNum = 10;

CTextFileMgr::CTextFileMgr( const char *szWorkPath )
{
	strcpy(m_szWorkPath,szWorkPath);
}

CTextFileMgr::~CTextFileMgr()
{
}

void CTextFileMgr::Release()
{
	m_FileDataMap.clear();
}

bool CTextFileMgr::LoadData()
{
	Release();
	
	for( int i = 0; i < iTextNum; ++i) 
	{
		char szPath[MAX_PATH];
		sprintf(szPath,"%s\\%s",m_szWorkPath,AC2AS(szPathArray[i]));
			
		AFileImage file;
		if(FALSE==file.Open(szPath,AFILE_OPENEXIST | AFILE_NOHEAD))
			return false;
		
		if(dwType[i]==0)
		{
			DWORD len = file.GetFileLength() - 2;
			char *szData = new char[len + 1];
			memset(szData,0,len + 1);
			DWORD rlen;
			file.Seek(2,(AFILE_SEEK)SEEK_SET);
			file.Read(szData,len - 2,&rlen);
			m_FileDataMap[szPathArray[i]] = ACString((WCHAR*)(szData));
			delete []szData;
		}else
		{
			DWORD len = file.GetFileLength();
			char *szData = new char[len + 1];
			memset(szData,0,len + 1);
			DWORD rlen;
			file.Read(szData,file.GetFileLength(),&rlen);
			
			WCHAR* szWData = new WCHAR[len + 1];
			rlen = MultiByteToWideChar(dwType[i],0,szData,len,szWData,len + 1);
			szWData[rlen] = 0;
			CString temp = CString(szWData);
			temp.Replace(_T("\r\n"),_T("\n"));
			temp.Replace(_T("\n"),_T("\r\n"));
			m_FileDataMap[szPathArray[i]] = temp;
			delete []szWData;
			delete []szData;
		}
		file.Close();
		
	
	}

	return true;
}

bool CTextFileMgr::SaveData()
{
	for( int i = 0; i < iTextNum; ++i) 
	{
		char szPath[MAX_PATH];
		sprintf(szPath,"%s\\%s",m_szWorkPath,AC2AS(szPathArray[i]));
		
		DWORD wlen = 0;
		AFileImage file;
		if(FALSE==file.Open(szPath,AFILE_OPENEXIST | AFILE_NOHEAD))
			return false;

		WORD head = 0xfeff;
		file.Read(&head,2,&wlen);
		file.Close();

		AMemFile memFile;
		memFile.Attach(NULL,0,1024*500);
		
		FILE_DATA_MAP::iterator it = m_FileDataMap.find(ACString(szPathArray[i]));
		if(it == m_FileDataMap.end()) continue;
		ACString strDest = it->second; 
		
		DWORD len = strDest.GetLength();
		if(dwType[i]==0)
		{
			memFile.Write(&head,2,&wlen);
			memFile.Write(strDest.GetBuffer(0),len*2,&wlen);
		}else
		{
			int nBufSize = WideCharToMultiByte(dwType[i],0,strDest,len,NULL,0,NULL,NULL);
			char *szData = new char[nBufSize+1];
			memset(szData,0,nBufSize + 1);			
			len = WideCharToMultiByte(dwType[i],0,strDest,len,szData,nBufSize,NULL,NULL);
			szData[len] = 0;
			memFile.Write(szData,len,&wlen);
			delete []szData; 
		}
		if (!AFileImage::WriteToPack(szPath, memFile.GetFileBuffer(), memFile.GetFileLength(), true))
			return false;
		
	}
	return true;
}