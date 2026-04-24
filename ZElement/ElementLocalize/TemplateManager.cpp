// TemplateManager.cpp: implementation of the CTemplateManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ElementLocalize.h"
#include "TemplateManager.h"
#include "TaskTemplate.h"
#include "PolicyTemplate.h"
#include "PrecinctTemplate.h"
#include "GlobalDataTemplate.h"

#include "TemplateBase.h"
#include "ExpTypes.h"
#include "hometowntemplate.h"

extern CTemplateBase * g_pElementTypes[];
extern CTemplateBase * g_pHomeTownTypes[];

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CTemplateManager::CTemplateManager(int iChunk)
{
	iChunkType = iChunk;
}

CTemplateManager::~CTemplateManager()
{

}

CTemplateBase *CTemplateManager::FindTemplate(unsigned int uid, int nTType)
{
	for( int i = 0; i < listTemplate.size(); ++i)
	{
		if(listTemplate[i]->GetID() == uid && listTemplate[i]->GetTemplateType()==nTType)
			return listTemplate[i];
	}
	return NULL;
}

void CTemplateManager::AddTemplate(CTemplateBase *pNewTemplate)
{
	listTemplate.push_back(pNewTemplate); 
	pNewTemplate->SetChunkType(iChunkType);
}

//////////////////////////////////////////////////////////////////////
// CTM_ElementData
//////////////////////////////////////////////////////////////////////

CTM_ElementData::CTM_ElementData(int iChunk) : CTemplateManager(iChunk)
{

}

CTM_ElementData::~CTM_ElementData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_ElementData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		int nTType = listTemplate[i]->GetTemplateType();
		fwrite(&nTType,sizeof(int),1,pFile);
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_ElementData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		int nTType;
		fread(&nTType,sizeof(int),1,pFile);
		if(nTType > DT_INVALID && nTType < DT_MAX)
		{
			int nTemp = nTType;
			if (dwVersion<0x10000007)
			{
				if (nTType>41)
				{
					nTemp++;
				}
			}
			if (dwVersion<0x10000010)
			{
				if (nTType>120 && nTType != 124) // for DT_UPGRADE_EQUIP_CONFIG_1  2012-09-03
				{
					nTemp++;
				}
			}
			if (dwVersion==0x10000010)
			{
				if (nTType==155) // for DT_COLLISION_RAID_AWARD_CONFIG 2012-09-25
				{
					nTemp = 156;
				}
			}
			
			CTemplateBase *pNewTemplate = g_pElementTypes[nTemp]->Clone();
			bool r = pNewTemplate->Load(pFile,dwVersion);	
			if(!r) 
				return false;
			if(dwVersion < 4) 
			{
				delete pNewTemplate;
				continue;
			}
			AddTemplate(pNewTemplate);
		}
		else
		{
			return false;
		}
	}
	return true;
}

bool CTM_ElementData::Export(FILE *pFile)
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_HomeTownData
//////////////////////////////////////////////////////////////////////

CTM_HomeTownData::CTM_HomeTownData(int iChunk) : CTemplateManager(iChunk)
{

}

CTM_HomeTownData::~CTM_HomeTownData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_HomeTownData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		int nTType = listTemplate[i]->GetTemplateType();
		fwrite(&nTType,sizeof(int),1,pFile);
		bool r = listTemplate[i]->Save(pFile);
		if(!r) return false;
	}
	return true;
}

bool CTM_HomeTownData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		int nTType;
		fread(&nTType,sizeof(int),1,pFile);
		if(nTType < HOMETOWN::DT_MAX)
		{
			CTemplateBase *pNewTemplate = g_pHomeTownTypes[nTType]->Clone();
			bool r = pNewTemplate->Load(pFile,dwVersion);	
			if(!r) return false;
			AddTemplate(pNewTemplate);
		}
	}
	return true;
}

bool CTM_HomeTownData::Export(FILE *pFile)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTM_TaskData
//////////////////////////////////////////////////////////////////////


CTM_TaskData::CTM_TaskData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_TaskData::~CTM_TaskData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_TaskData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_TaskData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTaskTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_TaskData::Export(FILE *pFile)
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_OthersData
//////////////////////////////////////////////////////////////////////


CTM_OthersData::CTM_OthersData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_OthersData::~CTM_OthersData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_OthersData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_OthersData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementStringTab();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_OthersData::Export(FILE *pFile)
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_InstanceData
//////////////////////////////////////////////////////////////////////


CTM_InstanceData::CTM_InstanceData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_InstanceData::~CTM_InstanceData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_InstanceData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_InstanceData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementInstance();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_InstanceData::Export(FILE *pFile)
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_MessageData
//////////////////////////////////////////////////////////////////////


CTM_MessageData::CTM_MessageData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_MessageData::~CTM_MessageData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_MessageData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_MessageData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementMessage();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_MessageData::Export(FILE *pFile)
{
	return true;
}
//////////////////////////////////////////////////////////////////////
// CTM_InterfaceData
//////////////////////////////////////////////////////////////////////


CTM_InterfaceHintData::CTM_InterfaceHintData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_InterfaceHintData::~CTM_InterfaceHintData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_InterfaceHintData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_InterfaceHintData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementInterfaceHint();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_InterfaceHintData::Export(FILE *pFile)
{
	return true;
}

CTemplateBase *CTM_InterfaceHintData::FindTemplate(TCHAR *pIndex)
{
	for( int i = 0; i < listTemplate.size(); ++i)
	{
		CTemplate_ElementInterfaceHint *pTempl = (CTemplate_ElementInterfaceHint *)listTemplate[i];
		if(_tcsicmp(pTempl->GetIndexText(),pIndex)==0)
			return listTemplate[i];
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
// CTM_InterfaceStringData
//////////////////////////////////////////////////////////////////////


CTM_InterfaceStringData::CTM_InterfaceStringData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_InterfaceStringData::~CTM_InterfaceStringData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_InterfaceStringData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_InterfaceStringData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementInterfaceString();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_InterfaceStringData::Export(FILE *pFile)
{
	return true;
}

CTemplateBase *CTM_InterfaceStringData::FindTemplate(TCHAR *pIndex, int uid)
{
	for( int i = 0; i < listTemplate.size(); ++i)
	{
		CTemplate_ElementInterfaceHint *pTempl = (CTemplate_ElementInterfaceHint *)listTemplate[i];
		if(_tcsicmp(pTempl->GetIndexText(),pIndex)==0 && pTempl->GetID() == uid)
			return listTemplate[i];
	}
	return NULL;
}


//////////////////////////////////////////////////////////////////////
// CTM_PolicyData
//////////////////////////////////////////////////////////////////////


CTM_PolicyData::CTM_PolicyData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_PolicyData::~CTM_PolicyData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_PolicyData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_PolicyData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CPolicyTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_PolicyData::Export(FILE *pFile)
{
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTM_PrecinctData
//////////////////////////////////////////////////////////////////////


CTM_PrecinctData::CTM_PrecinctData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_PrecinctData::~CTM_PrecinctData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_PrecinctData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_PrecinctData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CPrecinctTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

CTemplateBase *CTM_PrecinctData::FindTemplate(char *szFileName)
{
	for( int i = 0; i < listTemplate.size(); ++i)
	{
		CPrecinctTemplate *pTempl = (CPrecinctTemplate *)listTemplate[i];
		if(stricmp(pTempl->GetFileName(),szFileName)==0)
			return listTemplate[i];
	}
	return NULL;
}

bool CTM_PrecinctData::Export(FILE *pFile)
{
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_InterfaceStringData
//////////////////////////////////////////////////////////////////////


CTM_TreasureData::CTM_TreasureData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_TreasureData::~CTM_TreasureData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_TreasureData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_TreasureData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTreasureTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

bool CTM_TreasureData::Export(FILE *pFile)
{
	return true;
}

/*
//////////////////////////////////////////////////////////////////////
// CTM_TransTargetData
//////////////////////////////////////////////////////////////////////


CTM_TransTargetData::CTM_TransTargetData()
{
}

CTM_TransTargetData::~CTM_TransTargetData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_TransTargetData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_TransTargetData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTransTargetTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}
*/

//////////////////////////////////////////////////////////////////////
// CTM_DomainInforData
//////////////////////////////////////////////////////////////////////


CTM_DomainInforData::CTM_DomainInforData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_DomainInforData::~CTM_DomainInforData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_DomainInforData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_DomainInforData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CDomainInforTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

//////////////////////////////////////////////////////////////////////
// CTM_GShopData
//////////////////////////////////////////////////////////////////////


CTM_GShopData::CTM_GShopData(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_GShopData::~CTM_GShopData()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_GShopData::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_GShopData::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CGShopTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_GShopTypes
//////////////////////////////////////////////////////////////////////


CTM_GShopTypes::CTM_GShopTypes(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_GShopTypes::~CTM_GShopTypes()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_GShopTypes::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_GShopTypes::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CGShopTypesTemplate();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}


//////////////////////////////////////////////////////////////////////
// CTM_TextFile
//////////////////////////////////////////////////////////////////////


CTM_TextFile::CTM_TextFile(int iChunk) : CTemplateManager(iChunk)
{
}

CTM_TextFile::~CTM_TextFile()
{
	for( int i = 0; i < listTemplate.size(); ++i)
		delete listTemplate[i];
	listTemplate.clear();
}

bool CTM_TextFile::Save(FILE *pFile)
{
	DWORD size = listTemplate.size();
	fwrite(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		bool r = listTemplate[i]->Save(pFile);	
		if(!r) return false;
	}
	return true;
}

bool CTM_TextFile::Load(FILE *pFile, DWORD dwVersion)
{
	DWORD size = 0;
	fread(&size,sizeof(DWORD),1,pFile);
	for( int i = 0; i < size; ++i)
	{
		CTemplateBase *pNewTemplate = new CTemplate_ElementTextFile();
		bool r = pNewTemplate->Load(pFile,dwVersion);	
		if(!r) return false;
		AddTemplate(pNewTemplate);
	}
	return true;
}

CTemplateBase *CTM_TextFile::FindTemplate(TCHAR *pIndex)
{
	for( int i = 0; i < listTemplate.size(); ++i)
	{
		CTemplate_ElementTextFile *pTempl = (CTemplate_ElementTextFile *)listTemplate[i];
		if(_tcsicmp(pTempl->GetIndexText(),pIndex)==0)
			return listTemplate[i];
	}
	return NULL;
}










