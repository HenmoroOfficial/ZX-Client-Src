// PathDataMan.cpp: implementation of the PathDataMan class.
//
//////////////////////////////////////////////////////////////////////
#include "StdAfx.h"
#include "PathDataSel.h"
#include "BaseDataIDMan.h"
#include "Global.h"
#include "Render.h"
#include "CommonFileName.h"

BOOL CALLBACK SEL_PATH_FUNCS::OnActivate(void)
{
	CString strFilter = m_strExt;

	CCommonFileName FileDlg(g_Render.GetA3DEngine(), AFILE_NOALLOWMULFILES,m_strRootDir);
	if (FileDlg.DoModal() != IDOK)
		return FALSE;
	CString strFileName = FileDlg.GetFullFileName();
	if(!strFileName.IsEmpty())
	{
		unsigned long id = g_PathIDMan.GetIDByPath(AString(strFileName));
		if(0==id)
		{
			id = g_PathIDMan.CreateID(AString(strFileName));
			if(0==id)
			{
				m_strText = "";
				m_var = 0;
			}
		}
		m_strText = strFileName;
		m_var = id;
	}else
	{
		m_strText = "";
		m_var = 0;
	}
	return TRUE;
}

LPCTSTR CALLBACK SEL_PATH_FUNCS::OnGetShowString(void) const
{
	return m_strText;
}

AVariant CALLBACK SEL_PATH_FUNCS::OnGetValue(void) const
{
	return m_var;
}

void CALLBACK SEL_PATH_FUNCS::OnSetValue(const AVariant& var)
{
	m_var = var;
	unsigned long ulID = unsigned long(m_var);
	m_strText = g_PathIDMan.GetPathByID(ulID);
}
