/*
 * FILE: MODELVIEWDataMan.cpp
 *
 * DESCRIPTION: 
 *
 * CREATED BY: MaYuanzheng 2010.9.2
 *
 * HISTORY:
 *
 * Copyright (c) 2010 ZhuXian Studio, All Rights Reserved.
 */

#include "MODELVIEWDataMan.h"



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
MODELVIEWDataMan::MODELVIEWDataMan(){
	elementdataman();
}

MODELVIEWDataMan::~MODELVIEWDataMan(){

}



int MODELVIEWDataMan::GetEquipEssenceNum()
{
	return equipment_essence_array.size();
}

int MODELVIEWDataMan::GetVehicleEssenceNum()
{
	return vehicle_essence_array.size();
}

int MODELVIEWDataMan::GetNPCEssenceNum()
{
	return npc_essence_array.size();
}

int MODELVIEWDataMan::GetMonsterEssenceNum()
{
	return monster_essence_array.size();
}

EQUIPMENT_ESSENCE* MODELVIEWDataMan::GetEquipEssencePtrByIndex(int index)
{
	return &equipment_essence_array[index];
}

NPC_ESSENCE* MODELVIEWDataMan::GetNPCEssencePtrByIndex(int index)
{
	return &npc_essence_array[index];
}

VEHICLE_ESSENCE* MODELVIEWDataMan::GetVehicleEssencePtrByIndex(int index)
{
	return &vehicle_essence_array[index];
}

MONSTER_ESSENCE* MODELVIEWDataMan::GetMonsterEssencePtrByIndex(int index)
{
	return &monster_essence_array[index];
}