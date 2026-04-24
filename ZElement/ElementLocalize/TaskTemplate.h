// TaskTemplate.h: interface for the CTaskTemplate class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TASKTEMPLATE_H__6CEBACEE_F00C_4A6A_B723_9B802D94ABE6__INCLUDED_)
#define AFX_TASKTEMPLATE_H__6CEBACEE_F00C_4A6A_B723_9B802D94ABE6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TemplateBase.h"

class CTaskTemplate :public CTemplateBase
{
public:
	CTaskTemplate(): CTemplateBase(){};
	CTaskTemplate(int nttype): CTemplateBase(){ template_type = nttype; }
	virtual ~CTaskTemplate();

	virtual CTemplateBase * Clone()
	{
		return new CTaskTemplate(template_type);
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
	
protected:
	bool _AddChild(CTemplateBase* pTemplate, void* pData, int id);
	bool _AddText(CText_Resize* pTextBase, TCHAR* pText, TCHAR* szTextName);

	bool Update(void *pOrgData);
	void SetTreeStatus(int status);

};

struct AWARD_DATA;
class CTemplate_TaskAward : public CTemplateBase
{
public:
	CTemplate_TaskAward() {};
	virtual ~CTemplate_TaskAward();

	virtual CTemplateBase* Clone()
	{
		return new CTemplate_TaskAward;
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);

protected:
	bool _Update(const AWARD_DATA* data);
	bool _Import(const AWARD_DATA* data);
};


class CTemplate_TaskAwardRatio : public CTemplateBase
{
public:
	CTemplate_TaskAwardRatio() {};
	virtual ~CTemplate_TaskAwardRatio();

	virtual CTemplateBase* Clone()
	{
		return new CTemplate_TaskAwardRatio;
	}

public:
	virtual bool Load(FILE *pFile, DWORD dwVersion);
	virtual bool Save(FILE *pFile);
	
	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_TaskAwardItem : public CTemplate_TaskAwardRatio
{
public:
	virtual CTemplateBase* Clone()
	{
		return new CTemplate_TaskAwardItem;
	}

	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

class CTemplate_TaskAwardCount : public CTemplate_TaskAwardRatio
{
public:
	virtual CTemplateBase* Clone()
	{
		return new CTemplate_TaskAwardCount;
	}

	virtual bool Import(void *pOrgData);
	virtual bool Export(void *pOrgData);
};

#endif // !defined(AFX_TASKTEMPLATE_H__6CEBACEE_F00C_4A6A_B723_9B802D94ABE6__INCLUDED_)
