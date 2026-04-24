// TextBase.h: interface for the CTextBase class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_TEXTBASE_H__02E36E51_AAB3_49AF_943B_CC3EC5EF5390__INCLUDED_)
#define AFX_TEXTBASE_H__02E36E51_AAB3_49AF_943B_CC3EC5EF5390__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRID_TOK _T(".")

struct SYNTAX_ITEM
{
	int start;
	int len;
	CString txt;
};

class CTemplateBase;
class CTextBase  
{
public:
	enum
	{
		status_normal = 0,
		status_new,
		status_disuse,
	};
	
	CTextBase();
	virtual ~CTextBase();

	static bool     DataIsEqual(char *pData1, int nLen1, char *pData2, int nLen2);
public:
	
	virtual CTextBase* Clone() = 0;
	virtual bool    CheckSyntax(CString strDest, int& start, int& len, bool bMsg = true) = 0;
	virtual bool    ReplaceOriginalText(const TCHAR* pNewText);
	virtual bool    ReplaceTransText(const TCHAR *pNewText);
	virtual bool	Load(FILE *pFile, DWORD dwVersion);
	virtual bool	Save(FILE *pFile);
	virtual void    Init(unsigned int nLmt, bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName);
	virtual bool	UpdateFromData(const TCHAR* pOrgText);
	virtual bool	ExportToData(TCHAR*& pDataText, bool bReallocate);
	virtual bool	ExportToData(TCHAR* pDataText);
	
	virtual int     GetID(){ return 0; }
	virtual void    SetID( int uid ){}
	virtual int     GetParam(){ return 0; }
	virtual void    SetParam( int uid ){}
	
	int             GetCharUpperLimit(){ return nLimit; }
	TCHAR*			GetOriginalText(){ return pOriginalText; }
	TCHAR*			GetTranslationText(){ return pTranslationText; }
	void            SetStatus( int ns){ nStatus = ns; }
	int             GetStatus(){ return nStatus; }
	bool            IsMuiltiLine(){  return bMuiltiLine; }
	int             GetOriginalTextLineNum();
	TCHAR*          GetTextName(){ return szTextName; }
	void            SetTextName(TCHAR *pTextName);
	TCHAR*			GetTextLabel() { return szLabel; }
	void			SetTextLabel(const TCHAR* pLabel);
	bool            IsIdVaild(){ return bIsIdVaild; }
	
	void			SetStrIDInfo(CTemplateBase* pTemplate, int idx);
	CString			GetStrID();
	CTemplateBase*	GetTemplate() {return pTemp;}

	bool			ChangeLmtSize(int nNewLimit);
protected:
	void            Release();
	
	TCHAR *pOriginalText; //具体对象具体定义
	TCHAR *pTranslationText;//具体对象具体定义
	TCHAR szTextName[32];//本条文字的名字
	TCHAR szLabel[32];//文本标签
	int   nStatus;	//状态标记
	int   nLimit;   //字符上限个数
	bool  bMuiltiLine; //该文本是否支持多行
	bool  bIsIdVaild;
	CTemplateBase* pTemp;

	int   index;
};


class CText_Normal : public CTextBase  
{

public:
	CText_Normal();
	virtual ~CText_Normal();

public:
	virtual bool    CheckSyntax(CString strDest, int& start, int& len, bool bMsg = true);
	virtual CTextBase* Clone();
};


class CText_TalkWindow : public CTextBase  
{

public:
	CText_TalkWindow();
	virtual ~CText_TalkWindow();

public:
	virtual bool	Load(FILE *pFile, DWORD dwVersion);
	virtual bool	Save(FILE *pFile);
	virtual void    Init(bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName);
	virtual bool    CheckSyntax(CString strDest, int& start, int& len, bool bMsg = true);
	virtual CTextBase* Clone();
	virtual bool    ReplaceOriginalText(const TCHAR* pNewText);
	virtual bool    ReplaceTransText(const TCHAR *pNewText);

	virtual int GetID(){ return id; }
	virtual void SetID( int uid){ id = uid; }
protected:
	unsigned int id;
};


class CText_TalkOption : public CText_Normal
{
public:
	CText_TalkOption();
	virtual ~CText_TalkOption();
public:
	virtual int GetID(){ return id; }
	virtual void SetID( int uid){ id = uid; }
	virtual int GetParam(){ return par; }
	virtual void SetParam( int upar){ par = upar; }

	virtual CTextBase* Clone();
	virtual bool	Load(FILE *pFile, DWORD dwVersion);
	virtual bool	Save(FILE *pFile);
protected:
	int id;
	int par;
};


class CText_Resize : public CTextBase  
{

public:
	CText_Resize();
	virtual ~CText_Resize();

public:
	virtual bool	Load(FILE *pFile, DWORD dwVersion);
	virtual bool	Save(FILE *pFile);
	virtual void    Init(bool bMuil, TCHAR *pOrgText, TCHAR *pTransText, TCHAR *pTextName);
	virtual bool    CheckSyntax(CString strDest, int& start, int& len, bool bMsg = true);
	virtual bool    ReplaceOriginalText(const TCHAR* pNewText);
	virtual bool    ReplaceTransText(const TCHAR *pNewText);
	virtual CTextBase* Clone();
	void			SetMaxLimit(int nMaxLimit) { m_nMaxLimit = nMaxLimit; }
protected:
	int				m_nMaxLimit;
};

class CText_ScriptFile : public CText_Resize
{
public:
	virtual bool    CheckSyntax(CString strDest, int& start, int& len, bool bMsg = true);
};

#endif // !defined(AFX_TEXTBASE_H__02E36E51_AAB3_49AF_943B_CC3EC5EF5390__INCLUDED_)
