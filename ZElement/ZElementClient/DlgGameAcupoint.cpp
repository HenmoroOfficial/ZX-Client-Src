/*******************************************************************
FILE: .cpp

CREATED BY: Li Bin, 2013.07.11

HISTORY: 

DESCRIPTION: 

Copyright (c) 2011 ZhuXian Studio, All Rights Reserved.
*******************************************************************/
#include "AIniFile.h"
#include "afi.h"
#include "A3DVector.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIProgress.h"
#include "AUI\\AXMLFile.h"
#include "EC_UIGameAcupoint.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameUIMan.h"
#include "EC_TaskInterface.h"
#include "TaskTemplMan.h"
#include "DlgGameAcupoint.h"
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

AXMLItem* FindCheckPointItemWithName( AXMLItem * pItem, ACString name )
{
		while ( pItem )
		{
			if(a_stricmp(pItem->GetName(), _AL("STILLIMAGEBUTTON")) == 0)
			{
				ACString controlName;
				XMLGET_WSTRING_VALUE(pItem, _AL("Name"), controlName);

				if (a_stricmp(controlName, name) == 0)
					return pItem;
			}

			pItem = pItem->GetNextItem();
		}

		return false;
}

///////////////////////////////////////////////////////////////////////////
//	
//	Implement
//	
///////////////////////////////////////////////////////////////////////////
AUI_BEGIN_COMMAND_MAP( CDlgGameAcupoint,CDlgBase )

AUI_ON_COMMAND("Btn_Help",				OnCommand_help)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Pause",				OnCommandPause)
AUI_ON_COMMAND("Btn_Continue",			OnCommandContinue)

AUI_END_COMMAND_MAP( )

AUI_BEGIN_EVENT_MAP( CDlgGameAcupoint,CDlgBase )

AUI_ON_EVENT( NULL,		WM_KEYDOWN,		OnEventKeyDown )

AUI_END_EVENT_MAP( )

CDlgGameAcupoint::CDlgGameAcupoint( )
{
	m_pGameAcupoint = new CECGameAcupoint( );

	// Load game data from the config file.
	const AString configFileName( "Configs\\GameAcupointConfig.xml" );

	if ( m_xGameDataFile.LoadFromFile(configFileName) == false )
	{
		a_LogOutput( 1, "CECGameRush::CECGameRush, Faild to load config file %s", configFileName );
		return;
	}

	AXMLItem *pItem = m_xGameDataFile.GetRootItem( )->GetFirstChildItem( );
	assert( pItem );
	if ( pItem == NULL || a_stricmp( pItem->GetName( ), _AL("ACUPOINTGAME") ) != 0 )
	{
		a_LogOutput( 1, "CECGameRush::CECGameRush, there are some errors in config file %s", configFileName );
		return;
	}

	XMLGET_STRING_VALUE( pItem, _AL("Name"), m_szName );
	pItem = pItem->GetFirstChildItem( );

	// Add stages to CECGameRush.
	while ( pItem )
	{
		if( a_stricmp( pItem->GetName( ), _AL("STAGE") ) == 0 )
		{
			CECGameAcupoint::Stage stage;

			XMLGET_STRING_VALUE( pItem, _AL("Name"), stage.m_strName );
			XMLGET_INT_VALUE( pItem, _AL("AcupointNum"), stage.m_nAcupointNum );
			XMLGET_FLOAT_VALUE( pItem, _AL("AcupointSpeed"), stage.m_fMoveSpeed );
			XMLGET_FLOAT_VALUE( pItem, _AL("CheckRange"), stage.m_fCheckRange );

			stage.m_aCheckPoints = new AArray<CECGameAcupoint::CheckPoint>();

			// Load check points.
			AXMLItem *pItemChild = pItem->GetFirstChildItem();
			for (int checkPointIndex = 1; checkPointIndex <= stage.m_nAcupointNum; checkPointIndex ++)
			{
				ACString checkPointControlName;
				checkPointControlName.Format(_AL("Acupoint%d"), checkPointIndex);

				AXMLItem *cpItem = FindCheckPointItemWithName(pItemChild, checkPointControlName);
				assert( cpItem != NULL );
				if (cpItem == NULL)
				{
					a_LogOutput(1, "CECGameRush::CECGameRush, can't find Check Point %d in config file.", checkPointIndex);
					delete stage.m_aCheckPoints;
					return;
				}

				int key = 0;

				CECGameAcupoint::CheckPoint cp;
				int x, y, width, height;
//				XMLGET_FLOAT_VALUE(cpItem, _AL("x"), cp.m_ptPoint.x);
//				XMLGET_FLOAT_VALUE(cpItem, _AL("y"), cp.m_ptPoint.y);
				XMLGET_INT_VALUE(cpItem, _AL("x"), x);
				XMLGET_INT_VALUE(cpItem, _AL("y"), y);
				XMLGET_INT_VALUE(cpItem, _AL("Width"), width);
				XMLGET_INT_VALUE(cpItem, _AL("Height"), height);

				cp.m_ptPoint.x = x + width / 2.0f;
				cp.m_ptPoint.y = y + height / 2.0f;
				cp.m_ptPoint.z = 0.0f;

				XMLGET_INT_VALUE(cpItem, _AL("Key"), key);
				cp.m_bIsKeyCheckPoint = key == 1;
				cp.m_sStatus = CECGameAcupoint::SEALED;

				stage.m_aCheckPoints->Add(cp);
			}

			m_pGameAcupoint->AddStage(stage);
		}

		pItem = pItem->GetNextItem();
	}
}

CDlgGameAcupoint::~CDlgGameAcupoint( )
{
	delete m_pGameAcupoint;
	m_xGameDataFile.Release( );
}

void CDlgGameAcupoint::OnHideDialog( )
{
	// Hide result UI.
	GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_WinGfx" )->Show( false );
	GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_LoseGfx" )->Show( false );
}

bool CDlgGameAcupoint::Tick( )
{
	bool ret = CDlgBase::Tick( );

	CECGameAcupoint::GAME_PHASE lastGamePhase = m_pGameAcupoint->GetGamePhase( );

	m_pGameAcupoint->Tick( );

	if ( !m_bIsShowEndingUI && m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_WIN )
		ShowWinUI( );
	else if ( !m_bIsShowEndingUI && m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_OVER )
		ShowLoseUI( );
	else if ( !m_bIsShowWaitUI && m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_WAIT )
		ShowWaitUI( );
	
	if ( m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_PLAY )
	{
		m_pPauseBtn->Show( true );
		m_pContinueBtn->Show( false );
	}
	else if ( m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_PAUSE )
	{
		m_pPauseBtn->Show( false );
		m_pContinueBtn->Show( true );
	}

	// Update check points.
	const AArray<CECGameAcupoint::CheckPoint>* cpData = m_pGameAcupoint->GetCheckPoints( );
	for ( int i = 0; i < cpData->GetSize( ); ++ i )
	{
		if ( !(*cpData)[i].m_bIsKeyCheckPoint )
			continue;

		if ( (*cpData)[i].m_sStatus == CECGameAcupoint::ACTIVE )
		{
			m_pCheckPointBtns[i]->SetState( AUISTILLIMAGEBUTTON_STATE_NORMAL );
			m_pCheckPointBtns[i]->Enable(true);
		}
		else if ( (*cpData)[i].m_sStatus == CECGameAcupoint::PASS )
		{
			m_pCheckPointBtns[i]->SetState( AUISTILLIMAGEBUTTON_STATE_CLICK );
			m_pCheckPointBtns[i]->Enable( true );
		}
		else
		{
			m_pCheckPointBtns[i]->Enable( false );
		}
	}

	if ( m_pGameAcupoint->GetGamePhase( ) == CECGameAcupoint::GAME_WAIT )
		return ret;

	// Update lives' UI.
	ACString szText;
	szText.Format( _AL("%d/%d"), m_pGameAcupoint->GetLives( ), m_pGameAcupoint->GetMaxLives( ) );
	m_pLivesLabel->SetText( szText );

	// Update rusher.
	int rusherX = (int) (m_pGameAcupoint->GetRusherPos( ).x - m_pRusherBtn->GetSize( ).cx / 2.0f);
	int rusherY = (int) (m_pGameAcupoint->GetRusherPos( ).y - m_pRusherBtn->GetSize( ).cy / 2.0f);
	m_pRusherBtn->SetPos( rusherX, rusherY );

	return ret;
}

bool CDlgGameAcupoint::OnInitDialog( )
{
	m_pPauseBtn = (PAUISTILLIMAGEBUTTON) GetDlgItem( "Btn_Pause" );
	m_pContinueBtn = (PAUISTILLIMAGEBUTTON) GetDlgItem( "Btn_Continue" );
	m_pLivesLabel = (PAUILABEL) GetDlgItem( "Txt_Livs" );
	m_pCountDown = (PAUIIMAGEPICTURE) GetDlgItem( "Gfx_CountDown" );

	return CDlgBase::OnInitDialog( );
}

void CDlgGameAcupoint::OnCommandPause( const char * szCommand )
{
	m_pGameAcupoint->PauseGame( true );
}

void CDlgGameAcupoint::OnCommandContinue( const char * szCommand )
{
	m_pGameAcupoint->PauseGame( false );
}

void CDlgGameAcupoint::OnEventKeyDown( WPARAM wParam, LPARAM lParam, AUIObject * pObj )
{
	if( wParam == VK_SPACE )
	{
		m_pGameAcupoint->InteractCheckPoint( );
	}
	else if ( wParam == VK_RETURN )
	{
		ShowStageUI( );
	}
	else if ( wParam == VK_F1 )
	{
		StartStage( 0 );
	}
}

bool CDlgGameAcupoint::StartStage( int gameid )
{
	if ( gameid < GAME_ID_BEGIN || gameid > GAME_ID_END )
		return false;

	if ( !m_pGameAcupoint->StartGame( gameid - GAME_ID_BEGIN, 5000 ) )
		return false;
	
	m_nGameID = gameid;
	ShowStageUI( );

	return true;
}

void CDlgGameAcupoint::	ShowStageUI( )
{
	ClearStageUI( );

	// Hide result UI.
	GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_WinGfx" )->Show( false );
	GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_LoseGfx" )->Show( false );

	int stageCount = m_pGameAcupoint->GetCurrentStage( );
	int acupointNum = 0;
	AXMLItem *pItem = m_xGameDataFile.GetRootItem( )->GetFirstChildItem( );
	pItem = pItem->GetFirstChildItem( );
	while ( pItem )
	{
		if( a_stricmp( pItem->GetName( ), _AL("STAGE") ) == 0 )
		{
			if ( stageCount == 0 )
			{
				XMLGET_INT_VALUE( pItem, _AL("AcupointNum"), acupointNum );

				int checkPointIndex = 1;
				ACString checkPointControlName;
				checkPointControlName.Format( _AL("Acupoint%d"), checkPointIndex );

				AXMLItem *pItemChild = pItem->GetFirstChildItem( );

				// Add game UI.
				while ( pItemChild )
				{
					if( a_stricmp( pItemChild->GetName( ), _AL("STILLIMAGEBUTTON") ) == 0 )
						AddStillImageBtn( pItemChild );
					else if ( a_stricmp( pItemChild->GetName(), _AL("IMAGEPICTURE") ) == 0 )
						AddImagePic( pItemChild );
					else if ( a_stricmp( pItemChild->GetName(), _AL("LABEL") ) == 0 )
						AddLabel( pItemChild );

					pItemChild = pItemChild->GetNextItem( );
				}

				break;
			}
			else
			{
				stageCount --;
			}
		}

		pItem = pItem->GetNextItem( );
	}

	// Get lives' label.
	m_pLivesLabel = (PAUILABEL) GetDlgItem( "Txt_Livs" );

	// Get check points' button.
	m_pCheckPointBtns.RemoveAll( );
	for ( int checkPointIndex = 1; checkPointIndex <= acupointNum; checkPointIndex ++ )
	{
		AString checkPointControlName;
		checkPointControlName.Format( "Acupoint%d", checkPointIndex );

		PAUISTILLIMAGEBUTTON btn =(PAUISTILLIMAGEBUTTON) GetDlgItem( checkPointControlName );
		assert( btn != NULL );
		if ( btn == NULL )
		{
			a_LogOutput( 1, "CECGameRush::ShowStageUI, can't find Check Point %d in config file.", checkPointIndex );
			return;
		}

		m_pCheckPointBtns.Add( btn );
	}

	// Get rusher button.
	m_pRusherBtn = (PAUIIMAGEPICTURE) GetDlgItem( "ZhenYuan" );
	PutControlToBottom( m_pRusherBtn );

	// Hide result dialog.
	m_bIsShowWaitUI = false;
	m_bIsShowEndingUI = false;
}

void CDlgGameAcupoint::ShowWinUI( )
{
	PAUIDIALOG winDlg = GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_WinGfx" );
	winDlg->Show( true );
	winDlg->SetData( m_nGameID );
	m_bIsShowEndingUI = true;
}

void CDlgGameAcupoint::ShowLoseUI( )
{
	PAUIDIALOG loseDlg = GetGameUIMan( )->GetDialog( "Win_Game_Acupoint_LoseGfx" );
	loseDlg->Show( true );
	loseDlg->SetData( m_nGameID );
	m_bIsShowEndingUI = true;
}

void CDlgGameAcupoint::ShowWaitUI( )
{
	m_pCountDown->Show( true );
	m_bIsShowWaitUI = true;
}

void CDlgGameAcupoint::ClearStageUI( )
{
	for ( int i = 0; i < m_pAUIImages.GetSize( ); ++ i )
		DeleteControl( m_pAUIImages[i] );
	m_pAUIImages.RemoveAll( );

	for ( int i = 0; i < m_pAUIImageBtns.GetSize( ); ++ i )
		DeleteControl( m_pAUIImageBtns[i] );
	m_pAUIImageBtns.RemoveAll( );

	for ( int i = 0; i < m_pAUILabels.GetSize(); ++ i )
		DeleteControl( m_pAUILabels[i] );
	m_pAUILabels.RemoveAll( );
}

PAUISTILLIMAGEBUTTON CDlgGameAcupoint::AddStillImageBtn( AXMLItem *xml )
{
	if ( xml == NULL )
		return NULL;

	PAUISTILLIMAGEBUTTON newBtn = new AUIStillImageButton;
	newBtn->SetParent( this );
	newBtn->InitXML( g_pGame->GetA3DEngine( ), g_pGame->GetA3DDevice( ), xml );
	AddObjectToList( newBtn );

	m_pAUIImageBtns.Add( newBtn );
	return newBtn;
}

PAUIIMAGEPICTURE CDlgGameAcupoint::AddImagePic( AXMLItem *xml )
{
	if ( xml == NULL )
		return NULL;

	PAUIIMAGEPICTURE newPic = new AUIImagePicture;
	newPic->SetParent( this );
	newPic->InitXML( g_pGame->GetA3DEngine( ), g_pGame->GetA3DDevice( ), xml );
	AddObjectToList( newPic );

	m_pAUIImages.Add( newPic );
	return newPic;
}

PAUILABEL CDlgGameAcupoint::AddLabel( AXMLItem *xml )
{
	if ( xml == NULL )
		return NULL;

	PAUILABEL newLabel = new AUILabel;
	newLabel->SetParent( this );
	newLabel->InitXML( g_pGame->GetA3DEngine( ), g_pGame->GetA3DDevice( ), xml );
	AddObjectToList( newLabel );

	m_pAUILabels.Add( newLabel );
	return newLabel;
}