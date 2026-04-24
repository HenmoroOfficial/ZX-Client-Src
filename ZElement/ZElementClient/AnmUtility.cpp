#include "AnmUtility.h"
#include "AnmSystem.h"
#include "EC_World.h"
#include "EC_Global.h"
#include <crtdbg.h>


#ifdef _DEBUG
#pragma comment(lib, "ImageHlp.lib")
#include <ImageHlp.h>
#include <sstream>

struct SymbolInfo
{
	const void*	addr;
	std::string fun_name;
	std::string file_name;
	DWORD       line_num;
};
static LPFNDEFLOGOUTPUT l_pfnOldLog;
static char l_szPDBFilePath[1024];
static bool _IsHexNum( char c ) { return isdigit(c) || c >= 'a' && c <= 'f' || c >= 'A' && c <= 'F'; }

static std::string _GetAddrStr( const std::string& strMsg, size_t nPos )
{
	if( nPos + 2 >= strMsg.size() || strMsg[ nPos ] != '0' || strMsg[ nPos + 1 ] != 'x' || ! _IsHexNum( strMsg[ nPos + 2 ] ) ) 
		return "";

	size_t nCnt = 0;
	while( nPos + nCnt + 2 < strMsg.size() && _IsHexNum( strMsg[ nPos + nCnt + 2 ] ) )
		++ nCnt;

	return std::string( & strMsg[ nPos ], & strMsg[ nPos + 2 + nCnt ] );
}

static HANDLE _InitSymForCurProc()
{
	static HANDLE s_hProc;
	static char s_szLastPath[1024];

	if( ! s_hProc || strcmp( s_szLastPath, l_szPDBFilePath ) )
	{
		strcpy( s_szLastPath, l_szPDBFilePath );
		SymSetOptions( SYMOPT_LOAD_LINES );
		s_hProc = GetCurrentProcess();
		BOOL bRet = SymInitialize( s_hProc, l_szPDBFilePath, true );
		ASSERT( bRet );
	}
	return s_hProc;
}

static SymbolInfo _AddrToSym( void* pAddr, HANDLE hProc )
{
	BOOL bRet;
	SymbolInfo si;
	si.addr = pAddr;

	DWORD dwAddr = reinterpret_cast< DWORD >( pAddr );
	char aBuff[ sizeof( SYMBOL_INFO ) + MAX_SYM_NAME * sizeof( TCHAR ) + sizeof( ULONG64 ) - 1 ];
	PSYMBOL_INFO pSymInfo = reinterpret_cast< PSYMBOL_INFO >( aBuff );
	pSymInfo->SizeOfStruct = sizeof( SYMBOL_INFO );
	pSymInfo->MaxNameLen = MAX_SYM_NAME;

	DWORD64 dw64Offset;
	bRet = SymFromAddr( hProc, dwAddr, & dw64Offset, pSymInfo );
	if( bRet )
		si.fun_name = pSymInfo->Name;

	IMAGEHLP_LINE line;
	line.SizeOfStruct = sizeof( IMAGEHLP_LINE );
	DWORD dwOffset = static_cast< DWORD >( dw64Offset );
	bRet = SymGetLineFromAddr( hProc, dwAddr, & dwOffset, & line );
	if( bRet )
	{
		si.file_name = line.FileName;
		si.line_num  = line.LineNumber;
	}
	return si;
}

std::string _SymToLine( const SymbolInfo& si )
{
	std::ostringstream os;
	if( ! si.file_name.empty() )
		os << si.file_name << "(" << si.line_num << "): ";
	if( ! si.fun_name.empty() )
	{
		os << "In function: " << si.fun_name; 
		os.clear();
		os << "() ";
	}
	if( si.addr )
		os << "0x" << std::hex << si.addr;
	return os.str();
}

std::string _AddrToMemLeakInfo( const char* szMsg )
{
	std::ostringstream oss;
	HANDLE hProc = _InitSymForCurProc();

	std::istringstream iss( szMsg );
	std::string strLine;
	while( std::getline( iss, strLine ) )
	{
		oss << strLine << "\n";
		size_t i = 0;
		while( i < strLine.size() )
		{
			std::string strAddr = _GetAddrStr( strLine, i );
			if( strAddr.empty() )
			{ 
				++ i;
				continue;
			}
			size_t nPos;
			sscanf( strAddr.c_str(), "%x", & nPos );
			SymbolInfo si = _AddrToSym( reinterpret_cast< void* >( nPos ), hProc );
			if( si.addr )
				oss << "\t" << _SymToLine( si ) << "\n";
			i += strAddr.size();
		}
	}
	return oss.str();
}

static void _IDEDebugReport( const char* szReport )
{
	std::istringstream iss( szReport );
	std::string strLine;
	while( std::getline( iss, strLine ) )
		_CrtDbgReport( _CRT_WARN, 0, 0, 0, ( strLine + "\n" ).c_str() );
}

static void _ShowMemLeakInfo( const char* szMsg )
{
	if( ! szMsg || ! *szMsg )
		return;
	std::string strInfo = _AddrToMemLeakInfo( szMsg );
	if( ! strInfo.empty() )
		_IDEDebugReport( strInfo.c_str() );
}

static void _DebugMemLeak( const char* szMsg )
{
	if( l_pfnOldLog ) 
		l_pfnOldLog( szMsg );
	_ShowMemLeakInfo( szMsg );
}

static void _OnExitProcMemDebug()
{
	l_pfnOldLog = a_RedirectDefLogOutput( _DebugMemLeak );
}

void EnablePDBMemDebug( const char* szPDBFilePath )
{
	if( szPDBFilePath && *szPDBFilePath )
		strcpy( l_szPDBFilePath, szPDBFilePath );
	atexit( _OnExitProcMemDebug );	// register
}


#endif

