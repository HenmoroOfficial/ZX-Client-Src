// GeneralDataMan.cpp: implementation of the CGeneralDataMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementdata.h"
#include "GeneralDataMan.h"
#include "BaseDataIDMan.h"
#include "AFile.h"
#include "AVariant.h"
#include "GeneralDataExporter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//=============================================================================
static const char _format_item_name[]		= "模板名称：%s";
static const char _format_item_id[]			= "模板ID：%d";
static const char _format_item_treasure_id[]= "物品ID：%d";
static const char _format_item_sub_type[]	= "物品类别：%s";

//=============================================================================
CGeneralDataMan::CGeneralDataMan(const char* szMainTypes[], int nMainTypesCount, const char* szTemplIDFileName, CString strOrgPath, GENERAL_DATA_TEMPL_TYPE_ID nDataTemplTypeID) : m_bChanged(false),
m_nCurMaxSubID(0), m_pGeneralDataIDMan(NULL), m_strOrgPath(strOrgPath), m_nDataTemplTypeID( nDataTemplTypeID )
{
	if ( !szMainTypes || !nMainTypesCount )
	{
		return;
	}

	//初始化主类型
	for ( int i = 0; i < nMainTypesCount; ++i )
	{
		GENERAL_DATA_MAIN_TYPE* pMainType = new GENERAL_DATA_MAIN_TYPE;
		pMainType->id = i;
		sprintf(pMainType->szName, szMainTypes[i]);
		m_listMainType.push_back( pMainType );
	}

	//初始化模板ID管理器
	InitGeneralDataIDMan( szTemplIDFileName );
}

CGeneralDataMan::~CGeneralDataMan()
{
	Release();
}

void CGeneralDataMan::Release()
{
	int nMainTypesCount = m_listMainType.size();
	for ( int i = 0; i < nMainTypesCount; ++i )
	{
		delete m_listMainType[i];
	}

	int nDataTemplCount = m_vDataTemplList.size();
	for ( int j = 0; j < nDataTemplCount; ++j )
	{
		delete m_vDataTemplList[j];
	}
	m_vDataTemplList.clear();

	if ( m_pGeneralDataIDMan )
	{
		m_pGeneralDataIDMan->Release();
		delete m_pGeneralDataIDMan;
		m_pGeneralDataIDMan = NULL;
	}
	
}

void CGeneralDataMan::ClearAllDataTemplItems( void )
{
	int nDataTemplCount = m_vDataTemplList.size();
	for ( int j = 0; j < nDataTemplCount; ++j )
	{
		delete m_vDataTemplList[j];
	}
	m_vDataTemplList.clear();

	int nMainTypesCount = m_listMainType.size();
	for ( int i = 0; i < nMainTypesCount; ++i )
	{
		m_listMainType[i]->listSubType.clear();
	}
	
}

bool CGeneralDataMan::Load( const char* szFileName )
{
	AFile aFile;
	if ( !aFile.Open( szFileName, AFILE_OPENEXIST | AFILE_BINARY ) )
	{
		AfxMessageBox( "无法读取文件！" );
		return false;
	}

	DWORD dwReadLength = 0;
	DWORD dwVersion = 0;
	aFile.Read( &dwVersion, sizeof(DWORD), &dwReadLength );
	int nDataTemplCount = 0;
	aFile.Read( &nDataTemplCount, sizeof(int), &dwReadLength );
	if ( nDataTemplCount )
	{
		abase::vector<OrgItem> listOrgItem;
		listOrgItem.clear();
		LoadDataItemFormat( &aFile, listOrgItem );
		LoadBaseDataTemplValue( &aFile, nDataTemplCount, listOrgItem );
	}
	LoadDataTypes( &aFile );

	aFile.Close();
	//
	return true;
}

bool CGeneralDataMan::LoadDataItemFormat( AFile* file, abase::vector<OrgItem>& listOrgItem )
{
	DWORD dwReadLength = 0;
	int nDataItemCount = 0;
	file->Read( &nDataItemCount, sizeof(int), &dwReadLength );
	const int nStrMaxLen = 1024;
	char szStrTemp[nStrMaxLen];
	for ( size_t i = 0; i < nDataItemCount; ++i )
	{
		OrgItem orgItem;
		memset( szStrTemp, '\0', nStrMaxLen );
		int nNameLen = 0;
		file->Read( &nNameLen, sizeof(int), &dwReadLength );
		file->Read( szStrTemp, sizeof(char)*nNameLen, &dwReadLength );
		orgItem.m_strName = szStrTemp;
		int nTypeLen = 0;
		memset( szStrTemp, '\0', nStrMaxLen );
		file->Read( &nTypeLen, sizeof(int), &dwReadLength );
		file->Read( szStrTemp, sizeof(char)*nTypeLen, &dwReadLength );
		orgItem.m_strType = szStrTemp;
		listOrgItem.push_back( orgItem );
	}
	//
	return true;
}

bool CGeneralDataMan::LoadDataTypes( AFile* file )
{
	DWORD dwWriteLength = 0;
	int nMainTypeCount = 0;
	file->Read( &nMainTypeCount, sizeof(int), &dwWriteLength );
//	if ( nMainTypeCount != m_listMainType.size() )

	for ( int i = 0; i < nMainTypeCount; ++i )
	{
		int nSubTypeCount = 0;
		file->Read( &nSubTypeCount, sizeof(int), &dwWriteLength );
		for ( int j = 0; j < nSubTypeCount; ++j )
		{
			GENERAL_DATA_SUB_TYPE subTypeTemp;
			file->Read( &subTypeTemp.id, sizeof(int), &dwWriteLength );
			//-------------------------
			if ( subTypeTemp.id > m_nCurMaxSubID )
			{
				m_nCurMaxSubID = subTypeTemp.id;
			}
			//-------------------------
			file->Read( &subTypeTemp.szName, sizeof(char)*nStrNameLen, &dwWriteLength );
			int nSubTypeIDItemCount = 0;
			file->Read( &nSubTypeIDItemCount, sizeof(int), &dwWriteLength );
			for ( int k = 0; k < nSubTypeIDItemCount; ++k )
			{
				int nSubTypeItemID = 0;
				file->Read( &nSubTypeItemID, sizeof(int), &dwWriteLength );
				subTypeTemp.listSubTypeItemID.push_back( nSubTypeItemID );
			}

			m_listMainType[i]->listSubType.push_back( subTypeTemp );
		}
	}

	return true;
}

bool CGeneralDataMan::LoadBaseDataTemplValue( AFile* file, int nDataTemplCount, abase::vector<OrgItem>& listOrgItem )
{
	DWORD dwReadLength = 0;
	for ( int i = 0; i < nDataTemplCount; ++i )
	{
		BaseDataTempl* pDataItemTempl = CreateDefaultDataItemTempl( m_strOrgPath );
		
		//读取模板基本项
		int nTemplID = 0;
		file->Read( &nTemplID, sizeof(int), &dwReadLength );
		pDataItemTempl->SetIDEx( nTemplID );
		int nStrNameLen = 0;
		char szTemplName[nStrMaxLen];
		memset( szTemplName, '\0', nStrMaxLen );
		file->Read( &nStrNameLen, sizeof(int), &dwReadLength );
		file->Read( szTemplName, sizeof(char)*nStrNameLen, &dwReadLength );
		pDataItemTempl->SetName( szTemplName );
		memset( szTemplName, '\0', nStrMaxLen );
		file->Read( &nStrNameLen, sizeof(int), &dwReadLength );
		file->Read( szTemplName, sizeof(char)*nStrNameLen, &dwReadLength );
		pDataItemTempl->SetParentPathName( szTemplName );

		//读取模板通用项
		int nDataItemCount = 0;
		file->Read( &nDataItemCount, sizeof(int), &dwReadLength );
		if ( nDataItemCount != listOrgItem.size() )
		{
			AfxMessageBox( "数据项个数和模板项个数不匹配！" );
			return false;
		}
		for ( int j = 0; j < nDataItemCount; ++j )
		{
			AVariant var;
			var.Load( file );
			pDataItemTempl->SetItemValue( listOrgItem[j].m_strName + listOrgItem[j].m_strType, var );
		}

		//存入模板列表中
		m_vDataTemplList.push_back( pDataItemTempl );
	}

	return true;
}

bool CGeneralDataMan::Save( const char* szFileName )
{
	AFile aFile;
	if ( !aFile.Open( szFileName, AFILE_CREATENEW | AFILE_BINARY ) )
	{
		AfxMessageBox( "无法保存文件！" );
		return false;
	}

	//首先保存基本信息和模板信息，注意读取和写入的顺序要一致
	DWORD dwWriteLength = 0;
	DWORD dwVersion = nCurDataItemTemplVersion;
	aFile.Write( &dwVersion, sizeof(DWORD), &dwWriteLength );
	int nDataTemplCount = m_vDataTemplList.size();
	aFile.Write( &nDataTemplCount, sizeof(int), &dwWriteLength );
	SaveDataItemFormat( &aFile );
	SaveBaseDataTemplValue( &aFile );
	SaveDataTypes( &aFile );

	aFile.Close();
	//
	return true;
}

bool CGeneralDataMan::SaveDataItemFormat( AFile* file )
{
	if ( !m_vDataTemplList.size() )
	{
		return false;
	}
	BaseDataTempl* pDataItemTempl = m_vDataTemplList[0];

	DWORD dwWriteLength = 0;
	int nDataItemCount = pDataItemTempl->GetItemNum();
	file->Write( &nDataItemCount, sizeof(int), &dwWriteLength );

	for ( size_t i = 0; i < nDataItemCount; ++i )
	{
		AString strName = pDataItemTempl->GetItemName( i );
		AString strType = pDataItemTempl->GetItemType( i );
		int nNameLen = strName.GetLength();
		file->Write( &nNameLen, sizeof(int), &dwWriteLength );
		file->Write( (void* )((const char*)strName), sizeof(char)*nNameLen, &dwWriteLength );
		int nTypeLen = strType.GetLength();
		file->Write( &nTypeLen, sizeof(int), &dwWriteLength );
		file->Write( (void* )((const char*)strType), sizeof(char)*nTypeLen, &dwWriteLength );
	}
	//
	return true;
}

bool CGeneralDataMan::SaveBaseDataTemplValue( AFile* file )
{
	DWORD dwWriteLength = 0;
	int nDataTemplCount = m_vDataTemplList.size();

	for ( size_t i = 0; i < nDataTemplCount; ++i )
	{
		BaseDataTempl* pDataItemTempl = m_vDataTemplList[i];

		//存入模板的基本项
		int nTemplID = pDataItemTempl->GetID();
		file->Write( &nTemplID, sizeof(int), &dwWriteLength );
		AString strName = pDataItemTempl->GetName();
		int nStrNameLen = strName.GetLength();
		file->Write( &nStrNameLen, sizeof(int), &dwWriteLength );
		file->Write( (void*)((const char*)strName), sizeof(char)*nStrNameLen, &dwWriteLength );
		AString strParentPath = pDataItemTempl->GetParentPathName();
		int nStrParentPathLen = strParentPath.GetLength();
		file->Write( &nStrParentPathLen, sizeof(int), &dwWriteLength );
		file->Write( (void*)((const char*)strParentPath), sizeof(char)*nStrParentPathLen, &dwWriteLength );

		//存入通用数据项
		int nDataItemCount = pDataItemTempl->GetItemNum();
		file->Write( &nDataItemCount, sizeof(int), &dwWriteLength );
		for ( size_t j = 0; j < nDataItemCount; ++j )
		{
			AVariant var = pDataItemTempl->GetItemValue( j );
			var.Save( file );
		}
	}

	return true;
}

bool CGeneralDataMan::SaveDataTypes( AFile* file )
{
	DWORD dwWriteLength = 0;
	int nMainTypeCount = m_listMainType.size();
	file->Write( &nMainTypeCount, sizeof(int), &dwWriteLength );

	for ( size_t i = 0; i < nMainTypeCount; ++i )
	{
		int nSubTypeCount = m_listMainType[i]->listSubType.size();
		file->Write( &nSubTypeCount, sizeof(int), &dwWriteLength );
		for ( size_t j = 0; j < nSubTypeCount; ++j )
		{
			file->Write( &m_listMainType[i]->listSubType[j].id, sizeof(int), &dwWriteLength );
			file->Write( &m_listMainType[i]->listSubType[j].szName, sizeof(char)*nStrNameLen, &dwWriteLength );
			int nSubTypeIDItemCount = m_listMainType[i]->listSubType[j].listSubTypeItemID.size();
			file->Write( &nSubTypeIDItemCount, sizeof(int), &dwWriteLength );
			for ( size_t k = 0; k < nSubTypeIDItemCount; ++k )
			{
				file->Write( &m_listMainType[i]->listSubType[j].listSubTypeItemID[k], sizeof(int), &dwWriteLength );
			}
		}
	}

	return true;
}


bool CGeneralDataMan::Export( const char* szFileName )
{
	//判断输入参数的合法性
	if ( !szFileName || szFileName[0] == '\0' )
	{
		AfxMessageBox( "无效文件名！" );
		return false;
	}
	
	FILE* pFile = fopen( szFileName, "wb" );
	if ( !pFile )
	{
		CString str;
		str.Format( "无法打开文件%s!", szFileName );
		AfxMessageBox( str );
		return false;
	}

	DWORD dwTimeStamp = (DWORD)time(NULL);
	fwrite(&dwTimeStamp,sizeof(DWORD),1,pFile);
	
	CGeneralDataExporter* pDataExporter = CGeneralDataExporter::CreateDataExporter( m_nDataTemplTypeID );
	if ( pDataExporter )
	{
		pDataExporter->Export( m_vDataTemplList, m_listMainType, pFile );
	}

	fclose( pFile );
	delete pDataExporter;
	//
	return true;
}


bool CGeneralDataMan::AddDataItem( BaseDataTempl* pDataItemTempl, int nMainID, int nSubID )
{
	if ( !pDataItemTempl )
	{
		return false;
	}
	m_vDataTemplList.push_back( pDataItemTempl );

	//存入到SubType的模板ID列表中
	if ( !AddMainAndSubTypeID( pDataItemTempl->GetID(), nMainID, nSubID ) )
	{
		return false;
	}
	//
	return true;
}

bool CGeneralDataMan::RemoveDataItem( int nDataTemplID, int nMainID, int nSubID )
{
	int nDataTemplItemCount = m_vDataTemplList.size();
	for ( int i = 0; i < nDataTemplItemCount; ++i )
	{
		if ( m_vDataTemplList[i]->GetID() == nDataTemplID )
		{
			BaseDataTempl* pDataTempl = m_vDataTemplList[i];
			m_vDataTemplList.erase( &m_vDataTemplList[i] );
			delete pDataTempl;
		}
	}

	//删除类型表中对应的模板ID
	if ( !RemoveMainAndSubTypeID( nDataTemplID, nMainID, nSubID ) )
	{
		return false;
	}
	
	return true;
}

bool CGeneralDataMan::RemoveSubTypeAndDataItem( int nMainID, int nSubID )
{
	// 判断参数合法性
	if ( nMainID < 0 || nSubID < 0 )
	{
		return false;
	}

	//定位到具体SubType
	int nCurIndex = 0;
	for ( int i = 0; i < m_listMainType.size(); ++i )
	{
		if ( nMainID == m_listMainType[i]->id )
		{
			for ( int j = 0; j < m_listMainType[i]->listSubType.size(); ++j )
			{
				if ( nSubID == m_listMainType[i]->listSubType[j].id )
				{
					//删除所有该类下面的数据模板
					for ( int k = 0; k < m_listMainType[i]->listSubType[j].listSubTypeItemID.size(); ++k )
					{
						for ( nCurIndex = 0; nCurIndex < m_vDataTemplList.size(); ++nCurIndex )
						{
							if ( m_listMainType[i]->listSubType[j].listSubTypeItemID[k] == m_vDataTemplList[nCurIndex]->GetID() )
							{
								delete m_vDataTemplList[nCurIndex];
								m_vDataTemplList.erase( &m_vDataTemplList[nCurIndex] );
								break;
							}
						}
					}
					m_listMainType[i]->listSubType.erase( &(m_listMainType[i]->listSubType[j]) );
					return true;
				}
			}
		}
	}

	return false;
}

bool CGeneralDataMan::InitGeneralDataIDMan( const char* szDataIDManFileName )
{
	//判断输入参数合法性
	if ( !szDataIDManFileName )
	{
		return false;
	}

	if ( m_pGeneralDataIDMan )
	{
		delete m_pGeneralDataIDMan;
		m_pGeneralDataIDMan = NULL;
	}

	m_pGeneralDataIDMan = new BaseDataIDManBase;
	if ( !m_pGeneralDataIDMan )
	{
		AfxMessageBox( "初始化模板ID数据对象失败！" );
		return false;
	}

	if ( m_pGeneralDataIDMan->Load( szDataIDManFileName ) != 0 )
	{
		AfxMessageBox( "加载模板ID数据失败！" );
		return false;
	}

	//
	return true;
}

BaseDataTempl* CGeneralDataMan::CreateDefaultDataItemTempl( CString strOrgPath, bool bIsNewID )
{
	CString strFileName;
	int nCurrentMaxID = m_pGeneralDataIDMan->GetMaxID() + 1;
	strFileName.Format( "模板_%d", nCurrentMaxID );
	BaseDataTempl* pDataItemTempl = new BaseDataTempl;
	if ( !pDataItemTempl ||
		!pDataItemTempl->CreateTempl( strOrgPath + "\\" + strFileName, "", m_pGeneralDataIDMan, bIsNewID ) )
	{
		AfxMessageBox( "创建模板失败！" );
		return NULL;
	}
	pDataItemTempl->SetName( (const char*)strFileName );

	//
	return pDataItemTempl;
}

bool CGeneralDataMan::ExportAllItemAttribute( const char* szFileName )
{
	//判断输入参数的合法性
	if ( !szFileName || !szFileName[0] )
	{
		AfxMessageBox( "无效文件名！" );
		return false;
	}

	//打开文件
	FILE* fp = fopen( szFileName, "w" );
	if ( !fp )
	{
		AfxMessageBox( "无法打开文件!" );
		return false;
	}

	//遍历整个主类型列表,存入各物品的主要信息, Modified 2012-04-28.
	for ( int i = 0; i < m_listMainType.size(); ++i )
	{
		for ( int j = 0; j < m_listMainType[i]->listSubType.size(); ++j )
		{
			for ( int k = 0; k < m_listMainType[i]->listSubType[j].listSubTypeItemID.size(); ++k )
			{
				BaseDataTempl* pDataTempl = GetDataItemTemplByID( m_listMainType[i]->listSubType[j].listSubTypeItemID[k] );
				if ( pDataTempl )
				{
					fprintf(fp, _format_item_sub_type, m_listMainType[i]->listSubType[j].szName);
					fprintf(fp, "\r\n");

					fprintf(fp, _format_item_name, pDataTempl->GetName());
					fprintf(fp, "\r\n");

					fprintf(fp, _format_item_id, pDataTempl->GetID());
					fprintf(fp, "\r\n");

					fprintf(fp, _format_item_treasure_id, (int)(pDataTempl->GetItemValue(1)));//!!!!!!注意：由于需要从模板中取得相应的数据项，现在暂约定模板第1，2项固定!!!!!!
					fprintf(fp, "\r\n");
				}
			}
		}
	}

	fclose(fp);
	return true;
}
