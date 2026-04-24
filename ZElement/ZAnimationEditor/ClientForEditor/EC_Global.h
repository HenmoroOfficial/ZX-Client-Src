/*
 * FILE: GL_Global.h
 *
 * DESCRIPTION: Element client, Global definition and declaration
 *
 * CREATED BY: duyuxin, 2003/12/11
 *
 * HISTORY:
 *
 * Copyright (c) 2003 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#pragma warning (disable: 4786)

#include "ABaseDef.h"
#include "ALog.h"
#include "AMemory.h"
#include "A3DVector.h"

///////////////////////////////////////////////////////////////////////////
//
//	Define and Macro
//
///////////////////////////////////////////////////////////////////////////

//	Basic render size
#define BASERENDER_WIDTH	800
#define BASERENDER_HEIGHT	600

//	Default camera FOV
#define DEFCAMERA_FOV		56.0f

//	Define virtual key of `
#define	VK_CONSOLE			0xC0

//	Error code
enum
{
	ECERR_UNKNOWN = 0,			//	Unknown
	ECERR_INVALIDPARAMETER,		//	Invalid parameter
	ECERR_NOTENOUGHMEMORY,		//	Not enough memory
	ECERR_FILEOPERATION,		//	File operation
	ECERR_FAILEDTOCALL,			//	Failed to call function
	ECERR_WRONGVERSION,			//	Wrong version
};

class ALog;
class CECGame;

///////////////////////////////////////////////////////////////////////////
//
//	Types and Global variables
//
///////////////////////////////////////////////////////////////////////////

extern ALog				g_Log;
extern char				g_szWorkDir[];
extern char				g_szIniFile[];
extern CECGame*			g_pGame;

extern A3DVECTOR3		g_vOrigin;
extern A3DVECTOR3		g_vAxisX;
extern A3DVECTOR3		g_vAxisY;
extern A3DVECTOR3		g_vAxisZ;

extern CRITICAL_SECTION	g_csException;
extern CRITICAL_SECTION	g_csSession;
extern CRITICAL_SECTION	g_csRTDebug;

extern bool				g_bTrojanDumpLastTime;

///////////////////////////////////////////////////////////////////////////
//
//	Declare of Global functions
//
///////////////////////////////////////////////////////////////////////////

//	Initialize log system
bool glb_InitLogSystem(const char* szFile);
//	Close log system
void glb_CloseLogSystem();
//	Output predefined error
void glb_ErrorOutput(int iErrCode, const char* szFunc, int iLine);
//	Calculate a file's md5, return false if file missing or true with md5 returned
bool glb_CalcFileMD5(const char * szFile, BYTE md5[16]);

///////////////////////////////////////////////////////////////////////////
//
//	Inline functions
//
///////////////////////////////////////////////////////////////////////////


