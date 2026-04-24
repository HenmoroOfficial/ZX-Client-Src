/*
 * FILE: EC_ElsePlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/10
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Player.h"

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

namespace GNET
{
	class Octets;
}

namespace S2C
{
	struct info_player_1;
	struct info_npc;
	struct cmd_object_move;
	struct cmd_object_stop_move;
	struct cmd_player_revive;
}

struct OtherPlayer_Move_Info;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECEPCacheData
//	
///////////////////////////////////////////////////////////////////////////

class CECEPCacheData
{
public:		//	Types

	//	Extend resource flags
	enum
	{
		RESFG_BOOTHNAME	= 0x010000,
	};

public:		//	Constructor and Destructor

	CECEPCacheData();

public:		//	Attributes

	int			m_cid;				//	Character ID
	ACString	m_strName;			//	Player name
	int			m_iFaceid;			//	Face ID
	int			m_iHairid;			//	Hair ID
	int			m_iEarid;			//	Ear ID
	int			m_iTailid;			//  Tail ID
	int			m_iDefaultSkin;		//  默认皮肤
	int			m_iGender;			//	Gender
	int			m_iRace;			//	Race
	int			m_iPhysique;		//  Physique
	DWORD		m_dwResFlags;		//	Resource flags
	int			m_crc_e;			//	Equipment data crc
	int			m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array

	int			m_crcBooth;			//	Booth CRC
	ACString	m_strBoothName;		//	Booth name

	DWORD		m_dwCacheTime;		//	Cache time stamp (s)

public:		//	Operations

protected:	//	Attributes

protected:	//	Operations

};

#pragma pack(1)

struct record_player_cache
{
	int			m_cid;				//	Character ID
	wchar_t		m_strName[10];
	char		m_Faceid;			//	Face ID
	char		m_Hairid;			//	Hair ID
	char		m_Earid;
	char		m_Tailid;
	char		m_DefaultSkin;
	char		m_Gender;			//	Gender
	char		m_Race;
	char		m_Physique;
	DWORD		m_dwResFlags;		//	Resource flags
	int			m_crc_e;			//	Equipment data crc
	int			m_aEquips[SIZE_EQUIPIVTR];	//	Equipment item ID array
	int			m_crcBooth;			//	Booth CRC
	wchar_t		m_strBoothName[15];
};

#pragma pack()

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECElsePlayer
//	
///////////////////////////////////////////////////////////////////////////

class CECElsePlayer : public CECPlayer
{
public:		//	Types

	//	Work ID
	enum
	{
		WORK_STAND = 1,		//	Stand and do nothing
		WORK_MOVE,			//	Move
		WORK_HACKOBJECT,	//	Hack specified object (NPC, player etc.)
		WORK_SPELL,			//	Spell magic
		WORK_PICKUP,		//	Pickup matter
		WORK_GENERAL_OPRT,	//	General operation
		WORK_DEAD,			//	Dead
		WORK_USEITEM,		//	Use item
		WORK_IDLE,			//	Idle works
		WORK_FLASHMOVE,		//	Flash move
	};

	//	Work type
	enum
	{
		WT_NOTHING = 0,		//	Do thing
		WT_NORMAL,			//	Normal type work
		WT_INTERRUPT,		//	Interrupt type work
		NUM_WORKTYPE,
	};

	//	Idle work type
	enum
	{
		IDLE_SITDOWN = 0,
		IDLE_REVIVE,
		IDLE_BOOTH,
	};

	//	Player appear flag
	enum
	{
		APPEAR_DISAPPEAR = -1,		//	Player disappear
		APPEAR_ENTERWORLD = 0,		//	Player join world
		APPEAR_RUNINTOVIEW,			//	Player run into view
		APPEAR_GHOST,				//	Player is in ghost state, in player list but not active
	};

	//	Idle work info.
	struct IDLEWORK
	{
		int			iType;		//	Idle work type
		bool		bOTCheck;	//	Overtime check
		CECCounter	OTCnt;		//	Overtime counter
	};

	friend class CECPlayerMan;
	friend class CECPlayer;

public:		//	Constructor and Destructor

	CECElsePlayer(CECPlayerMan* pPlayerMan);
	virtual ~CECElsePlayer();

public:		//	Attributes

public:		//	Operations

	//	Initlaize object
	bool Init(const S2C::info_player_1& Info, int iAppearFlag, bool bReInit);
	//  Initlaize clone body
	bool Init(const S2C::info_npc& Info, int iAppearFlag, bool bReInit);
	//	Gen info_player_1

	//	Initialize player from cached data
	bool InitFromCache(const S2C::info_player_1& Info, CECEPCacheData* pCacheData, int iAppearFlag);
	bool InitFromCache(const S2C::info_npc& Info, CECEPCacheData* pCacheData, int iAppearFlag);
	//	Release object
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0) { return InternalRender(pViewport, iRenderFlag, false); }
	bool RenderHighLight(CECViewport* pViewport) { return InternalRender(pViewport, RD_NORMAL, true); }
	//	Render when player is opening booth
	virtual bool RenderForBooth(CECViewport* pViewport, int iRenderFlag=0, bool bHighLight=false) { return RenderSimpleModels(true, pViewport, iRenderFlag, bHighLight); }
	//	Render when player using hint model
	bool RenderForHint(CECViewport* pViewport, int iRenderFlag=0, bool bHighLight=false) { return RenderSimpleModels(false, pViewport, iRenderFlag, bHighLight); }

	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	Player was killed
	virtual void Killed(int idKiller);
	//	Is player in fight state ?
	virtual bool IsFighting();
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

	//	Set loaded model to player object, this function is used in multithread loading process
	virtual bool SetPlayerLoadedResult(EC_PLAYERLOADRESULT& Ret);

	//	Unload resources
	void UnloadResources();
	//	Create player cache data
	CECEPCacheData* CreateCacheData();

	//	Move to a destination
	void MoveTo(const S2C::cmd_object_move& Cmd);
	//	Stop moving to a destination position
	void StopMoveTo(const S2C::cmd_object_stop_move& Cmd);
	//	Revive
	void Revive(const S2C::cmd_player_revive* pCmd);
	//	Level up
	void LevelUp();
	//	Enter fight state
	void EnterFightState();
	
	//	Is base info ready ?
	bool IsBaseInfoReady() { return m_bBaseInfoReady; }
	//	Is equipment data ready ?
	bool IsEquipDataReady() { return m_bEquipReady; }
	//	Get distance to host player
	float GetDistToHost() { return m_fDistToHost; }
	float GetDistToHostH() { return m_fDistToHostH; }
	//	Is spelling magic
	bool IsSpellingMagic();
	//	Turn to ghost mode
	void TurnToGhost() { m_iAppearFlag = APPEAR_GHOST; }
	//	Is player in ghost state ?
	bool InGhostState() { return m_iAppearFlag == APPEAR_GHOST ? true : false; }
	//	Is player jumping ?
	bool IsJumping() { return m_bJumping; }
	//	Get player's real position on server
	const A3DVECTOR3& GetServerPos() { return m_vServerPos; }
	//	Set server position
	void SetServerPos(const A3DVECTOR3& vPos);
	//	Get player in team state
	int GetInTeamState() { return m_iInTeamState; } 
	//	Is player moving
	virtual bool IsPlayerMoving() { return m_iCurWork == WORK_MOVE; }
	//	Set use hint model flag
	void SetUseHintModelFlag(bool bUse) { m_bUseHintModel = bUse; }
	//	Get use hint model flag
	bool GetUseHintModelFlag() { return m_bUseHintModel; }

	//	Get equipment item
	int GetEquipment(int n) { return m_aEquips[n]; }

	//	Load player equipments
	bool LoadPlayerEquipments();
	//  Copy the equips form other player
	void UpdateCurEquips(int id);
	
	virtual void DeityLevelUp(bool bSuccess, int iLevel);
	virtual void UpdateSpecialState(int state, bool on);

protected:	//	Attributes

	int			m_iAttackTarget;		//	Attack target's ID
	float		m_fDistToHost;			//	Distance to host player
	float		m_fDistToHostH;			//	Horizontal distance to host player
	
	int			m_iAppearFlag;			//	Player's appearing flag
	bool		m_bBaseInfoReady;		//	true, Basic info is ready
	bool		m_bEquipReady;			//	true, Equipment data is ready
	int			m_aNewEquips[SIZE_EQUIPIVTR];		//	New equipment item ID array
	DWORD		m_dwNewEqpMask;			//	New equipment mask
	int			m_iMMIndex;				//	Index in CECPlayerMan::m_aMMPlayers array

	int			m_aWorks[NUM_WORKTYPE];	//	Work array
	int			m_iCurWork;				//	Current work's id
	int			m_iCurWorkType;			//	Current work type
	IDLEWORK	m_IdleWork;				//	Idle work

	A3DGFXEx*	m_pAppearGFX;			//	Appear GFX
	CECCounter	m_SkillCnt;				//	Time counter for casting skill
	CECCounter	m_PickupCnt;			//	Time counter for pickup matter
	int			m_iInTeamState;			//	Player team state. 0 no team; 1 leader; 2 member;
	CECCounter	m_FightCnt;				//	Fight time counter

	bool		m_bStopMove;			//	Stop move flag
	A3DVECTOR3	m_vMoveDir;				//	Player's velocity
	A3DVECTOR3	m_vServerPos;			//	Player's real position on server
	float		m_fMoveSpeed;			//	Move speed
	float		m_fLastSpeed;			//	Move speed of last time
	DWORD		m_dwLastMoveTime;		//	Last move command arrived time
	A3DVECTOR3	m_vStopDir;				//	The direction when player stop moving
	bool		m_bJumping;				//	Jumping flag
	bool		m_bLoadingModel;		//	Model loading flag
	bool		m_bUseHintModel;		//	true, use hint model
	
	A3DVECTOR3	m_vRelServerPos;		//	The relative server pos on a carrier
	A3DVECTOR3	m_vRelStopDir;			//	The relative stop direction on a carrier

	OtherPlayer_Move_Info*	m_pcdr;

	//Added 2011-06-17.
	float		m_fSkillSpeedRate;
	//test end.	
	//Added 2011-07-22.
	float		m_fYDelta;
	//Added end.
	
protected:	//	Operations

	//	When all resources are ready, this function will be called
	virtual void OnAllResourceReady();
	virtual bool SetPetLoadResult(CECModel* pPetModel);
	//	When weapon has been changed, this function will be called
	virtual void OnWeaponChanged();
	
	//	Render routine when player use simple models
	bool RenderSimpleModels(bool bBooth, CECViewport* pViewport, int iRenderFlag, bool bHighLight);
	//	Internal render routine
	bool InternalRender(CECViewport* pViewport, int iRenderFlag, bool bHighLight);

	//	Change equipment
	bool ChangeEquipments(bool bReset, int crc, int iAddMask, int iDelMask, int* aAddedEquip);

	//	Move step
	A3DVECTOR3 MoveStep(const A3DVECTOR3& vDir, float fSpeed, float fTime);
	//	Set player info
	void SetPlayerInfo(const S2C::info_player_1& Info);
	//	Set clone body info
	void SetPlayerInfo(const S2C::info_npc& Info);
	//	Set player's brief info
	bool SetPlayerBriefInfo(int iFaceid, int iHairid, int iEarid, int iTailid, int iDefaultSkin,
		int iGender, const ACHAR* szName);
	
	//	Move to destination position
	bool MovingTo(DWORD dwDeltaTime);
	//	Start a work of specified type
	void StartWork(int iWorkType, int iNewWork, DWORD dwParam=0);
	//	Current work finished
	void WorkFinished(int iWorkID);
	//	Stop / pause work of specified type
	void StopWork(int iWorkType);
	//	Release work resources of specified type
	void ReleaseWork(int iWorkType);
	//	Set idle work information. This function should be called after startwork immediately
	void SetIdleWorkInfo(int iType, int iOTTime, DWORD dwParam);
	//	Finish idle work
	void IdleWorkFinished(int iWorkID);

	//	Start works 
	void StartWorkByID(int iWorkID, DWORD dwParam);
	void StartWork_Stand(DWORD dwParam);
	void StartWork_Move(DWORD dwParam);
	void StartWork_Melee(DWORD dwParam);
	void StartWork_Spell(DWORD dwParam);
	void StartWork_Dead(DWORD dwParam);
	void StartWork_Pickup(DWORD dwParam);
	void StartWork_GeneralOprt(DWORD dwParam);
	void StartWork_UseItem(DWORD dwParam);
	void StartWork_FlashMove(DWORD dwParam);

	//	Work tick routine
	void TickWork_Stand(DWORD dwDeltaTime);
	void TickWork_Move(DWORD dwDeltaTime);
	void TickWork_Melee(DWORD dwDeltaTime);
	void TickWork_Spell(DWORD dwDeltaTime);
	void TickWork_Pickup(DWORD dwDeltaTime);
	void TickWork_Dead(DWORD dwDeltaTime);
	void TickWork_Idle(DWORD dwDeltaTime);
	void TickWork_FlashMove(DWORD dwDeltaTime);
	
	//	Message handler
	void OnMsgPlayerFly(const ECMSG& Msg);
	void OnMsgPlayerBaseInfo(const ECMSG& Msg);
	void OnMsgPlayerBaseInfo2(const ECMSG& Msg);
	void OnMsgPlayerCustomData(const ECMSG& Msg);
	void OnMsgPlayerEquipData(const ECMSG& Msg);
	void OnMsgPlayerSitDown(const ECMSG& Msg);
	void OnMsgPlayerAtkResult(const ECMSG& Msg);
	void OnMsgPlayerStartAtk(const ECMSG& Msg);
	void OnMsgPlayerSkillResult(const ECMSG& Msg);
	void OnMsgPlayerCastSkill(const ECMSG& Msg);
	void OnMsgPlayerDoEmote(const ECMSG& Msg);
	void OnMsgPlayerUseItem(const ECMSG& Msg);
	void OnMsgPlayerUseItemWithData(const ECMSG& Msg);
	void OnMsgPlayerRoot(const ECMSG& Msg);
	void OnMsgPlayerDoAction(const ECMSG& Msg);
	void OnMsgPlayerInTeam(const ECMSG& Msg);
	void OnMsgPickupMatter(const ECMSG& Msg);
	void OnMsgPlayerGather(const ECMSG& Msg);
	void OnMsgDoConEmote(const ECMSG& Msg);
	void OnMsgPlayerChangeShape(const ECMSG& Msg);
	void OnMsgBoothOperation(const ECMSG& Msg);
	void OnMsgPlayerTravel(const ECMSG& Msg);
	void OnMsgPlayerChangeFace(const ECMSG& Msg);
	void OnMsgPlayerBindOpt(const ECMSG& Msg);
	void OnMsgPlayerDuelOpt(const ECMSG& Msg);
	void OnMsgPlayerMultiBindOpt(const ECMSG& Msg);
	void OnMsgPlayerCarrierOpt(const ECMSG& Msg);	// 新增载体（电梯）相关
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

