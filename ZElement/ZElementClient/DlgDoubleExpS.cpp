// File		: DlgDoubleExpS.cpp
// Creator	: Xiao Zhou
// Date		: 2005/12/15

#include "DlgDoubleExpS.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgDoubleExpS, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",		OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgDoubleExpS::CDlgDoubleExpS()
{
	m_pImg_Icon = NULL;
	m_pTxt_RestTime = NULL;
	m_nMode = 0;
}

CDlgDoubleExpS::~CDlgDoubleExpS()
{
}

void CDlgDoubleExpS::OnCommandCancel(const char *szCommand)
{
	GetGameUIMan()->RespawnMessage();	
}

bool CDlgDoubleExpS::OnInitDialog()
{
	DDX_Control("Img_Icon", m_pImg_Icon);
	DDX_Control("Txt_RestTime", m_pTxt_RestTime);
	m_pTxt_RestTime->SetAlign(AUIFRAME_ALIGN_CENTER);

	return true;
}

void CDlgDoubleExpS::OnShowDialog()
{
	//if( GetGameRun()->GetDoubleExpMode() == CECGameRun::DBEXP_OWN )
	//	m_pImg_Icon->FixFrame(1);
	//else
	//	m_pImg_Icon->FixFrame(0);
	m_pImg_Icon->FixFrame(0);
	m_pTxt_RestTime->SetText(_AL(""));
}

void CDlgDoubleExpS::OnTick()
{
	if(GetGameRun()->GetDoubleExpMode() < 2)
	{
	   Show(false);
	   return;
	}
	m_pImg_Icon->FixFrame(GetGameRun()->GetDoubleExpMode() - 2);
	ACString strMsg;
	strMsg.Format(GetStringFromTable(17950),GetStringFromTable(17951+GetGameRun()->GetDoubleExpMode() - 2));
	m_pImg_Icon->SetHint(strMsg);
	/*switch( GetGameRun()->GetDoubleExpMode() )
	{
	case CECGameRun::DBEXP_OWN:
//		m_pImg_Icon->FixFrame(1);
		m_pImg_Icon->SetHint(GetStringFromTable(778));
		break;
	case CECGameRun::DBEXP_GLOBAL:
//		m_pImg_Icon->FixFrame(0);
		m_pImg_Icon->SetHint(GetStringFromTable(777));
		break;
	case CECGameRun::DBEXP_NONE:
		Show(false);
		return;
	}*/
	int nRestTime = GetGameRun()->GetRemainDblExpTime();
	if( nRestTime <= 0 )
	{
		Show(false);
		return;
	}
	int nHour = nRestTime / 3600;
	int nMin = (nRestTime % 3600) / 60;
	int nSec = nRestTime % 60;
	ACString strText;
	if( nRestTime < 60 )
		strText.Format(GetStringFromTable(1601), nRestTime);
	else if( nHour == 0 )
		strText.Format(GetStringFromTable(1602), nMin);
	else
		strText.Format(GetStringFromTable(1611), nHour, nMin);
	m_pTxt_RestTime->SetText(strText);
}
