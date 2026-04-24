// HintTool.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define _WIN32_WINDOWS 0x0410 
#include <windows.h>
#include <Winsock2.h>
#include "EC_Utility_HintTool.h"


#include "EC_Game_HintTool.h"
#include "EC_GameSession_HintTool.h"
/*//#include ".\Network\EC_HTPDataType.h"
#include "..\ZCommon\itemdataman.h"

CECGame* g_pGame = NULL;


void generate_item_guid(item_data*)
{
}
*/

#include "EC_GameRecord_HintTool.h"

game_record gr;

//-----------------------------------------------------------------------------
//#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>


//=============================================================================
#include <A3DTypes.h>
#include <AF.h>
#include "EC_Global.h"

//#include "LuaBind.h"
#include "LuaState.h"

bool InitApp( void )
{
	af_Initialize();

	//	Initialize log system
	glb_InitLogSystem("ZhuXian_HintTool.log");

//	char szFile[MAX_PATH];

	//	Set current directory as work directory
	GetCurrentDirectoryA(MAX_PATH, g_szWorkDir);

	strcpy(g_szIniFile, g_szWorkDir);
	strcat(g_szIniFile, "\\ElementClient.ini");

	af_SetBaseDir(g_szWorkDir);

	//
	return true;
}

void FinalizeApp( void )
{
	//	Close log system
	glb_CloseLogSystem();

	//finalize the AFile resouce.
	af_Finalize();
}


//-----------------------------------------------------------------------------
#include "EC_Inventory.h"
#include "A3DMacros.h"
#include "EC_IvtrItem.h"
unsigned char* g_pUInfoData = NULL;
int g_nIvtrSize = 0;
int g_iDataLength = 4096;
CECInventory* g_pInventory = NULL;

void readData( void )
{
	FILE* fp = fopen( "./TestFord.dat", "r+t" );
	if( !fp )
	{
		return;
	}
//	int ivtrSize = 0;
	fread( &g_nIvtrSize, sizeof(unsigned char), 1, fp );
	fread( &g_iDataLength, sizeof(int), 1, fp );
	//
	g_pUInfoData = new unsigned char[g_iDataLength];
	if( !g_pUInfoData )
	{
		return;
	}
	//
	fread( g_pUInfoData, sizeof( char ), g_iDataLength, fp );
	fclose( fp );
}

void freeGlobalData( void )
{
	if( g_pUInfoData )
	{
		delete []g_pUInfoData;
		g_pUInfoData = NULL;
	}

	//release the test inventory.
	A3DRELEASE(g_pInventory);
}

void writeData( void )
{
	FILE* fp = fopen( "./TestFord_Write.dat", "w+t" );
	if( !fp )
	{
		return;
	}
	//
	for( int i = 0; i < g_pInventory->GetSize(); ++i )
	{
		CECIvtrItem* pItem = g_pInventory->GetItem( i );
		if( pItem )
		{
			const wchar_t *pszHint = pItem->GetDesc( CECIvtrItem::DESC_NORMAL, g_pInventory );
			char* resultHint = new char[ wcslen(pszHint)*2 ];
			a_WideCharToMultiByte( pszHint, wcslen(pszHint), resultHint, wcslen(pszHint)*2 );
			fwrite( resultHint, sizeof( char ), wcslen(pszHint)*2, fp );
			fwrite( "\n", sizeof( char ), 1, fp );
			//free the memory.
			delete []resultHint;
			resultHint = NULL;

		}
	}

	//
	fclose( fp );
}

void setInventory( void )
{
	if( !g_pInventory )
	{
		g_pInventory = new CECInventory;
		if( !g_pInventory )
		{
			return;
		}
		//
		g_pInventory->Init( 0 );
	}

	//read data.
	readData();

	//set data.
	g_pInventory->ResetWithDetailData( g_nIvtrSize, g_pUInfoData, g_iDataLength );

	//write data.
	writeData();

	//free the global data.
	freeGlobalData();
}
//-----------------------------------------------------------------------------
int DEBUG_PRINT(const char* str, ... ) 
{
#ifdef _DEBUG 
	return printf( str ); 
#endif
	//
	return 0;
}

//-----------------------------------------------------------------------------
//For new version.
#include "gnconf.h"
#include "gnpollio.h"

//
bool InitNetwork()
{
	using namespace GNET;
	
	//startup the socket.
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		a_LogOutput(1, "_InitNetwork, Cannot initialize Windows Sockets !");
    	return false;
	}
	AString strFile = g_szWorkDir;
	strFile += "\\dbserver.conf";
	GNET::Conf::GetInstance(strFile);
	GNET::PollIO::Init();

	return true;
}

//
//	Finalize network module
void FinalizeNetwork()
{
	using namespace GNET;

	PollIO::Close();
	Sleep(1000);
}
//-----------------------------------------------------------------------------

//=============================================================================


static CECGame l_theGame;

int main(int argc, char* argv[])
{
	//for test.
//	_CrtMemState s1, s2, s3;
	//test end.
	//test
	
	//Initialize the basic log system and base dir.
	InitApp();   //Modified 2010-11-26.
	
	int i;
	for(i=1; i<argc; ++i)
	{
		if(strstr(argv[i], "xunbao"))
		{
			l_theGame.SetTooltype(CECGame::TT_XUNBAO);
			a_LogOutput(1, "Tool type: XunBao!");
		}
		else if(strstr(argv[i], "zhuti"))
		{
			l_theGame.SetTooltype(CECGame::TT_ZHUTI);
			a_LogOutput(1, "Tool type: ZhuTi!");
		}
	}
	
	//test, wheather the procedure is enter main.
	a_LogOutput( 1, "Enter main..\n" );
	//test end.
	CECGame::GAMEINIT GameInit;
	memset(&GameInit, 0, sizeof (GameInit));

	//init lua
	if (!g_LuaStateMan.Init(false))
	{
		a_LogOutput( 1, "Can not Initialize the lua state...\n" );//Can not init the lua state.
		return false;
	}

	//initialize the critical section.
	InitializeCriticalSection(&g_csRTDebug);
	InitializeCriticalSection(&g_csSession);
	//
//	InitApp();   //Modified 2010-11-26.
	//test memory leaks.
//	_CrtMemCheckpoint( &s1 );
	//test end.
	//test.
	a_LogOutput( 1,  "finish Init App..\n" );
	//test end.

	//Initialize the network.
	bool retValue = InitNetwork();
	//test.
	a_LogOutput( 1, "Init the network return: %d\n", retValue );
	//test end,

	bool ret = l_theGame.Init( GameInit );

	//test.
	a_LogOutput( 2, "finish Init Game.., return %d\n", ret );
	if( !ret )
	{
		a_LogOutput( 1, "Fail to Initialize the Game!\n" );
		return -1;
	}
	//test end.
	//
//	setInventory();
//_CrtDumpMemoryLeaks();
	
	//
	ret = l_theGame.GetGameSession()->Open();
	if( !ret )
	{
		a_LogOutput(1,  "Fail to open network!\n" );
		return -1;
	}

	__try
	//recv and send the info to server.
	{
		while( true )
		{
			l_theGame.GetGameSession()->ProcessNewProtocols();
			Sleep(300);//How long the interval is suitable.
		}

	}
	__except (glb_HandleException(GetExceptionInformation()))
	{
		::OutputDebugString(_AL("Exception occurred...\n"));
		a_LogOutput(1, "Exception occurred... mini dumped!");
		::ExitProcess(-1);
	}
	//
	l_theGame.Release();

	//test memory leaks.
/*	_CrtMemCheckpoint( &s2 );
	if( _CrtMemDifference( &s3, &s1, &s2 ) )
	{
		_CrtMemDumpStatistics( &s3 );
	}
*/	//test end.
//_CrtDumpMemoryLeaks();
	//
	g_LuaStateMan.Release();
//_CrtDumpMemoryLeaks();
	//delete the critical section.
	DeleteCriticalSection(&g_csRTDebug);

	//finalize the app.
	FinalizeApp();

	//
	FinalizeNetwork();

	//
	_CrtDumpMemoryLeaks();
	return 0;
}

