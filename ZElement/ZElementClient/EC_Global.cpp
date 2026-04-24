 /*
 * FILE: EC_Global.cpp
 *
 * DESCRIPTION: Element client, Global definition and declaration
 *
 * CREATED BY: duyuxin, 2003/12/11
 *
 * HISTORY:
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#define _WIN32_WINDOWS 0x0410 

#include "EC_Global.h"
#include "AF.h"

#include "Network/IOLib/gnoctets.h"
#include "Network/IOLib/gnsecure.h"

#define new A_DEBUG_NEW

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


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

ALog	g_Log;
char	g_szWorkDir[MAX_PATH];
char	g_szIniFile[MAX_PATH];

A3DVECTOR3	g_vOrigin(0.0f);
A3DVECTOR3	g_vAxisX(1.0f, 0.0f, 0.0f);
A3DVECTOR3	g_vAxisY(0.0f, 1.0f, 0.0f);
A3DVECTOR3	g_vAxisZ(0.0f, 0.0f, 1.0f);

CRITICAL_SECTION	g_csException;
CRITICAL_SECTION	g_csSession;
CRITICAL_SECTION	g_csRTDebug;

bool				g_bTrojanDumpLastTime = false;

//	Error messages
static char* l_aErrorMsgs[] = 
{
	"Unknown error.",			//	ECERR_UNKNOWN
	"Invalid parameter.",		//	ECERR_INVALIDPARAMETER	
	"Not enough memory.",		//	ECERR_NOTENOUGHMEMORY
	"File operation error.",	//	ECERR_FILEOPERATION	
	"Failed to call function.",	//	ECERR_FAILEDTOCALL	
	"Wrong version.",			//	ECERR_WRONGVERSION
};

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

static void _LogOutput(const char* szMsg)
{
	g_Log.Log(szMsg);
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////

//	Initialize log system
bool glb_InitLogSystem(const char* szFile)
{
#ifndef _INVENTORY_DESC_MAN //Added by Ford.W 2010-08-23.
	if (!g_Log.Init(szFile, "Element client log file"))
		return false;
#else
	if( !g_Log.Init(szFile, "ZhuXian Element-Hint-Tool log file") )
	{
		return false;
	}
#endif
	a_RedirectDefLogOutput(_LogOutput);
	return true;
}

//	Close log system
void glb_CloseLogSystem()
{
	g_Log.Release();
	a_RedirectDefLogOutput(NULL);
}

//	Output predefined error
void glb_ErrorOutput(int iErrCode, const char* szFunc, int iLine)
{
	char szMsg[1024];
	sprintf(szMsg, "%s: %s (line: %d)", szFunc, l_aErrorMsgs[iErrCode], iLine);
	a_LogOutput(1, szMsg);
}

//	Calculate a file's md5
bool glb_CalcFileMD5(const char * szFile, BYTE md5[16])
{
	AFileImage file;
	if( !file.Open(szFile, AFILE_BINARY | AFILE_OPENEXIST) )
		return false;

	memset(md5, 0, 16);

	GNET::Octets input(file.GetFileBuffer(), file.GetFileLength());
	GNET::Octets output = GNET::MD5Hash::Digest(input);

	memcpy(md5, output.begin(), min(16, output.size()));

	return true;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Below codes are used to deal with some Trojan or virus which may do harm to our exe file in memory
//////////////////////////////////////////////////////////////////////////////////////////////////////
struct EXE_HOT_SPOT
{
	DWORD	offset;
	DWORD	len;
};

class TEMP_BUFFER
{
private:
	BYTE *	m_pData;
	int		m_nDataLen;

public:
	TEMP_BUFFER()
		: m_pData(NULL)
	{
	}

	~TEMP_BUFFER()
	{
		if( m_pData )
		{
			delete [] m_pData;
			m_pData = NULL;
		}
	}

	void SetData(BYTE * pData, int nLen)
	{
		if( m_pData )
		{
			delete [] m_pData;
			m_pData = NULL;
		}

		m_pData = new BYTE[nLen];
		memcpy(m_pData, pData, nLen);
		m_nDataLen = nLen;
	}

	BYTE * GetData() { return m_pData; }
	int GetDataLen() { return m_nDataLen; }
};
