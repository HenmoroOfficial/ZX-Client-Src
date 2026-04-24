/*
 * FILE: EC_Cutscene.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: SunXuewei, 2010/04/30
 *
 * HISTORY: 
 *
 * Copyright (c) 2010 Zhuxian Studio, All Rights Reserved.
 */


#include "EC_Global.h"
#include "EC_Cutscene.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Scene.h"
#include "EC_Configs.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_HPWork.h"
#include "EC_HPWorkTrace.h"
#include "EC_HPWorkMove.h"
#include "EC_HPWorkAutoMove.h"
#include "DlgCameraPath.h"
#include "EL_BackMusic.h"
#include <AMEngine.h>
#include <A3DDevice.h>
#include "A3DTerrain2.h"
#include "AnmSystem.h"

#include "AUI/AUIDialog.h"

#include "LuaState.h"
#include "LuaUtil.h"
#include "ScriptValue.h"
#include "LuaFuncCommon.h"

#include "EC_NewHelpGuide.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define	new A_DEBUG_NEW

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CECCutscene
//	
///////////////////////////////////////////////////////////////////////////

CECCutscene::CECCutscene()
{
	m_id				= 0;
	m_dwTimeElapsed		= 0;
	m_bLastShowUI		= true;
	m_bIsPlayingCutscene= false;

	m_iCurrCameraPathIndex = 0;
	int i;
	for(i=0;i<MAX_CAMERA_PATH; i++)
	{
		m_ulCameraPathIDList[i] = 0;
		m_ulCameraPathStayTime[i] = 0;
	}

	m_pCameraPath		= NULL;
	m_idMusic			= 0;
	m_bTurnOffSound		= false;
	m_bSettingChanged	= false;

	for(i=0;i<4;i++)
		m_idCutsceneMaskData[i] = 0;
}
CECCutscene::~CECCutscene()
{
	if(m_pCameraPath)
	{
		delete m_pCameraPath;
		m_pCameraPath = NULL;
	}
}

bool CECCutscene::Init()
{
	//	Create camera path
	if (!(m_pCameraPath = new CECCameraPath))
	{
		glb_ErrorOutput(ECERR_NOTENOUGHMEMORY, "CECCutscene::Init", __LINE__);
		return false;
	}

	m_ExitCounter.SetPeriod(500);
	m_ExitCounter.Reset(true);

	return true;
}

void CECCutscene::Tick(DWORD dwDeltaTime)
{
	if(IsPlayingCutscene())
	{
		m_pCameraPath->Tick(dwDeltaTime);

		if(!m_pCameraPath->IsUsingCameraPath())
		{
			m_iCurrCameraPathIndex ++;

			if(m_iCurrCameraPathIndex < MAX_CAMERA_PATH && m_ulCameraPathIDList[m_iCurrCameraPathIndex] != 0)
			{
				CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
				CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pGameUI->GetDialog("Win_CameraPath");

 				if(pDlgCameraPath && !m_bShowBgImage)
					pDlgCameraPath->Shadow(m_iShadowTime/2, m_iShadowTime/2);

				m_pCameraPath->SetStayTime(m_ulCameraPathStayTime[m_iCurrCameraPathIndex]);
				m_pCameraPath->StartCameraPath(m_ulCameraPathIDList[m_iCurrCameraPathIndex]);
			}
			else if(m_ExitCounter.IsFull())
			{
				EndCutscene();
			}
		}
	}

	if(!m_ExitCounter.IsFull())
	{
		if(m_ExitCounter.IncCounter(dwDeltaTime))
		{
			m_bIsPlayingCutscene = false;
			EndCutscene();
		}
	}
}
//由于历史使用的原因，需要将ID转换一下
static int ConvertCutSceneIdToAnmId(DWORD  dwSceneId)
{
	DWORD dwAnmId = dwSceneId;
	if(dwSceneId == 4)
	{
        dwAnmId =  ANIMATION_TERRAN;
	}
	else if(dwSceneId == 6)
	{
        dwAnmId =  ANIMATION_PROTOSS;
	}
	else if(dwSceneId == 8)
	{
        dwAnmId =  ANIMATION_ELDAR;
	}
    return dwAnmId;
}
void CECCutscene::StartAnimation(DWORD dwAnmId,bool bNeedCheck)
{
	if(bNeedCheck && HasBeenPlayed(dwAnmId))
		return;
	SaveHasBeenPlayedConfig(dwAnmId);
    if(g_pGame->GetGameRun()->GetAnmSystem()->Play(ConvertCutSceneIdToAnmId(dwAnmId)))
	{
	}
}
bool CECCutscene::StartCutscene(DWORD dwCutsceneIndex, bool bNeedCheck)
{
	// Check if has been played
	if(bNeedCheck && HasBeenPlayed(dwCutsceneIndex))
		return false;

	//end other cutscene playing now.
//	EndCutscene();

	int i;
	// clear camera path list
	for(i=0;i<MAX_CAMERA_PATH; i++)
		m_ulCameraPathIDList[i] = 0;

	abase::vector<CScriptValue> args;
	abase::vector<CScriptValue> results;
	CScriptValue val;

	val.SetVal((double)dwCutsceneIndex);
	args.push_back(val);
	CLuaState *pState = g_LuaStateMan.GetAIState();
	if (!pState)
	{
		ASSERT(0);
		return false;
	}
	pState->Call(CUTSCENE_TBL_NAME, ("GetCutscene"), args, results);
	if( results.size()<=0 || CScriptValue::SVT_ARRAY!=results[0].m_Type)
	{
		a_LogOutput(1, ("CECCutscene::StartCutscene, error cutscene index %d\n"), dwCutsceneIndex);
		return false;
	}

#define _LUA_GET_VAR(var,varName) if(!luaf::GetValue(var,results[0],varName)) { ASSERT(0); return false; }

	m_id = dwCutsceneIndex;	
	_LUA_GET_VAR(m_idSubtitle,			L"idSubtitle")

	_LUA_GET_VAR(m_idMusic,				L"idBackGroundMusic")

	int idDisplaySetting = 0;
	_LUA_GET_VAR(idDisplaySetting,		L"idDisplaySettings")

	_LUA_GET_VAR(m_bTurnOffSound,		L"turnOffSound")

	_LUA_GET_VAR(m_idBgImage,			L"idBackGroundImage")



	int idNPCConfig = 0;
	_LUA_GET_VAR(idNPCConfig,			L"idNPCConfig")

	int idPlayerConfig = 0;
	_LUA_GET_VAR(idPlayerConfig,		L"idPlayerConfig")

	_LUA_GET_VAR(m_iShadowTime,			L"shadowTime")

	// Load camera path id list
    const CScriptValue* pPathID	= luaf::GetValue(results[0], L"idCameraPath");
	for (i=0; i<(int)pPathID->m_ArrKey.size()/2; i++)
	{
		if(i>=MAX_CAMERA_PATH)
			break;

		m_ulCameraPathIDList[i] = (unsigned int)pPathID->m_ArrVal[i*2].GetInt();
		m_ulCameraPathStayTime[i] = (unsigned int)pPathID->m_ArrVal[i*2+1].GetInt();
	}

	// Load display settings
	val.SetVal((double)idDisplaySetting);
	args.clear();
	args.push_back(val);
	pState->Call(DISPLAYSETTING_TBL_NAME, ("GetDisplaySettings"), args, results);
	if( results.size()<=0 || CScriptValue::SVT_ARRAY!=results[0].m_Type)
	{
		a_LogOutput(1, ("CECCutscene::StartCutscene, error display setting index %d\n"), idDisplaySetting);
		return false;
	}

	_LUA_GET_VAR(m_bOnlyShowOnce,		L"onlyShowOnce")
	_LUA_GET_VAR(m_bCanEscClose,		L"canEscClose")
	_LUA_GET_VAR(m_bDisableOtherKey,	L"disableOtherKey")
	_LUA_GET_VAR(m_bShowUI,				L"showUI")
	_LUA_GET_VAR(m_bShowSubtitle,		L"showSubtitle")
	_LUA_GET_VAR(m_bShowElsePlayers,	L"showElsePlayers")
	_LUA_GET_VAR(m_bShowSelf,			L"showSelf")
	_LUA_GET_VAR(m_bShowNPC,			L"showNPC")
	_LUA_GET_VAR(m_bShowMonster,		L"showMonster")
	_LUA_GET_VAR(m_bShowName,			L"showModelName")
	_LUA_GET_VAR(m_bCancelAutoPath,		L"cancelAutoPath")
	_LUA_GET_VAR(m_bShowBgImage,		L"showBgImage")


	// Load subtitle
	if(m_bShowSubtitle)		
	{
		val.SetVal((double)m_idSubtitle);
		args.clear();
		args.push_back(val);
		pState->Call(SUBTITLE_TBL_NAME, ("GetSubtitle"), args, results);
		if( results.size()<=0 || CScriptValue::SVT_STRING!=results[0].m_Type)
		{
			a_LogOutput(1, ("CECCutscene::StartCutscene, error subtitle index %d\n"), m_idSubtitle);
			return false;
		}

		results[0].RetrieveAWString(m_strSubtitle);
	}

	// Load background image
	val.SetVal((double)m_idBgImage);
	args.clear();
	args.push_back(val);
	pState->Call(BGIMAGE_TBL_NAME, ("GetImage"), args, results);
	if( results.size()<=0 || CScriptValue::SVT_STRING!=results[0].m_Type)
	{
		a_LogOutput(1, ("CECCutscene::StartCutscene, error background image index %d\n"), m_idBgImage);
		return false;
	}

	results[0].RetrieveAWString(m_strImage);


	// Load background music
	if(m_idMusic > 0)
	{
		val.SetVal((double)m_idMusic);
		args.clear();
		args.push_back(val);
		pState->Call(BACKGROUNDMUSIC_TBL_NAME, ("GetMusic"), args, results);
		if( results.size()<=0 || CScriptValue::SVT_STRING!=results[0].m_Type)
		{
			a_LogOutput(1, ("CECCutscene::StartCutscene, error background music index %d\n"), m_idMusic);
			return false;
		}

		results[0].RetrieveAWString(m_strMusic);
	}


	// Load NPC
//	if(idNPCConfig != 0)
//	{
// 		m_NPCList.clear();
// 
// 		val.SetVal((double)idNPCConfig);
// 		args.clear();
// 		args.push_back(val);
// 		pState->Call(NPCCONFIG_TBL_NAME, ("GetNPCConfig"), args, results);
// 		if( results.size()<=0 || CScriptValue::SVT_ARRAY!=results[0].m_Type)
// 		{
// 			a_LogOutput(1, ("CECCutscene::StartCutscene, error npc config index %d\n"), idNPCConfig);
// 			return false;
// 		}
// 
// 		CUTSCENE_NPC npc;
// 		
// 		_LUA_GET_VAR(npc.m_ulTemplId,			L"npcID")
// 		_LUA_GET_VAR(npc.m_ulShowUpTime,		L"npcShowUpTime")
// 		_LUA_GET_VAR(npc.m_ulDisappearTime,		L"npcDisappearTime")
// 		_LUA_GET_VAR(npc.m_npcPathId,			L"npcPathID")
// 
// 
// 		// to be continued..
// 
//		m_NPCList.push_back(npc);
//	}
	
#undef _LUA_GET_VAR

//	if(bNeedCheck)
//	{
		if(!_CheckCutscene())
			return false;
//	}

	// Load resource first
	for(i=0;i<MAX_CAMERA_PATH; i++)
	{
		A3DVECTOR3 vPos;
		if(m_ulCameraPathIDList[i] != 0)
			vPos = m_pCameraPath->GetBezier(m_ulCameraPathIDList[i])->GetPoint(0)->GetPos();
		else 
			break;

		CECScene* pScene = g_pGame->GetGameRun()->GetWorld()->GetScene();
		pScene->SetLoadCenter(vPos, true);
		pScene->LoadBlockResByPos(vPos);
	}
		

	g_pGame->GetGameRun()->GetHostPlayer()->Stand();
	m_bIsPlayingCutscene = true;
	_OnStartCutscene(dwCutsceneIndex);
	m_iCurrCameraPathIndex = 0;
	m_pCameraPath->SetStayTime(m_ulCameraPathStayTime[m_iCurrCameraPathIndex]);
	bool bSuccess = m_pCameraPath->StartCameraPath(m_ulCameraPathIDList[m_iCurrCameraPathIndex]);

	if(bSuccess)
	{
		SaveHasBeenPlayedConfig(dwCutsceneIndex);
	}


	return true;
}

void CECCutscene::EndCutscene()
{
	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pGameUI->GetDialog("Win_CameraPath");	
	
	if (m_bIsPlayingCutscene)
	{		
		if(m_ExitCounter.IsFull())
		{
			m_ExitCounter.SetPeriod(500);
			m_ExitCounter.Reset(false);	
			
			if(pDlgCameraPath && m_bShowSubtitle)
			{
				pDlgCameraPath->ShowText(L"");
				pDlgCameraPath->ShowTextBackGround(false);
			}
		}

//		m_bIsPlayingCutscene = false;

		return;
	}

	_OnStopCutscene();

	// clear camera path list
	for(int i=0;i<MAX_CAMERA_PATH; i++)
		m_ulCameraPathIDList[i] = 0;

	if(pDlgCameraPath)
		pDlgCameraPath->Show(false);
}

bool CECCutscene::IsPlayingCutscene()
{
	return m_bIsPlayingCutscene;
}

bool CECCutscene::HasDelayedCutscene()
{
	return false;
}

bool CECCutscene::HasBeenPlayed(DWORD dwCutsceneIndex)
{
	int index = g_pGame->GetCutsceneTabByIndex(dwCutsceneIndex);
	int iRet = 0;
	if(index >= 0)
	{
		//check mask data in configs
		if(index < 32)
			iRet =  (m_idCutsceneMaskData[0] & (1 << index));
		else if(index < 64)
			iRet = (m_idCutsceneMaskData[1] & (1 << (index-32)));
		else if(index < 96)
			iRet = (m_idCutsceneMaskData[2] & (1 << (index-64)));
		else 
			iRet = (m_idCutsceneMaskData[3] & (1 << (index-96)));

		return iRet > 0 ? true : false;
	}

	return false;
}
void CECCutscene::SaveHasBeenPlayedConfig(DWORD dwCutsceneIndex)
{
	// save to configs data
	int index = g_pGame->GetCutsceneTabByIndex(dwCutsceneIndex);
	if(index >= 0)
	{
		if(index < 32)
			m_idCutsceneMaskData[0] |= (1 << index);
		else if(index < 64)
			m_idCutsceneMaskData[1] |= (1 << (index-32));
		else if(index < 96)
			m_idCutsceneMaskData[2] |= (1 << (index-64));
		else
			m_idCutsceneMaskData[3] |= (1 << (index-96));

		CECConfigs* pConfigs = g_pGame->GetConfigs();
		EC_GAME_CLIENTDATA gcd = pConfigs->GetGameClientData();

		for(int i=0;i<4; i++)
			gcd.nCutsceneData[i] = m_idCutsceneMaskData[i];

		pConfigs->SetGameClientData(gcd);

		//	Save configs when time reached
		g_pGame->GetGameRun()->ForceSaveConfigsToServer();
	}
}
void CECCutscene::InitMaskData()
{
	CECConfigs* pConfigs = g_pGame->GetConfigs();
	int i = 0;
	for(i=0;i<4;i++)
	{
		m_idCutsceneMaskData[i] = pConfigs->GetGameClientData().nCutsceneData[i];
	}
}

bool CECCutscene::_CheckCutscene()
{
	for(int i=0;i<MAX_CAMERA_PATH; i++)
	{
		if(m_ulCameraPathIDList[i] != 0 && !m_pCameraPath->GetBezier(m_ulCameraPathIDList[i]))		// bezier not found in current map
			return false;
	}
	return true;
}
void CECCutscene::_OnStartCutscene(DWORD dwCutsceneIndex)
{
//	m_dwTickTimeExp.Clear();
	
	memcpy(&m_OldSettings,g_pGame->GetConfigs()->GetSystemSettingsPtr(), sizeof(EC_SYSTEM_SETTING));
	EC_SYSTEM_SETTING ss = g_pGame->GetConfigs()->GetSystemSettings();

	// check if we need to change settings.
	if(ss.nSight < 4)
	{
		ss.nSight = 4;
		m_bSettingChanged = true;
	}

	if(m_bTurnOffSound)
	{
		if(ss.nSoundVol > 0)
		{
			ss.nSoundVol = 0;
			g_pGame->GetA3DDevice()->GetA3DEngine()->GetAMSoundEngine()->SetVolume(0);
			m_bSettingChanged = true;
		}
	}

	if(m_bSettingChanged)
		g_pGame->GetConfigs()->SetSystemSettings(ss);

	CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
	CDlgCameraPath* pDlgCameraPath = (CDlgCameraPath*)pGameUI->GetDialog("Win_CameraPath");

	if(pDlgCameraPath)
	{
		// 显示字幕
		if(m_bShowSubtitle)
		{
			pDlgCameraPath->ShowText(m_strSubtitle);
			pDlgCameraPath->ShowTextBackGround(true);
		}

		// 显示背景图
		if(m_idBgImage)
			pDlgCameraPath->SetBgImage(AC2AS(m_strImage));

		if(m_bShowBgImage)	// 一直显示背景图，无渐变
			pDlgCameraPath->AlphaInfo(255);

		if(!pDlgCameraPath->IsShow())
			pDlgCameraPath->Show(true);
	}

	if (!IsUIShow() && 
		g_pGame->GetGameRun()->GetUIManager() && 
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan())
	{
		CECGameUIMan* pUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		m_bLastShowUI = pUI->IsShowAllPanels();
		if (m_bLastShowUI)
		{
			pUI->SetShowAllPanels(m_bShowUI);
		}
	}
	if (!IsNameShow())
	{
		if (g_pGame->GetGameRun()->GetHostPlayer())
		{
			g_pGame->GetGameRun()->GetHostPlayer()->SetRenderNameFlag(false);
			g_pGame->GetGameRun()->GetHostPlayer()->SetRenderBarFlag(false);
		}
	}

	if (IsOtherKeyDisabled())
	{
		g_pGame->ShowCursor(false);
	}
	
	if (m_bCancelAutoPath)
	{
		if (g_pGame->GetGameRun()->GetHostPlayer())
		{			
			CECHPWorkMan* pWorkMan = g_pGame->GetGameRun()->GetHostPlayer()->GetHPWorkMan();
			if (pWorkMan)
			{
				CECHPWork* pCurWork = pWorkMan->GetCurWork();
				if (pCurWork)
				{	
					if (pCurWork->GetWorkID() == CECHPWork::WORK_MOVETOPOS)
					{
						((CECHPWorkMove*)pCurWork)->PressCancel();
					}
                    
					if ((pCurWork->GetWorkID() == CECHPWork::WORK_AUTOMOVE))
					{
					    CECHostPlayer *pHost = g_pGame->GetGameRun()->GetHostPlayer();
						int iCurMoveMode;
						float fSpeed ;
						A3DVECTOR3 vPos = pHost->GetPos();
						if(pHost->GetMoveEnv() == CECPlayer::MOVEENV_GROUND)
						{
							iCurMoveMode = pHost->GetWalkRunFlag() ? GP_MOVE_RUN : GP_MOVE_WALK;
                            fSpeed = pHost->GetGroundSpeed();
						}
						else
						{
	                        iCurMoveMode = (pHost->GetMoveEnv() == CECPlayer::MOVEENV_AIR) ? GP_MOVE_AIR : GP_MOVE_WATER;
							iCurMoveMode = iCurMoveMode | GP_MOVE_RUN;
                            fSpeed = (pHost->GetMoveEnv() == CECPlayer::MOVEENV_AIR) ? pHost->GetFlySpeed() : pHost->GetSwimSpeed(); 
						}

					    if (pCurWork->GetWorkID() == CECHPWork::WORK_AUTOMOVE) 
						{
                            ((CECHPWorkAutoMove*)pCurWork)->StopMove(vPos,fSpeed,iCurMoveMode);
				            pWorkMan->FinishCurWork(CECHPWork::WORK_AUTOMOVE,true);
						}
					}
				}
			}
		}
	}

	CELBackMusic* pBackMusic = g_pGame->GetBackMusic();

	// 是否禁掉音效
	if(m_bTurnOffSound)
		pBackMusic->StopBackSFX(false);

	// 播放特定背景音乐
	if(m_idMusic > 0)
	{
		pBackMusic->StopMusic(false, true);
		pBackMusic->SetLoopType((CELBackMusic::LOOPTYPE)0);
		pBackMusic->SetSilenceTime(0);
		pBackMusic->PlayMusic(AC2AS(m_strMusic), false/*true*/, true);
	}
	else if(m_idMusic < 0)	// 禁止背景音乐
		pBackMusic->StopMusic(false, true);	
}
void CECCutscene::_OnStopCutscene()
{
	if (!IsUIShow() && 
		g_pGame->GetGameRun()->GetUIManager() && 
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan())
	{
		CECGameUIMan* pUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		if (m_bLastShowUI)
		{
			pUI->SetShowAllPanels(m_bLastShowUI);
		}
	}
	if (!IsNameShow())
	{
		if (g_pGame->GetGameRun()->GetHostPlayer())
		{
			g_pGame->GetGameRun()->GetHostPlayer()->SetRenderNameFlag(true);
			g_pGame->GetGameRun()->GetHostPlayer()->SetRenderBarFlag(true);
		}
	}
	
	if (IsOtherKeyDisabled())
	{
		g_pGame->ShowCursor(true);
	}

	// 停止播放特定背景音乐，恢复播放原来音乐
	if(m_idMusic != 0)
	{
		g_pGame->GetGameRun()->GetWorld()->StopCurrMusic();
		g_pGame->GetGameRun()->GetWorld()->RestartMusic();
	}

	// 临时加载玩家附近小范围区域，防止卡住
	float fOldActRadius = g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetActRadius();
	float fOldViewRadius = g_pGame->GetGameRun()->GetWorld()->GetTerrain()->GetViewRadius();
	float fOldSceneRadius = g_pGame->GetGameRun()->GetWorld()->GetScene()->GetActRadius();

	g_pGame->GetGameRun()->GetWorld()->GetTerrain()->SetActRadius(10.0f);
	g_pGame->GetGameRun()->GetWorld()->GetTerrain()->SetViewRadius(10.0f);
	g_pGame->GetGameRun()->GetWorld()->GetScene()->SetActRadius(10.0f);

	g_pGame->GetGameRun()->GetWorld()->SetLoadCenter(g_pGame->GetGameRun()->GetHostPlayer()->GetPos());

	g_pGame->GetGameRun()->GetWorld()->GetTerrain()->SetActRadius(fOldActRadius);
	g_pGame->GetGameRun()->GetWorld()->GetTerrain()->SetViewRadius(fOldViewRadius);
	g_pGame->GetGameRun()->GetWorld()->GetScene()->SetActRadius(fOldSceneRadius);

	if(m_bSettingChanged)
	{
		// set settings back
		g_pGame->GetConfigs()->SetSystemSettings(m_OldSettings);
		g_pGame->GetA3DDevice()->GetA3DEngine()->GetAMSoundEngine()->SetVolume(g_pGame->GetConfigs()->GetSystemSettings().nSoundVol);
		m_bSettingChanged = false;
	}

	//初次游戏进入，结束开启动画后，打开新手指引提示
	if(m_id == 4 || m_id == 6)
	{
	  //提示是否需要帮助
		//NewHelpGuide::StartGuide_Welcome();
	}
}