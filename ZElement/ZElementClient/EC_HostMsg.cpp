/*
 * FILE: EC_HostMsg.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/12/21
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Utility.h"
#include "EC_World.h"
#include "EC_GameSession.h"
#include "EC_NPCServer.h"
#include "EC_NPC.h"
#include "EC_Interactor.h"
#include "EC_RTDebug.h"
#include "EC_ManNPC.h"
#include "EC_ManPlayer.h"
#include "EC_UIManager.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_DealInventory.h"
#include "EC_FixedMsg.h"
#include "EC_IvtrTypes.h"
#include "EC_IvtrEquip.h"
#include "EC_IvtrItem.h"
#include "A3DGFXExMan.h"
#include "EC_GFXCaster.h"
#include "EC_ElsePlayer.h"
#include "EC_Resource.h"
#include "EC_Team.h"
#include "EC_TeamMan.h"
#include "EC_Skill.h"
#include "EC_TaskInterface.h"
#include "EC_Configs.h"
#include "EC_Friend.h"
#include "EC_Faction.h"
#include "EC_Model.h"
#include "EC_Clipboard.h"
#include "EC_GameUIMan.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#else
#include "ECViewport.h"
#endif
#include "EC_UIManager.h"
#include "EC_PetCorral.h"
#include "EC_ComboSkill.h"
#include "EC_CDR.h"
#include "ECScriptMan.h"
#include "ECScriptContext.h"
#include "ECSCriptCheckState.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_Achievement.h"
#include "EC_ShortcutSet.h"
#include "EC_BattleDungeon.h"
#include "EC_BattleDungeonTower.h"
#include "TaskTemplMan.h"
#include "EC_AutoHelper.h"
#include "EC_HelpGraphGuide.h"
#include "EC_NewHelpGuide.h"

#include "defence/stackchecker.h"

#include "DlgBattleInfo.h"
#include "DlgInfo.h"
#include "DlgHost.h"
#include "DlgTrade.h"
#include "DlgFriendList.h"
#include "DlgChannelChat.h"
#include "DlgChat.h"
#include "DlgMailList.h"
#include "DlgAuctionList.h"
#include "DlgEPEquip.h"
#include "DlgFrdCallBck.h"
#include "DlgGuildMap.h"
#include "DlgPetAction.h"
#include "DlgTeacherMan.h"
#include "DlgInventory.h"
#include "DlgStorage.h"
#include "DlgBattleSong.h"
#include "DlgBattlePk.h"
#include "DlgRevive.h"
#include "DlgBattleInstance.h"
#include "DlgTerrWarMap.h"
#include "DlgCameraPath.h"
#include "DlgTalisman.h"
#include "DlgMinimizeBar.h"
#include "globaldataman.h"
#include "DlgPKMsg.h"
#include "DlgFengshenUpgrade.h"
#include "DlgFengshenConvert.h"
#include "DlgLevelUp.h"
#include "DlgRemedyMetempsychosis.h"
#include "DlgDynamicSystem.h"
#include "DlgDungeonAncient.h" 
#include "DlgInventoryRide.h"
#include "DlgCalendarPresent.h"
#include "DlgMidMap.h"
#include "DlgTalismanService.h"
#include "DlgAttriUp.h"
#include "DlgKingWar.h"
#include "DlgProduce.h"
#include "DlgRoleRune.h"
#include "DlgBattleFlow.h"
#include "DlgWebShop.h"
#include "DlgSkillSpeOperate.h"
#include "DlgQuickBar.h"
#include "DlgIndividualityColor.h"
#include "DlgGuildMain.h"
#include "DlgGuildLandBuild.h"
#include "DlgGuildShop.h"

#include "gnetdef.h"
#include "TradeStart_Re.hpp"
#include "TradeStartRqst.hrp"
#include "TradeMoveObj_Re.hpp"
#include "TradeDiscard_Re.hpp"
#include "TradeConfirm_Re.hpp"
#include "TradeAddGoods_Re.hpp"
#include "TradeRemoveGoods_Re.hpp"
#include "TradeSubmit_Re.hpp"
#include "TradeEnd.hpp"
#include "PlayerBaseInfo_Re.hpp"
#include "addfriend_re.hpp"
#include "getfriends_re.hpp"
#include "setgroupname_re.hpp"
#include "setfriendgroup_re.hpp"
#include "delfriend_re.hpp"
#include "friendstatus.hpp"
#include "addfriendrqst.hrp"
#include "getsavedmsg_re.hpp"
#include "setcustomdata_re.hpp"
#include "chatroomcreate_re.hpp"
#include "chatroomexpel.hpp"
#include "chatroominvite.hpp"
#include "chatroominvite_re.hpp"
#include "chatroomjoin_re.hpp"
#include "chatroomleave.hpp"
#include "chatroomlist_re.hpp"
#include "chatroomspeak.hpp"
#include "announcenewmail.hpp"
#include "getmail_re.hpp"
#include "getmaillist_re.hpp"
#include "deletemail_re.hpp"
#include "preservemail_re.hpp"
#include "playersendmail_re.hpp"
#include "getmailattachobj_re.hpp"
#include "auctionattendlist_re.hpp"
#include "auctionbid_re.hpp"
#include "auctionclose_re.hpp"
#include "auctionget_re.hpp"
#include "auctionlist_re.hpp"
#include "auctionopen_re.hpp"
#include "auctionexitbid_re.hpp"
#include "auctiongetitem_re.hpp"

#include "battleenter_re.hpp"
#include "battlejoin_re.hpp"
#include "battlestartnotify.hpp"
#include "battlegetfield_re.hpp"
#include "battlegetlist_re.hpp"

#include "sellcancel_re.hpp"
#include "sellpoint_re.hpp"
#include "findsellpointinfo_re.hpp"
#include "buypoint_re.hpp"
#include "getselllist_re.hpp"
#include "announcesellresult.hpp"
#include "familyrecord.hpp"
#include "instancingenter_re.hpp"
#include "territorychallenge_re.hpp"
#include "territoryenter_re.hpp"
#include "territoryleave_re.hpp"
#include "territorymapget_re.hpp"
#include "territoryenterremind.hpp"
#include "getcirclebaseinfo_re.hpp"
#include "circlelist_re.hpp"
#include "circlenotify.hpp"
#include "raidopenvote_re.hpp"
#include "raidbriefinfo.hpp"

#include "friendcallbackinfo_re.hpp"
#include "friendcallbackmail_re.hpp"
#include "friendcallbackaward_re.hpp"
#include "friendnamechange.hpp"
#include "crssvrteamsinvite.hrp"

#include "EC_HPWork.h"
#include "EC_HPWorkDead.h"
#include "EC_HPWorkMelee.h"
#include "EC_HPWorkStand.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkFollow.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkFly.h"
#include "EC_HPWorkFall.h"
#include "EC_HPWorkSpell.h"
#include "EC_HPWorkSit.h"
#include "EC_HPWorkUse.h"
#include "EC_HPWorkMoveBezier.h"
#include "EC_GameRecord.h"
#include "EC_Cutscene.h"
#include "EC_Circle.h"
#include "EC_GameUIMan.h"
#include "EC_OnlineAward.h"	//Added 2012-03-26.
#include "EC_LoginPlayer.h"
#include "EC_HPWorkMoveBezier.h"
#include "EC_ManOrnament.h"
#include <A3DTerrain2.h>
#include <A3DLight.h>
#include <A3DCamera.h>
#include "A3DFuncs.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUITextArea.h"
#include <AMSoundBufferMan.h>
#include <AMSoundEngine.h>

#include "LuaEvent.h"

#include "elementdataman.h"

#ifdef LUA_UI
#include "LuaEvent.h"
#endif

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

extern game_record gr;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECHostPlayer
//	
///////////////////////////////////////////////////////////////////////////

//	Process message
bool CECHostPlayer::ProcessMessage(const ECMSG& Msg)
{
	//  主角处在未初始化状态，无视服务器发过来的任何和主角相关的协议
	if (m_PlayerInfo.cid == 0)
	{
//		ASSERT(0);
		return true;
	}
	switch (Msg.dwMsg)
	{
	case MSG_GST_MOVE:				OnMsgGstMove(Msg);				break;
	case MSG_GST_PITCH:				OnMsgGstPitch(Msg);				break;
	case MSG_GST_YAW:				OnMsgGstYaw(Msg);				break;
	case MSG_GST_MOVEABSUP:			OnMsgGstMoveAbsUp(Msg);			break;
	case MSG_HST_PUSHMOVE:			OnMsgHstPushMove(Msg);			break;
	case MSG_HST_PITCH:				OnMsgHstPitch(Msg);				break;
	case MSG_HST_YAW:				OnMsgHstYaw(Msg);				break;
	case MSG_HST_CAMERAMODE:		OnMsgHstCameraMode(Msg);		break;
	case MSG_HST_LBTNCLICK:			OnMsgLBtnClick(Msg);			break;
	case MSG_HST_RBTNCLICK:			OnMsgRBtnClick(Msg);			break;
	case MSG_HST_RBTNCLICK_OTHER:	OnMsgRBtnClickForOtherUse(Msg);	break;	//Added 2012-05-07.
	case MSG_HST_WHEELCAM:			OnMsgHstWheelCam(Msg);			break;
	case MSG_HST_CAMDEFAULT:		OnMsgHstCamDefault(Msg);		break;
	case MSG_HST_CAMPRESET:			OnMsgHstCamPreset(Msg);			break;
	case MSG_HST_CAMUSERGET:		OnMsgHstCamUserGet(Msg);		break;
	case MSG_HST_CAMUSERSET:		OnMsgHstCamUserSet(Msg);		break;

	case MSG_HST_TURNCAM:
		
		if (Msg.dwParam1)
			m_iTurnCammera |= Msg.dwParam2 ? TURN_LBUTTON : TURN_RBUTTON;
		else
			m_iTurnCammera &= Msg.dwParam2 ? ~TURN_LBUTTON : ~TURN_RBUTTON;

		break;

	case MSG_HST_ATKRESULT:			OnMsgHstAttackResult(Msg);		break;
	case MSG_HST_ATTACKED:			OnMsgHstAttacked(Msg);			break;
	case MSG_HST_DIED:				OnMsgHstDied(Msg);				break;
	case MSG_HST_PICKUPMONEY:		OnMsgHstPickupMoney(Msg);		break;
	case MSG_HST_PICKUPITEM:		OnMsgHstPickupItem(Msg);		break;
	case MSG_HST_FOLLOW:			BeginFollow(Msg.dwParam1);		break;
	case MSG_HST_RECEIVEEXP:		OnMsgHstReceiveExp(Msg);		break;
	case MSG_HST_INFO00:			OnMsgHstInfo00(Msg);			break;
	case MSG_HST_GOTO:				OnMsgHstGoto(Msg);				break;
	case MSG_HST_OWNITEMINFO:		OnMsgHstOwnItemInfo(Msg);		break;
	case MSG_HST_SELTARGET:			OnMsgHstSelTarget(Msg);			break;
	case MSG_HST_FIXCAMERA:			OnMsgHstFixCamera(Msg);			break;
	case MSG_HST_OWNEXTPROP:		OnMsgHstExtProp(Msg);			break;
	case MSG_HST_ADDSTATUSPT:		OnMsgHstAddStatusPt(Msg);		break;
	case MSG_HST_JOINTEAM:			OnMsgHstJoinTeam(Msg);			break;
	case MSG_HST_LEAVETEAM:			OnMsgHstLeaveTeam(Msg);			break;
	case MSG_HST_NEWTEAMMEM:		OnMsgHstNewTeamMem(Msg);		break;
	case MSG_HST_ITEMOPERATION:		OnMsgHstItemOperation(Msg);		break;
	case MSG_HST_TRASHBOXOP:		OnMsgHstTrashBoxOperation(Msg);	break;
	case MSG_HST_TEAMINVITE:		OnMsgHstTeamInvite(Msg);		break;
	case MSG_HST_TEAMREJECT:		OnMsgHstTeamReject(Msg);		break;
	case MSG_HST_TEAMMEMPOS:		OnMsgHstTeamMemPos(Msg);		break;
	case MSG_HST_EQUIPDAMAGED:		OnMsgHstEquipDamaged(Msg);		break;
	case MSG_HST_TEAMMEMPICKUP:		OnMsgHstTeamMemPickup(Msg);		break;
	case MSG_HST_NPCGREETING:		OnMsgHstNPCGreeting(Msg);		break;
	case MSG_HST_TRADESTART:		OnMsgHstTradeStart(Msg);		break;
	case MSG_HST_TRADEREQUEST:		OnMsgHstTradeRequest(Msg);		break;
	case MSG_HST_TRADEMOVEITEM:		OnMsgHstTradeMoveItem(Msg);		break;
	case MSG_HST_TRADECANCEL:		OnMsgHstTradeCancel(Msg);		break;
	case MSG_HST_TRADESUBMIT:		OnMsgHstTradeSubmit(Msg);		break;
	case MSG_HST_TARDECONFIRM:		OnMsgHstTradeConfirm(Msg);		break;
	case MSG_HST_TRADEEND:			OnMsgHstTradeEnd(Msg);			break;
	case MSG_HST_TRADEADDGOODS:		OnMsgHstTradeAddGoods(Msg);		break;
	case MSG_HST_TRADEREMGOODS:		OnMsgHstTradeRemGoods(Msg);		break;
	case MSG_HST_IVTRINFO:			OnMsgHstIvtrInfo(Msg);			break;
	case MSG_HST_STARTATTACK:		OnMsgHstStartAttack(Msg);		break;
	case MSG_HST_GAINITEM:			OnMsgHstGainItem(Msg);			break;
	case MSG_HST_PURCHASEITEMS:		OnMsgHstPurchaseItems(Msg);		break;
	case MSG_HST_SPENDMONEY:		OnMsgHstSpendMoney(Msg);		break;
	case MSG_HST_ITEMTOMONEY:		OnMsgHstItemToMoney(Msg);		break;
	case MSG_HST_REPAIR:			OnMsgHstRepair(Msg);			break;
	case MSG_HST_USEITEM:			OnMsgHstUseItem(Msg);			break;
	case MSG_HST_USEITEMWITHDATA:	OnMsgHstUseItemWithData(Msg);	break;
	case MSG_HST_SKILLDATA:			OnMsgHstSkillData(Msg);			break;
	case MSG_HST_EMBEDITEM:			OnMsgHstEmbedItem(Msg);			break;
	case MSG_HST_CLEARTESSERA:		OnMsgHstClearTessera(Msg);		break;
	case MSG_HST_COSTSKILLPT:		OnMsgHstCostSkillPt(Msg);		break;
	case MSG_HST_LEARNSKILL:		OnMsgHstLearnSkill(Msg);		break;
	case MSG_HST_FLYSWORDTIME:		OnMsgHstFlySwordTime(Msg);		break;
	case MSG_HST_PRODUCEITEM:		OnMsgHstProduceItem(Msg);		break;
	case MSG_HST_BREAKITEM:			OnMsgHstBreakItem(Msg);			break;
	case MSG_HST_TASKDATA:			OnMsgHstTaskData(Msg);			break;
	case MSG_HST_TARGETISFAR:		OnMsgHstTargetIsFar(Msg);		break;
	case MSG_HST_PRESSCANCEL:		OnMsgHstPressCancel(Msg);		break;
	case MSG_HST_ROOTNOTIFY:		OnMsgHstRootNotify(Msg);		break;
	case MSG_HST_STOPATTACK:		OnMsgHstStopAttack(Msg);		break;
	case MSG_HST_JUMP:				OnMsgHstJump(Msg);				break;
	case MSG_HST_HURTRESULT:		OnMsgHstHurtResult(Msg);		break;
	case MSG_HST_ATTACKONCE:		OnMsgHstAttackOnce(Msg);		break;
	case MSG_HST_PLAYTRICK:			OnMsgHstPlayTrick(Msg);			break;
	case MSG_HST_SKILLRESULT:		OnMsgHstSkillResult(Msg);		break;
	case MSG_HST_SKILLATTACKED:		OnMsgHstSkillAttacked(Msg);		break;
	case MSG_HST_ASKTOJOINTEAM:		OnMsgHstAskToJoinTeam(Msg);		break;
	case MSG_HST_FACTION:			OnMsgHstFaction(Msg);			break;	
	case MSG_HST_SECT:				OnMsgHstSect(Msg);				break;	
	case MSG_HST_TASKDELIVER:		OnMsgHstTaskDeliver(Msg);		break;
	case MSG_HST_ITEMIDENTIFY:		OnMsgHstItemIdentify(Msg);		break;
	case MSG_HST_SANCTUARY:			OnMsgHstSanctuary(Msg);			break;
	case MSG_HST_CORRECTPOS:		OnMsgHstCorrectPos(Msg);		break;
	case MSG_HST_FRIENDOPT:			OnMsgHstFriendOpt(Msg);			break;
	case MSG_HST_WAYPOINT:			OnMsgHstWayPoint(Msg);			break;
	case MSG_HST_BREATHDATA:		OnMsgHstBreathData(Msg);		break;
	case MSG_HST_SKILLABILITY:		OnMsgHstSkillAbility(Msg);		break;
	case MSG_HST_COOLTIMEDATA:		OnMsgHstCoolTimeData(Msg);		break;
	case MSG_HST_REVIVALINQUIRE:	OnMsgHstRevivalInquire(Msg);	break;
	case MSG_HST_SETCOOLTIME:		OnMsgHstSetCoolTime(Msg);		break;
	case MSG_HST_CHGTEAMLEADER:		OnMsgHstChangeTeamLeader(Msg);	break;
	case MSG_HST_EXITINSTANCE:		OnMsgHstExitInstance(Msg);		break;
	case MSG_HST_CHANGEFACE:		OnMsgHstChangeFace(Msg);		break;
	case MSG_HST_TEAMMEMBERDATA:	OnMsgHstTeamMemberData(Msg);	break;
	case MSG_HST_SETMOVESTAMP:		OnMsgHstSetMoveStamp(Msg);		break;
	case MSG_HST_CHATROOMOPT:		OnMsgHstChatroomOpt(Msg);		break;
	case MSG_HST_MAILOPT:			OnMsgHstMailOpt(Msg);			break;
	case MSG_HST_VENDUEOPT:			OnMsgHstVendueOpt(Msg);			break;
	case MSG_HST_VIEWOTHEREQUIP:	OnMsgHstViewOtherEquips(Msg);	break;
	case MSG_HST_PARIAHTIME:		OnMsgHstPariahTime(Msg);		break;
	case MSG_HST_PETOPT:			OnMsgHstPetOpt(Msg);			break;
	case MSG_HST_BATTLEOPT:			OnMsgHstBattleOpt(Msg);			break;
	case MSG_HST_INSTANCINGOPT:		OnMsgHstInstancingOpt(Msg);		break;
	case MSG_HST_RAIDOPT:			OnMsgHstRaidOpt(Msg);			break; // 副本相关的协议处理Added 2011-07-21.
	case MSG_HST_BE_MOVED:			OnMsgHstBeMoved(Msg);			break; // 处理主角玩家被拉扯/牵引的移动协议Added 2011-07-27.
	case MSG_HST_PULL_OPT:			OnMsgHstPullOpt(Msg);			break; // 处理玩家拉扯/牵引技能的相关协议（除移动消息）Added 2011-07-29.
	case MSG_HST_ONLINE_AWARD:		OnMsgHstOnlineAwardOpt(Msg);	break; // 处理主角玩家在线奖励相关协议 Added 2012-02-16.
	case MSG_HST_ACCOUNTPOINT:		OnMsgHstAccountPoint(Msg);		break;
	case MSG_HST_GMOPT:				OnMsgHstGMOpt(Msg);				break;
	case MSG_HST_AUTOSELECT:		OnMsgHstAutoSelect(Msg);		break;
	case MSG_HST_MOVE_RECORD:		OnMsgMoveRecord(Msg);			break;
	case MSG_HST_TRACE_RECORD:		OnMsgTraceRecord(Msg);			break;
	case MSG_HST_FOLLOW_RECORD:		OnMsgFollowRecord(Msg);			break;
	case MSG_HST_WALK_RECORED:		OnMsgWalkRecord(Msg);			break;
	case MSG_HST_CHECK_RECORD:		OnMsgCheckRecord(Msg);			break;
	case MSG_HST_PUSH_RECORD:		OnMsgPushRecord(Msg);			break;
	case MSG_HST_POCKETOPT:			OnMsgHstPocketOperation(Msg);	break;
	case MSG_HST_FASHIONPACKOPT:	OnMsgHstFashionPackOpt(Msg);	break;
	case MSG_HST_ACHIEVEMENT:		OnMsgPlayerAchievement(Msg);	break;
	case MSG_HST_TSSKILLDATA:		OnMsgTransfigureSkillData(Msg); break;
	case MSG_HST_RANDOMMOVE:		OnMsgHstRandomMove(Msg);		break;
	case MSG_HST_TERRITORYOPT:		OnMsgHstTerritoryOpt(Msg);		break;
	case MSG_HST_COMBINE_SKILLDATA:	OnMsgHstCombineSkillData(Msg);	break;
	case MSG_HST_CIRCLE:			OnMsgHstCircleOpt(Msg);			break;
	case MSG_HST_FALL_RECORD:		OnMsgHstFallRecord(Msg);		break;
	case MSG_HST_FRIENDCALLBCKOPT:  OnMsgHstFriendCallBckOpt(Msg);  break;	
	case MSG_HST_ADDON_PERMANENT_SKILL:	OnMsgHstAddonPermanentSkill(Msg); break;	//处理主角关于时装等附加技能的相关协议 Added 2012-02-20.
	case MSG_HOST_5YEAR_ANNI:		OnMsgHst5YearAnni(Msg);			break;
	case MSG_HOST_SELL_ROLE_SUCCESS:OnMsgHstSellRoleSuccess(Msg);	break;
	case MSG_HST_TREASURE_REGION_OPT:	OnMsgHstTreasureRegionOpt(Msg);	break;		//处理诛仙古卷挖宝相关gs发过来的协议 Added 2012-03-22.
	case MSG_HST_TOWER_RAID_OPT:		OnMsgHstTowerRaidOpt(Msg);	break;			//处理趴塔副本相关协议，包括副本地图全局变量信息协议 Added 2012-04-17.
	case MSG_HST_LITTLEPET:			OnMsgHstLittlePet(Msg);	break;	
	case MSG_HOST_RUNEINFO:			OnMsgHstRuneInfo(Msg);	break;	
	case MSG_HST_UI_FUNCTION:		OnMsgHstUIFunction(Msg);	break;	
	case MSG_HST_MOUNTWINGPACKOPT:	OnMsgHstMountWingPackOpt(Msg);	break;	
	case MSG_HST_UI_TRANSFER_OPEN:	OnMsgHstUiTransferOpen(Msg);	break;
	case MSG_HST_NOTIFY_ASTROLOGY_ENERGY:	OnMsgHstNotifyAstrologyEnergy(Msg);	break;
	case MSG_HST_ASTROLOGY_UPGRADE_RESULT:	OnMsgHstAstrologyUpgradeResult(Msg);break;
	case MSG_HST_ASTROLOGY_DESTORY_RESULT:	OnMsgHstAstrologyDestoryResult(Msg);break;


	case MSG_PM_PLAYERBASEINFO:		OnMsgPlayerBaseInfo(Msg);		break;
	case MSG_PM_PLAYERCUSTOM:		OnMsgPlayerCustomData(Msg);		break;
	case MSG_PM_PLAYERFLY:			OnMsgPlayerFly(Msg);			break;
	case MSG_PM_PLAYERSITDOWN:		OnMsgPlayerSitDown(Msg);		break;
	case MSG_PM_CASTSKILL:			OnMsgPlayerCastSkill(Msg);		break;
	case MSG_PM_STARTATTACK:		OnMsgPlayerStartAtk(Msg);		break;
	case MSG_PM_PLAYERATKRESULT:	OnMsgPlayerAtkResult(Msg);		break;
	case MSG_PM_CHANGENAMECOLOR:	OnMsgChangeNameColor(Msg);		break;
	case MSG_PM_PLAYEREXTSTATE:		OnMsgPlayerExtState(Msg);		break;
	case MSG_PM_ENCHANTRESULT:		OnMsgEnchantResult(Msg);		break;
	case MSG_PM_PLAYERADVDATA:		OnMsgPlayerAdvData(Msg);		break;
	case MSG_PM_PLAYERGATHER:		OnMsgPlayerGather(Msg);			break;
	case MSG_PM_PLAYERDOEMOTE:		OnMsgPlayerDoEmote(Msg);		break;
	case MSG_PM_DOCONEMOTE:			OnMsgDoConEmote(Msg);			break;
	case MSG_PM_PICKUPMATTER:		OnMsgPickupMatter(Msg);			break;
	case MSG_PM_PLAYERCHGSHAPE:		OnMsgPlayerChangeShape(Msg);	break;
	case MSG_PM_BOOTHOPT:			OnMsgBoothOperation(Msg);		break;
	case MSG_PM_PLAYERTRAVEL:		OnMsgPlayerTravel(Msg);			break;
	case MSG_PM_PLAYERPVP:			OnMsgPlayerPVP(Msg);			break;
	case MSG_PM_FASHIONENABLE:		OnMsgSwitchFashionMode(Msg);	break;
	case MSG_PM_PLAYEREFFECT:		OnMsgPlayerEffect(Msg);			break;
	case MSG_PM_PLAYERUSEITEM:		OnMsgPlayerUseItem(Msg);		break;
	case MSG_PM_CHANGEFACE:			OnMsgPlayerChangeFace(Msg);		break;
	case MSG_PM_PLAYERBINDOPT:		OnMsgPlayerBindOpt(Msg);		break;
	case MSG_PM_BATH_OPT:           OnMsgPlayerBathOpt(Msg);        break;
	case MSG_PM_QILIN_BINDOPT:		OnMsgPlayerQilinBindOpt(Msg);   break;
	case MSG_PM_PLAYERMOUNT:		OnMsgPlayerMount(Msg);			break;
	case MSG_PM_PLAYERDUELOPT:		OnMsgPlayerDuelOpt(Msg);		break;
	case MSG_PM_PLAYERLEVEL2:		OnMsgPlayerLevel2(Msg);			break;
	case MSG_PM_PLAYERSKILLRESULT:	OnMsgPlayerSkillResult(Msg);	break;
	case MSG_PM_MULTIBINDOPT:       OnMsgPlayerMultiBindOpt(Msg);	break;
	case MSG_PM_CARRIER:			OnMsgPlayerCarrierOpt(Msg);		break;
	case MSG_PM_PLAYERSCALESHAPE:	OnMsgPlayerScaleShape(Msg);		break; //Added 2011-06-28.
	case MSG_PM_PLAYERVIPAWARDUPDATE: OnMsgPlayerVIPAwardUpdate(Msg); break; //Added 2012-01-17.
	case MSG_PM_START_INTERACT:		OnMsgPlayerStartInteract(Msg);	break; //Added 2013-07-29
	case MSG_PM_IN_INTERACT:		OnMsgPlayerInInteract(Msg);		break;
	case MSG_PM_END_INTERACT:		OnMsgPlayerEndInteract(Msg);	break;
	case MSG_PM_FINISH_INTERACT:	OnMsgPlayerFinishInteract(Msg);	break;
	case MSG_PM_CANCEL_INTERACT:	OnMsgPlayerCancelInteract(Msg);	break;
	case MSG_HST_PLAYER_REMEDY_METEMPSYCHOSIS_LEVEL: OnMsgHstRemedy_MetempsyChosisLevel(Msg); break; // 2012-10-18 add 弥补飞升值 by zy	
	case MSG_HST_LIVENESS_NOTIFY: OnMsgLiveness(Msg); break; // 2012-10-18 add 弥补飞升值 by zy	
	case MSG_HST_KINGDOM_OPT: OnMsgHstKingdomOpt(Msg); break;
	case MSG_HST_ANCIENT_RAID_OPT:  OnMsgHstAncientRaidOpt(Msg);		break; //处理上古副本的相关协议		
	case MSG_HST_SALE_PROMOTION_INFO: OnMsgSalePromotionInfo(Msg);		break;
	case MSG_HST_EVENT_OPT: OnMsgHstEventOpt(Msg); break;
	case MSG_HST_TALIAMAN_SERVER_OPT:OnMsgHstTalismanServiceOpt(Msg);break;
	case MSG_HST_PROPADD_RESULT:	 OnMsgHstPropAddResult(Msg);	break;	// 玩家基础属性增益
	case MSG_HST_FUWEN2013_OPT:		 OnMsgHstFuWenOpt(Msg);			break;
	case MSG_CSFLOW_BATTLE_PLAYER_INFO: OnMsgHstFlowBattlePlayerInfo(Msg); break;	// 流水席数据
	case MSG_HST_EXCHANGE_ITEM_LIST_INFO: OnMsgHstExchangeItemList(Msg); break;
	case MSG_HST_WEB_ORDER_INFO:	OnMsgHstWebOrderInfo(Msg);	break;
	case MSG_HST_BATTLE_OFF_LINE_INFO: OnMsgHstOffLineInfo(Msg);	break;
	case MSG_HST_CONTROL_TRAP_INFO:	   OnMsgHstControlTrapInfo(Msg);	break;
	case MSG_HST_NPC_INVISIBLE:		   OnMsgHstNpcInvisible(Msg);       break;
	case MSG_HST_PHASE_INFO:		   OnMsgHstPhaseInfo(Msg);			break;
	case MSG_HST_PLAYER_BE_TAUNTED2:   OnMsgHstBeTaunted2(Msg);			break;
	case MSG_PM_FASHION_COLORANT:OnMsgFashionColor(Msg);  break;
	case MSG_PM_FAC_BUILDING:			OnMsgFacBuild(Msg); break;
	case MSG_HST_INVITE_ACTIVE_EMOTE:	OnMsgHstInviteActiveEmote(Msg);	break;
	case MSG_HST_REPLY_ACTIVE_EMOTE:	OnMsgHstReplyActiveEmote(Msg);	break;
	case MSG_PM_PLAY_ACTIVE_EMOTE:		OnMsgPlayerPlayActiveEmote(Msg);		break;
	case MSG_PM_STOP_ACTIVE_EMOTE:		OnMsgPlayerStopActiveEmote(Msg);		break;
	case MSG_HST_BE_CHARGE_PRE:			OnMsgBePlayerChargePre(Msg);    break;
	case MSG_PM_TRAVEL_FREE:			OnMsgTravelFree(Msg);           break;
	}
	  
	return true;
}

//	Handle message MSG_GST_MOVE
void CECHostPlayer::OnMsgGstMove(const ECMSG& Msg)
{
	A3DVECTOR3 vDir = m_CameraCoord.GetDir();
	A3DVECTOR3 vRight = m_CameraCoord.GetRight();
	A3DVECTOR3 vMoveDir;

	switch (Msg.dwParam1)
	{
	case 0:	vMoveDir = vDir;	break;
	case 1:	vMoveDir = a3d_Normalize(vDir-vRight);	break;
	case 2:	vMoveDir = -vRight;	break;
	case 3:	vMoveDir = a3d_Normalize(-vDir-vRight);	break;
	case 4:	vMoveDir = -vDir;	break;
	case 5:	vMoveDir = a3d_Normalize(-vDir+vRight);	break;
	case 6:	vMoveDir = vRight;	break;
	case 7:	vMoveDir = a3d_Normalize(vDir+vRight);	break;
	}
	
	float fDist = 5.0f * g_pGame->GetTickTime() * 0.001f;
	vMoveDir *= fDist;
	m_CameraCoord.Move(vMoveDir);
}

//	Handle message MSG_GST_PITCH
void CECHostPlayer::OnMsgGstPitch(const ECMSG& Msg)
{
	CameraPitch(FIX16TOFLOAT((int)Msg.dwParam1));
}

//	Handle message MSG_GST_YAW
void CECHostPlayer::OnMsgGstYaw(const ECMSG& Msg)
{
	CameraYaw(FIX16TOFLOAT((int)Msg.dwParam1));
}

//	Handle message MSG_GST_MOVEABSUP
void CECHostPlayer::OnMsgGstMoveAbsUp(const ECMSG& Msg)
{
	float fDist = 5.0f * g_pGame->GetTickTime() * 0.001f;
	if (!Msg.dwParam1)
		fDist = -fDist;

	m_CameraCoord.Move(g_vAxisY * fDist);
}

void CECHostPlayer::OnMsgHstPushMove(const ECMSG& Msg)
{
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return;
	}

	if (!CanDo(CANDO_MOVETO)) 
	{
		if(m_dwLIES & LIES_STUN)	// 处于冰壳状态，可以通过按W\A\S\D移动健来结束冰壳效果
		{
			g_pGame->GetGameSession()->c2s_CmdCancelIceCrust();
		}
		else
			return;
	}

	switch (Msg.dwParam1)
	{
	case 0:	m_dwMoveRelDir |= MD_FORWARD;				break;
	case 1:	m_dwMoveRelDir |= MD_FORWARD | MD_LEFT;		break;
	case 2:	m_dwMoveRelDir |= MD_LEFT;					break;
	case 3:	m_dwMoveRelDir |= MD_BACK | MD_LEFT;		break;
	case 4:	m_dwMoveRelDir |= MD_BACK;					break;
	case 5:	m_dwMoveRelDir |= MD_BACK | MD_RIGHT;		break;
	case 6:	m_dwMoveRelDir |= MD_RIGHT;					break;
	case 7:	m_dwMoveRelDir |= MD_FORWARD | MD_RIGHT;	break;
	case 8:	m_dwMoveRelDir |= MD_ABSUP;					break;
	case 9:	m_dwMoveRelDir |= MD_ABSDOWN;				break;
	}

	g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelp(CECHelpGraphGuide::HELP2012_1_1_WASD_MOVE);

	bool bPushMove = true;
	if (Msg.dwParam1 == 8 || Msg.dwParam1 == 9)
	{
		if (m_iMoveEnv != MOVEENV_AIR && m_iMoveEnv != MOVEENV_WATER)
			bPushMove = false;
	}

	if (bPushMove && !IsAboutToDie() && CanDo(CANDO_MOVETO))
	{
		bool bRecord = false;
		CECHPWork* pWork = m_pWorkMan->GetCurWork();

		if (!pWork || (pWork->GetWorkID() != CECHPWork::WORK_MOVETOPOS && pWork->CanTransferTo(CECHPWork::MASK_MOVETOPOS)))
		{
			if ( g_pGame->GetGameRun()->IsIn6v6Battle() )
			{
				CECHPWorkSpecialMove* pNewWork = (CECHPWorkSpecialMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_SPECIAL_MOVE);
				if ( pNewWork )
				{
					//pNewWork->
					m_pWorkMan->StartWork_p1(pNewWork);
				}
			}
			else
			{
				CECHPWorkMove* pNewWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVETOPOS);
				pNewWork->SetDestination(CECHPWorkMove::DEST_PUSH, g_vOrigin);
				m_pWorkMan->StartWork_p1(pNewWork);
			}
			bRecord = true;
		}
		else if (m_dwMoveRelDir != m_dwOldDir && pWork->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
			bRecord = true;

		if (bRecord && gr.get_state() == game_record::enum_gr_record)
		{
			frame_record* fr = gr.get_cur_frame();

			if (fr)
				fr->push_event(new event_record_msg(Msg));
		}
	}
}

void CECHostPlayer::OnMsgHstPitch(const ECMSG& Msg)
{
	//	Turn camera
	CECCamera::ACTION_DATA ActData;
	memset(&ActData, 0, sizeof(CECCamera::ACTION_DATA));
	ActData.nDeltaX		= 0;
	ActData.nDeltaY		= -(int)Msg.dwParam1;
	ActData.nDeltaZ		= 0;
	ActData.vecDirChar	= GetDir();
	m_CameraCtrl.Action(&ActData);

	// we need to notify help system that the user has turned the camera, to keep the check state uptodate.
	CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
	if( pScriptMan )
	{
		pScriptMan->GetContext()->GetCheckState()->SetPlayerStatusIsAdjustOrient(true);
	}
	LuaEvent::PlayerIsAdjustOrient();
}

void CECHostPlayer::OnMsgHstYaw(const ECMSG& Msg)
{
	if (Msg.dwParam2)
	{
		CECConfigs* pConfig = g_pGame->GetConfigs();

		//	Turn player
		float fYaw = -(int)Msg.dwParam1 * pConfig->GetGameSettings().fCamTurnSpeed / 80.0f;

		A3DVECTOR3 vPos = GetPos();
		SetPos(g_vOrigin);
		RotateAxis(g_vAxisY, DEG2RAD(fYaw), false);
		SetPos(vPos);

		SetDirAndUp(GetDir(), GetUp());
	}
	else
	{
		//	Turn camera
		CECCamera::ACTION_DATA ActData;
		memset(&ActData, 0, sizeof (CECCamera::ACTION_DATA));
		ActData.nDeltaX		= -(int)Msg.dwParam1;
		ActData.nDeltaY		= 0;
		ActData.nDeltaZ		= 0;
		ActData.vecDirChar	= GetDir();
		m_CameraCtrl.Action(&ActData);
	}

	// we need to notify help system that the user has turned the camera, to keep the check state uptodate.
	CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
	if( pScriptMan )
	{
		pScriptMan->GetContext()->GetCheckState()->SetPlayerStatusIsAdjustOrient(true);
	}
	LuaEvent::PlayerIsAdjustOrient();
}

//	Message MSG_HST_WHEELCAM handler
void CECHostPlayer::OnMsgHstWheelCam(const ECMSG& Msg)
{
	CECCamera::ACTION_DATA ActData;
	memset(&ActData, 0, sizeof(CECCamera::ACTION_DATA));
	ActData.nDeltaX		= 0;
	ActData.nDeltaY		= 0;
	ActData.nDeltaZ		= (int)Msg.dwParam1;
	ActData.vecDirChar	= GetDir();
	m_CameraCtrl.Action(&ActData);

	// we need to notify help system that the user has zoom in or out of the camera, to keep the check state uptodate.
	CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
	if( pScriptMan )
	{
		pScriptMan->GetContext()->GetCheckState()->SetPlayerStatusIsAdjustDistance(true);
	}
//	LuaEvent::PlayerIsAdjustDistance();
}

void CECHostPlayer::OnMsgHstCamDefault(const ECMSG& Msg)
{
	CECCamera::ACTION_DATA ActData;
	memset(&ActData, 0, sizeof(CECCamera::ACTION_DATA));
	ActData.nAction = CECCamera::ACTION_DEFAULT;
	ActData.vecDirChar = GetDir();
	m_CameraCtrl.Action(&ActData);
}

void CECHostPlayer::OnMsgHstCamPreset(const ECMSG& Msg)
{
	CECCamera::ACTION_DATA ActData;
	memset(&ActData, 0, sizeof(CECCamera::ACTION_DATA));
	ActData.nAction = CECCamera::ACTION_PRESET;
	m_CameraCtrl.Action(&ActData);
}

void CECHostPlayer::OnMsgHstCamUserGet(const ECMSG& Msg)
{
//	m_CameraCtrl.UserView_Retrieve(Msg.dwParam1);
}

void CECHostPlayer::OnMsgHstCamUserSet(const ECMSG& Msg)
{
//	m_CameraCtrl.UserView_Define(Msg.dwParam1);
}

//	Message MSG_HST_ATKRESULT handler
void CECHostPlayer::OnMsgHstAttackResult(const ECMSG& Msg)
{
	ASSERT(FALSE);
}

//	Message MSG_HST_ATTACKED handler
void CECHostPlayer::OnMsgHstAttacked(const ECMSG& Msg)
{
}

//	Message MSG_HST_SKILL_ATTACK_RESULT handler
void CECHostPlayer::OnMsgHstSkillResult(const ECMSG& Msg)
{
}

//	Message MSG_HST_SKILL_ATTACKED handler
void CECHostPlayer::OnMsgHstSkillAttacked(const ECMSG& Msg)
{
}

void CECHostPlayer::OnMsgHstAskToJoinTeam(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_team_ask_join* pCmd = (cmd_team_ask_join*)Msg.dwParam1;
	ASSERT(pCmd);

	if (IsAutoTeam(pCmd->idAsker, pCmd->mafiaid, pCmd->familyid))
	{
		g_pGame->GetGameSession()->c2s_CmdTeamReplyJoinAsk(pCmd->idAsker, true);
	}
	else
	{
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ACString strMsg;
		strMsg.Format(pGameUI->GetStringFromTable(686), g_pGame->GetGameRun()->GetPlayerName(pCmd->idAsker, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_AskToJoin", 
			strMsg, 30000, pCmd->idAsker, 0, 0);
	}
}

//	Message MSG_HST_DIED handler
void CECHostPlayer::OnMsgHstDied(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_host_died* pCmd = (cmd_host_died*)Msg.dwParam1;
	m_iReviveCountDown = pCmd->revive_countdown;
	if( !m_bAboutToDie )
		Killed(pCmd->idKiller);
}

//	Message MSG_HST_PICKUPMONEY handler
void CECHostPlayer::OnMsgHstPickupMoney(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_pickup_money* pCmd = (cmd_pickup_money*)Msg.dwParam1;
	ASSERT(pCmd);

	//	Add money amount
	AddMoneyAmount(pCmd->iAmount);

	ACHAR szPrice[256];
	CECIvtrItem::GetPriceText(szPrice, pCmd->iAmount);
	ACString strMsg;
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	strMsg.Format(pStrTab->GetWideString(FIXMSG_PICKUPMONEY), szPrice);
	pGameUI->AddChatMessage(strMsg, CLIENT_CHAT_MONEY);

	//	Popup notify bubble text
	BubbleText(BUBBLE_MONEY, pCmd->iAmount);
}

//	Message MSG_HST_PICKUPITEM handler
void CECHostPlayer::OnMsgHstPickupItem(const ECMSG& Msg)
{
	using namespace S2C;

	int idItem, iExpireDate=0, iAmount, iCmdLastSlot, iCmdSlotAmount, iPack, iMsg=-1;
	int state = 0;//物品的状态，目前仅用于发放的任务物品 Added 2011-05-12.

	switch (Msg.dwParam2)
	{
	case PICKUP_ITEM:
	{
		cmd_pickup_item* pCmd = (cmd_pickup_item*)Msg.dwParam1;
		ASSERT(pCmd);

		idItem			= pCmd->tid;
		iExpireDate		= pCmd->expire_date;
		iAmount			= pCmd->wAmount;
		iCmdLastSlot	= pCmd->bySlot;
		iCmdSlotAmount	= pCmd->wSlotAmount;
		iPack			= pCmd->byPackage;
		state			= pCmd->state;

		iMsg			= FIXMSG_PICKUPITEM;
		
		LuaEvent::FirstPickUpItem();
		break;
	}
	case HOST_OBTAIN_ITEM:
	{
		cmd_host_obtain_item* pCmd = (cmd_host_obtain_item*)Msg.dwParam1;
		ASSERT(pCmd);

		idItem			= pCmd->type;
		iExpireDate		= pCmd->expire_date;
		iAmount			= pCmd->amount;
		iCmdLastSlot	= pCmd->index;
		iCmdSlotAmount	= pCmd->slot_amount;
		iPack			= pCmd->where;
		state			= pCmd->state;

		iMsg			= FIXMSG_GAINITEM;
		break;
	}
	case PRODUCE_ONCE:
	{
		cmd_produce_once* pCmd = (cmd_produce_once*)Msg.dwParam1;
		ASSERT(pCmd);

		idItem			= pCmd->type;
		iExpireDate		= 0;
		iAmount			= pCmd->amount;
		iCmdLastSlot	= pCmd->index;
		iCmdSlotAmount	= pCmd->slot_amount;
		iPack			= pCmd->where;
		state			= pCmd->state;
		iMsg			= FIXMSG_PRODUCEITEM;

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_PRODUCE_END_ONE, pCmd);
		break;
	}
	case TASK_DELIVER_ITEM:
	{
		cmd_task_deliver_item* pCmd = (cmd_task_deliver_item*)Msg.dwParam1;
		ASSERT(pCmd);

		idItem			= pCmd->type;
		iExpireDate		= pCmd->expire_date;
		iAmount			= pCmd->amount;
		iCmdLastSlot	= pCmd->index;
		iCmdSlotAmount	= pCmd->slot_amount;
		iPack			= pCmd->where;
		iMsg			= FIXMSG_GAINITEM;
		state			= pCmd->state;	//Added 2011-05-12.
		
//		if(iPack == 2)	// task package, show message in middle of the screen
		{
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

			CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, iExpireDate, iAmount);
			if(!pItem)
			{
				ASSERT(0);
				return;
			}

			bool bDisplayCnt = ((PAUICHECKBOX)pGameUI->GetDialog("Win_Quest")->GetDlgItem("Chk_QuestNum"))->IsChecked();
			if(bDisplayCnt)
			{
				//  任务物品显示中间提示文字
				Task_State_info tsi;
				CECTaskInterface *pTask = GetTaskInterface();
				ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();				

				for(int i = 0; i < (int)pTask->GetTaskCount(); i++ )
				{
					int idTask = pTask->GetTaskId(i);

					ATaskTempl *pTopTask = pMan->GetTaskTemplByID(idTask);
					pTask->GetTaskStateInfo(idTask, &tsi);
				
					int j;
					// 物品
					for (j = 0; j < MAX_ITEM_WANTED; j++)
					{
						if(tsi.m_ItemsWanted[j].m_ulItemId != (unsigned long)idItem)
							continue;

						ACString strText;
						strText.Format(_AL("%s: %s(%d/%d)"), pTopTask->GetName(), pItem->GetName(), iCmdSlotAmount, tsi.m_ItemsWanted[j].m_ulItemsToGet);
						
						CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)pGameUI->GetDialog("Win_Pkmsg");
						if(pDlgPKMsg)
							pDlgPKMsg->AddMessage(strText);
					}

					// sub task
					int nPos = pTask->GetFirstSubTaskPosition(idTask);
					while( nPos > -1 )
					{
						unsigned long id = pTask->GetNextSub(nPos);
						ATaskTempl *pTemp = pMan->GetTaskTemplByID(id);
						pTask->GetTaskStateInfo(id, &tsi);
					
						// 物品
						for (j = 0; j < MAX_ITEM_WANTED; j++)
						{
							if(tsi.m_ItemsWanted[j].m_ulItemId != (unsigned long)idItem)
								continue;

							ACString strText;
							strText.Format(_AL("%s: %s(%d/%d)"), pTemp->GetName(), pItem->GetName(), iCmdSlotAmount, tsi.m_ItemsWanted[j].m_ulItemsToGet);
							
							CDlgPKMsg *pDlgPKMsg = (CDlgPKMsg*)pGameUI->GetDialog("Win_Pkmsg");
							if(pDlgPKMsg)
								pDlgPKMsg->AddMessage(strText);
						}
					}
				}	
			}
			if(pItem)
				delete pItem;
		}

		break;
	}
	default:
	{
		ASSERT(0);
		return;
	}
	}

	CECInventory* pInventory = GetPack(iPack);
	if (!pInventory)
	{
		ASSERT(0);
		return;
	}
	
	int iLastSlot, iSlotNum;
	if (!pInventory->MergeItem(idItem, state, iExpireDate, iAmount, &iLastSlot, &iSlotNum) ||
		iLastSlot != iCmdLastSlot || iSlotNum != iCmdSlotAmount)
	{
		//	TODO: !! Local inventory data lose synchronization with server,
		//	so we have to re-get all inventory data from server.
		ASSERT(0);
		return;
	}

	//  自动出售生产出来装备
	if(Msg.dwParam2 == PRODUCE_ONCE && iPack == IVTRTYPE_PACK)
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgProduce *pDlg = pGameUI->m_pDlgProduce;
		CECIvtrItem *pItem = pInventory->GetItem(iLastSlot);

		if (pDlg && pDlg->GetAutoSellOpt() && pItem && pItem->IsSellable() && !pItem->IsLocked() && 
			!g_pGame->IsValuableItem(pItem->GetTemplateID()) && pItem->IsEquipment())
		{

			INT64 nMoney = GetMoneyAmount();
			
			int nIvtrSize = pInventory->GetSize();
			for(int i = 0; i < nIvtrSize; i++)
			{
				CECIvtrItem *pEquipItem = pInventory->GetItem(i);
				if(pEquipItem && pEquipItem->GetTemplateID() == pItem->GetTemplateID() && pEquipItem->IsSellable() && !pEquipItem->IsLocked())
				{
					int nLevel = ((CECIvtrEquip*)pEquipItem)->GetEssence().num_estone;
					if(nMoney + pEquipItem->GetScaledPrice() > 2000000000)
					{
						g_pGame->GetGameRun()->AddChatMessage(pGameUI->GetStringFromTable(8485), GP_CHAT_MISC);
						break;
					}
					else if(nLevel > 0)
					{
//						g_pGame->GetGameRun()->AddChatMessage(pGameUI->GetStringFromTable(17750), GP_CHAT_MISC);
					}
					else
					{
						pEquipItem->Freeze(true);
						m_pSellPack->RemoveAllItems();
						m_pSellPack->AddItem(pEquipItem, i, pEquipItem->GetCount());
						SellItemsToNPC();
					}

				}
			}		
		}
	}


	//NewHelpGuide::TriggerGetItem();
	//	If item is equipment we have to get its detail information, 
	//	otherwise it may be shown as "unable to be equipped"
	CECIvtrItem* pItem = pInventory->GetItem(iCmdLastSlot);
	ASSERT(pItem);
	LuaEvent::FirstObtainSpecialItem(idItem);

	if (pItem)
	{
		//如果是任务物品，则将服务器最新的物品状态同步到服务器Added 2011-05-12.
		if ( TASK_DELIVER_ITEM == Msg.dwParam2 )
		{
			pItem->SetStateFlag( state );
			pItem->UpdateInfoMD5();//更新状态后，在release下一定要更新md5信息 Added 2011-05-13
		}
		//Added end.
		if (pItem->IsEquipment() || pItem->NeedUpdate())
			g_pGame->GetGameSession()->c2s_CmdGetItemInfo(iPack, iCmdLastSlot);
		
		if (pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
			LuaEvent::FirstObtainTalisman(); 
		if (pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			LuaEvent::FirstObtainPet();
			LuaEvent::FirstObtainSpecialPet(idItem);
		}
		if (pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR)
		{
			LuaEvent::FirstObtainPetEquip();
		}
		//	Print a notice
		if (iMsg >= 0)
		{
			CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			ACString strMsg;
			strMsg.Format(pStrTab->GetWideString(iMsg), iAmount, pItem->GetName());
			pGameUI->AddChatMessage(strMsg, CLIENT_CHAT_PICKUP);
		}
	}
}

//	Message MSG_HST_RECEIVEEXP handler
void CECHostPlayer::OnMsgHstReceiveExp(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_receive_exp* pCmd = (cmd_receive_exp*)Msg.dwParam1;
	ASSERT(pCmd);

	m_BasicProps.exp += pCmd->exp;

	if (pCmd->exp)
	{
		//	Print notify text
	//	g_pGame->GetGameRun()->AddFixedMessage(pCmd->exp > 0 ? FIXMSG_GOTEXP : FIXMSG_LOSTEXP, (int)pCmd->exp);
		BubbleText(BUBBLE_EXP, pCmd->exp);
	}
}

//	Message MSG_HST_Info00 handler
void CECHostPlayer::OnMsgHstInfo00(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_self_info_00* pCmd = (cmd_self_info_00*)Msg.dwParam1;
	ASSERT(pCmd);

	bool bFirstTime = !m_BasicProps.iLevel ? true : false;
	if (!bFirstTime)
	{
		int iLimit = (int)(pCmd->iMaxHP * 0.3f);
		if (pCmd->iHP < m_BasicProps.iCurHP && m_BasicProps.iCurHP >= iLimit && pCmd->iHP < iLimit)
			BubbleText(BUBBLE_HPWARN, 0);

		iLimit = (int)(pCmd->iMaxMP * 0.2f);
		if (pCmd->iMP < m_BasicProps.iCurMP && m_BasicProps.iCurMP >= iLimit && pCmd->iMP < iLimit)
			BubbleText(BUBBLE_MPWARN, 0);
		
		iLimit = (int)(pCmd->iMaxDP * 0.2f);
		if (pCmd->iDP < m_BasicProps.iCurDP && m_BasicProps.iCurDP >= iLimit && pCmd->iDP < iLimit)
			BubbleText(BUBBLE_DPWARN, 0);
		/*
		if (m_ExtProps.max_ap != pCmd->iMaxAP)
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ADDMAXAP, pCmd->iMaxAP - m_ExtProps.max_ap);
			*/
	}

	m_BasicProps.iLevel		= pCmd->sLevel;
	m_BasicProps.iDeityLevel= pCmd->sDeityLevel;
	m_BasicProps.exp		= pCmd->exp;
	m_BasicProps.deityExp	= pCmd->deityExp;
	m_BasicProps.iCurHP		= pCmd->iHP;
	m_BasicProps.iCurMP		= pCmd->iMP;
	m_BasicProps.iCurDP		= pCmd->iDP;
//	m_BasicProps.ip			= pCmd->ip;
	m_BasicProps.irage		= pCmd->irage;
	m_BasicProps.iTalismanStamina = pCmd->iTalismanStamina;
	m_ExtProps.max_hp		= pCmd->iMaxHP;
	m_ExtProps.max_mp		= pCmd->iMaxMP;
	m_ExtProps.max_dp		= pCmd->iMaxDP;
	// m_ExtProps.max_ap		= pCmd->iMaxAP;
	if (GetCloneID())
	{
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(GetCloneID());
		if (pPlayer)
		{
			pPlayer->GetBasicProps().iCurMP = pCmd->iMP;
			pPlayer->GetBasicProps().iCurDP = pCmd->iDP;
			pPlayer->GetExtendProps().max_mp = pCmd->iMaxMP;
			pPlayer->GetExtendProps().max_dp = pCmd->iMaxDP;
		}
	}
	
	if(m_iPuppetId && !m_bFight && m_PuppetStateCount.IsFull() && pCmd->State)
	{
		g_pGame->GetGameSession()->c2s_CmdPuppetFormChange();
		m_PuppetStateCount.Reset(false);
	}

	m_bFight = pCmd->State ? true : false;
}

//	Message MSG_HST_GOTO handler
void CECHostPlayer::OnMsgHstGoto(const ECMSG& Msg)
{
	using namespace S2C;
	
	//普通的跳地图和副本的调地图使用了不同的消息协议，但处理流程还统一在这个函数中 Modified 2011-07-29.
	A3DVECTOR3 vPos;
	int iMapId = 0;

	//现在以ECMSG的第三个参数dwParam3的值区别协议cmd_notify_hostpos和cmd_notify_raid_pos
	if ( NOTIFY_HOSTPOS == Msg.dwParam2 ) //普通协议
	{
		cmd_notify_hostpos* pCmd = (cmd_notify_hostpos*)Msg.dwParam1;
		ASSERT(pCmd);
		vPos = pCmd->vPos;
		iMapId = pCmd->tag;
	}
	else if ( OBJECT_NOTIFY_RAID_POS == Msg.dwParam2 ) //副本相关协议
	{
		cmd_notify_raid_pos* pCmd = (cmd_notify_raid_pos*)Msg.dwParam1;
		ASSERT(pCmd);
		vPos = pCmd->pos;
		iMapId = pCmd->raid_map_id;
		//pCmd->tag ：副本实例ID，暂时不用
		//---------------------------------------
		m_bSpecialMoving = false;//强制设置为false
		m_vVelocity.Clear();
		m_vAccel.Clear();
#ifdef CDR_HL2_MOVE

		if ( m_pCDRInfo )
		{
			m_pCDRInfo->vClipVel.Clear();
		}
#endif
		//---------------------------------------
	}
	
	//如果处于多人骑乘状态，先将成员解除绑定
	if(m_bMultiRideLeader)
	{
		for(int i=1; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if(m_iMultiBuddyId[i])
				DetachMultiBuddy(i);
		}
	}
	if(m_bMultiRideMember)
	{
		CECPlayer* pLeader = m_pPlayerMan->GetPlayer(m_iLeaderId);
		if(pLeader)
		{
			pLeader->RemoveMultiRideMember(m_PlayerInfo.cid);
		}
 	}

//	A3DVECTOR3 vPos = pCmd->vPos; //Canceled 2011-07-29.
	if (!g_pGame->GetGameRun()->JumpToInstance(iMapId, vPos)) //pCmd->tag Modified 2011-07-29.
	{
		a_LogOutput(1, "CECGameRun::OnMsgHstGoto, Failed to jump to instance %d", iMapId);//pCmd->tag
		return;
	}

	// 清理玩家的载体状态
	if (m_idCarrier)
	{		
		g_pGame->GetGameSession()->c2s_CmdLeaveCarrier(m_idCarrier, vPos, glb_CompressDirH(GetDir().x, GetDir().z));//pCmd->vPos
		m_idCarrier = 0;
		SetCarrierCooling(true);
		OutputDebugStringA("Send Leave Carrier!!\n");
	}

	//	Stop all current work and goto specified position
	m_pWorkMan->FinishAllWork(true);
	
	//	In some instances flying is forbidden, server should has notified host to fall
	//	down before he enter these instances (before GOTO message is sent). 
	//	The problem is that wing or flysword won't disappear until host touch
	//	ground, so when host fly into these	'no-flying' instances, he may stand on
	//	ground still has wing or flysword !
	if (!IsFlying())
		ShowWing(false);

	//	Add a little height to ensure player's AABB won't embed with building
	vPos += g_vAxisY * 0.1f;

	//	Ensure we are not under ground
	A3DVECTOR3 vNormal;
	float vTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrainHeight(vPos, &vNormal);
	if (vPos.y < vTerrainHeight)
		vPos.y = vTerrainHeight;

	// 强制设置为服务器位置
	int _state = gr.get_state();
	gr.set_state(game_record::enum_gr_disable);
	SetPos(vPos);
	gr.set_state(_state);
	if (gr.get_state() == game_record::enum_gr_replay)
	{
		gr.reset_host_pos(vPos);
	}
	if (IsDead())
	{
		CECHPWorkDead* pWork = (CECHPWorkDead*)m_pWorkMan->CreateWork(CECHPWork::WORK_DEAD);
		pWork->SetBeDeadFlag(true);
		m_pWorkMan->StartWork_p0(pWork);
	}
//	m_pCDRInfo->vTPNormal	= vPos.y <= vTerrainHeight + 0.1f ? vNormal : g_vOrigin;
#ifdef CDR_HL2_MOVE
	m_pCDRInfo->vClipVel.Clear();
#else
	m_pCDRInfo->fYVel		= 0.0f;
#endif	
	m_pCDRInfo->vAbsVelocity.Clear();

	// 6v6地图特殊处理
	if (g_pGame->GetGameRun()->GetWorld()->GetInstanceID()==535)
	{
		vPos -= g_vAxisY * 0.1f;
		float fHei = g_pGame->GetGameRun()->GetWorld()->GetGroundHeight(vPos, &vNormal);
		//  高度差很小，认为人物在地面上，不用播放掉落的动作
		if (fabs(fHei - vPos.y) < 0.15f)
			m_GndInfo.bOnGround = true;
		else
			m_GndInfo.bOnGround = false;
	}

	ResetCDRInfo();
	ResetJump();

	// 6v6地图特殊处理
	if (g_pGame->GetGameRun()->GetWorld()->GetInstanceID()==535)
	{
		if (m_GndInfo.bOnGround)
		{
			m_pCDRInfo->bOnSurface = true;
			m_pCDRInfo->bCanStay = true;
		}
	}

	m_MoveCtrl.SetHostLastPos(vPos);
	m_MoveCtrl.SetLastSevPos(vPos);

	//	Update camera
	UpdateFollowCamera(false, 10);

	//	Notify our position to other teammates if we are in a team
	if (m_pTeam)
		g_pGame->GetGameSession()->c2s_CmdTeamNotifyOwnPos();

	// now clear exit instance count down flags
	// modified 2011/3/15 by fuchonggang
	if(m_iBattleCamp==GP_BATTLE_CAMP_NONE)
	{
		CECGameUIMan * pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->PopupInstanceCountDown(0);
		m_nTimeToExitInstance = 0;
	}
}

//	Message MSG_HST_OWNITEMINFO handler
void CECHostPlayer::OnMsgHstOwnItemInfo(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == OWN_ITEM_INFO)
	{
		cmd_own_item_info cmd;

		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
			return;

		CECInventory* pInventory = GetPack(cmd.byPackage);
		CECIvtrItem* pItem = NULL;

		if (!pInventory)
		{
			ASSERT(0);
			return;
		}

		pItem = pInventory->GetItem(cmd.bySlot);

		if (!pItem)
		{
			pItem = CECIvtrItem::CreateItem(cmd.type, cmd.expire_date, cmd.count);

			if (!pItem)
			{
				ASSERT(0);
				return;
			}

			pInventory->PutItem(cmd.bySlot, pItem);
		}
		else
		{
			pItem->SetExpireDate(cmd.expire_date);
			pItem->SetAmount(cmd.count);
		}

		pItem->SetStateFlag(cmd.state);

		if (cmd.content && cmd.content_length)
			pItem->SetItemInfo((BYTE*)cmd.content, cmd.content_length);
		else
			pItem->SetEmptyInfo();

		pItem->UpdateInfoMD5();
	}
	else if (Msg.dwParam2 == EMPTY_ITEM_SLOT)
	{
		cmd_empty_item_slot* pCmd = (cmd_empty_item_slot*)Msg.dwParam1;
		ASSERT(pCmd);

		CECInventory* pInventory = GetPack(pCmd->byPackage);
		if (pInventory)
		{
			//	Update shortcuts
			CECIvtrItem* pItem = pInventory->GetItem(pCmd->bySlot);
			if (pItem)
				UpdateRemovedItemSC(pItem->GetTemplateID(), pCmd->byPackage, pCmd->bySlot);

			pInventory->SetItem(pCmd->bySlot, NULL);
		}
	}
	else if (Msg.dwParam2 == EQUIP_SPIRIT_POWER_DECREASE)   //更新装备的当前魂力值
	{
		cmd_spirit_power_decrease* pCmd = (cmd_spirit_power_decrease*)Msg.dwParam1;
		ASSERT(pCmd);
		CECInventory* pInventory = GetEquipment();
		if (pInventory)
		{
			CECIvtrEquip* pItem = (CECIvtrEquip*)pInventory->GetItem(pCmd->index);
			if(pItem)
			{
				pItem->UpdateSpritePower(pCmd->dec_amount, pCmd->result);
				pItem->UpdateInfoMD5();
			}
		}
	}
	else if (Msg.dwParam2 == MAGIC_DURATION_DECREASE)
	{
		magic_duration_decrease* pCmd = (magic_duration_decrease*)Msg.dwParam1;
		ASSERT(pCmd);
		CECInventory* pInventory = GetEquipment();
		if (pInventory)
		{
			CECIvtrEquip* pItem = (CECIvtrEquip*)pInventory->GetItem(pCmd->index);
			if (pItem && pItem->IsChangeShapeCard())
			{
				CECIvtrChangeShapeCard* pCard = (CECIvtrChangeShapeCard*)pItem;
				pCard->UpdateDuration(pCmd->dec_amount, pCmd->result);
			}
		}
	}
	else if (Msg.dwParam2 == MAGIC_EXP_NOTIFY)
	{
		magic_exp_notify* pCmd = (magic_exp_notify*)Msg.dwParam1;
		ASSERT(pCmd);
		CECInventory* pInventory = GetPack(pCmd->where);
		if (pInventory)
		{
			CECIvtrEquip* pItem = (CECIvtrEquip*)pInventory->GetItem(pCmd->index);
			if (pItem && pItem->IsChangeShapeCard())
			{
				CECIvtrChangeShapeCard* pCard = (CECIvtrChangeShapeCard*)pItem;
				pCard->UpdataExp(pCmd->new_exp);
			}
		}
	}
}

//	Message MSG_HST_SELTARGET handler
void CECHostPlayer::OnMsgHstSelTarget(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == SELECT_TARGET)
	{
		cmd_select_target* pCmd = (cmd_select_target*)Msg.dwParam1;
		ASSERT(pCmd);

		m_idSelTarget	= pCmd->idTarget;
		m_idUCSelTarget	= 0;
		if (ISNPCID(m_idSelTarget))
		{
			// 无法被选中的怪，注意这里服务器和客户端的选中判断将不一致。。
			CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(pCmd->idTarget);
			if (pNPC && pNPC->GetSelectStrategy() == 2) 
				m_idSelTarget = 0;
			LuaEvent::FirstClickSpecialMonster(m_idSelTarget);
		}
		//	Test code ...
		g_pGame->RuntimeDebugInfo(0xffffff00, _AL("Select %x"), m_idSelTarget);
	}
	else if (Msg.dwParam2 == UNSELECT)
	{
		m_idSelTarget = 0;
	}
    
	//更新泡澡邀请界面
	{
	   CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	   pGameUI->UpdateKingBathState();
	}

}

//	Message MSG_HST_FIXCAMERA handler
void CECHostPlayer::OnMsgHstFixCamera(const ECMSG& Msg)
{
//	m_CameraCtrl.Fix_Set(!m_CameraCtrl.Fix_Get());
}

void CECHostPlayer::OnMsgHstExtProp(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_own_ext_prop* pCmd = (cmd_own_ext_prop*)Msg.dwParam1;
	ASSERT(pCmd);

	m_ExtProps = pCmd->prop;
	m_BasicProps.iStatusPt = (int)pCmd->status_point;
}

void CECHostPlayer::OnMsgHstAddStatusPt(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_add_status_point* pCmd = (cmd_add_status_point*)Msg.dwParam1;
	ASSERT(pCmd);

	m_BasicProps.iStatusPt	= (int)pCmd->remain;
	/*
	m_ExtProps.bs.vitality += (int)pCmd->vitality;
	m_ExtProps.bs.energy   += (int)pCmd->energy;
	m_ExtProps.bs.strength += (int)pCmd->strength;
	m_ExtProps.bs.agility  += (int)pCmd->agility;
	*/

	//	Get extend properties
	g_pGame->GetGameSession()->c2s_CmdGetExtProps();
}

void CECHostPlayer::OnMsgHstJoinTeam(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_join_team* pCmd = (cmd_team_join_team*)Msg.dwParam1;
	ASSERT(pCmd);

	CECTeamMan* pTeamMan = g_pGame->GetGameRun()->GetTeamMan();

	//	Try to get team at first
	CECTeam* pTeam = pTeamMan->GetTeam(pCmd->idLeader);
	if (!pTeam)
	{
		//	Create a new team
		if (!(pTeam = pTeamMan->CreateTeam(pCmd->idLeader)))
		{
			glb_ErrorOutput(ECERR_FAILEDTOCALL, "CECTeamMan::OnMsgHstJoinTeam", __LINE__);
			return;
		}
	}

	//	If host player is leader, add heself into team
	if (pCmd->idLeader == m_PlayerInfo.cid)
		pTeam->AddMember(pCmd->idLeader);
	
	pTeam->SetPickupFlag(pCmd->wPickFlag);

	m_pTeam = pTeam;
}

void CECHostPlayer::OnMsgHstLeaveTeam(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_leave_party* pCmd = (cmd_team_leave_party*)Msg.dwParam1;
	ASSERT(pCmd);
	
	if (!m_pTeam)
	{
		ASSERT(m_pTeam);
		return;
	}

	if (pCmd->idLeader != m_pTeam->GetLeaderID())
	{
		ASSERT(pCmd->idLeader == m_pTeam->GetLeaderID());
		return;
	}

	//	In current game, we don't care those teams which doesn't include host
	//	player, so if only host leave team, delete the team immediately !
	CECTeamMan* pTeamMan = g_pGame->GetGameRun()->GetTeamMan();
	pTeamMan->ReleaseTeam(pCmd->idLeader);
	m_pTeam = NULL;

	if (pCmd->reason != GP_LTR_LEAVE)
	{
		//	Display the reason
		int iMsg = (pCmd->reason == GP_LTR_KICKEDOUT) ? FIXMSG_KICKEDOUTTEAM : FIXMSG_CANCELTEAM;
		g_pGame->GetGameRun()->AddFixedMessage(iMsg);
	}

	//	Update team UI
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateTeam();
}

void CECHostPlayer::OnMsgHstNewTeamMem(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_new_member* pCmd = (cmd_team_new_member*)Msg.dwParam1;
	ASSERT(pCmd);

	if (!m_pTeam)
	{
		ASSERT(m_pTeam);
		return;
	}

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	int cid = pCmd->idMember;

	//	Print notify message
	m_pTeam->AddMember(cid);
	const ACHAR* szName = pGameRun->GetPlayerName(cid, false);
	if (szName)
		pGameRun->AddFixedMessage(FIXMSG_JOINTEAM, szName);
	else
		m_pTeam->AddUnknownID(cid);

	//	If this member isn't near host, get his/her position
	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(cid);
	if (!pPlayer)
		g_pGame->GetGameSession()->c2s_CmdTeamMemberPos(1, &cid);
	else
	{
		CECTeamMember* pMem = m_pTeam->GetMemberByID(cid);
		if (pMem)
			pMem->SetPos(pGameRun->GetWorld()->GetInstanceID(), pPlayer->GetPos());
	}
	
	//	Update team UI
	CECGameUIMan* pGameUI = pGameRun->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateTeam();
}

void CECHostPlayer::OnMsgHstItemOperation(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case EXG_IVTR_ITEM:
	{
		cmd_exg_ivtr_item* pCmd = (cmd_exg_ivtr_item*)Msg.dwParam1;
		ASSERT(pCmd);


		m_pPack->ExchangeItem(pCmd->index1, pCmd->index2);

		//	Update shortcuts
		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->index1);
		CECIvtrItem* pItem2 = m_pPack->GetItem(pCmd->index2);
		if (pItem1 && pItem2)
			UpdateExchangedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->index2, pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->index1);
		else if (pItem1)
			UpdateMovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->index2, IVTRTYPE_PACK, pCmd->index1);
		else if (pItem2)
			UpdateMovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->index1, IVTRTYPE_PACK, pCmd->index2);

		// Arrange Ivtr Item
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI) {
			CDlgInventory* pDlgIvtr = (CDlgInventory*)pGameUI->GetDialog("Win_Inventory");
			if (pDlgIvtr)
				pDlgIvtr->OnItemExchanged(pCmd->index1, pCmd->index2);
		}
		setRecordMonnt(IVTRTYPE_PACK,pCmd->index1,IVTRTYPE_PACK,pCmd->index2);
		break;
	}
	case MOVE_IVTR_ITEM:
	{
		cmd_move_ivtr_item* pCmd = (cmd_move_ivtr_item*)Msg.dwParam1;
		ASSERT(pCmd);
		if(pCmd->count == 0)
			return;
		m_pPack->MoveItem(pCmd->src, pCmd->dest, pCmd->count);

		//	Update shortcuts
		CECIvtrItem* pItem = m_pPack->GetItem(pCmd->dest);
		if (pItem)
			UpdateMovedItemSC(pItem->GetTemplateID(), IVTRTYPE_PACK, pCmd->src, IVTRTYPE_PACK, pCmd->dest);
		else
			ASSERT(pItem);

		break;
	}
	case PLAYER_DROP_ITEM:
	{
		cmd_player_drop_item* pCmd = (cmd_player_drop_item*)Msg.dwParam1;
		ASSERT(pCmd);
		CECInventory* pPack = GetPack(pCmd->byPackage);
		if (!pPack) break;

		//	Keep item information for updating shortcuts later
		CECIvtrItem* pItem = pPack->GetItem(pCmd->bySlot);
		if (pItem && pItem->GetTemplateID() == pCmd->tid)
		{
			//	Print notify message
			switch (pCmd->reason)
			{
			case GP_DROP_DEATH:	
				
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSEITEM, pCmd->count, pItem->GetName());
				break;

			case GP_DROP_TAKEOUT:
			case GP_DROP_TASK:
			case GP_DROP_USED:
			case GP_DROP_BIND_DISAPPEAR:
			case GP_DROP_DAMAGED:

				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ITEMDISAPPEAR, pCmd->count, pItem->GetName());
				break;

			case GP_DROP_EXPIRED:
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ITEMEXPIRED, pCmd->count, pItem->GetName());
				break;

			}

			//	Remove item from pack
			pPack->RemoveItem(pCmd->bySlot, pCmd->count);

			//	Update shortcuts if all items have been dropped
			if (!pPack->GetItem(pCmd->bySlot))
				UpdateRemovedItemSC(pCmd->tid, pCmd->byPackage, pCmd->bySlot);
			m_pBuyPack->RemoveItem(pCmd->bySlot, pCmd->count);
			m_pSellPack->RemoveItem(pCmd->bySlot, pCmd->count);
			m_pBoothSPack->RemoveItem(pCmd->bySlot, pCmd->count);
			m_pBoothBPack->RemoveItem(pCmd->bySlot, pCmd->count);
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)pGameUI->GetDialog("Win_Inventory_Vechile");
			if(pDlgInvRide) 
			{
				pDlgInvRide->UpdataMountsOrFly();
			}

			if (pCmd->byPackage == IVTRTYPE_FUWEN)
			{
				CDlgRoleRune* pDlgRoleRune = (CDlgRoleRune*)pGameUI->GetDialog("Win_Equip_Rune");
				if (pDlgRoleRune->IsShow())
					pDlgRoleRune->UpdateView();
			}
		}
		else if (pCmd->tid >= 0)
		{
			// just get the name of the item and output the messages here.
			pItem = CECIvtrItem::CreateItem(pCmd->tid, 0, 1);
			//	Print notify message
			switch (pCmd->reason)
			{
			case GP_DROP_DEATH:	
				
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LOSEITEM, pCmd->count, pItem->GetName());
				break;

			case GP_DROP_TAKEOUT:
			case GP_DROP_TASK:
			case GP_DROP_USED:

				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ITEMDISAPPEAR, pCmd->count, pItem->GetName());
				break;

			case GP_DROP_EXPIRED:
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ITEMEXPIRED, pCmd->count, pItem->GetName());
				break;
			}

			delete pItem;
			pItem = NULL;

		}

		if (pCmd->byPackage == IVTRTYPE_EQUIPPACK)
		{
			//	Update player's appearance
			UpdateEquipSkins();

			//	Update player's properties.
			//	TODO: use more cheap updating commands
			g_pGame->GetGameSession()->c2s_CmdGetExtProps();
		}

		break;
	}
	case EXG_EQUIP_ITEM:
	{
		cmd_exg_equip_item* pCmd = (cmd_exg_equip_item*)Msg.dwParam1;
		ASSERT(pCmd);
		m_pEquipPack->ExchangeItem(pCmd->index1, pCmd->index2);

		//	Update shortcuts
		CECIvtrItem* pItem1 = m_pEquipPack->GetItem(pCmd->index1);
		CECIvtrItem* pItem2 = m_pEquipPack->GetItem(pCmd->index2);
		if (pItem1 && pItem2)
			UpdateExchangedItemSC(pItem1->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index2, pItem2->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index1);
		else if (pItem1)
			UpdateMovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index2, IVTRTYPE_EQUIPPACK, pCmd->index1);
		else if (pItem2)
			UpdateMovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index1, IVTRTYPE_EQUIPPACK, pCmd->index2);

		//	Update player's appearance
		
		UpdateEquipSkins();
		CheckRecordMount(IVTRTYPE_EQUIPPACK,EQUIPIVTR_WING,0);
		break;
	}
	case EQUIP_ITEM:
	{
		cmd_equip_item* pCmd = (cmd_equip_item*)Msg.dwParam1;
		ASSERT(pCmd);

		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->index_inv, true);
		CECIvtrItem* pItem2 = m_pEquipPack->PutItem(pCmd->index_equip, pItem1);
		m_pPack->SetItem(pCmd->index_inv, pItem2);

		//	Synchronize item count
		if (!pCmd->count_equip)
		{
			if (pItem1)
				UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->index_inv);

			m_pEquipPack->SetItem(pCmd->index_equip, NULL);
		}
		else if (pItem1)
			pItem1->SetCount(pCmd->count_equip);

		if (!pCmd->count_inv)
		{
			if (pItem2)
				UpdateRemovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index_equip);

			m_pPack->SetItem(pCmd->index_inv, NULL);
		}
		else if (pItem2)
			pItem2->SetCount(pCmd->count_inv);

		pItem1 = m_pPack->GetItem(pCmd->index_inv);
		pItem2 = m_pEquipPack->GetItem(pCmd->index_equip);
		if( pItem2 )
			pItem2->SetNeedUpdate(true);

		if (pItem1 && pItem2)
			UpdateExchangedItemSC(pItem1->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index_equip, pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->index_inv);
		else if (pItem1)
			UpdateMovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_EQUIPPACK, pCmd->index_equip, IVTRTYPE_PACK, pCmd->index_inv);
		else if (pItem2)
			UpdateMovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->index_inv, IVTRTYPE_EQUIPPACK, pCmd->index_equip);

		//	Update player's properties.
		//	TODO: use more cheap updating commands
		g_pGame->GetGameSession()->c2s_CmdGetExtProps();

		//	Update player's appearance
		UpdateEquipSkins();

		LuaEvent::FirstEquipItem();
		if( pCmd->index_equip == EQUIPIVTR_WEAPON && pCmd->count_equip )
		{
			// we need to notify help system that the user has just equipped on a weapon, to keep the check state uptodate.
			CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
			if( pScriptMan )
			{
				pScriptMan->GetContext()->GetCheckState()->SetEquipNewWeapon(true);
			}
		}
		CheckRecordMount(IVTRTYPE_EQUIPPACK,EQUIPIVTR_WING,0);
		break;
	}
	case MOVE_EQUIP_ITEM:
	{
		cmd_move_equip_item* pCmd = (cmd_move_equip_item*)Msg.dwParam1;
		ASSERT(pCmd);

		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->index_inv);
		CECIvtrItem* pItem2 = m_pEquipPack->GetItem(pCmd->index_equip);
		if (pItem2)
		{
			ASSERT(pItem2->GetTemplateID() == pItem1->GetTemplateID());
			pItem2->AddAmount(pCmd->amount);
		}
		else
		{
			pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pCmd->amount);
			m_pEquipPack->SetItem(pCmd->index_equip, pItem2);
		}

		//	Keep item information for updating shortcuts later
		ASSERT(pItem1);
		int tid = pItem1 ? pItem1->GetTemplateID() : 0;

		m_pPack->RemoveItem(pCmd->index_inv, pCmd->amount);

		//	Update shortcuts if all items have been moved
		if (tid && !m_pPack->GetItem(pCmd->index_inv))
			UpdateRemovedItemSC(tid, IVTRTYPE_PACK, pCmd->index_inv);

		//	Update player's properties.
		//	TODO: use more cheap updating commands
		g_pGame->GetGameSession()->c2s_CmdGetExtProps();
		CheckRecordMount(IVTRTYPE_EQUIPPACK,EQUIPIVTR_WING,0);
		break;
	}
	case UNFREEZE_IVTR_SLOT:
	{
		cmd_unfreeze_ivtr_slot* pCmd = (cmd_unfreeze_ivtr_slot*)Msg.dwParam1;
		FreezeItem(pCmd->where, pCmd->index, false, true);
		break;
	}
	case PUT_ITEM:
	{
		put_item* pCmd = (put_item*)Msg.dwParam1;
		CECInventory* pInv = GetPack(pCmd->where);
		CECIvtrItem* pItem = pInv->GetItem(pCmd->index);
		if (!pItem)
		{
			pInv->PutItem(pCmd->index, CECIvtrItem::CreateItem(pCmd->type, pCmd->expire_date, pCmd->amount));
		}
		else if (pItem->GetTemplateID()==pCmd->type && (pItem->GetCount()+pCmd->amount)==pCmd->slot_amount)
		{
			pItem->MergeItem(pCmd->type, pCmd->state, pCmd->amount);
		}
		else
			ASSERT(0);

		//NewHelpGuide::TriggerGetItem();
	}
	}
}

void CECHostPlayer::OnMsgHstTrashBoxOperation(const ECMSG& Msg)
{
	using namespace S2C;
	ASSERT(Msg.dwParam1);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case TRASHBOX_OPEN:
	{
		const cmd_trashbox_open* pCmd = (const cmd_trashbox_open*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
		bool* pbFirstOpen = pCmd->where == IVTRTYPE_TRASHBOX ? &m_bFirstTBOpen : &m_bFirstFTBOpen;

		m_bUsingTrashBox = true;

		if (*pbFirstOpen)
		{
			//	It's the first time to open trash box, get trash box data
			*pbFirstOpen = false;
			g_pGame->GetGameSession()->c2s_CmdGetTrashBoxData(true);
		}
		else
		{
			if (pPack->GetSize() < pCmd->slot_size)
				pPack->Resize(pCmd->slot_size);

			pGameUI->PopupStorageDialog(false, pCmd->where != IVTRTYPE_TRASHBOX);
		}
		LuaEvent::FirstOpenTrash();

		break;
	}
	case TRASHBOX_CLOSE:
	{
		m_bUsingTrashBox = false;
		pGameUI->PopupStorageDialog(true);
		break;
	}
	case TRASHBOX_WEALTH: {
		const cmd_trashbox_wealth* pCmd = (const cmd_trashbox_wealth*)Msg.dwParam1;
		m_iTrashBoxMoneyCnt = pCmd->money;
		break; }
	case EXG_TRASH_MONEY: {
		const cmd_exg_trash_money* pCmd = (cmd_exg_trash_money*)Msg.dwParam1;
		AddMoneyAmount(pCmd->inv_delta);
		m_iTrashBoxMoneyCnt += pCmd->tra_delta;
		break; }
	case EXG_TRASHBOX_ITEM:	{
		const cmd_exg_trashbox_item* pCmd = (const cmd_exg_trashbox_item*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
		pPack->ExchangeItem(pCmd->idx1, pCmd->idx2);

		// Arrange Storage Item
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (pGameUI) {
			CDlgStorage* pDlgStorage = (CDlgStorage*)pGameUI->GetDialog("Win_Storage");
			if (pDlgStorage)
				pDlgStorage->OnItemExchanged(pCmd->idx1, pCmd->idx2);
		}
		break; }
	case MOVE_TRASHBOX_ITEM: {
		const cmd_move_trashbox_item* pCmd = (const cmd_move_trashbox_item*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
		pPack->MoveItem(pCmd->src, pCmd->dest, pCmd->amount);
		break; }
	case EXG_TRASHBOX_IVTR:
	{
		const cmd_exg_trashbox_ivtr* pCmd = (const cmd_exg_trashbox_ivtr*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
 
		
		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->idx_inv, true);
		CECIvtrItem* pItem2 = pPack->GetItem(pCmd->idx_tra, true);


		EC_LOCAL_SETTING setting = g_pGame->GetConfigs()->GetLocalSettings();
		if((IVTRTYPE_PACK == setting.nRecordMountType) && (pCmd->idx_inv == setting.nRecordMountID))
		{
			setting.nRecordMountType = IVTRTYPE_PACK;
			setting.nRecordMountID = -1;
			g_pGame->GetConfigs()->SetLocalSettings(setting);
			CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			CDlgInventoryRide* pDlgInvRide = (CDlgInventoryRide*)pGameUI->GetDialog("Win_Inventory_Vechile");
			if(pDlgInvRide) 
			{
				pDlgInvRide->UpdataMountsOrFly();
			}
		}
		m_pPack->SetItem(pCmd->idx_inv, pItem2);
		pPack->SetItem(pCmd->idx_tra, pItem1);

		//	Update shortcuts
		if (pItem1)
			UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->idx_inv);

		break; 
	}
	case IVTR_ITEM_TO_TRASH: {
		const cmd_ivty_item_to_trash* pCmd = (const cmd_ivty_item_to_trash*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->src);
		CECIvtrItem* pItem2 = pPack->GetItem(pCmd->dest);
		if (!pItem1)
		{
			ASSERT(pItem1);
			break;
		}

		if (pItem2)
		{
			ASSERT(pItem1->GetTemplateID() == pItem2->GetTemplateID());
			pItem2->AddAmount(pCmd->amount);
		}
		else
		{
			pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pCmd->amount);
			pPack->SetItem(pCmd->dest, pItem2);
		}

		//	Keep item information for updating shortcuts later
		int tid = pItem1->GetTemplateID();
		
		m_pPack->RemoveItem(pCmd->src, pCmd->amount);

		//	Update shortcuts if all items have been moved
		if (!m_pPack->GetItem(pCmd->src))
			UpdateRemovedItemSC(tid, IVTRTYPE_PACK, pCmd->src);

		break; }
	case TRASH_ITEM_TO_IVTR: {
		const cmd_trash_item_to_ivtr* pCmd = (const cmd_trash_item_to_ivtr*)Msg.dwParam1;
		CECInventory* pPack = pCmd->where == IVTRTYPE_TRASHBOX ? m_pTrashBoxPack : m_pFactionTrashBoxPack;
		CECIvtrItem* pItem1 = pPack->GetItem(pCmd->src);
		CECIvtrItem* pItem2 = m_pPack->GetItem(pCmd->dest);
		if (!pItem1)
		{
			ASSERT(pItem1);
			break;
		}

		if (pItem2)
		{
			ASSERT(pItem1->GetTemplateID() == pItem2->GetTemplateID());
			pItem2->AddAmount(pCmd->amount);
		}
		else
		{
			pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pCmd->amount);
			m_pPack->SetItem(pCmd->dest, pItem2);
		}

		pPack->RemoveItem(pCmd->src, pCmd->amount);
		break; }
	case TRASHBOX_PWD_CHANGED:
	{
		const cmd_trashbox_pwd_changed* pCmd = (const cmd_trashbox_pwd_changed*)Msg.dwParam1;
		m_bTrashPsw = pCmd->has_passwd ? true : false;
		break;
	}
	case TRASHBOX_PWD_STATE:
	{
		const cmd_trashbox_pwd_state* pCmd = (const cmd_trashbox_pwd_state*)Msg.dwParam1;
		m_bTrashPsw = pCmd->has_passwd ? true : false;
		break;
	}
	case TRASHBOX_SIZE:
	{
		const cmd_trashbox_size* pCmd = (const cmd_trashbox_size*)Msg.dwParam1;
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_TRASHBOX_EXPAND, pCmd->iNewSize);
		if(m_pTrashBoxPack->GetSize() < pCmd->iNewSize)
		{
			m_pTrashBoxPack->Resize(pCmd->iNewSize);
		}		
		LuaEvent::FirstExpandTrash();
		break;
	}
	case MAFIA_TRASHBOX_CAPACITY:
	{
		const mafia_trashbox_capacity_notify* pCmd = (const mafia_trashbox_capacity_notify*)Msg.dwParam1;
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FACTION_TRASHBOX_EXPAND, pCmd->cap);
		break;
	}
	}
}

//新增随身包裹
void CECHostPlayer::OnMsgHstPocketOperation(const ECMSG& Msg)
{
	using namespace S2C;
	ASSERT(Msg.dwParam1);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
		case EXG_POCKET_ITEM:
		{
			const cmd_exg_pocket_item* pCmd = (const cmd_exg_pocket_item*)Msg.dwParam1;
			CECInventory* pPack = m_pPocketPack;
			pPack->ExchangeItem(pCmd->idx1, pCmd->idx2);

			// Arrange Storage Item
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			if (pGameUI) {
//				CDlgPocket* pDlgPocket = (CDlgPocket*)pGameUI->GetDialog("Win_Pocket");
//				if (pDlgPocket)
// 					pDlgPocket->OnItemExchanged(pCmd->idx1, pCmd->idx2);
			}
			break;
		}
		case MOVE_POCKET_ITEM:
		{
			const cmd_move_pocket_item* pCmd = (const cmd_move_pocket_item*)Msg.dwParam1;
			CECInventory* pPack = m_pPocketPack;
			pPack->MoveItem(pCmd->src, pCmd->dest, pCmd->amount);
			break;
		}
		case EXG_IVTR_POCKET_ITEM:
		{
			const cmd_exg_pocket_ivtr* pCmd = (const cmd_exg_pocket_ivtr*)Msg.dwParam1;
			CECInventory* pPack = m_pPocketPack;
			CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->idx_inv, true);
			CECIvtrItem* pItem2 = pPack->GetItem(pCmd->idx_poc, true);
			m_pPack->SetItem(pCmd->idx_inv, pItem2);
			pPack->SetItem(pCmd->idx_poc, pItem1);

			//	Update shortcuts
			if (pItem1)
				UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->idx_inv);

			break;
		}
		case IVTR_ITEM_TO_POCKET:
		{
			const cmd_ivtr_item_to_pocket* pCmd = (const cmd_ivtr_item_to_pocket*)Msg.dwParam1;
			CECInventory* pPack = m_pPocketPack;
			CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->src);
			CECIvtrItem* pItem2 = pPack->GetItem(pCmd->dest);
			if (!pItem1)
			{
				ASSERT(pItem1);
				break;
			}

			if (pItem2)
			{
				ASSERT(pItem1->GetTemplateID() == pItem2->GetTemplateID());
				pItem2->AddAmount(pCmd->amount);
			}
			else
			{
				pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pCmd->amount);
				pPack->SetItem(pCmd->dest, pItem2);
			}

			//	Keep item information for updating shortcuts later
			int tid = pItem1->GetTemplateID();
			
			m_pPack->RemoveItem(pCmd->src, pCmd->amount);

			//	Update shortcuts if all items have been moved
			if (!m_pPack->GetItem(pCmd->src))
				UpdateRemovedItemSC(tid, IVTRTYPE_PACK, pCmd->src);
			break;
		}
		case POCKET_ITEM_TO_IVTR:
		{
			const cmd_pocket_item_to_ivtr* pCmd = (const cmd_pocket_item_to_ivtr*)Msg.dwParam1;
			CECInventory* pPack = m_pPocketPack;
			CECIvtrItem* pItem1 = pPack->GetItem(pCmd->src);
			CECIvtrItem* pItem2 = m_pPack->GetItem(pCmd->dest);
			if (!pItem1)
			{
				ASSERT(pItem1);
				break;
			}

			if (pItem2)
			{
				ASSERT(pItem1->GetTemplateID() == pItem2->GetTemplateID());
				pItem2->AddAmount(pCmd->amount);
			}
			else
			{
				pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pCmd->amount);
				m_pPack->SetItem(pCmd->dest, pItem2);
			}

			pPack->RemoveItem(pCmd->src, pCmd->amount);
			break;
		}
		case CHANGE_POCKET_SIZE:
		{
			const cmd_change_pocket_size* pCmd = (const cmd_change_pocket_size*)Msg.dwParam1;
			if(m_pPocketPack->GetSize() < pCmd->new_size)
			{
				m_pPocketPack->Resize(pCmd->new_size);
			}
			break;
		}
		case POCKET_ITEM_ALL_TO_IVTR:
		{
			// 将随身包裹中的所有物品全部移到背包中
			CECIvtrItem* pItem1 = NULL;
			CECIvtrItem* pItem2 = NULL;
			int iEmptySlot = -1;
			if( m_pPocketPack->GetSize() - m_pPocketPack->GetEmptySlotNum() > m_pPack->GetEmptySlotNum())
			{
				ASSERT(0);
				break;
			}
			for(int i= 0; i< m_pPocketPack->GetSize(); ++i)
			{
				pItem1 = m_pPocketPack->GetItem(i);
				if(pItem1)
				{
					iEmptySlot = m_pPack->SearchEmpty();
					if(iEmptySlot != -1)
					{
						pItem2 = m_pPack->GetItem(iEmptySlot);
						if(pItem2)
						{
							ASSERT(!pItem2);
						}
						else
						{
							pItem2 = CECIvtrItem::CreateItem(pItem1->GetTemplateID(), pItem1->GetExpireDate(), pItem1->GetCount());
							m_pPack->SetItem(iEmptySlot, pItem2);
						}
					}
				}
			}
			m_pPocketPack->RemoveAllItems();
			break;
		}
	}
}


//新增时装包裹协议
void CECHostPlayer::OnMsgHstFashionPackOpt(const ECMSG& Msg)
{
	using namespace S2C;
	ASSERT(Msg.dwParam1);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
		case NOTIFY_FASHION_HOTKEY:
		{
			notify_fashion_hotkey cmd;
			if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			if(cmd.count)
			{
				for(int i=0; i<cmd.count; ++i)
				{
					m_aFashionHotKey[cmd.hotkey_list->index].id_head = cmd.hotkey_list->id_head;
					m_aFashionHotKey[cmd.hotkey_list->index].id_body = cmd.hotkey_list->id_cloth;
					m_aFashionHotKey[cmd.hotkey_list->index].id_shoe = cmd.hotkey_list->id_shoe;
					cmd.hotkey_list++;
				}
			}
			break;
		}
		case EXG_FASHION_ITEM:
		{
			const cmd_exg_fashion_item* pCmd = (const cmd_exg_fashion_item*)Msg.dwParam1;
			m_pFashionPack->ExchangeItem(pCmd->index1, pCmd->index2);
			break;
		}
		case EXG_IVTR_FASHION:
		{
			const cmd_exg_ivtr_fashion* pCmd = (const cmd_exg_ivtr_fashion*)Msg.dwParam1;
			CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->idx_ivr, true);
			CECIvtrItem* pItem2 = m_pFashionPack->GetItem(pCmd->idx_fas, true);
			m_pPack->SetItem(pCmd->idx_ivr, pItem2);
			m_pFashionPack->SetItem(pCmd->idx_fas, pItem1);

			//	Update shortcuts
			if (pItem1)
				UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->idx_ivr);

			break;
		}
		case EXG_EQUIP_FASHION:
		{
			const cmd_exg_equ_fashion* pCmd = (const cmd_exg_equ_fashion*)Msg.dwParam1;
			CECIvtrItem* pItem1 = m_pEquipPack->GetItem(pCmd->idx_equ, true);
			CECIvtrItem* pItem2 = m_pFashionPack->GetItem(pCmd->idx_fas, true);
			m_pEquipPack->SetItem(pCmd->idx_equ, pItem2);
			m_pFashionPack->SetItem(pCmd->idx_fas, pItem1);
			if(pItem2)
			{
				m_aEquips[pCmd->idx_equ] = pItem2->GetTemplateID();
				CECIvtrEquip * pEquip = (CECIvtrEquip *) pItem2;
				if(pEquip->GetNewEssenceForAstrology().reserved1 != 0)
				{
					CECIvtrIndividualityItem* pItem = 	(CECIvtrIndividualityItem*)CECIvtrItem::CreateItem(pEquip->GetNewEssenceForAstrology().reserved1, 0, 1);
					if(pItem&& pItem->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
					{
						WORD nColorId = pItem->GetDBEssence()->index;
						m_aEquips[pCmd->idx_equ] |= (nColorId << 16) & 0x7fffffff;
						delete pItem;
					}
				}
			}
			else
				m_aEquips[pCmd->idx_equ] = 0;

			// 竞技场中，无法换装
			if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType ==BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1)
			{
				break;
			}
			// 避免更换时装时候的闪烁
			ShowEquipments(m_aEquips, true, true);
			break;
		}
		case HOTKEY_CHANGE_FASHION:
		{
			const hotkey_change_fashion* pCmd = (const hotkey_change_fashion*)Msg.dwParam1;
			CECIvtrItem* pItem1;
			CECIvtrItem* pItem2;
			int idx;
			if(pCmd->idx1 != 0xff)
			{
				pItem1 = m_pFashionPack->GetItem(pCmd->idx1, true);
				m_aEquips[EQUIPIVTR_FASHION_HEAD] = pItem1->GetTemplateID();
				pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_HEAD, true);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_HEAD, pItem1);
				m_pFashionPack->SetItem(pCmd->idx1, pItem2);	
			}
			else
			{
				pItem1 = NULL;
				idx = m_pFashionPack->SearchEmpty();
				if(idx >=0)
				{
					m_aEquips[EQUIPIVTR_FASHION_HEAD] = 0;
					pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_HEAD, true);
					m_pEquipPack->SetItem(EQUIPIVTR_FASHION_HEAD, pItem1);
					m_pFashionPack->SetItem(idx, pItem2);					
				}
			}
	
			if(pCmd->idx2 != 0xff)
			{
				pItem1 = m_pFashionPack->GetItem(pCmd->idx2, true);
				m_aEquips[EQUIPIVTR_FASHION_BODY] = pItem1->GetTemplateID();
				pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_BODY, true);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BODY, pItem1);
				m_pFashionPack->SetItem(pCmd->idx2, pItem2);	
			}
			else
			{
				pItem1 = NULL;
				idx = m_pFashionPack->SearchEmpty();
				if(idx >=0)
				{
					m_aEquips[EQUIPIVTR_FASHION_BODY] = 0;
					pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_BODY, true);
					m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BODY, pItem1);
					m_pFashionPack->SetItem(idx, pItem2);					
				}
			}

			if(pCmd->idx3 != 0xff)
			{
				pItem1 = m_pFashionPack->GetItem(pCmd->idx3, true);
				m_aEquips[EQUIPIVTR_FASHION_FOOT] = pItem1->GetTemplateID();
				pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_FOOT, true);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_FOOT, pItem1);
				m_pFashionPack->SetItem(pCmd->idx3, pItem2);	
			}
			else
			{
				pItem1 = NULL;
				idx = m_pFashionPack->SearchEmpty();
				if(idx >=0)
				{
					m_aEquips[EQUIPIVTR_FASHION_FOOT] = 0;
					pItem2 = m_pEquipPack->GetItem(EQUIPIVTR_FASHION_FOOT, true);
					m_pEquipPack->SetItem(EQUIPIVTR_FASHION_FOOT, pItem1);
					m_pFashionPack->SetItem(idx, pItem2);					
				}
			}
			// 竞技场中，无法换装
			if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType ==BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBathType != -1)
			{
				m_bFashionModeBackUp = true;
				break;
			}
			// 自动切换到时装模式
			m_bFashionMode = true;
			ShowEquipments(m_aEquips);
			break;
		}
	}
}

//新增坐骑飞剑包裹协议
void CECHostPlayer::OnMsgHstMountWingPackOpt(const ECMSG& Msg)
{
	using namespace S2C;
	ASSERT(Msg.dwParam1);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
		case EXCHANGE_MOUNT_WING_ITEM:
		{
			const exchange_mount_wing_item* pCmd = (const exchange_mount_wing_item*)Msg.dwParam1;
			m_pMountWingPack->ExchangeItem(pCmd->index1, pCmd->index2);
			setRecordMonnt(IVTRTYPE_MOUNTWING,pCmd->index1,IVTRTYPE_MOUNTWING,pCmd->index2);
			break;
		}
		case EXCHANGE_INVENTORY_MOUNTWING_ITEM:
		{
			const exchange_Inventory_mountwing_item* pCmd = (const exchange_Inventory_mountwing_item*)Msg.dwParam1;
			CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->idx_inv, true);
			CECIvtrItem* pItem2 = m_pMountWingPack->GetItem(pCmd->idx_mv, true);

			m_pPack->SetItem(pCmd->idx_inv, pItem2);
			m_pMountWingPack->SetItem(pCmd->idx_mv, pItem1);

			//	Update shortcuts
			if (pItem1)
				UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->idx_inv);
			setRecordMonnt(IVTRTYPE_PACK,pCmd->idx_inv,IVTRTYPE_MOUNTWING,pCmd->idx_mv);
			break;
		}
		case EXCHANGE_EQUIPMENT_MOUNTWING_ITEM:
		{
			const exchange_equipment_mountwing_item* pCmd = (const exchange_equipment_mountwing_item*)Msg.dwParam1;
			CECIvtrItem* pItem1 = m_pEquipPack->GetItem(pCmd->idx_equ, true);
			CECIvtrItem* pItem2 = m_pMountWingPack->GetItem(pCmd->idx_mv, true);
			m_pEquipPack->SetItem(pCmd->idx_equ, pItem2);
			m_pMountWingPack->SetItem(pCmd->idx_mv, pItem1);
			if( pItem2 )
				pItem2->SetNeedUpdate(true);
			if(pItem2)
				m_aEquips[pCmd->idx_equ] = pItem2->GetTemplateID();
			else
				m_aEquips[pCmd->idx_equ] = 0;
			
			UpdateEquipSkins();
			CheckRecordMount(IVTRTYPE_EQUIPPACK,EQUIPIVTR_WING,0);
			break;
		}
		case PLAYER_CHANGE_MOUNTWING_INV_SIZE:
		{
			const player_change_mountwing_inv_size* pCmd = (const player_change_mountwing_inv_size*)Msg.dwParam1;
			if(m_pMountWingPack->GetSize() < pCmd->new_size)
			{
				m_pMountWingPack->Resize(pCmd->new_size);
			}
			break;	

			break;
		}
	}
}

void CECHostPlayer::OnMsgHstIvtrInfo(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case OWN_IVTR_DATA:
	{
		cmd_own_ivtr_info cmd;
		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
			return;

		CECInventory* pPack = GetPack(cmd.byPackage);
		if (!pPack)
			return;

		pPack->ResetItems(cmd);

		//	Update skins
		if (cmd.byPackage == IVTRTYPE_EQUIPPACK)
		{
			UpdateEquipSkins();
		}
		else if (cmd.byPackage == IVTRTYPE_TRASHBOX)
		{
			if (m_bUsingTrashBox)
				pGameUI->PopupStorageDialog(false, false);
		}
		else if (cmd.byPackage == IVTRTYPE_FACTIONTRASHBOX)
		{
			if (m_bUsingTrashBox)
				pGameUI->PopupStorageDialog(false, true);
		}
		else if (cmd.byPackage == IVTRTYPE_PET)
		{
			m_pPetPack->ResetItems(cmd);
		}
		else if(cmd.byPackage == IVTRTYPE_POCKETPACK)
		{
			m_pPocketPack->ResetItems(cmd);
		}
		else if(cmd.byPackage == IVTRTYPE_FASHION)
		{
			m_pFashionPack->ResetItems(cmd);
		}
		else if(cmd.byPackage == IVTRTYPE_MOUNTWING)
		{
			m_pMountWingPack->ResetItems(cmd);
		}
		else if(cmd.byPackage == IVTRTYPE_GIFT)
		{
			m_pGiftPack->ResetItems(cmd);
		}
		else if (cmd.byPackage == IVTRTYPE_FUWEN)
		{
			m_pRune2013Pack->ResetItems(cmd);
		}

		break;
	}
	case OWN_IVTR_DETAIL_DATA:
	{
		cmd_own_ivtr_detail_info cmd;
		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
			return;

		CECInventory* pPack = GetPack(cmd.byPackage);
		if (!pPack)
			return;

		pPack->ResetItems(cmd);

		//	Update skins
		if (cmd.byPackage == IVTRTYPE_EQUIPPACK)
		{
			//	Test code ...
			/*
			if (m_iGender == GENDER_FEMALE)
			{
				CECIvtrItem* pEquip = CECIvtrItem::CreateItem(4, 1);
				m_pEquipPack->SetItem(1, pEquip);
				pEquip = CECIvtrItem::CreateItem(6, 0, 1);
				m_pEquipPack->SetItem(2, pEquip);
				pEquip = CECIvtrItem::CreateItem(9, 0, 1);
				m_pEquipPack->SetItem(3, pEquip);
				pEquip = CECIvtrItem::CreateItem(11, 0, 1);
				m_pEquipPack->SetItem(4, pEquip);
				pEquip = CECIvtrItem::CreateItem(15, 0, 1);
				m_pEquipPack->SetItem(5, pEquip);
				pEquip = CECIvtrItem::CreateItem(17, 0, 1);
				m_pEquipPack->SetItem(6, pEquip);
				pEquip = CECIvtrItem::CreateItem(19, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BACK, pEquip);
				pEquip = CECIvtrItem::CreateItem(13, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_FOOT, pEquip);
				pEquip = CECIvtrItem::CreateItem(18, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_LEG, pEquip);
				pEquip = CECIvtrItem::CreateItem(5, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BODY, pEquip);
				
				pEquip = CECIvtrItem::CreateItem(21, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_WEAPON, pEquip);
			}
			else
			{
				CECIvtrItem* pEquip = CECIvtrItem::CreateItem(4, 0, 1);
				m_pEquipPack->SetItem(1, pEquip);
				pEquip = CECIvtrItem::CreateItem(7, 0, 1);
				m_pEquipPack->SetItem(2, pEquip);
				pEquip = CECIvtrItem::CreateItem(8, 0, 1);
				m_pEquipPack->SetItem(3, pEquip);
				pEquip = CECIvtrItem::CreateItem(10, 0, 1);
				m_pEquipPack->SetItem(4, pEquip);
				pEquip = CECIvtrItem::CreateItem(14, 0, 1);
				m_pEquipPack->SetItem(5, pEquip);
				pEquip = CECIvtrItem::CreateItem(16, 0, 1);
				m_pEquipPack->SetItem(6, pEquip);
				pEquip = CECIvtrItem::CreateItem(19, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BACK, pEquip);
				pEquip = CECIvtrItem::CreateItem(12, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_FOOT, pEquip);
				pEquip = CECIvtrItem::CreateItem(18, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_LEG, pEquip);
				pEquip = CECIvtrItem::CreateItem(5, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_FASHION_BODY, pEquip);

				pEquip = CECIvtrItem::CreateItem(22, 0, 1);
				m_pEquipPack->SetItem(EQUIPIVTR_WEAPON, pEquip);
			}*/

			UpdateEquipSkins();
		}
		else if (cmd.byPackage == IVTRTYPE_TRASHBOX)
		{
			if (m_bUsingTrashBox)
				pGameUI->PopupStorageDialog(false, false);
		}
		else if (cmd.byPackage == IVTRTYPE_FACTIONTRASHBOX)
		{
			if (m_bUsingTrashBox)
				pGameUI->PopupStorageDialog(false, true);
		}
		else if (cmd.byPackage == IVTRTYPE_PET)
		{
			m_pPetPack->ResetItems(cmd);
		}
		else if	(cmd.byPackage == IVTRTYPE_POCKETPACK)
		{
			m_pPocketPack->ResetItems(cmd);
		}
		else if( cmd.byPackage == IVTRTYPE_FASHION )
		{
			m_pFashionPack->ResetItems(cmd);
		}
		else if( cmd.byPackage == IVTRTYPE_MOUNTWING )
		{
			m_pMountWingPack->ResetItems(cmd);
		}
		else if( cmd.byPackage == IVTRTYPE_GIFT )
		{
			m_pGiftPack->ResetItems(cmd);
		}
		else if (cmd.byPackage == IVTRTYPE_FUWEN)
		{
			m_pRune2013Pack->ResetItems(cmd);
		}

		break;
	}
	case GET_OWN_MONEY:
	{
		cmd_get_own_money* pCmd = (cmd_get_own_money*)Msg.dwParam1;
		SetMoneyAmount(pCmd->amount);
		m_iMaxMoney = (int)pCmd->max_amount;
		break;
	}
	case REPURCHASE_INV_DATA:
	{
		repurchase_inv_data cmd;
		if (!cmd.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
			return;
		
		m_pSellBackPack->ResetItems(cmd);
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->CommonMessage("Win_Shop_Withdraw", "update", 0, 0);
		// TODO: 填充m_pSellBackPack
	}
	}
}

void CECHostPlayer::OnMsgHstTeamInvite(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_leader_invite* pCmd = (cmd_team_leader_invite*)Msg.dwParam1;
	ASSERT(pCmd);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	//	Try to get player's information
	CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(pCmd->idLeader);
	if (pPlayer)
	{
		pGameUI->PopupInviteGroupMenu(pCmd->idLeader, pPlayer->GetName(), pCmd->seq, pCmd->wPickFlag, pCmd->familyid, pCmd->mafiaid);
		return;
	}

	CECEPCacheData* pCacheData = m_pPlayerMan->GetPlayerCacheData(pCmd->idLeader);
	if (pCacheData)
	{
		pGameUI->PopupInviteGroupMenu(pCmd->idLeader, pCacheData->m_strName, pCmd->seq, pCmd->wPickFlag, pCmd->familyid, pCmd->mafiaid);
		return;
	}

	//	Couldn't get player's information immediately, so we have to got it from
	//	server.
	for (int i=0; i < m_aTeamInvs.GetSize(); i++)
	{
		TEAMINV& Inv = m_aTeamInvs[i];
		if (Inv.idLeader == pCmd->idLeader)
		{
			//	Invitation from this leader has existed, update invitation info.
			Inv.seq			= pCmd->seq;
			Inv.wPickFlag	= pCmd->wPickFlag;
			return;
		}
	}

	//	Save team invitation information
	TEAMINV TeamInv;
	TeamInv.idLeader	= pCmd->idLeader;
	TeamInv.seq			= pCmd->seq;
	TeamInv.wPickFlag	= pCmd->wPickFlag;
	TeamInv.idFamily	= pCmd->familyid;
	TeamInv.idGuild		= pCmd->mafiaid;
	m_aTeamInvs.Add(TeamInv);

	g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &pCmd->idLeader, 3);
}


void CECHostPlayer::OnMsgHstTeamReject(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_reject_invite* pCmd = (cmd_team_reject_invite*)Msg.dwParam1;
	ASSERT(pCmd);

	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_REJECTJOINTEAM, g_pGame->GetGameRun()->GetPlayerName(pCmd->idPlayer, true));
}

void CECHostPlayer::OnMsgHstTeamMemPos(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_member_pos* pCmd = (cmd_team_member_pos*)Msg.dwParam1;
	ASSERT(pCmd);

	if (!m_pTeam)
	{
		ASSERT(0);
		return;
	}

	CECTeamMember* pMember = m_pTeam->GetMemberByID(pCmd->idMember);
	if (pMember)
		pMember->SetPos(pCmd->idInst, pCmd->pos);
}

void CECHostPlayer::OnMsgHstEquipDamaged(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_equip_damaged* pCmd = (cmd_equip_damaged*)Msg.dwParam1;
	ASSERT(pCmd);

	CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pEquipPack->GetItem(pCmd->index);
	ASSERT(pEquip);

	//	Reduce equipment's endurance to 0
	pEquip->AddCurEndurance(-pEquip->GetCurEndurance());
}

void CECHostPlayer::OnMsgHstTeamMemPickup(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_member_pickup* pCmd = (cmd_team_member_pickup*)Msg.dwParam1;
	ASSERT(pCmd);

	//	Get item name from it's tid
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(pCmd->tid, 0, 1);
	if (pItem)
	{
		ACHAR szName[64];
		ACString strMsg;
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		AUI_ConvertChatString(g_pGame->GetGameRun()->GetPlayerName(pCmd->idMember, true), szName);
		strMsg.Format(pStrTab->GetWideString(FIXMSG_TEAMMEMPICKUP), szName, (int)pCmd->count, pItem->GetName());
		pGameUI->AddChatMessage(strMsg, CLIENT_CHAT_PICKUP);
		delete pItem;
	}
}

void CECHostPlayer::OnMsgHstNPCGreeting(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_npc_greeting* pCmd = (cmd_npc_greeting*)Msg.dwParam1;
	ASSERT(pCmd);

	if (ISNPCID(pCmd->idObject))
	{
		CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(pCmd->idObject);
		if (!pNPC || !pNPC->IsServerNPC())
		{
			ASSERT(0);
			return;
		}
	
		//	Check distance again
		if (!CanTouchTarget(pNPC->GetPos(), pNPC->GetTouchRadius(), 3))
			return;

		if (gr.get_state() != game_record::enum_gr_replay)
		{
			m_idSevNPC		= pCmd->idObject;
			m_bTalkWithNPC	= true;
		}

		//	Check way point service on NPC
		DWORD dwID = ((CECNPCServer*)pNPC)->GetWayPointID();
		if (dwID && !HasWayPoint((WORD)dwID))
			g_pGame->GetGameSession()->c2s_CmdNPCSevWaypoint();

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		const NPC_ESSENCE * pNPCEssence = ((CECNPCServer*)pNPC)->GetDBEssence();
		pGameUI->PopupNPCDialog(pNPCEssence);

		if( pNPCEssence && (pNPCEssence->id == 61180 || pNPCEssence->id == 61218) )
		{
			AString strSound;
			strSound.Format("sfx\\wumochou\\%d.wav", rand() % 8 + 1);
			g_pGame->GetA3DEngine()->GetAMSoundEngine()->GetAMSoundBufferMan()->Play2DAutoSound(strSound);
		}

		/*
		pNPC->NPCTurnFaceTo(m_PlayerInfo.cid, 300);
		((CECNPCServer*)pNPC)->StartGreeting();
		*/
	}
	else if (ISPLAYERID(pCmd->idObject))
	{
		CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(pCmd->idObject);

		//	Check distance again
		if (!pPlayer || !CanTouchTarget(pPlayer->GetPos(), 0.0f, 3))
			return;

		if (gr.get_state() != game_record::enum_gr_replay)
		{
			m_idSevNPC		= pCmd->idObject;
			m_bTalkWithNPC	= true;
			m_iBoothState	= 3;
		}
		
		g_pGame->GetGameSession()->c2s_CmdNPCSevGetContent(GP_NPCSEV_BOOTHSELL);

		m_pBuyPack->RemoveAllItems();
		m_pSellPack->RemoveAllItems();
		m_pEPBoothBPack->RemoveAllItems();
		m_pEPBoothSPack->RemoveAllItems();

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->PopupBoothDialog(true, false);
	}
	else
	{
		ASSERT(0);
		return;
	}
}

void CECHostPlayer::OnMsgHstTradeStart(const ECMSG& Msg)
{
	using namespace GNET;
	TradeStart_Re* p = (TradeStart_Re*)Msg.dwParam1;
	ASSERT(p->roleid == m_PlayerInfo.cid);


	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (p->retcode == ERR_SUCCESS)
	{
		//	Clear deal packages
		m_pDealPack->RemoveAllItems();
		m_pDealPack->Resize(IVTRSIZE_DEALPACK);
		m_pEPDealPack->RemoveAllItems();
		m_pEPDealPack->Resize(IVTRSIZE_DEALPACK);

		m_idTradePlayer = p->partner_roleid;
		pGameUI->PopupTradeResponseMenu(p->partner_roleid, p->tid, true);
	}
	else
		pGameUI->PopupTradeResponseMenu(p->partner_roleid, p->tid, false);
}

void CECHostPlayer::OnMsgHstTradeRequest(const ECMSG& Msg)
{
	using namespace GNET;
	TradeStartRqst* p = (TradeStartRqst*)Msg.dwParam1;
	TradeStartRqstArg* pArg = (TradeStartRqstArg*)p->GetArgument();

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->PopupTradeInviteMenu(pArg->start_roleid, (DWORD)p);
}

void CECHostPlayer::OnMsgHstTradeMoveItem(const ECMSG& Msg)
{
	using namespace GNET;
	TradeMoveObj_Re* p = (TradeMoveObj_Re*)Msg.dwParam1;
	if (p->retcode != ERR_SUCCESS)
	{
		//	Un-freeze items
		FreezeItem(IVTRTYPE_PACK, p->src_pos, false, true);
		FreezeItem(IVTRTYPE_PACK, p->dst_pos, false, true);
		return;
	}

	ASSERT(p->roleid == m_PlayerInfo.cid);

	CECIvtrItem* pSrcItem = m_pPack->GetItem(p->src_pos);
	CECIvtrItem* pDstItem = m_pPack->GetItem(p->dst_pos);

	if (!pSrcItem)
	{
		ASSERT(pSrcItem);
		return;
	}

	if (!pDstItem || (pSrcItem->GetTemplateID() == pDstItem->GetTemplateID() && pSrcItem->GetPileLimit() > 1))
	{
		m_pPack->MoveItem(p->src_pos, p->dst_pos, p->count);

		//	Update shortcuts
		UpdateMovedItemSC(pSrcItem->GetTemplateID(), IVTRTYPE_PACK, p->src_pos, IVTRTYPE_PACK, p->dst_pos);
	}
	else
	{
		m_pPack->ExchangeItem(p->src_pos, p->dst_pos);

		//	Update shortcuts
		if (pSrcItem && pDstItem)
			UpdateExchangedItemSC(pSrcItem->GetTemplateID(), IVTRTYPE_PACK, p->src_pos, pDstItem->GetTemplateID(), IVTRTYPE_PACK, p->dst_pos);
		else if (pSrcItem)
			UpdateMovedItemSC(pSrcItem->GetTemplateID(), IVTRTYPE_PACK, p->src_pos, IVTRTYPE_PACK, p->dst_pos);
		else if (pDstItem)
			UpdateMovedItemSC(pDstItem->GetTemplateID(), IVTRTYPE_PACK, p->dst_pos, IVTRTYPE_PACK, p->src_pos);
	}

	//	Un-freeze items
	FreezeItem(IVTRTYPE_PACK, p->src_pos, false, true);
	FreezeItem(IVTRTYPE_PACK, p->dst_pos, false, true);
}

void CECHostPlayer::OnMsgHstTradeCancel(const ECMSG& Msg)
{
	using namespace GNET;
	TradeDiscard_Re* p = (TradeDiscard_Re*)Msg.dwParam1;

	m_idTradePlayer = 0;

	//	Clear deal packages
	m_pDealPack->RemoveAllItems();
	m_pEPDealPack->RemoveAllItems();

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->TradeAction(p->discard_roleid, p->tid, CDlgTrade::TRADE_ACTION_CANCEL, 0);
}

void CECHostPlayer::OnMsgHstTradeSubmit(const ECMSG& Msg)
{
	using namespace GNET;
	TradeSubmit_Re* p = (TradeSubmit_Re*)Msg.dwParam1;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	if (p->retcode == ERR_TRADE_READY_HALF || p->retcode == ERR_TRADE_READY)
	{
		pGameUI->TradeAction(p->submit_roleid, p->tid, CDlgTrade::TRADE_ACTION_LOCK, p->retcode);
	}
	else if (p->retcode == ERR_TRADE_SPACE)
	{
		//	One of trader doesn't have enough package slot
		pGameUI->TradeAction(p->submit_roleid, p->tid, CDlgTrade::TRADE_ACTION_UNLOCK, p->retcode);
	}
}

void CECHostPlayer::OnMsgHstTradeConfirm(const ECMSG& Msg)
{
	using namespace GNET;
	TradeConfirm_Re* p = (TradeConfirm_Re*)Msg.dwParam1;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->TradeAction(p->confirm_roleid, p->tid, CDlgTrade::TRADE_ACTION_DEAL, p->retcode);

	//	Trade succeeded, update shortcuts which reference to sold items
	if (p->retcode == ERR_TRADE_DONE && m_pDealPack->GetSize())
	{
		//	Build exclude slots array
		AArray<int, int> aExcSlots;
		int i;

		for (i=0; i < m_pDealPack->GetSize(); i++)
		{
			CECIvtrItem* pItem = m_pDealPack->GetItem(i);
			if (pItem)
			{
				const CECDealInventory::ITEMINFO& ItemInfo = m_pDealPack->GetItemInfo(i);
				if (ItemInfo.iAmount >= pItem->GetCount())
					aExcSlots.Add(ItemInfo.iOrigin);
			}
		}

		for (i=0; i < m_pDealPack->GetSize(); i++)
		{
			CECIvtrItem* pItem = m_pDealPack->GetItem(i);
			if (!pItem)
				continue;

			const CECDealInventory::ITEMINFO& ItemInfo = m_pDealPack->GetItemInfo(i);
			if (ItemInfo.iAmount >= pItem->GetCount())
			{
				UpdateRemovedItemSC(pItem->GetTemplateID(), IVTRTYPE_PACK, ItemInfo.iOrigin, 
					aExcSlots.GetData(), aExcSlots.GetSize());
			}
		}
		const ACHAR *pTradePlayerName = g_pGame->GetGameRun()->GetPlayerName(m_idTradePlayer, true);
		m_pFriendMan->AddTempFriend(m_idTradePlayer, pTradePlayerName);
	}
	else if (p->retcode == ERR_TRADE_SPACE)
	{
		//	Notify player that one of trader doesn't have enough package slot
		if (p->confirm_roleid == m_PlayerInfo.cid)
			pGameUI->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_PACKFULL1), "errmsg_packfull1");
		else
			pGameUI->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_PACKFULL2), "errmsg_packfull2");
	}
}

void CECHostPlayer::OnMsgHstTradeEnd(const ECMSG& Msg)
{
	using namespace GNET;
	TradeEnd* p = (TradeEnd*)Msg.dwParam1;

	m_idTradePlayer = 0;

	//	Un-freeze items
	for (int i=0; i < m_pDealPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = m_pDealPack->GetItem(i);
		if (pItem)
			pItem->NetFreeze(false);
	}

	//	Clear deal packages
	m_pDealPack->RemoveAllItems();
	m_pEPDealPack->RemoveAllItems();

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->TradeAction(p->roleid, p->tid, CDlgTrade::TRADE_ACTION_END, p->cause);
}

void CECHostPlayer::OnMsgHstTradeAddGoods(const ECMSG& Msg)
{
	using namespace GNET;
	TradeAddGoods_Re* p = (TradeAddGoods_Re*)Msg.dwParam1;
	ASSERT(p->roleid == m_PlayerInfo.cid);
	
	if (p->owner_roleid == m_PlayerInfo.cid)
	{
		if (p->retcode == ERR_SUCCESS)
		{
			if (p->goods.id)
			{
				CECIvtrItem* pItem = m_pPack->GetItem(p->goods.pos);
				if (!pItem)
				{
					ASSERT(0);
					return;
				}

				m_pDealPack->AddItem(pItem, p->goods.pos, p->goods.count, false);
			}

			m_pDealPack->AddMoney(p->money);
		}
		else
		{
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->ShowErrorMsg(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_NOTTRADEITEM));
		}
	}
	else
	{
		ASSERT(ISPLAYERID(p->owner_roleid));

		if (p->goods.id)
		{
			CECIvtrItem* pItem = NULL;
			int iIndex = m_pEPDealPack->GetItemIndex(p->goods.pos);
			if (iIndex >= 0)
			{
				const CECDealInventory::ITEMINFO& Info = m_pEPDealPack->GetItemInfo(iIndex);
				ASSERT(Info.iOrigin == p->goods.pos);

				pItem = m_pEPDealPack->GetItem(iIndex);
				if (pItem)
					pItem->AddAmount(p->goods.count);
			}
			else
			{
				//	Create a new item
				CECIvtrItem* pItem = CECIvtrItem::CreateItem(p->goods.id, p->goods.expire_date, p->goods.count);
				if (pItem)
				{
					if( p->goods.data.begin() && p->goods.data.size() )
					{
						if (pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART)
						{
							((CECIvtrTalismanMain*)pItem)->SetItemFromMailInfo((unsigned char*)p->goods.data.begin(), p->goods.data.size());
						}
						else
							pItem->SetItemInfo((BYTE*)p->goods.data.begin(), p->goods.data.size());
					}
					else
						pItem->SetEmptyInfo();
					pItem->SetPriceScale(CECIvtrItem::SCALE_SELL, PLAYER_PRICE_SCALE);
					pItem->UpdateInfoMD5();

					m_pEPDealPack->AddItem(pItem, p->goods.pos, p->goods.count, true);
				}
			}
		}

		m_pEPDealPack->AddMoney(p->money);
	}

	//	Notify interface other bargainer changed his goods
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->TradeAction(p->owner_roleid, p->tid, CDlgTrade::TRADE_ACTION_ALTER, 0);
}

void CECHostPlayer::OnMsgHstTradeRemGoods(const ECMSG& Msg)
{
	using namespace GNET;
	TradeRemoveGoods_Re* p = (TradeRemoveGoods_Re*)Msg.dwParam1;
	ASSERT(p->roleid == m_PlayerInfo.cid);

	if (p->owner_roleid == m_PlayerInfo.cid)
	{
		if (p->retcode != ERR_SUCCESS)
			return;

		if (p->goods.id)
		{
			CECIvtrItem* pItem = m_pPack->GetItem(p->goods.pos);
			if (!pItem)
			{
				ASSERT(0);
				return;
			}

			m_pDealPack->RemoveItem(p->goods.pos, p->goods.count);
		}

		m_pDealPack->AddMoney(-(int)p->money);
	}
	else
	{
		ASSERT(ISPLAYERID(p->owner_roleid));

		if (p->goods.id)
		{
			int iIndex = m_pEPDealPack->GetItemIndex(p->goods.pos);
			if (iIndex < 0)
			{
				ASSERT(0);
				return;
			}

			m_pEPDealPack->RemoveItem(p->goods.pos, p->goods.count);
		}

		m_pEPDealPack->AddMoney(-(int)p->money);
	}

	//	Notify interface other bargainer changed his goods
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->TradeAction(p->owner_roleid, p->tid, CDlgTrade::TRADE_ACTION_ALTER, 0);
}

void CECHostPlayer::OnMsgHstStartAttack(const ECMSG& Msg)
{
	//	DISCUSED function !
	ASSERT(FALSE);
	/*
	using namespace S2C;
	
	cmd_host_start_attack* pCmd = (cmd_host_start_attack*)Msg.dwParam1;
	ASSERT(pCmd);

	//	test code...
//	g_pGame->GetRTDebug()->OutputNotifyMessage(RTDCOL_WARNING, _AL("start attack !"));

	//	Check whether target is the one that we have selected
	if (m_idSelTarget != pCmd->idTarget)
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Target has changed !"));
	
	//	If target turn to be un-attackable, cancel action
	if (!AttackableJudge(pCmd->idTarget, true))
	{
		g_pGame->GetGameSession()->c2s_CmdCancelAction();
		g_pGame->RuntimeDebugInfo(RTDCOL_WARNING, _AL("Cannel attacking !"));
		return;
	}

	CECHPWorkMelee* pWork = (CECHPWorkMelee*)m_pWorkMan->CreateWork(CECHPWork::WORK_HACKOBJECT);
	m_pWorkMan->StartWork_p1(pWork);

	m_bMelee = true;
	*/
}

void CECHostPlayer::OnMsgHstGainItem(const ECMSG& Msg)
{
	using namespace S2C;

	ACString strMsg;
	CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ASSERT(pGameUI);

	switch (Msg.dwParam2)
	{
	case GAIN_MONEY_IN_TRADE:
	case GAIN_ITEM_IN_TRADE:

		//	TODO: print a message to notify player
		break;

	case GAIN_MONEY_AFTER_TRADE:
	{
		cmd_gain_money_after_trade* pCmd = (cmd_gain_money_after_trade*)Msg.dwParam1;
		ASSERT(pCmd);
		AddMoneyAmount(pCmd->amount);
		break;
	}
	case GAIN_ITEM_AFTER_TRADE:
	{
		cmd_gain_item_after_trade* pCmd = (cmd_gain_item_after_trade*)Msg.dwParam1;
		ASSERT(pCmd);

		int iLastSlot, iSlotNum;
		if (!m_pPack->MergeItem(pCmd->tid, pCmd->state, pCmd->expire_date, pCmd->amount, &iLastSlot, &iSlotNum) ||
			iLastSlot != (int)pCmd->index || iSlotNum != (int)pCmd->wSlotAmount)
		{
			//	Local inventory data lose synchronization with server,
			//	so we have to re-get all inventory data from server.
			g_pGame->GetGameSession()->c2s_CmdGetIvtrDetailData(IVTRTYPE_PACK);

			//	Print a notice
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(pCmd->tid, pCmd->expire_date, 1);
			if (pItem)
			{
				strMsg.Format(pStrTab->GetWideString(FIXMSG_PICKUPITEM), (int)pCmd->amount, pItem->GetName());
				pGameUI->AddChatMessage(strMsg, CLIENT_CHAT_PICKUP);
				delete pItem;
			}
		}
		else
		{
			//	If item is equipment we have to get it's detail information, 
			//	otherwise it may be shown as "unable to be equipped"
			CECIvtrItem* pItem = m_pPack->GetItem(pCmd->index);
			if (!pItem)
				return;

			if (pItem->IsEquipment())
				g_pGame->GetGameSession()->c2s_CmdGetItemInfo(IVTRTYPE_PACK, (BYTE)pCmd->index);

			if (pItem->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART)
				LuaEvent::FirstObtainTalisman(); 
			if (pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
			{
				LuaEvent::FirstObtainPet();
				LuaEvent::FirstObtainSpecialPet(pItem->GetTemplateID());
			}
			if (pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR)
			{
				LuaEvent::FirstObtainPetEquip();
			}
			//	Print a notice
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_PICKUPITEM, (int)pCmd->amount, pItem->GetName());
		}

		//NewHelpGuide::TriggerGetItem();

		break;
	}
	}
}

void CECHostPlayer::OnMsgHstPurchaseItems(const ECMSG& Msg)
{
	using namespace S2C;
	
	cmd_purchase_item* pCmd = (cmd_purchase_item*)Msg.dwParam1;
	ASSERT(pCmd);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	AArray<BYTE, BYTE> aNeedUpdateItem;
	aNeedUpdateItem.SetSize(pCmd->item_count, 10);

	for (int i=0; i < pCmd->item_count; i++)
	{
		const cmd_purchase_item::ITEM& Item = pCmd->item_list[i];

		//	Put item into pack
		int iLastSlot, iSlotNum;
		if (!m_pPack->MergeItem(Item.item_id, Item.item_state, Item.expire_date, Item.count, &iLastSlot, &iSlotNum) ||
			iLastSlot != Item.inv_index)
		{
			ASSERT(0);
			continue;
		}

		//	If item is equipment we have to get it's detail information, 
		//	otherwise it may be shown as "unable to be equipped"
		CECIvtrItem* pItem = m_pPack->GetItem(Item.inv_index);
		if (pItem && pItem->IsEquipment())
			aNeedUpdateItem.Add((BYTE)Item.inv_index);

		if (pItem && pItem->GetClassID()==CECIvtrItem::ICID_LOTTERY_TANGYUAN)
			aNeedUpdateItem.Add((BYTE)Item.inv_index);

		if (pCmd->flag && m_iBoothState == 2)
		{
			int iBoothIndex = m_pBoothBPack->GetItemIndexByFlag(Item.booth_slot);
			CECIvtrItem* pBoothItem = m_pBoothBPack->GetItem(iBoothIndex);
			if (pBoothItem)
			{
				//	Give player a notify message
				struct tm* time = glb_GetFormatLocalTime();
				ACString strMsg;
				ACHAR szPrice[256];
				CECIvtrItem::GetPriceText(szPrice, pBoothItem->GetUnitPrice());
				strMsg.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_BOOTHBUY), 
					time->tm_hour, time->tm_min,g_pGame->GetGameRun()->GetPlayerName(pCmd->trade_id, true), Item.count, pBoothItem->GetName(), szPrice);
				pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_OK", strMsg, 0x0fffffff, 0, 0, 0);
				PAUIDIALOG pDlg = pGameUI->GetDialog("Win_PShop1");
				PAUITEXTAREA pMsgList = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_MsgList");
				strMsg += _AL('\r');
				pMsgList->AppendText(strMsg);
			}

			//	From item from booth buy pack
			m_pBoothBPack->RemoveItemByFlag(Item.booth_slot, Item.count);
		}
	}

	//	Spent money
	AddMoneyAmount(-(int)pCmd->cost);

	if (aNeedUpdateItem.GetSize())
		g_pGame->GetGameSession()->c2s_CmdGetItemInfoList(IVTRTYPE_PACK, aNeedUpdateItem.GetSize(), aNeedUpdateItem.GetData());

	//NewHelpGuide::TriggerGetItem();
}

void CECHostPlayer::OnMsgHstSpendMoney(const ECMSG& Msg)
{
	using namespace S2C;
	
	cmd_spend_money* pCmd = (cmd_spend_money*)Msg.dwParam1;
	ASSERT(pCmd);

	AddMoneyAmount(-(int)pCmd->cost);
}

void CECHostPlayer::OnMsgHstItemToMoney(const ECMSG& Msg)
{
	using namespace S2C;
	
	cmd_item_to_money* pCmd = (cmd_item_to_money*)Msg.dwParam1;
	ASSERT(pCmd);


	CECIvtrItem* pItem = m_pPack->GetItem(pCmd->index);
	if (pItem)
	{
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		ACString strMoney1, strMoney2, strMoney3;
		ACString strMsg;
		if (pCmd->money>=10000)
			strMoney1.Format(pGameUI->GetStringFromTable(1701), pCmd->money/10000);
		else
			strMoney1.Empty();
		
		if ((pCmd->money%10000)>=100)
			strMoney2.Format(pGameUI->GetStringFromTable(1702), (pCmd->money%10000)/100);
		else
			strMoney2.Empty();
		
		if ((pCmd->money%100)>0)
			strMoney3.Format(pGameUI->GetStringFromTable(1703), pCmd->money%100);
		else
			strMoney3.Empty();
		if (strMoney1.IsEmpty()&&strMoney2.IsEmpty()&&strMoney3.IsEmpty())
		{
			strMoney3.Format(pGameUI->GetStringFromTable(1703), 0);
		}
		strMsg.Format(pGameUI->GetStringFromTable(9171), pCmd->count, pItem->GetName(), strMoney1+strMoney2+strMoney3);
			pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
	}

	m_pPack->RemoveItem(pCmd->index, pCmd->count);

	//	Update shortcut
	if (!m_pPack->GetItem(pCmd->index))
		UpdateRemovedItemSC(pCmd->type, IVTRTYPE_PACK, pCmd->index);

	AddMoneyAmount((int)pCmd->money);
}

void CECHostPlayer::OnMsgHstRepair(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == REPAIR_ALL)
	{
		cmd_repair_all* pCmd = (cmd_repair_all*)Msg.dwParam1;
		ASSERT(pCmd);

		m_pEquipPack->RepairAllItems();
	//	m_pPack->RepairAllItems();
	//	m_pTaskPack->RepairAllItems();

		AddMoneyAmount(-(int)pCmd->cost);
	}
	else if (Msg.dwParam2 == REPAIR)
	{
		cmd_repair* pCmd = (cmd_repair*)Msg.dwParam1;
		ASSERT(pCmd);

		CECInventory* pPack = GetPack(pCmd->byPackage);
		if (!pPack) return;
		CECIvtrItem* pItem = (CECIvtrItem*)pPack->GetItem(pCmd->bySlot);
		if (pItem && pItem->IsEquipment())
			((CECIvtrEquip*)pItem)->Repair();

		AddMoneyAmount(-(int)pCmd->cost);
	}
}

void CECHostPlayer::OnMsgHstUseItem(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_host_use_item* pCmd = (cmd_host_use_item*)Msg.dwParam1;
	ASSERT(pCmd);

	CECInventory* pPack = GetPack(pCmd->byPackage);
	if (!pPack) return;

	CECIvtrItem* pItem = (CECIvtrItem*)pPack->GetItem(pCmd->bySlot);
	if (!pItem || pItem->GetTemplateID() != pCmd->item_id)
	{
		ASSERT(0);
		return;
	}

	//	Play some effect or sound ?
	PlayUseItemEffect(pCmd->item_id);

	pItem->Use();

	if (pCmd->use_count)
	{
		pPack->RemoveItem(pCmd->bySlot, pCmd->use_count);

		//	Update shortcut
		if (!pPack->GetItem(pCmd->bySlot))
			UpdateRemovedItemSC(pCmd->item_id, pCmd->byPackage, pCmd->bySlot);			
	}

	//	End using-item work
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_USEITEM)
	{
		CECHPWorkUse* pWork = (CECHPWorkUse*)m_pWorkMan->GetCurWork();
		if (pWork->GetItem() == pCmd->item_id)
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_USEITEM, false);
	}
}

void CECHostPlayer::OnMsgHstUseItemWithData(const ECMSG& Msg)
{
	using namespace S2C;

	player_use_item_with_arg* pCmd = (player_use_item_with_arg*)Msg.dwParam1;
	ASSERT(pCmd);

	CECInventory* pPack = GetPack(pCmd->where);
	if (!pPack) return;

	CECIvtrItem* pItem = (CECIvtrItem*)pPack->GetItem(pCmd->index);
	if (!pItem || pItem->GetTemplateID() != pCmd->item_id)
	{
		ASSERT(0);
		return;
	}

	//	Play some effect or sound ?
	PlayUseItemEffect(pCmd->item_id, pCmd->arg, pCmd->size);

	pItem->Use();

	if (pCmd->use_count)
	{
		pPack->RemoveItem(pCmd->index, pCmd->use_count);

		//	Update shortcut
		if (!pPack->GetItem(pCmd->index))
			UpdateRemovedItemSC(pCmd->item_id, pCmd->where, pCmd->index);
	}

	//	End using-item work
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_USEITEM)
	{
		CECHPWorkUse* pWork = (CECHPWorkUse*)m_pWorkMan->GetCurWork();
		if (pWork->GetItem() == pCmd->item_id)
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_USEITEM, false);
	}

}

void CECHostPlayer::OnMsgHstSkillData(const ECMSG& Msg)
{
	// first clear all skill's base level
	int i;
	
	if( m_bSkillDataInited ) // if has set skill data before, we need to clear it here.
	{
		for (i=0; i < m_aPtSkills.GetSize(); i++)
		{
			if (!m_aPtSkills[i]->IsExtra() && m_aAPSkills.find(m_aPtSkills[i]->GetSkillID())==m_aAPSkills.end())
				m_aPtSkills[i]->SetLevel(0);
		}
		for (i=0; i < m_aPsSkills.GetSize(); i++)
		{
			if (!m_aPsSkills[i]->IsExtra() && m_aAPSkills.find(m_aPsSkills[i]->GetSkillID())==m_aAPSkills.end())
				m_aPsSkills[i]->SetLevel(0);
		}
	}

	using namespace S2C;

	cmd_skill_data* pCmd = (cmd_skill_data*)Msg.dwParam1;
	ASSERT(pCmd);
	
	int skill_count = (int)pCmd->skill_count;

	BYTE* pData = (BYTE*)pCmd + sizeof (size_t);
	void* skill_list = pData;
	pData += skill_count * sizeof(cmd_skill_data::SKILL);

	int skill_count2 = *(int*)pData;
	pData += sizeof (int);
	void* skill_list2 = pData;

	GNET::ElementSkill::LoadSkillData(pCmd);

	for (i=0; i < skill_count; i++)
	{
		const cmd_skill_data::SKILL& Data = ((cmd_skill_data::SKILL*)skill_list)[i];
		CECSkill* pSkill = GetSkill(Data.id_skill);

		if (!pSkill)
		{
			pSkill = new CECSkill(Data.id_skill, Data.level);
			if (pSkill->GetType() != CECSkill::TYPE_PASSIVE &&
				pSkill->GetType() != CECSkill::TYPE_LIVE)
				m_aPtSkills.Add(pSkill);
			else
				m_aPsSkills.Add(pSkill);
		}
		else
			pSkill->SetLevel(Data.level);
	}

	for (i=0; i< skill_count2; i++)
	{
		const cmd_skill_data::SKILL2& Data2 = ((cmd_skill_data::SKILL2*)skill_list2)[i];
		CECSkill* pSkill = GetSkill(Data2.id_skill);
		if(!pSkill)
		{

		}
		else
			pSkill->SetProficiency(Data2.proficiency);
	}

	m_bSkillDataInited = true;
}

void CECHostPlayer::OnMsgTransfigureSkillData(const ECMSG& Msg)
{
	// first clear all skill's base level
	int i, j;
	abase::vector<SKILL_COOL_BACKUP> coolDownBackUp; // 变身技能的冷却备份
	abase::vector<int> replaceIds;		// 法宝栏待替换的id

	// 如果当前技能为变身技能，则清掉当前技能
	if (m_pCurSkill && GetTransfigureSkillByID(m_pCurSkill->GetSkillID()))
	{
		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SPELLOBJECT)
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_SPELLOBJECT, false);	
		m_pCurSkill = NULL;
	}

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if( m_bTransfigureSkillDataInited ) // if has set skill data before, we need to clear it here.
	{
		for (i =0; i< m_aTsSkills.GetSize(); i++)
		{
			m_TsSkillCoolDown[m_aTsSkills[i]->GetSkillID()] = m_aTsSkills[i]->GetCoolingCnt();
			for (j=0; j < NUM_HOSTSCSETS1; j++)
			{
				if (m_aSCSets1[j])
					m_aSCSets1[j]->ReplaceSkillID(m_aTsSkills[i]->GetSkillID(), NULL);
			}
			
			for (j=0; j < NUM_HOSTSCSETS2; j++)
			{
				if (m_aSCSets2[j])
					m_aSCSets2[j]->ReplaceSkillID(m_aTsSkills[i]->GetSkillID(), NULL);
			}
			((CDlgTalisman*)pGameUI->GetDialog("Win_TalismanSmall"))->UpdateRemovedSkill(m_aTsSkills[i]->GetSkillID());
			replaceIds.push_back(m_aTsSkills[i]->GetSkillID());
		}

		for (i=0; i < m_aTsSkills.GetSize(); i++)
			delete m_aTsSkills[i];

		m_aTsSkills.RemoveAll(false);
	}

	using namespace S2C;

	cmd_skill_data* pCmd = (cmd_skill_data*)Msg.dwParam1;
	ASSERT(pCmd);

	int skill_count = (int)pCmd->skill_count;

	BYTE* pData = (BYTE*)pCmd + sizeof (size_t);
	void* skill_list = pData;
	pData += skill_count * sizeof(cmd_skill_data::SKILL);

	int skill_count2 = *(int*)pData;
	pData += sizeof (int);
	void* skill_list2 = pData;

//	GNET::ElementSkill::LoadSkillData(pCmd);

	for (i=0; i < skill_count; i++)
	{
		const cmd_skill_data::SKILL& Data = ((cmd_skill_data::SKILL*)skill_list)[i];
		CECSkill* pSkill = GetSkill(Data.id_skill);

		if (!pSkill)
		{
			pSkill = new CECSkill(Data.id_skill, Data.level);
			m_aTsSkills.Add(pSkill);
		}
		else
			pSkill->SetLevel(Data.level);

		if (i<(int)replaceIds.size())
		{
			((CDlgTalisman*)pGameUI->GetDialog("Win_TalismanSmall"))->UpdateExchangeSkill(replaceIds[i], pSkill);
		}
	}

	for (i=0; i< skill_count2; i++)
	{
		const cmd_skill_data::SKILL2& Data2 = ((cmd_skill_data::SKILL2*)skill_list2)[i];
		CECSkill* pSkill = GetSkill(Data2.id_skill);
		if(!pSkill)
		{

		}
		else
			pSkill->SetProficiency(Data2.proficiency);
	}
	//  变身技能重置冷却
	for (i=0; i< (int)m_aTsSkills.GetSize(); ++i)
	{
		abase::hash_map<int, int>::iterator it = m_TsSkillCoolDown.find(m_aTsSkills[i]->GetSkillID());
		if (it != m_TsSkillCoolDown.end())
			m_aTsSkills[i]->StartCooling(0, it->second);
	}

	m_bTransfigureSkillDataInited = true;
}

/**
*@param: Msg,协议消息体
*@return: void
*@usage: 处理关于时装、首饰等附加永久技能的相关协议
**/
void CECHostPlayer::OnMsgHstAddonPermanentSkill( const ECMSG& Msg )
{
	using namespace S2C;

	if ( Msg.dwParam2 == ADDON_PERMANENT_SKILL )
	{
		addon_permanent_skill* pCmd = (addon_permanent_skill*)Msg.dwParam1;
		ASSERT(pCmd);
	
		CECSkill* pSkill = GetSkill( pCmd->skill_id );
		if ( !pSkill )
		{
			pSkill = new CECSkill( pCmd->skill_id, pCmd->skill_level );
			ASSERT(pSkill);
			if (pSkill->GetType() != CECSkill::TYPE_PASSIVE &&
				pSkill->GetType() != CECSkill::TYPE_LIVE)
			{
				m_aPtSkills.Add(pSkill);
			}
			else
			{
				m_aPsSkills.Add(pSkill);
			}
			//同时将该技能指针加入到查询Map中。注意，这个Map只作查询，不做删除，具体的存储和删除由m_aPtSkills或者m_aPsSkills维护
			m_aAPSkills[pCmd->skill_id] = pSkill;
		}
		else
		{
			if ( !GetAddonPermanentSkillByID( pCmd->skill_id ) ) //同时将该技能指针加入到查询Map中.这种情况保证m_aAPSkills更新的同步
			{
				m_aAPSkills[pCmd->skill_id] = pSkill;
			}
			pSkill->SetLevel( pCmd->skill_level );
		}
		//重置冷却时间
		abase::hash_map<int, int>::iterator it = m_TsSkillCoolDown.find( pCmd->skill_id );
		if ( it != m_TsSkillCoolDown.end() )
		{
			pSkill->StartCooling( 0, it->second );
		}
	}
	else if ( Msg.dwParam2 == ADDON_PERMANENT_SKILL_DATA )
	{
		addon_permanent_skill_data* pCmd = (addon_permanent_skill_data*)Msg.dwParam1;
		ASSERT(pCmd);

		int nSkillCount = (int)pCmd->skill_count;
		BYTE* pData = (BYTE*)pCmd + sizeof(size_t);
		void* skill_list = pData;

		for ( int i = 0; i < nSkillCount; ++i )
		{
			const addon_permanent_skill_data::SKILL& aSkill = ((addon_permanent_skill_data::SKILL*)skill_list)[i];
			
			GNET::ElementSkill::SetFashionSkillLevel(aSkill.id_skill, aSkill.level);
			CECSkill* pSkill = GetSkill( aSkill.id_skill );
			if ( !pSkill )
			{
				pSkill = new CECSkill(aSkill.id_skill, aSkill.level);
				ASSERT(pSkill);
				if (pSkill->GetType() != CECSkill::TYPE_PASSIVE &&
					pSkill->GetType() != CECSkill::TYPE_LIVE)
				{
					m_aPtSkills.Add(pSkill);
				}
				else
				{
					m_aPsSkills.Add(pSkill);
				}
				//同时将该技能指针加入到查询Map中。注意，这个Map只作查询，不做删除，具体的存储和删除由m_aPtSkills或者m_aPsSkills维护
				m_aAPSkills[aSkill.id_skill] = pSkill;
			}
			else
			{
				if ( !GetAddonPermanentSkillByID( aSkill.id_skill ) ) //同时将该技能指针加入到查询Map中.这种情况保证m_aAPSkills更新的同步
				{
					m_aAPSkills[aSkill.id_skill] = pSkill;
				}
				pSkill->SetLevel( aSkill.level );
			}
		}

		//附加永久技能重置冷却
		for ( AddonPermanentSkillMap::iterator iter = m_aAPSkills.begin(); iter != m_aAPSkills.end(); ++iter )
		{
			abase::hash_map<int, int>::iterator it = m_TsSkillCoolDown.find(iter->second->GetSkillID());
			if ( it != m_TsSkillCoolDown.end() )
			{
				iter->second->StartCooling( 0, it->second );
			}
		}
	}
}

void CECHostPlayer::OnMsgHstCombineSkillData(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_skill_elems_data* pCmd = (cmd_skill_elems_data*)Msg.dwParam1;
	ASSERT(pCmd);	

	GNET::ElementSkill::LoadSkillElemsData(pCmd);
}

void CECHostPlayer::OnMsgHstRandomMove(const ECMSG& Msg)
{
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return;
	}

	switch (Msg.dwParam1)
	{
	case 0:	m_dwMoveRelDir |= MD_FORWARD;				break;
	case 1:	m_dwMoveRelDir |= MD_FORWARD | MD_LEFT;		break;
	case 2:	m_dwMoveRelDir |= MD_LEFT;					break;
	case 3:	m_dwMoveRelDir |= MD_BACK | MD_LEFT;		break;
	case 4:	m_dwMoveRelDir |= MD_BACK;					break;
	case 5:	m_dwMoveRelDir |= MD_BACK | MD_RIGHT;		break;
	case 6:	m_dwMoveRelDir |= MD_RIGHT;					break;
	case 7:	m_dwMoveRelDir |= MD_FORWARD | MD_RIGHT;	break;
	case 8:	m_dwMoveRelDir |= MD_ABSUP;					break;
	case 9:	m_dwMoveRelDir |= MD_ABSDOWN;				break;
	}

	bool bPushMove = true;
	if (Msg.dwParam1 == 8 || Msg.dwParam1 == 9)
	{
		if (m_iMoveEnv != MOVEENV_AIR && m_iMoveEnv != MOVEENV_WATER)
			bPushMove = false;
	}

	if (bPushMove && !IsAboutToDie())
	{
		bool bRecord = false;
		CECHPWork* pWork = m_pWorkMan->GetCurWork();

		if (!pWork || (pWork->GetWorkID() != CECHPWork::WORK_MOVETOPOS && pWork->CanTransferTo(CECHPWork::MASK_MOVETOPOS)))
		{
			CECHPWorkMove* pNewWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVETOPOS);
			pNewWork->SetDestination(CECHPWorkMove::DEST_PUSH, g_vOrigin);
			m_pWorkMan->StartWork_p1(pNewWork);
			bRecord = true;
		}
		else if (m_dwMoveRelDir != m_dwOldDir && pWork->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
			bRecord = true;

		if (bRecord && gr.get_state() == game_record::enum_gr_record)
		{
			frame_record* fr = gr.get_cur_frame();

			if (fr)
				fr->push_event(new event_record_msg(Msg));
		}
	}
}

GNET::GamedataSend CECHostPlayer::gen_skill_data()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(4000);
	cmd_header h;
	h.cmd = SKILL_DATA;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<size_t>(o, m_aPtSkills.GetSize() + m_aPsSkills.GetSize());

	int i;
	for (i = 0; i < m_aPtSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPtSkills[i];
		add_to_octets<short>(o, pSkill->GetSkillID());
		add_to_octets<unsigned char>(o, pSkill->GetSkillLevel());
	}

	for (i = 0; i < m_aPsSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPsSkills[i];
		add_to_octets<short>(o, pSkill->GetSkillID());
		add_to_octets<unsigned char>(o, pSkill->GetSkillLevel());
	}
	//  另外的一部分技能数据
	add_to_octets<size_t>(o, 0);
	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_title_list()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(1024);
	cmd_header h;
	h.cmd = PLAYER_TITLE_LIST;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<size_t>(o, m_vecTitles.size());

	for (size_t i = 0; i < m_vecTitles.size(); i++)
		add_to_octets<short>(o, m_vecTitles[i]);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_sel_target()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(64);
	cmd_header h;
	h.cmd = SELECT_TARGET;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_idSelTarget);
	return gds;
}

void CECHostPlayer::OnMsgHstEmbedItem(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_embed_item* pCmd = (cmd_embed_item*)Msg.dwParam1;
	ASSERT(pCmd);

	CECIvtrItem* pEquip = m_pPack->GetItem(pCmd->equip_idx);
	CECIvtrItem* pTessera = m_pPack->GetItem(pCmd->chip_idx);

	if (!pEquip || !pTessera)
	{
		ASSERT(0);
		return;
	}

	m_pPack->RemoveItem(pCmd->chip_idx, 1);

	//	Refresh equip's data
	g_pGame->GetGameSession()->c2s_CmdGetItemInfo(IVTRTYPE_PACK, pCmd->equip_idx);
}

void CECHostPlayer::OnMsgHstClearTessera(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_clear_tessera* pCmd = (cmd_clear_tessera*)Msg.dwParam1;
	ASSERT(pCmd);

	CECIvtrItem* pEquip = m_pPack->GetItem(pCmd->equip_idx);
	if (!pEquip)
	{
		ASSERT(0);
		return;
	}

	AddMoneyAmount(-(int)pCmd->cost);

	//	Refresh equip's data
	g_pGame->GetGameSession()->c2s_CmdGetItemInfo(IVTRTYPE_PACK, (BYTE)pCmd->equip_idx);
}

void CECHostPlayer::OnMsgHstCostSkillPt(const ECMSG& Msg)
{
	using namespace S2C;
}

void CECHostPlayer::OnMsgHstLearnSkill(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_learn_skill* pCmd = (cmd_learn_skill*)Msg.dwParam1;
	ASSERT(pCmd);

	CECSkill* pSkill = GetSkill(pCmd->skill_id);
	if (pSkill)
	{
		if (pCmd->skill_level)
		{
			pSkill->LevelUp();
			GNET::ElementSkill::SetLevel(pCmd->skill_id, pCmd->skill_level);
		}
		else
		{
			//	Remove this skill
			RemoveSkill(pCmd->skill_id);
		}
	}
	else if (pCmd->skill_level)
	{
		if (!(pSkill = new CECSkill(pCmd->skill_id, pCmd->skill_level)))
		{
			ASSERT(pSkill);
			return;
		}

		if (pSkill->GetJunior())
		{
			ReplaceJuniorSkill(pSkill->GetJunior(), pSkill);
		}
		else
		{
			if (pSkill->GetType() != CECSkill::TYPE_PASSIVE &&
				pSkill->GetType() != CECSkill::TYPE_LIVE)
				m_aPtSkills.Add(pSkill);
			else
				m_aPsSkills.Add(pSkill);
		}

		GNET::ElementSkill::SetLevel(pCmd->skill_id, pCmd->skill_level);
	}

	if (pSkill)
	{
		if( pSkill->IsCategory(GNET::CATEGORY_TALENT) )
			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_LEARNTALENT, GP_CHAT_SYSTEM, g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), pCmd->skill_level);
		else
			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_LEARNSKILL, GP_CHAT_SYSTEM, g_pGame->GetSkillDesc()->GetWideString(pSkill->GetSkillID() * 10), pCmd->skill_level);
	}

	//	Update UI
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateTeach();
}

void CECHostPlayer::OnMsgHstFlySwordTime(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_flysword_time* pCmd = (cmd_flysword_time*)Msg.dwParam1;
	ASSERT(pCmd);

/*	CECInventory* pPack = (CECInventory*)GetPack(pCmd->where);
	if (!pPack) return;

	CECIvtrItem* pItem = pPack->GetItem(pCmd->index);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_FLYSWORD)
	{
		ASSERT(0);
		return;
	}

	CECIvtrFlySword* pFlySword = (CECIvtrFlySword*)pItem;
	pFlySword->SetCurTime(pCmd->cur_time);
*/
}

void CECHostPlayer::OnMsgHstProduceItem(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ASSERT(pGameUI);

	if (Msg.dwParam2 == PRODUCE_START)
	{
		cmd_produce_start* pCmd = (cmd_produce_start*)Msg.dwParam1;
		ASSERT(pCmd);
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_PRODUCE_START, pCmd);
	}
	else if (Msg.dwParam2 == PRODUCE_END)
	{
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_PRODUCE_END, NULL);
	}
	else if (Msg.dwParam2 == PRODUCE_NULL)
	{
		cmd_produce_null* pCmd = (cmd_produce_null*)Msg.dwParam1;
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_PRODUCE_NULL, pCmd);
	}
}

void CECHostPlayer::OnMsgHstBreakItem(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ASSERT(pGameUI);

	if (Msg.dwParam2 == DECOMPOSE_START)
	{
		cmd_decompose_start* pCmd = (cmd_decompose_start*)Msg.dwParam1;
		ASSERT(pCmd);
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_SPLIT_START, pCmd);
	}
	else if (Msg.dwParam2 == DECOMPOSE_END)
	{
		pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_SPLIT_END, NULL);
	}
}

void CECHostPlayer::OnMsgHstTaskData(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == TASK_DATA)
	{
		cmd_task_data* pCmd = (cmd_task_data*)Msg.dwParam1;
		ASSERT(pCmd);

		int iActiveListSize = (int)pCmd->active_list_size;
		BYTE* pData = (BYTE*)pCmd + sizeof (size_t);
		void* pActiveListbuf = pData;
		pData += iActiveListSize;
		int iFinishedListSize = *(int*)pData;
		pData += sizeof (int);
		void* pFinishedListBuf = pData;
		pData += iFinishedListSize;
		int iFinishTimeListSize = *(int*)pData;
		pData += sizeof(int);
		void* pFinishTimeListBuf = pData;

		A3DRELEASE(m_pTaskInterface);

		if (!(m_pTaskInterface = new CECTaskInterface(this)))
		{
			glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::OnMsgHstTaskData", __LINE__);
			return;
		}

		if (!m_pTaskInterface->Init(pActiveListbuf, iActiveListSize, pFinishedListBuf, iFinishedListSize, pFinishTimeListBuf, iFinishTimeListSize))
		{
			a_LogOutput(1, "CECHostPlayer::OnMsgHstTaskData, failed to initialize task interface");
			return;
		}

		int i;
		int idTask;
		ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
		for( i = 0; i < (int)m_pTaskInterface->GetTaskCount(); i++ )
		{
			idTask = m_pTaskInterface->GetTaskId(i);
			ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask); 
			if ( pTemp && pTemp->m_bScriptFinishTask )
			{
				m_ScriptFinishTaskMap[idTask] = true;
			} 
			ProcessChildTask(idTask);
		}

		//	Note: this command now is also used as the end flag of responding
		//		for GET_ALL_DATA request
		g_pGame->GetGameSession()->c2s_CmdGetServerTime();
		g_pGame->GetGameSession()->LoadConfigData();
		g_pGame->GetGameSession()->LoadHelpStates();
		g_pGame->GetGameSession()->war_infoget(1);
	}
	else if (Msg.dwParam2 == TASK_VAR_DATA)
	{
		cmd_task_var_data* pCmd = (cmd_task_var_data*)Msg.dwParam1;
		ASSERT(pCmd);
		OnServerNotify(m_pTaskInterface, pCmd->data, pCmd->size);
	}
}
//  Only for lua trigger task, to traverse the child task
void CECHostPlayer::ProcessChildTask(int idTask)
{
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	int nPos = m_pTaskInterface->GetFirstSubTaskPosition(idTask);
	while( nPos > -1 )
	{
		int id = m_pTaskInterface->GetNextSub(nPos);
		ATaskTempl *pTemp = pMan->GetTaskTemplByID(id);
		if ( pTemp && pTemp->m_bScriptFinishTask )
		{
			m_ScriptFinishTaskMap[id] = true;
		} 
		ProcessChildTask(id);
	}
}


void CECHostPlayer::OnMsgHstTargetIsFar(const ECMSG& Msg)
{
//	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_HACKOBJECT)
	CmdNormalAttack(true);
}

void CECHostPlayer::OnMsgHstCameraMode(const ECMSG& Msg)
{
	if(Msg.dwParam1 == 1)
	{
		StartCutscene(1);
//		StartCameraPath(2925);
//		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
//		CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pGameUI->GetDialog("Win_CameraPath");
//
//		pDlgCameraPath->ShowText(_AL("$5000$hello world诛仙2摄像机测试\n$10000$这个string显示10s时间...\n$5000$\n$10000$camera漫游到此结束"));
	}
//		m_CameraCtrl.MovieMode_Set(!m_CameraCtrl.MovieMode_Get());	
	else
		m_CameraCtrl.SceneryMode_Set(!m_CameraCtrl.SceneryMode_Get());

	LuaEvent::FirstCameraMode();
}

void CECHostPlayer::OnMsgHstPressCancel(const ECMSG& Msg)
{
	if( m_pCutscene &&
		m_pCutscene->IsPlayingCutscene() &&
		m_pCutscene->CanEscClose())
	{
		m_pCutscene->EndCutscene();
		return;
	}

	if (gr.get_state() == game_record::enum_gr_record)
	{
		frame_record* fr = gr.get_cur_frame();

		if (fr)
			fr->push_event(new event_record_msg(Msg));
	}
	
	// 如果角色处于被嘲讽状态， 禁止ESC功能
	if(m_BetaunTed)
		return;

	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	if (pCurWork)
	{
		switch (pCurWork->GetWorkID())
		{
	//	case CECHPWork::WORK_MOVETOPOS:
			
	//		((CECHPWorkMove*)pCurWork)->PressCancel();
	//		return;

		case CECHPWork::WORK_TRACEOBJECT:
			
			((CECHPWorkTrace*)pCurWork)->PressCancel();
			return;

	//	case CECHPWork::WORK_FOLLOW:
			
	//		m_pWorkMan->FinishCurWork(true);
	//		return;

		case CECHPWork::WORK_HACKOBJECT:
		case CECHPWork::WORK_USEITEM:
		case CECHPWork::WORK_GENERAL_OPRT:

			g_pGame->GetGameSession()->c2s_CmdCancelAction();
			return;

		case CECHPWork::WORK_SPELLOBJECT:
		{	
			int iState = ((CECHPWorkSpell*)pCurWork)->GetState();
			if (iState == CECHPWorkSpell::ST_INCANT)
				g_pGame->GetGameSession()->c2s_CmdCancelAction();
	
			return;
		}
		case CECHPWork::WORK_PICKUP:
		{
			if (((CECHPWorkPick*)pCurWork)->IsGather())
				g_pGame->GetGameSession()->c2s_CmdCancelAction();

			return;
		}
		case CECHPWork::WORK_CONCENTRATE:
		{
			if (IsOperatingPet())
			{
				g_pGame->GetGameSession()->c2s_CmdCancelAction();
				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pGameUI->CommonMessage("Win_Pet_Detail", "stop_opt", 0, 0);
			}
			return;
		}
		case CECHPWork::WORK_MOVEBEZIEL:
		{
			((CECHPWorkBezierMove*)pCurWork)->PressCancel();
			return;
		}
		}
	}

	if (m_bUsingTrashBox || DoingSessionPose())
	{
		g_pGame->GetGameSession()->c2s_CmdCancelAction();
		return;
	}

	//	Cancel current selection
	if (m_idSelTarget)
	{
		g_pGame->GetGameSession()->c2s_CmdUnselect();
		return;
	}

	//	Some work have lower priority
	if (pCurWork->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
	{
		((CECHPWorkMove*)pCurWork)->PressCancel();
		return;
	}
}

void CECHostPlayer::OnMsgHstRootNotify(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == HOST_NOTIFY_ROOT)
	{
		cmd_host_notify_root* pCmd = (cmd_host_notify_root*)Msg.dwParam1;
		m_dwLIES |= (1 << pCmd->type);

		if (pCmd->type != 3)
		{
			//	Force pull host to specified position
			//SetPos(pCmd->pos);
			//  HostPlayer has been ticked, the player will shake if we set pos at once. 
			m_vCachePos = pCmd->pos;
			m_bHasCachePos = true;
		}
		
		if (IsRooting())
		{
			int iCurWork = m_pWorkMan->GetCurWorkID();
			if (iCurWork == CECHPWork::WORK_FOLLOW ||
				iCurWork == CECHPWork::WORK_MOVETOPOS ||
				iCurWork == CECHPWork::WORK_TRACEOBJECT)
				m_pWorkMan->FinishCurWork(iCurWork, true);
		}
	}
	else if (Msg.dwParam2 == HOST_DISPEL_ROOT)
	{
		cmd_host_dispel_root* pCmd = (cmd_host_dispel_root*)Msg.dwParam1;
		m_dwLIES &= ~(1 << pCmd->type);
	}
}

void CECHostPlayer::OnMsgHstStopAttack(const ECMSG& Msg)
{
	using namespace S2C;

	m_bMelee = false;

	// if there is an attack event currently, we should let it fire now.
	if( m_pPlayerModel )
		m_pPlayerModel->ClearComActFlag(true);

	if (!m_pWorkMan)
	{
		ASSERT(0);
		return;
	}
//	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
//	if (!pCurWork || pCurWork->GetWorkID() != CECHPWork::WORK_HACKOBJECT)
//		return;

	cmd_host_stop_attack* pCmd = (cmd_host_stop_attack*)Msg.dwParam1;
	ASSERT(pCmd);

	/*	If attack stopped for target is leave too far, trace it and continue
		attacking. Stop reason defined as below:

		0x00: attack is canceled or host want to do some other things.
		0x01: unable to attack anymore (no ammo, weapon is broken etc.)
		0x02: invalid target (target missed or died)
		0x04: target is out of range
	*/
	if (pCmd->iReason & 0x04)
	{
		if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_MOVETOPOS &&
			m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_TRACEOBJECT)
			CmdNormalAttack();
	}
	else
		m_pWorkMan->FinishCurWork(CECHPWork::WORK_HACKOBJECT, false);
}


void CECHostPlayer::OnMsgHstJump(const ECMSG& Msg)
{
#define    JUMP_START_SPEED(d)  (sqrt((d) *2.0 * EC_GRAVITY))
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return;
	}

	if (!CanDo(CANDO_JUMP))
		return;

	if (gr.get_state() == game_record::enum_gr_record && Msg.dwParam1 != MAKEFOURCC('Q', 'I', 'N', 'G'))
	{
		frame_record* fr = gr.get_cur_frame();

		if (fr)
			fr->push_event(new event_record_msg(Msg));
	}

	//float fVertSpeed = 5.0f;
	float fVertSpeed = (float)JUMP_START_SPEED(1.2);

	if( Msg.dwParam1 == MAKEFOURCC('Q', 'I', 'N', 'G') )
	{
		fVertSpeed = (float)JUMP_START_SPEED(2.5);
		float fHorzSpeed = *(float *)&Msg.dwParam2;
#ifdef CDR_HL2_MOVE
		A3DVECTOR3 vH = Normalize(A3DVECTOR3(m_pCDRInfo->vClipVel.x, 0.0f, m_pCDRInfo->vClipVel.z)) * fHorzSpeed;
		m_pCDRInfo->vClipVel.x = vH.x;
		m_pCDRInfo->vClipVel.z = vH.z;
#endif
		SetJumpByQingGong(true);
	}
	if( Msg.dwParam1 == MAKEFOURCC('R', 'U', 'S', 'H') )
	{
		fVertSpeed = (float)JUMP_START_SPEED(0.0);
		float fHorzSpeed = *(float *)&Msg.dwParam2;
#ifdef CDR_HL2_MOVE
		A3DVECTOR3 vH = Normalize(A3DVECTOR3(m_pCDRInfo->vClipVel.x, 0.0f, m_pCDRInfo->vClipVel.z)) * fHorzSpeed ;
		m_pCDRInfo->vClipVel.x = vH.x;
		m_pCDRInfo->vClipVel.z = vH.z;
#endif
		SetJumpByQingGong(true);
	}

	if (m_iJumpCount == 0)
	{
		if (m_iMoveEnv == MOVEENV_WATER)
		{
			if (!IsUnderWater())
				return;

			fVertSpeed = 7.0f;
			SetJumpInWater(true);
		}
		else if (!m_GndInfo.bOnGround)
			return;
	}

	if (InSlidingState())
		return;

	m_iJumpCount++;
	m_fVertSpeed = fVertSpeed;
	m_pCDRInfo->vAbsVelocity.y = fVertSpeed;
#ifdef CDR_HL2_MOVE
	//m_pCDRInfo->vClipVel.Clear();
	m_pCDRInfo->vClipVel.y = 0.0f;
#else
	m_pCDRInfo->fYVel = 0.0f;
#endif

//	m_pCDRInfo->vTPNormal.Clear();
	ResetCDRInfo();

	// special case, its qing gong here
	if( Msg.dwParam1 == MAKEFOURCC('Q', 'I', 'N', 'G') )
	{
		PlayAction(ACT_QINGGONG_LOOP, 1.0f, true);
		//PlayAction(ACT_QINGGONG_START, 1.0f);
		//PlayAction(ACT_QINGGONG_LOOP, 1.0f, true, 0, true);
	}
	else if(Msg.dwParam1 ==  MAKEFOURCC('R', 'U', 'S', 'H'))
	{

	}
	else
	{
		if ( !m_pChangedModel ) //正常情况下，仍然保持QueueAction播放动作
		{
			PlayAction(ACT_JUMP_START, 1.0f);
			PlayAction(ACT_JUMP_LOOP, 1.0f, true, 0, true);
		}
		else					//变身情况下，走PlayActionByName播放动作
		{
			PlayAction(ACT_JUMP_START, 1.0f);
			PlayAction(ACT_JUMP_LOOP, 1.0f, true, 0, false);
		}
	}
}

void CECHostPlayer::OnMsgHstHurtResult(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == BE_HURT)
	{
		cmd_be_hurt* pCmd = (cmd_be_hurt*)Msg.dwParam1;
		if (pCmd->damage)
			Damaged(pCmd->damage,1);
	}
	else if (Msg.dwParam2 == HURT_RESULT)
	{
		cmd_hurt_result* pCmd = (cmd_hurt_result*)Msg.dwParam1;
		if (pCmd->target_id == m_PlayerInfo.cid)
			return;		//	Host heself will receive BE_HURT, so ignore this.

		if (ISPLAYERID(pCmd->target_id))
		{
			CECElsePlayer* pTarget = m_pPlayerMan->GetElsePlayer(pCmd->target_id);
			if (pTarget)
				pTarget->Damaged(pCmd->damage, 1);
		}
		else if (ISNPCID(pCmd->target_id))
		{
			CECNPC* pTarget = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(pCmd->target_id);
			if (pTarget)
				pTarget->Damaged(pCmd->damage, 1);
		}
	}
}

void CECHostPlayer::OnMsgHstAttackOnce(const ECMSG& Msg)
{
	//	AttackOnce has come into discuse
/*	using namespace S2C;
	cmd_attack_once* pCmd = (cmd_attack_once*)Msg.dwParam1;

	if (m_bMelee && m_pComboSkill && !m_pComboSkill->IsStop())
		m_pComboSkill->Continue(true);
*/
}

void CECHostPlayer::OnMsgHstPlayTrick(const ECMSG& Msg)
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return;
	}

	if (!m_TrickCnt.IsFull())
		return;

	m_TrickCnt.Reset();

	int iCurAction = m_pPlayerModel ? (int)m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY) : 0;
	int iTrickAct;

	if (IsJumping())
		iTrickAct = ACT_TRICK_JUMP;
	else if (iCurAction == ACT_RUN)
		iTrickAct = ACT_TRICK_RUN;
	else
		return;

	PlayAction(iTrickAct, 1.0f);
	g_pGame->GetGameSession()->c2s_CmdTrickAction(iTrickAct);

	if (IsValidAction(iCurAction))
		PlayAction(iCurAction, 1.0f, true, 200, true);

	// we need to notify help system that the user did a trick, to keep the check state uptodate.
	CECScriptMan * pScriptMan = g_pGame->GetGameRun()->GetUIManager()->GetScriptMan();
	if( pScriptMan )
	{
		pScriptMan->GetContext()->GetCheckState()->SetPlayerStatusIsPlayTrick(true);
	}
}

void CECHostPlayer::OnMsgHstFaction(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_mafia_info_notify* pNotify;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case MAFIA_INFO_NOTIFY:

		pNotify = reinterpret_cast<const cmd_mafia_info_notify*>(Msg.dwParam1);

		if (pNotify->idPlayer == m_PlayerInfo.cid)
		{
			SetFactionID(pNotify->idFaction);
			SetFamilyID(pNotify->idFamily);
			SetFRoleID(pNotify->player_rank);
			
			if (GetFamilyID())
			{
				g_pGame->GetFactionMan()->GetFaction(GetFamilyID(), true, true);
				GNET::FamilyRecord rec(GetFamilyID(), GetCharacterID(), 0, std::vector<int>(), 0);
				g_pGame->GetGameSession()->SendNetData(rec);
			}
		}
		else
		{
			CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(pNotify->idPlayer);

			if (pPlayer)
			{
				pPlayer->SetFactionID(pNotify->idFaction);
				pPlayer->SetFamilyID(pNotify->idFamily);
				pPlayer->SetFRoleID(pNotify->player_rank);
			}
		}

		if (pNotify->idFaction)
			g_pGame->GetFactionMan()->GetFaction(pNotify->idFaction, true);

		break;

	case MAFIA_TRADE_START:

		break;

	case MAFIA_TRADE_END:

		break;
		
	case CHANGE_FACTION_NAME:
	{
		const cmd_change_faction_name *pCmd = (const cmd_change_faction_name*)(Msg.dwParam1);
		Faction_Info* pFaction = g_pGame->GetFactionMan()->GetFaction(pCmd->fid, false, pCmd->type>0);
		if (pFaction)
		{
			ACString strNameOrg = pFaction->GetName();
			pFaction->SetName(pCmd->name, pCmd->name_len);
			CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
			if (pPlayerMan->GetPlayer(pCmd->id))
			{
				if(pCmd->type==0)
					pPlayerMan->GetPlayer(pCmd->id)->SetFactionID(pCmd->fid);
				else
					pPlayerMan->GetPlayer(pCmd->id)->SetFamilyID(pCmd->fid);
			}

			int iCurId = pCmd->type>0 ? GetFamilyID() : GetFactionID();
			if( pCmd->fid==iCurId && strNameOrg!=pFaction->GetName() )
			{
				ACString strText;
				strText.Format(pGameUI->GetStringFromTable(pCmd->type>0?15032:15033), strNameOrg, pFaction->GetName());
				pGameUI->AddChatMessage(strText, GP_CHAT_MISC);
			}
		}
		break;
	}
		
    case PROTOCOL_FACTIONCREATE_RE:
		g_pGame->GetFactionMan()->OnFactionCreate(reinterpret_cast<const FactionCreate_Re*>(Msg.dwParam1));
		break;

    case PROTOCOL_FACTIONANNOUNCE_RE:
		g_pGame->GetFactionMan()->OnBroadcastNotice(reinterpret_cast<const FactionAnnounce_Re*>(Msg.dwParam1));
		break;

    case PROTOCOL_FACTIONUPGRADE_RE:
		g_pGame->GetFactionMan()->OnUpgrade(reinterpret_cast<const FactionUpgrade_Re*>(Msg.dwParam1));
		break;

	case PROTOCOL_FACTIONLIST_RE:
		g_pGame->GetFactionMan()->OnListMember(reinterpret_cast<FactionList_Re*>(Msg.dwParam1));
		break;
		
	case PROTOCOL_GETFACTIONBASEINFO_RE:
		g_pGame->GetFactionMan()->OnBaseInfo(reinterpret_cast<GetFactionBaseInfo_Re*>(Msg.dwParam1));
		break;

	case PROTOCOL_FACTIONDISMISS_RE:
		g_pGame->GetFactionMan()->OnDismiss(reinterpret_cast<FactionDismiss_Re*>(Msg.dwParam1));
		break;

	case RPC_FACTIONINVITE:
		g_pGame->GetFactionMan()->OnInviteJoin(reinterpret_cast<FactionInvite*>(Msg.dwParam1));
		break;

	case RPC_COMBATINVITE:
		g_pGame->GetFactionMan()->OnInviteCombat(reinterpret_cast<CombatInvite*>(Msg.dwParam1));
		break;
	case PROTOCOL_TERRITORYSCOREUPDATE:
		g_pGame->GetFactionMan()->OnUpdateTerritoryScore(reinterpret_cast<TerritoryScoreUpdate*>(Msg.dwParam1));
		break;
	case PROTOCOL_CHANGEFACTIONNAME_RE:
		g_pGame->GetFactionMan()->OnFactionChangeName(reinterpret_cast<ChangeFactionName_Re*>(Msg.dwParam1));
		break;
	case RPC_CRSSVRTEAMSINVITE:
		g_pGame->GetFactionMan()->OnCrssvrTeamsJoin(reinterpret_cast<CrssvrTeamsInvite*>(Msg.dwParam1));
		break;
	}
}

void CECHostPlayer::OnMsgHstSect(const ECMSG& Msg)
{
	using namespace S2C;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case RPC_SECTINVITE:
	{
		((CDlgTeacherMan*)pGameUI->GetDialog("Win_TeacherManage"))->OnSectInvite(reinterpret_cast<SectInvite*>(Msg.dwParam1));
		break;
	}
	}
}

void CECHostPlayer::OnMsgHstTaskDeliver(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case TASK_DELIVER_REP:
	{
		cmd_task_deliver_rep* pCmd = (cmd_task_deliver_rep*)Msg.dwParam1;
		SetReputation(pCmd->cur_reputaion);
		break;
	}
	case TASK_DELIVER_EXP:
	{
		cmd_task_deliver_exp* pCmd = (cmd_task_deliver_exp*)Msg.dwParam1;
		m_BasicProps.exp	+= pCmd->exp;
		m_BasicProps.deityExp += pCmd->deity_exp;

		if (pCmd->exp)
		{
			//	Print notify text
		//	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTEXP, (int)pCmd->exp);
			BubbleText(BUBBLE_EXP, pCmd->exp);
		}

		if (pCmd->sp > 0)
		{
			//	Print notify text
		//	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GOTSP, (int)pCmd->sp);
			BubbleText(BUBBLE_SP, pCmd->sp);
		}
		
		if (pCmd->deity_exp >0 )
		{
			BubbleText(BUBBLE_DEITY_EXP, pCmd->deity_exp);
		}

		break;
	}
	case TASK_DELIVER_MONEY:
	{
		cmd_task_deliver_money* pCmd = (cmd_task_deliver_money*)Msg.dwParam1;
		SetMoneyAmount(pCmd->cur_money);

		//	Print notify text
		ACHAR szPrice[256];
		CECIvtrItem::GetPriceText(szPrice, pCmd->amount);
		ACString strMsg;
		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		strMsg.Format(pStrTab->GetWideString(FIXMSG_PICKUPMONEY), szPrice);
		pGameUI->AddChatMessage(strMsg, CLIENT_CHAT_MONEY);
		BubbleText(BUBBLE_MONEY, pCmd->amount);
		break;
	}
	default:
	{
		ASSERT(0);
		return;
	}
	}
}

void CECHostPlayer::OnMsgHstItemIdentify(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_item_identify* pCmd = (cmd_item_identify*)Msg.dwParam1;
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->ServiceNotify(CECGameUIMan::NOTIFY_IDENTIFY_END, pCmd);
}

void CECHostPlayer::OnMsgHstSanctuary(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == ENTER_SANCTUARY)
	{
		m_bInSanctuary = true;

		//	Print a notify
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ENTERSHELTER);
	}
	else if (Msg.dwParam2 == LEAVE_SANCTUARY)
	{
		m_bInSanctuary = false;

		//	Print a notify
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEAVESHELTER);
		CECGameUIMan* pGameUI = (CECGameUIMan*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->CommonMessage("Win_Popmsg_SafeArea", "show", 0, 0);
	}
}

void CECHostPlayer::OnMsgHstCorrectPos(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_host_correct_pos* pCmd = (cmd_host_correct_pos*)Msg.dwParam1;

	SetPos(pCmd->pos);
	m_vVelocity.Clear();
	m_pCDRInfo->vAbsVelocity.Clear();
	ResetCDRInfo();
	ResetJump();	

	m_MoveCtrl.SetMoveStamp(pCmd->stamp);

	g_pGame->GetRTDebug()->OutputDebugInfo(RTDCOL_WARNING, _AL("correct pos !"));
}

void CECHostPlayer::OnMsgHstFriendOpt(const ECMSG& Msg)
{
	if (!m_pFriendMan)
		return;

	using namespace GNET;

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case PROTOCOL_ADDFRIEND_RE:
	{
		AddFriend_Re* p = (AddFriend_Re*)Msg.dwParam1;
		switch (p->retcode)
		{
		case ERR_SUCCESS:
		{
			//	If the same id player has been in our list, notify UI to delete
			//	it at first
			CECFriendMan::FRIEND* pFriend = m_pFriendMan->GetFriendByID(p->info.rid);
			if (pFriend)
				pGameUI->FriendAction(p->info.rid, -1, CDlgFriendList::FRIEND_ACTION_FRIEND_DELETE, 0);
			
			if (m_pFriendMan->AddFriend(p->info))
			{
				pGameUI->FriendAction(p->info.rid, p->info.gid, CDlgFriendList::FRIEND_ACTION_FRIEND_ADD, 0);
				g_pGame->GetGameSession()->get_friend_gt_status(GetCharacterID(), p->info.rid);
			}

			break;
		}
		case ERR_FS_OFFLINE:	pGameRun->AddFixedMessage(FIXMSG_OFFLINE2);			break;
		case ERR_FS_TIMEOUT:	pGameRun->AddFixedMessage(FIXMSG_FRIEND_OT);		break;
		case ERR_FS_REFUSE:		pGameRun->AddFixedMessage(FIXMSG_REFUSEFRIEND);		break;
		case ERR_FS_NOSPACE:	pGameRun->AddFixedMessage(FIXMSG_FRIENDLISTFULL);	break;
		}

		break;
	}
	case PROTOCOL_GETFRIENDS_RE:
	{
		GetFriends_Re* p = (GetFriends_Re*)Msg.dwParam1;
		m_pFriendMan->ResetFriends(*p);
		pGameUI->BuildFriendList();
		break;
	}
	case PROTOCOL_SETGROUPNAME_RE:
	{
		SetGroupName_Re* p = (SetGroupName_Re*)Msg.dwParam1;
		if (p->retcode == ERR_SUCCESS)
		{
			if (!p->name.size())	//	Delete group
			{
				m_pFriendMan->RemoveGroup(p->gid);
				pGameUI->FriendAction(0, p->gid, CDlgFriendList::FRIEND_ACTION_GROUP_DELETE, 0);
			}
			else
			{
				ACString strName((const ACHAR*)p->name.begin(), p->name.size() / sizeof (ACHAR));
				if (m_pFriendMan->GetGroupByID(p->gid))
				{
					//	Change group name
					m_pFriendMan->ChangeGroupName(p->gid, strName);
					pGameUI->FriendAction(0, p->gid, CDlgFriendList::FRIEND_ACTION_GROUP_RENAME, 0);
				}
				else	//	Create new group
				{
					m_pFriendMan->AddGroup(p->gid, strName);
					pGameUI->FriendAction(0, p->gid, CDlgFriendList::FRIEND_ACTION_GROUP_ADD, 0);
				}
			}
		}

		break;
	}
	case PROTOCOL_SETFRIENDGROUP_RE:
	{
		SetFriendGroup_Re* p = (SetFriendGroup_Re*)Msg.dwParam1;
		if (p->retcode == ERR_SUCCESS)
		{
			m_pFriendMan->ChangeFriendGroup(p->friendid, p->gid);
			pGameUI->FriendAction(p->friendid, p->gid, CDlgFriendList::FRIEND_ACTION_FRIEND_UPDATE, 0);
		}

		break;
	}
	case PROTOCOL_DELFRIEND_RE:
	{
		DelFriend_Re* p = (DelFriend_Re*)Msg.dwParam1;
		if (p->retcode == ERR_SUCCESS)
		{
			m_pFriendMan->RemoveFriend(p->friendid);
			pGameUI->FriendAction(p->friendid, -1, CDlgFriendList::FRIEND_ACTION_FRIEND_DELETE, 0);
		}

		break;
	}
	case PROTOCOL_FRIENDSTATUS:
	{
		FriendStatus* p = (FriendStatus*)Msg.dwParam1;
		m_pFriendMan->ChangeFriendStatus(p->roleid, p->lineid);
		pGameUI->FriendAction(p->roleid, -1, CDlgFriendList::FRIEND_ACTION_FRIEND_UPDATE, p->lineid);
		break;
	}
	case PROTOCOL_GETSAVEDMSG_RE:
	{
		GetSavedMsg_Re* p = (GetSavedMsg_Re*)Msg.dwParam1;
		if (p->retcode == ERR_SUCCESS)
			m_pFriendMan->ResetOfflineMsgs(*p);

		break;
	}
	case RPC_ADDFRIENDRQST:
	{
		AddFriendRqst* p = (AddFriendRqst*)Msg.dwParam1;
		AddFriendRqstArg* pArg = (AddFriendRqstArg*)p->GetArgument();
		
		// 如果请求的人在自己的黑名单中，直接无视
		EC_BLACKLIST_SETTING setting = g_pGame->GetConfigs()->GetBlackListSettings();
		int nCount = 0;
		while( nCount < EC_BLACKLIST_LEN && setting.idPlayer[nCount] > 0 )
		{
			if (pArg->srcroleid == setting.idPlayer[nCount])
				return;
			nCount++;
		}
		//	Get asker name and add it to player manager
		ACString strName((const ACHAR*)pArg->srcname.begin(), pArg->srcname.size() / sizeof (ACHAR));
		g_pGame->GetGameRun()->AddPlayerName(pArg->srcroleid, strName);

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->PopupFriendInviteMenu(pArg->srcroleid, strName, (DWORD)p);
		break;
	}
	case PROTOCOL_FRIENDNAMECHANGE:
	{
		FriendNameChange* p = (FriendNameChange*)Msg.dwParam1;
		ACString strName((const ACHAR*)p->newname.begin(), p->newname.size()/sizeof(ACHAR));
		CECFriendMan::FRIEND *pFriend = m_pFriendMan->GetFriendByID(p->roleid);
		if (pFriend)
		{
			pFriend->strName = strName;
		}
		g_pGame->GetGameRun()->AddPlayerName(p->roleid, strName, true);
		ACString strMsg, strOrgName((const ACHAR*)p->oldname.begin(), p->oldname.size()/sizeof(ACHAR));
		strMsg.Format(pGameUI->GetStringFromTable(15023), strOrgName, strName);
		pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_Friend_ChangeName", strMsg, 0x0fffffff, 0, 0, 0);
		pGameUI->BuildFriendList();
	}
	}
}

void CECHostPlayer::OnMsgHstWayPoint(const ECMSG& Msg)
{
	using namespace S2C;

	/*
	if (Msg.dwParam2 == ACTIVATE_WAYPOINT)
	{
		cmd_activate_waypoint* pCmd = (cmd_activate_waypoint*)Msg.dwParam1;
		m_aWayPoints.Add(pCmd->waypoint);

		//	Print a notify message
		const abase::vector<TRANS_TARGET>& aWayPoints = *globaldata_gettranstargets();
		for (size_t i=0; i < aWayPoints.size(); i++)
		{
			if (aWayPoints[i].id == (int)pCmd->waypoint)
			{
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_NEWWAYPOINT, aWayPoints[i].name);
				break;
			}
		}
	}
	else if (Msg.dwParam2 == WAYPOINT_LIST)
	{
		cmd_waypoint_list* pCmd = (cmd_waypoint_list*)Msg.dwParam1;

		m_aWayPoints.SetSize(pCmd->count, 16);
		for (size_t i=0; i < pCmd->count; i++)
			m_aWayPoints[i] = pCmd->list[i];
	}*/
}

void CECHostPlayer::OnMsgHstBreathData(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == BREATH_DATA)
	{
		cmd_breath_data* pCmd = (cmd_breath_data*)Msg.dwParam1;
		m_BreathData.bDiving	= true;
		m_BreathData.iCurBreath	= pCmd->breath;
		m_BreathData.iMaxBreath	= pCmd->breath_capacity;
	}
	else if (Msg.dwParam2 == HOST_STOP_DIVE)
	{
		m_BreathData.bDiving = false;
	}
}

void CECHostPlayer::OnMsgHstSkillAbility(const ECMSG& Msg)
{
	/*
	using namespace S2C;
	cmd_skill_ability* pCmd = (cmd_skill_ability*)Msg.dwParam1;
	GNET::ElementSkill::SetAbility(pCmd->skill_id, pCmd->skill_ability);
	*/
}

void CECHostPlayer::OnMsgHstCoolTimeData(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_cooltime_data* pCmd = (cmd_cooltime_data*)Msg.dwParam1;
	
	for (int i=0; i < pCmd->count; i++)
	{
		const cmd_cooltime_data::item_t& item = pCmd->list[i];
		if (item.idx > GP_CT_SKILL_START)
		{
			//	Is skill cool time
			CECSkill* pSkill = GetSkill(item.idx - GP_CT_SKILL_START);
			if (pSkill)
			{
				pSkill->StartCooling(item.max_cooltime, item.cooldown);
			}
			else
			{
				// 变身技能的冷却信息备份（变身技能是动态附加的,所以玩家刚登陆时没有对应的变身pSkill）
				m_TsSkillCoolDown[item.idx - GP_CT_SKILL_START] = item.cooldown;
				ASSERT(pSkill);
			}
		}
		else if (item.idx >= GP_CT_COMBINE_EDIT_START && item.idx <= GP_CT_COMBINE_EDIT_END)
		{
			COOLTIME& ct = m_aSkillEditCoolTimes[item.idx - GP_CT_COMBINE_EDIT_START];
			ct.iCurTime = item.cooldown;
			ct.iMaxTime	= item.max_cooltime;
			a_Clamp(ct.iCurTime, 0, ct.iMaxTime);
		}
		else if (item.idx >= GP_CT_PRODUCE_START && item.idx <= GP_CT_PRODUCE_END )
		{
			COOLTIME& ct = m_aProduceCoolTimes[item.idx - GP_CT_PRODUCE_START];
			ct.iCurTime = item.cooldown;
			ct.iMaxTime	= item.max_cooltime;
			a_Clamp(ct.iCurTime, 0, ct.iMaxTime);
		}
		else if (item.idx >= 0 && item.idx < GP_CT_MAX)
		{
			//	Other cool time
			COOLTIME& ct = m_aCoolTimes[item.idx];
			ct.iCurTime = item.cooldown;
			ct.iMaxTime	= item.max_cooltime;
			a_Clamp(ct.iCurTime, 0, ct.iMaxTime);
		}
		else
		{
			ASSERT(item.idx >= 0);
		}
	}
}

void CECHostPlayer::OnMsgHstRevivalInquire(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_revival_inquire* pCmd = (cmd_revival_inquire*)Msg.dwParam1;
	
	m_fReviveExp = pCmd->exp_reduce;
}

void CECHostPlayer::OnMsgHstSetCoolTime(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_set_cooldown* pCmd = (cmd_set_cooldown*)Msg.dwParam1;

	if (pCmd->cooldown_index < 0)
	{
		ASSERT(pCmd->cooldown_index >= 0);
		return;
	}

	if (pCmd->cooldown_index < GP_CT_MAX)
	{
		COOLTIME& ct = m_aCoolTimes[pCmd->cooldown_index];
		ct.iCurTime = pCmd->cooldown_time;
		ct.iMaxTime	= pCmd->cooldown_time;
		a_ClampRoof(ct.iCurTime, ct.iMaxTime);
	}
	else if (pCmd->cooldown_index > GP_CT_SKILL_START)
	{
		CECSkill* pSkill = GetSkill(pCmd->cooldown_index - GP_CT_SKILL_START);
		if (pSkill)
		{
			pSkill->StartCooling(pCmd->cooldown_time, pCmd->cooldown_time);
		}
		else
		{
			m_TsSkillCoolDown[pCmd->cooldown_index - GP_CT_SKILL_START] = pCmd->cooldown_time;
			ASSERT(pSkill);
		}
	}
	else if(pCmd->cooldown_index >= GP_CT_PRODUCE_START && pCmd->cooldown_index <= GP_CT_PRODUCE_END )
	{
		COOLTIME& ct = m_aProduceCoolTimes[pCmd->cooldown_index - GP_CT_PRODUCE_START];
		ct.iCurTime = pCmd->cooldown_time;
		ct.iMaxTime	= pCmd->cooldown_time;
		a_ClampRoof(ct.iCurTime, ct.iMaxTime);
	}
	else if(pCmd->cooldown_index >= GP_CT_COMBINE_EDIT_START && pCmd->cooldown_index <= GP_CT_COMBINE_EDIT_END)
	{
		COOLTIME& ct = m_aSkillEditCoolTimes[pCmd->cooldown_index - GP_CT_COMBINE_EDIT_START];
		ct.iCurTime = pCmd->cooldown_time;
		ct.iMaxTime	= pCmd->cooldown_time;
		a_ClampRoof(ct.iCurTime, ct.iMaxTime);
	}
	else
	{
//		ASSERT(0);
	}
}

void CECHostPlayer::OnMsgHstChangeTeamLeader(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_change_team_leader* pCmd = (cmd_change_team_leader*)Msg.dwParam1;

	if (!m_pTeam)
	{
		ASSERT(m_pTeam);
		return;
	}

	CECTeamMan* pTeamMan = g_pGame->GetGameRun()->GetTeamMan();
	pTeamMan->ChangeTeamLeader(m_pTeam->GetLeaderID(), pCmd->new_leader);
}

void CECHostPlayer::OnMsgHstExitInstance(const ECMSG& Msg)
{
	CECGameUIMan * pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	using namespace S2C;
	cmd_exit_instance * pCmd = (cmd_exit_instance *) Msg.dwParam1;

	if( !pCmd || pCmd->idInst != g_pGame->GetGameRun()->GetWorld()->GetInstanceID() )
		return;

	pGameUI->PopupInstanceCountDown(pCmd->iLeftTime);

	if( pCmd->iLeftTime > 0 )
		m_nTimeToExitInstance = pCmd->iLeftTime * 1000;
	else
		m_nTimeToExitInstance = 0;

	// 爬塔副本弹出提示信息
	if (CECBattleDungeonMan::getSingletonPtr()->IsInTower())
	{
		pGameUI->GetDialog("Win_Battle_Dungeon_Tower_Result")->Show(true);
		pGameUI->GetDialog("Win_Battle_Dungeon_Tower_LuckySkill")->Show(false);
		pGameUI->GetDialog("Win_Battle_Dungeon_Tower_Lucky")->Show(false);
	}

	//////////////////////////////////////////////////////////////////////////
	//2013-4-9 add 添加退出游戏提示
	if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR)
	{
		if(checkFlowBattleScore())
		{
			pGameUI->GetDialog("Win_Battle_SeniorPK_Exit01")->Show(true);
		}
		else
		{
			pGameUI->GetDialog("Win_Battle_SeniorPK_Exit02")->Show(true);
		}
	}
}

void CECHostPlayer::OnMsgHstChangeFace(const ECMSG& Msg)
{
}

void CECHostPlayer::OnMsgHstTeamMemberData(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_team_member_data* pCmd = (cmd_team_member_data*)Msg.dwParam1;
	ASSERT(pCmd);

	if (!m_pTeam || pCmd->idLeader != m_pTeam->GetLeaderID())
	{
		ASSERT(0);
		return;
	}

	//	If m_pTeam only has one member, it means the team is justed
	//	created, force to update team members' position
	if (m_pTeam->GetMemberNum() <= 1)
		m_TMPosCnt.Reset(true);

	//	Update team data
	m_pTeam->UpdateTeamData(*pCmd);

	AArray<int, int> aTemp;

	//	Ensure all member's info is ready
	for (int i=0; i < m_pTeam->GetMemberNum(); i++)
	{
		CECTeamMember* pMember = m_pTeam->GetMemberByIndex(i);
		if (pMember->IsInfoReady())
			continue;

		//	Can we find this player in current player list ?
		CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(pMember->GetCharacterID());
		if (pPlayer)
		{
			//	Ok, we use ready-made player information
			pMember->SetProfession(pPlayer->GetProfession());
			pMember->SetGender(pPlayer->GetGender());
			pMember->SetInfoReadyFlag(true);
		}
		else
		{
			//	Member's information isn't in local place, try to get
			//	from server
			aTemp.Add(pMember->GetCharacterID());
		}
	}

	//	Are there some member's info need updating ?
	if (aTemp.GetSize())
		g_pGame->GetGameSession()->GetPlayerBriefInfo(aTemp.GetSize(), aTemp.GetData(), 0);

	//	Update team UI
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->UpdateTeam();
}

void CECHostPlayer::OnMsgHstSetMoveStamp(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_set_move_stamp* pCmd = (cmd_set_move_stamp*)Msg.dwParam1;
	m_MoveCtrl.SetMoveStamp(pCmd->move_stamp);
}

void CECHostPlayer::OnMsgHstChatroomOpt(const ECMSG& Msg)
{
	using namespace GNET;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case PROTOCOL_CHATROOMCREATE_RE:
	{
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_CREATE_RE, (void *)Msg.dwParam1);
		break;
	}
	case PROTOCOL_CHATROOMINVITE:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_INVITE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMINVITE_RE:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_INVITE_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMJOIN_RE:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_JOIN_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMLEAVE:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_LEAVE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMEXPEL:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_EXPEL, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMSPEAK:
		pGameUI->ChannelAction(CDlgChannelChat::CHANNEL_ACTION_SPEAK, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_CHATROOMLIST_RE:
		pGameUI->UpdateChannelList((void *)Msg.dwParam1);
		break;
	}
}

void CECHostPlayer::OnMsgHstMailOpt(const ECMSG& Msg)
{
	using namespace GNET;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case PROTOCOL_ANNOUNCENEWMAIL:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_ANNOUNCENEWMAIL_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_GETMAILLIST_RE:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_GETMAILLIST_RE, (void *)Msg.dwParam1);
		break;
		
	case PROTOCOL_GETMAIL_RE:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_GETMAIL_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_GETMAILATTACHOBJ_RE:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_GETATTACH_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_DELETEMAIL_RE:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_DELETEMAIL_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_PRESERVEMAIL_RE:
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_PRESERVEMAIL_RE, (void *)Msg.dwParam1);
		break;

	case PROTOCOL_PLAYERSENDMAIL_RE:
	{
		PlayerSendMail_Re* p = (PlayerSendMail_Re*)Msg.dwParam1;
		if (p->attach_obj_pos >= 0 && p->attach_obj_pos < m_pPack->GetSize())
			FreezeItem(IVTRTYPE_PACK, p->attach_obj_pos, false, true);
		pGameUI->MailAction(CDlgMailList::MAIL_ACTION_SENDMAIL_RE, (void *)Msg.dwParam1);

		break;
	}
	}
}

void CECHostPlayer::OnMsgHstVendueOpt(const ECMSG& Msg)
{
	using namespace GNET;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case PROTOCOL_AUCTIONOPEN_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_OPEN_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONBID_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_BID_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONLIST_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_LIST_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONCLOSE_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_CLOSE_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONGET_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_GET_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONATTENDLIST_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_ATTENDLIST_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONEXITBID_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_EXITBID_RE, (void *)Msg.dwParam1);
		break;
	case PROTOCOL_AUCTIONGETITEM_RE:
		pGameUI->AuctionAction(CDlgAuctionList::AUCTION_ACTION_GETITEM_RE, (void *)Msg.dwParam1);
		break;
	}
}

void CECHostPlayer::OnMsgHstViewOtherEquips(const ECMSG& Msg)
{
	if (!m_pEPEquipPack)
		return;

	using namespace S2C;
	struct msgdata
	{
		size_t content_length;
		char content[1];
	};

	int i;
	cmd_player_equip_detail* pCmd = (cmd_player_equip_detail*)Msg.dwParam1;
	char* pHead = (char*)pCmd;
	msgdata* msg;
	pHead += sizeof(cmd_player_equip_detail);
	msg = (msgdata*)pHead;
	m_pEPEquipPack->ResetWithDetailData(0, (void*) &msg->content, msg->content_length);

	pHead += sizeof(size_t) + msg->content_length;
	msg = (msgdata*)pHead;
	cmd_skill_data* pSkillData = (cmd_skill_data*)msg->content;
	ASSERT(pCmd);
	for (i=0; i < m_aEPSkills.GetSize(); i++)
		delete m_aEPSkills[i];
	m_aEPSkills.RemoveAll(true);
	for (i=0; i < (int)pSkillData->skill_count; i++)
	{
		const cmd_skill_data::SKILL& Data = pSkillData->skill_list[i];
		CECSkill *pSkill = new CECSkill(Data.id_skill, Data.level);
		m_aEPSkills.Add(pSkill);
	}
	
	pHead += sizeof(size_t) + msg->content_length;
	msg = (msgdata*)pHead;
	m_pEPPetPack->ResetWithDetailData(0, (void*) &msg->content, msg->content_length);

	pHead += sizeof(size_t) + msg->content_length;
	msg = (msgdata*)pHead;
	m_pEPPetEquipPack->ResetWithDetailData(0, (void*) &msg->content, msg->content_length);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
#ifdef LUA_UI
	pGameUI->LuaEvent(LuaBind::DLG_SHOW_EPEQUIP, pCmd->id);
#else
	CDlgEPEquip *pDlg = (CDlgEPEquip *)pGameUI->GetDialog("Win_EPEquip");
	pDlg->UpdateEquip(pCmd->id);
#endif
}

void CECHostPlayer::OnMsgHstPariahTime(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_pariah_time* pCmd = (cmd_pariah_time*)Msg.dwParam1;
	m_dwPariahTime = pCmd->pariah_time * 1000;
}

void CECHostPlayer::OnMsgHstPetOpt(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case SUMMON_PET:
	{
		const cmd_summon_pet* pCmd = (const cmd_summon_pet*)Msg.dwParam1;
		m_nCurPetId = pCmd->pet_id;
		m_nCurPetIndex = pCmd->pet_index;
		CDlgPetAction::ClearAutoCastID();
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			m_nCurPetMaxHp = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_HP].cur_value;
			m_nCurPetMaxVp = pPet->GetEssence().attr[itemdataman::_pet_bedge_essence::MAX_VIGOR].cur_value;
		}
		break;
	}
	case RECALL_PET:
	{
		m_nCurPetIndex = -1;
		m_nCurPetId = 0;
		break;
	}
	case PLAYER_START_PET_OP:
	{
		const cmd_player_start_pet_op* pCmd = (const cmd_player_start_pet_op*)Msg.dwParam1;
		int iDoWhat;
		if (pCmd->op == 0)
			iDoWhat = CECHPWorkConcentrate::DO_SUMMONPET;
		else if (pCmd->op == 1)
			iDoWhat = CECHPWorkConcentrate::DO_RECALLPET;
		else if (pCmd->op == 2)
			iDoWhat = CECHPWorkConcentrate::DO_COMBINE1;
		else if (pCmd->op == 3)
			iDoWhat = CECHPWorkConcentrate::DO_UNCOMBINE1;
		else if (pCmd->op == 4)
			iDoWhat = CECHPWorkConcentrate::DO_COMBINE2;
		else if (pCmd->op == 5)
			iDoWhat = CECHPWorkConcentrate::DO_UNCOMBINE2;
		else if (pCmd->op == 6)
			iDoWhat = CECHPWorkConcentrate::DO_FREE;
		else if (pCmd->op == 7)
		{
			iDoWhat = CECHPWorkConcentrate::DO_ADOPT;
			LuaEvent::FirstTamePet();
		}
		else if (pCmd->op == 8)
			iDoWhat = CECHPWorkConcentrate::DO_REFINE;		

		m_PetOptCnt.SetPeriod(pCmd->delay * 50);
		m_PetOptCnt.Reset();

		CECHPWorkConcentrate* pWork = (CECHPWorkConcentrate*)m_pWorkMan->CreateWork(CECHPWork::WORK_CONCENTRATE);
		pWork->SetDoWhat(iDoWhat);
		m_pWorkMan->StartWork_p1(pWork);
		break;
	}
	case PLAYER_STOP_PET_OP:
	{
		if (IsOperatingPet())
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_CONCENTRATE, false);

		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->CommonMessage("Win_Pet_Detail", "continue_opt", 0, 0);
		break;
	}
	case PET_RECEIVE_EXP:
	{
		const cmd_pet_receive_exp* pCmd = (const cmd_pet_receive_exp*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().exp = pCmd->exp;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PET_LEVELUP:
	{
		const cmd_pet_levelup* pCmd = (const cmd_pet_levelup*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().level = pCmd->level;
			pPet->GetEssence().exp = pCmd->exp;
			pPet->UpdateInfoMD5();
			LuaEvent::FirstPetLvUp();
		}

		break;
	}
	case PET_HONOR_POINT:
	{
		const cmd_pet_honor_point* pCmd = (const cmd_pet_honor_point*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().cur_honor_point = pCmd->cur_honor_point;
			pPet->GetEssence().max_honor_point = pCmd->max_honor_point;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PET_HUNGER_GAUGE:
	{
		const cmd_pet_hunger_gauge* pCmd = (const cmd_pet_hunger_gauge*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().cur_hunger_point = pCmd->cur_hunge_gauge;
			pPet->GetEssence().cur_hunger_point = pCmd->max_hunge_point;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case EQUIP_PET_BEDGE:
	{
		const equip_pet_bedge* pCmd = (const equip_pet_bedge*)Msg.dwParam1;
		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->inv_index, true);
		CECIvtrItem* pItem2 = m_pPetPack->GetItem(pCmd->pet_index, true);
		m_pPack->SetItem(pCmd->inv_index, pItem2);
		m_pPetPack->SetItem(pCmd->pet_index, pItem1);

		//	Update shortcuts
		if (pItem1)
			UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->inv_index);
		LuaEvent::FirstUsePet();
		break;
	}
	case PLAYER_MOVE_PET_BEDGE_ITEM:
	{
		const player_move_pet_bedge* pCmd = (player_move_pet_bedge*)Msg.dwParam1;
		m_pPetPack->ExchangeItem(pCmd->src_index, pCmd->dst_index);

		//	Update shortcuts
		CECIvtrItem* pItem1 = m_pPetPack->GetItem(pCmd->src_index);
		CECIvtrItem* pItem2 = m_pPetPack->GetItem(pCmd->dst_index);

		if (pItem1 && pItem2)
			UpdateExchangedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PET, pCmd->dst_index, pItem2->GetTemplateID(), IVTRTYPE_PET, pCmd->src_index);
		else if (pItem1)
			UpdateMovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PET, pCmd->dst_index, IVTRTYPE_PET, pCmd->src_index);
		else if (pItem2)
			UpdateMovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_PET, pCmd->src_index, IVTRTYPE_PET, pCmd->dst_index);
		break;
	}
	case EQUIP_PET_EQUIP:
	{
		const equip_pet_equip* pCmd = (const equip_pet_equip*)Msg.dwParam1;
		CECIvtrItem* pItem1 = m_pPack->GetItem(pCmd->inv_index, true);
		CECIvtrItem* pItem2 = m_pPetEquipPack->GetItem(pCmd->pet_index, true);
		m_pPack->SetItem(pCmd->inv_index, pItem2);
		m_pPetEquipPack->SetItem(pCmd->pet_index, pItem1);

		//	Update shortcuts
		if (pItem1)
			UpdateRemovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->inv_index);
//		LuaEvent::FirstEquipPet();
		break;
	}
	case PLAYER_SET_PET_STATUS:
	{
		const player_set_pet_status* pCmd = (const player_set_pet_status*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().main_status = pCmd->main_status;
			pPet->GetEssence().sub_status = pCmd->sub_status;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PLAYER_SET_PET_RANK:
	{
		const player_set_pet_rank* pCmd = (const player_set_pet_rank*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().rank = pCmd->rank;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PLAYER_COMBINE_PET:
	{
		const player_combine_pet* pCmd = (const player_combine_pet*)Msg.dwParam1;
		m_nPetCombinedType = pCmd->type;
		LuaEvent::FirstRefinePet();
		break;
	}
	case PLAYER_UNCOMBINE_PET:
		m_nPetCombinedType = -1;
		break;
	case PLAYER_PET_HP_NOTIFY:
	{
		const player_pet_hp_notify* pCmd = (const player_pet_hp_notify*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().cur_hp = pCmd->cur_hp;
			pPet->GetEssence().cur_vigor = pCmd->cur_vp;
			m_nCurPetMaxHp = pCmd->max_hp;
			m_nCurPetMaxVp = pCmd->max_vp;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PLAYER_PET_AGE_LIFE:
	{
		const player_pet_age_life* pCmd = (const player_pet_age_life*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().life = pCmd->life;
			pPet->GetEssence().age = pCmd->age;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PLAYER_PET_DEAD:
	{
		const player_pet_dead* pCmd = (const player_pet_dead*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		{
			pPet->GetEssence().cur_hp = 0;
			pPet->UpdateInfoMD5();
		}

		break;
	}
	case PLAYER_PET_AI_STATE:
	{
		const player_pet_ai_state* pCmd = (const player_pet_ai_state*)Msg.dwParam1;
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if( pGameUI )
			CDlgPetAction::SetPetAIState(pCmd->stay_state, pCmd->aggro_state);

		break;
	}
	case PLAYER_PET_SET_AUTO_SKILL:
	{
		const player_pet_set_auto_skill* pCmd = (const player_pet_set_auto_skill*)Msg.dwParam1;
		CDlgPetAction::SetAutoCastID(pCmd->skill_id, pCmd->set_flag);

		break;
	}
	case PLAYER_PET_SET_SKILL_COOLDOWN:
	{
		const player_pet_set_skill_cooldown* pCmd = (const player_pet_set_skill_cooldown*)Msg.dwParam1;
		CECInventory* pPetPack = GetPack(IVTRTYPE_PET);
		CECPetBedge* pPet = (CECPetBedge*)pPetPack->GetItem(pCmd->pet_index);

		if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
			pPet->SetSkillCoolTime(pCmd->cooldown_index, pCmd->cooldown_time);

		break;
	}
	case PET_ROOM:
		break;
	case PET_ROOM_CAPACITY:
	{
		const cmd_pet_room_capacity* p = (const cmd_pet_room_capacity*)Msg.dwParam1;
		m_pPetPack->Resize(p->capacity);
		break;
	}
	default:
		ASSERT(0);
		break;
	}
}

void CECHostPlayer::OnMsgHstBattleOpt(const ECMSG& Msg)
{
	using namespace GNET;
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strText;

	switch (Msg.dwParam2)
	{
	case PROTOCOL_BATTLEGETLIST_RE:
	{
		BattleGetList_Re *p = (BattleGetList_Re*)(Msg.dwParam1);
		switch(p->battle_type)
		{
		case BATTLE_TYPE_NORMAL:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList"))->UpdateBattleList((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_CRSSVR:
			{
				g_pGame->GetGameSession()->battle_join(g_pGame->GetGameRun()->GetCurLine(), p->fields[0].map_id, ((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList"))->GetJoinType());
				break;
			}
		case BATTLE_TYPE_NAMED_CRSSVR:
			{
				g_pGame->GetGameSession()->battle_join(g_pGame->GetGameRun()->GetCurLine(), p->fields[0].map_id, ((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList"))->GetJoinType());
				break;
			}
		case BATTLE_TYPE_REBORN:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList2"))->UpdateBattleList((void*)Msg.dwParam1, BATTLE_TYPE_REBORN);
			break;
		case BATTLE_TYPE_SONJIN:
		case BATTLE_TYPE_SONJINREBORN:
			((CDlgBattleSong*)pGameUI->GetDialog("Win_BattleSong"))->UpdateBattleList((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_ARENA:
		case BATTLE_TYPE_ARENAREBORN:
			((CDlgBattlePk*)pGameUI->GetDialog("Win_BattleListPK"))->UpdatePkBattleList((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_CHALLENGE:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList2"))->UpdateBattleList((void*)Msg.dwParam1, BATTLE_TYPE_CHALLENGE);
			break;
		case BATTLE_TYPE_CRSSVRTEAM:
			{
				g_pGame->GetGameSession()->battle_join(g_pGame->GetGameRun()->GetCurLine(), p->fields[0].map_id, ((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleList"))->GetJoinType());
				break;
			}
		case BATTLE_TYPE_FLPW_CRSSVR:
			{
				//((DlgBattleFlowInfoApply*)pGameUI->GetDialog("Dialog_FlowBattle"))->UpDateFolwInfoApply();
			}
			break;
		}
		break;
	}
	case PROTOCOL_BATTLEGETFIELD_RE:
	{
		BattleGetField_Re * p =( BattleGetField_Re*)(Msg.dwParam1);
		switch(p->battle_type)
		{
		case BATTLE_TYPE_NORMAL:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleInfo"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_CRSSVR:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleInfo"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_NAMED_CRSSVR:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleInfo"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_REBORN:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleInfo2"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_SONJIN:
			((CDlgBattleSong*)pGameUI->GetDialog("Win_BattleSongTime"))->UpdateSongBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_SONJINREBORN:
			((CDlgBattleSong*)pGameUI->GetDialog("Win_BattleSongTime"))->UpdateSongBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_ARENA:
			((CDlgBattlePk*)pGameUI->GetDialog("Win_BattleListPKAwait"))->UpdatePkBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_ARENAREBORN:
			((CDlgBattlePk*)pGameUI->GetDialog("Win_BattleListPKAwait"))->UpdatePkBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_CHALLENGE:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleInfo_Challenge"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_CRSSVRTEAM:
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_Battle6v6"))->UpdateBattleInfo((void*)Msg.dwParam1);
			break;
		case BATTLE_TYPE_FLPW_CRSSVR:
			{
			//	((DlgBattleFlowInfoApply*)pGameUI->GetDialog("Win_Battle_SeniorPK_Score"))->UpdateFolwBattleInfo((void*)Msg.dwParam1);
			}
			break;
		}	
		break;
	}
	case PROTOCOL_BATTLEENTER_RE:
	{
		int retcode =  ((BattleEnter_Re*)Msg.dwParam1)->retcode;
		if( ((BattleEnter_Re*)Msg.dwParam1)->retcode != ERR_SUCCESS )
		{
			if(ERR_BATTLE_ENTER_NOT_IN_JOINROOM == ((BattleEnter_Re*)Msg.dwParam1)->retcode)
			{
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(19953), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
			else
			{
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(920), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
		}
		else
		{
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(CDlgHost::BATTLE_STATE_INBATTLE, ((BattleEnter_Re*)Msg.dwParam1)->endtime,0,((BattleEnter_Re*)Msg.dwParam1)->iscross);
			m_iApplyBattleType = ((BattleEnter_Re*)Msg.dwParam1)->battle_type;     // 所在的战场
		}
		break;
	}
	case PROTOCOL_BATTLEJOIN_RE:
	{

		BattleJoin_Re *p = (BattleJoin_Re*)Msg.dwParam1;
		if( p->retcode == ERR_SUCCESS)
		{
			pGameUI->MessageBox("", pGameUI->GetStringFromTable(919), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(
				CDlgHost::BATTLE_STATE_INTEAM, p->starttime, p->gs_id, p->iscross,p->map_id);
			m_iApplyBattleType = p->battle_type;
		}
		else if(ERR_BATTLE_CROSS == p->retcode)	// 2012-8-23 add by zy 从跨服返回是有跨服战场信息,不要要弹出提示框
		{
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(
				CDlgHost::BATTLE_STATE_INTEAM, p->starttime, p->gs_id, p->iscross,p->map_id);
			m_iApplyBattleType = p->battle_type;
		}
		else
		{	
			if( p->retcode == ERR_BATTLE_TEAM_FULL )
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(910), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_JOIN_ALREADY )
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(916), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_MAP_NOTEXIST )
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(915), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_COOLDOWN )
			{
				ACString strText;
				strText.Format(pGameUI->GetStringFromTable(917), p->cooltime/60, p->cooltime%60);
				pGameUI->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			}
			else if( p->retcode == ERR_BATTLE_LEVEL_LIMIT )
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(918), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_REBORN_LIMIT)
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(7327), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_ENTER_MAXTIMES)
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(7328), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_STARTED)
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(7705), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_STARTED10)
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(7706), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			else if( p->retcode == ERR_BATTLE_ENTER_NOT_IN_JOINROOM)
				pGameUI->MessageBox("", pGameUI->GetStringFromTable(19953), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
		break;
	}
	case PROTOCOL_BATTLELEAVE_RE:
	{
		if (GetRebornInfo().size()>0)
		{
			pGameUI->GetDialog("Win_BattleInfo2")->Show(false);
		}
		else
			pGameUI->GetDialog("Win_BattleInfo")->Show(false);
		pGameUI->GetDialog("Win_BattleListPKAwait")->Show(false);
		pGameUI->GetDialog("Win_BattleRaidStage")->Show(false);
		pGameUI->GetDialog("Win_BattleRaidMiniHint")->Show(false);
		((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);

		m_iApplyBattleType = -1; 
		
		break;
	}
	case PROTOCOL_BATTLESTARTNOTIFY:
	{
		BattleStartNotify* p = (BattleStartNotify*)Msg.dwParam1;
		if(p->start_result == 0)
		{		
			PAUIDIALOG pMsgBox;
			strText.Format(pGameUI->GetStringFromTable(914), pGameUI->GetStringFromTable(1900 + ((BattleStartNotify*)Msg.dwParam1)->map_id));
			pGameUI->MessageBox("Game_BattleEnter", strText, MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetData(((BattleStartNotify*)Msg.dwParam1)->map_id);
		}
		else if(p->start_result == 1)
		{
			pGameUI->MessageBox("", pGameUI->GetStringFromTable(8455), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));			
		}
		// 2012-8-12 add by zy 
		((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(CDlgHost::BATTLE_STATE_NORMAL, 0,0,p->iscross);

		break;
	}
	case HOST_ENTER_BATTLE:
	{
		cmd_host_enter_battle* p = (cmd_host_enter_battle*)Msg.dwParam1;
		m_iBattleCamp = p->battle_faction;
		m_iBattleType = p->battle_type;
		if(m_iBattleType == BATTLE_TYPE_SONJIN || m_iBattleType == BATTLE_TYPE_SONJINREBORN)
		{
			//默认的将刚进入竞技场的主角的军衔设置为1
			SetMilitaryRank(1);
		}
		// 玩家进入竞技场中，将玩家自身的装备和隐藏掩码备份
		if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
		{	
			m_dwHideEquipMaskBackUp = m_dwHideEquipMask;
			m_bFashionModeBackUp	= m_bFashionMode;
			m_bFashionMode			= false;   // 强制为装备模式
			m_dwHideEquipMask = 0;
			int aEquips[SIZE_EQUIPIVTR];
			memset(aEquips, 0, sizeof(aEquips));
			int index = 0;
			CECPlayer::SECT_LEVEL2_INFO* pInfo = CECPlayer::GetSectInfoByProf(GetProfession());
			for (int i= 0; i< NUM_SECT; ++i)
			{
				if(pInfo && (pInfo->mask_sect & (1<<i)) )
				{	
					index = i;
				}
			}
			index = index * 2 + m_iGender;
			ASSERT(index>=0 && index < 30);
			memcpy(aEquips, m_EquipsInArena[index], sizeof(m_EquipsInArena[index]));
			
			ShowEquipments(aEquips, true, true);

			// 关闭聊天框
			CDlgChat *pDlgChat = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->m_pDlgChat;

			PAUIEDITBOX pEdt = (PAUIEDITBOX)pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");
			if (pEdt)
			{
				pEdt->Enable(false);
			}
		}
		if (m_iBattleType == BATTLE_TYPE_CRSSVR || m_iBattleType == BATTLE_TYPE_NAMED_CRSSVR)
		{
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_PKScoreHover2013")->Show(true);
		}
		else if (m_iBattleType == BATTLE_TYPE_CRSSVRTEAM)
		{
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardS")->GetDlgItem("IDCANCEL")->Show(false);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardS")->SetCanEscClose(false);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardL")->GetDlgItem("IDCANCEL")->Show(false);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardL")->SetCanEscClose(false);
			if (!pGameUI->GetDialog("Win_Battle6V6ScoreBoardS")->IsShow() && !pGameUI->GetDialog("Win_Battle6V6ScoreBoardL")->IsShow())
				pGameUI->GetDialog("Win_Battle6V6ScoreBoardS")->Show(true);
		}
		else if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR)
		{
			PAUIDIALOG	pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_ScoreMin");
			pDlg->Show(true);
			pDlg->SetCanMove(true);
			if (pDlg)
			{
				pDlg->AlignTo(g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_MiniMap"),
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_CENTER);
			}
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_Score")->Show(false);
			SetTitle(0);
		}
		break;
	}

	case BATTLE_LEAVE:
		{
			m_iBattleCamp = 0;
			// 主角离开竞技场之后，重新恢复原来的装备显示
			if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
			{
				m_dwHideEquipMask = m_dwHideEquipMaskBackUp;
				m_bFashionMode = m_bFashionModeBackUp;
				UpdateEquipSkins(true);
				
				//			CDlgChat *pDlgChatS = (CDlgChat*)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_ChatClose");
				//			pDlgChatS->GetDlgItem("Btn_Close")->Enable(true);
				//			pDlgChatS->OnCommand_close("");
				CDlgChat *pDlgChat = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->m_pDlgChat;
				PAUIEDITBOX pEdt = (PAUIEDITBOX)pDlgChat->GetDlgItem("DEFAULT_Txt_Speech");
				if (pEdt)
				{
					pEdt->Enable(true);
				}
			}
			if (m_iBattleType==BATTLE_TYPE_INSTANCING)
			{
				((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);
				m_iBattleInstanceState = 0;
			}
			else
			{
				((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetBattleInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);
				m_iApplyBattleType = -1; 
			}
			
			if (m_iBattleType==BATTLE_TYPE_FLPW_CRSSVR)
			{
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_ScoreMin")->Show(false);
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_Score")->Show(false);
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_Exit01")->Show(false);
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle_SeniorPK_Exit02")->Show(false);
				g_pGame->GetGameRun()->GetHostPlayer()->ClearFlowBattle();
			}
			m_iBattleType = -1;
			((CDlgRevive*)pGameUI->GetDialog("Win_Message4"))->BattleOut();
			pGameUI->GetDialog("Win_PKScoreHover2013")->Show(false);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardS")->GetDlgItem("IDCANCEL")->Show(true);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardS")->SetCanEscClose(true);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardL")->GetDlgItem("IDCANCEL")->Show(true);
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_Battle6V6ScoreBoardL")->SetCanEscClose(true);


			break;
		}
		
	case BATTLE_RESULT:
		{
			if(m_iBattleType == BATTLE_TYPE_NAMED_CRSSVR)
			{
				cmd_battle_result* p = (cmd_battle_result*)Msg.dwParam1;

				PAUIDIALOG pDlg = pGameUI->GetDialog("Win_PkScore2013");
				ACString strText;
				strText.Format(_AL("%d"),p->attacker_score);
				PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_ScoreA");
				pLab->SetText(strText);
				strText.Format(_AL("%d"),p->defender_score);
				pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_ScoreB");
				pLab->SetText(strText);
				pLab = (PAUILABEL)pDlg->GetDlgItem("Title");
				pLab->SetText(pGameUI->GetStringFromTable(8477+p->result));
				((CDlgBattleInfo*)pGameUI->GetDialog("Win_PkScore2013"))->m_bisBattleUIType = false;
				pDlg->Show(true);
			}
			else if(m_iBattleType == BATTLE_TYPE_CRSSVR)
			{
				cmd_battle_result* p = (cmd_battle_result*)Msg.dwParam1;
				PAUIDIALOG pDlg = pGameUI->GetDialog("Win_PKRecord");
				ACString strText;
				strText.Format(_AL("%d:%d"),p->attacker_score, p->defender_score);
				PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Score");
				pLab->SetText(strText);
				pLab = (PAUILABEL)pDlg->GetDlgItem("Lab_Victor");
				pLab->SetText(pGameUI->GetStringFromTable(8477+p->result));
				pDlg->Show(true);
			}
			break;
		}
		
	case BATTLE_INFO:
		{
			cmd_battle_info info;
			
			if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			
			if (m_iBattleType == BATTLE_TYPE_CRSSVRTEAM)
			{
				((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleResult6v6"))->UpdateBattleScorePK2011(&info);	
			}
// 			else if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR)
// 			{
// 				((DlgBattleFlowInfoMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_Score"))->UpdateFolwBattleInfo(&info);
// 			}
			else if (m_iBattleType == BATTLE_TYPE_NAMED_CRSSVR)
			{
				((CDlgBattleInfo*)pGameUI->GetDialog("Win_PkScore2013"))->UpdateBattleScore(&info);
				((CDlgBattleInfo*)pGameUI->GetDialog("Win_PkScore2013"))->m_bisBattleUIType = true;
				pGameUI->GetDialog("Win_PkScore2013")->Show(true);
			}
			else
			{
				((CDlgBattleInfo*)pGameUI->GetDialog("Win_BattleScore"))->UpdateBattleScore(&info);
			}
			break;
		}
	case BATTLE_SCORE:
		{
			battle_score* p = (battle_score*)Msg.dwParam1;
			m_nBattleScore = p->score;
			
			break;
		}
	case GAIN_BATTLE_SCORE:
		{
			gain_battle_score* p = (gain_battle_score*)Msg.dwParam1;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_BATTLESCORE_INC, p->inc);
			m_nBattleScore += p->inc;
			break;
		}
		
	case SPEND_BATTLE_SCORE:
		{
			spend_battle_score* p = (spend_battle_score*)Msg.dwParam1;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_BATTLESCORE_DEC, p->dec);
			m_nBattleScore -= p->dec;
			break;
		}
		
	case BATTLE_PLAYER_INFO:
		break;
		
	case SELF_SJBATTLE_INFO:
		{
			self_sjbattle_info* p = (self_sjbattle_info*)Msg.dwParam1;
			((CDlgBattleSong*)pGameUI->GetDialog("Win_BattleSongScore"))->UpdateSongSelfInfo(p);
			m_nSJBattleScore = p->battle_score;
			break;
		}
	case GET_SJBATTLE_INFO:
		{
			cmd_sjbattle_info info;
			if(!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			
			//界面加上宋金战场排行榜	
			((CDlgBattleSong*)pGameUI->GetDialog("Win_BattleSongScore"))->UpdateSongBattleScore(&info);
			break;
		}
	case GET_ARENA_INFO:
		{
			arena_info info;
			if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			
			((CDlgBattlePk*)pGameUI->GetDialog("Win_BattleScorePK"))->UpdatePkBattleScore(&info);
			break;
		}
	case SJBATTLE_SCORE_CHANGE:
		{
			sjbattle_score_change* p = (sjbattle_score_change*)Msg.dwParam1;
			ASSERT(m_nSJBattleScore == p->old_score);
			m_nSJBattleScore = p->new_score;
			break;
		}
	case CROSSSERVER_BATTLE_SCORE:
		{
			crossserver_battle_score* p = (crossserver_battle_score*)Msg.dwParam1;
			
			((CDlgBattleInfo*)pGameUI->GetDialog("Win_PKScoreHover2013"))->UpdataCrossServerBattleScore(p);

			((CDlgBattleInfo*)pGameUI->GetDialog("Win_PkScore2013"))->UpDataPKScoreBattleInfo(p);
			
			break;
		}
	case CRSSVR_TEAM_BATTLE_INFO:
		{
			//添加收到跨服小队PK战场信息后的处理代码
			crossserver_team_battle_info* p = (crossserver_team_battle_info*)Msg.dwParam1;
			((CDlgBattlePkScore*)pGameUI->GetDialog("Win_Battle6V6ScoreBoardS"))->UpdateBattleScore2011OnLine(p);
			((CDlgBattlePkScore*)pGameUI->GetDialog("Win_Battle6V6ScoreBoardL"))->UpdateBattleScore2011OnLine(p);
			break;
		}
	case CSFLOW_BATTLE_INFO:
		{
			flow_battle_info info;
			
			if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			if(m_iBattleType == BATTLE_TYPE_FLPW_CRSSVR)
			{
				((DlgBattleFlowInfoMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_Score"))->UpdateFlowBattleInfo(&info);
			}
		}
		break;
	}
}

void CECHostPlayer::OnMsgHstInstancingOpt(const ECMSG& Msg)
{
	using namespace GNET;
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strText;

	switch (Msg.dwParam2)
	{
		
	case PROTOCOL_INSTANCINGAVAILABLELIST_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaid"))->UpdateBattleList((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGGETLIST_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaid"))->UpdateBattleListInfo((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGGETFIELD_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingGetFieldRe((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGKICK_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingKickRe((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGSTART_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingStartRe((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGAPPOINT_RE:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingAppointRe((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGAPPOINTNOTIFY:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingAppointNotifyAction((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGJOINREQ:
		{
			m_iBattleInstanceState = 1;
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingJoinReqAction((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGJOIN_RE:
		{
			m_iBattleInstanceState = 1;
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingJoinRe((void*)Msg.dwParam1);
			break;
		}
	case PROTOCOL_INSTANCINGLEAVE_RE:
		{
			m_iBattleInstanceState = 0;
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);
			pGameUI->GetDialog("Win_BattleRaidApliList")->Show(false);
			pGameUI->GetDialog("Win_BattleRaidStage")->Show(false);
			pGameUI->GetDialog("Win_BattleRaidMiniHint")->Show(false);
			break;
		}
	case PROTOCOL_INSTANCINGENTER_RE:
		{
			pGameUI->GetDialog("Win_BattleRaidApliList")->Show(false);
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingEnterRe((void*)Msg.dwParam1);
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_INBATTLE, ((InstancingEnter_Re*)Msg.dwParam1)->endtime);
			m_iBattleInstanceState = 1;     // 所在的战场
			break;
		}
	case PROTOCOL_INSTANCINGSTARTNOTIFY:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingStartNotifyAction((void*)Msg.dwParam1);
			break;
		}
    case PROTOCOL_INSTANCINGCLOSENOTIFY:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->InstancingCloseNotifyAction((void*)Msg.dwParam1);
			m_iBattleInstanceState = 0;
			break;
		}
	case PROTOCOL_INSTANCINGKICKNOTIFY:
		{
			pGameUI->MessageBox("",pGameUI->GetStringFromTable(8587), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			((CDlgHost*)pGameUI->GetDialog("Win_CharHead"))->SetInstanceInfo(CDlgHost::BATTLE_STATE_NORMAL, 0);
			pGameUI->GetDialog("Win_BattleRaidApliList")->Show(false);
			break;
		}		
	case PROTOCOL_INSTANCINGACCEPT_RE:
		{
			g_pGame->GetGameSession()->instancing_getfield(g_pGame->GetGameRun()->GetCurLine(), ((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidApliList"))->GetData());
			break;
		}
	case SELF_INSTANCING_BATTLE_INFO:
		{
			const self_instancing_battle_info* pCmd = (self_instancing_battle_info*)Msg.dwParam1;
			ACString strText, strTemp;
			strTemp.Format(pGameUI->GetStringFromTable(8580), pCmd->level_score);
			strText += strTemp;
			strText += _AL("\r");
			strTemp.Format(pGameUI->GetStringFromTable(8581), pCmd->monster_score);
			strText += strTemp;
			strText += _AL("\r");
			strTemp.Format(pGameUI->GetStringFromTable(8582), pCmd->time_score);
			strText += strTemp;
			strText += _AL("\r");
			strTemp.Format(pGameUI->GetStringFromTable(8583), pCmd->death_penalty_score);
			strText += strTemp;
			strText += _AL("\r");
			strTemp.Format(pGameUI->GetStringFromTable(8584), pCmd->level_score+pCmd->monster_score+pCmd->time_score+pCmd->death_penalty_score);
			strText += strTemp;
			PAUITEXTAREA pTxtScore = (PAUITEXTAREA)((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidStage"))->GetDlgItem("Txt_Score");
			pTxtScore->SetText(strText);
			CDlgBattleInstance::SetScore(pCmd->level_score+pCmd->monster_score+pCmd->time_score+pCmd->death_penalty_score);
			break;
		}
	case INSTANCING_BATTLE_INFO:
		{
			((CDlgBattleInstance*)pGameUI->GetDialog("Win_BattleRaidStage"))->InstancingInfo((void*)Msg.dwParam1);
			break;
		}
	}
}

void CECHostPlayer::OnMsgHstRaidOpt( const ECMSG& Msg )
{
	using namespace GNET;
	using namespace S2C;
	//TODO: UI相关处理逻辑的接口可以在此处理执行
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
		case PROTOCOL_RAIDJOINREQ:	// deliver发给副本房主关于申请人申请加入副本房间的请求
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "join_req", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDOPEN_RE:	// 开启副本房间
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "open_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDJOIN_RE:	// 报名加入副本房间
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "join_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDJOINAPPLYLIST:	// 条件不满足，申请加入副本房间失败时
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "join_apply", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDJOINACCEPT_RE:	// 房主接受申请者的请求后，delivery返回的协议
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "join_accept_success", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDENTER_RE:		// 进入副本
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "enter_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDGETLIST_RE:	// 获得副本当前信息列表
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApply", "getlist_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDSTART_RE:		// 通知房主开启副本
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "start_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDAPPOINT_RE:	// 通知之前的老团长：副本团长已转移
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "appoint_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDAPPOINTNOTIFY:	// 通知其他人：副本团长已转移
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "appoint_notify", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDSTARTNOTIFY:		// 通知所有成员：副本开启
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "start_notify", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDKICK_RE:		// 通知团长踢人情况
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "kick_re", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDKICKNOTIFY:	// 通知所有人，踢人情况
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "kick_notify", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDQUIT_RE:		// 玩家不同意进入副本
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "refuse_enter", Msg.dwParam1, 0);
			break;
		}
		case PROTOCOL_RAIDGETROOM_RE:	// 返回关于某副本房间的相关信息（申请人列表，成员列表）
		{
			pGameUI->CommonMessage("Win_Battle_DungeonApplyList", "room_info", Msg.dwParam1, 0);
			break;
		}
		case ENTER_RAID_COUNT:			// gs通知客户端玩家进入各副本的次数
		{
			const cmd_enter_raid_count* pCmd = (const cmd_enter_raid_count*)Msg.dwParam1;
			CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo.clear();
			for (int i=0; i<pCmd->raid_list_size; ++i)
			{
				CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[pCmd->enter_count_list[i].raid_map_id] = pCmd->enter_count_list[i].enter_count;
			}
			break;
		}
		case PLAYER_ENTER_RAID_COUNT:	//gs通知客户端玩家进入某单个副本的次数
		{
			const player_enter_raid_count *pCmd = (const player_enter_raid_count*)Msg.dwParam1;
			CECBattleDungeonMan::getSingletonPtr()->m_mapDungeonEnterInfo[pCmd->map_id] = pCmd->enter_count;
			// 执行其中一个
			if (pGameUI->CommonMessage("Win_Battle_Dungeon_Tower", "enter_count", pCmd->map_id, pCmd->enter_count) || // or
				pGameUI->CommonMessage("Win_Battle_DungeonApply", "enter_count", pCmd->map_id, pCmd->enter_count))
			{
			}
			break;
		}
		case RAID_PLAYER_INFO:			// gs通知客户端进入副本玩家的相关信息 Added 2011-08-03.
		{
			//const raid_player_info* pCmd = (const raid_player_info*)Msg.dwParam1;
			pGameUI->CommonMessage("Win_Battle_DungeonMemberList", "member_list", Msg.dwParam1, 0);
			//TODO:
			break;
		}
		case PROTOCOL_RAIDOPENVOTE_RE:	// 发起投票回复，只需要处理错误码或不处理 2012-07-24
		{
			RaidOpenVote_Re *p = (RaidOpenVote_Re*)Msg.dwParam1;
			if (p->retcode!=0)
			{
				CECBattleDungeonMan::getSingletonPtr()->ReportError(p->retcode);
			}
			break;
		}
		case PROTOCOL_RAIDBEGINVOTES:	// 玩家开始投票是否踢人 2012-07-24
		{
			CECBattleDungeonMan::getSingletonPtr()->KickPlayer((void*)Msg.dwParam1);
			break;
		}
		case PROTOCOL_RAIDVOTERESULT:	// 投票结果喊话 2012-07-24
		{
			CECBattleDungeonMan::getSingletonPtr()->AddHint((void*)Msg.dwParam1);
			break;
		}
		case PROTOCOL_RAIDBRIEFINFO:	// 副本中房主通知
		{
			RaidBriefInfo *p = (RaidBriefInfo*)Msg.dwParam1;
			CECBattleDungeonMan::getSingletonPtr()->m_iRoomId = p->raidroom_id;
			CECBattleDungeonMan::getSingletonPtr()->m_iLeaderId = p->masterid;
		}
		default:
			break;
	}
}

void CECHostPlayer::OnMsgHstBeMoved(const ECMSG& Msg)
{
	using namespace S2C;
	
	// 设置一个当前玩家位置距离服务器传来的目标位置的最大距离阈值
	#define MAX_DIST		8.0f

	//判合法性
	if ( OBJECT_BE_MOVED != Msg.dwParam2 )
	{
		return;
	}

	const cmd_object_be_moved* pCmd = (const cmd_object_be_moved*)(Msg.dwParam1);
	//如果所发协议的角色id不是主角玩家，则错误
	if ( pCmd->id != m_PlayerInfo.cid )
	{
		return;
	}

	// 判断移动的使用时间合法性
	if ( !pCmd->use_time )
	{
		return;
	}

	// 判断距离是否过远，如果过远则直接拉到服务器的目标位置
	A3DVECTOR3 vMoveDir = pCmd->dest - GetPos();
	float fDist = vMoveDir.Normalize();
	if ( fDist >= MAX_DIST )
	{
		SetPos( pCmd->dest );
		return;
	}
	
	DWORD dwTimeNow = a_GetTime();
	DWORD dwDeltaTime = dwTimeNow - m_dwLastMoveTime;
	m_dwLastMoveTime = dwTimeNow;
	if( dwDeltaTime < 500 )
		dwDeltaTime = 500;
	if( dwDeltaTime > 1000 )
		dwDeltaTime = 1000;
	
	//	Calcualte speed
	float fSpeed	= FIX8TOFLOAT(pCmd->speed);
	float fCurSpeed = fDist / (dwDeltaTime * 0.001f);
	a_Clamp(fCurSpeed, 0.0f, fSpeed * 1.2f);

	//启动BeMove work
	CECHPWorkBeMoved* pNewWork = (CECHPWorkBeMoved*)m_pWorkMan->CreateWork(CECHPWork::WORK_BE_MOVED);
	pNewWork->SetDestination(pCmd->dest, fCurSpeed);
	m_pWorkMan->StartWork_p1(pNewWork);
	
}

void CECHostPlayer::OnMsgHstPullOpt(const ECMSG& Msg)
{
	using namespace S2C;

	//根据拉扯/牵引相关技能的协议不同，进行不同的处理
	switch (Msg.dwParam2)
	{
		case PLAYER_PULL_STATE:	//目前仅供拉扯专用，设置开始/终止拉扯的状态
		{
			const cmd_player_pull_state* pCmd = (const cmd_player_pull_state*)(Msg.dwParam1);
			if ( pCmd->is_pull )	//开始拉扯
			{
				CECHPWorkSpecialCharge* pWork = (CECHPWorkSpecialCharge*)m_pWorkMan->CreateWork(CECHPWork::WORK_SPECIAL_CHARGE);
				m_pWorkMan->StartWork_p1(pWork);
				
				// 设置持续按键标志
				SetSpecialCharge(true);
			}
			else					//结束拉扯
			{
				m_pWorkMan->FinishCurWork( CECHPWork::WORK_SPECIAL_CHARGE, true ); //直接停止该work
				// 设置持续按键标志
				SetSpecialCharge(false);
			}
			break;
		}
		case PLAYER_BE_PULLED_INFO:	//玩家拉扯/牵引信息
			{
				const cmd_player_be_pulled_info* pCmd = (const cmd_player_be_pulled_info*)(Msg.dwParam1);
				
				if ( pCmd->is_pull )	//开始拉扯/牵引状态下，保存这两个玩家的信息
				{
					AddPullGfxInfo( pCmd->player_cast_id, pCmd->player_target_id, pCmd->type );
				}
				else					//玩家拉扯/牵引状态终止，删除这两个玩家信息
				{
					if ( !pCmd->type )	//这样的搞法真的很蛋疼。策划需求改变，暂时在此加入代码以应变化
					{
						CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->player_cast_id, 1);
						if ( !pCaster )
						{
							return;
						}
						CECPlayer* pPlayer = (CECPlayer*)pCaster;
						if(pPlayer->GetPlayerModel())
							pPlayer->GetPlayerModel()->RemoveGfx("程序联入\\擒龙决.gfx", NULL);
					}
					else
					{
						RemovePullGfxInfo( pCmd->player_cast_id );
					}
				}
				break;
		}
		default:
			break;
	}
}

void CECHostPlayer::OnMsgHstOnlineAwardOpt( const ECMSG& Msg )
{
	//
	using namespace S2C;

	switch ( Msg.dwParam2 )
	{
		case ONLINE_AWARD_INFO:		//收到gs发来的在线奖励倒计时信息列表协议
		{
			online_award_info* pCmd = (online_award_info*)(Msg.dwParam1);
			if ( !pCmd )
			{
				return;
			}
			m_pOnlineAwardManager->UpdateData( *pCmd );
			break;
		}
		case ONLINE_AWARD_PERMIT:	//收到gs发来关于某在线奖励是否可以发放的协议
		{
			online_award_permit* pCmd = (online_award_permit*)(Msg.dwParam1);
			if ( !pCmd )
			{
				return;
			}
			//检查具体的permit_code判断相应操作
			if ( 0 == pCmd->permit_code )		//表示可以领取奖励
			{
				//TODO:UI操作？

				//停止对该奖励礼包的Tick操作
				m_pOnlineAwardManager->StopAwardListTickStatus( pCmd->award_id, pCmd->index );
			}
			else if ( 1 == pCmd->permit_code )	//表示该轮在线奖励成功进入包裹，可以启动下一轮倒计时
			{
				m_pOnlineAwardManager->UpdateAwardListInfo( pCmd->award_id, pCmd->index );
			}
			
			break;
		}
		default:
			break;
	}

}

void CECHostPlayer::OnMsgHstTreasureRegionOpt( const ECMSG& Msg )
{
	//TODO
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	switch ( Msg.dwParam2 )
	{
		case TREASURE_INFO:							//挖宝区域信息更新
		{
			pGameUI->CommonMessage("Win_ZhuxianExplore", "TREASURE_INFO", Msg.dwParam1, 0);
			break;
		}
		case TREASURE_REGION_UPGRADE_RESULT:		//挖宝区域升级结果信息
		{
			pGameUI->CommonMessage("Win_ZhuxianExplore", "TREASURE_REGION_UPGRADE_RESULT", Msg.dwParam1, 0);
			break;
		}
		case TREASURE_REGION_UNLOCK_RESULT:			//挖宝区域解锁结果信息
		{
			pGameUI->CommonMessage("Win_ZhuxianExplore", "TREASURE_REGION_UNLOCK_RESULT", Msg.dwParam1, 0);
			break;
		}
		case TREASURE_REGION_DIG_RESULT:			//挖宝结果信息
		{
			pGameUI->CommonMessage("Win_ZhuxianExplore", "TREASURE_REGION_DIG_RESULT", Msg.dwParam1, 0);
			break;
		}
		case INC_TREASURE_DIG_COUNT:			//挖宝结果信息
		{
			pGameUI->CommonMessage("Win_ZhuxianExplore", "INC_TREASURE_DIG_COUNT", Msg.dwParam1, 0);
			break;
		}
		default:
			break;
	}
}

void CECHostPlayer::OnMsgHstTowerRaidOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	switch ( Msg.dwParam2 )
	{
		case RAID_GLOBAL_VARIABLE:				//副本地图全局变量， 也通过这个总消息接口走
		{
			pGameUI->CommonMessage("Win_Battle_Dungeon_Heyang", "RAID_GLOBAL_VARIABLE", Msg.dwParam1, 0);
			break;
		}
		case RANDOM_TOWER_MONSTER_RESULT:		//通知客户端随机选怪结果
		{
			pGameUI->CommonMessage("Win_Battle_Dungeon_Tower_Random", "random_result", Msg.dwParam1, 0);
			break;
		}
		case PLAYER_TOWER_DATA:					//进入副本通知：玩家相关趴塔副本信息
		{
			m_pBattleDungeonTower->UpdateTowerData((void*)Msg.dwParam1);
			break;
		}
		case PLAYER_TOWER_MONSTER_LIST:			//进入副本通知：玩家趴塔副本怪物图鉴列表
		{
			m_pBattleDungeonTower->UpdateMonsters((void*)Msg.dwParam1);
			break;
		}
		case PLAYER_TOWER_GIVE_SKILLS:			//进入副本通知：趴塔副本技能手牌
		{
			pGameUI->CommonMessage("Win_Battle_Dungeon_Tower_LuckySkill", "PLAYER_TOWER_GIVE_SKILLS", Msg.dwParam1, 0);
			break;
		}
		case SET_TOWER_GIVE_SKILL:				//单个通知：单个手牌使用后通知客户端改变状态
		{
			pGameUI->CommonMessage("Win_Battle_Dungeon_Tower_LuckySkill", "SET_TOWER_GIVE_SKILL", Msg.dwParam1, 0);
			break;
		}
		case ADD_TOWER_MONSTER:					//单个通知：获得趴塔副本一个怪物图鉴
		{
			S2C::add_tower_monster *p = (S2C::add_tower_monster*)Msg.dwParam1;
			m_pBattleDungeonTower->AddMonster(p->monster_id);
			break;
		}
		case SET_TOWER_AWARD_STATE:				//趴塔副本奖励领取成功后发送给客户端状态协议
		{
			m_pBattleDungeonTower->UpdateAwardState((void*)Msg.dwParam1);
			break;
		}
		case TOWER_LEVEL_PASS:
		{
			m_pBattleDungeonTower->PassOneLevel((void*)Msg.dwParam1);
			break;
		}
	}
}

void CECHostPlayer::OnMsgHstLittlePet(const ECMSG& Msg)
{
	using namespace S2C;

	littlepet_info* pInfo = (littlepet_info*)Msg.dwParam1;
	
	if (Msg.dwParam2 == LITTLEPET_INFO)
	{
		m_LittlePetInfo.iLevel = pInfo->level;
		m_LittlePetInfo.iExp = pInfo->cur_exp;
		m_LittlePetInfo.iAward = pInfo->cur_award_lvl;
	}
}

void CECHostPlayer::OnMsgHstRuneInfo(const ECMSG& Msg)
{
	using namespace S2C;
	
	if (Msg.dwParam2 == RUNE_INFO)
	{
		rune_info* pInfo = (rune_info*)Msg.dwParam1;
		m_bEquipRuneOpen = pInfo->active;
		m_nRuneValue	 = pInfo->rune_score;
		m_nRuneCustomize = pInfo->customize_info;
	}
	else if (Msg.dwParam2 == RUNE_REFINE_RESULT)
	{
		// TODO:
	}
}

void CECHostPlayer::OnMsgHstUIFunction(const ECMSG& Msg)
{
	using namespace S2C;
	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (Msg.dwParam2 == PK_BET_DATA)
	{
		pGameUI->CommonMessage("Win_Kf2012Bet1", "set_award", Msg.dwParam1, 0);
		pGameUI->CommonMessage("Win_Kf2012Bet2", "set_award", Msg.dwParam1, 0);
	}
	else if (Msg.dwParam2 == TALISMAN_REFINESKILL)
	{
		talisman_refineskill* pCmd = (talisman_refineskill*)Msg.dwParam1;
		pGameUI->CommonMessage("Win_TalismanCombineSkill_Info", "refine_finish", pCmd->index, pCmd->id);
	}
	else if (Msg.dwParam2 == COLLISION_RAID_INFO)
	{
		pGameUI->CommonMessage("Win_Battle_Dungeon_6V6_Award", "info", Msg.dwParam1, 0);
		pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_GroupAward01", "info", Msg.dwParam1, 0);
		pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_GroupAward02", "info", Msg.dwParam1, 0);
		pGameUI->CommonMessage("Win_Battle_Dungeon_6v6_GroupAward03", "info", Msg.dwParam1, 0);
	}
	else if (Msg.dwParam2 == COLLISION_RAID_RESULT)
	{
		collision_raid_result* pCmd = (collision_raid_result*)Msg.dwParam1;
		PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Battle_Dungeon_6V6_ResultPic");
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pDlg->GetDlgItem("Img_Result");
		pImg->FixFrame(pCmd->result);
		pDlg->Show(true);
	}
	else if (Msg.dwParam2 == CASH_GIFT_IDS)
	{
		cash_gift_ids* pCmd = (cash_gift_ids*)Msg.dwParam1;
		m_iGiftScoreItem1 = pCmd->gift_id1;
		m_iGiftScoreItem2 = pCmd->gift_id2;
	}
	else if (Msg.dwParam2 == NOTIFY_CASH_GIFT)
	{
		// 服务器应该已经做了处理，这里双保险
		if (g_pGame->GetConfigs()->IsConsumeScoreDisable())
			return;
		
		notify_cash_gift* pCmd = (notify_cash_gift*)Msg.dwParam1;
		PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Qshop_ConsumeScore_Popup");
		pDlg->Show(true);
		PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_ConsumeScore");
		ACString strText;
		strText.Format(_AL("%d"), pCmd->gift_score);
		pLab->SetText(strText);
	}
}

void CECHostPlayer::OnMsgHstTalismanServiceOpt(const ECMSG& Msg)
{
  g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->m_pDlgTalismanEnchanting->ContinueEnchant();
}

void CECHostPlayer::OnMsgHstKingdomOpt(const ECMSG& Msg)
{
   //m_cKingdomWarType 0 离开战场 1 主战场第一阶段 2 主战场第二阶段 3 辅战场
   using namespace S2C;
   CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
   if (Msg.dwParam2 == ENTER_KINGDOM)
   {
	   enter_kingdom* pCmd = (enter_kingdom*)(Msg.dwParam1);
	   if ( !pCmd )
	   {
		   return;
	   }
	   if(pCmd->kingdom_type == 1)
	   {
		   m_cKingdomWarType = pCmd->cur_phase == 1 ? 1 : 2;
	   }
	   else if(pCmd->kingdom_type == 2)
	   {
	       m_cKingdomWarType = 3;
	   }
	   //主战场第一阶段有阵营
	   if(m_cKingdomWarType == 1)
	   {
		   //设置阵营信息
		   m_iBattleCamp = pCmd->battle_faction;
	   }
	   if(m_cKingdomWarType != 2)
	   {
		   CDlgKingBattleInfo* pDlg = (CDlgKingBattleInfo*)pGameUI->GetDialog("Win_King_BattleInfo");
		   pDlg->SetEndTime(pCmd->end_timestamp);
	       pDlg->Show(true);
	   }
	   else
	   {
		   CDlgKingBattleInfo* pDlg = (CDlgKingBattleInfo*)pGameUI->GetDialog("Win_King_BattleInfo2");
		   pDlg->SetEndTime(pCmd->end_timestamp);
	       pDlg->Show(true);
	   }
	   if(m_cKingdomWarType != 3)
	   {
           CDlgKingBattleInfo3* pDlg3 = (CDlgKingBattleInfo3*)pGameUI->GetDialog("Win_King_BattleInfo3");
		   pDlg3->SetMafiaIds(pCmd->defender_mafia_id
			                  ,pCmd->attacker_mafia_count
							  ,pCmd->attacker_mafia_ids);
		   pDlg3->SetEndTime(pCmd->end_timestamp,m_cKingdomWarType);
	   }

   }
   else if (Msg.dwParam2 == LEAVE_KINGDOM)
   {
	   leave_kingdom* pCmd = (leave_kingdom*)(Msg.dwParam1);
	   if ( !pCmd )
	   {
		   return;
	   }
	   m_cKingdomWarType = 0;
	   m_iBattleCamp = GP_BATTLE_CAMP_NONE;
	   if(pGameUI->GetDialog("Win_King_BattleInfo")->IsShow())
	      pGameUI->GetDialog("Win_King_BattleInfo")->Show(false);
	   if(pCmd->kingdom_type == 1)
	   {
		   if(pGameUI->GetDialog("Win_King_BattleInfo2")->IsShow())
             pGameUI->GetDialog("Win_King_BattleInfo2")->Show(false);
		   if(pGameUI->GetDialog("Win_King_BattleInfo3")->IsShow())
             pGameUI->GetDialog("Win_King_BattleInfo3")->Show(false);

	   }
   }
   else if(Msg.dwParam2 == KINGDOM_HALF_END)
   {
	   kingdom_half_end* pCmd = (kingdom_half_end*)(Msg.dwParam1);
	   if ( !pCmd )
	   {
		   return;
	   }
	   m_cKingdomWarType = 2;
	   m_iBattleCamp = GP_BATTLE_CAMP_NONE;

	   POINT pos = pGameUI->GetDialog("Win_King_BattleInfo")->GetPos();
	   CDlgKingBattleInfo* pDlg = (CDlgKingBattleInfo*)pGameUI->GetDialog("Win_King_BattleInfo2");
	   pDlg->SetEndTime(pCmd->end_timestamp);
	   pDlg->SetPosEx(pos.x,pos.y);
	   pDlg->Show(true);
	   CDlgKingBattleInfo3* pDlg3 = (CDlgKingBattleInfo3*)pGameUI->GetDialog("Win_King_BattleInfo3");
	   pDlg3->SetEndTime(pCmd->end_timestamp,m_cKingdomWarType);
	   pGameUI->GetDialog("Win_King_BattleInfo")->Show(false);
	   
   }
   else if(Msg.dwParam2 == KINGDOM_END_RESULT)
   {
	   kingdom_end_result* pCmd = (kingdom_end_result*)(Msg.dwParam1);
	   if ( !pCmd )
	   {
		   return;
	   }
   }
   else if(Msg.dwParam2 == KINGDOM_MAFIA_INFO)
   {
       kingdom_mafia_info* pCmd = (kingdom_mafia_info*)(Msg.dwParam1);
	   if ( !pCmd )
	   {
		   return;
	   }
	   CDlgKingBattleInfo3* pDlg3 = (CDlgKingBattleInfo3*)pGameUI->GetDialog("Win_King_BattleInfo3");
	   pDlg3->SetMafiaInfo(pCmd);
   }
   else if (Msg.dwParam2 == KINGDOM_KEY_NPC_INFO)
   {
	   kingdom_key_npc_info* pCmd = (kingdom_key_npc_info*)(Msg.dwParam1);
	   if(!pCmd)
	   {
		   return;
	   }
	   //界面显示刷新
	   CDlgKingBattleInfo* pDlg = (CDlgKingBattleInfo*)pGameUI->GetDialog("Win_King_BattleInfo");
	   pDlg->RefreshState(pCmd->cur_hp,pCmd->max_hp,pCmd->hp_add,pCmd->hp_dec);
   }
   else if(Msg.dwParam2 == KINGDOM_GATHER_MINE_START)
   {
	   kingdom_gather_mine_start* pCmd = (kingdom_gather_mine_start*)(Msg.dwParam1);
	   if(!pCmd)
	   {
		  return;
	   }
       Faction_Info *pFInfo = NULL;
	   if(pCmd->mafia_id != 0)
	   {
	       pFInfo = g_pGame->GetFactionMan()->GetFaction(pCmd->mafia_id,true);
		   if(NULL != pFInfo)
		   {
              ACString strMsg;
			  strMsg.Format(pGameUI->GetStringFromTable(15676),
				            pFInfo->GetName(),
							g_pGame->GetGameRun()->GetPlayerName(pCmd->roleid, true));
			  pGameUI->AddChatMessage(strMsg, GP_CHAT_BROADCAST);
		   }
	   }
   }

}
void CECHostPlayer::OnMsgHstAccountPoint(const ECMSG& Msg)
{
	using namespace GNET;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

/*
	switch (Msg.dwParam2)
	{
	case PROTOCOL_SELLPOINT_RE:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_SELLPOINT_RE, (void*)Msg.dwParam1);
		break;
	case PROTOCOL_GETSELLLIST_RE:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_GETSELFSELLLIST_RE, (void*)Msg.dwParam1);
		break;
	case PROTOCOL_FINDSELLPOINTINFO_RE:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_GETSHOPSELLLIST_RE, (void*)Msg.dwParam1);
		break;
	case PROTOCOL_SELLCANCEL_RE:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_CANCELPOINT_RE, (void*)Msg.dwParam1);
		break;
	case PROTOCOL_BUYPOINT_RE:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_BUYPOINT_RE, (void*)Msg.dwParam1);
		break;
	case PROTOCOL_ANNOUNCESELLRESULT:
		pGameUI->CardAction(CDlgCardSell::CARD_ACTION_ANNOUNCESELLRESULT, (void*)Msg.dwParam1);
		break;
	}
*/
}

void CECHostPlayer::OnMsgHstGMOpt(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == GM_INVISIBLE)
	{
		const gm_cmd_invisible* pCmd = (const gm_cmd_invisible*)Msg.dwParam1;
		if (pCmd->is_visible)
		{
			m_dwGMFlags &= ~GMF_INVISIBLE;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GM_VISIBLE);
		}
		else
		{
			m_dwGMFlags |= GMF_INVISIBLE;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GM_INVISIBLE);
		}
	}
	else
	{
		const gm_cmd_invincible* pCmd = (const gm_cmd_invincible*)Msg.dwParam1;
		if (pCmd->is_invincible)
		{
			m_dwGMFlags |= GMF_INVINCIBLE;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GM_INVINCIBLE);
		}
		else
		{
			m_dwGMFlags &= ~GMF_INVINCIBLE;
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_GM_VINCIBLE);
		}
	}
}

void CECHostPlayer::OnMsgHstAutoSelect(const ECMSG& Msg)
{
	StackChecker::ACTrace(3);

	if (m_nTabSelOrder == TABSEL_ALL)	// 既选怪又可以选人
	{
		// 玩家手动tab选怪，优先选择人
		if (Msg.dwParam1 == 0)
		{
			if(TabSelPlayer())
				return;

			if(TabSelMonster())
				return;
		}
		else if (Msg.dwParam1 == 1)		// 天人合一情况下的自动选怪逻辑，与原来的逻辑一致
		{
			// 帮战状态下，天人合一不能选怪问题的修改（现在为只能选怪）
			// Tab选怪和选人的逻辑再次修改，优先选人的，如果选择范围内没有人则开始选怪，优先选怪同样的处理

			if (/*GetCombatFaction() || */ IsInBattle() || m_pvp.ucFreePVPType == 2)
			{
				if(TabSelPlayer())
					return;

				if(TabSelMonster())
					return;
			}
			else
			{
				if(TabSelMonster())
					return;
			}
		}
	}
	else if (m_nTabSelOrder == TABSEL_MONSTER)	// 选怪
	{
		if(TabSelMonster())
			return;
	}
	else if (m_nTabSelOrder == TABSEL_PLAYER)	// 选人
	{
		if(TabSelPlayer())
			return;
	}		
}

bool CECHostPlayer::TabSelPlayer()
{
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();

	int idCurSel = (ISPLAYERID(m_idSelTarget) && m_idSelTarget != GetCharacterID()) ? m_idSelTarget : 0;
	int idNewSel = pPlayerMan->TabSelectEnemy(idCurSel);
	
	if (idNewSel && idNewSel != idCurSel)	// 如果选中的仍是当前角色，则需要切换选择对象
	{
		m_idUCSelTarget = idNewSel;
		g_pGame->GetGameSession()->c2s_CmdSelectTarget(idNewSel);
	
		CECPlayer * pPlayer = pPlayerMan->GetElsePlayer(idNewSel);
		if( pPlayer )
		{
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->SetTempTargetInMiniMap(pPlayer->GetPos() + A3DVECTOR3(0.0f, pPlayer->GetModelAABB().Extents.y * 2.0f, 0.0f));
		}

		return true;
	}

	return false;
}

bool CECHostPlayer::TabSelMonster()
{
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();

	int idCurSel = ISNPCID(m_idSelTarget) ? m_idSelTarget : 0;
	int idNewSel = pNPCMan->TabSelectMonster(idCurSel);
	
	if (idNewSel)
	{
		if (idNewSel != idCurSel)
		{
			m_idUCSelTarget = idNewSel;
			g_pGame->GetGameSession()->c2s_CmdSelectTarget(idNewSel);
		}

		CECNPC * pNPC = pNPCMan->GetNPC(idNewSel);
		if( pNPC )
		{
			g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->SetTempTargetInMiniMap(pNPC->GetPos() + A3DVECTOR3(0.0f, pNPC->GetPickAABB().Extents.y * 2.0f, 0.0f));
		}

		return true;
	}

	return false;
}

// 获取一定范围内距离目标idTarget最近且不在列表中的敌人
int CECHostPlayer::GetNearestEnemy(int idTarget, float fRange, abase::vector<int>& vecTargets)
{
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	
	A3DVECTOR3 vTargetPos;
	// find target
	if(ISPLAYERID(idTarget))
	{
		if(idTarget == GetCharacterID())
		{
			vTargetPos = GetPos();
		}
		else
		{
			CECPlayer * pPlayer = pPlayerMan->GetElsePlayer(idTarget);
			if(!pPlayer)
			{
				ASSERT(0);
				return 0;
			}
			
			vTargetPos = pPlayer->GetPos();
		}
	}
	else if(ISNPCID(idTarget))
	{
		CECNPC * pNPC = pNPCMan->GetNPC(idTarget);
		if(!pNPC)
		{
			ASSERT(0);
			return 0;
		}
		
		vTargetPos = pNPC->GetPos();	
	}
	
	float fMinDistNPC = fRange, fMinDistPlayer = fRange;
	
	// find nearest monster
	int idNPC = pNPCMan->GetNearestEnemyNPC(vTargetPos, fRange, vecTargets, &fMinDistNPC);
	
	// find nearest player
	int idPlayer = pPlayerMan->GetNearestEnemyPlayer(vTargetPos, fRange, vecTargets, &fMinDistPlayer);
	
	if(fMinDistNPC < fMinDistPlayer)
		return idNPC;
	else
		return idPlayer;
}

void CECHostPlayer::OnMsgMoveRecord(const ECMSG& Msg)
{
	A3DVECTOR3 vMoveDest;
	vMoveDest.x = *(float*)&Msg.dwParam2;
	vMoveDest.y = *(float*)&Msg.dwParam3;
	vMoveDest.z = *(float*)&Msg.dwParam4;
	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();

	//	Move on the clicked direction
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
	{
		CECHPWorkMove* pWork = (CECHPWorkMove*)pP1Work;
		pWork->SetDestination(Msg.dwParam1, vMoveDest);
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_MOVETOPOS))
	{
		CECHPWorkMove* pWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVETOPOS);
		pWork->SetDestination(Msg.dwParam1, vMoveDest);
		m_pWorkMan->StartWork_p1(pWork);
	}
}

void CECHostPlayer::OnMsgTraceRecord(const ECMSG& Msg)
{
	int idTraceTarget = Msg.dwParam1;
	int iTraceReason = Msg.dwParam2;
	bool bForceAttack = (Msg.dwParam3 != 0);
	CECSkill* pSkill;

	if (iTraceReason == CECHPWorkTrace::TRACE_SPELL)
	{
		pSkill = GetPositiveSkillByID(Msg.dwParam4);

		if ( !pSkill )
		{
			return;
		}
	}

	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();

	//	Trace a object
	if (iTraceReason == CECHPWorkTrace::TRACE_ATTACK)
		NormalAttackObject(idTraceTarget, bForceAttack, Msg.dwParam4 != 0);
	else if (iTraceReason == CECHPWorkTrace::TRACE_PICKUP)
		PickupObject(idTraceTarget, false);
	else if (iTraceReason == CECHPWorkTrace::TRACE_GATHER)
		PickupObject(idTraceTarget, true);
	else if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)pP1Work;
		pWork->ChangeTarget(idTraceTarget, iTraceReason);
		pWork->SetForceAttack(bForceAttack);

		if (iTraceReason == CECHPWorkTrace::TRACE_SPELL)
			pWork->SetPrepSkill(pSkill);
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
		pWork->ChangeTarget(idTraceTarget, iTraceReason);
		pWork->SetForceAttack(bForceAttack);

		if (iTraceReason == CECHPWorkTrace::TRACE_SPELL)
			pWork->SetPrepSkill(pSkill);

		m_pWorkMan->StartWork_p1(pWork);
	}
}

void CECHostPlayer::OnMsgFollowRecord(const ECMSG& Msg)
{
	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
	int idTarget = Msg.dwParam1;

	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::MASK_FOLLOW)
	{
		((CECHPWorkFollow*)pP1Work)->ChangeTarget(idTarget);
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_FOLLOW))
	{
		CECHPWorkFollow* pWork = (CECHPWorkFollow*)m_pWorkMan->CreateWork(CECHPWork::WORK_FOLLOW);
		pWork->ChangeTarget(idTarget);
		m_pWorkMan->StartWork_p1(pWork);
	}
}

void CECHostPlayer::OnMsgWalkRecord(const ECMSG& Msg)
{
	m_bWalkRun = (Msg.dwParam1 != 0);
}

void CECHostPlayer::OnMsgPushRecord(const ECMSG& Msg)
{
	if (Msg.dwParam1 == 1)
		m_dwMoveRelDir = 0;
}

void CECHostPlayer::OnMsgCheckRecord(const ECMSG& Msg)
{
	using namespace S2C;

	if (gr.get_state() == game_record::enum_gr_replay)
	{
		record_check_result* p = (record_check_result*)Msg.dwParam1;
		int stamp = gr.get_record_stamp();
		int roleid = GetCharacterID();
		CECInventory* pPack = GetPack();

		GNET::MD5Hash hash;
		hash.Update(GNET::Octets((const unsigned char*)&roleid, sizeof(roleid)));
		hash.Update(GNET::Octets((const unsigned char*)&stamp, sizeof(stamp)));

		for (int i = 0; i < pPack->GetSize(); i++)
		{
			CECIvtrItem* pItem = pPack->GetItem(i);
			int id = pItem ? pItem->GetTemplateID() : -1;
			hash.Update(GNET::Octets((const unsigned char*)&id, sizeof(id)));
		}

		GNET::Octets o;
		hash.Final(o);

		if (memcmp(o.begin(), p->value, 16) == 0)
			gr.inc_replay_valid();
		else
		{
			assert(false);
		}
	}

	gr.inc_record_stamp();
}

void CECHostPlayer::OnMsgHstFallRecord(const ECMSG& Msg)
{
	A3DVECTOR3 vPos;
	vPos.x = *(float*)&Msg.dwParam2;
	vPos.y = *(float*)&Msg.dwParam3;
	vPos.z = *(float*)&Msg.dwParam4;

	gr.reset_host_pos(vPos);
}

void CECHostPlayer::OnMsgPlayerSitDown(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == OBJECT_SIT_DOWN)
	{
		int iCurWorkID = m_pWorkMan->GetCurWorkID();
		if (iCurWorkID == CECHPWork::WORK_MOVETOPOS || 
			iCurWorkID == CECHPWork::WORK_TRACEOBJECT ||
			iCurWorkID == CECHPWork::WORK_FOLLOW)
		{
			m_MoveCtrl.SendStopMoveCmd(GetPos(), GetGroundSpeed(), GP_MOVE_RUN);
		}

		m_dwStates |= GP_STATE_SITDOWN;
		CECHPWorkSit* pWork = (CECHPWorkSit*)m_pWorkMan->CreateWork(CECHPWork::WORK_SIT);
		pWork->SetBeSittingFlag(false);
		m_pWorkMan->StartWork_p1(pWork);
	}
	else if (Msg.dwParam2 == OBJECT_STAND_UP)
	{
		m_dwStates &= ~GP_STATE_SITDOWN;
		CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->CreateWork(CECHPWork::WORK_STAND);
		m_pWorkMan->StartWork_p1(pWork);
	}
}

void CECHostPlayer::OnMsgPlayerBaseInfo(const ECMSG& Msg)
{
	//	We have cached host's basic information when login, 
	//	this function is obosolete.
	ASSERT(0);
	return;

/*	using namespace GNET;
	
	PlayerBaseInfo_Re* p = (PlayerBaseInfo_Re*)Msg.dwParam1;
	const GRoleBase& base = p->player;

	ASSERT((int)base.id == m_PlayerInfo.cid);

	m_iProfession	= base.cls;
	m_iRace			= base.race;
	m_iGender		= base.gender;

	m_PlayerInfo.crc_c = base.custom_stamp;

	//	Get player name and save into name cache
	m_strName = ACString((const ACHAR*)base.name.begin(), base.name.size() / sizeof (ACHAR));
	m_pPlayerMan->AddPlayerName(m_PlayerInfo.cid, m_strName);
	if (m_pPateName)
		m_pPateName->SetText(m_strName, false);

	//	Calculate player's AABB
	CalcPlayerAABB();

	//	Update upper body radius using new AABB
	m_pCDRInfo->fRadius = (m_aabb.Extents.y * 2.0f - m_pCDRInfo->fStepHeight) * 0.5f;
	m_pAirCDRInfo->fRadius = m_pCDRInfo->fRadius;

	//	Load player skeleton
	if (!LoadPlayerSkeleton())
	{
		a_LogOutput(1, "CECHostPlayer::OnMsgPlayerCustomData, Failed to load skeleton");
		return;
	}

	//	Load custom data
	if (base.custom_data.size() >= 4)
	{
		if (!ChangeCustomizeData(*(PLAYER_CUSTOMIZEDATA*)base.custom_data.begin()))
		{
			a_LogOutput(1, "CECHostPlayer::OnMsgPlayerCustomData, Failed to load custom data");
		//	return;
		}
	}
	else
		SetResReadyFlag(RESFG_CUSTOM, true);

	//	Get host's necessary data
	g_pGame->GetGameSession()->c2s_CmdGetAllData(true, true, false);
*/
}

void CECHostPlayer::OnMsgPlayerCustomData(const ECMSG& Msg)
{
}

void CECHostPlayer::OnMsgPlayerFly(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == OBJECT_TAKEOFF)
	{
		m_dwStates |= GP_STATE_FLY;
		m_bRushFly	= false;

		CECHPWorkFly* pWork = (CECHPWorkFly*)m_pWorkMan->CreateWork(CECHPWork::WORK_FLYOFF);

		if(Msg.dwParam3 == 1)	// 玩家释放技能导致的飞行效果
		{
			pWork->m_bSkillFly = true;
			m_bSkillFly = true;
		}
		else
		{
			pWork->m_bSkillFly = false;
			m_bSkillFly = false;
		}

		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_FREEFALL)
		{
			pWork->m_bContinueFly = true;
			m_pWorkMan->StartWork_p1(pWork);
		}
		else
		{
			pWork->m_bContinueFly = false;
			m_pWorkMan->StartWork_p2(pWork);
		}
	}
	else if (Msg.dwParam2 == OBJECT_LANDING)
	{
		StartLanding();
	}
	else	//	HOST_RUSH_FLY
	{
		cmd_host_rush_fly* pCmd = (cmd_host_rush_fly*)Msg.dwParam1;
		m_bRushFly = pCmd->is_active ? true : false;
	}
}

void CECHostPlayer::OnMsgPlayerCastSkill(const ECMSG& Msg)
{
	using namespace S2C;

	bool bDoOtherThing = false;
	int idTarget = 0;

	switch (Msg.dwParam2)
	{
	case OBJECT_CAST_SKILL:
	{
		cmd_object_cast_skill* pCmd = (cmd_object_cast_skill*)Msg.dwParam1;
		ASSERT(pCmd->caster == m_PlayerInfo.cid);

// 		if (m_pCurSkill)
// 		{
// 			m_pCurSkill->EndCharging();
// 		}
// 		DWORD time = ACounter::GetMilliSecondNow();
// 		AString str;
// 		str.Format("cast skill receive time : %d\n", time);
// 		OutputDebugStringA(str);

		if (!(m_pCurSkill = GetPositiveSkillByID(pCmd->skill)))
		{
			if (!(m_pCurSkill = GetTransfigureSkillByID(pCmd->skill)))
			{
				if (!(m_pCurSkill = GetItemSkillByID(pCmd->skill)))
				{
					if (!(m_pCurSkill = m_pCastTwSkills))
					{
						ASSERT(m_pCurSkill);
						return;
					}
				}
			}		
		}
		
		//Added 2011-06-17.
		if (pCmd->state == 0) // 第一阶段：吟唱, 无论如何都设置技能发放速度值，专为太昊的麻痹和迅疾技能使用
		{
			if ( pCmd->cast_speed_rate >= 100 )
			{
				pCmd->cast_speed_rate = 90;
			}
			m_fSkillSpeedRate = 1.0f / ((100 - pCmd->cast_speed_rate)*0.01f);
			
			//Modified 2011-08-02.
			const char* szCircleGfxName = m_pCurSkill->GetCircleGfxName();
			if ( szCircleGfxName && szCircleGfxName[0] != '\0' ) //如果当前技能中有阵法特效？是否应该要判断该技能为阵法技能
			{
				char szCODSponsorGfxName[512], szCODGfxName[512];
				sprintf( szCODSponsorGfxName, "%s%s%s.gfx", "程序联入\\", szCircleGfxName, "阵眼");
				sprintf( szCODGfxName, "%s%s%s.gfx", "程序联入\\", szCircleGfxName, "选区" );
				if(m_pPlayerModel)
				{
					m_pPlayerModel->PlayGfx(szCODSponsorGfxName, NULL);//"程序联入\\阵眼测试.gfx"
					m_pPlayerModel->PlayGfx(szCODGfxName, NULL);//"程序联入\\法阵选区测试.gfx"
				}
				//将该阵法的gfx名字添加到gfx名字列表中
				AddCODGfxName( pCmd->caster, szCircleGfxName );
			}
			//Modified end.
		}
		//Added end.
		
		if (m_pCurSkill->IsChargeable())  // 蓄力技能
		{
			m_pCurSkill->SetStamp(pCmd->attack_stamp);
			if (pCmd->state == 0) // 第一阶段：吟唱
			{
				m_pCurSkill->StartChanting();
			}
			else if(pCmd->state == 1) // 第二阶段：蓄力
			{
				if (m_pWorkMan->GetCurWork()->GetWorkID() == CECHPWork::WORK_SPELLOBJECT)
				{
					((CECHPWorkSpell*)m_pWorkMan->GetCurWork())->SetChargeTime(pCmd->time);
					return;
				}
			}
		}
		int iWaitTime = -1;
		if (m_pCurSkill->GetExecuteTime() >= 0)
		{
			iWaitTime = pCmd->time + m_pCurSkill->GetExecuteTime();
		}
		LuaEvent::FirstCastSpecialSkill(m_pCurSkill->GetSkillID());

		if(pCmd->spirit_index >= 0)		// 由灵来释放技能
		{
			int aaa =1;
		}
		else
		{
			CECHPWorkSpell* pWork = (CECHPWorkSpell*)m_pWorkMan->CreateWork(CECHPWork::WORK_SPELLOBJECT);
			pWork->PrepareCast(pCmd->targets[0], m_pCurSkill, iWaitTime);
			m_pWorkMan->StartWork_p1(pWork);
		}

		//	Start time counter for some type skill
		//  希望所有的技能播放都显示进度条
//		if (!m_pCurSkill->IsChargeable() && !m_pCurSkill->ChangeToMelee())    
		{
			int iTime = pCmd->time;
			a_ClampFloor(iTime, 10);
			m_IncantCnt.SetPeriod(iTime);
			m_IncantCnt.Reset();
			
			if(m_pCurSkill->IsGuide())
			{
				int nPerformTime = m_pCurSkill->GetExecuteTime() - iTime;
				if(nPerformTime > 0)
				{
					m_PerformCnt.SetPeriod(m_pCurSkill->GetExecuteTime() - iTime);
					m_PerformCnt.Reset(true);
				}				
			}
			else
			{
				m_PerformCnt.SetPeriod(0);
				m_PerformCnt.Reset();
			}
		}
		
		// add chain gfx
		if(m_pCurSkill->GetRangeType() == GNET::TYPE_CHAIN)	// 链式攻击技能
		{
			RemoveChainGfxInfo(pCmd->caster);

			abase::vector<int> vecChainTargets;

			for(int i=0;i<pCmd->target_cnt;i++)
				vecChainTargets.push_back(pCmd->targets[i]);

			AddChainGfxInfo(pCmd->caster, vecChainTargets);
		}
		
		// 多条线形攻击技能
		if(m_pCurSkill->GetRangeType() == GNET::TYPE_MULTILINE)
		{
			//-----------------------------------------------------------------
			//更新多线性攻击技能特效 Added 2012-09-11.
			UpdateMultilineSkillGfx( pCmd->targets, pCmd->target_cnt );
			//-----------------------------------------------------------------
		}
		
		m_bSpellDSkill = false;
		m_idCurSkillTarget = pCmd->targets[0];

		// 判断是否需要灵模型来释放
		if(pCmd->spirit_index >= 0)	
		{
			CECLoginPlayer* pSpiritPlayer = CreateSpiritModel(pCmd->spirit_index);
			if(pSpiritPlayer)
			{
				pSpiritPlayer->SetPos(GetPos());
				pSpiritPlayer->SetVisible(true);
				pSpiritPlayer->SetCurSkill(m_pCurSkill);
				pSpiritPlayer->TurnFaceTo(pCmd->targets[0]);
				pSpiritPlayer->PlayAttackEffect(pCmd->targets[0], pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, 0, NULL);
			}
		}
		else
		{
			TurnFaceTo(pCmd->targets[0]);
			PlayAttackEffect(pCmd->targets[0], pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, 0, NULL, -1, &(pCmd->cast_skill_pos));
		}

//		g_pGame->RuntimeDebugInfo(0xffffffff, _AL("Cast skill: %s %d"), g_pGame->GetSkillDesc()->GetWideString(m_pCurSkill->GetSkillID() * 10), GetTickCount());
		g_pGame->RuntimeDebugInfo(0xffffffff, _AL("Cast skill: %s "), g_pGame->GetSkillDesc()->GetWideString(m_pCurSkill->GetSkillID() * 10));
		
		break;
	}
	case SKILL_PERFORM:
	{
		m_pPrepSkill = NULL;

		if (m_pCurSkill && m_pCurSkill->IsDurative())
			m_bSpellDSkill = true;

		break;
	}
	case HOST_STOP_SKILL:
	{
		m_pPrepSkill = NULL;

		cmd_host_stop_skill* pCmd = (cmd_host_stop_skill*)Msg.dwParam1;
		if(!pCmd)
			return;

		int iFollowSkillID = 0;
		if (m_pCurSkill)
		{
			if(m_pCurSkill->GetRangeType() == GNET::TYPE_CHAIN)
			{
				RemoveChainGfxInfo(GetCharacterID());
			}

			if (m_pPlayerModel)
				m_pPlayerModel->ClearComActFlag(true);
			
			if (((m_pComboSkill && !m_pComboSkill->IsStop()) || 
				m_pCurSkill->ChangeToMelee()) && !m_pWorkMan->GetDelayedWork())
			{
				bDoOtherThing = true;
				idTarget = m_idCurSkillTarget;
			}

			m_pCurSkill->EndCharging();

			iFollowSkillID = m_pCurSkill->GetFollowSkill();

			m_pCurSkill = NULL;
		}

		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SPELLOBJECT)
		{
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_SPELLOBJECT, false);
//			m_PerformCnt.Reset();
		}

		if(pCmd->spirit_index >=0)
		{
			CECLoginPlayer* pSpiritPlayer = m_SpiritModels[pCmd->spirit_index];
			if(pSpiritPlayer)
			{
				pSpiritPlayer->SetVisible(false);
			}
		}

		//Added 2011-06-17.
		m_fSkillSpeedRate = 1.0f;
		//Added end.
		
		//---------------------------------------------------------------------
		//如果无目标释放技能特效还在播放，则停止
		if ( m_pCastPosSkillGFX && (m_pCastPosSkillGFX->GetState() != ST_STOP) )
		{
			m_pCastPosSkillGFX->Stop();
		}
		//---------------------------------------------------------------------
		
		if(iFollowSkillID != 0)
		{
			ApplySkillShortcut(iFollowSkillID);
		}

		m_idCurSkillTarget = 0;
		break;
	}
	case SELF_SKILL_INTERRUPTED:
	{
		m_pPrepSkill = NULL;

		if (m_pCurSkill)
		{
			if(m_pCurSkill->GetRangeType() == GNET::TYPE_CHAIN)
			{
				RemoveChainGfxInfo(GetCharacterID());
			}

			if( m_pPlayerModel )
				m_pPlayerModel->ClearComActFlag(false);
			
			if (((m_pComboSkill && !m_pComboSkill->IsStop()) ||
				m_pCurSkill->ChangeToMelee()) && !m_pWorkMan->GetDelayedWork())
			{
				bDoOtherThing = true;
				idTarget = m_idCurSkillTarget;
			}

			m_pCurSkill->EndCharging();
			if(IsLiuDaoSkill(m_pCurSkill->GetSkillID()))
			{
				CECAutoHelper* pECHelper = g_pGame->GetGameRun()->GetHelper();
				if(pECHelper)
					pECHelper->SetLiuDaoSkillCnt(true);
			}

			m_pCurSkill = NULL;
		}

		m_idCurSkillTarget = 0;
		
		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SPELLOBJECT)
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_SPELLOBJECT, true);

		//	Print a notify message
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_SKILLINTERRUPT);
		break;
	}
	case OBJECT_CAST_INSTANT_SKILL:
	{
	//	m_pPrepSkill = NULL;

		cmd_object_cast_instant_skill* pCmd = (cmd_object_cast_instant_skill*)Msg.dwParam1;
		ASSERT(pCmd->caster == m_PlayerInfo.cid);

		CECSkill* pSkill = GetPositiveSkillByID(pCmd->skill);
		if ( !pSkill )
		{
//			ASSERT(pSkill);
			return;
		}

		if (pSkill->IsCharge())
			return;

		if (pCmd->targets[0] && pCmd->targets[0] != m_PlayerInfo.cid)
			TurnFaceTo(pCmd->targets[0]);

		if( pSkill->GetType() == CECSkill::TYPE_JUMP && !m_pPetModel )
		{
			ECMSG Msg;
			Msg.dwParam1 = MAKEFOURCC('Q', 'I', 'N', 'G');
			float dist = pSkill->GetCastRange(1.0f);
			Msg.dwParam2 = *(DWORD *)&dist;
			OnMsgHstJump(Msg);
		}
// 		if( pSkill->GetType() == CECSkill::TYPE_RUSH && !m_pPetModel)
// 		{
// 			return;
// 			ECMSG Msg;
// 			Msg.dwParam1 = MAKEFOURCC('R', 'U', 'S', 'H');
// 			float dist = 0.0f;
// 			if(!pCmd->target)
// 				return;
// 			CalcDist(pCmd->target, &dist);
// 			Msg.dwParam2 = *(DWORD*)&dist;
// 
// 			CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target,1);
// 			A3DVECTOR3 vMoveDest;
// 			if(ISNPCID(pCmd->target))
// 				vMoveDest = ((CECNPC*)pObject)->GetPos();
// 			else if(ISPLAYERID(pCmd->target))
// 				vMoveDest = ((CECPlayer*)pObject)->GetPos();
// 			else
// 				return;  // 选中目标不对	
// 			
// 			m_pCDRInfo->vTPNormal	= m_MoveCtrl.m_vFlashTPNormal;
// 			m_pCDRInfo->bOnSurface	= m_MoveCtrl.m_bFlashOnGnd;
// 			m_pCDRInfo->bCanStay	= m_MoveCtrl.m_bFlashCanStay;
// 
// 			CECHPWorkFMove* pWork = (CECHPWorkFMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_FLASHMOVE);
// 			pWork->PrepareMove(vMoveDest, 1.0f);
// 			m_pWorkMan->StartWork_p2(pWork);
// 			
// 			OnMsgHstJump(Msg);
// 		}
	//	m_idCurSkillTarget = pCmd->target;
		// PlayAttackEffect(pCmd->target, pCmd->skill, pCmd->level, pCmd->attack_stamp, -1, 0);

		break;
	}
	case OBJECT_CAST_POS_SKILL:
	{
		cmd_object_cast_pos_skill* pCmd = (cmd_object_cast_pos_skill*)Msg.dwParam1;
		ASSERT(pCmd->caster == m_PlayerInfo.cid);

		CECSkill* pSkill = GetSkill(pCmd->skill);
		if (!pSkill)
		{
			ASSERT(pSkill);
			break;
		}

	//	SetPos(pCmd->pos);
	//	UpdateFollowCamera(false, 10000);
		m_pCDRInfo->vTPNormal	= m_MoveCtrl.m_vFlashTPNormal;
		m_pCDRInfo->bOnSurface	= m_MoveCtrl.m_bFlashOnGnd;
		m_pCDRInfo->bCanStay	= m_MoveCtrl.m_bFlashCanStay;

		CECHPWorkFMove* pWork = (CECHPWorkFMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_FLASHMOVE);
		pWork->PrepareMove(pCmd->pos, pSkill->GetExecuteTime() * 0.001f);
		m_pWorkMan->StartWork_p2(pWork);
		break;
	}
	case SKILL_CONTINUE:
	{
		skill_continue* pCmd = (skill_continue*)Msg.dwParam1;
		ASSERT(pCmd->caster == m_PlayerInfo.cid);

		if(!m_pCurSkill)
		{
			ASSERT(m_pCurSkill);
			break;
		}
	}
	case SKILL_PROFICIENCY:
	{
		cmd_skill_proficiency* pCmd = (cmd_skill_proficiency*)Msg.dwParam1;
		CECSkill* pSkill = GetSkill(pCmd->skill_id);
		pSkill->SetProficiency(pCmd->proficiency);
		break;
	}
	case PLAYER_CHARGE:
	{
		player_charge* pCmd = (player_charge*)Msg.dwParam1;

		//在之前瞬移技能的基础上加上真正的冲锋技能效果（共用同一个协议）Modified 2011-07-22.
		if ( !pCmd->type )	// 瞬移技能
		{
			SetPos(pCmd->target_pos);

			UpdateFollowCamera(false, 10);

			CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target_id, 0);
			if (!pObject)
				return;
			SetDirAndUp(pObject->GetDir(), g_vAxisY);
		}
		else if( 1 == pCmd->type || 3 == pCmd->type || 4 == pCmd->type || 5 == pCmd->type)	// 冲锋技能  3：吸引 4:弹飞 等动画资源
		{
/*			ECMSG Msg;
 			Msg.dwParam1 = MAKEFOURCC('R', 'U', 'S', 'H');
			float dist = 0.0f;
			dist = (GetPos() - pCmd->target_pos).Magnitude();
 			//CalcDist(pCmd->target_id, &dist);
 			Msg.dwParam2 = *(DWORD*)&dist;
*/
 /*			CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->target_id,1);
			A3DVECTOR3 vMoveDest, vMoveDir;
 			if(ISNPCID(pCmd->target_id))
			{
 				vMoveDest = ((CECNPC*)pObject)->GetPos();
				vMoveDir = ((CECNPC*)pObject)->GetDir();
			}
 			else if(ISPLAYERID(pCmd->target_id))
			{
 				vMoveDest = ((CECPlayer*)pObject)->GetPos();
				vMoveDir = ((CECPlayer*)pObject)->GetDir();
			}
 			else
			{
 				return false;  // 选中目标不对	
			}
			//---------------
			A3DVECTOR3 vMoveTmp = vMoveDest;
			vMoveDir.y = 0.0f;
			vMoveDir.Normalize();
			vMoveTmp -= vMoveDir;
 
			A3DVECTOR3 vCanGoPos = m_MoveCtrl.TryToGo( vMoveDest );
			float fDelta = a3d_MagnitudeH(vMoveDest - vCanGoPos);
			if ( (ISNPCID(m_idSelTarget)&&(fDelta > 0.95f)) || (ISPLAYERID(m_idSelTarget)&&(fDelta > 0.15f)) )
			{
				return false;
			}
*/			//---------------
 			
 			m_pCDRInfo->vTPNormal	= m_MoveCtrl.m_vFlashTPNormal;
 			m_pCDRInfo->bOnSurface	= m_MoveCtrl.m_bFlashOnGnd;
			m_pCDRInfo->bCanStay	= m_MoveCtrl.m_bFlashCanStay;

			CECHPWorkFMove* pWork = (CECHPWorkFMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_FLASHMOVE);
 			pWork->PrepareMove(pCmd->target_pos, 1.0f);
			m_pWorkMan->StartWork_p2(pWork);
 			
 //			OnMsgHstJump(Msg);			
		}
		else if(2 == pCmd->type )	// 闪现技能
		{
			SetPos(pCmd->target_pos);

			UpdateFollowCamera(false, 10);

			// 可能需要播放一些特效	
			m_pPlayerModel->PlayGfx("程序联入\\闪现.gfx", NULL);//"程序联入\\闪现.gfx"

		}
		else if(6 == pCmd->type)
		{
			CECHPWorkParabolic* pWork = (CECHPWorkParabolic*)m_pWorkMan->CreateWork(CECHPWork::WORK_PARABOLIC);
			pWork->PrepareParabolic(1500, 5, pCmd->target_pos);
			m_pWorkMan->StartWork_p1(pWork);
		}

		//Anyway,let's have a break;
		break;
	}
	case ERROR_MESSAGE:
	
	//	m_pPrepSkill = NULL;
		break;

	case PLAYER_STOP_SKILL:	//HOST_STOP_SKILL已经处理了，这里不需要处理？gs广播时包含了自己。。
		break;
	
	default:
		ASSERT(0);
		break;
	}

	if (bDoOtherThing)
	{
		if (m_pComboSkill)
			m_pComboSkill->Continue(false);
		else if (!g_pGame->GetGameRun()->GetHelper()->IsRunning())
			NormalAttackObject(idTarget, GetForceAttackFlag(NULL));
	}
}

void CECHostPlayer::OnMsgPlayerStartAtk(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_start_attack* pCmd = (cmd_object_start_attack*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->idAttacker == m_PlayerInfo.cid);

	//	Face to target
	TurnFaceTo(pCmd->idTarget);
	PlayAttackEffect(pCmd->idTarget, 0, 0, pCmd->attack_stamp, -1, 0);

	// just ignore this latent message
	if( !m_bAboutToFight )
		return;

	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_HACKOBJECT)
	{
		CECHPWorkMelee* pCurWork = (CECHPWorkMelee*)m_pWorkMan->GetCurWork();
		if (pCurWork->GetTarget() == pCmd->idTarget)
		{
			m_bMelee = true;
			return;		//	We are attacking specified target, return directly
		}
	}

	CECHPWorkMelee* pWork = (CECHPWorkMelee*)m_pWorkMan->CreateWork(CECHPWork::WORK_HACKOBJECT);
	m_pWorkMan->StartWork_p1(pWork);
	m_bMelee = true;
	LuaEvent::FirstAttack();
	NewHelpGuide::StartGuide_Step_1(GUIDE_HELP_USESKILL_1);
}

void CECHostPlayer::OnMsgPlayerAtkResult(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_atk_result* pCmd = (cmd_object_atk_result*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->attacker_id == m_PlayerInfo.cid);

	if (pCmd->attack_flag & MOD_RETORT)
	{
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, pCmd->damage, 1, pCmd->attack_flag);
		AfxSkillGfxShowCaster(GetPlayerInfo().cid, pCmd->attack_flag);
	}
	else if (m_pPlayerModel)
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag, pCmd->damage);

	m_bMelee = false;

	if (m_pComboSkill && !m_pComboSkill->IsStop())
		m_pComboSkill->Continue(true);
	
	//	If host is attacking another player, reset player attacking counter
	if (ISPLAYERID(pCmd->target_id))
		m_ReAtkCnt.Reset();
}

void CECHostPlayer::OnMsgPlayerGather(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == PLAYER_GATHER_START)
	{
		cmd_player_gather_start* pCmd = (cmd_player_gather_start*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->pid == m_PlayerInfo.cid);

		CECHPWorkPick* pWork = (CECHPWorkPick*)m_pWorkMan->CreateWork(CECHPWork::WORK_PICKUP);
		if (pWork)
		{
			pWork->SetGather(true);
			m_pWorkMan->StartWork_p1(pWork);
		}

		//	Start time counter
		m_GatherCnt.SetPeriod(pCmd->use_time * 1000);
		m_GatherCnt.Reset();
	}
	else if (Msg.dwParam2 == PLAYER_GATHER_STOP)
	{
		cmd_player_gather_stop* pCmd = (cmd_player_gather_stop*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->pid == m_PlayerInfo.cid);
		m_pWorkMan->FinishCurWork(CECHPWork::WORK_PICKUP, false);
	}
}

void CECHostPlayer::OnMsgPickupMatter(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_matter_pickup* pCmd = (cmd_matter_pickup*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->who == m_PlayerInfo.cid);

	CECHPWorkPick* pWork = (CECHPWorkPick*)m_pWorkMan->CreateWork(CECHPWork::WORK_PICKUP);
	if (pWork)
	{
		pWork->SetGather(false);
		m_pWorkMan->StartWork_p1(pWork);
	}
}

void CECHostPlayer::OnMsgPlayerDoEmote(const ECMSG& Msg)
{
	using namespace S2C;

	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND && 
		m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_BEBOUND)
		return;

	CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();

	if (Msg.dwParam2 == OBJECT_DO_EMOTE)
	{
		cmd_object_do_emote* pCmd = (cmd_object_do_emote*)Msg.dwParam1;
		ASSERT(pCmd && pCmd->id == m_PlayerInfo.cid);

		DoEmote(pCmd->emotion);

		if( m_iBuddyId )
		{
			CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_iBuddyId);
			pBuddy->DoEmote(pCmd->emotion);
		}
	}
	else if (Msg.dwParam2 == OBJECT_EMOTE_RESTORE)
	{
		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_STAND)
		{
			CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();
			pWork->SetPoseAction(ACT_STAND, false);
		}
	}
}

void CECHostPlayer::OnMsgDoConEmote(const ECMSG& Msg)
{
	using namespace S2C;

	if (Msg.dwParam2 == CON_EMOTE_REQUEST)
	{
		cmd_con_emote_request* pCmd = (cmd_con_emote_request*)Msg.dwParam1;
		ASSERT(pCmd);
	}
	else if (Msg.dwParam2 == DO_CONCURRENT_EMOTE)
	{
		cmd_do_concurrent_emote* pCmd = (cmd_do_concurrent_emote*)Msg.dwParam1;
		ASSERT(pCmd);

		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_STAND)
		{
			CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();
			pWork->SetPoseAction(pCmd->emotion, false);
		}
	}
}

void CECHostPlayer::OnMsgPlayerChangeShape(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_chgshape* pCmd = (cmd_player_chgshape*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->idPlayer == m_PlayerInfo.cid);
	TransformShape(pCmd->shape, true);
}

void CECHostPlayer::OnMsgBoothOperation(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch (Msg.dwParam2)
	{
	case OPEN_BOOTH_TEST:
	{
		cmd_open_booth_test* pCmd = (cmd_open_booth_test*)Msg.dwParam1;
		//	Re-arrange booth sell and buy package so that items in booth
		//	last can be kept as many as possible.
		if(m_idBoothItem == pCmd->item_id)
		{
			RearrangeBoothPacks();
		
			m_iBoothState = 1;
			pGameUI->PopupBoothDialog(true, true, true);
		}
		break;
	}
	case SELF_OPEN_BOOTH:
	{	
		cmd_self_open_booth* pCmd = (cmd_self_open_booth*)Msg.dwParam1;
		if(m_idBoothItem == pCmd->item_id)
		{
			DATA_TYPE DataType;
			BOOTH_FIGURE_ITEM_ESSENCE* pDBEssence = (BOOTH_FIGURE_ITEM_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pCmd->item_id, ID_SPACE_ESSENCE, DataType);
			if(pCmd->item_id != -1)
				m_pSpeBoothModel = m_pPlayerMan->LoadSpeBoothModel(pDBEssence->file_model);
			m_iBoothState = 2;
			pGameUI->PopupBoothDialog(true);
		}
		break;
	}
	case PLAYER_CLOSE_BOOTH:
	{
		cmd_player_close_booth* pCmd = (cmd_player_close_booth*)Msg.dwParam1;
		if (pCmd->pid == m_PlayerInfo.cid)
		{
			m_iBoothState = 0;
			pGameUI->PopupBoothDialog(false);
		}
		else
		{
			ASSERT(0);
		}
		
		break;
	}
	case BOOTH_SELL_ITEM:
	{
		cmd_booth_sell_item* pCmd = (cmd_booth_sell_item*)Msg.dwParam1;
		ASSERT(pCmd);

		//	Give player a notify message
		int iBoothIndex = m_pBoothSPack->GetItemIndex(pCmd->inv_index);
		CECIvtrItem* pBoothItem = m_pBoothSPack->GetItem(iBoothIndex);
		if (pBoothItem)
		{
			struct tm* time = glb_GetFormatLocalTime();
			ACString strMsg;
			ACHAR szPrice[256];
			CECIvtrItem::GetPriceText(szPrice, pBoothItem->GetUnitPrice());
			strMsg.Format(g_pGame->GetFixedMsgTab()->GetWideString(FIXMSG_BOOTHSELL), 
				 time->tm_hour, time->tm_min, g_pGame->GetGameRun()->GetPlayerName(pCmd->idBuyer, true), pCmd->item_count, pBoothItem->GetName(), szPrice);
			pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_OK", strMsg, 0x0fffffff, 0, 0, 0);
			PAUIDIALOG pDlg = pGameUI->GetDialog("Win_PShop1");
			PAUITEXTAREA pMsgList = (PAUITEXTAREA)pDlg->GetDlgItem("Txt_MsgList");
			strMsg += _AL('\r');
			pMsgList->AppendText(strMsg);
			pGameUI->m_pDlgMinimizeBar[0]->FlashDialog(pDlg);

		}

		//	Remove item from booth package
		m_pBoothSPack->RemoveItem(pCmd->inv_index, pCmd->item_count);

		//	Get item information
		CECIvtrItem* pItem = m_pPack->GetItem(pCmd->inv_index);
		if (pItem)
		{
			//	Keep item's template id
			int tid = pItem->GetTemplateID();
			m_pPack->RemoveItem(pCmd->inv_index, pCmd->item_count);

			//	Update shortcuts if all items have been sold
			if (!m_pPack->GetItem(pCmd->inv_index))
				UpdateRemovedItemSC(tid, IVTRTYPE_PACK, pCmd->inv_index);
		}
		else
		{
			ASSERT(pItem);
		}

		break;
	}
	case PLAYER_BOOTH_INFO:
	{
		cmd_player_booth_info* pCmd = (cmd_player_booth_info*)Msg.dwParam1;
		ASSERT(pCmd);

		if (pCmd->pid == m_PlayerInfo.cid)
		{
			ASSERT(pCmd->pid != m_PlayerInfo.cid);
			return;
		}

		m_pEPBoothSPack->RemoveAllItems();
		m_pEPBoothBPack->RemoveAllItems();

		m_idBoothTrade = pCmd->booth_id;
		BYTE* pData = (BYTE*) &pCmd->goods_list;
		int iCount = 0;

		while (iCount < (int)pCmd->count)
		{
			int idItem = *(int*)pData;
			pData += sizeof (int);

			if (idItem)
			{
				int iAmount = *(int*)pData;
				pData += sizeof (int);
				int iPrice = *(int*)pData;
				pData += sizeof (int);

				if (iAmount > 0)
				{
					int iExpireDate = *(int*)pData;
					pData += sizeof (int);

					int iState = *(int*)pData;
					pData += sizeof (int);

					WORD wDataLen = *(WORD*)pData;
					pData += sizeof (WORD);
					m_pEPBoothSPack->AddEPBoothItem(idItem, iExpireDate, iState, iCount, iAmount, iPrice, pData, wDataLen);
					pData += wDataLen;
				}
				else
				{
					m_pEPBoothBPack->AddEPBoothItem(idItem, 0, 0, iCount, -iAmount, iPrice, NULL, 0);
				}
			}

			iCount++;
		}

		break;
	}
	case BOOTH_TRADE_SUCCESS:

		if (m_iBoothState == 3)	//	If we are visiting other's booth
			g_pGame->GetGameSession()->c2s_CmdNPCSevGetContent(GP_NPCSEV_BOOTHSELL);

		break;
	}
}

void CECHostPlayer::OnMsgPlayerTravel(const ECMSG& Msg)
{
	using namespace S2C;
}

void CECHostPlayer::OnMsgPlayerPVP(const ECMSG& Msg)
{
	CECPlayer::OnMsgPlayerPVP(Msg);

	using namespace S2C;

	if (Msg.dwParam2 == HOST_PVP_COOLDOWN)
	{
		cmd_host_pvp_cooldown* pCmd = (cmd_host_pvp_cooldown*)Msg.dwParam1;
		m_pvp.dwCoolTime	= pCmd->cool_time * 1000;
		m_pvp.dwMaxCoolTime	= pCmd->max_cool_time * 1000;
	}
	else if (Msg.dwParam2 == HOST_ENABLE_FREEPVP)
	{
		cmd_host_enable_freepvp* pCmd = (cmd_host_enable_freepvp*)Msg.dwParam1;
		m_pvp.ucFreePVPType = pCmd->enable_type;
	}
}

void CECHostPlayer::OnMsgPlayerUseItem(const ECMSG& Msg)
{
	using namespace S2C;

	switch (Msg.dwParam2)
	{
	case OBJECT_START_USE:
	case OBJECT_START_USE_T:
	{
		int idTarget, idItem;
		DWORD dwTime;

		if (Msg.dwParam2 == OBJECT_START_USE)
		{
			cmd_object_start_use* pCmd = (cmd_object_start_use*)Msg.dwParam1;
			idTarget	= 0;
			idItem		= pCmd->item;
			dwTime		= pCmd->time;
		}
		else
		{
			cmd_object_start_use_t* pCmd = (cmd_object_start_use_t*)Msg.dwParam1;
			idTarget	= pCmd->target;
			idItem		= pCmd->item;
			dwTime		= pCmd->time;
		}

		CECHPWorkUse* pNewWork = (CECHPWorkUse*)m_pWorkMan->CreateWork(CECHPWork::WORK_USEITEM);
		pNewWork->SetParams(idItem, dwTime, idTarget);
		m_pWorkMan->StartWork_p1(pNewWork);
		break;
	}
	case OBJECT_CANCEL_USE:
	{
		m_pWorkMan->FinishCurWork(CECHPWork::WORK_USEITEM, true);
		break;
	}
	case OBJECT_USE_ITEM:

		ASSERT(0);
		return;
	}
}

void CECHostPlayer::OnMsgPlayerChangeFace(const ECMSG& Msg)
{
	using namespace S2C;
	cmd_player_chg_face* pCmd = (cmd_player_chg_face*)Msg.dwParam1;
//	m_PlayerInfo.crc_c = pCmd->crc_c;
}

void CECHostPlayer::OnMsgPlayerBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACString strMsg;
	switch (Msg.dwParam2)
	{
	case PLAYER_BIND_REQUEST:
	{
		cmd_player_bind_request* pCmd = (cmd_player_bind_request*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(680), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_RequestBind", 
			strMsg, 30000, pCmd->who, 0, 0);
		break;
	}
	case PLAYER_BIND_INVITE:
	{
		cmd_player_bind_invite* pCmd = (cmd_player_bind_invite*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(681), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteBind", 
			strMsg, 30000, pCmd->who, 0, 0);
		break;
	}
	case PLAYER_BIND_REQUEST_REPLY:
	{
		cmd_player_bind_request_reply* pCmd = (cmd_player_bind_request_reply*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(682), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		break;
	}
	case PLAYER_BIND_INVITE_REPLY:
	{
		cmd_player_bind_invite_reply* pCmd = (cmd_player_bind_invite_reply*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(682), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		break;
	}
	case PLAYER_BIND_STOP:
	{
		m_idCandBuddy = 0;
		m_bCandHangerOn = false;

		//	Restore buddy's transparence
		if (m_iBuddyId)
		{
			CECElsePlayer* pBuddy = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
			pBuddy->SetTransparent(-1.0f);
		}

		//	Detach buddy
		if (!m_bHangerOn)
			DetachBuddy(NULL);

		//	Stop current work and restore to stand state
		if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND)
			m_pWorkMan->FinishAllWork(true);

		break;
	}
	}
}
void CECHostPlayer::OnMsgPlayerBathOpt(const ECMSG& Msg)
{
	using namespace S2C;
	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACString strMsg;
	switch(Msg.dwParam2)
	{ 
		//邀请搓澡
	    case PLAYER_BATH_INVITE:
		{
			 player_bath_invite* pCmd = (player_bath_invite*)Msg.dwParam1;
			 pGameUI->PopupKingBathInvite(g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true),pCmd->who);
			 break;
		}
		//邀请搓澡回复
	    case PLAYER_BATH_INVITE_REPLAY:
		{
			 player_bath_invite_reply* pCmd = (player_bath_invite_reply*)Msg.dwParam1;
			 if(pCmd->param != 0)
			 {
			   strMsg.Format(pGameUI->GetStringFromTable(15670),g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
			   pGameUI->MessageBox("", strMsg, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			 }
			 break;
		}
		//进入泡澡地图
	    case ENTER_BATH_WORLD:
		{
             enter_bath_world* pCmd = (enter_bath_world*)Msg.dwParam1;
             m_iBathType = BATH_TYPE_NORMAL;
			 pGameUI->CommonMessage("Win_King_ShowerInvite","enter",pCmd->remain_bath_count,0);
			 {
				 int aEquips[SIZE_EQUIPIVTR];
				 memset(aEquips, 0, sizeof(aEquips));
				 m_dwHideEquipMaskBackUp = m_dwHideEquipMask;
				 m_bFashionModeBackUp	 = m_bFashionMode;
				 m_bFashionMode			 = true;   // 强制为时装模式
			     m_dwHideEquipMask = 0;
				 memcpy(aEquips,m_EquipsInBath[GetGender()],sizeof(m_EquipsInBath[GetGender()]));
				 ShowEquipments(aEquips, true, true);
			 }
			 break; 
		}
		//离开泡澡地图
	    case LEAVE_BATH_WORLD:
		{
			 m_iBathType = -1;
			 if(!(m_BasicProps.iProfession >= 108 && m_BasicProps.iProfession <= 112))
			 {
				 m_dwHideEquipMask = m_dwHideEquipMaskBackUp;
				 m_bFashionMode    = m_bFashionModeBackUp;
				 UpdateEquipSkins(true);
			 }
			 break;
		}
		//搓澡结束
	    case PLAYER_BATH_STOP:    
		{
			 //player_bath_stop* pCmd = (player_bath_stop*)Msg.dwParam1;
			 m_idCandBathBuddy = 0;
			 m_bCandHangerOn = false;
			 m_bIsBathing = false;
			 m_GatherCnt.Reset(true);
			 
			 pGameUI->CommonMessage("Win_King_ShowerInvite","end",0,0);
			 //	Restore buddy's transparence
			 if (m_iBuddyId)
			 {
				 CECElsePlayer* pBuddy = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
				 pBuddy->SetTransparent(-1.0f);
			 }
			 
			 //	Detach buddy
			 if (!m_bHangerOn)
				 DetachBathBuddy(NULL);
			 
			 //	Stop current work and restore to stand state
			 if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND)
			 {
				 m_pWorkMan->FinishAllWork(true);
			 }
			 else
			 {
				 CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();
				 pWork->SetPoseAction(ACT_STAND, false);
	
			 }
			 
		break;
			 break;
		}
	  
	}
}

void CECHostPlayer::OnMsgPlayerQilinBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	
	ACString strMsg;
	switch(Msg.dwParam2)
	{
	case PLAYER_QILIN_INVITE:
	{
		player_qilin_invite* pCmd = (player_qilin_invite*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(21501), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_QilinInviteBind", strMsg, 30000,pCmd->who, 0, 0);
		break;
	}
	case PLAYER_QILIN_INVITE_REPLY:
	{		
		player_qilin_invite_reply* pCmd = (player_qilin_invite_reply*)Msg.dwParam1;
		if(pCmd->param != 0)
		{
			strMsg.Format(pGameUI->GetStringFromTable(21502), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
			pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		}
		break;
	}
	case PLAYER_QILIN_STOP:
	{
		CECPlayer* pBuddy = m_pPlayerMan->GetPlayer(m_idQilinConnectBuddy);
		if(pBuddy)
		{
			pBuddy->m_idQilinConnectBuddy = 0;
			pBuddy->m_bQilinConnectHangerOn = false;
		}
		m_idQilinConnectBuddy = 0;
		m_bQilinConnectHangerOn = false;
		m_idCandQilinBuddy = 0;
		m_bCandHangerOn = false;

		//	Restore buddy's transparence
		if (m_iBuddyId)
		{
			CECElsePlayer* pBuddy = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
			pBuddy->SetTransparent(-1.0f);
		}

		//	Detach buddy
		RemovePullGfxInfo(m_PlayerInfo.cid);
		DetachQilinBuddy(NULL);

		//	Stop current work and restore to stand state
		if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND && !IsDead())
			m_pWorkMan->FinishAllWork(true);					
		
		break;
	}
	}
}

// 多人骑乘
void CECHostPlayer::OnMsgPlayerMultiBindOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACString strMsg;
	switch (Msg.dwParam2)
	{
	case PLAYER_MULTI_BIND_INVITE:
	{
		cmd_player_bind_invite* pCmd = (cmd_player_bind_invite*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(7103), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteRideTogether", 
			strMsg, 30000, pCmd->who, 0, 0);
		break;
	}
	case PLAYER_MULTI_BIND_INVITE_REPLY:
	{
		cmd_player_bind_invite_reply* pCmd = (cmd_player_bind_invite_reply*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(7104), g_pGame->GetGameRun()->GetPlayerName(pCmd->who, true));
		pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		break;
	}
	case PLAYER_MULTI_BIND_JOIN:
	{
		cmd_player_multi_bind_join* pCmd = (cmd_player_multi_bind_join*)Msg.dwParam1;
		
		m_iMultiCandBuddyId[pCmd->pos] = pCmd->member;

		// 将队友加入到主骑的临时好友中
		const ACHAR* pName =  g_pGame->GetGameRun()->GetPlayerName(pCmd->member, false);
		m_pFriendMan->AddTempFriend(pCmd->member, pName);

		//strMsg.Format(pGameUI->GetStringFromTable(681), g_pGame->GetGameRun()->GetPlayerName(pCmd->leader, true));
		//pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		break;
	}
	case PLAYER_MULTI_BIND_LEAVE:
	{
		cmd_player_multi_bind_leave* pCmd = (cmd_player_multi_bind_leave*)Msg.dwParam1;
		//strMsg.Format(pGameUI->GetStringFromTable(681), g_pGame->GetGameRun()->GetPlayerName(pCmd->leader, true));
		//pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		// 如果是leader离开，则所有的member都要被解除绑定
		if( pCmd->leader == pCmd->member )           //主骑离开
		{	
			for(int i =1; i< MULTI_RIDE_CAPACITY; ++i)
			{
				if( m_iMultiBuddyId[i])
					DetachMultiBuddy(i);
			}
		}
		else                                          //一个成员离开
		{
			if (m_iMultiBuddyId[pCmd->pos])
				DetachMultiBuddy(pCmd->pos);
		}
		break;
	}
	case PLAYER_MULTI_BIND_KICK:
	{
		cmd_player_multi_bind_kick* pCmd = (cmd_player_multi_bind_kick*)Msg.dwParam1;
		DetachMultiBuddy(pCmd->pos);
		break;
	}
	}
}
// 新增载体（电梯）相关
void CECHostPlayer::OnMsgPlayerCarrierOpt(const ECMSG& Msg)
{
	using namespace S2C;

	switch(Msg.dwParam2)
	{
	case PLAYER_ENTER_CARRIER:
	{
		cmd_player_enter_carrier* pCmd = (cmd_player_enter_carrier*)Msg.dwParam1;
	
		if (!pCmd->success)
		{
			m_idCarrier = 0;
			g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Host enter carrier failed"));
		}
		else
		{
			m_idCarrier = pCmd->carrier_id;
			CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
			pNPCMan->CalcCarrierRelMat(m_idCarrier, GetAbsoluteTM(), m_matRelCarrier);

			g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Host enter carrier"));
		}
		
		SetCarrierCooling(false);
		char szMsg[100];
		char* szRst = pCmd->success ? "Succeed" : "Failed";
		sprintf(szMsg, "Enter Carrier Re : %s\n", szRst);
		OutputDebugStringA(szMsg);

		break;
	}
	case PLAYER_LEAVE_CARRIER:
	{
		cmd_player_leave_carrier* pCmd = (cmd_player_leave_carrier*)Msg.dwParam1;

		g_pGame->RuntimeDebugInfo(RTDCOL_NETWORK, _AL("NET - Host leave carrier"));

		m_idCarrier = 0;
		SetCarrierCooling(false);
		char szMsg[100];
		char* szRst = pCmd->success ? "Succeed" : "Failed";
		sprintf(szMsg, "Leave Carrier Re : %s\n", szRst);
		OutputDebugStringA(szMsg);

		break;
	}
	case PLAYER_MOVE_ON_CARRIER:
	{

		break;
	}
	case PLAYER_STOP_ON_CARRIER:
	{

		break;
	}
	default:
		ASSERT(0);
	    break;
	}
}

void CECHostPlayer::OnMsgPlayerDuelOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	ACString strMsg;

	switch (Msg.dwParam2)
	{
	case DUEL_RECV_REQUEST:
	{
		cmd_duel_recv_request* pCmd = (cmd_duel_recv_request*)Msg.dwParam1;
		strMsg.Format(pGameUI->GetStringFromTable(678), g_pGame->GetGameRun()->GetPlayerName(pCmd->player_id, true));
		pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteDuel", 
			strMsg, 30000, pCmd->player_id, 0, 0);
		LuaEvent::FirstPK();
		break;
	}
	case DUEL_REJECT_REQUEST:
	{
		cmd_duel_reject_request* pCmd = (cmd_duel_reject_request*)Msg.dwParam1;
		if( pCmd->reason == 2 )
			strMsg.Format(pGameUI->GetStringFromTable(679), g_pGame->GetGameRun()->GetPlayerName(pCmd->player_id, true));
		else
			strMsg.Format(pGameUI->GetStringFromTable(697), g_pGame->GetGameRun()->GetPlayerName(pCmd->player_id, true));
		pGameUI->AddChatMessage(strMsg, GP_CHAT_MISC);
		break;
	}
	case DUEL_PREPARE:
	{
		cmd_duel_prepare* pCmd = (cmd_duel_prepare*)Msg.dwParam1;
		m_pvp.iDuelState = DUEL_ST_PREPARE;
		m_pvp.iDuelTimeCnt = pCmd->delay * 1000;
		m_pvp.iDuelRlt = 0;
		break;
	}
	case DUEL_CANCEL:

		break;

	case HOST_DUEL_START:
	{
		cmd_host_duel_start* pCmd = (cmd_host_duel_start*)Msg.dwParam1;
		m_pvp.iDuelState = DUEL_ST_INDUEL;
		m_pvp.idDuelOpp = pCmd->idOpponent;
		m_pvp.iDuelTimeCnt = 0;
		break;
	}
	case DUEL_STOP:
	{
		m_pvp.iDuelState = DUEL_ST_STOPPING;
		m_pvp.iDuelTimeCnt = 5000;
		break;
	}
	}
}

void CECHostPlayer::OnMsgPlayerLevel2(const ECMSG& Msg)
{
	CECPlayer::OnMsgPlayerLevel2(Msg);

	using namespace S2C;
	cmd_task_deliver_level2* pCmd = (cmd_task_deliver_level2*)Msg.dwParam1;
		
	//	Print notify text
	int iIndex;
	if (pCmd->level2 <= 8)
		iIndex = FIXMSG_LEVEL2_01 + pCmd->level2;
	else if (pCmd->level2 >= 20 && pCmd->level2 <= 22)
		iIndex = FIXMSG_LEVEL2_20 + pCmd->level2 - 20;
	else if (pCmd->level2 >= 30 && pCmd->level2 <= 32)
		iIndex = FIXMSG_LEVEL2_30 + pCmd->level2 - 30;
	else
	{
		ASSERT(0);
		return;
	}
	
	const ACHAR* szStr = g_pGame->GetFixedMsgTab()->GetWideString(iIndex);
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVEL2UP, szStr);
}

void CECHostPlayer::OnMsgPlayerSkillResult(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_object_skill_attack_result* pCmd = (cmd_object_skill_attack_result*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->attacker_id == m_PlayerInfo.cid);

	if (pCmd->attack_flag & MOD_RETORT)
	{
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, pCmd->damage, 1, pCmd->attack_flag | MOD_SKILL);
		AfxSkillGfxShowDamage(GetPlayerInfo().cid, pCmd->target_id, pCmd->dt_damage, 1, pCmd->attack_flag | MOD_DEITY_DAMAGE);
		AfxSkillGfxShowCaster(GetPlayerInfo().cid, pCmd->attack_flag | MOD_SKILL);
	}
	else if (m_pPlayerModel)
	{
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag | MOD_SKILL, pCmd->damage);
		m_pPlayerModel->AddOneAttackDamageData(ACT_CHANNEL_BODY, GetPlayerInfo().cid, pCmd->target_id, pCmd->attack_stamp, pCmd->attack_flag | MOD_DEITY_DAMAGE, pCmd->dt_damage);
	}
}

void CECHostPlayer::OnMsgPlayerAchievement(const ECMSG& Msg)
{
	using namespace S2C;
	CECAchievementMan* pMan = GetAchievementMan();
	if(!pMan)
	{
		ASSERT(0);
		return;
	}
	switch (Msg.dwParam2)	
	{
	case PLAYER_ACHIEVEMENT:
		{
			player_achievement* pCmd = (player_achievement*)Msg.dwParam1;
			pMan->UpdateData(pCmd, Msg.dwParam3);
			break;
		}
    case PLAYER_ACHIEVEMENT_FINISH:
		{
			player_achievement_finish* pCmd = (player_achievement_finish*)Msg.dwParam1;
			pMan->UpdateData(pCmd, Msg.dwParam3);
			break;
		}
	case PLAYER_ACHIEVEMENT_ACTIVE:
		{
			player_achievement_active* pCmd = (player_achievement_active*)Msg.dwParam1;
			pMan->UpdateData(pCmd,Msg.dwParam3);
			break;
		}
	case PLAYER_PREMISS_DATA:
		{
			player_premiss_data* pCmd = (player_premiss_data*)Msg.dwParam1;
			pMan->UpdateData(pCmd, Msg.dwParam3);
			break;
		}
    case PLAYER_PREMISS_FINISH:   
		{
			player_premiss_finish* pCmd = (player_premiss_finish*)Msg.dwParam1;
			pMan->UpdateData(pCmd, Msg.dwParam3);
			break;
		}
	case ACHIEVEMENT_MESSAGE:
		{
			achievement_message* p = (achievement_message*)Msg.dwParam1;
			CECGameUIMan* pGameUI	= g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			if(p && pGameUI)
			{
				AWString name((wchar_t*)p->name,p->name_len/sizeof(wchar_t));
				if (name.GetLength()<=0)
				{
					name = _AL("error");
				}
				if (pMan->GetAchievement(p->achieve_id))
				{
					sAchievement* pAchievement = new sAchievement(*pMan->GetAchievement(p->achieve_id));
					pAchievement->iFinishTime = *(int*) (p->name + p->name_len);
					pAchievement->bFinish = true;
					pAchievement->strOwnerName = name;

					ACHAR szText[512];
					AUI_ConvertChatString(name, szText, false);
					AWString str;
					str.Format(pAchievement->strMsg, szText, AUICOMMON_ITEM_CODE_START, pAchievement->name);

					//str.Format(L"^ffff00恭喜 ^ff0000%s^ffff00 达成成就^00ff00 %c<6><%s><>", szText, AUICOMMON_ITEM_CODE_START, pAchievement->name);
					pGameUI->AddChatMessage(str, GP_CHAT_LOCAL, -1, NULL, 0, 0, NULL, pAchievement);
				}
			}
			break;
		}
	case OBTAIN_ACHIEVE_AWARD:
		{
			const player_obtain_achieve_award* pCmd = (const player_obtain_achieve_award*)Msg.dwParam1;
			pMan->UpdateData(pCmd, Msg.dwParam3);
			break;
		}
	}
}

void CECHostPlayer::OnMsgPlayerStartInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_start_interact* pCmd = (cmd_player_start_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	m_CandInteractorInfo.id = pCmd->obj_id;
	m_CandInteractorInfo.pos = pCmd->pos;
	m_CandInteractorInfo.state = 0;

	/*
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->StartInteract(m_PlayerInfo.cid, pCmd->pos);
	StartInteract(pCmd->obj_id, pCmd->pos);
	*/
}

void CECHostPlayer::OnMsgPlayerInInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_in_interact* pCmd = (cmd_player_in_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	m_CandInteractorInfo.id = pCmd->obj_id;
	m_CandInteractorInfo.pos = pCmd->pos;
	m_CandInteractorInfo.state = 1;

	/*
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->InInteract(m_PlayerInfo.cid, pCmd->pos);
	InInteract(pCmd->obj_id, pCmd->pos);
	*/
}

void CECHostPlayer::OnMsgPlayerEndInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_end_interact* pCmd = (cmd_player_end_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	m_CandInteractorInfo.id = pCmd->obj_id;
	m_CandInteractorInfo.pos = pCmd->pos;
	m_CandInteractorInfo.state = 2;

	/*
	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	pInteractor->EndInteract(m_PlayerInfo.cid, pCmd->pos);
	EndInteract(pCmd->obj_id, pCmd->pos);
	*/
}

void CECHostPlayer::OnMsgPlayerFinishInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_finish_interact* pCmd = (cmd_player_finish_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	// Clear candidate.
	m_CandInteractorInfo.id = 0;

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	//	Finish interactions.
	pInteractor->FinishInteract(m_PlayerInfo.cid, pCmd->pos);
	FinishInteract(pCmd->obj_id, pCmd->pos);

	//	Sync positions.
	pInteractor->SetPos(pCmd->obj_pos);
	pInteractor->SetServerPos(pCmd->obj_pos);
	SetPos(pCmd->player_pos);
}

void CECHostPlayer::OnMsgPlayerCancelInteract(const ECMSG& Msg)
{
	using namespace S2C;

	cmd_player_cancel_interact* pCmd = (cmd_player_cancel_interact*)Msg.dwParam1;
	ASSERT(pCmd && pCmd->player_id == m_PlayerInfo.cid);

	// Clear candidate.
	m_CandInteractorInfo.id = 0;

	CECInteractor* pInteractor = (CECInteractor*) g_pGame->GetGameRun()->GetWorld()->GetObject(pCmd->obj_id, 0);
	if (!pInteractor)
		return;

	//	Cancel interactions.
	pInteractor->CancelInteract(m_PlayerInfo.cid);
	CancelInteract(pCmd->obj_id);

	//	Sync positions.
	pInteractor->SetPos(pCmd->obj_pos);
	pInteractor->SetServerPos(pCmd->obj_pos);
	SetPos(pCmd->player_pos);
}

void CECHostPlayer::OnMsgHstTerritoryOpt(const ECMSG& Msg)
{
	using namespace GNET;
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case PROTOCOL_TERRITORYMAPGET_RE:
		{
			TerritoryMapGet_Re* p = (TerritoryMapGet_Re*)Msg.dwParam1;
			ACString strText;
			if (p->retcode>0)
			{
				strText = pGameUI->GetStringFromTable(9390+p->retcode);
				pGameUI->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255,255,255,160));
				break;
			}
			vector<DOMAIN_INFO>* infos = globaldata_getdomaininfos2();

			for(int i = 0; i< (int)p->tlist.size(); ++i)
			{ 
				for (int j = 0; j< (int)infos->size(); ++j)
				{
					int id = p->tlist[i].territoryid;
					if ((*infos)[j].id == id)
					{
						DOMAIN_INFO& info = (*infos)[j];
						info.id_owner		= p->tlist[i].owner;
						info.id_attacker	= p->tlist[i].attacker;
						info.id_defender	= p->tlist[i].defender;
						info.flag			= p->tlist[i].flag;
						info.battle_time_start	= p->tlist[i].battle_time;
						info.battle_time_end	= p->tlist[i].battle_end_time;
						info.deposit		= p->tlist[i].item_count;
						info.assis_num		= p->tlist[i].assis_num;
						info.assis_drawn_num= p->tlist[i].assis_drawn_num;
						info.attacker_num	= p->tlist[i].attacker_num;
						info.defender_num	= p->tlist[i].defender_num;
						info.random_award	= p->tlist[i].rand_award_itemid;
						info.random_award_num	= p->tlist[i].rand_award_count;
						info.random_award_drawn = p->tlist[i].rand_award_drawn;
						info.player_limit	= p->tlist[i].player_limit;
						info.color			= p->tlist[i].color;
						info.challenged_byself = p->tlist[i].challenged_by_selffaction;
					}
				}
			}
			if (pGameUI->GetDialog("Win_Battle_TerriWarArea")->IsShow())
			{
				pGameUI->GetDialog("Win_Battle_TerriWarArea")->Show(true);
			}
			CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)pGameUI->GetDialog("WorldMap_TerriSub");
			pDlgTerrMap->UpdateMapInfo(p->status, true);
			break;
		}
	case PROTOCOL_TERRITORYCHALLENGE_RE:
		{
			TerritoryChallenge_Re* p = (TerritoryChallenge_Re*)Msg.dwParam1;
			ACString strText;
			if (p->retcode>0)
			{
				strText = pGameUI->GetStringFromTable(9390+p->retcode);
			}
			else
			{
				vector<DOMAIN_INFO>* infos = globaldata_getdomaininfos2();
				for (int i = 0; i< (int)(*infos).size(); ++i)
				{
					if ((*infos)[i].id == p->territoryid)
					{
						(*infos)[i].deposit = p->itemcount;
						strText.Format(pGameUI->GetStringFromTable(9346), (*infos)[i].name,p->itemcount);
						break;
					}
				}
			}
			pGameUI->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255,255,255,160));
			CDlgTerrWarMap* pDlgTerrMap = (CDlgTerrWarMap*)pGameUI->GetDialog("WorldMap_TerriSub");
			pDlgTerrMap->UpdateMapInfo(-1, true);
			break;
		}
	case PROTOCOL_TERRITORYENTER_RE:
		{
			TerritoryEnter_Re* p = (TerritoryEnter_Re*)Msg.dwParam1;
			ACString strText;
			if (p->retcode>0)
			{
				strText = pGameUI->GetStringFromTable(9390+p->retcode);
				pGameUI->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255,255,255,160));
				break;
			}
		}
	case PROTOCOL_TERRITORYLEAVE_RE:
		{
			break;
		}
	case PROTOCOL_TERRITORYENTERREMIND:
		{
		TerritoryEnterRemind* p = (TerritoryEnterRemind*)Msg.dwParam1;
		pGameUI->MessageBox("GameOk_TerriRemind", pGameUI->GetStringFromTable(9436), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));

		break;
		}
	}
}

void CECHostPlayer::OnMsgHstCircleOpt(const ECMSG& Msg)
{
	using namespace GNET;
	using namespace S2C;

	const circle_info_notify* pNotify;
	switch(Msg.dwParam2)
	{
	case CIRCLE_INFO_NOTIFY:
		{
			pNotify = reinterpret_cast<const circle_info_notify*>(Msg.dwParam1);

			if (pNotify->idPlayer == m_PlayerInfo.cid)
			{
				SetCircleID(pNotify->idCircle);
				SetCRole(pNotify->cTitle);		
			}
			else
			{
				CECElsePlayer* pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetElsePlayer(pNotify->idPlayer);

				if (pPlayer)
				{
					pPlayer->SetCircleID(pNotify->idCircle);
					pPlayer->SetCRole(pNotify->cTitle);
				}
			}

			if (pNotify->idCircle)
				g_pGame->GetCircleMan()->GetCircle(pNotify->idCircle, true);
			
			break;
		}
	case DELIVERY_EXP_NOTIFY:
		{
			receive_deliveryd_exp* p = (receive_deliveryd_exp*)(Msg.dwParam1);
			ASSERT(p->exp >= 0);
			if(p->award_type == 1)//快雪
				g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_FRIENDCALLBACK_AWARD_EXP, CLIENT_CHAT_EXPERIENCE, p->exp > 0 ? p->exp : -p->exp);
			else if(p->award_type == 0){//圈子
				g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_CIRCLE_AWARD_EXP, CLIENT_CHAT_EXPERIENCE, p->exp > 0 ? p->exp : -p->exp);
				CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				pGameUI->CommonMessage("Win_ClassmateManage", "get_exp_re", 0, 0);
			}
			break;
		}
	case PROTOCOL_CIRCLELIST_RE:
		{
			g_pGame->GetCircleMan()->OnCircleList(reinterpret_cast<const CircleList_Re*>(Msg.dwParam1) );
			break;
		}
	case PROTOCOL_GETCIRCLEBASEINFO_RE:
		{
			g_pGame->GetCircleMan()->OnBaseInfo(reinterpret_cast<const GetCircleBaseInfo_Re*>(Msg.dwParam1));
			break;
		}
	case PROTOCOL_CIRCLENOTIFY:
		{
			g_pGame->GetCircleMan()->OnCircleNotify(reinterpret_cast<const CircleNotify*>(Msg.dwParam1));
			break;
		}
	default:
		break;
	}
}

void CECHostPlayer::OnMsgHstFriendCallBckOpt(const ECMSG& Msg){
	using namespace GNET;
	FriendCallbackAward_Re* pAward;
	CECGameUIMan* pGameUI;
	int localTime = 0;
	ACString msg = _AL("");
	switch(Msg.dwParam2) {
	case PROTOCOL_FRIENDCALLBACKINFO_RE:
		if(!m_pFriendMan) return;
		if(m_pFriendMan->StartFriendCallBck((FriendCallbackInfo_Re*) Msg.dwParam1)){
			pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->StartFrdCallBck(((FriendCallbackInfo_Re*) Msg.dwParam1)->callback_limit.mailnum,((FriendCallbackInfo_Re*) Msg.dwParam1)->callback_limit.is_subscribe);
		}
		break;
	case PROTOCOL_FRIENDCALLBACKMAIL_RE:
		pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		switch(((FriendCallbackMail_Re*) Msg.dwParam1) -> retcode){
		case ERR_CALLBACK_SUCCESS:
			localTime = g_pGame->GetServerGMTTime() - g_pGame->GetTimeZoneBias() * 60;
			m_pFriendMan->SetFrdLastSndtime(((FriendCallbackMail_Re*) Msg.dwParam1)->receiver_roleid,localTime);
			pGameUI->OnFrdCallBckRe(((FriendCallbackMail_Re*) Msg.dwParam1) -> retcode);
			break;
		case ERR_CALLBACK_TIMEOUT:
		case ERR_CALLBACK_EXCEPTION:
		case ERR_CALLBACK_LIMITNOTINIT:
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(10525),GP_CHAT_MISC);
			g_pGame->GetGameSession()->GetFriendCallBckInfo();
			break;
		case ERR_CALLBACK_MAXMAILNUM:
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(10526),GP_CHAT_MISC);//fall dowm
		case ERR_CALLBACK_NOTHISFRIEND:
			g_pGame->GetGameSession()->GetFriendCallBckInfo();
			break;
		case ERR_CALLBACK_UNVALIDTEMPID:
		case ERR_CALLBACK_OFFLINE:
		case ERR_CALLBACK_FDLISTNOTINIT:
		case ERR_CALLBACK_MSGCOOLDOWN:
			//it cannot be XQed if client programmed well
			a_LogOutput(1,"CECHostPlayer::OnMsgHstFriendCallBckOpt, FriendCallbackMail_Re retcode id: %d",((FriendCallbackMail_Re*) Msg.dwParam1) -> retcode);
			break;
		case ERR_CALLBACK_FRIENDONLINE:
			pGameUI->AddChatMessage(pGameUI->GetStringFromTable(10523),GP_CHAT_MISC);
			break;
		case ERR_CALLBACK_COOLDOWN:
			msg.Format(pGameUI->GetStringFromTable(10524),CDDAYS_FRIENDCALLBCK);
			pGameUI->AddChatMessage(msg,GP_CHAT_MISC);
			break;
		default:
			break;
		}
		break;
	case PROTOCOL_FRIENDCALLBACKAWARD_RE:
		pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pAward = (FriendCallbackAward_Re*) Msg.dwParam1;
		if(pAward->award_type & CDlgFrdCallBck::SENDER_AWARD){
			if(pAward->returnplayer_name.size()>0)
				pGameUI->PopupFriendCallBckSnderAward(pAward->returnplayer_name.begin(),pAward->returnplayer_name.size());
		}
		if(pAward->award_type & CDlgFrdCallBck::BACKER_AWARD)
			pGameUI->PopupFriendCallBckBckerAward();
		break;
	default:
		break;
	}
}

void CECHostPlayer::OnMsgHst5YearAnni(const ECMSG& Msg){
	using namespace S2C;
	const zx_five_anni_data* pCmd = (const zx_five_anni_data*)(Msg.dwParam1);

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->CommonMessage("Win_AniversarySign5", "info", pCmd->star, pCmd->state);
}

void CECHostPlayer::OnMsgHstSellRoleSuccess(const ECMSG& Msg){
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->GetDialog("Win_ShopConsign_LinkPage")->Show(true);
}

void CECHostPlayer::OnMsgHstUiTransferOpen(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_ui_transfer_opened_notify* pCmd = (const cmd_ui_transfer_opened_notify*)Msg.dwParam1;
	
	m_vecAcitveUiTransId.clear();

	int* pData = (int*)(pCmd + 1);
	int count(pCmd->length);
	for (int i = 0; i < count; ++ i)
	{
		int transid = pData[i];
		m_vecAcitveUiTransId.push_back(transid);	
	}

	// 激活成功后, 通过npc对话方式自动打开传送地图界面
	int idTarget = GetSelectedTarget();
	if (ISNPCID(idTarget))
	{
		if (CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(idTarget))
		{
			if ( ((CECNPCServer*)pNPC)->GetDBEssence()->id_trans_dest_service > 0 )
				g_pGame->GetGameSession()->c2s_CmdNPCSevHello(idTarget);
		}
	}
}

void CECHostPlayer::OnMsgHstNotifyAstrologyEnergy(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_notify_astrology_energy* pCmd = (const cmd_notify_astrology_energy*)Msg.dwParam1;

	// 第一次鉴定星座完成, 激活精力值, 精力值默认要显示, 将该配置设上
	if (!m_bAstrologyEnergyActive && pCmd->active) 
	{
		EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();
		vs.bPlayerEnergy = true;
		g_pGame->GetConfigs()->SetVideoSettings(vs);
	}
	m_bAstrologyEnergyActive = pCmd->active;
	m_nAstrologyEnergy = pCmd->cur_energy;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Equip_Fengshen_Prgs");
	if (pDlg)
	{
		AUIProgress* pProgress = (AUIProgress*)pDlg->GetDlgItem("Prgs_1");
		pProgress->SetProgress(pProgress->GetRangeMax() * m_nAstrologyEnergy / CDlgFengshenConvert::ms_nMaxEnergy);
		ACString szText;
		szText.Format(pGameUI->GetStringFromTable(16029), m_nAstrologyEnergy);
		pProgress->SetHint(szText);

		CECConfigs* pConfigs = g_pGame->GetConfigs();
		if (m_bAstrologyEnergyActive && pConfigs->GetVideoSettings().bPlayerEnergy)
			pDlg->Show(true);
	}
}

void CECHostPlayer::OnMsgHstAstrologyUpgradeResult(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_astrology_upgrade_result* pCmd = (const cmd_astrology_upgrade_result*)Msg.dwParam1;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	if (pCmd->success)
	{
		pGameUI->MessageBox(16021);

		// 刷新显示
		pGameUI->m_pDlgFengshenUpgrade->UpdateProps();

		ACString szText;
		szText.Format(pGameUI->GetStringFromTable(16022), 
			(PAUILABEL*)(pGameUI->m_pDlgFengshenUpgrade->GetDlgItem("Title"))->GetText(), pCmd->new_level);
		pGameUI->AddChatMessage(szText, GP_CHAT_SYSTEM);
	}
	else
	{
		pGameUI->MessageBox(16023);
		ACString szText;
		szText.Format(pGameUI->GetStringFromTable(16024), 
			(PAUILABEL*)(pGameUI->m_pDlgFengshenUpgrade->GetDlgItem("Title"))->GetText());
		pGameUI->AddChatMessage(szText, GP_CHAT_SYSTEM);
	}
}

void CECHostPlayer::OnMsgHstAstrologyDestoryResult(const ECMSG& Msg)
{
	using namespace S2C;
	const cmd_astrology_destory_result* pCmd = (const cmd_astrology_destory_result*)Msg.dwParam1;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString szText;
	szText.Format(pGameUI->GetStringFromTable(16025), pCmd->gain_energy);
	pGameUI->AddChatMessage(szText, GP_CHAT_SYSTEM);
}

void CECHostPlayer::OnMsgHstRemedy_MetempsyChosisLevel(const ECMSG& Msg)
{
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
 	DlgRemedyMetempsychosis* pTempDlg = (DlgRemedyMetempsychosis*)pGameUI->GetDialog("Win_Character_Attrimakeup");
 	if(pTempDlg)
 	{
 		pTempDlg->UpdataRemedyMetempsychosisItem();
		pTempDlg->Show(false);
	}

	CDlgLevelUp* pLevelUpDlg = (CDlgLevelUp*)pGameUI->GetDialog("Win_Character_LvAttriDetail");
	if(pLevelUpDlg)
	{
		(g_pGame->GetGameRun()->GetHostPlayer()->GetRebornInfo())[0].level = 150;
		pLevelUpDlg->SetRebornClass(pTempDlg->GetClass());
		pLevelUpDlg->SetLevel(pTempDlg->GetLevel());
		pLevelUpDlg->SetClass(pTempDlg->GetClass());
		pLevelUpDlg->SetRebornLevel(150);
		pLevelUpDlg->SetRebornType(false);
		pLevelUpDlg->Show(true);
	}
}

void CECHostPlayer::OnMsgLiveness(const ECMSG& Msg)// 2012-10-18 add 活跃度 by zy	
{
	using namespace S2C;
	const liveness_notify* pCmd = (const liveness_notify*)Msg.dwParam1;
	m_nCur_point = pCmd->cur_point;
	m_vecLiveness.clear();

	int count(pCmd->length);
	for (int i = 0; i < count; ++ i)
	{
		int transid = pCmd->achieved_ids[i];	
		m_vecLiveness.push_back(transid);
	}


	bool isTaken = false;
	for(int _i = 0; _i < 4; _i++)
	{
		m_bIsTaken[_i] = pCmd->is_taken[_i];
		if(pCmd->is_taken[_i] == 1)
		{
			isTaken = true;
		}
	}
	if(pCmd->length == 0 && isTaken == false && pCmd->cur_point ==0)
	{
		/// 清空数据 
		m_vecLiveness.clear();
	}


	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	DlgDynamicSystem* pTempDlg = (DlgDynamicSystem*)pGameUI->GetDialog("Win_Activity");
	if(pTempDlg)
	{
		int nPoint = m_nCur_point - max(0, g_pGame->GetOldPoint());
		if(g_pGame->GetOldPoint() >=0 && nPoint > 0) // oldpoint=-1表示当前为登录后第一次同步信息, 无需喊话
		{
			ACString strName;
			strName.Format(pGameUI->GetStringFromTable(17219),nPoint);
			pGameUI->AddChatMessage(strName, GP_CHAT_MISC);
		}
		g_pGame->SetOldPoint(m_nCur_point);
		pTempDlg->UpdataDysnamicItem();
		pTempDlg->UpdataDysnamic();
	}

}


void CECHostPlayer::OnMsgHstAncientRaidOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgAncientLevelInfo* pDlgAncientLevelInfo = (CDlgAncientLevelInfo*)pGameUI->GetDialog("Win_Dungeon_Ling");
	CDlgAncientTime* pDlgAncientTime = (CDlgAncientTime*)pGameUI->GetDialog("Win_Battle_Dungeon_Level_Time");
	if(!pDlgAncientLevelInfo || !pDlgAncientTime)
		return;

	switch(Msg.dwParam2)
	{
		case ANCIENT_RAID_LEVEL_START:
		{
			ancient_raid_level_start* pCmd = (ancient_raid_level_start*)Msg.dwParam1;
			pDlgAncientLevelInfo->SetCurLevel(pCmd->level);
					
			pDlgAncientLevelInfo->SetLevelState(pCmd->level, 1);
			pDlgAncientLevelInfo->SetMatterCnt(pCmd->level, 0); 
			pDlgAncientLevelInfo->SetStartTime(pCmd->level, g_pGame->GetServerGMTTime());
			break;
		}

		case ANCIENT_RAID_LEVEL_END:
		{
			ancient_raid_level_end* pCmd = (ancient_raid_level_end*)Msg.dwParam1;
			pGameUI->GetDialog("Win_Dungeon_Ling_Popup1")->Show(false);
			pGameUI->GetDialog("Win_Dungeon_Ling_Popup1")->Show(true);

			if(pCmd->result)
			{
				pGameUI->GetDialog("Win_Dungeon_Ling_Popup2")->Show(false);
				pGameUI->GetDialog("Win_Dungeon_Ling_Popup2")->Show(true);
				pGameUI->GetDialog("Win_Dungeon_Ling_Popup2")->AlignTo(pGameUI->GetDialog("Win_Dungeon_Ling_Popup1"),
					AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_LEFT,
					AUIDIALOG_ALIGN_OUTSIDE, AUIDIALOG_ALIGN_BOTTOM);

				pDlgAncientLevelInfo->SetResult(pCmd->level, pCmd->result);
			}
			
			pDlgAncientLevelInfo->SetTimerFlag(pCmd->level, false);

			pDlgAncientLevelInfo->SetLevelState(pCmd->level, 2);
			break;
		}
		case ANCIENT_RAID_LEVEL_RESULT:
		{
			ancient_raid_level_result* pCmd = (ancient_raid_level_result*)Msg.dwParam1;
			pDlgAncientLevelInfo->SetLevelInfo(pCmd);
				
			break;
		}

		case ANCIENT_RAID_LEVEL_INFO:
		{
			ancient_raid_level_info* pCmd = (ancient_raid_level_info*)Msg.dwParam1;
			pDlgAncientLevelInfo->SetMatterCnt(pCmd->level, pCmd->matter_cnt);
			pDlgAncientLevelInfo->SetStartTime(pCmd->level, pCmd->start_time);
			pDlgAncientLevelInfo->SetCurLevel(pCmd->level);
			pDlgAncientLevelInfo->SetState(pCmd->level);
			break;
		}
	}
}

void CECHostPlayer::OnMsgSalePromotionInfo(const ECMSG& Msg)
{
	using namespace S2C;
	sale_promotion_info* pCmd = (sale_promotion_info*)Msg.dwParam1;
	
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)pGameUI->GetDialog("Win_CalendarPresent");
	if (pDlg)
		pDlg->UpdateConditionInfo(pCmd);
}

void CECHostPlayer::OnMsgHstEventOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();	
	switch(Msg.dwParam2)
	{
		case EVENT_START:
		{
			event_start* pCmd = (event_start*)Msg.dwParam1;
			pGameUI->m_pDlgMidMap->SetEvent(pCmd->event_id - 1, true);	
			break;
		}
		case EVENT_STOP:
		{
			event_stop* pCmd = (event_stop*)Msg.dwParam1;
			pGameUI->m_pDlgMidMap->SetEvent(pCmd->event_id - 1, false);
			break;
		}
		case EVENT_INFO_ONLINE:
		{
			event_info* pCmd = (event_info*)Msg.dwParam1;
			for(int i = 0; i < pCmd->event_count; i++)
				pGameUI->m_pDlgMidMap->SetEvent(pCmd->event[i] - 1, true);
			break;
		}
	}
}

// 玩家基础属性增益
void CECHostPlayer::OnMsgHstPropAddResult(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
		case PLAYER_PROPADD:
		{
			player_propadd* pCmd = (player_propadd*)Msg.dwParam1;
			memcpy(m_propAdd, pCmd->add_prop, sizeof(int)*30);
			CDlgAttriUpNature* pDlg = (CDlgAttriUpNature*)pGameUI->GetDialog("Win_AttriUp_Nature");
			if (pDlg->IsShow())
				pDlg->UpdatePropAddProgress();
			break;
		}
		case PLAYER_GET_PROPADD_ITEM:
		{
			player_get_propadd_item* pCmd = (player_get_propadd_item*)Msg.dwParam1;
			CDlgAttriUp* pDlg = (CDlgAttriUp*)pGameUI->GetDialog("Win_AttriUp");
			if (pDlg->IsShow())
				pDlg->OnPropAddItemResult(pCmd->item_id, pCmd->item_index);
			break;
		}
	default:
		break;
	}	
}

void CECHostPlayer::OnMsgHstFuWenOpt(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case FUWEN_COMPOSE_INFO:
		{
			fuwen_compose_info* pCmd = (fuwen_compose_info*)Msg.dwParam1;
			if (CDlgRoleRuneCombine* pDlg = (CDlgRoleRuneCombine*)pGameUI->GetDialog("Win_Equip_Rune_Combine"))
				pDlg->UpdateCombineTimes(pCmd->compose_count, pCmd->extra_compose_count);
			break;
		}
	case FUWEN_COMPOSE_END:
		{
			if (CDlgRoleRuneCombine* pDlg = (CDlgRoleRuneCombine*)pGameUI->GetDialog("Win_Equip_Rune_Combine"))
			{
				if (pDlg->IsShow())
					pDlg->ProduceOne();
			}
			break;
		}
	case FUWEN_INSTALL_RESULT:
		{
			fuwen_install_result * pCmd = (fuwen_install_result*)Msg.dwParam1;
			if (CECIvtrItem* pItem = m_pPack->GetItem(pCmd->src_index, true))
				m_pRune2013Pack->SetItem(pCmd->dst_index, pItem);

			if (CDlgRoleRune* pDlg = (CDlgRoleRune*)pGameUI->GetDialog("Win_Equip_Rune"))
				pDlg->UpdateView();
			break;
		}
	case FUWEN_UNINSTALL_RESULT:
		{
			fuwen_uninstall_result * pCmd = (fuwen_uninstall_result*)Msg.dwParam1;
			if (CECIvtrItem* pItem = m_pRune2013Pack->GetItem(pCmd->fuwen_index, true))
				m_pPack->SetItem(pCmd->inv_index, pItem);

			if (CDlgRoleRune* pDlg = (CDlgRoleRune*)pGameUI->GetDialog("Win_Equip_Rune"))
				pDlg->UpdateView();
			break;
		}
	default:
		break;
	}
}

void CECHostPlayer::OnMsgHstFlowBattlePlayerInfo(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	cs_flow_player_info* pCmd = (cs_flow_player_info*)Msg.dwParam1;
	if(pCmd->player_id == this->GetCharacterID())
	{
		CECPlayer::OnMsgHstFlowBattlePlayerInfo(Msg);
		// 更新自己的积分版
		DlgBattleFlowInfoMax* pDlg = (DlgBattleFlowInfoMax*)pGameUI->GetDialog("Win_Battle_SeniorPK_Score");
		DlgBattleFlowInfoMin* pDlgMin = (DlgBattleFlowInfoMin*)pGameUI->GetDialog("Win_Battle_SeniorPK_ScoreMin");
		
		pDlg->UpDateFlowInfo(pCmd->kill_cnt,pCmd->death_cnt,m_nScore,pCmd->max_ckill_cnt,pCmd->c_kill_cnt,pCmd->max_mkill_cnt);
		pDlgMin->UpDateFlowInfo(m_nScore,pCmd->kill_cnt,pCmd->death_cnt);
		
		if(pCmd->m_kill_change == true && pCmd->m_kill_cnt >= 5)
		{
			ACString strMsg;
			strMsg.Format(pGameUI->GetStringFromTable(18001), g_pGame->GetGameRun()->GetPlayerName(pCmd->player_id, true),pCmd->m_kill_cnt);
			
			pGameUI->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
			
			PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Popmsg");
			PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Message");
			
			pLabel->SetText(strMsg);
			pLabel->SetMarquee(true, 3, 40);
			if( !pDlg->IsShow() ) pDlg->Show(true, false, false);
		}
	}
	else
	{
		CECPlayerMan* pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
		CECPlayer *pPlayer = pPlayerMan->GetElsePlayer(pCmd->player_id);

		if( pPlayer )
		{
			pPlayer->ProcessMessage(Msg);
			
			if(pCmd->m_kill_change == true && (pCmd->m_kill_cnt / 10 != 0 && pCmd->m_kill_cnt % 10 == 0))
			{
				ACString strMsg;
				strMsg.Format(pGameUI->GetStringFromTable(18001), g_pGame->GetGameRun()->GetPlayerName(pCmd->player_id, true),pCmd->m_kill_cnt);
				
				pGameUI->AddChatMessage(strMsg, GP_CHAT_SYSTEM);
				
				PAUIDIALOG pDlg = pGameUI->GetDialog("Win_Popmsg");
				PAUILABEL pLabel = (PAUILABEL)pDlg->GetDlgItem("Message");
				
				pLabel->SetText(strMsg);
				pLabel->SetMarquee(true, 3, 40);
				if( !pDlg->IsShow() ) pDlg->Show(true, false, false);
			}
		}
	}	

}

void CECHostPlayer::OnMsgHstExchangeItemList(const ECMSG& Msg)
{
	using namespace S2C;

	CECIvtrItem* pItem1;
	CECIvtrItem* pItem2;
	exchange_item_list_info* pCmd = (exchange_item_list_info*)Msg.dwParam1;
	
	if(!pCmd)
		return;
	if(pCmd->content_length % 2 != 0)
		return;

	if(Msg.dwParam2 ==  ERROR_MESSAGE)
	{
		m_pPack->UnfreezeAllItems();
		return;
	}

	if(pCmd->type == 0)
	{
		for(size_t i = 0; i < pCmd->content_length; i += 2)
		{
			m_pPack->ExchangeItem(pCmd->content[i], pCmd->content[i + 1]);
			pItem1 = m_pPack->GetItem(pCmd->content[i]);
			pItem2 = m_pPack->GetItem(pCmd->content[i + 1]);
			if (pItem1 && pItem2)
				UpdateExchangedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->content[i + 1], pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->content[i]);
			else if (pItem1)
				UpdateMovedItemSC(pItem1->GetTemplateID(), IVTRTYPE_PACK, pCmd->content[i + 1], IVTRTYPE_PACK, pCmd->content[i]);
			else if (pItem2)
				UpdateMovedItemSC(pItem2->GetTemplateID(), IVTRTYPE_PACK, pCmd->content[i], IVTRTYPE_PACK, pCmd->content[i + 1]);
		}
		m_pPack->UnfreezeAllItems();
	}
	else
	{
		for(size_t i = 0; i < pCmd->content_length; i += 2)
		{
			m_pTrashBoxPack->ExchangeItem(pCmd->content[i], pCmd->content[i + 1]);
		}
	}
}

void CECHostPlayer::OnMsgHstWebOrderInfo(const ECMSG& Msg)
{
	using namespace S2C;

	web_order_list* pCmd = (web_order_list*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgWebShopOrder* pDlgWebShopOrder = (CDlgWebShopOrder*)pGameUI->GetDialog("Win_Qshop_Web");	
	
	if(!pDlgWebShopOrder)
		return;

	pDlgWebShopOrder->ClearOrderInfo();

	BYTE* pData = (BYTE*)pCmd + sizeof(int);

	for(int i = 0; i < pCmd->web_order_count; i++)
	{
		order_info order;
		ACString strText;
		memset(&order, 0, sizeof(order_info));

		web_order_info* pData_Order_Info = (web_order_info*)pData;
		order.order_id = pData_Order_Info->order_id;
		order.pay_type = pData_Order_Info->pay_type;
		order.status = pData_Order_Info->status;
		order.timestamp = pData_Order_Info->timestamp;
		order.package_count = pData_Order_Info->package_count;
		pData += sizeof(__int64) + 4 * sizeof(int);
		
		for(int j = 0; j < pData_Order_Info->package_count; j++)
		{
			web_order_package* pData_Order_Package = (web_order_package*)pData;
			order.package[j].package_id = pData_Order_Package->package_id;
			ACString strPackageName((const ACHAR*)pData_Order_Package->package_name, pData_Order_Package->name_len / sizeof (ACHAR));
			if(strPackageName.IsEmpty())
				strText = pGameUI->GetStringFromTable(19920);
			else
				strText = strPackageName;
			pData += sizeof(int) + sizeof(size_t) + pData_Order_Package->name_len * sizeof(char);
			order.package[j].count = *(int*)pData;
			pData += sizeof(int);
			order.package[j].price = *(int*)pData;
			pData += sizeof(int);
			order.package[j].good_count = *(int*)pData;
			pData += sizeof(int);
			web_order_goods* pData_Order_Goods = (web_order_goods*)pData;
		
			for(int k = 0; k < order.package[j].good_count; k++)
			{
				order.package[j].goods[k].goods_id = pData_Order_Goods[k].goods_id;
				order.package[j].goods[k].goods_count = pData_Order_Goods[k].goods_count;
				order.package[j].goods[k].goods_flag = pData_Order_Goods[k].goods_flag;
				order.package[j].goods[k].goods_time = pData_Order_Goods[k].goods_time;
			}
			
			pData += order.package[j].good_count * sizeof(web_order_goods);
		}
		pDlgWebShopOrder->AddOrderInfo(order, strText);
	}
}


void CECHostPlayer::OnMsgHstOffLineInfo(const ECMSG& Msg)
{
	using namespace S2C;
	battle_off_line_info info;
	
	if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
		return;
	
	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgBattleInfo* pBattleInfo = (CDlgBattleInfo*)pGameUI->GetDialog("Win_PkScore2013");	
	if(pBattleInfo)
	{
		pBattleInfo->UpDateBattleOffLineInfo(&info);
	}
	
}

void CECHostPlayer::OnMsgHstControlTrapInfo(const ECMSG& Msg)
{
	using namespace S2C;

	control_trap_info* pCmd = (control_trap_info*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgSkillRemote* pDlgSkillRemote = (CDlgSkillRemote*)pGameUI->GetDialog("Win_Prof13_Skill_Remote");
	if(!pDlgSkillRemote)
		return;
	
	pDlgSkillRemote->SetTrapInfo(pCmd->id, pCmd->tid, pCmd->time);
	pDlgSkillRemote->Show(true);
}

void CECHostPlayer::OnMsgHstNpcInvisible(const ECMSG& Msg)
{
	using namespace S2C;

	npc_invisible* pCmd = (npc_invisible*)Msg.dwParam1;
	if(!pCmd)
		return;
	
	CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(pCmd->id);
	if(pNPC)
		pNPC->SetInvisible(pCmd->on);
}


void CECHostPlayer::OnMsgHstPhaseInfo(const ECMSG& Msg)
{
	using namespace S2C;
	
	switch(Msg.dwParam2)
	{
	case PHASE_INFO:
		{
			phase_info* pCmd = (phase_info*)Msg.dwParam1;
			for(int i = 0; i < pCmd->phase_count; i++)
				if(pCmd->phase_list[i].phase_id > 0 && pCmd->phase_list[i].phase_id <= PHASE_NUM)
					if(pCmd->phase_list[i].on)
						m_aPhase[pCmd->phase_list[i].phase_id - 1] = 1;
					else 
						m_aPhase[pCmd->phase_list[i].phase_id - 1] = 0;
			break;
		}
	case PHASE_INFO_CHANGE:
		{
			phase_info_change* pCmd = (phase_info_change*)Msg.dwParam1;
			if(pCmd->phase_id >0 && pCmd->phase_id <= PHASE_NUM)
				if(pCmd->on)
					m_aPhase[pCmd->phase_id - 1] = 1;
				else
					m_aPhase[pCmd->phase_id - 1] = 0;
		}
		break;
	}
}

void CECHostPlayer::OnMsgHstBeTaunted2(const ECMSG & Msg)
{
	using namespace S2C;

	be_taunted2* pCmd = (be_taunted2*)Msg.dwParam1;
	if(!pCmd)
		return;

	m_pWorkMan->FinishAllWork(true);
	
	m_BetaunTed = true;
	m_BetauntedCnt.SetPeriod(pCmd->time * 1000);
	m_BetauntedCnt.Reset(false);
	m_iTaunterId = pCmd->idPlayer;
	m_idUCSelTarget = pCmd->idPlayer;
	m_idSelTarget = pCmd->idPlayer;

	g_pGame->GetGameSession()->c2s_CmdSelectTarget(pCmd->idPlayer);
	CmdNormalAttack();
}


void CECHostPlayer::OnMsgFashionColor(const ECMSG& Msg)
{
	using namespace S2C;

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case FASHION_COLORANT_PRODUCE:
		{
				fashion_colorant_produce* pCmd = (fashion_colorant_produce*)Msg.dwParam1;
				DlgIndividualityColor* pDlgIndividualityColor = (DlgIndividualityColor*)pGameUI->GetDialog("Win_Dye_Combine");
				if(pDlgIndividualityColor)
				{
					pDlgIndividualityColor->UpDateResult(pCmd->nIndex);
				}
		}
		break;
	case FASHION_COLORANT_USE:
		{
				fashion_colorant_use* pCmd = (fashion_colorant_use*)Msg.dwParam1;
				DlgIndividualityReveal* pDlgIndividualityReveal = (DlgIndividualityReveal*)pGameUI->GetDialog("Win_Dye_FashDress");
				if(pDlgIndividualityReveal)
				{
					pDlgIndividualityReveal->UpdateFreezeItem();
				}
		}
		break;
	default:
		break;
	}
}

void CECHostPlayer::OnMsgFacBuild(const ECMSG& Msg)			//帮派建设
{
	using namespace S2C;
	CECOrnamentMan* pOnmtMan = g_pGame->GetGameRun()->GetWorld()->GetOrnamentMan();
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case FAC_BUILDING_ADD:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			fac_building_add* pCmd = (fac_building_add*)Msg.dwParam1;
			DlgGuildLandBuild* pDlgLandBuild = (DlgGuildLandBuild*)pGameUI->GetDialog("Win_Guild_LandBuild");
			AddBuildInfo(pCmd->field_index,pCmd->building_tid);
			pDlgLandBuild->ResultBuilding(pCmd->building_tid,pCmd->field_index - 1);
			if(pBuildInfo)
			{
				pBuildInfo->UpDateList();
			}
			pOnmtMan->AddLandBuild(pCmd->field_index,pCmd->building_tid,0,0);
		}
		break;
	case FAC_BUILDING_UPGRADE_FAC:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			fac_building_upgrade* pCmd = (fac_building_upgrade*)Msg.dwParam1;
			DlgGuildLandBuild* pDlgLandBuild = (DlgGuildLandBuild*)pGameUI->GetDialog("Win_Guild_LandBuild");
			UpgradeBuildInfo(pCmd->field_index,pCmd->building_tid,pCmd->new_level);
			pDlgLandBuild->ResultBuildUpgrade(pCmd->building_tid,pCmd->field_index - 1,pCmd->new_level);
			if(pBuildInfo)
			{
				pBuildInfo->UpDateList();
			}
			pOnmtMan->ChangeLandBuild(pCmd->field_index,pCmd->building_tid,pCmd->new_level,0);
		}
		break;
	case FAC_BUILDING_REMOVE:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			fac_building_remove* pCmd = (fac_building_remove*)Msg.dwParam1;
			DlgGuildLandBuild* pDlgLandBuild = (DlgGuildLandBuild*)pGameUI->GetDialog("Win_Guild_LandBuild");
			RemoveBuildInfo(pCmd->field_index);
			pDlgLandBuild->ResultBuildRemove(pCmd->field_index);
			if(pBuildInfo)
			{
				pBuildInfo->UpDateList();
			}
			pOnmtMan->DelLandBuild(pCmd->field_index);
		}
		break;
	case FAC_BUILDING_COMPLETE:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			fac_building_complete* pCmd = (fac_building_complete*)Msg.dwParam1;
			DlgGuildLandBuild* pDlgLandBuild = (DlgGuildLandBuild*)pGameUI->GetDialog("Win_Guild_LandBuild");
			ChangeBuildInfo(pCmd->field_index,pCmd->building_tid,pCmd->new_level);
			pDlgLandBuild->ResultBuildComplete(pCmd->building_tid,pCmd->field_index - 1,pCmd->new_level - 1);
			if(pBuildInfo)
			{
				pBuildInfo->UpDateList();
			}
			pOnmtMan->ChangeLandBuild(pCmd->field_index,pCmd->building_tid,pCmd->new_level - 1,1);
		}
		break;
	case PLAYER_FAC_BASE_INFO:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			player_fac_base_info info;
			if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			SetFactionGrass(info.pValue.grass);
			SetFactionMine(info.pValue.mine);
			SetFactionMonsterFood(info.pValue.monster_food);
			SetFactionMonsterCore(info.pValue.monster_core);
			SetFactionMoney(info.pValue.money);
			SetFactionTask_id(info.pValue.task_id);
			SetFactionTask_count(info.pValue.task_count);
			SetFactionTask_need(info.pValue.task_need);
			ACString msg =  ACString((ACHAR*)info.msg, info.msg_size/sizeof(ACHAR));
			pBuildInfo->UpDataEdit(msg);
			SetMapBuildInfo(&info);
			if(pBuildInfo)
			{
				pBuildInfo->UpDateList();
			}
		}
		break;
	case FAC_BASE_PROP_CHANGE:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			if(pBuildInfo)
			{
				S2C::fac_base_prop_change* pCmd = (S2C::fac_base_prop_change*)Msg.dwParam1;
				if(GetCharacterID() == pCmd->noti_foleid)
				{
					ACString szTemp;
					if(GetFactionGrass()  != pCmd->pValue.grass)
					{
						if(GetFactionGrass() > pCmd->pValue.grass)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20225),GetFactionGrass() - pCmd->pValue.grass);
						}
						else if(GetFactionGrass() < pCmd->pValue.grass)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20226),pCmd->pValue.grass - GetFactionGrass());
						}
						pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
					}
					if(GetFactionMine() != pCmd->pValue.mine)
					{
						if(GetFactionMine() > pCmd->pValue.mine)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20227),GetFactionMine() - pCmd->pValue.mine);
						}
						else if(GetFactionMine() < pCmd->pValue.mine)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20228),pCmd->pValue.mine - GetFactionMine());
						}
						pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
					}

					if(GetFactionMonsterFood() != pCmd->pValue.monster_food)
					{
						if(GetFactionMonsterFood() > pCmd->pValue.monster_food)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20229),GetFactionMonsterFood() - pCmd->pValue.monster_food);
						}
						else if(GetFactionMonsterFood() < pCmd->pValue.monster_food)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20230),pCmd->pValue.monster_food - GetFactionMonsterFood());
						}
						pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
					}
					if(GetFactionMonsterCore() != pCmd->pValue.monster_core)
					{
						if(GetFactionMonsterCore() > pCmd->pValue.monster_core)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20231),GetFactionMonsterCore() - pCmd->pValue.monster_core);
						}
						else if(GetFactionMonsterCore() < pCmd->pValue.monster_core)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20232),pCmd->pValue.monster_core - GetFactionMonsterCore());
						}
						pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
					}
					if(GetFactionMoney() != pCmd->pValue.money)
					{
						if(GetFactionMoney() > pCmd->pValue.money)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20233),GetFactionMoney() - pCmd->pValue.money);
						}
						else if(GetFactionMoney() < pCmd->pValue.money)
						{
							szTemp.Format(pGameUI->GetStringFromTable(20234),pCmd->pValue.money - GetFactionMoney());
						}
						pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
					}
				}
				SetFactionGrass(pCmd->pValue.grass);
				SetFactionMine(pCmd->pValue.mine);
				SetFactionMonsterFood(pCmd->pValue.monster_food);
				SetFactionMonsterCore(pCmd->pValue.monster_core);
				SetFactionMoney(pCmd->pValue.money);
				SetFactionTask_id(pCmd->pValue.task_id);
				SetFactionTask_count(pCmd->pValue.task_count);
				SetFactionTask_need(pCmd->pValue.task_need);
				pBuildInfo->UpDateFacBaseInfo();
			}
		}
		break;
	case GET_FACBASE_MALL_INFO:
		{
				facbase_mall_item_info* pCmd = (facbase_mall_item_info*)Msg.dwParam1;
				DlgGuildShop* pGuildQShop = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
				pGuildQShop->UpDataLockSwapItem((void*)Msg.dwParam1);
		}
		break;
	case GET_FACTION_COUPON:
		{
				get_faction_coupon* pCmd = (get_faction_coupon*)Msg.dwParam1;
				CDlgGuildMain* pBuildMain = (CDlgGuildMain*)pGameUI->GetDialog("Win_GuildMain");
				if(GetFactionGoldNote() != -1)
				{
					ACString szTemp;
					if(GetFactionGoldNote() > pCmd->faction_coupon)
					{
						szTemp.Format(pGameUI->GetStringFromTable(20199),GetFactionGoldNote() - pCmd->faction_coupon);
					}
					else if(GetFactionGoldNote() < pCmd->faction_coupon)
					{
						szTemp.Format(pGameUI->GetStringFromTable(20200),pCmd->faction_coupon - GetFactionGoldNote());
					}
					pGameUI->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
				}
				if(pBuildMain)
				{
					pBuildMain->UpDataFactionCoupon(pCmd->faction_coupon);
				}
				SetFactionGoldNote(pCmd->faction_coupon);
		}
		break;
	case GET_FACBASE_MONEY_ITEMS_INFO:
		{
			DlgGuildQShop* pGuildQShop = (DlgGuildQShop*)pGameUI->GetDialog("Win_Guild_Qshop");
			pGuildQShop->UpDataBuyItemInfo((void*)Msg.dwParam1);
		}
		break;
	case PLAYER_FACBASE_AUCTION:
		{
			player_facbase_auction info;
			if (!info.FillData((const void*)Msg.dwParam1, Msg.dwParam3))
				return;
			DlgGuildShop* pGuildShop = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
			pGuildShop->UpDateSaleItem(&info);
		}
		break;
	case FACBASE_AUCTION_UPDATE:
		{
				DlgGuildShop* pGuildShop = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
				pGuildShop->UpDateSaleElement((void*)Msg.dwParam1);
		}
		break;
	case FAC_COUPON_RETURN:
		{
			fac_coupon_return* pCmd = (fac_coupon_return*)Msg.dwParam1;
			DlgGuildShop* pGuildShop = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
			pGuildShop->UpDateReturnCoupon(pCmd->coupon_retrun);
		}
		break;
	case GET_FACBASE_CASH_ITEM_SUCCESS:
		{
			get_facbase_cash_item_success* pCmd = (get_facbase_cash_item_success*)Msg.dwParam1;
			DlgGuildQShop* pGuildQShop = (DlgGuildQShop*)pGameUI->GetDialog("Win_Guild_Qshop");
			pGuildQShop->UpDateBuyItem(pCmd->item_index);
		}
		break;
	case FACBASE_AUCTION_ADD_HISTORY:
		{
			facbase_auction_add_history* pCmd = (facbase_auction_add_history*)Msg.dwParam1;
			DlgGuildShop* pGuildShop = (DlgGuildShop*)pGameUI->GetDialog("Win_Guild_Shop");
			pGuildShop->AddHistory((void*)&pCmd->entry);
		}
		break;
	case FACBASE_MSG_UPDATE:
		{
			CDlgGuildBuild* pBuildInfo = (CDlgGuildBuild*)pGameUI->GetDialog("Win_Guild_Build");
			pBuildInfo->UpDataMsg((void*)Msg.dwParam1);
		}
		break;
	default:
		break;
	}
}

void CECHostPlayer::OnMsgHstInviteActiveEmote(const ECMSG & Msg)
{
	using namespace S2C;

	invite_active_emote* pCmd = (invite_active_emote*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strMsg;
	strMsg.Format(pGameUI->GetStringFromTable(20193), g_pGame->GetGameRun()->GetPlayerName(pCmd->playerid, true));
	pGameUI->AddInformation(CDlgInfo::INFO_SYSTEM, "Game_InviteActiveEmote", 
		strMsg, 30000, pCmd->playerid, pCmd->type, 0);
}

void CECHostPlayer::OnMsgHstReplyActiveEmote(const ECMSG & Msg)
{
	using namespace S2C;

	reply_active_emote* pCmd = (reply_active_emote*)Msg.dwParam1;
	if(!pCmd)
		return;

	CECGameUIMan* uiman = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	ACString strMsg;
	strMsg.Format(uiman->GetStringFromTable(20194), g_pGame->GetGameRun()->GetPlayerName(pCmd->playerid, true));
	if (!pCmd->answer)
		uiman->AddChatMessage(strMsg, GP_CHAT_SYSTEM);;
}

void CECHostPlayer::OnMsgPlayerPlayActiveEmote(const ECMSG & Msg)
{
	using namespace S2C;

	play_active_emote* pCmd = (play_active_emote*)Msg.dwParam1;
	if(!pCmd)
		return;

	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND && 
		m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_BEBOUND)
		return;


	m_CandEmoteInfo.id = pCmd->playerid2;
	m_CandEmoteInfo.type = pCmd->type;
}

void CECHostPlayer::OnMsgPlayerStopActiveEmote(const ECMSG & Msg)
{
	using namespace S2C;

	stop_active_emote* pCmd = (stop_active_emote*)Msg.dwParam1;
	if(!pCmd)
		return;

	if (pCmd->playerid2 != m_iBuddyId || pCmd->type != m_iEmoteType)
		return;

	StopActiveEmote();
}

void CECHostPlayer::OnMsgBePlayerChargePre(const ECMSG& Msg)
{
	using namespace S2C;

	object_try_charge* pCmd = (object_try_charge*)Msg.dwParam1;
	if(!pCmd)
		return ;
	
	A3DVECTOR3 vCanGoPos = m_MoveCtrl.TryToGo(pCmd->destpos);

	g_pGame->GetGameSession()->c2s_SendCmdChargeTo(pCmd->type, pCmd->target_id, vCanGoPos);
}

void CECHostPlayer::OnMsgTravelFree(const ECMSG& Msg)
{	
	using namespace S2C;
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();

	switch(Msg.dwParam2)
	{
	case PLAYER_START_TRAVEL_AROUND:
		{
			player_start_travel_around* pCmd = (player_start_travel_around*)Msg.dwParam1;
			
			CECIvtrTalismanMain *pEquip = (CECIvtrTalismanMain*)CECIvtrItem::CreateItem(pCmd->travel_id, 0, 1);
			if(!pEquip)
				return;
			m_pItemWingOrg = m_pEquipPack->PutItem(EQUIPIVTR_WING, pEquip);
			UpdateEquipSkins(true);
			ShowWing(false);
			
			m_dwStates |= GP_STATE_FLY;
			CECHPWorkBezierMove* pWorkMove = (CECHPWorkBezierMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVEBEZIEL);
			pWorkMove->SetBezierIdAndSpeed(pCmd->travel_path_id, pCmd->travel_speed);
			m_pWorkMan->StartWork_p2(pWorkMove);

	
			CECHPWorkFly* pWork = (CECHPWorkFly*)m_pWorkMan->CreateWork(CECHPWork::WORK_FLYOFF);
			pWork->m_bSkillFly = false;
			m_pWorkMan->StartWork_p3(pWork);
			
		
/*			TALISMAN_MAINPART_ESSENCE* pTalisman = (TALISMAN_MAINPART_ESSENCE*)pEquip->GetDBEssence();

			if (!pTalisman->is_aircraft)
				return;
				
			//CECPlayer* pPlayer = NULL;
			const char* szModelFile = "";
			//if(g_pGame->GetGameRun() && g_pGame->GetGameRun()->GetWorld() && g_pGame->GetGameRun()->GetWorld()->GetPlayerMan())
			//	pPlayer = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan()->GetPlayer(iRoleID);

			//四足体型使用非翅膀类型的飞剑时，使用默认的翅膀模型
			//if(iProfession >= 108 && iProfession <= 112 && pPlayer && pPlayer->GetShapeChangedId() != SHAPE_HUMAN && pTalisman->fly_mode != FLYMODE_WING)
			//{
			//	pResult->nFlyMode = FLYMODE_WING;
			//	szModelFile = "models\\players\\通用装备\\翅膀\\末日翅膀\\末日翅膀.ecm";
			//}
			//else
			//{
			szModelFile = g_pGame->GetDataPath(pTalisman->file_model[0]);
			AString strNewModelFile;
			m_iTravalVehicleId = pCmd->travel_id;
			EquipsLoadResult loadResult;
			memset(&loadResult, 0, sizeof(EquipsLoadResult));
			loadResult.nFlyMode = pTalisman->fly_mode;
			ChangeWing(&loadResult, szModelFile, m_BasicProps.iProfession, m_iChangeShape);

			SetEquipsLoadedResult(loadResult, true);
			ShowWing(false);
			delete pEquip;
*/			m_iFreeFlyState = 1;
		}
		break;
	case PLAYER_STOP_TRAVEL_AROUND:
		{
			m_pWorkMan->FinishCurWork(CECHPWork::WORK_MOVEBEZIEL, true);
			StartLanding();
			m_iFreeFlyState = 0; 
			CECIvtrItem* pItem = m_pEquipPack->PutItem(EQUIPIVTR_WING, m_pItemWingOrg);
			m_pItemWingOrg = 0;
			UpdateEquipSkins(false);
			if(pItem)
				delete pItem;
		}
		break;
	}

}

void CECHostPlayer::StartLanding()
{
	m_dwStates &= ~GP_STATE_FLY;

	if (IsDead() || m_bCandHangerOn || IsHangerOn())
		ShowWing(false);
	else
	{
		if (IsGathering())
		{
			ECMSG msg;
			OnMsgHstPressCancel(msg);
		}
		CECHPWorkFall* pWork = (CECHPWorkFall*)m_pWorkMan->CreateWork(CECHPWork::WORK_FREEFALL);
		pWork->SetFallType(CECHPWorkFall::TYPE_FLYFALL);
		m_pWorkMan->StartWork_p1(pWork);
	}

	//	Below two lines will fix the "host stand in air" bug.
	m_iMoveEnv = CECPlayer::MOVEENV_GROUND;
	//	m_pCDRInfo->vTPNormal.Clear();
	ResetCDRInfo();
}
