// GeneralDataMan.h: interface for the CGeneralDataMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALDATAMAN_H__B48691AB_EC77_4479_BD4A_EAA2E0F4DF5D__INCLUDED_)
#define AFX_GENERALDATAMAN_H__B48691AB_EC77_4479_BD4A_EAA2E0F4DF5D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//=============================================================================
#include "BaseDataTemplate.h"
#include "GeneralDataManUtil.h"

//=============================================================================
class BaseDataIDManBase;
class AFile;
//=============================================================================
struct GENERAL_DATA_SUB_TYPE
{
	unsigned int		id;
	char				szName[nStrNameLen];
	abase::vector<int>	listSubTypeItemID;
};

struct GENERAL_DATA_MAIN_TYPE
{
	unsigned int	id;
	char			szName[nStrNameLen];
	abase::vector<GENERAL_DATA_SUB_TYPE> listSubType;
};
//=============================================================================
class CGeneralDataMan  
{
public:
	CGeneralDataMan(const char* szMainTypes[], int nMainTypesCount, const char* szTemplIDFileName, CString strOrgPath, GENERAL_DATA_TEMPL_TYPE_ID nDataTemplTypeID = GENERAL_DATA_TEMPL_TYPE_DEFAULT);
	virtual ~CGeneralDataMan();
	bool Load( const char* szFileName );
	bool Save( const char* szFileName );
	bool Export( const char* szFileName );
	bool IsChanged( void ){ return m_bChanged; };
	void Release();
	void ClearAllDataTemplItems( void );
	bool AddDataItem( BaseDataTempl* pDataItemTempl, int nMainID = 0, int nSubID = 0 );
	bool RemoveDataItem( int nDataTemplID, int nMainID = 0, int nSubID = 0 );
	bool RemoveSubTypeAndDataItem( int nMainID = 0, int nSubID = 0 );
	BaseDataTempl* GetDataItemTempl( int index )
	{ 
		if ( index < 0 || index >= m_vDataTemplList.size() )
		{
			return NULL;
		}
		return m_vDataTemplList[index]; 
	};
	BaseDataTempl* GetDataItemTemplByID( int tid )
	{
		for ( int i = 0; i < m_vDataTemplList.size(); ++i )
		{
			if ( m_vDataTemplList[i]->GetID() == tid )
			{
				return m_vDataTemplList[i];
			}
		}

		//
		return NULL;
	}

	int GetDataItemTemplCount(){ return m_vDataTemplList.size(); };

	abase::vector<GENERAL_DATA_MAIN_TYPE*>* GetDataManMainTypeList()
	{
		return &m_listMainType;
	}

	int GenSubID()
	{
		return ++m_nCurMaxSubID;
	}

	GENERAL_DATA_MAIN_TYPE* GetMainType( int nMainTypeID )
	{
		int nMainTypeCount = m_listMainType.size();
		for ( int i = 0; i < nMainTypeCount; ++i )
		{
			if ( nMainTypeID == m_listMainType[i]->id )
			{
				return m_listMainType[i];
			}
		}
		//
		return NULL;
	}

	inline bool SetSubTypeName( int nMainTypeID, int nSubTypeID, const char* szName )
	{
		int nMainTypeCount = m_listMainType.size();
		for ( int i = 0; i < nMainTypeCount; ++i )
		{
			if ( nMainTypeID == m_listMainType[i]->id )
			{
				for ( int j = 0; j < m_listMainType[i]->listSubType.size(); ++j )
				{
					if ( nSubTypeID == m_listMainType[i]->listSubType[j].id )
					{
						//判断字符串长度
						if ( strlen(szName) >= nStrNameLen )
						{
							return false;
						}
						strcpy( m_listMainType[i]->listSubType[j].szName,szName );
						return true;
					}
				}
			}
		}

		return false;
	}

	inline bool SwapSubType( int nMainID, int nSubID1, int nSubID2 )
	{
		if ( nMainID < 0 || nSubID1 < 0 || nSubID2 < 0 )
		{
			return false;
		}

		//寻找到相应主类型下的两个子类型，并进行交换
		int nIndex1 = -1, nIndex2 = -1;
		for ( int i = 0; i < m_listMainType.size(); ++i )
		{
			if ( nMainID == m_listMainType[i]->id )
			{
				int nSubTypeCount = m_listMainType[i]->listSubType.size();
				for ( int j = 0; j < nSubTypeCount; ++j )
				{
					if ( nSubID1 == m_listMainType[i]->listSubType[j].id )
					{
						nIndex1 = j;
					}

					if ( nSubID2 == m_listMainType[i]->listSubType[j].id )
					{
						nIndex2 = j;
					}
				}

				if ( nIndex1 < 0 || nIndex2 < 0 )
				{
					return false;
				}

				//swap the sub type
				GENERAL_DATA_SUB_TYPE subType = m_listMainType[i]->listSubType[nIndex1];
				m_listMainType[i]->listSubType[nIndex1] = m_listMainType[i]->listSubType[nIndex2];
				m_listMainType[i]->listSubType[nIndex2] = subType;
				
				return true;
			}
		}

		//
		return false;
	}

	inline bool SwapItemPosByID( int nMainID, int nSubID, int nItemID1, int nItemID2 )
	{
		//判断参数的合法性
		if ( nMainID < 0 || nSubID < 0 || nItemID1 <= 0 || nItemID2 <= 0 || (nItemID1 == nItemID2) )
		{
			return false;
		}

		int nDataTemplCount = m_vDataTemplList.size();

		int nIndex1 = -1, nIndex2 = -1;
		for ( int i = 0; i < nDataTemplCount; ++i )
		{
			if ( nItemID1 == m_vDataTemplList[i]->GetID() )
			{
				nIndex1 = i;
			}

			if ( nItemID2 == m_vDataTemplList[i]->GetID() )
			{
				nIndex2 = i;
			}
		}

		//是否寻找到需要交换位置的两个物品？
		if ( nIndex1 < 0 || nIndex2 < 0 )
		{
			return false;
		}

		//交换物品模板在列表中的位置
		BaseDataTempl* pDataTempl = m_vDataTemplList[nIndex1];
		m_vDataTemplList[nIndex1] = m_vDataTemplList[nIndex2];
		m_vDataTemplList[nIndex2] = pDataTempl;

		//交换这两个物品在子类中的ID位置
		for ( int j = 0; j < m_listMainType.size(); ++j )
		{
			if ( nMainID == m_listMainType[j]->id )
			{
				for ( int k = 0; k < m_listMainType[j]->listSubType.size(); ++k )
				{
					if ( nSubID == m_listMainType[j]->listSubType[k].id )
					{
						int nItemIDIndex1 = FindPos( m_listMainType[j]->listSubType[k].listSubTypeItemID, nItemID1 );
						int nItemIDIndex2 = FindPos( m_listMainType[j]->listSubType[k].listSubTypeItemID, nItemID2 );
						if ( nItemIDIndex1 < 0 || nItemIDIndex2 < 0 )
						{
							return false;
						}
						int nItemIDTmpl = m_listMainType[j]->listSubType[k].listSubTypeItemID[nItemIDIndex1];
						m_listMainType[j]->listSubType[k].listSubTypeItemID[nItemIDIndex1] = m_listMainType[j]->listSubType[k].listSubTypeItemID[nItemIDIndex2];
						m_listMainType[j]->listSubType[k].listSubTypeItemID[nItemIDIndex2] = nItemIDTmpl;
					}
				}
			}
		}
		return true;
	}

	inline bool MoveItemToLastPosByID( int nItemID )
	{
		if ( nItemID <= 0 )
		{
			return false;
		}
		
		int nIndex = -1;
		for ( int i = 0; i < m_vDataTemplList.size(); ++i )
		{
			if ( nItemID == m_vDataTemplList[i]->GetID() )
			{
				nIndex = i;
				break;
			}
		}
		if ( (nIndex >= 0) && (nIndex < (m_vDataTemplList.size() - 1)) )
		{
			BaseDataTempl* pDataTempl = m_vDataTemplList[nIndex];
			m_vDataTemplList.erase( &m_vDataTemplList[nIndex] );
			m_vDataTemplList.push_back(pDataTempl);
		}
		else
		{
			return false;
		}

		//
		return true;
	}

	inline bool AddMainAndSubTypeID( int nDataItemID, int nMainID, int nSubID )
	{
		//存入到SubType的模板ID列表中
		for ( int i = 0; i < m_listMainType.size(); ++i )
		{
			if ( m_listMainType[i]->id == nMainID )
			{
				for ( int j = 0; j < m_listMainType[i]->listSubType.size(); ++j )
				{
					if ( m_listMainType[i]->listSubType[j].id == nSubID )
					{
						m_listMainType[i]->listSubType[j].listSubTypeItemID.push_back( nDataItemID );
						return true;
					}
				}
			}
		}
		
		return false;
	}

	inline bool RemoveMainAndSubTypeID( int nDataItemID, int nMainID, int nSubID )
	{
		//删除类型表中对应的模板ID
		for ( int i = 0; i < m_listMainType.size(); ++i )
		{
			if ( m_listMainType[i]->id == nMainID )
			{
				for ( int j = 0; j < m_listMainType[i]->listSubType.size(); ++j )
				{
					if ( m_listMainType[i]->listSubType[j].id == nSubID )
					{
						//abase::vector<int>& listSubTypeID = m_listMainType[i]->listSubType[j].listSubTypeItemID;
						int nListIndex = FindPos( m_listMainType[i]->listSubType[j].listSubTypeItemID, nDataItemID );
						if ( nListIndex >= 0 )
						{
							m_listMainType[i]->listSubType[j].listSubTypeItemID.erase( &(m_listMainType[i]->listSubType[j].listSubTypeItemID[nListIndex]) );
						}
						return true;
					}
				}
			}
		}
		
		return false;
	}

	bool AddGeneralDataSubType( const char* szSubTypeName, int nMainTypeID, int nSubTypeID )
	{
		//判断输入参数合法性
		if ( !szSubTypeName || (strlen(szSubTypeName) > nStrNameLen) || nMainTypeID < 0 || nSubTypeID < 0 )
		{
			return false;
		}

		//
		int nMainTypeCount = m_listMainType.size();
		for ( int i = 0; i < nMainTypeCount; ++i )
		{
			if ( nMainTypeID == m_listMainType[i]->id )
			{
				GENERAL_DATA_SUB_TYPE subType;
				subType.id = nSubTypeID;
				strcpy( subType.szName, szSubTypeName );
				m_listMainType[i]->listSubType.push_back( subType );
			}
		}

		//
		return true;
	}

	BaseDataTempl* CreateDefaultDataItemTempl( CString strOrgPath, bool bIsNewID = false );

	//导出所有item模板属性Added 2012-02-07.
	bool ExportAllItemAttribute( const char* szFileName );

//---------------------------
protected:
	struct OrgItem
	{
		AString		m_strName;
		AString		m_strType;
	};
//---------------------------

protected:
	bool InitGeneralDataIDMan( const char* szDataIDManFileName );
	bool SaveDataItemFormat( AFile* file );
	bool SaveBaseDataTemplValue( AFile* file );
	bool SaveDataTypes( AFile* file );
	bool LoadDataItemFormat( AFile* file, abase::vector<OrgItem>& listOrgItem );
	bool LoadBaseDataTemplValue( AFile* file, int nDataTemplCount, abase::vector<OrgItem>& listOrgItem );
	bool LoadDataTypes( AFile* file );

protected:
	inline int FindPos( abase::vector<int>& listTypeID, int nValue )
	{
		int nListTypeIDCount = listTypeID.size();
		for ( int i = 0; i < nListTypeIDCount; ++i )
		{
			if ( listTypeID[i] == nValue )
			{
				return i;
			}
		}

		return -1;
	}

protected:
	abase::vector<BaseDataTempl*>				m_vDataTemplList;
	abase::vector<GENERAL_DATA_MAIN_TYPE*>		m_listMainType;
	bool										m_bChanged;
	int											m_nCurMaxSubID;
	BaseDataIDManBase*							m_pGeneralDataIDMan;
	CString										m_strOrgPath;
	GENERAL_DATA_TEMPL_TYPE_ID					m_nDataTemplTypeID;
};

#endif // !defined(AFX_GENERALDATAMAN_H__B48691AB_EC77_4479_BD4A_EAA2E0F4DF5D__INCLUDED_)
