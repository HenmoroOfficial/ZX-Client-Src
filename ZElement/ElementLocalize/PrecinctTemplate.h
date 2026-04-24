// PrecinctTemplate.h: interface for the CPrecinctTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PRECINCTTEMPLATE_H__2A2BE643_2C7F_4063_AC7E_348D9361C007__INCLUDED_)
#define AFX_PRECINCTTEMPLATE_H__2A2BE643_2C7F_4063_AC7E_348D9361C007__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "TemplateBase.h"

class CPrecinctTemplate :public CTemplateBase 
{
public:
	CPrecinctTemplate(): CTemplateBase(){};
	virtual ~CPrecinctTemplate();


	virtual CTemplateBase * Clone()
	{
		return new CPrecinctTemplate();
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	void SetTreeStatus(int status);
	char* GetFileName(){ return szFileName; }
	virtual CString GetStrID();

protected:
	char szFileName[256];
};

#endif // !defined(AFX_PRECINCTTEMPLATE_H__2A2BE643_2C7F_4063_AC7E_348D9361C007__INCLUDED_)
