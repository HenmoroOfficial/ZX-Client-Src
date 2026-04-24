/*
 * FILE: EC_HostInputFilter.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2004/8/31
 *
 * HISTORY: 
 *
 * Copyright (c) 2004 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "APoint.h"
#include "EC_InputFilter.h"

///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

//	Logic keys
/*enum
{
	LKEY_CANCEL = 1,		//	Cancel action etc.
	LKEY_ITFTRIGGER,		//	Interface open/close trigger
	LKEY_GHOSTMODE,			//	Trigger camera to ghost mode
	LKEY_JUMP,				//	Test code, Jump
	LKEY_PLAYTRICK,			//	Test code, Play trick action
	LKEY_PUSHCAMERA,		//	Push camera
	LKEY_PULLCAMERA,		//	Pull camera
	LKEY_AUTOSELECT,		//	Auto select monster
	LKEY_BOSSKEY,			
	LKEY_MOVIEMODE,			//	Trigger camera to movie mode
	NUM_LKEY,
};
*/

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////

class CECHostPlayer;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECHostInputFilter
//	
///////////////////////////////////////////////////////////////////////////

class CECHostInputFilter : public CECInputFilter
{
public:		//	Types

	//	Mouse press information
	struct PRESS
	{
		bool	bPressed;		//	Pressed flag
		DWORD	dwPressTime;	//	Start pressing time stamp
		APointI	ptPress;		//	Press position
		bool	bTurnCamera;	//	Turn camera flag
	};

	//	Key map group
	enum KEYMAP
	{
		KM_NORMAL = 0,
		KM_CTRL,
		KM_SHIFT,
		NUM_KEYMAP,
	};

public:		//	Constructor and Destructor

	CECHostInputFilter(CECInputCtrl* pInputCtrl);
	virtual ~CECHostInputFilter();

public:		//	Attributes
	static char*	m_aLKNames[NUM_LKEY];
public:		//	Operations

	//	Translate input
	virtual bool TranslateInput(int iInput);

	const PRESS& GetLBtnPressinfo() { return m_LBPress; }
	const PRESS& GetRBtnPressInfo() { return m_RBPress; }
	void RegisterBossKeys(unsigned int key, unsigned int dwCtrlKey );

	//	Reset logic key map to default settings
	void ResetLogicMap();
	//	Load logic key map from file
	bool LoadLogicKeyMap(const char* szFile);
	//	Save logic key map to file
	bool SaveLogicKeyMap(const char* szFile);


protected:	//	Attributes

	APointI		m_TurnOffset;		//	Camera turning offset
	APointI		m_ptTurnStart;		//	Start position to turn camera

	PRESS		m_LBPress;			//	Left button press information
	PRESS		m_RBPress;			//	Right button press information

	BYTE		m_aKeyMaps[NUM_KEYMAP][256];	//	Key map table
	BYTE		m_aLKeyStates[NUM_LKEY];		//	Logic key states

protected:	//	Operations

	//	Translate keyboard input
	bool TranslateKeyboard(CECHostPlayer* pHost);
	//	Translate mouse input
	bool TranslateMouse(CECHostPlayer* pHost);
	//	Translate moving control keys
	bool TranslateMoveCtrlKeys(CECHostPlayer* pHost);
	//	Key message handlers
	void OnKeyDown(CECHostPlayer* pHost, int iLogicKey, DWORD dwFlags);
	void OnKeyUp(CECHostPlayer* pHost, int iLogicKey, DWORD dwFlags);
	//	Mouse message handlers
	void OnLButtonDown(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnLButtonUp(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnLButtonDblClk(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnRButtonDown(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnRButtonUp(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnRButtonDblClk(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnMouseMove(CECHostPlayer* pHost, int x, int y, DWORD dwFlags);
	void OnMouseWheel(CECHostPlayer* pHost, int x, int y, int iDelta, DWORD dwFlags);
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////


