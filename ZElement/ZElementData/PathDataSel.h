// PathDataMan.h: interface for the PathDataMan class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _PATH_DATA_SEL_H_
#define _PATH_DATA_SEL_H_

#include "AObject.h"

struct SEL_PATH_FUNCS : public CUSTOM_FUNCS
{
	~SEL_PATH_FUNCS() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);

	CString		m_strRootDir;
	CString		m_strExt;
	AVariant	m_var;
	CString		m_strText;
	void Resume() { m_strText.Empty(); }
};

class CPathSel
{
public:
	CPathSel() {}
	virtual ~CPathSel() {}

protected:
	SEL_PATH_FUNCS m_PathSelFuncs;

public:
	void SetRootPath(LPCTSTR szRoot) { m_PathSelFuncs.m_strRootDir = szRoot; }
	void SetFileExt(LPCTSTR szExt) { m_PathSelFuncs.m_strExt = szExt; }
	void SetValue(const AVariant& var) { m_PathSelFuncs.OnSetValue(var); }
	void* GetFuncsPtr() { return (void*)&m_PathSelFuncs; }
	void Init(
		LPCTSTR szRoot,
		LPCTSTR szExt,
		const AVariant& var)
	{
		m_PathSelFuncs.Resume();
		SetRootPath(szRoot);
		SetFileExt(szExt);
		SetValue(var);
	}
};


#endif // !defined(AFX_PATHDATAMAN_H__C036BA2D_8F3E_461D_B106_AE16C3F7E774__INCLUDED_)
