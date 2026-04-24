/*
 * FILE: EC_PlayerImage.cpp
 *
 * DESCRIPTION: Manage personal image
 *
 * CREATED BY: Jiangdalong, 2008/04/11
 *
 * HISTORY:
 *
 * Copyright (c) 2001-2008 Archosaur Studio, All Rights Reserved.	
 */

#include "EC_PlayerImage.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "EC_PageDownloaderMan.h"
#include "EC_Utility.h"

#include "EC_Game.h"
#include "EC_UIManager.h"
#include "EC_GameUIMan.h"
#include "DlgSnSInfo.h"

#include <AString.h>
#include <AFI.h>

#define BASE_IMAGE_PATH		"cache\\playerimages"
#define BASE_IMAGE_URL		"http://images.wanmei.com/personal/zhuxian/ok/"
#define OUT_TIME			1800000		// 1800Ãë

CECPageDownloaderMan* CECPlayerImage::m_pDownloaderMan = NULL;

CECPlayerImage::CECPlayerImage()
{
	m_pDownloaderMan = NULL;
}

CECPlayerImage::~CECPlayerImage()
{
}

void CECPlayerImage::Init()
{
	if( !m_pDownloaderMan )
	{
		m_pDownloaderMan = new CECPageDownloaderMan;
		m_pDownloaderMan->Init(PlayerImageDownloadCallback);
	}

	SYSTEMTIME stime;
	FILETIME ftime;
	GetLocalTime(&stime);
	SystemTimeToFileTime(&stime, &ftime);
	UINT64 curTime = (UINT64(ftime.dwHighDateTime) << 32) + ftime.dwLowDateTime;

	BOOL bval;
	HANDLE hFile;
	WIN32_FIND_DATAA wfd;
	AString strFind = AString(BASE_IMAGE_PATH) + AString("\\*.jpg");

	hFile = FindFirstFileA(strFind, &wfd);
	if( INVALID_HANDLE_VALUE == hFile )
		return;

	do
	{
		UINT64 utime = (UINT64(wfd.ftLastWriteTime.dwHighDateTime) << 32) + wfd.ftLastWriteTime.dwLowDateTime;
		bval = FindNextFileA(hFile, &wfd);
		if( curTime - utime > OUT_TIME )
			DeleteFileA(AString(BASE_IMAGE_PATH) + wfd.cFileName);
	}while(bval);

	FindClose(hFile);
}

int CECPlayerImage::GetPlayerImage(int nServerID, int nPlayerID, AString& strImagePath)
{
	strImagePath = GetImagePath(nServerID, nPlayerID);

	if( IsImageTooOld(strImagePath) )
	{
		AString strUrl = GetImageUrl(nServerID, nPlayerID);
		UpdateImage(strUrl, strImagePath, nPlayerID);

		return PI_ISDOWNLOADING;
	}

	return PI_AVAILABLE;
}

bool CECPlayerImage::IsImageTooOld(const char* szImagePath)
{
	SYSTEMTIME stime;
	FILETIME ftime;
	GetLocalTime(&stime);
	SystemTimeToFileTime(&stime, &ftime);
	UINT64 curTime = (UINT64(ftime.dwHighDateTime) << 32) + ftime.dwLowDateTime;

	HANDLE hFile;
	WIN32_FIND_DATAA wfd;

	hFile = FindFirstFileA(szImagePath, &wfd);
	if( INVALID_HANDLE_VALUE == hFile )
		return true;

	UINT64 utime = (UINT64(wfd.ftLastWriteTime.dwHighDateTime) << 32) + wfd.ftLastWriteTime.dwLowDateTime;
	if( curTime - utime > OUT_TIME )
		return true;

	return false;
}

bool CECPlayerImage::UpdateImage(const char* szUrl, const char* szImagePath, int nPlayerID)
{
	if (NULL == m_pDownloaderMan)
		return false;

	AString strPath;
	af_GetFilePath(szImagePath, strPath);
	if (!glb_CreateDirectory(strPath))
		return false;
	if (!m_pDownloaderMan->AddNewTask(szUrl, szImagePath, nPlayerID))
		return false;

	return true;
}

AString CECPlayerImage::GetImageUrl(int nServerID, int nPlayerID)
{
	AString strUrl;
	strUrl.Format("http://photo.zhuxian.wanmei.com/personal/zhuxian/ok/%d/%d/%d_%d.jpg",nServerID,nPlayerID%100,nServerID,nPlayerID);
	return strUrl;
}

AString CECPlayerImage::GetImagePath(int nServerID, int nPlayerID)
{
	AString strPath;
	strPath.Format("%s%d-%d.jpg", BASE_IMAGE_PATH, nServerID, nPlayerID);
	return strPath;
}

void CECPlayerImage::PlayerImageDownloadCallback(int taskid, AString localfn, bool success)
{
	if( !success )
		DeleteFileA(localfn);
	else if(g_pGame->GetGameRun()->GetUIManager()->GetCurrentUIMan()==CECUIManager::UIMAN_INGAME)
	{
		CECGameUIMan *pGameUI = g_pGame->GetGameRun()->GetUIManager()->GetInGameUIMan();
		CDlgSnsInfo *pSnsInfo = (CDlgSnsInfo*)pGameUI->GetDialog("Win_PersInfo");
		pSnsInfo->SetPhoto(localfn.GetBuffer(0));
	}
}

void CECPlayerImage::Release()
{
	A3DRELEASE(m_pDownloaderMan)
}