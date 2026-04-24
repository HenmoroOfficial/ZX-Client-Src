/**
* @File EC_BattleDungeon.h
* @Brief 当前所在副本信息
* @Author Fu Chonggang
* @Date 2011-7-25 17:02:44
* @Copyright (c) 2011 ZhuXian Studio, All Rights Reserved. 
* 注意：这里不包含爬塔副本的管理
*/
#pragma once

#include "DlgBase.h"
#include "graidroominfo"
#include "vector.h"
#include "hashmap.h"

struct TRANSCRIPTION_CONFIG;

enum DungeonError
{
	ERR_RAID_MAP_NOTEXIST                   = 1, //地图不存在
    ERR_RAID_ROOM_NUMBER_FULL               = 2, //房间数到达上限
    ERR_RAID_JOIN_ALREADY                   = 3, //已经加入副本
    ERR_RAID_ROOMID_EXIST                   = 4, //副本房间id重复
    ERR_RAID_ROLE_OFFLINE                   = 5, //玩家已下线
    ERR_RAID_ROOM_INEXIST                   = 6, //副本房间不存在
    ERR_RAID_ARGUMENT_ERR                   = 7, //参数错误
    ERR_RAID_STARTED                        = 8, //副本已开始
    ERR_RAID_APPLY_TEAM_FULL                = 9, //副本报名队伍已满
    ERR_RAID_ROLE_ALREADY_IN_ROOM           = 10,//已经在副本房间中
    ERR_RAID_OPERATOR_NOT_MASTER            = 11,//操作人不是房主
    ERR_RAID_TEAM_FULL                      = 12,//副本正式队伍已满
    ERR_RAID_NOT_IN_APPLYLIST               = 13,//不在申请队伍中
    ERR_RAID_JOIN_REFUSED                   = 14,//房主拒绝申请者
    ERR_RAID_START_TEAMNUMBER               = 15,//房间人数不足
    ERR_RAID_GET_INSTANCE_FAIL              = 16,//DELIVER获取实例失败
    ERR_RAID_ROLE_NOT_IN_ROOM               = 17,//玩家不在此房间
    ERR_RAID_STATUS                         = 18,//房间当前状态不允许此操作
    ERR_RAID_INSTANCE_INEXIST               = 19,//副本实例不存在
    ERR_RAID_ROOMCLOSE                      = 20,//副本房间关闭
    ERR_RAID_ROLE_NOT_IN_RAID               = 21,//玩家未报名副本
    ERR_RAID_ITEMLIMIT                      = 22,//开房间物品不足
    ERR_RAID_STATUS_CHANGE                  = 23,//状态切换错误
    ERR_RAID_GS_GET_INSTANCE_FAIL           = 24,//GS获取实例失败
    ERR_RAID_IS_OPENING                     = 25,//玩家已经在申请开房间
    ERR_RAID_GS_ENTER_CHECK_FAIL            = 26,//GS检查进入副本条件失败
    ERR_RAID_DB_TIMEOUT                     = 27,//DB处理超时
	ERR_RAID_ROOMNAME                       = 28,//房间名错误
	ERR_RAID_VOTECOOLDOWN					= 29,//投票冷却中
	ERR_RAID_VOTEING 						= 30,//正在进行其他投票
	ERR_RAID_VOTEDROLE_NOT_IN_ROOM 			= 31,//被投票人不在房间中
	ERR_RAID_VOTEDROLE_NOT_IN_RAID 			= 32,//被投票人不在副本中
	ERR_RAID_VOTER_NOT_IN_ROOM 				= 33,//投票人不在房间中
	ERR_RAID_VOTER_NOT_IN_RAID 				= 34,//投票人不在副本中
	ERR_RAID_NOT_VOTEING 					= 35,//不在进行投票
	ERR_RAID_VOTEDROLE_MISMATCH				= 36,//被投票人不匹配
	ERR_RAID_ROOM_FORBID_VOTE 				= 37,//房间不允许投票

    
	//-------帮战错误码，由于以前的代码限制，此处的宏仅供参考，不可直接用来报错误码
	ERR_RAID_NOT_VALID_FACTITLE             = 41,//帮派职务不符
	ERR_RAID_TARGET_NOT_ONLINE              = 42,//被挑战人不在线
	ERR_RAID_NOT_IN_FACTION                 = 43,//未参加帮派
	ERR_RAID_IN_SAME_FACTION                = 44,//属于同一帮派
	ERR_RAID_FAC_LOWLEVEL                   = 45,//帮派等级不符
	ERR_RAID_CHALLENGE_COOLING              = 46,//帮派挑战冷却中
	ERR_RAID_FACTION_COMBAT_BUSY            = 47,//正在帮战中
	ERR_RAID_TARGET_MASTER_NOT_ONLINE       = 48,//被挑战帮主不在线
	ERR_RAID_FACTION_NOPROSPERITY           = 49,//帮贡不足
	ERR_RAID_FACTION_CHALLENGE_REJECTED     = 50,//帮战被拒绝
	ERR_RAID_FACTION_CHALLENGE_TIMEOUT      = 51,//帮主邀请超时     
	ERR_RAID_FACTION_JOIN_GROUP             = 52,//报名阵营错误
	ERR_RAID_FACTION_CANCEL_ARU_ERR         = 53,//取消帮战错误
	//--------帮战错误码

	ERR_RAID_TEAMAPPLY_COOLING				= 55,//组队报名冷却
	ERR_RAID_NOT_APPLY_TEAM					= 56,//没有申请组队副本
	ERR_RAID_NOT_TEAMAPPLY_TIME				= 57,//不是报名时间
	
	// 客户端单独使用的错误码
	ERR_RAID_6V6_SIGNUP_IN_BATTLE			= 1043,//6v6不能在战场中报名
	ERR_RAID_RENASCENCE_COUNT				= 1044,//转生次数限制
	ERR_RAID_CLIENT_ENTER_ITEM_LIMIT		= 1045,//进入物品限制
	ERR_RAID_CLIENT_REPUTATION_LIIT			= 1046,//声望限制
	ERR_RAID_CLIENT_PLAYER_LEVEL			= 1047,//等级限制		
	ERR_RAID_CLIENT_ENTER_ENOUGHTIMES		= 1048,//每天加入次数限制
	ERR_RAID_CLIENT_LACK_MONEY				= 1049,//加入金钱不够
	

};

enum RAID_TEAM_TYPE//TEAM_RAID
{               
	RAIDTEAM_NEUTRAL = 0,//not used
    RAIDTEAM_ATTACK,
    RAIDTEAM_DEFEND,
    RAIDTEAM_LOOKER,
}; 

class CECBattleDungeon
{
public:
	CECBattleDungeon() {}
	CECBattleDungeon(int id);
	~CECBattleDungeon(){}
	int			m_iId;
	int			m_iMapId;
	TRANSCRIPTION_CONFIG*	m_pConfigData;
	abase::vector<GRaidRoomInfo>	m_vecRoomInfo;

	void Init(int id);
};

class CECBattleDungeonMan
{
public:
	typedef abase::hash_map<int, int> DungeonEnterInfoMap;
	DungeonEnterInfoMap		m_mapDungeonEnterInfo;
	int			m_iRoomId;
	int			m_iLeaderId;
	int			m_iEndTime;
	int			m_iFaction;		//0:中立 1：攻方 2：守方 3：旁观
	
    static CECBattleDungeonMan* getSingletonPtr( void )
	{  return ( &ms_Singleton );  }
	
	~CECBattleDungeonMan() {}

	void Init(int id);
	void InitFromMapId(int id);
	CECBattleDungeon* GetCurBattle() { return m_pCurBattle; }
	CECBattleDungeon* GetInTeamBattle() { return m_pInTeamBattle; }
	CECBattleDungeon* GetBattle(int mapId);
	void JoinBattle(int mapId);
	void ReportError(int id);
	void ReportFactionPKError(int id);
	void AddHint(void *pData);
	void KickPlayer(void *pData);
	
	void DoKickPlayer(int id);

	bool IsInTower() { return m_bInTower; }
	void SetInTower(bool b) { m_bInTower=b; }
	
private:
	CECBattleDungeonMan() : m_pCurBattle(0) {}
private:
	typedef abase::hash_map<int, CECBattleDungeon*> BattleDungeonMap;
	BattleDungeonMap	m_mapBattleDungeon;
	CECBattleDungeon*	m_pCurBattle;		// 当前查看副本或正在进行的副本
	CECBattleDungeon*	m_pInTeamBattle;	// 当前已经报名的副本
	bool				m_bInTower;

	static CECBattleDungeonMan ms_Singleton;
};