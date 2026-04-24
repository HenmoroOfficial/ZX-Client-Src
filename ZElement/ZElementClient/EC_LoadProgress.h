/*
 * FILE: EC_LoadProgress.h
 *
 * DESCRIPTION: 
 *
 * CREATED BY: Duyuxin, 2005/5/4
 *
 * HISTORY: 
 *
 * Copyright (c) 2005 Archosaur Studio, All Rights Reserved.
 */

#pragma once

#include "AArray.h"
#include "ARect.h"
#include "AAssist.h"

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

class A2DSprite;
class A3DFTFont;
class AUILuaManager;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class CECLoadProgress
//	
///////////////////////////////////////////////////////////////////////////

class CECLoadProgress
{
public:		//	Types

	//	Load ID
	enum
	{
		LOAD_JUMPINSTANCE = 0,
		LOAD_LOGIN,
		LOAD_ENTERGAME,
	};

public:		//	Constructor and Destructor

	CECLoadProgress();
	virtual ~CECLoadProgress();

public:		//	Attributes

public:		//	Operations

	//	Initialize object
	bool Init(int iLoadID, const char* szBackPic);
	//	Release object
	void Release();
	//	Render routine
	bool Render();

	//	Gets the current position. 
	int GetProgressPos() { return m_iPos; }
	//	Gets the lower and upper limits of the range 
	int GetProgressRange(int* piMin, int* piMax) { *piMin=m_iMin; *piMax=m_iMax; }
	//	Advances the current position
	int OffsetProgressPos(int iOffset);
	//	Sets the current position 
	void SetProgressPos(int iPos);
	//	Sets the minimum and maximum ranges
	void SetProgressRange(int iMin, int iMax); 
	//	Specifies the step increment
	void SetProgressStep(int iStep) { m_iStep = iStep; }
	//	Advances the current position by the step increment
	int StepProgress();

protected:	//	Attributes

	int		m_iPos;		//	Current position
	int		m_iMin;		//	Minimum position
	int		m_iMax;		//	Maximum position
	int		m_iStep;	//	Step size
	ACString m_strHint;
	int		m_xHint;
	int		m_yHint;

	ARectI	m_rcBar;	//	Rectangle of progress bar

	A2DSprite*	m_pBackPic;		//	Background picture
	A2DSprite*	m_pBarPic;		//	Progress bar picture
	A2DSprite*	m_pLogoPic;		//	Logo picture
	A2DSprite*	m_pBarTopPic;	//	Progress bar top picture

	A3DFTFont *m_pFont;
	AUILuaManager*	m_pLoadingUIMan;

protected:	//	Operations

};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////



