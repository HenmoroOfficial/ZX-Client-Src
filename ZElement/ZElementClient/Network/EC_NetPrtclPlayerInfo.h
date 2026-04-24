#ifndef _EC_NETPRTCLPLAYERINFO_
#define _EC_NETPRTCLPLAYERINFO_

#include "EC_NetPrtclGame.h"

class CECNetPrtclPlayerInfo : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerInfo() {}
	~CECNetPrtclPlayerInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_INFO_1, true, 1);
	DECL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_ENTER_WORLD, true, 2);
	DECL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_ENTER_SLICE, true, 3);
};

class CECNetPrtclPlayerInfo00 : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerInfo00() {}
	~CECNetPrtclPlayerInfo00() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerInfo00, S2C::PLAYER_INFO_00, true, 1);
};

class CECNetPrtclSelfInfo1 : public CECNetPrtclGame
{
public:
	CECNetPrtclSelfInfo1() {}
	~CECNetPrtclSelfInfo1() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclSelfInfo1, S2C::SELF_INFO_1, true, 1);
};

class CECNetPrtclSelfInfo00 : public CECNetPrtclGame
{
public:
	CECNetPrtclSelfInfo00() {}
	~CECNetPrtclSelfInfo00() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclSelfInfo00, S2C::SELF_INFO_00, true, 1);
};

class CECNetPrtclPlayerList : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerList() {}
	~CECNetPrtclPlayerList() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerList, S2C::PLAYER_INFO_1_LIST, true, 1);
};

class CECNetPrtclPlayerLeaveWorld : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerLeaveWorld() {}
	~CECNetPrtclPlayerLeaveWorld() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerLeaveWorld, S2C::PLAYER_LEAVE_WORLD, true, 1);
};

class CECNetPrtclPlayerRecipeData : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerRecipeData() {}
	~CECNetPrtclPlayerRecipeData() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerRecipeData, S2C::PLAYER_RECIPE_DATA, true, 1);
};

class CECNetPrtclPlayerLearnRecipe : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerLearnRecipe() {}
	~CECNetPrtclPlayerLearnRecipe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerLearnRecipe, S2C::PLAYER_LEARN_RECIPE, true, 1);
};

class CECNetPrtclPlayerProduceInfo : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerProduceInfo() {}
	~CECNetPrtclPlayerProduceInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerProduceInfo, S2C::PLAYER_PRODUCE_SKILL_INFO, true, 1);
};

class CECNetPrtclPlayerRefineResult : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerRefineResult() {}
	~CECNetPrtclPlayerRefineResult() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerRefineResult, S2C::PLAYER_REFINE_RESULT, true, 1);
};

class CECNetPrtclPlayerStartOprt : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerStartOprt() {}
	~CECNetPrtclPlayerStartOprt() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerStartOprt, S2C::PLAYER_START_GENERAL_OPERATION, true, 1);
};

class CECNetPrtclPlayerStopOprt : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerStopOprt() {}
	~CECNetPrtclPlayerStopOprt() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerStopOprt, S2C::PLAYER_STOP_GENERAL_OPERATION, true, 1);
};

class CECNetPrtclPlayerPKLevel : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerPKLevel() {}
	~CECNetPrtclPlayerPKLevel() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerPKLevel, S2C::PK_LEVEL_NOTIFY, true, 1);
};

class CECNetPrtclPlayerPKValue : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerPKValue() {}
	~CECNetPrtclPlayerPKValue() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerPKValue, S2C::PK_VALUE_NOTIFY, true, 1);
};

class CECNetPrtclPlayerClass : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerClass() {}
	~CECNetPrtclPlayerClass() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerClass, S2C::PLAYER_CHANGE_CLASS, true, 1);
};

class CECNetPrtclPlayerCash : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerCash() {}
	~CECNetPrtclPlayerCash() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerCash, S2C::PLAYER_CASH, true, 1);
};

class CECNetPrtclPlayerSkillAddon : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerSkillAddon() {}
	~CECNetPrtclPlayerSkillAddon() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerSkillAddon, S2C::PLAYER_SKILL_ADDON, true, 1);
};

class CECNetPrtclPlayerSkillCommonAddon : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerSkillCommonAddon() {}
	~CECNetPrtclPlayerSkillCommonAddon() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerSkillCommonAddon, S2C::PLAYER_SKILL_COMMON_ADDON, true, 1);
};

class CECNetPrtclPlayerExtraSkill : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerExtraSkill() {}
	~CECNetPrtclPlayerExtraSkill() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerExtraSkill, S2C::PLAYER_EXTRA_SKILL, true, 1);
};

class CECNetPrtclPlayerTitleList : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerTitleList() {}
	~CECNetPrtclPlayerTitleList() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerTitleList, S2C::PLAYER_TITLE_LIST, true, 1);
};

class CECNetPrtclPlayerAddTitle : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerAddTitle() {}
	~CECNetPrtclPlayerAddTitle() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerAddTitle, S2C::PLAYER_ADD_TITLE, true, 1);
};

class CECNetPrtclPlayerDelTitle : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerDelTitle() {}
	~CECNetPrtclPlayerDelTitle() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerDelTitle, S2C::PLAYER_DEL_TITLE, true, 1);
};

class CECNetPrtclPlayerRegionReputation : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerRegionReputation() {}
	~CECNetPrtclPlayerRegionReputation() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerRegionReputation, S2C::PLAYER_REGION_REPUTATION, true, 1);
};

class CECNetPrtclPlayerChangeRegionReputation : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerChangeRegionReputation() {}
	~CECNetPrtclPlayerChangeRegionReputation() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerChangeRegionReputation, S2C::PLAYER_CHANGE_REGION_REPUTATION, true, 1);
};

class CECNetPrtclPlayerChangeTitle : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerChangeTitle() {}
	~CECNetPrtclPlayerChangeTitle() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerChangeTitle, S2C::PLAYER_CHANGE_TITLE, true, 1);
};

class CECNetPrtclPlayerReputation : public CECNetPrtclGame
{
public:
	CECNetPrtclPlayerReputation() {}
	~CECNetPrtclPlayerReputation() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclPlayerReputation, S2C::PLAYER_REPUTATION, true, 1);
};

class CECNetPrtclChangeInventorySize : public CECNetPrtclGame
{
public:
	CECNetPrtclChangeInventorySize() {}
	~CECNetPrtclChangeInventorySize() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclChangeInventorySize, S2C::PLAYER_CHANGE_INVENTORY_SIZE, true, 1);
};

class CECNetPrtcPlayerBindSuccess : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerBindSuccess() {}
	~CECNetPrtcPlayerBindSuccess() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerBindSuccess, S2C::PLAYER_BIND_SUCCESS, true, 1);
};

class CECNetPrtcPlayerChangeSpouse : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerChangeSpouse() {}
	~CECNetPrtcPlayerChangeSpouse() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerChangeSpouse, S2C::PLAYER_CHANGE_SPOUSE, true, 1);
};

class CECNetPrtcPlayerInvaderState : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerInvaderState() {}
	~CECNetPrtcPlayerInvaderState() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerInvaderState, S2C::PLAYER_INVADER_STATE, true, 1);
};

class CECNetPrtcPlayerContribution : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerContribution() {}
	~CECNetPrtcPlayerContribution() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerContribution, S2C::PLAYER_MAFIA_CONTRIBUTION, true, 1);
};

class CECNetPrtcPlayerLotteryBonus : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLotteryBonus() {}
	~CECNetPrtcPlayerLotteryBonus() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLotteryBonus, S2C::LOTTERY_BONUS, true, 1);
};


class CECNetPrtcPlayerLottery2Bonus : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLottery2Bonus() {}
	~CECNetPrtcPlayerLottery2Bonus() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLottery2Bonus, S2C::LOTTERY2_BONUS, true, 1);
};

//新彩票的相关奖励协议 Added 2011-07-28.
class CECNetPrtcPlayerLottery3Bonus : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLottery3Bonus() {}
	~CECNetPrtcPlayerLottery3Bonus() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLottery3Bonus, S2C::LOTTERY3_BONUS, true, 1);
};

//Added 2011-08-03.
//gs通知客户端玩家进入副本（副本帮派、类型、id、结束时间） 
class CECNetPrtcPlayerEnterRaid : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerEnterRaid() {}
	~CECNetPrtcPlayerEnterRaid() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerEnterRaid, S2C::ENTER_RAID, true, 1);
};

//gs通知客户端玩家离开副本消息协议 （仅仅一个命令，无其他消息字段）
class CECNetPrtcPlayerLeaveRaid : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLeaveRaid() {}
	~CECNetPrtcPlayerLeaveRaid() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLeaveRaid, S2C::LEAVE_RAID, true, 1);
};

//gs通知客户端副本boss击杀进度
class CECNetPrtcDungeonProgress : public CECNetPrtclGame
{
public:
	CECNetPrtcDungeonProgress() {}
	~CECNetPrtcDungeonProgress(){}
public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcDungeonProgress, S2C::DUNGEON_PROGRESS,	true, 1);
};

//gs通知客户端触发技能时间相关消息协议
class CECNetPrtcPlayerTriggerSkillTime : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerTriggerSkillTime() {}
	~CECNetPrtcPlayerTriggerSkillTime() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerTriggerSkillTime, S2C::TRIGGER_SKILL_TIME, true, 1);
};
//Added end.

//Added 2012-08-01.
//gs通知客户端玩家进入碰撞副本(变身ID，所选技能，副本ID等)
class CECNetPrtcPlayerEnterCollisionRaid : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerEnterCollisionRaid() {}
	~CECNetPrtcPlayerEnterCollisionRaid() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerEnterCollisionRaid, S2C::ENTER_COLLISION_RAID, true, 1);
};

//gs通知客户端当前碰撞副本红蓝双方的比分
class CECNetPrtcPlayerCollisionRaidScore : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerCollisionRaidScore() {}
	~CECNetPrtcPlayerCollisionRaidScore() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerCollisionRaidScore, S2C::COLLISION_RAID_SCORE, true, 1);
};
//Added end.

//gs通知客户端十神宝窟的副本信息
class CECNetPrtcDungeonSskInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcDungeonSskInfo() {}
	~CECNetPrtcDungeonSskInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcDungeonSskInfo, S2C::BATTLE_DUNGEON_SSK_INFO, true, 1);
};
//Added end.

//gs通知客户端爬塔副本中开始选择怪
class CECNetPrtcPlayerStartSelectMonster : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerStartSelectMonster() {}
	~CECNetPrtcPlayerStartSelectMonster() {}
	
public:
	virtual bool Process(CECGameSession* pSession);
	
	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerStartSelectMonster, S2C::RANDOM_TOWER_MONSTER, true, 1);
};

//gs通知客户端爬塔副本中幸运技能
class CECNetPrtcPlayerTowerGiveSkills : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerTowerGiveSkills() {}
	~CECNetPrtcPlayerTowerGiveSkills() {}
	
public:
	virtual bool Process(CECGameSession* pSession);
	
	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerTowerGiveSkills, S2C::PLAYER_TOWER_GIVE_SKILLS, true, 1);
};

//处理服务器发来的玩家宝石升品等返回结果信息协议Added 2011-03-22.
class CECNetPrtcPlayerGemNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerGemNotify() {}
	~CECNetPrtcPlayerGemNotify() {}

public:
	virtual bool Process( CECGameSession* pSession );

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerGemNotify, S2C::GEM_NOTIFY, true, 1);
};

//处理服务器发来的通知各满足条件的队员开始结阵的广播消息协议 Added 2011-06-17.
class CECNetPrtcCircleOfDoomStartup : public CECNetPrtclGame
{
public:
	CECNetPrtcCircleOfDoomStartup() {}
	~CECNetPrtcCircleOfDoomStartup() {}

public:
	virtual bool Process( CECGameSession* pSession );

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCircleOfDoomStartup, S2C::CIRCLE_OF_DOOM_STARTUP, true, 1);
};

//处理服务器发来的更新阵法信息的协议 Added 2011-06-22.
class CECNetPrtcCircleOfDoomInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcCircleOfDoomInfo() {}
	~CECNetPrtcCircleOfDoomInfo() {}

public:
	virtual bool Process( CECGameSession* pSession );

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCircleOfDoomInfo, S2C::CIRCLE_OF_DOOM_INFO, true, 1);
};

//处理服务器发来的当前世界中某阵法结束的协议 Added 2011-06-22.
class CECNetPrtcCircleOfDoomStop : public CECNetPrtclGame
{
public:
	CECNetPrtcCircleOfDoomStop() {}
	~CECNetPrtcCircleOfDoomStop() {}

public:
	virtual bool Process( CECGameSession* pSession );

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCircleOfDoomStop, S2C::CIRCLE_OF_DOOM_STOP, true, 1);
};

//处理服务器发来的当前可视区域中某火焰囚牢的信息 Added 2011-07-08.
class CECNetPrtcJailCircleArea : public CECNetPrtclGame
{
public:
	CECNetPrtcJailCircleArea() {}
	~CECNetPrtcJailCircleArea() {}

public:
	virtual bool Process( CECGameSession* pSession );

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcJailCircleArea, S2C::PLAYER_JAIL_CIRCLE_AREA, true, 1);
};

class CECNetPrtcPlayerChangeName : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerChangeName() {}
	~CECNetPrtcPlayerChangeName() {}
	
public:
	virtual bool Process(CECGameSession* pSession);
	
	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerChangeName, S2C::PLAYER_CHANGE_NAME, true, 1);
};

class CECNetPrtcServerTime : public CECNetPrtclGame
{
public:
	CECNetPrtcServerTime() {}
	~CECNetPrtcServerTime() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcServerTime, S2C::SERVER_TIME, true, 1);
};

class CECNetPrtcTalismanExpNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcTalismanExpNotify() {}
	~CECNetPrtcTalismanExpNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcTalismanExpNotify, S2C::TALISMAN_EXP_NOTIFY, true, 1);
};

class CECNetPrtcTalismanMergeNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcTalismanMergeNotify() {}
	~CECNetPrtcTalismanMergeNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcTalismanMergeNotify, S2C::TALISMAN_MERGE_NOTIFY, true, 1);
};

class CECNetPrtcTalismanValueNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcTalismanValueNotify() {}
	~CECNetPrtcTalismanValueNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcTalismanValueNotify, S2C::TALISMAN_VALUE_NOTIFY, true, 1);
};

class CECNetPrtcBotBeginNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcBotBeginNotify() {}
	~CECNetPrtcBotBeginNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcBotBeginNotify, S2C::BOT_BEGIN_NOTIFY, true, 1);
};

class CECNetPrtcTalentNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcTalentNotify() {}
	~CECNetPrtcTalentNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcTalentNotify, S2C::TALENT_NOTIFY, true, 1);
};

class CECNetPrtcNotifySafeLock : public CECNetPrtclGame
{
public:
	CECNetPrtcNotifySafeLock() {}
	~CECNetPrtcNotifySafeLock() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcNotifySafeLock, S2C::NOTIFY_SAFE_LOCK, true, 1);
};

class CECNetPrtcWarInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcWarInfo() {}
	~CECNetPrtcWarInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarInfo, S2C::WAR_INFO, true, 1);
};

class CECNetPrtcEnterWar : public CECNetPrtclGame
{
public:
	CECNetPrtcEnterWar() {}
	~CECNetPrtcEnterWar() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcEnterWar, S2C::ENTER_WAR, true, 1);
};

class CECNetPrtcLeaveWar : public CECNetPrtclGame
{
public:
	CECNetPrtcLeaveWar() {}
	~CECNetPrtcLeaveWar() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcLeaveWar, S2C::LEAVE_WAR, true, 1);
};

class CECNetPrtcWarStart : public CECNetPrtclGame
{
public:
	CECNetPrtcWarStart() {}
	~CECNetPrtcWarStart() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarStart, S2C::WAR_START, true, 1);
};

class CECNetPrtcWarEnd : public CECNetPrtclGame
{
public:
	CECNetPrtcWarEnd() {}
	~CECNetPrtcWarEnd() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarEnd, S2C::WAR_END, true, 1);
};

class CECNetPrtcWarConstructionInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcWarConstructionInfo() {}
	~CECNetPrtcWarConstructionInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarConstructionInfo, S2C::WAR_CONSTRUCTION_INFO, true, 1);
};

class CECNetPrtcWarContributionInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcWarContributionInfo() {}
	~CECNetPrtcWarContributionInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarContributionInfo, S2C::WAR_CONTRIBUTION_INFO, true, 1);
};

class CECNetPrtcWarBuildingStatusChange : public CECNetPrtclGame
{
public:
	CECNetPrtcWarBuildingStatusChange() {}
	~CECNetPrtcWarBuildingStatusChange() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarBuildingStatusChange, S2C::WAR_BUILDING_STATUS_CHANGE, true, 1);
};

class CECNetPrtcPlayerRebornInfo : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerRebornInfo() {}
	~CECNetPrtcPlayerRebornInfo() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerRebornInfo, S2C::PLAYER_REBORN_INFO, true, 1);
};

class CECNetPrtcScriptMessage : public CECNetPrtclGame
{
public:
	CECNetPrtcScriptMessage() {}
	~CECNetPrtcScriptMessage() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcScriptMessage, S2C::SCRIPT_MESSAGE, true, 1);
};

class CECNetPrtcPlayerPetCivilization : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerPetCivilization() {}
	~CECNetPrtcPlayerPetCivilization() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerPetCivilization, S2C::PLAYER_PET_CIVILIZATION, true, 1);
};

class CECNetPrtcPlayerPetConstruction : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerPetConstruction() {}
	~CECNetPrtcPlayerPetConstruction() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerPetConstruction, S2C::PLAYER_PET_CONSTRUCTION, true, 1);
};

class CECNetPrtcPlayerStartFly : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerStartFly() {}
	~CECNetPrtcPlayerStartFly() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerStartFly, S2C::PLAYER_START_FLY, true, 1);
};

class CECNetPrtcPlayerPetPropAdded : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerPetPropAdded() {}
	~CECNetPrtcPlayerPetPropAdded() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerPetPropAdded, S2C::PLAYER_PET_PROP_ADDED, true, 1);
};

class CECNetPrtcPlayerSectBecomeDisciple : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerSectBecomeDisciple() {}
	~CECNetPrtcPlayerSectBecomeDisciple() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerSectBecomeDisciple, S2C::SECT_BECOME_DISCIPLE, true, 1);
};

class CECNetPrtcPlayerCultivationNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerCultivationNotify() {}
	~CECNetPrtcPlayerCultivationNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerCultivationNotify, S2C::CULTIVATION_NOTIFY, true, 1);
};

class CECNetPrtcOfflineAgentBonus : public CECNetPrtclGame
{
public:
	CECNetPrtcOfflineAgentBonus() {}
	~CECNetPrtcOfflineAgentBonus() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcOfflineAgentBonus, S2C::OFFLINE_AGENT_BONUS, true, 1);
};

class CECNetPrtcOfflineAgentTime : public CECNetPrtclGame
{
public:
	CECNetPrtcOfflineAgentTime() {}
	~CECNetPrtcOfflineAgentTime() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcOfflineAgentTime, S2C::OFFLINE_AGENT_TIME, true, 1);
};

class CECNetPrtcSelfBuffNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcSelfBuffNotify() {}
	~CECNetPrtcSelfBuffNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSelfBuffNotify, S2C::SELF_BUFF_NOTIFY, true, 1);
};

class CECNetPrtcPeepInfoNotify : public CECNetPrtclGame
{
public:
	CECNetPrtcPeepInfoNotify() {}
	~CECNetPrtcPeepInfoNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPeepInfoNotify, S2C::PEEP_INFO, true, 1);
};

class CECNetPrtcPlayerBonus : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerBonus() {}
	~CECNetPrtcPlayerBonus() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerBonus, S2C::PLAYER_BONUS, true, 1);
};

class CECNetPrtcMilitaryRankChange : public CECNetPrtclGame
{
public:
	CECNetPrtcMilitaryRankChange() {}
	~CECNetPrtcMilitaryRankChange() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcMilitaryRankChange, S2C::MILITARY_RANK_CHANGE, true, 1);
};

class CECNetPrtcPlayerChangeFace : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerChangeFace() {}
	~CECNetPrtcPlayerChangeFace() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerChangeFace, S2C::PLAYER_CHANGE_FACE, true, 1);
};

class CECNetPrtcPlayerChangeVIPState : public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerChangeVIPState() {}
	~CECNetPrtcPlayerChangeVIPState() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerChangeVIPState, S2C::PLAYER_CHANGE_VIPSTATE, true, 1);
};

class CECNetPrtcPlayerReceiveBonusExp: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerReceiveBonusExp() {}
	~CECNetPrtcPlayerReceiveBonusExp() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerReceiveBonusExp, S2C::RECEIVE_BONUS_EXP, true, 1);
};

class CECNetPrtcPlayerAchievementMap: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerAchievementMap() {}
	~CECNetPrtcPlayerAchievementMap() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerAchievementMap, S2C::PLAYER_ACHIEVEMENT_MAP, true, 1);
};

class CECNetPrtcPlayerTransfigure: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerTransfigure() {}
	~CECNetPrtcPlayerTransfigure() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerTransfigure, S2C::PLAYER_START_TRANSFIGURE, true, 1);
};

class CECNetPrtcNotifyMallStrategy: public CECNetPrtclGame
{
public:
	CECNetPrtcNotifyMallStrategy() {}
	~CECNetPrtcNotifyMallStrategy() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcNotifyMallStrategy, S2C::NOTIFY_MALL_STRATEGY, true, 1);
};

class CECNetPrtcLocalPKMessage: public CECNetPrtclGame
{
public:
	CECNetPrtcLocalPKMessage() {}
	~CECNetPrtcLocalPKMessage() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcLocalPKMessage, S2C::LOCAL_PK_MESSAGE, true, 1);
};

// 更新连续技状态
class CECNetPrtcUpdateComboState: public CECNetPrtclGame
{
public:
	CECNetPrtcUpdateComboState() {}
	~CECNetPrtcUpdateComboState() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcUpdateComboState, S2C::UPDATE_COMBO_STATE, true, 1);
};

class CECNetPrtcPlayerInvisible: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerInvisible() {}
	~CECNetPrtcPlayerInvisible() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerInvisible, S2C::ENTER_INVISIBLE_STATE, true, 1);
};

class CECNetPrtcPlayerEnterTerritoryBattle: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerEnterTerritoryBattle() {}
	~CECNetPrtcPlayerEnterTerritoryBattle() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerEnterTerritoryBattle, S2C::ENTER_TERRITORY_BATTLE, true, 1);
};

class CECNetPrtcPlayerLeaveTerritoryBattle: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLeaveTerritoryBattle() {}
	~CECNetPrtcPlayerLeaveTerritoryBattle() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLeaveTerritoryBattle, S2C::LEAVE_TERRITORY_BATTLE, true, 1);
};

class CECNetPrtcPlayerLastLogoutTime: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerLastLogoutTime() {}
	~CECNetPrtcPlayerLastLogoutTime() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerLastLogoutTime, S2C::LAST_LOGOUT_TIME, true, 1);
};

class CECNetPrtcPlayerDeityLevelUp: public CECNetPrtclGame
{
public:
	CECNetPrtcPlayerDeityLevelUp() {}
	~CECNetPrtcPlayerDeityLevelUp() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPlayerDeityLevelUp, S2C::DEITY_LEVELUP, true, 1);
};

#endif
