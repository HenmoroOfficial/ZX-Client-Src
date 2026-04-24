/*******************************************************************
FILE: .cpp

CREATED BY: Li Bin, 2013.07.18

HISTORY: 

DESCRIPTION: 

Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include "AIniFile.h"
#include "afi.h"
#include "A3DVector.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "DlgGameAcupointResult.h"
#include "EC_HostPlayer.h"

#define new A_DEBUG_NEW

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
// 

///////////////////////////////////////////////////////////////////////////
//	
//	Local functions
//	
///////////////////////////////////////////////////////////////////////////

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP( CDlgGameAcupointResult, CDlgBase )

AUI_ON_COMMAND("Btn_Confirm",				OnCommandConfirm)

AUI_END_COMMAND_MAP( )

CDlgGameAcupointResult::CDlgGameAcupointResult( )
{
}

CDlgGameAcupointResult::~CDlgGameAcupointResult( )
{
}

bool CDlgGameAcupointResult::Tick( )
{
	bool ret = CDlgBase::Tick( );

	PAUIDIALOG mainDlg = GetGameUIMan( )->GetDialog( "Win_Game_Acupoint" );
	AlignTo( mainDlg, 
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER,
		AUIDIALOG_ALIGN_INSIDE, AUIDIALOG_ALIGN_CENTER );

	GetGameUIMan( )->InsertDialogAfter( mainDlg );

	return ret;
}

void CDlgGameAcupointResult::OnCommandConfirm( const char * szCommand )
{
	if ( stricmp( GetName( ), "Win_Game_Acupoint_WinGfx" ) == 0 )
	{
		Show( false );
		GetHostPlayer( )->FinishTinyGameTask( GetData( ) );
	}
	else if ( stricmp( GetName( ), "Win_Game_Acupoint_LoseGfx" ) == 0 )
	{
		Show( false );
	}
}