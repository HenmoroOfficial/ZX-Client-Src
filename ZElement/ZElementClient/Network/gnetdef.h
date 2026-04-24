#ifndef __GNETDEF_H__
#define __GNETDEF_H__

namespace GNET{

#define WM_IOEVENT      (WM_USER + 100)
#define WM_IOPROTOCOL   (WM_USER + 101)

#define EVENT_ADDSESSION    1
#define EVENT_DELSESSION    2
#define EVENT_ABORTSESSION  3
#define EVENT_PINGRETURN    4
#define EVENT_DISCONNECT    5

#define _STATUS_OFFLINE         0
#define _STATUS_ONLINE          3
#define _STATUS_HIDDEN          6
#define _STATUS_BUSY            8
#define _STATUS_DEPART          9

#define _HANDLE_BEGIN       -1
#define _HANDLE_END         -1
#define _SID_INVALID        0
#define GETUID(rid)   (rid&0xFFFFFFF0)

//邮件类型定义
#define _MA_UNREAD           0x0001
#define _MA_ATTACH_OBJ       0x0002
#define _MA_ATTACH_MONEY     0x0004
#define _MA_PRESERVE         0x0010

enum ErrCode {
	ERR_SUCCESS = 0,            //成功
	ERR_TOBECONTINUE = 1,       //成功，并且还有更多数据未传递完，目前未用

	ERR_INVALID_ACCOUNT = 2,    //帐号不存在
	ERR_INVALID_PASSWORD = 3,   //密码错误
	ERR_TIMEOUT = 4,            //超时
	ERR_INVALID_ARGUMENT = 5,   //参数错误
	ERR_FRIEND_SYNCHRONIZE = 6, //好友信息保存到数据库时无法同步
	ERR_SERVERNOTSUPPLY = 7,    //该服务器不支持该请求
	ERR_COMMUNICATION=8,        //网络通讯错误
	ERR_ACCOUNTLOCKED=9,        //多次重复登陆，当前用户的一个登陆正在被处理，处于锁定状态
	ERR_MULTILOGIN  =10,        //多次重复登陆，且用户选择自动下线
	// keyexchange      
	ERR_INVALID_NONCE    = 11,  //无效的nonce值
	// deliver use
	ERR_DELIVER_SEND     = 21,  //转发失败
	ERR_DELIVER_TIMEOUT  = 22,  //转发超时
	//player login
	ERR_LOGINFAIL   =   31,     //登陆游戏失败
	ERR_KICKOUT     =   32,     //被踢下线
	ERR_CREATEROLE  =   33,     //创建角色失败
	ERR_DELETEROLE  =   34,     //删除角色失败
	ERR_ROLELIST    =   35,     //获得角色列表失败
	ERR_UNDODELROLE =   36,     //撤销删除角色失败
	ERR_LINKISFULL	=	39,		//服务器人数已满
	//add friend
	ERR_ADDFRD_REQUEST     = 51,  //请求加为好友
	ERR_ADDFRD_REFUSE      = 52,  //拒绝加为好友
	ERR_ADDFRD_AGREE       = 53,  //同意加为好友
	ERR_ADDFRD_AGREEANDADD = 54,  //同意并希望将对方加为好友

	//QQ DB retcode
	ERR_FAILED            = 41,
	ERR_EXCEPTION         = 42,
	ERR_NOTFOUND          = 43,
	ERR_INVALIDHANDLE     = 44,
	ERR_DUPLICATRECORD    = 45,
	ERR_NOFREESPACE       = 46,

	//game DB
	ERR_DATANOTFIND            = 60,  //数据不存在        
	ERR_GENERAL		           = 61,
	ERR_PERMISSION_DENIED      = 63, 
	ERR_DATABASE_TIMEOUT       = 64,
	ERR_UNAVAILABLE            = 65,  //已婚人士不能删除角色
	ERR_CMDCOOLING             = 66,  //命令正在冷却中

	ERR_DELROLE_SECT           = 91,  //删除角色前须先解除师徒关系
    //faction error code (101-200)  
    ERR_FC_NETWORKERR       =   101,    //服务器网络通讯错误
    ERR_FC_INVALID_OPERATION=   102,    //无效的操作类型
    ERR_FC_OP_TIMEOUT       =   103,    //操作超时
    ERR_FC_CREATE_ALREADY   =   104,    //玩家已经是某个帮派的成员，不能再创建帮派
    ERR_FC_CREATE_DUP       =   105,    //帮派名称重复
    ERR_FC_DBFAILURE        =   106,    //数据库IO错误
    ERR_FC_NO_PRIVILEGE     =   107,    //没有相关操作的权限
	ERR_FC_INVALIDNAME      =   108,    //不能使用此名称
	ERR_FC_FULL             =   109,    //人数已达上限
    ERR_FC_APPLY_REJOIN     =   110,    //已经是某个帮派的成员，申请失败
    ERR_FC_JOIN_SUCCESS     =   111,    //成功加入帮派
    ERR_FC_JOIN_REFUSE      =   112,    //申请被拒绝
    ERR_FC_ACCEPT_REACCEPT  =   113,    //被批准加入帮派的玩家已经加入帮派
    ERR_FC_FACTION_NOTEXIST =   114,    //帮派不存在or玩家没有申请过加入本帮派
    ERR_FC_NOTAMEMBER       =   115,    //玩家不是本帮派的帮众
    ERR_FC_CHECKCONDITION   =   116,    //不满足操作条件，如SP不够，资金不够
    ERR_FC_DATAERROR        =   117,    //操作参数类型错误，客户端提交的操作参数格式错误
	ERR_FC_OFFLINE          =   118,    //玩家不在线
	ERR_FC_INVITEELEVEL     =   120,    //被邀请方级别不够，不能加入
	ERR_FC_PREDELSUCCESS    =   121,    //帮派解散成功，七天后正式解散
	ERR_FC_DISMISSWAITING   =   122,    //帮派正在解散中
	ERR_FC_INVITEENOFAMILY  =   123,    //被邀请人没有加入家族，不能邀请进入帮派
	ERR_FC_LEAVINGFAMILY    =   124,    //被邀请者离开家族不足七天，不能加入新的家族
	ERR_FC_CHANGENAME_CD	=	125,	//改名冷却中
	ERR_FC_CHANGEKINGDOM	=	126,	//改名冷却中
	ERR_FC_RECRUIT_FAMILY_CD =	127,	//家族入帮冷却中

	//敌对帮派错误代码
	ERR_FC_INFACTION 			=	125,
	ERR_HOSTILE_COOLING			=	126,  // 加为敌对帮派不足10小时，不能撤销敌对状态
	ERR_HOSTILE_ALREADY			=	127,  // 已经是敌对状态
	ERR_HOSTILE_FULL 			=	128,  // 敌对帮派数达到上限
	ERR_HOSTILE_PEER_FULL 		=	129,  // 对方敌对状态数达到上限
	ERR_HOSTILE_LEVEL_LIMIT		=	130,  // 两个帮派等级相差过大，不能进入敌对状态
	ERR_HOSTILE_ITEM 			=	131,  // 没有宣战物品
	ERR_HOSTILE_PROTECTED 		=	132,  // 特殊物品保护，不能进入敌对状态
	ERR_HOSTILE_PEER_PROTECTED 	=	133,  // 对方有特殊物品保护，不能进入敌对状态

	//帮战领取奖金错误码
    ERR_FC_MAILBOXFULL          =   136, // 领取帮战奖金时邮箱已满
	ERR_FC_FACTION_INEXIST      =   137, //没有对应的帮派

	//战场错误代码
	ERR_BATTLE_TEAM_FULL		=	140,	// 阵营已满
	ERR_BATTLE_GAME_SERVER		=	141,	// 不在同一条线
	ERR_BATTLE_JOIN_ALREADY 	=	142,	// 已经加入队伍
	ERR_BATTLE_MAP_NOTEXIST		=	143,	// 没有找到地图
	ERR_BATTLE_COOLDOWN			=	144,	// 离上次战斗时间不足冷却时间，不能报名
	ERR_BATTLE_NOT_INTEAM		=	145,	// 用户不在队伍中
	ERR_BATTLE_LEVEL_LIMIT		=	146,	// 用户不符合战场级别限制
	ERR_BATTLE_CLOSE			=	147,	// 战场没有开放
	ERR_BATTLE_OCCUPATION       =	147,	// 玩家阵营限制
	ERR_BATTLE_QUEUELIMIT       =	148,	// 玩家排队超过限制
	ERR_BATTLE_INFIGHTING       =	149,	// 已经进入战场，不能退出排队
	ERR_BATTLE_REBORN_LIMIT		=	151,	//飞升需求
	ERR_BATTLE_STARTED          =	152,	//宋金战场开启后不能再报名
	ERR_BATTLE_STARTED10        =	153,	//宋金战场开启10分钟后不能再进入战场
	ERR_BATTLE_CROSS			=	166,	//  角色是否有跨服信息
	ERR_BATTLE_ENTER_MAXTIMES	=	168,	// 进入次数到达上限
	ERR_BATTLE_ENTER_NOT_IN_JOINROOM                = 169,  // 首次进入需在报名房间npc处，请联系GM

	//城战错误代码
	ERR_SIEGE_QUEUEFULL 		= 150, // 排队帮派过多
	ERR_SIEGE_FACTIONLIMIT 		= 151, // 帮派级别限制
	ERR_SIEGE_NOTFOUND	 		= 152, // 没有找到战场
	ERR_SIEGE_CHALLENGED		= 153, // 已经报名
	ERR_SIEGE_SELF				= 154, // 不能挑战自己
	ERR_SIEGE_CHECKFAILED		= 155, // 参数检查失败
	ERR_SIEGE_TIMEERROR			= 156, // 现在不能设置辅助帮派
	ERR_SIEGE_MONEYLIMIT		= 157, // 报名失败，金币不足
	ERR_SIEGE_ITEMLIMIT			= 158, // 报名失败，缺少物品
	ERR_SIEGE_FACTIONNOTFOUND	= 159, // 没有找到辅助帮派
	ERR_SIEGE_CANTENTER_NORMALCITY	= 161, //不能进入占领城市
	ERR_SIEGE_CANTENTER_BATTLECITY	= 162, //不能进入战场
	ERR_SIEGE_NOFACTION			= 163, // 没有帮派，不能进行城战
	ERR_SIEGE_ASSISTANT_TIME 	= 164, // 辅助帮成立时间不足
	ERR_SIEGE_ASSISTANT_LEVEL 	= 165, // 辅助帮级别不足
	ERR_SIEGE_ASSISTANT_SELF 	= 166, // 指定自己为辅助帮
	ERR_SIEGE_ASSISTANT_OWNER 	= 167, // 指定守城帮为辅助帮
	ERR_SIEGE_COOLING           = 168, // 距离上次报名不足10分钟
	
	//邮箱系统错误代码
	ERR_MS_DBSVR_INV           =   211, //数据库服务不可连接
	ERR_MS_MAIL_INV            =   212, //邮件不存在
	ERR_MS_ATTACH_INV          =   213, //错误的附件信息
	ERR_MS_SEND_SELF           =   214, //禁止给自己发送邮件
	ERR_MS_ACCOUNTFROZEN       =   215, //目标邮箱已经冻结
	ERR_MS_AGAIN               =   216, //邮箱服务暂时不可用
	ERR_MS_BOXFULL             =   217, //目标邮箱已满

	//拍卖错误代码
	ERR_AS_MAILBOXFULL         =   220, //玩家信箱无剩余空间
	ERR_AS_ITEM_INV            =   221, //错误的拍卖物品信息
	ERR_AS_MARKET_UNOPEN       =   222, //拍卖场未开放（未完成初始化）
	ERR_AS_ID_EXHAUSE          =   223, //拍卖号用尽
	ERR_AS_ATTEND_OVF          =   224, //参与的拍卖数达到上限
	ERR_AS_BID_LOWBID          =   225, //出价过低竞拍失败
	ERR_AS_BID_NOTFOUND        =   226, //未找到该拍卖事件
	ERR_AS_BID_BINSUCCESS      =   227, //一口价买断
	ERR_AS_BID_UNREDEEMABLE    =   228, //不能赎回
	
	ERR_SP_NOT_INIT            =   231, //系统没有初始化完成
    ERR_SP_SPARETIME           =   232, //剩余时间不满足挂售条件
    ERR_SP_INVA_POINT          =   233, //无效的挂售点数，必须是30元的整数倍
    ERR_SP_EXPIRED             =   234, //该点卡已经过期
    ERR_SP_NOMONEY             =   237, //虚拟币不足
    ERR_SP_SELLING             =   239, //点卡已经处于销售状态
    ERR_SP_MONEYEXCEED         =   242, //金钱数达到上限
    ERR_SP_BUYSELF             =   243, //不能购买自己挂售的点卡

    //城战错误代码
    ERR_BS_INVALIDROLE         =   260, //角色身份不符合
    ERR_BS_FAILED              =   261, //竞价失败
    ERR_BS_OUTOFSERVICE        =   262, //城战服务暂时不可用
	ERR_BS_NEWBIE_BANNED       =   263, //加入帮派72小时内不允许进入城战
	ERR_BS_ALREADYBID          =   265, //不能多次竞价
	ERR_BS_NOTBATTLECITY       =   266, //该地图没有开启城战
	ERR_BS_PROCESSBIDDING      =   267, //正在处理请求，请重试
	ERR_BS_BIDSELF             =   268, //不能对自己的领地竞价
	ERR_BS_BIDNOOWNERCITY      =   269, //有领地帮派不能对无主领地竞价
	ERR_BS_NOMONEY             =   270, //金币数不足
	ERR_BS_LEVELNOTENOUGH      =   271, //帮派级别不足
	ERR_BS_RANKNOTENOUGH       =   272, //帮派排名不足
	ERR_BS_CREDITNOTENOUGH     =   273, //帮派贡献度不足
	
	
};

#define _MSG_CONVERSATION   1
#define _MSG_ADDFRD_RQST    2
#define _MSG_ADDFRD_RE      3

#define _INFO_PUBLIC        1
#define _INFO_PRIVATE       2
#define _INFO_PROTECTED     3

enum{
	//Trade
	ERR_TRADE_PARTNER_OFFLINE=  68,     //对方已经下线
	ERR_TRADE_AGREE         =   0,      //同意交易
	ERR_TRADE_REFUSE        =   69,     //对方拒绝交易
	ERR_TRADE_BUSY_TRADER   =   70,     //trader 已经在交易中
	ERR_TRADE_DB_FAILURE    =   71,     //读写数据库失败
	ERR_TRADE_JOIN_IN       =   72,     //加入交易失败，交易对象的双方已经存在
	ERR_TRADE_INVALID_TRADER=   73,     //无效的交易者
	ERR_TRADE_ADDGOODS      =   74,     //增加交易物品失败
	ERR_TRADE_RMVGOODS      =   75,     //减少交易物品失败
	ERR_TRADE_READY_HALF    =   76,     //提交完成一半，等待对方提交
	ERR_TRADE_READY         =   77,     //提交完成
	ERR_TRADE_SUBMIT_FAIL   =   78,     //提交失败
	ERR_TRADE_CONFIRM_FAIL  =   79,     //确认失败
	ERR_TRADE_DONE          =   80,     //交易完成
	ERR_TRADE_HALFDONE      =   81,     //交易完成一半，等待另一方确认
	ERR_TRADE_DISCARDFAIL   =   82,     //取消交易失败
	ERR_TRADE_MOVE_FAIL     =   83,     //移动物品失败
	ERR_TRADE_SPACE         =   84,     //物品栏空间不足
	ERR_TRADE_SETPSSN       =   85,     //设置交易者财产错误
	ERR_TRADE_ATTACH_HALF   =   86,     //成功加入一个一个交易者
	ERR_TRADE_ATTACH_DONE   =   87,     //成功加入两个交易者
};

enum{
	ERR_COMBAT_MASTEROFFLINE = 1,       //对方帮主不在线
	ERR_COMBAT_NOPROSPERITY  = 2,       //帮派繁荣度不足
	ERR_COMBAT_COOLING       = 3,       //挑战命令冷却中
	ERR_COMBAT_BUSY          = 4,       //对方正在野战中
	ERR_COMBAT_LOWLEVEL      = 5,       //对方帮派等级不足2级
	
};

enum ERR_STOCK
{
    ERR_STOCK_CLOSED        = 1,        //元宝交易系统未开放
    ERR_STOCK_ACCOUNTBUSY   = 2,        //相关账户正在进行交易，请稍候
    ERR_STOCK_INVALIDINPUT  = 3,        //输入数据错误
    ERR_STOCK_OVERFLOW      = 4,        //超过金额上限
    ERR_STOCK_DATABASE      = 5,        //数据库忙，请稍候
    ERR_STOCK_NOTENOUGH     = 6,        //账户余额不足
	ERR_STOCK_MAXCOMMISSION = 7,        //交易数目已达上限
	ERR_STOCK_NOTFOUND      = 8,
	ERR_STOCK_CASHLOCKED    = 9,
	ERR_STOCK_CASHUNLOCKFAILED= 10,
};

enum ERR_SECT
{
	ERR_SECT_OFFLINE        = 1,   //玩家不在线
	ERR_SECT_UNAVAILABLE    = 2,   //对方已经拜师
	ERR_SECT_FULL           = 3,   //徒弟数量已经达到上限
	ERR_SECT_BUSY           = 4,   //系统繁忙，请稍后再试
	ERR_SECT_REFUSE         = 5,   //对方拒绝了你的收徒邀请
	ERR_SECT_INVALIDLEVEL   = 6,   //对方级别不满足要求
};

#define _TRADE_END_TIMEOUT  0
#define _TRADE_END_NORMAL   1       

//player logout style
#define _PLAYER_LOGOUT_FULL 0
#define _PLAYER_LOGOUT_HALF 1

#define  _ROLE_STATUS_NORMAL    1
#define  _ROLE_STATUS_MUSTDEL   2
#define  _ROLE_STATUS_READYDEL  3
#define  _ROLE_STATUS_FROZEN	4
#define  _ROLE_STATUS_SELLING	5
enum FS_ERR
{
	// Friend error code (201-250)
	ERR_FS_OFFLINE          =  1,     //玩家不在线
	ERR_FS_REFUSE           =  2,     //被拒绝
	ERR_FS_TIMEOUT          =  3,     //超时
	ERR_FS_NOSPACE          =  4,     //无剩余空间
	ERR_FS_NOFOUND          =  5,     //未找到
	ERR_FS_ERRPARAM         =  6,     //参数错误
	ERR_FS_DUPLICATE        =  7,     //重复
};
enum PRIVATE_CHANNEL
{
	CHANNEL_NORMAL			= 0,		//非好友
	CHANNEL_NORMALRE		= 1,		//非好友自动回复
	CHANNEL_FRIEND			= 2,		//好友
	CHANNEL_FRIEND_RE		= 3,		//好友自动回复
	CHANNEL_USERINFO		= 4,		//好友相关信息
	CHANNEL_GM				= 5,		//used by GM
	CHANNEL_SERVERINFO		= 6,
};

enum AUCTION_INFORM
{
    _AUCTION_ITEM_SOLD,
    _AUCTION_BID_WIN,
    _AUCTION_BID_LOSE,
    _AUCTION_BID_CANCEL,
	_AUCTION_BID_TIMEOUT,   
};

enum BATTLE_INFORM
{
    _BATTLE_BONUS,           // 领地分红
    _BATTLE_WIN_PRIZE,       // 城战胜利,获得奖金
    _BATTLE_BID_FAILED,      // 竞价失败,退还押金
    _BATTLE_BID_WITHDRAW,    // 城战取消,退还押金
};

enum CONSIGN_INFORM
{       
	_CONSIGN_BUY,
	_CONSIGN_SOLD,     
    _CONSIGN_CANCEL,
    _CONSIGN_FORCED_CANCEL,
    _CONSIGN_EXPIRE,
    _CONSIGN_FAIL,
};//sender

enum MAIL_SENDER_TYPE
{
    _MST_PLAYER=0,
    _MST_NPC,
    _MST_AUCTION,
    _MST_WEB,
    _MST_BATTLE,
    _MST_TYPE_NUM,   
	_MST_LOCAL_START = 32,   
    _MST_LOCAL_CONSIGN = _MST_LOCAL_START,	
	_MST_CALLBACK, 
	_MST_CALLBACK_AWARD, 
	_MST_ROLE_CONSIGN_NOTIFY,	// 2012.4.1 角色寄售发给好友通知
    _MST_KINGDOM_AWARD = 37,
	_MST_FACTION_PK_BONUS_NOTIFY = 40, // 2012.9.6 帮战奖金
};

enum //Friend Call Back System
{
ERR_CALLBACK_SUCCESS,					// 成功（客户端）0
ERR_CALLBACK_TIMEOUT,					// 消息处理超时（客户端）1
ERR_CALLBACK_UNVALIDTEMPID,		// 发送召回邮件的邮件模板ID无效 2
ERR_CALLBACK_LIMITNOTINIT,		// 玩家的发送召回邮件限制条件没有加载（客户端）3
ERR_CALLBACK_MSGCOOLDOWN,			// 发送召回邮件的时间间隔过短 4
ERR_CALLBACK_OFFLINE,					// 发送者不在线 5
ERR_CALLBACK_FRIENDONLINE,  	// 召回邮件的接收者在线（客户端）6
ERR_CALLBACK_FDLISTNOTINIT,		// 玩家的好友列表未加载（客户端）7
ERR_CALLBACK_NOTHISFRIEND, 		// 接收者不是发送者的好友（客户端）8
ERR_CALLBACK_MAXMAILNUM,			// 发送者已经达到当天最大发送邮件数（客户端）9
ERR_CALLBACK_COOLDOWN,				// 接收者处于冷却期（客户端）10
ERR_CALLBACK_EXCEPTION,				// 数据库操作异常（客户端）11
ERR_CALLBACK_ACCOUNTFROZEN,		// 召回邮件的接收者为老玩家 12
ERR_CALLBACK_MAILBOXFULL,			// 邮箱已满 13
ERR_CALLBACK_NOAWARD,					// 没有奖励 14
};


}

#endif	//	__GNETDEF_H__
