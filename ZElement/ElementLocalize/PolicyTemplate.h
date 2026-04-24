// PolicyTemplate.h: interface for the CTriggerTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POLICYTEMPLATE_H__90426F89_B92D_4861_9B34_7C13E00B1E08__INCLUDED_)
#define AFX_POLICYTEMPLATE_H__90426F89_B92D_4861_9B34_7C13E00B1E08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TemplateBase.h"

class CTriggerTemplate :public CTemplateBase 
{
public:
	CTriggerTemplate(): CTemplateBase(){};
	virtual ~CTriggerTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CTriggerTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	void SetTreeStatus(int status);

};


class CPolicyTemplate :public CTemplateBase 
{
public:
	CPolicyTemplate(): CTemplateBase(){};
	virtual ~CPolicyTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CPolicyTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	void SetTreeStatus(int status);

};

#endif // !defined(AFX_POLICYTEMPLATE_H__90426F89_B92D_4861_9B34_7C13E00B1E08__INCLUDED_)
