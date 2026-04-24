/*******************************************************************
FILE: .h

CREATED BY: Li Bin, 2013.07.11

HISTORY: 

DESCRIPTION: 

Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#pragma once

#include "DlgBase.h"
#include <list>
///////////////////////////////////////////////////////////////////////////
//	
//	Define and Macro
//	
///////////////////////////////////////////////////////////////////////////

#ifdef UNICODE
#define a_atof		_wtof
#else
#define a_atof		atof
#endif	//	UNICODE

///////////////////////////////////////////////////////////////////////////
//	
//	Types and Global variables
//	
///////////////////////////////////////////////////////////////////////////
class CECGameAcupoint;
class AUIProgress;

///////////////////////////////////////////////////////////////////////////
//	
//	Declare of Global functions
//	
///////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////
//	
//	Class
//	
//	NAME:  CDlgGameRush
//
//	RESOUSE: 
//
//	DESCRIPTION: 
//
///////////////////////////////////////////////////////////////////////////
class CDlgGameAcupoint : public CDlgBase{
	AUI_DECLARE_COMMAND_MAP( )
	AUI_DECLARE_EVENT_MAP( )

public:
	enum
	{
		GAME_ID_BEGIN = 2001,
		GAME_ID_END = 3000,
	};
public:
	CDlgGameAcupoint( );
	~CDlgGameAcupoint( );

	virtual void OnShowDialog( ) {};
	virtual void OnHideDialog();
	virtual bool Tick();

	virtual bool OnInitDialog( );

	void OnCommandPause( const char * szCommand );
	void OnCommandContinue( const char * szCommand );

	void OnEventKeyDown( WPARAM wParam, LPARAM lParam, AUIObject * pObj );

	bool StartStage( int gameid );
	void ShowStageUI( );
	void ShowWinUI( );
	void ShowLoseUI( );
	void ShowWaitUI( );
	void ClearStageUI( );
	PAUISTILLIMAGEBUTTON AddStillImageBtn( AXMLItem *xml );
	PAUIIMAGEPICTURE AddImagePic( AXMLItem *xml );
	PAUILABEL AddLabel( AXMLItem *xml );

private:
	AXMLFile						m_xGameDataFile;

	PAUISTILLIMAGEBUTTON			m_pPauseBtn;
	PAUISTILLIMAGEBUTTON			m_pContinueBtn;
	PAUILABEL						m_pLivesLabel;
	PAUIIMAGEPICTURE				m_pCountDown;

	CECGameAcupoint*				m_pGameAcupoint;
	PAUIIMAGEPICTURE				m_pRusherBtn;
	AArray<PAUISTILLIMAGEBUTTON>	m_pCheckPointBtns;
	AArray<PAUISTILLIMAGEBUTTON>	m_pAUIImageBtns;
	AArray<PAUIIMAGEPICTURE>		m_pAUIImages;
	AArray<PAUILABEL>				m_pAUILabels;

	int								m_nGameID;
	bool							m_bIsShowEndingUI;
	bool							m_bIsShowWaitUI;
};

///////////////////////////////////////////////////////////////////////////
//	
//	Inline functions
//	
///////////////////////////////////////////////////////////////////////////