#pragma once
#include <propdlg\AObject.h>
#include <propdlg\PropertyList.h>
#include "DlgVectorEditor.h"

class A3DFXUIParam;
class A3DEngine;



struct TextureProperty : public CUSTOM_FUNCS 
{
	TextureProperty(A3DEngine* pA3DEngine);
	virtual ~TextureProperty() {}
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	const char* GetFilePath() const;

	AVariant	m_var;
	mutable CString m_csText;
	CString m_csRootDir;
private:
	A3DEngine* m_pA3DEngine;
};

struct VectorXProperty : public CUSTOM_FUNCS 
{
	enum
	{
		VECTORX_NONE,
		VECTORX_INT,
		VECTORX_FLOAT
	};	
	virtual ~VectorXProperty();
	virtual BOOL CALLBACK OnActivate(void);
	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
	virtual AVariant CALLBACK OnGetValue(void) const;
	virtual void CALLBACK OnSetValue(const AVariant& var);
	void GetDisplay();
	BYTE* GetData() const { return m_pBytes; }
	int GetDataLength() const { return m_nBytes; }

	AVariant	m_var;
	mutable CString m_csText;
	int m_nType;
	BYTE* m_pBytes;
	int m_nBytes;
	CWnd* m_pParentWnd;
};

//BOOL CALLBACK VectorXProperty::OnActivate(void)
//{	
//	CDlgVectorEditor dlg(this, m_pParentWnd);
//	dlg.DoModal();
//	return TRUE;
//}

//struct AttenuationProperty : public CUSTOM_FUNCS 
//{
//	virtual ~AttenuationProperty() {}
//	virtual BOOL CALLBACK OnActivate(void);
//	virtual LPCTSTR CALLBACK OnGetShowString(void) const;
//	virtual AVariant CALLBACK OnGetValue(void) const;
//	virtual void CALLBACK OnSetValue(const AVariant& var);
//	void GetAttenuation(float att0, float att1) const;
//	void GetDisplay();
//
//	AVariant	m_var;
//	mutable CString m_csText;
//	float	m_fAttenuation0;
//	float	m_fAttenuation1;
//	float	m_fRange;
//	DWORD	m_dwDiffuse;
//	DWORD	m_dwAmbient;
//};