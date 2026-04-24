// Filename	:	DlgAuction.cpp
// Creator	:	Chen Zhixin
// Date		:	Apr 8, 2010

#include "DlgAuction.h"
#include "DlgMinimizeBar.h"
#include "EC_GameUIMan.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_IvtrItem.h"
#include "EC_GameSession.h"
#include "EC_GameRun.h"
#include "AFI.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIListBox.h"
#include "AUI\\AUICTranslate.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgAuction, CDlgBase)

AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Close",				OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Buy",				OnCommand_Buy)
AUI_ON_COMMAND("Btn_RangeBuy",			OnCommand_RangeBuy)
AUI_ON_COMMAND("Btn_DecideSingle",		OnCommand_ConsignSingle)
AUI_ON_COMMAND("Btn_DecideRange",		OnCommand_ConsignRange)
AUI_ON_COMMAND("Btn_DecideRangeConfirm",OnCommand_ConsignRangeConfirm)	
AUI_ON_COMMAND("Btn_Award",				OnCommand_GetAward)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgAuction, CDlgBase)

AUI_ON_EVENT("Btn_Add*",			WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("Btn_Minus*",			WM_LBUTTONUP,	OnEventLButtonUp)
AUI_ON_EVENT("List_BuyHistory",		WM_LBUTTONUP,	OnEventLButtonDownPrice)
AUI_ON_EVENT("Lst_Player",			WM_RBUTTONUP,	OnEventRButtonUpPlayer)

AUI_END_EVENT_MAP()

const int		CDlgAuction::AUCTION_NUM_MAX = 500;
int				CDlgAuction::m_siMoneyFloor = 0;
UniquePriceMap	CDlgAuction::m_mapUniquePrice;

//#define DEBUG_FOR_SERVER

CDlgAuction::CDlgAuction()
{
}

CDlgAuction::~CDlgAuction()
{
}

bool CDlgAuction::OnInitDialog()
{
	if (m_szName == "Win_InputMoney_Auction")
	{
		AString strName;
		for (int i=0; i<3; ++i)
		{
			strName.Format("Btn_Add%d", i+1);
			m_pBtnAdd[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
			strName.Format("Btn_Minus%d", i+1);
			m_pBtnMinus[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
			strName.Format("Txt_Money%d", i+1);
			m_pEdtPrice[i] = (PAUIEDITBOX)GetDlgItem(strName);
		}
		m_pEdtPrice[0] =  (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money1");
	}
	else if (m_szName == "Win_AuctionRangeBuy")
	{
		AString strName;
		for (int i=0; i<6; ++i)
		{
			strName.Format("Btn_Add%d", i+1);
			m_pBtnAdd[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
			strName.Format("Btn_Minus%d", i+1);
			m_pBtnMinus[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(strName);
			strName.Format("Txt_Money%d", i+1);
			m_pEdtPrice[i] = (PAUIEDITBOX)GetDlgItem(strName);
		}
		m_pEdtPrice[0] =  (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money1");
		m_pLabItemNum = dynamic_cast<PAUILABEL>(GetDlgItem("Txt_Item"));
	}
	return true;
}

void CDlgAuction::OnShowDialog()
{
	if (m_szName == "Win_InputMoney_Auction")
		GetGameUIMan()->GetDialog("Win_AuctionRangeBuy")->Show(false);
	else if (m_szName == "Win_AuctionRangeBuy")
		GetGameUIMan()->GetDialog("Win_InputMoney_Auction")->Show(false);
}

void CDlgAuction::OnHideDialog()
{
	if (m_szName == "Win_Auction")
	{
		GetGameUIMan()->GetDialog("Win_InputMoney_Auction")->Show(false);
		GetGameUIMan()->GetDialog("Win_AuctionRangeBuy")->Show(false);
		GetGameUIMan()->GetDialog("Win_AuctionRangeBuyConfirm")->Show(false);
		GetGameUIMan()->GetDialog("Win_AuctionChecklist")->Show(false);
	}
}

void CDlgAuction::OnTick()
{
	if (m_szName == "Win_InputMoney_Auction")
	{
		PressStateAction(m_pBtnAdd[0], m_pEdtPrice[0], 0, 200000);
		PressStateAction(m_pBtnAdd[1], m_pEdtPrice[1], 0, 99);
		PressStateAction(m_pBtnAdd[2], m_pEdtPrice[2], 0, 99);
		PressStateAction(m_pBtnMinus[0], m_pEdtPrice[0], 0, 200000, false);
		PressStateAction(m_pBtnMinus[1], m_pEdtPrice[1], 0, 99, false);
		PressStateAction(m_pBtnMinus[2], m_pEdtPrice[2], 0, 99, false);

		// 银、铜最多2位，不需限制，金需要限制
		int nCur;
		PAUIEDITBOX pEdt = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money1");
		nCur = a_atoi(pEdt->GetText());
		if (nCur>200000)
		{
			pEdt->SetText(_AL("200000"));
		}
	}
	else if (m_szName == "Win_AuctionRangeBuy")
	{
		PressStateAction(m_pBtnAdd[0], m_pEdtPrice[0], 0, 200000);
		PressStateAction(m_pBtnAdd[1], m_pEdtPrice[1], 0, 99);
		PressStateAction(m_pBtnAdd[2], m_pEdtPrice[2], 0, 99);
		PressStateAction(m_pBtnMinus[0], m_pEdtPrice[0], 0, 200000, false);
		PressStateAction(m_pBtnMinus[1], m_pEdtPrice[1], 0, 99, false);
		PressStateAction(m_pBtnMinus[2], m_pEdtPrice[2], 0, 99, false);
		
		PressStateAction(m_pBtnAdd[3], m_pEdtPrice[3], 0, 200000);
		PressStateAction(m_pBtnAdd[4], m_pEdtPrice[4], 0, 99);
		PressStateAction(m_pBtnAdd[5], m_pEdtPrice[5], 0, 99);
		PressStateAction(m_pBtnMinus[3], m_pEdtPrice[3], 0, 200000, false);
		PressStateAction(m_pBtnMinus[4], m_pEdtPrice[4], 0, 99, false);
		PressStateAction(m_pBtnMinus[5], m_pEdtPrice[5], 0, 99, false);

		PAUIEDITBOX pEdt = m_pEdtPrice[0];
		int nCur = a_atoi(pEdt->GetText());
		if (nCur>200000)
		{
			pEdt->SetText(_AL("200000"));
		}
		pEdt = m_pEdtPrice[3];
		nCur = a_atoi(pEdt->GetText());
		if (nCur>200000)
		{
			pEdt->SetText(_AL("200000"));
		}

		int moneyLow = 0;
		moneyLow += 10000 * a_atoi(m_pEdtPrice[0]->GetText());
		moneyLow += 100 * a_atoi(m_pEdtPrice[1]->GetText());	
		moneyLow += a_atoi(m_pEdtPrice[2]->GetText());
		int moneyHigh = 0;
		moneyHigh += 10000 * a_atoi(m_pEdtPrice[3]->GetText());
		moneyHigh += 100 * a_atoi(m_pEdtPrice[4]->GetText());	
		moneyHigh += a_atoi(m_pEdtPrice[5]->GetText());

		ACString strText;
		int num = moneyHigh-moneyLow+1;
		if (num<=0||moneyLow<10000)
			num = 0;

		strText.Format(_AL("%d"), num);
		m_pLabItemNum->SetText(strText);
	}
	else if (m_szName == "Win_Auction")
	{
	}
}

bool CDlgAuction::PressStateAction(AUIStillImageButton* pBtn, AUIEditBox* pEdt, int mn, int mx, bool bAdd)
{
	bool bRet = false;
	if (pBtn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK)
	{
		int nCur = a_atoi(pEdt->GetText());
		if(bAdd)
			nCur++;
		else
			nCur--;
		a_Clamp(nCur, mn, mx);
		ACString strText;
		strText.Format(_AL("%d"), nCur);
		pEdt->SetText(strText);
		bRet = true;
	}
	return bRet;
}

bool CDlgAuction::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

void CDlgAuction::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
}

void CDlgAuction::OnCommand_Buy(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_InputMoney_Auction");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgAuction::OnCommand_RangeBuy(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AuctionRangeBuy");
	pDlg->Show(!pDlg->IsShow());
}

void CDlgAuction::OnCommand_ConsignSingle(const char * szCommand)
{
	int money = 0;
	ACString strText;
	int idItem = GetGameRun()->GetSpecialIDConfig().id_unique_bid_item;
	PAUIEDITBOX pLab = (PAUIEDITBOX)GetDlgItem("DEFAULT_Txt_Money1");
	money += 10000 * a_atoi(pLab->GetText());
	pLab = (PAUIEDITBOX)GetDlgItem("Txt_Money2");
	money += 100 * a_atoi(pLab->GetText());	
	pLab = (PAUIEDITBOX)GetDlgItem("Txt_Money3");
	money += a_atoi(pLab->GetText());

	if(GetHostPlayer()->GetBoothState()==2)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9871), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
	else if (GetHostPlayer()->GetPack()->GetItemTotalNum(idItem)<=0)		// 出价符不够
	{
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, 0, 1);
		if (pItem)
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(9884), pItem->GetName());
			delete pItem;
		}
		GetGameUIMan()->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255,255,255,160), MB_OK);
	}
	else if (money<m_siMoneyFloor)									// 小于底价
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9874), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
	else if (m_mapUniquePrice.size()>=AUCTION_NUM_MAX)				// 最多出价AUCTION_NUM_MAX次
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9873), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
	else if (money>GetHostPlayer()->GetMoneyAmount())				// money不够
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9885), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
	else if (GetHostPlayer()->GetPack()->SearchEmpty()<0)			// 出价可能获得物品，包裹必须有空格子
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9887), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
	else if (m_mapUniquePrice.find(money)!=m_mapUniquePrice.end())	// 没出过这个价格
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9875), MB_OK, A3DCOLORRGBA(255,255,255,160));
	}
#ifndef DEBUG_FOR_SERVER
	else
#endif
	{
		PAUIDIALOG pDlg;
		
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, 0, 1);
		if (pItem)
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(9886), pItem->GetName());
			delete pItem;
		}
		GetGameUIMan()->MessageBox("Game_AuctionBuy", strText, MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
		pDlg->SetData(money);
	}
}

void CDlgAuction::OnCommand_ConsignRange(const char * szCommand)
{
	int moneyLow = 0;
	moneyLow += 10000 * a_atoi(m_pEdtPrice[0]->GetText());
	moneyLow += 100 * a_atoi(m_pEdtPrice[1]->GetText());	
	moneyLow += a_atoi(m_pEdtPrice[2]->GetText());
	int moneyHigh = 0;
	moneyHigh += 10000 * a_atoi(m_pEdtPrice[3]->GetText());
	moneyHigh += 100 * a_atoi(m_pEdtPrice[4]->GetText());	
	moneyHigh += a_atoi(m_pEdtPrice[5]->GetText());
	ACString strText;
	int numConsign = moneyHigh-moneyLow+1;
	int idItem = GetGameRun()->GetSpecialIDConfig().id_unique_bid_item;
	
	if(GetHostPlayer()->GetBoothState()==2)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9871), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}
	else if (moneyHigh<moneyLow)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9876), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}
	else if (moneyLow<m_siMoneyFloor)									// 小于底价
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9874), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}
	else if (GetHostPlayer()->GetPack()->SearchEmpty()<0)				//出价可能获得物品，包裹必须有空格子
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9887), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}
	if (GetHostPlayer()->GetPack()->GetItemTotalNum(idItem)<numConsign)	//出价符数量不够
	{
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(idItem, 0, 1);
		if (pItem)
		{
			strText.Format(GetGameUIMan()->GetStringFromTable(9872), pItem->GetName());
			delete pItem;
		}
		GetGameUIMan()->MessageBox("", strText, MB_OK, A3DCOLORRGBA(255,255,255,160), MB_OK);
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}
	else if (numConsign+m_mapUniquePrice.size()>AUCTION_NUM_MAX)
	{
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9873), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
		return;
#endif
	}

	int moneyTotal = 0;
	int packetMoney = GetHostPlayer()->GetMoneyAmount();
	for (int i=moneyLow; i<=moneyHigh; ++i)
	{
		moneyTotal += i;
		if (m_mapUniquePrice.find(i)!=m_mapUniquePrice.end())			// 已经出过这个价格
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9875), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
			return;
#endif
		}
		packetMoney -= i;
		if (packetMoney<0)												// 包裹金钱不够
		{
			GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(9885), MB_OK, A3DCOLORRGBA(255,255,255,160));
#ifndef DEBUG_FOR_SERVER
			return;
#endif
		}
	}

	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_AuctionRangeBuyConfirm");	
	PAUILABEL pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Gold");
	strText.Format(_AL("%d"), moneyTotal/10000);
	pLab->SetText(strText);
	pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Silver");
	strText.Format(_AL("%d"), (moneyTotal%10000)/100);
	pLab->SetText(strText);
	pLab = (PAUILABEL)pDlg->GetDlgItem("Txt_Bronze");
	strText.Format(_AL("%d"), moneyTotal%100);
	pLab->SetText(strText);
	pLab = (PAUILABEL)pDlg->GetDlgItem("Lab_Goods");
	strText.Format(_AL("%d"), numConsign);
	pLab->SetText(strText);
	pDlg->SetData(moneyLow);
	pDlg->SetDataPtr((void*)moneyHigh);
	pDlg->Show(true);
}

void CDlgAuction::OnCommand_ConsignRangeConfirm(const char * szCommand)
{
	GetGameSession()->c2s_CmdUniqueBid(GetData(), (int)GetDataPtr());
	Show(false);
	GetGameUIMan()->GetDialog("Win_AuctionRangeBuy")->Show(false);
}

void CDlgAuction::OnCommand_GetAward(const char * szCommand)
{
	if (GetHostPlayer()->GetPack()->SearchEmpty()>=0)
	{
		GetGameSession()->c2s_CmdUniqueBidGetAward();
	}
	else
		GetGameUIMan()->MessageBox(8702);
}

void CDlgAuction::OnEventLButtonUp(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	for (int i=0; i<6; ++i)
	{
		if (pObj==m_pBtnAdd[i]||pObj==m_pBtnMinus[i])
		{
			ChangeFocus(m_pEdtPrice[i]);
			return;
		}
	}
}

void CDlgAuction::OnEventLButtonDownPrice(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUILISTBOX pLstPlayer = (PAUILISTBOX)GetDlgItem("Lst_Player");
	pLstPlayer->ResetContent();
	PAUILISTBOX pLstPrice = (PAUILISTBOX)GetDlgItem("List_BuyHistory");
	if (pLstPrice->GetCurSel()<0||pLstPrice->GetCurSel()>=pLstPrice->GetCount())
	{
		return;
	}

	int money = pLstPrice->GetItemData(pLstPrice->GetCurSel());
	if (m_mapUniquePrice.find(money)==m_mapUniquePrice.end())
	{
		return;
	}


	std::vector<UniqueBidder>& vecBindders = m_mapUniquePrice[money].same_bidders;
	ACString strText;
	for (UINT i=0; i<vecBindders.size(); ++i)
	{
		ACString strName = ACString((const ACHAR*)vecBindders[i].rolename.begin(), vecBindders[i].rolename.size() / sizeof(ACHAR));
		//strText.Format(_AL("%d\t%s"), i, strName);
		pLstPlayer->AddString(strName);
		pLstPlayer->SetItemData(i, vecBindders[i].roleid);
	}
}

void CDlgAuction::OnEventRButtonUpPlayer(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	POINT ptPos = pObj->GetPos();
	PAUILISTBOX pLst = (PAUILISTBOX)pObj;
	int index = pLst->GetCurSel();
	A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();
	int x = GET_X_LPARAM(lParam) - ptPos.x - p->X;
	int y = GET_Y_LPARAM(lParam) - ptPos.y - p->Y;
	if (index>=0 && index<pLst->GetCount() &&
		pLst->GetHitArea(x, y) == AUILISTBOX_RECT_TEXT)
	{
		int idPlayer = pLst->GetItemData(index);
		
		x = GET_X_LPARAM(lParam) - p->X;
		y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->PopupPlayerContextMenu(idPlayer, x, y);
	}
}

void CDlgAuction::HistoryInfo(void* p)
{
	enum
	{
		STATUS_AUCTION	= 0x04, // 19:30 - 21:00
		STATUS_READY	= 0x08, // 19:30 - 20:00
	};
	GNET::UniqueBidHistory_Re* info = (GNET::UniqueBidHistory_Re*)p;
	bool bAuctionTime = (info->status & STATUS_AUCTION)>0; //与服务器约定的竞拍时间标志
	ACString strText;
	ACString strMoney;
	PAUIIMAGEPICTURE pImage = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Goods");

	// 设置悬浮
	if (bAuctionTime)
		GetGameUIMan()->SetIvtrInfo(pImage, info->item_tobid, 1, true);
	else
		pImage->SetHint(_AL(""));

	// 采用特定目录下图标
	pImage->ClearCover();
	AUIOBJECT_SETPROPERTY pif;
	sprintf(pif.fn, "ingame\\AuctionIcon\\%d.dds", bAuctionTime ? info->item_tobid : 0);
	pImage->SetProperty("image File", &pif);
	GetDlgItem("Btn_Award")->Enable(info->winner == GetHostPlayer()->GetCharacterID() && info->win_item!=0);

	PAUILABEL pLabBaseAward = (PAUILABEL)GetDlgItem("Txt_BasicPrice");
	m_siMoneyFloor = info->money_min;
	GetGameUIMan()->FormatMoney(info->money_min, strText);
	pLabBaseAward->SetText(bAuctionTime ? strText : _AL("?"));

	int i, j;

	std::vector<GNET::UniquePriceInfo>& selflist = info->self_bid_list;
	PAUILISTBOX pLst = (PAUILISTBOX)GetDlgItem("List_BuyHistory");
	pLst->ResetContent();
	PAUILABEL pLabNum = (PAUILABEL)GetDlgItem("Txt_Record");
	strText.Format(_AL("%d/%d"), selflist.size(), AUCTION_NUM_MAX);
	pLabNum->SetText(strText);
	m_mapUniquePrice.clear();
	for (i=0; i<(int)selflist.size(); i++)
	{
		int money = selflist[i].money;
		m_mapUniquePrice[money] = selflist[i];
		GetGameUIMan()->FormatMoney(money, strMoney);
		strText.Format(_AL("%d\t%s"), i+1, strMoney);
		pLst->AddString(strText);
		pLst->SetItemData(i, money);
		for (j=0; j<(int)selflist[i].same_bidders.size(); j++)
		{
			ACString strName = ACString((const ACHAR*)selflist[i].same_bidders[j].rolename.begin(), selflist[i].same_bidders[j].rolename.size() / sizeof(ACHAR));
			GetGameRun()->AddPlayerName(selflist[i].same_bidders[j].roleid, strName);
		}
	}

	OnEventLButtonDownPrice(0, 0, GetDlgItem("List_BuyHistory"));
	
#ifndef DEBUG_FOR_SERVER
	GetDlgItem("Btn_Buy")->Enable((info->status & STATUS_AUCTION)>0 && (info->status & STATUS_READY)==0);
	GetDlgItem("Btn_RangeBuy")->Enable((info->status & STATUS_AUCTION)>0 && (info->status & STATUS_READY)==0);
#else
	GetDlgItem("Btn_Buy")->Enable(true);
	GetDlgItem("Btn_RangeBuy")->Enable(true);	
#endif

	pLst = (PAUILISTBOX)GetDlgItem("Lst_Place");
	pLst->ResetContent();
	std::vector<UniqueWinner>& history = info->winner_list;
	i = history.size();
	i--;
	for (; i>=0; i--)
	{
		UniqueWinner item = history[i];
		long time = item.time;
		tm t = *gmtime((const time_t*)&time);
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(item.itemid, 0, 1);
		if (!pItem)
		{
			continue;
		}
		if (item.roleid!=0)
		{
			ACString strName((const ACHAR*)item.rolename.begin(), item.rolename.size() / sizeof (ACHAR));
			GetGameUIMan()->FormatMoney(item.money, strMoney);
			strText.Format(_AL("%d.%d\t%s\t%s\t%s"), t.tm_mon+1, t.tm_mday, pItem->GetName(), strName, strMoney);
		}
		else
			strText.Format(_AL("%d.%d\t%s\t--\t--"), t.tm_mon+1, t.tm_mday, pItem->GetName());
		pLst->AddString(strText);
		delete pItem;
	}

	if (!IsShow())
	{
		Show(true);
	}
}
