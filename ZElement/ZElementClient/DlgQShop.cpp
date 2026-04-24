// Filename	: DlgQShop.cpp
// Creator	: Tom Zhou
// Date		: 2006/5/22

#include "AIniFile.h"
#include "elementdataman.h"
#include "DlgQShopItem.h"
#include "DlgQShop.h"
#include "DlgWebShop.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDef.h"
#include "EC_Configs.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrEquip.h"
#include "EC_HostPlayer.h"
#include "EC_LoginPlayer.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameSession.h"
#include "EC_World.h"
#include "EC_Inventory.h"
#include "roleinfo"
#include "EC_Model.h"
#include "globaldataman.h"
#include "LuaEvent.h"
#include "EC_SendC2SCmds.h"
#include "DlgFastpay.h"
#include "AUI\\CSplit.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgQShop, CDlgBase)

AUI_ON_COMMAND("top",				OnCommand_Top)
AUI_ON_COMMAND("bottom",			OnCommand_Bottom)
AUI_ON_COMMAND("prec",				OnCommand_Prec)
AUI_ON_COMMAND("next",				OnCommand_Next)
AUI_ON_COMMAND("Btn_LotteryPrev",	OnCommand_PresentPrev)
AUI_ON_COMMAND("Btn_LotteryNext",	OnCommand_PresentNext)
AUI_ON_COMMAND("maintype*",			OnCommand_MainType)
AUI_ON_COMMAND("subtype*",			OnCommand_SubType)
AUI_ON_COMMAND("resumeequip",		OnCommand_ResumeEquip)
AUI_ON_COMMAND("payment",			OnCommand_Payment)
AUI_ON_COMMAND("qshop",				OnCommand_ChangeShop)
AUI_ON_COMMAND("popshop",			OnCommand_ChangeShop)
AUI_ON_COMMAND("help",				OnCommand_Help)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_QSBStipple_Quick",	OnCommand_FastPay)
AUI_ON_COMMAND("Btn_ConsumeScore",	OnCommand_GetGift)
AUI_ON_COMMAND("Btn_Search",	OnCommand_Search)
AUI_ON_COMMAND("Edt_Search",	OnCommand_Edit)
AUI_ON_COMMAND("Btn_Webshop",   OnCommand_WebShop)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgQShop, CDlgBase)

AUI_ON_EVENT("*",		WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,		WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Lst_Search",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_END_EVENT_MAP()

int					CDlgQShop::m_iQshopType = 0;
const int			CDlgQShop::ms_iCrossServerItemId = 41511;
abase::hash_map<int, int>	CDlgQShop::m_mapIndexRemainTime;
bool CDlgQShop::ms_bTestServer = false;

CDlgQShop::CDlgQShop() : m_bGetSaleTime(false)
{
	m_pPlayer = NULL;
	memset(m_aEquips, 0, sizeof(m_aEquips));
}

CDlgQShop::~CDlgQShop()
{
}

bool CDlgQShop::OnInitDialog()
{
	UINT i;
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
	m_pTxt_Cash = (PAUILABEL)GetDlgItem("Txt_Cash");
	m_pLab_Title = (PAUILABEL)GetDlgItem("Title");
	m_pLab_ConsumeScore = (PAUILABEL)GetDlgItem("Txt_ConsumeScore");
	m_pImg_Char = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Part");
	m_pBtn_LeftTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_LeftTurn");
	m_pBtn_RightTurn = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_RightTurn");
	for(i = 0; i < CDLGQSHOP_BUTTONMAX; i++)
	{
		char szName[20];
		sprintf(szName, "Btn_MainType%d", i);
		m_pBtn_MainType[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szName);
		sprintf(szName, "Btn_SubType%d", i);
		m_pBtn_SubType[i] = (PAUISTILLIMAGEBUTTON)GetDlgItem(szName);
	}
	
	assert(!m_pPlayer);
	ReLoadModel();
	
	//GetDlgItem("Btn_QSBBuy")->Show(false);
	m_iQshopType = 0;
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->SetPushed(true);
	GetDlgItem("Img_SubType0")->Show(false);
	GetDlgItem("Img_SubType1")->Show(false);
	GetDlgItem("Img_MainType0")->Show(false);
	m_nMainType = 0;
	m_nSubType = 0;
	m_bGetName = false;

	m_bKeepSearchResultOnce = false;
	
	if (m_szName=="Win_QShop")
	{
		bool bDisableConsumeScore = GetGame()->GetConfigs()->IsConsumeScoreDisable();
		GetDlgItem("Lab_ConsumeScore")->Show(!bDisableConsumeScore);
		GetDlgItem("Txt_ConsumeScore")->Show(!bDisableConsumeScore);
		GetDlgItem("Btn_ConsumeScore")->Show(!bDisableConsumeScore);
	}
	return true;
}

bool CDlgQShop::Release()
{
	UINT nSize = m_vecPresentItems.size();
	for(UINT i=0; i<nSize; i++)
	{
		GSHOP_ITEM* pItem = m_vecPresentItems[i];
		SAFE_DELETE(pItem);
	}
	m_vecPresentItems.clear();
	A3DRELEASE(m_pPlayer);
	return CDlgBase::Release();
}

void CDlgQShop::OnShowDialog()
{
	vector<GSHOP_MAIN_TYPE> *maintype;
	if (SHOP_TYPE_Q==m_iQshopType)
	{
		maintype = globaldata_getgshopmaintypes();
		GetDlgItem("Btn_Webshop")->Show(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->SetPushed(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->SetPushed(false);
		 ((PAUIEDITBOX)GetDlgItem("Edt_Search"))->SetText(_AL(""));
		m_strSearch = _AL("");	
		abase::vector<GSHOP_ITEM> *items;
		items = globaldata_getgshopitems();
		vector<GSHOP_MAIN_TYPE> *maintype;
		maintype = globaldata_getgshopmaintypes();
		if(!m_bGetName)
		{
			for(int i = 0; i < (int)items->size(); i++)
			{
				CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[i].id, 0, (*items)[i].num);
				ACString strName = _AL(""), strLabel = _AL("");
				if(pItem)
				{
					strName = pItem->GetName();
					delete pItem;
				}
				if((*items)[i].main_type < maintype->size() && (*items)[i].sub_type < (*maintype)[m_nMainType].sub_types.size())
					strLabel = (*maintype)[(*items)[i].main_type].sub_types[(*items)[i].sub_type];

				ACString strKeyWord; 
				strKeyWord = (wchar_t*)(*items)[i].SearchKey;
				strKeyWord = strName + _AL(",") + strLabel + _AL(",") + strKeyWord;
				CSplit s = strKeyWord;
				CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
				bool bMatch;
				for(int k = 0; k < vec.size(); k++)
				{
					int a = vec.size();
					bMatch = false;
					for(int j = 0; j < m_vecSearchWord.size(); j++)
					{
						if(m_vecSearchWord[j].Compare(vec[k]) == 0)
						{
							bMatch = true;
							break;
						}
					}
					if(!bMatch)
						m_vecSearchWord.push_back(vec[k]);
				}

			}
			m_bGetName = true;
		}
	}
	else if (SHOP_TYPE_POP==m_iQshopType)
	{
		maintype = globaldata_getgshopmaintypes2();
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->Show(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->Show(true);
		GetDlgItem("Btn_Webshop")->Show(true);
		GetDlgItem("Btn_QshopHelp")->Show(true);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->SetPushed(false);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->SetPushed(true);
		m_pLab_Title->SetText(GetStringFromTable(7399));
	}
	else if (SHOP_TYPE_CROSSSERVER==m_iQshopType)
	{
		if (m_szName!="Win_QShopPop")
		{
			Show(false);
			GetGameUIMan()->GetDialog("Win_QShopPop")->Show(true);
			return;
		}
		maintype = globaldata_getgshopmaintypes3();
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->Show(false);
		((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->Show(false);
		GetDlgItem("Btn_Webshop")->Show(false);
		GetDlgItem("Gfx_Webshop")->Show(false);
		GetDlgItem("Btn_QshopHelp")->Show(false);
		m_pLab_Title->SetText(GetStringFromTable(7397));
	}
	GetDlgItem("Btn_QSBStipple")->Enable(SHOP_TYPE_CROSSSERVER!=m_iQshopType);
	DWORD i;
	for(i = 0; i < maintype->size(); i++ )
		if( m_pBtn_MainType[i] )
		{
			m_pBtn_MainType[i]->SetText((ACHAR*)(*maintype)[i].szName);
			m_pBtn_MainType[i]->Show(true);
		}
	for(; i < CDLGQSHOP_BUTTONMAX; i++ )
		if( m_pBtn_MainType[i] )
			m_pBtn_MainType[i]->Show(false);

	OnCommand_MainType("maintype0");
	m_nAngle = 0;

	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		pHost->SyncDemonstrationPlayer(m_pPlayer, 1);
		memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
	m_dwPresentTime = GetTickCount();
}

void CDlgQShop::OnCommand_Payment(const char * szCommand)
{
	GetGameSession()->open_url_by_token("payment");
}

void CDlgQShop::OnCommand_FastPay(const char * szCommand)
{
	CDlgFastPay* pDlgFastPay = 	(CDlgFastPay*)GetGameUIMan()->GetDialog("Win_Qshop_QuickInputMoney");
	if(pDlgFastPay->HasBind(1))
		pDlgFastPay->Show(true);
	else	// open url
	{
		GetGameSession()->open_url_by_token("fastpay");
	}
}

void CDlgQShop::OnCommand_GetGift(const char * szCommand)
{
	int index = GetHostPlayer()->GetTackGiftIndex();
	if (index>=0)
		c2s_SendCmdGetCashGiftAward();
}

void CDlgQShop::OnCommand_ChangeShop(const char * szCommand)
{
	if( strcmp(szCommand, "qshop") == 0 && m_szName == "Win_QShopPop")
	{
		Show(false);
		CDlgQShop *pDlgQshop = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShop");
		SetShopType(SHOP_TYPE_Q);
		pDlgQshop->Show(true);
		pDlgQshop->SetPosEx(GetPos().x, GetPos().y);
//		GetDlgItem("Img_MainType0")->Show(true);
	}
	else if( strcmp(szCommand, "popshop") == 0 && m_szName == "Win_QShop")
	{
		Show(false);
		CDlgQShop *pDlgQshop = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");
		SetShopType(SHOP_TYPE_POP);
		pDlgQshop->Show(true);
		pDlgQshop->SetPosEx(GetPos().x, GetPos().y);
		LuaEvent::FirstOpenQshop2();
//		GetDlgItem("Img_MainType0")->Show(false);
	}
}

void CDlgQShop::OnCommand_Help(const char * szCommand)
{
	/*PAUIDIALOG pDlgHelp = GetGameUIMan()->GetDialog("Win_Qshop_Help");
	if (pDlgHelp)
	{
		pDlgHelp->Show(!pDlgHelp->IsShow());
	}*/
	AString strDlg = "Win_Qshop_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgQShop::OnCommand_Top(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(0);
}

void CDlgQShop::OnCommand_Bottom(const char * szCommand)
{
	m_pScl_Item->SetBarLevel((m_vecIdItem.size() - 1) / 3 - 2);
}

void CDlgQShop::OnCommand_Prec(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 3);
}

void CDlgQShop::OnCommand_Next(const char * szCommand)
{
	m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 3);
}

void CDlgQShop::OnCommand_PresentNext(const char * szCommand)
{
	int nSize = (int)m_vecPresentItems.size();
	int nPropType = CDlgQShopItem::GetSaleType();
	for (int i=m_nCurPresentIndex+1; i<((int)m_nCurPresentIndex+nSize); i++)
	{
		UINT index;
		if (i>=nSize)
			index = i-nSize;
		else
			index = i;
		GSHOP_ITEM* item = m_vecPresentItems[index];
		if (item->props & nPropType)
		{
			m_nCurPresentIndex = index;
			break;
		}
	}

	UpdatePresentItemInfo();
}

void CDlgQShop::OnCommand_PresentPrev(const char * szCommand)
{
	int nSize = (int)m_vecPresentItems.size();
	int nPropType = CDlgQShopItem::GetSaleType();
	for (int i=(int)m_nCurPresentIndex-1; i>((int)m_nCurPresentIndex-nSize); i--)
	{
		UINT index;
		if (i>=0)
			index = i;
		else
			index = i+nSize;
		GSHOP_ITEM* item = m_vecPresentItems[index];
		if (item->props & nPropType)
		{
			m_nCurPresentIndex = index;
			break;
		}
	}

	UpdatePresentItemInfo();
}

void CDlgQShop::BuildPresentItems()
{
	if ( m_szName != "Win_QShop")
		return;

	m_vecPresentItems.clear();
	m_nCurPresentIndex = 0;
	abase::vector<GSHOP_ITEM> *items;
	items = globaldata_getgshopitems();
	UINT nSize = items->size();
	UINT i;
	for (i=0; i<nSize; i++)
	{
		GSHOP_ITEM item = (*items)[i];
		if ( item.valid_time.type==0 || m_mapIndexRemainTime.find(i)!=m_mapIndexRemainTime.end())
		{
			if (item.bHasPresent)
			{
				GSHOP_ITEM* pItem = new GSHOP_ITEM;
				int n = sizeof(GSHOP_ITEM);
				memcpy(pItem, &item, sizeof(GSHOP_ITEM));
				m_vecPresentItems.push_back(pItem);
			}
		}
	}
	UpdatePresentItemInfo();
	nSize = m_vecPresentItems.size();
	GetDlgItem("Btn_LotteryPrev")->Show(nSize>1);
	GetDlgItem("Btn_LotteryNext")->Show(nSize>1);
	if (nSize==0)
	{
		m_pImg_Char->SetSize(m_pImg_Char->GetSize().cx, m_pImg_Char->GetSize().cy+(m_pImg_Char->GetPos().y - GetDlgItem("Shape")->GetPos().y));
		m_pImg_Char->SetPos(GetDlgItem("Shape")->GetPos(true).x, GetDlgItem("Shape")->GetPos(true).y);
		GetDlgItem("Pic_Goods")->Show(false);
		GetDlgItem("Lab_LotteryName")->Show(false);
		GetDlgItem("Lab_LotteryGift")->Show(false);
		GetDlgItem("Lab_GiftPage")->Show(false);
		GetDlgItem("Img_Lottery")->Show(false);
		GetDlgItem("Btn_LotteryNext")->Show(false);
		GetDlgItem("Btn_LotteryPrev")->Show(false);
		GetDlgItem("Lab_Txt1")->Show(false);
		GetDlgItem("Lab_Txt2")->Show(false);
		GetDlgItem("Shape")->Show(false);
	}
}

void CDlgQShop::UpdatePresentItemInfo()
{
	if (m_nCurPresentIndex>=0 && m_nCurPresentIndex<m_vecPresentItems.size())
	{
		UINT nSize = m_vecPresentItems.size();
		int nPropType = CDlgQShopItem::GetSaleType();
		int nTotal = 0;
		int nCurIndex = 0;
		GSHOP_ITEM* item = m_vecPresentItems[m_nCurPresentIndex];
		if ((item->props&nPropType) == 0)
		{
			for (UINT i=m_nCurPresentIndex; i<(m_nCurPresentIndex+nSize); i++)
			{
				UINT index;
				if (i>=nSize)
					index = i-nSize;
				else
					index = i;
				GSHOP_ITEM* item = m_vecPresentItems[index];
				if (item->props & nPropType)
				{
					m_nCurPresentIndex = index;
					break;
				}
			}
		}
		
		for (UINT i=0; i<nSize; i++)
		{
			GSHOP_ITEM* item = m_vecPresentItems[i];
			if (item->props & nPropType)
			{
				nTotal++;
				if (i<=m_nCurPresentIndex)
				{
					nCurIndex++;
				}
			}
		}

		GetDlgItem("Btn_LotteryPrev")->Show(nTotal>1);
		GetDlgItem("Btn_LotteryNext")->Show(nTotal>1);
		if (nTotal==0)
		{
			m_pImg_Char->SetSize(m_pImg_Char->GetSize().cx, m_pImg_Char->GetSize().cy+(m_pImg_Char->GetPos().y - GetDlgItem("Shape")->GetPos().y));
			m_pImg_Char->SetPos(GetDlgItem("Shape")->GetPos(true).x, GetDlgItem("Shape")->GetPos(true).y);
			GetDlgItem("Pic_Goods")->Show(false);
			GetDlgItem("Lab_LotteryName")->Show(false);
			GetDlgItem("Lab_LotteryGift")->Show(false);
			GetDlgItem("Lab_GiftPage")->Show(false);
			GetDlgItem("Img_Lottery")->Show(false);
			GetDlgItem("Btn_LotteryNext")->Show(false);
			GetDlgItem("Btn_LotteryPrev")->Show(false);
			GetDlgItem("Lab_Txt1")->Show(false);
			GetDlgItem("Lab_Txt2")->Show(false);
			GetDlgItem("Shape")->Show(false);
			return;
		}
		else if (!GetDlgItem("Pic_Goods")->IsShow())
		{
			m_pImg_Char->SetSize(m_pImg_Char->GetSize().cx, m_pImg_Char->GetSize().cy-GetDlgItem("Shape")->GetSize().cy-2);
			m_pImg_Char->SetPos(GetDlgItem("Shape")->GetPos(true).x, GetDlgItem("Shape")->GetPos(true).y+GetDlgItem("Shape")->GetSize().cy+2);
			GetDlgItem("Pic_Goods")->Show(true);
			GetDlgItem("Lab_LotteryName")->Show(true);
			GetDlgItem("Lab_LotteryGift")->Show(true);
			GetDlgItem("Lab_GiftPage")->Show(true);
			GetDlgItem("Img_Lottery")->Show(true);
			GetDlgItem("Btn_LotteryNext")->Show(true);
			GetDlgItem("Btn_LotteryPrev")->Show(true);
			GetDlgItem("Lab_Txt1")->Show(true);
			GetDlgItem("Lab_Txt2")->Show(true);
			GetDlgItem("Shape")->Show(true);
		}

		item = m_vecPresentItems[m_nCurPresentIndex];
		
		PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Goods");
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Lab_LotteryName");
		PAUILABEL pLabGift = (PAUILABEL)GetDlgItem("Lab_LotteryGift");
		PAUILABEL pLabGiftPage = (PAUILABEL)GetDlgItem("Lab_GiftPage");

		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn, item->Present_file_icon + strlen("Surfaces\\"));
		pImg->SetProperty("Image File", &p);
		pImg->SetHint((ACHAR*)item->Presentdesc);
		CECIvtrItem *pItem = CECIvtrItem::CreateItem(item->id, 0, item->num);
		if (pItem)
		{
			pLab->SetText(pItem->GetName());
			delete pItem;
		}
		CECIvtrItem *pItemGift = CECIvtrItem::CreateItem(item->Presentid, 0, item->Presentcount);
		if (pItemGift)
		{
			pLabGift->SetText(pItemGift->GetName());
			delete pItemGift;
		}
		ACString strText;
		strText.Format(_AL("%d/%d"), nCurIndex, nTotal);
		pLabGiftPage->SetText(strText);
		m_dwPresentTime = GetTickCount();
	}

}

void CDlgQShop::OnCommand_MainType(const char * szCommand)
{
	m_nMainType = atoi(szCommand + strlen("maintype"));
	vector<GSHOP_MAIN_TYPE> *maintype;
	if (SHOP_TYPE_Q==m_iQshopType)
	{
		maintype = globaldata_getgshopmaintypes();
	}
	else if (SHOP_TYPE_POP==m_iQshopType)
	{
		maintype = globaldata_getgshopmaintypes2();
	}
	else if (SHOP_TYPE_CROSSSERVER==m_iQshopType)
	{
		maintype = globaldata_getgshopmaintypes3();
	}
	DWORD i;
	for(i = 0; i < CDLGQSHOP_BUTTONMAX; i++ )
		if( m_pBtn_MainType[i] )
			m_pBtn_MainType[i]->SetPushed(m_nMainType == (int)i);
	for(i = 0; i < (*maintype)[m_nMainType].sub_types.size(); i++ )
		if( m_pBtn_SubType[i] )
		{
			m_pBtn_SubType[i]->SetText((*maintype)[m_nMainType].sub_types[i]);
			m_pBtn_SubType[i]->Show(true);
		}
	for(; i < CDLGQSHOP_BUTTONMAX; i++ )
		if( m_pBtn_SubType[i] )
			m_pBtn_SubType[i]->Show(false);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SubType-1"))->Show(true);
	if( (*maintype)[m_nMainType].sub_types.size() > 0 )
		OnCommand_SubType("subtype0");
	else
		OnCommand_SubType("subtype-1");
}

void CDlgQShop::OnCommand_SubType(const char * szCommand)
{
	m_vecIdItem.clear();
	m_nSubType = atoi(szCommand + strlen("subtype"));
	int i;
	abase::vector<GSHOP_ITEM> *items;
	if (SHOP_TYPE_Q==m_iQshopType)
	{
		items = globaldata_getgshopitems();
	}
	else if (SHOP_TYPE_POP==m_iQshopType)
	{
		items = globaldata_getgshopitems2();
	}
	else if (SHOP_TYPE_CROSSSERVER==m_iQshopType)
	{
		items = globaldata_getgshopitems3();
	}
	int w0 = 0;
	int w1 = 0;
	int w2 = 0;
	for(i = 0; i < CDLGQSHOP_BUTTONMAX; i++ )
		if( m_pBtn_SubType[i] )
			m_pBtn_SubType[i]->SetPushed(m_nSubType == i);
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SubType-1"))->SetPushed(m_nSubType == -1);
	int nSaleType = CDlgQShopItem::GetSaleType();
	for(i = 0; i < (int)items->size(); i++ )
	{
		if (((*items)[i].props&nSaleType) == 0)
		{
			continue;
		}
		else if ( !ms_bTestServer && (*items)[i].valid_time.type!=0 && m_mapIndexRemainTime.find(i)==m_mapIndexRemainTime.end())
		{
			continue;
		}

		if( (int)(*items)[i].main_type == m_nMainType &&
			(m_nSubType == -1 || (int)(*items)[i].sub_type == m_nSubType) )
		{
			if( (int)((*items)[i].props & 1) == 1 )
			{
				m_vecIdItem.insert(m_vecIdItem.begin() + w0, i);
				w0++;
				w1++;
				w2++;
			}
			else if( (int)((*items)[i].props & 2) == 2 )
			{
				m_vecIdItem.insert(m_vecIdItem.begin() + w1, i);
				w1++;
				w2++;
			}
			else if( (int)((*items)[i].props & 4) == 4 )
			{
				m_vecIdItem.insert(m_vecIdItem.begin() + w2, i);
				w2++;
			}
			else
				m_vecIdItem.push_back(i);
		}
	}
	
	if( m_vecIdItem.size() > CECGAMEUIMAN_MAX_QSHOPITEMS )
	{
		m_pScl_Item->SetScrollRange(0, (m_vecIdItem.size() - 1) / 3 - 2);
		m_pScl_Item->Show(true);
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}
	m_pScl_Item->SetBarLevel(0);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nStartLine = -1;
	SetData(CECGAMEUIMAN_MAX_QSHOPITEMS);
}

void CDlgQShop::OnCommand_ResumeEquip(const char * szCommand)
{
	if (m_pPlayer)
	{
		CECHostPlayer* pHost = GetHostPlayer();
		memcpy(m_aEquips, pHost->GetEquips(), sizeof(m_aEquips));
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
}

void CDlgQShop::OnCommand_CANCEL(const char * szCommand)
{
	Show(false);
	int i;
	for(i = 0; i < CECGAMEUIMAN_MAX_QSHOPITEMS; i++)
	{
		GetGameUIMan()->m_pDlgQShopDefault[i]->Show(false);
	}
}

void CDlgQShop::OnCommand_Edit(const char * szCommand)
{
	if(m_iQshopType != SHOP_TYPE_Q) return;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Search");
	pList->ResetContent();

	ACString strInfo = ((PAUIEDITBOX)GetDlgItem("Edt_Search"))->GetText();
	for(int i = 0; i < m_vecSearchWord.size(); i++)
	{
		if(IsStringMatch(m_vecSearchWord[i], strInfo))
			pList->AddString(m_vecSearchWord[i]);
	}
}

void CDlgQShop::OnCommand_Search(const char * szCommand)
{
	m_vecIdItem.clear();
	ACString strText = ((PAUIEDITBOX)GetDlgItem("Edt_Search"))->GetText();
	AWString strInfo;
	strInfo = strText;
	strInfo.MakeLower();
	abase::vector<GSHOP_ITEM> *items;
	vector<GSHOP_MAIN_TYPE> *maintype;
	if (SHOP_TYPE_Q==m_iQshopType && !strText.IsEmpty())
	{
		items = globaldata_getgshopitems();			
		maintype = globaldata_getgshopmaintypes();
	}
	else return;

	int w0 = 0;
	int w1 = 0;
	int w2 = 0;
		
	int nSaleType = CDlgQShopItem::GetSaleType();
	int i;
	for(i = 0; i < (int)items->size(); i++ )
	{
		if (((*items)[i].props&nSaleType) == 0)
		{
			continue;
		}
		else if ( !ms_bTestServer && (*items)[i].valid_time.type!=0 && m_mapIndexRemainTime.find(i)==m_mapIndexRemainTime.end())
		{
			continue;
		}
		
		AWString strLabel = _AL("");
		if((*items)[i].main_type < maintype->size() && (*items)[i].sub_type < (*maintype)[m_nMainType].sub_types.size())
			strLabel = (*maintype)[(*items)[i].main_type].sub_types[(*items)[i].sub_type];

		AWString strKeyWord; 
		strKeyWord = (wchar_t*)(*items)[i].SearchKey;
		strKeyWord.MakeLower();
		CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[i].id, 0, (*items)[i].num);
		AWString strItem = _AL("");
		if(pItem)
		{
			strItem = pItem->GetName();
			delete pItem;
		}
		AWString strName = strItem;
		strName.MakeLower();		
		
		CSplit s = strKeyWord;
		CSPLIT_STRING_VECTOR vec = s.Split(_AL(","));
		bool bMatch = false;
		bool bEqual = false;
		if(strKeyWord.GetLength() > 0)
			for(int j = 0; j < vec.size(); j++)
			{
				if(vec[j].Find(strInfo) >= 0 || strInfo.Find(vec[j]) >= 0)
					bMatch = true;
				if(vec[j].Compare(strInfo) == 0)
					bEqual = true;
			}
		

		if(strName.Compare(strInfo) == 0 || bEqual)
		{
			m_vecIdItem.insert(m_vecIdItem.begin() + w0, i);
			w0++;
			w1++;
			w2++;
		}
		else if(strName.Find(strInfo) >= 0 || (strInfo.Find(strName) >=0 && !strName.IsEmpty()))
		{
			m_vecIdItem.insert(m_vecIdItem.begin() + w1, i);
			w1++;
			w2++;
		}
		else if(bMatch)
		{
			m_vecIdItem.insert(m_vecIdItem.begin() + w2, i);
			w2++;
		}
		else if(strLabel.Find(strInfo) >= 0 || (strInfo.Find(strLabel) >= 0 && !strLabel.IsEmpty()))
		{
			m_vecIdItem.push_back(i);
		}
	}
	
	if( m_vecIdItem.size() > CECGAMEUIMAN_MAX_QSHOPITEMS )
	{
		m_pScl_Item->SetScrollRange(0, (m_vecIdItem.size() - 1) / 3 - 2);
		m_pScl_Item->Show(true);
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}
	m_pScl_Item->SetBarLevel(0);
	m_pScl_Item->SetBarLength(-1);
	m_pScl_Item->SetScrollStep(1);
	m_nStartLine = -1;
	SetData(CECGAMEUIMAN_MAX_QSHOPITEMS);

	for(i = 0; i < CDLGQSHOP_BUTTONMAX; i++ )
	{
		if( m_pBtn_MainType[i] )
			m_pBtn_MainType[i]->SetPushed(false);
		if( m_pBtn_SubType[i] )
			m_pBtn_SubType[i]->Show(false);
	}
	((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_SubType-1"))->Show(false);
	if(m_vecIdItem.size() == 0)
		GetGameUIMan()->MessageBox("", GetGameUIMan()->GetStringFromTable(18210),MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
}

void CDlgQShop::OnCommand_WebShop(const char* szCommand)
{
	CDlgWebShopOrder* pDlgWebShopOrder = (CDlgWebShopOrder*)GetGameUIMan()->GetDialog("Win_Qshop_Web");
	if(pDlgWebShopOrder)
		pDlgWebShopOrder->Show(!pDlgWebShopOrder->IsShow());
	OnCommand_CANCEL("");
}

void CDlgQShop::OnTick()
{
	vector<GSHOP_ITEM> *items;
	if (SHOP_TYPE_Q==m_iQshopType)
	{
		m_pTxt_Cash->SetText(GetCashText(GetHostPlayer()->GetCash(), true));
		ACString strText;
		strText.Format(_AL("%d"), GetHostPlayer()->GetGiftScore());
		m_pLab_ConsumeScore->SetText(strText);
		GetDlgItem("Btn_ConsumeScore")->Enable(GetHostPlayer()->GetTackGiftIndex()>=0);
		items = globaldata_getgshopitems();
		//500s自动切换
		if ((GetTickCount()-m_dwPresentTime)>5000)
		{
			OnCommand_PresentNext("");
		}

		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Search");
		PAUIEDITBOX pEdit = (PAUIEDITBOX)GetDlgItem("Edt_Search");
		ACString strSearch = pEdit->GetText();
		if(m_strSearch != strSearch)
		{
			m_strSearch = strSearch;
			OnCommand_Edit("");
		}
		pList->Show(pEdit == GetFocus() && !strSearch.IsEmpty());
		//特殊处理Lst_Search下面控件的字显示在其上的问题
		if(pList->IsShow())
		{
			if(m_pBtn_MainType[3]->IsShow())
				m_pBtn_MainType[3]->SetText(_AL(""));
			if(m_pBtn_MainType[4]->IsShow())
				m_pBtn_MainType[4]->SetText(_AL(""));
			if(m_pBtn_SubType[5]->IsShow())
				m_pBtn_SubType[5]->SetText(_AL(""));
			if(m_pBtn_SubType[6]->IsShow())
				m_pBtn_SubType[6]->SetText(_AL(""));
		}
		else
		{
			vector<GSHOP_MAIN_TYPE> *maintype;
			maintype = globaldata_getgshopmaintypes();
			if(maintype->size() > 3)
				m_pBtn_MainType[3]->SetText((ACHAR*)(*maintype)[3].szName);
			if(maintype->size() > 4)
				m_pBtn_MainType[4]->SetText((ACHAR*)(*maintype)[4].szName);
			if((*maintype)[m_nMainType].sub_types.size() > 5)
				m_pBtn_SubType[5]->SetText((*maintype)[m_nMainType].sub_types[5]);
			if((*maintype)[m_nMainType].sub_types.size() > 6)
				m_pBtn_SubType[6]->SetText((*maintype)[m_nMainType].sub_types[6]);
		}
	}
	else if (SHOP_TYPE_POP==m_iQshopType)
	{
		m_pTxt_Cash->SetText(GetCashText(GetHostPlayer()->GetBonusAmount(), true));
		items = globaldata_getgshopitems2();
	}
	else if (SHOP_TYPE_CROSSSERVER==m_iQshopType)
	{
		ACString strText;
		strText.Format(_AL("%d"), GetHostPlayer()->GetPack()->GetItemTotalNum(ms_iCrossServerItemId));
		m_pTxt_Cash->SetText(strText);
		items = globaldata_getgshopitems3();
	}
	if( m_nStartLine != m_pScl_Item->GetBarLevel() )
	{
		int w0 = 0;
		m_nStartLine = m_pScl_Item->GetBarLevel();
		int nStart = m_nStartLine * 3;
		int nEnd = nStart + CECGAMEUIMAN_MAX_QSHOPITEMS;
		if( nEnd > (int)m_vecIdItem.size() )
			nEnd = m_vecIdItem.size();
		int i;
		for(i = nStart; i < nEnd; i++)
		{
			int index = m_vecIdItem[i];
			int remainTime = m_mapIndexRemainTime.find(index)==m_mapIndexRemainTime.end() ? -1 : m_mapIndexRemainTime[index];
			if ( CDlgQShop::SHOP_TYPE_Q!=CDlgQShop::GetShopType() )
			{
				remainTime = -1;
			}
			GetGameUIMan()->m_pDlgQShopDefault[w0++]->SetItem(i - nStart, index, remainTime);
		}
		for(i = w0; i < CECGAMEUIMAN_MAX_QSHOPITEMS; i++)
			GetGameUIMan()->m_pDlgQShopDefault[i]->SetItem();
	}

	if( m_pBtn_LeftTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + 360 - GetGame()->GetTickTime() / 6 ) % 360;
	else if( m_pBtn_RightTurn->GetState() == AUISTILLIMAGEBUTTON_STATE_CLICK )
		m_nAngle = (m_nAngle + GetGame()->GetTickTime() / 6) % 360;

	if (m_pPlayer)
	{
		m_pPlayer->Stand();
		m_pPlayer->Tick(g_pGame->GetTickTime());


		m_pImg_Char->SetRenderCallback(
			PlayerRenderDemonstration,
			(DWORD)m_pPlayer,
			m_nAngle);
	}

	abase::hash_map<int, int>::iterator it;
	bool bHasOverTime = false;
	for( it=m_mapIndexRemainTime.begin(); it!=m_mapIndexRemainTime.end(); ++it)
	{
		if (it->second > (int)GetGame()->GetTickTime())
		{
			it->second -= GetGame()->GetTickTime();
		}
		else if ( it->second==-1 )	// 定时开始，不结束
		{
			continue;
		}
		else if( it->second!=-9999) // 定义这个数为无效值，等待服务器确定
		{
			it->second = -9999;
			bHasOverTime = true;
		}
	}

	if (bHasOverTime)
	{
		m_bGetSaleTime = true;
	}
	if (m_bGetSaleTime && GetHostPlayer()->GetCoolTime(GP_CT_GET_MALL_PRICE)<=0)
	{
		m_bGetSaleTime = false;
		c2s_SendCmdQueryMallSaleTimeItem();
	}

	// 玩家在天界上不能快捷支付
	if(g_pGame->GetGameRun()->GetHostPlayer())
	{
		bool bCrsServer = (GetHostPlayer()->GetSrcServerID()!=0);
		if(bCrsServer)
		{
			if(a_strcmp(AS2AC(m_szName), _AL("Winn_QShop")) == 0)
			{
				if(GetDlgItem("Btn_QSBStipple_Quick"))
					GetDlgItem("Btn_QSBStipple_Quick")->Enable(false);
			}
		}
	}
	if(m_iQshopType == SHOP_TYPE_Q || m_iQshopType == SHOP_TYPE_POP)
	{
		CDlgWebShopOrder* pDlgWebShopOrder = (CDlgWebShopOrder*)GetGameUIMan()->GetDialog("Win_Qshop_Web");
		if(pDlgWebShopOrder)
			GetDlgItem("Gfx_Webshop")->Show(pDlgWebShopOrder->IsHasOrder());
	}
}

void CDlgQShop::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(m_iQshopType == SHOP_TYPE_Q)
	{
		PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Search");
		if(pList && pList->IsShow())
			return;
	}
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;
	if( m_pScl_Item->IsShow() )
		m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - zDelta);
}

void CDlgQShop::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if(m_iQshopType != SHOP_TYPE_Q) return;
	PAUILISTBOX pList = (PAUILISTBOX)GetDlgItem("Lst_Search");
	int nCurSel = pList->GetCurSel();
	if(nCurSel >= 0 && nCurSel < pList->GetCount())
	{
		((PAUIEDITBOX)GetDlgItem("Edt_Search"))->SetText(pList->GetText(nCurSel));
		OnCommand_Search("");
	}
	pList->Show(false);
	
	
}

void CDlgQShop::ChangeEquip(const EQUIPMENT_ESSENCE* pEquip)
{
	if (!m_pPlayer)
		return;

	if (pEquip->require_gender != 2 && (int)pEquip->require_gender != GetHostPlayer()->GetGender())
		return;

	if (pEquip->equip_type == 3 || pEquip->equip_type == 4) // 时装或翅膀
	{
		int i;
		for (i = 0; i < SIZE_EQUIPIVTR; i++)
			if ((1 << i) & pEquip->equip_mask)
				break;

		if (i == SIZE_EQUIPIVTR)
			return;

		m_aEquips[i] = pEquip->id;
		m_pPlayer->ShowEquipments(m_aEquips, false, true);
		m_pPlayer->SetFashionMode(true);
	}
}

ACString CDlgQShop::GetCashText(int nCash, bool bFullText, bool bPoint)
{
	ACString strText;
	if( bFullText )
		strText.Format(GetStringFromTable(1403), nCash / 100, nCash % 100);
	else if( bPoint )
	{
		if (nCash%100 == 0)
		{
			strText.Format(GetStringFromTable(1401), nCash/100);
		}
		else
		{
			if (nCash%10 == 0)
			{
				strText.Format(GetStringFromTable(1401), (nCash%100)/10);
			}
			else
			{
				strText.Format(GetStringFromTable(1401), nCash%100);
			}
			strText = _AL(".") + strText;
			ACString strInt;
			strInt.Format(_AL("%d"), nCash / 100);
			strText = strInt + strText;
		}
	}
	else if( nCash < 100 )
		strText.Format(GetStringFromTable(1402), nCash);
	else if( nCash % 100 == 0 )
		strText.Format(GetStringFromTable(1401), nCash / 100);
	else
		strText.Format(GetStringFromTable(1403), nCash / 100, nCash % 100);
	return strText;
}

void CDlgQShop::SetShopType(int type)
{
	if (m_iQshopType != type)
	{
		m_iQshopType = type;
// 		if (0==m_iQshopType)
// 		{
// 			((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->SetPushed(true);
// 			((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->SetPushed(false);
// 			m_pLab_Title->SetText(GetStringFromTable(7398));
// 		}
// 		else if (1==m_iQshopType)
// 		{
// 			((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Qshop"))->SetPushed(false);
// 			((PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PopShop"))->SetPushed(true);
// 			m_pLab_Title->SetText(GetStringFromTable(7399));
// 		}
// 		OnCommand_MainType("maintype0");
//		PAUISTILLIMAGEBUTTON pButton = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_QShopG");
//		AUIOBJECT_SETPROPERTY p;
//		if (0==m_iQshopType)
//		{
//			strcpy(p.fn,"button/QShopG.tga");
//			pButton->SetProperty("Up Frame File",&p);
//			strcpy(p.fn,"button/QShopGDown.tga");
//			pButton->SetProperty("Down Frame File",&p);
//		}
//		else if (1==m_iQshopType)
//		{
//			strcpy(p.fn,"button/PopShopP.tga");
//			pButton->SetProperty("Up Frame File",&p);
//			strcpy(p.fn,"button/PopShopPDown.tga");
//			pButton->SetProperty("Down Frame File",&p);
//		}
	}
}

int CDlgQShop::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg=="set_time")
	{
		m_mapIndexRemainTime.clear();
		const S2C::cmd_mall_item_price *pCmd = (const S2C::cmd_mall_item_price *)wParam;
		int i;
		for (i=0; i<pCmd->count; ++i)
		{
			m_mapIndexRemainTime[pCmd->list[i].good_index] = pCmd->list[i].remaining_time;
			if (m_mapIndexRemainTime[pCmd->list[i].good_index]>0)
			{
				m_mapIndexRemainTime[pCmd->list[i].good_index] *= 1000;
			}
		}
		BuildPresentItems();

		if ( m_bKeepSearchResultOnce )
			m_bKeepSearchResultOnce = false;
		else
			Refresh();
	}
	else if (strMsg=="open_url")
	{		
		DWORD dwThreadID;
		m_strUrl = *(AString*)wParam;
		CreateThread(NULL, 0, OpenUrl, this, 0, &dwThreadID);
	}

	return 0;
}

void CDlgQShop::OpenQShop( ACString strSearch )
{
	if ( CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() )
	{
		if( globaldata_getgshop_timestamp() != GetGameRun()->GetGShopTimeStamp() )
		{
			GetGameUIMan()->MessageBox("GShopVersionError", GetStringFromTable(763), 
			MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
			return;
		}
	}

	if (GetHostPlayer()->GetSrcServerID()==0)
	{
		CDlgQShop::SetShopType(CDlgQShop::SHOP_TYPE_Q);
		if (GetHostPlayer()->GetCoolTime(GP_CT_GET_MALL_PRICE)<=0)
		{
			m_bKeepSearchResultOnce = true;
			c2s_SendCmdQueryMallSaleTimeItem();
		}
	}
	else
	{
		CDlgQShop::SetShopType(CDlgQShop::SHOP_TYPE_CROSSSERVER);
	}

	Show(true);
	LuaEvent::FirstOpenQshop();

	((PAUIEDITBOX)GetDlgItem("Edt_Search"))->SetText( strSearch );
	OnCommand_Search("");
}

void CDlgQShop::Refresh()
{
	AString strCommand;
	strCommand.Format("subtype%d", m_nSubType);
	OnCommand_SubType(strCommand);
	UpdatePresentItemInfo();
}

void CDlgQShop::ReLoadModel()
{
	A3DRELEASE(m_pPlayer);
	m_pPlayer = new CECLoginPlayer(g_pGame->GetGameRun()->GetWorld()->GetPlayerMan());
	const GNET::RoleInfo& info = g_pGame->GetGameRun()->GetSelectedRoleInfo();
	
	if (!m_pPlayer->Load(CECPlayer::GetPhysiqueByProf(info.occupation), info.occupation, info.faceid, info.hairid, info.gender, NULL))
	{
		m_pPlayer->Release();
		delete m_pPlayer;
		m_pPlayer = NULL;
	}
	else
	{
		m_pPlayer->SetPos(A3DVECTOR3(0));
		m_pPlayer->SetDirAndUp(A3DVECTOR3(0, 0, -1.0f), A3DVECTOR3(0, 1.0f, 0));
		m_pPlayer->GetPlayerModel()->SetAutoUpdateFlag(false);
	}
	memset(m_aEquips, 0, sizeof(m_aEquips));
}

DWORD WINAPI CDlgQShop::OpenUrl(void * ptr)
{
	CDlgQShop* pDlgQShop = (CDlgQShop*)ptr;
	ShellExecuteA(NULL,"open", pDlgQShop->m_strUrl, NULL, NULL, SW_NORMAL);

	return 0;
}

bool CDlgQShop::IsStringMatch(ACString strName, ACString strSearch)
{
	bool bRet = a_strstr(strName, strSearch) != NULL;
	const char *szCountry = GetGame()->GetConfigs()->GetCountryRegion();
	if ( !bRet && 0 == stricmp(szCountry, "cn") )
	{
		ACHAR ch = _AL('');
		int i;
		for (i=0; i<strSearch.GetLength(); ++i)
		{
			if (i>=strName.GetLength())
				break;
			
			if (strSearch[i]>=_AL('a')&&strSearch[i]<=_AL('z'))
				ch = strSearch[i];
			else if (strSearch[i]>=_AL('A')&&strSearch[i]<=_AL('Z'))
				ch = strSearch[i] - _AL('A') + _AL('a');
			if (!_ChinaPinYin(ch, strName[i]))
			{
				break;
			}
		}
		// 没有不匹配字符
		if (i==strName.GetLength()||i==strSearch.GetLength())
		{
			bRet = true;
		}
	}
	return bRet;
}

bool CDlgQShop::_ChinaPinYin(ACHAR chSearch, ACHAR chName)
{
	AString strName = AC2AS(ACString(chName, 1));
	int chWideChar = (unsigned char)strName[0];
	chWideChar <<= 8;
	chWideChar += (unsigned char)strName[1];
	return (chSearch==_AL('a') && chWideChar>=0xB0A1 && chWideChar<=0xB0C4) ||
		(chSearch==_AL('b') && chWideChar>=0XB0C5 && chWideChar<=0XB2C0) ||
		(chSearch==_AL('c') && chWideChar>=0xB2C1 && chWideChar<=0xB4ED) ||
		(chSearch==_AL('d') && chWideChar>=0xB4EE && chWideChar<=0xB6E9) ||
		(chSearch==_AL('e') && chWideChar>=0xB6EA && chWideChar<=0xB7A1) ||
		(chSearch==_AL('f') && chWideChar>=0xB7A2 && chWideChar<=0xB8c0) ||
		(chSearch==_AL('g') && chWideChar>=0xB8C1 && chWideChar<=0xB9FD) ||
		(chSearch==_AL('h') && chWideChar>=0xB9FE && chWideChar<=0xBBF6) ||
		(chSearch==_AL('j') && chWideChar>=0xBBF7 && chWideChar<=0xBFA5) ||
		(chSearch==_AL('k') && chWideChar>=0xBFA6 && chWideChar<=0xC0AB) ||
		(chSearch==_AL('l') && chWideChar>=0xC0AC && chWideChar<=0xC2E7) ||
		(chSearch==_AL('m') && chWideChar>=0xC2E8 && chWideChar<=0xC4C2) ||
		(chSearch==_AL('n') && chWideChar>=0xC4C3 && chWideChar<=0xC5B5) ||
		(chSearch==_AL('o') && chWideChar>=0xC5B6 && chWideChar<=0xC5BD) ||
		(chSearch==_AL('p') && chWideChar>=0xC5BE && chWideChar<=0xC6D9) ||
		(chSearch==_AL('q') && chWideChar>=0xC6DA && chWideChar<=0xC8BA) ||
		(chSearch==_AL('r') && chWideChar>=0xC8BB && chWideChar<=0xC8F5) ||
		(chSearch==_AL('s') && chWideChar>=0xC8F6 && chWideChar<=0xCBF0) ||
		(chSearch==_AL('t') && chWideChar>=0xCBFA && chWideChar<=0xCDD9) ||
		(chSearch==_AL('w') && chWideChar>=0xCDDA && chWideChar<=0xCEF3) ||
		(chSearch==_AL('x') && chWideChar>=0xCEF4 && chWideChar<=0xD188) ||
		(chSearch==_AL('y') && chWideChar>=0xD1B9 && chWideChar<=0xD4D0) ||
		(chSearch==_AL('z') && chWideChar>=0xD4D1 && chWideChar<=0xD7F9);
}
