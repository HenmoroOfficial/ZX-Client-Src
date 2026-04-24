// Filename	: DlgAutoLock.cpp
// Creator	: Xiao Zhou
// Date		: 2007/10/17

#include "DlgAutoLock.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_GameSession.h"
#include "EC_IvtrScroll.h"
#include "EC_Inventory.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Instance.h"
#include "autolockset_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAutoLock, CDlgBase)

AUI_ON_COMMAND("confirm",		OnCommand_Confirm)

AUI_END_COMMAND_MAP()

CDlgAutoLock::CDlgAutoLock()
{
}

CDlgAutoLock::~CDlgAutoLock()
{
}

void CDlgAutoLock::OnCommand_Confirm(const char * szCommand)
{
	int nHour = a_atoi(GetDlgItem("Txt_Hour")->GetText());
	int nMin = a_atoi(GetDlgItem("Txt_Minute")->GetText());
	if( nHour >= 0 && nMin >= 0 )
	{
		int nTime = nHour * 3600 + nMin * 60;
		if( nTime == 0 )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(938), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		ACString strText;
		if( nHour == 0 )
			strText.Format(GetStringFromTable(1602), nMin);
		else if( nMin == 0 )
			strText.Format(GetStringFromTable(1603), nHour);
		else
			strText.Format(GetStringFromTable(1611), nHour, nMin);
		ACString strMsg;
		if( nTime > m_nTotalTime )
			strMsg.Format(GetStringFromTable(934), strText);
		else if( nTime < m_nTotalTime )
			strMsg.Format(GetStringFromTable(935), strText);
		else
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(937), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
		PAUIDIALOG pMsgBox;
		GetGameUIMan()->MessageBox("Game_SetAutoLock", strMsg, MB_OKCANCEL, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
		pMsgBox->SetData(nTime);
	}
}

void CDlgAutoLock::OnShowDialog()
{
	((PAUIEDITBOX)GetDlgItem("Txt_Hour"))->SetIsNumberOnly(true);
	((PAUIEDITBOX)GetDlgItem("Txt_Minute"))->SetIsNumberOnly(true);
	if(GetHostPlayer()->GetSrcServerID()==0)
		ChangeFocus(GetDlgItem("Txt_Hour"));
	GetDlgItem("Txt_Hour")->SetText(_AL(""));
	GetDlgItem("Txt_Minute")->SetText(_AL(""));
}

void CDlgAutoLock::OnTick()
{
	int nTime, nHour, nMin, nSec;
	ACString strText;
	if( !m_bLock )
		GetDlgItem("Txt_CountDown")->SetText(GetStringFromTable(932));
	else
	{
		nTime = m_nEndTime - GetGame()->GetServerGMTTime();
		if( nTime < 0 )
			nTime = 0;
		nHour = nTime / 3600;
		nMin = (nTime % 3600) / 60;
		nSec = nTime%60;
		if( nHour != 0 )
			strText.Format(GetStringFromTable(1603), nHour);
		else if( nMin != 0 )
			strText.Format(GetStringFromTable(1602), nMin);
		else
			strText.Format(GetStringFromTable(1601), nSec);
		GetDlgItem("Txt_CountDown")->SetText(strText);
	}
	nTime = m_nTotalTime;
	if( nTime < 0 )
		nTime = 0;
	nHour = nTime / 3600;
	nMin = (nTime % 3600) / 60;
	if( nHour == 0 )
		strText.Format(GetStringFromTable(1602), nMin);
	else if( nMin == 0 )
		strText.Format(GetStringFromTable(1603), nHour);
	else
		strText.Format(GetStringFromTable(1611), nHour, nMin);
	GetDlgItem("Txt_CurLimit")->SetText(strText);

	nHour = a_atoi(GetDlgItem("Txt_Hour")->GetText());
	nMin = a_atoi(GetDlgItem("Txt_Minute")->GetText());
	if( nHour > 24 )
		GetDlgItem("Txt_Hour")->SetText(_AL("24"));
	if( nMin > 59 )
		GetDlgItem("Txt_Minute")->SetText(_AL("59"));
}

void CDlgAutoLock::InitAutoLockInfo(void* pData)
{
	S2C::notify_safe_lock* p= (S2C::notify_safe_lock*)pData;
	m_bLock = p->active != 0;
	m_nTotalTime = p->total_time;
	m_nEndTime = p->time;
}

void CDlgAutoLock::SetAutoLockTime(void* pData)
{
	AutolockSet_Re* p = (AutolockSet_Re*)pData;
	if( p->result == 0 )
	{
		if( p->timeout > m_nTotalTime )
			m_nEndTime += p->timeout - m_nTotalTime;
		m_nTotalTime = p->timeout;
		ACString strText;
		int nTime, nHour, nMin;
		nTime = m_nTotalTime;
		if( nTime < 0 )
			nTime = 0;
		nHour = nTime / 3600;
		nMin = (nTime % 3600) / 60;
		if( nHour == 0 )
			strText.Format(GetStringFromTable(1602), nMin);
		else if( nMin == 0 )
			strText.Format(GetStringFromTable(1603), nHour);
		else
			strText.Format(GetStringFromTable(1611), nHour, nMin);
		ACString strMsg;
		strMsg.Format(GetStringFromTable(936), strText);
		GetGameUIMan()->MessageBox("", strMsg, MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
		GetGameUIMan()->MessageBox("", GetStringFromTable(933), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}