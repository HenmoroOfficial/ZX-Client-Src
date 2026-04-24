// GeneralDataExporter.cpp: implementation of the CGeneralDataExporter class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "elementdata.h"
#include "GeneralDataExporter.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CGeneralDataExporter::CGeneralDataExporter()
{

}

CGeneralDataExporter::~CGeneralDataExporter()
{

}

CGeneralDataExporter* CGeneralDataExporter::CreateDataExporter( GENERAL_DATA_TEMPL_TYPE_ID nDataTemplTypeID )
{
	CGeneralDataExporter* pGeneralDataExporter = NULL;
	if ( nDataTemplTypeID < GENERAL_DATA_TEMPL_TYPE_DEFAULT || 
		 nDataTemplTypeID > GENERAL_DATA_TEMPL_TYPE_ID_MAX_NUM )
	{
		return NULL;
	}

	switch ( nDataTemplTypeID )
	{
		case GENERAL_DATA_TEMPL_TYPE_DEFAULT:
		{
			break;
		}
		case GENERAL_DATA_TEMPL_TYPE_VIPAWARD:
		{
			pGeneralDataExporter = new VIPAwardDataExporter;
			break;
		}
		default:
			break;
	}

	return pGeneralDataExporter;
}

//=============================================================================
bool VIPAwardDataExporter::Export( abase::vector<BaseDataTempl*>& vDataTemplList, abase::vector<GENERAL_DATA_MAIN_TYPE*>& vMainTypeList, FILE* pFile )
{
	int nDataTemplCount = vDataTemplList.size();
	
	//过滤掉下架物品数量
	int nFilterDataCount = GetFilterDataCount( vMainTypeList );
	int nDataTemplExportCount = nDataTemplCount - nFilterDataCount;
	if ( nDataTemplExportCount < 0 )
	{
		CString str;
		str.Format( "导出数据模板个数错误！导出模板数 = %d", nDataTemplExportCount );
		MessageBox(NULL,str,"错误", MB_ICONSTOP);
		return false;
	}
	fwrite( &nDataTemplExportCount, sizeof(int), 1, pFile ); //保存所需要导出的模板个数

	if ( !pFile || !nDataTemplCount  )
	{
		return false;
	}
	
	for ( int i = 0; i < nDataTemplCount; ++i )
	{
		BaseDataTempl* pDataTempl = vDataTemplList[i];
		if ( IsFilterDataTempl(pDataTempl->GetID(), vMainTypeList) ) //如果是需要过滤得模板，则不需要导出
		{
			nFilterDataCount--;
			continue;
		}

		//开始导出到具体数据结构中
		VIPAward curVIPAward;
		curVIPAward.tid = pDataTempl->GetID();
		wcsncpy( (wchar_t*)curVIPAward.szName, 
				 CSafeString(pDataTempl->GetName()),
				 sizeof(curVIPAward.szName) / sizeof(namechar) - 1 );

		int nHeaderSize = sizeof(unsigned int) + sizeof(namechar)*nStrNameLen;
		if ( !ExportData( pDataTempl, (char*)(curVIPAward.szName + nStrNameLen), sizeof(VIPAward) - nHeaderSize, 1 ) )//!!!dummy_int项（第一项）去掉，不导出!!!
		{
			return false;
		}

		// 判断一些数据项的合法性，也可以通过重载FilterAndCheckData接口来实现，这里，为了效率，就直接写到这个导出接口中
		if ( !curVIPAward.item_id )
		{
			CString str;
			str.Format( "物品：%s的ID不能为0！", pDataTempl->GetName() );
			MessageBox(NULL,str,"错误", MB_ICONSTOP);
			return false;
		}

		if ( curVIPAward.award_type < 0 || curVIPAward.award_type > 1 )
		{
			CString str;
			str.Format( "奖励物品(ID=%d)的类型非法！", pDataTempl->GetID() );
			MessageBox(NULL,str,"错误", MB_ICONSTOP);
			return false;			
		}

		//直接写文件
		fwrite( &curVIPAward, sizeof( VIPAward ), 1, pFile );
	}

	if ( nFilterDataCount != 0 )
	{
		CString str;
		str.Format( "数据导出错误！下架物品过滤出错！过滤数量=%d", nFilterDataCount );
		MessageBox(NULL,str,"错误", MB_ICONSTOP);
		return false;			
	}
	//
	return true;
}
