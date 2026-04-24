#include "MiniDump.h"

LPTOP_LEVEL_EXCEPTION_FILTER __hPreviousExceptionFilter = NULL;

PDUMPWORK __fnDumpWork = NULL;

CMiniDump::CMiniDump()
{
	
}

BOOL CMiniDump::Begin()
{
	SetErrorMode( SEM_FAILCRITICALERRORS );
	__hPreviousExceptionFilter = ::SetUnhandledExceptionFilter( MyUnhandledExceptionFilter );
	return TRUE;
}

BOOL CMiniDump::End()
{
	if(__hPreviousExceptionFilter)
		::SetUnhandledExceptionFilter( __hPreviousExceptionFilter );
	return TRUE;
}

LONG CMiniDump::MyUnhandledExceptionFilter( struct _EXCEPTION_POINTERS *pExceptionInfo )
{
	struct tm *tm_;
	time_t  time_;
	time(&time_);
	tm_= localtime(&time_);
	
	char Date[64];
	wsprintf(Date,"_%d_%d_%d_%d_%d",tm_->tm_year + 1900,tm_->tm_mon + 1 ,tm_->tm_mday,tm_->tm_hour,tm_->tm_min);
	
	char FileName[MAX_PATH];
	GetModuleFileName(NULL, FileName, sizeof(FileName));
	
	char* ext = strrchr(FileName, '.');
	strcpy(ext ? ext : FileName+strlen(FileName),Date);
	strcat(FileName,".dmp");
	
	HANDLE hFile = CreateFile(FileName, GENERIC_READ|GENERIC_WRITE, 0, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	
	MINIDUMP_EXCEPTION_INFORMATION eInfo;
	eInfo.ThreadId   = GetCurrentThreadId();
	eInfo.ExceptionPointers = pExceptionInfo;
	eInfo.ClientPointers = FALSE;
	
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hFile, MiniDumpNormal, pExceptionInfo ? &eInfo : NULL, NULL, NULL);
	
	if(__fnDumpWork) (*__fnDumpWork)();
	
	return EXCEPTION_EXECUTE_HANDLER;
}

