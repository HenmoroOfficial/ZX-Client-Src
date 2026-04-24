// Filename	: DlgPKSetting.cpp
// Creator	: Fu Chonggang
// Date		: 2009/10/20

#include "AFI.h"
#include "AUI\\AUILabel.h"
#include "DlgPKMsg.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Global.h"
#include "EC_World.h"
#include "EC_ManPlayer.h"
#define new A_DEBUG_NEW

const ACHAR DefaultFontName[]=TEXT("方正细黑一简体");

CDlgPKMsg::CDlgPKMsg()
{
}

CDlgPKMsg::~CDlgPKMsg()
{
}

bool CDlgPKMsg::OnInitDialog()
{
	AString strName;
	for(int i=0; i<CDLGPKMSG_NUM; i++)
	{
		strName.Format("Message%d", i+1);
		m_pLabMsg[i] = (PAUILABEL)GetDlgItem(strName);
		m_iOrigX[i] = m_pLabMsg[i]->GetPos(true).x;
		m_iOrigY[i] = m_pLabMsg[i]->GetPos(true).y;
	}

	m_dwShowTime = 0;

	return true;
}

void CDlgPKMsg::OnShowDialog()
{
	for(int i=0; i<CDLGPKMSG_NUM; i++)
	{
		m_pLabMsg[i]->SetText(_AL(""));
	}
}

void CDlgPKMsg::OnTick()
{
	if (GetGameUIMan()->GetDialog("Win_Popmsg")->IsShow())
	{
		for (int i=0; i<CDLGPKMSG_NUM; i++)
		{
			m_pLabMsg[i]->SetText(_AL(""));
			m_pLabMsg[i]->SetData(0);
		}
		Show(false);
		return;
	}

	static const int s_ShowTime = 5*1000;
	DWORD dwTimeNow = GetTickCount();
	DWORD dwTimePast;
	int   iFontHeight;
	int i;
	for (i=0; i<CDLGPKMSG_NUM; i++)
	{
		if (a_stricmp(m_pLabMsg[i]->GetText(), _AL(""))==0)
			continue;
		int   iOutLine = 0;
		A3DCOLOR outLine = 0x0;
		dwTimePast = dwTimeNow - m_pLabMsg[i]->GetData();
		if (dwTimePast >= 0 && dwTimePast < 500)
		{
			iFontHeight = 12 + dwTimePast*12/500;
			outLine = 0xffff8080;
			iOutLine = 255;
		}
		else if (dwTimePast >= 500 && dwTimePast < 1000)
		{
			iFontHeight = 12 + (1000 - dwTimePast)*12/500;
			outLine = 0xffff8080;
			iOutLine = 255;
		}
		else
		{
			iFontHeight = 12;
		}
		m_pLabMsg[i]->SetPos(m_iOrigX[i], m_iOrigY[i] - (iFontHeight-12)/2);
		m_pLabMsg[i]->SetFontAndColor(DefaultFontName, iFontHeight, 0xffffffff, 0, 0, false, false, true, outLine);
	}

	dwTimePast = dwTimeNow - m_dwShowTime;
	if (dwTimePast > s_ShowTime)
	{
		int alphaWhole = 255 - (dwTimePast - s_ShowTime)/10;
		if (alphaWhole<0)
		{
			alphaWhole = 0;
		}
		SetWholeAlpha(alphaWhole);
		if (GetWholeAlpha() == 0)
		{
			for (i=0; i<CDLGPKMSG_NUM; i++)
			{
				m_pLabMsg[i]->SetText(_AL(""));
				m_pLabMsg[i]->SetData(0);
			}
			Show(false);
		}
	}
}

void CDlgPKMsg::SetInfo(int killer_id, int deader_id, ACHAR* killer_name, ACHAR* deader_name)
{
	CECPlayerMan *pPlayerMan = g_pGame->GetGameRun()->GetWorld()->GetPlayerMan();
	if ( (killer_name == NULL && pPlayerMan->GetPlayer(killer_id)==NULL) || 
		 (deader_name == NULL && pPlayerMan->GetPlayer(deader_id)==NULL) )
	{
		return;
	}
	int idHost = GetHostPlayer()->GetCharacterID();

	// 匿名地图，不显示击杀信息
	if (GetHostPlayer()->IsInAnonymousMap() && idHost != killer_id)
		return;

	ACString strMsg;
	if (idHost == killer_id)
	{
		strMsg.Format(GetStringFromTable(9165), deader_name ? deader_name : pPlayerMan->GetPlayer(deader_id)->GetName());
	}
	else if (idHost == deader_id)
	{
		strMsg.Format(GetStringFromTable(9166), killer_name ? killer_name : pPlayerMan->GetPlayer(killer_id)->GetName());
	}
	else
	{
		strMsg.Format(GetStringFromTable(9167), killer_name ? killer_name : pPlayerMan->GetPlayer(killer_id)->GetName(), 
												deader_name ? deader_name : pPlayerMan->GetPlayer(deader_id)->GetName());
	}
	GetGameUIMan()->AddChatMessage(strMsg, CLIENT_CHAT_KILL);
	AddMessage(strMsg);
}

void CDlgPKMsg::AddMessage(ACString strText)
{
	m_dwShowTime = GetTickCount();
	if (!IsShow() || GetWholeAlpha()<255)
	{
		Show(true, false, false);
		SetWholeAlpha(255);
	}
	for (int i=CDLGPKMSG_NUM-1; i>0; i--)
	{
		if (a_stricmp(m_pLabMsg[i-1]->GetText(), _AL(""))!=0)
		{
			m_pLabMsg[i]->SetText(m_pLabMsg[i-1]->GetText());
			m_pLabMsg[i]->SetData(m_pLabMsg[i-1]->GetData());
		}
	}
	
	m_pLabMsg[0]->SetText(strText);
	m_pLabMsg[0]->SetData(m_dwShowTime);

}