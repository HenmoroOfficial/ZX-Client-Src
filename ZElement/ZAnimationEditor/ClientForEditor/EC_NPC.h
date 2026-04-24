/*
 * FILE: EC_NPC.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_MsgDef.h"
#include "EC_StringTab.h"
#include "EC_Object.h"
#include "EC_RoleTypes.h"
#include "EC_Counter.h"
#include "EC_CDS.h"
#include "A3DGeometry.h"
#include "AAssist.h"
#include "CDWithCH.h"
#include "aabbcd.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MONSTER_ATTACK_SPEED	2000

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class A3DViewport;
class A3DSkin;
#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif
class CECModel;
class CECSkill;
class CECPateText;
class CECBubbleDecalList;

struct MONSTER_ESSENCE;
struct EC_NPCLOADRESULT;
struct OtherPlayer_Move_Info;

namespace S2C
{
	struct info_npc;
	struct cmd_object_move;
	struct cmd_object_stop_move;
}

namespace GNET
{
	class Octets;
}

using namespace CHBasedCD;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECNPC
//	
///////////////////////////////////////////////////////////////////////////

class CECNPC : public CECObject
{
public:		//	Types

	//	NPC type
	enum
	{
		NPC_UNKNOWN = -1,
		NPC_MONSTER = 0,
		NPC_SERVER,
		NPC_PET,
	};

	//	NPC action index
	enum
	{
		ACT_STAND = 0,
		ACT_IDLE,
		ACT_GUARD,
		ACT_SKILL1,
		ACT_WALK,
		ACT_ATTACK1,
		ACT_ATTACK2,
		ACT_RUN,
		ACT_DIE,
		ACT_JUMP_START,
		ACT_JUMP_LAND,
		ACT_JUMP_LOOP,
		ACT_MAGIC1,
		ACT_WOUNDED,
		ACT_NPC_CHAT1,
		ACT_NPC_CHAT2,
		ACT_NPC_CHATLOOP,
		ACT_NPC_IDLE1,
		ACT_NPC_IDLE2,
		ACT_NPC_STAND,
		ACT_NPC_WALK,
		ACT_NPC_RUN,
		ACT_NPC_ATTACK,
		ACT_NPC_DIE,
		ACT_COMMON_BORN,
		ACT_NPC_GREETING,
		ACT_NPC_DISAPPEAR,
		ACT_NPC_PETFIGHT1,
		ACT_NPC_PETFIGHT2,
		ACT_NPC_PETFIGHT3,
		// 技能动作名 for 变身技能播放
		ACT_NPC_SKILL_ATTACK1,
		ACT_NPC_SKILL_ATTACK2,
		ACT_NPC_SKILL_ATTACK3,
		ACT_NPC_SKILL_ATTACK4,
	};

	//	Render Name Flag
	enum
	{
		RNF_NPCNAME		= 0x01,
		RNF_MONSTERNAME	= 0x02,
		RNF_WORDS		= 0x04,
		RNF_ALL			= 0x07,
	};

	//	Move environment
	enum
	{
		MOVEENV_GROUND = 0,	//	Move on ground
		MOVEENV_WATER,
		MOVEENV_AIR,
	};

	//	Work ID
	enum
	{
		WORK_STAND = 1,		//	Stand and do nothing
		WORK_FIGHT,			//	Fighting
		WORK_SPELL,			//	Monster is cast skill
		WORK_DEAD,			//	Monster is dead
		WORK_MOVE,			//	Move to a destination terrain position
	};

	//	Work type
	enum
	{
		WT_NOTHING = 0,		//	Do thing
		WT_NORMAL,			//	Normal type work
		WT_INTERRUPT,		//	Interrupt type work
		NUM_WORKTYPE,
	};

	//	NPC basic states
	enum
	{
		STATE_NORMAL	= 0,
		STATE_FLY		= 0x10,
		STATE_SITDOWN	= 0x20,
		STATE_CORPSE	= 0x80,
	};

	//	Bubble text
	enum
	{
		BUBBLE_DAMAGE = 0,
		BUBBLE_SKILL_DAMAGE,
		BUBBLE_HITMISSED,
		BUBBLE_INVALIDHIT,
		BUBBLE_IMMUNE,
		BUBBLE_RESIST,
		BUBBLE_HPWARN,
		BUBBLE_SKILL_SPEC,
		BUBBLE_DEITY_DAMAGE,
	};

	// owner type
	enum 
	{
		OWNER_NONE = 0,		// 无
		OWNER_SELF,			// 自己
		OWNER_MASTER,		// 师傅
		OWNER_APPRENTICE, 	// 徒弟
		OWNER_SP0USE,		// 夫妻
		OWNER_TEAM,			// 队伍
		OWNER_TEAM_OR_SELF,	// 队伍或自己
		OWNER_TEAM_AND_SELF,// 队伍和自己
		OWNER_ANY,			// 怪的名字属于刷怪的人，但是没有特殊的攻击逻辑
		OWNER_FAMILY,		// 家族（保留）
		OWNER_MAFIA,		// 帮派（保留）
	};

	//	NPC information got from server
	struct INFO
	{
		int		nid;	//	NPC id
		int		tid;	//	Template id
	};

	//	Pate content render info
	struct PATECONTENT
	{
		int		iVisible;	//	Visible flag. 0, not set; 1, not visible; 2, visible
		int		iBaseX;		//	Base x
		int		iBaseY;		//	Base y
		int		iCurY;		//	Current y
		float	z;			//	z value
		float	rhw;
	};
	//	Buff info
	struct BUFFINFO
	{
		int		id;			//	Buff id
		int		iLevel;		//	Buff level
	};

	friend class CECNPCMan;

public:		//	Constructor and Destructor

	CECNPC(CECNPCMan* pNPCMan);
	virtual ~CECNPC();

public:		//	Attributes

public:		//	Operations

	//	Initlaize object
	virtual bool Init(int tid, const S2C::info_npc& Info);
	//	Release object
	virtual void Release();

	//	Set loaded model to NPC object, this function is used in multithread loading process
	virtual bool SetNPCLoadedResult(const EC_NPCLOADRESULT& Ret);

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0) { return InternalRender(pViewport, iRenderFlag, false); }
	virtual bool RenderHighLight(CECViewport* pViewport) { return InternalRender(pViewport, RD_NORMAL, true); }
	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	Set absolute position
	virtual void SetPos(const A3DVECTOR3& vPos);
	//	Set absolute forward and up direction
	virtual void SetDirAndUp(const A3DVECTOR3& vDir, const A3DVECTOR3& vUp);

	//	Move to a destination
	virtual void MoveTo(const S2C::cmd_object_move& Cmd);
	//	Stop moving to a destination position
	virtual void StopMoveTo(const S2C::cmd_object_stop_move& Cmd);
	//	NPC was killed
	virtual void Killed(bool bDelay);
	virtual void Disappear(bool bAtOnce = false);
	//	Stop attack
	virtual void StopAttack();
	//	NPC is damaged
	virtual void Damaged(int iDamage, int nDivisions, DWORD dwModifier=0);
	//	Play an attack effect
	void PlayAttackEffect(int idTarget, int idSkill, int skillLevel, unsigned char cSerialId, int nDamage, DWORD dwModifier);
	//	The function that will call into CECObject::TurnFaceTo()
	void NPCTurnFaceTo(int idTarget, DWORD dwTime=200);

	//	Get NPC name color
	virtual DWORD GetNameColor() { return 0xffffc000; }
	//	Is monster in invader camp in battle ?
	virtual bool IsInBattleInvaderCamp() { return false; }
	//	Is monster in defender camp in battle ?
	virtual bool IsInBattleDefenderCamp() { return false; }
	//  该npc是否可以被攻击，解决了战场中怪物的选中，表面是npc，其实是怪物monster。
	virtual bool IsAttackable() { return false; }
	
	//  人神阵营 人神pk
	virtual bool IsHumanCamp() { return false; }
	virtual bool IsOrcCamp()  { return false; }
	
	// 被选中策略，0可以被正常选中，1 tab键不可选中，2任何条件下不能被选中
	int GetSelectStrategy();

	virtual float GetSize() { return 0.0f; }

	//	Get NPC name
	const ACHAR* GetName() { return m_strName; }
	//	Get NPC information got from server
	const INFO& GetNPCInfo() { return m_NPCInfo; }
	//	Get basic properties
	ROLEBASICPROP& GetBasicProps() { return m_BasicProps; }
	//	Get extend properties
	ROLEEXTPROP& GetExtendProps() { return m_ExtProps; }
	//	Get NPC ID
	int GetNPCID() { return m_NPCInfo.nid; }
	//	Get NPC template ID
	int GetTemplateID() { return m_NPCInfo.tid; }
	//	Get NPC's aabb used to pick
	const A3DAABB& GetPickAABB();
	//	Get NPC state
	bool IsDead() { return (m_dwStates & STATE_CORPSE) ? true : false; }
	bool IsFlying() { return (m_dwStates & STATE_FLY) ? true : false; }
	bool IsSitting() { return (m_dwStates & STATE_SITDOWN) ? true : false; }
	bool IsMoving();
	//	Get NPC's real position on server
	const A3DVECTOR3& GetServerPos() { return m_vServerPos; }

	void SetServerPos(const A3DVECTOR3 vec) { m_vServerPos = vec; }
	void SetMoveDir(const A3DVECTOR3 vec) { m_vMoveDir = vec; }
	
	//	Whether this NPC should be removed
	bool ShouldDisappear() { return m_DisappearCnt.IsFull(); }
	//	Set NPC's transparence
	void SetTransparence(float fCur, float fDest, DWORD dwTime);
	//	Is the NPC a pet ?
	bool IsPet() { return m_idMaster ? true : false; }
	//	Get master id
	int GetMasterID() { return m_idMaster; }
	//  Get Summoner id
	int GetSummonerID() { return m_idSummoner; }
	// Get pet star
	char GetPetStar() { return m_cPetStar; }
	//	Set last said words
	void SetLastSaidWords(const ACHAR* szWords, int nEmotionSet);	//Modified 2012-04-25.
	
	bool IsAboutToDie() { return m_bAboutToDie; }
	void SetAboutToDie(bool bFlag) { m_bAboutToDie = bFlag; }
	//	Set / Get name rendering flag
	void SetRenderNameFlag(bool bTrue) { m_bRenderName = bTrue; }
	bool GetRenderNameFlag() { return m_bRenderName; }
	//	Set / Get bars rendering flag
	void SetRenderBarFlag(bool bTrue) { m_bRenderBar = bTrue; }
	bool GetRenderBarFlag() { return m_bRenderBar; }

	//	Set extend state
	void SetExtState(int n) { m_dwExtStates |= ( UINT64(1) << n); }
	//	Remove extend state
	void RemExtState(int n) { m_dwExtStates &= ~( UINT64(1) << n); }
	//	Check extend state
	bool GetExtState(int n) { return (m_dwExtStates & (UINT64(1) << n)) ? true : false; }
	//	Get icon state number
	int GetIconStateNum() { return m_aIconStates.GetSize(); }
	//	Get icon state
	BUFFINFO& GetIconState(int n) { return m_aIconStates[n]; }
	//	Get inherent random properties
	int GetRandomProp() { return m_iRandomProp; }

	//	Get touch radius
	float GetTouchRadius() { return m_fTouchRad; }
	//	Judge NPC type
	bool IsServerNPC() { return (DistinguishID(m_NPCInfo.tid) == NPC_SERVER) ? true : false; }
	bool IsMonsterNPC() { return (DistinguishID(m_NPCInfo.tid) == NPC_MONSTER) ? true : false; }
	bool IsPetNPC() { return (DistinguishID(m_NPCInfo.tid) == NPC_PET) ? true : false; }
	bool IsCarrierNPC() { return (m_iCID == OCID_CARRIER) ? true : false; }
	//	Get distance to host player
	float GetDistToHost() { return m_fDistToHost; }
	float GetDistToHostH() { return m_fDistToHostH; }
	//	Get distance to camera
	float GetDistToCamera() { return m_fDistToCamera; }

	//	Print bubble text
	void BubbleText(int iIndex, DWORD dwNum, int p1=0, bool bLastOne=true, bool bRandPos=false);

	//	Initialize static resources
	static bool InitStaticRes();
	//	Release static resources
	static void ReleaseStaticRes();
	//	Distinguish a NPC id. Return 0 if it's monster NPC id. 1 if it's server
	//	NPC id. -1 if it's not a NPC id
	static int DistinguishID(int tid);
	//	Load NPC model
	static bool LoadNPCModel(int tid, const char* szFile, EC_NPCLOADRESULT& Ret);
	//	Release NPC model
	static void ReleaseNPCModel(EC_NPCLOADRESULT& Ret);

	//	Play model action
	bool PlayModelAction(int iAction, float fWeight, bool bRestart=true, int iTransTime=200);
	//	Play move action
	void PlayMoveAction(int iMoveMode);
	//	Move to destination position
	bool MovingTo(DWORD dwDeltaTime);
	void EnterDimState(unsigned char state);
	void UpdateSpecialState(int state, bool on);

	//增加缩放NPC的接口Added 2011-08-17.
	bool ScaleNPCAndGFX( float fScale = 1.0f );
	//Added end.

protected:	//	Attributes

	static CECStringTab		m_ActionNames;	//	Action name table
	static CECStringTab		m_SkillActs;	//	Skill action name table

	CECNPCMan*		m_pNPCMan;			//	NPC manager
	CECModel*		m_pNPCModel;		//	NPC model
	A3DSkin*		m_pNPCSkin;			//	NPC skin of normal lod
	A3DSkin*		m_pNPCSkinLow;		//	NPC skin of low lod
	A3DSkin*		m_pCurSkin;			//	NPC skin current shown
	INFO			m_NPCInfo;			//	NPC information got from server
	ROLEBASICPROP	m_BasicProps;		//	Basic properties
	ROLEEXTPROP		m_ExtProps;			//	Extend properties
	float			m_fTouchRad;		//	Touch radius
	bool			m_bCastShadow;		//	flag indicates whether it will cast shadow
	A3DAABB			m_aabbDefPick;		//	Default aabb used to pick
	int				m_iMMIndex;			//	Index in CECNPCMan::m_aMMNPCs array
	DWORD			m_dwStates;			//	NPC's basic states
	__int64			m_dwExtStates;		//	Visible extend states
	CECCounter		m_DisappearCnt;		//	Disappear time counter
	int				m_iRandomProp;		//	NPC's inherent random properties
	DWORD			m_dwISTimeCnt;		//	Icon state time counter
	int				m_idMaster;			//	id of master if the NPC is a pet
	int				m_idSummoner;		//  id of summoner if the npc is summoned by a player
	char			m_cOwnerType;		//  the npc is belong to specified player or team
	int				m_idOwner;			//  id of the npc belonged to
	char			m_cPetStar;			//	pet star
	ACString		m_strName;			//	Name of NPC
	ACString		m_strDesc;			//  NPC description text, show below the name.
	ACString		m_strOwnerName;		//  NPC Owner name, show if the npc belonged to specified player or team

	float			m_fCurTrans;		//	Current transparence
	float			m_fDstTrans;		//	Target transparence
	float			m_fTransDelta;		//	Transparence delta
	CECCounter		m_TransCnt;			//	Transparence time counter

	bool			m_bAboutToDie;		//	a object_attack_result with deadly strike flag has been received
	CECCounter		m_IdleCnt;			//	Idle time counter
	
	float			m_fDistToHost;		//	Distance to host player
	float			m_fDistToHostH;		//	Horizontal distance to host player
	float			m_fDistToCamera;	//	Distance to camera

	A3DVECTOR3		m_vMoveDir;			//	Move direction
	A3DVECTOR3		m_vServerPos;		//	NPC position on server
	A3DVECTOR3		m_vStopDir;			//	The direction after moving stopped the NPC will face to
	float			m_fMoveSpeed;		//	Move speed
	int				m_iMoveEnv;			//	Move environment
	bool			m_bStopMove;		//	Stop move flag
	bool			m_bPushBack;		//	Push back flag

	int				m_aWorks[NUM_WORKTYPE];		//	Work array
	int				m_iCurWork;					//	Current work's id
	int				m_iCurWorkType;				//	Current work type

	int				m_iAction;					//	Current action being played

//	CECSkill*		m_pCurSkill;		//	The skill NPC is casting
	CECCounter		m_SkillCnt;			//	Time counter for casting skill
	CECCounter		m_LODCnt;			//	Time counter for model's LOD
	int				m_nFightTimeLeft;	//	Work Fight time left
	int				m_idAttackTarget;	//	Attack target
	int				m_idCurSkillTarget;	//	Spell target
	bool			m_bStartFight;		//	Start fight flag

	bool			m_bRenderName;		//	Render name flag
// 	CECPateText*	m_pPateName;		//	NPC name
// 	CECPateText*	m_pPateDesc;		//  NPC description text
// 	CECPateText*	m_pPateOwner;		//  NPC Owner name 
	bool			m_bRenderBar;		//	Render HP, MP, EXP bars
	PATECONTENT		m_PateContent;		//	Pate content
// 	CECPateText*	m_pPateLastWords1;	//	The words player said last time, line 1
// 	CECPateText*	m_pPateLastWords2;	//	The words player said last time, line 2
	int				m_iLastSayCnt;		//	Time counter of last said words

	CECBubbleDecalList*		m_pBubbleTexts;	//	Bubble text list

	AArray<BUFFINFO>		m_aIconStates;	//	Icon states (un-visible extend states)
	OtherPlayer_Move_Info*	m_pcdr;

	int				m_nBrushes;			//	number of brush object used in collision
	CCDBrush**		m_ppBrushes;		//	Brush object used in collision
	
	MONSTER_ESSENCE*	m_pMonEssence;

	bool			m_bIsMovieNPC;		//  Whether this npc is movie npc 
	bool			m_bDim;				//  朦胧状态
	int				m_iSpecialState;    // 由buff附加的状态，以后统一放在这里面进行处理

protected:	//	Operations

	//	Load NPC model
	bool LoadNPCModel(const char* szFile);
	//	Release NPC model
	void ReleaseNPCModel();

	//	Render titles / names / talks above player's header
	bool RenderName(CECViewport* pViewport, DWORD dwFlags);
	//	Render bar above player's header
	bool RenderBars(CECViewport* pViewport);
	//	Set new visible extend states
	void SetNewExtendStates(__int64 dwNewStates);
	//	Fill pate content
	bool FillPateContent(CECViewport* pViewport);
	//	Internal render routine
	bool InternalRender(CECViewport* pViewport, int iRenderFlag, bool bHighLight);
	//	Is monster or pet
	bool IsMonsterOrPet() { return (OCID_MONSTER == m_iCID || OCID_PET == m_iCID); }


	//	Move step
	A3DVECTOR3 MoveStep(const A3DVECTOR3& vDir, float fSpeed, float fTime);

	//	Start a work of specified type
	void StartWork(int iWorkType, int iNewWork, DWORD dwParam=0);
	//	Current work finished
	void WorkFinished(int iWorkID);
	//	Stop / pause work of specified type
	void StopWork(int iWorkType);
	//	Release work resources of specified type
	void ReleaseWork(int iWorkType);

	//	Start works 
	void StartWorkByID(int iWorkID, DWORD dwParam);
	virtual void StartWork_Stand(DWORD dwParam);
	virtual void StartWork_Fight(DWORD dwParam);
	virtual void StartWork_Spell(DWORD dwParam);
	virtual void StartWork_Dead(DWORD dwParam);
	virtual void StartWork_Move(DWORD dwParam);

	//	Work tick routine
	virtual void TickWork_Stand(DWORD dwDeltaTime);
	virtual void TickWork_Fight(DWORD dwDeltaTime);
	virtual void TickWork_Spell(DWORD dwDeltaTime);
	virtual void TickWork_Dead(DWORD dwDeltaTime);
	virtual void TickWork_Move(DWORD dwDeltaTime);

	//	Message handlers
	void OnMsgNPCAtkResult(const ECMSG& Msg);
	void OnMsgNPCStartAttack(const ECMSG& Msg);
	void OnMsgNPCSkillResult(const ECMSG& Msg);
	void OnMsgNPCExtState(const ECMSG& Msg);
	void OnMsgNPCCastSkill(const ECMSG& Msg);
	void OnMsgNPCEnchantResult(const ECMSG& Msg);
	void OnMsgNPCRoot(const ECMSG& Msg);
	
	void BuildOwnerName();
public:
	void OnMsgAttackHostResult(int idHost, int nDamage, int nFlag, int nSpeed);
	bool RayTrace(ECRAYTRACE* pTraceInfo);
	bool TraceWithBrush(BrushTraceInfo * pInfo);
	bool RebuildTraceBrush();
	bool ReleaseTraceBrush();
	static const char* GetActionName(int iAct);
	const MONSTER_ESSENCE* GetMonsterEssence() { return m_pMonEssence; }
	bool IsHide();
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
bool NPCRenderForShadow(A3DViewport * pViewport, void * pArg);
#endif


