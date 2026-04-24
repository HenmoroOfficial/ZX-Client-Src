/*
 * FILE: NPCGenMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/8
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#include "npcgendata.h"
#include <stdio.h>
#include <memory.h>
#include <string.h>

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


///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Implement CNPCGenMan
//	
///////////////////////////////////////////////////////////////////////////

CNPCGenMan::CNPCGenMan() :
m_aAAreaTab(64),
m_aResAAreaTab(64)
{
}

CNPCGenMan::~CNPCGenMan()
{
}

//	Load data from file
bool CNPCGenMan::Load(const char* szFileName)
{
	FILE* fp = fopen(szFileName, "rb");
	if (!fp)
		return false;
	
	size_t version;
	fread(&version, 1, sizeof (size_t), fp);
	if (version > AIGENFILE_VERSION)
		return false;

	NPCGENFILEHEADER7 Header;
	memset(&Header, 0, sizeof (Header));

	if (version < 6)
	{
		NPCGENFILEHEADER temp;
		fread(&temp, 1, sizeof (temp), fp);

		memcpy(&Header, &temp, sizeof (temp));
	}
	else if (version < 7)	//	version >= 6 && version < 7
	{
		NPCGENFILEHEADER6 temp;
		fread(&temp, 1, sizeof (temp), fp);
		memcpy(&Header, &temp, sizeof (temp));
	}
	else	//	version >= 7
	{
		fread(&Header, 1, sizeof (Header), fp);
	}

	int i, j;

	//	Read NPC generate areas
	for (i=0; i < Header.iNumAIGen; i++)
	{
		NPCGENFILEAREA14 SrcArea;
		SrcArea.iPhase = 0;

		if (version < 7)
		{
			NPCGENFILEAREA temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&SrcArea, &temp, sizeof (temp));

			SrcArea.idCtrl		= 0;
			SrcArea.iLifeTime	= 0;
			SrcArea.iMaxNum		= 0;
			SrcArea.dwExportID	= 0;
			SrcArea.iAttachNum	= 0;
		}
		else if (version < 12)
		{
			NPCGENFILEAREA7 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&SrcArea, &temp, sizeof (temp));

			SrcArea.dwExportID	= 0;
			SrcArea.iAttachNum	= 0;
		}
		else if (version < 14)
		{
			NPCGENFILEAREA12 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&SrcArea, &temp, sizeof (temp));
		}
		else	//	version >= 14
			fread(&SrcArea, 1, sizeof (SrcArea), fp);
		
		AREA Area;
		Area.iType			= SrcArea.iType;
		Area.iNumGen		= SrcArea.iNumGen;
		Area.vPos[0]		= SrcArea.vPos[0];
		Area.vPos[1]		= SrcArea.vPos[1];
		Area.vPos[2]		= SrcArea.vPos[2];
		Area.vDir[0]		= SrcArea.vDir[0];
		Area.vDir[1]		= SrcArea.vDir[1];
		Area.vDir[2]		= SrcArea.vDir[2];
		Area.vExts[0]		= SrcArea.vExts[0];
		Area.vExts[1]		= SrcArea.vExts[1];
		Area.vExts[2]		= SrcArea.vExts[2];
		Area.iNPCType		= SrcArea.iNPCType;
		Area.iGroupType		= SrcArea.iGroupType;
		Area.cRevive		= SrcArea.cReviveType;
		Area.idCtrl			= SrcArea.idCtrl;
		Area.iLifeTime		= SrcArea.iLifeTime;
		Area.iMaxNum		= SrcArea.iMaxNum;
		Area.iPhase			= SrcArea.iPhase;
		Area.iFirstGen		= m_aNPCGens.size();
		
		for (j=0; j < SrcArea.iNumGen; j++)
		{
			NPCGENFILEAIGEN SrcGen;
			fread(&SrcGen, 1, sizeof (SrcGen), fp);

			NPCGEN Gen;
			Gen.iArea			= i;
			Gen.dwID			= SrcGen.dwID;
			Gen.dwNum			= SrcGen.dwNum;
			Gen.dwDiedTimes		= SrcGen.dwDiedTimes;
			Gen.dwAggressive	= SrcGen.dwAggressive;
			Gen.dwRefresh		= SrcGen.dwRefresh;
			Gen.fOffsetTrn		= SrcGen.fOffsetTrn;
			Gen.fOffsetWater	= SrcGen.fOffsetWater;
			Gen.dwFaction		= SrcGen.dwFaction;
			Gen.dwFacHelper		= SrcGen.dwFacHelper;
			Gen.dwFacAccept		= SrcGen.dwFacAccept;
			Gen.bNeedHelp		= SrcGen.bNeedHelp;
			Gen.bDefFaction		= SrcGen.bDefFaction;
			Gen.bDefFacHelper	= SrcGen.bDefFacHelper;
			Gen.bDefFacAccept	= SrcGen.bDefFacAccept;
			Gen.iPathID			= SrcGen.iPathID;
			Gen.iLoopType		= SrcGen.iLoopType;
			Gen.iSpeedFlag		= SrcGen.iSpeedFlag;
			Gen.iDeadTime		= SrcGen.iDeadTime;

			m_aNPCGens.push_back(Gen);
		}

		//	Attaching area
		if (SrcArea.iAttachNum < 0)
		{
			m_aAAreaTab.put(SrcArea.dwExportID, Area);
		}
		else
		{
			int id;
			for (j=0; j < SrcArea.iAttachNum; j++)
			{
				fread(&id, 1, sizeof (id), fp);
				Area.aAttachAreas.push_back(id);
			}

			m_aAreas.push_back(Area);
		}
	}

	//	Read resources areas
	for (i=0; i < Header.iNumResArea; i++)
	{
		NPCGENFILERESAREA14 src;

		if (version < 6)
		{
			NPCGENFILERESAREA temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.dir[0]	= 0;
			src.dir[1]	= 0;
			src.rad		= 0;

			src.idCtrl	= 0;
			src.iMaxNum	= 0;

			src.dwExportID	= 0;
			src.iAttachNum	= 0;
			src.iPhase = 0;
		}
		else if (version < 7)	//	version >= 6 && version < 7
		{
			NPCGENFILERESAREA6 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.idCtrl	= 0;
			src.iMaxNum	= 0;

			src.dwExportID	= 0;
			src.iAttachNum	= 0;
			src.iPhase = 0;
		}
		else if (version < 12)
		{
			NPCGENFILERESAREA7 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.dwExportID	= 0;
			src.iAttachNum	= 0;
			src.iPhase = 0;
		}
		else if (version < 14)
		{
			NPCGENFILERESAREA12 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.iPhase = 0;
		}
		else
			fread(&src, 1, sizeof (src), fp);

		RESAREA dst;
		dst.vPos[0]		= src.vPos[0];
		dst.vPos[1]		= src.vPos[1];
		dst.vPos[2]		= src.vPos[2];
		dst.fExtX		= src.fExtX;
		dst.fExtZ		= src.fExtZ;
		dst.iResNum		= src.iNumRes;
		dst.bAutoRevive	= src.bAutoRevive;
		dst.idCtrl		= src.idCtrl;
		dst.iMaxNum		= src.iMaxNum;
		dst.dir[0]		= src.dir[0];
		dst.dir[1]		= src.dir[1];
		dst.rad			= src.rad;
		dst.iPhase		= src.iPhase;
		dst.iFirstRes	= m_aRes.size();

		for (j=0; j < src.iNumRes; j++)
		{
			NPCGENFILERES src_res;
			fread(&src_res, 1, sizeof (src_res), fp);

			RES dst_res;
			dst_res.iResType		= src_res.iResType;
			dst_res.idTemplate		= src_res.idTemplate;
			dst_res.dwRefreshTime	= src_res.dwRefreshTime;
			dst_res.dwNumber		= src_res.dwNumber;
			dst_res.fHeiOff			= src_res.fHeiOff;

			m_aRes.push_back(dst_res);
		}

		//	Attaching area
		if (src.iAttachNum < 0)
		{
			m_aResAAreaTab.put(src.dwExportID, dst);
		}
		else
		{
			int id;
			for (j=0; j < src.iAttachNum; j++)
			{
				fread(&id, 1, sizeof (id), fp);
				dst.aAttachAreas.push_back(id);
			}

			m_aResAreas.push_back(dst);
		}
	}

	//	Read dynamic objects
	for (i=0; i < Header.iNumDynObj; i++)
	{
		NPCGENFILEDYNOBJ14 src;
		src.iPhase = 0;

		if (version < 9)
		{
			NPCGENFILEDYNOBJ temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.scale = 16;		//	16 means scale == 1.0f
			src.idController = 0;
		}
		else if (version < 10)
		{
			NPCGENFILEDYNOBJ9 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.idController = 0;
		}
		else if (version < 14)
		{
			NPCGENFILEDYNOBJ10 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));
		}
		else
			fread(&src, 1, sizeof (src), fp);
		
		DYNOBJ dst;
		dst.dwDynObjID	= src.dwDynObjID;
		dst.vPos[0]		= src.vPos[0];
		dst.vPos[1]		= src.vPos[1];
		dst.vPos[2]		= src.vPos[2];
		dst.dir[0]		= src.dir[0];
		dst.dir[1]		= src.dir[1];
		dst.rad			= src.rad;
		dst.scale		= src.scale;
		dst.idCtrl		= src.idController;
		dst.iPhase		= src.iPhase;

		m_aDynObjs.push_back(dst);
	}

	//	Read NPC controller data table
	for (i=0; i < Header.iNumNPCCtrl; i++)
	{
		NPCGENFILECTRL13 src;

		if (version < 8)
		{
			NPCGENFILECTRL temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.iActiveTimeRange = 0;
			src.bRepeatActived = false;
			src.nTimeSegNum = 0;
		}
		else if (version < 11)	//	8 <= version < 11
		{
			NPCGENFILECTRL8 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.bRepeatActived = false;
			src.nTimeSegNum = 0;
		}
		else if (version < 13)	//	11 <= version < 13
		{
			NPCGENFILECTRL11 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			src.nTimeSegNum = 0;
		}
		else
		{
			NPCGENFILECTRL11 temp;
			fread(&temp, 1, sizeof (temp), fp);

			memcpy(&src, &temp, sizeof (temp));

			fread(&src.nTimeSegNum, 1, sizeof(int), fp);

			for(int j=0;j<src.GetTimeSegNum();j++)
			{
				NPCCTRLTIME _time;
				fread(&_time, 1, sizeof(NPCCTRLTIME), fp);
				src.aTimeSeg.push_back(_time);

				fread(&_time, 1, sizeof(NPCCTRLTIME), fp);
				src.aTimeSeg.push_back(_time);
			}
		}

		CONTROLLER dst;
		dst.id					= src.id;
		dst.iControllerID		= src.iControllerID;
		dst.bActived			= src.bActived;
		dst.iWaitTime			= src.iWaitTime;
		dst.iStopTime			= src.iStopTime;
		dst.bActiveTimeInvalid	= src.bActiveTimeInvalid;
		dst.bStopTimeInvalid	= src.bStopTimeInvalid;
		dst.ActiveTime			= src.ActiveTime;
		dst.StopTime			= src.StopTime;
		dst.iActiveTimeRange	= src.iActiveTimeRange;
		dst.bRepeatActived		= src.bRepeatActived;

		dst.nTimeSegNum			= src.nTimeSegNum;
		for(int j=0;j<dst.GetTimeSegNum();j++)
		{
			dst.aTimeSeg.push_back(src.aTimeSeg[2*j]);
			dst.aTimeSeg.push_back(src.aTimeSeg[2*j+1]);
		}
		
		strncpy(dst.szName, src.szName, 128);

		m_aControllers.push_back(dst);
	}

	fclose(fp);
	return true;
}

//	Get generator attaching area by id
const CNPCGenMan::AREA* CNPCGenMan::GetGenAttachArea(int id)
{
	AreaTable::pair_type Pair = m_aAAreaTab.get(id);
	if (!Pair.second)
		return NULL;
	else
		return Pair.first;
}

//	Get resources attaching area by id
const CNPCGenMan::RESAREA* CNPCGenMan::GetResAttachArea(int id)
{
	ResAreaTable::pair_type Pair = m_aResAAreaTab.get(id);
	if (!Pair.second)
		return NULL;
	else
		return Pair.first;
}


