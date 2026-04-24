/*
 * FILE: EC_Cutscene.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: SunXuewei, 2010/04/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Zhuxian Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Bezier.h"
#include "EC_Counter.h"
#include "EC_DataExpectation.h"
#include "EC_CameraPath.h"
#include "EC_InputCtrl.h"
#include "EC_Configs.h"

#include "EC_NPC.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define MAX_NPC_ACTIONLIST	8
#define MAX_CAMERA_PATH		32

#define CUTSCENE_TBL_NAME ("Cutscene")
#define DISPLAYSETTING_TBL_NAME ("DisplaySettings")
#define SUBTITLE_TBL_NAME ("Subtitle")
#define NPCCONFIG_TBL_NAME ("NPCConfig")
#define ACTIONGROUP_TBL_NAME ("ActionGroup")
#define BACKGROUNDMUSIC_TBL_NAME ("BackGroundMusic")
#define BGIMAGE_TBL_NAME ("BackGroundImage")
#define ONLYSHOWONCE_TBL_NAME ("OnlyShowOnceCutscenes")

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////
struct NPC_ACTIONGROUP
{
	unsigned int	timeStart;
	unsigned int	timeEnd;
	bool			isMoving;
	bool			isLoopPlay;
	bool			isRandomPlay;
	unsigned long	actionList[MAX_NPC_ACTIONLIST];
	
};
struct CUTSCENE_NPC
{
	unsigned int	m_ulTemplId;
	unsigned int	m_ulShowUpTime;
	unsigned int	m_ulDisappearTime;
	unsigned int	m_npcPathId;
	CECNPC*			m_pNPC;
	abase::vector<NPC_ACTIONGROUP> m_npcActionGroup;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

class CECConfigs;

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECCutscene
//	
///////////////////////////////////////////////////////////////////////////

class CECCutscene
{
public:		//	Types
	
public:		//	Constructor and Destructor

	CECCutscene();
	~CECCutscene();

public:		//	Attributes

public:		//	Operations
	bool Init();
	void Tick(DWORD dwDeltaTime);

	int	GetID() { return m_id; }
	int GetSubtitleID() { return m_idSubtitle; }

	bool StartCutscene(DWORD dwCutsceneIndex, bool bNeedCheck = true);
	//为了使用之前的播放记录，将新的动画播放在此处
	void StartAnimation(DWORD dwAnmId,bool bNeedCheck = true);
	void EndCutscene();
	bool IsPlayingCutscene();
	bool HasDelayedCutscene();
	bool HasBeenPlayed(DWORD dwCutsceneIndex);		// whether this cutscene has been played. Used for those cutscenes that only show once.
	void SaveHasBeenPlayedConfig(DWORD dwCutsceneIndex);

	void InitMaskData();
	
	inline bool CanEscClose() { return m_bCanEscClose; }
	inline bool IsOtherKeyDisabled() { return m_bDisableOtherKey; }
	inline bool IsUIShow() { return m_bShowUI; }
	inline bool IsSubtitleShow() { return m_bShowSubtitle; }
	inline bool IsElsePlayerShow() { return m_bShowElsePlayers; }
	inline bool IsSelfShow() { return m_bShowSelf; }
	inline bool IsNPCShow() { return m_bShowNPC; }
	inline bool IsMonsterShow() { return m_bShowMonster; }
	inline bool IsNameShow() { return m_bShowName; }
	inline bool IsBgImageShow() { return m_bShowBgImage; }

	CECCameraPath* GetCameraPath() { return m_pCameraPath; }

protected:		//	Operations
//	void _DelayCameraPath(const unsigned int pathIndex);
//	void _ProcessDelayedCamera();

	bool _CheckCutscene();
	void _OnStartCutscene(DWORD dwCutsceneIndex);
	void _OnStopCutscene();
	
protected:	//	Attributes
	
	int					m_id;
	DWORD				m_dwTimeElapsed;
	
	int					m_idCutsceneMaskData[4];

	bool				m_bIsPlayingCutscene;
	int					m_idSubtitle;
	
	int					m_iCurrCameraPathIndex;
	unsigned long		m_ulCameraPathIDList[MAX_CAMERA_PATH];	
	unsigned long		m_ulCameraPathStayTime[MAX_CAMERA_PATH];	

	abase::vector<CUTSCENE_NPC> m_NPCList;
//	abase::vector<CUTSCENE_PLAYER> m_PlayerList;


	// Display settings
	bool m_bOnlyShowOnce;
	bool m_bCanEscClose;
	bool m_bDisableOtherKey;
	bool m_bShowUI;
	bool m_bShowSubtitle;
	bool m_bShowElsePlayers;
	bool m_bShowSelf;
	bool m_bShowNPC;
	bool m_bShowMonster;
	bool m_bShowName;
	bool m_bCancelAutoPath;

	bool m_bLastShowUI;
	CECCameraPath*		m_pCameraPath;
	
	int			m_idBgImage;
	AWString	m_strSubtitle;
	
	bool		m_bTurnOffSound;
	int			m_idMusic;
	AWString	m_strMusic;

	bool m_bShowBgImage;
	AWString	m_strImage;
	int			m_iShadowTime;

	CECCounter	m_ExitCounter;	

	EC_SYSTEM_SETTING	m_OldSettings;
	bool				m_bSettingChanged;
	
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////