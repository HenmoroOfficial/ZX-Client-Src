#include <winsock2.h>
#include "crssvrteamsinvite.hrp"
#include "addfriendrqst.hrp"
#include "tradestartrqst.hrp"
#include "response.hpp"
#include "selectrole.hpp"
#include "enterworld.hpp"
#include "rolelist.hpp"
#include "createrole.hpp"
#include "deleterole.hpp"
#include "undodeleterole.hpp"
#include "playerbaseinfo.hpp"
#include "playerbaseinfocrc.hpp"
#include "getplayeridbyname.hpp"
#include "setcustomdata.hpp"
#include "getcustomdata.hpp"
#include "setuiconfig.hpp"
#include "getuiconfig.hpp"
#include "sethelpstates.hpp"
#include "gethelpstates.hpp"
#include "getplayerbriefinfo.hpp"
#include "publicchat.hpp"
#include "getfriends.hpp"
#include "factionchat.hpp"
#include "factioncreate.hpp"
#include "factionexpel.hpp"
#include "factionappoint.hpp"
#include "factionresign.hpp"
#include "factionleave.hpp"
#include "factiondismiss.hpp"
#include "factionupgrade.hpp"
#include "factionlist.hpp"
#include "factionannounce.hpp"
#include "factioncreate_re.hpp"
#include "factionexpel_re.hpp"
#include "factionappoint_re.hpp"
#include "factionresign_re.hpp"
#include "factionleave_re.hpp"
#include "factiondismiss_re.hpp"
#include "factionupgrade_re.hpp"
#include "factionlist_re.hpp"
#include "factionrecruit_re.hpp"
#include "factionnickname_re.hpp"
#include "factionannounce_re.hpp"
#include "getfactionbaseinfo.hpp"
#include "getfactionbaseinfo_re.hpp"
#include "tradeaddgoods.hpp"
#include "traderemovegoods.hpp"
#include "tradesubmit.hpp"
#include "trademoveobj.hpp"
#include "tradeconfirm.hpp"
#include "tradediscard.hpp"
#include "gmonlinenum.hpp"
#include "gmlistonlineuser.hpp"
#include "gmkickoutuser.hpp"
#include "gmkickoutrole.hpp"
#include "gmshutup.hpp"
#include "gmshutuprole.hpp"
#include "gmtogglechat.hpp"
#include "gmforbidrole.hpp"
#include "report2gm.hpp"
#include "complain2gm.hpp"
#include "acreport.hpp"
#include "checknewmail.hpp"
#include "announcenewmail.hpp"
#include "getmaillist.hpp"
#include "getmail.hpp"
#include "getmailattachobj.hpp"
#include "deletemail.hpp"
#include "preservemail.hpp"
#include "playersendmail.hpp"
#include "errorinfo.hpp"
#include "challenge.hpp"
#include "keyexchange.hpp"
#include "announceforbidinfo.hpp"
#include "onlineannounce.hpp"
#include "keepalive.hpp"
#include "rolelist_re.hpp"
#include "createrole_re.hpp"
#include "deleterole_re.hpp"
#include "undodeleterole_re.hpp"
#include "selectrole_re.hpp"
#include "setcustomdata_re.hpp"
#include "getcustomdata_re.hpp"
#include "gamedatasend.hpp"
#include "playerbaseinfo_re.hpp"
#include "playerbaseinfocrc_re.hpp"
#include "getplayeridbyname_re.hpp"
#include "setuiconfig_re.hpp"
#include "getuiconfig_re.hpp"
#include "sethelpstates_re.hpp"
#include "gethelpstates_re.hpp"
#include "getplayerbriefinfo_re.hpp"
#include "statusannounce.hpp"
#include "playerlogout.hpp"
#include "chatmessage.hpp"
#include "worldchat.hpp"
#include "privatechat.hpp"
#include "rolestatusannounce.hpp"
#include "addfriend.hpp"
#include "addfriend_re.hpp"
#include "getfriends_re.hpp"
#include "setgroupname.hpp"
#include "setgroupname_re.hpp"
#include "setfriendgroup.hpp"
#include "setfriendgroup_re.hpp"
#include "delfriend.hpp"
#include "delfriend_re.hpp"
#include "friendstatus.hpp"
#include "getsavedmsg.hpp"
#include "getsavedmsg_re.hpp"
#include "chatroomcreate_re.hpp"
#include "chatroominvite.hpp"
#include "chatroominvite_re.hpp"
#include "chatroomjoin_re.hpp"
#include "chatroomleave.hpp"
#include "chatroomexpel.hpp"
#include "chatroomspeak.hpp"
#include "chatroomlist_re.hpp"
#include "tradestart.hpp"
#include "tradestart_re.hpp"
#include "tradeaddgoods_re.hpp"
#include "traderemovegoods_re.hpp"
#include "tradesubmit_re.hpp"
#include "trademoveobj_re.hpp"
#include "tradeconfirm_re.hpp"
#include "tradediscard_re.hpp"
#include "tradeend.hpp"
#include "gmrestartserver.hpp"
#include "gmonlinenum_re.hpp"
#include "gmlistonlineuser_re.hpp"
#include "gmkickoutuser_re.hpp"
#include "gmkickoutrole_re.hpp"
#include "gmshutup_re.hpp"
#include "gmshutuprole_re.hpp"
#include "gmtogglechat_re.hpp"
#include "gmrestartserver_re.hpp"
#include "gmforbidrole_re.hpp"
#include "report2gm_re.hpp"
#include "complain2gm_re.hpp"
#include "queryuserprivilege_re.hpp"
#include "acremotecode.hpp"
#include "getmaillist_re.hpp"
#include "getmail_re.hpp"
#include "getmailattachobj_re.hpp"
#include "deletemail_re.hpp"
#include "preservemail_re.hpp"
#include "playersendmail_re.hpp"

#include "auctionopen.hpp"
#include "auctionbid.hpp"
#include "auctionlist.hpp"
#include "auctionclose.hpp"
#include "auctionget.hpp"
#include "auctionopen_re.hpp"
#include "auctionbid_re.hpp"
#include "auctionclose_re.hpp"
#include "auctionlist_re.hpp"
#include "auctionget_re.hpp"
#include "auctionattendlist.hpp"
#include "auctionattendlist_re.hpp"
#include "auctionexitbid.hpp"
#include "auctionexitbid_re.hpp"
#include "updateremaintime.hpp"
#include "auctiongetitem.hpp"
#include "auctiongetitem_re.hpp"

#include "sellpoint.hpp"
#include "buypoint.hpp"
#include "getselllist.hpp"
#include "findsellpointinfo.hpp"
#include "sellcancel.hpp"
#include "sellpoint_re.hpp"
#include "getselllist_re.hpp"
#include "findsellpointinfo_re.hpp"
#include "announcesellresult.hpp"
#include "sellcancel_re.hpp"
#include "buypoint_re.hpp"
#include "linelist.hpp"

#include "gettoptable.hpp"
#include "gettoptable_re.hpp"
#include "factioninvite.hrp"
#include "getenemies.hpp"
#include "updateenemy.hpp"
#include "getenemies_re.hpp"
#include "updateenemy_re.hpp"
#include "stockcommission.hpp"
#include "stockbill.hpp"
#include "stockaccount.hpp"
#include "stockcommission_re.hpp"
#include "stockaccount_re.hpp"
#include "stocktransaction_re.hpp"
#include "stockbill_re.hpp"
#include "stockcancel.hpp"
#include "stockcancel_re.hpp"

#include "combatinvite.hrp"
#include "combatchallenge.hpp"
#include "combattop.hpp"
#include "combatchallenge_re.hpp"
#include "combatcontrol.hpp"
#include "combatstatus.hpp"
#include "combattop_re.hpp"
#include "acanswer.hpp"
#include "acquestion.hpp"

#include "matrixchallenge.hpp"
#include "matrixresponse.hpp"
#include "familyrecord.hpp"

#include "cashlock_re.hpp"
#include "cashpasswordset_re.hpp"
#include "cashlock.hpp"
#include "cashpasswordset.hpp"
#include "hostiledelete.hpp"
#include "hostileadd.hpp"
#include "hostileadd_re.hpp"
#include "hostiledelete_re.hpp"
#include "hostileprotect_re.hpp"

#include "autolockset.hpp"
#include "autolockset_re.hpp"
#include "instancinggetlist.hpp"
#include "instancinggetfield.hpp"
#include "instancingkick.hpp"
#include "instancingstart.hpp"
#include "instancingavailablelist.hpp"
#include "instancingaccept.hpp"
#include "instancingleave.hpp"
#include "instancingappoint.hpp"
#include "instancingenter.hpp"
#include "instancinggetlist_re.hpp"
#include "instancinggetfield_re.hpp"
#include "instancingkick_re.hpp"
#include "instancingstart_re.hpp"
#include "instancingavailablelist_re.hpp"
#include "instancingjoinreq.hpp"
#include "instancingjoin_re.hpp"
#include "instancingleave_re.hpp"
#include "instancingappoint_re.hpp"
#include "instancingappointnotify.hpp"
#include "instancingenter_re.hpp"
#include "instancingstartnotify.hpp"
#include "instancingclosenotify.hpp"
#include "instancingkicknotify.hpp"
#include "instancingaccept_re.hpp"

#include "snssetplayerinfo.hpp"
#include "snsgetplayerinfo.hpp"
#include "snscancelmessage.hpp"
#include "snslistmessage.hpp"
#include "snsgetmessage.hpp"
#include "snsacceptapply.hpp"

#include "snssetplayerinfo_re.hpp"
#include "snspressmessage_re.hpp"
#include "snscancelmessage_re.hpp"
#include "snsapply_re.hpp"
#include "snsapplynotify.hpp"
#include "snslistmessage_re.hpp"
#include "snsgetmessage_re.hpp"
#include "snsgetplayerinfo_re.hpp"
#include "snsvote_re.hpp"
#include "snsacceptapply_re.hpp"
#include "snscancelleavemessage.hpp"
#include "snscancelleavemessage_re.hpp"

#include "reflistreferrals.hpp"
#include "refwithdrawexp.hpp"
#include "refwithdrawbonus.hpp"
#include "refgetreferencecode.hpp"
#include "reflistreferrals_re.hpp"
#include "refwithdrawexp_re.hpp"
#include "refwithdrawbonus_re.hpp"
#include "refgetreferencecode_re.hpp"

#include "getserverrtt.hpp"
#include "announcedistrictid.hpp"
#include "fungamedatasend.hpp"
#include "hometowndatasend.hpp"
#include "hometownexchgmoney_re.hpp"
#include "achievementmessage.hpp"
#include "gshopnotifyscheme.hpp"
#include "pkmessage.hpp"
#include "refinemessage.hpp"

#include "territorymapget.hpp"
#include "territorymapget_re.hpp"
#include "territorychallenge_re.hpp"
#include "territoryenter_re.hpp"
#include "territoryleave_re.hpp"
#include "territoryenterremind.hpp"
#include "territoryscoreupdate.hpp"
#include "uniquebidhistory.hpp"
#include "uniquebidhistory_re.hpp"
#include "uniquebid_re.hpp"
#include "uniquegetitem_re.hpp"
#include "playerbaseinfo2.hpp"
#include "playerbaseinfo2_re.hpp"

#include "circlelist.hpp"
#include "circlelist_re.hpp"
#include "claimcirclegradbonus.hpp"
#include "gcirclechat.hpp"
#include "getcirclebaseinfo.hpp"
#include "getcirclebaseinfo_re.hpp"
#include "circlenotify.hpp"

#include "keyreestablish.hpp"
#include "playerchangeds_re.hpp"
#include "changeds_re.hpp"
#include "trychangegs.hpp"
#include "trychangegs_re.hpp"
#include "dochangegs.hpp"
#include "dochangegs_re.hpp"

#include "consigncancelpost_re.hpp"
#include "consigncancelpost.hpp"
#include "consignquery_re.hpp"
#include "consignquery.hpp"
#include "consignstarterr.hpp"

#include "consigngetitem.hpp"
#include "consigngetitem_re.hpp"
#include "consignlistall.hpp"
#include "consignlistall_re.hpp"
#include "consignlistlargecategory.hpp"
#include "consignlistlargecategory_re.hpp"

#include "gateofflinechat.hpp"
#include "getfriendgtstatus.hpp"
#include "updatefriendgtstatus.hpp"
#include "gategetgtrolestatus_re.hpp"


#include "friendcallbackinfo_re.hpp"
#include "friendcallbackmail_re.hpp"
#include "friendcallbackaward_re.hpp"
#include "friendcallbacksubscribe_re.hpp"

#include "raidstart.hpp"
#include "raidenter.hpp"
#include "raidquit.hpp"
#include "raidgetlist.hpp"
#include "raidappoint.hpp"
#include "raidkick.hpp"
#include "raidgetroom.hpp"
#include "raidjoinaccept.hpp"
#include "raidjoinreq.hpp"
#include "raidopen_re.hpp"
#include "raidjoin_re.hpp"
#include "raidjoinapplylist.hpp"
#include "raidjoinaccept_re.hpp"
#include "raidenter_re.hpp"
#include "raidgetlist_re.hpp"
#include "raidstart_re.hpp"
#include "raidappoint_re.hpp"
#include "raidappointnotify.hpp"
#include "raidstartnotify.hpp"
#include "raidkick_re.hpp"
#include "raidkicknotify.hpp"
#include "raidquit_re.hpp"
#include "raidgetroom_re.hpp"
#include "getkdctoken.hpp"
#include "getkdctoken_re.hpp"
#include "consignlistrole.hpp"
#include "consignlistrole_re.hpp"
#include "consignplayer_re.hpp"
#include "ssogetticketreq.hpp"
#include "ssogetticketrep.hpp"
#include "fastpay.hpp"
#include "fastpay_re.hpp"
#include "discountannounce.hpp"
#include "raidopenvote.hpp"
#include "raidvotes.hpp"
#include "raidopenvote_re.hpp"
#include "raidbeginvotes.hpp"
#include "raidvoteresult.hpp"
#include "raidbriefinfo.hpp"
#include "globaldropbroadcast.hpp"
#include "globaldropremainbroadcast.hpp"
#include "fastpaybindinfo.hpp"
#include "queryrolenamehistory_re.hpp"
#include "changerolename_re.hpp"
#include "queryrolenamehistory.hpp"
#include "friendnamechange.hpp"
#include "teamraidactiveapplynotice.hpp"
#include "teamraidmappingsuccess_re.hpp"
#include "teamraidupdatechar.hpp"
#include "teamraidapply_re.hpp"
#include "teamraidmappingsuccess.hpp"
#include "teamraidbeginchoosechar.hpp"
#include "teamraidacktimeout.hpp"
#include "changefactionname_re.hpp"
#include "factionpkraidchallenge.hpp"
#include "factionpkraidinvite_re.hpp"
#include "factionpkraidgetscore.hpp"
#include "factionpkraidgetlist.hpp"
#include "factionpkraidchallenge_re.hpp"
#include "factionpkraidinvite.hpp"
#include "factionpkraidbegin.hpp"
#include "factionpkraidgetscore_re.hpp"
#include "factionpkraidgetlist_re.hpp"
#include "factionpkraidstopfight.hpp"
#include "factionpkraidchallengecancel.hpp"
#include "factionpkraidchallengecancel_re.hpp"
#include "factionwithdrawbonus.hpp"
#include "factionwithdrawbonus_re.hpp"
#include "kingdombattleenter_re.hpp"
#include "kingdombattleleave_re.hpp"
#include "kingdomannounce.hpp"
#include "kingdomappoint.hpp"
#include "kingdomdischarge.hpp"
#include "kingdomsetgameattri.hpp"
#include "kingdomgetinfo.hpp"
#include "kingdomannounce_re.hpp"
#include "kingdomappoint_re.hpp"
#include "kingdomdischarge_re.hpp"
#include "kingdomsetgameattri_re.hpp"
#include "kingdomgetinfo_re.hpp"
#include "kingdomgettitle.hpp"
#include "kingdomgettitle_re.hpp"
#include "kingdomtitlechange.hpp"
#include "kingdompointchange.hpp"
#include "kingdomkingchange.hpp"
#include "kingguardinvite.hrp"
#include "kingcallguards_re.hpp"
#include "queenopenbathinvite.hrp"
#include "queentryopenbath.hpp"
#include "queenopenbath_re.hpp"
#include "kingissuetask_re.hpp"
#include "getkingtask_re.hpp"
#include "getkingtask.hpp"
#include "getcommondata.hpp"
#include "getcommondata_re.hpp"
#include "teamraidquit.hpp"
#include "teamraidapplynotice.hpp"
#include "sendflowerreq.hpp"
#include "topflowerreq.hpp"
#include "topflower_re.hpp"
#include "topflower_err.hpp"
#include "topflowermsg_re.hpp"
#include "acaccuse.hpp"
#include "gettouchpoint.hpp"
#include "getpassportcashadd.hpp"
#include "gettouchpoint_re.hpp"
#include "getpassportcashadd_re.hpp"
#include "touchpointexchange_re.hpp"
#include "openbanquetjoin_re.hpp"
#include "openbanquetenter.hpp"
#include "openbanquetenter_re.hpp"
#include "openbanquetcancel.hpp"
#include "openbanquetdrag_re.hpp"
#include "openbanquetqueuecount.hpp"
#include "openbanquetqueuecount_re.hpp"
#include "openbanquetgetfield.hpp"
#include "openbanquetgetfield_re.hpp"
#include "openbanquetnotify_re.hpp"
#include "acaccuse_re.hpp"
#include "acclienttimeout.hpp"
#include "getfactiondynamic.hpp"
#include "getfactionmsg.hpp"
#include "getfactiondynamic_re.hpp"
#include "getfactionmsg_re.hpp"
#include "factionpostmsg.hpp"
#include "factionpostmsg_re.hpp"
#include "kinggetreward_re.hpp"

#include "crssvrteamsgetrolelist.hpp"
#include "crssvrteamsgetteambase.hpp"
#include "crssvrteamsgetrolenotify.hpp"
#include "crssvrteamsrename.hpp"
#include "crssvrteamsdismiss.hpp"
#include "crssvrteamsrecruit.hpp"
#include "crssvrteamschgcaptain.hpp"
#include "crssvrteamsleave.hpp"
#include "crssvrteamsnotify_re.hpp"
#include "crssvrteamsteambase_re.hpp"
#include "crssvrteamsrolelist_re.hpp"
#include "crssvrteamsrolenotify_re.hpp"
#include "crssvrteamskick.hpp"
#include "crssvrteamssearch.hpp"
#include "crssvrteamssearch_re.hpp"
#include "createfacbase_re.hpp"
#include "createfacbase.hpp"
#include "facbaseenter_re.hpp"
#include "getfacacttoptable.hpp"
#include "getfacacttoptable_re.hpp"
#include "facbasedatasend.hpp"
#include "facbasestopnotice.hpp"

namespace GNET
{
static CrssvrTeamsInvite __stub_CrssvrTeamsInvite (RPC_CRSSVRTEAMSINVITE, new CrssvrTeamsInviteArg, new CrssvrTeamsInviteRes);
static FactionInvite __stub_FactionInvite (RPC_FACTIONINVITE, new FactionInviteArg, new FactionInviteRes);
static AddFriendRqst __stub_AddFriendRqst (RPC_ADDFRIENDRQST, new AddFriendRqstArg, new AddFriendRqstRes);
static TradeStartRqst __stub_TradeStartRqst (RPC_TRADESTARTRQST, new TradeStartRqstArg, new TradeStartRqstRes);
static CombatInvite __stub_CombatInvite (RPC_COMBATINVITE, new CombatInviteArg, new CombatInviteRes);
static KingGuardInvite __stub_KingGuardInvite (RPC_KINGGUARDINVITE, new KingGuardInviteArg, new KingGuardInviteRes);
static QueenOpenBathInvite __stub_QueenOpenBathInvite (RPC_QUEENOPENBATHINVITE, new QueenOpenBathInviteArg, new QueenOpenBathInviteRes);
static Response __stub_Response((void*)0);
static SelectRole __stub_SelectRole((void*)0);
static EnterWorld __stub_EnterWorld((void*)0);
static RoleList __stub_RoleList((void*)0);
static CreateRole __stub_CreateRole((void*)0);
static DeleteRole __stub_DeleteRole((void*)0);
static UndoDeleteRole __stub_UndoDeleteRole((void*)0);
static PlayerBaseInfo __stub_PlayerBaseInfo((void*)0);
static PlayerBaseInfoCRC __stub_PlayerBaseInfoCRC((void*)0);
static GetPlayerIDByName __stub_GetPlayerIDByName((void*)0);
static SetCustomData __stub_SetCustomData((void*)0);
static GetCustomData __stub_GetCustomData((void*)0);
static SetUIConfig __stub_SetUIConfig((void*)0);
static GetUIConfig __stub_GetUIConfig((void*)0);
static SetHelpStates __stub_SetHelpStates((void*)0);
static GetHelpStates __stub_GetHelpStates((void*)0);
static GetPlayerBriefInfo __stub_GetPlayerBriefInfo((void*)0);
static PublicChat __stub_PublicChat((void*)0);
static GetFriends __stub_GetFriends((void*)0);
static GetFactionBaseInfo __stub_GetFactionBaseInfo((void*)0);
static GetFactionBaseInfo_Re __stub_GetFactionBaseInfo_Re((void*)0);
static TradeAddGoods __stub_TradeAddGoods((void*)0);
static TradeRemoveGoods __stub_TradeRemoveGoods((void*)0);
static TradeSubmit __stub_TradeSubmit((void*)0);
static TradeMoveObj __stub_TradeMoveObj((void*)0);
static TradeConfirm __stub_TradeConfirm((void*)0);
static TradeDiscard __stub_TradeDiscard((void*)0);
static GMOnlineNum __stub_GMOnlineNum((void*)0);
static GMListOnlineUser __stub_GMListOnlineUser((void*)0);
static GMKickoutUser __stub_GMKickoutUser((void*)0);
static GMKickoutRole __stub_GMKickoutRole((void*)0);
static GMShutup __stub_GMShutup((void*)0);
static GMShutupRole __stub_GMShutupRole((void*)0);
static GMToggleChat __stub_GMToggleChat((void*)0);
static GMForbidRole __stub_GMForbidRole((void*)0);
static Report2GM __stub_Report2GM((void*)0);
static Complain2GM __stub_Complain2GM((void*)0);
static ACReport __stub_ACReport((void*)0);
static CheckNewMail __stub_CheckNewMail((void*)0);
static AnnounceNewMail __stub_AnnounceNewMail((void*)0);
static GetMailList __stub_GetMailList((void*)0);
static GetMail __stub_GetMail((void*)0);
static GetMailAttachObj __stub_GetMailAttachObj((void*)0);
static DeleteMail __stub_DeleteMail((void*)0);
static PreserveMail __stub_PreserveMail((void*)0);
static PlayerSendMail __stub_PlayerSendMail((void*)0);
static ErrorInfo __stub_ErrorInfo((void*)0);
static Challenge __stub_Challenge((void*)0);
static KeyExchange __stub_KeyExchange((void*)0);
static AnnounceForbidInfo __stub_AnnounceForbidInfo((void*)0);
static OnlineAnnounce __stub_OnlineAnnounce((void*)0);
static KeepAlive __stub_KeepAlive((void*)0);
static RoleList_Re __stub_RoleList_Re((void*)0);
static CreateRole_Re __stub_CreateRole_Re((void*)0);
static DeleteRole_Re __stub_DeleteRole_Re((void*)0);
static UndoDeleteRole_Re __stub_UndoDeleteRole_Re((void*)0);
static SelectRole_Re __stub_SelectRole_Re((void*)0);
static SetCustomData_Re __stub_SetCustomData_Re((void*)0);
static GetCustomData_Re __stub_GetCustomData_Re((void*)0);
static GamedataSend __stub_GamedataSend((void*)0);
static PlayerBaseInfo_Re __stub_PlayerBaseInfo_Re((void*)0);
static PlayerBaseInfoCRC_Re __stub_PlayerBaseInfoCRC_Re((void*)0);
static GetPlayerIDByName_Re __stub_GetPlayerIDByName_Re((void*)0);
static SetUIConfig_Re __stub_SetUIConfig_Re((void*)0);
static GetUIConfig_Re __stub_GetUIConfig_Re((void*)0);
static SetHelpStates_Re __stub_SetHelpStates_Re((void*)0);
static GetHelpStates_Re __stub_GetHelpStates_Re((void*)0);
static GetPlayerBriefInfo_Re __stub_GetPlayerBriefInfo_Re((void*)0);
static StatusAnnounce __stub_StatusAnnounce((void*)0);
static PlayerLogout __stub_PlayerLogout((void*)0);
static ChatMessage __stub_ChatMessage((void*)0);
static WorldChat __stub_WorldChat((void*)0);
static PrivateChat __stub_PrivateChat((void*)0);
static RoleStatusAnnounce __stub_RoleStatusAnnounce((void*)0);
static FactionChat __stub_FactionChat((void*)0);
static FactionCreate_Re __stub_FactionCreate_Re((void*)0);
static FactionList_Re __stub_FactionList_Re((void*)0);
static FactionRecruit_Re __stub_FactionRecruit_Re((void*)0);
static FactionNickname_Re __stub_FactionNickname_Re((void*)0);
static FactionExpel_Re __stub_FactionExpel_Re((void*)0);
static FactionAppoint_Re __stub_FactionAppoint_Re((void*)0);
static FactionResign_Re __stub_FactionResign_Re((void*)0);
static FactionLeave_Re __stub_FactionLeave_Re((void*)0);
static FactionDismiss_Re __stub_FactionDismiss_Re((void*)0);
static FactionUpgrade_Re __stub_FactionUpgrade_Re((void*)0);
static FactionAnnounce_Re __stub_FactionAnnounce_Re((void*)0);
static FactionCreate __stub_FactionCreate((void*)0);
static FactionList __stub_FactionList((void*)0);
static FactionExpel __stub_FactionExpel((void*)0);
static FactionAppoint __stub_FactionAppoint((void*)0);
static FactionResign __stub_FactionResign((void*)0);
static FactionLeave __stub_FactionLeave((void*)0);
static FactionDismiss __stub_FactionDismiss((void*)0);
static FactionUpgrade __stub_FactionUpgrade((void*)0);
static FactionAnnounce __stub_FactionAnnounce((void*)0);
static AddFriend __stub_AddFriend((void*)0);
static AddFriend_Re __stub_AddFriend_Re((void*)0);
static GetFriends_Re __stub_GetFriends_Re((void*)0);
static SetGroupName __stub_SetGroupName((void*)0);
static SetGroupName_Re __stub_SetGroupName_Re((void*)0);
static SetFriendGroup __stub_SetFriendGroup((void*)0);
static SetFriendGroup_Re __stub_SetFriendGroup_Re((void*)0);
static DelFriend __stub_DelFriend((void*)0);
static DelFriend_Re __stub_DelFriend_Re((void*)0);
static FriendStatus __stub_FriendStatus((void*)0);
static GetSavedMsg __stub_GetSavedMsg((void*)0);
static GetSavedMsg_Re __stub_GetSavedMsg_Re((void*)0);
static ChatRoomCreate_Re __stub_ChatRoomCreate_Re((void*)0);
static ChatRoomInvite __stub_ChatRoomInvite((void*)0);
static ChatRoomInvite_Re __stub_ChatRoomInvite_Re((void*)0);
static ChatRoomJoin_Re __stub_ChatRoomJoin_Re((void*)0);
static ChatRoomLeave __stub_ChatRoomLeave((void*)0);
static ChatRoomExpel __stub_ChatRoomExpel((void*)0);
static ChatRoomSpeak __stub_ChatRoomSpeak((void*)0);
static ChatRoomList_Re __stub_ChatRoomList_Re((void*)0);
static TradeStart __stub_TradeStart((void*)0);
static TradeStart_Re __stub_TradeStart_Re((void*)0);
static TradeAddGoods_Re __stub_TradeAddGoods_Re((void*)0);
static TradeRemoveGoods_Re __stub_TradeRemoveGoods_Re((void*)0);
static TradeSubmit_Re __stub_TradeSubmit_Re((void*)0);
static TradeMoveObj_Re __stub_TradeMoveObj_Re((void*)0);
static TradeConfirm_Re __stub_TradeConfirm_Re((void*)0);
static TradeDiscard_Re __stub_TradeDiscard_Re((void*)0);
static TradeEnd __stub_TradeEnd((void*)0);
static GMRestartServer __stub_GMRestartServer((void*)0);
static GMOnlineNum_Re __stub_GMOnlineNum_Re((void*)0);
static GMListOnlineUser_Re __stub_GMListOnlineUser_Re((void*)0);
static GMKickoutUser_Re __stub_GMKickoutUser_Re((void*)0);
static GMKickoutRole_Re __stub_GMKickoutRole_Re((void*)0);
static GMShutup_Re __stub_GMShutup_Re((void*)0);
static GMShutupRole_Re __stub_GMShutupRole_Re((void*)0);
static GMToggleChat_Re __stub_GMToggleChat_Re((void*)0);
static GMRestartServer_Re __stub_GMRestartServer_Re((void*)0);
static GMForbidRole_Re __stub_GMForbidRole_Re((void*)0);
static Report2GM_Re __stub_Report2GM_Re((void*)0);
static Complain2GM_Re __stub_Complain2GM_Re((void*)0);
static QueryUserPrivilege_Re __stub_QueryUserPrivilege_Re((void*)0);
static ACRemoteCode __stub_ACRemoteCode((void*)0);
static GetMailList_Re __stub_GetMailList_Re((void*)0);
static GetMail_Re __stub_GetMail_Re((void*)0);
static GetMailAttachObj_Re __stub_GetMailAttachObj_Re((void*)0);
static DeleteMail_Re __stub_DeleteMail_Re((void*)0);
static PreserveMail_Re __stub_PreserveMail_Re((void*)0);
static PlayerSendMail_Re __stub_PlayerSendMail_Re((void*)0);
static AuctionOpen __stub_AuctionOpen((void*)0);
static AuctionBid __stub_AuctionBid((void*)0);
static AuctionList __stub_AuctionList((void*)0);
static AuctionClose __stub_AuctionClose((void*)0);
static AuctionGet __stub_AuctionGet((void*)0);
static AuctionOpen_Re __stub_AuctionOpen_Re((void*)0);
static AuctionBid_Re __stub_AuctionBid_Re((void*)0);
static AuctionClose_Re __stub_AuctionClose_Re((void*)0);
static AuctionList_Re __stub_AuctionList_Re((void*)0);
static AuctionGet_Re __stub_AuctionGet_Re((void*)0);
static AuctionAttendList __stub_AuctionAttendList((void*)0);
static AuctionAttendList_Re __stub_AuctionAttendList_Re((void*)0);
static AuctionExitBid __stub_AuctionExitBid((void*)0);
static AuctionExitBid_Re __stub_AuctionExitBid_Re((void*)0);
static UpdateRemainTime __stub_UpdateRemainTime((void*)0);
static AuctionGetItem __stub_AuctionGetItem((void*)0);
static AuctionGetItem_Re __stub_AuctionGetItem_Re((void*)0);

static SellPoint __stub_SellPoint((void*)0);
static BuyPoint __stub_BuyPoint((void*)0);
static GetSellList __stub_GetSellList((void*)0);
static FindSellPointInfo __stub_FindSellPointInfo((void*)0);
static SellCancel __stub_SellCancel((void*)0);
static SellPoint_Re __stub_SellPoint_Re((void*)0);
static GetSellList_Re __stub_GetSellList_Re((void*)0);
static FindSellPointInfo_Re __stub_FindSellPointInfo_Re((void*)0);
static AnnounceSellResult __stub_AnnounceSellResult((void*)0);
static SellCancel_Re __stub_SellCancel_Re((void*)0);
static BuyPoint_Re __stub_BuyPoint_Re((void*)0);
static LineList __stub_LineList((void*)0);
static GetTopTable __stub_GetTopTable((void*)0);
static GetTopTable_Re __stub_GetTopTable_Re((void*)0);

static GetEnemies __stub_GetEnemies((void*)0);
static UpdateEnemy __stub_UpdateEnemy((void*)0);
static GetEnemies_Re __stub_GetEnemies_Re((void*)0);
static UpdateEnemy_Re __stub_UpdateEnemy_Re((void*)0);
static StockCommission __stub_StockCommission((void*)0);
static StockBill __stub_StockBill((void*)0);
static StockAccount __stub_StockAccount((void*)0);

static StockCommission_Re __stub_StockCommission_Re((void*)0);
static StockAccount_Re __stub_StockAccount_Re((void*)0);
static StockTransaction_Re __stub_StockTransaction_Re((void*)0);
static StockBill_Re __stub_StockBill_Re((void*)0);
static StockCancel_Re __stub_StockCancel_Re((void*)0);
static StockCancel __stub_StockCancel((void*)0);
static CombatChallenge __stub_CombatChallenge((void*)0);
static CombatTop __stub_CombatTop((void*)0);
static CombatChallenge_Re __stub_CombatChallenge_Re((void*)0);
static CombatControl __stub_CombatControl((void*)0);
static CombatStatus __stub_CombatStatus((void*)0);
static CombatTop_Re __stub_CombatTop_Re((void*)0);

static MatrixChallenge __stub_MatrixChallenge((void*)0);
static MatrixResponse __stub_MatrixResponse((void*)0);
static FamilyRecord __stub_FamilyRecord((void*)0);
static CashLock __stub_CashLock((void*)0);
static CashPasswordSet __stub_CashPasswordSet((void*)0);
static CashLock_Re __stub_CashLock_Re((void*)0);
static CashPasswordSet_Re __stub_CashPasswordSet_Re((void*)0);
static ACAnswer __stub_ACAnswer((void*)0);
static ACQuestion __stub_ACQuestion((void*)0);

static AutolockSet __stub_AutolockSet((void*)0);
static AutolockSet_Re __stub_AutolockSet_Re((void*)0);
static InstancingGetList __stub_InstancingGetList((void*)0);
static InstancingGetField __stub_InstancingGetField((void*)0);
static InstancingKick __stub_InstancingKick((void*)0);
static InstancingStart __stub_InstancingStart((void*)0);
static InstancingAvailableList __stub_InstancingAvailableList((void*)0);
static InstancingAccept __stub_InstancingAccept((void*)0);
static InstancingLeave __stub_InstancingLeave((void*)0);
static InstancingAppoint __stub_InstancingAppoint((void*)0);
static InstancingEnter __stub_InstancingEnter((void*)0);
static InstancingGetList_Re __stub_InstancingGetList_Re((void*)0);
static InstancingGetField_Re __stub_InstancingGetField_Re((void*)0);
static InstancingKick_Re __stub_InstancingKick_Re((void*)0);
static InstancingStart_Re __stub_InstancingStart_Re((void*)0);
static InstancingAvailableList_Re __stub_InstancingAvailableList_Re((void*)0);
static InstancingJoinReq __stub_InstancingJoinReq((void*)0);
static InstancingJoin_Re __stub_InstancingJoin_Re((void*)0);
static InstancingLeave_Re __stub_InstancingLeave_Re((void*)0);
static InstancingAppoint_Re __stub_InstancingAppoint_Re((void*)0);
static InstancingAppointNotify __stub_InstancingAppointNotify((void*)0);
static InstancingEnter_Re __stub_InstancingEnter_Re((void*)0);
static InstancingStartNotify __stub_InstancingStartNotify((void*)0);
static InstancingCloseNotify __stub_InstancingCloseNotify((void*)0);
static InstancingKickNotify __stub_InstancingKickNotify((void*)0);
static InstancingAccept_Re __stub_InstancingAccept_Re((void*)0);


static SNSSetPlayerInfo __stub_SNSSetPlayerInfo((void*)0);
static SNSGetPlayerInfo __stub_SNSGetPlayerInfo((void*)0);
static SNSCancelMessage __stub_SNSCancelMessage((void*)0);
static SNSListMessage __stub_SNSListMessage((void*)0);
static SNSGetMessage __stub_SNSGetMessage((void*)0);
static SNSAcceptApply __stub_SNSAcceptApply((void*)0);
static SNSSetPlayerInfo_Re __stub_SNSSetPlayerInfo_Re((void*)0);
static SNSPressMessage_Re __stub_SNSPressMessage_Re((void*)0);
static SNSCancelMessage_Re __stub_SNSCancelMessage_Re((void*)0);
static SNSApply_Re __stub_SNSApply_Re((void*)0);
static SNSApplyNotify __stub_SNSApplyNotify((void*)0);
static SNSListMessage_Re __stub_SNSListMessage_Re((void*)0);
static SNSGetMessage_Re __stub_SNSGetMessage_Re((void*)0);
static SNSGetPlayerInfo_Re __stub_SNSGetPlayerInfo_Re((void*)0);
static SNSVote_Re __stub_SNSVote_Re((void*)0);
static SNSAcceptApply_Re __stub_SNSAcceptApply_Re((void*)0);
static SNSCancelLeaveMessage __stub_SNSCancelLeaveMessage((void*)0);
static SNSCancelLeaveMessage_Re __stub_SNSCancelLeaveMessage_Re((void*)0);


static RefListReferrals __stub_RefListReferrals((void*)0);
static RefWithdrawExp __stub_RefWithdrawExp((void*)0);
static RefWithdrawBonus __stub_RefWithdrawBonus((void*)0);
static RefGetReferenceCode __stub_RefGetReferenceCode((void*)0);
static RefListReferrals_Re __stub_RefListReferrals_Re((void*)0);
static RefWithdrawExp_Re __stub_RefWithdrawExp_Re((void*)0);
static RefWithdrawBonus_Re __stub_RefWithdrawBonus_Re((void*)0);
static RefGetReferenceCode_Re __stub_RefGetReferenceCode_Re((void*)0);

static FunGamedataSend __stub_FunGamedataSend((void*)0);

static GetServerRTT __stub_GetServerRTT((void*)0);
static AnnounceDistrictId __stub_AnnounceDistrictId((void*)0);

static HometowndataSend __stub_HometowndataSend((void*)0);
static HometownExchgMoney_Re __stub_HometownExchgMoney_Re((void*)0);
static AchievementMessage __stub_AchievementMessage((void*)0);
static GShopNotifyScheme __stub_GShopNotifyScheme((void*)0);
static PKMessage __stub_PKMessage((void*)0);
static RefineMessage __stub_RefineMessage((void*)0);

static TerritoryMapGet __stub_TerritoryMapGet((void*)0);
static TerritoryMapGet_Re __stub_TerritoryMapGet_Re((void*)0);
static TerritoryChallenge_Re __stub_TerritoryChallenge_Re((void*)0);
static TerritoryEnter_Re __stub_TerritoryEnter_Re((void*)0);
static TerritoryLeave_Re __stub_TerritoryLeave_Re((void*)0);
static TerritoryEnterRemind __stub_TerritoryEnterRemind((void*)0);
static TerritoryScoreUpdate __stub_TerritoryScoreUpdate((void*)0);

static UniqueBidHistory __stub_UniqueBidHistory((void*)0);
static UniqueBidHistory_Re __stub_UniqueBidHistory_Re((void*)0);
static UniqueBid_Re __stub_UniqueBid_Re((void*)0);
static UniqueGetItem_Re __stub_UniqueGetItem_Re((void*)0);

static PlayerBaseInfo2 __stub_PlayerBaseInfo2((void*)0);
static PlayerBaseInfo2_Re __stub_PlayerBaseInfo2_Re((void*)0);

static CircleList __stub_CircleList((void*)0);
static CircleList_Re __stub_CircleList_Re((void*)0);
static ClaimCircleGradBonus __stub_ClaimCircleGradBonus((void*)0);
static GCircleChat __stub_GCircleChat((void*)0);
static GetCircleBaseInfo __stub_GetCircleBaseInfo((void*)0);
static GetCircleBaseInfo_Re __stub_GetCircleBaseInfo_Re((void*)0);
static CircleNotify __stub_CircleNotify((void*)0);

static KeyReestablish __stub_KeyReestablish((void*)0);
static PlayerChangeDS_Re __stub_PlayerChangeDS_Re((void*)0);
static ChangeDS_Re __stub_ChangeDS_Re((void*)0);
static TryChangeGS __stub_TryChangeGS((void*)0);
static TryChangeGS_Re __stub_TryChangeGS_Re((void*)0);
static DoChangeGS __stub_DoChangeGS((void*)0);
static DoChangeGS_Re __stub_DoChangeGS_Re((void*)0);

static ConsignCancelPost __stub_ConsignCancelPost((void*)0);
static ConsignCancelPost_Re __stub_ConsignCancelPost_Re((void*)0);
static ConsignQuery __stub_ConsignQuery((void*)0);
static ConsignQuery_Re __stub_ConsignQuery_Re((void*)0);
static ConsignStartErr __stub_ConsignStartErr((void*)0);

static ConsignGetItem __stub_ConsignGetItem((void*)0);
static ConsignGetItem_Re __stub_ConsignGetItem_Re((void*)0);
static ConsignListAll __stub_ConsignListAll((void*)0);
static ConsignListAll_Re __stub_ConsignListAll_Re((void*)0);
static ConsignListLargeCategory __stub_ConsignListLargeCategory((void*)0);
static ConsignListLargeCategory_Re __stub_ConsignListLargeCategory_Re((void*)0);

static GateOfflineChat __stub_GateOfflineChat((void*)0);
static GetFriendGTStatus __stub_GetFriendGTStatus((void*)0);
static UpdateFriendGTStatus __stub_UpdateFriendGTStatus((void*)0);
static GateGetGTRoleStatus_Re __stub_GateGetGTRoleStatus_Re((void*)0);


static FriendCallbackInfo_Re __stub_FriendCallbackInfo_Re((void*)0);
static FriendCallbackMail_Re __stub_FriendCallbackMail_Re((void*)0);
static FriendCallbackAward_Re   __stub_FriendCallbackAward_Re((void*)0);
static FriendCallbackSubscribe_Re __stub_FriendCallbackSubscribe_Re((void*)0);

static RaidStart __stub_RaidStart((void*)0);
static RaidEnter __stub_RaidEnter((void*)0);
static RaidQuit __stub_RaidQuit((void*)0);
static RaidGetList __stub_RaidGetList((void*)0);
static RaidAppoint __stub_RaidAppoint((void*)0);
static RaidKick __stub_RaidKick((void*)0);
static RaidGetRoom __stub_RaidGetRoom((void*)0);
static RaidJoinAccept __stub_RaidJoinAccept((void*)0);
static RaidKick_Re __stub_RaidKick_Re((void*)0);
static RaidKickNotify __stub_RaidKickNotify((void*)0);
static RaidQuit_Re __stub_RaidQuit_Re((void*)0);
static RaidGetRoom_Re __stub_RaidGetRoom_Re((void*)0);
static RaidJoinReq __stub_RaidJoinReq((void*)0);
static RaidOpen_Re __stub_RaidOpen_Re((void*)0);
static RaidJoin_Re __stub_RaidJoin_Re((void*)0);
static RaidJoinApplyList __stub_RaidJoinApplyList((void*)0);
static RaidJoinAccept_Re __stub_RaidJoinAccept_Re((void*)0);
static RaidEnter_Re __stub_RaidEnter_Re((void*)0);
static RaidGetList_Re __stub_RaidGetList_Re((void*)0);
static RaidStart_Re __stub_RaidStart_Re((void*)0);
static RaidAppoint_Re __stub_RaidAppoint_Re((void*)0);
static RaidAppointNotify __stub_RaidAppointNotify((void*)0);
static RaidStartNotify __stub_RaidStartNotify((void*)0);

static GetKDCToken __stub_GetKDCToken((void*)0);
static GetKDCToken_Re __stub_GetKDCToken_Re((void*)0);

static ConsignPlayer_Re __stub_ConsignPlayer_Re((void*)0);
static ConsignListRole __stub_ConsignListRole((void*)0);
static ConsignListRole_Re __stub_ConsignListRole_Re((void*)0);

static SSOGetTicketReq __stub_SSOGetTicketReq((void*)0);
static SSOGetTicketRep __stub_SSOGetTicketRep((void*)0);

static FastPay __stub_FastPay((void*)0);
static FastPay_Re __stub_FastPay_Re((void*)0);
static DiscountAnnounce __stub_DiscountAnnounce((void*)0);

static RaidOpenVote __stub_RaidOpenVote((void*)0);
static RaidVotes __stub_RaidVotes((void*)0);
static RaidOpenVote_Re __stub_RaidOpenVote_Re((void*)0);
static RaidBeginVotes __stub_RaidBeginVotes((void*)0);
static RaidVoteResult __stub_RaidVoteResult((void*)0);
static RaidBriefInfo __stub_RaidBriefInfo((void*)0);
static GlobalDropBroadCast __stub_GlobalDropBroadCast((void*)0);
static GlobalDropRemainBroadCast __stub_GlobalDropRemainBroadCast((void*)0);
static FastPayBindInfo __stub_FastPayBindInfo((void*)0);
static QueryRolenameHistory __stub_QueryRolenameHistory((void*)0);
static QueryRolenameHistory_Re __stub_QueryRolenameHistory_Re((void*)0);
static ChangeRolename_Re __stub_ChangeRolename_Re((void*)0);
static FriendNameChange __stub_FriendNameChange((void*)0);
static TeamRaidMappingSuccess_Re __stub_TeamRaidMappingSuccess_Re((void*)0);
static TeamRaidUpdateChar __stub_TeamRaidUpdateChar((void*)0);
static TeamRaidApply_Re __stub_TeamRaidApply_Re((void*)0);
static TeamRaidMappingSuccess __stub_TeamRaidMappingSuccess((void*)0);
static TeamRaidBeginChooseChar __stub_TeamRaidBeginChooseChar((void*)0);
static TeamRaidAckTimeout __stub_TeamRaidAckTimeout((void*)0);
static ChangeFactionName_Re __stub_ChangeFactionName_Re((void*)0);
static FactionPkRaidChallenge __stub_FactionPkRaidChallenge((void*)0);
static FactionPkRaidInvite_Re __stub_FactionPkRaidInvite_Re((void*)0);
static FactionPkRaidGetScore __stub_FactionPkRaidGetScore((void*)0);
static FactionPkRaidGetList __stub_FactionPkRaidGetList((void*)0);
static FactionPkRaidChallenge_Re __stub_FactionPkRaidChallenge_Re((void*)0);
static FactionPkRaidInvite __stub_FactionPkRaidInvite((void*)0);
static FactionPkRaidBegin __stub_FactionPkRaidBegin((void*)0);
static FactionPkRaidGetScore_Re __stub_FactionPkRaidGetScore_Re((void*)0);
static FactionPkRaidGetList_Re __stub_FactionPkRaidGetList_Re((void*)0);
static FactionPkRaidStopFight __stub_FactionPkRaidStopFight((void*)0);
static FactionPkRaidChallengeCancel __stub_FactionPkRaidChallengeCancel((void*)0);
static FactionPkRaidChallengeCancel_Re __stub_FactionPkRaidChallengeCancel_Re((void*)0);
static FactionWithDrawBonus __stub_FactionWithDrawBonus((void*)0);
static FactionWithDrawBonus_Re __stub_FactionWithDrawBonus_Re((void*)0);
static KingdomBattleEnter_Re __stub_KingdomBattleEnter_Re((void*)0);
static KingdomBattleLeave_Re __stub_KingdomBattleLeave_Re((void*)0);
static KingdomAnnounce __stub_KingdomAnnounce((void*)0);
static KingdomAppoint __stub_KingdomAppoint((void*)0);
static KingdomDischarge __stub_KingdomDischarge((void*)0);
static KingdomSetGameAttri __stub_KingdomSetGameAttri((void*)0);
static KingdomGetInfo __stub_KingdomGetInfo((void*)0);
static KingdomAnnounce_Re __stub_KingdomAnnounce_Re((void*)0);
static KingdomAppoint_Re __stub_KingdomAppoint_Re((void*)0);
static KingdomDischarge_Re __stub_KingdomDischarge_Re((void*)0);
static KingdomSetGameAttri_Re __stub_KingdomSetGameAttri_Re((void*)0);
static KingdomGetInfo_Re __stub_KingdomGetInfo_Re((void*)0);
static KingdomGetTitle __stub_KingdomGetTitle((void*)0);
static KingdomGetTitle_Re __stub_KingdomGetTitle_Re((void*)0);
static KingdomTitleChange __stub_KingdomTitleChange((void*)0);
static KingdomPointChange __stub_KingdomPointChange((void*)0);
static KingdomKingChange __stub_KingdomKingChange((void*)0);
static KingCallGuards_Re __stub_KingCallGuards_Re((void*)0);
static QueenTryOpenBath __stub_QueenTryOpenBath((void*)0);
static QueenOpenBath_Re __stub_QueenOpenBath_Re((void*)0);
static GetKingTask __stub_GetKingTask((void*)0);
static KingIssueTask_Re __stub_KingIssueTask_Re((void*)0);
static GetKingTask_Re __stub_GetKingTask_Re((void*)0);

static GetCommonData __stub_GetCommonData((void*)0);
static GetCommonData_Re __stub_GetCommonData_Re((void*)0);
static TeamRaidQuit __stub_TeamRaidQuit((void*)0);
static TeamRaidActiveApplyNotice __stub_TeamRaidActiveApplyNotice((void*)0);
static TeamRaidApplyNotice __stub_TeamRaidApplyNotice((void*)0);

static SendFlowerReq __stub_SendFlowerReq((void*)0);
static TopFlowerReq __stub_TopFlowerReq((void*)0);
static TopFlower_Re __stub_TopFlower_Re((void*)0);
static TopFlower_Err __stub_TopFlower_Err((void*)0);
static TopFlowerMsg_Re __stub_TopFlowerMsg_Re((void*)0);

static OpenBanquetNotify_Re __stub_OpenBanquetNotify_Re((void*)0);
static OpenBanquetCancel __stub_OpenBanquetCancel((void*)0);
static OpenBanquetJoin_Re __stub_OpenBanquetJoin_Re((void*)0);
static OpenBanquetEnter __stub_OpenBanquetEnter((void*)0);
static OpenBanquetEnter_Re __stub_OpenBanquetEnter_Re((void*)0);
static OpenBanquetQueueCount __stub_OpenBanquetQueueCount((void*)0);
static OpenBanquetDrag_Re __stub_OpenBanquetDrag_Re((void*)0);
static OpenBanquetQueueCount_Re __stub_OpenBanquetQueueCount_Re((void*)0);
static OpenBanquetGetField __stub_OpenBanquetGetField((void*)0);
static OpenBanquetGetField_Re __stub_OpenBanquetGetField_Re((void*)0);

static ACAccuse __stub_ACAccuse((void*)0);
static GetTouchPoint __stub_GetTouchPoint((void*)0);
static GetPassportCashAdd __stub_GetPassportCashAdd((void*)0);
static GetTouchPoint_Re __stub_GetTouchPoint_Re((void*)0);
static GetPassportCashAdd_Re __stub_GetPassportCashAdd_Re((void*)0);
static TouchPointExchange_Re __stub_TouchPointExchange_Re((void*)0);
static ACAccuse_Re __stub_ACAccuse_Re((void*)0);
static ACClientTimeout __stub_ACClientTimeout((void*)0);
static GetFactionDynamic __stub_GetFactionDynamic((void*)0);
static GetFactionMsg __stub_GetFactionMsg((void*)0);
static GetFactionDynamic_Re __stub_GetFactionDynamic_Re((void*)0);
static GetFactionMsg_Re __stub_GetFactionMsg_Re((void*)0);
static FactionPostMsg __stub_FactionPostMsg((void*)0);
static FactionPostMsg_Re __stub_FactionPostMsg_Re((void*)0);

static CrssvrTeamsGetTeamBase __stub_CrssvrTeamsGetTeamBase((void*)0);
static CrssvrTeamsGetRoleList __stub_CrssvrTeamsGetRoleList((void*)0);
static CrssvrTeamsGetRoleNotify __stub_CrssvrTeamsGetRoleNotify((void*)0);
static CrssvrTeamsRename __stub_CrssvrTeamsRename((void*)0);
static CrssvrTeamsDismiss __stub_CrssvrTeamsDismiss((void*)0);
static CrssvrTeamsRecruit __stub_CrssvrTeamsRecruit((void*)0);
static CrssvrTeamsChgCaptain __stub_CrssvrTeamsChgCaptain((void*)0);
static CrssvrTeamsLeave __stub_CrssvrTeamsLeave((void*)0);
static CrssvrTeamsKick __stub_CrssvrTeamsKick((void*)0);
static CrssvrTeamsSearch __stub_CrssvrTeamsSearch((void*)0);

static CrssvrTeamsTeamBase_Re __stub_CrssvrTeamsTeamBase_Re((void*)0);
static CrssvrTeamsRoleList_Re __stub_CrssvrTeamsRoleList_Re((void*)0);
static CrssvrTeamsRoleNotify_Re __stub_CrssvrTeamsRoleNotify_Re((void*)0);
static CrssvrTeamsNotify_Re __stub_CrssvrTeamsNotify_Re((void*)0);
static CrssvrTeamsSearch_Re __stub_CrssvrTeamsSearch_Re((void*)0);

static KingGetReward_Re __stub_KingGetReward_Re((void*)0);
static CreateFacBase __stub_CreateFacBase((void*)0);
static CreateFacBase_Re __stub_CreateFacBase_Re((void*)0);
static FacBaseEnter_Re __stub_FacBaseEnter_Re((void*)0);
static GetFacActTopTable __stub_GetFacActTopTable((void*)0);
static GetFacActTopTable_Re __stub_GetFacActTopTable_Re((void*)0);
static FacBaseDataSend __stub_FacBaseDataSend((void*)0);
static FacBaseStopNotice __stub_FacBaseStopNotice((void*)0);
};
