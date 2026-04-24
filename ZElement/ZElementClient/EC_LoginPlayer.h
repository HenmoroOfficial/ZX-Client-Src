/*
 * FILE: EC_LoginPlayer.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/1/28
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "A3DSkinModel.h"
#include "EC_Player.h"

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

class CECIvtrItem;

namespace GNET
{
	class RoleInfo;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECLoginPlayer
//	
///////////////////////////////////////////////////////////////////////////

class CECLoginPlayer : public CECPlayer
{
public:		//	Types

public:		//	Constructor and Destructor

	CECLoginPlayer(CECPlayerMan* pPlayerMan);
	virtual ~CECLoginPlayer();

public:		//	Attributes

public:		//	Operations

	//	Release object
	virtual void Release();
	//	When weapon has been changed, this function will be called
	virtual void OnWeaponChanged();

	//	Tick routine
	virtual bool Tick(DWORD dwDeltaTime);
	//	Render routine
	virtual bool Render(CECViewport* pViewport, int iRenderFlag=0);
	//	Search the full suite
	virtual int SearchFullSuite();
	//	Get number of equipped items of specified suite
	virtual int GetEquippedSuiteItem(int idSuite, int* aItems=NULL);

	//	Load player data
	bool Load(GNET::RoleInfo& Info);
	bool Load(int physique, int profession, int face, int hair, int gener, const int* equips, bool bHideEquip=true);
	bool ChangeFace(int iFaceid);
	bool ChangeHair(int iHairid);
	bool ChangeEar(int iEarid);
	bool ChangeTail(int iTailid);
	bool ChangeDefaultSkin(int iSkin);
	void SetRace(int race) { m_iRace = race; }

	void StandForCustomize();
	void Stand(bool restart=false);
	void Fly(bool restart=false);
	void Run();
	void StandUp();
	void SitDown();
	void ShowPose();
	void UpdateEquipments(const int* pEquipmentID, bool bFashion);
	void UpdateLieShan(int iEarid, int iTailid);
	void UpdateFace(int iFaceid, int iHairid, int iSkin);
	bool IsTrusteeMode() { return m_bTrusteeMode; }
	void SyncBasicProps(ROLEBASICPROP& roleProp);
	void UpdateEquipmentsSkin( int iDarkLightState );	//用于更新轩辕职业黑暗光明装备 Added 2012-09-05.

	virtual void RideOnPet(int id, int iLev,int type);
	
	void SetPlayerCharacterID(int cid) { m_PlayerInfo.cid = cid; }	// 某些特定的场合，需要设定一个临时cid 
protected:	//	Attributes

	CECIvtrItem*	m_aEquipItems[SIZE_EQUIPIVTR];
	bool			m_bTrusteeMode;

protected:	//	Operations

	//	Render for refelction
	bool RenderForReflect(CECViewport* pViewport);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


