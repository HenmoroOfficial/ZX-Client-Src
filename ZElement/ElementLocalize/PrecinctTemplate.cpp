// PrecinctTemplate.cpp: implementation of the CPrecinctTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementlocalize.h"
#include "PrecinctTemplate.h"
#include "PrecinctData.h"
#include "DataManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern ALog g_Log;

//////////////////////////////////////////////////////////////////////
// CPrecinctTemplate
//////////////////////////////////////////////////////////////////////

CPrecinctTemplate::~CPrecinctTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}


CString CPrecinctTemplate:: GetStrID()
{
	CString str;
	str.Format(_T("%d%s%s"), iChunkType, STRID_TOK, c_to_w(szFileName));
	return str;
}

bool CPrecinctTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&szFileName,256,1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		pNewBase->Init(false,NULL,NULL,_T("Precinct_Name"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	return true;	
}

bool CPrecinctTemplate::Save(FILE *pFile)
{
	fwrite(&szFileName,256,1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

void CPrecinctTemplate::SetTreeStatus(int status)
{
	int size = listText.size();
	for(int i = 0; i < size; ++i)
		listText[i]->SetStatus(status);
	
}

bool CPrecinctTemplate::Import(void *pOrgData)
{
	char * pData = (char *) pOrgData;
	CPrecinctDataSet dataSet;
	if(!dataSet.Load(pData)) return false;
	
	
	if(listText.size() > 0 )
	{
		for( int i = 0; i < listText.size(); ++i)
		{
			CText_Resize * pText = (CText_Resize *)listText[i];
			char *pOrg = (char*) pText->GetOriginalText();
			const TCHAR *szText = NULL;
			if(dataSet.GetTextNum() > i) szText = dataSet.GetTextByIndex(i);
			if(szText==NULL) 
			{
				listText[i]->SetStatus(CTextBase::status_disuse);
				continue;
			}
			
			int len1 = 0;
			int len2 = 0;
			if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
			if( szText != NULL) len2 = _tcslen(szText);
			if(len1==len2)
			{
				if(len1!=0)
				{
					if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)szText,sizeof(TCHAR)*len1))
					{
						listText[i]->ReplaceOriginalText((TCHAR*)szText); 
						listText[i]->SetStatus(CTextBase::status_new);
					}				
				}
			}else
			{
				listText[i]->ReplaceOriginalText((TCHAR*)szText); 
				listText[i]->SetStatus(CTextBase::status_new);
			}
		}
		
		for( i = listText.size(); i < dataSet.GetTextNum(); ++i)
		{
			CText_Resize * pText = new CText_Resize();
			const TCHAR *szText = dataSet.GetTextByIndex(i);
			pText->Init(false,(TCHAR*)szText, NULL, _T("Precinct_Name"));
			AddText(pText);
		}
	
	}else
	{
		strcpy(szFileName,pData);
		for( int i = 0; i < dataSet.GetTextNum(); ++i)
		{
			CText_Resize * pText = new CText_Resize();
			TCHAR *szText =(TCHAR*)dataSet.GetTextByIndex(i);
			pText->Init(false,szText, NULL, _T("Precinct_Name"));
			AddText(pText);
		}
	}
	
	return true;
}

//注意 pOrgData 这个地方开始查找的时候用的是相对路径，这儿却是输出路径
bool CPrecinctTemplate::Export(void *pOrgData)
{
	char * pData = (char *) pOrgData;
	CPrecinctDataSet dataSet;
	if(!dataSet.Load(pData)) return false;

	for( int i = 0; i < dataSet.GetTextNum(); ++i)
	{
		if(i < listText.size())
		{
			if(listText[i]->GetStatus()==CTextBase::status_normal && listText[i]->GetTranslationText()!=NULL)
				dataSet.SetTextByIndex(i,listText[i]->GetTranslationText());
		}
	}
	

	DWORD flag = GetFileAttributesA(pData);
	if (flag & FILE_ATTRIBUTE_READONLY) 
	{
		flag ^= FILE_ATTRIBUTE_READONLY;
		if(!SetFileAttributesA(pData,flag))
			g_Log.Log("Faild to set file attributes (%s)",pData);
	}
	if(!dataSet.Save(pData)) return false;
	
	return true;
}




