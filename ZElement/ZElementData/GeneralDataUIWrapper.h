// GeneralDataUIWrapper.h: interface for the CGeneralDataUIWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALDATAUIWRAPPER_H__0B9722F5_1063_4E0B_AB4D_9BB7C4CC84DA__INCLUDED_)
#define AFX_GENERALDATAUIWRAPPER_H__0B9722F5_1063_4E0B_AB4D_9BB7C4CC84DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//=============================================================================
#include "AObject.h"

//-----------------------------------------------------------------------------
class ADynPropertyObject;
class BaseDataTempl;
//=============================================================================
class CGeneralDataUIWrapper  
{
public:
	CGeneralDataUIWrapper( BaseDataTempl* pDataItemTempl );
	virtual ~CGeneralDataUIWrapper();
	void Release();
	void UpdateProperty( bool bGetData );
	ADynPropertyObject* GetDynPropertyObject(){ return m_pProperty; }

protected:
	ADynPropertyObject*		m_pProperty;
	BaseDataTempl*			m_pDataItemTempl;
	int						m_nPropertyFlag;
	CPtrList				m_listCustoms;
	CPtrList				m_listEnumTypes;
protected:
	void BuildProperty();
	void SetDefaultProperty();
	void AddProperty( AString type, AVariant var, AString name );
	void AddCustomProperty( AString type, AVariant var, AString name );
};

#endif // !defined(AFX_GENERALDATAUIWRAPPER_H__0B9722F5_1063_4E0B_AB4D_9BB7C4CC84DA__INCLUDED_)
