// TemplateBase.cpp: implementation of the CTemplateBase class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementLocalize.h"
#include "TemplateBase.h"
#include "EC_StringTab.h"
#include "../ZElementData/ExpTypes.h"
#include "treasure.h"
#include "TextFileMgr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern ALog g_Log;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
struct LogErr
{
	LogErr(const char* szAction, int chunk, int type, int id) : _pszAction(szAction), _chunk(chunk), _type(type), _id(id)
	{
		_pszFile = NULL;
		_bSuc = false;
	}
	void File(const unsigned short* pszFile)
	{
		_pszFile = pszFile;
	}
	void Suc()
	{
		_bSuc = true;
	}
	~LogErr()
	{
		if (!_bSuc)
		{
			if (_pszFile)
			{
				char sz[MAX_PATH] = {0};
				WideCharToMultiByte(CP_ACP, 0, _pszFile, wcslen(_pszFile), sz, MAX_PATH, NULL, NULL);
				g_Log.Log("%s %s template chunk(%d) type(%d) id(%d) fail", _pszAction, sz, _type, _id); 
			}
			else
				g_Log.Log("%s template chunk(%d) type(%d) id(%d) fail", _pszAction,  _type, _id); 
		}
	}
	const char* _pszAction;
	const unsigned short* _pszFile;
	int _chunk;
	int _type;
	int _id;
	bool _bSuc;
};

CTemplateBase::CTemplateBase()
{
	id = 0;
	template_type = -1;
	pTempParent = NULL;
	iChunkType = -1;
	pTempParent = NULL;
}

CTemplateBase::~CTemplateBase()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

CString CTemplateBase::GetStrID()
{
	CString strID;
	strID.Format(_T("%d%s%d%s%d"), iChunkType, STRID_TOK, template_type, STRID_TOK, id);
	if (pTempParent)
		strID.Format(_T("%s%s%d%s%d%s%d"), pTempParent->GetStrID(), STRID_TOK, index, STRID_TOK, template_type, STRID_TOK, id);
	strID.MakeLower();
	return strID;
}

bool CTemplateBase::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementChangeShapeCardEssence
//////////////////////////////////////////////////////////////////////
CTemplate_ElementChangeShapeCardEssence::~CTemplate_ElementChangeShapeCardEssence()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementChangeShapeCardEssence::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);	
	
	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("preface"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("epilogue"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l.Suc();
	return true;
}

bool CTemplate_ElementChangeShapeCardEssence::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l.Suc();
	return true;
}

bool CTemplate_ElementChangeShapeCardEssence::Import(void *pOrgData)
{
	CHANGE_SHAPE_CARD_ESSENCE * pData = (CHANGE_SHAPE_CARD_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //preface[16];
	   pOrg = (char*)listText[1]->GetOriginalText();	
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->preface,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->preface); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //epilogue[16];
	   pOrg = (char*)listText[2]->GetOriginalText();	
	   len = listText[2]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->epilogue,sizeof(TCHAR)*len))
	   {
		  listText[2]->ReplaceOriginalText(pData->epilogue); 
		  listText[2]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[2];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//preface[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->preface, NULL, _T("preface"));
		AddText(pText);
		//epilogue[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->epilogue, NULL, _T("epilogue"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementChangeShapeCardEssence::Export(void *pOrgData)
{
	CHANGE_SHAPE_CARD_ESSENCE * pData = (CHANGE_SHAPE_CARD_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	
	//preface[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->preface, listText[1]->GetTranslationText());

	//epilogue[16]
	if( listText[2]->GetStatus() == CTextBase::status_normal && listText[2]->GetTranslationText()!=NULL)
		lstrcpy(pData->epilogue, listText[2]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementItemTrade
//////////////////////////////////////////////////////////////////////
CTemplate_ElementItemTrade::~CTemplate_ElementItemTrade()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementItemTrade::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);	
	LogErr l("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	if(dwVersion < 0x10000006) return true;
	
	for( int i = 0; i < 4; ++i)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(8,false,NULL,NULL,_T("page_title"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}

	l.Suc();
	return true;
}

bool CTemplate_ElementItemTrade::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	l.Suc();
	return true;
}

bool CTemplate_ElementItemTrade::Import(void *pOrgData)
{
	ITEM_TRADE_CONFIG * pData = (ITEM_TRADE_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
	{
	   CTextBase *pText = listText[0];
	   TCHAR* pStr = pText->GetTranslationText();
	   int nTranslationTextLen = pStr?_tcslen(pStr):0;
		if (nTranslationTextLen > 0)
		{
			pText->SetStatus(CTextBase::status_normal);
		}
	}

	   //page_title[8]
	   if(listText.size() > 1)
	   {//版本 0x10000006 前没有后面8个字符串
		   for( int i = 0; i < 4; ++i)
		   {
			   pOrg = (char*)listText[i+1]->GetOriginalText();
			   len = listText[i+1]->GetCharUpperLimit();
			   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->pages[i].page_title,sizeof(TCHAR)*len))
			   {
				   listText[i+1]->ReplaceOriginalText(pData->pages[i].page_title); 
				   listText[i+1]->SetStatus(CTextBase::status_new);
			   }
			   else
				{
				   CTextBase *pText = listText[i+1];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
		   }
	   }else
	   {
		   for( int i = 0; i < 4; ++i)
		   {
			   CTextBase *pText = new CText_Normal();
			   pText->Init(8,false,pData->pages[i].page_title, NULL, _T("page_title"));
			   AddText(pText);
		   }
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//page_title[8]
		for( int i = 0; i < 4; ++i)
		{
			pText = new CText_Normal();
			pText->Init(8,false,pData->pages[i].page_title, NULL, _T("page_title"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementItemTrade::Export(void *pOrgData)
{
	ITEM_TRADE_CONFIG * pData = (ITEM_TRADE_CONFIG *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	
	//page_title[8]
	if(listText.size() == 5)
	{
		for( int i = 0; i < 4; ++i)
		{
			if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
				lstrcpy(pData->pages[i].page_title, listText[i+1]->GetTranslationText());
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementInstancingBattleConfig
//////////////////////////////////////////////////////////////////////
CTemplate_ElementInstancingBattleConfig::~CTemplate_ElementInstancingBattleConfig()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementInstancingBattleConfig::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	
	LogErr l("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Desc"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for( int i = 0; i < 20; ++i)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(16,false,NULL,NULL,_T("tollgate_name"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);

		pNewBase = new CText_Normal;
		pNewBase->Init(16,false,NULL,NULL,_T("introduce"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}

	l.Suc();
	return true;
}

bool CTemplate_ElementInstancingBattleConfig::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l.Suc();
	return true;
}

bool CTemplate_ElementInstancingBattleConfig::Import(void *pOrgData)
{
	INSTANCING_BATTLE_CONFIG * pData = (INSTANCING_BATTLE_CONFIG *) pOrgData;
	int i;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //decs[16]
	   pOrg = (char*)listText[1]->GetOriginalText();	
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->desc); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   for (i=0; i<20; ++i)
	   {		   
		   pOrg = (char*)listText[i*2 + 2]->GetOriginalText();	
		   int len = listText[i*2 + 2]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->tollgate[i].tollgate_name,sizeof(TCHAR)*len))
		   {
			  listText[i*2 + 2]->ReplaceOriginalText(pData->tollgate[i].tollgate_name);
			  listText[i*2 + 2]->SetStatus(CTextBase::status_new);
		   }
		   else
			{
			   CTextBase *pText = listText[i*2 + 2];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}

		  
		   pOrg = (char*)listText[i*2 + 3]->GetOriginalText();	
		   len = listText[i*2 + 3]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->tollgate[i].introduce,sizeof(TCHAR)*len))
		   {
			  listText[i*2 + 3]->ReplaceOriginalText(pData->tollgate[i].introduce); 
			  listText[i*2 + 3]->SetStatus(CTextBase::status_new);
		   }
		   else
			{
			   CTextBase *pText = listText[i*2 + 3];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}	   
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);

		//desc[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->desc, NULL, _T("Desc"));
		AddText(pText);
		for( i = 0; i < 20; ++i)
		{
			pText = new CText_Normal();
			pText->Init(16,false,pData->tollgate[i].tollgate_name, NULL, _T("tollgate_name"));
			AddText(pText);
				   

			pText = new CText_Normal();
			pText->Init(16,false,pData->tollgate[i].introduce, NULL, _T("introduce"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementInstancingBattleConfig::Export(void *pOrgData)
{
	INSTANCING_BATTLE_CONFIG * pData = (INSTANCING_BATTLE_CONFIG *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());

	//desc[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[1]->GetTranslationText());
	
	//tollgate[20]
	for( int i = 0; i < 20; ++i)
	{
		if( listText[i*2 + 2]->GetStatus() == CTextBase::status_normal && listText[i*2 + 2]->GetTranslationText()!=NULL)
			lstrcpy(pData->tollgate[i].tollgate_name, listText[i*2 + 2]->GetTranslationText());
		if( listText[i*2 + 3]->GetStatus() == CTextBase::status_normal && listText[i*2 + 3]->GetTranslationText()!=NULL)
			lstrcpy(pData->tollgate[i].introduce, listText[i*2 + 3]->GetTranslationText());
	}
	return true;
}




//////////////////////////////////////////////////////////////////////
// CTemplate_ElementTextFile
//////////////////////////////////////////////////////////////////////

CTemplate_ElementTextFile::~CTemplate_ElementTextFile()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	if(m_pIndex) delete[] m_pIndex;
}

bool CTemplate_ElementTextFile::Load(FILE *pFile, DWORD dwVersion)
{
	
	fread(&m_nIndexLen,sizeof(int),1,pFile);
	m_pIndex = new TCHAR[m_nIndexLen];
	fread(m_pIndex,m_nIndexLen*sizeof(TCHAR),1,pFile);

	LogErr l1("load", iChunkType, template_type, id);
	l1.File(m_pIndex);
	
	bool bLua = true;
	CString temp(m_pIndex);
	temp.MakeLower();
	if(temp.Find(_T(".lua"))==-1)
		bLua = false;

	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = NULL;
		if(bLua) 
		{
			pNewBase = new CText_ScriptFile;
			pNewBase->Init(true,NULL,NULL,_T("Script_file"));
		}else 
		{
			pNewBase = new CText_Resize;
			pNewBase->Init(true,NULL,NULL,_T("Text_file"));
		}
		
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementTextFile::Save(FILE *pFile)
{
	fwrite(&m_nIndexLen,sizeof(int),1,pFile);
	fwrite(m_pIndex,sizeof(TCHAR)*m_nIndexLen,1,pFile);
	
	LogErr l1("save", iChunkType, template_type, id);
	l1.File(m_pIndex);

	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementTextFile::Import(void *pOrgData)
{
	FILE_DATA * pData = (FILE_DATA *) pOrgData;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		if(_tcsicmp(pData->szIndex,m_pIndex)!=0) return false;
		
		int len1 = 0;
		int len2 = 0;
		len1 = _tcslen(pData->szText);
		len2 = _tcslen(listText[0]->GetOriginalText());
		if(len1 == len2)
		{
			if(len1!=0)
			{
				if(!CTextBase::DataIsEqual((char*)pData->szText,len1*sizeof(TCHAR),(char*)listText[0]->GetOriginalText(),sizeof(TCHAR)*len1))
			   {
				   listText[0]->ReplaceOriginalText(pData->szText); 
				   listText[0]->SetStatus(CTextBase::status_new);
			   }
				else
				{
				   CTextBase *pText = listText[0];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
			}
		}else
		{
			listText[0]->ReplaceOriginalText(pData->szText); 
			listText[0]->SetStatus(CTextBase::status_new);
		}
	}else
	{
		m_nIndexLen = _tcslen(pData->szIndex) + 1;
		m_pIndex = new TCHAR[m_nIndexLen];
		lstrcpy(m_pIndex,pData->szIndex);

		bool bLua = true;
		CString temp(m_pIndex);
		temp.MakeLower();
		if(temp.Find(_T(".lua"))==-1)
			bLua = false;
		
		CText_Resize * pText = NULL;
		if(bLua)
		{
			pText = new CText_ScriptFile();
			pText->Init(false,pData->szText, NULL, _T("script_file"));
		}else
		{
			pText = new CText_Resize();
			pText->Init(false,pData->szText, NULL, _T("Text_file"));
		}
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementTextFile::Export(void *pOrgData)
{
	FILE_DATA * pData = (FILE_DATA *) pOrgData;
	LogErr l1("export", iChunkType, template_type, id);
	l1.File(m_pIndex);
	if(listText.size()!=1) return false;
	if(_tcsicmp(pData->szIndex,m_pIndex)!=0) return false;

	if(listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		CString temp(listText[0]->GetTranslationText());
		lstrcpy(pData->szText,temp);
	}
	l1.Suc();
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementNpcTransmit
//////////////////////////////////////////////////////////////////////
CTemplate_ElementNpcTransmit::~CTemplate_ElementNpcTransmit()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementNpcTransmit::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	if(dwVersion < 0x10000002) return true;
	
	for( int i = 0; i < 16; ++i)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(16,false,NULL,NULL,_T("Targets_Name"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementNpcTransmit::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);

	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementNpcTransmit::Import(void *pOrgData)
{
	NPC_TRANSMIT_SERVICE * pData = (NPC_TRANSMIT_SERVICE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //targets[16]
	   if(listText.size() > 1)
	   {//版本 0x10000002 前没有后面16个字符串
		   for( int i = 0; i < 16; ++i)
		   {
			   pOrg = (char*)listText[i+1]->GetOriginalText();
			   len = listText[i+1]->GetCharUpperLimit();
			   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->targets[i].name,sizeof(TCHAR)*len))
			   {
				   listText[i+1]->ReplaceOriginalText(pData->targets[i].name); 
				   listText[i+1]->SetStatus(CTextBase::status_new);
			   }
			   else
				{
				   CTextBase *pText = listText[i+1];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
		   }
	   }else
	   {
		   for( int i = 0; i < 16; ++i)
		   {
			   CTextBase *pText = new CText_Normal();
			   pText->Init(16,false,pData->targets[i].name, NULL, _T("Targets_Name"));
			   AddText(pText);
		   }
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//targets[16]
		for( int i = 0; i < 16; ++i)
		{
			pText = new CText_Normal();
			pText->Init(16,false,pData->targets[i].name, NULL, _T("Targets_Name"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementNpcTransmit::Export(void *pOrgData)
{
	NPC_TRANSMIT_SERVICE * pData = (NPC_TRANSMIT_SERVICE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	
	//targets[16]
	if(listText.size() == 17)
	{
		for( int i = 0; i < 16; ++i)
		{
			if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
				lstrcpy(pData->targets[i].name, listText[i+1]->GetTranslationText());
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementTalisman
//////////////////////////////////////////////////////////////////////
CTemplate_ElementTalisman::~CTemplate_ElementTalisman()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementTalisman::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=2) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Show_Level"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l1.Suc();
	return true;
}

bool CTemplate_ElementTalisman::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementTalisman::Import(void *pOrgData)
{
	TALISMAN_MAINPART_ESSENCE * pData = (TALISMAN_MAINPART_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //Show_Level[16]
	   pOrg = (char*)listText[1]->GetOriginalText();
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->show_level,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->show_level); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//show_level[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->show_level, NULL, _T("Show_Level"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementTalisman::Export(void *pOrgData)
{
	TALISMAN_MAINPART_ESSENCE * pData = (TALISMAN_MAINPART_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//Show_Level[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->show_level, listText[1]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementDropInternalConfig
//////////////////////////////////////////////////////////////////////////
CTemplate_ElementDropInternalConfig::~CTemplate_ElementDropInternalConfig()
{
	for (int i = 0; i < listText.size(); ++i)
		if (listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementDropInternalConfig::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=3) return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(100,false,NULL,NULL,_T("Speak_Words"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(100,false,NULL,NULL,_T("Remain_Words"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementDropInternalConfig::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementDropInternalConfig::Import(void *pOrgData)
{
	DROP_INTERNAL_CONFIG * pData = (DROP_INTERNAL_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//speak_words[100]
		listText[1]->UpdateFromData(pData->speak_words);

		//remain_words[100]
		listText[2]->UpdateFromData(pData->remain_words);
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);

		//speak_words[100]
		pText = new CText_Normal();
		pText->Init(100,false,pData->speak_words, NULL, _T("speak_words"));
		AddText(pText);
		
		//remain_words[100]
		pText = new CText_Normal();
		pText->Init(100,false,pData->remain_words, NULL, _T("remain_words"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementDropInternalConfig::Export(void *pOrgData)
{
	DROP_INTERNAL_CONFIG * pData = (DROP_INTERNAL_CONFIG *) pOrgData;

	listText[0]->ExportToData(pData->name);
	listText[1]->ExportToData(pData->speak_words);
	listText[2]->ExportToData(pData->remain_words);

	return true;
}


//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementCollRaidTransfigConfig
//////////////////////////////////////////////////////////////////////////
CTemplate_ElementCollRaidTransfigConfig::~CTemplate_ElementCollRaidTransfigConfig()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementCollRaidTransfigConfig::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=21) return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for (int i=0; i<20; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(100,false,NULL,NULL,_T("Desc"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementCollRaidTransfigConfig::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementCollRaidTransfigConfig::Import(void *pOrgData)
{
	COLLISION_RAID_TRANSFIGURE_CONFIG * pData = (COLLISION_RAID_TRANSFIGURE_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//description[100]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->transfigure_info_list[i-1].description);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//description[100]
		for (int i=0; i<20; i++)
		{
			pText = new CText_Normal();
			pText->Init(100,false,pData->transfigure_info_list[i].description, NULL, _T("Desc"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementCollRaidTransfigConfig::Export(void *pOrgData)
{
	COLLISION_RAID_TRANSFIGURE_CONFIG * pData = (COLLISION_RAID_TRANSFIGURE_CONFIG *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->transfigure_info_list[i-1].description);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementNpcRepShopSvc
//////////////////////////////////////////////////////////////////////////
bool CTemplate_ElementNpcRepShopSvc::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=5) return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for (int i=0; i<4; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(8,false,NULL,NULL,_T("PageTitle"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementNpcRepShopSvc::Import(void *pOrgData)
{
	NPC_REPUTATION_SHOP_SERVICE * pData = (NPC_REPUTATION_SHOP_SERVICE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//pages[4]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->pages[i-1].page_title);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//description[100]
		for (int i=0; i<4; i++)
		{
			pText = new CText_Normal();
			pText->Init(8, false, pData->pages[i].page_title, NULL, _T("PageTitle"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementNpcRepShopSvc::Export(void *pOrgData)
{
	NPC_REPUTATION_SHOP_SERVICE * pData = (NPC_REPUTATION_SHOP_SERVICE *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->pages[i-1].page_title);
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementUITransferCfg
//////////////////////////////////////////////////////////////////////////
bool CTemplate_ElementUITransferCfg::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=97) 
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	for (int i=0; i<96; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		if (dwVersion < 0x10000012)
			pNewBase->Init(8,false,NULL,NULL,_T("TransName"));
		else
			pNewBase->Init(16, false, NULL, NULL, _T("TransName"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		if (dwVersion < 0x10000012)
			pNewBase->ChangeLmtSize(16);
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementUITransferCfg::Import(void *pOrgData)
{
	UI_TRANSFER_CONFIG * pData = (UI_TRANSFER_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//trans_point_list[96]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->trans_point_list[i-1].name);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//trans_point_list[96]
		for (int i=0; i<96; i++)
		{
			pText = new CText_Normal();
			pText->Init(16, false, pData->trans_point_list[i].name, NULL, _T("TransName"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementUITransferCfg::Export(void *pOrgData)
{
	UI_TRANSFER_CONFIG * pData = (UI_TRANSFER_CONFIG *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->trans_point_list[i-1].name);
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementLivenessCfg
//////////////////////////////////////////////////////////////////////////
bool CTemplate_ElementLivenessCfg::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=121) 
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for (int i=0; i<120; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(16, false, NULL, NULL, _T("ElementName"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementLivenessCfg::Import(void *pOrgData)
{
	LIVENESS_CONFIG * pData = (LIVENESS_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//element_list[120]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->element_list[i-1].name);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//element_list[120]
		for (int i=0; i<120; i++)
		{
			pText = new CText_Normal();
			pText->Init(16, false, pData->element_list[i].name, NULL, _T("ElementName"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementLivenessCfg::Export(void *pOrgData)
{
	LIVENESS_CONFIG * pData = (LIVENESS_CONFIG *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->element_list[i-1].name);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementSalePromotionItem
//////////////////////////////////////////////////////////////////////////
bool CTemplate_ElementSalePromotionItem::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=21) 
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for (int i=0; i<20; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(100, false, NULL, NULL, _T("ElementDesc"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementSalePromotionItem::Import(void *pOrgData)
{
	SALE_PROMOTION_ITEM_ESSENCE * pData = (SALE_PROMOTION_ITEM_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//element_list[20]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->element_list[i-1].desc);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//element_list[20]
		for (int i=0; i<20; i++)
		{
			pText = new CText_Normal();
			pText->Init(100, false, pData->element_list[i].desc, NULL, _T("ElementDesc"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementSalePromotionItem::Export(void *pOrgData)
{
	SALE_PROMOTION_ITEM_ESSENCE * pData = (SALE_PROMOTION_ITEM_ESSENCE *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->element_list[i-1].desc);
	}
	
	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_ElementTaskSpecialAwardCfg
//////////////////////////////////////////////////////////////////////////
bool CTemplate_ElementTaskSpecialAwardCfg::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=11) 
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	for (int i=0; i<10; i++)
	{
		CTextBase *pNewBase = new CText_Normal;
		pNewBase->Init(8, false, NULL, NULL, _T("mini_game"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementTaskSpecialAwardCfg::Import(void *pOrgData)
{
	TASK_SPECIAL_AWARD_CONFIG * pData = (TASK_SPECIAL_AWARD_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		
		//name[32];
		listText[0]->UpdateFromData(pData->name);
		
		//mini_game[10]
		for (int i=1; i<listText.size(); i++)
		{
			listText[i]->UpdateFromData(pData->mini_game[i-1]);
		}
		
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//mini_game[10]
		for (int i=0; i<10; i++)
		{
			pText = new CText_Normal();
			pText->Init(8, false, pData->mini_game[i], NULL, _T("mini_game"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementTaskSpecialAwardCfg::Export(void *pOrgData)
{
	TASK_SPECIAL_AWARD_CONFIG * pData = (TASK_SPECIAL_AWARD_CONFIG *) pOrgData;
	
	listText[0]->ExportToData(pData->name);
	for (int i=1; i<listText.size(); i++)
	{
		listText[i]->ExportToData(pData->mini_game[i-1]);
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementFireworks
//////////////////////////////////////////////////////////////////////
CTemplate_ElementFireworks::~CTemplate_ElementFireworks()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementFireworks::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=2) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(256,false,NULL,NULL,_T("Char_Can_Use"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l1.Suc();
	return true;
}

bool CTemplate_ElementFireworks::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementFireworks::Import(void *pOrgData)
{
	TEXT_FIREWORKS_ESSENCE * pData = (TEXT_FIREWORKS_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //char_can_use[256]
	   pOrg = (char*)listText[1]->GetOriginalText();
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->char_can_use,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->char_can_use); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//char_can_use[256]
		pText = new CText_Normal();
		pText->Init(256,false,pData->char_can_use, NULL, _T("char_can_use"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementFireworks::Export(void *pOrgData)
{
	TEXT_FIREWORKS_ESSENCE * pData = (TEXT_FIREWORKS_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//char_can_use[256]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->char_can_use, listText[1]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementLottery
//////////////////////////////////////////////////////////////////////
CTemplate_ElementLottery::~CTemplate_ElementLottery()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementLottery::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if (size != 1 && size != 33)
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	if(size == 33)
	{
		for( int i = 0; i < 32; ++i)
		{
			pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Candidates_Desc"));
			r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}
	}
	else
	{
		for( int i = 0; i < 32; ++i)
		{
			pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Candidates_Desc"));
			AddText(pNewBase);
		}
	}
	l1.Suc();

	return true;
}

bool CTemplate_ElementLottery::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementLottery::Import(void *pOrgData)
{
	LOTTERY_ESSENCE * pData = (LOTTERY_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
		else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //candidates[32]
	   for( int i = 0; i < 32; ++i)
	   {
		   pOrg = (char*)listText[i+1]->GetOriginalText();
		   len = listText[i+1]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->candidates[i].desc,sizeof(TCHAR)*len))
		   {
			   listText[i+1]->ReplaceOriginalText(pData->candidates[i].desc); 
			   listText[i+1]->SetStatus(CTextBase::status_new);
		   }
		   else
			{
			   CTextBase *pText = listText[i+1];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//candidates[32]
		for( int i = 0; i < 32; ++i)
		{
			pText = new CText_Normal();
			pText->Init(16,false,pData->candidates[i].desc, NULL, _T("Candidates_Desc"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementLottery::Export(void *pOrgData)
{
	LOTTERY_ESSENCE * pData = (LOTTERY_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//candidates[32]
	for( int i = 0; i < 32; ++i)
	{
		if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
		lstrcpy(pData->candidates[i].desc, listText[i+1]->GetTranslationText());
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementLottery2
//////////////////////////////////////////////////////////////////////
CTemplate_ElementLottery2::~CTemplate_ElementLottery2()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementLottery2::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if (size != 1 && size != 33)
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	if(size == 33)
	{
		for( int i = 0; i < 32; ++i)
		{
			pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Candidates_Desc"));
			r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}
	}
	else
	{
		for( int i = 0; i < 32; ++i)
		{
			pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Candidates_Desc"));
			AddText(pNewBase);
		}
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementLottery2::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementLottery2::Import(void *pOrgData)
{
	LOTTERY_ESSENCE * pData = (LOTTERY_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
		else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //candidates[32]
	   for( int i = 0; i < 32; ++i)
	   {
		   pOrg = (char*)listText[i+1]->GetOriginalText();
		   len = listText[i+1]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->candidates[i].desc,sizeof(TCHAR)*len))
		   {
			   listText[i+1]->ReplaceOriginalText(pData->candidates[i].desc); 
			   listText[i+1]->SetStatus(CTextBase::status_new);
		   }
		   else
			{
			   CTextBase *pText = listText[i+1];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//candidates[32]
		for( int i = 0; i < 32; ++i)
		{
			pText = new CText_Normal();
			pText->Init(16,false,pData->candidates[i].desc, NULL, _T("Candidates_Desc"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementLottery2::Export(void *pOrgData)
{
	LOTTERY_ESSENCE * pData = (LOTTERY_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//candidates[32]
	for( int i = 0; i < 32; ++i)
	{
		if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
		lstrcpy(pData->candidates[i].desc, listText[i+1]->GetTranslationText());
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementScoreToRank
//////////////////////////////////////////////////////////////////////
CTemplate_ElementScoreToRank::~CTemplate_ElementScoreToRank()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementScoreToRank::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if (size != 11)
		return false;
	
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	for( int i = 0; i < 10; ++i)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(16,false,NULL,NULL,_T("Rank_Name"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementScoreToRank::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementScoreToRank::Import(void *pOrgData)
{
	SCORE_TO_RANK_CONFIG * pData = (SCORE_TO_RANK_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
		else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //Rank_Name[16]
	   for( int i = 0; i < 10; ++i)
	   {
		   pOrg = (char*)listText[i+1]->GetOriginalText();
		   len = listText[i+1]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->score2rank[i].rank_name,sizeof(TCHAR)*len))
		   {
			   listText[i+1]->ReplaceOriginalText(pData->score2rank[i].rank_name); 
			   listText[i+1]->SetStatus(CTextBase::status_new);
		   }
		   else
			{
			   CTextBase *pText = listText[i+1];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}
	   }
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//Rank_Name[16]
		for( int i = 0; i < 10; ++i)
		{
			pText = new CText_Normal();
			pText->Init(16,false,pData->score2rank[i].rank_name, NULL, _T("Rank_Name"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementScoreToRank::Export(void *pOrgData)
{
	SCORE_TO_RANK_CONFIG * pData = (SCORE_TO_RANK_CONFIG *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//Rank_Name[16]
	for( int i = 0; i < 10; ++i)
	{
		if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
		lstrcpy(pData->score2rank[i].rank_name, listText[i+1]->GetTranslationText());
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementEquipment
//////////////////////////////////////////////////////////////////////
CTemplate_ElementEquipment::~CTemplate_ElementEquipment()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementEquipment::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=2) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Show_Level"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l1.Suc();
	return true;
}

bool CTemplate_ElementEquipment::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementEquipment::Import(void *pOrgData)
{
	EQUIPMENT_ESSENCE * pData = (EQUIPMENT_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //Show_Level[16]
	   pOrg = (char*)listText[1]->GetOriginalText();
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->show_level,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->show_level); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//Show_Level[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->show_level, NULL, _T("Show_Level"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementEquipment::Export(void *pOrgData)
{
	EQUIPMENT_ESSENCE * pData = (EQUIPMENT_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//Show_Level[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->show_level, listText[1]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementMonster
//////////////////////////////////////////////////////////////////////

CTemplate_ElementMonster::~CTemplate_ElementMonster()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementMonster::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=3 && size != 4) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Prop"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Desc"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Monster_Desc"));
	if (size == 4)
	{
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
	}
	AddText(pNewBase);
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementMonster::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementMonster::Import(void *pOrgData)
{
	MONSTER_ESSENCE * pData = (MONSTER_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //prop[16]
	   pOrg = (char*)listText[1]->GetOriginalText();
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->prop,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->prop); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //desc[16];
	   pOrg = (char*)listText[2]->GetOriginalText();	
	   len = listText[2]->GetCharUpperLimit();
		if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
	   {
		  listText[2]->ReplaceOriginalText(pData->desc); 
		  listText[2]->SetStatus(CTextBase::status_new);
	   }
		else
		{
		   CTextBase *pText = listText[2];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

		//monster_desc[16];
	   pOrg = (char*)listText[3]->GetOriginalText();	
	   len = listText[3]->GetCharUpperLimit();
		if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->monster_desc,sizeof(TCHAR)*len))
	   {
		  listText[3]->ReplaceOriginalText(pData->monster_desc); 
		  listText[3]->SetStatus(CTextBase::status_new);
	   }
		else
		{
		   CTextBase *pText = listText[3];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//prop[16]
		pText = new CText_Normal();
		pText->Init(16,false,pData->prop, NULL, _T("Prop"));
		AddText(pText);
		//desc[16];
		pText = new CText_Normal();
		pText->Init(16,false,pData->desc, NULL, _T("Desc"));
		AddText(pText);
		//monster_desc[16];
		pText = new CText_Normal();
		pText->Init(16,false,pData->monster_desc, NULL, _T("Monster_Desc"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementMonster::Export(void *pOrgData)
{
	MONSTER_ESSENCE * pData = (MONSTER_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//prop[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->prop, listText[1]->GetTranslationText());
	//desc[16];
	if( listText[2]->GetStatus() == CTextBase::status_normal && listText[2]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[2]->GetTranslationText());
	//monster_desc[16];
	if( listText[3]->GetStatus() == CTextBase::status_normal && listText[3]->GetTranslationText()!=NULL)
		lstrcpy(pData->monster_desc, listText[3]->GetTranslationText());
	
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTreasureTemplate
//////////////////////////////////////////////////////////////////////

CTreasureTemplate::~CTreasureTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTreasureTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=1) return false;

	CText_Normal *pNewBase = new CText_Normal;
	pNewBase->Init(64,true,NULL,NULL,_T("treasure_desc"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	l1.Suc();
	return true;
}

bool CTreasureTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTreasureTemplate::Import(void *pOrgData)
{
	TREASURE_ITEM * pData = (TREASURE_ITEM *) pOrgData;	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		//desc[64];
		char * pOrg = (char*)listText[0]->GetOriginalText();	
		int len = listText[0]->GetCharUpperLimit();
		if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
		{
			listText[0]->ReplaceOriginalText(pData->desc); 
			listText[0]->SetStatus(CTextBase::status_new);
		}
		else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//desc[64];
		CTextBase * pText = new CText_Normal();
		pText->Init(64,true,pData->desc, NULL, _T("treasure_desc"));
		AddText(pText);
	
	}
	return true;
}

bool CTreasureTemplate::Export(void *pOrgData)
{
	TREASURE_ITEM * pData = (TREASURE_ITEM *) pOrgData;
	//desc[64];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[0]->GetTranslationText());

	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementNpcSell
//////////////////////////////////////////////////////////////////////

CTemplate_ElementNpcSell::~CTemplate_ElementNpcSell()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementNpcSell::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=9) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	for( int i = 0; i < 8; ++i)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(8,false,NULL,NULL,_T("Page_name"));
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementNpcSell::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementNpcSell::Import(void *pOrgData)
{
	NPC_SELL_SERVICE * pData = (NPC_SELL_SERVICE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
		//name[32];
		char * pOrg = (char*)listText[0]->GetOriginalText();	
		int len = listText[0]->GetCharUpperLimit();
		if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
		{
			listText[0]->ReplaceOriginalText(pData->name); 
			listText[0]->SetStatus(CTextBase::status_new);
		}
		else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
		for( int i = 0; i < 8; ++i)
		{
			pOrg = (char*)listText[i+1]->GetOriginalText();	
			len = listText[i+1]->GetCharUpperLimit();
			if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->pages[i].page_title,sizeof(TCHAR)*len))
			{
				listText[i+1]->ReplaceOriginalText(pData->pages[i].page_title); 
				listText[i+1]->SetStatus(CTextBase::status_new);
			}
			else
			{
			   CTextBase *pText = listText[i+1];
			   TCHAR* pStr = pText->GetTranslationText();
			   int nTranslationTextLen = pStr?_tcslen(pStr):0;
				if (nTranslationTextLen > 0)
				{
					pText->SetStatus(CTextBase::status_normal);
				}
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		for( int i = 0; i < 8; ++i)
		{
			//page[8]
			pText = new CText_Normal();
			pText->Init(8,false,pData->pages[i].page_title, NULL, _T("Page_name"));
			AddText(pText);
		}
	
	}
	return true;
}

bool CTemplate_ElementNpcSell::Export(void *pOrgData)
{
	NPC_SELL_SERVICE * pData = (NPC_SELL_SERVICE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//page[8]
	for( int i = 0; i < 8; ++i)
	{
		if( listText[i+1]->GetStatus() == CTextBase::status_normal && listText[i+1]->GetTranslationText()!=NULL)
		lstrcpy(pData->pages[i].page_title, listText[i+1]->GetTranslationText());
	}
	
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementNpc
//////////////////////////////////////////////////////////////////////

CTemplate_ElementNpc::~CTemplate_ElementNpc()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementNpc::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=2 && size != 3) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,false,NULL,NULL,_T("Desc"));
	if (size >= 3)
	{
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
	}
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(256,true,NULL,NULL,_T("Hello_msg"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l1.Suc();
	return true;
}

bool CTemplate_ElementNpc::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementNpc::Import(void *pOrgData)
{
	NPC_ESSENCE * pData = (NPC_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //desc[16];
	   pOrg = (char*)listText[1]->GetOriginalText();
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->desc); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //hello_msg[256]
	   pOrg = (char*)listText[2]->GetOriginalText();	
	   len = listText[2]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->hello_msg,sizeof(TCHAR)*len))
	   {
		  listText[2]->ReplaceOriginalText(pData->hello_msg); 
		  listText[2]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[2];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//desc[16];
		pText = new CText_Normal();
		pText->Init(16,false,pData->desc, NULL, _T("Desc"));
		AddText(pText);
		//hello_msg[256]
		pText = new CText_Normal();
		pText->Init(256,true, pData->hello_msg, NULL, _T("Hello_msg"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementNpc::Export(void *pOrgData)
{
	NPC_ESSENCE * pData = (NPC_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//desc[16];
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[1]->GetTranslationText());
	//hello_msg[256]
	if( listText[2]->GetStatus() == CTextBase::status_normal && listText[2]->GetTranslationText()!=NULL)
		lstrcpy(pData->hello_msg, listText[2]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementPetArmor
//////////////////////////////////////////////////////////////////////

CTemplate_ElementPetArmor::~CTemplate_ElementPetArmor()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementPetArmor::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=2) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);
	
	pNewBase = new CText_Normal;
	pNewBase->Init(16,true,NULL,NULL,_T("Desc"));
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	l1.Suc();
	return true;
}

bool CTemplate_ElementPetArmor::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementPetArmor::Import(void *pOrgData)
{
	PET_ARMOR_ESSENCE * pData = (PET_ARMOR_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //desc[16]
	   pOrg = (char*)listText[1]->GetOriginalText();	
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->desc); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//desc[16]
		pText = new CText_Normal();
		pText->Init(16,true, pData->desc, NULL, _T("Desc"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementPetArmor::Export(void *pOrgData)
{
	PET_ARMOR_ESSENCE * pData = (PET_ARMOR_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//desc[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[1]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementPetBedge
//////////////////////////////////////////////////////////////////////

CTemplate_ElementPetBedge::~CTemplate_ElementPetBedge()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementPetBedge::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size!=1 && size!=2) return false;

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(16,true,NULL,NULL,_T("Desc"));	
	if(size == 2)
	{
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
	}
	AddText(pNewBase);
	l1.Suc();
	return true;
}

bool CTemplate_ElementPetBedge::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementPetBedge::Import(void *pOrgData)
{
	PET_BEDGE_ESSENCE * pData = (PET_BEDGE_ESSENCE *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	   //name[32];
	   char * pOrg = (char*)listText[0]->GetOriginalText();
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->name,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->name); 
		  listText[0]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[0];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}

	   //desc[16]
	   pOrg = (char*)listText[1]->GetOriginalText();	
	   len = listText[1]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->desc,sizeof(TCHAR)*len))
	   {
		  listText[1]->ReplaceOriginalText(pData->desc); 
		  listText[1]->SetStatus(CTextBase::status_new);
	   }
	   else
		{
		   CTextBase *pText = listText[1];
		   TCHAR* pStr = pText->GetTranslationText();
		   int nTranslationTextLen = pStr?_tcslen(pStr):0;
			if (nTranslationTextLen > 0)
			{
				pText->SetStatus(CTextBase::status_normal);
			}
		}
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		//desc[16]
		pText = new CText_Normal();
		pText->Init(16,true, pData->desc, NULL, _T("Desc"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementPetBedge::Export(void *pOrgData)
{
	PET_BEDGE_ESSENCE * pData = (PET_BEDGE_ESSENCE *) pOrgData;
	//name[32];
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	//desc[16]
	if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		lstrcpy(pData->desc, listText[1]->GetTranslationText());
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ScrollRegionConfig
//////////////////////////////////////////////////////////////////////

CTemplate_ElementScrollRegionConfig::~CTemplate_ElementScrollRegionConfig()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementScrollRegionConfig::Load(FILE *pFile, DWORD dwVersion)
{	
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);

	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if(size != 13)
		return false;	

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(32,false,NULL,NULL,_T("Name"));
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(200,true,NULL,NULL,_T("Region_Desc"));	
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;	
	AddText(pNewBase);

	pNewBase = new CText_Normal;
	pNewBase->Init(100,true,NULL,NULL,_T("Hide_Region_Desc"));	
	r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;	
	AddText(pNewBase);

	for (int i=0; i<10; i++)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(100,true,NULL,NULL,_T("Event_Desc"));	
		r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;	
		AddText(pNewBase);
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementScrollRegionConfig::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementScrollRegionConfig::Import(void *pOrgData)
{
	SCROLL_REGION_CONFIG * pData = (SCROLL_REGION_CONFIG *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	  //name[32];
	  listText[0]->UpdateFromData(pData->name);

	  //region_desc[200]
	  if (listText.size() > 1)
		  listText[1]->UpdateFromData(pData->region_desc);

	  // hide_region_active_desc
	  if (listText.size() > 2)
		  listText[2]->UpdateFromData(pData->hide_region_active_desc);

	  //event_desc
	  for (int i = 3; i < listText.size(); i++)
		  listText[i]->UpdateFromData(pData->event_list[i-3].event_desc);
	}else
	{
		id = pData->id;
		//name[32];
		CTextBase * pText = new CText_Normal();
		pText->Init(32,false,pData->name, NULL, _T("Name"));
		AddText(pText);
		
		//desc[200]
		pText = new CText_Normal();
		pText->Init(200,true, pData->region_desc, NULL, _T("Region_Desc"));
		AddText(pText);

		//hide_region_active_desc[100]
		pText = new CText_Normal();
		pText->Init(100,true, pData->region_desc, NULL, _T("Hide_Region_Desc"));
		AddText(pText);

		//event_desc
		for (int i=0; i<10; i++)
		{
			pText = new CText_Normal();
			pText->Init(100, true, pData->event_list[i].event_desc, NULL, _T("Event_Desc"));
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementScrollRegionConfig::Export(void *pOrgData)
{
	SCROLL_REGION_CONFIG * pData = (SCROLL_REGION_CONFIG *) pOrgData;
	//name[32];
	if (listText.size() > 0)
		listText[0]->ExportToData(pData->name);

	//region_desc[200]
	if (listText.size() > 1)
		listText[1]->ExportToData(pData->region_desc);

	//hide_region_active_desc[100]
	if (listText.size() > 2)
		listText[2]->ExportToData(pData->hide_region_active_desc);

	//event_desc[10][100]
	for (int i=3; i<listText.size(); i++)
		listText[i]->ExportToData(pData->event_list[i-3].event_desc);
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementTalk
//////////////////////////////////////////////////////////////////////

CTemplate_ElementTalk::~CTemplate_ElementTalk()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	for( i = 0; i < listChild.size(); ++i)
		if(listChild[i]) delete listChild[i];
	listChild.clear();
}

bool CTemplate_ElementTalk::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	
	for( int i = 0; i < size; ++i)
	{
		CText_TalkWindow *pNewBase = new CText_TalkWindow;
		pNewBase->Init(true,NULL,NULL,_T("talk_text"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}

	fread(&size,sizeof(DWORD),1,pFile);
	for( i = 0; i < size; ++i)
	{
		CTemplateBase *pTemp = new CTemplate_ElementTalkOptions();
		if(!pTemp->Load(pFile,dwVersion)) return false;
		AddChild(pTemp);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementTalk::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	//write children
	size = listChild.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for(i = 0; i < size; ++i)
	{
		bool r = listChild[i]->Save(pFile);
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

talk_proc::window *FindWnd(unsigned int uid, void *pOrgData)
{
	talk_proc * pData = (talk_proc *) pOrgData;	
	for( int i = 0; i < pData->num_window; ++i)
	{
		if(pData->windows[i].id == uid)
			return &pData->windows[i];
	}
	return NULL;
}

CTemplateBase *CTemplate_ElementTalk::FindTalkOption(unsigned int wndid)
{
	for( int i = 0; i < listChild.size(); ++i)
	{
		if(listChild[i]->GetID()==wndid) return listChild[i];
	}
	return	NULL;
}

bool CTemplate_ElementTalk::Import(void *pOrgData)
{
	talk_proc * pData = (talk_proc *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		int len = 0;
		abase::vector<CTextBase*> delList;
		abase::vector<CTextBase*> newList;
		//首先更新旧的窗口
		for( int i = 0; i < listText.size(); ++i)
		{
			CText_TalkWindow *pWnd = (CText_TalkWindow*)listText[i];
			talk_proc::window *pNewWnd = FindWnd(pWnd->GetID(),pOrgData);
			if(pNewWnd==NULL)
			{//这是个现在没有的窗口,丢弃
				pWnd->SetStatus(CTextBase::status_disuse);
				CTemplate_ElementTalkOptions* pOption = (CTemplate_ElementTalkOptions*)FindTalkOption(pWnd->GetID());
				if(pOption) pOption->SetStatus(CTextBase::status_disuse);
			}else
			{
				int len1 = 0;
				int len2 = 0;
				if(  pWnd->GetOriginalText() ) len1 =  _tcslen(pWnd->GetOriginalText());
				if(  pNewWnd->talk_text ) len2 = _tcslen(pNewWnd->talk_text);
				if( len2 == len1)
				{//长度相等，做MD5比较
					if(len1!=0)
					{
						if(!CTextBase::DataIsEqual((char *)pWnd->GetOriginalText(),len1*sizeof(TCHAR),(char*)pNewWnd->talk_text,sizeof(TCHAR)*len1))
						{
							pWnd->ReplaceOriginalText(pNewWnd->talk_text); 
							pWnd->SetStatus(CTextBase::status_new);
						}
						else
						{
						   CTextBase *pText = pWnd;
						   TCHAR* pStr = pText->GetTranslationText();
						   int nTranslationTextLen = pStr?_tcslen(pStr):0;
							if (nTranslationTextLen > 0)
							{
								pText->SetStatus(CTextBase::status_normal);
							}
						}
					}
				}else
				{//长度不等的情况下，删除掉原窗口，加入新窗口
					delList.push_back(pWnd);
					CText_TalkWindow* pWndText = new CText_TalkWindow();
					pWndText->Init(true,pNewWnd->talk_text,NULL,_T("talk_text"));
					pWndText->SetID(pNewWnd->id);
					newList.push_back(pWndText);
				}
				//选项的更新
				CTemplate_ElementTalkOptions* pOption = (CTemplate_ElementTalkOptions*)FindTalkOption(pWnd->GetID());
				if(pOption) pOption->Import(pNewWnd);
				else if(pNewWnd->num_option > 0)
				{
					pOption = new CTemplate_ElementTalkOptions();
					pOption->Import(pNewWnd);
					AddChild(pOption);	
				}
			}
		}
		//其次看是否有新的窗口出现，如果有的话加入到表中
		for( i = 0; i < pData->num_window; ++i)
		{
			bool bFound = false;	
			for( int j = 0; j < listText.size(); ++j)
			{
				if(((CText_TalkWindow*)listText[j])->GetID()==pData->windows[i].id)
				{
					bFound = true;
					break;
				}
			}
			if(bFound) continue;
			
			for( j = 0; j <newList.size(); ++j )
			{
				if(((CText_TalkWindow*)newList[j])->GetID()==pData->windows[i].id)
				{
					bFound = true;
					break;
				}
			}
			if(!bFound)
			{
				CText_TalkWindow* pWndText = new CText_TalkWindow();
				pWndText->Init(true,pData->windows[i].talk_text,NULL,_T("talk_text"));
				pWndText->SetID(pData->windows[i].id);
				newList.push_back(pWndText);
			}
		}
		//现在真的更新数据
		for( i = 0; i < delList.size(); ++i)
		{
			for( int j = 0; j < listText.size(); ++j)
			{
				if(((CText_TalkWindow*)listText[j])->GetID() == ((CText_TalkWindow*)delList[i])->GetID())
				{
					listText.erase(&listText[j]);
					delete delList[i];
					break;
				}
			}
		}
		for( i = 0; i < newList.size(); ++i)
			AddText(newList[i]);
	}else
	{
		id = pData->id_talk;
		//window[]
		for( int i = 0; i < pData->num_window; ++i)
		{
			CText_TalkWindow* pWndText = new CText_TalkWindow();
			pWndText->Init(true,pData->windows[i].talk_text,NULL,_T("talk_text"));
			pWndText->SetID(pData->windows[i].id);
			AddText(pWndText);
			
			CTemplateBase *pTemp = new CTemplate_ElementTalkOptions();
			pTemp->Import((void*)&pData->windows[i]);
			AddChild(pTemp);
		}
	}
	return true;
}

bool CTemplate_ElementTalk::Export(void *pOrgData)
{
	talk_proc * pData = (talk_proc *) pOrgData;
	LogErr l1("export", iChunkType, template_type, id);
	
	for( int i = 0; i < listText.size(); ++i)
	{
		if( listText[i]->GetStatus() == CTextBase::status_normal)
		{
			for( int j = 0; j < pData->num_window; ++j)
			{
				if(pData->windows[j].id == ((CText_TalkWindow*)listText[i])->GetID())
				{
					int len = 0;
					if( listText[i]->GetTranslationText()!=NULL ) len = _tcslen(listText[i]->GetTranslationText());
					if(len>0)
					{
						if(pData->windows[j].talk_text) delete pData->windows[j].talk_text;
						pData->windows[j].talk_text = new TCHAR[len + 1];
						pData->windows[j].talk_text_len = len + 1;
						lstrcpy(pData->windows[j].talk_text, listText[i]->GetTranslationText());
					}
				}
			}
		}
	}

	for( int j = 0; j < pData->num_window; ++j)
	{
		for( int k = 0; k < listChild.size(); ++k)
		{
			CTemplate_ElementTalkOptions *pOption = (CTemplate_ElementTalkOptions*)listChild[k];
			if(pOption->GetID()==pData->windows[j].id)
			{
				if(!pOption->Export(&pData->windows[j])) return false;
				break;
			}
		}
	}

	l1.Suc();
	return true;
}

void CTemplate_ElementTalk::SetStatus(int status)
{
	for( int i = 0; i < listText.size(); ++i)
		listText[i]->SetStatus(status);
	for( i = 0; i < listChild.size(); ++i)
	{
		int n = ((CTemplate_ElementTalkOptions*)listChild[i])->GetTextNum();
		for( int j = 0; j < n; ++j)
		{
			CTextBase *pText = ((CTemplate_ElementTalkOptions*)listChild[i])->GetTextByIdx(j);
			pText->SetStatus(status);
		}
	}
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementTalkOptions
//////////////////////////////////////////////////////////////////////

CTemplate_ElementTalkOptions::~CTemplate_ElementTalkOptions()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementTalkOptions::Load(FILE *pFile, DWORD dwVersion)
{
	LogErr l1("load", iChunkType, template_type, id);
	if(dwVersion <=5)
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_TalkOption;
			pNewBase->Init(64,false,NULL,NULL,_T("option_text"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			
			//查找有没有id,par,与原始文本都相同的Text,如果有保留一份
			bool bAdd = true;
			for( int j = 0; j < listText.size(); j++)
			{
				CText_TalkOption *pTemp = (CText_TalkOption*)listText[j];
				if(pTemp->GetID() == pNewBase->GetID() && pTemp->GetParam() == pNewBase->GetParam())
				{
					if(_tcscmp(pTemp->GetOriginalText(),pNewBase->GetOriginalText())==0)
					{
						if(pTemp->GetTranslationText()==NULL && pNewBase->GetTranslationText() != NULL) pTemp->ReplaceTransText(pNewBase->GetTranslationText());
						delete pNewBase;
						bAdd = false;
						break;
					}
				}
			}
			
			if(bAdd) AddText(pNewBase);
		}

		for( i = 0; i < listText.size(); ++i)
		{
			CText_TalkOption *pTemp = (CText_TalkOption*)listText[i];
			pTemp->SetParam( -1 );
		}
	}else
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_TalkOption;
			pNewBase->Init(64,false,NULL,NULL,_T("option_text"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}
	}
	
	l1.Suc();
	return true;
}

bool CTemplate_ElementTalkOptions::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementTalkOptions::Import(void *pOrgData)
{
	talk_proc::window * pData = (talk_proc::window *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
	  //text[64];
		for( int i = 0; i < listText.size(); i++)
		{
			char * pOrg = (char*)listText[i]->GetOriginalText();
			int len = listText[i]->GetCharUpperLimit();

			CText_TalkOption *pOption = (CText_TalkOption *)listText[i];
			if(pOption->GetID() != -1)
			{
				if(pOption->GetParam() == -1)
				{//老版本(6以前)，需要把参数元素加入
					for( int j = 0; j < pData->num_option; ++j)
					{
						if(pOption->GetID()==pData->options[j].id)
						{
							if(CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->options[j].text,sizeof(TCHAR)*len))
							{
								pOption->SetParam(j);
								break;
							}							
						}
					}
				}else
				{
					bool bFd = false;
					for( int j = 0; j < pData->num_option; ++j)
					{
						if(pOption->GetID()==pData->options[j].id && pOption->GetParam() == j)
						{
							if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->options[j].text,sizeof(TCHAR)*len))
							{
								pOption->ReplaceOriginalText(pData->options[j].text); 
								pOption->SetStatus(CTextBase::status_new);
							}
							else
							{
							   CTextBase *pText = pOption;
							   TCHAR* pStr = pText->GetTranslationText();
							   int nTranslationTextLen = pStr?_tcslen(pStr):0;
								if (nTranslationTextLen > 0)
								{
									pText->SetStatus(CTextBase::status_normal);
								}
							}
							if(pOption->GetStatus()==CTextBase::status_disuse) pOption->SetStatus(CTextBase::status_new);
							bFd = true;
							break;
						}
					}
					if(!bFd) pOption->SetStatus(CTextBase::status_disuse);
				}
				
			}else
			{//老版本(3以前)，需要转换
				bool bFound = false;
				for( int j = 0; j < pData->num_option; ++j)
				{
					if(CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->options[j].text,sizeof(TCHAR)*len))
					{
						pOption->SetID(pData->options[j].id);
						pOption->SetParam(j);
						bFound = true;
						break;
					}
				}
				if(!bFound) pOption->SetStatus(CTextBase::status_disuse);
			}
		}

		//下面找出新的Option数据
		for( i = 0; i < pData->num_option; ++i)
		{
			bool bFound = false;
			for( int j = 0; j < listText.size(); ++j)
			{
				CText_TalkOption *pOption = (CText_TalkOption *)listText[j];
				if(pData->options[i].id == pOption->GetID() && i == pOption->GetParam())
				{
					bFound = true;
					break;
				}
			}

			if(!bFound)
			{
				CText_TalkOption * pText = new CText_TalkOption();
				
				/*TEST
				TCHAR szTemp[128];
				wsprintf(szTemp,_T("\nNew option = %s"),pData->options[i].text);
				TRACE(szTemp);
				*/
				
				pText->Init(64,false,pData->options[i].text, NULL, _T("option_text"));
				pText->SetID(pData->options[i].id);
				pText->SetParam(i);
				AddText(pText);
			}
		}
		
	   
	}else
	{
		id = pData->id;
		//text[64];
		for( int i = 0 ; i < pData->num_option; ++i)
		{
			CText_TalkOption * pText = new CText_TalkOption();
			pText->Init(64,false,pData->options[i].text, NULL, _T("option_text"));
			pText->SetID(pData->options[i].id);
			pText->SetParam(i);
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementTalkOptions::Export(void *pOrgData)
{
	talk_proc::window * pData = (talk_proc::window *) pOrgData;
	
	if(pData==NULL) return true;
	//text[64];
	for( int i = 0; i < pData->num_option; ++i)
	{
		for( int j = 0; j < listText.size(); ++j)
		{
			CText_TalkOption *pOption = (CText_TalkOption *)listText[j];
			if(pOption->GetID()==pData->options[i].id && pOption->GetParam()==i)
			{
				if( pOption->GetStatus() == CTextBase::status_normal && pOption->GetTranslationText()!=NULL)
					lstrcpy(pData->options[i].text, pOption->GetTranslationText());
				break;
			}
		}
	}
	return true;
}

void CTemplate_ElementTalkOptions::SetStatus(int ns)
{
	for( int j = 0; j < listText.size(); ++j)
	{
		CText_TalkOption *pOption = (CText_TalkOption *)listText[j];
		pOption->SetStatus(ns);
	}
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementStringTab
//////////////////////////////////////////////////////////////////////

CTemplate_ElementStringTab::~CTemplate_ElementStringTab()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementStringTab::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		pNewBase->Init(true,NULL,NULL,_T("item_desc"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementStringTab::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementStringTab::Import(void *pOrgData)
{
	CECStringTab::TEXT * pData = (CECStringTab::TEXT *) pOrgData;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   if(id!=pData->id) return false;
	   char *pOrg = (char*)listText[0]->GetOriginalText();	
	   int len1 = 0;
	   int len2 = 0;
	   if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
	   if( pData->szText != NULL) len2 = _tcslen(pData->szText);
	   if(len1==len2)
	   {
		   if(len1!=0)
		   {
			    if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pData->szText,sizeof(TCHAR)*len1))
			   {
				   listText[0]->ReplaceOriginalText(pData->szText); 
				   listText[0]->SetStatus(CTextBase::status_new);
			   }
				else
				{
				   CTextBase *pText = listText[0];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
		   }
	   }else
	   {
		   listText[0]->ReplaceOriginalText(pData->szText); 
		   listText[0]->SetStatus(CTextBase::status_new);
	   }

	   //if(listText[0]->GetOriginalText() && _tcsicmp(listText[0]->GetOriginalText(),_T("%s达到%d阶"))==0)
		//	 TRACE(_T("TTTT"));
	   
	}else
	{
		id = pData->id;
		CText_Resize * pText = new CText_Resize();
		pText->Init(false,pData->szText, NULL, _T("item_desc"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementStringTab::Export(void *pOrgData)
{
	CECStringTab::TEXT * pData = (CECStringTab::TEXT *) pOrgData;
	LogErr l1("export", iChunkType, template_type, id);
	if( listText[0]->GetStatus() == CTextBase::status_normal)
	{
		if(pData->id == id)
		{
			int len = 0;
			if(listText[0]->GetTranslationText()) len = _tcslen(listText[0]->GetTranslationText()) + 1;
			if(len > 0)
			{
				if(pData->szText) delete []pData->szText;
				pData->szText = new TCHAR[len];
				if(pData->szText) memset(pData->szText,0,len*sizeof(TCHAR));
				CString temp(listText[0]->GetTranslationText());
				temp.Replace(_T("\""),_T("'"));
				/*
				CString txt(listText[0]->GetOriginalText());
				if(txt.Find(_T("描述天泪之城的神书")) != -1)
					AfxMessageBox(temp);
				*/
			
				
				lstrcpy(pData->szText, temp);
			}
		}else return false;
	}
	l1.Suc();
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementInstance
//////////////////////////////////////////////////////////////////////

CTemplate_ElementInstance::~CTemplate_ElementInstance()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementInstance::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	TCHAR szRegion[128] = {0};
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		if (i==0)
		{
			pNewBase->Init(true,NULL,NULL,_T("item_desc"));
		}
		else
		{
			_sntprintf(szRegion, 127, _T("region_text_%d"), i);
			pNewBase->Init(true,NULL,NULL,szRegion);
		}
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementInstance::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementInstance::Import(void *pOrgData)
{
	CECInstanceText::_INSTANCE_TEXT * pInstanceData = (CECInstanceText::_INSTANCE_TEXT *) pOrgData;
	CECStringTab::TEXT * pData = pInstanceData->pText;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   if(id!=pData->id) return false;
	   for (int i=0; i<listText.size() && (i-1)<pInstanceData->listRegion.size(); i++)
	   {
		   char *pOrg = (char*)listText[i]->GetOriginalText();	
		   TCHAR* pszImport = ((i==0)?(pData->szText) : (pInstanceData->listRegion[i-1].szText));
		   int len1 = 0;
		   int len2 = 0;
		   if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
		   if( pData->szText != NULL) 
			   len2 = _tcslen(pszImport);
		   if(len1==len2)
		   {
			   if(len1!=0)
			   {
				   if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pszImport,sizeof(TCHAR)*len1))
				   {
					   listText[i]->ReplaceOriginalText(pszImport); 
					   listText[i]->SetStatus(CTextBase::status_new);
				   }
				   else
				   {
					   CTextBase *pText = listText[i];
					   TCHAR* pStr = pText->GetTranslationText();
					   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					   if (nTranslationTextLen > 0)
					   {
						   pText->SetStatus(CTextBase::status_normal);
					   }
				   }
			   }
		   }else
		   {
			   listText[i]->ReplaceOriginalText(pszImport); 
			   listText[i]->SetStatus(CTextBase::status_new);
		   }
	   }

	   for (i=pInstanceData->listRegion.size()+1; i<listText.size(); i++)
	   {
		   listText[i]->SetStatus(CTextBase::status_disuse);
	   }

	   TCHAR szRegion[128] = {0};
	   for (i=listText.size(); i<pInstanceData->listRegion.size()+1; i++)
	   {
			CText_Resize * pText = new CText_Resize();
			_sntprintf(szRegion, 127, _T("region_text_%d"), i+1);
			pText->Init(false,NULL,NULL,szRegion);
			AddText(pText);
	   }

	   

	   //if(listText[0]->GetOriginalText() && _tcsicmp(listText[0]->GetOriginalText(),_T("%s达到%d阶"))==0)
		//	 TRACE(_T("TTTT"));
	   
	}else
	{
		id = pData->id;
		CText_Resize * pText = new CText_Resize();
		pText->Init(false,pData->szText, NULL, _T("item_desc"));
		AddText(pText);

		TCHAR szRegion[128] = {0};
		for (int i=0; i<pInstanceData->listRegion.size(); i++)
		{
			CText_Resize * pText = new CText_Resize();
			_sntprintf(szRegion, 127, _T("region_text_%d"), i+1);
			pText->Init(false,pInstanceData->listRegion[i].szText, NULL, szRegion);
			AddText(pText);
		}
	}
	return true;
}

bool CTemplate_ElementInstance::Export(void *pOrgData)
{
	CECInstanceText::_INSTANCE_TEXT * pInstanceTxt = (CECInstanceText::_INSTANCE_TEXT*) pOrgData;
	CECStringTab::TEXT * pData = pInstanceTxt->pText;
	
	LogErr l1("export", iChunkType, template_type, id);
	if( listText[0]->GetStatus() == CTextBase::status_normal)
	{
		if(pData->id == id)
		{
			int len = 0;
			if(listText[0]->GetTranslationText()) len = _tcslen(listText[0]->GetTranslationText()) + 1;
			if(len > 0)
			{
				if(pData->szText) delete []pData->szText;
				pData->szText = new TCHAR[len];
				if(pData->szText) memset(pData->szText,0,len*sizeof(TCHAR));
				CString temp(listText[0]->GetTranslationText());
				temp.Replace(_T("\""),_T("'"));
				/*
				CString txt(listText[0]->GetOriginalText());
				if(txt.Find(_T("描述天泪之城的神书")) != -1)
					AfxMessageBox(temp);
				*/
			
				
				lstrcpy(pData->szText, temp);
			}
		}else return false;
	}

	// Region Text
	for (int i=1; i<listText.size(); i++)
	{
		if( listText[i]->GetStatus() != CTextBase::status_normal)
			continue;
		int len = 0;
		CECInstanceText::_REGION_TEXT& RegionData = pInstanceTxt->listRegion[i-1];
		if(listText[i]->GetTranslationText()) 
			len = _tcslen(listText[i]->GetTranslationText()) + 1;
		if(len > 0)
		{
			if(RegionData.szText) 
				delete []RegionData.szText;
			RegionData.szText = new TCHAR[len];
			if(pData->szText) memset(RegionData.szText,0,len*sizeof(TCHAR));
			CString temp(listText[i]->GetTranslationText());
			temp.Replace(_T("\""),_T("'"));
			lstrcpy(RegionData.szText, temp);
		}
	}
	l1.Suc();
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTemplate_ElementMessage
//////////////////////////////////////////////////////////////////////

CTemplate_ElementMessage::~CTemplate_ElementMessage()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTemplate_ElementMessage::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		pNewBase->Init(true,NULL,NULL,_T("message_text"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementMessage::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	LogErr l1("save", iChunkType, template_type, id);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementMessage::Import(void *pOrgData)
{
	CECStringTab::TEXT * pData = (CECStringTab::TEXT *) pOrgData;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   if(id!=pData->id) return false;
	   char *pOrg = (char*)listText[0]->GetOriginalText();	
	   int len1 = 0;
	   int len2 = 0;
	   if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
	   if( pData->szText != NULL) len2 = _tcslen(pData->szText);
	   if(len1==len2)
	   {
		   if(len1!=0)
		   {
			   if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pData->szText,sizeof(TCHAR)*len1))
			   {
				   listText[0]->ReplaceOriginalText(pData->szText); 
				   listText[0]->SetStatus(CTextBase::status_new);
			   }
			   else
				{
				   CTextBase *pText = listText[0];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
		   }
	   }else
	   {
		   listText[0]->ReplaceOriginalText(pData->szText); 
		   listText[0]->SetStatus(CTextBase::status_new);
	   }
	   
	}else
	{
		id = pData->id;
		CText_Resize * pText = new CText_Resize();
		pText->Init(false,pData->szText, NULL, _T("message_text"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementMessage::Export(void *pOrgData)
{
	CECStringTab::TEXT * pData = (CECStringTab::TEXT *) pOrgData;
	
	LogErr l1("export", iChunkType, template_type, id);
	if( listText[0]->GetStatus() == CTextBase::status_normal)
	{
		if(pData->id == id)
		{
			int len = 0;
			if(listText[0]->GetTranslationText()) len = _tcslen(listText[0]->GetTranslationText()) + 1;
			if(len > 0)
			{
				if(pData->szText) delete []pData->szText;
				pData->szText = new TCHAR[len];
				CString temp = listText[0]->GetTranslationText();
				temp.Replace(_T("\""),_T("'"));
				lstrcpy(pData->szText, temp);
			}
		}else return false;
	}
	l1.Suc();
	return true;
}



//////////////////////////////////////////////////////////////////////
// CTemplate_ElementInterface
//////////////////////////////////////////////////////////////////////

CTemplate_ElementInterfaceHint::~CTemplate_ElementInterfaceHint()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	if(m_pIndex) delete[] m_pIndex;
}

bool CTemplate_ElementInterfaceHint::Load(FILE *pFile, DWORD dwVersion)
{
	
	fread(&m_nIndexLen,sizeof(int),1,pFile);
	m_pIndex = new TCHAR[m_nIndexLen];
	fread(m_pIndex,m_nIndexLen*sizeof(TCHAR),1,pFile);
	LogErr l1("load", iChunkType, template_type, id);
	l1.File(m_pIndex);
	
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		pNewBase->Init(true,NULL,NULL,_T("interface_text"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	
	return true;
}

bool CTemplate_ElementInterfaceHint::Save(FILE *pFile)
{	
	LogErr l1("save", iChunkType, template_type, id);
	l1.File(m_pIndex);

	fwrite(&m_nIndexLen,sizeof(int),1,pFile);
	fwrite(m_pIndex,sizeof(TCHAR)*m_nIndexLen,1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	l1.Suc();
	
	return true;
}

bool CTemplate_ElementInterfaceHint::Import(void *pOrgData)
{
	INTER_HINT * pData = (INTER_HINT *) pOrgData;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
		if(_tcsicmp(pData->szIndex,m_pIndex)!=0) return false;
		
		int len1 = 0;
		int len2 = 0;
		len1 = _tcslen(pData->szText);
		if(listText[0]->GetOriginalText()) len2 = _tcslen(listText[0]->GetOriginalText());
		if(len1 == len2)
		{
			if(len1!=0)
			{
				if(!CTextBase::DataIsEqual((char*)pData->szText,len1*sizeof(TCHAR),(char*)listText[0]->GetOriginalText(),sizeof(TCHAR)*len1))
			   {
				   listText[0]->ReplaceOriginalText(pData->szText); 
				   listText[0]->SetStatus(CTextBase::status_new);
			   }
				else
				{
				   CTextBase *pText = listText[0];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = 0;
				   if (pStr)
				   {
					   nTranslationTextLen = _tcslen(pStr);
				   }				    
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
			}
		}else
		{
			listText[0]->ReplaceOriginalText(pData->szText); 
			listText[0]->SetStatus(CTextBase::status_new);
		}
	}else
	{
		m_nIndexLen = _tcslen(pData->szIndex) + 1;
		m_pIndex = new TCHAR[m_nIndexLen];
		lstrcpy(m_pIndex,pData->szIndex);

		CText_Resize * pText = new CText_Resize();
		pText->Init(false,pData->szText, NULL, _T("interface_text"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementInterfaceHint::Export(void *pOrgData)
{	
	LogErr l1("export", iChunkType, template_type, id);
	l1.File(m_pIndex);

	INTER_HINT * pData = (INTER_HINT *) pOrgData;
	if(listText.size()!=1) return false;
	if(_tcsicmp(pData->szIndex,m_pIndex)!=0) return false;

	if(listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		CString temp(listText[0]->GetTranslationText());
		temp.Replace(_T("\""),_T("'"));
		temp.Replace(_T("\r\n"),_T("\\r"));
		temp.Replace(_T("\n"),_T("\\r"));
		temp.Replace(_T("\r"),_T("\\r"));
		lstrcpy(pData->szText,temp);
	}

	l1.Suc();
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTemplate_ElementInterfaceString
//////////////////////////////////////////////////////////////////////

CTemplate_ElementInterfaceString::~CTemplate_ElementInterfaceString()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();

	if(m_pIndex) delete[] m_pIndex;
}

bool CTemplate_ElementInterfaceString::Load(FILE *pFile, DWORD dwVersion)
{
	
	fread(&m_nIndexLen,sizeof(int),1,pFile);
	m_pIndex = new TCHAR[m_nIndexLen];
	fread(m_pIndex,m_nIndexLen*sizeof(TCHAR),1,pFile);
	
	LogErr l1("load", iChunkType, template_type, id);
	l1.File(m_pIndex);

	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CText_Resize *pNewBase = new CText_Resize;
		pNewBase->Init(true,NULL,NULL,_T("interface_text"));
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
		AddText(pNewBase);
	}
	l1.Suc();
	return true;
}

bool CTemplate_ElementInterfaceString::Save(FILE *pFile)
{	
	LogErr l1("save", iChunkType, template_type, id);
	l1.File(m_pIndex);

	fwrite(&m_nIndexLen,sizeof(int),1,pFile);
	fwrite(m_pIndex,sizeof(TCHAR)*m_nIndexLen,1,pFile);
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	l1.Suc();
	return true;
}

bool CTemplate_ElementInterfaceString::Import(void *pOrgData)
{
	INTER_STRING * pData = (INTER_STRING *) pOrgData;
	
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   if(id!=pData->id) return false;
	   char *pOrg = (char*)listText[0]->GetOriginalText();	
	   int len1 = 0;
	   int len2 = 0;
	   if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
	   if( pData->szText != NULL) len2 = _tcslen(pData->szText);
	   if(len1==len2)
	   {
		   if(len1!=0)
		   {
			   if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pData->szText,sizeof(TCHAR)*len1))
			   {
				   listText[0]->ReplaceOriginalText(pData->szText); 
				   listText[0]->SetStatus(CTextBase::status_new);
			   }
			   else
				{
				   CTextBase *pText = listText[0];
				   TCHAR* pStr = pText->GetTranslationText();
				   int nTranslationTextLen = pStr?_tcslen(pStr):0;
					if (nTranslationTextLen > 0)
					{
						pText->SetStatus(CTextBase::status_normal);
					}
				}
		   }
	   }else
	   {
		   listText[0]->ReplaceOriginalText(pData->szText); 
		   listText[0]->SetStatus(CTextBase::status_new);
	   }
	   
	}else
	{
		id = pData->id;
		m_nIndexLen = _tcslen(pData->szIndex) + 1;
		m_pIndex = new TCHAR[m_nIndexLen];

		CText_Resize * pText = new CText_Resize();
		lstrcpy(m_pIndex,pData->szIndex);
		pText->Init(false,pData->szText, NULL, _T("interface_text"));
		AddText(pText);
	}
	return true;
}

bool CTemplate_ElementInterfaceString::Export(void *pOrgData)
{
	INTER_STRING * pData = (INTER_STRING *) pOrgData;
	
	LogErr l1("export", iChunkType, template_type, id);
	l1.File(m_pIndex);
	if( listText[0]->GetStatus() == CTextBase::status_normal)
	{
		if(pData->id == id)
		{
			int len = 0;
			if(listText[0]->GetOriginalText()) len = _tcslen(listText[0]->GetOriginalText());
			if(len == 0)
				return true;
			len = 0;
			if(listText[0]->GetTranslationText()) len = _tcslen(listText[0]->GetTranslationText()) + 1;
			if(len > 0)
			{
				//这儿不能做删除操作，因为INTER_STRING是固定的最大是10000
				//if(pData->szText) delete []pData->szText;
				if(len > 10000) ASSERT(FALSE);
				CString temp(listText[0]->GetTranslationText());
				temp.Replace(_T("\""),_T("'"));
				temp.Replace(_T("\r\n"),_T("\\r"));
				temp.Replace(_T("\n"),_T("\\r"));
				temp.Replace(_T("\r"),_T("\\r"));
				lstrcpy(pData->szText, temp);
			}
		}else return false;
	}
	l1.Suc();
	return true;
}

