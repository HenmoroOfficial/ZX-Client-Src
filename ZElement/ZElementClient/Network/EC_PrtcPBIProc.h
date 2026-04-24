/*
 * FILE: EC_PrtcPBIProc.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2007/3/9
 *
 * HISTORY: 
 *
 * Copyright (c) 2007 Archosaur Studio, All Rights Reserved.
 */

#ifndef _EC_PRTCPBIPROC_H_
#define _EC_PRTCPBIPROC_H_

#include <hashmap.h>
#include <AArray.h>
#include <AList2.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

class CECNetProtocolBase;

namespace GNET
{
	class GetPlayerBriefInfo_Re;
	class Protocol;
	class PlayerBriefInfo;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECPrtcPBIProc
//	
///////////////////////////////////////////////////////////////////////////

class CECPrtcPBIProc
{
public:		//	Types

	typedef abase::hash_map<int, GNET::PlayerBriefInfo*> BriefInfoMap;
	typedef abase::hash_map<int, bool> ValidPrtcMap;

public:		//	Constructor and Destructor

	CECPrtcPBIProc();
	virtual ~CECPrtcPBIProc();

public:		//	Attributes

public:		//	Operations

	//	Reset object
	void Reset();

	//	Add player brief info protocol
	void AddPlayerBriefInfoPrtc(GNET::GetPlayerBriefInfo_Re* pProtocol);
	//	Add unprocessed protocol
	bool AddUnprocessedPrtc(CECNetProtocolBase* pProtocol);

	//	Process routine
	void Process();

protected:	//	Attributes

	BriefInfoMap	m_BriefInfoMap;
	ValidPrtcMap	m_ValidPrtcMap;
	
	APtrArray<GNET::GetPlayerBriefInfo_Re*>	m_aBriefInfoPrtcs;	//	Player brief info protocols
	APtrList<CECNetProtocolBase*>			m_UnprocPrtcsList;	//	Un-processed protocols

protected:	//	Operations

	//	Process one player brief info protocol
	void ProcessPBIPrtc(GNET::GetPlayerBriefInfo_Re* pProtocol);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////

#endif	//	_EC_PRTCPBIPROC_H_
