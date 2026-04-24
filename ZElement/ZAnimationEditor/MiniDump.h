#ifndef __MINIDUMP_H__
#define __MINIDUMP_H__

#pragma comment ( lib, "dbghelp.lib" )

#include <windows.h>
#include <dbghelp.h>
#include <time.h>

typedef VOID (*PDUMPWORK)();

class CMiniDump
{
public:
	CMiniDump();


public:
	static BOOL Begin();
	static BOOL End();
	
private:
	static LONG WINAPI MyUnhandledExceptionFilter( struct _EXCEPTION_POINTERS *pExceptionInfo );
};

extern PDUMPWORK __fnDumpWork;

#endif
