#include "BaseDataTemplate.h"
#include "BaseDataOrg.h"
#include "AFile.h"
#include "Global.h"
#include "EC_MD5Hash.h"

static const char* _format_version	= "Version: %u";
static const char* _format_id		= "ID: %u";
static const char* _format_tmpl_name= "TemplName: %s";
static const char* _format_parent	= "ParentPath: %s";
static const char* _format_num		= "ItemNum: %d";
static const char* _format_name		= "ItemName: %s";
static const char* _format_type		= "ItemType: %s";

#define TEMPL_CUR_VERSION	3

ORG_PATHS _org_paths;
ORG_PATHS _org_versions;
const char* _org_config_path = "BaseData\\OrgData.cfg";
const char* _update_log_path = "BaseData\\UpdateLog.txt";



#ifdef TASK_TEMPL_EDITOR
#include "ExpTypes.h"
bool TemplUneditable(AString tmpl)
{
	AString path(tmpl);
	path.CutLeft(strlen("BaseData\\"));
	BaseDataIDManBase* pIDMan;
	if (path.Left(4) == "配方")
		pIDMan = &g_RecipeIDMan;
	else if (path.Left(6) == "Config")
		pIDMan = &g_ConfigIDMan;
	else if (path.Left(8) == "附加属性")
		pIDMan = &g_ExtBaseIDMan;
	else
		pIDMan = &g_BaseIDMan;
	
	int iVState = pIDMan->VersionState(tmpl);
	return iVState!=0 && iVState!=1;
}
unsigned long g_ulVersionCurrent = ELEMENTDATA_VERSION;
unsigned long g_ulVersion = ELEMENTDATA_VERSION;
#endif	// TASK_TEMPL_EDITOR

const AString& BaseDataTempl::GetParentPathName() const
{
	return m_strParentPath;
}

int BaseDataTempl::GetItemNum() const
{
	return static_cast<int>(m_ItemArray.size());
}

const AString& BaseDataTempl::GetItemName(int nIndex) const
{
	return m_ItemArray[nIndex]->m_strName;
}

const AString& BaseDataTempl::GetItemType(int nIndex) const
{
	return m_ItemArray[nIndex]->m_strType;
}

AVariant BaseDataTempl::GetItemValue(int nIndex) const
{
	return m_ItemArray[nIndex]->m_varVal;
}

int BaseDataTempl::GetItemIndex(const char* szItem) const
{
	for (size_t i = 0; i < m_ItemArray.size(); i++)
		if (m_ItemArray[i]->m_strName.CompareNoCase(szItem) == 0)
			return i;

	return -1;
}

void BaseDataTempl::SetParentPathName(const AString& strPath)
{
	m_strParentPath = strPath;
}

void BaseDataTempl::SetItemValue(int nIndex, const AVariant& var)
{
	m_ItemArray[nIndex]->m_varVal = var;
}

void BaseDataTempl::SetItemValue( const AString& strKey, const AVariant& var )
{
	ItemMap::iterator it = m_ItemMap.find(strKey);
	if ( it != m_ItemMap.end() )
	{
		it->second->m_varVal = var;	
	}
}

AString BaseDataTempl::ParseOrgPath(const char* szOrgPath, const char* szPath, int& nLevel)
{
	const char* szSub = strstr(szPath, "\\");
	nLevel = 0;

	while (szSub)
	{
		nLevel++;
		szSub = strstr(szSub+1, "\\");
	}

	AString strOrgPath;

	strOrgPath.Format("%s\\%d.org", szOrgPath, nLevel);
	if (g_ulVersion==g_ulVersionCurrent)
	{
		AString testOrgPath(szOrgPath);
		testOrgPath.Format("%s\\%x_%d.org", szOrgPath, g_ulVersion, nLevel);
		
		FILE *file;
		file = fopen(testOrgPath,"r");
		if(file) 
		{
			fclose(file);
			strOrgPath = testOrgPath;
		}
	}

	return strOrgPath;
}

AString BaseDataTempl::ParseParentPath() const
{
	int nFind = m_strPath.ReverseFind('\\');
	if (nFind == -1) return AString();
	AString str = m_strPath.Left(nFind);
	nFind = str.ReverseFind('\\');
	if (nFind == -1) return AString();
	str = str.Left(nFind);

	for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		if (str.CompareNoCase(n->first) == 0)
			return AString();
	}

	// 2013加了版本分支以后目录可能是10000094BaseData\...，但是基本tmpl用的还是BaseData\...下面的
	if (str.Find("BaseData")!=0)
		str.CutLeft(strlen("10000094"));

	nFind = str.ReverseFind('\\');
	if (nFind == -1) return AString();
	return str + str.Mid(nFind) + ".tmpl";
}

bool BaseDataTempl::CreateTempl(const char* szPath, const char* szParentPath, BaseDataIDManBase* pIDMan, bool bNew, unsigned long ulTemplID)
{
	AString strOrgPath, strSavePath;

	for (ORG_PATHS::iterator n = _org_paths.begin(); n != _org_paths.end(); ++n)
	{
		const char* szSub = strstr(szPath, n->first + "\\");
		if (szSub)
		{
			strOrgPath = n->first;
			szSub += strlen(n->first) + 1;
			strSavePath = szSub;
			break;
		}
	}

	m_strParentPath = szParentPath;

	BaseDataOrg OrgData;
	if (OrgData.Load(ParseOrgPath(strOrgPath, strSavePath, m_nLevel)) != 0)
		return false;

	for (int i = 0; i < OrgData.GetItemNum(); i++)
	{
		ITEM_DATA* pItem = new ITEM_DATA;
		pItem->m_strName = OrgData.GetItemName(i);
		pItem->m_strType = OrgData.GetItemType(i);
		if( OrgData.GetItemDefValue(i).GetType() != AVariant::AVT_INVALIDTYPE )
		{
			// use default value
			pItem->m_varVal = OrgData.GetItemDefValue(i);
		}
		
		m_ItemArray.push_back(pItem);
		m_ItemMap[pItem->m_strName + pItem->m_strType] = pItem;
	}

	OrgData.Release();

	if (bNew)
	{
		//Modified 2011-04-18.
		unsigned long ulID = 0;
		if ( g_bOverseasEnable ) //启用海外ID预留功能
		{
			ulID = pIDMan->CreateIDExt( ulTemplID, szPath );
		}
		else
		{
			ulID = pIDMan->CreateID(szPath);
		}
		//Modified end.
		if (ulID == 0)
		{
			Release();
			return false;
		}
		m_strPath = szPath;
		m_ulID = ulID;
	}

	return true;
}

int BaseDataTempl::Load(const char* szPath, bool bFromOrg)
{
	AFile file;

	if (!file.Open(szPath, AFILE_OPENEXIST | AFILE_TEXT))
		return -1;
	
	char	szLine[AFILE_LINEMAXLEN];
	char	szBuf[AFILE_LINEMAXLEN];
	DWORD	dwRead;
	DWORD	dwVersion = 0;
	int		nItemNum = 0;

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_version, &dwVersion);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_id, &m_ulID);

	szBuf[0] = '\0';
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_tmpl_name, szBuf);
	m_strName = szBuf;

	szBuf[0] = '\0';
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_parent, szBuf);
	m_strParentPath = szBuf;

	CreateTempl(szPath, m_strParentPath, NULL, false);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_num, &nItemNum);

	for (int i = 0; i < nItemNum; i++)
	{
		AString strName, strType;

		szBuf[0] = '\0';
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		sscanf(szLine, _format_name, szBuf);
		strName = szBuf;

		szBuf[0] = '\0';
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		sscanf(szLine, _format_type, szBuf);
		strType = szBuf;

		AVariant var;
		var.Load(&file);

		if (bFromOrg)
		{
			ItemMap::iterator it = m_ItemMap.find(strName + strType);
			if (it != m_ItemMap.end()) it->second->m_varVal = var;
			else
			{
#ifdef _DEBUG
				AString str;
				str.Format("Cant found item : %s\n", strName);
				OutputDebugString(str);
#endif
			}
		}
		else
		{
			ITEM_DATA* pItem = new ITEM_DATA;
			pItem->m_strName = strName;
			pItem->m_strType = strType;
			pItem->m_varVal = var;
			m_ItemArray.push_back(pItem);
			m_ItemMap[strName + strType] = pItem;
		}
	}

	file.Close();
	m_strPath = szPath;
	return 0;
}

bool BaseDataTempl::Save(const char* szPath)
{
	int iVersion = TemplVersionState(m_strPath);
	if (iVersion!=0 && iVersion!=1)
	{
		return false;
	}

	AFile file;

	if (!file.Open(szPath, AFILE_CREATENEW | AFILE_TEXT))
		return false;

	char	szLine[AFILE_LINEMAXLEN];
	
	sprintf(szLine, _format_version, TEMPL_CUR_VERSION);
	file.WriteLine(szLine);

	sprintf(szLine, _format_id, m_ulID);
	file.WriteLine(szLine);

	sprintf(szLine, _format_tmpl_name, m_strName);
	file.WriteLine(szLine);

	sprintf(szLine, _format_parent, m_strParentPath);
	file.WriteLine(szLine);

	sprintf(szLine, _format_num, m_ItemArray.size());
	file.WriteLine(szLine);

	for (size_t i = 0; i < m_ItemArray.size(); i++)
	{
		ITEM_DATA* pItem = m_ItemArray[i];

		sprintf(szLine, _format_name, pItem->m_strName);
		file.WriteLine(szLine);

		sprintf(szLine, _format_type, pItem->m_strType);
		file.WriteLine(szLine);

		pItem->m_varVal.Save(&file);
	}

	file.Close();
	return true;
}

void BaseDataTempl::Release()
{
	for (size_t i = 0; i < m_ItemArray.size(); i++)
		delete m_ItemArray[i];

	m_ItemArray.clear();
	m_ItemMap.clear();
}

bool BaseDataTempl::LoadHeader(const char* szPath)
{
	AFile file;

	if (!file.Open(szPath, AFILE_OPENEXIST | AFILE_TEXT))
		return false;

	char	szLine[AFILE_LINEMAXLEN];
	DWORD	dwRead;
	DWORD	dwVersion = 0;

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_version, &dwVersion);

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	sscanf(szLine, _format_id, &m_ulID);

	file.Close();
	m_strPath = szPath;
	return true;
}

BaseDataTempl& BaseDataTempl::operator = (const BaseDataTempl& src)
{
	for (size_t i = 0; i < src.m_ItemArray.size(); i++)
	{
		const ITEM_DATA* pSrc = src.m_ItemArray[i];
		ItemMap::iterator it = m_ItemMap.find(pSrc->m_strName + pSrc->m_strType);
		if (it != m_ItemMap.end())
			it->second->m_varVal = pSrc->m_varVal;
	}

	return *this;
}


bool WriteMd5CodeToTemplate(const char *szPathName)
{
	AFile file;
	PATCH::MD5Hash md;

	if (!file.Open(szPathName, AFILE_OPENEXIST | AFILE_TEXT))
		return false;
	
	char	szLine[AFILE_LINEMAXLEN];
	DWORD	dwRead;
	DWORD	dwVersion = 0;
	int		nItemNum = 0;

	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	sscanf(szLine, "Version: %u", &dwVersion);
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
	md.Update(szLine,dwRead);
	sscanf(szLine,"ItemNum: %d", &nItemNum);

	for (int i = 0; i < nItemNum; i++)
	{
		AString strName, strType;

		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		
		//----------------------------------------
		file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
		md.Update(szLine,dwRead);
		int iType;
		sscanf(szLine,"Type: %d", &iType);
		switch (iType)
		{
		case AVariant::AVT_INVALIDTYPE:
			break;
		case AVariant::AVT_BOOL:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_INT:
		case AVariant::AVT_LONG:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_UINT:
		case AVariant::AVT_ULONG:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_FLOAT:
		case AVariant::AVT_DOUBLE:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_STRING:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_A3DVECTOR3: 
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine,dwRead);
			break;
		case AVariant::AVT_INT64:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine, dwRead);
			break;
		case AVariant::AVT_UINT64:
			file.ReadLine(szLine, AFILE_LINEMAXLEN, &dwRead);
			md.Update(szLine, dwRead);
			break;
		default:
			ASSERT(FALSE);
		}
	}
	file.Close();
	
	//Md5 finished
	md.Final();
	unsigned int nLenght = 64; 
	char szBuffer[64];
	md.GetString(szBuffer,nLenght);
	
	//Write md5 code to file tail
	if (!file.Open(szPathName, AFILE_OPENAPPEND | AFILE_TEXT))
		return false;
	sprintf(szLine,"Md: %s",szBuffer);
	file.WriteLine(szLine);
	file.Close();
	
	return true;
}
