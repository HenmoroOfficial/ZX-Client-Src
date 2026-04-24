/*
 * FILE: EC_HTC2SRawCmds.h
 *
 * DESCRIPTION: home town c2s commands
 *
 * CREATED BY: lihengju 2009/8/19
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
void htc2s_SendCmdGetCompoList(int roleid);
void htc2s_SendCmdAddCompo(int id);
void htc2s_SendCmdExgMoney(BYTE type, __int64 money);
void htc2s_SendCmdLevelUp();
void htc2s_SendCmdBuyItem(int itemid, int count);
void htc2s_SendCmdUseItem(int pos, int itemid, int target_type, int roleid, int fieldid);
void htc2s_SendCmdSellItem(int pos, int itemid, int count);
void htc2s_SendCmdGetPackage();
void htc2s_SendCmdViewFarm(int roleid);
void htc2s_SendCmdViewFarmField(int roleid, int fieldid);
void htc2s_SendCmdPlowField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdWaterField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdSowField(int roleid, int fieldid, int seedid);
void htc2s_SendCmdPestField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdWeedField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdHarvestField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdStealField(int roleid, int fieldid, int emotion);
void htc2s_SendCmdListLoveFields();
void htc2s_SendCmdListMatureFields();
