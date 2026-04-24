// GeneralDataUIWrapper.cpp: implementation of the CGeneralDataUIWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementdata.h"
#include "GeneralDataUIWrapper.h"
#include "EnumTypes.h"
#include "Global.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//=============================================================================
#include "BaseDataTemplate.h"
#include "TemplIDSelDlg.h"

//=============================================================================
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeneralDataUIWrapper::CGeneralDataUIWrapper( BaseDataTempl* pDataItemTempl ) : 
m_pDataItemTempl( pDataItemTempl ), m_nPropertyFlag( WAY_DEFAULT )
{
	//创建并初始化动态属性对象
	m_pProperty = new ADynPropertyObject;
	if ( !m_pProperty )
	{
		return;
	}
	//根据数据模板建立属性列表
	BuildProperty();
}

CGeneralDataUIWrapper::~CGeneralDataUIWrapper()
{
	Release();
}

void CGeneralDataUIWrapper::BuildProperty()
{
	//判前提
	if ( !m_pDataItemTempl || !m_pProperty )
	{
		return;
	}
	
	//在列表中添加模板ID项
	AString strName = m_pDataItemTempl->GetName();
	m_pProperty->DynAddProperty( AVariant(strName), "模板名称", NULL, NULL, m_nPropertyFlag );


	SetDefaultProperty();//设置模板中的默认值
	int nDataItemTemplCount = m_pDataItemTempl->GetItemNum();
	for ( int i = 0; i < nDataItemTemplCount; ++i )
	{
		AString itemType = m_pDataItemTempl->GetItemType(i);
		AString itemName = m_pDataItemTempl->GetItemName(i);
		AVariant itemVar = m_pDataItemTempl->GetItemValue(i);
		AddProperty( itemType, itemVar, itemName );
	}
}


void CGeneralDataUIWrapper::SetDefaultProperty()
{
	int nDataItemTemplCount = m_pDataItemTempl->GetItemNum();
	for ( int i = 0; i < nDataItemTemplCount; ++i )
	{	
		AString itemType = m_pDataItemTempl->GetItemType(i);
		AString itemName = m_pDataItemTempl->GetItemName(i);
		AVariant itemVar = m_pDataItemTempl->GetItemValue(i);
		if( AVariant::AVT_INVALIDTYPE == itemVar.GetType() )
		{
			AVariant var;
			if ( stricmp(itemType,"int")==0 )
			{
				int x = 0;
				var = AVariant(x);
			}
			else if ( stricmp(itemType,"int64")==0 )
			{
				__int64 x = 0;
				var = AVariant(x);
			}
			else if ( stricmp(itemType, "float")==0 )
			{
				float x = 0.0f;
				var = AVariant(x);
			}
			else if ( stricmp(itemType, "double")==0 )
			{
				double x = 0.0;
				var = AVariant(x);
			}
			else if ( stricmp(itemType, "uint")==0 )
			{
				unsigned int x = 0;
				var = AVariant(x);
			}
			else
			{
				int x = 0;
				var = AVariant(x);
			}
			m_pDataItemTempl->SetItemValue( i, var );
		}
	}
}


void CGeneralDataUIWrapper::AddProperty( AString type, AVariant var, AString name )
{
	if ( stricmp(type, "int")==0 )
	{
		m_pProperty->DynAddProperty(AVariant((int)var), name, NULL, NULL, m_nPropertyFlag);
	}
	else if ( stricmp(type, "int64")==0 )
	{
		m_pProperty->DynAddProperty(AVariant((__int64)var), name, NULL, NULL, m_nPropertyFlag);
	}
	else if ( stricmp(type,"float")==0 )
	{
		m_pProperty->DynAddProperty(AVariant((float)var), name, NULL, NULL, m_nPropertyFlag);
	}
	else if ( stricmp(type,"double")==0  )
	{
		m_pProperty->DynAddProperty(AVariant((double)var), name, NULL, NULL, m_nPropertyFlag);
	}
	else if ( stricmp(type,"uint")==0  )
	{
		m_pProperty->DynAddProperty(AVariant((unsigned int)var), name, NULL, NULL, m_nPropertyFlag);
	}
	else if ( stricmp(type,"dummy_int")==0 )
	{
		m_pProperty->DynAddProperty(AVariant((int)var), name, NULL, NULL, WAY_READONLY | m_nPropertyFlag);
	}
	else
	{
		ENUM_TYPE* pEnumType = g_EnumTypes.GetType(AString(type));
		if ( pEnumType )
		{
			ASetTemplate<int> *pNewType = new ASetTemplate<int>;
			if ( !pNewType )
			{
				AfxMessageBox( "初始化失败！" );
				return;
			}
			for ( int i = 0; i < pEnumType->dwItemNum; ++i )
			{
				pNewType->AddElement( pEnumType->listItemName[i], i );
			}

			m_listEnumTypes.AddTail( pNewType );

			m_pProperty->DynAddProperty(var, name, pNewType, NULL, m_nPropertyFlag);
		}
		else
		{
			AddCustomProperty( type, var, name );
		}
	}

}

void CGeneralDataUIWrapper::AddCustomProperty( AString type, AVariant var, AString name )
{
	if ( stricmp(type,"item_id") == 0 )
	{
		CTemplIDSel* pTemplIDSel = new CTemplIDSel;
		if ( !pTemplIDSel )
		{
			AfxMessageBox( "初始化失败！" );
			return;
		}
		pTemplIDSel->Init( "BaseData", ".tmpl", var, enumBaseID );
		m_listCustoms.AddTail( pTemplIDSel );

		m_pProperty->DynAddProperty( var, name, (ASet*)pTemplIDSel->GetFuncsPtr(), NULL, WAY_CUSTOM | m_nPropertyFlag );
	}

}

void CGeneralDataUIWrapper::Release()
{
	if ( m_pProperty )
	{
		delete m_pProperty;
		m_pProperty = NULL;
	}

	POSITION pos = m_listCustoms.GetHeadPosition();
	while ( pos )
	{
		delete (CUSTOM_FUNCS*)m_listCustoms.GetNext(pos);
	}

	pos = m_listEnumTypes.GetHeadPosition();
	while ( pos )
	{
		delete (ASetTemplate<int>*)m_listEnumTypes.GetNext( pos );
	}
}

void CGeneralDataUIWrapper::UpdateProperty( bool bGetData )
{
	int nDataItemCount = m_pDataItemTempl->GetItemNum();
	
	if ( bGetData )
	{
		//存储物品模板名称
		m_pDataItemTempl->SetName( m_pProperty->GetPropVal(0) );

		//存储通用数据项
		for ( int i =0; i < nDataItemCount; ++i )
		{
			AVariant var = m_pProperty->GetPropVal(i+1);	//!!!!!!注意!!!!!!因为之前在m_pProperty中插入了一项“模板ID”
			m_pDataItemTempl->SetItemValue( i, var );
			
			//-------------------------------------------------------
			//!!!!!!特殊处理!!!!!!新需求希望：模板中有些数据项仅仅是用来查看的，而不能修改，因此
			//在程序中dummy项的值由它之后的一项的值进行赋值。
			AString strType = m_pDataItemTempl->GetItemType(i);
			if ( stricmp(strType,"dummy_int")==0 )
			{
				m_pDataItemTempl->SetItemValue( i, m_pProperty->GetPropVal(i+2) );
			}
			//-------------------------------------------------------
		}
	}
	else
	{
		//更新模板名称
		AString strName = m_pDataItemTempl->GetName();
		m_pProperty->SetPropVal( 0, AVariant(strName) );

		//更新通用项
		for ( int i = 0; i < nDataItemCount; ++i )
		{
			m_pProperty->SetPropVal( i+1, m_pDataItemTempl->GetItemValue(i) );
		}
	}
}