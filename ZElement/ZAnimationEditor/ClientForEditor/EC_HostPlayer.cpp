/*
 * FILE: EC_HostPlayer.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */


#include "EC_Global.h"
#include "EC_HostPlayer.h"
#include "EC_GameRun.h"
#include "EC_Game.h"
//#include "EC_InputCtrl.h"
#include "EC_Model.h"
#include "EC_Utility.h"
#include "EC_World.h"
//#include "EC_GameSession.h"
#include "EC_CDS.h"
#include "EC_ShadowRender.h"
#include "EC_PortraitRender.h"
//#include "EC_NPCServer.h"
#include "EC_NPC.h"
#include "EC_Matter.h"
#include "EC_RTDebug.h"
#include "EC_ManNPC.h"
#include "EC_ManMatter.h"
#include "EC_ManPlayer.h"
//#include "EC_GameUIMan.h"
//#include "EC_UIManager.h"
#include "EC_FixedMsg.h"
//#include "EC_Inventory.h"
//#include "EC_DealInventory.h"
//#include "EC_IvtrScroll.h"
#include "EC_IvtrTypes.h"
//#include "EC_IvtrConsume.h"
//#include "EC_IvtrTalismanMain.h"
//#include "EC_IvtrPetItem.h"
#include "EC_ElsePlayer.h"
#include "EC_GFXCaster.h"
#include "EC_Resource.h"
//#include "EC_ShortcutSet.h"
//#include "EC_Skill.h"
//#include "EC_TaskInterface.h"
//#include "EC_HostInputFilter.h"
#include "EC_Configs.h"
//#include "EC_PateText.h"
//#include "EC_Team.h"
//#include "EC_Friend.h"
//#include "EC_Faction.h"
#include "EC_Sprite.h"
//#include "EC_PetCorral.h"
//#include "EC_ComboSkill.h"
//#include "EC_LoginPlayer.h"
#include "EC_CDR.h"
//#include "EC_Helper.h"
//#include "EC_AutoHelper.h"
//#include "EC_NPCCarrier.h"
//#include "EC_Secretary.h"
//#include "TaskTemplMan.h"

// #include "EC_HPWork.h"
// #include "EC_HPWorkUse.h"
// #include "EC_HPWorkDead.h"
// #include "EC_HPWorkMelee.h"
// #include "EC_HPWorkStand.h"
// #include "EC_HPWorkTrace.h"
// #include "EC_HPWorkFollow.h"
// #include "EC_HPWorkAutoMove.h"
// #include "EC_HPWorkMove.h"
// #include "EC_HPWorkFly.h"
// #include "EC_HPWorkSit.h"
// #include "EC_HPWorkFall.h"
#include "EC_Decal.h"
//#include "defence/stackchecker.h"

//#include "DlgTalisman.h"
//#include "DlgPetPackage.h"

//#include "roleinfo"
//#include "playerbriefinfo"
//#include "Network\\gnetdef.h"

#include "A3DBone.h"
#include "A3DSkeleton.h"
#include "A3DGFXEx.h"
#include "A3DGFXExMan.h"
#include "A2DSprite.h"
#include "A3DTerrain2.h"
#include "A3DViewport.h"
#include "A3DFont.h"
#include "A3DCamera.h"
#include "A3DCollision.h"
#include "A3DSkinMan.h"
#include "A3DSkin.h"
//Added 2011-06-13.
#include "A3DGFXFuncs.h"

#include "elementdataman.h"

#include <algorithm>

#include "AMSoundBufferMan.h"
#include "AMSoundBuffer.h"
//#include "EC_GameRecord.h"
//#include "DlgChat.h"
#include "EL_Precinct.h"
#include "EC_World.h"
//#include "EC_Achievement.h"
//#include "EC_Cutscene.h"
//#include "EC_Circle.h"

//#include "LuaEvent.h"
//#include "EC_OnlineAward.h"	//Added 2012-03-23.
//#include "EC_BattleDungeonTower.h"

#ifdef BACK_VERSION
#include "EC_Viewport.h"
#include "A3DTerrainWater.h"
#else
#include "ECViewport.h"
#include "A3DTerrainWater2.h"
#endif

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Host configs data version
#define HOSTCFG_VERSION		4

//	ID of return-town skill
#define ID_RETURNTOWN_SKILL	167

#define FADE_DIS	120.0f
#define FADE_DIS2	3.0f

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

static float l_fTestDist = -1.0f;
static int max_jump_count = 1;
//extern game_record gr;

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

CECHostPlayer::CECHostPlayer(CECPlayerMan* pPlayerMan) : 
CECPlayer(pPlayerMan),
m_MoveCtrl(this),
m_tbGFXTable(256),
m_tbJailCircleAreaTable(256),
m_tbGfxName(256),
m_tbPullGfxInfo(256)
{
	m_iCID				= OCID_HOSTPLAYER;
	m_fCameraPitch		= 0.0f;
	m_fCameraYaw		= 0.0f;
	m_dwMoveRelDir		= 0;
	m_dwOldDir			= 0;
	m_iTurnCammera		= 0;
	m_bCastShadow		= true;
	m_idSelTarget		= 0;
	m_idUCSelTarget		= 0;
	m_idPickMatter		= 0;
	m_idClickedMan		= 0;
	m_idSevNPC			= 0;
	m_idTradePlayer		= 0;
	m_bUsingTrashBox	= false;
	m_fFlyHeight		= 0.0f;
	m_iOldWalkMode		= MOVE_STAND;
	m_bTalkWithNPC		= false;
	m_pPrepSkill		= NULL;
	m_bRenderName		= true;
	m_bBeRoot			= false;
	m_bMelee			= false;
	m_bTrashPsw			= false;
	m_iTrashBoxMoneyCnt	= 0;
	m_bFirstTBOpen		= true;
	m_bFirstFTBOpen		= true;
	m_dwLIES			= 0;
	m_idCurHover		= 0;
	m_fTransparent		= -1.0f;
	m_bEnterGame		= false;
	m_idBoothTrade		= 0;
	m_fReviveExp		= -1.0f;
	m_bChangingFace		= false;
	m_iLoyalty			= 0;
	m_bSpellDSkill		= false;
	m_iGetFriendCnt		= 0;
	m_dwPariahTime		= 0;
	m_bProducing		= false;
	m_bRenderFamily		= true;
	m_nTalismanValue	= 0;
	m_nRuneValue		= 0;
	m_nTalentValue		= 0;
	m_nBattleScore		= 0;
	m_nSJBattleScore	= 0;
	m_iReviveCountDown  = 0;

	m_pPack				= NULL;
	m_pEquipPack		= NULL;
	m_pTaskPack			= NULL;
	m_pPetPack			= NULL;
	m_pPetEquipPack		= NULL;
	m_pDealPack			= NULL;
	m_pEPDealPack		= NULL;
	m_pTrashBoxPack		= NULL;
	m_pFactionTrashBoxPack = NULL;
	m_pBuyPack			= NULL;
	m_pSellPack			= NULL;
	m_pBoothSPack		= NULL;
	m_pBoothBPack		= NULL;
	m_pEPBoothSPack		= NULL;
	m_pEPBoothBPack		= NULL;
	m_pEPEquipPack		= NULL;
	m_pEPPetPack		= NULL;
	m_pEPPetEquipPack	= NULL;
	m_pPocketPack		= NULL;
	m_pFashionPack		= NULL;
	m_pHTFarmPack		= NULL;

	m_pMoveTargetGFX	= NULL;
	m_pSelectedGFX		= NULL;
	m_pHoverGFX			= NULL;
	m_pFloatDust		= NULL;
	m_pTaskInterface	= NULL;
	m_pWorkMan			= NULL;
	m_pSpritePortrait	= NULL;
	memset(m_pDuelCountDown, 0, sizeof(A2DSprite *) * 3);
	memset(m_pDuelStates, 0, sizeof(A2DSprite *) * 3);
	m_pFactionCombatSprite = NULL;
	m_nFactionCombatCounts = 0;
	
	m_pGuildWarStartSprite	= NULL;
	m_nGuildWarStartCounts	= 0x7fffffff;
	m_pGuildWarWinSprite	= NULL;
	m_nGuildWarWinCounts	= 0x7fffffff;
	m_pGuildWarLostSprite	= NULL;
	m_nGuildWarLostCounts	= 0x7fffffff;

	m_pCurMoveSnd		= NULL;
	m_pFriendMan		= NULL;
	m_pPetCorral		= NULL;
	m_pComboSkill		= NULL;

	m_bAboutToFight		= false;

	m_iJumpCount		= 0;
	m_bJumpInWater		= false;
	m_bJumpByQingGong	= false;
	m_fVertSpeed		= 0.0f;

	m_nProduceSkillLevel= -1;
	m_nProduceSkillExp	= 0;
	m_iAllSkillLvlUp	= 0;
	m_bSkillDataInited	= false;
	m_bTransfigureSkillDataInited = false;
	m_bCombineSkillDataInited	= false;

	m_pModelTray		= NULL;
	m_bShowMoveArrow	= false;
	m_bShowTempArrow	= false;
	m_bShowTeamArrow	= false;
	m_bShowRequestArrow	= false;
	m_pModelMoveArrow	= NULL;
	m_pModelTempArrow	= NULL;
	m_pModelTeamArrow	= NULL;
	m_pModelRequestArrow= NULL;
	m_pModelTargetArrow	= NULL;
	m_pModelMoveCursor	= NULL;
	m_pModelTempCursor	= NULL;
	m_pModelTeamCursor	= NULL;
	m_pModelRequestCursor=NULL;
	m_pModelTargetCursor= NULL;

	m_pCameraPath		= NULL;
	m_pCutscene			= NULL;

	m_vVelocity.Clear();
	m_vAccel.Clear();

	m_TaskCounter.SetPeriod(6000);
	m_TrickCnt.SetPeriod(1000);
	m_TrickCnt.Reset(true);
	m_TransCnt.SetPeriod(500);
	m_GatherCnt.SetPeriod(1000);
	m_GatherCnt.Reset(true);
	m_TLPosCnt.SetPeriod(10000);
	m_TLPosCnt.Reset(true);
	m_TMPosCnt.SetPeriod(60000);
	m_TMPosCnt.Reset(true);
	m_BindCmdCoolCnt.SetPeriod(35000);
	m_BindCmdCoolCnt.Reset(true);
	m_PetOptCnt.SetPeriod(1000);
	m_PetOptCnt.Reset(true);
	m_UnderAtkCnt.SetPeriod(3000);
	m_UnderAtkCnt.Reset(true);
	m_ReAtkCnt.SetPeriod(10000);
	m_ReAtkCnt.Reset(true);
	m_RandomMoveCnt.SetPeriod(a_Random(100,2000));
	m_RandomMoveCnt.Reset(true);
	m_RandomStopCnt.SetPeriod(a_Random(100,2000));
	m_RandomStopCnt.Reset(true);
	m_iRandomMoveDir = a_Random(0,7);
	m_FlyingOffCnt.SetPeriod(1500);
	m_FlyingOffCnt.Reset(true);
	m_SkillCoolCnt.SetPeriod(2000);
	m_SkillCoolCnt.Reset(true);
	m_nCash = 0;
	m_nCashConsumed = 0;
	m_AgentTime = 0;
	m_nCurEmotionSet = 0;	
	m_nContribution = 0;
	m_nFamilyContrb = 0;
	m_nCombatFaction = 0;
	m_bInWar = false;
	m_nCurPetId = 0;
	m_nCurPetIndex = -1;
	m_nCurPetMaxHp = 1;
	m_nCurPetMaxVp = 1;
	m_nPetCombinedType = -1;
	m_bSafeLock = false;
	m_bUnderEPAtk = false;
	m_iBonusAmount = 0;
	m_iBonusUsed   = 0;
	m_iCurSkillKeyCode = 0;
	m_bFlyingOff	= false;
//	m_dwHideEquipMaskBackUp = 0;

	m_pCDRInfo = new CDR_INFO;
	m_pAirCDRInfo = new ON_AIR_CDR_INFO;
	m_bCarrierCooling	= false;
	m_nTerritoryBattle	= -1;
	m_nRaidBattle		= 0;
	m_iLastLogoutTime	= 0;
	m_bHasCachePos		= false;

	//
	m_fSkillSpeedRate	= 1.0f;
	//Added 2011-07-28.
	m_dwLastMoveTime	= 0;
	//Added 2011-08-02.
	m_bSpecialCharge	= false;
	//Added 2012-05-04.
	m_bStartPosSkill	= false;
	m_nPosSkillID		= 0;
	m_pCastPosSkillGFX	= NULL;	//Added 2012-05-05.

	m_nTabSelOrder = TABSEL_ALL;

	memset(m_aNPCPacks, 0, sizeof (m_aNPCPacks));
	memset(m_aSCSets1, 0, sizeof (m_aSCSets1));
	memset(m_aSCSets2, 0, sizeof (m_aSCSets2));
	memset(m_aMoveSnds, 0, sizeof (m_aMoveSnds));
	memset(&m_BreathData, 0, sizeof (m_BreathData));
	memset(&m_BattleInfo, 0, sizeof (m_BattleInfo));
	memset(&m_WarInfo, 0, sizeof (m_WarInfo));
	memset(&m_ApoiseSkill, 0, sizeof(m_ApoiseSkill));
	memset(m_aFashionHotKey, 0, sizeof(FASHION_HOTKEY)*FASHION_HOTKEY_NUM);
	memset(&m_TransfigureProp, 0, sizeof(m_TransfigureProp));
	memset(&m_LittlePetInfo, 0 , sizeof(m_LittlePetInfo));
//	memset(m_aBackUpEquips, 0, sizeof(m_aBackUpEquips));

	int i;
	for (i=0; i < GP_CT_MAX; i++)
	{
		m_aCoolTimes[i].iCurTime = 0;
		m_aCoolTimes[i].iMaxTime = 0;
	}

	for (i=0; i < GP_CT_PRODUCE_END - GP_CT_PRODUCE_START; i++)
	{
		m_aProduceCoolTimes[i].iCurTime = 0;
		m_aProduceCoolTimes[i].iMaxTime = 0;
	}

	for (i=0; i < GP_CT_COMBINE_EDIT_END - GP_CT_COMBINE_EDIT_START; i++)
	{
		m_aSkillEditCoolTimes[i].iCurTime = 0;
		m_aSkillEditCoolTimes[i].iMaxTime = 0;
	}

	m_nTimeToExitInstance = 0;

	//	Create inventories
	if (!CreateInventories())
	{
		a_LogOutput(1, "CECHostPlayer::CECHostPlayer, Failed to create host's inventoies");
	}

	//	Create shortcuts
	if (!CreateShortcutSets())
	{
		a_LogOutput(1, "CECHostPlayer::CECHostPlayer, Failed to create host's shortcut sets");
	}
}

CECHostPlayer::~CECHostPlayer()
{
	delete m_pCDRInfo;
	delete m_pAirCDRInfo;
}

//	Initlaize object
bool CECHostPlayer::Init(const S2C::cmd_self_info_1& Info)
{
	m_PlayerInfo.cid	= Info.cid;
	m_PlayerInfo.crc_e	= Info.crc_e;

	m_BasicProps.exp		= Info.exp;
	m_BasicProps.iPKLevel	= Info.pk_level;
	m_BasicProps.iLevel		= Info.level;
	m_BasicProps.iProfession= Info.profession;
	m_BasicProps.iDeityLevel= Info.dt_level;
	m_iReputation			= Info.reputation;
	m_nRebornCount			= Info.reborn_count;
	m_iCultivation			= Info.cultivation;
	m_dwHideEquipMask		= Info.hide_equip;
	m_iRace					= CECPlayer::GetRaceByProf(Info.profession);
	m_iPhysique				= CECPlayer::GetPhysiqueByProf(Info.profession);
//	m_iRace					= g_pGame->GetGameInit().iRaceTest;
//	m_iRace					= RACE_ORC;
	//	Create work manager
// 	if (!(m_pWorkMan = new CECHPWorkMan(this)))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}

	//	Create friend manager
// 	if (!(m_pFriendMan = new CECFriendMan))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}
// 
// 	//	Create pet corral
// 	if (!(m_pPetCorral = new CECPetCorral))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}

	//	Create camera path
// 	if (!(m_pCameraPath = new CECCameraPath))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}
// 
// 	//	Create cutscene
// 	if (!(m_pCutscene = new CECCutscene))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}
// 	m_pCutscene->Init();
// 
// 	//	Create Online award manager
// 	if ( !(m_pOnlineAwardManager = new OnlineAwardManager) )
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}
// 	
// 	if ( !(m_pBattleDungeonTower = new CECBattleDungeonTower) )
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}

// 	if ( !(m_pBattleDungeonTower->Init()) )
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 		return false;
// 	}

	//	Load resources
	if (!LoadResources())
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load resources");
		return false;
	}

	//	Get host's necessary data
//	g_pGame->GetGameSession()->c2s_CmdGetAllData(true, true, false, true);

// 	if (!(m_pAchievementMan = new CECAchievementMan()) || !m_pAchievementMan->Init())
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
// 	}
	//	Load GFX
	m_pMoveTargetGFX	= g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_MOVETARGET));
	m_pSelectedGFX		= g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_SELECTED));
	m_pHoverGFX			= g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_CURSORHOVER));
	m_pFloatDust		= g_pGame->GetGFXCaster()->LoadGFXEx(res_GFXFile(RES_GFX_FLOATING_DUST));

	//	Set host's position and direction
	A3DVECTOR3 vPos(Info.pos), vNormal(0.0f);
	float fHei = g_pGame->GetGameRun()->GetWorld()->GetGroundHeight(Info.pos, &vNormal);
	//  高度差很小，认为人物在地面上，不用播放掉落的动作
	if (fabs(fHei - vPos.y) < 0.1f)
		m_GndInfo.bOnGround = true;
	else
		m_GndInfo.bOnGround = false;

	a_ClampFloor(vPos.y, fHei);
	SetPos(vPos);
	SetDirAndUp(glb_DecompressDirH(Info.dir), g_vAxisY);
	m_MoveCtrl.SetHostLastPos(vPos);
	m_MoveCtrl.SetLastSevPos(vPos);

	//	Update upper body radius using new AABB
	m_pCDRInfo->vExtent			= m_aabb.Extents;
//	m_pCDRInfo->vTPNormal		= vPos.y < fHei + 0.1f ? vNormal : g_vOrigin;
#ifdef CDR_HL2_MOVE
	m_pCDRInfo->vClipVel.Clear();
#else
	m_pCDRInfo->fYVel			= 0.0f;
#endif

	m_pCDRInfo->fSlopeThresh	= EC_SLOPE_Y;
	m_pCDRInfo->fStepHeight		= m_MoveConst.fStepHei;
	m_pCDRInfo->vAbsVelocity.Clear();
	ResetCDRInfo();
	if (m_GndInfo.bOnGround)
	{
		m_pCDRInfo->bOnSurface = true;
		m_pCDRInfo->bCanStay = true;
	}
	m_pAirCDRInfo->vExtent	= m_aabb.Extents;
	m_pAirCDRInfo->fUnderWaterDistThresh = m_MoveConst.fWaterSurf;

	//	Initialize camera control
	CECCamera::INIT_DATA InitData;
	InitData.fDistance	= 3.0f;
	InitData.vecDirCam	= g_vAxisZ;
	InitData.vecUpCam	= g_vAxisY;
	m_CameraCtrl.Init(&InitData);

	UpdateFollowCamera(false, 10);

	//	Get friend list and offline messages from server
//	g_pGame->GetGameSession()->friend_GetList();
//	g_pGame->GetGameSession()->friend_GetOfflineMsg();
	m_iGetFriendCnt = 60000;

	//get Friend call back info and ask for award
//	g_pGame->GetGameSession()->GetFriendCallBckInfo();
//	g_pGame->GetGameSession()->AskForFriendcallBckAward();

	//	Load duel images.
	if( !LoadDuelImages() )
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel images");

	//	Load sounds
	if (!LoadSounds())
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load sounds");

	//	Load direction indicators
	if (!LoadDirIndicators())
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load dir indicators");

	m_dwStates = Info.state;
	
	//	Parse following data
	BYTE* pData = (BYTE*) &Info + sizeof (S2C::cmd_self_info_1);
	if (Info.state & GP_STATE_ADV_MODE)
		pData += sizeof (int) * 2;

	//	Parse shape data
	if (Info.state & GP_STATE_SHAPE)
	{
		m_iChangeShape = *pData;
		m_iCandChangeShape = *pData;
		pData += sizeof (BYTE);
	}
	else
		m_iChangeShape = 0;

	//	Parse emote info
	if (Info.state & GP_STATE_EMOTE)
	{
		pData += sizeof (BYTE);
	}

	//	Fill extend states
	__int64 dwExtStates = 0;
	if (Info.state & GP_STATE_EXTEND_PROPERTY)
	{
		dwExtStates = *(__int64*)pData;
		pData += sizeof (__int64);
	}

	SetNewExtendStates(dwExtStates);

	//	Parse faction ID
	int idFaction = 0;
	if (Info.state & GP_STATE_FACTION)
	{
		idFaction = *(int*)pData;
		pData += sizeof (int);
	}

	SetFactionID(idFaction);

	//	Parse booth crc flag
	if (Info.state & GP_STATE_BOOTH)
	{
		pData += sizeof (BYTE);
		m_iBoothState = 2;
	}
	
	//	Parse effect data
	if (Info.state & GP_STATE_EFFECT)
	{
		BYTE byNum = *pData;
		pData += sizeof (BYTE);
		short* aEffects = (short*)pData;
		pData += byNum * sizeof (short);

		for (BYTE n=0; n < byNum; n++)
			ApplyEffect((int)aEffects[n], true);
	}

	/*
	//	Parse pariah data
	m_byPariahLvl = 0;
	if (Info.state & GP_STATE_PARIAH)
	{
		m_byPariahLvl = *pData;
		pData += sizeof (BYTE);
	}
	*/

	//	Parse mount data
	if (Info.state & GP_STATE_IN_MOUNT)
	{
		m_RidingPet.iLevel = *(unsigned char*)pData;
		pData += sizeof (char);
		m_RidingPet.id = *(int*)pData;
		pData += sizeof (int);
		m_RidingPet.bMultiRider = (*(unsigned char*)pData)? true:false;
		pData += sizeof (char);

		m_CandPet.id = m_RidingPet.id;
		m_CandPet.iLevel = m_RidingPet.iLevel;
		m_CandPet.bMultiRider = m_RidingPet.bMultiRider;
	}

	//	Parse bind data
	if (Info.state & GP_STATE_IN_BIND)
	{
		unsigned char bind_type = *(unsigned char*)pData;
		pData += sizeof (char);
		if(bind_type == 1 || bind_type == 2)
			m_idCandBuddy = *(int*)pData;
		pData += sizeof (int);
		pData += sizeof (char);
	}

	int title = 0;

	if (Info.state & GP_STATE_TITLE)
	{
		title = *(short *)pData;
		pData += sizeof(short);
	}

	if (Info.state & GP_STATE_SPOUSE)
	{
		SetSpouse(*(int*)pData);
		pData += sizeof(int);
		
		if( GetSpouse() )
		{
			// force get my relation's name, so when I want to know its name, it is already known.
			const wchar_t * szSpouseName = g_pGame->GetGameRun()->GetPlayerName(GetSpouse(), false);
			if( !szSpouseName )
			{
				int idPlayer = GetSpouse();
//				g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
			}
		}
	}

	if (Info.state & GP_STATE_FAMILY)
	{
		SetFamilyID(*(int*)pData);
		pData += sizeof (int);
		m_idFRole = *pData;
		pData++;
	}

	if (Info.state & GP_STATE_SECT_MASTER_ID)
	{
		SetMaster(*(int*)pData);
		pData += sizeof(int);
	}

	if(Info.state & GP_STATE_IN_SJBATTLE)
	{
		SetMilitaryRank(*(unsigned char*)pData);
		pData += sizeof(char);
	}
	
	if (Info.state & GP_STATE_IN_VIPSTATE)
	{
		m_iVipState = *(unsigned char*)pData;
		pData += sizeof(char);
	}
	
	if (Info.state & GP_STATE_TRANSFIGURE)
	{
		Transfigure((*(int*)pData));
		pData += sizeof(int);
	}

	//	Parse travel flag
	m_bFashionMode = false;
	if (Info.state & GP_STATE_FASHION)
		m_bFashionMode = true;

	//	Parse battle data
	m_iBattleCamp = GP_BATTLE_CAMP_NONE;
	if (Info.state & GP_STATE_BC_INVADER)
		m_iBattleCamp = GP_BATTLE_CAMP_INVADER;
	else if (Info.state & GP_STATE_BC_DEFENDER)
		m_iBattleCamp = GP_BATTLE_CAMP_DEFENDER;
	else if (Info.state & GP_STATE_IS_OBSERVER)
		m_iBattleCamp = GP_BATTLE_CAMP_OBSERVER;

	m_dwGMFlags = (Info.state & GP_STATE_GMFLAG) ? GMF_IAMGM : 0;
	m_pvp.bEnable = (Info.state & GP_STATE_PVP_ENABLE) ? true : false;
	m_pvp.bInPVPCombat = (Info.state & GP_STATE_INPVPCOMBAT) ? true : false;

	// this state will be invalid here. host player will never be in this state just login in.
	if (Info.extra_state & GP_EXTRA_STATE_CARRIER)
	{
		ASSERT(0);
//		m_idCarrier = *(int*)pData;
		pData += sizeof(int);
		pData += 3*sizeof(float);
		pData += sizeof(char);
	}

	if (Info.extra_state & GP_EXTRA_STATE_ANONYMOUS)
		m_bAnonymous = true;
 
	if (Info.extra_state & GP_EXTRA_STATE_INVISIBLE)
	{
		m_bInvisible = true;
	}
	if (Info.extra_state & GP_EXTRA_STATE_DIM)
	{
		m_bDim       = true;
	}
	if (Info.extra_state & GP_EXTRA_STATE_CLONE)
	{
	}
	if (Info.extra_state & GP_EXTRA_STATE_CIRCLE)
	{
		SetCircleID(*(int *)pData);
		pData += sizeof(int);
		SetCRole(*(char*)pData);
		pData += sizeof(char);
	}
	if (Info.extra_state & GP_EXTRA_STATE_CROSSZONE)
	{
		m_idOrgServer = *(int*)pData;
		pData += sizeof(int);
	}

	//Added 2012-01-16.
	if (Info.extra_state & GP_EXTRA_STATE_VIP_AWARD)
	{
		m_ucVIPLevel = *(unsigned char*)pData;
		pData += sizeof(unsigned char);
	}
	//Added end.

	if (Info.extra_state & GP_EXTRA_STATE_IGNITE)
		m_iSpecialState |= SPECIAL_STATE_IGNITE;
	if (Info.extra_state & GP_EXTRA_STATE_FROZEN)
		m_iSpecialState |= SPECIAL_STATE_FROZEN;
	if (Info.extra_state & GP_EXTRA_STATE_FURY)
		m_iSpecialState |= SPECIAL_STATE_FURY;
	if (Info.extra_state & GP_EXTRA_STATE_DARK)
		m_iSpecialState |= SPECIAL_STATE_DARK;

	if (title)
		SetTitle(title);

	//	Stand and do nothing
//	m_pWorkMan->StartWork_p0(m_pWorkMan->CreateWork(CECHPWork::WORK_STAND));
// 	if (IsDead())
// 	{
// 		CECHPWorkDead* pWork = (CECHPWorkDead*)m_pWorkMan->CreateWork(CECHPWork::WORK_DEAD);
// 		pWork->SetBeDeadFlag(true);
// 		m_pWorkMan->StartWork_p0(pWork);
// 	}
// 	else if (IsSitting())
// 	{
// 		CECHPWorkSit* pWork = (CECHPWorkSit*)m_pWorkMan->CreateWork(CECHPWork::WORK_SIT);
// 		pWork->SetBeSittingFlag(true);
// 		m_pWorkMan->StartWork_p1(pWork);
// 	}

	m_pSpritePortrait = new A2DSprite();
	A3DRECT rect;
	rect.left = 10;
	rect.top = 0;
	rect.right = 54;
	rect.bottom = 64;
	if( !m_pSpritePortrait->InitWithoutSurface(g_pGame->GetA3DDevice(), 64, 64, g_pGame->GetPortraitRender()->GetTextureFormat(), 1, &rect) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to create host's portrait");
		return false;
	}
	m_pSpritePortrait->SetCurrentItem(0);
	m_pSpritePortrait->SetColor(0);
	
// 	if( g_pGame->GetGameRun() && g_pGame->GetGameRun()->GetHelper() )
// 	{
// 		//load helper settings
// 		g_pGame->GetGameRun()->GetHelper()->Load(m_PlayerInfo.cid);
// 
// 		//load helper monster settings
// 		CMonsterSelector * pMonsterSelector = g_pGame->GetGameRun()->GetHelper()->GetMonsterSelector();
// 		if (pMonsterSelector)
// 		{
// 			pMonsterSelector->Load(m_PlayerInfo.cid);
// 			pMonsterSelector->LoadMonsterList(m_PlayerInfo.cid);
// 		}
// 		//load helper  pickup include and exclude list
// 		CMatterSelector * pMatterSelector = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector();
// 
// 		if (pMatterSelector)
// 		{
// 			pMatterSelector->Load(m_PlayerInfo.cid, CMatterSelector::MATTER_INCLUDE);
// 			pMatterSelector->Load(m_PlayerInfo.cid, CMatterSelector::MATTER_EXCLUDE);
// 		}
// 	}
	// 第一次进入游戏
//	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
//	if (info.lastlogin_time < (info.create_time+10))
//		g_pGame->GetGameRun()->GetSecretary()->SetReady(true);
	return true;
}



bool CECHostPlayer::LoadDuelImages()
{
	// now create some sprites for duel usage.
	m_pDuelCountDown[0] = new A2DSprite();
	if( !m_pDuelCountDown[0]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗一.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel count down 1");
		return false;
	}
	m_pDuelCountDown[0]->SetLocalCenterPos(m_pDuelCountDown[0]->GetWidth() >> 1, m_pDuelCountDown[0]->GetHeight() >> 1);
	m_pDuelCountDown[0]->SetLinearFilter(true);

	m_pDuelCountDown[1] = new A2DSprite();
	if( !m_pDuelCountDown[1]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗二.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel count down 2");
		return false;
	}
	m_pDuelCountDown[1]->SetLocalCenterPos(m_pDuelCountDown[1]->GetWidth() >> 1, m_pDuelCountDown[1]->GetHeight() >> 1);
	m_pDuelCountDown[1]->SetLinearFilter(true);

	m_pDuelCountDown[2] = new A2DSprite();
	if( !m_pDuelCountDown[2]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗三.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel count down 3");
		return false;
	}
	m_pDuelCountDown[2]->SetLocalCenterPos(m_pDuelCountDown[2]->GetWidth() >> 1, m_pDuelCountDown[2]->GetHeight() >> 1);
	m_pDuelCountDown[2]->SetLinearFilter(true);

	m_pDuelStates[0] = new A2DSprite();
	if( !m_pDuelStates[0]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗开始.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel state 1");
		return false;
	}
	m_pDuelStates[0]->SetLocalCenterPos(m_pDuelStates[0]->GetWidth() >> 1, m_pDuelStates[0]->GetHeight() >> 1);
	m_pDuelStates[0]->SetLinearFilter(true);

	m_pDuelStates[1] = new A2DSprite();
	if( !m_pDuelStates[1]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗胜利.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel state 2");
		return false;
	}
	m_pDuelStates[1]->SetLocalCenterPos(m_pDuelStates[1]->GetWidth() >> 1, m_pDuelStates[1]->GetHeight() >> 1);
	m_pDuelStates[1]->SetLinearFilter(true);

	m_pDuelStates[2] = new A2DSprite();
	if( !m_pDuelStates[2]->Init(g_pGame->GetA3DDevice(), "ingame\\决斗失败.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load duel state 3");
		return false;
	}
	m_pDuelStates[2]->SetLocalCenterPos(m_pDuelStates[2]->GetWidth() >> 1, m_pDuelStates[2]->GetHeight() >> 1);
	m_pDuelStates[2]->SetLinearFilter(true);

	m_pFactionCombatSprite = new A2DSprite();
	if( !m_pFactionCombatSprite->Init(g_pGame->GetA3DDevice(), "ingame\\帮派征战.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load faction combat sprite");
		return false;
	}
	m_pFactionCombatSprite->SetLocalCenterPos(m_pFactionCombatSprite->GetWidth() >> 1, m_pFactionCombatSprite->GetHeight() >> 1);
	m_pFactionCombatSprite->SetLinearFilter(true);

	m_pGuildWarStartSprite = new A2DSprite();
	if( !m_pGuildWarStartSprite->Init(g_pGame->GetA3DDevice(), "ingame\\开始城战.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load guild war start sprite");
		return false;
	}
	m_pGuildWarStartSprite->SetLocalCenterPos(m_pGuildWarStartSprite->GetWidth() >> 1, m_pGuildWarStartSprite->GetHeight() >> 1);
	m_pGuildWarStartSprite->SetLinearFilter(true);

	m_pGuildWarWinSprite = new A2DSprite();
	if( !m_pGuildWarWinSprite->Init(g_pGame->GetA3DDevice(), "ingame\\城战成功.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load guild war win sprite");
		return false;
	}
	m_pGuildWarWinSprite->SetLocalCenterPos(m_pGuildWarWinSprite->GetWidth() >> 1, m_pGuildWarWinSprite->GetHeight() >> 1);
	m_pGuildWarWinSprite->SetLinearFilter(true);

	m_pGuildWarLostSprite = new A2DSprite();
	if( !m_pGuildWarLostSprite->Init(g_pGame->GetA3DDevice(), "ingame\\城战失败.dds", 0) )
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load guild war lost sprite");
		return false;
	}
	m_pGuildWarLostSprite->SetLocalCenterPos(m_pGuildWarLostSprite->GetWidth() >> 1, m_pGuildWarLostSprite->GetHeight() >> 1);
	m_pGuildWarLostSprite->SetLinearFilter(true);

	
	return true;
}

void CECHostPlayer::ReleaseDuelImages()
{
	for(int i=0; i<3; i++)
	{
		A3DRELEASE(m_pDuelCountDown[i]);
		A3DRELEASE(m_pDuelStates[i]);
	}
	A3DRELEASE(m_pFactionCombatSprite);
	A3DRELEASE(m_pGuildWarStartSprite);
	A3DRELEASE(m_pGuildWarWinSprite);
	A3DRELEASE(m_pGuildWarLostSprite);
}

//	Load dir indicators
bool CECHostPlayer::LoadDirIndicators()
{
	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("armor.sdr", A3DSkinMan::SHADERREPLACE_REFLECTPREFIX);
	m_pModelTray = new CECModel();
	if( !m_pModelTray->Load("Models\\pointer\\tray.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load tray model");
		m_pModelTray->Release();
		delete m_pModelTray;
		m_pModelTray = NULL;
	}
	
	m_pModelMoveArrow = new CECModel();
	if( !m_pModelMoveArrow->Load("Models\\pointer\\pointer.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelMoveArrow->Release();
		delete m_pModelMoveArrow;
		m_pModelMoveArrow = NULL;
	}

	m_pModelTempArrow = new CECModel();
	if( !m_pModelTempArrow->Load("Models\\pointer\\pointer手.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTempArrow->Release();
		delete m_pModelTempArrow;
		m_pModelTempArrow = NULL;
	}

	m_pModelTargetArrow = new CECModel();
	if( !m_pModelTargetArrow->Load("Models\\pointer\\pointer目标.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTargetArrow->Release();
		delete m_pModelTargetArrow;
		m_pModelTargetArrow = NULL;
	}

	m_pModelTeamArrow = new CECModel();
	if( !m_pModelTeamArrow->Load("Models\\pointer\\pointer队长.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTeamArrow->Release();
		delete m_pModelTeamArrow;
		m_pModelTeamArrow = NULL;
	}

	m_pModelRequestArrow = new CECModel();
	if( !m_pModelRequestArrow->Load("Models\\pointer\\pointer令牌.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelRequestArrow->Release();
		delete m_pModelRequestArrow;
		m_pModelRequestArrow = NULL;
	}

	m_pModelMoveCursor = new CECModel();
	if( !m_pModelMoveCursor->Load("Models\\pointer\\cursor.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelMoveCursor->Release();
		delete m_pModelMoveCursor;
		m_pModelMoveCursor = NULL;
	}
	else
		m_pModelMoveCursor->PlayActionByName("动画", 1.0f, true);

	m_pModelTempCursor = new CECModel();
	if( !m_pModelTempCursor->Load("Models\\pointer\\cursor手.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTempCursor->Release();
		delete m_pModelTempCursor;
		m_pModelTempCursor = NULL;
	}
	else
		m_pModelTempCursor->PlayActionByName("动画", 1.0f, true);

	m_pModelTargetCursor = new CECModel();
	if( !m_pModelTargetCursor->Load("Models\\pointer\\cursor目标.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTargetCursor->Release();
		delete m_pModelTargetCursor;
		m_pModelTargetCursor = NULL;
	}
	else
		m_pModelTargetCursor->PlayActionByName("动画", 1.0f, true);

	m_pModelTeamCursor = new CECModel();
	if( !m_pModelTeamCursor->Load("Models\\pointer\\cursor队长.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelTeamCursor->Release();
		delete m_pModelTeamCursor;
		m_pModelTeamCursor = NULL;
	}
	else
		m_pModelTeamCursor->PlayActionByName("动画", 1.0f, true);

	m_pModelRequestCursor = new CECModel();
	if( !m_pModelRequestCursor->Load("Models\\pointer\\cursor令牌.ecm") )
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources(), Failed to load pointer model");
		m_pModelRequestCursor->Release();
		delete m_pModelRequestCursor;
		m_pModelRequestCursor = NULL;
	}
	else
		m_pModelRequestCursor->PlayActionByName("动画", 1.0f, true);

	g_pGame->GetA3DEngine()->GetA3DSkinMan()->SetReplaceShaderFile("", 0);
	return true;
}

//	Release dir indicators
bool CECHostPlayer::ReleaseDirIndicators()
{
	if( m_pModelTray )
	{
		m_pModelTray->Release();
		delete m_pModelTray;
		m_pModelTray = NULL;
	}

	if( m_pModelMoveArrow )
	{
		m_pModelMoveArrow->Release();
		delete m_pModelMoveArrow;
		m_pModelMoveArrow = NULL;
	}

	if( m_pModelTempArrow )
	{
		m_pModelTempArrow->Release();
		delete m_pModelTempArrow;
		m_pModelTempArrow = NULL;
	}

	if( m_pModelTargetArrow )
	{
		m_pModelTargetArrow->Release();
		delete m_pModelTargetArrow;
		m_pModelTargetArrow = NULL;
	}

	if( m_pModelTeamArrow )
	{
		m_pModelTeamArrow->Release();
		delete m_pModelTeamArrow;
		m_pModelTeamArrow = NULL;
	}

	if( m_pModelRequestArrow )
	{
		m_pModelRequestArrow->Release();
		delete m_pModelRequestArrow;
		m_pModelRequestArrow = NULL;
	}

	if( m_pModelMoveCursor )
	{
		m_pModelMoveCursor->Release();
		delete m_pModelMoveCursor;
		m_pModelMoveCursor = NULL;
	}

	if( m_pModelTempCursor )
	{
		m_pModelTempCursor->Release();
		delete m_pModelTempCursor;
		m_pModelTempCursor = NULL;
	}

	if( m_pModelTargetCursor )
	{
		m_pModelTargetCursor->Release();
		delete m_pModelTargetCursor;
		m_pModelTargetCursor = NULL;
	}

	if( m_pModelTeamCursor )
	{
		m_pModelTeamCursor->Release();
		delete m_pModelTeamCursor;
		m_pModelTeamCursor = NULL;
	}

	if( m_pModelRequestCursor )
	{
		m_pModelRequestCursor->Release();
		delete m_pModelRequestCursor;
		m_pModelRequestCursor = NULL;
	}

	return true;
}


//	Load host resources
bool CECHostPlayer::LoadResources()
{
	//	Calculate player's AABB
	CalcPlayerAABB();

	//	Load player skeleton
	if (!LoadPlayerSkeleton(NULL, true))
	{
		a_LogOutput(1, "CECHostPlayer::LoadResources, Failed to load skeleton");
		return false;
	}

	// 在这个时候还不知道装备信息
	memset(m_aEquips, 0xff, sizeof(m_aEquips)); 
	memset(m_aHideEquips, 0xff, sizeof(m_aHideEquips));

	//	Host's skin will be updated when inventory data (it's comming) is 
	//	got, so we neen't to update here.

	return true;
}

//	Release object
void CECHostPlayer::Release()
{
// 	if( g_pGame->GetGameRun()&&  g_pGame->GetGameRun()->GetHelper() )
// 	{
// 		//save helper settings
// 		g_pGame->GetGameRun()->GetHelper()->Save(m_PlayerInfo.cid);
// 
// 		//save helper monster settings
// 		CMonsterSelector * pMonsterSelector = g_pGame->GetGameRun()->GetHelper()->GetMonsterSelector();
// 		if (pMonsterSelector)
// 		{
// 			pMonsterSelector->Save(m_PlayerInfo.cid);
// 			pMonsterSelector->SaveMonsterList(m_PlayerInfo.cid);
// 		}
// 		//save helper matter selector include and exclude list
// 		CMatterSelector * pMatterSelector = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector();
// 		if (pMatterSelector)
// 		{
// 			pMatterSelector->Save(m_PlayerInfo.cid, CMatterSelector::MATTER_INCLUDE);
// 			pMatterSelector->Save(m_PlayerInfo.cid, CMatterSelector::MATTER_EXCLUDE);
// 		}
// 	}
	
	//	Release dir indicators
	ReleaseDirIndicators();

	//	Release duel images
	ReleaseDuelImages();

	//	Release sounds
	ReleaseSounds();

	//	Release friend manger
// 	if (m_pFriendMan)
// 	{
// 		delete m_pFriendMan;
// 		m_pFriendMan = NULL;
// 	}
// 
// 	//	Release pet corral
// 	if (m_pPetCorral)
// 	{
// 		delete m_pPetCorral;
// 		m_pPetCorral = NULL;
// 	}

	int i;

	//	Release all shortcuts
// 	for (i=0; i < NUM_HOSTSCSETS1; i++)
// 		A3DRELEASE(m_aSCSets1[i]);
// 
// 	for (i=0; i < NUM_HOSTSCSETS2; i++)
// 		A3DRELEASE(m_aSCSets2[i]);

	//	Release all inventories
//	A3DRELEASE(m_pPack);
//	A3DRELEASE(m_pEquipPack);
//	A3DRELEASE(m_pTrashBoxPack);
//	A3DRELEASE(m_pFactionTrashBoxPack);
//	A3DRELEASE(m_pTaskPack);
//	A3DRELEASE(m_pPetPack);
//	A3DRELEASE(m_pPetEquipPack);
//	A3DRELEASE(m_pDealPack);
//	A3DRELEASE(m_pEPDealPack);
//	A3DRELEASE(m_pTaskInterface);
	A3DRELEASE(m_pSpritePortrait);
//	A3DRELEASE(m_pBuyPack);
//	A3DRELEASE(m_pSellPack);
//	A3DRELEASE(m_pBoothSPack);
//	A3DRELEASE(m_pBoothBPack);
//	A3DRELEASE(m_pEPBoothSPack);
//	A3DRELEASE(m_pEPBoothBPack);
//	A3DRELEASE(m_pEPEquipPack);
//	A3DRELEASE(m_pEPPetPack);
//	A3DRELEASE(m_pEPPetEquipPack);
//	A3DRELEASE(m_pPocketPack);
//	A3DRELEASE(m_pFashionPack);
//	A3DRELEASE(m_pHTFarmPack);

// 	for (i=0; i < NUM_NPCIVTR; i++)
// 	{
// 		A3DRELEASE(m_aNPCPacks[i]);
// 	}

	//	Release all skills
	ReleaseSkills();

	//	Clear current combo skill
	ClearComboSkill();

// 	if (m_pWorkMan)
// 	{
// 		delete m_pWorkMan;
// 		m_pWorkMan = NULL;
// 	}
// 
// 	if(m_pCameraPath)
// 	{
// 		delete m_pCameraPath;
// 		m_pCameraPath= NULL;
// 	}

	m_CameraCtrl.Release();

// 	if(m_pCutscene)
// 	{
// 		delete m_pCutscene;
// 		m_pCutscene= NULL;
// 	}
// 
// 	//-------------------------------------------
// 	//Added 2012-05-10.
// 	if ( m_pOnlineAwardManager )
// 	{
// 		delete m_pOnlineAwardManager;
// 		m_pOnlineAwardManager = NULL;
// 	}
// 
// 	if ( m_pBattleDungeonTower )
// 	{
// 		delete m_pBattleDungeonTower;
// 		m_pBattleDungeonTower = NULL;
// 	}
	//-------------------------------------------

	m_aTeamInvs.RemoveAll();

	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pMoveTargetGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSelectedGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pHoverGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pFloatDust);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pCastPosSkillGFX);	//Added 2012-05-05.

	m_pMoveTargetGFX = NULL;
	m_pSelectedGFX = NULL;
	m_pHoverGFX = NULL;
	m_pFloatDust = NULL;
	m_pCastPosSkillGFX = NULL;	//Added 2012-05-05.

	//释放阵法中维护的阵法列表
	ReleaseAllCircleOfDoom(); //Added 2011-06-22.

	CECPlayer::Release();
}

void CECHostPlayer::ReleaseModels()
{
	//	Release dummy model
	ReleaseDummyModel();

	//	Release player model
	ReleasePlayerModel();

	//	Clear resource ready flags
	SetResReadyFlag(RESFG_ALL, false);
}

void CECHostPlayer::ReloadModels()
{
	LoadPlayerSkeleton(m_aEquips, true);
}

void CECHostPlayer::SetNickname(const void* p, int len)
{
	// _cp_str(m_strNickname, p, len);
}

//	Create inventories
bool CECHostPlayer::CreateInventories()
{
// 	if (!(m_pPack = new CECInventory) ||
// 		!(m_pEquipPack = new CECInventory) ||
// 		!(m_pTrashBoxPack = new CECInventory) ||
// 		!(m_pFactionTrashBoxPack = new CECInventory) ||
// 		!(m_pTaskPack = new CECInventory) ||
// 		!(m_pPetPack = new CECInventory) ||
// 		!(m_pPetEquipPack = new CECInventory) ||
// 		!(m_pDealPack = new CECDealInventory) ||
// 		!(m_pEPDealPack = new CECDealInventory) ||
// 		!(m_pBuyPack = new CECDealInventory) ||
// 		!(m_pSellPack = new CECDealInventory) ||
// 		!(m_pBoothSPack = new CECDealInventory) ||
// 		!(m_pBoothBPack = new CECDealInventory) ||
// 		!(m_pEPBoothSPack = new CECDealInventory) ||
// 		!(m_pEPBoothBPack = new CECDealInventory) ||
// 		!(m_pEPEquipPack = new CECInventory) ||
// 		!(m_pEPPetPack = new CECInventory) ||
// 		!(m_pEPPetEquipPack = new CECInventory) ||
// 		!(m_pPocketPack = new CECInventory)  ||
// 		!(m_pFashionPack = new CECInventory) ||
// 		!(m_pHTFarmPack = new CECInventory))
// 	{
// 		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::CreateInventories", __LINE__);
// 		return false;
// 	}
// 
// 	for (int i=0; i < NUM_NPCIVTR; i++)
// 	{
// 		if (!(m_aNPCPacks[i] = new CECNPCInventory) || 
// 			!m_aNPCPacks[i]->Init(IVTRSIZE_NPCPACK))
// 			return false;
// 	}
// 
// 	if (!m_pPack->Init(0) || !m_pEquipPack->Init(IVTRSIZE_EQUIPPACK) ||
// 		!m_pTaskPack->Init(IVTRSIZE_TASKPACK) || !m_pPetPack->Init(0) || !m_pPetEquipPack->Init(IVTRSIZE_PETEQUIP) || !m_pDealPack->Init(IVTRSIZE_DEALPACK) ||
// 		!m_pTrashBoxPack->Init(0) || !m_pFactionTrashBoxPack->Init(0) || !m_pEPDealPack->Init(IVTRSIZE_DEALPACK) ||
// 		!m_pBuyPack->Init(IVTRSIZE_BUYPACK) || !m_pSellPack->Init(IVTRSIZE_SELLPACK) ||
// 		!m_pBoothSPack->Init(IVTRSIZE_BOOTHSPACK) || !m_pBoothBPack->Init(IVTRSIZE_BOOTHBPACK) ||
// 		!m_pEPBoothSPack->Init(IVTRSIZE_BOOTHSPACK) || !m_pEPBoothBPack->Init(IVTRSIZE_BOOTHBPACK) ||
// 		!m_pEPEquipPack->Init(IVTRSIZE_EQUIPPACK) || !m_pEPPetPack->Init(IVTRSIZE_PETPACK) ||
// 		!m_pEPPetEquipPack->Init(IVTRSIZE_PETEQUIP) || !m_pPocketPack->Init(0) || 
// 		!m_pFashionPack->Init(IVTRSIZE_FASHIONPACK) || !m_pHTFarmPack->Init(IVTRSIZE_HTFARMPACK))
// 	{
// 		a_LogOutput(1, "CECHostPlayer::CreateInventories, Failed to initialize inventories");
// 		return false;
// 	}

	return true;
}

//	Create shortcut sets
bool CECHostPlayer::CreateShortcutSets()
{
	int i;

// 	for (i=0; i < NUM_HOSTSCSETS1; i++)
// 	{
// 		m_aSCSets1[i] = new CECShortcutSet;
// 		m_aSCSets1[i]->Init(SIZE_HOSTSCSET1);
// 	}
// 
// 	for (i=0; i < NUM_HOSTSCSETS2; i++)
// 	{
// 		m_aSCSets2[i] = new CECShortcutSet;
// 		m_aSCSets2[i]->Init(SIZE_HOSTSCSET2);
// 	}

	return true;
}

//	Load sounds
bool CECHostPlayer::LoadSounds()
{		   
	return true;

	const static char* aWalkSnds[] = 
	{
		"SFX\\Character\\FootStep\\LandWalkMaleA.wav",	// 男
		"SFX\\Character\\FootStep\\LandWalkFemaleA.wav",// 女
	};

	const static char* aRunSnds[] = 
	{
		"SFX\\Character\\FootStep\\LandRunMaleA.wav",	// 男
		"SFX\\Character\\FootStep\\LandRunFemaleA.wav", // 女
	};

	AMSoundBufferMan* pSoundBufMan = g_pGame->GetAMSoundBufferMan();
	int iIndex = m_iGender;

	//	Load move sounds
	m_aMoveSnds[MOVESND_WALK]		= pSoundBufMan->Load2DSound(aWalkSnds[iIndex]);
	m_aMoveSnds[MOVESND_RUN]		= pSoundBufMan->Load2DSound(aRunSnds[iIndex]);
	m_aMoveSnds[MOVESND_SWIM]		= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\Swim.wav");
	m_aMoveSnds[MOVESND_FLOAT]		= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\Float.wav");
	m_aMoveSnds[MOVESND_JUMP]		= NULL;
	m_aMoveSnds[MOVESND_FALL]		= NULL;
	m_aMoveSnds[MOVESND_QUIET]		= NULL;
	m_aMoveSnds[MOVESND_TIGER_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkTigerA.wav");
	m_aMoveSnds[MOVESND_TIGER_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunTigerA.wav");
	m_aMoveSnds[MOVESND_PET_HORSE_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkPetHorse.wav");
	m_aMoveSnds[MOVESND_PET_HORSE_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunPetHorse.wav");
	m_aMoveSnds[MOVESND_PET_BEAR_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkPetBear.wav");
	m_aMoveSnds[MOVESND_PET_BEAR_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunPetBear.wav");
	m_aMoveSnds[MOVESND_PET_DINASAUR_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkPetDinasaur.wav");
	m_aMoveSnds[MOVESND_PET_DINASAUR_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunPetDinasaur.wav");
	m_aMoveSnds[MOVESND_PET_PUMA_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkPetPuma.wav");
	m_aMoveSnds[MOVESND_PET_PUMA_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunPetPuma.wav");
	m_aMoveSnds[MOVESND_PET_QILIN_WALK]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandWalkPetQilin.wav");
	m_aMoveSnds[MOVESND_PET_QILIN_RUN]	= pSoundBufMan->Load2DSound("SFX\\Character\\FootStep\\LandRunPetQilin.wav");
	
	m_pCurMoveSnd = NULL;

	return true;
}

//	Release sounds
void CECHostPlayer::ReleaseSounds()
{
	int i;
	AMSoundBufferMan* pSoundBufMan = g_pGame->GetAMSoundBufferMan();

	//	Release move sounds
	for (i=0; i < NUM_MOVESND; i++)
	{
		if (m_aMoveSnds[i])
		{
			pSoundBufMan->Release2DSound(&m_aMoveSnds[i]);
			m_aMoveSnds[i] = NULL;
		}
	}

	m_pCurMoveSnd = NULL;
}

//	Play move sound
void CECHostPlayer::PlayMoveSound(int iIndex)
{
	return;
	ASSERT(iIndex >= 0 && iIndex < NUM_MOVESND);

	AMSoundBuffer* pNewBuf = m_aMoveSnds[iIndex];
	if (pNewBuf == m_pCurMoveSnd)
		return;

	if (m_pCurMoveSnd)
		m_pCurMoveSnd->Stop();

	if (pNewBuf)
		pNewBuf->Play(true);

	m_pCurMoveSnd = pNewBuf;
}

//	Update move sound
void CECHostPlayer::UpdateMoveSound()
{
	return;
	static DWORD dwUpdateCnt = 0;
	dwUpdateCnt++;

// 	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
// 	if (!pCurWork || dwUpdateCnt % 3)
// 		return;

	if (false) //(pCurWork->IsMoving())
	{
		if (m_iMoveEnv == MOVEENV_GROUND)
		{
			if (IsJumping())
				PlayMoveSound(MOVESND_JUMP);
			else if (!m_GndInfo.bOnGround)
				PlayMoveSound(MOVESND_FALL);
			else
			{
				int iSnd;
				{
					if( m_RidingPet.id )
					{
						switch(m_RidingPet.id)
						{
						case 9061:
						case 9062:
						case 9063:
							iSnd = m_bWalkRun ? MOVESND_PET_HORSE_RUN : MOVESND_PET_HORSE_WALK;
							break;

						case 9064:
							iSnd = m_bWalkRun ? MOVESND_PET_BEAR_RUN : MOVESND_PET_BEAR_WALK;
							break;

						case 9065:
							iSnd = m_bWalkRun ? MOVESND_PET_PUMA_RUN : MOVESND_PET_PUMA_WALK;
							break;

						case 9066:
							iSnd = m_bWalkRun ? MOVESND_PET_DINASAUR_RUN : MOVESND_PET_DINASAUR_WALK;
							break;

						case 9067:
						case 9068:
							iSnd = m_bWalkRun ? MOVESND_PET_QILIN_RUN : MOVESND_PET_QILIN_WALK;
							break;

						default:
							iSnd = m_bWalkRun ? MOVESND_RUN : MOVESND_WALK;
							break;
						}
					}
					else
						iSnd = m_bWalkRun ? MOVESND_RUN : MOVESND_WALK;
				}

				PlayMoveSound(iSnd);
			}
		}
		else if (m_iMoveEnv == MOVEENV_WATER)
		{
			CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
			A3DVECTOR3 vEyePos = m_CameraCoord.GetPos();
			float fWaterHei = pWorld->GetWaterHeight(vEyePos);
			PlayMoveSound(vEyePos.y > fWaterHei ? MOVESND_SWIM : MOVESND_FLOAT);
		}
		else	//	m_iMoveEnv == MOVEENV_AIR
		{
			PlayMoveSound(MOVESND_QUIET);
		}
	}
	else
		PlayMoveSound(MOVESND_QUIET);
}

CECInventory* CECHostPlayer::GetPack(int iPack)
{
	CECInventory* pInventory = NULL;
	switch (iPack)
	{
	case IVTRTYPE_PACK:				pInventory = m_pPack;					break;
	case IVTRTYPE_EQUIPPACK:		pInventory = m_pEquipPack;				break;
	case IVTRTYPE_TASKPACK:			pInventory = m_pTaskPack;				break;
	case IVTRTYPE_PET:				pInventory = m_pPetPack;				break;
	case IVTRTYPE_PET_EQUIP:		pInventory = m_pPetEquipPack;			break;
	case IVTRTYPE_TRASHBOX:			pInventory = m_pTrashBoxPack;			break;
	case IVTRTYPE_FACTIONTRASHBOX:	pInventory = m_pFactionTrashBoxPack;	break;
#ifdef LUA_UI 
	case IVTRTYPE_EPEQUIPPACK:		pInventory = m_pEPEquipPack;			break;
#endif	
	case IVTRTYPE_EPPETPACK:		pInventory = m_pEPPetPack;				break;
	case IVTRTYPE_EPPETEQUIPPACK:	pInventory = m_pEPPetEquipPack;		    break;
	case IVTRTYPE_POCKETPACK:		pInventory = m_pPocketPack;				break;
	case IVTRTYPE_FASHION:			pInventory = m_pFashionPack;			break;
	default:
		ASSERT(0);
		return NULL;
	}
	return pInventory;
}

//	Get detail data of specified item
void CECHostPlayer::GetIvtrItemDetailData(int iPack, int iSlot)
{
	CECInventory* pPack = GetPack(iPack);
	if (!pPack) return;
// 	CECIvtrItem* pItem = pPack->GetItem(iSlot);
// 	if (pItem)
// 		pItem->GetDetailDataFromSev(iPack, iSlot);
}

//	Use specified inventory item in pack
bool CECHostPlayer::UseItemInPack(int iPack, int iSlot)
{
	//	Reject this action if we are in some special states
	if (!CanDo(CANDO_USEITEM))
		return false;

	CECInventory* pPack = GetPack(iPack);
	if (!pPack) return false;
/*
	CECIvtrItem* pItem = pPack->GetItem(iSlot);
	if (!pItem || pItem->IsFrozen())
		return false;

	if (pItem->Use_Persist() && (IsJumping() || IsFalling()))
		return false;

	CECGameSession* pSession = g_pGame->GetGameSession();
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	//	Is item an equipment ?
	if (pItem->IsEquipment())
	{
//		if (iPack == IVTRTYPE_EQUIPPACK)
//		{
//			//	Take off equipment, search an empty place at first
//			int iEmpty = m_pPack->SearchEmpty();
//			if (iEmpty < 0)
//				return false;
//
//			pSession->c2s_CmdEquipItem(iEmpty, iSlot);
//			return true;
// 		}
		if (iPack == IVTRTYPE_EQUIPPACK)
		{
			// Take off equipment,
			// 优先将时装放入到时装包裹中
			int iEmpty;
			switch(iSlot)
			{
			case 	EQUIPIVTR_FASHION_EYE:
			case	EQUIPIVTR_FASHION_HEAD:
			case 	EQUIPIVTR_FASHION_BODY:
			case	EQUIPIVTR_FASHION_FOOT:
				iEmpty = m_pFashionPack->SearchEmpty();
				if(iEmpty<0)
				{
					iEmpty = m_pPack->SearchEmpty();
					if(iEmpty <0)
						return false;
					pSession->c2s_CmdEquipItem(iEmpty, iSlot);
				}
				else
				{
					pSession->c2s_CmdExgEquFashionItem(iSlot, iEmpty);
				}
				break;
			default:
				iEmpty = m_pPack->SearchEmpty();
				if(iEmpty < 0)
					return false;
				pSession->c2s_CmdEquipItem(iEmpty, iSlot);
				break;
			}
		}

		//	Can equip this equipment ?
		if (!CanUseEquipment((CECIvtrEquip*)pItem))
			return false;

		//	Find the first free slot this equipment can be equipped
		int i, iDst, iFirstFree=-1, iFirstCan=-1;
		for (i=0; i < SIZE_EQUIPIVTR; i++)
		{
			if (pItem->CanEquippedTo(i))
			{
				if (iFirstCan < 0)
					iFirstCan = i;

				if (!m_pEquipPack->GetItem(i) && iFirstFree < 0)
				{
					iFirstFree = i;
					break;
				}
			}
		}

		if (iFirstFree >= 0)
			iDst = iFirstFree;
		else if (iFirstCan >= 0)
			iDst = iFirstCan;
		else
		{
			ASSERT(0);
			return false;
		}
		if(iPack == IVTRTYPE_PACK)
			pSession->c2s_CmdEquipItem(iSlot, iDst);
		else if(iPack == IVTRTYPE_FASHION)
			pSession->c2s_CmdExgEquFashionItem(iDst, iSlot);
		return true;
	}

	//	Is item an pet equipment ?
	if (pItem->GetClassID() == CECIvtrItem::ICID_PET_ARMOR )
	{
		if (iPack == IVTRTYPE_PACK)
		{
			CECPetArmor *pPetArmor = (CECPetArmor*)pItem;
			pSession->c2s_CmdPetEquip(iSlot, pPetArmor->GetDBEssence()->equip_location);
			return true;
		}
		else if (iPack == IVTRTYPE_PET_EQUIP)
		{
			int iEmpty = m_pPack->SearchEmpty();
			if (iEmpty < 0)
				return false;

			pSession->c2s_CmdPetEquip(iEmpty, iSlot);
			return true;
		}
	}

	//	Is item an pet bedge ?
	if (pItem->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
	{
		if (iPack == IVTRTYPE_PACK)
		{
			int iEmpty = m_pPetPack->SearchEmpty();
			if (iEmpty < 0)
				return false;

			pSession->c2s_CmdPetEquipBedge(iSlot, iEmpty);
			return true;
		}
		else if (iPack == IVTRTYPE_PET)
		{
			int iEmpty = m_pPack->SearchEmpty();
			if (iEmpty < 0)
				return false;

			pSession->c2s_CmdPetEquipBedge(iEmpty, iSlot);
			return true;
		}
	}

	//	Is item an pet food?
	if (pItem->GetClassID() == CECIvtrItem::ICID_PET_FOOD )
	{
		pSession->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)iSlot,
			pItem->GetTemplateID(), &m_nCurPetIndex, sizeof(int));

		return true;
	}

	//	Is item an pet food?
	if (pItem->GetClassID() == CECIvtrItem::ICID_PET_AUTO_FOOD )
	{
		struct auto_food_data
		{
			int index;
			int mask;
		};
		auto_food_data autofood;
		autofood.index = m_nCurPetIndex;
		autofood.mask = 0x7;
		pSession->c2s_CmdUseItemWithArg(IVTRTYPE_PACK, 1, (unsigned short)iSlot,
			pItem->GetTemplateID(), &autofood, sizeof(auto_food_data));

		return true;
	}
	
	if(pItem->GetClassID() == CECIvtrItem::ICID_MEDICINE)
	{
		CECIvtrMedicine* pMedicine = (CECIvtrMedicine*)pItem;
		int nMax;
		int type = pMedicine->GetDBEssence()->type;
		if (type >= 0 && type <= 5 && GetCoolTime(GP_CT_HP + type, &nMax))
		{
			if(nMax > 200) // hp/mp药品200ms以下冷却不提示，防外挂用
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_HP_COOLTIME + type);
			return false;
		}
		if (type > 5 && GetCoolTime(GP_CT_DP + type - 6))
		{
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_DP_COOLTIME + type - 6);
			return false;
		}

		if (m_TransfigureInfo.id && (!m_TransfigureProp.bCanUseMedicine))
		{
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_FORBID_MEDICINE);
		}
		LuaEvent::FirstUseMedicine();
	}

	
	if(pItem->GetClassID() == CECIvtrItem::ICID_TELEPORTATION)
	{
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->CommonMessage("Win_MarkAstroList", "show_mark", iSlot, (long)pItem);
	}
	
	if (iPack != IVTRTYPE_PACK)
		return false;	//	Non-equipment must be in normal package 

	// 如果是技能物品，进行缓存，待玩家选定释放的目标后再释放
	if (pItem->GetClassID() == CECIvtrItem::ICID_SKILLMATTER)
	{
		const SKILLMATTER_ESSENCE* pEssence = ((CECIvtrSkillMat*)pItem)->GetDBEssence();
		if(pEssence->cast_skill)
		{
			m_ApoiseSkill.idSkill		= pEssence->id_skill;
			m_ApoiseSkill.iSkillLvl		= pEssence->level_skill;
			m_ApoiseSkill.iItemSlot		= iSlot;
			m_ApoiseSkill.tidItem		= pEssence->id;
			ApplySkillShortcut(m_ApoiseSkill.idSkill);
			return true;	
		}
	}
	
	if (m_iVipState != NO_VIP_STATE && pItem->GetClassID() == CECIvtrItem::ICID_VIP_CARD)
	{
		pGameRun->AddFixedMessage(FIXMSG_ALREADY_VIPSTATE);
		return false;
	}

	if (!pItem->CheckUseCondition())
	{
		pGameRun->AddFixedMessage(FIXMSG_ITEM_CANNOTUSE);
		return false;
	}

	if (pItem->GetCoolTime())
	{
		pGameRun->AddFixedMessage(FIXMSG_ITEM_INCOOLTIME);
		return false;
	}

	//	Using item to attack a target ?
	if (pItem->Use_AtkTarget() || pItem->Use_Target())
	{
		if (pItem->Use_AtkTarget() && CannotAttack())
			return false;

		CECStringTab* pStrTab = g_pGame->GetFixedMsgTab();

		if (!m_idSelTarget || m_idSelTarget == m_PlayerInfo.cid)
		{
			pGameRun->AddChatMessage(pStrTab->GetWideString(FIXMSG_NOTARGET), GP_CHAT_SYSTEM);
			return false;
		}

		//	Check whether the target is too far away ?
		//	TODO: Should we completely depend on server to tell us this ?
		float fDist, fTargetRad, fAttackRange = 10000.0f;
	//	if (pItem->GetClassID() == CECIvtrItem::ICID_TOSSMAT)
	//		fAttackRange = ((CECIvtrTossMat*)pItem)->GetDBEssence()->attack_range;
	//	else if (pItem->GetClassID() == CECIvtrItem::ICID_TANKCALLIN)
	//		fAttackRange = 5.0f;

		CECObject* pObject;
		if (!CalcDist(m_idSelTarget, &fDist, &pObject))
			return false;

		if (ISNPCID(m_idSelTarget))
			fTargetRad = ((CECNPC*)pObject)->GetTouchRadius();
		else if (ISPLAYERID(m_idSelTarget))
			fTargetRad = ((CECElsePlayer*)pObject)->GetTouchRadius();

		if (fDist - fTargetRad > fAttackRange * 0.8f)
		{
			pGameRun->AddChatMessage(pStrTab->GetWideString(FIXMSG_TARGETISFAR), GP_CHAT_SYSTEM);
			return false;
		}

		BYTE byPVPMask = BuildPVPMask(GetForceAttackFlag(NULL));
		pSession->c2s_CmdUseItemWithTarget((BYTE)iPack, (BYTE)iSlot, pItem->GetTemplateID(), byPVPMask);
	}
	else
	{
		if( pItem->GetClassID() == CECIvtrItem::ICID_ARTILLERY_SCROLL )
			pSession->c2s_CmdUseItemWithTarget((BYTE)iPack, (BYTE)iSlot, pItem->GetTemplateID(), 0);
		else
			pSession->c2s_CmdUseItem((BYTE)iPack, (BYTE)iSlot, pItem->GetTemplateID(), 1);
	}
	*/
	return true;
}

//	Apply shortcut of specified skill
bool CECHostPlayer::ApplySkillShortcut(int idSkill, bool bCombo/* false */, 
						int idSelTarget/* 0 */, int iForceAtk/* -1 */, CECSkill *pForceSkill/*=NULL*/, A3DVECTOR3* pCastSkillPos/* = NULL*/)
{
	return true;
}
bool CECHostPlayer::ApplySkillShortcutUp(int idSkill, bool bCombo/* false */, 
								int idSelTarget/* 0 */, int iForceAtk/* -1 */)
{
	return true;
}
//	Return to a target town through skill
bool CECHostPlayer::ReturnToTargetTown(int idTarget)
{
	return true;
}

//	Stop host moving naturally, return true host stopped, otherwise return false
bool CECHostPlayer::NaturallyStopMoving()
{
	return true;
}

//	Tick routine
bool CECHostPlayer::Tick(DWORD dwDeltaTime)
{
	if (!IsSkeletonReady())
		return true;
	//  解决领域技能导致的主角骑乘状态下的抖动问题
	if (m_bHasCachePos)
	{
		m_bHasCachePos = false;
		SetPos(m_vCachePos);
	}

	CECPlayer::Tick(dwDeltaTime);

	int iRealTime = g_pGame->GetRealTickTime();
	
	//	If host player is standing on a carrier, apply carrier's pose change to host
	m_StartPosPair.bRecorded = false;
	m_EndPosPair.bRecorded = false;
	if (m_idCarrier)
	{
		A3DVECTOR3 vPos, vDir;
		CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();

		if (pNPCMan->GetPoseOnCarrier(m_idCarrier, m_matRelCarrier, vPos, vDir))
		{
			SetPos(vPos);
			SetDirAndUp(vDir, g_vAxisY);
			m_StartPosPair.bRecorded = true;
		}

		m_StartPosPair.vAbsPos = vPos + g_vAxisY * m_aabb.Extents.y;
		m_StartPosPair.vRelPos = m_matRelCarrier.GetRow(3) + g_vAxisY * m_aabb.Extents.y;
		m_StartPosPair.iCarrierID = m_idCarrier;
	}

	//	Estimate cursor
	EstimateCursor();

	m_MoveCtrl.Tick(dwDeltaTime);

	//	Update selected target
	UpdateSelectedTarget();

	//	Estimatie moving environment
	//EstimateMoveEnv(GetPos() + g_vAxisY * 0.2f);
	//@note :  By Kuiwu[12/10/2005]
	EstimateMoveEnv(GetPos());

	//	Work tick routine
// 	if (!m_bChangingFace && (m_pCutscene && !m_pCutscene->IsPlayingCutscene()))
// 	{
// 		CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
// 		if (pCurWork)
// 		{
// 			pCurWork->Tick(dwDeltaTime);
// 			if (pCurWork->IsFinished())
// 				m_pWorkMan->FinishCurWork(-1, false);
// 		}
// 	}
// 
// 	//	Update task
// 	if (m_pTaskInterface && m_TaskCounter.IncCounter(dwDeltaTime))
// 	{
// 		m_TaskCounter.Reset();
// 		OnTaskCheckStatus(m_pTaskInterface);
// 	}

	//	Update timers
	UpdateTimers(dwDeltaTime);

	//	Update shortcuts
	UpdateShortcuts(dwDeltaTime);

// 	if (gr.get_state() == game_record::enum_gr_replay)
// 		SetPos(gr.get_host_pos());

	
//	m_pCutscene->Tick(dwDeltaTime);

	//	The Online Award Manager Tick Procedure. Added 2012-03-23.
//	m_pOnlineAwardManager->Tick( dwDeltaTime );

//	m_pCameraPath->Tick(dwDeltaTime);
	//	Adjust camera's position
// 	if( m_pCutscene->IsPlayingCutscene())
// 	{
// 		m_CameraCoord.SetPos(m_pCutscene->GetCameraPath()->GetPos());
// 		m_CameraCoord.SetDirAndUp(m_pCutscene->GetCameraPath()->GetDir(), g_vAxisY);
// 	}
// 	else
// 	{
// 		if (!IsChangingFace())
// 			UpdateFollowCamera(m_iMoveMode != MOVE_STAND ? true : false, dwDeltaTime);
// 	}
	

	if (m_iBoothState != 2)
	{
		//如果正处于归神/脱灵技能并且角色不在坐骑上，那么重设模型的位置Added 2011-06-23.
		if ( !IS_EQUAL(m_fScaleShapeRatio,0.0f) && !m_pPetModel/*&& m_GndInfo.bOnGround*/ )
		{
//			float fTerrainHeight = g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetPosHeight( GetPos() );
//			float fDeltaHeight = fTerrainHeight - m_pPlayerModel->GetModelAABB().Mins.y;//GetA3DSkinModel()->GetModelAABB()
				
//			if ( fabsf(fDeltaHeight) > 0.005f ) //由于角色模型的骨骼中心点问题，在缩放状态下，直接修改模型位置，无需判断地形高度Canceled 2011-09-06.
			{
//				A3DVECTOR3 vCurPos = GetPos();
//				vCurPos.y = fTerrainHeight + 0.001f;
				if ( m_pPlayerModel ) //模型指针存在，则按照公式设置模型位置，但HostPlayer的位置不变
				{
					m_pPlayerModel->SetPos(GetPos() + A3DVECTOR3(0.0f, (m_fScaleShapeRatio - 1.0f)*(m_aabb.Extents.y+0.14f), 0.0f));//vCurPos
			
				}
			}
		}

		//Added 2011-06-17.
		DWORD dwDeltaTimeTmp = dwDeltaTime;
		if ( !IS_EQUAL(m_fSkillSpeedRate,1.0f) ) //效率起见，做此判断
		{
			float fDeltaTime = (float)dwDeltaTime * m_fSkillSpeedRate;
			dwDeltaTime = (DWORD)fDeltaTime;
		}
		//Added end.
		if (m_AttachMode != enumAttachNone && m_bHangerOn)
		{
		}
		else if (m_pTransfiguredModel)
			m_pTransfiguredModel->Tick(dwDeltaTime);	
		else if (m_pChangedModel)
			m_pChangedModel->Tick(dwDeltaTime);
		else if (m_pPetModel)
			m_pPetModel->Tick(dwDeltaTime);
		else if (m_pPlayerModel)
		{
			m_pPlayerModel->Tick(dwDeltaTime);
			if( IsShapeChanged() )
			{
				if( m_pBackupModel )
					m_pBackupModel->Tick(dwDeltaTime);
			}
		}
		//restore the value.
		dwDeltaTime = dwDeltaTimeTmp;
		//restore operation end.

		//	Update move sound
		UpdateMoveSound();
	}

	//	Update GFXs
	UpdateGFXs(dwDeltaTime);

	//	Update the GFX of circle-of-doom. Added 2011-06-22.
	UpdateCircleOfDoomGFXs(dwDeltaTime);

	//	Update the GFX of Pull skill. Added 2011-08-03.
	UpdatePullGfxs(dwDeltaTime);

	//	Update the NO-TARGET skill states. Added 2012-05-05.
	UpdateNoTargetSkillStates();

	//	Update team leader position
	UpdateTeamMemberPos((DWORD)iRealTime);

	//	Update pet skill cooldown
	UpdatePetSkillCoolDown(dwDeltaTime);

	m_TrickCnt.IncCounter(dwDeltaTime);
	if (m_pvp.dwCoolTime > (DWORD)iRealTime)
		m_pvp.dwCoolTime -= (DWORD)iRealTime;
	else
		m_pvp.dwCoolTime = 0;

	if( m_nTimeToExitInstance > iRealTime )
		m_nTimeToExitInstance -= iRealTime;
	else
		m_nTimeToExitInstance = 0;

	if(m_pBackupModel)
	{
		A3DBone * pBone = m_pBackupModel->GetA3DSkinModel()->GetSkeleton()->GetBone("Bip01 Pelvis", NULL); 
		if(pBone)
		{
			A3DVECTOR3 vecPos = pBone->GetAbsoluteTM().GetRow(3) + A3DVECTOR3(0.0f, 0.1f, 0.0f);
			if( m_pModelTray )
			{
				m_pModelTray->SetPos(vecPos);
				m_pModelTray->SetDirAndUp(g_vAxisZ, g_vAxisY);
				m_pModelTray->Tick(dwDeltaTime);
			}
		}
	}

	A3DVECTOR3 vecPos = GetPos() + A3DVECTOR3(0.0f, 1.0f, 0.0f);
	if( m_pModelMoveArrow && m_pModelMoveCursor && m_bShowMoveArrow )
	{
		A3DVECTOR3 vecDir = m_vecMoveArrow;
		vecDir.y = 0.0f;
		float vDis = MagnitudeH(vecPos - m_vecMovePos);

		m_pModelMoveArrow->SetPos(vecPos);
		m_pModelMoveArrow->SetDirAndUp(Normalize(vecDir), g_vAxisY);
		m_pModelMoveArrow->Tick(dwDeltaTime);
		if( vDis < FADE_DIS2 )
			m_pModelMoveArrow->Show(false);
		else
			m_pModelMoveArrow->Show(true);

		if( vDis < FADE_DIS )
		{
			m_pModelMoveCursor->ScaleBone("Bone01", CECModel::SCALE_WHOLE, A3DVECTOR3(2.0f + vDis/FADE_DIS * 4.0f));
			m_pModelMoveCursor->Show(true);
			m_pModelMoveCursor->SetPos(m_vecMovePos);
			m_pModelMoveCursor->Tick(dwDeltaTime);
		}
		else
		{
			m_pModelMoveCursor->Show(false);
		}
	}

	if( m_pModelTempArrow && m_pModelTempCursor && m_bShowTempArrow )
	{
		A3DVECTOR3 vecDir = m_vecTempArrow;
		vecDir.y = 0.0f;
		float vDis = MagnitudeH(vecPos - m_vecTempPos);

		m_pModelTempArrow->SetPos(vecPos);
		m_pModelTempArrow->SetDirAndUp(Normalize(vecDir), g_vAxisY);
		m_pModelTempArrow->Tick(dwDeltaTime);
		if( vDis < FADE_DIS2 )
			m_pModelTempArrow->Show(false);
		else
			m_pModelTempArrow->Show(true);

		if( vDis < FADE_DIS )
		{
			m_pModelTempCursor->ScaleBone("Bone01", CECModel::SCALE_WHOLE, A3DVECTOR3(2.0f + vDis/FADE_DIS * 4.0f));
			m_pModelTempCursor->Show(true);
			m_pModelTempCursor->SetPos(m_vecTempPos);
			m_pModelTempCursor->Tick(dwDeltaTime);
		}
		else
		{
			m_pModelTempCursor->Show(false);
		}
	}

	if( m_pModelTeamArrow && m_pModelTeamCursor && m_bShowTeamArrow )
	{
		A3DVECTOR3 vecDir = m_vecTeamArrow;
		vecDir.y = 0.0f;
		float vDis = MagnitudeH(vecPos - m_vecTeamPos);

		m_pModelTeamArrow->SetPos(vecPos);
		m_pModelTeamArrow->SetDirAndUp(Normalize(vecDir), g_vAxisY);
		m_pModelTeamArrow->Tick(dwDeltaTime);
		if( vDis < FADE_DIS2 )
			m_pModelTeamArrow->Show(false);
		else
			m_pModelTeamArrow->Show(true);
		
		if( vDis < FADE_DIS )
		{
			m_pModelTeamCursor->ScaleBone("Bone01", CECModel::SCALE_WHOLE, A3DVECTOR3(2.0f + vDis/FADE_DIS * 4.0f));
			m_pModelTeamCursor->Show(true);
			m_pModelTeamCursor->SetPos(m_vecTeamPos);
			m_pModelTeamCursor->Tick(dwDeltaTime);
		}
		else
		{
			m_pModelTeamCursor->Show(false);
		}
	}

	if( m_pModelRequestArrow && m_pModelRequestArrow && m_bShowRequestArrow )
	{
		A3DVECTOR3 vecDir = m_vecRequestArrow;
		vecDir.y = 0.0f;
		float vDis = MagnitudeH(vecPos - m_vecRequestPos);

		m_pModelRequestArrow->SetPos(vecPos);
		m_pModelRequestArrow->SetDirAndUp(Normalize(vecDir), g_vAxisY);
		m_pModelRequestArrow->Tick(dwDeltaTime);
		if( vDis < FADE_DIS2 )
			m_pModelRequestArrow->Show(false);
		else
			m_pModelRequestArrow->Show(true);
		
		if( vDis < FADE_DIS )
		{
			m_pModelRequestCursor->ScaleBone("Bone01", CECModel::SCALE_WHOLE, A3DVECTOR3(2.0f + vDis/FADE_DIS * 4.0f));
			m_pModelRequestCursor->Show(true);
			m_pModelRequestCursor->SetPos(m_vecRequestPos);
			m_pModelRequestCursor->Tick(dwDeltaTime);
		}
		else
		{
			m_pModelRequestCursor->Show(false);
		}
	}

	if( m_pModelTargetArrow && m_pModelTargetArrow )
	{
		m_pModelTargetArrow->Tick(dwDeltaTime);
		m_pModelTargetCursor->Tick(dwDeltaTime);
	}

	m_fVertSpeed	= 0.0f;

	//	Save relative pose if host is on a carrier
	if (m_idCarrier)
	{
		CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();
		pNPCMan->CalcCarrierRelMat(m_idCarrier, GetAbsoluteTM(), m_matRelCarrier);

		if ((GetPos() == m_StartPosPair.vAbsPos - g_vAxisY * m_aabb.Extents.y) && m_StartPosPair.bRecorded && (m_StartPosPair.iCarrierID == m_idCarrier))
		{
			// 如果没有位移, 将相对位移坐标设回Tick开始保存的那个，以免长时间来回变化累计误差
			m_matRelCarrier.SetRow(3, m_StartPosPair.vRelPos - g_vAxisY * m_aabb.Extents.y);
		}

		if ((GetPos() == m_EndPosPair.vAbsPos - g_vAxisY * m_aabb.Extents.y) && m_EndPosPair.bRecorded && (m_EndPosPair.iCarrierID == m_idCarrier))
		{
			m_matRelCarrier.SetRow(3, m_EndPosPair.vRelPos - g_vAxisY * m_aabb.Extents.y);			
		}
	}

	if(m_pTransfiguredModel && m_TransfigureProp.iMoveType == TRANSFIGURE_MOVE_RANDOM)
	{
		if (!m_RandomMoveCnt.IsFull())
		{
			m_RandomStopCnt.SetPeriod(a_Random(100,2000));
			m_RandomStopCnt.Reset();
			g_pGame->GetGameRun()->PostMessage(MSG_HST_RANDOMMOVE, MAN_PLAYER, 0, m_iRandomMoveDir);
		}

		if (m_RandomStopCnt.IsFull())
		{
			m_RandomMoveCnt.SetPeriod(a_Random(100,2000));
			m_RandomMoveCnt.Reset();
			m_iRandomMoveDir = a_Random(0,7);
		}
	}
	
	return true;
}

void CECHostPlayer::SyncDemonstrationPlayer(CECLoginPlayer* pPlayer, int iFashion)
{
// 	if (m_bLoadFlag)
// 	{
// 		pPlayer->UpdateFace(m_iFaceid, m_iHairid, m_iDefaultSkin);
// 		pPlayer->UpdateLieShan(m_iEarid, m_iTailid);
// 		pPlayer->SyncBasicProps(m_BasicProps);
// 		pPlayer->UpdateEquipments(m_aEquips, iFashion>=0 ? iFashion>0 : m_bFashionMode);
// 	}
}

//	Render routine
bool CECHostPlayer::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	m_PateContent.iVisible = 0;

	if (!IsAllResReady())
		return true;

	if( !IsClothesOn() )
		return true;

#ifdef BACK_VERSION
	A3DTerrainWater * pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#else
	A3DTerrainWater2* pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#endif
	if( iRenderFlag == RD_REFLECT && pTerrainWater->IsUnderWater(GetPlayerAABB().Maxs) )
		return true;
	else if( iRenderFlag == RD_REFRACT && !pTerrainWater->IsUnderWater(GetPlayerAABB().Mins) )
		return true;

	if ((m_AttachMode != enumAttachNone && m_bHangerOn) || m_bMultiRideMember)
	{
	}
	else
	{
		CECModel* pModel = m_pPetModel ? m_pPetModel : m_pPlayerModel;

		if(m_pTransfiguredModel)
			pModel = m_pTransfiguredModel;
		else if (m_pChangedModel)
			pModel = m_pChangedModel;

		if (pModel)
		{
			if( iRenderFlag == RD_BLOOM )
			{
				RenderEquipBloom(pViewport);
				return true;
			}
			else
				pModel->Render(pViewport->GetA3DViewport());

#ifdef BACK_VERSION
			if (m_bCastShadow && g_pGame->GetShadowRender())
			{
				CECPlayer * pPlayer = this;

				SHADOW_RECEIVER receiver = SHADOW_RECEIVER_TERRAIN;
				if( iRenderFlag == RD_NORMAL )
					receiver = (SHADOW_RECEIVER) (receiver | SHADOW_RECEIVER_LITMODEL);
				//-------------------------------------------------------------
				//由于在人物坐骑过大时，传入ShadowMap的aabb过于小，使得shadow显示不正常，
				//所以在传入aabb时进行相应判断 Added 2012-03-30.
				A3DAABB aabb = m_aabb;
				if ( pModel->GetA3DSkinModel() )
				{
					bool bUpdate = false;
					A3DAABB aabbTmp = pModel->GetA3DSkinModel()->GetModelAABB();
					if ( aabb.Extents.x < aabbTmp.Extents.x )
					{
						aabb.Extents.x = aabbTmp.Extents.x;
						bUpdate = true;
					}
					if ( aabb.Extents.y < aabbTmp.Extents.y )
					{
						aabb.Extents.y = aabbTmp.Extents.y;
						bUpdate = true;
					}
					if ( aabb.Extents.z < aabbTmp.Extents.z )
					{
						aabb.Extents.z = aabbTmp.Extents.z;
					}
					if ( bUpdate )
					{
						aabb.CompleteMinsMaxs();
					}
				}
				//-------------------------------------------------------------
				g_pGame->GetShadowRender()->AddShadower(aabb.Center, aabb, receiver, PlayerRenderForShadow, pPlayer, false);//m_aabb.Center, m_aabb
			}
#endif
		}
	}

	if (iRenderFlag == RD_NORMAL)
	{
		CECConfigs* pConfigs = g_pGame->GetConfigs();

		if (m_bRenderBar)
			RenderBars(pViewport);

		if (m_bRenderName)
		{
			DWORD dwFlags = pConfigs->GetPlayerTextFlags();
			if (pConfigs->GetVideoSettings().bPlayerSelfName)
				dwFlags |= RNF_NAME;
			else
				dwFlags &= ~RNF_NAME;

			RenderName(pViewport, dwFlags);
		}

		//	Print host's precise position
		if (g_pGame->GetConfigs()->GetShowPosFlag())
		{
			ACHAR szMsg[100];
			A3DVECTOR3 vPos = GetPos();
			a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), vPos.x, vPos.y, vPos.z);
			int y = pViewport->GetA3DViewport()->GetParam()->Height - 20;
			vPos += g_vAxisY * (m_aabb.Extents.y * 3.0f);
			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);
		}

		//	Show direction indicator if needs.
		if( m_bShowMoveArrow || m_bShowTempArrow || m_bShowTeamArrow || m_bShowRequestArrow ||
			m_vecTargetArrows.GetSize() > 0 )
		{
			if( m_pModelTray )
				m_pModelTray->Render(pViewport->GetA3DViewport());
		}
		if( m_bShowMoveArrow && m_pModelMoveArrow && m_pModelMoveCursor )
		{
			m_pModelMoveArrow->Render(pViewport->GetA3DViewport());
			m_pModelMoveCursor->Render(pViewport->GetA3DViewport());
		}
		if( m_bShowTempArrow && m_pModelTempArrow && m_pModelTempCursor )
		{
			m_pModelTempArrow->Render(pViewport->GetA3DViewport());
			m_pModelTempCursor->Render(pViewport->GetA3DViewport());
		}
		if( m_bShowTeamArrow && m_pModelTeamArrow && m_pModelTeamCursor )
		{
			m_pModelTeamArrow->Render(pViewport->GetA3DViewport());
			m_pModelTeamCursor->Render(pViewport->GetA3DViewport());
		}
		if( m_bShowRequestArrow && m_pModelRequestArrow && m_pModelRequestCursor )
		{
			m_pModelRequestArrow->Render(pViewport->GetA3DViewport());
			m_pModelRequestCursor->Render(pViewport->GetA3DViewport());
		}

		// enable alpha blend, because target arrows and cursors may fade
		for(int i=0; i<m_vecTargetArrows.GetSize(); i++)
		{
			const A3DCOLOR colors[5] = { 0xFF00D3FF, 0xFF00FFDA, 0xFFF300FF, 0xFFFF2800, 0xFF0C51C2 };

			if( m_pModelTargetArrow && m_pModelTargetCursor )
			{
				A3DVECTOR3 vecDir  = m_vecTargetArrows[i];
				vecDir.y = 0.0f;
				float vDis = MagnitudeH(GetPos() - m_vecTargetPos[i]);

				m_pModelTargetArrow->SetPos(GetPos() + A3DVECTOR3(0.0f, 1.0f, 0.0f));
				m_pModelTargetArrow->SetDirAndUp(Normalize(vecDir), g_vAxisY);
				if( vDis < FADE_DIS2 )
					m_pModelTargetArrow->Show(false);
				else
					m_pModelTargetArrow->Show(true);
				m_pModelTargetArrow->Tick(0);
				//m_pModelTargetArrow->GetA3DSkinModel()->SetMaterialScale(A3DCOLORVALUE(colors[min(i, 4)]));
				//m_pModelTargetArrow->GetA3DSkinModel()->SetMaterialMethod(A3DSkinModel::MTL_SCALED);
				m_pModelTargetArrow->Render(pViewport->GetA3DViewport(), true);  				
				
				if( vDis < FADE_DIS )
				{
					m_pModelTargetCursor->ScaleBone("Bone01", CECModel::SCALE_WHOLE, A3DVECTOR3(2.0f + vDis/FADE_DIS * 4.0f));
					m_pModelTargetCursor->Show(true);
					m_pModelTargetCursor->SetPos(m_vecTargetPos[i]);
					m_pModelTargetCursor->Tick(0);
					//m_pModelTargetCursor->GetA3DSkinModel()->SetMaterialScale(A3DCOLORVALUE(colors[min(i, 4)]));
					//m_pModelTargetCursor->GetA3DSkinModel()->SetMaterialMethod(A3DSkinModel::MTL_SCALED);
					m_pModelTargetCursor->Render(pViewport->GetA3DViewport(), true);
				}
				else
					m_pModelTargetCursor->Show(false);
			}
		}
	}

	if (iRenderFlag != RD_BLOOM && !m_pTransfiguredModel) // 变身情况下不显示法宝光效
	{
		if (m_pSprite)
		{
			SECT_LEVEL2_INFO* pInfo = GetSectInfoByProf(m_BasicProps.iProfession);
			// 怀光变身情况下不显示法宝
			if (IsShapeChanged() && pInfo && (pInfo->mask_sect & (1<<SECT_13)) )
			{

			}
			else 
				m_pSprite->Render(pViewport);
		}
	}

	return true;
}

//	Render when player is opening booth
bool CECHostPlayer::RenderForBooth(CECViewport* pViewport, int iRenderFlag/* 0 */)
{
	m_PateContent.iVisible = 0;

	if (iRenderFlag == RD_REFLECT || iRenderFlag == RD_REFRACT)
		return true;

	A3DSkinModel* pBoothModel = m_pPlayerMan->GetBoothModel(m_iGender);
	if (pBoothModel)
	{
		pBoothModel->SetPos(GetPos());
		pBoothModel->SetDirAndUp(GetDir(), GetUp());
		pBoothModel->Update(g_pGame->GetTickTime());
#ifdef BACK_VERSION
		pBoothModel->RenderAtOnce(pViewport->GetA3DViewport(), 0);
#else
		pBoothModel->Render(pViewport->GetA3DViewport(), 0);
#endif
	}

	CECConfigs* pConfigs = g_pGame->GetConfigs();

	if (m_bRenderBar)
		RenderBars(pViewport);

	if (m_bRenderName)
	{
		DWORD dwFlags = pConfigs->GetPlayerTextFlags();
		if (pConfigs->GetVideoSettings().bPlayerSelfName)
			dwFlags |= RNF_NAME;
		else
			dwFlags &= ~RNF_NAME;

		RenderName(pViewport, dwFlags);
	}

	//	Print host's precise position
	if (g_pGame->GetConfigs()->GetShowPosFlag())
	{
		ACHAR szMsg[100];
		A3DVECTOR3 vPos = GetPos();
		a_sprintf(szMsg, _AL("%.3f, %.3f, %.3f"), vPos.x, vPos.y, vPos.z);
		int y = pViewport->GetA3DViewport()->GetParam()->Height - 20;
		vPos += g_vAxisY * (m_aabb.Extents.y * 3.0f);
		g_pGame->GetA3DEngine()->GetSystemFont()->TextOut3D(szMsg, pViewport->GetA3DViewport(), vPos, 0, 0xffffffff);
	}

	return true;
}

bool CECHostPlayer::RenderForUI(CECViewport * pViewport)
{
	if( !HostIsReady() )
		return true;

	float	f;
	int		n;
	int		t;
	int		l;

	int x = pViewport->GetA3DViewport()->GetParam()->X + (pViewport->GetA3DViewport()->GetParam()->Width >> 1);
	int y = pViewport->GetA3DViewport()->GetParam()->Y + (pViewport->GetA3DViewport()->GetParam()->Height >> 1);
	
	float sf = pViewport->GetA3DViewport()->GetParam()->Width * 1.0f / 800.0f;

	//	Render for dueling
	if (m_pvp.iDuelState != DUEL_ST_NONE)
	{
		switch (m_pvp.iDuelState)
		{
		case DUEL_ST_PREPARE:
			l = m_pvp.iDuelTimeCnt;
			a_Clamp(l, 0, 2999);
			n = l / 1000;
			t = l - n * 1000;
			if( t > 800 )
				f = 1.0f + (t - 800) / 200.0f;
			else
				f = 1.0f;
			if( m_pDuelCountDown[n] )
			{
				m_pDuelCountDown[n]->SetPosition(x, y);
				m_pDuelCountDown[n]->SetScaleX(f * sf);
				m_pDuelCountDown[n]->SetScaleY(f * sf);
				m_pDuelCountDown[n]->DrawToBack();
			}
			break;

		case DUEL_ST_INDUEL:
			if( m_pvp.iDuelTimeCnt < 1000 )
			{
				t = m_pvp.iDuelTimeCnt;
				if( t < 200 )
					f = 1.0f + (200 - t) / 200.0f;
				else
					f = 1.0f;
				if( m_pDuelStates[0] )
				{
					m_pDuelStates[0]->SetPosition(x, y);
					m_pDuelStates[0]->SetScaleX(f * sf);
					m_pDuelStates[0]->SetScaleY(f * sf);
					m_pDuelStates[0]->DrawToBack();
				}
			}
			break;

		case DUEL_ST_STOPPING:
			switch(m_pvp.iDuelRlt)
			{
			case 0:
				break;
			case 1:
				if( m_pDuelStates[1] )
				{
					m_pDuelStates[1]->SetPosition(x, y);
					m_pDuelStates[1]->SetScaleX(sf);
					m_pDuelStates[1]->SetScaleY(sf);
					m_pDuelStates[1]->DrawToBack();
				}
				break;
			case 2:
				if( m_pDuelStates[2] )
				{
					m_pDuelStates[2]->SetPosition(x, y);
					m_pDuelStates[2]->SetScaleX(sf);
					m_pDuelStates[2]->SetScaleY(sf);
					m_pDuelStates[2]->DrawToBack();
				}
				break;
			case 3:
				break;
			}
			break;
		}
	}

	//	Render for faction combat
	if( GetCombatFaction() )
	{
		if( m_nFactionCombatCounts < 2500 )
		{
			t = m_nFactionCombatCounts;
			if( t < 300 )
				f = 1.0f + (300 - t) / 300.0f;
			else
				f = 1.0f;

			if( m_pFactionCombatSprite )
			{
				m_pFactionCombatSprite->SetPosition(x, y);
				m_pFactionCombatSprite->SetScaleX(f * sf);
				m_pFactionCombatSprite->SetScaleY(f * sf);
				m_pFactionCombatSprite->DrawToBack();
			}
		}
	}

	//	Render for guild war
	if( IsInBattle() )
	{
		if( m_nGuildWarStartCounts < 2500 )
		{
			t = m_nGuildWarStartCounts;
			if( t < 300 )
				f = 1.0f + (300 - t) / 300.0f;
			else
				f = 1.0f;

			if( m_nGuildWarStartCounts )
			{
				m_pGuildWarStartSprite->SetPosition(x, y);
				m_pGuildWarStartSprite->SetScaleX(f * sf);
				m_pGuildWarStartSprite->SetScaleY(f * sf);
				m_pGuildWarStartSprite->DrawToBack();
			}
		}

		if( m_nGuildWarWinCounts < 2500 )
		{
			t = m_nGuildWarWinCounts;
			if( t < 300 )
				f = 1.0f + (300 - t) / 300.0f;
			else
				f = 1.0f;

			if( m_nGuildWarWinCounts )
			{
				m_pGuildWarWinSprite->SetPosition(x, y);
				m_pGuildWarWinSprite->SetScaleX(f * sf);
				m_pGuildWarWinSprite->SetScaleY(f * sf);
				m_pGuildWarWinSprite->DrawToBack();
			}
		}

		if( m_nGuildWarLostCounts < 2500 )
		{
			t = m_nGuildWarLostCounts;
			if( t < 300 )
				f = 1.0f + (300 - t) / 300.0f;
			else
				f = 1.0f;

			if( m_nGuildWarLostCounts )
			{
				m_pGuildWarLostSprite->SetPosition(x, y);
				m_pGuildWarLostSprite->SetScaleX(f * sf);
				m_pGuildWarLostSprite->SetScaleY(f * sf);
				m_pGuildWarLostSprite->DrawToBack();
			}
		}
	}

	//	Render for battle
	if (IsInBattle() && m_BattleInfo.iResult && m_BattleInfo.iResultCnt)
	{
		int iImage;
		if ((m_BattleInfo.iResult == 1 && m_iBattleCamp == GP_BATTLE_CAMP_INVADER) ||
			(m_BattleInfo.iResult != 1 && m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER))
			iImage = 1;		//	win
		else
			iImage = 2;		//	lose

		if( m_pDuelStates[iImage] )
		{
			m_pDuelStates[iImage]->SetPosition(x, y);
			m_pDuelStates[iImage]->SetScaleX(sf);
			m_pDuelStates[iImage]->SetScaleY(sf);
			m_pDuelStates[iImage]->DrawToBack();
		}
	}

	return true;
}


//	Camera pitch
void CECHostPlayer::CameraPitch(float fDelta)
{
	float fPitch = m_fCameraPitch + fDelta;
	a_Clamp(fPitch, -89.0f, 89.0f);
	
	if (fPitch != m_fCameraPitch)
	{
		m_CameraCoord.RotateX(DEG2RAD(fPitch - m_fCameraPitch));
		m_fCameraPitch = fPitch;
	}
}

//	Camera yaw
void CECHostPlayer::CameraYaw(float fDelta)
{
	float fYaw = m_fCameraYaw + fDelta;

	if (fYaw != m_fCameraYaw)
	{
		A3DVECTOR3 vPos = m_CameraCoord.GetPos();
		m_CameraCoord.SetPos(g_vOrigin);
		m_CameraCoord.RotateAxis(g_vAxisY, DEG2RAD(fYaw - m_fCameraYaw), false);
		m_CameraCoord.SetPos(vPos);

		m_fCameraYaw = fYaw;
	}
}

extern A3DVECTOR3 _cam_shake_offset;
extern bool _shaking_cam;

//	Update camera in following mode
void CECHostPlayer::UpdateFollowCamera(bool bRunning, DWORD dwTime)
{
	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();

	CECCamera::TICK_DATA InData;
	CECCamera::ADJUST_DATA OutData;

	// always look at the mass center
	A3DVECTOR3 vLookAt;
	float fHalfHeight = m_pTransfiguredModel? m_fTransHeight * 0.5f : m_aabb.Extents.y;
	if (m_iPhysique == PHYSIQUE_BIG )    //对兽族身高太高的修正
		vLookAt = A3DVECTOR3(0.0f, fHalfHeight * 1.4f, 0.0f) * GetAbsoluteTM();
	else if ( m_iPhysique == PHYSIQUE_MIDDLE)
		vLookAt = A3DVECTOR3(0.0f, fHalfHeight * 1.5f, 0.0f) * GetAbsoluteTM();
	else
		vLookAt = A3DVECTOR3(0.0f, fHalfHeight * 1.2f, 0.0f) * GetAbsoluteTM();
	//A3DVECTOR3 vLookAt = GetPos() + A3DVECTOR3(0.0f, m_aabb.Extents.y * 1.2f, 0.0f);

	InData.bRunning		= bRunning;
	InData.bTuning		= m_iTurnCammera ? true : false;
	InData.vecDirChar	= GetDir();
	InData.vecPosChar	= vLookAt;
	InData.vecUpChar	= GetUp();
	InData.dwTime		= dwTime;

	InData.bWater		= false;
#ifdef BACK_VERSION
	A3DTerrainWater * pTerrainWater = pWorld->GetTerrainWater();
#else
	A3DTerrainWater2* pTerrainWater = pWorld->GetTerrainWater();
#endif
	if( pTerrainWater )
	{
		if( GetPos().y < pTerrainWater->GetWaterHeight(GetPos()) - 2.5f )
			InData.bWater = true;
	}

	m_CameraCtrl.Tick(&InData, &OutData);

	if (!m_CameraCtrl.SceneryMode_Get() && !m_CameraCtrl.MovieMode_Get() && !m_CameraCtrl.FixCamPitch_Get())
	{
		if (_shaking_cam && !g_pGame->GetConfigs()->GetSystemSettings().bCloseShake) OutData.vecPosCam += _cam_shake_offset;
		A3DVECTOR3 vDelta = OutData.vecPosCam - vLookAt;
		A3DVECTOR3 vDir = vDelta;
		float fDist = vDir.Normalize();

		float fFactor = 1.0f - (fDist - CECCAMERA_MIN_DISTANCE) / (CECCAMERA_MAX_DISTANCE - CECCAMERA_MIN_DISTANCE);
		vLookAt.y += m_aabb.Extents.y * 0.3f * fFactor;

		//	Do camera collision from player's head
		CECCDS* pCDS = pWorld->GetCDS();

		RAYTRACERT TraceRt;

	//	if (pCDS->RayTrace(vLookAt, vDelta, 1.0f, &TraceRt, TRACEOBJ_CAMERA, 0))
		if (RayTrace(vLookAt, vDelta, TraceRt.vHitPos, TraceRt.vNormal, TraceRt.fFraction))
		{
			float fOffset = fDist * TraceRt.fFraction;
			OutData.vecPosCam = vLookAt + vDir * fOffset;
			OutData.vecPosCam += TraceRt.vNormal * .2f;
		}
		else
		{
			OutData.vecPosCam = vLookAt + vDir * fDist;
			OutData.vecPosCam.y += .2f;
		}
	}

	//	Ensure camera isn't under groud
	A3DVECTOR3 vTerrNorm;
	float fHei = pWorld->GetTerrain()->GetPosHeight(OutData.vecPosCam, &vTerrNorm);

	if (OutData.vecPosCam.y < fHei + 0.2f)
	{
		OutData.vecPosCam.y = fHei;
		OutData.vecPosCam += vTerrNorm * .2f;
	}

	m_CameraCoord.SetPos(OutData.vecPosCam);
	m_CameraCoord.SetDirAndUp(OutData.vecDirCam, OutData.vecUpCam);

	//	Adjust host's transparence
	AdjustTransparence(a3d_Magnitude(OutData.vecPosCam - vLookAt), OutData.vecDirCam, g_pGame->GetRealTickTime());
	if (m_bDim || m_bInvisible)
		SetTransparent(0.7f);

	if (m_iTurnCammera & TURN_LBUTTON)
	{
		A3DVECTOR3 vDir = OutData.vecDirCam;
		vDir.y = 0.0f;
		vDir.Normalize();
		if (!vDir.IsZero())
			SetDirAndUp(vDir, g_vAxisY);
	}
}

//	Start camera path
void CECHostPlayer::StartCameraPath(DWORD dwPathIndex)
{
//	m_pCameraPath->StartCameraPath(dwPathIndex);
}

//	Start cutscene
void CECHostPlayer::StartCutscene(DWORD dwCutscneIndex, bool bNeedCheck)
{
// 	if(!m_pCutscene->IsPlayingCutscene())
// 		m_pCutscene->StartCutscene(dwCutscneIndex, bNeedCheck);
}

//	Adjust host's transparence
void CECHostPlayer::AdjustTransparence(float fDistToCam, const A3DVECTOR3& vDir, DWORD dwTime)
{
	//	Adjust host's transparence
/*	float f1=-1.0f, f2=-1.0f;
	if (fDistToCam < 1.0f)
	{
		f1 = (fDist - 0.3f) / (1.0f - 0.3f);
		f1 = 1.0f - f1;
		a_Clamp(f1, 0.01f, 1.0f);
	}

	if (vDir.y > 0.85f)
	{
		f2 = (vDir.y - 0.85f) / 0.04f;
		a_Clamp(f2, 0.01f, 1.0f);
	}

	float fTrans = a_Max(f1, f2);
*/
	static int iLastAdjust = 0;
	float fTrans;
	int iAdjust;

	if (fDistToCam < 0.9f || vDir.y > 0.8f)
		iAdjust = 1;	//	Increase transparence
	else if (fDistToCam < 1.2f || vDir.y > 0.75f)
		iAdjust = iLastAdjust;	//	Keep adjustment of last time
	else
		iAdjust = -1;	//	Decrease transparence

	if (iAdjust > 0)
	{
		//	Turn to transparent model
		if (m_TransCnt.IncCounter(dwTime))
			m_TransCnt.Reset(true);

		fTrans = (float)m_TransCnt.GetCounter() / m_TransCnt.GetPeriod();
		a_Clamp(fTrans, 0.01f, 1.0f);
	}
	else if (iAdjust < 0)	//	Turn to solid model
	{
		if (m_TransCnt.GetCounter() >= dwTime)
		{
			m_TransCnt.DecCounter(dwTime);
			fTrans = (float)m_TransCnt.GetCounter() / m_TransCnt.GetPeriod();
			a_Clamp(fTrans, 0.01f, 1.0f);
		}
		else
		{
			fTrans = -1.0f;
			m_TransCnt.Reset();
		}
	}

	SetTransparent(fTrans);

	//	Adjust buddy's transparence
	if (GetBuddyState() && m_iBuddyId)
	{
		CECElsePlayer* pBuddy = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
		pBuddy->SetTransparent(fTrans);
	}

	iLastAdjust = iAdjust;
}

static const float _yaw_vel = 500.0f;
static const float _yaw_vel_air = 300.0f;

//	Get push direction
bool CECHostPlayer::GetPushDir(A3DVECTOR3& vPushDir, DWORD dwMask, float fDeltaTime)
{
	vPushDir.Set(0.0f, 0.0f, 0.0f);
	A3DVECTOR3 vRight(0),vRightHostCamera(0),vDirHostCamera(0);
	bool bMove = false;

// 	if ( m_pCameraPath && m_pCameraPath->IsUsingCameraPath())
// 	{
// 		vRightHostCamera= GetRight();
// 		vDirHostCamera	= GetDir();
// 	}else
// 	{
// 		vRightHostCamera= m_CameraCoord.GetRight();
// 		vDirHostCamera	= m_CameraCoord.GetDir();
// 	}


	if (!m_dwMoveRelDir)
		return false;

	if (m_dwMoveRelDir & MD_LEFT)
	{
		/*
		ECMSG msg;
		msg.dwParam2 = 0;
		if (m_iMoveEnv == MOVEENV_AIR)
		{
			msg.dwParam1 = DWORD(_yaw_vel_air * fDeltaTime);
			if (m_dwMoveRelDir & MD_BACK) msg.dwParam1 = -(int)msg.dwParam1;
			OnMsgHstYaw(msg);
		}
		else if (m_iMoveEnv == MOVEENV_WATER)
		{
			msg.dwParam1 = DWORD(_yaw_vel * fDeltaTime);
			if (m_dwMoveRelDir & MD_BACK) msg.dwParam1 = -(int)msg.dwParam1;
			OnMsgHstYaw(msg);
		}
		else
		*/
		{
			vRight = -vRightHostCamera;
			vRight.y = 0;
			vRight.Normalize();
			vPushDir = vRight;
			bMove = true;
		}
	}
	else if (m_dwMoveRelDir & MD_RIGHT)
	{
		/*
		ECMSG msg;
		msg.dwParam2 = 0;
		if (m_iMoveEnv == MOVEENV_AIR)
		{
			msg.dwParam1 = DWORD((-_yaw_vel_air) * fDeltaTime);
			if (m_dwMoveRelDir & MD_BACK) msg.dwParam1 = -(int)msg.dwParam1;
			OnMsgHstYaw(msg);
		}
		else if (m_iMoveEnv == MOVEENV_WATER)
		{
			msg.dwParam1 = DWORD((-_yaw_vel) * fDeltaTime);
			if (m_dwMoveRelDir & MD_BACK) msg.dwParam1 = -(int)msg.dwParam1;
			OnMsgHstYaw(msg);
		}
		else
		*/
		{
			vRight = vRightHostCamera;
			vRight.y = 0;
			vRight.Normalize();
			vPushDir = vRight;
			bMove = true;
		}
	}

	if (m_dwMoveRelDir & MD_FORWARD)
	{
		vPushDir = vDirHostCamera;
		vPushDir.y = 0;
		vPushDir.Normalize();

		if (bMove)
		{
			vPushDir += vRight;
			vPushDir.Normalize();
		}
	}
	else if (m_dwMoveRelDir & MD_BACK)
	{
		vPushDir = -vDirHostCamera;
		vPushDir.y = 0;
		vPushDir.Normalize();

		if (bMove)
		{
			vPushDir += vRight;
			vPushDir.Normalize();
		}
	}
	else if (m_dwMoveRelDir & MD_ABSUP & dwMask)
		vPushDir = g_vAxisY;
	else if (m_dwMoveRelDir & MD_ABSDOWN & dwMask)
		vPushDir = -g_vAxisY;
	else
		return bMove;

/*
	A3DVECTOR3 vDir = m_CameraCoord.GetDir();
	A3DVECTOR3 vRight = m_CameraCoord.GetRight();

	if (m_dwMoveRelDir & MD_FORWARD)
	{
		if (m_dwMoveRelDir & MD_LEFT)
			vPushDir = a3d_Normalize(vDir-vRight);
		else if (m_dwMoveRelDir & MD_RIGHT)
			vPushDir = a3d_Normalize(vDir+vRight);
		else
			vPushDir = vDir;
	}
	else if (m_dwMoveRelDir & MD_BACK)
	{
		if (m_dwMoveRelDir & MD_LEFT)
			vPushDir = a3d_Normalize(-vDir-vRight);
		else if (m_dwMoveRelDir & MD_RIGHT)
			vPushDir = a3d_Normalize(-vDir+vRight);
		else
			vPushDir = -vDir;
	}
	else if (m_dwMoveRelDir & MD_LEFT)
		vPushDir = -vRight;
	else if (m_dwMoveRelDir & MD_RIGHT)
		vPushDir = vRight;
	else
		return false;
*/
	return true;
}

//	Message MSG_HST_LBTNCLICK handler
void CECHostPlayer::OnMsgLBtnClick(const ECMSG& Msg)
{

}

//	Message MSG_HST_RBTNCLICK_OTHER handler
void CECHostPlayer::OnMsgRBtnClickForOtherUse( const ECMSG& Msg )
{
	//如果已经处于开启无目标释放状态，则右键后取消该状态
/*	if ( IsStartPosSkill() )
	{
		SetStartPosSkill( false );//结束开始无目标释放状态
		if ( m_pCastPosSkillGFX )
		{
			m_pCastPosSkillGFX->Stop();
		}
		return;
	}
*/	//-------------用户体验不好，暂时取消右键停止技能释放功能
}

//	Message MSG_HST_RBTNCLICK handler
void CECHostPlayer::OnMsgRBtnClick(const ECMSG& Msg)
{
	if (!IsAllResReady() || IsAboutToDie() || IsDead() || IsTrading() ||
		IsUsingTrashBox()|| IsTalkingWithNPC() || IsChangingFace())
		return;

	CECCDS* pCDS = g_pGame->GetGameRun()->GetWorld()->GetCDS();

	A3DVECTOR3 vDest((float)Msg.dwParam1, (float)Msg.dwParam2, 1.0f);
	A3DViewport* pView = g_pGame->GetViewport()->GetA3DViewport();
	pView->InvTransform(vDest, vDest);

	A3DVECTOR3 vStart = pView->GetCamera()->GetPos();
	A3DVECTOR3 vDelta = vDest - vStart;

	RAYTRACERT TraceRt;
	int iTraceFlag = TRACEOBJ_RBTNCLICK;

	if (pCDS->RayTrace(vStart, vDelta, 1.0f, &TraceRt, iTraceFlag, (DWORD)this))
	{
		if (pCDS->m_RayTraceRt.iEntity == ECENT_PLAYER)
		{
			m_idClickedMan = pCDS->m_RayTraceRt.iObjectID;
// 			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
// 			pGameUI->PopupPlayerContextMenu();
		}
	}
}

//	Revive
void CECHostPlayer::Revive(const S2C::cmd_player_revive* pCmd)
{
	//	Revive type
	enum
	{
		REVIVE_RETURN = 0,
		REVIVE_HERE_START,
		REVIVE_HERE_END,
	};

}

//	Begin following the selected target
void CECHostPlayer::BeginFollow(int idTarget)
{
	if (!CanDo(CANDO_FOLLOW))
		return;

	if (!ISPLAYERID(idTarget) || idTarget == m_PlayerInfo.cid)
		return;
}

void CECHostPlayer::BeginAutoMove()
{
	

	
}

bool CECHostPlayer::EndAutoMove()
{

	return false;
}


//	Level up
void CECHostPlayer::LevelUp()
{

}

void CECHostPlayer::DeityLevelUp(bool bSuccess, int iLevel)
{
	
}

//=============================================================================
/**
*@param:Tick时所需要的时间，本函数目前基本不用该时间。
*@return:void，无特殊返回。
*@usage:更新当前玩家能看到的阵法中各gfx特效的位置和方向。
**/
void CECHostPlayer::UpdateCircleOfDoomGFXs(DWORD dwDeltaTime)
{
	//遍历整个阵法的id-gfx表，更新每个阵法的链接光效gfx位置和方向。
	CircleOfDoomTable::iterator itrCodTbl = m_tbGFXTable.begin();
	for ( ; itrCodTbl != m_tbGFXTable.end(); ++itrCodTbl )
	{
		//处理每个阵法。
		CircleOfDoomGFX* pCodGfx = *itrCodTbl.value();//取得对应的阵法列表的指针
		CircleOfDoomGFX::iterator itrCodGfx = pCodGfx->begin();
		CECObject* pSponsor = g_pGame->GetGameRun()->GetWorld()->GetObject(*itrCodTbl.key(), 1);
		CECGFXCaster* pGfxCaster = g_pGame->GetGFXCaster();
		
		//如果，该阵眼不存在，那么可能是该玩家已经离开该区域，或者阵眼已经消失，则删除该阵法。
		if ( !pSponsor )
		{
			for ( ; itrCodGfx != pCodGfx->end(); ++itrCodGfx )
			{
				//释放gfx资源
				pGfxCaster->ReleaseGFXEx( *itrCodGfx.value() );
			}
			//释放具体阵法的列表
			delete pCodGfx;

			//从特效名字列表中删除该阵法特效名字
			RemoveCODGfxName( *itrCodTbl.key() );
			//从整体表中删除
			itrCodTbl = m_tbGFXTable.erase( itrCodTbl );
		}
		else //如果，该阵法存在，那么更新其gfx的位置和方向等信息。
		{
			for ( ; itrCodGfx != pCodGfx->end(); ++itrCodGfx )
			{
				//首先获得该成员的id，然后取得该成员的对象。
				CECObject* pMember = g_pGame->GetGameRun()->GetWorld()->GetObject(*itrCodGfx.key(), 1);
				//如果不存在，那么清除该gfx资源，并且更新改阵法的表
				if ( !pMember )
				{
					//释放gfx资源
					pGfxCaster->ReleaseGFXEx( *itrCodGfx.value() );
					//从单个的阵法表中删除
					itrCodGfx = pCodGfx->erase( itrCodGfx );
				}
				else //成员存在，更新该gfx位置和方向。
				{
					A3DGFXEx* pCurrentGFX = *itrCodGfx.value();
					//判断合法性
					if ( !pCurrentGFX )
					{
						continue;
					}
					//计算两个位置的方向
					A3DVECTOR3 vDir = a3d_Normalize( pSponsor->GetPos() - pMember->GetPos() );
					//计算转换矩阵
					A3DMATRIX4 mat = a3d_TransformMatrix(g_vAxisY, vDir, pMember->GetPos()+A3DVECTOR3(0.0f, 2.0f, 0.0f));
					//设置该gfx的转换矩阵
					pCurrentGFX->SetParentTM(mat);
					//设置该gfx的两个端点位置
					A3DVECTOR3 aEdge[2] = { pMember->GetPos()+A3DVECTOR3(0.0f, 2.0f, 0.0f), pSponsor->GetPos()+A3DVECTOR3(0.0f, 4.6f, 0.0f) };
					//针对每一个gfx的元素，都设置其两头端点位置。
					int nElementCount = pCurrentGFX->GetElementCount();
					for ( int nCurIndex = 0; nCurIndex < nElementCount; ++nCurIndex )
					{
						A3DGFXElement* pCurElement = pCurrentGFX->GetElement( nCurIndex );
						if ( !pCurElement || (pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNING
								&& pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNINGEX) ) //判断合法性
						{
							continue;
						}
						const char* szElementName = pCurElement->GetName();
						bool ret = GFX_UpdateLightingEdgePos(pCurrentGFX, szElementName, aEdge);
					} 
				}//end else
			}//end for itrCodGfx
		}//end else pSponsor
	}//end for itrCodTbl

}

/**
*@param:依次为阵法发起者id，阵法成员个数，阵法成员id列表
*@return:bool,标示更新阵法信息是否成功
*@usage:更新以nSponsorId为阵眼的阵法组成信息。如果该阵法之前不存在，那么直接保存到阵法列表中；
*		如果存在，则保留本地列表中和aMemberIds（服务器最新的数据）同时存在的id，删除列表在aMemberIds中没有的id。
**/
bool CECHostPlayer::UpdateCircleOfDoom(int nSkillId, int nSponsorId, int nMemberNum, const int* aMemberIds)
{
	//操作成功
	return true;
}

/**
*@param:阵眼id，标示该阵法
*@return:void
*@usage:在收到gs发来的阵法结束协议后，清除该阵法的信息和gfx特效 
**/
void CECHostPlayer::ReleaseCircleofDoom(int nSponsorId)
{

}

/**
*@param:void
*@return:void
*@usage:清除所有的阵法的信息.
**/
void CECHostPlayer::ReleaseAllCircleOfDoom( void )
{

}

/**
*@param: nSponsorId:阵眼ID；szGfxName：该阵法gfx所具有的名字
*@return: bool:成功与否
*@usage: 添加一个与阵法相关的gfx名字
**/
bool CECHostPlayer::AddCODGfxName( int nSponsorId, const AString& szGfxName )
{
	//检查nSponsorId的合法性
	if ( !nSponsorId )
	{
		return false;
	}

	//添加一个阵法gfx名字到列表中
	m_tbGfxName.put( nSponsorId, szGfxName );

	//
	return true;
}

/**
*@param: nSponsorId:阵眼id
*@return: bool 操作成功与否
*@usage: 删除一个与阵法相关的gfx名字
**/
bool CECHostPlayer::RemoveCODGfxName( int nSponsorId )
{
	//检查nSponsorId的合法性
	if ( !nSponsorId )
	{
		return false;
	}

	//删除该阵眼id所对应的阵法的gfx名字
	m_tbGfxName.erase(nSponsorId);

	//
	return true;
}

/**
*@param:void
*@return:bool,标示此时主角玩家位置是否处于各囚牢限定范围的合法位置
*@usage:主角玩家每次移动时，如果当前可视范围内有囚牢存在，则检查玩家
*	    当前位置是否处于各囚牢限定范围的合法位置。
*@date:2011-07-08
**/
bool CECHostPlayer::CheckCurrentPosValid( const A3DVECTOR3& vCurPos )
{
	//获得囚牢列表迭代器
	JailCircleAreaTable::iterator itrJailTable = m_tbJailCircleAreaTable.begin();

	//遍历整个囚牢信息列表，并计算出当前位置相对每个囚牢的合法性；
	//在此，根据需要，对各囚牢合法性进行“与”操作，也就是说，只要一个
	//囚牢在检测时不合法，就标示该位置不合法。
	for ( ; itrJailTable != m_tbJailCircleAreaTable.end(); ++itrJailTable )
	{
		JAIL_CIRCLE_AREA* pJail = itrJailTable.value();

		//获得主角当前位置，并计算到当前囚牢中心点的距离
		float fDist = ( vCurPos - pJail->jail_center ).MagnitudeH();

		//判断囚牢限制类型：0：能进，不能出；1：能出，不能进
		if ( 0 == pJail->jail_type )
		{
			if ( fDist >= (pJail->jail_radius - 0.05f) )//超过范围，非法
			{
				return false;
			}
		}
		else if ( 1 == pJail->jail_type )
		{
			if ( fDist <= (pJail->jail_radius + 0.5f) )//进入范围，非法
			{
				return false;
			}
		}
	}

	//
	return true;
}

/**
*@param: id,该囚牢的id；jailCircleArea，囚牢相关信息
*@return: bool,添加囚牢是否成功
*@usage: 添加单个囚牢
**/
bool CECHostPlayer::AddJailCircleArea( int id, const JAIL_CIRCLE_AREA& jailCircleArea )
{
	//检测id的合法性
	if ( !id )
	{
		return false;
	}

	//将囚牢信息插入到囚牢列表中
	m_tbJailCircleAreaTable.put(id, jailCircleArea);

	//
	return true;
}

/**
*@param: id,对应当前存在的需要删除囚牢的id
*@return: bool,标示删除操作是否成功
*@usage: 在收到gs协议后，在客户端删除该囚牢信息
**/
bool CECHostPlayer::RemoveJailCircleArea( int id )
{
	//判断id的合法性
	if ( !id )
	{
		return false;
	}

	return m_tbJailCircleAreaTable.erase( id );
}

/**
*@param: void
*@return: void
*@usage: 删除所有囚牢信息
**/
void CECHostPlayer::ClearAllJailCircleArea( void )
{
	m_tbJailCircleAreaTable.clear();
}

/**
*@param: nCasterId:释放技能者ID，nTargetId：目标ID，type：技能类型，0：拉扯，1：牵引
*@return: bool,处理成功与否
*@usage: 收到gs拉扯/牵引技能相关协议后，添加gfx相关信息到列表中
**/
bool CECHostPlayer::AddPullGfxInfo( int nCasterId, int nTargetId, unsigned char type )
{
	//判断ID的合法性

	if ( !nCasterId || !nTargetId )
	{
		return false;
	}

	//根据处理类型分别加载不同的gfx特效
	char szGfxName[512];
	if ( !type ) //太昊拉扯
	{
		//这样的搞法真的很蛋疼。策划需求改变，暂时在此加入代码以应变化
		CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(nCasterId, 1);
		if ( !pCaster )
		{
			return false;
		}
		CECPlayer* pPlayer = (CECPlayer*)pCaster;
		CECModel* pPlayerModel = pPlayer->GetBackupModel();
		if ( pPlayerModel )
		{
			pPlayerModel->PlayGfx("程序联入\\擒龙决.gfx", NULL);
		}
/*		if(pPlayer->GetPlayerModel())
		{
			A3DGFXEx* pGfx = pPlayer->GetPlayerModel()->GetGfx("程序联入\\擒龙决.gfx", NULL);
			if ( pGfx )
			{
				A3DMATRIX4 mat = a3d_TransformMatrix(pPlayer->GetDir(), g_vAxisY, pPlayer->GetPos());
				pGfx->SetParentTM(mat);
			}
		}*/
		return true;
		//sprintf( szGfxName, "%s", "程序联入\\擒龙决.gfx" );//BE CARE! Canceled 2011-08-03.
	}
	else if ( 1 == type ) //焚香牵引
	{
		sprintf( szGfxName, "%s", "程序联入\\焚香_灵力牵制链接.gfx" );//BE CARE!

		//对于焚香的牵引技能，还需要在施法者和目标身上加入特定的特效，特效的开启和关闭都和牵引的链接光效相同
		CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(nCasterId, 1);
		CECObject* pTarget = g_pGame->GetGameRun()->GetWorld()->GetObject(nTargetId, 1);
		if ( pCaster && pTarget ) //gs所发送的玩家id都存在
		{
			CECPlayer* pCasterPlayer = (CECPlayer*)pCaster;
			CECPlayer* pTargetPlayer = (CECPlayer*)pTarget;
			//播放施法者和目标身上的特效
			CECModel* pModel = pCasterPlayer->GetBackupModel();
			if ( pModel )
			{
				pModel->PlayGfx("程序联入\\焚香_灵力牵制自身手上效果.gfx", "HH_weapon");
			}
			pModel = pTargetPlayer->GetBackupModel();
			if ( pModel )
			{
				pModel->PlayGfx("程序联入\\焚香_灵力牵制目标身上.gfx", "HH_spine");
			}
		}
	}

	//取得gfxcaster
	CECGFXCaster* pGfxCaster = g_pGame->GetGFXCaster();

	A3DGFXEx* pCurrentGfx = pGfxCaster->LoadGFXEx(szGfxName);//加载gfx资源
	if ( pCurrentGfx )	//加载gfx资源成功，将其存入表中，并设置其启动标志
	{
		//将gfx相关信息存入表中
		PULL_GFX_INFO gfxInfo;
		gfxInfo.nTargetId = nTargetId;
		gfxInfo.pGfx = pCurrentGfx;
		m_tbPullGfxInfo.put( nCasterId, gfxInfo );
		//启动
		if ( pCurrentGfx->GetState() == ST_STOP )
		{
			pCurrentGfx->Start( true );
		}
	}

	//
	return true;
}

/**
*@param: nCasterId:技能释放者ID ->也是整个PullGfxInfoTable的索引index
*@return: bool 操作成功与否
*@usage: 收到gs协议后，从表中删除nCasterId所标示的拉扯/牵引相关gfx信息
**/
bool CECHostPlayer::RemovePullGfxInfo( int nCasterId )
{
	//判断nCasterId的合法性
	if ( !nCasterId )
	{
		return false;
	}
	
	//取得gfxcaster
	CECGFXCaster* pGfxCaster = g_pGame->GetGFXCaster();

	//取得该nCasterId所标示的gfx信息
	PullGfxInfoTable::pair_type Pair = m_tbPullGfxInfo.get( nCasterId );
	if ( !Pair.second )
	{
		return false;
	}
	
	A3DGFXEx* pCurrentGfx = Pair.first->pGfx;
	if ( pCurrentGfx )
	{
		pGfxCaster->ReleaseGFXEx( pCurrentGfx );
	}

	//删除与牵引相关的施法者和目标身上的锁链特效
/*	CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(nCasterId, 1);
	CECObject* pTarget = g_pGame->GetGameRun()->GetWorld()->GetObject(Pair.first->nTargetId, 1);
	if ( pCaster && pTarget ) //gs所发送的玩家id都存在
	{
		CECPlayer* pCasterPlayer = (CECPlayer*)pCaster;
		CECPlayer* pTargetPlayer = (CECPlayer*)pTarget;
		//播放施法者和目标身上的特效
		CECModel* pModel = pCasterPlayer->GetPlayerModel();
		if ( pModel )
		{
			pModel->RemoveGfx("程序联入\\焚香_灵力牵制自身手上效果.gfx", "HH_weapon");
		}
		pModel = pTargetPlayer->GetPlayerModel();
		if ( pModel )
		{
			pModel->RemoveGfx("程序联入\\焚香_灵力牵制目标身上.gfx", "HH_spine");
		}
	}
*/	//Canceled and modified 2011-09-14.
	RemovePullSpecialGfxInfo( nCasterId, Pair.first->nTargetId ); //Added 2011-09-14.

	//从gfx 信息列表中删除该数据项
	m_tbPullGfxInfo.erase(nCasterId);

	//
	return true;
}

/**
*@param: dwDeltaTime,Tick 时间间隔
*@return: void
*@usage: 每个Tick中更新拉扯/牵引技能的gfx特效
**/
void CECHostPlayer::UpdatePullGfxs( DWORD dwDeltaTime )
{
	//获得GFXCaster指针
	CECGFXCaster* pGfxCaster = g_pGame->GetGFXCaster();
	//初始化迭代器
	PullGfxInfoTable::iterator itrPullGfxInfoTbl = m_tbPullGfxInfo.begin();

	//依次更新列表中每个gfx的位置和状态
	for ( ; itrPullGfxInfoTbl != m_tbPullGfxInfo.end(); ++itrPullGfxInfoTbl )
	{
		//获得该gfx相关的技能释放者
		CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(*itrPullGfxInfoTbl.key(), 1);
		if ( !pCaster ) //不存在该玩家世界,或者该玩家已经死亡？
		{
			//考虑到合理性，在玩家不存在的情况下，删除该gfx
			if ( itrPullGfxInfoTbl.value()->pGfx )
			{
				pGfxCaster->ReleaseGFXEx( itrPullGfxInfoTbl.value()->pGfx );
			}
			RemovePullSpecialGfxInfo( *itrPullGfxInfoTbl.key(), itrPullGfxInfoTbl.value()->nTargetId );//删除施法者和目标玩家身上的锁链特效 Added 2011-09-14.
			itrPullGfxInfoTbl = m_tbPullGfxInfo.erase(itrPullGfxInfoTbl);
			continue;
		}
		PULL_GFX_INFO gfxInfo = *itrPullGfxInfoTbl.value();
		//获得该gfx相关的技能释放目标对象
		CECObject* pTarget = g_pGame->GetGameRun()->GetWorld()->GetObject(gfxInfo.nTargetId, 1);
		if ( !pTarget ) //不存在该玩家世界？
		{
			//考虑到合理性，在目标玩家不存在的情况下，删除该gfx
			if ( gfxInfo.pGfx )
			{
				pGfxCaster->ReleaseGFXEx( gfxInfo.pGfx );
			}
			RemovePullSpecialGfxInfo( *itrPullGfxInfoTbl.key(), gfxInfo.nTargetId );//删除施法者和目标玩家身上的锁链特效 Added 2011-09-14.
			itrPullGfxInfoTbl = m_tbPullGfxInfo.erase(itrPullGfxInfoTbl);
			continue;
		}
		
		//更新gfx状态和位置
		A3DGFXEx* pCurrentGFX = gfxInfo.pGfx;
		//判断合法性
		if ( !pCurrentGFX )
		{
			continue;
		}
		//计算两个位置的方向
		A3DVECTOR3 vDir = a3d_Normalize( pCaster->GetPos() - pTarget->GetPos() );
		//计算转换矩阵
		A3DMATRIX4 mat = a3d_TransformMatrix(g_vAxisY, vDir, pTarget->GetPos());//+A3DVECTOR3(0.0f, 2.0f, 0.0f)
		//设置该gfx的转换矩阵
		pCurrentGFX->SetParentTM(mat);
		//设置该gfx的两个端点位置
		A3DVECTOR3 aEdge[2] = { pTarget->GetPos()+A3DVECTOR3(0.0f, 1.1f, 0.0f), pCaster->GetPos()+A3DVECTOR3(0.15f, 1.1f, 0.0f) };//+A3DVECTOR3(0.0f, 3.8f, 0.0f)
		//针对每一个gfx的元素，都设置其两头端点位置。
		int nElementCount = pCurrentGFX->GetElementCount();
		for ( int nCurIndex = 0; nCurIndex < nElementCount; ++nCurIndex )
		{
			A3DGFXElement* pCurElement = pCurrentGFX->GetElement( nCurIndex );
			if ( !pCurElement || (pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNING
								&& pCurElement->GetEleTypeId() != ID_ELE_TYPE_LIGHTNINGEX) ) //判断合法性
			{
				continue;
			}
			const char* szElementName = pCurElement->GetName();
			bool ret = GFX_UpdateLightingEdgePos(pCurrentGFX, szElementName, aEdge);
		} //end for nCurIndex
	}//end for itrPullGfxInfoTbl
}

/**
*@param: nCasterId:施法者id, nTargetId:目标者id
*@return: bool 表示函数操作是否成功
*@usage: 释放牵引者和被牵引者身上的束缚特效
**/
bool CECHostPlayer::RemovePullSpecialGfxInfo( int nCasterId, int nTargetId )
{
	//判断输入参数的合法性
	if ( !nCasterId || !nTargetId )
	{
		return false;
	}

	//删除与牵引相关的施法者和目标身上的锁链特效
	CECObject* pCaster = g_pGame->GetGameRun()->GetWorld()->GetObject(nCasterId, 0);//无论是死的还是活的
	if ( pCaster )	//该玩家存在（活的状态或者死的状态）
	{
		CECPlayer* pCasterPlayer = (CECPlayer*)pCaster;
		//删除施法者身上的特效
		CECModel* pModel = pCasterPlayer->GetBackupModel();
		if ( pModel )
		{
			pModel->RemoveGfx("程序联入\\焚香_灵力牵制自身手上效果.gfx", "HH_weapon");
		}
	}
	
	CECObject* pTarget = g_pGame->GetGameRun()->GetWorld()->GetObject(nTargetId, 0);//无论是死的还是活的
	if ( pTarget )
	{
		CECPlayer* pTargetPlayer = (CECPlayer*)pTarget;
		//删除目标身上的特效
		CECModel* pModel = pTargetPlayer->GetBackupModel();
		if ( pModel )
		{
			pModel->RemoveGfx("程序联入\\焚香_灵力牵制目标身上.gfx", "HH_spine");
		}
	}

	//
	return true;
}

void CECHostPlayer::OnLeaveGameWorld()
{
	if (m_pBubbleTexts)
	{
		delete m_pBubbleTexts;
		m_pBubbleTexts = NULL;
	}
	
	m_pBubbleTexts = new CECBubbleDecalList;
}


//=============================================================================

void CECHostPlayer::OnTaskFinished()
{
	if (m_pPlayerModel)
		m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_TASK_FINISHED), NULL);
}

//	Host touched target ?
bool CECHostPlayer::CanTouchTarget(const A3DAABB& aabbTarget)
{
	if (CLS_AABBAABBOverlap(m_aabb.Center, m_aabb.Extents, aabbTarget.Center, 
			aabbTarget.Extents * 1.5f))
		return true;

	return false;
}

static const float _attack_range_map[] =
{
	1.5f,	// 0
	1.5f,

	1.5f,	// 1
	1.5f,

	1.5f,	// 2
	1.5f,

	1.5f,	// 3
	2.0f,

	2.0f,	// 4
	2.5f,

	3.0f,	// 5
	3.0f,

	3.5f,	// 6
	4.0f,

	4.5f,	// 7
	4.5f,

	5.0f,	// 8
	5.5f,

	6.0f,	// 9
	6.5f,

	7.0f,	// 10
	7.5f,

	8.0f,	// 11
	8.5f,

	9.0f,	// 12
	9.5f,

	10.0f,	// 13
	10.5f,

	11.0f,	// 14
	11.0f
};

//	Can host touch target ?
//	iReason: 0 - no special reason; 1 - melee; 2 - cast magic; 3 - talk;
//	fMaxCut: Maximum cut distance
bool CECHostPlayer::CanTouchTarget(const A3DVECTOR3& vTargetPos, float fTargetRad, 
								int iReason, float fMaxCut/* 1.0f */)
{
	float fDist = a3d_Magnitude(vTargetPos - GetPos());

	// 回放时位置可能有很小的误差，导致判断不准，所以加大范围值

	switch (iReason)
	{
	case 1:		//	melee
	{
		unsigned int index = unsigned int(m_ExtProps.attack_range * 2.0f + .1f);

		if (index < sizeof(_attack_range_map) / sizeof(float))
		{
			float fRange = _attack_range_map[index];

			if (fDist - fTargetRad <= fRange)
				return true;
		}
		else
		{
			float fRange;
			if( m_ExtProps.attack_range > 11.0f )
				fRange = m_ExtProps.attack_range - 3.0f;
			else 
				fRange = m_ExtProps.attack_range * 0.6f;
			if (fDist - fTargetRad <= fRange)
				return true;
		}

		break;
	}
	case 2:		//	cast magic
	{
		if (m_pPrepSkill)
		{
		
			return true;
		}

		break;
	}
	case 3:		//	talk
	{
		if (fDist - fTargetRad <= 5.0f)
			return true;

		break;
	}
	default:	//	no special reason
	{
		if (fDist < (fTargetRad + m_fTouchRad) * 3.0f)
			return true;

		break;
	}
	}
	
	return false;
}

//	Update selected target
void CECHostPlayer::UpdateSelectedTarget()
{
	if (!m_idSelTarget)
		return;

	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
	CECObject* pObject = NULL;

	if (ISPLAYERID(m_idSelTarget))
		pObject = m_pPlayerMan->GetPlayer(m_idSelTarget);
	else if (ISNPCID(m_idSelTarget))
	{
		CECNPC* pNPC = pWorld->GetNPCMan()->GetNPC(m_idSelTarget);
		if (pNPC && !pNPC->IsDead())
			pObject = pNPC;
	}

	if (!pObject)
		m_idSelTarget = 0;
}

void CECHostPlayer::UpdatePetSkillCoolDown(DWORD dwTick)
{
	if( m_nCurPetIndex == -1 )
		return;
	
// 	CECPetBedge* pPet = (CECPetBedge*)m_pPetPack->GetItem(m_nCurPetIndex);
// 
// 	if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
// 		pPet->RefreshCoolDown(dwTick);
}

//	Estimate move environment
void CECHostPlayer::EstimateMoveEnv(const A3DVECTOR3& vPos)
{
	CECNPCMan* pNPCMan = g_pGame->GetGameRun()->GetWorld()->GetNPCMan();

	if (IsFlying())
	{
		m_iMoveEnv = MOVEENV_AIR;
		if (m_idCarrier && !m_bCarrierCooling)
		{
			A3DVECTOR3 vPos, vDir;
			pNPCMan->GetPoseOnCarrier(m_idCarrier, m_matRelCarrier, vPos, vDir);
// 			g_pGame->GetGameSession()->c2s_CmdLeaveCarrier(m_idCarrier, vPos, glb_CompressDirH(vDir.x, vDir.z));
// 			OutputDebugStringA("Send Leave Carrier!!\n");

			m_idCarrier = 0;	
		}

		return;
	}

	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();
//	m_GndInfo.fGndHei = pWorld->GetTerrainHeight(vPos, &m_GndInfo.vGndNormal);

	A3DVECTOR3 vGndPos, vTestPos = vPos + g_vAxisY * m_aabb.Extents.y;
	VertRayTrace(vTestPos, vGndPos, m_GndInfo.vGndNormal);
	m_GndInfo.fGndHei = vGndPos.y;
	m_GndInfo.fWaterHei = pWorld->GetWaterHeight(vTestPos);

	//@note : use the aabb trace. By Kuiwu[12/10/2005]
	CDR_TRACE_OUTPUT out;
	VertAABBTrace(vTestPos, m_aabb.Extents, out);
	A3DVECTOR3 vAABBGnd = out.vHitPos;
	vAABBGnd.y -= m_aabb.Extents.y;

	//@note : By Kuiwu[12/10/2005]
	int iNewEnv = MOVEENV_GROUND;
	if (CheckWaterMoveEnv(vPos, m_GndInfo.fWaterHei, vAABBGnd.y))
		iNewEnv = MOVEENV_WATER;

	if (iNewEnv == MOVEENV_GROUND)
	{
		m_GndInfo.bOnGround = true;

	//	if (vPos.y > m_GndInfo.fGndHei + 0.2f)
	//	if (m_pCDRInfo->vTPNormal.IsZero())
		if (!m_pCDRInfo->bOnSurface)
		{
			if (m_iMoveMode != MOVE_FREEFALL)
				m_iMoveMode = MOVE_FREEFALL;

			m_GndInfo.bOnGround = false;
			if (IsJumping())
			{ 
#define PRELAND_TIME		0.25f
				// see if we will land in a short time;
				if( m_pCDRInfo->vAbsVelocity.y < 0.0f )
				{
					float dh = -m_pCDRInfo->vAbsVelocity.y * PRELAND_TIME + 0.5f * m_pCDRInfo->fGravityAccel * PRELAND_TIME * PRELAND_TIME;
					if( vPos.y - vAABBGnd.y < dh )
						PlayAction(ACT_JUMP_LAND, 1.0f, false, 100);
				}
				else
				{
					if( m_pCDRInfo->vAbsVelocity.y < m_pCDRInfo->fGravityAccel * PRELAND_TIME * 0.5f )
					{
						float dt = PRELAND_TIME - 2.0f * m_pCDRInfo->vAbsVelocity.y / m_pCDRInfo->fGravityAccel;
						float dh = m_pCDRInfo->vAbsVelocity.y * dt + 0.5f * m_pCDRInfo->fGravityAccel * dt * dt;
						if( vPos.y - vAABBGnd.y < dh )
							PlayAction(ACT_JUMP_LAND, 1.0f, false, 100);
					}
				}
			}
		}
		else
		{
			//if (IsJumping() && m_pCDRInfo->vAbsVelocity.y < 0.0f && vPos.y - vAABBGnd.y < 0.6f)
			if (IsJumping())
			{
				PlayAction(ACT_JUMP_LAND, 1.0f, false);
				ResetJump();
			}

		//	if (m_GndInfo.vGndNormal.y < EC_SLOPE_Y)
		//	if (m_pCDRInfo->vTPNormal.y < EC_SLOPE_Y)
			if (!m_pCDRInfo->bCanStay)
			{
				if (!m_MoveCtrl.GetSlideLock())
				{
					if (m_iMoveMode != MOVE_SLIDE)
					{
						m_iOldWalkMode = m_iMoveMode;
						m_iMoveMode = MOVE_SLIDE;
					}
				}
				else
				{
					m_MoveCtrl.SetSlideLock(false);
					m_iMoveMode = MOVE_STAND;
				}
			}
			else
			{
				m_MoveCtrl.SetSlideLock(false);
				if (m_iMoveMode == MOVE_FREEFALL)
					m_iMoveMode = MOVE_STAND;
				else if (m_iMoveMode == MOVE_SLIDE)
					m_iMoveMode = m_iOldWalkMode;
			}
		}

		if (!m_bCarrierCooling)
		{
			if (out.iUserData1 == ECENT_CARRIER)
			{
				if (!m_idCarrier)
				{	
					//	TODO: tell enter that we change a carrier now
					pNPCMan->CalcCarrierRelMat(out.iUserData2, GetAbsoluteTM(), m_matRelCarrier);
					A3DVECTOR3 vPos = GetRelPosOnCarrier();
					A3DVECTOR3 vDir = GetRelDirOnCarrier();
//					g_pGame->GetGameSession()->c2s_CmdEnterCarrier(out.iUserData2, vPos, glb_CompressDirH(vDir.x, vDir.z));
					OutputDebugStringA("Send Enter Carrier!!\n");

      				m_bCarrierCooling = true;
				}
			}
			else if (m_idCarrier)
			{
				//	TODO: tell server that we leave carrier now
				A3DVECTOR3 vPos, vDir;
				pNPCMan->GetPoseOnCarrier(m_idCarrier, m_matRelCarrier, vPos, vDir);
			//	g_pGame->GetGameSession()->c2s_CmdLeaveCarrier(m_idCarrier, vPos, glb_CompressDirH(vDir.x, vDir.z));
				OutputDebugStringA("Send Leave Carrier!!\n");

				m_idCarrier = 0;
				m_bCarrierCooling = true;
			}
		}
	}
	else if (iNewEnv == MOVEENV_WATER)
	{
#ifdef CDR_HL2_MOVE
		m_pCDRInfo->vClipVel.Clear();
#else
		m_pCDRInfo->fYVel = 0.0f;
#endif
		
		if (m_iMoveMode == MOVE_SLIDE)
		{
// 			CECHPWork* pWork = m_pWorkMan->GetCurWork();
// 			if (pWork && pWork->IsMoving())
// 				m_iMoveMode = MOVE_MOVE;
// 			else
// 				m_iMoveMode = MOVE_STAND;
		}
	}

	m_iMoveEnv = iNewEnv;
}

//	Estimate mouse cursor
void CECHostPlayer::EstimateCursor()
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	m_idCurHover = 0;
	if (pGameRun->UIHasCursor() || IsChangingFace())
		return;
}

//	Update the NO-TARGET cast skill states. Added 2012-05-05.
void CECHostPlayer::UpdateNoTargetSkillStates()
{
	//首先判断是否处于开启无目标释放技能状态,不处于此状态则返回
	if ( !IsStartPosSkill() )
	{
		return;
	}

	//如果按ESC键则取消技能释放
	if ( GetAsyncKeyState(VK_ESCAPE) & 0x8000 )
	{
		SetStartPosSkill( false );//结束开始无目标释放状态
		if ( m_pCastPosSkillGFX )
		{
			m_pCastPosSkillGFX->Stop();
		}
		return;
	}
}

//	Init the NO-TARGET cast skill gfx info. Added 2012-05-07.
void CECHostPlayer::InitNoTargetSkillGFXInfo( CECSkill* pSkill )
{
	//判断参数合法性
	if ( !pSkill )
	{
		ASSERT(0);
		return;
	}

	if ( !m_pCastPosSkillGFX )	//还没初始化, 则先初始化
	{
		m_pCastPosSkillGFX = g_pGame->GetGFXCaster()->LoadGFXEx( "程序联入\\技能选区.gfx" );
		if ( !m_pCastPosSkillGFX )
		{
			ASSERT(0);
			return;
		}
	}	

	//-----------------------------------------------------------
	//根据技能的攻击半径更新特效的大小
// 	float fSkillRadius = pSkill->GetNoTargetSkillRadius();
// 	if ( fSkillRadius < 0.00001f )
// 	{
// 		ASSERT(0);
// 	}
// 	else
// 	{
// 		float fScale = fSkillRadius / 5.0f;	//目前固定设置特效的半径为5m
// 		m_pCastPosSkillGFX->SetScale( fScale );
// 	}
	//-----------------------------------------------------------
}

//	Update time counters
void CECHostPlayer::UpdateTimers(DWORD dwDeltaTime)
{
	//	Get real time tick of this frame
	int iRealTime = g_pGame->GetRealTickTime();

	//	Update flysword time
/*	if (IsFlying() && GetRushFlyFlag())
	{
		CECIvtrItem* pItem = m_pEquipPack->GetItem(EQUIPIVTR_FLYSWORD);
		ASSERT(pItem);

		if (pItem->GetClassID() == CECIvtrItem::ICID_FLYSWORD)
		{
			CECIvtrFlySword* pFlySword = (CECIvtrFlySword*)pItem;
			pFlySword->TimePass(iRealTime);
		}
	}	*/
	
	int i;

	//	Update skills
// 	for (i=0; i < m_aPtSkills.GetSize(); i++)
// 		m_aPtSkills[i]->Tick();
// 
// 	for (i=0; i < m_aPsSkills.GetSize(); i++)
// 		m_aPsSkills[i]->Tick();
// 	
// 	for (i=0; i < m_aTsSkills.GetSize(); i++)
// 		m_aTsSkills[i]->Tick();
// 
// 	for (i=0; i < m_aImSkills.GetSize(); i++)
// 		m_aImSkills[i]->Tick();
// 
// 	//	Update cool times
// 	for (i=0; i < GP_CT_MAX; i++)
// 	{
// 		if (m_aCoolTimes[i].iCurTime > 0)
// 		{
// 			if ((m_aCoolTimes[i].iCurTime -= iRealTime) < 0)
// 				m_aCoolTimes[i].iCurTime = 0;
// 		}
// 	}
// 
// 	for (i=0; i < GP_CT_PRODUCE_END - GP_CT_PRODUCE_START; i++)
// 	{
// 		if (m_aProduceCoolTimes[i].iCurTime > 0)
// 		{
// 			if ((m_aProduceCoolTimes[i].iCurTime -= iRealTime) < 0)
// 				m_aProduceCoolTimes[i].iCurTime = 0;
// 		}
// 	}
// 
// 	for (i=0; i< GP_CT_COMBINE_EDIT_END - GP_CT_COMBINE_EDIT_START; i++)
// 	{
// 		if (m_aSkillEditCoolTimes[i].iCurTime > 0)
// 		{
// 			if ((m_aSkillEditCoolTimes[i].iCurTime -= iRealTime) < 0)
// 				m_aSkillEditCoolTimes[i].iCurTime = 0;
// 		}
// 	}
// 
// 	//	Gather time counter
// 	if (m_GatherCnt.IncCounter(iRealTime))
// 		m_GatherCnt.Reset(true);
// 
// 	//	Bind command cool counter
// 	if (m_BindCmdCoolCnt.IncCounter(iRealTime))
// 		m_BindCmdCoolCnt.Reset(true);
// 
// 	//	For some reasons on server, sometimes friend list couldn't be got
// 	//	successfully. Try to get it again every 20s if this case really happen
// 	if (m_pFriendMan && !m_pFriendMan->CheckInit())
// 	{
// 		m_iGetFriendCnt -= iRealTime;
// 		if (m_iGetFriendCnt < 0)
// 		{
// 			g_pGame->GetGameSession()->friend_GetList();
// 			m_iGetFriendCnt = 60000;
// 		}
// 	}
// 	
// 	//	Duel stopping time counter
// 	if (m_pvp.iDuelState == DUEL_ST_PREPARE)
// 	{
// 		m_pvp.iDuelTimeCnt -= iRealTime;
// 		if (m_pvp.iDuelTimeCnt < 0)
// 			m_pvp.iDuelTimeCnt = 0;
// 	}
// 	else if (m_pvp.iDuelState == DUEL_ST_INDUEL)
// 	{
// 		m_pvp.iDuelTimeCnt += iRealTime;
// 	}
// 	else if (m_pvp.iDuelState == DUEL_ST_STOPPING)
// 	{
// 		m_pvp.iDuelTimeCnt -= iRealTime;
// 		if (m_pvp.iDuelTimeCnt < 0)
// 		{
// 			m_pvp.iDuelTimeCnt = 0;
// 			m_pvp.iDuelState = DUEL_ST_NONE;
// 			m_pvp.idDuelOpp = 0;
// 		}
// 	}
// 
// 	if( GetCombatFaction() )
// 	{
// 		m_nFactionCombatCounts += iRealTime;
// 	}
// 
// 	if( IsInBattle() )
// 	{
// 		if( m_nGuildWarStartCounts != 0x7fffffff )
// 			m_nGuildWarStartCounts += iRealTime;
// 		if( m_nGuildWarWinCounts != 0x7fffffff )
// 			m_nGuildWarWinCounts += iRealTime;
// 		if( m_nGuildWarLostCounts != 0x7fffffff )
// 			m_nGuildWarLostCounts += iRealTime;
// 	}
// 
// 	//	Update pariah time counter
// 	if (m_dwPariahTime)
// 	{
// 		if (m_dwPariahTime > (DWORD)iRealTime)
// 			m_dwPariahTime -= (DWORD)iRealTime;
// 		else
// 			m_dwPariahTime = 0;
// 	}
// 
// 	//	Update pet operation time counter
// 	m_PetOptCnt.IncCounter(iRealTime);
// 
// 	//	Update battle result time counter
// 	if (IsInBattle() && m_BattleInfo.iResult && m_BattleInfo.iResultCnt)
// 	{
// 		if ((m_BattleInfo.iResultCnt -= iRealTime) < 0)
// 			m_BattleInfo.iResultCnt = 0;
// 	}	
// 
// 	//	Hasn't been attacked from a player for over 3 seconds, change to Not Under Attack state
// 	if (m_bUnderEPAtk && m_UnderAtkCnt.IncCounter(iRealTime))
// 	{
// 		m_bUnderEPAtk = false;
// 	}
// 	//	Time Elapsed since last attacking another player
// 	m_ReAtkCnt.IncCounter(iRealTime);
// 
// 	if (m_pTransfiguredModel && m_TransfigureProp.iMoveType == TRANSFIGURE_MOVE_RANDOM)
// 	{
// 		m_RandomMoveCnt.IncCounter(iRealTime);
// 		m_RandomStopCnt.IncCounter(iRealTime);
// 	}
// 	// 对于起飞中释放技能导致飞剑或飞骑显示不出来的修正
// 	if (m_bFlyingOff && m_FlyingOffCnt.IncCounter(iRealTime))
// 	{
// 		ShowWing(true);
// 		m_bFlyingOff = false;
// 	}
// 
// 	if (!m_SkillCoolCnt.IsFull())
// 		m_SkillCoolCnt.IncCounter(iRealTime);
// 
// 	// 其他技能的冷却缓存时间的更新
// 	abase::hash_map<int, int>::iterator it;
// 	for(it= m_TsSkillCoolDown.begin(); it != m_TsSkillCoolDown.end(); ++it)
// 	{
// 		if (it->second > 0)
// 		{
// 			it->second -= iRealTime;
// 			if (it->second < 0)
// 				it->second = 0;
// 		}
// 	}
}

//	Update GFXs
void CECHostPlayer::UpdateGFXs(DWORD dwDeltaTime)
{
//	if (m_pLevelUpGFX)
//		m_pLevelUpGFX->SetParentTM(GetAbsoluteTM());

	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld();

	if (m_pHoverGFX)// && m_idCurHover != m_idSelTarget)
	{
		if (!IsChangingFace() && (ISPLAYERID(m_idCurHover) || ISNPCID(m_idCurHover)))
		{
			CECObject* pObject = pWorld->GetObject(m_idCurHover, 1);
			if (pObject)
			{
				if (m_pHoverGFX->GetState() == ST_STOP)
					m_pHoverGFX->Start();

				m_pHoverGFX->SetParentTM(pObject->GetAbsoluteTM());
			}
			else
				m_pHoverGFX->Stop();
		}
		else
			m_pHoverGFX->Stop();
	}

	if (m_pSelectedGFX)
	{
		if (!IsChangingFace() && (ISPLAYERID(m_idSelTarget) || ISNPCID(m_idSelTarget)))
		{
			CECObject* pObject = pWorld->GetObject(m_idSelTarget, 1);
			if (pObject)
			{
				if (m_pSelectedGFX->GetState() == ST_STOP)
					m_pSelectedGFX->Start();

				m_pSelectedGFX->SetParentTM(pObject->GetAbsoluteTM());
			}
			else
				m_pSelectedGFX->Stop();
		}
		else
			m_pSelectedGFX->Stop();
	}

	if (m_pFloatDust)
	{
#ifdef BACK_VERSION
		A3DTerrainWater* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#else
		A3DTerrainWater2* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
#endif

		if (pWater->IsUnderWater(m_CameraCoord.GetPos()))
		{
			if (m_pFloatDust->GetState() == ST_STOP)
			{
				m_pFloatDust->Start(true);
				m_pFloatDust->TickAnimation(2000);
			}

			m_pFloatDust->SetParentTM(GetAbsoluteTM());
		}
		else if (m_pFloatDust->GetState() != ST_STOP)
			m_pFloatDust->Stop();
	}
}

//	Check whether player can use specified equipment
bool CECHostPlayer::CanUseEquipment(CECIvtrEquip* pEquip)
{

	
	return true;
}

//	Is spelling magic
bool CECHostPlayer::IsSpellingMagic()
{
	return false;
}

//	Is flash moving ?
bool CECHostPlayer::IsFlashMoving()
{
	return false;
}

//	Is taking off ?
bool CECHostPlayer::IsTakingOff()
{
	return false;
}

//	Is picking up something
bool CECHostPlayer::IsPicking()
{
	return false;
}

//	Is gathering resources
bool CECHostPlayer::IsGathering()
{
	
	return false;
}

bool CECHostPlayer::IsOperating()
{
	return false;
}

//	Is reviving
bool CECHostPlayer::IsReviving()
{
	return false;
}

//	Is using item ?
bool CECHostPlayer::IsUsingItem()
{
	return false;
}

//	Is doing session pose ?
bool CECHostPlayer::DoingSessionPose()
{	
	return false;
}

//	Update item shortcut when item position changed
void CECHostPlayer::UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot)
{

}

//	Update item shortcut when item removed
void CECHostPlayer::UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot,
								int* aExcSlots/* NULL */, int iNumExcSlot/* 0 */)
{

}

//	Update item shortcut when two items exchanged
void CECHostPlayer::UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2)
{
	
}

//	Update shortcuts
void CECHostPlayer::UpdateShortcuts(DWORD dwDeltaTime)
{

}

//	Prepare to begin NPC service
void CECHostPlayer::PrepareNPCService(int idSev)
{
	if (!ISNPCID(m_idSevNPC))
	{
		ASSERT(0);
		return;
	}

	DATA_TYPE DataType;
	const void* pBuf = g_pGame->GetElementDataMan()->get_data_ptr(idSev, ID_SPACE_ESSENCE, DataType);

	switch (DataType)
	{
	case DT_NPC_TALK_SERVICE:

		break;

	case DT_NPC_SELL_SERVICE:
	case DT_NPC_BUY_SERVICE:
	{
		//	Fill NPC package
		NPC_SELL_SERVICE* pData = (NPC_SELL_SERVICE*)pBuf;

		for (int j=0; j < NUM_NPCIVTR; j++)
			FillNPCPack(j, pData->pages[j].page_title, (int*)pData->pages[j].id_goods, 1.0f + pData->pages[j].discount, false);
		
		//	Clear packs
		break;
	}
	case DT_NPC_TASK_IN_SERVICE:
	case DT_NPC_TASK_OUT_SERVICE:
	case DT_NPC_TASK_MATTER_SERVICE:
	case DT_NPC_HEAL_SERVICE:
	case DT_NPC_TRANSMIT_SERVICE:
	case DT_NPC_PROXY_SERVICE:
	case DT_NPC_STORAGE_SERVICE:

		break;
	}
}

//	End NPC service
void CECHostPlayer::EndNPCService()
{
	/*
	CECNPCServer* pNPC = (CECNPCServer*)g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_idSevNPC);

	if (pNPC && pNPC->IsServerNPC())
		pNPC->RestorServerDir();
		*/

	m_idSevNPC = 0;
	m_bTalkWithNPC = false;
	m_iBoothState = 0;
}

//	Buy items from NPC
void CECHostPlayer::BuyItemsFromNPC()
{

}

//	Sell items to NPC
void CECHostPlayer::SellItemsToNPC()
{

}

//	Buy items from else player's booth
void CECHostPlayer::BuyItemsFromBooth()
{

}

//	Sell items to else player's booth
void CECHostPlayer::SellItemsToBooth()
{

}

//	When the booth we are visiting closed
void CECHostPlayer::OnOtherBoothClosed()
{

}

//	Open booth
void CECHostPlayer::OpenBooth(const ACHAR* szName)
{

}

//	Release skills
void CECHostPlayer::ReleaseSkills()
{
	
}

//	Update equipment skins
bool CECHostPlayer::UpdateEquipSkins(bool bAtOnce)
{

	SetResReadyFlag(RESFG_SKIN, true);

	return true;
}

//	Get positive skill by id
CECSkill* CECHostPlayer::GetPositiveSkillByID(int id, bool bSenior/* false */)
{
	return NULL;
}

//	Get passive skill by id
CECSkill* CECHostPlayer::GetPassiveSkillByID(int id, bool bSenior/* false */)
{
	return NULL;
}

//Get addon permanent skill by index
CECSkill* CECHostPlayer::GetAddonPermanentSkillByIndex( int n )
{
	return NULL;
}

//	Get addon permanent skill by id
CECSkill* CECHostPlayer::GetAddonPermanentSkillByID( int id, bool bSenior/* = false*/ )
{
	return NULL;
}

CECSkill* CECHostPlayer::GetTransfigureSkillByID(int id, bool bSenior /* = false */)
{
	return NULL;
}
// get item skill by id
CECSkill* CECHostPlayer::GetItemSkillByID(int id, bool bSenior /* = false */)
{
	return NULL;
}
//	Get else player's skill by id
CECSkill* CECHostPlayer::GetElsePlayerSkillByID(int id, bool bSenior/* false */)
{
	return NULL;
}

//	Get skill by id
CECSkill* CECHostPlayer::GetSkill(int id, bool bSenior/* false */)
{
	return NULL;
}

//	Check whether host can learn specified skill
int CECHostPlayer::CheckSkillLearnCondition(int idSkill, bool bCheckBook)
{
	return 0;
}

//	Check whether host can cast specified skill
int CECHostPlayer::CheckSkillCastCondition(CECSkill* pSkill)
{
	return 0;
}

int CECHostPlayer::CheckSkillCostItemCondition(CECSkill* pSkill)
{
	return 0;
}

void CECHostPlayer::SetPlayerProps(CECSkill* pSkill)
{

}
//	Get skill ability
int CECHostPlayer::GetSkillAbility(int idSkill)
{
	return 0; // GNET::ElementSkill::GetAbility(idSkill);
}

//	Get skill ability percent
int CECHostPlayer::GetSkillAbilityPercent(int idSkill)
{
	return 0; // GNET::ElementSkill::GetAbilityPercent(idSkill);
}

//	Remove skill
void CECHostPlayer::RemoveSkill(int idSkill)
{

}

//	Cast skill
bool CECHostPlayer::CastSkill(int idTarget, bool bForceAttack, CECObject* pTarget/* NULL */, A3DVECTOR3* pCastSkillPos/* = NULL*/)
{

	return true;
}

//	Save configs data (shortcut, etc.)
bool CECHostPlayer::SaveConfigData(void* pDataBuf, int* piSize)
{
	int i, iSize, iTotalSize=0;
	BYTE* pData = (BYTE*)pDataBuf;

	//	Version
	iTotalSize += sizeof (DWORD);
	if (pDataBuf)
	{
		*((DWORD*)pData) = HOSTCFG_VERSION;
		pData += sizeof (DWORD);
	}

// 	//	Save shortcut configs...
// 	for (i=0; i < NUM_HOSTSCSETS1; i++)
// 	{
// 		m_aSCSets1[i]->SaveConfigData(pData, &iSize);
// 		iTotalSize += iSize;
// 		if (pData)
// 			pData += iSize;
// 	}
// 
// 	for (i=0; i < NUM_HOSTSCSETS2; i++)
// 	{
// 		m_aSCSets2[i]->SaveConfigData(pData, &iSize);
// 		iTotalSize += iSize;
// 		if (pData)
// 			pData += iSize;
// 	}

	if (piSize)
		*piSize = iTotalSize;

	return true;
}

//	Load configs data (shortcut, etc.) from specified buffer
bool CECHostPlayer::LoadConfigData(const void* pDataBuf)
{
	int i;
	BYTE* pData = (BYTE*)pDataBuf;

	//	Version number
	DWORD dwVer = *(DWORD*)pData;
	pData += sizeof (DWORD);
	if (dwVer > HOSTCFG_VERSION)
	{
		ASSERT(dwVer > HOSTCFG_VERSION);
		return false;
	}

	//	Load shortcut configs...
// 	for (i=0; i < NUM_HOSTSCSETS1; i++)
// 	{
// 		int iSize;
// 		if (!m_aSCSets1[i]->LoadConfigData(pData, &iSize, dwVer))
// 			return false;
// 
// 		pData += iSize;
// 	}
// 
// 	for (i=0; i < NUM_HOSTSCSETS2; i++)
// 	{
// 		int iSize;
// 		if (!m_aSCSets2[i]->LoadConfigData(pData, &iSize, dwVer))
// 			return false;
// 
// 		pData += iSize;
// 	}

	return true;
}

//	Calculate distance between host and specified object
bool CECHostPlayer::CalcDist(int idObject, float* pfDist, CECObject** ppObject/* NULL */)
{
	if (!idObject || idObject == m_PlayerInfo.cid)
		return false;

	CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idObject, 1);
	if (!pObject)
		return false;

	if (ppObject)
		*ppObject = pObject;

	float fDist = 0.0f;

	if (ISNPCID(idObject))
	{
		CECNPC* pNPC = (CECNPC*)pObject;
		fDist = pNPC->CalcDist(GetPos(), true);
	}
	else if (ISPLAYERID(idObject))
	{
		ASSERT(pObject->GetClassID() == CECObject::OCID_ELSEPLAYER);
		CECElsePlayer* pPlayer = (CECElsePlayer*)pObject;
		fDist = pPlayer->CalcDist(GetPos(), true);
	}
	else if (ISMATTERID(idObject))
	{
		ASSERT(pObject->GetClassID() == CECObject::OCID_MATTER);
		CECMatter* pMatter = (CECMatter*)pObject;
		fDist = a3d_Magnitude(pMatter->GetPos() - GetPos());
	}
	else
		return false;

	*pfDist = fDist;
	return true;
}

//	When all resources are ready, this function will be called
void CECHostPlayer::OnAllResourceReady()
{
	CECPlayer::OnAllResourceReady();
}

bool CECHostPlayer::SetPetLoadResult(CECModel* pPetModel)
{
	if (CECPlayer::SetPetLoadResult(pPetModel))
	{
		m_pCDRInfo->vExtent = m_aabb.Extents;
		//m_pCDRInfo->fStepHeight *= 2.0f;
		return true;
	}

	return false;
}

//	When all initial data is ready, this function is called
void CECHostPlayer::OnAllInitDataReady()
{
	if (IsDead())
	{
		//	Popup revive notify dialog
// 		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
// 		pGameUI->PopupReviveDialog(true);
	}

	m_bEnterGame = true;

}

//	Attack an object
void CECHostPlayer::NormalAttackObject(int idTarget, bool bForceAttack, bool bMoreClose/* false */)
{

}

//	Pickup an dobject
void CECHostPlayer::PickupObject(int idTarget, bool bGather)
{

}

//	Check whether host can gather specified matter
bool CECHostPlayer::CanGatherMatter(CECMatter* pMatter)
{
	return false;
}

//	Build PVP mask
BYTE CECHostPlayer::BuildPVPMask(bool bForceAttack)
{
	BYTE byMask = 0;

	if (bForceAttack)
		byMask = GP_PVPMASK_ALL;
	else
	{
		CECConfigs* pConfigs = g_pGame->GetConfigs();

		if (!pConfigs->GetGameSettings().bAtk_NoWhite)
			byMask |= GP_PVPMASK_WHITE;

		if (!pConfigs->GetGameSettings().bAtk_NoRed)
			byMask |= GP_PVPMASK_RED;

		if (!pConfigs->GetGameSettings().bAtk_NoBlue)
			byMask |= GP_PVPMASK_BLUE;

		if (!pConfigs->GetGameSettings().bAtk_NoMafia)
			byMask |= GP_PVPMASK_MAFIA;

		if (!pConfigs->GetGameSettings().bAtk_NoZone)
			byMask |= GP_PVPMASK_ZONE;
	}

	return byMask;
}

inline int _judge_pk_state(bool bForceAttack, const EC_GAME_SETTING& gs, CECPlayer* pOpp)
{
}

#define JUDGE_PK_STATE \
{\
	if (IsTeamMember(idTarget))\
		return 0;\
	else if (bForceAttack)\
		iRet = 1;\
	else if (gs.bAtk_NoBlue && m_idFamily != 0 && m_idFamily == pPlayer->GetFamilyID())\
		return 0;\
	else if (gs.bAtk_NoMafia && m_idFaction && m_idFaction == pPlayer->GetFactionID())\
		iRet = 0;\
	else\
	{\
		int opp_pk_lev = pPlayer->GetBasicProps().iPKLevel;\
\
		if (pPlayer->IsInvader())\
			iRet = (gs.bAtk_NoRed ? 0 : 1);\
		else if (opp_pk_lev == 0)\
		{\
			if (m_bSafeLock)\
				iRet = 0;\
			else\
				iRet = (gs.bAtk_NoWhite ? 0 : 1);\
		}\
		else\
			iRet = (gs.bAtk_NoRed ? 0 : 1);\
	}\
}

/*	Attackable judge. Check whether we can attack an object.

	Return value:

		-1: target has missed, or target id is invalid, or target is dead
		0: target isn't attackable
		1: target is attackable
*/
int CECHostPlayer::AttackableJudge(int idTarget, bool bForceAttack)
{
	if (CannotAttack())
		return 0;

	if (!idTarget || idTarget == m_PlayerInfo.cid)
		return -1;

	CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 1);
	if (!pObject)
		return -1;

	int iRet = 0;

	//	If target is pet, it's attacked possibility depends on it's monster
	if (ISNPCID(idTarget))
	{
		CECNPC* pNPC = (CECNPC*)pObject;
		int idMaster = pNPC->GetMasterID();
		// 飞行模式不能攻击地面怪
		if (IsFlying())
			return 0;

		if (idMaster)
		{
			// master可能是hostplayer
			if (idMaster == m_PlayerInfo.cid)
				return 0;

			idTarget = idMaster;
			pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 1);

			if (!pObject)
				return -1;
		}
		int idSummoner = pNPC->GetSummonerID();

		if (idSummoner)
		{
			// summoner may be hostplayer
			if (idSummoner == m_PlayerInfo.cid)
				return 0;

			idTarget = idSummoner;
			pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 1);

			if(!pObject)
				return -1;
		}
	}
	// 目标是分身
	if (ISCLONEPLAYERID(idTarget))
	{
		CECElsePlayer* pPlayer = (CECElsePlayer*)pObject;
		int idSummoner = pPlayer->GetSummonerID();
		if (idSummoner)
		{
			// summoner may be hostplayer
			if (idSummoner == m_PlayerInfo.cid)
				return 0;
			// 如果主人已经隐身
			CECObject * pObj = g_pGame->GetGameRun()->GetWorld()->GetObject(idSummoner, 1);
			if (pObj)
			{
				idTarget = idSummoner;
				pObject = pObj;
			}
		}
	}

	if (ISNPCID(idTarget))
	{
		CECNPC* pNPC = (CECNPC*)pObject;

		// 在飞船上不能攻击npc
		if (GetCarrierID())
			return 0;

		if (IsInBattle())	//	Host is in battle
		{
			// 竞技场中，没有阵营信息，直接判定可攻击
			if (m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN)
				iRet = 1;
			else if (InSameBattleCamp(pNPC))
				iRet = 0;
			else
				iRet = 1;
		}
		else if (m_pvp.ucFreePVPType == 3)  // 人神pk，可以攻击对方阵营的怪物
		{
			// 人神阵营pk
			if (pNPC->IsServerNPC() || pNPC->IsMonsterNPC())
			{
				if ((m_iRace == RACE_HUMAN && pNPC->IsOrcCamp()) || (m_iRace == RACE_ORC && pNPC->IsHumanCamp()) )
					iRet = 1;
			}
			else
				iRet = 0;
		}
		else if (pNPC->IsServerNPC())
		{
			//	In sanctuary we cannot attack NPCs
			if (!IsPVPOpen() || m_bInSanctuary || !bForceAttack)
				iRet = 0;
			else
				iRet = 1;
		}
		else	//	Is monster
		{
			iRet = 1;
		}
	}
	else if (ISPLAYERID(idTarget))
	{
		//	Check duel at first
		if (m_pvp.iDuelState == DUEL_ST_INDUEL && m_pvp.idDuelOpp == idTarget)
			return 1;
		else if (m_pvp.iDuelState == DUEL_ST_STOPPING && m_pvp.idDuelOpp == idTarget)
			return 0;

		//	In sanctuary we cannot attack other players
		if (m_bInSanctuary)
			return 0;

		ASSERT(pObject->GetClassID() == CECObject::OCID_ELSEPLAYER);
		CECElsePlayer* pPlayer = (CECElsePlayer*)pObject;
		// 空对地 or 地对空 的攻击都被限制了
		if ( (IsFlying() && !pPlayer->IsFlying()) || (!IsFlying() && pPlayer->IsFlying()) )
			return 0;

		// 队员不能攻击多人骑乘的主骑
		if (pPlayer->GetMultiRideLeaderId() == idTarget)
			return 0;
		// 主骑也不能攻击队员
		for (int i = 0; i< MULTI_RIDE_CAPACITY; ++i)
		{
			if (idTarget == m_iMultiBuddyId[i])
				return 0;
 		}

		// 如果任何一方在载体状态下，不能攻击
		if (GetCarrierID() || pPlayer->GetCarrierID())
			return 0;

		// 如果在竞技场中，自由pk
		if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR )
			return 1;
				
		const ROLEBASICPROP& bp = pPlayer->GetBasicProps();
		const EC_GAME_SETTING& gs = g_pGame->GetConfigs()->GetGameSettings();
		
		// 大跨服地图中
		if (m_idOrgServer) 
		{  
			if ( m_idOrgServer != pPlayer->GetSrcServerID())
				return 1;
			else
			{
				if ((gs.bAtk_NoZone && !bForceAttack) || IsTeamMember(idTarget))
					return 0;
				else 
					return 1;
			}
		}
// 		if (m_idOrgServer && m_idOrgServer == pPlayer->GetSrcServerID() && !gs.bAtk_NoZone)
// 			return 1;

		if (m_pvp.ucFreePVPType == 1)
		{
			JUDGE_PK_STATE
		}
		else if (m_pvp.ucFreePVPType == 2)
		{
			if (IsTeamMember(idTarget))
				iRet = 0;
			else if (GetCultivation() == pPlayer->GetCultivation())
				iRet = 0;
			else
				iRet = 1;
		}
		else if (m_pvp.ucFreePVPType == 3)
		{
			// 人神阵营pk
			if (m_iRace == pPlayer->GetRace())
				iRet = 0;
			else
				iRet = 1;
		}
		else if (m_iBattleCamp != GP_BATTLE_CAMP_NONE)
		{
			//	Host is in battle
			int iCamp = pPlayer->GetBattleCamp();
			if (iCamp != GP_BATTLE_CAMP_NONE && iCamp != m_iBattleCamp)
				iRet = 1;
			else
				iRet = 0;
		}
		else if (m_nCombatFaction && m_nCombatFaction == pPlayer->GetFactionID())
			iRet = 1;
		// 查看该线是否是PK保护，如果PK保护则不可以攻击白名玩家
		else if (g_pGame->GetGameRun()->IsNoPKLine() && bp.iPKLevel == 0)
			return 0;
		else	//	Normal mode
		{
			if (!IsPVPOpen() || !pPlayer->IsPVPOpen())
				return 0;

			JUDGE_PK_STATE
		}
	}
	else
	{
		return -1;
	}

	return iRet;
}

//	Decide target name color basing on target level
DWORD CECHostPlayer::DecideTargetNameCol(int iTargetLevel)
{
	int iDelta = m_BasicProps.iLevel - iTargetLevel;
	DWORD dwCol = 0xffffffff;

	if (iDelta >= 15)
		dwCol = A3DCOLORRGB(119, 119, 119);
	else if (iDelta >= 9)
		dwCol = A3DCOLORRGB(10, 113, 0);
	else if (iDelta >= 5)
		dwCol = A3DCOLORRGB(145, 255, 64);
	else if (iDelta >= 0)
		dwCol = A3DCOLORRGB(255, 255, 255);
	else if (iDelta >= -7)
		dwCol = A3DCOLORRGB(255, 185, 210);
	else if (iDelta >= -14)
		dwCol = A3DCOLORRGB(255, 79, 79);
	else
		dwCol = A3DCOLORRGB(128, 0, 0);

	return dwCol;
}

//	Check whether host can do a behavior
bool CECHostPlayer::CanDo(int iThing)
{
	bool bRet = true;

	switch (iThing)
	{
	case CANDO_SITDOWN:

		if (IsDead() || IsAboutToDie() || IsJumping() || IsTrading() || IsUsingTrashBox() || 
			IsRooting() || IsReviving() || IsTalkingWithNPC() || IsChangingFace() ||
			!m_GndInfo.bOnGround ||	GetBoothState() != 0 || m_iBuddyId || IsOperatingPet() ||
			IsUsingItem() || IsRidingOnPet() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
		
	case CANDO_MOVETO:
	{
		if (IsDead() || IsSitting() || IsTrading() || IsUsingTrashBox() || IsRooting() ||
			IsReviving() || IsTalkingWithNPC() || IsChangingFace() || IsUsingItem() ||
			GetBoothState() != 0 || m_bHangerOn || IsOperatingPet() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;
	
		break;
	}
	case CANDO_MELEE:

		if (IsDead() || IsSitting() || !m_idSelTarget || m_idSelTarget == m_PlayerInfo.cid ||
			IsJumping() || ISMATTERID(m_idSelTarget) || IsTrading() || IsReviving() || 
			IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || CannotAttack() ||
			GetBoothState() != 0 || m_iBuddyId || IsOperatingPet() ||
			IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		if (m_pTransfiguredModel)
		{
			if (!m_TransfigureProp.bCanFight)
			{
				g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_ATTACK_DISABLED);
				bRet = false;
			}
		}
		break;

	case CANDO_ASSISTSEL:

		break;

	case CANDO_FLY:

		break;

	case CANDO_PICKUP:
	case CANDO_GATHER:
	case CANDO_PRODUCE:

		if (IsDead() || IsAboutToDie() || IsSitting() || IsTrading() || IsUsingTrashBox() ||
			IsReviving() || IsTalkingWithNPC() || IsChangingFace() || GetBoothState() != 0 ||
			GetBuddyState() == 1 || IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;

	case CANDO_TRADE:

		if (IsDead() || IsAboutToDie() || IsSitting() || IsJumping() || IsMeleeing() || 
			IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || 
			IsSpellingMagic() || GetBoothState() != 0 || m_iBuddyId || IsOperatingPet() ||
			IsUsingItem() || IsProducing() || m_bMultiRideMember || m_iBattleType == BATTLE_TYPE_ARENA ||
			m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
			bRet = false;

		break;

	case CANDO_PLAYPOSE:
	
		if (IsDead() || IsAboutToDie() || IsSitting() || IsJumping() || IsMeleeing() ||
			IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || 
			IsSpellingMagic() || IsShapeChanged() || IsReviving() || m_iMoveEnv != MOVEENV_GROUND ||
			GetBoothState() != 0 || m_iBuddyId || IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
	
	case CANDO_SPELLMAGIC:
	case CANDO_SUMMONPET:

		if (IsDead() || ISMATTERID(m_idSelTarget) || IsAboutToDie() || IsSitting() ||
			IsJumping() || IsTrading() || IsUsingTrashBox() ||	IsTalkingWithNPC() ||
			IsChangingFace() || CannotAttack() || IsReviving() || GetBoothState() != 0 ||
			m_iBuddyId || IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember || 
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE) || m_bFlyingOff)
			bRet = false;

		break;

	case CANDO_USEITEM:

		if (IsAboutToDie() || IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || GetBoothState() != 0 || IsProducing())
			bRet = false;

		break;
		
	case CANDO_JUMP:
	{	
		if (IsDead() || m_iJumpCount >= max_jump_count || IsJumpInWater() || m_iMoveEnv == MOVEENV_AIR || IsSitting() || 
			IsMeleeing() || IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || IsReviving() || IsSpellingMagic() || IsPicking() ||
			IsGathering() || IsRooting() ||	GetBoothState() != 0 || m_bHangerOn || (IsJumping() && IsRidingOnPet()) ||
			IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
	}
	case CANDO_FOLLOW:
	{	
		if (IsDead() || IsAboutToDie() || IsSitting() || IsMeleeing() || IsReviving() || 
			IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || 
			IsSpellingMagic() || GetBoothState() != 0 || m_bHangerOn || IsOperatingPet() ||
			IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
	}
	case CANDO_AUTOMOVE:
	{
		if (IsDead() || IsAboutToDie() || IsSitting() || IsMeleeing() || IsReviving() || 
			IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || 
			IsSpellingMagic() || GetBoothState() != 0 || m_bHangerOn || IsOperatingPet() ||
			IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
	}
	case CANDO_BOOTH:
		
		if (IsDead() || IsAboutToDie() || IsPlayerMoving() || IsSitting() || IsReviving() || 
			IsMeleeing() || IsJumping() || IsTrading() || IsUsingTrashBox() || 
			IsTalkingWithNPC() || IsChangingFace() || IsSpellingMagic() || IsFlying() ||
			IsUnderWater() || m_iBuddyId || IsOperatingPet() || IsUsingItem() || IsRidingOnPet() ||
			IsProducing() || m_bMultiRideMember || m_TransfigureInfo.id)
			bRet = false;

		break;

	case CANDO_FLASHMOVE:
		
		
	
		break;
		
	case CANDO_BINDBUDDY:
		
	
		break;
		
	case CANDO_DUEL:



		break;

	case CANDO_BINDMULTIBUDDY:

	
		break;

	case CANDO_BODYCHANGE:


		break;
	}

	return bRet;
}

bool CECHostPlayer::IsPlayerMoving()
{
	return false;
}

//	Can jump or take off in water ?
bool CECHostPlayer::CanTakeOffWater()
{
	A3DVECTOR3 vPos = GetPos();
	if (vPos.y < g_pGame->GetGameRun()->GetWorld()->GetWaterHeight(vPos) - m_MoveConst.fShoreDepth)
		return false;
	else
		return true;
}

//	Get cool time
int CECHostPlayer::GetCoolTime(int iIndex, int* piMax/* NULL */)
{
	if (iIndex >= 0 && iIndex < GP_CT_MAX)
	{
		if (piMax)
			*piMax = m_aCoolTimes[iIndex].iMaxTime; 

		return m_aCoolTimes[iIndex].iCurTime;
	}

	return 0;
}

//	Get produce cool time
int CECHostPlayer::GetProduceCoolTime(int iIndex, int* piMax/* NULL */)
{
	if (iIndex >= 0 && iIndex <= GP_CT_PRODUCE_END - GP_CT_PRODUCE_START )
	{
		if (piMax)
			*piMax = m_aProduceCoolTimes[iIndex].iMaxTime; 

		return m_aProduceCoolTimes[iIndex].iCurTime;
	}

	return 0;
}

// Get Skill Edit cool time
int CECHostPlayer::GetSkillEditCoolTime(int iIndex, int* piMax)
{
	if (iIndex >= 0 && iIndex <= GP_CT_COMBINE_EDIT_END - GP_CT_COMBINE_EDIT_START )
	{
		if (piMax)
			*piMax = m_aSkillEditCoolTimes[iIndex].iMaxTime; 

		return m_aSkillEditCoolTimes[iIndex].iCurTime;
	}

	return 0;
}

//	Get time counter of using item in pack
bool CECHostPlayer::GetUsingItemTimeCnt(DWORD& dwCurTime, DWORD& dwMaxTime, int* piItem/* NULL */)
{
	return true;
}

//	Fill NPC packs
void CECHostPlayer::FillNPCPack(int iIndex, const ACHAR* szName, int* aItems, 
								float fPriceScale, bool bRecipe)
{

}

//	Set absolute position
void CECHostPlayer::SetPos(const A3DVECTOR3& vPos)
{
	A3DVECTOR3 _pos = vPos;


	CECPlayer::SetPos(_pos);

	if (m_iBuddyId)
	{
		if (m_bHangerOn)	//	Host is rider
		{
			m_MoveCtrl.SetLastSevPos(_pos);
		}
		else	//	Host is mule
		{
			CECElsePlayer* pBuddy = m_pPlayerMan->GetElsePlayer(m_iBuddyId);
			if (pBuddy)
				pBuddy->SetServerPos(_pos);
		}
	}
}

//	Set absolute forward and up direction
void CECHostPlayer::SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp)
{
	CECPlayer::SetDirAndUp(vDir, vUp);
}

void CECHostPlayer::Killed(int idKiller)
{
	if( m_pPlayerModel )
		m_pPlayerModel->ClearComActFlag(true);

	m_dwStates |= GP_STATE_CORPSE;


}

//	Get off pet
void CECHostPlayer::GetOffPet()
{
	CECPlayer::GetOffPet();

	//	Restore cdr info
	m_pCDRInfo->vExtent		= m_aabb.Extents;
	//m_pCDRInfo->fStepHeight	= m_MoveConst.fStepHei;
}

//	Find mine tool in packages
bool CECHostPlayer::FindMineTool(int tidMine, int* piPack, int* piIndex, int* pidTool)
{
	if (!tidMine)
		return false;

	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(tidMine, ID_SPACE_ESSENCE, DataType);

	if (DataType != DT_MINE_ESSENCE)
	{
		ASSERT(DataType != DT_MINE_ESSENCE);
		return false;
	}

	const MINE_ESSENCE* pData = (const MINE_ESSENCE*)pDataPtr;
	int idTool = pData->id_equipment_required;
	bool bRet = true;

	

	return bRet;
}

//	When host is going to move, this function will be called
void CECHostPlayer::GoingMove()
{
	
}

//	Update positions of team member
void CECHostPlayer::UpdateTeamMemberPos(DWORD dwDeltaTime)
{
}

//	Freeze / Unfreeze specified item
bool CECHostPlayer::FreezeItem(int iIvtr, int iIndex, bool bFreeze, bool bFreezeByNet)
{
	return true;
}

//	Check whether player pickup specified matter
bool CECHostPlayer::CanTakeItem(int idItem, int iAmount)
{
	bool bCanPick = false;

	return bCanPick;
}

//	Get force attack flag
bool CECHostPlayer::GetForceAttackFlag(const DWORD* pdwParam)
{
	bool bForceAttack = false;
	return bForceAttack;
}

//	Does host player have specified way point ?
bool CECHostPlayer::HasWayPoint(WORD wID)
{
	for (int i=0; i < m_aWayPoints.GetSize(); i++)
	{
		if (m_aWayPoints[i] == wID)
			return true;
	}

	return false;
}

//	Team invite
void CECHostPlayer::TeamInvite(const GNET::PlayerBriefInfo& Info)
{
	
}

//	Rearrange booth packages
void CECHostPlayer::RearrangeBoothPacks()
{

}

//	Get deadly strike rate (%)
int CECHostPlayer::GetDeadlyStrikeRate()
{
	return (int)0;
}

//	Is host in sliding state (in the state, host is sliding on slope) ?
bool CECHostPlayer::InSlidingState()
{
	if (m_iMoveMode != MOVE_SLIDE)
		return false;

	return true;
}

//	On start binding buddy
void CECHostPlayer::OnStartBinding(int idMule, int idRider)
{
	if (m_iChangeShape)
		return;

	CECPlayer::OnStartBinding(idMule, idRider);

}

//	Apply combo skill
bool CECHostPlayer::ApplyComboSkill(int iGroup)
{
	// first of all see if we need to cancel sitdown work.

	return true;
}

//	Clear combo skill
void CECHostPlayer::ClearComboSkill()
{
	
}

//	Replace specified skill with it's senior skill
void CECHostPlayer::ReplaceJuniorSkill(int idJuniorSkill, CECSkill* pSeniorSkill)
{

}

CECSkill* CECHostPlayer::InsertAddonSkill(int idSkill, int lev)
{
	return NULL;
}

//	Do emote action
bool CECHostPlayer::DoEmote(int idEmote)
{
	return true;
}

void CECHostPlayer::StartGeneralOprt(int op_id, int duration)
{
	
}

void CECHostPlayer::StopGeneralOprt(int op_id)
{
	
}

//	Hatch pet
bool CECHostPlayer::HatchPet(int iIvtrIdx)
{

	return true;
}

//	Restore pet
bool CECHostPlayer::RestorePet(int iPetIdx)
{

	return true;
}

//	Summon pet
bool CECHostPlayer::SummonPet(int iPetIdx)
{

	return true;
}

//	Recall pet
bool CECHostPlayer::RecallPet()
{

	return true;
}

//	Banish pet
bool CECHostPlayer::BanishPet(int iPetIdx)
{
	return true;
}

//	Get battle pet index
int CECHostPlayer::GetBattlePetIndex()
{
	
	return -1;
}

/*	Is host operating pet ?
	return value:

	0: host doesn't operating pet.
	1: host is summoning pet.
	2: host is recalling pet.
*/
int CECHostPlayer::IsOperatingPet()
{
	return 0;
}

//	Reset cdr info
void CECHostPlayer::ResetCDRInfo()
{
	if (m_pCDRInfo)
	{
		m_pCDRInfo->vTPNormal.Clear();
		m_pCDRInfo->bBlocked	= false;
		m_pCDRInfo->bOnSurface	= false;
		m_pCDRInfo->bCanStay	= false;
	}
}

//	Get number of equipped items of specified suite
int CECHostPlayer::GetEquippedSuiteItem(int idSuite, int* aItems/* NULL */)
{
	int i, iItemCnt = 0;


	return iItemCnt;
}

//	Search the full suite
int CECHostPlayer::SearchFullSuite()
{
	
	return 0;
}

void CECHostPlayer::SortTitle()
{
	std::sort(m_vecTitles.begin(), m_vecTitles.end());
}

void CECHostPlayer::OnWeaponChanged()
{
	if( IsJumping() )
	{
		PlayAction(ACT_JUMP_LOOP, 1.0f, true);
	}
	else if(m_pPlayerModel)
	{
		// replay current action to yield weapon change result
		PlayAction(m_pPlayerModel->GetCurActionUserData(ACT_CHANNEL_BODY), 1.0f, true);
	}
}

void CECHostPlayer::ResetFactionCombatCounts()
{
	m_nFactionCombatCounts = 0;
}

void CECHostPlayer::ResetGuildWarStartCounts()
{
	m_nGuildWarStartCounts = 0;
}

void CECHostPlayer::ResetGuildWarWinCounts()
{
	m_nGuildWarWinCounts = 0;
}

void CECHostPlayer::ResetGuildWarLostCounts()
{
	m_nGuildWarLostCounts = 0;
}

void CECHostPlayer::Stand()
{
	int iCurrHP = m_BasicProps.iCurHP;
	m_BasicProps.iCurHP = m_ExtProps.max_hp;
	PlayAction(CECPlayer::ACT_STAND, 1.0f, false);
	m_BasicProps.iCurHP = iCurrHP;
}

bool CECHostPlayer::IsUnderWickedAtk() 
{
	// If m_bUnderEPAtk is set
	// Means HostPlayer has been attacked by another player in the last 3000ms
	return m_bUnderEPAtk && 

	// No matter in what state the host player is, It has just been attacked by another player now
	// Thus the AttackableJudge has been passed
	// So we just simplely ignore the specific state
	// If the hostplayer is in battle / duel / combatfaction, the only players who can attack the host can only be the hostiles
	// So, if in battle / duel / faction and attacked, that is not a Wicked Attack
	!IsInBattle() && !IsInDuel() && !GetCombatFaction() &&

	// Meanwhile,
	// If the hostplayer is an Invader() ---- which means the host player has recently attacked a player with 0 PKLevel
	// An attack from other player is not considered as a Wicked Attack
	!IsInvader() && 

	// Unknown Meaning for the IsInWar, For Aggressing a city?
	!IsInWar() && 

	// Futher,
	// If the hostplayer's PKLevel is higher than 0 which means he is an Red-Named Player
	// An attack from other player is not considered as a Wicked Attack
	GetBasicProps().iPKLevel == 0 && 
	
	// At last,
	// If the hostplayer is attacking another player
	// Do not consider an attack from another player is a Wicked Attack
	!IsAttackingOtherPlayer(); 
}

void CECHostPlayer::SetUnderAtk(bool bIgnoreNormalAtk) 
{ 
	if (bIgnoreNormalAtk && 
		IsInBattle() || IsInDuel() || GetCombatFaction())
		return;

	m_bUnderEPAtk = true; 
	m_UnderAtkCnt.Reset(); 
}

void CECHostPlayer::SetBuffState(const S2C::self_buff_notify* cmd)
{
	
}

void CECHostPlayer::ClearApoiseSkill()
{

}

void CECHostPlayer::SetFashionHotkey(int iIndex, int id_head, int id_body, int id_shoe)
{
	ASSERT(iIndex>=0 && iIndex<FASHION_HOTKEY_NUM);
	m_aFashionHotKey[iIndex].id_head = id_head;
	m_aFashionHotKey[iIndex].id_body = id_body;
	m_aFashionHotKey[iIndex].id_shoe = id_shoe;
}

void CECHostPlayer::MoveItemPack2Pocket()
{
	
}

void CECHostPlayer::SetSelfReferenceCode(const void* p, int len)
{
	
}

void CECHostPlayer::SetReferralCode(const void* p, int len)
{
	 
}

const ACHAR* CECHostPlayer::GetSelfReferenceCode() const 
{ 
	return m_strSelfReferenceCode; 
}

void CECHostPlayer::ReceiveBonusExp(double exp)
{
	m_BasicProps.exp += exp;

}

void CECHostPlayer::SetTransfigureProp(int id, int level, int exp_level)
{

}

void CECHostPlayer::ClearTransfigureProp()
{
	memset(&m_TransfigureProp, 0, sizeof(m_TransfigureProp));
}

bool CECHostPlayer::IsAutoTeam(int id, int iFactionId, int iFamilyId, ACHAR *szName)
{
	
	return false;
}

int CECHostPlayer::GetRefineItemNum(int id, int level, int con)
{
	return 0;
}

bool CECHostPlayer::IsScriptFinishTask(int idTask)
{
	abase::hash_map<unsigned short, bool>::iterator it = m_ScriptFinishTaskMap.find(idTask);
	if (it != m_ScriptFinishTaskMap.end())
		return true;
	return false;
}

void CECHostPlayer::AddScriptFinishTask(int idTask)
{ 
	 
}

int CECHostPlayer::GetFriendNum()
{
	return 0;
}

bool CECHostPlayer::IsInSociety(int id, ACHAR* name)
{
	return false;
}

unsigned short* CECHostPlayer::GetCombineSkillElems(int skill_id, int& num)
{
	return NULL;
}

void CECHostPlayer::UpdateCombineSkillElems(int skill_id, int* data, int num)
{

}

int CECHostPlayer::GetTerritoryScore()
{
	return 0;
}

int CECHostPlayer::GetCircleScore()
{
	return 0;
}

bool CECHostPlayer::IsDeityExpFull()
{
	return m_BasicProps.deityExp >= GetDeityLevelUpExp(m_BasicProps.iDeityLevel + 1);
}

void CECHostPlayer::UpdateSpecialState(int state, bool on)
{

}
