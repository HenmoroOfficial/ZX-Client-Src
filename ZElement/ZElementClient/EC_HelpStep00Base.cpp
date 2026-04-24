/********************************************************************
	created:	2012/8/17
	author:		fuchonggang
	
	purpose:	图形帮助系统-每一步操作基类
	Copyright (C) 2012 - All Rights Reserved
*********************************************************************/

#include "EC_HelpStep00Base.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_HelpGraphGuide.h"

void CECHelpStepBase::StartHelp(int index)
{
	if ( index<m_iStartIndex || index>m_iEndIndex )
		return;
	
	if ( (index-m_iStartIndex)==0 )
		Help0();
	else if ( (index-m_iStartIndex)==1 )
		Help1();
	else if ( (index-m_iStartIndex)==2 )
		Help2();
	else if ( (index-m_iStartIndex)==3 )
		Help3();
	else if ( (index-m_iStartIndex)==4 )
		Help4();
	else if ( (index-m_iStartIndex)==5 )
		Help5();
	else if ( (index-m_iStartIndex)==6 )
		Help6();
	else if ( (index-m_iStartIndex)==7 )
		Help7();
	else if ( (index-m_iStartIndex)==8 )
		Help8();
	else if ( (index-m_iStartIndex)==9 )
		Help9();

	m_iCurIndex = index;
}

void CECHelpStepBase::FinishHelp(int index)
{
	if ( index<m_iStartIndex || index>m_iEndIndex )
		return;
	
	if ( (index-m_iStartIndex)==0 )
		Help0(false);
	else if ( (index-m_iStartIndex)==1 )
		Help1(false);
	else if ( (index-m_iStartIndex)==2 )
		Help2(false);
	else if ( (index-m_iStartIndex)==3 )
		Help3(false);
	else if ( (index-m_iStartIndex)==4 )
		Help4(false);
	else if ( (index-m_iStartIndex)==5 )
		Help5(false);
	else if ( (index-m_iStartIndex)==6 )
		Help6(false);
	else if ( (index-m_iStartIndex)==7 )
		Help7(false);
	else if ( (index-m_iStartIndex)==8 )
		Help8(false);
	else if ( (index-m_iStartIndex)==9 )
		Help9(false);
	else
		return;

	if (index==m_iEndIndex)
	{
		g_pGame->GetGameRun()->GetHelpGraphGuide()->FinishHelpAction(m_iEndIndex);
		m_iCurIndex = -1;
	}
	else
		g_pGame->GetGameRun()->GetHelpGraphGuide()->StartHelp(index+1);	
}
