 /*
 * FILE: EC_GameUIManDlgs.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Tom Zhou, 2005/8/27
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_GameUIMan2.inl"
#include "AUI/AStringWithWildcard.h"



#define INIT_DIALOG_POINTER(variableName, className, dialogName) \
{ \
	variableName = dynamic_cast<className *>(GetDialog(dialogName)); \
	ASSERT(variableName); \
}

void CECGameUIMan::InitDialogs()
{
	INIT_DIALOG_POINTER(m_pDlgAction, CDlgAction, "Win_Action");
	INIT_DIALOG_POINTER(m_pDlgArrangeTeam, CDlgArrangeTeam, "Win_ArrangeTeam");
	INIT_DIALOG_POINTER(m_pDlgAskHelpToGM, CDlgAskHelpToGM, "Win_InputString6");
// 	INIT_DIALOG_POINTER(m_pDlgAuctionBuyList, CDlgAuctionBuyList, "Win_AuctionBuyList");
// 	INIT_DIALOG_POINTER(m_pDlgAuctionList, CDlgAuctionList, "Win_AuctionList");
// 	INIT_DIALOG_POINTER(m_pDlgAuctionSellList, CDlgAuctionSellList, "Win_AuctionSellList");
// 	INIT_DIALOG_POINTER(m_pDlgAuctionSearch, CDlgAuctionSearch, "Win_AuctionSearch");
	INIT_DIALOG_POINTER(m_pDlgAutoLock, CDlgAutoLock, "Win_AutoLock");
//	INIT_DIALOG_POINTER(m_pDlgBBS, CDlgBBS, "Win_BBS");
	INIT_DIALOG_POINTER(m_pDlgBuddyState, CDlgBuddyState, "Win_BuddyState");
	INIT_DIALOG_POINTER(m_pDlgBuff, CDlgBuff, "Win_Buff");
	INIT_DIALOG_POINTER(m_pDlgBooth1, CDlgBooth, "Win_PShop1");
	INIT_DIALOG_POINTER(m_pDlgBooth2, CDlgBooth, "Win_PShop2");
	INIT_DIALOG_POINTER(m_pDlgBook, CDlgBookBase, "Win_Book");
	INIT_DIALOG_POINTER(m_pDlgContest, CDlgContest, "Win_Contest");
	INIT_DIALOG_POINTER(m_pDlgGoldAccount, CDlgGoldAccount, "Win_GoldAccount");
	INIT_DIALOG_POINTER(m_pDlgGoldInquire, CDlgGoldInquire, "Win_GoldInquire");
	INIT_DIALOG_POINTER(m_pDlgGoldTrade, CDlgGoldTrade, "Win_GoldTrade");
	INIT_DIALOG_POINTER(m_pDlgGoldPwdChange, CDlgGoldPassword, "Win_GoldPwdChange");
	INIT_DIALOG_POINTER(m_pDlgGoldPwdInput, CDlgGoldPassword, "Win_GoldPwdInput");
	INIT_DIALOG_POINTER(m_pDlgFriendSeek, CDlgFriendSeek, "Win_FriendSeeking");
	INIT_DIALOG_POINTER(m_pDlgFriendInfoPub, CDlgFriendSeek, "Win_FriendInfoPub");

	INIT_DIALOG_POINTER(m_pDlgChatNormal, CDlgChat, "Win_Chat");
	m_pDlgChat = m_pDlgChatNormal;
	INIT_DIALOG_POINTER(m_pDlgChatPk, CDlgChat, "Win_KfpkChat");
	INIT_DIALOG_POINTER(m_pDlgChat1, CDlgChat, "Win_ChatInfo");
	INIT_DIALOG_POINTER(m_pDlgChat2, CDlgChat, "Win_ChatInfo2");
//	INIT_DIALOG_POINTER(m_pDlgChatChoose, CDlgChat, "Win_WhisperChoose");
	INIT_DIALOG_POINTER(m_pDlgChatWhisper, CDlgChat, "Win_WhisperChat");
	INIT_DIALOG_POINTER(m_pDlgCamera, CDlgCamera, "Win_Camera");
	INIT_DIALOG_POINTER(m_pDlgCharacter, CDlgCharacter, "Win_Character");
	INIT_DIALOG_POINTER(m_pDlgChannelCreate, CDlgChannelCreate, "Win_ChannelCreate");
	INIT_DIALOG_POINTER(m_pDlgChannelJoin, CDlgChannelJoin, "Win_ChannelJoin");
	
	INIT_DIALOG_POINTER(m_pDlgChannelChat, CDlgChannelChat, "Win_ChannelChat");

	INIT_DIALOG_POINTER(m_pDlgChannelOption, CDlgChannelOption, "ChannelOption");
	INIT_DIALOG_POINTER(m_pDlgChannelPW, CDlgChannelPW, "Win_InputString9");
	INIT_DIALOG_POINTER(m_pDlgDragDrop, CDlgDragDrop, "DragDrop");
	INIT_DIALOG_POINTER(m_pDlgDoubleExp, CDlgDoubleExp, "Win_DoubleExp");
	INIT_DIALOG_POINTER(m_pDlgDoubleExpS, CDlgDoubleExpS, "Win_DoubleExpS");

	INIT_DIALOG_POINTER(m_pDlgFriendOptionNormal, CDlgFriendOptionNormal, "FriendOptionNormal");
	INIT_DIALOG_POINTER(m_pDlgEnemyOptionName, CDlgEnemyOptionName, "EnemyOptionName");
 	INIT_DIALOG_POINTER(m_pDlgGMConsole, CDlgGMConsole, "Win_GMConsole");
 	INIT_DIALOG_POINTER(m_pDlgGMDelMsgReason, CDlgGMDelMsgReason, "Win_GMDelMsgReason");
 	INIT_DIALOG_POINTER(m_pDlgGMFinishMsg, CDlgGMFinishMsg, "Win_GMFinishMsg");
	INIT_DIALOG_POINTER(m_pDlgEPEquip, CDlgEPEquip, "Win_EPEquip");
	INIT_DIALOG_POINTER(m_pDlgExit, CDlgExit, "Win_Message2");
	INIT_DIALOG_POINTER(m_pDlgGameQuit, CDlgExit, "Win_Message7");

	INIT_DIALOG_POINTER(m_pDlgEquipBind, CDlgEquipBind, "Win_EquipBind");
	INIT_DIALOG_POINTER(m_pDlgEquipDestroy, CDlgEquipDestroy, "Win_EquipDestroy");
	INIT_DIALOG_POINTER(m_pDlgEquipUndestroy, CDlgEquipUndestroy, "Win_EquipUndestroy");
//	INIT_DIALOG_POINTER(m_pDlgFaceLift, CDlgFaceLift, "Win_Cosmetic");
	INIT_DIALOG_POINTER(m_pDlgFriendColor, CDlgFriendColor, "Win_FriendColor");
	INIT_DIALOG_POINTER(m_pDlgFriendGroup, CDlgFriendGroup, "Win_FriendGroup");
	INIT_DIALOG_POINTER(m_pDlgFriendHistory, CDlgFriendHistory, "Win_FriendHistory");
	INIT_DIALOG_POINTER(m_pDlgFriendList, CDlgFriendList, "Win_FriendList");
	INIT_DIALOG_POINTER(m_pDlgFriendOptionGroup, CDlgFriendOptionGroup, "FriendOptionGroup");
	INIT_DIALOG_POINTER(m_pDlgFriendOptionName, CDlgFriendOptionName, "FriendOptionName");
	INIT_DIALOG_POINTER(m_pDlgFrdCallBck, CDlgFrdCallBck, "Win_MailtoFriend");
	INIT_DIALOG_POINTER(m_pDlgGMParam, CDlgGMParam, "Win_GMParam");
	INIT_DIALOG_POINTER(m_pDlgFamilyCreate, CDlgGuildCreate, "Win_FamilyCreate");
	INIT_DIALOG_POINTER(m_pDlgGuildCreate, CDlgGuildCreate, "Win_GuildCreate");
	INIT_DIALOG_POINTER(m_pDlgGuildMan, CDlgGuildMan, "Win_GuildManage");
	INIT_DIALOG_POINTER(m_pDlgFamilyMan, CDlgGuildMan, "Win_FamilyManage");
//	INIT_DIALOG_POINTER(m_pDlgGuildMap, CDlgGuildMap, "GuildMap");
// 	INIT_DIALOG_POINTER(m_pDlgGMapStatus1, CDlgGuildMapStatus, "Win_GMapStatus1");
// 	INIT_DIALOG_POINTER(m_pDlgGMapStatus2, CDlgGuildMapStatus, "Win_GMapStatus2");
// 	INIT_DIALOG_POINTER(m_pDlgGMapChallenge, CDlgGuildMapStatus, "Win_GMapChallenge");
// 	INIT_DIALOG_POINTER(m_pDlgGMapTravel, CDlgGuildMapTravel, "Win_GMapTravel");
	INIT_DIALOG_POINTER(m_pDlgHelp, CDlgHelp, "Win_Help_Cyclo");
	INIT_DIALOG_POINTER(m_pDlgHorseTame, CDlgHorseTame, "Win_HorseTame");
	INIT_DIALOG_POINTER(m_pDlgHost, CDlgHost, "Win_CharHead");
	INIT_DIALOG_POINTER(m_pDlgHostPet, CDlgHostPet, "Win_PetHead");
//	INIT_DIALOG_POINTER(m_pDlgIdentify, CDlgIdentify, "Win_Identify");
	INIT_DIALOG_POINTER(m_pDlgInfo, CDlgInfo, "Win_Poplist");
	INIT_DIALOG_POINTER(m_pDlgInfoIcon, CDlgInfo, "Win_Pop");

	INIT_DIALOG_POINTER(m_pDlgInventory, CDlgInventory, "Win_Inventory");
	INIT_DIALOG_POINTER(m_pDlgInventoryPocket, CDlgInventory, "Win_InventoryPet");

	INIT_DIALOG_POINTER(m_pDlgInputName, CDlgInputName, "Win_InputString5");
	INIT_DIALOG_POINTER(m_pDlgInputNO, CDlgInputNO, "Win_InputNO");
	INIT_DIALOG_POINTER(m_pDlgInputGNO, CDlgInputNO, "Win_InputGNO");
	INIT_DIALOG_POINTER(m_pDlgInputMoney, CDlgInputNO, "Win_InputMoney");
	INIT_DIALOG_POINTER(m_pDlgInputMoney2, CDlgInputNO, "Win_InputMoney2");
	INIT_DIALOG_POINTER(m_pDlgInstall, CDlgInstall, "Win_Enchase");
	INIT_DIALOG_POINTER(m_pDlgUninstall1, CDlgInstall, "Win_Disenchase1");
	INIT_DIALOG_POINTER(m_pDlgUninstall2, CDlgInstall, "Win_Disenchase2");
	INIT_DIALOG_POINTER(m_pDlgLottery, CDlgLottery, "Win_Lottery");
	INIT_DIALOG_POINTER(m_pDlgMailList, CDlgMailList, "Win_MailList");
	INIT_DIALOG_POINTER(m_pDlgMailOption, CDlgMailOption, "MailOption");
	INIT_DIALOG_POINTER(m_pDlgMailRead, CDlgMailRead, "Win_MailRead");
	INIT_DIALOG_POINTER(m_pDlgMailWrite, CDlgMailWrite, "Win_MailWrite");
	INIT_DIALOG_POINTER(m_pDlgMidMap, CDlgMidMap, "Win_MidMap");
	INIT_DIALOG_POINTER(m_pDlgMiniMap, CDlgMiniMap, "Win_MiniMap");
	INIT_DIALOG_POINTER(m_pDlgMiniMapMark, CDlgMiniMap, "Win_Mark");
	INIT_DIALOG_POINTER(m_pDlgNPC, CDlgNPC, "Win_NPC");
	INIT_DIALOG_POINTER(m_pDlgNpcMain, CDlgNPC, "Win_NPC_Main");
	INIT_DIALOG_POINTER(m_pDlgMapTransfer, CDlgMapTransfer, "Win_Map_Transfer");
	INIT_DIALOG_POINTER(m_pDlgPetSkill, CDlgPetAction, "Win_Pet_Skill");
//	INIT_DIALOG_POINTER(m_pDlgPetAttackSetting, CDlgPetAction, "Win_Pet_AttackSetting");
	INIT_DIALOG_POINTER(m_pDlgPetDetail, CDlgPetDetail, "Win_PetDetail");
	INIT_DIALOG_POINTER(m_pDlgPetEquip, CDlgPetEquip, "Win_PetEquip");
	INIT_DIALOG_POINTER(m_pDlgPetFactory, CDlgPetFactory, "Win_PetFactory");
	INIT_DIALOG_POINTER(m_pDlgSoulCombine, CDlgSoulCombine, "Win_SoulCombine");
	INIT_DIALOG_POINTER(m_pDlgPetPackage, CDlgPetPackage, "Win_PetPackage");
	INIT_DIALOG_POINTER(m_pDlgPKSetting, CDlgPKSetting, "Win_PKSetting");
	INIT_DIALOG_POINTER(m_pDlgProduce, CDlgProduce, "Win_Produce");
	INIT_DIALOG_POINTER(m_pDlgProclaim, CDlgProclaim, "Win_InputString8");
	INIT_DIALOG_POINTER(m_pDlgQuestion, CDlgQuestion, "Win_Question");
	INIT_DIALOG_POINTER(m_pDlgQuickAction, CDlgQuickAction, "Win_QuickAction");
//	INIT_DIALOG_POINTER(m_pDlgQuickBar10, CDlgQuickBar, "Win_QuickBar10V_1");
	INIT_DIALOG_POINTER(m_pDlgQuickBar8, CDlgQuickBar, "Win_QuickBar8H_1");
	INIT_DIALOG_POINTER(m_pDlgQShop, CDlgQShop, "Win_QShop");
	INIT_DIALOG_POINTER(m_pDlgRankList, CDlgRankList, "Win_RankList");
	INIT_DIALOG_POINTER(m_pDlgRecord, CDlgRecord, "Win_Record");
	INIT_DIALOG_POINTER(m_pDlgRecordEnd, CDlgRecord, "Win_RecordEnd");
	INIT_DIALOG_POINTER(m_pDlgReplay, CDlgReplay, "Win_Replay");
	INIT_DIALOG_POINTER(m_pDlgReplayEdit, CDlgReplayEdit, "Win_ReplayEdit");
	INIT_DIALOG_POINTER(m_pDlgReportToGM, CDlgReportToGM, "Win_InputString7");
	INIT_DIALOG_POINTER(m_pDlgResetSkillPart, CDlgResetProp, "Win_ResetSkillPart");
	INIT_DIALOG_POINTER(m_pDlgResetSkillAll, CDlgResetProp, "Win_ResetSkillAll");
	INIT_DIALOG_POINTER(m_pDlgResetTalent, CDlgResetProp, "Win_ResetTalent");
	INIT_DIALOG_POINTER(m_pDlgRevive, CDlgRevive, "Win_Message4");
	INIT_DIALOG_POINTER(m_pDlgSettingVideo, CDlgSettingVideo, "Win_SettingVideo");
	INIT_DIALOG_POINTER(m_pDlgSettingGame, CDlgSettingGame, "Win_SettingGame");
	INIT_DIALOG_POINTER(m_pDlgChatSetting, CDlgSettingGame, "Win_Chat_Setting");
	INIT_DIALOG_POINTER(m_pDlgSettingSystem, CDlgSettingSystem, "Win_SettingSystem");
	INIT_DIALOG_POINTER(m_pDlgShop, CDlgShop, "Win_Shop");
	INIT_DIALOG_POINTER(m_pDlgShopExchange, CDlgShopExchange, "Win_ShopExchange");
	INIT_DIALOG_POINTER(m_pDlgShopReputation, CDlgShopReputation, "Win_Shop_Credit");	
	INIT_DIALOG_POINTER(m_pDlgSkillMan, CDlgSkillMan, "Win_Skillzx2");
	INIT_DIALOG_POINTER(m_pDlgSkill, CDlgSkill, "Win_Skill");
	INIT_DIALOG_POINTER(m_pDlgSkillOther, CDlgSkill, "Win_SkillOther");
	INIT_DIALOG_POINTER(m_pDlgSkillSpec, CDlgSkill, "Win_SkillSpec");
	INIT_DIALOG_POINTER(m_pDlgSkillEdit, CDlgSkillEdit, "Win_SkillContinual");
//	INIT_DIALOG_POINTER(m_pDlgSplit, CDlgSplit, "Win_Split");
	INIT_DIALOG_POINTER(m_pDlgStorage, CDlgStorage, "Win_Storage");
	INIT_DIALOG_POINTER(m_pDlgStoragePW, CDlgStoragePW, "Win_InputString");
	INIT_DIALOG_POINTER(m_pDlgStorageChangePW, CDlgStorageChangePW, "Win_InputString3");
	INIT_DIALOG_POINTER(m_pDlgSystem, CDlgSystem, "Win_Main");
	INIT_DIALOG_POINTER(m_pDlgSystem2, CDlgSystem2, "Win_Main2");
	INIT_DIALOG_POINTER(m_pDlgSystem3, CDlgSystem3, "Win_Main3");
	INIT_DIALOG_POINTER(m_pDlgSystem4, CDlgSystem4, "Win_Main4");
	INIT_DIALOG_POINTER(m_pDlgTalisman, CDlgTalisman, "Win_Talisman");
	INIT_DIALOG_POINTER(m_pDlgTalismanClean, CDlgTalismanService, "Win_TalismanClean");
	INIT_DIALOG_POINTER(m_pDlgTalismanCombine, CDlgTalismanService, "Win_TalismanCombine");
	INIT_DIALOG_POINTER(m_pDlgTalismanRefine, CDlgTalismanService, "Win_TalismanRefine");
	INIT_DIALOG_POINTER(m_pDlgTalismanUpgrade, CDlgTalismanService, "Win_TalismanUpgrade");
	INIT_DIALOG_POINTER(m_pDlgTalismanEnchanting, CDlgTalismanService, "Win_TalismanEnchanting");
	
	INIT_DIALOG_POINTER(m_pDlgMend, CDlgMend, "Win_Mend");
	INIT_DIALOG_POINTER(m_pDlgTask, CDlgTask, "Win_Quest");
//	INIT_DIALOG_POINTER(m_pDlgTaskList, CDlgTaskList, "Win_QuestList");
	INIT_DIALOG_POINTER(m_pDlgTaskTrace, CDlgTask, "Win_QuestMinion");
	INIT_DIALOG_POINTER(m_pDlgTaskDaily, CDlgTaskDaily, "Win_TaskDaily");
//	INIT_DIALOG_POINTER(m_pDlgTeach, CDlgTeach, "Win_Teach");
	INIT_DIALOG_POINTER(m_pDlgTeacherMan, CDlgTeacherMan, "Win_TeacherManage");
	INIT_DIALOG_POINTER(m_pDlgTeamMe1, CDlgTeamContext, "GroupMe1");
	INIT_DIALOG_POINTER(m_pDlgTeamMe2, CDlgTeamContext, "GroupMe2");
	INIT_DIALOG_POINTER(m_pDlgTeamOther1, CDlgTeamContext, "GroupOther1");
	INIT_DIALOG_POINTER(m_pDlgTeamOther2, CDlgTeamContext, "GroupOther2");
	INIT_DIALOG_POINTER(m_pDlgTeamMain, CDlgTeamMain, "Win_TeamMain");
	INIT_DIALOG_POINTER(m_pDlgTextFirework, CDlgTextFirework, "Win_TextFirework");
	INIT_DIALOG_POINTER(m_pDlgTrade, CDlgTrade, "Win_Trade");
	INIT_DIALOG_POINTER(m_pDlgWorldMap, CDlgWorldMap, "Win_WorldMap");
	INIT_DIALOG_POINTER(m_pDlgScriptHelp, CDlgScriptHelp, "Win_ScriptHelp");
	INIT_DIALOG_POINTER(m_pDlgSettingScriptHelp, CDlgSettingScriptHelp, "Win_ScriptHelpSetting");
	INIT_DIALOG_POINTER(m_pDlgScriptHelpHistory, CDlgScriptHelpHistory, "Win_HelpHistory");
	INIT_DIALOG_POINTER(m_pDlgBroadCast, CDlgBroadCast, "Win_Broadcast");
	INIT_DIALOG_POINTER(m_pDlgFengshenAttach, CDlgFengshenAttach, "Win_Equip_Fengshen_Attach");
	INIT_DIALOG_POINTER(m_pDlgFengshenConvert, CDlgFengshenConvert, "Win_Equip_Fengshen_Convert");
	INIT_DIALOG_POINTER(m_pDlgFengshenDrill, CDlgFengshenDrill, "Win_Equip_Fengshen_Drill");
	INIT_DIALOG_POINTER(m_pDlgFengshenErase, CDlgFengshenErase, "Win_Equip_Fengshen_Erase");
	INIT_DIALOG_POINTER(m_pDlgFengshenIdentify, CDlgFengshenIdentify, "Win_Equip_Fengshen_Identify");
	INIT_DIALOG_POINTER(m_pDlgFengshenUpgrade, CDlgFengshenUpgrade, "Win_Equip_Fengshen_Upgrade");
	INIT_DIALOG_POINTER(m_pDlgTaskAncient, CDlgTaskAncient, "Win_Quest_Wanted");
	INIT_DIALOG_POINTER(m_pDlgDungeonAncientMini, CDlgAncientMini, "Win_Dungeon_Ling_Mini");
	INIT_DIALOG_POINTER(m_pDlgAncientLevelInfo, CDlgAncientLevelInfo, "Win_Dungeon_Ling");	
	INIT_DIALOG_POINTER(m_pDlgAncientTime, CDlgAncientTime, "Win_Battle_Dungeon_Level_Time");	

	for (abase::vector<PAUIDIALOG>::iterator it=m_aDialog.begin(); it!=m_aDialog.end(); ++it)
	{
		if ((*it)->IsOptimizeResourceLoad() && !(*it)->IsShow())
		{
			(*it)->OptimizeSpriteResource(false);
		}
	}
}

#define CREATE_DIALOG_FROM_TEMPLATE(className, templateName) \
{ \
	if (IsMatch(strTemplName, templateName)) \
	{\
		PAUIDIALOG pDlg = new className;\
		pDlg->SetOptimizeResourceLoad(true);\
		return  pDlg;\
	}\
}

PAUIDIALOG CECGameUIMan::CreateDlgInstance(const AString strTemplName)
{
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillXP,       "Skill_Xp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRoleRune,      "Equip_Rune.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRoleRuneCombine,       "Equip_Rune_Combine.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRoleRuneEnchanting,       "Equip_Rune_Enchanting.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRoleRuneEnchantingExp,    "Equip_RuneEnchanting_Exp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDynamicMin,   "Activity_Min.xml");	//2012-11-6 by add zy 活跃度
	CREATE_DIALOG_FROM_TEMPLATE(DlgDynamicSystem,   "Activity.xml");	//2012-11-6 by add zy 活跃度
	CREATE_DIALOG_FROM_TEMPLATE(DlgDynamicContent,   "Activity_Daily.xml");	//2012-11-6 by add zy 活跃度
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerMin,   "Rose_Min.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(DlgFlowers,   "Rose.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(DlgFlowersList,   "Roserank.xml");	//2013-2-27 by add zy 鲜花

	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerRankaward,   "Roserankaward.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerPopup,   "RosePop01.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerAnounce,   "RoseAnounce.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerManifesto,   "Rosepopup.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerRankOption,   "RoseRankOption.xml");	//2013-2-27 by add zy 鲜花
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlowerGfx,   "Rose_Gfx.xml");	//2013-2-27 by add zy 鲜花

	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsMin,		"Battle_Dungeon_6v6_Min_Expand.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsCreateInput,	"Battle_Dungeon_6v6_InputGroupName.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCrssvrTeamsOptionName,	"Battle_Dungeon_6v6_Option.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsGroupInfo,	"Battle_Dungeon_6v6_GroupInfo.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsGroupRank,	"Battle_Dungeon_6v6_GroupRank.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsImg,	"Battle_Dungeon_6v6_GroupRank_Member.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsItem,	"Battle_Dungeon_6v6_GroupRank_SubDetail.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsSub,	"Battle_Dungeon_6v6_GroupRankSub.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsApply,	"Battle_Dungeon_6v6_GroupList.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsMain,		"Battle_Dungeon_6v6_GroupMain.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsAward_Week,	"Battle_Dungeon_6v6_GroupAward01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsAward_Exchange,	"Battle_Dungeon_6v6_GroupAward02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgCrssvrTeamsAward_Stage,		"Battle_Dungeon_6v6_GroupAward03.xml");

	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildInfo,		"Guild_Info.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildInfoSubItem,		"Guild_InfoS.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildInfoSubItemExpand,		"Guild_InfoL.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildInfoSub,		"Guild_Sub.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildLandCreate,		"Guild_LandCreate.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildBuild,		"Guild_Build.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildInputMoney,		"Guild_DonateInput.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildCreateLandBuild,		"Guild_LandBuild_List.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildBBS,		"Guild_Bbs.xml");

	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildShop,		"Guild_Shop.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildQShop,		"Guild_Qshop.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildQShopSub,		"Guild_Qshop_Sub.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildShopSub,		"Guild_shop_Sub.xml");

	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildSale,		"Guild_ShopItem01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildQSale,		"Guild_ShopItem06.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgGuildLandBuild,		"Guild_LandBuild.xml");

	CREATE_DIALOG_FROM_TEMPLATE(DlgIndividualityColor,		"Dye_Combine.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgIndividualityReveal,		"Dye_FashDress.xml");
	

    CREATE_DIALOG_FROM_TEMPLATE(CDlgWebHot,     "Explorer_Popu_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKnowsMin,     "zhuxianknows_min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillActive,				"Skill_Activate.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAction, "Action.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgArrangeTeam, "ArrangeTeamSip.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgArrangeTeamSetting, "ArrangeTeamSip_Setting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAskHelpToGM, "InputString6.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuctionBuyList, "AuctionBuyList.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuctionList, "AuctionList.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuctionSellList, "AuctionSellList.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuctionSearch, "AuctionSearch.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAward, "Award.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAutoLock, "AutoLock.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgBBS, "BBS.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleInfo.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleInfo2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleInfo_Challenge.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleScore.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleList2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "BattleJoinChoice.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "PKScore.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "PKScoreHover2013.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo, "PkScore2013.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInstance, "BattleRaidStage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInstance, "BattleRaidMiniHint.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInstance, "BattleRaidJoinChoice.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInstance, "BattleRaidApliList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInstance, "BattleRaid.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBuddyState, "BuddyState.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBuddyState, "RideTogether.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQilinBuddy, "InviteRide.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBuff, "Buff.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBooth, "PShop?.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBooth, "InputNO2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBookScroll, "Book.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBookType1, "Book_Type1.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBookVertical, "Book_Type2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGoldAccount, "GoldAccount.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGoldInquire, "GoldInquire.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGoldTrade, "GoldTrade.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGoldPassword, "GoldPwdChange.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGoldPassword, "GoldPwdInput.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgSpeakerSpecial , "ChatSpec.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingGame, "Chat_Setting.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "Chat*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "KfpkChat.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "Popface.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "PopTalk.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "PopfaceManage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgChat, "WhisperChat.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCamera, "Camera.*");
	CREATE_DIALOG_FROM_TEMPLATE(DlgRemedyMetempsychosis,   "Character_Attrimakeup.xml"); 	//2012-10-17 by add zy 添加 弥补飞升界面
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLevelUp , "Character_LvAttriDetail.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacterDungeonAward,		"Character_Dungeon_Award.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacterDungeonBook,		"Character_Dungeon_Book.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacter, "Character.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacter, "Credit.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacter, "Credit2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacter, "TitleBox.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCharacter, "Character_*.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgChannelCreate, "ChannelCreate.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgChannelJoin, "ChannelJoin.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSnsInfo, "PersInfo.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSnsInfo, "PersInfoCheck.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgConsole, "Console.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgContest, "Contest.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgContest, "ContestMini.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSiege, "Siege*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSiege, "GuildSiege.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSiege, "SiegeHelperCh.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCityBuilding, "CityBuilding.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCityBuilding, "CityGate.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCityBuilding, "CityTower.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgChannelChat, "ChannelChat.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgChannelOption, "ChannelOption.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgChannelPW, "InputString9.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDragDrop, "DragDrop.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDoubleExp, "DoubleExp.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDoubleExpS, "DoubleExpS.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgEPEquip, "EPEquip.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgExit, "Message2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgExit, "Message7.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgExit, "Message6.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipBind, "EquipBind.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipDestroy, "EquipDestroy.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipUndestroy, "EquipUndestroy.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgFaceLift, "Cosmetic.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat0.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat1.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat3.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat4.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat5.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat6.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat7.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat8.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChat, "FriendChat9.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendChatHotKey, "FriendChatHotkey.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendColor, "FriendColor.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendGroup, "FriendGroup.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendHistory, "FriendHistory.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendList, "FriendList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendOptionGroup, "FriendOptionGroup.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendOptionName, "FriendOptionName.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendMoveGroup, "FriendOptionName_Grouplist.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendOptionNormal, "FriendOptionNormal.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEnemyOptionName, "EnemyOptionName.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendRequest, "Message1.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFrdCallBck,"MailtoFriend.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFrdCallBck,"MailtoFriend_Success.xml");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMConsole, "GMConsole.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMDelMsgReason, "GMDelMsgReason.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMFinishMsg, "GMFinishMsg.*");
 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMParam, "GMParam.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildCreate, "FamilyCreate.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildCreate, "GuildCreate.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "GuildEnemies.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "FamilyManage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "GuildManage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "GuildAppoint.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "GuildWar.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMan, "GuildWarStatus.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMain,		"GuildMain.xml");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMap, "GuildMap.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMapStatus, "GMapStatus1.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMapStatus, "GMapStatus2.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMapStatus, "GMapChallenge.*");
// 	CREATE_DIALOG_FROM_TEMPLATE(CDlgGuildMapTravel, "GMapTravel.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelp, "Help_Cyclo.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHorseTame, "HorseTame.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "CharHead.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "CharHeadClose.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "Progress*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "MagicProgress*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "CharHead_Prof11.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "CharHead_Prof13.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHost, "Charhead_HpPrgs.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHostPet, "PetHead.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHostPet, "PetHeadClose.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgIdentify, "Identify.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInfo, "Pop.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInfo, "Poplist.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputName, "InputString5.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputName, "InputString10.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputName, "InputString11.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputName, "InputString12.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputNO, "InputNO.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputNO, "InputGNO.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputNO, "InputMoney.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputNO, "InputMoney2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInputNO, "Message15.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInstall, "Enchase.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInstall, "Disenchase*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgItemLock, "ItemLock.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgItemLock, "ItemUnlock.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventory, "Inventory.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventory, "InventoryPet.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventoryCostume, "Inventory_CostumeQuick.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventoryCostume, "Inventory_Costume.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventoryRide,	"Inventory_Vechile.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMend, "Mend.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMend, "Mending.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLottery, "Lottery.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLotteryNew, "Lottery2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLottery3, "Lottery3.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLotteryGift, "Lottery4.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgLotteryTangYuan, "Lottery5.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMailList, "MailList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMailOption, "MailOption.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMailRead, "MailRead.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMailWrite, "MailWrite.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMidMap, "MidMap.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgMiniMap, "MiniMapB.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMiniMap, "Minimap_Recommend.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMiniMap, "MiniMap.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMiniMap, "MiniMapClose.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMiniMap, "Mark.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMinimizeBar, "MinimizeBar*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgNPC, "NPC.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgNPC, "npc_Main.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMapTransfer, "Map_Transfer.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetAction, "Pet_Skill.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetAction, "Pet_AttackSetting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetAction, "PetAutoFood.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetAction, "Pet_Feed.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetEquip, "PetEquip.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetFactory, "PetFactory.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetPackage, "PetPackage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetDetail, "PetDetail.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetService, "PetRaise.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetService, "PetChangeName.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetService, "PetCombine.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetService, "PetRelease.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetService, "PetEquipEnchanse.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetMain, "Pet_Main.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetOperate, "Pet_Tame.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetOperate, "Pet_Release.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetReName, "Pet_ChangeName.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetDetailOperate, "Pet_Detail.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetEquipService, "Pet_EquipEnhanse.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangeBone.*");		// 修炼舍利
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangeFight.*");		// 修炼斗气
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombine, "Pet_ChangeHoroscope.*");	// 修炼星相
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombine, "Pet_ChangeImage.*");		// 修炼外观
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombine, "Pet_ChangeLife.*");		// 补充寿命
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangeNature.*");		// 改变特质
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangePrelife.*");		// 改变前世
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangeSkill.*");		// 修炼技能
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ExpandSkill.*");		// 扩充技能
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineZiZhi, "Pet_UpNum.*");				// 修炼资质
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetCombineExchange, "Pet_ChangeBasicType.*");	// 改变限界

	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetFly, "PetLevelUp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetFlyAttrUp, "Pet_Detail_LvupAttri.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetFlyChangeAppearance, "PetLvUp_SetImg.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetFlyShow, "PetLvUp_Img.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetSetting, "PetProSetting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPetSetting, "PetStsSetting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPKSetting,	"PKSetting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPKSetting,	"KfpkPKSetting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPKMsg,		"PKMsg.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgProduce, "Produce_Icon.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgProclaim, "InputString8.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuestion, "Question.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickAction, "QuickAction.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBodyChange, "BodychangeSkill.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubR.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL01.xml");
    CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL03.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL04.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL05.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL06.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL07.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetail, "QuickBar_SettingSubL08.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSettingDetailFrame, "QuickBar_SettingSubL.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBarSetting, "QuickBar_Setting.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuickBar, "Quickbar*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQShop, "QShop.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQShop, "QShopPop.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQShopItem, "QShopItem*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRankList, "RankList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRecord, "Record.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRecord, "RecordEnd.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgReplay, "Replay.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgReplayEdit, "ReplayEdit.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgResetProp, "ResetSkillPart.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgResetProp, "ResetSkillAll.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgResetProp, "ResetTalent.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRevive, "Message4.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgReportToGM, "InputString7.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingSystem, "SettingSystem.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingVideo, "SettingVideo.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingGame, "SettingGame.*");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgShop, "Shop.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopBuyBack, "Shop_Withdraw.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShop, "Shop_AutoSell.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopExchange, "ShopExchange*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopReputation, "Shop_Credit.xml");		
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillMan, "Skillzx2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "Skill.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "SkillEquip.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "SkillOther.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "SkillSocial.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "SkillTalent.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkill, "SkillSpec.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillEdit, "SkillContinual.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillTree, "Skill_*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillTree, "Talent_*.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgSplit, "Split.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgStorage, "Storage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgStorageChangePW, "InputString3.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgStoragePW, "InputString.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystemAll, "Winmain_All.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem3, "Winmain_System.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem, "WinMainClose.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem, "WinMain.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem2, "WinMain2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem3, "WinMain3.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSystem4, "WinMain4.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameCasual, "GameCasual.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "Talisman.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanSmall.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanAttribute.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanSettingAll.*");
	//CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanSetting.*");
	//CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanSettingOther.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "TalismanSettingShieldList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "Talisman_AutoPick.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalisman, "Talisman_AutoShield.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanSkillSel, "TalismanQuickBar.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanClean.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanCombine.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanRefine.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanUpgrade.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanEnchanting.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanLevelUp.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanCombineSkill.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanService, "TalismanEnchaseSkill.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanCombineSkillSelect, "TalismanCombineSkill_Info.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTalismanExchange, "TalismanMedium.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTarget, "TargetPlayer.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTarget, "TargetMonster.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTarget, "TargetNPC.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTarget, "TargetNPC2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTargetHP, "Target_HpPrgs.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTask, "QuestMinion_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTask, "QuestMinion.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTask, "Quest.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTaskList, "QuestList.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTaskDaily, "TaskDaily.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeach, "Teach.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeacherMan, "TeacherManage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeacherMan, "Skill_Teacher.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeamContext, "Group*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeamMain, "TeamMain.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTeamMate, "TeamMate*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTextFirework, "TextFirework.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTrade, "Trade.*");
//	CREATE_DIALOG_FROM_TEMPLATE(CDlgWarTower, "WarTower.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWorldMap, "WorldMap.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWorldMap, "WorldMap_Sub.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWorldMap, "WorldMap_Button.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgScriptHelp, "ScriptHelp.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingScriptHelp, "Setting_ScriptHelp.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgScriptHelpHistory, "ScriptHelpHistory.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBroadCast, "Broadcast.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "EquipDepart.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "EquipEnchase.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "EquipOwn.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "EquipResume.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "EquipSpilt.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoul, "Message8.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoulCombine, "SoulCombineFactory.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSoftKB, "SoftkbGame.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCalendar, "Calendar.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendSeeking.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendSeekingMsg.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendInfoPub.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendInfoPub.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "AskingForGuild.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "AskingForFriend.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "AskingForTeacher.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "AskingForMarriage.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendSeekingPerCenter.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFriendSeek, "FriendSeekingPerCharm.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSpread, "PopFriend.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSpread, "PopProfit.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSpread, "PopIdCheck.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleSong, "BattleSong.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleSong, "BattleSongScore.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleSong, "BattleSongTime.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePk, "BattleListPK.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePk, "BattleScorePK.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePk, "BattleListPKAwait.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFaceOff,  "ImageChange.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFaceOff,  "ImageChange_Lieshan.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBaseExplorer, "explorer.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBaseExplorer, "explorer*.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgZhuxianKnows, "ZhuxianKnows.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchievement,		"Success.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivMain,			"Achievement.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivItemMan,		"Achievement_Sub.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivItem,			"Achievement_Done.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivItemExpand,	"Achievement_DoneFolder.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivPop,			"Achievement_PopUP.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchivItemOther,		"Achievement_Done_Other.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAchievement,		"Success.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,			"MessageSuccess.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,			"ItemDesc.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,			"AchievementDesc.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKrGM,				"Kr_winmain2.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGamePoker,			"GamePoker_Match.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGamePlaza,			"GamePoker_Hall.*");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameHelp,			"GamePoker_Help.*");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgModule, "Garden_Option.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgModule, "Garden_OptionPlus.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_PlotMethod.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Seed.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_CheckStatus.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_CheckStatusOther.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Mine.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Others.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Action.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_ActionOther.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_CheckStatusLove.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_CheckStatusMature.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Setting.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMan, "Garden_Friendlist.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenInventory, "Garden_Inventory.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenQShop, "Garden_Prop.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpMemo, "Help_Memo.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpMemo, "Help_MemoCustom.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpMemo, "Help_MemoWay.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpLvUp, "Help_LvUp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpFariy, "Help_Fariy.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMoney, "Garden_ExchangeMain.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMoney, "Garden_Exchange1.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMoney, "Garden_Exchange2.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGardenMoney, "Garden_Exchange3.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgBodyChange, "BodyChange_Enchanting.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBodyChange, "BodyChange_Recover.xml");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTerrWarMap, "WorldMap_TerriSub.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTerrInfo, "Battle_TerriDetail*.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTerr, "Battle_Terri*.xml");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillComboEdit, "SkillComboEdit.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCameraPath, "CameraPath.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuction, "Auction.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuction, "AuctionRangeBuy.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuction, "AuctionRangeBuyConfirm.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuction, "InputMoney_Auction.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAuction, "AuctionChecklist.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMarkAstro, "MarkAstro.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgClassmateManage, "ClassmateManage.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMarkAstroList, "MarkAstroList.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipLvup, "EquipLvup.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipLvup, "Equip_JewelUp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopConsign, "ShopConsign.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgPopmsgSafeArea, "Popmsg_SafeArea.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMenology, "Menology.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPromp, "DailyPromp_Today.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPromp, "DailyPromp_Week.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPromp, "DailyPromp_Battlefield.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPromp, "DailyPrompBoss.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenSoul, "Fengshen_Soul.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenSoul, "Fengshen_Soul_Check.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPrompLoginHint, "DailyPrompLoginHint.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDailyPrompLoginHint, "DailyPrompExit.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKfSelectLine, "KfSelectLine.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopConsignView,"shopconsignview.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgShopConsignAction, "ShopConsignAction.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgStorageTaskCross, "questKf.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipJack, "Accessory_Drill.xml");		
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipJack, "Accessory_ReDrill.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipJack, "Accessory_Custom.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipGem, "Accessory_Attach.xml");		// 宝石镶嵌
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipGem, "Accessory_Detach.xml");		// 宝石拆除
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBijouUpgrade, "Accessory_Upgrade.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBijouRefine, "Accessory_Refine.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBijouExtract, "Accessory_Spilt.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBijouTransfer, "Accessory_Move.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBroadCastInGame, "radio.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTaskZhuXianList, "Quest_Main.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo,		"Battle6v6.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleInfo,		"BattleResult6v6.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePkScore,	"Battle6V6ScoreBoardL.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePkScore,	"Battle6V6ScoreBoardS.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattlePkScoreResult2011,	"PKRecord2011.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMCallMultiPlayer,		"GMGroupTransfer.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgGMTransferMap,		"GM_TransferMap.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonApply,			"Battle_DungeonApply.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonApplyList,		"Battle_DungeonApplyList.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonMemberList,	"Battle_DungeonMemberList.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonNameInput,	"Battle_DungeonRoomNameInput.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonNameInput,	"Battle_Dungeon_SiX_Room.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMessageBoxYesNoTime,		"Message9.xml");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgVipAwardItem,		"QShopVipItemDefault*.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgVipAward,		"QShop_vip.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKrAntiwallow,		"Kr_AntiWallow.xml");	
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgZhuxian5Year,		"AniversarySign5.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgZhuxian5YearMin,	"AniversarySign5_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAwardOnline,		"AwardOnline.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAwardAccountInfo,	"AccountInfo_Award.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDlgTreas,	"ZhuxianExplore.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDlgTreasMin,	"ZhuxianExplore_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDlgTreasEvent,	"ZhuxianExploreIssue.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDlgTreasIntro,	"ZhuxianExploreIntro.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTower,			"Battle_Dungeon_Tower.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTowerStage,	"Battle_Dungeon_Tower_CharScore.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTowerLuckySkill,	"Battle_Dungeon_Tower_Lucky.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTowerLuckySkill,	"Battle_Dungeon_Tower_LuckySkill.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTowerRandom,	"Battle_Dungeon_Tower_Random.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTowerScore,	"Battle_Dungeon_Tower_Result.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonHeyang,		"Battle_Dungeon_Heyang.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonHeyangIntro,	"Battle_Dungeon_Heyang_Intro.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonTime,	"Battle_Dungeon_SiX_Time.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneMain,	"Equip_Yuanying.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneReset,	"Equip_Yuanying_Default.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSlotErase,	"Equip_Yuanying_HoleErase.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneIdentify,	"Equip_Yuanying_Identify.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneRefine,	"Equip_Yuanying_Refresh.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneFillSlot,	"Equip_Yuanying_RuneOn.xml");
	//CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneDecompose,	"Equip_Yuanying_Spilt.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneLevelUp,	"Equip_Yuanying_Upgrade.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneErase,	"Equip_Yuanying_RuneErase.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAniversaryPet5,	"AniversaryPet5.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlganiversarypet5_min , "AniversaryPet5_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAniversaryMail5Min , "AniversaryMail5_Min.xml");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgUpgrade , "UpdateDiary.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTreasureEnter , "ShopConsign_LinkPage.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgTab3D,					"Kf2012Enrol.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKf2012GuessChampion,	"Kf2012Bet1.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKf2012GuessTop3,		"Kf2012Bet2.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKf2012GuessChampionInput,"Kf2012BetInput1.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKf2012GuessTop3Input,	"Kf2012BetInput2.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,		"Kf2012Bet_Min.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgFastPay,			"Qshop_QuickInputMoney.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFastPayCode,		"Qshop_QuickInputCode.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgReName,				"Char_ChangeName.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgReName,				"Guild_ChangeName.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgNameHistory,		"Char_ChangeName_History.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Apply,		"Battle_Dungeon_6v6_MapSelect.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6ApplyMin,	"Battle_Dungeon_6v6_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6ApplyMin,	"Battle_Dungeon_6v6_Wait.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeamCustom,		"Battle_Dungeon_6v6_CharSelect.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Skill,		"Battle_Dungeon_6v6_CharSelect_Skill.xml");
	//CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Score,		"Battle_Dungeon_6v6_Score.xml"); 策划说不要了，但是某天又要要呢？
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Award,		"Battle_Dungeon_6V6_Award.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Record,	"Battle_Dungeon_6v6_Record.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleTeam6v6Score,		"Battle_Dungeon_6v6_Score.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgTreasureRadar,           "DragonCampass.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTreasureRadar,           "ZongziCampass.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAttack,				  "NewerHelp_ImgPop.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpStart,				  "NewerHelp_Hint.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleFactionWarApply,    "Battle_GuildWar_Apply.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleFactionWarControl,   "Battle_GuildWar_Applylist.xml");
    CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleFactionWarWatch,     "Battle_GuildWarList.xml");
    CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpGraphGuide,     "NewerHelp_Min.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenAttach,     "Equip_Fengshen_Attach.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenConvert,     "Equip_Fengshen_Convert.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenDrill,     "Equip_Fengshen_Drill.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenErase,     "Equip_Fengshen_Erase.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenIdentify,     "Equip_Fengshen_Identify.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFengshenUpgrade,     "Equip_Fengshen_Upgrade.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,			"Equip_Fengshen_Prgs.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTaskAncient,   "Quest_Wanted.xml");	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMessageBoxYesNoCheck,   "MsgBoxYesNoeCheck.xml");	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQuitReason,   "QuitReason.xml");	

	CREATE_DIALOG_FROM_TEMPLATE(CDlgAgentCombined,   "DrugEnchanting.xml");	//2012-10-15 by add zy 添加药剂组合界面	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCommonDlg,   "Radio_Min.xml");	

	CREATE_DIALOG_FROM_TEMPLATE(CDlgAncientMini,		"Dungeon_Ling_Mini.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAncientLevelInfo,   "Dungeon_Ling.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAncientTime,		"Battle_Dungeon_Level_Time.xml");	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMessageBoxYesTime,  "MessageBoxYesTime.xml");	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFlyProgress,		"Fly_Prgs.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameSnake,		    "GameSnake.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameAcupoint,		    "Game_Acupoint.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameAcupointResult,	    "Game_Acupoint_WinGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGameAcupointResult,	    "Game_Acupoint_LoseGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgMessageBoxYesNoCheckYuanBao,	"Lottery5_Popup.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCalendarPresent,	"CalendarPresent.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgCalendarItem,		"CalendarItemDefault*.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWindowsGfx,						"WindowGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSpring2012,          "SpringFestival2012.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHostAction,          "Charhead_Copyname.xml");

    CREATE_DIALOG_FROM_TEMPLATE(CDlgKingBattleInfo,          "King_BattleInfo.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingBattleInfo,          "King_BattleInfo2.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingChangeSlogan,          "KingChat.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingInfo,          "King_Info.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingMin,          "King_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingRankList,     "King_Ranklist.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingOfficialInfo,  "King_SubRight01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingShop,          "King_SubRight02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingContinueAward,          "King_SubRight03.xml");
    CREATE_DIALOG_FROM_TEMPLATE(CDlgKingTask,          "King_SubRight04.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingTask,          "King_SubRight05.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingGrantRightContext,       "KingOption.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingGrantOfficial,    "KingAuthorize.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingAnnounce,          "King_Anounce.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingBattleInfo3,          "King_BattleInfo3.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingBathInvite,          "King_ShowerInvite.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgKingBathTip,          "King_ShowerInfo.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAttriUp,				 "AttriUp.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAttriUpEnchase,      "AttriUp_Enchase.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgAttriUpNature,       "AttriUp_Nature.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgQProduceTool,       "QProduce_Tool.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingPostEffect,       "SettingPostEffect.xml");


	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoApply,				 "Battle_SeniorPK_Apply.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoMax,      "Battle_SeniorPK_Score.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoMin,       "Battle_SeniorPK_ScoreMin.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoWaitMax,       "Battle_SeniorPK_WaitLine.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoPrompt,       "Battle_SeniorPK_Enter.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoReachExit,       "Battle_SeniorPK_Exit01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgBattleFlowInfoNoReachExit,       "Battle_SeniorPK_Exit02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTouchMin,    "Game_Touch_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTouchInfo,          "Game_Touch.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgTouchExchange,          "Game_TouchExchange.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideMin,		"Help_GameGuide_Min.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideEnter,		"Help_GameGuide.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuide,		    "Help_GameGuide_Detail.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail,		"Help_GameGuide_DetailSub1.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail,		"Help_GameGuide_DetailSub2.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail,		"Help_GameGuide_DetailSub3.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail2,	"Help_GameGuide_DetailSub4.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail2,	"Help_GameGuide_DetailSub5.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpAimGuideDetail2,	"Help_GameGuide_DetailSub6.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgEquipReName,          "Equip_ChangeName.xml");
	
	CREATE_DIALOG_FROM_TEMPLATE(CDlgGT,			    "Gt.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgIcons,          "FoldIcon.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventoryExPand, "Inventory_Expand02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgInventoryExPand, "Inventory_Expand01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWebShopOrder,    "Qshop_Web.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneCustom,		"Equip_Yuanying_Custom01.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgRuneCustom,		"Equip_Yuanying_Custom02.xml");
	CREATE_DIALOG_FROM_TEMPLATE(DlgIndividualityColor,		"Individuality.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGameGuess,		"Game_Guess.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGame,				"Game_Guess_Start.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGameResult,		"Game_Guess_Result.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGameResult,		"Game_Guess_LoseGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGameResult,		"Game_Guess_DeuceGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgFGGameResult,		"Game_Guess_WinGfx.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSettingEffect,		"Panel_SetEffect.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpGuide,		"Help_Up2013.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpGuide,		"Help_Right2013.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpGuide,		"Help_Left2013.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpGuide,		"Help_Down2013.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpMove,		"Help_Wasd.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpWelcom,		"Help_Welcome.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpFight,		"Help_Tab.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillTeleport,		"Prof13_Skill_Transfer.xml");
	CREATE_DIALOG_FROM_TEMPLATE(CDlgSkillRemote,		"Prof13_Skill_Remote.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgDungeonStep,	"Battle_Dungeon_Step_Min.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDungeonStep,	"Battle_Dungeon_Step_Max.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDungeonStepDetail,	"Battle_Dungeon_LzStep_Detail.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDungeonStepDetail,	"Battle_Dungeon_FxStep_Detail.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgDungeonStepDetail,	"Battle_Dungeon_SxStep_Detail.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgBattleDungeonSsk,	"Battle_Dungeon_Ssk_Prgs.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgExit, "InteractionOption.xml");

	CREATE_DIALOG_FROM_TEMPLATE(CDlgHelpInfo,	"HelpInfo.xml")
	CREATE_DIALOG_FROM_TEMPLATE(CDlgWish,			"Wish.xml");

	return new CDlgBase;
}
