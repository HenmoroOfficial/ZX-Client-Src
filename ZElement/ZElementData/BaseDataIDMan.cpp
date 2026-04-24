#include "StdAfx.h"
#include "BaseDataIDMan.h"
#include "AFile.h"
#include "WinBase.h"
#include "Global.h"
#include "VssOperation.h"
#include "EC_MD5Hash.h"

const char* _format_id_type = "IDType = %s";
const char* _format_id = "ID = %u, Path = %s";
const char* _format_id_version = "Version = %x, ID = %u, Path = %s";

const char* _base_type	= "BaseType";
const char* _ext_type	= "ExtType";
const char* _path_type  = "PathType";

BaseDataIDManBase g_BaseIDMan;
BaseDataIDManBase g_TaskIDMan;
BaseDataIDManBase g_TalkIDMan;
BaseDataIDManBase g_PolicyIDMan;
BaseDataIDManBase g_RecipeIDMan;
BaseDataIDManBase g_ConfigIDMan;
BaseDataIDManBase g_QuestionsIDMan;

ExtDataIDMan      g_ExtBaseIDMan;
DataPathIDMan     g_PathIDMan;

//增加海外预留ID相关的控制变量 Added 2011-04-18
bool	g_bOverseasEnable = false;	//海外ID预留是否启用
//Added end.

bool BaseDataIDManBase::IsRightType(const char* szType) const
{
	return strcmp(szType, _base_type) == 0;
}

const char* BaseDataIDManBase::GetType() const
{
	return _base_type;
}

int BaseDataIDManBase::Load(
	const char* szPath,
	BaseDataPathIDMap& PathMap,
	BaseDataIDPathMap& IDMap,
	unsigned long& ulMaxID)
{
	AFile file;
	if (!file.Open(szPath, AFILE_OPENEXIST | AFILE_TEXT))
		return -1;

	ulMaxID = 0;
	char	szLine[AFILE_LINEMAXLEN];
	char	szBuf[AFILE_LINEMAXLEN];
	DWORD	dwRead;

	szLine[0] = '\0';
	if (!file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead))
	{
		file.Close();
		return -1;
	}

	szBuf[0] = '\0';
	sscanf(szLine, _format_id_type, szBuf);

	if (!IsRightType(szBuf))
	{
		file.Close();
		return -1;
	}

	while (true)
	{
		szLine[0] = '\0';
		if (!file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead)
		|| strlen(szLine) == 0) break;

		unsigned long ulID = 0;
		unsigned long ulVersion = 0;
		szBuf[0] = '\0';

		if (HasVersionInfo(szLine))
		{
			sscanf(szLine, _format_id_version, &ulVersion, &ulID, szBuf);
		}
		else
		{
			sscanf(szLine, _format_id, &ulID, szBuf);
		}

		if (ulID == 0 || strlen(szBuf) == 0) continue;
		
		AString strPath = szBuf;
		if (IDMap.find(ulID) != IDMap.end()
		 || PathMap.find(strPath) != PathMap.end())
		{
			file.Close();
			return -2;
		}

		IDMap[ulID] = strPath;
		PathMap[strPath] = ulID;
		if(ulVersion>0)
			m_IDVersionMap[ulID] = ulVersion;

		if (ulID > ulMaxID) ulMaxID = ulID;
	}

	file.Close();
	return 0;
}

int BaseDataIDManBase::Load(const char* szPath)
{
	unsigned long ulMaxID;
	int nRet = Load(szPath, m_PathIDMap, m_IDPathMap, ulMaxID);
	if (nRet != 0) return nRet;
	m_ulMaxID = ulMaxID;
	m_strFile = szPath;
	return 0;
}

bool BaseDataIDManBase::CanViewTempl(const AString& strPath) const
{
	unsigned long ulID = GetIDByPath(strPath);
	unsigned long ulVersion = 0;
	BaseDataIDVersionMap::const_iterator it = m_IDVersionMap.find(ulID);
	if (it!=m_IDVersionMap.end())
	{
		ulVersion = it->second; 
	}

	if (g_ulVersion==g_ulVersionCurrent)
	{
		return ulVersion<=g_ulVersionCurrent;
	}

	return true;

}

int BaseDataIDManBase::VersionState(unsigned long ulID) const
{
	unsigned long ulVersion = 0;
	BaseDataIDVersionMap::const_iterator it = m_IDVersionMap.find(ulID);
	if (it!=m_IDVersionMap.end())
	{
		ulVersion = it->second; 
	}

	if (g_ulVersion==g_ulVersionCurrent)		// 老编辑器
	{
		return ulVersion<=g_ulVersionCurrent ? VS_OK : VS_2;
	}
	else if (g_ulVersion>g_ulVersionCurrent)	// 新编辑器
	{
		return ulVersion>g_ulVersionCurrent ? VS_OK : VS_1;
	}
	assert(0);
	return -1;
}

int BaseDataIDManBase::VersionState(const AString& strPath) const
{
	return VersionState(GetIDByPath(strPath));
}

unsigned long BaseDataIDManBase::CreateID(const AString& strPath)
{
	if (strPath.FindOneOf("\n \/:*?\"<>|") >= 0) return 0;
	if (!CheckOut(m_strFile)) return 0;

	BaseDataPathIDMap PathMap;
	BaseDataIDPathMap IDMap;
	unsigned long ulMaxID;

	m_IDVersionMap.clear();

	if (Load(m_strFile, PathMap, IDMap, ulMaxID) != 0
	 || PathMap.find(strPath) != PathMap.end())
	{
		CheckIn(m_strFile);
		return 0;
	}

	ulMaxID = IncID(ulMaxID);
	IDMap[ulMaxID] = strPath;
	PathMap[strPath] = ulMaxID;

	m_PathIDMap.clear();
	m_IDPathMap.clear();

	for (BaseDataPathIDMap::iterator iPath = PathMap.begin(); iPath != PathMap.end(); ++iPath)
		m_PathIDMap[iPath->first] = iPath->second;

	for (BaseDataIDPathMap::iterator iID = IDMap.begin(); iID != IDMap.end(); ++iID)
		m_IDPathMap[iID->first] = iID->second;

	m_ulMaxID = ulMaxID;
	m_IDVersionMap[ulMaxID] = g_ulVersion;

	Save();
	CheckIn(m_strFile);

	return ulMaxID;
}

//根据给定的ID来执行“生成ID”的流程，主要是判断ID的合法性并且将ID-PATH对写入文件中Added 2011-04-18.
unsigned long BaseDataIDManBase::CreateIDExt( unsigned long ulID, const AString& strPath )
{
	//判断下路径字符串的合法性
	if (strPath.FindOneOf("\n ") >= 0) return 0;
	if (!CheckOut(m_strFile)) return 0;

	BaseDataPathIDMap PathMap;
	BaseDataIDPathMap IDMap;
	unsigned long ulMaxID;

	//读取全部ID-PATH数据到map中，并判断输入模板路径和ID的合法性
	if (Load(m_strFile, PathMap, IDMap, ulMaxID) != 0
	 || PathMap.find(strPath) != PathMap.end() 
	 || IDMap.find(ulID) != IDMap.end())
	{
		CheckIn(m_strFile);
		return 0;
	}
	
	//将合法的给定ID-PATH对存入各MAP中
	IDMap[ulID] = strPath;
	PathMap[strPath] = ulID;

	//清除成员MAP值
	m_PathIDMap.clear();
	m_IDPathMap.clear();

	//将全部更新的MAP数据存入成员MAP中
	for (BaseDataPathIDMap::iterator iPath = PathMap.begin(); iPath != PathMap.end(); ++iPath)
		m_PathIDMap[iPath->first] = iPath->second;

	for (BaseDataIDPathMap::iterator iID = IDMap.begin(); iID != IDMap.end(); ++iID)
		m_IDPathMap[iID->first] = iID->second;

	//如果之前最大ID小于ulID，则更新当前最大ID？
	if ( m_ulMaxID < ulID )
	{
		m_ulMaxID = ulID;
	}
	
	m_IDVersionMap[ulID] = g_ulVersion;
	Save();
	CheckIn(m_strFile);

	return ulID;
}
//Added end.

void BaseDataIDManBase::WipeOffByPath(abase::vector<AString>& WipeOffArray)
{
	if (!CheckOut(m_strFile)) return;

	BaseDataPathIDMap PathMap;
	BaseDataIDPathMap IDMap;
	unsigned long ulMaxID;

	if (Load(m_strFile, PathMap, IDMap, ulMaxID) != 0) return;

	for (size_t i = 0; i < WipeOffArray.size(); i++)
	{
		BaseDataPathIDMap::iterator it = PathMap.find(WipeOffArray[i]);
		if (it == PathMap.end()) continue;

		ASSERT(IDMap.find(it->second) != IDMap.end());
		IDMap.erase(it->second);
		PathMap.erase(WipeOffArray[i]);
	}

	m_ulMaxID = 0;
	m_PathIDMap.clear();
	m_IDPathMap.clear();

	for (BaseDataPathIDMap::iterator iPath = PathMap.begin(); iPath != PathMap.end(); ++iPath)
		m_PathIDMap[iPath->first] = iPath->second;

	for (BaseDataIDPathMap::iterator iID = IDMap.begin(); iID != IDMap.end(); ++iID)
	{
		if (iID->first > m_ulMaxID) m_ulMaxID = iID->first;
		m_IDPathMap[iID->first] = iID->second;
	}

	Save();
	CheckIn(m_strFile);
}

void BaseDataIDManBase::WipeOffByID(abase::vector<unsigned long>& WipeOffArray)
{
	if (!CheckOut(m_strFile)) return;

	BaseDataPathIDMap PathMap;
	BaseDataIDPathMap IDMap;
	unsigned long ulMaxID;

	if (Load(m_strFile, PathMap, IDMap, ulMaxID) != 0) return;

	for (size_t i = 0; i < WipeOffArray.size(); i++)
	{
		BaseDataIDPathMap::iterator it = IDMap.find(WipeOffArray[i]);
		if (it == IDMap.end()) continue;

		ASSERT(PathMap.find(it->second) != PathMap.end());
		PathMap.erase(it->second);
		IDMap.erase(WipeOffArray[i]);
	}

	m_ulMaxID = 0;
	m_PathIDMap.clear();
	m_IDPathMap.clear();

	for (BaseDataPathIDMap::iterator iPath = PathMap.begin(); iPath != PathMap.end(); ++iPath)
		m_PathIDMap[iPath->first] = iPath->second;

	for (BaseDataIDPathMap::iterator iID = IDMap.begin(); iID != IDMap.end(); ++iID)
	{
		if (iID->first > m_ulMaxID) m_ulMaxID = iID->first;
		m_IDPathMap[iID->first] = iID->second;
	}

	Save();
	CheckIn(m_strFile);
}

bool BaseDataIDManBase::CanEdit(const char* szPath)
{
	if (strlen(szPath) == 0) return false;
	if(g_bLinkVss)
		return g_VSS.IsCheckOutStatus(szPath);
	else
	{
		DWORD dwAttr = GetFileAttributesA(szPath);
		return (dwAttr != -1 && !(dwAttr & FILE_ATTRIBUTE_READONLY));
	}
}

bool BaseDataIDManBase::CanExport(unsigned long ulID) const
{
	unsigned long ulVersion = 0;
	BaseDataIDVersionMap::const_iterator it = m_IDVersionMap.find(ulID);
	if (it!=m_IDVersionMap.end())
	{
		ulVersion = it->second; 
	}
	
	if (g_ulVersion==g_ulVersionCurrent)
	{
		return ulVersion<=g_ulVersionCurrent;
	}
	else
	{
		return true;
	}
}

bool BaseDataIDManBase::HasVersionInfo(char *str)
{
	int numComma = 0;

	char *pchStart = str, *pch = NULL;
	while( true )
	{
		pch = strstr(pchStart, ",");
		if( !pch ) break;

		numComma++;
		pchStart = pch + 1;
	}

	return numComma>=2;
}

bool BaseDataIDManBase::CheckOut(const char* szPath)
{
	CString str(szPath);
	int n = str.ReverseFind('\\');
	if (n == -1) return false;
	g_VSS.SetProjectPath(str.Left(n));
	
	g_VSS.CheckOutFile(szPath);
	if (CanEdit(szPath)) return true;

	int nTryTimes = 3;
	while (nTryTimes--)
	{
		Sleep(1000);
		if (CanEdit(szPath)) return true;
	}
	return false;
}

bool BaseDataIDManBase::CheckIn(const char* szPath)
{
	CString str(szPath);
	int n = str.ReverseFind('\\');
	if (n == -1) return false;
	g_VSS.SetProjectPath(str.Left(n));

	g_VSS.CheckInFile(str.Mid(n+1));
	if (!CanEdit(szPath)) return true;

	int nTryTimes = 3;
	while (nTryTimes--)
	{
		Sleep(1000);
		if (!CanEdit(szPath)) return true;
	}
	return false;
}

bool BaseDataIDManBase::Save()
{
	AFile file;
	if (!file.Open(m_strFile, AFILE_CREATENEW | AFILE_TEXT))
		return false;

	char	szLine[AFILE_LINEMAXLEN];

	sprintf(szLine, _format_id_type, GetType());
	file.WriteLine(szLine);

	for (unsigned long i = 1; i <= m_ulMaxID; i = IncID(i))
	{
		BaseDataIDPathMap::iterator it = m_IDPathMap.find(i);
		if (it != m_IDPathMap.end())
		{
			unsigned ulVersion = 0;
			BaseDataIDVersionMap::iterator it1 = m_IDVersionMap.find(i);
			if (it1!=m_IDVersionMap.end())
			{
				ulVersion = it1->second; 
			}
			sprintf(szLine, _format_id_version, ulVersion, i, it->second);
			file.WriteLine(szLine);
		}
	}

	file.Close();
	return true;
}

void BaseDataIDManBase::GeneratePathArray(abase::vector<AString>& PathArray)
{
	for (BaseDataIDPathMap::iterator it = m_IDPathMap.begin(); it != m_IDPathMap.end(); ++it)
	{
		if (CanExport(it->first))
		{
			PathArray.push_back(it->second);
		}
	}
}

//////////////////////////////////////////////////////

const unsigned long _high_mask	= 0xffff8000;
const unsigned long _low_mask	= 0x00000fff;
const unsigned long _high_unit	= (~_high_mask + 1);

unsigned long ExtDataIDMan::IncID(unsigned long ulID)
{
	unsigned long ulHighPart = ulID & _high_mask;
	unsigned long ulLowPart = ulID & _low_mask;

	ulLowPart++;
	if (ulLowPart > _low_mask)
		ulHighPart += _high_unit;

	return (ulHighPart & _high_mask) + (ulLowPart & _low_mask);
}

bool ExtDataIDMan::IsRightType(const char* szType) const
{
	return strcmp(szType, _ext_type) == 0;
}

const char* ExtDataIDMan::GetType() const
{
	return _ext_type;
}

////////////////////////////////////////////////////////////////////////////////
bool DataPathIDMan::IsRightType(const char* szType) const
{
	return strcmp(szType, _path_type) == 0;
}

const char* DataPathIDMan::GetType() const
{
	return _path_type;
}

#define	PATHMAP_IDENTIFY		(('P'<<24) | ('M'<<16) | ('I'<<8) | 'D')

int DataPathIDMan::Export(const char *szPath)
{
	FILE * pFile = fopen( szPath, "wb+");
	if( pFile == NULL ) return 0; 
	unsigned long Identify = PATHMAP_IDENTIFY;
	char  szBuffer[256];
	fwrite(&Identify,sizeof(unsigned long),1,pFile);
	unsigned long i;
	// 计算size大小，分版本以后不是全部导出
	unsigned long size = 0;
	for (i = 1; i <= m_ulMaxID; i = IncID(i))
	{
		BaseDataIDPathMap::iterator it = m_IDPathMap.find(i);
		if ( it!=m_IDPathMap.end() && CanExport(i))
		{
			size++;
		}
	}
	fwrite(&size,sizeof(unsigned long),1,pFile);
	for (i = 1; i <= m_ulMaxID; i = IncID(i))
	{
		BaseDataIDPathMap::iterator it = m_IDPathMap.find(i);
		if ( it!=m_IDPathMap.end() && CanExport(i))
		{
			memset(szBuffer,0,256);
			fwrite(&i,sizeof(unsigned long),1,pFile);
			sprintf(szBuffer,"%s",it->second);
			unsigned int len = strlen(szBuffer);
			fwrite(&len,sizeof(unsigned int),1,pFile);
			fwrite(szBuffer,sizeof(char)*len,1,pFile);
		}
	}

	int fend = ftell(pFile);
	fseek(pFile, 0, SEEK_SET);
	int fstart = 0;
	char buf[8192];
	while(fstart < fend)
	{
		int readsize = 8192;
		if( fend - fstart < readsize )
			readsize = fend - fstart;
		fread(buf, 1, readsize, pFile);
		g_MD5Hash.Update(buf, readsize);
		fstart += readsize;
	}

	fclose(pFile);

	return 1;
}