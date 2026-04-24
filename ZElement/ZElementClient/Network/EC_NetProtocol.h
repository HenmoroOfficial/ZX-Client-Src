#ifndef _EC_NETPROTOCOL_H_
#define _EC_NETPROTOCOL_H_

#include "EC_NetProtocolBase.h"

class CECGameSession;

class CECNetPrtclLineList : public CECNetProtocolBase
{
protected:

	CECNetPrtclLineList() {}
	virtual ~CECNetPrtclLineList() {}

public:

	virtual bool Filter(CECGameSession* pSession);
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclLineList, GNET::PROTOCOL_LINELIST, false, 1);
};

class CECNetPrtclGetTopTableRe : public CECNetProtocolBase
{
protected:

	CECNetPrtclGetTopTableRe() {}
	virtual ~CECNetPrtclGetTopTableRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclGetTopTableRe, GNET::PROTOCOL_GETTOPTABLE_RE, false, 1);
};

class CECNetPrtclGetEnemiesRe : public CECNetProtocolBase
{
protected:

	CECNetPrtclGetEnemiesRe() {}
	virtual ~CECNetPrtclGetEnemiesRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclGetEnemiesRe, GNET::PROTOCOL_GETENEMIES_RE, false, 1);
};

class CECNetPrtclUpdateEnemyRe : public CECNetProtocolBase
{
protected:

	CECNetPrtclUpdateEnemyRe() {}
	virtual ~CECNetPrtclUpdateEnemyRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtclUpdateEnemyRe, GNET::PROTOCOL_UPDATEENEMY_RE, false, 1);
};

class CECNetPrtcStockAccountRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcStockAccountRe() {}
	virtual ~CECNetPrtcStockAccountRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcStockAccountRe, GNET::PROTOCOL_STOCKACCOUNT_RE, false, 1);
};

class CECNetPrtcStockBillRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcStockBillRe() {}
	virtual ~CECNetPrtcStockBillRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcStockBillRe, GNET::PROTOCOL_STOCKBILL_RE, false, 1);
};

class CECNetPrtcStockCommissionRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcStockCommissionRe() {}
	virtual ~CECNetPrtcStockCommissionRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcStockCommissionRe, GNET::PROTOCOL_STOCKCOMMISSION_RE, false, 1);
};

class CECNetPrtcStockTransactionRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcStockTransactionRe() {}
	virtual ~CECNetPrtcStockTransactionRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcStockTransactionRe, GNET::PROTOCOL_STOCKTRANSACTION_RE, false, 1);
};

class CECNetPrtcStockCancelRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcStockCancelRe() {}
	virtual ~CECNetPrtcStockCancelRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcStockCancelRe, GNET::PROTOCOL_STOCKCANCEL_RE, false, 1);
};

class CECNetPrtcCombatChallengeRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcCombatChallengeRe() {}
	virtual ~CECNetPrtcCombatChallengeRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCombatChallengeRe, GNET::PROTOCOL_COMBATCHALLENGE_RE, false, 1);
};

class CECNetPrtcCombatTopRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcCombatTopRe() {}
	virtual ~CECNetPrtcCombatTopRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCombatTopRe, GNET::PROTOCOL_COMBATTOP_RE, false, 1);
};

class CECNetPrtcCombatControl : public CECNetProtocolBase
{
protected:

	CECNetPrtcCombatControl() {}
	virtual ~CECNetPrtcCombatControl() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCombatControl, GNET::PROTOCOL_COMBATCONTROL, false, 1);
};

class CECNetPrtcCombatStatus : public CECNetProtocolBase
{
protected:

	CECNetPrtcCombatStatus() {}
	virtual ~CECNetPrtcCombatStatus() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcCombatStatus, GNET::PROTOCOL_COMBATSTATUS, false, 1);
};

class CECNetPrtcTryChangeGSRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcTryChangeGSRe() {}
	virtual ~CECNetPrtcTryChangeGSRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcTryChangeGSRe, GNET::PROTOCOL_TRYCHANGEGS_RE, false, 1);
};

class CECNetPrtcDoChangeGSRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcDoChangeGSRe() {}
	virtual ~CECNetPrtcDoChangeGSRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcDoChangeGSRe, GNET::PROTOCOL_DOCHANGEGS_RE, false, 1);
};

class CECNetPrtcFamilyRecord : public CECNetProtocolBase
{
protected:

	CECNetPrtcFamilyRecord() {}
	virtual ~CECNetPrtcFamilyRecord() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcFamilyRecord, GNET::PROTOCOL_FAMILYRECORD, false, 1);
};

class CECNetPrtcACQuestion : public CECNetProtocolBase
{
protected:

	CECNetPrtcACQuestion() {}
	virtual ~CECNetPrtcACQuestion() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcACQuestion, GNET::PROTOCOL_ACQUESTION, false, 1);
};

class CECNetPrtcAutolockSetRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcAutolockSetRe() {}
	virtual ~CECNetPrtcAutolockSetRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcAutolockSetRe, GNET::PROTOCOL_AUTOLOCKSET_RE, false, 1);
};

class CECNetPrtcWarChallengeRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcWarChallengeRe() {}
	virtual ~CECNetPrtcWarChallengeRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarChallengeRe, GNET::PROTOCOL_SIEGECHALLENGE_RE, false, 1);
};

class CECNetPrtcWarEnterRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcWarEnterRe() {}
	virtual ~CECNetPrtcWarEnterRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarEnterRe, GNET::PROTOCOL_SIEGEENTER_RE, false, 1);
};

class CECNetPrtcWarInfoGetRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcWarInfoGetRe() {}
	virtual ~CECNetPrtcWarInfoGetRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarInfoGetRe, GNET::PROTOCOL_SIEGEINFOGET_RE, false, 1);
};

class CECNetPrtcWarAssistantRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcWarAssistantRe() {}
	virtual ~CECNetPrtcWarAssistantRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarAssistantRe, GNET::PROTOCOL_SIEGESETASSISTANT_RE, false, 1);
};

class CECNetPrtcWarTopRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcWarTopRe() {}
	virtual ~CECNetPrtcWarTopRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWarTopRe, GNET::PROTOCOL_SIEGETOP_RE, false, 1);
};

class CECNetPrtcSectRecruitRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcSectRecruitRe() {}
	virtual ~CECNetPrtcSectRecruitRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSectRecruitRe, GNET::PROTOCOL_SECTRECRUIT_RE, false, 1);
};

class CECNetPrtcSectExpelRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcSectExpelRe() {}
	virtual ~CECNetPrtcSectExpelRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSectExpelRe, GNET::PROTOCOL_SECTEXPEL_RE, false, 1);
};

class CECNetPrtcSectListRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcSectListRe() {}
	virtual ~CECNetPrtcSectListRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSectListRe, GNET::PROTOCOL_SECTLIST_RE, false, 1);
};

class CECNetPrtcSectQuit : public CECNetProtocolBase
{
protected:

	CECNetPrtcSectQuit() {}
	virtual ~CECNetPrtcSectQuit() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSectQuit, GNET::PROTOCOL_SECTQUIT, false, 1);
};

class CECNetPrtcContestInvite : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestInvite() {}
	virtual ~CECNetPrtcContestInvite() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestInvite, GNET::PROTOCOL_CONTESTINVITE, false, 1);
};

class CECNetPrtcContestBegin : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestBegin() {}
	virtual ~CECNetPrtcContestBegin() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestBegin, GNET::PROTOCOL_CONTESTBEGIN, false, 1);
};

class CECNetPrtcContestQuestion : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestQuestion() {}
	virtual ~CECNetPrtcContestQuestion() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestQuestion, GNET::PROTOCOL_CONTESTQUESTION, false, 1);
};

class CECNetPrtcContestAnswerRe : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestAnswerRe() {}
	virtual ~CECNetPrtcContestAnswerRe() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestAnswerRe, GNET::PROTOCOL_CONTESTANSWER_RE, false, 1);
};

class CECNetPrtcContestPlaceNotify : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestPlaceNotify() {}
	virtual ~CECNetPrtcContestPlaceNotify() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestPlaceNotify, GNET::PROTOCOL_CONTESTPLACENOTIFY, false, 1);
};

class CECNetPrtcContestEnd : public CECNetProtocolBase
{
protected:

	CECNetPrtcContestEnd() {}
	virtual ~CECNetPrtcContestEnd() {}

public:

	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcContestEnd, GNET::PROTOCOL_CONTESTEND, false, 1);
};

class CECNetPrtcSNSSetPlayerInfoRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSSetPlayerInfoRe() {}
	virtual ~CECNetPrtcSNSSetPlayerInfoRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSSetPlayerInfoRe, GNET::PROTOCOL_SNSSETPLAYERINFO_RE, false, 1);
};

class CECNetPrtcSNSPressMessageRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSPressMessageRe() {}
	virtual ~CECNetPrtcSNSPressMessageRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSPressMessageRe, GNET::PROTOCOL_SNSPRESSMESSAGE_RE, false, 1);
};

class CECNetPrtcSNSCancelMessageRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSCancelMessageRe() {}
	virtual ~CECNetPrtcSNSCancelMessageRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSCancelMessageRe, GNET::PROTOCOL_SNSCANCELMESSAGE_RE, false, 1);
};

class CECNetPrtcSNSApplyRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSApplyRe() {}
	virtual ~CECNetPrtcSNSApplyRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSApplyRe, GNET::PROTOCOL_SNSAPPLY_RE, false, 1);
};

class CECNetPrtcSNSListMessageRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSListMessageRe() {}
	virtual ~CECNetPrtcSNSListMessageRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSListMessageRe, GNET::PROTOCOL_SNSLISTMESSAGE_RE, false, 1);
};

class CECNetPrtcSNSGetMessageRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSGetMessageRe() {}
	virtual ~CECNetPrtcSNSGetMessageRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSGetMessageRe, GNET::PROTOCOL_SNSGETMESSAGE_RE, false, 1);
};

class CECNetPrtcSNSGetPlayerInfoRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSGetPlayerInfoRe() {}
	virtual ~CECNetPrtcSNSGetPlayerInfoRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSGetPlayerInfoRe, GNET::PROTOCOL_SNSGETPLAYERINFO_RE, false, 1);
};

class CECNetPrtcSNSVoteRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSVoteRe() {}
	virtual ~CECNetPrtcSNSVoteRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSVoteRe, GNET::PROTOCOL_SNSVOTE_RE, false, 1);
};

class CECNetPrtcSNSAcceptApplyRe : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSAcceptApplyRe() {}
	virtual ~CECNetPrtcSNSAcceptApplyRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSAcceptApplyRe, GNET::PROTOCOL_SNSACCEPTAPPLY_RE, false, 1);
};


class CECNetPrtcSNSApplyNotify : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSApplyNotify() {}
	virtual ~CECNetPrtcSNSApplyNotify() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSApplyNotify, GNET::PROTOCOL_SNSAPPLYNOTIFY, false, 1);
};

class CECNetPrtcSNSDeleteLeaveMsg : public CECNetProtocolBase
{
public:
	CECNetPrtcSNSDeleteLeaveMsg() {}
	virtual ~CECNetPrtcSNSDeleteLeaveMsg() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcSNSDeleteLeaveMsg, GNET::PROTOCOL_SNSCANCELLEAVEMESSAGE_RE, false, 1);
};

class CECNetPrtcGetReferenceCodeRe : public CECNetProtocolBase
{
public:
	CECNetPrtcGetReferenceCodeRe() {}
	virtual ~CECNetPrtcGetReferenceCodeRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcGetReferenceCodeRe, GNET::PROTOCOL_REFGETREFERENCECODE_RE, false, 1);
};

class CECNetPrtcListReferralsRe : public CECNetProtocolBase
{
public:
	CECNetPrtcListReferralsRe() {}
	virtual ~CECNetPrtcListReferralsRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcListReferralsRe, GNET::PROTOCOL_REFLISTREFERRALS_RE, false, 1);
};

class CECNetPrtcWithdrawBonusRe : public CECNetProtocolBase
{
public:
	CECNetPrtcWithdrawBonusRe() {}
	virtual ~CECNetPrtcWithdrawBonusRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWithdrawBonusRe, GNET::PROTOCOL_REFWITHDRAWBONUS_RE, false, 1);
};

class CECNetPrtcWithdrawExpRe : public CECNetProtocolBase
{
public:
	CECNetPrtcWithdrawExpRe() {}
	virtual ~CECNetPrtcWithdrawExpRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcWithdrawExpRe, GNET::PROTOCOL_REFWITHDRAWEXP_RE, false, 1);
};


class CECNetPrtcGetServerRTT : public CECNetProtocolBase
{
public:
	CECNetPrtcGetServerRTT() {}
	virtual ~CECNetPrtcGetServerRTT() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcGetServerRTT, GNET::PROTOCOL_GETSERVERRTT, false, 1);
};

class CECNetPrtcHomeTownExgMoney : public CECNetProtocolBase
{
public:
	CECNetPrtcHomeTownExgMoney() {}
	virtual ~CECNetPrtcHomeTownExgMoney() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcHomeTownExgMoney, GNET::PROTOCOL_HOMETOWNEXCHGMONEY_RE, false, 1);
};

class CECNetPrtcAchievementMsg : public CECNetProtocolBase
{
public:
	CECNetPrtcAchievementMsg() {}
	virtual ~CECNetPrtcAchievementMsg() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcAchievementMsg, GNET::PROTOCOL_ACHIEVEMENTMESSAGE, false, 1);
};

class CECNetPrtcGshopNotifyScheme : public CECNetProtocolBase
{
public:
	CECNetPrtcGshopNotifyScheme() {}
	virtual ~CECNetPrtcGshopNotifyScheme() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcGshopNotifyScheme, GNET::PROTOCOL_GSHOPNOTIFYSCHEME, false, 1);
};

class CECNetPrtcPKMessage : public CECNetProtocolBase
{
public:
	CECNetPrtcPKMessage() {}
	virtual ~CECNetPrtcPKMessage() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcPKMessage, GNET::PROTOCOL_PKMESSAGE, false, 1);
};

class CECNetPrtcRefineMessage : public CECNetProtocolBase
{
public:
	CECNetPrtcRefineMessage() {}
	virtual ~CECNetPrtcRefineMessage() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcRefineMessage, GNET::PROTOCOL_REFINEMESSAGE, false, 1);
};

class CECNetPrtcUniqueBidHistoryRe : public CECNetProtocolBase
{
public:
	CECNetPrtcUniqueBidHistoryRe() {}
	virtual ~CECNetPrtcUniqueBidHistoryRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcUniqueBidHistoryRe, GNET::PROTOCOL_UNIQUEBIDHISTORY_RE, false, 1);
};

// 唯一最低价竞拍出价返回
class CECNetPrtcUniqueBidRe : public CECNetProtocolBase
{
public:
	CECNetPrtcUniqueBidRe() {}
	virtual ~CECNetPrtcUniqueBidRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcUniqueBidRe, GNET::PROTOCOL_UNIQUEBID_RE, false, 1);
};


// 唯一最低价竞拍出价返回
class CECNetPrtcUniqueBidGetItemRe : public CECNetProtocolBase
{
public:
	CECNetPrtcUniqueBidGetItemRe() {}
	virtual ~CECNetPrtcUniqueBidGetItemRe() {}

public:
	virtual bool Process(CECGameSession* pSession);

	static CECNetProtocolBase* _Create(GNET::Protocol*);
	DECL_STUB(CECNetPrtcUniqueBidGetItemRe, GNET::PROTOCOL_UNIQUEGETITEM_RE, false, 1);
};

#endif
