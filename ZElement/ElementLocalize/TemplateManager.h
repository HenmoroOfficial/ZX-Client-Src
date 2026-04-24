// TemplateManager.h: interface for the CTemplateManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEMPLATEMANAGER_H__43A4E1E9_676E_4E35_954A_E4A143E35961__INCLUDED_)
#define AFX_TEMPLATEMANAGER_H__43A4E1E9_676E_4E35_954A_E4A143E35961__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector.h>

class CTemplateBase;

class CTemplateManager  
{
public:
	CTemplateManager(int iChunk);
	virtual ~CTemplateManager();
public:
	virtual bool Save(FILE *pFile) = 0;
	virtual bool Load(FILE *pFile, DWORD dwVersion) = 0;
    virtual bool Export(FILE *pFile){ return true; }
	
	int GetTempalteNum(){ return listTemplate.size(); }
	CTemplateBase * GetTemplateByIdx( int idx)
	{
		if(idx >= 0 && idx < listTemplate.size()) 
			return listTemplate[idx];
		return NULL;
	}
	void SetTemplateToNull(int idx)
	{
		listTemplate[idx] = NULL;
	}
	virtual CTemplateBase *FindTemplate(unsigned int uid, int nTType);
	void AddTemplate(CTemplateBase *pNewTemplate);

protected:
	abase::vector<CTemplateBase *> listTemplate;
	int iChunkType;

};

class CTM_ElementData : public CTemplateManager
{
public:
	CTM_ElementData(int iChunk);
	virtual ~CTM_ElementData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_HomeTownData : public CTemplateManager
{
public:
	CTM_HomeTownData(int iChunk);
	virtual ~CTM_HomeTownData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_TaskData : public CTemplateManager
{
public:
	CTM_TaskData(int iChunk);
	virtual ~CTM_TaskData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_OthersData : public CTemplateManager
{
public:
	CTM_OthersData(int iChunk);
	virtual ~CTM_OthersData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_InstanceData :  public CTemplateManager
{
public:
	CTM_InstanceData(int iChunk);
	virtual ~CTM_InstanceData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_MessageData : public CTemplateManager
{
public:
	CTM_MessageData(int iChunk);
	virtual ~CTM_MessageData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_InterfaceHintData : public CTemplateManager
{
public:
	CTM_InterfaceHintData(int iChunk);
	virtual ~CTM_InterfaceHintData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);

	virtual CTemplateBase *FindTemplate(TCHAR *pIndex);
};


class CTM_InterfaceStringData : public CTemplateManager
{
public:
	CTM_InterfaceStringData(int iChunk);
	virtual ~CTM_InterfaceStringData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
	virtual CTemplateBase *FindTemplate(TCHAR *pIndex, int uid);
};

class CTM_PolicyData : public CTemplateManager
{
public:
	CTM_PolicyData(int iChunk);
	virtual ~CTM_PolicyData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};

class CTM_TreasureData : public CTemplateManager
{
public:
	CTM_TreasureData(int iChunk);
	virtual ~CTM_TreasureData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
};


class CTM_PrecinctData : public CTemplateManager
{
public:
	CTM_PrecinctData(int iChunk);
	virtual ~CTM_PrecinctData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Export(FILE *pFile);
	virtual CTemplateBase *FindTemplate(char *szFileName);
};

/*
class CTM_TransTargetData : public CTemplateManager
{
public:
	CTM_TransTargetData();
	virtual ~CTM_TransTargetData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
};
*/

class CTM_DomainInforData : public CTemplateManager
{
public:
	CTM_DomainInforData(int iChunk);
	virtual ~CTM_DomainInforData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
};


class CTM_GShopData : public CTemplateManager
{
public:
	CTM_GShopData(int iChunk);
	virtual ~CTM_GShopData();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
};


class CTM_GShopTypes : public CTemplateManager
{
public:
	CTM_GShopTypes(int iChunk);
	virtual ~CTM_GShopTypes();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
};

class CTM_TextFile : public CTemplateManager
{
public:
	CTM_TextFile(int iChunk);
	virtual ~CTM_TextFile();

public:
	virtual bool Save(FILE *pFile);
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual CTemplateBase *FindTemplate(TCHAR *pIndex);
};

#endif // !defined(AFX_TEMPLATEMANAGER_H__43A4E1E9_676E_4E35_954A_E4A143E35961__INCLUDED_)
