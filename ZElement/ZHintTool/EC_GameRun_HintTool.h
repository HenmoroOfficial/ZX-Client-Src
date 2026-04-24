/*
 * FILE: EC_GameRun.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/27
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include <Windows.h>
#include <hashtab.h>
#include <AAssist.h>
#include "EC_GPDataType.h"
#include "ExpTypes.h"

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

//class A3DVECTOR3;
class CECUIManager;
class CECWorld;
/*class CECInputCtrl;
class A3DGFXExMan;
class CECLoadProgress;
class CECFullGlowRender;
class CECSimpleBloomRender;
class CECScreenRender;
*/class CECHostPlayer;
//class CECViewport;
class CECInstance;
/*class CECTeamMan;
class CECShortcutSet;
class CECSCCommand;
class CECMessageMan;
class CECClipboard;
class CECHostInputFilter;
*/class CECHelper;    
class CECIvtrItem;
/*class CECSecretary;

struct ECMSG;

namespace GNET
{
	class RoleInfo;
}

namespace _SGC
{
	class A3DSkillGfxComposerMan;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

enum
{
	MOD_PHYSIC_ATTACK_RUNE	= 0x01,		// 物理攻击优化符生效
	MOD_MAGIC_ATTACK_RUNE	= 0x02,		// 法术攻击优化符生效
	MOD_RESIST				= 0x04,		// 抵抗
	MOD_SKILL_SPEC			= 0x08,		// 技能特色
	MOD_CRITICAL_STRIKE		= 0x10,		// 爆击
	MOD_RETORT				= 0x20,		// 反震
	MOD_NULLITY				= 0x40,		// 无效攻击
	MOD_IMMUNE				= 0x80,		// 免疫了此次攻击，优先级高于无效
	MOD_SKILL				= 0x80000000,// 技能攻击标志
};

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECGameRun
//	
///////////////////////////////////////////////////////////////////////////
*/
class CECGameRun
{
public:		//	Types

	//	Game state
	enum
	{
		GS_NONE = 0,	//	None
		GS_LOGIN,		//	Login in state
		GS_GAME,		//	In game
	};

	//	Double experence mode
	enum
	{
		DBEXP_NONE		= 0x00,
		DBEXP_OWN		= 0x01,
		DBEXP_GLOBAL	= 0x02,
	};

	// wallow stage for korea
	enum
	{
		WALLOW_STAGE0	= 0,
		WALLOW_STAGE1,
		WALLOW_STAGE2,
		WALLOW_STAGE3,
	};
	
	struct LineInfo
	{
		int id;
		ACString name;
		int attribute;
		bool bNoPKLine;		// 是否不可pk的线
	};

//	typedef abase::vector<LineInfo> LineInfoVec;

	typedef abase::hashtab<ACString*, int, abase::_hash_function> ID2NameTable;
	typedef abase::hashtab<int, ACString, abase::_hash_function> Name2IDTable;
	typedef abase::hashtab<CECInstance*, int, abase::_hash_function> InstTable;

public:		//	Constructor and Destructor

	CECGameRun();
	virtual ~CECGameRun();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init();
	//	Release object
	void Release();

	//	Start game
/*	bool StartGame(int idInst, const A3DVECTOR3& vHostPos);
	//	Start login interface
	bool StartLogin(bool bFirst);
	//	Deal windows message
	bool DealWindowsMessage(UINT message, WPARAM wParam, LPARAM lParam);

	//	Game tick routine
	bool Tick(DWORD dwDeltaTime);
	//	Game render routine
	bool Render(bool bPresent=true);
	//	Capture screen
	bool CaptureScreen(int N);

	//  Get game state.
	int GetGameState() { return m_iGameState; }
	//	Get UI manager
*/	CECUIManager* GetUIManager() { return m_pUIManager; }
	//	Get game world
	CECWorld* GetWorld() { return m_pWorld; }
	//	Get input control
/*	CECInputCtrl* GetInputCtrl() { return m_pInputCtrl; }
	//	Get host input filter
	CECHostInputFilter* GetHostInputFilter() { return m_pInputFilter; }
	//	Whether UI has cursor
	bool UIHasCursor() { return m_bUIHasCursor; }
	//	Save necessary user configs (UI, shortcut, accelerate keys) to server
	bool SaveConfigsToServer(bool bRecord = false);
	//	Load necessary user configs (UI, shortcut, accelerate keys) from server
	bool LoadConfigsFromServer(const void* pDataBuf, int iDataSize);
	//	Set logout flag
	void SetLogoutFlag(int iFlag);
	//	Get double experence mode
	int GetDoubleExpMode() { return m_iDblExpMode; }
	//	Get available double experence time
	int GetAvailableDblExpTime() { return m_iAvaDExpTime; }
	//	Get remain time of current double experence time
	int GetRemainDblExpTime();
	//	Lock game points
	void LockGamePoints(bool bLock) { m_bLockGamePt = bLock; }
	//	Get game points locking flag
	bool IsGamePointsLocked() { return m_bLockGamePt; }

	//	Force save necessary user configs (UI, shortcut, accelerate keys) to server
	bool ForceSaveConfigsToServer(bool bRecord = false);

	//	Begin load progress
	bool BeginLoadProgress(int iLoadID, int iMin, int iMax, const char* szBackPic);
	//	End load progress
	void EndLoadProgress();
	//	Step load progress
	void StepLoadProgress(int iStep);

	//	Set / Get selected character's role info.
	const GNET::RoleInfo& GetSelectedRoleInfo();
	void SetSelectedRoleInfo(const GNET::RoleInfo& Info);
	void UpdateSelectedRoleRace(int new_cls);

*/	//	Add fixed format message to chat window
	void AddFixedMessage(int iMsg, ...);
//	void AddFixedChannelMsg(int iMsg, char cChannel, ...);
	//	Add message to chat window
/*	void AddChatMessage(const ACHAR *pszMsg, char cChannel, int idPlayer=-1, const ACHAR* szName=NULL, BYTE byFlag = 0, char cEmotion = 0, CECIvtrItem* pItem = NULL);

	//	Get host player
*/	CECHostPlayer* GetHostPlayer() { return m_pHostPlayer; }
	//	Get host player's position or a candidate position
//	A3DVECTOR3 GetSafeHostPos(CECViewport* pViewport);
	//	Get player name by ID
	const ACHAR* GetPlayerName(int cid, bool bAutoGen);
	//	Get player ID by name
/*	int GetPlayerID(const ACHAR* szName);
	//	Add player name to table
	void AddPlayerName(int cid, const ACHAR* szName, bool bOverwrite=false);
	//	Get team manager
	CECTeamMan* GetTeamMan() { return m_pTeamMan; }

	//	Get instance by ID
*/	CECInstance* GetInstance(int id);
	//	Jump to specified position in a instance
/*	bool JumpToInstance(int idInst, const A3DVECTOR3& vPos);

	//	Post game message
	bool PostMessage(DWORD dwMsg, int iManager, int iSubID, DWORD p1=0, DWORD p2=0, DWORD p3=0, DWORD p4=0);
	//	Post game message
	bool PostMessage(const ECMSG& Msg);
	//	Process game message
	bool ProcessMessage(const ECMSG& Msg);
	//	Check defence
	void CheckDefenceCode();
	void CheckDefenceCodeImpl();

	//	Get shortcut sets
	CECShortcutSet* GetGenCmdShortcuts() { return m_pNormalSCS; }
	CECShortcutSet* GetTeamCmdShortcuts() { return m_pTeamSCS; }
	CECShortcutSet* GetTradeCmdShortcuts() { return m_pTradeSCS; }
	CECShortcutSet* GetPoseCmdShortcuts() { return m_pPoseSCS; }
	CECShortcutSet* GetFactionCmdShortcuts() { return m_pFactionSCS; }
	//	Get command shortcut (except pose command) by command ID
	CECSCCommand* GetCmdShortcut(int iCommand);
	//	Get pose command shortcut by pose
	CECSCCommand* GetPoseCmdShortcut(int iPose);
	//	Get game message manager
	CECMessageMan* GetMessageMan() { return m_pMessageMan; }
	//	Get game data clipboard
	CECClipboard* GetClipboard() { return m_pClipboard; }
	//	Get gfx composer man
	_SGC::A3DSkillGfxComposerMan* GetSkillGfxComposerMan() { return m_pSkillGfxComposerMan; }
	//	Init gfx composer man
	void InitGfxComposerMan();
	//	Load special id config
*/	void LoadSpecialIDConfig();
	//	Get special id config
	SPECIAL_ID_CONFIG& GetSpecialIDConfig() { return m_SpecialIDConfig; }

/*	ACString GetCurGroup() const { return m_strCurGroup; }
	void SetCurGroup(ACString group) { m_strCurGroup = group; }
	ACString GetServerName() const { return m_strServerName; }
	void SetServerName(ACString name) { m_strServerName = name; }
	LineInfoVec& GetLinesInfo() { return m_LineInfoVec; }
	int GetCurLine() const { return m_nCurLine; }
	void SetCurLine(int line) { m_nCurLine = line; }
	bool IsNoPKLine() const	{ return m_bNoPKLine; }
	void SetNoPKLine(bool flag){ m_bNoPKLine = flag; }
	unsigned int GetGShopTimeStamp() const { return m_nGShopTimeStamp; }
	bool IsPVPMode() { return m_bPVPMode; }
	void SetPVPMode(bool b) { m_bPVPMode = b; }
	int GetCityOwnerFaction() const { return m_nCityOwnerFaction; }
	void SetCityOwnerFaction(int id) { m_nCityOwnerFaction = id; }
	int GetCityOwnerMaster() const { return m_nCityOwnerMaster; }
	void SetCityOwnerMaster(int id) { m_nCityOwnerMaster = id; }
	int GetCityOccupyTime() const { return m_nCityOccupyTime; }
	void SetCityOccupyTime(int t) { m_nCityOccupyTime = t; }
	bool GetMasterFlag() const { return m_bMasterFlag; }
	void SetMasterFlag(bool b) { m_bMasterFlag = b; }
	bool GetFlashWindow() const { return m_bFlashWindow; }
	//	当前窗口是否被托盘化
//	bool GetTrayFlag() const { return m_bIsTray; }
//	void SetTrayFlag(bool bTray)  { m_bIsTray = bTray; }
// 	bool GetTrayFlashFlag() const { return m_bTrayFlash; }
	
	//AI helper
	CECHelper * GetHelper() {return m_pECHelper;}
	CECSecretary * GetSecretary() { return m_pSecretary; }
	bool IsHideElsePlayer() { return m_bHideElsePlayer; } 
*/
	//	Create host player
	bool CreateHostPlayer();
	//	Release host player
	void ReleaseHostPlayer();
protected:	//	Attributes

	CECUIManager*		m_pUIManager;		//	UI manager
	CECWorld*			m_pWorld;			//	Game world
/*	CECInputCtrl*		m_pInputCtrl;		//	Input controller
	CECHostInputFilter*	m_pInputFilter;		//	Host input filter
	CECLoadProgress*	m_pLoadProgress;	//	Load progress
	CECFullGlowRender*	m_pFullGlowRender;	//	Full screen glow render
	CECSimpleBloomRender * m_pSBRender;		//	SimpleBloomRender for TNT or Geforce2
	CECScreenRender *	m_pScreenRender;	//	Screen render
	_SGC::A3DSkillGfxComposerMan* m_pSkillGfxComposerMan;		// skill gfx composer mananger

*/	CECHostPlayer*		m_pHostPlayer;		//	Host player object
	ID2NameTable		m_ID2NameTab;		//	Player ID to name table
	Name2IDTable		m_Name2IDTab;		//	Player name to ID table
	ACString			m_strAutoName;		//	Auto generated name
/*	CECMessageMan*		m_pMessageMan;		//	Game message manager
	CECClipboard*		m_pClipboard;		//	Clipboard

*/	InstTable			m_InstTab;			//	Instance table

/*	CECTeamMan*			m_pTeamMan;			//	Team manager
	CECShortcutSet*		m_pNormalSCS;		//	Normal shortcut set
	CECShortcutSet*		m_pTeamSCS;			//	Team shortcut set
	CECShortcutSet*		m_pTradeSCS;		//	Trade shortcut set
	CECShortcutSet*		m_pPoseSCS;			//	Pose shortcut set
	CECShortcutSet*		m_pFactionSCS;		//	Faction shortcut set

	int			m_iGameState;		//	Game state
	DWORD		m_dwRenderTime;		//	Render time of this tick
	bool		m_bUIHasCursor;		//	true, show UI cursor. false, show game cursor
	int			m_iLogoutFlag;		//	Logout flag
	int			m_iCfgDataSize;		//	Size of config data sent last time
	BYTE*		m_pCfgDataBuf;		//	Config data sent last time

	int 		m_iDblExpMode;		//	Double experence mode
	int			m_iDExpEndTime;		//	End time of current double experence
	int			m_iAvaDExpTime;		//	Available double experence time
	bool		m_bLockGamePt;		//	true, lock game points

	ACString	m_strCurGroup;
	ACString	m_strServerName;
	LineInfoVec	m_LineInfoVec;
	int			m_nCurLine;
	bool		m_bNoPKLine;
	bool		m_bPVPMode;
	int			m_nCityOwnerFaction;
	int			m_nCityOwnerMaster;
	int			m_nCityOccupyTime;
	bool		m_bMasterFlag;
	bool		m_bFlashWindow;		//	true for flash window when Not Focused On
	bool		m_bIsFirstFlash;	//	true if player hasn't focused window after the window flash
//	bool		m_bIsTray;
// 	bool		m_bTrayFlash;
	bool		m_bHasReferrer;
	bool		m_bHideElsePlayer;
	int			m_nWallowStage;
	DWORD		m_nWallowTime;

	unsigned int m_nGShopTimeStamp;
*/	SPECIAL_ID_CONFIG m_SpecialIDConfig;
/*	CECHelper	* m_pECHelper;      //the helper baby
	CECSecretary * m_pSecretary;	// the Secretary 

	S2C::player_wallow_info m_WallowInfo;

public:
	CECFullGlowRender* GetFullGlowRender() { return m_pFullGlowRender; }
	CECSimpleBloomRender* GetSBRender() { return m_pSBRender; }
	CECScreenRender* GetScreenRender() { return m_pScreenRender; }

	const S2C::player_wallow_info& GetWallowInfo() { return m_WallowInfo; }
	void SetWallowInfo(const S2C::player_wallow_info& info) { m_WallowInfo = info; }

protected:	//	Operations

	//	Create world
	bool CreateWorld(int id, const A3DVECTOR3& vHostPos);
	//	Release world
	void ReleaseWorld();
	//	Create Login World
	bool CreateLoginWorld(const A3DVECTOR3& vCameraPos);
	//	Release Login World
	void ReleaseLoginWorld();

	//	Create shortcuts
	bool CreateShortcuts();
	//	Release shortcuts
	void ReleaseShortcuts();
*/	//	Release name/id pairs cache
	void ClearNameIDPairs();
	//	Release Gfx composer man
/*	void ReleaseGfxComposerMan();

	//	Load instance information from file
	bool LoadInstanceInfo(const char* szFile);

	//	End current game state
	void EndGameState(bool bReset=true);
	//	End login state
	void OnEndLoginState();
	//	End game state
	void OnEndGameState();
	//	Re-login
	bool Relogin();
	//	Logout
	void Logout(int iFlag);

	//	Tick game world
	bool TickGameWorld(DWORD dwDeltaTime, CECViewport* pViewport);
	//	Begin render
	bool BeginRender();
	//	End render
	bool EndRender(bool bPresent=true);
	//	Display game statistic
	void DisplayGameStatistic();
	
	// Start record game
	void StartRecordGame();
	// Save key point
	void RecordSaveKeyPoint();*/
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

