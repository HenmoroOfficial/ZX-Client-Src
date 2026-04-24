/*
 * FILE: EC_HostPlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/1
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Player.h"
#include "EC_CameraCtrl.h"
#include "EC_HostMove.h"
#include "EC_IvtrTypes.h"
#include "AArray.h"
#include "EC_Inventory.h"
#ifndef BACK_VERSION
#include "ElementRenderParam.h"
#endif

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

class A3DGFXEx;
class A2DSprite;
class AMSoundBuffer;
class CECObjectWork;
class CECInventory;
class CECDealInventory;
class CECNPCInventory;
class CECIvtrItem;
class CECIvtrEquip;
class CECShortcutSet;
class CECSkill;
class CECTaskInterface;
class CECHPWorkMan;
class CECFriendMan;
class CECMatter;
class CECIvtrArrow;
class CECPetCorral;
class CECComboSkill;
class CECLoginPlayer;
class CECCameraPath;
class CECCutscene;
class OnlineAwardManager;
class CECBattleDungeonTower;

struct CDR_INFO;
struct ON_AIR_CDR_INFO;

namespace GNET
{
	class PlayerBriefInfo;
	class Octets;
	class GamedataSend;
}

namespace S2C
{
	struct cmd_self_info_1;
	struct cmd_player_revive;
	struct self_buff_notify;
}

struct PosPair	//	To deal with precision problem caused by coordinate transformation 
{
	bool                bRecorded;
	A3DVECTOR3          vRelPos;
	A3DVECTOR3          vAbsPos;
    int                 iCarrierID;

	PosPair::PosPair() : bRecorded(false), vRelPos(0.0f), vAbsPos(0.0f), iCarrierID(0) {}
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHostPlayer
//	
///////////////////////////////////////////////////////////////////////////

class CECHostPlayer : public CECPlayer
{
public:		//	Types

	//	Move relative direction
	enum
	{
		MD_FORWARD	= 0x01,	
		MD_RIGHT	= 0x02,
		MD_BACK		= 0x04,
		MD_LEFT		= 0x08,
		MD_ABSUP	= 0x10,
		MD_ABSDOWN	= 0x20,
		MD_ALL		= 0x3f,
	};

	//	Turning camera flag
	enum
	{
		TURN_LBUTTON = 0x01,
		TURN_RBUTTON = 0x02,
	};

	//	Mask of some special extend states which will influence host game logic.
	//	Logic Influence Extned states
	enum
	{
		LIES_SLEEP		= 0x0001,
		LIES_STUN		= 0x0002,
		LIES_ROOT		= 0x0004,
		LIES_NOFGIHT	= 0x0008,

		LIES_DISABLEFIGHT	= 0x000B,

		LIES_DRAGGED_ROOT	= 0x10000,	//被牵引者的定身掩码 Added 2011-08-26.
		LIES_PULLED_ROOT	= 0x20000,	//被拉扯者的定身掩码 Added 2011-07-29.
		LIES_PULLING_ROOT	= 0x40000,	//释放拉扯技能者的定身掩码 Added 2011-07-29.
	};

	//	Behavior id used by CanDo()
	enum
	{
		CANDO_SITDOWN = 0,
		CANDO_MOVETO,
		CANDO_MELEE,
		CANDO_ASSISTSEL,
		CANDO_FLY,
		CANDO_PICKUP,
		CANDO_TRADE,
		CANDO_PLAYPOSE,
		CANDO_SPELLMAGIC,
		CANDO_USEITEM,
		CANDO_JUMP,
		CANDO_FOLLOW,
		CANDO_AUTOMOVE,
		CANDO_GATHER,
		CANDO_BOOTH,
		CANDO_FLASHMOVE,
		CANDO_BINDBUDDY,
		CANDO_DUEL,
		CANDO_SUMMONPET,
		CANDO_PRODUCE,
		CANDO_BINDMULTIBUDDY,
		CANDO_BODYCHANGE,
		CANDO_BATH,
		CANDO_QILINBUDDY,
		CANDO_PLAYACTIVEPOSE,
	};

	//	Move sound
	enum
	{
		MOVESND_WALK = 0,
		MOVESND_RUN,
		MOVESND_SWIM,
		MOVESND_FLOAT,
		MOVESND_JUMP,
		MOVESND_FALL,
		MOVESND_QUIET,
		MOVESND_TIGER_WALK,
		MOVESND_TIGER_RUN,
		MOVESND_PET_HORSE_WALK,
		MOVESND_PET_HORSE_RUN,
		MOVESND_PET_BEAR_WALK,
		MOVESND_PET_BEAR_RUN,
		MOVESND_PET_DINASAUR_WALK,
		MOVESND_PET_DINASAUR_RUN,
		MOVESND_PET_PUMA_WALK,
		MOVESND_PET_PUMA_RUN,
		MOVESND_PET_QILIN_WALK,
		MOVESND_PET_QILIN_RUN,
		NUM_MOVESND,
	};
	
	enum
	{
		FASHION_HOTKEY_NUM = 8,
	};
	
	enum
	{
		TABSEL_ALL = 0,
		TABSEL_MONSTER,
		TABSEL_PLAYER,
	};

	enum
	{
		PHASE_NUM = 200,
	};

	//	Current ground information
	struct GNDINFO
	{
		float		fGndHei;		//	Ground height
		float		fWaterHei;		//	Water height
		A3DVECTOR3	vGndNormal;		//	Terrain normal
		bool		bOnGround;		//	On ground flag
	};

	//	Breath data
	struct BREATHDATA
	{
		bool		bDiving;		//	true, is diving
		int			iCurBreath;
		int			iMaxBreath;
	};

	//	Team invite info.
	struct TEAMINV
	{
		int		idLeader;
		int		seq;
		WORD	wPickFlag;
		int		idFamily;
		int		idGuild;
	};

	//	Cool time
	struct COOLTIME
	{
		int		iCurTime;
		int		iMaxTime;
	};

	//	Battle info.
	struct BATTLEINFO
	{
		int		idBattle;		//	Battle id
		int		iResult;		//	Battle result
		int		iResultCnt;		//	Result time counter
		int		iMaxScore_I;	//	Maximum score of invader
		int		iMaxScore_D;	//	Maximum score of defender
		int		iScore_I;		//	Score of invader
		int		iScore_D;		//	Score of defender
		int		iFlagAttacker;
		int		iFlagDefender;
		int		iResAttacker;
		int		iResDefender;
		int		iEndTime;		//	Battle end time
		int		iDefenderFaction;   // 守方帮派
        int		iAttackerFaction;   // 攻方帮派
	};

	struct WAR_INFO
	{
		int		iBattleID;
		int		iWarAttackFID;
		int		iWarAssistFID;
		int		iWarDefenceFID;
		int		iEndTime;		//	Battle end time
	};

	struct REBORN_INFO
	{
		short prof;     //本次转生时的职业
		short level;    //本次转生时的等级
	};

	//	Apoise skill info
	struct APOISESKILL
	{
		int		idSkill;		//	Skill id
		int		iSkillLvl;		//	Skill level
//		int		iTargetClass;	//	Target class
		int		iItemSlot;		//	Slot of item
		int		tidItem;		//	Item id

//		bool IsPosSkill();
	};
	
	struct TRANSFIGUREPROP
	{
		bool    bShieldSkill;
		bool	bCanFight;
		bool	bCanRunHelper;
		bool    bCanUseMedicine;
		int		iMoveType;
		int		level;
		int		exp_level;
	};
	// transfigured shape move type 
	enum
	{
		TRANSFIGURE_MOVE_FREE = 0,
		TRANSFIGURE_MOVE_FORBID,
		TRANSFIGURE_MOVE_RANDOM,
	};
	//	Target state
	enum
	{
		TARGET_OK = 0,			//	Good target
		TARGET_OKINTHEORY,		//	Good target in theory
		TARGET_NOTOK,			//	Bad target in both theory and fact
		TARGET_INVALID,			//	Totally invalid target
	};
	
	struct FASHION_HOTKEY
	{
		int id_head;
		int id_body;
		int id_shoe;
	};
	struct SKILL_COOL_BACKUP
	{
		int skill_id;
		int cool_time;
	};

	//Added 2011-07-07. 焚香火焰囚牢技能所需要存储的单个囚牢信息
	struct JAIL_CIRCLE_AREA
	{
		char		jail_type;
		float		jail_radius;
		A3DVECTOR3	jail_center;	
	};
	//Added end.

	//Added 2011-08-02. 用于焚香和太昊的牵引/拉扯技能中链接光效信息管理
	struct PULL_GFX_INFO
	{
		int			nTargetId;	//被牵引/拉扯的目标ID
		A3DGFXEx*	pGfx;		//牵引/拉扯光效gfx指针
	};
	//Added end.

	//猪小仙的信息
	struct LITTLE_PET_INFO
	{
		int iLevel;
		int iExp;
		int iAward;	//待领取的奖励等级
	};

	// 链式攻击技能特效信息
	struct CHAIN_GFX_INFO
	{
		abase::vector<int>			vecTargets;		// 目标id列表
		abase::vector<A3DGFXEx*>	vecGfxs;		// 特效
	};

	struct Fac_BuildField
	{
		int  nIndex;
		int  nId;
		int  nLevel;
		int  nStatus;
	};

	typedef abase::vector<REBORN_INFO> RebornInfoVec;

	//Added 2011-06-20.焚香-八荒玄火阵技能，用于管理阵法的特殊gfx
	typedef abase::hashtab<A3DGFXEx*, int, abase::_hash_function> CircleOfDoomGFX;
	typedef abase::hashtab<CircleOfDoomGFX*, int, abase::_hash_function> CircleOfDoomTable;
	typedef abase::hashtab<AString, int, abase::_hash_function> CircleOfDoomGfxNameTable; //Added 2011-08-02.
	//Added end.

	//Added 2011-07-08. 焚香-火焰囚牢技能，用于管理主角可视范围内所有囚牢信息
	typedef abase::hashtab<JAIL_CIRCLE_AREA, int, abase::_hash_function> JailCircleAreaTable;
	//Added end.

	//Added 2011-08-02. 用于焚香/太昊的牵引/拉扯技能中存储特效gfx等信息的列表
	typedef abase::hashtab<PULL_GFX_INFO, int, abase::_hash_function> PullGfxInfoTable;
	//Added end.

	//Added 2012-02-21.	用于存储附加永久技能的id-CECSkill指针列表。!!!注意：永久时装技能的存储、删除等管理有主动和被动技能列表负责，这个列表仅用于查询的方便!!!
	typedef abase::hash_map<int, CECSkill*>	AddonPermanentSkillMap;
	//Added end.

	// 链式攻击技能特效信息列表
	typedef abase::hashtab<CHAIN_GFX_INFO, int, abase::_hash_function> ChainGfxInfoTable;

	typedef abase::hash_map<int ,Fac_BuildField> FacBuildField;

	friend class CECHPWorkStand;
	friend class CECHPWorkTrace;
	friend class CECHPWorkUse;
	friend class CECHPWorkDead;
	friend class CECHPWorkMelee;
	friend class CECHPWorkMove;
	friend class CECHPWorkBeMoved; //Added 2011-07-28.
	friend class CECHPWorkSpecialCharge; //Added 2011-08-01.
	friend class CECHPWorkSpecialMove;	//Added 2012-06-12.
	friend class CECHPWorkFollow;
	friend class CECHPWorkAutoMove;
	friend class CECHPWorkFly;
	friend class CECHPWorkFall;
	friend class CECHPWorkSit;
	friend class CECHPWorkSpell;
	friend class CECHPWorkPick;
	friend class CECHPWorkGeneralOprt;
	friend class CECHPWorkRevive;
	friend class CECHPWorkFMove;
	friend class CECHPWorkBlewUp;
	friend class CECHPWorkParabolic;
	friend class CECHPWorkMoveNoCollision;
	friend class CECHPWorkBezierMove;
	friend class CECHostInputFilter;

	friend class CECHostMove;
	friend class CECHPWorkMan;
	friend class CECPlayer;

public:		//	Constructor and Destructor

	CECHostPlayer(CECPlayerMan* pPlayerMan);
	virtual ~CECHostPlayer();

public:		//	Attributes

public:		//	Operations

	//	Initlaize object
	bool Init(const S2C::cmd_self_info_1& Info);
	GNET::GamedataSend gen_self_info();
	GNET::GamedataSend gen_self_info_00();
	GNET::GamedataSend gen_cmd_own_ext_prop();
	GNET::GamedataSend gen_cmd_get_own_money();
	GNET::GamedataSend gen_player_cash();
	GNET::GamedataSend gen_pk_value();
	GNET::GamedataSend gen_mafia_contribution();
	GNET::GamedataSend gen_region_reputation();
	GNET::GamedataSend gen_task_data();
	GNET::GamedataSend gen_skill_data();
	GNET::GamedataSend gen_title_list();
	GNET::GamedataSend gen_sel_target();
	bool gen_record_msg(ECMSG& msg);
	GNET::Octets gen_skill_addon_data();
	void set_skill_addon(GNET::Octets& o);
	GNET::GamedataSend gen_reborn_info();
	//	Release object
	virtual void Release();
	// Release models
	void ReleaseModels();
	// Reload models
	void ReloadModels();

	//	When all initial data is ready, this function is called
	void OnAllInitDataReady();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0);
	//	Render when player is opening booth
	virtual bool RenderForBooth(CECViewport* pViewport, int iRenderFlag=0);
	//	Render for UI
	virtual bool RenderForUI(CECViewport * pViewport);
	
	//	Update camera in following mode
	void UpdateFollowCamera(bool bRunning, DWORD dwTime);

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	Set absolute position
	virtual void SetPos(const A3DVECTOR3& vPos);
	//	Set absolute forward and up direction
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);	
	//	Player was killed
	virtual void Killed(int idKiller);
	//	Get off pet
	virtual void GetOffPet();

	//	Do an emote action
	virtual bool DoEmote(int idEmote);
	//	Start general operation
	virtual void StartGeneralOprt(int op_id, int duration);
	//	Stop general operation
	virtual void StopGeneralOprt(int op_id);
	//	Search the full suite
	virtual int SearchFullSuite();
	//	Get number of equipped items of specified suite
	virtual int GetEquippedSuiteItem(int idSuite, int* aItems=NULL);

	void SetMoveArrow(bool bFlag, const A3DVECTOR3& vecDir, const A3DVECTOR3& vecPos) { m_bShowMoveArrow = bFlag; m_vecMoveArrow = vecDir; m_vecMovePos = vecPos; }
	void SetTempArrow(bool bFlag, const A3DVECTOR3& vecDir, const A3DVECTOR3& vecPos) { m_bShowTempArrow = bFlag; m_vecTempArrow = vecDir; m_vecTempPos = vecPos; }
	void SetTeamArrow(bool bFlag, const A3DVECTOR3& vecDir, const A3DVECTOR3& vecPos) { m_bShowTeamArrow = bFlag; m_vecTeamArrow = vecDir; m_vecTeamPos = vecPos; }
	void SetRequestArrow(bool bFlag, const A3DVECTOR3& vecDir, const A3DVECTOR3& vecPos) { m_bShowRequestArrow = bFlag; m_vecRequestArrow = vecDir; m_vecRequestPos = vecPos; }
	void ClearTargetArrows() { m_vecTargetArrows.RemoveAll(false); m_vecTargetPos.RemoveAll(false); }
	void AddTargetArrow(const A3DVECTOR3& vecDir, const A3DVECTOR3& vecPos) { m_vecTargetArrows.Add(vecDir); m_vecTargetPos.Add(vecPos); }
	CECModel * GetModelTray() { return m_pModelTray; }
	CECModel * GetModelMoveArrow() { return m_pModelMoveArrow; }
	CECModel * GetModelTempArrow() { return m_pModelTempArrow; }
	CECModel * GetModelTeamArrow() { return m_pModelTeamArrow; }
	CECModel * GetModelRequestArrow() { return m_pModelTeamArrow; }
	CECModel * GetModelTargetArrow() { return m_pModelTargetArrow; }
	
	//	Get camera coordinates
	A3DCoordinate* GetCameraCoord() { return &m_CameraCoord; }
	//	Get cameractrl
	CECCamera* GetCameraCtrl() { return &m_CameraCtrl; }
	//	Save configs data (shortcut, etc.) to specified buffer
	bool SaveConfigData(void* pDataBuf, int* piSize);
	//	Load configs data (shortcut, etc.) from specified buffer
	bool LoadConfigData(const void* pDataBuf);

	//	Revive
	void Revive(const S2C::cmd_player_revive* pCmd);
	//	Begin following the selected target
	void BeginFollow(int idTarget);
	
	void BeginAutoMove();
	bool EndAutoMove();
	
	virtual void SetName(const ACHAR *szName);

	//	Level up
	void LevelUp();
	//	Task finish
	void OnTaskFinished();
	//	Hatch pet
	bool HatchPet(int iIvtrIdx);
	//	Restore pet
	bool RestorePet(int iPetIdx);
	//	Summon pet
	bool SummonPet(int iPetIdx);
	//	Recall pet
	bool RecallPet();
	//	Banish pet
	bool BanishPet(int iPetIdx);
	//	Get battle pet index
	int GetBattlePetIndex();
	//	Is host operating pet ?
	int IsOperatingPet();
	//	Get pet operation time counter
	CECCounter& GetPetOptTime() { return m_PetOptCnt; }
	//	Producing
	bool IsProducing() const { return m_bProducing; }
	void SetProducing(bool b) { m_bProducing = b; }

	//	Prepare to begin NPC service
	void PrepareNPCService(int idSev);
	//	End NPC service
	void EndNPCService();
	//	Buy items from NPC
	void BuyItemsFromNPC();
	//	Sell items to NPC
	void SellItemsToNPC();
	void BuyItemsBackFromNPC();
	//	Buy item from reputation shop
	void BuyItemsFromShopReputation();
	//	Buy items from else player's booth
	void BuyItemsFromBooth();
	//	Sell items to else player's booth
	void SellItemsToBooth();
	//	When the booth we are visiting closed
	void OnOtherBoothClosed();
	//	Open booth
	void OpenBooth(const ACHAR* szName);

	//	Team invite
	void TeamInvite(const GNET::PlayerBriefInfo& Info);

	//	Commands ...
	bool CmdSitDown(bool bSitDown);		//	Normal commands...
	bool CmdWalkRun(bool bRun);
	bool CmdNormalAttack(bool bMoreClose=false, bool bCombo=false, int idTarget=0, int iForceAtk=-1);
	bool CmdFindTarget();
	bool CmdAssistAttack();
	bool CmdFly();
	bool CmdPickup();
	bool CmdGather();
	bool CmdRushFly();
	bool CmdBindBuddy(int idTarget);
	bool CmdBindQilinBuddy(int idTarget);
	bool CmdViewOtherEquips(int idTarget);
	bool CmdAskDuel(int idTarget);
	bool CmdInviteToTeam();				//	Team commands...
	bool CmdLeaveTeam();
	bool CmdKickTeamMember();
	bool CmdFindTeam();
	bool CmdStartTrade();				//	Trade commands...
	bool CmdSellBooth();
	bool CmdBuyBooth();
	bool CmdStartPose(int iPose);		//	Pose commands...
	bool CmdStartActivePose(int iPose);	//	Active pose command...
	bool CmdInviteToFaction();			//	Faction commands...
	bool CmdPetCall();
	bool CmdPetCombine1();
	bool CmdPetCombine2();
	bool CmdBindMultiBuddy(int idTarget);
	bool CmdMultiBindKick(int idTarget);
	bool CmdSetHideEquipMask(int mask);
	bool CmdGetBattleInfo();


	//	Get inventories interface
	CECInventory* GetPack(int iPack);
	CECInventory* GetPack() { return m_pPack; }
	CECInventory* GetEquipment() { return m_pEquipPack; }
	CECInventory* GetTrashBox() { return m_pTrashBoxPack; }
	CECInventory* GetFactionTrashBox() { return m_pFactionTrashBoxPack; }
	CECInventory* GetTaskPack() { return m_pTaskPack; }
	CECInventory* GetPetPack() { return m_pPetPack; }
	CECInventory* GetPetEquipPack() { return m_pPetEquipPack; }
	CECDealInventory* GetDealPack() { return m_pDealPack; }
	CECDealInventory* GetEPDealPack() { return m_pEPDealPack; }
	CECDealInventory* GetBuyPack() { return m_pBuyPack; }
	CECDealInventory* GetSellPack() { return m_pSellPack; }
	CECInventory* GetSellBackPack() { return m_pSellBackPack; }
	CECNPCInventory* GetNPCSevPack(int n) { ASSERT(n >= 0 && n < NUM_NPCIVTR); return m_aNPCPacks[n]; }
	CECReputationInventory* GetReputationPack(int n) {ASSERT(n >= 0 && n < 4); return m_aReputationPacks[n];}
	CECDealInventory* GetBoothSellPack() { return m_pBoothSPack; }
	CECDealInventory* GetBoothBuyPack() { return m_pBoothBPack; }
	CECDealInventory* GetEPBoothSellPack() { return m_pEPBoothSPack; }
	CECDealInventory* GetEPBoothBuyPack() { return m_pEPBoothBPack; }
	CECInventory* GetEPEquipPack() { return m_pEPEquipPack; }
	CECInventory* GetEPPetPack() { return m_pEPPetPack; }
	CECInventory* GetEPPetEquipPack() { return m_pEPPetEquipPack; }
	CECInventory* GetPocketPack() { return m_pPocketPack; }
	CECInventory* GetFashionPack(){ return m_pFashionPack; }
	CECInventory* GetHTFarmPack() { return m_pHTFarmPack; }
	CECInventory* GetMountWingPack(){ return m_pMountWingPack; }
	CECInventory* GetGiftPack(){ return m_pGiftPack; }
	CECInventory* GetRune2013Pack() { return m_pRune2013Pack; }

	int GetGiftScore();
	int GetTackGiftIndex();

	int GetTrashBoxMoneyCnt() const { return m_iTrashBoxMoneyCnt; }
	//	Get detail data of host's specified item
	void GetIvtrItemDetailData(int iPack, int iSlot);
	//	Check whether player can use specified equipment
	bool CanUseEquipment(CECIvtrEquip* pEquip);
	//	Use specified inventory item in pack
	bool UseItemInPack(int iPack, int iSlot);
	//	Freeze / Unfreeze specified item
	bool FreezeItem(int iIvtr, int iIndex, bool bFreeze, bool bFreezeByNet);
	//	Check whether player can take specified matter
	bool CanTakeItem(int idItem, int iAmount);

	//	Carrier about
	PosPair GetStartPosPair() const { return m_StartPosPair; } 
	PosPair GetEndPosPair() const { return m_EndPosPair; } 
    void SetStartPosPair(const PosPair& pairpos) { m_StartPosPair = pairpos; }
    void SetEndPosPair(const PosPair& pairpos) { m_EndPosPair = pairpos; }
		
	//	Get shortsets
	CECShortcutSet* GetShortcutSet1(int n) { ASSERT(n >= 0 && n < NUM_HOSTSCSETS1); return m_aSCSets1[n]; }
	CECShortcutSet* GetShortcutSet2(int n) { ASSERT(n >= 0 && n < NUM_HOSTSCSETS2); return m_aSCSets2[n]; }
	CECShortcutSet* GetShortcutSet3(int n) { ASSERT(n >= 0 && n < NUM_HOSTSCSETS3); return m_aSCSets3[n]; }
	//	Apply shortcut of specified inventory item
	bool ApplyItemShortcut(int iPack, int iSlot) { return UseItemInPack(iPack, iSlot); }
	//	Apply shortcut of specified skill
	//  pForceSkill 用于爬塔技能中技能，只能使用一次
	bool ApplySkillShortcut(int idSkill, bool bCombo=false, int idSelTarget=0, int iForceAtk=-1, CECSkill *pForceSkill=NULL, A3DVECTOR3* pCastSkillPos = NULL);	//pCastSkillPos:释放无目标技能时的位置信息 Modified2012-05-04.
	//	Apply shortcut key up of specified skill 
	bool ApplySkillShortcutUp(int idSkill, bool bCombo = false, int idSelTarget = 0, int iFroceAtk =-1);
	//  蓄力技能记录对应的键码
	void SetCurSkillKeyCode(unsigned int keycode) { m_iCurSkillKeyCode = keycode; }
	unsigned int GetCurSkillKeyCode() const { return m_iCurSkillKeyCode; } 
	//	Return to a target town through skill
	bool ReturnToTargetTown(int idTarget);
	//	Can do produce
	bool CanDoProduce() { return CanDo(CANDO_PRODUCE); }

	//	Get positive skill number
	int GetPositiveSkillNum() { return m_aPtSkills.GetSize(); }
	//	Get positive skill by index
	CECSkill* GetPositiveSkillByIndex(int n) { return m_aPtSkills[n]; }
	//	Get positive skill by id
	CECSkill* GetPositiveSkillByID(int id, bool bSenior=false);
	//	Get passive skill number
	int GetPassiveSkillNum() { return m_aPsSkills.GetSize(); }
	//	Get passive skill by index
	CECSkill* GetPassiveSkillByIndex(int n) { return m_aPsSkills[n]; }
	//	Get passive skill by id
	CECSkill* GetPassiveSkillByID(int id, bool bSenior=false);
	//  Get transfigure skill number
	int GetTransfigureSkillNum() { return m_aTsSkills.GetSize(); }
	//  Get transfigure skill by index
	CECSkill* GetTransfigureSkillByIndex(int n) { return m_aTsSkills[n]; }
	//  Get transfigure skill by id
	CECSkill* GetTransfigureSkillByID(int id, bool bSenior = false);
	//	Get addon permanent skill number
	int GetAddonPermanentSkillNum() { return m_aAPSkills.size(); }
	//	Get addon permanent skill by index
	CECSkill* GetAddonPermanentSkillByIndex( int n );
	//	Get addon permanent skill by id
	CECSkill* GetAddonPermanentSkillByID( int id, bool bSenior = false );

	//  Get transfigure skill number
	int GetItemSkillNum() { return m_aImSkills.GetSize(); }
	//  Get transfigure skill by index
	CECSkill* GetItemSkillByIndex(int n) { return m_aImSkills[n]; }
	//  Get transfigure skill by id
	CECSkill* GetItemSkillByID(int id, bool bSenior = false);

	//	Get skill by id
	CECSkill* GetSkill(int id, bool bSenior=false);
	//	Get else player's skill number
	int GetElsePlayerSkillNum() { return m_aEPSkills.GetSize(); }
	//	Get else player's skill by index
	CECSkill* GetElsePlayerSkillByIndex(int n) { return m_aEPSkills[n]; }
	//	Get else player's skill by id
	CECSkill* GetElsePlayerSkillByID(int id, bool bSenior=false);
	//	Check whether host can learn specified skill
	int CheckSkillLearnCondition(int idSkill, bool bCheckBook);
	//	Check whether host can cast specified skill
	int CheckSkillCastCondition(CECSkill* pSkill);
	//  Check whether specified item existed for skill to cost
	//  检查释放该技能需要的物品是否存在
	int CheckSkillCostItemCondition(CECSkill* pSkill);
	//  检查释放该技能需要的召唤兽是否存在
	int CheckSkillSummonMonsterCondition(CECSkill* pSkill);
	//  向技能里面设置玩家属性
	void SetPlayerProps(CECSkill* pSkill);
	//	Get prepare skill
	CECSkill* GetPrepSkill() { return m_pPrepSkill; }
	//	Get skill ability
	int GetSkillAbility(int idSkill);
	//	Get skill ability percent
	int GetSkillAbilityPercent(int idSkill);
	//	Remove skill
	void RemoveSkill(int idSkill);
	//	Apply combo skill
	bool ApplyComboSkill(int iGroup);
	//	Clear combo skill
	void ClearComboSkill();
	//	Replace specified skill with it's senior skill
	void ReplaceJuniorSkill(int idJuniorSkill, CECSkill* pSeniorSkill);
	//	Get all skill level up
	int GetAllSkillLevelUp() { return m_iAllSkillLvlUp; }
	//	Insert addon skill
	CECSkill* InsertAddonSkill(int idSkill, int lev);
	CECSkill* InsertTransfigureSkill(int idSkill, int lev);

	//	Is host player trading ?
	bool IsTrading() { return m_idTradePlayer ? true : false; }
	//	Is host player open trash box ?
	bool IsUsingTrashBox() const { return m_bUsingTrashBox; }
	//	Is host in root state
	bool IsRooting() { return ( ((m_dwLIES & (LIES_ROOT | LIES_SLEEP | LIES_STUN | LIES_PULLED_ROOT | LIES_PULLING_ROOT | LIES_DRAGGED_ROOT)) ? true : false ) || ( m_iSpecialState & SPECIAL_STATE_FROZEN) ); }
	//	Can player attack ?
	bool CannotAttack() { return (( (m_dwLIES & LIES_DISABLEFIGHT) ? true : false) || (m_iSpecialState & SPECIAL_STATE_FROZEN) ); }
	//	Is host in melee state ?
	bool IsMeleeing() { return m_bMelee; }
	//	Is host player talking with NPC ?
	bool IsTalkingWithNPC() { return m_bTalkWithNPC; }
	//	Is spelling magic
	bool IsSpellingMagic();
	//	Is spelling durative skill ?
	bool IsSpellingDurativeSkill() { return IsSpellingMagic() && m_bSpellDSkill; }
	//	Is flash moving ?
	bool IsFlashMoving();
	//	Is taking off ?
	bool IsTakingOff();
	//	Is jumping
	bool IsJumping() const { return m_iJumpCount > 0; }
	bool IsJumpInWater() const { return m_bJumpInWater; }
	bool IsJumpByQingGong() const { return m_bJumpByQingGong; }

	void SetJumpInWater(bool b) { m_bJumpInWater = b; }
	void SetJumpByQingGong(bool b) { m_bJumpByQingGong = b; }
	void ResetJump() { m_iJumpCount = 0; m_bJumpInWater = false; m_bJumpByQingGong = false; }
	//	Is free falling
	bool IsFalling() { return (m_iMoveEnv == MOVEENV_GROUND && !m_GndInfo.bOnGround); }
	//	Is moving
	bool IsMoving() { return m_iMoveMode != MOVE_STAND ? true : false; }
	//	Is picking up something
	bool IsPicking();
	//	Is gathering resources
	bool IsGathering();
	//	Is operating
	bool IsOperating();
	//	Is reviving
	bool IsReviving();
	//	Is using item ?
	bool IsUsingItem();
	//	Is changing face ?
	bool IsChangingFace() { return m_bChangingFace; }
	//	Is at special charge states?	Added 2011-08-02.
	bool IsSpecialCharge() { return m_bSpecialCharge; }
	void SetSpecialCharge( bool bState ) { m_bSpecialCharge = bState; }
	//	Is at the starting pos skill? Added 2012-05-04.
	bool IsStartPosSkill() { return m_bStartPosSkill; }
	void SetStartPosSkill( bool bStartPosSkill ) { m_bStartPosSkill = bStartPosSkill; }
	
	bool IsLiuDaoSkill(int idSkill);

	//	Is doing session pose ?
	bool DoingSessionPose();
	//	Is adjust orient
	bool IsAdjustOrient() { return m_bAdjustOrient; }
	void SetAdjustOrient(bool bAdjust) { m_bAdjustOrient = bAdjust; }
	//	Can left button turn camera
	bool LeftButtonTurnCamera();
	//	Does trash box have password ?
	bool TrashBoxHasPsw() { return m_bTrashPsw; }
	//	Decide target name color basing on target level
	DWORD DecideTargetNameCol(int iTargetLevel);
	//	Is all initial data ready ?
	bool HostIsReady() { return m_bEnterGame; }
	//	Is player moving
	virtual bool IsPlayerMoving();
	//	Is under water
	bool IsUnderWater() { return m_iMoveEnv == MOVEENV_WATER ? true : false; }
	//	Can jump or take off in water ?
	bool CanTakeOffWater();
	//	Get breath data
	const BREATHDATA& GetBreathData() { return m_BreathData; }
	//	Get the exp will lost if revived by other player
	float GetReviveLostExp() { return m_fReviveExp; }
	//	Get deadly strike rate
	int GetDeadlyStrikeRate();
	//	Is host in sliding state (in the state, host is sliding on slope) ?
	bool InSlidingState();
	//	Get left pariah time
	DWORD GetPariahTime() { return m_dwPariahTime; }
	//	Get battle info.
	BATTLEINFO& GetBattleInfo() { return m_BattleInfo; }

	bool IsInWar() const { return m_bInWar; }
	void SetIsInWar(bool b) { m_bInWar = b; }
	const WAR_INFO& GetWarInfo() const { return m_WarInfo; }
	void SetWarInfo(const WAR_INFO& info) { m_WarInfo = info; }

	RebornInfoVec& GetRebornInfo() { return m_RebornInfo; }

	bool IsSafeLock() const { return m_bSafeLock; }
	void SetSafeLock(bool b) { m_bSafeLock = b; }

	//	On start binding buddy
	virtual void OnStartBinding(int idMule, int idRider,bool isBath = false);

	// On start binding  Qilin buddy
	virtual void OnStartQiLinBinding(int idInvitor, int idInvitee);
	void ReConnectQilin();

	virtual void UpdatePupPetState(DWORD dwDeltaTime);

	//	Get cool time
	int GetCoolTime(int iIndex, int* piMax=NULL);
	//	Get produce cool time
	int GetProduceCoolTime(int iIndex, int* piMax=NULL);
	//  Get SKill edit cool time
	int GetSkillEditCoolTime(int iIndex, int* piMax=NULL);
	//	Get gather counter
	CECCounter& GetGatherCnt() { return m_GatherCnt; }
	CECCounter& GetPerformCnt() { return m_PerformCnt; 	}
	void ResetPerfomCnt(bool bFull=false) { m_PerformCnt.Reset(bFull);}
	int GetOptId() { return m_iOperateId; }
	//	Get ext instance left time
	int GetTimeToExitInstance()	{ return m_nTimeToExitInstance; }
	//	Get bind command cool time counter
	CECCounter& GetBindCmdCoolCnt() { return m_BindCmdCoolCnt; }
	//	Get time counter of using item in pack
	bool GetUsingItemTimeCnt(DWORD& dwCurTime, DWORD& dwMaxTime, int* piItem=NULL);

	//	Get selected target
	int GetSelectedTarget() { return m_idSelTarget; }
	//	Get right button clicked player
	int	GetClickedMan() { return m_idClickedMan; }
	//	Get current service NPC
	int GetCurServiceNPC() { return m_idSevNPC; }
	//	Get object under cursor
	int GetCursorHoverObject() { return m_idCurHover; }

	//	Get active waypoint number
	int GetWayPointNum() { return m_aWayPoints.GetSize(); }
	//	Get way point
	WORD GetWayPoint(int n) { return m_aWayPoints[n]; } 
	//	Does host player have specified way point ?
	bool HasWayPoint(WORD wID);

	//	Get task interface
	CECTaskInterface* GetTaskInterface() { return m_pTaskInterface; }
	//	Get Portrait
	A2DSprite* GetPortraitSprite() { return m_pSpritePortrait; }
	//	Get friend manager
	CECFriendMan* GetFriendMan() { return m_pFriendMan; }
	//	Get move control
	CECHostMove& GetMoveControl() { return m_MoveCtrl; }
	//	Set player manager
	void SetPlayerMan(CECPlayerMan* pPlayerMan) { m_pPlayerMan = pPlayerMan; }
	//	Get pet corral object
	CECPetCorral* GetPetCorral() { return m_pPetCorral; }
	//  Get current pet id
	int GetCurPetId() { return m_nCurPetId; }
	//  Get current pet index
	int GetCurPetIndex() { return m_nCurPetIndex; }
	//  Get current pet maxhp
	int GetCurPetMaxHp() { return m_nCurPetMaxHp; }
	//  Get current pet maxvp
	int GetCurPetMaxVp() { return m_nCurPetMaxVp; }
	int GetPetCombinedType() { return m_nPetCombinedType; }
	//  Get current pet index
	void SetPetCivilization(int nCivilization) { m_nPetCivilization = nCivilization; }
	int GetPetCivilization() { return m_nPetCivilization; }
	void SetPetConstruction(int nConstruction) { m_nPetConstruction = nConstruction; }
	int GetPetConstruction() { return m_nPetConstruction; }

	// Host faction info
	int GetLoyalty() const { return m_iLoyalty; } 	//	Get loyalty
	void SetLoyalty(int l) { m_iLoyalty = l; }		//	Set loyalty
	const ACHAR* GetNickname() const { return m_strNickname; } 	//	Get nickname
	void SetNickname(const void* p, int len); 		//	Set nickname

	// Recipe data info
	void AddOneRecipe(unsigned short recipe_id) { m_RecipeData.push_back(recipe_id); }
	int GetRecipeCount() const { return (int)m_RecipeData.size(); }
	unsigned short GetRecipeID(int index) const { return m_RecipeData[index]; }
	bool HasRecipe(unsigned short id) const
	{
		for (size_t i = 0; i < m_RecipeData.size(); i++)
			if (m_RecipeData[i] == id)
				return true;

		return false;
	}
	void ClearRecipeData() { m_RecipeData.clear(); }
	int GetProduceSkillLevel() const { return m_nProduceSkillLevel; }
	void SetProduceSkillLevel(int nLev) { m_nProduceSkillLevel = nLev; }
	int GetProduceSkillExp() const { return m_nProduceSkillExp; }
	void SetProduceSkillExp(int nExp) { m_nProduceSkillExp = nExp; }

	int GetPKValue() const { return m_nPKValue; }
	void SetPKValue(int nValue) { m_nPKValue = nValue; }

	int GetBattleScore() const { return m_nBattleScore; }
	void SetBattleScore(int nScore) { m_nBattleScore = nScore; }
	
	int GetSJBattleScore() const { return m_nSJBattleScore; }
	
	int GetTalismanValue() const { return m_nTalismanValue; }
	void SetTalismanValue(int nValue) { m_nTalismanValue = nValue; }
	int GetRuneValue() const { return m_nRuneValue; }
	int GetRuneCustomize() const {return m_nRuneCustomize;}

	int GetTalentValue() const { return m_nTalentValue; }
	void SetTalentValue(int nValue) { m_nTalentValue = nValue; }

	int GetCombatFaction() const { return m_nCombatFaction; }
	void SetCombatFaction(int id) { m_nCombatFaction = id; }
	void SetKingdomHasInit() {m_bInitKingTitle = true;}
	char GetKingdomTitle() const {return  m_cKingTitle;}
    void SetKingdomTitle(char title){ m_cKingTitle = title;}
	int  GetKingdomKingId() const { return m_iKingId;}
	void SetKingdomKingId(int id){m_iKingId = id;}
	int  GetKingdomPoints() const {return  m_iKingPoints;}
	void SetKingdomPoints(int points){m_iKingPoints = points;}
	char GetKingdomWarType(){return m_cKingdomWarType;}
	bool IsBathing(){return m_bIsBathing;}
	void ResetFactionCombatCounts();
	void ResetGuildWarStartCounts();
	void ResetGuildWarWinCounts();
	void ResetGuildWarLostCounts();

	// UI使用，iFashion默认值表示时装模式跟hostplayer同步，否则其它界面一般采用时装模式
	void SyncDemonstrationPlayer(CECLoginPlayer* pPlayer, int iFashion=-1);

	int GetCash() const { return m_nCash; }
	void SetCash(int nCash) { m_nCash = nCash; }
	int GetCashAdd() const { return m_nCashAdd; }
	void SetCashAdd(int n) { m_nCashAdd = n; }
	int GetCashConsumed() const { return m_nCashConsumed; }
	void SetCashConsumed(int n) { m_nCashConsumed = n; }
	int GetAgentTime() const { return m_AgentTime; }
	void SetAgentTime(int n) { m_AgentTime = n; }
	int GetCurEmotionSet() const { return m_nCurEmotionSet; }

	int GetContribution() const { return m_nContribution; }
	void SetContribution(int iVal) { m_nContribution = iVal; }
	int GetFamilyContrib() const { return m_nFamilyContrb; }
	void SetFamilyContrib(int iVal) { m_nFamilyContrb = iVal; }

	int GetTitleCount() const { return m_vecTitles.size(); }
	short GetOneTitle(int nIndex) const { return m_vecTitles[nIndex]; }
	void AddOneTitle(short title) { m_vecTitles.push_back(title); SortTitle(); }
	void DelOneTitle(short title)
	{
		for (size_t i = 0; i < m_vecTitles.size(); i++)
		{
			if (m_vecTitles[i] == title)
			{
				m_vecTitles.erase(m_vecTitles.begin() + i);
				break;
			}
		}

		SortTitle();
	}
	void RemoveAllTitles() { m_vecTitles.clear(); }
	bool HasTitle(short title)
	{
		for (size_t i = 0; i < m_vecTitles.size(); i++)
		{
			if (m_vecTitles[i] == title)
				return true;
		}

		return false;
	}

	//	Get region reputation count
	int GetRegionReputationCount() const { return m_vecRegionRepu.size(); }
	int GetRegionReputation(int iIndex) const { return m_vecRegionRepu[iIndex]; }
	//	Add region reputation
	void AddRegionReputations(int count, int* repu)
	{
		m_vecRegionRepu.clear();

		for (int i = 0; i < count; i++)
			m_vecRegionRepu.push_back(repu[i]);
	}
	//	Update region reputation
	void UpdateRegionReputation(int iIndex, int iRepu)
	{
		int diff = iRepu - m_vecRegionRepu[iIndex];
		m_vecRegionRepu[iIndex] = iRepu;
		BubbleText(BUBBLE_REGION_REPU, diff, 0, iIndex);
	}

	//	Check whether host can do a behavior
	bool CanDo(int iThing);
	//	Get force attack flag
	bool GetForceAttackFlag(const DWORD* pdwParam);
	//	Attackable judge
	int AttackableJudge(int idTarget, bool bForceAttack);
	//	Build PVP mask
	BYTE BuildPVPMask(bool bForceAttack);
	//	Get hpwork man
	CECHPWorkMan * GetHPWorkMan()const{ return m_pWorkMan;}

	//	Get the Online award manager Added 2012-03-23.
	OnlineAwardManager* GetOnlineAwardManager() const { return m_pOnlineAwardManager; }
	CECBattleDungeonTower* GetBattleDungeonTower() const { return m_pBattleDungeonTower;}

	//	Set state of under other player's attack
	//	bIgnoreNormalAtk == true will ignore the attacks from ElsePlayers in Duel / Battle / Combat faction
	void SetUnderAtk(bool bIgnoreNormalAtk = true);
	//	Is Attacking another player, time elapsed since last attack another play is below 10000ms 
	bool IsAttackingOtherPlayer() { return !m_ReAtkCnt.IsFull(); }
	//	Under Wicked Attack (Host is white name and Host is not attacking any other player)
	//	Only when the host player is in white name and not in battle / duel / combat faction
	bool IsUnderWickedAtk();
	
	void SetBuffState(const S2C::self_buff_notify* cmd);
	
	FASHION_HOTKEY& GetFashionHotkey(int iIndex){ ASSERT(iIndex>=0 && iIndex<FASHION_HOTKEY_NUM); return m_aFashionHotKey[iIndex];}
	void SetFashionHotkey(int iIndex, int id_head, int id_body, int id_shoe);
	
	void MoveItemPack2Pocket();
	// 获取自身的推广码
	const ACHAR* GetSelfReferenceCode() const;

	void SetSelfReferenceCode(const void* p, int len); 	
	// 获取自己上线的推广码
	const ACHAR* GetReferralCode() const { return m_strReferralCode; }
	void SetReferralCode(const void* p, int len);
	
	// 获取向服务器发送载体协议是否在冷却中
	const bool GetCarrierCooling() const { return m_bCarrierCooling; }
	void SetCarrierCooling(bool bCooling) { m_bCarrierCooling = bCooling; }

	void SetBonus(int amount, int used){ m_iBonusAmount = amount; m_iBonusUsed = used; }
	const int GetBonusAmount() const { return m_iBonusAmount;}
	const int GetBonusUsed() const{ return m_iBonusUsed; }
	
	void ReceiveBonusExp(double exp);

	void SetTransfigureProp(int id, int level, int exp_level);
	
	const TRANSFIGUREPROP& GetTransfigureProp() { return m_TransfigureProp; }

	void ClearTransfigureProp();
	
	bool IsAutoTeam(int id, int iFactionId, int iFamilyId, ACHAR *strName = NULL);
		//	Update equipment skins
	bool UpdateEquipSkins(bool bAtOnce = false);
	
	int  GetRefineItemNum(int id, int level, int con);
	//  is script trigger finish task
	bool IsScriptFinishTask(int idTask);
	//  is the new task will be finished by trigger script 
	void AddScriptFinishTask(int idTask); 
	void EraseScriptFinishTask(int idTask) { m_ScriptFinishTaskMap.erase(idTask); }

	CECCameraPath*	GetCameraPath() { return m_pCameraPath; }
	CECCutscene*	GetCutscene() { return m_pCutscene; }

	//	start camera path
	void StartCameraPath(DWORD idPath);

	//	start cutscene
	void StartCutscene(DWORD idCutscene, bool bNeedCheck = true);
	//  start animation
	void StartAnimation(DWORD idAnm, bool bNeedCheck = true);
	
	int GetFriendNum(); 
	int GetTerritoryBattleInfo() {  return m_nTerritoryBattle; }
	void SetTerritoryBattleInfo(int info) { m_nTerritoryBattle = info;}
	int GetRaidBattleInfo() {  return m_nRaidBattle; }
	void SetRaidBattleInfo(int info) { m_nRaidBattle = info;}
	void SetLastLogoutTime(int time) { m_iLastLogoutTime = time; }
	int  GetLastLogoutTime() { return m_iLastLogoutTime; } 
	int  GetTerritoryScore();
	int  GetCircleScore();
	
	bool IsInSociety(int id, ACHAR* name = NULL);
	unsigned short* GetCombineSkillElems(int skill_id, int& num);
	void UpdateCombineSkillElems(int skill_id, int* data, int num);
	
	void Stand();
	
	virtual void DeityLevelUp(bool bSuccess, int iLevel);
	bool IsDeityExpFull();
	virtual void UpdateSpecialState(int state, bool on);
	bool IsInDarkState() { return (m_iSpecialState & SPECIAL_STATE_DARK)? true:false; }

	//Added 2011-06-20.
	void UpdateCircleOfDoomGFXs(DWORD dwDeltaTime);
	bool UpdateCircleOfDoom(int nSkillId, int nSponsorId, int nMemberNum, const int* aMemberIds);
	void ReleaseCircleofDoom(int nSponsorId);
	void ReleaseAllCircleOfDoom( void );
	//Added end.

	//Added 2011-07-08.
	bool CheckCurrentPosValid(const A3DVECTOR3& vCurPos); //专用于火焰囚牢技能，判断玩家当前位置是否处于囚牢限定范围的合法位置
	bool AddJailCircleArea( int id, const JAIL_CIRCLE_AREA& jailCircleArea ); //为主角添加一个囚牢信息
	bool RemoveJailCircleArea( int id ); //删除一个囚牢信息
	void ClearAllJailCircleArea( void ); //删除所有的囚牢信息
	bool IsNeedCheckCurPosValid( void )
	{
		return !m_tbJailCircleAreaTable.empty();
	};
	//Added end.

	//Added 2011-08-02. 阵法gfx文件名字列表相关操作
	bool AddCODGfxName( int nSponsorId, const AString& szGfxName ); //根据当前阵法技能而添加相应的阵法gfx 名称
	bool RemoveCODGfxName( int nSponsorId );	//删除一个阵法技能对应的gfx名字
	//Added end.

	//Added 2011-08-02.	拉扯/牵引技能的gfx信息列表相关操作
	bool AddPullGfxInfo( int nCasterId, int nTargetId, unsigned char type ); //添加一项gfx信息
	bool RemovePullGfxInfo( int nCasterId );	//删除一项gfx信息
	void UpdatePullGfxs( DWORD dwDeltaTime );	//刷新拉扯/牵引的gfx位置和状态
	bool RemovePullSpecialGfxInfo( int nCasterId, int nTargetId );	//删除施法者和目标对象身上的特效
	//Added end.

	//added 2011-12-23 切换世界需要做些处理
	void OnLeaveGameWorld();
	

	int GetTabSelOrder() const { return m_nTabSelOrder; }
	void SetTabSelOrder(int nOrder) { m_nTabSelOrder = nOrder; }

	int GetLittlePetLv(){ return m_LittlePetInfo.iLevel;}
	int GetLittlePetExp() {return m_LittlePetInfo.iExp;}
	int GetLittlePetAward() {return m_LittlePetInfo.iAward;}

	//
	A3DVECTOR3	GetVelocity()const { return m_vVelocity; }
	void		SetVelocity( const A3DVECTOR3& vVelocity ) { m_vVelocity = vVelocity; }
	A3DVECTOR3	GetAcceleration()const { return m_vAccel; }
	void		SetAcceleration( const A3DVECTOR3& vAcceleration ) { m_vAccel = vAcceleration; }
	void		SetCDRInfoClipVelocity( const A3DVECTOR3& vClipVelocity );
	CDR_INFO*	GetCDRInfo() const { return m_pCDRInfo; }
	float		GetVertSpeed() const { return m_fVertSpeed; }
	//

	bool AddChainGfxInfo( int nCasterId, abase::vector<int>& vecTargets); //添加一项gfx信息
	bool RemoveChainGfxInfo( int nCasterId );	//删除一项gfx信息
	void UpdateChainGfxs( DWORD dwDeltaTime );	//刷新链式攻击特效gfx位置和状态
	bool RemoveChainSpecialGfxInfo( int nCasterId, abase::vector<int>& vecTargets);	//删除施法者和目标对象身上的特效
	
	bool IsUiTransIdActive(int transid); //检查传送阵的某个传送点是否已激活

	bool IsAstrologyEnergyActive() const { return m_bAstrologyEnergyActive; }	//检查精力值是否已激活
	int  GetAstrologyEnergy() const { return m_nAstrologyEnergy; }				//获取当前精力值, 只在精力值激活后有效

	// 2012-8-22 add by zy 添加角色战场状态
	int	GetBattleFlag()const { return m_nBattleFlag; }
	void SetBattleFlag( const int nFlag ) { m_nBattleFlag = nFlag; }

	// 2012-11-8 add 判断已完成活跃度 by zy
	int GetLivenessCount() const { return (int)m_vecLiveness.size(); }
	bool HasLiveness(int id) const
	{
		int nSize = m_vecLiveness.size();
		for(int nIndex = 0; nIndex < nSize; nIndex++)
		{
			if (m_vecLiveness[nIndex] == id)
				return true;
		}
		return false;
	}
	int getLivenessCurPoint()
	{
		return m_nCur_point;
	}
	int CheckLivenessTacken(int nIndex)
	{
		return m_bIsTaken[nIndex];
	}
	// 2012-12-25 by add zy 
	void CheckRecordMount(int nPackage, int nPoint,int nID);
	void setRecordMonnt(int nPackage, int nPoint,int nNewPackage,int nNewPoint);

	void FinishTinyGameTask(unsigned long ulGameID);    //直接小游戏任务

	// 获取基础属性增益
	int GetPropAddValue(unsigned int type) { ASSERT(type < PROP_ADD_NUM); return m_propAdd[type]; }

	void setIsBattleWait(bool val){m_bIsBattleWait = val;}
	bool getIsBattleWait(){return m_bIsBattleWait;}

	void SetGTTeamState(bool b) { m_bGTInTeam=b; }
	bool GetGTTeamState() { return m_bGTInTeam; }
	
	void SetGTGuildState(bool b) { m_bGTInGuild=b; }
	bool GetGTGuildState() { return m_bGTInGuild; }
		
	bool checkFlowBattleScore();

	int GetCrssvrTeamsId() const { return m_nCrssvrTeamsId; }
	void SetCrssvrTeamsId(int val) { m_nCrssvrTeamsId = val; }
	int GetCrssvrTeamsMaster() const { return m_nCrssvrTeamsMasterId; }
	void SetCrssvrTeamsMaster(int val) { m_nCrssvrTeamsMasterId = val; }

	char GetPhaseByIndex(int n){return m_aPhase[n];}
	char GetDefaultPhaseByIndex(int n){return m_aDefaultPhase[n];}

	int GetBuildingLevel(int iBuildId) ;
	int GetFactionGrass() const { return m_nFactionGrass; }
	void SetFactionGrass(int val) { m_nFactionGrass = val; }
	int GetFactionMine() const { return m_nFactionMine; }
	void SetFactionMine(int val) { m_nFactionMine = val; }
	int GetFactionMonsterFood() const { return m_nFactionMonsterFood; }
	void SetFactionMonsterFood(int val) { m_nFactionMonsterFood = val; }
	int GetFactionMonsterCore() const { return m_nFactionMonsterCore; }
	void SetFactionMonsterCore(int val) { m_nFactionMonsterCore = val; }
	int GetFactionMoney() const { return m_nFactionMoney; }
	void SetFactionMoney(int val) { m_nFactionMoney = val; }
	FacBuildField& GetMapBuildInfo() {return m_pMapBuildInfo; };
	void SetMapBuildInfo(void* pData);
	Fac_BuildField* GetBuildInfo(int nIndex);
	Fac_BuildField* GetBuildInfoTemp(int nTempIndex);
	void AddBuildInfo(int field_index,int building_tid);
	void RemoveBuildInfo(int field_index);
	void UpgradeBuildInfo(int field_index,	int building_tid,	int new_level);
	void ChangeBuildInfo(int field_index,	int building_tid,	int new_level);
	int GetFactionTask_id() const { return m_nFactionTask_id; }
	void SetFactionTask_id(int val) { m_nFactionTask_id = val; }
	int GetFactionTask_count() const { return m_nFactionTask_count; }
	void SetFactionTask_count(int val) { m_nFactionTask_count = val; }
	int GetFactionTask_need() const { return m_nFactionTask_need; }
	void SetFactionTask_need(int val) { m_nFactionTask_need = val; }
	int GetFactionGoldNote() const { return m_nFactionGoldNote; }
	void SetFactionGoldNote(int val) { m_nFactionGoldNote = val; }

	void SetBeChargeType(int type) { m_iBeChargeType = type; }
	bool IsFreeFlying();
	virtual int GetBuildLevelInConstruct();
	void SetTeleportInfo(int idSkill, int iTeleportNum);
protected:	//	Attributes

	float			m_fCameraPitch;		//	Camera pitch
	float			m_fCameraYaw;		//	Camera yaw
	A3DCoordinate	m_CameraCoord;		//	Camera coordinates
	CECCamera		m_CameraCtrl;		//	Camera control
	CECHostMove		m_MoveCtrl;			//	Move control
	int				m_iTurnCammera;		//	0x01: left button turning camera's direction; 0x02 right button
	DWORD			m_dwMoveRelDir;		//	Move relative direction flags
	DWORD			m_dwOldDir;
	float			m_fFlyHeight;		//	Fly height
	bool			m_bBeRoot;			//	Host is in root state
	bool			m_bMelee;			//	Host is in melee state
	bool			m_bTrashPsw;		//	true, trash box has password
	DWORD			m_dwLIES;			//	Logic-influence extend states
	float			m_fTransparent;		//	Transparent of host model
	bool			m_bEnterGame;		//	true, all data ready, can play game now
	float			m_fReviveExp;		//	The exp will lost if revived by other player. < 0: invalid exp
	int				m_nPKValue;			//	PK value
	bool			m_bProducing;		//	Is Producing
	int				m_nTalismanValue;	// 血炼值
	int				m_nRuneValue;		// 元魂祝福值
	int				m_nRuneCustomize;   //元婴定制信息
	int				m_nTalentValue;
	int				m_nBattleScore;
	int				m_nSJBattleScore;	//  宋金战场的积分
	int				m_iReviveCountDown; //  复活倒计时（目前用在宋金战场中）

	bool			m_bUnderEPAtk;		//	Player under attack from another player

	CECCounter		m_TaskCounter;		//	Task time counter
	CECCounter		m_TrickCnt;			//	Trick action time counter
	CECCounter		m_TransCnt;			//	Transparent counter

	CECCounter		m_GatherCnt;		//	Gather counter
	CECCounter		m_TLPosCnt;			//	Team leader position counter
	CECCounter		m_TMPosCnt;			//	Team member position counter
	CECCounter		m_UnderAtkCnt;		//	Time elapsed since last under attack time
	CECCounter		m_ReAtkCnt;			//	Time elapsed since last attacking another player
	CECCounter		m_RandomMoveCnt;	//  Counter for random move for transfigured mode
	CECCounter		m_RandomStopCnt;	//  Counter for random stop for transfigured mode 
	CECCounter		m_PerformCnt;		//  Skill Perform counter
	CECCounter		m_WingColorChangeCnt;// Wing color change counter, for example: color change  per 5min
	int				m_iRandomMoveDir;	//  direction for random move for transfigured mode
	int				m_iOperateId;		//  当前执行什么操作
	
	COOLTIME		m_aCoolTimes[GP_CT_MAX];	//	Cool times
	COOLTIME		m_aProduceCoolTimes[GP_CT_PRODUCE_END - GP_CT_PRODUCE_START];	//	Produce Cool times
	COOLTIME		m_aSkillEditCoolTimes[GP_CT_COMBINE_EDIT_END - GP_CT_COMBINE_EDIT_START];
	int				m_nTimeToExitInstance;		//	Exit current instance time left
	CECCounter		m_BindCmdCoolCnt;	//	Bind command cool time counter
	int				m_iGetFriendCnt;	//	Time counter used to get friend list
	DWORD			m_dwPariahTime;		//	Left pariah time
	CECCounter		m_PetOptCnt;		//	Pet operation time counter

	int				m_idSelTarget;		//	Selected object's ID
	int				m_idUCSelTarget;	//	Uncertificatedly selected object's ID
	int				m_idPickMatter;		//	ID of matter will be picked
	int				m_idClickedMan;		//	ID of the right button clicked player
	int				m_idSevNPC;			//	Current service NPC
	int				m_idTradePlayer;	//	ID of player who is trading with us
	int				m_idCurHover;		//	ID of object under cursor
	bool			m_bUsingTrashBox;	//	Whether being using trash box
	bool			m_bTalkWithNPC;		//	true, is talking with NPC
	bool			m_bChangingFace;	//	true, host is changing face
	bool			m_bSpellDSkill;		//	true, is spelling durative skill

	A3DVECTOR3		m_vVelocity;			//	Velocity
	A3DVECTOR3		m_vAccel;				//	Accelerate
	GNDINFO			m_GndInfo;
	int				m_iOldWalkMode;			//	Copy of work mode
	CDR_INFO*		m_pCDRInfo;
	ON_AIR_CDR_INFO*m_pAirCDRInfo;
	int				m_iJumpCount;
	bool			m_bJumpInWater;
	bool			m_bJumpByQingGong;		
	float			m_fVertSpeed;
	BREATHDATA		m_BreathData;			//	Breath data
	BATTLEINFO		m_BattleInfo;

	int				m_iTrashBoxMoneyCnt;
	bool			m_bFirstTBOpen;			//	true, it's the first time trash box is opened
	bool			m_bFirstFTBOpen;
	int				m_idBoothTrade;			//	Booth trade id	

	bool			m_bAboutToFight;		//	true means the following start attack, cast skill or atkresult is valid

	CECInventory*		m_pPack;			//	Normal package
	CECInventory*		m_pEquipPack;		//	Equipment package
	CECInventory*		m_pTaskPack;		//	Task package
	CECInventory*		m_pPetPack;
	CECInventory*		m_pPetEquipPack;
	CECDealInventory*	m_pDealPack;		//	Deal package
	CECDealInventory*	m_pEPDealPack;		//	Else player deal package
	CECDealInventory*	m_pBuyPack;			//	Buy pack 
	CECDealInventory*	m_pSellPack;		//	Sell pack
	CECInventory*		m_pSellBackPack;	//	回购待买回包裹
	CECNPCInventory*	m_aNPCPacks[NUM_NPCIVTR];		//	NPC packages
	CECReputationInventory* m_aReputationPacks[4];      //  Reputation packages
	CECInventory*		m_pTrashBoxPack;
	CECInventory*		m_pFactionTrashBoxPack;
	CECDealInventory*	m_pBoothSPack;		//	Booth pack for selling
	CECDealInventory*	m_pBoothBPack;		//	Booth pack for buying
	CECDealInventory*	m_pEPBoothSPack;	//	Else player's booth pack for selling
	CECDealInventory*	m_pEPBoothBPack;	//	Else player's booth pack for buying
	CECInventory*		m_pEPEquipPack;		//	Else player's equipment package
	CECInventory*		m_pEPPetPack;		//	Else player's pet package
	CECInventory*		m_pEPPetEquipPack;	//	Else player's pet equipment package
	CECInventory*       m_pPocketPack;		//  新增随身包裹，存放宠物生产材料
	CECInventory*		m_pFashionPack;		//	新增时装包裹，专门存放时装
	CECInventory*		m_pHTFarmPack;		//  庄园农场包裹
	CECInventory*		m_pMountWingPack;		//	新增包裹，专门存放法宝、坐骑
	CECInventory*		m_pGiftPack;		// 充值赠送物品存放包裹
	CECInventory*		m_pRune2013Pack;	// 符文2013

	int					m_iGiftScoreItem1;	// 用来计算充值积分物品1
	int					m_iGiftScoreItem2;	// 用来计算充值积分物品2*10000

	bool				m_bCarrierCooling;	//	控制向服务器发送载体相关协议频度的参数
	PosPair				m_StartPosPair;		//	carrier载体局部坐标变换相关辅助参数
	PosPair				m_EndPosPair;

	CECShortcutSet*		m_aSCSets1[NUM_HOSTSCSETS1];	//	Shortcut sets 1
	CECShortcutSet*		m_aSCSets2[NUM_HOSTSCSETS2];	//	Shortcut sets 2
	CECShortcutSet*     m_aSCSets3[NUM_HOSTSCSETS3];    //  Shortcut sets 3

	CECHPWorkMan*		m_pWorkMan;			//	Host work manager
	int					m_iDelayWork;		//	Delayed work
	CECTaskInterface*	m_pTaskInterface;	//	Task interface
	CECSkill*			m_pPrepSkill;		//	The skill prepare to be spelled
	CECFriendMan*		m_pFriendMan;		//	Friend manager
	CECPetCorral*		m_pPetCorral;		//	Pet corral
	CECComboSkill*		m_pComboSkill;		//	Combo skill
	int					m_iAllSkillLvlUp;	//	All skill's level up
	OnlineAwardManager*	m_pOnlineAwardManager;	//	Online award manager Added 2012-03-23.
	CECBattleDungeonTower* m_pBattleDungeonTower;	// 爬塔副本相关数据

	APtrArray<CECSkill*>		m_aPtSkills;	//	Positive skill array
	APtrArray<CECSkill*>		m_aPsSkills;	//	Passive skill array
	APtrArray<CECSkill*>		m_aTsSkills;	//	Transfigured skill array
	APtrArray<CECSkill*>		m_aImSkills;	//  Item skill array
	APtrArray<CECSkill*>		m_aEPSkills;	//	Else player's skill array
	CECSkill*					m_pCastTwSkills;//	当前释放爬塔技能
	AddonPermanentSkillMap		m_aAPSkills;	//	addon permanent skill array, just for search and other convience. Added 2012-02-20.
	AArray<WORD, WORD>			m_aWayPoints;	//	Active way points
	AArray<TEAMINV, TEAMINV&>	m_aTeamInvs;	//	Team invitations

#ifndef BACK_VERSION
	A3DPOST_EFFECT_RADIUSBLUR_PARAM m_AccelerateRunEffect;
	A3DPOST_EFFECT_WARP_PARAM m_UnderWaterEffect;
	A3DPOST_EFFECT_CC_PARAM m_DeathEffect;
	A3DPOST_EFFECT_DOF_PARAM m_CameraEffectLast;
	A3DPOST_EFFECT_DOF_PARAM m_CameraEffectDest;
#endif

	A3DGFXEx*		m_pMoveTargetGFX;
	A3DGFXEx*		m_pSelectedGFX;
	A3DGFXEx*		m_pHoverGFX;
	A3DGFXEx*		m_pFloatDust;
	A2DSprite*		m_pSpritePortrait;			//	The portrait sprite

	A2DSprite*		m_pDuelCountDown[3];		//	Duel count down sprites.
	A2DSprite*		m_pDuelStates[3];			//	Duel state, 0 - start, 1 - victory, 2 - lost

	A2DSprite*		m_pFactionCombatSprite;		//	Faction combat sprite;
	int				m_nFactionCombatCounts;		//	Ticks for show faction combat sprite;

	// guild war sprites.
	A2DSprite*		m_pGuildWarStartSprite;		//	Faction combat sprite;
	int				m_nGuildWarStartCounts;		//	Ticks for show guild war sprite
	A2DSprite*		m_pGuildWarWinSprite;		//	Faction combat sprite;
	int				m_nGuildWarWinCounts;		//	Ticks for show guild war sprite
	A2DSprite*		m_pGuildWarLostSprite;		//	Faction combat sprite;
	int				m_nGuildWarLostCounts;		//	Ticks for show guild war sprite
	
	AMSoundBuffer*	m_aMoveSnds[NUM_MOVESND];	//	Move sounds
	AMSoundBuffer*	m_pCurMoveSnd;				//	Current move sound

	// Direction Indicator
	CECModel *		m_pModelTray;				//	Direction indicator -- tray
	CECModel *		m_pModelMoveArrow;			//	Direction indicator -- move arrow
	CECModel *		m_pModelTempArrow;			//	Direction indicator -- temp target arrow
	CECModel *		m_pModelTargetArrow;		//	Direction indicator -- target arrow
	CECModel *		m_pModelTeamArrow;			//	Direction indicator -- team arrow
	CECModel *		m_pModelRequestArrow;		//	Direction indicator -- npc request arrow
	CECModel *		m_pModelMoveCursor;			//	Position indicator -- move cursor
	CECModel *		m_pModelTempCursor;			//	Position indicator -- temp target cursor
	CECModel *		m_pModelTargetCursor;		//	Position indicator -- target cursor
	CECModel *		m_pModelTeamCursor;			//	Position indicator -- team cursor
	CECModel *		m_pModelRequestCursor;		//	Position indicator -- npc request cursor

	bool			m_bShowMoveArrow;			//	Flag indicate whether or not show move arrow indicator
	bool			m_bShowTempArrow;			//	Flag indicate whether or not show temp target arrow indicator
	bool			m_bShowTeamArrow;			//	Flag indicate whether or not show team arrow indicator
	bool			m_bShowRequestArrow;		//	Flag indicate whether or not show npc request arrow indicator
	A3DVECTOR3		m_vecMoveArrow;				//	Direction of move arrow indicator
	A3DVECTOR3		m_vecTempArrow;				//	Direction of temp target arrow indicator
	A3DVECTOR3		m_vecTeamArrow;				//	Direction of team arrow indicator
	A3DVECTOR3		m_vecRequestArrow;			//	Direction of npc request arrow indicator
	AArray<A3DVECTOR3, const A3DVECTOR3&> m_vecTargetArrows;//  Directions of target arrow indicator
	A3DVECTOR3		m_vecMovePos;				//	Position of move arrow indicator
	A3DVECTOR3		m_vecTempPos;				//	Position of temp target arrow indicator
	A3DVECTOR3		m_vecTeamPos;				//	Position of team arrow indicator
	A3DVECTOR3		m_vecRequestPos;			//	Position of npc request arrow indicator
	AArray<A3DVECTOR3, const A3DVECTOR3&> m_vecTargetPos;//  Positions of target arrow indicator

	CECCameraPath*	m_pCameraPath;
	CECCutscene*	m_pCutscene;

	// Host faction info
	int				m_iLoyalty;					//	Loyalty
	ACString		m_strNickname;				//	Nick name

	// Host skill data has been inited
	bool			m_bSkillDataInited;			//	flag indicates skill data has been set
	bool			m_bTransfigureSkillDataInited; // flag indicates transfigure skill data has been set
	bool			m_bCombineSkillDataInited;
	// Host recipe info
	abase::vector<unsigned short> m_RecipeData;
	int m_nProduceSkillLevel;
	int m_nProduceSkillExp;

	int m_nCash;
	int m_nCashAdd; // 充值数
	int m_nCashConsumed;
	int m_AgentTime;
	// Current emotion set
	int				m_nCurEmotionSet;
	abase::vector<short> m_vecTitles;
	abase::vector<int> m_vecRegionRepu;	//	RegionReputation
	int m_nContribution;
	int m_nFamilyContrb;

	// Combat faction
	int m_nCombatFaction;

	//kingdom title
	char  m_cKingTitle;
	//kingdom id
	int   m_iKingId;
	//kingdom points
	int   m_iKingPoints;
	//kingdom battle
	char  m_cKingdomWarType;
	//Time counter used to get kingtitle
	int	  m_iGetKingTitleCnt;	
	//
	bool  m_bInitKingTitle; 

	bool  m_bIsBathing;

	bool			m_bInWar;
	WAR_INFO		m_WarInfo;

	RebornInfoVec	m_RebornInfo;

	// Host pet info
	int m_nCurPetId;
	int m_nCurPetIndex;
	int m_nCurPetMaxHp;
	int m_nCurPetMaxVp;
	int m_nPetCivilization;
	int m_nPetConstruction;
	int m_nPetCombinedType;
	
	bool m_bSafeLock;

	APOISESKILL m_ApoiseSkill;
	FASHION_HOTKEY m_aFashionHotKey[FASHION_HOTKEY_NUM];
	
	ACString	m_strSelfReferenceCode;
	ACString	m_strReferralCode;

	int m_iBonusAmount;
	int m_iBonusUsed;
	unsigned int m_iCurSkillKeyCode;
	TRANSFIGUREPROP m_TransfigureProp;
	abase::hash_map<int,int> m_TsSkillCoolDown;
	bool		m_bFlyingOff;		// 正在起飞
	CECCounter	m_FlyingOffCnt;		// 起飞动作计时器
	abase::hash_map<unsigned short,bool> m_ScriptFinishTaskMap;
	CECCounter  m_SkillCoolCnt;		// 技能冷却提示的计时器
	int			m_nTerritoryBattle;	// 是否在领土战战场中
	int			m_nRaidBattle;		// 所在副本类型，0为不在副本中
	int			m_iLastLogoutTime;
	A3DVECTOR3  m_vCachePos;
	bool		m_bHasCachePos;
//	DWORD m_dwHideEquipMaskBackUp;
//	int m_aBackUpEquips[SIZE_EQUIPIVTR];
	//Added 2011-06-17. 太昊职业的麻痹和迅疾技能所用
	float		m_fSkillSpeedRate;
	//Added end.
	//Added 2011-06-20.
	CircleOfDoomTable	m_tbGFXTable;//结阵成员id与对应gfx指针的表,对应多个结阵阵法。
	CircleOfDoomGfxNameTable	m_tbGfxName;//阵眼/阵法选区（图腾）/阵法链接光效所对应的gfx名字列表,每个阵法对应一个，以阵眼ID为index
	//Added end.

	//Added 2011-07-08.
	JailCircleAreaTable	m_tbJailCircleAreaTable; //囚牢id与对应单个囚牢信息的hash表，可对应多个囚牢。
	//Added end.

	//Added 2011-07-28.
	DWORD m_dwLastMoveTime; //用于拉扯/牵引 移动时计算速度
	//Added end.

	//Added 2011-08-02.
	bool m_bSpecialCharge;	//用于标示玩家当前处于持续按键状态，目前仅仅用于太昊拉扯的持续按键
	PullGfxInfoTable m_tbPullGfxInfo;//拉扯/牵引技能的特效gfx等信息列表	
	//Added end.

	// Tab 选怪还是选玩家， 0-怪和玩家；  1-仅怪；  2-仅玩家
	int m_nTabSelOrder;

	LITTLE_PET_INFO m_LittlePetInfo;	//猪小仙信息

	//Added 2012-05-04. 
	bool			m_bStartPosSkill;	//用于标示玩家当前是否处于启动无目标释放技能的点选位置阶段
	int				m_nPosSkillID;		//用于记录当前玩家准备释放的无目标释放技能ID
	A3DGFXEx*		m_pCastPosSkillGFX;	//用于显示无目标释放技能的特效
	//Added end.

	ChainGfxInfoTable	m_tbChainGfxInfo;	// 链式攻击技能特效列表

	abase::vector<int>  m_vecAcitveUiTransId; // 传送阵激活点

	bool	m_bAstrologyEnergyActive;	// 星座精力槽是否激活
	int		m_nAstrologyEnergy;			// 星座精力值
	
	// 2012-8-22 add by zy 添加玩家战场状态
	int m_nBattleFlag;	

	// 2012-11-7 add by zy 

	int m_nCur_point;       // 当前的分数      
	int m_bIsTaken[4];        // 今天是否领取过了  

	abase::vector<int>  m_vecLiveness; // 活跃度已激活的数据

	int m_propAdd[PROP_ADD_NUM]; // 基础属性增益

	bool m_bIsBattleWait;
	
	bool	m_bGTInTeam;
	bool	m_bGTInGuild;

	int m_nCrssvrTeamsId;
	int m_nCrssvrTeamsMasterId;
	char m_aPhase[PHASE_NUM];
	char m_aDefaultPhase[PHASE_NUM];

	int m_nFactionGrass;//获取帮派的灵草数量
	int m_nFactionMine; //获取帮派的矿石数量
	int m_nFactionMonsterFood; //获取帮派的兽粮数量
	int m_nFactionMonsterCore;//获取帮派兽核的数量
	int m_nFactionMoney;//获取帮派元宝的数量
	int m_nFactionTask_id;			// 任务ID // 检出所需任务ID
	int m_nFactionTask_count;		// 任务总数
	int m_nFactionTask_need;		// 任务需要数量
	FacBuildField m_pMapBuildInfo;
	int m_nFactionGoldNote;		// 帮派金劵

	bool m_BetaunTed;    //是否处于被嘲讽状态
	CECCounter m_BetauntedCnt; //嘲讽计时器
	int  m_iTaunterId;

	int	 m_iBeChargeType;
	CECIvtrItem *m_pItemWingOrg;	// 自由飞行前原始翅膀
	int	m_iFreeFlyState;			// 0:没有自由飞行

protected:	//	Operations

	//	Load host resources
	bool LoadResources();
	//	When all resources are ready, this function is called
	virtual void OnAllResourceReady();
	virtual bool SetPetLoadResult(CECModel* pPetModel);
	//	When weapon has been changed, this function will be called
	virtual void OnWeaponChanged();

	//	Create inventories
	bool CreateInventories();
	//	Create shortcut sets
	bool CreateShortcutSets();
	//	Update item shortcut when item position changed
	void UpdateMovedItemSC(int tidItem, int iSrcIvtr, int iSrcSlot, int iDstIvtr, int iDstSlot);
	//	Update item shortcut when item removed
	void UpdateRemovedItemSC(int tidItem, int iIvtr, int iSlot, int* aExcSlots=NULL, int iNumExcSlot=0);
	//	Update item shortcut when two items exchanged
	void UpdateExchangedItemSC(int tidItem1, int iIvtr1, int iSlot1, int tidItem2, int iIvtr2, int iSlot2);
	//	Update shortcuts
	void UpdateShortcuts(DWORD dwDeltaTime);
	//	Release skills
	void ReleaseSkills();
	//	Load sounds
	bool LoadSounds();
	//	Release sounds
	void ReleaseSounds();
	//	Load duel images
	bool LoadDuelImages();
	//	Release duel images
	void ReleaseDuelImages();
	//	Load dir indicators
	bool LoadDirIndicators();
	//	Release dir indicators
	bool ReleaseDirIndicators();

	//	Cast skill
	bool CastSkill(int idTarget, bool bForceAttack, CECObject* pTarget=NULL, A3DVECTOR3* pCastSkillPos = NULL);	//pCastSkillPos:无目标释放技能也走这个接口，转入位置信息 Modified 2012-05-04.
	//	Estimate mouse cursor
	void EstimateCursor();
	//	Update time counters
	void UpdateTimers(DWORD dwDeltaTime);
	//	Update GFXs
	void UpdateGFXs(DWORD dwDeltaTime);
	//	Attack an object
	void NormalAttackObject(int idTarget, bool bForceAttack, bool bMoreClose=false);
	//	Pickup an dobject
	void PickupObject(int idTarget, bool bGather);
	//	Check whether host can gather specified matter
	bool CanGatherMatter(CECMatter* pMatter);
	//	Interactor with object.
	void InteractWithObject(int idTarget);
	//	Update the NO-TARGET cast skill states Added 2012-05-05.
	void UpdateNoTargetSkillStates();
	//	Init the NO-TARGET cast skill gfx info. Added 2012-05-07.
	void InitNoTargetSkillGFXInfo( CECSkill* pSkill );

	void ClearApoiseSkill();

	void StartLanding();

public:
#ifndef BACK_VERSION
	bool StartAccelerateRun();
	bool TickAccelerateRun(DWORD dwDeltaTime);
	void StopAccelerateRun();
	const A3DPOST_EFFECT_RADIUSBLUR_PARAM& GetAccelerateRunEffect() const { return m_AccelerateRunEffect; }

	bool TickUnderWaterEffect(DWORD dwDeltaTime);
	const A3DPOST_EFFECT_WARP_PARAM& GetUnderWaterEffect() const { return m_UnderWaterEffect; }

	bool TickDeathEffect(DWORD dwDeltaTime);
	const A3DPOST_EFFECT_CC_PARAM& GetDeathEffect() const { return m_DeathEffect; }

	bool TickCameraEffect(DWORD dwDeltaTime);
	const A3DPOST_EFFECT_DOF_PARAM& GetCameraEffect() const { return m_CameraEffectLast; }
#endif
	
	//	Message handler
	void OnMsgGstMove(const ECMSG& Msg);
	void OnMsgGstPitch(const ECMSG& Msg);
	void OnMsgGstYaw(const ECMSG& Msg);
	void OnMsgGstMoveAbsUp(const ECMSG& Msg);
	void OnMsgHstPushMove(const ECMSG& Msg);
	void OnMsgHstPitch(const ECMSG& Msg);
	void OnMsgHstYaw(const ECMSG& Msg);
	void OnMsgLBtnClick(const ECMSG& Msg);
	void OnMsgRBtnClick(const ECMSG& Msg);
	void OnMsgRBtnClickForOtherUse( const ECMSG& Msg );
	void OnMsgHstWheelCam(const ECMSG& Msg);
	void OnMsgHstCamDefault(const ECMSG& Msg);
	void OnMsgHstCamPreset(const ECMSG& Msg);
	void OnMsgHstCamUserGet(const ECMSG& Msg);
	void OnMsgHstCamUserSet(const ECMSG& Msg);
	void OnMsgHstAttackResult(const ECMSG& Msg);
	void OnMsgHstAttacked(const ECMSG& Msg);
	void OnMsgHstDied(const ECMSG& Msg);
	void OnMsgHstPickupMoney(const ECMSG& Msg);
	void OnMsgHstPickupItem(const ECMSG& Msg);
	void OnMsgHstReceiveExp(const ECMSG& Msg);
	void OnMsgHstInfo00(const ECMSG& Msg);
	void OnMsgHstGoto(const ECMSG& Msg);
	void OnMsgHstOwnItemInfo(const ECMSG& Msg);
	void OnMsgHstSelTarget(const ECMSG& Msg);
	void OnMsgHstFixCamera(const ECMSG& Msg);
	void OnMsgHstExtProp(const ECMSG& Msg);
	void OnMsgHstAddStatusPt(const ECMSG& Msg);
	void OnMsgHstJoinTeam(const ECMSG& Msg);
	void OnMsgHstLeaveTeam(const ECMSG& Msg);
	void OnMsgHstNewTeamMem(const ECMSG& Msg);
	void OnMsgHstItemOperation(const ECMSG& Msg);
	void OnMsgHstTrashBoxOperation(const ECMSG& Msg);
	void OnMsgHstPocketOperation(const ECMSG& Msg);     //新增随身包裹
	void OnMsgHstFashionPackOpt(const ECMSG& Msg);
	void OnMsgHstMountWingPackOpt(const ECMSG& Msg);
	void OnMsgHstTeamInvite(const ECMSG& Msg);
	void OnMsgHstTeamReject(const ECMSG& Msg);
	void OnMsgHstTeamMemPos(const ECMSG& Msg);
	void OnMsgHstEquipDamaged(const ECMSG& Msg);
	void OnMsgHstTeamMemPickup(const ECMSG& Msg);
	void OnMsgHstNPCGreeting(const ECMSG& Msg);
	void OnMsgHstTradeStart(const ECMSG& Msg);
	void OnMsgHstTradeRequest(const ECMSG& Msg);
	void OnMsgHstTradeMoveItem(const ECMSG& Msg);
	void OnMsgHstTradeCancel(const ECMSG& Msg);
	void OnMsgHstTradeSubmit(const ECMSG& Msg);
	void OnMsgHstTradeConfirm(const ECMSG& Msg);
	void OnMsgHstTradeEnd(const ECMSG& Msg);
	void OnMsgHstTradeAddGoods(const ECMSG& Msg);
	void OnMsgHstTradeRemGoods(const ECMSG& Msg);
	void OnMsgHstIvtrInfo(const ECMSG& Msg);
	void OnMsgHstStartAttack(const ECMSG& Msg);
	void OnMsgHstGainItem(const ECMSG& Msg);
	void OnMsgHstPurchaseItems(const ECMSG& Msg);
	void OnMsgHstSpendMoney(const ECMSG& Msg);
	void OnMsgHstItemToMoney(const ECMSG& Msg);
	void OnMsgHstRepair(const ECMSG& Msg);
	void OnMsgHstUseItem(const ECMSG& Msg);
	void OnMsgHstUseItemWithData(const ECMSG& Msg);
	void OnMsgHstSkillData(const ECMSG& Msg);
	void OnMsgHstEmbedItem(const ECMSG& Msg);
	void OnMsgHstClearTessera(const ECMSG& Msg);
	void OnMsgHstCostSkillPt(const ECMSG& Msg);
	void OnMsgHstLearnSkill(const ECMSG& Msg);
	void OnMsgHstFlySwordTime(const ECMSG& Msg);
	void OnMsgHstProduceItem(const ECMSG& Msg);
	void OnMsgHstBreakItem(const ECMSG& Msg);
	void OnMsgHstTaskData(const ECMSG& Msg);
	void OnMsgHstTargetIsFar(const ECMSG& Msg);
	void OnMsgHstCameraMode(const ECMSG& Msg);
	void OnMsgHstPressCancel(const ECMSG& Msg);
	void OnMsgHstRootNotify(const ECMSG& Msg);
	void OnMsgHstStopAttack(const ECMSG& Msg);
	void OnMsgHstJump(const ECMSG& Msg);
	void OnMsgHstHurtResult(const ECMSG& Msg);
	void OnMsgHstAttackOnce(const ECMSG& Msg);
	void OnMsgHstPlayTrick(const ECMSG& Msg);
	void OnMsgHstSkillResult(const ECMSG& Msg);
	void OnMsgHstSkillAttacked(const ECMSG& Msg);
	void OnMsgHstAskToJoinTeam(const ECMSG& Msg);
	void OnMsgHstFaction(const ECMSG& Msg);
	void OnMsgHstSect(const ECMSG& Msg);
	void OnMsgHstTaskDeliver(const ECMSG& Msg);
	void OnMsgHstItemIdentify(const ECMSG& Msg);
	void OnMsgHstSanctuary(const ECMSG& Msg);
	void OnMsgHstCorrectPos(const ECMSG& Msg);
	void OnMsgHstFriendOpt(const ECMSG& Msg);
	void OnMsgHstWayPoint(const ECMSG& Msg);
	void OnMsgHstBreathData(const ECMSG& Msg);
	void OnMsgHstSkillAbility(const ECMSG& Msg);
	void OnMsgHstCoolTimeData(const ECMSG& Msg);
	void OnMsgHstRevivalInquire(const ECMSG& Msg);
	void OnMsgHstSetCoolTime(const ECMSG& Msg);
	void OnMsgHstChangeTeamLeader(const ECMSG& Msg);
	void OnMsgHstExitInstance(const ECMSG& Msg);
	void OnMsgHstChangeFace(const ECMSG& Msg);
	void OnMsgHstTeamMemberData(const ECMSG& Msg);
	void OnMsgHstSetMoveStamp(const ECMSG& Msg);
	void OnMsgHstChatroomOpt(const ECMSG& Msg);
	void OnMsgHstMailOpt(const ECMSG& Msg);
	void OnMsgHstVendueOpt(const ECMSG& Msg);
	void OnMsgHstViewOtherEquips(const ECMSG& Msg);
	void OnMsgHstPariahTime(const ECMSG& Msg);
	void OnMsgHstPetOpt(const ECMSG& Msg);
	void OnMsgHstBattleOpt(const ECMSG& Msg); 
	void OnMsgHstInstancingOpt(const ECMSG& Msg);    //新增场景战场
	void OnMsgHstRaidOpt( const ECMSG& Msg );		// 副本相关协议处理逻辑 Added 2011-07-21.
	void OnMsgHstBeMoved( const ECMSG& Msg );		// 处理主角玩家被拉扯/牵引的移动协议 Added 2011-07-27.
	void OnMsgHstPullOpt( const ECMSG& Msg );		// 处理玩家拉扯/牵引的相关协议消息 Added 2011-07-29.
	void OnMsgHstOnlineAwardOpt( const ECMSG& Msg );// 处理主角玩家在线奖励相关协议 Added 2012-02-16.
	void OnMsgHstAccountPoint(const ECMSG& Msg);
	void OnMsgHstGMOpt(const ECMSG& Msg);
	void OnMsgHstAutoSelect(const ECMSG& Msg);
	void OnMsgPlayerBaseInfo(const ECMSG& Msg);
	void OnMsgPlayerCustomData(const ECMSG& Msg);
	void OnMsgPlayerFly(const ECMSG& Msg);
	void OnMsgPlayerSitDown(const ECMSG& Msg);
	void OnMsgPlayerStartAtk(const ECMSG& Msg);
	void OnMsgPlayerAtkResult(const ECMSG& Msg);
	void OnMsgPlayerCastSkill(const ECMSG& Msg);
	void OnMsgPlayerGather(const ECMSG& Msg);
	void OnMsgPlayerDoEmote(const ECMSG& Msg);
	void OnMsgDoConEmote(const ECMSG& Msg);
	void OnMsgPickupMatter(const ECMSG& Msg);
	void OnMsgPlayerChangeShape(const ECMSG& Msg);
	void OnMsgBoothOperation(const ECMSG& Msg);
	void OnMsgPlayerTravel(const ECMSG& Msg);
	virtual void OnMsgPlayerPVP(const ECMSG& Msg);
	void OnMsgPlayerUseItem(const ECMSG& Msg);
	void OnMsgPlayerChangeFace(const ECMSG& Msg);
	void OnMsgPlayerBindOpt(const ECMSG& Msg);
	void OnMsgPlayerBathOpt(const ECMSG& Msg);
	void OnMsgPlayerQilinBindOpt(const ECMSG& Msg);
	void OnMsgPlayerMultiBindOpt(const ECMSG& Msg);
	void OnMsgPlayerCarrierOpt(const ECMSG& Msg);	// 新增载体（电梯）相关
	void OnMsgPlayerDuelOpt(const ECMSG& Msg);
	virtual void OnMsgPlayerLevel2(const ECMSG& Msg);
	void OnMsgPlayerSkillResult(const ECMSG& Msg);
	void OnMsgPlayerAchievement(const ECMSG& Msg);
	void OnMsgPlayerStartInteract(const ECMSG& Msg);
	void OnMsgPlayerInInteract(const ECMSG& Msg);
	void OnMsgPlayerEndInteract(const ECMSG& Msg);
	void OnMsgPlayerFinishInteract(const ECMSG& Msg);
	void OnMsgPlayerCancelInteract(const ECMSG& Msg);
	void OnMsgTransfigureSkillData(const ECMSG& Msg);
	void OnMsgHstTerritoryOpt(const ECMSG& Msg);
	void OnMsgHstCombineSkillData(const ECMSG& Msg);
	void OnMsgHstCircleOpt(const ECMSG& Msg);
	void OnMsgHstAddonPermanentSkill( const ECMSG& Msg );
	void OnMsgHst5YearAnni( const ECMSG& Msg );
	void OnMsgHstSellRoleSuccess( const ECMSG& Msg );
	void OnMsgHstTreasureRegionOpt( const ECMSG& Msg );
	void OnMsgHstTowerRaidOpt(const ECMSG& Msg);	//Added 2012-04-17.
	void OnMsgHstLittlePet(const ECMSG& Msg);
	void OnMsgHstRuneInfo(const ECMSG& Msg);
	void OnMsgHstUIFunction(const ECMSG& Msg);
	void OnMsgHstUiTransferOpen(const ECMSG& Msg);  //Added 2012-08-21 上古大陆传送阵已激活的传送点
	void OnMsgHstNotifyAstrologyEnergy(const ECMSG& Msg);
	void OnMsgHstAstrologyUpgradeResult(const ECMSG& Msg);
	void OnMsgHstAstrologyDestoryResult(const ECMSG& Msg);
	void OnMsgHstKingdomOpt(const ECMSG& Msg);
	void OnMsgHstTalismanServiceOpt(const ECMSG& Msg);

	void OnMsgHstRandomMove(const ECMSG& Msg);
	void OnMsgMoveRecord(const ECMSG& Msg);
	void OnMsgTraceRecord(const ECMSG& Msg);
	void OnMsgFollowRecord(const ECMSG& Msg);
	void OnMsgWalkRecord(const ECMSG& Msg);
	void OnMsgPushRecord(const ECMSG& Msg);
	void OnMsgCheckRecord(const ECMSG& Msg);
	void OnMsgHstFallRecord(const ECMSG& Msg);
	void OnMsgHstFriendCallBckOpt(const ECMSG& Msg);   //快雪时晴系统消息处理机制	
	void OnMsgHstRemedy_MetempsyChosisLevel(const ECMSG& Msg);   // 2012-10-18 add 弥补飞升值 by zy	
	void OnMsgLiveness(const ECMSG& Msg);   // 2012-11-8 add  活跃度 by zy	
	void OnMsgHstAncientRaidOpt(const ECMSG& Msg);
	void OnMsgSalePromotionInfo(const ECMSG& Msg);
	void OnMsgHstEventOpt(const ECMSG& Msg);     // 开启与关闭控制器事件
	void OnMsgHstPropAddResult(const ECMSG& Msg);		 // 玩家基础属性增益
	void OnMsgHstFuWenOpt(const ECMSG& Msg);			 // 符文2013 
	virtual void OnMsgHstFlowBattlePlayerInfo(const ECMSG& Msg); // 流水席
	void OnMsgHstExchangeItemList(const ECMSG& Msg);  //包裹整理物品类表
	void OnMsgHstWebOrderInfo(const ECMSG& Msg);	//WEB商城订单信息
	void OnMsgHstOffLineInfo(const ECMSG& Msg);		/// 战场离线消息
	void OnMsgHstControlTrapInfo(const ECMSG& Msg); //可控陷阱信息
	void OnMsgHstNpcInvisible(const ECMSG& Msg);	//NPC是否可见
	void OnMsgHstPhaseInfo(const ECMSG& Msg);      //相位信息
	void OnMsgFashionColor(const ECMSG& Msg);			// 染色剂合成与时装合成
	void OnMsgFacBuild(const ECMSG& Msg);			//帮派建设
	void OnMsgHstBeTaunted2(const ECMSG & Msg);	  //被嘲讽
	void OnMsgHstInviteActiveEmote(const ECMSG& Msg);	//被邀请做互动表情
	void OnMsgHstReplyActiveEmote(const ECMSG& Msg);	//回复表情邀请
	void OnMsgPlayerPlayActiveEmote(const ECMSG& Msg);	//做互动表情
	void OnMsgPlayerStopActiveEmote(const ECMSG& Msg);	//停止互动表情
	void OnMsgBePlayerChargePre(const ECMSG& Msg);        // 被冲锋位置预算
	void OnMsgTravelFree(const ECMSG& Msg);        // 自由飞行
protected:

	// 交互
	virtual	bool StartInteract(int iObj, int pos);
	virtual bool InInteract(int iObj, int pos);
	virtual bool EndInteract(int iObj, int pos);
	virtual bool FinishInteract(int iObj, int pos);
	virtual bool CancelInteract(int iObj);

	// 互动表情
	virtual bool PlayActiveEmote(ACTIVEEMOTEINFO info);

	//	Camera pitch
	void CameraPitch(float fDelta);
	//	Camera yaw
	void CameraYaw(float fDelta);
	//	Get push direction
	bool GetPushDir(A3DVECTOR3& vPushDir, DWORD dwMask, float fDeltaTime = 0);
	//	Can host touch target ?
	bool CanTouchTarget(const A3DAABB& aabbTarget);
	//	Can host touch target ?
	bool CanTouchTarget(const A3DVECTOR3& vTargetPos, float fTargetRad, int iReason, float fMaxCut=1.0f);
	//	Calculate distance between host and specified object
	bool CalcDist(int idObject, float* pfDist, CECObject** ppObject=NULL);
	//	Update selected target
	void UpdateSelectedTarget();
	//	Update pet skill cooldown
	void UpdatePetSkillCoolDown(DWORD dwTick);
	//	Estimate move environment
	void EstimateMoveEnv(const A3DVECTOR3& vPos);

	//	Fill NPC packs
	void FillNPCPack(int iIndex, const ACHAR* szName, int* aItems, float fPriceScale, bool bRecipe);
	//	Fill reputation packs
	void FillReputationPack(int iIndex, NPC_REPUTATION_SHOP_SERVICE* pData);
	//	Find mine tool in packages
	bool FindMineTool(int tidMine, int* piPack, int* piIndex, int* pidTool, int stateid);
	//	When host is going to move, this function will be called
	void GoingMove();
	//	Play move sound
	void PlayMoveSound(int iIndex);
	//	Update move sound
	void UpdateMoveSound();
	//	Adjust host's transparence
	void AdjustTransparence(float fDistToCam, const A3DVECTOR3& vDir, DWORD dwTime);
	//	Update team member's position
	void UpdateTeamMemberPos(DWORD dwDeltaTime);
	//	Rearrange booth packages
	void RearrangeBoothPacks();
	//	Stop host moving naturally
	bool NaturallyStopMoving();
	//	Reset cdr info
	void ResetCDRInfo();
	//	Sort title
	void SortTitle();
	//  Only For Lua Trigger Task process 
	void ProcessChildTask(int idTask);

	//	Tab sel
	bool TabSelPlayer();
	bool TabSelMonster();
	
	// 获取一定范围内距离目标idTarget最近且不在列表中的敌人
	int GetNearestEnemy(int idTarget, float fRange, abase::vector<int>& vecTargets);
	
	void ResetKingdomInfo();

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

