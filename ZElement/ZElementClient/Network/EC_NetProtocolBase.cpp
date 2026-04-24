#include "hashmap.h"
#include "EC_Global.h"
#include "gnet.h"
#include "EC_NetProtocolBase.h"
#include "EC_NetRawProtocol.h"

#include "EC_NetPrtclPlayerInfo.h"
#include "EC_NetPrtclNPCInfo.h"
#include "EC_NetPrtclObjectInfo.h"
#include "EC_NetProtocol.h"

typedef CECNetProtocolBase* (*NET_PROTOCOL_CREATE_FUNC)(GNET::Protocol* pSrc);
typedef abase::hash_map<int, NET_PROTOCOL_CREATE_FUNC> NetProtocolStubMap;

using namespace S2C;

// game command protocol stub

class CECGameProtocolStub
{
public:
	CECGameProtocolStub();
	~CECGameProtocolStub() {}

public:

	NetProtocolStubMap m_StubMap;
};

CECGameProtocolStub::CECGameProtocolStub()
{
}

static CECGameProtocolStub _GameProtocolStub;

CECNetProtocolBase* _CreateNetGameProtocolFrom(GNET::Protocol* pSrc)
{
	GNET::Octets& Data = static_cast<GamedataSend*>(pSrc)->data;
	DWORD dwDataSize = Data.size();

	if (dwDataSize < sizeof(cmd_header))
	{
		assert(false);
		return 0;
	}

	cmd_header* header = static_cast<cmd_header*>(Data.begin());
	NetProtocolStubMap::iterator it = _GameProtocolStub.m_StubMap.find(header->cmd);

	if (it == _GameProtocolStub.m_StubMap.end())
		return CECNetRawProtocol::CreateRaw(pSrc);
	else
		return (it->second)(pSrc);
}

// totol net protocol stub

class CECNetProtocolStub
{
public:
	CECNetProtocolStub();
	~CECNetProtocolStub() {}

public:

	NetProtocolStubMap m_StubMap;
};

// Register other protocol creation function here

CECNetProtocolStub::CECNetProtocolStub()
{
	// game protocol here
	m_StubMap[PROTOCOL_GAMEDATASEND] = _CreateNetGameProtocolFrom;
}

static CECNetProtocolStub _NetProtocolStub;

template<class T, int id, bool game_prtcl>
NetPrtclRegisterStub<T, id, game_prtcl>::NetPrtclRegisterStub()
{
	if (game_prtcl)
		_GameProtocolStub.m_StubMap[id] = T::_Create;
	else
		_NetProtocolStub.m_StubMap[id] = T::_Create;
}

// player stubs
IMPL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_INFO_1, true, 1);
IMPL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_ENTER_WORLD, true, 2);
IMPL_STUB(CECNetPrtclPlayerInfo, S2C::PLAYER_ENTER_SLICE, true, 3);
IMPL_STUB(CECNetPrtclPlayerInfo00, S2C::PLAYER_INFO_00, true, 1);
IMPL_STUB(CECNetPrtclSelfInfo1, S2C::SELF_INFO_1, true, 1);
IMPL_STUB(CECNetPrtclSelfInfo00, S2C::SELF_INFO_00, true, 1);
IMPL_STUB(CECNetPrtclPlayerList, S2C::PLAYER_INFO_1_LIST, true, 1);
IMPL_STUB(CECNetPrtclPlayerLeaveWorld, S2C::PLAYER_LEAVE_WORLD, true, 1);
IMPL_STUB(CECNetPrtclPlayerRecipeData, S2C::PLAYER_RECIPE_DATA, true, 1);
IMPL_STUB(CECNetPrtclPlayerLearnRecipe, S2C::PLAYER_LEARN_RECIPE, true, 1);
IMPL_STUB(CECNetPrtclPlayerProduceInfo, S2C::PLAYER_PRODUCE_SKILL_INFO, true, 1);
IMPL_STUB(CECNetPrtclPlayerRefineResult, S2C::PLAYER_REFINE_RESULT, true, 1);
IMPL_STUB(CECNetPrtclPlayerStartOprt, S2C::PLAYER_START_GENERAL_OPERATION, true, 1);
IMPL_STUB(CECNetPrtclPlayerStopOprt,S2C:: PLAYER_STOP_GENERAL_OPERATION, true, 1);
IMPL_STUB(CECNetPrtclPlayerPKLevel, S2C::PK_LEVEL_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtclPlayerPKValue, S2C::PK_VALUE_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtclPlayerClass, S2C::PLAYER_CHANGE_CLASS, true, 1);
IMPL_STUB(CECNetPrtclPlayerCash, S2C::PLAYER_CASH, true, 1);
IMPL_STUB(CECNetPrtclPlayerSkillAddon, S2C::PLAYER_SKILL_ADDON, true, 1);
IMPL_STUB(CECNetPrtclPlayerSkillCommonAddon, S2C::PLAYER_SKILL_COMMON_ADDON, true, 1);
IMPL_STUB(CECNetPrtclPlayerExtraSkill, S2C::PLAYER_EXTRA_SKILL, true, 1);
IMPL_STUB(CECNetPrtclPlayerTitleList, S2C::PLAYER_TITLE_LIST, true, 1);
IMPL_STUB(CECNetPrtclPlayerAddTitle, S2C::PLAYER_ADD_TITLE, true, 1);
IMPL_STUB(CECNetPrtclPlayerDelTitle, S2C::PLAYER_DEL_TITLE, true, 1);
IMPL_STUB(CECNetPrtclPlayerRegionReputation, S2C::PLAYER_REGION_REPUTATION, true, 1);
IMPL_STUB(CECNetPrtclPlayerChangeRegionReputation, S2C::PLAYER_CHANGE_REGION_REPUTATION, true, 1);
IMPL_STUB(CECNetPrtclPlayerChangeTitle, S2C::PLAYER_CHANGE_TITLE, true, 1);
IMPL_STUB(CECNetPrtclPlayerReputation, S2C::PLAYER_REPUTATION, true, 1);
IMPL_STUB(CECNetPrtclChangeInventorySize, S2C::PLAYER_CHANGE_INVENTORY_SIZE, true, 1);
IMPL_STUB(CECNetPrtcPlayerBindSuccess, S2C::PLAYER_BIND_SUCCESS, true, 1);
IMPL_STUB(CECNetPrtcPlayerChangeSpouse, S2C::PLAYER_CHANGE_SPOUSE, true, 1);
IMPL_STUB(CECNetPrtcPlayerInvaderState, S2C::PLAYER_INVADER_STATE, true, 1);
IMPL_STUB(CECNetPrtcPlayerContribution, S2C::PLAYER_MAFIA_CONTRIBUTION, true, 1);
IMPL_STUB(CECNetPrtcPlayerLotteryBonus, S2C::LOTTERY_BONUS, true, 1);
IMPL_STUB(CECNetPrtcServerTime, S2C::SERVER_TIME, true, 1);
IMPL_STUB(CECNetPrtcTalismanExpNotify, S2C::TALISMAN_EXP_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcTalismanMergeNotify, S2C::TALISMAN_MERGE_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcTalismanValueNotify, S2C::TALISMAN_VALUE_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcBotBeginNotify, S2C::BOT_BEGIN_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcTalentNotify, S2C::TALENT_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcNotifySafeLock, S2C::NOTIFY_SAFE_LOCK, true, 1);
IMPL_STUB(CECNetPrtcEnterWar, S2C::ENTER_WAR, true, 1);
IMPL_STUB(CECNetPrtcLeaveWar, S2C::LEAVE_WAR, true, 1);
IMPL_STUB(CECNetPrtcWarInfo, S2C::WAR_INFO, true, 1);
IMPL_STUB(CECNetPrtcWarStart, S2C::WAR_START, true, 1);
IMPL_STUB(CECNetPrtcWarEnd, S2C::WAR_END, true, 1);
IMPL_STUB(CECNetPrtcWarConstructionInfo, S2C::WAR_CONSTRUCTION_INFO, true, 1);
IMPL_STUB(CECNetPrtcWarContributionInfo, S2C::WAR_CONTRIBUTION_INFO, true, 1);
IMPL_STUB(CECNetPrtcWarBuildingStatusChange, S2C::WAR_BUILDING_STATUS_CHANGE, true, 1);
IMPL_STUB(CECNetPrtcPlayerRebornInfo, S2C::PLAYER_REBORN_INFO, true, 1);
IMPL_STUB(CECNetPrtcScriptMessage, S2C::SCRIPT_MESSAGE, true, 1);
IMPL_STUB(CECNetPrtcPlayerPetCivilization, S2C::PLAYER_PET_CIVILIZATION, true, 1);
IMPL_STUB(CECNetPrtcPlayerPetConstruction, S2C::PLAYER_PET_CONSTRUCTION, true, 1);
IMPL_STUB(CECNetPrtcPlayerStartFly, S2C::PLAYER_START_FLY, true, 1);
IMPL_STUB(CECNetPrtcPlayerPetPropAdded, S2C::PLAYER_PET_PROP_ADDED, true, 1);
IMPL_STUB(CECNetPrtcPlayerSectBecomeDisciple, S2C::SECT_BECOME_DISCIPLE, true, 1);
IMPL_STUB(CECNetPrtcPlayerCultivationNotify, S2C::CULTIVATION_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcOfflineAgentBonus, S2C::OFFLINE_AGENT_BONUS, true, 1);
IMPL_STUB(CECNetPrtcOfflineAgentTime, S2C::OFFLINE_AGENT_TIME, true, 1);
IMPL_STUB(CECNetPrtcSelfBuffNotify, S2C::SELF_BUFF_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcPeepInfoNotify, S2C::PEEP_INFO, true, 1);
IMPL_STUB(CECNetPrtcPlayerBonus, S2C::PLAYER_BONUS, true, 1);
IMPL_STUB(CECNetPrtcMilitaryRankChange, S2C::MILITARY_RANK_CHANGE, true, 1);
IMPL_STUB(CECNetPrtcPlayerChangeFace, S2C::PLAYER_CHANGE_FACE, true, 1);
IMPL_STUB(CECNetPrtcPlayerChangeVIPState, S2C::PLAYER_CHANGE_VIPSTATE, true, 1);
IMPL_STUB(CECNetPrtcPlayerReceiveBonusExp, S2C::RECEIVE_BONUS_EXP, true, 1);
IMPL_STUB(CECNetPrtcPlayerAchievementMap, S2C::PLAYER_ACHIEVEMENT_MAP, true, 1);
IMPL_STUB(CECNetPrtcPlayerTransfigure, S2C::PLAYER_START_TRANSFIGURE, true, 1);
IMPL_STUB(CECNetPrtcNotifyMallStrategy, S2C::NOTIFY_MALL_STRATEGY, true, 1);
IMPL_STUB(CECNetPrtcLocalPKMessage, S2C::LOCAL_PK_MESSAGE, true, 1);
IMPL_STUB(CECNetPrtcUpdateComboState, S2C::UPDATE_COMBO_STATE, true,1);
IMPL_STUB(CECNetPrtcPlayerInvisible,S2C::ENTER_INVISIBLE_STATE, true,1);
IMPL_STUB(CECNetPrtcPlayerEnterTerritoryBattle,S2C::ENTER_TERRITORY_BATTLE, true,1);
IMPL_STUB(CECNetPrtcPlayerLeaveTerritoryBattle,S2C::LEAVE_TERRITORY_BATTLE, true,1);
IMPL_STUB(CECNetPrtcPlayerLastLogoutTime,S2C::LAST_LOGOUT_TIME, true,1);
IMPL_STUB(CECNetPrtcPlayerDeityLevelUp,S2C::DEITY_LEVELUP, true,1);
IMPL_STUB(CECNetPrtcPlayerLottery2Bonus, S2C::LOTTERY2_BONUS, true, 1);
IMPL_STUB(CECNetPrtcPlayerGemNotify, S2C::GEM_NOTIFY, true, 1);
IMPL_STUB(CECNetPrtcCircleOfDoomStartup, S2C::CIRCLE_OF_DOOM_STARTUP, true, 1); //Added 2011-06-22.
IMPL_STUB(CECNetPrtcCircleOfDoomInfo, S2C::CIRCLE_OF_DOOM_INFO, true, 1); //Added 2011-06-22.
IMPL_STUB(CECNetPrtcCircleOfDoomStop, S2C::CIRCLE_OF_DOOM_STOP, true, 1); //Added 2011-06-22.
IMPL_STUB(CECNetPrtcJailCircleArea, S2C::PLAYER_JAIL_CIRCLE_AREA, true, 1); //Added 2011-07-08.
IMPL_STUB(CECNetPrtcPlayerLottery3Bonus, S2C::LOTTERY3_BONUS, true, 1); //Added 2011-07-28.
IMPL_STUB(CECNetPrtcPlayerEnterRaid, S2C::ENTER_RAID, true, 1);	//Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerLeaveRaid, S2C::LEAVE_RAID, true, 1);	//Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerTriggerSkillTime, S2C::TRIGGER_SKILL_TIME, true, 1); //Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerStartSelectMonster, S2C::RANDOM_TOWER_MONSTER, true, 1); //Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerTowerGiveSkills, S2C::PLAYER_TOWER_GIVE_SKILLS, true, 1); //Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerChangeName, S2C::PLAYER_CHANGE_NAME, true, 1); //Added 2011-08-03.
IMPL_STUB(CECNetPrtcPlayerEnterCollisionRaid, S2C::ENTER_COLLISION_RAID, true, 1); //Added 2012-08-01.
IMPL_STUB(CECNetPrtcPlayerCollisionRaidScore, S2C::COLLISION_RAID_SCORE, true, 1); //Added 2012-08-01.
IMPL_STUB(CECNetPrtcDungeonProgress, S2C::DUNGEON_PROGRESS, true, 1);	//Added 2013-08-23.
IMPL_STUB(CECNetPrtcDungeonSskInfo, S2C::BATTLE_DUNGEON_SSK_INFO, true, 1); // Added 2013-09-02.

// npc stubs
IMPL_STUB(CECNetPrtclNPCInfo, S2C::NPC_INFO_LIST, true, 1);
IMPL_STUB(CECNetPrtclNPCInfo, S2C::NPC_ENTER_SLICE, true, 2);
IMPL_STUB(CECNetPrtclNPCInfo, S2C::NPC_ENTER_WORLD, true, 3);
IMPL_STUB(CECNetPrtclNPCInfo, S2C::NPC_INFO_00, true, 4);
IMPL_STUB(CECNetPrtclNPCDie, S2C::NPC_DIED, true, 1);
IMPL_STUB(CECNetPrtclNPCDie2, S2C::NPC_DIED2, true, 1);

// object stubs
IMPL_STUB(CECNetPrtclObjectMove, S2C::OBJECT_MOVE, true, 1);
IMPL_STUB(CECNetPrtclObjectLeaveSlice, S2C::OBJECT_LEAVE_SLICE, true, 1);
IMPL_STUB(CECNetPrtclObjectDisappear, S2C::OBJECT_DISAPPEAR, true, 1);
IMPL_STUB(CECNetPrtclObjectOutOfSight, S2C::OUT_OF_SIGHT_LIST, true, 1);
IMPL_STUB(CECNetPrtclObjectEnterDim, S2C::ENTER_DIM_STATE, true,1);
IMPL_STUB(CECNetPrtclRecordStart, S2C::RECORD_START, true, 1);
IMPL_STUB(CECNetPrtcNPCServiceResult, S2C::NPC_SERVICE_RESULT, true, 1);
IMPL_STUB(CECNetPrtcBattleFlagChange, S2C::BATTLE_FLAG_CHANGE, true, 1);
IMPL_STUB(CECNetPrtcBattleStart, S2C::BATTLE_START, true, 1);
IMPL_STUB(CECNetPrtcBattleFlagChangeStatus, S2C::BATTLE_FLAG_CHANGE_STATUS, true, 1);
// IMPL_STUB(CECNetPrtcPVPMode, S2C::PVP_MODE, true, 1);
IMPL_STUB(CECNetPrtcAntiCheatBonus, S2C::ANTI_CHEAT_BONUS, true, 1);
IMPL_STUB(CECNetPrtcObjectSpecialState,S2C::OBJECT_SPECIAL_STATE, true,1);

// net protocols
IMPL_STUB(CECNetPrtclLineList, GNET::PROTOCOL_LINELIST, false, 1);
IMPL_STUB(CECNetPrtclGetTopTableRe, GNET::PROTOCOL_GETTOPTABLE_RE, false, 1);
IMPL_STUB(CECNetPrtclGetEnemiesRe, GNET::PROTOCOL_GETENEMIES_RE, false, 1);
IMPL_STUB(CECNetPrtclUpdateEnemyRe, GNET::PROTOCOL_UPDATEENEMY_RE, false, 1);
IMPL_STUB(CECNetPrtcStockAccountRe, GNET::PROTOCOL_STOCKACCOUNT_RE, false, 1);
IMPL_STUB(CECNetPrtcStockBillRe, GNET::PROTOCOL_STOCKBILL_RE, false, 1);
IMPL_STUB(CECNetPrtcStockCommissionRe, GNET::PROTOCOL_STOCKCOMMISSION_RE, false, 1);
IMPL_STUB(CECNetPrtcStockTransactionRe, GNET::PROTOCOL_STOCKTRANSACTION_RE, false, 1);
IMPL_STUB(CECNetPrtcStockCancelRe, GNET::PROTOCOL_STOCKCANCEL_RE, false, 1);
IMPL_STUB(CECNetPrtcCombatChallengeRe, GNET::PROTOCOL_COMBATCHALLENGE_RE, false, 1);
IMPL_STUB(CECNetPrtcCombatTopRe, GNET::PROTOCOL_COMBATTOP_RE, false, 1);
IMPL_STUB(CECNetPrtcCombatControl, GNET::PROTOCOL_COMBATCONTROL, false, 1);
IMPL_STUB(CECNetPrtcCombatStatus, GNET::PROTOCOL_COMBATSTATUS, false, 1);
IMPL_STUB(CECNetPrtcDoChangeGSRe, GNET::PROTOCOL_DOCHANGEGS_RE, false, 1);
IMPL_STUB(CECNetPrtcFamilyRecord, GNET::PROTOCOL_FAMILYRECORD, false, 1);
IMPL_STUB(CECNetPrtcACQuestion, GNET::PROTOCOL_ACQUESTION, false, 1);
IMPL_STUB(CECNetPrtcAutolockSetRe, GNET::PROTOCOL_AUTOLOCKSET_RE, false, 1);
IMPL_STUB(CECNetPrtcWarChallengeRe, GNET::PROTOCOL_SIEGECHALLENGE_RE, false, 1);
IMPL_STUB(CECNetPrtcWarEnterRe, GNET::PROTOCOL_SIEGEENTER_RE, false, 1);
IMPL_STUB(CECNetPrtcWarInfoGetRe, GNET::PROTOCOL_SIEGEINFOGET_RE, false, 1);
IMPL_STUB(CECNetPrtcWarAssistantRe, GNET::PROTOCOL_SIEGESETASSISTANT_RE, false, 1);
IMPL_STUB(CECNetPrtcWarTopRe, GNET::PROTOCOL_SIEGETOP_RE, false, 1);
IMPL_STUB(CECNetPrtcSectRecruitRe, GNET::PROTOCOL_SECTRECRUIT_RE, false, 1);
IMPL_STUB(CECNetPrtcSectExpelRe, GNET::PROTOCOL_SECTEXPEL_RE, false, 1);
IMPL_STUB(CECNetPrtcSectListRe, GNET::PROTOCOL_SECTLIST_RE, false, 1);
IMPL_STUB(CECNetPrtcSectQuit, GNET::PROTOCOL_SECTQUIT, false, 1);
IMPL_STUB(CECNetPrtcContestInvite, GNET::PROTOCOL_CONTESTINVITE, false, 1);
IMPL_STUB(CECNetPrtcContestBegin, GNET::PROTOCOL_CONTESTBEGIN, false, 1);
IMPL_STUB(CECNetPrtcContestQuestion, GNET::PROTOCOL_CONTESTQUESTION, false, 1);
IMPL_STUB(CECNetPrtcContestAnswerRe, GNET::PROTOCOL_CONTESTANSWER_RE, false, 1);
IMPL_STUB(CECNetPrtcContestPlaceNotify, GNET::PROTOCOL_CONTESTPLACENOTIFY, false, 1);
IMPL_STUB(CECNetPrtcContestEnd, GNET::PROTOCOL_CONTESTEND, false, 1);
IMPL_STUB(CECNetPrtcSNSSetPlayerInfoRe, GNET::PROTOCOL_SNSSETPLAYERINFO_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSPressMessageRe, GNET::PROTOCOL_SNSPRESSMESSAGE_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSCancelMessageRe, GNET::PROTOCOL_SNSCANCELMESSAGE_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSApplyRe, GNET::PROTOCOL_SNSAPPLY_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSListMessageRe, GNET::PROTOCOL_SNSLISTMESSAGE_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSGetMessageRe, GNET::PROTOCOL_SNSGETMESSAGE_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSGetPlayerInfoRe, GNET::PROTOCOL_SNSGETPLAYERINFO_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSVoteRe, GNET::PROTOCOL_SNSVOTE_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSAcceptApplyRe, GNET::PROTOCOL_SNSACCEPTAPPLY_RE, false, 1);
IMPL_STUB(CECNetPrtcSNSApplyNotify, GNET::PROTOCOL_SNSAPPLYNOTIFY, false, 1);
IMPL_STUB(CECNetPrtcSNSDeleteLeaveMsg, GNET::PROTOCOL_SNSCANCELLEAVEMESSAGE_RE, false, 1);
IMPL_STUB(CECNetPrtcGetReferenceCodeRe, GNET::PROTOCOL_REFGETREFERENCECODE_RE, false, 1);
IMPL_STUB(CECNetPrtcListReferralsRe, GNET::PROTOCOL_REFLISTREFERRALS_RE, false, 1);
IMPL_STUB(CECNetPrtcWithdrawBonusRe, GNET::PROTOCOL_REFWITHDRAWBONUS_RE, false, 1);
IMPL_STUB(CECNetPrtcWithdrawExpRe, GNET::PROTOCOL_REFWITHDRAWEXP_RE, false, 1);
IMPL_STUB(CECNetPrtcGetServerRTT, GNET::PROTOCOL_GETSERVERRTT, false, 1);
IMPL_STUB(CECNetPrtcHomeTownExgMoney, GNET::PROTOCOL_HOMETOWNEXCHGMONEY_RE, false, 1);
IMPL_STUB(CECNetPrtcAchievementMsg, GNET::PROTOCOL_ACHIEVEMENTMESSAGE, false, 1);
IMPL_STUB(CECNetPrtcGshopNotifyScheme, GNET::PROTOCOL_GSHOPNOTIFYSCHEME, false, 1);
IMPL_STUB(CECNetPrtcPKMessage, GNET::PROTOCOL_PKMESSAGE, false, 1);
IMPL_STUB(CECNetPrtcRefineMessage, GNET::PROTOCOL_REFINEMESSAGE, false, 1);
IMPL_STUB(CECNetPrtcUniqueBidHistoryRe, GNET::PROTOCOL_UNIQUEBIDHISTORY_RE, false, 1);
IMPL_STUB(CECNetPrtcUniqueBidRe, GNET::PROTOCOL_UNIQUEBID_RE, false, 1);
IMPL_STUB(CECNetPrtcUniqueBidGetItemRe, GNET::PROTOCOL_UNIQUEGETITEM_RE, false, 1);
IMPL_STUB(CECNetPrtcTryChangeGSRe, GNET::PROTOCOL_TRYCHANGEGS_RE, false, 1);

CECNetProtocolBase* CECNetProtocolBase::CreateFrom(GNET::Protocol* pSrc)
{
	NetProtocolStubMap::iterator it = _NetProtocolStub.m_StubMap.find(pSrc->GetType());

	if (it == _NetProtocolStub.m_StubMap.end())
		return CECNetRawProtocol::CreateRaw(pSrc);
	else
		return (it->second)(pSrc);
}
