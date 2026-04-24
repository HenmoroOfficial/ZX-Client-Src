// Filename	:	DlgShopConsign.cpp
// Creator	:	Fu Chonggang
// Date		:	Aug 3, 2010

#include "DlgShopConsign.h"
#include "DlgTeacherMan.h"
#include "AUI\\AUIEditBox.h"
#include "AUI\\AUILabel.h"
#include "AUI\\AUIRadioButton.h"
#include "AUI\\AUIStillImageButton.h"
#include "AUI\\AUIImagePicture.h"
#include "AUI\\AUIComboBox.h"
#include "AUI\\AUICheckBox.h"
#include "AUI\\AUIListBox.h"
#include "AFI.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_GameUIMan.h"
#include "EC_IvtrItem.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_Global.h"
#include "EC_GameSession.h"
#include "EC_Algorithm.h"
#include "EC_TaskInterface.h"
#include "ec_sendc2scmds.h"
#include "roleinfo"
#include "Network\\ids.hxx"

#include "AFileImage.h"
#include "consignquery_re.hpp"
#include "CONSIGNCANCELPOST_RE.hpp"
#include "consignstarterr.hpp"
#include "consignplayer_re.hpp"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgShopConsign, CDlgBase)

AUI_ON_COMMAND("Btn_CancelConsign",		OnCommand_cancelconsign)
AUI_ON_COMMAND("Btn_Clear",				OnCommand_clear)
AUI_ON_COMMAND("Btn_Website",			OnCommand_website)
AUI_ON_COMMAND("Btn_Confirm",			OnCommand_confirm)
AUI_ON_COMMAND("Btn_ListName",			OnCommand_listname)
AUI_ON_COMMAND("Btn_ListTime",			OnCommand_listtime)
AUI_ON_COMMAND("Btn_ListPrice",			OnCommand_listprice)
AUI_ON_COMMAND("Rdo_ConsignItem",		OnCommand_ConsignItem)
AUI_ON_COMMAND("Rdo_ConsignMoney",		OnCommand_ConsignMoney)
AUI_ON_COMMAND("Rdo_ConsignChar",		OnCommand_ConsignChar)
AUI_ON_COMMAND("Rdo_Yes",				OnCommand_EnterName)
AUI_ON_COMMAND("Btn_Refresh",			OnCommand_Refresh)
AUI_ON_COMMAND("Btn_Help",				OnCommand_Help)
AUI_ON_COMMAND("Btn_Close",				OnCommand_CANCEL)
AUI_ON_COMMAND("IDCANCEL",				OnCommand_CANCEL)

//added by Lei Dongyu 2010-10-29
AUI_ON_COMMAND("Rdo_AllList",			OnCommand_AllList)
AUI_ON_COMMAND("Rdo_CharAllList",		OnCommand_CharList)
//added end

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgShopConsign, CDlgBase)

AUI_ON_EVENT("Img_Goods",			WM_LBUTTONDOWN,		OnEventLButtonDown)

AUI_END_EVENT_MAP()

const int CDlgShopConsign::CONSIGN_PRICE = 10;
const int CDlgShopConsign::GOLD_TRADE_MIN = 100;

CDlgShopConsign::CDlgShopConsign()
{
}

CDlgShopConsign::~CDlgShopConsign()
{
}

bool CDlgShopConsign::Release()
{
	return CDlgBase::Release();
}

bool CDlgShopConsign::OnInitDialog()
{
	m_pBtn_CancelConsign = (AUIStillImageButton *)GetDlgItem("Btn_CancelConsign");
	m_pImg_Goods = (AUIImagePicture *)GetDlgItem("Img_Goods");
	m_pImg_Head = (AUIImagePicture *)GetDlgItem("Pic_Charhead");
	m_pLab_Goods = (AUILabel *)GetDlgItem("Lab_Goods");
	m_pLab_Goods->SetText(_AL(""));
	m_pEdt_PriceNum = (AUIEditBox *)GetDlgItem("Edt_PriceNum");
	m_pCombo_Day = (AUIComboBox *)GetDlgItem("Combo_Day");
	m_pEdt_BuyerName = (AUIEditBox *)GetDlgItem("Edt_BuyerName");
	m_pBtn_Confirm = (AUIStillImageButton *)GetDlgItem("Btn_Confirm");
	m_pBtn_ListName = (AUIStillImageButton *)GetDlgItem("Btn_ListName");
	m_pBtn_ListTime = (AUIStillImageButton *)GetDlgItem("Btn_ListTime");
	m_pBtn_ListPrice = (AUIStillImageButton *)GetDlgItem("Btn_ListPrice");
	m_pBtn_ListBuyer = (AUIStillImageButton *)GetDlgItem("Btn_ListBuyer");
	m_pLst_History = (AUIListBox *)GetDlgItem("Lst_History");
	m_pEdt_GoldNum = (AUIEditBox *)GetDlgItem("Edt_GoldNum");
	m_pLab_Gold = (AUILabel *)GetDlgItem("Lab_Gold");
	m_mapItems.clear();
	AFileImage f;
	char szPath[MAX_PATH];
	sprintf(szPath, "%s\\Configs\\consign_item_list.txt", af_GetBaseDir());
	if (f.Open(szPath, AFILE_OPENEXIST | AFILE_TEXT | AFILE_NOHEAD | AFILE_TEMPMEMORY))
	{
		char line[256];
		DWORD  nLen;
		while(f.ReadLine(line, 256, &nLen))
		{
			int nType, nId;
			sscanf(line, "%d,%d", &nType, &nId);
			m_mapItems[nId] = nType;
		}
		f.Close();
	}
	else
	{
		assert(0);
		AUI_ReportError(__LINE__, __FILE__);
	}

	m_nItemIndex = -1;

	CheckRadioButton(1, 2);
	CheckRadioButton(2, 2);

	//added by Lei Dongyu 2010-10-29
	CheckRadioButton(3, 1);
	//added end

	OnCommand_ConsignItem("");
	ACString strText;
	m_pCombo_Day->ResetContent();
	for (int i=1; i<=7; i++)
	{
		strText.Format(GetGameUIMan()->GetStringFromTable(789), i);
		m_pCombo_Day->AddString(strText);
	}
	m_pCombo_Day->SetCurSel(0);

	return true;
}

void CDlgShopConsign::OnShowDialog()
{
	OnCommand_clear("");
	OnCommand_Refresh("");
}

void CDlgShopConsign::OnTick()
{
	if (GetCheckedRadioButton(1)==1)
	{
	}
	else if (GetCheckedRadioButton(1)==2)
	{
		int num = a_atoi(m_pEdt_GoldNum->GetText());
		if (num>200000)
		{
			m_pEdt_GoldNum->SetText(_AL("200000"));
		}
	}
	else if (GetCheckedRadioButton(1)==3)
	{
		m_pImg_Head->SetRenderCallback(PlayerRenderPortrait, (DWORD)GetHostPlayer());
	}
	m_pEdt_BuyerName->Enable(GetCheckedRadioButton(2)==1);
	m_pBtn_CancelConsign->Enable(m_pLst_History->GetCount()>0 && m_pLst_History->GetCurSel()>=0);	
	
	int nCurSel = m_pLst_History->GetCurSel();
	m_pLst_History->ResetContent();
	ACString strText;
	ACString strItem;
	ACString strMoney;
	ACString strInfo;
	for (UINT i=0; i<m_ConsignItems.size(); i++ )
	{
		GConsignItem item = m_ConsignItems[i];
		strInfo = GetStringFromTable(10242+item.state);

		if (item.state==STATE_SELL)
		{
			int iRemainTime = item.sell_endtime - g_pGame->GetServerGMTTime();
			if (iRemainTime > 24*3600)
			{
				strInfo.Format(GetStringFromTable(1610),iRemainTime/(24*3600), (iRemainTime%(24*3600))/3600);
			}
			else if (iRemainTime > 3600)
			{
				strInfo.Format(GetStringFromTable(1611),iRemainTime/3600, (iRemainTime%3600)/60);
			}
			else if (iRemainTime > 60)
			{
				strInfo.Format(GetStringFromTable(1609),iRemainTime/60, (iRemainTime%60));
			}
			else if (iRemainTime > 0)
			{
				strInfo.Format(GetStringFromTable(1601),iRemainTime);
			}
			else
				strInfo = _AL("---");
		}
		if (item.state==STATE_SHOW || item.state==STATE_SELL)
		{
			ACString strTemp;
			if (item.price%100==0)
				strTemp.Format(_AL("%d"), item.price/100);
			else
				strTemp.Format(_AL("%d.%02d"), item.price/100, item.price%100);
			strMoney.Format(GetGameUIMan()->GetStringFromTable(10241), strTemp);
		}
		else
			strMoney = _AL("---");
		if (item.consign_type==2)	// 1:money	2:item
		{
			CECIvtrItem* pItem = CECIvtrItem::CreateItem(item.item_id, 0, 1);
			strItem = pItem->GetName();
			delete pItem;
			if (item.item_cnt>1)
			{
				ACString strPostfix;
				strPostfix.Format(_AL(" x%d"), item.item_cnt);
				strItem += strPostfix;
			}
		}
		else if (item.consign_type==1)
			strItem.Format(GetGameUIMan()->GetStringFromTable(1700), item.money/10000);
		else
			strItem = GetHostPlayer()->GetName();
		strText.Format(_AL("  %s\t%s\t%s"), strItem, strInfo, strMoney);
		m_pLst_History->AddString(strText);
		m_pLst_History->SetItemData64(i, item.sn);
	}
	if (nCurSel>=0 && m_pLst_History->GetCount()>nCurSel)
	{
		m_pLst_History->SetCurSel(nCurSel);
	}
}

bool CDlgShopConsign::Render()
{
	if( !CDlgBase::Render() )
		return false;
	return true;
}

int CDlgShopConsign::CommonMessage(AString strMsg, WPARAM wParam, LPARAM lParam)
{
	if (strMsg == "set_item") // wParam: item pos	lParam: item count
	{
		m_nItemIndex = wParam;
		CECIvtrItem* pItem = GetHostPlayer()->GetPack()->GetItem(wParam);
		if (m_mapItems.find(pItem->GetTemplateID())!= m_mapItems.end())
		{
			if (m_pImg_Goods->GetDataPtr("ptr_CECIvtrItem"))
			{
				CECIvtrItem* pItemOld = (CECIvtrItem*)m_pImg_Goods->GetDataPtr("ptr_CECIvtrItem");
				pItemOld->Freeze(false);
			}
			pItem->Freeze(true);
			GetGameUIMan()->SetIvtrInfo(m_pImg_Goods, pItem, lParam);
			m_pLab_Goods->SetText(pItem->GetName());
			m_pImg_Goods->SetDataPtr(pItem, "ptr_CECIvtrItem");
			m_pImg_Goods->SetData(lParam);
		}
		else
			GetGameUIMan()->MessageBox(10288);
	}
	else if (strMsg == "consign")
	{
		int type = GetCheckedRadioButton(1);// 1:money	2:item	3:char
		int item_id = 0;
		int item_index = 0;
		int item_count = 0;
		int money = 0;
		if (type==1)
		{
			money = a_atoi(m_pEdt_GoldNum->GetText());
		}
		else if (type==2)
		{
			item_id = ((CECIvtrItem*)m_pImg_Goods->GetDataPtr("ptr_CECIvtrItem"))->GetTemplateID();
			item_index = m_nItemIndex;
			item_count = m_pImg_Goods->GetData();
		}
		else if (type==3)
		{
			type = 4;	// 服务器定义寄售角色是4
			item_id = GetGameRun()->GetSpecialIDConfig().id_consign_role_item;
			item_index = GetHostPlayer()->GetPack()->FindItem(GetGameRun()->GetSpecialIDConfig().id_consign_role_item);
			item_count = GetGameRun()->GetSpecialIDConfig().consign_role_item_count;
		}

		int sold_time = 1+m_pCombo_Day->GetCurSel();
		int price = a_atoi(m_pEdt_PriceNum->GetText());
		ACString strName = GetCheckedRadioButton(2)>1 ? _AL("") : m_pEdt_BuyerName->GetText();
		c2s_SendCmdNPCSevOnlineTrade(type, item_id, item_index, item_count, money*10000, sold_time, price, strName);
		OnCommand_clear("");
	}
	else if (strMsg == "query_re")	// wParam : protocol pointer
	{
		m_ConsignItems.clear();
		ConsignQuery_Re* pConsignQueryRe = (ConsignQuery_Re*)wParam;
		for (UINT i=0; i<pConsignQueryRe->items.size(); i++ )
		{
			GConsignItem item = pConsignQueryRe->items[i];
			m_ConsignItems.push_back(item);
		}

	}
	else if (strMsg == "calcel_re")	// wParam : protocol pointer
	{
		OnCommand_Refresh("");
		ConsignCancelPost_Re* pConsignCancelRe = (ConsignCancelPost_Re*)wParam;
		if (pConsignCancelRe->retcode!=0)
		{
			GetGameUIMan()->MessageBox(10290+pConsignCancelRe->retcode);
		}
	}
	else if (strMsg == "server_error")	// wParam : protocol pointer
	{
		OnCommand_Refresh("");
		ConsignStartErr* pConsignError = (ConsignStartErr*)wParam;
		if (pConsignError->retcode!=0)
		{
			GetGameUIMan()->MessageBox(10290+pConsignError->retcode);
		}
	}
	else if (strMsg == "consign_player_re")	// 寄售不成功处理
	{
		ConsignPlayer_Re* p = (ConsignPlayer_Re*)wParam;
		if (p->retcode!=0)
		{
			GetGameUIMan()->MessageBox(10290+p->retcode);
		}
	}
	return 1;
}

void CDlgShopConsign::OnCommand_CANCEL(const char * szCommand)
{
	OnEventLButtonDown(0, 0, m_pImg_Goods);
	Show(false);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
	GetGameUIMan()->EndNPCService();
}

void CDlgShopConsign::OnCommand_cancelconsign(const char * szCommand)
{
	PAUIDIALOG pDlg;
	GetGameUIMan()->MessageBox("Game_ConsignCancel", GetGameUIMan()->GetStringFromTable(10349), MB_YESNO, A3DCOLORRGBA(255,255,255,160), &pDlg);
	pDlg->SetData64(m_pLst_History->GetItemData64(m_pLst_History->GetCurSel()));
}

void CDlgShopConsign::OnCommand_clear(const char * szCommand)
{
	OnEventLButtonDown(0, 0, m_pImg_Goods);
	m_pEdt_GoldNum->SetText(_AL(""));
	m_pEdt_PriceNum->SetText(_AL(""));
	m_pCombo_Day->SetCurSel(0);
	CheckRadioButton(2,2);
	m_pEdt_BuyerName->SetText(_AL(""));

	//added by Lei Dongyu 2010-11-2
	CheckRadioButton(3,1);
	//added end
}

void CDlgShopConsign::OnCommand_website(const char * szCommand)
{
	ShellExecuteA(NULL, "open", AC2AS(GetGameUIMan()->GetStringFromTable(10280)), NULL, NULL, SW_SHOWNORMAL);
}

bool CDlgShopConsign::IsSameUserId(ACString str)
{
	abase::vector<GNET::RoleInfo> vecRoleInfo = GetGameRun()->GetVecRoleInfo();
	abase::vector<GNET::RoleInfo>::iterator it = vecRoleInfo.begin();
	for (; it!=vecRoleInfo.end(); ++it)
	{
		ACString strName((const ACHAR *)it->name.begin(), it->name.size() / sizeof(ACHAR));
		if (strName == str)
		{
			return true;
		}
	}
	return false;
}

void CDlgShopConsign::OnCommand_confirm(const char * szCommand)
{
	int type = GetCheckedRadioButton(1);// 1:money	2:item	3:char
	bool bCanConsign = true;
	ACString strText;
	int price = a_atoi(m_pEdt_PriceNum->GetText());
	if (GetCheckedRadioButton(2)==1 && a_strlen(m_pEdt_BuyerName->GetText())==0)
	{
		GetGameUIMan()->MessageBox(10285);
		return;
	}
	else if (GetCheckedRadioButton(2)==1 && IsSameUserId(m_pEdt_BuyerName->GetText()))
	{
		GetGameUIMan()->MessageBox(10286);
		return;
	}
	if (type!=3)
	{
		if(price<10)			// price smaller than 10
		{
			GetGameUIMan()->MessageBox(10290);
			return;
		}
		else if(price>100000)			// price larger than 100000
		{
			GetGameUIMan()->MessageBox(10287);
			return;
		}
		else if (m_pEdt_GoldNum->IsShow()&&a_atoi(m_pEdt_GoldNum->GetText())<GOLD_TRADE_MIN)	// gold lower limit: 1
		{
			GetGameUIMan()->MessageBox(10348);
			return;
		}
		else if (m_pEdt_GoldNum->IsShow()&&GetHostPlayer()->GetMoneyAmount()<(10000*(a_atoi(m_pEdt_GoldNum->GetText())+a_atoi(m_pEdt_PriceNum->GetText())))) // gold up limit: packet money count
		{
			GetGameUIMan()->MessageBox(656);
			return;
		}
		else if (m_pImg_Goods->IsShow()&&!m_pImg_Goods->GetDataPtr("ptr_CECIvtrItem")) // item not set
		{
			GetGameUIMan()->MessageBox(10289);
			return;
		}
		strText.Format(GetGameUIMan()->GetStringFromTable(10240), CONSIGN_PRICE);
	}
	else
	{
		if(price<60)			// price smaller than 10
		{
			GetGameUIMan()->MessageBox(10350);
			return;
		}
		else if(price>1000000)			// price larger than 100000
		{
			GetGameUIMan()->MessageBox(10351);
			return;
		}
		else if (GetHostPlayer()->GetBasicProps().iLevel<90 || GetHostPlayer()->GetRebornInfo().size()==0)
		{
			GetGameUIMan()->MessageBox(10352);
			return;
		}
		else if (GetHostPlayer()->GetPKValue()!=0)
		{
			GetGameUIMan()->MessageBox(10353);
			return;
		}
		else if (GetHostPlayer()->GetFamilyID() || GetHostPlayer()->GetFactionID())
		{
			GetGameUIMan()->MessageBox(10310);
			return;
		}
		else if (GetHostPlayer()->GetSpouse())
		{
			GetGameUIMan()->MessageBox(10312);
			return;
		}
		else if (m_ConsignItems.size()>0)
		{
			GetGameUIMan()->MessageBox(10355);
			return;
		}
		else if (GetHostPlayer()->GetPack()->GetItemTotalNum(GetGameRun()->GetSpecialIDConfig().id_consign_role_item)
			< GetGameRun()->GetSpecialIDConfig().consign_role_item_count )
		{
			GetGameUIMan()->MessageBox(10356);
			return;
		}
		else if (!GetHostPlayer()->GetTaskInterface()->CheckSpecialTasksForConsign())
		{
			GetGameUIMan()->MessageBox(10391);
			return;
		}

		CDlgTeacherMan *pTeachMan = dynamic_cast<CDlgTeacherMan*>(GetGameUIMan()->GetDialog("Win_TeacherManage"));
		if (pTeachMan->HasTeacher())
		{
			GetGameUIMan()->MessageBox(10311);
			return;
		}
		strText = GetGameUIMan()->GetStringFromTable(10388);
	}

	GetGameUIMan()->MessageBox("Game_Consign", strText, MB_YESNO);
}

//money low -> high	item 
class CompareListByNameAscent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		if (p1.money>0 && p2.money==0)
			return true;
		else if (p1.money==0 && p2.money>0)
			return false;
		else if (p1.money>0 && p2.money>0)
			return p1.money<p2.money;
		else
			return p1.item_id < p2.item_id;
	}
};

class CompareListByNameDescent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		CompareListByNameAscent proxy;
		return !(proxy(p1, p2));
	}
};

void CDlgShopConsign::OnCommand_listname(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByNameAscent());
	else
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByNameDescent());
	bAscent = !bAscent;
}

class CompareListByTimeAscent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		return p1.show_endtime < p2.show_endtime;
	}
};

class CompareListByTimeDescent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		return p1.show_endtime > p2.show_endtime;
	}
};

void CDlgShopConsign::OnCommand_listtime(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByTimeAscent());
	else
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByTimeDescent());
	bAscent = !bAscent;
}

class CompareListByPriceAscent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		return p1.price < p2.price;
	}
};

class CompareListByPriceDescent
{
public:
	bool operator ()(GConsignItem & p1, GConsignItem & p2)
	{
		return p1.price > p2.price;
	}
};

void CDlgShopConsign::OnCommand_listprice(const char * szCommand)
{
	static int bAscent = true;
	if (bAscent)
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByPriceAscent());
	else
		BubbleSort(m_ConsignItems.begin(), m_ConsignItems.end(), CompareListByPriceDescent());
	bAscent = !bAscent;
}

void CDlgShopConsign::OnCommand_ConsignItem(const char * szCommand)
{
	m_pImg_Goods->Show(true);
	m_pLab_Goods->Show(true);
	m_pEdt_GoldNum->Show(false);
	m_pLab_Gold->Show(false);
	GetDlgItem("Shape3")->Show(false);
	GetDlgItem("Lab_Price")->Show(true);
	
	GetDlgItem("Shape_Charhead")->Show(false);
	GetDlgItem("Pic_Charhead")->Show(false);	
	GetDlgItem("Txt_Camp")->Show(false);
	GetDlgItem("Txt_CharName")->Show(false);
	GetDlgItem("Txt_CharProf")->Show(false);
	GetDlgItem("Lab_ConsignCharHint")->Show(false);
}

void CDlgShopConsign::OnCommand_ConsignMoney(const char * szCommand)
{
	m_pImg_Goods->Show(false);
	m_pLab_Goods->Show(false);
	m_pEdt_GoldNum->Show(true);
	m_pLab_Gold->Show(true);
	GetDlgItem("Shape3")->Show(true);
	GetDlgItem("Lab_Price")->Show(true);
	
	GetDlgItem("Shape_Charhead")->Show(false);
	GetDlgItem("Pic_Charhead")->Show(false);	
	GetDlgItem("Txt_Camp")->Show(false);
	GetDlgItem("Txt_CharName")->Show(false);
	GetDlgItem("Txt_CharProf")->Show(false);
	GetDlgItem("Lab_ConsignCharHint")->Show(false);
}

void CDlgShopConsign::OnCommand_ConsignChar(const char * szCommand)
{
	m_pImg_Goods->Show(false);
	m_pLab_Goods->Show(false);
	m_pEdt_GoldNum->Show(false);
	m_pLab_Gold->Show(false);
	GetDlgItem("Shape3")->Show(false);
	GetDlgItem("Lab_Price")->Show(false);
	
	GetDlgItem("Shape_Charhead")->Show(true);
	GetDlgItem("Pic_Charhead")->Show(true);	
	GetDlgItem("Txt_Camp")->Show(true);
	GetDlgItem("Lab_ConsignCharHint")->Show(true);

	PAUILABEL pLabName = (PAUILABEL)GetDlgItem("Txt_CharName");
	pLabName->Show(true);
	pLabName->SetText(GetHostPlayer()->GetName());
	PAUILABEL pLabProf = (PAUILABEL)GetDlgItem("Txt_CharProf");
	pLabProf->Show(true);
	pLabProf->SetText(GetGameUIMan()->GetStringFromTable(PROFESSION_START + GetHostPlayer()->GetProfession()));
	PAUILABEL pLabCult = (PAUILABEL)GetDlgItem("Txt_Camp");
	pLabCult->SetText(GetGameUIMan()->GetStringFromTable(7000 + GetHostPlayer()->GetCultivation()));

	CDlgTeacherMan *pTeachMan = dynamic_cast<CDlgTeacherMan*>(GetGameUIMan()->GetDialog("Win_TeacherManage"));
	pTeachMan->OnCommandRefresh("");
}

void CDlgShopConsign::OnCommand_EnterName(const char * szCommand)
{
	ChangeFocus(m_pEdt_BuyerName);
}

void CDlgShopConsign::OnCommand_Refresh(const char * szCommand)
{
	GetGameSession()->consign_query();
}

void CDlgShopConsign::OnCommand_Help(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ShopConsign_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_ShopConsign_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgShopConsign::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;

	CECIvtrItem* pItem = (CECIvtrItem*)m_pImg_Goods->GetDataPtr("ptr_CECIvtrItem");
	pItem->Freeze(false);
	pObj->SetData(0);
	pObj->SetDataPtr(NULL, "ptr_CECIvtrItem");
	PAUIIMAGEPICTURE pImg = (PAUIIMAGEPICTURE)pObj;
	pImg->ClearCover();
	pImg->SetText(_AL(""));
	pImg->SetHint(_AL(""));
	m_pLab_Goods->SetText(_AL(""));
}

//added by Lei Dongyu 2010-10-29
void CDlgShopConsign::OnCommand_AllList(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ShopConsignView");
	if( pDlg )
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		pDlg->CheckRadioButton(3,2);
		pDlg->Show(true);
	}
}

void CDlgShopConsign::OnCommand_CharList(const char * szCommand)
{
	PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_ShopConsignView");
	if( pDlg )
	{
		Show(false);
		GetGameUIMan()->GetDialog("Win_Inventory")->Show(false);
		pDlg->SetPosEx(GetPos().x, GetPos().y);
		pDlg->CheckRadioButton(3,3);
		pDlg->Show(true);
	}
}

bool CDlgShopConsign::CanBeConsigned(int tid)
{
	return m_mapItems.find(tid)!= m_mapItems.end();
}