// GlobalDataTemplate.cpp: implementation of the GlobalDataTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementlocalize.h"
#include "GlobalDataTemplate.h"
#include "GlobalDataMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

/*
//////////////////////////////////////////////////////////////////////
// CTransTargetTemplate
//////////////////////////////////////////////////////////////////////

CTransTargetTemplate::~CTransTargetTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTransTargetTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	if(dwVersion < 9)
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_Normal;
			pNewBase->Init(32,false,NULL,NULL,_T("Name"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			
			CTextBase *pNewBase2 = new CText_Normal;
			pNewBase2->Init(16,false,NULL,NULL,_T("Name"));
			
			pNewBase2->ReplaceOriginalText(pNewBase->GetOriginalText());
			
			TCHAR szTemp[16];
			memset(szTemp,0,16*sizeof(TCHAR));
			memcpy(szTemp,pNewBase->GetTranslationText(),15*sizeof(TCHAR));
			pNewBase2->ReplaceTransText(szTemp);


			int len = _tcslen(pNewBase->GetTranslationText());
			if( len > 16) pNewBase2->SetStatus(CTextBase::status_new);
			
			delete pNewBase;
			AddText(pNewBase2);
		}
	}else
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Name"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}
	}
	return true;	
}

bool CTransTargetTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

bool CTransTargetTemplate::Import(void *pOrgData)
{
	_TRANS_TARGET * pData = (_TRANS_TARGET *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
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
	}else
	{
		id = pData->id;
		CTextBase * pText = new CText_Normal();
		pText->Init(16,false,pData->name, NULL,_T("Name"));
		AddText(pText);
	}
	return true;
}

bool CTransTargetTemplate::Export(void *pOrgData)
{
	_TRANS_TARGET * pData = (_TRANS_TARGET *) pOrgData;
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	}
	return true;
}
*/


//////////////////////////////////////////////////////////////////////
// CDomainInforTemplate
//////////////////////////////////////////////////////////////////////

CDomainInforTemplate::~CDomainInforTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CDomainInforTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	if(dwVersion < 9)
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_Normal;
			pNewBase->Init(32,false,NULL,NULL,_T("Name"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			
			CTextBase *pNewBase2 = new CText_Normal;
			pNewBase2->Init(16,false,NULL,NULL,_T("Name"));
			
			pNewBase2->ReplaceOriginalText(pNewBase->GetOriginalText());
			
			TCHAR szTemp[16];
			memset(szTemp,0,16*sizeof(TCHAR));
			memcpy(szTemp,pNewBase->GetTranslationText(),15*sizeof(TCHAR));
			pNewBase2->ReplaceTransText(szTemp);
			
			int len = _tcslen(pNewBase->GetTranslationText());
			if( len > 16) pNewBase2->SetStatus(CTextBase::status_new);
			
			delete pNewBase;
			AddText(pNewBase2);
		}
	}else
	{
		fread(&id,sizeof(unsigned int),1,pFile);
		DWORD size = 0;
		fread(&size,sizeof(DWORD),1,pFile);
		for( int i = 0; i < size; ++i)
		{
			CTextBase *pNewBase = new CText_Normal;
			pNewBase->Init(16,false,NULL,NULL,_T("Name"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}
	}
	return true;	
}

bool CDomainInforTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

bool CDomainInforTemplate::Import(void *pOrgData)
{
	_DOMAIN_INFO * pData = (_DOMAIN_INFO *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
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
	}else
	{
		id = pData->id;
		CTextBase * pText = new CText_Normal();
		pText->Init(16,false,pData->name, NULL,_T("Name"));
		AddText(pText);
	}
	return true;
}

bool CDomainInforTemplate::Export(void *pOrgData)
{
	_DOMAIN_INFO * pData = (_DOMAIN_INFO *) pOrgData;
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		lstrcpy(pData->name, listText[0]->GetTranslationText());
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CGShopTemplate
//////////////////////////////////////////////////////////////////////

CGShopTemplate::~CGShopTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CGShopTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);

	//0
	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(512,false,NULL,NULL,_T("treasure_desc"));
	if(dwVersion==1)
	{
		CText_Normal temp;
		temp.Init(64,false,NULL,NULL,_T("treasure_desc"));
		bool r = temp.Load(pFile,dwVersion);
		if(!r) return false;
		pNewBase->ReplaceOriginalText(temp.GetOriginalText());
		pNewBase->ReplaceTransText(temp.GetTranslationText());
		pNewBase->SetStatus(temp.GetStatus());
	}else 
	{
		
		bool r = pNewBase->Load(pFile,dwVersion);
		if(!r) return false;
	}
	
	AddText(pNewBase);

	//1
	if(size > 1)
	{
		pNewBase = new CText_Normal;
		pNewBase->Init(32,false,NULL,NULL,_T("treasure_name"));
		if(!pNewBase->Load(pFile,dwVersion)) return false;
		AddText(pNewBase);

		if (dwVersion >= 0x10000009 && size == 4)
		{
			pNewBase = new CText_Normal;
			pNewBase->Init(32, false, NULL, NULL, _T("present_name"));
			if(!pNewBase->Load(pFile,dwVersion)) return false;
			AddText(pNewBase);
			
			pNewBase = new CText_Normal;
			pNewBase->Init(512, false, NULL, NULL, _T("present_desc"));
			if(!pNewBase->Load(pFile,dwVersion)) return false;
			AddText(pNewBase);
		}
	}	
	
	return true;
}

bool CGShopTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

bool CGShopTemplate::Import(void *pOrgData)
{
	_GSHOP_ITEM * pData = (_GSHOP_ITEM *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
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

	   if(listText.size() == 2)//这儿因为原来没有这个数据项，现在有名字这个数据项了。
	   {
		   pOrg = (char*)listText[1]->GetOriginalText();	
		   len = listText[1]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->szName,sizeof(TCHAR)*len))
		   {
			   listText[1]->ReplaceOriginalText(pData->szName); 
			   listText[1]->SetStatus(CTextBase::status_new);
		   }
		  else
			{
			   CTextBase *pText = listText[1];
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

			CTextBase *pText = new CText_Normal();
			pText->Init(32,false,pData->szPresentName, NULL,_T("present_name"));
			AddText(pText);

			pText = new CText_Normal();
			pText->Init(512,false,pData->Presentdesc, NULL,_T("present_desc"));
			AddText(pText);
	   }else
		if(listText.size() == 4)
		{
			pOrg = (char*)listText[1]->GetOriginalText();	
		   len = listText[1]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->szName,sizeof(TCHAR)*len))
		   {
			   listText[1]->ReplaceOriginalText(pData->szName); 
			   listText[1]->SetStatus(CTextBase::status_new);
		   }
		  else
			{
			   CTextBase *pText = listText[1];
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

		  pOrg = (char*)listText[2]->GetOriginalText();	
		   len = listText[2]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->szPresentName,sizeof(TCHAR)*len))
		   {
			   listText[2]->ReplaceOriginalText(pData->szPresentName); 
			   listText[2]->SetStatus(CTextBase::status_new);
		   }
		  else
			{
			   CTextBase *pText = listText[2];
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

		  pOrg = (char*)listText[3]->GetOriginalText();	
		   len = listText[3]->GetCharUpperLimit();
		   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->Presentdesc,sizeof(TCHAR)*len))
		   {
			   listText[3]->ReplaceOriginalText(pData->Presentdesc); 
			   listText[3]->SetStatus(CTextBase::status_new);
		   }
		  else
			{
			   CTextBase *pText = listText[3];
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
		else
		if(listText.size() == 1)
	   {
			CTextBase *pText = new CText_Normal();
			pText->Init(32,false,pData->szName, NULL,_T("treasure_name"));
			AddText(pText);

			pText = new CText_Normal();
			pText->Init(32,false,pData->szPresentName, NULL,_T("present_name"));
			AddText(pText);

			pText = new CText_Normal();
			pText->Init(512,false,pData->Presentdesc, NULL,_T("present_desc"));
			AddText(pText);
	   }

	   id = pData->local_id;//这儿是为了纠正以前ID传入的错误，原来是用pData->id;

	}else
	{
		id = pData->local_id;
		CTextBase * pText = new CText_Normal();
		pText->Init(512,false,pData->desc, NULL,_T("treasure_desc"));
		AddText(pText);
		
		pText = new CText_Normal();
		pText->Init(32,false,pData->szName, NULL,_T("treasure_name"));
		AddText(pText);

		pText = new CText_Normal();
		pText->Init(32,false,pData->szPresentName, NULL,_T("present_name"));
		AddText(pText);

		pText = new CText_Normal();
		pText->Init(512,false,pData->Presentdesc, NULL,_T("present_desc"));
		AddText(pText);
	}
	return true;
}

bool CGShopTemplate::Export(void *pOrgData)
{
	_GSHOP_ITEM * pData = (_GSHOP_ITEM *) pOrgData;
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		lstrcpy(pData->desc, listText[0]->GetTranslationText());
	}

	if(listText.size() > 1) 
	{
		if( listText[1]->GetStatus() == CTextBase::status_normal && listText[1]->GetTranslationText()!=NULL)
		{
			lstrcpy(pData->szName, listText[1]->GetTranslationText());
		}
	}

	if(listText.size() > 2)
	{
		if( listText[2]->GetStatus() == CTextBase::status_normal && listText[2]->GetTranslationText()!=NULL)
		{
			lstrcpy(pData->szPresentName, listText[2]->GetTranslationText());
		}
	}

	if(listText.size() > 3)
	{
		if( listText[3]->GetStatus() == CTextBase::status_normal && listText[3]->GetTranslationText()!=NULL)
		{
			lstrcpy(pData->Presentdesc, listText[3]->GetTranslationText());
		}
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CGShopTypesTemplate
//////////////////////////////////////////////////////////////////////

CGShopTypesTemplate::~CGShopTypesTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();

	for( i = 0; i < listChild.size(); ++i)
		delete listChild[i];
	listChild.clear();
}

bool CGShopTypesTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);

	CTextBase *pNewBase = new CText_Normal;
	pNewBase->Init(64,false,NULL,NULL,_T("main_type"));
	
	bool r = pNewBase->Load(pFile,dwVersion);
	if(!r) return false;
	
	AddText(pNewBase);

	fread(&size,sizeof(DWORD),1,pFile);

	//children
	if(size > 0)
	{
		CGShopSubTypesTemplate* pChild = new CGShopSubTypesTemplate;
		if(!pChild->Load(pFile,dwVersion)) return false;
		AddChild(pChild);
	}
	
	return true;	
}

bool CGShopTypesTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}

	size = listChild.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( i = 0; i < size; ++i)
	{
		bool r = listChild[i]->Save(pFile);	
		if(!r) return false;
	}

	return true;	
}

bool CGShopTypesTemplate::Import(void *pOrgData)
{
	GSHOP_MAIN_TYPE_PARAMS * pData = (GSHOP_MAIN_TYPE_PARAMS *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   char * pOrg = (char*)listText[0]->GetOriginalText();	
	   int len = listText[0]->GetCharUpperLimit();
	   if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)pData->pMainType->szName,sizeof(TCHAR)*len))
	   {
		  listText[0]->ReplaceOriginalText(pData->pMainType->szName); 
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

	   if(pData->pMainType->sub_types.size() > 0)
	   {
		   if(listChild.size() > 0)
		   {
			   listChild[0]->Import(&pData->pMainType->sub_types);
		   }else 
		   {
			   CGShopSubTypesTemplate *pChild = new CGShopSubTypesTemplate;
			   if(!pChild->Import(&pData->pMainType->sub_types)) return false;
			   AddChild(pChild);
		   }
	   }
	   
	}else
	{
		id = pData->id;
		CTextBase * pText = new CText_Normal();
		pText->Init(64,false,pData->pMainType->szName, NULL,_T("main_type"));
		AddText(pText);
		
		if(pData->pMainType->sub_types.size() > 0)
		{
			CGShopSubTypesTemplate *pChild = new CGShopSubTypesTemplate;
			if(!pChild->Import(&pData->pMainType->sub_types)) return false;
			AddChild(pChild);
		}
	}
	return true;
}

bool CGShopTypesTemplate::Export(void *pOrgData)
{
	GSHOP_MAIN_TYPE_PARAMS * pData = (GSHOP_MAIN_TYPE_PARAMS *) pOrgData;
	if( listText[0]->GetStatus() == CTextBase::status_normal && listText[0]->GetTranslationText()!=NULL)
	{
		lstrcpy(pData->pMainType->szName, listText[0]->GetTranslationText());
	}

	if(listChild.size() > 0) 
	{
		listChild[0]->Export(&pData->pMainType->sub_types);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CGShopSubTypesTemplate
//////////////////////////////////////////////////////////////////////

CGShopSubTypesTemplate::~CGShopSubTypesTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CGShopSubTypesTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	//1
	for(int i = 0; i < size; ++i)
	{
		CText_Normal *pNewBase = new CText_Normal;
		pNewBase->Init(64,false,NULL,NULL,_T("sub_type"));
		if(!pNewBase->Load(pFile,dwVersion)) return false;
		AddText(pNewBase);
	}
	return true;	
}

bool CGShopSubTypesTemplate::Save(FILE *pFile)
{
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

bool CGShopSubTypesTemplate::Import(void *pOrgData)
{
	TCHAR szBuffer[64];
	vector<ACString>* pData = (vector<ACString>*) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   
		for( int i = 0; i < pData->size(); ++i)
		{
			_tcscpy(szBuffer,(*pData)[i]);
			if( i < listText.size() )
			{
				char * pOrg = (char*)listText[i]->GetOriginalText();	
				int len = listText[i]->GetCharUpperLimit();
				TCHAR szBuffer[64];
				_tcscpy(szBuffer,(*pData)[i]);
				if(!CTextBase::DataIsEqual(pOrg,len*sizeof(TCHAR),(char*)szBuffer,sizeof(TCHAR)*len))
				{
					listText[i]->ReplaceOriginalText(szBuffer); 
					listText[i]->SetStatus(CTextBase::status_new);
				}
				else
				{
				   CTextBase *pText = listText[i];
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
			}else
			{
				CTextBase * pText = new CText_Normal();
				pText->Init(64,false,szBuffer, NULL,_T("sub_type"));
				AddText(pText);
			}
		}
		
		//设置废弃的
		for( i = pData->size(); i < listText.size(); ++i)
			listText[i]->SetStatus(CTextBase::status_disuse);

	}else
	{
		for( int i = 0; i < pData->size(); ++i)
		{
			CTextBase * pText = new CText_Normal();
			_tcscpy(szBuffer,(*pData)[i]);
			pText->Init(64,false,szBuffer, NULL,_T("sub_type"));
			AddText(pText);
		}
	}
	return true;
}

bool CGShopSubTypesTemplate::Export(void *pOrgData)
{
	vector<ACString>* pData = (vector<ACString>*) pOrgData;
	
	for( int i = 0; i < pData->size(); ++i)
	{
		if( i >= listText.size() ) break;
		if( listText[i]->GetStatus() == CTextBase::status_normal && listText[i]->GetTranslationText()!=NULL)
		{
			(*pData)[i] = listText[i]->GetTranslationText();
		}
	}

	return true;
}