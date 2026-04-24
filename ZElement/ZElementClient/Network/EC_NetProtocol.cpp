#include "EC_Global.h"
#include "gnet.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_LoginUIMan.h"
#include "EC_GPDataType.h"
#include "EC_GameSession.h"
#include "EC_NetProtocol.h"
#include "EC_Faction.h"
#include "EC_HostPlayer.h"
#include "EC_Achievement.h"
#include "elementdataman.h"
#include "DlgAutoLock.h"
#include "DlgContest.h"
#include "DlgFriendList.h"
#include "DlgGoldTrade.h"
#include "DlgGuildMan.h"
#include "DlgQuestion.h"
#include "DlgSiege.h"
#include "DlgTeacherMan.h"
#include "DlgFriendSeeking.h"
#include "DlgSnsInfo.h"
#include "DlgSpread.h"
#include "DlgGardenInventory.h"
#include "DlgModule.h"
#include "DlgQShop.h"
#include "DlgQShopItem.h"
#include "DlgPKMsg.h"
#include "DlgAuction.h"
#include "linelist.hpp"
#include "gettoptable_re.hpp"
#include "getenemies_re.hpp"
#include "updateenemy_re.hpp"
#include "stockaccount_re.hpp"
#include "stockbill_re.hpp"
#include "stockcommission_re.hpp"
#include "stocktransaction_re.hpp"
#include "stockcancel_re.hpp"
#include "combatchallenge_re.hpp"
#include "combattop_re.hpp"
#include "combatcontrol.hpp"
#include "combatstatus.hpp"
#include "familyrecord.hpp"
#include "acquestion.hpp"
#include "EC_GPDataType.h"
#include "autolockset_re.hpp"
#include "siegechallenge_re.hpp"
#include "siegeenter_re.hpp"
#include "siegeinfoget_re.hpp"
#include "siegesetassistant_re.hpp"
#include "siegetop_re.hpp"
#include "sectrecruit_re.hpp"
#include "sectexpel_re.hpp"
#include "sectlist_re.hpp"
#include "sectquit.hpp"
#include "globaldataman.h"
#include "vector.h"
#include "contestquestion.hpp"
#include "ContestPlaceNotify.hpp"
#include "contestanswer_re.hpp"
#include "contestbegin.hpp"
#include "contestend.hpp"
#include "contestinvite.hpp"
#include "snssetplayerinfo_re.hpp"
#include "snspressmessage_re.hpp"
#include "snscancelmessage_re.hpp"
#include "snsapply_re.hpp"
#include "snslistmessage_re.hpp"
#include "snsgetmessage_re.hpp"
#include "snsgetplayerinfo_re.hpp"
#include "snsvote_re.hpp"
#include "snsacceptapply_re.hpp"
#include "snsapplynotify.hpp"
#include "snscancelleavemessage.hpp"
#include "snscancelleavemessage_re.hpp"
#include "refgetreferencecode_re.hpp"
#include "reflistreferrals_re.hpp"
#include "refwithdrawbonus_re.hpp"
#include "refwithdrawexp_re.hpp"
#include "getserverrtt.hpp"
#include "hometownexchgmoney_re.hpp"
#include "achievementmessage.hpp"
#include "gshopnotifyscheme.hpp"
#include "pkmessage.hpp"
#include "refinemessage.hpp"
#include "uniquebidhistory_re.hpp"
#include "uniquebid_re.hpp"
#include "uniquegetitem_re.hpp"

#include "trychangegs_re.hpp"
#include "dochangegs_re.hpp"
#include "DlgGuildMain.h"
CECNetProtocolBase* CECNetPrtclLineList::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclLineList)
}

bool CECNetPrtclLineList::Filter(CECGameSession* pSession)
{
	return false;
//	return g_pGame->GetGameRun()->GetGameState() != CECGameRun::GS_LOGIN;
}

bool CECNetPrtclLineList::Process(CECGameSession* pSession)
{
	GNET::LineList* pList = static_cast<GNET::LineList*>(m_pRaw);
	CECGameRun::LineInfoVec& vec = g_pGame->GetGameRun()->GetLinesInfo();
	vec.clear();

	for (size_t i = 0; i < pList->linelist.size(); i++)
	{
		CECGameRun::LineInfo info;
		GNET::LineInfo& l = pList->linelist[i];

		info.id = l.id;
		_cp_str(info.name, l.name.begin(), l.name.size());
		info.attribute = *((int*)l.attribute.begin());
		if(l.attribute.size() >4 )
			info.bNoPKLine = *(((int*)l.attribute.begin())+1) == 0 ? false:true;
		else
			info.bNoPKLine = false;
		vec.push_back(info);
	}

	CECLoginUIMan *pLoginUI = g_pGame->GetGameRun()->GetUIManager()->GetLoginUIMan();
	if (pLoginUI)
		pLoginUI->SetPwFlag(char((pList->algorithm>>9)&0x3));

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pGameUI && pGameUI->GetDialog("Win_KfSelectLine")->IsShow())
		pGameUI->GetDialog("Win_KfSelectLine")->Show(true); //call OnShowDialog()

	return true;
}

CECNetProtocolBase* CECNetPrtclGetTopTableRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclGetTopTableRe)
}

bool CECNetPrtclGetTopTableRe::Process(CECGameSession* pSession)
{
	GNET::GetTopTable_Re* p = static_cast<GNET::GetTopTable_Re*>(m_pRaw);
	if(p->tableid == 96) /// 96 为服务器硬性规定的6V6排行榜
	{
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->RankCrssvrTemas(p);
	}
	else
	{
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->RankListAction(p);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtclGetEnemiesRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclGetEnemiesRe)
}

bool CECNetPrtclGetEnemiesRe::Process(CECGameSession* pSession)
{
	GNET::GetEnemies_Re* p = static_cast<GNET::GetEnemies_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->EnemyListAction(CDlgFriendList::ENEMYLIST_ACTION_UPDATELIST, p);
	return true;
}

CECNetProtocolBase* CECNetPrtclUpdateEnemyRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtclUpdateEnemyRe)
}

bool CECNetPrtclUpdateEnemyRe::Process(CECGameSession* pSession)
{
	GNET::UpdateEnemy_Re* p = static_cast<GNET::UpdateEnemy_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->EnemyListAction(CDlgFriendList::ENEMYLIST_ACTION_UPDATE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcStockAccountRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcStockAccountRe)
}

bool CECNetPrtcStockAccountRe::Process(CECGameSession* pSession)
{
	GNET::StockAccount_Re* p = static_cast<GNET::StockAccount_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->
		GoldAction(CDlgGoldTrade::GOLD_ACTION_ACCOUNT_RE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcStockBillRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcStockBillRe)
}

bool CECNetPrtcStockBillRe::Process(CECGameSession* pSession)
{
	GNET::StockBill_Re* p = static_cast<GNET::StockBill_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->
		GoldAction(CDlgGoldTrade::GOLD_ACTION_BILL_RE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcStockCommissionRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcStockCommissionRe)
}

bool CECNetPrtcStockCommissionRe::Process(CECGameSession* pSession)
{
	GNET::StockCommission_Re* p = static_cast<GNET::StockCommission_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->
		GoldAction(CDlgGoldTrade::GOLD_ACTION_COMMISSION_RE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcStockTransactionRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcStockTransactionRe)
}

bool CECNetPrtcStockTransactionRe::Process(CECGameSession* pSession)
{
	GNET::StockTransaction_Re* p = static_cast<GNET::StockTransaction_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->
		GoldAction(CDlgGoldTrade::GOLD_ACTION_TRANSACTION_RE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcStockCancelRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcStockCancelRe)
}

bool CECNetPrtcStockCancelRe::Process(CECGameSession* pSession)
{
	GNET::StockCancel_Re* p = static_cast<GNET::StockCancel_Re*>(m_pRaw);
	g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->
		GoldAction(CDlgGoldTrade::GOLD_ACTION_CANCEL_RE, p);
	return true;
}

CECNetProtocolBase* CECNetPrtcCombatChallengeRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCombatChallengeRe)
}

bool CECNetPrtcCombatChallengeRe::Process(CECGameSession* pSession)
{
	GNET::CombatChallenge_Re* p = static_cast<GNET::CombatChallenge_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( p->retcode == ERR_COMBAT_NOPROSPERITY )
	{
		pGameUI->MessageBox("", pGameUI->GetStringFromTable(825), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( p->retcode == ERR_COMBAT_MASTEROFFLINE )
	{
		pGameUI->MessageBox("", pGameUI->GetStringFromTable(828), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( p->retcode == ERR_COMBAT_BUSY )
	{
		pGameUI->MessageBox("", pGameUI->GetStringFromTable(827), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( p->retcode == ERR_COMBAT_COOLING )
	{
		pGameUI->MessageBox("", pGameUI->GetStringFromTable(829), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	}
	else if( p->retcode == ERR_COMBAT_LOWLEVEL )
	{
		pGameUI->MessageBox("", pGameUI->GetStringFromTable(889), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcCombatTopRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCombatTopRe)
}

bool CECNetPrtcCombatTopRe::Process(CECGameSession* pSession)
{
	GNET::CombatTop_Re* p = static_cast<GNET::CombatTop_Re*>(m_pRaw);

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGuildInfo*)pGameUI->GetDialog("Win_Guild_Info"))->BuildGuildWarRankList(p);
	return true;
}

CECNetProtocolBase* CECNetPrtcCombatControl::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCombatControl)
}

bool CECNetPrtcCombatControl::Process(CECGameSession* pSession)
{
	GNET::CombatControl* p = static_cast<GNET::CombatControl*>(m_pRaw);
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pHost)
	{
		if (p->cmd)
		{
			int idCombatFaction = pHost->GetFactionID() == p->attacker ? p->defender : p->attacker;
			pHost->SetCombatFaction(idCombatFaction);
			pHost->ResetFactionCombatCounts();
			if( !g_pGame->GetFactionMan()->GetFaction(idCombatFaction) )
				g_pGame->GetGameSession()->GetFactionInfo(1, &idCombatFaction, 0);
		}
		else
			pHost->SetCombatFaction(0);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcCombatStatus::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcCombatStatus)
}

bool CECNetPrtcCombatStatus::Process(CECGameSession* pSession)
{
	GNET::CombatStatus* p = static_cast<GNET::CombatStatus*>(m_pRaw);
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if( pHost )
	{
		int idCombatFaction = pHost->GetFactionID() == p->attacker ? p->defender : p->attacker;
		pHost->SetCombatFaction(idCombatFaction);
		if( !g_pGame->GetFactionMan()->GetFaction(idCombatFaction) )
			g_pGame->GetGameSession()->GetFactionInfo(1, &idCombatFaction, 0);
	}
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgGuildMan*)pGameUI->GetDialog("Win_GuildWarStatus"))->RefreshGuildWarStatus(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcTryChangeGSRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcTryChangeGSRe)
}

bool CECNetPrtcTryChangeGSRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::TryChangeGS_Re* p = static_cast<GNET::TryChangeGS_Re*>(m_pRaw);

	g_pGame->SetGuildEnter(false);
	if (p->retcode == 0)
	{
		g_pGame->GetGameRun()->SaveConfigsToLocal();
		g_pGame->GetGameRun()->EndGameState();
		g_pGame->GetGameSession()->do_change_gs(p->dst_lineid);
		if(p->reason == 4) // 与GS 定义了 如果是4 代表进入帮派基地
		{
			g_pGame->SetGuildEnter(true);
		}
	}
	else
		g_pGame->GetGameSession()->OutputLinkSevError(p->retcode);

	return true;
}

CECNetProtocolBase* CECNetPrtcDoChangeGSRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcDoChangeGSRe)
}

bool CECNetPrtcDoChangeGSRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::DoChangeGS_Re* p = static_cast<GNET::DoChangeGS_Re*>(m_pRaw);

	if (p->retcode == 0)
	{
		g_pGame->GetGameRun()->SetCurLine(p->dst_lineid);
		A3DVECTOR3 pos;
		pos.x	= p->scalex;
		pos.y	= p->scaley;
		pos.z	= p->scalez;

//		g_pGame->GetGameRun()->ReleaseHostPlayer();
//		g_pGame->GetGameRun()->CreateHostPlayer();
//		g_pGame->GetGameRun()->JumpToInstance(p->mapid,pos);
		g_pGame->GetGameRun()->StartGame(p->mapid, pos);
		g_pGame->GetGameSession()->StartGame();
	}
	else
		g_pGame->GetGameSession()->OutputLinkSevError(p->retcode);

	return true;
}

CECNetProtocolBase* CECNetPrtcFamilyRecord::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcFamilyRecord)
}

bool CECNetPrtcFamilyRecord::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::FamilyRecord* p = static_cast<GNET::FamilyRecord*>(m_pRaw);
	CECFactionMan* pMan = g_pGame->GetFactionMan();
	Faction_Info* pInfo = pMan->GetFaction(p->familyid, false, true);
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (pInfo)
	{
		pInfo->SetLevel(p->star);

		if (pHost && p->familyid == pHost->GetFamilyID())
		{
			for (size_t i = 0; i < p->records.size() && i < FAMILY_RECORD_COUNT; i++)
				pMan->SetFamilyRecord(i, p->records[i]);
		}
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcACQuestion::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcACQuestion)
}

bool CECNetPrtcACQuestion::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ACQuestion* p = static_cast<GNET::ACQuestion*>(m_pRaw);
	
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgQuestion*)pGameUI->GetDialog("Win_Question"))->SetQuestion(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcAutolockSetRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcAutolockSetRe)
}

bool CECNetPrtcAutolockSetRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::AutolockSet_Re* p = static_cast<GNET::AutolockSet_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgAutoLock*)pGameUI->GetDialog("Win_AutoLock"))->SetAutoLockTime(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarChallengeRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarChallengeRe)
}

bool CECNetPrtcWarChallengeRe::Process(CECGameSession* pSession)
{
	GNET::SiegeChallenge_Re* p = static_cast<GNET::SiegeChallenge_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo"))->OnWarChallengeRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarEnterRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarEnterRe)
}

bool CECNetPrtcWarEnterRe::Process(CECGameSession* pSession)
{
	GNET::SiegeEnter_Re* p = static_cast<GNET::SiegeEnter_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo"))->OnWarEnterRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarInfoGetRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarInfoGetRe)
}

bool CECNetPrtcWarInfoGetRe::Process(CECGameSession* pSession)
{
	GNET::SiegeInfoGet_Re* p = static_cast<GNET::SiegeInfoGet_Re*>(m_pRaw);
	g_pGame->GetGameRun()->SetCityOwnerFaction(p->city.owner.fid);
	g_pGame->GetGameRun()->SetCityOwnerMaster(p->city.owner.master);
	g_pGame->GetGameRun()->SetCityOccupyTime(p->city.occupy_time);
	g_pGame->GetGameRun()->SetMasterFlag(false);

	vector<DOMAIN_INFO>* info_vec = globaldata_getdomaininfos();

	for (size_t i = 0; i < info_vec->size(); i++)
	{
		DOMAIN_INFO& info = (*info_vec)[i];

		if (info.id == 1)
		{
			info.id_owner = p->city.owner.fid;
			break;
		}
	}

	if (g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan()==CECUIManager::UIMAN_INGAME)
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		//((CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo"))->OnWarInfoGetRe(p);
		((CDlgGuildInfo*)pGameUI->GetDialog("Win_Guild_Info"))->UpdateGuildSiege(p);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcWarAssistantRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarAssistantRe)
}

bool CECNetPrtcWarAssistantRe::Process(CECGameSession* pSession)
{
	GNET::SiegeSetAssistant_Re* p = static_cast<GNET::SiegeSetAssistant_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSiege*)pGameUI->GetDialog("Win_SiegeInfo"))->OnWarAssistRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWarTopRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWarTopRe)
}

bool CECNetPrtcWarTopRe::Process(CECGameSession* pSession)
{
	GNET::SiegeTop_Re* p = static_cast<GNET::SiegeTop_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSiege*)pGameUI->GetDialog("Win_SiegeScore"))->OnWarTopRe(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcSectRecruitRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSectRecruitRe)
}

bool CECNetPrtcSectRecruitRe::Process(CECGameSession* pSession)
{
	GNET::SectRecruit_Re* p = static_cast<GNET::SectRecruit_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTeacherMan*)pGameUI->GetDialog("Win_TeacherManage"))->OnSectRecruitRe(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcSectExpelRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSectExpelRe)
}

bool CECNetPrtcSectExpelRe::Process(CECGameSession* pSession)
{
	GNET::SectExpel_Re* p = static_cast<GNET::SectExpel_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTeacherMan*)pGameUI->GetDialog("Win_TeacherManage"))->OnSectExpelRe(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcSectListRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSectListRe)
}

bool CECNetPrtcSectListRe::Process(CECGameSession* pSession)
{
	GNET::SectList_Re* p = static_cast<GNET::SectList_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTeacherMan*)pGameUI->GetDialog("Win_TeacherManage"))->OnSectListRe(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcSectQuit::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSectQuit)
}

bool CECNetPrtcSectQuit::Process(CECGameSession* pSession)
{
	GNET::SectQuit* p = static_cast<GNET::SectQuit*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTeacherMan*)pGameUI->GetDialog("Win_TeacherManage"))->OnSectQuit(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcContestInvite::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestInvite)
}

bool CECNetPrtcContestInvite::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestInvite* p = static_cast<GNET::ContestInvite*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_ContestInvite(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcContestBegin::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestBegin)
}

bool CECNetPrtcContestBegin::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestBegin* p = static_cast<GNET::ContestBegin*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_ContestBegin(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcContestQuestion::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestQuestion)
}

bool CECNetPrtcContestQuestion::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestQuestion* p = static_cast<GNET::ContestQuestion*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_ContestQuestion(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcContestAnswerRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestAnswerRe)
}

bool CECNetPrtcContestAnswerRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestAnswer_Re* p = static_cast<GNET::ContestAnswer_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_ContestAnswerRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcContestPlaceNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestPlaceNotify)
}

bool CECNetPrtcContestPlaceNotify::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestPlaceNotify* p = static_cast<GNET::ContestPlaceNotify*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_NotifyContestPlace(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcContestEnd::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcContestEnd)
}

bool CECNetPrtcContestEnd::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::ContestEnd* p = static_cast<GNET::ContestEnd*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgContest*)pGameUI->GetDialog("Win_Contest"))->OnPrtc_ContestEnd(p);

	return true;
}


CECNetProtocolBase* CECNetPrtcSNSSetPlayerInfoRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSSetPlayerInfoRe)
}

bool CECNetPrtcSNSSetPlayerInfoRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSSetPlayerInfo_Re* p = static_cast<GNET::SNSSetPlayerInfo_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->AddChatMessage(pGameUI->GetStringFromTable(7178), GP_CHAT_MISC);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSPressMessageRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSPressMessageRe)
}

bool CECNetPrtcSNSPressMessageRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSPressMessage_Re* p = static_cast<GNET::SNSPressMessage_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSPressMessageRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSCancelMessageRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSCancelMessageRe)
}

bool CECNetPrtcSNSCancelMessageRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSCancelMessage_Re* p = static_cast<GNET::SNSCancelMessage_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSCancelMessageRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSApplyRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSApplyRe)
}

bool CECNetPrtcSNSApplyRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSApply_Re* p = static_cast<GNET::SNSApply_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSApplyRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSListMessageRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSListMessageRe)
}

bool CECNetPrtcSNSListMessageRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSListMessage_Re* p = static_cast<GNET::SNSListMessage_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSListMessageRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSGetMessageRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSGetMessageRe)
}

bool CECNetPrtcSNSGetMessageRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSGetMessage_Re* p = static_cast<GNET::SNSGetMessage_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeekingMsg"))->OnPrtc_SNSGetMessageRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSGetPlayerInfoRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSGetPlayerInfoRe)
}

bool CECNetPrtcSNSGetPlayerInfoRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSGetPlayerInfo_Re* p = static_cast<GNET::SNSGetPlayerInfo_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSGetPlayerInfoRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSVoteRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSVoteRe)
}

bool CECNetPrtcSNSVoteRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSVote_Re* p = static_cast<GNET::SNSVote_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeeking"))->OnPrtc_SNSVoteRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSAcceptApplyRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSAcceptApplyRe)
}

bool CECNetPrtcSNSAcceptApplyRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSAcceptApply_Re* p = static_cast<GNET::SNSAcceptApply_Re*>(m_pRaw);
 	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeekingMsg"))->OnPrtc_SNSAcceptApplyRe(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcSNSApplyNotify::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSApplyNotify)
}

bool CECNetPrtcSNSApplyNotify::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSApplyNotify* p = static_cast<GNET::SNSApplyNotify*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACHAR szMsg[1024];
	ACString strMsg = _AL("");
	strMsg.Format( pGameUI->GetStringFromTable(7271 + p->message_type), ACString((const ACHAR*)p->apply_rolename.begin(), p->apply_rolename.size()/sizeof(ACHAR)) );

	if(strMsg != _AL(""))
	{
		AUI_ConvertChatString(strMsg, szMsg);
		pGameUI->AddChatMessage(szMsg, GP_CHAT_MISC);
	}
	return true;
}

CECNetProtocolBase* CECNetPrtcSNSDeleteLeaveMsg::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcSNSDeleteLeaveMsg)
}

bool CECNetPrtcSNSDeleteLeaveMsg::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::SNSCancelLeaveMessage* p = static_cast<GNET::SNSCancelLeaveMessage*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgFriendSeek*)pGameUI->GetDialog("Win_FriendSeekingMsg"))->OnPrtc_SNSDeleteLeaveMsgRe(p);
	return true;
}

CECNetProtocolBase* CECNetPrtcGetReferenceCodeRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcGetReferenceCodeRe)
}

bool CECNetPrtcGetReferenceCodeRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::RefGetReferenceCode_Re* p = static_cast<GNET::RefGetReferenceCode_Re*>(m_pRaw);
	if(p->retcode == ERR_SUCCESS)
	{	
		CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
		if (pHost)
			pHost->SetSelfReferenceCode(p->refcode.begin(), p->refcode.size());
	}
	else
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if(p->retcode == 10)
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(7609), GP_CHAT_MISC);
	}
	return true;
}


CECNetProtocolBase* CECNetPrtcListReferralsRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcListReferralsRe)
}

bool CECNetPrtcListReferralsRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::RefListReferrals_Re* p = static_cast<GNET::RefListReferrals_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSpread*)pGameUI->GetDialog("Win_PopProfit"))->ReferAction(p);

	return true;
}

CECNetProtocolBase* CECNetPrtcWithdrawBonusRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWithdrawBonusRe)
}

bool CECNetPrtcWithdrawBonusRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::RefWithdrawBonus_Re* p = static_cast<GNET::RefWithdrawBonus_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSpread*)pGameUI->GetDialog("Win_PopProfit"))->UpdatePage();
	return true;
}

CECNetProtocolBase* CECNetPrtcWithdrawExpRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcWithdrawExpRe)
}

bool CECNetPrtcWithdrawExpRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::RefWithdrawExp_Re* p = static_cast<GNET::RefWithdrawExp_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgSpread*)pGameUI->GetDialog("Win_PopProfit"))->UpdatePage();
	return true;
}


CECNetProtocolBase* CECNetPrtcGetServerRTT::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcGetServerRTT)
}

bool CECNetPrtcGetServerRTT::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::GetServerRTT* p = static_cast<GNET::GetServerRTT*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	return true;
}

CECNetProtocolBase* CECNetPrtcHomeTownExgMoney::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcHomeTownExgMoney)
}

bool CECNetPrtcHomeTownExgMoney::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::HometownExchgMoney_Re* p = static_cast<GNET::HometownExchgMoney_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strText, strMoney;
	if (p->retcode==0)
	{
		CDlgModule::SetModuleMoney(p->hometown_money);
		pGameUI->Int64ToStr(p->hometown_money, strMoney);
		strText.Format(pGameUI->GetStringFromTable(9103), strMoney);
		pGameUI->GetDialog("Win_Garden_Exchange1")->Show(false);
	}
	else if (p->retcode==38)
		strText = pGameUI->GetStringFromTable(9105);
	else
		strText = pGameUI->GetStringFromTable(8700+p->retcode);
	pGameUI->AddChatMessage(strText, GP_CHAT_MISC);


	return true;
}

CECNetProtocolBase* CECNetPrtcAchievementMsg::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcAchievementMsg)
}

bool CECNetPrtcAchievementMsg::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::AchievementMessage* p = static_cast<GNET::AchievementMessage*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (!pGameUI)
		return true;

	CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (!pHost)
		return true;

	CECAchievementMan *pMan = pHost->GetAchievementMan();
	if (!pMan)
		return true;
	
	ACString name((ACHAR*)p->rolename.begin(),p->rolename.size()/sizeof(ACHAR));
	if (name.GetLength()<=0)
	{
		name = _AL("error");
	}

	if (pMan->GetAchievement(p->achieve_id))
	{
		sAchievement* pAchievement = new sAchievement(*pMan->GetAchievement(p->achieve_id));
		pAchievement->iFinishTime = p->timestamp;
		pAchievement->bFinish = true;
		pAchievement->strOwnerName = name;
		pAchievement->SetFinish(pAchievement->iFinishTime != 0);

		ACHAR szText[512];
		AUI_ConvertChatString(name, szText, false);
		AWString str;
		str.Format(pAchievement->strMsg, szText, AUICOMMON_ITEM_CODE_START, pAchievement->name);

		pGameUI->AddChatMessage(str, GP_CHAT_SYSTEM, -1, NULL, 0, 0, NULL, pAchievement);
	}
	return true;
}


CECNetProtocolBase* CECNetPrtcGshopNotifyScheme::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcGshopNotifyScheme)
}

bool CECNetPrtcGshopNotifyScheme::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::GShopNotifyScheme* p = static_cast<GNET::GShopNotifyScheme*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	int type = (p->discount)<<24;
	type |= (p->sale)<<16;
	CDlgQShopItem::SetPropType(type);
	CDlgQShop *pQshop = (CDlgQShop*)pGameUI->GetDialog("Win_QShop");
	pQshop->Refresh();

	return true;
}

CECNetProtocolBase* CECNetPrtcPKMessage::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcPKMessage)
}

bool CECNetPrtcPKMessage::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::PKMessage* p = static_cast<GNET::PKMessage*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost && (pHost->GetBattleType() == BATTLE_TYPE_ARENA || pHost->GetBattleType() == BATTLE_TYPE_ARENAREBORN || pHost->GetBattleType() == BATTLE_TYPE_CRSSVR ) )
		return true;
	
	CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)pGameUI->GetDialog("Win_Pkmsg");
	ACString killer((ACHAR*)p->killer_name.begin(),p->killer_name.size()/sizeof(ACHAR));
	ACString deader((ACHAR*)p->deader_name.begin(),p->deader_name.size()/sizeof(ACHAR));
	pDlgPKMsg->SetInfo(p->killer_id, p->deader_id, killer.GetBuffer(0), deader.GetBuffer(0));

	return true;
}

CECNetProtocolBase* CECNetPrtcRefineMessage::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcRefineMessage)
}

bool CECNetPrtcRefineMessage::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::RefineMessage* p = static_cast<GNET::RefineMessage*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString name((ACHAR*)p->rolename.begin(),p->rolename.size()/sizeof(ACHAR));
	ACString strText;
	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(p->itemid, ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_EQUIPMENT_ESSENCE)
		return true;
	EQUIPMENT_ESSENCE* pEssence = (EQUIPMENT_ESSENCE*)pDataPtr;
	if (p->refine_level  <= 16)
		strText.Format(pGameUI->GetStringFromTable(10170 + p->refine_level), name, pEssence->name);
	else
		strText.Format(pGameUI->GetStringFromTable(10170 + p->refine_level), name, pEssence->name, name);

	pGameUI->AddChatMessage(strText, GP_CHAT_SYSTEM);
	return true;
}

CECNetProtocolBase* CECNetPrtcUniqueBidHistoryRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcUniqueBidHistoryRe)
}

bool CECNetPrtcUniqueBidHistoryRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::UniqueBidHistory_Re* p = static_cast<GNET::UniqueBidHistory_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	CDlgAuction* pDlgAuction = (CDlgAuction*)pGameUI->GetDialog("Win_Auction");
	pDlgAuction->HistoryInfo(p);
	
	return true;
}

CECNetProtocolBase* CECNetPrtcUniqueBidRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcUniqueBidRe)
}

bool CECNetPrtcUniqueBidRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::UniqueBid_Re* p = static_cast<GNET::UniqueBid_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (p->retcode!=0)
	{
		pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9850+p->retcode), GP_CHAT_MISC);
	}

	return true;
}

CECNetProtocolBase* CECNetPrtcUniqueBidGetItemRe::_Create(GNET::Protocol* pSrc)
{
	_CREATE_PRTCL_FROM(CECNetPrtcUniqueBidGetItemRe)
}

bool CECNetPrtcUniqueBidGetItemRe::Process(CECGameSession* pSession)
{
	using namespace GNET;
	GNET::UniqueGetItem_Re* p = static_cast<GNET::UniqueGetItem_Re*>(m_pRaw);
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (p->retcode!=0)
	{
		pGameUI->AddChatMessage(pGameUI->GetStringFromTable(9850+p->retcode), GP_CHAT_MISC);
	}

	return true;
}