//=============================================================================
//
//EC_OnlineAward.cpp
//
//=============================================================================
//-----------------------------------------------------------------------------
#include "EC_Global.h"
#include "EC_OnlineAward.h"
#include "EC_Game.h"
#include "ExpTypes.h"
#include "elementdataman.h"
#include "EC_GameSession.h"


#define new A_DEBUG_NEW
//-----------------------------------------------------------------------------
bool OnlineAwardInfoList::InitFromLocal( int nAwardId )
{
	//首先获得相应奖励config模板
	DATA_TYPE dt;
	ONLINE_GIFT_BAG_CONFIG* pAwardConfig = (ONLINE_GIFT_BAG_CONFIG*)g_pGame->GetElementDataMan()->get_data_ptr( nAwardId, ID_SPACE_CONFIG, dt );
	if ( !pAwardConfig )
	{
		return false;
	}

	//从config模板中取得awardinfo
	for ( int i = 0; i < MAX_LIST_SIZE; ++i )
	{
		m_AwardInfoList[i].small_gift_bag_id	= pAwardConfig->small_gift_bags[i].gift_bag_id;
		m_AwardInfoList[i].remain_time			= pAwardConfig->small_gift_bags[i].deliver_interval;	
	}
	//
	return true;
}

//-----------------------------------------------------------------------------
OnlineAwardManager::~OnlineAwardManager()
{
	//清除在线奖励信息列表
	ReleaseOnlineAwardInfoMap();
}

void OnlineAwardManager::ReleaseOnlineAwardInfoMap( void )
{
	OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.begin();
	for ( ; iterAwardInfo != m_OnlineAwardInfoMap.end(); ++iterAwardInfo )
	{
		if ( iterAwardInfo->second )	//存在，则删除
		{
			delete (iterAwardInfo->second);
		}
	}
	m_OnlineAwardInfoMap.clear();
}

void OnlineAwardManager::Tick( DWORD dwDeltaTime )
{
	//设置为：每隔1m检查一次当前所有激活的在线奖励信息列表
	if ( m_TickCounter.IncCounter( dwDeltaTime ) )
	{
		m_TickCounter.Reset();
		
		//分别检查各大礼包的奖励发放情况
		OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.begin();
		for ( ; iterAwardInfo != m_OnlineAwardInfoMap.end(); ++iterAwardInfo )
		{
			if ( iterAwardInfo->second && iterAwardInfo->second->CheckDeliverTime( TIME_INTERVAL ) )
			{
				g_pGame->GetGameSession()->c2s_CmdObtainOnlineAward( iterAwardInfo->first, 
																	iterAwardInfo->second->GetCurValidIndex(), 
																	iterAwardInfo->second->GetCurSmallBagID(), true );
			}
		}
		
	}
}

int OnlineAwardManager::UpdateData( const S2C::online_award_info& onlineAwardInfo )
{
	//如果在线奖励信息列表中的强制更新标志不为0，则先清除本地缓存奖励信息列表 Modified 2012-04-01.
	if ( onlineAwardInfo.force_update_flag )
	{
		//清除在线奖励信息列表
		ReleaseOnlineAwardInfoMap();	
	}

	//遍历整个奖励信息列表，并更新本地的奖励信息缓存列表
	for ( int i = 0; i < onlineAwardInfo.award_count; ++i )
	{
		OnlineAwardInfoList* pOnlineAwardInfo = NULL;
		int nCurAwardID = onlineAwardInfo.award_info_list[i].award_id;
		OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.find( nCurAwardID );
		if ( iterAwardInfo == m_OnlineAwardInfoMap.end() ) //当前OnlineAwardManager中不存在该Award信息
		{
			pOnlineAwardInfo = new OnlineAwardInfoList;
			if ( !pOnlineAwardInfo )
			{
				return ERR_ALLOCATE_FAIL;
			}
			if ( !pOnlineAwardInfo->InitFromLocal( nCurAwardID ) )
			{
				delete pOnlineAwardInfo;
				pOnlineAwardInfo = NULL;
				return ERR_NO_SUCH_DATA;
			}

			//存入到OnlineAwardInfoMap中
			m_OnlineAwardInfoMap[nCurAwardID] = pOnlineAwardInfo;
		}
		else	//当前OnlineAwardManager中存在该Award信息,仅做更新操作
		{
			pOnlineAwardInfo = iterAwardInfo->second;
		}

		//更新操作
		int retCode = pOnlineAwardInfo->UpdateAwardInfoList(	onlineAwardInfo.award_info_list[i].index, 
																onlineAwardInfo.award_info_list[i].small_gift_id, 
																onlineAwardInfo.award_info_list[i].remain_time );
		if ( ERR_ONLINE_AWARD_SUCCESS != retCode )
		{
			delete pOnlineAwardInfo;
			pOnlineAwardInfo = NULL;
			m_OnlineAwardInfoMap.erase( nCurAwardID );
			return retCode;
		}
	}

	//
	return ERR_ONLINE_AWARD_SUCCESS;

}


void OnlineAwardManager::UpdateAwardListInfo( int nAwardID, int nBagIndex )
{
	//根据奖励大礼包ID查找整个列表
	OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.find( nAwardID );
	if ( iterAwardInfo == m_OnlineAwardInfoMap.end() )
	{
		return;
	}

	//更新下一轮发奖，如果到了该大礼包奖励列表的最后，则直接清除该大礼包
	OnlineAwardInfoList* pOnlineAwardInfo = iterAwardInfo->second;
	if ( pOnlineAwardInfo && !pOnlineAwardInfo->UpdateAwardInfo( nBagIndex ) )	//清除？
	{
		m_OnlineAwardInfoMap.erase( iterAwardInfo );
		delete pOnlineAwardInfo;
	}
}

void OnlineAwardManager::StopAwardListTickStatus( int nAwardID, int nBagIndex )
{
	//根据奖励大礼包ID查找整个列表
	OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.find( nAwardID );
	if ( iterAwardInfo == m_OnlineAwardInfoMap.end() )
	{
		return;
	}

	//检测参数合法性
	OnlineAwardInfoList* pOnlineAwardInfo = iterAwardInfo->second;
	if ( !pOnlineAwardInfo->CheckBagItem( nBagIndex ) )	//不合法？不处理？
	{
		return;
	}

	//停止当前礼包的Tick检测
	pOnlineAwardInfo->SetTickStatus( true );
}

void OnlineAwardManager::GetOnlineAwardInfos( OnlineAwardInfoVector& onlineAwardInfoVector )
{
	onlineAwardInfoVector.clear();
	
	//遍历整个礼包信息列表
	OnlineAwardInfoList* pOnlineAwardInfo = NULL;
	OnlineAwardInfoMap::iterator iterAwardInfo = m_OnlineAwardInfoMap.begin();
	for ( ; iterAwardInfo != m_OnlineAwardInfoMap.end(); ++iterAwardInfo )
	{
		pOnlineAwardInfo = iterAwardInfo->second;
		if ( pOnlineAwardInfo )
		{
			OnlineAwardInfoItem awardInfoItem;
			awardInfoItem.m_nAwardID		=	iterAwardInfo->first;
			awardInfoItem.m_nSmallBagIndex	=	pOnlineAwardInfo->GetCurValidIndex();
			awardInfoItem.m_nRemainTime		=	pOnlineAwardInfo->GetCurBagRemainTime();
			onlineAwardInfoVector.push_back( awardInfoItem );
		}
	}
}
