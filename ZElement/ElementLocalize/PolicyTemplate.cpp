// PolicyTemplate.cpp: implementation of the CTriggerTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementlocalize.h"
#include "PolicyTemplate.h"
#include "../ZElementData/Policy.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTriggerTemplate::~CTriggerTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
}

bool CTriggerTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		if(dwVersion < 0x10000005)
		{
			CText_TalkWindow *pNewBase = new CText_TalkWindow;
			pNewBase->Init(true,NULL,NULL,_T("talk_text"));
			bool r = pNewBase->Load(pFile,dwVersion);
			if(!r) return false;
			AddText(pNewBase);
		}else
		{
			TCHAR szName[32];
			fread(szName,32*sizeof(TCHAR),1,pFile);
			if(_tcsicmp(szName,_T("talk_text"))==0)
			{
				CText_TalkWindow *pNewBase = new CText_TalkWindow;
				pNewBase->Init(true,NULL,NULL,_T("talk_text"));
				bool r = pNewBase->Load(pFile,dwVersion);
				if(!r) return false;
				AddText(pNewBase);
			}else
			{
				CText_TalkOption *pNewBase = new CText_TalkOption;
				pNewBase->Init(16,false,NULL, NULL, _T("name"));
				bool r = pNewBase->Load(pFile,dwVersion);
				if(!r) return false;
				AddText(pNewBase);	
			}
		}
	}
	return true;	
}

bool CTriggerTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
	DWORD size = listText.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		fwrite(listText[i]->GetTextName(),32*sizeof(TCHAR),1,pFile);
		bool r = listText[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;	
}

void CTriggerTemplate::SetTreeStatus(int status)
{
	for( int i = 0; i < listText.size(); ++i)
		listText[i]->SetStatus(status);
}

bool CTriggerTemplate::Import(void *pOrgData)
{
	CTriggerData * pData = (CTriggerData *) pOrgData;
	if(listText.size() > 0)
	{
		//找出废弃的文字,和更新改动过的文字
		for( int i = 0; i < listText.size(); ++i)
		{
			int num = pData->GetOperaionNum();
			CTextBase *pOldText = listText[i];
			if(_tcsicmp(pOldText->GetTextName(),_T("talk_text"))==0)
			{
				if(((CText_TalkWindow*)pOldText)->GetID() < num)
				{
					CTriggerData::_s_operation *po = pData->GetOperaion(((CText_TalkWindow*)pOldText)->GetID());
					if(po->iType == CTriggerData::o_talk)
					{//找到对应文字，看内容是否更新
						O_TALK_TEXT* pTalk = (O_TALK_TEXT*)po->pParam;
						char *pOrg = (char*)listText[i]->GetOriginalText();	
						int len1 = 0;
						int len2 = 0;
						if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
						if( po->pParam) len2 = _tcslen((TCHAR*)pTalk->szData);
						if(len1==len2)
						{
							if(len1!=0)
							{
								if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pTalk->szData,sizeof(TCHAR)*len1))
								{
									listText[i]->ReplaceOriginalText((TCHAR*)pTalk->szData); 
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
							listText[i]->ReplaceOriginalText((TCHAR*)pTalk->szData); 
							listText[i]->SetStatus(CTextBase::status_new);
						}
					}else pOldText->SetStatus(CTextBase::status_disuse);
				}else pOldText->SetStatus(CTextBase::status_disuse);
			}else
			{
				if(((CText_TalkOption*)pOldText)->GetID() < num)
				{
					CTriggerData::_s_operation *po = pData->GetOperaion(((CText_TalkOption*)pOldText)->GetID());
					if(po->iType == CTriggerData::o_summon)
					{//找到对应文字，看内容是否更新
						O_SUMMON* pTalk = (O_SUMMON*)po->pParam;
						char *pOrg = (char*)listText[i]->GetOriginalText();	
						int len1 = 0;
						int len2 = 0;
						if( pOrg!=NULL ) len1 =  _tcslen((TCHAR*)pOrg);
						if( po->pParam) len2 = _tcslen((TCHAR*)pTalk->szName);
						if(len1==len2)
						{
							if(len1!=0)
							{
								if(!CTextBase::DataIsEqual(pOrg,len1*sizeof(TCHAR),(char*)pTalk->szName,sizeof(TCHAR)*len1))
								{
									listText[i]->ReplaceOriginalText((TCHAR*)pTalk->szName); 
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
							listText[i]->ReplaceOriginalText((TCHAR*)pTalk->szName); 
							listText[i]->SetStatus(CTextBase::status_new);
						}
					}else pOldText->SetStatus(CTextBase::status_disuse);
				}else pOldText->SetStatus(CTextBase::status_disuse);
			}
		}

		//找出新加入的文字，把它加入到列表中
		int num = pData->GetOperaionNum();
		for( i = 0; i < num; ++i)
		{
			CTriggerData::_s_operation *po = pData->GetOperaion(i);
			if(po->iType == CTriggerData::o_talk)
			{
				bool bFound = false;
				for( int j = 0; j < listText.size(); ++j)
				{
					if(((CText_TalkWindow*)listText[j])->GetID()==i)
					{
						bFound = true;
						break;
					}
				}
				if(!bFound)
				{//新的文字需要加入
					CText_TalkWindow *pNewText = new CText_TalkWindow;
					O_TALK_TEXT* pTalk = (O_TALK_TEXT*)po->pParam;
					pNewText->Init(true,(TCHAR*)pTalk->szData,NULL,_T("talk_text"));
					pNewText->SetID(i);
					AddText(pNewText);
				}
			}else if(po->iType == CTriggerData::o_summon)
			{
				bool bFound = false;
				for( int j = 0; j < listText.size(); ++j)
				{
					if(((CText_TalkOption*)listText[j])->GetID()==i)
					{
						bFound = true;
						break;
					}
				}
				if(!bFound)
				{//新的文字需要加入
					CText_TalkOption *pNewText = new CText_TalkOption;
					O_SUMMON* pSummon = (O_SUMMON*)po->pParam;
					pNewText->Init(16,false,pSummon->szName, NULL, _T("name"));
					pNewText->SetID(i);
					AddText(pNewText);
				}
			}
		}
	}else
	{
		id = pData->GetID();
		int n = pData->GetOperaionNum();
		for(  int i = 0; i < n; ++i)
		{
			CTriggerData::_s_operation *po = pData->GetOperaion(i);
			if(po->iType==CTriggerData::o_talk)
			{
				//这儿借用CText_TalkWindow来存字符
				CText_TalkWindow *pNewText = new CText_TalkWindow;
				O_TALK_TEXT* pTalk = (O_TALK_TEXT*)po->pParam;
				pNewText->Init(true,(TCHAR*)pTalk->szData,NULL,_T("talk_text"));
				pNewText->SetID(i);//这儿把它的索引当做ID来保存
				AddText(pNewText);
			}else if(po->iType==CTriggerData::o_summon)
			{
				CText_TalkOption *pNewText = new CText_TalkOption;
				O_SUMMON* pSummon = (O_SUMMON*)po->pParam;
				pNewText->Init(16,false,pSummon->szName, NULL, _T("name"));
				pNewText->SetID(i);//这儿把它的索引当做ID来保存
				AddText(pNewText);
			}
		}
	}
	return true;
}

bool CTriggerTemplate::Export(void *pOrgData)
{
	CTriggerData * pData = (CTriggerData *) pOrgData;
	for(int i = 0; i < pData->GetOperaionNum(); ++i)
	{
		
		for( int j = 0; j < listText.size(); ++j)
		{
			CTriggerData::_s_operation *po = pData->GetOperaion(i);
			if(po->iType == CTriggerData::o_talk)
			{
				
				if(((CText_TalkWindow*)listText[j])->GetID()==i)
				{
					if(listText[j]->GetStatus()==CTextBase::status_normal && listText[j]->GetTranslationText()!=NULL)
					{
						O_TALK_TEXT* pTalk = (O_TALK_TEXT*)po->pParam;
						pTalk->uSize = _tcslen(listText[j]->GetTranslationText())*2 + 2;
						delete[] pTalk->szData;
						pTalk->szData = (unsigned short*) new char[pTalk->uSize];
						lstrcpy(pTalk->szData,listText[j]->GetTranslationText());
					}
				}
			}else if( po->iType == CTriggerData::o_summon )
			{
				if(((CText_TalkOption*)listText[j])->GetID()==i)
				{
					if(listText[j]->GetStatus()==CTextBase::status_normal && listText[j]->GetTranslationText()!=NULL)
					{
						O_SUMMON* pSummon = (O_SUMMON*)po->pParam;
						lstrcpy(pSummon->szName,listText[j]->GetTranslationText());
					}
				}	
			}
		}
		
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CPolicyTemplate
//////////////////////////////////////////////////////////////////////

CPolicyTemplate::~CPolicyTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();

	for( i = 0; i < listChild.size(); ++i)
		if(listChild[i]) delete listChild[i];
	listChild.clear();
}

bool CPolicyTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		ASSERT(FALSE);
	}

	fread(&size,sizeof(DWORD),1,pFile);
	for( i = 0; i < size; ++i)
	{
		CTemplateBase *pTemp = new CTriggerTemplate();
		if(!pTemp->Load(pFile,dwVersion)) return false;
		AddChild(pTemp);
	}

	return true;	
}

bool CPolicyTemplate::Save(FILE *pFile)
{
	fwrite(&id,sizeof(unsigned int),1,pFile);
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
	return true;	
}

void CPolicyTemplate::SetTreeStatus(int status)
{
	int size = listChild.size();
	for(int i = 0; i < size; ++i)
		((CTriggerTemplate*)listChild[i])->SetTreeStatus(status);
	
}

bool CPolicyTemplate::Import(void *pOrgData)
{
	CPolicyData * pData = (CPolicyData *) pOrgData;
	if(listChild.size() > 0 )
	{
		//对已经存在的数据的处理
		for( int i = 0; i < listChild.size(); ++i)
		{
			CTriggerTemplate *pOldTempl = (CTriggerTemplate*)listChild[i];
			bool bFound = false;
			for( int j = 0; j < pData->GetTriggerPtrNum(); ++j)
			{
				CTriggerData *pTrigger = pData->GetTriggerPtr(j);
				if(pTrigger->GetID() == pOldTempl->GetID())
				{
					if(!pOldTempl->Import(pTrigger)) return false;
					bFound = true;
					break;
				}
			}
			if(!bFound) pOldTempl->SetTreeStatus(CTextBase::status_disuse);
		}

		//对新数据的处理
		for( i = 0; i < pData->GetTriggerPtrNum(); ++i)
		{
			bool bFound = false;
			CTriggerData *pTrigger = pData->GetTriggerPtr(i);
			for( int j = 0; j < listChild.size(); ++j)
			{
				if(pTrigger->GetID() == listChild[j]->GetID())
				{
					bFound = true;
					break;
				}
			}
			
			if(!bFound)
			{
				CTemplateBase *pNewTempl = new CTriggerTemplate();
				if(!pNewTempl->Import(pTrigger)) return false;
				AddChild(pNewTempl);
			}

		}

	}else
	{
		id = pData->GetID();
		int num = pData->GetTriggerPtrNum();
		for( int i = 0; i < num; ++i)
		{
			CTriggerData* pTrigger = pData->GetTriggerPtr(i);
			CTemplateBase *pNewTempl = new CTriggerTemplate();
			if(!pNewTempl->Import(pTrigger)) return false;
			AddChild(pNewTempl);
		}
	}
	return true;
}

bool CPolicyTemplate::Export(void *pOrgData)
{
	CPolicyData * pData = (CPolicyData *) pOrgData;
	for(  int i = 0 ; i < pData->GetTriggerPtrNum(); ++i)
	{
		CTriggerData *pTrigger = pData->GetTriggerPtr(i);
		for( int j = 0; j < listChild.size(); ++j)
		{
			if(listChild[j]->GetID() == pTrigger->GetID())
			{
				if(!listChild[j]->Export(pTrigger)) return false;
				break;
			}
		}
	}
	return true;
}