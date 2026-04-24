// GeneralDataExporter.h: interface for the CGeneralDataExporter class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GENERALDATAEXPORTER_H__1518AB06_DD3A_40C0_AE70_F97623B01240__INCLUDED_)
#define AFX_GENERALDATAEXPORTER_H__1518AB06_DD3A_40C0_AE70_F97623B01240__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//=============================================================================
#include "BaseDataTemplate.h"
#include "GeneralDataManUtil.h"
#include "GeneralDataMan.h"
//=============================================================================
//export types.
struct VIPAward
{
	unsigned int	tid;					//模板ID
	namechar		szName[nStrNameLen];	//所含物品的名称
	unsigned int	item_id;				//所含物品的模板id
	unsigned int	count;					//所含物品的个数

	unsigned int	award_type;				//奖励类型 0：普通奖励， 1：VIP奖励
	unsigned int	award_level;			//奖励级别 普通奖励的奖励级别根据玩家等级设定，VIP等级根据充值额度设定
	unsigned int	award_obtain_type;		//获取奖励的方式：0：每日领取，1：特殊领取
	int				expire_time;			//期限，奖品有单独的时限
};


//=============================================================================
class CGeneralDataExporter  
{
public:
	CGeneralDataExporter();
	virtual ~CGeneralDataExporter();
	virtual bool Export( abase::vector<BaseDataTempl*>& vDataTemplList, abase::vector<GENERAL_DATA_MAIN_TYPE*>& vMainTypeList, FILE* pFile )
	{
		return true;
	}

	static CGeneralDataExporter* CreateDataExporter( GENERAL_DATA_TEMPL_TYPE_ID nDataTemplTypeID );

protected:
	inline bool ExportData( const BaseDataTempl* pDataTempl, char* pDataBuf, int nDataBufSize, int nTemplListBeginIndex = 0 )
	{
		//判断输入参数的合法性
		if ( !pDataTempl || !pDataBuf || !nDataBufSize )
		{
			return false;
		}
		
		char* pOrg = pDataBuf;
		//将模板中的通用数据项倒入到数据缓存中
		for ( size_t i = nTemplListBeginIndex; i < pDataTempl->GetItemNum(); ++i )
		{
			if ( "int64" == pDataTempl->GetItemType(i) )
			{
				*(__int64*)pDataBuf = ((__int64)pDataTempl->GetItemValue(i));
				pDataBuf += sizeof(__int64);
			}
			else if ( "double" == pDataTempl->GetItemType(i) )
			{
				*(double*)pDataBuf = ((double)pDataTempl->GetItemValue(i));
				pDataBuf += sizeof(double);
			}
			else if ( "uint64" == pDataTempl->GetItemType(i) )
			{
				*(unsigned __int64*)pDataBuf = ((unsigned __int64)pDataTempl->GetItemValue(i));
				pDataBuf += sizeof(unsigned __int64);
			}
			else if ( "uint64" == pDataTempl->GetItemType(i) )
			{
				*(unsigned __int64*)pDataBuf = ((unsigned __int64)pDataTempl->GetItemValue(i));
				pDataBuf += sizeof(unsigned __int64);
			}
			else
			{
				*(int*)pDataBuf = ((int)pDataTempl->GetItemValue(i));
				pDataBuf += sizeof(int);
			}
		}
		
		//判断缓存长度
		if ( (pDataBuf - pOrg) != nDataBufSize )
		{
			CString str;
			str.Format( "数据导出错误！模板名称=%s, 模板大小=%d, 数据结构大小=%d",
						pDataTempl->GetName(), pDataBuf - pOrg, nDataBufSize);
			::MessageBox( NULL, str, "错误", MB_ICONSTOP );
		}

		return true;
	}

	virtual bool FilterAndCheckData( void )
	{
		return true;
	}

	virtual bool SaveFile( FILE* pFile )
	{
		return true;
	}

};

class VIPAwardDataExporter : public CGeneralDataExporter
{
public:
	VIPAwardDataExporter(){};
	virtual ~VIPAwardDataExporter(){};
	virtual bool Export( abase::vector<BaseDataTempl*>& vDataTemplList, abase::vector<GENERAL_DATA_MAIN_TYPE*>& vMainTypeList, FILE* pFile );

protected:
	inline int GetFilterDataCount( abase::vector<GENERAL_DATA_MAIN_TYPE*>& vMainTypeList )
	{
		int nFilterDataCount = 0;
		if ( vMainTypeList.size() < nVIPAwardMainTypeCount )
		{
			return 0;
		}
	
		for ( int i = 0; i < vMainTypeList[nVIPAwardMainTypeCount-1]->listSubType.size(); ++i )
		{
			nFilterDataCount += vMainTypeList[nVIPAwardMainTypeCount-1]->listSubType[i].listSubTypeItemID.size();
		}

		return nFilterDataCount;
	}

	inline bool IsFilterDataTempl( int nDataTemplID, abase::vector<GENERAL_DATA_MAIN_TYPE*>& vMainTypeList )
	{
		if ( vMainTypeList.size() < nVIPAwardMainTypeCount )
		{
			return false;
		}

		for ( int i = 0; i < vMainTypeList[nVIPAwardMainTypeCount-1]->listSubType.size(); ++i )
		{
			for ( int j = 0; j < vMainTypeList[nVIPAwardMainTypeCount-1]->listSubType[i].listSubTypeItemID.size(); ++j )
			{
				if ( nDataTemplID == vMainTypeList[nVIPAwardMainTypeCount-1]->listSubType[i].listSubTypeItemID[j] )
				{
					return true;
				}
			}
		}

		//
		return false;
	}
};


#endif // !defined(AFX_GENERALDATAEXPORTER_H__1518AB06_DD3A_40C0_AE70_F97623B01240__INCLUDED_)
