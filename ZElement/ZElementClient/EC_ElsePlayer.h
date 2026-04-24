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
#ifndef EC_GRAVITY
#define EC_GRAVITY	9.8f
#endif

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
	struct cmd_object_start_special_move;
	struct cmd_object_special_move;
	struct cmd_object_stop_special_move;
}

struct OtherPlayer_Move_Info;

struct CDR_INFO;

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

class CECElsePlayer;

class MovingCorrect
{
public:
	enum PlayerState
	{
		PS_STOP				= 0,	//  停止
		PS_STOP_CORRECT		= 1,	//	应该停止，位置不同步
		PS_MOVING			= 0x10,	//	正常运动中
	};
	
public:
	MovingCorrect(CECElsePlayer *p);
	~MovingCorrect() {}
	
	void Tick(DWORD dwDeltaTime);
	void CorrectPos(const A3DVECTOR3 &p);
	void SetPos(const A3DVECTOR3 &p);		// CorrectPos and stop
	void SetVA(const A3DVECTOR3 &v, const A3DVECTOR3 &a);
	
	CECElsePlayer	*m_pPlayer;
	A3DVECTOR3		m_vPos;
	A3DVECTOR3		m_vVelocity;
	A3DVECTOR3		m_vAcceleration;
	int				m_iLastRefresh;
	bool			m_bStop;		// elseplayer应该停止并平缓过渡到m_vPos
};

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
		WORK_SPECIALMOVE,	//	Special move
		WORK_BLEWUP,		//  Blew up	
		WORK_PARABOLIC,     //  Parabolic
		WORK_MOVENOCOLLISION,
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

	//	碰撞状态信息
	struct COLLISION_STATE_INFO
	{
		int			iCollisionTime;	//	碰撞时间戳
	};

	//	碰撞状态信息列表，分为客户端ElsePlayer模拟的碰撞状态和服务器HostPlayer发过来的碰撞状态
	struct COLLISION_STATE_INFO_LIST
	{
		AArray<COLLISION_STATE_INFO>	arClientCDStateList;
		AArray<COLLISION_STATE_INFO>	arServerCDStateList;
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
	GNET::Octets gen_info_player_1();
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
	//	Play an active emote action
	virtual bool PlayActiveEmote(ACTIVEEMOTEINFO info);

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
	virtual bool IsSpellingMagic();
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

	int GetCurWork(){ return m_iCurWork; }

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

	//
	A3DVECTOR3	GetVelocity()const { return m_vVelocity; }
	A3DVECTOR3	GetAcceleration()const { return m_vAcceleration; }
	void		SetVA( const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration );
	void		SetSvrVA( const A3DVECTOR3& vVelocity, const A3DVECTOR3& vAcceleration );
	void		StartSpecialMove() { StartWork( WT_NORMAL, WORK_SPECIALMOVE ); m_bStopMove = false; m_pMovingCorrect->m_bStop=false; }
	void		StopSpecialMove()	{ WorkFinished(WORK_SPECIALMOVE); m_bStopMove=true; }
	void		CheckStartSepcialMove( const S2C::cmd_object_start_special_move& specialMoveCmd );
	void		UpdateSpecialMove( const S2C::cmd_object_special_move& specialMoveCmd );
	void		StopSpecialMove( const S2C::cmd_object_stop_special_move& specialMoveCmd );
//	void		SetServerCollisionState( bool bCollision ) { m_bServerCollisionState = bCollision; }//Canceled 2012-09-01.
//	bool		GetServerCollisionState() const { return m_bServerCollisionState; }	//Canceled 2012-09-01.
	void		UpdateCollisionState( int iCollisionTime, unsigned char ucType );	//更新碰撞状态，包括更新服务器和客户端的碰撞状态列表接口ucType = 0:client更新, ucType = 1：Server更新	Added 2012-09-01.
	int			CheckMoveCmd( int iCmdTime );	//检查碰撞副本中特殊移动消息的时效性和合法性 0: 通过检测，1：没通过检测Added 2012-09-01.
	void		CheckClearCollisionState();		//隔时清除过时的碰撞不同步状态 Added 2012-09-01.
	CDR_INFO*	GetCDRInfo() const { return m_pCDRInfo; }
	void		SetCDRInfoClipVelocity(const A3DVECTOR3& vClipVelocity);
	//	Ground Move step with collosion detection.
	A3DVECTOR3 GroundMoveStep( const A3DVECTOR3& vDir, float fSpeed, float fTime, float fSpeedV = 0.0f, float fGravity = EC_GRAVITY );
	void		GetElsePos();	
	//
	void	   PrepareBlewUp(DWORD DWDuration, float fSpeed);
	void	   PrepareParabolic(DWORD dwDuration, float height, A3DVECTOR3 vecDest);
	void	   SetMoveInfo(DWORD dwStartTime, DWORD dwDuration, float fSpeed, A3DVECTOR3 vecDest, int idTarget);
	OtherPlayer_Move_Info* GetOtherPlayerMoveInfo() {return m_pcdr;}
	void		SetSpeed(float fSpeed) { m_fMoveSpeed = fSpeed;}

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
	//Added 2012-06-25.
	A3DVECTOR3	m_vVelocity;			//	the current velocity of the else player, currently used for special movement.
	A3DVECTOR3	m_vAcceleration;		//	the current acceleration of the else player, currently used for special movement.
	MovingCorrect	*m_pMovingCorrect;	//  纠正客户端模拟与实际的偏差
	//Added end.
	CDR_INFO*	m_pCDRInfo;				//	Else player CDR Info -->Just used for move step with collosion detection.
	//Added 2012-08-14.
//	bool		m_bServerCollisionState;//	Indicate the current collision state of server!!.(If the elseplayer in server is in the collide state).
	//Added end.
	//Added 2012-09-01.
	COLLISION_STATE_INFO_LIST	m_CollisionStateInfoList;	//	碰撞状态信息列表
	//Added end.
	
	DWORD		m_dwStartTime;
	DWORD		m_dwPastTime;
	DWORD		m_dwDuration;
	float		m_fSpeed;
	float		m_fSpeedV;
	float		m_fAccelerate;
	A3DVECTOR3  m_vecDest;
	float		m_fHeight;
	float		m_fDistance;
	int			m_idMoveToTarget;

	int			m_iItemWingOrg;		// 自由飞行前翅膀id，hostplayer中对应m_pItemWingOrg
	
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
	
	// 更新pPlayer的位置，保证其与这个elseplayer不相交
	void CorrectPos(CECPlayer *pPlayer);
	
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

	void ChangeFreeFlyWing();

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
	void StartWork_SpecialMove(DWORD dwParam);	//Added 2012-06-25.
	void StartWork_BlewUp(DWORD dwParam);
	void StartWork_Parabolic(DWORD dwParam);
	void StartWork_MoveNoCollision(DWORD dwParam);

	//	Work tick routine
	void TickWork_Stand(DWORD dwDeltaTime);
	void TickWork_Move(DWORD dwDeltaTime);
	void TickWork_Melee(DWORD dwDeltaTime);
	void TickWork_Spell(DWORD dwDeltaTime);
	void TickWork_Pickup(DWORD dwDeltaTime);
	void TickWork_Dead(DWORD dwDeltaTime);
	void TickWork_Idle(DWORD dwDeltaTime);
	void TickWork_FlashMove(DWORD dwDeltaTime);
	void TickWork_SpecialMove(DWORD dwDeltaTime);	//Added 2012-06-25. For the special move work of the else player. there are pridiction operations.
	void TickWork_BlewUp(DWORD dwDeltaTime);
	void TickWork_Parabolic(DWORD dwDeltaTime);
	void TickWork_MoveNoCollision(DWORD dwDeltaTime);

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
	void OnMsgPlayerBathOpt(const ECMSG& Msg);
	void OnMsgPlayerStartInteract(const ECMSG& Msg);
	void OnMsgPlayerInInteract(const ECMSG& Msg);
	void OnMsgPlayerEndInteract(const ECMSG& Msg);
	void OnMsgPlayerFinishInteract(const ECMSG& Msg);
	void OnMsgPlayerCancelInteract(const ECMSG& Msg);
	void OnMsgPlayerQilinBindOpt(const ECMSG& Msg);
	void OnMsgPlayerPlayActiveEmote(const ECMSG& Msg);
	void OnMsgPlayerStopActiveEmote(const ECMSG& Msg);
	void OnMsgTravelFree(const ECMSG& Msg);        // 自由飞行
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

