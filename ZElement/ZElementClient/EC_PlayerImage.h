/*
 * FILE: EC_PlayerImage.h
 *
 * DESCRIPTION: Manage personal image
 *
 * CREATED BY: Jiangdalong, 2008/04/11
 *
 * HISTORY:
 *
 * Copyright (c) 2001-2008 Archosaur Studio, All Rights Reserved.	
 */

#pragma once

#include <AString.h>
#include <ABaseDef.h>
#include <Windows.h>
#include "hashmap.h"


class CECPageDownloaderMan;

class CECPlayerImage  
{
public:
	enum
	{
		PI_ISDOWNLOADING	= 0,
		PI_AVAILABLE		= 1,
	};

public:
	CECPlayerImage();
	virtual ~CECPlayerImage();

	void Init();
	void Release();

	int GetPlayerImage(int nServerID, int nPlayerID, AString& strImagePath); 

	static void PlayerImageDownloadCallback(int taskid, AString localfn, bool success);

protected:
	bool IsImageTooOld(const char* szImagePath);
	bool UpdateImage(const char* szUrl, const char* szImagePath, int nPlayerID);
	AString GetImageUrl(int nServerID, int nPlayerID);
	AString GetImagePath(int nServerID, int nPlayerID);
	
protected:
	static CECPageDownloaderMan *			m_pDownloaderMan;
};

