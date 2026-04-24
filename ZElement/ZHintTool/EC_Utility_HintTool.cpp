//
//
//
//-----------------------------------------------------------------------------
//
#define _WIN32_WINDOWS 0x0410 

#include "EC_Global.h"
#include <winbase.h>
#include <dbghelp.h>
#pragma comment(lib, "dbghelp.lib")

//-----------------------------------------------------------------------------
bool g_bExceptionOccured = false;

//-----------------------------------------------------------------------------

unsigned long glb_HandleException(LPEXCEPTION_POINTERS pExceptionPointers)
{
	TCHAR szFile[MAX_PATH];
	extern DWORD GAME_BUILD;
	wsprintf(szFile, _AL("%S\\Logs\\ec_build%d.dmp"), g_szWorkDir, GAME_BUILD);
	
	HANDLE hDumpFile = ::CreateFile(szFile, GENERIC_READ | GENERIC_WRITE, 
		FILE_SHARE_WRITE | FILE_SHARE_READ, 0, CREATE_ALWAYS, 0, 0);
	if (hDumpFile == INVALID_HANDLE_VALUE)
		return -1;

	
	MINIDUMP_EXCEPTION_INFORMATION stInfo = {0};
	
	stInfo.ThreadId = GetCurrentThreadId();
	stInfo.ExceptionPointers = pExceptionPointers;
	stInfo.ClientPointers = TRUE;

	BOOL bOK = MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile,
		MiniDumpNormal, &stInfo, NULL, NULL);
	if (bOK)
	{
		a_LogOutput( 1, "MiniDump Write OK\n" );
	}
	else
	{
		a_LogOutput( 1, "MiniDump Write Failed\n" );

	}

	return 0;
//	if( IsDebuggerPresent() )
//		return EXCEPTION_CONTINUE_SEARCH;

	/*
	//	Use current time to build file name
	FILETIME CurrentTime;
	GetSystemTimeAsFileTime(&CurrentTime);
	TCHAR szTimeBuffer[100];
	_FormatTime(szTimeBuffer, CurrentTime);
	
	TCHAR szFile[MAX_PATH];

#ifdef UNICODE
	wsprintf(szFile, _AL("%S\\Logs\\ec_%s.dmp"), g_szWorkDir, szTimeBuffer);
#else
	wsprintf(szFile, _AL("%s\\Logs\\ec_%s.dmp"), g_szWorkDir, szTimeBuffer);
#endif
	*/
/*
	TCHAR szFile[MAX_PATH];
	extern DWORD GAME_BUILD;
	wsprintf(szFile, _AL("%S\\Logs\\ec_build%d.dmp"), g_szWorkDir, GAME_BUILD);

	if( GetVersion() < 0x80000000 )
	{
		// WinNT or Win2000
		AMiniDump::Create(NULL, pExceptionPointers, szFile, &g_csException);
	}
	else
	{
		// Win95 or Win98 or Win32		
	}

	// if dump success, we should launch bug report program here
	STARTUPINFO si;
	ZeroMemory(&si, sizeof(si));
	si.cb = sizeof(si);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = SW_SHOW;
	PROCESS_INFORMATION pi;
	ZeroMemory(&pi, sizeof(pi));
	CreateProcess(L"ReportBugs\\ZReportBugs.exe", NULL, NULL, NULL, FALSE, 0, NULL, L"ReportBugs", &si, &pi);

	g_bExceptionOccured = true;

	return EXCEPTION_EXECUTE_HANDLER;
	*/
}

