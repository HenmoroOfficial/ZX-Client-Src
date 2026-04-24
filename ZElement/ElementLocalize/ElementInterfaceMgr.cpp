#include "ElementInterfaceMgr.h"
#include "AScriptFile.h"
#include "AWScriptFile.h"
#include "AFileImage.h"
#include "..\ZAUInterface\\AXMLFile.h"
#include "AMemFile.h"

CElementInterfaceManager::CElementInterfaceManager(const char *szWorkPath)
{
	m_vecHtf.clear();
	m_vecStf.clear();
	m_vecHelpScriptFile.clear();
	m_vecHelpTextFile.clear();
	m_mapInterfaceStringTables.clear();
	m_mapInterfaceHintTables.clear();
	m_mapHelpScriptTables.clear();
	m_mapHelpTextTables.clear();
	strcpy(m_szWorkPath,szWorkPath);
}

CElementInterfaceManager::~CElementInterfaceManager()
{
	Release();
}

void CElementInterfaceManager::Release()
{
	m_vecHtf.clear();
	m_vecStf.clear();
	m_vecHelpScriptFile.clear();
	m_vecHelpTextFile.clear();
	InterfaceStringTables::iterator it1 = m_mapInterfaceStringTables.begin();
	for (; it1 != m_mapInterfaceStringTables.end(); ++it1)
	{
		it1->second->clear();
		delete it1->second;
	}
	m_mapInterfaceStringTables.clear();
	InterfaceHintTables::iterator it2 = m_mapInterfaceHintTables.begin();
	for (; it2 != m_mapInterfaceHintTables.end(); ++it2)
	{
		it2->second->clear();
		delete it2->second;
	}
	m_mapInterfaceHintTables.clear();
	it2 = m_mapHelpScriptTables.begin();
	for (; it2 != m_mapHelpScriptTables.end(); ++it2)
	{
		it2->second->clear();
		delete it2->second;
	}
	m_mapHelpScriptTables.clear();
	it2 = m_mapHelpTextTables.begin();
	for (; it2 != m_mapHelpTextTables.end(); ++it2)
	{
		it2->second->clear();
		delete it2->second;
	}
	m_mapHelpTextTables.clear();
}

bool CElementInterfaceManager::LoadInterface()
{
	Release();

	bool bval;
	AScriptFile as;
	AWScriptFile ws;

	//	Load Interface
	char szPath[2000];
	sprintf(szPath,"%s\\Interfaces\\LoginUI.dcf",m_szWorkPath);
	bval = as.Open(szPath);
	if( !bval )
		return false;
	while( !as.IsEnd() )
	{
		bval = as.GetNextToken(true);
		if( !bval ) break;		// End of file.

		if( 0 == strcmpi(as.m_szToken, "#") ||
			0 == strcmpi(as.m_szToken, "//") )
		{
			as.SkipLine();
		}
		else
		{
			AString strDtf = as.m_szToken;
			if( strDtf.Right(4) == ".xml" )
				m_vecHtf.push_back(AS2AC(strDtf));
			else
			{
				strDtf.CutRight(4);
				m_vecStf.push_back(AS2AC(strDtf + ".stf"));
			}
			as.SkipLine();
		}
	}
	as.Close();
	
	sprintf(szPath,"%s\\Interfaces\\InGame.dcf",m_szWorkPath);
	bval = as.Open(szPath);
	if( !bval )
		return false;
	while( !as.IsEnd() )
	{
		bval = as.GetNextToken(true);
		if( !bval ) break;		// End of file.

		if( 0 == strcmpi(as.m_szToken, "#") ||
			0 == strcmpi(as.m_szToken, "//") )
		{
			as.SkipLine();
		}
		else
		{
			AString strDtf = as.m_szToken;
			if( strDtf.Right(4) == ".xml" )
				m_vecHtf.push_back(AS2AC(strDtf));
			else
			{
				strDtf.CutRight(4);
				m_vecStf.push_back(AS2AC(strDtf + ".stf"));
			}
			as.SkipLine();
		}
	}
	as.Close();
	
	sprintf(szPath,"%s\\Interfaces\\InGamesub.dcf",m_szWorkPath);
	bval = as.Open(szPath);
	if( !bval )
		return false;
	while( !as.IsEnd() )
	{
		bval = as.GetNextToken(true);
		if( !bval ) break;		// End of file.

		if( 0 == strcmpi(as.m_szToken, "#") ||
			0 == strcmpi(as.m_szToken, "//") )
		{
			as.SkipLine();
		}
		else
		{
			AString strDtf = as.m_szToken;
			if( strDtf.Right(4) == ".xml" )
				m_vecHtf.push_back(AS2AC(strDtf));
			else
			{
				strDtf.CutRight(4);
				m_vecStf.push_back(AS2AC(strDtf + ".stf"));
			}
			as.SkipLine();
		}
	}
	as.Close();
	m_vecStf.push_back(_AL("LoginUI.stf"));
	m_vecStf.push_back(_AL("InGame.stf"));
	m_vecStf.push_back(_AL("MsgBox.stf"));

	//	Load help script
	char temp[10000];
	DWORD dwTemp;

	sprintf(szPath,"%s\\Help\\Script\\list.txt",m_szWorkPath);
	AFileImage fileList;
	if (!fileList.Open(szPath, AFILE_NOHEAD | AFILE_TEXT | AFILE_OPENEXIST))
		return false;
	while (fileList.ReadLine(temp, 10000, &dwTemp))
	{
		AString strFile = temp;
		strFile.TrimLeft();
		strFile.TrimRight();
		if (!strFile.IsEmpty())
			m_vecHelpScriptFile.push_back(AS2AC(strFile));
	}
	if( !fileList.Close() )
		return false;

	//	Load help text
	for(int i = 0; ; i++)
	{
		sprintf(szPath, "%s\\Info\\group%d.txt", m_szWorkPath, i);
		if(!ws.Open(szPath)) break;

		while( ws.PeekNextToken(true) )
		{
			int nIndent = ws.GetNextTokenAsInt(true);
			for( int j = 0; j <= nIndent; j++ )
				ws.GetNextToken(true);
			HintTableFile * htf = new HintTableFile;
			(*htf)[ACString(_AL("topic"))] = ws.m_szToken;
			ws.GetNextToken(true);
			if( a_stricmp(ws.m_szToken , _AL("")) != 0)
			{
				m_vecHelpTextFile.push_back(ws.m_szToken);
				InterfaceHintTables::iterator it = m_mapHelpTextTables.find(ws.m_szToken);
				if (it == m_mapHelpTextTables.end())
				{
					m_mapHelpTextTables[ws.m_szToken] = htf;
				}
				else
				{
					htf->clear();
					delete htf;
					htf = 0;
				}
				
			}
			else
			{
				htf->clear();
				delete htf;
				htf = 0;
			}
			ws.GetNextToken(true);
		}

		ws.Close();
	}
	
	return ImportTables();
}

bool CElementInterfaceManager::SaveInterface()
{
	ACString strFileName;
	ACHAR szText[2000];
	char szPath[2000];
	
	InterfaceStringTables::iterator it1 = m_mapInterfaceStringTables.begin();
	DWORD dwLen;
	// Save string table files
	for (; it1 != m_mapInterfaceStringTables.end(); ++it1)
	{
		strFileName = it1->first;
		AMemFile file;
		if( !file.Attach(NULL, 0, 100000) )
			return false;
		sprintf(szPath,"%s\\Interfaces\\%s",m_szWorkPath,AC2AS(strFileName));

		// Unicode header.
		BYTE byteHeader[2];
		byteHeader[0] = 0xFF;
		byteHeader[1] = 0xFE;
		if( !file.Write(byteHeader, 2, &dwLen) )
			return false;

		StringTableFile::iterator it = it1->second->begin();
		for( ; it != it1->second->end(); ++it )
		{
			int idString = it->first;
			ACString str = it->second;
			a_sprintf(szText, _AL("%d\t\"%s\"\n"), idString, str);
			if( !file.Write(szText, a_strlen(szText) * sizeof(ACHAR), &dwLen) )
				return false;
		}

		if (!AFileImage::WriteToPack(szPath, file.GetFileBuffer(), file.GetFileLength(), true))
			return false;
	}

	// Save hint table files
	InterfaceHintTables::iterator it2 = m_mapInterfaceHintTables.begin();
	for (; it2 != m_mapInterfaceHintTables.end(); ++it2)
	{
		strFileName = it2->first;
		HintTableFile* htf = it2->second;
		AMemFile file;
		if( !file.Attach(NULL, 0, 100000) )
			return false;
		sprintf(szPath,"%s\\Interfaces\\%s",m_szWorkPath,AC2AS(strFileName));

		// Unicode header.
		BYTE byteHeader[2];
		byteHeader[0] = 0xFF;
		byteHeader[1] = 0xFE;
		if( !file.Write(byteHeader, 2, &dwLen) )
			return false;

		AXMLFile xmlfile;
		bool bval = xmlfile.LoadFromFile(szPath);
		if( !bval ) continue;	// Ignore error.

		AXMLItem *pItem = xmlfile.GetRootItem();
		if( pItem )
		{
			pItem = pItem->GetFirstChildItem();
			if( pItem )
			{
				pItem = pItem->GetFirstChildItem();
				while( pItem )
				{
					const ACHAR* pszName = pItem->GetValue(_AL("Name"));
					if( pszName )
					{
						AXMLItem *pChildItem = pItem->GetFirstChildItem();
						while( pChildItem )
						{
							if( a_stricmp(pChildItem->GetName(), _AL("Text")) == 0 || a_stricmp(pChildItem->GetName(), _AL("Hint")) == 0 )
							{
								const ACHAR* pszText = pChildItem->GetValue(_AL("String"));
								if( pszText )
								{
									ACString strName;
									strName.Format(_AL("%s_%s"), pszName, pChildItem->GetName());
									HintTableFile::iterator it = htf->find(strName);
									if( it != htf->end() )
										pChildItem->SetValue(_AL("String"), it->second);
								}
							}
							pChildItem = pChildItem->GetNextItem();
						}
					}
					pItem = pItem->GetNextItem();
				}
			}
		}
		const ACHAR *pszFile = xmlfile.GetText();
		if( !file.Write((void*)pszFile, a_strlen(pszFile) * sizeof(ACHAR), &dwLen) )
			return false;
		xmlfile.Release();

		if (!AFileImage::WriteToPack(szPath, file.GetFileBuffer(), file.GetFileLength(), true))
			return false;
	}

	// Save help script files
	it2 = m_mapHelpScriptTables.begin();
	for (; it2 != m_mapHelpScriptTables.end(); ++it2)
	{
		strFileName = it2->first;
		abase::vector<AString> vecFileStr;
		char temp[10000];
		DWORD dwTemp;
		AFileImage fImage;
		sprintf(szPath,"%s\\Help\\Script\\%s",m_szWorkPath,AC2AS(strFileName));
		if( !fImage.Open(szPath, AFILE_NOHEAD | AFILE_OPENEXIST) )
			return false;
		while( fImage.ReadLine(temp, 10000, &dwTemp) )
		{
			AString str = temp;
			str.TrimLeft();
			vecFileStr.push_back(str);
		}
		if( !fImage.Close() )
			return false;

		AMemFile file;
		if( !file.Attach(NULL, 0, 100000) )
			return false;
//		AFile file;
//		if (!file.Open(szPath, AFILE_CREATENEW | AFILE_BINARY))
//			return false;
		
		for(int i = 0; i < vecFileStr.size(); i++ )
			if( vecFileStr[i].Left(7) == "#define" )
			{
				char szChar[100], szName[100];				
				sscanf(vecFileStr[i], "%s%s", szChar, szName);

				wchar_t wszText[10000];
				wsprintf(wszText, L"%s %s \"%s\"", AS2AC(szChar), AS2AC(szName), (*it2->second)[AS2AC(szName)]);				
				
				AWString wcsText(wszText);
				int iBufferLen = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), NULL, 0, 0, 0);	
				char* strText = new char[iBufferLen+1];
				int len = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), strText, iBufferLen, 0, 0);				
				strText[len] = 0;
				file.WriteLine(strText);
				delete []strText;				
			}
			else if( vecFileStr[i].Left(5) == "#name" )
			{
				char szChar[100];
				sscanf(vecFileStr[i], "%s", szChar);	
				
				wchar_t wszText[10000];
				wsprintf(wszText, L"%s \"%s\"", AS2AC(szChar), (*it2->second)[_AL("name")]);
				
				AWString wcsText(wszText);
	
				int iBufferLen = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), NULL, 0, 0, 0);		
				char* strText = new char[iBufferLen + 1];
				int len = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), strText, iBufferLen, 0, 0);				
				strText[len] = 0;
				file.WriteLine(strText);
				delete []strText;
			}
			else
			{
				int len = MultiByteToWideChar(CP_UTF8, 0, vecFileStr[i], vecFileStr[i].GetLength(), NULL, 0);
				ACHAR* wstrText = new ACHAR[len + 1];
				MultiByteToWideChar(CP_UTF8, 0, vecFileStr[i], vecFileStr[i].GetLength(), wstrText, len + 1);
				wstrText[len] = 0;
				AWString wcsText(wstrText);
				delete []wstrText;

				
				int iBufferLen = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), NULL, 0, 0, 0);		
				char* strText = new char[iBufferLen + 1];
				len = WideCharToMultiByte(CP_UTF8, 0, wcsText, wcsText.GetLength(), strText, iBufferLen, 0, 0);				
				strText[len] = 0;
				file.WriteLine(strText);
				delete []strText;
			}
//		file.Close();

		sprintf(szPath,"Help\\Script\\%s",AC2AS(strFileName));
		if (!AFileImage::WriteToPack(szPath, file.GetFileBuffer(), file.GetFileLength(), true))
			return false;
		/*
		AWScriptFile ws;
		if(!ws.Open(szPath))
			return false;
			*///debug
	}

	// Save help text files
	AWScriptFile ws;
	for(int k = 0; ; k++)
	{
		abase::vector<ACString> vecFileStr;
		sprintf(szPath, "%s\\Info\\group%d.txt", m_szWorkPath, k);
		if(!ws.Open(szPath)) break;
		while( ws.PeekNextToken(true) )
		{
			ws.GetNextToken(true);
			vecFileStr.push_back(ws.m_szToken);
		}
		ws.Close();
		AFile file;
		if (!file.Open(szPath, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
			return false;
		BYTE byteHeader[2];
		byteHeader[0] = 0xFF;
		byteHeader[1] = 0xFE;
		if( !file.Write(byteHeader, 2, &dwLen) )
			return false;
		for(int i = 0; i < vecFileStr.size(); i++ )
		{
			int nIndent = a_atoi(vecFileStr[i]);
			ACString strText = vecFileStr[i];
			DWORD dwLen;
			for(int j = 0; j < nIndent; j++ )
			{
				i++;
				strText += _AL("\t") + vecFileStr[i];
			}
			i++;
			i++;
			if( vecFileStr[i] != _AL("") )
			{
				ACString strTemp = (*m_mapHelpTextTables[vecFileStr[i]])[ACString(_AL("topic"))];
				int i = strTemp.Find(_AL("\\r"), 0);
				while( i != -1 )
				{
					strTemp = strTemp.Left(i) + _AL("\r") + strTemp.Mid(i + 2);
					i = strTemp.Find(_AL("\\r"), i + 1);
				}
				
				strText += _AL("\t\"") + strTemp;
			}
			else
				strText += _AL("\t\"") + vecFileStr[i - 1];
			strText += _AL("\"\t\"") + vecFileStr[i] + _AL("\"");
			i++;
			strText += _AL("\t\"") + vecFileStr[i] + _AL("\"\r\n");
			file.Write(strText.GetBuffer(0), strText.GetLength() * sizeof(ACHAR), &dwLen);
		}
		if( !file.Close() )
			return false;
	}
	
	it2 = m_mapHelpTextTables.begin();
	for (; it2 != m_mapHelpTextTables.end(); ++it2)
	{
		strFileName = it2->first;
		abase::vector<AString> vecFileStr;
		sprintf(szPath,"%s\\info\\%s",m_szWorkPath,AC2AS(strFileName));
		AFile file;
		if (!file.Open(szPath, AFILE_CREATENEW | AFILE_BINARY | AFILE_NOHEAD))
			return false;
		HintTableFile::iterator it = it2->second->begin();

		// Unicode header.
		BYTE byteHeader[2];
		byteHeader[0] = 0xFF;
		byteHeader[1] = 0xFE;
		if( !file.Write(byteHeader, 2, &dwLen) )
			return false;
		ACString strTemp = (*it2->second)[ACString(_AL("content"))];
		int i = strTemp.Find(_AL("\\r"), 0);
		while( i != -1 )
		{
			strTemp = strTemp.Left(i) + _AL("\r") + strTemp.Mid(i + 2);
			i = strTemp.Find(_AL("\\r"), i + 1);
		}
		DWORD dwLen;
		file.Write(strTemp.GetBuffer(0), strTemp.GetLength() * sizeof(ACHAR), &dwLen);
		if( !file.Close() )
			return false;
	}

	return true;
}

bool CElementInterfaceManager::ImportTables()
{
	bool bval;
	AWScriptFile ws;
	AScriptFile as;
	ACString strFileName;
	char szPath[2000];
	int i;

	// Load string table files
	for (i = 0; i < m_vecStf.size(); i++)
	{
		strFileName = m_vecStf[i];
		sprintf(szPath,"%s\\Interfaces\\%s",m_szWorkPath,AC2AS(strFileName));
		bval = ws.Open(szPath);
		if( !bval )
			return false;

		StringTableFile * stf = new StringTableFile;
		int idString;
		ACString str;
		while( !ws.IsEnd() )
		{
			bval = ws.GetNextToken(true);
			if( !bval ) break;		// End of file.
			idString = a_atoi(ws.m_szToken);
			bval = ws.GetNextToken(true);
			if( !bval )
			{
				stf->clear();
				delete stf;
				return false;
			}
			str = ws.m_szToken;
			StringTableFile::iterator it = stf->find(idString);
			if( it != stf->end() )
				it->second = str;
			else
				(*stf)[idString] = str;
		}
		ws.Close();
		InterfaceStringTables::iterator tabit = m_mapInterfaceStringTables.find(strFileName);
		if (tabit == m_mapInterfaceStringTables.end())
		{
			m_mapInterfaceStringTables[strFileName] = stf;
		}
		else
		{
			stf->clear();
			delete stf;
			stf = 0;
		}
	}

	// Load hint table files
	for (i = 0; i < m_vecHtf.size(); i++)
	{
		strFileName = m_vecHtf[i];
		sprintf(szPath,"%s\\Interfaces\\%s",m_szWorkPath,AC2AS(strFileName));
		AXMLFile xmlfile;
		bval = xmlfile.LoadFromFile(szPath);
		if( !bval ) continue;	// Ignore error.

		HintTableFile * htf = new HintTableFile;
		AXMLItem *pItem = xmlfile.GetRootItem();
		if( pItem )
		{
			pItem = pItem->GetFirstChildItem();
			if( pItem )
			{
				pItem = pItem->GetFirstChildItem();
				while( pItem )
				{
					const ACHAR* pszName = pItem->GetValue(_AL("Name"));
					if( pszName )
					{
						AXMLItem *pChildItem = pItem->GetFirstChildItem();
						while( pChildItem )
						{
							if( a_stricmp(pChildItem->GetName(), _AL("Text")) == 0 || a_stricmp(pChildItem->GetName(), _AL("Hint")) == 0 )
							{
								const ACHAR* pszText = pChildItem->GetValue(_AL("String"));
								if( pszText )
								{
									ACString strName;
									strName.Format(_AL("%s_%s"), pszName, pChildItem->GetName());
									HintTableFile::iterator it = htf->find(strName);
									if( it != htf->end() )
										it->second = pszText;
									else
										(*htf)[strName] = pszText;
								}
							}
							pChildItem = pChildItem->GetNextItem();
						}
					}
					pItem = pItem->GetNextItem();
				}
			}
		}
		xmlfile.Release();

		InterfaceHintTables::iterator tabit = m_mapInterfaceHintTables.find(strFileName);
		if (tabit == m_mapInterfaceHintTables.end())
		{
			m_mapInterfaceHintTables[strFileName] = htf;
		}
		else
		{
			htf->clear();
			delete htf;
			htf = 0;
		}
	}

	int len = 0;
	ACHAR *strText = 0;

	// Load help script files
	for (i = 0; i < m_vecHelpScriptFile.size(); i++)
	{
		strFileName = m_vecHelpScriptFile[i];
		sprintf(szPath,"%s\\Help\\Script\\%s",m_szWorkPath,AC2AS(strFileName));
		bval = as.Open(szPath);
		if( !bval )
			return false;

		HintTableFile * htf = new HintTableFile;
		ACString name;
		ACString str;
		while( !as.IsEnd() )
		{
			bval = as.GetNextToken(true);
			if( !bval ) break;		// End of file.
			if( stricmp(as.m_szToken, "#define") == 0 )
			{
				bval = as.GetNextToken(true);
				if( !bval )
				{
					htf->clear();
					delete htf;
					return false;
				}
				len = MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), NULL, 0);
				strText = new ACHAR[len + 1];
				MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), strText, len + 1);
				strText[len] = 0;
				name = strText;
				delete []strText;
				bval = as.GetNextToken(true);
				if( !bval )
				{
					htf->clear();
					delete htf;
					return false;
				}			
				len = MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), NULL, 0);
				strText = new ACHAR[len + 1];
				MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), strText, len + 1);
				strText[len] = 0;
				(*htf)[name] = strText;
				delete []strText;			
			}
			else if( stricmp(as.m_szToken, "#name") == 0 ) 
			{
				bval = as.GetNextToken(true);
				if( !bval )
				{
					htf->clear();
					delete htf;
					return false;
				}
				name = _AL("name");
				
				len = MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), NULL, 0);
				strText = new ACHAR[len + 1];
				MultiByteToWideChar(CP_UTF8, 0, as.m_szToken, strlen(as.m_szToken), strText, len + 1);
				strText[len] = 0;
				(*htf)[name] = strText;
				delete []strText;
			}
		}
		as.Close();
		InterfaceHintTables::iterator tabit = m_mapHelpScriptTables.find(strFileName);
		if (tabit == m_mapHelpScriptTables.end())
		{
			m_mapHelpScriptTables[strFileName] = htf;
		}
		else
		{
			htf->clear();
			delete htf;
			htf = 0;
		}
	}

	// Load help text files
	for (i = 0; i < m_vecHelpTextFile.size(); i++)
	{
		strFileName = m_vecHelpTextFile[i];
		sprintf(szPath, "%s\\info\\%s", m_szWorkPath, AC2AS(strFileName));
		AFileImage file;
		if (!file.Open(szPath, AFILE_BINARY | AFILE_OPENEXIST))
			return false;

		ACHAR szText[10000];
		memset(szText, 0, 10000);
		DWORD dwLen;
		file.Read(szText, 10000 * sizeof(ACHAR), &dwLen);
		if( dwLen == 10000 )
			return false;
		
		file.Close();
		(*m_mapHelpTextTables[strFileName])[ACString(_AL("content"))] = szText + 1;
	}

	return true;
}

StringTableFile* CElementInterfaceManager::GetStringTable(const ACHAR *szFileName)
{
	InterfaceStringTables::iterator it = m_mapInterfaceStringTables.find(szFileName);
	if( it != m_mapInterfaceStringTables.end() )
		return it->second;
	else
		return NULL;
}

HintTableFile* CElementInterfaceManager::GetHintTable(const ACHAR *szFileName)
{
	InterfaceHintTables::iterator it = m_mapInterfaceHintTables.find(szFileName);
	if( it != m_mapInterfaceHintTables.end() )
		return it->second;
	else
		return NULL;
}

HintTableFile* CElementInterfaceManager::GetHelpScriptTable(const ACHAR *szFileName)
{
	InterfaceHintTables::iterator it = m_mapHelpScriptTables.find(szFileName);
	if( it != m_mapHelpScriptTables.end() )
		return it->second;
	else
		return NULL;
}

HintTableFile* CElementInterfaceManager::GetHelpTextTable(const ACHAR *szFileName)
{
	InterfaceHintTables::iterator it = m_mapHelpTextTables.find(szFileName);
	if( it != m_mapHelpTextTables.end() )
		return it->second;
	else
		return NULL;
}