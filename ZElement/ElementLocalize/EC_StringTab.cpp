/*
 * FILE: EC_StringTab.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2006/7/3
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "stdafx.h"
#include "EC_StringTab.h"
#include "AWScriptFile.h"
#include "AMemFile.h"
#include "AFileImage.h"

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

extern CString g_strWorkingPath;

static const char* l_aFileNames[] = 
{
	"Configs\\item_desc.txt",
	"Configs\\item_ext_desc.txt",
	"Configs\\skillstr.txt",
	"Configs\\fixed_msg.txt",
	"Configs\\server_error.txt",
	"Configs\\buff_str.txt",
	"Configs\\task_err.txt",
	"Configs\\loading_hint.txt",
	"Configs\\pet_talk.txt",
	"Configs\\addon_str.txt",
};

static const wchar_t* l_aTitles[] = 
{
	L"//  Element item description.",
	L"//  Element item extend descriptions.",
	L"//  Element skill descriptions.",
	L"//  Element fixed notify messages.",
	L"//  Element server returned error messages.",
	L"//  Element buff descriptions.",
	L"//  Element task error messages.",
	L"//  Element loading hint messages.",
	L"//  Element pet talk messages.",
	L"//  Element addon string messages.",
};

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
//	Implement CECStringTab
//	
///////////////////////////////////////////////////////////////////////////

CECStringTab::CECStringTab(STRSET StrSet, char *szWorkPath)
{
	m_StrSet	= StrSet;
	m_bIndexed	= false;
	sprintf(m_szFile, "%s\\%s", szWorkPath, l_aFileNames[m_StrSet]);
}

CECStringTab::~CECStringTab()
{
	int i, iNumText = m_aStrings.GetSize();
	for (i=0; i < iNumText; i++)
	{
		TEXT* pItem = m_aStrings[i];
		if (pItem)
		{
			if (pItem->szText)
				delete [] pItem->szText;
		}

		delete pItem;
	}

	m_aStrings.RemoveAll();
}

//	Load strings from file
bool CECStringTab::Load()
{
	AWScriptFile ScriptFile;
	if (!ScriptFile.Open(m_szFile))
		return false;

	m_bIndexed = false;
	bool bFileEnd = true;

	//	Read configs
	while (ScriptFile.GetNextToken(true))
	{
		if (!wcsicmp(ScriptFile.m_szToken, L"#_index"))
			m_bIndexed = true;
		else if (!wcsicmp(ScriptFile.m_szToken, L"#_begin"))
		{
			bFileEnd = false;
			break;
		}
	}

	if (bFileEnd)
	{
		ScriptFile.Close();
		return true;
	}

	if (m_bIndexed)
	{
		//	Every string has a preset index
		while (ScriptFile.PeekNextToken(true))
		{
			int n = ScriptFile.GetNextTokenAsInt(true);
			ScriptFile.GetNextToken(false);

			if (!AddStringItem(n, ScriptFile.m_szToken))
				return false;
		}
	}
	else
	{
		int iCnt = 0;

		//	Read strings sequently
		while (ScriptFile.GetNextToken(true))
		{
			if (!AddStringItem(iCnt++, ScriptFile.m_szToken))
				return false;
		}
	}

	ScriptFile.Close();

	return true;
}

//	Save strings to file
bool CECStringTab::Save()
{
	AMemFile file;
	if (!file.Attach(NULL, 0, 8192))
		return false;
	
	DWORD dwWrite;
	wchar_t sz[4096];

	//	Write unicode header
	wchar_t wcFlag = 0xfeff;
	swprintf(sz, L"%c", wcFlag);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write file description title
	swprintf(sz, L"%s\n\n", l_aTitles[m_StrSet]);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	//	Write parameters
	if (m_bIndexed)
	{
		swprintf(sz, L"#_index\n");
		file.Write(sz, _StrLenInByte(sz), &dwWrite);
	}

	swprintf(sz, L"#_begin\n\n");
	file.Write(sz, _StrLenInByte(sz), &dwWrite);

	int i, iNumText = m_aStrings.GetSize();

	if (m_bIndexed)
	{
		for (i=0; i < iNumText; i++)
		{
			TEXT* pItem = m_aStrings[i];
			swprintf(sz, L"%d  \"%s\"\n", pItem->id, pItem->szText);
			file.Write(sz, _StrLenInByte(sz), &dwWrite);
		}
	}
	else
	{
		for (i=0; i < iNumText; i++)
		{
			TEXT* pItem = m_aStrings[i];
			swprintf(sz, L"\"%s\"\n", pItem->szText);
			file.Write(sz, _StrLenInByte(sz), &dwWrite);
		}
	}

	if (!AFileImage::WriteToPack(l_aFileNames[m_StrSet], file.GetFileBuffer(), file.GetFileLength(), true))
		return false;

	//	Open destination file
/*	FILE* fp = fopen(m_szFile, "wb");
	if (!fp)
		return false;

	//	Write unicode header
	wchar_t wcFlag = 0xfeff;
	fwprintf(fp, L"%c", wcFlag);

	//	Write file description title
	fwprintf(fp, L"%s\n\n", l_aTitles[m_StrSet]);

	//	Write parameters
	if (m_bIndexed)
		fwprintf(fp, L"#_index\n");

	fwprintf(fp, L"#_begin\n\n");

	int i, iNumText = m_aStrings.GetSize();

	if (m_bIndexed)
	{
		for (i=0; i < iNumText; i++)
		{
			TEXT* pItem = m_aStrings[i];
			fwprintf(fp, L"%d  \"%s\"\n", pItem->id, pItem->szText);
		}
	}
	else
	{
		for (i=0; i < iNumText; i++)
		{
			TEXT* pItem = m_aStrings[i];
			fwprintf(fp, L"\"%s\"\n", pItem->szText);
		}
	}

	fclose(fp);
*/
	return true;
}

//	Add a string item
bool CECStringTab::AddStringItem(int id, const TCHAR* szText)
{
	TEXT* pItem = new TEXT;
	if (!pItem)
		return false;

	pItem->id = id;

	int iLen = (_tcslen(szText) + 1) * sizeof (TCHAR);
	pItem->szText = new ACHAR[iLen];
	_tcscpy(pItem->szText, szText);

	m_aStrings.Add(pItem);

	return true;
}

//	Get string number
int CECStringTab::GetTextNum()
{
	return m_aStrings.GetSize();
}

//	Get specified text by index
CECStringTab::TEXT* CECStringTab::GetText(int iIndex)
{
	return m_aStrings[iIndex];
}

//	Get specified text by id
CECStringTab::TEXT* CECStringTab::GetTextByID(int id)
{
	int i, iNumText = m_aStrings.GetSize();
	for (i=0; i < iNumText; i++)
	{
		TEXT* pItem = m_aStrings[i];
		if (pItem && pItem->id == id)
			return pItem;
	}

	return NULL;
}

////////////////////////////////////////////////////////////////////////////////////////////////////
//class CECInstanceText
////////////////////////////////////////////////////////////////////////////////////////////////////

CECInstanceText::CECInstanceText()
{
}

CECInstanceText::~CECInstanceText()
{
	int i, iNumText = m_aStrings.GetSize();
	for (i=0; i < iNumText; i++)
	{
		_INSTANCE_TEXT* pItem = m_aStrings[i];
		if (pItem)
		{
			if (pItem->pText)
			{
				if(pItem->pText->szText) delete [] pItem->pText->szText;
				delete pItem->pText;
			}
			
			int j, iNumRegion = pItem->listRegion.size();
			for (j = 0; j < iNumRegion; j++)
			{
				if(pItem->listRegion[j].szText) delete [] pItem->listRegion[j].szText;
			}
			pItem->listRegion.clear();
		}

		delete pItem;
	}

	m_aStrings.RemoveAll();
}

bool CECInstanceText::Load(const char * szPath)
{
	AWScriptFile ScriptFile;
	if (!ScriptFile.Open(szPath))
		return false;

	int r;
	TCHAR* szName = NULL;
	while (ScriptFile.GetNextToken(true))
	{
		r =  _tcsicmp(ScriptFile.m_szToken, L"//");
		if(r==0) continue;
		r = _tcslen(ScriptFile.m_szToken);
		if(r==0) continue;
		
		_INSTANCE_TEXT *pInstanceText = new _INSTANCE_TEXT;
		
		CECStringTab::TEXT* pItem = new CECStringTab::TEXT;
		if (!pItem) return false;
		int iLen = (_tcslen(ScriptFile.m_szToken) + 1) * sizeof (TCHAR);
		pItem->szText = new ACHAR[iLen];
		_tcscpy(pItem->szText, ScriptFile.m_szToken);
		
		pInstanceText->pText = pItem; 
		
		ScriptFile.GetNextToken(true);//{
		r =  _tcsicmp(ScriptFile.m_szToken, L"{");
		if(r!=0) return false;
		pItem->id = ScriptFile.GetNextTokenAsInt(true);//id
		
		ScriptFile.GetNextToken(true);//Zone ID
		pInstanceText->listItem.push_back(ScriptFile.m_szToken);

		ScriptFile.GetNextToken(true);//map path
		pInstanceText->listItem.push_back(ScriptFile.m_szToken);

		ScriptFile.GetNextToken(true);//data path
		pInstanceText->listItem.push_back(ScriptFile.m_szToken);

		ScriptFile.GetNextToken(true);//detail texture
		pInstanceText->listItem.push_back(ScriptFile.m_szToken);

		CString temp;
		ScriptFile.GetNextToken(true);//row
		temp.Format(L"%s,",ScriptFile.m_szToken);
		ScriptFile.GetNextToken(true);//column
		temp += ScriptFile.m_szToken;
		pInstanceText->listItem.push_back(temp);

		//water color
		ScriptFile.GetNextToken(true);
		temp.Format(L"%s,",ScriptFile.m_szToken);
		ScriptFile.GetNextToken(true);
		temp += ScriptFile.m_szToken;
		temp += L",";
		ScriptFile.GetNextToken(true);
		temp += ScriptFile.m_szToken;
		pInstanceText->listItem.push_back(temp);
		
		ScriptFile.GetNextToken(true);
		temp.Format(L"%s,",ScriptFile.m_szToken);
		pInstanceText->listItem.push_back(temp);
		
		// ÇøÓòÊýÁ¿
		ScriptFile.GetNextToken(true);
		bool bThereisRegion = (0 != _tcsicmp(ScriptFile.m_szToken, L"}"));
		if (bThereisRegion)
		{
			int nRegion = _ttoi(ScriptFile.m_szToken);
			temp.Format(L"%s",ScriptFile.m_szToken);
			pInstanceText->listItem.push_back(temp);
			for (int i=0; i<nRegion; i++)
			{
				_REGION_TEXT regionText;

				ScriptFile.GetNextToken(true);
				temp.Format(L"%s ",ScriptFile.m_szToken);
				ScriptFile.GetNextToken(true);
				temp += ScriptFile.m_szToken;
				temp += L" ";
				ScriptFile.GetNextToken(true);
				temp += ScriptFile.m_szToken;
				temp += L" ";
				ScriptFile.GetNextToken(true);
				temp += ScriptFile.m_szToken;

				regionText.sItem = temp;

				// Region Int
				ScriptFile.GetNextToken(true);
				regionText.sIntValue = ScriptFile.m_szToken;

				// Region Text
				ScriptFile.GetNextToken(true);
				regionText.szText = new TCHAR[_tcslen(ScriptFile.m_szToken)+1];
				_tcscpy(regionText.szText, ScriptFile.m_szToken);

				pInstanceText->listRegion.push_back(regionText);
			}
			ScriptFile.GetNextToken(true);
		}
		
		r =  _tcsicmp(ScriptFile.m_szToken, L"}");
		if(r!=0) return false;

		m_aStrings.Add(pInstanceText);
		
	}

	ScriptFile.Close();

	return true;
}

bool CECInstanceText::Save(const char * szPath)
{
	AMemFile file;
	if (!file.Attach(NULL, 0, 4096))
		return false;
	
	DWORD dwWrite;
	wchar_t sz[2048];

	//	Write unicode header
	wchar_t wcFlag = 0xfeff;
	swprintf(sz, L"%c", wcFlag);
	file.Write(sz, _StrLenInByte(sz), &dwWrite);
	
	int i, iNumText = m_aStrings.GetSize(), j;
	for (i=0; i < iNumText; i++)
	{
		_INSTANCE_TEXT* pItem = m_aStrings[i];
		if (pItem)
		{
			swprintf(sz, L"%s", L"////////////////////////\r\n");
			file.Write(sz, _StrLenInByte(sz), &dwWrite);

			swprintf(sz, L"\"%s\"\r\n", pItem->pText->szText);
			file.Write(sz, _StrLenInByte(sz), &dwWrite);

			swprintf(sz, L"%s\r\n", L"{");
			file.Write(sz, _StrLenInByte(sz), &dwWrite);
			swprintf(sz, L"%d\r\n", pItem->pText->id);
			file.Write(sz, _StrLenInByte(sz), &dwWrite);
			
			for( j = 0; j < pItem->listItem.size(); ++j)
			{
				if(j > 0 && j < 4)
					swprintf(sz, L"\"%s\"\r\n", pItem->listItem[j]);
				else swprintf(sz, L"%s\r\n", pItem->listItem[j]);
				file.Write(sz, _StrLenInByte(sz), &dwWrite);
			}

			for( j = 0; j < pItem->listRegion.size(); j++)
			{
				swprintf(sz, L"%s \"%s\" \"%s\"\r\n", pItem->listRegion[j].sItem, pItem->listRegion[j].sIntValue, pItem->listRegion[j].szText);
				file.Write(sz, _StrLenInByte(sz), &dwWrite);
			}
			
			swprintf(sz, L"%s\r\n", L"}");
			file.Write(sz, _StrLenInByte(sz), &dwWrite);
		}
	}

	if (!AFileImage::WriteToPack(szPath, file.GetFileBuffer(), file.GetFileLength(), true))
		return false;
	
	return true;
}

int CECInstanceText::GetTextNum()
{
	return m_aStrings.GetSize();
}

CECInstanceText::_INSTANCE_TEXT* CECInstanceText::GetText( int idx)
{
	return m_aStrings[idx];
}
	
CECInstanceText::_INSTANCE_TEXT* CECInstanceText::GetTextByID(int id)
{
	int i, iNumText = m_aStrings.GetSize();
	for (i=0; i < iNumText; i++)
	{
		_INSTANCE_TEXT* pItem = m_aStrings[i];
		if (pItem && pItem->pText && pItem->pText->id == id)
			return pItem;
	}
	return NULL;
}

