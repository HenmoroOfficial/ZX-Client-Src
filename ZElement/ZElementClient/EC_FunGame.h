/*
 * FILE: EC_FunGame.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Wangrui, 2009/03/17
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "EC_Global.h"
#include <AArray.h>
#include "vector.h"
#include <AWString.h>

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#define PLAZA_ROOM_MAX 200
#define PLAZA_ROOM_PERPAGE	20
#define ROOM_PLAYER_MAX 8

namespace FG_S2C
{
	namespace INFO
	{
		struct room_info;
		struct player_base_info;
	};
};

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

//	Fun game player info
struct FG_PLAYERINFO
{
	int			idPlayer;
	int			iState;
	char		cGender;
	INT64		iScore;
	int			iWinCount;
	int			iLoseCount;
	int			iDrawCount;
	int			iEscapeCount;
	AWString	strName;
};

//	Fun game room
struct FG_ROOMINFO
{
	bool		bInit;
	int			iGameType;
	int			iPlayerNum;
	FG_PLAYERINFO	players[ROOM_PLAYER_MAX];
};

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Class CECFunGame
//	
///////////////////////////////////////////////////////////////////////////

class CECFunGamePlaza
{

public:		//	Types

public:		//	Constructor and Destructor

	CECFunGamePlaza();
	~CECFunGamePlaza();

public:		//	Attributes

public:		//	Operations

	//	Get room info by id
	FG_ROOMINFO* GetFGRoomInfo(int idRoom);
	//	Set room info
	void SetFGRoomInfo(int iGameType, const FG_S2C::INFO::room_info* pInfo);
	//	Get player info
	FG_PLAYERINFO* GetFGPlayerInfo(int idRoom, int iDir);
	//	Set player info
	void SetFGPlayerInfo(int idRoom, const FG_S2C::INFO::player_base_info* pInfo);
	//	Clear the plaza
	void ClearFGPlaza();

	//	Clear room info
	void ClearFGRoomInfo(int idRoom);

	//	Clear player info
	void ClearFGPlayerInfo(int idRoom, int iDir);

	//	Find player in room 
	int	FindPlayerInRoom(int idRoom, int idPlayer);
	void PlayerEnterHall(FG_S2C::INFO::player_base_info* pData);
	void PlayerEnterHall(FG_PLAYERINFO* pData);
	void PlayerLeaveHall(int playerId);
	void NotifyHallMember(void *pData);

	abase::vector<FG_PLAYERINFO>& GetHallMembers() { return m_vecHallPlayers;}

protected:	//	Attributes

	FG_ROOMINFO	m_aRooms[PLAZA_ROOM_MAX];		//	Fun game rooms
	abase::vector<FG_PLAYERINFO>	m_vecHallPlayers;

protected:	//	Operations

};