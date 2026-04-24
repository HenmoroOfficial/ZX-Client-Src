/*
 * FILE: EC_FGC2SRawCmds.h
 *
 * DESCRIPTION: Fun game c2s commands
 *
 * CREATED BY: Wangrui, 2009/3/12
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

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

//	Assistant functions used to send C2S commands

//	Poker cards commands
void fgc2s_SendCmdGetRoomList(BYTE byGameType, BYTE byIndex);
void fgc2s_SendCmdPlayerEnterRoom(BYTE byGameType, WORD wRoomID, BYTE byDir);
void fgc2s_SendCmdPlayerBeginGame(BYTE byGameType, WORD wRoomID);
void fgc2s_SendCmdPlayerReEnterRoom(BYTE byGameType, WORD wRoomID);
void fgc2s_SendCmdPlayerLeaveRoom(BYTE byGameType, WORD wRoomID);
void fgc2s_SendCmdPlayerThrowNormalCard(BYTE byGameType, WORD wRoomID, BYTE byCardNum, const BYTE* aCards);
void fgc2s_SendCmdPlayerPickupNormalCard(BYTE byGameType, WORD wRoomID);
void fgc2s_SendCmdPlayerGiveupNormalCard(BYTE byGameType, WORD wRoomID);
void fgc2s_SendCmdPlayerAuction(BYTE byGameType, WORD wRoomID, int idRole, BYTE byScore);
void fgc2s_SendCmdPlayerEnterHall(BYTE byGameType);
void fgc2s_SendCmdPlayerLeaveHall(BYTE byGameType);
