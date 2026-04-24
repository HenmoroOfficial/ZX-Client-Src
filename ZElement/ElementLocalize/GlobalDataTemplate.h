// GlobalDataTemplate.h: interface for the GlobalDataTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GLOBALDATATEMPLATE_H__4AA5F871_2933_4105_9DF2_02C70C67772B__INCLUDED_)
#define AFX_GLOBALDATATEMPLATE_H__4AA5F871_2933_4105_9DF2_02C70C67772B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


#include "TemplateBase.h"
#include "globaldataman.h"

struct GSHOP_MAIN_TYPE_PARAMS 
{
	unsigned int id;
	GSHOP_MAIN_TYPE* pMainType;
};

/*
class CTransTargetTemplate :public CTemplateBase  
{
public:
	CTransTargetTemplate(): CTemplateBase(){};
	virtual ~CTransTargetTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CTransTargetTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};
*/

class CDomainInforTemplate :public CTemplateBase  
{
public:
	CDomainInforTemplate(): CTemplateBase(){};
	virtual ~CDomainInforTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CDomainInforTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};


class CGShopTemplate :public CTemplateBase  
{
public:
	CGShopTemplate(): CTemplateBase(){};
	virtual ~CGShopTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CGShopTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CGShopTypesTemplate :public CTemplateBase  
{
public:
	CGShopTypesTemplate(): CTemplateBase(){};
	virtual ~CGShopTypesTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CGShopTypesTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CGShopSubTypesTemplate :public CTemplateBase  
{
public:
	CGShopSubTypesTemplate(): CTemplateBase(){};
	virtual ~CGShopSubTypesTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CGShopSubTypesTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};




#endif // !defined(AFX_GLOBALDATATEMPLATE_H__4AA5F871_2933_4105_9DF2_02C70C67772B__INCLUDED_)
