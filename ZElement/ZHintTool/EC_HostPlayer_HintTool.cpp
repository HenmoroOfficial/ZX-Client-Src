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
#include "EC_HostPlayer_HintTool.h"
#include "EC_GameRun_HintTool.h"
#include "EC_Game_HintTool.h"
//#include "EC_InputCtrl.h"
//#include "EC_Model.h"
//#include "EC_Utility.h"
//#include "EC_World.h"
#include "EC_GameSession_HintTool.h"
/*#include "EC_CDS.h"
#include "EC_ShadowRender.h"
#include "EC_PortraitRender.h"
#include "EC_Viewport.h"
#include "EC_NPCServer.h"
#include "EC_Matter.h"
*/#include "EC_RTDebug.h"
/*#include "EC_ManNPC.h"
#include "EC_ManMatter.h"
#include "EC_ManPlayer.h"
#include "EC_GameUIMan.h"
*/#include "EC_UIManager_HintTool.h"
#include "EC_FixedMsg.h"
#include "EC_Inventory.h"
#include "EC_DealInventory.h"
#include "EC_IvtrScroll.h"
#include "EC_IvtrTypes.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrPetItem.h"
//#include "EC_ElsePlayer.h"
//#include "EC_GFXCaster.h"
//#include "EC_Resource.h"
//#include "EC_ShortcutSet.h"
#include "EC_Skill.h"
//#include "EC_TaskInterface.h"
//#include "EC_HostInputFilter.h"
#include "EC_Configs.h"
//#include "EC_PateText.h"
//#include "EC_Team.h"
//#include "EC_Friend.h"
#include "EC_Faction_HintTool.h"
//#include "EC_Sprite.h"
#include "EC_PetCorral.h"
//#include "EC_ComboSkill.h"
//#include "EC_LoginPlayer.h"
//#include "EC_CDR.h"
//#include "EC_Helper.h"
//#include "EC_NPCCarrier.h"
//#include "EC_Secretary.h"
//#include "TaskTemplMan.h"
/*
#include "EC_HPWork.h"
#include "EC_HPWorkUse.h"
#include "EC_HPWorkDead.h"
#include "EC_HPWorkMelee.h"
#include "EC_HPWorkStand.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkFollow.h"
#include "EC_HPWorkAutoMove.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkFly.h"
#include "EC_HPWorkSit.h"
#include "EC_HPWorkFall.h"
#include "defence/stackchecker.h"

#include "DlgTalisman.h"
#include "DlgPetPackage.h"
*/
#include "roleinfo"
//#include "playerbriefinfo"
//#include "Network\\gnetdef.h"
/*
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
#include "A3DTerrainWater.h"
#include "A3DSkinMan.h"
#include "A3DSkin.h"
*/
#include "elementdataman.h"
/*
#include "AMSoundBufferMan.h"
#include "AMSoundBuffer.h"
#include "EC_GameRecord.h"
#include "DlgChat.h"
#include "EL_Precinct.h"
#include "EC_World.h"
#include "EC_Achievement.h"
#include "EC_Cutscene.h"
#include "EC_Circle.h"
*/
#include "LuaEvent.h"

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

CECHostPlayer::CECHostPlayer(/*CECPlayerMan* pPlayerMan*/)/* : 
CECPlayer(pPlayerMan),
m_MoveCtrl(this)*/
{
/*	m_iCID				= OCID_HOSTPLAYER;
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
	m_nTalentValue		= 0;
	m_nBattleScore		= 0;
	m_nSJBattleScore	= 0;
	m_iReviveCountDown  = 0;
*/
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

/*	m_pMoveTargetGFX	= NULL;
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
*/	m_pPetCorral		= NULL;
/*	m_pComboSkill		= NULL;

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
	m_bCarrierCooling = false;
	m_nTerritoryBattle = -1;
	m_iLastLogoutTime	= 0;
	m_bHasCachePos		= false;

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
//	memset(m_aBackUpEquips, 0, sizeof(m_aBackUpEquips));
*/
	int i;
	for (i=0; i < GP_CT_MAX; i++)
	{
		m_aCoolTimes[i].iCurTime = 0;
		m_aCoolTimes[i].iMaxTime = 0;
	}
/*
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
*/
	//	Create inventories
	if (!CreateInventories())
	{
		a_LogOutput(1, "CECHostPlayer::CECHostPlayer, Failed to create host's inventoies");
	}
/*
	//	Create shortcuts
	if (!CreateShortcutSets())
	{
		a_LogOutput(1, "CECHostPlayer::CECHostPlayer, Failed to create host's shortcut sets");
	}
*/
}

CECHostPlayer::~CECHostPlayer()
{
//	delete m_pCDRInfo;
//	delete m_pAirCDRInfo;
}

//	Initlaize object
bool CECHostPlayer::Init(const S2C::cmd_self_info_1& Info)
{
/*	m_PlayerInfo.cid	= Info.cid;
	m_PlayerInfo.crc_e	= Info.crc_e;
*/
	m_BasicProps.exp		= Info.exp;
	m_BasicProps.iPKLevel	= Info.pk_level;
	m_BasicProps.iLevel		= Info.level;
	m_BasicProps.iProfession= Info.profession;
//	m_iReputation			= Info.reputation;
//	m_nRebornCount			= Info.reborn_count;
//	m_iCultivation			= Info.cultivation;
//	m_dwHideEquipMask		= Info.hide_equip;
//	m_iRace					= CECPlayer::GetRaceByProf(Info.profession);
//	m_iPhysique				= CECPlayer::GetPhysiqueByProf(Info.profession);
//	m_iRace					= g_pGame->GetGameInit().iRaceTest;
//	m_iRace					= RACE_ORC;
	//	Create work manager
/*	if (!(m_pWorkMan = new CECHPWorkMan(this)))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
		return false;
	}

	//	Create friend manager
	if (!(m_pFriendMan = new CECFriendMan))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
		return false;
	}
*/
	//	Create pet corral
	if (!(m_pPetCorral = new CECPetCorral))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
		return false;
	}

	//	Create camera path
/*	if (!(m_pCameraPath = new CECCameraPath))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
		return false;
	}

	//	Create cutscene
	if (!(m_pCutscene = new CECCutscene))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
		return false;
	}
	m_pCutscene->Init();

	//	Load resources
	if (!LoadResources())
	{
		a_LogOutput(1, "CECHostPlayer::Init, Failed to load resources");
		return false;
	}

	//	Get host's necessary data
	g_pGame->GetGameSession()->c2s_CmdGetAllData(true, true, false, true);

	if (!(m_pAchievementMan = new CECAchievementMan()) || !m_pAchievementMan->Init())
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::Init", __LINE__);
	}
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
	g_pGame->GetGameSession()->friend_GetList();
	g_pGame->GetGameSession()->friend_GetOfflineMsg();
	m_iGetFriendCnt = 60000;

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
*/
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
/*	if (Info.state & GP_STATE_IN_MOUNT)
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
				g_pGame->GetGameSession()->GetPlayerBriefInfo(1, &idPlayer, 2);
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

	if (title)
		SetTitle(title);

	//	Stand and do nothing
	m_pWorkMan->StartWork_p0(m_pWorkMan->CreateWork(CECHPWork::WORK_STAND));
	if (IsDead())
	{
		CECHPWorkDead* pWork = (CECHPWorkDead*)m_pWorkMan->CreateWork(CECHPWork::WORK_DEAD);
		pWork->SetBeDeadFlag(true);
		m_pWorkMan->StartWork_p0(pWork);
	}
	else if (IsSitting())
	{
		CECHPWorkSit* pWork = (CECHPWorkSit*)m_pWorkMan->CreateWork(CECHPWork::WORK_SIT);
		pWork->SetBeSittingFlag(true);
		m_pWorkMan->StartWork_p1(pWork);
	}

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
	
	if( g_pGame->GetGameRun() && g_pGame->GetGameRun()->GetHelper() )
	{
		//load helper settings
		g_pGame->GetGameRun()->GetHelper()->Load(m_PlayerInfo.cid);

		//load helper monster settings
		CMonsterSelector * pMonsterSelector = g_pGame->GetGameRun()->GetHelper()->GetMonsterSelector();
		if (pMonsterSelector)
		{
			pMonsterSelector->Load(m_PlayerInfo.cid);
			pMonsterSelector->LoadMonsterList(m_PlayerInfo.cid);
		}
		//load helper  pickup include and exclude list
		CMatterSelector * pMatterSelector = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector();

		if (pMatterSelector)
		{
			pMatterSelector->Load(m_PlayerInfo.cid, CMatterSelector::MATTER_INCLUDE);
			pMatterSelector->Load(m_PlayerInfo.cid, CMatterSelector::MATTER_EXCLUDE);
		}
	}
	// 第一次进入游戏
//	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
//	if (info.lastlogin_time < (info.create_time+10))
		g_pGame->GetGameRun()->GetSecretary()->SetReady(true);
*/	return true;
}
/*
bool CECHostPlayer::gen_record_msg(ECMSG& msg)
{
	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	return pCurWork ? pCurWork->gen_record_msg(msg) : false;
}

GNET::Octets CECHostPlayer::gen_skill_addon_data()
{
	GNET::Octets o;
	unsigned short count = 0;
	add_to_octets<unsigned short>(o, count);

	for (int i = 0; i < m_aPsSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPsSkills[i];

		if (pSkill->GetAddonLevel() > 0)
		{
			add_to_octets<int>(o, pSkill->GetSkillID());
			add_to_octets<int>(o, pSkill->GetSkillLevel());
			count++;
		}
	}

	for (i = 0; i < m_aPtSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPtSkills[i];

		if (pSkill->GetAddonLevel() > 0)
		{
			add_to_octets<int>(o, pSkill->GetSkillID());
			add_to_octets<int>(o, pSkill->GetSkillLevel());
			count++;
		}
	}

	memcpy(o.begin(), &count, sizeof(count));
	return o;
}

void CECHostPlayer::set_skill_addon(GNET::Octets& o)
{
	char* pBuf = (char*)o.begin();
	unsigned short count = *(unsigned short*)pBuf;
	pBuf += sizeof(count);

	struct add_on
	{
		int id;
		int lev;
	};

	if (sizeof(count) + count * sizeof(add_on) != o.size())
		return;

	add_on* p = (add_on*)pBuf;

	for (unsigned short i = 0; i < count; i++)
	{
		CECSkill* pSkill = GetSkill(p[i].id);

		if (pSkill)
			pSkill->SetAddonLevel(p[i].lev);
	}
}

GNET::GamedataSend CECHostPlayer::gen_self_info()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(sizeof(S2C::cmd_self_info_1) + 64);
	cmd_header h;
	h.cmd = SELF_INFO_1;
	add_to_octets<cmd_header>(o, h);

	add_to_octets<double>			(o, m_BasicProps.exp);
	add_to_octets<int>				(o, m_PlayerInfo.cid);
	add_to_octets<A3DVECTOR3>		(o, GetPos());
	add_to_octets<unsigned short>	(o, m_PlayerInfo.crc_e);
	add_to_octets<unsigned char>	(o, glb_CompressDirH(GetDir().x, GetDir().z));
	add_to_octets<unsigned char>	(o, m_BasicProps.iProfession);
	add_to_octets<unsigned char>	(o, m_BasicProps.iLevel);
	add_to_octets<char>				(o, m_BasicProps.iPKLevel);
	add_to_octets<int>				(o, m_iReputation);

	size_t offset = o.size();
	int state = 0;
	add_to_octets<int>				(o, state);

	if (m_dwExtStates)
	{
		state |= GP_STATE_EXTEND_PROPERTY;
		add_to_octets<__int64>			(o, m_dwExtStates);
	}

	if (m_idFaction)
	{
		state |= GP_STATE_FACTION;
		add_to_octets<int>			(o, m_idFaction);
	}

	if (m_iBoothState == 2)
	{
		state |= GP_STATE_BOOTH;
		add_to_octets<char>			(o, m_crcBooth);
	}

	if (m_aCurEffects.GetSize())
	{
		state |= GP_STATE_EFFECT;
		add_to_octets<unsigned char>(o, m_aCurEffects.GetSize());
		
		for (int i = 0; i < m_aCurEffects.GetSize(); i++)
			add_to_octets<short>	(o, m_aCurEffects[i]);
	}

	if (m_RidingPet.id)
	{
		state |= GP_STATE_IN_MOUNT;
		add_to_octets<unsigned char>(o, m_RidingPet.iLevel);
		add_to_octets<int>(o, m_RidingPet.id);
		add_to_octets<unsigned char>(o, m_RidingPet.bMultiRider? 1:0);
	}

	if (m_iBuddyId)
	{
		state |= GP_STATE_IN_BIND;
		add_to_octets<char>			(o, 0);
		add_to_octets<int>			(o, m_iBuddyId);
	}

	if (m_iTitle)
	{
		state |= GP_STATE_TITLE;
		add_to_octets<short>		(o, m_iTitle);
	}

	if (m_idSpouse)
	{
		state |= GP_STATE_SPOUSE;
		add_to_octets<int>			(o, m_idSpouse);
	}

	if (m_idFamily)
	{
		state |= GP_STATE_FAMILY;
		add_to_octets<int>			(o, m_idFamily);
		add_to_octets<char>			(o, m_idFRole);
	}

	if (m_bFashionMode)
		state |= GP_STATE_FASHION;

	if (m_iBattleCamp == GP_BATTLE_CAMP_INVADER)
		state |= GP_STATE_BC_INVADER;
	else if (m_iBattleCamp == GP_BATTLE_CAMP_DEFENDER)
		state |= GP_STATE_BC_DEFENDER;

	if (m_dwGMFlags)
		state |= GP_STATE_GMFLAG;

	if (m_pvp.bEnable)
		state |= GP_STATE_PVP_ENABLE;

	if (m_pvp.bInPVPCombat)
		state |= GP_STATE_INPVPCOMBAT;

	if (IsDead())
		state |= GP_STATE_CORPSE;

	if (IsSitting())
		state |= GP_STATE_SITDOWN;

	// set state flag
	memcpy((char*)o.begin() + offset, &state, sizeof(state));
	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_self_info_00()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(sizeof(S2C::cmd_self_info_00) + 64);
	cmd_header h;
	h.cmd = SELF_INFO_00;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<short>(o, m_BasicProps.iLevel);
	add_to_octets<unsigned char>(o, m_bFight ? 1 : 0);
	add_to_octets<unsigned char>(o, 0);
	add_to_octets<int>(o, m_BasicProps.iCurHP);
	add_to_octets<int>(o, m_ExtProps.max_hp);
	add_to_octets<int>(o, m_BasicProps.iCurMP);
	add_to_octets<int>(o, m_ExtProps.max_mp);
	add_to_octets<int>(o, m_BasicProps.iTalismanStamina);
	add_to_octets<double>(o, m_BasicProps.exp);
	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_cmd_own_ext_prop()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = OWN_EXT_PROP;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<size_t>(o, m_BasicProps.iStatusPt);
	add_to_octets<ROLEEXTPROP>(o, m_ExtProps);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_cmd_get_own_money()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = GET_OWN_MONEY;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_iMoneyCnt);
	add_to_octets<int>(o, m_iMaxMoney);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_player_cash()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = PLAYER_CASH;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_nCash);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_pk_value()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = PK_VALUE_NOTIFY;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_PlayerInfo.cid);
	add_to_octets<int>(o, m_nPKValue);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_mafia_contribution()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = PLAYER_MAFIA_CONTRIBUTION;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_nContribution);
	add_to_octets<int>(o, m_nFamilyContrb);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_region_reputation()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	cmd_header h;
	h.cmd = PLAYER_REGION_REPUTATION;
	add_to_octets<cmd_header>(o, h);
	add_to_octets<int>(o, m_vecRegionRepu.size());

	for (size_t i = 0; i < m_vecRegionRepu.size(); i++)
		add_to_octets<int>(o, m_vecRegionRepu[i]);

	return gds;
}

GNET::GamedataSend CECHostPlayer::gen_task_data()
{
	using namespace S2C;

	GNET::GamedataSend gds;
	GNET::Octets& o = gds.data;
	o.reserve(8000);
	cmd_header h;
	h.cmd = TASK_DATA;
	add_to_octets<cmd_header>(o, h);
	TaskInterface* p = GetTaskInterface();
	add_to_octets<size_t>(o, p->GetActLstDataSize());
	o.insert(o.end(), p->GetActiveTaskList(), p->GetActLstDataSize());
	add_to_octets<size_t>(o, p->GetFnshLstDataSize());
	o.insert(o.end(), p->GetFinishedTaskList(), p->GetFnshLstDataSize());
	add_to_octets<size_t>(o, p->GetFnshTimeLstDataSize());
	o.insert(o.end(), p->GetFinishedTimeList(), p->GetFnshTimeLstDataSize());
	return gds;
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
*/
/*
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
	//	Get cached role information
	const GNET::RoleInfo& RoleInfo = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	ASSERT(RoleInfo.roleid == m_PlayerInfo.cid);

//	m_iRace			= RACE_ORC;	
//	m_iRace			= g_pGame->GetGameInit().iRaceTest;
	m_iRace			= CECPlayer::GetRaceByProf(RoleInfo.occupation);
	m_iPhysique		= CECPlayer::GetPhysiqueByProf(RoleInfo.occupation);
	m_iFaceid		= RoleInfo.faceid;
	m_iHairid		= RoleInfo.hairid;
	m_iGender		= RoleInfo.gender;
	m_iEarid		= RoleInfo.earid;		// ROLEROLEROLE
	m_iTailid		= RoleInfo.tailid;		// ROLEROLEROLE
	//	Get player name and save into name cache
	m_strName = ACString((const ACHAR*)RoleInfo.name.begin(), RoleInfo.name.size() / sizeof (ACHAR));
	g_pGame->GetGameRun()->AddPlayerName(m_PlayerInfo.cid, m_strName);
	if (m_pPateName)
		m_pPateName->SetText(m_strName, false);

	
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
*/
//	Release object
void CECHostPlayer::Release()
{
/*	if( g_pGame->GetGameRun()&&  g_pGame->GetGameRun()->GetHelper() )
	{
		//save helper settings
		g_pGame->GetGameRun()->GetHelper()->Save(m_PlayerInfo.cid);

		//save helper monster settings
		CMonsterSelector * pMonsterSelector = g_pGame->GetGameRun()->GetHelper()->GetMonsterSelector();
		if (pMonsterSelector)
		{
			pMonsterSelector->Save(m_PlayerInfo.cid);
			pMonsterSelector->SaveMonsterList(m_PlayerInfo.cid);
		}
		//save helper matter selector include and exclude list
		CMatterSelector * pMatterSelector = g_pGame->GetGameRun()->GetHelper()->GetMatterSelector();
		if (pMatterSelector)
		{
			pMatterSelector->Save(m_PlayerInfo.cid, CMatterSelector::MATTER_INCLUDE);
			pMatterSelector->Save(m_PlayerInfo.cid, CMatterSelector::MATTER_EXCLUDE);
		}
	}
	
	//	Release dir indicators
	ReleaseDirIndicators();

	//	Release duel images
	ReleaseDuelImages();

	//	Release sounds
	ReleaseSounds();

	//	Release friend manger
	if (m_pFriendMan)
	{
		delete m_pFriendMan;
		m_pFriendMan = NULL;
	}
*/
	//	Release pet corral
	if (m_pPetCorral)
	{
		delete m_pPetCorral;
		m_pPetCorral = NULL;
	}
/*
	int i;

	//	Release all shortcuts
	for (i=0; i < NUM_HOSTSCSETS1; i++)
		A3DRELEASE(m_aSCSets1[i]);

	for (i=0; i < NUM_HOSTSCSETS2; i++)
		A3DRELEASE(m_aSCSets2[i]);
*/
	//	Release all inventories
	A3DRELEASE(m_pPack);
	A3DRELEASE(m_pEquipPack);
	A3DRELEASE(m_pTrashBoxPack);
	A3DRELEASE(m_pFactionTrashBoxPack);
	A3DRELEASE(m_pTaskPack);
	A3DRELEASE(m_pPetPack);
	A3DRELEASE(m_pPetEquipPack);
	A3DRELEASE(m_pDealPack);
	A3DRELEASE(m_pEPDealPack);
//	A3DRELEASE(m_pTaskInterface);
//	A3DRELEASE(m_pSpritePortrait);
	A3DRELEASE(m_pBuyPack);
	A3DRELEASE(m_pSellPack);
	A3DRELEASE(m_pBoothSPack);
	A3DRELEASE(m_pBoothBPack);
	A3DRELEASE(m_pEPBoothSPack);
	A3DRELEASE(m_pEPBoothBPack);
	A3DRELEASE(m_pEPEquipPack);
	A3DRELEASE(m_pEPPetPack);
	A3DRELEASE(m_pEPPetEquipPack);
	A3DRELEASE(m_pPocketPack);
	A3DRELEASE(m_pFashionPack);
	A3DRELEASE(m_pHTFarmPack);

	int i;
	for (i=0; i < NUM_NPCIVTR; i++)
	{
		A3DRELEASE(m_aNPCPacks[i]);
	}

	//	Release all skills
/*	ReleaseSkills();

	//	Clear current combo skill
	ClearComboSkill();

	if (m_pWorkMan)
	{
		delete m_pWorkMan;
		m_pWorkMan = NULL;
	}

	if(m_pCameraPath)
	{
		delete m_pCameraPath;
		m_pCameraPath= NULL;
	}

	m_CameraCtrl.Release();

	if(m_pCutscene)
	{
		delete m_pCutscene;
		m_pCutscene= NULL;
	}

	m_aTeamInvs.RemoveAll();

	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pMoveTargetGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pSelectedGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pHoverGFX);
	g_pGame->GetGFXCaster()->ReleaseGFXEx(m_pFloatDust);

	m_pMoveTargetGFX = NULL;
	m_pSelectedGFX = NULL;
	m_pHoverGFX = NULL;
	m_pFloatDust = NULL;

	CECPlayer::Release();
*/
}
/*
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
	 _cp_str(m_strNickname, p, len);
}
*/
//	Create inventories
bool CECHostPlayer::CreateInventories()
{
	if (!(m_pPack = new CECInventory) ||
		!(m_pEquipPack = new CECInventory) ||
		!(m_pTrashBoxPack = new CECInventory) ||
		!(m_pFactionTrashBoxPack = new CECInventory) ||
		!(m_pTaskPack = new CECInventory) ||
		!(m_pPetPack = new CECInventory) ||
		!(m_pPetEquipPack = new CECInventory) ||
		!(m_pDealPack = new CECDealInventory) ||
		!(m_pEPDealPack = new CECDealInventory) ||
		!(m_pBuyPack = new CECDealInventory) ||
		!(m_pSellPack = new CECDealInventory) ||
		!(m_pBoothSPack = new CECDealInventory) ||
		!(m_pBoothBPack = new CECDealInventory) ||
		!(m_pEPBoothSPack = new CECDealInventory) ||
		!(m_pEPBoothBPack = new CECDealInventory) ||
		!(m_pEPEquipPack = new CECInventory) ||
		!(m_pEPPetPack = new CECInventory) ||
		!(m_pEPPetEquipPack = new CECInventory) ||
		!(m_pPocketPack = new CECInventory)  ||
		!(m_pFashionPack = new CECInventory) ||
		!(m_pHTFarmPack = new CECInventory))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECHostPlayer::CreateInventories", __LINE__);
		return false;
	}

	for (int i=0; i < NUM_NPCIVTR; i++)
	{
		if (!(m_aNPCPacks[i] = new CECNPCInventory) || 
			!m_aNPCPacks[i]->Init(IVTRSIZE_NPCPACK))
			return false;
	}

	if (!m_pPack->Init(0) || !m_pEquipPack->Init(IVTRSIZE_EQUIPPACK) ||
		!m_pTaskPack->Init(IVTRSIZE_TASKPACK) || !m_pPetPack->Init(0) || !m_pPetEquipPack->Init(IVTRSIZE_PETEQUIP) || !m_pDealPack->Init(IVTRSIZE_DEALPACK) ||
		!m_pTrashBoxPack->Init(0) || !m_pFactionTrashBoxPack->Init(0) || !m_pEPDealPack->Init(IVTRSIZE_DEALPACK) ||
		!m_pBuyPack->Init(IVTRSIZE_BUYPACK) || !m_pSellPack->Init(IVTRSIZE_SELLPACK) ||
		!m_pBoothSPack->Init(IVTRSIZE_BOOTHSPACK) || !m_pBoothBPack->Init(IVTRSIZE_BOOTHBPACK) ||
		!m_pEPBoothSPack->Init(IVTRSIZE_BOOTHSPACK) || !m_pEPBoothBPack->Init(IVTRSIZE_BOOTHBPACK) ||
		!m_pEPEquipPack->Init(IVTRSIZE_EQUIPPACK) || !m_pEPPetPack->Init(IVTRSIZE_PETPACK) ||
		!m_pEPPetEquipPack->Init(IVTRSIZE_PETEQUIP) || !m_pPocketPack->Init(0) || 
		!m_pFashionPack->Init(IVTRSIZE_FASHIONPACK) || !m_pHTFarmPack->Init(IVTRSIZE_HTFARMPACK))
	{
		a_LogOutput(1, "CECHostPlayer::CreateInventories, Failed to initialize inventories");
		return false;
	}

	return true;
}

/*
//	Create shortcut sets
bool CECHostPlayer::CreateShortcutSets()
{
	int i;

	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		m_aSCSets1[i] = new CECShortcutSet;
		m_aSCSets1[i]->Init(SIZE_HOSTSCSET1);
	}

	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		m_aSCSets2[i] = new CECShortcutSet;
		m_aSCSets2[i]->Init(SIZE_HOSTSCSET2);
	}

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

	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	if (!pCurWork || dwUpdateCnt % 3)
		return;

	if (pCurWork->IsMoving())
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
*/
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
/*
//	Get detail data of specified item
void CECHostPlayer::GetIvtrItemDetailData(int iPack, int iSlot)
{
	CECInventory* pPack = GetPack(iPack);
	if (!pPack) return;
	CECIvtrItem* pItem = pPack->GetItem(iSlot);
	if (pItem)
		pItem->GetDetailDataFromSev(iPack, iSlot);
}

//	Use specified inventory item in pack
bool CECHostPlayer::UseItemInPack(int iPack, int iSlot)
{
	//	Reject this action if we are in some special states
	if (!CanDo(CANDO_USEITEM))
		return false;

	CECInventory* pPack = GetPack(iPack);
	if (!pPack) return false;
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
		int type = pMedicine->GetDBEssence()->type;
		if (GetCoolTime(GP_CT_HP + type))
		{	
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_HP_COOLTIME + type);
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
	
	return true;
}
*/
//	Apply shortcut of specified skill
//bool CECHostPlayer::ApplySkillShortcut(int idSkill, bool bCombo/* false */, 
//								int idSelTarget/* 0 */, int iForceAtk/* -1 */)
/*{
	StackChecker::ACTrace(4);

	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

		// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	//	Return-town skill is very special, handle it separately
//	if (idSkill == ID_RETURNTOWN_SKILL)
//		return ReturnToTargetTown(0);

	if (!CanDo(CANDO_SPELLMAGIC))
		return false;

	if (!bCombo)
		ClearComboSkill();

	if (!idSelTarget)
		idSelTarget = m_idSelTarget;

	CECSkill* pSkill = NULL;
	// 如果使用了技能物品，优先消耗技能物品
	if (idSkill == m_ApoiseSkill.idSkill)
	{
		pSkill = GetItemSkillByID(idSkill);
		if (!pSkill)
		{
			pSkill = new CECSkill(idSkill, m_ApoiseSkill.iSkillLvl);
			m_aImSkills.Add(pSkill);
			// 重置物品技能的冷却
			abase::hash_map<int, int>::iterator it = m_TsSkillCoolDown.find(idSkill);
			if (it != m_TsSkillCoolDown.end())
				pSkill->StartCooling(0, it->second);
		}
		if (!pSkill)
		{
			ClearApoiseSkill();
			ASSERT(0);
			return false;
		}
	}
	else
	{	
		pSkill= GetPositiveSkillByID(idSkill);
		if (!pSkill)
		{
			pSkill = GetTransfigureSkillByID(idSkill);
			// 如果取到了变身被动技能，返回
			if (!pSkill || pSkill->GetType() == CECSkill::TYPE_PASSIVE || pSkill->GetType() == CECSkill::TYPE_LIVE )
			{
				ASSERT(0);
				return false;
			}
		}
	}

	//	If we press a chargeable skill again when it's being charged, 
	//	we cast it out at once
	//  用keyup来表示技能的蓄力终止，与以前两次按键的实现方式不一样
// 	if (IsSpellingMagic() && m_pCurSkill && m_pCurSkill->IsCharging() &&
// 		m_pCurSkill->GetSkillID() == pSkill->GetSkillID())
// 	{
// 		m_pCurSkill->EndCharging();
// 		g_pGame->GetGameSession()->c2s_CmdContinueAction();
// 		return true;
// 	}
	// 避免持续按键导致的问题
	if (IsSpellingMagic() && m_pCurSkill && m_pCurSkill->IsCharging() &&
		m_pCurSkill->GetSkillID() == pSkill->GetSkillID())
	{
		return true;
	}
	// 避免吟唱时切换其他按键导致蓄力成功的问题
	if (IsSpellingMagic() && m_pCurSkill && m_pCurSkill->IsChargeable() && 
		m_pCurSkill->GetSkillID() != pSkill->GetSkillID())
		return false;

	if (m_pPrepSkill && m_pPrepSkill->IsChargeable() && m_pPrepSkill->GetSkillID() == pSkill->GetSkillID())
	{
		return true;
	}

	// 检查使用该技能需要消耗的物品是否存在
	int iItemCon = CheckSkillCostItemCondition(pSkill);
	if(iItemCon)
	{
		g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_SKILL_COST_ITEM, GP_CHAT_SYSTEM);
		return false;
	}
	int iCon = CheckSkillCastCondition(pSkill);
	if (iCon)
	{
		int iMsg = -1;
		switch (iCon)
		{
		case 1:	iMsg = FIXMSG_INVALIDWEAPON;		break;
		case 2:	iMsg = FIXMSG_NEEDMP;				break;
		case 4: iMsg = FIXMSG_WEAPON_MISMATCH;		break;
		case 5: iMsg = FIXMSG_SKILL_BATTLE_LIMIT;	break;
		case 6:	iMsg = FIXMSG_SKILL_FIGHT_LIMIT;	break;
		case 7:	iMsg = FIXMSG_SKILL_FIGHT_LIMIT2;	break;
		case 8:	iMsg = FIXMSG_SKILL_CLONE_LIMIT;	break;
		case 9:	iMsg = FIXMSG_SKILL_MOUNT_LIMIT;	break;
		case 10:iMsg = FIXMSG_SKILL_FLY_LIMIT;		break;
		}

		if (iMsg >= 0)
			g_pGame->GetGameRun()->AddFixedChannelMsg(iMsg, GP_CHAT_SYSTEM);

		return false;
	}

	//	Get force attack flag
	bool bForceAttack;
	if (iForceAtk < 0)
		bForceAttack = GetForceAttackFlag(NULL);
	else
		bForceAttack = iForceAtk > 0 ? true : false;

	//	Check negative effect skill
	if (pSkill->GetType() == CECSkill::TYPE_ATTACK || pSkill->GetType() == CECSkill::TYPE_CURSE)
	{
		if (idSelTarget == m_PlayerInfo.cid)
		{
			//	Host cannot spell negative effect magic to himself.
			g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_TARGETWRONG, GP_CHAT_SYSTEM);
			return false;
		}
		else if (idSelTarget)
		{
			if (AttackableJudge(idSelTarget, bForceAttack) != 1)
				return false;
		}
	}

	//	Check whether target type match
	int idCastTarget = idSelTarget;
	int iTargetType = pSkill->GetTargetType();

	if (pSkill->GetType() == CECSkill::TYPE_BLESS)
	{
		if (!iTargetType || !ISPLAYERID(idSelTarget))
			idCastTarget = m_PlayerInfo.cid;

		//	In some case, we shouldn't add bless effect to other players
		if (ISPLAYERID(idCastTarget) && idCastTarget != m_PlayerInfo.cid)
		{
			//	If host is in duel, bless skill couldn't add to opponent
			if (IsInDuel() && idSelTarget == m_pvp.idDuelOpp)
				idCastTarget = m_PlayerInfo.cid;

			if (GetCombatFaction())
			{
				CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idCastTarget);

				if (pPlayer && pPlayer->GetFactionID() == GetCombatFaction())
					idCastTarget = m_PlayerInfo.cid;
			}
			
			//	If host is in battle, bless skill couldn't add to enemies
			if (IsInBattle())
			{
				CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(idCastTarget);
				if (!InSameBattleCamp(pPlayer))
					idCastTarget = m_PlayerInfo.cid;
			}
		}
	}
	else
	{
		if (iTargetType != 0 && !idCastTarget)
			return false;
	}

	if (iTargetType)
	{
		//	Target shoundn't be a corpse ?
		int iAliveFlag = 0;
		if (iTargetType == 1)
			iAliveFlag = 1;
		else if (iTargetType == 2)
			iAliveFlag = 2;

		CECObject* pObject = g_pGame->GetGameRun()->GetWorld()->GetObject(idCastTarget, iAliveFlag);
		if (!pObject)
			return false;
	}

	if (!IsMeleeing() && !IsSpellingMagic() &&
		(!iTargetType || idCastTarget == m_PlayerInfo.cid))
	{
		//	Cast this skill need't checking cast distance
// 		if (!pSkill->ReadyToCast())
// 			return false;

		//	Prepare to cast skill, if skill isn't INSTANT, 
		//	we must stop moving and stand
		if (!pSkill->IsInstant())
		{
			if (!NaturallyStopMoving())
				return false;	//	Couldn't stop naturally, so cancel casting skill
		}

		m_pPrepSkill = pSkill;
		CastSkill(m_PlayerInfo.cid, bForceAttack);
	}
	else if (IsSpellingMagic() && m_pCurSkill == pSkill && !pSkill->ReadyToCast())
	{
		//	If we are casting the same skill and it's in cooling time
		return false;
	}
	else
	{
		//	Trace selected object
		CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
		if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
		{
			CECHPWorkTrace* pWork = (CECHPWorkTrace*)pP1Work;
			if (pWork->GetTraceReason() == CECHPWorkTrace::TRACE_SPELL &&
				pWork->GetTarget() == idCastTarget &&
				pWork->GetPrepSkill() == pSkill)
				return false;	//	We are just doing the same thing

			pWork->ChangeTarget(idCastTarget, CECHPWorkTrace::TRACE_SPELL);
			pWork->SetForceAttack(bForceAttack);
			pWork->SetPrepSkill(pSkill);

			if (gr.get_state() == game_record::enum_gr_record)
			{
				ECMSG msg;
				msg.dwMsg = MSG_HST_TRACE_RECORD;
				msg.iManager = MAN_PLAYER;
				msg.iSubID = 0;
				msg.dwParam1 = idCastTarget;
				msg.dwParam2 = CECHPWorkTrace::TRACE_SPELL;
				msg.dwParam3 = bForceAttack;
				msg.dwParam4 = idSkill;
				gr.inc_frame()->push_event(new event_record_msg(msg));
			}
		}
		else if (m_pPrepSkill != pSkill)
		{
			if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
			{
				CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
				pWork->ChangeTarget(idCastTarget, CECHPWorkTrace::TRACE_SPELL);
				pWork->SetForceAttack(bForceAttack);
				pWork->SetPrepSkill(pSkill);
				m_pWorkMan->StartWork_p1(pWork);

				if (gr.get_state() == game_record::enum_gr_record)
				{
					ECMSG msg;
					msg.dwMsg = MSG_HST_TRACE_RECORD;
					msg.iManager = MAN_PLAYER;
					msg.iSubID = 0;
					msg.dwParam1 = idCastTarget;
					msg.dwParam2 = CECHPWorkTrace::TRACE_SPELL;
					msg.dwParam3 = bForceAttack;
					msg.dwParam4 = idSkill;
					gr.inc_frame()->push_event(new event_record_msg(msg));
				}
			}
		}
	}

	return true;
}*/
//bool CECHostPlayer::ApplySkillShortcutUp(int idSkill, bool bCombo/* false */, 
//								int idSelTarget/* 0 */, int iForceAtk/* -1 */)
/*{
	StackChecker::ACTrace(4);

	if (gr.get_state() == game_record::enum_gr_replay)
		return false;

		// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	//	Return-town skill is very special, handle it separately
//	if (idSkill == ID_RETURNTOWN_SKILL)
//		return ReturnToTargetTown(0);

	if (!CanDo(CANDO_SPELLMAGIC))
		return false;

	if (!bCombo)
		ClearComboSkill();

	if (!idSelTarget)
		idSelTarget = m_idSelTarget;

	CECSkill* pSkill = GetPositiveSkillByID(idSkill);
	if (!pSkill)
	{
		ASSERT(0);
		return false;
	}
*/
	//	If we press a chargeable skill again when it's being charged, 
	//	we cast it out at once
//	if (/*IsSpellingMagic() && */m_pCurSkill && m_pCurSkill->IsCharging() &&
//		m_pCurSkill->GetSkillID() == pSkill->GetSkillID())
/*	{	
		m_pCurSkill->EndCharging();
		PlayAttackEffect(idSelTarget, idSkill, m_pCurSkill->GetSkillLevel(), m_pCurSkill->GetStamp(), -1, 0);
		g_pGame->GetGameSession()->c2s_CmdContinueAction();		
		return true;
	}
	return true;
}
//	Return to a target town through skill
bool CECHostPlayer::ReturnToTargetTown(int idTarget)
{
	if (!CanDo(CANDO_SPELLMAGIC))
		return false;

	int idSkill = ID_RETURNTOWN_SKILL;
	CECSkill* pSkill = GetPositiveSkillByID(idSkill);
	if (!pSkill)
		return false;

	ClearComboSkill();

	// 检查使用该技能需要消耗的物品是否存在
	int iItemCon = CheckSkillCostItemCondition(pSkill);
	if(iItemCon)
	{
		g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_SKILL_COST_ITEM, GP_CHAT_SYSTEM);
		return false;
	}

	int iCon = CheckSkillCastCondition(pSkill);
	if (iCon)
	{
		int iMsg = -1;
		switch (iCon)
		{
		case 1:	iMsg = FIXMSG_INVALIDWEAPON;		break;
		case 2:	iMsg = FIXMSG_NEEDMP;				break;
		case 4: iMsg = FIXMSG_WEAPON_MISMATCH;		break;
		case 5: iMsg = FIXMSG_SKILL_BATTLE_LIMIT;	break;
		case 6:	iMsg = FIXMSG_SKILL_FIGHT_LIMIT;	break;
		case 7:	iMsg = FIXMSG_SKILL_FIGHT_LIMIT2;	break;
		case 8:	iMsg = FIXMSG_SKILL_CLONE_LIMIT;	break;
		case 9:	iMsg = FIXMSG_SKILL_MOUNT_LIMIT;	break;
		case 10:iMsg = FIXMSG_SKILL_FLY_LIMIT;		break;
		}

		if (iMsg >= 0)
			g_pGame->GetGameRun()->AddFixedChannelMsg(iMsg, GP_CHAT_SYSTEM);

		return false;
	}

	//	If this skill is in cooling time or we are casting other skill, return 
	if (IsSpellingMagic() || !pSkill->ReadyToCast())
		return false;

	m_pPrepSkill = pSkill;
	BYTE byPVPMask = BuildPVPMask(false);
	g_pGame->GetGameSession()->c2s_CmdCastSkill(idSkill, byPVPMask, 1, &idTarget);

	return true;
}

//	Stop host moving naturally, return true host stopped, otherwise return false
bool CECHostPlayer::NaturallyStopMoving()
{
//	if (!m_MoveCtrl.IsStop())
	if (!IsPlayerMoving())
		return true;	//	Host has been stopped

	if (m_iMoveMode == MOVE_FREEFALL || InSlidingState() || IsJumping())
		return false;	//	Host couldn't stop naturally

	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND)
		m_pWorkMan->FinishCurWork(-1, true);

	m_MoveCtrl.SendStopMoveCmd();

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
	if (!m_bChangingFace && (m_pCutscene && !m_pCutscene->IsPlayingCutscene()))
	{
		CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
		if (pCurWork)
		{
			pCurWork->Tick(dwDeltaTime);
			if (pCurWork->IsFinished())
				m_pWorkMan->FinishCurWork(-1, false);
		}
	}

	//	Update task
	if (m_pTaskInterface && m_TaskCounter.IncCounter(dwDeltaTime))
	{
		m_TaskCounter.Reset();
		OnTaskCheckStatus(m_pTaskInterface);
	}

	//	Update timers
	UpdateTimers(dwDeltaTime);

	//	Update shortcuts
	UpdateShortcuts(dwDeltaTime);

	if (gr.get_state() == game_record::enum_gr_replay)
		SetPos(gr.get_host_pos());

	
	m_pCutscene->Tick(dwDeltaTime);

	m_pCameraPath->Tick(dwDeltaTime);
	//	Adjust camera's position
	if( m_pCutscene->IsPlayingCutscene())
	{
		m_CameraCoord.SetPos(m_pCutscene->GetCameraPath()->GetPos());
		m_CameraCoord.SetDirAndUp(m_pCutscene->GetCameraPath()->GetDir(), g_vAxisY);
	}
	else
	{
		if (!IsChangingFace())
			UpdateFollowCamera(m_iMoveMode != MOVE_STAND ? true : false, dwDeltaTime);
	}
	

	if (m_iBoothState != 2)
	{
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

		//	Update move sound
		UpdateMoveSound();
	}

	//	Update GFXs
	UpdateGFXs(dwDeltaTime);

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

	A3DBone * pBone = m_pBackupModel->GetA3DSkinModel()->GetSkeleton()->GetBone("Bip01 Pelvis", NULL); 
	A3DVECTOR3 vecPos = pBone->GetAbsoluteTM().GetRow(3) + A3DVECTOR3(0.0f, 0.1f, 0.0f);
	if( m_pModelTray )
	{
		m_pModelTray->SetPos(vecPos);
		m_pModelTray->SetDirAndUp(g_vAxisZ, g_vAxisY);
		m_pModelTray->Tick(dwDeltaTime);
	}

	vecPos = GetPos() + A3DVECTOR3(0.0f, 1.0f, 0.0f);
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

	if (gr.get_state() == game_record::enum_gr_record)
	{
		if (m_dwOldDir != 0 && m_dwMoveRelDir == 0) // stop push
		{
			ECMSG msg;
			msg.dwMsg		= MSG_HST_PUSH_RECORD;
			msg.iManager	= MAN_PLAYER;
			msg.iSubID		= 0;
			msg.dwParam1	= 1;
			msg.dwParam2	= 0;
			msg.dwParam3	= 0;
			msg.dwParam4	= 0;
			frame_record* fr = gr.get_cur_frame();

			if (fr)
				fr->push_event(new event_record_msg(msg));
		}
	}

	if (gr.get_state() != game_record::enum_gr_replay)
	{
		m_dwOldDir		= m_dwMoveRelDir;
		m_dwMoveRelDir	= 0;
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

void CECHostPlayer::SyncDemonstrationPlayer(CECLoginPlayer* pPlayer)
{
	if (m_bLoadFlag)
	{
		pPlayer->UpdateFace(m_iFaceid, m_iHairid);
		pPlayer->SyncBasicProps(m_BasicProps);
		pPlayer->UpdateEquipments(m_aEquips, m_bFashionMode);
	}
}
*/
//	Render routine
//bool CECHostPlayer::Render(CECViewport* pViewport, int iRenderFlag/* 0 */)
/*{
	m_PateContent.iVisible = 0;

	if (!IsAllResReady())
		return true;

	if( !IsClothesOn() )
		return true;

	A3DTerrainWater * pTerrainWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();
	if( iRenderFlag == RD_REFLECT && pTerrainWater->IsUnderWater(GetPlayerAABB().Maxs) )
		return true;
	else if( iRenderFlag == RD_REFRACT && !pTerrainWater->IsUnderWater(GetPlayerAABB().Mins) )
		return true;

	if (m_AttachMode != enumAttachNone && m_bHangerOn)
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

			if (m_bCastShadow && g_pGame->GetShadowRender())
			{
				CECPlayer * pPlayer = this;

				SHADOW_RECEIVER receiver = SHADOW_RECEIVER_TERRAIN;
				if( iRenderFlag == RD_NORMAL )
					receiver = (SHADOW_RECEIVER) (receiver | SHADOW_RECEIVER_LITMODEL);

				g_pGame->GetShadowRender()->AddShadower(m_aabb.Center, m_aabb, receiver, PlayerRenderForShadow, pPlayer, false);
			}
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

		//	Print distance between camera to position under cursor
		if (g_pGame->GetConfigs()->GetTestDistFlag())
		{
			int x, y;
			g_pGame->GetGameRun()->GetInputCtrl()->GetMousePos(&x, &y);
			ACHAR szMsg[100];
			if (l_fTestDist >= 0.0f)
				a_sprintf(szMsg, _AL("Dist: %f"), l_fTestDist);
			else
				a_sprintf(szMsg, _AL("Dist: too far"));

			g_pGame->GetA3DEngine()->GetSystemFont()->TextOut(x-40, y-20, szMsg, 0xffffffff);
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
			m_pSprite->Render(pViewport);
	}

	return true;
}
*/
//	Render when player is opening booth
//bool CECHostPlayer::RenderForBooth(CECViewport* pViewport, int iRenderFlag/* 0 */)
/*{
	m_PateContent.iVisible = 0;

	if (iRenderFlag == RD_REFLECT || iRenderFlag == RD_REFRACT)
		return true;

	A3DSkinModel* pBoothModel = m_pPlayerMan->GetBoothModel(m_iGender);
	if (pBoothModel)
	{
		pBoothModel->SetPos(GetPos());
		pBoothModel->SetDirAndUp(GetDir(), GetUp());
		pBoothModel->Update(g_pGame->GetTickTime());
		pBoothModel->RenderAtOnce(pViewport->GetA3DViewport(), 0);
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
	A3DTerrainWater * pTerrainWater = pWorld->GetTerrainWater();
	if( pTerrainWater )
	{
		if( GetPos().y < pTerrainWater->GetWaterHeight(GetPos()) - 2.5f )
			InData.bWater = true;
	}

	m_CameraCtrl.Tick(&InData, &OutData);

	if (!m_CameraCtrl.SceneryMode_Get() && !m_CameraCtrl.MovieMode_Get() && !m_CameraCtrl.FixCamPitch_Get())
	{
		if (_shaking_cam) OutData.vecPosCam += _cam_shake_offset;
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
	m_pCameraPath->StartCameraPath(dwPathIndex);
}

//	Start cutscene
void CECHostPlayer::StartCutscene(DWORD dwCutscneIndex, bool bNeedCheck)
{
	if(!m_pCutscene->IsPlayingCutscene())
		m_pCutscene->StartCutscene(dwCutscneIndex, bNeedCheck);
}
*/
//	Adjust host's transparence
//void CECHostPlayer::AdjustTransparence(float fDistToCam, const A3DVECTOR3& vDir, DWORD dwTime)
//{
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

/////
/*
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

	if ( m_pCameraPath && m_pCameraPath->IsUsingCameraPath())
	{
		vRightHostCamera= GetRight();
		vDirHostCamera	= GetDir();
	}else
	{
		vRightHostCamera= m_CameraCoord.GetRight();
		vDirHostCamera	= m_CameraCoord.GetDir();
	}


	if (!m_dwMoveRelDir)
		return false;

	if (m_dwMoveRelDir & MD_LEFT)
	{
*/		/*
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
/*		{
			vRight = -vRightHostCamera;
			vRight.y = 0;
			vRight.Normalize();
			vPushDir = vRight;
			bMove = true;
		}
	}
	else if (m_dwMoveRelDir & MD_RIGHT)
	{
*/		/*
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
/*		{
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
*/
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
//	return true;
//}

/*
//	Message MSG_HST_LBTNCLICK handler
void CECHostPlayer::OnMsgLBtnClick(const ECMSG& Msg)
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return;

	CECCDS* pCDS = g_pGame->GetGameRun()->GetWorld()->GetCDS();
	CECInputCtrl* pInputCtrl = g_pGame->GetGameRun()->GetInputCtrl();

	A3DVECTOR3 vDest((float)Msg.dwParam1, (float)Msg.dwParam2, 1.0f);
	A3DViewport* pView = g_pGame->GetViewport()->GetA3DViewport();
	pView->InvTransform(vDest, vDest);

	A3DVECTOR3 vStart = pView->GetCamera()->GetPos();
	A3DVECTOR3 vDelta = vDest - vStart;

	RAYTRACERT TraceRt;
	int iTraceFlag = TRACEOBJ_LBTNCLICK;
	if (pInputCtrl->IsShiftPressed(Msg.dwParam3))
		iTraceFlag |= 0x80000000;

	if (pInputCtrl->IsAltPressed(Msg.dwParam3))
		iTraceFlag |= 0x40000000;

	if (pCDS->RayTrace(vStart, vDelta, 1.0f, &TraceRt, iTraceFlag, (DWORD)this))
	{
		CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
		
		if (pCDS->m_RayTraceRt.iEntity == ECENT_TERRAIN ||
			pCDS->m_RayTraceRt.iEntity == ECENT_BUILDING ||
			pCDS->m_RayTraceRt.iEntity == ECENT_FOREST )
		{

			if(pInputCtrl->IsShiftPressed(Msg.dwParam3) && pCDS->m_RayTraceRt.iEntity == ECENT_TERRAIN)
			{
				A3DVECTOR3 vMoveDest = vStart + vDelta * pCDS->m_RayTraceRt.fFraction;
			//	CECWorld* pWorld = g_pGame->GetGameRun()->GetWorld
				CDlgChat* pChat = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->m_pDlgChat;
				PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
				if( pEdit->GetItemCount(enumEICoord) < 3)
				{
					ACHAR szText[100];
					ACHAR szName[100];
					A3DCOLOR color = A3DCOLORRGBA(108, 251, 75,  255);

					CELPrecinct* pPrecient = g_pGame->GetGameRun()->GetWorld()->GetCurPrecinct();
					a_sprintf(szName, _AL("【%s (%d, %d)】"), pPrecient? pPrecient->GetName():_AL(""), (int)(vMoveDest.x), (int)(vMoveDest.z));
//					a_sprintf(szName, _AL("%s"), pPrecient? pPrecient->GetName():_AL(""));
					
					// 对szName进行校验
					BYTE  checknum = 0;
					BYTE  ch, cl;  // 校验位的高位和低位
					for (int i =0;szName[i] != '\0';  ++i)
					{
						checknum =  checknum^szName[i];
					}
					ch = (checknum >> 4) & 0x0F;
					cl = checknum & 0x0F;
					ch += 'w';
					cl += 'm';
					if (!szName[0])
					{
						ch = 'z';
						ch = 'x';
					}
					int checkcode = (cl << 16) | ch;
					// 得到最终的校验码

					a_sprintf(szText, _AL("%d %d %d %d %d"), 
						(int)(vMoveDest.x),
						(int)(vMoveDest.y),
						(int)(vMoveDest.z),
						g_pGame->GetGameRun()->GetWorld()->GetInstanceID(),
						checkcode
						);

					pEdit->AppendItem(enumEICoord, color, szName, szText);
				}
				g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->BringWindowToTop(pChat);
				pChat->ChangeFocus(pEdit);
				return;
			}

			if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
			{
				g_pGame->GetGameSession()->c2s_CmdStandUp();
				return;
			}

			if (!CanDo(CANDO_MOVETO))
				return;

			//	Hit terrain
			A3DVECTOR3 vMoveDest = vStart + vDelta * pCDS->m_RayTraceRt.fFraction;

			if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
			{
				CECHPWorkMove* pWork = (CECHPWorkMove*)pP1Work;
				pWork->SetDestination(CECHPWorkMove::DEST_2D, vMoveDest);
				pWork->PlayMoveTargetGFX(TraceRt.vHitPos, TraceRt.vNormal);

				if (gr.get_state() == game_record::enum_gr_record)
				{
					A3DVECTOR3& v = vMoveDest;
					ECMSG msg;
					msg.dwMsg = MSG_HST_MOVE_RECORD;
					msg.iManager = MAN_PLAYER;
					msg.iSubID = 0;
					msg.dwParam1 = CECHPWorkMove::DEST_2D;
					msg.dwParam2 = *(DWORD*)&v.x;
					msg.dwParam3 = *(DWORD*)&v.y;
					msg.dwParam4 = *(DWORD*)&v.z;
					frame_record* fr = gr.get_cur_frame();

					if (fr)
						fr->push_event(new event_record_msg(msg));
				}
			}
			else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_MOVETOPOS))
			{
				//	If destination is too near, ignore it.
				A3DVECTOR3 vDist = vMoveDest - GetPos();
				if (vDist.MagnitudeH() > 0.5f)
				{
					CECHPWorkMove* pWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVETOPOS);
					pWork->SetDestination(CECHPWorkMove::DEST_2D, vMoveDest);
					pWork->PlayMoveTargetGFX(TraceRt.vHitPos, TraceRt.vNormal);
					m_pWorkMan->StartWork_p1(pWork);

					if (gr.get_state() == game_record::enum_gr_record)
					{
						A3DVECTOR3& v = vMoveDest;
						ECMSG msg;
						msg.dwMsg = MSG_HST_MOVE_RECORD;
						msg.iManager = MAN_PLAYER;
						msg.iSubID = 0;
						msg.dwParam1 = CECHPWorkMove::DEST_2D;
						msg.dwParam2 = *(DWORD*)&v.x;
						msg.dwParam3 = *(DWORD*)&v.y;
						msg.dwParam4 = *(DWORD*)&v.z;
						frame_record* fr = gr.get_cur_frame();

						if (fr)
							fr->push_event(new event_record_msg(msg));
					}
				}
			}
		}
		else	//	Hit a object
		{
			int idTraceTarget = 0, idSelTarget = 0;
			int idHitObject = pCDS->m_RayTraceRt.iObjectID;
			bool bForceAttack = false;
			int iTraceReason = CECHPWorkTrace::TRACE_NONE;

			if (pCDS->m_RayTraceRt.iEntity == ECENT_MATTER)
			{
				CECMatter* pMatter = g_pGame->GetGameRun()->GetWorld()->GetMatterMan()->GetMatter(idHitObject);

				idTraceTarget	= idHitObject;
				m_idPickMatter	= idTraceTarget;
				iTraceReason	= pMatter->IsMine() ? CECHPWorkTrace::TRACE_GATHER : CECHPWorkTrace::TRACE_PICKUP;
			}
			else if (pCDS->m_RayTraceRt.iEntity == ECENT_DYN_OBJ)
			{
				return;
			}
			else if (pCDS->m_RayTraceRt.iEntity == ECENT_NPC)
			{
				CECNPC* pNPC = g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(idHitObject);

				//	Msg.dwParam4 is double click flag
				if (!pNPC->IsDead() && (m_idSelTarget == idHitObject ||
					(Msg.dwParam4 && m_idUCSelTarget == idHitObject)))
					idTraceTarget = idHitObject;
				else
					idSelTarget = idHitObject;

				if (idTraceTarget)
				{
					bForceAttack = GetForceAttackFlag(&Msg.dwParam3);
					if (AttackableJudge(idHitObject, bForceAttack) == 1)
						iTraceReason = CECHPWorkTrace::TRACE_ATTACK;
					else if (pNPC->IsServerNPC())
						iTraceReason = CECHPWorkTrace::TRACE_TALK;
				}
			}
			else if (pCDS->m_RayTraceRt.iEntity == ECENT_CARRIER)
			{
				return;
			}
			else	//	pCDS->m_RayTraceRt.iEntity == ECENT_PLAYER
			{
				CECPlayer* pPlayer = m_pPlayerMan->GetPlayer(idHitObject);

				//	1. Msg.dwParam4 is double click flag.
				//	2. Buddy player counld't be traced
				if (!pPlayer->IsDead() && pPlayer->GetCharacterID() != m_iBuddyId &&
					(m_idSelTarget == idHitObject || (Msg.dwParam4 && m_idUCSelTarget == idHitObject)))
				{
					idTraceTarget	= idHitObject;
					bForceAttack	= GetForceAttackFlag(&Msg.dwParam3);

					if (AttackableJudge(idHitObject, bForceAttack) == 1)
						iTraceReason = CECHPWorkTrace::TRACE_ATTACK;
					else if (pPlayer->GetBoothState())
						iTraceReason = CECHPWorkTrace::TRACE_TALK;
				}
				else
				{
					idSelTarget = idHitObject;
				}
			}

			if (idTraceTarget)
			{
				if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
				{
					g_pGame->GetGameSession()->c2s_CmdStandUp();
					return;
				}

				bool bRecord = true;

				//	Trace a object
				if (iTraceReason == CECHPWorkTrace::TRACE_ATTACK)
				{
					if (!CanDo(CANDO_MELEE))
						return;

					NormalAttackObject(idTraceTarget, bForceAttack);
				}
				else
				{
					if (!CanDo(CANDO_MOVETO))
						return;

					if (iTraceReason == CECHPWorkTrace::TRACE_PICKUP)
						PickupObject(idTraceTarget, false);
					else if (iTraceReason == CECHPWorkTrace::TRACE_GATHER)
						PickupObject(idTraceTarget, true);
					else if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
					{
						CECHPWorkTrace* pWork = (CECHPWorkTrace*)pP1Work;
						pWork->ChangeTarget(idTraceTarget, iTraceReason);
						pWork->SetForceAttack(bForceAttack);
					}
					else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
					{
						CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
						pWork->ChangeTarget(idTraceTarget, iTraceReason);
						pWork->SetForceAttack(bForceAttack);
						m_pWorkMan->StartWork_p1(pWork);
					}
					else
						bRecord = false;
				}

				if (bRecord && gr.get_state() == game_record::enum_gr_record)
				{
					ECMSG msg;
					msg.dwMsg = MSG_HST_TRACE_RECORD;
					msg.iManager = MAN_PLAYER;
					msg.iSubID = 0;
					msg.dwParam1 = idTraceTarget;
					msg.dwParam2 = iTraceReason;
					msg.dwParam3 = bForceAttack;
					msg.dwParam4 = 0;

					frame_record* fr = gr.get_cur_frame();

					if (fr)
						fr->push_event(new event_record_msg(msg));
				}
			}

			//	Tell server we select a target
			if (idSelTarget && m_idSelTarget != idSelTarget)
			{
				m_idUCSelTarget = idSelTarget;
				g_pGame->GetGameSession()->c2s_CmdSelectTarget(m_idUCSelTarget);
			}
		}
	}
	else	//	Nothing is clicked
	{
		if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
		{
			g_pGame->GetGameSession()->c2s_CmdStandUp();
			return;
		}

		if (!CanDo(CANDO_MOVETO))
			return;

		A3DVECTOR3 vMoveDir = vDelta;
		vMoveDir.y = 0.0f;
		if (vMoveDir.IsZero())
			return;

		vMoveDir.Normalize();

		CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();

		//	Move on the clicked direction
		if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
		{
			CECHPWorkMove* pWork = (CECHPWorkMove*)pP1Work;
			pWork->SetDestination(CECHPWorkMove::DEST_DIR, vMoveDir);

			if (gr.get_state() == game_record::enum_gr_record)
			{
				A3DVECTOR3& v = vMoveDir;
				ECMSG msg;
				msg.dwMsg = MSG_HST_MOVE_RECORD;
				msg.iManager = MAN_PLAYER;
				msg.iSubID = 0;
				msg.dwParam1 = CECHPWorkMove::DEST_DIR;
				msg.dwParam2 = *(DWORD*)&v.x;
				msg.dwParam3 = *(DWORD*)&v.y;
				msg.dwParam4 = *(DWORD*)&v.z;
				frame_record* fr = gr.get_cur_frame();

				if (fr)
					fr->push_event(new event_record_msg(msg));
			}
		}
		else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_MOVETOPOS))
		{
			CECHPWorkMove* pWork = (CECHPWorkMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_MOVETOPOS);
			pWork->SetDestination(CECHPWorkMove::DEST_DIR, vMoveDir);
			m_pWorkMan->StartWork_p1(pWork);

			if (gr.get_state() == game_record::enum_gr_record)
			{
				A3DVECTOR3& v = vMoveDir;
				ECMSG msg;
				msg.dwMsg = MSG_HST_MOVE_RECORD;
				msg.iManager = MAN_PLAYER;
				msg.iSubID = 0;
				msg.dwParam1 = CECHPWorkMove::DEST_DIR;
				msg.dwParam2 = *(DWORD*)&v.x;
				msg.dwParam3 = *(DWORD*)&v.y;
				msg.dwParam4 = *(DWORD*)&v.z;
				frame_record* fr = gr.get_cur_frame();

				if (fr)
					fr->push_event(new event_record_msg(msg));
			}
		}
	}

	ClearComboSkill();
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
	if (g_pGame->GetGameRun()->GetInputCtrl()->IsShiftPressed(Msg.dwParam3))
		iTraceFlag |= 0x80000000;

	if (g_pGame->GetGameRun()->GetInputCtrl()->IsAltPressed(Msg.dwParam3))
		iTraceFlag |= 0x40000000;

	if (pCDS->RayTrace(vStart, vDelta, 1.0f, &TraceRt, iTraceFlag, (DWORD)this))
	{
		if (pCDS->m_RayTraceRt.iEntity == ECENT_PLAYER)
		{
			m_idClickedMan = pCDS->m_RayTraceRt.iObjectID;
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			pGameUI->PopupPlayerContextMenu();
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

	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();

	if (pCmd->sReviveType != REVIVE_HERE_END)
	{
		SetUseGroundNormal(false);

		if (!IsDead())
			return;

		m_dwStates &= ~GP_STATE_CORPSE;

		if (!pCurWork || pCurWork->GetWorkID() != CECHPWork::WORK_DEAD)
		{
			ASSERT(pCurWork && pCurWork->GetWorkID() == CECHPWork::WORK_DEAD);
			return;
		}

		m_bAboutToDie	= false;
		m_fReviveExp	= -1.0f;

		//	Close death dialog
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->PopupReviveDialog(false);

		m_pWorkMan->FinishCurWork(CECHPWork::WORK_DEAD, true);
		
		if (pCmd->sReviveType == REVIVE_HERE_START)
		{
			CECHPWorkRevive* pWork = (CECHPWorkRevive*)m_pWorkMan->CreateWork(CECHPWork::WORK_REVIVE);
			m_pWorkMan->StartWork_p1(pWork);
		}
	}
	else	//	pCmd->sReviveType == REVIVE_HERE_END
	{
		if (!pCurWork || pCurWork->GetWorkID() != CECHPWork::WORK_REVIVE)
		{
			ASSERT(pCurWork && pCurWork->GetWorkID() == CECHPWork::WORK_REVIVE);
			return;
		}

		m_pWorkMan->FinishCurWork(CECHPWork::WORK_REVIVE, true);
	}
	
	if( g_pGame->GetGameRun()->GetHelper()->IsRunning() )
		g_pGame->GetGameSession()->bot_begin(0, 0);
}

//	Begin following the selected target
void CECHostPlayer::BeginFollow(int idTarget)
{
	if (gr.get_state() == game_record::enum_gr_replay)
		return;

	if (!CanDo(CANDO_FOLLOW))
		return;

	if (!ISPLAYERID(idTarget) || idTarget == m_PlayerInfo.cid)
		return;

	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
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

	if (gr.get_state() == game_record::enum_gr_record)
	{
		ECMSG msg;
		msg.dwMsg = MSG_HST_FOLLOW_RECORD;
		msg.iManager = MAN_PLAYER;
		msg.iSubID = 0;
		msg.dwParam1 = idTarget;
		msg.dwParam2 = 0;
		msg.dwParam3 = 0;
		msg.dwParam4 = 0;

		frame_record* fr = gr.get_cur_frame();

		if (fr)
			fr->push_event(new event_record_msg(msg));
	}
}

void CECHostPlayer::BeginAutoMove()
{
	
	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_AUTOMOVE)
	{
		pP1Work->Reset();
	}
	else
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_AUTOMOVE_BEGIN);
		CECHPWorkAutoMove * pWork = (CECHPWorkAutoMove*)m_pWorkMan->CreateWork(CECHPWork::WORK_AUTOMOVE);
		m_pWorkMan->StartWork_p1(pWork);
		LuaEvent::FirstAutoMove();
	}
	
}

bool CECHostPlayer::EndAutoMove()
{
	CECHPWork* pCurWork = m_pWorkMan->GetCurWork();
	if (pCurWork&& pCurWork->GetWorkID() == CECHPWork::WORK_AUTOMOVE)
	{
		m_pWorkMan->FinishCurWork(CECHPWork::WORK_AUTOMOVE, true);
		return true;
	}

	return false;
}


//	Level up
void CECHostPlayer::LevelUp()
{
	CECGameSession *pSession = g_pGame->GetGameSession();

	m_BasicProps.iLevel++;
	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_LEVELUP, m_BasicProps.iLevel);

	//	Get all extend properties
	pSession->c2s_CmdGetExtProps();

//	if (m_pLevelUpGFX)
//		m_pLevelUpGFX->Start(true);
	if (m_pTransfiguredModel)
		m_pTransfiguredModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
	else if (m_pChangedModel)
		m_pChangedModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
	else if (m_pPlayerModel)
		m_pPlayerModel->PlayGfx(res_GFXFile(RES_GFX_LEVELUP), NULL);
	
	//	Popup notify bubble text
	BubbleText(BUBBLE_LEVELUP, 0);

	//	Notify my friends that my level changed
	ACHAR szInfo[40];
	a_sprintf(szInfo, _AL("L%d"), m_BasicProps.iLevel);

	for (int i=0; i < m_pFriendMan->GetGroupNum(); i++)
	{
		CECFriendMan::GROUP* pGroup = m_pFriendMan->GetGroupByIndex(i);
		for (int j=0; j < pGroup->aFriends.GetSize(); j++)
		{
			CECFriendMan::FRIEND* pFriend = pGroup->aFriends[j];
			if (pFriend->iLine >= 0)
			{
				pSession->SendPrivateChatData(pFriend->GetName(),
							szInfo, GNET::CHANNEL_USERINFO, pFriend->id);
			}
		}
	}
	LuaEvent::LevelUp(m_BasicProps.iLevel + m_nRebornCount * 150);
}

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

*/
//	Can host touch target ?
//	iReason: 0 - no special reason; 1 - melee; 2 - cast magic; 3 - talk;
//	fMaxCut: Maximum cut distance
//bool CECHostPlayer::CanTouchTarget(const A3DVECTOR3& vTargetPos, float fTargetRad, 
//								int iReason, float fMaxCut/* 1.0f */)
/*{
	float fDist = a3d_Magnitude(vTargetPos - GetPos());

	// 回放时位置可能有很小的误差，导致判断不准，所以加大范围值
	if (gr.get_state() == game_record::enum_gr_replay)
	{
		float d = gr.get_play_speed() > 1.0f ? .1f * gr.get_play_speed() : .1f;
		fTargetRad += d;
	}

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
			float fRange = m_pPrepSkill->GetCastRange(m_ExtProps.attack_range);
			if (fRange > 0.0f)
			{
				if (fDist - fTargetRad <= fRange)
					return true;
			}
			else
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
	
	CECPetBedge* pPet = (CECPetBedge*)m_pPetPack->GetItem(m_nCurPetIndex);

	if (pPet && pPet->GetClassID() == CECIvtrItem::ICID_PET_BEDGE)
		pPet->RefreshCoolDown(dwTick);
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
			g_pGame->GetGameSession()->c2s_CmdLeaveCarrier(m_idCarrier, vPos, glb_CompressDirH(vDir.x, vDir.z));
			OutputDebugStringA("Send Leave Carrier!!\n");

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
					g_pGame->GetGameSession()->c2s_CmdEnterCarrier(out.iUserData2, vPos, glb_CompressDirH(vDir.x, vDir.z));
					OutputDebugStringA("Send Enter Carrier!!\n");

      				m_bCarrierCooling = true;
				}
			}
			else if (m_idCarrier)
			{
				//	TODO: tell server that we leave carrier now
				A3DVECTOR3 vPos, vDir;
				pNPCMan->GetPoseOnCarrier(m_idCarrier, m_matRelCarrier, vPos, vDir);
				g_pGame->GetGameSession()->c2s_CmdLeaveCarrier(m_idCarrier, vPos, glb_CompressDirH(vDir.x, vDir.z));
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
			CECHPWork* pWork = m_pWorkMan->GetCurWork();
			if (pWork && pWork->IsMoving())
				m_iMoveMode = MOVE_MOVE;
			else
				m_iMoveMode = MOVE_STAND;
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

	CECCDS* pCDS = pGameRun->GetWorld()->GetCDS();
	CECInputCtrl* pInputCtrl = pGameRun->GetInputCtrl();

	int x, y, iCursor=RES_CUR_NORMAL;
	pInputCtrl->GetMousePos(&x, &y);
	
	A3DVECTOR3 vDest((float)x, (float)y, 1.0f);
	A3DViewport* pView = g_pGame->GetViewport()->GetA3DViewport();
	pView->InvTransform(vDest, vDest);

	A3DVECTOR3 vStart = pView->GetCamera()->GetPos();
	A3DVECTOR3 vDelta = vDest - vStart;

	RAYTRACERT TraceRt;
	int iTraceFlag = TRACEOBJ_LBTNCLICK;
	if (pInputCtrl->KeyIsBeingPressed(VK_SHIFT))
		iTraceFlag |= 0x80000000;

	if (pInputCtrl->KeyIsBeingPressed(VK_MENU))
		iTraceFlag |= 0x40000000;

	if (pCDS->RayTrace(vStart, vDelta, 1.0f, &TraceRt, iTraceFlag, (DWORD)this))
	{
		int idHitObject = pCDS->m_RayTraceRt.iObjectID;
		bool bForceAttack = GetForceAttackFlag(NULL);

		if (pCDS->m_RayTraceRt.iEntity == ECENT_MATTER)
		{
			CECMatter* pMatter = pGameRun->GetWorld()->GetMatterMan()->GetMatter(idHitObject);
			if (pMatter)
			{
				if (!pMatter->IsMine())
					iCursor = RES_CUR_PICKUP;
				else if (CanGatherMatter(pMatter))
				{
					iCursor = RES_CUR_DIG;
					// 在采矿中加入新的鼠标悬浮图标
					DATA_TYPE DataType;
					const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(pMatter->GetTemplateID(), ID_SPACE_ESSENCE, DataType);

					if (DataType == DT_MINE_ESSENCE)
					{
						const MINE_ESSENCE* pData = (const MINE_ESSENCE*)pDataPtr;
						if(pData->cursor_icon && (RES_CUR_AXE + pData->cursor_icon -1)< NUM_RES_CURSOR)
							iCursor = RES_CUR_AXE + pData->cursor_icon -1;
 					}
				}				
			}

			if (iCursor != RES_CUR_NORMAL)
				m_idCurHover = idHitObject;
		}
		else if (pCDS->m_RayTraceRt.iEntity == ECENT_NPC)
		{
			m_idCurHover = idHitObject;

			CECNPC* pNPC = pGameRun->GetWorld()->GetNPCMan()->GetNPC(idHitObject);
			if (pNPC)
			{
				if (!IsInBattle())
				{
					if (m_pvp.ucFreePVPType == 3) // 人神pk，非战场，但是逻辑又希望借用战场的逻辑
					{
						if (pNPC->IsServerNPC() && AttackableJudge(idHitObject,bForceAttack) != 1)
							iCursor = RES_CUR_TALK;
						else if (m_idSelTarget == idHitObject && AttackableJudge(idHitObject, bForceAttack) == 1)
							iCursor = RES_CUR_ATTACK;
					}
					else if (pNPC->IsServerNPC())
						iCursor = RES_CUR_TALK;
					else if (pNPC->IsMonsterNPC())
					{
						if (m_idSelTarget == idHitObject &&	AttackableJudge(idHitObject, bForceAttack) == 1)
							iCursor = RES_CUR_ATTACK;
					}
				}
				else	//	Host is in battle
				{
					// 竞技场中，没有阵营信息，所有的怪物都是可以攻击的，除非是自己的宠物或自己的召唤兽
					if (m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN)
					{
						if (pNPC->IsServerNPC())
							iCursor = RES_CUR_TALK;
						else if ( (pNPC->IsPet() && pNPC->GetMasterID() == m_PlayerInfo.cid) || pNPC->GetSummonerID() == m_PlayerInfo.cid )
							iCursor = RES_CUR_NORMAL;
						else
							iCursor = RES_CUR_ATTACK;
					}
					else if (InSameBattleCamp(pNPC))
					{
						if (pNPC->IsServerNPC())
							iCursor = RES_CUR_TALK;
					}
					else	//	Host and npc aren't in same camp
					{
						iCursor = RES_CUR_ATTACK;
					}
				}
			}
		}
		else if (pCDS->m_RayTraceRt.iEntity == ECENT_PLAYER)
		{
			m_idCurHover = idHitObject;
			if (m_idSelTarget == idHitObject && AttackableJudge(idHitObject, bForceAttack) == 1)
				iCursor = RES_CUR_ATTACK;
		}
		else if (pCDS->m_RayTraceRt.iEntity == ECENT_CARRIER)
		{
			iCursor = RES_CUR_NORMAL;
		}

		l_fTestDist = vDelta.Magnitude() * TraceRt.fFraction;
	}
	else
		l_fTestDist = -1.0f;

	g_pGame->ChangeCursor(iCursor);
}
*/
//	Update time counters
//void CECHostPlayer::UpdateTimers(DWORD dwDeltaTime)
//{
	//	Get real time tick of this frame
//	int iRealTime = g_pGame->GetRealTickTime();

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
	
/*	int i;

	//	Update skills
	for (i=0; i < m_aPtSkills.GetSize(); i++)
		m_aPtSkills[i]->Tick();

	for (i=0; i < m_aPsSkills.GetSize(); i++)
		m_aPsSkills[i]->Tick();
	
	for (i=0; i < m_aTsSkills.GetSize(); i++)
		m_aTsSkills[i]->Tick();

	for (i=0; i < m_aImSkills.GetSize(); i++)
		m_aImSkills[i]->Tick();

	//	Update cool times
	for (i=0; i < GP_CT_MAX; i++)
	{
		if (m_aCoolTimes[i].iCurTime > 0)
		{
			if ((m_aCoolTimes[i].iCurTime -= iRealTime) < 0)
				m_aCoolTimes[i].iCurTime = 0;
		}
	}

	for (i=0; i < GP_CT_PRODUCE_END - GP_CT_PRODUCE_START; i++)
	{
		if (m_aProduceCoolTimes[i].iCurTime > 0)
		{
			if ((m_aProduceCoolTimes[i].iCurTime -= iRealTime) < 0)
				m_aProduceCoolTimes[i].iCurTime = 0;
		}
	}

	for (i=0; i< GP_CT_COMBINE_EDIT_END - GP_CT_COMBINE_EDIT_START; i++)
	{
		if (m_aSkillEditCoolTimes[i].iCurTime > 0)
		{
			if ((m_aSkillEditCoolTimes[i].iCurTime -= iRealTime) < 0)
				m_aSkillEditCoolTimes[i].iCurTime = 0;
		}
	}

	//	Gather time counter
	if (m_GatherCnt.IncCounter(iRealTime))
		m_GatherCnt.Reset(true);

	//	Bind command cool counter
	if (m_BindCmdCoolCnt.IncCounter(iRealTime))
		m_BindCmdCoolCnt.Reset(true);

	//	For some reasons on server, sometimes friend list couldn't be got
	//	successfully. Try to get it again every 20s if this case really happen
	if (m_pFriendMan && !m_pFriendMan->CheckInit())
	{
		m_iGetFriendCnt -= iRealTime;
		if (m_iGetFriendCnt < 0)
		{
			g_pGame->GetGameSession()->friend_GetList();
			m_iGetFriendCnt = 60000;
		}
	}
	
	//	Duel stopping time counter
	if (m_pvp.iDuelState == DUEL_ST_PREPARE)
	{
		m_pvp.iDuelTimeCnt -= iRealTime;
		if (m_pvp.iDuelTimeCnt < 0)
			m_pvp.iDuelTimeCnt = 0;
	}
	else if (m_pvp.iDuelState == DUEL_ST_INDUEL)
	{
		m_pvp.iDuelTimeCnt += iRealTime;
	}
	else if (m_pvp.iDuelState == DUEL_ST_STOPPING)
	{
		m_pvp.iDuelTimeCnt -= iRealTime;
		if (m_pvp.iDuelTimeCnt < 0)
		{
			m_pvp.iDuelTimeCnt = 0;
			m_pvp.iDuelState = DUEL_ST_NONE;
			m_pvp.idDuelOpp = 0;
		}
	}

	if( GetCombatFaction() )
	{
		m_nFactionCombatCounts += iRealTime;
	}

	if( IsInBattle() )
	{
		if( m_nGuildWarStartCounts != 0x7fffffff )
			m_nGuildWarStartCounts += iRealTime;
		if( m_nGuildWarWinCounts != 0x7fffffff )
			m_nGuildWarWinCounts += iRealTime;
		if( m_nGuildWarLostCounts != 0x7fffffff )
			m_nGuildWarLostCounts += iRealTime;
	}

	//	Update pariah time counter
	if (m_dwPariahTime)
	{
		if (m_dwPariahTime > (DWORD)iRealTime)
			m_dwPariahTime -= (DWORD)iRealTime;
		else
			m_dwPariahTime = 0;
	}

	//	Update pet operation time counter
	m_PetOptCnt.IncCounter(iRealTime);

	//	Update battle result time counter
	if (IsInBattle() && m_BattleInfo.iResult && m_BattleInfo.iResultCnt)
	{
		if ((m_BattleInfo.iResultCnt -= iRealTime) < 0)
			m_BattleInfo.iResultCnt = 0;
	}	

	//	Hasn't been attacked from a player for over 3 seconds, change to Not Under Attack state
	if (m_bUnderEPAtk && m_UnderAtkCnt.IncCounter(iRealTime))
	{
		m_bUnderEPAtk = false;
	}
	//	Time Elapsed since last attacking another player
	m_ReAtkCnt.IncCounter(iRealTime);

	if (m_pTransfiguredModel && m_TransfigureProp.iMoveType == TRANSFIGURE_MOVE_RANDOM)
	{
		m_RandomMoveCnt.IncCounter(iRealTime);
		m_RandomStopCnt.IncCounter(iRealTime);
	}
	// 对于起飞中释放技能导致飞剑或飞骑显示不出来的修正
	if (m_bFlyingOff && m_FlyingOffCnt.IncCounter(iRealTime))
	{
		ShowWing(true);
		m_bFlyingOff = false;
	}

	if (!m_SkillCoolCnt.IsFull())
		m_SkillCoolCnt.IncCounter(iRealTime);

	// 其他技能的冷却缓存时间的更新
	abase::hash_map<int, int>::iterator it;
	for(it= m_TsSkillCoolDown.begin(); it != m_TsSkillCoolDown.end(); ++it)
	{
		if (it->second > 0)
		{
			it->second -= iRealTime;
			if (it->second < 0)
				it->second = 0;
		}
	}
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
		A3DTerrainWater* pWater = g_pGame->GetGameRun()->GetWorld()->GetTerrainWater();

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
	if (!pEquip)
	{
		ASSERT(pEquip);
		return false;
	}

	if( !pEquip->VerifyInfoMD5() )
		return false;

	//	Equipment's endurance will be checked separatly in other place,
	//	so ignore it here
	if (m_BasicProps.iLevel < pEquip->GetLevelRequirement())
		return false;

	switch (pEquip->GetClassID())
	{
		case CECIvtrItem::ICID_EQUIP:
		case CECIvtrItem::ICID_TALISMAN_MAINPART:
		case CECIvtrItem::ICID_CHANGE_SHAPE_CARD:
		{
			if (!(pEquip->GetProfessionRequirement() & (UINT64 (1) << m_BasicProps.iProfession)))
				return false;
			if (pEquip->GetGenderRequirement() != 2 && pEquip->GetGenderRequirement() != m_iGender)
				return false;
			if (pEquip->GetRebornRequirement() > m_nRebornCount)
				return false;
			break;
		}
	}
	// 装备的声望限制
	if (pEquip->GetClassID() == CECIvtrItem::ICID_EQUIP)
	{
		const EQUIPMENT_ESSENCE * pEquipEss = pEquip->GetDBEssence();
		for (int i = 0; i < 2; ++i)
		{		
			if (pEquipEss->require_repu[i].index >= 0 && pEquipEss->require_repu[i].num > 0)
			{
				if (GetRegionReputation(pEquipEss->require_repu[i].index) < pEquipEss->require_repu[i].num)
					return false;
			}
		}
		if (pEquipEss->require_territory)
		{
			Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(m_idFaction);
			if (!pInfo)
				return false;
			if (pEquipEss->require_territory > pInfo->GetTerritoryScore())
				return false;
		}
	}
	
	return true;
}

//	Is spelling magic
bool CECHostPlayer::IsSpellingMagic()
{
	return (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SPELLOBJECT) ? true : false;
}

//	Is flash moving ?
bool CECHostPlayer::IsFlashMoving()
{
	return (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_FLASHMOVE) ? true : false;
}

//	Is taking off ?
bool CECHostPlayer::IsTakingOff()
{
	return (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_FLYOFF) ? true : false;
}

//	Is picking up something
bool CECHostPlayer::IsPicking()
{
	CECHPWork* pWork = m_pWorkMan->GetCurWork();
	if (pWork && pWork->GetWorkID() == CECHPWork::WORK_PICKUP)
		return !((CECHPWorkPick*)pWork)->IsGather();
	else
		return false;
}

//	Is gathering resources
bool CECHostPlayer::IsGathering()
{
	CECHPWork* pWork = m_pWorkMan->GetCurWork();
	if (pWork && pWork->GetWorkID() == CECHPWork::WORK_PICKUP)
		return ((CECHPWorkPick*)pWork)->IsGather();
	else
		return false;
}

bool CECHostPlayer::IsOperating()
{
	CECHPWork* pWork = m_pWorkMan->GetCurWork();
	return (pWork && pWork->GetWorkID() == CECHPWork::WORK_GENERAL_OPRT);
}

//	Is reviving
bool CECHostPlayer::IsReviving()
{
	return (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_REVIVE) ? true : false;
}

//	Is using item ?
bool CECHostPlayer::IsUsingItem()
{
	return (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_USEITEM) ? true : false;
}

//	Is doing session pose ?
bool CECHostPlayer::DoingSessionPose()
{
	CECHPWork* pWork = m_pWorkMan->GetCurWork();
	if (pWork && pWork->GetWorkID() == CECHPWork::WORK_STAND)
		return ((CECHPWorkStand*)pWork)->DoingSessionPose();
	else
		return false;
}

//	Update item shortcut when item position changed
void CECHostPlayer::UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot)
{
	int i;

	//	Update shortcut sets 1
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->UpdateMovedItemSC(tidItem, iSrcIvtr, iSrcSlot, iDstIvtr, iDstSlot);
	}

	//	Update shortcut sets 2
	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->UpdateMovedItemSC(tidItem, iSrcIvtr, iSrcSlot, iDstIvtr, iDstSlot);
	}
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTalisman*)pGameUI->GetDialog("Win_Talisman"))->UpdateMovedItemSC(tidItem, iSrcIvtr, iSrcSlot, iDstIvtr, iDstSlot);
	((CDlgTalisman*)pGameUI->GetDialog("Win_TalismanSmall"))->UpdateMovedItemSC(tidItem, iSrcIvtr, iSrcSlot, iDstIvtr, iDstSlot);
	((CDlgPetPackage*)pGameUI->GetDialog("Win_PetPackage"))->UpdateMovedItemSC(tidItem, iSrcIvtr, iSrcSlot, iDstIvtr, iDstSlot);
}
*/
//	Update item shortcut when item removed
//void CECHostPlayer::UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot,
//								int* aExcSlots/* NULL */, int iNumExcSlot/* 0 */)
/*{
	int i, iSameItem = -1;

	//	Are there some same kind items exists ? Some items with different
	//	id but have the same effect. For example, when we counldn't find
	//	middle heal bottle, we can use small or large heal bottles as candidates.
	if (iIvtr == IVTRTYPE_PACK)
	{
		int iCandidate = -1; 
		int iCandidate2 = -1;

		for (i=0; i < m_pPack->GetSize(); i++)
		{
			CECIvtrItem* pItem = m_pPack->GetItem(i);
			if (i == iSlot || !pItem)
				continue;

			//	Check exclude slots
			if (aExcSlots && iNumExcSlot)
			{
				for (int j=0; j < iNumExcSlot; j++)
				{
					if (i == aExcSlots[j])
						break;
				}

				if (j < iNumExcSlot)
					continue;
			}

			int idSlotItem = pItem->GetTemplateID();
			if (idSlotItem == tidItem)
			{
				iSameItem = i;
				break;
			}
			else if (iCandidate < 0 && CECIvtrItem::IsCandidate(tidItem, pItem) && pItem->CheckUseCondition())
				iCandidate = i;
			else if (iCandidate2< 0 && CECIvtrItem::IsCandidate(tidItem, pItem, false) && pItem->CheckUseCondition())
				iCandidate2 = i;
		}
//		if (i >= m_pPack->GetSize() && iCandidate >= 0)
// 			iSameItem = iCandidate;
		if(i >= m_pPack->GetSize())
		{
			if(iCandidate >= 0)
				iSameItem = iCandidate;
			else if(iCandidate2 >= 0)
				iSameItem = iCandidate2;
		}
	}

	//	Update horizontal shortcut sets
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->UpdateRemovedItemSC(tidItem, iIvtr, iSlot, iSameItem);
	}

	//	Update vertical shortcut sets
	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->UpdateRemovedItemSC(tidItem, iIvtr, iSlot, iSameItem);
	}
	
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTalisman*)pGameUI->GetDialog("Win_Talisman"))->UpdateRemovedItemSC(tidItem, iIvtr, iSlot, iSameItem);
	((CDlgTalisman*)pGameUI->GetDialog("Win_TalismanSmall"))->UpdateRemovedItemSC(tidItem, iIvtr, iSlot, iSameItem);
	((CDlgPetPackage*)pGameUI->GetDialog("Win_PetPackage"))->UpdateRemovedItemSC(tidItem, iIvtr, iSlot, iSameItem);
}

//	Update item shortcut when two items exchanged
void CECHostPlayer::UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2)
{
	int i;

	//	Update shortcut sets 1
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->UpdateExchangedItemSC(tidItem1, iIvtr1, iSlot1, tidItem2, iIvtr2, iSlot2);
	}

	//	Update shortcut sets 2
	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->UpdateExchangedItemSC(tidItem1, iIvtr1, iSlot1, tidItem2, iIvtr2, iSlot2);
	}
	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	((CDlgTalisman*)pGameUI->GetDialog("Win_Talisman"))->UpdateExchangedItemSC(tidItem1, iIvtr1, iSlot1, tidItem2, iIvtr2, iSlot2);
	((CDlgTalisman*)pGameUI->GetDialog("Win_TalismanSmall"))->UpdateExchangedItemSC(tidItem1, iIvtr1, iSlot1, tidItem2, iIvtr2, iSlot2);
	((CDlgPetPackage*)pGameUI->GetDialog("Win_PetPackage"))->UpdateExchangedItemSC(tidItem1, iIvtr1, iSlot1, tidItem2, iIvtr2, iSlot2);
}

//	Update shortcuts
void CECHostPlayer::UpdateShortcuts(DWORD dwDeltaTime)
{
	int i;

	//	Update shortcut sets 1
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->Tick(dwDeltaTime);
	}

	//	Update shortcut sets 2
	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->Tick(dwDeltaTime);
	}
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
		m_pBuyPack->RemoveAllItems();
		m_pSellPack->RemoveAllItems();
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
*/
//	End NPC service
//void CECHostPlayer::EndNPCService()
//{
	/*
	CECNPCServer* pNPC = (CECNPCServer*)g_pGame->GetGameRun()->GetWorld()->GetNPCMan()->GetNPC(m_idSevNPC);

	if (pNPC && pNPC->IsServerNPC())
		pNPC->RestorServerDir();
		*/

/*	m_idSevNPC = 0;
	m_bTalkWithNPC = false;
	m_iBoothState = 0;
}

//	Buy items from NPC
void CECHostPlayer::BuyItemsFromNPC()
{
	using namespace C2S;

	//	Check package content at first
	if (m_pBuyPack->GetSize() - m_pBuyPack->GetEmptySlotNum() > m_pPack->GetEmptySlotNum())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_PACKFULL1);
		return;
	}

	AArray<npc_trade_item, npc_trade_item&> aItems(0, m_pBuyPack->GetSize());
	INT64 iPrice = 0;

	for (int i=0; i < m_pBuyPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = m_pBuyPack->GetItem(i);
		if (!pItem)
			continue;

		const CECDealInventory::ITEMINFO& ItemInfo = m_pBuyPack->GetItemInfo(i);

		npc_trade_item dst;
		dst.tid		= pItem->GetTemplateID();
		dst.index	= ItemInfo.iOrigin;
		dst.count	= ItemInfo.iAmount;
		aItems.Add(dst);

		iPrice += CECIvtrItem::GetScaledPrice(pItem->GetUnitPrice(), ItemInfo.iAmount, 
			CECIvtrItem::SCALE_SELL, PLAYER_PRICE_SCALE);
	}

	if (aItems.GetSize() && iPrice <= m_iMoneyCnt)
		g_pGame->GetGameSession()->c2s_CmdNPCSevBuy(aItems.GetSize(), aItems.GetData());

	//	Clear buy pack
	m_pBuyPack->RemoveAllItems();
}

//	Sell items to NPC
void CECHostPlayer::SellItemsToNPC()
{
	using namespace C2S;

	AArray<npc_trade_item, npc_trade_item&> aItems(0, m_pSellPack->GetSize());

	for (int i=0; i < m_pSellPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = m_pSellPack->GetItem(i);
		if (!pItem)
			continue;

		const CECDealInventory::ITEMINFO& ItemInfo = m_pSellPack->GetItemInfo(i);

		npc_trade_item dst;
		dst.tid		= pItem->GetTemplateID();
		dst.index	= ItemInfo.iOrigin;
		dst.count	= ItemInfo.iAmount;
		aItems.Add(dst);
	}

	if (aItems.GetSize())
		g_pGame->GetGameSession()->c2s_CmdNPCSevSell(aItems.GetSize(), aItems.GetData());

	//	Clear sell pack
	m_pSellPack->RemoveAllItems();
}

//	Buy items from else player's booth
void CECHostPlayer::BuyItemsFromBooth()
{
	using namespace C2S;

	//	Check package content at first
	if (m_pBuyPack->GetSize() - m_pBuyPack->GetEmptySlotNum() > m_pPack->GetEmptySlotNum())
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_PACKFULL1);
		return;
	}

	AArray<npc_booth_item, npc_booth_item&> aItems(0, m_pBuyPack->GetSize());
	INT64 iPrice = 0;

	for (int i=0; i < m_pBuyPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = m_pBuyPack->GetItem(i);
		if (!pItem)
			continue;

		const CECDealInventory::ITEMINFO& ItemInfo = m_pBuyPack->GetItemInfo(i);

		npc_booth_item dst;
		dst.tid			= pItem->GetTemplateID();
		dst.index		= ItemInfo.iOrigin;
		dst.inv_index	= 0;
		dst.count		= ItemInfo.iAmount;
		aItems.Add(dst);

		iPrice += CECIvtrItem::GetScaledPrice(pItem->GetUnitPrice(), ItemInfo.iAmount, 
			CECIvtrItem::SCALE_BOOTH, 1.0f);
	}

	if (aItems.GetSize() && iPrice <= m_iMoneyCnt)
		g_pGame->GetGameSession()->c2s_CmdNPCSevBoothBuy(m_idBoothTrade, aItems.GetSize(), aItems.GetData());

	//	Clear buy pack
	m_pBuyPack->RemoveAllItems();
}

//	Sell items to else player's booth
void CECHostPlayer::SellItemsToBooth()
{
	using namespace C2S;

	AArray<npc_booth_item, npc_booth_item&> aItems(0, m_pSellPack->GetSize());

	for (int i=0; i < m_pSellPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = m_pSellPack->GetItem(i);
		if (!pItem)
			continue;

		const CECDealInventory::ITEMINFO& ItemInfo = m_pSellPack->GetItemInfo(i);

		npc_booth_item dst;
		dst.tid			= pItem->GetTemplateID();
		dst.inv_index	= ItemInfo.iFlag;
		dst.index		= ItemInfo.iOrigin;
		dst.count		= ItemInfo.iAmount;
		aItems.Add(dst);
	}

	if (aItems.GetSize())
		g_pGame->GetGameSession()->c2s_CmdNPCSevBoothSell(m_idBoothTrade, aItems.GetSize(), aItems.GetData());

	//	Clear sell pack
	m_pSellPack->RemoveAllItems();
}

//	When the booth we are visiting closed
void CECHostPlayer::OnOtherBoothClosed()
{
	EndNPCService();
	
	//	Unfreeze items dragged into sell pack
	for (int i=0; i < m_pSellPack->GetSize(); i++)
	{
		if (m_pSellPack->GetItem(i))
		{
			const CECDealInventory::ITEMINFO& ii = m_pSellPack->GetItemInfo(i);
			CECIvtrItem* pItem = m_pPack->GetItem(ii.iFlag);
			if (pItem)
				pItem->Freeze(false);
		}
	}
	
	//	Else player close booth
	m_pEPBoothSPack->RemoveAllItems();
	m_pEPBoothBPack->RemoveAllItems();

	CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	pGameUI->PopupBoothDialog(false, false);
}

//	Open booth
void CECHostPlayer::OpenBooth(const ACHAR* szName)
{
	int i, iCount=0;
	C2S::cmd_open_booth::entry_t aEntries[IVTRSIZE_BOOTHSPACK + IVTRSIZE_BOOTHBPACK];

	for (i=0; i < IVTRSIZE_BOOTHSPACK; i++)
	{
		CECIvtrItem *pItem = m_pBoothSPack->GetItem(i);
		if (!pItem)
			continue;

		//	Record booth index
		m_pBoothSPack->SetItemInfoFlag(i, iCount);

		const CECDealInventory::ITEMINFO& ii = m_pBoothSPack->GetItemInfo(i);
		aEntries[iCount].type = pItem->GetTemplateID();
		aEntries[iCount].index = ii.iOrigin;
		aEntries[iCount].count = ii.iAmount;
		aEntries[iCount].price = pItem->GetUnitPrice();
		iCount++;
	}

	for (i=0; i < IVTRSIZE_BOOTHSPACK; i++)
	{
		CECIvtrItem *pItem = m_pBoothBPack->GetItem(i);
		if (!pItem)
			continue;

		//	Record booth index
		m_pBoothBPack->SetItemInfoFlag(i, iCount);

		const CECDealInventory::ITEMINFO& ii = m_pBoothBPack->GetItemInfo(i);
		aEntries[iCount].type = pItem->GetTemplateID();
		aEntries[iCount].index = 0xFFFF;
		aEntries[iCount].count = ii.iAmount;
		aEntries[iCount].price = pItem->GetUnitPrice();
		iCount++;
	}

	g_pGame->GetGameSession()->c2s_CmdOpenBooth(iCount, szName, aEntries);

	SetBoothName(szName);
}

//	Release skills
void CECHostPlayer::ReleaseSkills()
{
	int i;

	//	Release positive skills
	for (i=0; i < m_aPtSkills.GetSize(); i++)
		delete m_aPtSkills[i];

	m_aPtSkills.RemoveAll(false);

	//	Release passive skills
	for (i=0; i < m_aPsSkills.GetSize(); i++)
		delete m_aPsSkills[i];

	m_aPsSkills.RemoveAll(false);

	for (i=0; i < m_aTsSkills.GetSize(); i++)
		delete m_aTsSkills[i];

	m_aTsSkills.RemoveAll(false);

	for (i=0; i < m_aImSkills.GetSize(); i++)
		delete m_aImSkills[i];

	m_aImSkills.RemoveAll(false);

	//	Release else player's skills
	for (i=0; i < m_aEPSkills.GetSize(); i++)
		delete m_aEPSkills[i];
	m_aEPSkills.RemoveAll(false);
}

//	Update equipment skins
bool CECHostPlayer::UpdateEquipSkins(bool bAtOnce)
{
	int i, aNewEquips[IVTRSIZE_EQUIPPACK];
	DWORD dwChangeMask = EQUIP_MASK_ALL;
	// 竞技场中，无法换装
	if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType ==BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
		return true;
	
	for (i=0; i < IVTRSIZE_EQUIPPACK; i++)
	{
		CECIvtrItem* pItem = m_pEquipPack->GetItem(i);
		if (pItem)
		{
			if( i == EQUIPIVTR_SPEAKER )
				m_nCurEmotionSet = ((CECIvtrSpeaker *)pItem)->GetDBEssence()->id_icon_set;

			aNewEquips[i] = pItem->GetTemplateID();
			if( i >= EQUIPIVTR_WEAPON && i <= EQUIPIVTR_FOOT )
			{
				CECIvtrEquip * pEquip = (CECIvtrEquip *) pItem;
				WORD num_estone = pEquip->GetEssence().num_estone;
				aNewEquips[i] |= (num_estone << 16) & 0x7fffffff;
			}
			if( i == EQUIPIVTR_MAIN_TALISMAN || i == EQUIPIVTR_WING)
			{
				CECIvtrTalismanMain * pEquip = (CECIvtrTalismanMain *) pItem;
				WORD quality = pEquip->GetEssence().ess.quality & 0x00ff;
				aNewEquips[i] |= (quality << 16) & 0x7fffffff;
			}
		}
		else
		{
			aNewEquips[i] = 0;
			if( i == EQUIPIVTR_SPEAKER )
				m_nCurEmotionSet = 0;
		}
	}


	ShowEquipments(aNewEquips, true, bAtOnce);

	SetResReadyFlag(RESFG_SKIN, true);

	return true;
}
*/
//	Get positive skill by id
//CECSkill* CECHostPlayer::GetPositiveSkillByID(int id, bool bSenior/* false */)
/*{
	CECSkill* pSenior = NULL;

	for (int i=0; i < m_aPtSkills.GetSize(); i++)
	{
		if (m_aPtSkills[i]->GetSkillID() == id)
			return m_aPtSkills[i];
		else if (m_aPtSkills[i]->GetJunior() == id)
			pSenior = m_aPtSkills[i];
	}

	if (bSenior && pSenior)
		return pSenior;

	return NULL;
}
*/
//	Get passive skill by id
//CECSkill* CECHostPlayer::GetPassiveSkillByID(int id, bool bSenior/* false */)
/*{
	CECSkill* pSenior = NULL;

	for (int i=0; i < m_aPsSkills.GetSize(); i++)
	{
		if (m_aPsSkills[i]->GetSkillID() == id)
			return m_aPsSkills[i];
		else if (m_aPsSkills[i]->GetJunior() == id)
			pSenior = m_aPsSkills[i];
	}

	if (bSenior && pSenior)
		return pSenior;

	return NULL;
}
*/
//CECSkill* CECHostPlayer::GetTransfigureSkillByID(int id, bool bSenior /* = false */)
/*{
	CECSkill* pSenior = NULL;

	for (int i=0; i < m_aTsSkills.GetSize(); i++)
	{
		if (m_aTsSkills[i]->GetSkillID() == id)
			return m_aTsSkills[i];
		else if (m_aTsSkills[i]->GetJunior() == id)
			pSenior = m_aTsSkills[i];
	}

	if (bSenior && pSenior)
		return pSenior;

	return NULL;
}
*/
// get item skill by id
CECSkill* CECHostPlayer::GetItemSkillByID(int id, bool bSenior /* = false */)
{
	CECSkill* pSenior = NULL;

	for (int i=0; i < m_aImSkills.GetSize(); i++)
	{
		if (m_aImSkills[i]->GetSkillID() == id)
			return m_aImSkills[i];
		else if (m_aImSkills[i]->GetJunior() == id)
			pSenior = m_aImSkills[i];
	}

	if (bSenior && pSenior)
		return pSenior;

	return NULL;
}

//	Get else player's skill by id
//CECSkill* CECHostPlayer::GetElsePlayerSkillByID(int id, bool bSenior/* false */)
/*{
	CECSkill* pSenior = NULL;

	for (int i=0; i < m_aEPSkills.GetSize(); i++)
	{
		if (m_aEPSkills[i]->GetSkillID() == id)
			return m_aEPSkills[i];
		else if (m_aEPSkills[i]->GetJunior() == id)
			pSenior = m_aEPSkills[i];
	}

	if (bSenior && pSenior)
		return pSenior;

	return NULL;
}
*/
//	Get skill by id
//CECSkill* CECHostPlayer::GetSkill(int id, bool bSenior/* false */)
/*{
	CECSkill* pSkill = NULL;
	if (GNET::ElementSkill::GetType(id) == CECSkill::TYPE_PASSIVE ||
		GNET::ElementSkill::GetType(id) == CECSkill::TYPE_LIVE)
		pSkill = GetPassiveSkillByID(id, bSenior);
	else
		pSkill = GetPositiveSkillByID(id, bSenior);
	
	if (!pSkill)
		pSkill = GetTransfigureSkillByID(id, bSenior);

	if (!pSkill)
		pSkill = GetItemSkillByID(id, bSenior);

	return pSkill;
}

//	Check whether host can learn specified skill
int CECHostPlayer::CheckSkillLearnCondition(int idSkill, bool bCheckBook)
{
	int iLevel = 1;
	CECSkill* pSkill = GetSkill(idSkill);
	if (pSkill)
		iLevel = pSkill->GetSkillLevel() + 1;

	if (iLevel == 1 && bCheckBook)
	{
		//	Do we have the skill book ?
		int idBook = 0; // GNET::ElementSkill::GetRequiredBook(idSkill, iLevel);
		if (idBook && m_pPack->FindItem(idBook) < 0)
			return 8;
	}

	//	Build player information
	GNET::LearnRequirement Info;
	memset(&Info, 0, sizeof (Info));

	Info.level		= m_BasicProps.iLevel;
	Info.profession	= m_BasicProps.iProfession;
	Info.sp			= m_BasicProps.iStatusPt;
	Info.talent		= m_nTalentValue;
	Info.reborncount= m_RebornInfo.size();
	Info.region_repu= m_vecRegionRepu.begin();
	Info.repu_count	= m_vecRegionRepu.size();
	Info.cultivation= m_iCultivation;
	
	//专为夫妻技能而增加的掩码
	if(GetSpouse())
	{
		enum { SPOUSE_MASK  = 0x08, };
		Info.cultivation |= SPOUSE_MASK;
	}
	
	return GNET::ElementSkill::LearnCondition(idSkill, Info, iLevel);
}

//	Check whether host can cast specified skill
int CECHostPlayer::CheckSkillCastCondition(CECSkill* pSkill)
{
	GNET::UseRequirement Info;
	memset(&Info, 0, sizeof (Info));

	Info.mp	= m_BasicProps.iCurMP;
	Info.form = m_iChangeShape;
	Info.cultivation = m_iCultivation;

	//专为夫妻技能而增加的掩码
	if(GetSpouse())
	{
		enum { SPOUSE_MASK  = 0x08, };
		Info.cultivation |= SPOUSE_MASK;
	}
	Info.weapon_type = m_uAttackType;
*/	//	Get weapon's major class ID
/*	CECIvtrWeapon* pWeapon = (CECIvtrWeapon*)m_pEquipPack->GetItem(EQUIPIVTR_WEAPON);
	if (!pWeapon || !pWeapon->GetCurEndurance() || !CanUseEquipment(pWeapon))
		Info.weapon	= 0;
	else
		Info.weapon = pWeapon->GetDBMajorType()->id;
*/
/*	enum
	{
		PLAYER_IN_BATTLE		= 0x00000001,
		PLAYER_IS_FIGHTING		= 0x00000002,
		PLAYER_HAS_CLONE		= 0x00000004,
		PLAYER_IN_MOUNT			= 0x00000008,
		PLAYER_IN_FLYING		= 0x00000010,
	};

	if (IsInBattle())
		Info.skill_limit |= PLAYER_IN_BATTLE;
	if (IsFighting())
		Info.skill_limit |= PLAYER_IS_FIGHTING;
	if (GetCloneID() && m_pPlayerMan->GetPlayer(GetCloneID()))
		Info.skill_limit |= PLAYER_HAS_CLONE;
	if (m_pPetModel)
		Info.skill_limit |= PLAYER_IN_MOUNT;
	if (IsFlying())
		Info.skill_limit |= PLAYER_IN_FLYING;

	SetPlayerProps(pSkill);

	return pSkill->CheckCastCondition(Info);
}

int CECHostPlayer::CheckSkillCostItemCondition(CECSkill* pSkill)
{
	int id1, id2;
	int iNum1, iNum2;
	if(pSkill->IsNeedItem())
	{
		id1 = pSkill->GetItem1Id();
		id2 = pSkill->GetItem2Id();
		iNum1 = pSkill->GetItem1Num();
		iNum2 = pSkill->GetItem2Num();
		int con = pSkill->GetItemRelation();
		if(con == 0)
		{
			if(id1 && iNum1 <= GetPack()->GetItemTotalNum(id1))
				return 0;
			else
				return 1;
		}
		else if(con == 1)
		{
			if(iNum1 <= GetPack()->GetItemTotalNum(id1) &&
			   iNum2 <= GetPack()->GetItemTotalNum(id2) )
			   return 0;
			else
				return 1;
		}
		else if(con ==2)
		{
			if(iNum1 <= GetPack()->GetItemTotalNum(id1) ||
			   iNum2 <= GetPack()->GetItemTotalNum(id2) )
			   return 0;
			else
				return 1;
		}
		else
			return 0;
 	}
	return 0;
}

void CECHostPlayer::SetPlayerProps(CECSkill* pSkill)
{
	// 设置玩家属性到PlayerWrapper中
	// 以后如果属性多了，可以区分处理，一些固定的属性设置一次就ok，另外一些，则需要不停的更新
	// 因为设置过去的属性只是在一个技能的本体中保存，每个技能都有一份，所以，固定属性也需要不停的设置。
	GNET::PlayerProps props;
	memset(&props, 0, sizeof(props));
	props.level = m_BasicProps.iLevel;
	pSkill->SetPlayerProps(props);
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
	int i;

	//	Update shortcuts ...
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->ReplaceSkillID(idSkill, NULL);
	}

	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->ReplaceSkillID(idSkill, NULL);
	}

	//	Remove skill
	for (i=0; i < m_aPtSkills.GetSize(); i++)
	{
		if (m_aPtSkills[i]->GetSkillID() == idSkill)
		{
			delete m_aPtSkills[i];
			m_aPtSkills.RemoveAt(i);
			return;
		}
	}

	for (i=0; i < m_aPsSkills.GetSize(); i++)
	{
		if (m_aPsSkills[i]->GetSkillID() == idSkill)
		{
			delete m_aPsSkills[i];
			m_aPsSkills.RemoveAt(i);
			return;
		}
	}

	for (i=0; i < m_aTsSkills.GetSize(); i++)
	{
		if (m_aTsSkills[i]->GetSkillID() == idSkill)
		{
			delete m_aTsSkills[i];
			m_aTsSkills.RemoveAt(i);
			return;
		}
	}

	for (i=0; i< m_aImSkills.GetSize(); i++)
	{
		if (m_aImSkills[i]->GetSkillID() == idSkill)
		{
			delete m_aImSkills[i];
			m_aImSkills.RemoveAt(i);
			return;
		}
	}
}
*/
//	Cast skill
//bool CECHostPlayer::CastSkill(int idTarget, bool bForceAttack, CECObject* pTarget/* NULL */)
/*{
	// 去掉多余的技能冷却提示
	static iLastCoolSkillID = 0;
	if (m_SkillCoolCnt.IsFull())
	{
		m_SkillCoolCnt.Reset();
		iLastCoolSkillID = 0;
	}
	if (!m_pPrepSkill || !m_pPrepSkill->ReadyToCast() || IsSpellingMagic())
	{
		if (m_pPrepSkill && !m_pPrepSkill->ReadyToCast() && m_pPrepSkill->GetSkillID() != iLastCoolSkillID)
		{
			iLastCoolSkillID = m_pPrepSkill->GetSkillID();
			m_SkillCoolCnt.Reset();
			g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_COOLING);
		}

		m_pPrepSkill = NULL;
		return false;
	}

	// 在飞船上，无法释放技能
	if (GetCarrierID())
		return false;

	//	Check target when this is a negative skill.
	if (m_pPrepSkill->GetType() == CECSkill::TYPE_ATTACK ||
		m_pPrepSkill->GetType() == CECSkill::TYPE_CURSE)
	{
		if (idTarget && AttackableJudge(idTarget, bForceAttack) != 1)
		{
			m_pPrepSkill = NULL;
			return false;
		}
	}
	
	// 检查使用该技能需要消耗的物品是否存在
	int iItemCon = CheckSkillCostItemCondition(m_pPrepSkill);
	if(iItemCon)
	{
		g_pGame->GetGameRun()->AddFixedChannelMsg(FIXMSG_SKILL_COST_ITEM, GP_CHAT_SYSTEM);
		return false;
	}
	//	Check cast condition
	int iRet = CheckSkillCastCondition(m_pPrepSkill);
	
	if (iRet)
	{
		switch (iRet)
		{
		case 2:	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_NEEDMP);	break;
		case 8:	g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_NEEDAP);	break;
		}

		m_pPrepSkill = NULL;
		return false;
	}
	
	// 变身情况下，而且屏蔽了技能
	if (IsTransfigured() && !GetTransfigureSkillByID(m_pPrepSkill->GetSkillID()) && m_TransfigureProp.bShieldSkill)
	{
		g_pGame->GetGameRun()->AddFixedMessage(FIXMSG_SKILL_DISABLED);
		return false;
	}
	// 变身情况下，而且使用的是变身技能
	BYTE byPVPMask = BuildPVPMask(bForceAttack);
	if (IsTransfigured() && GetTransfigureSkillByID(m_pPrepSkill->GetSkillID()))
	{
		int skill_type = m_pPrepSkill->IsInstant()? 1:0;
		g_pGame->GetGameSession()->c2s_CmdCastTransfigureSkill(m_pPrepSkill->GetSkillID(), skill_type, byPVPMask, 1, &idTarget);
		if(m_pPrepSkill->IsInstant())
			m_pPrepSkill = NULL;
	}
	else if (m_ApoiseSkill.idSkill && GetItemSkillByID(m_pPrepSkill->GetSkillID()))
	{
		int skill_type = m_pPrepSkill->IsInstant()? 1:0;
		g_pGame->GetGameSession()->c2s_CmdCastApoiseSkill(m_pPrepSkill->GetSkillID(), skill_type, m_ApoiseSkill.tidItem, m_ApoiseSkill.iItemSlot, byPVPMask, 1, &idTarget);
		if (m_pPrepSkill->IsInstant())
			m_pPrepSkill = NULL;
		ClearApoiseSkill();
	}
	else
	{
		if (m_pPrepSkill->IsInstant())
		{
			if (m_pPrepSkill->IsCharge())
			{
				A3DObject *pTar = g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 1);
				if (!pTar)
				{
					ASSERT(pTar);
					return false;
				}
				A3DVECTOR3 TargetPos;
				A3DVECTOR3 TargetDir;
				if (ISNPCID(idTarget))
				{
  	        		CECNPC* pNPC = (CECNPC*)pTar;
					TargetPos = pNPC->GetPos();
					TargetDir = pNPC->GetDir();
					TargetDir.y = 0;
					TargetDir.Normalize();
					TargetPos -= TargetDir;
				}
         		else if (ISPLAYERID(idTarget))
				{
         			CECElsePlayer* pPlayer = (CECElsePlayer*)pTar;
					TargetPos = pPlayer->GetPos();
					TargetDir = pPlayer->GetDir();
					TargetDir.y = 0;
					TargetDir.Normalize();
					TargetPos -= TargetDir;
				}
         		else
				{
         			ASSERT(0);
					return false;
				}
				
//				A3DVECTOR3 dirH = (TargetPos - GetPos());
//				dirH.y = 0;
//				float fDist = dirH.Normalize();
//				A3DVECTOR3 vDest = m_MoveCtrl.TryToGo(fDist, dirH);
				A3DVECTOR3 vDest = m_MoveCtrl.TryToGo(TargetPos);
				
				g_pGame->GetGameSession()->c2s_CmdCastChargeSkill(m_pPrepSkill->GetSkillID(), byPVPMask, idTarget, vDest, 1, &idTarget);
			}
			else
				g_pGame->GetGameSession()->c2s_CmdCastInstantSkill(m_pPrepSkill->GetSkillID(), byPVPMask, 1, &idTarget);
			m_pPrepSkill = NULL;
		}
		else
		{
			g_pGame->GetGameSession()->c2s_CmdCastSkill(m_pPrepSkill->GetSkillID(), byPVPMask, 1, &idTarget);
		}
	}
	iLastCoolSkillID = 0;
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

	//	Save shortcut configs...
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		m_aSCSets1[i]->SaveConfigData(pData, &iSize);
		iTotalSize += iSize;
		if (pData)
			pData += iSize;
	}

	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		m_aSCSets2[i]->SaveConfigData(pData, &iSize);
		iTotalSize += iSize;
		if (pData)
			pData += iSize;
	}

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
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		int iSize;
		if (!m_aSCSets1[i]->LoadConfigData(pData, &iSize, dwVer))
			return false;

		pData += iSize;
	}

	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		int iSize;
		if (!m_aSCSets2[i]->LoadConfigData(pData, &iSize, dwVer))
			return false;

		pData += iSize;
	}

	return true;
}
*/
//	Calculate distance between host and specified object
//bool CECHostPlayer::CalcDist(int idObject, float* pfDist, CECObject** ppObject/* NULL */)
/*{
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
		CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		pGameUI->PopupReviveDialog(true);
	}

	m_bEnterGame = true;
	gr.reset_seeking();
}
*/
//	Attack an object
//void CECHostPlayer::NormalAttackObject(int idTarget, bool bForceAttack, bool bMoreClose/* false */)
/*{
	if (!idTarget || idTarget == m_PlayerInfo.cid)
	{
		//	We should have check target isn't dead
		ASSERT(idTarget && idTarget != m_PlayerInfo.cid);
		return;
	}

	if (!g_pGame->GetGameRun()->GetWorld()->GetObject(idTarget, 1))
		return;

	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();
	CECHPWorkTrace* pWork = NULL;
	bool bStartNewWork = false;

	//	Trace a object
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		pWork = (CECHPWorkTrace*)pP1Work;
	}
	else if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_HACKOBJECT)
	{
		if (((CECHPWorkMelee*)pP1Work)->GetTarget() == idTarget)
			return;	//	Host is attacking the target

		pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
		bStartNewWork = true;
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
	{
		pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
		bStartNewWork = true;
	}

	if (pWork)
	{
		pWork->ChangeTarget(idTarget, CECHPWorkTrace::TRACE_ATTACK);
		pWork->SetForceAttack(bForceAttack);
		pWork->SetMoveCloseFlag(bMoreClose);

		if (bStartNewWork)
			m_pWorkMan->StartWork_p1(pWork);
	}

	m_bAboutToFight = true;
}

//	Pickup an dobject
void CECHostPlayer::PickupObject(int idTarget, bool bGather)
{
	if (IsDead() || IsSpellingMagic() || !idTarget || !ISMATTERID(idTarget))
		return;

	//	Check matter type
	CECMatter* pMatter = g_pGame->GetGameRun()->GetWorld()->GetMatterMan()->GetMatter(idTarget);
	if (!pMatter || bGather != pMatter->IsMine())
		return;

	if (bGather && !CanGatherMatter(pMatter))
		return;

	CECHPWork* pP1Work = m_pWorkMan->GetWork_p1();

	//	Trace a object
	if (pP1Work && pP1Work->GetWorkID() == CECHPWork::WORK_TRACEOBJECT)
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)pP1Work;
		pWork->ChangeTarget(idTarget, bGather ? CECHPWorkTrace::TRACE_GATHER : CECHPWorkTrace::TRACE_PICKUP);
	}
	else if (!pP1Work || pP1Work->CanTransferTo(CECHPWork::MASK_TRACEOBJECT))
	{
		CECHPWorkTrace* pWork = (CECHPWorkTrace*)m_pWorkMan->CreateWork(CECHPWork::WORK_TRACEOBJECT);
		pWork->ChangeTarget(idTarget, bGather ? CECHPWorkTrace::TRACE_GATHER : CECHPWorkTrace::TRACE_PICKUP);
		m_pWorkMan->StartWork_p1(pWork);
	}
}

//	Check whether host can gather specified matter
bool CECHostPlayer::CanGatherMatter(CECMatter* pMatter)
{
	if (!pMatter || !pMatter->IsMine())
		return false;

	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(pMatter->GetTemplateID(), ID_SPACE_ESSENCE, DataType);

	if (DataType != DT_MINE_ESSENCE)
	{
		ASSERT(DataType == DT_MINE_ESSENCE);
		return false;
	}
	
	const MINE_ESSENCE* pData = (const MINE_ESSENCE*)pDataPtr;
	if (!pData->task_in || m_pTaskInterface->HasTask(pData->task_in))
		return true;

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
*/
/*	Attackable judge. Check whether we can attack an object.

	Return value:

		-1: target has missed, or target id is invalid, or target is dead
		0: target isn't attackable
		1: target is attackable
*/
/*int CECHostPlayer::AttackableJudge(int idTarget, bool bForceAttack)
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
	if (gr.get_state() == game_record::enum_gr_replay)
		return true;

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

		if (IsDead() || !ISPLAYERID(m_idSelTarget) || m_idSelTarget == m_PlayerInfo.cid ||
			!m_pTeam || !m_pTeam->GetMemberByID(m_idSelTarget) || m_iBuddyId || IsProducing() || m_bMultiRideMember)
			bRet = false;

		break;

	case CANDO_FLY:

		if (IsDead() || IsRooting() || IsSitting() || IsTrading() || IsReviving() || 
			IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() || GetBoothState() != 0 ||
			IsFlashMoving() || m_pWorkMan->GetWork_p2() || m_bHangerOn || IsOperatingPet() ||
			IsUsingItem() || IsProducing() || m_bMultiRideMember || m_pChangedModel ||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

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
		
		if (IsDead() || IsAboutToDie() || IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || GetBoothState() != 0 || IsTakingOff() || m_pWorkMan->GetWork_p2() ||
			m_iBuddyId || IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
		
	case CANDO_BINDBUDDY:
		
		if (IsDead() || IsAboutToDie() || IsJumping() || IsSitting() || 
			IsMeleeing() || IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || IsReviving() || IsSpellingMagic() || IsPicking() ||
			IsGathering() || IsRooting() ||	GetBoothState() != 0 ||
			m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND || m_iBuddyId ||
			IsOperatingPet() || IsUsingItem() || IsProducing() || IsShapeChanged() 
			|| m_bMultiRideMember || m_bMultiRideLeader||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;
		
	case CANDO_DUEL:

		if (IsDead() || IsAboutToDie() ||IsSitting() || IsFighting() || IsTrading() || 
			IsReviving() || IsUsingTrashBox() || IsTalkingWithNPC() || IsChangingFace() ||
			GetBoothState() != 0 || m_iBuddyId || m_pvp.iDuelState != DUEL_ST_NONE ||
			IsOperatingPet() || IsUsingItem() || IsProducing() || m_bMultiRideMember||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;

		break;

	case CANDO_BINDMULTIBUDDY:

		if (IsDead() || IsAboutToDie() || IsJumping() || IsSitting() || 
			IsMeleeing() || IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || IsReviving() || IsSpellingMagic() || IsPicking() ||
			IsGathering() || IsRooting() ||	GetBoothState() != 0 ||
			m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND || m_iBuddyId ||
			IsOperatingPet() || IsUsingItem() || IsProducing() || IsShapeChanged() 
			|| m_bMultiRideMember || !m_bMultiRideLeader||
			(m_pTransfiguredModel && m_TransfigureProp.iMoveType != TRANSFIGURE_MOVE_FREE))
			bRet = false;
		break;

	case CANDO_BODYCHANGE:

		if (IsDead() || GetBoothState() != 0 || IsSitting() || 
			IsTrading() || IsUsingTrashBox() || IsTalkingWithNPC() ||
			IsChangingFace() || IsReviving() || IsSpellingMagic() || IsPicking() ||
			IsGathering() || IsRooting() ||	IsFlying() || IsOperating() || m_iBuddyId ||
			IsOperatingPet() || IsUsingItem() || IsProducing() || 
			m_bMultiRideLeader || m_bMultiRideMember || IsRidingOnPet())

			bRet = false;
		break;
	}

	return bRet;
}

bool CECHostPlayer::IsPlayerMoving()
{
	return m_pWorkMan->GetCurWork() && m_pWorkMan->GetCurWork()->IsMoving();
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
*/
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
//int CECHostPlayer::GetProduceCoolTime(int iIndex, int* piMax/* NULL */)
/*{
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
*/
//	Get time counter of using item in pack
//bool CECHostPlayer::GetUsingItemTimeCnt(DWORD& dwCurTime, DWORD& dwMaxTime, int* piItem/* NULL */)
/*{
	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_USEITEM)
		return false;

	CECHPWorkUse* pWork = (CECHPWorkUse*)m_pWorkMan->GetCurWork();
	CECCounter& cnt = pWork->GetTimeCounter();
	dwCurTime = cnt.GetCounter();
	dwMaxTime = cnt.GetPeriod();

	if (piItem)
		*piItem = pWork->GetItem();

	return true;
}

//	Fill NPC packs
void CECHostPlayer::FillNPCPack(int iIndex, const ACHAR* szName, int* aItems, 
								float fPriceScale, bool bRecipe)
{
	CECNPCInventory* pPack = m_aNPCPacks[iIndex];
	pPack->RemoveAllItems();
	pPack->SetName(szName);

	ID_SPACE idSpace = bRecipe ? ID_SPACE_RECIPE : ID_SPACE_ESSENCE;
	int iScaleType = bRecipe ? CECIvtrItem::SCALE_MAKE : CECIvtrItem::SCALE_BUY;

	for (int i=0; i < IVTRSIZE_NPCPACK; i++)
	{
		int idItem = aItems[i];
		if (idItem)
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, 0, 1, idSpace);
			if (pItem->GetClassID() != CECIvtrItem::ICID_RECIPE)
			{
				pItem->GetDetailDataFromLocal();
				pItem->SetPriceScale(iScaleType, fPriceScale);
			}

			pPack->SetItem(i, pItem);
		}
	}
}

//	Set absolute position
void CECHostPlayer::SetPos(const A3DVECTOR3& vPos)
{
	A3DVECTOR3 _pos = vPos;

	if (gr.get_state() == game_record::enum_gr_replay && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
		_pos = gr.get_host_pos();

	CECPlayer::SetPos(_pos);

	if (gr.get_state() == game_record::enum_gr_record && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		frame_record* fr = gr.get_cur_frame();

		if (fr)
			fr->set_host_pos(_pos);
	}

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

	if (gr.get_state() == game_record::enum_gr_replay && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		A3DVECTOR3 _dir = gr.get_host_dir();
		A3DVECTOR3 right = Normalize(CrossProduct(vUp, _dir));
		_dir = Normalize(CrossProduct(right, vUp));
		CECPlayer::SetDirAndUp(_dir, vUp);
		return;
	}

	CECPlayer::SetDirAndUp(vDir, vUp);

	if (gr.get_state() == game_record::enum_gr_record && g_pGame->GetGameRun()->GetGameState() == CECGameRun::GS_GAME)
	{
		frame_record* fr = gr.get_cur_frame();

		if (fr)
			fr->set_host_dir(vDir);
	}
}

void CECHostPlayer::Killed(int idKiller)
{
	if( m_pPlayerModel )
		m_pPlayerModel->ClearComActFlag(true);

	m_dwStates |= GP_STATE_CORPSE;

	CECHPWorkDead* pWork = (CECHPWorkDead*)m_pWorkMan->CreateWork(CECHPWork::WORK_DEAD);
	m_pWorkMan->StartWork_p1(pWork);

	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECGameUIMan* pGameUI = pGameRun->GetUIManager()->GetInGameUIMan();
	pGameUI->PopupReviveDialog(true, m_iReviveCountDown);
	m_iReviveCountDown = 0;
	SetUseGroundNormal(true);
	
	LuaEvent::FirstDie();
	//  if host in the arena, do not print the death notify. 
	if(m_iBattleType == BATTLE_TYPE_ARENA || m_iBattleType == BATTLE_TYPE_ARENAREBORN || m_iBattleType == BATTLE_TYPE_CRSSVR)
		return;
	//  if host in the anonymous map, do not print the death notify.
	if (m_bAnonymous)
		return;
	//	Print a notify
	if (ISNPCID(idKiller))
	{
		CECNPC* pKiller = pGameRun->GetWorld()->GetNPCMan()->GetNPC(idKiller);
		if (pKiller)
			pGameRun->AddFixedMessage(FIXMSG_BEKILLED, pKiller->GetName());
	}
	else if (ISPLAYERID(idKiller))
	{
		pGameRun->AddFixedMessage(FIXMSG_BEKILLED, pGameRun->GetPlayerName(idKiller, true));
	}
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

	if (idTool)
	{
		int iIndex = m_pPack->FindItem(idTool);
		if (iIndex >= 0)
		{
			*piPack	 = IVTRTYPE_PACK;
			*piIndex = iIndex;
			*pidTool = idTool;
		}
		else if ((iIndex = m_pTaskPack->FindItem(idTool)) >= 0)
		{
			*piPack	 = IVTRTYPE_TASKPACK;
			*piIndex = iIndex;
			*pidTool = idTool;
		}
		else
			bRet = false;
	}
	else
	{
		*piPack	 = 0;
		*piIndex = 0;
		*pidTool = 0;
	}

	return bRet;
}

//	When host is going to move, this function will be called
void CECHostPlayer::GoingMove()
{
	if (DoingSessionPose())
		g_pGame->GetGameSession()->c2s_CmdCancelAction();
}

//	Update positions of team member
void CECHostPlayer::UpdateTeamMemberPos(DWORD dwDeltaTime)
{
	if (!m_pTeam)
		return;

	m_TLPosCnt.IncCounter(dwDeltaTime);
	m_TMPosCnt.IncCounter(dwDeltaTime);

	int aMemIDs[32], iMemCnt=0;
	int idCurInst = g_pGame->GetGameRun()->GetWorld()->GetInstanceID();
	
	//	Update team leader's position
	if (m_pTeam->GetLeaderID() != m_PlayerInfo.cid)
	{
		//	Team leader should be the first member if all member data is ready
		CECTeamMember* pLeader = m_pTeam->GetLeader();
		if (pLeader)
		{
			CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(m_pTeam->GetLeaderID());
			if (pPlayer && !pPlayer->InGhostState())
			{
				pLeader->SetPos(idCurInst, pPlayer->GetPos());
			}
			else if (m_TLPosCnt.IsFull())
			{
				m_TLPosCnt.Reset();
				aMemIDs[iMemCnt++] = pLeader->GetCharacterID();
			}
		}
	}

	//	Update team members' positions
	int i, iNumMem = m_pTeam->GetMemberNum();
	for (i=0; i < iNumMem; i++)
	{
		CECTeamMember* pMem = m_pTeam->GetMemberByIndex(i);
		int cid = pMem->GetCharacterID();
		if (cid == m_pTeam->GetLeaderID() || cid == m_PlayerInfo.cid)
			continue;

		CECElsePlayer* pPlayer = m_pPlayerMan->GetElsePlayer(cid);
		if (pPlayer && !pPlayer->InGhostState())
			pMem->SetPos(idCurInst, pPlayer->GetPos());
		else if (m_TMPosCnt.IsFull())
			aMemIDs[iMemCnt++] = cid;
	}

	if (m_TMPosCnt.IsFull())
		m_TMPosCnt.Reset();
	
	if (iMemCnt)
	{
		//	Try to get teammates position from server. 
		g_pGame->GetGameSession()->c2s_CmdTeamMemberPos(iMemCnt, aMemIDs);
	}
}

//	Freeze / Unfreeze specified item
bool CECHostPlayer::FreezeItem(int iIvtr, int iIndex, bool bFreeze, bool bFreezeByNet)
{
	CECInventory* pPack = GetPack(iIvtr);
	if (!pPack)
		return false;

	CECIvtrItem* pItem = pPack->GetItem(iIndex);
	if (!pItem)
		return false;

	if (bFreezeByNet)
		pItem->NetFreeze(bFreeze);
	else
		pItem->Freeze(bFreeze);

	return true;
}

//	Check whether player pickup specified matter
bool CECHostPlayer::CanTakeItem(int idItem, int iAmount)
{
	bool bCanPick = false;

	const SPECIAL_ID_CONFIG& ids = g_pGame->GetGameRun()->GetSpecialIDConfig();
	if (idItem == ids.id_money_matter)
	{
		if (GetMoneyAmount() < GetMaxMoneyAmount())
			bCanPick = true;
	}
	else
	{
		if (m_pPack->CanAddOneItem(idItem))
			bCanPick = true;
	}

	return bCanPick;
}

//	Get force attack flag
bool CECHostPlayer::GetForceAttackFlag(const DWORD* pdwParam)
{
	bool bForceAttack = false;
	CECInputCtrl* pInputCtrl = g_pGame->GetGameRun()->GetInputCtrl();
	
	if (pdwParam)
		bForceAttack = pInputCtrl->IsCtrlPressed(*pdwParam);
	else
		bForceAttack = pInputCtrl->KeyIsBeingPressed(VK_CONTROL);

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
	for (int i=0; i < m_aTeamInvs.GetSize(); i++)
	{
		const TEAMINV& Inv = m_aTeamInvs[i];
		if (Inv.idLeader == Info.roleid)
		{
			CECGameUIMan* pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
			ACString strName = ACString((const ACHAR*)Info.name.begin(), Info.name.size() / sizeof (ACHAR));
			pGameUI->PopupInviteGroupMenu(Inv.idLeader, strName, Inv.seq, Inv.wPickFlag);
			
			//	Remove this invivation from array
			m_aTeamInvs.RemoveAtQuickly(i);
			return;	
		}
	}
}

//	Rearrange booth packages
void CECHostPlayer::RearrangeBoothPacks()
{
	struct INFO
	{
		int	iOrigin;
		int iAmount;
		int iUnitPrice;
	};

	INFO aSellItems[IVTRSIZE_BOOTHSPACK];
	INFO aBuyItems[IVTRSIZE_BOOTHBPACK];
	memset(aSellItems, 0, sizeof (aSellItems));
	memset(aBuyItems, 0, sizeof (aBuyItems));

	int i, iCnt=0;

	//	Rearrange booth sell package
	for (i=0; i < m_pBoothSPack->GetSize(); i++)
	{
		CECIvtrItem* pItem1 = m_pBoothSPack->GetItem(i);
		if (!pItem1)
			continue;

		const CECDealInventory::ITEMINFO& ti = m_pBoothSPack->GetItemInfo(i);
		CECIvtrItem* pItem2 = m_pPack->GetItem(ti.iOrigin);

		if (!pItem2 || pItem2->GetTemplateID() != pItem1->GetTemplateID())
			continue;

		//	Save item's information
		INFO& info = aSellItems[iCnt++];
		info.iAmount	= a_Min(ti.iAmount, pItem2->GetCount());
		info.iOrigin	= ti.iOrigin;
		info.iUnitPrice	= pItem1->GetUnitPrice();
	}

	m_pBoothSPack->RemoveAllItems();
	for (i=0; i < iCnt; i++)
	{
		const INFO& info = aSellItems[i];
		CECIvtrItem* pItem = m_pPack->GetItem(info.iOrigin);
		m_pBoothSPack->AddBoothItem(pItem, info.iOrigin, info.iAmount, info.iUnitPrice);
		pItem->Freeze(true);
	}

	//	Rearrange booth buy package
	iCnt = 0;
	for (i=0; i < m_pBoothBPack->GetSize(); i++)
	{
		CECIvtrItem* pItem1 = m_pBoothBPack->GetItem(i);
		if (!pItem1)
			continue;

		const CECDealInventory::ITEMINFO& ti = m_pBoothBPack->GetItemInfo(i);
		CECIvtrItem* pItem2 = m_pPack->GetItem(ti.iOrigin);

		if (!pItem2 || pItem2->GetTemplateID() != pItem1->GetTemplateID())
			continue;

		//	Save item's information
		INFO& info = aBuyItems[iCnt++];
		info.iAmount	= ti.iAmount;
		info.iOrigin	= ti.iOrigin;
		info.iUnitPrice	= pItem1->GetUnitPrice();
	}
	
	m_pBoothBPack->RemoveAllItems();
	for (i=0; i < iCnt; i++)
	{
		const INFO& info = aBuyItems[i];
		CECIvtrItem* pItem = m_pPack->GetItem(info.iOrigin);
		m_pBoothBPack->AddBoothItem(pItem, info.iOrigin, info.iAmount, info.iUnitPrice);
		pItem->Freeze(true);
	}
}

//	Get deadly strike rate (%)
int CECHostPlayer::GetDeadlyStrikeRate()
{
	float fVal = 0;

	//	Get basic rate from database
	DATA_TYPE DataType;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();

	unsigned int id = pDataMan->get_first_data_id(ID_SPACE_CONFIG, DataType);
	while (id)
	{
		if (DataType == DT_CHARRACTER_CLASS_CONFIG)
		{
			const CHARRACTER_CLASS_CONFIG* pData = (const CHARRACTER_CLASS_CONFIG*)
				pDataMan->get_data_ptr(id, ID_SPACE_CONFIG, DataType);

			if ((int)pData->character_class_id == m_BasicProps.iProfession)
			{
				fVal += pData->crit_rate;
				break;
			}
		}

		id = pDataMan->get_next_data_id(ID_SPACE_CONFIG, DataType);
	}

	// fVal += m_ExtProps.bs.agility * 100 / 2000.0f + 0.001f;

	for (int i=0; i < m_pEquipPack->GetSize(); i++)
	{
		CECIvtrItem* pItem = (CECIvtrItem*)m_pEquipPack->GetItem(i);
		if (pItem && pItem->IsEquipment())
			fVal += ((CECIvtrEquip*)pItem)->GetDeadlyStrikeRate();
	}

	a_ClampRoof(fVal, 95.0f);

	return (int)fVal;
}

//	Is host in sliding state (in the state, host is sliding on slope) ?
bool CECHostPlayer::InSlidingState()
{
	if (m_iMoveMode != MOVE_SLIDE)
		return false;

	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_STAND)
	{
		CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();
		if (pWork && pWork->GetStopSlideFlag())
			return false;
	}

	return true;
}
*/
/*
//	On start binding buddy
void CECHostPlayer::OnStartBinding(int idMule, int idRider)
{
	if (m_iChangeShape)
		return;

	CECPlayer::OnStartBinding(idMule, idRider);

	//	Stop current work and restore to stand state
	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND)
		m_pWorkMan->FinishAllWork(true);

	//	If host is rider, goto be bound state
	if (idRider == GetCharacterID())
	{
		// 将对方加入到临时好友中
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(idMule, false);
		m_pFriendMan->AddTempFriend(idMule, pName);
		
		ShowWing(false);
	//	m_pCDRInfo->vTPNormal.Clear();
		ResetCDRInfo();

		CECHPWorkBeBound* pWork = (CECHPWorkBeBound*)m_pWorkMan->CreateWork(CECHPWork::WORK_BEBOUND);
		m_pWorkMan->StartWork_p1(pWork);
	}
	else
	{
		// 将对方加入到临时好友中
		const ACHAR* pName = g_pGame->GetGameRun()->GetPlayerName(idRider, false);
		m_pFriendMan->AddTempFriend(idRider, pName);	
	}
}

//	Apply combo skill
bool CECHostPlayer::ApplyComboSkill(int iGroup)
{
	// first of all see if we need to cancel sitdown work.
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_SIT)
	{
		g_pGame->GetGameSession()->c2s_CmdStandUp();
		return false;
	}

	//	Clear current combo skill
	ClearComboSkill();

	if (!(m_pComboSkill = new CECComboSkill))
		return false;

	bool bForceAttack = GetForceAttackFlag(NULL);
	if (!(m_pComboSkill->Init(this, iGroup, m_idSelTarget, bForceAttack)))
		return false;

	//	Start skill group
	m_pComboSkill->Continue(m_bMelee);

	return true;
}

//	Clear combo skill
void CECHostPlayer::ClearComboSkill()
{
	if (m_pComboSkill)
	{
		delete m_pComboSkill;
		m_pComboSkill = NULL;
	}
}

//	Replace specified skill with it's senior skill
void CECHostPlayer::ReplaceJuniorSkill(int idJuniorSkill, CECSkill* pSeniorSkill)
{
	if (!pSeniorSkill || !pSeniorSkill->GetJunior())
	{
		ASSERT(pSeniorSkill);
		return;
	}

	int i, idSenior = pSeniorSkill->GetSkillID();

	//	Update shortcuts ...
	for (i=0; i < NUM_HOSTSCSETS1; i++)
	{
		if (m_aSCSets1[i])
			m_aSCSets1[i]->ReplaceSkillID(idJuniorSkill, pSeniorSkill);
	}

	for (i=0; i < NUM_HOSTSCSETS2; i++)
	{
		if (m_aSCSets2[i])
			m_aSCSets2[i]->ReplaceSkillID(idJuniorSkill, pSeniorSkill);
	}
	
	//	Update skill groups ...
	EC_VIDEO_SETTING vs = g_pGame->GetConfigs()->GetVideoSettings();
	for (i=0; i < EC_COMBOSKILL_NUM; i++)
	{
		bool bChange = false;

		for (int j=0; j < EC_COMBOSKILL_NUM; j++)
		{
			if (!vs.comboSkill[j].nIcon)
				continue;

			for (int n=0; n < EC_COMBOSKILL_LEN; n++)
			{
				if (vs.comboSkill[j].idSkill[n] == idJuniorSkill)
				{
					vs.comboSkill[j].idSkill[n] = idSenior;
					bChange = true;
				}
			}
		}

		if (bChange)
			g_pGame->GetConfigs()->SetVideoSettings(vs);
	}

	if (m_pComboSkill)
		m_pComboSkill->ReplaceSkillID(idJuniorSkill, idSenior);

	//	Note: Replace skills in m_aPtSkills and m_aPsSkills at last !!!
	//	Check positiive skills
	for (i=0; i < m_aPtSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPtSkills[i];
		if (pSkill->GetSkillID() == idJuniorSkill)
			m_aPtSkills[i] = pSeniorSkill;
	}
	
	//	Check passive skills
	for (i=0; i < m_aPsSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aPsSkills[i];
		if (pSkill->GetSkillID() == idJuniorSkill)
			m_aPsSkills[i] = pSeniorSkill;
	}

	for (i=0; i < m_aTsSkills.GetSize(); i++)
	{
		CECSkill* pSkill = m_aTsSkills[i];
		if (pSkill->GetSkillID() == idJuniorSkill)
			m_aTsSkills[i] = pSeniorSkill;
	}

	for (i=0; i<m_aImSkills.GetSize(); i++ )
	{
		CECSkill* pSkill = m_aImSkills[i];
		if (pSkill->GetSkillID() == idJuniorSkill)
			m_aImSkills[i] = pSeniorSkill;
	}
}

CECSkill* CECHostPlayer::InsertAddonSkill(int idSkill, int lev)
{
	CECSkill* pSkill = new CECSkill(idSkill, lev);

	if (pSkill->GetType() != CECSkill::TYPE_PASSIVE &&
		pSkill->GetType() != CECSkill::TYPE_LIVE)
		m_aPtSkills.Add(pSkill);
	else
		m_aPsSkills.Add(pSkill);

	GNET::ElementSkill::SetLevel(idSkill, lev);
	// 重置附加技能的冷却
	abase::hash_map<int, int>::iterator it = m_TsSkillCoolDown.find(idSkill);
	if (it != m_TsSkillCoolDown.end())
		pSkill->StartCooling(0, it->second);

	return pSkill;
}

//	Do emote action
bool CECHostPlayer::DoEmote(int idEmote)
{
	if (m_pWorkMan->GetCurWorkID() != CECHPWork::WORK_STAND)
		return false;

	CECHPWorkStand* pWork = (CECHPWorkStand*)m_pWorkMan->GetCurWork();

	int iAction = ACT_STAND;
	bool bSession = false;

	//	Select action according to pose
	switch (idEmote)
	{
	case ROLEEXP_WAVE:		iAction = ACT_EXP_WAVE;			break;
	case ROLEEXP_NOD:		iAction = ACT_EXP_NOD;			break;
	case ROLEEXP_SHAKEHEAD:	iAction = ACT_EXP_SHAKEHEAD;	break;
	case ROLEEXP_SHRUG:		iAction = ACT_EXP_SHRUG;		break;
	case ROLEEXP_LAUGH:		iAction = ACT_EXP_LAUGH;		break;
	case ROLEEXP_ANGRY:		iAction = ACT_EXP_ANGRY;		break;
	case ROLEEXP_STUN:		iAction = ACT_EXP_STUN;			break;
	case ROLEEXP_DEPRESSED:	iAction = ACT_EXP_DEPRESSED;	break;
	case ROLEEXP_KISSHAND:	iAction = ACT_EXP_KISSHAND;		break;
	case ROLEEXP_SHY:		iAction = ACT_EXP_SHY;			break;
	case ROLEEXP_SALUTE:	iAction = ACT_EXP_SALUTE;		break;

	case ROLEEXP_SITDOWN:	
		
		iAction = ACT_EXP_SITDOWN;	
		bSession = true;
		break;

	case ROLEEXP_ASSAULT:	iAction = ACT_EXP_ASSAULT;		break;
	case ROLEEXP_THINK:		iAction = ACT_EXP_THINK;		break;
	case ROLEEXP_DEFIANCE:	iAction = ACT_EXP_DEFIANCE;		break;
	case ROLEEXP_VICTORY:	iAction = ACT_EXP_VICTORY;		break;
	case ROLEEXP_GAPE:		iAction = ACT_GAPE;				break;
	case ROLEEXP_KISS:		iAction = ACT_EXP_KISS;			break;
	case ROLEEXP_FIGHT:		iAction = ACT_EXP_FIGHT;		break;
	case ROLEEXP_ATTACK1:	iAction = ACT_EXP_ATTACK1;		break;
	case ROLEEXP_ATTACK2:	iAction = ACT_EXP_ATTACK2;		break;
	case ROLEEXP_ATTACK3:	iAction = ACT_EXP_ATTACK3;		break;
	case ROLEEXP_ATTACK4:	iAction = ACT_EXP_ATTACK4;		break;
	case ROLEEXP_DEFENCE:	iAction = ACT_EXP_DEFENCE;		break;	
	case ROLEEXP_FALL:		iAction = ACT_EXP_FALL;			break;
	case ROLEEXP_FALLONGROUND:	iAction = ACT_EXP_FALLONGROUND;	break;
	case ROLEEXP_LOOKAROUND:	iAction = ACT_EXP_LOOKAROUND;	break;	
	case ROLEEXP_DANCE:		iAction = ACT_EXP_DANCE;		break;
	case ROLEEXP_TWO_KISS:	iAction = ACT_TWO_KISS;			break;
	case ROLEEXP_FIREWORK:	iAction = ACT_ATTACK_TOSS;	break;
	default:
		ASSERT(0);
		break;
	}

	pWork->SetPoseAction(iAction, bSession); 

	return true;
}

void CECHostPlayer::StartGeneralOprt(int op_id, int duration)
{
	CECHPWorkGeneralOprt* pWork = (CECHPWorkGeneralOprt*)m_pWorkMan->CreateWork(CECHPWork::WORK_GENERAL_OPRT);

	if (pWork)
		m_pWorkMan->StartWork_p1(pWork);

	//	Start time counter
	m_GatherCnt.SetPeriod(duration * 50);
	m_GatherCnt.Reset();
}

void CECHostPlayer::StopGeneralOprt(int op_id)
{
	m_pWorkMan->FinishCurWork(CECHPWork::WORK_GENERAL_OPRT, false);
}

//	Hatch pet
bool CECHostPlayer::HatchPet(int iIvtrIdx)
{*/
/*	CECIvtrItem* pItem = m_pPack->GetItem(iIvtrIdx);
	if (!pItem || pItem->GetClassID() != CECIvtrItem::ICID_PETEGG)
		return false;

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	CECIvtrPetEgg* pEgg = (CECIvtrPetEgg*)pItem;
	const IVTR_ESSENCE_PETEGG& e = pEgg->GetEssence();
	
	//	Check profession
	if (!(e.req_class & (1 << m_iProfession)))
	{
		pGameRun->AddFixedMessage(FIXMSG_WRONGPROF);
		return false;
	}

	//	Check level
	int iLevelReq = a_Max((int)e.level, e.req_level);
	if (m_BasicProps.iLevel < iLevelReq)
	{
		pGameRun->AddFixedMessage(FIXMSG_LEVELTOOLOW);
		return false;
	}

	//	Check money
	if (GetMoneyAmount() < pEgg->GetDBEssence()->money_hatched)
	{
		pGameRun->AddFixedMessage(FIXMSG_NEEDMONEY);
		return false;
	}

	//	Check whether pet corral is full
	if (!m_pPetCorral->GetEmptySlotNum())
	{
		pGameRun->AddFixedMessage(FIXMSG_PET_CORRALFULL);
		return false;
	}

	g_pGame->GetGameSession()->c2s_CmdNPCSevHatchPet(iIvtrIdx, pItem->GetTemplateID());
*/
//	return true;
//}

//	Restore pet
//bool CECHostPlayer::RestorePet(int iPetIdx)
//{
/*	CECPetData* pPet = m_pPetCorral->GetPetData(iPetIdx);
	if (!pPet || iPetIdx == m_pPetCorral->GetActivePetIndex())
		return false;

	CECGameRun* pGameRun = g_pGame->GetGameRun();

	//	Check money
	DATA_TYPE DataType;
	PET_EGG_ESSENCE* pdbe = (PET_EGG_ESSENCE*)g_pGame->GetElementDataMan()->get_data_ptr(pPet->GetEggID(), ID_SPACE_ESSENCE, DataType);
	if (DataType != DT_PET_EGG_ESSENCE || !pdbe)
		return false;

	if (GetMoneyAmount() < pdbe->money_restored)
	{
		pGameRun->AddFixedMessage(FIXMSG_NEEDMONEY);
		return false;
	}

	//	Check package space
	if (!m_pPack->GetEmptySlotNum())
	{
		pGameRun->AddFixedMessage(FIXMSG_PACKISFULL);
		return false;
	}

	g_pGame->GetGameSession()->c2s_CmdNPCSevRestorePet(iPetIdx);
*/
//	return true;
//}

//	Summon pet
//bool CECHostPlayer::SummonPet(int iPetIdx)
//{
/*	if (!CanDo(CANDO_SUMMONPET))
		return false;

	//	If host could't stop naturally, cancel summoning
	if (!NaturallyStopMoving())
		return false;

	CECPetData* pPet = m_pPetCorral->GetPetData(iPetIdx);
	if (!pPet)
		return false;

	g_pGame->GetGameSession()->c2s_CmdPetSummon(iPetIdx);
*/
//	return true;
//}

/*
//	Recall pet
bool CECHostPlayer::RecallPet()
{
	//	If host could't stop naturally, cancel recalling
	if (!NaturallyStopMoving())
		return false;

//	g_pGame->GetGameSession()->c2s_CmdPetRecall();
	return true;
}

//	Banish pet
bool CECHostPlayer::BanishPet(int iPetIdx)
{
	CECPetData* pPet = m_pPetCorral->GetPetData(iPetIdx);
	if (!pPet)
		return false;

//	g_pGame->GetGameSession()->c2s_CmdPetBanish(iPetIdx);
	return true;
}

//	Get battle pet index
int CECHostPlayer::GetBattlePetIndex()
{
	for(int i = 0; i < m_pPetPack->GetSize(); i++)
	{
		CECPetBedge *pPet = (CECPetBedge*)m_pPetPack->GetItem(i);
		if( pPet && pPet->GetEssence().main_status == 0 )
			return i;
	}
	return -1;
}
*/
/*	Is host operating pet ?
	return value:

	0: host doesn't operating pet.
	1: host is summoning pet.
	2: host is recalling pet.
*/
/*
int CECHostPlayer::IsOperatingPet()
{
	if (m_pWorkMan->GetCurWorkID() == CECHPWork::WORK_CONCENTRATE)
	{
		CECHPWorkConcentrate* pWork = (CECHPWorkConcentrate*)m_pWorkMan->GetCurWork();
		if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_SUMMONPET)
			return 1;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_RECALLPET)
			return 2;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_COMBINE1)
			return 3;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_UNCOMBINE1)
			return 4;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_COMBINE2)
			return 5;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_UNCOMBINE2)
			return 6;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_FREE)
			return 7;		
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_ADOPT)
			return 8;
		else if (pWork->GetDoWhat() == CECHPWorkConcentrate::DO_REFINE)
			return 9;
	}

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
*/
//	Get number of equipped items of specified suite
int CECHostPlayer::GetEquippedSuiteItem(int idSuite, int* aItems/* NULL */)
{
	int i, iItemCnt = 0;

	for (i=0; i < m_pEquipPack->GetSize(); i++)
	{
		CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pEquipPack->GetItem(i);
		if (!pEquip || pEquip->GetSuiteID() != idSuite)
			continue;

//		if (!CanUseEquipment(pEquip))
//			continue;

		if (aItems)
			aItems[iItemCnt] = pEquip->GetTemplateID();
		
		iItemCnt++;
	}

	return iItemCnt;
}
/*
//	Search the full suite
int CECHostPlayer::SearchFullSuite()
{
	DATA_TYPE DataType = DT_INVALID;
	elementdataman* pDataMan = g_pGame->GetElementDataMan();
	
	int i;
	typedef abase::hashtab<int, int, abase::_hash_function> SuiteTable;
	SuiteTable SuiteTab(32);

	//	Check how many suits of equipment host have dressed
	for (i=0; i < m_pEquipPack->GetSize(); i++)
	{
		CECIvtrEquip* pEquip = (CECIvtrEquip*)m_pEquipPack->GetItem(i);
		if (!pEquip)
			continue;
		
		int idSuite = pEquip->GetSuiteID();
		if (!idSuite)
			continue;

		const void* pData = pDataMan->get_data_ptr(idSuite, ID_SPACE_ESSENCE, DataType);
		if (DataType != DT_SUITE_ESSENCE)
			continue;

		const SUITE_ESSENCE* pSuiteEss = (const SUITE_ESSENCE*)pData;
		if (pSuiteEss->file_gfx[0])
			SuiteTab.put(idSuite, pSuiteEss->max_equips);
	}

	if (!SuiteTab.size())
		return 0;	//	No suite was found

	//	Check whether suite is full
	SuiteTable::iterator it = SuiteTab.begin();
	for (; it != SuiteTab.end(); ++it)
	{
		int idSuite = *it.key();
		int iMaxNum = *it.value();

		if (iMaxNum == GetEquippedSuiteItem(idSuite))
			return idSuite;
	}

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
	else
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
	using namespace S2C;
	m_aIconStates.RemoveAll(false);

	if (cmd->count)
	{
		m_aIconStates.SetSize(cmd->count, 10);

		for (int i=0; i < cmd->count; i++)
		{
			BUFFINFO& bi = m_aIconStates[i];
			bi.id		= cmd->buff_list[i].state;
			bi.iLevel	= cmd->buff_list[i].level;
			bi.iEndTime	= cmd->buff_list[i].buff_endtime;
		}
	}
}

void CECHostPlayer::ClearApoiseSkill()
{
	if (m_ApoiseSkill.idSkill)
	{
		m_ApoiseSkill.idSkill = 0;

		//	Unlock item
		if (m_ApoiseSkill.iItemSlot >= 0 && m_ApoiseSkill.tidItem)
		{
			CECIvtrItem* pItem = GetPack()->GetItem(m_ApoiseSkill.iItemSlot);
			if (pItem)
				pItem->Freeze(false);
		}

		m_ApoiseSkill.iItemSlot	= -1;
		m_ApoiseSkill.tidItem	= 0;
	}	
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
	int i, j; 
	// 因为考虑到网络延迟的问题，所以必须做一些特殊的判断，比如判断该位置为空，或者该位置物品已经堆叠已满
	bool* pEmpty = new bool[m_pPocketPack->GetSize()];
	bool* pFull	 = new bool[m_pPocketPack->GetSize()];
	for(i =0; i< m_pPocketPack->GetSize(); ++i)
	{
		pEmpty[i] = false;
		pFull[i] = false;
		CECIvtrItem* pItem = m_pPocketPack->GetItem(i);
		if(!pItem)
			pEmpty[i] = true;
		if(pItem && pItem->GetPileLimit() == pItem->GetCount())
			pFull[i] = true;
	}

	for(i =0; i< m_pPack->GetSize(); ++i)
	{
		CECIvtrItem* pItem = m_pPack->GetItem(i);
		if(!pItem)
			continue;
		if(g_pGame->IsPack2PocketItem( pItem->GetTemplateID()) )
		{
			int tid = pItem->GetTemplateID();
			int nCount = pItem->GetCount();

			for(j =0; j< m_pPocketPack->GetSize(); ++j)
			{
				CECIvtrItem* pItem2 = m_pPocketPack->GetItem(j);
				if( pItem2 && pItem2->GetTemplateID() == tid && pItem2->GetPileLimit() >pItem2->GetCount() && !pFull[j])
				{
					int nMove = min(pItem2->GetPileLimit()- pItem2->GetCount(), nCount);
					g_pGame->GetGameSession()->c2s_CmdMoveIvtrToPocket(j, i, nMove);
					nCount -= nMove;
					pFull[j] = true;    // 有可能不满，但这里假定他已经满了
				}
			}
			if ( nCount == 0)
				continue;

			// 寻找一个空的位置，不能直接用SearchEmpty,
			int iSlot = -1;
			for(j =0; j< m_pPocketPack->GetSize(); ++j)
			{
				if( pEmpty[j])
				{
					pEmpty[j] = false;
					iSlot = j;
					break;
				}
			}
			if(iSlot >= 0)
			{
				g_pGame->GetGameSession()->c2s_CmdExgPocketIvtrItem(iSlot, i);
			}
			else
			{
				delete[] pEmpty;
				delete[] pFull;
				return;
			}
		}
	}
	delete[] pEmpty;
	delete[] pFull;
}

void CECHostPlayer::SetSelfReferenceCode(const void* p, int len)
{
	 _cp_str(m_strSelfReferenceCode, p, len);	
}

void CECHostPlayer::SetReferralCode(const void* p, int len)
{
	 _cp_str(m_strReferralCode, p, len);
}

const ACHAR* CECHostPlayer::GetSelfReferenceCode() const 
{ 
	if(m_strSelfReferenceCode.GetLength() == 0) 
	{
		static DWORD timeLast = 0;
		if( timeLast + 1000 < timeGetTime() )
		{
			g_pGame->GetGameSession()->ref_get_reference_code();
			timeLast = timeGetTime();
		}
		return NULL;
	}
	return m_strSelfReferenceCode; 
}

void CECHostPlayer::ReceiveBonusExp(double exp)
{
	m_BasicProps.exp += exp;

//	if ((int)exp > 0)
//	{
//		//	Print notify text
//	//	g_pGame->GetGameRun()->AddFixedMessage(pCmd->exp > 0 ? FIXMSG_GOTEXP : FIXMSG_LOSTEXP, (int)pCmd->exp);
//		BubbleText(BUBBLE_EXP, (int)exp);
// 	}
}

void CECHostPlayer::SetTransfigureProp(int id, int level, int exp_level)
{
	DATA_TYPE DataType;
	const void* pDataPtr = g_pGame->GetElementDataMan()->get_data_ptr(m_TransfigureInfo.id, ID_SPACE_CONFIG, DataType);
	if (DataType == DT_CHANGE_SHAPE_PROP_CONFIG)
	{
		const CHANGE_SHAPE_PROP_CONFIG* pData = (const CHANGE_SHAPE_PROP_CONFIG*)pDataPtr;
		if (pData->can_fight)
			m_TransfigureProp.bCanFight = true;
		if (pData->can_run_helper)
			m_TransfigureProp.bCanRunHelper = true;
		if (pData->shield_skill)
			m_TransfigureProp.bShieldSkill  = true;
		if (pData->can_use_medicine)
			m_TransfigureProp.bCanUseMedicine = true;

		m_TransfigureProp.iMoveType = pData->move_type;
		m_TransfigureProp.level = level;
		m_TransfigureProp.exp_level = exp_level;
	}
}

void CECHostPlayer::ClearTransfigureProp()
{
	memset(&m_TransfigureProp, 0, sizeof(m_TransfigureProp));
}

bool CECHostPlayer::IsAutoTeam(int id, int iFactionId, int iFamilyId, ACHAR *szName)
{
	int i, j;
	EC_AUTOTEAM_SETTING setting = g_pGame->GetConfigs()->GetAutoTeamSettings();
	CECFriendMan::GROUP *pGroup;
	ACString strName = szName;
	if (!setting.bUse)
	{
		return false;
	}
	if (setting.bAutoAll)
	{
		return true;
	}
	else
	{
		if (setting.bAutoFriend)
		{
			for( i = 0; i < m_pFriendMan->GetGroupNum(); i++ )
			{
				pGroup = m_pFriendMan->GetGroupByIndex(i);
				for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
				{
					int id1 = pGroup->aFriends[j]->id;
					if (id == pGroup->aFriends[j]->id)
					{
						return true;
					}
				}
			}
		}
		if (setting.bAutoGuild && 0!=iFactionId && iFactionId == m_idFaction)
		{
			return true;
		}
		else if (setting.bAutoFamily && 0!=iFamilyId && iFamilyId == m_idFamily)
		{
			return true;
		}
		if (strName.IsEmpty())
		{
			CECPlayerMan *pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
			CECPlayer *pPlayer = (CECPlayer *)pPlayerMan->GetPlayer(id);
			if (pPlayer)
			{
				strName = pPlayer->GetName();
			}
		}
		if (!strName.IsEmpty())
		{
			for (i=0; i<EC_AUTOTEAM_LEN; i++)
			{
				if (a_stricmp(strName, setting.szName[i]) == 0)
				{
					return true;
				}
			}
		}
	}
	return false;
}

int CECHostPlayer::GetRefineItemNum(int id, int level, int con)
{
	int num = 0;
	if (!m_pPack)
		return 0;

	for(int i =0; i< m_pPack->GetSize(); ++i)
	{
		CECIvtrItem* pItem = m_pPack->GetItem(i);
		if (pItem && pItem->IsEquipment() && pItem->GetTemplateID() == id)
		{
			CECIvtrEquip* pEquip = (CECIvtrEquip*)pItem;
			if (con == 0)
			{
				if (pEquip->GetEssence().num_estone >= level)
					num +=1;
			}
			else if (con == 1)
			{
				if (pEquip->GetEssence().num_estone <= level)
					num +=1;
			}
			else if (con == 2)
			{
				if(pEquip->GetEssence().num_estone == level)
					num +=1;
			}
		}
	}
	return num;
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
	ATaskTemplMan *pMan = g_pGame->GetTaskTemplateMan();
	ATaskTempl *pTemp = pMan->GetTaskTemplByID(idTask);
	if (pTemp->m_bScriptFinishTask)
		m_ScriptFinishTaskMap[idTask] = true; 
	ProcessChildTask(idTask); 
}

int CECHostPlayer::GetFriendNum()
{
	return m_pFriendMan->GetFriendNum();
}

bool CECHostPlayer::IsInSociety(int id, ACHAR* name)
{
	if (id==0 && name == NULL)
	{
		return false;
	}
	//夫妻、好友、师徒、家族成员、帮派成员
	int i, j;
	if (id>0)
	{
		if (id==GetSpouse())
		{
			return true;
		}
		
		for( i = 0; i < m_pFriendMan->GetGroupNum(); i++ )
		{
			CECFriendMan::GROUP *pGroup = m_pFriendMan->GetGroupByIndex(i);
			for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
			{
				if (id == pGroup->aFriends[j]->id)
				{
					return true;
				}
			}
		}*/
/*
		if (GetMaster() > 0)
		{
			PAUILISTBOX pLst = (PAUILISTBOX)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_TeacherManage")->GetDlgItem("List_Member");
			for (i=0; i<pLst->GetCount(); i++)
			{
				if (pLst->GetItemData(i)==(UINT)id)
				{
					return true;
				}
			}
		}
		
		FactionMemList &fml = g_pGame->GetFactionMan()->GetMemList();
		for( i = 0; i < (int)fml.size(); i++ )
		{
			if (id==fml[i]->GetID())
			{
				return true;
			}
		}
*//*	}
	else
	{
		if (g_pGame->GetGameRun()->GetPlayerName(GetSpouse(), false) && a_stricmp(name, g_pGame->GetGameRun()->GetPlayerName(GetSpouse(), false)) == 0)
		{
			return true;
		}
		
		for( i = 0; i < m_pFriendMan->GetGroupNum(); i++ )
		{
			CECFriendMan::GROUP *pGroup = m_pFriendMan->GetGroupByIndex(i);
			for( j = 0; j < pGroup->aFriends.GetSize(); j++ )
			{
				if (a_stricmp(name, pGroup->aFriends[j]->GetName())==0)
				{
					return true;
				}
			}
		}
*/	
/*		if (GetMaster() > 0)
		{
			PAUILISTBOX pLst = (PAUILISTBOX)g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_TeacherManage")->GetDlgItem("List_Member");
			for (i=0; i<pLst->GetCount(); i++)
			{
				if (g_pGame->GetGameRun()->GetPlayerName(pLst->GetItemData(i), false) && a_stricmp(name, g_pGame->GetGameRun()->GetPlayerName(pLst->GetItemData(i), false)) == 0)
				{
					return true;
				}
			}
		}
		
		FactionMemList &fml = g_pGame->GetFactionMan()->GetMemList();
		for( i = 0; i < (int)fml.size(); i++ )
		{
			if (a_stricmp(name, fml[i]->GetName()) == 0)
			{
				return true;
			}
		}
*//*	}
	return false;
}

unsigned short* CECHostPlayer::GetCombineSkillElems(int skill_id, int& num)
{
	return GNET::ElementSkill::GetComboSkillElems(skill_id, num);
}

void CECHostPlayer::UpdateCombineSkillElems(int skill_id, int* data, int num)
{
	GNET::ElementSkill::UpdateComboSkillElems(skill_id, data, num);
}

int CECHostPlayer::GetTerritoryScore()
{
	Faction_Info* pInfo = g_pGame->GetFactionMan()->GetFaction(m_idFaction);
	if (!pInfo)
	{
		return 0;
	}
	return pInfo->GetTerritoryScore();
}

int CECHostPlayer::GetCircleScore()
{
	Circle_Info* pInfo = g_pGame->GetCircleMan()->GetCircle(m_idCircle);
	if (!pInfo)
	{
		return 0;
	}
	return g_pGame->GetCircleMan()->GetCirclePoint();
}

*/