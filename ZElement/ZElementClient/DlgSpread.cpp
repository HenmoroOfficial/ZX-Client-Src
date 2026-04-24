// Filename	: DlgSpread.cpp
// Creator	: Fuchonggang
// Date		: 2009/04/24

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgSpread.h"
#include "DlgMiniMap.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUILabel.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_Friend.h"

#include "A3DDevice.h"

#include "reflistreferrals_re.hpp"


#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgSpread, CDlgBase)
AUI_ON_COMMAND("prev",			OnCommandPrev)
AUI_ON_COMMAND("next",			OnCommandNext)
AUI_ON_COMMAND("exp",			OnCommandGetExp)
AUI_ON_COMMAND("bonus",			OnCommandGetBonus)
AUI_ON_COMMAND("paste",			OnCommandPaste)
AUI_ON_COMMAND("pasteword",		OnCommandPaste)
AUI_ON_COMMAND("addfriend",		OnCommandAddFriend)
AUI_ON_COMMAND("refresh",		OnCommandRefresh)
AUI_ON_COMMAND("refreshword",	OnCommandRefreshWord)
AUI_ON_COMMAND("help",			OnCommandHelp)
AUI_ON_COMMAND("IDCANCEL",		OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgSpread, CDlgBase)

AUI_END_EVENT_MAP()


CDlgSpread::CDlgSpread()
{
}

CDlgSpread::~CDlgSpread()
{
}

bool CDlgSpread::OnInitDialog()
{
	if (m_szName == "Win_PopProfit")
	{
		m_iCurPage = 1;
		m_iMaxPage = 1;
		m_pLstAll = (PAUILISTBOX)GetDlgItem("Lst_All");
		m_pLabExpAvail = (PAUILABEL)GetDlgItem("Txt_ExpLeft");
		m_pLabBonusAvail = (PAUILABEL)GetDlgItem("Txt_PointLeft");
		m_pLabExpAvail->SetText(_AL("0"));
		m_pLabBonusAvail->SetText(_AL("0"));
	}
	else if (m_szName == "Win_PopFriend")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
		pLab->SetText(_AL(""));
		Show(true);
	}
	else if (m_szName == "Win_PopIdCheck")
	{
		m_iCurWordIndex = 0;
	}
	return true;
}

void CDlgSpread::OnShowDialog()
{
	if (m_szName == "Win_PopFriend")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
		ACHAR szText[256];
		const ACHAR *szName;
		szName = GetGameRun()->GetPlayerName(GetGameSession()->GetReferrerID(), false);

		if (szName)
		{
			a_sprintf(szText,GetStringFromTable(7391), GetGameSession()->GetReferrerID());
			pLab->SetText(szText);
		}
	}
	else if (m_szName == "Win_PopProfit")
		UpdatePage();
	else if (m_szName == "Win_PopIdCheck")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_1");
		const ACHAR *szText1 = GetHostPlayer()->GetSelfReferenceCode();
		if (szText1!=NULL)
		{
			pLab->SetText(szText1);
		}
		else
			pLab->SetText(_AL(""));
		if (m_iCurWordIndex==0)
		{
			m_iCurWordIndex = 4;
		}
		else
			m_iCurWordIndex--;
		OnCommandRefreshWord(NULL);
	}
}


bool CDlgSpread::Release()
{
	return CDlgBase::Release();
}

void CDlgSpread::OnTick()
{
	if (m_szName == "Win_PopFriend")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_1");
		const ACString strText = pLab->GetText();
		if (strText.GetLength()==0)
		{
			ACHAR szText[256];
			const ACHAR *szName;
			szName = GetGameRun()->GetPlayerName(GetGameSession()->GetReferrerID(), false);
			
			if (szName)
			{
				a_sprintf(szText,GetStringFromTable(7391), szName);
				pLab->SetText(szText);
			}
		}
	}
	else if (m_szName == "Win_PopIdCheck")
	{
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_1");
		ACString strText = pLab->GetText();
		if (strText.GetLength()==0)
		{
			const ACHAR *szText1 = GetHostPlayer()->GetSelfReferenceCode();
			if (szText1!=NULL)
			{
				pLab->SetText(szText1);
			}
		}
	}
	else if (m_szName == "Win_PopProfit")
	{
		GetDlgItem("Btn_Prev")->Enable(m_iCurPage>1);
		GetDlgItem("Btn_Next")->Enable(m_iCurPage<m_iMaxPage);
	}
}

void CDlgSpread::OnCommandPrev(const char * szCommand)
{
	if (m_iCurPage>=2)
	{
		m_iCurPage--;
		UpdatePage();
	}
}

void CDlgSpread::OnCommandNext(const char * szCommand)
{
	if (m_iCurPage<m_iMaxPage)
	{
		m_iCurPage++;
		UpdatePage();
	}
}

void CDlgSpread::OnCommandGetExp(const char * szCommand)
{
	if (m_iExpAvail>0)
	{
		GetGameSession()->ref_withdraw_exp();
	}
}

void CDlgSpread::OnCommandGetBonus(const char * szCommand)
{
	if (m_iBonusAvail>0)
	{
		GetGameSession()->ref_withdraw_bonus();
	}
}

void CDlgSpread::OnCommandPaste(const char * szCommand)
{
	PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_1");
	PAUIEDITBOX pTxt = (PAUIEDITBOX)GetDlgItem("Edt_Word");
	ACString source;
	ACHAR tempBuffer[1024];
	if (0==stricmp(szCommand, "pasteword"))
	{
		source= pTxt->GetText();
		int j = 0;
		for (int i=0; i<source.GetLength(); i++,j++)
		{
			ACHAR c = source[i];
			tempBuffer[j] = c;
			if (c == '\r')
			{
				tempBuffer[++j] = '\n';
			}
		}
		tempBuffer[j++] = '\0';
		source = tempBuffer;
		GetGameUIMan()->MessageBox("",GetStringFromTable(7395),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	else
	{
		source= pLab->GetText();
		GetGameUIMan()->MessageBox("",GetStringFromTable(7394),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	}
	if (source.GetLength()==0)
	{
		return;
	}
	//put your text in source
	if(OpenClipboard(g_pGame->GetA3DDevice()->GetDeviceWnd()))
	{
		HGLOBAL clipbuffer = GlobalAlloc(LMEM_ZEROINIT, 2*(source.GetLength())+2);
		if (clipbuffer)
		{
			ACHAR *buffer = (ACHAR*)GlobalLock(clipbuffer);
			a_strcpy(buffer, source);

			EmptyClipboard();
#ifdef UNICODE
			SetClipboardData(CF_UNICODETEXT, buffer);
#else
			SetClipboardData(CF_TEXT, buffer);
#endif
			GlobalUnlock(clipbuffer);
		}
		CloseClipboard();
	}
}

void CDlgSpread::OnCommandRefreshWord(const char * szCommand)
{
	m_iCurWordIndex++;
	if (m_iCurWordIndex>4)
	{
		m_iCurWordIndex = 0;
	}
	PAUIEDITBOX pTxt = (PAUIEDITBOX)GetDlgItem("Edt_Word");
	ACString strText;
	strText.Format(GetGameUIMan()->GetStringFromTable(8450+m_iCurWordIndex),
		GetGameRun()->GetCurGroup(),GetGameRun()->GetServerName(),
		GetHostPlayer()->GetName(), GetHostPlayer()->GetSelfReferenceCode());
	
	pTxt->SetText(strText);
}

void CDlgSpread::OnCommandAddFriend(const char * szCommand)
{
	const ACHAR *pszName;
	pszName = GetGameRun()->GetPlayerName(GetGameSession()->GetReferrerID(), false);
	
	if (pszName)
	{
		CECFriendMan::FRIEND* pFriend = GetHostPlayer()->GetFriendMan()->GetFriendByID(GetGameSession()->GetReferrerID());
		if( pFriend && !a_strcmp(pszName, pFriend->GetName()) )
			GetGameUIMan()->AddChatMessage(GetStringFromTable(565), GP_CHAT_MISC);
		else
		{
			GetGameSession()->friend_Add(GetGameSession()->GetReferrerID(), pszName);
			GetGameUIMan()->AddChatMessage(GetStringFromTable(533), GP_CHAT_MISC);
		}
	}
}

void CDlgSpread::OnCommandRefresh(const char * szCommand)
{
	UpdatePage();
}

void CDlgSpread::OnCommandHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_PopProfit_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_PopProfit_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgSpread::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}

void CDlgSpread::ReferAction(void *pData)
{
	RefListReferrals_Re *p = (RefListReferrals_Re*)pData;

	if (p->retcode != ERROR_SUCCESS)
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(7599+p->retcode),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		m_pLstAll->ResetContent();
		m_pLabExpAvail->SetText(_AL("0"));
		m_pLabBonusAvail->SetText(_AL("0"));
		return;
	}

	ACHAR szText[20];
	ACString strText;
	ACString strPage = _AL("");
	m_iBonusAvail = p->bonus_avail_today;
	m_iExpAvail	  = p->exp_avail_today;
	m_iMaxPage	  = (p->total+14)/15;
	GetGameUIMan()->Int64ToStr(m_iExpAvail, strText);
	m_pLabExpAvail->SetText(strText);
	a_sprintf(szText,_AL("%d"),m_iBonusAvail);
	m_pLabBonusAvail->SetText(szText);
	m_pLstAll->ResetContent();
	int iSize = p->referrals.size();
	for (int i=0; i<iSize; i++)
	{
		ReferralBrief rInfo;
		rInfo.index = p->start_index*15;
		rInfo.roleid = p->referrals[i].roleid;
		rInfo.rolename = ACString((const ACHAR*)p->referrals[i].rolename.begin(), p->referrals[i].rolename.size() / sizeof(ACHAR));
		rInfo.level = p->referrals[i].level;
		rInfo.sub_level =
			p->referrals[i].sub_level;
		rInfo.bonus_total1 = p->referrals[i].bonus_total1;
		rInfo.bonus_total2 = p->referrals[i].bonus_total2;
		rInfo.bonus_left = p->referrals[i].bonus_left;
		rInfo.bonus_avail = p->referrals[i].bonus_avail;
		rInfo.exp_total1 = p->referrals[i].exp_total1;
		rInfo.exp_total2 = p->referrals[i].exp_total2;
		rInfo.exp_left = p->referrals[i].exp_left;
		strText.Empty();

		ACString strExp, strTotal, strTotal2;
		GetGameUIMan()->Int64ToStr(rInfo.exp_left, strExp);
		GetGameUIMan()->Int64ToStr(rInfo.exp_total1+rInfo.exp_total2, strTotal);
		GetGameUIMan()->Int64ToStr(rInfo.exp_total2, strTotal2);

		if (rInfo.sub_level>0)
		{
			if(rInfo.bonus_total1+rInfo.bonus_total2 - rInfo.bonus_left < MAXBONUS)
				strText.Format(_AL("%s\t%s%d\t%d/%d/%d(+%d)\t%s/%s(+%s)"),rInfo.rolename, GetStringFromTable(7012), rInfo.level,
					rInfo.bonus_avail,rInfo.bonus_left,rInfo.bonus_total1+rInfo.bonus_total2,rInfo.bonus_total2,
					strExp, strTotal, strTotal2);
			else
				strText.Format(_AL("%s\t%s%d\t MAX \t%s/%s(+%s)"),rInfo.rolename, GetStringFromTable(7012), rInfo.level,
					strExp, strTotal, strTotal2);				
		}
		else
			strText.Format(_AL("%s\t%d\t%d/%d/%d(+%d)\t%s/%s(+%s)"),rInfo.rolename, rInfo.level,
				rInfo.bonus_avail,rInfo.bonus_left,rInfo.bonus_total1+rInfo.bonus_total2,rInfo.bonus_total2,
				strExp, strTotal, strTotal2);
		m_pLstAll->AddString(strText);
	}
}

void CDlgSpread::UpdatePage()
{
	GetGameSession()->ref_get_referrals(15*(m_iCurPage-1));
}
