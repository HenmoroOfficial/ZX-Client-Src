#include "AFI.h"

#include "EC_GameUIMan.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_World.h"
#include "EC_Algorithm.h"

#include "AUI\\AUICommon.h"
#include "AUI\\AUIDef.h"
#include "AUI\\AUICTranslate.h"
#include "AUI\\AUISubDialog.h"
#include "AUI\\AUITreeView.h"
#include "aui\\CSplit.h"
#include "aui\\AUICheckBox.h"

#include "DlgGuildShop.h"
#include "Network\\ids.hxx"
#include "elementdataman.h"
#include "EC_TaskInterface.h"
#include "TaskProcess.h"
#include "TaskTempl.h"
#include "TaskTemplMan.h"
#include "EC_IvtrItem.h"

#include "DlgInputNO.h"
#include "DlgInfo.h"

AUI_BEGIN_COMMAND_MAP(DlgGuildSale, CDlgBase)
AUI_ON_COMMAND("Btn_Bid", OnCommand_Buy)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildSale, CDlgBase)
AUI_END_EVENT_MAP()

DlgGuildSale::DlgGuildSale()
{
	m_bIsInit = false;
}
DlgGuildSale::~DlgGuildSale()
{
}

bool DlgGuildSale::OnInitDialog()
{
	m_pImg_Achievement  = (AUIImagePicture*)GetDlgItem("Img_Achievement");
	m_pTxt_Buyer  = (AUILabel*)GetDlgItem("Txt_Buyer");
	m_pTxt_Price  = (AUILabel*)GetDlgItem("Txt_Price");
	m_pTxt_Time  = (AUILabel*)GetDlgItem("Txt_Time");
	m_pImg_Success  = (AUIImagePicture*)GetDlgItem("Img_Success");
	return true;
}

void DlgGuildSale::OnShowDialog()
{
	if(m_bIsInit == false)
	{
		Show(false);
	}
}
void DlgGuildSale::OnTick()
{
	if(m_nEndTime > GetGame()->GetServerGMTTime())
	{
		ACString szDayTime;
		ACString szHourTime;
		ACString szMinTime;
		ACString szSecTime;
		ACString szTextTime;
		int nCountTime =  m_nEndTime - GetGame()->GetServerGMTTime();
		if(nCountTime > (3600 * 24))				// 天
		{
			int time = (nCountTime % (3600 * 24))/3600;
			szDayTime.Format(GetStringFromTable(20237),nCountTime / (3600 * 24));
			szHourTime.Format(GetStringFromTable(20238),time);
			szTextTime=szDayTime + szHourTime;
		}
		else if(nCountTime > 3600)					// 小时
		{
			szHourTime.Format(GetStringFromTable(20238),nCountTime / 3600);
			szMinTime.Format(GetStringFromTable(20239), (nCountTime/60)%60);
			szTextTime=szHourTime + szMinTime;	
		}
		else
		{
			szHourTime.Format(GetStringFromTable(20239),nCountTime / 60);
			szSecTime.Format(GetStringFromTable(20240),nCountTime % 60);
			szTextTime= szHourTime + szSecTime;	
		}
		m_pTxt_Time->SetText(szTextTime);
	}
	else
	{
		m_pTxt_Time->SetText(_AL(""));
	}
}

void DlgGuildSale::UpSaleData(void* pData)
{
	m_bIsInit = true;
	DlgGuildShop::FacBaseAucItem* pInfo = (DlgGuildShop::FacBaseAucItem*)pData;
	GetGameUIMan()->SetIvtrInfo(m_pImg_Achievement,pInfo->itemid);
	m_nItemIndex = pInfo->idx;
	m_nItemId = pInfo->itemid;
	m_pTxt_Buyer->SetText(pInfo->szName);
	ACString szTemp;
	szTemp.Format(_AL("%d"),pInfo->cost);
	m_pTxt_Price->SetText(szTemp);
	m_nItemCost = pInfo->cost;
	ACString szDayTime;
	ACString szHourTime;
	ACString szMinTime;
	ACString szSecTime;
	ACString szAllTextTime;
	m_nEndTime = pInfo->end_time;
	m_pTxt_Time->SetText(szAllTextTime);
	m_pImg_Success->Show(false);
	GetDlgItem("Btn_Bid")->Enable(true);
	GetDlgItem("Btn_Bid")->SetText(GetStringFromTable(20247));
	GetDlgItem("Btn_Bid")->SetData(1);
	if(pInfo->status == 2)
	{
		m_pImg_Success->Show(true);
		if( GetGame()->GetGuildEnter() == true  && pInfo->winner == GetHostPlayer()->GetCharacterID())
		{
			GetDlgItem("Btn_Bid")->SetText(GetStringFromTable(20248));
			GetDlgItem("Btn_Bid")->Enable(true);
			GetDlgItem("Btn_Bid")->SetData(0);
		}
		else if(pInfo->winner == GetHostPlayer()->GetCharacterID())
		{
			GetDlgItem("Btn_Bid")->SetText(GetStringFromTable(20248));
			GetDlgItem("Btn_Bid")->Enable(false);
		}
		else 
		{
			GetDlgItem("Btn_Bid")->Enable(false);
		}
	}
}

void DlgGuildSale::OnCommand_Buy(const char * szCommand)
{
	if(GetDlgItem(szCommand)->GetData() == 1)
	{
		CDlgInputNO *pShow = GetGameUIMan()->m_pDlgInputNO;
		PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_No.");
		pEdit->SetDataPtr((void*)m_nItemCost);
		pEdit->SetData(GetHostPlayer()->GetFactionGoldNote());
		pShow->Show(true, true);
		pShow->SetType(CDlgInputNO::INPUTNO_GUILD_SHOP);
		pShow->SetDataPtr((void*)m_nItemId);
		pShow->SetData(m_nItemIndex);
	}
	else
	{
		c2s_SendCmdWithdrawFacBaseAuction(m_nItemIndex);
	}
}

AUI_BEGIN_COMMAND_MAP(DlgGuildQSale, CDlgBase)

AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildQSale, CDlgBase)
AUI_ON_EVENT("*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("*",					WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_ON_EVENT(NULL,					WM_MOUSEWHEEL,	OnEventMouseWheel)
AUI_END_EVENT_MAP()

DlgGuildQSale::DlgGuildQSale()
{

}
DlgGuildQSale::~DlgGuildQSale()
{
}

bool DlgGuildQSale::OnInitDialog()
{
	m_nItemIndex = 0;
	m_pImg_Achievement  = (AUIImagePicture*)GetDlgItem("Img_Achievement");
	m_pTxt_Name  = (AUILabel*)GetDlgItem("Txt_Name");
	m_pTxt_Price  = (AUILabel*)GetDlgItem("Txt_Price");
	m_pTxt_Time  = (AUILabel*)GetDlgItem("Txt_Time");
	m_pTxt_Intro  = (AUILabel*)GetDlgItem("Txt_Intro");
	m_pImg_Select = (AUIImagePicture*)GetDlgItem("Img_Choose");
	m_pImg_Cool = (AUIImagePicture*)GetDlgItem("Img_Cool");
	UpDateSelect(false);
	return true;
}

void DlgGuildQSale::OnTick()
{
	if(m_nTimeCooltime > GetGame()->GetServerGMTTime())
	{
		ACString szDayTime;
		ACString szHourTime;
		ACString szMinTime;
		ACString szSecTime;
		ACString szTextTime;
		int nCountTime =  m_nTimeCooltime - GetGame()->GetServerGMTTime();
		if(nCountTime > (3600 * 24))				// 天
		{
			szDayTime.Format(GetStringFromTable(20237),nCountTime / (3600 * 24));
			szHourTime.Format(GetStringFromTable(20238),(nCountTime % (3600 * 24))/3600);
			szTextTime.Format(GetStringFromTable(20236),szDayTime + szHourTime);
		}
		else if(nCountTime > 3600)					// 小时
		{
			szHourTime.Format(GetStringFromTable(20238),nCountTime / 3600);
			szMinTime.Format(GetStringFromTable(20239), (nCountTime/60)%60);
			szTextTime.Format(GetStringFromTable(20236),szHourTime + szMinTime);	
		}
		else
		{
			szHourTime.Format(GetStringFromTable(20239),nCountTime / 60);
			szSecTime.Format(GetStringFromTable(20240),nCountTime % 60);
			szTextTime.Format(GetStringFromTable(20236),szHourTime + szSecTime);	
		}
		m_pTxt_Time->SetText(szTextTime);
		m_pTxt_Time->Show(true);
		m_pImg_Cool->Show(true);
		GetDlgItem("Lab_Name")->SetColor(0xff2E5059);
		GetDlgItem("Lab_Price")->SetColor(0xff2E5059);
		GetDlgItem("Lab_Intro")->SetColor(0xff2E5059);
		m_pTxt_Name->SetColor(0xff49492C);
		m_pTxt_Price->SetColor(0xff49492C);
		m_pTxt_Intro->SetColor(0xff49492C);

	}
	else
	{
		DlgGuildQShop *pDlgGuildQShop = (DlgGuildQShop*)GetGameUIMan()->GetDialog("Win_Guild_Qshop");
		pDlgGuildQShop->ClearItemCooltime(m_nItemIndex);
		m_pTxt_Time->SetText(_AL(""));
		m_pTxt_Time->Show(false);
		m_pImg_Cool->Show(true);
		GetDlgItem("Lab_Name")->SetColor(0xff9bcef4);
		GetDlgItem("Lab_Price")->SetColor(0xff9bcef4);
		GetDlgItem("Lab_Intro")->SetColor(0xff9bcef4);
		m_pTxt_Name->SetColor(0xffffff80);
		m_pTxt_Price->SetColor(0xffffff80);
		m_pTxt_Intro->SetColor(0xffffff80);
	}
}

void DlgGuildQSale::UpQSaleData(void* pData)
{
	m_pTxt_Time->Show(false);
	DlgGuildQShop::QShopItems* pItem = (DlgGuildQShop::QShopItems*)pData;
	ACString szTemp;
	m_pTxt_Name->SetText(pItem->szName);
	szTemp.Format(_AL("%d"),pItem->nPrice);
	m_pTxt_Price->SetText(szTemp);
	m_pTxt_Intro->SetText(pItem->szDes);

	AString strFile;
	AUICTranslate trans;
	af_GetFileTitle(GetGame()->GetDataPath(pItem->nFile_Icon), strFile);
	m_pImg_Achievement->SetCover(GetGameUIMan()->GetIconSet(CECGameUIMan::ICONS_INVENTORY),
		GetGameUIMan()->GetIconId(CECGameUIMan::ICONS_INVENTORY,strFile));
	m_pImg_Achievement->SetCover(NULL, 0, 1);
	m_nItemIndex = pItem->nIndex;

	m_nTimeCooltime = pItem->nCurCoolTime;
	m_nTimeCount = pItem->nCooltime;
}

void DlgGuildQSale::UpDateSelect(bool isSelect)
{
	m_pImg_Select->Show(isSelect);
}


void DlgGuildQSale::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	DlgGuildQShop *pDlgGuildQShop = (DlgGuildQShop*)GetGameUIMan()->GetDialog("Win_Guild_Qshop");
	pDlgGuildQShop->SetCurItemIndex(this);
}

void DlgGuildQSale::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Info")->GetDlgItem("Sub_01");
	if(pSubD)
		pSubD->OnDlgItemMessage(WM_MOUSEWHEEL, wParam, lParam);
}




AUI_BEGIN_COMMAND_MAP(CDlgGuildShopSub, CDlgBase)
AUI_END_COMMAND_MAP()
CDlgGuildShopSub::CDlgGuildShopSub()
{

}
CDlgGuildShopSub::~CDlgGuildShopSub()
{
	m_vecDlgItems.clear();
	int nSize = m_vecSubDialogs.size();
	for (int i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
}
bool CDlgGuildShopSub::OnInitDialog()
{
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
	m_pSubItem = (PAUISUBDIALOG)GetDlgItem("Sub_1");
	m_rcWindowOld = GetGameUIMan()->GetRect();

	return true;
}
bool CDlgGuildShopSub::Release()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	//创建的界面uiman统一管理，不必单独destroy
	m_vecDlgItems.clear();

	return CDlgBase::Release();
}
bool CDlgGuildShopSub::Render()
{
	return AUIDialog::Render();
}

void CDlgGuildShopSub::InitItem()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	//先删掉控件，再删对应的dialog
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	nSize = m_vecDlgItems.size();
	for (i=0; i<nSize; i++)
	{
		GetGameUIMan()->DestroyDialog(m_vecDlgItems[i]->GetName());
	}
	m_vecDlgItems.clear();
	int y=0;
	A3DRECT	rcNew = GetGameUIMan()->GetRect();

	for (int i=0; i< 5; i++)
	{
		GetGameUIMan()->CreateDlg(GetGame()->GetA3DEngine(), GetGame()->GetA3DDevice(), "Guild_ShopItem01.xml");
		DlgGuildSale* pDlg = (DlgGuildSale*)GetGameUIMan()->GetDialog("Win_Guild_ShopItem01");
		AString strName;
		strName.Format("Win_GuildShopItem%d", i);
		pDlg->SetName(strName);
		pDlg->Resize(m_rcWindowOld, rcNew);
		pDlg->Show(false);
		m_vecDlgItems.push_back(pDlg);
		PAUISUBDIALOG pSubItem = (PAUISUBDIALOG)CloneControl(m_pSubItem);
		strName.Format("Sub_Clone%d", i);
		pSubItem->SetName(strName.GetBuffer(0));
		pSubItem->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);//pDlg->GetSize().cx,pDlg->GetSize().cy);
		pSubItem->SetDialog(pDlg);
		pSubItem->SetPos(0,y);
		m_vecSubDialogs.push_back(pSubItem);
		y += pSubItem->GetSize().cy;
	}
	int w = GetSize().cx;
	int h = y;
	SetSize(w, h);

	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Shop")->GetDlgItem("Sub_01");
	pSubD->SetSubDialogOrgSize(w, h);
}

void CDlgGuildShopSub::UpDateItems(void* pData, int nIndex)
{
	if((int)m_vecDlgItems.size() < nIndex)
	{
		return;
	}
	DlgGuildSale* pDlg = m_vecDlgItems[nIndex];
	if(pData == NULL)
	{
		pDlg->Show(false);
		return;
	}
	pDlg->UpSaleData(pData);
	pDlg->Show(true);
}

AUI_BEGIN_COMMAND_MAP(DlgGuildShop, CDlgBase)
AUI_ON_COMMAND("Btn_GetMoney", OnCommand_GetMoney)
AUI_ON_COMMAND("Btn_Prev01", OnCommand_SaleUp)
AUI_ON_COMMAND("Btn_Next01", OnCommand_SaleDown)
AUI_ON_COMMAND("Btn_Prev02", OnCommand_SwapUp)
AUI_ON_COMMAND("Btn_Next02", OnCommand_SwapDown)

AUI_ON_COMMAND("confirm", OnCommand_SwapOK)
AUI_ON_COMMAND("IDCANCEL", OnCommand_SwapCancel)
AUI_ON_COMMAND("max", OnCommand_Max)
AUI_ON_COMMAND("add", OnCommand_Add)
AUI_ON_COMMAND("minus", OnCommand_Reduce)

AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildShop, CDlgBase)
AUI_ON_EVENT("Item_*",					WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_ON_EVENT("NULL",				WM_LBUTTONDOWN,	OnEventLButtonDown)
AUI_END_EVENT_MAP()
DlgGuildShop::DlgGuildShop()
{
	m_nSaleCurPage = 1;
	m_nSaleMaxPage = 1;
	m_nSwapCurPage = 1;
	m_nSwapMaxPage = 1;
	m_nCurItemNum = 1;
	m_bIsLoadAuction =false;
}

DlgGuildShop::~DlgGuildShop()
{

}

bool DlgGuildShop::OnInitDialog()
{
	m_pTxt_Money = (AUILabel*)GetDlgItem("Txt_Money");
	m_pTxt_Num =  (AUILabel*)GetDlgItem("Txt_Num");
	m_pImg_ExchangeItem = (AUIImagePicture*)GetDlgItem("Img_ExchangeItem");
	m_pLst_History  = (PAUILISTBOX)GetDlgItem("Lst_History");
	m_pTxt_PageSale = (AUILabel*)GetDlgItem("Txt_Page01");
	m_pTxt_PageSwap = (AUILabel*)GetDlgItem("Txt_Page02");
	AString szTemp;
	for(int nIndex = 0; nIndex < 28; nIndex++)
	{
		szTemp.Format("Item_%d",nIndex + 1);
		m_pImg_Icon[nIndex] = (AUIImagePicture*)GetDlgItem(szTemp);
		szTemp.Format("Img_Lock%02d",nIndex + 1);
		m_pImg_IconLock[nIndex] = (AUIImagePicture*)GetDlgItem(szTemp);
	}
	DATA_TYPE DataType;
	FACTION_SHOP_CONFIG * m_pEssence = (FACTION_SHOP_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(845, ID_SPACE_CONFIG, DataType);

	for(int nIndex = 0; nIndex < 100; nIndex++)
	{
		if(m_pEssence->faction_item_list[nIndex].item_id != 0)
		{
			ActivedItems pItem;
			pItem.nIndex = m_pEssence->faction_item_list[nIndex].item_index;
			pItem.bIsActive = false;
			pItem.nItemId = m_pEssence->faction_item_list[nIndex].item_id;
			pItem.nContribution = m_pEssence->faction_item_list[nIndex].contribution;
			m_vecActivedItems.push_back(pItem);
		}
	}
	m_nSwapMaxPage = (int)m_vecActivedItems.size() / 28;
	if((int)m_vecActivedItems.size() % 28 > 0)
	{
		m_nSwapMaxPage += 1;
	}

	FACTION_AUCTION_CONFIG* pEssence = (FACTION_AUCTION_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(844, ID_SPACE_CONFIG, DataType);
	for(int nIndex = 0; nIndex < 100; nIndex++)
	{
		if(pEssence->faction_auction_list[nIndex].item_id != 0)
		{
			FactionAuctionItem pItem;
			pItem.item_id = pEssence->faction_auction_list[nIndex].item_id;
			pItem.file_icon = pEssence->faction_auction_list[nIndex].file_icon;
			pItem.words = pEssence->faction_auction_list[nIndex].words;
			m_mapAuctionItem[pItem.item_id] = pItem;
		}
	}
	UpDateReturnCoupon(0);
	return true;
}

void DlgGuildShop::OnShowDialog()
{
	CDlgGuildShopSub*pDlg = (CDlgGuildShopSub*)GetGameUIMan()->GetDialog("Win_Guild_Shop_Sub");
	pDlg->InitItem();
	if(m_bIsLoadAuction == false)
	{
		c2s_SendCmdGetFacBaseAuction();
	}
	else
	{
		UpDateSale();
	}
	c2s_SendCmdMallInfo();
	UpDateSwapItem(m_nSwapCurPage);
	GetGameUIMan()->SetIvtrInfo(m_pImg_ExchangeItem,0);
	m_pTxt_Num->SetText(_AL(""));
	m_pCurItem = NULL;
	UpDataSalePage();
	UpDataSwapPage();
	GetDlgItem("Btn_GetMoney")->Enable(false);
	if(GetGame()->GetGuildEnter())
	{
		GetDlgItem("Btn_GetMoney")->Enable(true);
	}

}

void DlgGuildShop::OnCommand_GetMoney(const char * szCommand)
{
	if(GetGame()->GetGuildEnter() && a_atoi(GetDlgItem("Txt_Money")->GetText()) > 0)
	{
		c2s_SendCmdWithdrawFacBaseCoupon();
	}
}

void DlgGuildShop::OnCommand_SaleUp(const char * szCommand)
{
	m_nSaleCurPage -= 1;
	if(m_nSaleCurPage < 1)
	{
		m_nSaleCurPage = 1;
		return;
	}
	UpDataSalePage();
	UpDateSale();
}
void DlgGuildShop::OnCommand_SaleDown(const char * szCommand)
{
	m_nSaleCurPage += 1;
	if(m_nSaleCurPage >= m_nSaleMaxPage)
	{
		m_nSaleCurPage = m_nSaleMaxPage;
	}
	UpDataSalePage();
	UpDateSale();
}

void DlgGuildShop::OnCommand_SwapUp(const char * szCommand)
{
	m_nSwapCurPage -= 1;
	if(m_nSwapCurPage < 1)
	{
		m_nSwapCurPage = 1;
	}
	c2s_SendCmdMallInfo();
}
void DlgGuildShop::OnCommand_SwapDown(const char * szCommand)
{
	m_nSwapCurPage += 1;
	if(m_nSwapCurPage >= m_nSwapMaxPage)
	{
		m_nSwapCurPage = m_nSwapMaxPage;
	}
	c2s_SendCmdMallInfo();
}

void DlgGuildShop::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject * pObj)
{
	DlgGuildShop::ActivedItems* pItem = (DlgGuildShop::ActivedItems* )pObj->GetDataPtr();
	if(pItem && pItem->bIsActive == true)
	{
		m_pCurItem = pItem;
		GetGameUIMan()->SetIvtrInfo(m_pImg_ExchangeItem,pItem->nItemId);
		ACString szTemp;
		szTemp.Format(_AL("%d"),pItem->nContribution);
		m_pTxt_Num->SetText(szTemp);
		szTemp.Format(_AL("%d"),1);
		GetDlgItem("Txt_Amount")->SetText(szTemp);
	}
}

void DlgGuildShop::OnCommand_SwapOK(const char * szCommand)
{
	if(m_pCurItem)
	{
		m_nCurItemNum = a_atoi(GetDlgItem("Txt_Amount")->GetText());
		if(GetHostPlayer()->GetFactionGoldNote() >= m_pCurItem->nContribution * m_nCurItemNum)
		{
			c2s_SendCmdShopMallInfo(m_pCurItem->nIndex,m_pCurItem->nItemId,m_nCurItemNum);
		}
		else
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20222), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		}
	}
}
void DlgGuildShop::OnCommand_SwapCancel(const char * szCommand)
{
	GetGameUIMan()->SetIvtrInfo(m_pImg_ExchangeItem,0);
	m_pTxt_Num->SetText(_AL(""));
	m_pCurItem = NULL;
	m_nCurItemNum = 1;
	ACString szTemp;
	szTemp.Format(_AL("%d"),m_nCurItemNum);
	GetDlgItem("Txt_Amount")->SetText(szTemp);
}

void DlgGuildShop::OnCommand_Max(const char * szCommand)
{
	if(m_pCurItem == NULL || GetHostPlayer()->GetFactionGoldNote() == 0)
	{
		return;
	}
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pCurItem->nItemId, 0, 1);
	if(pItem)
	{
		m_nCurItemNum = GetHostPlayer()->GetFactionGoldNote() / m_pCurItem->nContribution;
		ACString szTemp;
		szTemp.Format(_AL("%d"),m_nCurItemNum);
		GetDlgItem("Txt_Amount")->SetText(szTemp);
		delete pItem;
	}
}

void DlgGuildShop::OnCommand_Add(const char * szCommand)
{
	if(m_pCurItem == NULL || GetHostPlayer()->GetFactionGoldNote() == 0)
	{
		return;
	}
	m_nCurItemNum +=1;
	if(m_pCurItem)
	{
		CECIvtrItem* pItem = CECIvtrItem::CreateItem(m_pCurItem->nItemId, 0, 1);
		if(m_nCurItemNum >= GetHostPlayer()->GetFactionGoldNote() / m_pCurItem->nContribution)
		{
			m_nCurItemNum =GetHostPlayer()->GetFactionGoldNote() / m_pCurItem->nContribution;
		}
		delete pItem;
	}
	ACString szTemp;
	szTemp.Format(_AL("%d"),m_nCurItemNum);
	GetDlgItem("Txt_Amount")->SetText(szTemp);
}

void DlgGuildShop::OnCommand_Reduce(const char * szCommand)
{
	if(m_pCurItem == NULL)
	{
		return;
	}
	m_nCurItemNum -=1;
	if(m_nCurItemNum <= 1)
	{
		m_nCurItemNum = 1;
	}
	ACString szTemp;
	szTemp.Format(_AL("%d"),m_nCurItemNum);
	GetDlgItem("Txt_Amount")->SetText(szTemp);
}

void DlgGuildShop::UpDataSalePage()
{
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nSaleCurPage,m_nSaleMaxPage);
	m_pTxt_PageSale->SetText(szTemp);
}

void DlgGuildShop::UpDataSwapPage()
{
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nSwapCurPage,m_nSwapMaxPage);
	m_pTxt_PageSwap->SetText(szTemp);
}


void DlgGuildShop::UpDateSwapItem(int nPage) 
{
	for(int nIndex = 0; nIndex < 28; nIndex++)
	{
		DlgGuildShop::ActivedItems* pItem = CheckItemActived(nIndex +(m_nSwapCurPage - 1) * 28);
		if(pItem)
		{
			CECIvtrItem* pItems = CECIvtrItem::CreateItem(pItem->nItemId, 0,1);
			AString strFile;
			if (pItems )
			{
				pItems->SetNeedUpdate(false);
				GetGameUIMan()->SetIvtrInfo(m_pImg_Icon[nIndex], pItems);
				delete pItems;
				m_pImg_Icon[nIndex]->SetDataPtr((void*)pItem);
			}
		}
	}
}

void DlgGuildShop::UpDataLockSwapItem(void* pData)
{
	using namespace S2C;
	for(int nIndex = 0; nIndex < (int)m_vecActivedItems.size();nIndex++)
	{
		DlgGuildShop::ActivedItems* pItem = &m_vecActivedItems[nIndex];
		pItem->bIsActive = false;
	}
	facbase_mall_item_info* pCmd = (facbase_mall_item_info*)pData;
	for(int nIndex = 0; nIndex < pCmd->items_cnt; nIndex++)
	{
		for(int nIndex = 0; nIndex < (int)m_vecActivedItems.size();nIndex++)
		{
			DlgGuildShop::ActivedItems* pItem = &m_vecActivedItems[nIndex];
			if(pCmd->actived_items[nIndex] == pItem->nIndex)
			{
				pItem->bIsActive = true;
			}
		}
	}
	UpdateItemLock();
	UpDateSwapItem(m_nSwapCurPage);
}

void DlgGuildShop::UpdateItemLock() 
{
	for(int nIndex = 0; nIndex < 28;nIndex++)
	{
		DlgGuildShop::ActivedItems* pItem = CheckItemActived(nIndex +(m_nSwapCurPage - 1) * 28);
		if(pItem)
		{
			m_pImg_IconLock[nIndex]->Show(!pItem->bIsActive);
		}
	}
}

DlgGuildShop::ActivedItems*  DlgGuildShop::CheckItemActived(int nActivedIndex)
{
	for(int nIndex = 0; nIndex < (int)m_vecActivedItems.size();nIndex++)
	{
		if(nActivedIndex == nIndex)
		{
			return &m_vecActivedItems[nIndex];;
		}
	}
	return NULL;
}

DlgGuildShop::FacBaseAucItem* DlgGuildShop::GetFacBaseAucItem(int nIndex)
{
	for(int nItemIndex = 0; nItemIndex < (int)m_vecAucItems.size(); nItemIndex++)
	{
		DlgGuildShop::FacBaseAucItem* pItem = &m_vecAucItems[nItemIndex];
		if(nIndex == pItem->idx)
		{
			return pItem;
		}
	}
	return NULL;
}
void DlgGuildShop::UpDateReturnCoupon(int nCoupon) 
{
	ACString szTemp;
	szTemp.Format(_AL("%d"),nCoupon);
	GetDlgItem("Txt_Money")->SetText(szTemp);
}

void DlgGuildShop::UpDateSaleItem(void* pData)
{
	using namespace S2C;
	player_facbase_auction*  pInfo = (player_facbase_auction*)pData;
	UpDateReturnCoupon(pInfo->return_coupon);
	UpDateHistory(pInfo);
	if(m_bIsLoadAuction == false)
	{
		m_bIsLoadAuction = true;
		m_vecAucItems.clear();
		m_nSaleMaxPage = pInfo->size / 5;
		if( pInfo->size % 5 > 0)
		{
			m_nSaleMaxPage += 1;
		}
		if(m_nSaleMaxPage == 0)
		{
			m_nSaleMaxPage = 1;
		}
		for(int nIndex = 0; nIndex < pInfo->size; nIndex++)
		{
			DlgGuildShop::FacBaseAucItem pItem;
			pItem.idx = pInfo->listItem[nIndex].idx;
			pItem.itemid = pInfo->listItem[nIndex].itemid;
			pItem.winner = pInfo->listItem[nIndex].winner;
			pItem.cost = pInfo->listItem[nIndex].cost;
			pItem.status = pInfo->listItem[nIndex].status;
			pItem.end_time = pInfo->listItem[nIndex].end_time;
			pItem.szName = ACString((ACHAR*)pInfo->listItem[nIndex].playername, pInfo->listItem[nIndex].name_len/sizeof(ACHAR));

			m_vecAucItems.push_back(pItem);
		}
	}
	BubbleSort(m_vecAucItems.begin(), m_vecAucItems.end(), SortShopAucItem(GetHostPlayer()->GetName(),GetGame()->GetServerGMTTime()));
	UpDateSale();
	UpDataSalePage();
}

void DlgGuildShop::UpDateSale()
{
	CDlgGuildShopSub*pDlg = (CDlgGuildShopSub*)GetGameUIMan()->GetDialog("Win_Guild_Shop_Sub");
	int nSize = m_vecAucItems.size();
	for(int nIndex  = 0; nIndex < 5; nIndex++)
	{
		int nCur = nIndex + 5 * (m_nSaleCurPage - 1);
		if(nCur >= nSize)
		{
			pDlg->UpDateItems(NULL,nIndex);
		}
		else
		{
			DlgGuildShop::FacBaseAucItem* pItem = &m_vecAucItems[nCur];
			pDlg->UpDateItems(pItem,nIndex);
		}
	}
}

void DlgGuildShop::UpDateSaleElement(void* pData)
{
	using namespace S2C;
	facbase_auction_update* pCmd = (facbase_auction_update*)pData;
	if(DlgGuildShop::ADD == pCmd->type)
	{
		DlgGuildShop::FacBaseAucItem pItem;
		pItem.idx = pCmd->listItem.idx;
		pItem.itemid = pCmd->listItem.itemid;
		pItem.winner = pCmd->listItem.winner;
		pItem.cost =pCmd->listItem.cost;
		pItem.status = pCmd->listItem.status;
		pItem.end_time = pCmd->listItem.end_time;
		pItem.szName = ACString((ACHAR*)pCmd->listItem.playername, pCmd->listItem.name_len/sizeof(ACHAR));

		m_vecAucItems.push_back(pItem);
		m_nSaleMaxPage = m_vecAucItems.size() / 5;
		if(m_vecAucItems.size() % 5 > 0)
		{
			m_nSaleMaxPage += 1;
		}
	}
	else if(DlgGuildShop::END == pCmd->type)
	{
		if(GetHostPlayer()->GetCharacterID() == pCmd->listItem.winner)
		{
			DlgGuildShop::FacBaseAucItem* pItem = GetFacBaseAucItem(pCmd->listItem.idx);
			if(pItem)
			{
				if(pCmd->listItem.winner == GetHostPlayer()->GetCharacterID())
				{
					GetGameUIMan()->AddInformation(CDlgInfo::INFO_SYSTEM, "",
						GetStringFromTable(20243), 30000, 0, 0, 0);
				}


				// 提示自己  玩家拍卖成功，可以进入基地领取奖品
				pItem->winner = pCmd->listItem.winner;
				pItem->cost =pCmd->listItem.cost;
				pItem->status = pCmd->listItem.status;
				pItem->end_time = pCmd->listItem.end_time;
				pItem->szName =  ACString((ACHAR*)pCmd->listItem.playername,pCmd->listItem.name_len/sizeof(ACHAR));
			}
		}
	}
	else if(DlgGuildShop::DEL == pCmd->type)
	{
		DlgGuildShop::FacBaseAucItem* pItem = GetFacBaseAucItem(pCmd->listItem.idx);
		if(pItem)
		{
			m_vecAucItems.erase(pItem);
			m_nSaleMaxPage = m_vecAucItems.size() / 5;
			if(m_vecAucItems.size() % 5 > 0)
			{
				m_nSaleMaxPage += 1;
			}
			m_nSaleCurPage = 1;
		}
	}
	else if(DlgGuildShop::REFRESH == pCmd->type)
	{
		DlgGuildShop::FacBaseAucItem* pItem = GetFacBaseAucItem(pCmd->listItem.idx);
		if(pItem)
		{
			if(pItem->winner == GetHostPlayer()->GetCharacterID())
			{
				int nCost =  pItem->cost;
				ACString szTemp;
				szTemp.Format(GetStringFromTable(20242),nCost);
				GetGameUIMan()->AddChatMessage(szTemp, GP_CHAT_SYSTEM);
			}
			pItem->szName =  ACString((ACHAR*)pCmd->listItem.playername, pCmd->listItem.name_len/sizeof(ACHAR));
			pItem->winner = pCmd->listItem.winner;
			pItem->cost =pCmd->listItem.cost;
			pItem->status = pCmd->listItem.status;
			pItem->end_time = pCmd->listItem.end_time;
		}
	}
	BubbleSort(m_vecAucItems.begin(), m_vecAucItems.end(), SortShopAucItem(GetHostPlayer()->GetName(),GetGame()->GetServerGMTTime()));
	if(IsShow() == true)
	{
		UpDateSale();
		UpDataSalePage();
	}
}

void DlgGuildShop::UpDateHistory(void* pData)
{
	using namespace S2C;
	player_facbase_auction*  pInfo = (player_facbase_auction*)pData;
	for(int nIndex = 0; nIndex < pInfo->historysize; nIndex++)
	{
		fac_base_auc_history* pElement = &pInfo->phistoryList[nIndex];
		AddHistory(pElement);
	}
}

void DlgGuildShop::AddHistory( void*  pElement) 
{
	using namespace S2C;
	fac_base_auc_history* pInfo =(fac_base_auc_history*) pElement;
	ACString szTemp ;
	ACString rolename =  ACString((ACHAR*)pInfo->playername, pInfo->name_len/sizeof(ACHAR));
	DlgGuildShop::FactionAuctionItem* pItem = GetFactionAuctionItem(pInfo->itemid);
	if(pItem)
	{
		szTemp = pItem->words;
		m_pLst_History->InsertString(m_pLst_History->GetCount(),szTemp);
	}	
}


DlgGuildShop::FactionAuctionItem* DlgGuildShop::GetFactionAuctionItem(int nItemIndex)
{
	mapAuctionItems::iterator pIter = m_mapAuctionItem.find(nItemIndex);
	if(pIter != m_mapAuctionItem.end())
	{
		return &pIter->second;
	}
	return NULL;
}


AUI_BEGIN_COMMAND_MAP(CDlgGuildQShopSub, CDlgBase)
AUI_END_COMMAND_MAP()
CDlgGuildQShopSub::CDlgGuildQShopSub()
{

}
CDlgGuildQShopSub::~CDlgGuildQShopSub()
{
	m_vecDlgItems.clear();
	int nSize = m_vecSubDialogs.size();
	for (int i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
}
bool CDlgGuildQShopSub::OnInitDialog()
{
	m_vecDlgItems.clear();
	m_vecSubDialogs.clear();
	m_pSubItem = (PAUISUBDIALOG)GetDlgItem("Sub_1");
	m_rcWindowOld = GetGameUIMan()->GetRect();

	return true;
}
bool CDlgGuildQShopSub::Release()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	//创建的界面uiman统一管理，不必单独destroy
	m_vecDlgItems.clear();

	return CDlgBase::Release();
}
bool CDlgGuildQShopSub::Render()
{
	return AUIDialog::Render();
}

void CDlgGuildQShopSub::InitItem()
{
	int i;
	int nSize = m_vecSubDialogs.size();
	//先删掉控件，再删对应的dialog
	for (i=0; i<nSize; i++)
	{
		DeleteControl(m_vecSubDialogs[i]);
	}
	m_vecSubDialogs.clear();
	nSize = m_vecDlgItems.size();
	for (i=0; i<nSize; i++)
	{
		GetGameUIMan()->DestroyDialog(m_vecDlgItems[i]->GetName());
	}
	m_vecDlgItems.clear();
	int y=0;
	A3DRECT	rcNew = GetGameUIMan()->GetRect();

	for (int i=0; i< 4; i++)
	{
		GetGameUIMan()->CreateDlg(GetGame()->GetA3DEngine(), GetGame()->GetA3DDevice(), "Guild_ShopItem06.xml");
		DlgGuildQSale* pDlg = (DlgGuildQSale*)GetGameUIMan()->GetDialog("Win_Guild_ShopItem06");
		AString strName;
		strName.Format("Win_Guild_ShopItemQ%d", i);
		pDlg->SetName(strName);
		pDlg->Resize(m_rcWindowOld, rcNew);
		m_vecDlgItems.push_back(pDlg);
		PAUISUBDIALOG pSubItem = (PAUISUBDIALOG)CloneControl(m_pSubItem);
		strName.Format("Sub_Clone%d", i);
		pSubItem->SetName(strName.GetBuffer(0));
		pSubItem->SetSize(m_pSubItem->GetSize().cx, m_pSubItem->GetSize().cy);//pDlg->GetSize().cx,pDlg->GetSize().cy);
		pSubItem->SetDialog(pDlg);
		pSubItem->SetPos(0,y);
		m_vecSubDialogs.push_back(pSubItem);
		y += pSubItem->GetSize().cy;
	}
	int w = GetSize().cx;
	int h = y;
	SetSize(w, h);

	PAUISUBDIALOG pSubD = (PAUISUBDIALOG)GetGameUIMan()->GetDialog("Win_Guild_Qshop")->GetDlgItem("Sub_01");
	pSubD->SetSubDialogOrgSize(w, h);
}

void CDlgGuildQShopSub::UpDateItems(void* pData, int nIndex)
{
	if((int)m_vecDlgItems.size() < nIndex)
	{
		return;
	}
	DlgGuildQSale* pDlg = m_vecDlgItems[nIndex];
	if(pData == NULL)
	{
		pDlg->Show(false);
		return;
	}
	pDlg->UpQSaleData(pData);
	pDlg->Show(true);
}

AUI_BEGIN_COMMAND_MAP(DlgGuildQShop, CDlgBase)
AUI_ON_COMMAND("Btn_Prev", OnCommand_Up)
AUI_ON_COMMAND("Btn_Next", OnCommand_Down)
AUI_ON_COMMAND("Btn_Buy", OnCommand_Buy)
AUI_END_COMMAND_MAP()
AUI_BEGIN_EVENT_MAP(DlgGuildQShop, CDlgBase)
AUI_END_EVENT_MAP()
DlgGuildQShop::DlgGuildQShop()
{

}
DlgGuildQShop::~DlgGuildQShop()
{

}
bool DlgGuildQShop::OnInitDialog()
{
	m_pCurSelect = NULL;
	m_pTxt_Num = (AUILabel*)GetDlgItem("Txt_Page");						// 页码
	m_nCurPage = 1;
	m_nMaxPage = 1;
	DATA_TYPE DataType;
	FACTION_MONEY_SHOP_CONFIG * m_pEssence = (FACTION_MONEY_SHOP_CONFIG *)GetGame()->GetElementDataMan()->
		get_data_ptr(846, ID_SPACE_CONFIG, DataType);
	for(int nIndex = 0; nIndex < 100; nIndex++)
	{
		DlgGuildQShop::QShopItems pItem;
		pItem.szName = m_pEssence->faction_money_item_list[nIndex].item_name;
		if(!pItem.szName.IsEmpty())
		{
			pItem.nIndex = m_pEssence->faction_money_item_list[nIndex].item_index;
			pItem.nPrice = m_pEssence->faction_money_item_list[nIndex].price;
			pItem.nFile_Icon = m_pEssence->faction_money_item_list[nIndex].file_icon;
			pItem.szDes = m_pEssence->faction_money_item_list[nIndex].itemdes;
			pItem.nCooltime = m_pEssence->faction_money_item_list[nIndex].cooltime;
			pItem.szWord = m_pEssence->faction_money_item_list[nIndex].words;
			pItem.nCurCoolTime = 0;
			m_vecShopItem.push_back(pItem);
		}
	}
	m_nMaxPage = m_vecShopItem.size() / 4;
	if(m_vecShopItem.size() % 4 > 0)
	{
		m_nMaxPage += 1;
	}
	return true;
}

void DlgGuildQShop::OnShowDialog()
{
	if (GetHostPlayer()->GetFRoleID() != GNET::_R_MASTER
		&& GetHostPlayer()->GetFRoleID() != GNET::_R_VICEMASTER)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(20244), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		Show(false);
		return;
	}
	CDlgGuildQShopSub*pDlg = (CDlgGuildQShopSub*)GetGameUIMan()->GetDialog("Win_Guild_Qshop_Sub");
	pDlg->InitItem();
	m_nCurPage = 1;
	CheckQShopItem();
	c2s_SendCmdFacbaseMoneyItemInfo();
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nCurPage,m_nMaxPage);
	m_pTxt_Num->SetText(szTemp);
}

void DlgGuildQShop::OnHideDialog()
{
	GetGameUIMan()->EndNPCService();
}



void DlgGuildQShop::CheckQShopItem() 
{
	if(m_pCurSelect)
	{
		m_pCurSelect->UpDateSelect(false);
	}
	CDlgGuildQShopSub*pDlg = (CDlgGuildQShopSub*)GetGameUIMan()->GetDialog("Win_Guild_Qshop_Sub");
	for(int nIndex  = 0; nIndex < 4; nIndex++)
	{
		int nCur = nIndex + 4 * (m_nCurPage - 1);
		if(nCur >= (int)m_vecShopItem.size())
		{
			pDlg->UpDateItems(NULL,nIndex);
		}
		else
		{
			DlgGuildQShop::QShopItems* pItem = &m_vecShopItem[nCur];
			pDlg->UpDateItems(pItem,nIndex);
		}
	}
}
void DlgGuildQShop::OnCommand_Up(const char * szCommand)
{
	m_nCurPage -=1;
	if(m_nCurPage <= 1)
	{
		m_nCurPage = 1;
	}
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nCurPage,m_nMaxPage);
	m_pTxt_Num->SetText(szTemp);
	CheckQShopItem();
}

void DlgGuildQShop::OnCommand_Down(const char * szCommand)
{
	m_nCurPage +=1;

	if(m_nCurPage >= m_nMaxPage)
	{
		m_nCurPage = m_nMaxPage;
	}
	ACString szTemp;
	szTemp.Format(_AL("%d / %d"),m_nCurPage,m_nMaxPage);
	m_pTxt_Num->SetText(szTemp);
	CheckQShopItem();
}

void DlgGuildQShop::OnCommand_Buy(const char * szCommand)
{
	for(int nItemIndex = 0; nItemIndex < (int)m_vecShopItem.size(); nItemIndex++)
	{
		QShopItems* pItem = &m_vecShopItem[nItemIndex];
		if(pItem->nIndex == GetCurItemIndex() && pItem->nCurCoolTime > 0)
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(20235), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}
	c2s_SendCmdBuyFacbaseCashItem(GetCurItemIndex());
}

void  DlgGuildQShop::UpDataBuyItemInfo(void* pData)
{
	using namespace S2C;
	for(int nItemIndex = 0; nItemIndex < (int)m_vecShopItem.size(); nItemIndex++)
	{
		QShopItems* pItem = &m_vecShopItem[nItemIndex];
		pItem->nCurCoolTime = 0;
	}
	get_facbase_money_items_info* pInfo = (get_facbase_money_items_info*)pData;
	for(int nIndex = 0; nIndex < pInfo->items_cnt; nIndex++)
	{
		for(int nItemIndex = 0; nItemIndex < (int)m_vecShopItem.size(); nItemIndex++)
		{
			QShopItems* pItem = &m_vecShopItem[nItemIndex];
			if(pItem->nIndex == pInfo->item_cooldown[nIndex].idx)
			{
				pItem->nCurCoolTime = pInfo->item_cooldown[nIndex].cooldown;
			}
		}
	}
	CheckQShopItem();
}

void DlgGuildQShop::SetCurItemIndex(DlgGuildQSale* val) 
{
	if(m_pCurSelect)
	{
		m_pCurSelect->UpDateSelect(false);
	}
	m_pCurSelect = val; 
	m_pCurSelect->UpDateSelect(true);
}

int  DlgGuildQShop::GetCurItemIndex()
{
	if(m_pCurSelect)
	{
		return m_pCurSelect->m_nItemIndex;
	}
	return 0;
}
void DlgGuildQShop::UpDateBuyItem(int nIndex)
{
	for(int nItemIndex = 0; nItemIndex < (int)m_vecShopItem.size(); nItemIndex++)
	{
		QShopItems* pItem = &m_vecShopItem[nItemIndex];
		if(pItem->nIndex == nIndex)
		{
			GetGameUIMan()->AddChatMessage(pItem->szWord, GP_CHAT_FACTION);
		}
	}
}
void  DlgGuildQShop::ClearItemCooltime(int nIndex)
{
	for(int nItemIndex = 0; nItemIndex < (int)m_vecShopItem.size(); nItemIndex++)
	{
		QShopItems* pItem = &m_vecShopItem[nItemIndex];
		if(pItem->nIndex == nIndex)
		{
			pItem->nCurCoolTime = 0;
		}
	}
}