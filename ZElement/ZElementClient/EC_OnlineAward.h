/**
 * FILE: EC_OnlineAward.h
 * 
 * DESCRIPTION: 
 *
 * CREATED BY: Ford.W 2012-02-16.
 *
 * HISTORY:
 *
 * Copyright (c) 2012 PWRD, All Rights Reserved.
 */

#ifndef __EC_ONLINE_AWARD__
#define __EC_ONLINE_AWARD__
//-----------------------------------------------------------------------------
#include "hashmap.h"
#include "EC_GPDataType.h"
#include "EC_Counter.h"

//-----------------------------------------------------------------------------
//
enum ONLINE_AWARD_ERR_CODE
{
	ERR_ONLINE_AWARD_SUCCESS		= 0,
	ERR_INVALID_POS					= 1,
	ERR_INVALID_BAG_ID				= 2,
	ERR_INVALID_REMAIN_TIME			= 3,
	ERR_ALLOCATE_FAIL				= 4,
	ERR_NO_SUCH_DATA				= 5,
};
//-----------------------------------------------------------------------------
//
class OnlineAwardInfoList
{
public:
	typedef struct InfoList
	{
		int		small_gift_bag_id;	//当前礼包中小礼包的id
		int		remain_time;		//小礼包可以发放的剩余时间
	}AwardInfoList;

public:
	OnlineAwardInfoList() : m_nCurValidIndex(0), m_bStopTick(false)
	{
		memset( m_AwardInfoList, 0, sizeof(m_AwardInfoList) );
	}

	~OnlineAwardInfoList() {}

public:
	enum CONST_VALUE
	{
		MAX_LIST_SIZE = 16,		//!!!注意：需要和data编辑器中大礼包中小礼包容量相匹配!!!
	};

public:
	bool InitFromLocal( int nAwardId );
	inline int UpdateAwardInfoList( int nIndex, int nBagID, int nRemainTime )
	{
		//判断输入参数的合法性
		if ( nIndex < 0 || nIndex >= MAX_LIST_SIZE )
		{
			return ERR_INVALID_POS;
		}

		if ( m_AwardInfoList[nIndex].small_gift_bag_id != nBagID )
		{
			return ERR_INVALID_BAG_ID;
		}

		if ( m_AwardInfoList[nIndex].remain_time < nRemainTime )	//客户端该奖励礼包的剩余时间小于服务器传过来的时间，则直接返回不进行更新
		{
			return ERR_ONLINE_AWARD_SUCCESS;
		}

		m_AwardInfoList[nIndex].remain_time = nRemainTime;
		m_nCurValidIndex = nIndex;

		//一切正常，返回0
		return ERR_ONLINE_AWARD_SUCCESS;
	}
	inline bool CheckDeliverTime( int nDeltaTime ) //nDeltaTime:单位，秒
	{
		if ( m_bStopTick )	//如果此时Tick检测操作需要停止，则直接返回，不需要判断
		{
			return false;
		}
		m_AwardInfoList[m_nCurValidIndex].remain_time -= nDeltaTime;
		return ( m_AwardInfoList[m_nCurValidIndex].remain_time <= 0 ); //剩余时间为<=0， 可以发放了
	}
	inline int GetCurValidIndex()const { return m_nCurValidIndex; }
	inline int GetCurSmallBagID()const { return m_AwardInfoList[m_nCurValidIndex].small_gift_bag_id; }
	inline bool UpdateAwardInfo( int nBagIndex )	//当前大礼包的前一个小礼包已经领取，更新下一个小礼包的倒计时
	{
		//检查输入参数的合法性
		if ( nBagIndex < 0 || nBagIndex >= MAX_LIST_SIZE || 
			nBagIndex != m_nCurValidIndex || m_AwardInfoList[nBagIndex].small_gift_bag_id <= 0 )
		{
			return false;
		}

		//寻找下一个有效的小礼包位置
		while ( (++m_nCurValidIndex < MAX_LIST_SIZE) && (m_AwardInfoList[m_nCurValidIndex].small_gift_bag_id <= 0) );

		//如果没有找到合适的，说明已经到了奖励列表的最后，可以直接移出该奖励小礼包列表
		if ( m_nCurValidIndex >= MAX_LIST_SIZE )
		{
			return false;
		}

		//启动了新一轮的奖励发放倒计时，开启Tick状态
		SetTickStatus( false );

		//
		return true;
	}
	inline void SetTickStatus( bool bStopTick ) { m_bStopTick = bStopTick; }
	inline bool	CheckBagItem( int nBagIndex )
	{
		if ( nBagIndex < 0 || nBagIndex >= MAX_LIST_SIZE )
		{
			return false;
		}

		if ( m_AwardInfoList[nBagIndex].small_gift_bag_id <= 0 )
		{
			return false;
		}
		//
		return true;
	}
	inline int GetCurBagRemainTime() const { return m_AwardInfoList[m_nCurValidIndex].remain_time; }

protected:
	AwardInfoList	m_AwardInfoList[MAX_LIST_SIZE];	//奖励小礼包列表
	int				m_nCurValidIndex;				//当前发放的礼包有效位置
	bool			m_bStopTick;					//是否启动Tick操作，针对情况：如果本轮在线奖励可以发放，并通知了服务器，但玩家不领取的情况下，需要停止本轮Tick检测
};

//-----------------------------------------------------------------------------
struct OnlineAwardInfoItem
{
	int		m_nAwardID;
	int		m_nSmallBagIndex;
	int		m_nRemainTime;
};
typedef abase::vector<OnlineAwardInfoItem>	OnlineAwardInfoVector;

//-----------------------------------------------------------------------------
class OnlineAwardManager
{
public:
	OnlineAwardManager() 
	{
		m_TickCounter.SetPeriod(TIME_INTERVAL*1000);//将秒数换算成毫秒
	}
	~OnlineAwardManager();

public:
	enum CONST_VALUE
	{
		// Changed by Fu Chonggang,2013.8.15, send protocol too often
		TIME_INTERVAL = 60,		//单位：秒 ,需要和m_TickCounter的检测周期相同
	};

public:
	void Tick( DWORD dwDeltaTime = 0 );									//不断检测当前激活的在线奖励是否可以发放
	int UpdateData( const S2C::online_award_info& onlineAwardInfo );	//在收到gs在线奖励列表信息的时候，更新相关数据信息
	void UpdateAwardListInfo( int nAwardID, int nBagIndex );			//更新某大礼包中的下一轮奖励发放
	void StopAwardListTickStatus( int nAwardID, int nBagIndex );		//停止某大礼包本轮奖励的Tick检测操作
	void GetOnlineAwardInfos( OnlineAwardInfoVector& onlineAwardInfoVector );	//获得所有在线奖励相关信息，列表形式

protected:
	void ReleaseOnlineAwardInfoMap( void );								//清除本地缓存的在线奖励信息列表

protected:
	typedef abase::hash_map<int, OnlineAwardInfoList*> OnlineAwardInfoMap;
	OnlineAwardInfoMap		m_OnlineAwardInfoMap;
	CECCounter				m_TickCounter;
};




#endif //(__EC_ONLINE_AWARD__)