/*
 * FILE: EC_SendC2SCmds.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/22
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

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

namespace C2S
{
	struct npc_trade_item;
	struct npc_booth_item;
	struct npc_produce_jinfashen_material;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

void _SendEmptyDataNPCSev(int iSevice);
void c2s_SendDebugCmd(int iCmd, int iNumParams, ...);
void c2s_SendDebugCmd1(int iCmd, int iNumParams, ...);

//	Assistant functions used to send C2S commands
void c2s_SendCmdPlayerMove(const A3DVECTOR3& vCurPos, const A3DVECTOR3& vDest, int iTime, float fSpeed, int iMoveMode, WORD wStamp);
void c2s_SendCmdStartMove();
void c2s_SendCmdStopMove(const A3DVECTOR3& vDest, float fSpeed, int iMoveMode, BYTE byDir, WORD wStamp, int iTime);
void c2s_SendCmdSelectTarget(int id);
void c2s_SendCmdNormalAttack(BYTE byPVPMask);
void c2s_SendCmdReviveVillage();
void c2s_SendCmdReviveItem();
void c2s_SendCmdGoto(float x, float y, float z);
void c2s_SendCmdGetExtProps();
void c2s_SendCmdGetPetProps();
void c2s_SendCmdGetPartExtProps(int iPropIndex);
void c2s_SendCmdLearnSkill(int idSkill);
void c2s_SendCmdLogout(int iOutType, int iTrusteeMode);
void c2s_SendCmdGetAllData(bool bpd, bool bed, bool btd, bool bfd);
void c2s_SendCmdChargeEquipFlySword(int iEleIdx, int iCount);
void c2s_SendCmdChargeFlySword(int iEleIdx, int iFSIdx, int iCount, int idFlySword);
void c2s_SendCmdCancelAction();
void c2s_SendCmdSitDown();
void c2s_SendCmdStandUp();
void c2s_SendCmdEmoteAction(WORD wPose);
void c2s_SendCmdTaskNotify(const void* pData, DWORD dwDataSize);
void c2s_SendCmdUnselect();
void c2s_SendCmdContinueAction();
void c2s_SendCmdStopFall();
void c2s_SendCmdGatherMaterial(int idMatter, int iToolPack, int iToolIdx, int idTool, int idTask);
void c2s_SendCmdCastSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos = NULL);	//Modified 2012-05-03.
void c2s_SendCmdCastInstantSkill(int idSkill, BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos = NULL);	//Modified 2012-05-03.
void c2s_SendCmdCastPosSkill(int idSkill, const A3DVECTOR3& vPos);
void c2s_SendCmdCastApoiseSkill(int idSkill, BYTE bySkillType, int item_id, short item_index, BYTE force_attack, int iNumTarget, int* aTargets);
void c2s_SendCmdCastTransfigureSkill(int idSkill, char skill_type,  BYTE byPVPMask, int iNumTarget, int* aTargets, A3DVECTOR3* pPos = NULL);	//Modified 2012-05-05.
void c2s_SendCmdCastChargeSkill(int idSkill,  BYTE byPVPMask, int idTarget, const A3DVECTOR3& vPos, int iNumTarget, int* aTargets);
void c2s_SendCmdTrickAction(int iAction);
void c2s_SendCmdSetAdvData(int iData1, int iData2);
void c2s_SendCmdClearAdvData();
void c2s_SendCmdQueryPlayerInfo1(int iNum, int* aIDs);
void c2s_SendCmdQueryNPCInfo1(int iNum, int* aIDs);
void c2s_SendCmdSessionEmote(int iAction);
void c2s_SendCmdConEmoteRequest(int iAction, int idTarget);
void c2s_SendCmdConEmoteReply(int iResult, int iAction, int idTarget);
void c2s_SendCmdDeadMove(float y, int iTime, float fSpeed, int iMoveMode);
void c2s_SendCmdDeadStopMove(float y, float fSpeed, BYTE byDir, int iMoveMode);
void c2s_SendCmdEnterSanctuary();
void c2s_SendCmdOpenBooth(int iNumEntry, const void* szName, void* aEntries, int iBoothItemID, int iBoothItemSlot);
void c2s_SendCmdCloseBooth();
void c2s_SendCmdQueryBoothName(int iNum, int* aIDs);
void c2s_SendCmdCompleteTravel();
void c2s_SendCmdEnalbePVP();
void c2s_SendCmdDisablePVP();
void c2s_SendCmdOpenBoothTest(int iBoothItemID, int iBoothItemSlot);
void c2s_SendCmdFashionSwitch();
void c2s_SendCmdEnterInstance(int iTransIdx, int idInst);
void c2s_SendCmdRevivalAgree();
void c2s_SendCmdActiveRushFly(bool bActive);
void c2s_SendCmdQueryDoubleExp();
void c2s_SendCmdDuelRequest(int idTarget);
void c2s_SendCmdDuelReply(int idWho, int iReply);
void c2s_SendCmdBindPlayerRequest(int idTarget);
void c2s_SendCmdBindPlayerInvite(int idTarget);
void c2s_SendCmdBindPlayerRequestRe(int idWho, int iReply);
void c2s_SendCmdBindPlayerInviteRe(int idWho, int iReply);
void c2s_SendCmdCancelBindPlayer();
void c2s_SendCmdMallShopping(int iCount, const void* pGoods);
void c2s_SendCmdMallShopping2(unsigned short id, unsigned short index, unsigned short slot, unsigned short count);
void c2s_SendCmdBonusMallShopping(unsigned short id, unsigned short index, unsigned short slot, unsigned short count);
void c2s_SendCmdZoneMallShopping(unsigned short id, unsigned short index, unsigned short slot, unsigned short count);
void c2s_SendCmdSelectTitle(short title_id);
void c2s_SendCmdDebugDeliverCmd(WORD type, const char* szCmd);
void c2s_SendCmdDebugGSCmd(const char* szCmd);
void c2s_SendCmdLotteryCashing(int item_index);
void c2s_SendCmdCheckRecord(int item_index);
void c2s_SendCmdUseItemWithArg(unsigned char where, unsigned char count, unsigned short index, int item_id, const void* arg, size_t size);
void c2s_SendCmdCancelIceCrust();
void c2s_SendCmdResizeInventory(char type, int newsize, int itemnum, void* item);
// multi bind 
void c2s_SendCmdMultiBindPlayerInvite(int idTarget);
void c2s_SendCmdMultiBindPlayerInviteRe(int idWho, int iReply);
void c2s_SendCmdCancelMultiBindPlayer();
void c2s_SendCmdMultiBindKick(int iPos);

// interact with object
void c2s_SendCmdInteractWithObj(int idTarget, int idTask, int idTool);
void c2s_SendCmdCancelInteract(int idTarget);

// sns 征婚交友平台
void c2s_SendCmdPressSNSMessage(unsigned char msg_type, unsigned char occupation, unsigned char gender, 
								int level, int factionLevel, const ACHAR* szMsg);
void c2s_SendCmdApplySNSMessage(unsigned char msg_type, int msg_id, const ACHAR* szMsg);
void c2s_SendCmdVoteSNSMessage(unsigned char msg_type, int msg_id, char vote_type);
void c2s_SendCmdResponseSNSMessage(unsigned char msg_type, int msg_id, short dst_index, const ACHAR* szMsg);

// Carrier related
void c2s_SendCmdEnterCarrier(int idCarrier, const A3DVECTOR3& vRelPos, unsigned char cRelDir);
void c2s_SendCmdLeaveCarrier(int idCarrier, const A3DVECTOR3& vPos, unsigned char cDir);
void c2s_SendCmdMoveOnCarrier(const A3DVECTOR3& vCurRelPos, const A3DVECTOR3& vNextRelPos, unsigned short wUseTime, float fSpeed, int iMoveMode, unsigned short wCmdSeq);
void c2s_SendCmdStopOnCarrier(const A3DVECTOR3& vRelDest, float fSpeed, int iMoveMode, BYTE byRelDir, WORD wStamp, int iTime);

//	Team commands ...
void c2s_SendCmdTeamInvite(int idPlayer);
void c2s_SendCmdTeamAgreeInvite(int idLeader, int iTeamSeq);
void c2s_SendCmdTeamRejectInvite(int idLeader);
void c2s_SendCmdTeamLeaveParty();
void c2s_SendCmdTeamKickMember(int idMember);
void c2s_SendCmdTeamSetPickupFlag(short sFlag);
void c2s_SendCmdTeamMemberPos(int iNumMem, int* aMemIDs);
void c2s_SendCmdTeamAssistSel(int idTeamMember);
void c2s_SendCmdTeamAskJoin(int idTarget);
void c2s_SendCmdTeamReplyJoinAsk(int idAsker, bool bAgree);
void c2s_SendCmdTeamChangeLeader(int idLeader);
void c2s_SendCmdTeamNotifyOwnPos();

//	Inventory and equipment commands ...
void c2s_SendCmdGetIvtrData(int iPackage);
void c2s_SendCmdGetIvtrDetailData(int iPackage);
void c2s_SendCmdMoveIvtrItem(int iSrc, int iDest, int iAmount);
void c2s_SendCmdExgIvtrItem(int iIndex1, int iIndex2);
void c2s_SendCmdDropIvtrItem(int iIndex, int iAmount);
void c2s_SendCmdDropEquipItem(int iIndex);
void c2s_SendCmdExgEquipItem(int iIndex1, int iIndex2);
void c2s_SendCmdEquipItem(int iIvtrIdx, int iEquipIdx);
void c2s_SendCmdMoveItemToEquip(int iIvtrIdx, int iEquipIdx);
void c2s_SendCmdGetOtherEquip(int iNumID, int* aIDs);
void c2s_SendCmdGetWealth(bool bpd, bool bed, bool btd);
void c2s_SendCmdGetItemInfo(BYTE byPackage, BYTE bySlot);
void c2s_SendCmdPickup(int idItem, int tid);
void c2s_SendCmdUseItem(BYTE byPackage, BYTE bySlot, int tid, BYTE byCount);
void c2s_SendCmdUseItemWithTarget(BYTE byPackage, BYTE bySlot, int tid, BYTE byPVPMask);
void c2s_SendCmdThrowMoney(DWORD dwAmount);
void c2s_SendCmdGetItemInfoList(int iPackage, int iCount, BYTE* aIndices);
void c2s_SendCmdGetTrashBoxData(bool bDetail);
void c2s_SendCmdExgTrashBoxItem(int iIndex1, int iIndex2);
void c2s_SendCmdMoveTrashBoxItem(int iSrc, int iDst, int iAmount);
void c2s_SendCmdExgTrashBoxIvtrItem(int iTrashIdx, int iIvtrIdx);
void c2s_SendCmdMoveTrashBoxToIvtr(int iTrashIdx, int iIvtrIdx, int iAmount);
void c2s_SendCmdMoveIvtrToTrashBox(int iTrashIdx, int iIvtrIdx, int iAmount);
void c2s_SendCmdExgTrashBoxMoney(int iTrashMoney, int iIvtrMoney);
void c2s_SendCmdGetOtherEquipDetail(int idTarget);
void c2s_SendCmdGetCloneEquip(int idMaster, int idClone);

//新增随身包裹
void c2s_SendCmdExgPocketItem(int iIndex1, int iIndex2);
void c2s_SendCmdMovePocketItem(int iSrc, int iDst, int iAmount); 
void c2s_SendCmdExgPocketIvtrItem(int iPocIdx, int iIvtrIdx);
void c2s_SendCmdMovePocketToIvtr(int iPocIdx, int iIvtrIdx, int iAmount);
void c2s_SendCmdMoveIvtrToPocket(int iPocIdx, int iIvtrIdx, int iAmount);
void c2s_SendCmdMovePocketAllToIvtr();
//新增时装包裹
void c2s_SendCmdUpdateFashionHotkey(int iNumEntry, void* aEntries);
void c2s_SendCmdExgFashionItem(int iIndex1, int iIndex2);
void c2s_SendCmdExgIvtrFashionItem(int iIvtrIdx, int iFasIdx);
void c2s_SendCmdExgEquFashionItem(int iEquIdx, int iFasIdx);
void c2s_SendCmdHotkeyChangeFashion(int iKeyIdx, int iHeadIdx, int iBodyIdx, int iShoeIdx);
//新增飞剑、坐骑包裹
void c2s_SendCmdExgMountWingItem(int iIndex1, int iIndex2);
void c2s_SendCmdExgIvtrMountWingItem(int iIvtrIdx, int iMvIdx);
void c2s_SendCmdExgEquMountWingItem(int iEquIdx, int iMvIdx);

//	Pet commands ...
void c2s_SendCmdPetSummon(int iPetIdx);
void c2s_SendCmdPetRecall(int iPetIdx);
void c2s_SendCmdPetBanish(int iPetIdx);

//	Produce
void c2s_SendCmdProduceItem(int iRecipeId);

// 符文2013
void c2s_SendCmdFuWenCompose(bool consume_extra_item, int extra_item_index);
void c2s_SendCmdFuWenUpgrade(int main_fuwen_index, int main_fuwen_where, int assist_count, int* assist_index);
void c2s_SendCmdFuWenInstall(int src_index, int dst_index);
void c2s_SendCmdFuWenUninstall(int fuwen_index, int assist_index, int inv_index);

//	NPC service commands ...
void c2s_SendCmdNPCSevHello(int nid);
void c2s_SendCmdNPCSevBuy(int iItemNum, C2S::npc_trade_item* aItems);
void c2s_SendCmdNPCSevSellBuyBack(char opType, int iItemNum, C2S::npc_trade_item* aItems);
void c2s_SendCmdNPCSevReputationBuy(int iItemNum, C2S::npc_trade_item* aItems);
void c2s_SendCmdNPCSevRepair(BYTE byPackage, BYTE bySlot, int idItem);
void c2s_SendCmdNPCSevRepairAll();
void c2s_SendCmdNPCSevHeal();
void c2s_SendCmdNPCSevTransmit(int iIndex);
void c2s_SendCmdNPCSevEmbed(int item_index, int item_id, int stone_index, int rt_index);
void c2s_SendCmdNPCSevClearEmbeddedChip(int iEquipIdx, int tidEquipvoid, char uninstall_pstone);
void c2s_SendCmdNPCSevLearnSkill(int idSkill);
void c2s_SendCmdNPCSevMakeItem(int id);
void c2s_SendCmdNPCSevResetPKValue(int value);
void c2s_SendCmdNPCSevAcceptTask(int idTask, int idTaskSet);
void c2s_SendCmdNPCSevReturnTask(int idTask, int iChoice);
void c2s_SendCmdNPCSevTaskMatter(int idTask);
void c2s_SendCmdNPCSevChgTrashPsw(const char* szOldPsw, const char* szNewPsw);
void c2s_SendCmdNPCSevOpenTrash(const char* szPsw, bool bFaction);
void c2s_SendCmdNPCSevIdentify(int iSlot, int tidItem);
void c2s_SendCmdNPCSevFaction(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevWarChallenge(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevTravel(int iIndex, int idLine);
void c2s_SendCmdNPCSevGetContent(int idSevice);
void c2s_SendCmdNPCSevBoothBuy(int idBooth, int iItemNum, C2S::npc_booth_item* aItems);
void c2s_SendCmdNPCSevBoothSell(int idBooth, int iItemNum, C2S::npc_booth_item* aItems);
void c2s_SendCmdNPCSevWaypoint();
void c2s_SendCmdNPCSevForgetSkill(int idSkill);
void c2s_SendCmdNPCSevFaceChange(int iIvtrIdx, int idItem);
void c2s_SendCmdNPCSevMail(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevVendue(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevDblExpTime(int iIndex);
void c2s_SendCmdNPCSevHatchPet(int iIvtrIdx, int idEgg);
void c2s_SendCmdNPCSevRestorePet(int iPetIdx);
void c2s_SendCmdNPCSevBattle(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevBuildTower(int iTowerIdx, int idItem);
void c2s_SendCmdNPCSevLeaveBattle();
void c2s_SendCmdNPCSevReturnStatusPt(int iIndex, int idItem);
void c2s_SendCmdNPCSevAccountPoint(int iAction, void* pDataBuf, int iDataSize);
void c2s_SendCmdNPCSevBindItem(int iIndex, int idItem);
void c2s_SendCmdNPCSevDestroyBindItem(int iIndex, int idItem);
void c2s_SendCmdNPCSevCancelDestroy(int iIndex, int idItem);
void c2s_SendCmdNPCSevStockTransaction(int nWithdraw, int nCash, int nMoney);
void c2s_SendCmdNPCSevStockOperation(int nType, void* pData, int nLen);
void c2s_SendCmdNPCSevTalismanRefine(int item_index, int item_id, int material_id);
void c2s_SendCmdNPCSevTalismanUpgrade(int item_index, int item_id);
void c2s_SendCmdNPCSevTalismanRemoveOwner(int item_index, int item_id);
void c2s_SendCmdNPCSevTalismanCombine(int item_index1, int item_id1, int item_index2, int item_id2, int catalyst_id);
void c2s_SendCmdNPCSevTalismanEnchant(int item_index1, int item_type1, int item_index2, int item_type2);
// 法宝飞升
void c2s_SendCmdNPCSevTalismanHoleyLevelUp(int item_index, int item_id, int material_id);
// 法宝技能融合
void c2s_SendCmdNPCSevTalismanSkillRefine(int item_index1, int item_id1, int item_index2, int item_id2, int material_id);
// 法宝技能镶嵌
void c2s_SendCmdNPCSevTalismanSkillEmbed(int item_index1, int item_id1, int item_index2, int item_id2, int material_index1, int material_id1, int material_index2, int material_id2);
void c2s_SendCmdNPCSevRepairDamagedItem(int iIndex, int idItem);
void c2s_SendCmdNPCSevBleedSacrifice(int iIndex, int idItem);                                       //装备血祭
void c2s_SendCmdNPCSevEmbedSoul(int item_index, int item_id, int soul_index, int soul_slot);		//镶嵌器魄		
void c2s_SendCmdNPCSevClearSoulTessera(int item_index, int item_id, int soul_slot);					//移除器魄
void c2s_SendCmdNPCSevRefineSoul(int item_index, int item_id);										//恢复魂力
void c2s_SendCmdNPCSevBreakDownEquip(int item_index, int item_id);									//装备分解
void c2s_SendCmdNPCSevMeldSoul(int recipe_id, int territory_id);
void c2s_SendCmdNPCSevPetEquipEnchant(int item_index, int item_id, int stone_index);
void c2s_SendCmdNPCSevChangeFace(int faceid, int hairid, int earid, int tailid, int fashionid, int stone_index);
void c2s_SendCmdNPCSevRefineChangeCard(int item_index, int item_id, int stone_index, int stone_id, int sev_type);
void c2s_SendCmdNPCSevRecoverCardWakan(int item_index, int item_id);
void c2s_SendCmdNPCSevArenaJoin(int map_id, int item_index);
void c2s_SendCmdNPCSevTerritoryChallenge(int faction_id, int territory_id, int item_id, int item_amount);
void c2s_SendCmdNPCSevTerritoryEnter(int faction_id, int territory_id, int is_assistant);
void c2s_SendCmdNPCSevTerritoryAward(int faction_id, int territory_id, int reward_type, int item_id, int item_count, int money);
void c2s_SendCmdNPCSevTeleportationCharge(int item_index, int item_id, int stone_index, int stone_id);
void c2s_SendCmdNPCSevRepairDamagedItem2(int iIndex, int idItem);
void c2s_SendCmdNPCSevUpgradeEquip(int item_index, int item_id, int stone_index, int rt_index);
void c2s_SendCmdNPCSevOnlineTrade(int type, int item_id, int item_index, int item_count, int money, int sold_time, int price, const ACHAR* szName);
void c2s_SendCmdNPCSevTransmitToCross();
void c2s_SendCmdNPCSevTransmitToNormal();
void c2s_SendCmdNPCSevTransmitServe();	// 2012-8-27 add by zy 
void c2s_SendCmdNPCSevBattleLeaveTransmitServe();	// 2012-9-3 add by zy  从战场离开服务
//Added 2011-03-11.
void c2s_SendCmdNPCSevIdentifyGemSlots( int nItemSlot, int nItemTemplId );//宝石插槽鉴定服务命令
void c2s_SendCmdNPCSevRebuildGemSlots( int nItemSlot, int nItemTemplId, bool bLocked[3] );//宝石插槽重铸服务命令
void c2s_SendCmdNPCSevCustomizeGemSlots( int nItemSlot, int nItemTemplId, int nGemSealSlots[3], int nGemSealTemplId[3] );//宝石插槽定制服务命令
void c2s_SendCmdNPCSevEmbedGems( int nItemSlot, int nItemTemplId, int nGemSlots[3], int nGemTemplId[3] );//宝石镶嵌服务命令
void c2s_SendCmdNPCSevRemoveGems( int nItemSlot, int nItemTemplId, bool bRemove[3] );//宝石拆除服务命令
void c2s_SendCmdNPCSevUpgradeGems( int nGemSlot, int nGemTemplId, int nArticleSlots[12], int nArticleTemplId[12] );//宝石升品服务命令
void c2s_SendCmdNPCSevRefineGems( int nGemSlot, int nGemTemplId, int nArticleSlot, int nArticleTemplId );//宝石精炼服务命令
void c2s_SendCmdNPCSevExtractGems( int nGemSlot, int nGemTemplId );//宝石萃取服务命令
void c2s_SendCmdNPCSevSmeltGems( int nSrcGemSlot, int nSrcGemTemplId, int nDestGemSlot, int nDestGemTemplId, int nArticleSlot, int nArticleTemplId );//宝石熔炼服务命令

// 副本相关C2S服务消息 Added 2011-07-19.
void c2s_SendCmdNPCSevOpenRaidRoom( int iMapId, int iRaidTemplateId, char cVote, const void* szName, char cDiffi=0 ); // 开启副本房间消息协议
void c2s_SendCmdNPCSevJoinRaidRoom( int iMapId, int iRaidTemplateId, int iRoomId ,char cFaction); // 请求报名加入副本房间
void c2s_SendCmdFactionPKJoinRaidRoom(int iMapId,int iRaidTemplateId,int iRoomId,char cFaction);

// 金身法身快速制作
void c2s_SendCmdNPCSevProduceJinFaShen(char type, char produce_idx, int zhaohua[3], short money_item_count, short material_cnt, C2S::npc_produce_jinfashen_material* materials);

void c2s_SendCmdNPCSevPetFly(int iPetIndex, int iAssistIndex);
 
//	GM commands ...
void c2s_SendCmdGMMoveToPlayer(int idPlayer);
void c2s_SendCmdGMCallInPlayer(int idPlayer);
void c2s_SendCmdGMGenerate(int tid);
void c2s_SendCmdGMActiveSpawner(bool bActive, int id_spawner);
void c2s_SendCmdGMGenerateMob(int idMob, int idVis, short sCount, short sLifeTime, const ACHAR* szName);
void c2s_SendCmdGMTriggerInvisible();
void c2s_SendCmdGMTriggerInvincible();
void c2s_SendCmdGMTransferMap(int idMap, float x, float y, float z);

void c2s_SendCmdBotBegin(char flag, char answer_question);
void c2s_SendCmdBotRun();

//	Battle
void c2s_SendCmdBattleGetScore();
void c2s_SendCmdBattleGetInfo();

//  set hide equip mask
void c2s_SendCmdSetFashionMask(int mask);

void c2s_SendCmdStartOnlineAgent();

// Instancing 
void c2s_SendCmdJoinInstancing(int map_id);

// achievement
void c2s_SendCmdQueryOthersAchievement(const unsigned int idPlayer);

void c2s_SendCmdGetAchievementAward(const unsigned int idAchieve, const unsigned int idAward);

// update pvp mask

void c2s_SendCmdUpdatePVPMask(BYTE mask);

// exchange game money to hometown money
void c2s_SendCmdExchgHomeTownMoney(int money);

// get server time
void c2s_SendCmdGetServerTime();

// territory battle
void c2s_SendCmdTerritoryLeave();

void c2s_SendCmdUpdateCombineSkill(int idSkill, int iNumElement, int* aElements);

void c2s_SendCmdUniqueBid( int nBidMoneyLowerLimit, int nBidMoneyUpperLimit  );//Modified 2011-02-16.
void c2s_SendCmdUniqueBidGetAward();
void c2s_SendCmdTaskFlyPos(int idTask, int idNpc);
void c2s_SendCmdTaskReachSiteFly(int idTask);
void c2s_SendCmdActivityFlyPos(int idActivity);
void c2s_SendCmdDeityLevelUp();
// query the circle-of-doom infomation from the gs Added 2011-06-22.
void c2s_SendCmdQueryCodInfo( int nCodSponsorId );
// query the drag skill infomation from the gs Added 2011-08-25.
void c2s_SendCmdQueryBeDraggedInfo( int nBeDraggedPlayerId );
// query the pull skill infomation from the gs Added 2011-08-25.
void c2s_SendCmdQueryBePulledInfo( int nBePulledPlayerId );

void c2s_SendCmdQueryMallSaleTimeItem();

//玩家主动离开副本 Added 2011-07-25.
void c2s_SendCmdLeaveRaid( void );
//玩家请求服务器获得进入副本玩家的相关信息
void c2s_SendCmdGetRaidInfo( void );

//玩家取消拉扯技能 Added 2011-07-28.
void c2s_SendCmdCancelPulling( void );

//向gs查询当前玩家进入某副本的次数 Added 2011-12-02.
void c2s_SendCmdQueryRaidEnterCount( int nMapId );

//向gs请求获得当前可以领取的vip奖励信息 Added 2012-01-04.
void c2s_SendCmdGetVipAwardInfo( void );

//客户端向gs请求领取某一奖励 Added 2012-01-04.
void c2s_SendCmdObtainVipAwardByID( int nAwardID, int nItemID );

//客户端向gs请求验证当前某在线奖励是否能够发放(bTryObtainFlag = true),以及领取某在线奖励(bTryObtainFlag = false)（走同一条消息结构） Added 2012-02-17.
void c2s_SendCmdObtainOnlineAward( int nAwardID, int nAwardIndex, int nSmallBagID, bool bTryObtainFlag = false );

void c2s_SendFiveAnniRequest(char type);

//客户端向gs请求升级挖宝区域
void c2s_SendCmdUpgradeTreasureRegion( int nItemID, int nItemSlot, int nRegionID, int nRegionPos );

//客户端向gs请求解锁挖宝区域
void c2s_SendCmdUnlockTreasureRegion( int nItemID, int nItemSlot, int nRegionID, int nRegionPos );

//客户端向gs请求探索挖宝区域
void c2s_SendCmdDigTreasureRegion( int nRegionID, int nRegionPos );

//客户端点击哪张牌，掉线后服务器发回来显示
void c2s_SendCmdStartRandomTowerMonster( char index );

// 领养诛小仙
void c2s_SendCmdAdoptLittlePet();

// 喂养诛小仙
void c2s_SendCmdFeedLittlePet(int nItemIndex, int nItemId);

// 获取诛小仙奖励
void c2s_SendCmdGetLittlePetAward();

// 元魂鉴定
void c2s_SendCmdRuneIdentify(int nRuneIndex, int nAssistIndex);

// 元魂合成
void c2s_SendCmdRuneCombine(int nRuneIndex, int nAssistIndex);

// 元魂洗练
void c2s_SendCmdRuneRefine(int nRuneIndex, int nAssistIndex);

// 元魂归元
void c2s_SendCmdRuneReset(int nRuneIndex, int nAssistIndex);

// 元魂分解
void c2s_SendCmdRuneDecompose(int nRuneIndex);

// 元魂升级
void c2s_SendCmdRuneLevelUp(int nRuneIndex);

// 元魂孔位改变
void c2s_SendCmdRuneChangeSlot(int nRuneIndex, int nAssistIndex);

// 元魂擦写符语
void c2s_SendCmdRuneEraseSlot(int nRuneIndex, int nStoneIndex);

// 领取爬塔副本奖励
void c2s_SendCmdTowerReward(int nTmpId, int nLevel, int type);

// 领取特殊任务奖励
void c2s_SendCmdGetTaskAward(int type);

// 镶嵌符文
void c2s_SendCmdInstallSlot(int nRuneIndex, int nSlotIndex, int nStoneIndex, int nStoneId);

// 元魂洗练是否接收结果
void c2s_SendCmdRefineAction(bool bAccept, int nIndex);

// 90级前免费洗技能天书
void c2s_SendCmdResetSkillPropFree(unsigned char opcode);

// 星座鉴定
void c2s_SendCmdAstrologyIdentify(int item_id, int item_index); 

// 星座升级
void c2s_SendCmdAstrologyUpgrade(int item_id, int item_index, int stone_id, int stone_index); 

// 星座粉碎
void c2s_SendCmdAstrologyDestory(int item_id, int item_index);  

// 首次登陆不足1小时下线原因
void c2s_SendCmdPlayerFirstExitReason(unsigned char reason);

// 基础属性增益丹药合成
void c2s_SendCmdGenPropAddItem(int material_id, short material_index, short material_count);

// 基础属性增益丹药重炼
void c2s_SendCmdRebuildPropAddItem(int item_id, short item_index, int forge_id, short forge_index);

// 获取基础属性增益
void c2s_SendCmdGetPropAdd();

// 跨服pk竞猜
void c2s_SendCmdCancelTop3Guess(int area);
void c2s_SendCmdPK1stGuessReward();
void c2s_SendCmdPK1stGuessResultReward();
void c2s_SendCmdPKTop3GuessReward();
void c2s_SendCmdPKTop3GuessResultReward(char type);
void c2s_SendCmdGetPlayerBetData();		// 获取pk数据

// 特殊移动相关命令
void c2s_SendCmdStartSpecialMove( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& vCurPos, char collisionState = 0 );
void c2s_SendCmdSpecialMove( const A3DVECTOR3& velocity, const A3DVECTOR3& acceleration, const A3DVECTOR3& curPos, unsigned short useTime, unsigned short stamp, char collisionState = 0 );
void c2s_SendCmdStopSpecialMove( unsigned char curDir, const A3DVECTOR3& vCurPos, unsigned short stamp );

// 6v6碰撞副本报名
void c2s_SendCmd6v6CollisionApply(int mapId, int tmplId, bool isTeam,bool isCross = false);
void c2s_SendCmdGetRePurchaseInv();

// 设置是否让别的玩家看到自己vip等级
void c2s_SendCmdHideVipLevel(char cHide);

//	通知服务器，更新当前飞剑颜色
void c2s_SendCmdChangeWingColor( unsigned char cColorIndex );

// 法宝技能融合是否接受
void c2s_SendCmdTalismanSkillRefineResult(int item_index1, int item_id1, char result);

// 碰撞副本积分兑换
void c2s_SendCmdGetCollisionAward(bool daily, int index);

// 发送合并药剂	2012-10-15 by zy
void c2s_SendCmdMergeMedicine(int bottle_id,int bottle_index,int potion_id,int potion_index);

// 发送弥补飞升数据物品 2012-10-17 by zy
void c2s_SendCmdRemedyMetempsyChosisLevel(int nItemIndex);

void c2s_SendCmdGetCollisionPlayerPos(int nPlayerId);

// 发送活跃度数据物品 2012-10-17 by zy
void c2s_SendCmdLiveness(int nItemIndex);

//退出国战
void c2s_SendCmdKingdomLeave();
//亲卫传送
void c2s_SendCmdKingTryCallGuard();
//搓澡邀请
void c2s_SendCmdKingBathInvite(int who);
//搓澡邀请回复
void c2s_SendCmdKingBathInviteReply(int who,int param);
//发布国王任务
void c2s_SendCmdDeliverKingTask(int type);
//接受国王任务
void c2s_SendCmdReceiveKingTask(int type);
//传到国战NPC位置
void c2s_SendCmdKingFlyNPCPos();
//领取连任奖励
void c2s_SendCmdKingGetReward();

// 领取上古副本关卡的奖励
void c2s_SendCmdAncientLevelAward(int level);

// 请求上古副本各关卡是否通关、发放奖励信息
void c2s_SendCmdAncientLevelResult();

// 打开远程仓库
void c2s_CmdOpenTrashBox(const char* szPsw);

// 彩票送的礼包实际兑换
void c2s_SendCmdGiftLotteryDeliver(int index, int id);

void c2s_SendCmdGetCashGiftAward();

/// 请求离开流水席
void c2s_SendCmdFlowBattleLeave();


/// 请求流水席报名
void c2s_SendCmdFlowBattleApply();

// 整理包裹
void c2s_SendCmdArrangeInventory(char page);

//整理仓库 
void c2s_SendCmdArrangeTrashBox(char page);

//领取WEB商城礼包
void c2s_SendCmdGetWebOrder(__int64 order_id);

//定制元婴
void c2s_SendCmdCustomsizeRune(int type, int count, void *prop);

//引爆陷阱
void c2s_SendCmdControltrap(int id, int tid);

//怪远程传送
void c2s_SendCmdSummonTeleport(int npc_id);

// 请求战场信息 
void c2s_SendLookBattleInfo(int mapid);

//麒麟邀请
void c2s_SendCmdQilinInvite(int who);
//麒麟邀请回复
void c2s_SendCmdQilinInviteReply(int who,int param);

//取消麒麟绑定状态
void c2s_SendCmdCancelQilinBinding();
//上麒麟
void c2s_SendCmdQilinUp();
//下麒麟
void c2s_SendCmdQilinDown();

void c2s_SendCmdPuppetFormChange();

//互动表情
void c2s_SendCmdInviteActiveEmote(int playerid, int type);
void c2s_SendCmdReplyActiveEmote(int playerid, int type, int answer);
void c2s_SendCmdStopActiveEmote();

void c2s_SendCmdZaibian();

// 创建战队
void c2s_SendCreateCrossvrTeam(const ACHAR* szName);


// 请求合并颜色 @ 背包位置
void c2s_SendFashionColorantCombine(int nPos1,int nPos2,int nPos3);
// 请求合并 @1 背包类型 @2背包位置, @3 背包位置
void c2s_SendChangeFashionColor(int nPackType, int nPackPos,int nPos);

// 请求进入帮派基地
void c2s_SendEndter_Faction_Base();

// 创建帮派基地建筑
void c2s_SendAddFacBuild(int fid,int field_index,int building_tid);
// 升级帮派基地建筑
void c2s_SendUpGradeFacBuild(int fid,int field_index,int building_tid,int nlevel);
// 删除帮派基地建筑
void c2s_SendRemoveFacBuild(int fid,int field_index,int building_tid);
// 在帮派基地请求帮派信息
void c2s_SendCmdFacBaseInfo();
// 请求6v6奖励物品
void c2s_SendCmdCS_6v6Award(int nType, int nId);
// 请求帮派商城信息
void c2s_SendCmdMallInfo();
// 商城购物
void c2s_SendCmdShopMallInfo(int item_index,int item_id,int cnt);
//帮派捐献
void c2s_SendCmdFacbaseMoney(int nMoney);
// 元宝商城 物品index 到映射时间冷却关系
void c2s_SendCmdFacbaseMoneyItemInfo();
// 申请领奖兑换
void c2s_SendCmdCS_6v6_ExchangeAward(int index,int item_id,int currency_id);
// 帮派基地留言
void c2s_SendCmdPostFacBaseMsg(int nFacId,const ACHAR* szMsg);
// 帮派传送
void c2s_SendCmdFacBaseTransfer(int nIndex);
// 6v6 兑换币兑换
void c2s_SendCmdExchangeCs6v6Money(int nType, int nCount);
// 帮派基地竞拍
void c2s_SendCmdBigOnFacbase(int item_index,int item_id,int coupon,const ACHAR* szName);
/// 元宝商城购买物品
void c2s_SendCmdBuyFacbaseCashItem(int item_index);
// 离开帮派基地
void c2s_SendCmdLevelFaction();
// 不在帮派基地发送请求消息 
void c2s_SendCmdOueSideFacBaseInfo();
// 发送请求帮派基地拍卖信息
void c2s_SendCmdGetFacBaseAuction();
// 发送请求返回帮派金劵
void c2s_SendCmdWithdrawFacBaseCoupon();
// 发送请求返回帮派拍卖物品
void c2s_SendCmdWithdrawFacBaseAuction(int nItemIndex);

void c2s_SendCmdCharegePos(int type, int target_id, A3DVECTOR3& vCurPos);

void c2s_SendCmdStopFreeFly();
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////
