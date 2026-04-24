/*
 * FILE: EC_ManPlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#ifdef BACK_VERSION
#include "EC_Manager.h"
#else
#include "ECManager.h"
#include "ECManagerID.h"
#endif
#include "EC_GPDataType.h"
#include "EC_Counter.h"
#include "AArray.h"
#include "hashtab.h"
#include "vector.h"
#include "EC_Player.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

namespace GNET
{
	class Octets;
	class GamedataSend;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECPlayer;
class CECHostPlayer;
class CECElsePlayer;
class CECEPCacheData;
class CECLoginPlayer;
class CECModel;
class A3DSkinModel;

//	Player laod result structure used by CECPlayer::LoadPlayerModel
struct EC_PLAYERLOADRESULT
{
	CECModel*	pPlayerModel;
	CECModel*	pDummyModel;
	CECModel*	pPetModel;
	CECModel*	pChangedModel;
	CECModel*	pMirrorModel[6];	// 镜像模型最大为6个
	CECModel*   pTransfiguredModel;
	CECModel*	pAircraftModel;
	CECModel*   pPuppetModel;
	CECModel*	pInteractorModel;
	A3DSkinModel* pPortraitModel;
	A3DShader*	pBodyShaders[3];

	CECPlayer::EquipsLoadResult	EquipResult[6];  // 加载镜像模型时，装备加载6套，其他只用1套
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

#define FASHION_WORDCOLOR_TO_A3DCOLOR(c) A3DCOLORRGB(((c) & (0x1f << 10)) >> 7, ((c) & (0x1f << 5)) >> 2, ((c) & 0x1f) << 3)
#define FASHION_A3DCOLOR_TO_WORDCOLOR(c) ((((c) & 0x00f10000) >> 19) | (((c) & 0x0000f100) >> 11) | (((c) & 0x000000f1) >> 3));

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPlayerMan
//	
///////////////////////////////////////////////////////////////////////////

#ifdef BACK_VERSION
class CECPlayerMan : public CECManager
#else
class CECPlayerMan : public ECManager
#endif
{
public:		//	Types

	//	Loaded player model information
	struct PLAYERMODEL
	{
		int	cid;					//	Player's character ID
		DWORD dwBornStamp;
		bool bOnlyEquip;
		bool bOnlyPet;
		bool bOnlyChangedModel;
		bool bOnlyTransfiguredModel;
		bool bOnlyAircraft;
		bool bOnlyMirrorModel;
		bool bOnlyMirrorEquip;
		bool bOnlyPuppetModel;
		EC_PLAYERLOADRESULT Ret;	//	Model loaded result
	};

	typedef abase::hashtab<CECElsePlayer*, int, abase::_hash_function>	PlayerTable;
	typedef abase::hashtab<CECEPCacheData*, int, abase::_hash_function>	CacheTable;
	typedef abase::vector<CECLoginPlayer *>	LoginPlayerTable;
	typedef abase::hashtab<int, int, abase::_hash_function> UkPlayerTable;

public:		//	Constructor and Destructor

	CECPlayerMan(CECGameRun* pGameRun);
	virtual ~CECPlayerMan();

public:		//	Attributes

public:		//	Operations

	//	Release manager
	virtual void Release();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport);
	//	RenderForReflected routine
	virtual bool RenderForReflect(CECViewport * pViewport);
	//	RenderForRefract routine
	virtual bool RenderForRefract(CECViewport * pViewport);
	//	RenderForBloom routine
	virtual bool RenderForBloom(CECViewport * pViewport);
#ifndef BACK_VERSION
	//  RenderForShadow routine
	virtual bool RenderForShadow(ECViewport* pViewport);
	virtual bool RenderPost(ECViewport* pViewport) { return true; }
#endif
	//	Process message
	virtual bool ProcessMessage(const ECMSG& Msg);

	//	On entering game world
	virtual bool OnEnterGameWorld();
	//	On leaving game world
	virtual bool OnLeaveGameWorld();

	//	Ray trace
	virtual bool RayTrace(ECRAYTRACE* pTraceInfo);
	// Trace for CD, and get the players collided.
	bool TraceWithBrush(BrushTraceInfo * pInfo, APtrArray<CECElsePlayer*>& playerArray );
	// Update the collision detect states.
	void UpdateAllCollisionStates(DWORD dwDeltaTime);
	// Update the collision detection states, called by each elseplayer when they do special move.
	bool UpdateCollisionStates( CECElsePlayer* pPlayer, float fDeltaTime );
	// Update the Host player's collision detection states with all other else players. Currently just used for special move.
	bool UpdateHostCollisionStates( float fDeltaTime );

	inline PlayerTable& GetPlayerTable();
	inline LoginPlayerTable& GetLoginPlayerTable() { return m_LoginPlayers; }

	//	Get host player
	CECHostPlayer* GetHostPlayer();
	//	Get else player by id
	CECElsePlayer* GetElsePlayer(int cid, DWORD dwBornStamp=0);
	//	Get a player (may be host or else player) by id
	CECPlayer* GetPlayer(int cid, DWORD dwBornStamp=0);
	//	Tick host player separately
	bool TickHostPlayer(DWORD dwDeltaTime, CECViewport* pViewport);
	//	Get else player number
	int GetElsePlayerNum() { return (int)m_PlayerTab.size(); }
	//	Query player's booth name
	void QueryPlayerBoothName(int idPlayer) { m_aBoothQuery.Add(idPlayer); }
	//	Get booth model
	A3DSkinModel* GetBoothModel(int iGender) { return iGender == GENDER_MALE ? m_pBoothModel_M : m_pBoothModel_F; }
	abase::hash_map<int, CECModel*>* GetSpeBoothModel() {return &m_BoothModelMap;}
	CECModel* LoadSpeBoothModel(int tid);	

	//	Get else player cache data
	CECEPCacheData* GetPlayerCacheData(int cid);
	//	Get players should appear in mini-map
	const APtrArray<CECElsePlayer*>& GetPlayersInMiniMap() { return m_aMMPlayers; }
	//	When world's loading center changed
	void OnLoadCenterChanged();

	//	Load player model in loading thread
	bool ThreadLoadPlayerModel(int cid, DWORD dwBornStamp, int iPhysique, int iProfession, int iLevel, int iEarid, int iTailid, int iFaceid, int iHairid, 
		int iDefaultSkin, int iGender, const int* pEquips, const char* szPetPath, const char* szChangedPath);
	bool ThreadLoadPlayerEquips(int cid, DWORD dwBornStamp,int iPhysique, int iProfession, int iDefaultSkin, int iGender, const int* pEquips, DWORD dwEquipMask);
	bool ThreadLoadMirrorPlayerEquips(int cid, DWORD dwBornStamp,int iPhysique, int iProfession, int iDefaultSkin, int iGender, const int* pEquips, DWORD dwEquipMask);
	bool ThreadLoadPetModel(int cid, DWORD dwBornStamp, const char* szPetPath);
	bool ThreadLoadChangedModel(int cid, DWORD dwBornStamp, const char* szPath);
	bool ThreadLoadTransfiguredModel(int cid, DWORD dwBornStamp, const char* szPath);
	bool ThreadLoadAircraftModel(int cid, DWORD dwBornStamp, const char* szPath);
	bool ThreadLoadMirrorModel(int cid, DWORD dwBornStamp, int iPhysique, int iProfession, int iLevel, int iEarid, int iTailid, int iFaceid, int iHairid, 
		int iDefaultSkin, int iGender, const int* pEquips, const char* szPetPath, const char* szChangedPath);
	bool ThreadLoadPuppetModel(int cid, DWORD dwBornStamp, const char* szPuppetPath);
	//	Auto-select an enemy faction's member through 'Tab' key
	int TabSelectEnemy(int idCurSel);

	GNET::GamedataSend gen_player_list();
	GNET::Octets gen_player_cache_list();
	void create_cache_from_record(GNET::Octets& o);

	// Get nearest player of specified position
	int GetNearestEnemyPlayer(const A3DVECTOR3& vTargetPos, float fRange, abase::vector<int> vecTargets, float* fDistToTarget);

protected:	//	Attributes

	PlayerTable			m_PlayerTab;	//	Players in view area
	UkPlayerTable		m_UkPlayerTab;	//	Unknown player table
	CacheTable			m_PlayerCache;	//	Player cache
	CECCounter			m_QueryCnt;		//	Query unknown player time counter
	CECCounter			m_CacheCnt;		//	Cache garbage collector time counter
	CECCounter			m_TLTimeCnt;	//	Tank leader update time counter

	A3DSkinModel*		m_pBoothModel_M;	//	Model used when player open booth
	A3DSkinModel*		m_pBoothModel_F;
	LoginPlayerTable	m_LoginPlayers;	//	Players in login UI, just for render here

	APtrArray<CECElsePlayer*>	m_aMMPlayers;		//	Player will appear in mini-maps
	APtrArray<PLAYERMODEL*>		m_aLoadedModels;	//	Loaded player models
	CRITICAL_SECTION			m_csLoad;			//	Loaded models lock
	AArray<int, int>			m_aBoothQuery;		//	Used to query player's booth name

	APtrArray<CECElsePlayer*>	m_aTabSels;			//	'Tab' key selected enemies

	abase::hash_map<int, CECModel*> m_BoothModelMap;

protected:	//	Operations

	//	Create else player
	CECElsePlayer* CreateElsePlayer(const S2C::info_player_1& Info, int iAppearFlag);
	//  Create player's clone body  
	CECElsePlayer* CreateElsePlayer(const S2C::info_npc& Info, int iAppearFlag);
	//	Release else player
	void ReleaseElsePlayer(CECElsePlayer* pPlayer);

	//	One else player enter view area
	CECElsePlayer* ElsePlayerEnter(const S2C::info_player_1& Info, int iCmd);
	//  One player's clone body enter view area
	CECElsePlayer* ElsePlayerEnter(const S2C::info_npc& Info);
	//	One else player leave view area or exit game
	void ElsePlayerLeave(int cid, bool bExit, bool bUpdateMMArray=true);
	//	One else player's more information arrived
	bool MoreElsePlayerInfo2(int cid, const S2C::info_player_2& Info);
	//	One else player's more information arrived
	bool MoreElsePlayerInfo3(int cid, const S2C::info_player_3& Info);
	//	One else player's more information arrived
	bool MoreElsePlayerInfo4(int cid, const S2C::info_player_4& Info);
	//	Host information arrived
	bool HostPlayerInfo1(const S2C::cmd_self_info_1& Info);
	//	Update players in various ranges (Active, visible, mini-map etc.)
	void UpdatePlayerInRanges(DWORD dwDeltaTime);
	//	Remove else player from m_aMMPlayers array
	void RemovePlayerFromMiniMap(CECElsePlayer* pPlayer);  
	//	Remove player from tab-selected array
	void RemovePlayerFromTabSels(CECElsePlayer* pPlayer);

	//	Pop player from cache
	CECEPCacheData* PopPlayerFromCache(int cid);
	//	Push player to cache
	bool PushPlayerToCache(CECEPCacheData* pData);
	//	Clear up player data cache
	void ClearUpPlayerCache();
	//	Is player in cache ?
	inline bool IsPlayerInCache(int cid);
	//	Seek out else player with specified id, if he doesn't exist, try to get from server
	CECElsePlayer* SeekOutElsePlayer(int cid);
	//	Update unknown else player table
	void UpdateUnknownElsePlayers();
	//	Deliver loaded player models
	void DeliverLoadedPlayerModels();

public:

	//	Messages handlers
	bool OnMsgPlayerInfo(const ECMSG& Msg);
	bool OnMsgPlayerMove(const ECMSG& Msg);
	bool OnMsgPlayerRunOut(const ECMSG& Msg);
	bool OnMsgPlayerExit(const ECMSG& Msg);
	bool OnMsgPlayerDied(const ECMSG& Msg);
	bool OnMsgPlayerRevive(const ECMSG& Msg);
	bool OnMsgPlayerOutOfView(const ECMSG& Msg);
	bool OnMsgPlayerDisappear(const ECMSG& Msg);
	bool OnMsgPlayerStopMove(const ECMSG& Msg);
	bool OnMsgPlayerLevelUp(const ECMSG& Msg);
	bool OnMsgPlayerExtProp(const ECMSG& Msg);
	bool OnMsgPlayerLeaveTeam(const ECMSG& Msg);
	bool OnMsgDoConEmote(const ECMSG& Msg);
	bool OnMsgInvalidObject(const ECMSG& Msg);
	bool OnMsgPlayerExtState(const ECMSG& Msg);
	void OnMsgPickUpMatter(const ECMSG& Msg);
	void OnMsgPlayerBindStart(const ECMSG& Msg);
	void OnMsgPlayerDuelResult(const ECMSG& Msg);
	void OnMsgTankLeader(const ECMSG& Msg);
	void OnMsgPlayerScaleShape(const ECMSG& Msg); //Added 2011-06-28.
	void OnMsgPlayerVIPAwardUpdate(const ECMSG& Msg); //Added 2012-01-16.
	void OnMsgPlayerSpecialMoveOpt(const ECMSG& Msg); //Added 2012-06-27.
	void OnMsgPlayerChangeName(const ECMSG& Msg); //Added 2012-07-30.
	void OnMsgPlayerCollisionSpecialState(const ECMSG& Msg); //Added 2012-08-02.
	void OnMsgPlayerXYDarkLight(const ECMSG& Msg);	//Added 2012-08-16.
	void OnMsgPlayerWingChangeColor(const ECMSG& Msg);	//Added 2012-08-20.
	void OnMsgDirVisibleState(const ECMSG& Msg);		//Added 2012-09-05.
	void OnMsgPlayerBathStart(const ECMSG& Msg);
	void OnMsgPlayerGetBathAward(const ECMSG& Msg);
	void OnMsgPlayerPuppetForm(const ECMSG& Msg);
	void OnMsgPlayerQilinBindStart(const ECMSG& Msg);
	void OnMsgPlayerPuppetFormChange(const ECMSG& Msg);
	void OnMsgPlayerBlowOff(const ECMSG& Msg);
	void OnMsgPlayerBeCharge(const ECMSG& Msg);
	void OnMsgPlayerBloodPool(const ECMSG& Msg);
	void OnMsgPlayerTeleportInfo(const ECMSG& Msg);

	void OnMsgPlayerXYDarkLightSpirit(const ECMSG& Msg);
	void OnMsgPlayerXYMirrorImageCnt(const ECMSG& Msg);
	
	void ProcessNPC2PlayerMsg(const S2C::info_npc& Info);
	void ProcessNPC2PlayerMsg(int nid, int killerid);
	void ProcessNPC2PlayerMsg(S2C::cmd_npc_info_00& Info);
	//	Transmit message
	bool TransmitMessage(const ECMSG& Msg);

	void OnMsgHstFlowBattlePlayerInfo(const ECMSG& Msg);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

//	Is player data in cache ?
bool CECPlayerMan::IsPlayerInCache(int cid)
{
	CacheTable::pair_type Pair = m_PlayerCache.get(cid);
	return Pair.second;
}

CECPlayerMan::PlayerTable& CECPlayerMan::GetPlayerTable()
{
	return m_PlayerTab;
}
