// Filename	: DlgQShopItem.cpp
// Creator	: Tom Zhou
// Date		: 2006/5/22

#include "AFI.h"
#include "A2DSprite.h"
#include "DlgQShopItem.h"
#include "DlgQShop.h"
#include "DlgInputNO.h"
#include "EC_GameUIMan.h"
#include "AUI\\AUIDef.h"
#include "AUI\\A3DFTFont.h"
#include "globaldataman.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrConsume.h"
#include "EC_GameSession.h"
#include "EC_HostPlayer.h"
#include "EC_GPDataType.h"
#include "EC_Game.h"
#include "EC_Global.h"
#include "EC_Inventory.h"
#include "EC_GameUIMisc.h"
#include "A3DFlatCollector.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgQShopItem, CDlgBase)

AUI_ON_COMMAND("buy",		OnCommand_Buy)
AUI_ON_COMMAND("IDCANCEL",	OnCommand_CANCEL)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgQShopItem, CDlgBase)

AUI_ON_EVENT("*",			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,			WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Pic_Goods",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Pic_Goods",	WM_LBUTTONDBLCLK,	OnEventLButtonDown)
AUI_ON_EVENT("Img_Discount",WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Discount",WM_LBUTTONDBLCLK,	OnEventLButtonDown)
AUI_ON_EVENT("Img_Discount",WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Discount",WM_LBUTTONDBLCLK,	OnEventLButtonDown)
AUI_ON_EVENT("Img_Cover",	WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Img_Cover",	WM_LBUTTONDBLCLK,	OnEventLButtonDown)

AUI_END_EVENT_MAP()

int	CDlgQShopItem::m_nDiscountType = 0;
int	CDlgQShopItem::m_nSaleType = 0x00ff0000;

CDlgQShopItem::CDlgQShopItem()
{
}

CDlgQShopItem::~CDlgQShopItem()
{
}

bool CDlgQShopItem::OnInitDialog()
{
	m_nItemIndex = -1;
	m_nDlgPostion = -1;
	m_bTimeLimit = false;
	m_pTxt_ItemName = (PAUILABEL)GetDlgItem("Txt_ItemName");
	m_pTxt_Time = (PAUILABEL)GetDlgItem("Txt_Time");
	m_pTxt_Price = (PAUILABEL)GetDlgItem("Txt_Price");
	m_pTxt_Number = (PAUILABEL)GetDlgItem("Txt_Number");
	m_pTxt_Bonus = (PAUILABEL)GetDlgItem("Txt_Off");
	m_pTxt_SaleRemainTime = (PAUILABEL)GetDlgItem("Txt_LimitTime");
	m_pBtn_Buy = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Buy");
	m_pImg_Model = (PAUIIMAGEPICTURE)GetDlgItem("Pic_Goods");
	SetCanMove(false);

	m_pImagePresent = new A2DSprite;
	if( !m_pImagePresent ) return AUI_ReportError(__LINE__, __FILE__);
	GetGameUIMan()->AddImageListItem(m_pImagePresent, m_nHintImageIndex);

	return true;
}

bool CDlgQShopItem::Release()
{
	A3DRELEASE(m_pImagePresent)
	return CDlgBase::Release();
}

bool CDlgQShopItem::Render()
{
	char szText[20];
	sprintf(szText, "Lab_Dialog%d", m_nDlgPostion);
	POINT pt;
	if (GetGameUIMan()->m_pDlgQShop->IsShow())
	{
		pt = GetGameUIMan()->m_pDlgQShop->GetDlgItem(szText)->GetPos();
	}
	else
		pt = GetGameUIMan()->GetDialog("Win_QShopPop")->GetDlgItem(szText)->GetPos();
	SetPosEx(pt.x, pt.y);
	if (m_bDiscount)
	{
		ACString strText = m_pTxt_Price->GetText();
		A3DFTFont *pFont = m_pTxt_Price->GetFont();
		pFont->Resize(m_pTxt_Price->GetFontHeight() * GetGameUIMan()->GetWindowScale());
		int nWidth, nHeight, nLines;
		AUI_GetTextRect(pFont, strText, nWidth, nHeight, nLines, 3);
		A3DFlatCollector *pFC = GetGame()->GetA3DEngine()->GetA3DFlatCollector();
		int x = m_pTxt_Price->GetPos().x;
		int y = m_pTxt_Price->GetPos().y + nHeight/2;
		pFC->AddRect_2D(x, y, x + nWidth, y+1, A3DCOLORRGBA(255,255,255,255));
	}

	return CDlgBase::Render();
}

void CDlgQShopItem::OnTick()
{
	if ( CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() )
	{
		m_pBtn_Buy->Enable(m_nPrice <= GetHostPlayer()->GetCash());
		if (m_bTimeLimit)
		{
			m_pTxt_SaleRemainTime->SetText(ACString(GetGameUIMan()->GetStringFromTable(13010))+GetTimeText(CDlgQShop::GetRemainTime(m_nItemIndex)/1000));
		}
	}
	else if ( CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType() )
	{
		m_pBtn_Buy->Enable(m_nPrice <= GetHostPlayer()->GetBonusAmount());
	}
	else if ( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() )
	{
		m_pBtn_Buy->Enable(m_nPrice <= GetHostPlayer()->GetPack()->GetItemTotalNum(CDlgQShop::GetCrossServerItemId()));
	}
}

void CDlgQShopItem::OnCommand_Buy(const char * szCommand)
{
	if( m_nItemIndex == -1)
		return;
	if(	CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() && m_nPrice > GetHostPlayer()->GetCash())
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8397), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	else if( CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType() && m_nPrice > GetHostPlayer()->GetBonusAmount())
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8398), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	else if( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() && m_nPrice>GetHostPlayer()->GetPack()->GetItemTotalNum(CDlgQShop::GetCrossServerItemId()) )
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(8395), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	ACHAR szText[40];
	PAUIOBJECT pEdit = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("DEFAULT_Txt_No.");
	vector<GSHOP_ITEM> *items;
	
	if ( CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() )
	{
		pEdit->SetData(GetHostPlayer()->GetCash() / m_nPrice );
		GetGameUIMan()->m_pDlgInputGNO->SetType(CDlgInputNO::INPUTNO_QSHOPITEM_BUY);
		items = globaldata_getgshopitems();
	}
	else if ( CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType() )
	{
		pEdit->SetData(GetHostPlayer()->GetBonusAmount() / m_nPrice );
		GetGameUIMan()->m_pDlgInputGNO->SetType(CDlgInputNO::INPUTNO_QSHOPITEM_BONUSBUY);
		items = globaldata_getgshopitems2();
	}
	else if ( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() )
	{
		pEdit->SetData(GetHostPlayer()->GetPack()->GetItemTotalNum(CDlgQShop::GetCrossServerItemId())/m_nPrice);
		GetGameUIMan()->m_pDlgInputGNO->SetType(CDlgInputNO::INPUTNO_QSHOPITEM_CSSBUY);
		items = globaldata_getgshopitems3();
	}
	GetGameUIMan()->m_pDlgInputGNO->Show(true, true);
	a_sprintf(szText, _AL("%d"), 1);
	pEdit->SetText(szText);
	GetGameUIMan()->m_pDlgInputGNO->SetData(m_nPrice);
	GetGameUIMan()->m_pDlgInputGNO->SetText(m_pTxt_ItemName->GetText());
	PAUIOBJECT pObj = GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Confirm");
	pObj->SetData(m_nItemIndex);
	pObj->SetDataPtr((void*)(*items)[m_nItemIndex].id);
	GetGameUIMan()->m_pDlgInputGNO->GetDlgItem("Txt_Off")->SetData(((*items)[m_nItemIndex].buy[0].price)*((*items)[m_nItemIndex].bonus)/100);
}

void CDlgQShopItem::OnCommand_CANCEL(const char * szCommand)
{
	if (GetGameUIMan()->m_pDlgQShop->IsShow())
		GetGameUIMan()->m_pDlgQShop->OnCommand_CANCEL("");
	else
	{
		CDlgQShop* pDlgQShop = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");
		pDlgQShop->OnCommand_CANCEL("");
	}
}

void CDlgQShopItem::SetItem(int dlgpostion, int itemindex, int remain_time)
{
	m_nDlgPostion = dlgpostion;
	m_bTimeLimit = remain_time>=0;

	if( m_nDlgPostion == -1 )
	{
		Show(false);
		return;
	}

	if( !IsShow() )
		Show(true);
	
	
	vector<GSHOP_ITEM> *items;
	if ( CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType() )
	{
		items = globaldata_getgshopitems();
	}
	else if ( CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType() )
	{
		items = globaldata_getgshopitems2();
	}
	else if ( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() )
	{
		items = globaldata_getgshopitems3();
	}
	if( m_nItemIndex != itemindex && (*items)[itemindex].bHasPresent)
	{
		m_pImagePresent->Release();
		AString strName;
		if (strlen((*items)[itemindex].Present_file_icon) > strlen("Surfaces\\"))
		{
			strName = (*items)[itemindex].Present_file_icon + strlen("Surfaces\\");
		}
		else
			strName = "1\\Ã»Í¼±ê.dds";
		bool bval = m_pImagePresent->Init(m_pA3DDevice, strName, AUI_COLORKEY);
		if( !bval ) { AUI_ReportError(__LINE__, __FILE__); return;}
	}
	
	m_nItemIndex = itemindex;
	m_pTxt_Bonus->Show(false);

	CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[itemindex].id, 0, (*items)[itemindex].num);
	GetDlgItem("Img_Discount")->Show(false);
	if( pItem )
	{
		ACString strText, strTemp;
		strText.Format(GetStringFromTable(1504), GetFormatTime((*items)[itemindex].buy[0].time));
		m_pTxt_Time->SetText(strText);
		m_nPrice = (*items)[itemindex].buy[0].price;
		ACString strPrice;
		if ( CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType() )
			strPrice.Format(_AL("%d"), m_nPrice);
		else
			strPrice = GetGameUIMan()->m_pDlgQShop->GetCashText(m_nPrice);
		strText.Format(GetStringFromTable(1502), strPrice);

		m_pTxt_Price->SetText(strText);
		m_pTxt_ItemName->SetText(pItem->GetName());
		strText.Format(GetStringFromTable(1507), (*items)[itemindex].num);
		m_pTxt_Number->SetText(strText);
		strText.Format(GetStringFromTable(1509), GetGameUIMan()->m_pDlgQShop->GetCashText(((*items)[itemindex].bonus*m_nPrice)/100, false, true));
		m_pTxt_Bonus->SetText(strText);
		ACString strHint;
		strHint = (const wchar_t*)(*items)[itemindex].desc;
		if ((*items)[itemindex].bHasPresent)
		{
			strHint += _AL("\f");
			strTemp.Format(GetGameUIMan()->GetStringFromTable(9900), (*items)[itemindex].szPresentName);
			strHint += strTemp;
			EditBoxItemBase item(enumEIImage);
			item.SetImageIndex(m_nHintImageIndex);
			strHint += (ACHAR)AUICOMMON_ITEM_CODE_START + item.Serialize();
			strTemp.Format(_AL("x%d\r"), (*items)[itemindex].Presentcount);
			strHint += strTemp;
			strHint += (const wchar_t*)(*items)[itemindex].Presentdesc;
		}
		m_pImg_Model->SetHint(strHint);
		GetDlgItem("Img_Discount")->SetHint(strHint);
		GetDlgItem("Img_Cover")->SetHint(strHint);
		GetDlgItem("Img_Suggest")->SetHint(strHint);
		GetDlgItem("Img_Gift")->SetHint(strHint);
		GetDlgItem("Img_LimitSale")->SetHint(strHint);
		GetDlgItem("Img_Gift")->Show((*items)[itemindex].bHasPresent);
		GetDlgItem("Img_LimitSale")->Show(remain_time>0);
		m_pTxt_SaleRemainTime->Show(remain_time>0);
		AUIOBJECT_SETPROPERTY p;
		strcpy(p.fn, (*items)[itemindex].icon + strlen("Surfaces\\"));
		m_pImg_Model->SetProperty("Image File", &p);
		delete pItem;
		int prop = (*items)[itemindex].props;
		PAUILABEL pLab = (PAUILABEL)GetDlgItem("Txt_Discount");
		if ((prop&m_nSaleType) == 0)
			Show(false);
		if (prop&m_nDiscountType)
		{
			int discount = (*items)[itemindex].discount;
			GetDlgItem("Img_Discount")->Show(true);
			strText.Format(GetStringFromTable(1510), GetGameUIMan()->m_pDlgQShop->GetCashText(m_nPrice, false, true));
			m_pTxt_Price->SetText(strText);
			m_pTxt_Bonus->Show(true);
			strText.Format(GetStringFromTable(1511), GetGameUIMan()->m_pDlgQShop->GetCashText(m_nPrice*discount/100, false, true));
			m_pTxt_Bonus->SetText(strText);
			pLab->Show(true);
			if (discount%10 == 0)
				strText.Format(GetGameUIMan()->GetStringFromTable(9110), discount/10);
			else
				strText.Format(GetGameUIMan()->GetStringFromTable(9111), discount/10, discount%10);
			pLab->SetText(strText);
			m_nPrice = m_nPrice * discount / 100;
			m_bDiscount = true;
		}
		else
		{
			GetDlgItem("Img_Discount")->Show(false);
			pLab->Show(false);
			m_bDiscount = false;
		}
		if ((*items)[itemindex].bonus>0 && CDlgQShop::SHOP_TYPE_Q == CDlgQShop::GetShopType())
		{
			GetDlgItem("Img_Cover")->Show(true);
			m_pTxt_Bonus->Show(true);
			GetDlgItem("Img_Cover")->SetHint((const wchar_t*)(*items)[itemindex].desc);
		}
		else
			GetDlgItem("Img_Cover")->Show(false);
		if (prop&0x00000001)
			GetDlgItem("Img_Suggest")->Show(true);
		else
			GetDlgItem("Img_Suggest")->Show(false);

	}
}

void CDlgQShopItem::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if (GetGameUIMan()->m_pDlgQShop->IsShow())
		GetGameUIMan()->m_pDlgQShop->OnEventMouseWheel(wParam, lParam, NULL);
	else
	{
		CDlgQShop* pDlgQShop = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");
		pDlgQShop->OnEventMouseWheel(wParam, lParam, NULL);
	}
}

void CDlgQShopItem::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	vector<GSHOP_ITEM> *items;
	if (CDlgQShop::SHOP_TYPE_Q==CDlgQShop::GetShopType())
	{
		items = globaldata_getgshopitems();
	}
	else if (CDlgQShop::SHOP_TYPE_POP==CDlgQShop::GetShopType())
	{
		items = globaldata_getgshopitems2();
	}
	else if (CDlgQShop::SHOP_TYPE_CROSSSERVER==CDlgQShop::GetShopType())
	{
		items = globaldata_getgshopitems3();
	}
	CECIvtrItem *pItem = CECIvtrItem::CreateItem((*items)[m_nItemIndex].id, 0, (*items)[m_nItemIndex].num);
	if( pItem )
	{
		if( pItem->IsEquipment() )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip*)pItem;
			if( pEquip->GetDBEssence() )
			{
				if (GetGameUIMan()->m_pDlgQShop->IsShow())
					GetGameUIMan()->m_pDlgQShop->ChangeEquip(pEquip->GetDBEssence());
				else
				{
					CDlgQShop* pDlgQShop = (CDlgQShop*)GetGameUIMan()->GetDialog("Win_QShopPop");
					pDlgQShop->ChangeEquip(pEquip->GetDBEssence());
				}
			}
		}
		delete pItem;
	}
}

ACString CDlgQShopItem::GetFormatTime(int nTime)
{
	ACString szTime;
	if( nTime == 0 )
		szTime = GetStringFromTable(1600);
	else if( nTime % (24 * 3600) == 0 )
		szTime.Format(GetStringFromTable(1604), nTime / 24 / 3600);
	else if( nTime % 3600 == 0 )
		szTime.Format(GetStringFromTable(1603), nTime / 3600);
	else if( nTime % 60 == 0 )
		szTime.Format(GetStringFromTable(1602), nTime / 60);
	else
		szTime.Format(GetStringFromTable(1601), nTime);
		
	return szTime;
}
