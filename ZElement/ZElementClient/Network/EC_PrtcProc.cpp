/*
 * FILE: EC_GameDataPrtc.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include <AFile.h>

#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_Game.h"
#include "EC_GPDataType.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_RTDebug.h"
#include "EC_FixedMsg.h"
#include "EC_DealInventory.h"
#include "EC_IvtrTypes.h"
#include "EC_IvtrItem.h"
#include "EC_HostPlayer.h"
#include "EC_PrtcPBIProc.h "

#include "gnet.h"
#include "gameclient.h"
#include "LuaEvent.h"

#include "gmkickoutrole.hpp"
#include "gmkickoutrole_re.hpp"
#include "gmkickoutuser.hpp"
#include "gmkickoutuser_re.hpp"
#include "gmlistonlineuser.hpp"
#include "gmlistonlineuser_re.hpp"
#include "gmonlinenum.hpp"
#include "gmonlinenum_re.hpp"
#include "gmrestartserver.hpp"
#include "gmrestartserver_re.hpp"
#include "gmshutup.hpp"
#include "gmshutup_re.hpp"
#include "gmshutuprole.hpp"
#include "gmshutuprole_re.hpp"
#include "gmforbidrole.hpp"
#include "gmforbidrole_re.hpp"
#include "gmtogglechat.hpp"
#include "gmtogglechat_re.hpp"
#include "friendcallbacksubscribe.hpp"
#include "factionchat.hpp"
#include "factiondata.hxx"
#include "EC_Faction.h"
#include "ids.hxx"
#include "factioninviteres"
#include "factioncreate.hpp"
#include "factiondismiss.hpp"
#include "factionexpel.hpp"
#include "factionleave.hpp"
#include "factionlist.hpp"
#include "factionnickname.hpp"
#include "factionupgrade.hpp"
#include "factionresign.hpp"
#include "factionrecruit.hpp"
#include "factionappoint.hpp"
#include "factionannounce.hpp"
#include "factioninvite.hrp"

#include "chatroomcreate.hpp"
#include "chatroomexpel.hpp"
#include "chatroominvite.hpp"
#include "chatroominvite_re.hpp"
#include "chatroomjoin.hpp"
#include "chatroomleave.hpp"
#include "chatroomlist.hpp"
#include "chatroomspeak.hpp"

#include "checknewmail.hpp"
#include "getmail.hpp"
#include "getmaillist.hpp"
#include "playersendmail.hpp"
#include "getmailattachobj.hpp"
#include "preservemail.hpp"
#include "deletemail.hpp"

#include "auctionattendlist.hpp"
#include "auctionbid.hpp"
#include "auctionclose.hpp"
#include "auctionget.hpp"
#include "auctionlist.hpp"
#include "auctionopen.hpp"
#include "auctionexitbid.hpp"
#include "auctiongetitem.hpp"

#include "stockaccount.hpp"
#include "stockbill.hpp"
#include "stockcommission.hpp"
#include "stockcancel.hpp"

#include "battleenter.hpp"
#include "battlegetlist.hpp"
#include "battlegetfield.hpp"
#include "battlejoin.hpp"
#include "battleleave.hpp"

#include "combatchallenge.hpp"
#include "combattop.hpp"
#include "combatinviteres"

#include "siegechallenge.hpp"
#include "siegeenter.hpp"
#include "siegeinfoget.hpp"
#include "siegesetassistant.hpp"
#include "siegetop.hpp"

#include "getselllist.hpp"
#include "findsellpointinfo.hpp"
#include "sellpoint.hpp"
#include "sellcancel.hpp"
#include "buypoint.hpp"
#include "gettoptable.hpp"
#include "getenemies.hpp"
#include "updateenemy.hpp"
#include "matrixresponse.hpp"
#include "autolockset.hpp"

#include "sectlist.hpp"
#include "sectexpel.hpp"
#include "sectrecruit.hpp"
#include "sectinvite.hrp"

#include "contestinvite_re.hpp"
#include "contestanswer.hpp"
#include "contestexit.hpp"

#include "instancinggetlist.hpp"
#include "instancinggetfield.hpp"
#include "instancingkick.hpp"
#include "instancingstart.hpp"
#include "instancingaccept.hpp"
#include "instancingleave.hpp"
#include "instancingenter.hpp"
#include "instancingavailablelist.hpp"
#include "instancingappoint.hpp"
#include "instancingavailablelist_re.hpp"
#include "instancingappoint_re.hpp"
#include "instancingappointnotify.hpp"
#include "instancinggetlist_re.hpp"
#include "instancinggetfield_re.hpp"
#include "instancingkick_re.hpp"
#include "instancingstart_re.hpp"
#include "instancingjoinreq.hpp"
#include "instancingjoin_re.hpp"
#include "instancingleave_re.hpp"
#include "instancingenter_re.hpp"
#include "instancingstartnotify.hpp"
#include "instancingclosenotify.hpp"

//副本相关协议 Added 2011-07-21.
#include "raidappoint.hpp"
#include "raidappoint_re.hpp"
#include "raidappointnotify.hpp"
#include "raidenter.hpp"
#include "raidenter_re.hpp"
#include "raidgetlist.hpp"
#include "raidgetlist_re.hpp"
#include "raidgetroom.hpp"
#include "raidgetroom_re.hpp"
#include "raidjoin_re.hpp"
#include "raidjoinaccept.hpp"
#include "raidjoinaccept_re.hpp"
#include "raidjoinapplylist.hpp"
#include "raidjoinreq.hpp"
#include "raidkick.hpp"
#include "raidkick_re.hpp"
#include "raidkicknotify.hpp"
#include "raidopen_re.hpp"
#include "raidquit.hpp"
#include "raidquit_re.hpp"
#include "raidstart.hpp"
#include "raidstart_re.hpp"
#include "raidstartnotify.hpp"
#include "raidopenvote.hpp"
#include "raidvotes.hpp"
#include "teamraidquit.hpp"
//Added end.

#include "getcommondata.hpp"

#include "snssetplayerinfo.hpp"
#include "snsgetplayerinfo.hpp"
#include "snscancelmessage.hpp"
#include "snslistmessage.hpp"
#include "snsgetmessage.hpp"
#include "snsacceptapply.hpp"
#include "snscancelleavemessage.hpp"

#include "snssetplayerinfo_re.hpp"
#include "snspressmessage_re.hpp"
#include "snscancelmessage_re.hpp"
#include "snsapply_re.hpp"
#include "snslistmessage_re.hpp"
#include "snsgetmessage_re.hpp"
#include "snsgetplayerinfo_re.hpp"
#include "snsvote_re.hpp"
#include "snsacceptapply_re.hpp"
#include "snscancelleavemessage_re.hpp"

#include "reflistreferrals.hpp"
#include "refwithdrawexp.hpp"
#include "refwithdrawbonus.hpp"
#include "refgetreferencecode.hpp"
#include "reflistreferrals_re.hpp"
#include "refwithdrawexp_re.hpp"
#include "refwithdrawbonus_re.hpp"
#include "refgetreferencecode_re.hpp"

#include "territorymapget.hpp"
#include "uniquebidhistory.hpp"

#include "circlelist.hpp"
#include "getcirclebaseinfo.hpp"
#include "claimcirclegradbonus.hpp"
#include "gcirclechat.hpp"

#include "consignquery.hpp"
#include "consigncancelpost.hpp"

#include "friendcallbackinfo.hpp"
#include "friendcallbackmail.hpp"
#include "friendcallbackaward.hpp"

#include "trychangegs.hpp"
#include "dochangegs.hpp"
#include "linelist.hpp"
#include "getfriendgtstatus.hpp"
#include "getkdctoken.hpp"

#include "fastpay.hpp"

#include "factionpkraidchallenge.hpp"
#include "factionpkraidchallengecancel.hpp"
#include "factionpkraidgetlist.hpp"
#include "factionpkraidgetscore.hpp"
#include "factionpkraidinvite_re.hpp"
#include "factionwithdrawbonus.hpp"

#include "kingdomgetinfo.hpp"
#include "kingdomappoint.hpp"
#include "kingdomdischarge.hpp"
#include "kingdomsetgameattri.hpp"
#include "kingdomannounce.hpp"
#include "kingdomgettitle.hpp"
#include "kingguardinvite.hrp"
#include "queentryopenbath.hpp"
#include "queenopenbathinvite.hrp"
#include "getkingtask.hpp"

#include "gettouchpoint.hpp"
#include "getpassportcashadd.hpp"

#include "acclienttimeout.hpp"
//2013-3-1 by zy add 服务器消息
#include "topflowerreq.hpp"		
#include "sendflowerreq.hpp"
#include "topflowergetgift.hpp"
#include "openbanquetenter.hpp"
#include "openbanquetcancel.hpp"
#include "openbanquetqueuecount.hpp"
#include "openbanquetgetfield.hpp"

#include "getfactiondynamic.hpp"
#include "getfactionmsg.hpp"
#include "factionpostmsg.hpp"
#include "createfacbase.hpp"
#include "getfacacttoptable.hpp"

#include "crssvrteamschgcaptain.hpp"
#include "crssvrteamsdismiss.hpp"
#include "crssvrteamsgetrolelist.hpp"
#include "crssvrteamsgetteambase.hpp"
#include "crssvrteamsinvite.hrp"
#include "crssvrteamsleave.hpp"
#include "crssvrteamsrecruit.hpp"
#include "crssvrteamsrename.hpp"
#include "crssvrteamskick.hpp"
#include "crssvrteamssearch.hpp"
#include "crssvrteamsgetrolenotify.hpp"
#include "facbasedatasend.hpp"
#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Reference to External variables and functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local Types and Variables and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	GM Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::gm_KickOutRole(int idPlayer, int iTime, const ACHAR* szReason)
{
	GNET::GMKickoutRole p;
	p.gmroleid = m_iCharID;
	p.kickroleid = idPlayer;
	p.forbid_time = iTime;

	if (szReason && szReason[0])
	{
		int iLen = a_strlen(szReason) * sizeof (ACHAR);
		p.reason.replace(szReason, iLen);
	}

	SendNetData(p);
}

void CECGameSession::gm_KickOutUser(int idPlayer, int iTime, const ACHAR* szReason)
{
	GNET::GMKickoutUser p;
	p.gmroleid = m_iCharID;
	p.kickuserid = idPlayer;
	p.forbid_time = iTime;

	if (szReason && szReason[0])
	{
		int iLen = a_strlen(szReason) * sizeof (ACHAR);
		p.reason.replace(szReason, iLen);
	}

	SendNetData(p);
}

void CECGameSession::gm_ListOnlineUser(int iHandler)
{
	GNET::GMListOnlineUser p;
	p.gmroleid = m_iCharID;
	p.handler = iHandler;
	SendNetData(p);
}

void CECGameSession::gm_OnlineNumber()
{
	GNET::GMOnlineNum p;
	p.gmroleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::gm_RestartServer(int idServer, int iRestartTime)
{
	GNET::GMRestartServer p;
	p.gmroleid = m_iCharID;
	p.gsid = idServer;
	p.restart_time = iRestartTime;
	SendNetData(p);
}

void CECGameSession::gm_ShutupRole(int idPlayer, int iTime, const ACHAR* szReason)
{
	GNET::GMShutupRole p;
	p.gmroleid = m_iCharID;
	p.dstroleid = idPlayer;
	p.forbid_time = iTime;

	if (szReason && szReason[0])
	{
		int iLen = a_strlen(szReason) * sizeof (ACHAR);
		p.reason.replace(szReason, iLen);
	}

	SendNetData(p);
}

void CECGameSession::gm_ShutupUser(int idPlayer, int iTime, const ACHAR* szReason)
{
	GNET::GMShutup p;
	p.gmroleid = m_iCharID;
	p.dstuserid = idPlayer;
	p.forbid_time = iTime;

	if (szReason && szReason[0])
	{
		int iLen = a_strlen(szReason) * sizeof (ACHAR);
		p.reason.replace(szReason, iLen);
	}

	SendNetData(p);
}

void CECGameSession::gm_ForbidRole(int iType, int idPlayer, int iTime, const ACHAR* szReason)
{
	GNET::GMForbidRole p;
	p.fbd_type = (BYTE)iType;
	p.gmroleid = m_iCharID;
	p.dstroleid = idPlayer;
	p.forbid_time = iTime;

	if (szReason && szReason[0])
	{
		int iLen = a_strlen(szReason) * sizeof (ACHAR);
		p.reason.replace(szReason, iLen);
	}

	SendNetData(p);
}

void CECGameSession::gm_TriggerChat(bool bEnable)
{
	GNET::GMToggleChat p;
	p.gmroleid = m_iCharID;
	p.enable = bEnable ? 1 : 0;
	SendNetData(p);
}

void CECGameSession::OnPrtcGMKickOutRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMKickoutRole_Re* p = (GMKickoutRole_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_KICKOUTROLE, p->kickroleid);
}

void CECGameSession::OnPrtcGMKickOutUserRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMKickoutUser_Re* p = (GMKickoutUser_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_KICKOUTUSER, p->kickuserid);
}

void CECGameSession::OnPrtcGMListOnlineUserRe(GNET::Protocol* pProtocol)
{
}

void CECGameSession::OnPrtcGMOnlineNumberRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMOnlineNum_Re* p = (GMOnlineNum_Re*)pProtocol;
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ONLINENUMBER, p->local_num, p->total_num);
}

void CECGameSession::OnPrtcGMRestartServerRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMRestartServer_Re* p = (GMRestartServer_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_RESTARTSEV);
}

void CECGameSession::OnPrtcGMRestartServer(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMRestartServer* p = (GMRestartServer*)pProtocol;
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_SERVERCLOSE, p->restart_time);
}

void CECGameSession::OnPrtcGMShutupRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMShutupRole_Re* p = (GMShutupRole_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_SHUTUPROLE, p->dstroleid, p->forbid_time);
}

void CECGameSession::OnPrtcGMShutupUserRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMShutup_Re* p = (GMShutup_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_SHUTUPUSER, p->dstuserid, p->forbid_time);
}

void CECGameSession::OnPrtcGMForbidRoleRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMForbidRole_Re* p = (GMForbidRole_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_OPTSUCCESS);
}

void CECGameSession::OnPrtcGMToggleChatRe(GNET::Protocol* pProtocol)
{
	using namespace GNET;
	GMToggleChat_Re* p = (GMToggleChat_Re*)pProtocol;
	if (p->retcode == ERR_SUCCESS)
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_OPTSUCCESS);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Trade Protocols
//	
///////////////////////////////////////////////////////////////////////////

bool CECGameSession::trade_Start(int idTarget)
{
	GNET::TradeStart p;
	p.roleid = m_iCharID;
	p.partner_roleid = idTarget;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_StartResponse(DWORD& dwHandle, bool bAgree)
{
	using namespace GNET;

	if (!dwHandle)
	{
		ASSERT(0);
		return false;
	}

	Rpc* p = (Rpc*)dwHandle;
	TradeStartRqstRes* pResult = (TradeStartRqstRes*)p->GetResult();
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_TRADE_AGREE : ERR_TRADE_REFUSE;
		SendNetData(p);
	}

	p->Destroy();
	dwHandle = 0;

	return true;
}

bool CECGameSession::trade_AddGoods(int idTrade, int idItem, int iPos, int iCount, int iMoney)
{
	GNET::TradeAddGoods p;
	p.tid = idTrade;
	p.roleid = m_iCharID;
	p.money = iMoney;
	p.goods.id = idItem;
	p.goods.pos = iPos;
	p.goods.count = iCount;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_RemoveGoods(int idTrade, int idItem, int iPos, int iCount, int iMoney)
{
	GNET::TradeRemoveGoods p;
	p.tid = idTrade;
	p.roleid = m_iCharID;
	p.money = iMoney;
	p.goods.id = idItem;
	p.goods.pos = iPos;
	p.goods.count = iCount;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_MoveItem(int idTrade, int idItem, int iSrc, int iDst, int iCount)
{
	FreezeHostItem(IVTRTYPE_PACK, iSrc, true);
	FreezeHostItem(IVTRTYPE_PACK, iDst, true);

	GNET::TradeMoveObj p;
	p.roleid = m_iCharID;
	p.tid = idTrade;
	p.dst_pos = iDst;
	p.goods.id = idItem;
	p.goods.pos = iSrc;
	p.goods.count = iCount;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_Submit(int idTrade)
{
	GNET::TradeSubmit p;
	p.tid = idTrade;
	p.roleid = m_iCharID;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_Confirm(int idTrade)
{
	//	Freeze items
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	CECDealInventory* pDealPack = pHost->GetDealPack();
	for (int i=0; i < pDealPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = pDealPack->GetItem(i);
		if (pItem)
			pItem->NetFreeze(true);
	}

	GNET::TradeConfirm p;
	p.tid = idTrade;
	p.roleid = m_iCharID;

	SendNetData(p);
	return true;
}

bool CECGameSession::trade_Cancel(int idTrade)
{
	GNET::TradeDiscard p;
	p.tid = idTrade;
	p.roleid = m_iCharID;

	SendNetData(p);
	return true;
}

void CECGameSession::OnTradeProtocols(GNET::Protocol* pProtocol)
{
	ASSERT(pProtocol);

	int iMsg;

	switch (pProtocol->GetType())
	{
	case PROTOCOL_TRADESTART_RE:		iMsg = MSG_HST_TRADESTART;		break;
	case RPC_TRADESTARTRQST:			iMsg = MSG_HST_TRADEREQUEST;	break;
	case PROTOCOL_TRADEMOVEOBJ_RE:		iMsg = MSG_HST_TRADEMOVEITEM;	break;
	case PROTOCOL_TRADEADDGOODS_RE:		iMsg = MSG_HST_TRADEADDGOODS;	break;
	case PROTOCOL_TRADEREMOVEGOODS_RE:	iMsg = MSG_HST_TRADEREMGOODS;	break;
	case PROTOCOL_TRADEDISCARD_RE:		iMsg = MSG_HST_TRADECANCEL;		break;
	case PROTOCOL_TRADESUBMIT_RE:		iMsg = MSG_HST_TRADESUBMIT;		break;
	case PROTOCOL_TRADECONFIRM_RE:		iMsg = MSG_HST_TARDECONFIRM;	break;
	case PROTOCOL_TRADEEND:				iMsg = MSG_HST_TRADEEND;		break;

	default:
		ASSERT(0);
		return;
	}

	//	When player return login interface from game, pWorld may be NULL.
	g_pGame->GetGameRun()->PostMessage(iMsg, MAN_PLAYER, 0, (DWORD)pProtocol);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Friend Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::friend_Add(int idPlayer, const ACHAR* szName)
{
	GNET::AddFriend p;
	p.srcroleid = m_iCharID;
	p.dstroleid = idPlayer;

	if (szName)
	{
		int iLen = a_strlen(szName) * sizeof (ACHAR);
		p.dstname.replace(szName, iLen);
	}

	SendNetData(p);
}

void CECGameSession::friend_AddResponse(DWORD& dwHandle, bool bAgree)
{
	using namespace GNET;

	if (!dwHandle)
	{
		ASSERT(0);
		return;
	}

	Rpc* p = (Rpc*)dwHandle;
	AddFriendRqstRes* pResult = (AddFriendRqstRes*)p->GetResult();
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_TRADE_AGREE : ERR_TRADE_REFUSE;
		SendNetData(p);
	}
	
	p->Destroy();
	dwHandle = 0;
}

void CECGameSession::friend_GetList()
{
	GNET::GetFriends p;
	p.roleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::friend_SetGroupName(int iGroup, const ACHAR* szName)
{
	GNET::SetGroupName p;
	p.roleid = m_iCharID;
	p.gid = (char)iGroup;

	if (szName)
	{
		int iLen = a_strlen(szName) * sizeof (ACHAR);
		p.name.replace(szName, iLen);
	}

	SendNetData(p);
}

void CECGameSession::friend_SetGroup(int iGroup, int idFriend)
{
	GNET::SetFriendGroup p;
	p.roleid = m_iCharID;
	p.gid = (char)iGroup;
	p.friendid = idFriend;

	SendNetData(p);
}

void CECGameSession::friend_Delete(int idFriend)
{
	GNET::DelFriend p;
	p.roleid = m_iCharID;
	p.friendid = idFriend;
	
	SendNetData(p);
}

void CECGameSession::friend_GetOfflineMsg()
{
	GNET::GetSavedMsg p;
	p.roleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::OnFriendProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_FRIENDOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Faction Protocols
//	
///////////////////////////////////////////////////////////////////////////

inline size_t a_str_size(const ACHAR* sz)
{
	return a_strlen(sz) * sizeof(ACHAR);
}

void CECGameSession::OnFactionProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_FACTION, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

void CECGameSession::faction_chat(const ACHAR* szMsg, char channel, int nPack, int nSlot, int nAchievement)
{
	using namespace CHAT_C2S;
	Octets data;

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		data.replace(&c, sizeof(c));
	}
	else if (nAchievement >= 0)
	{
		chat_achievement c;
		c.cmd_id = CHAT_ACHIEVEMENT;
		c.achieve_id = nAchievement;
		data.replace(&c, sizeof(c));
	}

	SendNetData(GNET::FactionChat(m_iCharID, channel, 0, Octets(szMsg, a_str_size(szMsg)), data));
}

void CECGameSession::faction_create(const ACHAR* szName, const ACHAR* szProclaim, char scale)
{
	GNET::FactionCreate p(m_iCharID, Octets(szName, a_str_size(szName)), scale);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevFaction(GNET::PROTOCOL_FACTIONCREATE, o.begin(), o.size());
}

void CECGameSession::faction_dismiss(char scale, bool del)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::FactionDismiss p(scale ? pHost->GetFamilyID() : pHost->GetFactionID(), m_iCharID, scale, del ? 1 : 0);
	
	if (del)
	{
		Octets o = p.marshal(Marshal::OctetsStream());
		c2s_CmdNPCSevFaction(GNET::PROTOCOL_FACTIONDISMISS, o.begin(), o.size());
	}
	else
		SendNetData(p);
}

void CECGameSession::faction_upgrade(char scale)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost->GetFactionID() == 0) return;
	GNET::FactionUpgrade p(pHost->GetFactionID(), m_iCharID);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevFaction(GNET::PROTOCOL_FACTIONUPGRADE, o.begin(), o.size());
}

void CECGameSession::faction_degrade()
{
	/*
	Octets o = GNET::degrade_param_ct().marshal();
	c2s_CmdNPCSevFaction(GNET::_O_FACTION_DEGRADE, o.begin(), o.size());
	*/
}

void CECGameSession::faction_change_proclaim(const ACHAR* szNew, char scale)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::FactionAnnounce(m_iCharID, scale ? pHost->GetFamilyID() : pHost->GetFactionID(), Octets(szNew, a_str_size(szNew)), scale));
}

void CECGameSession::faction_accept_join(int iApplicant, char scale)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::FactionRecruit(m_iCharID, scale ? pHost->GetFamilyID() : pHost->GetFactionID(), iApplicant, scale));
}

void CECGameSession::faction_expel_member(int iMember, char scale)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::FactionExpel(m_iCharID, scale ? pHost->GetFamilyID() : pHost->GetFactionID(), iMember, scale));
}

void CECGameSession::faction_appoint(int iMember, char new_position)
{
	m_pNetMan->Send(m_idLink, GNET::FactionAppoint(m_iCharID, iMember, new_position));
}

void CECGameSession::faction_master_resign(int iNewMaster, char scale)
{
	m_pNetMan->Send(m_idLink, GNET::FactionAppoint(m_iCharID, iNewMaster, scale ? _R_HEADER : _R_MASTER, scale));
}

void CECGameSession::faction_resign(char scale)
{
	m_pNetMan->Send(m_idLink, GNET::FactionResign(m_iCharID, scale));
}

void CECGameSession::faction_leave(char scale)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::FactionLeave(scale ? pHost->GetFamilyID() : pHost->GetFactionID(), m_iCharID, scale));
}

void CECGameSession::faction_broadcast(const ACHAR* szMsg)
{
	/*
	m_pNetMan->Send(
		m_idLink,
		GNET::FactionOPRequest(
			GNET::_O_FACTION_BROADCAST,
			m_iCharID,
			GNET::broadcast_param_ct(Octets(szMsg, a_str_size(szMsg))).marshal()));
			*/
}

void CECGameSession::faction_listmember(int page, unsigned int familyid, bool first_time)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::FactionList(page, m_iCharID, pHost->GetFactionID(), familyid, first_time ? 1 : 0));
}

void CECGameSession::faction_rename(int iRole, const ACHAR* szNewName)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if (pHost->GetFactionID() == 0) return;
	m_pNetMan->Send(m_idLink, GNET::FactionNickname(pHost->GetFactionID(), 0, m_iCharID, iRole, Octets(szNewName, a_str_size(szNewName))));
}


void CECGameSession::faction_invite_response(DWORD& dwHandle, bool bAgree, char scale)
{
	using namespace GNET;
	assert(dwHandle);

	if (dwHandle == 0)
		return;

	Rpc* p = reinterpret_cast<Rpc*>(dwHandle);
	FactionInviteRes* pResult = static_cast<FactionInviteRes*>(p->GetResult());

	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_SUCCESS : ERR_FC_JOIN_REFUSE;
		pResult->scale = scale;
		SendNetData(p);
	}

	p->Destroy();
	dwHandle = 0;
}

void CECGameSession::faction_player_info()
{
	// SendNetData(GNET::GetPlayerFactionInfo(m_iCharID, 0));
}

///////////////////////////////////////////////////////////////////////////
//	
//	Chatroom Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::chatroom_Create(const ACHAR* szSubject, const ACHAR* szPassword, int iCapacity)
{
	GNET::ChatRoomCreate p;
	p.roleid = m_iCharID;
	p.capacity = (unsigned short)iCapacity;

	if (szSubject && szSubject[0])
	{
		int iLen = a_strlen(szSubject) * sizeof (ACHAR);
		p.subject.replace(szSubject, iLen);
	}

	if (szPassword && szPassword[0])
	{
		int iLen = a_strlen(szPassword) * sizeof (ACHAR);
		p.password.replace(szPassword, iLen);
	}
	
	SendNetData(p);
}

void CECGameSession::chatroom_Expel(int idRoom, int idPlayer)
{
	GNET::ChatRoomExpel p;
	p.roleid = idPlayer;
	p.roomid = idRoom;
	p.owner = m_iCharID;
	
	SendNetData(p);
}

void CECGameSession::chatroom_Invite(int idRoom, int idInvitee)
{
	GNET::ChatRoomInvite p;
	p.invitee = idInvitee;
	p.roomid = idRoom;
	p.inviter = m_iCharID;
	
	SendNetData(p);
}

void CECGameSession::chatroom_Invite(int idRoom, const ACHAR* szName)
{
	if (szName && szName[0])
	{
		GNET::ChatRoomInvite p;
		p.invitee = 0;
		p.roomid = idRoom;
		p.inviter = m_iCharID;
		int iLen = a_strlen(szName) * sizeof (ACHAR);
		p.name.replace(szName, iLen);
		SendNetData(p);
	}
}

void CECGameSession::chatroom_RejectInvite(int idRoom, int idInviter)
{
	GNET::ChatRoomInvite_Re p;
	p.invitee = m_iCharID;
	p.roomid = idRoom;
	p.inviter = idInviter;
//	p.retcode = GNET::ERR_CHAT_INVITE_REFUSED;
	
	SendNetData(p);
}

void CECGameSession::chatroom_Join(int idRoom, const ACHAR* szPassword)
{
	GNET::ChatRoomJoin p;
	p.roomid = idRoom;
	p.roleid = m_iCharID;
	
	if (szPassword && szPassword[0])
	{
		int iLen = a_strlen(szPassword) * sizeof (ACHAR);
		p.password.replace(szPassword, iLen);
	}

	SendNetData(p);
}

void CECGameSession::chatroom_Join(const ACHAR* szOwner, const ACHAR* szPassword)
{
	GNET::ChatRoomJoin p;
	p.roomid = 0;
	p.ownerid = 0;
	p.roleid = m_iCharID;
	
	if (!szOwner || !szOwner[0])
		return;
	else
	{
		int iLen = a_strlen(szOwner) * sizeof (ACHAR);
		p.ownername.replace(szOwner, iLen);
	}
		
	if (szPassword && szPassword[0])
	{
		int iLen = a_strlen(szPassword) * sizeof (ACHAR);
		p.password.replace(szPassword, iLen);
	}

	SendNetData(p);
}

void CECGameSession::chatroom_Leave(int idRoom)
{
	GNET::ChatRoomLeave p;
	p.roomid = idRoom;
	p.roleid = m_iCharID;
	
	SendNetData(p);
}

void CECGameSession::chatroom_List(int iBegin, bool bReverse)
{
	GNET::ChatRoomList p;
	p.begin = iBegin;
	p.reverse = bReverse ? 1 : 0;
	
	SendNetData(p);
}

void CECGameSession::chatroom_Speak(int idRoom, int idDstPlayer, const ACHAR* szMsg)
{
	GNET::ChatRoomSpeak p;
	p.roomid = idRoom;
	p.src = m_iCharID;
	p.dst = idDstPlayer;
	
	if (!szMsg || !szMsg[0])
		return;
	else
	{
		int iLen = a_strlen(szMsg) * sizeof (ACHAR);
		p.message.replace(szMsg, iLen);
	}

	SendNetData(p);
}

void CECGameSession::OnChatRoomProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_CHATROOMOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Mail Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::mail_CheckNew()
{
	GNET::CheckNewMail p;
	p.roleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::mail_GetList()
{
	GNET::GetMailList p;
	p.roleid = m_iCharID;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_GETMAILLIST, o.begin(), o.size());
}

void CECGameSession::mail_Get(int idMail)
{
	GNET::GetMail p;
	p.roleid = m_iCharID;
	p.mail_id = (BYTE)idMail;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_GETMAIL, o.begin(), o.size());
	LuaEvent::FirstReceiveMail();
}

void CECGameSession::mail_GetAttachment(int idMail, int iType)
{
	GNET::GetMailAttachObj p;
	p.roleid = m_iCharID;
	p.mail_id = (BYTE)idMail;
	p.obj_type = (BYTE)iType;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_GETMAILATTACHOBJ, o.begin(), o.size());
}

void CECGameSession::mail_Delete(int idMail)
{
	GNET::DeleteMail p;
	p.roleid = m_iCharID;
	p.mail_id = (BYTE)idMail;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_DELETEMAIL, o.begin(), o.size());
}

void CECGameSession::mail_Preserve(int idMail, bool bPreserve)
{
	GNET::PreserveMail p;
	p.roleid = m_iCharID;
	p.mail_id = (BYTE)idMail;
	p.blPreserve = bPreserve ? 1 : 0;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_PRESERVEMAIL, o.begin(), o.size());
}

void CECGameSession::mail_Send(int idReceiver, const ACHAR* szTitle, const ACHAR* szContext,
						int idItem, int iItemNum, int iItemPos, size_t dwMoney)
{
	if (idItem > 0 && iItemNum > 0 && iItemPos >= 0)
		FreezeHostItem(IVTRTYPE_PACK, iItemPos, true);

	GNET::PlayerSendMail p;
	p.roleid = m_iCharID;
	p.receiver = idReceiver;
	p.attach_obj_id = idItem;
	p.attach_obj_num = iItemNum;
	p.attach_obj_pos = iItemPos;
	p.attach_money = dwMoney;

	if (szTitle)
	{
		int iLen = a_strlen(szTitle) * sizeof (ACHAR);
		p.title.replace(szTitle, iLen);
	}

	if (szContext)
	{
		int iLen = a_strlen(szContext) * sizeof (ACHAR);
		p.context.replace(szContext, iLen);
	}
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevMail(GNET::PROTOCOL_PLAYERSENDMAIL, o.begin(), o.size());
	LuaEvent::FirstSendMail();
}

void CECGameSession::OnMailProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_MAILOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Auction Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::auction_Open(int iCategory, int tid, int iItemPos, int iItemNum,
							int iBasePrice, int iBinPrice, int iTime)
{
	GNET::AuctionOpen p;
	p.roleid = m_iCharID;
	p.category = iCategory;
	p.item_id = tid;
	p.item_pos = iItemPos;
	p.item_num = iItemNum;
	p.baseprice = iBasePrice;
	p.binprice = iBinPrice;
	p.elapse_time = iTime;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONOPEN, o.begin(), o.size());
}

void CECGameSession::auction_Bid(int idAuction, int iBidPrice, bool bBin)
{
	GNET::AuctionBid p;
	p.roleid = m_iCharID;
	p.auctionid = idAuction;
	p.bidprice = iBidPrice;
	p.bin = bBin ? 1 : 0;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONBID, o.begin(), o.size());
}

void CECGameSession::auction_List(int iCategory, int iBegin, bool bReverse)
{
	GNET::AuctionList p;
	p.roleid = m_iCharID;
	p.category = iCategory;
	p.begin = iBegin;
	p.reverse = bReverse ? 1 : 0;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONLIST, o.begin(), o.size());
}

void CECGameSession::auction_Close(int idAuction, int iReason)
{
	GNET::AuctionClose p;
	p.roleid = m_iCharID;
	p.auctionid = idAuction;
	p.reason = (BYTE)iReason;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONCLOSE, o.begin(), o.size());
}

void CECGameSession::auction_Get(int idAuction)
{
	GNET::AuctionGet p;
	p.roleid = m_iCharID;
	p.auctionid = idAuction;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONGET, o.begin(), o.size());
}

void CECGameSession::auction_AttendList()
{
	GNET::AuctionAttendList p;
	p.roleid = m_iCharID;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONATTENDLIST, o.begin(), o.size());
}

void CECGameSession::auction_ExitBid(int idAuction)
{
	GNET::AuctionExitBid p;
	p.roleid = m_iCharID;
	p.auctionid = idAuction;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONEXITBID, o.begin(), o.size());
}

void CECGameSession::auction_GetItems(int iNumItem, int* aAuctionIDs)
{
	if (!iNumItem || !aAuctionIDs)
		return;

	GNET::AuctionGetItem p;
	p.roleid = m_iCharID;
	
	for (int i=0; i < iNumItem; i++)
		p.ids.push_back(aAuctionIDs[i]);

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevVendue(GNET::PROTOCOL_AUCTIONGETITEM, o.begin(), o.size());
}

void CECGameSession::OnVendueProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_VENDUEOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Stock Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::stock_account()
{
	GNET::StockAccount p(m_iCharID);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevStockOperation(GNET::PROTOCOL_STOCKACCOUNT, o.begin(), o.size());
}

void CECGameSession::stock_bill()
{
	GNET::StockBill p(m_iCharID);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevStockOperation(GNET::PROTOCOL_STOCKBILL, o.begin(), o.size());
}

void CECGameSession::stock_commission(char bBuy, int iPrice, int iVolume)
{
	GNET::StockCommission p(m_iCharID, bBuy, iPrice, iVolume);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevStockOperation(GNET::PROTOCOL_STOCKCOMMISSION, o.begin(), o.size());
}

void CECGameSession::stock_cancel(int tid, int price)
{
	GNET::StockCancel p(m_iCharID, tid, price);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevStockOperation(GNET::PROTOCOL_STOCKCANCEL, o.begin(), o.size());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Battle Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::battle_getlist(char battle_type)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::BattleGetList(pHost->GetCharacterID(), battle_type, 0));
}

void CECGameSession::battle_getfield(int line_id, int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::BattleGetField(pHost->GetCharacterID(), line_id, map_id, 0,pHost->GetBattleFlag())); // 2012-8-29 add zy 根据消息修改参数
}

void CECGameSession::battle_join(int line_id, int map_id, unsigned char team, char isreenter)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::BattleJoin(pHost->GetCharacterID(), line_id, map_id, pHost->GetBattleFlag(),team, 0, isreenter)); // 2012-8-29 add zy 根据消息修改参数
}

void CECGameSession::battle_enter(int line_id, int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::BattleEnter(pHost->GetCharacterID(), line_id, map_id, 0,pHost->GetBattleType(),pHost->GetBattleFlag())); // 2012-8-29 add zy 根据消息修改参数
}


void CECGameSession::battle_leave()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::BattleLeave(pHost->GetCharacterID(), 0,pHost->GetBattleFlag()));// 2012-8-29 add zy 根据消息修改参数
}

void CECGameSession::OnBattleProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_BATTLEOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

void CECGameSession::war_challege(int battle_id, int day)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::SiegeChallenge p(pHost->GetCharacterID(), battle_id, pHost->GetFactionID(), day);
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_SendCmdNPCSevWarChallenge(GNET::PROTOCOL_SIEGECHALLENGE, o.begin(), o.size());
}

void CECGameSession::war_enter(int battle_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SiegeEnter(pHost->GetCharacterID(), battle_id));
}

void CECGameSession::war_infoget(int battle_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SiegeInfoGet(pHost->GetCharacterID(), battle_id, 0));
}

void CECGameSession::war_top(int battle_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SiegeTop(pHost->GetCharacterID(), battle_id, 0));
}

void CECGameSession::war_assistant(int battle_id, const ACHAR* szFactionName)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::SiegeSetAssistant p(pHost->GetCharacterID(), battle_id, GNET::Octets(szFactionName, a_str_size(szFactionName)));
	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_SendCmdNPCSevWarChallenge(GNET::PROTOCOL_SIEGESETASSISTANT, o.begin(), o.size());
}

void CECGameSession::war_buy_archer()
{
	_SendEmptyDataNPCSev(GP_NPCSEV_WAR_ARCHER);
}

void CECGameSession::war_get_construction_info(int mode, int type, int key, int cur_level/*=0*/)
{
	using namespace C2S;

#pragma pack(1)
	
	struct CONTENT
	{
		int mode;       //0 察看信息, 1 捐献, 2 建设
        int type;       //mode为0时：type为0取建筑信息，type为1取玩家捐献信息；mode为 1 时 这个表示捐献物资id,  mode为 2 时 这个表示建设的类型
        int key;        //mode为0，type为1时：key表示取哪页,每页10个,0开始)；  mode为 1 时 这个表示捐献物资key, mode为 2 时 这个表示建设的key
		int cur_level;	//mode为2，type为0时：cur_level表示当前建筑的级别
	};   

#pragma pack()

	int iSize = sizeof (cmd_header) + sizeof (cmd_sevnpc_serve) + sizeof (CONTENT);
	BYTE* pBuf = new BYTE [iSize];
	if (!pBuf)
		return;
	
	((cmd_header*)pBuf)->cmd = C2S::SEVNPC_SERVE;

	cmd_sevnpc_serve* pCmd = (cmd_sevnpc_serve*)(pBuf + sizeof (cmd_header));
	pCmd->service_type = GP_NPCSEV_WAR_CONSTRUCTION;
	pCmd->len = sizeof (CONTENT);

	CONTENT* pContent = (CONTENT*)(pBuf + sizeof (cmd_header) + sizeof (cmd_sevnpc_serve));
	pContent->mode = mode;
	pContent->type = type;
	pContent->key = key;
	pContent->cur_level = cur_level;

	g_pGame->GetGameSession()->SendGameData(pBuf, iSize);

	delete [] pBuf;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Instancing Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::instancing_availablelist()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingAvailableList(pHost->GetCharacterID(), 0));
}

void CECGameSession::instancing_getlist(int battle_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingGetList(pHost->GetCharacterID(), battle_id, 0));	
}

void CECGameSession::instancing_getfield(int line_id, int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingGetField(pHost->GetCharacterID(), line_id, map_id, 0));
}

void CECGameSession::instancing_enter(int line_id, int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingEnter(pHost->GetCharacterID(), line_id, map_id, 0));
}

// 移交首领权限
void CECGameSession::instancing_appoint(int map_id, int target_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingAppoint(pHost->GetCharacterID(), map_id, target_id, 0));
}
void CECGameSession::instancing_kick(int map_id, int target)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingKick(pHost->GetCharacterID(), map_id, target, 0));
}

void CECGameSession::instancing_accept(bool bAccept, int map_id, int requester)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingAccept(bAccept? 1:0, pHost->GetCharacterID(), map_id, requester));
}

void CECGameSession::instancing_leave(int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingLeave(pHost->GetCharacterID(), map_id, 0));
}

void CECGameSession::instancing_start(int map_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::InstancingStart(pHost->GetCharacterID(), map_id, 0));
}

void CECGameSession::OnInstancingProtocols(GNET::Protocol * pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_INSTANCINGOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//
//@description: 副本相关协议（client -> deliver）
//@date: 2011-07-21
///////////////////////////////////////////////////////////////////////////
/**
*@param: map_id: 副本地图ID
*@return: void
*@usage: 获得副本房间信息列表（，当前还未开启的副本）
**/
void CECGameSession::raid_getlist( int map_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidGetList(pHost->GetCharacterID(), map_id, 0));
}

/**
*@param: raid_room_id: 副本房间ID，map_id: 副本地图号
*@return: void
*@usage: 开启副本(，由房间主人发起?)
**/
void CECGameSession::raid_start( int raid_room_id, int map_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidStart(pHost->GetCharacterID(), raid_room_id, map_id, 0));
}

void CECGameSession::raid_start_tower( int raid_room_id, int map_id, int lv)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	GNET::RaidStart raidStart(pHost->GetCharacterID(), raid_room_id, map_id, 
	0, 0, Octets(&lv, 4));
	m_pNetMan->Send(m_idLink, raidStart);
}

/**
*@param: raid_room_id: 副本房间ID，map_id: 副本地图号
*@return: void
*@usage: 进入副本
**/
void CECGameSession::raid_enter( int raid_room_id, int map_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidEnter(pHost->GetCharacterID(), map_id, raid_room_id, 0));
}

/**
*@param: raid_room_id: 副本房间ID，team_id: 阵营号，现在只默认为0
*@return: void
*@usage: 脱离副本团队
**/
void CECGameSession::raid_quit( int raid_room_id, unsigned char team_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidQuit(pHost->GetCharacterID(), raid_room_id, team_id));
}

/**
*@param: raid_room_id: 副本房间ID，map_id: 副本地图号, requester_id: 申请人ID，is_accept: 是否接受: 0:不接受，1：接受
*@return: void
*@usage: 团长（房主）审批申请加入副本房间的人是否能加入该房间
**/
void CECGameSession::raid_accept( int raid_room_id, int map_id, int requester_id, unsigned char is_accept )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidJoinAccept(is_accept, pHost->GetCharacterID(), map_id, raid_room_id, requester_id, 0));
}

/**
*@param: raid_room_id: 副本房间ID，map_id: 副本地图号, target_id: 团长转移人的ID
*@return: void
*@usage: 团长指定某人为团长：团长转移
**/
void CECGameSession::raid_appoint( int raid_room_id, int target_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidAppoint(pHost->GetCharacterID(), raid_room_id, target_id,  0));
}

/**
*@param: raid_room_id: 副本房间ID, target_id: 被踢的人的ID, reason: 踢人的理由
*@return: void
*@usage: 团长踢掉某成员
**/
void CECGameSession::raid_kick( int raid_room_id, int target_id, int reason )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidKick(pHost->GetCharacterID(), target_id, raid_room_id, reason));
}

/**
*@param: raid_room_id: 副本房间ID
*@return: void
*@usage: 获得副本房间的相关信息(申请人列表，成员列表等)
**/
void CECGameSession::raid_getroom( int raid_room_id )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}

	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidGetRoom(pHost->GetCharacterID(), raid_room_id, 0));
}

void CECGameSession::raid_open_vote( int role_id, int raid_room_id )
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}
	
	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidOpenVote(pHost->GetCharacterID(), role_id, raid_room_id, 0));
}

void CECGameSession::raid_votes( int role_id, unsigned char vote )
{
	//获得HostPlayer对象指针
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}
	
	//发送协议
	m_pNetMan->Send(m_idLink, GNET::RaidVotes(pHost->GetCharacterID(), role_id, vote));
}

void CECGameSession::raid_quit_6v6()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}
	m_pNetMan->Send(m_idLink, GNET::TeamRaidQuit(pHost->GetCharacterID(), 0));
}

void CECGameSession::get_global_value(int key)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	if ( !pHost )
	{
		return;
	}
	m_pNetMan->Send(m_idLink, GNET::GetCommonData(key));
}

// 快捷支付相关协议命令
// cash: 折扣前金额，单位分
// cash_after_count: 折扣后金额，单位分
// merchant_id: 支付商家的id（支付宝为1）
// reserved: 保留字段
void CECGameSession::fast_pay(int cash, int cash_after_count, int merchant_id, int reserved)
{
	//发送协议
	m_pNetMan->Send(m_idLink, GNET::FastPay(GetUserID(), cash, cash_after_count, merchant_id));		// for test 201207
}

/**
*@param: protocol 协议本身
*@return: void
*@usage: 收到服务器关于副本相关的协议后的处理函数统一接口
**/
void CECGameSession::OnRaidProtocols(GNET::Protocol* pProtocol)
{
	//直接处理成消息形式转发到HostPlayer去处理
	g_pGame->GetGameRun()->PostMessage(MSG_HST_RAIDOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Account Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::account_GetSelfSellList()
{
	GNET::GetSellList p;
	p.roleid = m_iCharID;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevAccountPoint(GNET::PROTOCOL_GETSELLLIST, o.begin(), o.size());
}

void CECGameSession::account_GetShopSellList(int idStart, bool bReverse)
{
	GNET::FindSellPointInfo p;
	p.startid = idStart;
	p.forward = bReverse ? 0 : 1;
	SendNetData(p);
}

void CECGameSession::account_SellPoint(int iPoint, DWORD dwPrice)
{
	GNET::SellPoint p;
	p.roleid = m_iCharID;
	p.point = iPoint;
	p.price = (int)dwPrice;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevAccountPoint(GNET::PROTOCOL_SELLPOINT, o.begin(), o.size());
}

void CECGameSession::account_CancelSellPoint(int idSell)
{
	GNET::SellCancel p;
	p.roleid = m_iCharID;
	p.sellid = idSell;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevAccountPoint(GNET::PROTOCOL_SELLCANCEL, o.begin(), o.size());
}

void CECGameSession::account_BuyPoint(int idSell, int idSeller)
{
	GNET::BuyPoint p;
	p.roleid = m_iCharID;
	p.sellid = idSell;
	p.seller = idSeller;

	Octets o = p.marshal(Marshal::OctetsStream());
	c2s_CmdNPCSevAccountPoint(GNET::PROTOCOL_BUYPOINT, o.begin(), o.size());
}

void CECGameSession::OnAccountProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_ACCOUNTPOINT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

///////////////////////////////////////////////////////////////////////////
//	
//	Combat Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::combat_Challenge(int roleid, int faction)
{
	SendNetData(GNET::CombatChallenge(roleid, faction));
}

void CECGameSession::combat_Top(int roleid, int faction, int handle)
{
	SendNetData(GNET::CombatTop(0, roleid, faction, handle));
}

void CECGameSession::combat_invite_response(DWORD& dwHandle, bool bAgree)
{
	using namespace GNET;
	assert(dwHandle);
	if (dwHandle == 0) return;

	Rpc* p = reinterpret_cast<Rpc*>(dwHandle);
	CombatInviteRes* pResult = static_cast<CombatInviteRes*>(p->GetResult());
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_SUCCESS : 1;
		SendNetData(p);
	}

	p->Destroy();
	dwHandle = 0;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Sect Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::OnSectProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_SECT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

void CECGameSession::sect_list()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::SectList p;
	p.master = pHost->GetMaster() == 0 ? m_iCharID : pHost->GetMaster();
	p.roleid = m_iCharID;
	p.localsid = 0;

	SendNetData(p);
}

void CECGameSession::sect_expel(int roleid)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::SectExpel p;
	p.master = m_iCharID;
	p.disciple = roleid;
	p.localsid = 0;

	SendNetData(p);
}

void CECGameSession::sect_recruit(int roleid)
{
	GNET::SectRecruit p;
	p.roleid = m_iCharID;
	p.invitee = roleid;

	SendNetData(p);
	LuaEvent::FirstbeMaster();
}

bool CECGameSession::sect_invite_response(DWORD& dwHandle, bool bAgree)
{
	using namespace GNET;

	if (!dwHandle)
	{
		ASSERT(0);
		return false;
	}

	Rpc* p = (Rpc*)dwHandle;
	SectInviteRes* pResult = (SectInviteRes*)p->GetResult();
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_SUCCESS: 1;
		SendNetData(p);
	}

	p->Destroy();
	dwHandle = 0;

	return true;
}

void CECGameSession::get_kdctoken()
{
	using namespace GNET;

	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GetKDCToken p(pHost->GetCharacterID());
	SendNetData(p);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Contest Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::contest_invite_re(int accept_invite)
{
	GNET::ContestInvite_Re p;
	p.roleid = m_iCharID;
	p.accept_invite = accept_invite;
	SendNetData(p);
}

void CECGameSession::contest_answer(int questionid, int answer, char usestar)
{
	GNET::ContestAnswer p;
	p.roleid = m_iCharID;
	p.questionid = questionid;
	p.answer = answer;
	p.usestar = usestar;
	SendNetData(p);
}

void CECGameSession::contest_exit()
{
	GNET::ContestExit p;
	p.roleid = m_iCharID;
	SendNetData(p);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Other Protocols
//	
///////////////////////////////////////////////////////////////////////////

// void CECGameSession::player_change_gs(int line, int instance, int reason, float pos_x, float pos_z)
// {
// 	SendNetData(GNET::PlayerChangeGS(GetCharacterID(), 0, line, instance, reason, pos_x, 0, pos_z));
// }

//  player try change gs, in cross server, player can change gs directly without back to the half logout state
void CECGameSession::try_change_gs(int line)
{
	SendNetData(GNET::TryChangeGS(GetCharacterID(), line, 0));
}

void CECGameSession::do_change_gs(int line)
{
	// 清除该列表，防止在跨服中切换gs的时候发送其他的协议
	m_aPBIPlayers.RemoveAll(false);
	m_pPBIProc->Reset();	
	SendNetData(GNET::DoChangeGS(GetCharacterID(), line, 0));
}

void CECGameSession::matrix_response(int iResponse)
{
	GNET::MatrixResponse p;
	p.response = iResponse;
	SendNetData(p);
}

void CECGameSession::autolock_set(int timeout)
{
	GNET::AutolockSet p;
	p.roleid = m_iCharID;
	p.userid = 0;
	p.timeout = timeout;
	p.localsid = 0;

	SendNetData(p);
}

void CECGameSession::toptable_get(int iTable, int iPage, bool bNeedMyRank)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::GetTopTable(iTable, iPage, bNeedMyRank ? pHost->GetCharacterID() : 0, 0));
}

void CECGameSession::enemies_get(int iRole, int iFullList)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::GetEnemies(pHost->GetCharacterID(), iFullList, 0));
}

void CECGameSession::enemies_update(int iRole, int iEnemy, char oprt)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::UpdateEnemy(pHost->GetCharacterID(), iEnemy, oprt, 0));
}


///////////////////////////////////////////////////////////////////////////
//	
//	SNS Platform Protocols
//	
///////////////////////////////////////////////////////////////////////////

void CECGameSession::sns_accept_apply(int apply_roleid, char message_type, int message_id, char oper)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
 	m_pNetMan->Send(m_idLink, GNET::SNSAcceptApply(pHost->GetCharacterID(), apply_roleid, message_type, message_id, oper, 0));	
}

void CECGameSession::sns_cancel_message(int message_id, char message_type)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSCancelMessage(pHost->GetCharacterID(), message_id, message_type, 0));
}

void CECGameSession::sns_get_message(char message_type, int message_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSGetMessage(pHost->GetCharacterID(), message_type, message_id, 0));
}

void CECGameSession::sns_get_playerinfo(char check_private, int asker)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSGetPlayerInfo(pHost->GetCharacterID(), check_private, asker, 0));
}

void CECGameSession::sns_list_message(char message_type)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSListMessage(pHost->GetCharacterID(), message_type, 0));
}

void CECGameSession::sns_set_playerinfo(int is_private,  const ACHAR* szNickname, short age, int birthday, int province, int city, const ACHAR* szCareer, const ACHAR* szAddress, const ACHAR* szIntroduce)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSSetPlayerInfo(pHost->GetCharacterID(), is_private, Octets(szNickname, a_str_size(szNickname)), age, birthday,province, city, 
													Octets(szCareer, a_str_size(szCareer)), Octets(szAddress, a_str_size(szAddress)), Octets(szIntroduce, a_str_size(szIntroduce)) ));
}

void CECGameSession::sns_del_message(int message_id, char message_type, int leavemsg_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::SNSCancelLeaveMessage(pHost->GetCharacterID(), message_id, message_type, leavemsg_id));
}

///////////////////////////////////////////////////////////////////////////
//	
//	线上推广 Protocols
//	
///////////////////////////////////////////////////////////////////////////
void CECGameSession::ref_get_reference_code()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::RefGetReferenceCode(pHost->GetCharacterID()));
}

void CECGameSession::ref_get_referrals(int idx)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::RefListReferrals(pHost->GetCharacterID(), idx));
}

void CECGameSession::ref_withdraw_bonus()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::RefWithdrawBonus(pHost->GetCharacterID()));
}

void CECGameSession::ref_withdraw_exp()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::RefWithdrawExp(pHost->GetCharacterID()));
}

// 领土战
void CECGameSession::OnTerritoryProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_TERRITORYOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}
void CECGameSession::territory_get_map()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::TerritoryMapGet(pHost->GetCharacterID()));
}

void CECGameSession::unique_bid_get_history()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::UniqueBidHistory(pHost->GetCharacterID()));
}

// Circle Protocols

void CECGameSession::OnCircleProtocols(GNET::Protocol* pProtocol)
{
	g_pGame->GetGameRun()->PostMessage(MSG_HST_CIRCLE, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}

void CECGameSession::circle_get_baseinfo(int iNumCircle, const int* aCircleIDs)
{
	using namespace GNET;
	const int iNumLimit = 240;
	int iCount = 0;

	while (iCount < iNumCircle)
	{
		GetCircleBaseInfo p;
		p.roleid = m_iCharID;
		int iNumSend = iNumLimit;

		if (iCount + iNumLimit > iNumCircle)
			iNumSend = iNumCircle - iCount;

		for (int i = 0; i < iNumSend; i++)
			p.circleidlist.add(aCircleIDs[iCount+i]);

		SendNetData(p);
		iCount += iNumSend;
	}
}

void CECGameSession::circle_get_list(int idCircle)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::CircleList(pHost->GetCharacterID(), idCircle));
}

void CECGameSession::circle_get_award(int idCircle)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::ClaimCircleGradBonus(idCircle, pHost->GetCharacterID()));
}


void CECGameSession::circle_chat(const ACHAR* szMsg, char channel, int nPack, int nSlot, int nAchievement)
{
	using namespace CHAT_C2S;
	Octets data;

	if (nPack >= 0)
	{
		chat_equip_item c;
		c.cmd_id = CHAT_EQUIP_ITEM;
		c.where = nPack;
		c.index = nSlot;
		data.replace(&c, sizeof(c));
	}
	else if (nAchievement >= 0)
	{
		chat_achievement c;
		c.cmd_id = CHAT_ACHIEVEMENT;
		c.achieve_id = nAchievement;
		data.replace(&c, sizeof(c));
	}
	int circleid = g_pGame->GetGameRun()->GetHostPlayer()->GetCircleID();
	SendNetData(GNET::GCircleChat(m_iCharID, circleid, channel, 0, Octets(szMsg, a_str_size(szMsg)), data));
}

void CECGameSession::consign_query()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::ConsignQuery(pHost->GetCharacterID(), 0));
}

void CECGameSession::consign_cancel(INT64 sn)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::ConsignCancelPost(sn, pHost->GetCharacterID(), 0));	
}

void CECGameSession::get_line_info()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::LineList());	
}

void CECGameSession::get_friend_gt_status(int cid, int idFriend)
{
	SendNetData(GNET::GetFriendGTStatus(cid, idFriend));
}

///////////////////////////////////////////////////////////////////////////
//	
//	FriendCallback Protocols
//	
///////////////////////////////////////////////////////////////////////////
void CECGameSession::GetFriendCallBckInfo(){
	GNET::FriendCallbackInfo p;
	p.roleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::AskForFriendcallBckAward(){
	GNET::FriendCallbackAward p;
	p.roleid = m_iCharID;
	SendNetData(p);
}


void CECGameSession::friendCallBckMail_Send(int receiver, int iMailID){
	GNET::FriendCallbackMail p;
	p.receiver_roleid = receiver;
	p.roleid = m_iCharID;
	p.template_id = iMailID;
	SendNetData(p);
}

void CECGameSession::OnFriendCallBckPrtcls(GNET::Protocol* pProtocol){
	g_pGame->GetGameRun()->PostMessage(MSG_HST_FRIENDCALLBCKOPT, MAN_PLAYER, 0, (DWORD)pProtocol, pProtocol->GetType());
}


void CECGameSession::SendFriendCallbackSubscribe(unsigned char c){
	GNET::FriendCallbackSubscribe p;
	p.is_subscribe = c;
	p.roleid = m_iCharID;
	SendNetData(p);
}

void CECGameSession::factionpkraid_challenge(const ACHAR* szName,int roleid,int mapid)
{	
    SendNetData(GNET::FactionPkRaidChallenge(roleid, mapid, Octets(szName, a_str_size(szName))));
}
void CECGameSession::factionpkraid_invite(int roleid,int mapid,char result,unsigned int inviterid,unsigned int dstid)
{
	SendNetData(GNET::FactionPkRaidInvite_Re(roleid,mapid,result,inviterid,dstid));
}
void CECGameSession::factionpkraid_getscore(int roleid,int mapid,int roomid)
{
	SendNetData(GNET::FactionPkRaidGetScore(mapid,roomid,roleid));
}
void CECGameSession::factionpkraid_getlist(int roleid,int mapid)
{
	SendNetData(GNET::FactionPkRaidGetList(roleid,mapid));
}
void CECGameSession::factionpkraid_challengecancel(int roleid,int mapid,unsigned int dstid)
{
	SendNetData(GNET::FactionPkRaidChallengeCancel(roleid,mapid,dstid));
}
void CECGameSession::factionpkraid_withdrawbonus(int roleid)
{
	  SendNetData(GNET::FactionWithDrawBonus(roleid));
}
void CECGameSession::kingwar_getinfo(int roleid)
{
    SendNetData(GNET::KingdomGetInfo(roleid,0));
}
void CECGameSession::kingwar_annouce(int roleid,const ACHAR* szAnnouce)
{
    SendNetData(GNET::KingdomAnnounce(roleid,Octets(szAnnouce, a_str_size(szAnnouce)),0));
}
void CECGameSession::kingwar_appoint(int roleid,const ACHAR* cadidate,char title)
{
    SendNetData(GNET::KingdomAppoint(roleid,Octets(cadidate, a_str_size(cadidate)),title));
}
void CECGameSession::kingwar_discharge(int roleid,int cadidate,char title)
{
    SendNetData(GNET::KingdomDischarge(roleid,cadidate,title));
}
void CECGameSession::kingwar_gameattri(int roleid,int flag)
{
    SendNetData(GNET::KingdomSetGameAttri(roleid,flag,0));
}

void CECGameSession::flower_send(int targetroleid,const ACHAR* rolename,const ACHAR* szMsg,int num)
{
 	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
 	GNET::SendFlowerReq p(pHost->GetCharacterID(),targetroleid,Octets(rolename, a_str_size(rolename)),Octets(szMsg, a_str_size(szMsg)),num);
 
 	m_pNetMan->Send(m_idLink, p);
}
void CECGameSession::top_flower(char isRecv,int page)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::TopFlowerReq(pHost->GetCharacterID(),isRecv,page - 1));
}

void CECGameSession::getgiftFlower(char isRece)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	m_pNetMan->Send(m_idLink, GNET::TopFlowerGetGift(isRece,pHost->GetCharacterID()));
}
void CECGameSession::kingwar_gettitle(int roleid)
{
    SendNetData(GNET::KingdomGetTitle(roleid,0));
}
void CECGameSession::kingwar_guardinvite_res(DWORD& dwHandle,bool bAgree)
{
	using namespace GNET;
	assert(dwHandle);
	if (dwHandle == 0) return;
	
	Rpc* p = reinterpret_cast<Rpc*>(dwHandle);
	KingGuardInviteRes* pResult = static_cast<KingGuardInviteRes*>(p->GetResult());
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_SUCCESS : 1;
		SendNetData(p);
	}
	
	p->Destroy();
	dwHandle = 0;
}


void CECGameSession::c2s_CmdSendFlowBattleEnter(int gs_id,int map_id,int iscross)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::OpenBanquetEnter p;
	p.gs_id = gs_id;
	p.map_id = map_id;
	p.roleid = pHost->GetCharacterID();
	p.iscross = iscross;

	SendNetData(p);
}

void CECGameSession::c2s_CmdSendFlowApplyCancel(int gs_id,int map_id,int isqueue)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::OpenBanquetCancel p;
	p.gs_id = gs_id;
	p.map_id = map_id;
	p.roleid = pHost->GetCharacterID();
	p.isqueue = isqueue;
	SendNetData(p);	
}


void CECGameSession::c2s_CmdSendFlowQueueCount()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::OpenBanquetQueueCount p;
	p.roleid = pHost->GetCharacterID();
	SendNetData(p);		
}

void CECGameSession::c2s_CmdSendFlowGetField(int gs_id)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::OpenBanquetGetField p;
	p.gs_id = gs_id;
	p.roleid = pHost->GetCharacterID();
	SendNetData(p);		
}

void CECGameSession::c2s_CmdSendGetFactiondyNamic(int nPageId)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::GetFactionDynamic p;
	p.pageid = nPageId;
	p.roleid = pHost->GetCharacterID();
	SendNetData(p);		
}

void CECGameSession::c2s_CmdSendGetFactionMsg(int nPageId)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::GetFactionMsg p;
	p.pageid = nPageId;
	p.roleid = pHost->GetCharacterID();
	SendNetData(p);	
}

void  CECGameSession::c2s_CmdSendFactionPostMsg(char repost, const ACHAR* szMsg)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	
	SendNetData(GNET::FactionPostMsg(pHost->GetCharacterID(), 0, Octets(szMsg, a_str_size(szMsg)), repost));
}

void CECGameSession::c2s_CmdSendCreateFacBase()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	SendNetData(GNET::CreateFacBase(pHost->GetCharacterID()));
}
void CECGameSession::c2s_CmdSendGetFacActTopTable(int nPage)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	SendNetData(GNET::GetFacActTopTable(nPage,pHost->GetCharacterID(),0));
}

void CECGameSession::SendFacBaseDataSend(void* pData, int iSize)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	GNET::FacBaseDataSend p;
	p.roleid = pHost->GetCharacterID();
	p.data.replace(pData, iSize);
	SendNetData(p);
}

void CECGameSession::touch_getpoint(int roleid)
{
    SendNetData(GNET::GetTouchPoint(roleid));
}
void CECGameSession::touch_getpassportcashadd(int roleid)
{
	SendNetData(GNET::GetPassportCashAdd(roleid));
}
void CECGameSession::kingwar_openbath(int roleid)
{
	SendNetData(GNET::QueenTryOpenBath(roleid,0));
}
void CECGameSession::kingwar_openbathinvite_res(DWORD& dwHandle,bool bAgree)
{
    using namespace GNET;
	assert(dwHandle);
	if (dwHandle == 0) return;
	
	Rpc* p = reinterpret_cast<Rpc*>(dwHandle);
	QueenOpenBathInviteRes* pResult = static_cast<QueenOpenBathInviteRes*>(p->GetResult());
	if (pResult)
	{
		pResult->retcode = bAgree ? ERR_SUCCESS : 1;
		SendNetData(p);
	}
	
	p->Destroy();
	dwHandle = 0;
}
void CECGameSession::kingwar_getkingtask(int roleid)
{
	SendNetData(GNET::GetKingTask(roleid));
}
void CECGameSession::defencetimeoutnotify(int roleid)
{
    SendNetData(GNET::ACClientTimeout(roleid));
}



void CECGameSession::c2s_CmdSendCrssvrTeamsRename(const ACHAR* szNew)		// 变更名字 PROTOCOL_CRSSVRTEAMSRENAME
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsRename(pHost->GetCharacterID(),pHost->GetCrssvrTeamsId(),Octets(szNew, a_str_size(szNew))));
}

void CECGameSession::c2s_CmdSendCrssvrTeamsRecruit(const ACHAR* szName, int invitee)		// 邀请加入战队 #3被邀请人ID PROTOCOL_CRSSVRTEAMSRECRUIT 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsRecruit(pHost->GetCharacterID(),Octets(szName, a_str_size(szName)),pHost->GetCrssvrTeamsId(),invitee));
}

void CECGameSession::c2s_CmdSendCrssvrTeamsLeave()		// 离开战队 ID PROTOCOL_CRSSVRTEAMSLEAVE 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsLeave(pHost->GetCrssvrTeamsId(),pHost->GetCharacterID()));
}
void CECGameSession::c2s_CmdSendCrssvrTeamsGetRoleList()		// 获取战队列表 ID PROTOCOL_CRSSVRTEAMSGETLIST 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsGetRoleList(pHost->GetCharacterID(),pHost->GetCrssvrTeamsId()));
}

void CECGameSession::c2s_CmdSendCrssvrTeamsBase()		// 获取战队基础 ID PROTOCOL_CRSSVRTEAMSGETTEAMBASE 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsGetTeamBase(pHost->GetCharacterID(),pHost->GetCrssvrTeamsId()));
}

void CECGameSession::c2s_CmdSendCrssvrTeamsDismiss()		// 解散战队 ID PROTOCOL_CRSSVRTEAMSDISMISS 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsDismiss(pHost->GetCharacterID(),pHost->GetCrssvrTeamsId()));
}

void CECGameSession::c2s_CmdSendCrssvrTeamsChgCaptain(int master_new)		// 变更队长 ID PROTOCOL_CRSSVRTEAMSCHGCAPTAIN 
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsChgCaptain(pHost->GetCharacterID(),master_new,pHost->GetCrssvrTeamsId()));
}

void   CECGameSession::c2s_CmdSendCrssvrTeamsInvite(DWORD& dwHandle, bool bAgree)
{
	using namespace GNET;
	assert(dwHandle);

	if (dwHandle == 0)
		return;

	Rpc* p = reinterpret_cast<Rpc*>(dwHandle);
	CrssvrTeamsInviteRes* pResult = static_cast<CrssvrTeamsInviteRes*>(p->GetResult());

	if (pResult)
	{
		pResult->retcode = !bAgree;
		SendNetData(p);
	}

	p->Destroy();
	dwHandle = 0;
}

void CECGameSession::c2s_CmdSendCrssvrTeamsKick(int roleid)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsKick(pHost->GetCrssvrTeamsId(),pHost->GetCharacterID(),roleid));
}

void CECGameSession::c2s_CmdSendCrssvrSearch(const ACHAR* szName)
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsSearch(pHost->GetCharacterID(),Octets(szName, a_str_size(szName))));
}

void CECGameSession::c2s_CmdSendCrssvrGetRoleNotify()
{
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();
	SendNetData(GNET::CrssvrTeamsGetRoleNotify(pHost->GetCharacterID()));
}