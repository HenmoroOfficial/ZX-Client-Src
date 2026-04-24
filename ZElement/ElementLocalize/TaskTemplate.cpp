// TaskTemplate.cpp: implementation of the CTaskTemplate class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementlocalize.h"
#include "TaskTemplate.h"
#include "TaskTempl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define B_AWARD_DATA
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CTaskTemplate::~CTaskTemplate()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	
	for( i = 0; i < listChild.size(); ++i)
		if(listChild[i]) delete listChild[i];
	listChild.clear();
}

bool CTaskTemplate::Load(FILE *pFile, DWORD dwVersion)
{
	bool hr;
	fread(&id,sizeof(unsigned int),1,pFile);
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	
//	if(size!=8) return true;
	
	CTextBase * pText = new CText_Normal();
	pText->Init(MAX_TASK_NAME_LEN,false,NULL, NULL, _T("Name"));
	hr = pText->Load(pFile,dwVersion);
	if(pText->GetTranslationText()) pText->GetTranslationText()[MAX_TASK_NAME_LEN - 1] = 0;
	if(!hr) return false;
	AddText(pText);

	if (dwVersion > 0x10000012)
	{
		pText = new CText_Normal();
		pText->Init(MAX_TASK_NAME_LEN,false,NULL, NULL, _T("AutoMoveDestPosName"));
		hr = pText->Load(pFile,dwVersion);
		if(pText->GetTranslationText()) pText->GetTranslationText()[MAX_TASK_NAME_LEN - 1] = 0;
		if(!hr) return false;
		AddText(pText);
	}
	else
	{
		pText = new CText_Normal();
		pText->Init(MAX_TASK_NAME_LEN,false,NULL, NULL, _T("AutoMoveDestPosName"));	
		AddText(pText);
	}
	
	CText_Resize*pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("Signature"));
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;
	AddText(pTextResize);
	pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("Descript"));
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;
	AddText(pTextResize);
	pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("OkText"));
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;
	AddText(pTextResize);
	pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("NoText"));
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;
	AddText(pTextResize);
	pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("Tribute"));
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;
	AddText(pTextResize);

	int iDestSize = 8;
	if (dwVersion > 0x10000012)
	{
		iDestSize = 9;
	}
	if (size == iDestSize)
	{
		pTextResize = new CText_Resize();
		pTextResize->Init(true,NULL, NULL, _T("HintText"));
		hr = pTextResize->Load(pFile,dwVersion);
		if(!hr) return false;
		AddText(pTextResize);
		pTextResize = new CText_Resize();
		pTextResize->Init(true,NULL, NULL, _T("CanDeliverText"));
		hr = pTextResize->Load(pFile,dwVersion);
		if(!hr) return false;
		AddText(pTextResize);
	}
	
	fread(&size,sizeof(DWORD),1,pFile);
	

	for(int i = 0; i < size; ++i)
	{
		CTemplateBase* pTemplate = NULL;
#ifdef B_AWARD_DATA
		switch (i)
		{
		case 0: case 1: case 2: case 3: case 4:
			pTemplate = new CTemplate_ElementTalk();
			break;

		case 5: case 6:
			pTemplate = new CTemplate_TaskAward;
			break;

		case 7: case 8:
			pTemplate = new CTemplate_TaskAwardCount;
			break;
			
		case 9: case 10:
			pTemplate = new CTemplate_TaskAwardItem;
			break;
			
		case 11: case 12:
			pTemplate = new CTemplate_TaskAwardRatio;
			break;

		default:
			continue;
		}
#else
		pTemplate = new CTemplate_ElementTalk;
#endif
		if(!pTemplate->Load(pFile,dwVersion))
			return false;
		if(pTemplate->GetID() != 100000 + i + 1 ) pTemplate->SetID(100000 + i + 1);//Old version translate
		AddChild(pTemplate);
	}
	
	return true;
}

bool CTaskTemplate::Save(FILE *pFile)
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
#ifdef B_AWARD_DATA
	size = listChild.size();
#else 
	size = 5;
#endif
	fwrite(&size,sizeof(DWORD),1,pFile);
	for(i = 0; i < size; ++i)
	{
		bool r = listChild[i]->Save(pFile);
		if(!r) return false;
	}
	return true;
}

void CTaskTemplate::SetTreeStatus(int status)
{
	for( int i = 0; i < listText.size(); ++i)
		listText[i]->SetStatus(status);

	for( i = 0; i < 5; ++i)
		((CTemplate_ElementTalk*)listChild[i])->SetStatus(status);
	
	for( i = 5; i < listChild.size(); ++i)
		((CTaskTemplate*)listChild[i])->SetTreeStatus(status);
}

bool CTaskTemplate::Update(void *pOrgData)
{
	ATaskTempl * pData = (ATaskTempl *) pOrgData;

	int size = 0;
	//name[32];
	if (!listText.empty())
		listText[size]->UpdateFromData(pData->m_szName);

	//AutoMoveDestPosName
	if (listText.size() > ++size)
	{
		listText[size]->UpdateFromData(pData->m_szAutoMoveDestPosName);
	}

	//动态长度 5个
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pszSignature);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrDescript);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrOkText);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrNoText);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrTribute);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrHintText);
	if (listText.size() > ++size)
		listText[size]->UpdateFromData(pData->m_pwstrCanDeliverText);


	//处理它的5个对话
	CTemplateBase* pChildTalk = listChild[0];
	if(!pChildTalk->Import((void*)&pData->m_DelvTaskTalk)) return false;
	pChildTalk = listChild[1];
	if(!pChildTalk->Import((void*)&pData->m_UnqualifiedTalk)) return false;
	pChildTalk = listChild[2];
	if(!pChildTalk->Import((void*)&pData->m_DelvItemTalk)) return false;
	pChildTalk = listChild[3];
	if(!pChildTalk->Import((void*)&pData->m_ExeTalk)) return false;
	pChildTalk = listChild[4];
	if(!pChildTalk->Import((void*)&pData->m_AwardTalk)) return false;

	//处理8个AWARD
	if (listChild.size() > 5)
		listChild[5]->Import((void*)pData->m_Award_S);
	else
		_AddChild(new CTemplate_TaskAward, (void*)pData->m_Award_S, 100001 + listChild.size());

	if (listChild.size() > 6)
		listChild[6]->Import((void*)pData->m_Award_F);
	else
		_AddChild(new CTemplate_TaskAward, (void*)pData->m_Award_F, 100001 + listChild.size());

	if (listChild.size() > 7)
		listChild[7]->Import((void*)pData->m_AwByCount_S);
	else
		_AddChild(new CTemplate_TaskAwardCount, (void*)pData->m_AwByCount_S, 100001 + listChild.size());

	if (listChild.size() > 8)
		listChild[8]->Import((void*)pData->m_AwByCount_F);
	else
		_AddChild(new CTemplate_TaskAwardCount, (void*)pData->m_AwByCount_F, 100001 + listChild.size());

	if (listChild.size() > 9)
		listChild[9]->Import((void*)pData->m_AwByItems_S);
	else
		_AddChild(new CTemplate_TaskAwardItem, (void*)pData->m_AwByItems_S, 100001 + listChild.size());

	if (listChild.size() > 10)
		listChild[10]->Import((void*)pData->m_AwByItems_F);
	else
		_AddChild(new CTemplate_TaskAwardItem, (void*)pData->m_AwByItems_F, 100001 + listChild.size());

	if (listChild.size() > 11)
		listChild[11]->Import((void*)pData->m_AwByRatio_S);
	else
		_AddChild(new CTemplate_TaskAwardRatio, (void*)pData->m_AwByRatio_S, 100001 + listChild.size());

	if (listChild.size() > 12)
		listChild[12]->Import((void*)pData->m_AwByRatio_F);
	else
		_AddChild(new CTemplate_TaskAwardRatio, (void*)pData->m_AwByRatio_F, 100001 + listChild.size());

	return true;
}
	
bool CTaskTemplate::Import(void *pOrgData)
{
	ATaskTempl * pData = (ATaskTempl *) pOrgData;
	if(listText.size() > 0)
	{// 比较md5，并进行更新，同时判断更新标志
	   if(id!=pData->GetID()) return false;
	   if(!Update(pOrgData)) return false;
	}else
	{
		id = pData->GetID();
		//name[30];
		CTextBase * pText = new CText_Normal();
		pText->Init(MAX_TASK_NAME_LEN,false,pData->m_szName, NULL, _T("Name"));
		AddText(pText);

		//AudoMovePosName[30]
		pText = new CText_Normal();
		pText->Init(MAX_TASK_NAME_LEN,false,pData->m_szAutoMoveDestPosName, NULL, _T("AudoMovePosName"));
		AddText(pText);		

		 //动态长度？
		_AddText(new CText_Resize, pData->m_pszSignature, _T("Signature"));
		_AddText(new CText_Resize, pData->m_pwstrDescript, _T("Descript"));
		_AddText(new CText_Resize, pData->m_pwstrOkText, _T("OkText"));
		_AddText(new CText_Resize, pData->m_pwstrNoText, _T("NoText"));
		_AddText(new CText_Resize, pData->m_pwstrTribute, _T("Tribute"));
		_AddText(new CText_Resize, pData->m_pwstrHintText, _T("HintText"));
		_AddText(new CText_Resize, pData->m_pwstrCanDeliverText, _T("CanDeliverText"));
				
		//处理它的5个对话
		_AddChild(new CTemplate_ElementTalk, (void*)&pData->m_DelvTaskTalk, 100001 + listChild.size());
		_AddChild(new CTemplate_ElementTalk, (void*)&pData->m_UnqualifiedTalk, 100001 + listChild.size());
		_AddChild(new CTemplate_ElementTalk, (void*)&pData->m_DelvItemTalk, 100001 + listChild.size());
		_AddChild(new CTemplate_ElementTalk, (void*)&pData->m_ExeTalk, 100001 + listChild.size());
		_AddChild(new CTemplate_ElementTalk, (void*)&pData->m_AwardTalk, 100001 + listChild.size());			
		
		//处理AWARD 2011-12-02
		_AddChild(new CTemplate_TaskAward, (void*)pData->m_Award_S, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAward, (void*)pData->m_Award_F, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardCount, (void*)pData->m_AwByCount_S, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardCount, (void*)pData->m_AwByCount_F, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardItem, (void*)pData->m_AwByItems_S, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardItem, (void*)pData->m_AwByItems_F, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardRatio, (void*)pData->m_AwByRatio_S, 100001 + listChild.size());
		_AddChild(new CTemplate_TaskAwardRatio, (void*)pData->m_AwByRatio_F, 100001 + listChild.size());
	}
	
	return true;
}

bool CTaskTemplate::Export(void *pOrgData)
{
	if(listText.size() < 1) return false;
	
	ATaskTempl * pData = (ATaskTempl *) pOrgData;
	
	int size = 0;
	//固定长度的
	if (listText.size() > size)
	{
		if (listText[size]->GetStatus() == CTextBase::status_normal && listText[size]->GetTranslationText() && _tcslen(listText[size]->GetTranslationText()) > 0)
			_tcscpy(pData->m_szName, listText[size]->GetTranslationText());		
	}

	if (listText.size() > ++size)
	{
		if (listText[size]->GetStatus() == CTextBase::status_normal && listText[size]->GetTranslationText() && _tcslen(listText[size]->GetTranslationText()) > 0)
			_tcscpy(pData->m_szAutoMoveDestPosName, listText[size]->GetTranslationText());
	}

	//变长的，只好替换它的指针了
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pszSignature, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrDescript, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrOkText, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrNoText, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrTribute, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrHintText, true);
	if (listText.size() > ++size)
		listText[size]->ExportToData(pData->m_pwstrCanDeliverText, true);

	//输出5个对话
	if(!((CTemplate_ElementTalk*)listChild[0])->Export((void*)&pData->m_DelvTaskTalk)) return false;
	if(!((CTemplate_ElementTalk*)listChild[1])->Export((void*)&pData->m_UnqualifiedTalk)) return false;
	if(!((CTemplate_ElementTalk*)listChild[2])->Export((void*)&pData->m_DelvItemTalk)) return false;
	if(!((CTemplate_ElementTalk*)listChild[3])->Export((void*)&pData->m_ExeTalk)) return false;
	if(!((CTemplate_ElementTalk*)listChild[4])->Export((void*)&pData->m_AwardTalk)) return false;
	
	//8*AWARD
	if (listChild.size() > 5 && !listChild[5]->Export((void*)pData->m_Award_S))
		return false;
	if (listChild.size() > 6 && !listChild[6]->Export((void*)pData->m_Award_F))
		return false;
	if (listChild.size() > 7 && !listChild[7]->Export((void*)pData->m_AwByCount_S))
		return false;
	if (listChild.size() > 8 && !listChild[8]->Export((void*)pData->m_AwByCount_F))
		return false;
	if (listChild.size() > 9 && !listChild[9]->Export((void*)pData->m_AwByItems_S))
		return false;
	if (listChild.size() > 10 && !listChild[10]->Export((void*)pData->m_AwByItems_F))
		return false;
	if (listChild.size() > 11 && !listChild[11]->Export((void*)pData->m_AwByRatio_S))
		return false;
	if (listChild.size() > 12 && !listChild[12]->Export((void*)pData->m_AwByRatio_F))
		return false;

	return true;
}

bool CTaskTemplate::_AddChild(CTemplateBase* pTemplate, void* pData, int id)
{
	if (!pTemplate || !pData)
		return false;

	if (!pTemplate->Import(pData))
	{
		delete pTemplate;
		return false;
	}
	pTemplate->SetID(id);

	AddChild(pTemplate);

	return true;
}

bool CTaskTemplate::_AddText(CText_Resize* pTextBase, TCHAR* pText, TCHAR* szTextName)
{
	if (!pTextBase)
		return false;

	pTextBase->Init(true, pText, NULL, szTextName);
	AddText(pTextBase);

	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_TaskAward
//////////////////////////////////////////////////////////////////////////
CTemplate_TaskAward::~CTemplate_TaskAward()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	
	for( i = 0; i < listChild.size(); ++i)
		if(listChild[i]) delete listChild[i];
	listChild.clear();
}

bool CTemplate_TaskAward::Load(FILE *pFile, DWORD dwVersion)
{
	bool hr;
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	if (size != 1)
		return false;

	CText_Resize* pTextResize = new CText_Resize();
	pTextResize->Init(true,NULL, NULL, _T("ExtraTribute"));
	AddText(pTextResize);
	hr = pTextResize->Load(pFile,dwVersion);
	if(!hr) return false;

	fread(&size,sizeof(DWORD),1,pFile);
	for (DWORD i=0; i<size; i++)
	{
		CTemplate_TaskAward* pChild = new CTemplate_TaskAward;
		AddChild(pChild);
		hr = pChild->Load(pFile, dwVersion);
		if (!hr) return false;
	}
	
	return true;
}

bool CTemplate_TaskAward::Save(FILE *pFile)
{
	bool hr;
	DWORD size = (DWORD)listText.size();
	fwrite(&size, sizeof(DWORD), 1, pFile);
	DWORD i=0;
	for (i=0; i<size; i++)
	{
		hr = listText[i]->Save(pFile);
		if (!hr)
			return false;
	}

	size = (DWORD)listChild.size();
	fwrite(&size, sizeof(DWORD), 1, pFile);
	for (i=0; i<size; i++)
	{
		hr = listChild[i]->Save(pFile);
		if (!hr)
			return false;
	}

	return true;
}

bool CTemplate_TaskAward::Import(void *pOrgData)
{
	AWARD_DATA* pData = (AWARD_DATA*) pOrgData;
	if (!pData)
		return false;

	if (listText.empty())
		return _Import(pData);
	else
		return _Update(pData);
}

bool CTemplate_TaskAward::Export(void *pOrgData)
{
	AWARD_DATA* pData = (AWARD_DATA*) pOrgData;
	if (!pData)
		return false;

	if (!listText.empty())
	{
		//变长的，替换指针
		if( listText[0]->GetStatus() == CTextBase::status_normal)
		{
			int len = 0;
			if(listText[0]->GetTranslationText()) len = _tcslen(listText[0]->GetTranslationText());
			if(len > 0)
			{
				if(pData->m_pwstrExtraTribute) delete []pData->m_pwstrExtraTribute;
				pData->m_pwstrExtraTribute = new TCHAR[len+1];
				lstrcpy(pData->m_pwstrExtraTribute,listText[0]->GetTranslationText());
			}
		}
	}
	
	if (!listChild.empty() && pData->m_pAwardSpecifyRole)
	{
		if (!listChild[0]->Export(pData->m_pAwardSpecifyRole))
			return false;
	}

	return true;
}

bool CTemplate_TaskAward::_Import(const AWARD_DATA* pData)
{
	ASSERT(listText.empty() && pData);
	if (!pData)
		return true;

	const AWARD_DATA& data = *pData;
	CText_Resize* pText = new CText_Resize;
	pText->Init(true, data.m_pwstrExtraTribute, NULL, _T("ExtraTribute"));

	AddText(pText);

	if (data.m_pAwardSpecifyRole)
	{
		CTemplate_TaskAward* pChild = new CTemplate_TaskAward;
		AddChild(pChild);
		if (!pChild->Import((void*)data.m_pAwardSpecifyRole))
			return false;
	}

	return true;
}

bool CTemplate_TaskAward::_Update(const AWARD_DATA* pData)
{
	ASSERT(!listText.empty() && pData);
	if (!pData)
		return true;

	const AWARD_DATA& data = *pData;
	if (!listText[0]->UpdateFromData(data.m_pwstrExtraTribute))
		return false;
	
	if (data.m_pAwardSpecifyRole)
	{
		if (listChild.empty())
			AddChild(new CTemplate_TaskAward);

		if (!listChild[0]->Import(data.m_pAwardSpecifyRole))
			return false;
	}

	return true;
}


//////////////////////////////////////////////////////////////////////////
// CTemplate_TaskAwardRatio
//////////////////////////////////////////////////////////////////////////
CTemplate_TaskAwardRatio::~CTemplate_TaskAwardRatio()
{
	for( int i = 0; i < listText.size(); ++i)
		if(listText[i]) delete listText[i];
	listText.clear();
	
	for( i = 0; i < listChild.size(); ++i)
		if(listChild[i]) delete listChild[i];
	listChild.clear();
}

bool CTemplate_TaskAwardRatio::Load(FILE *pFile, DWORD dwVersion)
{
	bool hr;
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for (DWORD i=0; i<size; i++)
	{
		CTemplate_TaskAward* pChild = new CTemplate_TaskAward;
		AddChild(pChild);
		hr = pChild->Load(pFile, dwVersion);
		if (!hr) return false;
	}
	
	return true;
}

bool CTemplate_TaskAwardRatio::Save(FILE *pFile)
{
	bool hr;
	DWORD i = 0;
	DWORD size = (DWORD)listChild.size();
	fwrite(&size, sizeof(DWORD), 1, pFile);
	for (i=0; i<size; i++)
	{
		hr = listChild[i]->Save(pFile);
		if (!hr)
			return false;
	}

	return true;
}

bool CTemplate_TaskAwardRatio::Import(void *pOrgData)
{
	AWARD_RATIO_SCALE* pData = (AWARD_RATIO_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Import(&(pData->m_Awards[i])))
			return false;
	}

	for (; i<pData->m_ulScales; i++)
	{
		AddChild(new CTemplate_TaskAward);
		if (!listChild[i]->Import(&(pData->m_Awards[i])))
			return false;
	}

	return true;
}

bool CTemplate_TaskAwardRatio::Export(void *pOrgData)
{
	AWARD_RATIO_SCALE* pData = (AWARD_RATIO_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Export(&pData->m_Awards[i]))
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_TaskAwardItem
//////////////////////////////////////////////////////////////////////////
bool CTemplate_TaskAwardItem::Import(void *pOrgData)
{
	AWARD_ITEMS_SCALE* pData = (AWARD_ITEMS_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Import(&pData->m_Awards[i]))
			return false;
	}

	for (; i<pData->m_ulScales; i++)
	{
		AddChild(new CTemplate_TaskAward);
		if (!listChild[i]->Import(&pData->m_Awards[i]))
			return false;
	}

	return true;
}

bool CTemplate_TaskAwardItem::Export(void *pOrgData)
{
	AWARD_ITEMS_SCALE* pData = (AWARD_ITEMS_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Export(&pData->m_Awards[i]))
			return false;
	}

	return true;
}

//////////////////////////////////////////////////////////////////////////
// CTemplate_TaskAwardCount
//////////////////////////////////////////////////////////////////////////
bool CTemplate_TaskAwardCount::Import(void *pOrgData)
{
	AWARD_COUNT_SCALE* pData = (AWARD_COUNT_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Import(&pData->m_Awards[i]))
			return false;
	}

	for (; i<pData->m_ulScales; i++)
	{
		AddChild(new CTemplate_TaskAward);
		if (!listChild[i]->Import(&pData->m_Awards[i]))
			return false;
	}

	return true;
}

bool CTemplate_TaskAwardCount::Export(void *pOrgData)
{
	AWARD_COUNT_SCALE* pData = (AWARD_COUNT_SCALE*) pOrgData;
	if (!pData)
		return false;

	int i = 0;
	for (i=0; i<listChild.size() && i<pData->m_ulScales; i++)
	{
		if (!listChild[i]->Export(&pData->m_Awards[i]))
			return false;
	}

	return true;
}
