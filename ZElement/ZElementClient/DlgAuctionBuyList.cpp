// Filename	: DlgAuctionBuyList.cpp
// Creator	: Xiao Zhou
// Date		: 2005/11/10

#include "AFileImage.h"
#include "DlgAuctionBuyList.h"
#include "DlgAuctionList.h"
#include "DlgAuctionSellList.h"
#include "DlgAuctionSearch.h"
#include "AUI\\CSplit.h"
#include "AUI\\AUICommon.h"
#include "AUI\\AUICTranslate.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_IvtrItem.h"
#include "AWScriptFile.h"
#include "auctionget_re.hpp"
#include "elementdataman.h"
#include "A3DDevice.h"

#define new A_DEBUG_NEW

#define LIST_TICK 5000

AUI_BEGIN_COMMAND_MAP(CDlgAuctionBuyList, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",			OnCommandCancel)
AUI_ON_COMMAND("auctionbuylist",	OnCommandAuctionBuyList)
AUI_ON_COMMAND("auctionlist",		OnCommandAuctionList)
AUI_ON_COMMAND("auctionselllist",	OnCommandAuctionSellList)
AUI_ON_COMMAND("searchid",			OnCommandSearchID)
AUI_ON_COMMAND("searchmode",		OnCommandSearchMode)
AUI_ON_COMMAND("buy",				OnCommandBuy)
AUI_ON_COMMAND("binbuy",			OnCommandBinBuy)
AUI_ON_COMMAND("last",				OnCommandLast)
AUI_ON_COMMAND("refresh",			OnCommandRefresh)
AUI_ON_COMMAND("next",				OnCommandNext)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAuctionBuyList, CDlgBase)

AUI_ON_EVENT("Lst_Item",WM_LBUTTONDOWN,	OnEventLButtonDown_Lst_Item)

AUI_END_EVENT_MAP()

CDlgAuctionBuyList::CDlgAuctionBuyList()
{
	m_pTxt_AuctionID = NULL;
	m_pTxt_PackGold = NULL;
	m_pTxt_RefreshStatus = NULL;
	m_pTxt_Search = NULL;
	m_nIDNext = -1;
	m_nIDNow = 0;
	m_nSearchID = 0;
}

CDlgAuctionBuyList::~CDlgAuctionBuyList()
{
}

bool CDlgAuctionBuyList::OnInitDialog()
{
	DDX_Control("Txt_AuctionID", m_pTxt_AuctionID);
	DDX_Control("Txt_PackGold", m_pTxt_PackGold);
	DDX_Control("Txt_RefreshStatus", m_pTxt_RefreshStatus);
	DDX_Control("Txt_Search", m_pTxt_Search);
	m_pTxt_AuctionID->SetIsNumberOnly(true);
	m_pTxt_PackGold->SetIsNumberOnly(true);
	m_mapAuctionID.clear();
	m_mapAuctionName.clear();
	m_mapAuctionHint.clear();
	m_dwLastListTime = GetTickCount();

	AWScriptFile s;

	if( s.Open("surfaces\\ingame\\auctionid.txt") )
	{
		while( !s.IsEnd() )
		{
			if( !s.GetNextToken(true) )
				break;
			int id = a_atoi(s.m_szToken);
			if( !s.GetNextToken(true) )
				break;
			int auctionid = a_atoi(s.m_szToken);
			if( !s.GetNextToken(true) )
				break;
			m_mapAuctionID[id] = auctionid;
			if( id == auctionid || auctionid > 10000000 )
				m_mapAuctionName[auctionid] = s.m_szToken;
		}
		s.Close();
	}
	else
		AUI_ReportError(__LINE__, 1,"CDlgAuctionBuyList::OnInitDialog(), auction id file error!");
	
	return CDlgAuctionBase::OnInitDialog();
}

void CDlgAuctionBuyList::OnShowDialog()
{
	CDlgAuctionBase::OnShowDialog();

	m_pBtn_AuctionBuyList->SetPushed(true);
	m_pTxt_Price->SetText(_AL("0"));
}

void CDlgAuctionBuyList::OnTick()
{
	CDlgAuctionBase::OnTick();

	DWORD dwTick = GetTickCount();

	if( dwTick - m_dwLastListTime > LIST_TICK && m_nIDNext != -1 )
	{
		GetGameSession()->auction_List(m_nIDNext, m_nPageNext * 16, false);
		m_nIDNow = m_nIDNext;
		m_nIDNext = -1;
		m_nPage = m_nPageNext;
		m_nPageNext = 0;
		m_dwLastListTime = dwTick;
	}

	ACHAR szGold[20];
	a_sprintf(szGold, _AL("%d"), GetHostPlayer()->GetMoneyAmount() );
	m_pTxt_PackGold->SetText(szGold);
}

void CDlgAuctionBuyList::OnCommandSearchID(const char* szCommand)
{
	if( m_pTxt_AuctionID->GetText()[0] != _AL('\0') &&
		isdigit(m_pTxt_AuctionID->GetText()[0]) )
	{
		m_pLst_Item->ResetContent();
		m_nSearchID = a_atoi(m_pTxt_AuctionID->GetText());
		GetGameSession()->auction_Get(m_nSearchID);
		ACString szText;
		szText.Format(GetStringFromTable(3502), m_nSearchID);
		m_pTxt_RefreshStatus->SetText(szText);
	}
}

void CDlgAuctionBuyList::OnCommandSearchMode(const char* szCommand)
{
	GetGameUIMan()->m_pDlgAuctionSearch->Show(true, true);
}

void CDlgAuctionBuyList::OnCommandLast(const char* szCommand)
{
	if( m_nPage > 0 )
	{
		m_pTxt_RefreshStatus->SetText(GetStringFromTable(3507));
		m_nPageNext = m_nPage - 1;
		m_nIDNext = m_nIDNow;
	}
	else
		GetGameUIMan()->MessageBox("",GetStringFromTable(758), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgAuctionBuyList::OnCommandNext(const char* szCommand)
{
	m_pTxt_RefreshStatus->SetText(GetStringFromTable(3508));
	m_nPageNext = m_nPage + 1;
	m_nIDNext = m_nIDNow;
}

void CDlgAuctionBuyList::OnCommandRefresh(const char* szCommand)
{
	m_nPageNext = m_nPage;
	m_nIDNext = m_nIDNow;
}

void CDlgAuctionBuyList::UpdateItem(void *pData)
{
	AuctionGet_Re *pLstItem = (AuctionGet_Re *)pData;
	if( pLstItem->retcode != ERR_SUCCESS )
	{
		m_pTxt_RefreshStatus->SetText(_AL(""));
		return;
	}

	m_pTxt_RefreshStatus->SetText(_AL(""));
	m_nSearchID = 0;
	if( pLstItem->retcode == ERR_AS_BID_NOTFOUND )
	{
		GetGameUIMan()->MessageBox("",GetStringFromTable(668), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}

	GAuctionDetail pDetail = pLstItem->item;
	GRoleInventory pIvtr = pDetail.item;
	GAuctionItem pAuctionItem = pDetail.info;
	CECIvtrItem *pItem = CECIvtrItem::CreateItem(pIvtr.id, pIvtr.expire_date, pIvtr.count);
	ACString szName,szText, szTime;
	int nRestTime = pAuctionItem.end_time - GetGame()->GetServerGMTTime();
	if( nRestTime > 3600 )
		szTime.Format(GetStringFromTable(1603), nRestTime / 3600);
	else
		szTime.Format(GetStringFromTable(1602), nRestTime / 60);
	if( pAuctionItem.count > 1)
		szName.Format(_AL("%s(%d)"), pItem->GetName(), pAuctionItem.count);
	else
		szName = pItem->GetName();
	if( pAuctionItem.binprice > 0 )
		szText.Format(_AL("%d\t%s\t%s\t%d\t%d"), pAuctionItem.auctionid, szName, 
			szTime, pAuctionItem.bidprice, pAuctionItem.binprice);
	else
		szText.Format(_AL("%d\t%s\t%s\t%d\t "), pAuctionItem.auctionid, szName, 
			szTime, pAuctionItem.bidprice);
	m_pLst_Item->AddString(szText);
	pItem->SetItemInfo((unsigned char*)pIvtr.data.begin(), pIvtr.data.size());
	pItem->UpdateInfoMD5();
	const wchar_t* szDesc = pItem->GetDesc();
	AUICTranslate trans;
	if( szDesc )
		m_pLst_Item->SetItemHint(m_pLst_Item->GetCount() - 1, trans.Translate(szDesc));
	else
		m_pLst_Item->SetItemHint(m_pLst_Item->GetCount() - 1, _AL(""));
	delete pItem;
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.auctionid);
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.seller, 1);
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.bidprice, 2);
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.binprice, 3);
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.bidder, 4);
	m_pLst_Item->SetItemData(m_pLst_Item->GetCount() - 1, pAuctionItem.end_time, 5);
	m_pLst_Item->SetCurSel(m_pLst_Item->GetCount());
}

void CDlgAuctionBuyList::UpdateList(void *pData)
{
	m_pTxt_RefreshStatus->SetText(_AL(""));
	CDlgAuctionBase::UpdateList(pData);
}