// Filename	: DlgInventory.cpp
// Creator	: Tom Zhou
// Date		: October 10, 2005

#include "AFI.h"
#include "AUI\\AUIDef.h"
#include "DlgChat.h"
#include "DlgCharacter.h"
#include "DlgDragDrop.h"
#include "DlgEquipBind.h"
#include "DlgEquipDestroy.h"
#include "DlgEquipUndestroy.h"
#include "DlgInventory.h"
#include "DlgInputNO.h"
#include "DlgResetProp.h"
#include "DlgTextFirework.h"
#include "DlgTrade.h"
#include "DlgShop.h"
#include "DlgSplit.h"
#include "DlgStorage.h"
#include "DlgIdentify.h"
#include "DlgBooth.h"
#include "DlgFaceLift.h"
#include "DlgInstall.h"
#include "DlgTalismanService.h"
#include "DlgPetService.h"
#include "DlgHorseTame.h"
#include "DlgShopExchange.h"
#include "DlgSkill.h"
#include "DlgItemLock.h"
#include "DlgMend.h"
#include "DlgSoul.h"
#include "DlgSoulCombine.h"
#include "DlgInventoryCostume.h"
#include "DlgBodyChange.h"
#include "DlgBook.h"
#include "DlgSystem.h"
#include "DlgMarkAstro.h"
#include "DlgShopConsign.h"
#include "DlgItemSvrBase.h"
#include "DlgRuneOperate.h"
#include "DlgPetDetail.h"
#include "DlgReName.h"
#include "DlgTreasureRadar.h"
#include "DlgFengshenConvert.h" 
#include "DlgFengshenIdentify.h"
#include "DlgFengshenUpgrade.h"
#include "DlgAgentCombined.h"
#include "DlgRemedyMetempsychosis.h "
#include "DlgCalendarPresent.h"
#include "DlgFlower.h"
#include "DlgAttriUp.h"
#include "DlgRoleRune.h"
#include "DlgInventoryExpand.h"
#include "DlgIndividualityColor.h"

#include "EC_Algorithm.h"
#include "EC_GameSession.h"
#include "EC_GameUIMan.h"
#include "EC_Friend.h"
#include "EC_ManPlayer.h"
#include "EC_HostPlayer.h"
#include "EC_Inventory.h"
#include "EC_DealInventory.h"
#include "EC_IvtrItem.h"
#include "EC_IvtrTalismanMain.h"
#include "EC_IvtrTaskItem.h"
#include "EC_Global.h"
#include "EC_Game.h"
#include "EC_GameRun.h"
#include "EC_GameRecord.h"
#include "EC_Resource.h"
#include "EC_IvtrConsume.h"
#include "EC_IvtrStone.h"
#include "EC_IvtrRune.h"
#include "EC_Helper.h"
#include "EC_AutoHelper.h"
#include "LuaEvent.h"
#include "EC_HelpGraphGuide.h"
#include "EC_NewHelpGuide.h"


#define new A_DEBUG_NEW

extern game_record gr;

static const ACHAR EMPTY_STR[] = _AL("");
static const A3DCOLOR WHITE_COLOR = A3DCOLORRGB(255, 255, 255);
static const A3DCOLOR GREY_COLOR = A3DCOLORRGB(128, 128, 128);

static const int ARRANGE_ITEM_MAX_DURATION_SEC = 15; // Max duration of each Arrange
DWORD CDlgInventory::ARRANGE_ITEM_INTERVEL_SEC = 120; // interval of each Arrange
int CDlgInventory::m_nShowIvtrItem = 0;
bool CDlgInventory::m_bArranging = false;

#define  LOCAL_DIR "userdata\\layout"

AUI_BEGIN_COMMAND_MAP(CDlgInventory, CDlgBase)

AUI_ON_COMMAND("choosemoney",		OnCommand_choosemoney)
AUI_ON_COMMAND("normalpack",		OnCommand_Pack)
AUI_ON_COMMAND("taskpack",			OnCommand_Pack)
AUI_ON_COMMAND("alongpack",			OnCommand_Pack)
AUI_ON_COMMAND("materialpack",		OnCommand_Pack)
AUI_ON_COMMAND("fashionpack",		OnCommand_Pack)
AUI_ON_COMMAND("Btn_Vechile",		OnCommand_Pack)
AUI_ON_COMMAND("showinfo",			OnCommand_showinfo)
AUI_ON_COMMAND("booth",				OnCommand_booth)
AUI_ON_COMMAND("split",				OnCommand_split)
AUI_ON_COMMAND("arrange",			OnCommand_AutoArrangeItems)
AUI_ON_COMMAND("Btn_ArrangeAll",	OnCommand_AutoArrangeItems)
AUI_ON_COMMAND("moveall",			OnCommand_MoveAllToPacket)
AUI_ON_COMMAND("help",				OnCommand_PetPocketHelp)
AUI_ON_COMMAND("autoin",			OnCommand_SetAutoIn)
AUI_ON_COMMAND("IDCANCEL",			OnCommand_CANCEL)
AUI_ON_COMMAND("Btn_Convert",		OnCommand_Convert)
AUI_ON_COMMAND("Rdo_Page*",			OnCommand_Page)
AUI_ON_COMMAND("Btn_Expand",		OnCommand_Expand)
AUI_ON_COMMAND("Btn_Switch",        OnCommand_ModeSwitch)

AUI_END_COMMAND_MAP()

AUI_BEGIN_EVENT_MAP(CDlgInventory, CDlgBase)

AUI_ON_EVENT("*",				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT(NULL,				WM_MOUSEWHEEL,		OnEventMouseWheel)
AUI_ON_EVENT("Img_FrameMoney",	WM_LBUTTONDOWN,		OnEventLButtonDownMoney)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDOWN,		OnEventLButtonDown)
AUI_ON_EVENT("Item_*",			WM_LBUTTONDBLCLK,	OnEventLButtonDblclk)
AUI_ON_EVENT("Item_*",			WM_RBUTTONUP,		OnEventLButtonDblclk)

AUI_END_EVENT_MAP()

CDlgInventory::CDlgInventory()
{
	m_nShowIvtrItem = INVENTORY_ITEM_NORMAL;

	m_nLastSendTime = 0;
	m_nSendInterval = 0;
	m_nCurSendNum = -1;
	m_bPageMode = true;
}

CDlgInventory::~CDlgInventory()
{
}

bool CDlgInventory::OnInitDialog()
{
	int i;
	char szItem[40];
	m_pScl_Item = (PAUISCROLL)GetDlgItem("Scl_Item");
	m_pLab_Expand = (PAUILABEL)GetDlgItem("Lab_Expand");
	m_pLab_ExpandLevel15 = (PAUILABEL)GetDlgItem("Lab_Expand1");
	m_pLab_ExpandLevel45 = (PAUILABEL)GetDlgItem("Lab_Expand2");
	m_pLab_ExpandLevel75 = (PAUILABEL)GetDlgItem("Lab_Expand3");
	m_pLab_ExpandLevel105 = (PAUILABEL)GetDlgItem("Lab_Expand4");
	m_pTxt_BagNum = (PAUILABEL)GetDlgItem("Txt_BagNum");
	for( i = 0; i < CECDLGSHOP_PACKMAX; i++ )
	{
		sprintf(szItem, "Item_%02d", i);
		m_pImgItem[i] = (PAUIIMAGEPICTURE)GetDlgItem(szItem);
	}
	if (0==stricmp(m_szName,"Win_InventoryPet"))
	{
		m_pLab_Expand1 = (PAUILABEL)GetDlgItem("Lab_Expand1");
		m_pChk_AutoIn = (PAUICHECKBOX)GetDlgItem("Chk_AutoIn");
	}
	else if (0==stricmp(m_szName,"Win_Inventory"))
	{
		m_pTxt_Money1 = (PAUILABEL)GetDlgItem("Txt_Money1");
		m_pTxt_Money2 = (PAUILABEL)GetDlgItem("Txt_Money2");
		m_pTxt_Money3 = (PAUILABEL)GetDlgItem("Txt_Money3");
		m_pTxt_Money1->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money2->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pTxt_Money3->SetAlign(AUIFRAME_ALIGN_RIGHT);
		m_pBtn_Arrange = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Arrange");
		m_pBtn_ArrangeAll = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_ArrangeAll");
		m_pBtn_Pocket  = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_PetBag");
		m_pBtn_FashionPocket  = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Fash");
		m_pBtn_WingPocket  = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Vechile");
		m_pBtn_Expand = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Expand");
		m_pBtn_Switch = (PAUISTILLIMAGEBUTTON)GetDlgItem("Btn_Switch");
		
		CheckRadioButton(2, 1);
		m_nFirstLine = 0;
		
		ARRANGE_ITEM_INTERVEL_SEC = 60;
		m_pBtn_Arrange->SetHint(GetStringFromTable(3551));
		m_pBtn_ArrangeAll->SetHint(GetStringFromTable(18770));
		m_bArranging = false;
		m_iHelpItemId = -1;

		ACHAR strText[50];
		GetPrivateProfileString(_AL("Mode"), _AL("Inventory"), _AL(""), 
		strText, 50, _AL("userdata\\pagemode.ini"));
		if(a_strcmp(strText, _AL("page")) == 0)
			m_bPageMode = true;
		else if(a_strcmp(strText, _AL("classic")) == 0)
			m_bPageMode = false;
		if(m_bPageMode)
		{
			m_pBtn_Switch->SetText(GetStringFromTable(18821));
			m_pBtn_Switch->SetHint(GetStringFromTable(18823));
		}
		else
		{
			m_pBtn_Switch->SetText(GetStringFromTable(18820));
			m_pBtn_Switch->SetHint(GetStringFromTable(18822));
		}

		char szItem[40];
		for(int i = 0; i < 6; i++)
		{
			sprintf(szItem, "Rdo_Page%02d", i + 1);
			m_pRdo_Page[i] = (PAUIRADIOBUTTON)GetDlgItem(szItem);
		}
		CheckRadioButton(3, 1);

		GetDlgItem("Sub_NewerHelp1")->Show(false);
	}
	m_aItemEmphasize.clear();

	return true;
}

void CDlgInventory::OnShowDialog()
{
	CECIvtrItem* pItem = CECIvtrItem::CreateItem(44823, 0, 1);
	if (pItem)
	{
		delete pItem;
	}
	if (0==stricmp(m_szName,"Win_InventoryPet"))
	{
		m_pChk_AutoIn->Check(g_pGame->GetGameRun()->GetHelper()->GetTransferEnable());
	}
	else if (0==stricmp(m_szName,"Win_Inventory"))
	{
		OnCommand_Pack("normalpack");
		GetDlgItem("Btn_Booth")->Show(GetHostPlayer()->GetSrcServerID()==0);
	}
}


void CDlgInventory::OnCommand_choosemoney(const char * szCommand)
{
	CDlgInputNO *pShow = GetGameUIMan()->m_pDlgInputMoney;
	pShow->Show(true, true);

	PAUIOBJECT pEdit = pShow->GetDlgItem("DEFAULT_Txt_Money1");
	pEdit->SetData(GetHostPlayer()->GetMoneyAmount());

	if( GetGameUIMan()->m_pDlgStorage->IsShow() )
		pShow->SetType(CDlgInputNO::INPUTNO_STORAGE_IVTR_MONEY);
	else if( GetGameUIMan()->m_pDlgTrade->IsShow() )
		pShow->SetType(CDlgInputNO::INPUTNO_TRADE_MONEY);
	else if( m_pAUIManager->GetDialog("Win_MailWrite")->IsShow() )
		pShow->SetType(CDlgInputNO::INPUTNO_ATTACH_MONEY);
	else
		pShow->SetType(CDlgInputNO::INPUTNO_DROP_MONEY);
}

void CDlgInventory::OnCommand_showinfo(const char * szCommand)
{
	if( GetGameUIMan()->m_bReplayMode && !gr.get_replay_setting(REPLAY_SELF_INFO) )
		return;

	if( !GetGameUIMan()->m_pDlgCharacter->IsShow() )
		GetGameSession()->c2s_CmdGetExtProps();

	GetGameUIMan()->m_pDlgCharacter->Show(!GetGameUIMan()->m_pDlgCharacter->IsShow());
}

void CDlgInventory::OnCommand_booth(const char * szCommand)
{
	GetGameUIMan()->m_pDlgCharacter->Show(false);
	if(GetHostPlayer()->GetBoothState() == 0)
	{
		GetHostPlayer()->SetBoothItemID(-1);
		GetHostPlayer()->SetBoothItemSlot(-1);
	}
	GetHostPlayer()->CmdSellBooth();
	LuaEvent::FirstBooth();
}

void CDlgInventory::OnCommand_split(const char * szCommand)
{
	if( GetHostPlayer()->GetPack()->SearchEmpty() == -1 )
		GetGameUIMan()->MessageBox("", GetStringFromTable(989), MB_OK, A3DCOLORRGBA(255, 255, 255, 160));
	else
	{
		GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_SEPERATING;
		GetGame()->ChangeCursor(RES_CUR_SPLIT);
		LuaEvent::FirstSplitItem();
	}
}

void CDlgInventory::OnCommand_Pack(const char * szCommand)
{
	if( strcmp(szCommand, "normalpack") == 0 )
	{
		m_nShowIvtrItem = INVENTORY_ITEM_NORMAL;
		m_pBtn_Pocket->SetPushed(false);
		m_pBtn_FashionPocket->SetPushed(false);
		GetGameUIMan()->GetDialog("Win_Inventory_Costume")->Show(false);
		if (GetHostPlayer()->GetPocketPack()->GetSize()==0)
			GetGameUIMan()->GetDialog("Win_InventoryPet")->Show(false);
		CheckRadioButton(2, 1);
	}
	else if( strcmp(szCommand, "taskpack") == 0 )
	{
		m_nShowIvtrItem = INVENTORY_ITEM_TASK;
		m_pBtn_Pocket->SetPushed(false);
		m_pBtn_FashionPocket->SetPushed(false);
		GetGameUIMan()->GetDialog("Win_Inventory_Costume")->Show(false);
		GetGameUIMan()->GetDialog("Win_Inventory_Vechile")->Show(false);
		if (GetHostPlayer()->GetPocketPack()->GetSize()==0)
			GetGameUIMan()->GetDialog("Win_InventoryPet")->Show(false);
	}
	else if( strcmp(szCommand, "alongpack") == 0 )
	{
		CDlgInventory *pDlg = (CDlgInventory*)GetGameUIMan()->GetDialog("Win_InventoryPet");
		if (!pDlg->IsShow())
		{
			pDlg->Show(true);
			if (m_nShowIvtrItem == INVENTORY_ITEM_TASK)
			{
				m_nShowIvtrItem = INVENTORY_ITEM_NORMAL;
			}
			CheckRadioButton(2, 1);
			m_pBtn_Pocket->SetPushed(true);
			m_pBtn_FashionPocket->SetPushed(false);
			GetGameUIMan()->GetDialog("Win_Inventory_Costume")->Show(false);
			LuaEvent::FirstOpenPetPack();
		}
//		else
//		{
//			pDlg->Show(false);
//			CheckRadioButton(2, 1);
//			m_pBtn_Pocket->SetPushed(false);
//		}
	}
	else if( strcmp(szCommand, "fashionpack") == 0 )
	{
		CDlgInventoryCostume *pDlg = (CDlgInventoryCostume*)GetGameUIMan()->GetDialog("Win_Inventory_Costume");
		if (!pDlg->IsShow())
		{
			pDlg->Show(true);
			if (m_nShowIvtrItem == INVENTORY_ITEM_TASK)
			{
				m_nShowIvtrItem = INVENTORY_ITEM_NORMAL;
			}
			CheckRadioButton(2, 1);
			m_pBtn_Pocket->SetPushed(false);
			m_pBtn_FashionPocket->SetPushed(true);
			GetGameUIMan()->GetDialog("Win_InventoryPet")->Show(false);
			LuaEvent::FirstOpenFashionPack();
		}
//		else
//		{
//			pDlg->Show(false);
//			CheckRadioButton(2, 1);
//			m_pBtn_FashionPocket->SetPushed(false);
//		}
	}
	else if( strcmp(szCommand, "Btn_Vechile") == 0 )
	{
		PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Inventory_Vechile");
		pDlg->Show(!pDlg->IsShow());
		CheckRadioButton(2, 1);
		if (m_nShowIvtrItem == INVENTORY_ITEM_TASK)
		{
			m_nShowIvtrItem = INVENTORY_ITEM_NORMAL;
		}
	}
}

void CDlgInventory::OnCommand_PetPocketHelp(const char * szCommand)
{
	//PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_InventoryPet_Help");
	//pDlg->Show(!pDlg->IsShow());
	AString strDlg = "Win_InventoryPet_Help";
	GetGameUIMan()->CommonMessage("Win_HelpInfo","open",(WPARAM)(&strDlg),0);
}

void CDlgInventory::OnCommand_SetAutoIn(const char * szCommand)
{
	g_pGame->GetGameRun()->GetHelper()->SetTransferEnable(m_pChk_AutoIn->IsChecked());
}

void CDlgInventory::OnCommand_CANCEL(const char * szCommand)
{
	if (0==stricmp(m_szName,"Win_InventoryPet") && GetGameUIMan()->m_pDlgInventory->IsShow())
	{
		Show(false);
		//GetGameUIMan()->GetDialog("Win_InventoryPet_Help")->Show(false);
		return;
	}
	PAUIDIALOG pClose = NULL;

	if( GetGameUIMan()->m_pDlgTrade->IsShow() )
	{
		pClose = GetGameUIMan()->m_pDlgTrade;
		int idTrade = (int)pClose->GetDataPtr("int");
		GetGameSession()->trade_Cancel(idTrade);
	}
	else if( GetGameUIMan()->m_pDlgStorage->IsShow() )
	{
		GetGameSession()->c2s_CmdCancelAction();
		return;
	}
	else
	{
		static const char* dlgToClose[] = {
			"Win_Shop",
			"Win_Enchase",
			"Win_Disenchase1",
			"Win_Disenchase2",
			"Win_PShop2",
			"Win_EquipBind",
			"Win_EquipDestroy",
			"Win_EquipUndestroy",
			"Win_TalismanClean",
			"Win_TalismanCombine",
			"Win_TalismanRefine",
			"Win_TalismanUpgrade",
			"Win_TalismanEnchanting",
			"Win_ItemLock",
			"Win_ItemUnlock",
			"Win_HorseTame",
			"Win_ShopExchange",
			"Win_Mend",
			"Win_MendMoney",
			"Win_EquipLvup",
			"Win_Equip_JewelUp",
			"Win_MarkAstro",
			"Win_ShopConsign",
			"Win_EquipDepart",
			"Win_EquipEnchase",
			"Win_EquipOwn",
			"Win_EquipResume",
			"Win_Spilt",
			"Win_Accessory_Drill",
			"Win_Accessory_ReDrill",
			"Win_Accessory_Custom",
			"Win_Accessory_Attach",
			"Win_Accessory_Detach",
			"Win_Accessory_Upgrade",
			"Win_Accessory_Refine",
			"Win_Accessory_Spilt",
			"Win_Accessory_Move",
			"Win_Shop_Credit",	
            "Win_Equip_ChangeName",
		};
		for (int i=0; i<sizeof(dlgToClose)/sizeof(char*); ++i)
		{
			if(GetGameUIMan()->GetDialog(dlgToClose[i]) && GetGameUIMan()->GetDialog(dlgToClose[i])->IsShow())
				pClose = GetGameUIMan()->GetDialog(dlgToClose[i]);
		}

		if( pClose ) GetGameUIMan()->EndNPCService();
	}

	Show(false);
	GetGameUIMan()->m_nCursorState = CECGameUIMan::CURSOR_STATE_NORMAL;
	NewHelpGuide::FinishGuide(GUIDE_HELP_EQUIPWEAPON_3);
	if( pClose )
	{
		pClose->OnCommand("IDCANCEL");
		GetGameUIMan()->UnfreezeAllItems();
	}
}

void CDlgInventory::OnCommand_Convert(const char * szCommand)
{
	GetGameUIMan()->m_pDlgFengshenConvert->Show(true);
}

void CDlgInventory::OnCommand_Page(const char *szCommand)
{
	if(0 == stricmp(m_szName,"Win_Inventory"))
	{
		int nPage = GetCheckedRadioButton(3) - 1;
		m_pScl_Item->SetBarLevel(nPage * 8);
	}	
}

void CDlgInventory::OnCommand_Expand(const char * szCommand)
{
	bool bCrsServer = (GetHostPlayer()->GetSrcServerID() != 0);
	if(bCrsServer)
	{
		GetGameUIMan()->MessageBox("", GetStringFromTable(18706), MB_OK,
			A3DCOLORRGBA(255, 255, 255, 160));
		return;
	}
	
	CDlgInventoryExPand* pDlgInventoryExpand = (CDlgInventoryExPand*)GetGameUIMan()->GetDialog("Win_Inventory_Expand02");
	bool bShow = pDlgInventoryExpand->IsShow();
	pDlgInventoryExpand->Show(!bShow);
	if(bShow)
		pDlgInventoryExpand->OnCommandCancel("");
}

void CDlgInventory::OnCommand_ModeSwitch(const char * szCommand)
{
	if(0 == stricmp(m_szName, "Win_Inventory"))
	{
		m_bPageMode = !m_bPageMode;
		m_pScl_Item->SetBarLevel(0);
		ACString strText;
		if(m_bPageMode)
		{
		
			m_pBtn_Switch->SetText(GetStringFromTable(18821));
			m_pBtn_Switch->SetHint(GetStringFromTable(18823));
			CheckRadioButton(3,1);
			strText = _AL("page");
			WritePrivateProfileString(_AL("Mode"), _AL("Inventory"), strText, 
				_AL("userdata\\pagemode.ini"));
		}
		else
		{
			m_pBtn_Switch->SetText(GetStringFromTable(18820));
			m_pBtn_Switch->SetHint(GetStringFromTable(18822));
			strText = _AL("classic");
			WritePrivateProfileString(_AL("Mode"), _AL("Inventory"), strText, 
				_AL("userdata\\pagemode.ini"));
		}
	}
}

void CDlgInventory::SetPage(int iPage)
{
	if(m_bPageMode)
	{	
		CheckRadioButton(3, iPage+ 1);
		m_pScl_Item->SetBarLevel(iPage * 8);
	}
}

int CDlgInventory::GetShowItem()
{
	return m_nShowIvtrItem;
}

void CDlgInventory::SetShowItem(int nShowItem)
{
	m_nShowIvtrItem = nShowItem;
}

bool CDlgInventory::Tick(void)
{
	UpdateInventory();

	if ( 0==stricmp(m_szName,"Win_Inventory") )
	{
		CECGameUIMan* pGameUI = GetGameUIMan();
		
		// Update Arrange
		if (!m_aExchanges2Send.empty() &&
			GetTickCount() > m_nLastSendTime + ARRANGE_ITEM_MAX_DURATION_SEC * 1000)
		{
			// Force Stop arrange send
			GetGameRun()->AddChatMessage(pGameUI->GetStringFromTable(3555), GP_CHAT_MISC);
			
			m_nCurSendNum = (int)m_aExchanges2Send.size();
			UpdateArrangeSend();
		}
		
		m_pBtn_Arrange->Show(m_bPageMode);
		m_pBtn_Arrange->Enable(
			GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_ARRANGE_INVENTORY) <= 0 &&
			!pGameUI->m_pDlgTrade->IsShow() && // Disable during Trade
			!GetHostPlayer()->IsProducing() && // Disable during Produce
			!GetHostPlayer()->IsPicking() && // Disable during Picking
			(!pGameUI->m_pCurNPCEssence || pGameUI->m_pDlgStorage->IsShow()) && // Disable during service (Except storage service)
			GetHostPlayer()->GetBoothState() == 0
			);

		m_pBtn_ArrangeAll->Enable(
			GetHostPlayer()->GetCoolTime(COOLDOWN_INDEX_ARRANGE_INVENTORY) <= 0 &&
			!pGameUI->m_pDlgTrade->IsShow() && // Disable during Trade
			!GetHostPlayer()->IsProducing() && // Disable during Produce
			!GetHostPlayer()->IsPicking() && // Disable during Picking
			(!pGameUI->m_pCurNPCEssence || pGameUI->m_pDlgStorage->IsShow()) && // Disable during service (Except storage service)
			GetHostPlayer()->GetBoothState() == 0
			);
		
		m_pBtn_Pocket->Enable( !(GetGameUIMan()->m_pDlgTrade->IsShow() ||
								GetGameUIMan()->m_pDlgBooth1->IsShow()) );
		m_pBtn_FashionPocket->Enable( !(GetGameUIMan()->m_pDlgTrade->IsShow()  ||
										GetGameUIMan()->m_pDlgBooth1->IsShow() ||
										GetGameUIMan()->m_pDlgShop->IsShow()) );
		m_pBtn_WingPocket->Enable( !(GetGameUIMan()->m_pDlgTrade->IsShow()  ||
			GetGameUIMan()->m_pDlgBooth1->IsShow() ||
										GetGameUIMan()->m_pDlgShop->IsShow()) );
		m_pBtn_Expand->Enable(GetHostPlayer()->GetPack()->GetSize() < CECINVENTORY_SIZEMAX &&
			!GetGameUIMan()->m_pDlgTrade->IsShow() &&
			GetHostPlayer()->GetBoothState() == 0);	
		m_pBtn_Switch->Enable(!GetGameUIMan()->m_pDlgTrade->IsShow() && GetHostPlayer()->GetBoothState() == 0);

		if (m_iHelpItemId>0)
		{
			int index = GetHostPlayer()->GetPack()->FindItem(m_iHelpItemId);
			if (index>=0 && index>=m_nFirstLine*CECDLGSHOP_PACKLINEMAX && index<(m_nFirstLine*CECDLGSHOP_PACKLINEMAX+CECDLGSHOP_PACKMAX))
			{
				PAUIOBJECT pObj = m_pImgItem[index-m_nFirstLine*CECDLGSHOP_PACKLINEMAX];
				GetDlgItem("Sub_NewerHelp1")->SetPos(pObj->GetPos(true).x, pObj->GetPos(true).y);
				GetDlgItem("Sub_NewerHelp1")->Show(true);
			}
			else
			{
				GetDlgItem("Sub_NewerHelp1")->Show(false);
			}
		}
		else
			GetDlgItem("Sub_NewerHelp1")->Show(false);
	}

	return CDlgBase::Tick();
}

void CDlgInventory::OnItemExchanged(int iSlot1, int iSlot2)
{
	if (m_aExchanges2Send.empty() || m_nCurSendNum < 0)
		return;

	if (m_nCurSendNum <= (int)m_aExchanges2Send.size()) {
		int iSrc = m_aExchanges2Send[m_nCurSendNum - 1].iSrc;
		int iDst = m_aExchanges2Send[m_nCurSendNum - 1].iDst;
		if (iSrc == iSlot1 && iDst == iSlot2)
		{
			UpdateArrangeSend();
		}
		else {
			ASSERT(0);
		}
	}
}

void CDlgInventory::UpdateArrangeSend()
{
	if (m_aExchanges2Send.empty() || m_nCurSendNum < 0)
		return;

	if (m_nCurSendNum >= (int)m_aExchanges2Send.size())
	{
		// All Arrange Exchanges sent. Unfreeze Items that Originally not frozen
		CECHostPlayer* pHost = GetHostPlayer();
		CECInventory* pPack = pHost->GetPack();
		for (size_t i = 0; i < m_aPackItemFrozen.size(); ++i) {
			CECIvtrItem* pItem = pPack->GetItem(i);
			if (pItem && !m_aPackItemFrozen[i])
				pItem->Freeze(false);
		}

		m_bArranging = false;

#ifdef _DEBUG
		CreateDirectoryA(LOCAL_DIR, NULL);
		AString sub_dir;
		sub_dir.Format("%s\\%d", LOCAL_DIR, pHost->GetCharacterID());
		CreateDirectoryA(sub_dir, NULL);
		AString file_name;
		file_name.Format("%s\\ArrangeIvtr.txt", sub_dir);
		AFile afile;
		if (afile.Open(file_name, AFILE_OPENAPPEND|AFILE_TEXT))
		{
			afile.WriteLine("\nAfter Arrange: \n");
			int i;
			char sz[16];
			AString str;
			for (i = 0; i < pPack->GetSize(); ++i)
			{
				CECIvtrItem* pItem = pPack->GetItem(i);
				sprintf(sz, "%8d ", pItem ? pItem->GetTemplateID() : 0);
				str += sz;
				if ((i+1) % 6 == 0) {
					afile.WriteLine(str);
					str.Empty();
				}
			}
		}
		afile.Close();
#endif

		// Reset variables
		m_nSendInterval = 0;
		m_nCurSendNum = -1;
		m_aExchanges2Send.clear();
	}
	else
	{
		// Send one exchange.
		GetGameSession()->c2s_CmdExgIvtrItem(
			m_aExchanges2Send[m_nCurSendNum].iSrc, 
			m_aExchanges2Send[m_nCurSendNum].iDst);
		++m_nCurSendNum;
		m_nLastSendTime = GetTickCount();
	}
}

bool CDlgInventory::UpdateInventory()
{
	int i, nMax;
	AString strFile;
	ACHAR szText[40];
	AUIClockIcon *pClock;
	PAUIIMAGEPICTURE pObj;
	CECInventory *pInventory;
	CECIvtrItem *pItem;

	int nIvtrSize = 0;
	if( 0==stricmp(m_szName,"Win_InventoryPet") )
	{
		pInventory = GetHostPlayer()->GetPocketPack();
		nIvtrSize = pInventory->GetSize();
		m_pLab_Expand->Show( nIvtrSize <= 36 );
		m_pLab_Expand1->Show( nIvtrSize <= 36 );
	}
	else if( 0==stricmp(m_szName,"Win_Inventory") )
	{
		m_pLab_Expand->Show(false);
		m_pLab_ExpandLevel15->Show(false);
		m_pLab_ExpandLevel45->Show(false);
		m_pLab_ExpandLevel75->Show(false);
		m_pLab_ExpandLevel105->Show(false);

		for(i = 0; i < 6; i++)
		{
			m_pRdo_Page[i]->Show(false);
		}
		if(m_nShowIvtrItem == INVENTORY_ITEM_NORMAL && m_bPageMode)
			m_pRdo_Page[0]->Show(true);

		if( m_nShowIvtrItem == INVENTORY_ITEM_NORMAL )
		{
			pInventory = GetHostPlayer()->GetPack();
			nIvtrSize = pInventory->GetSize();
			if (nIvtrSize<=42)
			{
				m_pLab_ExpandLevel105->Show(true);
			}
			if (nIvtrSize<=36)
				m_pLab_ExpandLevel75->Show(true);
			if (nIvtrSize<=30)
				m_pLab_ExpandLevel45->Show(true);
			if (nIvtrSize<=24)
				m_pLab_ExpandLevel15->Show(true);
		}
		else if( m_nShowIvtrItem == INVENTORY_ITEM_TASK )
		{
			pInventory = GetHostPlayer()->GetTaskPack();
			nIvtrSize = pInventory->GetSize();
		}
	}

	a_sprintf(szText, _AL("%d/%d"), pInventory->GetEmptySlotNum(), nIvtrSize);
	m_pTxt_BagNum->SetText(szText);
	if( m_nShowIvtrItem == INVENTORY_ITEM_TASK )
		m_pTxt_BagNum->Show(false);
	else
		m_pTxt_BagNum->Show(true);
	
	if( nIvtrSize > CECDLGSHOP_PACKMAX )
	{
		if(m_nShowIvtrItem == INVENTORY_ITEM_NORMAL &&  0 == stricmp(m_szName,"Win_Inventory") && m_bPageMode)
		{
			int nRange = (nIvtrSize - 1) / CECDLGSHOP_PACKMAX * 8;
			m_pScl_Item->SetScrollRange(0, nRange);
			m_pScl_Item->Show(false);
			m_pScl_Item->SetBarLength(-1);
			m_pScl_Item->SetScrollStep(1);

			for(i = 0; i <= (nIvtrSize - 1) / CECDLGSHOP_PACKMAX; i++)
			{
				m_pRdo_Page[i]->Show(true);
			}
		}
		else
		{
			m_pScl_Item->SetScrollRange(0, (nIvtrSize - 1) / CECDLGSHOP_PACKLINEMAX - 
				CECDLGSHOP_PACKMAX / CECDLGSHOP_PACKLINEMAX + 1);
			m_pScl_Item->Show(true);
			m_pScl_Item->SetBarLength(-1);
			m_pScl_Item->SetScrollStep(1);
		}
	}
	else
	{
		m_pScl_Item->SetScrollRange(0, 0);
		m_pScl_Item->Show(false);
	}

	if( m_nFirstLine != m_pScl_Item->GetBarLevel() )
		m_nFirstLine = m_pScl_Item->GetBarLevel();
	int nStart = m_nFirstLine * CECDLGSHOP_PACKLINEMAX;

	for( i = 0; i < CECDLGSHOP_PACKMAX; i++ )
	{
		pObj = m_pImgItem[i];
		pClock = pObj->GetClockIcon();
		pClock->SetProgressRange(0, 1);
		pClock->SetProgressPos(1);
		pObj->SetCover(NULL, -1, 1);

		if( i + nStart < nIvtrSize )
		{
			pObj->Show(true);
			if (pItem = pInventory->GetItem(i + nStart))
			{

				if( pItem->GetClassID() == CECIvtrItem::ICID_TASKDICE && 
					(((CECIvtrTaskDice*)pItem)->GetDBEssence()->type == 2 || ((CECIvtrTaskDice*)pItem)->GetDBEssence()->type == 3))
				{
                     if(((CECIvtrTaskDice*)pItem)->IsUsing())
						 pItem->Freeze(true);
				}
				pObj->SetDataPtr(pItem,"ptr_CECIvtrItem");
				pObj->SetData(CECGameUIMan::ICONS_INVENTORY);
				if( pItem->GetCount() > 1 )
				{
					a_sprintf(szText, _AL("%d"), pItem->GetCount());
					pObj->SetText(szText);
				}
				else
					pObj->SetText(_AL(""));

				af_GetFileTitle(pItem->GetIconFile(), strFile);
				strFile.MakeLower();
				if(GetGameUIMan()->GetDialog("Win_ShopConsign")->IsShow())
					ItemForConsign(pObj, pItem);
				else if(GetGameUIMan()->GetDialog("Win_Accessory_Upgrade")->IsShow())
					ItemForBijouUpgrade(pObj, pItem);
				else
				{
					pObj->SetColor(GetGameUIMan()->GetItemColor(pItem));
				}
				if( pItem->GetCoolTime(&nMax) > 0 )
				{
					pClock->SetProgressRange(0, nMax);
					pClock->SetProgressPos(nMax - pItem->GetCoolTime());
					pClock->SetColor(A3DCOLORRGBA(0, 0, 0, 128));
				}
				pObj->SetCover(GetGameUIMan()->m_pA2DSpriteIcons[CECGameUIMan::ICONS_INVENTORY],
					GetGameUIMan()->m_IconMap[CECGameUIMan::ICONS_INVENTORY][strFile]);
				
				pObj->SetCover(NULL, 0, 1);
				if (pItem->IsLocked() && pItem->GetTemplateID()!= 21695)// 摧毁中的物品不显示锁，暂时写死
				{
					pObj->SetCover(CDlgItemLock::m_pA2DSpriteLock, 0, 1);
				}
				
				if( ( pItem->GetClassID()==CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pItem)->GetEssence().num_estone>=9 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_TALISMAN_MAINPART && ((CECIvtrTalismanMain*)pItem)->GetRebornCount()>0 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_PET_BEDGE && ((CECPetBedge*)pItem)->GetEssence().reborn_cnt>0 ) ||
					( pItem->GetClassID()==CECIvtrItem::ICID_RUNE && ((CECIvtrRune*)pItem)->GetEssence().quality >= 3) )
				{
					if (pItem->IsLocked())
					{
						pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquipLock, (GetTickCount() / 200) % 6, 1);
					}
					else
					{
						pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSpecEquip, (GetTickCount() / 200) % 6, 1);
					}
				}

				for (UINT j=0; j<m_aItemEmphasize.size(); j++)
				{
					if ((i + nStart)==m_aItemEmphasize[j])
					{
						pObj->SetCover(CDlgSkill::m_pA2DSpriteSkillSelect, (GetTickCount() / 200) % 7, 1);
					}
				}
			}
			else
			{
				pObj->SetText(_AL(""));
				pObj->SetDataPtr(NULL);
				pObj->SetCover(NULL, -1);
				pObj->SetColor(A3DCOLORRGB(255, 255, 255));
			}
		}
		else
		{
			pObj->Show(m_nShowIvtrItem == INVENTORY_ITEM_TASK);
			pObj->SetText(_AL(""));
			pObj->SetDataPtr(NULL);
			pObj->SetCover(NULL, -1);
			pObj->SetColor(A3DCOLORRGB(255, 255, 255));
		}
	}

	if (0==stricmp(m_szName,"Win_Inventory"))
	{
		int nMoney = GetHostPlayer()->GetMoneyAmount();
		ACHAR szMoney[20];
		a_sprintf(szMoney, _AL("%d"), nMoney % 100);
		m_pTxt_Money3->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), (nMoney % 10000) / 100);
		m_pTxt_Money2->SetText(szMoney);
		a_sprintf(szMoney, _AL("%d"), nMoney / 10000);
		m_pTxt_Money1->SetText(szMoney);
	}
	
	return true;
}

void CDlgInventory::ItemForConsign(PAUIOBJECT pObj, CECIvtrItem* pItem)
{	
	if(!CanConsign(pItem))
	{
		pObj->SetColor(A3DCOLORRGB(192,0,0));
	}
	else
	{
		pObj->SetColor(A3DCOLORRGB(255,255,255));
	}
}

bool CDlgInventory::CanConsign(CECIvtrItem* pItem)
{
	CDlgShopConsign* pDlgTemp = (CDlgShopConsign*)GetGameUIMan()->GetDialog("Win_ShopConsign");
	return pItem->GetExpireDate()==0 && pDlgTemp->CanBeConsigned(pItem->GetTemplateID()) && pItem->IsTradeable();
}

void CDlgInventory::ItemForBijouUpgrade(PAUIOBJECT pObj, CECIvtrItem* pItem)
{
	if( pItem->IsFrozen() )
	{
		pObj->SetColor(A3DCOLORRGB(128, 128, 128));
	}
	else if( pItem->GetClassID()==CECIvtrItem::ICID_GEM || pItem->GetClassID()==CECIvtrItem::ICID_GEM_DUST )
	{
		pObj->SetColor(A3DCOLORRGB(255,255,255));
	}
	else
	{
		pObj->SetColor(A3DCOLORRGB(192,0,0));
	}
}

void CDlgInventory::OnEventMouseWheel(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	int x = GET_X_LPARAM(lParam);
	int y = GET_Y_LPARAM(lParam);
	int zDelta = (short)HIWORD(wParam);
	if( zDelta > 0 )
		zDelta = 1;
	else
		zDelta = -1;

	if(m_bPageMode)
	{
		int iPage;
		if(m_szName=="Win_Inventory" && m_nShowIvtrItem == INVENTORY_ITEM_NORMAL)
		{
			if(zDelta > 0)
			{
				m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - 8);			
			}
			else
			{
				m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() + 8);
			}
			iPage = m_pScl_Item->GetBarLevel() / 8;
			if(iPage >=0 && iPage <=5)
				CheckRadioButton(3, iPage + 1);
		}
	}
	else
	{
		if( m_pScl_Item->IsShow() )
			m_pScl_Item->SetBarLevel(m_pScl_Item->GetBarLevel() - zDelta);
	}
}

void CDlgInventory::OnEventLButtonDownMoney(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	OnCommand_choosemoney("");
}

void CDlgInventory::OnEventLButtonDown(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") )
		return;
	if( m_nShowIvtrItem == CDlgInventory::INVENTORY_ITEM_TASK )
		return;

	CECIvtrItem *pItem = (CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem");

	if( GetGameUIMan()->m_nCursorState == CECGameUIMan::CURSOR_STATE_SEPERATING )
	{
		if( pItem->IsFrozen() )
			return;
		
		WORD wSlot = (WORD)atoi(pObj->GetName() + strlen("Item_")) + GetFirstItem();
		if( pItem->GetPileLimit() > 1 && pItem->GetCount() > 1 )
		{
			int nMax = pItem->GetCount() - 1;
			if( GetGameUIMan()->m_pDlgTrade->IsShow() && 0 == stricmp(GetName(), "Win_InventoryPet"))
				GetGameUIMan()->InvokeNumberDialog(pObj, NULL, CDlgInputNO::INPUTNO_TRADE_MOVE, nMax);
			else if ( 0 == stricmp(GetName(), "Win_Inventory") )
				GetGameUIMan()->InvokeNumberDialog(pObj, NULL, CDlgInputNO::INPUTNO_MOVE_ITEM, nMax);
			else if ( 0 == stricmp(GetName(), "Win_InventoryPet") )
				GetGameUIMan()->InvokeNumberDialog(pObj, NULL, CDlgInputNO::INPUTNO_PETINVO_MOVE_ITEMS, nMax);
		}
	}
	else if( AUI_PRESS(VK_CONTROL) || AUI_PRESS(VK_SHIFT) )
	{
		if( pItem->VerifyInfoMD5() )
		{
			PAUIDIALOG pChat = GetGameUIMan()->m_pDlgChat;
			PAUIEDITBOX pEdit = (PAUIEDITBOX)pChat->GetDlgItem("DEFAULT_Txt_Speech");
			ACString strName;
			A3DCOLOR color;
			CDlgChat::TransformItemColorName(pItem, strName, color);
			WORD wSlot = (WORD)atoi(pObj->GetName() + strlen("Item_")) + GetFirstItem();
			if (GetGameUIMan()->GetDialog("Win_ChatSpec")->IsShow())
			{
				pChat = GetGameUIMan()->GetDialog("Win_ChatSpec");
				pEdit = (PAUIEDITBOX)pChat->GetDlgItem("Txt_Content");
			}
			if( pEdit->GetItemCount(enumEIIvtrlItem) < 1 && pEdit->GetItemCount(enumEITask) < 1 )
			{
				ACString strInfo(strName);
				int index;
				while ( (index=strInfo.Find(_AL('>')))>=0 )
				{
					strInfo[index] = _AL('＞');
				}
				if (pEdit->AppendItem(enumEIIvtrlItem, color, strName, strInfo))
				{
					if (m_szName=="Win_Inventory")
						pEdit->SetIvtrItemLocInfo(IVTRTYPE_PACK, wSlot);
					else
						pEdit->SetIvtrItemLocInfo(IVTRTYPE_POCKETPACK, wSlot);
				}
			}
			GetGameUIMan()->BringWindowToTop(pChat);
			pChat->ChangeFocus(pEdit);
		}
	}
	else if( !pItem->IsFrozen() )
	{
		A3DVIEWPORTPARAM *p = m_pA3DEngine->GetActiveViewport()->GetParam();

		GetGameUIMan()->m_ptLButtonDown.x = GET_X_LPARAM(lParam) - p->X;
		GetGameUIMan()->m_ptLButtonDown.y = GET_Y_LPARAM(lParam) - p->Y;
		GetGameUIMan()->InvokeDragDrop(this, pObj, GetGameUIMan()->m_ptLButtonDown);
		GetGameUIMan()->PlayItemSound((CECIvtrItem *)pObj->GetDataPtr("ptr_CECIvtrItem"), false);
		GetGameUIMan()->m_pDlgDragDrop->SetData(GetFirstItem());

		if(NewHelpGuide::CheckInventoryItemId(pItem->GetTemplateID(),ESTEP_ITEM_EQUIPTALISMAN_4_3))
			NewHelpGuide::StartGuide_Step_4(GUIDE_HELP_EQUIPTALISMAN_4);
	}
}

void CDlgInventory::OnEventLButtonDblclk(WPARAM wParam, LPARAM lParam, AUIObject *pObj)
{
	if( !strstr(pObj->GetName(), "Item_") )
		return;
	if( !pObj->GetDataPtr("ptr_CECIvtrItem") || GetGameUIMan()->m_nCursorState != CECGameUIMan::CURSOR_STATE_NORMAL )
		return;
	if( m_nShowIvtrItem == CDlgInventory::INVENTORY_ITEM_TASK )
		return;

	CECIvtrItem *pIvtrSrc = (CECIvtrItem*)pObj->GetDataPtr("ptr_CECIvtrItem");
	if( pIvtrSrc->IsFrozen() )
	{
		if(pIvtrSrc && pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TASKDICE)
		{
			int type = ((CECIvtrTaskDice*)pIvtrSrc)->GetDBEssence()->type;
			if(type == 2 || type == 3)
			{
				CDlgTreasureRadar* pTreasureRadar = (type == 2) 
					                                ? (CDlgTreasureRadar*)(GetGameUIMan()->GetDialog("Win_DragonCampass")) 
													: (CDlgTreasureRadar*)(GetGameUIMan()->GetDialog("Win_ZongziCampass"));
		        int iSrc = atoi(pObj->GetName() + strlen("Item_"));
		        pTreasureRadar->CheckRadarState(pIvtrSrc,GetFirstItem() + iSrc);
			}
		}
        return;
	}
	
	GetGameRun()->GetHelpGraphGuide()->UseItem(pIvtrSrc->GetTemplateID());

	NewHelpGuide::TriggerClickInventoryItem(pIvtrSrc->GetTemplateID());
	
	int iSrc = atoi(pObj->GetName() + strlen("Item_"));
	int i;

	static const char* dlgToDrop[] = { 
		"Win_Accessory_Drill",
		"Win_Accessory_ReDrill",
		"Win_Accessory_Custom",
		"Win_Accessory_Attach",
		"Win_Accessory_Detach",
		"Win_Accessory_Upgrade",
		"Win_Accessory_Refine",
		"Win_Accessory_Spilt",
		"Win_Accessory_Move",
		"Win_Pet_Tame",
		"Win_Pet_Release",
		"Win_Pet_EquipEnhanse",
		"Win_Pet_ChangeBone",
		"Win_Pet_ChangeFight",
		"Win_Pet_ChangeHoroscope",
		"Win_Pet_ChangeImage",
		"Win_Pet_ChangeLife",
		"Win_Pet_ChangeNature",
		"Win_Pet_ChangePrelife",
		"Win_Pet_ChangeSkill",
		"Win_Pet_UpNum",
		"Win_Pet_ChangeBasicType",
		"Win_Pet_ExpandSkill",
		"Win_PetLevelUp",
	};
		
	CDlgItemSvrBase* pDlgItemSvr = NULL;
	for (i=0; i<sizeof(dlgToDrop)/sizeof(char*); ++i)
	{
		if(GetGameUIMan()->GetDialog(dlgToDrop[i]) && GetGameUIMan()->GetDialog(dlgToDrop[i])->IsShow())
		{
			pDlgItemSvr = dynamic_cast<CDlgItemSvrBase*>(GetGameUIMan()->GetDialog(dlgToDrop[i]));
			break;
		}
	}

	if(pDlgItemSvr)
	{
		pDlgItemSvr->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Pet_Detail")->IsShow())
	{
		if (pIvtrSrc->GetClassID()==CECIvtrItem::ICID_PET_BEDGE)
		{
			CDlgPetDetailOperate *pDlg = (CDlgPetDetailOperate*)GetGameUIMan()->GetDialog("Win_Pet_Detail");
			pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
		}
	}
	else if ( 0 == stricmp(GetName(), "Win_InventoryPet") )
	{
		if( GetGameUIMan()->m_pDlgInventory->IsShow() )
		{
			CECInventory *pInventory = GetHostPlayer()->GetPack();
			if( pIvtrSrc->GetPileLimit() > 1 )
			{
				int i;
				int nCount = pIvtrSrc->GetCount();
				for(i = 0; i < pInventory->GetSize(); i++ )
				{
					CECIvtrItem *pItem = pInventory->GetItem(i);
					if( pItem && pItem->GetTemplateID() == pIvtrSrc->GetTemplateID() &&
						pItem->GetCount() < pItem->GetPileLimit() )
					{
						int nMove = min(pItem->GetPileLimit() - pItem->GetCount(), nCount);
						GetGameSession()->c2s_CmdMovePocketToIvtr(GetFirstItem() + iSrc, i, nMove);
						nCount -= nMove;
						if( nCount == 0 )
							return;
					}
				}
			}
			int iDst = GetHostPlayer()->GetPack()->SearchEmpty();
			if( iDst != -1 )
				GetGameSession()->c2s_CmdExgPocketIvtrItem(GetFirstItem() + iSrc, iDst);
		}
		return;
	}
	else if ( GetGameUIMan()->GetDialog("Win_ShopConsign")->IsShow() )
	{
		if( !CanConsign(pIvtrSrc) )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(652), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if (GetGameUIMan()->GetDialog("Win_ShopConsign")->GetDlgItem("Img_Goods")->IsShow())
		{
			if (pIvtrSrc->GetCount()==1)
				GetGameUIMan()->CommonMessage("Win_ShopConsign", "set_item", GetFirstItem() + iSrc, 1);
			else
				GetGameUIMan()->InvokeNumberDialog(pObj, GetGameUIMan()->GetDialog("Win_ShopConsign")->GetDlgItem("Img_Goods"),
													CDlgInputNO::INPUTNO_COSIGN, pIvtrSrc->GetCount());
		}
	}
	else if( GetGameUIMan()->m_pDlgShop->IsShow() )
	{
		if( !pIvtrSrc->IsSellable() )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(812), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if (pIvtrSrc->IsLocked())
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
		}
		else
		{
			CECDealInventory *pSell = GetHostPlayer()->GetSellPack();
			if( pSell->SearchEmpty() >= 0 )
			{
				m_pObjSell = pObj;
				pIvtrSrc->Freeze(true);
				if (GetGame()->IsValuableItem(pIvtrSrc->GetTemplateID()))
				{
					GetGameUIMan()->MessageBox("Game_WorthyItem",GetStringFromTable(7300), MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160));
				}
				else
				{
					pSell->AddItem(pIvtrSrc, GetFirstItem() + iSrc, pIvtrSrc->GetCount());
					GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
				}
			}
		}
	}
	else if( GetGameUIMan()->m_pDlgTrade->IsShow() )
	{
		if( !pIvtrSrc->IsTradeable() )
		{
			GetGameUIMan()->MessageBox("", GetStringFromTable(652), MB_OK,
				A3DCOLORRGBA(255, 255, 255, 160));
		}
		else if( pIvtrSrc->IsLocked() )
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
		}
		else
		{
			int idItem = pIvtrSrc->GetTemplateID();
			int nCount = pIvtrSrc->GetCount();
			GetGameSession()->trade_AddGoods(GetGameUIMan()->m_idTrade, idItem, GetFirstItem() + iSrc, nCount, 0);
			pIvtrSrc->Freeze(true);
		}
	}
	else if( GetGameUIMan()->m_pDlgStorage->IsShow() )
	{
		CECInventory *pTrashBox = GetHostPlayer()->GetTrashBox();
		if( !pIvtrSrc->CanMoveToTrash())
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(8461), GP_CHAT_MISC);
			return;
		}
		if( pIvtrSrc->GetPileLimit() > 1 )
		{
			int i;
			int nCount = pIvtrSrc->GetCount();
			for(i = 0; i < pTrashBox->GetSize(); i++ )
			{
				CECIvtrItem *pItem = pTrashBox->GetItem(i);
				if( pItem && pItem->GetTemplateID() == pIvtrSrc->GetTemplateID() && pItem->GetStateFlag() == pIvtrSrc->GetStateFlag()
					&& pItem->GetCount() < pItem->GetPileLimit() )
				{
					int nMove = min(pItem->GetPileLimit() - pItem->GetCount(), nCount);
					GetGameSession()->c2s_CmdMoveIvtrToTrashBox(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), i, GetFirstItem() + iSrc, nMove);
					nCount -= nMove;
					if( nCount == 0 )
						return;
				}
			}
		}
		int iDst = pTrashBox->SearchEmpty();
		if( iDst != -1 )
			GetGameSession()->c2s_CmdExgTrashBoxIvtrItem(GetGameUIMan()->m_pDlgStorage->IsFactionStorage(), iDst, GetFirstItem() + iSrc);
	}
	else if( GetGameUIMan()->GetDialog("Win_InventoryPet")->IsShow()  )
	{

		CECInventory *pPetBox = GetHostPlayer()->GetPocketPack();
		if( g_pGame->IsPocketableItem(pIvtrSrc->GetTemplateID()) )
		{
			if( pIvtrSrc->GetPileLimit() > 1 )
			{
				int i;
				int nCount = pIvtrSrc->GetCount();
				for(i = 0; i < pPetBox->GetSize(); i++ )
				{
					CECIvtrItem *pItem = pPetBox->GetItem(i);
					if( pItem && pItem->GetTemplateID() == pIvtrSrc->GetTemplateID() &&
						pItem->GetCount() < pItem->GetPileLimit() )
					{
						int nMove = min(pItem->GetPileLimit() - pItem->GetCount(), nCount);
						GetGameSession()->c2s_CmdMoveIvtrToPocket( i, GetFirstItem() + iSrc, nMove);
						nCount -= nMove;
						if( nCount == 0 )
							return;
					}
				}
			}
			int iDst = pPetBox->SearchEmpty();
			if( iDst != -1 )
				GetGameSession()->c2s_CmdExgPocketIvtrItem(iDst, GetFirstItem() + iSrc);
		}
		else
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7325), GP_CHAT_MISC);
	}
	else if( GetGameUIMan()->m_pDlgEquipBind->IsShow()  )
	{
		if( pIvtrSrc->IsEquipment() && 
			(pIvtrSrc->GetStateFlag() == 0) )
			GetGameUIMan()->m_pDlgEquipBind->SetEquip(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if( GetGameUIMan()->m_pDlgMend->IsShow()  )
	{
		const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
		if (pIvtrSrc->GetTemplateID() == cfg.id_damaged_item)
		{
			CDlgMend *pDlgMend = GetGameUIMan()->m_pDlgMend;
			pDlgMend->SetBrokeItem(pIvtrSrc, GetFirstItem() + iSrc);
		}
		else if (pIvtrSrc->GetTemplateID() == cfg.id_repair_damaged_item)
		{
			CDlgMend *pDlgMend = GetGameUIMan()->m_pDlgMend;
			pDlgMend->SetRepairItem(pIvtrSrc, GetFirstItem() + iSrc);
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_MendMoney")->IsShow()  )
	{
		const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
		if (pIvtrSrc->GetTemplateID() == cfg.id_damaged_item)
		{
			CDlgMend *pDlgMend = (CDlgMend*)GetGameUIMan()->GetDialog("Win_MendMoney");
			pDlgMend->SetBrokeItem(pIvtrSrc, GetFirstItem() + iSrc);
		}
	}
	else if( GetGameUIMan()->m_pDlgEquipDestroy->IsShow() )
	{
		if( pIvtrSrc->IsLocked() )
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
		}
		else if( pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED )
			GetGameUIMan()->m_pDlgEquipDestroy->SetEquip(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if( GetGameUIMan()->m_pDlgEquipUndestroy->IsShow() )
	{
		if( pIvtrSrc->IsLocked() )
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_DESTROYINGESSENCE )
			GetGameUIMan()->m_pDlgEquipUndestroy->SetEquip(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if( GetGameUIMan()->m_pDlgResetSkillPart->IsShow() )
	{
		GetGameUIMan()->m_pDlgResetSkillPart->SetScroll(pIvtrSrc);
	}
	else if( GetGameUIMan()->m_pDlgResetSkillAll->IsShow() )
	{
		GetGameUIMan()->m_pDlgResetSkillAll->SetScroll(pIvtrSrc);
	}
	else if( GetGameUIMan()->m_pDlgResetTalent->IsShow() )
	{
		GetGameUIMan()->m_pDlgResetTalent->SetScroll(pIvtrSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Char_ChangeName")->IsShow())
	{
		CDlgReName *pDlg = (CDlgReName*)GetGameUIMan()->GetDialog("Win_Char_ChangeName");
		pDlg->SetItem(GetFirstItem()+iSrc, pIvtrSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Guild_ChangeName")->IsShow())
	{
		CDlgReName *pDlg = (CDlgReName*)GetGameUIMan()->GetDialog("Win_Guild_ChangeName");
		pDlg->SetItem(GetFirstItem()+iSrc, pIvtrSrc);
	}
	else if( GetGameUIMan()->m_pDlgHorseTame->IsShow()  )
	{
		if( pIvtrSrc->IsLocked() )
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7035), GP_CHAT_MISC);
		}
		else
		{
			if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_VEHICLE )
				GetGameUIMan()->m_pDlgHorseTame->DragDropItem(iSrc + GetFirstItem(), GetGameUIMan()->m_pDlgHorseTame->GetDlgItem("Item_Horse"));
			else
				GetGameUIMan()->m_pDlgHorseTame->DragDropItem(iSrc + GetFirstItem(), GetGameUIMan()->m_pDlgHorseTame->GetDlgItem("Item_Assistant"));
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_ItemLock")->IsShow()  )
	{
		if (pIvtrSrc->IsLocked() || pIvtrSrc->GetExpireDate()>0)
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7036), GP_CHAT_MISC);
		}
		else if (pIvtrSrc->GetPileLimit()==1)
		{
			const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
			if (pIvtrSrc->GetTemplateID() != cfg.id_destroying_matter)
			{
				CDlgItemLock *pDlgLock = (CDlgItemLock*)GetGameUIMan()->GetDialog("Win_ItemLock");
				pDlgLock->SetItemLock(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_ItemUnlock")->IsShow()  )
	{
		if (!(pIvtrSrc->IsLocked()))
		{
			GetGameUIMan()->AddChatMessage(GetStringFromTable(7037), GP_CHAT_MISC);
		}
		else if ( pIvtrSrc->GetPileLimit()==1 )
		{
			CDlgItemLock *pDlgLock = (CDlgItemLock*)GetGameUIMan()->GetDialog("Win_ItemUnlock");
			pDlgLock->SetItemUnLock(pIvtrSrc, GetFirstItem() + iSrc);
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_EquipOwn")->IsShow()  )
	{
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) pIvtrSrc;
			if (pEquip->GetDBEssence()->blood_sacrifice && 0==pEquip->GetNewEssence().master_id)
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipOwn");
				pDlgSoul->SetItemToOwn(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_EquipEnchase")->IsShow()  )
	{
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) pIvtrSrc;
			if (pEquip->GetNewEssence().master_id)
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipEnchase");
				pDlgSoul->SetItemEnchaseEquip(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
		else if ( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP_SOUL )
		{
			CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipEnchase");
			pDlgSoul->SetItemEnchaseSoul(pIvtrSrc, GetFirstItem() + iSrc);
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_Spilt")->IsShow()  )
	{
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) pIvtrSrc;
			if (pEquip->GetNewEssence().master_id)
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_Spilt");
				pDlgSoul->SetItemSplit(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_EquipResume")->IsShow()  )
	{
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) pIvtrSrc;
			if (pEquip->GetNewEssence().master_id)
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipResume");
				pDlgSoul->SetItemResume(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_EquipDepart")->IsShow()  )
	{
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP )
		{
			CECIvtrEquip *pEquip = (CECIvtrEquip *) pIvtrSrc;
			if (pEquip->GetNewEssence().master_id && !(pIvtrSrc->IsLocked()))
			{
				CDlgSoul *pDlgSoul = (CDlgSoul*)GetGameUIMan()->GetDialog("Win_EquipDepart");
				pDlgSoul->SetItemDepart(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_BodyChange_Enchanting")->IsShow()  )
	{
		CDlgBodyChange* pDlgBodyChange = (CDlgBodyChange*)GetGameUIMan()->GetDialog("Win_BodyChange_Enchanting");
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_CHANGE_SHAPE_CARD )
		{
			pDlgBodyChange->DragDropItem(GetFirstItem() + iSrc,pDlgBodyChange->GetDlgItem("Item_a"));
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_CHANGE_SHAPE_STONE )
		{
			CECIvtrChangeShapeStone *pStone = (CECIvtrChangeShapeStone*)pIvtrSrc;
			if (pStone->GetDBEssence()->refine_type == (int)pDlgBodyChange->GetData())
				pDlgBodyChange->DragDropItem(GetFirstItem() + iSrc,pDlgBodyChange->GetDlgItem("Item_d"));			
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_BodyChange_Recover")->IsShow()  )
	{
		CDlgBodyChange* pDlgBodyChange = (CDlgBodyChange*)GetGameUIMan()->GetDialog("Win_BodyChange_Recover");
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_CHANGE_SHAPE_CARD )
		{
			pDlgBodyChange->DragDropItem(GetFirstItem() + iSrc,pDlgBodyChange->GetDlgItem("Item_a"));			
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_MarkAstro")->IsShow()  )
	{
		CDlgMarkAstro* pDlg = (CDlgMarkAstro*)GetGameUIMan()->GetDialog("Win_MarkAstro");
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TELEPORTATION )
		{
			CECIvtrTelePortation* pTelePort = (CECIvtrTelePortation*)pIvtrSrc;
			if (pTelePort->GetEssence().ess.time==0)
			{
				GetGameUIMan()->MessageBox(9467);
			}
			else
				pDlg->DragDropItem(GetFirstItem() + iSrc,pDlg->GetDlgItem("Item_a"));			
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TELEPORTATION_STONE )
		{
			pDlg->DragDropItem(GetFirstItem() + iSrc,pDlg->GetDlgItem("Item_d"));			
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_EquipLvup")->IsShow()  )
	{
		if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_UPGRADE_EQUIP_STONE)
		{
			GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_helper", GetFirstItem() + iSrc, 0);
		}
		else if (GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_equip", GetFirstItem() + iSrc, 0))
		{
			GetGameUIMan()->CommonMessage("Win_EquipLvup", "set_props", GetFirstItem() + iSrc, 0);
		}
	}
	else if( GetGameUIMan()->GetDialog("Win_Equip_JewelUp")->IsShow() )
	{
	    if (GetGameUIMan()->CommonMessage("Win_Equip_JewelUp", "set_equip", GetFirstItem() + iSrc, 0))
		{
			GetGameUIMan()->CommonMessage("Win_Equip_JewelUp", "set_props", GetFirstItem() + iSrc, 0);
		}
	}
	else if(GetGameUIMan()->GetDialog("Win_Equip_ChangeName")->IsShow())
	{
	    if(GetGameUIMan()->CommonMessage("Win_Equip_ChangeName","set_equip",GetFirstItem() + iSrc,0))
		{
		   GetGameUIMan()->CommonMessage("Win_Equip_ChangeName","set_props",GetFirstItem() + iSrc,0);
		}
	}
	else if ( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_SCROLL_UNLOCK )
	{
		CECIvtrScrollUnlockItem* pUnlockItem = (CECIvtrScrollUnlockItem*)pIvtrSrc;
		c2s_SendCmdUnlockTreasureRegion(pIvtrSrc->GetTemplateID(), GetFirstItem() + iSrc, 
			pUnlockItem->GetDBEssence()->region_id, pUnlockItem->GetDBEssence()->region_pos);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Identify")->IsShow() && pIvtrSrc->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		CDlgRuneIdentify *pDlg = (CDlgRuneIdentify*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Identify");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade")->IsShow() && pIvtrSrc->GetClassID()==CECIvtrItem::ICID_RUNE)
	{
		CDlgRuneLevelUp *pDlg = (CDlgRuneLevelUp*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Upgrade");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh")->IsShow() && pIvtrSrc->GetClassID()!=CECIvtrItem::ICID_RUNE)
	{
		CDlgRuneRefine *pDlg = (CDlgRuneRefine*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Refresh");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn")->IsShow() && pIvtrSrc->GetClassID()==CECIvtrItem::ICID_RUNE_SLOT)
	{
		CDlgRuneFillSlot *pDlg = (CDlgRuneFillSlot*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_RuneOn");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default")->IsShow() && pIvtrSrc->GetClassID()!=CECIvtrItem::ICID_RUNE_SLOT)
	{
		CDlgRuneReset *pDlg = (CDlgRuneReset*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Default");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase")->IsShow() && pIvtrSrc->GetClassID()!=CECIvtrItem::ICID_RUNE_SLOT)
	{
		CDlgSlotErase *pDlg = (CDlgSlotErase*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_HoleErase");
		pDlg->SetItem(pIvtrSrc, GetFirstItem() + iSrc);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Drill")->IsShow())
	{
		if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP)
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Drill", "set_equip", GetFirstItem() + iSrc, 0);
		else
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Drill", "set_props", GetFirstItem() + iSrc, 0);
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Attach")->IsShow())
	{
		if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP)
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Attach", "set_equip", GetFirstItem() + iSrc, 0);
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_ASTROLOGY)
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Attach", "set_props", GetFirstItem() + iSrc, 0);
	
	}
	else if (GetGameUIMan()->GetDialog("Win_Equip_Fengshen_Erase")->IsShow() && pIvtrSrc->GetClassID()==CECIvtrItem::ICID_EQUIP)
	{
			GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Erase", "set_equip", GetFirstItem() + iSrc, 0);
	}
	else if(GetGameUIMan()->GetDialog("Win_DrugEnchanting")->IsShow())
	{
		CDlgAgentCombined* pAgentCombined = (CDlgAgentCombined*)(GetGameUIMan()->GetDialog("Win_DrugEnchanting"));
		if(pAgentCombined)
		{
			if(!pAgentCombined->IsItemIDEffective(pIvtrSrc->GetTemplateID()))
			{
				GetGameUIMan()->MessageBox(17000);
			}
			else
			{
				pIvtrSrc->Freeze(true);
				pAgentCombined->IsAgentEffective(pIvtrSrc, GetFirstItem() + iSrc);
			}
		}
	}
	else if(GetGameUIMan()->GetDialog("Win_Character_Attrimakeup")->IsShow())	// 2012-10-17 添加弥补飞升界面
	{
		DlgRemedyMetempsychosis* pRemedyMetempsychosis = (DlgRemedyMetempsychosis*)(GetGameUIMan()->GetDialog("Win_Character_Attrimakeup"));
		if(pRemedyMetempsychosis)
		{
			int iError = pRemedyMetempsychosis->CheckRemedyMetempsychosisItem(pIvtrSrc->GetTemplateID(),pIvtrSrc->GetCount());
			if(iError == 2)
			{
				GetGameUIMan()->MessageBox(17005);
			}
			else if(iError == 3)
			{
				GetGameUIMan()->MessageBox(17003);
			}
			else if(iError == 1)
			{
				pIvtrSrc->Freeze(true);
				pRemedyMetempsychosis->SetRemedyMetempsychosisItem(pIvtrSrc,GetFirstItem() + iSrc);
			}
		}
	}
	else if (GetGameUIMan()->GetDialog("Win_AttriUp_Enchase")->IsShow())
	{
		GetGameUIMan()->CommonMessage("Win_AttriUp_Enchase", "set_forge", GetFirstItem() + iSrc, 0);
	}
	else
	{
		if (!(pIvtrSrc->IsLocked()))
		{
			CDlgInstall *pDlgInstall = NULL;
			if( GetGameUIMan()->m_pDlgInstall->IsShow() )
				pDlgInstall = GetGameUIMan()->m_pDlgInstall;
			else if( GetGameUIMan()->m_pDlgUninstall1->IsShow() )
				pDlgInstall = GetGameUIMan()->m_pDlgUninstall1;
			else if( GetGameUIMan()->m_pDlgUninstall2->IsShow() )
				pDlgInstall = GetGameUIMan()->m_pDlgUninstall2;
			if( pDlgInstall )
			{
				if( pIvtrSrc->IsEquipment() )
					pDlgInstall->DragDropItem(iSrc + GetFirstItem(), pDlgInstall->GetDlgItem("Item_a"));
				else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_ESTONE || 
					pIvtrSrc->GetClassID() == CECIvtrItem::ICID_SSTONE ||
					pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PSTONE )
					pDlgInstall->DragDropItem(iSrc + GetFirstItem(), pDlgInstall->GetDlgItem("Item_b"));
				else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_REFINETICKET )
					pDlgInstall->DragDropItem(iSrc + GetFirstItem(), pDlgInstall->GetDlgItem("Item_c"));
				return;
			}
		}
		const SPECIAL_ID_CONFIG& cfg = g_pGame->GetGameRun()->GetSpecialIDConfig();
		
		CDlgTalismanService *pDlgTasliman = NULL;
		if( GetGameUIMan()->m_pDlgTalismanRefine->IsShow() )
			pDlgTasliman = GetGameUIMan()->m_pDlgTalismanRefine;
		else if( GetGameUIMan()->m_pDlgTalismanUpgrade->IsShow() )
			pDlgTasliman = GetGameUIMan()->m_pDlgTalismanUpgrade;
		else if( GetGameUIMan()->m_pDlgTalismanCombine->IsShow() )
			pDlgTasliman = GetGameUIMan()->m_pDlgTalismanCombine;
		else if( GetGameUIMan()->m_pDlgTalismanEnchanting->IsShow() )
			pDlgTasliman = GetGameUIMan()->m_pDlgTalismanEnchanting;
		else if( GetGameUIMan()->GetDialog("Win_TalismanLevelUp")->IsShow() )
			pDlgTasliman = (CDlgTalismanService*)GetGameUIMan()->GetDialog("Win_TalismanLevelUp");
		else if( GetGameUIMan()->GetDialog("Win_TalismanCombineSkill")->IsShow() )
			pDlgTasliman = (CDlgTalismanService*)GetGameUIMan()->GetDialog("Win_TalismanCombineSkill");
		else if( GetGameUIMan()->GetDialog("Win_TalismanEnchaseSkill")->IsShow() )
			pDlgTasliman = (CDlgTalismanService*)GetGameUIMan()->GetDialog("Win_TalismanEnchaseSkill");
		else if (!(pIvtrSrc->IsLocked()))
		{
			if( GetGameUIMan()->m_pDlgTalismanClean->IsShow() )
				pDlgTasliman = GetGameUIMan()->m_pDlgTalismanClean;
		}
		if( pDlgTasliman )
		{
			if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MAINPART )
			{
				if( pDlgTasliman->GetDlgItem("Item_a")->GetDataPtr("ptr_CECIvtrItem") != NULL &&
					pDlgTasliman->GetDlgItem("Item_b") )
					pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_b"));
				else
					pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_a"));
			}
			else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_MERGEKATALYST )
				pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_d"));
			else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TALISMAN_EXPFOOD )
				pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_c"));
			else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP && (((CECIvtrEquip*)pIvtrSrc)->GetDBEssence()->equip_mask&EQUIP_MASK_SUB_TALISMAN1) )
				pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_e"));
			else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP && (((CECIvtrEquip*)pIvtrSrc)->GetDBEssence()->equip_mask&EQUIP_MASK_SUB_TALISMAN2) )
				pDlgTasliman->DragDropItem(iSrc + GetFirstItem(), pDlgTasliman->GetDlgItem("Item_f"));
			return;
		}

		if ( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_CHANGE_SHAPE_CARD && 
			GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)pIvtrSrc) && 
			((CECIvtrChangeShapeCard*)pIvtrSrc)->GetEssence().ess.master_id == 0)
		{
			PAUIDIALOG pMsgBox;
			GetGameUIMan()->MessageBox("Game_EquipBind2", GetStringFromTable(8588), 
				MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
			pMsgBox->SetData(GetFirstItem() + iSrc);
				return;
		}
		if( pIvtrSrc->IsEquipment() && GetHostPlayer()->CanUseEquipment((CECIvtrEquip*)pIvtrSrc) )
		{
			if( ((CECIvtrEquip*)pIvtrSrc)->IsTalismanMain() && 
				((CECIvtrTalismanMain*)pIvtrSrc)->GetEssence().ess.master_id == 0 )
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_EquipBind2", GetStringFromTable(873), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(GetFirstItem() + iSrc);
				return;
			}
			else if( ((CECIvtrEquip*)pIvtrSrc)->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP )
			{
				DlgIndividualityReveal* pDlgReveal = (DlgIndividualityReveal*)GetGameUIMan()->GetDialog("Win_Dye_FashDress");
				if (pDlgReveal && pDlgReveal->IsShow() == true)
				{
					pDlgReveal->SetNameToPosItem("Item_00",pIvtrSrc,0,0,GetFirstItem() + iSrc);
					return;
				}
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_EquipBind2", GetStringFromTable(811), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(GetFirstItem() + iSrc);
				return;
			}
			else if ( ((CECIvtrEquip*)pIvtrSrc)->IsRune() && ((CECIvtrRune*)pIvtrSrc)->GetEssence().level==0 )
			{
				CDlgRuneIdentify *pDlg = (CDlgRuneIdentify*)GetGameUIMan()->GetDialog("Win_Equip_Yuanying_Identify");
				pDlg->SetRune(GetFirstItem() + iSrc);
				return;
			}
		}
		else if( !pIvtrSrc->IsEquipment() && (pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDAFTEREQUIP) &&
			!(pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED) && (pIvtrSrc->GetClassID() != CECIvtrItem::ICID_GEM) )
		{
			if ( pIvtrSrc->GetTemplateID() != cfg.id_destroying_matter)
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_ItemBind", GetStringFromTable(922), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(GetFirstItem() + iSrc);
			}
			return;
		}
		if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_LOTTERY )
		{
			CECIvtrLottery *pLottery = (CECIvtrLottery*)pIvtrSrc;
			PAUIDIALOG pDlg;
			if ( 0 == pLottery->GetDBEssence()->type )
			{
				pDlg = GetGameUIMan()->GetDialog("Win_Lottery");
			}
			else if ( 1 == pLottery->GetDBEssence()->type )
			{
				pDlg = GetGameUIMan()->GetDialog("Win_Lottery2");
			}
			else
			{
				GetGameUIMan()->MessageBox("", _AL("抽奖后不直接发物品彩票不能是葫芦式彩票"));
				return;
			}
			if( pDlg->GetData() != 9999 )
			{
				CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(pDlg->GetData());
				if( pItem )
					pItem->Freeze(false);
			}
			pDlg->SetData(GetFirstItem() + iSrc);
			pDlg->Show(true);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_LOTTERY2 ||
			pIvtrSrc->GetClassID() == CECIvtrItem::ICID_LOTTERY3)
		{
			PAUIDIALOG pDlg;
			if ( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_LOTTERY2 )
			{
				CECIvtrLottery2 *pLottery = (CECIvtrLottery2*)pIvtrSrc;
				if (pLottery->GetDBEssence()->type==1)
					pDlg = GetGameUIMan()->GetDialog("Win_Lottery2");
				else
					pDlg = GetGameUIMan()->GetDialog("Win_Lottery3");
			}
			else
			{
				CECIvtrLottery3 *pLottery = (CECIvtrLottery3*)pIvtrSrc;
				if (pLottery->GetDBEssence()->type==1)
					pDlg = GetGameUIMan()->GetDialog("Win_Lottery2");
				else
					pDlg = GetGameUIMan()->GetDialog("Win_Lottery3");
			}

			if( pDlg->GetData() != 9999 )
			{
				CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(pDlg->GetData());
				if( pItem )
					pItem->Freeze(false);
			}
			pDlg->SetData(GetFirstItem() + iSrc);
			pDlg->Show(true);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_GIFT_BAG_LOTTERY_DELIVER )
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Lottery4");
			if( pDlg->GetData() != 9999 )
			{
				CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(pDlg->GetData());
				if( pItem )
					pItem->Freeze(false);
			}
			pDlg->SetData(GetFirstItem()+iSrc); 
			pDlg->Show(true);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_LOTTERY_TANGYUAN )
		{
			PAUIDIALOG pDlg = GetGameUIMan()->GetDialog("Win_Lottery5");
			if( pDlg->GetData() != 9999 )
			{
				CECIvtrItem *pItem = GetHostPlayer()->GetPack()->GetItem(pDlg->GetData());
				if( pItem )
					pItem->Freeze(false);
			}
			pDlg->SetData(GetFirstItem()+iSrc); 
			pDlg->Show(true);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_BOOK )
		{
			GetGameUIMan()->m_pDlgBook->SetBookInfo(GetFirstItem() + iSrc, (CECIvtrBook*)pIvtrSrc);
		}
		else if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TEXT_FIREWORK )
		{
			GetGameUIMan()->m_pDlgTextFirework->Show(true);
			GetGameUIMan()->m_pDlgTextFirework->SetFirework(GetFirstItem() + iSrc);
		}
		else if(pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TELEPORTATION)
		{
			CECIvtrTelePortation* pTelePort = (CECIvtrTelePortation*)pIvtrSrc;
			if (pTelePort->GetEssence().ess.time==0)
			{
				GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetFirstItem() + iSrc);
			}

			if (pTelePort->GetEssence().ess.time!=0 && pTelePort->GetEssence().ess.time <= g_pGame->GetServerGMTTime())
			{
				GetGameUIMan()->MessageBox(9466);
				return;
			}
			GetGameUIMan()->CommonMessage("Win_MarkAstroList", "show_mark", GetFirstItem() + iSrc, (long)pIvtrSrc);
			
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_TASKDICE)
		{
		     CDlgTreasureRadar* pTreasureRadar = (CDlgTreasureRadar*)(GetGameUIMan()->GetDialog("Win_DragonCampass"));
			 int type = ((CECIvtrTaskDice*)pIvtrSrc)->GetDBEssence()->type;
			 if(type == 1)
			 {
                 pTreasureRadar->UseTaskRectItem(pIvtrSrc,GetFirstItem() + iSrc);
			 }
			 else if(type == 2 || type == 3)
			 {
                  if(type == 3)  pTreasureRadar = (CDlgTreasureRadar*)(GetGameUIMan()->GetDialog("Win_ZongziCampass"));
                  pTreasureRadar->UpdateRadarState(pIvtrSrc,GetFirstItem() + iSrc);
			 }
			 else
			 {
                  GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetFirstItem() + iSrc);
			 }
		     
		} 
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_ASTROLOGY)
		{
			if ( ((CECIvtrAstrology*)pIvtrSrc)->GetEssence().level < 0 )
				GetGameUIMan()->m_pDlgFengshenIdentify->SetAstrology(iSrc);
			else
			{
				if (GetGameUIMan()->m_pDlgFengshenConvert->IsShow())
					GetGameUIMan()->CommonMessage("Win_Equip_Fengshen_Convert", "set_equip", GetFirstItem() + iSrc, (long)pIvtrSrc);
				else
					GetGameUIMan()->m_pDlgFengshenUpgrade->SetAstrology(iSrc);
			}
			return;
		}
		else if(pIvtrSrc->GetClassID() == CECIvtrItem::ICID_AGENTCOMBINED)// 2012-10-16  添加 add by zy
		{
			if(!(pIvtrSrc->GetStateFlag() & ITEM_STATE_BINDED))
			{
				PAUIDIALOG pMsgBox;
				GetGameUIMan()->MessageBox("Game_AgentItemBind", GetStringFromTable(922), 
					MB_YESNO, A3DCOLORRGBA(255, 255, 255, 160), &pMsgBox);
				pMsgBox->SetData(GetFirstItem() + iSrc);
				return;
			}
			else
			{
				CDlgAgentCombined* pAgentCombined = (CDlgAgentCombined*)(GetGameUIMan()->GetDialog("Win_DrugEnchanting"));
				if(pAgentCombined)
				{
					pIvtrSrc->Freeze(true);
					pAgentCombined->SetAgentCombiendType(pIvtrSrc,GetFirstItem() + iSrc);
					pAgentCombined->Show(true);
				}
			}
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_SALE_PROMOTION_ITEM)
		{
			CDlgCalendarPresent* pDlg = (CDlgCalendarPresent*)GetGameUIMan()->GetDialog("Win_CalendarPresent");
			if (pDlg)
				pDlg->OpenSalePromotionItem(GetFirstItem() + iSrc, pIvtrSrc->GetTemplateID());
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PROP_ADD_MATERIAL_ITEM)
		{
			CDlgAttriUp* pDlg = (CDlgAttriUp*)GetGameUIMan()->GetDialog("Win_AttriUp");
			if (!pDlg->IsShow())
				pDlg->Show(true);
			GetGameUIMan()->CommonMessage("Win_AttriUp", "set_material", GetFirstItem() + iSrc, (long)pIvtrSrc);
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_PROP_ADD_ITEM)
		{
			CDlgAttriUp* pDlg = (CDlgAttriUp*)GetGameUIMan()->GetDialog("Win_AttriUp");
			if (!pDlg->IsShow())
				pDlg->Show(true);
			GetGameUIMan()->CommonMessage("Win_AttriUp", "set_propadd_item", GetFirstItem() + iSrc, (long)pIvtrSrc);
		}
		else if(pIvtrSrc->GetTemplateID() == cfg.id_rose_free
			|| pIvtrSrc->GetTemplateID() == cfg.id_rose_money)
		{
			DlgFlowers* pDlg = (DlgFlowers*)GetGameUIMan()->GetDialog("Win_Rose");
			if (pDlg)
			{
				tm tmNow = GetGame()->GetServerLocalTime();
				if (checkFlowerTime(tmNow))
				{
					if(GetHostPlayer()->GetSrcServerID() == 0)
					{
						pDlg->Show(true);	
						pDlg->UpdateUI(0,_AL(""));
					}
				}

			}
		}
		else if (pIvtrSrc->GetTemplateID() == cfg.id_rune2013_fragment[0] || pIvtrSrc->GetTemplateID() == cfg.id_rune2013_fragment[1])
		{
			CDlgRoleRuneCombine* pDlg = (CDlgRoleRuneCombine*)GetGameUIMan()->GetDialog("Win_Equip_Rune_Combine");
			if (pDlg)
				pDlg->Show(true);
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_RUNE2013_ITEM)
		{
			CDlgRoleRuneEnchanting* pDlg = (CDlgRoleRuneEnchanting*)GetGameUIMan()->GetDialog("Win_Equip_Rune_Enchanting");
			if (pDlg)
				pDlg->EnchantingRune(GetFirstItem() + iSrc);
		}
		else if (pIvtrSrc->GetClassID() == CECIvtrItem::ICID_INDIVIDUALITY_ITEM)
		{
			// 打开界面
			DlgIndividualityColor* pDlg = (DlgIndividualityColor*)GetGameUIMan()->GetDialog("Win_Dye_Combine");
			DlgIndividualityReveal* pDlgReveal = (DlgIndividualityReveal*)GetGameUIMan()->GetDialog("Win_Dye_FashDress");
			if (pDlg && pDlgReveal)
			{
				if(pDlg->IsShow() != true)
				{
					if(pDlgReveal->IsShow() == true)
					{
						pDlgReveal->SetNameToPosItem("Item_01",pIvtrSrc,GetFirstItem() + iSrc,0,0);
						return;
					}
					else
					{
						pDlg->Show(true);
					}
				}
				pDlg->SetIndividualityItem(pIvtrSrc,GetFirstItem() + iSrc);
			}
		}
		else if(GetGameUIMan()->GetDialog("Win_Dye_FashDress")->IsShow())
		{
			if( pIvtrSrc->GetClassID() == CECIvtrItem::ICID_EQUIP && ((CECIvtrEquip*)pIvtrSrc)->IsFashion()&& ((CECIvtrEquip*)pIvtrSrc)->GetDBEssence()->color == 1)
			{
				DlgIndividualityReveal* pDlgReveal = (DlgIndividualityReveal*)GetGameUIMan()->GetDialog("Win_Dye_FashDress");
				if (pDlgReveal && pDlgReveal->IsShow() == true)
				{
					pDlgReveal->SetNameToPosItem("Item_00",pIvtrSrc,0,0,GetFirstItem() + iSrc);
					return;
				}
			}
		}
		else
			GetHostPlayer()->UseItemInPack(IVTRTYPE_PACK, GetFirstItem() + iSrc);
	}
}

void CDlgInventory::OnCommand_AutoArrangeItems(const char * szCommand)
{

	CECInventory* pPack = GetHostPlayer()->GetPack();
	int i;
	for(i = 0; i < pPack->GetSize(); i++)
	{
		if(pPack->GetItem(i))
		{
			if(pPack->GetItem(i)->IsFrozen())
				return;
		}
	}
	for(i = 0; i < pPack->GetSize(); i++)
	{
		if(pPack->GetItem(i))
			pPack->GetItem(i)->Freeze(true);
	}
	int page = GetCheckedRadioButton(3);
	if(0 == stricmp(szCommand, "arrange"))
		GetGameSession()->c2s_CmdArrangeInventory(page);
	else
		GetGameSession()->c2s_CmdArrangeInventory(0);
	return;

	if(0 == stricmp(szCommand, "arrange"))
		m_bPageArrange = true;
	else 
		m_bPageArrange = false;

	// size < 2: No need to arrange
	if (!pPack || pPack->GetSize() == pPack->GetEmptySlotNum()) {
		GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3556), GP_CHAT_MISC);
		return;
	}
	LuaEvent::FirstNeatenPack();
	// Calc Exchanges
	int iRet = CalcArrangeExchanges();
	if (iRet == 0 && !m_aExchanges2Send.empty()) {
		int nPage = GetCheckedRadioButton(3) - 1;
		int nBegin = nPage * 48;
		int nEnd = (nPage + 1) * 48 - 1;
		int nPackSize = GetHostPlayer()->GetPack()->GetSize();
		if(nEnd >= nPackSize)
			nEnd = nPackSize - 1;
		// Freeze All Items
		for (i = 0; i < pPack->GetSize(); ++i) {
			if (pPack->GetItem(i))
			{
				if(m_bPageArrange)
				{
					if(i >= nBegin && i <= nEnd)
						pPack->GetItem(i)->Freeze(true);
				}
				else
					pPack->GetItem(i)->Freeze(true);
			}
		}
		m_bArranging = true;
		// Send Exchanges
		SendArrangeExchanges();
	}
	else {
		m_nSendInterval = 0;
		m_nCurSendNum = -1;

		if (iRet < 0)
			GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3557), GP_CHAT_MISC);
		else if (m_aExchanges2Send.empty())
			GetGameRun()->AddChatMessage(GetGameUIMan()->GetStringFromTable(3554), GP_CHAT_MISC);
	}
}

void CDlgInventory::OnCommand_MoveAllToPacket(const char * szCommand)
{
	int iNum = GetHostPlayer()->GetPocketPack()->GetSize() - GetHostPlayer()->GetPocketPack()->GetEmptySlotNum();
	if (GetHostPlayer()->GetPack()->GetEmptySlotNum()>=iNum)
	{
		GetGameSession()->c2s_CmdMovePocketAlltoIvtr();
	}
	else
		GetGameUIMan()->AddChatMessage(GetStringFromTable(7324), GP_CHAT_MISC);
}

enum SLOT_TYPE {
	TYPE_NORMAL = 0,
	TYPE_BLANK,
	TYPE_ORDER_NOT_DEFINED,
};
struct OrderIndex {
	BYTE iType;
	int iOrder;
	int iSlot;
};
class CompareIvtrShowOrder
{
public:
	bool operator ()(OrderIndex& o1, OrderIndex& o2) { return o1.iOrder < o2.iOrder; }
};
class CompareIvtrSlot
{
public:
	bool operator ()(OrderIndex& o1, OrderIndex& o2) { return o1.iSlot < o2.iSlot; }
};

int CDlgInventory::CalcArrangeExchanges()
{
	int i;
	CECHostPlayer* pHost = GetHostPlayer();
	CECInventory* pPack = pHost->GetPack();
	if (!pPack || pPack->GetSize() <= 0)
		return -1;

	int nPackSize = pPack->GetSize();
	int nPage = GetCheckedRadioButton(3) - 1;
	int nBegin = nPage * 48;
	int nEnd = (nPage + 1) * 48 - 1;
	if(nEnd >= nPackSize)
		nEnd = nPackSize - 1;
	int nPackItemNum;
	if(m_bPageArrange)
		nPackItemNum = nPackSize - pPack->GetEmptySlotNum(nBegin, nEnd);
	else
		nPackItemNum = nPackSize - pPack->GetEmptySlotNum();
	
	abase::vector<OrderIndex> Orders2OldSlots;
	Orders2OldSlots.reserve(nPackSize);

	int* aPreFrozenItemNum = new int[nPackSize];
	memset(aPreFrozenItemNum, 0, nPackSize * sizeof(aPreFrozenItemNum[0]));

	// 1. Get Order of each Slot.
	int nLastTotalFrozenItem = 0;
	const CECGameUIMan::IDOrderMap& mapIDOrder = GetGameUIMan()->GetIDOrderMap();
	for (i = 0; i < nPackSize; ++i)
	{
		OrderIndex oi;
		oi.iType = TYPE_NORMAL;
		oi.iSlot = i;
		oi.iOrder = INT_MAX;
		CECIvtrItem* pItem = pPack->GetItem(i);
		if((m_bPageArrange && i >= nBegin && i <= nEnd) || !m_bPageArrange)
		{
			if (pItem)
			{
				if (!pItem->IsFrozen())
				{
					CECGameUIMan::IDOrderMap::const_iterator it = mapIDOrder.find(pItem->GetTemplateID());
					if (it == mapIDOrder.end())
						oi.iType = TYPE_ORDER_NOT_DEFINED;
					else
						oi.iOrder = it->second;

					aPreFrozenItemNum[Orders2OldSlots.size()] = nLastTotalFrozenItem;
					Orders2OldSlots.push_back(oi);
				}
				else
				{
					// Frozen Item: Do Not Sort!
					++nLastTotalFrozenItem;
				}
			}
			else
			{
				oi.iType = TYPE_BLANK;
				aPreFrozenItemNum[Orders2OldSlots.size()] = nLastTotalFrozenItem;
				Orders2OldSlots.push_back(oi);
			}
		}
		else
		{
			// 非当前页的物品不参与排序
			++nLastTotalFrozenItem;
		}
	}

	// 2. Sort the order
	BubbleSort(Orders2OldSlots.begin(), Orders2OldSlots.end(), CompareIvtrShowOrder());

	// NewSlot for Ordered Array
	abase::vector<OrderIndex>& NewOrders2OldSlots = Orders2OldSlots;
	for (i = 0; i < (int)NewOrders2OldSlots.size(); ++i)
		NewOrders2OldSlots[i].iOrder = i + aPreFrozenItemNum[i];

	abase::vector<OrderIndex> OldSlots2NewOrders = NewOrders2OldSlots;
	BubbleSort(OldSlots2NewOrders.begin(), OldSlots2NewOrders.end(), CompareIvtrSlot());

	nLastTotalFrozenItem = 0;
	m_aPackItemFrozen.clear();
	for (i = 0; i < nPackSize; ++i) {
		CECIvtrItem* pItem = pPack->GetItem(i);
		if (pItem && pItem->IsFrozen() || ((m_bPageArrange && i < nBegin) || (m_bPageArrange && i > nEnd))) {
			aPreFrozenItemNum[i] = ++nLastTotalFrozenItem;
			m_aPackItemFrozen.push_back(1);
		}
		else {
			aPreFrozenItemNum[i] = nLastTotalFrozenItem;
			m_aPackItemFrozen.push_back(0);
		}
	}

#ifdef _DEBUG
	char sz[16];
	AString str, str1;
	CreateDirectoryA(LOCAL_DIR, NULL);
	str.Format("%s\\%d", LOCAL_DIR, pHost->GetCharacterID());
	CreateDirectoryA(str, NULL);
	AString file_name;
	file_name.Format("%s\\ArrangeIvtr.txt", str);
	AFile afile;
	bool bOpened = false;
	abase::vector<int> aIDs;
	aIDs.reserve(nPackSize);
	if (afile.Open(file_name, AFILE_CREATENEW|AFILE_TEXT|AFILE_TEMPMEMORY))
	{
		bOpened = true;
		afile.WriteLine("\nBefore Arrange: \n");
		str.Empty();
		for (i = 0; i < nPackSize; ++i)
		{
			CECIvtrItem* pItem = pPack->GetItem(i);
			sprintf(sz, "%8d ", pItem ? pItem->GetTemplateID() : 0);
			aIDs.push_back(pItem ? pItem->GetTemplateID() : 0);
			str += sz;
			if ((i+1) % 6 == 0) {
				afile.WriteLine(str);
				str.Empty();
			}
		}

		afile.WriteLine("NewOrders2OldSlots:");
		str.Empty();
		str1.Empty();
		for (i = 0; i < (int)NewOrders2OldSlots.size(); ++i) {
			sprintf(sz, "%2d ", NewOrders2OldSlots[i].iOrder);
			str += sz;
			str1 += "-----";
		}
		afile.WriteLine(str);
		afile.WriteLine(str1);
		str.Empty();
		for (i = 0; i < (int)NewOrders2OldSlots.size(); ++i) {
			sprintf(sz, "%2d ", NewOrders2OldSlots[i].iSlot);
			str += sz;
		}
		afile.WriteLine(str);

		afile.WriteLine("OldSlots2NewOrders:");
		str.Empty();
		str1.Empty();
		for (i = 0; i < (int)OldSlots2NewOrders.size(); ++i) {
			sprintf(sz, "%2d ", OldSlots2NewOrders[i].iOrder);
			str += sz;
			str1 += "-----";
		}
		afile.WriteLine(str);
		afile.WriteLine(str1);
		str.Empty();
		for (i = 0; i < (int)OldSlots2NewOrders.size(); ++i) {
			sprintf(sz, "%2d ", OldSlots2NewOrders[i].iSlot);
			str += sz;
		}
		afile.WriteLine(str);
	}
#endif

	// 3. Get Exchanges
	m_aExchanges2Send.clear();
	for (i = 0; i < (int)NewOrders2OldSlots.size(); ++i)
	{
		if ((int)m_aExchanges2Send.size() >= nPackItemNum - nLastTotalFrozenItem)
			break;

		// For each Slot that NOT FROZEN: Exchange the Correct-Order item to this slot
		while (NewOrders2OldSlots[i].iOrder != NewOrders2OldSlots[i].iSlot)
		{
			if ((int)m_aExchanges2Send.size() >= nPackItemNum - nLastTotalFrozenItem)
				// NO NEED to sort the rest items, they are either BLANK or ORDER-NOT-DEFINED items.
				break;

			// Get an Exchange
			int oldSlot = NewOrders2OldSlots[i].iSlot;
			CECIvtrItem* pItemSrc = pPack->GetItem(oldSlot);
			int newSlot = NewOrders2OldSlots[i].iOrder;
			CECIvtrItem* pItemDst = pPack->GetItem(newSlot);

			if (((pItemSrc && pItemSrc->IsFrozen()) || ((m_bPageArrange && oldSlot < nBegin) || (m_bPageArrange && oldSlot > nEnd))) || ((pItemDst && pItemDst->IsFrozen()) || ((m_bPageArrange && newSlot < nBegin) || (m_bPageArrange && newSlot > nEnd))))
			{
				ASSERT(0); 
				delete [] aPreFrozenItemNum;
				return -1;
			}

			BYTE iOldType = OldSlots2NewOrders[i].iType;
			BYTE iNewType = OldSlots2NewOrders[oldSlot - aPreFrozenItemNum[oldSlot]].iType;
			if (iOldType == TYPE_BLANK && iNewType == TYPE_BLANK ||
				iOldType == TYPE_ORDER_NOT_DEFINED && iNewType == TYPE_ORDER_NOT_DEFINED)
				// NO NEED to exchange: Both are BLANK or ORDER-NOT-DEFINED items
				break;

			ExchangeItem exchange = {newSlot, oldSlot};
			m_aExchanges2Send.push_back(exchange);

			// Swap oldSlot and newSlot:
			// In NewOrders2OldSlots
			NewOrders2OldSlots[i].iSlot = newSlot; // Slot[newSlot] Get the correct order
			int oldOrder = OldSlots2NewOrders[i].iOrder;
			int idx = oldOrder - aPreFrozenItemNum[oldOrder];
			NewOrders2OldSlots[idx].iSlot = oldSlot;
			// In OldSlots2NewOrders
			OldSlots2NewOrders[i].iOrder = newSlot;
			OldSlots2NewOrders[i].iType = iNewType;
			idx = oldSlot - aPreFrozenItemNum[oldSlot];
			OldSlots2NewOrders[idx].iOrder = oldOrder;
			OldSlots2NewOrders[idx].iType = iOldType;

#ifdef _DEBUG
			if (bOpened)
			{
				size_t j;
				str.Format("\n=== After Exchange#%d: (%d, %d) ===", m_aExchanges2Send.size(), oldSlot, newSlot);
				afile.WriteLine(str);
				afile.WriteLine("NewOrders2OldSlots:");
				str.Empty();
				str1.Empty();
				for (j = 0; j < NewOrders2OldSlots.size(); ++j) {
					sprintf(sz, "%2d ", NewOrders2OldSlots[j].iOrder);
					str += sz;
					str1 += "-----";
				}
				afile.WriteLine(str);
				afile.WriteLine(str1);
				str.Empty();
				for (j = 0; j < NewOrders2OldSlots.size(); ++j) {
					sprintf(sz, "%2d ", NewOrders2OldSlots[j].iSlot);
					str += sz;
				}
				afile.WriteLine(str);

				afile.WriteLine("OldSlots2NewOrders:");
				str.Empty();
				str1.Empty();
				for (j = 0; j < OldSlots2NewOrders.size(); ++j) {
					sprintf(sz, "%2d ", OldSlots2NewOrders[j].iOrder);
					str += sz;
					str1 += "-----";
				}
				afile.WriteLine(str);
				afile.WriteLine(str1);
				str.Empty();
				str1.Empty();
				for (j = 0; j < OldSlots2NewOrders.size(); ++j) {
					sprintf(sz, "%2d ", OldSlots2NewOrders[j].iSlot);
					str += sz;
					BYTE iType = OldSlots2NewOrders[j].iType;
					str1 += (iType == TYPE_NORMAL ? " A " : (iType == TYPE_BLANK ? " B " : " N "));
				}
				afile.WriteLine(str);
				afile.WriteLine(str1);

				int tmp = aIDs[oldSlot];
				aIDs[oldSlot] = aIDs[newSlot];
				aIDs[newSlot] = tmp;
				afile.WriteLine("\nIvtr IDs: \n");
				str.Empty();
				for (j = 0; j < aIDs.size(); ++j)
				{
					sprintf(sz, "%8d ", aIDs[j]);
					str += sz;
					if ((j+1) % 6 == 0) {
						afile.WriteLine(str);
						str.Empty();
					}
				}

				afile.Flush();
			}
#endif
		}
	}
	delete [] aPreFrozenItemNum;

#ifdef _DEBUG
	if (bOpened)
		afile.Close();
#endif

	return 0;
}

void CDlgInventory::SendArrangeExchanges()
{
	// Send the first Exchange:
	m_nCurSendNum = 0;
	GetGameSession()->c2s_CmdExgIvtrItem(
		m_aExchanges2Send[m_nCurSendNum].iSrc, 
		m_aExchanges2Send[m_nCurSendNum].iDst);
	++m_nCurSendNum;
	m_nLastSendTime = GetTickCount();
}

void CDlgInventory::SellConfirm(bool bYes)
{
	CECIvtrItem *pIvtrSrc = (CECIvtrItem*)m_pObjSell->GetDataPtr("ptr_CECIvtrItem");
	if (bYes)
	{
		CECDealInventory *pSell = GetHostPlayer()->GetSellPack();
		int iSrc = atoi(m_pObjSell->GetName() + strlen("Item_"));
		pSell->AddItem(pIvtrSrc, GetFirstItem() + iSrc, pIvtrSrc->GetCount());
		GetGameUIMan()->PlayItemSound(pIvtrSrc, true);
	}
	else
		pIvtrSrc->Freeze(false);
}

void CDlgInventory::EmphasizeItem(int id)
{
	CECInventory *pInventory = GetHostPlayer()->GetPack();
	int index = pInventory->FindItem(id);
	if (index<0)
	{
		return;
	}
	
	m_aItemEmphasize.push_back(index);
}

void CDlgInventory::EraseEmphasize(int index)
{
	abase::vector<int>::iterator it = m_aItemEmphasize.begin();
	while(it!=m_aItemEmphasize.end())
	{
		if (index==*it)
		{
			m_aItemEmphasize.erase(it);
			break;
		}
		it++;
	}
}

bool CDlgInventory::SetHelpItem(int id)
{
	m_iHelpItemId = id;
	if (id<0)
	{
		return true;
	}
	if (GetHostPlayer()->GetPack()->FindItem(id)>=0)
	{
		return true;
	}
	return false;
}
