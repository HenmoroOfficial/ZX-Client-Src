/*
 * FILE: EC_DlgCmdConsole.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/31
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#include "EC_Global.h"
#include "EC_GameUIMan.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Scene.h"
#include "EC_HostPlayer.h"
#include "EC_RTDebug.h"
#include "EC_GameSession.h"
#include "EC_GFXCaster.h"
#include "EC_Configs.h"
#include "TaskTemplMan.h"
#include "EC_TaskInterface.h"
#ifdef BACK_VERSION
#include "EC_SunMoon.h"
#include "EC_Manager.h"
#include "A3DTerrainWater.h"
#else
#include "A3DSunMoon.h"
#include "ECManager.h"
#include "A3DTerrainWater2.h"
#endif
#include "EC_World.h"
#include "TaskTemplMan.h"
#include "EC_Cutscene.h"
#include "EC_FullGlowRender.h"
#include "EC_Menology.h"

#include "EC_UIManager.h"
#include "AnmSystem.h"

#include "gnetdef.h"
#include "privilege.hxx"

#include "A3DConfig.h"
#include "A3DTerrain2CullHC.h"
#include "A3DTerrain2.h"
#include "A3DSkySphere.h"
#include "A3DDevice.h"

#include "DlgGMConsole.h"
#include "DlgClassmateManage.h"
#include "DlgQShop.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Maximum number of tokens
#define MAXNUM_TOKEN	18

//	Console commands
enum
{
	CCMD_EXITGAME = 0,		//	Exit game

	//	Debug commands
	CCMD_CAMERAMODE,		//	Host's camera mode
	CCMD_BOUNDBOX,			//	Trigger bounding box
	CCMD_RTDEBUG,			//	Trigger runtime debug
	CCMD_NPCID,				//	Trigger NPC ID
	CCMD_ITEMID,			//	Trigger Item ID
	CCMD_RUNSPEED,			//	Host run speed
	CCMD_GOTO,				//	Goto specified position
	CCMD_GOTOLINE,			//	Goto specified line
	CCMD_FLY,				//	Go to fly mode
	CCMD_C2SCMD,			//	Send c2s command
	CCMD_C2SCMDP,			//	Send c2s command with customized parameter
	CCMD_VIEWRAIDUS,		//	Set view radius
	CCMD_RELOGIN,			//	Re-login
	CCMD_CASTSKILL,			//	Cast skill
	CCMD_RENDERWATER,		//	Water render quality
	CCMD_RENDERGRASS,		//	Grass render flag
	CCMD_RENDERFOREST,		//	Forest render flag
	CCMD_RENDERSHADOW,		//	Shadow render flag
	CCMD_RENDEROUTLINE,		//	Outline render flag
	CCMD_TURNAROUND,		//	Turnaround flag
	CCMD_TESTDIST,			//	Test distance
	CCMD_PLAYGFX,			//	Play specified GFX
	CCMD_SHOWPOS,			//	Show position
	CCMD_TRNLAYER,			//	Terrain layer
	CCMD_A3DSTAT,			//	A3D statistic
	CCMD_GAMESTAT,			//	Game statistic
	CCMD_TREELOD,			//	Tree lod
	CCMD_FPS,				//	FPS switch
	CCMD_PVRADIUS,			//	Player visible radius
	CCMD_SHOWID,			//	Show player ID
	CCMD_SKIPFRAME,			//	Skip frame flag
	CCMD_MODELUPDATE,		//	Model update optimization flag
	CCMD_MINIDUMP,			//	Test mini-dump
	CCMD_SETTIMEOFDAY,		//	Set time of the day
	CCMD_GETSERVERTIME,		//	Get time on the server
	CCMD_TASK,				//	task info
	CCMD_SETBIAS,			//	Set MipMap Bias value
	CCMD_UPDATELUA,			//	Update lua script files
	CCMD_TERRAINCULLER,		//	Enable / disable terrain culler
	CCMD_DEBUGGSCMD,		//	Debug GS command
	CCMD_DEBUGDELIVERCMD,	//	Debug deliver command
	CCMD_FIXEDCAMERA,
	CCMD_TOWERDEFENSE,      //  tower defense, tiny game embedded in ZhuXian
	CCMD_CAMTURNSPEED,		//  camera turn speed, zoom speed
	CCMD_PLAYCUTSCENE,		//  play cutscene
	CCMD_HDR,				//  set hdr param
	CCMD_SKILLNOTARGET,		//  skill no target
	CCMC_BUYREPUITEM,       //  forced to buy reputation items 
	CCMD_ANM,				//  play animation file in elements/Animations/
	CCMD_GAMESNAKE,         //  game snake, tiny game embedded in ZhuXian
	CCMD_TINYGAME,          // popup tinygame
	CCMD_MANUALTASK,		// triger manual task

	//	GM commonds
	CCMD_GM_KICKOUTROLE,
	CCMD_GM_KICKOUTUSER,
	CCMD_GM_LISTONLINEUSER,
	CCMD_GM_ONLINENUMBER,
	CCMD_GM_RESTARTSERVER,
	CCMD_GM_SHUTUPROLE,
	CCMD_GM_SHUTUPUSER,
	CCMD_GM_MOVETOPLAYER,
	CCMD_GM_CALLINPLAYER,
	CCMD_GM_BROADCAST,
	CCMD_GM_SHOWID,
	CCMD_GM_FORBIDROLE,
	CCMD_GM_TRIGGERCHAT,
	CCMD_GM_GENERATE,
};

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

static AString	l_aCmdTokens[MAXNUM_TOKEN];		//	Command tokens
static int		l_iNumCmdToken = 0;				//	Number of command token
static ACString	l_strOriginCmd;					//	Original command string
static ACHAR	l_strMsg[256];
static bool		l_bInvalidParam;

static DWORD	l_colRed	= 0xffff0000;
static DWORD	l_colGreen	= 0xff00ff00;

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

//	Parse command line
static void _ParseCommandLine(const char* szCmd)
{
	l_iNumCmdToken	= 0;
	l_bInvalidParam	= false;

	const BYTE* pch = (const BYTE*)szCmd;

	//	Skip space
	while (*pch && *pch <= 32)
		pch++;

	while (*pch && l_iNumCmdToken < MAXNUM_TOKEN)
	{
		const BYTE* pEnd = pch;

		//	Get token
		while (*pEnd > 32)
			pEnd++;

		l_aCmdTokens[l_iNumCmdToken++] = AString((const char*)pch, pEnd-pch);

		pch = pEnd;

		//	Skip space
		while (*pch && *pch <= 32)
			pch++;
	}
}

//	Check command parameter number
static inline bool _CheckConsoleCmdParamNum(int iMinNum, int iMaxNum)
{
	int iNumParam = l_iNumCmdToken - 1;
	l_bInvalidParam = (iNumParam < iMinNum || iNumParam > iMaxNum) ? true : false;
	return !l_bInvalidParam;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

//	On console dialog command
bool CECGameUIMan::OnDlgCmd_Console(const ACHAR* szCommand)
{
	if (!g_pGame->GetConfigs()->HasConsole())
		return true;

	if (!szCommand || !szCommand[0])
		return true;
	

	ACHAR szName[64] = {'\0'};
	ACHAR szTemp[256] = {'\0'};
	const ACHAR* strParams = a_strstr(szCommand, L" ");
	if(!strParams)
	{
		a_strcpy(szName, szCommand);

		AString replacedString = g_pGame->GetDebugCmdByName(AC2AS(szName));
		
		if(replacedString != "")
		{
			a_strcpy(szTemp, AS2AC(replacedString));

			szCommand = szTemp;
		}
	}
	else
	{
		int index = strParams - szCommand;
		a_strncpy(szName, szCommand, index);
		AString replacedString = g_pGame->GetDebugCmdByName(AC2AS(szName));
			
		if(replacedString != "")
		{			
			a_strcpy(szTemp, AS2AC(replacedString));
			a_strcat(szTemp, strParams);
			szCommand = szTemp;
		}
	}

	//	Convert UNICODE to ANSI string
	l_strOriginCmd = szCommand;
	AString strCmdStr = AC2AS(szCommand);
	_ParseCommandLine(strCmdStr);

	if (strCmdStr[0] == 'g' && strCmdStr[1] == 'm' && strCmdStr[2] == '_')
	{
		if (g_pGame->GetGameRun()->GetHostPlayer()->IsGM())
		{
 			if (!m_pDlgGMConsole->IsShow())
 				m_pDlgGMConsole->Show(true);
			ParseGMConsoleCommand();
		}
	}
	else if (strCmdStr[0] == 'd' && strCmdStr[1] == '_')
		ParseDebugConsoleCommand();
	else
		ParseUserConsoleCommand();
	
	return true;
}

//	Compare command name
bool CECGameUIMan::CompConsoleCommand(int iCmd)
{
	const char* szCmd = m_ConsoleCmds.GetANSIString(iCmd);
	if (!szCmd)
		return false;

	return l_aCmdTokens[0].CompareNoCase(szCmd) == 0 ? true : false;
}

//	Parse user console command
void CECGameUIMan::ParseUserConsoleCommand()
{
	CECGameRun* pGameRun = g_pGame->GetGameRun();

	if (CompConsoleCommand(CCMD_EXITGAME))
	{
		//	Exit game
		pGameRun->PostMessage(MSG_EXITGAME, -1, 0);
	}
	else if (strstr(l_aCmdTokens[0], "Win_"))
	{
		PAUIDIALOG pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog(l_aCmdTokens[0]);
		if (pDlg)
			pDlg->Show(!pDlg->IsShow());
		else
		{
			pDlg = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog(l_aCmdTokens[0].Right(l_aCmdTokens[0].GetLength()-strlen("Win_")));
			if (pDlg)
				pDlg->Show(!pDlg->IsShow());
		}
	}
	else if (strstr(l_aCmdTokens[0], "cmds"))
	{
		g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->CommonMessage("Dlg_Console", "run", l_aCmdTokens[1].ToInt(), 0);
	}
	else if (strstr(l_aCmdTokens[0], "testshop"))
	{
		CDlgQShop::TestServer();
	}
	else if (strstr(l_aCmdTokens[0], "testconsign"))
	{
		GetDialog("Win_ShopConsign")->GetDlgItem("Rdo_CharAllList")->Show(l_aCmdTokens[1].ToInt()!=0);
		GetDialog("Win_ShopConsign")->GetDlgItem("Rdo_ConsignChar")->Show(l_aCmdTokens[1].ToInt()!=0);
		GetDialog("Win_ShopConsignView")->GetDlgItem("Rdo_CharAllList")->Show(l_aCmdTokens[1].ToInt()!=0);
	}
	else
	{
		//	Respond: unknown command
		AddConsoleLine(_AL("Unknown command."), l_colRed);
	}
}

//	Parse debug console command
void CECGameUIMan::ParseDebugConsoleCommand()
{
	CECGameSession* pSession = g_pGame->GetGameSession();
	CECGameRun* pGameRun = g_pGame->GetGameRun();
	CECConfigs* pGameCfg = g_pGame->GetConfigs();

	if (CompConsoleCommand(CCMD_CAMERAMODE))
	{
		//	Ghost mode switch
		pGameRun->PostMessage(MSG_HST_CAMERAMODE, MAN_PLAYER, 0);
	}
	else if (CompConsoleCommand(CCMD_BOUNDBOX))
	{
		g_pA3DConfig->RT_SetShowBoundBoxFlag(!g_pA3DConfig->RT_GetShowBoundBoxFlag());
	}
	else if (CompConsoleCommand(CCMD_RTDEBUG))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
			pGameCfg->SetRTDebugLevel(l_aCmdTokens[1].ToInt());
	}
	else if (CompConsoleCommand(CCMD_NPCID))
	{
		pGameCfg->ShowNPCID(!pGameCfg->IsNPCIDShown());
	}
	else if (CompConsoleCommand(CCMD_ITEMID))
	{
		pGameCfg->ShowItemID(!pGameCfg->IsItemIDShown());
	}
	else if (CompConsoleCommand(CCMD_RUNSPEED))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
			pGameCfg->SetHostRunSpeed(l_aCmdTokens[1].ToFloat());
		else
		{
			a_sprintf(l_strMsg, _AL("runspeed = %.2f"), pGameCfg->GetHostRunSpeed());
			AddConsoleLine(l_strMsg, l_colGreen);
		}
	}
	else if (CompConsoleCommand(CCMD_GOTO))
	{
		if (_CheckConsoleCmdParamNum(2, 2))
		{
			A3DVECTOR3 v;
			v.x = l_aCmdTokens[1].ToFloat();
			v.z = l_aCmdTokens[2].ToFloat();
			v.y = 1.0f;
			g_pGame->GetGameSession()->c2s_CmdGoto(v.x, v.y, v.z);
		}
	}
	else if (CompConsoleCommand(CCMD_GOTOLINE))
	{
		// 		if (_CheckConsoleCmdParamNum(4, 4))
		// 		{
		// 			int line = l_aCmdTokens[1].ToInt();
		// 			int instance = l_aCmdTokens[2].ToInt();
		// 			int x = l_aCmdTokens[3].ToInt();
		// 			int z = l_aCmdTokens[4].ToInt();
		// 			g_pGame->GetGameSession()->player_change_gs(line, instance, 0, x, z);
		// 		}
		if (_CheckConsoleCmdParamNum(1,1))
		{
			int line = l_aCmdTokens[1].ToInt();
			g_pGame->GetGameSession()->try_change_gs(line);
		}
	}
	else if (CompConsoleCommand(CCMD_FLY))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			if (l_aCmdTokens[1].ToInt())
				g_pGame->GetGameSession()->c2s_CmdStartFly();
			else
				g_pGame->GetGameSession()->c2s_CmdStopFly();
		}
	}
	else if (CompConsoleCommand(CCMD_C2SCMD))
	{
		if (_CheckConsoleCmdParamNum(1, 19))
		{
			int iNumParam = l_iNumCmdToken - 2;
			int iCmd = l_aCmdTokens[1].ToInt();
			
			if (iNumParam <= 0)
				c2s_SendDebugCmd(iCmd, 0);
			else if (iNumParam == 1)
				c2s_SendDebugCmd(iCmd, 1, l_aCmdTokens[2].ToInt());
			else if (iNumParam == 2)
				c2s_SendDebugCmd(iCmd, 2, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt());
			else if (iNumParam == 3)
				c2s_SendDebugCmd(iCmd, 3, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt());
			else if (iNumParam == 4)
				c2s_SendDebugCmd(iCmd, 4, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt());
			else if (iNumParam == 5)
				c2s_SendDebugCmd(iCmd, 5, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt());
			else if (iNumParam == 6)
				c2s_SendDebugCmd(iCmd, 6, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt());
			else if (iNumParam == 7)
				c2s_SendDebugCmd(iCmd, 7, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt());
			else if (iNumParam == 8)
				c2s_SendDebugCmd(iCmd, 8, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt());
		}
	}
	else if (CompConsoleCommand(CCMD_C2SCMDP))
	{
		if (_CheckConsoleCmdParamNum(1, 19))
		{
			int iNumParam = l_iNumCmdToken - 2;
			int iCmd = l_aCmdTokens[1].ToInt();
			
			if (iNumParam <= 0)
				c2s_SendDebugCmd1(iCmd, 0);
			else if (iNumParam == 1)
				c2s_SendDebugCmd1(iCmd, 1, l_aCmdTokens[2].ToInt());
			else if (iNumParam == 2)
				c2s_SendDebugCmd1(iCmd, 2, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt());
			else if (iNumParam == 3)
				c2s_SendDebugCmd1(iCmd, 3, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt());
			else if (iNumParam == 4)
				c2s_SendDebugCmd1(iCmd, 4, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt());
			else if (iNumParam == 5)
				c2s_SendDebugCmd1(iCmd, 5, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt());
			else if (iNumParam == 6)
				c2s_SendDebugCmd1(iCmd, 6, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt());
			else if (iNumParam == 7)
				c2s_SendDebugCmd1(iCmd, 7, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt());
			else if (iNumParam == 8)
				c2s_SendDebugCmd1(iCmd, 8, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt());
			else if (iNumParam == 9)
				c2s_SendDebugCmd1(iCmd, 9, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt());
			else if (iNumParam == 10)
				c2s_SendDebugCmd1(iCmd, 10, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), l_aCmdTokens[11].ToInt());
			else if (iNumParam == 11)
				c2s_SendDebugCmd1(iCmd, 11, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt());
			else if (iNumParam == 12)
				c2s_SendDebugCmd1(iCmd, 12, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), 
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt());
			else if (iNumParam == 13)
				c2s_SendDebugCmd1(iCmd, 13, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), 
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt());
			else if (iNumParam == 14)
				c2s_SendDebugCmd1(iCmd, 14, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), 
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt());
			else if (iNumParam == 15)
				c2s_SendDebugCmd1(iCmd, 15, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), 
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt(), l_aCmdTokens[16].ToInt());
			else if (iNumParam == 16)
				c2s_SendDebugCmd1(iCmd, 16, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(), 
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt(), 
				l_aCmdTokens[16].ToInt(), l_aCmdTokens[17].ToInt());
			else if (iNumParam == 17)
				c2s_SendDebugCmd1(iCmd, 17, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(),  
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt(), 
				l_aCmdTokens[16].ToInt(), l_aCmdTokens[17].ToInt(), l_aCmdTokens[18].ToInt());
			else if (iNumParam == 18)
				c2s_SendDebugCmd1(iCmd, 18, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(),  
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt(), 
				l_aCmdTokens[16].ToInt(), l_aCmdTokens[17].ToInt(), l_aCmdTokens[18].ToInt(), l_aCmdTokens[19].ToInt());
			else if (iNumParam == 19)
				c2s_SendDebugCmd1(iCmd, 19, l_aCmdTokens[2].ToInt(), l_aCmdTokens[3].ToInt(), l_aCmdTokens[4].ToInt(), l_aCmdTokens[5].ToInt(), 
				l_aCmdTokens[6].ToInt(), l_aCmdTokens[7].ToInt(), l_aCmdTokens[8].ToInt(), l_aCmdTokens[9].ToInt(), l_aCmdTokens[10].ToInt(),  
				l_aCmdTokens[11].ToInt(), l_aCmdTokens[12].ToInt(), l_aCmdTokens[13].ToInt(), l_aCmdTokens[14].ToInt(), l_aCmdTokens[15].ToInt(), 
				l_aCmdTokens[16].ToInt(), l_aCmdTokens[17].ToInt(), l_aCmdTokens[18].ToInt(), l_aCmdTokens[19].ToInt(), l_aCmdTokens[20].ToInt());
		}
	}
	else if (CompConsoleCommand(CCMD_VIEWRAIDUS))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
			pGameCfg->SetSceneLoadRadius(l_aCmdTokens[1].ToFloat());
	}
	else if (CompConsoleCommand(CCMD_RELOGIN))
	{
		//	Re-login
		g_pGame->GetGameSession()->c2s_CmdLogout(_PLAYER_LOGOUT_HALF);
	}
	else if (CompConsoleCommand(CCMD_CASTSKILL))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
			pGameRun->GetHostPlayer()->ApplySkillShortcut(l_aCmdTokens[1].ToInt());
	}
	else if (CompConsoleCommand(CCMD_RENDERWATER))
	{
		EC_SYSTEM_SETTING ss = pGameCfg->GetSystemSettings();
		ss.nWaterEffect = l_aCmdTokens[1].ToInt();
		pGameCfg->SetSystemSettings(ss);
	}
	else if (CompConsoleCommand(CCMD_RENDERGRASS))
	{
		pGameCfg->m_bShowGrassLand = !pGameCfg->m_bShowGrassLand;
	}
	else if (CompConsoleCommand(CCMD_RENDERFOREST))
	{
		pGameCfg->m_bShowForest = !pGameCfg->m_bShowForest;
	}
	else if (CompConsoleCommand(CCMD_RENDERSHADOW))
	{
		EC_SYSTEM_SETTING ss = pGameCfg->GetSystemSettings();
		ss.bShadow = !ss.bShadow;
		pGameCfg->SetSystemSettings(ss);
	}
	else if (CompConsoleCommand(CCMD_RENDEROUTLINE))
	{
		//EC_SYSTEM_SETTING ss = pGameCfg->GetSystemSettings();
		//ss.bGroundOutline = !ss.bGroundOutline;
		//pGameCfg->SetSystemSettings(ss);
	}
	else if (CompConsoleCommand(CCMD_TURNAROUND))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			EC_GAME_SETTING gs = pGameCfg->GetGameSettings();
			gs.bTurnaround = l_aCmdTokens[1].ToInt() ? false : true;
			pGameCfg->SetGameSettings(gs);
		}
	}
	else if (CompConsoleCommand(CCMD_TESTDIST))
	{
		pGameCfg->SetTestDistFlag(!pGameCfg->GetTestDistFlag());
	}
	else if (CompConsoleCommand(CCMD_PLAYGFX))
	{
		if (_CheckConsoleCmdParamNum(1, 3))
		{
			//	Play specified gfx near host player
			A3DVECTOR3 vPos = pGameRun->GetHostPlayer()->GetPos();
			if (l_iNumCmdToken >= 3)
			{
				int n = l_aCmdTokens[2].ToInt();
				if (n == 1)
					vPos = pGameRun->GetHostPlayer()->GetPlayerAABB().Center;
				else if (n == 2)
					vPos += g_vAxisX * 2.0f + g_vAxisY;
			}

			float fScale = 0.0f;
			if (l_iNumCmdToken >= 4)
				fScale = l_aCmdTokens[3].ToFloat();

			g_pGame->GetGFXCaster()->PlayAutoGFXEx(l_aCmdTokens[1], 1000, vPos, g_vAxisZ, g_vAxisY, fScale);
		}
	}
	else if (CompConsoleCommand(CCMD_SHOWPOS))
	{
		pGameCfg->SetShowPosFlag(!pGameCfg->GetShowPosFlag());
	}
	else if (CompConsoleCommand(CCMD_SKILLNOTARGET))
	{
		pGameCfg->SetSkillNoTarget(!pGameCfg->GetSkillNoTarget());

		char szStr[128];
		sprintf(szStr, "Skill No Target: %d", pGameCfg->GetSkillNoTarget());

		szStr[strlen(szStr)] = '\0';
		AddConsoleLine( AS2AC(szStr), l_colGreen);
	}
	else if (CompConsoleCommand(CCMD_TRNLAYER))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			//EC_SYSTEM_SETTING ss = pGameCfg->GetSystemSettings();
			//ss.nGroundDetail = l_aCmdTokens[1].ToInt();
			//a_ClampFloor(ss.nGroundDetail, (BYTE)0);
			//pGameCfg->SetSystemSettings(ss);
		}
	}
	else if (CompConsoleCommand(CCMD_A3DSTAT))
	{
		g_pA3DConfig->RT_SetShowPerformanceFlag(!g_pA3DConfig->RT_GetShowPerformanceFlag());
	}
	else if (CompConsoleCommand(CCMD_GAMESTAT))
	{
		pGameCfg->SetShowGameStatFlag(!pGameCfg->GetShowGameStatFlag());
	}
	else if (CompConsoleCommand(CCMD_TREELOD))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			float fLOD = l_aCmdTokens[1].ToFloat();
			pGameCfg->SetForestDetail(fLOD);
		}
	}
	else if (CompConsoleCommand(CCMD_FPS))
	{
		g_pA3DConfig->RT_SetShowFPSFlag(!g_pA3DConfig->RT_GetShowFPSFlag());
	}
	else if (CompConsoleCommand(CCMD_PVRADIUS))
	{
		float fRadius = l_aCmdTokens[1].ToFloat();
		pGameCfg->SetPlayerVisRadius(fRadius);
	}
	else if (CompConsoleCommand(CCMD_SHOWID))
	{
		pGameCfg->SetShowIDFlag(!pGameCfg->GetShowIDFlag());
	}
	else if (CompConsoleCommand(CCMD_SKIPFRAME))
	{
		pGameCfg->SetSkipFrameFlag(!pGameCfg->GetSkipFrameFlag());
	}
	else if (CompConsoleCommand(CCMD_MODELUPDATE))
	{
		pGameCfg->SetModelUpdateFlag(!pGameCfg->GetModelUpdateFlag());
	}
	else if (CompConsoleCommand(CCMD_MINIDUMP))
	{
		int* p = NULL;
		*p = 0;
	}
	else if (CompConsoleCommand(CCMD_SETTIMEOFDAY))
	{
		if( _CheckConsoleCmdParamNum(2, 2) )
		{
			int hour = l_aCmdTokens[1].ToInt();
			int minitute = l_aCmdTokens[2].ToInt();
			if( hour < 0 ) hour = 0;
			if( hour > 23 ) hour = 23;
			if( minitute < 0 ) minitute = 0;
			if( minitute > 59 ) minitute = 59;
			float t = (hour + minitute / 60.0f) / 24.0f; 
			g_pGame->GetGameRun()->GetWorld()->SetTimeOfDay(t);
		}
	}
	else if (CompConsoleCommand(CCMD_GETSERVERTIME))
	{
		struct tm servertime = g_pGame->GetServerLocalTime();
		char szTime[128];
		strcpy(szTime, asctime(&servertime));
		if( szTime[strlen(szTime) - 1] == '\n' )
			szTime[strlen(szTime) - 1] = '\0';
		AddConsoleLine( GetStringFromTable(799) + AS2AC(szTime), l_colGreen);
	}
	else if (CompConsoleCommand(CCMD_TASK))
	{
		if (_CheckConsoleCmdParamNum(2, 2))
		{
			int func = l_aCmdTokens[1].ToInt();

			switch (func)
			{
			case 1:
				g_pGame->GetTaskTemplateMan()->ManualTrigTask(
					reinterpret_cast<TaskInterface*>(g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface()),
					l_aCmdTokens[2].ToInt());
				break;
			case 2:
				g_pGame->GetTaskTemplateMan()->ForceGiveUpTask(
					reinterpret_cast<TaskInterface*>(g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface()),
					l_aCmdTokens[2].ToInt());
				break;
			case 3:
				g_pGame->GetTaskTemplateMan()->ForceRemoveFinishTask(
					reinterpret_cast<TaskInterface*>(g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface()),
					l_aCmdTokens[2].ToInt());
				break;
			}
		}
	}
	else if (CompConsoleCommand(CCMD_SETBIAS))
	{
		if( _CheckConsoleCmdParamNum(1, 1) )
		{
			float v = l_aCmdTokens[1].ToFloat(); 
			for(int i=0; i<m_pA3DDevice->GetMaxSimultaneousTextures(); i++)
			{
#ifdef BACK_VERSION
				m_pA3DDevice->SetDeviceTextureStageState(i, D3DTSS_MIPMAPLODBIAS, *(DWORD *)&v);
#else
				m_pA3DDevice->SetSamplerState(i, D3DSAMP_MIPMAPLODBIAS, *(DWORD *)&v);
#endif
			}
		}
	}
#ifdef LUA_UI
	else if (CompConsoleCommand(CCMD_UPDATELUA))
	{
		g_LuaFileMan.Reload();
	}
#endif
	else if (CompConsoleCommand(CCMD_TERRAINCULLER))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			int val = l_aCmdTokens[1].ToInt();
			A3DTerrain2Cull* pTrnCull = g_pGame->GetGameRun()->GetWorld()->GetTerrainCuller();
			if (pTrnCull)
				pTrnCull->Enable(val ? true : false);
		}
	}
	else if (CompConsoleCommand(CCMD_DEBUGGSCMD))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
			pSession->c2s_CmdDebugGSCmd(l_aCmdTokens[1]);
	}
	else if (CompConsoleCommand(CCMD_DEBUGDELIVERCMD))
	{
		if (_CheckConsoleCmdParamNum(2, 2))
		{
			int iType = l_aCmdTokens[1].ToInt();
			pSession->c2s_CmdDebugDeliverCmd((WORD)iType, l_aCmdTokens[2]);
		}
	}
	else if (CompConsoleCommand(CCMD_FIXEDCAMERA))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			g_pGame->GetGameRun()->GetHostPlayer()->GetCameraCtrl()->FixCamPitch_Set(l_aCmdTokens[1].ToInt() != 0);
		}
	}
	else if (CompConsoleCommand(CCMD_TOWERDEFENSE))
	{
// 		PAUIDIALOG pShow = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("Win_TowerDefense");
// 		if (pShow)
// 			pShow->Show(!pShow->IsShow());
	}
	else if (CompConsoleCommand(CCMD_CAMTURNSPEED))
	{
		EC_GAME_SETTING gs = pGameCfg->GetGameSettings();
		gs.fCamTurnSpeed = 10.0f;
		gs.fCamZoomSpeed = 1.0f;
		pGameCfg->SetGameSettings(gs);
	}
	else if (CompConsoleCommand(CCMD_PLAYCUTSCENE))
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
			g_pGame->GetGameRun()->GetHostPlayer()->GetCutscene()->StartCutscene(l_aCmdTokens[1].ToInt(), false);
		}
	}
	else if(CompConsoleCommand(CCMC_BUYREPUITEM))
	{
		g_pGame->GetGameRun()->GetHostPlayer()->BuyItemsFromShopReputation();
	}
	else if(CompConsoleCommand(CCMD_GAMESNAKE))
	{
 		PAUIDIALOG pShow = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan()->GetDialog("GameSnake");
 		if (pShow)
 			pShow->Show(!pShow->IsShow());
	}
	else if(CompConsoleCommand(CCMD_TINYGAME))
	{
		if(_CheckConsoleCmdParamNum(1, 1))
		{
			PlayTinyGame(l_aCmdTokens[1].ToInt());	
		}

	}
	else if(CompConsoleCommand(CCMD_MANUALTASK))
	{
		if(_CheckConsoleCmdParamNum(1, 1))
			g_pGame->GetTaskTemplateMan()->ManualTrigTask(g_pGame->GetGameRun()->GetHostPlayer()->GetTaskInterface(), l_aCmdTokens[1].ToInt());
	}
	else if (CompConsoleCommand(CCMD_ANM))
	{
//#ifndef BACK_VERSION
		
		bool bAnmId = true;
		const AString& param = l_aCmdTokens[1];
		int len = param.GetLength();
		for (int i = 0; i < len; ++ i)
		{
			const char& c = param[i];
			if (c < '0' || c > '9')
			{
				bAnmId = false;
				break;
			}
		}
		
		if (bAnmId)
		{
			int id = l_aCmdTokens[1].ToInt();
			if (id != 0)
				bAnmId = g_pGame->GetGameRun()->GetAnmSystem()->Play(id);
		}
		
		if (!bAnmId)
		{
			ACString path;
			path.Format(_AL("configs\\Animations\\%s"), AS2AC(l_aCmdTokens[1]));
			g_pGame->GetGameRun()->GetAnmSystem()->Play(path);
		}

//#endif
	}
	else if (stricmp(l_aCmdTokens[0], "d_adjustcam") == 0)
	{
		if (_CheckConsoleCmdParamNum(2,2))
		{
			pGameCfg->SetCamDis(l_aCmdTokens[1].ToFloat());
			pGameCfg->SetPlayerDeg(l_aCmdTokens[2].ToInt());
		}
	}
	else if (stricmp(l_aCmdTokens[0], "d_contrast") == 0)
	{
		if (_CheckConsoleCmdParamNum(3, 3))
		{
			g_pGame->GetGameRun()->GetFullGlowRender()->SesContrastOn(l_aCmdTokens[1].ToInt() != 0);
			g_pGame->GetGameRun()->GetFullGlowRender()->SetContrastPower(l_aCmdTokens[2].ToFloat());
			g_pGame->GetGameRun()->GetFullGlowRender()->SetContrastMidLum(l_aCmdTokens[3].ToFloat());
		}
	}	
	else if (stricmp(l_aCmdTokens[0], "d_skillall") == 0)
	{
		for (int i=1; i<=12; i++)
		{
			if (i==11)
			{
				continue;
			}
			for (int j=8; j<=12; j++)
			{
				AString strDlg, strObj;
				strDlg.Format("Win_Skill_Taoism2_%d", i);
				strObj.Format("Rdo_%d", j);
				GetDialog(strDlg)->GetDlgItem(strObj)->Enable(true);
				strDlg.Format("Win_Skill_Evil2_%d", i);
				strObj.Format("Rdo_%d", j);
				GetDialog(strDlg)->GetDlgItem(strObj)->Enable(true);
				strDlg.Format("Win_Skill_Buddhism2_%d", i);
				strObj.Format("Rdo_%d", j);
				GetDialog(strDlg)->GetDlgItem(strObj)->Enable(true);
			}
		}
	}
	else if (stricmp(l_aCmdTokens[0], "d_blacklight") == 0)
	{
		if (_CheckConsoleCmdParamNum(1, 1))
		{
//			g_nDebugBlackAndLightValue = l_aCmdTokens[1].ToInt();
			DWORD dwForceChangeMask = 0;
			dwForceChangeMask |= ( 1 << EQUIPIVTR_HEAD );
			dwForceChangeMask |= ( 1 << EQUIPIVTR_BODY );
			dwForceChangeMask |= ( 1 << EQUIPIVTR_FOOT );
			g_pGame->GetGameRun()->GetHostPlayer()->SetForceChangeMask(dwForceChangeMask);
			g_pGame->GetGameRun()->GetHostPlayer()->SetDarkLightState( l_aCmdTokens[1].ToInt() );
			g_pGame->GetGameRun()->GetHostPlayer()->UpdateEquipSkins();
		}
	}
	else
	{
		//	Respond: unknown command
		AddConsoleLine(_AL("Unknown command."), l_colRed);
	}

	if (l_bInvalidParam)
		AddConsoleLine(_AL("Wrong parameter number."), l_colRed);
}

//	Parse GM console command
void CECGameUIMan::ParseGMConsoleCommand()
{
	CECGameSession* pSession = g_pGame->GetGameSession();
	GNET::Privilege* pPrivilege = g_pGame->GetPrivilege();
	CECConfigs* pGameCfg = g_pGame->GetConfigs();
	CECHostPlayer* pHost = g_pGame->GetGameRun()->GetHostPlayer();

	if (CompConsoleCommand(CCMD_GM_KICKOUTROLE))
	{
		if (pPrivilege->Has_Force_Offline() && _CheckConsoleCmdParamNum(3, 3))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			int iTime = l_aCmdTokens[2].ToInt();
			ACString strReason = AS2AC(l_aCmdTokens[3]);
			pSession->gm_KickOutRole(idPlayer, iTime, strReason);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_KICKOUTUSER))
	{
		if (pPrivilege->Has_Force_Offline() && _CheckConsoleCmdParamNum(3, 3))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			int iTime = l_aCmdTokens[2].ToInt();
			ACString strReason = AS2AC(l_aCmdTokens[3]);
			pSession->gm_KickOutUser(idPlayer, iTime, strReason);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_LISTONLINEUSER))
	{
		if (pPrivilege->Has_ListUser())
		{
		}
	}
	else if (CompConsoleCommand(CCMD_GM_ONLINENUMBER))
	{
		if (pPrivilege->Has_ListUser())
			pSession->gm_OnlineNumber();
	}
	else if (CompConsoleCommand(CCMD_GM_RESTARTSERVER))
	{
		if (pPrivilege->Has_Shutdown_GameServer() && _CheckConsoleCmdParamNum(1, 1))
		{
			int iTime = l_aCmdTokens[1].ToInt();
			pSession->gm_RestartServer(-1, iTime);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_SHUTUPROLE))
	{
		if (pPrivilege->Has_Forbid_Talk() && _CheckConsoleCmdParamNum(3, 3))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			int iTime = l_aCmdTokens[2].ToInt();
			ACString strReason = AS2AC(l_aCmdTokens[3]);
			pSession->gm_ShutupRole(idPlayer, iTime, strReason);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_SHUTUPUSER))
	{
		if (pPrivilege->Has_Forbid_Talk() && _CheckConsoleCmdParamNum(3, 3))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			int iTime = l_aCmdTokens[2].ToInt();
			ACString strReason = AS2AC(l_aCmdTokens[3]);
			pSession->gm_ShutupUser(idPlayer, iTime, strReason);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_MOVETOPLAYER))
	{
		if (pPrivilege->Has_MoveTo_Role() && _CheckConsoleCmdParamNum(1, 1))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			pSession->gm_MoveToPlayer(idPlayer);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_CALLINPLAYER))
	{
		if (pPrivilege->Has_Fetch_Role() && _CheckConsoleCmdParamNum(1, 1))
		{
			int idPlayer = l_aCmdTokens[1].ToInt();
			pSession->gm_CallInPlayer(idPlayer);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_BROADCAST))
	{
		if (pPrivilege->Has_Broadcast() && _CheckConsoleCmdParamNum(1, 1))
		{
			ACString str = AS2AC(l_aCmdTokens[1]);
			pSession->SendChatData(GP_CHAT_BROADCAST, str);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_SHOWID))
	{
		if (pPrivilege->Has_Toggle_NameID())
			pGameCfg->SetShowIDFlag(!pGameCfg->GetShowIDFlag());
	}
	else if (CompConsoleCommand(CCMD_GM_FORBIDROLE))
	{
		if (pHost->IsGM() && _CheckConsoleCmdParamNum(3, 3))
		{
			int iType = l_aCmdTokens[1].ToInt();
			int idPlayer = l_aCmdTokens[2].ToInt();
			int iTime = l_aCmdTokens[3].ToInt();
			ACString strReason = AS2AC(l_aCmdTokens[4]);
			pSession->gm_ForbidRole(iType, idPlayer, iTime, strReason);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_TRIGGERCHAT))
	{
		if (pPrivilege->Has_Chat_OrNot() && _CheckConsoleCmdParamNum(1, 1))
		{
			int iFlag = l_aCmdTokens[1].ToInt();
			pSession->gm_TriggerChat(iFlag ? true : false);
		}
	}
	else if (CompConsoleCommand(CCMD_GM_GENERATE))
	{
		if (pHost->IsGM() && _CheckConsoleCmdParamNum(1, 1))
		{
			int tid = l_aCmdTokens[1].ToInt();
			pSession->gm_Generate(tid);
		}
	}
	else
	{
		//	Respond: unknown command
		AddConsoleLine(_AL("Unknown command."), l_colRed);
	}
	
	if (l_bInvalidParam)
		AddConsoleLine(_AL("Wrong parameter number."), l_colRed);
}

