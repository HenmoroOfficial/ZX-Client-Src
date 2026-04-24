/********************************************************************
  created:	   4/7/2009  
  author:      liudong  
  description:  envionment for lua api
  Copyright (c) , All Rights Reserved.
*********************************************************************/
#include "LuaApiEnv.h"

#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"

CECGame * CLuaApiEnv::GetGame()
{	
	return g_pGame;
}

CECGameRun * CLuaApiEnv::GetGameRun()
{
	return g_pGame->GetGameRun();
}

CECWorld * CLuaApiEnv::GetWorld()
{
	return g_pGame->GetGameRun()->GetWorld();
}

CECGameSession * CLuaApiEnv::GetGameSession()
{
	return g_pGame->GetGameSession();
}

CECHostPlayer * CLuaApiEnv::GetHostPlayer()
{	
	return GetGameRun()->GetHostPlayer();
}

CECGameUIMan * CLuaApiEnv::GetGameUIMan()
{
	return GetGameRun()->GetUIManager()->GetInGameUIMan();
}


void CLuaApiEnv::AddChatMsg(const wchar_t* pMsg)
{
	if (pMsg && pMsg[0])
	{
		GetGameRun()->AddChatMessage(pMsg,GP_CHAT_SYSTEM);
	}
}