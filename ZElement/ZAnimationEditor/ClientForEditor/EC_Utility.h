/*
 * FILE: EC_Utility.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/9/2
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DVector.h"
#include "Aassist.h"
#include <time.h>
#include <hashtab.h>
#include <hashmap.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Macros used to implement 16.16 fix point calculation
#define	INTTOFIX16(x)		(((int)(x)) << 16)
#define FIX16TOINT(x)		(((x) + 0x00008000) >> 16)
#define FIX16TOINTCEIL(x)	(((x) + 0x0000ffff) >> 16)
#define FIX16TOINTFLOOR(x)	((x) >> 16)
#define FLOATTOFIX16(x)		((int)((x) * 65536.0f + 0.5f))
#define FIX16TOFLOAT(x)		((x) / 65536.0f)

#define FLOATTOFIX8(x)		((short)((x) * 256.0f + 0.5f))
#define FIX8TOFLOAT(x)		((x) / 256.0f)

#define FIX16MUL(M1, M2)	(int)(((__int64)(M1) * (M2) + 0x00008000) >> 16)
#define FIX16DIV(D1, D2)	(int)(((__int64)(D1) << 16) / (D2))

//	Check whether a position value is valid
#define INVALIDPOS			9000000.0f
#define POSISVALID(x)		((x) < INVALIDPOS-1.0f && (x) > -INVALIDPOS+1.0f)

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

//	Struct used by gl_StepSlideMove and gl_SlideMove
struct SLIDEMOVE
{
	A3DVECTOR3	vStart;			//	(in) Start position
	A3DVECTOR3	vExts;			//	(in) Extents
	A3DVECTOR3	vVelocity;		//	(in) Velocity
	float		fTime;			//	(in) Time
	float		fStepHeight;	//	(in) Step height
	float		fVertVel;		//	(in) Additional vertical velocity
	float		fClipY;			//	(in) If 0 < Normal.y of side < fClipY, vel.y will be clipped

	A3DVECTOR3	vDest;			//	(out) Destination point
};

#ifdef BACK_VERSION
class CECViewport;
#else
class ECViewport;
#define CECViewport ECViewport
#endif
class A3DSkin;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

//	File operations
bool glb_CreateDirectory(const char* szDir);
bool glb_FileExist(const char* szFile);
void glb_ChangeExtension(char* szFile, char* szNewExt);
void glb_ClearExtension(char* szFile);
bool glb_WriteString(FILE* fp, const char* str);

bool glb_PercentProbability(int iPercent);
bool glb_PercentProbability(float fPercent);
A3DVECTOR3 glb_RandomVectorH(int* piDegree=NULL);
A3DVECTOR3 glb_RandomVector();

BYTE glb_CompressDirH(float x, float z);
A3DVECTOR3 glb_DecompressDirH(BYTE byDir);

int glb_IntCompare(const void* arg1, const void* arg2);
int glb_WordCompare(const void* arg1, const void* arg2);

extern bool g_bExceptionOccured;
unsigned long glb_HandleException(LPEXCEPTION_POINTERS pExceptionPointers);

//	Get command line parameters
bool glb_IsConsoleEnable();

tm* glb_GetFormatLocalTime();

ACString glb_FilterEditboxText(const ACHAR* szText);

#define FATAL_ERROR_LOAD_BUILDING 1
#define FATAL_ERROR_WRONG_CONFIGDATA 2
extern DWORD g_dwFatalErrorFlag;

bool glb_ConvertBmpToJpeg(const char* szBmp, const char* szJpeg, int nQuality);
void glb_ReplaceShader(A3DSkin * pSkin);

template <class _Key, class _Value, class _HashFunc, class _Allocator>
void CopyHashMap(abase::hash_map<_Key,_Value,_HashFunc,_Allocator>& target, const abase::hash_map<_Key,_Value,_HashFunc,_Allocator>& src)
{
	if (&target == &src)
	{
		return;
	}
	target.clear();
	abase::hash_map<_Key,_Value,_HashFunc,_Allocator>::const_iterator it;
	for (it=src.begin(); it!=src.end(); ++it)
	{
		target[it->first] = it->second;
	}
}

template <class _Value, class _Key, class _HashFunc,class _Allocator>
void CopyHashTab(abase::hashtab<_Value,_Key,_HashFunc,_Allocator>& target, const abase::hashtab<_Value,_Key,_HashFunc,_Allocator>& src)
{
	if (&target == &src)
	{
		return;
	}
	target.clear();
	abase::hashtab<_Value,_Key,_HashFunc,_Allocator>::const_iterator it;
	for (it=src.begin(); it!=src.end(); ++it)
	{
		target[it->first] = it->second;
	}
}
///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


