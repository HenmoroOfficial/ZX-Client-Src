/*
 * FILE: EC_GameSession.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AString.h"
#include "AArray.h"
#include "AAssist.h"
#include "hashmap.h"
//#include "EC_C2SCmdCache.h"
#include "EC_StringTab.h"
/*#include "EC_SendC2SCmds.h"
#include "EC_FGC2SRawCmds.h"
#include "EC_HTC2SRawCmds.h"
#include "gnetdef.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A3DVECTOR3;
class AFile;
class CECNetProtocolBase;
class CECPrtcPBIProc;
class CECFunGamePrtcHandler;
class CECHomeTownPrtcHandler;
*/

class AXMLItem;


namespace GNET
{
//	class RoleInfo;
	class Protocol;
	class GameClient;
	class Octets;
	class GConsignRole;
	class GSNSRole;
}
/*
namespace C2S
{
	struct npc_trade_item;
	struct item_trade;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

typedef void (* PONRECEIVEHELPSTATES) (const void * pData, int iSize);

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECGameSession
//	
///////////////////////////////////////////////////////////////////////////
*/
class CECGameSession
{
public:		//	Types

	//	Overtime check ID
	enum
	{
		OT_CHALLENGE = 0,
		OT_ENTERGAME,
	};

	//	Overtime check
	struct OVERTIME
	{
		int		iCheckID;
		bool	bCheck;
		DWORD	dwTime;
		DWORD	dwTimeCnt;
	};		

	//	Pingtime
	struct PINGTIME
	{
		DWORD	idLink;
		DWORD	dwTime;
		DWORD	dwStatus;
		DWORD	dwLastPingTime;
	};		

	static int IOCallBack(void* pData, unsigned int idLink, int iEvent);
	static DWORD WINAPI ConnectThread(LPVOID pArg);

public:		//	Constructor and Destructor

	CECGameSession();
	virtual ~CECGameSession();

public:		//	Attributes

public:		//	Operations

	//	Open session
	bool Open();
	//	Close sesssion
	void Close();

	//	Update
/*	bool Update(DWORD dwDeltaTime);
	//	Net pulse
	void NetPulse();

*/	//	Set link ID
	void SetLinkID(DWORD idLink) { m_idLink = idLink; }
	//	Get link ID
	DWORD GetLinkID() { return m_idLink; }

	//	Add a protocol to m_aNewProtocols and prepare to be processed
	int AddNewProtocol(GNET::Protocol* pProtocol);
	//	Process protocols in m_aNewProtocols
	bool ProcessNewProtocols();

	//Send NewKeepAlive protocol to the server.
	void SendNewKeepAlivePrtc( void );
	void SendAnnounceAid( void );
	//	Filter protocols
//	bool FilterProtocols(GNET::Protocol* pProtocol);
	//	Process one protocol
//	bool ProcessOneProtocol(CECNetProtocolBase* p);
	//	Clear all processed protocols in m_aOldProtocols
/*	void ClearOldProtocols();
	//	Send game data
	bool SendGameData(void* pData, int iSize);
	//  Send fun game data
	bool SendFunGameData(void* pData, int iSize);
	//  Send home town data
	bool SendHomeTownData(void* pData, int iSize);
	//	Send chat data
	bool SendChatData(char cChannel, const ACHAR* szMsg, int nPack = -1, int nSlot = 0);
	//	Send private chat data
	bool SendPrivateChatData(const ACHAR* szDstName, const ACHAR* szMsg, BYTE byFlag=0, int idPlayer=0, int nPack = -1, int nSlot = 0);
*/	// Send protocl
	bool SendNetData(const GNET::Protocol& p, bool bUrg = false);

/*	//	Create a role
	bool CreateRole(const GNET::RoleInfo& Info, const ACHAR* szReferID);
	//	Select role
	bool SelectRole(int cid, int line_id);
	//	Delete role
	bool DeleteRole(int cid);
	//	Restore role
	bool RestoreRole(int cid);
	//	Start game
	bool StartGame();
	//	Re-login
	bool ReLogin(bool bSelRole);

	//	Get player's bese info (including custom data)
	void GetRoleBaseInfo(int iNumRole, const int* aRoleIDs);
	//  Get clone's base info by player's id 
	void GetRoleBaseInfo2(int iPlayerId, int iCloneId);
	//	Get player's customized data
	void GetRoleCustomizeData(int iNumRole, const int* aRoleIDs);
	//	Get player's faction info
	void GetFactionInfo(int iNumFaction, const int* aFactinoIDs, char scale);
	//	Set custom data
	void SetRoleCustomizeData(int idRole, void* pData, int iDataSize);
	//	Save necessary user configs (UI, shortcut, accelerate keys) to server
	void SaveConfigData(const void* pDataBuf, int iSize);
	//	Load necessary user configs (UI, shortcut, accelerate keys) from server
	void LoadConfigData();
	//	Save help states data to server
	void SaveHelpStates(const void* pDataBuf, int iSize);
	//	Load help states data from server, and when help states received, we will call callback function to
	//	let the caller get the data
	void LoadHelpStates();
*/	//	Get player's brief information
	void GetPlayerBriefInfo(int iNumPlayer, int* aIDs, int iReason);
	//	Get player's ID by name
/*	void GetPlayerIDByName(const ACHAR* szName, int iReason);
	//	Submit problems to GM
	void SubmitProblemToGM(const ACHAR* szContent);
	//	Impeach else player to GM
	void ImpeachPlayerToGM(const ACHAR* szName, const ACHAR* szContent);
	//	Get ping time
	DWORD GetPingTime(int idServer, DWORD& dwStatus);
	
	//	General c2s commands ...
	void c2s_CmdPlayerMove(const A3DVECTOR3& vCurPos, const A3DVECTOR3& vDest, int iTime, float fSpeed, int iMoveMode, WORD wStamp) { ::c2s_SendCmdPlayerMove(vCurPos, vDest, iTime, fSpeed, iMoveMode, wStamp); }
	void c2s_CmdStartMove() { ::c2s_SendCmdStartMove(); }
	void c2s_CmdStopMove(const A3DVECTOR3& vDest, float fSpeed, int iMoveMode, BYTE byDir, WORD wStamp, int iTime) { ::c2s_SendCmdStopMove(vDest, fSpeed, iMoveMode, byDir, wStamp, iTime); }
	void c2s_CmdSelectTarget(int id) { m_CmdCache.SendCmdSelectTarget(id); }
	void c2s_CmdNormalAttack(BYTE byPVPMask) { ::c2s_SendCmdNormalAttack(byPVPMask); }
	void c2s_CmdReviveVillage() { m_CmdCache.SendCmdReviveVillage(); }
	void c2s_CmdReviveItem() { m_CmdCache.SendCmdReviveItem(); }
	void c2s_CmdGoto(float x, float y, float z) { ::c2s_SendCmdGoto(x, y, z); }
	void c2s_CmdGetExtProps() { m_CmdCache.SendCmdExtProps(); }
	void c2s_CmdGetPartExtProps(int iPropIndex) { ::c2s_SendCmdGetPartExtProps(iPropIndex); }
	void c2s_CmdLearnSkill(int idSkill) { ::c2s_SendCmdLearnSkill(idSkill); }
	void c2s_CmdLogout(int iOutType, int iTrusteeMode = 0) { ::c2s_SendCmdLogout(iOutType, iTrusteeMode); }
	void c2s_CmdGetAllData(bool bpd, bool bed, bool btd, bool bfd) { ::c2s_SendCmdGetAllData(bpd, bed, btd, bfd); }
	void c2s_CmdChargeEquipFlySword(int iEleIdx, int iCount);
	void c2s_CmdChargeFlySword(int iEleIdx, int iFSIdx, int iCount, int idFlySword);
	void c2s_CmdCancelAction() { m_CmdCache.SendCmdCancelAction(); }
	void c2s_CmdSitDown() { ::c2s_SendCmdSitDown(); }
	void c2s_CmdStandUp() { ::c2s_SendCmdStandUp(); }
	void c2s_CmdEmoteAction(WORD wPose) { ::c2s_SendCmdEmoteAction(wPose); }
	void c2s_CmdTaskNotify(const void* pData, DWORD dwDataSize) { ::c2s_SendCmdTaskNotify(pData, dwDataSize); }
	void c2s_CmdUnselect() { ::c2s_SendCmdUnselect(); }
	void c2s_CmdContinueAction() { ::c2s_SendCmdContinueAction(); }
	void c2s_CmdStopFall() { ::c2s_SendCmdStopFall(); }
	void c2s_CmdGatherMaterial(int idMatter, int iToolPack, int iToolIdx, int idTool, int idTask) { ::c2s_SendCmdGatherMaterial(idMatter, iToolPack, iToolIdx, idTool, idTask); }
	void c2s_CmdCastSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets) { m_CmdCache.SendCmdCastSkill(idSkill, byPVPMask, iNumTarget, aTargets); }
	void c2s_CmdCastInstantSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets) { m_CmdCache.SendCmdCastInstantSkill(idSkill, byPVPMask, iNumTarget, aTargets); }
	void c2s_CmdCastPosSkill(int idSkill, const A3DVECTOR3& vPos) { ::c2s_SendCmdCastPosSkill(idSkill, vPos); }
	void c2s_CmdCastApoiseSkill(int idSkill, BYTE bySkillType, int item_id, short item_index, BYTE force_attack, int iNumTarget, int* aTargets) {m_CmdCache.SendCmdCastApoiseSkill(idSkill, bySkillType, item_id, item_index, force_attack, iNumTarget, aTargets);}
	void c2s_CmdCastTransfigureSkill(int idSkill, BYTE bySkillType, BYTE byPVPMask, int iNumTarget, int* aTargets) { m_CmdCache.SendCmdTransfigureSkill(idSkill, bySkillType, byPVPMask, iNumTarget, aTargets); }
	void c2s_CmdCastChargeSkill(int idSkill, BYTE byPVPMask, int idTarget, const A3DVECTOR3& vPos, int iNumTarget, int* aTargets) {m_CmdCache.SendCmdChargeSkill(idSkill, byPVPMask, idTarget, vPos, iNumTarget, aTargets);}
	void c2s_CmdTrickAction(int iAction) { ::c2s_SendCmdTrickAction(iAction); }
	void c2s_CmdSetAdvData(int iData1, int iData2) { ::c2s_SendCmdSetAdvData(iData1, iData2); }
	void c2s_CmdClearAdvData() { ::c2s_SendCmdClearAdvData(); }
	void c2s_CmdQueryPlayerInfo1(int iNum, int* aIDs) { ::c2s_SendCmdQueryPlayerInfo1(iNum, aIDs); }
	void c2s_CmdQueryNPCInfo1(int iNum, int* aIDs) { ::c2s_SendCmdQueryNPCInfo1(iNum, aIDs); }
	void c2s_CmdSessionEmote(int iAction) { ::c2s_SendCmdSessionEmote(iAction); }
	void c2s_CmdConEmoteRequest(int iAction, int idTarget) { ::c2s_SendCmdConEmoteRequest(iAction, idTarget); }
	void c2s_CmdConEmoteReply(int iResult, int iAction, int idTarget) { ::c2s_SendCmdConEmoteReply(iResult, iAction, idTarget); }
	void c2s_CmdDeadMove(float y, int iTime, float fSpeed, int iMoveMode) { ::c2s_SendCmdDeadMove(y, iTime, fSpeed, iMoveMode); }
	void c2s_CmdDeadStopMove(float y, float fSpeed, BYTE byDir, int iMoveMode) { ::c2s_SendCmdDeadStopMove(y, fSpeed, byDir, iMoveMode); }
	void c2s_CmdEnterSanctuary() { m_CmdCache.SendCmdEnterSanctuary(); }
	void c2s_CmdOpenBooth(int iNumEntry, const void* szName, void* aEntries) { ::c2s_SendCmdOpenBooth(iNumEntry, szName, aEntries); }
	void c2s_CmdCloseBooth() { ::c2s_SendCmdCloseBooth(); }
	void c2s_CmdQueryBoothName(int iNum, int* aIDs) { ::c2s_SendCmdQueryBoothName(iNum, aIDs); }
	void c2s_CmdCompleteTravel() { ::c2s_SendCmdCompleteTravel(); }
	void c2s_CmdEnalbePVP() { ::c2s_SendCmdEnalbePVP(); }
	void c2s_CmdDisablePVP() { ::c2s_SendCmdDisablePVP(); }
	void c2s_CmdOpenBoothTest() { ::c2s_SendCmdOpenBoothTest(); }
	void c2s_CmdFashionSwitch() { ::c2s_SendCmdFashionSwitch(); }
	void c2s_CmdEnterInstance(int iTransIdx, int idInst) { m_CmdCache.SendCmdEnterInstance(iTransIdx, idInst); }
	void c2s_CmdRevivalAgree() { ::c2s_SendCmdRevivalAgree(); }
	void c2s_CmdActiveRushFly(bool bActive) { m_CmdCache.SendCmdActiveRushFly(bActive); }
	void c2s_CmdQueryDoubleExp() { ::c2s_SendCmdQueryDoubleExp(); }
	void c2s_CmdDuelRequest(int idTarget) { ::c2s_SendCmdDuelRequest(idTarget); }
	void c2s_CmdDuelReply(int idWho, int iReply) { ::c2s_SendCmdDuelReply(idWho, iReply); }
	void c2s_CmdBindPlayerRequest(int idTarget) { ::c2s_SendCmdBindPlayerRequest(idTarget); }
	void c2s_CmdBindPlayerInvite(int idTarget) { ::c2s_SendCmdBindPlayerInvite(idTarget); }
	void c2s_CmdBindPlayerRequestRe(int idWho, int iReply) { ::c2s_SendCmdBindPlayerRequestRe(idWho, iReply); }
	void c2s_CmdBindPlayerInviteRe(int idWho, int iReply);
	void c2s_CmdCancelBindPlayer() { ::c2s_SendCmdCancelBindPlayer(); }
	void c2s_SendCmdMallShopping(int iCount, const void* pGoods) { ::c2s_SendCmdMallShopping(iCount, pGoods); }
	void c2s_SendCmdMallShopping2(unsigned short id, unsigned short index, unsigned short slot, unsigned short count) { ::c2s_SendCmdMallShopping2(id, index, slot, count); }
	void c2s_CmdBonusMallShopping(unsigned short id, unsigned short index, unsigned short slot, unsigned short count) { ::c2s_SendCmdBonusMallShopping(id, index, slot, count); }
	void c2s_SendCmdSelectTitle(short title_id) { ::c2s_SendCmdSelectTitle(title_id); }
	void c2s_CmdDebugDeliverCmd(WORD type, const char* szCmd) { ::c2s_SendCmdDebugDeliverCmd(type, szCmd); }
	void c2s_CmdDebugGSCmd(const char* szCmd) { ::c2s_SendCmdDebugGSCmd(szCmd); }
	void c2s_CmdLotteryCashing(int item_index) { ::c2s_SendCmdLotteryCashing(item_index); }
	void c2s_CmdUseItemWithArg(unsigned char where, unsigned char count, unsigned short index, int item_id, const void* arg, size_t size) { ::c2s_SendCmdUseItemWithArg(where, count, index, item_id, arg, size); }
	//   多人骑乘
	void c2s_CmdMultiBindPlayerInvite(int idTarget) { ::c2s_SendCmdMultiBindPlayerInvite(idTarget); }
	void c2s_CmdMultiBindPlayerInviteRe(int idWho, int iReply); 
	void c2s_CmdCancelMultiBindPlayer() { ::c2s_SendCmdCancelMultiBindPlayer(); }
	void c2s_CmdMultiBindKick(int iPos) { ::c2s_SendCmdMultiBindKick(iPos); }

	// 征婚交友平台
	void c2s_CmdPressSNSMessage(unsigned char msg_type, unsigned char occupation, unsigned char gender, 
									int level, int factionLevel, const ACHAR* szMsg)
	{
		::c2s_SendCmdPressSNSMessage(msg_type, occupation, gender, level, factionLevel, szMsg);
	}
	void c2s_CmdApplySNSMessage(unsigned char msg_type, int msg_id, const ACHAR* szMsg){  ::c2s_SendCmdApplySNSMessage(msg_type, msg_id, szMsg); }
	void c2s_CmdVoteSNSMessage(unsigned char msg_type, int msg_id, char vote_type) { ::c2s_SendCmdVoteSNSMessage(msg_type, msg_id, vote_type);}
	void c2s_CmdResponseSNSMessage(unsigned char msg_type, int msg_id, short dst_index, const ACHAR* szMsg){ ::c2s_SendCmdResponseSNSMessage(msg_type, msg_id, dst_index, szMsg);}
	
	//	Carrier about
	void c2s_CmdEnterCarrier(int idCarrier, const A3DVECTOR3& vRelPos, unsigned char cRelDir) { ::c2s_SendCmdEnterCarrier(idCarrier, vRelPos, cRelDir); }
	void c2s_CmdLeaveCarrier(int idCarrier, const A3DVECTOR3& vPos, unsigned char cDir) { ::c2s_SendCmdLeaveCarrier(idCarrier, vPos, cDir); }
	void c2s_CmdMoveOnCarrier(const A3DVECTOR3& vCurRelPos, const A3DVECTOR3& vNextRelPos, unsigned short wUseTime, float fSpeed, int iMoveMode, unsigned short wCmdSeq) { ::c2s_SendCmdMoveOnCarrier(vCurRelPos, vNextRelPos, wUseTime, fSpeed, iMoveMode, wCmdSeq); }
	void c2s_CmdStopOnCarrier(const A3DVECTOR3& vRelDest, float fSpeed, int iMoveMode, BYTE byRelDir, WORD wStamp, int iTime) { ::c2s_SendCmdStopOnCarrier(vRelDest, fSpeed, iMoveMode, byRelDir, wStamp, iTime); }

	//	Team commands ...
	void c2s_CmdTeamInvite(int idPlayer);
	void c2s_CmdTeamAgreeInvite(int idLeader, int iTeamSeq) { ::c2s_SendCmdTeamAgreeInvite(idLeader, iTeamSeq); }
	void c2s_CmdTeamRejectInvite(int idLeader) { ::c2s_SendCmdTeamRejectInvite(idLeader); }
	void c2s_CmdTeamLeaveParty() { ::c2s_SendCmdTeamLeaveParty(); }
	void c2s_CmdTeamKickMember(int idMember) { ::c2s_SendCmdTeamKickMember(idMember); }
	void c2s_CmdTeamSetPickupFlag(short sFlag) { ::c2s_SendCmdTeamSetPickupFlag(sFlag); }
	void c2s_CmdTeamMemberPos(int iNumMem, int* aMemIDs) { ::c2s_SendCmdTeamMemberPos(iNumMem, aMemIDs); }
	void c2s_CmdTeamAssistSel(int idTeamMember) { ::c2s_SendCmdTeamAssistSel(idTeamMember); }
	void c2s_CmdTeamAskJoin(int idTarget) { ::c2s_SendCmdTeamAskJoin(idTarget); }
	void c2s_CmdTeamReplyJoinAsk(int idAsker, bool bAgree) { ::c2s_SendCmdTeamReplyJoinAsk(idAsker, bAgree); }
	void c2s_CmdTeamChangeLeader(int idLeader) { ::c2s_SendCmdTeamChangeLeader(idLeader); }
	void c2s_CmdTeamNotifyOwnPos() { ::c2s_SendCmdTeamNotifyOwnPos(); }
	//	Inventory and equipment commands ...
	void c2s_CmdGetIvtrData(int iPackage) { ::c2s_SendCmdGetIvtrData(iPackage); }
	void c2s_CmdGetIvtrDetailData(int iPackage) { ::c2s_SendCmdGetIvtrDetailData(iPackage); }
	void c2s_CmdMoveIvtrItem(int iSrc, int iDest, int iAmount);
	void c2s_CmdExgIvtrItem(int iIndex1, int iIndex2);
	void c2s_CmdDropIvtrItem(int iIndex, int iAmount);
	void c2s_CmdDropEquipItem(int iIndex);
	void c2s_CmdExgEquipItem(int iIndex1, int iIndex2);
	void c2s_CmdEquipItem(int iIvtrIdx, int iEquipIdx);
	void c2s_CmdMoveItemToEquip(int iIvtrIdx, int iEquipIdx);
	void c2s_CmdGetOtherEquip(int iNumID, int* aIDs) { ::c2s_SendCmdGetOtherEquip(iNumID, aIDs); }
	void c2s_CmdGetWealth(bool bpd, bool bed, bool btd) { ::c2s_SendCmdGetWealth(bpd, bed, btd); }
*/	void c2s_CmdGetItemInfo(BYTE byPackage, BYTE bySlot) { /*::c2s_SendCmdGetItemInfo(byPackage, bySlot)*/; }
/*	void c2s_CmdPickup(int idItem, int tid) { m_CmdCache.SendCmdPickUp(idItem, tid); }
	void c2s_CmdUseItem(BYTE byPackage, BYTE bySlot, int tid, BYTE byCount) { m_CmdCache.SendCmdUseItem(byPackage, bySlot, tid, byCount); }
	void c2s_CmdUseItemWithTarget(BYTE byPackage, BYTE bySlot, int tid, BYTE byPVPMask) { ::c2s_SendCmdUseItemWithTarget(byPackage, bySlot, tid, byPVPMask); }
	void c2s_CmdThrowMoney(DWORD dwAmount) { ::c2s_SendCmdThrowMoney(dwAmount); }
	void c2s_CmdGetItemInfoList(int iPackage, int iCount, BYTE* aIndices) { ::c2s_SendCmdGetItemInfoList(iPackage, iCount, aIndices); }
	void c2s_CmdGetTrashBoxData(bool bDetail) { ::c2s_SendCmdGetTrashBoxData(bDetail); }
	void c2s_CmdExgTrashBoxItem(bool bFaction, int iIndex1, int iIndex2);
	void c2s_CmdMoveTrashBoxItem(bool bFaction, int iSrc, int iDst, int iAmount);
	void c2s_CmdExgTrashBoxIvtrItem(bool bFaction, int iTrashIdx, int iIvtrIdx);
	void c2s_CmdMoveTrashBoxToIvtr(bool bFaction, int iTrashIdx, int iIvtrIdx, int iAmount);
	void c2s_CmdMoveIvtrToTrashBox(bool bFaction, int iTrashIdx, int iIvtrIdx, int iAmount);
	void c2s_CmdExgTrashBoxMoney(int iTrashMoney, int iIvtrMoney) { ::c2s_SendCmdExgTrashBoxMoney(iTrashMoney, iIvtrMoney); }
	void c2s_CmdGetOtherEquipDetail(int idTarget) { ::c2s_SendCmdGetOtherEquipDetail(idTarget); }
	void c2s_CmdGetCloneEquip(int idMaster, int idClone) { ::c2s_SendCmdGetCloneEquip(idMaster, idClone); }
	
	void c2s_CmdExgPocketItem(int iIndex1, int iIndex2);
	void c2s_CmdMovePocketItem(int iSrc, int iDst, int iAmount);
	void c2s_CmdExgPocketIvtrItem(int iPocIdx, int iIvtrIdx);
	void c2s_CmdMovePocketToIvtr(int iPocIdx, int iIvtrIdx, int iAmount);
	void c2s_CmdMoveIvtrToPocket(int iPocIdx, int iIvtrIdx, int iAmount);
	void c2s_CmdMovePocketAlltoIvtr();

	void c2s_CmdUpdateFashionHotkey();
	void c2s_CmdExgFashionItem(int iIndex1, int iIndex2);
	void c2s_CmdExgIvtrFashionItem(int iIvtrIdx, int iFasIdx);
	void c2s_CmdExgEquFashionItem(int iEquIdx, int iFasIdx);
	void c2s_CmdHotkeyChangeFashion(int iKeyIdx);
	
	//	Pet commands ...
	void c2s_CmdPetSummon(int iPetIdx) { ::c2s_SendCmdPetSummon(iPetIdx); } 
	void c2s_CmdPetRecall(int iPetIdx) { ::c2s_SendCmdPetRecall(iPetIdx); }
	void c2s_CmdPetSetRank(unsigned char pet_index, int pet_tid, unsigned char rank);
	void c2s_CmdPetEquipBedge(unsigned char inv_index, unsigned char pet_index);
	void c2s_CmdPetEquip(unsigned char inv_index, unsigned char pet_index);
	void c2s_CmdPetCtrlCmd(unsigned char pet_index, int target, int pet_cmd, void* buf, int len);
	void c2s_CmdPetCombine(unsigned char pet_index, unsigned char combine_type);
	void c2s_CmdPetUncombine(unsigned char pet_index);
	void c2s_CmdPetSetStatus(unsigned char pet_index, int pet_tid, unsigned char main_status, unsigned char sub_status);
	void c2s_CmdPetMoveBedgeItem( unsigned char src_index, unsigned char dst_index);
	//	Produce
	void c2s_CmdProduceItem(int iRecipeId) { ::c2s_SendCmdProduceItem(iRecipeId); }
	//	Fly
	void c2s_CmdStartFly();
	void c2s_CmdStopFly();
	//  Transfigure 
	void c2s_CmdStartTransfigure();
	void c2s_CmdStopTransfigure();
	
	// join instancing battle
	void c2s_CmdJoinInstancing(int map_id) { ::c2s_SendCmdJoinInstancing(map_id); }

	// get other palyer's achievement
	void c2s_CmdQueryOthersAchievement(const unsigned int idPlayer){::c2s_SendCmdQueryOthersAchievement(idPlayer); }
	// update hostplayer's pvp mask
	void c2s_CmdUpatePVPMask(BYTE mask) {::c2s_SendCmdUpdatePVPMask(mask); }
	// exchange game money and home town money
	void c2s_CmdExgHomeTownMoney(int money) {::c2s_SendCmdExchgHomeTownMoney(money); }
	// get server time
	void c2s_CmdGetServerTime() {::c2s_SendCmdGetServerTime(); }
	// territory battle
	void c2s_CmdTerritoryLeave(){::c2s_SendCmdTerritoryLeave(); }
	// update combine skill
	void c2s_CmdUpdateCombineSkill(int idSkill, int iNumElement, int* aElements) {::c2s_SendCmdUpdateCombineSkill(idSkill, iNumElement, aElements); }
	void c2s_CmdUniqueBid(int money){ ::c2s_SendCmdUniqueBid(money); }
	void c2s_CmdUniqueBidGetAward(){ ::c2s_SendCmdUniqueBidGetAward(); }

	//	NPC service commands .\..
	void c2s_CmdNPCSevHello(int nid) { m_CmdCache.SendCmdNPCSevHello(nid); }
	void c2s_CmdNPCSevBuy(int iItemNum, C2S::npc_trade_item* aItems) { ::c2s_SendCmdNPCSevBuy(iItemNum, aItems); }
	void c2s_CmdNPCSevSell(int iItemNum, C2S::npc_trade_item* aItems);
	void c2s_CmdNPCSevRepair(BYTE byPackage, BYTE bySlot, int idItem) { ::c2s_SendCmdNPCSevRepair(byPackage, bySlot, idItem); }
	void c2s_CmdNPCSevRepairAll() { ::c2s_SendCmdNPCSevRepairAll(); }
	void c2s_CmdNPCSevHeal() { ::c2s_SendCmdNPCSevHeal(); }
	void c2s_CmdNPCSevTransmit(int iIndex) { ::c2s_SendCmdNPCSevTransmit(iIndex); }
	void c2s_CmdNPCSevEmbed(int item_index, int item_id, int stone_index, int rt_index) { ::c2s_SendCmdNPCSevEmbed(item_index, item_id, stone_index, rt_index); }
	void c2s_CmdNPCSevClearEmbeddedChip(int iEquipIdx, int tidEquip, char uninstall_pstone) { ::c2s_SendCmdNPCSevClearEmbeddedChip(iEquipIdx, tidEquip, uninstall_pstone); }
	void c2s_CmdNPCSevLearnSkill(int idSkill) { ::c2s_SendCmdNPCSevLearnSkill(idSkill); }
	void c2s_CmdNPCSevMakeItem(int id) { ::c2s_SendCmdNPCSevMakeItem(id); }
	void c2s_SendCmdNPCSevResetPKValue(int value) { ::c2s_SendCmdNPCSevResetPKValue(value); }
	void c2s_CmdNPCSevAcceptTask(int idTask, int idTaskSet) { ::c2s_SendCmdNPCSevAcceptTask(idTask, idTaskSet); }
	void c2s_CmdNPCSevReturnTask(int idTask, int iChoice) { ::c2s_SendCmdNPCSevReturnTask(idTask, iChoice); }
	void c2s_CmdNPCSevTaskMatter(int idTask) { ::c2s_SendCmdNPCSevTaskMatter(idTask); }
	void c2s_CmdNPCSevChgTrashPsw(const char* szOldPsw, const char* szNewPsw) { ::c2s_SendCmdNPCSevChgTrashPsw(szOldPsw, szNewPsw); }
	void c2s_CmdNPCSevOpenTrash(const char* szPsw, bool bFaction = false) { ::c2s_SendCmdNPCSevOpenTrash(szPsw, bFaction); }
	void c2s_CmdNPCSevIdentify(int iSlot, int tidItem) { ::c2s_SendCmdNPCSevIdentify(iSlot, tidItem); }
	void c2s_CmdNPCSevFaction(int iAction, void* pDataBuf, int iDataSize) { ::c2s_SendCmdNPCSevFaction(iAction, pDataBuf, iDataSize); }
	void c2s_CmdNPCSevTravel(int iIndex, int idLine) { ::c2s_SendCmdNPCSevTravel(iIndex, idLine); }
	void c2s_CmdNPCSevGetContent(int idService) { ::c2s_SendCmdNPCSevGetContent(idService); }
	void c2s_CmdNPCSevBoothBuy(int idBooth, int iItemNum, C2S::npc_booth_item* aItems) { ::c2s_SendCmdNPCSevBoothBuy(idBooth, iItemNum, aItems); }
	void c2s_CmdNPCSevBoothSell(int idBooth, int iItemNum, C2S::npc_booth_item* aItems);
	void c2s_CmdNPCSevWaypoint() { ::c2s_SendCmdNPCSevWaypoint(); }
	void c2s_CmdNPCSevForgetSkill(int idSkill) { ::c2s_SendCmdNPCSevForgetSkill(idSkill); }
	void c2s_CmdNPCSevFaceChange(int iIvtrIdx, int idItem) { ::c2s_SendCmdNPCSevFaceChange(iIvtrIdx, idItem); }
	void c2s_CmdNPCSevMail(int iAction, void* pDataBuf, int iDataSize) { ::c2s_SendCmdNPCSevMail(iAction, pDataBuf, iDataSize); }
	void c2s_CmdNPCSevVendue(int iAction, void* pDataBuf, int iDataSize) { ::c2s_SendCmdNPCSevVendue(iAction, pDataBuf, iDataSize); } 
	void c2s_CmdNPCSevDblExpTime(int iIndex) { ::c2s_SendCmdNPCSevDblExpTime(iIndex); }
	void c2s_CmdNPCSevHatchPet(int iIvtrIdx, int idEgg) { ::c2s_SendCmdNPCSevHatchPet(iIvtrIdx, idEgg); }
	void c2s_CmdNPCSevRestorePet(int iPetIdx) { ::c2s_SendCmdNPCSevRestorePet(iPetIdx); }
	void c2s_CmdNPCSevBattle(int iAction, void* pDataBuf, int iDataSize) { ::c2s_SendCmdNPCSevBattle(iAction, pDataBuf, iDataSize); }
	void c2s_CmdNPCSevBuildTower(int iTowerIdx, int idItem) { ::c2s_SendCmdNPCSevBuildTower(iTowerIdx, idItem); }
	void c2s_CmdNPCSevLeaveBattle() { ::c2s_SendCmdNPCSevLeaveBattle(); }
	void c2s_CmdNPCSevReturnStatusPt(int iIndex, int idItem) { ::c2s_SendCmdNPCSevReturnStatusPt(iIndex, idItem); }
	void c2s_CmdNPCSevAccountPoint(int iAction, void* pDataBuf, int iDataSize) { ::c2s_SendCmdNPCSevAccountPoint(iAction, pDataBuf, iDataSize); }
	void c2s_CmdNPCSevBindItem(int iIndex, int idItem) { ::c2s_SendCmdNPCSevBindItem(iIndex, idItem); }
	void c2s_CmdNPCSevDestroyBindItem(int iIndex, int idItem) { ::c2s_SendCmdNPCSevDestroyBindItem(iIndex, idItem); }
	void c2s_CmdNPCSevCancelDestroy(int iIndex, int idItem) { ::c2s_SendCmdNPCSevCancelDestroy(iIndex, idItem); }
	void c2s_CmdNPCSevStockTransaction(int nWithdraw, int nCash, int nMoney) { ::c2s_SendCmdNPCSevStockTransaction(nWithdraw, nCash, nMoney); }
	void c2s_CmdNPCSevStockOperation(int nType, void* pData, int nLen) { ::c2s_SendCmdNPCSevStockOperation(nType, pData, nLen); }
	void c2s_CmdNPCSevTalismanRefine(int item_index, int item_id, int material_id) { ::c2s_SendCmdNPCSevTalismanRefine(item_index, item_id, material_id); }
	void c2s_CmdNPCSevTalismanUpgrade(int item_index, int item_id) { ::c2s_SendCmdNPCSevTalismanUpgrade(item_index, item_id); }
	void c2s_CmdNPCSevTalismanRemoveOwner(int item_index, int item_id) { ::c2s_SendCmdNPCSevTalismanRemoveOwner(item_index, item_id); }
	void c2s_CmdNPCSevTalismanCombine(int item_index1, int item_id1, int item_index2, int item_id2, int catalyst_id) { ::c2s_SendCmdNPCSevTalismanCombine(item_index1, item_id1, item_index2, item_id2, catalyst_id); }
	void c2s_CmdNPCSevTalismanEnchant(int item_index1, int item_type1, int item_index2, int item_type2) { ::c2s_SendCmdNPCSevTalismanEnchant(item_index1, item_type1, item_index2, item_type2); }
	void c2s_CmdNPCSevPetAdopt(int inv_index);
	void c2s_CmdNPCSevFree(int inv_index);
	void c2s_CmdNPCSevCombine(int inv_index_pet, int inv_index_c, int inv_index_a);
	void c2s_CmdNPCSevRename(int inv_index, const ACHAR* szName);
	void c2s_CmdNPCSevVehicleUpgrade(int inv_mount, int inv_material);
	void c2s_CmdNPCSevItemTrade(const C2S::item_trade* pItemList, size_t count);
	void c2s_CmdNPCSevLockItem(int item_index, int item_id);
	void c2s_CmdNPCSevUnlockItem(int item_index, int item_id);
	void c2s_CmdNPCSevRepairDamagedItem(int iIndex, int idItem) { ::c2s_SendCmdNPCSevRepairDamagedItem(iIndex, idItem); }
	void c2s_CmdNPCSevBleedSacrifice(int iIndex, int idItem){ ::c2s_SendCmdNPCSevBleedSacrifice(iIndex, idItem); }
	void c2s_CmdNPCSevEmbedSoul(int item_index, int item_id, int soul_index, int soul_slot){ ::c2s_SendCmdNPCSevEmbedSoul(item_index, item_id, soul_index, soul_slot); }
	void c2s_CmdNPCSevClearSoulTessera(int item_index, int item_id, int soul_slot){ ::c2s_SendCmdNPCSevClearSoulTessera(item_index, item_id, soul_slot); }
	void c2s_CmdNPCSevRefineSoul(int item_index, int item_id){ ::c2s_SendCmdNPCSevRefineSoul(item_index, item_id); }										//恢复魂力
	void c2s_CmdNPCSevBreakDownEquip(int item_index, int item_id){::c2s_SendCmdNPCSevBreakDownEquip(item_index, item_id); }
	void c2s_CmdNPCSevMeldSoul(int recipe_id, int territory_id) { ::c2s_SendCmdNPCSevMeldSoul(recipe_id, territory_id);}
	void c2s_CmdNPCSevPetEquipEnchant(int item_index, int item_id, int stone_index) {::c2s_SendCmdNPCSevPetEquipEnchant(item_index, item_id, stone_index);}
	void c2s_CmdNPCSevChangeFace(int faceid, int hairid, int earid, int tailid, int stone_index) {::c2s_SendCmdNPCSevChangeFace(faceid, hairid, earid, tailid, stone_index); }	
	void c2s_CmdNPCSevRefineChangeCard(int item_index, int item_id, int stone_index, int stone_id, int sev_type){::c2s_SendCmdNPCSevRefineChangeCard(item_index, item_id,stone_index,stone_id, sev_type);}
	void c2s_CmdNPCSevRecoverCardWakan(int item_index, int item_id){::c2s_SendCmdNPCSevRecoverCardWakan(item_index, item_id); }	
	void c2s_CmdNPCSevArenaJoin(int map_id, int item_index){::c2s_SendCmdNPCSevArenaJoin(map_id, item_index);}
	void c2s_CmdNPCSevTerritoryChallenge(int faction_id, int territory_id, int item_id, int item_count){::c2s_SendCmdNPCSevTerritoryChallenge(faction_id, territory_id, item_id, item_count); }
	void c2s_CmdNPCSevTerritoryEnter(int faction_id, int territory_id, int is_assistant){::c2s_SendCmdNPCSevTerritoryEnter(faction_id, territory_id, is_assistant); }
	void c2s_CmdNPCSevTerritoryAward(int faction_id, int territory_id, int reward_type, int item_id, int item_count, int money) {::c2s_SendCmdNPCSevTerritoryAward(faction_id, territory_id, reward_type, item_id, item_count, money); }
	void c2s_CmdNPCSevTeleportationCharge(int item_index, int item_id, int stone_index, int stone_id){::c2s_SendCmdNPCSevTeleportationCharge(item_index, item_id, stone_index, stone_id); }
	void c2s_CmdNPCSevRepairDamagedItem2(int iIndex, int idItem) { ::c2s_SendCmdNPCSevRepairDamagedItem2(iIndex, idItem); }
	void c2s_CmdSendCmdNPCSevUpgradeEquip(int item_index, int item_id, int stone_index, int rt_index){::c2s_SendCmdNPCSevUpgradeEquip(item_index, item_id, stone_index, rt_index); }

	//	Trade commands ...
	bool trade_Start(int idTarget);
	bool trade_StartResponse(DWORD& dwHandle, bool bAgree);
	bool trade_AddGoods(int idTrade, int idItem, int iPos, int iCount, int iMoney);
	bool trade_RemoveGoods(int idTrade, int idItem, int iPos, int iCount, int iMoney);
	bool trade_MoveItem(int idTrade, int idItem, int iSrc, int iDst, int iCount);
	bool trade_Submit(int idTrade);
	bool trade_Confirm(int idTrade);
	bool trade_Cancel(int idTrade);
	//  fun game commands ...
	void fgc2s_SendCmdGetRoomList(BYTE byGameType, BYTE byIndex) { ::fgc2s_SendCmdGetRoomList(byGameType, byIndex); }
	void fgc2s_SendCmdPlayerEnterRoom(BYTE byGameType, WORD wRoomID, BYTE byDir) { ::fgc2s_SendCmdPlayerEnterRoom(byGameType, wRoomID, byDir); }
	void fgc2s_SendCmdPlayerBeginGame(BYTE byGameType, WORD wRoomID) { ::fgc2s_SendCmdPlayerBeginGame(byGameType, wRoomID); }
	void fgc2s_SendCmdPlayerReEnterRoom(BYTE byGameType, WORD wRoomID) { ::fgc2s_SendCmdPlayerReEnterRoom(byGameType, wRoomID); }
	void fgc2s_SendCmdPlayerLeaveRoom(BYTE byGameType, WORD wRoomID) { ::fgc2s_SendCmdPlayerLeaveRoom(byGameType, wRoomID); }
	void fgc2s_SendCmdPlayerThrowNormalCard(BYTE byGameType, WORD wRoomID, BYTE byCardNum, const BYTE* aCards) { ::fgc2s_SendCmdPlayerThrowNormalCard(byGameType, wRoomID, byCardNum, aCards); }
	void fgc2s_SendCmdPlayerPickupNormalCard(BYTE byGameType, WORD wRoomID) { ::fgc2s_SendCmdPlayerPickupNormalCard(byGameType, wRoomID); }
	void fgc2s_SendCmdPlayerGiveupNormalCard(BYTE byGameType, WORD wRoomID) { ::fgc2s_SendCmdPlayerGiveupNormalCard(byGameType, wRoomID); }
	void fgc2s_SendCmdPlayerAuction(BYTE byGameType, WORD wRoomID, int idRole, BYTE byScore) { ::fgc2s_SendCmdPlayerAuction(byGameType, wRoomID, idRole, byScore); }
	void fgc2s_SendCmdPlayerEnterHall(BYTE byGameType) { ::fgc2s_SendCmdPlayerEnterHall(byGameType); }
	void fgc2s_SendCmdPlayerLeaveHall(BYTE byGameType) { ::fgc2s_SendCmdPlayerLeaveHall(byGameType); }

	//  home town commands ...
	void htc2s_SendCmdGetCompoList(int roleid){ ::htc2s_SendCmdGetCompoList(roleid); }
	void htc2s_SendCmdAddCompo(int id){::htc2s_SendCmdAddCompo(id); }
	void htc2s_SendCmdExgMoney(BYTE type, __int64 money){::htc2s_SendCmdExgMoney(type, money); }
	void htc2s_SendCmdLevelUp(){::htc2s_SendCmdLevelUp(); }
	void htc2s_SendCmdBuyItem(int itemid, int count){::htc2s_SendCmdBuyItem(itemid, count); }
	void htc2s_SendCmdUseItem(int pos, int itemid, int target_type, int roleid, int fieldid){::htc2s_SendCmdUseItem(pos, itemid, target_type, roleid, fieldid); }
	void htc2s_SendCmdSellItem(int pos, int itemid, int count){::htc2s_SendCmdSellItem(pos, itemid, count); }
	void htc2s_SendCmdGetPackage(){::htc2s_SendCmdGetPackage(); }
	void htc2s_SendCmdViewFarm(int roleid){::htc2s_SendCmdViewFarm(roleid); }
	void htc2s_SendCmdPlowField(int roleid, int fieldid, int emotion){::htc2s_SendCmdPlowField(roleid, fieldid, emotion); }
	void htc2s_SendCmdWaterField(int roleid, int fieldid, int emotion){::htc2s_SendCmdWaterField(roleid, fieldid, emotion); }
	void htc2s_SendCmdSowField(int roleid, int fieldid, int seedid){::htc2s_SendCmdSowField(roleid, fieldid, seedid); }
	void htc2s_SendCmdPestField(int roleid, int fieldid, int emotion){::htc2s_SendCmdPestField(roleid, fieldid, emotion); }
	void htc2s_SendCmdWeedField(int roleid, int fieldid, int emotion){::htc2s_SendCmdWeedField(roleid, fieldid, emotion); }
	void htc2s_SendCmdHarvestField(int roleid, int fieldid, int emotion){::htc2s_SendCmdHarvestField(roleid, fieldid, emotion); }
	void htc2s_SendCmdStealField(int roleid, int fieldid, int emotion){::htc2s_SendCmdStealField(roleid, fieldid, emotion); }
	void htc2s_SendCmdListLoveFields(){::htc2s_SendCmdListLoveFields(); }
	void htc2s_SendCmdListMatureFields(){::htc2s_SendCmdListMatureFields();}

	//	GM commands ...
	void gm_KickOutRole(int idPlayer, int iTime, const ACHAR* szReason);
	void gm_KickOutUser(int idPlayer, int iTime, const ACHAR* szReason);
	void gm_ListOnlineUser(int iHandler);
	void gm_OnlineNumber();
	void gm_RestartServer(int idServer, int iRestartTime);
	void gm_ShutupRole(int idPlayer, int iTime, const ACHAR* szReason);
	void gm_ShutupUser(int idPlayer, int iTime, const ACHAR* szReason);
	void gm_MoveToPlayer(int idPlayer) { ::c2s_SendCmdGMMoveToPlayer(idPlayer); }
	void gm_CallInPlayer(int idPlayer) { ::c2s_SendCmdGMCallInPlayer(idPlayer); }
	void gm_ForbidRole(int iType, int idPlayer, int iTime, const ACHAR* szReason);
	void gm_TriggerChat(bool bEnable);
	void gm_Generate(int tid) { ::c2s_SendCmdGMGenerate(tid); }
	void gm_ActiveSpawner(bool bActive, int id_spawner) { ::c2s_SendCmdGMActiveSpawner(bActive, id_spawner); }
	void gm_GenerateMob(int idMob, int idVis, short sCount, short sLifeTime, const ACHAR* szName) { ::c2s_SendCmdGMGenerateMob(idMob, idVis, sCount, sLifeTime, szName); }
	void gm_TriggerInvisible() { ::c2s_SendCmdGMTriggerInvisible(); }
	void gm_TriggerInvincible() { ::c2s_SendCmdGMTriggerInvincible(); }
	//	Friend commands ...
	void friend_Add(int idPlayer, const ACHAR* szName);
	void friend_AddResponse(DWORD& dwHandle, bool bAgree);
	void friend_GetList();
	void friend_SetGroupName(int iGroup, const ACHAR* szName);
	void friend_SetGroup(int iGroup, int idFriend);
	void friend_Delete(int idFriend);
	void friend_GetOfflineMsg();
	//	Faction commands ...
	void faction_chat(const ACHAR* szMsg, char channel, int nPack = -1, int nSlot = 0);
	void faction_create(const ACHAR* szName, const ACHAR* szProclaim, char scale);
	void faction_change_proclaim(const ACHAR* szNew, char scale);
	void faction_accept_join(int iApplicant, char scale);
	void faction_expel_member(int iMember, char scale);
	void faction_appoint(int iMember, char new_position);
	void faction_master_resign(int iNewMaster, char scale);
	void faction_resign(char scale);
	void faction_leave(char scale);
	void faction_broadcast(const ACHAR* szMsg);
	void faction_dismiss(char scale, bool del = true);
	void faction_upgrade(char scale);
	void faction_degrade();
	void faction_listmember(int page, unsigned int familyid, bool first_time);
	void faction_rename(int iRole, const ACHAR* szNewName);
	void faction_invite_response(DWORD& dwHandle, bool bAgree, char scale);
	void faction_player_info();
	//	Top table
	void toptable_get(int iTable, int iPage, bool bNeedMyRank);
	//	Enemies
	void enemies_get(int iRole, int iFullList);
	void enemies_update(int iRole, int iEnemy, char oprt);
	//	Chatroom commands ...
	void chatroom_Create(const ACHAR* szSubject, const ACHAR* szPassword, int iCapacity);
	void chatroom_Expel(int idRoom, int idPlayer);
	void chatroom_Invite(int idRoom, int idInvitee);
	void chatroom_Invite(int idRoom, const ACHAR* szName);
	void chatroom_RejectInvite(int idRoom, int idInviter);
	void chatroom_Join(int idRoom, const ACHAR* szPassword);
	void chatroom_Join(const ACHAR* szOwner, const ACHAR* szPassword);
	void chatroom_Leave(int idRoom);
	void chatroom_List(int iBegin, bool bReverse);
	void chatroom_Speak(int idRoom, int idDstPlayer, const ACHAR* szMsg);
	//	Mail commands ...
	void mail_CheckNew();
	void mail_GetList();
	void mail_Get(int idMail);
	void mail_GetAttachment(int idMail, int iType);
	void mail_Delete(int idMail);
	void mail_Preserve(int idMail, bool bPreserve);
	void mail_Send(int idReceiver, const ACHAR* szTitle, const ACHAR* szContext, int idItem, int iItemNum, int iItemPos, size_t dwMoney);
	//	Auction commands ...
	void auction_Open(int iCategory, int tid, int iItemPos, int iItemNum, int iBasePrice, int iBinPrice, int iTime);
	void auction_Bid(int idAuction, int iBidPrice, bool bBin);
	void auction_List(int iCategory, int iBegin, bool bReverse);
	void auction_Close(int idAuction, int iReason);
	void auction_Get(int idAuction);
	void auction_AttendList();
	void auction_ExitBid(int idAuction);
	void auction_GetItems(int iNumItem, int* aAuctionIDs);
	//	Stock commands
	void stock_account();
	void stock_bill();
	void stock_commission(char bBuy, int iPrice, int iVolume);
	void stock_cancel(int tid, int price);
	//	Battle commands ...
	void battle_getlist(char battle_type);
	void battle_getfield(int line_id, int map_id);
	void battle_join(int line_id, int map_id, unsigned char team);
	void battle_enter(int line_id, int map_id);
	void battle_leave(); 
	//	Instancing commands  
	//  新增情景战场
	void instancing_availablelist();
	void instancing_getlist(int battle_id);
	void instancing_getfield(int line_id, int map_id);
	void instancing_enter(int line_id, int map_id);
	void instancing_appoint(int map_id, int target_id);
	void instancing_accept(bool bAccept, int map_id, int requester);
	void instancing_kick(int map_id, int target);
	void instancing_leave(int map_id);
	void instancing_start(int map_id);
	//	War commands
	void war_challege(int battle_id, int day);
	void war_enter(int battle_id);
	void war_infoget(int battle_id);
	void war_assistant(int battle_id, const ACHAR* szFactionName);
	void war_buy_archer();
	void war_get_construction_info(int mode, int type, int key, int cur_level = 0);
	void war_top(int battle_id);
	//	Account commands ...
	void account_GetSelfSellList();
	void account_GetShopSellList(int idStart, bool bReverse);
	void account_SellPoint(int iPoint, DWORD dwPrice);
	void account_CancelSellPoint(int idSell);
	void account_BuyPoint(int idSell, int idSeller);
	//	Combat commands
	void combat_Challenge(int roleid, int faction);
	void combat_Top(int roleid, int faction, int handle);
	void combat_invite_response(DWORD& dwHandle, bool bAgree);
	// Bot commands
	void bot_begin(char flag, char answer_question) { c2s_SendCmdBotBegin(flag, answer_question); }
	void bot_run() { c2s_SendCmdBotRun(); }
	//	Player change GS
	void player_change_gs(int line, int instance, int reason, float pos_x, float pos_z);
	//	ping commands ...
	void server_Ping(int idServer, in_addr * addr, int nPort);
	//	matrix commands ...
	void matrix_response(int iResponse);
	//	Output link server error
	void OutputLinkSevError(int iRetCode);
	// cash
	void cash_Lock(char cLock, const ACHAR* szPassword);
	void cash_SetPassword(const ACHAR* szPassword);	
	// ac answer
	void send_acanswer(int type, int seq, int reserved, int answer);
	// Autolock
	void autolock_set(int timeout);	
	// Contest
	void contest_invite_re(int accept_invite);
	void contest_answer(int questionid, int answer, char usestar);
	void contest_exit();
	// Sect
	void sect_list();	
	void sect_expel(int roldid);	
	void sect_recruit(int roldid);	
	bool sect_invite_response(DWORD& dwHandle, bool bAgree);

	// sns commands 
	// 征婚（交友）平台

	void sns_accept_apply(int apply_roleid, char message_type, int message_id, char oper);
	void sns_cancel_message(int message_id, char message_type);
	void sns_get_message(char message_type, int message_id);
	void sns_get_playerinfo(char check_private, int asker);
	void sns_list_message(char message_type);
	void sns_set_playerinfo(int is_private,  const ACHAR* szNickname, short age, int birthday, int province, int city, const ACHAR* szCareer, const ACHAR* szAddress, const ACHAR* szIntroduce);
	void sns_del_message(int message_id, char message_type, int leavemsg_id);

	// 线上推广
	void ref_get_reference_code();
	void ref_get_referrals(int idx);
	void ref_withdraw_bonus();
	void ref_withdraw_exp();


	// 领土战
	void territory_get_map();
	// 最低价拍卖信息获取
	void unique_bid_get_history();

	//  Circle commands
	void circle_get_list(int idCircle);
	void circle_get_baseinfo(int iNumCircle, const int* aCircleIDs);
	void circle_get_award(int idPlayer);
	void circle_chat(const ACHAR* szMsg, char channel, int nPack, int nSlot);

	//	Set user name
	void SetUserName(const char* szName) { m_strUserName = szName; }
	//	Get user name
	const char* GetUserName() { return m_strUserName; }
	//	Set user password
	void SetUserPassword(const char* szPass) { m_strPassword = szPass; }
	//	Get user password
	const char* GetUserPassword() { return m_strPassword; }
	//	Set user ID
	void SetUserID(int iUserID) { m_iUserID = iUserID; }
	//	Get user ID
	int GetUserID() { return m_iUserID; }
	//	Set user character ID
	void SetCharacterID(int iCharID) { m_iCharID = iCharID; }
	//	Get user character ID
	int GetCharacterID() { return m_iCharID; }
*/	//	Get connected flag
	bool IsConnected() { return m_bConnected; }
	//	Get kick user flag
/*	bool GetKickUserFlag() { return m_bKickUser; }
	//	Set kick user flag
	void SetKickUserFlag(bool bKick) { m_bKickUser = bKick; }
	//	Get account remained time
	__int64 GetAccountTime() { return m_iiAccountTime; }
	//	Get remained free game time
	__int64 GetFreeGameTime() { return m_iiFreeGameTime; }
	//	Get end time of free game
	int GetFreeGameEndTime() { return m_iFreeGameEnd; }
*/	//	Get overtime counter
	bool GetOvertimeCnt(DWORD& dwTotalTime, DWORD& dwCnt);
	//	Start / End overtime
	void DoOvertimeCheck(bool bStart, int iCheckID, DWORD dwTime);
	//	Log S2C command data
/*	void LogS2CCmdData(int iCmd, BYTE* pDataBuf, int nSize);

	//	Add id of player whose brief info will be got
	void AddIDForBriefInfo(int id)
	{
		if (m_bProcNewPtrc)
			m_aPBIPlayers.Add(id);
	}

	//	Get net manager
*/	GNET::GameClient* GetNetManager() { return m_pNetMan; }
/*
//	void SetDistrictID(int id){ m_iDistrictID = id; }
	int GetDistrictID() const  { return m_iDistrictID; }

	// 刚刚登陆的玩家是否已经有了推荐人，如果没有则提示玩家来输入推广码
//	void SetHasReferrer(bool b) { m_bHasReferrer = b; }
	bool IsHasReferrer() { return m_iReferrer > 0; }
	int  GetReferrerID() const { return m_iReferrer; }
	int  GetZoneID() const { return m_zoneID; }
*/
protected:	//	Attributes

	APtrArray<GNET::Protocol*>		m_aNewProtocols;	//	New protocol array
/*	APtrArray<CECNetProtocolBase*>	m_aOldProtocols;	//	Old protocol array
	APtrArray<CECNetProtocolBase*>	m_aTempProtocols;	//	Temporary protocol array

	AArray<int, int>		m_aPBIPlayers;		//	ID of players whose brief info should be got from server
	APtrArray<PINGTIME*>	m_Ping;				//	Ping time
	CRITICAL_SECTION		m_csPingSection;

*/	GNET::GameClient*	m_pNetMan;		//	Net manager
	DWORD				m_idLink;		//	Link ID
/*	CECStringTab		m_ErrorMsgs;	//	Server error messages
	CECC2SCmdCache		m_CmdCache;		//	C2S command cache
	CECPrtcPBIProc*		m_pPBIProc;		//	Player brief info processor
*/	bool				m_bProcNewPtrc;	//	true, is processing new protocols

	volatile bool		m_bConnected;		//	Connected flag
	
	bool		m_bLinkBroken;		//	Connection was broken
/*	AString		m_strUserName;		//	User name
	AString		m_strPassword;		//	User password
	int			m_iUserID;			//	User ID
	int			m_iCharID;			//	User character ID, this ID should be same as host player's cid
	bool		m_bKickUser;		//	true, kill user who is using the same account
	__int64		m_iiAccountTime;	//	Account remained time
	__int64		m_iiFreeGameTime;	//	Remained free game time
	int			m_iFreeGameEnd;		//	Free game end time
	DWORD		m_dwLastAccTime;
*/	HANDLE		m_hConnectThread;	//	Handle of connect thread
/*
	PONRECEIVEHELPSTATES m_pfnHelpStatesCallBack;
*/
	OVERTIME	m_ot;				//	Overtime info
/*
	int			m_nS2CCmdSize;		//	total server to client command size
	int			m_iDistrictID;
	int			m_iReferrer;		//  推荐人的id
	int         m_zoneID;			//  服务器id
	
	ALog*		m_pS2CCmdLog;		//	Command log file
	AFile*		m_pS2CCmdBinLog;	//	Command log in binary format
	CECFunGamePrtcHandler* m_pFunGamePrtcHandler;
	CECHomeTownPrtcHandler* m_pHomeTownPrtcHandler;

protected:	//	Operations

*/	//	Connect server
	bool Connect();
	//	Send net data
	bool SendNetData(const GNET::Protocol* p) { return p ? SendNetData(*p) : false; }
	//	Proecess game data
/*	void ProcessGameData(const GNET::Octets& Data);
	//  Process fun game data
	void ProcessFunGameData(GNET::Protocol*p);

	void ProcessHomeTownData(GNET::Protocol*p);
	//	Calculate S2C command data size
	DWORD CalcS2CCmdDataSize(int iCmd, BYTE* pDataBuf, DWORD dwDataSize);
	//	When connection was broken, this function is called
	void LinkBroken(bool bDisconnect);
	//	On overtime happens
	void OnOvertimeHappen();
	//	Freeze host's inventory item
	void FreezeHostItem(int iIvtr, int iIndex, bool bFreeze);

	//	Protocol handlers
	void OnPrtcChallenge(GNET::Protocol* pProtocol);
	void OnPrtcKeyExchange(GNET::Protocol* pProtocol);
	void OnPrtcOnlineAnnounce(GNET::Protocol* pProtocol);
	void OnPrtcRoleListRe(GNET::Protocol* pProtocol);
	void OnPrtcSelectRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcCreateRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcDeleteRoleRe(GNET::Protocol* pProtocol);
	void OnPtrcUndoDeleteRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcChatMessage(GNET::Protocol* pProtocol, bool& bAddToOld);
	void OnPrtcWorldChat(GNET::Protocol* pProtocol);
	void OnPrtcErrorInfo(GNET::Protocol* pProtocol);
	void OnPrtcPlayerLogout(GNET::Protocol* pProtocol);
	void OnPrtcPrivateChat(GNET::Protocol* pProtocol);
	void OnPrtcPlayerBaseInfoRe(GNET::Protocol* pProtocol);
	void OnPrtcPlayerBaseInfo2Re(GNET::Protocol* pProtocol);
	void OnPrtcGetConfigRe(GNET::Protocol* pProtocol);
	void OnPrtcSetConfigRe(GNET::Protocol* pProtocol);
	void OnPrtcGetHelpStatesRe(GNET::Protocol* pProtocol);
	void OnPrtcSetHelpStatesRe(GNET::Protocol* pProtocol);
	void OnPrtcSetCustomDataRe(GNET::Protocol* pProtocol);
	void OnPrtcGetCustomDataRe(GNET::Protocol* pProtocol);
	void OnPrtcGetPlayerIDByNameRe(GNET::Protocol* pProtocol);
	void OnPrtcRoleStatusAnnounce(GNET::Protocol* pProtocol);
	void OnPrtcAnnounceForbidInfo(GNET::Protocol* pProtocol);
	void OnPrtcUpdateRemainTime(GNET::Protocol* pProtocol);
	void OnPrtcSubmitProblemToGMRe(GNET::Protocol* pProtocol);
	void OnPrtcImpeachPlayerToGMRe(GNET::Protocol* pProtocol);
	void OnPrtcMatrixChallenge(GNET::Protocol* pProtocol);
	void OnPrtcCashOpt(GNET::Protocol* pProtocol);

	void OnTradeProtocols(GNET::Protocol* pProtocol);
	void OnFriendProtocols(GNET::Protocol* pProtocol);
	void OnFactionProtocols(GNET::Protocol* pProtocol);
	void OnChatRoomProtocols(GNET::Protocol* pProtocol);
	void OnMailProtocols(GNET::Protocol* pProtocol);
	void OnVendueProtocols(GNET::Protocol* pProtocol);
	void OnBattleProtocols(GNET::Protocol* pProtocol);
	void OnInstancingProtocols(GNET::Protocol * pProtocol);
	void OnAccountProtocols(GNET::Protocol* pProtocol);
	void OnSectProtocols(GNET::Protocol* pProtocol);
	void OnTerritoryProtocols(GNET::Protocol* pProtocol);
	void OnCircleProtocols(GNET::Protocol* pProtocol);

	void OnPrtcGMKickOutRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcGMKickOutUserRe(GNET::Protocol* pProtocol);
	void OnPrtcGMListOnlineUserRe(GNET::Protocol* pProtocol);
	void OnPrtcGMOnlineNumberRe(GNET::Protocol* pProtocol);
	void OnPrtcGMRestartServerRe(GNET::Protocol* pProtocol);
	void OnPrtcGMRestartServer(GNET::Protocol* pProtocol);
	void OnPrtcGMShutupRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcGMShutupUserRe(GNET::Protocol* pProtocol);
	void OnPrtcGMForbidRoleRe(GNET::Protocol* pProtocol);
	void OnPrtcGMToggleChatRe(GNET::Protocol* pProtocol);*/
	void OnPrtcGetItemDesc(GNET::Protocol* pProtocol);//寻宝网协议处理函数
	void OnPrtcGetItemDescForPlatform( GNET::Protocol* pProtocol );//系统平台协议处理函数
	void OnPrtcProcessGetItemDescForSNS( GNET::Protocol* pProtocol );//SNS平台协议处理函数
	void OnPrtcGetRoleDesc( GNET::Protocol* pProtocol );//寻宝网角色交易协议处理函数
	void OnPrtcGetRoleDescSnS( GNET::Protocol* pProtocol );//主题站角色展示数据处理函数

};



///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

