/**
 * @file      cc_sdk.h
   @brief     The header file for CORE Client SDK APIs.
   @author    shenhui@pwrd.com
   @data      2011/06/15
   @version   01.00.00
 */

/* 
 *  Copyright(C) 2010. Perfect World Entertainment Inc
 *
 *  This PROPRIETARY SOURCE CODE is the valuable property of PWE and 
 *  its licensors and is protected by copyright and other intellectual 
 *  property laws and treaties.  We and our licensors own all rights, 
 *  title and interest in and to the SOURCE CODE, including all copyright 
 *  and other intellectual property rights in the SOURCE CODE.
 */

/*
 * $LastChangedBy: shenhui@pwrd.com
 * $LastChangedDate: 
 * $Revision: 01.00.01
 * $Id:
 * $notes:.
 */

#pragma once

#include <windows.h>
#include <tchar.h>
#include "CC_Datatype.h"
#include "CC_Error.h"

namespace CC_SDK
{


#ifdef _cplusplus
extern "C"
{
#endif

#ifdef CC_SDK_EXPORTS
#define CC_SDK_API extern "C" __declspec(dllexport)
#else
#define CC_SDK_API extern "C" __declspec(dllimport)
#endif


/****************************************************************************/
/*                       Interface declaration                              */
/****************************************************************************/

CC_SDK_API HRESULT CC_LaunchCOREClient(int nStartFlag, TCHAR* pszUserAccount, TCHAR* pszGameAbbr, int nLanguage) ;

/**
* @brief: initialize CORE Client SDK.
* @param[in] nPartnerId: useless now, reserve for future.
* @param[in] nAppId: useless now, reserve for future.
* @param[in] nVersion: D3D version of calling process, must be one value of D3D_VERSION.
*
* @return: If succeed, return a handle of SDK, otherwise return NULL 
* @notes: none.
*/
CC_SDK_API CC_HANDLE CC_Init(unsigned int nPartnerId, unsigned int nAppId, D3D_VERSION nVersion) ;

/**
* @brief: exit CORE Client SDK.
* @param[in] hSDK: handle returned by CC_Init
*
* @return: If succeed, return S_OK, otherwise return error code
* @notes: if return FALSE, error code can be found in CC_Error.h.
*/
CC_SDK_API HRESULT CC_UnInit(const CC_HANDLE hSDK) ;

/**
* @brief: get a token when user are trying to log into game.
* @param[in] hSDK: handle returned by CC_Init.
* @param[in] nPartnerId: this is reserved for future use, can be set 0 now.
* @param[in] nAppId: this is reserved for future use, can be set 0 now.
* @param[in] pszAccount: this is account name used to log into game, this parameter is null-terminated.
* @param[out]pszToken: this is the token return back to caller.
*            Token is a 16Bytes null-terminated string at present, so caller need allocate at least 17Bytes buffer.
* @param[in] nTokenLen: length of pszToken in byte, should larger than 16Bytes at present.
* @param[in] dwMilliseconds:  timeout in milliseconds.
* @return: If succeed, return S_OK, otherwise return error code
* @notes: This API should be called in the real game client executable, not the launcher or patcher.
*/
CC_SDK_API HRESULT CC_GetToken(const CC_HANDLE hSDK, unsigned int nPartnerId, unsigned int nAppId, TCHAR* pszAccount, TCHAR* pszToken, unsigned int nTokenLen,DWORD dwTimeout) ;

/**
* @brief: create a browser tab in game browser.
* @param[in] hSDK: handle returned by CC_Init
* @param[in] pszUrl: url to browser in game browser.
*
* @return: If succeed, return S_OK, otherwise return error code
* @notes: if return FALSE, error code can be found in CC_Error.h.
*/
CC_SDK_API HRESULT CC_GotoUrlInOverlay(const CC_HANDLE hSDK, TCHAR* pszUrl) ;

#ifdef _cplusplus
}
#endif

}//end of namespace