#include "EPStdAfx.h"
#include <resource.h>
#include <io.h>
#include <CommonFileName.h>
#include <A3DEngine.h>
#include <hashmap.h>

#include "TextureProperty.h"
//#include "DlgVectorEditor.h"
//#include ".\Render.h"
//#include "DlgLightAttenuation.h"

extern abase::hash_map<A3DFXUIParam*, AString> g_mPt2Path;
//////////////////////////////////////////////////////////////////////////
TextureProperty::TextureProperty(A3DEngine* pA3DEngine)
	: m_pA3DEngine(pA3DEngine)
{
}
BOOL CALLBACK TextureProperty::OnActivate(void)
{
	if(0 == _access("SkeletonEditor22_d.dir", 0))
	{
		CFile::Remove("SkeletonEditor22_d.dir");
	}
	else
	if (0 == _access("SkeletonEditor22.dir", 0))
	{
		CFile::Remove("SkeletonEditor22.dir");
	}

	CString csAllRootDir = "Shaders\\Textures|"+m_csRootDir;

	CCommonFileName FileDlg(m_pA3DEngine, AFILE_NOALLOWMULFILES, csAllRootDir, "", AfxGetMainWnd());
	FileDlg.SetFileExtension("dds tga bmp jpg atx");

	if (FileDlg.DoModal() == IDOK)
	{
		CString csFullPath = FileDlg.GetFullFileName();
		int nSeperator = csFullPath.ReverseFind('\\');
		CString csPath = csFullPath.Mid(0, nSeperator) + '\\';
		CString csFileName = csFullPath.Mid(nSeperator+1, csFullPath.GetLength());

		//m_pFxUIParam->m_strObjectPath = csFileName;
		//m_pFxUIParam->SetTexture(csFullPath);
		m_csText = csFullPath;
	}
	return TRUE;
}
LPCTSTR CALLBACK TextureProperty::OnGetShowString(void) const
{
	return m_csText;
}
AVariant CALLBACK TextureProperty::OnGetValue(void) const
{
	return m_var;
}
void CALLBACK TextureProperty::OnSetValue(const AVariant& var)
{
	m_var = var;
}

const char* TextureProperty::GetFilePath() const
{
	return m_csText.GetBuffer(m_csText.GetLength());
}

//////////////////////////////////////////////////////////////////////////
VectorXProperty::~VectorXProperty()
{
	if (m_pBytes)
	{
		delete []m_pBytes;
		m_pBytes = 0;
	}
}
BOOL CALLBACK VectorXProperty::OnActivate(void)
{	
	CDlgVectorEditor dlg(this, m_pParentWnd);
	dlg.DoModal();
	return TRUE;
}
LPCTSTR CALLBACK VectorXProperty::OnGetShowString(void) const
{
	return m_csText;
}
AVariant CALLBACK VectorXProperty::OnGetValue(void) const
{
	return m_var;
}
void CALLBACK VectorXProperty::OnSetValue(const AVariant& var)
{
	m_var = var;
}

void VectorXProperty::GetDisplay()
{
	int nSize = 0;
	if (m_nType == VECTORX_INT)
	{
		nSize = m_nBytes/sizeof(int);
		switch(nSize)
		{
		case 2:
			m_csText.Format("X: %d, Y: %d", ((int*)m_pBytes)[0], ((int*)m_pBytes)[1]);
			break;
		case 3:
			m_csText.Format("X: %d, Y: %d, Z: %d", ((int*)m_pBytes)[0], ((int*)m_pBytes)[1], ((int*)m_pBytes)[2]);
			break;
		case 4:
			m_csText.Format("X: %d, Y: %d, Z: %d, W: %d", ((int*)m_pBytes)[0], ((int*)m_pBytes)[1], ((int*)m_pBytes)[2], ((int*)m_pBytes)[3]);
			break;
		}
	}
	else
	if (m_nType == VECTORX_FLOAT)
	{
		nSize = m_nBytes/sizeof(float);
		switch(nSize)
		{
		case 2:
			m_csText.Format("X: %.3f, Y: %.3f", ((float*)m_pBytes)[0], ((float*)m_pBytes)[1]);
			break;
		case 3:
			m_csText.Format("X: %.3f, Y: %.3f, Z: %.3f", ((float*)m_pBytes)[0], ((float*)m_pBytes)[1], ((float*)m_pBytes)[2]);
			break;
		case 4:
			m_csText.Format("X: %.3f, Y: %.3f, Z: %.3f, W: %.3f", ((float*)m_pBytes)[0], ((float*)m_pBytes)[1], ((float*)m_pBytes)[2], ((float*)m_pBytes)[3]);
			break;
		}
	}
}
////////////////////////////////////////////////////////////////////////////

//BOOL CALLBACK AttenuationProperty::OnActivate(void)
//{
//	CDlgLightAttenuation dlg(m_fAttenuation0, m_fAttenuation1, m_fRange, m_dwDiffuse, m_dwAmbient);
//	if(IDOK != dlg.DoModal())
//		return FALSE;
//	m_fAttenuation0 = dlg.m_fAttenuation1;
//	m_fAttenuation1 = dlg.m_fAttenuation2;
//	m_csText.Format("¡¡∂»:%.3f;À•ºı:%.3f", m_fAttenuation0, m_fAttenuation1);
//	return TRUE;
//}
//LPCTSTR CALLBACK AttenuationProperty::OnGetShowString(void) const
//{	
//	return m_csText;
//}
//AVariant CALLBACK AttenuationProperty::OnGetValue(void) const
//{
//	return m_var;
//}
//void CALLBACK AttenuationProperty::OnSetValue(const AVariant& var)
//{
//	m_var = var;
//}
//
//void AttenuationProperty::GetDisplay()
//{
//	m_csText.Format("¡¡∂»:%.3f;À•ºı:%.3f", m_fAttenuation0, m_fAttenuation1);
//}
