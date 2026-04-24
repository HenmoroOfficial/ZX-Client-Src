// Filename	: DlgPetMain.cpp
// Creator	: Fu Chonggang
// Date		: 2012-6-20


#include "DlgPetMain.h"
#include "EC_GameUIMan.h"
#include "DlgShop.h"
#include "DlgShopExchange.h"
#include "EC_HostPlayer.h"
#include "EC_NewHelpGuide.h"

#define new A_DEBUG_NEW

AUI_BEGIN_COMMAND_MAP(CDlgPetMain, CDlgBase)

AUI_ON_COMMAND("Btn_PetTame",			OnCommandTame)
AUI_ON_COMMAND("Btn_PeUp",				OnCommandPetUp)
AUI_ON_COMMAND("Btn_PetRelease",		OnCommandRelease)
AUI_ON_COMMAND("Btn_PetEquipStr",		OnCommandEquipEnchase)
AUI_ON_COMMAND("Btn_PetItemExchange",	OnCommandShopExchange)
AUI_ON_COMMAND("Btn_PetShop",			OnCommandShop)
AUI_ON_COMMAND("Btn_PetLvup",			OnCommandFly)
AUI_ON_COMMAND("Btn_Close",				OnCommandCancel)
AUI_ON_COMMAND("IDCANCEL",				OnCommandCancel)

AUI_END_COMMAND_MAP()

CDlgPetMain::CDlgPetMain()
{
}

CDlgPetMain::~CDlgPetMain()
{
}

void CDlgPetMain::OnCommandTame(const char* szCommand)
{
	GetGameUIMan()->GetDialog("Win_Pet_Tame")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	Show(false);
	NewHelpGuide::StartGuide_Step_3(GUIDE_HELP_PET_11);
}

void CDlgPetMain::OnCommandPetUp(const char* szCommand)
{
	GetGameUIMan()->GetDialog("Win_Pet_Detail")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	Show(false);	
}

void CDlgPetMain::OnCommandRelease(const char* szCommand)
{
	GetGameUIMan()->GetDialog("Win_Pet_Release")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	Show(false);	
}

void CDlgPetMain::OnCommandEquipEnchase(const char* szCommand)
{
	GetGameUIMan()->GetDialog("Win_Pet_EquipEnhanse")->Show(true);	
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	Show(false);
}

void CDlgPetMain::OnCommandShopExchange(const char* szCommand)
{
	CDlgShopExchange* pShopExchange = (CDlgShopExchange*)GetGameUIMan()->GetDialog("Win_ShopExchange");
	pShopExchange->OnCommand_set("set1");
	pShopExchange->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->CheckRadioButton(2, 1);
	Show(false);
}

void CDlgPetMain::OnCommandShop(const char* szCommand)
{
	CDlgShop* pShop = (CDlgShop*)GetGameUIMan()->GetDialog("Win_Shop");
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	GetGameUIMan()->GetDialog("Win_Inventory")->CheckRadioButton(2, 1);
	
	NPC_ESSENCE *pEssence = GetGameUIMan()->GetNpcEssence();
	GetHostPlayer()->PrepareNPCService(pEssence->id_sell_service);
	pShop->UpdateShop(1);
	pShop->Show(true);
	Show(false);
}

void CDlgPetMain::OnCommandFly(const char* szCommand)
{
	GetGameUIMan()->GetDialog("Win_PetLevelUp")->Show(true);	
	GetGameUIMan()->GetDialog("Win_Inventory")->Show(true);
	Show(false);
}

void CDlgPetMain::OnCommandCancel(const char* szCommand)
{
	Show(false);
	GetGameUIMan()->EndNPCService();
}